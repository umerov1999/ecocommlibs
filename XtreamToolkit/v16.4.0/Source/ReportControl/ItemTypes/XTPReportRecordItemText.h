// XTPReportRecordItemText.h: interface for the CXTPReportRecordItemText class.
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
#if !defined(__XTPREPORTRECORDITEMTEXT_H__)
#define __XTPREPORTRECORDITEMTEXT_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//===========================================================================
// Summary:
//     This Class represents a text-only cell and inherits basic functionality
//     from CXTPReportRecordItem class.
//     You create a text record item simply by calling a constructor with one
//     parameter - text string
// See Also: CXTPReportRecordItem
//===========================================================================
class _XTP_EXT_CLASS CXTPReportRecordItemText : public CXTPReportRecordItem
{
	DECLARE_SERIAL(CXTPReportRecordItemText)
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates a record text item.
	// Parameters:
	//     szText - A text value for the item.
	//-----------------------------------------------------------------------
	CXTPReportRecordItemText(LPCTSTR szText = _T(""));

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
	//     Sets the text caption for the item.
	// Parameters:
	//     szText - Text caption for the item.
	//-----------------------------------------------------------------------
	void SetValue(LPCTSTR szText);

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
	//     Returns the item value in its original format.
	// Returns:
	//     Initial item text string.
	//-----------------------------------------------------------------------
	CString GetValue();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to Store/Load a report record item
	//     using the specified data object.
	// Parameters:
	//     pPX - Source or destination CXTPPropExchange data object reference.
	//-----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);


protected:

	CString m_strText;  // Item text value.
};

AFX_INLINE CString CXTPReportRecordItemText::GetValue()
{
	return m_strText;
}

#endif //#if !defined(__XTPREPORTRECORDITEMTEXT_H__)
