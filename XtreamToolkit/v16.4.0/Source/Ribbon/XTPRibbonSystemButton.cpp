// XTPRibbonSystemButton.cpp : implementation file
//
// This file is a part of the XTREME RIBBON MFC class library.
// (c)1998-2013 Codejock Software, All Rights Reserved.
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

#include "StdAfx.h"

#include <Common/XTPIntel80Helpers.h>
#include <Common/XTPImageManager.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPHookManager.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPDrawHelpers.h>
#include <Common/ScrollBar/XTPScrollInfo.h>

#include <CommandBars/XTPCommandBarsDefines.h>
#include <CommandBars/XTPCommandBars.h>
#include <CommandBars/XTPCommandBar.h>
#include <CommandBars/XTPToolBar.h>
#include <CommandBars/XTPMenuBar.h>
#include <CommandBars/XTPPopupBar.h>
#include <CommandBars/XTPControls.h>
#include <CommandBars/XTPControl.h>
#include <CommandBars/XTPControlButton.h>
#include <CommandBars/XTPControlPopup.h>
#include <CommandBars/XTPPaintManager.h>
#include <CommandBars/XTPRecentFileList.h>

#include <Ribbon/Resource.h>
#include <Ribbon/XTPRibbonSystemButton.h>
#include <Ribbon/XTPRibbonPopups.h>
#include <Ribbon/XTPRibbonBar.h>
#include <Ribbon/XTPRibbonPaintManager.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_XTP_CONTROL(CXTPRibbonControlSystemButton, CXTPControlPopup)

CXTPRibbonControlSystemButton::CXTPRibbonControlSystemButton()
{
	SetFlags(xtpFlagNoMovable | xtpFlagManualUpdate);
	m_bShowShadow = FALSE;

	m_bCloseOnDblClick = TRUE;

	EnableAutomation();

}

BOOL CXTPRibbonControlSystemButton::IsSimpleButton() const
{
	return GetStyle() != xtpButtonAutomatic;
}

CString CXTPRibbonControlSystemButton::GetCaption() const
{
	CString strCaption = CXTPControlPopup::GetCaption();

	if (GetParent()->GetCommandBars()->IsAllCaps())
	{
		strCaption.MakeUpper();
	}

	return strCaption;
}

CSize CXTPRibbonControlSystemButton::GetSize(CDC* pDC)
{
	CSize sz = CXTPControlPopup::GetSize(pDC);

	if (IsSimpleButton())
	{
		sz.cx = max(sz.cx, 56);
		sz.cy = max(sz.cy, 23);
	}

	return sz;
}

void CXTPRibbonControlSystemButton::Draw(CDC* pDC)
{
	((CXTPRibbonBar*)GetParent())->GetRibbonPaintManager()->DrawRibbonFrameSystemButton(pDC, this, GetRect());
}

BOOL CXTPRibbonControlSystemButton::OnLButtonDblClk(CPoint /*point*/)
{
	GetParent()->GetCommandBars()->ClosePopups();

	CWnd* pSite = GetParent()->GetSite();

	if ((!IsSimpleButton() && (pSite->GetStyle() & WS_CHILD) == 0) && m_bCloseOnDblClick)
	{
		pSite->SendMessage(WM_SYSCOMMAND, SC_CLOSE | HTSYSMENU, 0);
	}
	else
	{
		OnExecute();
	}

	return TRUE;
}

void CXTPRibbonControlSystemButton::AdjustExcludeRect(CRect& rc, BOOL bVertical)
{
	CXTPRibbonBar* pRibbonBar = DYNAMIC_DOWNCAST(CXTPRibbonBar, GetParent());
	if (!pRibbonBar)
	{
		CXTPControlPopup::AdjustExcludeRect(rc, bVertical);
		return;
	}

	if (IsSimpleButton())
		return;

	if (pRibbonBar->IsCaptionVisible() && pRibbonBar->IsTabsVisible() &&
		DYNAMIC_DOWNCAST(CXTPRibbonSystemPopupBar, GetCommandBar()))
	{
		rc.bottom -= 18;
		return;
	}

	CXTPControlPopup::AdjustExcludeRect(rc, bVertical);
}

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_XTP_COMMANDBAR(CXTPRibbonSystemPopupBar, CXTPPopupBar)

BEGIN_MESSAGE_MAP(CXTPRibbonSystemPopupBar, CXTPPopupBar)
	ON_WM_NCHITTEST_EX()
END_MESSAGE_MAP()

CXTPRibbonSystemPopupBar::CXTPRibbonSystemPopupBar()
{
	m_rcBorders.SetRect(6, 18, 6, 29);
	SetShowGripper(FALSE);
}

CRect CXTPRibbonSystemPopupBar::GetBorders()
{
	return m_rcBorders;
}

void CXTPRibbonSystemPopupBar::FillCommandBarEntry(CDC* pDC)
{
	CXTPClientRect rc(this);
	CXTPRibbonPaintManager* pPaintManager = GetPaintManager()->GetRibbonPaintManager();

	pPaintManager->FillSystemPopupBarEntry(pDC, this);
}

LRESULT CXTPRibbonSystemPopupBar::OnNcHitTest(CPoint point)
{
	if (!DYNAMIC_DOWNCAST(CXTPRibbonControlSystemButton, m_pControlPopup))
		return CXTPPopupBar::OnNcHitTest(point);

	CRect rcPopup = m_pControlPopup->GetRect();
	m_pControlPopup->GetParent()->ClientToScreen(rcPopup);
	if (rcPopup.PtInRect(point))
		return HTTRANSPARENT;

	return CXTPPopupBar::OnNcHitTest(point);
}

CSize CXTPRibbonSystemPopupBar::CalcDynamicLayout(int nLength, DWORD dwMode)
{
	CArray<CXTPControl*, CXTPControl*> arrButtons;
	int i;

	for (i = 0; i < GetControlCount(); i++)
	{
		CXTPControl* pControl = GetControl(i);
		if (pControl && pControl->IsKindOf(RUNTIME_CLASS(CXTPRibbonControlSystemPopupBarButton)))
		{
			arrButtons.Add(pControl);
			pControl->SetHideFlag(xtpHideDockingPosition, TRUE);
		}
	}

	CSize sz = CXTPPopupBar::CalcDynamicLayout(nLength, dwMode);

	if (arrButtons.GetSize() == 0)
		return sz;

	CClientDC dc(this);
	CXTPFontDC font(&dc, GetPaintManager()->GetCommandBarFont(this));

	int nRight = sz.cx - m_rcBorders.right + 1;

	for (i = (int)arrButtons.GetSize() - 1; i >= 0; i--)
	{
		CXTPControl* pControl = arrButtons[i];
		pControl->SetHideFlag(xtpHideDockingPosition, FALSE);

		CSize szControl = pControl->GetSize(&dc);
		pControl->SetRect(CRect(nRight - szControl.cx, sz.cy - m_rcBorders.bottom + 4, nRight, sz.cy - 3));
		nRight -= szControl.cx + 6;
	}

	return sz;
}

//////////////////////////////////////////////////////////////////////////
// CXTPRibbonControlSystemPopupBarButton

IMPLEMENT_XTP_CONTROL(CXTPRibbonControlSystemPopupBarButton, CXTPControlButton)

CXTPRibbonControlSystemPopupBarButton::CXTPRibbonControlSystemPopupBarButton()
{

}

BOOL CXTPRibbonControlSystemPopupBarButton::IsSystemPopupButton() const
{
	return m_pParent && m_pParent->IsKindOf(RUNTIME_CLASS(CXTPRibbonSystemPopupBar));
}

BOOL CXTPRibbonControlSystemPopupBarButton::IsTransparent() const
{
	if (IsSystemPopupButton())
		return TRUE;

	return CXTPControlButton::IsTransparent();
}

void CXTPRibbonControlSystemPopupBarButton::Draw(CDC* pDC)
{
	if (IsSystemPopupButton())
	{
		CXTPRibbonPaintManager* pPaintManager = GetPaintManager()->GetRibbonPaintManager();
		pPaintManager->DrawSystemPopupBarButton(pDC, this);
	}
	else
	{
		CXTPControlButton::Draw(pDC);
	}
}

CSize CXTPRibbonControlSystemPopupBarButton::GetSize(CDC* pDC)
{
	if (IsSystemPopupButton())
		return GetPaintManager()->DrawControlToolBarParent(pDC, this, FALSE);

	return CXTPControlButton::GetSize(pDC);
}

CSize CXTPRibbonControlSystemPopupBarButton::GetButtonSize() const
{
	return CSize(22, 22);
}

CSize CXTPRibbonControlSystemPopupBarButton::GetIconSize() const
{
	return CSize(16, 16);
}


//////////////////////////////////////////////////////////////////////////
// CXTPControlRecentFileList
IMPLEMENT_XTP_CONTROL(CXTPRibbonControlSystemPopupBarListItem, CXTPControlButton)

CXTPRibbonControlSystemPopupBarListItem::CXTPRibbonControlSystemPopupBarListItem()
{
	m_nWidth = 300;
	m_nHeight = 21;
	m_bAlignShortcut = FALSE;
}

CSize CXTPRibbonControlSystemPopupBarListItem::GetSize(CDC* /*pDC*/)
{
	return CSize(m_nWidth, m_nHeight);
}

void CXTPRibbonControlSystemPopupBarListItem::Draw(CDC* pDC)
{
	CXTPPaintManager* pPaintManager = (CXTPPaintManager*)GetPaintManager();

	pPaintManager->DrawControlEntry(pDC, this);

	COLORREF clrText = pPaintManager->GetControlTextColor(this);

	pDC->SetTextColor(clrText);
	pDC->SetBkMode (TRANSPARENT);

	CRect rc(GetRect());
	CRect rcText(rc.left + 7, rc.top, rc.right, rc.bottom);
	CString strText(GetCaption());

	if (m_nIconId > 0)
	{
		CXTPImageManagerIcon* pIcon = GetImage();
		if (pIcon)
		{
			pIcon->Draw(pDC, CPoint(rcText.left, (rcText.top + rcText.bottom - pIcon->GetExtent().cy) / 2));
			rcText.left += pIcon->GetExtent().cx + 4;
		}
	}

	if (m_bAlignShortcut)
	{
		if (strText.GetLength() > 2 && strText[0] == _T('&') && strText[2] == _T(' '))
		{
			pDC->DrawText(strText.Left(2), &rcText, DT_SINGLELINE | DT_VCENTER);
			strText.Delete(0, 3);
		}

		rcText.left += pDC->GetTextExtent(_T("0"), 1).cx + 7;
	}

	pDC->DrawText(strText, &rcText, DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS);
}

//////////////////////////////////////////////////////////////////////////
// CXTPRibbonControlSystemPopupBarPinableListItem

IMPLEMENT_XTP_CONTROL(CXTPRibbonControlSystemPopupBarPinableListItem, CXTPRibbonControlSystemPopupBarListItem)

#define  PIN_WIDTH 28

CXTPRibbonControlSystemPopupBarPinableListItem::CXTPRibbonControlSystemPopupBarPinableListItem()
{
	m_pIcons = new CXTPImageManager();

	m_pIcons->SetIcons(XTP_IDB_RIBBON_PINICON, 0, 3, 0, xtpImageNormal);

}

CXTPRibbonControlSystemPopupBarPinableListItem::~CXTPRibbonControlSystemPopupBarPinableListItem()
{
	SAFE_DELETE(m_pIcons);
}

void CXTPRibbonControlSystemPopupBarPinableListItem::Draw(CDC* pDC)
{
	CXTPPaintManager* pPaintManager = (CXTPPaintManager*)GetPaintManager();

	if (m_bSelected == TRUE_SPLITDROPDOWN)
	{
		CRect rcCheck(GetRect());
		rcCheck.left = rcCheck.right - PIN_WIDTH;

		pPaintManager->DrawRectangle(pDC, rcCheck, GetSelected(), GetPressed(), GetEnabled(), FALSE,
			FALSE, GetParent()->GetType(), GetParent()->GetPosition());
	}
	else
	{
		pPaintManager->DrawRectangle(pDC, GetRect(), GetSelected(), GetPressed(), GetEnabled(), FALSE,
			FALSE, GetParent()->GetType(), GetParent()->GetPosition());
	}

	COLORREF clrText = pPaintManager->GetControlTextColor(this);

	pDC->SetTextColor(clrText);
	pDC->SetBkMode (TRANSPARENT);

	CRect rc(GetRect());
	CRect rcText(rc.left + 7, rc.top, rc.right - PIN_WIDTH, rc.bottom);
	CString strText(GetCaption());

	if (m_nIconId > 0)
	{
		CXTPImageManagerIcon* pIcon = GetImage();
		if (pIcon)
		{
			pIcon->Draw(pDC, CPoint(rcText.left, (rcText.top + rcText.bottom - pIcon->GetExtent().cy) / 2));
			rcText.left += pIcon->GetExtent().cx + 4;
		}
	}

	if (m_bAlignShortcut)
	{
		if (strText.GetLength() > 2 && strText[0] == _T('&') && strText[2] == _T(' '))
		{
			pDC->DrawText(strText.Left(2), &rcText, DT_SINGLELINE | DT_VCENTER);
			strText.Delete(0, 3);
		}

		rcText.left += pDC->GetTextExtent(_T("0"), 1).cx + 7;
	}

	pDC->DrawText(strText, &rcText, DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS);


	CRect rcCheck(rc.right - PIN_WIDTH, rc.top, rc.right, rc.bottom);

	m_pIcons->GetImage(GetChecked() ? 0 : 1)->Draw(pDC, CPoint(rcCheck.CenterPoint().x - 8, rcCheck.CenterPoint().y - 8));
}

void CXTPRibbonControlSystemPopupBarPinableListItem::OnMouseMove(CPoint point)
{
	CRect rcCheck(GetRect());
	rcCheck.left = rcCheck.right - PIN_WIDTH;

	if (m_bSelected && rcCheck.PtInRect(point) && m_bSelected != TRUE_SPLITDROPDOWN)
	{
		m_bSelected = TRUE_SPLITDROPDOWN;
		RedrawParent(FALSE);
		return;
	}
	else if (m_bSelected == TRUE_SPLITDROPDOWN && !rcCheck.PtInRect(point) && GetRect().PtInRect(point))
	{
		m_bSelected = TRUE;
		RedrawParent(FALSE);
		return;
	}

	CXTPRibbonControlSystemPopupBarListItem::OnMouseMove(point);
}


BOOL CXTPRibbonControlSystemPopupBarPinableListItem::OnSetSelected(int bSelected)
{
	if (!CXTPRibbonControlSystemPopupBarListItem::OnSetSelected(bSelected))
		return FALSE;

	if (bSelected && !IsKeyboardSelected(bSelected))
	{
		CRect rcCheck(GetRect());
		rcCheck.left = rcCheck.right - PIN_WIDTH;

		CPoint point;
		GetCursorPos(&point);
		GetParent()->ScreenToClient(&point);

		if (rcCheck.PtInRect(point))
			m_bSelected = TRUE_SPLITDROPDOWN;
	}


	return TRUE;
}

void CXTPRibbonControlSystemPopupBarPinableListItem::OnLButtonUp(CPoint point)
{
	CRect rcCheck(GetRect());
	rcCheck.left = rcCheck.right - PIN_WIDTH;

	if (rcCheck.PtInRect(point))
	{
		SetChecked(!GetChecked());
		return;
	}

	CXTPRibbonControlSystemPopupBarListItem::OnLButtonUp(point);

}

//////////////////////////////////////////////////////////////////////////
// CXTPRibbonControlSystemPopupBarListCaption


IMPLEMENT_XTP_CONTROL(CXTPRibbonControlSystemPopupBarListCaption, CXTPControl)

CXTPRibbonControlSystemPopupBarListCaption::CXTPRibbonControlSystemPopupBarListCaption()
{
	SetFlags(xtpFlagManualUpdate | xtpFlagSkipFocus | xtpFlagNoMovable | xtpFlagWrapRow);
	m_nWidth = 300;
	m_nHeight = 27;
}

CSize CXTPRibbonControlSystemPopupBarListCaption::GetSize(CDC* /*pDC*/)
{
	return CSize(m_nWidth, m_nHeight);
}

void CXTPRibbonControlSystemPopupBarListCaption::Draw(CDC* pDC)
{
	CXTPPaintManager* pPaintManager = GetPaintManager();
	CXTPFontDC dont(pDC, pPaintManager->GetRegularBoldFont());

	COLORREF clrText = pPaintManager->GetControlTextColor(this);

	pDC->SetTextColor(clrText);
	pDC->SetBkMode (TRANSPARENT);

	CRect rc(GetRect());
	CRect rcText(rc.left + 7, rc.top + 4, rc.right, rc.bottom - 5);

	pDC->DrawText(GetCaption(), &rcText, DT_SINGLELINE | DT_TOP);

	pPaintManager->HorizontalLine(pDC, rc.left, rc.bottom - 6, rc.right,
		pPaintManager->GetRibbonPaintManager()->m_clrRecentFileListEdgeShadow);
	pPaintManager->HorizontalLine(pDC, rc.left, rc.bottom - 5, rc.right,
		pPaintManager->GetRibbonPaintManager()->m_clrRecentFileListEdgeHighLight);
}

IMPLEMENT_XTP_CONTROL(CXTPRibbonControlSystemRecentFileList, CXTPRibbonControlSystemPopupBarListCaption)

CXTPRibbonControlSystemRecentFileList::CXTPRibbonControlSystemRecentFileList()
{
}

CRecentFileList* CXTPRibbonControlSystemRecentFileList::GetRecentFileList()
{
#ifndef _XTP_ACTIVEX
	USES_PROTECTED_ACCESS(CXTPRibbonControlSystemRecentFileList, CWinApp, CRecentFileList*, m_pRecentFileList)

	return PROTECTED_ACCESS(CWinApp, AfxGetApp(), m_pRecentFileList);
#else
	CXTPCommandBars* pCommandBars = m_pParent->GetCommandBars();
	return pCommandBars->m_pRecentFileList;
#endif

}

int CXTPRibbonControlSystemRecentFileList::GetFirstMruID()
{
#ifdef _XTP_ACTIVEX
	return GetID();
#else
	return ID_FILE_MRU_FILE1;
#endif
}

UINT AFXAPI AfxGetFileTitle(LPCTSTR lpszPathName, LPTSTR lpszTitle, UINT nMax);

CString CXTPRibbonControlSystemRecentFileList::ConstructCaption(const CString& lpszTitle, int nIndex)
{
	CString strTemp;

	// double up any '&' characters so they are not underlined
	LPCTSTR lpszSrc = lpszTitle;
	LPTSTR lpszDest = strTemp.GetBuffer(lpszTitle.GetLength() * 2);

	while (*lpszSrc != 0)
	{
		if (*lpszSrc == '&')
			*lpszDest++ = '&';

		if (_istlead(*lpszSrc))
			*lpszDest++ = *lpszSrc++;
		*lpszDest++ = *lpszSrc++;
	}

	*lpszDest = 0;
	strTemp.ReleaseBuffer();

	CString strTitle;

	if (nIndex == 0)
		return strTemp;

	if (nIndex < 10)
	{
		strTitle.Format(_T("&%i %s"), nIndex, (LPCTSTR)strTemp);
	}
	else
	{
		strTitle = strTemp;
	}

	return strTitle;
}

class CXTPRibbonControlSystemRecentFileList::CControlFileItem : public CXTPRibbonControlSystemPopupBarListItem
{
public:
	CControlFileItem()
	{
		m_bAlignShortcut = TRUE;
	}
};

class CXTPRibbonControlSystemRecentFileList::CControlPinableFileItem : public CXTPRibbonControlSystemPopupBarPinableListItem
{
public:
	CControlPinableFileItem(CXTPRecentFileList* pFileList, int iMRU)
	{
		m_bAlignShortcut = TRUE;
		m_iMRU = iMRU;
		m_pFileList = pFileList;
	}

	void SetChecked(BOOL bChecked)
	{
		CXTPControl::SetChecked(bChecked);

		m_pFileList->GetItem(m_iMRU)->SetPinned(bChecked);
	}

protected:
	int m_iMRU;
	CXTPRecentFileList* m_pFileList;
};


void CXTPRibbonControlSystemRecentFileList::OnCalcDynamicSize(DWORD /*dwMode*/)
{
	CRecentFileList* pRecentFileList = GetRecentFileList();

	if (!pRecentFileList)
		return;

	ASSERT(pRecentFileList->m_arrNames != NULL);
	if (!pRecentFileList->m_arrNames)
		return;

	while (m_nIndex + 1 < m_pControls->GetCount())
	{
		CXTPControl* pControl = m_pControls->GetAt(m_nIndex + 1);
		if (pControl->GetID() >= GetFirstMruID() && pControl->GetID() <= GetFirstMruID() + pRecentFileList->m_nSize)
		{
			m_pControls->Remove(pControl);
		}
		else break;
	}

	if (m_pParent->IsCustomizeMode())
	{
		m_dwHideFlags = 0;
		SetEnabled(TRUE);
		return;
	}

	CString strName;
	BOOL bExtendedRecentFileList = pRecentFileList->m_strOriginal == _T("PinableRecentFileList");
	BOOL bPinable = bExtendedRecentFileList && ((CXTPRecentFileList*)pRecentFileList)->m_bPinable;

	for (int iMRU = 0; iMRU < pRecentFileList->m_nSize; iMRU++)
	{
		if (pRecentFileList->m_arrNames[iMRU].IsEmpty())
			break;

		// copy file name only since directories are same
		AfxGetFileTitle(pRecentFileList->m_arrNames[iMRU], strName.GetBuffer(_MAX_PATH), _MAX_PATH);
		strName.ReleaseBuffer();

#ifdef _XTP_ACTIVEX
		int nId = GetFirstMruID();
#else
		int nId = iMRU + GetFirstMruID();
#endif

		CXTPRecentFileListItem* pItem = bExtendedRecentFileList ? ((CXTPRecentFileList*)pRecentFileList)->GetItem(iMRU) : NULL;
		if (pItem)
		{
			if (!pItem->GetCaption().IsEmpty())
				strName = pItem->GetCaption();
		}

		CXTPControl* pControl = m_pControls->Add(!bPinable ? (CXTPControl*)new CControlFileItem() :
			(CXTPControl*) new CControlPinableFileItem((CXTPRecentFileList*)pRecentFileList, iMRU),
			nId, _T(""), m_nIndex + iMRU + 1 , TRUE);

		if (pItem && bPinable)
		{
			pControl->SetChecked(pItem->IsPinned());
		}

		if (pItem)
		{
			pControl->SetIconId(pItem->GetIconId());
		}

		pControl->SetCaption(ConstructCaption(strName, iMRU + 1));
		pControl->SetFlags(xtpFlagManualUpdate|xtpFlagShowPopupBarTip);
		pControl->SetTooltip(pRecentFileList->m_arrNames[iMRU]);
		pControl->SetParameter(pRecentFileList->m_arrNames[iMRU]);
		pControl->SetDescription(NULL);
	}
}

BOOL CXTPRibbonControlSystemRecentFileList::IsCustomizeDragOverAvail(CXTPCommandBar* pCommandBar, CPoint /*point*/, DROPEFFECT& dropEffect)
{
	if (pCommandBar->GetType() != xtpBarTypePopup)
	{
		dropEffect = DROPEFFECT_NONE;
		return FALSE;
	}
	return TRUE;
}



//////////////////////////////////////////////////////////////////////////
// CXTPRibbonSystemPopupBarPage


IMPLEMENT_XTP_COMMANDBAR(CXTPRibbonSystemPopupBarPage, CXTPPopupBar)

CXTPRibbonSystemPopupBarPage::CXTPRibbonSystemPopupBarPage()
{
}

CSize CXTPRibbonSystemPopupBarPage::CalcDynamicLayout(int nLength, DWORD dwMode)
{
	CSize sz = CXTPPopupBar::CalcDynamicLayout(nLength, dwMode);

	if (!m_pControlPopup->GetParent()->IsKindOf(RUNTIME_CLASS(CXTPRibbonSystemPopupBar)))
		return sz;

	CXTPRibbonSystemPopupBar* pParent = ((CXTPRibbonSystemPopupBar*)m_pControlPopup->GetParent());

	int nHeight = CXTPClientRect(pParent).Height() - (pParent->GetBorders().top + pParent->GetBorders().bottom) - 2;

	m_nMaxHeight = nHeight;

	if (nHeight > sz.cy)
		sz.cy = nHeight;

	return sz;
}

void CXTPRibbonSystemPopupBarPage::AdjustExcludeRect(CRect& rc, BOOL bVertical)
{
	if (!m_pControlPopup->GetParent()->IsKindOf(RUNTIME_CLASS(CXTPRibbonSystemPopupBar)))
	{
		CXTPPopupBar::AdjustExcludeRect(rc, bVertical);
		return;
	}

	rc.top = ((CXTPRibbonSystemPopupBar*)m_pControlPopup->GetParent())->GetBorders().top + 1;
	rc.right += 1;
}

#ifdef _XTP_ACTIVEX


BEGIN_DISPATCH_MAP(CXTPRibbonControlSystemButton, CXTPControlPopup)
	DISP_PROPERTY_ID(CXTPRibbonControlSystemButton, "CloseOnDblClick", 200, m_bCloseOnDblClick, VT_BOOL)
END_DISPATCH_MAP()

// {56728A35-789A-40d9-B564-7577DA549319}
static const GUID IID_IRibbonControlSystemButton =
{ 0x56728a35, 0x789a, 0x40d9, { 0xb5, 0x64, 0x75, 0x77, 0xda, 0x54, 0x93, 0x19 } };

BEGIN_INTERFACE_MAP(CXTPRibbonControlSystemButton, CXTPControlPopup)
	INTERFACE_PART(CXTPRibbonControlSystemButton, IID_IRibbonControlSystemButton, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPRibbonControlSystemButton, IID_IRibbonControlSystemButton)


#endif
