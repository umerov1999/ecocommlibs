// XTPRibbonSystemButton.h: interface for the CXTPRibbonControlSystemButton class.
//
// This file is a part of the XTREME RIBBON MFC class library.
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
#if !defined(__XTPRIBBONSYSTEMBUTTON_H__)
#define __XTPRIBBONSYSTEMBUTTON_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000




//===========================================================================
// Summary:
//     CXTPRibbonControlSystemButton is CXTPControlPopup derived class used as system
//     button of ribbon bar
//===========================================================================
class _XTP_EXT_CLASS CXTPRibbonControlSystemButton : public CXTPControlPopup
{
	DECLARE_XTP_CONTROL(CXTPRibbonControlSystemButton)
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPRibbonControlSystemButton object
	//-----------------------------------------------------------------------
	CXTPRibbonControlSystemButton();

public:
	//----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the control.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//----------------------------------------------------------------------
	void Draw(CDC* pDC);

	//----------------------------------------------------------------------
	// Summary:
	//     This method is called to determine if the system button is a normal
	//     button or the Office 2007 style system button.
	// Returns:
	//     TRUE if this is a normal button, FALSE if this is an Office 2007
	//     style system button.
	//----------------------------------------------------------------------
	BOOL IsSimpleButton() const;

	//----------------------------------------------------------------------
	// Summary:
	//     This method is called to determine the size of the control.
	// Parameters:
	//     pDC - Pointer to a valid device context
	// Returns:
	//     Size of the control.
	//----------------------------------------------------------------------
	virtual CSize GetSize(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the control's caption.
	// Returns:
	//     The caption of the control.
	//-----------------------------------------------------------------------
	virtual CString GetCaption() const;

protected:
	//----------------------------------------------------------------------
	// Summary:
	//     This member is called when the user double-clicks the left mouse button.
	// Parameters:
	//     point - Specifies the x- and y-coordinates of the cursor.
	// Returns:
	//     TRUE if successful; otherwise returns FALSE
	//----------------------------------------------------------------------
	virtual BOOL OnLButtonDblClk(CPoint point);

	// ----------------------------------------------------------------------
	// Summary:
	//         Updates rectangle to set position where child popup bar become
	//         visible.
	// Parameters:
	//     rc :         CRect object specifying size of area.
	//     bVertical :  TRUE if vertical. FALSE otherwise.
	// ----------------------------------------------------------------------
	void AdjustExcludeRect(CRect& rc, BOOL bVertical);

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPRibbonControlSystemButton);

	//}}AFX_CODEJOCK_PRIVATE
#endif
protected:
	BOOL m_bCloseOnDblClick;

	friend class CXTPRibbonBackstageView;
	friend class CXTPRibbonBar;
};


//===========================================================================
// Summary:
//     CXTPRibbonControlSystemPopupBarButton is CXTPControlButton derived class used
//     in system popupbar as bottom option buttons
//===========================================================================
class _XTP_EXT_CLASS CXTPRibbonControlSystemPopupBarButton : public CXTPControlButton
{
	DECLARE_XTP_CONTROL(CXTPRibbonControlSystemPopupBarButton)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPRibbonControlSystemPopupBarButton object
	//-----------------------------------------------------------------------
	CXTPRibbonControlSystemPopupBarButton();

	//----------------------------------------------------------------------
	// Summary:
	//     This method is called to determine the size of the control.
	// Parameters:
	//     pDC - Pointer to a valid device context
	// Returns:
	//     Size of the control.
	//----------------------------------------------------------------------
	virtual CSize GetSize(CDC* pDC);

	//----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the control.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//----------------------------------------------------------------------
	virtual void Draw(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if control need to draw its background
	// Returns:
	//     TRUE if paint manager must skip filling background of control
	//-----------------------------------------------------------------------
	BOOL IsTransparent() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the size of the command bar button.
	// Returns:
	//     The width and height values of the command bar button.
	//-----------------------------------------------------------------------
	virtual CSize GetButtonSize() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the size of a toolbar icon.
	// See Also:
	//     GetButtonSize
	//-----------------------------------------------------------------------
	virtual CSize GetIconSize() const;

private:
	BOOL IsSystemPopupButton() const;
};

//===========================================================================
// Summary:
//     CXTPRibbonSystemPopupBar is CXTPPopupBar derived class used
//     as system popupbar of RibbonBar.
//===========================================================================
class _XTP_EXT_CLASS CXTPRibbonSystemPopupBar : public CXTPPopupBar
{
	DECLARE_XTP_COMMANDBAR(CXTPRibbonSystemPopupBar)

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPRibbonSystemPopupBar object
	//-----------------------------------------------------------------------
	CXTPRibbonSystemPopupBar();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to get borders margins of commandbar
	// Returns:
	//     Rectangle contains border margins
	//-----------------------------------------------------------------------
	virtual CRect GetBorders();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw background of command bar
	// Parameters:
	//     pDC - POinter to device context to draw
	// See Also: DrawCommandBar
	//-----------------------------------------------------------------------
	virtual void FillCommandBarEntry(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to get the size of the command bar.
	// Parameters:
	//      nLength - The requested dimension of the control bar, either horizontal or vertical, depending on dwMode.
	//      dwMode - see CControlBar::CalcDynamicLayout for list of supported flags.
	// Returns:
	//     Size of the command bar.
	//-----------------------------------------------------------------------
	virtual CSize CalcDynamicLayout(int nLength, DWORD dwMode);

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	afx_msg LRESULT OnNcHitTest(CPoint point);
//}}AFX_CODEJOCK_PRIVATE
};

//===========================================================================
// Summary:
//     CXTPRibbonSystemPopupBarPage is CXTPPopupBar derived class used
//     as system popupbar page of RibbonBar.
//===========================================================================
class _XTP_EXT_CLASS CXTPRibbonSystemPopupBarPage : public CXTPPopupBar
{
	DECLARE_XTP_COMMANDBAR(CXTPRibbonSystemPopupBarPage)

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPRibbonSystemPopupBarPage object
	//-----------------------------------------------------------------------
	CXTPRibbonSystemPopupBarPage();

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to get the size of the command bar.
	// Parameters:
	//      nLength - The requested dimension of the control bar, either horizontal or vertical, depending on dwMode.
	//      dwMode - see CControlBar::CalcDynamicLayout for list of supported flags.
	// Returns:
	//     Size of the command bar.
	//-----------------------------------------------------------------------
	virtual CSize CalcDynamicLayout(int nLength, DWORD dwMode);

	//-----------------------------------------------------------------------
	// Summary:
	//     Updates rectangle to set position where popup bar become visible.
	// Parameters:
	//     rc - CRect object specifying size of exclude area.
	//     bVertical :  TRUE if vertical. FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual void AdjustExcludeRect(CRect& rc, BOOL bVertical);
};

//===========================================================================
// Summary:
//     CXTPRibbonControlSystemPopupBarListCaption is CXTPControl derived class.
//     It used in Ribbon system popup to draw caption of enumerable controls
//===========================================================================
class _XTP_EXT_CLASS CXTPRibbonControlSystemPopupBarListCaption : public CXTPControl
{
	DECLARE_XTP_CONTROL(CXTPRibbonControlSystemPopupBarListCaption)
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPRibbonControlSystemPopupBarListCaption object
	//-----------------------------------------------------------------------
	CXTPRibbonControlSystemPopupBarListCaption();

protected:
	//----------------------------------------------------------------------
	// Summary:
	//     This method is called to determine the size of the control.
	// Parameters:
	//     pDC - Pointer to a valid device context
	// Returns:
	//     Size of the control.
	//----------------------------------------------------------------------
	virtual CSize GetSize(CDC* pDC);

	//----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the control.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//----------------------------------------------------------------------
	virtual void Draw(CDC* pDC);
};

//===========================================================================
// Summary:
//     CXTPRibbonControlSystemPopupBarListItem is CXTPControlButton derived class.
//     It used in Ribbon system popup to draw button of enumerable controls
//===========================================================================
class _XTP_EXT_CLASS CXTPRibbonControlSystemPopupBarListItem : public CXTPControlButton
{
	DECLARE_XTP_CONTROL(CXTPRibbonControlSystemPopupBarListItem)

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPRibbonControlSystemPopupBarListItem object
	//-----------------------------------------------------------------------
	CXTPRibbonControlSystemPopupBarListItem();

	//----------------------------------------------------------------------
	// Summary:
	//     This method is called to determine the size of the control.
	// Parameters:
	//     pDC - Pointer to a valid device context
	// Returns:
	//     Size of the control.
	//----------------------------------------------------------------------
	virtual CSize GetSize(CDC* pDC);

	//----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the control.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//----------------------------------------------------------------------
	virtual void Draw(CDC* pDC);

protected:
	BOOL m_bAlignShortcut;  // Alignment of the shortcut in the list.
};

//===========================================================================
// Summary:
//     CXTPRibbonControlSystemPopupBarPinableListItem is CXTPRibbonControlSystemPopupBarListItem derived class.
//     It used in Ribbon system popup to draw recent file list items.
//===========================================================================
class _XTP_EXT_CLASS CXTPRibbonControlSystemPopupBarPinableListItem : public CXTPRibbonControlSystemPopupBarListItem
{
	DECLARE_XTP_CONTROL(CXTPRibbonControlSystemPopupBarPinableListItem)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPRibbonControlSystemPopupBarListItem object
	//-----------------------------------------------------------------------
	CXTPRibbonControlSystemPopupBarPinableListItem();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPRibbonControlSystemPopupBarPinableListItem object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	~CXTPRibbonControlSystemPopupBarPinableListItem();


protected:
	//----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the control.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//----------------------------------------------------------------------
	virtual void Draw(CDC* pDC);

	//----------------------------------------------------------------------
	// Summary:
	//     This member is called when the mouse cursor moves.
	// Parameters:
	//     point - Specifies the x- and y-coordinate of the cursor.
	//----------------------------------------------------------------------
	virtual void OnMouseMove(CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when the user releases the left mouse button.
	// Parameters:
	//     point - Specifies the x- and y-coordinate of the cursor.
	//-----------------------------------------------------------------------
	virtual void OnLButtonUp(CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the MRU item becomes selected.
	// Parameters:
	//     bSelected - TRUE if the MRU item becomes selected.
	// Returns:
	//     TRUE if successful; otherwise returns FALSE
	//-----------------------------------------------------------------------
	virtual BOOL OnSetSelected(int bSelected);

protected:
	CXTPImageManager* m_pIcons;  // Icons used for the MRU item.

};

//===========================================================================
// Summary:
//     CXTPRibbonControlSystemRecentFileList is a CXTPRibbonControlSystemPopupBarListCaption derived class.
//     It used in Ribbon system popup to draw recent file list controls
//===========================================================================
class _XTP_EXT_CLASS CXTPRibbonControlSystemRecentFileList : public CXTPRibbonControlSystemPopupBarListCaption
{
	class CControlFileItem;
	class CControlPinableFileItem;
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPRibbonControlSystemRecentFileList object
	//-----------------------------------------------------------------------
	CXTPRibbonControlSystemRecentFileList();

protected:

	//----------------------------------------------------------------------
	// Summary:
	//     This method is called before recalculating the parent command
	//     bar size to calculate the dimensions of the control.
	// Parameters:
	//     dwMode - Flags used to determine the height and width of the
	//              dynamic command bar.
	//-----------------------------------------------------------------------
	virtual void OnCalcDynamicSize(DWORD dwMode);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to check if the user can drop the control.
	// Parameters:
	//     pCommandBar - Points to a CXTPCommandBar object
	//     point - Mouse position.
	//     dropEffect - DROPEFFECT enumerator.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE
	//-----------------------------------------------------------------------
	virtual BOOL IsCustomizeDragOverAvail(CXTPCommandBar* pCommandBar, CPoint point, DROPEFFECT& dropEffect);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Returns pointer to recent file list collection
	// Returns:
	//     Pointer to recent file list collection
	//-----------------------------------------------------------------------
	virtual CRecentFileList* GetRecentFileList();

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns ID_FILE_MRU_FILE1.
	//-----------------------------------------------------------------------
	virtual int GetFirstMruID();

private:
	CString ConstructCaption(const CString& lpszTitle, int nIndex);

private:
	DECLARE_XTP_CONTROL(CXTPRibbonControlSystemRecentFileList)
};

#define CXTPPinableRecentFileList CXTPRecentFileList

#endif // !defined(__XTPRIBBONSYSTEMBUTTON_H__)
