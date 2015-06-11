// XTPMarqueeCtrl.h : header file
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
#if !defined(__XTPMARQUEECTRL_H__)
#define __XTPMARQUEECTRL_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPWinThemeWrapper;

// ---------------------------------------------------------------------
// Summary:
//     CXTPMarqueeCtrl is a CProgressCtrl derived class. It is used to
//     create a customizable Marquee style progress control. Using the
//     SetMarquee method you can set speed, gap between marquee chunks,
//     width and transparency for each chunk. See the sample located in
//     the Controls/MarqueeProgress for more details.
// ---------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPMarqueeCtrl : public CProgressCtrl
{
	DECLARE_DYNAMIC(CXTPMarqueeCtrl)

public:
	// -----------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPMarqueeCtrl object.
	// -----------------------------------------------------------------
	CXTPMarqueeCtrl();

	// -----------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPMarqueeCtrl object, handles cleanup and
	//     deallocation.
	// -----------------------------------------------------------------
	virtual ~CXTPMarqueeCtrl();

	// -----------------------------------------------------------------
	// Summary:
	//     This member is called to update color and chunk bitmaps used
	//     by the marquee progress control.
	// -----------------------------------------------------------------
	void RefreshMetrics();

	// -----------------------------------------------------------------
	// Summary:
	//     Turns marquee mode on or off for the current progress bar
	//     control.
	// Parameters:
	//     bMarqueeMode - TRUE to turn marquee mode on, or FALSE to turn
	//                    marquee mode off.
	//     nInterval    - Time in milliseconds between updates of the
	//                    marquee animation.
	//     nGap         - Size in pixels of the gap between each chunk
	//                    displayed in the marquee progress bar.
	//     cx           - Size in pixels of each chunk displayed in the
	//                    marquee progress bar.
	//     piTrans      - Array of 5 integers representing the amount of
	//                    transparency for each chunk displayed in the
	//                    marquee progress bar in percent ranging from 0
	//                    to 100. The method assumes the array contains
	//                    5 integers.
	// Example:
	//    The following code example starts and stops the marquee
	//    scrolling animation.
	// <code>
	// int piAlpha[5] = { 25, 50, 75, 100, 100 };
	// m_wndProgress.SetMarquee(TRUE, 50, 2, 12, piAlpha);
	// </code>
	// -----------------------------------------------------------------
	void SetMarquee(BOOL bMarqueeMode = TRUE, int nInterval = 50, int nGap = 2, int cx = 12, int* piTrans = NULL);

	// -----------------------------------------------------------------
	// Summary:
	//     Determines if the PBS_SMOOTH style has been set for the marquee
	//     progress bar.
	// Returns:
	//     Returns TRUE if the progress bar has PBS_SMOOTH style set,
	//     otherwise false.
	// -----------------------------------------------------------------
	BOOL IsSmooth();

	// -----------------------------------------------------------------
	// Summary:
	//     Determines if the PBS_VERTICAL style has been set for the marquee
	//     progress bar.
	// Returns:
	//     Returns TRUE if the progress bar has PBS_VERTICAL style set,
	//     otherwise false.
	// -----------------------------------------------------------------
	BOOL IsVertical();

	// -----------------------------------------------------------------
	// Summary:
	//     Determines if the PBS_MARQUEE style has been set for the marquee
	//     progress bar.
	// Returns:
	//     Returns TRUE if the progress bar has PBS_MARQUEE style set,
	//     otherwise false.
	// -----------------------------------------------------------------
	BOOL IsMarquee();

	// -----------------------------------------------------------------
	// Summary:
	//    This member function sets the background and progress indicator
	//    color for the progress bar.
	// Parameters:
	//    crBack  - COLORREF value that specifies the new background color.
	//    crChunk - COLORREF value that specifies the new progress indicator
	//              bar color.
	// -----------------------------------------------------------------
	void SetColors(COLORREF crBack, COLORREF crChunk);

protected:

	// -----------------------------------------------------------------
	// Summary:
	//     This member function is called when the progress bar has been
	//     created or subclassed.
	// Returns:
	//     TRUE if the control has been successfully initialized, otherwise
	//     FALSE.
	// -----------------------------------------------------------------
	virtual BOOL OnInitControl();

	// -----------------------------------------------------------------
	// Summary:
	//     This member function is called to determine if Windows themes
	//     are enabled for the calling application.
	// Returns:
	//     TRUE if Windows themes are enabled, otherwise FALSE.
	// -----------------------------------------------------------------
	virtual BOOL UseWinThemes();

	// -----------------------------------------------------------------
	// Summary:
	//     This member function is called to draw the background for the
	//     marquee progress bar.
	// Parameters:
	//     rect :  Size of the background area to draw.
	//     pDC :   Pointer to a valid device context.
	//
	// -----------------------------------------------------------------
	virtual void OnDrawBackground(CDC* pDC, CRect rect);

	// ----------------------------------------------------------------------
	// Summary:
	//     This member function is called to draw the marquee on the progress
	//     bar.
	// Parameters:
	//     rect :  Size of the client area to draw the marquee on.
	//     pDC :   Pointer to a valid device context.
	//
	// ----------------------------------------------------------------------
	virtual void OnDrawMarquee(CDC* pDC, CRect rect);


	//{{AFX_CODEJOCK_PRIVATE
	//{{AFX_VIRTUAL(CXTPMarqueeCtrl)
	virtual void PreSubclassWindow();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL
	//{{AFX_MSG(CXTPMarqueeCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	//}}AFX_CODEJOCK_PRIVATE

protected:

	int                     m_nGap;          // Size in pixels between each chunk in the marquee.
	int                     m_nPos;          // Current position of the marquee animation.
	int                     m_cxChunk;       // Size in pixels of each chunk in the marquee.
	BYTE                    m_chAlpha[5];    // Represents transparency for each chunk in the marquee.
	BOOL                    m_bWinThemed;    // TRUE if Windows themes are used.
	BOOL                    m_bIsSubclassed; // TRUE if the progress bar was subclassed.
	CXTPPaintManagerColor   m_crBack;        // RGB background color for non Windows theme progress.
	CXTPPaintManagerColor   m_crChunk;       // RGB chunk color for each chunk for non Windows theme progress.
	CXTPWinThemeWrapper*    m_themeProgress;  // Used for access to the Windows theme API.
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__XTPMARQUEECTRL_H__)
