class CXTPResourceImages;


class _XTP_EXT_CLASS CXTPDockingPaneVisualStudio2012Theme : public CXTPDockingPaneDefaultTheme // CXTPDockingPanePaintManager
{
// Construction / Destruction
public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPDockingPaneVisualStudio2012Theme object.
	//-------------------------------------------------------------------------
	CXTPDockingPaneVisualStudio2012Theme();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPDockingPaneVisualStudio2012Theme object, handles cleanup and deallocation.
	//-------------------------------------------------------------------------
	virtual ~CXTPDockingPaneVisualStudio2012Theme();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the pane.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get pane's caption rectangle.
	// Parameters:
	//     pPane - Side Panel container to get caption CRECT.
	// Returns:
	//     Bounding rectangle of the side panel's caption.
	//-----------------------------------------------------------------------
	virtual CRect GetPaneCaptionRect(const CXTPDockingPaneSidePanel *pPane);

	virtual void AdjustClientRect(CXTPDockingPaneTabbedContainer* pPane, CRect& rc, BOOL bApply);
	virtual void AdjustCaptionRect(const CXTPDockingPaneTabbedContainer* pPane, CRect& rc);

// Drawing methods
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Override this method to draw splitter.
	// Parameters:
	//     pDC        - Pointer to a valid device context
	//     pSplitter - Pointer to splitter window
	//-----------------------------------------------------------------------
	virtual void DrawSplitter(CDC *pDC, CXTPDockingPaneSplitterWnd *pSplitter);

	//-----------------------------------------------------------------------
	// Summary:
	//     Override this member function to draw a caption.
	// Parameters:
	//     pDC - Reference to the device context in which to draw.
	//     pPane - Pointer to the tabbed container.
	//     rc - Client rectangle of the tabbed container.
	//-----------------------------------------------------------------------
	virtual void DrawCaption(CDC *pDC, CXTPDockingPaneTabbedContainer *pPane, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to draw frame of floating window
	// Parameters:
	//     pDC    - Pointer to a valid device context
	//     pPane - Floating window pointer
	//     rc    - Client rectangle of floating frame
	//-----------------------------------------------------------------------
	virtual void DrawFloatingFrame(CDC *pDC, CXTPDockingPaneMiniWnd *pPane, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Override this member function to draw a tabbed container.
	// Parameters:
	//     pDC    - Reference to the device context in which to draw.
	//     pPane - Pointer to the tabbed container.
	//     rc    - Client rectangle of the tabbed container.
	//-----------------------------------------------------------------------
	virtual void DrawPane(CDC *pDC, CXTPDockingPaneTabbedContainer *pPane, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Override this member function to draw the side panel.
	// Parameters:
	//     pDC    - Reference to the device context in which to draw.
	//     pPane - Pointer to the side panel.
	//     rc    - Client rectangle of the side panel.
	//-----------------------------------------------------------------------
	virtual void DrawSidePanel(CDC *pDC, CXTPDockingPaneSidePanel *pPane, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Override this member function to draw button of tabbed caption.
	// Parameters:
	//     pDC         - Pointer to a valid device context
	//     pButton     - Button pointer need to draw.
	//     clrButton   - Color of button text.
	//-----------------------------------------------------------------------
	virtual void DrawCaptionButton(CDC *pDC, CXTPDockingPaneCaptionButton *pButton, COLORREF clrButton);

	//-----------------------------------------------------------------------
	// Summary:
	//      This mmethod is called by the framework to draw the docking pane
	//      auto hide panel background and splitter.
	// Parameters:
	//      pDC       - Pointer to the auto hide panes device context.
	//      rc        - Size of the area to draw.
	//      direction - Docking direction either xtpPaneDockLeft, xtpPaneDockRight,
	//                  xtpPaneDockTop or xtpPaneDockBottom.
	//-----------------------------------------------------------------------
	virtual void DrawAutoHidePanel(CDC* pDC, CRect rc, XTPDockingPaneDirection direction);

protected:

	void DrawTitleBar(CDC *pDC, CXTPDockingPaneBase *pPane, CRect rc, BOOL bVertical);

	void DrawGripper(CDC *pDC, CRect rcGripper, COLORREF clr, BOOL bVertical);

	void DrawDashedLineH(CDC *pDC, int left, int top, int right,  COLORREF clr);
	void DrawDashedLineV(CDC *pDC, int left, int top, int bottom, COLORREF clr);

public:

	CXTPDockingPaneColorSetVisualStudio2012 m_csInactive;
	CXTPDockingPaneColorSetVisualStudio2012 m_csActive;

	CString               m_strFontFace;     // Font face name
	int                   m_nFontSize;       // Font size in points
};
