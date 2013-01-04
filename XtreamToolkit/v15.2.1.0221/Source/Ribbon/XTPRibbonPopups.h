// XTPRibbonBar.h: interface for the CXTPRibbonBar class.
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
#if !defined(__XTPRIBBONPOPUPS_H__)
#define __XTPRIBBONPOPUPS_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CXTPRibbonQuickAccessControls;
class CXTPRibbonTab;
class CXTPRibbonControlTab;
class CXTPTabPaintManager;
class CXTPRibbonBar;
class CXTPRibbonGroup;
class CXTPCommandBarsFrameHook;
class CXTPRibbonTabContextHeaders;
class CXTPRibbonGroups;


//{{AFX_CODEJOCK_PRIVATE

class CXTPRibbonScrollableBar
{
public:
	CXTPRibbonScrollableBar();
private:
	class CControlGroupsScroll;

protected:
	void InitScrollableBar(CXTPCommandBar* pParent);

	void ShowScrollableRect(CXTPRibbonGroups* pGroups, CRect rc);

public:
	void EnableGroupsScroll(BOOL bScrollLeft, BOOL bScrollRight);
	virtual void OnGroupsScroll(BOOL bScrollLeft) = 0;
	virtual CRect GetGroupsRect() const = 0;
	virtual CXTPRibbonGroup* HitTestGroup(CPoint point) const = 0;

	void CreateGroupKeyboardTips(CXTPRibbonTab* pSelectedTab);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get highlighted group
	// Returns:
	//     CXTPRibbonGroup object under mouse cursor
	// See Also: CXTPRibbonGroup
	//-----------------------------------------------------------------------
	CXTPRibbonGroup* GetHighlightedGroup() const;

	void HighlightGroup(CXTPRibbonGroup* pHighlightedGroup);

protected:
	void CreateControlKeyboardTips(CXTPControl* pControl, LPCTSTR lpszPrefix, BOOL bVisible);

	BOOL ProcessSpecialKey(XTPSpecialKey key);
	CRect _GetBoundRect(CXTPControl* pControl, BOOL bSelected, XTPSpecialKey key);


public:
	int m_nGroupsScrollPos;         // Groups scroll position

protected:
	CXTPCommandBar* m_pParent;
	CXTPControl* m_pControlScrollGroupsLeft;    // Control to draw left scroll
	CXTPControl* m_pControlScrollGroupsRight;   // Control to draw right scroll
	CXTPRibbonGroup* m_pHighlightedGroup;       // Currently highlighted group
};

CXTPRibbonScrollableBar* AFX_CDECL GetScrollableBar(CXTPCommandBar* pCommandBar);

//}}AFX_CODEJOCK_PRIVATE

//===========================================================================
// Summary:
//     Implements popup when ribbon is in minimzed mode.
//===========================================================================
class _XTP_EXT_CLASS CXTPRibbonTabPopupToolBar : public CXTPPopupToolBar, public CXTPRibbonScrollableBar
{
	DECLARE_DYNAMIC(CXTPRibbonTabPopupToolBar)
public:

	// ----------------------------------------------------------
	// Summary:
	//     Constructs a CXTPRibbonTabPopupToolBar object
	// Parameters:
	//     pRibbonTab :  Ribbon tab the popup toolbar belongs to.
	// ----------------------------------------------------------
	CXTPRibbonTabPopupToolBar(CXTPRibbonTab* pRibbonTab);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPRibbonTabPopupToolBar object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	~CXTPRibbonTabPopupToolBar();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves Groups bounding rectangle
	// Returns:
	//     Groups bounding rectangle
	//-----------------------------------------------------------------------
	CRect GetGroupsRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     The framework calls this member function when the group popup is scrolled
	// Parameters:
	//     bScrollLeft - TRUE when scrolling left, FALSE when scrolling right
	//-----------------------------------------------------------------------
	void OnGroupsScroll(BOOL bScrollLeft);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to be sure control is visible on ribbon bar
	// Parameters:
	//     pControl - CXTPControl child that need to check
	//-----------------------------------------------------------------------
	void EnsureVisible(CXTPControl* pControl);

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called to create keyboard tips collection for the ribbon
	//-------------------------------------------------------------------------
	void CreateKeyboardTips();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to get the size of the command bar.
	// Parameters:
	//      nLength - The requested dimension of the control bar, either horizontal or vertical, depending on dwMode.
	//      nMode - see CControlBar::CalcDynamicLayout for list of supported flags.
	// Returns:
	//     Size of the command bar.
	//-----------------------------------------------------------------------
	CSize CalcDynamicLayout(int nLength, DWORD nMode);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the command bar in the given context.
	// Parameters:
	//     pDC - Pointer to a valid device context
	//     rcClipBox - The rectangular area of the control that is invalid
	//-----------------------------------------------------------------------
	void DrawCommandBar(CDC* pDC, CRect rcClipBox);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the size of the command bar button.
	// Returns:
	//     The width and height values of the command bar button.
	//-----------------------------------------------------------------------
	virtual CSize GetButtonSize() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the size of the icon for a minimized group.
	// Returns:
	//     Size of the icon
	//-----------------------------------------------------------------------
	virtual CSize GetIconSize() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to calculate destination rectangle of popup bar
	// Parameters:
	//     ptPopup - Point to display popup bar
	//     sz - Size of popup bar
	// Returns:
	//     Rectangle where popup bar will be located.
	//-----------------------------------------------------------------------
	virtual CRect CalculatePopupRect(CPoint ptPopup, CSize sz);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function determines which ribbon group,
	//     if any, is at a specified position.
	// Parameters:
	//     point  - A CPoint that contains the coordinates of the point to test.
	// Returns:
	//      The CXTPRibbonGroup that is at the specified point, if no group is
	//      at the point, then NULL is returned.
	//-----------------------------------------------------------------------
	CXTPRibbonGroup* HitTestGroup(CPoint point) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to change the tracking state.
	// Parameters:
	//     bMode        - TRUE to set the tracking mode; otherwise FALSE.
	//     bSelectFirst - TRUE to select the first item.
	//     bKeyboard    - TRUE if the item is popuped by the keyboard.
	// Returns:
	//     TRUE if the method was successful.
	//-----------------------------------------------------------------------
	virtual BOOL SetTrackingMode(int bMode, BOOL bSelectFirst = TRUE, BOOL bKeyboard = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when keyboard tip was found for the commandbar
	// Parameters:
	//     pTip - Keyboard tip was found for commandbar
	//-----------------------------------------------------------------------
	void OnKeyboardTip(CXTPCommandBarKeyboardTip* pTip);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to determin if cues are visible
	// Returns:
	//     True if visible, False otherwise.
	//-----------------------------------------------------------------------
	BOOL IsKeyboardCuesVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     The framework calls this member function to determine whether a
	//     point is in the bounding rectangle of the specified tool.
	// Parameters:
	//     point - Specifies the x- and y-coordinate of the cursor. These
	//             coordinates are always relative to the upper-left corner of the window
	//     pTI   - A pointer to a TOOLINFO structure.
	// Returns:
	//     If the tooltip control was found, the window control ID. If
	//     the tooltip control was not found, -1.
	//-----------------------------------------------------------------------
	INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;

protected:
//{{AFX_CODEJOCK_PRIVATE
	void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
//}}AFX_CODEJOCK_PRIVATE

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to process special keys.
	// Parameters:
	//     key - Special keys enumerator. Can be any of the values listed in the Remarks section.
	// Remarks:
	//     CXTPCommandBar maps key passed as parameter of OnKeyDown to XTPSpecialKey enumerator and call ProcessSpecialKey.<p/>
	//     key parameter can be one of the following:
	//         * <b>xtpKeyNext</b> Indicates the RIGHT or DOWN key was pressed  Next control must be selected
	//         * <b>xtpKeyPrev</b> Indicates the LEFT or UP key was pressed. Previous control must be selected.
	//         * <b>xtpKeyBack</b> Indicates the LEFT key was pressed for popup bar. Current submenu must be closed
	//         * <b>xtpKeyPopup</b> Indicates the RIGHT key was pressed for popup bar. Submenu must be opened.
	//         * <b>xtpKeyEscape</b> Indicates the ESCAPE key was pressed.
	//         * <b>xtpKeyReturn</b> Indicates the RETURN key was pressed
	//         * <b>xtpKeyHome</b> Indicates the HOME key was pressed. First control must be selected.
	//         * <b>xtpKeyEnd</b> Indicates the END key was pressed. Last control must be selected.
	// Returns:
	//     TRUE if key handled, otherwise returns FALSE
	//-----------------------------------------------------------------------
	virtual BOOL ProcessSpecialKey(XTPSpecialKey key);

//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()
//}}AFX_CODEJOCK_PRIVATE

public:
	CXTPRibbonTab* m_pRibbonTab;  // Ribbon Bar tab the group belongs to.
	CXTPRibbonBar* m_pRibbonBar;  // The CXTPRibbonBar object the CXTPRibbonTab belongs to.
	CRect m_rcGroups;             // Groups bounding rectangle.

};

//{{AFX_CODEJOCK_PRIVATE

class CXTPRibbonBarMorePopupToolBar : public CXTPPopupToolBar
{
	DECLARE_DYNAMIC(CXTPRibbonBarMorePopupToolBar)

public:
	CXTPRibbonBarMorePopupToolBar(CXTPRibbonBar* pRibbonBar);

public:
	void FillCommandBarEntry(CDC* pDC);
	CSize GetIconSize() const;
	CSize GetButtonSize() const;

public:
	CXTPRibbonBar* m_pRibbonBar;
};


class CXTPRibbonGroupPopupToolBar : public CXTPPopupToolBar, public CXTPRibbonScrollableBar
{
	DECLARE_XTP_COMMANDBAR(CXTPRibbonGroupPopupToolBar)
public:
	CXTPRibbonGroupPopupToolBar(CXTPRibbonGroup* pRibbonGroup = NULL);
	~CXTPRibbonGroupPopupToolBar();

public:
	CSize CalcDynamicLayout(int, DWORD /*nMode*/);
	void DrawCommandBar(CDC* pDC, CRect rcClipBox);
	void CreateKeyboardTips();
	CSize GetIconSize() const;
	CSize GetButtonSize() const;
	virtual void OnGroupsScroll(BOOL bScrollLeft);
	virtual CRect GetGroupsRect() const;
	virtual BOOL SetTrackingMode(int bMode, BOOL bSelectFirst = TRUE, BOOL bKeyboard = FALSE);
	virtual CXTPRibbonGroup* HitTestGroup(CPoint point) const;
	BOOL IsKeyboardCuesVisible() const;
protected:
	void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	virtual BOOL ProcessSpecialKey(XTPSpecialKey key);

	DECLARE_MESSAGE_MAP()

public:
	CXTPRibbonGroup* m_pRibbonGroup;
	CXTPRibbonBar* m_pRibbonBar;

	CXTPRibbonGroup* m_pParentGroup;
};


//}}AFX_CODEJOCK_PRIVATE


#endif // !defined(__XTPRIBBONPOPUPS_H__)
