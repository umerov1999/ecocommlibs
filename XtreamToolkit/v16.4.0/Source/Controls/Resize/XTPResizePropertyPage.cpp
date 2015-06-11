// XTPResizePropertyPage.cpp: implementation of the CXTPResizePropertyPage class.
//
// This file is a part of the XTREME CONTROLS MFC class library.
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

#include "stdafx.h"


#include "XTPResizeRect.h"
#include "XTPResizePoint.h"
#include "XTPResize.h"
#include "XTPResizePropertyPage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#pragma warning(disable: 4355) // 'this' : used in base member initializer list

/////////////////////////////////////////////////////////////////////////////
// CXTPResizePropertyPage

IMPLEMENT_DYNCREATE(CXTPResizePropertyPage, CPropertyPage)

BEGIN_MESSAGE_MAP(CXTPResizePropertyPage, CPropertyPage)
	//{{AFX_MSG_MAP(CXTPResizePropertyPage)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTPResizePropertyPage construction/destruction

CXTPResizePropertyPage::CXTPResizePropertyPage(const UINT nTemplate, const UINT nCaption, const UINT nFlags) :
	CPropertyPage(nTemplate, nCaption),
	CXTPResize(this, nFlags | xtpResizeNoSizeIcon)
{
	m_nDialogID = nTemplate;
}

BOOL CXTPResizePropertyPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	CXTPResize::Init();

	return TRUE;
}

void CXTPResizePropertyPage::OnGetMinMaxInfo(MINMAXINFO* pMMI)
{
	CXTPResize::GetMinMaxInfo(pMMI);
}

void CXTPResizePropertyPage::OnSize(UINT nType, int cx, int cy)
{
	CPropertyPage::OnSize(nType, cx, cy);
	CXTPResize::Size();
}

void CXTPResizePropertyPage::OnDestroy()
{
	// Save window placement if AutoLoadPlacement was used.
	if (!m_strSection.IsEmpty())
	{
		SavePlacement(m_strSection);
	}

	CPropertyPage::OnDestroy();
}
