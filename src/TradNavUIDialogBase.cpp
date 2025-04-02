///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.0.0-0-g0efcecf)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "TradNavUIDialogBase.h"

///////////////////////////////////////////////////////////////////////////

tradnavUIDialogBase::tradnavUIDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );

	wxString m_choicePlotChoices[] = { _("Range and Bearing"), _("Running Fix"), _("Identify"), _("EBL") };
	int m_choicePlotNChoices = sizeof( m_choicePlotChoices ) / sizeof( wxString );
	m_choicePlot = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choicePlotNChoices, m_choicePlotChoices, 0 );
	m_choicePlot->SetSelection( 0 );
	m_choicePlot->SetFont( wxFont( 14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Arial") ) );

	bSizer3->Add( m_choicePlot, 0, wxALL|wxEXPAND, 5 );

	m_simplebook1 = new wxSimplebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panelRangeBearing = new wxPanel( m_simplebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panelRangeBearing->SetBackgroundColour( wxColour( 139, 197, 197 ) );

	wxBoxSizer* bSizer713;
	bSizer713 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer253;
	bSizer253 = new wxBoxSizer( wxVERTICAL );

	m_textCtrl83 = new wxTextCtrl( m_panelRangeBearing, wxID_ANY, _("Range and Bearing"), wxDefaultPosition, wxDefaultSize, wxTE_CENTER );
	m_textCtrl83->SetForegroundColour( wxColour( 255, 255, 255 ) );
	m_textCtrl83->SetBackgroundColour( wxColour( 75, 126, 211 ) );

	bSizer253->Add( m_textCtrl83, 0, wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizer813;
	bSizer813 = new wxBoxSizer( wxVERTICAL );


	bSizer253->Add( bSizer813, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer53;
	bSizer53 = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizer43;
	fgSizer43 = new wxFlexGridSizer( 0, 4, 0, 0 );
	fgSizer43->SetFlexibleDirection( wxBOTH );
	fgSizer43->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText153 = new wxStaticText( m_panelRangeBearing, wxID_ANY, _("  Bearing    "), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText153->Wrap( -1 );
	fgSizer43->Add( m_staticText153, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxString m_choiceFirstNumChoices[] = { _("0"), _("1"), _("2"), _("3"), _("4"), _("5"), _("6"), _("7"), _("8"), _("9") };
	int m_choiceFirstNumNChoices = sizeof( m_choiceFirstNumChoices ) / sizeof( wxString );
	m_choiceFirstNum = new wxChoice( m_panelRangeBearing, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceFirstNumNChoices, m_choiceFirstNumChoices, 0 );
	m_choiceFirstNum->SetSelection( 0 );
	fgSizer43->Add( m_choiceFirstNum, 0, wxALIGN_RIGHT|wxALL, 5 );

	wxString m_choiceSecondNumChoices[] = { _("0"), _("1"), _("2"), _("3"), _("4"), _("5"), _("6"), _("7"), _("8"), _("9") };
	int m_choiceSecondNumNChoices = sizeof( m_choiceSecondNumChoices ) / sizeof( wxString );
	m_choiceSecondNum = new wxChoice( m_panelRangeBearing, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceSecondNumNChoices, m_choiceSecondNumChoices, 0 );
	m_choiceSecondNum->SetSelection( 0 );
	fgSizer43->Add( m_choiceSecondNum, 0, wxALL, 5 );

	wxString m_choiceThirdNumChoices[] = { _("0"), _("1"), _("2"), _("3"), _("4"), _("5"), _("6"), _("7"), _("8"), _("9") };
	int m_choiceThirdNumNChoices = sizeof( m_choiceThirdNumChoices ) / sizeof( wxString );
	m_choiceThirdNum = new wxChoice( m_panelRangeBearing, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceThirdNumNChoices, m_choiceThirdNumChoices, 0 );
	m_choiceThirdNum->SetSelection( 0 );
	fgSizer43->Add( m_choiceThirdNum, 0, wxALIGN_LEFT|wxALL, 5 );


	bSizer53->Add( fgSizer43, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer141;
	fgSizer141 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer141->SetFlexibleDirection( wxBOTH );
	fgSizer141->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText181 = new wxStaticText( m_panelRangeBearing, wxID_ANY, _("  Draw Length     "), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText181->Wrap( -1 );
	fgSizer141->Add( m_staticText181, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxString m_choiceDrawLengthChoices[] = { _("0"), _("1"), _("2"), _("3"), _("4"), _("5"), _("6"), _("7"), _("8"), _("9") };
	int m_choiceDrawLengthNChoices = sizeof( m_choiceDrawLengthChoices ) / sizeof( wxString );
	m_choiceDrawLength = new wxChoice( m_panelRangeBearing, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceDrawLengthNChoices, m_choiceDrawLengthChoices, 0 );
	m_choiceDrawLength->SetSelection( 9 );
	fgSizer141->Add( m_choiceDrawLength, 0, wxALL, 5 );

	m_staticText191 = new wxStaticText( m_panelRangeBearing, wxID_ANY, _("NM"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText191->Wrap( -1 );
	fgSizer141->Add( m_staticText191, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );


	bSizer53->Add( fgSizer141, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer52;
	fgSizer52 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer52->SetFlexibleDirection( wxBOTH );
	fgSizer52->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText101 = new wxStaticText( m_panelRangeBearing, wxID_ANY, _("  Target Range   "), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText101->Wrap( -1 );
	fgSizer52->Add( m_staticText101, 0, wxALL, 5 );

	m_range = new wxSpinCtrlDouble( m_panelRangeBearing, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0.000000, 1 );
	m_range->SetDigits( 1 );
	fgSizer52->Add( m_range, 0, wxALL, 5 );


	bSizer53->Add( fgSizer52, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer22;
	fgSizer22 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer22->SetFlexibleDirection( wxBOTH );
	fgSizer22->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText111 = new wxStaticText( m_panelRangeBearing, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText111->Wrap( -1 );
	fgSizer22->Add( m_staticText111, 0, wxALL, 5 );

	m_staticText411 = new wxStaticText( m_panelRangeBearing, wxID_ANY, _("Range"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText411->Wrap( -1 );
	fgSizer22->Add( m_staticText411, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );

	m_staticText42 = new wxStaticText( m_panelRangeBearing, wxID_ANY, _("Bearing"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText42->Wrap( -1 );
	fgSizer22->Add( m_staticText42, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );

	m_staticText121 = new wxStaticText( m_panelRangeBearing, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText121->Wrap( -1 );
	fgSizer22->Add( m_staticText121, 0, wxALL, 5 );

	m_buttonAttach = new wxButton( m_panelRangeBearing, wxID_ANY, _("Add"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonAttach->SetFont( wxFont( 14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Arial") ) );

	fgSizer22->Add( m_buttonAttach, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );

	m_buttonAttach2 = new wxButton( m_panelRangeBearing, wxID_ANY, _("Add"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonAttach2->SetFont( wxFont( 14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Arial") ) );

	fgSizer22->Add( m_buttonAttach2, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );

	m_staticText131 = new wxStaticText( m_panelRangeBearing, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText131->Wrap( -1 );
	fgSizer22->Add( m_staticText131, 0, wxALL, 5 );

	m_buttonAttach22 = new wxButton( m_panelRangeBearing, wxID_ANY, _("Del"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonAttach22->SetFont( wxFont( 14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Arial") ) );

	fgSizer22->Add( m_buttonAttach22, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_buttonAttach21 = new wxButton( m_panelRangeBearing, wxID_ANY, _("Del"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonAttach21->SetFont( wxFont( 14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Arial") ) );

	fgSizer22->Add( m_buttonAttach21, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );


	bSizer53->Add( fgSizer22, 0, wxEXPAND, 5 );


	bSizer15->Add( bSizer53, 0, wxEXPAND, 5 );


	bSizer253->Add( bSizer15, 0, wxEXPAND, 5 );


	bSizer713->Add( bSizer253, 0, wxEXPAND, 5 );


	m_panelRangeBearing->SetSizer( bSizer713 );
	m_panelRangeBearing->Layout();
	bSizer713->Fit( m_panelRangeBearing );
	m_simplebook1->AddPage( m_panelRangeBearing, _("a page"), false );
	m_panelRunningFix = new wxPanel( m_simplebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panelRunningFix->SetBackgroundColour( wxColour( 192, 192, 192 ) );

	wxBoxSizer* bSizer7121;
	bSizer7121 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer2521;
	bSizer2521 = new wxBoxSizer( wxVERTICAL );

	m_textCtrl821 = new wxTextCtrl( m_panelRunningFix, wxID_ANY, _("Running Fix"), wxDefaultPosition, wxDefaultSize, wxTE_CENTER );
	m_textCtrl821->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNTEXT ) );
	m_textCtrl821->SetBackgroundColour( wxColour( 255, 128, 0 ) );

	bSizer2521->Add( m_textCtrl821, 0, wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizer8121;
	bSizer8121 = new wxBoxSizer( wxVERTICAL );


	bSizer2521->Add( bSizer8121, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer131;
	bSizer131 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer521;
	bSizer521 = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizer4211;
	fgSizer4211 = new wxFlexGridSizer( 0, 4, 0, 0 );
	fgSizer4211->SetFlexibleDirection( wxBOTH );
	fgSizer4211->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText15211 = new wxStaticText( m_panelRunningFix, wxID_ANY, _("  First Bearing"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText15211->Wrap( -1 );
	fgSizer4211->Add( m_staticText15211, 0, wxALL, 5 );

	wxString m_choiceRFfirstNumChoices[] = { _("0"), _("1"), _("2"), _("3"), _("4"), _("5"), _("6"), _("7"), _("8"), _("9") };
	int m_choiceRFfirstNumNChoices = sizeof( m_choiceRFfirstNumChoices ) / sizeof( wxString );
	m_choiceRFfirstNum = new wxChoice( m_panelRunningFix, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceRFfirstNumNChoices, m_choiceRFfirstNumChoices, 0 );
	m_choiceRFfirstNum->SetSelection( 0 );
	fgSizer4211->Add( m_choiceRFfirstNum, 0, wxALIGN_RIGHT|wxALL, 5 );

	wxString m_choiceRFsecondNumChoices[] = { _("0"), _("1"), _("2"), _("3"), _("4"), _("5"), _("6"), _("7"), _("8"), _("9") };
	int m_choiceRFsecondNumNChoices = sizeof( m_choiceRFsecondNumChoices ) / sizeof( wxString );
	m_choiceRFsecondNum = new wxChoice( m_panelRunningFix, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceRFsecondNumNChoices, m_choiceRFsecondNumChoices, 0 );
	m_choiceRFsecondNum->SetSelection( 0 );
	fgSizer4211->Add( m_choiceRFsecondNum, 0, wxALL, 5 );

	wxString m_choiceRFthirdNumChoices[] = { _("0"), _("1"), _("2"), _("3"), _("4"), _("5"), _("6"), _("7"), _("8"), _("9") };
	int m_choiceRFthirdNumNChoices = sizeof( m_choiceRFthirdNumChoices ) / sizeof( wxString );
	m_choiceRFthirdNum = new wxChoice( m_panelRunningFix, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceRFthirdNumNChoices, m_choiceRFthirdNumChoices, 0 );
	m_choiceRFthirdNum->SetSelection( 0 );
	fgSizer4211->Add( m_choiceRFthirdNum, 0, wxALIGN_LEFT|wxALL, 5 );


	bSizer521->Add( fgSizer4211, 0, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer422;
	fgSizer422 = new wxFlexGridSizer( 0, 4, 0, 0 );
	fgSizer422->SetFlexibleDirection( wxBOTH );
	fgSizer422->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText1523 = new wxStaticText( m_panelRunningFix, wxID_ANY, _("  Course"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1523->Wrap( -1 );
	fgSizer422->Add( m_staticText1523, 0, wxALL, 5 );

	wxString m_choiceCourseFirstNumChoices[] = { _("0"), _("1"), _("2"), _("3"), _("4"), _("5"), _("6"), _("7"), _("8"), _("9") };
	int m_choiceCourseFirstNumNChoices = sizeof( m_choiceCourseFirstNumChoices ) / sizeof( wxString );
	m_choiceCourseFirstNum = new wxChoice( m_panelRunningFix, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceCourseFirstNumNChoices, m_choiceCourseFirstNumChoices, 0 );
	m_choiceCourseFirstNum->SetSelection( 0 );
	fgSizer422->Add( m_choiceCourseFirstNum, 0, wxALIGN_RIGHT|wxALL, 5 );

	wxString m_choiceCourseSecondNumChoices[] = { _("0"), _("1"), _("2"), _("3"), _("4"), _("5"), _("6"), _("7"), _("8"), _("9") };
	int m_choiceCourseSecondNumNChoices = sizeof( m_choiceCourseSecondNumChoices ) / sizeof( wxString );
	m_choiceCourseSecondNum = new wxChoice( m_panelRunningFix, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceCourseSecondNumNChoices, m_choiceCourseSecondNumChoices, 0 );
	m_choiceCourseSecondNum->SetSelection( 0 );
	fgSizer422->Add( m_choiceCourseSecondNum, 0, wxALL, 5 );

	wxString m_choiceCourseThirdNumChoices[] = { _("0"), _("1"), _("2"), _("3"), _("4"), _("5"), _("6"), _("7"), _("8"), _("9") };
	int m_choiceCourseThirdNumNChoices = sizeof( m_choiceCourseThirdNumChoices ) / sizeof( wxString );
	m_choiceCourseThirdNum = new wxChoice( m_panelRunningFix, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceCourseThirdNumNChoices, m_choiceCourseThirdNumChoices, 0 );
	m_choiceCourseThirdNum->SetSelection( 0 );
	fgSizer422->Add( m_choiceCourseThirdNum, 0, wxALIGN_LEFT|wxALL, 5 );


	bSizer521->Add( fgSizer422, 0, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer511;
	fgSizer511 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer511->SetFlexibleDirection( wxBOTH );
	fgSizer511->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText1011 = new wxStaticText( m_panelRunningFix, wxID_ANY, _("  Speed (Kts)   "), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1011->Wrap( -1 );
	fgSizer511->Add( m_staticText1011, 0, wxALL, 5 );

	m_speed = new wxSpinCtrlDouble( m_panelRunningFix, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0.000000, 1 );
	m_speed->SetDigits( 1 );
	fgSizer511->Add( m_speed, 0, wxALL, 5 );


	bSizer521->Add( fgSizer511, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer211;
	fgSizer211 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer211->SetFlexibleDirection( wxBOTH );
	fgSizer211->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText15221 = new wxStaticText( m_panelRunningFix, wxID_ANY, _("  Minutes"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText15221->Wrap( -1 );
	fgSizer211->Add( m_staticText15221, 0, wxALL, 5 );

	wxString m_choiceMinutesFirstNumChoices[] = { _("0"), _("1"), _("2"), _("3"), _("4"), _("5"), _("6"), _("7"), _("8"), _("9") };
	int m_choiceMinutesFirstNumNChoices = sizeof( m_choiceMinutesFirstNumChoices ) / sizeof( wxString );
	m_choiceMinutesFirstNum = new wxChoice( m_panelRunningFix, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceMinutesFirstNumNChoices, m_choiceMinutesFirstNumChoices, 0 );
	m_choiceMinutesFirstNum->SetSelection( 0 );
	fgSizer211->Add( m_choiceMinutesFirstNum, 0, wxALL, 5 );

	wxString m_choiceMinutesSecondNumChoices[] = { _("0"), _("1"), _("2"), _("3"), _("4"), _("5"), _("6"), _("7"), _("8"), _("9") };
	int m_choiceMinutesSecondNumNChoices = sizeof( m_choiceMinutesSecondNumChoices ) / sizeof( wxString );
	m_choiceMinutesSecondNum = new wxChoice( m_panelRunningFix, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceMinutesSecondNumNChoices, m_choiceMinutesSecondNumChoices, 0 );
	m_choiceMinutesSecondNum->SetSelection( 0 );
	fgSizer211->Add( m_choiceMinutesSecondNum, 0, wxALL, 5 );


	bSizer521->Add( fgSizer211, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer281;
	bSizer281 = new wxBoxSizer( wxVERTICAL );

	m_button271 = new wxButton( m_panelRunningFix, wxID_ANY, _("Plot transferred position line"), wxDefaultPosition, wxDefaultSize, 0 );
	m_button271->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_ACTIVECAPTION ) );

	bSizer281->Add( m_button271, 0, wxALL, 5 );


	bSizer521->Add( bSizer281, 0, wxEXPAND, 5 );


	bSizer131->Add( bSizer521, 0, wxEXPAND, 5 );


	bSizer2521->Add( bSizer131, 0, wxEXPAND, 5 );


	bSizer7121->Add( bSizer2521, 0, wxEXPAND, 5 );


	m_panelRunningFix->SetSizer( bSizer7121 );
	m_panelRunningFix->Layout();
	bSizer7121->Fit( m_panelRunningFix );
	m_simplebook1->AddPage( m_panelRunningFix, _("a page"), false );
	m_panelIdentify = new wxPanel( m_simplebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panelIdentify->SetBackgroundColour( wxColour( 45, 232, 105 ) );

	wxBoxSizer* bSizer7111;
	bSizer7111 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer2511;
	bSizer2511 = new wxBoxSizer( wxVERTICAL );

	m_textCtrl811 = new wxTextCtrl( m_panelIdentify, wxID_ANY, _("Identify"), wxDefaultPosition, wxDefaultSize, wxTE_CENTER );
	m_textCtrl811->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNTEXT ) );
	m_textCtrl811->SetBackgroundColour( wxColour( 255, 255, 128 ) );

	bSizer2511->Add( m_textCtrl811, 0, wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizer8111;
	bSizer8111 = new wxBoxSizer( wxVERTICAL );


	bSizer2511->Add( bSizer8111, 1, wxEXPAND, 5 );


	bSizer7111->Add( bSizer2511, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer121;
	bSizer121 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer511;
	bSizer511 = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizer411;
	fgSizer411 = new wxFlexGridSizer( 0, 4, 0, 0 );
	fgSizer411->SetFlexibleDirection( wxBOTH );
	fgSizer411->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText1511 = new wxStaticText( m_panelIdentify, wxID_ANY, _("  Bearing   "), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1511->Wrap( -1 );
	fgSizer411->Add( m_staticText1511, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxString m_choiceFirstNum1Choices[] = { _("0"), _("1"), _("2"), _("3"), _("4"), _("5"), _("6"), _("7"), _("8"), _("9") };
	int m_choiceFirstNum1NChoices = sizeof( m_choiceFirstNum1Choices ) / sizeof( wxString );
	m_choiceFirstNum1 = new wxChoice( m_panelIdentify, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceFirstNum1NChoices, m_choiceFirstNum1Choices, 0 );
	m_choiceFirstNum1->SetSelection( 0 );
	fgSizer411->Add( m_choiceFirstNum1, 0, wxALIGN_RIGHT|wxALL, 5 );

	wxString m_choiceSecondNum1Choices[] = { _("0"), _("1"), _("2"), _("3"), _("4"), _("5"), _("6"), _("7"), _("8"), _("9") };
	int m_choiceSecondNum1NChoices = sizeof( m_choiceSecondNum1Choices ) / sizeof( wxString );
	m_choiceSecondNum1 = new wxChoice( m_panelIdentify, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceSecondNum1NChoices, m_choiceSecondNum1Choices, 0 );
	m_choiceSecondNum1->SetSelection( 0 );
	fgSizer411->Add( m_choiceSecondNum1, 0, wxALL, 5 );

	wxString m_choiceThirdNum1Choices[] = { _("0"), _("1"), _("2"), _("3"), _("4"), _("5"), _("6"), _("7"), _("8"), _("9") };
	int m_choiceThirdNum1NChoices = sizeof( m_choiceThirdNum1Choices ) / sizeof( wxString );
	m_choiceThirdNum1 = new wxChoice( m_panelIdentify, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceThirdNum1NChoices, m_choiceThirdNum1Choices, 0 );
	m_choiceThirdNum1->SetSelection( 0 );
	fgSizer411->Add( m_choiceThirdNum1, 0, wxALIGN_LEFT|wxALL, 5 );


	bSizer511->Add( fgSizer411, 1, wxEXPAND, 5 );


	bSizer121->Add( bSizer511, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer1111;
	bSizer1111 = new wxBoxSizer( wxVERTICAL );

	m_bButtonBearing111 = new wxButton( m_panelIdentify, wxID_ANY, _("Plot bearing from ship "), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1111->Add( m_bButtonBearing111, 0, wxALL, 5 );

	m_button811 = new wxButton( m_panelIdentify, wxID_ANY, _("Switch off plot"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1111->Add( m_button811, 0, wxALL, 5 );


	bSizer121->Add( bSizer1111, 0, wxEXPAND, 5 );


	bSizer7111->Add( bSizer121, 0, wxEXPAND, 5 );


	m_panelIdentify->SetSizer( bSizer7111 );
	m_panelIdentify->Layout();
	bSizer7111->Fit( m_panelIdentify );
	m_simplebook1->AddPage( m_panelIdentify, _("a page"), false );
	m_panelEBL = new wxPanel( m_simplebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer141;
	bSizer141 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer112;
	bSizer112 = new wxBoxSizer( wxHORIZONTAL );

	m_bButtonBearing12 = new wxButton( m_panelEBL, wxID_ANY, _("EBL On"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer112->Add( m_bButtonBearing12, 0, wxALL, 5 );

	m_button82 = new wxButton( m_panelEBL, wxID_ANY, _("EBL Off"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer112->Add( m_button82, 0, wxALL, 5 );


	bSizer141->Add( bSizer112, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer171;
	bSizer171 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText161 = new wxStaticText( m_panelEBL, wxID_ANY, _("   Bearing:    "), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText161->Wrap( -1 );
	bSizer171->Add( m_staticText161, 0, wxALL, 5 );

	m_EBLbearing = new wxTextCtrl( m_panelEBL, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer171->Add( m_EBLbearing, 0, wxALL, 5 );


	bSizer141->Add( bSizer171, 0, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer3111;
	fgSizer3111 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer3111->SetFlexibleDirection( wxBOTH );
	fgSizer3111->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText6111 = new wxStaticText( m_panelEBL, wxID_ANY, _("Cross hairs Latitude"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6111->Wrap( -1 );
	fgSizer3111->Add( m_staticText6111, 0, wxALL, 5 );

	m_Lat1 = new wxTextCtrl( m_panelEBL, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3111->Add( m_Lat1, 0, wxALL, 5 );

	m_staticText7111 = new wxStaticText( m_panelEBL, wxID_ANY, _("Cross hairs Longitude"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7111->Wrap( -1 );
	fgSizer3111->Add( m_staticText7111, 0, wxALL, 5 );

	m_Lon1 = new wxTextCtrl( m_panelEBL, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3111->Add( m_Lon1, 0, wxALL, 5 );


	bSizer141->Add( fgSizer3111, 0, wxEXPAND, 5 );


	m_panelEBL->SetSizer( bSizer141 );
	m_panelEBL->Layout();
	bSizer141->Fit( m_panelEBL );
	m_simplebook1->AddPage( m_panelEBL, _("a page"), false );

	bSizer3->Add( m_simplebook1, 1, wxEXPAND | wxALL, 5 );


	fgSizer1->Add( bSizer3, 1, wxEXPAND, 5 );


	this->SetSizer( fgSizer1 );
	this->Layout();
	m_menubar3 = new wxMenuBar( 0 );
	m_mHelp = new wxMenu();
	wxMenuItem* m_mInformation;
	m_mInformation = new wxMenuItem( m_mHelp, wxID_ANY, wxString( _("Guide") ) , wxEmptyString, wxITEM_NORMAL );
	m_mHelp->Append( m_mInformation );

	wxMenuItem* m_mAbout;
	m_mAbout = new wxMenuItem( m_mHelp, wxID_ANY, wxString( _("About") ) , wxEmptyString, wxITEM_NORMAL );
	m_mHelp->Append( m_mAbout );

	m_menubar3->Append( m_mHelp, _("Help") );

	this->SetMenuBar( m_menubar3 );

	m_timer1.SetOwner( this, m_timer1.GetId() );

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( tradnavUIDialogBase::OnClose ) );
	this->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( tradnavUIDialogBase::key_shortcut ) );
	this->Connect( wxEVT_SIZE, wxSizeEventHandler( tradnavUIDialogBase::OnSize ) );
	m_choicePlot->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( tradnavUIDialogBase::OnChoice ), NULL, this );
	m_simplebook1->Connect( wxEVT_COMMAND_BOOKCTRL_PAGE_CHANGED, wxBookCtrlEventHandler( tradnavUIDialogBase::OnPageChanged ), NULL, this );
	m_panelRangeBearing->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( tradnavUIDialogBase::key_shortcut ), NULL, this );
	m_buttonAttach->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( tradnavUIDialogBase::OnRangeCircle ), NULL, this );
	m_buttonAttach2->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( tradnavUIDialogBase::OnBearingLine ), NULL, this );
	m_buttonAttach22->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( tradnavUIDialogBase::OnRangeCircleDelete ), NULL, this );
	m_buttonAttach21->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( tradnavUIDialogBase::OnBearingLineDelete ), NULL, this );
	m_panelRunningFix->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( tradnavUIDialogBase::key_shortcut ), NULL, this );
	m_button271->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( tradnavUIDialogBase::OnPlotRunningFix ), NULL, this );
	m_panelIdentify->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( tradnavUIDialogBase::key_shortcut ), NULL, this );
	m_bButtonBearing111->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( tradnavUIDialogBase::OnButtonIdentify ), NULL, this );
	m_button811->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( tradnavUIDialogBase::OnButtonIdentify_off ), NULL, this );
	m_bButtonBearing12->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( tradnavUIDialogBase::OnButtonEBL ), NULL, this );
	m_button82->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( tradnavUIDialogBase::OnButtonEBL_off ), NULL, this );
	m_mHelp->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( tradnavUIDialogBase::OnInformation ), this, m_mInformation->GetId());
	m_mHelp->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( tradnavUIDialogBase::OnAbout ), this, m_mAbout->GetId());
	this->Connect( m_timer1.GetId(), wxEVT_TIMER, wxTimerEventHandler( tradnavUIDialogBase::OnTimer ) );
}

tradnavUIDialogBase::~tradnavUIDialogBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( tradnavUIDialogBase::OnClose ) );
	this->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( tradnavUIDialogBase::key_shortcut ) );
	this->Disconnect( wxEVT_SIZE, wxSizeEventHandler( tradnavUIDialogBase::OnSize ) );
	m_choicePlot->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( tradnavUIDialogBase::OnChoice ), NULL, this );
	m_simplebook1->Disconnect( wxEVT_COMMAND_BOOKCTRL_PAGE_CHANGED, wxBookCtrlEventHandler( tradnavUIDialogBase::OnPageChanged ), NULL, this );
	m_panelRangeBearing->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( tradnavUIDialogBase::key_shortcut ), NULL, this );
	m_buttonAttach->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( tradnavUIDialogBase::OnRangeCircle ), NULL, this );
	m_buttonAttach2->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( tradnavUIDialogBase::OnBearingLine ), NULL, this );
	m_buttonAttach22->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( tradnavUIDialogBase::OnRangeCircleDelete ), NULL, this );
	m_buttonAttach21->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( tradnavUIDialogBase::OnBearingLineDelete ), NULL, this );
	m_panelRunningFix->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( tradnavUIDialogBase::key_shortcut ), NULL, this );
	m_button271->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( tradnavUIDialogBase::OnPlotRunningFix ), NULL, this );
	m_panelIdentify->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( tradnavUIDialogBase::key_shortcut ), NULL, this );
	m_bButtonBearing111->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( tradnavUIDialogBase::OnButtonIdentify ), NULL, this );
	m_button811->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( tradnavUIDialogBase::OnButtonIdentify_off ), NULL, this );
	m_bButtonBearing12->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( tradnavUIDialogBase::OnButtonEBL ), NULL, this );
	m_button82->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( tradnavUIDialogBase::OnButtonEBL_off ), NULL, this );
	this->Disconnect( m_timer1.GetId(), wxEVT_TIMER, wxTimerEventHandler( tradnavUIDialogBase::OnTimer ) );

}
