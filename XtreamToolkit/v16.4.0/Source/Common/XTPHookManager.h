// XTPHookManager.h : interface for the CXTPHookManager class.
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
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
#if !defined(__XTPHOOKMANAGER_H__)
#define __XTPHOOKMANAGER_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//===========================================================================
// Summary:
//     HookAble interface.
// See Also: CXTPHookManager, CXTPHookManager::SetHook
//===========================================================================
class _XTP_EXT_CLASS CXTPHookManagerHookAble
{
public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPHookManagerHookAble object
	//-------------------------------------------------------------------------
	CXTPHookManagerHookAble();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPHookManagerHookAble object, handles clean up and deallocation
	//-------------------------------------------------------------------------
	virtual ~CXTPHookManagerHookAble();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by WindowProc, or is called during message reflection.
	// Parameters:
	//     hWnd - Window handle that the message belongs to.
	//     nMessage - Specifies the message to be sent.
	//     wParam - Specifies additional message-dependent information.
	//     lParam - Specifies additional message-dependent information.
	//     lResult - The return value of WindowProc. Depends on the message; may be NULL.
	// Returns:
	//     TRUE if message was processed.
	//-----------------------------------------------------------------------
	virtual int OnHookMessage(HWND hWnd, UINT nMessage, WPARAM& wParam, LPARAM& lParam, LRESULT& lResult) = 0;

public:
	BOOL m_bAutoDestroy;            // TRUE to automatically delete hook
};


//===========================================================================
// Summary:
//     CXTPHookManager is standalone class. It is used to hook a CWnd object
//     in order to intercept and act upon window messages that are received.
// Remarks:
//     To access CXTPHookManager methods use XTPHookManager function
// See Also:
//     XTPHookManager, CXTPHookManagerHookAble
//===========================================================================
class _XTP_EXT_CLASS CXTPHookManager
{
private:
	class CHookSink;

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPHookManager object.
	//-----------------------------------------------------------------------
	CXTPHookManager();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPHookManager object, handles cleanup and de-
	//     allocation.
	//-----------------------------------------------------------------------
	~CXTPHookManager();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will hook a window so that its messages are
	//     intercepted before they are passed on to the specified window.
	// Parameters:
	//     hWnd  - A handle to a window that represents that represents the window to hook.
	//     pHook - Hookable class that will receive messages
	//-----------------------------------------------------------------------
	void SetHook(HWND hWnd, CXTPHookManagerHookAble* pHook);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes a hook associated with a window.
	// Parameters:
	//     hWnd  - A handle to a window that hooks need to remove
	//     pHook - Hookable class that hooks need to remove
	//-----------------------------------------------------------------------
	void RemoveHook(HWND hWnd, CXTPHookManagerHookAble* pHook);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes all hooks associated with a window.
	// Parameters:
	//     hWnd  - A handle to a window that hooks need to remove
	//-----------------------------------------------------------------------
	void RemoveAll(HWND hWnd);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes all hooks associated with a window.
	// Parameters:
	//     pHook - Hookable class that hooks need to remove
	//-----------------------------------------------------------------------
	void RemoveAll(CXTPHookManagerHookAble* pHook);

	//-----------------------------------------------------------------------
	// Summary:
	//     Searches collection of Hookable interfaces that receive hooks of specified window.
	// Parameters:
	//     hWnd  - A handle to a window need to test
	// Returns:
	//     CHookSink pointer if found; otherwise returns NULL;
	//-----------------------------------------------------------------------
	CHookSink* Lookup(HWND hWnd);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//      Calls the default window procedure.
	//-----------------------------------------------------------------------
	LRESULT Default();

	//-----------------------------------------------------------------------
	// Summary:
	//      Calls the default window procedure.
	// Parameters:
	//      wParam - [in] Specifies additional message information. The content of this parameter depends on the value of the Msg parameter.
	//      lParam - [in] Specifies additional message information. The content of this parameter depends on the value of the Msg parameter.
	//-----------------------------------------------------------------------
	LRESULT Default(WPARAM wParam, LPARAM lParam);

	//-----------------------------------------------------------------------
	// Summary:
	//      Calls the default window procedure.
	// Parameters:
	//      hWnd -  [in] handle to HWND
	//      message - [in] message id
	//      wParam - [in] Specifies additional message information. The content of this parameter depends on the value of the Msg parameter.
	//      lParam - [in] Specifies additional message information. The content of this parameter depends on the value of the Msg parameter.
	//-----------------------------------------------------------------------
	LRESULT Default(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	static LRESULT CALLBACK HookWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	static UINT m_nMsgSinkRemoved;

private:
	void RemoveAll();
	void RemoveAll(HWND hWnd, BOOL bLastMessage);
	CMap<HWND, HWND, LPVOID, LPVOID> m_mapHooks;

	friend _XTP_EXT_CLASS CXTPHookManager* AFX_CDECL XTPHookManager();
	friend class CHookSink;
};


//---------------------------------------------------------------------------
// Summary:
//     Call this function to access CXTPHookManager members.
//     Since this class is designed as a single instance object you can
//     only access version info through this method. You <b>cannot</b>
//     directly instantiate an object of type CXTPHookManager.
// Example:
//     <code>XTPHookManager()->SetHook(hWnd, this);</code>
//---------------------------------------------------------------------------
_XTP_EXT_CLASS CXTPHookManager* AFX_CDECL XTPHookManager();




//-------------------------------------------------------------------------
// Summary:
//     Shadow options of selected paint manager
// See Also: CXTPPaintManager::GetShadowOptions()
//-------------------------------------------------------------------------
enum XTPShadowOptions
{
	xtpShadowOfficeAlpha = 1,           // Office alpha shadow
	xtpShadowShowPopupControl = 2       // Draw shadow for popup controls
};

//===========================================================================
// Summary:
//     CXTPShadowManager is standalone class used to manage CommandBars' shadows.
//===========================================================================
class _XTP_EXT_CLASS CXTPShadowManager : public CXTPCmdTarget
{
private:
	typedef BOOL(WINAPI* LPFNUPDATELAYEREDWINDOW) (HWND hwnd, HDC hdcDst, POINT *pptDst, SIZE *psize, HDC hdcSrc, POINT *pptSrc, COLORREF crKey, BLENDFUNCTION *pblend, DWORD dwFlags);

private:
	class CShadowWnd;
	class CShadowList;

public:
	//===========================================================================
	// Summary:
	//     Constructs a CXTPShadowManager object.
	//===========================================================================
	CXTPShadowManager();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPShadowManager object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	~CXTPShadowManager();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Check the system alpha shadow ability.
	// Returns:
	//     TRUE if alpha shadow available; otherwise returns FALSE
	//-----------------------------------------------------------------------
	BOOL IsAlphaShadow();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the command bar  shadow.
	// Parameters:
	//     rcWindow - Bounding rectangle of parent window
	//     pShadowOwner - Points to a parent windows
	//     rcExclude - Excluded rectangle.
	//     nLevel - Recursive level of the parent window
	//-----------------------------------------------------------------------
	void SetShadow(CRect rcWindow, CWnd* pShadowOwner);
	void SetShadow(CWnd* pShadowOwner, const CRect& rcExclude = CRect(0, 0, 0, 0), int nLevel = 0);  // <combine CXTPShadowManager::SetShadow@CRect@CWnd*>

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes shadows for the command bar.
	// Parameters:
	//     pShadowOwner - Points to a CXTPCommandBar object
	//-----------------------------------------------------------------------
	void RemoveShadow(CWnd* pShadowOwner);
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to move shadow to specified offset
	// Parameters:
	//     pShadowOwner - Shadow to move
	//     szOffset - Window move offset
	//-----------------------------------------------------------------------
	void OffsetShadow(CWnd* pShadowOwner, CSize szOffset);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method used to enumerate all shadows of specific parent window
	// Parameters:
	//     pShadowOwner - Point to parent owner of shadows
	// Returns:
	//     Position to first element in list of shadows. Use GetNext method to retrieve it.
	//-----------------------------------------------------------------------
	POSITION GetHeadPosition(CWnd* pShadowOwner) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method used  to enumerate all shadows of specific parent window. Call GetHeadPosition to get position of first shadow
	// Parameters:
	//     pos - Current position of shadow in the list
	// Returns:
	//     Pointer to Shadow corresponded to pos position.
	//-----------------------------------------------------------------------
	CWnd* GetNext(POSITION& pos) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns combination of XTPShadowOptions flags for shadows
	// Returns:
	//     Combination of XTPShadowOptions flags
	//-----------------------------------------------------------------------
	int GetShadowOptions() const;
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set shadow options.
	// Parameters:
	//     nOptions - Combination of XTPShadowOptions flags to set. It can be following values
	//                 * <b>xtpShadowOfficeAlpha</b> -  Office alpha shadow
	//                 * <b>xtpShadowShowPopupControl</b> - Draw shadow for popup controls
	//-----------------------------------------------------------------------
	void SetShadowOptions(int nOptions);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set specific color for shadows
	// Parameters:
	//     clrShadow - New color to set
	//-----------------------------------------------------------------------
	void SetShadowColor(COLORREF clrShadow);

public:
	BOOL m_bUseSystemSaveBitsStyle; // If this flag is set CS_SAVEBITS style will be used for shadow windows.

private:
	void DestroyShadow(CShadowWnd*);
	CShadowWnd* CreateShadow(BOOL bHoriz, CRect rc, CRect rcExclude, CWnd* pShadowOwner, BOOL bControlPopup, int nLevel);

private:
	LPFNUPDATELAYEREDWINDOW m_pfnUpdateLayeredWindow;
	CShadowList* m_pShadows;
	BOOL m_bAlphaShadow;
	int m_nShadowOptions;
	COLORREF m_clrShadowFactor;

	friend class CShadowWnd;
};


AFX_INLINE int CXTPShadowManager::GetShadowOptions() const {
	return m_nShadowOptions;
}
AFX_INLINE void CXTPShadowManager::SetShadowOptions(int nOptions) {
	m_nShadowOptions = nOptions;
}
AFX_INLINE void CXTPShadowManager::SetShadowColor(COLORREF clrShadow) {
	m_clrShadowFactor = clrShadow;
}



#endif //#if !defined(__XTPHOOKMANAGER_H__)
