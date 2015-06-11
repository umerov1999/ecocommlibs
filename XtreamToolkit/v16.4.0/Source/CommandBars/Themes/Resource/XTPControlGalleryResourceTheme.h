//===========================================================================
// Summary:
//     The CXTPControlGalleryResourceTheme class is used to enable a Gallery Office 2007 style theme for Command Bars.
// See Also: CXTPPaintManager::SetTheme
//===========================================================================
class _XTP_EXT_CLASS CXTPControlGalleryResourceTheme : public CXTPControlGalleryPaintManager
{
public:
	//-----------------------------------------------------------------------
	// Summary: Constructs a CXTPControlGalleryResourceTheme object.
	// Input:   pPaintManager - Reference to a CXTPPaintManager object.
	//-----------------------------------------------------------------------
	CXTPControlGalleryResourceTheme(CXTPPaintManager* pPaintManager);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw background of the gallery
	// Parameters:
	//      pDC - Pointer to device context
	//      pGallery - CXTPControlGallery object to draw
	//      rc - Bounding rectangle of gallery object
	//-----------------------------------------------------------------------
	virtual void FillControl(CDC* pDC, CXTPControlGallery* pGallery, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method called to draw scrollbar of inplace gallery
	// Parameters:
	//     pDC - Pointer to device context
	//     pGallery - CXTPControlGallery object to draw
	// See Also: DrawScrollBar
	//-----------------------------------------------------------------------
	virtual void DrawPopupScrollBar(CDC* pDC, CXTPControlGallery* pGallery);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw label control
	// Parameters:
	//     pDC - Pointer to device context
	//     pLabel - Label item
	//     rc - Bounding rectangle of the label
	//-----------------------------------------------------------------------
	virtual void DrawLabel(CDC* pDC, CXTPControlGalleryItem* pLabel, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the manager.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

protected:
	COLORREF m_clrControlGallerySelected;   // Selected item color.
	COLORREF m_clrControlGalleryNormal;     // Normal item color.
	COLORREF m_clrControlGalleryBorder;     // Border color.

};
