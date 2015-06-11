// XTPRibbonBackstagePage.cpp : implementation file
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

#include <Common/XTPDrawHelpers.h>
#include <Common/XTPResourceImage.h>
#include <Common/XTPVC80Helpers.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPWinThemeWrapper.h>

#include <Controls/Util/XTPControlTheme.h>
#include <Controls/Resize/XTPResizeRect.h>
#include <Controls/Resize/XTPResizePoint.h>
#include <Controls/Resize/XTPResize.h>
#include <Controls/Resize/XTPResizePropertyPage.h>

#include <Ribbon/Backstage/XTPRibbonBackstagePage.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPRibbonBackstagePage

IMPLEMENT_DYNAMIC(CXTPRibbonBackstagePage, CXTPResizePropertyPage)

CXTPRibbonBackstagePage::CXTPRibbonBackstagePage(UINT nID) : CXTPResizePropertyPage(nID)
{
	LOGFONT lf;
	XTPDrawHelpers()->GetIconLogFont(&lf);

	if (XTPDrawHelpers()->FontExists(_T("Segoe UI"))) // Primary font
	{
		lf.lfHeight = 30;
		STRCPY_S(lf.lfFaceName, LF_FACESIZE, _T("Segoe UI"));
	}
	else if (XTPDrawHelpers()->FontExists(_T("Tahoma"))) // Fall-back font for XP
	{
		lf.lfHeight = 30;
		STRCPY_S(lf.lfFaceName, LF_FACESIZE, _T("Tahoma"));
	}
	else
	{
		lf.lfHeight = 25;
	}
	m_fntTitle.CreateFontIndirect(&lf);


	lf.lfHeight = 18;
	lf.lfWeight = FW_BOLD;

	m_fntCaption.CreateFontIndirect(&lf);
}

BEGIN_MESSAGE_MAP(CXTPRibbonBackstagePage, CXTPResizePropertyPage)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


HBRUSH CXTPRibbonBackstagePage::OnCtlColor(CDC* pDC, CWnd* /*pWnd*/, UINT /*nCtlColor*/)
{
	pDC->SetBkColor(RGB(255, 255, 255));

	return (HBRUSH)GetStockObject(WHITE_BRUSH);
}
