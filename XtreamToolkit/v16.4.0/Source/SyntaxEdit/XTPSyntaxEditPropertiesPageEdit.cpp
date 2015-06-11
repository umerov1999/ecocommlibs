// XTPSyntaxEditPropertiesPageEdit.cpp : implementation file
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// (c)1998-2013 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME SYNTAX EDIT LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
/////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Resource.h"

// common includes
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPVC80Helpers.h"
#include "Common/XTPColorManager.h"

#include "Common/XTPResourceManager.h"
#include "Common/XTPDrawHelpers.h"

// syntax editor includes
#include "XTPSyntaxEditDefines.h"
#include "XTPSyntaxEditStruct.h"
#include "XTPSyntaxEditCtrl.h"
#include "XTPSyntaxEditDoc.h"
#include "XTPSyntaxEditView.h"
#include "XTPSyntaxEditPaintManager.h"
#include "XTPSyntaxEditColorSampleText.h"
#include "XTPSyntaxEditColorComboBox.h"
#include "XTPSyntaxEditPropertiesPage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

//===========================================================================
// CXTPSyntaxEditPropertiesPageEdit property page
//===========================================================================

CXTPSyntaxEditPropertiesPageEdit::CXTPSyntaxEditPropertiesPageEdit(CXTPSyntaxEditView* pEditView/*=NULL*/)
: //CPropertyPage(CXTPSyntaxEditPropertiesPageEdit::IDD),
m_bModified(FALSE)
, m_pEditView(pEditView)
{
	ASSERT_VALID(m_pEditView); // must be valid.

	m_psp.dwFlags |= PSP_DLGINDIRECT;
	m_psp.pResource = XTPResourceManager()->LoadDialogTemplate(IDD);

	//{{AFX_DATA_INIT(CXTPSyntaxEditPropertiesPageEdit)
	m_bAutoReload = TRUE;
	m_bHorzScrollBar = TRUE;
	m_bVertScrollBar = TRUE;
	m_bSyntaxColor = TRUE;
	m_bAutoIndent = TRUE;
	m_bSelMargin = TRUE;
	m_bLineNumbers = TRUE;
	m_bVirtualSpace = TRUE;
	m_bCollapsibleNodes = TRUE;
	m_nCaretStyle = 0;
	m_nTabSize = 4;
	m_nTabType = 1;
	//}}AFX_DATA_INIT

	ReadRegistryValues();
}

CXTPSyntaxEditPropertiesPageEdit::~CXTPSyntaxEditPropertiesPageEdit()
{
}

void CXTPSyntaxEditPropertiesPageEdit::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXTPSyntaxEditPropertiesPageEdit)
	DDX_Check(pDX, XTP_IDC_EDIT_CHK_AUTO_RELOAD, m_bAutoReload);
	DDX_Check(pDX, XTP_IDC_EDIT_CHK_HORZ_SCROLLBAR, m_bHorzScrollBar);
	DDX_Check(pDX, XTP_IDC_EDIT_CHK_VERT_SCROLLBAR, m_bVertScrollBar);
	DDX_Check(pDX, XTP_IDC_EDIT_CHK_SYNTAX_COLOR, m_bSyntaxColor);
	DDX_Check(pDX, XTP_IDC_EDIT_CHK_AUTO_INDENT, m_bAutoIndent);
	DDX_Check(pDX, XTP_IDC_EDIT_CHK_SELMARGIN, m_bSelMargin);
	DDX_Check(pDX, XTP_IDC_EDIT_CHK_LINE_NUMBERS, m_bLineNumbers);
	DDX_Check(pDX, XTP_IDC_EDIT_CHK_VIRTUAL_SPACE, m_bVirtualSpace);
	DDX_Check(pDX, XTP_IDC_EDIT_CHK_COLLAPSIBLE_NODES, m_bCollapsibleNodes);

	DDX_Control(pDX, XTP_IDC_EDIT_RAD_TABS_SPACES, m_btnRadioSpaces);
	DDX_Control(pDX, XTP_IDC_EDIT_RAD_TABS_TAB, m_btnRadioTab);
	DDX_Control(pDX, XTP_IDC_EDIT_RAD_CARET_THIN, m_btnRadioCaretThin);
	DDX_Control(pDX, XTP_IDC_EDIT_RAD_CARET_THICK, m_btnRadioCaretThick);
	DDX_Text(pDX, XTP_IDC_EDIT_TABS_SIZE, m_nTabSize);
	DDV_MinMaxInt(pDX, m_nTabSize, 1, 64);
	//}}AFX_DATA_MAP
}

IMPLEMENT_DYNCREATE(CXTPSyntaxEditPropertiesPageEdit, CPropertyPage)

BEGIN_MESSAGE_MAP(CXTPSyntaxEditPropertiesPageEdit, CPropertyPage)
	//{{AFX_MSG_MAP(CXTPSyntaxEditPropertiesPageEdit)
	ON_BN_CLICKED(XTP_IDC_EDIT_CHK_AUTO_RELOAD, OnChkAutoReload)
	ON_BN_CLICKED(XTP_IDC_EDIT_CHK_HORZ_SCROLLBAR, OnChkHorzScrollBar)
	ON_BN_CLICKED(XTP_IDC_EDIT_CHK_VERT_SCROLLBAR, OnChkVertScrollBar)
	ON_BN_CLICKED(XTP_IDC_EDIT_CHK_SYNTAX_COLOR, OnChkSyntaxColor)
	ON_BN_CLICKED(XTP_IDC_EDIT_CHK_AUTO_INDENT, OnChkAutoIndent)
	ON_BN_CLICKED(XTP_IDC_EDIT_CHK_SELMARGIN, OnChkSelMargin)
	ON_BN_CLICKED(XTP_IDC_EDIT_CHK_LINE_NUMBERS, OnChkLineNumbers)
	ON_BN_CLICKED(XTP_IDC_EDIT_CHK_COLLAPSIBLE_NODES, OnChkLineNumbers)
	ON_BN_CLICKED(XTP_IDC_EDIT_CHK_VIRTUAL_SPACE, OnChkVirtualSpace)
	ON_BN_CLICKED(XTP_IDC_EDIT_RAD_TABS_SPACES, OnTabsSpaces)
	ON_BN_CLICKED(XTP_IDC_EDIT_RAD_TABS_TAB, OnTabsTab)
	ON_BN_CLICKED(XTP_IDC_EDIT_RAD_CARET_THIN, OnCaretThin)
	ON_BN_CLICKED(XTP_IDC_EDIT_RAD_CARET_THICK, OnCaretThick)
	ON_EN_CHANGE(XTP_IDC_EDIT_TABS_SIZE, OnChangeTabsSize)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CXTPSyntaxEditPropertiesPageEdit::ReadRegistryValues()
{
	if (!::IsWindow(m_pEditView->GetSafeHwnd()))
		return FALSE;

	CXTPSyntaxEditDoc* pEditDoc = m_pEditView->GetDocument();
	if (pEditDoc)
	{
		m_bAutoReload = pEditDoc->GetAutoReload();
	}

	CXTPSyntaxEditCtrl& wndEditCtrl = m_pEditView->GetEditCtrl();
	if (::IsWindow(wndEditCtrl.m_hWnd))
	{
		m_bVertScrollBar = wndEditCtrl.GetVertScrollBar();
		m_bHorzScrollBar = wndEditCtrl.GetHorzScrollBar();

		m_bSyntaxColor  = wndEditCtrl.GetSyntaxColor();
		m_bAutoIndent   = wndEditCtrl.GetAutoIndent();
		m_bSelMargin    = wndEditCtrl.GetSelMargin();
		m_bLineNumbers  = wndEditCtrl.GetLineNumbers();
		m_bVirtualSpace = wndEditCtrl.IsEnabledVirtualSpace();
		m_bCollapsibleNodes = wndEditCtrl.GetCollapsibleNodes();
		m_nCaretStyle   = wndEditCtrl.GetWideCaret();
		m_nTabSize      = wndEditCtrl.GetTabSize();
		m_nTabType      = wndEditCtrl.GetTabWithSpace() ? 0: 1;
	}

	return TRUE;
}

BOOL CXTPSyntaxEditPropertiesPageEdit::WriteRegistryValues()
{
	if (!::IsWindow(m_pEditView->GetSafeHwnd()))
		return FALSE;

	CXTPSyntaxEditDoc* pEditDoc = m_pEditView->GetDocument();
	if (pEditDoc)
	{
		pEditDoc->SetAutoReload(m_bAutoReload, TRUE);
	}

	CXTPSyntaxEditCtrl& wndEditCtrl = m_pEditView->GetEditCtrl();
	if (::IsWindow(wndEditCtrl.m_hWnd))
	{
		wndEditCtrl.SetScrollBars(m_bHorzScrollBar, m_bVertScrollBar, TRUE);

		wndEditCtrl.SetSyntaxColor(m_bSyntaxColor, TRUE);
		wndEditCtrl.SetAutoIndent(m_bAutoIndent, TRUE);
		wndEditCtrl.SetSelMargin(m_bSelMargin, TRUE);
		wndEditCtrl.SetLineNumbers(m_bLineNumbers, TRUE);
		wndEditCtrl.EnableVirtualSpace(m_bVirtualSpace, TRUE);
		wndEditCtrl.SetCollapsibleNodes(m_bCollapsibleNodes, TRUE);
		wndEditCtrl.SetWideCaret(m_nCaretStyle, TRUE);
		wndEditCtrl.SetTabSize(m_nTabSize, TRUE);
		wndEditCtrl.SetTabWithSpace(m_nTabType? FALSE: TRUE, TRUE);
	}

	wndEditCtrl.RestoreCursor();

	return TRUE;
}

void CXTPSyntaxEditPropertiesPageEdit::OnChkAutoReload()
{
	UpdateData();
	SetModified(TRUE);
}

void CXTPSyntaxEditPropertiesPageEdit::OnChkHorzScrollBar()
{
	UpdateData();
	SetModified(TRUE);
}

void CXTPSyntaxEditPropertiesPageEdit::OnChkVertScrollBar()
{
	UpdateData();
	SetModified(TRUE);
}

void CXTPSyntaxEditPropertiesPageEdit::OnChkSyntaxColor()
{
	UpdateData();
	SetModified(TRUE);
}

void CXTPSyntaxEditPropertiesPageEdit::OnChkAutoIndent()
{
	UpdateData();
	SetModified(TRUE);
}

void CXTPSyntaxEditPropertiesPageEdit::OnChkSelMargin()
{
	UpdateData();
	SetModified(TRUE);
}

void CXTPSyntaxEditPropertiesPageEdit::OnChkLineNumbers()
{
	UpdateData();
	SetModified(TRUE);
}

void CXTPSyntaxEditPropertiesPageEdit::OnChkVirtualSpace()
{
	UpdateData();
	SetModified(TRUE);
}

void CXTPSyntaxEditPropertiesPageEdit::OnChangeTabsSize()
{
	UpdateData();
	SetModified(TRUE);
}

void CXTPSyntaxEditPropertiesPageEdit::OnTabsSpaces()
{
	m_nTabType = 0;
	SetModified(TRUE);
}

void CXTPSyntaxEditPropertiesPageEdit::OnTabsTab()
{
	m_nTabType = 1;
	SetModified(TRUE);
}

void CXTPSyntaxEditPropertiesPageEdit::OnCaretThin()
{
	m_nCaretStyle = 0;
	SetModified(TRUE);
}

void CXTPSyntaxEditPropertiesPageEdit::OnCaretThick()
{
	m_nCaretStyle = 1;
	SetModified(TRUE);
}

BOOL CXTPSyntaxEditPropertiesPageEdit::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_btnRadioSpaces.SetCheck(m_nTabType == 0);
	m_btnRadioTab.SetCheck(m_nTabType == 1);
	m_btnRadioCaretThin.SetCheck(m_nCaretStyle == 0);
	m_btnRadioCaretThick.SetCheck(m_nCaretStyle == 1);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CXTPSyntaxEditPropertiesPageEdit::OnApply()
{
	if (!CPropertyPage::OnApply())
		return FALSE;

	// no changes made or they have already been applied.
	if (!m_bModified)
		return TRUE;

	// update registry settings.
	if (!WriteRegistryValues())
		return FALSE;

	m_bModified = FALSE;

	if (m_pEditView && m_pEditView->GetDocument())
		m_pEditView->GetDocument()->UpdateAllViews(NULL, xtpEditHintInitView);

	return TRUE;
}

void CXTPSyntaxEditPropertiesPageEdit::SetModified(BOOL bChanged/*=TRUE*/)
{
	CPropertyPage::SetModified();
	m_bModified = bChanged;
}
