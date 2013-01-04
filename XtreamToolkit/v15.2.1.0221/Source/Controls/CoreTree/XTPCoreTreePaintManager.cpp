// XTPCoreTreePaintManager.cpp: implementation of the CXTPCoreTreePaintManager class.
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

#include "Common/XTPWinThemeWrapper.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPMarkupRender.h"

#include "XTPCoreTreePaintManager.h"
#include "XTPCoreTreeControl.h"
#include "XTPCoreTreeItem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPCoreTreePaintManager::CXTPCoreTreePaintManager()
{
	m_nItemHeight = NULL;

	m_themeButton = new CXTPWinThemeWrapper();
	m_themeTree = new CXTPWinThemeWrapper();

	m_bShowButtons = TRUE;
}

CXTPCoreTreePaintManager::~CXTPCoreTreePaintManager()
{
	SAFE_DELETE(m_themeButton);
	SAFE_DELETE(m_themeTree);
}

void CXTPCoreTreePaintManager::RefreshMetrics()
{
	m_clrBackground = GetSysColor(COLOR_WINDOW);
	m_clrText = GetSysColor(COLOR_WINDOWTEXT);

	m_clrLabelBackground = RGB(240, 242, 245);
	m_clrLabelText = RGB(59, 59, 59);

	m_clrSelectedText = GetSysColor(COLOR_HIGHLIGHTTEXT);
	m_clrSelectedBackground = GetSysColor(COLOR_HIGHLIGHT);

	m_clrDisabledText = GetSysColor(COLOR_GRAYTEXT);

	m_themeButton->OpenThemeData(NULL, L"BUTTON");
	m_themeTree->OpenThemeData(NULL, L"TREEVIEW");

	LOGFONT lfIcon;
	VERIFY(CXTPDrawHelpers::GetIconLogFont(&lfIcon));

	SetFontIndirect(&lfIcon);

}

void CXTPCoreTreePaintManager::SetFontIndirect(LOGFONT* pLogFont)
{
	m_fntItem.DeleteObject();
	m_fntLabel.DeleteObject();


	pLogFont->lfWeight = FW_BOLD;
	m_fntLabel.CreateFontIndirect(pLogFont);

	pLogFont->lfWeight = FW_NORMAL;
	m_fntItem.CreateFontIndirect(pLogFont);

	{
		CWindowDC dc (NULL);
		CXTPFontDC font(&dc, &m_fntItem);
		m_nItemHeight = dc.GetTextExtent(_T(" "), 1).cy + 2;
		if (m_nItemHeight < 16) m_nItemHeight = 16;
	}
}

void CXTPCoreTreePaintManager::FillTreeControl(CDC* pDC, CXTPCoreTreeControl* pTreeControl)
{
	CXTPClientRect rc(pTreeControl);
	pDC->FillSolidRect(rc, m_clrBackground);
}

void CXTPCoreTreePaintManager::DrawButton(CDC* pDC, CXTPCoreTreeItem* pItem, CRect rcItem)
{
	rcItem.right = rcItem.left + m_nItemHeight;

	CPoint pt = rcItem.CenterPoint();

	CRect rcSign(pt.x - 4, pt.y - 4, pt.x + 5, pt.y + 5);

	if (m_themeTree->IsAppThemed())
	{
		m_themeTree->DrawThemeBackground(pDC->GetSafeHdc(), TVP_GLYPH, pItem->IsExpanded() ? GLPS_OPENED : GLPS_CLOSED, &rcSign, 0);
	}
	else
	{

		pDC->Draw3dRect(rcSign, GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));

		pDC->FillSolidRect(pt.x - 2, pt.y, 5, 1, m_clrText);

		if (!pItem->IsExpanded())
		{
			pDC->FillSolidRect(pt.x, pt.y - 2, 1, 5, m_clrText);
		}
	}
}

void CXTPCoreTreePaintManager::DrawCheckbox(CDC* pDC, CXTPCoreTreeItem* pItem, CRect rcItem)
{
	rcItem.right = rcItem.left + m_nItemHeight;
	BOOL bChecked = pItem->IsChecked();
	BOOL bEnabled = pItem->IsEnabled();

	CPoint pt(rcItem.CenterPoint());
	CRect rcCheckMark(pt.x - 7, pt.y - 7, pt.x + 6, pt.y + 6);

	if (m_themeButton->IsAppThemed())
	{
		m_themeButton->DrawThemeBackground(pDC->GetSafeHdc(), BP_CHECKBOX,
			!bEnabled ? (bChecked ? CBS_CHECKEDDISABLED : CBS_UNCHECKEDDISABLED) :
		(bChecked ? CBS_CHECKEDNORMAL : CBS_UNCHECKEDNORMAL),
			rcCheckMark, 0);
	}
	else
	{
		pDC->DrawFrameControl(rcCheckMark, DFC_BUTTON, DFCS_BUTTONCHECK |
			(!bEnabled ? DFCS_INACTIVE : 0) | (bChecked ? DFCS_CHECKED : 0));
	}

}


void CXTPCoreTreePaintManager::DrawDropRect(CDC* pDC, CXTPCoreTreeItem* pItem)
{
	CRect rcItem(pItem->GetRect());

	CXTPCoreTreeControl* pTreeControl = pItem->GetTreeControl();

	if (pTreeControl->GetDropPosition() == xtpCoreTreeDropBefore)
	{
		pDC->FillSolidRect(rcItem.left, rcItem.top - 1, rcItem.Width(), 2, 0);
	}
	else if (pTreeControl->GetDropPosition() == xtpCoreTreeDropAfter)
	{
		pDC->FillSolidRect(rcItem.left, rcItem.bottom - 1, rcItem.Width(), 2, 0);
	}
	else if (pTreeControl->GetDropPosition() == xtpCoreTreeDropChild)
	{
		int nIndent = GetChildrenIndent(pItem);

		pDC->FillSolidRect(rcItem.left + nIndent, rcItem.bottom - 1, rcItem.Width() - nIndent, 2, 0);
	}
}

void CXTPCoreTreePaintManager::DrawItem(CDC* pDC, CXTPCoreTreeItem* pItem)
{
	if (pItem->IsLabel())
	{
		CXTPFontDC fnt(pDC, &m_fntLabel);
		CRect rc(pItem->GetRect());

		pDC->FillSolidRect(rc, m_clrLabelBackground);
		pDC->FillSolidRect(rc.left, rc.bottom - 1, rc.Width(), 1, RGB(226, 228, 231));

		pDC->SetTextColor(m_clrLabelText);

		CRect rcText(rc);
		rcText.left += 2;

		if (pItem->GetMarkupUIElement())
		{
			XTPMarkupSetDefaultFont(XTPMarkupElementContext(pItem->GetMarkupUIElement()), m_fntLabel, pDC->GetTextColor());
			XTPMarkupRenderElement(pItem->GetMarkupUIElement(), pDC->GetSafeHdc(), rcText);
		}
		else
		{
			pDC->DrawText(pItem->GetCaption(), rcText, DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER);
		}
	}
	else
	{
		CXTPFontDC fnt(pDC, &m_fntItem);

		CRect rcItem(pItem->GetRect());

		if (m_bShowButtons)
		{
			if (pItem->HasChildren())
			{
				DrawButton(pDC, pItem, rcItem);
			}
			rcItem.left += m_nItemHeight;
		}


		if (pItem->IsSelected())
		{
			if (::GetFocus() == pItem->GetTreeControl()->GetSafeHwnd())
			{
				pDC->FillSolidRect(rcItem, m_clrSelectedBackground);
				pDC->SetTextColor(m_clrSelectedText);
			}
			else
			{
				pDC->FillSolidRect(rcItem, GetSysColor(COLOR_BTNFACE));
				pDC->SetTextColor(GetSysColor(COLOR_BTNTEXT));
			}
		}
		else
		{
			pDC->SetTextColor(m_clrText);
		}

		if (!pItem->IsEnabled())
		{
			pDC->SetTextColor(m_clrDisabledText);
		}

		if (pItem->IsCheckboxVisible())
		{
			DrawCheckbox(pDC, pItem, rcItem);
			rcItem.left += m_nItemHeight;
		}

		if (pItem->GetIconId() != -1)
		{

			CRect rcIcon;

			rcIcon.left = rcItem.left + 2;
			rcIcon.right = rcIcon.left + 16;
			rcIcon.top = (rcItem.top + rcItem.bottom - 16) / 2;
			rcIcon.bottom = rcItem.top + 16;

			CXTPImageManagerIcon* pIcon = pItem->GetImage();
			if (pIcon)
			{
				pIcon->Draw(pDC, rcIcon.TopLeft(), pIcon->GetIcon(pItem->IsEnabled() ? xtpImageNormal : xtpImageDisabled), rcIcon.Size());
			}



			rcItem.left += 20;

		}

		CRect rcText(rcItem);
		rcText.left += 2;

		if (pItem->GetMarkupUIElement())
		{
			XTPMarkupSetDefaultFont(XTPMarkupElementContext(pItem->GetMarkupUIElement()), m_fntItem, pDC->GetTextColor());
			XTPMarkupRenderElement(pItem->GetMarkupUIElement(), pDC->GetSafeHdc(), rcText);
		}
		else
		{
			pDC->DrawText(pItem->GetCaption(), rcText, DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER);
		}
	}
}

int CXTPCoreTreePaintManager::GetChildrenIndent(CXTPCoreTreeItem* pItem)
{
	if (pItem->IsCheckboxVisible())
		return 2 * m_nItemHeight;

	return m_nItemHeight;
}

BOOL CXTPCoreTreePaintManager::IsAppThemed() const
{
	return m_themeTree->IsAppThemed();
}
