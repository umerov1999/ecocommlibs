// XTPReportRecordItemPreview.h: interface for the CXTPReportRecordItemPreview class.
//
// This file is a part of the XTREME REPORTCONTROL MFC class library.
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
#if !defined(__XTPREPORTRECORDITEMPREVIEW_H__)
#define __XTPREPORTRECORDITEMPREVIEW_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//===========================================================================
// Summary:
//     This represents a preview cell and inherits basic functionality
//     from CXTPReportRecordItem class.
// Remarks:
//     You create a CXTPReportRecordItemPreview record item simply by calling constructor with one
//     parameter - text string.
// See Also: CXTPReportRecordItem
//===========================================================================
class _XTP_EXT_CLASS CXTPReportRecordItemPreview : public CXTPReportRecordItem
{
	DECLARE_SERIAL(CXTPReportRecordItemPreview)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Creates a new preview record item.
	// Parameters:
	//     szPreviewText - Pointer to the preview text of item.
	//-----------------------------------------------------------------------
	CXTPReportRecordItemPreview(LPCTSTR szPreviewText = _T(""));

	//-----------------------------------------------------------------------
	// Summary:
	//     Set preview text
	// Parameters:
	//     strPreviewText - new preview text
	//-----------------------------------------------------------------------
	void SetPreviewText(LPCTSTR strPreviewText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns preview text
	// Returns:
	//     Text of preview item
	//-----------------------------------------------------------------------
	virtual CString GetPreviewText();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to determine preview area height.
	// Parameters:
	//     pDC    - Pointer to control drawing context.
	//     pRow   - Pointer to corresponded row.
	//     nWidth - Width of the row
	// Returns:
	//     Height of the preview area.
	//-----------------------------------------------------------------------
	virtual int GetPreviewHeight(CDC* pDC, CXTPReportRow* pRow, int nWidth);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns preview text
	// Parameters:
	//     pColumn - Corresponded column of the item.
	// Returns:
	//     Text of preview item
	//-----------------------------------------------------------------------
	CString GetCaption(CXTPReportColumn* pColumn);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets caption of the item.
	// Parameters:
	//     strCaption - Caption of item to set.
	//-----------------------------------------------------------------------
	virtual void SetCaption(LPCTSTR strCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Should be overridden by descendants for drawing itself.
	// Parameters:
	//     pDrawArgs - structure which contain drawing arguments.
	//     pMetrics - structure which contain metrics of the item.
	//-----------------------------------------------------------------------
	virtual void OnDrawCaption(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, XTP_REPORTRECORDITEM_METRICS* pMetrics);

	//-----------------------------------------------------------------------
	// Summary:
	//     Fills XTP_REPORTRECORDITEM_METRICS structure
	// Parameters:
	//     pDrawArgs    - Draw arguments for calculating item metrics.
	//     pItemMetrics - Pointer to the metrics item to fill with values.
	// Remarks:
	//     Calculates preview item metrics based on provided draw arguments.
	//     Overwrites parents' member function
	// See Also: XTP_REPORTRECORDITEM_DRAWARGS, XTP_REPORTRECORDITEM_METRICS
	//-----------------------------------------------------------------------
	virtual void GetItemMetrics(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, XTP_REPORTRECORDITEM_METRICS* pItemMetrics);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines of this item is a preview item.
	// Returns:
	//     TRUE for the preview item, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL IsPreviewItem() const;

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
	//     Retrieves caption text bounding rectangle
	// Parameters:
	//     pDrawArgs - structure which contain drawing arguments.
	//     rcItem    - Bounding rectangle of the item
	//-----------------------------------------------------------------------
	virtual void GetCaptionRect(XTP_REPORTRECORDITEM_ARGS* pDrawArgs, CRect& rcItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to Store/Load a report record item
	//     using the specified data object.
	// Parameters:
	//     pPX - Source or destination CXTPPropExchange data object reference.
	//-----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

protected:

	virtual void OnBeginEdit(XTP_REPORTRECORDITEM_ARGS *pItemArgs);

protected:
	CString m_strPreviewText;   // Preview content.
};


AFX_INLINE CString CXTPReportRecordItemPreview::GetPreviewText()
{
	return m_strPreviewText;
}

AFX_INLINE BOOL CXTPReportRecordItemPreview::IsPreviewItem() const
{
	return TRUE;
}

#endif // !defined(__XTPREPORTRECORDITEMPREVIEW_H__)
