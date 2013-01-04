// XTPChartNumberFormat.h
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

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPCHARTNUMBERFORMAT_H__)
#define __XTPCHARTNUMBERFORMAT_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartColor;
class CXTPChartDeviceCommand;
class CXTPPropExchange;
class CXTPChartSeriesPoint;


//===========================================================================
// Summary:
//     Format category used by CXTPChartNumberFormat
//===========================================================================
enum XTPChartNumberFormatCategory
{
	xtpChartGeneral,        // General (100000.123)
	xtpChartNumber,         // Number  (10,000,000.12)
	xtpChartCurrency,       // Currency ($10,000,000.12)
	xtpChartPercentage,     // Percent  (99.12%)
	xtpChartScientific       // Scientific (12.12e+005)
};


//===========================================================================
// Summary:
//     DateTime Format category used by CXTPChartNumberFormat
//===========================================================================
enum XTPChartDateTimeFormatCategory
{
	xtpChartDateTimeGeneral,
	xtpChartDateTimeLongDate,
	xtpChartDateTimeShortDate,
	xtpChartDateTimeLongTime,
	xtpChartDateTimeShortTime,
	xtpChartDateTimeMonthAndDay,
	xtpChartDateTimeMonthAndYear,
	xtpChartDateTimeCustom,
};


//===========================================================================
// Summary:
//     This class helper used to format number to string
// Remarks:
// See Also:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartNumberFormat : public CXTPChartElement
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartNumberFormat object.
	// Parameters:
	//     pOwner     - The owner element of this element.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartNumberFormat(CXTPChartElement* pOwner);

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartNumberFormat object, handles cleanup
	//-------------------------------------------------------------------------
	virtual ~CXTPChartNumberFormat();

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Returns string representing format pattern
	//-------------------------------------------------------------------------
	CString GetPattern() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to set new pattern
	// Parameters:
	//     lpszPattern - new pattern to be set
	// Remarks
	//     You can specify C++ kind of format pattern: %lf, %g or {V} to use Active Locale.
	// Example:
	// pFormat->SetPattern(_T("Wasted {V} Dollars");
	//-------------------------------------------------------------------------
	void SetPattern(LPCTSTR lpszPattern);

	//-------------------------------------------------------------------------
	// Summary:
	//     Returns format category
	//-------------------------------------------------------------------------
	XTPChartNumberFormatCategory GetCategory() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to specify format category of double value (Number, Currency, Percent, Scientific etc)
	// Parameters:
	//     nCategory - New category to be set
	//-------------------------------------------------------------------------
	void SetCategory(XTPChartNumberFormatCategory nCategory);

	//-------------------------------------------------------------------------
	// Summary:
	//     Returns number of digits after decimal point
	// Remarks:
	//     This method returns -1 if locale specified number should be used
	//-------------------------------------------------------------------------
	int GetDecimalPlaces() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to set number of digits after decimal points
	// Parameters:
	//     nDecimalPlaces - Non negative number specified umber of digits after decimal points, or -1 to use locale value
	//-------------------------------------------------------------------------
	void SetDecimalPlaces(int nDecimalPlaces);

	//-------------------------------------------------------------------------
	// Summary:
	//     Returns TRUE if Thousand Separator (, or ' ') should be used
	//-------------------------------------------------------------------------
	BOOL GetUseThousandSeparator() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to use thousand separator for xtpChartNumber category of numbers.
	// Parameters:
	//     bThousandSeparator - TRUE to use thousand separator
	//-------------------------------------------------------------------------
	void SetUseThousandSeparator(BOOL bThousandSeparator);

	//-------------------------------------------------------------------------
	// Summary:
	//     Returns active locale to be used to format numbers
	//-------------------------------------------------------------------------
	LCID GetLocale() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to specify locale that will be used to format numbers
	// Parameters:
	//     nLocale - New locale to be set
	//-------------------------------------------------------------------------
	void SetLocale(LCID nLocale);

	//-------------------------------------------------------------------------
	// Summary:
	//     Returns used currency symbol for xtpChartCurrency category
	//-------------------------------------------------------------------------
	CString GetCurrencySymbol() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to set new currency symbol for xtpChartCurrency category
	// Parameters:
	//     lpszCurrencySymbol - new currency symbol. Can be empty string to use active locale.
	//-------------------------------------------------------------------------
	void SetCurrencySymbol(LPCTSTR lpszCurrencySymbol);


	XTPChartDateTimeFormatCategory GetDateTimeCategory() const;

	void SetDateTimeCategory(XTPChartDateTimeFormatCategory category);


	CString GetDateTimeFormat() const;
	void SetDateTimeFormat(LPCTSTR lpszDateTimeFormat);

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to format number to string
	// Parameters:
	//     dValue - Value to format
	//-------------------------------------------------------------------------
	virtual CString FormatNumber(double dValue) const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to format string
	// Parameters:
	//     lpszValue - Value to format
	//-------------------------------------------------------------------------
	virtual CString FormatString(LPCTSTR lpszValue) const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to format COleDateTime value to string
	// Parameters:
	//     dt - Value to format
	//-------------------------------------------------------------------------
	virtual CString FormatDate(COleDateTime& dt) const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to format value to string
	// Parameters:
	//     pSeriesPoint - Series Point
	//     dValue - Value to format
	//-------------------------------------------------------------------------
	virtual CString FormatPoint(CXTPChartSeriesPoint* pSeriesPoint, double dValue) const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to store/Load the properties of chart boundary
	//     object using the specified data object.
	// Parameters:
	//     pPX - Source or destination CXTPPropExchange data object reference.
	// Remarks:
	//     This member function is used to store or load property data to or
	//     from a storage.
	//-----------------------------------------------------------------------
	void DoPropExchange(CXTPPropExchange* pPX);

protected:
	CString DoubleToString(double dMark) const;
	void CreateNumberFormat();
	void CreateCurrencyFormat();
	void CreateDateTimeFormat();
	int GetLocaleLong(LCTYPE LCType) const;
	CString GetLocaleString(LCTYPE LCType, int nMaxLength);

protected:

	CString m_strPattern;                       // Format patter
	XTPChartNumberFormatCategory m_nCategory;   // Format category
	int m_nDecimalPlaces;                       // Number of digits after decimal places. -1 to use default
	BOOL m_bUseThousandSeparator;               // TRUE to separate thousands.
	LCID m_nLocale;                             // Current locale
	CString m_strCurrencySymbol;                // Currency symbol
	XTPChartDateTimeFormatCategory m_nDateTimeCategory;

	NUMBERFMT m_fmtNumber;
	CURRENCYFMT m_fmtCurrency;

	CString m_strDateTimeFormat;
	CString m_strActiveDateTimeFormat;

#ifdef _XTP_ACTIVEX
public:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartNumberFormat);

	afx_msg BSTR OleGetPattern();
	afx_msg void OleSetPattern(LPCTSTR lpszValue);
	afx_msg BSTR OleGetCurrencySymbol();
	afx_msg void OleSetCurrencySymbol(LPCTSTR lpszValue);
	afx_msg BSTR OleGetDateDateTimeFormat();
//}}AFX_CODEJOCK_PRIVATE
#endif

};


AFX_INLINE CString CXTPChartNumberFormat::GetPattern() const {
	return m_strPattern;
}
AFX_INLINE void CXTPChartNumberFormat::SetPattern(LPCTSTR lpszPattern) {
	m_strPattern = lpszPattern;
	OnChartChanged();
}
AFX_INLINE XTPChartNumberFormatCategory CXTPChartNumberFormat::GetCategory() const {
	return m_nCategory;
}
AFX_INLINE void CXTPChartNumberFormat::SetCategory(XTPChartNumberFormatCategory nCategory) {
	m_nCategory = nCategory;
	OnChartChanged();
}
AFX_INLINE int CXTPChartNumberFormat::GetDecimalPlaces() const {
	return m_nDecimalPlaces;
}
AFX_INLINE void CXTPChartNumberFormat::SetDecimalPlaces(int nDecimalPlaces) {
	m_nDecimalPlaces = nDecimalPlaces;
	CreateNumberFormat();
	CreateCurrencyFormat();
	OnChartChanged();
}
AFX_INLINE BOOL CXTPChartNumberFormat::GetUseThousandSeparator() const {
	return m_bUseThousandSeparator;
}
AFX_INLINE void CXTPChartNumberFormat::SetUseThousandSeparator(BOOL bThousandSeparator) {
	m_bUseThousandSeparator = bThousandSeparator;
	CreateNumberFormat();
	CreateCurrencyFormat();
	OnChartChanged();
}

AFX_INLINE LCID CXTPChartNumberFormat::GetLocale() const {
	return m_nLocale;
}
AFX_INLINE void CXTPChartNumberFormat::SetLocale(LCID nLocale) {
	m_nLocale = nLocale;
	CreateNumberFormat();
	CreateCurrencyFormat();
	CreateDateTimeFormat();
	OnChartChanged();
}
AFX_INLINE void CXTPChartNumberFormat::SetCurrencySymbol(LPCTSTR lpszCurrencySymbol) {
	m_strCurrencySymbol = lpszCurrencySymbol;
	CreateCurrencyFormat();
	OnChartChanged();
}
AFX_INLINE CString CXTPChartNumberFormat::GetCurrencySymbol() const {
	return m_strCurrencySymbol;
}
AFX_INLINE void CXTPChartNumberFormat::SetDateTimeCategory(XTPChartDateTimeFormatCategory category) {
	m_nDateTimeCategory = category;
	CreateDateTimeFormat();
	OnChartChanged();
}
AFX_INLINE XTPChartDateTimeFormatCategory CXTPChartNumberFormat::GetDateTimeCategory() const {
	return m_nDateTimeCategory;
}
AFX_INLINE CString CXTPChartNumberFormat::GetDateTimeFormat() const  {
	return m_strDateTimeFormat;
}
AFX_INLINE void CXTPChartNumberFormat::SetDateTimeFormat(LPCTSTR lpszDateTimeFormat) {
	m_strDateTimeFormat = lpszDateTimeFormat;
	CreateDateTimeFormat();
	OnChartChanged();
}


#endif //#if !defined(__XTPCHARTNUMBERFORMAT_H__)
