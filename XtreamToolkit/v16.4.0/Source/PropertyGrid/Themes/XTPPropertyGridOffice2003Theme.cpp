// XTPPropertyGridOffice2003Theme.cpp : implementation of the CXTPPropertyGridOffice2003Theme class.
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

#ifdef _XTP_INCLUDE_CONTROLS
#include <Controls/Util/XTPControlTheme.h>
#endif

#include <PropertyGrid/XTPPropertyGridDefines.h>
#include <PropertyGrid/XTPPropertyGridPaintManager.h>
#include <PropertyGrid/XTPPropertyGridInplaceButton.h>
#include <PropertyGrid/XTPPropertyGridItem.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// CXTPPropertyGridOffice2003Theme

CXTPPropertyGridOffice2003Theme::CXTPPropertyGridOffice2003Theme(CXTPPropertyGrid* pGrid)
	: CXTPPropertyGridPaintManager(pGrid)
{
	m_bLunaTheme = FALSE;
	m_buttonsStyle = xtpGridButtonsThemed;
}



void CXTPPropertyGridOffice2003Theme::RefreshMetrics()
{
	CXTPPropertyGridPaintManager::RefreshMetrics();

	m_bLunaTheme = FALSE;

	if (!XTPColorManager()->IsLunaColorsDisabled())
	{
		XTPCurrentSystemTheme systemTheme = XTPColorManager()->GetCurrentSystemTheme();

		switch (systemTheme)
		{
		case xtpSystemThemeRoyale:
		case xtpSystemThemeAero:
		case xtpSystemThemeBlue:
			m_clrFace.SetStandardValue(RGB(221, 236, 254));
			m_clrShadow.SetStandardValue(RGB(123, 164, 224));
			m_pMetrics->m_clrHelpBack.SetStandardValue(m_clrFace);
			m_pMetrics->m_clrLine.SetStandardValue(RGB(169, 199, 240));
			m_pMetrics->m_clrCategoryFore.SetStandardValue(0);

			m_bLunaTheme = TRUE;
			break;

		case xtpSystemThemeOlive:
			m_clrFace.SetStandardValue(RGB(243, 242, 231));
			m_clrShadow.SetStandardValue(RGB(188, 187, 177));
			m_pMetrics->m_clrHelpBack.SetStandardValue(m_clrFace);
			m_pMetrics->m_clrLine.SetStandardValue(RGB(197, 212, 159));
			m_pMetrics->m_clrCategoryFore.SetStandardValue(0);

			m_bLunaTheme = TRUE;
			break;

		case xtpSystemThemeSilver:
			m_clrFace.SetStandardValue(RGB(238, 238, 244));
			m_clrShadow.SetStandardValue(RGB(161, 160, 187));
			m_pMetrics->m_clrHelpBack.SetStandardValue(m_clrFace);
			m_pMetrics->m_clrLine.SetStandardValue(RGB(192, 192, 211));
			m_pMetrics->m_clrCategoryFore.SetStandardValue(0);

			m_bLunaTheme = TRUE;
			break;
		}
	}
}
