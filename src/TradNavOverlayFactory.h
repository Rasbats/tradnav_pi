/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  tradnav Plugin Freinds
 * Author:   David Register
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

#include <map>
#include <wx/string.h>
#include "pidc.h"
#include <wx/brush.h>
#include <wx/gdicmn.h>
#include "ocpn_plugin.h"
#include <wx/object.h>
#include <wx/font.h>
#include <wx/dcbuffer.h>
#include <wx/dc.h>
#include <cmath>

class piDC;

using namespace std;

#define NUM_DIRECTION_ARROW_POINTS 9
static wxPoint DirectionArrowArray[NUM_DIRECTION_ARROW_POINTS] = {
    wxPoint(0, 0),    wxPoint(0, -10), wxPoint(55, -10),
    wxPoint(55, -25), wxPoint(100, 0), wxPoint(55, 25),
    wxPoint(55, 10),  wxPoint(0, 10),  wxPoint(0, 0)};

#define NUM_REVERSE_ARROW_POINTS 9
static wxPoint ReverseArrowArray[NUM_REVERSE_ARROW_POINTS] = {
    wxPoint(100, -100), wxPoint(100, -90),  wxPoint(45, -90),
    wxPoint(45, -75),   wxPoint(0, -100),   wxPoint(45, -125),
    wxPoint(45, -110),  wxPoint(100, -110), wxPoint(100, -100)};

//----------------------------------------------------------------------------------------------------------
//    tradnav Overlay Factory Specification
//----------------------------------------------------------------------------------------------------------

class tradnavUIDialog;

class tradnavOverlayFactory {
public:
  tradnavOverlayFactory(tradnavUIDialog &dlg);
  ~tradnavOverlayFactory();

  void SetSettings(bool hiDefGraphics, bool GradualColors) {
    m_hiDefGraphics = hiDefGraphics;
    m_bGradualColors = GradualColors;
  }

  void SetMessage(wxString message) { m_Message = message; }
  void SetTimeZone(int TimeZone) { m_TimeZone = TimeZone; }
  void SetParentSize(int w, int h) {
    m_ParentSize.SetWidth(w);
    m_ParentSize.SetHeight(h);
  }
  void setData(double lat1, double lon1);
  bool RenderOverlay(piDC &dc, PlugIn_ViewPort &vp);
  void Reset();
  wxDateTime m_dtUseNew;

  piDC *m_dc;

private:
  wxPoint polyPoints[7];
  wxPoint rectPoints[7];

  wxPoint polyReversePoints[7];
  wxPoint rectReversePoints[7];

  double myLat1;
  double myLon1;
  void DrawAllLinesInViewPort(PlugIn_ViewPort *BBox);
  wxBitmap m_bmp;
  wxImage &DrawLabel(double value, int precision);
  void DrawWptDisk(PlugIn_ViewPort *BBox);
  void DrawBearingTargets(PlugIn_ViewPort *BBox);
  void DrawIndexTargets(PlugIn_ViewPort *BBox);
  void DrawRangeTargets(PlugIn_ViewPort *BBox);

  void DrawDirectionTargets(PlugIn_ViewPort *BBox);
  wxImage DrawDirectionArrows(int x, int y, double scale);
  wxImage DrawDirectionLabels(double value, int x, int y, double scale,
                              bool reverse);
  void DrawIdentifyLineInViewPort(PlugIn_ViewPort *BBox);
  void DrawEBLLineInViewPort(PlugIn_ViewPort *BBox);
  void DrawRFInViewPort(PlugIn_ViewPort *BBox);
  void DrawBearingLineInViewPort(PlugIn_ViewPort *BBox);
  void DrawRangeCircleInViewPort(PlugIn_ViewPort *BBox);
  wxImage &DrawLabelEBL(double value, int precision);
  wxString FormatBearing(double dbearing);

  wxPoint p[12];
  wxPoint r[12];

  double m_last_vp_scale;

  //  for GL
  wxColour c_GLcolour;
  wxPoint p_basic[9];
  //
  wxString m_Message;
  wxString m_Message_Hiden;
  int m_TimeZone;
  wxSize m_ParentSize;

  wxDC *m_pdc;
  wxFont *m_dFont_map;
  wxFont *m_dFont_war;

  bool m_hiDefGraphics;
  bool m_bGradualColors;

  std::map<double, wxImage> m_labelCache;
  std::map<wxString, wxImage> m_labelCacheText;

  tradnavUIDialog &m_dlg;
};
