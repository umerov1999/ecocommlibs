// XTPMarkupTextBlock.h: interface for the CXTPMarkupTextBlock class.
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
#if !defined(__XTPMARKUPTEXTBLOCK_H__)
#define __XTPMARKUPTEXTBLOCK_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPMarkupBrush;
class CXTPMarkupInlineCollection;
class CXTPMarkupInline;
class CXTPMarkupDrawingContext;
class CXTPMarkupRenderTransform;

//-----------------------------------------------------------------------
// Summary:
//     Text Trimming type
// Example:
//     <code>pTexBlock->SetTextTrimming(xtpMarkupTextTrimmingCharacterEllipsis)</code>
// See Also: CXTPMarkupTextBlock
//-----------------------------------------------------------------------
enum XTPMarkupTextTrimming
{
	xtpMarkupTextTrimmingNone,              // Drawing without trimming
	xtpMarkupTextTrimmingCharacterEllipsis, // Replaces part of the given string with ellipses, if necessary
	xtpMarkupTextTrimmingWordEllipsis,      // Replaces last word of the given string with ellipses, if necessary
};

//-----------------------------------------------------------------------
// Summary:
//     Text Wrapping type
// Example:
//     <code>pTexBlock->SetTextWrapping(xtpMarkupTextWrap)</code>
// See Also: CXTPMarkupTextBlock
//-----------------------------------------------------------------------
enum XTPMarkupTextWrapping
{
	xtpMarkupTextNoWrap,            // No line wrapping is performed.
	xtpMarkupTextWrap,              // Line-breaking occurs if the line overflows beyond the available block width, even if the standard line breaking algorithm cannot determine any line break opportunity
	xtpMarkupTextWrapWithOverflow   // Line-breaking occurs if the line overflows beyond the available block width. However, a line may overflow beyond the block width if the line breaking algorithm cannot determine a line break opportunity
};

//===========================================================================
// Summary: CXTPMarkupTextBlock is CXTPMarkupFrameworkElement derived class. It implements TextBlock XAML Tag.
//===========================================================================
class _XTP_EXT_CLASS CXTPMarkupTextBlock : public CXTPMarkupFrameworkElement
{
	DECLARE_MARKUPCLASS(CXTPMarkupTextBlock)

private:
	struct CLine;
	struct CLineIterator;
	struct CLinePart;
	class CLinePartList;
	class CLineList;

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPMarkupTextBlock object
	//-----------------------------------------------------------------------
	CXTPMarkupTextBlock();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPMarkupTextBlock object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPMarkupTextBlock();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set Text for TextBlock object
	// Parameters: lpszText - Next text to be set
	//-----------------------------------------------------------------------
	void SetText(LPCWSTR lpszText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get current Text of the TextBlock
	// Returns: Returns string contained text of TextBlock object
	//-----------------------------------------------------------------------
	CString GetText() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get poiner to collection of TextBlock's inline blocks
	// Returns: Returns pointer to CXTPMarkupInlineCollection contained TextBlock's inlines
	//-----------------------------------------------------------------------
	CXTPMarkupInlineCollection* GetInlines() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set background of the TextBlock object
	// Parameters: brush - New Brush to be set as background brush
	//-----------------------------------------------------------------------
	void SetBackground(CXTPMarkupBrush* brush);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get background brush of the TextBlock object
	// Returns: Pointer to CXTPMarkupBrush object contained current brush of TextBlock's object
	//-----------------------------------------------------------------------
	CXTPMarkupBrush* GetBackground() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set text brush of the TextBlock object
	// Parameters: brush - New Brush to be set as text brush
	//-----------------------------------------------------------------------
	void SetForeground(CXTPMarkupBrush* brush);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get text brush of the TextBlock object
	// Returns: Pointer to CXTPMarkupBrush object contained current text brush of TextBlock's object
	//-----------------------------------------------------------------------
	CXTPMarkupBrush* GetForeground() const;

	CXTPMarkupRenderTransform* GetRenderTransform() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method set text alignment of the TextBlock
	// Parameters: alignment - new alignment to be set
	// See Also: XTPMarkupTextAlignment, GetTextAlignment
	//-----------------------------------------------------------------------
	void SetTextAlignment(XTPMarkupTextAlignment alignment);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns current text alignment settings for TextBlock object
	// Returns: XTPMarkupTextAlignment enum with current alignment settings
	// See Also: XTPMarkupTextAlignment, SetTextAlignment
	//-----------------------------------------------------------------------
	XTPMarkupTextAlignment GetTextAlignment() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set how text will be wrapped inside bounding rectangle
	// Parameters: nTextWrapping - Text wrapping type
	// See Also: XTPMarkupTextWrapping, SetTextWrapping
	//-----------------------------------------------------------------------
	void SetTextWrapping(XTPMarkupTextWrapping nTextWrapping);
	void SetTextWrapping(BOOL bTextWrapping);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get text wrapping type
	// Returns: XTPMarkupTextWrapping text wrapping type
	// See Also: XTPMarkupTextWrapping, SetTextWrapping
	//-----------------------------------------------------------------------
	XTPMarkupTextWrapping GetTextWrapping() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the text trimming behavior to employ when content overflows the content area.
	// Parameters:
	//     bTextTrimming - New Text trimming style to be set
	// Remarks: The TextTrimming property has no effect unless the TextWrapping property is set to NoWrap
	//-----------------------------------------------------------------------
	void SetTextTrimming(XTPMarkupTextTrimming bTextTrimming);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the text trimming behavior to employ when content overflows the content area.
	// Returns:
	//     Current Text trimming type
	//-----------------------------------------------------------------------
	XTPMarkupTextTrimming GetTextTrimming() const;

	// ---------------------------------------------------------------------
	// Summary:
	//     Call this method to set a value that indicates the thickness of padding space between the boundaries of the content area,
	//     and the content displayed by a TextBlock
	// Parameters
	//     nLeft - Left space
	//     nTop - Top space
	//     nRight - Right space
	//     nBottom - Bottom space
	//     nPadding - All borders
	//-----------------------------------------------------------------------
	void SetPadding(int nLeft, int nTop, int nRight, int nBottom);
	void SetPadding(int nPadding); // <Combine CXTPMarkupTextBlock::SetPadding@int@int@int@int>

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets a value that indicates the thickness of padding space between the boundaries of the content area,
	//     and the content displayed by a TextBlock
	// Returns:
	//     CXTPMarkupThickness object contains padding space.
	// See Also: SetPadding
	//-----------------------------------------------------------------------
	CXTPMarkupThickness* GetPadding() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set font size for TextBlock object
	// Parameters
	//     nFontSize - New font size to be set
	//-----------------------------------------------------------------------
	void SetFontSize(int nFontSize);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves font size of the TextBlock
	// Returns
	//     Font size that was set
	//-----------------------------------------------------------------------
	int GetFontSize() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set font weight for TextBlock object
	// Parameters
	//     nFontWeight - New font weight to be set
	//-----------------------------------------------------------------------
	void SetFontWeight(int nFontWeight);


	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves font weight of the TextBlock
	// Returns
	//     Font weight that was set
	//-----------------------------------------------------------------------
	int GetFontWeight() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set typeface name of the font
	// Parameters
	//     lpszFontFamily - New typeface name to be set
	//-----------------------------------------------------------------------
	void SetFontFamily(LPCWSTR lpszFontFamily);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves typeface name of the font
	// Returns
	//     Typeface name of the font if was set, NULL otherwise
	//-----------------------------------------------------------------------
	LPCWSTR GetFontFamily() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set font style of the TextBlock
	// Parameters
	//     nFontStyle - New font style to be set.
	// Remarks:
	//      Italic (nFontStyle = 1) and Normal (FontStyle = 0) are supported.
	//-----------------------------------------------------------------------
	void SetFontStyle(int nFontStyle);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves font style of the TextBlock
	// Returns
	//     Font style that was set
	//-----------------------------------------------------------------------
	int GetFontStyle() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set text decoration of TextBlock
	// Parameters
	//     nTextDecorations - New font text decoration to be set.
	// Remarks:
	//      Underline (1) and Strikethrough(2) are supported.
	//-----------------------------------------------------------------------
	void SetTextDecorations(int nTextDecorations);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves font decoration of the TextBlock
	// Returns
	//     Font decoration that was set
	//-----------------------------------------------------------------------
	int GetTextDecorations() const;

//{{AFX_CODEJOCK_PRIVATE
// Internal methods

public:
	CXTPMarkupInline* GetFirstInline() const;

public:
	virtual void SetContentObject(CXTPMarkupBuilder* pBuilder, CXTPMarkupObject* pContent);
	virtual BOOL HasContentObject() const;

protected:
	virtual CSize MeasureOverride(CXTPMarkupDrawingContext* pDC, CSize szAvailableSize);
	virtual CSize ArrangeOverride(CSize szFinal);
	virtual void OnRender(CXTPMarkupDrawingContext* pDC);

	virtual BOOL AllowWhiteSpaceContent() const;
	virtual void OnPropertyChanged(CXTPMarkupDependencyProperty* pProperty, CXTPMarkupObject* pOldValue, CXTPMarkupObject* pNewValue);

	virtual int GetLogicalChildrenCount() const;
	virtual CXTPMarkupObject* GetLogicalChild(int nIndex) const;

	CXTPMarkupInputElement* InputHitTestOverride(CPoint point) const;

	void InvalidateMeasureOverride(CXTPMarkupDrawingContext* pDC);

//}}AFX_CODEJOCK_PRIVATE


private:
	void RemoveAllLines();

	void CloseLine(CLineIterator& li, int nWidth);
	void TrimLine(CLineIterator& li, int nWidth);
	void PushLinePart(CLineIterator& li, int nWidth);

	void CalculateLines(CXTPMarkupDrawingContext* pDC, int nWidth);

	void RenderTextDecorations(CXTPMarkupDrawingContext* pDC, CLinePart* part);
	int GetStackWidth(CLineIterator& li) const;

	void AddEmptyRun(CLineIterator& li);


protected:
	CXTPMarkupInlineCollection* m_pInlines; // Inline collection of text block

public:
	static CXTPMarkupDependencyProperty* m_pBackgroundProperty;
	static CXTPMarkupDependencyProperty* m_pForegroundProperty;
	static CXTPMarkupDependencyProperty* m_pTextWrappingProperty;
	static CXTPMarkupDependencyProperty* m_pTextTrimmingProperty;
	static CXTPMarkupDependencyProperty* m_pTextAlignmentProperty;
	static CXTPMarkupDependencyProperty* m_pTextDecorationsProperty;
	static CXTPMarkupDependencyProperty* m_pFontSizeProperty;
	static CXTPMarkupDependencyProperty* m_pFontWeightProperty;
	static CXTPMarkupDependencyProperty* m_pFontFamilyProperty;
	static CXTPMarkupDependencyProperty* m_pFontStyleProperty;
	static CXTPMarkupDependencyProperty* m_pPaddingProperty;
	static CXTPMarkupDependencyProperty* m_pTextProperty;
	static CXTPMarkupDependencyProperty* m_pFontQualityProperty;
	static CXTPMarkupDependencyProperty* m_pFontCharsetProperty;
	static CXTPMarkupDependencyProperty* m_pRenderTransformProperty;

private:
	CLineList* m_pLineList;
	int m_nLastWidth;
	CSize m_szBlockSize;
};

AFX_INLINE CXTPMarkupInlineCollection* CXTPMarkupTextBlock::GetInlines() const {
	return m_pInlines;
}
AFX_INLINE void CXTPMarkupTextBlock::SetTextWrapping(XTPMarkupTextWrapping nTextWrapping) {
	SetValue(m_pTextWrappingProperty, CXTPMarkupEnum::CreateValue(nTextWrapping));
}
AFX_INLINE void CXTPMarkupTextBlock::SetTextWrapping(BOOL bTextWrapping) {
	SetValue(m_pTextWrappingProperty, CXTPMarkupEnum::CreateValue(bTextWrapping ? xtpMarkupTextWrapWithOverflow : xtpMarkupTextNoWrap));
}
AFX_INLINE XTPMarkupTextWrapping CXTPMarkupTextBlock::GetTextWrapping() const {
	CXTPMarkupEnum* pTextWrapping = MARKUP_STATICCAST(CXTPMarkupEnum, GetValue(m_pTextWrappingProperty));
	return pTextWrapping != NULL ? (XTPMarkupTextWrapping)(int)*pTextWrapping : xtpMarkupTextNoWrap;
}
AFX_INLINE void CXTPMarkupTextBlock::SetTextTrimming(XTPMarkupTextTrimming bTextTrimming) {
	SetValue(m_pTextTrimmingProperty, CXTPMarkupEnum::CreateValue(bTextTrimming));
}
AFX_INLINE XTPMarkupTextTrimming CXTPMarkupTextBlock::GetTextTrimming() const {
	CXTPMarkupEnum* pTextTrimming = MARKUP_STATICCAST(CXTPMarkupEnum, GetValue(m_pTextTrimmingProperty));
	return pTextTrimming != NULL ? (XTPMarkupTextTrimming)(int)*pTextTrimming : (XTPMarkupTextTrimming)xtpMarkupTextTrimmingNone;
}
AFX_INLINE int CXTPMarkupTextBlock::GetLogicalChildrenCount() const {
	return m_pInlines != NULL ? 1 : 0;
}
AFX_INLINE CXTPMarkupObject* CXTPMarkupTextBlock::GetLogicalChild(int nIndex) const {
	return nIndex == 0 ? (CXTPMarkupObject*)m_pInlines : NULL;
}


#endif // !defined(__XTPMARKUPTEXTBLOCK_H__)
