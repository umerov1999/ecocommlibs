// XTPKeyboardManager.h : interface for the CXTPKeyboardManager class.
//
// This file is a part of the XTREME COMMANDBARS MFC class library.
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
#if !defined(__XTPKEYBOARDMANAGER_H__)
#define __XTPKEYBOARDMANAGER_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//===========================================================================
// Summary:
//     CXTPKeyboardManager class used to handle keyboard hooks
//===========================================================================
class _XTP_EXT_CLASS CXTPKeyboardManager : public CNoTrackObject
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPKeyboardManager object.
	//-----------------------------------------------------------------------
	CXTPKeyboardManager();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPKeyboardManager object, handles clean up and deallocation
	//-------------------------------------------------------------------------
	~CXTPKeyboardManager();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to start/end keyboard hooks
	// Parameters:
	//     bSetup - TRUE to start keyboard hooks
	// See Also: SetupCallWndProcHook
	//-----------------------------------------------------------------------
	void SetupKeyboardHook(BOOL bSetup = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to start/end message hooks
	// Parameters:
	//     bSetup - TRUE to start message hooks
	// See Also: SetupKeyboardHook
	//-----------------------------------------------------------------------
	void SetupCallWndProcHook(BOOL bSetup = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to hook keyboard notifications
	// Parameters:
	//     pHook - CXTPHookManagerHookAble object that will receive keyboard notifications
	// See Also: UnhookKeyboard
	//-----------------------------------------------------------------------
	void HookKeyboard(CXTPHookManagerHookAble* pHook);

	//-----------------------------------------------------------------------
	// Summary:
	//      Call this method to unhook keyboard notifications
	// Parameters:
	//      pHook - CXTPHookManagerHookAble object to remove
	// See Also: HookKeyboard
	//-----------------------------------------------------------------------
	void UnhookKeyboard(CXTPHookManagerHookAble* pHook);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if keyboard is hooked
	// Returns:
	//     TRUE if keyboard is hooked
	//-----------------------------------------------------------------------
	BOOL IsKeyboardHooked() { return m_lstKeyboardHooks.GetCount() > 0; }

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to process keyboard hook
	// Parameters:
	//     nMessage - Message to process
	//     wParam - Message specified parameter
	//     lParam - Message specified parameter
	// Returns:
	//     TRUE if message was processed
	//-----------------------------------------------------------------------
	BOOL ProcessKeyboardHooks(UINT nMessage, WPARAM wParam, LPARAM lParam = 0);

protected:
//{{AFX_CODEJOCK_PRIVATE
	static LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK CallWndProc(int code, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK GetMessageProc(int code, WPARAM wParam, LPARAM lParam);

//}}AFX_CODEJOCK_PRIVATE

protected:
	HHOOK m_hHookKeyboard;          // Keyboard hook
	HHOOK m_hHookCallWndProc;       // Message hook
	HHOOK m_hHookGetMessage;       // Message hook

#ifdef _AFXDLL
	AFX_MODULE_STATE* m_pModuleState; // Module state
#endif

	static CThreadLocal<CXTPKeyboardManager> _xtpKeyboardThreadState;           // Instance of Keyboard hook
	CList<CXTPHookManagerHookAble*, CXTPHookManagerHookAble*> m_lstKeyboardHooks;   // List of keyboard hooks

private:
	friend CXTPKeyboardManager* XTPKeyboardManager();
	friend class CXTPCommandBars;

};


AFX_INLINE CXTPKeyboardManager* XTPKeyboardManager() {
	return CXTPKeyboardManager::_xtpKeyboardThreadState.GetData();
}


#endif //#if !defined(__XTPKEYBOARDMANAGER_H__)
