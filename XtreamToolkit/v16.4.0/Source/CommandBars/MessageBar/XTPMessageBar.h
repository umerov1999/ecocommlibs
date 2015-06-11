// XTPMessageBar.h : interface for the CXTPMessageBar class.
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
#if !defined(__XTPMESSAGEBAR_H__)
#define __XTPMESSAGEBAR_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define XTP_IDW_MESSAGE_BAR 0xE806

class CXTPCommandBars;
class CXTPMarkupUIElement;
class CXTPMarkupContext;
class CXTPMessageBar;
class CXTPPaintManager;
class CXTPMessageBarPaintManager;

//===========================================================================
// Summary:
//     CXTPMessageBarButton is a standalone class. It represents single button of message bar
//===========================================================================
class _XTP_EXT_CLASS CXTPMessageBarButton
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPMessageBarButton object
	//-----------------------------------------------------------------------
	CXTPMessageBarButton();
	~CXTPMessageBarButton();

public:

	//-----------------------------------------------------------------------
	// Input:   pBar - Reference to the message bar the button belongs to.
	//          pt - Poiont in which to simulate a click.
	// Summary: Generates a click event for the button, simulating a click by a user.
	//-----------------------------------------------------------------------
	void PerformClick(CXTPMessageBar* pBar, CPoint pt);

public:
	CRect m_rcButton;       // Bounding rectangle for the button
	CString m_strCaption;   // Text displayed in the button
	CString m_strToolTip;   // Tooltip text displayed for the button
	BOOL m_bPressed;        // TRUE if the button is currently pressed
	BOOL m_bHot;            // TRUE if the button is in a "hot" state, meaning the mouse cursor is hovered over the button
	UINT m_nID;             // Id of the button
	CXTPMarkupUIElement* m_pUIElement;  // Store pointer to CXTPMarkupUIElement object.
};

//===========================================================================
// Summary:
//     CXTPMessageBar is a CControlBar derived class. It can be used to show some information in top of Frame
//===========================================================================
class _XTP_EXT_CLASS  CXTPMessageBar : public CControlBar
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPMessageBar object.
	//-----------------------------------------------------------------------
	CXTPMessageBar();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPMessageBar object, handles cleanup and
	//     deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPMessageBar();

public:

	//-----------------------------------------------------------------------
	// Input:   pCommandBars - Parent CommandBar control to attache the
	//                         MessageBar.
	//          dwStyle - Style of the MessageBar.
	//          nID - Id of the MessageBar
	// Summary: Creates a MessageBar.
	// Returns: TRUE if successful, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL Create(CXTPCommandBars* pCommandBars, DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP, UINT nID = XTP_IDW_MESSAGE_BAR);

public:

	//-----------------------------------------------------------------------
	// Input:   lpszMessage - Sting of text or XML Markup to display in the
	//                        MessageBar.
	// Summary: Sets the message displayed in the message bar.
	// Remarks: The message displayed can contain simple text or it can
	//          contain a XML string as in the sample below.  Use XML strings
	//          to add items such as images and formatted text.
	//-----------------------------------------------------------------------
	void SetMessage(LPCTSTR lpszMessage);

	//-----------------------------------------------------------------------
	// Input:   bEnableMarkup - TRUE to enable markup
	// Summary: Call this method to enable markup for the text inside the
	//          MessageBar
	//-----------------------------------------------------------------------
	void EnableMarkup(BOOL bEnableMarkup = TRUE);

	//-----------------------------------------------------------------------
	// Input:   nID - Id of the button.
	//          lpszCaption - Text displayed inside the button.
	//          lpszToolTop - Text displayed as a tooltip when the mouse is placed over the button.
	// Summary: Adds a button to the message bar.
	//-----------------------------------------------------------------------
	void AddButton(UINT nID, LPCTSTR lpszCaption, LPCTSTR lpszToolTop = NULL);

	//-------------------------------------------------------------------------
	// Summary: Removes all buttons from the message bar.
	//-------------------------------------------------------------------------
	void RemoveButtons();

	//-----------------------------------------------------------------------
	// Input:   pButton - Reference to the button to click.
	// Summary: Generates a click event for the button, simulating a click by a user.
	//-----------------------------------------------------------------------
	virtual void Click(CXTPMessageBarButton* pButton);

public:

	//-----------------------------------------------------------------------
	// Summary: Call this member to get the bounding rectangle of the
	//          MessageBox.
	// Returns: The bounding rectangle of the MessageBox.
	//-----------------------------------------------------------------------
	CRect GetMessageRect();

	//-----------------------------------------------------------------------
	// Summary: This member function obtains a pointer to the associated
	//          paint manager.
	// Returns: A CXTPPaintManager pointer to the associated paint manager
	//          object.
	// Remarks: Call this member function to obtain the paint manager object
	//          used for drawing a MessageBar.
	// ----------------------------------------------------------------------
	CXTPPaintManager* GetPaintManager() const;

	CXTPMessageBarPaintManager* GetMessageBarPaintManager() const;

	//-----------------------------------------------------------------------
	// Input:   point - POINT structure that contains the point to be hit
	//                  test, in client coordinates.
	// Summary: Tests a point to determine which messagebar button, if any,
	//          is at the specified point.
	// Returns: If the hit test is successful, contains a reference to the
	//          CXTPMessageBarButton at the hit test point, if any, or - 1 otherwise.
	//-----------------------------------------------------------------------
	CXTPMessageBarButton* HitTestButton(CPoint point) const;

	//-----------------------------------------------------------------------
	// Input:   nID - Button's Id.
	// Summary: Call this member to find the specified button.
	// Returns: The CXTPMessageBarButton object if successful; otherwise NULL.
	//-----------------------------------------------------------------------
	CXTPMessageBarButton* FindButton(UINT nID) const;

	//-----------------------------------------------------------------------
	// Summary: Returns markup context
	// Returns: Returns markup context
	//-----------------------------------------------------------------------
	CXTPMarkupContext* GetMarkupContext() const;

public:

	//-----------------------------------------------------------------------
	// Summary: This member function sets the default height of the message bar.
	//          If auto-sizing is enabled this value will be used as the minimum
	//          height of the message bar.
	// ----------------------------------------------------------------------
	void SetHeight(int nHeight);

	//-----------------------------------------------------------------------
	// Summary: This member function enables auto-sizing of the message bar.
	//          If auto-sizing is enabled, the height of the message bar will
	//          be adjusted to the content.
	// ----------------------------------------------------------------------
	void SetAutoSize(BOOL bAutoSize);

protected:

	//-----------------------------------------------------------------------
	// Input:   pDC - Pointer to a valid device context.
	// Summary: Draws the message bar.
	//-----------------------------------------------------------------------
	virtual void FillMessageBar(CDC* pDC);

	//-----------------------------------------------------------------------
	// Input:   pDC - Pointer to a valid device context.
	// Summary: Gets the size of the content in the message bar.
	// Returns: Returns the size of the content.
	//-----------------------------------------------------------------------
	virtual CSize MeasureContent(CDC* pDC);

	//-----------------------------------------------------------------------
	// Input:   pDC - Pointer to a valid device context.
	//          pButton - Button to measure.
	// Summary: Gets the size of a button.
	// Returns: Returns the size of a button.
	//-----------------------------------------------------------------------
	virtual CSize MeasureButton(CDC* pDC, CXTPMessageBarButton* pButton);

	//-----------------------------------------------------------------------
	// Input:   pDC - Pointer to a valid device context.
	// Summary: Draws the content of the message bar.
	//-----------------------------------------------------------------------
	virtual void DrawContent(CDC* pDC);

	//-----------------------------------------------------------------------
	// Input:   pDC - Pointer to a valid device context.
	// Summary: Call this member function to recalculate the layout of the
	//          message bar.
	//-----------------------------------------------------------------------
	virtual void RecalcLayout(CDC* pDC);

	//-----------------------------------------------------------------------
	// Input:   pDC - Pointer to a valid device context.
	//          pButton - Button to draw.
	// Summary: Call this member to draw a button in the message bar.
	//-----------------------------------------------------------------------
	virtual void DrawButton(CDC* pDC, CXTPMessageBarButton* pButton);

	//-----------------------------------------------------------------------
	// Input:   pDC - Pointer to a valid device context.
	// Summary: Call this member to draw the buttons in the message bar.
	//-----------------------------------------------------------------------
	void DrawButtons(CDC* pDC);

protected:
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPMessageBar)

	virtual CSize CalcFixedLayout(BOOL, BOOL bHorz);
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;

	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPMessageBar)

	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();

	//}}AFX_MSG

protected:

	int m_nHeight;                      // Height of the message bar
	BOOL m_bAutoSize;                   // If TRUE the height of the bar will be adjusted to the content

	CString m_strContent;               // Content string of the message bar.
	CXTPMarkupUIElement* m_pUIElement;  // Store pointer to CXTPMarkupUIElement object.
	CXTPMarkupContext* m_pMarkupContext;// Store pointer to CXTPMarkupContext object.
	CXTPCommandBars* m_pCommandBars;    // Store pointer to CXTPCommandBars object.

	CXTPMessageBarButton* m_pHotButton; // Current hot button.
	CArray<CXTPMessageBarButton*, CXTPMessageBarButton*> m_arrButtons;  // Collection of buttons in the message bar.

	CRect m_rcPadding;          // Padding around the content of the message bar.
	CRect m_rcContent;          // Bounding rectangle of the content in the message bar.


#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
	void OnFinalRelease ();

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPMessageBar);

	afx_msg BOOL OleGetVisible();
	afx_msg void OleSetVisible(BOOL bNewValue);
	afx_msg BSTR OleGetMessage();
	afx_msg void OleSetMessage(LPCTSTR lpszMessage);
	afx_msg void OleAddButton(long Id, LPCTSTR lpszCaption, LPCTSTR lpszToolTip);
	afx_msg void OleAddCloseButton(LPCTSTR lpszToolTip);

	afx_msg int OleGetHeight();
	afx_msg void OleSetHeight(int nHeight);

//}}AFX_CODEJOCK_PRIVATE
#endif
};

/////////////////////////////////////////////////////////////////////////////

AFX_INLINE CXTPMarkupContext* CXTPMessageBar::GetMarkupContext() const {
	return m_pMarkupContext;
}

#endif // !defined(__XTPMESSAGEBAR_H__)
