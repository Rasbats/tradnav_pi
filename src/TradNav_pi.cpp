/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  tradnav Plugin
 * Author:   David Register, Mike Rossiter
 *
 ***************************************************************************
 *   Copyright (C) 2010 by David S. Register   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.             *
 ***************************************************************************
 */

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#include <wx/glcanvas.h>
#endif  // precompiled headers

#include <wx/fileconf.h>
#include <wx/stdpaths.h>

#include "ocpn_plugin.h"

#include "TradNav_pi.h"
#include "TradNavUIDialogBase.h"
#include "TradNavUIDialog.h"

// the class factories, used to create and destroy instances of the PlugIn

extern "C" DECL_EXP opencpn_plugin *create_pi(void *ppimgr) {
  return new tradnav_pi(ppimgr);
}

extern "C" DECL_EXP void destroy_pi(opencpn_plugin *p) { delete p; }

//---------------------------------------------------------------------------------------------------------
//
//    tradnav PlugIn Implementation
//
//---------------------------------------------------------------------------------------------------------

#include "icons.h"

//---------------------------------------------------------------------------------------------------------
//
//          PlugIn initialization and de-init
//
//---------------------------------------------------------------------------------------------------------

tradnav_pi::tradnav_pi(void *ppimgr)
  :opencpn_plugin_118(ppimgr) {
  // Create the PlugIn icons
  initialize_images();

  wxFileName fn;

  auto path = GetPluginDataDir("tradnav_pi");
  fn.SetPath(path);
  fn.AppendDir("data");
  fn.SetFullName("tradnav_panel_icon.png");

  path = fn.GetFullPath();

  wxInitAllImageHandlers();

  wxLogDebug(wxString("Using icon path: ") + path);
  if (!wxImage::CanRead(path)) {
    wxLogDebug("Initiating image handlers.");
    wxInitAllImageHandlers();
  }
  wxImage panelIcon(path);
  if (panelIcon.IsOk())
    m_panelBitmap = wxBitmap(panelIcon);
  else
    wxLogWarning("tradnav panel icon has NOT been loaded");

  m_bShowtradnav = false;
}

tradnav_pi::~tradnav_pi(void) {
  delete _img_tradnav_pi;
  delete _img_tradnav;
}

int tradnav_pi::Init(void) {
  AddLocaleCatalog(_T("opencpn-tradnav_pi"));

  // Set some default private member parameters
  m_tradnav_dialog_x = 0;
  m_tradnav_dialog_y = 0;
  m_tradnav_dialog_sx = 200;
  m_tradnav_dialog_sy = 400;
  m_ptradnavDialog = NULL;
  m_ptradnavOverlayFactory = NULL;

  ::wxDisplaySize(&m_display_width, &m_display_height);

  m_pconfig = GetOCPNConfigObject();

  //    And load the configuration items
  LoadConfig();

  err_msg = NULL;
  wxString sql;

  // Establish the location of the database file
  wxString dbpath;
  dbpath = StandardPath() + _T(DATABASE_NAME);
  // wxMessageBox(dbpath);
  bool newDB = !wxFileExists(dbpath);
  b_dbUsable = true;

  // void *cache;  // SOLUTION

  ret = sqlite3_open_v2(dbpath.mb_str(), &m_database,
                        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
  if (ret != SQLITE_OK) {
    wxLogMessage(_T("FINSAR_PI: cannot open '%s': %s\n"), DATABASE_NAME,
                 sqlite3_errmsg(m_database));
    sqlite3_close(m_database);
    b_dbUsable = false;
  }
  if (newDB && b_dbUsable) {
    // create empty db
    sql =
        "CREATE TABLE RTZ ("
        "route_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "route_name TEXT,"
        "created INTEGER,"
        "submitted INTEGER)";
    dbQuery(sql);

    Add_RTZ_db("-----");

    sql =
        "CREATE TABLE EXT ("
        "ext_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "extensions_file TEXT,"
        "route_name TEXT,"
        "rtz_date_stamp TEXT,"
        "created INTEGER,"
        "submitted INTEGER)";
    dbQuery(sql);
    wxDateTime date_stamp = wxDateTime::Now();
    date_stamp.MakeUTC(false);
    wxString dateLabel = date_stamp.Format(_T("%Y-%m-%d %H:%M:%S"));
    Add_EXT_db("-----.xml", "-----", dateLabel);
  }
  // Make the folder for the RTZ files
  wxString rtzpath;
  rtzpath = StandardPathRTZ();
  wxString extpath;
  extpath = StandardPathEXT();

 

  // Get a pointer to the opencpn display canvas, to use as a parent for the
  // tradnav dialog
  m_parent_window = GetOCPNCanvasWindow();

  //    This PlugIn needs a toolbar icon, so request its insertion if enabled
  //    locally
  if (m_btradnavShowIcon)
#ifdef ocpnUSE_SVG
    m_leftclick_tool_id = InsertPlugInToolSVG(
        _T("tradnav"), _svg_tradnav, _svg_tradnav,
        _svg_tradnav_toggled, wxITEM_CHECK, _("tradnav"), _T(""), NULL,
        tradnav_TOOL_POSITION, 0, this);
#else
    m_leftclick_tool_id = InsertPlugInTool(
        _T(""), _img_tradnav, _img_tradnav, wxITEM_CHECK,
        _("tradnav"), _T(""), NULL, tradnav_TOOL_POSITION, 0, this);
#endif

  return (WANTS_OVERLAY_CALLBACK | WANTS_OPENGL_OVERLAY_CALLBACK |
          WANTS_TOOLBAR_CALLBACK | WANTS_CURSOR_LATLON | INSTALLS_TOOLBAR_TOOL |
          WANTS_CONFIG | WANTS_ONPAINT_VIEWPORT | WANTS_PLUGIN_MESSAGING |
          WANTS_NMEA_EVENTS);
}

bool tradnav_pi::DeInit(void) {
  if (m_ptradnavDialog) {
    m_ptradnavDialog->Close();
    delete m_ptradnavDialog;
    m_ptradnavDialog = NULL;
  }

  delete m_ptradnavOverlayFactory;
  m_ptradnavOverlayFactory = NULL;

  return true;
}

int tradnav_pi::GetAPIVersionMajor() { return atoi(API_VERSION); }

int tradnav_pi::GetAPIVersionMinor() {
  std::string v(API_VERSION);
  size_t dotpos = v.find('.');
  return atoi(v.substr(dotpos + 1).c_str());
}

int tradnav_pi::GetPlugInVersionMajor() { return PLUGIN_VERSION_MAJOR; }

int tradnav_pi::GetPlugInVersionMinor() { return PLUGIN_VERSION_MINOR; }

int GetPlugInVersionPatch() { return PLUGIN_VERSION_PATCH; }
int GetPlugInVersionPost() { return PLUGIN_VERSION_TWEAK; }
const char *GetPlugInVersionPre() { return PKG_PRERELEASE; }
const char *GetPlugInVersionBuild() { return PKG_BUILD_INFO; }

wxBitmap *tradnav_pi::GetPlugInBitmap() { return &m_panelBitmap; }

wxString tradnav_pi::GetCommonName() { return PLUGIN_API_NAME; }

wxString tradnav_pi::GetShortDescription() { return PKG_SUMMARY; }

wxString tradnav_pi::GetLongDescription() { return PKG_DESCRIPTION; }

void tradnav_pi::SetDefaults(void) {}

int tradnav_pi::GetToolbarToolCount(void) { return 1; }

void tradnav_pi::OnToolbarToolCallback(int id) {
  if (!m_ptradnavDialog) {
    m_ptradnavDialog = new tradnavUIDialog(m_parent_window, this);
    wxPoint p = wxPoint(m_tradnav_dialog_x, m_tradnav_dialog_y);
    m_ptradnavDialog->pPlugIn = this;
    m_ptradnavDialog->Move(
        0,
        0);  // workaround for gtk autocentre dialog behavior
    m_ptradnavDialog->Move(p);

    // Clear route & mark manager
    auto uids = GetRouteGUIDArray();
    for (size_t i = 0; i < uids.size(); i++) {
      DeletePlugInRoute(uids[i]);
    }

    /*
    // Fill the choices
    wxArrayString items;
    if (m_ptradnavDialog->m_choiceObjectBearing) {
      int count = 359;
      for (int n = 0; n < count; n++) {
        items.Add(wxString::Format("%i",n));
      }
    }

    m_ptradnavDialog->m_choiceObjectBearing->Set(items);
*/   

    // Create the drawing factory
    m_ptradnavOverlayFactory =
        new tradnavOverlayFactory(*m_ptradnavDialog);
    m_ptradnavOverlayFactory->SetParentSize(m_display_width,
                                                m_display_height);

    wxMenu dummy_menu;
    m_position_menu_id = AddCanvasContextMenuItem(
        new wxMenuItem(&dummy_menu, -1, _("tradnav sets this range")), this);
    SetCanvasContextMenuItemViz(m_position_menu_id, true);
  }

  // Qualify the tradnav dialog position
  bool b_reset_pos = false;

#ifdef __WXMSW__
  //  Support MultiMonitor setups which an allow negative window positions.
  //  If the requested window does not intersect any installed monitor,
  //  then default to simple primary monitor positioning.
  RECT frame_title_rect;
  frame_title_rect.left = m_tradnav_dialog_x;
  frame_title_rect.top = m_tradnav_dialog_y;
  frame_title_rect.right = m_tradnav_dialog_x + m_tradnav_dialog_sx;
  frame_title_rect.bottom = m_tradnav_dialog_y + 30;

  if (NULL == MonitorFromRect(&frame_title_rect, MONITOR_DEFAULTTONULL))
    b_reset_pos = true;
#else
  //    Make sure drag bar (title bar) of window on Client Area of screen, with
  //    a little slop...
  wxRect window_title_rect;  // conservative estimate
  window_title_rect.x = m_tradnav_dialog_x;
  window_title_rect.y = m_tradnav_dialog_y;
  window_title_rect.width = m_tradnav_dialog_sx;
  window_title_rect.height = 30;

  wxRect ClientRect = wxGetClientDisplayRect();
  ClientRect.Deflate(
      60, 60);  // Prevent the new window from being too close to the edge
  if (!ClientRect.Intersects(window_title_rect)) b_reset_pos = true;

#endif

  if (b_reset_pos) {
    m_tradnav_dialog_x = 20;
    m_tradnav_dialog_y = 170;
    m_tradnav_dialog_sx = 300;
    m_tradnav_dialog_sy = 540;
  }

  // Toggle tradnav overlay display
  m_bShowtradnav = !m_bShowtradnav;

  //    Toggle dialog?
  if (m_bShowtradnav) {
    m_ptradnavDialog->Show();
  } else {
    m_ptradnavDialog->Hide();
  }

  // Toggle is handled by the toolbar but we must keep plugin manager b_toggle
  // updated to actual status to ensure correct status upon toolbar rebuild
  SetToolbarItemState(m_leftclick_tool_id, m_bShowtradnav);

  RequestRefresh(m_parent_window);  // refresh main window
}

void tradnav_pi::OntradnavDialogClose() {
  m_bShowtradnav = false;
  SetToolbarItemState(m_leftclick_tool_id, m_bShowtradnav);

  m_ptradnavDialog->Hide();

  SaveConfig();

  RequestRefresh(m_parent_window);  // refresh main window
}

int tradnav_pi::Add_RTZ_db(wxString route_name) {
  wxString sql = wxString::Format(
      "INSERT INTO RTZ (route_name, created, submitted) "
      "VALUES ('%s', current_timestamp, 0)",
      route_name.c_str());
  // wxMessageBox(sql);
  dbQuery(sql);
  return sqlite3_last_insert_rowid(m_database);
}

int tradnav_pi::GetRoute_Id(wxString route_name) {
  wxString rte = route_name;
  wxString sql1 = wxString::Format(
      "SELECT route_id FROM RTZ WHERE route_name = '%s'", route_name.c_str());

  wxString sql2 = " union all select 'Nothing' where not exists ";

  wxString sql3 = "(select route_id from RTZ where route_name = '" + rte + "')";

  wxString sql = sql1 + sql2 + sql3;
  // wxMessageBox(sql);
  return dbGetIntNotNullValue(sql);
}

wxString tradnav_pi::GetRTZDateStamp(wxString route_name) {
  char **result;
  int n_rows;
  int n_columns;
  char *measured;
  wxString rte = route_name;
  wxString sql = wxString::Format(
      "SELECT created FROM RTZ WHERE route_name = '%s'", rte.c_str());

  ret = sqlite3_get_table(m_database, sql.mb_str(), &result, &n_rows,
                          &n_columns, &err_msg);
  if (ret != SQLITE_OK) {
    /* some error occurred */
    wxLogMessage(_T("Spatialite SQL error: %s\n"), err_msg);
    sqlite3_free(err_msg);
    return "error";
  }
  for (int i = 1; i <= n_rows; i++) {
    measured = result[(i * n_columns) + 0];
  }

  dbFreeResults(result);
  wxString output = measured;
  // wxMessageBox(measured);
  return output;
}

void tradnav_pi::DeleteRTZ_Id(int id) {
  wxString sql;
  sql = wxString::Format("DELETE FROM RTZ WHERE route_id = %i", id);
  // wxMessageBox(sql);
  dbQuery(sql);
}

void tradnav_pi::DeleteRTZ_Name(wxString route_name) {
  wxString sql;
  sql = wxString::Format("DELETE FROM RTZ WHERE route_name = \'%s\'",
                         route_name.c_str());
  // wxMessageBox(sql);
  bool res = dbQuery(sql);
  if (res) {
    wxString del = "\"" + route_name + "\" has been deleted";
    // wxMessageBox(del, "Route Deleted");
  } else
    wxMessageBox("Error");
}

void tradnav_pi::DeleteEXT_Name(wxString route_name) {
  wxString sql;
  sql = wxString::Format("DELETE FROM EXT WHERE route_name = \'%s\'",
                         route_name.c_str());
  // wxMessageBox(sql);
  bool res = dbQuery(sql);
  if (res) {
    wxString del = "\"" + route_name + "\" has been deleted";
    // wxMessageBox(del, "Route Deleted");
  } else
    wxMessageBox("Error");
}

int tradnav_pi::Add_EXT_db(wxString extensions_file, wxString route_name,
                               wxString rtz_date_stamp) {
  wxString sql = wxString::Format(
      "INSERT INTO EXT (extensions_file, route_name, rtz_date_stamp, created, "
      "submitted) "
      "VALUES ('%s', '%s', '%s',current_timestamp, 0)",
      extensions_file.c_str(), route_name.c_str(), rtz_date_stamp.c_str());
  // wxMessageBox(sql);
  dbQuery(sql);
  return sqlite3_last_insert_rowid(m_database);
}

bool tradnav_pi::dbQuery(wxString sql) {
  if (!b_dbUsable) return false;
  ret = sqlite3_exec(m_database, sql.mb_str(), NULL, NULL, &err_msg);
  if (ret != SQLITE_OK) {
    wxMessageBox(err_msg);
    // some error occurred
    wxLogMessage(_T("Database error: %s in query: %s\n"), *err_msg,
                 sql.c_str());
    sqlite3_free(err_msg);
    b_dbUsable = false;
  }
  return b_dbUsable;
}

int tradnav_pi::dbGetIntNotNullValue(wxString sql) {
  char **result;
  int n_rows;
  int n_columns;
  dbGetTable(sql, &result, n_rows, n_columns);
  wxArrayString routes;
  int ret = atoi(result[1]);
  dbFreeResults(result);
  if (n_rows == 1)
    return ret;
  else
    return 0;
}

void tradnav_pi::dbGetTable(wxString sql, char ***results, int &n_rows,
                                int &n_columns) {
  ret = sqlite3_get_table(m_database, sql.mb_str(), results, &n_rows,
                          &n_columns, &err_msg);
  // wxMessageBox(err_msg);
  if (ret != SQLITE_OK) {
    wxLogMessage(_T("Database error: %s in query: %s\n"), *err_msg,
                 sql.c_str());
    sqlite3_free(err_msg);
    b_dbUsable = false;
  }
}

void tradnav_pi::dbFreeResults(char **results) {
  sqlite3_free_table(results);
}

wxString tradnav_pi::StandardPath() {
  wxString stdPath(*GetpPrivateApplicationDataLocation());
  wxString s = wxFileName::GetPathSeparator();

  stdPath += s + "plugins" + s + "finSAR";
  if (!wxDirExists(stdPath)) wxMkdir(stdPath);
  // stdPath = stdPath + s + "data";
  // if (!wxDirExists(stdPath)) wxMkdir(stdPath);

  stdPath += s;
  return stdPath;
}

wxString tradnav_pi::StandardPathRTZ() {
  wxString stdPath(*GetpPrivateApplicationDataLocation());
  wxString s = wxFileName::GetPathSeparator();

  stdPath += s + "plugins" + s + "finSAR" + s + "RTZ";
  if (!wxDirExists(stdPath)) wxMkdir(stdPath);

  stdPath += s;
  return stdPath;
}

wxString tradnav_pi::StandardPathEXT() {
  wxString stdPath(*GetpPrivateApplicationDataLocation());
  wxString s = wxFileName::GetPathSeparator();

  stdPath += s + "plugins" + s + "finSAR" + s + "EXT";
  if (!wxDirExists(stdPath)) wxMkdir(stdPath);

  stdPath += s;
  return stdPath;
}

bool tradnav_pi::RenderOverlay(wxDC &dc, PlugIn_ViewPort *vp) {
  if (!m_ptradnavDialog || !m_ptradnavDialog->IsShown() ||
      !m_ptradnavOverlayFactory)
    return false;

  if (m_ptradnavDialog) {
    m_ptradnavDialog->SetViewPort(vp);
    m_ptradnavDialog->MakeBoxPoints();
  }

  piDC pidc(dc);

  m_ptradnavOverlayFactory->RenderOverlay(pidc, *vp);
  return true;
}

bool tradnav_pi::RenderGLOverlay(wxGLContext *pcontext,
                                     PlugIn_ViewPort *vp) {
  if (!m_ptradnavDialog || !m_ptradnavDialog->IsShown() ||
      !m_ptradnavOverlayFactory)
    return false;

  if (m_ptradnavDialog) {
    m_ptradnavDialog->SetViewPort(vp);
    m_ptradnavDialog->MakeBoxPoints();
  }

  piDC piDC;
  glEnable(GL_BLEND);
  piDC.SetVP(vp);

  m_ptradnavOverlayFactory->RenderOverlay(piDC, *vp);
  return true;
}

void tradnav_pi::SetPositionFix(PlugIn_Position_Fix &pfix) {
  m_ship_lon = pfix.Lon;
  m_ship_lat = pfix.Lat;
  // std::cout<<"Ship--> Lat: "<<m_ship_lat<<" Lon: "<<m_ship_lon<<std::endl;
  //}
}
bool tradnav_pi::LoadConfig(void) {
  wxFileConfig *pConf = (wxFileConfig *)m_pconfig;

  if (!pConf) return false;

  pConf->SetPath(_T( "/PlugIns/tradnav" ));

  m_CopyFolderSelected = pConf->Read(_T( "tradnavFolder" ));

  if (m_CopyFolderSelected == wxEmptyString) {
    wxString g_SData_Locn = *GetpSharedDataLocation();
    // Establish location of Tide and Current data
    pTC_Dir = new wxString(_T("tcdata"));
    pTC_Dir->Prepend(g_SData_Locn);

    m_CopyFolderSelected = *pTC_Dir;
  }

  m_tradnav_dialog_sx = pConf->Read(_T( "tradnavDialogSizeX" ), 300L);
  m_tradnav_dialog_sy = pConf->Read(_T( "tradnavDialogSizeY" ), 540L);
  m_tradnav_dialog_x = pConf->Read(_T( "tradnavDialogPosX" ), 20L);
  m_tradnav_dialog_y = pConf->Read(_T( "tradnavDialogPosY" ), 170L);

  return true;
}

bool tradnav_pi::SaveConfig(void) {
  wxFileConfig *pConf = (wxFileConfig *)m_pconfig;

  if (!pConf) return false;

  pConf->SetPath(_T( "/PlugIns/tradnav" ));

  pConf->Write(_T( "tradnavFolder" ), m_CopyFolderSelected);

  pConf->Write(_T( "tradnavDialogSizeX" ), m_tradnav_dialog_sx);
  pConf->Write(_T( "tradnavDialogSizeY" ), m_tradnav_dialog_sy);
  pConf->Write(_T( "tradnavDialogPosX" ), m_tradnav_dialog_x);
  pConf->Write(_T( "tradnavDialogPosY" ), m_tradnav_dialog_y);

  return true;
}

void tradnav_pi::SetColorScheme(PI_ColorScheme cs) {
  DimeWindow(m_ptradnavDialog);
}

void tradnav_pi::OnContextMenuItemCallback(int id) {
  if (!m_ptradnavDialog) return;

  if (id == m_position_menu_id) {
    m_cursor_lat = GetCursorLat();
    m_cursor_lon = GetCursorLon();
    m_ptradnavDialog->OnContextMenu(m_cursor_lat, m_cursor_lon);
  }
}

void tradnav_pi::SetCursorLatLon(double lat, double lon) {
  m_cursor_lat = lat;
  m_cursor_lon = lon;
}

void tradnav_pi::SetPluginMessage(wxString &message_id,
                                  wxString &message_body) {

  if (message_id == _T("WMM_VARIATION_BOAT")) {
    Json::CharReaderBuilder builder;
    Json::CharReader *reader = builder.newCharReader();

    Json::Value value;
    string errors;

    bool parsingSuccessful = reader->parse(
        message_body.c_str(), message_body.c_str() + message_body.size(),
        &value, &errors);
    delete reader;

    if (!parsingSuccessful) {
      wxLogMessage("WMM_VARIATION error");
      return;
    }

    // get the DECL value from the JSON message
    double decl = value["Decl"].asDouble();  

    g_dVar = decl;
  }
}