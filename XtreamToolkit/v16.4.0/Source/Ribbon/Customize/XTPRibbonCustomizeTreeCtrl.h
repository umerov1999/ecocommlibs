// XTPRibbonCustomizeTreeCtrl.h: interface for the CXTPRibbonCustomizeTreeCtrl class.
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

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPRIBBONCUSTOMIZETREECTRL_H__)
#define __XTPRIBBONCUSTOMIZETREECTRL_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPCoreTreeItem;
class CXTPCommandBars;

class CXTPRibbonCustomizeTreeCtrl : public CXTPCoreTreeControl
{
public:
	CXTPRibbonCustomizeTreeCtrl();
protected:
	virtual void StartDragItem(CXTPCoreTreeItem* pItem);
	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);

	int GetItemLevel(CXTPCoreTreeItem* pItem) const;

public:
	void UpdateCommandBars();

public:
	BOOL m_bItemsTree;
	CXTPCommandBars* m_pCommandBars;
};

//{{AFX_CODEJOCK_PRIVATE
#endif // __XTPRIBBONCUSTOMIZETREECTRL_H__
//}}AFX_CODEJOCK_PRIVATE