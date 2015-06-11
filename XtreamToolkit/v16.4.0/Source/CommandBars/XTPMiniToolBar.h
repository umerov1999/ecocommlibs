// XTPMiniToolBar.h : interface for the CXTPMiniToolBar class.
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

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPMINITOOLBAR_H__)
#define __XTPMINITOOLBAR_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////////////////////////
// CXTPMiniToolBar window

//===========================================================================
// Summary: Represents a MiniToolbar object.
// Remarks: MiniToolBar is a Office 2007 style context toolbar.  It has many
// uses, for example, you might want to display a MiniToolbar presenting some
// of the more commonly used commands that a user would probably want to use
// in a given situation like selecting text.  For example, this can save the
// user allot of time by making font related tasks available without fumbling
// through tabs or menus.  So to effectively use a MiniToolbar you would
// place all commands the user would most probably want to use when
// performing a given action.
//===========================================================================
class _XTP_EXT_CLASS CXTPMiniToolBar : public CXTPPopupToolBar
{
	DECLARE_XTP_COMMANDBAR(CXTPMiniToolBar);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//      Call this method to create CXTPMiniToolBar class
	// Input:
	//      pCommandBars - Pointer to owner CXTPCommandBars
	// Returns: Pointer to new CXTPMiniToolBar object
	//-----------------------------------------------------------------------
	static CXTPMiniToolBar* AFX_CDECL CreateMiniToolBar(CXTPCommandBars* pCommandBars);

protected:

	//-------------------------------------------------------------------------
	// Summary: Constructs a CXTPMiniToolBar object.
	//-------------------------------------------------------------------------
	CXTPMiniToolBar();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPMiniToolBar object, handles cleanup and
	//     deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPMiniToolBar();

public:

	//-----------------------------------------------------------------------
	// Summary: Call this method to show minibar in specific position
	// Input:   nFlags - Reserved, currently not used.
	//          xPos - Specifies the horizontal position in screen coordinates
	//              of the minibar.
	//          yPos - Specifies the vertical position in screen coordinates
	//              of the top of the minibar.
	// Returns: TRUE if successful; otherwise returns FALSE
	//-----------------------------------------------------------------------
	virtual BOOL TrackMiniBar(UINT nFlags, int xPos, int yPos);

	//-----------------------------------------------------------------------
	// Input:   pPopupMenu - Points to a CXTPPopupBar object.
	//          nFlags - Reserved, currently not used.
	//          xPos - Specifies the horizontal position in screen coordinates
	//              of the popup menu.
	//          yPos - Specifies the vertical position in screen coordinates
	//              of the top of the menu on the screen.
	// Summary: Call this member to display a floating pop-up menu at
	//          the specified location.
	// Returns: TRUE if successful; otherwise returns FALSE
	//-----------------------------------------------------------------------
	virtual BOOL TrackPopupMenu(CXTPPopupBar* pPopupMenu, UINT nFlags, int xPos, int yPos);

	//-----------------------------------------------------------------------
	// Input:   nChar - Specifies the virtual key code of the given key.
	//          lParam - Specifies additional message-dependent information.
	// Summary: The framework calls this member function when a non-system
	//          key is pressed.
	// Returns: TRUE if key handled, otherwise returns FALSE
	//-----------------------------------------------------------------------
	virtual BOOL OnHookKeyDown(UINT nChar, LPARAM lParam);

	//-----------------------------------------------------------------------
	// Summary: Call this method to enable/disable keyboard interaction with custom control (xtpControlCustom)
	// disabled by default (the minitoolbar closes on keyboard events in the custom control)
	void SetCustomControlKeyboardInteraction(BOOL bInteract = TRUE);

	BOOL IsCustomControlKeyboardInteraction() const;

protected:

	//-------------------------------------------------------------------------
	// Summary: Translates all messages in message queue.
	//-------------------------------------------------------------------------
	virtual void PumpMessage();

	//-------------------------------------------------------------------------
	// Summary: Updates the opacity of the mini toolbar and popup menu.
	//-------------------------------------------------------------------------
	void UpdateOpacity();

	//-----------------------------------------------------------------------
	// Summary: Specifies whether the mouse cursor is hovered over the mini
	//          toolbar.
	// Returns: TRUE if the cursor is over the mini toolbar, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL CursorInWindow() const;


//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPMiniToolBar)

	virtual void Animate();
	virtual BOOL SetTrackingMode(int bMode, BOOL bSelectFirst = TRUE, BOOL bKeyboard = FALSE);
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	virtual int OnHookMessage(HWND hWnd, UINT nMessage, WPARAM& wParam, LPARAM& lParam, LRESULT& lResult);

	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPMiniToolBar)
	//}}AFX_MSG

//}}AFX_CODEJOCK_PRIVATE

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_OLETYPELIB_EX(CXTPMiniToolBar);
	DECLARE_INTERFACE_MAP()

	long OlePopupMiniToolBar(const VARIANT& varFlags, const VARIANT& x, const VARIANT& y);
	long OlePopupContextMenu(LPDISPATCH lpDispatch, const VARIANT& varFlags, const VARIANT& x, const VARIANT& y);

//}}AFX_CODEJOCK_PRIVATE
#endif

protected:

	//-------------------------------------------------------------------------
	// Summary:
	//      UpdateLayeredWindow declaration.
	// Input:
	//      LPFNUPDATELAYEREDWINDOW - Struct that contains the position, size, shape, content, and translucency of a layered window.
	// Remarks:
	//      Required for transparent Windows.  But they are not present in the VC6 headers. They are only present
	//      on Win2k or later.
	// Returns:
	//      TRUE if successful; otherwise returns FALSE
	//-------------------------------------------------------------------------
	typedef BOOL(WINAPI* LPFNUPDATELAYEREDWINDOW) (HWND hwnd, HDC hdcDst, POINT *pptDst, SIZE *psize, HDC hdcSrc, POINT *pptSrc, COLORREF crKey, BLENDFUNCTION *pblend, DWORD dwFlags);

	//-------------------------------------------------------------------------
	// Summary:
	//      SetLayeredWindowAttributes declaration.
	// Input:
	//      PFNSETLAYEREDWINDOWATTRIBUTES - Struct that contains the opacity and transparency color key of a layered window.
	// Remarks:
	//      Required for transparent Windows.  But they are not present in the VC6 headers. They are only present
	//      on Win2k or later.
	// Returns:
	//      TRUE if successful; otherwise returns FALSE
	//-------------------------------------------------------------------------
	typedef BOOL (WINAPI *PFNSETLAYEREDWINDOWATTRIBUTES) (HWND hwnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags);

	PFNSETLAYEREDWINDOWATTRIBUTES m_pfnSetLayeredWindowAttributes;  // Point to Transparency proc in USER32.dll module
	LPFNUPDATELAYEREDWINDOW m_pfnUpdateLayeredWindow;               // Point to UpdateLayeredWindow proc in USER32.dll module

	BOOL m_bActivated;          // TRUE if the mini toolbar is activated.
	int m_nOpacity;         // Opacity level.
	BOOL m_bTracking;           // TRUE if the command bar is in tracking mode.
	CXTPPopupBar* m_pContextMenu;           // Context menu that is displayed in customization mode when a user right-clicks on a control.
	BOOL m_bCustomControlKeyboardInteraction;
};

#endif // !defined(__XTPMINITOOLBAR_H__)
