// XTPChartLegend.h
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
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
#if !defined(__XTPCHARTLEGEND_H__)
#define __XTPCHARTLEGEND_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CXTPChartFont;
class CXTPChartElementView;
class CXTPChartDeviceContext;
class CXTPChartLegend;
class CXTPChartLegendView;
class CXTPChartDeviceContext;
class CXTPChartDeviceCommand;
class CXTPChartBorder;
class CXTPChartFillStyle;
class CXTPPropExchange;


//===========================================================================
// Summary:
//     Enumerates the legend orientation.
// Remarks:
//===========================================================================
enum XTPChartLegendDirection
{
	xtpChartLegendTopToBottom,       //The legend items are arranged from top to bottom.
	xtpChartLegendLeftToRight,       //The legend items are arranged from left to right.
};

//===========================================================================
// Summary:
//     Enumerates the legend alignment.
// Remarks:
//===========================================================================
enum XTPChartLegendAlignment
{
	xtpChartLegendNearOutside,       //The legend is placed out side of the chart, near the primary Y axis.
	xtpChartLegendNear,              //The legend is placed inside of the chart, near the primary Y axis.
	xtpChartLegendCenter,            //The legend is placed at the center of the chart.
	xtpChartLegendFar,               //The legend is placed inside of the chart, near the secondary Y axis.
	xtpChartLegendFarOutside,        //The legend is placed outside of the chart, near the secondary Y axis.
};

//===========================================================================
// Summary:
//     This class represents a chart legend as a kind of chart element.
//     It abstracts all the features of a chart legend.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartLegend : public CXTPChartTextElement
{
	DECLARE_DYNAMIC(CXTPChartLegend)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartLegend object.
	// Parameters:
	//     pOwner     - Pointer to a CXTPChartContent object, which owns this legend.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartLegend(CXTPChartContent* pOwner);

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartLegend object, handles cleanup
	//-------------------------------------------------------------------------
	virtual ~CXTPChartLegend();

public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to to see whether the legend is visible.
	// Returns:
	//     Returns a BOOL value , TRUE if the legend is visible,
	//     FALSE if the legend is invisible.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	BOOL IsVisible() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to set the legend visible/invisible.
	// Parameters:
	//     bVisible    - A BOOL value TRUE to set the legend visible and
	//                   FALSE to hide the legend.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	void SetVisible(BOOL bVisible);

	//-------------------------------------------------------------------------
	// Summary:
	//     This function returns the CXTPChartFont object pointer associated with
	//     the CXTPChartLegend object.
	// Returns:
	//     Returns a pointer to CXTPChartFont
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartFont* GetFont() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function, to set the font for the legend
	// Parameters:
	//     pFont    - A CXTPChartFont object, represents the type of font.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	void SetFont(CXTPChartFont* pFont);

	//-------------------------------------------------------------------------
	// Summary:
	//     This function returns whether the marker is visible.
	// Returns:
	//     Returns a BOOL value , TRUE if the marker is visible,
	//     FALSE if the marker is invisible.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	BOOL IsMarkerVisible() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to set the marker visible/invisible.
	// Parameters:
	//     bVisible    - A BOOL value TRUE to set the marker visible and
	//                   FALSE to hide the marker.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	void SetMarkerVisible(BOOL bVisible);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the marker size(width and height).
	// Returns:
	//     Returns a CSize object, which gives the size of the marker.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CSize GetMarkerSize() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to set the marker size.
	// Parameters:
	//     sz    - A CSize object representing the width and height
	//             of the marker.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	void SetMarkerSize(CSize sz);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to set the text color of the legend.
	// Parameters:
	//     clrTextColor    - A CXTPChartColor object representing the ARGB value
	//                       of the color of choice.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	void SetTextColor(const CXTPChartColor& clrTextColor);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the current text color.
	// Returns:
	//     Returns a CXTPChartColor object, which gives the ARGB value
	//     of the current text color.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartColor GetTextColor() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the actual text color.
	// Returns:
	//     Returns a CXTPChartColor object, which gives the ARGB value
	//     of the actual text color.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartColor GetActualTextColor() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to set the background color of the legend.
	// Parameters:
	//     clrBackgroundColor    - A CXTPChartColor object representing the ARGB value
	//                       of the color of choice.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	void SetBackgroundColor(const CXTPChartColor& clrBackgroundColor);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the current background color.
	// Returns:
	//     Returns a CXTPChartColor object, which gives the ARGB value
	//     of the current background color.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartColor GetBackgroundColor() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the actual background color of the legend.
	// Returns:
	//     Returns a CXTPChartColor object, which gives the ARGB value
	//     of the actual background color.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartColor GetActualBackgroundColor() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the actual border color of the legend.
	// Returns:
	//     Returns a CXTPChartColor object, which gives the ARGB value
	//     of the actual border color.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartColor GetActualBorderColor() const;

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

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the fill style.
	// Returns:
	//     Returns a pointer to CXTPChartFillStyle object which represents the
	//     fill style of the legend.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartFillStyle* GetFillStyle() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the legend border.
	// Returns:
	//     Returns a pointer to CXTPChartBorder object which represents the
	//     border of the legend.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartBorder* GetBorder() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to set the horizontal alignment of the legend.
	// Parameters:
	//     nAlignment    - An enumerated value representing the legend alignment.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	void SetHorizontalAlignment(XTPChartLegendAlignment nAlignment);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the legend's horizontal alignment.
	// Returns:
	//     Returns an enumerated value XTPChartLegendAlignment, which represents the
	//     the kind of horizontal alignment chosen.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	XTPChartLegendAlignment GetHorizontalAlignment() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to set the vertical alignment of the legend.
	// Parameters:
	//     nAlignment    - An enumerated value representing the legend alignment.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	void SetVerticalAlignment(XTPChartLegendAlignment nAlignment);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the legend's vertical alignment.
	// Returns:
	//     Returns an enumerated value XTPChartLegendAlignment, which represents the
	//     the kind of horizontal alignment chosen.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	XTPChartLegendAlignment GetVerticalAlignment() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to set the the legend direction, top to bottom or
	//     left to right.
	// Parameters:
	//     nAlignment    - An enumerated value representing the legend alignment.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	void SetDirection(XTPChartLegendDirection nDirection);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the legend's direction. Top to bottom or
	//     left to right.
	// Returns:
	//     Returns an enumerated value XTPChartLegendDirection, which represents the
	//     the direction of the legend.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	XTPChartLegendDirection GetDirection() const;

	void SetColumnCount(int nColumnCount);
	int GetColumnCount() const;

	void SetOffset(CPoint ptOffset);
	CPoint GetOffset() const;

public:

	//-------------------------------------------------------------------------
	// Summary:
	//     This function creates the view (CXTPChartLegendView)of the legend.
	// Parameters:
	//     pDC - The chart device context object pointer.
	//     pParentView - The parent view of the legend.
	// Returns:
	//     Returns an pointer to CXTPChartLegendView object newly created.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	virtual CXTPChartLegendView* CreateView(CXTPChartDeviceContext* pDC, CXTPChartElementView* pParentView);

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to Store/Load the properties of legend object
	//      using the specified data object.
	// Parameters:
	//     pPX - Source or destination CXTPPropExchange data object reference.
	// Remarks:
	//     This member function is used to store or load property data to or
	//     from a storage.
	//-----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

#ifdef _XTP_ACTIVEX
public:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartLegend);



	afx_msg OLE_COLOR OleGetTextColor();
	afx_msg void OleSetTextColor(OLE_COLOR clr);
	afx_msg OLE_COLOR OleGetBackgroundColor();
	afx_msg void OleSetBackgroundColor(OLE_COLOR clr);
	afx_msg int OleGetMarkerWidth();
	afx_msg void OleSetMarkerWidth(int sz);
	afx_msg int OleGetMarkerHeight();
	afx_msg void OleSetMarkerHeight(int sz);
	afx_msg LPDISPATCH OleGetFillStyle();
	afx_msg LPDISPATCH OleGetBorder();
	afx_msg int OleGetOffsetX();
	afx_msg int OleGetOffsetY();
	afx_msg void OleSetOffsetY(int val);
	afx_msg void OleSetOffsetX(int val);
	afx_msg LPDISPATCH CXTPChartLegend::OleGetFont();
//}}AFX_CODEJOCK_PRIVATE
#endif

public:

	BOOL m_bVisible;                                    //TRUE if the legend is visible.
	BOOL m_bAntialiasing;                               //TRUE if antialiasing is turned ON
	CXTPChartBorder* m_pBorder;                         //Pointer to chart border object.
	CXTPChartFillStyle* m_pFillStyle;                   //Pointer to chart fill style object.
	CXTPChartFont* m_pFont;                             //Pointer to font object.
	CSize m_szMarkerSize;                               //Size of the marker.

	CXTPChartColor m_clrBackgroundColor;                //Chart background color.
	CXTPChartColor m_clrTextColor;                      //Chart text color.
	BOOL m_bMarkerVisible;                              //TRUE if marker is visible.

	int m_nColumnCount;                                 // Column Count

	XTPChartLegendAlignment m_nHorizontalAlignment;     //Legend horizontal alignment.
	XTPChartLegendAlignment m_nVerticalAlignment;       //Legend vertical alignment.
	XTPChartLegendDirection m_nDirection;               //Legend orientation.

	CPoint m_ptOffset;


	friend class CXTPChartContent;
};

AFX_INLINE BOOL CXTPChartLegend::IsVisible() const
{
	return m_bVisible;
}

AFX_INLINE void CXTPChartLegend::SetVisible(BOOL bVisible)
{
	m_bVisible = bVisible;
	OnChartChanged();
}

AFX_INLINE CXTPChartFont* CXTPChartLegend::GetFont() const
{
	return m_pFont;
}

AFX_INLINE CSize CXTPChartLegend::GetMarkerSize() const
{
	return m_szMarkerSize;
}

AFX_INLINE void CXTPChartLegend::SetMarkerSize(CSize sz)
{
	m_szMarkerSize = sz;
	OnChartChanged();
}

AFX_INLINE BOOL CXTPChartLegend::IsMarkerVisible() const
{
	return m_bMarkerVisible;
}

AFX_INLINE void CXTPChartLegend::SetMarkerVisible(BOOL bVisible)
{
	m_bMarkerVisible = bVisible;
	OnChartChanged();
}

AFX_INLINE void CXTPChartLegend::SetAntialiasing(BOOL bAntialiasing)
{
	m_bAntialiasing = bAntialiasing;
	OnChartChanged();
}

AFX_INLINE BOOL CXTPChartLegend::GetAntialiasing() const
{
	return m_bAntialiasing;
}

AFX_INLINE CXTPChartFillStyle* CXTPChartLegend::GetFillStyle() const
{
	return m_pFillStyle;
}

AFX_INLINE CXTPChartBorder* CXTPChartLegend::GetBorder() const
{
	return m_pBorder;
}

AFX_INLINE void CXTPChartLegend::SetHorizontalAlignment(XTPChartLegendAlignment nAlignment)
{
	m_nHorizontalAlignment = nAlignment;
	OnChartChanged();
}

AFX_INLINE XTPChartLegendAlignment CXTPChartLegend::GetHorizontalAlignment() const
{
	return m_nHorizontalAlignment;
}

AFX_INLINE void CXTPChartLegend::SetVerticalAlignment(XTPChartLegendAlignment nAlignment)
{
	m_nVerticalAlignment = nAlignment;
	OnChartChanged();
}

AFX_INLINE XTPChartLegendAlignment CXTPChartLegend::GetVerticalAlignment() const
{
	return m_nVerticalAlignment;
}

AFX_INLINE void CXTPChartLegend::SetDirection(XTPChartLegendDirection nDirection)
{
	m_nDirection = nDirection;
	OnChartChanged();
}

AFX_INLINE XTPChartLegendDirection CXTPChartLegend::GetDirection() const
{
	return m_nDirection;
}

AFX_INLINE void CXTPChartLegend::SetColumnCount(int nColumnCount)
{
	m_nColumnCount = nColumnCount;
	OnChartChanged();
}

AFX_INLINE int CXTPChartLegend::GetColumnCount() const
{
	return m_nColumnCount;
}

AFX_INLINE void CXTPChartLegend::SetOffset(CPoint ptOffset)
{
	m_ptOffset = ptOffset;
	OnChartChanged();
}

AFX_INLINE CPoint CXTPChartLegend::GetOffset() const
{
	return m_ptOffset;
}

#endif //#if !defined(__XTPCHARTLEGEND_H__)
