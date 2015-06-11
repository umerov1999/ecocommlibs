// XTPTaskPanelListViewTheme.cpp : implementation of the CXTPTaskPanelListViewTheme class.
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

#include <TaskPanel/Resource.h>
#include <TaskPanel/XTPTaskPanelDefines.h>
#include <TaskPanel/XTPTaskPanelItem.h>
#include <TaskPanel/XTPTaskPanelGroup.h>
#include <TaskPanel/XTPTaskPanelGroupItem.h>
#include <TaskPanel/XTPTaskPanelItems.h>
#include <TaskPanel/XTPTaskPanelPaintManager.h>

#include "XTPTaskPanelPaintManagerPlain.h"
#include "XTPTaskPanelToolboxTheme.h"
#include "XTPTaskPanelListViewTheme.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////////
// CXTPTaskPanelListViewTheme

CXTPTaskPanelListViewTheme::CXTPTaskPanelListViewTheme()
{
	m_rcGroupInnerMargins.SetRect(8, 6, 8, 6);
	m_rcControlMargins.SetRectEmpty();
	m_nGroupSpacing = 0;
	m_bOfficeHighlight = FALSE;
	m_bInvertDragRect = FALSE;
	m_bCaptionScrollButton = FALSE;
	m_bEmbossedDisabledText = TRUE;
}

void CXTPTaskPanelListViewTheme::RefreshMetrics()
{
	CXTPTaskPanelToolboxTheme::RefreshMetrics();

	m_clrBackground.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));
	m_groupNormal.clrClient.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));
	m_groupNormal.clrClientText.SetStandardValue(GetXtremeColor(COLOR_WINDOW));
	m_groupNormal.clrClientLink.SetStandardValue(GetXtremeColor(COLOR_WINDOW));
	m_groupNormal.clrClientLinkHot.SetStandardValue(GetXtremeColor(COLOR_WINDOW));
	m_groupNormal.clrClientLinkSelected.SetStandardValue(GetXtremeColor(COLOR_WINDOW));
	m_nCaptionHeight = max(20, m_nCaptionHeight);

	if (!m_bOfficeHighlight)
	{
		m_clrHighlightHot.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));
		m_clrHighlightBorder.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT), GetXtremeColor(COLOR_3DFACE));
	}
	m_groupSpecial = m_groupNormal;
}