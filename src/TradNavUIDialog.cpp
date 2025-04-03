/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  tradnav Object
 * Author:   Mike Rossiter
 *
 ***************************************************************************
 *   Copyright (C) 2016 by Mike Rossiter  *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.         *
 ***************************************************************************
 *
 */
#include <wx/intl.h>
#include "wx/wx.h"

#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/debug.h>
#include <wx/graphics.h>
#include <wx/stdpaths.h>

#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "TradNav_pi.h"
#include "icons.h"
#include <wx/arrimpl.cpp>

#ifdef __WXMSW__
#include <windows.h>
#endif
#include <memory.h>

#include <wx/colordlg.h>
#include <wx/event.h>
#include "pugixml.hpp"

class Position;

using namespace std;
using namespace pugi;

#define FAIL(X)  \
  do {           \
    error = X;   \
    goto failed; \
  } while (0)

static double deg2rad(double degrees) { return M_PI * degrees / 180.0; }
static double rad2deg(double radians) { return 180.0 * radians / M_PI; }

GetRouteDialog::GetRouteDialog(wxWindow* parent, wxWindowID id,
                               const wxString& title, const wxPoint& position,
                               const wxSize& size, long style)
    : wxDialog(parent, id, title, position, size, style) {
  wxString dimensions = wxT(""), s;
  wxPoint p;
  wxSize sz;

  sz.SetWidth(size.GetWidth() - 20);
  sz.SetHeight(size.GetHeight() - 70);

  p.x = 6;
  p.y = 2;

  wxBoxSizer* bSizer1;
  bSizer1 = new wxBoxSizer(wxVERTICAL);

  dialogText =
      new wxListView(this, wxID_ANY, p, sz, wxLC_NO_HEADER | wxLC_REPORT,
                     wxDefaultValidator, wxT(""));
  wxFont font(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);

  dialogText->SetFont(font);

  bSizer1->Add(dialogText, 0, wxALL, 5);

  m_sdbSizer1 = new wxStdDialogButtonSizer();
  m_sdbSizer1OK = new wxButton(this, wxID_OK);
  m_sdbSizer1->AddButton(m_sdbSizer1OK);
  m_sdbSizer1Cancel = new wxButton(this, wxID_CANCEL);
  m_sdbSizer1->AddButton(m_sdbSizer1Cancel);
  m_sdbSizer1->Realize();

  bSizer1->Add(m_sdbSizer1, 1, wxALIGN_CENTER, 5);

  m_sdbSizer1OK->SetDefault();

  this->SetSizer(bSizer1);
  this->Layout();
  bSizer1->Fit(this);
}

GetRouteDialog::~GetRouteDialog() {}

tradnavUIDialog::tradnavUIDialog(wxWindow* parent, tradnav_pi* ppi)
    : tradnavUIDialogBase(parent) {
  pParent = parent;
  pPlugIn = ppi;
  m_bIdentify = false;
  m_bEBL = false;
  m_bRunningFix = false;

  wxFileConfig* pConf = GetOCPNConfigObject();

  if (pConf) {
    pConf->SetPath(_T ( "/PlugIns/tradnav" ));

    pConf->Read(_T("tradnavFolder"), &m_FolderSelected);
  }

  // m_dirPicker1->GetTextCtrlValue();

  // wxMessageBox(m_FolderSelected);

  this->Connect(wxEVT_MOVE, wxMoveEventHandler(tradnavUIDialog::OnMove));

  m_dtNow = wxDateTime::Now();
  m_dtNow.MakeUTC(false);
  wxString initStartDate = m_dtNow.Format(_T("%Y-%m-%d %H:%M:%S"));

  Fit();
  SetMinSize(GetBestSize());

  for (int z = 0; z < 10; z++) {
    for (int fz = 0; fz < 10; fz++) {
      my_files[z][fz].Add("0");
    }
  }

  id_wpt = wxEmptyString;
  ebl_range = 5.;
}

tradnavUIDialog::~tradnavUIDialog() {
  wxFileConfig* pConf = GetOCPNConfigObject();
  ;

  if (pConf) {
    pConf->SetPath(_T ( "/PlugIns/tradnav" ));
  }

  i_vector.clear();
  r_vector.clear();
  d_vector.clear();
}

void tradnavUIDialog::SetViewPort(PlugIn_ViewPort* vp) {
  if (m_vp == vp) return;

  m_vp = new PlugIn_ViewPort(*vp);
  m_bMoveUpDownLeftRight = false;
  centreLat = m_vp->clat;
  centreLon = m_vp->clon;
  ppm = vp->view_scale_ppm;
}

void tradnavUIDialog::OnClose(wxCloseEvent& event) {
  pPlugIn->OntradnavDialogClose();

  i_vector.clear();
  r_vector.clear();
  d_vector.clear();

  DeleteChartedRoute();
}

void tradnavUIDialog::OnMove(wxMoveEvent& event) {
  //    Record the dialog position
  wxPoint p = GetPosition();
  pPlugIn->SettradnavDialogX(p.x);
  pPlugIn->SettradnavDialogY(p.y);

  event.Skip();
}

void tradnavUIDialog::OnSize(wxSizeEvent& event) {
  //    Record the dialog size
  wxSize p = event.GetSize();
  pPlugIn->SettradnavDialogSizeX(p.x);
  pPlugIn->SettradnavDialogSizeY(p.y);

  event.Skip();
}

wxString tradnavUIDialog::MakeDateTimeLabel(wxDateTime myDateTime) {
  wxDateTime dt = myDateTime;

  wxString s2 = dt.Format("%Y-%m-%d");
  wxString s = dt.Format("%H:%M:%S");
  wxString dateLabel = s2 + " " + s;

  return dateLabel;
}

void tradnavUIDialog::OnInformation(wxCommandEvent& event) {}

void tradnavUIDialog::AddChartRoute(wxString myRoute) {}

int tradnavUIDialog::GetRandomNumber(int range_min, int range_max) {
  long u = (long)wxRound(
      ((double)rand() / ((double)(RAND_MAX) + 1) * (range_max - range_min)) +
      range_min);
  return (int)u;
}

void tradnavUIDialog::SaveIndexRangeDirection(wxString route_name,
                                              wxString date_stamp) {
  // Create Main level XML container
  xml_document xmlDoc;

  auto declarationNode = xmlDoc.append_child(node_declaration);

  declarationNode.append_attribute("version") = "1.0";

  declarationNode.append_attribute("encoding") = "UTF-8";

  // Create XML root node called extensions
  xml_node pRoot = xmlDoc.append_child("extensions");

  int idn = 0;
  wxString route;

  xml_node m_rootNode = pRoot.append_child("route");
  m_rootNode.append_attribute("route_name").set_value(route_name.mb_str());
  m_rootNode.append_attribute("date_stamp").set_value(date_stamp.mb_str());

  xml_node m_targetNode = m_rootNode.append_child("targets");

  for (std::vector<IndexTarget>::iterator itOut = i_vector.begin();
       itOut != i_vector.end(); itOut++) {
    xml_node m_targetpoint = m_targetNode.append_child("index_target");

    // wxString myIdn = wxString::Format("%i", idn);
    m_targetpoint.append_attribute("wp_id").set_value((*itOut).wpId.mb_str());
    wxString wpid = (*itOut).wpId.mb_str();
    // wxMessageBox(wpid);

    xml_node b_position = m_targetpoint.append_child("begin");

    wxString bLat = wxString::Format("%f", (*itOut).beginLat);
    wxString bLon = wxString::Format("%f", (*itOut).beginLon);

    b_position.append_attribute("lat").set_value(bLat.mb_str());
    b_position.append_attribute("lon").set_value(bLon.mb_str());

    xml_node e_position = m_targetpoint.append_child("end");

    wxString eLat = wxString::Format("%f", (*itOut).endLat);
    wxString eLon = wxString::Format("%f", (*itOut).endLon);

    e_position.append_attribute("lat").set_value(eLat.mb_str());
    e_position.append_attribute("lon").set_value(eLon.mb_str());

    xml_node idistance = m_targetpoint.append_child("index_distance");
    wxString dist = wxString::Format("%f", (*itOut).distance);
    idistance.append_attribute("distance").set_value(dist);

    xml_node label = m_targetpoint.append_child("label");

    wxString llat = wxString::Format("%f", (*itOut).label_lat);
    wxString llon = wxString::Format("%f", (*itOut).label_lon);

    label.append_attribute("label_lat").set_value(llat.mb_str());
    label.append_attribute("label_lon").set_value(llon.mb_str());

    idn++;
  }

  for (std::vector<RangeTarget>::iterator itOutRange = r_vector.begin();
       itOutRange != r_vector.end(); itOutRange++) {
    xml_node r_targetpoint = m_targetNode.append_child("range_target");

    // wxString myIdn = wxString::Format("%i", idn);
    r_targetpoint.append_attribute("wp_id").set_value(
        (*itOutRange).wpId.mb_str());
    wxString wpid = (*itOutRange).wpId.mb_str();
    // wxMessageBox(wpid);

    xml_node b_position = r_targetpoint.append_child("begin");

    wxString bLat = wxString::Format("%f", (*itOutRange).beginLat);
    wxString bLon = wxString::Format("%f", (*itOutRange).beginLon);

    b_position.append_attribute("lat").set_value(bLat.mb_str());
    b_position.append_attribute("lon").set_value(bLon.mb_str());

    xml_node e_position = r_targetpoint.append_child("end");

    wxString eLat = wxString::Format("%f", (*itOutRange).endLat);
    wxString eLon = wxString::Format("%f", (*itOutRange).endLon);

    e_position.append_attribute("lat").set_value(eLat.mb_str());
    e_position.append_attribute("lon").set_value(eLon.mb_str());

    xml_node idistance = r_targetpoint.append_child("range_distance");
    wxString dist = wxString::Format("%f", (*itOutRange).distance);
    idistance.append_attribute("distance").set_value(dist);

    xml_node label = r_targetpoint.append_child("label");

    wxString ldist = wxString::Format("%f", (*itOutRange).label_lat);
    wxString ldir = wxString::Format("%f", (*itOutRange).label_lon);

    label.append_attribute("label_lat").set_value(ldist.mb_str());
    label.append_attribute("label_lon").set_value(ldir.mb_str());

    idn++;
  }

  for (std::vector<DirectionTarget>::iterator itOutDirection = d_vector.begin();
       itOutDirection != d_vector.end(); itOutDirection++) {
    xml_node d_directionpoint = m_targetNode.append_child("direction_target");

    wxString dLat = wxString::Format("%f", (*itOutDirection).m_lat);
    d_directionpoint.append_attribute("direction_lat").set_value(dLat);
    ;

    wxString dlon = wxString::Format("%f", (*itOutDirection).m_lon);
    d_directionpoint.append_attribute("direction_lon").set_value(dlon);

    wxString dir = wxString::Format("%f", (*itOutDirection).m_dir);
    d_directionpoint.append_attribute("direction").set_value(dir);

    idn++;
  }

  // done adding index, range and direction targets
  // Write xmlDoc into a file
  wxString file_name = route_name + ".xml";
  wxString file_path = pPlugIn->StandardPathEXT() + file_name;
  // Route name must be the same as the file name, without file extension

  xmlDoc.save_file(file_path.mb_str());
}

void tradnavUIDialog::SelectRoutePoints(wxString routeName) {}

wxString tradnavUIDialog::SelectRoute(bool isDR) { return ""; }

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*:: This function converts decimal degrees to radians :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
double tradnavUIDialog::deg2rad(double deg) { return (deg * pi / 180); }
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*:: This function converts radians to decimal degrees :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
double tradnavUIDialog::rad2deg(double rad) { return (rad * 180 / pi); };

struct xml_string_writer : pugi::xml_writer {
  std::string result;

  virtual void write(const void* data, size_t size) {
    result.append(static_cast<const char*>(data), size);
  }
};

//////////////////////////////////////////
//////////////////////////////////////////
//////////////////////////////////////////

std::string InnerXML(pugi::xml_node target) {
  xml_string_writer writer;
  for (pugi::xml_node child = target.first_child(); child;
       child = child.next_sibling())
    child.print(writer, "");
  return writer.result;
}

void tradnavUIDialog::AddTestItems(wxCommandEvent& event) {
  wxString filename;
  wxFileDialog dlg(this, "Select file", wxEmptyString, wxEmptyString,
                   "RTZ files(*.rtz) | *.*;*.*", wxFD_OPEN);
  if (dlg.ShowModal() == wxID_OK) {
    if (dlg.GetPath() != wxEmptyString) {
      filename = dlg.GetPath();
      // wxMessageBox(filename);
    }

  } else
    wxMessageBox(_("No file entered"));

  ca = 0;
  cf = 0;

  pugi::xml_document xmlDoc;
  pugi::xml_parse_result result = xmlDoc.load_file(filename.mb_str());

  // pugi::xml_node pRoot = xmlDoc.child("Areas");
  // if (pRoot == nullptr) return;

  // pugi::xml_node pAreasElement = xmlDoc.child("Areas");
  string area_name;
  pugi::xml_node nodes = xmlDoc.child("Areas");
  for (pugi::xml_node node = nodes.first_child(); node;
       node = node.next_sibling()) {
    if (!strcmp(node.name(), "Area")) {
      pugi::xml_node name = node.first_child();
      // pugi::xml_node name = node.child("Name");

      if (!strcmp(name.name(), "Name")) {
        string area_name = InnerXML(node.child("Name"));
        // wxMessageBox(area_name);
        my_areas[ca].Add(area_name);
        ca++;
      }
      pugi::xml_node routes = node.first_child().next_sibling();

      if (!strcmp(routes.name(), "Routes")) {
        // wxMessageBox("here");

        pugi::xml_node rnode = routes.first_child();
        if (!strcmp(rnode.name(), "file")) {
          cf = 0;
          for (pugi::xml_node child = routes.first_child(); child;
               child = child.next_sibling()) {
            string f = InnerXML(child);
            my_files[ca][cf].Add(f, 1);
            // wxMessageBox(my_files[ca][cf].Item(1));

            cf++;
          }
        }
        /*
          pugi::xml_node files;

          for (pugi::xml_node files = routes.first_child(); routes;
               files = routes.next_sibling()) {
            string file_name = InnerXML(routes.child("file");
            wxMessageBox(file_name);
          }*/
      }
    }
  }
  wxString testca = wxString::Format("%i", ca);
  wxString testcf = wxString::Format("%i", cf);
}

int tradnavUIDialog::GetScale(double myChartScale) {
  // If myChartScale is not exactly as shown in OpenCPN get the best scale to
  // use.

  if (myChartScale < 1200) {
    myChartScale = 800;
  }

  else if (myChartScale >= 1200 && myChartScale < 2400) {
    myChartScale = 1600;
  }

  else if (myChartScale >= 2400 && myChartScale < 4800) {
    myChartScale = 3200;
  }

  else if (myChartScale >= 4800 && myChartScale < 9600) {
    myChartScale = 6400;
  }

  else if (myChartScale >= 9600 && myChartScale < 19200) {
    myChartScale = 12800;
  }

  else if (myChartScale >= 19200 && myChartScale < 38400) {
    myChartScale = 25600;
  }

  else if (myChartScale >= 38400 && myChartScale < 76800) {
    myChartScale = 51200;
  }

  else if (myChartScale >= 76800 && myChartScale < 153550) {
    myChartScale = 102400;
  }

  else if (myChartScale >= 153550 && myChartScale < 306850) {
    myChartScale = 204700;
  }

  else if (myChartScale >= 306850 && myChartScale < 613700) {
    myChartScale = 409000;
  }

  else if (myChartScale >= 613700 && myChartScale < 1227400) {
    myChartScale = 818000;
  }

  else if (myChartScale >= 1227400 && myChartScale < 2454800) {
    myChartScale = 1636000;
  }

  else if (myChartScale >= 2454800 && myChartScale < 4909600) {
    myChartScale = 3272000;
  }

  else
    myChartScale = 9999000;

  // Use the modified scale to find the correct zoom level
  // Would switch case be quicker???

  if (myChartScale == 800) {
    return 19;
  }

  else if (myChartScale == 1600) {
    return 18;
  }

  else if (myChartScale == 3200) {
    return 17;
  }

  else if (myChartScale == 6400) {
    return 16;
  }

  else if (myChartScale == 12800) {
    return 15;
  }

  else if (myChartScale == 25600) {
    return 14;
  }

  else if (myChartScale == 51200) {
    return 13;
  }

  else if (myChartScale == 102400) {
    return 12;
  }

  else if (myChartScale == 204700) {
    return 11;
  } else if (myChartScale == 409000) {
    return 10;
  }

  else if (myChartScale == 818000) {
    return 9;
  }

  else if (myChartScale == 1636000) {
    return 8;
  }

  else if (myChartScale == 3272000) {
    return 7;
  }

  else
    return 6;
}

void tradnavUIDialog::key_shortcut(wxKeyEvent& event) {
  // wxMessageBox("here");
  //  of course, it doesn't have to be the control key. You can use others:
  //  http://docs.wxwidgets.org/stable/wx_wxkeyevent.html
  if (event.GetModifiers() == wxMOD_CONTROL) {
    switch (event.GetKeyCode()) {
      case 'S':  // can return the upper ASCII value of a key
        // do whatever you like for a Ctrl+G event here!
        // wxMessageBox(_("CTRL+G") );
        // OnCursor();
        break;  /*
      case WXK_LEFT: // we also have special keycodes for non-ascii values.
          // get a full list of special keycodes here:
          // http://docs.wxwidgets.org/stable/wx_keycodes.html
          wxMessageBox(_("CTRL+Left") );
          break;*/
      default:  // do nothing
        break;
    }
  }
  event.Skip();
}

void tradnavUIDialog::MakeBoxPoints() {
  myZoom = GetScale(m_vp->chart_scale);

  double boxlat = m_vp->clat;
  double boxlon = m_vp->clon;

  double mpp;
  mpp = 156543.03392 * cos(boxlat * PI / 180) / pow(2, myZoom);

  mpp *= 640;   // Reference box is 640x640
  mpp /= 1852;  // metres to NM

  double halfbox = mpp / 60 / 2;  // decimal degree

  double centreC = boxlat;
  double dlat1, dlat2;

  dlat1 = centreC + halfbox;
  dlat2 = centreC - halfbox;

  int pixtopbottom = m_vp->pix_height / 4;
  int pixleftright = m_vp->pix_height / 4;

  double elat, elon;
  ll_gc_ll(dlat2, boxlon, 0, 2 * halfbox, &elat, &elon);

  wxPoint r1;
  GetCanvasPixLL(m_vp, &r1, elat, elon);

  wxPoint ib;
  GetCanvasPixLL(m_vp, &ib, dlat1, boxlon);

  double lpp =
      sqrt(pow((double)(ib.x - r1.x), 2) + pow((double)(ib.y - r1.y), 2));
  int pixheight = (int)lpp;
  pixheight /= 2;

  myPixArray[0].x = 0;
  myPixArray[0].y = pixtopbottom;
  myPixArray[0].y *= -1;

  myPixArray[1].x = 0;
  myPixArray[1].y = pixtopbottom;

  myPixArray[2].x = 0;
  myPixArray[2].y = 0;

  myPixArray[3].x = pixleftright;
  myPixArray[3].x *= -1;
  myPixArray[3].y = 0;

  myPixArray[4].x = pixleftright;
  myPixArray[4].y = 0;

  myPixHeight = pixheight * 2;
}

void tradnavUIDialog::OnNewRoute(wxCommandEvent& event) {
  // This sleep is needed to give the time for the currently pressed modifier
  // keys, if any, to be released. Notice that Control modifier could well be
  // pressed if this command was activated from the menu using accelerator
  // and keeping it pressed would totally derail the test below, e.g. "A" key
  // press would actually become "Ctrl+A" selecting the entire text and so on.

  // Remove any indexes from previous route
  i_vector.clear();
  r_vector.clear();
  d_vector.clear();

  auto uids = GetRouteGUIDArray();
  for (size_t i = 0; i < uids.size(); i++) {
    DeletePlugInRoute(uids[i]);
  }

  RequestRefresh(pParent);

  wxMessageBox(
      "Right-Click \"End Route\" after drawing the route.\nRight-Click on the "
      "route\nPress CTRL+R and enter route name in route properties");

  pParent->SetFocus();
  wxUIActionSimulator sim;
  sim.KeyDown(82, wxMOD_CONTROL);

  // sim.KeyUp(82, wxMOD_CONTROL);
}

void tradnavUIDialog::WriteRTZ(wxString route_name) {
  // Select the route from the route table
  //
  // Create Main level XML container
  xml_document xmlDoc;

  auto declarationNode = xmlDoc.append_child(node_declaration);

  declarationNode.append_attribute("version") = "1.0";

  declarationNode.append_attribute("encoding") = "UTF-8";

  const char* value = "http://www.cirm.org/RTZ/1/2";

  // Create XML root node called route
  xml_node pRoot = xmlDoc.append_child("route");

  pRoot.append_attribute("xmlns").set_value(value);
  pRoot.append_attribute("xmlns:xsi")
      .set_value("http://www.w3.org/2001/XMLSchema-instance");

  pRoot.append_attribute("version").set_value("1.2");

  // ************* Add routeInfo to root node *******

  xml_node routeInfo = pRoot.append_child("routeInfo");
  routeInfo.append_attribute("routeName").set_value(route_name.mb_str());

  // Insert cat's name as first child of animal

  // ************* Add waypoints *******
  xml_node waypoints = pRoot.append_child("waypoints");

  for (std::vector<PlugIn_Waypoint_Ex*>::iterator itOut = theWaypoints.begin();
       itOut != theWaypoints.end(); itOut++) {
    xml_node m_waypoint = waypoints.append_child("waypoint");

    m_waypoint.append_attribute("id").set_value((*itOut)->m_MarkName.mb_str());
    m_waypoint.append_attribute("name").set_value(
        (*itOut)->m_MarkName.mb_str());
    m_waypoint.append_attribute("revision").set_value("0");

    xml_node position = m_waypoint.append_child("position");
    double dLat = (*itOut)->m_lat;
    double dLon = (*itOut)->m_lon;
    wxString sLat = wxString::Format("%f", dLat);
    wxString sLon = wxString::Format("%f", dLon);

    position.append_attribute("lat").set_value(sLat);
    position.append_attribute("lon").set_value(sLon);
  }

  wxString file_name = route_name + ".rtz";
  wxString file_path = pPlugIn->StandardPathRTZ() + file_name;

  // Route name must be the same as the file name, without file extension

  xmlDoc.save_file(file_path.mb_str());
}

void tradnavUIDialog::WriteEXT(wxString route_name) {
  // Select the route from the route table
  //
  // Create Main level XML container
  xml_document xmlDoc;

  // Create XML root node called extensions

  xml_node pRoot = xmlDoc.append_child("extensions");

  // ************* Add extensionsInfo to root node *******

  xml_node extensionsInfo = pRoot.append_child("route");

  m_dateStamp = pPlugIn->GetRTZDateStamp(route_name);
  extensionsInfo.append_attribute("route_name").set_value(route_name.mb_str());
  extensionsInfo.append_attribute("date_stamp").set_value(m_dateStamp.mb_str());

  // done adding data
  // Write xmlDoc into an extensions file

  wxString file_name = route_name + ".xml";
  wxString file_path = pPlugIn->StandardPathEXT() + file_name;

  // wxMessageBox(file_path);

  // Route name must be the same as the file name, without file extension

  xmlDoc.save_file(file_path.mb_str());
}

void tradnavUIDialog::DeleteEXTFile(wxString route_name) {
  wxString rt = route_name;
  wxString file_folder = pPlugIn->StandardPathEXT();
  wxString file_name = file_folder + rt + ".xml";
  wxFileName fn;
  fn.SetFullName(file_name);
  // wxMessageBox(file_name);
  if (!wxFileExists(fn.GetFullName())) {
    bool success = wxRemove(file_name);
  } else {
    wxMessageBox("File not found");
  }
}

void tradnavUIDialog::OnBearingLine(wxCommandEvent& event) {
  ebl_lat = centreLat;
  ebl_lon = centreLon;

  wxString first_num, second_num, third_num;

  int fi = this->m_choiceFirstNum->GetSelection();
  wxString fs = this->m_choiceFirstNum->GetString(fi);

  int si = this->m_choiceSecondNum->GetSelection();
  wxString ss = this->m_choiceSecondNum->GetString(si);

  int ti = this->m_choiceThirdNum->GetSelection();
  wxString ts = this->m_choiceThirdNum->GetString(ti);

  wxString cs = fs + ss + ts;

  cs.ToDouble(&ebl_brg);

  double rev_brg = ebl_brg + 180;
  if (rev_brg > 360) rev_brg -= 360;

  double dlat, dlon;

  ll_gc_ll(centreLat, centreLon, rev_brg, ebl_range, &dlat, &dlon);

  BearingPoint start, end;
  start.lat = dlat;
  start.lon = dlon;
  end.lat = centreLat;
  end.lon = centreLon;

  SetBearing(cs, start, end);
  RequestRefresh(pParent);
}

void tradnavUIDialog::SetBearing(wxString bearing_name, BearingPoint start,
                                 BearingPoint end) {
  b_target = new BearingTarget;
  b_target->route_name = bearing_name;
  // wxMessageBox(b_target->route_name);
  // wxMessageBox(b_target->wpId);
  b_target->startLat = start.lat;
  b_target->startLon = start.lon;
  b_target->endLat = end.lat;
  b_target->endLon = end.lon;

  double brg, dist;

  DistanceBearingMercator_Plugin(end.lat, end.lon, start.lat, start.lon, &brg,
                                 &dist);
  b_target->bearing = brg;
  double label_distance = dist / 4;

  // double llat, llon;
  // PositionBearingDistanceMercator_Plugin(start.lat, start.lon, brg,
  // label_distance,
  //                                        &llat, &llon);

  b_target->label_lat = end.lat;
  b_target->label_lon = end.lon;

  b_vector.push_back(*b_target);

  wxString date_stamp = "Now";  // pPlugIn->GetRTZDateStamp(mySelectedRoute);
  wxString extensions_file = mySelectedRoute + ".xml";
  SaveBearingInfo(mySelectedRoute, date_stamp);
}

void tradnavUIDialog::SaveBearingInfo(wxString route_name,
                                      wxString date_stamp) {
  // Create Main level XML container
  xml_document xmlDoc;

  auto declarationNode = xmlDoc.append_child(node_declaration);

  declarationNode.append_attribute("version") = "1.0";

  declarationNode.append_attribute("encoding") = "UTF-8";

  // Create XML root node called extensions
  xml_node pRoot = xmlDoc.append_child("extensions");

  int idn = 0;
  wxString route;

  xml_node m_rootNode = pRoot.append_child("route");
  m_rootNode.append_attribute("route_name").set_value(route_name.mb_str());
  m_rootNode.append_attribute("date_stamp").set_value(date_stamp.mb_str());

  xml_node m_targetNode = m_rootNode.append_child("targets");

  for (std::vector<BearingTarget>::iterator itOut = b_vector.begin();
       itOut != b_vector.end(); itOut++) {
    xml_node m_targetpoint = m_targetNode.append_child("bearing_target");

    // wxString myIdn = wxString::Format("%i", idn);
    m_targetpoint.append_attribute("wp_id").set_value((*itOut).wpId.mb_str());
    wxString wpid = (*itOut).wpId.mb_str();
    // wxMessageBox(wpid);

    xml_node b_position = m_targetpoint.append_child("begin");

    wxString bLat = wxString::Format("%f", (*itOut).startLat);
    wxString bLon = wxString::Format("%f", (*itOut).startLon);

    b_position.append_attribute("lat").set_value(bLat);
    b_position.append_attribute("lon").set_value(bLon);

    xml_node e_position = m_targetpoint.append_child("end");

    wxString eLat = wxString::Format("%f", (*itOut).endLat);
    wxString eLon = wxString::Format("%f", (*itOut).endLon);

    e_position.append_attribute("lat").set_value(eLat);
    e_position.append_attribute("lon").set_value(eLon);

    xml_node ibearing = m_targetpoint.append_child("object_bearing");
    wxString brg = wxString::Format("%f", (*itOut).bearing);
    ibearing.append_attribute("bearing").set_value(brg);

    xml_node label = m_targetpoint.append_child("label");

    wxString llat = wxString::Format("%f", (*itOut).label_lat);
    wxString llon = wxString::Format("%f", (*itOut).label_lon);

    label.append_attribute("label_lat").set_value(llat);
    label.append_attribute("label_lon").set_value(llon);

    idn++;
  }

  // done adding index, range and direction targets
  // Write xmlDoc into a file
  wxString file_name = route_name + ".xml";
  wxString file_path = pPlugIn->StandardPathEXT() + file_name;
  // Route name must be the same as the file name, without file extension

  xmlDoc.save_file(file_path.mb_str());
}

void tradnavUIDialog::OnBearingLineDelete(wxCommandEvent& event) {
  double label_distance = 1000;
  double minDist = 500.0;
  int index_num = 0;
  int it_num = 0;
  bool foundIndex = false;
  double label_lat;
  double label_lon;
  double it_lat, it_lon;

  // while (!foundWP) {
  for (vector<BearingTarget>::iterator it = b_vector.begin();
       it != b_vector.end(); it++) {
    label_lat = (*it).label_lat;
    label_lon = (*it).label_lon;

    double brg;
    DistanceBearingMercator_Plugin(centreLat, centreLon, label_lat, label_lon,
                                   &brg, &label_distance);

    if (label_distance < minDist) {
      minDist = label_distance;
      it_lat = label_lat;
      it_lon = label_lon;
      it_num = index_num;
    }
    index_num++;
  }

  b_vector.erase(b_vector.begin() + it_num);

  wxString date_stamp = pPlugIn->GetRTZDateStamp(mySelectedRoute);
  wxString extensions_file = mySelectedRoute + ".xml";
  SaveBearingInfo(mySelectedRoute, date_stamp);

  RequestRefresh(pParent);
}

void tradnavUIDialog::OnIndex(wxCommandEvent& event) {
  ebl_lat = pPlugIn->GetCursorLat();
  ebl_lon = pPlugIn->GetCursorLon();

  // int ci = this->m_choiceObjectBearing->GetSelection();
  // wxString cs = this->m_choiceObjectBearing->GetString(ci);
  // cs.ToDouble(&ebl_brg);

  // GetIndex(active_waypoint, prev_waypoint);
  RequestRefresh(pParent);
}

void tradnavUIDialog::GetIndex(Position* A, Position* B) {
  double value = 0.0;
  A->lat.ToDouble(&value);
  double lat1 = value;
  A->lon.ToDouble(&value);
  double lon1 = value;
  B->lat.ToDouble(&value);
  double lat2 = value;
  B->lon.ToDouble(&value);
  double lon2 = value;

  double lat3 = centreLat;
  double lon3 = centreLon;

  // Bearing A -> C
  double dist1, dist2, brg1, brg2;
  DistanceBearingMercator_Plugin(lat3, lon3, lat1, lon1, &brg1, &dist1);
  // Bearing A -> B
  DistanceBearingMercator_Plugin(lat2, lon2, lat1, lon1, &brg2, &dist2);

  wxString sbrg1 = wxString::Format("%f", brg1);
  // wxMessageBox(sbrg1);

  wxString sbrg2 = wxString::Format("%f", brg2);
  // wxMessageBox(sbrg2);
  /*
  Location a;
  Location b;
  Location x;
*/
  double ax = dist1;
  double alfa = (abs(brg2 - brg1)) / 180 * PI;
  double distance = sin(alfa) * ax;

  double beta = cos(alfa) * ax;

  double dlat, dlon;
  PositionBearingDistanceMercator_Plugin(lat1, lon1, brg2, beta, &dlat, &dlon);

  wxString sdlat = wxString::Format("%f", dlat);
  // wxMessageBox(sdlat);

  wxString sdlon = wxString::Format("%f", dlon);
  // wxMessageBox(sdlon);

  i_target = new IndexTarget;
  i_target->route_name = A->route_name;
  // wxMessageBox(i_target->route_name);
  i_target->wpId = A->wpId;
  // wxMessageBox(i_target->wpId);
  i_target->beginLat = dlat;
  i_target->beginLon = dlon;
  i_target->endLat = lat3;
  i_target->endLon = lon3;

  DistanceBearingMercator_Plugin(lat3, lon3, dlat, dlon, &brg2, &dist2);
  i_target->distance = dist2;
  double label_distance = dist2 / 2;

  double llat, llon;
  PositionBearingDistanceMercator_Plugin(dlat, dlon, brg2, label_distance,
                                         &llat, &llon);
  i_target->label_lat = llat;
  i_target->label_lon = llon;

  i_vector.push_back(*i_target);

  wxString date_stamp = pPlugIn->GetRTZDateStamp(mySelectedRoute);
  wxString extensions_file = mySelectedRoute + ".xml";
  SaveIndexRangeDirection(mySelectedRoute, date_stamp);
}
void tradnavUIDialog::OnIndexDelete(wxCommandEvent& event) {
  double label_distance = 1000;
  double minDist = 500.0;
  int index_num = 0;
  int it_num = 0;
  bool foundIndex = false;
  double label_lat;
  double label_lon;
  double it_lat, it_lon;

  if (i_vector.size() == 0) {
    wxMessageBox("No RTZ file has been read");
  }

  // while (!foundWP) {
  for (vector<IndexTarget>::iterator it = i_vector.begin();
       it != i_vector.end(); it++) {
    label_lat = (*it).label_lat;
    label_lon = (*it).label_lon;

    double brg;
    DistanceBearingMercator_Plugin(centreLat, centreLon, label_lat, label_lon,
                                   &brg, &label_distance);

    if (label_distance < minDist) {
      minDist = label_distance;
      it_lat = label_lat;
      it_lon = label_lon;
      it_num = index_num;
    }
    index_num++;
  }

  i_vector.erase(i_vector.begin() + it_num);

  wxString date_stamp = pPlugIn->GetRTZDateStamp(mySelectedRoute);
  wxString extensions_file = mySelectedRoute + ".xml";
  SaveIndexRangeDirection(mySelectedRoute, date_stamp);

  RequestRefresh(pParent);
}

void tradnavUIDialog::OnRangeCircle(wxCommandEvent& event) {
  ebl_lat = centreLat;
  ebl_lon = centreLon;

  RangeTarget* range_centre = new RangeTarget;
  range_centre->beginLat = centreLat;
  range_centre->beginLon = centreLon;
  setRangeCircle(range_centre);
}

void tradnavUIDialog::setRangeCircle(RangeTarget* range_centre) {
  m_bRunningFix = false;

  wxString first_num, second_num, third_num;

  int fi = this->m_choiceFirstNum->GetSelection();
  wxString fs = this->m_choiceFirstNum->GetString(fi);

  int si = this->m_choiceSecondNum->GetSelection();
  wxString ss = this->m_choiceSecondNum->GetString(si);

  int ti = this->m_choiceThirdNum->GetSelection();
  wxString ts = this->m_choiceThirdNum->GetString(ti);

  wxString cs = fs + ss + ts;

  cs.ToDouble(&ebl_brg);

  m_ShipLat2 = pPlugIn->m_ship_lat;
  m_ShipLon2 = pPlugIn->m_ship_lon;

  r_target = new RangeTarget;
  r_target->route_name = "";
  // wxMessageBox(r_target->route_name);
  r_target->wpId = "";
  // wxMessageBox(r_target->wpId);
  r_target->beginLat = range_centre->beginLat;
  r_target->beginLon = range_centre->beginLon;

  double rs = m_range->GetValue();

  r_target->distance = rs;

  r_target->label_lat = range_centre->beginLat;
  r_target->label_lon = range_centre->beginLon;

  r_vector.push_back(*r_target);

  wxString date_stamp = pPlugIn->GetRTZDateStamp(mySelectedRoute);
  wxString extensions_file = mySelectedRoute + ".xml";
  SaveRangeInfo(mySelectedRoute, date_stamp);

  RequestRefresh(pParent);
}

void tradnavUIDialog::SaveRangeInfo(wxString route_name, wxString date_stamp) {
  // Create Main level XML container
  xml_document xmlDoc;

  auto declarationNode = xmlDoc.append_child(node_declaration);

  declarationNode.append_attribute("version") = "1.0";

  declarationNode.append_attribute("encoding") = "UTF-8";

  // Create XML root node called extensions
  xml_node pRoot = xmlDoc.append_child("extensions");

  int idn = 0;
  wxString route;

  xml_node m_rootNode = pRoot.append_child("route");
  m_rootNode.append_attribute("route_name").set_value(route_name.mb_str());
  m_rootNode.append_attribute("date_stamp").set_value(date_stamp.mb_str());

  xml_node m_targetNode = m_rootNode.append_child("targets");

  for (std::vector<RangeTarget>::iterator itOut = r_vector.begin();
       itOut != r_vector.end(); itOut++) {
    xml_node m_targetpoint = m_targetNode.append_child("range_target");

    // wxString myIdn = wxString::Format("%i", idn);
    m_targetpoint.append_attribute("wp_id").set_value((*itOut).wpId.mb_str());
    wxString wpid = (*itOut).wpId.mb_str();
    // wxMessageBox(wpid);

    xml_node b_position = m_targetpoint.append_child("begin");

    wxString bLat = wxString::Format("%f", (*itOut).beginLat);
    wxString bLon = wxString::Format("%f", (*itOut).beginLon);

    b_position.append_attribute("lat").set_value(bLat);
    b_position.append_attribute("lon").set_value(bLon);

    xml_node e_position = m_targetpoint.append_child("end");

    wxString eLat = wxString::Format("%f", (*itOut).endLat);
    wxString eLon = wxString::Format("%f", (*itOut).endLon);

    e_position.append_attribute("lat").set_value(eLat);
    e_position.append_attribute("lon").set_value(eLon);

    xml_node ibearing = m_targetpoint.append_child("object_distance");
    wxString dist = wxString::Format("%f", (*itOut).distance);
    ibearing.append_attribute("distance").set_value(dist);

    xml_node label = m_targetpoint.append_child("label");

    wxString llat = wxString::Format("%f", (*itOut).label_lat);
    wxString llon = wxString::Format("%f", (*itOut).label_lon);

    label.append_attribute("label_lat").set_value(llat);
    label.append_attribute("label_lon").set_value(llon);

    idn++;
  }

  // done adding index, range and direction targets
  // Write xmlDoc into a file
  wxString file_name = route_name + ".xml";
  wxString file_path = pPlugIn->StandardPathEXT() + file_name;
  // Route name must be the same as the file name, without file extension

  xmlDoc.save_file(file_path.mb_str());
}

void tradnavUIDialog::OnRangeCircleDelete(wxCommandEvent& event) {
  double label_distance = 1000;
  double minDist = 500.0;
  int index_num = 0;
  int it_num = 0;
  bool foundIndex = false;
  double label_lat;
  double label_lon;
  double it_lat, it_lon;

  for (vector<RangeTarget>::iterator it = r_vector.begin();
       it != r_vector.end(); it++) {
    label_lat = (*it).label_lat;
    label_lon = (*it).label_lon;

    double brg;
    DistanceBearingMercator_Plugin(centreLat, centreLon, label_lat, label_lon,
                                   &brg, &label_distance);

    if (label_distance < minDist) {
      minDist = label_distance;
      it_lat = label_lat;
      it_lon = label_lon;
      it_num = index_num;
    }
    index_num++;
  }

  r_vector.erase(r_vector.begin() + it_num);

  wxString date_stamp = pPlugIn->GetRTZDateStamp(mySelectedRoute);
  wxString extensions_file = mySelectedRoute + ".xml";
  SaveIndexRangeDirection(mySelectedRoute, date_stamp);

  RequestRefresh(pParent);
}

void tradnavUIDialog::OnRange(wxCommandEvent& event) {
  if (mySelectedLeg == 999) {
    wxMessageBox("Please activate the waypoint for the leg");
    return;
  }
  Position* range_object = new Position;
  range_object->lat = wxString::Format("%f", centreLat);
  range_object->lon = wxString::Format("%f", centreLon);
  GetRange(active_waypoint, range_object);
}

void tradnavUIDialog::GetRange(Position* A, Position* B) {
  double value = 0.0;
  A->lat.ToDouble(&value);
  double lat1 = value;
  A->lon.ToDouble(&value);
  double lon1 = value;
  B->lat.ToDouble(&value);
  double lat2 = value;
  B->lon.ToDouble(&value);
  double lon2 = value;

  double dist, brg;
  // Bearing A -> B
  DistanceBearingMercator_Plugin(lat2, lon2, lat1, lon1, &brg, &dist);

  r_target = new RangeTarget;
  r_target->route_name = A->route_name;
  // wxMessageBox(r_target->route_name);
  r_target->wpId = A->wpId;
  // wxMessageBox(r_target->wpId);
  r_target->beginLat = lat1;
  r_target->beginLon = lon1;
  r_target->endLat = lat2;
  r_target->endLon = lon2;

  r_target->distance = dist;

  double label_distance = dist / 2;
  double dlat, dlon;
  PositionBearingDistanceMercator_Plugin(lat1, lon1, brg, label_distance, &dlat,
                                         &dlon);

  r_target->label_lat = dlat;
  r_target->label_lon = dlon;

  r_vector.push_back(*r_target);

  wxString date_stamp = pPlugIn->GetRTZDateStamp(mySelectedRoute);
  wxString extensions_file = mySelectedRoute + ".xml";
  SaveIndexRangeDirection(mySelectedRoute, date_stamp);
}

void tradnavUIDialog::OnRangeDelete(wxCommandEvent& event) {
  double label_distance = 1000;
  double minDist = 500.0;
  int index_num = 0;
  int it_num = 0;
  bool foundIndex = false;
  double label_lat;
  double label_lon;
  double it_lat, it_lon;

  if (r_vector.size() == 0) {
    wxMessageBox("No RTZ file has been read");
  }

  for (vector<RangeTarget>::iterator it = r_vector.begin();
       it != r_vector.end(); it++) {
    label_lat = (*it).label_lat;
    label_lon = (*it).label_lon;

    double brg;
    DistanceBearingMercator_Plugin(centreLat, centreLon, label_lat, label_lon,
                                   &brg, &label_distance);

    if (label_distance < minDist) {
      minDist = label_distance;
      it_lat = label_lat;
      it_lon = label_lon;
      it_num = index_num;
    }
    index_num++;
  }

  r_vector.erase(r_vector.begin() + it_num);

  wxString date_stamp = pPlugIn->GetRTZDateStamp(mySelectedRoute);
  wxString extensions_file = mySelectedRoute + ".xml";
  SaveIndexRangeDirection(mySelectedRoute, date_stamp);

  RequestRefresh(pParent);
}

void tradnavUIDialog::OnDirection(wxCommandEvent& event) {
  if (mySelectedLeg == 999) {
    wxMessageBox("Please activate the waypoint for the leg");
    return;
  }
  if (!m_bDrawDirectionArrow) {
    wxMessageBox(
        "No previous waypoint\nPlease activate the waypoint for the leg");
    return;
  }
  GetDirection(active_waypoint, prev_waypoint);
}

void tradnavUIDialog::GetDirection(Position* A, Position* B) {
  double value = 0.0;
  A->lat.ToDouble(&value);
  double lat1 = value;
  A->lon.ToDouble(&value);
  double lon1 = value;
  B->lat.ToDouble(&value);
  double lat2 = value;
  B->lon.ToDouble(&value);
  double lon2 = value;

  double brg, dist;

  // Bearing A -> B
  DistanceBearingMercator_Plugin(lat1, lon1, lat2, lon2, &brg, &dist);

  d_target = new DirectionTarget;
  d_target->m_lat = centreLat;
  d_target->m_lon = centreLon;
  d_target->m_dir = brg;

  d_vector.push_back(*d_target);

  wxString date_stamp = pPlugIn->GetRTZDateStamp(mySelectedRoute);
  wxString extensions_file = mySelectedRoute + ".xml";
  SaveIndexRangeDirection(mySelectedRoute, date_stamp);
}

void tradnavUIDialog::OnDirectionDelete(wxCommandEvent& event) {
  double label_distance = 1000;
  double minDist = 500.0;
  int index_num = 0;
  int it_num = 0;
  bool foundIndex = false;
  double label_lat;
  double label_lon;
  double it_lat, it_lon;

  if (d_vector.size() == 0) {
    wxMessageBox("No RTZ file has been read");
  }

  for (vector<DirectionTarget>::iterator it = d_vector.begin();
       it != d_vector.end(); it++) {
    label_lat = (*it).m_lat;
    label_lon = (*it).m_lon;

    double brg;
    DistanceBearingMercator_Plugin(centreLat, centreLon, label_lat, label_lon,
                                   &brg, &label_distance);

    if (label_distance < minDist) {
      minDist = label_distance;
      it_lat = label_lat;
      it_lon = label_lon;
      it_num = index_num;
    }
    index_num++;
  }

  d_vector.erase(d_vector.begin() + it_num);

  wxString date_stamp = pPlugIn->GetRTZDateStamp(mySelectedRoute);
  wxString extensions_file = mySelectedRoute + ".xml";
  SaveIndexRangeDirection(mySelectedRoute, date_stamp);

  RequestRefresh(pParent);
}

void tradnavUIDialog::OnSaveExtensions(wxCommandEvent& event) {
  wxString date_stamp = pPlugIn->GetRTZDateStamp(mySelectedRoute);
  wxString extensions_file = mySelectedRoute + ".xml";
  SaveIndexRangeDirection(mySelectedRoute, date_stamp);
  pPlugIn->DeleteEXT_Name(mySelectedRoute);
  pPlugIn->Add_EXT_db(extensions_file, mySelectedRoute, date_stamp);
  m_bDrawWptDisk = false;
  m_bDrawDirectionArrow = false;
}

void tradnavUIDialog::ReadRTZ(wxString file_name) {
  my_positions.clear();  // Set up a new vector

  wxString file = file_name;
  pugi::xml_document xmlDoc;
  pugi::xml_parse_result result =
      xmlDoc.load_file(file.mb_str(), parse_default | parse_declaration);

  string rtz_version = xmlDoc.child("route").attribute("version").value();

  pugi::xml_node pRoot = xmlDoc.child("route").child("routeInfo");
  if (pRoot == nullptr) return;

  wxString error;
  wxProgressDialog* progressdialog = NULL;

  my_position.visible = "1";  // dummy value to make sure there is an entry
  my_position.planned_speed =
      "5";  // dummy value to make sure there is an entry
  my_position.route_name = "na";
  my_position.wpSym = "diamond";

  xml_node pRouteNameElement = xmlDoc.child("route").child("routeInfo");

  if (pRouteNameElement == nullptr) return;

  string route_name = pRouteNameElement.attribute("routeName").value();
  my_position.route_name = route_name;
  mySelectedRoute = route_name;

  bool exists = false;
  // wxMessageBox(route_name);

  xml_node pWaypointsElement = xmlDoc.child("route").child("waypoints");
  if (pWaypointsElement == nullptr) return;

  xml_node pListWaypointsElement = pWaypointsElement.child("waypoint");
  if (pListWaypointsElement == nullptr) return;

  int legnum = 0;

  while (pListWaypointsElement != nullptr) {
    string value = "nullptr";
    value = pListWaypointsElement.attribute("id").value();
    if (value == "nullptr") return;  // must have id

    my_position.leg_number =
        legnum;  // Route leg defined by the waypoint at the end of each leg

    my_position.wpId = value;
    // wxMessageBox(value);

    value = pListWaypointsElement.attribute("name").value();
    if (value != "nullptr") {
      my_position.wpName = value;
    } else {
      my_position.wpName = my_position.wpId;  // make name same as id if missing
    }

    xml_node pListPositionElement = pListWaypointsElement.child("position");
    if (pListPositionElement == nullptr) return;
    while (pListPositionElement != nullptr) {
      string stp = pListPositionElement.attribute("lat").value();
      my_position.lat = stp;
      // wxMessageBox(stp);

      string stpl = pListPositionElement.attribute("lon").value();
      my_position.lon = stpl;
      // wxMessageBox(stpl);

      pListPositionElement = pListPositionElement.next_sibling(
          "position");  // stop the loop when position empty
    }

    pListWaypointsElement = pListWaypointsElement.next_sibling(
        "waypoint");  // stop the loop when waypoints empty

    my_positions.push_back(my_position);

    legnum++;
  }

  int count = my_positions.size();
  wxString mycount = wxString::Format("%i", count);
}

void tradnavUIDialog::ReadEXT(wxString file_name) {
  i_vector.clear();  // Set up a new vector
  i_target = new IndexTarget;
  r_vector.clear();  // Set up a new vector
  r_target = new RangeTarget;
  d_vector.clear();  // Set up a new vector
  d_target = new DirectionTarget;

  pugi::xml_document xmlDoc;
  pugi::xml_parse_result result =
      xmlDoc.load_file(file_name.mb_str(), parse_default | parse_declaration);
  bool exists = false;

  pugi::xml_node root = xmlDoc.child("extensions");
  pugi::xml_node route = root.child("route");
  i_target->route_name = route.attribute("route_name").value();
  i_target->date_stamp = route.attribute("date_stamp").value();

  //
  pugi::xml_node targetNode = route.child("targets");
  if (targetNode == NULL) return;

  for (pugi::xml_node indexNode = targetNode.child("index_target"); indexNode;
       indexNode = indexNode.next_sibling("index_target")) {
    i_target->wpId = indexNode.attribute("wp_id").value();
    xml_node pBeginElement = indexNode.child("begin");

    double dvalue = 0.0;
    wxString stp = pBeginElement.attribute("lat").value();
    stp.ToDouble(&dvalue);
    i_target->beginLat = dvalue;

    wxString stpl = pBeginElement.attribute("lon").value();
    stpl.ToDouble(&dvalue);
    i_target->beginLon = dvalue;

    xml_node pEndElement = indexNode.child("end");
    stp = pEndElement.attribute("lat").value();
    stp.ToDouble(&dvalue);
    i_target->endLat = dvalue;

    stpl = pEndElement.attribute("lon").value();
    stpl.ToDouble(&dvalue);
    i_target->endLon = dvalue;

    xml_node pDistance = indexNode.child("index_distance");
    stp = pDistance.attribute("distance").value();
    stp.ToDouble(&dvalue);
    i_target->distance = dvalue;

    xml_node pLabel = indexNode.child("label");
    stp = pLabel.attribute("label_lat").value();
    stp.ToDouble(&dvalue);
    i_target->label_lat = dvalue;

    stp = pLabel.attribute("label_lon").value();
    stp.ToDouble(&dvalue);
    i_target->label_lon = dvalue;

    i_vector.push_back(*i_target);
  }

  for (pugi::xml_node rangeNode = targetNode.child("range_target"); rangeNode;
       rangeNode = rangeNode.next_sibling("range_target")) {
    r_target->wpId = rangeNode.attribute("wp_id").value();
    xml_node pBeginElement = rangeNode.child("begin");

    double dvalue = 0.0;
    wxString stp = pBeginElement.attribute("lat").value();
    stp.ToDouble(&dvalue);
    r_target->beginLat = dvalue;

    wxString stpl = pBeginElement.attribute("lon").value();
    stpl.ToDouble(&dvalue);
    r_target->beginLon = dvalue;

    xml_node pEndElement = rangeNode.child("end");
    stp = pEndElement.attribute("lat").value();
    stp.ToDouble(&dvalue);
    r_target->endLat = dvalue;

    stpl = pEndElement.attribute("lon").value();
    stpl.ToDouble(&dvalue);
    r_target->endLon = dvalue;

    xml_node pDistance = rangeNode.child("range_distance");
    stp = pDistance.attribute("distance").value();
    // wxMessageBox(stp);
    stp.ToDouble(&dvalue);
    r_target->distance = dvalue;

    xml_node pLabel = rangeNode.child("label");
    stp = pLabel.attribute("label_distance").value();
    stp.ToDouble(&dvalue);
    r_target->distance = dvalue;

    stp = pLabel.attribute("label_lat").value();
    stp.ToDouble(&dvalue);
    r_target->label_lat = dvalue;

    stp = pLabel.attribute("label_lon").value();
    stp.ToDouble(&dvalue);
    r_target->label_lon = dvalue;

    r_vector.push_back(*r_target);
  }

  for (pugi::xml_node directionNode = targetNode.child("direction_target");
       directionNode;
       directionNode = directionNode.next_sibling("direction_target")) {
    double dvalue = 0.0;
    wxString stp = directionNode.attribute("direction_lat").value();
    stp.ToDouble(&dvalue);
    d_target->m_lat = dvalue;

    wxString stpl = directionNode.attribute("direction_lon").value();
    stpl.ToDouble(&dvalue);
    d_target->m_lon = dvalue;

    stp = directionNode.attribute("direction").value();
    stp.ToDouble(&dvalue);
    d_target->m_dir = dvalue;

    d_vector.push_back(*d_target);
  }

  RequestRefresh(pParent);
}

void tradnavUIDialog::OnContextMenu(double m_lat, double m_lon) {
  c_lat = m_lat;
  c_lon = m_lon;

  double b_lat, b_lon;
  b_lat = centreLat;
  b_lon = centreLon;
  double dist1, brg1;
  DistanceBearingMercator_Plugin(c_lat, c_lon, b_lat, b_lon, &brg1, &dist1);

  ebl_range = dist1;

  // wxMessageBox(wxString::Format("%f",ebl_range))
}

int tradnavUIDialog::SetActiveWaypoint(double t_lat, double t_lon) {
  double wpDistance = 1000;
  double rLat, rLon;
  double minDist = 500.0;
  int wpRefNum = 0;
  int it_num = 0;
  bool foundWP = false;
  wxString wp_name;
  wxString it_name;
  double it_lat, it_lon;

  if (my_positions.size() == 0) {
    wxMessageBox("No RTZ file has been read");
    return 999;
  }

  // while (!foundWP) {
  for (vector<Position>::iterator it = my_positions.begin();
       it != my_positions.end(); it++) {
    wpRefNum = (*it).leg_number;

    wp_name = (*it).wpName;

    double value = 0.0;
    wxString slat = (*it).lat;
    slat.ToDouble(&value);
    rLat = value;

    double value2 = 0.0;
    wxString slon = (*it).lon;
    slon.ToDouble(&value2);
    rLon = value2;

    double brg;
    // Bearing/Distance A -> B
    DistanceBearingMercator_Plugin(rLat, rLon, t_lat, t_lon, &brg, &wpDistance);

    if (wpDistance < minDist) {
      minDist = wpDistance;
      it_num = wpRefNum;
      it_name = wp_name;
      it_lat = rLat;
      it_lon = rLon;
    }
  }

  if (it_num != 0)
    FindPreviousWaypoint(it_name);
  else {
    active_waypoint = new Position;
    active_waypoint->wpName = it_name;

    wxString slat = wxString::Format("%f", it_lat);
    active_wp_lat = it_lat;
    active_waypoint->lat = slat;
    wxString slon = wxString::Format("%f", it_lon);
    active_wp_lon = it_lon;
    active_waypoint->lon = slon;
    active_waypoint->route_name = mySelectedRoute;
    m_bDrawWptDisk = true;
    m_bDrawDirectionArrow = false;
  }

  return it_num;
}

Position* tradnavUIDialog::FindPreviousWaypoint(wxString ActiveWpt) {
  active_waypoint = new Position;
  prev_waypoint = new Position;
  std::vector<Position>::iterator prev;
  for (std::vector<Position>::iterator itp = my_positions.begin(),
                                       prev = my_positions.end();
       itp != my_positions.end(); prev = itp, itp++) {
    wxString wpn = (*itp).wpName;

    if (wpn == ActiveWpt) {
      active_waypoint->wpName = wpn;
      active_waypoint->lat = (*itp).lat;
      active_waypoint->lon = (*itp).lon;
      active_waypoint->route_name = (*itp).route_name;
      active_waypoint->wpId = (*itp).wpId;

      double value = 0.0;
      wxString slat = active_waypoint->lat;
      slat.ToDouble(&value);
      active_wp_lat = value;

      double value2 = 0.0;
      wxString slon = active_waypoint->lon;
      slon.ToDouble(&value2);
      active_wp_lon = value2;

      prev_waypoint->wpId = (*prev).wpId;
      prev_waypoint->wpName = (*prev).wpName;

      prev_waypoint->lat = (*prev).lat;
      prev_waypoint->lon = (*prev).lon;

      // wxMessageBox(prev_waypoint->wpId);

      m_bDrawWptDisk = true;
      m_bDrawDirectionArrow = true;

      RequestRefresh(pParent);

      return active_waypoint;
    }
  }

  active_waypoint->wpName = wxEmptyString;
  return active_waypoint;
}

int tradnavUIDialog::DeleteChartedRoute() {
  std::vector<std::unique_ptr<PlugIn_Route_Ex>> routes;
  auto uids = GetRouteGUIDArray();
  if (uids.size() > 1) {
    wxMessageBox("Clear the other routes!");
    return 0;
  }

  DeletePlugInRoute(uids[0]);
  return 1;
}

double tradnavUIDialog::FindDistanceFromLeg(Position* A, Position* B,
                                            Position* C) {
  double value = 0.0;
  A->lat.ToDouble(&value);
  double lat1 = value;
  A->lon.ToDouble(&value);
  double lon1 = value;
  B->lat.ToDouble(&value);
  double lat2 = value;
  B->lon.ToDouble(&value);
  double lon2 = value;
  C->lat.ToDouble(&value);
  double lat3 = value;
  C->lon.ToDouble(&value);
  double lon3 = value;

  // Bearing A -> C
  double dist1, dist2, brg1, brg2;
  DistanceBearingMercator_Plugin(lat3, lon3, lat1, lon1, &brg1, &dist1);
  // Bearing A -> B
  DistanceBearingMercator_Plugin(lat2, lon2, lat1, lon1, &brg2, &dist2);

  wxString sbrg1 = wxString::Format("%f", brg1);
  // wxMessageBox(sbrg1);

  wxString sbrg2 = wxString::Format("%f", brg2);
  // wxMessageBox(sbrg2);
  /*
  Location a;
  Location b;
  Location x;
*/
  double ax = dist1;
  double alfa = (abs(brg2 - brg1)) / 180 * PI;
  double distance = sin(alfa) * ax;

  return distance;
}

void tradnavUIDialog::RequestVariation() {
  Json::Value value;

  wxDateTime time = time.FromUTC();

  value["Day"] = time.GetDay();
  value["Month"] = time.GetMonth();
  value["Year"] = time.GetYear();
  value["Hour"] = time.GetHour();
  value["Minute"] = time.GetMinute();
  value["Second"] = time.GetSecond();

  wxString out;

  Json::StreamWriterBuilder builder;
  std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
  std::ostringstream outStream;
  writer->write(value, &outStream);
  std::string str = outStream.str();

  SendPluginMessage(wxString("WMM_VARIATION_BOAT"), str);

  // Lock();
  // m_bNeedsGrib = false;
  // Unlock();
}

void tradnavUIDialog::OnButtonIdentify(wxCommandEvent& event) {
  m_bIdentify = true;
  m_bEBL = false;
  m_bRunningFix = false;

  wxString first_num, second_num, third_num;

  int fi = m_choiceFirstNum1->GetSelection();
  wxString fs = m_choiceFirstNum1->GetString(fi);

  int si = m_choiceSecondNum1->GetSelection();
  wxString ss = m_choiceSecondNum1->GetString(si);

  int ti = m_choiceThirdNum1->GetSelection();
  wxString ts = m_choiceThirdNum1->GetString(ti);

  wxString cs = fs + ss + ts;

  cs.ToDouble(&ebl_brg);

  rev_brg = ebl_brg + 180;
  if (rev_brg > 360) rev_brg -= 360;

  m_timer1.Start(200);
}

void tradnavUIDialog::OnButtonIdentify_off(wxCommandEvent& event) {
  m_bIdentify = false;
  m_bEBL = false;
  m_bRunningFix = false;

  this->m_timer1.Stop();

  RequestRefresh(pParent);
}

void tradnavUIDialog::OnButtonEBL(wxCommandEvent& event) {
  m_bEBL = true;
  m_bIdentify = false;
  m_bRunningFix = false;

  this->m_timer1.Start(200);
}

void tradnavUIDialog::OnButtonEBL_off(wxCommandEvent& event) {
  m_bEBL = false;
  m_bIdentify = false;
  m_bRunningFix = false;

  m_EBLbearing->SetValue(wxEmptyString);
  m_Lat1->SetValue(wxEmptyString);
  m_Lon1->SetValue(wxEmptyString);

  this->m_timer1.Stop();

  RequestRefresh(pParent);
}

void tradnavUIDialog::OnTimer(wxTimerEvent& event) {
  if (m_bIdentify) {
    MakeIdentifyEvent();
  } else if (m_bEBL) {
    MakeEBLEvent();
  }
}

void tradnavUIDialog::OnChoice(wxCommandEvent& event) {
  int fi = m_choicePlot->GetSelection();
  switch (fi) {
    case 0: {
      m_simplebook1->SetSelection(0);
      break;
    }
    case 1: {
      m_simplebook1->SetSelection(1);
      break;
    }
    case 2: {
      m_simplebook1->SetSelection(2);
      break;
    }
    case 3: {
      m_simplebook1->SetSelection(3);
      break;
    }
  }
}

void tradnavUIDialog::MakeIdentifyEvent() {
  if (m_bIdentify) {
    // cursor_lat = pPlugIn->GetCursorLat();
    // cursor_lon = pPlugIn->GetCursorLon();
    m_ShipLat2 = pPlugIn->m_ship_lat;
    m_ShipLon2 = pPlugIn->m_ship_lon;

    // double dist;
    // DistanceBearingMercator_Plugin( centreLat, centreLon,ebl_lat, ebl_lon,
    //                                &ebl_brg, &dist);

    brgs = wxString::Format("%3.0f", ebl_brg);
    if (ebl_brg < 10.0) {
      std::string s = brgs.ToStdString();
      string r = s.substr(2, 1);
      unsigned int number_of_zeros = 3 - r.length();  // add 1 zero

      r.insert(0, number_of_zeros, '0');
      brgs = r;
    } else if (ebl_brg >= 10 && ebl_brg < 100) {
      std::string s = brgs.ToStdString();
      string r = s.substr(1, 2);
      unsigned int number_of_zeros = 3 - r.length();  // add 1 zero

      r.insert(0, number_of_zeros, '0');
      brgs = r;
    }

  } else {
    ebl_lat = 0;
    ebl_lon = 0;

    m_ShipLat2 = 0;
    m_ShipLat2 = 0;
  }

  RequestRefresh(pParent);
}

void tradnavUIDialog::MakeEBLEvent() {
  if (m_bEBL) {
    this->m_Lat1->SetValue(wxString::Format("%.6f", pPlugIn->GetCursorLat()));
    this->m_Lon1->SetValue(wxString::Format("%.6f", pPlugIn->GetCursorLon()));

    ebl_lat = pPlugIn->GetCursorLat();
    ebl_lon = pPlugIn->GetCursorLon();
    m_ShipLat2 = centreLat;
    m_ShipLon2 = centreLon;

    double dist;
    DistanceBearingMercator_Plugin(ebl_lat, ebl_lon, m_ShipLat2, m_ShipLon2,
                                   &ebl_brg, &dist);

    brgs = wxString::Format("%3.0f", ebl_brg);

    if (ebl_brg < 10.0) {
      std::string s = brgs.ToStdString();
      string r = s.substr(2, 1);
      unsigned int number_of_zeros = 3 - r.length();  // add 1 zero

      r.insert(0, number_of_zeros, '0');
      brgs = r;
    } else if (ebl_brg >= 10 && ebl_brg < 100) {
      std::string s = brgs.ToStdString();
      string r = s.substr(1, 2);
      unsigned int number_of_zeros = 3 - r.length();  // add 1 zero

      r.insert(0, number_of_zeros, '0');
      brgs = r;
    }

    m_EBLbearing->SetValue(brgs);
  }

  RequestRefresh(pParent);
}

void tradnavUIDialog::OnPlotRunningFix(wxCommandEvent& event) {
  m_bRunningFix = true;

  ebl_lat = centreLat;
  ebl_lon = centreLon;

  int fi = this->m_choiceRFfirstNum->GetSelection();
  wxString fs = this->m_choiceRFfirstNum->GetString(fi);

  int si = this->m_choiceRFsecondNum->GetSelection();
  wxString ss = this->m_choiceRFsecondNum->GetString(si);

  int ti = this->m_choiceRFthirdNum->GetSelection();
  wxString ts = this->m_choiceRFthirdNum->GetString(ti);

  wxString cs = fs + ss + ts;

  cs.ToDouble(&rf_bearing);

  fi = this->m_choiceCourseFirstNum->GetSelection();
  fs = this->m_choiceCourseFirstNum->GetString(fi);

  si = this->m_choiceCourseSecondNum->GetSelection();
  ss = this->m_choiceCourseSecondNum->GetString(si);

  ti = this->m_choiceCourseThirdNum->GetSelection();
  ts = this->m_choiceCourseThirdNum->GetString(ti);

  cs = fs + ss + ts;

  cs.ToDouble(&rf_course);

  rf_speed = this->m_speed->GetValue();

  fi = this->m_choiceMinutesFirstNum->GetSelection();
  fs = this->m_choiceMinutesFirstNum->GetString(fi);

  si = this->m_choiceMinutesSecondNum->GetSelection();
  ss = this->m_choiceMinutesSecondNum->GetString(si);

  cs = fs + ss;
  cs.ToDouble(&rf_minutes);

  rf_distance = (rf_minutes / 60) * rf_speed;

  // wxMessageBox(wxString::Format("%f", rf_distance));
}