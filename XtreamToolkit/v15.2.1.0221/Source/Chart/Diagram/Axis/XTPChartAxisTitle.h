// XTPChartAxisTitle.h
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
#if !defined(__XTPCHARTAXISTITLE_H__)
#define __XTPCHARTAXISTITLE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartAxis;

//===========================================================================
// Summary:
//     This class represents the axis title.This object is a kind of chart element.
// Remarks:
//
//===========================================================================
class _XTP_EXT_CLASS CXTPChartAxisTitle : public CXTPChartTextElement
{
	DECLARE_DYNAMIC(CXTPChartAxisTitle)
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartAxisTitle object.
	// Parameters:
	//     pAxis - A pointer to the chart axis object, to which the title is
	//             associated with.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartAxisTitle(CXTPChartAxis* pAxis);

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartAxisTitle object, handles cleanup
	//-------------------------------------------------------------------------
	virtual ~CXTPChartAxisTitle();

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     This function returns a pointer to the CXTPChartAxis object associated with
	//     the axis title object.
	// Returns:
	//     Returns a pointer to the associated chart axis object.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartAxis* GetAxis() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the axis text.
	// Parameters:
	//     lpszTitle - A string represents the axis title.
	// Remarks:
	//-----------------------------------------------------------------------
	void SetText(LPCTSTR lpszTitle);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the axis title string value.
	// Returns:
	//     Returns string represented by a CXTPChartString object..
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartString GetText() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the text color.
	// Parameters:
	//     clrTextColor - A string represents the axis title.
	// Remarks:
	//-----------------------------------------------------------------------
	void SetTextColor(const CXTPChartColor& clrTextColor);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the apparent text color of the axis title.
	// Returns:
	//     Returns CXTPChartColor object denoting the apparent text color in ARGB.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartColor GetTextColor() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the actual text color of the axis title.
	// Returns:
	//     Returns CXTPChartColor object denoting the actual text color in ARGB.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartColor GetActualTextColor() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the font of axis title.
	// Returns:
	//     A pointer to CXTPChartFont object denoting the font used.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartFont* GetFont() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the font of the title.
	// Parameters:
	//     pFont - A pointer to a chart font object.
	// Remarks:
	//-----------------------------------------------------------------------
	void SetFont(CXTPChartFont* pFont);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the alignment of the title.
	// Parameters:
	//     nAlignment - An enumerated value representing the title alignment.
	// Remarks:
	//     The normal alignments are near, far and center.
	//-----------------------------------------------------------------------
	void SetAlignment(XTPChartStringAlignment nAlignment);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the alignment of the axis title.
	// Returns:
	//     An enumerated value representing the title alignment.
	// Remarks:
	//     The normal alignments are near, far and center.
	// See Also:
	//-------------------------------------------------------------------------
	XTPChartStringAlignment GetAlignment() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to know whether the axis title is visible.
	// Returns:
	//     BOOL value of TRUE if the title is visible and FALSE if the title is not visible.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	BOOL IsVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the axis title visible.
	// Parameters:
	//     bVisible -  TRUE if the title is to set visible and FALSE to not.
	// Remarks:
	//-----------------------------------------------------------------------
	void SetVisible(BOOL bVisible);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to set the antialiasing ON or OFF.Antialiasing is
	//     the smooothing of sharp edges of text and drawing.
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
	//     Antialiasing is the smooothing of sharp edges of text and drawing.
	// Returns:
	//     Returns boolean TRUE if the antialiasing is turned on FALSE if the
	//     antialiasing is turned off.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	BOOL GetAntialiasing() const;

public:
	void DoPropExchange(CXTPPropExchange* pPX);

#ifdef _XTP_ACTIVEX
public:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartAxisTitle);

	BSTR OleGetText();
	void OleSetText(LPCTSTR lpszText);
	LPDISPATCH OleGetFont();
//}}AFX_CODEJOCK_PRIVATE
#endif

protected:
	CXTPChartString m_strText;               //The axis title value.
	CXTPChartColor m_clrTextColor;           //The text color.
	BOOL m_bVisible;                        //TRUE if the axis title is visible and FALSE if invisible.
	CXTPChartFont* m_pFont;                  //Pointer to the associated font object.
	BOOL m_bAntialiasing;                   //TRUE if the antialiasing ON and FALSE if antialiasing OFF.
	XTPChartStringAlignment m_nAlignment;    //The axis title alignment.

};

AFX_INLINE CXTPChartAxis* CXTPChartAxisTitle::GetAxis() const {
	return (CXTPChartAxis*)m_pOwner;
}

AFX_INLINE CXTPChartString CXTPChartAxisTitle::GetText() const {
	return m_strText;
}
AFX_INLINE CXTPChartFont* CXTPChartAxisTitle::GetFont() const {
	return m_pFont;
}
AFX_INLINE XTPChartStringAlignment CXTPChartAxisTitle::GetAlignment() const {
	return m_nAlignment;
}
AFX_INLINE BOOL CXTPChartAxisTitle::IsVisible() const {
	return m_bVisible;
}
AFX_INLINE void CXTPChartAxisTitle::SetAntialiasing(BOOL bAntialiasing) {
	m_bAntialiasing = bAntialiasing;
	OnChartChanged();
}
AFX_INLINE  BOOL CXTPChartAxisTitle::GetAntialiasing() const {
	return m_bAntialiasing;
}


#endif //#if !defined(__XTPCHARTAXISTITLE_H__)
