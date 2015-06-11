// XTPReportView.h: interface for the CXTPReportPageSetupDialog class.
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
#if !defined(__XTPREPORTPAGESETUPDIALOG_H__)
#define __XTPREPORTPAGESETUPDIALOG_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//-------------------------------------------------------------------------
// Summary:
//      This class used to store printing options.
// See Also: CXTPPrintOptions, CXTPPrintPageHeaderFooter
//-------------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPReportViewPrintOptions : public CXTPPrintOptions
{
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DYNAMIC(CXTPReportViewPrintOptions)
	//}}AFX_CODEJOCK_PRIVATE

public:

	//-------------------------------------------------------------------------
	// Summary: Default Object constructor.
	//-------------------------------------------------------------------------
	CXTPReportViewPrintOptions();

	//-----------------------------------------------------------------------
	// Summary:
	//      Override this member function in derived class to specify active
	//      locale ID.
	// Returns:
	//      Locale ID. Base implementation returns LOCALE_USER_DEFAULT;
	//-----------------------------------------------------------------------
	virtual LCID GetActiveLCID();

	//-----------------------------------------------------------------------
	// Summary:
	//      Call this member function to copy members from specified source object.
	// Parameters:
	//      pSrc - A pointer to source object to copy data.
	//-----------------------------------------------------------------------
	virtual void Set(const CXTPReportViewPrintOptions* pSrc);

	BOOL    m_bRepeatHeaderRows;    // If TRUE - the header rows will be printed on every page. FALSE by default.
	BOOL    m_bRepeatFooterRows;    // If TRUE - the footer rows will be printed on every page. FALSE by default.

protected:

//{{AFX_CODEJOCK_PRIVATE
#ifdef _XTP_ACTIVEX
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPReportViewPrintOptions);
#endif
//}}AFX_CODEJOCK_PRIVATE
};

//===========================================================================
// Summary:
//     Customized Report control page setup dialog.
// Remarks:
//     Use this class when you want to allow user to customize printing
//     options for the Report control.
//
//     This class enhances standard MFC page setup dialog, which encapsulates
//     the services provided by the Windows common OLE Page Setup dialog box
//     with additional support for setting and modifying print margins.
//     This class is designed to take the place of the Print Setup dialog box.
//
// See Also:
//     CXTPReportViewPrintOptions overview,
//     CPageSetupDialog overview
//===========================================================================
class _XTP_EXT_CLASS CXTPReportPageSetupDialog : public CPageSetupDialog
{
public:
	//{{AFX_CODEJOCK_PRIVATE
	enum {IDD = XTP_IDD_REPORT_PRINT_PAGE_SETUP};
	//}}AFX_CODEJOCK_PRIVATE

	//-----------------------------------------------------------------------
	// Summary:
	//     Class constructor.
	// Parameters:
	//     pOptions - A pointer to an associated CXTPReportViewPrintOptions
	//                object for storing user's interaction results.
	//     dwFlags  - One or more flags you can use to customize the settings
	//                of the dialog box. The values can be combined using the
	//                bitwise-OR operator. For more details see CPageSetupDialog::CPageSetupDialog
	//     pParentWnd - Pointer to the dialog box's parent or owner.
	// Remarks:
	//     Use the DoModal function to display the dialog box.
	// See also:
	//      CPageSetupDialog::CPageSetupDialog()
	//-----------------------------------------------------------------------
	CXTPReportPageSetupDialog(CXTPReportViewPrintOptions* pOptions,
		DWORD dwFlags = PSD_MARGINS | PSD_INWININIINTLMEASURE,
		CWnd* pParentWnd = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Default class constructor, handles members cleanup.
	//-----------------------------------------------------------------------
	virtual ~CXTPReportPageSetupDialog();

protected:
	CXTPReportViewPrintOptions* m_pOptions; // Pointer to associated print options object.

	//-----------------------------------------------------------------------
	// Summary:
	//     Converts font name to a human readable string.
	// Parameters:
	//     lfFont - A reference to a font.
	// Returns:
	//     A string describing a provided font.
	//-----------------------------------------------------------------------
	//CString FormatFontName(const LOGFONT& lfFont);

	//-----------------------------------------------------------------------
	// Summary:
	//     Dialog-box initialization.
	// Returns:
	//     FALSE because it has explicitly set the input focus to one of the
	//     controls in the dialog box.
	// See also:
	//     CPageSetupDialog::OnInitDialog().
	//-----------------------------------------------------------------------
	virtual BOOL OnInitDialog();

	//-----------------------------------------------------------------------
	// Summary:
	//     Called when the user clicks the OK button (the button with an ID of IDOK).
	// Remarks:
	//     Saves all values into m_pOptions structure.
	// See also:
	//     CPageSetupDialog::OnOK().
	//-----------------------------------------------------------------------
	virtual void OnOK();

	//{{AFX_CODEJOCK_PRIVATE
	CEdit       m_ctrlHeaderFormat;
	CEdit       m_ctrlFooterFormat;

	CButton     m_ctrlHeaderFormatBtn;
	CButton     m_ctrlFooterFormatBtn;

	afx_msg void OnBnClickedHeaderFormat();
	afx_msg void OnBnClickedFooterFormat();

	DECLARE_MESSAGE_MAP()
	//}}AFX_CODEJOCK_PRIVATE
};



#endif // !defined(__XTPREPORTPAGESETUPDIALOG_H__)
