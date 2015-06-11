class CXTPTabPaintManagerColorSet;
class CXTPTabManager;
class CXTPTabManagerItem;
class CXTPTabPaintManager;

//-------------------------------------------------------------------------
// Summary:
//     CXTPTabPaintManagerTheme is a class that represents the appearance of all the
//     components (tab button, tab caption font, button size, tab margins, etc)
//     of the tab manager.
// Remarks:
//     CXTPTabPaintManagerTheme is the base class for all appearances.  To create a custom
//     appearance, and new class can be derived from the base class or any of
//     the appearances.
//
//     The appearance should be updated in the RefreshMetrics member's for each
//     CXTPTabPaintManagerTheme object.
// See Also: XTPTabAppearanceStyle, SetAppearance, SetAppearanceSet, GetAppearance,
//           GetAppearanceSet, SetColor, GetColor, GetColorSet, SetColorSet, CXTPTabPaintManagerColorSet
//-------------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPTabPaintManagerTheme
{
public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTabPaintManagerTheme object.
	//-------------------------------------------------------------------------
	CXTPTabPaintManagerTheme();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPTabPaintManagerTheme object, handles cleanup and deallocation.
	//-------------------------------------------------------------------------
	virtual ~CXTPTabPaintManagerTheme();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to refresh the appearance metrics.
	// Remarks:
	//     This member is used to refresh the appearance metrics such
	//     as font, button height, margin size, etc.  Override this
	//     member in derived classes to change the appearance metrics.
	// See Also: CXTPTabPaintManager::SetFontIndirect, GetHeaderMargin, GetClientMargin, GetButtonLength, GetButtonHeight
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the currently used color set.
	// Returns:
	//     Currently used custom color set.
	// See Also: XTPTabColorStyle, CXTPTabPaintManager::GetColor, CXTPTabPaintManager::SetColorSet, CXTPTabPaintManager::SetColor
	//-----------------------------------------------------------------------
	CXTPTabPaintManagerColorSet* GetColorSet() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the bounding rectangle to the tab
	//     header area.
	// Parameters:
	//     rcControl   - Bounding rectangle of the tab header.
	//                   See CXTPTabManager::GetControlRect.
	//     pTabManager - Pointer to tab manager.
	// Returns:
	//     Bounding rectangle of tab header area.
	// See Also: GetClientRect, GetHeaderMargin, GetClientMargin, CXTPTabManager::GetControlRect
	//-----------------------------------------------------------------------
	virtual CRect GetHeaderRect(CRect rcControl, CXTPTabManager* pTabManager);
	virtual CRect GetHeaderClipBox(CDC* pDC, CXTPTabManager* pTabManager);

	// Summary:
	//     Call this member to get the height of the tab
	//     header area.
	// Parameters:
	//     pTabManager - Pointer to tab manager.
	// Returns:
	//     Height of tab header area.
	// See Also: GetHeaderRect, GetClientRect, GetHeaderMargin, GetClientMargin, CXTPTabManager::GetControlRect
	//-----------------------------------------------------------------------
	int GetHeaderHeight(CXTPTabManager* pTabManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the bounding rectangle to the tab
	//     client area.
	// Parameters:
	//     rcControl   - Bounding rectangle of the tab manager control.
	//                   See CXTPTabManager::GetControlRect.
	//     pTabManager - Pointer to tab manager.
	// Returns:
	//     Bounding rectangle of tab client area.
	// See Also: GetHeaderRect, GetHeaderMargin, GetClientMargin, CXTPTabManager::GetControlRect
	//-----------------------------------------------------------------------
	virtual CRect GetClientRect(CRect rcControl, CXTPTabManager* pTabManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to get the top, left, right, and bottom
	//     tab header margins.
	// Returns:
	//     CRect struct containing the left, top, right, and bottom margins
	//     of the tab header.  This is the margin (space) placed around the
	//     tab buttons inside the tab header.
	//
	// Remarks:
	//     The CRect structure will contain the margins of the tab header.
	//     m_rcHeaderMargin contains these margins.
	//
	//     This is the margin placed around the tab buttons in the tab header.
	//     This is not placed around each tab button, but all the tab buttons.
	//
	// Example:
	// <code>
	// // Set the top, left, right, and bottom tab header margins to 5 pixels.  This will
	// // place a margin of 5 pixels around the tab buttons in the tab header.
	// m_wndTabControl.GetPaintManager()->GetAppearanceSet()->m_rcHeaderMargin.SetRect(5, 5, 5, 5);
	//
	// // Set the top, left, right, and bottom tab control margins to 4 pixels.  This will
	// // place a margin of 4 pixels around the entire tab control (Includes tab header and tab client area).
	// m_wndTabControl.GetPaintManager()->m_rcControlMargin.SetRect(4, 4, 4, 4);
	//
	// // Set the top, left, right, and bottom tab button margins to 2 pixels.  This will
	// // place a margin of 2 pixels around the text and icon in the tab buttons.
	// m_wndTabControl.GetPaintManager()->m_rcButtonMargin.SetRect(2, 2, 2, 2);
	//
	// // Set the top, left, right, and bottom tab client margins to 10 pixels.  This will
	// // place a margin of 10 pixels around the tab client area.
	// m_wndTabControl.GetPaintManager()->m_rcClientMargin.SetRect(10, 10, 10, 10);
	// </code>
	// See Also: m_rcHeaderMargin, GetHeaderRect
	//-----------------------------------------------------------------------
	virtual CRect GetHeaderMargin();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to get the top, left, right, and bottom
	//     tab client area margins.
	// Returns:
	//     CRect struct containing the left, top, right, and bottom margins
	//     of the tab client area.  This is the margin (space) placed around the
	//     tab client area inside the tab control.
	//
	// Remarks:
	//     The CRect structure will contain the margins of the tab client area.
	//     CXTPTabPaintManager::m_rcClientMargin contains these margins.
	//
	//     This is the margin placed around the tab client area in the tab control.
	//
	// Example:
	// <code>
	// // Set the top, left, right, and bottom tab header margins to 5 pixels.  This will
	// // place a margin of 5 pixels around the tab buttons in the tab header.
	// m_wndTabControl.GetPaintManager()->GetAppearanceSet()->m_rcHeaderMargin.SetRect(5, 5, 5, 5);
	//
	// // Set the top, left, right, and bottom tab control margins to 4 pixels.  This will
	// // place a margin of 4 pixels around the entire tab control (Includes tab header and tab client area).
	// m_wndTabControl.GetPaintManager()->m_rcControlMargin.SetRect(4, 4, 4, 4);
	//
	// // Set the top, left, right, and bottom tab button margins to 2 pixels.  This will
	// // place a margin of 2 pixels around the text and icon in the tab buttons.
	// m_wndTabControl.GetPaintManager()->m_rcButtonMargin.SetRect(2, 2, 2, 2);
	//
	// // Set the top, left, right, and bottom tab client margins to 10 pixels.  This will
	// // place a margin of 10 pixels around the tab client area.
	// m_wndTabControl.GetPaintManager()->m_rcClientMargin.SetRect(10, 10, 10, 10);
	// </code>
	// See Also: CXTPTabPaintManager::m_rcClientMargin, GetClientRect
	//-----------------------------------------------------------------------
	virtual CRect GetClientMargin();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to get the total button length including
	//     the left and right button margin.
	// Parameters:
	//     pDC   - Pointer to a valid device context.
	//     pItem - Pointer to tab button to get the length of.
	// Returns:
	//     Tab button length including the left and right button margin.
	// Remarks:
	//     This members calls CXTPTabPaintManager::DrawSingleButtonIconAndText
	//     with bDraw = FALSE to get the total button length including margins.
	// See Also: CXTPTabPaintManager::m_rcButtonMargin, GetClientMargin, GetHeaderMargin, CXTPTabPaintManager::DrawSingleButtonIconAndText, CXTPTabManager::GetItemMetrics, GetButtonHeight
	//-----------------------------------------------------------------------
	virtual int GetButtonLength(CDC* pDC, CXTPTabManagerItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to determine the total height of the tab buttons
	//     including the top and bottom margins and icon size.
	// Parameters:
	//     pTabManager - Pointer to the tab manager to get the button height.
	// Returns:
	//     The height of the tab buttons.
	// Remarks:
	//     This member add the size of the left and right margins and the
	//     icon size to the size of CXTPTabPaintManager.CXTPTabPaintManagerTheme.m_nButtonHeight,
	//     which is the current height of the tabs without margins and icon size.
	// See Also: CXTPTabPaintManager.CXTPTabPaintManagerTheme.m_nButtonHeight, CXTPTabPaintManager::m_rcButtonMargin, GetClientMargin, GetHeaderMargin, CXTPTabPaintManager::DrawSingleButtonIconAndText, CXTPTabManager::GetItemMetrics, GetButtonLength
	//-----------------------------------------------------------------------
	virtual int GetButtonHeight(const CXTPTabManager* pTabManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method called to get bounding rectangle of item that was drawn
	// Parameters:
	//     pItem - Item to test
	// Returns: Bounding rectangle of drawing item
	// See Also: CXTPTabManagerItem::GetRect
	//-----------------------------------------------------------------------
	virtual CRect GetButtonDrawRect(const CXTPTabManagerItem* pItem);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to draw the tab control.
	// Parameters:
	//     pTabManager - Pointer to the tab manager to draw.
	//     pDC         - Pointer to a valid device context.
	//     rcControl   - Bounding rectangle of the client area.
	// Remarks:
	//     This member is called in various Draw and Paint functions.
	//
	//     Unless overridden, this member will just call
	//     CXTPTabPaintManager::DrawTabControlEx.
	// See Also: CXTPTabPaintManager::DrawTabControlEx.
	//-----------------------------------------------------------------------
	virtual void DrawTabControl(CXTPTabManager* pTabManager, CDC* pDC, CRect rcControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member fills in the tab header and tab client area.  It
	//     will then draw tab client frame.
	// Parameters:
	//     pTabManager - Pointer to tab manager to fill.
	//     pDC         - Pointer to a valid device context.
	//     rcControl   - Bounding rectangle of the tab manager control.
	//                   See CXTPTabManager::GetControlRect.
	// Returns:
	//     Tab header bounding rectangle.  This value is returned from GetHeaderRect.
	// Remarks:
	//     If CXTPTabPaintManager::m_bFillBackground is TRUE, then the tab client
	//     space is filled using CXTPTabPaintManagerColorSet::m_clrControlFace color.
	//     CXTPTabPaintManager::m_bFillBackground is only FALSE when drawing
	//     DockingPanePanel AutoHide tabs because there is no client area to draw.
	// See Also: GetHeaderRect, CXTPTabPaintManager::m_bFillBackground, XTPTabClientFrame, CXTPTabPaintManager::m_clientFrame
	//-----------------------------------------------------------------------
	virtual CRect FillTabControl(CXTPTabManager* pTabManager, CDC* pDC, CRect rcControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to move the tab buttons of the tab control to their
	//     correct positions.
	// Parameters:
	//     pTabManager - Pointer to the tab manager the tab button are on.
	//     pDC         - Pointer to a valid device context.
	//     rcClient    - Bounding rectangle of the tab client area.
	// Remarks:
	//     CXTPTabPaintManager::RepositionTabControl and CXTPTabPaintManager::AdjustClientRect
	//     are called when the window is resized.  When DrawTabControl is called
	//     the changes will be used when drawing the tab control.
	//
	//     This member is called by CXTPTabPaintManager::RepositionTabControl.
	//     Unless overridden this member will just call CXTPTabPaintManager::RepositionTabControlEx.
	//-----------------------------------------------------------------------
	virtual void RepositionTabControl(CXTPTabManager* pTabManager, CDC* pDC, CRect rcClient);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to adjust the tab client rectangle.  It will
	//     return the new tab client rectangle with margins included.
	// Parameters:
	//     pTabManager - Pointer to the tab manager to adjust the tab client rectangle.
	//     rcClient    - [out] CRect to store tab client rectangle.
	// Remarks:
	//     CXTPTabPaintManager::AdjustClientRect and CXTPTabPaintManager::RepositionTabControl
	//     are called when the window is resized.  When CXTPTabPaintManager::DrawTabControl
	//     is called the changes will be used when drawing the tab control.
	//
	//     This member will adjust the tab client rectangle based on the
	//     currently set XTPTabClientFrame.
	//
	//     Then CXTPTabPaintManager::AdjustClientRect calls this member
	//     if there is one or more tab in the tab manager.
	// See Also:
	//     CXTPTabPaintManager::AdjustClientRect
	//-----------------------------------------------------------------------
	virtual void AdjustClientRect(CXTPTabManager* pTabManager, CRect& rcClient);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw a single tab button in the tab
	//     client header area.
	// Parameters:
	//     pDC   - Pointer to a valid device context.
	//     pItem - Tab button to draw.
	// Remarks:
	//     This method is overridden in derived classes and will draw a
	//     single tab button.  This method only draws the button, no text
	//     or icon is added.
	//
	//     DrawSingleButton calls CXTPTabPaintManager::DrawSingleButtonIconAndText
	//     after the button is drawn.  This is called to draw the button's
	//     icon and text.
	// See Also:: CXTPTabPaintManager::DrawTabControlEx, CXTPTabPaintManager::DrawSingleButtonIconAndText
	//-----------------------------------------------------------------------
	virtual void DrawSingleButton(CDC* pDC, CXTPTabManagerItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to draw the different parts that make up
	//     the TabMDIClient TabWorkSpace.
	// Parameters:
	//     pDC    - Pointer to a valid device context.
	//     lpRect - Size of splitter.
	//     workspacePart  - XTPTabWorkspacePart to draw.
	// Remarks:
	//     This is used when drawing the TabWorkspace of MDI tabs when
	//     tab grouping is enabled.
	//
	//     lpRect is the size of the splitter that is placed between the
	//     vertical or horizontal tab groups if workspacePart is
	//     XTPTabWorkspacePartHSplitter or XTPTabWorkspacePartVSplitter.  And
	//     the splitter will be drawn with lpRect.
	//
	//     If workspacePart is XTPTabWorkspacePartWidth, then this is the size
	//     of the splitter.
	// See Also: XTPTabWorkspacePart
	//-----------------------------------------------------------------------
	virtual void DrawWorkspacePart(CDC* pDC, LPRECT lpRect, XTPTabWorkspacePart workspacePart);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve all the supported XTPTabColorStyle
	//     for this appearance.
	// Returns:
	//     Support color sets.
	// See Also: GetDefaultColorSet
	//-----------------------------------------------------------------------
	virtual int GetSupportedColorSets() { return xtpTabColorAll; }

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the default XTPTabColorStyle for this appearance.
	// Returns:
	//     xtpTabColorDefault
	// See Also: GetSupportedColorSets
	//-----------------------------------------------------------------------
	virtual XTPTabColorStyle GetDefaultColorSet() { return xtpTabColorDefault; }

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     This helper method an be called to draw item inside polygon rectangle
	// Parameters:
	//     pDC - Pointer to device context to draw
	//     pItem - Tab item to draw
	//     pts - Array of points that specified rectangle
	//     nCount - Total number of point in rectangle
	//     tabPosition - Tab position
	//     bAbsoulute - Flag that specified if points specified absolute coordinates
	// Returns:
	//     Color of tab was filled
	//-----------------------------------------------------------------------
	COLORREF FillButton(CDC* pDC, CXTPTabManagerItem* pItem, LPPOINT pts, int nCount, XTPTabPosition tabPosition, BOOL bAbsoulute = FALSE);

	//{{AFX_CODEJOCK_PRIVATE
	static void AFX_CDECL DrawPolyLine(CDC* pDC, COLORREF clr, LPPOINT pts, int nCount);
	static void AFX_CDECL DrawSingleLineBorder(CDC* pDC, CRect rc, XTPTabPosition position, COLORREF clrTopLeft, COLORREF clrBottomRight);
	static void AFX_CDECL InflateRectEx(CRect& rc, CRect rcInflate, XTPTabPosition position);
	static void AFX_CDECL DeflateRectEx(CRect& rc, CRect rcDeflate, XTPTabPosition position);
	//}}AFX_CODEJOCK_PRIVATE

	virtual void DrawText(CDC* pDC, CXTPTabManager* pManager, const CString& str, LPRECT lpRect, UINT nFormat);

public:
	CRect m_rcHeaderMargin;                 // CRect struct containing the left, top, right, and bottom margins of the tab header.  This is the margin (space) placed around the tab buttons inside the tab header.
	int   m_nRowMargin;                     // Margin between rows

protected:
	int m_nButtonHeight;                    // Current tab button height (without margins).
	CXTPTabPaintManager* m_pPaintManager;   // Self paint manager pointer.
	BOOL m_bButtonsReverseZOrder;           // Draw buttons in right to left order.

	friend class CXTPTabPaintManager;

};

#define CXTPTabPaintManagerAppearanceSet CXTPTabPaintManagerTheme // Old class name
