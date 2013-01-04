// XTPChartNumberFormat.cpp
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
#include "Common/XTPVC80Helpers.h"

#include "../Types/XTPChartTypes.h"
#include "../XTPChartDefines.h"
#include "../XTPChartElement.h"
#include "../Utils/XTPChartMathUtils.h"

#include "../XTPChartSeriesPoint.h"
#include "../XTPChartSeries.h"

#include "XTPChartNumberFormat.h"

#ifndef LOCALE_SYEARMONTH
#define LOCALE_SYEARMONTH             0x00001006   // year month format string
#endif



//////////////////////////////////////////////////////////////////////////
// CXTPChartNumberFormat

CXTPChartNumberFormat::CXTPChartNumberFormat(CXTPChartElement* pOwner)
{
	m_pOwner = pOwner;

	m_strPattern = _T("");
	m_nCategory = xtpChartGeneral;
	m_nDecimalPlaces = -1;
	m_bUseThousandSeparator = TRUE;
	m_nLocale = LOCALE_USER_DEFAULT;



	memset(&m_fmtNumber, 0, sizeof(m_fmtNumber));
	m_fmtNumber.lpDecimalSep = new TCHAR[10];
	m_fmtNumber.lpThousandSep = new TCHAR[10];

	CreateNumberFormat();

	memset(&m_fmtCurrency, 0, sizeof(m_fmtCurrency));
	m_fmtCurrency.lpDecimalSep = new TCHAR[10];
	m_fmtCurrency.lpThousandSep = new TCHAR[10];
	m_fmtCurrency.lpCurrencySymbol = new TCHAR[10];

	CreateCurrencyFormat();

	m_nDateTimeCategory = xtpChartDateTimeShortDate;
	CreateDateTimeFormat();


#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif

}

CXTPChartNumberFormat::~CXTPChartNumberFormat()
{
	delete[] m_fmtNumber.lpDecimalSep;
	delete[] m_fmtNumber.lpThousandSep;

	delete[] m_fmtCurrency.lpDecimalSep;
	delete[] m_fmtCurrency.lpThousandSep;
	delete[] m_fmtCurrency.lpCurrencySymbol;
}

void CXTPChartNumberFormat::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_String(pPX, _T("Pattern"), m_strPattern,  _T(""));
	PX_Enum(pPX, _T("Category"), m_nCategory, xtpChartGeneral);
	PX_Int(pPX, _T("DecimalPlaces"), m_nDecimalPlaces, -1);
	PX_Bool(pPX, _T("UseThousandSeparator"), m_bUseThousandSeparator, TRUE);
	PX_DWord(pPX, _T("Locale"), m_nLocale, LOCALE_USER_DEFAULT);
	PX_String(pPX, _T("CurrencySymbol"), m_strCurrencySymbol,  _T(""));

	PX_Enum(pPX, _T("DateTimeCategory"), m_nDateTimeCategory, xtpChartDateTimeShortDate);

	PX_String(pPX, _T("DateTimeFormat"), m_strDateTimeFormat,  _T(""));

	if (pPX->IsLoading())
	{
		CreateNumberFormat();
		CreateCurrencyFormat();
		CreateDateTimeFormat();
	}
}


CString CXTPChartNumberFormat::GetLocaleString(LCTYPE LCType, int nMaxLength)
{
	CString strResult;

	::GetLocaleInfo(m_nLocale, LCType , strResult.GetBufferSetLength(nMaxLength), nMaxLength);
	strResult.ReleaseBuffer();

	return strResult;
}

int CXTPChartNumberFormat::GetLocaleLong(LCTYPE LCType) const
{
	TCHAR szResult[5];

	int nResult = ::GetLocaleInfo(m_nLocale, LCType , szResult, 4);
	ASSERT(nResult >= 2 && nResult <= 3);

	return _ttoi(szResult);
}

void CXTPChartNumberFormat::CreateCurrencyFormat()
{
	m_fmtCurrency.NumDigits = m_nDecimalPlaces == -1 ? GetLocaleLong(LOCALE_IDIGITS) : m_nDecimalPlaces;
	m_fmtCurrency.LeadingZero = GetLocaleLong(LOCALE_ILZERO);
	m_fmtCurrency.Grouping = 3;
	m_fmtCurrency.NegativeOrder = GetLocaleLong(LOCALE_INEGCURR);


	m_fmtCurrency.PositiveOrder = GetLocaleLong(LOCALE_ICURRENCY);

	GetLocaleInfo(m_nLocale, LOCALE_SDECIMAL, m_fmtCurrency.lpDecimalSep, 10);

	if (m_bUseThousandSeparator)
		GetLocaleInfo(m_nLocale, LOCALE_STHOUSAND, m_fmtCurrency.lpThousandSep, 10);
	else
		m_fmtCurrency.lpThousandSep[0] = 0;

	if (m_strCurrencySymbol.IsEmpty())
		GetLocaleInfo(m_nLocale, LOCALE_SCURRENCY, m_fmtCurrency.lpCurrencySymbol, 10);
	else
		STRNCPY_S(m_fmtCurrency.lpCurrencySymbol, 10, m_strCurrencySymbol, 9);
}

void CXTPChartNumberFormat::CreateNumberFormat()
{
	m_fmtNumber.NumDigits = m_nDecimalPlaces == -1 ? GetLocaleLong(LOCALE_IDIGITS) : m_nDecimalPlaces;
	m_fmtNumber.LeadingZero = GetLocaleLong(LOCALE_ILZERO);;
	m_fmtNumber.Grouping = 3;
	m_fmtNumber.NegativeOrder = GetLocaleLong(LOCALE_INEGNUMBER);

	GetLocaleInfo(m_nLocale, LOCALE_SDECIMAL, m_fmtNumber.lpDecimalSep, 10);

	if (m_bUseThousandSeparator)
		GetLocaleInfo(m_nLocale, LOCALE_STHOUSAND, m_fmtNumber.lpThousandSep, 10);
	else
		m_fmtNumber.lpThousandSep[0] = 0;

}

CString CXTPChartNumberFormat::DoubleToString(double dValue) const
{
	CString strResult;

	switch (m_nCategory)
	{
	case xtpChartGeneral:
		{
			strResult.Format(_T("%g"), dValue);

			if (m_fmtNumber.lpDecimalSep[0] == _T(','))
				strResult.Replace(_T('.'), _T(','));

			return strResult;
		}

	case xtpChartNumber:
		{
			CString strTemp;
			strTemp.Format(_T("%lf"), dValue);
			strTemp.Replace(_T(','), _T('.'));

			GetNumberFormat(m_nLocale, 0, (LPCTSTR)strTemp, &m_fmtNumber, strResult.GetBuffer(40), 40);
			strResult.ReleaseBuffer();
			return strResult;
		}

	case xtpChartCurrency:
		{
			CString strTemp;
			strTemp.Format(_T("%lf"), dValue);
			strTemp.Replace(_T(','), _T('.'));

			GetCurrencyFormat(m_nLocale, 0, (LPCTSTR)strTemp, &m_fmtCurrency, strResult.GetBuffer(40), 40);
			strResult.ReleaseBuffer();
			return strResult;
		}

	case xtpChartPercentage:
		{
			CString strTemp;
			strTemp.Format(_T("%lf"), dValue * 100);
			strTemp.Replace(_T(','), _T('.'));

			GetNumberFormat(m_nLocale, 0, (LPCTSTR)strTemp, &m_fmtNumber, strResult.GetBuffer(40), 40);
			strResult.ReleaseBuffer();

			strResult += _T('%');
			return strResult;
		}

	case xtpChartScientific:
		{
			if (m_nDecimalPlaces == -1)
				strResult.Format(_T("%e"), dValue);
			else
			{
				CString strFormat;
				strFormat.Format(_T("%%.%de"), m_nDecimalPlaces);

				strResult.Format(strFormat, dValue);

			}
			return strResult;
		}
	}

	return strResult;
}

CString CXTPChartNumberFormat::FormatNumber(double dValue) const
{
	if (CXTPChartMathUtils::Compare(dValue, 0) == 0)
		dValue = 0;

	CString strResult;

	if (!m_strPattern.IsEmpty())
	{
		strResult.Format(m_strPattern, dValue);

		int nIndex = strResult.Find(_T("{V}"));
		if (nIndex != -1)
		{
			CString str = DoubleToString(dValue);

			strResult.Replace(_T("{V}"), str);
		}
	}
	else
	{
		strResult = DoubleToString(dValue);
	}

	return strResult;
}

CString CXTPChartNumberFormat::FormatString(LPCTSTR lpsz) const
{
	if (m_strPattern.IsEmpty())
		return lpsz;


	CXTPChartString strResult;
	strResult.Format(m_strPattern, lpsz);

	int nIndex = strResult.Find(_T("{V}"));
	if (nIndex != -1)
	{
		strResult.Replace(_T("{V}"), lpsz);
	}

	return strResult;
}

CString CXTPChartNumberFormat::FormatPoint(CXTPChartSeriesPoint* pSeriesPoint, double dValue) const
{
	if (CXTPChartMathUtils::Compare(dValue, 0) == 0)
		dValue = 0;

	CString strResult;

	if (!m_strPattern.IsEmpty())
	{
		strResult.Format(m_strPattern, dValue);

		int nIndex = strResult.Find(_T("{V}"));
		if (nIndex != -1)
		{
			CString str = DoubleToString(dValue);

			strResult.Replace(_T("{V}"), str);
		}

		nIndex = strResult.Find(_T("{A}"));
		if (nIndex != -1)
		{
			CString strArgument = pSeriesPoint->GetArgument();

			if (pSeriesPoint->GetSeries()->GetArgumentScaleType() == xtpChartScaleDateTime)
			{
				COleDateTime dt((DATE)pSeriesPoint->GetArgumentValue());
				strArgument = FormatDate(dt);
			}

			strResult.Replace(_T("{A}"), strArgument);
		}

		strResult.Replace(_T("{S}"), pSeriesPoint->GetSeries()->GetName());
	}
	else
	{
		strResult = DoubleToString(dValue);
	}

	return strResult;
}

void CXTPChartNumberFormat::CreateDateTimeFormat()
{
	m_strActiveDateTimeFormat = m_strDateTimeFormat;

	if (m_nDateTimeCategory == xtpChartDateTimeLongDate)
	{
		m_strActiveDateTimeFormat = GetLocaleString(LOCALE_SLONGDATE, 255);
	}
	else if (m_nDateTimeCategory == xtpChartDateTimeShortDate)
	{
		m_strActiveDateTimeFormat = GetLocaleString(LOCALE_SSHORTDATE, 255);
	}
	else if (m_nDateTimeCategory == xtpChartDateTimeMonthAndDay)
	{
		BOOL bDayMonth = GetLocaleLong(LOCALE_ILDATE) == 1;

		if (bDayMonth)
			m_strActiveDateTimeFormat = _T("d MMMM ");
		else
			m_strActiveDateTimeFormat = _T("MMMM d");
	}
	else if (m_nDateTimeCategory == xtpChartDateTimeMonthAndYear)
	{
		m_strActiveDateTimeFormat = GetLocaleString(LOCALE_SYEARMONTH, 255);
	}
	else if (m_nDateTimeCategory == xtpChartDateTimeLongTime || m_nDateTimeCategory == xtpChartDateTimeShortTime)
	{
		////////////////////////////////////////////////////////////////////////////
		//  LOCALE_STIME    // time separator
		//
		//  LOCALE_ITLZERO  //leading zeros in time field
		//      0 No leading zeros for hours.
		//      1 Leading zeros for hours.
		//
		//  LOCALE_ITIME    // time format specifier
		//      0 AM / PM 12-hour format.
		//      1 24-hour format.
		//
		//  LOCALE_ITIMEMARKPOSN    // time marker position
		//      0 Use as suffix.
		//      1 Use as prefix.
		//
		//  LOCALE_S1159    // AM designator
		//  LOCALE_S2359    // PM designator
		////////////////////////////////////////////////////////////////////////////

		CString strTmp;

		CString strTimeSeparator = GetLocaleString(LOCALE_STIME, 16);

		BOOL b24HFormat = GetLocaleLong(LOCALE_ITIME) == 1;

		BOOL bHLZero = GetLocaleLong(LOCALE_ITLZERO) == 1;

		BOOL bAMPMPrefix = GetLocaleLong(LOCALE_ITIMEMARKPOSN) == 1;

		//---------------------------------------------------------------------------
		CString strTimeFormat;
		strTimeFormat += strTimeSeparator;
		strTimeFormat += _T("mm");

		if (b24HFormat)
		{
			strTmp = bHLZero ? _T("HH") : _T("H");
			strTimeFormat = strTmp + strTimeFormat;
		}
		else //12 AM/PM Format
		{
			strTmp = bHLZero ? _T("hh") : _T("h");
			strTimeFormat = strTmp + strTimeFormat;

		}

		if (m_nDateTimeCategory == xtpChartDateTimeLongTime)
			strTimeFormat = strTimeFormat + strTimeSeparator + _T("ss");

		if (!b24HFormat)
		{
			if (bAMPMPrefix)
			{
				strTimeFormat = _T("tt ") + strTimeFormat;
			}
			else
			{
				strTimeFormat = strTimeFormat + _T(" tt");
			}
		}

		m_strActiveDateTimeFormat = strTimeFormat;
	}
}

CString CXTPChartNumberFormat::FormatDate(COleDateTime& dt) const
{
	CString strResult;

	if (m_nDateTimeCategory == xtpChartDateTimeGeneral)
	{
		return dt.Format((DWORD)0, m_nLocale);
	}
	else if (m_nDateTimeCategory != xtpChartDateTimeCustom)
	{
		SYSTEMTIME st;
		if (dt.GetAsSystemTime(st))
		{
			if (m_nDateTimeCategory == xtpChartDateTimeLongTime || m_nDateTimeCategory == xtpChartDateTimeShortTime)
				::GetTimeFormat(m_nLocale, 0, &st, m_strActiveDateTimeFormat, strResult.GetBufferSetLength(255), 255);
			else
				::GetDateFormat(m_nLocale, 0, &st, m_strActiveDateTimeFormat, strResult.GetBufferSetLength(255), 255);
			strResult.ReleaseBuffer();
		}
	}
	else
	{
		if (m_strActiveDateTimeFormat.IsEmpty())
		{
			return dt.Format(VAR_DATEVALUEONLY);
		}
		else
		{
			if (m_strActiveDateTimeFormat.Find(_T("%")) >= 0)
				return dt.Format(m_strActiveDateTimeFormat);

			SYSTEMTIME st;
			if (dt.GetAsSystemTime(st))
			{
				CString strPattern;

				::GetTimeFormat(m_nLocale, 0, &st, m_strActiveDateTimeFormat, strPattern.GetBufferSetLength(255), 255);
				strPattern.ReleaseBuffer();

				::GetDateFormat(m_nLocale, 0, &st, strPattern, strResult.GetBufferSetLength(255), 255);
				strResult.ReleaseBuffer();
			}
		}
	}

	return strResult;
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartNumberFormat, CCmdTarget)
	DISP_PROPERTY_EX_ID(CXTPChartNumberFormat, "Pattern", DISPID_VALUE, OleGetPattern, OleSetPattern, VT_BSTR)
	DISP_PROPERTY_EX_ID(CXTPChartNumberFormat, "Category", 2, GetCategory, SetCategory, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPChartNumberFormat, "DecimalPlaces", 3, GetDecimalPlaces, SetDecimalPlaces, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPChartNumberFormat, "UseThousandSeparator", 4, GetUseThousandSeparator, SetUseThousandSeparator, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPChartNumberFormat, "Locale", 5, GetLocale, SetLocale, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPChartNumberFormat, "CurrencySymbol", 6, OleGetCurrencySymbol, OleSetCurrencySymbol, VT_BSTR)
	DISP_PROPERTY_EX_ID(CXTPChartNumberFormat, "DateTimeCategory", 7, GetDateTimeCategory, SetDateTimeCategory, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPChartNumberFormat, "DateTimeFormat", 8, OleGetDateDateTimeFormat, SetDateTimeFormat, VT_BSTR)
END_DISPATCH_MAP()


// {A90BCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartNumberFormat =
{ 0xa90bcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartNumberFormat, CCmdTarget)
	INTERFACE_PART(CXTPChartNumberFormat, IID_IChartNumberFormat, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartNumberFormat, IID_IChartNumberFormat)


BSTR CXTPChartNumberFormat::OleGetPattern()
{
	return m_strPattern.AllocSysString();
}

void CXTPChartNumberFormat::OleSetPattern(LPCTSTR lpszValue)
{
	SetPattern(lpszValue);
}


BSTR CXTPChartNumberFormat::OleGetCurrencySymbol()
{
	return m_strCurrencySymbol.AllocSysString();
}

void CXTPChartNumberFormat::OleSetCurrencySymbol(LPCTSTR lpszValue)
{
	SetCurrencySymbol(lpszValue);
}

BSTR CXTPChartNumberFormat::OleGetDateDateTimeFormat()
{
	return m_strDateTimeFormat.AllocSysString();
}


#endif
