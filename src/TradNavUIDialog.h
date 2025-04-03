/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  tradnav Plugin Friends
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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.         *
 ***************************************************************************
 */

#ifndef __tradnavUIDIALOG_H__
#define __tradnavUIDIALOG_H__

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif  // precompiled headers

#include <wx/fileconf.h>
#include <wx/glcanvas.h>

#include "TradNavUIDialogBase.h"
#include "NavFunc.h"

#include <wx/progdlg.h>
#include <list>
#include <vector>
#include "wx/dateevt.h"
#include "wx/stattext.h"
#include "ocpn_plugin.h"
#include "wx/dialog.h"
#include <wx/calctrl.h>
#include "wx/window.h"
#include <wx/colordlg.h>
#include <wx/event.h>
#include <wx/scrolwin.h>
#include <wx/datetime.h>
#include <wx/thread.h>
#include <wx/event.h>
#include <wx/listctrl.h>
//#include <wx/uiaction.h>
#include <wx/app.h>
#include <cmath>
#include <wx/menu.h>
#include <wx/string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctime>
#include <wx/filefn.h>

#ifndef WIN32
#include <unistd.h>
#endif

#ifdef WIN32
#define stat _stat
#endif

#if wxUSE_UIACTIONSIMULATOR
//#include "wx/uiaction.h"
#endif

/* XPM */
static const char* eye[] = {"20 20 7 1",
                            ". c none",
                            "# c #000000",
                            "a c #333333",
                            "b c #666666",
                            "c c #999999",
                            "d c #cccccc",
                            "e c #ffffff",
                            "....................",
                            "....................",
                            "....................",
                            "....................",
                            ".......######.......",
                            ".....#aabccb#a#.....",
                            "....#deeeddeebcb#...",
                            "..#aeeeec##aceaec#..",
                            ".#bedaeee####dbcec#.",
                            "#aeedbdabc###bcceea#",
                            ".#bedad######abcec#.",
                            "..#be#d######dadb#..",
                            "...#abac####abba#...",
                            ".....##acbaca##.....",
                            ".......######.......",
                            "....................",
                            "....................",
                            "....................",
                            "....................",
                            "...................."};

using namespace std;

#ifndef PI
#define PI 3.1415926535897931160E0 /* pi */
#endif

#if !defined(NAN)
static const long long lNaN = 0xfff8000000000000;
#define NAN (*(double*)&lNaN)
#endif

#define RT_RCDATA2 MAKEINTRESOURCE(999)

/* Maximum value that can be returned by the rand function. */
#ifndef RAND_MAX
#define RAND_MAX 0x7fff
#endif

#define distance(X, Y) \
  sqrt((X) * (X) +     \
       (Y) * (Y))  // much faster than hypot#define distance(X, Y) sqrt((X)*(X)
                   // + (Y)*(Y)) // much faster than hypot

class tradnavOverlayFactory;
class PlugIn_ViewPort;
class PositionRecordSet;

class wxFileConfig;
class tradnav_pi;
class wxGraphicsContext;
class routeprop;
class BearingTarget;
class BearingPoint;
class IndexTarget;
class DirectionTarget;
class RangeTarget;

class BearingTarget {
public:
  wxString route_name;
  wxString date_stamp;
  wxString wpId;
  double startLat, startLon;
  double endLat, endLon;
  double bearing;
  double label_lat;
  double label_lon;
};

class BearingPoint {
public:
  double lat, lon, rng;
};

class IndexTarget {
public:
  wxString route_name;
  wxString date_stamp;
  wxString wpId;
  double beginLat, beginLon;
  double endLat, endLon;
  double distance;
  double label_lat;
  double label_lon;
};

class DirectionTarget {
public:
  wxString route_name;
  wxString date_stamp;
  wxString dId;
  double m_lat, m_lon;
  double m_dir;
};

class RangeTarget {
public:
  wxString route_name;
  wxString date_stamp;
  wxString wpId;
  double beginLat, beginLon;
  double endLat, endLon;
  double distance;
  double label_lat;
  double label_lon;
  double object_bearing;
};

class rtept {
public:
  wxString Name, m_GUID;
  int index;
  wxString lat, lon;
  wxString visible;
  wxString sym;
  wxString planned_speed;
};

class rte {
public:
  wxString Name;
  vector<rtept> m_rteptList;
};

class routeCurrent {
public:
  int m_LegNumber;
  int m_TCRefNumber;
  double lat, lon;
};

class routeLeg {
public:
  wxString LegName;
  vector<rtept> m_rteptList;
};

class Position {
public:
  wxString lat, lon, wpt_num;
  wxString name;
  wxString guid;
  wxString time;
  wxString etd;
  wxString CTS;
  wxString SMG;
  wxString distTo;
  wxString brgTo;
  wxString set;
  wxString rate;
  wxString icon_name;
  bool show_name;
  wxString visible;
  bool is_visible;
  wxString wpSym;
  wxString wpName;
  wxString wpId;
  int routepoint;
  wxString route_name;
  wxString planned_speed;
  int leg_number;
};

#define pi 3.14159265358979323846

class tradnavUIDialog : public tradnavUIDialogBase {
public:
  tradnavUIDialog(wxWindow* parent, tradnav_pi* ppi);
  ~tradnavUIDialog();

  tradnav_pi* pPlugIn;

  // void SetCursorLatLon(double lat, double lon);

  void SetViewPort(PlugIn_ViewPort* vp);
  PlugIn_ViewPort* vp;

  wxDateTime m_dtNow;
  double m_dInterval;

  bool onNext;
  bool onPrev;

  wxString m_FolderSelected;
  int m_IntervalSelected;

  time_t myCurrentTime;

  wxString MakeDateTimeLabel(wxDateTime myDateTime);
  void OnInformation(wxCommandEvent& event);

  void AddChartRoute(wxString myRoute);

  virtual void Lock() { routemutex.Lock(); }
  virtual void Unlock() { routemutex.Unlock(); }

  void OnContextMenu(double m_lat, double m_lon);

  double FindDistanceFromLeg(Position* A, Position* B, Position* C);
  int SetActiveWaypoint(double t_lat, double t_lon);
  bool m_bDrawWptDisk;
  bool m_bDrawDirectionArrow;
  Position* FindPreviousWaypoint(wxString ActiveWpt);

  int DeleteChartedRoute();
  PlugIn_Waypoint_Ex *active_wpt, prev_wpt;

  double c_lat, c_lon;

  double deg2rad(double deg);
  double rad2deg(double rad);
  wxString SelectRoute(bool isDR);
  void SelectRoutePoints(wxString routeName);

  Position my_position;
  vector<Position> my_positions;
  vector<Position> my_points;

  double active_wp_lat, active_wp_lon;
  double prev_wp_lat, prev_wp_lon;

  wxString rte_start;
  wxString rte_end;

  void AddTestItems(wxCommandEvent& event);

  PlugIn_ViewPort *m_vp, m_current_vp;
  PlugIn_ViewPort& GetCurrentViewPort() { return m_current_vp; }
  wxPoint myPixArray[5];
  int myPixHeight;
  double chartScale;
  double centreLat;
  double centreLon;
  double ppm;
  void MakeBoxPoints();
  bool m_bEBL;
  bool m_bIdentify;
  bool m_bIndexLabel;
  bool m_bMoveUpDownLeftRight;
  double m_ShipLat1, m_ShipLon1;
  double m_ShipLat2, m_ShipLon2;
  double ebl_lat, ebl_lon;
  double cursor_lat, cursor_lon;
  void key_shortcut(wxKeyEvent& event);
  BearingTarget* b_target;
  IndexTarget* i_target;
  DirectionTarget* d_target;
  RangeTarget* r_target;

  vector<BearingTarget> b_vector;
  vector<IndexTarget> i_vector;
  vector<DirectionTarget> d_vector;
  vector<RangeTarget> r_vector;

  wxString m_dateStamp;
  wxWindow* pParent;

  wxString brgs;
  double ebl_brg;
  double rev_brg;
  double ebl_range;

  double rf_course;  // rf == running fix
  double rf_speed;
  double rf_bearing;
  double rf_minutes;
  double rf_distance;

  bool m_bRunningFix;


protected:
private:
  double myDist;

  double initLat;
  double initLon;
  double nextLat;
  double nextLon;

  int nextRoutePointIndex;
  double nextRoutePoint;
  double followDir;
  int countRoutePoints;
  wxMutex routemutex;

  void OnClose(wxCloseEvent& event);
  void OnMove(wxMoveEvent& event);
  void OnSize(wxSizeEvent& event);

  vector<rte> my_routes;
  vector<rtept> routePoints;
  vector<routeLeg> routeLegs;
  wxString mySelectedRoute;
  int mySelectedLeg;

  int GetRandomNumber(int range_min, int range_max);
  double m_cursor_lat, m_cursor_lon;
  wxString g_SData_Locn;

  bool error_found;
  bool dbg;

  wxString waypointName[2000];
  wxString waypointVisible[2000];

  int myZoom;
  int GetScale(double myChartScale);

  wxString rtz_version;
  wxArrayString my_areas[10], my_files[10][10];
  int ca, cf;
  wxString id_wpt;
  void ReadRTZ(wxString file_name);
  void ReadEXT(wxString file_name);

  Position* active_waypoint;
  Position* prev_waypoint;
  void OnNewRoute(wxCommandEvent& event);
  unique_ptr<PlugIn_Route_Ex> thisRoute;
  vector<PlugIn_Waypoint_Ex*> theWaypoints;
  Plugin_WaypointExList* myList;
  void WriteRTZ(wxString route_name);
  void WriteEXT(wxString route_name);
  void DeleteEXTFile(wxString route_name);
  void OnSaveExtensions(wxCommandEvent& event);

  void OnBearingLine(wxCommandEvent& event);
  void SetBearing(wxString bearing_name, BearingPoint start, BearingPoint end);
  void SaveBearingInfo(wxString route_name, wxString date_stamp);
  void OnBearingLineDelete(wxCommandEvent& event);
  
  void OnRangeCircle(wxCommandEvent& event);
  void setRangeCircle(RangeTarget* range_centre);
  void SaveRangeInfo(wxString route_name, wxString date_stamp);
  void OnRangeCircleDelete(wxCommandEvent& event);

  void OnIndex(wxCommandEvent& event);
  void SaveIndexRangeDirection(wxString route_name, wxString date_stamp);
  void GetIndex(Position* A, Position* B);
  void OnIndexDelete(wxCommandEvent& event);
  void OnRange(wxCommandEvent& event);
  void GetRange(Position* A, Position* B);
  void OnRangeDelete(wxCommandEvent& event);
  void OnDirection(wxCommandEvent& event);
  void GetDirection(Position* A, Position* B);
  void OnDirectionDelete(wxCommandEvent& event);
  void OnTimer(wxTimerEvent& event);
  void OnButtonIdentify(wxCommandEvent& event);
  void OnButtonIdentify_off(wxCommandEvent& event);
  void MakeIdentifyEvent();
  void MakeEBLEvent();
  void OnButtonEBL(wxCommandEvent& event);
  void OnButtonEBL_off(wxCommandEvent& event);
  void RequestVariation();
  void OnPlotRunningFix(wxCommandEvent& event);
  void OnChoice(wxCommandEvent& event);
};

class GetRouteDialog : public wxDialog {
public:
  GetRouteDialog(wxWindow* parent, wxWindowID id,
                 const wxString& title = "Routes available",
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = wxDEFAULT_DIALOG_STYLE);

  ~GetRouteDialog();

  wxStdDialogButtonSizer* m_sdbSizer1;
  wxButton* m_sdbSizer1OK;
  wxButton* m_sdbSizer1Cancel;
  wxListView* dialogText;

  void OnCancel();

protected:
private:
};

#endif
