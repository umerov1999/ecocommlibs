// XTPMenuBarMDIMenus.cpp : implementation of the CXTPMenuBarMDIMenus class.
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

#include <Common/XTPResourceManager.h>
#include <Common/XTPImageManager.h>
#include <Common/XTPIntel80Helpers.h>
#include <Common/XTPHookManager.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPColorManager.h>
#include <Common/ScrollBar/XTPScrollInfo.h>

#include <CommandBars/Resource.h>
#include <CommandBars/XTPCommandBarsDefines.h>
#include <CommandBars/XTPCommandBar.h>
#include <CommandBars/XTPToolBar.h>
#include <CommandBars/XTPMenuBar.h>
#include <CommandBars/XTPControls.h>
#include <CommandBars/XTPControl.h>
#include <CommandBars/XTPControlButton.h>
#include <CommandBars/XTPControlPopup.h>
#include <CommandBars/XTPPaintManager.h>
#include <CommandBars/XTPCommandBars.h>
#include <CommandBars/XTPMouseManager.h>
#include <CommandBars/XTPPopupBar.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////

CXTPMenuBarMDIMenus::CXTPMenuBarMDIMenus(CXTPMenuBar* pMenuBar)
	: m_pMenuBar(pMenuBar)
{
#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPMenuBarMDIMenus::~CXTPMenuBarMDIMenus()
{
	RemoveAll();
}

void CXTPMenuBarMDIMenus::RemoveAll()
{
	POSITION pos = m_mapMenus.GetStartPosition();
	while (pos)
	{
		UINT nIDCommand;
		CXTPMenuBarMDIMenuInfo* pInfo;
		m_mapMenus.GetNextAssoc(pos, nIDCommand, pInfo);
		pInfo->InternalRelease();
	}
	m_mapMenus.RemoveAll();
}

void CXTPMenuBarMDIMenus::Remove(UINT nIDResource)
{
	CXTPMenuBarMDIMenuInfo* pInfo = Lookup(nIDResource);
	if (pInfo)
	{
		m_mapMenus.RemoveKey(nIDResource);
		pInfo->InternalRelease();
	}
}

void CXTPMenuBarMDIMenus::SetAt(UINT nIDResource, CXTPMenuBarMDIMenuInfo* pInfo)
{
	ASSERT(pInfo);
	ASSERT(!Lookup(nIDResource));

	m_mapMenus.SetAt(nIDResource, pInfo);
}

CXTPMenuBarMDIMenuInfo* CXTPMenuBarMDIMenus::Lookup(UINT nIDResource) const
{
	CXTPMenuBarMDIMenuInfo* pInfo;
	if (!m_mapMenus.Lookup(nIDResource, pInfo))
		return NULL;

	return pInfo;
}

int CXTPMenuBarMDIMenus::GetCount() const
{
	return (int)m_mapMenus.GetCount();
}

POSITION CXTPMenuBarMDIMenus::GetStartPosition() const
{
	return m_mapMenus.GetStartPosition();
}

void CXTPMenuBarMDIMenus::GetNextMenu(POSITION& pos, CXTPMenuBarMDIMenuInfo*& pInfo) const
{
	UINT nIDResource = 0;
	m_mapMenus.GetNextAssoc(pos, nIDResource, pInfo);
}
