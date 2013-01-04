// XTPMarkupKeyboardNavigation.h: interface for the CXTPMarkupKeyboardNavigation class.
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// (c)1998-2012 Codejock Software, All Rights Reserved.
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
#if !defined(__XTPMMARKUPKEYBOARDNAVIGATION_H__)
#define __XTPMMARKUPKEYBOARDNAVIGATION_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPMarkupInputElement;
class CXTPMarkupContext;


enum XTPMarkupFocusNavigationDirection
{
	xtpMarkupFocusNavigationDirectionNext,
	xtpMarkupFocusNavigationDirectionPrevious,
	xtpMarkupFocusNavigationDirectionFirst,
	xtpMarkupFocusNavigationDirectionLast,
	xtpMarkupFocusNavigationDirectionLeft,
	xtpMarkupFocusNavigationDirectionRight,
	xtpMarkupFocusNavigationDirectionUp,
	xtpMarkupFocusNavigationDirectionDown
};

enum XTPMarkupKeyboardNavigationMode
{
	xtpMarkupKeyboardNavigationContinue,
	xtpMarkupKeyboardNavigationOnce,
	xtpMarkupKeyboardNavigationCycle,
	xtpMarkupKeyboardNavigationNone,
	xtpMarkupKeyboardNavigationContained,
	xtpMarkupKeyboardNavigationLocal
};


class _XTP_EXT_CLASS CXTPMarkupKeyboardNavigation : public CXTPMarkupObject
{
	DECLARE_MARKUPCLASS(CXTPMarkupKeyboardNavigation)

protected:
	CXTPMarkupKeyboardNavigation(CXTPMarkupContext* pContext = NULL);
	virtual ~CXTPMarkupKeyboardNavigation();

public:
	static XTPMarkupKeyboardNavigationMode AFX_CDECL GetTabNavigation(CXTPMarkupObject* pElement);


public:
	static CXTPMarkupObject* AFX_CDECL ConvertKeyboardNavigationMode(CXTPMarkupBuilder* pBuilder, CXTPMarkupObject* pObject);

public:
	void ProcessInput(UINT nChar);
	void Focus(CXTPMarkupInputElement* pInputElement);

protected:
	BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

protected:
	void Navigate(CXTPMarkupInputElement* pSourceElement, UINT nChar);
	void Navigate(CXTPMarkupInputElement* pSourceElement, XTPMarkupFocusNavigationDirection direction);

private:
	CXTPMarkupObject* GetGroupParent(CXTPMarkupObject* e, BOOL bIncludeCurrent = FALSE);
	CXTPMarkupObject* GetParent(CXTPMarkupObject* e);
	BOOL IsInNavigationTree(CXTPMarkupObject* pVisual);
	XTPMarkupKeyboardNavigationMode GetKeyNavigationMode(CXTPMarkupObject* pElement);
	BOOL IsGroup(CXTPMarkupObject* e);
	CXTPMarkupObject* GetNextTab(CXTPMarkupObject* e, CXTPMarkupObject* pContainer, BOOL goDownOnly);
	BOOL IsTabStop(CXTPMarkupObject* e);
	CXTPMarkupObject* GetTabOnceActiveElement(CXTPMarkupObject* d);
	CXTPMarkupObject* GetActiveElement(CXTPMarkupObject* d);
	CXTPMarkupObject* GetNextTabInGroup(CXTPMarkupObject* e, CXTPMarkupObject* container, XTPMarkupKeyboardNavigationMode tabbingType);
	CXTPMarkupObject* GetFirstTabInGroup(CXTPMarkupObject* container);
	BOOL IsTabStopOrGroup(CXTPMarkupObject* e);
	CXTPMarkupObject* GetNextInTree(CXTPMarkupObject* e, CXTPMarkupObject* container);
	CXTPMarkupObject* GetNextTabWithSameIndex(CXTPMarkupObject* e, CXTPMarkupObject* container);
	CXTPMarkupObject* GetNextTabWithNextIndex(CXTPMarkupObject* e, CXTPMarkupObject* container, XTPMarkupKeyboardNavigationMode tabbingType);
	int GetTabIndexHelper(CXTPMarkupObject* d);
	CXTPMarkupObject* FocusedElement(CXTPMarkupObject* e);

	CXTPMarkupObject* GetTreeFirstChild(CXTPMarkupObject* e);
	CXTPMarkupObject* GetTreeNextSibling(CXTPMarkupObject* e);

public:
	static CXTPMarkupDependencyProperty* m_pIsTabStopProperty;
	static CXTPMarkupDependencyProperty* m_pTabNavigationProperty;
	static CXTPMarkupDependencyProperty* m_pTabIndexProperty;

protected:
	CXTPMarkupInputElement* m_pFocused;
	CXTPMarkupInputElement* m_pLastFocused;
	CXTPMarkupContext* m_pMarkupContext;
	CXTPMarkupDependencyProperty* m_pNavigationProperty;

private:
	friend class CXTPMarkupContext;
};

#endif // !defined(__XTPMMARKUPKEYBOARDNAVIGATION_H__)
