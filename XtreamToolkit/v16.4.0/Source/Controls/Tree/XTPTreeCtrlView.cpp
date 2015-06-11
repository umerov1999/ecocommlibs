// XTPTreeCtrl.cpp : implementation file
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

#include "XTPTreeBase.h"
#include "XTPTreeCtrlView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CXTPTreeView

CXTPTreeView::CXTPTreeView()
{
	m_pTreeCtrl = (CTreeCtrl_Friendly*)&GetTreeCtrl();

}

CXTPTreeView::~CXTPTreeView()
{
}

IMPLEMENT_DYNCREATE(CXTPTreeView, CTreeView)

BEGIN_MESSAGE_MAP(CXTPTreeView, CTreeView)
	//{{AFX_MSG_MAP(CXTPTreeView)
	ON_TREECTRL_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CXTPTreeCtrl

CXTPTreeCtrl::CXTPTreeCtrl()
{
	m_pTreeCtrl = (CTreeCtrl_Friendly*)this;
}

CXTPTreeCtrl::~CXTPTreeCtrl()
{
}

IMPLEMENT_DYNAMIC(CXTPTreeCtrl, CTreeCtrl)

BEGIN_MESSAGE_MAP(CXTPTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CXTPTreeCtrl)
	ON_TREECTRL_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTPTreeCtrl message handlers
