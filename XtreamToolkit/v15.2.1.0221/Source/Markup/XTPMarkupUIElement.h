// XTPMarkupUIElement.h: interface for the CXTPMarkupUIElement class.
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
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
#if !defined(__XTPMARKUPUIELEMENT_H__)
#define __XTPMARKUPUIELEMENT_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPMarkupContext;



//===========================================================================
// Summary: Provides rendering support for XML Markup, which includes hit testing,
//          coordinate transformation, and bounding box calculations.
//===========================================================================
class _XTP_EXT_CLASS CXTPMarkupVisual : public CXTPMarkupInputElement
{
	DECLARE_MARKUPCLASS(CXTPMarkupVisual);
public:

	//-------------------------------------------------------------------------
	// Summary: Provides the base initialization for objects derived from the
	//          CXTPMarkupVisual class.
	//-------------------------------------------------------------------------
	CXTPMarkupVisual();

public:
	//-------------------------------------------------------------------------
	// Summary: Call this method to draw Visual object inside drawing context.
	// Parameters: drawingContext - Drawing context to draw in.
	//-------------------------------------------------------------------------
	void Render(CXTPMarkupDrawingContext* drawingContext);

	//-----------------------------------------------------------------------
	// Input:   point - Point to test.
	// Summary: Detects if a point belongs to this object.
	// Returns: The CXTPMarkupInputElement at the specified point.
	//-----------------------------------------------------------------------
	CXTPMarkupInputElement* InputHitTest(CPoint point) const;

public:

	//-----------------------------------------------------------------------
	// Summary: Gets the number of child elements for the Visual.
	// Remarks: By default, a Visual has no children. Therefore, the default
	//          implementation always returns 0.
	// Returns: The number of child elements.
	//-----------------------------------------------------------------------
	virtual int GetVisualChildrenCount() const;

	//-----------------------------------------------------------------------
	// Input:   nIndex - The index of the visual object in the VisualCollection.
	// Summary: Returns the specified Visual in the parent VisualCollection.
	// Returns: The child in the VisualCollection at the specified index value.
	//-----------------------------------------------------------------------
	virtual CXTPMarkupVisual* GetVisualChild(int nIndex) const;


	//-----------------------------------------------------------------------
	// Summary: Number of child elements of object
	// Returns: The number of logical child elements of this object.
	//-----------------------------------------------------------------------
	virtual int GetLogicalChildrenCount() const;

	//-----------------------------------------------------------------------
	// Summary: Returns logical child elements of this object.
	// Parameters: nIndex - The index of the object in the logical objects tree.
	// Returns: Returns the specified logical child object in the parent collection
	// Remarks: LogicalChildren allows you to iterate over child elements.
	//          This is useful for elements that may not have a defined, dedicated collection but still contain more than one child element
	//-----------------------------------------------------------------------
	virtual CXTPMarkupObject* GetLogicalChild(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary: Returns bounding rectangle of the element
	// Returns: Bounding rectangle of the element
	//-----------------------------------------------------------------------
	CRect GetBoundRect() const;

	//-----------------------------------------------------------------------
	// Summary: Gets the final render size of this element.
	// Returns: The rendered size for this element.
	//-----------------------------------------------------------------------
	CSize GetRenderSize() const;

	//-----------------------------------------------------------------------
	// Summary: Gets or sets the offset value of the visual object.
	// Returns: The offset value of the visual object.
	//-----------------------------------------------------------------------
	CPoint GetVisualOffset() const;

	//-----------------------------------------------------------------------
	// Input:   rc - The available size provided by the element.
	// Summary: Returns an alternative clipping geometry.
	// Returns: The potential clipping geometry.
	//-----------------------------------------------------------------------
	virtual BOOL GetLayoutClip(CRect& rc) const;

	//-----------------------------------------------------------------------
	// Summary: Gets the visual tree parent of the visual object.
	// Returns: The Visual parent.
	//-----------------------------------------------------------------------
	CXTPMarkupVisual* GetVisualParent() const;

protected:
	//-----------------------------------------------------------------------
	// Summary: When overridden in a derived class, participates in rendering operations that are directed by the layout system
	// Parameters: drawingContext - Drawing context to draw in.
	//-----------------------------------------------------------------------
	virtual void OnRender(CXTPMarkupDrawingContext* drawingContext);

	//-----------------------------------------------------------------------
	// Summary: This method is called to render focus rectangle of focused object
	// Parameters: drawingContext - Drawing context to draw in.
	//-----------------------------------------------------------------------
	virtual void OnRenderFocusVisual(CXTPMarkupDrawingContext* drawingContext);

	//-----------------------------------------------------------------------
	// Summary: Override this method in derived class to provide visual hit testing of child object
	// Parameters: point - POisiotn of cursor to test.
	//-----------------------------------------------------------------------
	virtual CXTPMarkupInputElement* InputHitTestOverride(CPoint point) const;

	virtual void RenderClipped(CXTPMarkupDrawingContext* drawingContext);

//{{AFX_CODEJOCK_PRIVATE
	// Internal methods
protected:
	void UpdateBoundRect();
//}}AFX_CODEJOCK_PRIVATE



protected:
	CPoint m_ptVisualOffset;        // The offset value of the visual object.
	CSize m_szRenderSize;           // The rendered size for this element.
	CRect m_rcBoundRect;            // Bounding rectangle of the element
};


//===========================================================================
// Summary: Base class for XML Markup core level implementations.
//===========================================================================
class _XTP_EXT_CLASS CXTPMarkupUIElement : public CXTPMarkupVisual
{
	DECLARE_MARKUPCLASS(CXTPMarkupUIElement);

protected:

	//-------------------------------------------------------------------------
	// Summary: Initializes a new instance of the CXTPMarkupUIElement class.
	//-------------------------------------------------------------------------
	CXTPMarkupUIElement();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPMarkupUIElement object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPMarkupUIElement();

public:

	//-----------------------------------------------------------------------
	// Input:   rcFinalRect - The final size that the parent computes for the child
	//                  element, provided as a CRect instance.
	// Summary: Positions child elements and determines a size for a CXTPMarkupUIElement.
	//          Parent elements call this method from their ArrangeOverride implementation
	//          to form a recursive layout update. This method constitutes the
	//          second pass of a layout update.
	//-----------------------------------------------------------------------
	void Arrange(CRect rcFinalRect);

	//-----------------------------------------------------------------------
	// Input:   pDC - Pointer to a CXTPMarkupDrawingContext.
	//          szAvailableSize - The available space that a parent element can allocate
	//                  a child element. A child element can request a larger
	//                  space than what is available; the provided size might
	//                  be accommodated if scrolling is possible in the
	//                  content model for the current element.
	// Summary: Updates the DesiredSize of a CXTPMarkupUIElement. Parent elements call
	//          this method from their own MeasureOvveride implementations to form
	//          a recursive layout update. Calling this method constitutes
	//          the first pass (the "Measure" pass) of a layout update.
	//-----------------------------------------------------------------------
	void Measure(CXTPMarkupDrawingContext* pDC, CSize szAvailableSize);

public:

	//-----------------------------------------------------------------------
	// Summary: Gets the size that this element computed during the measure pass of the layout process.
	// Returns: The computed size, which becomes the desired size for the arrange pass.
	// Remarks: DesiredSize is typically checked as one of the measurement
	//          factors when you implement layout behavior overrides such
	//          as ArrangeOverride, MeasureOverride or OnRender.
	//-----------------------------------------------------------------------
	CSize GetDesiredSize() const;

	//-----------------------------------------------------------------------
	// Summary: Gets the final render size of this element.
	// Returns: The rendered size for this element.
	//-----------------------------------------------------------------------
	CRect GetFinalRect() const;

	//-----------------------------------------------------------------------
	// Summary: Gets the user interface (UI) visibility of this element. This
	//          is a dependency property.
	// Returns: XTPMarkupVisibility enumerator.
	//-----------------------------------------------------------------------
	XTPMarkupVisibility GetVisibility() const;

	//-----------------------------------------------------------------------
	// Input:   visibility - A value of the XTPMarkupVisibility enumeration. The default value is Visible.
	// Summary: Sets the user interface (UI) visibility of this element. This
	//          is a dependency property.
	//-----------------------------------------------------------------------
	void SetVisibility(XTPMarkupVisibility visibility);

	//-----------------------------------------------------------------------
	// Summary: Gets or sets a value that indicates whether the element can
	//          receive focus. This is a dependency property.
	// Returns: TRUE if the element is focusable; otherwise false. The
	//          default is false, but see Remarks.
	//-----------------------------------------------------------------------
	BOOL IsFocusable() const;

	//-----------------------------------------------------------------------
	// Summary: Gets a value indicating whether this element is visible in
	//          the user interface (UI). This is a dependency property.
	// Returns: true if the element is visible; otherwise, false.
	//-----------------------------------------------------------------------
	BOOL IsVisible() const;

	//-----------------------------------------------------------------------
	// Summary: Gets or sets a value indicating whether this element is
	//          enabled in the user interface (UI). This is a dependency property.
	// Returns: true if the element is enabled; otherwise, false. The
	//          default value is true.
	//-----------------------------------------------------------------------
	BOOL IsEnabled() const;

public:

	//-------------------------------------------------------------------------
	// Summary: Invalidates the arrange state (layout) for the element.
	// Remarks: Frequent calls to InvalidateArrange have significant performance
	//          consequences. Therefore, avoid calling this method unless you
	//          absolutely require precise layout state for subsequent calls to
	//          other APIs in your code.
	//-------------------------------------------------------------------------
	virtual void InvalidateArrange();

	//-------------------------------------------------------------------------
	// Summary: Invalidates the rendering of the element, and forces a complete
	//          new layout pass. OnRender is called after the layout cycle is completed.
	// Remarks: This method calls InvalidateArrange internally.
	//-------------------------------------------------------------------------
	virtual void InvalidateVisual();

	//-------------------------------------------------------------------------
	// Summary: Invalidates the measurement state (layout) for the element.
	// Remarks: Calling this method also calls InvalidateArrange internally,
	//          there is no need to call InvalidateMeasure and
	//          InvalidateArrange in succession.
	//-------------------------------------------------------------------------
	virtual void InvalidateMeasure();

protected:

	//-----------------------------------------------------------------------
	// Input:   rcFinalRect - The final area within the parent that element
	//                        should use to arrange itself and its child elements.
	// Summary: Defines the template for the core-level arrange layout definition.
	//-----------------------------------------------------------------------
	virtual void ArrangeCore(CRect rcFinalRect);

	//-----------------------------------------------------------------------
	// Input:   szAvailableSize - The available size that the parent element can
	//                 allocate for the child.
	//          pDC - Device context.
	// Summary: When overridden in a derived class, provides measurement logic
	//          for sizing this element properly, with consideration of the
	//          size of any child element content.
	// Returns: The desired size of this element in layout.
	//-----------------------------------------------------------------------
	virtual CSize MeasureCore(CXTPMarkupDrawingContext* pDC, CSize szAvailableSize);

	//-----------------------------------------------------------------------
	// Input:   rc - The available size provided by the element.
	// Summary: Returns an alternative clipping geometry that represents the
	//          region that would be clipped if m_pClipToBoundsProperty were set to true.
	// Returns: The potential clipping geometry.
	//-----------------------------------------------------------------------
	virtual BOOL GetLayoutClip(CRect& rc) const;

	//-----------------------------------------------------------------------
	// Summary: Override this method to provide custom reaction on InvalidateMeasure call.
	// Parameters: pDC - Device context to measure.
	//-----------------------------------------------------------------------
	virtual void InvalidateMeasureOverride(CXTPMarkupDrawingContext* pDC);


protected:
	CSize m_szDesiredSize;  // Gets the size that this element computed during the measure pass of the layout process.
	CRect m_rcFinalRect;    // Final position of visual element
	BOOL m_bMeasureDirty;   // TRUE if object haven't recalculate its size
	BOOL m_bNeverMeasured;  // TRUE if object never run Measure to update its size
	CSize m_szPreviousAvailableSize; // Previous available size of measure call
	BOOL m_bArrangeDirty;   // TRUE if object position can be invalid
	BOOL m_bNeverArranged;  // TRUE if Arrange never called for this object
	BOOL m_bArrangeInProgress; // TRUE if Arrange is currently executed.
	BOOL m_bMeasureInProgress; // TRUE if Measure is currently executed.

public:
	static CXTPMarkupDependencyProperty* m_pClipToBoundsProperty;  // Gets or sets a value indicating whether to
	                                                               // clip the content of this element (or content
	                                                               // coming from the child elements of this element)
	                                                               // to fit into the size of the containing element. This is a dependency property.
	static CXTPMarkupDependencyProperty* m_pVisibilityProperty;    // A value indicating whether this element is visible in the user interface (UI).
	static CXTPMarkupDependencyProperty* m_pFocusableProperty;     // A value that indicates whether the element can receive focus.
	static CXTPMarkupDependencyProperty* m_pIsEnabledProperty;     // A value indicating whether this element is enabled in the user interface (UI).
};


AFX_INLINE void CXTPMarkupVisual::OnRender(CXTPMarkupDrawingContext* /*drawingContext*/) {

}
AFX_INLINE void CXTPMarkupVisual::OnRenderFocusVisual(CXTPMarkupDrawingContext* /*drawingContext*/) {

}

AFX_INLINE CSize CXTPMarkupVisual::GetRenderSize() const {
	return m_szRenderSize;
}
AFX_INLINE CRect CXTPMarkupVisual::GetBoundRect() const {
	return m_rcBoundRect;
}
AFX_INLINE CPoint CXTPMarkupVisual::GetVisualOffset() const {
	return m_ptVisualOffset;
}
AFX_INLINE CSize CXTPMarkupUIElement::GetDesiredSize() const {
	return m_szDesiredSize;
}
AFX_INLINE CRect CXTPMarkupUIElement::GetFinalRect() const {
	return m_rcFinalRect;
}
AFX_INLINE XTPMarkupVisibility CXTPMarkupUIElement::GetVisibility() const {
	CXTPMarkupEnum* pValue =  MARKUP_STATICCAST(CXTPMarkupEnum, GetValue(m_pVisibilityProperty));
	return pValue ? (XTPMarkupVisibility)(int)*pValue : xtpMarkupVisibilityVisible;
}
AFX_INLINE void CXTPMarkupUIElement::SetVisibility(XTPMarkupVisibility visibility) {
	SetValue(m_pVisibilityProperty, CXTPMarkupEnum::CreateValue(visibility));
}
AFX_INLINE BOOL CXTPMarkupUIElement::IsFocusable() const {
	CXTPMarkupBool* pValue =  MARKUP_STATICCAST(CXTPMarkupBool, GetValue(m_pFocusableProperty));
	return pValue ? (BOOL)*pValue : FALSE;
}
AFX_INLINE BOOL CXTPMarkupUIElement::IsVisible() const {
	return GetVisibility() == xtpMarkupVisibilityVisible;
}
AFX_INLINE BOOL CXTPMarkupUIElement::IsEnabled() const {
	CXTPMarkupBool* pValue =  MARKUP_STATICCAST(CXTPMarkupBool, GetValue(m_pIsEnabledProperty));
	return pValue ? (BOOL)*pValue : TRUE;
}

#endif // !defined(__XTPMARKUPUIELEMENT_H__)
