class CXTPCommandBarsFrameHook;

/*
class _XTP_EXT_CLASS CXTPFrameElement
{
public:

	virtual CSize GetSize();

	CRect m_rcMargin;
	CRect m_rcPadding;
	CXTPPaintManagerColor m_clrBackground;
};
*/

//===========================================================================
// Summary: Paint manager for the Office 2007 style window frame.
//===========================================================================
class _XTP_EXT_CLASS CXTPFramePaintManager
{
public:

	//-----------------------------------------------------------------------
	// Summary: Constructs a CXTPFramePaintManager object.
	// Input:   pPaintManager - Reference to a CXTPPaintManager object.
	//-----------------------------------------------------------------------
	CXTPFramePaintManager(CXTPPaintManager* pPaintManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPFramePaintManager object, handles cleanup and
	//     deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPFramePaintManager();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the frame icon.
	//-----------------------------------------------------------------------
	void SetIcon(HICON hIcon, CSize szIcon);


public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Calculates the frame caption rect
	//-----------------------------------------------------------------------
	virtual CRect CalcFrameCaption(const CXTPCommandBarsFrameHook *pFrameHook) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Calculates the frame border sizes
	//-----------------------------------------------------------------------
	virtual CRect CalcFrameBorder(const CXTPCommandBarsFrameHook *pFrameHook) const;

public:

	virtual void RecalcLayout(CXTPCommandBarsFrameHook *pFrameHook);


	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to create window region
	// Parameters:
	//      pFrameHook - CXTPCommandBarsFrameHook hook object
	//      sz - Window size
	// Returns: Region of window to be set
	//-----------------------------------------------------------------------
	virtual HRGN CalcFrameRegion(CXTPCommandBarsFrameHook* pFrameHook, CSize sz) const = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw frame if Ribbon was found
	// Parameters:
	//      pDC - Pointer to device context
	//      pFrameHook - CXTPCommandBarsFrameHook hook object
	//-----------------------------------------------------------------------
	virtual void DrawFrame(CDC* pDC, CXTPCommandBarsFrameHook* pFrameHook) = 0;

	virtual void DrawFrameIcon(CDC *pDC, CXTPCommandBarsFrameHook *pFrameHook, CRect &rcCaption);


	virtual void DrawFrameCaptionButtons(
		CDC *pDC, CXTPCommandBarsFrameHook *pFrameHook, CRect &rcCaptionText);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw caption button
	// Parameters:
	//      pDC - Pointer to device context
	//      rc - Bounding rectangle of the button
	//      nId - Identifier of the button
	//      bSelected - TURE if button is highlighted
	//      bPressed -  TURE if button is pressed
	//      bActive - TURE if frame is active
	//-----------------------------------------------------------------------
	virtual void DrawFrameCaptionButton(CDC* pDC, CRect rc, int nId,
		BOOL bSelected, BOOL bPressed, BOOL bActive) = 0;


	virtual CSize GetCaptionButtonSize(UINT nId) const;

	virtual CRect GetCaptionButtonMargin() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves height of caption.
	// Returns:
	//     Frame Caption's height.
	//-----------------------------------------------------------------------
	virtual int GetFrameCaptionHeight() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the manager.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics() = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to get CXTPResourceImages pointer.
	// Returns:
	//     Pointer to image collection of the class.
	//-----------------------------------------------------------------------
	CXTPResourceImages* GetImages() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw frame caption text
	// Parameters:
	//     pDC - Pointer to device context
	//     rcCaptionText - Bounding rectangle of window text
	//     pSite - Frame to draw
	//     bActive - TRUE if frame is active
	//-----------------------------------------------------------------------
	virtual void DrawCaptionText(CDC *pDC, CRect rcCaptionText, CWnd *pSite, BOOL bActive);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw frame caption bar.
	// Parameters:
	//     pDC - Pointer to device context
	//     rcCaptionBar - Caption bar rectangle
	//     pSite - Frame to draw
	//     bActive - TRUE if frame is active
	//-----------------------------------------------------------------------
	virtual void DrawCaptionBar(CDC *pDC, CRect rcCaptionBar, CWnd *pSite, BOOL bActive);

	//-----------------------------------------------------------------------
	// Summary:
	//     Loads a bitmap image from the specified path.
	// Parameters:
	//     lpszFileName - The name of the file that contains the bitmaps
	//                    to load.
	// Remarks:
	//     The images are the bitmaps that represent all the visual components
	//     of the Ribbon Bar.  For example tab buttons, group buttons, menu buttons,
	//     toolbar buttons, option button, toolbar dropdown, etc.
	//     The images are loaded using LoadImage and are stored in the m_pImages
	//     image collection.
	//     Images for the Office 2007 theme can be found in the \Source\Ribbon\Res
	//     folder.
	// Returns: New CXTPResourceImage containing the loaded bitmap.
	// See Also: SetImageHandle, m_pImages
	//-----------------------------------------------------------------------
	CXTPResourceImage* LoadImage(LPCTSTR lpszFileName);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves frame small icon.
	// Parameters:
	//     pFrame - Window to retrieve icon.
	// Returns:
	//     Frame small icon.
	// See Also: GetFrameLargeIcon
	//-----------------------------------------------------------------------
	HICON GetFrameSmallIcon(CWnd* pFrame);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves frame large icon.
	// Parameters:
	//     pFrame - Window to retrieve icon.
	// Returns:
	//     Frame large icon.
	// See Also: GetFrameLargeIcon
	//-----------------------------------------------------------------------
	HICON GetFrameLargeIcon(CWnd* pFrame);


public:
	BOOL m_bFrameStatusBar;                    // TRUE if status bar used to draw.
	BOOL m_bRoundedCornersAlways;

	CXTPPaintManagerFont m_fntFrameCaption;    // Font used to draw text in the frame caption

public:
	int m_nFrameCaptionHeight;                 // Frame caption

	CXTPPaintManager *m_pPaintManager;         // Parent Paint Manager.

	BOOL m_bFlatFrame;                         // TRUE to use a flat frame, FALSE otherwise.

protected:

	HICON m_hIcon;         // Icon handle
	CSize m_szIcon;        // Icon size
	CRect m_rcIconMargin;  // Icon margin

	COLORREF m_clrFrameCaptionActive;           // Caption color (active)
	COLORREF m_clrFrameCaptionInactive;         // Caption color (inactive)

	COLORREF m_clrFrameCaptionTextActive;       // Caption text color
	COLORREF m_clrFrameCaptionTextInactive;     // Caption text color
	COLORREF m_clrFrameCaptionTextActiveTitle;  // Caption text color

	BOOL m_bCaptionCenterAligned;               // If TRUE, caption title will be center aligned.
};

AFX_INLINE int CXTPFramePaintManager::GetFrameCaptionHeight() const
{
	return m_nFrameCaptionHeight;
}

AFX_INLINE CRect CXTPFramePaintManager::GetCaptionButtonMargin() const
{
	return CRect(0, 0, 0, 0);
}
