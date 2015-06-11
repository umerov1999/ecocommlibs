// XTPReportRecordItemNumber.h: interface for the CXTPReportRecordItemNumber class.
//
// This file is a part of the XTREME REPORTCONTROL MFC class library.
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
#if !defined(__XTPREPORTRECORDITEMNUMBER_H__)
#define __XTPREPORTRECORDITEMNUMBER_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//===========================================================================
// Summary:
//     This class represents a numeric cell and inherits basic functionality
//     from CXTPReportRecordItem class.
// Remarks:
//     You create a number record item simply by calling constructor with
//     the only one parameter - its numeric value. There is an additional
//     constructor with second parameter - format string.
//     Format string allows you to determine exactly in which format
//     the numeric value will be represented on the screen.
//     It could be useful for displaying money values, etc.
// See Also: CXTPReportRecordItem
//===========================================================================
class _XTP_EXT_CLASS CXTPReportRecordItemNumber : public CXTPReportRecordItem
{
	DECLARE_SERIAL(CXTPReportRecordItemNumber)
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates a record numeric item with the specific formatting.
	// Parameters:
	//     dValue - Value of this numeric item.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPReportRecordItemNumber(double dValue = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates a record numeric item with the specific formatting.
	// Parameters:
	//     dValue - Value of this numeric item.
	//     strFormat - Format string for creating caption value of the item.
	// Remarks:
	//     If the format string is set, the value of the item will be formatted
	//     according to this format string before drawing it convenient
	//     in many cases (drawing date for example). Format string is C - like
	//     style, see sprintf() C function or CString.Format() member function
	//-----------------------------------------------------------------------
	CXTPReportRecordItemNumber(double dValue, LPCTSTR strFormat);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns a text caption of this item.
	// Parameters:
	//     pColumn - Corresponded column of the item.
	// Remarks:
	//     Overwrites parents' member function
	// Returns:
	//     Text caption of the item.
	//-----------------------------------------------------------------------
	CString GetCaption(CXTPReportColumn* pColumn);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns item value in its original format.
	// Returns:
	//     Item original numeric value.
	//-----------------------------------------------------------------------
	double GetValue();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set value of the item
	// Parameters:
	//     dValue - New item value
	//-----------------------------------------------------------------------
	void SetValue(double dValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when value of in-place edit control changed
	// Parameters:
	//     pItemArgs - Pointer to structure with items arguments.
	//     szText    - New in-place edit text.
	//-----------------------------------------------------------------------
	virtual void OnEditChanged(XTP_REPORTRECORDITEM_ARGS* pItemArgs, LPCTSTR szText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Compares this item with the provided one.
	// Parameters:
	//  pColumn - Pointer to a CXTPReportColumn object.
	//  pItem - An item to compare with the current.
	// Remarks:
	//     Overwrites parents' member function
	// Returns:
	//     Zero if pItem is equal with current;
	//     Less than zero if less than pItem;
	//     Greater than zero if greater than pItem.
	//-----------------------------------------------------------------------
	int Compare(CXTPReportColumn* pColumn, CXTPReportRecordItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to Store/Load a report record item
	//     using the specified data object.
	// Parameters:
	//     pPX - Source or destination CXTPPropExchange data object reference.
	//-----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

protected:
	double m_dValue;    // Cell value.
};

//===========================================================================
// Summary:
//     This class represents a numeric percent cell and inherits basic functionality
//     from CXTPReportRecordItemNumber class.
// See Also: CXTPReportRecordItemNumber
//===========================================================================
class _XTP_EXT_CLASS CXTPReportRecordItemPercentNumber : public CXTPReportRecordItemNumber
{
	DECLARE_SERIAL(CXTPReportRecordItemPercentNumber)
public:

	//-----------------------------------------------------------------------
	// Summary:
	//  Call this member function to Store/Load a report record item
	//  using the specified data object.
	//  c-tr can pass value and color to draw like task manager percent complete field
	// Parameters:
	//  dValue - double value
	//  clr - color
	//  bPercentCompleteDisplay - show as colored rect inside
	//-----------------------------------------------------------------------
	CXTPReportRecordItemPercentNumber(double dValue = 0, COLORREF clr = RGB(224,224,224), BOOL bPercentCompleteDisplay = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Should be overridden by descendants for drawing itself.
	// Parameters:
	//     pDrawArgs - structure which contain drawing arguments.
	//     pMetrics - structure which contain metrics of the item.
	//-----------------------------------------------------------------------
	void OnDrawCaption(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs,
		XTP_REPORTRECORDITEM_METRICS* pMetrics);

public:
	COLORREF m_clr; //color
	BOOL m_bPercentCompleteDisplay; //flag
};


AFX_INLINE void CXTPReportRecordItemNumber::SetValue(double dValue)
{
	m_dValue = dValue;
}

AFX_INLINE double CXTPReportRecordItemNumber::GetValue()
{
	return m_dValue;
}

#endif // !defined(__XTPREPORTRECORDITEMNUMBER_H__)
