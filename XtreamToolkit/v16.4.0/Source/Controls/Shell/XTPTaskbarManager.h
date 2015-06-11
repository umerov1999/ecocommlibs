// XTPTaskbarManager.h: interface for the CXTPTaskbarManager class.
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
#if !defined(__XTPTASKBARMANAGER_H__)
#define __XTPTASKBARMANAGER_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

interface ITaskbarList3;
interface ICustomDestinationList;
interface IObjectArray;
interface IShellItem;
interface IShellLinkW;


enum XTPTaskbarProgressFlag
{
	xtpProgressFlagNoProgress = 0,
	xtpProgressFlagIndeterminate    = 0x1,
	xtpProgressFlagNormal   = 0x2,
	xtpProgressFlagError    = 0x4,
	xtpProgressFlagPaused   = 0x8
};



class _XTP_EXT_CLASS CXTPTaskbarManagerJumpList : public CXTPCmdTarget
{
protected:
	struct CTask
	{
		CString m_strPath;
		CString m_strArguments;
		CString m_strTitle;
		CString m_strIconPath;
		int m_nIconIndex;
	};

protected:
	CXTPTaskbarManagerJumpList();
	~CXTPTaskbarManagerJumpList();

public:
	void AddTask(LPCTSTR lpszPath, LPCTSTR lpszArgument, LPCTSTR lpszTitle, LPCTSTR lpszIconPath, int nIconIndex = -1);
	void Clear();
	void CommitList();
	void SetAppID(LPCWSTR lpszAppID);
	void AppendKnownCategory(UINT uiCategory);

private:
	void DeleteList();
	void BuildList();

private:
	HRESULT AddUserTasks();
	HRESULT AddCustomCategory();

	HRESULT CreateShellLink(LPCWSTR lpszApp, LPCWSTR lpszCommandLine, LPCWSTR lpszTitle, LPCWSTR lpszIconPath, int nIconIndex, IShellLinkW** ppSL);
	BOOL IsItemInArray(IShellItem* pSI, IObjectArray* pArray);

private:
	ICustomDestinationList* m_pcdl;
	IObjectArray* m_poaRemoved;
	CString m_sAppID;
	UINT m_uiCategory;

	friend class CXTPTaskbarManager;

	CArray<CTask, CTask&> m_arrTask;

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
public:
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPTaskbarManagerJumpList);

//}}AFX_CODEJOCK_PRIVATE
#endif
};

AFX_INLINE void CXTPTaskbarManagerJumpList::SetAppID(LPCWSTR lpszAppID) {
	m_sAppID = lpszAppID;
}
AFX_INLINE void CXTPTaskbarManagerJumpList::AppendKnownCategory(UINT uiCategory) {
	m_uiCategory = uiCategory;
}

class _XTP_EXT_CLASS CXTPTaskbarManager
{

// Constructor
public:
	CXTPTaskbarManager();
	virtual ~CXTPTaskbarManager();

public:

	void SetProgressBarRange(HWND hWndParent, int nRangeMin, int nRangeMax);
	void SetProgressBarPosition(HWND hWndParent, int nProgressPos);
	void SetProgressBarState(HWND hWndParent, int nFlags);

	BOOL SetOverlayIcon(HWND hWndParent, HICON hIcon, LPCTSTR lpszDescription);
	BOOL SetOverlayIcon(HWND hWndParent, UINT nIDResource, LPCTSTR lpszDescription);

public:
	CXTPTaskbarManagerJumpList* GetJumpList() const;

// Implementation
protected:

	int m_nProgressBarRangeMax;
	int m_nProgressBarRangeMin;

	ITaskbarList3* m_pTaskbarList3;

	CXTPTaskbarManagerJumpList* m_pJumpList;
};

AFX_INLINE CXTPTaskbarManagerJumpList* CXTPTaskbarManager::GetJumpList() const {
	return m_pJumpList;
}

#endif // #if !defined(__XTPTASKBARMANAGER_H__)
