// XTPReportView.h: interface for the CXTPReportView class.
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
#if !defined(__XTPREPORTVIEW_H__)
#define __XTPREPORTVIEW_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPReportViewPrintOptions;
class CXTPReportControl;
class CXTPReportPaintManager;

//===========================================================================
// Summary:
//     Identifier of report control child window of CXTPReportView.
// Example:
// <code>
// int CXTPReportView::OnCreate(LPCREATESTRUCT lpCreateStruct)
// {
//     if (CView::OnCreate(lpCreateStruct) == -1)
//         return -1;
//
//     if (!m_wndReport.Create(WS_CHILD | WS_TABSTOP | WS_VISIBLE | WM_VSCROLL,
//         CRect(0, 0, 0, 0), this, XTP_ID_REPORT_CONTROL))
//     {
//         TRACE(_T("Failed to create tab control window\n"));
//         return -1;
//     }
//     return 0;
// }
// </code>
// See Also:
//     XTP_NM_REPORT_CHECKED, XTP_NM_REPORT_COLUMNORDERCHANGED, XTP_NM_REPORT_HEADER_RCLICK,
//     XTP_NM_REPORT_HYPERLINK, XTP_NM_REPORT_INPLACEBUTTONDOWN, XTP_NM_REPORT_SELCHANGED
//     XTP_NM_REPORT_SORTORDERCHANGED, XTP_NM_REPORT_VALUECHANGED
//     CXTPReportView, CXTPReportControl, CXTPReportHeader
//===========================================================================
const UINT XTP_ID_REPORT_CONTROL = 100;


//===========================================================================
// Summary:
//     The CXTPReportView class provides an implementation of
//     the Report control as view class. Use GetReportCtrl method to access methods
//     of CXTPReportControl
// See Also: CXTPReportControl
//===========================================================================
class _XTP_EXT_CLASS CXTPReportView : public CView
{
	DECLARE_DYNCREATE(CXTPReportView)
protected:

	//-------------------------------------------------------------------------
	// Summary:
	//     Protected constructor used by dynamic creation
	//-------------------------------------------------------------------------
	CXTPReportView();

	BOOL m_bSwitchMode;     //Tells whether to switch between modes(icon and report)
	BOOL m_bShowRowNumber;  //TRUE to show the row number, FLASE else.
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to access Report control object.
	// Returns:
	//     Reference to child CXTPReportControl window
	//-----------------------------------------------------------------------
	virtual CXTPReportControl& GetReportCtrl() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set custom Report control object.
	// Parameters:
	//     pReport - Pointer to custom report object
	//-----------------------------------------------------------------------
	void SetReportCtrl(CXTPReportControl* pReport);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieved paint manager of the child report control.
	// Returns:
	//     Pointer to paint manager of report control.
	//-----------------------------------------------------------------------
	CXTPReportPaintManager* GetPaintManager() const;


	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to access print options for this view.
	// Returns:
	//     Pointer to an object describing print options of this view.
	//-----------------------------------------------------------------------
	CXTPReportViewPrintOptions* GetPrintOptions() const;
protected:

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPReportView object, handles cleanup and deallocation.
	//-------------------------------------------------------------------------
	virtual ~CXTPReportView();

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to print single page of report control.
	// Parameters:
	//     pDC         - Pointer to a device context for page output.
	//     pInfo       - Points to a CPrintInfo structure that describes the
	//                   current print job.
	//     rcPage      - Page bounding rectangle
	//     nIndexStart - First row to print
	// Remarks:
	//     This method prints page header, page footer and call PrintReport method.
	// Returns:
	//     Index of last printed row
	//-----------------------------------------------------------------------
	virtual int PrintPage(CDC* pDC, CPrintInfo* pInfo, CRect rcPage,
		int nRowFrom, int nColumnFrom, int nColumnTo);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to print report control (columns header and rows)
	//     on the page.
	// Parameters:
	//     pDC         - Pointer to a device context for page output.
	//     pInfo       - Points to a CPrintInfo structure that describes the current print job.
	//     rcPage      - Report bounding rectangle on the page
	//     nIndexStart - First row to print
	// Remarks:
	//     This method call PrintHeader, PrintRows methods.
	// Returns:
	//     Index of last printed row
	//-----------------------------------------------------------------------
	virtual int PrintReport(CDC *pDC, CPrintInfo *pInfo, CRect rcPage,
		int nRowFrom, int nColumnFrom, int nColumnTo);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to print header of each page.
	// Parameters:
	//     pDC      - Pointer to a device context for page output.
	//     rcHeader - Header bounding rectangle
	//-----------------------------------------------------------------------
	virtual void PrintHeader(CDC* pDC, CRect rcHeader);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to print footer of each page.
	// Parameters:
	//     pDC      - Pointer to a device context for page output.
	//     rcFooter - Header bounding rectangle
	//-----------------------------------------------------------------------
	virtual void PrintFooter(CDC* pDC, CRect rcFooter);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to print the page header of the report control
	// Parameters:
	//     pDC              - Print device context
	//     pInfo            - Points to a CPrintInfo structure that describes the current print job.
	//     rcPage           - Page print area
	//     bOnlyCalculate   - TRUE = only calculate
	//                        FALSE = calculate and print
	//     nPageNumber      - The printed page number
	//     nNumberOfPages   - The total number of printed pages
	//     nHorizontalPageNumber - The printed horizontal page number
	//     nNumberOfHorizontalPages - The total number of printed horizontal pages
	// Returns:
	//     The header height in device units.
	//-----------------------------------------------------------------------
	virtual int PrintPageHeader(CDC* pDC, CPrintInfo* pInfo, CRect rcPage, BOOL bOnlyCalculate, int nPageNumber, int nNumberOfPages, int nHorizontalPageNumber, int nNumberOfHorizontalPages);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to print the page footer of the report control
	// Parameters:
	//     pDC              - Print device context
	//     pInfo            - Points to a CPrintInfo structure that describes the current print job.
	//     rcPage           - Page print area
	//     bOnlyCalculate   - TRUE = only calculate
	//                        FALSE = calculate and print
	//     nPageNumber      - The printed page number
	//     nNumberOfPages   - The total number of printed pages
	//     nHorizontalPageNumber - The printed horizontal page number
	//     nNumberOfHorizontalPages - The total number of printed horizontal pages
	// Returns:
	//     The footer height in device units.
	//-----------------------------------------------------------------------
	virtual int PrintPageFooter(CDC* pDC, CPrintInfo* pInfo, CRect rcPage, BOOL bOnlyCalculate, int nPageNumber, int nNumberOfPages, int nHorizontalPageNumber, int nNumberOfHorizontalPages);

	//-----------------------------------------------------------------------
	// Summary:
	//      Use this member function to set external scroll bar control.
	// Parameters:
	//  pScrollBar - Pointer to a scrollbar object.
	//  bHor - orientation flag (TRUE for Horizontal)
	// See Also: CScrollBar
	//-----------------------------------------------------------------------
	void SetScrollBarCtrl(CScrollBar* pScrollBar, BOOL bHor = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//      Use this member function to set external CSliderCtrl.
	// Parameters:
	//  pSlider - Pointer to a CSliderCtrl object.
	void SetSliderCtrl(CSliderCtrl* pSlider);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method prints either header or footer divider.
	// Parameters:
	//     pDC  - Pointer to a device context for page output.
	//     rc   - Bounding rectangle of the divider.
	//     bHeaderRows - If TRUE, prints the header divider.
	// Returns:
	//     Height of printed rows.
	//-----------------------------------------------------------------------
	int PrintFixedRowsDivider(CDC* pDC, const CRect& rc, BOOL bHeaderRows);

	//-----------------------------------------------------------------------
	// Summary:
	//  Prepare array of column's start - column's end indexes and return this array's size
	// Parameters:
	//     pDC  - Pointer to a device context for page output.
	//     pInfo   - CPrintInfo*
	// Returns:
	//     int as number of columns
	//-----------------------------------------------------------------------
	int SetupStartCol(CDC* pDC, CPrintInfo* pInfo);

	//-----------------------------------------------------------------------
	// Summary:
	//  GetScrollBarCtrl
	// Parameters:
	//  nBar  - orientation flag
	// Returns:
	//  CScrollBar*
	//-----------------------------------------------------------------------
	CScrollBar* GetScrollBarCtrl(int nBar) const;

protected:
//{{AFX_CODEJOCK_PRIVATE

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPReportView)
	virtual BOOL PaginateTo(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL PreCreateWindow(CREATESTRUCT &cs);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnDraw(CDC* pDC);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPReportView)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnPaint();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnFilePageSetup();
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

protected:
	CBitmap m_bmpGrayDC;            // A temporary bitmap to convert colored report control image to 'gray' colors when printing in black&while mode.
	CXTPReportControl m_wndReport;  // Child report control window.
	CXTPReportControl* m_pReport;   // Child report control pointer.
	BOOL m_bPrintSelection;         // TRUE if only printing the currently selected rows in the report, FALSE if printing the entire report,

	CXTPReportViewPrintOptions* m_pPrintOptions; // Printing options.
	CScrollBar* m_pScrollBarVer;    // Store pointer to external vertical scrollbar control.
	CScrollBar* m_pScrollBarHor;    // Store pointer to external horizontal scrollbar control.

//{{AFX_CODEJOCK_PRIVATE
	CSliderCtrl* m_pSlider;

	friend class CReportControlCtrl;

	int m_nColumnStart;
	int m_nColumnEnd;
	UINT m_nStartIndex;

	CUIntArray m_aPageStart;
	BOOL m_bPaginated;

//}}AFX_CODEJOCK_PRIVATE

public:
	BOOL    m_bAllowCut;                // If TRUE the cut operation is allowed.
	BOOL    m_bAllowPaste;              // If TRUE the paste operation is allowed.
	BOOL    m_bPrintDirect;             // if TRUE - the Print Dialog is bypassed. FALSE by default.
	BOOL    m_bResizeControlWithView;   // if FALSE - attached Report control will not be resized with view. TRUE by default.
};

/////////////////////////////////////////////////////////////////////////////
AFX_INLINE CXTPReportViewPrintOptions* CXTPReportView::GetPrintOptions() const {
	return m_pPrintOptions;
}


#endif // !defined(__XTPREPORTVIEW_H__)
