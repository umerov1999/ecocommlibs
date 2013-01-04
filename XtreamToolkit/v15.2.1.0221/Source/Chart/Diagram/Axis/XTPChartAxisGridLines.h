// XTPChartAxisGridLines.h
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
#if !defined(__XTPCHARTAXISGRIDLINES_H__)
#define __XTPCHARTAXISGRIDLINES_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartAxis;
class CXTPChartLineStyle;

//===========================================================================
// Summary:
//     CXTPChartAxisGridLines is a kind of CXTPChartElement,
//     This class represents the grid lines parallel to an axis.
// Remarks:
//
//===========================================================================
class _XTP_EXT_CLASS CXTPChartAxisGridLines : public CXTPChartElement
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartAxisGridLines object.
	// Parameters:
	//     paxis - Pointer to a chart axis object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartAxisGridLines(CXTPChartAxis* pAxis);

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartAxisGridLines object, handles cleanup
	//-------------------------------------------------------------------------
	virtual ~CXTPChartAxisGridLines();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to know whether the grid lines are visible.
	// Returns:
	//     A boolean TRUE if the grid lines are visible, FALSE if the lines
	//     are invisible.
	// Remarks:
	//-----------------------------------------------------------------------
	BOOL IsVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to set the grid lines visible/invisible.
	// Parameters:
	//     bVisible - A boolean TRUE to make the grid lines visible, FALSE to make
	//                the lines invisible.
	// Remarks:
	//-----------------------------------------------------------------------
	void SetVisible(BOOL bVisible);


	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to know whether the minor grid lines are visible.
	// Returns:
	//     A boolean TRUE if the minor grid lines are visible, FALSE if the lines
	//     are invisible.
	// Remarks:
	//-----------------------------------------------------------------------
	BOOL IsMinorVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to set the minor grid lines visible/invisible.
	// Parameters:
	//     bMinorVisible - A boolean TRUE to make the minor grid lines visible, FALSE to
	//                make the lines invisible.
	// Remarks:
	//-----------------------------------------------------------------------
	void SetMinorVisible(BOOL bMinorVisible);

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to get the color of the grid lines.
	// Returns:
	//     A CXTPChartColor object representing the ARGB color value, selected.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartColor GetColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to get the color of the minor grid lines.
	// Returns:
	//     A CXTPChartColor object representing the ARGB color value, selected.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartColor GetMinorColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to get the grid line style used.
	// Returns:
	//     The pointer to a CXTPChartLineStyle object representing grid line style
	//     selected.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartLineStyle* GetLineStyle() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to get the minor grid line style used.
	// Returns:
	//     The pointer to a CXTPChartLineStyle object representing minor grid line
	//     style selected.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartLineStyle* GetMinorLineStyle() const;


public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the axis object to which the grid lines
	//     are associated.
	// Returns:
	//     The pointer to a CXTPChartAxis object representing a chart axis.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartAxis* GetAxis() const;


public:
	void DoPropExchange(CXTPPropExchange* pPX);

#ifdef _XTP_ACTIVEX
public:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartAxisGridLines);
	LPDISPATCH OleGetLineStyle();
	LPDISPATCH OleGetMinorLineStyle();
//}}AFX_CODEJOCK_PRIVATE
#endif


protected:
	BOOL m_bVisible;                        //TRUE if the grid lines are visible FALSE if not.
	BOOL m_bMinorVisible;                   //TRUE if the minor grid lines are visible FALSE if not.
	CXTPChartColor m_clrColor;               //ARGB value of the grid lines color.
	CXTPChartColor m_clrMinorColor;          //ARGB value of the minor grid lines color.

	CXTPChartLineStyle* m_pLineStyle;        //The grid line style.
	CXTPChartLineStyle* m_pMinorLineStyle;   //The minor grid line style.
};

AFX_INLINE CXTPChartAxis* CXTPChartAxisGridLines::GetAxis() const {
	return (CXTPChartAxis*)m_pOwner;
}
AFX_INLINE BOOL CXTPChartAxisGridLines::IsVisible() const {
	return m_bVisible;
}
AFX_INLINE BOOL CXTPChartAxisGridLines::IsMinorVisible() const {
	return m_bMinorVisible;
}
AFX_INLINE void CXTPChartAxisGridLines::SetVisible(BOOL bVisible) {
	m_bVisible = bVisible;
	OnChartChanged();
}
AFX_INLINE CXTPChartLineStyle* CXTPChartAxisGridLines::GetLineStyle() const {
	return m_pLineStyle;
}
AFX_INLINE CXTPChartLineStyle* CXTPChartAxisGridLines::GetMinorLineStyle() const {
	return m_pMinorLineStyle;
}
AFX_INLINE void CXTPChartAxisGridLines::SetMinorVisible(BOOL bMinorVisible) {
	m_bMinorVisible = bMinorVisible;
	OnChartChanged();
}


#endif //#if !defined(__XTPCHARTAXISGRIDLINES_H__)
