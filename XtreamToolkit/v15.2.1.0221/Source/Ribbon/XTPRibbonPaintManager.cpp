// XTPRibbonTheme.cpp: implementation of the CXTPRibbonPaintManager class.
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
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPWinThemeWrapper.h"

#include "TabManager/XTPTabManager.h"
#include "TabManager/XTPTabPaintManager.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPCommandBar.h"
#include "CommandBars/XTPToolBar.h"
#include "CommandBars/XTPMenuBar.h"
#include "CommandBars/XTPPopupBar.h"
#include "CommandBars/XTPControls.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPControlButton.h"
#include "CommandBars/XTPControlPopup.h"

#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPCommandBarsFrameHook.h"

#include "XTPRibbonGroup.h"
#include "XTPRibbonPopups.h"
#include "XTPRibbonBar.h"
#include "XTPRibbonPaintManager.h"
#include "XTPRibbonTab.h"
#include "XTPRibbonControlTab.h"
#include "XTPRibbonSystemButton.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CXTPRibbonPaintManager::CXTPRibbonPaintManager(CXTPPaintManager* pPaintManager)
{
	m_pPaintManger = pPaintManager;

	m_bHotTrackingGroups = TRUE;


	m_pTabPaintManager = CreateTabPaintManager();
	m_pTabPaintManager->RefreshMetrics();

	m_hApplicationIcon = 0;
	m_bAlphaApplicationIcon = FALSE;



}


CXTPRibbonPaintManager::~CXTPRibbonPaintManager()
{
	CMDTARGET_RELEASE(m_pTabPaintManager);
}

CXTPResourceImage* CXTPRibbonPaintManager::LoadImage(LPCTSTR lpszFileName)
{
	return GetImages()->LoadFile(lpszFileName);
}

CXTPResourceImages* CXTPRibbonPaintManager::GetImages() const
{
	return XTPResourceImages();
}

int CXTPRibbonPaintManager::GetFontHeight() const
{
	return m_pPaintManger->m_nFontHeight;
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

AFX_INLINE BOOL IsCompositeRect(CXTPCommandBar* pRibbonBar, const CRect& rc)
{
	if (!pRibbonBar || !pRibbonBar->IsRibbonBar() || !((CXTPRibbonBar*)pRibbonBar)->IsDwmEnabled())
		return FALSE;

	CRect rcCaption = ((CXTPRibbonBar*)pRibbonBar)->GetCaptionRect();

	if (((CXTPRibbonBar*)pRibbonBar)->GetRibbonPaintManager()->m_bFlatFrame)
		rcCaption.bottom += ((CXTPRibbonBar*)pRibbonBar)->GetTabsHeight();

	return CRect().IntersectRect(rcCaption, rc);
}

#ifdef _XTP_INCLUDE_RIBBON
BOOL CXTPControl::HasDwmCompositedRect() const
{
	if (!GetParent())
		return FALSE;

	if (IsCompositeRect(GetParent(), GetRect()))
		return TRUE;

	 if (GetParent()->IsDwmEnabled())
		return TRUE;

	return FALSE;
}
#endif

AFX_INLINE DWORD GetDrawImageFlags(CXTPControl* pControl, const CRect& rc)
{
	return IsCompositeRect(pControl->GetParent(), rc) ? XTP_DI_COMPOSITE : 0;
}

AFX_INLINE DWORD GetDrawImageFlags(CXTPControl* pControl)
{
	return IsCompositeRect(pControl->GetParent(), pControl->GetRect()) ? XTP_DI_COMPOSITE : 0;
}


AFX_INLINE LPCTSTR GetColorName(XTPRibbonTabContextColor color)
{
	switch (color)
	{
		case xtpRibbonTabContextColorBlue: return _T("BLUE");
		case xtpRibbonTabContextColorYellow: return _T("YELLOW");
		case xtpRibbonTabContextColorGreen: return _T("GREEN");
		case xtpRibbonTabContextColorRed: return _T("RED");
		case xtpRibbonTabContextColorPurple: return _T("PURPLE");
		case xtpRibbonTabContextColorCyan: return _T("CYAN");
		case xtpRibbonTabContextColorOrange: return _T("ORANGE");
	}

	return _T("");
}



//////////////////////////////////////////////////////////////////////////
// CXTPRibbonPaintManager

class CXTPRibbonPaintManager::CRibbonAppearanceSet : public CXTPTabPaintManager::CAppearanceSetPropertyPageFlat
{
public:
	CRibbonAppearanceSet(CXTPRibbonPaintManager* pRibbonTheme)
		: m_pRibbonTheme(pRibbonTheme)
	{
		m_rcHeaderMargin.SetRect(0, 1, 0, 0);
	}
protected:
	void DrawTabControl(CXTPTabManager* pTabManager, CDC* pDC, CRect rcControl);

	void DrawSingleButton(CDC* pDC, CXTPTabManagerItem* pItem);
	void DrawText(CDC* pDC, CXTPTabManager* pManager, const CString& str, LPRECT lpRect, UINT nFormat);

	int GetButtonHeight(const CXTPTabManager* /*pTabManager*/)
	{
		return m_pRibbonTheme->m_nTabsHeight + 1;
	}

	CXTPRibbonPaintManager* m_pRibbonTheme;

};

class CXTPRibbonPaintManager::CRibbonColorSet : public CXTPTabPaintManager::CColorSetOffice2003
{
protected:
	void RefreshMetrics()
	{
		CColorSetOffice2003::RefreshMetrics();
	}

	void SetTextColor(CDC* pDC, CXTPTabManagerItem* pItem)
	{
		CXTPRibbonBar* pRibbonBar = ((CXTPRibbonTab*)pItem)->GetRibbonBar();

		BOOL bSelected = pItem->IsSelected();
		BOOL bHighlighted = pItem->IsHighlighted();
		BOOL bFocused = pItem->IsFocused();

		if (bSelected && pRibbonBar->IsRibbonMinimized())
		{
			CXTPRibbonControlTab* pControlTab =  (CXTPRibbonControlTab*)pItem->GetTabManager();
			if (pControlTab->GetCommandBar() && pControlTab->GetCommandBar()->IsTrackingMode())
			{
				bFocused = bHighlighted = FALSE;
			}
			else if (pControlTab->GetSelected() && bFocused)
			{
				bFocused = bSelected = FALSE;
				bHighlighted = TRUE;
			}
			else
			{
				bFocused = bSelected = FALSE;
			}
		}

		if (bSelected && pRibbonBar->IsBackstageViewVisible())
			bSelected = FALSE;


		pDC->SetTextColor(!pItem->IsEnabled() ? m_clrDisabledText: bSelected ? m_clrSelectedText: bHighlighted ? m_clrHighlightText : m_clrNormalText);
	}

};

void CXTPRibbonPaintManager::FillRibbonTabControl(CDC* pDC, CXTPRibbonBar* pRibbonBar, CRect rcControl)
{
	int nCount = pRibbonBar->GetContextHeaders()->GetCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPRibbonTabContextHeader* pHeader =  pRibbonBar->GetContextHeaders()->GetHeader(i);
		if (!pHeader || !pHeader->m_pFirstTab || !pHeader->m_pLastTab || pHeader->m_color == xtpRibbonTabContextColorNone)
			continue;

		CRect rcFirst(pHeader->m_pFirstTab->GetRect());
		CRect rcLast(pHeader->m_pLastTab->GetRect());

		CXTPResourceImage* pImage = LoadImage(FormatName(_T("CONTEXTTAB%sBACKGROUND"), GetColorName(pHeader->m_color)));
		if (pImage) pImage->DrawImage(pDC, CRect(rcFirst.left, rcControl.top, rcLast.right, rcLast.bottom), pImage->GetSource(), CRect(2, 2, 2, 2));
	}
}

void CXTPRibbonPaintManager::DrawRibbonTab(CDC* pDC, CXTPRibbonTab* pItem)
{
	XTPTabPosition tabPosition = pItem->GetTabManager()->GetPosition();

	CRect rcEntry(pItem->GetRect());
	CXTPTabPaintManagerAppearanceSet::InflateRectEx(rcEntry, CRect(0, 1, -1,  -1), tabPosition);

	XTPRibbonTabContextColor tabColor = pItem->GetContextColor();

	CXTPRibbonBar* pRibbonBar = pItem->GetRibbonBar();

	BOOL bSelected = pItem->IsSelected();
	BOOL bHighlighted = pItem->IsHighlighted();
	BOOL bFocused = pItem->IsFocused();

	if (bSelected && pRibbonBar->IsRibbonMinimized())
	{
		CXTPRibbonControlTab* pControlTab =  (CXTPRibbonControlTab*)pItem->GetTabManager();
		if (pControlTab->GetCommandBar() && pControlTab->GetCommandBar()->IsTrackingMode())
		{
			bFocused = bHighlighted = FALSE;
		}
		else if (pControlTab->GetSelected() && bFocused)
		{
			bFocused = bSelected = FALSE;
			bHighlighted = TRUE;
		}
		else
		{
			bFocused = bSelected = FALSE;
		}
	}

	if (bSelected && pRibbonBar->IsBackstageViewVisible())
		bSelected = FALSE;


	if (tabColor != xtpRibbonTabContextColorNone)
	{
		if (bSelected || bHighlighted)
		{
			CXTPResourceImage* pImage = LoadImage(FormatName(_T("CONTEXTTAB%s"), GetColorName(tabColor)));

			if (pImage && pImage->GetHeight() < 70)
			{
				int nState = 0;
				if (bSelected)
				{
					nState = bFocused ? 2 : 1;
				}
				else if (bHighlighted)
				{
					nState = 0;
				}

				pImage->DrawImage(pDC, rcEntry, pImage->GetSource(nState, 3), CRect(5, 5, 5, 5));
			}
			else
			{
				int nState = 0;
				if (bSelected)
				{
					nState = bFocused || bHighlighted ? 4 : 3;
				}
				else if (bHighlighted)
				{
					nState = 1;
				}

				if (nState == 4)
				{
					pImage = LoadImage(FormatName(_T("RIBBONTAB")));
					if (pImage) pImage->DrawImage(pDC, rcEntry, pImage->GetSource(5, 6), CRect(5, 5, 5, 5), 0xFF00FF);
				}
				else
				{
					if (pImage) pImage->DrawImage(pDC, rcEntry, pImage->GetSource(nState, 5), CRect(5, 5, 5, 5));
				}
			}
		}
	}
	else
	{
		if (bSelected || bHighlighted)
		{
			CXTPResourceImage* pImage = LoadImage(FormatName(_T("RIBBONTAB")));

			int nState = 0;
			if (bSelected && bFocused)
			{
				nState = 5;
			}
			else if (bSelected && bHighlighted)
			{
				nState = 4;
			}
			else if (bSelected)
			{
				nState = 3;
			}
			else if (pItem->IsPressed())
			{
				nState = 2;
			}
			else if (bHighlighted)
			{
				nState = 1;
			}

			if (pImage) pImage->DrawImage(pDC, rcEntry, pImage->GetSource(nState, 6), CRect(5, 5, 5, 5), 0xFF00FF);
		}
	}

	if (pItem->GetButtonLength() < pItem->GetContentLength())
	{
		CXTPResourceImage* pImage = LoadImage(FormatName(_T("RIBBONTABSEPARATOR")));
		CRect rcSeparator(rcEntry.right, rcEntry.top, rcEntry.right + pImage->GetWidth(), rcEntry.bottom - 1);
		if (pImage) pImage->DrawImage(pDC, rcSeparator, pImage->GetSource(), CRect(0, 0, 0, 0));
	}

	if (tabColor != xtpRibbonTabContextColorNone && pItem->GetContextHeader() &&
		(pItem->GetContextHeader()->m_pFirstTab == pItem))
	{
		CRect rcSeparator(rcEntry.left, rcEntry.top, rcEntry.left + 1, rcEntry.bottom - 1);
		CXTPResourceImage* pImage = LoadImage(FormatName(_T("CONTEXTTABSEPARATOR")));
		if (pImage) pImage->DrawImage(pDC, rcSeparator, pImage->GetSource(), CRect(0, 0, 0, 0));
	}
	if (tabColor != xtpRibbonTabContextColorNone && pItem->GetContextHeader() &&
		(pItem->GetContextHeader()->m_pLastTab == pItem))
	{
		CRect rcSeparator(rcEntry.right, rcEntry.top, rcEntry.right + 1, rcEntry.bottom - 1);
		CXTPResourceImage* pImage = LoadImage(FormatName(_T("CONTEXTTABSEPARATOR")));
		if (pImage) pImage->DrawImage(pDC, rcSeparator, pImage->GetSource(), CRect(0, 0, 0, 0));
	}
}

void CXTPRibbonPaintManager::CRibbonAppearanceSet::DrawTabControl(CXTPTabManager* pTabManager, CDC* pDC, CRect rcControl)
{
	m_pRibbonTheme->FillRibbonTabControl(pDC, ((CXTPRibbonControlTab*)pTabManager)->GetRibbonBar(), rcControl);

	CAppearanceSetPropertyPageFlat::DrawTabControl(pTabManager, pDC, rcControl);
}

void CXTPRibbonPaintManager::CRibbonAppearanceSet::DrawSingleButton(CDC* pDC, CXTPTabManagerItem* pItem)
{
	m_pRibbonTheme->DrawRibbonTab(pDC, (CXTPRibbonTab*)pItem);

	m_pPaintManager->DrawSingleButtonIconAndText(pDC, pItem, pItem->GetRect() , TRUE);
}

void CXTPRibbonPaintManager::CRibbonAppearanceSet::DrawText(CDC* pDC, CXTPTabManager* pManager, const CString& str, LPRECT lpRect, UINT nFormat)
{
	CXTPRibbonControlTab* pControlTab = (CXTPRibbonControlTab*)pManager;
	CXTPRibbonBar* pRibbonBar = pControlTab->GetRibbonBar();

	if (nFormat & DT_CALCRECT)
		pDC->DrawText(str, lpRect, nFormat);
	else
	{
		BOOL bComposite =  IsCompositeRect(pRibbonBar, lpRect);
		CXTPPaintManager::DrawTextApi(pDC, str, lpRect, nFormat, bComposite);
	}
}

int CXTPRibbonPaintManager::GetEditHeight() const
{
	int nEditHeight = m_pPaintManger->GetEditHeight();
	return max(22, nEditHeight);
}

void CXTPRibbonPaintManager::SetFontHeight(int nFontHeight)
{
	m_pPaintManger->m_nFontHeight = nFontHeight;
	m_pPaintManger->RefreshMetrics();
}

void CXTPRibbonPaintManager::RefreshMetrics()
{
	CXTPPaintManager::CNonClientMetrics ncm;

	int nBaseHeight = ncm.lfMenuFont.lfHeight < 0 ? -ncm.lfMenuFont.lfHeight : ncm.lfMenuFont.lfHeight;

	int nFontHeiht = GetFontHeight();
	if (nFontHeiht > 11)
		nBaseHeight = nFontHeiht;

	if (nBaseHeight <= 12) nBaseHeight = 11;

	int nLogPixel = 96;

	LOGFONT lf;
	m_pPaintManger->GetRegularFont()->GetLogFont(&lf);

	m_fontGroupCaption.SetStandardFont(&lf);

	int nEditHeight = GetEditHeight();

	m_nTabsHeight = nEditHeight > 22 ? nEditHeight + 2 : 23;

	m_nGroupCaptionHeight = max(17, nBaseHeight * 144 / nLogPixel);


	CXTPResourceImages* pImages = GetImages();

	pImages->AssertValid();

	m_clrRibbonFace = GetImages()->GetImageColor(_T("Ribbon"), _T("RibbonFace"));
	m_clrRibbonInactiveFace= GetImages()->GetImageColor(_T("Ribbon"), _T("RibbonInactiveFace"), m_clrRibbonFace);

	m_pTabPaintManager->RefreshMetrics();
	m_pTabPaintManager->SetFontIndirect(&lf, TRUE);

	COLORREF clrTabNormalText = pImages->GetImageColor(_T("Ribbon"), _T("TabNormalText"));

	m_pTabPaintManager->GetColorSet()->m_clrHighlightText.SetStandardValue(
		pImages->GetImageColor(_T("Ribbon"), _T("TabHighlightText"), clrTabNormalText));
	m_pTabPaintManager->GetColorSet()->m_clrNormalText.SetStandardValue(clrTabNormalText);

	m_pTabPaintManager->GetColorSet()->m_clrSelectedText.SetStandardValue(pImages->GetImageColor(_T("Ribbon"), _T("TabSelectedText")));

	m_clrRibbonGroupCaptionText = pImages->GetImageColor(_T("Ribbon"), _T("GroupCaptionText"));


	m_clrMinimizedFrameEdgeHighLight = pImages->GetImageColor(_T("Ribbon"), _T("MinimizedFrameEdgeHighLight"));
	m_clrMinimizedFrameEdgeShadow = pImages->GetImageColor(_T("Ribbon"), _T("MinimizedFrameEdgeShadow"));

	m_clrRecentFileListEdgeShadow = GetImages()->GetImageColor(_T("Ribbon"), _T("RecentFileListEdgeShadow"));
	m_clrRecentFileListEdgeHighLight = GetImages()->GetImageColor(_T("Ribbon"), _T("RecentFileListEdgeHighLight"));

	m_nRibbonCaptionHeight = GetSystemMetrics(SM_CYCAPTION) + 1;
	m_nRibbonCaptionHeight = max(m_nRibbonCaptionHeight, 18);

	m_nGroupSpacing = GetImages()->GetImageInt(_T("Ribbon"), _T("GroupSpacing"), 2);

	m_bFlatFrame = GetImages()->GetImageInt(_T("Window"), _T("FlatFrame"), 0);

	m_clrFileButtonText = GetImages()->GetImageColor(_T("Ribbon"), _T("FileButtonText"));

	m_rcFileButtonMargin = GetImages()->GetImageRect(_T("Ribbon"), _T("FileButtonMargin"));

	m_nFileButtonImageCount = GetImages()->GetImageInt(_T("Ribbon"), _T("FileButtonImageCount"), 3);
}



CXTPTabPaintManager* CXTPRibbonPaintManager::CreateTabPaintManager()
{
	CXTPTabPaintManager* pTabPaintManager = new CXTPTabPaintManager();
	pTabPaintManager->SetAppearanceSet(new CRibbonAppearanceSet(this));
	pTabPaintManager->SetLayout(xtpTabLayoutSizeToFit);

	pTabPaintManager->SetColorSet(new CRibbonColorSet);
	pTabPaintManager->m_bFillBackground = FALSE;
	pTabPaintManager->m_bHotTracking = TRUE;
	pTabPaintManager->m_clientFrame = xtpTabFrameSingleLine;
	pTabPaintManager->m_bDrawTextNoPrefix = FALSE;
	pTabPaintManager->m_rcButtonMargin.SetRect(0, 2, 0, 2);
	pTabPaintManager->m_rcButtonTextPadding.SetRect(4, 0, 4, 0);
	pTabPaintManager->m_nButtonExtraLength = 21;
	pTabPaintManager->m_bDrawTextEndEllipsis = FALSE;
	pTabPaintManager->m_bDrawFocusRect = FALSE;
	pTabPaintManager->EnableToolTips(xtpTabToolTipShrinkedOnly);
	pTabPaintManager->m_bClearTypeTextQuality = m_pPaintManger->m_bClearTypeTextQuality;
	pTabPaintManager->m_bClipHeader = FALSE;

	return pTabPaintManager;
}

void CXTPRibbonPaintManager::FillGroupPopupToolBar(CDC* pDC, CXTPRibbonGroup* pGroup, CXTPCommandBar* pCommandBar)
{
	pDC->SetBkMode(TRANSPARENT);

	CXTPClientRect rcRibbon(pCommandBar);

	pDC->FillSolidRect(rcRibbon, m_clrRibbonFace);

	XTPRibbonTabContextColor tabColor = pGroup->GetParentTab()->GetContextColor();

	if (tabColor != xtpRibbonTabContextColorNone)
	{
		CXTPResourceImage* pImage = LoadImage(FormatName(_T("CONTEXTTAB%sGROUPPOPUPBACKGROUND"), GetColorName(tabColor)));

		if (!pImage)
		{
			pImage = LoadImage(FormatName(_T("CONTEXTTAB%sCLIENT"), GetColorName(tabColor)));
			rcRibbon.InflateRect(3, 3);
		}

		if (pImage) pImage->DrawImage(pDC, rcRibbon, pImage->GetSource(0, 1), pImage->GetWidth() < 8 ? CRect(2, 8, 2, 8) : CRect(8, 8, 8, 8));
	}
	else
	{
		CXTPResourceImage* pImage = LoadImage(FormatName(_T("RIBBONGROUPPOPUPBACKGROUND")));

		if (!pImage)
		{
			pImage = LoadImage(FormatName(_T("RIBBONGROUPS")));
			rcRibbon.InflateRect(3, 3);
		}

		if (pImage) pImage->DrawImage(pDC, rcRibbon, pImage->GetSource(0, 1), pImage->GetWidth() < 8 ? CRect(2, 8, 2, 8) :  CRect(8, 8, 8, 8));
	}
}

void CXTPRibbonPaintManager::DrawGroup(CDC* pDC, CXTPRibbonGroup* pGroup)
{
	CRect rcGroup(pGroup->GetRect());

	CRect rcGroupClient(rcGroup.left, rcGroup.top, rcGroup.right, rcGroup.bottom - m_nGroupCaptionHeight);
	CRect rcGroupCaption(rcGroup.left, rcGroupClient.bottom, rcGroup.right, rcGroup.bottom);


	if (!pGroup->IsReduced())
	{
		CXTPResourceImage* pImage = 0, *pImageCaption = 0;

		if (pGroup->GetParentTab()->GetContextColor() == xtpRibbonTabContextColorNone)
		{
			pImage = LoadImage(FormatName(_T("RIBBONGROUPCLIENT")));
			pImageCaption = LoadImage(FormatName(_T("RIBBONGROUPCAPTION")));
		}
		else
		{
			XTPRibbonTabContextColor tabColor = pGroup->GetParentTab()->GetContextColor();

			pImage = LoadImage(FormatName(_T("CONTEXTTAB%sGROUPCLIENT"), GetColorName(tabColor)));
			if (!pImage)
			{
				pImage = LoadImage(FormatName(_T("CONTEXTTABGROUPCLIENT")));
				pImageCaption = LoadImage(FormatName(_T("CONTEXTTABGROUPCAPTION")));
			}
		}
		if (!pImageCaption)
		{
			rcGroupClient = rcGroup;
		}

		int nState = pGroup->IsHighlighted() ? 1 : 0;

		if (pImage) pImage->DrawImage(pDC, rcGroupClient, pImage->GetSource(nState, 2), CRect(5, 5, 5, 5));
		if (pImageCaption) pImageCaption->DrawImage(pDC, rcGroupCaption,  pImageCaption->GetSource(nState, 2), CRect(5, 5, 5, 5));

		pImage = LoadImage(FormatName(_T("RIBBONGROUPSEPARATOR")));
		if (pImage)
		{
			pImage->DrawImage(pDC, CRect(rcGroup.right - 1, rcGroup.top + 2, rcGroup.right + 2, rcGroup.bottom - 5), pImage->GetSource());
		}


		CString strCaption = pGroup->GetCaption();
		XTPDrawHelpers()->StripMnemonics(strCaption);

		CXTPFontDC font(pDC, &m_fontGroupCaption);

		CRect rcGroupCaptionText(rcGroupCaption);
		rcGroupCaptionText.OffsetRect(0, -1);
		if (pGroup->IsOptionButtonVisible())
			rcGroupCaptionText.right -= m_nGroupCaptionHeight + 1;
		rcGroupCaptionText.DeflateRect(2, 2);

		if (!rcGroupCaptionText.IsRectEmpty())
		{
			pDC->SetTextColor(m_clrRibbonGroupCaptionText);
			pDC->DrawText(strCaption, rcGroupCaptionText, DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX | DT_END_ELLIPSIS | DT_CENTER);
		}
	}
}

void CXTPRibbonPaintManager::FillTabPopupToolBar(CDC* pDC, CXTPPopupToolBar* pPopupToolBar)
{
	CXTPClientRect rcRibbonClient(pPopupToolBar);
	pDC->FillSolidRect(rcRibbonClient, m_clrRibbonFace);
}

void CXTPRibbonPaintManager::FillRibbonBar(CDC* pDC, CXTPRibbonBar* pRibbonBar)
{

	CXTPClientRect rcRibbonClient(pRibbonBar);
	if (pRibbonBar->IsFrameThemeEnabled())
		rcRibbonClient.top += m_nRibbonCaptionHeight;

	CRect rcRibbonTabs(pRibbonBar->GetTabControlRect());
	rcRibbonTabs.left = rcRibbonClient.left; rcRibbonTabs.right = rcRibbonClient.right;

	CRect rcRibbonGroups(rcRibbonClient.left, rcRibbonTabs.bottom, rcRibbonClient.right, rcRibbonClient.bottom);

	pDC->FillSolidRect(rcRibbonTabs, m_clrRibbonFace);
	pDC->FillSolidRect(rcRibbonGroups, m_clrRibbonFace);

	if (m_bFlatFrame)
	{
		if (!pRibbonBar->IsDwmEnabled())
		{
			CXTPCommandBarsFrameHook* pFrameHook = pRibbonBar->GetFrameHook();
			if (pFrameHook && !pFrameHook->IsFrameActive() && pRibbonBar->IsFrameThemeEnabled())
			{
				pDC->FillSolidRect(rcRibbonTabs, m_clrRibbonInactiveFace);
			}
		}
		else
		{
			pDC->FillSolidRect(rcRibbonTabs, 0);

			CXTPResourceImage* pImage = LoadImage(FormatName(_T("RIBBONTABBACKGROUNDDWM")));
			if (pImage)
			{
				CRect rcSrc(pImage->GetSource(0, 1));
				CXTPClientRect rcRibbonClient(pRibbonBar);
				CRect rcRibbonTabs(pRibbonBar->GetTabControlRect());

				int nHeight = min(rcRibbonTabs.Height(), rcSrc.Height());
				rcSrc.top = rcSrc.bottom - nHeight;
				CRect rcArea(rcRibbonClient.left, rcRibbonTabs.bottom - nHeight, rcRibbonClient.right, rcRibbonTabs.bottom);

				pImage->DrawImage(pDC, rcArea, rcSrc, CRect(10, 0, 10, 0));
			}
		}
	}

	if (pRibbonBar->IsQuickAccessBelowRibbon())
	{
		CRect rcQuickAccess = pRibbonBar->GetQuickAccessRect();
		CXTPResourceImage* pImage = LoadImage(FormatName(_T("RIBBONQUICKACCESSFRAME")));
		if (pImage) pImage->DrawImage(pDC, rcQuickAccess, pImage->GetSource(0, 1), CRect(3, 3, 3, 3));
	}

	if (!pRibbonBar->IsGroupsVisible() && !pRibbonBar->IsQuickAccessBelowRibbon())
	{
		m_pPaintManger->HorizontalLine(pDC, rcRibbonClient.left, rcRibbonClient.bottom - 2, rcRibbonClient.right, m_clrMinimizedFrameEdgeShadow);
		m_pPaintManger->HorizontalLine(pDC, rcRibbonClient.left, rcRibbonClient.bottom - 1, rcRibbonClient.right, m_clrMinimizedFrameEdgeHighLight);
	}
}

void CXTPRibbonPaintManager::FillGroupRect(CDC* pDC, CXTPRibbonTab* pActiveTab, CRect rcGroups)
{
	XTPRibbonTabContextColor tabColor = pActiveTab ? pActiveTab->GetContextColor() : xtpRibbonTabContextColorNone;

	if (tabColor != xtpRibbonTabContextColorNone)
	{
		CXTPResourceImage* pImage = LoadImage(FormatName(_T("CONTEXTTAB%sCLIENT"), GetColorName(tabColor)));
		if (pImage) pImage->DrawImage(pDC, rcGroups, pImage->GetSource(0, 1), pImage->GetWidth() < 5 ? CRect(1, 8, 1, 8) : CRect(8, 8, 8, 8));
	}
	else
	{
		CXTPResourceImage* pImage = LoadImage(FormatName(_T("RIBBONGROUPS")));
		if (pImage) pImage->DrawImage(pDC, rcGroups, pImage->GetSource(0, 1), CRect(8, 8, 8, 8));
	}
}

CSize CXTPRibbonPaintManager::DrawControlGroupOption(CDC* pDC, CXTPControl* pControlGroupOption, BOOL bDraw)
{
	if (bDraw)
	{
		CRect rc = pControlGroupOption->GetRect();

		CXTPResourceImage* pImage = LoadImage(FormatName(_T("RIBBONOPTIONBUTTON")));

		int nState = 0;
		if (pControlGroupOption->GetSelected()) nState = 1;
		if (pControlGroupOption->GetPressed()) nState = 2;

		if (pImage) pImage->DrawImage(pDC, rc, pImage->GetSource(nState, 4), CRect(3, 2, 2, 2), 0xFF00FF);
	}

	return CSize(15, 14);
}

CSize CXTPRibbonPaintManager::DrawControlGroupPopup(CDC* pDC, CXTPControlPopup* pControlGroupPopup, BOOL bDraw)
{
	CXTPRibbonGroup* pRibbonGroup = pControlGroupPopup->GetRibbonGroup();
	CXTPResourceImage* pImage = NULL;

	int nOffset = 0;
	if (pControlGroupPopup->GetSelected()) nOffset = 1;
	if (pControlGroupPopup->GetPopuped()) nOffset = 2;

	if (!pRibbonGroup)
	{
		if (bDraw)
		{
			pImage = LoadImage(FormatName(_T("RIBBONGROUPQUICKACCESS")));
			ASSERT(pImage);

			if (pImage) pImage->DrawImage(pDC, pControlGroupPopup->GetRect(),
				pImage->GetSource(nOffset, 3), CRect(2, 2, 2, 2), 0xFF00FF, GetDrawImageFlags(pControlGroupPopup));
		}
		return m_pPaintManger->DrawControlToolBarParent(pDC, pControlGroupPopup, bDraw);
	}

	if (bDraw)
	{
		CRect rc = pControlGroupPopup->GetRect();

		XTPRibbonTabContextColor tabColor = pRibbonGroup ? pRibbonGroup->GetParentTab()->GetContextColor() : xtpRibbonTabContextColorNone;

		if (tabColor != xtpRibbonTabContextColorNone)
		{
			pImage = LoadImage(FormatName(_T("CONTEXTTAB%sGROUPBUTTON"), GetColorName(tabColor)));
		}
		else
		{
			pImage = LoadImage(FormatName(_T("RIBBONGROUPBUTTON")));
		}

		if (pImage) pImage->DrawImage(pDC, rc, pImage->GetSource(nOffset, 3), CRect(5, 5, 5, 5));
	}

	CSize szButton(max(42, m_pPaintManger->m_nTextHeight * 2 - 3), GetEditHeight() * 3);

	CXTPFontDC font(pDC, m_pPaintManger->GetControlFont(pControlGroupPopup));

	int nSplitDropDownHeight = m_pPaintManger->GetSplitDropDownHeight();

	if (bDraw)
	{
		CXTPImageManagerIcon* pImageIcon = pControlGroupPopup->GetImage(0);
		BOOL bDrawImage = (pImageIcon != NULL);

		CRect rcButton = pControlGroupPopup->GetRect();
		if (bDrawImage)
		{
			CSize sz(pImageIcon->GetWidth(), pImageIcon->GetHeight());

			CPoint pt = CPoint(rcButton.CenterPoint().x - sz.cx / 2, rcButton.top + 4);

			if (sz.cx < 20)
			{
				CXTPResourceImage* pImageButton = LoadImage(FormatName(_T("RIBBONGROUPBUTTONICON")));
				CRect rc(CPoint(rcButton.CenterPoint().x - pImageButton->GetWidth() / 2, rcButton.top + 2),
					CSize(pImageButton->GetWidth(), pImageButton->GetHeight()));

				if (pImageButton) pImageButton->DrawImage(pDC, rc, pImageButton->GetSource(0, 1), CRect(0, 0, 0, 0));

				pt.y = rcButton.top + 9;
			}

			m_pPaintManger->DrawImage(pDC, pt, sz, pImageIcon,
				pControlGroupPopup->GetSelected(), pControlGroupPopup->GetPressed(),
				pControlGroupPopup->GetEnabled(), FALSE, pControlGroupPopup->GetPopuped());
		}

		CRect rcText = rcButton;
		rcText.bottom -= m_nGroupCaptionHeight + 2;
		rcText.top = rcText.bottom - nSplitDropDownHeight;

		pDC->SetTextColor(m_pPaintManger->GetControlTextColor(pControlGroupPopup));
		m_pPaintManger->DrawControlText2(pDC, pControlGroupPopup, rcText, TRUE, FALSE, TRUE);
	}
	else
	{
		CSize szText = m_pPaintManger->DrawControlText2(pDC, pControlGroupPopup, CXTPEmptyRect(), FALSE, FALSE, TRUE);

		szButton.cx = max(szButton.cx, szText.cx + 5);
	}
	return szButton;
}

void CXTPRibbonPaintManager::DrawRibbonFrameContextHeaders(CDC* pDC, CXTPRibbonBar* pRibbonBar, CXTPRibbonTabContextHeaders* pContextHeaders)
{
	int nCount = pContextHeaders->GetCount();
	if (nCount == 0)
		return;

	CXTPFontDC font(pDC, m_pTabPaintManager->GetFont());
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(m_pTabPaintManager->GetColorSet()->m_clrNormalText);

	for (int i = 0; i < nCount; i++)
	{
		CXTPRibbonTabContextHeader* pHeader = pContextHeaders->GetHeader(i);

		CXTPResourceImage* pImage = LoadImage(FormatName(_T("CONTEXTTAB%sHEADER"), GetColorName(pHeader->m_color)));
		if (!pImage)
			return;

		CRect rc(pHeader->m_rcRect);
		pImage->DrawImage(pDC, rc, pImage->GetSource(), CRect(2, 4, 2, 2));

		rc.left += 6;
		int nFrameBorder = pRibbonBar->IsFrameThemeEnabled() ?
			pRibbonBar->GetFrameHook()->GetFrameBorder() : 0;

		rc.top += nFrameBorder - 2;

		if (pRibbonBar->IsDwmEnabled())
		{
			DrawDwmCaptionText(pDC, rc, pHeader->m_strCaption, pRibbonBar->GetSite(), pRibbonBar->GetFrameHook()->IsFrameActive());
		}
		else
		{
			pDC->DrawText(pHeader->m_strCaption, rc, DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX | DT_END_ELLIPSIS | DT_LEFT);
		}
	}

}

#define XTP_DTT_GLOWSIZE        (1UL << 11)     // iGlowSize has been specified
#define XTP_DTT_COMPOSITED      (1UL << 13)     // Draws text with antialiased alpha (needs a DIB section)

void CXTPRibbonPaintManager::DrawDwmCaptionText(CDC* pDC, CRect rcCaptionText, CString strWindowText, CWnd* pSite, BOOL bActive)
{
	if ((pSite->GetStyle() & WS_MAXIMIZE) && !XTPSystemVersion()->IsWin7OrGreater())
	{
		pDC->SetTextColor(0xFFFFFF);
		pDC->DrawText(strWindowText, rcCaptionText,
			DT_VCENTER | DT_LEFT| DT_END_ELLIPSIS | DT_SINGLELINE | DT_NOPREFIX);
		return;
	}

	CXTPWinThemeWrapper wrapper(FALSE);
	wrapper.OpenTheme(0, L"WINDOW");

	if (!wrapper.IsAppThemed())
	{
		pDC->SetTextColor(0);
		pDC->DrawText(strWindowText, rcCaptionText, DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX | DT_END_ELLIPSIS | DT_LEFT);
		return;
	}

	CRect rcBuffer(rcCaptionText);
	rcBuffer.InflateRect(5, 0);

	HDC hDC = 0;
	HPAINTBUFFER pb = wrapper.BeginBufferedPaint(pDC->GetSafeHdc(), rcBuffer, XTP_BPBF_TOPDOWNDIB, 0, &hDC);

	if (hDC != NULL)
	{
		::BitBlt(hDC, rcBuffer.left, rcBuffer.top, rcBuffer.Width(), rcBuffer.Height(), pDC->GetSafeHdc(), rcBuffer.left, rcBuffer.top, SRCCOPY);
		::SetTextColor(hDC, 0);
		XTP_UX_DTTOPTS op;
		op.dwSize = sizeof(op);
		op.dwFlags = XTP_DTT_COMPOSITED | XTP_DTT_GLOWSIZE;
		op. iGlowSize = 8;
		CXTPFontDC font(CDC::FromHandle(hDC), &m_pPaintManger->GetFramePaintManager()->m_fontFrameCaption);

		wrapper.DrawThemeTextEx(hDC, pSite->GetStyle() & WS_MAXIMIZE ? WP_MAXCAPTION : WP_CAPTION, bActive ? FS_ACTIVE : FS_INACTIVE, XTP_CT2CW(strWindowText), -1,
			DT_VCENTER | DT_LEFT| DT_END_ELLIPSIS | DT_SINGLELINE | DT_NOPREFIX,  rcCaptionText, &op);
	}

	wrapper.EndBufferedPaint(pb, TRUE);
}

void CXTPRibbonPaintManager::DrawRibbonFrameCaptionBar(CDC* pDC, CXTPRibbonBar* pRibbonBar)
{
	CRect rcCaption = pRibbonBar->GetCaptionRect();
	rcCaption.top--;

	CXTPResourceImage* pImage = LoadImage(FormatName(_T("FRAMETOPCENTER")));
	pImage->DrawImage(pDC, rcCaption, pImage->GetSource(0, 2), CRect(0, 5, 0, 3));

	if (!pRibbonBar->IsQuickAccessBelowRibbon() && pRibbonBar->IsQuickAccessVisible() &&
		((CXTPControls*)pRibbonBar->GetQuickAccessControls())->GetVisibleCount() > 0)
	{
		if (pRibbonBar->GetSystemButton() && !pRibbonBar->GetSystemButton()->IsSimpleButton())
		{
			CRect rcQuickAccess = pRibbonBar->GetQuickAccessRect();
			int nFrameBorder = 0;
			CRect rcQuickAccessArea(rcQuickAccess.left - 15, nFrameBorder, rcQuickAccess.right, pRibbonBar->GetQuickAccessHeight() + nFrameBorder);

			BOOL bDwm = FALSE;
			pImage = LoadImage(FormatName(bDwm ? _T("RIBBONQUICKACCESSAREADWM") : _T("RIBBONQUICKACCESSAREA")));
			if (pImage)
			{
				pImage->PreserveOrientation();
				pImage->DrawImage(pDC, rcQuickAccessArea, pImage->GetSource(), CRect(16, 3, 16, 3));
			}
		}
	}

	CXTPRibbonTabContextHeaders* pContextHeaders = pRibbonBar->GetContextHeaders();
	if (pContextHeaders->GetCount() != 0)
	{
		DrawRibbonFrameContextHeaders(pDC, pRibbonBar, pContextHeaders);
	}
}

void CXTPRibbonPaintManager::DrawRibbonFramePart(CDC* pDC, CXTPRibbonBar* pRibbonBar)
{
	if (!pRibbonBar->IsCaptionVisible())
		return;

	if (m_bFlatFrame)
	{
		CWnd* pSite = pRibbonBar->GetSite();
		CXTPCommandBarsFrameHook* pFrameHook = pRibbonBar->GetFrameHook();
		if (!pFrameHook)
			return;

		CXTPFramePaintManager* pFramePaintManager = pFrameHook->GetFramePaintManager();

		BOOL bActive = pFrameHook->IsFrameActive();



		CXTPClientRect rcBorders(pSite);
		pSite->ClientToScreen(rcBorders);

		CXTPWindowRect rcWindow(pSite);

		CRect rcTabControl = pRibbonBar->GetTabControlRect();
		pRibbonBar->ClientToScreen(rcTabControl);
		rcTabControl.OffsetRect(-rcWindow.TopLeft());

		int nRightBorder = rcBorders.left - rcWindow.left, nLeftBorder = rcBorders.left - rcWindow.left;

		pDC->FillSolidRect(nLeftBorder - 1, rcTabControl.top, 1, rcTabControl.Height() - 1,
			bActive ? pFramePaintManager->m_clrFrameBorderActive2 : pFramePaintManager->m_clrFrameBorderInactive2);

		pDC->FillSolidRect(rcWindow.Width() - nRightBorder, rcTabControl.top, 1, rcTabControl.Height() - 1,
			bActive ? pFramePaintManager->m_clrFrameBorderActive2 : pFramePaintManager->m_clrFrameBorderInactive2);
	}
}

void CXTPRibbonPaintManager::DrawRibbonApplicationIcon(CDC* pDC, CXTPRibbonBar* pRibbonBar, CRect rc, HICON hIcon)
{
	if (pRibbonBar->IsDwmEnabled())
	{
		if (m_hApplicationIcon != hIcon)
		{
			m_hApplicationIcon = hIcon;
			m_bAlphaApplicationIcon = CXTPImageManagerIcon::IsAlphaIconHandle(hIcon);
		}

		if (m_bAlphaApplicationIcon)
		{
			DrawIconEx(pDC->GetSafeHdc(), rc.left, rc.top, hIcon, rc.Width(), rc.Height(), 0, NULL, DI_NORMAL);
		}
		else
		{
			CXTPImageManagerIcon::DrawIconComposited(pDC, rc.TopLeft(), rc.Size(), hIcon);
		}
	}
	else
	{
		DrawIconEx(pDC->GetSafeHdc(), rc.left, rc.top, hIcon, rc.Width(), rc.Height(), 0, NULL, DI_NORMAL);
	}

};

void CXTPRibbonPaintManager::DrawRibbonFrameCaption(CDC* pDC, CXTPRibbonBar* pRibbonBar, BOOL bActive)
{
	CFrameWnd* pSite = (CFrameWnd*)pRibbonBar->GetSite();
	CRect rcCaption = pRibbonBar->GetCaptionRect();
	CXTPResourceImage* pImage;

	if (!pRibbonBar->IsDwmEnabled())
	{

		pImage = LoadImage(FormatName(_T("FRAMETOPLEFT")));

		ASSERT(pImage);
		if (!pImage)
			return;

		CRect rcSrc(pImage->GetSource(bActive ? 0 : 1, 2));

		CRect rcTopLeft(rcCaption.left, rcCaption.top, rcCaption.left + rcSrc.Width(), rcCaption.bottom);
		pImage->DrawImage(pDC, rcTopLeft, rcSrc, CRect(0, 5, 0, 3), 0xFF00FF);

		//////////////////////////////////////////////////////////////////////////

		pImage = LoadImage(FormatName(_T("FRAMETOPRIGHT")));

		rcSrc = pImage->GetSource(bActive ? 0 : 1, 2);
		CRect rcTopRight(rcCaption.right - rcSrc.Width(), rcCaption.top, rcCaption.right, rcCaption.bottom);
		pImage->DrawImage(pDC, rcTopRight, rcSrc, CRect(0, 5, 0, 3));


		//////////////////////////////////////////////////////////////////////////

		pImage = LoadImage(FormatName(_T("FRAMETOPCENTER")));

		CRect rcTopCenter(rcTopLeft.right, rcCaption.top, rcTopRight.left, rcCaption.bottom);
		pImage->DrawImage(pDC, rcTopCenter, pImage->GetSource(bActive ? 0 : 1, 2), CRect(0, 5, 0, 3));
	}
	else
	{
		pDC->FillSolidRect(rcCaption, 0);

		if (m_bFlatFrame)
		{
			CXTPResourceImage* pImage = LoadImage(FormatName(_T("RIBBONTABBACKGROUNDDWM")));
			if (pImage)
			{
				CRect rcSrc(pImage->GetSource(0, 1));
				CXTPClientRect rcRibbonClient(pRibbonBar);
				CRect rcRibbonTabs(pRibbonBar->GetTabControlRect());
				rcSrc.bottom = rcSrc.bottom - rcRibbonTabs.Height();
				if (rcSrc.Height() > 0)
				{
					CRect rcArea(rcRibbonClient.left, rcRibbonTabs.top - rcSrc.Height(), rcRibbonClient.right, rcRibbonTabs.top);
					pImage->DrawImage(pDC, rcArea, rcSrc, CRect(10, 0, 10, 0));
				}
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////

	if (!pRibbonBar->IsQuickAccessBelowRibbon() && pRibbonBar->IsQuickAccessVisible() &&
		((CXTPControls*)pRibbonBar->GetQuickAccessControls())->GetVisibleCount() > 0)
	{
		if (pRibbonBar->GetSystemButton() && !pRibbonBar->GetSystemButton()->IsSimpleButton())
		{
			CRect rcQuickAccess = pRibbonBar->GetQuickAccessRect();
			CRect rcQuickAccessArea(rcQuickAccess.left - 15, rcQuickAccess.top + 2, rcQuickAccess.right, rcQuickAccess.top + 2 + pRibbonBar->GetQuickAccessHeight());

			BOOL bDwm = IsCompositeRect(pRibbonBar, rcQuickAccess);
			pImage = LoadImage(FormatName(bDwm ? _T("RIBBONQUICKACCESSAREADWM") : _T("RIBBONQUICKACCESSAREA")));
			if (pImage)
			{
				pImage->PreserveOrientation();
				pImage->DrawImage(pDC, rcQuickAccessArea, pImage->GetSource(), CRect(16, 3, 16, 3));
			}
		}
	}


	//////////////////////////////////////////////////////////////////////////


	if ((pRibbonBar->GetSystemButton() == NULL || pRibbonBar->GetSystemButton()->IsSimpleButton()) && pRibbonBar->GetFrameHook())
	{
		HICON hIcon = m_pPaintManger->GetFramePaintManager()->GetFrameSmallIcon(pSite);
		if (hIcon)
		{
			int nFrameBorder = pRibbonBar->GetFrameHook()->GetFrameBorder();
			int nTopBorder = pRibbonBar->GetRibbonTopBorder();

			CSize szIcon(GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON));

			int nTop = nTopBorder / 2 - 1 + (rcCaption.bottom  - szIcon.cy)/2;
			int nLeft = rcCaption.left + nFrameBorder + 1;

			DrawRibbonApplicationIcon(pDC, pRibbonBar, CRect(nLeft, nTop, nLeft + szIcon.cx, nTop + szIcon.cy), hIcon);

			if (pRibbonBar->IsQuickAccessVisible())
			{
				pImage = LoadImage(FormatName(_T("SYSTEMICONSEPARATOR")));
				if (pImage)
				{
					CRect rcSrc(pImage->GetSource());
					CRect rcDest(CPoint(nLeft + szIcon.cy + 6, nTop + szIcon.cy / 2 - rcSrc.Height() / 2), rcSrc.Size());
					pImage->DrawImage(pDC, rcDest, rcSrc);
				}
			}

		}
	}


	CRect rcCaptionText = pRibbonBar->GetCaptionTextRect();

	CXTPRibbonTabContextHeaders* pContextHeaders = pRibbonBar->GetContextHeaders();
	if (pContextHeaders->GetCount() != 0)
	{
		DrawRibbonFrameContextHeaders(pDC, pRibbonBar, pContextHeaders);
	}

	CXTPFontDC font(pDC, &m_pPaintManger->GetFramePaintManager()->m_fontFrameCaption);

	CString strWindowText;
	pSite->GetWindowText(strWindowText);
	rcCaptionText.left += max(0, (rcCaptionText.Width() - pDC->GetTextExtent(strWindowText).cx) / 2);

	if (pSite->GetStyle() & WS_MAXIMIZE)
	{
		rcCaptionText.top += pRibbonBar->GetFrameHook()->GetFrameBorder() / 2;
	}

	if (!pRibbonBar->IsDwmEnabled())
	{
		m_pPaintManger->GetFramePaintManager()->DrawCaptionText(pDC, rcCaptionText, pSite, bActive);
	}
	else
	{
		DrawDwmCaptionText(pDC, rcCaptionText, strWindowText, pSite, bActive);
	}
}

void CXTPRibbonPaintManager::DrawRibbonQuickAccessButton(CDC* pDC, CXTPControlPopup* pControl)
{
	CXTPResourceImage* pImage = LoadImage(FormatName(_T("RIBBONQUICKACCESSBUTTON")));

	ASSERT(pImage);
	if (!pImage)
		return;

	pImage->DrawImage(pDC, pControl->GetRect(), pImage->GetSource(!pControl->GetEnabled() ? 4 : pControl->GetPopuped() ? 2 : pControl->GetSelected() ? 1 : 0, 5),
		CRect(2, 2, 2, 2), 0xFF00FF, GetDrawImageFlags(pControl));
}

void CXTPRibbonPaintManager::DrawRibbonQuickAccessMoreButton(CDC* pDC, CXTPControlPopup* pControl)
{
	CXTPResourceImage* pImage = LoadImage(FormatName(_T("RIBBONQUICKACCESSMORE")));

	ASSERT(pImage);
	if (!pImage)
		return;

	pImage->DrawImage(pDC, pControl->GetRect(), pImage->GetSource(pControl->GetPopuped() ? 2 : pControl->GetSelected() ? 1 : 0, 4),
		CRect(2, 2, 2, 2), 0xFF00FF, GetDrawImageFlags(pControl));

}

void CXTPRibbonPaintManager::DrawRibbonFrameSystemButton(CDC* pDC, CXTPRibbonControlSystemButton* pControl, CRect rc)
{
	CXTPRibbonBar* pRibbonBar = (CXTPRibbonBar*)pControl->GetParent();

	if (!pRibbonBar->IsCaptionVisible() || !pRibbonBar->IsTabsVisible() || pControl->IsSimpleButton())
	{

		CXTPResourceImage* pImage = LoadImage(FormatName(_T("FILEBUTTON")));

		if (pImage)
		{

			if (m_nFileButtonImageCount == 7)
			{
				int nState = !pControl->GetEnabled() ? 5 :
					pControl->GetPopuped() ? (pControl->GetSelected() ? 3 : 2) :
					IsKeyboardSelected(pControl->GetSelected()) ? 4 :
					pControl->GetSelected()? 1 : 0;

				CRect rcSrc(pImage->GetSource(nState, 7));

				CRect rcDest(rc);

				if (pControl->GetPopuped())
				{
					rcDest.bottom++;

				}
				else
				{
					rcSrc.bottom--;
				}

				pImage->DrawImage(pDC, rcDest, rcSrc, CRect(3, 3, 3, 3));

			}
			else
			{
				int nState = pControl->GetPopuped() ? 2 : pControl->GetSelected() ? 1 :  0;

				pImage->DrawImage(pDC, rc, pImage->GetSource(nState, 3), CRect(3, 3, 3, 3));
			}

			if (pControl->GetStyle() == xtpButtonIcon)
			{
				CXTPImageManagerIcon* pImage = pControl->GetImage();
				if (pImage)
				{
					CSize sz(pImage->GetExtent());

					if (sz.cy > 16)
						sz = CSize(16, 16);

					CPoint pt((rc.left + rc.right - sz.cx) / 2, (rc.top + rc.bottom - sz.cy) / 2);
					pImage->Draw(pDC, pt, sz);
				}
			}
			else
			{
				pDC->SetTextColor(m_clrFileButtonText);

				BOOL bComposite =  IsCompositeRect(pRibbonBar, rc);

				CString strCaption(pControl->GetCaption());
				CXTPPaintManager::StripMnemonics(strCaption);

				CXTPPaintManager::DrawTextApi(pDC, strCaption, rc, DT_NOPREFIX | DT_VCENTER | DT_CENTER | DT_SINGLELINE, bComposite);
			}
		}
		else
		{
			m_pPaintManger->DrawControlToolBarParent(pDC, pControl, TRUE);
		}
		return;
	}

	XTPImageState nState = xtpImageNormal;
	if (pControl->GetPopuped())
		nState = xtpImagePressed;
	else if (pControl->GetSelected())
		nState = xtpImageHot;
	else if (!pControl->GetEnabled())
		nState = xtpImageDisabled;

	CXTPImageManagerIcon* pIcon = pControl->GetImage(32);

	if (pIcon && pIcon->GetWidth() > 32)
	{
		CPoint ptIcon((rc.left + rc.right - pIcon->GetWidth()) / 2, (rc.top + rc.bottom - pIcon->GetHeight()) / 2);

		pIcon->m_bDrawComposited = pControl->HasDwmCompositedRect();
		pIcon->Draw(pDC, ptIcon, pIcon->GetIcon(nState));
		pIcon->m_bDrawComposited = FALSE;
		return;
	}

	CXTPResourceImage* pImage = NULL;

	if (rc.Width() > 54 && rc.Height() > 54)
		pImage = LoadImage(FormatName(_T("FRAMESYSTEMBUTTON52")));

	if (pImage == NULL)
		pImage = LoadImage(FormatName(_T("FRAMESYSTEMBUTTON")));

	ASSERT(pImage);
	if (!pImage)
		return;

	int nOffset = 0;
	if (rc.top < 0)
	{
		nOffset = rc.top;
		rc.OffsetRect(0, -nOffset);
	}

	CRect rcSrc(pImage->GetSource(nState == xtpImageDisabled || nState == xtpImageNormal ? 0 : nState == xtpImageHot ? 1 : 2, 3));
	CSize szDest(rcSrc.Size());
	CPoint ptDest((rc.left + rc.right - szDest.cx) / 2, nOffset + (rc.top + rc.bottom - szDest.cy) / 2);

	pImage->DrawImage(pDC, CRect(ptDest, szDest), rcSrc, CRect(0, 0, 0, 0));

	if (pIcon)
	{
		CPoint ptIcon((rc.left + rc.right - pIcon->GetWidth()) / 2, nOffset + (rc.top + rc.bottom + 1 - pIcon->GetHeight()) / 2);
		pIcon->m_bDrawComposited = pControl->HasDwmCompositedRect();
		pIcon->Draw(pDC, ptIcon, pIcon->GetIcon(nState));
		pIcon->m_bDrawComposited = FALSE;
	}
	else
	{
		CWnd* pSite = pControl->GetParent()->GetSite();
		HICON hIcon = m_pPaintManger->GetFramePaintManager()->GetFrameLargeIcon(pSite);
		if (hIcon)
		{
			CPoint ptIcon((rc.left + rc.right - 33) / 2, nOffset + (rc.top + rc.bottom - 31) / 2);

				DrawIconEx(pDC->GetSafeHdc(), ptIcon.x, ptIcon.y, hIcon, 32, 32, NULL, NULL, DI_NORMAL);
		}
	}
}

void CXTPRibbonPaintManager::DrawRibbonScrollButton(CDC* pDC, CXTPControl* pControl, BOOL bScrollLeft)
{
	CXTPResourceImage* pImage = LoadImage(FormatName(bScrollLeft ? _T("RIBBONGROUPSSCROLLLEFT") : _T("RIBBONGROUPSSCROLLRIGHT")));

	ASSERT(pImage);
	if (!pImage)
		return;

	CRect rc(pControl->GetRect());

	int nState = pControl->GetPressed() ? 2 : pControl->GetSelected() ? 1 : 0;

	CXTPResourceImage* pImageGlyph = LoadImage(FormatName(bScrollLeft ? _T("RIBBONGROUPSSCROLLLEFTGLYPH") : _T("RIBBONGROUPSSCROLLRIGHTGLYPH")));

	pImage->PreserveOrientation();
	pImage->DrawImage(pDC, rc, pImage->GetSource(nState, 3), CRect(4, 6, 3, 6), 0xFF00FF);

	CSize szGlyph = pImageGlyph->GetSource(0, 3).Size();
	CRect rcGlyph(CPoint((rc.right + rc.left - szGlyph.cx) / 2, (rc.top + rc.bottom - szGlyph.cy) / 2), szGlyph);

	pImageGlyph->DrawImage(pDC, rcGlyph, pImageGlyph->GetSource(nState, 3), CRect(0, 0, 0, 0), 0xFF00FF);

}

void CXTPRibbonPaintManager::FillMorePopupToolBarEntry(CDC* pDC, CXTPCommandBar* pBar)
{
	CRect rc;
	pBar->GetClientRect(&rc);

	CXTPResourceImage* pImage = LoadImage(FormatName(_T("MOREPOPUPBARFRAME")));
	if (!pImage)
		pImage = LoadImage(FormatName(_T("POPUPBARFRAME")));

	ASSERT(pImage);
	if (!pImage)
		return;

	pImage->DrawImage(pDC, rc, pImage->GetSource(), CRect(4, 4, 4, 4));
}

void CXTPRibbonPaintManager::FillSystemPopupBarEntry(CDC* pDC, CXTPPopupBar* pBar)
{
	CXTPClientRect rc(pBar);

	CXTPResourceImage* pImage = LoadImage(_T("RIBBONSYSTEMMENU"));
	if (!pImage)
		return;

	pImage->DrawImage(pDC, rc, pImage->GetSource(), pBar->GetBorders(), COLORREF_NULL);

	CXTPRibbonControlSystemButton* pControlPopup = DYNAMIC_DOWNCAST(CXTPRibbonControlSystemButton, pBar->GetControlPopup());

	if (!pControlPopup)
		return;

	CXTPRibbonBar* pRibbonBar = DYNAMIC_DOWNCAST(CXTPRibbonBar, pControlPopup->GetParent());
	if (!pRibbonBar)
		return;


	CRect rcPopup = pControlPopup->GetRect();
	pControlPopup->GetParent()->ClientToScreen(rcPopup);
	pBar->ScreenToClient(rcPopup);

	if (pRibbonBar->IsCaptionVisible() && CRect().IntersectRect(rcPopup, rc))
	{
		DrawRibbonFrameSystemButton(pDC, pControlPopup, rcPopup);
	}

	for (int i = 0; i < pBar->GetControlCount(); i++)
	{
		CXTPControl* pControl = pBar->GetControl(i);
		if (pControl && pControl->GetWrap())
		{
			CRect rcRow = pControl->GetRowRect();
			pDC->FillSolidRect(rcRow, GetImages()->GetImageColor(_T("Ribbon"), _T("RecentFileListBackground")));
			break;
		}
	}
}

void CXTPRibbonPaintManager::DrawSystemPopupBarButton(CDC* pDC, CXTPControl* pButton)
{
	CXTPResourceImage* pImage = LoadImage(_T("RIBBONSYSTEMMENUBUTTON"));

	if (pImage)
	{
		pImage->DrawImage(pDC, pButton->GetRect(), pImage->GetSource(pButton->GetSelected() ? 1 : 0, 2), CRect(2, 2, 2, 2), 0xFF00FF);
	}

	m_pPaintManger->DrawControlToolBarParent(pDC, pButton, TRUE);
}

BOOL CXTPRibbonPaintManager::DrawRibbonGroupControlEntry(CDC* pDC, CXTPControl* pButton)
{
	if (pButton->GetRibbonGroup() && pButton->GetRibbonGroup()->IsControlsGrouping())
	{
		if (pButton->GetType() == xtpControlSplitButtonPopup)
			return FALSE;

		BOOL bLeft = pButton->GetBeginGroup() || pButton->GetWrap();

		int nIndexNext = pButton->GetControls()->GetNext(pButton->GetIndex());

		BOOL bRight = nIndexNext <= pButton->GetIndex() ||
			pButton->GetControls()->GetAt(nIndexNext)->GetBeginGroup() ||
			pButton->GetControls()->GetAt(nIndexNext)->GetWrap();

		CString strImage = FormatName(bLeft && bRight ? _T("TOOLBARBUTTONSSPECIALSINGLE") :
		bLeft ? _T("TOOLBARBUTTONSSPECIALLEFT") :
		bRight ? _T("TOOLBARBUTTONSSPECIALRIGHT") :
		_T("TOOLBARBUTTONSSPECIALCENTER"));

		CXTPResourceImage* pImage = LoadImage(strImage);
		ASSERT(pImage);
		if (!pImage)
			return FALSE;

		BOOL bPopuped = pButton->GetPopuped();
		BOOL bEnabled = pButton->GetEnabled();
		BOOL bChecked = pButton->GetChecked();
		BOOL bSelected = pButton->GetSelected();
		BOOL bPressed = pButton->GetPressed();
		CRect rc = pButton->GetRect();

		int nState = 0;

		if (!bEnabled)
		{
			if (IsKeyboardSelected(bSelected)) nState = 0 + 1;
		}
		else if (bPopuped) nState = 2 + 1;
		else if (bChecked && !bSelected && !bPressed) nState = 2 + 1;
		else if (bChecked && bSelected && !bPressed) nState = 3 + 1;
		else if (IsKeyboardSelected(bPressed) || (bSelected && bPressed)) nState = 1 + 1;
		else if (bSelected || bPressed) nState = 0 + 1;

		pImage->DrawImage(pDC, rc, pImage->GetSource(nState, 5), CRect(2, 2, 2, 2), 0xFF00FF);
		return TRUE;
	}

	return FALSE;
}


//////////////////////////////////////////////////////////////////////////
//


