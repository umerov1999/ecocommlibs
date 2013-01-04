// XTPSyntaxEditPropertiesPageFont.cpp : implementation file
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// (c)1998-2012 Codejock Software, All Rights Reserved.
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

AFX_STATIC int CALLBACK EnumFontFamExProc(
	ENUMLOGFONTEX *lpelfe,    // logical-font data
	NEWTEXTMETRICEX *lpntme,  // physical-font data
	DWORD FontType,           // type of font
	LPARAM lParam);           // application-defined data


//===========================================================================
// CXTPSyntaxEditPropertiesPageFont property page
//===========================================================================

CXTPSyntaxEditPropertiesPageFont::CXTPSyntaxEditPropertiesPageFont(CXTPSyntaxEditView* pEditView/*=NULL*/)
: //CPropertyPage(CXTPSyntaxEditPropertiesPageFont::IDD),
m_bModified(FALSE)
, m_pEditView(pEditView)
, m_uFaceSize(LF_FACESIZE)
{
	m_psp.dwFlags |= PSP_DLGINDIRECT;
	m_psp.pResource = XTPResourceManager()->LoadDialogTemplate(IDD);

	//{{AFX_DATA_INIT(CXTPSyntaxEditPropertiesPageFont)
	m_bStrikeOut = FALSE;
	m_bUnderline = FALSE;
	m_csName = _T("Courier");
	m_csStyle = _T("Regular");
	m_csSize = _T("10");
	//}}AFX_DATA_INIT

	m_iCharSet = DEFAULT_CHARSET;
	m_crHiliteText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
	m_crHiliteBack = ::GetSysColor(COLOR_HIGHLIGHT);
	m_crText = ::GetSysColor(COLOR_WINDOWTEXT);
	m_crBack = ::GetSysColor(COLOR_WINDOW);

	ReadRegistryValues();
}

CXTPSyntaxEditPropertiesPageFont::~CXTPSyntaxEditPropertiesPageFont()
{
}

void CXTPSyntaxEditPropertiesPageFont::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXTPSyntaxEditPropertiesPageFont)
	DDX_Control(pDX, XTP_IDC_EDIT_COMB_SCRIPT, m_wndComboScript);
	DDX_Control(pDX, XTP_IDC_EDIT_COMB_STYLES, m_wndComboStyle);
	DDX_Control(pDX, XTP_IDC_EDIT_COMB_SIZES, m_wndComboSize);
	DDX_Control(pDX, XTP_IDC_EDIT_COMB_NAMES, m_wndComboName);
/*  DDX_Check(pDX, XTP_IDC_EDIT_CHK_STRIKEOUT, m_bStrikeOut);*/
/*  DDX_Check(pDX, XTP_IDC_EDIT_CHK_UNDERLINE, m_bUnderline);*/
	DDX_Control(pDX, XTP_IDC_EDIT_COMB_TEXT, m_wndComboText);
	DDX_Control(pDX, XTP_IDC_EDIT_COMB_BACK, m_wndComboBack);
	DDX_Control(pDX, XTP_IDC_EDIT_COMB_TEXTSEL, m_wndComboHiliteText);
	DDX_Control(pDX, XTP_IDC_EDIT_COMB_BACKSEL, m_wndComboHiliteBack);
	DDX_Control(pDX, XTP_IDC_EDIT_TXT_SAMPLE, m_txtSample);
	DDX_Control(pDX, XTP_IDC_EDIT_TXT_SAMPLESEL, m_txtSampleSel);
	DDX_CBSyntaxColor(pDX, XTP_IDC_EDIT_COMB_TEXT, m_crText);
	DDX_CBSyntaxColor(pDX, XTP_IDC_EDIT_COMB_BACK, m_crBack);
	DDX_CBSyntaxColor(pDX, XTP_IDC_EDIT_COMB_TEXTSEL, m_crHiliteText);
	DDX_CBSyntaxColor(pDX, XTP_IDC_EDIT_COMB_BACKSEL, m_crHiliteBack);
	DDX_Control(pDX, XTP_IDC_EDIT_BTN_CUSTOM_TEXT, m_btnCustomText);
	DDX_Control(pDX, XTP_IDC_EDIT_BTN_CUSTOM_BACK, m_btnCustomBack);
	DDX_Control(pDX, XTP_IDC_EDIT_BTN_CUSTOM_TEXTSEL, m_btnCustomHiliteText);
	DDX_Control(pDX, XTP_IDC_EDIT_BTN_CUSTOM_BACKSEL, m_btnCustomHiliteBack);
	DDX_CBString(pDX, XTP_IDC_EDIT_COMB_STYLES, m_csStyle);
	DDX_CBString(pDX, XTP_IDC_EDIT_COMB_NAMES, m_csName);
	DDX_CBString(pDX, XTP_IDC_EDIT_COMB_SIZES, m_csSize);
	DDV_MaxChars(pDX, m_csSize, m_uFaceSize);
	//}}AFX_DATA_MAP
}

IMPLEMENT_DYNCREATE(CXTPSyntaxEditPropertiesPageFont, CPropertyPage)

BEGIN_MESSAGE_MAP(CXTPSyntaxEditPropertiesPageFont, CPropertyPage)
	//{{AFX_MSG_MAP(CXTPSyntaxEditPropertiesPageFont)
	ON_CBN_SELCHANGE(XTP_IDC_EDIT_COMB_NAMES, OnSelChangeComboNames)
	ON_CBN_SELCHANGE(XTP_IDC_EDIT_COMB_STYLES, OnSelChangeComboStyles)
	ON_CBN_SELCHANGE(XTP_IDC_EDIT_COMB_SIZES, OnSelChangeComboSizes)
/*  ON_BN_CLICKED(XTP_IDC_EDIT_CHK_STRIKEOUT, OnChkStrikeOut)*/
/*  ON_BN_CLICKED(XTP_IDC_EDIT_CHK_UNDERLINE, OnChkUnderline)*/
	ON_BN_CLICKED(XTP_IDC_EDIT_BTN_CUSTOM_TEXT, OnBtnCustomText)
	ON_BN_CLICKED(XTP_IDC_EDIT_BTN_CUSTOM_BACK, OnBtnCustomBack)
	ON_BN_CLICKED(XTP_IDC_EDIT_BTN_CUSTOM_TEXTSEL, OnBtnCustomHiliteText)
	ON_BN_CLICKED(XTP_IDC_EDIT_BTN_CUSTOM_BACKSEL, OnBtnCustomtHiliteBack)
	ON_CBN_SELENDOK(XTP_IDC_EDIT_COMB_TEXTSEL, OnSelEndOkHiliteText)
	ON_CBN_SELENDOK(XTP_IDC_EDIT_COMB_BACKSEL, OnSelEndOkHiliteBack)
	ON_CBN_SELENDOK(XTP_IDC_EDIT_COMB_TEXT, OnSelEndOkText)
	ON_CBN_SELENDOK(XTP_IDC_EDIT_COMB_BACK, OnSelEndOkBack)
	ON_CBN_SELENDOK(XTP_IDC_EDIT_COMB_SCRIPT, OnSelEndOkScript)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CXTPSyntaxEditPropertiesPageFont::CreateSafeFontIndirect(CFont& editFont, const LOGFONT& lf)
{
	if (editFont.GetSafeHandle())
		editFont.DeleteObject();

	if (!editFont.CreateFontIndirect(&lf))
		return FALSE;

	return TRUE;
}

BOOL CXTPSyntaxEditPropertiesPageFont::GetSafeLogFont(LOGFONT& lf)
{
	if (m_editFont.GetSafeHandle())
	{
		m_editFont.GetLogFont(&lf);
		return TRUE;
	}

	CFont* pFont = GetPaintManager() ? GetPaintManager()->GetFont() : NULL;
	if (pFont && pFont->GetSafeHandle())
	{
		pFont->GetLogFont(&lf);
		return TRUE;
	}

	return FALSE;
}

BOOL CXTPSyntaxEditPropertiesPageFont::ReadRegistryValues()
{
	LOGFONT lf = {0};
	if (!GetSafeLogFont(lf))
		return FALSE;

	if (!CreateSafeFontIndirect(m_editFont, lf))
		return FALSE;

	if (!GetPaintManager())
		return FALSE;

	m_crText        = GetPaintManager()->GetTextColor();
	m_crBack        = GetPaintManager()->GetBackColor();
	m_crHiliteText  = GetPaintManager()->GetHiliteTextColor();
	m_crHiliteBack  = GetPaintManager()->GetHiliteBackColor();
	m_bStrikeOut    = (BOOL)lf.lfStrikeOut;
	m_bUnderline    = (BOOL)lf.lfUnderline;

	return TRUE;
}

BOOL CXTPSyntaxEditPropertiesPageFont::WriteRegistryValues()
{
	// update paint manager font(paint manager font is set with CXTPSyntaxEditCtrl::CreateFontIndirect).
	LOGFONT lf = {0};
	if (!GetSafeLogFont(lf))
		return FALSE;

	if (!m_pEditView || !GetPaintManager())
		return FALSE;

	m_pEditView->SetFontIndirect(&lf, TRUE);

	// commit values to registry.
	GetPaintManager()->SetTextColor(m_crText, TRUE);
	GetPaintManager()->SetBackColor(m_crBack, TRUE);
	GetPaintManager()->SetHiliteTextColor(m_crHiliteText, TRUE);
	GetPaintManager()->SetHiliteBackColor(m_crHiliteBack, TRUE);

	return TRUE;
}

CXTPSyntaxEditPaintManager* CXTPSyntaxEditPropertiesPageFont::GetPaintManager()
{
	if (m_pEditView)
		return m_pEditView->GetEditCtrl().GetPaintManager();

	return NULL;
}

BOOL CXTPSyntaxEditPropertiesPageFont::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// Initialize combo boxes.
	InitFontCombo();
	InitStyleCombo();
	InitSizeCombo();
	InitScriptCombo();
	InitColorComboxes();

	UpdateSampleFont();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CXTPSyntaxEditPropertiesPageFont::InitFontCombo()
{
	m_wndComboName.ResetContent();

	LOGFONT lfEnum = {0};
	lfEnum.lfCharSet = DEFAULT_CHARSET;
	lfEnum.lfPitchAndFamily = /*FIXED_PITCH | */ FF_DONTCARE;
	lfEnum.lfFaceName[0] = 0;

	CWindowDC dc(NULL);
	::EnumFontFamiliesEx(dc.m_hDC, &lfEnum,
		(FONTENUMPROC)EnumFontFamExProc, (LPARAM)&m_wndComboName, 0 );

	if (!m_editFont.GetSafeHandle())
		return;

	LOGFONT lf = {0};
	if (!GetSafeLogFont(lf))
		return;

	m_csName = lf.lfFaceName;

	if (m_wndComboName.SelectString(-1, m_csName) == CB_ERR)
		m_wndComboName.SetWindowText(m_csName);
}

void CXTPSyntaxEditPropertiesPageFont::InitStyleCombo()
{
	m_wndComboStyle.ResetContent();

	CString csBuffer;
	XTPResourceManager()->LoadString(&csBuffer, XTP_IDS_EDIT_REGULAR);
	m_wndComboStyle.AddString(csBuffer);

	XTPResourceManager()->LoadString(&csBuffer, XTP_IDS_EDIT_BOLD);
	m_wndComboStyle.AddString(csBuffer);

	XTPResourceManager()->LoadString(&csBuffer, XTP_IDS_EDIT_ITALIC);
	m_wndComboStyle.AddString(csBuffer);

	XTPResourceManager()->LoadString(&csBuffer, XTP_IDS_EDIT_BOLDITALIC);
	m_wndComboStyle.AddString(csBuffer);

	if (!m_editFont.GetSafeHandle())
		return;

	LOGFONT lf = {0};
	if (!GetSafeLogFont(lf))
		return;

	if (lf.lfWeight >= FW_BOLD && lf.lfItalic)
	{
		XTPResourceManager()->LoadString(&m_csStyle, XTP_IDS_EDIT_BOLDITALIC);
	}
	else if (lf.lfItalic)
	{
		XTPResourceManager()->LoadString(&m_csStyle, XTP_IDS_EDIT_ITALIC);
	}
	else if (lf.lfWeight >= FW_BOLD)
	{
		XTPResourceManager()->LoadString(&m_csStyle, XTP_IDS_EDIT_BOLD);
	}
	else
	{
		XTPResourceManager()->LoadString(&m_csStyle, XTP_IDS_EDIT_REGULAR);
	}

	if (m_wndComboStyle.SelectString(-1, m_csStyle) == CB_ERR)
		m_wndComboStyle.SetWindowText(m_csStyle);
}

void CXTPSyntaxEditPropertiesPageFont::InitSizeCombo()
{
	m_wndComboSize.ResetContent();

	if (!m_editFont.GetSafeHandle())
		return;

	LOGFONT lf = {0};
	if (!GetSafeLogFont(lf))
		return;

	CWindowDC dc(NULL);
	::EnumFontFamiliesEx(dc.m_hDC, &lf,
		(FONTENUMPROC)EnumFontFamExProc, (LPARAM)&m_wndComboSize, 0);

	if (m_wndComboSize.GetCount() == 0)
	{
		TCHAR* szSize[] = {
				_T("8"),  _T("9"),  _T("10"), _T("11"),
				_T("12"), _T("14"), _T("16"), _T("18"),
				_T("20"), _T("22"), _T("24"), _T("26"),
				_T("28"), _T("36"), _T("48"), _T("72") };

			for (int i = 0; i < (sizeof(szSize)/sizeof(szSize[0])); i++)
			{
				m_wndComboSize.AddString(szSize[i]);
			}
	}

	m_csSize.Format(_T("%i"), -::MulDiv(lf.lfHeight,
		72, ::GetDeviceCaps(dc.m_hDC, LOGPIXELSY)));

	if (m_wndComboSize.SelectString(-1, m_csSize) == CB_ERR)
		m_wndComboSize.SetWindowText(m_csSize);
}

void CXTPSyntaxEditPropertiesPageFont::InitScriptCombo()
{
	m_wndComboScript.ResetContent();

	if (!m_editFont.GetSafeHandle())
		return;

	LOGFONT lf = {0};
	if (!GetSafeLogFont(lf))
		return;

	m_iCharSet = lf.lfCharSet;

	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfPitchAndFamily = FIXED_PITCH | FF_DONTCARE;

	CWindowDC dc(NULL);
	::EnumFontFamiliesEx(dc.m_hDC, &lf,
		(FONTENUMPROC)EnumFontFamExProc, (LPARAM)&m_wndComboScript, 0);

	int iCount = m_wndComboScript.GetCount();

	int i;
	for (i = 0; i < iCount; ++i)
	{
		BYTE lfCharSet = (BYTE)m_wndComboScript.GetItemData(i);
		if (lfCharSet == (BYTE)m_iCharSet)
		{
			m_wndComboScript.SetCurSel(i);
			break;
		}
	}

	if (i == iCount)
		m_wndComboScript.SetCurSel(0);
}

void CXTPSyntaxEditPropertiesPageFont::InitColorComboxes()
{
	ASSERT(GetPaintManager());
	if (!GetPaintManager())
		return;

	m_wndComboText.SetAutoColor(::GetSysColor(COLOR_WINDOWTEXT));
	m_crText = GetPaintManager()->GetTextColor();
	if (m_wndComboText.SetSelColor(m_crText) == CB_ERR)
	{
		m_wndComboText.SetUserColor(m_crText);
		m_wndComboText.SetSelColor(m_crText);
	}

	m_wndComboBack.SetAutoColor(::GetSysColor(COLOR_WINDOW));
	m_crBack = GetPaintManager()->GetBackColor();
	if (m_wndComboBack.SetSelColor(m_crBack) == CB_ERR)
	{
		m_wndComboBack.SetUserColor(m_crBack);
		m_wndComboBack.SetSelColor(m_crBack);
	}

	m_wndComboHiliteText.SetAutoColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
	m_crHiliteText = GetPaintManager()->GetHiliteTextColor();
	if (m_wndComboHiliteText.SetSelColor(m_crHiliteText) == CB_ERR)
	{
		m_wndComboHiliteText.SetUserColor(m_crHiliteText);
		m_wndComboHiliteText.SetSelColor(m_crHiliteText);
	}

	m_wndComboHiliteBack.SetAutoColor(::GetSysColor(COLOR_HIGHLIGHT));
	m_crHiliteBack = GetPaintManager()->GetHiliteBackColor();
	if (m_wndComboHiliteBack.SetSelColor(m_crHiliteBack) == CB_ERR)
	{
		m_wndComboHiliteBack.SetUserColor(m_crHiliteBack);
		m_wndComboHiliteBack.SetSelColor(m_crHiliteBack);
	}

	UpdateSampleColors();
}

void CXTPSyntaxEditPropertiesPageFont::UpdateSampleColors()
{
	m_txtSample.SetBackColor(m_crBack);
	m_txtSample.SetTextColor(m_crText);
	m_txtSampleSel.SetBackColor(m_crHiliteBack);
	m_txtSampleSel.SetTextColor(m_crHiliteText);
}

void CXTPSyntaxEditPropertiesPageFont::UpdateSampleFont()
{
	if (m_editFont.GetSafeHandle())
	{
		m_txtSample.SetFont(&m_editFont);
		m_txtSampleSel.SetFont(&m_editFont);
	}
}

int CXTPSyntaxEditPropertiesPageFont::GetLBText(CComboBox& comboBox, CString& csItemText)
{
	UpdateData();

	if (!::IsWindow(comboBox.m_hWnd))
		return CB_ERR;

	int iCurSel = comboBox.GetCurSel();
	if (iCurSel == CB_ERR)
		return CB_ERR;

	comboBox.GetLBText(iCurSel, csItemText);

	if (!csItemText.IsEmpty())
	{
		if (comboBox.SelectString(-1, csItemText) == CB_ERR)
			comboBox.SetWindowText(csItemText);

		return iCurSel;
	}

	return CB_ERR;
}

void CXTPSyntaxEditPropertiesPageFont::OnSelChangeComboNames()
{
	if (!m_wndComboName.GetListBox().SelChanged())
		return;

	if (GetLBText(m_wndComboName, m_csName) == CB_ERR)
		return;

	LOGFONT lf = {0};
	if (!GetSafeLogFont(lf))
		return;

	STRCPY_S(lf.lfFaceName, m_csName.GetLength() + 1, m_csName);

	if (CreateSafeFontIndirect(m_editFont, lf))
	{
		UpdateSampleFont();
		InitSizeCombo();
		InitScriptCombo();
		SetModified(TRUE);
	}
}

void CXTPSyntaxEditPropertiesPageFont::OnSelChangeComboStyles()
{
	if (!m_wndComboStyle.GetListBox().SelChanged())
		return;

	if (GetLBText(m_wndComboStyle, m_csStyle) == CB_ERR)
		return;

	LOGFONT lf = {0};
	if (!GetSafeLogFont(lf))
		return;

	CString csBuffer;
	XTPResourceManager()->LoadString(&csBuffer, XTP_IDS_EDIT_ITALIC);
	lf.lfItalic = (m_csStyle.Find(csBuffer) == -1)? (BYTE)FALSE: (BYTE)TRUE;

	XTPResourceManager()->LoadString(&csBuffer, XTP_IDS_EDIT_BOLD);
	lf.lfWeight = (m_csStyle.Find(csBuffer) == -1)? FW_NORMAL: FW_BOLD;

	if (CreateSafeFontIndirect(m_editFont, lf))
	{
		UpdateSampleFont();
		SetModified(TRUE);
	}
}

void CXTPSyntaxEditPropertiesPageFont::OnSelChangeComboSizes()
{
	if (!m_wndComboSize.GetListBox().SelChanged())
		return;

	if (GetLBText(m_wndComboSize, m_csSize) == CB_ERR)
		return;

	LOGFONT lf = {0};
	if (!GetSafeLogFont(lf))
		return;

	CWindowDC dc(NULL);
	lf.lfHeight = -MulDiv(_ttoi(m_csSize),
		::GetDeviceCaps(dc.m_hDC, LOGPIXELSY), 72);

	if (CreateSafeFontIndirect(m_editFont, lf))
	{
		UpdateSampleFont();
		SetModified(TRUE);
	}
}

void CXTPSyntaxEditPropertiesPageFont::OnChkStrikeOut()
{
	UpdateData();

	LOGFONT lf = {0};
	if (!GetSafeLogFont(lf))
		return;

	lf.lfStrikeOut = (BYTE)m_bStrikeOut;

	if (CreateSafeFontIndirect(m_editFont, lf))
	{
		UpdateSampleFont();
		SetModified(TRUE);
	}
}

void CXTPSyntaxEditPropertiesPageFont::OnChkUnderline()
{
	UpdateData();

	LOGFONT lf = {0};
	if (!GetSafeLogFont(lf))
		return;

	lf.lfUnderline = (BYTE)m_bUnderline;

	if (CreateSafeFontIndirect(m_editFont, lf))
	{
		UpdateSampleFont();
		SetModified(TRUE);
	}
}

void CXTPSyntaxEditPropertiesPageFont::OnSelEndOkScript()
{
	if (!m_wndComboScript.GetListBox().SelChanged())
		return;

	UpdateData();

	int iCurSel = m_wndComboScript.GetCurSel();
	if (iCurSel == CB_ERR)
		return;

	LOGFONT lf = {0};
	if (!GetSafeLogFont(lf))
		return;

	lf.lfCharSet = (BYTE)m_wndComboScript.GetItemData(iCurSel);
	m_iCharSet = lf.lfCharSet;

	if (CreateSafeFontIndirect(m_editFont, lf))
	{
		UpdateSampleFont();
		SetModified(TRUE);
		UpdateData(FALSE);
	}
}

void CXTPSyntaxEditPropertiesPageFont::OnSelEndOkHiliteText()
{
	UpdateData();
	UpdateSampleColors();
	SetModified(TRUE);
}

void CXTPSyntaxEditPropertiesPageFont::OnSelEndOkHiliteBack()
{
	UpdateData();
	UpdateSampleColors();
	SetModified(TRUE);
}

void CXTPSyntaxEditPropertiesPageFont::OnSelEndOkText()
{
	UpdateData();
	UpdateSampleColors();
	SetModified(TRUE);
}

void CXTPSyntaxEditPropertiesPageFont::OnSelEndOkBack()
{
	UpdateData();
	UpdateSampleColors();
	SetModified(TRUE);
}

void CXTPSyntaxEditPropertiesPageFont::OnBtnCustomText()
{
	if (m_wndComboText.SelectUserColor() != CB_ERR)
	{
		m_crText = m_wndComboText.GetSelColor();
		OnSelEndOkText();
		SetModified(TRUE);
	}
}

void CXTPSyntaxEditPropertiesPageFont::OnBtnCustomBack()
{
	if (m_wndComboBack.SelectUserColor() != CB_ERR)
	{
		m_crBack = m_wndComboBack.GetSelColor();
		OnSelEndOkBack();
		SetModified(TRUE);
	}
}

void CXTPSyntaxEditPropertiesPageFont::OnBtnCustomHiliteText()
{
	if (m_wndComboHiliteText.SelectUserColor() != CB_ERR)
	{
		m_crHiliteText = m_wndComboHiliteText.GetSelColor();
		OnSelEndOkHiliteText();
		SetModified(TRUE);
	}
}

void CXTPSyntaxEditPropertiesPageFont::OnBtnCustomtHiliteBack()
{
	if (m_wndComboHiliteBack.SelectUserColor() != CB_ERR)
	{
		m_crHiliteBack = m_wndComboHiliteBack.GetSelColor();
		OnSelEndOkHiliteBack();
		SetModified(TRUE);
	}
}

BOOL CXTPSyntaxEditPropertiesPageFont::OnApply()
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

void CXTPSyntaxEditPropertiesPageFont::SetModified(BOOL bChanged/*=TRUE*/)
{
	CPropertyPage::SetModified();
	m_bModified = bChanged;
}
