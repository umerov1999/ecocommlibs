// XTPPropertyGridItemFont.cpp : implementation of the CXTPPropertyGridItemFont class.
//
// This file is a part of the XTREME PROPERTYGRID MFC class library.
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

#include "stdafx.h"
#include "Common/XTPVC80Helpers.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPSystemHelpers.h"
#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollBarCtrl.h>
#include <Common/ScrollBar/XTPWnd.h>

#include "XTPPropertyGridDefines.h"
#include "XTPPropertyGridItem.h"
#include "XTPPropertyGridItemFont.h"
#include "XTPPropertyGridToolTip.h"
#include "XTPPropertyGridView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridItemFont
IMPLEMENT_DYNAMIC(CXTPPropertyGridItemFont, CXTPPropertyGridItem)


CXTPPropertyGridItemFont::CXTPPropertyGridItemFont(LPCTSTR strCaption, LOGFONT& font)
	: CXTPPropertyGridItem(strCaption)
{
	SetFont(font);
	m_nFlags = xtpGridItemHasExpandButton;
	m_clrValue = (COLORREF)-1;
	EnableAutomation();
	m_strDefaultValue = m_strValue;
}

CXTPPropertyGridItemFont::CXTPPropertyGridItemFont(UINT nID, LOGFONT& font)
	: CXTPPropertyGridItem(nID)
{
	SetFont(font);
	m_nFlags = xtpGridItemHasExpandButton;
	m_clrValue = (COLORREF)-1;
	EnableAutomation();
	m_strDefaultValue = m_strValue;
}

CXTPPropertyGridItemFont::~CXTPPropertyGridItemFont()
{
}

/////////////////////////////////////////////////////////////////////////////
//

void CXTPPropertyGridItemFont::SetFont(LOGFONT& font)
{
	MEMCPY_S(&m_lfValue, &font, sizeof(LOGFONT));

	m_strValue = FontToString(m_lfValue);
}

CString CXTPPropertyGridItemFont::FontToString(const LOGFONT& lfValue)
{
	CWindowDC dc(CWnd::GetDesktopWindow());
	int nHeight = -MulDiv(lfValue.lfHeight, 72, dc.GetDeviceCaps(LOGPIXELSY));

	CString strValue;
	strValue.Format(_T("%s; %ipt"), lfValue.lfFaceName, nHeight);

	if (lfValue.lfWeight == FW_BOLD)
		strValue += _T("; bold");

	return strValue;
}

BOOL CXTPPropertyGridItemFont::OnDrawItemValue(CDC& dc, CRect rcValue)
{
	if (m_clrValue == (COLORREF)-1)
		return CXTPPropertyGridItem::OnDrawItemValue(dc, rcValue);

	COLORREF clr = dc.GetTextColor();
	CRect rcSample(rcValue.left - 2, rcValue.top + 1, rcValue.left + 18, rcValue.bottom - 1);
	CXTPPenDC pen(dc, clr);
	CXTPBrushDC brush(dc, m_clrValue);
	dc.Rectangle(rcSample);

	CRect rcText(rcValue);
	rcText.left += 25;

	dc.DrawText(m_strValue, rcText, DT_SINGLELINE | DT_VCENTER);

	return TRUE;
}

CRect CXTPPropertyGridItemFont::GetValueRect()
{
	CRect rcValue(CXTPPropertyGridItem::GetValueRect());
	if (m_clrValue != (COLORREF)-1) rcValue.left += 25;
	return rcValue;
}


UINT_PTR CALLBACK CXTPPropertyGridItemFont::FontDlgProc(HWND hWnd, UINT message, WPARAM, LPARAM)
{
	// special case for WM_INITDIALOG
	if (message == WM_INITDIALOG)
	{
		HWND hWndCombo = GetDlgItem(hWnd, 1139);
		if (hWndCombo)
			EnableWindow(hWndCombo, FALSE);

		CDialog* pDlg = DYNAMIC_DOWNCAST(CDialog, CWnd::FromHandlePermanent(hWnd));
		if (pDlg != NULL)
			return (UINT_PTR)pDlg->OnInitDialog();
		else
			return 1;
	}

	return 0;

}

void CXTPPropertyGridItemFont::OnInplaceButtonDown(CXTPPropertyGridInplaceButton* pButton)
{
	if (m_pGrid->SendNotifyMessage(XTP_PGN_INPLACEBUTTONDOWN, (LPARAM)pButton) == TRUE)
		return;

	if (!OnRequestEdit())
		return;

	InternalAddRef();

	CFontDialog dlg(&m_lfValue, CF_EFFECTS | CF_SCREENFONTS, NULL, m_pGrid);

	if (m_clrValue == (COLORREF)-1)
	{
		dlg.m_cf.lpfnHook = FontDlgProc;
	}
	else
	{
		dlg.m_cf.rgbColors = m_clrValue;
	}

	if (dlg.DoModal() == IDOK)
	{
		LOGFONT lf;
		dlg.GetCurrentFont(&lf);

		CString strValue = FontToString(lf);

		if (OnAfterEdit(strValue))
		{
			SetFont(lf);

			if (m_clrValue != (COLORREF)-1)
				m_clrValue = dlg.GetColor();

			OnValueChanged(strValue);
			SAFE_INVALIDATE(m_pGrid);
		}
	}
	else
	{
		OnCancelEdit();
	}

	InternalRelease();
}

void CXTPPropertyGridItemFont::SetColor(COLORREF clr)
{
	m_clrValue = clr;
	SAFE_INVALIDATE(m_pGrid);
}

COLORREF CXTPPropertyGridItemFont::GetColor()
{
	return m_clrValue;
}

void CXTPPropertyGridItemFont::GetFont(LOGFONT* lf)
{
	ASSERT(lf != NULL);
	MEMCPY_S(lf, &m_lfValue, sizeof(LOGFONT));
}

#ifdef _XTP_ACTIVEX


BEGIN_DISPATCH_MAP(CXTPPropertyGridItemFont, CXTPPropertyGridItem)
	DISP_PROPERTY_EX_ID(CXTPPropertyGridItemFont, "Color", 50, GetColor, SetColor, VT_I4)

	DISP_PROPERTY_EX_ID(CXTPPropertyGridItemFont, "FontFaceName", 51, OleGetFaceName, OleSetFaceName, VT_BSTR)
	DISP_PROPERTY_EX_ID(CXTPPropertyGridItemFont, "FontSize", 52, OleGetSize, OleSetSize, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPPropertyGridItemFont, "FontBold", 53, OleGetBold, OleSetBold, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPPropertyGridItemFont, "FontItalic", 54, OleGetItalic, OleSetItalic, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPPropertyGridItemFont, "FontUnderline", 55, OleGetUnderline, OleSetUnderline, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPPropertyGridItemFont, "FontStrikeOut", 56, OleGetStrikeOut, OleSetStrikeOut, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPPropertyGridItemFont, "FontWeight", 57, OleGetWeight, OleSetWeight, VT_I4)
END_DISPATCH_MAP()

// {C0F773A5-62A8-4d96-B165-EE284F8AB188}
static const GUID IID_PropertyGridItemFont =
{ 0xc0f773a5, 0x62a8, 0x4d96, { 0xb1, 0x65, 0xee, 0x28, 0x4f, 0x8a, 0xb1, 0x88 } };


BEGIN_INTERFACE_MAP(CXTPPropertyGridItemFont, CXTPPropertyGridItem)
	INTERFACE_PART(CXTPPropertyGridItemFont, IID_PropertyGridItemFont, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPPropertyGridItemFont, IID_PropertyGridItemFont)


BSTR CXTPPropertyGridItemFont::OleGetFaceName()
{
	return CString(m_lfValue.lfFaceName).AllocSysString();
}
void CXTPPropertyGridItemFont::OleSetFaceName(LPCTSTR lpszFaceName)
{
	STRCPY_S(m_lfValue.lfFaceName, LF_FACESIZE, lpszFaceName);
	SetFont(m_lfValue);
}
int CXTPPropertyGridItemFont::OleGetSize()
{
	CWindowDC dc(CWnd::GetDesktopWindow());
	int nHeight = -MulDiv(m_lfValue.lfHeight, 72, dc.GetDeviceCaps(LOGPIXELSY));

	return nHeight;
}
void CXTPPropertyGridItemFont::OleSetSize(int nHeigth)
{
	CWindowDC dc(CWnd::GetDesktopWindow());
	m_lfValue.lfHeight = -MulDiv(nHeigth, dc.GetDeviceCaps(LOGPIXELSY), 72);

	SetFont(m_lfValue);
}

int CXTPPropertyGridItemFont::OleGetWeight()
{
	return m_lfValue.lfWeight;
}
void CXTPPropertyGridItemFont::OleSetWeight(int nWeight)
{
	m_lfValue.lfWeight = nWeight;
	SetFont(m_lfValue);
}

BOOL CXTPPropertyGridItemFont::OleGetBold()
{
	return m_lfValue.lfWeight == FW_BOLD;
}
void CXTPPropertyGridItemFont::OleSetBold(BOOL bBold)
{
	m_lfValue.lfWeight = bBold ? FW_BOLD : FW_NORMAL;
	SetFont(m_lfValue);
}

BOOL CXTPPropertyGridItemFont::OleGetItalic()
{
	return m_lfValue.lfItalic;
}
void CXTPPropertyGridItemFont::OleSetItalic(BOOL bItalic)
{
	m_lfValue.lfItalic = bItalic;
	SetFont(m_lfValue);
}

BOOL CXTPPropertyGridItemFont::OleGetUnderline()
{
	return m_lfValue.lfUnderline;
}
void CXTPPropertyGridItemFont::OleSetUnderline(BOOL bUnderline)
{
	m_lfValue.lfUnderline = bUnderline;
	SetFont(m_lfValue);
}
BOOL CXTPPropertyGridItemFont::OleGetStrikeOut()
{
	return m_lfValue.lfStrikeOut;
}
void CXTPPropertyGridItemFont::OleSetStrikeOut(BOOL bStrikeOut)
{
	m_lfValue.lfStrikeOut = bStrikeOut;
	SetFont(m_lfValue);
}

void CXTPPropertyGridItemFont::OleSetValue(const VARIANT* varValue)
{
	if (varValue->vt == (VT_VARIANT | VT_BYREF))
	{
		OleSetValue(varValue->pvarVal);
		return;
	}

	if (varValue->vt == VT_I4)
	{
		HGDIOBJ hFont = (HGDIOBJ)varValue->lVal;
		if (::GetObjectType(hFont) == OBJ_FONT)
		{
			LOGFONT lf;
			::GetObject(hFont, sizeof(LOGFONT), &lf);
			SetFont(lf);
		}
	}
	else if (varValue->vt == VT_DISPATCH)
	{
		CComQIPtr<IFont> lpFont (varValue->pdispVal);
		if (lpFont)
		{
			HFONT hFont;
			LOGFONT lf;

			lpFont->get_hFont(&hFont);
			::GetObject(hFont, sizeof(LOGFONT), &lf);

			SetFont(lf);
		}
	}
}

const VARIANT CXTPPropertyGridItemFont::OleGetValue()
{
	USES_CONVERSION;

	FONTDESC fd;

	fd.cbSizeofstruct = sizeof(FONTDESC);
	fd.lpstrName = T2OLE(m_lfValue.lfFaceName);
	fd.sWeight = (short)m_lfValue.lfWeight;
	fd.sCharset = m_lfValue.lfCharSet;
	fd.fItalic = m_lfValue.lfItalic;
	fd.fUnderline = m_lfValue.lfUnderline;
	fd.fStrikethrough = m_lfValue.lfStrikeOut;

	long lfHeight = m_lfValue.lfHeight;
	if (lfHeight < 0)
		lfHeight = -lfHeight;

	CWindowDC dc(CWnd::GetDesktopWindow());
	int ppi = dc.GetDeviceCaps(LOGPIXELSY);
	fd.cySize.Lo = lfHeight * 720000 / ppi;
	fd.cySize.Hi = 0;

	IFontDisp* lpFont = 0;

	if (FAILED(::OleCreateFontIndirect(&fd, IID_IFontDisp, (void**)&lpFont)))
		lpFont = NULL;


	return CComVariant(lpFont);
}

#endif
