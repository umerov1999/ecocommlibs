//===========================================================================
// Summary:
//     CXTPStatusBarSwitchPane is a CXTPStatusBarPane derived class. CXTPStatusBarSwitchPane works with
//     CXTStatusBar and allows you to create switches in your status
//     bar area.
//===========================================================================
class _XTP_EXT_CLASS CXTPStatusBarSwitchPane : public CXTPStatusBarPane
{
	DECLARE_DYNAMIC(CXTPStatusBarSwitchPane)
public:

	//-------------------------------------------------------------------------
	// Summary: Holds status bar pane switch properties.
	//-------------------------------------------------------------------------
	struct SWITCH
	{
		CString strToolTip; // Tooltip of the switch.
		UINT nID;           // Id of the switch.
		int nWidth;         // Width of the switch.
		BOOL bChecked;      // TRUE to "check" the switch.
		BOOL bHighlighted;  // TRUE if the switch is highlighted.
		BOOL bPressed;      // TRUE if the switch is pressed.
		BOOL bEnabled;      // TRUE if enabled
	};
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPStatusBarSwitchPane object.
	//-----------------------------------------------------------------------
	CXTPStatusBarSwitchPane();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPStatusBarSwitchPane object, handles cleanup and
	//     deallocation
	//-----------------------------------------------------------------------
	~CXTPStatusBarSwitchPane();

public:

	//-----------------------------------------------------------------------
	// Input:   lpIDArray - Pointer to Array with identifiers
	//          nIDCount - Count of elements in array
	// Summary: Call this method to set switches
	// Returns: TRUE if successful;
	//-----------------------------------------------------------------------
	BOOL SetSwitches(const UINT* lpIDArray, int nIDCount);

	//-----------------------------------------------------------------------
	// Input:   nID - Id of the switch.
	//          lpszToolTip - Tooltip of the switch.
	// Summary: Adds a switch to the Switch Pane.
	// Returns: Reference to a SWITCH object.
	//-----------------------------------------------------------------------
	SWITCH* AddSwitch(UINT nID);
	SWITCH* AddSwitch(UINT nID, LPCTSTR lpszToolTip); // <combine CXTPStatusBarSwitchPane::AddSwitch@UINT>

	//-----------------------------------------------------------------------
	// Summary: Returns switch by its position
	// Input:   nIndex - Index of switch to get
	// Returns: POinter to SWITCH.
	//-----------------------------------------------------------------------
	SWITCH* GetSwitch(int nIndex) const;
	SWITCH* FindSwitch(UINT nID) const;

	//-----------------------------------------------------------------------
	// Summary: Gets the total number of switches in the switch pane.
	// Returns: Total number of switches in the switch pane.
	//-----------------------------------------------------------------------
	int GetSwitchCount() const;

	//-------------------------------------------------------------------------
	// Summary: Removes all switches from the pane.
	//-------------------------------------------------------------------------
	void RemoveAll();

	//-----------------------------------------------------------------------
	// Summary: Removes a switch from the Switch Pane.
	// Parameters:
	//          nID - Id of the switch to remove.
	//-----------------------------------------------------------------------
	void RemoveSwitch(UINT nID);

	//-----------------------------------------------------------------------
	// Summary: Specifies whether the switch pane is enabled.
	// Parameters:
	//          nID - Id of the switch to enable\disable.
	//          bEnabled - TRUE to enable, FALSE to disable.
	//-----------------------------------------------------------------------
	void EnableSwitch(UINT nID, BOOL bEnabled);

	//-----------------------------------------------------------------------
	// Input:   nID - Id of the switch to be checked.
	// Summary: Sets the Id of the switch that is currently checked.
	// Remarks: Checked is used to get or set the currently selected\checked
	//          switch in the switch pane.
	//-----------------------------------------------------------------------
	void SetChecked(UINT nID);

	//-----------------------------------------------------------------------
	// Summary: Gets the width of the switch pane.
	// Returns: Width of the switch pane.
	//-----------------------------------------------------------------------
	virtual int GetWidth();

public:

	//-----------------------------------------------------------------------
	// Input:   point - Point to test.
	//          rcItem - Retrieves bounding rectangle of the item.
	// Summary: Call this method to determine which pane, if any, is at the
	//          specified screen position.
	// Returns: A SWITCH object containing data pf the pane at the specified point.
	//-----------------------------------------------------------------------
	SWITCH* HitTest(CPoint point, CRect& rcItem) const;

protected:

	//-----------------------------------------------------------------------
	// Input:   point - A CPoint that specifies the x- and y- coordinates of
	//                  the cursor. These coordinates are always relative to
	//                  the upper-left corner of the window.
	// Summary: This member function is used to processes left mouse button events.
	//-----------------------------------------------------------------------
	virtual void OnLButtonDown(CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw pane switch item. Override it to
	//     draw a custom switch.
	// Parameters:
	//     pDC    - Pointer to a valid device context
	//     rcItem - Item rectangle
	//-----------------------------------------------------------------------
	virtual void OnDraw(CDC* pDC, CRect rcItem);

	//-----------------------------------------------------------------------
	// Input:   point - Specifies the x- and y coordinate of the cursor.
	// Summary: This member is called when the mouse cursor moves.
	//-----------------------------------------------------------------------
	virtual void OnMouseMove(CPoint point);

	//-----------------------------------------------------------------------
	// Input:   point - Specifies the x- and y coordinate of the cursor. These
	//                  coordinates are always relative to the upper-left
	//                  corner of the window.
	//          pTI - A pointer to a TOOLINFO structure.
	// Summary: The framework calls this member function to determine whether
	//          a point is in the bounding rectangle of the specified tool.
	// Returns: If the tooltip control was found, the window control ID. If
	//          the tooltip control was not found, -1.
	//-----------------------------------------------------------------------
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPStatusBarSwitchPane);

	void OleAddSwitch(UINT nID, LPCTSTR lpszToolTip);
	void OleRemoveSwitch(UINT nID);
	int OleGetChecked();
	BSTR OleGetSwitchTooltip(int nID);
	void OleSetSwitchTooltip(int nID, LPCTSTR lpszTooltip);
//}}AFX_CODEJOCK_PRIVATE
#endif

protected:
	CArray<SWITCH*, SWITCH*> m_arrSwitches; // Collection of switces in the switch pane.
	SWITCH* m_pHighlighted;                 // TRUE if the switch is currently highlighted.
};
