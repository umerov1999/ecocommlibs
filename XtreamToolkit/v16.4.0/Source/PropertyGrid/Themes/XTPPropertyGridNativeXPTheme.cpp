// XTPPropertyGridNativeXPTheme.cpp : implementation of the CXTPPropertyGridNativeXPTheme class.
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
#include <PropertyGrid/XTPPropertyGridItemBool.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// CXTPPropertyGridNativeXPTheme

CXTPPropertyGridNativeXPTheme::CXTPPropertyGridNativeXPTheme(CXTPPropertyGrid* pGrid)
	: CXTPPropertyGridPaintManager(pGrid)
{
	m_buttonsStyle = xtpGridButtonsThemed;
}

void CXTPPropertyGridNativeXPTheme::RefreshMetrics()
{
	CXTPPropertyGridPaintManager::RefreshMetrics();

	m_clrShadow.SetStandardValue(GetXtremeColor(XPCOLOR_STATICFRAME));
}
