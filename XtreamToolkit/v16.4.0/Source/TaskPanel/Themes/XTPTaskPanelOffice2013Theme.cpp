// XTPTaskPanelOffice2013Theme.cpp : implementation of the CXTPTaskPanelOffice2013Theme class.
//
// This file is a part of the XTREME TASKPANEL MFC class library.
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

#include <Common/XTPVC80Helpers.h>
#include <Common/XTPDrawHelpers.h>
#include <Common/XTPWinThemeWrapper.h>
#include <Common/XTPImageManager.h>
#include <Common/XTPResourceManager.h>
#include <Common/XTPResourceImage.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPColorManager.h>
#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollBarCtrl.h>
#include <Common/ScrollBar/XTPScrollBarContainer.h>

#include <TaskPanel/Resource.h>
#include <TaskPanel/XTPTaskPanelDefines.h>
#include <TaskPanel/XTPTaskPanel.h>
#include <TaskPanel/XTPTaskPanelItem.h>
#include <TaskPanel/XTPTaskPanelGroup.h>
#include <TaskPanel/XTPTaskPanelGroupItem.h>
#include <TaskPanel/XTPTaskPanelItems.h>
#include <TaskPanel/XTPTaskPanelPaintManager.h>

#include "XTPTaskPanelPaintManagerPlain.h"
#include "XTPTaskPanelToolboxTheme.h"
#include "XTPTaskPanelToolboxWhidbeyTheme.h"
#include "XTPTaskPanelVisualStudio2012Theme.h"
#include "XTPTaskPanelOffice2013Theme.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPTaskPanelOffice2013Theme

CXTPTaskPanelOffice2013Theme::CXTPTaskPanelOffice2013Theme()
	:CXTPTaskPanelVisualStudio2012Theme(TRUE)
{

}

CXTPTaskPanelOffice2013Theme::~CXTPTaskPanelOffice2013Theme()
{
}

void CXTPTaskPanelOffice2013Theme::RefreshMetrics()
{
	CXTPTaskPanelVisualStudio2012Theme::RefreshMetrics();

	m_rcGroupOuterMargins.top = m_rcGroupOuterMargins.bottom = 5;
	m_rcItemOuterMargins.top = m_rcItemOuterMargins.bottom = 1;

	m_clrBackground.SetStandardValue(RGB(248, 248, 248));
	m_clrHighlightSelected.SetStandardValue(RGB(225, 225, 225));
	m_clrHighlightHot.SetStandardValue(RGB(205, 230, 247));
	m_clrSelectedWithoutFocus = m_clrHighlightSelected;
	m_clrText.SetStandardValue(RGB(68, 68, 68));
	m_clrSelectedText.SetStandardValue(RGB(38, 38, 38));//and must be bold
	m_clrSelectedTextWithoutFocus = m_clrText;

	m_clrTriangle.SetStandardValue(RGB(131,134,136));
	m_clrTriangleHot.SetStandardValue(RGB(42,141,212));
	m_clrTriangleFocused = m_clrTriangleHot;

	//set colors for groups
	m_groupNormal.clrHead.SetStandardValue(m_clrBackground);
	m_groupNormal.clrClient.SetStandardValue(m_clrBackground);
	m_groupSpecial = m_groupNormal;

	//selected, pressed and border
	m_clrHighlightBorder = m_clrHighlightSelected;
	m_clrHighlightPressed = m_clrHighlightSelected;
}

CXTPPaintManagerColor CXTPTaskPanelOffice2013Theme::GetTriangleColorAndState(CXTPTaskPanelGroup* pGroup, BOOL &bFilled)
{
	CXTPPaintManagerColor clrTriangle = m_clrTriangle;

	if (pGroup->IsExpanded())
	{
		bFilled = TRUE;

		clrTriangle = m_clrText;
	}
	else
	{
		bFilled = pGroup->IsItemHot();
	}

	if (pGroup->IsItemHot())
		clrTriangle = m_clrTriangleHot;

	return clrTriangle;
}

CRect CXTPTaskPanelOffice2013Theme::DrawGroupCaptionBackground(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc)
{
	if (pGroup->IsItemHot())
		Rectangle(pDC, rc, m_clrHighlightHot, m_clrHighlightHot);

	return DrawExpandButton(pDC, pGroup, rc);
}

void CXTPTaskPanelOffice2013Theme::DrawItemCaption(CDC* pDC, CXTPTaskPanelItem* pItem, CRect rcText, UINT nFormat)
{
	CFont* pOldFont = NULL;
	if (!pItem->IsGroup() && static_cast<CXTPTaskPanelGroupItem*>(pItem)->IsItemSelected())
	{
		pDC->SetTextColor(m_clrSelectedText);
		pOldFont = pDC->SelectObject(&m_fntIconBold);
	}
	else
		pDC->SetTextColor(m_clrText);


	pDC->SetTextColor(m_clrText);
	pDC->SetBkMode(TRANSPARENT);

	pItem->DrawCaptionText(pDC, rcText, nFormat);
	if (pOldFont != NULL) pDC->SelectObject(pOldFont);

	pItem->SetTextRect(pDC, rcText, nFormat);
}
