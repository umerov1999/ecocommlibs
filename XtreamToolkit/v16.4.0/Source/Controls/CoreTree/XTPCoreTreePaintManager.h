// XTPCoreTreePaintManager.h: interface for the CXTPCoreTreePaintManager class.
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

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPCORETREEPAINTMANAGER_H__)
#define __XTPCORETREEPAINTMANAGER_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPCoreTreeControl;
class CXTPCoreTreeItem;
class CXTPWinThemeWrapper;


class _XTP_EXT_CLASS CXTPCoreTreePaintManager
{
public:
	CXTPCoreTreePaintManager();
	virtual ~CXTPCoreTreePaintManager();

public:
	virtual void RefreshMetrics();

public:
	virtual void FillTreeControl(CDC* pDC, CXTPCoreTreeControl* pTreeControl);

	virtual void DrawItem(CDC* pDC, CXTPCoreTreeItem* pItem);

	virtual int GetChildrenIndent(CXTPCoreTreeItem* pItem);

	virtual int GetItemHeight() const;

	virtual void SetFontIndirect(LOGFONT* pLogFont);

	virtual void DrawDropRect(CDC* pDC, CXTPCoreTreeItem* pItem);


	BOOL IsAppThemed() const;
protected:
	virtual void DrawButton(CDC* pDC, CXTPCoreTreeItem* pItem, CRect rcItem);
	virtual void DrawCheckbox(CDC* pDC, CXTPCoreTreeItem* pItem, CRect rcItem);


public:
	BOOL m_bShowButtons;

protected:

	COLORREF m_clrBackground;

	COLORREF m_clrText;

	COLORREF m_clrLabelText;
	COLORREF m_clrLabelBackground;

	COLORREF m_clrSelectedText;
	COLORREF m_clrSelectedBackground;

	COLORREF m_clrDisabledText;

	CXTPWinThemeWrapper* m_themeButton;
	CXTPWinThemeWrapper* m_themeTree;

	int m_nItemHeight;

	CFont m_fntItem;
	CFont m_fntLabel;
};

AFX_INLINE int CXTPCoreTreePaintManager::GetItemHeight() const {
	return m_nItemHeight;
}

#endif // !defined(__XTPCORETREEPAINTMANAGER_H__)
