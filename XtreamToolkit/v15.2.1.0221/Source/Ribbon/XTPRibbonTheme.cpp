// XTPRibbonTheme.cpp: implementation of the CXTPRibbonTheme class.
//
// This file is a part of the XTREME RIBBON MFC class library.
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

#include "Common/XTPImageManager.h"
#include "Common/XTPResourceImage.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPHookManager.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPVC80Helpers.h"


#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPCommandBars.h"
#include "CommandBars/XTPCommandBar.h"
#include "CommandBars/XTPToolBar.h"
#include "CommandBars/XTPMenuBar.h"
#include "CommandBars/XTPPopupBar.h"
#include "CommandBars/XTPControls.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPControlButton.h"
#include "CommandBars/XTPControlPopup.h"
#include "CommandBars/XTPControlEdit.h"
#include "CommandBars/XTPControlComboBox.h"
#include "CommandBars/XTPControlProgress.h"

#include "CommandBars/XTPScrollBase.h"
#include "CommandBars/XTPStatusBar.h"

#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPOffice2003Theme.h"
#include "CommandBars/XTPResourceTheme.h"

#include "XTPRibbonTheme.h"

#ifdef _XTP_INCLUDE_RIBBON
#include "XTPRibbonGroup.h"
#include "XTPRibbonPopups.h"
#include "XTPRibbonBar.h"
#include "XTPRibbonPaintManager.h"
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CXTPRibbonTheme, CXTPResourceTheme)

CXTPRibbonTheme::CXTPRibbonTheme()
{
	m_pFramePaintManager = new CXTPFramePaintManager(this);
	m_pFramePaintManager->RefreshMetrics();

	m_pFramePaintManager->m_bFrameStatusBar = TRUE;


	m_bClearTypeTextQuality = TRUE;

	m_nSplitButtonDropDownWidth = 12;
	m_nSplitButtonPopupWidth = 26;
	m_nStatusBarMinHeight = 22;

	m_strOfficeFont = _T("Segoe UI");
	m_bUseOfficeFont = TRUE;

	m_bOffsetPopupLabelText = FALSE;

	m_rcStatusBarBorder.SetRect(0, 0, 0, 1);

	m_pShadowManager->SetShadowOptions(xtpShadowOfficeAlpha);

}

CXTPRibbonTheme::~CXTPRibbonTheme()
{

}

void CXTPRibbonTheme::SetRibbonPaintManager(CXTPRibbonPaintManager* pRibbonPaintManager)
{
#ifdef _XTP_INCLUDE_RIBBON
	CMDTARGET_RELEASE(m_pRibbonPaintManager);

	m_pRibbonPaintManager = pRibbonPaintManager;
	m_pRibbonPaintManager->RefreshMetrics();
#else
	UNREFERENCED_PARAMETER(pRibbonPaintManager);
#endif
}


AFX_INLINE LPCTSTR FormatName(LPCTSTR lpszName)
{
	return lpszName;
}

AFX_INLINE CString FormatName(LPCTSTR lpszFormat, LPCTSTR lpszParameter)
{
	CString strResult;
	strResult.Format(lpszFormat, lpszParameter);
	return strResult;
}

AFX_INLINE BOOL IsCompositeRect2(CXTPCommandBar* pRibbonBar, const CRect& rc)
{
	if (pRibbonBar->IsDwmEnabled())
		return TRUE;

#ifdef _XTP_INCLUDE_RIBBON
	if (!pRibbonBar || !pRibbonBar->IsRibbonBar() || !((CXTPRibbonBar*)pRibbonBar)->IsDwmEnabled())
		return FALSE;

	return CRect().IntersectRect(((CXTPRibbonBar*)pRibbonBar)->GetCaptionRect(), rc);
#else
	UNREFERENCED_PARAMETER(pRibbonBar);
	UNREFERENCED_PARAMETER(rc);
	return FALSE;
#endif
}


AFX_INLINE DWORD GetDrawImageFlags(CXTPControl* pControl, const CRect& rc)
{
	return IsCompositeRect2(pControl->GetParent(), rc) ? XTP_DI_COMPOSITE : 0;
}

AFX_INLINE DWORD GetDrawImageFlags(CXTPControl* pControl)
{
	return IsCompositeRect2(pControl->GetParent(), pControl->GetRect()) ? XTP_DI_COMPOSITE : 0;
}


void CXTPRibbonTheme::UpdateFonts()
{
	CNonClientMetrics ncm;
	CLogFont lf;
	CWindowDC dc(NULL);

	int nBaseHeight = ncm.lfMenuFont.lfHeight < 0 ? -ncm.lfMenuFont.lfHeight : ncm.lfMenuFont.lfHeight;
	if (m_nFontHeight > 11)
		nBaseHeight = m_nFontHeight;

	if (nBaseHeight <= 12) nBaseHeight = 11;

	int nLogPixel = 96;

	lf.lfWeight = FW_NORMAL;
	lf.lfItalic = FALSE;
	lf.lfCharSet = XTPResourceManager()->GetFontCharset();
	lf.lfQuality = ncm.lfMenuFont.lfQuality;

	if (m_bClearTypeTextQuality && XTPSystemVersion()->IsClearTypeTextQualitySupported())
	{
		lf.lfQuality = 5;
		ncm.lfSmCaptionFont.lfQuality = 5;
		ncm.lfStatusFont.lfQuality = 5;
	}
	if (m_bUseOfficeFont && FontExists(m_strOfficeFont))
	{
		STRCPY_S(lf.lfFaceName, LF_FACESIZE, m_strOfficeFont);
		STRCPY_S(ncm.lfStatusFont.lfFaceName, LF_FACESIZE, m_strOfficeFont);
	}
	else
	{
		STRCPY_S(lf.lfFaceName, LF_FACESIZE, ncm.lfMenuFont.lfFaceName);
	}

	lf.lfHeight = nBaseHeight * 120 / nLogPixel;

	if (m_bUseStandardFont)
	{
		LOGFONT lfFont = lf;
		SetCommandBarsFontIndirect(&lfFont, TRUE);
	}

	m_nTextHeight = max(22, nBaseHeight * 144 / nLogPixel + 6);

	m_nEditHeight = max(22, nBaseHeight * 195 / nLogPixel);


	m_nSplitButtonDropDownWidth = max(12, m_nTextHeight / 2);


	LOGFONT lfIcon = lf;
	VERIFY(m_fontIcon.SetStandardFont(&lfIcon));

	lfIcon.lfWeight = FW_BOLD;
	VERIFY(m_fontIconBold.SetStandardFont(&lfIcon));

	ncm.lfSmCaptionFont.lfCharSet = XTPResourceManager()->GetFontCharset();
	m_fontSmCaption.SetStandardFont(&ncm.lfSmCaptionFont);

	ncm.lfStatusFont.lfCharSet = XTPResourceManager()->GetFontCharset();
	m_fontToolTip.SetStandardFont(&ncm.lfStatusFont);
}

void CXTPRibbonTheme::RefreshMetrics()
{
	CXTPResourceTheme::RefreshMetrics();

	CXTPResourceImages* pImages = GetImages();

	pImages->AssertValid();

	m_bOffice2010Style = GetImages()->GetImageInt(_T("Ribbon"), _T("GroupSpacing")) == 0;


	m_arrColor[XPCOLOR_TOOLBAR_TEXT].SetStandardValue(pImages->GetImageColor(_T("Ribbon"), _T("GroupClientText")));
	m_arrColor[XPCOLOR_TOOLBAR_GRAYTEXT].SetStandardValue(RGB(141, 141, 141));

	m_clrControlEditNormal = pImages->GetImageColor(_T("Ribbon"), _T("ControlEditNormal"));
	m_clrControlEditSelected = pImages->GetImageColor(_T("Ribbon"), _T("ControlEditSelected"), RGB(255, 255, 255));

	m_arrColor[XPCOLOR_MENUBAR_TEXT].SetStandardValue(pImages->GetImageColor(_T("Ribbon"), _T("MenuPopupText")));

	m_clrMenuGripper.SetStandardValue(pImages->GetImageColor(_T("Ribbon"), _T("MenuPopupGripper")));
	m_clrMenuExpandedGripper.SetStandardValue(pImages->GetImageColor(_T("Ribbon"), _T("MenuPopupExpandedGripperLight")),
		pImages->GetImageColor(_T("Ribbon"), _T("MenuPopupExpandedGripperDark")));

	m_pShadowManager->SetShadowColor(0);

	m_arrColor[XPCOLOR_MENUBAR_GRAYTEXT].SetStandardValue(RGB(167, 167, 167));
	m_arrColor[XPCOLOR_HIGHLIGHT_TEXT].SetStandardValue(pImages->GetImageColor(_T("Ribbon"), _T("ControlHighlightText")));

	m_arrColor[XPCOLOR_MENUBAR_BORDER].SetStandardValue(RGB(134, 134, 134));
	m_arrColor[XPCOLOR_MENUBAR_FACE].SetStandardValue(RGB(250, 250, 250));

	m_clrMenuPopupSeparator = pImages->GetImageColor(_T("Ribbon"), _T("MenuPopupSeparator"), RGB(197, 197, 197));

	m_clrMenuExpand.SetStandardValue(RGB(233, 238, 238), RGB(233, 238, 238));

	m_clrRibbonText = pImages->GetImageColor(_T("Ribbon"), _T("RibbonText"));

	m_clrRibbonGrayText = pImages->GetImageColor(_T("Ribbon"), _T("RibbonGrayText"), RGB(141, 141, 141));

	m_clrMenuPopupGripperShadow = pImages->GetImageColor(_T("Ribbon"), _T("MenuPopupGripperShadow"), RGB(197, 197, 197));

	if (m_bOffice2010Style)
	{
		m_pFramePaintManager->m_bFrameStatusBar = FALSE;
		m_pFramePaintManager->m_bRoundedCornersAlways = TRUE;
	}
	else
	{
		m_pFramePaintManager->m_bFrameStatusBar = TRUE;
		m_pFramePaintManager->m_bRoundedCornersAlways = FALSE;
	}

	m_clrControlEditDisabledBorder = pImages->GetImageColor(_T("Toolbar"), _T("ControlEditDisabledBorder"), GetXtremeColor(XPCOLOR_EDITCTRLBORDER));


	CreateGradientCircle();
}

//////////////////////////////////////////////////////////////////////////
// CommandBars Theme



BOOL CXTPRibbonTheme::IsImagesAvailable()
{
	return TRUE;
}

void CXTPRibbonTheme::DrawControlEntry(CDC* pDC, CXTPControl* pButton)
{
	CXTPResourceTheme::DrawControlEntry(pDC, pButton);
}

COLORREF CXTPRibbonTheme::GetControlTextColor(CXTPControl* pButton)
{
#ifdef _XTP_INCLUDE_RIBBON
	if (pButton->GetRibbonGroup() == NULL && pButton->GetParent()->IsRibbonBar())
	{
		CXTPRibbonBar* pRibbonBar = (CXTPRibbonBar*)pButton->GetParent();

		BOOL bCheckBoxStyle = (pButton->GetType() == xtpControlCheckBox || pButton->GetType() == xtpControlRadioButton);
		if (pButton->IsKindOf(RUNTIME_CLASS(CXTPRibbonGroupControlPopup)))
			bCheckBoxStyle = TRUE;

		BOOL bNormalText = bCheckBoxStyle ? pButton->GetEnabled() :
			!pButton->GetSelected() && pButton->GetEnabled() &&
			!pButton->GetPressed() && !pButton->GetChecked() && !pButton->GetPopuped();

		if (bNormalText && pRibbonBar->IsQuickAccessControl(pButton))
		{
			if (pButton->HasDwmCompositedRect())
			{
				if (XTPSystemVersion()->IsWin7OrGreater())
					return RGB(0, 0, 0);

				return pRibbonBar->GetSite()->GetStyle() & WS_MAXIMIZE ? RGB(255, 255, 255) : RGB(0, 0, 0);
			}

			return m_clrRibbonText;
		}

		if (bNormalText)
		{
			return m_clrRibbonText;
		}

		if (!pButton->GetEnabled())
		{
			return m_clrRibbonGrayText;
		}
	}
#endif

	return CXTPResourceTheme::GetControlTextColor(pButton);
}

COLORREF CXTPRibbonTheme::GetRectangleTextColor(BOOL bSelected, BOOL bPressed, BOOL bEnabled, BOOL bChecked, BOOL bPopuped, XTPBarType barType, XTPBarPosition /*barPosition*/)
{
	if (barType == xtpBarTypeMenuBar && !bSelected && bEnabled && !bPressed && !bChecked && !bPopuped)
	{
		return m_clrMenuBarText;
	}

	if (barType == xtpBarTypePopup)
	{
		return GetXtremeColor(!bEnabled ? XPCOLOR_MENUBAR_GRAYTEXT : XPCOLOR_MENUBAR_TEXT);
	}

	return GetXtremeColor(!bEnabled ? XPCOLOR_TOOLBAR_GRAYTEXT : XPCOLOR_TOOLBAR_TEXT);
}

void CXTPRibbonTheme::DrawRectangle(CDC* pDC, CRect rc, BOOL bSelected, BOOL bPressed, BOOL bEnabled /*= TRUE*/, BOOL bChecked /*= FALSE*/, BOOL bPopuped /*= FALSE*/, XTPBarType barType, XTPBarPosition barPosition)
{
	if (!IsImagesAvailable())
	{
		CXTPOffice2003Theme::DrawRectangle(pDC, rc, bSelected, bPressed, bEnabled, bChecked, bPopuped, barType, barPosition);
		return;
	}

	if (barType != xtpBarTypePopup)
	{
		BOOL bSmallSize = rc.Height() < 33;

		CXTPResourceImage* pImage = LoadImage(FormatName(bSmallSize ? _T("TOOLBARBUTTONS22") : _T("TOOLBARBUTTONS50")));
		ASSERT(pImage);
		if (!pImage)
			return;

		int nState = -1;

		if (!bEnabled)
		{
			if (IsKeyboardSelected(bSelected)) nState = 0;
		}
		else if (bPopuped) nState = 2;
		else if (bChecked && !bSelected && !bPressed) nState = 2;
		else if (bChecked && bSelected && !bPressed) nState = 3;
		else if (IsKeyboardSelected(bPressed) || (bSelected && bPressed)) nState = 1;
		else if (bSelected || bPressed) nState = 0;

		if (nState != -1)
		{
			pImage->DrawImage(pDC, rc, pImage->GetSource(nState, 4), CRect(8, 8, 8, 8), COLORREF_NULL);
		}
	}
	else if (barPosition == xtpBarPopup && bChecked && barType == xtpBarTypePopup)
	{
		CXTPResourceImage* pImage = LoadImage(FormatName(_T("MENUCHECKEDITEM")));
		ASSERT(pImage);
		if (!pImage)
			return;

		pImage->DrawImage(pDC, rc, pImage->GetSource(bEnabled ? 0 : 1, 2), CRect(2, 2, 2, 2));
	}
	else if (barPosition == xtpBarPopup && bSelected && barType == xtpBarTypePopup)
	{
		if (!bEnabled && !IsKeyboardSelected(bSelected))
			return;

		BOOL bSmallSize = rc.Height() < 33;
		CXTPResourceImage* pImage = LoadImage(FormatName(
			!bEnabled ? bSmallSize ? _T("MENUSELECTEDDISABLEDITEM22") : _T("MENUSELECTEDDISABLEDITEM54") :
				bSmallSize ? _T("MENUSELECTEDITEM22") : _T("MENUSELECTEDITEM54")));
		ASSERT(pImage);
		if (!pImage)
			return;

		pImage->DrawImage(pDC, rc, pImage->GetSource(), CRect(4, 4, 4, 4));

	}
	else
	{
		CXTPOffice2003Theme::DrawRectangle(pDC, rc, bSelected, bPressed, bEnabled, bChecked, bPopuped, barType, barPosition);
	}
}


void CXTPRibbonTheme::DrawSplitButtonFrame(CDC* pDC, CXTPControl* pButton, CRect rcButton)
{
	if (!IsImagesAvailable())
	{
		CXTPOffice2003Theme::DrawSplitButtonFrame(pDC, pButton, rcButton);
		return;
	}

	BOOL bPressed = pButton->GetPressed(), bSelected = pButton->GetSelected(), bEnabled = pButton->GetEnabled(), bChecked = pButton->GetChecked(),
		bPopuped = pButton->GetPopuped();

	BOOL bSmallSize = pButton->GetStyle() != xtpButtonIconAndCaptionBelow;

	CXTPResourceImage* pImageSplit, *pImageSplitDropDown;

	int nSpecialOffset = 0;

#ifdef _XTP_INCLUDE_RIBBON
	if (pButton->GetRibbonGroup() && pButton->GetRibbonGroup()->IsControlsGrouping())
	{
		bSmallSize = TRUE;
		BOOL bLeft = pButton->GetBeginGroup() || pButton->GetWrap();

		int nIndexNext = pButton->GetControls()->GetNext(pButton->GetIndex());

		BOOL bRight = nIndexNext <= pButton->GetIndex() ||
			pButton->GetControls()->GetAt(nIndexNext)->GetBeginGroup() ||
			pButton->GetControls()->GetAt(nIndexNext)->GetWrap();

		CString strImage = bLeft ? FormatName(_T("TOOLBARBUTTONSSPECIALSPLITLEFT")) :
			FormatName(_T("TOOLBARBUTTONSSPECIALSPLITCENTER"));

		pImageSplit = LoadImage(strImage);

		strImage = bRight ? FormatName(_T("TOOLBARBUTTONSSPECIALSPLITDROPDOWNRIGHT")) :
			FormatName(_T("TOOLBARBUTTONSSPECIALSPLITDROPDOWNCENTER"));
		pImageSplitDropDown  = LoadImage(strImage);

		nSpecialOffset = 1;
	}
	else
#endif
	{
		pImageSplit = LoadImage(bSmallSize ? FormatName(_T("TOOLBARBUTTONSSPLIT22")) : FormatName(_T("TOOLBARBUTTONSSPLIT50")));

		pImageSplitDropDown = LoadImage(bSmallSize ? FormatName(_T("TOOLBARBUTTONSSPLITDROPDOWN22")) : FormatName(_T("TOOLBARBUTTONSSPLITDROPDOWN50")));
	}

	if (!pImageSplit || !pImageSplitDropDown)
		return;

	int nSplitDropDownHeight = GetSplitDropDownHeight();

	COLORREF clrText = !bEnabled ? RGB(141, 141, 141) : RGB(21, 66, 139);

	pDC->SetTextColor(clrText);
	pDC->SetBkMode (TRANSPARENT);

	CRect rcSplit = bSmallSize ? CRect(rcButton.left, rcButton.top, rcButton.right - m_nSplitButtonDropDownWidth, rcButton.bottom) :
		CRect(rcButton.left, rcButton.top, rcButton.right, rcButton.bottom - nSplitDropDownHeight);

	CRect rcSplitSrc  = pImageSplit->GetSource(0, 6 + nSpecialOffset);

	CRect rcSplitDropDown = bSmallSize ? CRect(rcButton.right - m_nSplitButtonDropDownWidth, rcButton.top, rcButton.right, rcButton.bottom) :
		CRect(rcButton.left, rcButton.bottom - nSplitDropDownHeight, rcButton.right, rcButton.bottom);

	CRect rcSplitDropDownSrc = pImageSplitDropDown->GetSource(0, 5 + nSpecialOffset);

	int nStateSplit = -1;

	BOOL bMouseInSplit = bSelected == TRUE_SPLITCOMMAND;
	BOOL bMouseInSplitDropDown = bSelected == TRUE_SPLITDROPDOWN;

	if (bEnabled == TRUE_SPLITDROPDOWN && (bSelected || bPressed || bPopuped))
	{
		nStateSplit = 4;
	}
	else if (!bEnabled)
	{
		if (IsKeyboardSelected(bSelected)) nStateSplit = 5;
	}
	else if (bChecked)
	{
		if (bPopuped) nStateSplit = (m_bOffice2010Style ? 1 : 5);
		else if (!bSelected && !bPressed) nStateSplit = 2;
		else if (bSelected && !bPressed) nStateSplit = 3;
		else if (IsKeyboardSelected(bPressed) || (bSelected && bPressed)) nStateSplit = 1;
		else if (bPressed) nStateSplit = 2;
		else if (bSelected || bPressed) nStateSplit = !bMouseInSplit ? 5 : 0;
	}
	else
	{
		if (bPopuped) nStateSplit = (m_bOffice2010Style ? 1 : 5);
		else if (IsKeyboardSelected(bPressed) || (bSelected && bPressed)) nStateSplit = 1;
		else if (bSelected || bPressed) nStateSplit = !bMouseInSplit ? 5 : 0;
	}
	nStateSplit += nSpecialOffset;

	if (nStateSplit != -1)
	{
		rcSplitSrc.OffsetRect(0, rcSplitSrc.Height() * nStateSplit);
		pImageSplit->DrawImage(pDC, rcSplit, rcSplitSrc,
			CRect(2, 2, 2, 2), 0xFF00FF, GetDrawImageFlags(pButton, rcSplit));
	}

	int nStateSplitDropDown = -1;

	if (bEnabled == TRUE_SPLITCOMMAND && (bSelected || bPressed || bPopuped))
	{
		nStateSplitDropDown = 3;
	}
	else if (!bEnabled)
	{
		if (IsKeyboardSelected(bSelected)) nStateSplitDropDown = 4;
	}
	else if (bChecked)
	{
		if (bPopuped) nStateSplitDropDown = 2;
		else if (!bSelected && !bPressed) nStateSplitDropDown = 2;
		else if (IsKeyboardSelected(bPressed) || (bSelected && bPressed)) nStateSplitDropDown = 0;
		else if (bSelected || bPressed) nStateSplitDropDown = !bMouseInSplitDropDown ? (m_bOffice2010Style ? 2 : 4) : 0;
		else nStateSplitDropDown = (m_bOffice2010Style ? 2 : 4);
	}
	else
	{
		if (bPopuped) nStateSplitDropDown = 2;
		else if (IsKeyboardSelected(bPressed) || (bSelected && bPressed)) nStateSplitDropDown = (m_bOffice2010Style ? 4 : 0);
		else if (bSelected || bPressed) nStateSplitDropDown = !bMouseInSplitDropDown ? 4 : 0;
	}

	nStateSplitDropDown += nSpecialOffset;

	if (nStateSplitDropDown != -1)
	{
		rcSplitDropDownSrc.OffsetRect(0, rcSplitDropDownSrc.Height() * nStateSplitDropDown);
		pImageSplitDropDown->DrawImage(pDC, rcSplitDropDown, rcSplitDropDownSrc, CRect(2, 2, 2, 2), 0xFF00FF,
			GetDrawImageFlags(pButton, rcSplitDropDown));
	}

	if (bSmallSize)
	{
		if (bEnabled == TRUE_SPLITCOMMAND) bEnabled = FALSE;

		CPoint pt = CPoint(rcButton.right - m_nSplitButtonDropDownWidth / 2 - 1, rcButton.CenterPoint().y);
		DrawDropDownGlyph(pDC, pButton, pt, bSelected, bPopuped, bEnabled, FALSE);
	}
}


void CXTPRibbonTheme::DrawPopupBarGripper(CDC* pDC, int x, int y, int cx, int cy, BOOL bExpanded)
{
	XTPDrawHelpers()->GradientFill(pDC, CRect(CPoint(x + 1, y), CSize(cx - 1, cy)),
		bExpanded ? m_clrMenuExpandedGripper : m_clrMenuGripper, TRUE);

	pDC->FillSolidRect(x + cx - 1, y, 1, cy, m_clrMenuPopupGripperShadow);
	pDC->FillSolidRect(x + cx, y, 1, cy, RGB(245, 245, 245));
}

void CXTPRibbonTheme::FillPopupLabelEntry(CDC* pDC, CRect rc)
{
	pDC->FillSolidRect(rc, GetXtremeColor(XPCOLOR_LABEL));
	pDC->FillSolidRect(rc.left, rc.bottom - 1, rc.Width(), 1, RGB(197, 197, 197));
}


CSize CXTPRibbonTheme::DrawSpecialControl(CDC* pDC, XTPSpecialControl controlType, CXTPControl* pButton, CXTPCommandBar* pBar, BOOL bDraw, LPVOID lpParam)
{
	if (controlType == xtpButtonExpandToolbar)
	{
		BOOL bVert = IsVerticalPosition(pBar->GetPosition());

		if (!bDraw)
		{
			CSize szBar = *(CSize*)lpParam;
			if (!bVert)
				pButton->SetRect(CRect(szBar.cx - 12, 0, szBar.cx, szBar.cy));
			else
				pButton->SetRect(CRect(0, szBar.cy - 12, szBar.cx, szBar.cy));
		}
		else
		{
			CRect rc = pButton->GetRect();

			if (pBar->IsDwmEnabled())
			{
				CXTPResourceImage* pImage = LoadImage(_T("RIBBONQUICKACCESSBUTTON"));
				if (!pImage)
					return CXTPResourceTheme::DrawSpecialControl(pDC, controlType, pButton, pBar, bDraw, lpParam);

				CRect rcSrc(pImage->GetSource(
					pButton->GetPopuped() ? 2 : pButton->GetSelected() ? 1 : 0 , 5));

				CRect rcDest(CPoint((rc.right + rc.left - rcSrc.Width()) / 2, (rc.top + rc.bottom - rcSrc.Height()) / 2), rcSrc.Size());

				pImage->DrawImage(pDC, rcDest, rcSrc, CRect(3, 3, 3, 3), RGB(255, 0, 255), XTP_DI_COMPOSITE);
			}
			else
			{
				CXTPResourceImage* pImage = LoadImage(FormatName(
					bVert ? _T("TOOLBAREXPANDBUTTONVERTICAL") : _T("TOOLBAREXPANDBUTTONHORIZONTAL")));

				if (!pImage)
					return CXTPResourceTheme::DrawSpecialControl(pDC, controlType, pButton, pBar, bDraw, lpParam);

				pImage->DrawImage(pDC, rc, pImage->GetSource(
					pButton->GetPopuped() ? 2 : pButton->GetSelected() ? 1 : 0 , 3), CRect(3, 3, 3, 3), RGB(255, 0, 255));

				COLORREF clrText = GetXtremeColor(pButton->GetPopuped() ? XPCOLOR_TOOLBAR_TEXT: pButton->GetSelected() ? XPCOLOR_HIGHLIGHT_TEXT : XPCOLOR_TOOLBAR_TEXT);
				BOOL bHiddenExists = *(BOOL*)lpParam;

				if (!bVert) rc.OffsetRect(-1, 0); else rc.OffsetRect(0, -1);
				DrawExpandSymbols(pDC, IsVerticalPosition(pBar->GetPosition()), CRect(CPoint(rc.left + 1, rc.top + 1), rc.Size()) , bHiddenExists, GetXtremeColor(COLOR_BTNHIGHLIGHT));
				DrawExpandSymbols(pDC, IsVerticalPosition(pBar->GetPosition()), rc, bHiddenExists, clrText);
			}
		}
		return 0;
	}

	return CXTPResourceTheme::DrawSpecialControl(pDC, controlType, pButton, pBar, bDraw, lpParam);
}

void CXTPRibbonTheme::FillCommandBarEntry(CDC* pDC, CXTPCommandBar* pBar)
{
	CRect rc;
	pBar->GetClientRect(&rc);

	if (pBar->IsDwmEnabled())
	{
		pDC->FillSolidRect(rc, 0);
		return;
	}

	if (pBar->GetPosition() == xtpBarPopup)
	{
		CXTPResourceImage* pImage = LoadImage(FormatName(_T("POPUPBARFRAME")));
		ASSERT(pImage);
		if (!pImage)
			return;

		pImage->DrawImage(pDC, rc, pImage->GetSource(), CRect(4, 4, 4, 4));
	}
	else if (IsFlatToolBar(pBar))
	{
		CXTPOffice2003Theme::FillCommandBarEntry(pDC, pBar);
	}
	else if (pBar->GetPosition() == xtpBarTop || pBar->GetPosition() == xtpBarBottom)
	{
		CXTPResourceImage* pImage = LoadImage(FormatName(_T("TOOLBARFACEHORIZONTAL")));

		if (!pImage)
		{
			CXTPOffice2003Theme::FillCommandBarEntry(pDC, pBar);
			return;
		}

		pImage->DrawImage(pDC, rc, pImage->GetSource(), CRect(5, 5, 5, 5));
	}
	else if (pBar->GetPosition() == xtpBarLeft || pBar->GetPosition() == xtpBarRight)
	{
		CXTPResourceImage* pImage = LoadImage(FormatName(_T("TOOLBARFACEVERTICAL")));

		if (!pImage)
		{
			CXTPOffice2003Theme::FillCommandBarEntry(pDC, pBar);
			return;
		}

		pImage->DrawImage(pDC, rc, pImage->GetSource(), CRect(5, 5, 5, 5));
	}
	else
	{
		CXTPOffice2003Theme::FillCommandBarEntry(pDC, pBar);
	}
}

CSize CXTPRibbonTheme::DrawCommandBarSeparator(CDC* pDC, CXTPCommandBar* pBar, CXTPControl* pControl, BOOL bDraw)
{
	if (pBar->GetType() == xtpBarTypePopup)
	{
		if (!bDraw)
			return CSize(2, 2);

		CRect rc;
		pBar->GetClientRect(&rc);
		CRect rcControl = pControl->GetRect();

		if (!pControl->GetWrap())
		{
			int nLeft = rcControl.left - 1 + GetPopupBarGripperWidth(pBar) + m_nPopupBarTextPadding;
			HorizontalLine(pDC, nLeft, rcControl.top - 2, rcControl.right, m_clrMenuPopupSeparator);
			HorizontalLine(pDC, nLeft, rcControl.top - 1, rcControl.right, RGB(245, 245, 245));
		}
		else
		{
			VerticalLine(pDC, rcControl.left - 2, pControl->GetRowRect().top - 1, pControl->GetRowRect().bottom + 1, m_clrMenuPopupSeparator);
		}

		return CSize(2, 2);
	}
	else if (bDraw && pBar->GetPosition() == xtpBarTop && pBar->IsDwmEnabled() && !pControl->GetWrap())
	{
		CRect rcControl = pControl->GetRect();
		CRect rcRow = pControl->GetRowRect();

		CXTPResourceImage* pImage = LoadImage(FormatName(_T("SYSTEMICONSEPARATOR")));
		if (pImage)
		{
			pImage->DrawImage(pDC, CRect(rcControl.left - 4, rcRow.top + m_nToolbarSeparatorMargin + 1, rcControl.left - 1, rcRow.bottom - m_nToolbarSeparatorMargin + 1),
				pImage->GetSource());
		}
		return 0;
	}
	else
	{
		return CXTPOffice2003Theme::DrawCommandBarSeparator(pDC, pBar, pControl, bDraw);
	}
}

CSize CXTPRibbonTheme::DrawCommandBarGripper(CDC* pDC, CXTPCommandBar* pBar, BOOL bDraw)
{
	if (bDraw && pDC && pBar->GetPosition() == xtpBarTop && pBar->IsDwmEnabled())
	{
		CRect rc;
		pBar->GetClientRect(&rc);

		for (int y = 5; y < rc.Height() - 7; y += 4)
		{
			if (!m_bFlatGripper) FillCompositeRect(pDC, CRect(7, y + 1, 9, y + 3), GetXtremeColor(COLOR_WINDOW));
			FillCompositeRect(pDC, CRect(6, y, 8, y + 2), GetXtremeColor(XPCOLOR_TOOLBAR_GRIPPER));
		}

		return 0;

	}
	else
	{
		return CXTPResourceTheme::DrawCommandBarGripper(pDC, pBar, bDraw);
	}
}

void CXTPRibbonTheme::FillStatusBar(CDC* pDC, CXTPStatusBar* pBar)
{
	CXTPClientRect rc(pBar);

	int nWidth = rc.Width() - (::GetSystemMetrics(SM_CXVSCROLL) + ::GetSystemMetrics(SM_CXBORDER) * 2);

	if (pBar->GetPaneCount() > 0)
	{
		int nIndex = min(pBar->GetPaneCount() - 1, pBar->GetRibbonDividerIndex() + 1);
		for (;nIndex < pBar->GetPaneCount(); nIndex++)
		{
			CXTPStatusBarPane* pPane = pBar->GetPane(nIndex);
			if (!pPane->IsVisible())
				continue;

			nWidth = pPane->GetRect().left - 3;
			break;
		}
	}

	CXTPResourceImage* pImage = LoadImage(FormatName(_T("STATUSBARLIGHT")));

	ASSERT(pImage);
	if (!pImage)
		return;

	CRect rcSrc(0, 0, pImage->GetWidth(), pImage->GetHeight());
	CRect rcLight(rc.left, rc.top, rc.left + nWidth - 3, rc.bottom);
	pImage->DrawImage(pDC, rcLight, rcSrc, CRect(0, 0, 0, 0));


	pImage = LoadImage(FormatName(_T("STATUSBARDEVIDER")));
	rcSrc.SetRect(0, 0, pImage->GetWidth(), pImage->GetHeight());
	CRect rcDevider(rcLight.right, rc.top, rcLight.right + rcSrc.Width(), rc.bottom);
	pImage->DrawImage(pDC, rcDevider, rcSrc, CRect(0, 0, 0, 0));


	pImage = LoadImage(FormatName(_T("STATUSBARDARK")));
	CRect rcDark(rcDevider.right, rc.top, rc.right, rc.bottom);
	rcSrc.SetRect(0, 0, pImage->GetWidth(), pImage->GetHeight());
	pImage->DrawImage(pDC, rcDark, rcSrc, CRect(0, 0, 0, 0));
}

void CXTPRibbonTheme::DrawStatusBarPaneBorder(CDC* pDC, CRect rc, CXTPStatusBarPane* pPane, BOOL /*bGripperPane*/)
{
	if (pPane->GetIndex() >= pPane->GetStatusBar()->GetRibbonDividerIndex())
		return;

	CXTPResourceImage* pImage = LoadImage(FormatName(_T("STATUSBARSEPARATOR")));

	ASSERT(pImage);
	if (!pImage)
		return;

	rc.left = rc.right - 2;
	rc.top -= 2;

	pImage->DrawImage(pDC, rc, pImage->GetSource(), CRect(0, 2, 0, 2));
}

void CXTPRibbonTheme::DrawStatusBarPaneSeparator(CDC* pDC, CRect rc, CXTPStatusBarPane* pPane)
{
	if (pPane->GetIndex() > pPane->GetStatusBar()->GetRibbonDividerIndex())
		return;

	CXTPResourceImage* pImage = LoadImage(FormatName(_T("STATUSBARSEPARATOR")));

	ASSERT(pImage);
	if (!pImage)
		return;

	CRect rcSrc(pImage->GetSource());
	rc.left = rc.left - 2;
	rc.right = rc.left + 2;
	rc.top -= 2;

	pImage->DrawImage(pDC, rc, pImage->GetSource(), CRect(0, 2, 0, 2));
}


void CXTPRibbonTheme::DrawStatusBarGripper(CDC* pDC, CRect rcClient)
{
	CXTPResourceImage* pImage = LoadImage(FormatName(_T("STATUSBARGRIPPER")));

	ASSERT(pImage);
	if (!pImage)
		return;

	CRect rcSrc(0, 0, pImage->GetWidth(), pImage->GetHeight());
	CRect rcGripper(rcClient.right - rcSrc.Width() , rcClient.bottom - rcSrc.Height() - 4, rcClient.right, rcClient.bottom - 4);
	pImage->DrawImage(pDC, rcGripper, rcSrc, CRect(0, 0, 0, 0), RGB(0xFF, 0, 0xFF));
}

int CXTPRibbonTheme::DrawStatusBarSwitchPane(CDC* pDC, CRect rcItem, CXTPStatusBarSwitchPane* pPane)
{
	int nWidth = 0;
	int nCount = pPane->GetSwitchCount();

	if (pDC)
	{
		for (int i = 0; i < nCount; i++)
		{
			CXTPStatusBarSwitchPane::SWITCH* pSwitch = pPane->GetSwitch(i);

			rcItem.right = rcItem.left + pSwitch->nWidth;
			BOOL bEnabled = pPane->IsEnabled() && pSwitch->bEnabled;

			CXTPResourceImage* pImage = LoadImage(FormatName(_T("STATUSBARSWITCHES")));

			if (pImage)
			{
				int nState = !bEnabled ? 5 : pSwitch->bPressed ? 2 : pSwitch->bChecked ? (pSwitch->bHighlighted ? 4 : 3) : pSwitch->bHighlighted ? 1 : 0;

				CRect rcSrc(pImage->GetSource(nState, 6));
				int nSrcWidth = rcSrc.Width() / 4;
				int nImage = nCount == 1 ? 3 : i == 0 ? 0 : i == pPane->GetSwitchCount() - 1 ? 2 : 1;
				if (pPane->GetStatusBar()->GetExStyle() & WS_EX_LAYOUTRTL) nImage = 3 - nImage;
				pImage->PreserveOrientation();

				rcSrc.left = nSrcWidth * nImage;
				rcSrc.right = rcSrc.left + nSrcWidth;

				pImage->DrawImage(pDC, rcItem, rcSrc, CRect(2, 2, 2, 2), RGB(0xFF, 0, 0xFF));
			}
			else
			{
				if (bEnabled && (pSwitch->bHighlighted || pSwitch->bPressed || pSwitch->bChecked))
				{
					DrawRectangle(pDC, rcItem, pSwitch->bHighlighted, pSwitch->bPressed, TRUE,
						pSwitch->bChecked, FALSE, xtpBarTypeNormal, xtpBarBottom);
				}

			}


			CXTPImageManagerIcon* pIcon = pPane->GetStatusBar()->GetImageManager()->GetImage(pSwitch->nID, ICON_SMALL);
			if (pIcon)
			{
				CPoint pt((rcItem.left + rcItem.right - pIcon->GetWidth()) / 2, (rcItem.top + rcItem.bottom - pIcon->GetHeight()) / 2);
				pIcon->Draw(pDC, pt, pIcon->GetIcon(!bEnabled ? xtpImageDisabled : pSwitch->bChecked ? xtpImageChecked : xtpImageNormal));
			}

			rcItem.left = rcItem.right;
		}

	}
	else
	{
		for (int i = 0; i < nCount; i++)
		{
			CXTPStatusBarSwitchPane::SWITCH* pSwitch = pPane->GetSwitch(i);

			pSwitch->nWidth = i == 0 || i == nCount - 1 ? 19 : 18;

			nWidth += pSwitch->nWidth;
		}
	}

	return nWidth;
}

CSize CXTPRibbonTheme::DrawControlCheckBoxMark(CDC* pDC, CRect rc, BOOL bDraw, BOOL bSelected, BOOL bPressed, BOOL bChecked, BOOL bEnabled)
{
	if (!bDraw)
		return CSize(13, 13);

	CXTPResourceImage* pImage = LoadImage(FormatName(_T("TOOLBARBUTTONCHECKBOX")));

	ASSERT(pImage);
	if (!pImage)
		return CSize(13, 13);

	int nState = 0;
	if (!bEnabled)
		nState = 3;
	else if (bSelected && bPressed)
		nState = 2;
	else if (bSelected)
		nState = 1;

	if (bChecked == 1)
		nState += 4;

	if (bChecked == 2)
		nState += 8;

	pImage->DrawImage(pDC, rc, pImage->GetSource(nState, 12), CRect(0, 0, 0, 0));


	return CSize(13, 13);
}

CSize CXTPRibbonTheme::DrawControlRadioButtonMark(CDC* pDC, CRect rc, BOOL bDraw, BOOL bSelected, BOOL bPressed, BOOL bChecked, BOOL bEnabled)
{
	if (!bDraw)
		return CSize(13, 13);

	CXTPResourceImage* pImage = LoadImage(FormatName(_T("TOOLBARBUTTONRADIOBUTTON")));

	ASSERT(pImage);
	if (!pImage)
		return CSize(13, 13);

	int nState = 0;
	if (!bEnabled)
		nState = 3;
	else if (bSelected && bPressed)
		nState = 2;
	else if (bSelected)
		nState = 1;

	if (bChecked)
		nState += 4;

	pImage->DrawImage(pDC, rc, pImage->GetSource(nState, 8), CRect(0, 0, 0, 0));


	return CSize(13, 13);
}

void CXTPRibbonTheme::DrawDropDownGlyph(CDC* pDC, CXTPControl* pControl, CPoint pt, BOOL bSelected, BOOL bPopuped, BOOL bEnabled, BOOL bVert)
{
	if (bVert)
	{
		CXTPOffice2003Theme::DrawDropDownGlyph(pDC, pControl, pt, bSelected, bPopuped, bEnabled, bVert);
		return;
	}

	CXTPResourceImage* pImage = LoadImage(FormatName(_T("TOOLBARBUTTONDROPDOWNGLYPH")));

	ASSERT(pImage);
	if (!pImage)
		return;

	CRect rc(pt.x - 2, pt.y - 2, pt.x + 3, pt.y + 2);
	pImage->DrawImage(pDC, rc, pImage->GetSource(!bEnabled ? 3 : bSelected ? 1 : 0, 4),
		CRect(0, 0, 0, 0), 0xFF00FF, GetDrawImageFlags(pControl, rc)) ;
}

CSize CXTPRibbonTheme::DrawControlComboBox(CDC* pDC, CXTPControlComboBox* pControlCombo, BOOL bDraw)
{
	if (!bDraw)
	{
		return CXTPPaintManager::DrawControlComboBox(pDC, pControlCombo, bDraw);
	}

	BOOL bPopupBar = pControlCombo->GetParent()->GetType() == xtpBarTypePopup;

	CXTPFontDC font(pDC, GetIconFont());

	CRect rc = pControlCombo->GetRect();
	BOOL bDropped = pControlCombo->GetDroppedState() != 0;
	BOOL bSelected = pControlCombo->GetSelected();
	BOOL bEnabled = pControlCombo->GetEnabled();

	CXTPPaintManager::DrawControlComboBox(pDC, pControlCombo, bDraw);

	rc.left += pControlCombo->GetLabelWidth();

	int nThumb = pControlCombo->GetThumbWidth();
	CRect rcBtn (rc.right - nThumb, rc.top, rc.right, rc.bottom);
	CRect rcText(rc.left + 3, rc.top + 1, rc.right - rcBtn.Width(), rc.bottom - 1);

	CXTPResourceImage* pImage = LoadImage(FormatName(_T("TOOLBARBUTTONSCOMBODROPDOWN")));

	ASSERT(pImage);
	if (!pImage)
		return 0;

	COLORREF clrBackground = GetControlEditBackColor(pControlCombo);


	if (!bEnabled)
	{
		pDC->FillSolidRect(rc, GetXtremeColor(COLOR_3DFACE));

		COLORREF clrBorder = IsKeyboardSelected(bSelected) ? GetXtremeColor(XPCOLOR_EDITCTRLBORDER) : m_clrControlEditDisabledBorder;
		pDC->Draw3dRect(rc, clrBorder, clrBorder);

		pImage->DrawImage(pDC, rcBtn, pImage->GetSource(4, 5), CRect(2, 2, 2, 2));
	}
	else if (bDropped)
	{
		pDC->FillSolidRect(rc, clrBackground);
		pDC->Draw3dRect(rc, GetXtremeColor(XPCOLOR_EDITCTRLBORDER), GetXtremeColor(XPCOLOR_EDITCTRLBORDER));

		pImage->DrawImage(pDC, rcBtn, pImage->GetSource(3, 5), CRect(2, 2, 2, 2));
	}
	else if (bSelected)
	{
		pDC->FillSolidRect(rc, clrBackground);
		pDC->Draw3dRect(rc, GetXtremeColor(XPCOLOR_EDITCTRLBORDER), GetXtremeColor(XPCOLOR_EDITCTRLBORDER));

		int nState = !pControlCombo->GetEditCtrl() || pControlCombo->IsFocused() ? 2 : bSelected == TRUE ? 1 : 2;
		pImage->DrawImage(pDC, rcBtn, pImage->GetSource(nState, 5), CRect(2, 2, 2, 2));

	}
	else
	{
		if (!bPopupBar) pDC->FillSolidRect(rc, clrBackground);
		pDC->Draw3dRect(rc, GetXtremeColor(XPCOLOR_EDITCTRLBORDER), GetXtremeColor(XPCOLOR_EDITCTRLBORDER));

		pImage->DrawImage(pDC, rcBtn, pImage->GetSource(0, 5), CRect(2, 2, 2, 2));
	}

	pControlCombo->DrawEditText(pDC, rcText);

	CPoint pt = rcBtn.CenterPoint();
	DrawDropDownGlyph(pDC, pControlCombo, pt, bSelected, bDropped, bEnabled, FALSE);

	if (pControlCombo->HasDwmCompositedRect())
	{
		FillCompositeAlpha(pDC, rc);
	}

	return 0;
}

COLORREF CXTPRibbonTheme::GetControlEditBackColor(CXTPControl* pControl)
{
	return !pControl->GetEnabled() ? GetXtremeColor(COLOR_3DFACE) :
		pControl->GetSelected() ? m_clrControlEditSelected :
		pControl->GetParent()->GetType() == xtpBarTypePopup ?
			GetXtremeColor(XPCOLOR_MENUBAR_FACE) : m_clrControlEditNormal;
}

CSize CXTPRibbonTheme::DrawControlEdit(CDC* pDC, CXTPControlEdit* pControlEdit, BOOL bDraw)
{
	if (!bDraw)
	{
		return CXTPPaintManager::DrawControlEdit(pDC, pControlEdit, bDraw);
	}

	BOOL bPopupBar = pControlEdit->GetParent()->GetType() == xtpBarTypePopup;
	BOOL bSelected = pControlEdit->GetSelected(), bEnabled = pControlEdit->GetEnabled();

	if (bPopupBar) pDC->SetTextColor(GetXtremeColor(!pControlEdit->GetEnabled() ? XPCOLOR_MENUBAR_GRAYTEXT: bSelected ? XPCOLOR_HIGHLIGHT_TEXT : XPCOLOR_MENUBAR_TEXT));
	else pDC->SetTextColor(GetXtremeColor(!pControlEdit->GetEnabled() ? XPCOLOR_TOOLBAR_GRAYTEXT : XPCOLOR_TOOLBAR_TEXT));

	CXTPPaintManager::DrawControlEdit(pDC, pControlEdit, bDraw);

	CRect rc = pControlEdit->GetRect();
	rc.left += pControlEdit->GetLabelWidth();

	COLORREF clrBackground = GetControlEditBackColor(pControlEdit);

	if (!bEnabled)
	{
		rc.DeflateRect(1, 0, 0, 0);

		pDC->FillSolidRect(rc, clrBackground);
		COLORREF clrBorder = IsKeyboardSelected(bSelected) ? GetXtremeColor(XPCOLOR_EDITCTRLBORDER) : m_clrControlEditDisabledBorder;
		pDC->Draw3dRect(rc, clrBorder, clrBorder);

	}
	else
	{
		if (bSelected)
		{
			pDC->FillSolidRect(rc, clrBackground);
			pDC->Draw3dRect(rc, GetXtremeColor(XPCOLOR_EDITCTRLBORDER), GetXtremeColor(XPCOLOR_EDITCTRLBORDER));
		}
		else
		{
			if (!bPopupBar) pDC->FillSolidRect(rc, clrBackground);
			pDC->Draw3dRect(rc, GetXtremeColor(XPCOLOR_EDITCTRLBORDER), GetXtremeColor(XPCOLOR_EDITCTRLBORDER));
		}
	}

	CRect rcText(pControlEdit->GetRect());
	rcText.DeflateRect(pControlEdit->GetLabelWidth() + 3, 1, 3, 1);
	pControlEdit->DrawEditText(pDC, rcText);

	if (pControlEdit->IsSpinButtonsVisible())
	{
		CXTPResourceImage* pImage = LoadImage(_T("TOOLBARBUTTONSPINARROWSVERTICAL"));
		if (!pImage)
			return 0;

		BOOL bPressed = pControlEdit->GetPressed();

		int nState = !bEnabled ? 4 : bSelected && bPressed == XTP_EDITSPIN_UP ? 3 : bSelected == XTP_EDITSPIN_UP ? 2 : bSelected ? 1 : 0;
		CRect rcTop(rc.right - 18, rc.top + 1, rc.right - 1, rc.CenterPoint().y);
		pImage->DrawImage(pDC, rcTop, pImage->GetSource(nState, 10), CRect(0, 0, 0, 0), 0xFF00FF);

		nState = !bEnabled ? 9 : bSelected && bPressed == XTP_EDITSPIN_DOWN ? 8 : bSelected == XTP_EDITSPIN_DOWN ? 7 : bSelected ? 6 : 5;
		CRect rcBottom(rcTop.left, rcTop.bottom, rcTop.right, rc.bottom - 1);
		pImage->DrawImage(pDC, rcBottom, pImage->GetSource(nState, 10), CRect(0, 0, 0, 0), 0xFF00FF);

		pImage = rcTop.Width() > 12 && rcTop.Height() > 12 ? LoadImage(_T("CONTROLGALLERYSCROLLARROWGLYPHS")) :
			LoadImage(_T("TOOLBARBUTTONSPINARROWGLYPHS"));

		if (!pImage)
			return 0;

		nState = !bEnabled ? 3 : bSelected && bPressed == XTP_EDITSPIN_UP ? 2 : bSelected == XTP_EDITSPIN_UP ? 2 : bSelected ? 1 :  0;
		CRect rcSrc = pImage->GetSource(nState, 16);
		CSize sz(rcSrc.Size());
		pImage->DrawImage(pDC, CRect( CPoint((rcTop.left + rcTop.right - sz.cx) / 2, (rcTop.top + rcTop.bottom + 1 - sz.cy) / 2), rcSrc.Size()), rcSrc, CRect(0, 0, 0, 0), 0xFF00FF);

		nState = 4 + (!bEnabled ? 3 : bSelected && bPressed == XTP_EDITSPIN_DOWN ? 2 : bSelected == XTP_EDITSPIN_DOWN ? 2 : bSelected ? 1 :  0);
		rcSrc = pImage->GetSource(nState, 16);
		pImage->DrawImage(pDC, CRect(CPoint((rcBottom.left + rcBottom.right - sz.cx) / 2, (rcBottom.top + rcBottom.bottom - sz.cy) / 2), rcSrc.Size()), rcSrc, CRect(0, 0, 0, 0), 0xFF00FF);

	}

	if (pControlEdit->IsBuddyButtonVisible())
	{
		BOOL bPressed = pControlEdit->GetPressed();
		CRect rc(pControlEdit->GetBuddyButtonRect());

		DrawRectangle(pDC, rc, bSelected == XTP_EDITBUDDYBUTTON, bPressed == XTP_EDITBUDDYBUTTON, bEnabled,  FALSE, FALSE, xtpBarTypeNormal, xtpBarTop);

		CXTPImageManager* pImageManager = pControlEdit->GetParent()->GetImageManager();

		CXTPImageManagerIcon* pIcon = pImageManager->GetImage(pControlEdit->GetBuddyButtonId());

		if (pIcon)
		{
			CSize sz(pIcon->GetExtent());
			pIcon->Draw(pDC, CPoint((rc.left + rc.right - sz.cx) / 2, (rc.top + rc.bottom - sz.cy) / 2),
				pIcon->GetIcon(!bEnabled ? xtpImageDisabled : pControlEdit->GetSelected() == XTP_EDITBUDDYBUTTON ?
xtpImageHot : xtpImageNormal));
		}
	}

	if (pControlEdit->HasDwmCompositedRect())
	{
		FillCompositeAlpha(pDC, rc);
	}

	return 0;
}

void CXTPRibbonTheme::DrawKeyboardTip(CDC* pDC, CXTPCommandBarKeyboardTip* pWnd, BOOL bSetRegion)
{
	if (bSetRegion)
	{
		SetRoundRectRegion(pWnd);
	}
	else
	{
		CXTPClientRect rc(pWnd);
		CXTPFontDC font(pDC, &m_fontToolTip);

		COLORREF clrLight = GetImages()->GetImageColor(_T("Window"), _T("TooltipLight"));
		COLORREF clrDark = GetImages()->GetImageColor(_T("Window"), _T("TooltipDark"));
		XTPDrawHelpers()->GradientFill(pDC, rc, clrLight, clrDark, FALSE);

		CXTPResourceImage* pImage = GetImages()->LoadFile(_T("TOOLTIPFRAME"));
		if (pImage)
		{
			pImage->DrawImage(pDC, rc, pImage->GetSource(0, 1), CRect(3, 3, 3, 3), 0xFF00FF);
		}
		else
		{
			COLORREF clrBorder = GetImages()->GetImageColor(_T("Window"), _T("TooltipBorder"));
			pDC->Draw3dRect(rc, clrBorder, clrBorder);
		}

		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(GetXtremeColor(pWnd->m_bEnabled ? COLOR_BTNTEXT : COLOR_GRAYTEXT));
		pDC->DrawText(pWnd->m_strTip, rc, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	}
}

void CXTPRibbonTheme::SetRoundRectRegion(CWnd* pWnd)
{
	CXTPWindowRect rc(pWnd);
	rc.OffsetRect(-rc.TopLeft());

	int cx = rc.Width(), cy = rc.Height();

	RECT rgn[] =
	{
		{1, 0, cx - 1, 1}, {0, 1, cx, cy - 1}, {1, cy - 1, cx - 1, cy}
	};

	int nSizeData = sizeof(RGNDATAHEADER) + sizeof(rgn);

	RGNDATA* pRgnData = (RGNDATA*)malloc(nSizeData);
	if (!pRgnData)
		return;
	MEMCPY_S(&pRgnData->Buffer, (void*)&rgn, sizeof(rgn));

	pRgnData->rdh.dwSize = sizeof(RGNDATAHEADER);
	pRgnData->rdh.iType = RDH_RECTANGLES;
	pRgnData->rdh.nCount = sizeof(rgn) / sizeof(RECT);
	pRgnData->rdh.nRgnSize = 0;
	pRgnData->rdh.rcBound = CRect(0, 0, cx, cy);

	CRgn rgnResult;
	VERIFY(rgnResult.CreateFromData(NULL, nSizeData, pRgnData));

	free(pRgnData);

	pWnd->SetWindowRgn((HRGN)rgnResult.Detach(), FALSE);
}

void CXTPRibbonTheme::SetCommandBarRegion(CXTPCommandBar* pCommandBar)
{
	if (pCommandBar->GetPosition() != xtpBarPopup)
		return;

	SetRoundRectRegion(pCommandBar);
}

void CXTPRibbonTheme::AdjustExcludeRect(CRect& /*rc*/, CXTPControl* /*pControl*/, BOOL /*bVertical*/)
{
}

void CXTPRibbonTheme::DrawSplitButtonPopup(CDC* pDC, CXTPControl* pButton)
{
	CRect rcButton = pButton->GetRect();
	BOOL bSelected = pButton->GetSelected(), bPopuped = pButton->GetPopuped(), bEnabled = pButton->GetEnabled();

	if (rcButton.Height() > 33 && bEnabled)
	{
		CXTPResourceImage* pImage = LoadImage(_T("MENUSPLITITEM"));
		if (pImage)
		{
			int nState = !bSelected  ? 0 : bSelected == TRUE_SPLITCOMMAND || (IsKeyboardSelected(bSelected) && !bPopuped) ? 2 : 1;

			pImage->DrawImage(pDC, CRect(rcButton.left, rcButton.top, rcButton.right - m_nSplitButtonPopupWidth, rcButton.bottom),
				pImage->GetSource(nState, 3), CRect(2, 2, 2, 2), 0xFF00FF);
		}
	}

	CXTPResourceImage* pImage = LoadImage(_T("MENUSPLITDROPDOWN"));

	int nState = !bSelected || !bEnabled ? 0 : bPopuped ? 2 : 1;

	pImage->DrawImage(pDC, CRect(rcButton.right - m_nSplitButtonPopupWidth, rcButton.top, rcButton.right, rcButton.bottom),
		pImage->GetSource(nState, 3), CRect(2, 2, 2, 2), 0xFF00FF);
}

void CXTPRibbonTheme::DrawControlPopupGlyph(CDC* pDC, CXTPControl* pButton)
{
	CRect rcButton = pButton->GetRect();

	CXTPResourceImage* pImage = LoadImage(_T("MENUPOPUPGLYPH"));
	CRect rcSrc(pImage->GetSource(pButton->GetEnabled() ? 0 : 1, 2));
	CPoint pt = CPoint(rcButton.right - 14, (rcButton.top + rcButton.bottom - rcSrc.Height()) / 2);

	pImage->DrawImage(pDC, CRect(pt, rcSrc.Size()), rcSrc, CRect(0, 0, 0, 0), 0xFF00FF);
}

void CXTPRibbonTheme::DrawPopupResizeGripper(CDC* pDC, CRect rcGripper, int nFlags)
{
	CXTPResourceImage* pImage = LoadImage(_T("POPUPBARRESIZEGRIPPERFACE"));
	ASSERT(pImage);
	if (!pImage)
	{
		CXTPOffice2003Theme::DrawPopupResizeGripper(pDC, rcGripper, nFlags);
		return;
	}

	pImage->DrawImage(pDC, rcGripper, pImage->GetSource(), CRect(1, 1, 1, 1), 0xFF00FF);

	if (nFlags == 2)
	{
		pImage = LoadImage(_T("POPUPBARRESIZEGRIPPERHEIGHT"));
		CRect rc = pImage->GetSource();
		CRect rcDest(CPoint(rcGripper.CenterPoint().x - rc.Width() / 2,
			rcGripper.bottom - rc.Height()), rc.Size());

		pImage->DrawImage(pDC, rcDest, rc, CRect(0, 0, 0, 0), 0xFF00FF);
	}
	else
	{
		pImage = LoadImage(_T("POPUPBARRESIZEGRIPPERWIDTH"));
		CRect rc = pImage->GetSource();
		CRect rcDest(CRect(CPoint(rcGripper.right - rc.Width(),
			rcGripper.bottom - rc.Height()), rc.Size()));

		pImage->DrawImage(pDC, rcDest, rc, CRect(0, 0, 0, 0), 0xFF00FF);
	}
}

void CXTPRibbonTheme::DrawPopupBarCheckMark(CDC* pDC, CRect rc, BOOL bEnabled, COLORREF clr)
{
	CXTPResourceImage* pImage = LoadImage(_T("MENUCHECKEDITEMMARK"));
	ASSERT(pImage);
	if (!pImage)
	{
		CXTPPaintManager::DrawPopupBarCheckMark(pDC, rc, bEnabled, clr);
		return;
	}

	CRect rcSrc = pImage->GetSource(bEnabled ? 0 : 1, 4);

	CRect rcDest(CPoint((rc.left + rc.right - rcSrc.Width()) / 2, (rc.top + rc.bottom - rcSrc.Height()) / 2), rcSrc.Size());

	pImage->DrawImage(pDC, rcDest, rcSrc, CRect(0, 0, 0, 0), 0xFF00FF);
}

void CXTPRibbonTheme::DrawPopupBarRadioMark(CDC* pDC, CRect rc, BOOL bEnabled, COLORREF clr)
{
	CXTPResourceImage* pImage = LoadImage(_T("MENUCHECKEDITEMMARK"));
	ASSERT(pImage);
	if (!pImage)
	{
		CXTPPaintManager::DrawPopupBarRadioMark(pDC, rc, bEnabled, clr);
		return;
	}

	CRect rcSrc = pImage->GetSource(bEnabled ? 2 : 3, 4);

	CRect rcDest(CPoint((rc.left + rc.right - rcSrc.Width()) / 2, (rc.top + rc.bottom - rcSrc.Height()) / 2), rcSrc.Size());

	pImage->DrawImage(pDC, rcDest, rcSrc, CRect(0, 0, 0, 0), 0xFF00FF);
}
