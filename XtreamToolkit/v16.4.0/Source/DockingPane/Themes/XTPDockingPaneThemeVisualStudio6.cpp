// XTPDockingPaneVisualStudio6Theme.cpp : implementation of the CXTPDockingPaneVisualStudio6Theme class.
//
// This file is a part of the XTREME DOCKINGPANE MFC class library.
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
#include <Common/XTPColorManager.h>
#include <Common/XTPDrawHelpers.h>
#include <Common/XTPResourceImage.h>
#include <Common/XTPResourceManager.h>
#include <Common/XTPWinThemeWrapper.h>
#include <Common/XTPImageManager.h>
#include <Common/XTPSystemHelpers.h>

#include <TabManager/Includes.h>

#include <DockingPane/Resource.h>
#include <DockingPane/XTPDockingPaneDefines.h>
#include <DockingPane/XTPDockingPaneBase.h>
#include <DockingPane/XTPDockingPaneBaseContainer.h>
#include <DockingPane/XTPDockingPanePaintManager.h>
#include <DockingPane/XTPDockingPaneTabbedContainer.h>
#include <DockingPane/XTPDockingPane.h>
#include <DockingPane/XTPDockingPaneMiniWnd.h>
#include <DockingPane/XTPDockingPaneSplitterContainer.h>
#include <DockingPane/XTPDockingPaneSidePanel.h>

#include <DockingPane/Themes/XTPDockingPaneThemeDefault.h>
#include <DockingPane/Themes/XTPDockingPaneThemeGrippered.h>
#include <DockingPane/Themes/XTPDockingPaneThemeVisualStudio6.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneVisualStudio6Theme

CXTPDockingPaneVisualStudio6Theme::CXTPDockingPaneVisualStudio6Theme()
{
	m_nCaptionButtonStyle = xtpPaneCaptionButtonDefault;
}

void CXTPDockingPaneVisualStudio6Theme::DrawCaptionPart(CDC* pDC, CXTPDockingPaneBase* pPane, CRect rcCaption, const CString& strTitle, BOOL bActive, BOOL bVertical)
{
	UNREFERENCED_PARAMETER(strTitle);
	UNREFERENCED_PARAMETER(bActive);

	XTPDeflateRectDirection(rcCaption, 0, 0, 2, 0, bVertical);
	DrawCaptionButtons(pDC, pPane->GetCaptionButtons(), rcCaption, m_clrNormalCaptionText, 14, 2, bVertical);

	int nOffset = 4;
	XTPDeflateRectDirection(rcCaption, 4, 1, 2, 0, bVertical);

	HBRUSH hCustomBrush = GetCustomTitleBrush(bActive, bVertical);
	if(NULL != hCustomBrush)
	{
		pDC->FillRect(rcCaption, CBrush::FromHandle(hCustomBrush));
	}

	if (bVertical)
	{
		if (rcCaption.bottom > nOffset)
		{
			int nCenter = rcCaption.CenterPoint().x;
			pDC->Draw3dRect(nCenter - 3, nOffset, 3, rcCaption.bottom - nOffset, GetXtremeColor(COLOR_BTNHILIGHT), GetXtremeColor(COLOR_3DSHADOW));
			pDC->Draw3dRect(nCenter + 1, nOffset, 3, rcCaption.bottom - nOffset, GetXtremeColor(COLOR_BTNHILIGHT), GetXtremeColor(COLOR_3DSHADOW));
		}
	}
	else
	{
		if (rcCaption.right > nOffset)
		{
			int nCenter = rcCaption.CenterPoint().y;
			pDC->Draw3dRect(nOffset, nCenter - 3, rcCaption.right - nOffset, 3, GetXtremeColor(COLOR_BTNHILIGHT), GetXtremeColor(COLOR_3DSHADOW));
			pDC->Draw3dRect(nOffset, nCenter + 1, rcCaption.right - nOffset, 3, GetXtremeColor(COLOR_BTNHILIGHT), GetXtremeColor(COLOR_3DSHADOW));
		}
	}
}
