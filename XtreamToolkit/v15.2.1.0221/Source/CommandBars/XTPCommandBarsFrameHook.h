// XTPCommandBarsFrameHook.h: interface for the CXTPCommandBarsFrameHook class.
//
// This file is a part of the XTREME RIBBON MFC class library.
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
#if !defined(__XTPCOMMANDBARSFRAMEHOOK_H__)
#define __XTPCOMMANDBARSFRAMEHOOK_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CXTPRibbonBar;
class CXTPResourceTheme;
class CXTPControls;
class CXTPFramePaintManager;
typedef DWORD XTP_NOTIFY_CODE;
class CXTPNotifySink;

//===========================================================================
// Summary:
//     CXTPCommandBarsFrameHook is CXTPHookManagerHookAble derived class is helper
//     for Office 2007 window skinning
//===========================================================================
class _XTP_EXT_CLASS CXTPCommandBarsFrameHook : public CXTPHookManagerHookAble
{
	class CControlCaptionButton;

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPCommandBarsFrameHook object
	//-----------------------------------------------------------------------
	CXTPCommandBarsFrameHook();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPCommandBarsFrameHook object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	~CXTPCommandBarsFrameHook();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to enable Office 2007 frame skinning
	// Parameters:
	//     pCommandBars - Parent commandbars object
	//-----------------------------------------------------------------------
	void EnableOffice2007Frame(CXTPCommandBars* pCommandBars);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to enable Office 2007 frame skinning
	// Parameters:
	//     pSite - Window to skin
	//     pPaintManager - Paint manager used to skin window
	//-----------------------------------------------------------------------
	void EnableOffice2007Frame(CWnd* pSite, CXTPPaintManager* pPaintManager);
	void EnableOffice2007Frame(CWnd* pSite, CXTPCommandBars* pCommandBars);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to disable Office 2007 frame skinning
	//-----------------------------------------------------------------------
	void DisableOffice2007Frame();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns Skinned window
	//-----------------------------------------------------------------------
	CWnd* GetSite() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns Paint Manager used to skin window
	//-----------------------------------------------------------------------
	CXTPPaintManager* GetPaintManager() const;

	//-----------------------------------------------------------------------
	// Summary: Call this method to retrieve the paint manager of the frame.
	// Returns: A pointer to a CXTPPaintManager object.
	//-----------------------------------------------------------------------
	CXTPFramePaintManager* GetFramePaintManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if caption of skinned window is visible
	//-----------------------------------------------------------------------
	BOOL IsCaptionVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if Skinned window is active
	//-----------------------------------------------------------------------
	BOOL IsFrameActive() const;

	//-------------------------------------------------------------------------
	// Summary:
	//      Recalculates borders and region of frame
	//-------------------------------------------------------------------------
	void RecalcFrameLayout();

	//-------------------------------------------------------------------------
	// Summary:
	//      Recalculates borders and region of frame with small delay
	//-------------------------------------------------------------------------
	void DelayRecalcFrameLayout();

	//-------------------------------------------------------------------------
	// Summary:
	//      Redraw ribbon bar if found
	// See Also: RedrawFrame
	//-------------------------------------------------------------------------
	void RedrawRibbonBar();

	//-------------------------------------------------------------------------
	// Summary:
	//     Redraws frame of skinned window
	// See Also: RedrawRibbonBar
	//-------------------------------------------------------------------------
	void RedrawFrame();

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if frame has skinned scrollbar to draw rounded rects.
	// Parameters:
	//      pnStatusHeight - Height of status bar to return.
	// Returns: TRUE if frame has skinned status bar.
	// See Also: IsMDIMaximized
	//-----------------------------------------------------------------------
	BOOL IsFrameHasStatusBar(int* pnStatusHeight = NULL) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if frame is maximized child window.
	// Returns: TRUE if frame is maximized child window.
	//-----------------------------------------------------------------------
	BOOL IsMDIMaximized() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns collection of caption buttons.
	// Returns: Collection of caption buttons.
	//-----------------------------------------------------------------------
	CXTPControls* GetCaptionButtons() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Calculates height of caption
	// Returns:
	//     Height of caption in pixels
	// See Also: GetFrameBorder
	//-----------------------------------------------------------------------
	int GetCaptionHeight() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Calculates size of frame borders
	// Returns: Size of frame borders  in pixels
	// See Also: GetCaptionHeight
	//-----------------------------------------------------------------------
	int GetFrameBorder() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if Vista AERO theme is enabled
	// Returns:
	//     TRUE if Vista AERO theme is enabled
	//-----------------------------------------------------------------------
	BOOL IsDwmEnabled() const;

	//-----------------------------------------------------------------------
	// Summary: Returns style of site window
	// Input:   bExStyle - TRUE to return ExStyle of window
	// Returns: Style of Extended Style of site window.
	//-----------------------------------------------------------------------
	DWORD GetSiteStyle(BOOL bExStyle = FALSE) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the ribbon frame if EnableFrameTheme was not called.
	// Parameters:
	//     pDC -  Pointer to a valid device context
	//     pRibbonBar - Ribbon Bar to draw
	//-----------------------------------------------------------------------
	void DrawRibbonFramePart(CDC* pDC);


	void UpdateDwmClientArea();
protected:

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
	virtual int OnHookMessage(HWND hWnd, UINT nMessage, WPARAM& wParam, LPARAM& lParam, LRESULT& lResult);

protected:

//{{AFX_CODEJOCK_PRIVATE
	void TrackCaptionButton(CXTPControl* pSelected);
	void UpdateFrameRegion();
	void UpdateFrameRegion(CSize szFrameRegion, BOOL bUpdate = FALSE);
	void RepositionCaptionButtons();
	void AddCaptionButton(int nID, int nHit);
	void ScreenToFrame(LPPOINT lpPoint);
	CXTPControl* HitTestCaptionButton(CPoint point);
	void RefreshFrameStyle();
	void SetWinHook();
	BOOL GetAutoHideBar() const;
	void OnResourceImagesChanged(XTP_NOTIFY_CODE Event, WPARAM wParam, LPARAM lParam);

//}}AFX_CODEJOCK_PRIVATE

public:
	static UINT m_nMsgQueryFrameHook;   // Helper message to determine if Hook was attached.
	BOOL m_bLockNCPaint;                // TRUE to disable repaint of non client area

	static BOOL m_bAllowDwm;

protected:
	HWND m_hwndSite;                    // Skinned window handle
	CXTPRibbonBar* m_pRibbonBar;        // Child Ribbonbar object
	CXTPCommandBars* m_pCommandBars;    // Attached commandbars object
	CXTPPaintManager* m_pPaintManager;  // PaintManager used to draw frame
	BOOL m_bActive;                     // TRUE if frame is active
	CSize m_szFrameRegion;              // Previous size of window
	BOOL m_bDelayReclalcLayout;         // TRUE to recalculate layout after small delay
	CXTPControls* m_pCaptionButtons;    // Caption buttons
	CXTPControl* m_pSelected;           // Selected caption button
	BOOL m_bButtonPressed;              // Pressed caption button
	BOOL m_bThemeFrameEnabled;          // TRUE if skinning is enabled
	BOOL m_nSkipNCPaint;                // TRUE to skip frame drawing
	BOOL m_bInUpdateFrame;              // TRUE if UpdateFrame method is executed
	int m_nFrameBorder;                 // Size of frame borders
	BOOL m_bDwmEnabled;                 // TRUE if Vista AERO theme is enabled

	BOOL m_bToolBarComposition;

private:
	UINT m_nMsgUpdateSkinState;
	UINT m_nMsgQuerySkinState;

	CXTPNotifySink* m_pSink;

private:
	class CWinEventHook;
	friend class CXTPRibbonBar;
	friend class CControlCaptionButton;
	friend class CXTPCommandBars;
};

// Obsolete name
#define CXTPOffice2007FrameHook CXTPCommandBarsFrameHook

AFX_INLINE CWnd* CXTPCommandBarsFrameHook::GetSite() const {
	return CWnd::FromHandle(m_hwndSite);
}
AFX_INLINE BOOL CXTPCommandBarsFrameHook::IsFrameActive() const {
	return m_bActive;
}
AFX_INLINE CXTPControls* CXTPCommandBarsFrameHook::GetCaptionButtons() const {
	return m_pCaptionButtons;
}
AFX_INLINE int CXTPCommandBarsFrameHook::GetFrameBorder() const {
	return m_nFrameBorder;
}
AFX_INLINE BOOL CXTPCommandBarsFrameHook::IsDwmEnabled() const {
	return m_bDwmEnabled;
}

#endif // !defined(__XTPCOMMANDBARSFRAMEHOOK_H__)
