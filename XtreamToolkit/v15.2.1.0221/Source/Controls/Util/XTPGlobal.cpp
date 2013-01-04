// XTPGlobal.cpp : implementation of the CXTPAuxData struct.
//
// This file is a part of the XTREME CONTROLS MFC class library.
// (c)1998-2012 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "Common/Resource.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPVC80Helpers.h"  // Visual Studio 2005 helper functions
#include "Common/XTPResourceManager.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPSystemHelpers.h"

#include "../Resource.h"
#include "XTPGlobal.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


//=============================================================================
// CXTPNonClientMetrics
//=============================================================================

CXTPNonClientMetrics::CXTPNonClientMetrics()
{
	::ZeroMemory(this, sizeof(NONCLIENTMETRICS));
	cbSize = sizeof(NONCLIENTMETRICS);

	VERIFY(::SystemParametersInfo(SPI_GETNONCLIENTMETRICS,
		sizeof(NONCLIENTMETRICS), this, 0));
}

//=============================================================================
// CXTPFontManager
//=============================================================================

CXTPLogFont::CXTPLogFont()
: dwType(0)
{
	::ZeroMemory(this, sizeof(CXTPLogFont));
}

CXTPLogFont::CXTPLogFont(LOGFONT& logfont)
: dwType(0)
{
	MEMCPY_S((void*)&*this, (const void*)&logfont, (DWORD)sizeof(LOGFONT));
}

void CXTPLogFont::operator=(LOGFONT& logfont)
{
	MEMCPY_S((void*)&*this, (const void*)&logfont, (DWORD)sizeof(LOGFONT));
}

//=============================================================================
// CXTPAuxData
//=============================================================================

CXTPAuxData::CXTPAuxData()
{
	iComCtlVersion = -1;
	hcurDragCopy = NULL;
	hcurDragMove = NULL;
	hcurDragNone = NULL;
	hcurHand = NULL;
	hcurHSplitBar = NULL;
	hcurVSplitBar = NULL;
	hcurMove = NULL;
	bUseOfficeFont = FALSE;
	bUseSolidShadows = FALSE;
	bMenuShadows = TRUE;

	bWin95 = XTPSystemVersion()->IsWin95();
	bWin98 = XTPSystemVersion()->IsWin98();
	bWinNT = XTPSystemVersion()->IsWinNT4();
	bWin2K = XTPSystemVersion()->IsWin2K();
	bWinXP = XTPSystemVersion()->IsWinXP();

	// Internet Explorer 4.0 or higher.
	GetComCtlVersion();
	VERIFY(IsComCtlValid());

	UpdateSysColors();
	UpdateSysMetrics();

	// Load cursors and fonts.
	LoadSysCursors();
	LoadSysFonts();
}

CXTPAuxData::~CXTPAuxData()
{
	FreeSysFonts();
}

// If LoadCursors() returns FALSE and your application is linking to the
// static version of the library you most likely need to include the following
// line in your .rc2 file: #include "XTResourcePro.rc".  This name will vary
// depending on the library you are linking to.

BOOL CXTPAuxData::LoadSysCursors()
{
	hcurDragNone = XTPResourceManager()->LoadCursor(XTP_IDC_DRAGNONE);

	if (hcurDragNone == NULL)
		return FALSE;

	hcurDragCopy = XTPResourceManager()->LoadCursor(XTP_IDC_DRAGCOPY);

	if (hcurDragCopy == NULL)
		return FALSE;

	hcurDragMove = XTPResourceManager()->LoadCursor(XTP_IDC_DRAGMOVE);

	if (hcurDragMove == NULL)
		return FALSE;

	hcurHand = AfxGetApp()->LoadStandardCursor(MAKEINTRESOURCE(32649));

	if (hcurHand == NULL)
		hcurHand = XTPResourceManager()->LoadCursor(XTP_IDC_HAND);

	if (hcurHand == NULL)
		return FALSE;

	hcurHSplitBar = XTPResourceManager()->LoadCursor(XTP_IDC_HSPLITBAR);

	if (hcurHSplitBar == NULL)
		return FALSE;

	hcurVSplitBar = XTPResourceManager()->LoadCursor(XTP_IDC_VSPLITBAR);

	if (hcurVSplitBar == NULL)
		return FALSE;

	hcurMove = AfxGetApp()->LoadStandardCursor(IDC_SIZEALL);

	if (hcurMove == NULL)
		return FALSE;

	return TRUE;
}

void CXTPAuxData::UpdateSysColors()
{
	RefreshXtremeColors();

	// Initialize standard color for windows components.
	clr3DFace = GetXtremeColor(COLOR_3DFACE);
	clr3DShadow = GetXtremeColor(COLOR_3DSHADOW);
	clr3DDkShadow = GetXtremeColor(COLOR_3DDKSHADOW);
	clr3DHilight = GetXtremeColor(COLOR_3DHILIGHT);
	clr3DLight = GetXtremeColor(COLOR_3DLIGHT);
	clrBtnText = GetXtremeColor(COLOR_BTNTEXT);
	clrGrayText = GetXtremeColor(COLOR_GRAYTEXT);
	clrHighlight = GetXtremeColor(COLOR_HIGHLIGHT);
	clrHighlightText = GetXtremeColor(COLOR_HIGHLIGHTTEXT);
	clrMenu = GetXtremeColor(COLOR_MENU);
	clrMenuText = GetXtremeColor(COLOR_MENUTEXT);
	clrWindow = GetXtremeColor(COLOR_WINDOW);
	clrWindowFrame = GetXtremeColor(COLOR_WINDOWFRAME);
	clrWindowText = GetXtremeColor(COLOR_WINDOWTEXT);
	clrActiveCaption = GetXtremeColor(COLOR_ACTIVECAPTION);
	clrInActiveCaption = GetXtremeColor(COLOR_INACTIVECAPTION);
	clrGradActiveCapt = GetXtremeColor(COLOR_GRADIENTACTIVECAPTION);
	clrGradInActiveCapt = GetXtremeColor(COLOR_GRADIENTINACTIVECAPTION);
	clrActiveCaptText = GetXtremeColor(COLOR_CAPTIONTEXT);
	clrInactiveCaptText = GetXtremeColor(COLOR_INACTIVECAPTIONTEXT);

	// Initialize special colors for XP style interfaces.
	clrXPBarFace = GetXtremeColor(XPCOLOR_TOOLBAR_FACE);
	clrXPHighlight = GetXtremeColor(XPCOLOR_HIGHLIGHT);
	clrXPHighlightBorder = GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER);
	clrXPHighlightPushed = GetXtremeColor(XPCOLOR_HIGHLIGHT_PUSHED);
	clrXPIconShadow = GetXtremeColor(XPCOLOR_ICONSHADDOW);
	clrXPGrayText = GetXtremeColor(XPCOLOR_GRAYTEXT);
	clrXPHighlightChecked = GetXtremeColor(XPCOLOR_HIGHLIGHT_CHECKED);
	clrXPHighlightCheckedBorder = GetXtremeColor(XPCOLOR_HIGHLIGHT_CHECKED_BORDER);
	clrXPGripper = GetXtremeColor(XPCOLOR_TOOLBAR_GRIPPER);
	clrXPSeparator = GetXtremeColor(XPCOLOR_SEPARATOR);
	clrXPDisabled = GetXtremeColor(XPCOLOR_DISABLED);
	clrXPMenuTextBack = GetXtremeColor(XPCOLOR_MENUBAR_FACE);
	clrXPMenuExpanded = GetXtremeColor(XPCOLOR_MENUBAR_EXPANDED);
	clrXPMenuBorder = GetXtremeColor(XPCOLOR_MENUBAR_BORDER);
	clrXPMenuText = GetXtremeColor(XPCOLOR_MENUBAR_TEXT);
	clrXPHighlightText = GetXtremeColor(XPCOLOR_HIGHLIGHT_TEXT);
	clrXPBarText = GetXtremeColor(XPCOLOR_TOOLBAR_TEXT);
	clrXPBarTextPushed = GetXtremeColor(XPCOLOR_PUSHED_TEXT);
	clrXPTabInactiveBack = GetXtremeColor(XPCOLOR_TAB_INACTIVE_BACK);
	clrXPTabInactiveText = GetXtremeColor(XPCOLOR_TAB_INACTIVE_TEXT);
}

void CXTPAuxData::UpdateSysMetrics()
{
	cxSmIcon = ::GetSystemMetrics(SM_CXSMICON);
	cySmIcon = ::GetSystemMetrics(SM_CYSMICON);
	cxHThumb = ::GetSystemMetrics(SM_CXHTHUMB);
	cyVThumb = ::GetSystemMetrics(SM_CYVTHUMB);
	cyMenuItem = ::GetSystemMetrics(SM_CYMENU);
	cxSize = 18; //::GetSystemMetrics(SM_CXSIZE);
	cySize = 18; //::GetSystemMetrics(SM_CYSIZE);
}

BOOL CXTPAuxData::LoadSysFonts(LPCTSTR strHorzFaceName/*= _T("")*/, LPCTSTR strVertFaceName/*= _T("")*/)
{
	CString strFaceTahoma(_T("Tahoma"));
	CString strFaceArial(_T("Arial"));

	// Free up any resources previously allocated.
	FreeSysFonts();

	CXTPNonClientMetrics ncm;

	LOGFONT lfMenuFont;
	MEMCPY_S(&lfMenuFont, &ncm.lfMenuFont, sizeof(LOGFONT));
	lfMenuFont.lfCharSet = XTPResourceManager()->GetFontCharset();

	if (strHorzFaceName != NULL)
	{
		if (FontExists(strHorzFaceName))
		{
			STRCPY_S(lfMenuFont.lfFaceName, LF_FACESIZE, strHorzFaceName);
			STRCPY_S(ncm.lfStatusFont.lfFaceName, LF_FACESIZE, strHorzFaceName);
			STRCPY_S(ncm.lfSmCaptionFont.lfFaceName, LF_FACESIZE, strHorzFaceName);
		}
	}

	// check to see if we can use the "Tahoma" font.
	else if (bUseOfficeFont && !(ncm.lfMenuFont.lfCharSet > SYMBOL_CHARSET))
	{
		if (FontExists(strFaceTahoma))
		{
			STRCPY_S(lfMenuFont.lfFaceName, LF_FACESIZE, strFaceTahoma);
		}
	}

	// create the menu fonts.
	if (!CreateSysFont(lfMenuFont, font))
		return FALSE;

	if (!CreateSysFont(lfMenuFont, fontBold, FW_BOLD))
		return FALSE;

	if (!CreateSysFont(lfMenuFont, fontULine, -1, 1))
		return FALSE;

	// If the "Tahoma" font is not used see if we can use "Arial" for vertical
	// fonts because it displays better than MS Sans Serif on older operating
	// systems...

	if (strVertFaceName != NULL)
	{
		if (FontExists(strVertFaceName))
		{
			STRCPY_S(lfMenuFont.lfFaceName, LF_FACESIZE, strVertFaceName);
		}
	}

	else if (strFaceTahoma.CompareNoCase(ncm.lfMenuFont.lfFaceName) != 0)
	{
		if (FontExists(strFaceArial))
		{
			STRCPY_S(lfMenuFont.lfFaceName, LF_FACESIZE, strFaceArial);
		}
	}

	// create the vertical menu fonts.
	if (!CreateSysFont(lfMenuFont, fontVert, -1, -1, 900, 2700))
		return FALSE;

	if (!CreateSysFont(lfMenuFont, fontVertBold, FW_BOLD, -1, 900, 2700))
		return FALSE;

	// create the icon title fonts.
	CXTPLogFont lfIconTitleFont;
	VERIFY(CXTPDrawHelpers::GetIconLogFont(&lfIconTitleFont));

	if (!CreateSysFont(lfIconTitleFont, fontIconTitle))
		return FALSE;

	if (!CreateSysFont(lfIconTitleFont, fontIconTitleBold, FW_BOLD))
		return FALSE;

	// Create the status bar font.
	if (!CreateSysFont(ncm.lfStatusFont, fontStatus))
		return FALSE;

	// Create the small caption font.
	if (!CreateSysFont(ncm.lfSmCaptionFont, fontSmCaption))
		return FALSE;

	// Create the marlett icon font.
	LOGFONT lfMarlettIcon = {lfIconTitleFont.lfHeight, 0, 0, 0, 0, 0, 0, 0,
		DEFAULT_CHARSET, 0, 0, 0, 0, _T("Marlett")};

	if (!CreateSysFont(lfMarlettIcon, fontMarlettIcon))
		return FALSE;

	return TRUE;
}

void CXTPAuxData::FreeSysFonts()
{
	if (font.GetSafeHandle())
		font.DeleteObject();
	if (fontBold.GetSafeHandle())
		fontBold.DeleteObject();
	if (fontULine.GetSafeHandle())
		fontULine.DeleteObject();
	if (fontVert.GetSafeHandle())
		fontVert.DeleteObject();
	if (fontVertBold.GetSafeHandle())
		fontVertBold.DeleteObject();
	if (fontIconTitle.GetSafeHandle())
		fontIconTitle.DeleteObject();
	if (fontIconTitleBold.GetSafeHandle())
		fontIconTitleBold.DeleteObject();
	if (fontMarlettIcon.GetSafeHandle())
		fontMarlettIcon.DeleteObject();
	if (fontStatus.GetSafeHandle())
		fontStatus.DeleteObject();
	if (fontSmCaption.GetSafeHandle())
		fontSmCaption.DeleteObject();
}

void CXTPAuxData::UseOfficeFont(BOOL bOfficeFont)
{
	bUseOfficeFont = bOfficeFont;
	LoadSysFonts();
}

BOOL CXTPAuxData::FontExists(LPCTSTR strFaceName)
{
	return CXTPDrawHelpers::FontExists(strFaceName);
}

BOOL CXTPAuxData::CreateSysFont(const CXTPLogFont& logFont, CFont& font, long lfWeight/*= -1*/, char lfUnderline/*= -1*/, long lfOrientation/*= -1*/, long lfEscapement/*= -1*/)
{
	// delete the font if it has already been created.
	if (font.GetSafeHandle())
	{
		font.DeleteObject();
	}

	// construct the log font.
	CXTPLogFont lf(logFont);

	// set font weight.
	if (lfWeight != -1)
	{
		lf.lfWeight = lfWeight;
	}

	// set font orientation.
	if (lfOrientation != -1)
	{
		lf.lfOrientation = lfOrientation;
	}

	// set font escapement.
	if (lfEscapement != -1)
	{
		lf.lfEscapement = lfEscapement;
	}

	// set font underline style.
	if (lfUnderline != -1)
	{
		lf.lfUnderline = lfUnderline;
	}

	// create the font using the log font we constructed.
	return font.CreateFontIndirect(&lf);
}

BOOL CXTPAuxData::SetGlobalFont(LPCTSTR lpszFaceName, LPCTSTR lpszVertFaceName/*= NULL*/)
{
	return LoadSysFonts(lpszFaceName, lpszVertFaceName);
}

BOOL CXTPAuxData::SetGlobalFont(CFont* pHorzFont, CFont* pVertFont/*= NULL*/)
{
	if (pHorzFont != NULL && pHorzFont->GetSafeHandle() != NULL)
	{
		CXTPLogFont lf;
		pHorzFont->GetLogFont(&lf);

		// create the menu fonts.
		if (!CreateSysFont(lf, font))
			return FALSE;

		if (!CreateSysFont(lf, fontBold, FW_BOLD))
			return FALSE;

		if (!CreateSysFont(lf, fontULine, -1, 1))
			return FALSE;

		if (!CreateSysFont(lf, fontStatus))
			return FALSE;
	}

	if (pVertFont != NULL && pVertFont->GetSafeHandle() != NULL)
	{
		CXTPLogFont lf;
		pVertFont->GetLogFont(&lf);

		if (!CreateSysFont(lf, fontVert, -1, -1, 900, 2700))
			return FALSE;

		if (!CreateSysFont(lf, fontVertBold, FW_BOLD, -1, 900, 2700))
			return FALSE;
	}

	return TRUE;
}


DWORD CXTPAuxData::GetComCtlVersion()
{
	iComCtlVersion = XTPSystemVersion()->GetComCtlVersion();
	return iComCtlVersion;
}

BOOL CXTPAuxData::IsComCtlValid() const
{
	return TRUE;
}

CString CXTPAuxData::XTPGetVersion(bool bVerNumOnly/*= false*/)
{
	CString strVersion;
	if (bVerNumOnly)
	{
		strVersion.Format(_T("%d.%02d"),
			_XTPLIB_VERSION_MAJOR, _XTPLIB_VERSION_MINOR);
	}
	else
	{
		strVersion.Format(_T("Xtreme Toolkit v%d.%02d"),
			_XTPLIB_VERSION_MAJOR, _XTPLIB_VERSION_MINOR);
	}
	return strVersion;
}

void CXTPAuxData::InitResources(HINSTANCE hInst)
{
	// Set the resource handle used by the library.
	XTPResourceManager()->SetResourceHandle(hInst);

	LoadSysCursors(); // Load cursors.
}

_XTP_EXT_CLASS CXTPAuxData& AFXAPI XTPAuxData()
{
	static CXTPAuxData instance;
	return instance;
}

// CXTPTcbItem

CXTPTcbItem::CXTPTcbItem()
: uiToolTipId(0)
, pWnd(0)
, dwStyle(0)
, dwExStyle(0)
, crTabBack(COLORREF_NULL)
, crTabText(COLORREF_NULL)
, crTabSelBack(COLORREF_NULL)
, crTabSelText(COLORREF_NULL)
{

}
