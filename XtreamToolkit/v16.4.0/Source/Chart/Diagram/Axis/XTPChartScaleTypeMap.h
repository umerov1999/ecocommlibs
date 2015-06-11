// XTPChartScaleTypeMap.h
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

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPCHARTSCALETYPEMAP_H__)
#define __XTPCHARTSCALETYPEMAP_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartAxis;
class CXTPChartSeries;
class CXTPChartSeriesPoint;
class CXTPChartAxisView;
class CXTPChartNumberFormat;

//===========================================================================
// Summary:
//     This class abstracts a scale type map, act as an abstract base for
//     various specialized scale types classes.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartScaleTypeMap : public CXTPChartElement
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartScaleTypeMap object.
	// Parameters:
	//     pAxis - A pointer to chart axis object, to which this object is
	//             associated with.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartScaleTypeMap(CXTPChartAxis* pAxis);

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartScaleTypeMap object, handles cleanup.
	//-------------------------------------------------------------------------
	virtual ~CXTPChartScaleTypeMap();

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the axis object to which this object is
	//     associated with.
	// Returns:
	//     A pointer to chart axis object.
	//-------------------------------------------------------------------------
	CXTPChartAxis* GetAxis() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function when there is a change in values of the series or
	//     change in the scale.
	// Remarks:
	//     This is a pure virtual function.
	//-------------------------------------------------------------------------
	virtual void UpdateSeries(CXTPChartAxisView* pAxisView);
	virtual void UpdateSeries(CXTPChartSeries* pSeries);
	virtual void UpdateSeriesPointArgument(CXTPChartSeriesPoint* pPoint);

	//-------------------------------------------------------------------------
	// Summary:
	//     Use this function to convert a double value to string.This actually
	//     convert the internal representation of data to a more meaningful value.
	// Returns:
	//     A CXTPChartString object representing the string value.
	// Remarks:
	//     This is a pure virtual function.
	//-------------------------------------------------------------------------
	virtual CXTPChartString InternalToValue(CXTPChartNumberFormat* pFromat, double dValue) const = 0;

	//-------------------------------------------------------------------------
	// Summary:
	//     Use this function to convert a value string to an internal type.
	// Returns:
	//     A double representing the internal value.
	// Remarks:
	//     This is a pure virtual function.
	//-------------------------------------------------------------------------
	virtual double ValueToInternal(const CXTPChartString& strValue) const = 0;


public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to create a scale type map object.
	// Parameters:
	//     pAxis - A pointer to chart axis object.
	// Remarks:
	//     This is a static function.
	//-------------------------------------------------------------------------
	static CXTPChartScaleTypeMap* AFX_CDECL Create(CXTPChartAxis* pAxis);

protected:

};

//===========================================================================
// Summary:
//     This class is a kind of CXTPChartScaleTypeMap, which abstracts a numeric
//     scale type for an axis.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS   CXTPChartNumericalScaleTypeMap : public CXTPChartScaleTypeMap
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartNumericalScaleTypeMap object.
	// Parameters:
	//     pAxis - A pointer to chart axis object, to which this object is
	//             associated with.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartNumericalScaleTypeMap(CXTPChartAxis* pAxis);

public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Use this function to convert a double value to string.This actually
	//     convert the internal representation of data to a more meaningful value.
	// Returns:
	//     A CXTPChartString object representing the string value.
	// Remarks:
	//-------------------------------------------------------------------------
	virtual CXTPChartString InternalToValue(CXTPChartNumberFormat* pFromat, double dValue) const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Use this function to convert a value string to a an internal type.
	// Returns:
	//     A double representing the internal value.
	// Remarks:
	//-------------------------------------------------------------------------
	virtual double ValueToInternal(const CXTPChartString& strValue) const;

protected:
	void UpdateSeriesPointArgument(CXTPChartSeriesPoint* pPoint);

protected:
	double m_dMinValue;     //The minimum value.
	double m_dMaxValue;     //The maximum value.
};

class _XTP_EXT_CLASS CXTPChartTime
{
	enum
	{
		Seconds = 1000,
		Minutes = Seconds * 60,
		Hours   = Minutes * 60
	};

public:
	CXTPChartTime(double dTime);

	CXTPChartTime(int nHours, int nMinutes, int nSeconds, int nMilliseconds);

	int GetHours() const;

	int GetMinutes() const;

	int GetSeconds() const;

	int GetMilliseconds() const;

	double m_dTime;
};

//===========================================================================
// Summary:
//     This class is a kind of CXTPChartScaleTypeMap, which abstracts a date-time
//     scale type for an axis.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartDateTimeScaleTypeMap : public CXTPChartScaleTypeMap
{
public:
	CXTPChartDateTimeScaleTypeMap(CXTPChartAxis* pAxis);

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Use this function to convert a double value to string.This actually
	//     convert the internal representation of data to a more meaningful value.
	// Returns:
	//     A CXTPChartString object representing the string value.
	// Remarks:
	//-------------------------------------------------------------------------
	virtual CXTPChartString InternalToValue(CXTPChartNumberFormat* pFromat, double dValue) const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Use this function to convert a value string to to an internal type.
	// Returns:
	//     A double representing the internal value.
	// Remarks:
	//-------------------------------------------------------------------------
	virtual double ValueToInternal(const CXTPChartString& strValue) const;

protected:

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to update a point argument in a series point.
	// Parameters:
	//     pPoint - A pointer to the chart series point object.
	// Remarks:
	//-------------------------------------------------------------------------
	void UpdateSeriesPointArgument(CXTPChartSeriesPoint* pPoint);

private:
	double DateToInternal(DATE dValue) const;
	DATE DoubleToDate(double dValue) const;

};


//===========================================================================
// Summary:
//     This class is a kind of CXTPChartScaleTypeMap, which abstracts a time
//     scale type for an axis.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartTimeScaleTypeMap : public CXTPChartScaleTypeMap
{
public:
	CXTPChartTimeScaleTypeMap(CXTPChartAxis* pAxis);

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Use this function to convert a double value to string.This actually
	//     convert the internal representation of data to a more meaningful value.
	// Returns:
	//     A CXTPChartString object representing the string value.
	// Remarks:
	//-------------------------------------------------------------------------
	virtual CXTPChartString InternalToValue(CXTPChartNumberFormat* pFromat, double dValue) const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Use this function to convert a value string to to an internal type.
	// Returns:
	//     A double representing the internal value.
	// Remarks:
	//-------------------------------------------------------------------------
	virtual double ValueToInternal(const CXTPChartString& strValue) const;

protected:

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to update a point argument in a series point.
	// Parameters:
	//     pPoint - A pointer to the chart series point object.
	// Remarks:
	//-------------------------------------------------------------------------
	void UpdateSeriesPointArgument(CXTPChartSeriesPoint* pPoint);

private:
	double DateToInternal(DATE dValue) const;
	DATE DoubleToDate(double dValue) const;

};


//===========================================================================
// Summary:
//     This class is a collection class of strings, it is a kind of CArray, which
//     is specialized into array of strings.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartStrings : public CArray<CXTPChartString, const CXTPChartString&>
{
public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Use this function to get the zero based index of an arbitrary string
	//     in the collection.
	// Parameters:
	//     strValue - A reference to the sting object whose index is to be determined..
	// Remarks:
	//-------------------------------------------------------------------------
	int IndexOf(const CXTPChartString& strValue) const;
};

//===========================================================================
// Summary:
//     This class abstracts a scale type map, which represents qualitaive values.
//     This class is a kind of CXTPChartScaleTypeMap.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartQualitativeScaleTypeMap : public CXTPChartScaleTypeMap
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartQualitativeScaleTypeMap object.
	// Parameters:
	//     pAxis - A pointer to chart axis object, to which this object is
	//             associated with.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartQualitativeScaleTypeMap(CXTPChartAxis* pAxis);
	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartQualitativeScaleTypeMap object, handles cleanup.
	//-------------------------------------------------------------------------
	virtual ~CXTPChartQualitativeScaleTypeMap();

public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Use this function to convert a double value to string. This actually
	//     convert the internal representation of data to a more meaningful value.
	// Returns:
	//     A CXTPChartString object representing the string value.
	// Remarks:
	//-------------------------------------------------------------------------
	virtual CXTPChartString InternalToValue(CXTPChartNumberFormat* pFromat, double dValue) const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Use this function to convert a value string to an internal type.
	// Returns:
	//     A double representing the internal value.
	// Remarks:
	//-------------------------------------------------------------------------
	virtual double ValueToInternal(const CXTPChartString& strValue) const;

protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function when there is a change in values of the series or
	//     change in the scale.
	// pSeries - A pointer to chart series object.
	// Remarks:
	//-------------------------------------------------------------------------
	void UpdateSeries(CXTPChartSeries* pSeries);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to update a point argument in a series point.
	// Parameters:
	//     pPoint - A pointer to the chart series point object.
	// Remarks:
	//-------------------------------------------------------------------------
	void UpdateSeriesPointArgument(CXTPChartSeriesPoint* pPoint);

	int FillNextsList(CXTPChartSeries* pSeries, int i, CXTPChartStrings& nexts, const CXTPChartString& argument_i);

protected:
	double m_dMinValue;     //The minimum value.
	double m_dMaxValue;     //The maximum value.

protected:
	class CStorage;
	CStorage* m_pStorage;   //The storage.
};

AFX_INLINE CXTPChartAxis* CXTPChartScaleTypeMap::GetAxis() const {
	return (CXTPChartAxis*)m_pOwner;
}


#endif //#if !defined(__XTPCHARTSCALETYPEMAP_H__)
