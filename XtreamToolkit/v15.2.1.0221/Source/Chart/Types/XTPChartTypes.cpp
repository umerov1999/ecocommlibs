// XTPChartTypes.cpp
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
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

#include "stdafx.h"

#include "Common/XTPPropExchange.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPVC80Helpers.h"

#include "XTPChartTypes.h"
#include "../Utils/XTPChartMathUtils.h"

//////////////////////////////////////////////////////////////////////////
// CXTPChartColor

const CXTPChartColor::ARGB CXTPChartColor::AliceBlue            = 0xFFF0F8FF;
const CXTPChartColor::ARGB CXTPChartColor::AntiqueWhite         = 0xFFFAEBD7;
const CXTPChartColor::ARGB CXTPChartColor::Aqua                 = 0xFF00FFFF;
const CXTPChartColor::ARGB CXTPChartColor::Aquamarine           = 0xFF7FFFD4;
const CXTPChartColor::ARGB CXTPChartColor::Azure                = 0xFFF0FFFF;
const CXTPChartColor::ARGB CXTPChartColor::Beige                = 0xFFF5F5DC;
const CXTPChartColor::ARGB CXTPChartColor::Bisque               = 0xFFFFE4C4;
const CXTPChartColor::ARGB CXTPChartColor::Black                = 0xFF000000;
const CXTPChartColor::ARGB CXTPChartColor::BlanchedAlmond       = 0xFFFFEBCD;
const CXTPChartColor::ARGB CXTPChartColor::Blue                 = 0xFF0000FF;
const CXTPChartColor::ARGB CXTPChartColor::BlueViolet           = 0xFF8A2BE2;
const CXTPChartColor::ARGB CXTPChartColor::Brown                = 0xFFA52A2A;
const CXTPChartColor::ARGB CXTPChartColor::BurlyWood            = 0xFFDEB887;
const CXTPChartColor::ARGB CXTPChartColor::CadetBlue            = 0xFF5F9EA0;
const CXTPChartColor::ARGB CXTPChartColor::Chartreuse           = 0xFF7FFF00;
const CXTPChartColor::ARGB CXTPChartColor::Chocolate            = 0xFFD2691E;
const CXTPChartColor::ARGB CXTPChartColor::Coral                = 0xFFFF7F50;
const CXTPChartColor::ARGB CXTPChartColor::CornflowerBlue       = 0xFF6495ED;
const CXTPChartColor::ARGB CXTPChartColor::Cornsilk             = 0xFFFFF8DC;
const CXTPChartColor::ARGB CXTPChartColor::Crimson              = 0xFFDC143C;
const CXTPChartColor::ARGB CXTPChartColor::Cyan                 = 0xFF00FFFF;
const CXTPChartColor::ARGB CXTPChartColor::DarkBlue             = 0xFF00008B;
const CXTPChartColor::ARGB CXTPChartColor::DarkCyan             = 0xFF008B8B;
const CXTPChartColor::ARGB CXTPChartColor::DarkGoldenrod        = 0xFFB8860B;
const CXTPChartColor::ARGB CXTPChartColor::DarkGray             = 0xFFA9A9A9;
const CXTPChartColor::ARGB CXTPChartColor::DarkGreen            = 0xFF006400;
const CXTPChartColor::ARGB CXTPChartColor::DarkKhaki            = 0xFFBDB76B;
const CXTPChartColor::ARGB CXTPChartColor::DarkMagenta          = 0xFF8B008B;
const CXTPChartColor::ARGB CXTPChartColor::DarkOliveGreen       = 0xFF556B2F;
const CXTPChartColor::ARGB CXTPChartColor::DarkOrange           = 0xFFFF8C00;
const CXTPChartColor::ARGB CXTPChartColor::DarkOrchid           = 0xFF9932CC;
const CXTPChartColor::ARGB CXTPChartColor::DarkRed              = 0xFF8B0000;
const CXTPChartColor::ARGB CXTPChartColor::DarkSalmon           = 0xFFE9967A;
const CXTPChartColor::ARGB CXTPChartColor::DarkSeaGreen         = 0xFF8FBC8B;
const CXTPChartColor::ARGB CXTPChartColor::DarkSlateBlue        = 0xFF483D8B;
const CXTPChartColor::ARGB CXTPChartColor::DarkSlateGray        = 0xFF2F4F4F;
const CXTPChartColor::ARGB CXTPChartColor::DarkTurquoise        = 0xFF00CED1;
const CXTPChartColor::ARGB CXTPChartColor::DarkViolet           = 0xFF9400D3;
const CXTPChartColor::ARGB CXTPChartColor::DeepPink             = 0xFFFF1493;
const CXTPChartColor::ARGB CXTPChartColor::DeepSkyBlue          = 0xFF00BFFF;
const CXTPChartColor::ARGB CXTPChartColor::DimGray              = 0xFF696969;
const CXTPChartColor::ARGB CXTPChartColor::DodgerBlue           = 0xFF1E90FF;
const CXTPChartColor::ARGB CXTPChartColor::Firebrick            = 0xFFB22222;
const CXTPChartColor::ARGB CXTPChartColor::FloralWhite          = 0xFFFFFAF0;
const CXTPChartColor::ARGB CXTPChartColor::ForestGreen          = 0xFF228B22;
const CXTPChartColor::ARGB CXTPChartColor::Fuchsia              = 0xFFFF00FF;
const CXTPChartColor::ARGB CXTPChartColor::Gainsboro            = 0xFFDCDCDC;
const CXTPChartColor::ARGB CXTPChartColor::GhostWhite           = 0xFFF8F8FF;
const CXTPChartColor::ARGB CXTPChartColor::Gold                 = 0xFFFFD700;
const CXTPChartColor::ARGB CXTPChartColor::Goldenrod            = 0xFFDAA520;
const CXTPChartColor::ARGB CXTPChartColor::Gray                 = 0xFF808080;
const CXTPChartColor::ARGB CXTPChartColor::Green                = 0xFF008000;
const CXTPChartColor::ARGB CXTPChartColor::GreenYellow          = 0xFFADFF2F;
const CXTPChartColor::ARGB CXTPChartColor::Honeydew             = 0xFFF0FFF0;
const CXTPChartColor::ARGB CXTPChartColor::HotPink              = 0xFFFF69B4;
const CXTPChartColor::ARGB CXTPChartColor::IndianRed            = 0xFFCD5C5C;
const CXTPChartColor::ARGB CXTPChartColor::Indigo               = 0xFF4B0082;
const CXTPChartColor::ARGB CXTPChartColor::Ivory                = 0xFFFFFFF0;
const CXTPChartColor::ARGB CXTPChartColor::Khaki                = 0xFFF0E68C;
const CXTPChartColor::ARGB CXTPChartColor::Lavender             = 0xFFE6E6FA;
const CXTPChartColor::ARGB CXTPChartColor::LavenderBlush        = 0xFFFFF0F5;
const CXTPChartColor::ARGB CXTPChartColor::LawnGreen            = 0xFF7CFC00;
const CXTPChartColor::ARGB CXTPChartColor::LemonChiffon         = 0xFFFFFACD;
const CXTPChartColor::ARGB CXTPChartColor::LightBlue            = 0xFFADD8E6;
const CXTPChartColor::ARGB CXTPChartColor::LightCoral           = 0xFFF08080;
const CXTPChartColor::ARGB CXTPChartColor::LightCyan            = 0xFFE0FFFF;
const CXTPChartColor::ARGB CXTPChartColor::LightGoldenrodYellow = 0xFFFAFAD2;
const CXTPChartColor::ARGB CXTPChartColor::LightGray            = 0xFFD3D3D3;
const CXTPChartColor::ARGB CXTPChartColor::LightGreen           = 0xFF90EE90;
const CXTPChartColor::ARGB CXTPChartColor::LightPink            = 0xFFFFB6C1;
const CXTPChartColor::ARGB CXTPChartColor::LightSalmon          = 0xFFFFA07A;
const CXTPChartColor::ARGB CXTPChartColor::LightSeaGreen        = 0xFF20B2AA;
const CXTPChartColor::ARGB CXTPChartColor::LightSkyBlue         = 0xFF87CEFA;
const CXTPChartColor::ARGB CXTPChartColor::LightSlateGray       = 0xFF778899;
const CXTPChartColor::ARGB CXTPChartColor::LightSteelBlue       = 0xFFB0C4DE;
const CXTPChartColor::ARGB CXTPChartColor::LightYellow          = 0xFFFFFFE0;
const CXTPChartColor::ARGB CXTPChartColor::Lime                 = 0xFF00FF00;
const CXTPChartColor::ARGB CXTPChartColor::LimeGreen            = 0xFF32CD32;
const CXTPChartColor::ARGB CXTPChartColor::Linen                = 0xFFFAF0E6;
const CXTPChartColor::ARGB CXTPChartColor::Magenta              = 0xFFFF00FF;
const CXTPChartColor::ARGB CXTPChartColor::Maroon               = 0xFF800000;
const CXTPChartColor::ARGB CXTPChartColor::MediumAquamarine     = 0xFF66CDAA;
const CXTPChartColor::ARGB CXTPChartColor::MediumBlue           = 0xFF0000CD;
const CXTPChartColor::ARGB CXTPChartColor::MediumOrchid         = 0xFFBA55D3;
const CXTPChartColor::ARGB CXTPChartColor::MediumPurple         = 0xFF9370DB;
const CXTPChartColor::ARGB CXTPChartColor::MediumSeaGreen       = 0xFF3CB371;
const CXTPChartColor::ARGB CXTPChartColor::MediumSlateBlue      = 0xFF7B68EE;
const CXTPChartColor::ARGB CXTPChartColor::MediumSpringGreen    = 0xFF00FA9A;
const CXTPChartColor::ARGB CXTPChartColor::MediumTurquoise      = 0xFF48D1CC;
const CXTPChartColor::ARGB CXTPChartColor::MediumVioletRed      = 0xFFC71585;
const CXTPChartColor::ARGB CXTPChartColor::MidnightBlue         = 0xFF191970;
const CXTPChartColor::ARGB CXTPChartColor::MintCream            = 0xFFF5FFFA;
const CXTPChartColor::ARGB CXTPChartColor::MistyRose            = 0xFFFFE4E1;
const CXTPChartColor::ARGB CXTPChartColor::Moccas            = 0xFFFFE4B5;
const CXTPChartColor::ARGB CXTPChartColor::NavajoWhite          = 0xFFFFDEAD;
const CXTPChartColor::ARGB CXTPChartColor::Navy                 = 0xFF000080;
const CXTPChartColor::ARGB CXTPChartColor::OldLace              = 0xFFFDF5E6;
const CXTPChartColor::ARGB CXTPChartColor::Olive                = 0xFF808000;
const CXTPChartColor::ARGB CXTPChartColor::OliveDrab            = 0xFF6B8E23;
const CXTPChartColor::ARGB CXTPChartColor::Orange               = 0xFFFFA500;
const CXTPChartColor::ARGB CXTPChartColor::OrangeRed            = 0xFFFF4500;
const CXTPChartColor::ARGB CXTPChartColor::Orchid               = 0xFFDA70D6;
const CXTPChartColor::ARGB CXTPChartColor::PaleGoldenrod        = 0xFFEEE8AA;
const CXTPChartColor::ARGB CXTPChartColor::PaleGreen            = 0xFF98FB98;
const CXTPChartColor::ARGB CXTPChartColor::PaleTurquoise        = 0xFFAFEEEE;
const CXTPChartColor::ARGB CXTPChartColor::PaleVioletRed        = 0xFFDB7093;
const CXTPChartColor::ARGB CXTPChartColor::PapayaWhip           = 0xFFFFEFD5;
const CXTPChartColor::ARGB CXTPChartColor::PeachPuff            = 0xFFFFDAB9;
const CXTPChartColor::ARGB CXTPChartColor::Peru                 = 0xFFCD853F;
const CXTPChartColor::ARGB CXTPChartColor::Pink                 = 0xFFFFC0CB;
const CXTPChartColor::ARGB CXTPChartColor::Plum                 = 0xFFDDA0DD;
const CXTPChartColor::ARGB CXTPChartColor::PowderBlue           = 0xFFB0E0E6;
const CXTPChartColor::ARGB CXTPChartColor::Purple               = 0xFF800080;
const CXTPChartColor::ARGB CXTPChartColor::Red                  = 0xFFFF0000;
const CXTPChartColor::ARGB CXTPChartColor::RosyBrown            = 0xFFBC8F8F;
const CXTPChartColor::ARGB CXTPChartColor::RoyalBlue            = 0xFF4169E1;
const CXTPChartColor::ARGB CXTPChartColor::SaddleBrown          = 0xFF8B4513;
const CXTPChartColor::ARGB CXTPChartColor::Salmon               = 0xFFFA8072;
const CXTPChartColor::ARGB CXTPChartColor::SandyBrown           = 0xFFF4A460;
const CXTPChartColor::ARGB CXTPChartColor::SeaGreen             = 0xFF2E8B57;
const CXTPChartColor::ARGB CXTPChartColor::SeaShell             = 0xFFFFF5EE;
const CXTPChartColor::ARGB CXTPChartColor::Sienna               = 0xFFA0522D;
const CXTPChartColor::ARGB CXTPChartColor::Silver               = 0xFFC0C0C0;
const CXTPChartColor::ARGB CXTPChartColor::SkyBlue              = 0xFF87CEEB;
const CXTPChartColor::ARGB CXTPChartColor::SlateBlue            = 0xFF6A5ACD;
const CXTPChartColor::ARGB CXTPChartColor::SlateGray            = 0xFF708090;
const CXTPChartColor::ARGB CXTPChartColor::Snow                 = 0xFFFFFAFA;
const CXTPChartColor::ARGB CXTPChartColor::SpringGreen          = 0xFF00FF7F;
const CXTPChartColor::ARGB CXTPChartColor::SteelBlue            = 0xFF4682B4;
const CXTPChartColor::ARGB CXTPChartColor::Tan                  = 0xFFD2B48C;
const CXTPChartColor::ARGB CXTPChartColor::Teal                 = 0xFF008080;
const CXTPChartColor::ARGB CXTPChartColor::Thistle              = 0xFFD8BFD8;
const CXTPChartColor::ARGB CXTPChartColor::Tomato               = 0xFFFF6347;
const CXTPChartColor::ARGB CXTPChartColor::Transparent          = 0x00FFFFFF;
const CXTPChartColor::ARGB CXTPChartColor::Turquoise            = 0xFF40E0D0;
const CXTPChartColor::ARGB CXTPChartColor::Violet               = 0xFFEE82EE;
const CXTPChartColor::ARGB CXTPChartColor::Wheat                = 0xFFF5DEB3;
const CXTPChartColor::ARGB CXTPChartColor::White                = 0xFFFFFFFF;
const CXTPChartColor::ARGB CXTPChartColor::WhiteSmoke           = 0xFFF5F5F5;
const CXTPChartColor::ARGB CXTPChartColor::Yellow               = 0xFFFFFF00;
const CXTPChartColor::ARGB CXTPChartColor::YellowGreen          = 0xFF9ACD32;
const CXTPChartColor::ARGB CXTPChartColor::Empty                = 0x00000000;

void CXTPChartColor::SetFromString(LPCTSTR lpsz)
{
	int clr[4];
	int res = SCANF_S(lpsz, _T("%d, %d, %d, %d"), &clr[0], &clr[1], &clr[2], &clr[3]);

	if (res == 3)
	{
		Argb = MakeARGB(255, (BYTE)clr[0], (BYTE)clr[1], (BYTE)clr[2]);
	}
	else if (res == 4)
	{
		Argb = MakeARGB((BYTE)clr[0], (BYTE)clr[1], (BYTE)clr[2], (BYTE)clr[3]);

	}
	else
	{
		ASSERT(FALSE);
	}
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartRectF

void CXTPChartRectF::Round()
{
	int r = CXTPChartMathUtils::Round(X + Width);
	int d = CXTPChartMathUtils::Round(Y + Height);

	X = (float)CXTPChartMathUtils::Round(X);
	Y = (float)CXTPChartMathUtils::Round(Y);
	Width = r - X;
	Height = d - Y;
}


//////////////////////////////////////////////////////////////////////////
// CXTPChartFont

CXTPChartFont::CXTPChartFont()
{
	ZeroMemory(&m_lf, sizeof(LOGFONT));

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPChartFont::CXTPChartFont(LOGFONT* pLogFont)
{
	if (pLogFont)
	{
		m_lf = *pLogFont;
	}
	else
	{

		ZeroMemory(&m_lf, sizeof(LOGFONT));
	}

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}


CXTPChartFont::~CXTPChartFont()
{
}

void CXTPChartFont::Release()
{
	InternalRelease();
}

void CXTPChartFont::SetLogFont(LOGFONT* lf)
{
	m_lf = *lf;
}

CXTPChartFont* CXTPChartFont::GetTahoma18()
{
	LOGFONT lf;
	ZeroMemory(&lf, sizeof(LOGFONT));
	lf.lfHeight = 29;
	lf.lfWeight = FW_NORMAL;
	STRCPY_S(lf.lfFaceName, LF_FACESIZE, _T("Tahoma"));

	return new CXTPChartFont(&lf);
}

CXTPChartFont* CXTPChartFont::GetTahoma8()
{
	LOGFONT lf;
	ZeroMemory(&lf, sizeof(LOGFONT));
	lf.lfHeight = -11;
	lf.lfWeight = FW_NORMAL;
	STRCPY_S(lf.lfFaceName, LF_FACESIZE, _T("Tahoma"));

	return new CXTPChartFont(&lf);
}


CXTPChartFont* CXTPChartFont::GetTahoma12()
{
	LOGFONT lf;
	ZeroMemory(&lf, sizeof(LOGFONT));
	lf.lfHeight = -16;
	lf.lfWeight = FW_NORMAL;
	STRCPY_S(lf.lfFaceName, LF_FACESIZE, _T("Tahoma"));

	return new CXTPChartFont(&lf);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartColor

CXTPChartColor CXTPChartColor::GetDarkColor() const
{
	COLORREF clrBackground = RGB(GetRed(), GetGreen(), GetBlue());

	DWORD dwHSLBackground = CXTPDrawHelpers::RGBtoHSL(clrBackground);
	DWORD dwL = GetBValue(dwHSLBackground);

	DWORD dwDark = MulDiv(dwL, 3, 4);
	COLORREF clrDark = CXTPDrawHelpers::HSLtoRGB(RGB(GetRValue(dwHSLBackground), GetGValue(dwHSLBackground), dwDark));

	return CXTPChartColor(GetAlpha(), GetRValue(clrDark), GetGValue(clrDark), GetBValue(clrDark));
}

//////////////////////////////////////////////////////////////////////////
//

BOOL AFX_CDECL PX_Color(CXTPPropExchange* pPX, LPCTSTR pszPropName, CXTPChartColor& clrValue)
{
	if (pPX->IsStoring())
	{
		CString str;
		if (!clrValue.IsEmpty())
			str.Format(_T("%d, %d, %d"), clrValue.GetRed(), clrValue.GetGreen(), clrValue.GetBlue());

		return PX_String(pPX, pszPropName, str, _T(""));
	}
	else
	{
		CString str;
		if (!PX_String(pPX, pszPropName, str, _T("")))
			return FALSE;

		if (str.IsEmpty())
			clrValue = CXTPChartColor::Empty;
		else
			clrValue.SetFromString(str);
	}

	return TRUE;
}


BOOL AFX_CDECL PX_Font(CXTPPropExchange* pPX, LPCTSTR pszPropName, CXTPChartFont* pFont)
{
	CXTPPropExchangeSection sec(pPX->GetSection(pszPropName));

	LOGFONT& lf = pFont->m_lf;

	CString strFaceName = lf.lfFaceName;
	int nHeight = lf.lfHeight;
	int nWeight = lf.lfWeight;

	PX_String(&sec, _T("FaceName"), strFaceName, _T("Tahoma"));
	PX_Int(&sec, _T("Height"), nHeight);
	PX_Int(&sec, _T("Weight"), nWeight, FW_NORMAL);

	if (!pPX->IsStoring())
	{
		lf.lfHeight = nHeight;
		lf.lfWeight = nWeight;
		STRCPY_S(lf.lfFaceName, LF_FACESIZE, strFaceName);
	}

	return TRUE;
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartFont, CCmdTarget)
	DISP_PROPERTY_EX_ID(CXTPChartFont, "StdFont", 1, OleGetFont, OleSetFont, VT_DISPATCH)
END_DISPATCH_MAP()


// {57DBCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartFont =
{ 0x57dbcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartFont, CCmdTarget)
	INTERFACE_PART(CXTPChartFont, IID_IChartFont, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartFont, IID_IChartFont)



void CXTPChartFont::OleSetFont(LPFONTDISP pFontDisp)
{
	ASSERT((pFontDisp == NULL) ||
		AfxIsValidAddress(pFontDisp, sizeof(IDispatch), FALSE));

	LOGFONT lf;
	if (AxGetLogFontFromDispatch(&lf, pFontDisp))
	{
		SetLogFont(&lf);
	}
	else
	{

	}
}

LPFONTDISP CXTPChartFont::OleGetFont()
{
	return AxCreateOleFont(m_lf, this, (LPFNFONTCHANGED)&CXTPChartFont::OleSetFont);
}



OLE_COLOR CXTPChartColor::ToOleColor()
{
	if (IsEmpty())
		return GetValue();

	return RGB(GetR(), GetG(), GetB());

}

CXTPChartColor AFX_CDECL CXTPChartColor::FromOleColor(OLE_COLOR clr)
{
	CXTPChartColor res;
	res.SetFromCOLORREF(AxTranslateColor(clr));

	return res;
}


double AFX_CDECL VariantToDouble(const VARIANT* pVariant)
{
	VARIANT var;
	::VariantInit(&var);

	double res = 0;

	if (SUCCEEDED(VariantChangeType(&var, (VARIANT*)pVariant, 0, VT_R8)))
	{
		res = V_R8(&var);
	}

	::VariantClear(&var);

	return res;
}

double AFX_CDECL VariantToDoubleEx(const VARIANT* pVariant)
{
	if (pVariant->vt == VT_NULL || pVariant->vt == VT_EMPTY  || pVariant->vt == VT_ERROR)
		return CXTPChartMathUtils::GetNaN();

	return VariantToDouble(pVariant);
}

extern BOOL AFX_CDECL IsStringVariant(const VARIANT* pVariant)
{
	return pVariant->vt == VT_BSTR || pVariant->vt == (VT_BSTR | VT_BYREF);
}


#endif
