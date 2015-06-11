// XTPCommandBarScrollBarCtrl.cpp : implementation of the CXTPCommandBarScrollBarCtrl class.
//
// This file is a part of the XTREME COMMANDBARS MFC class library.
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
#include <Common/XTPWinThemeWrapper.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPSystemHelpers.h>

#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollInfo.h>
#include <Common/ScrollBar/XTPScrollBarCtrl.h>
#include <Common/ScrollBar/XTPScrollBarPaintManager.h>

#include <CommandBars/XTPCommandBarsDefines.h>
#include <CommandBars/XTPCommandBars.h>
#include <CommandBars/XTPPaintManager.h>
#include <CommandBars/XTPControl.h>
#include <CommandBars/XTPControlButton.h>
#include <CommandBars/XTPControlPopup.h>
#include <CommandBars/XTPControlGallery.h>

#include <CommandBars/ScrollBar/XTPCommandBarScrollBarCtrl.h>


//////////////////////////////////////////////////////////////////////////
// CXTPCommandBarScrollBarCtrl

CXTPCommandBarScrollBarCtrl::CXTPCommandBarScrollBarCtrl()
{
	m_pCommandBars = 0;
}

CXTPCommandBarScrollBarCtrl::~CXTPCommandBarScrollBarCtrl()
{

}

BEGIN_MESSAGE_MAP(CXTPCommandBarScrollBarCtrl, CXTPScrollBarCtrl)
	//{{AFX_MSG_MAP(CXTPStatusBar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CXTPScrollBarPaintManager* CXTPCommandBarScrollBarCtrl::GetScrollBarPaintManager() const
{
	CXTPScrollBarPaintManager *pScrollBarPaintManager = NULL;

	if (NULL != m_pCommandBars)
	{
		pScrollBarPaintManager = m_pCommandBars->GetPaintManager()->GetScrollBarPaintManager();
	}
	else
	{
		pScrollBarPaintManager = XTPPaintManager()->GetScrollBarPaintManager();
	}

	return pScrollBarPaintManager;
}
