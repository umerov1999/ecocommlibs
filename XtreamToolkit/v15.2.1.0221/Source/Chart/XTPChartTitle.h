// XTPChartTitle.h
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
#if !defined(__XTPCHARTTITLE_H__)
#define __XTPCHARTTITLE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartDeviceContext;
class CXTPChartDeviceCommand;
class CXTPChartFont;
class CXTPChartElementView;
class CXTPChartContent;
class CXTPPropExchange;



enum XTPChartDocking
{
	xtpChartDockTop,
	xtpChartDockBottom,
	xtpChartDockLeft,
	xtpChartDockRight
};


//===========================================================================
// Summary:
//     This class represents a chart title.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartTitle : public CXTPChartTextElement
{
	DECLARE_DYNAMIC(CXTPChartTitle)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartTitle object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartTitle();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartTitle object, handles cleanup
	//-------------------------------------------------------------------------
	virtual ~CXTPChartTitle();

public:


	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to set the title visible/invisible.
	// Parameters:
	//     bVisible    - A BOOL value TRUE to set the title visible and
	//                   FALSE to hide the title.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	void SetVisible(BOOL bVisible);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to to see whether the title is visible.
	// Returns:
	//     Returns a BOOL value , TRUE if the title is visible,
	//     FALSE if the title is invisible.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	BOOL IsVisible() const;


	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to set text of the title
	// Parameters:
	//     lpszTitle    - New text of the title.
	// Remarks:
	// See Also: GetText
	//-------------------------------------------------------------------------
	void SetText(LPCTSTR lpszTitle);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get text of the title
	// Returns:
	//     Text of the title.
	// Remarks:
	// See Also: SetText
	//-------------------------------------------------------------------------
	CXTPChartString GetText() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to set custom text color of the title
	// Parameters:
	//     clrTextColor  - New text color of the title. Can be CXTPChartColor::Empty to use current Appearance color.
	// Remarks:
	// See Also: GetTextColor
	//-------------------------------------------------------------------------
	void SetTextColor(const CXTPChartColor& clrTextColor);


	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to get custom text color of the title
	// Returns:
	//     Text color of the title or CXTPChartColor::Empty if current Appearance color should be used.
	// Remarks:
	// See Also: SetTextColor
	//-------------------------------------------------------------------------
	CXTPChartColor GetTextColor() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to get text color of the title
	// Returns:
	//     Text color of the title
	// Remarks:
	// See Also: GetTextColor
	//-------------------------------------------------------------------------
	CXTPChartColor GetActualTextColor() const;


	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to set font for title
	// Parameters:
	//     pFont - New font to be set
	// Remarks:
	// See Also: GetFont
	//-------------------------------------------------------------------------
	void SetFont(CXTPChartFont* pFont);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to get font for title
	// Returns:
	//     Pointer to CXTPChartFont object that holds font information
	// Remarks:
	// See Also: SetFont
	//-------------------------------------------------------------------------
	CXTPChartFont* GetFont() const;


	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to set docking position of the title
	// Parameters:
	//     nDocking - docking position of title, it can be one of the following -
	//                xtpChartDockTop, xtpChartDockBottom, xtpChartDockLeft, xtpChartDockRight.
	// Remarks:
	// See Also: GetDocking
	//-------------------------------------------------------------------------
	void SetDocking(XTPChartDocking nDocking);


	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to get docking position of the title
	// Returns :
	//     Docking position of title, it can be one of the following -
	//                xtpChartDockTop, xtpChartDockBottom, xtpChartDockLeft, xtpChartDockRight.
	// Remarks:
	// See Also: SetDocking
	//-------------------------------------------------------------------------
	XTPChartDocking GetDocking() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to set indent between 2 titles
	// Parameters:
	//     nIndent - New indent to be set
	// Remarks:
	// See Also: GetIndent
	//-------------------------------------------------------------------------
	void SetIndent(int nIndent);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to get indent between 2 titles
	// Returns:
	//     Indent between 2 charts
	// Remarks:
	// See Also: GetIndent
	//-------------------------------------------------------------------------
	int GetIndent() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to set text alignment of the title
	// Parameters:
	//     nAlignment - New text alignment to be set.
	//                  It can be one of the following - xtpChartAlignNear, xtpChartAlignCenter, xtpChartAlignFar
	// Remarks:
	// See Also: GetAlignment
	//-------------------------------------------------------------------------
	void SetAlignment(XTPChartStringAlignment nAlignment);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to get text alignment of the title
	// Parameters:
	//     Text alignment of the title.
	//     It can be one of the following - xtpChartAlignNear, xtpChartAlignCenter, xtpChartAlignFar
	// Remarks:
	// See Also: SetAlignment
	//-------------------------------------------------------------------------
	XTPChartStringAlignment GetAlignment() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to set the antialiasing ON or OFF.Antialiasing is
	//     the smoothing of sharp edges of text and drawing.
	// Parameters:
	//     bAntialiasing - A BOOL value TRUE to set the antialiasing ON and
	//                   FALSE to OFF the anitialiasing operation.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	void SetAntialiasing(BOOL bAntialiasing);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get whether antialiasing is enabled or not.
	//     Antialiasing is the smoothing of sharp edges of text and drawing.
	// Returns:
	//     Returns boolean TRUE if the antialiasing is turned on FALSE if the
	//     antialiasing is turned off.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	BOOL GetAntialiasing() const;

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     This method is calculates text angle according current docking position
	// See Also: GetDocking
	//-------------------------------------------------------------------------
	int GetTextAngle() const;

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to Store/Load the properties of title object
	//      using the specified data object.
	// Parameters:
	//     pPX - Source or destination CXTPPropExchange data object reference.
	// Remarks:
	//     This member function is used to store or load property data to or
	//     from a storage.
	//-----------------------------------------------------------------------
	void DoPropExchange(CXTPPropExchange* pPX);

public:
//{{AFX_CODEJOCK_PRIVATE

	virtual CXTPChartElementView* CreateView(CXTPChartDeviceContext* pDC, CXTPChartElementView* pParentView);
	CPoint CalcOrigin(CRect bounds, CRect startBounds, CRect rect);
	XTPChartNearTextPosition GetNearTextPosition();
//}}AFX_CODEJOCK_PRIVATE


protected:
	friend class CXTPChartTitleCollection;

	CXTPChartString m_strText;      // Text of the title.
	CXTPChartColor m_clrTextColor;  // Text color
	BOOL m_bVisible;                // TRUE if title is visible
	CXTPChartFont* m_pFont;         // Font of the title
	BOOL m_bAntialiasing;           // Antialiasing flag
	XTPChartDocking m_nDocking;     // Current docking position of the title
	int m_nIndent;                  // Indent between 2 titles
	int m_nInnerIndent;             // Inner indent
	XTPChartStringAlignment m_nAlignment;   // Text alignment
};


//===========================================================================
// Summary:
//     CXTPChartTitleCollection represents collection of chart titles.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartTitleCollection : public CXTPChartElementCollection
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartTitleCollection object.
	// Parameters:
	//     pOwner - Owner object
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartTitleCollection(CXTPChartElement* pOwner);

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartTitleCollection object, handles cleanup
	//-------------------------------------------------------------------------
	virtual ~CXTPChartTitleCollection();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to add new title
	// Parameters:
	//     pTitle - New title to add
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartTitle* Add(CXTPChartTitle* pTitle);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get title by its index
	// Parameters:
	//     nIndex - Index of title to get
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartTitle* GetAt(int nIndex) const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to Store/Load the properties of title object
	//      using the specified data object.
	// Parameters:
	//     pPX - Source or destination CXTPPropExchange data object reference.
	// Remarks:
	//     This member function is used to store or load property data to or
	//     from a storage.
	//-----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

public:

//{{AFX_CODEJOCK_PRIVATE
	void CreateView(CXTPChartDeviceContext* pDC, CXTPChartElementView* pParentView);
	void CalculateView(CXTPChartDeviceContext* pDC, CRect& rcChart, CXTPChartElementView* pParentView);
//}}AFX_CODEJOCK_PRIVATE



protected:
	friend class CXTPChartContent;
};


AFX_INLINE CXTPChartTitle* CXTPChartTitleCollection::GetAt(int nIndex) const {
	return nIndex >= 0 && nIndex < m_arrElements.GetSize() ? (CXTPChartTitle*)m_arrElements.GetAt(nIndex) : NULL;
}


AFX_INLINE CXTPChartString CXTPChartTitle::GetText() const {
	return m_strText;
}
AFX_INLINE CXTPChartFont* CXTPChartTitle::GetFont() const {
	return m_pFont;
}
AFX_INLINE XTPChartDocking CXTPChartTitle::GetDocking() const {
	return m_nDocking;
}
AFX_INLINE int CXTPChartTitle::GetIndent() const {
	return m_nIndent;
}
AFX_INLINE XTPChartStringAlignment CXTPChartTitle::GetAlignment() const {
	return m_nAlignment;
}

AFX_INLINE void CXTPChartTitle::SetAntialiasing(BOOL bAntialiasing) {
	m_bAntialiasing = bAntialiasing;
	OnChartChanged();
}
AFX_INLINE  BOOL CXTPChartTitle::GetAntialiasing() const {
	return m_bAntialiasing;
}
AFX_INLINE  BOOL CXTPChartTitle::IsVisible() const {
	return m_bVisible;
}
AFX_INLINE  void CXTPChartTitle::SetVisible(BOOL bVisible) {
	m_bVisible = bVisible;
	OnChartChanged();
}

#endif //#if !defined(__XTPCHARTTITLE_H__)
