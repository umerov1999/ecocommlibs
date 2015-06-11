// XTPChartAxisConstantLines.cpp
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
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
#include "Common/XTPPropExchange.h"

#include "../../Types/XTPChartTypes.h"
#include "../../XTPChartDefines.h"
#include "../../XTPChartElement.h"
#include <Chart/XTPChartLegendItem.h>

#include "XTPChartAxisConstantLines.h"
#include "XTPChartAxis.h"
#include "../../Appearance/XTPChartAppearance.h"
#include "../../Appearance/XTPChartLineStyle.h"

IMPLEMENT_DYNCREATE(CXTPChartAxisConstantLine, CXTPChartTextElement)

//////////////////////////////////////////////////////////////////////////
// CXTPChartAxisConstantLine

CXTPChartAxisConstantLine::CXTPChartAxisConstantLine()
{
	m_bLegendVisible = TRUE;
	m_bVisible = TRUE;

	m_nAlignment = xtpChartAlignNear;
	m_bAntialiasing = FALSE;
	m_pFont = CXTPChartFont::GetTahoma8();

	m_bShowTextBelow = FALSE;
	m_bShowBehind = FALSE;

	m_pLineStyle = new CXTPChartLineStyle(this);

	m_dAxisValue = 0;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPChartAxisConstantLine::~CXTPChartAxisConstantLine()
{
	SAFE_RELEASE(m_pFont);
	SAFE_RELEASE(m_pLineStyle);
}

void CXTPChartAxisConstantLine::SetText(const CXTPChartString& lpszConstantLine)
{
	m_strText = lpszConstantLine;
	OnChartChanged();
}

void CXTPChartAxisConstantLine::SetFont(CXTPChartFont* pFont)
{
	SAFE_RELEASE(m_pFont);
	m_pFont = pFont;
	OnChartChanged();
}

void CXTPChartAxisConstantLine::SetTextColor(const CXTPChartColor& clrTextColor)
{
	m_clrTextColor = clrTextColor;
	OnChartChanged();
}

void CXTPChartAxisConstantLine::SetAlignment(XTPChartStringAlignment nAlignment)
{
	m_nAlignment = nAlignment;
	OnChartChanged();
}

CXTPChartString CXTPChartAxisConstantLine::GetLegendName() const
{
	if (!m_strLegendName.IsEmpty())
		return m_strLegendName;

	return m_strText;
}

CXTPChartColor CXTPChartAxisConstantLine::GetTextColor() const
{
	return m_clrTextColor;
}

CXTPChartColor CXTPChartAxisConstantLine::GetActualTextColor() const
{
	if (!m_clrTextColor.IsEmpty())
		return m_clrTextColor;

	return GetAppearance()->GetDiagram2DAppearance()->GetAxisAppearance()->ConstantLineTextColor;
}

CXTPChartColor CXTPChartAxisConstantLine::GetActualColor() const
{
	if (!m_clrColor.IsEmpty())
		return m_clrColor;

	return GetAppearance()->GetDiagram2DAppearance()->GetAxisAppearance()->ConstantLineColor;
}

CXTPChartDeviceCommand* CXTPChartAxisConstantLine::CreateLegendDeviceCommand(CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	UNREFERENCED_PARAMETER(pDC);

	rcBounds.DeflateRect(1, 1);
	int nCenter = rcBounds.CenterPoint().y;

	return m_pLineStyle->CreateDeviceCommand(CXTPChartPointF((float)rcBounds.left, (float)nCenter), CXTPChartPointF((float)rcBounds.right, (float)nCenter), GetActualColor());

}

void CXTPChartAxisConstantLine::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_Bool(pPX, _T("Visible"), m_bVisible, TRUE);
	PX_Bool(pPX, _T("LegendVisible"), m_bLegendVisible, TRUE);

	PX_String(pPX, _T("Text"), m_strText, _T(""));
	PX_String(pPX, _T("LegendName"), m_strLegendName, _T(""));

	PX_String(pPX, _T("AxisValue"), m_strAxisValue, _T(""));
	PX_Double(pPX, _T("AxisValueInternal"), m_dAxisValue, 0);

	PX_Color(pPX, _T("Color"), m_clrColor);
	PX_Color(pPX, _T("TextColor"), m_clrTextColor);

	PX_Enum(pPX, _T("Alignment"), m_nAlignment, xtpChartAlignNear);
	PX_Bool(pPX, _T("Antialiasing"), m_bAntialiasing, FALSE);

	PX_Font(pPX, _T("Font"), m_pFont);

	PX_Bool(pPX, _T("ShowTextBelow"), m_bShowTextBelow, FALSE);
	PX_Bool(pPX, _T("ShowBehind"), m_bShowBehind, FALSE);


	CXTPPropExchangeSection secLineStyle(pPX->GetSection(_T("LineStyle")));
	m_pLineStyle->DoPropExchange(&secLineStyle);
}


//////////////////////////////////////////////////////////////////////////
// CXTPChartAxisConstantLines


CXTPChartAxisConstantLines::CXTPChartAxisConstantLines(CXTPChartAxis* pAxis)
{
	m_pOwner = pAxis;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPChartAxisConstantLines::~CXTPChartAxisConstantLines()
{
}

CXTPChartAxisConstantLine* CXTPChartAxisConstantLines::Add(CXTPChartAxisConstantLine* pConstantLine)
{
	InsertAt(GetCount(), pConstantLine);

	return pConstantLine;
}

void CXTPChartAxisConstantLines::DoPropExchange(CXTPPropExchange* pPX)
{
	if (pPX->IsLoading())
	{
		RemoveAll();

		CXTPPropExchangeEnumeratorPtr pEnumerator(pPX->GetEnumerator(_T("ConstantLine")));
		POSITION pos = pEnumerator->GetPosition(0);

		while (pos)
		{
			CXTPPropExchangeSection pxItem(pEnumerator->GetNext(pos));


			CXTPChartAxisConstantLine* pElement = new CXTPChartAxisConstantLine();
			Add(pElement);

			pElement->DoPropExchange(&pxItem);
		}
	}
	else
	{
		CXTPPropExchangeEnumeratorPtr pEnumerator(pPX->GetEnumerator(_T("ConstantLine")));
		POSITION pos = pEnumerator->GetPosition((int)m_arrElements.GetSize());

		for (int i = 0; i < GetCount(); i++)
		{
			CXTPPropExchangeSection pxItem(pEnumerator->GetNext(pos));

			m_arrElements[i]->DoPropExchange(&pxItem);
		}
	}
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartAxisConstantLine, CXTPChartElement)
	DISP_PROPERTY_EX_ID(CXTPChartAxisConstantLine, "Text", DISPID_VALUE, OleGetText, OleSetText, VT_BSTR)
	DISP_PROPERTY_EX_ID(CXTPChartAxisConstantLine, "AxisValue", 1, OleGetAxisValue, OleSetAxisValue, VT_VARIANT)

	DISP_PROPERTY_EX_ID(CXTPChartAxisConstantLine, "LegendVisible", 2, OleGetLegendVisible, OleSetLegendVisible, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPChartAxisConstantLine, "Visible", 3, OleGetVisible, OleSetVisible, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPChartAxisConstantLine, "ShowTextBelow", 4, OleGetShowTextBelow, OleSetShowTextBelow, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPChartAxisConstantLine, "Alignment", 5, OleGetAlignment, OleSetAlignment, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPChartAxisConstantLine, "LineStyle", 6, OleGetLineStyle, SetNotSupported, VT_DISPATCH)

	DISP_PROPERTY_EX_ID(CXTPChartAxisConstantLine, "Font", DISPID_FONT, OleGetFont, SetNotSupported, VT_DISPATCH)

	DISP_PROPERTY_EX_ID(CXTPChartAxisConstantLine, "Color", 7, OleGetColor, OleSetColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPChartAxisConstantLine, "TextColor", 8, OleGetTextColor, OleSetTextColor, VT_COLOR)

	DISP_PROPERTY_EX_ID(CXTPChartAxisConstantLine, "ShowBehind", 9, IsShowBehind, SetShowBehind, VT_BOOL)

	DISP_PROPERTY_ID(CXTPChartAxisConstantLine, "LegendText", 10, m_strLegendName, VT_BSTR)

END_DISPATCH_MAP()

BOOL CXTPChartAxisConstantLine::OleGetLegendVisible()
{
	return m_bLegendVisible;
}

void CXTPChartAxisConstantLine::OleSetLegendVisible(BOOL bValue)
{
	SetLegendVisible(bValue);
}


BOOL CXTPChartAxisConstantLine::OleGetVisible()
{
	return m_bVisible;
}

void CXTPChartAxisConstantLine::OleSetVisible(BOOL bValue)
{
	SetVisible(bValue);
}

BOOL CXTPChartAxisConstantLine::OleGetShowTextBelow()
{
	return m_bShowTextBelow;
}

void CXTPChartAxisConstantLine::OleSetShowTextBelow(BOOL bValue)
{
	ShowTextBelow(bValue);
}

BOOL CXTPChartAxisConstantLine::OleGetAlignment()
{
	return m_nAlignment;
}

void CXTPChartAxisConstantLine::OleSetAlignment(int nValue)
{
	SetAlignment((XTPChartStringAlignment)nValue);
}

LPDISPATCH CXTPChartAxisConstantLine::OleGetLineStyle()
{
	return XTPGetDispatch(m_pLineStyle);
}


BSTR CXTPChartAxisConstantLine::OleGetText()
{
	return m_strText.AllocSysString();
}

void CXTPChartAxisConstantLine::OleSetText(LPCTSTR lpszText)
{
	SetText(lpszText);
}

VARIANT CXTPChartAxisConstantLine::OleGetAxisValue()
{
	VARIANT vt;
	if (!m_strAxisValue.IsEmpty())
	{
		vt.vt = VT_BSTR;
		vt.bstrVal = m_strAxisValue.AllocSysString();
	}
	else
	{
		vt.vt = VT_R8;
		vt.dblVal = m_dAxisValue;
	}
	return vt;
}

void CXTPChartAxisConstantLine::OleSetAxisValue(const VARIANT& var)
{
	if (IsStringVariant(&var))
		SetAxisValue(VariantToString(&var));
	else
		SetAxisValue(VariantToDoubleEx(&var));
}

LPDISPATCH CXTPChartAxisConstantLine::OleGetFont()
{
	return XTPGetDispatch(m_pFont);
}


OLE_COLOR CXTPChartAxisConstantLine::OleGetColor()
{
	return m_clrColor.ToOleColor();
}

OLE_COLOR CXTPChartAxisConstantLine::OleGetTextColor()
{
	return m_clrTextColor.ToOleColor();
}

void CXTPChartAxisConstantLine::OleSetColor(OLE_COLOR clr)
{
	SetColor(CXTPChartColor::FromOleColor(clr));
}

void CXTPChartAxisConstantLine::OleSetTextColor(OLE_COLOR clr)
{
	SetTextColor(CXTPChartColor::FromOleColor(clr));
}

// {E35BCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartAxisConstantLine =
{ 0xe35bcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartAxisConstantLine, CXTPChartElement)
	INTERFACE_PART(CXTPChartAxisConstantLine, IID_IChartAxisConstantLine, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartAxisConstantLine, IID_IChartAxisConstantLine)

//////////////////////////////////////////////////////////////////////////
//

BEGIN_DISPATCH_MAP(CXTPChartAxisConstantLines, CXTPChartElement)
	DISP_FUNCTION_ID(CXTPChartAxisConstantLines, "Count", 1, OleGetItemCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CXTPChartAxisConstantLines, "Item", DISPID_VALUE, OleGetItem, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPChartAxisConstantLines, "_NewEnum", DISPID_NEWENUM, OleNewEnum, VT_UNKNOWN, VTS_NONE)

	DISP_FUNCTION_ID(CXTPChartAxisConstantLines, "Add", 2, OleAdd, VT_DISPATCH, VTS_WBSTR)
	DISP_FUNCTION_ID(CXTPChartAxisConstantLines, "DeleteAll", 5, RemoveAll, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()

// {E34BCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartAxisConstantLines =
{ 0xe34bcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartAxisConstantLines, CXTPChartElement)
	INTERFACE_PART(CXTPChartAxisConstantLines, IID_IChartAxisConstantLines, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartAxisConstantLines, IID_IChartAxisConstantLines)


long CXTPChartAxisConstantLines::OleGetItemCount()
{
	return (long)GetCount();
}

#pragma warning(push)
#pragma warning(disable : 4702)

LPDISPATCH CXTPChartAxisConstantLines::OleGetItem(long nIndex)
{
	if (nIndex >= 0 && nIndex < GetCount())
	{
		CXTPChartAxisConstantLine* pItem = GetAt(nIndex);
		return pItem->GetIDispatch(TRUE);
	}
	AfxThrowOleException(E_INVALIDARG);
	return 0;
}

#pragma warning(pop)

LPDISPATCH CXTPChartAxisConstantLines::OleAdd(LPCOLESTR lpszConstantLine)
{
	CXTPChartAxisConstantLine* pConstantLine = Add(new CXTPChartAxisConstantLine());

	pConstantLine->SetText(lpszConstantLine);

	return XTPGetDispatch(pConstantLine);
}

IMPLEMENT_ENUM_VARIANT(CXTPChartAxisConstantLines)


#endif
