// XTPPropertyGridOffice2013Theme.cpp : implementation of the CXTPPropertyGridOffice2013Theme class.
//
// This file is a part of the XTREME PROPERTYGRID MFC class library.
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

#include <Common/XTPDrawHelpers.h>
#include <Common/XTPImageManager.h>
#include <Common/XTPResourceImage.h>
#include <Common/XTPMarkupRender.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPWinThemeWrapper.h>
#include <Common/XTPSystemHelpers.h>

#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollBarCtrl.h>
#include <Common/ScrollBar/XTPScrollBarContainer.h>

#ifdef _XTP_INCLUDE_CONTROLS
#include <Controls/Util/XTPControlTheme.h>
#endif

#include <PropertyGrid/XTPPropertyGridToolBar.h>
#include <PropertyGrid/XTPPropertyGridDefines.h>
#include <PropertyGrid/XTPPropertyGridPaintManager.h>
#include <PropertyGrid/XTPPropertyGridInplaceButton.h>
#include <PropertyGrid/XTPPropertyGridItem.h>

#include <PropertyGrid/XTPPropertyGrid.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridOffice2013Theme

CXTPPropertyGridOffice2013Theme::CXTPPropertyGridOffice2013Theme(CXTPPropertyGrid* pGrid)
	: CXTPPropertyGridPaintManager(pGrid)
{
	m_buttonsStyle = xtpGridButtonsThemed;

#ifdef _XTP_INCLUDE_CONTROLS
	m_bUseThemedControls = TRUE;
#endif
}

void CXTPPropertyGridOffice2013Theme::RefreshMetrics()
{
	CXTPPropertyGridPaintManager::RefreshMetrics();

	//-----------Colors of text-----------
	//Color of the text items.
	m_pMetrics->m_clrFore.SetStandardValue(RGB(0, 0, 0));

	//Color of the text groups.
	m_pMetrics->m_clrCategoryFore = m_pMetrics->m_clrFore;

	//Color of the text of the disabled items.
	m_pMetrics->m_clrReadOnlyFore.SetStandardValue(RGB(152, 152, 152));

	//-----------Colors of backgrounds-----------
	//color of the background items, and too color of the background propertygrid
	m_pMetrics->m_clrBack.SetStandardValue(RGB(255, 255, 255));

	//color of the background groups and lines between items.
	m_pMetrics->m_clrLine.SetStandardValue(RGB(192, 192, 192));

	//Colors for description pane.
	m_pMetrics->m_clrHelpFore = m_pMetrics->m_clrFore;// Color of the description text.
	m_pMetrics->m_clrHelpBack = m_pMetrics->m_clrBack;// Color of the description background.

	//---
	//Color ot text in Verb pane
	//m_pMetrics->m_clrVerbFace.SetStandardValue(RGB(255, 0, 0));


	//--------
	//This color appears between different the panes of propertygrid, such as Verb and Description.
	m_clrFace = m_pMetrics->m_clrBack;

	//Color the static edge in xtpGridBorderClientEdge mode.
	m_clrShadow = m_pMetrics->m_clrLine;

	//Color of the background for the highlight item.
	m_clrHighlight.SetStandardValue(RGB(0, 0, 0));

	//Color of the text for the highlight item.
	m_clrHighlightText.SetStandardValue(RGB(255, 255, 255));

	//Color of the separator between groups.
	m_clrCategorySeparator = m_pMetrics->m_clrLine;

#ifdef _XTP_INCLUDE_CONTROLS
	if (m_bUseThemedControls)
		SetControlsTheme(xtpControlThemeOffice2013);
#endif
}