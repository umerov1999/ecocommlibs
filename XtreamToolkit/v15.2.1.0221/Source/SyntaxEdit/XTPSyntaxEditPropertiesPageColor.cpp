// XTPSyntaxEditPropertiesPageColor.cpp : implementation file
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
#include "XTPSyntaxEditLexPtrs.h"
#include "XTPSyntaxEditLexClassSubObjT.h"
#include "XTPSyntaxEditSectionManager.h"
#include "XTPSyntaxEditLexCfgFileReader.h"
#include "XTPSyntaxEditLexClassSubObjDef.h"
#include "XTPSyntaxEditLexClass.h"
#include "XTPSyntaxEditLexColorFileReader.h"
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

namespace XTPSyntaxEditLexAnalyser
{
	extern CString MakeStr(const CStringArray& rArProps, LPCTSTR strSplitter);
	extern BOOL PropPathSplit(LPCTSTR pcszPropPath, CStringArray& rArProps);
}

using namespace XTPSyntaxEditLexAnalyser;

//===========================================================================
// CXTPSyntaxEditPropertiesPageColor property page
//===========================================================================

CXTPSyntaxEditPropertiesPageColor::CXTPSyntaxEditPropertiesPageColor(CXTPSyntaxEditView* pEditView/*=NULL*/)
: //CPropertyPage(CXTPSyntaxEditPropertiesPageColor::IDD),
  m_bModified(FALSE)
, m_pEditView(pEditView)
, m_parLexClassInfo(NULL)
{

	m_psp.dwFlags |= PSP_DLGINDIRECT;
	m_psp.pResource = XTPResourceManager()->LoadDialogTemplate(IDD);

	//{{AFX_DATA_INIT(CXTPSyntaxEditPropertiesPageColor)
	m_bBold = FALSE;
	m_bItalic = FALSE;
	m_bUnderline = FALSE;
	//}}AFX_DATA_INIT

	SetDefaults();

	m_ptrConfigMgr = NULL;

	if (m_pEditView)
	{
		CXTPSyntaxEditConfigurationManager* ptrConfigMgr =
			m_pEditView->GetEditCtrl().GetLexConfigurationManager();

		if (ptrConfigMgr)
		{
			m_ptrConfigMgr = ptrConfigMgr;
			CMDTARGET_ADDREF(m_ptrConfigMgr);

			m_pTextSchemesMgr = &m_ptrConfigMgr->GetTextSchemesManager();
		}
	}
}

CXTPSyntaxEditPropertiesPageColor::~CXTPSyntaxEditPropertiesPageColor()
{
	XTPSyntaxEditLexConfig()->CleanInfoMap(m_mapLexClassInfo);

	CMDTARGET_RELEASE(m_ptrConfigMgr);

}

void CXTPSyntaxEditPropertiesPageColor::SetDefaults()
{
	// reset color values.
	m_crText        = ::GetSysColor(COLOR_WINDOWTEXT);
	m_crBack        = ::GetSysColor(COLOR_WINDOW);
	m_crHiliteText  = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
	m_crHiliteBack  = ::GetSysColor(COLOR_HIGHLIGHT);

	// reset font attribute flags.
	m_bBold         = FALSE;
	m_bItalic       = FALSE;
	m_bUnderline    = FALSE;

	if (::IsWindow(m_hWnd))
	{
		UpdateData(FALSE);
	}
}

void CXTPSyntaxEditPropertiesPageColor::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXTPSyntaxEditPropertiesPageColor)
	DDX_Check(pDX, XTP_IDC_EDIT_CHK_BOLD, m_bBold);
	DDX_Check(pDX, XTP_IDC_EDIT_CHK_ITALIC, m_bItalic);
	DDX_Check(pDX, XTP_IDC_EDIT_CHK_UNDERLINE, m_bUnderline);
	DDX_Control(pDX, XTP_IDC_EDIT_COMB_TEXT, m_wndComboText);
	DDX_Control(pDX, XTP_IDC_EDIT_COMB_BACK, m_wndComboBack);
	DDX_Control(pDX, XTP_IDC_EDIT_COMB_TEXTSEL, m_wndComboHiliteText);
	DDX_Control(pDX, XTP_IDC_EDIT_COMB_BACKSEL, m_wndComboHiliteBack);
	DDX_Control(pDX, XTP_IDC_EDIT_TXT_SAMPLE, m_txtSample);
	DDX_Control(pDX, XTP_IDC_EDIT_TXT_SAMPLESEL, m_txtSampleSel);
	DDX_Control(pDX, XTP_IDC_EDIT_LBOX_SCHEMANAMES, m_lboxName);
	DDX_Control(pDX, XTP_IDC_EDIT_LBOX_SCHEMAPROP, m_lboxProp);
	DDX_Control(pDX, XTP_IDC_EDIT_CHK_BOLD, m_btnBold);
	DDX_Control(pDX, XTP_IDC_EDIT_CHK_ITALIC, m_btnItalic);
	DDX_Control(pDX, XTP_IDC_EDIT_CHK_UNDERLINE, m_btnUnderline);
	DDX_Control(pDX, XTP_IDC_EDIT_BTN_CUSTOM_TEXT, m_btnCustomText);
	DDX_Control(pDX, XTP_IDC_EDIT_BTN_CUSTOM_BACK, m_btnCustomBack);
	DDX_Control(pDX, XTP_IDC_EDIT_BTN_CUSTOM_TEXTSEL, m_btnCustomHiliteText);
	DDX_Control(pDX, XTP_IDC_EDIT_BTN_CUSTOM_BACKSEL, m_btnCustomHiliteBack);
	DDX_Control(pDX, XTP_IDC_EDIT_GRP_SAMPLE, m_gboxSampleText);
	DDX_CBSyntaxColor(pDX, XTP_IDC_EDIT_COMB_TEXT, m_crText);
	DDX_CBSyntaxColor(pDX, XTP_IDC_EDIT_COMB_BACK, m_crBack);
	DDX_CBSyntaxColor(pDX, XTP_IDC_EDIT_COMB_TEXTSEL, m_crHiliteText);
	DDX_CBSyntaxColor(pDX, XTP_IDC_EDIT_COMB_BACKSEL, m_crHiliteBack);
	//}}AFX_DATA_MAP
}

IMPLEMENT_DYNCREATE(CXTPSyntaxEditPropertiesPageColor, CPropertyPage)

BEGIN_MESSAGE_MAP(CXTPSyntaxEditPropertiesPageColor, CPropertyPage)
	//{{AFX_MSG_MAP(CXTPSyntaxEditPropertiesPageColor)
	ON_BN_CLICKED(XTP_IDC_EDIT_CHK_BOLD, OnChkBold)
	ON_BN_CLICKED(XTP_IDC_EDIT_CHK_ITALIC, OnChkItalic)
	ON_BN_CLICKED(XTP_IDC_EDIT_CHK_UNDERLINE, OnChkUnderline)
	ON_BN_CLICKED(XTP_IDC_EDIT_BTN_CUSTOM_TEXT, OnBtnCustomText)
	ON_BN_CLICKED(XTP_IDC_EDIT_BTN_CUSTOM_BACK, OnBtnCustomBack)
	ON_BN_CLICKED(XTP_IDC_EDIT_BTN_CUSTOM_TEXTSEL, OnBtnCustomHiliteText)
	ON_BN_CLICKED(XTP_IDC_EDIT_BTN_CUSTOM_BACKSEL, OnBtnCustomtHiliteBack)
	ON_CBN_SELENDOK(XTP_IDC_EDIT_COMB_TEXTSEL, OnSelEndOkHiliteText)
	ON_CBN_SELENDOK(XTP_IDC_EDIT_COMB_BACKSEL, OnSelEndOkHiliteBack)
	ON_CBN_SELENDOK(XTP_IDC_EDIT_COMB_TEXT, OnSelEndOkText)
	ON_CBN_SELENDOK(XTP_IDC_EDIT_COMB_BACK, OnSelEndOkBack)
	ON_LBN_SELCHANGE(XTP_IDC_EDIT_LBOX_SCHEMANAMES, OnSelChangeSchemaNames)
	ON_LBN_SELCHANGE(XTP_IDC_EDIT_LBOX_SCHEMAPROP, OnSelChangeSchemaProp)
	ON_LBN_DBLCLK(XTP_IDC_EDIT_LBOX_SCHEMANAMES, OnDblClickSchema)
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CXTPSyntaxEditPropertiesPageColor::UpdateSampleColors()
{
	m_txtSampleSel.SetBackColor(m_crHiliteBack);
	m_txtSampleSel.SetTextColor(m_crHiliteText);
	m_txtSample.SetBackColor(m_crBack);
	m_txtSample.SetTextColor(m_crText);
}

BOOL CXTPSyntaxEditPropertiesPageColor::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here
	m_wndComboHiliteBack.SetAutoColor(::GetSysColor(COLOR_HIGHLIGHT));
	m_wndComboHiliteText.SetAutoColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
	m_wndComboBack.SetAutoColor(::GetSysColor(COLOR_WINDOW));
	m_wndComboText.SetAutoColor(::GetSysColor(COLOR_WINDOWTEXT));

	if (m_wndComboBack.SetSelColor(m_crBack) == CB_ERR)
	{
		m_wndComboBack.SetUserColor(m_crBack);
		m_wndComboBack.SetSelColor(m_crBack);
	}

	if (m_wndComboText.SetSelColor(m_crText) == CB_ERR)
	{
		m_wndComboText.SetUserColor(m_crText);
		m_wndComboText.SetSelColor(m_crText);
	}

	if (m_wndComboHiliteBack.SetSelColor(m_crHiliteBack) == CB_ERR)
	{
		m_wndComboHiliteBack.SetUserColor(m_crHiliteBack);
		m_wndComboHiliteBack.SetSelColor(m_crHiliteBack);
	}

	if (m_wndComboHiliteText.SetSelColor(m_crHiliteText) == CB_ERR)
	{
		m_wndComboHiliteText.SetUserColor(m_crHiliteText);
		m_wndComboHiliteText.SetSelColor(m_crHiliteText);
	}

	XTPSyntaxEditLexConfig()->CopyInfoMap(m_mapLexClassInfo);

	// Get all schemes names
	CXTPSyntaxEditSchemaFileInfoList* pinfoList = m_pTextSchemesMgr->GetSchemaList();
	for (POSITION pos = pinfoList->GetHeadPosition(); pos;)
	{
		XTP_EDIT_SCHEMAFILEINFO& info = pinfoList->GetNext(pos);
		int iIndex = m_lboxName.AddString((info.csDesc.GetLength() > 0)? info.csDesc: info.csName);
		if (iIndex != LB_ERR)
		{
			m_lboxName.SetItemData(iIndex, (DWORD_PTR)&info);
		}
	}

	if (m_lboxName.GetCount() > 0)
	{
		m_lboxName.SetCurSel(0);
		OnSelChangeSchemaNames();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CXTPSyntaxEditPropertiesPageColor::WriteRegistryValues()
{
	if (!::IsWindow(m_pEditView->GetSafeHwnd()))
		return FALSE;

	for (POSITION pos = m_mapLexClassInfo.GetStartPosition(); pos;)
	{
		CString csKey; CXTPSyntaxEditLexClassInfoArray* pLexClassArray = NULL;
		m_mapLexClassInfo.GetNextAssoc(pos, csKey, (void*&)pLexClassArray);
		XTPSyntaxEditLexConfig()->WriteCfgFile(csKey, *pLexClassArray);
	}

	return TRUE;
}

void CXTPSyntaxEditPropertiesPageColor::OnSelEndOkHiliteText()
{
	UpdateData();
	UpdateSampleColors();
	SetModified(TRUE);
	UpdateColorValue(m_wndComboHiliteText, m_crHiliteText, XTPLEX_ATTR_TXT_COLORSELFG);
}

void CXTPSyntaxEditPropertiesPageColor::OnSelEndOkHiliteBack()
{
	UpdateData();
	UpdateSampleColors();
	SetModified(TRUE);
	UpdateColorValue(m_wndComboHiliteBack, m_crHiliteBack, XTPLEX_ATTR_TXT_COLORSELBK);
}

void CXTPSyntaxEditPropertiesPageColor::OnSelEndOkText()
{
	UpdateData();
	UpdateSampleColors();
	SetModified(TRUE);
	UpdateColorValue(m_wndComboText, m_crText, XTPLEX_ATTR_TXT_COLORFG);
}

void CXTPSyntaxEditPropertiesPageColor::OnSelEndOkBack()
{
	UpdateData();
	UpdateSampleColors();
	SetModified(TRUE);
	UpdateColorValue(m_wndComboBack, m_crBack, XTPLEX_ATTR_TXT_COLORBK);
}

void CXTPSyntaxEditPropertiesPageColor::OnChkBold()
{
	UpdateData();
	UpdateFont();
	SetModified(TRUE);
	UpdateFontValue(m_bBold, XTPLEX_ATTR_TXT_BOLD);
}

void CXTPSyntaxEditPropertiesPageColor::OnChkItalic()
{
	UpdateData();
	UpdateFont();
	SetModified(TRUE);
	UpdateFontValue(m_bItalic, XTPLEX_ATTR_TXT_ITALIC);
}

void CXTPSyntaxEditPropertiesPageColor::OnChkUnderline()
{
	UpdateData();
	UpdateFont();
	SetModified(TRUE);
	UpdateFontValue(m_bUnderline, XTPLEX_ATTR_TXT_UNDERLINE);
}

void CXTPSyntaxEditPropertiesPageColor::OnBtnCustomText()
{
	if (m_wndComboText.SelectUserColor() != CB_ERR)
	{
		m_crText = m_wndComboText.GetSelColor();
		OnSelEndOkText();
	}
}

void CXTPSyntaxEditPropertiesPageColor::OnBtnCustomBack()
{
	if (m_wndComboBack.SelectUserColor() != CB_ERR)
	{
		m_crBack = m_wndComboBack.GetSelColor();
		OnSelEndOkBack();
	}
}

void CXTPSyntaxEditPropertiesPageColor::OnBtnCustomHiliteText()
{
	if (m_wndComboHiliteText.SelectUserColor() != CB_ERR)
	{
		m_crHiliteText = m_wndComboHiliteText.GetSelColor();
		OnSelEndOkHiliteText();
	}
}

void CXTPSyntaxEditPropertiesPageColor::OnBtnCustomtHiliteBack()
{
	if (m_wndComboHiliteBack.SelectUserColor() != CB_ERR)
	{
		m_crHiliteBack = m_wndComboHiliteBack.GetSelColor();
		OnSelEndOkHiliteBack();
	}
}

BOOL CXTPSyntaxEditPropertiesPageColor::OnSetActive()
{
	UpdateFont();
	return CPropertyPage::OnSetActive();
}

void CXTPSyntaxEditPropertiesPageColor::UpdateFont()
{
	CPropertySheet* pParentDlg = DYNAMIC_DOWNCAST(
		CPropertySheet, GetParent());

	if (!pParentDlg)
		return;

	CXTPSyntaxEditPropertiesPageFont* pPage2 = DYNAMIC_DOWNCAST(
		CXTPSyntaxEditPropertiesPageFont, pParentDlg->GetPage(1));

	if (!pPage2)
		return;

	LOGFONT lf = {0};
	if (!pPage2->GetSafeLogFont(lf))
		return;

	lf.lfStrikeOut = 0;
	lf.lfItalic    = (BYTE)m_bItalic;
	lf.lfUnderline = (BYTE)m_bUnderline;
	lf.lfWeight    = m_bBold? FW_BOLD: FW_NORMAL;

	if (pPage2->CreateSafeFontIndirect(m_editFont, lf))
	{
		m_txtSample.SetFont(&m_editFont);
		m_txtSampleSel.SetFont(&m_editFont);
	}
}

BOOL CXTPSyntaxEditPropertiesPageColor::OnApply()
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

void CXTPSyntaxEditPropertiesPageColor::EnableControls()
{
	BOOL bEnable = (m_lboxName.GetCurSel() != LB_ERR);
	m_lboxProp.EnableWindow(bEnable);

	bEnable = (bEnable && m_lboxProp.GetCurSel() != LB_ERR);
	m_txtSampleSel.EnableWindow(bEnable);
	m_txtSample.EnableWindow(bEnable);
	m_gboxSampleText.EnableWindow(bEnable);
	m_wndComboHiliteText.EnableWindow(bEnable);
	m_wndComboHiliteBack.EnableWindow(bEnable);
	m_wndComboText.EnableWindow(bEnable);
	m_wndComboBack.EnableWindow(bEnable);
	m_btnBold.EnableWindow(bEnable);
	m_btnItalic.EnableWindow(bEnable);
	m_btnUnderline.EnableWindow(bEnable);
	m_btnCustomText.EnableWindow(bEnable);
	m_btnCustomBack.EnableWindow(bEnable);
	m_btnCustomHiliteText.EnableWindow(bEnable);
	m_btnCustomHiliteBack.EnableWindow(bEnable);
}

void CXTPSyntaxEditPropertiesPageColor::OnSelChangeSchemaNames()
{
	if (!m_lboxName.SelChanged())
		return;

	SetDefaults();

	int iIndex = m_lboxName.GetCurSel();
	if (iIndex != LB_ERR)
	{
		XTP_EDIT_SCHEMAFILEINFO* pSchemaInfo = (XTP_EDIT_SCHEMAFILEINFO*)m_lboxName.GetItemData(iIndex);
		if (pSchemaInfo)
		{
			InitSchemaClasses(pSchemaInfo);
		}
	}
}

void CXTPSyntaxEditPropertiesPageColor::OnSelChangeSchemaProp()
{
	if (!m_lboxProp.SelChanged())
		return;

	SetDefaults();

	int iIndex = m_lboxProp.GetCurSel();
	if (iIndex == LB_ERR)
		return;

	XTP_EDIT_LEXCLASSINFO* pInfoClass = (XTP_EDIT_LEXCLASSINFO*)m_lboxProp.GetItemData(iIndex);
	if (!pInfoClass)
		return;

	InitClassData(*pInfoClass);
	UpdateData(FALSE);
}

BOOL CXTPSyntaxEditPropertiesPageColor::IsTopLevelClass(const XTP_EDIT_LEXCLASSINFO& infoClass)
{
	int nPropertiesCount = (int)infoClass.arPropertyDesc.GetSize();
	for (int iIndex = 0; iIndex < nPropertiesCount; iIndex++)
	{
		// Process property name
		const XTP_EDIT_LEXPROPINFO& infoProp =
			infoClass.arPropertyDesc.GetAt(iIndex);

		if (infoProp.arPropName.GetSize() != 2)
			continue;

		CString csParent = infoProp.arPropName.GetAt(0);
		if (csParent.CompareNoCase(_T("parent")) != 0)
			continue;

		CString csFile = infoProp.arPropName.GetAt(1);
		if (csFile.CompareNoCase(_T("file")) != 0)
			continue;

		return TRUE;
	}

	return FALSE;
}

CString CXTPSyntaxEditPropertiesPageColor::GetDisplayName(const XTP_EDIT_LEXCLASSINFO& info) const
{
	for (int i = 0; i < info.arPropertyDesc.GetSize(); ++i)
	{
		if (MakeStr(info.arPropertyDesc[i].arPropName, _T(":")) == XTPLEX_ATTR_DISPLAYNAME)
		{
			CString csDisplayName = GetPropValue(info.arPropertyDesc[i]);
			csDisplayName.Remove(39);
			return csDisplayName;
		}
	}
	return info.csClassName;
}

BOOL CXTPSyntaxEditPropertiesPageColor::InitSchemaClasses(XTP_EDIT_SCHEMAFILEINFO* pSchemaInfo)
{
	CString csKey = pSchemaInfo->csValue;
	csKey.MakeLower();

	m_parLexClassInfo = NULL;
	if (!m_mapLexClassInfo.Lookup(csKey, (void*&)m_parLexClassInfo) || !m_parLexClassInfo)
		return FALSE;

	m_lboxProp.SetRedraw(FALSE);
	m_lboxProp.ResetContent();

	// For all classes in the current scheme
	int iSize = (int)m_parLexClassInfo->GetSize();
	for (int i = 0; i < iSize; i++)
	{
		int iIndex = m_lboxProp.AddString(GetDisplayName((*m_parLexClassInfo)[i]));
		if (iIndex != LB_ERR)
			m_lboxProp.SetItemData(iIndex, (DWORD_PTR)&(*m_parLexClassInfo)[i]);
	}

	m_lboxProp.SetCurSel(0);
	OnSelChangeSchemaProp();

	m_lboxProp.SetRedraw(TRUE);
	m_lboxProp.RedrawWindow();

	return TRUE;
}

CString CXTPSyntaxEditPropertiesPageColor::GetPropValue(const XTP_EDIT_LEXPROPINFO& infoProp) const
{
	CString csValue;
	for (int iItem = 0; iItem < infoProp.arPropValue.GetSize(); iItem++)
	{
		if (iItem > 0)
		{
			csValue += _T(", ");
		}
		CString strProp1 = infoProp.arPropValue.GetAt(iItem);
		csValue += XTPSyntaxEditLexConfig()->ESToStr(strProp1, TRUE);
	}
	return csValue;
}

AFX_STATIC int AFX_CDECL AfxStrToInt(LPCTSTR pcszValue)
{
	TCHAR* pCh = NULL;
	return _tcstol(pcszValue, &pCh, 0);
}

AFX_STATIC DWORD AFX_CDECL AfxStrToRGB(LPCTSTR pcszValue)
{
	return XTP_EDIT_RGB_INT2CLR(AfxStrToInt(pcszValue));
}

void CXTPSyntaxEditPropertiesPageColor::InitClassData(const XTP_EDIT_LEXCLASSINFO& infoClass)
{
	int iCount = (int)infoClass.arPropertyDesc.GetSize();
	for (int iIndex = 0; iIndex < iCount; ++iIndex)
	{
		const XTP_EDIT_LEXPROPINFO& infoProp = infoClass.arPropertyDesc.GetAt(iIndex);
		CString csPropName = MakeStr(infoProp.arPropName, _T(":"));

		if (csPropName == XTPLEX_ATTR_TXT_COLORFG)
		{
			m_crText = AfxStrToRGB(GetPropValue(infoProp));
		}
		else if (csPropName == XTPLEX_ATTR_TXT_COLORBK)
		{
			m_crBack = AfxStrToRGB(GetPropValue(infoProp));
		}
		else if (csPropName == XTPLEX_ATTR_TXT_COLORSELFG)
		{
			m_crHiliteText = AfxStrToRGB(GetPropValue(infoProp));
		}
		else if (csPropName == XTPLEX_ATTR_TXT_COLORSELBK)
		{
			m_crHiliteBack = AfxStrToRGB(GetPropValue(infoProp));
		}
		else if (csPropName == XTPLEX_ATTR_TXT_BOLD)
		{
			m_bBold = (BOOL)_ttoi(GetPropValue(infoProp));
		}
		else if (csPropName == XTPLEX_ATTR_TXT_ITALIC)
		{
			m_bItalic = (BOOL)_ttoi(GetPropValue(infoProp));
		}
		else if (csPropName == XTPLEX_ATTR_TXT_UNDERLINE)
		{
			m_bUnderline = (BOOL)_ttoi(GetPropValue(infoProp));
		}
	}

	UpdateFont();
	UpdateSampleColors();
	EnableControls();
}

void CXTPSyntaxEditPropertiesPageColor::SetModified(BOOL bChanged/*=TRUE*/)
{
	CPropertyPage::SetModified(bChanged);
	m_bModified = bChanged;

	if (m_parLexClassInfo)
		m_parLexClassInfo->m_bModified = bChanged;
}

int CXTPSyntaxEditPropertiesPageColor::PropExists(CXTPSyntaxEditLexPropInfoArray& arrProp, LPCTSTR lpszPropName)
{
	int iCount = (int)arrProp.GetSize();
	for (int iIndex = 0; iIndex < iCount; ++iIndex)
	{
		const XTP_EDIT_LEXPROPINFO& infoProp = arrProp.GetAt(iIndex);
		CString csPropName = MakeStr(infoProp.arPropName, _T(":"));

		if (csPropName.CompareNoCase(lpszPropName) == 0)
		{
			return iIndex;
		}
	}
	return -1;
}

BOOL CXTPSyntaxEditPropertiesPageColor::UpdateColorValue(CXTPSyntaxEditColorComboBox& combo, COLORREF& color, LPCTSTR lpszPropName)
{
	int iSel = m_lboxProp.GetCurSel();

	XTP_EDIT_LEXCLASSINFO* pInfoClass = (XTP_EDIT_LEXCLASSINFO*)m_lboxProp.GetItemData(iSel);
	if (!pInfoClass)
		return FALSE;

	int iIndex = PropExists(pInfoClass->arPropertyDesc, lpszPropName);

	// automatic color selected, remove from array.
	if (combo.GetAutoColor() == color)
	{
		if (iIndex != -1)
		{
			pInfoClass->arPropertyDesc.RemoveAt(iIndex);
			return TRUE;
		}
	}

	else
	{
		CString csColor;
		csColor.Format(_T("0x%06X"), XTP_EDIT_RGB_INT2CLR(color));

		if (iIndex == -1)
		{
			XTP_EDIT_LEXPROPINFO infoProp;
			infoProp.arPropValue.Add(csColor);
			PropPathSplit(lpszPropName, infoProp.arPropName);
			pInfoClass->arPropertyDesc.Add(infoProp);
		}
		else
		{
			XTP_EDIT_LEXPROPINFO& infoProp = pInfoClass->arPropertyDesc[iIndex];
			infoProp.arPropValue.RemoveAll();
			infoProp.arPropValue.Add(csColor);
		}

		return TRUE;
	}

	return FALSE;
}

BOOL CXTPSyntaxEditPropertiesPageColor::UpdateFontValue(BOOL& bValue, LPCTSTR lpszPropName)
{
	CString csValue(_T("1"));
	int iSel = m_lboxProp.GetCurSel();

	XTP_EDIT_LEXCLASSINFO* pInfoClass = (XTP_EDIT_LEXCLASSINFO*)m_lboxProp.GetItemData(iSel);
	if (!pInfoClass)
		return FALSE;

	int iIndex = PropExists(pInfoClass->arPropertyDesc, lpszPropName);
	if (iIndex != -1)
	{
		pInfoClass->arPropertyDesc.RemoveAt(iIndex);
	}

	if (bValue == TRUE)
	{
		XTP_EDIT_LEXPROPINFO infoProp;
		PropPathSplit(lpszPropName, infoProp.arPropName);
		infoProp.arPropValue.Add(csValue);
		pInfoClass->arPropertyDesc.Add(infoProp);
		return TRUE;
	}

	return FALSE;
}

void CXTPSyntaxEditPropertiesPageColor::OnDblClickSchema()
{
	int iIndex = m_lboxName.GetCurSel();
	if (iIndex == LB_ERR)
		return;

	XTP_EDIT_SCHEMAFILEINFO* pSchemaInfo = (XTP_EDIT_SCHEMAFILEINFO*)m_lboxName.GetItemData(iIndex);
	if (pSchemaInfo == NULL)
		return;

	if (!FILEEXISTS_S(pSchemaInfo->csValue))
		return;

	TCHAR szDrive[_MAX_DRIVE], szDir[_MAX_DIR], szFileName[_MAX_FNAME], szEx[_MAX_EXT];
	SPLITPATH_S(pSchemaInfo->csValue, szDrive, szDir, szFileName, szEx);

	CString csBuffer;
	XTPResourceManager()->LoadString(
		&csBuffer, XTP_IDS_EDIT_OPENSCHEMAMSG);

	CString csMessage;
	csMessage.Format(csBuffer, szFileName, szEx);
	if (AfxMessageBox(csMessage, MB_ICONQUESTION | MB_YESNO) == IDYES)
	{
		// close the options dialog.
		CPropertySheet* pWndParent = DYNAMIC_DOWNCAST(CPropertySheet, GetParent());
		if (pWndParent)
			pWndParent->EndDialog(IDCANCEL);

		// open the document.
		CWinApp* pWinApp = AfxGetApp();
		if (pWinApp)
			pWinApp->OpenDocumentFile(pSchemaInfo->csValue);
	}
}

void CXTPSyntaxEditPropertiesPageColor::OnContextMenu(CWnd* pWnd, CPoint point)
{
	UNREFERENCED_PARAMETER(pWnd);

	int iItem = m_lboxName.HitTest(point);
	if (iItem != LB_ERR)
	{
		m_lboxName.SetCurSel(iItem);
		OnSelChangeSchemaNames();

		CString csBuffer;
		XTPResourceManager()->LoadString(
			&csBuffer, XTP_IDS_EDIT_OPENSCHEMACMD);

		CMenu menu;
		menu.CreatePopupMenu();
		menu.AppendMenu(MF_STRING, ID_FILE_OPEN, csBuffer);

		UINT nFlags = TPM_LEFTALIGN|TPM_RIGHTBUTTON|TPM_NONOTIFY|TPM_RETURNCMD;
		if (menu.TrackPopupMenu(nFlags, point.x, point.y, this) == ID_FILE_OPEN)
		{
			OnDblClickSchema();
		}
	}
}
