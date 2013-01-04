// XTPChartScaleTypeMap.cpp
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

#include "Common/XTPResourceManager.h"

#include "../../Types/XTPChartTypes.h"
#include "../../XTPChartDefines.h"
#include "../../XTPChartElement.h"
#include "../../XTPChartElementView.h"
#include "../../XTPChartSeries.h"
#include "../../XTPChartSeriesView.h"
#include "../../XTPChartSeriesPoint.h"

#include "../../Utils/XTPChartNumberFormat.h"
#include "XTPChartScaleTypeMap.h"
#include "XTPChartAxis.h"
#include "XTPChartAxisView.h"

CXTPChartScaleTypeMap::CXTPChartScaleTypeMap(CXTPChartAxis* pAxis)
{
	m_pOwner = pAxis;
}

CXTPChartScaleTypeMap::~CXTPChartScaleTypeMap()
{
}

CXTPChartScaleTypeMap* CXTPChartScaleTypeMap::Create(CXTPChartAxis* pAxis)
{
	if (pAxis->GetScaleType() == xtpChartScaleNumerical)
		return new CXTPChartNumericalScaleTypeMap(pAxis);

	if (pAxis->GetScaleType() == xtpChartScaleQualitative)
		return new CXTPChartQualitativeScaleTypeMap(pAxis);

	if (pAxis->GetScaleType() == xtpChartScaleDateTime)
		return new CXTPChartDateTimeScaleTypeMap(pAxis);

	return NULL;
}


void CXTPChartScaleTypeMap::UpdateSeries(CXTPChartAxisView* pAxisView)
{
	CXTPChartAxis* pAxis = GetAxis();

	CArray<CXTPChartSeriesView*, CXTPChartSeriesView*>& arrSeries = pAxisView->m_arrSeries;

	int i;

	for (i = 0; i < arrSeries.GetSize(); i++)
	{
		UpdateSeries(arrSeries.GetAt(i)->GetSeries());
	}

	if (!pAxis->IsValuesAxis())
	{
		for (i = 0; i < arrSeries.GetSize(); i++)
		{
			CXTPChartSeries* pSeries = arrSeries.GetAt(i)->GetSeries();

			for (int j = 0; j < pSeries->GetPoints()->GetCount(); j++)
			{
				UpdateSeriesPointArgument(pSeries->GetPoints()->GetAt(j));
			}
		}
	}
}

void CXTPChartScaleTypeMap::UpdateSeries(CXTPChartSeries* /*pSeries*/)
{

}

void CXTPChartScaleTypeMap::UpdateSeriesPointArgument(CXTPChartSeriesPoint* /*pPoint*/)
{

}


//////////////////////////////////////////////////////////////////////////
// CXTPChartNumericalMap


CXTPChartNumericalScaleTypeMap ::CXTPChartNumericalScaleTypeMap(CXTPChartAxis* pAxis)
	: CXTPChartScaleTypeMap(pAxis)
{
	m_dMinValue = 0;
	m_dMaxValue = 0;
}


CXTPChartString CXTPChartNumericalScaleTypeMap::InternalToValue(CXTPChartNumberFormat* pFormat, double dMark) const
{
	return pFormat->FormatNumber(dMark);
}

double CXTPChartNumericalScaleTypeMap::ValueToInternal(const CXTPChartString& strValue) const
{
	double dValue = atof(XTP_CT2CA(strValue));
	return dValue;
}

void CXTPChartNumericalScaleTypeMap::UpdateSeriesPointArgument(CXTPChartSeriesPoint* pPoint)
{
	pPoint->SetInternalArgument(pPoint->GetArgumentValue());
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartNumericalMap


CXTPChartDateTimeScaleTypeMap ::CXTPChartDateTimeScaleTypeMap(CXTPChartAxis* pAxis)
	: CXTPChartScaleTypeMap(pAxis)
{
}

double CXTPChartDateTimeScaleTypeMap::ValueToInternal(const CXTPChartString& strValue) const
{
	COleDateTime dt;

	if (dt.ParseDateTime(strValue))
	{
		return DateToInternal((DATE)dt);
	}

	return 0;
}

double CXTPChartDateTimeScaleTypeMap::DateToInternal(DATE dValue) const
{
	switch (((CXTPChartAxis*)m_pOwner)->GetDateTimeScaleUnit())
	{

	case xtpChartScaleHour:
		return dValue * 24;

	case xtpChartScaleMinute:
		return dValue * 24 * 60;

	case xtpChartScaleMonth:
		{
			COleDateTime dt(dValue);

			COleDateTime dtBase(dt.GetYear(), dt.GetMonth(), 1, 0, 0, 0);

			dValue = (DATE)dt.GetYear() * 12 + (dt.GetMonth() - 1) + ((DATE)dt - (DATE)dtBase) / 31;
		}
		return dValue;

	case xtpChartScaleYear:
		{
			COleDateTime dt(dValue);

			COleDateTime dtBase(dt.GetYear(), 1, 1, 0, 0, 0);

			dValue = (DATE)dt.GetYear() + ((DATE)dt - (DATE)dtBase) / 366.0;
		}
		break;
	}

	return dValue;
}

DATE CXTPChartDateTimeScaleTypeMap::DoubleToDate(double dValue) const
{
	switch (((CXTPChartAxis*)m_pOwner)->GetDateTimeScaleUnit())
	{
	case xtpChartScaleHour:
		return dValue / 24;

	case xtpChartScaleMinute:
		return dValue / 24 / 60;

	case xtpChartScaleMonth:
		{
			int nYearMonth = (int)dValue;
			int nYear = nYearMonth / 12;
			int nMonth = nYearMonth - nYear * 12 + 1;


			COleDateTime dtBase(nYear, nMonth, 1, 0, 0, 0);

			dValue = (DATE)dtBase + (dValue - nYearMonth) * 31;
		}
		return dValue;

	case xtpChartScaleYear:
		{
			int nYear = (int)dValue;
			COleDateTime dtBase(nYear, 1, 1, 0, 0, 0);

			dValue = (DATE)dtBase + (dValue - nYear) * 366.0;
		}
		break;
	}

	return dValue;
}


void CXTPChartDateTimeScaleTypeMap::UpdateSeriesPointArgument(CXTPChartSeriesPoint* pPoint)
{
	COleDateTime dt;

	DATE dValue = 0;

	if (!pPoint->GetArgument().IsEmpty() && dt.ParseDateTime(pPoint->GetArgument()))
	{
		dValue = (DATE)dt;
	}
	else
	{
		dValue = (DATE)pPoint->GetArgumentValue();
	}

	pPoint->SetInternalArgument(DateToInternal(dValue));
}


CXTPChartString CXTPChartDateTimeScaleTypeMap::InternalToValue(CXTPChartNumberFormat* pFormat, double dMark) const
{
	COleDateTime dt(DoubleToDate(dMark));

	return pFormat->FormatDate(dt);
}


//////////////////////////////////////////////////////////////////////////
// CXTPChartQualitativeScaleTypeMap

template<> AFX_INLINE UINT AFXAPI HashKey(const CXTPChartString& key)
{
	// default identity hash - works for most primitive values
	return HashKey((LPCTSTR)key);
}

//===========================================================================
// Summary:
//     This class abstracts a storage mechamism for strings which are used in
//     charts with qualitative scale types.
// Remarks:
//===========================================================================
class CXTPChartQualitativeScaleTypeMap::CStorage
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the zero based index of an arbitrary string.
	// Parameter:
	//     strValue - CXTPChartString denoting the value.
	// Returns:
	//     An integer denoting the index of the string.
	// Remarks:
	//-----------------------------------------------------------------------
	int IndexOf(const CXTPChartString& strValue) const
	{
		int nValue = m_arrList.IndexOf(strValue);
		return nValue;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to find an arbitrary string in the storage.
	// Parameter:
	//     strValue - A reference to the CXTPChartString denoting the value.
	// Returns:
	//     TRUE if it could find the string and FALSE if not.
	// Remarks:
	//-----------------------------------------------------------------------
	BOOL Find(const CXTPChartString& strValue) const
	{
		return IndexOf(strValue) != -1;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to add a collection of strings to the storage.
	// Parameter:
	//     strArray - A reference to the CXTPChartStrings denoting the strings
	//     collection.
	// Remarks:
	//-----------------------------------------------------------------------
	void AddRange(const CXTPChartStrings& strArray)
	{
		for (int i = 0; i < strArray.GetSize(); i++)
			m_arrList.Add(strArray[i]);

	}
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to add a string to the storage.
	// Parameter:
	//     str - A reference to the CXTPChartString denoting the string to be
	//           added to the storage.
	// Remarks:
	//-----------------------------------------------------------------------
	void Add(const CXTPChartString& str)
	{
		m_arrList.Add(str);

	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to insert a collection of strings to the storage,
	//     after a particular index.
	// Parameters:
	//     index    - The index after which the strings to be inserted.
	//     strArray - A reference to the CXTPChartStrings denoting the strings
	//     collection.
	// Remarks:
	//-----------------------------------------------------------------------
	void InsertRange(int index, const CXTPChartStrings& strArray)
	{
		for (int i = 0; i < strArray.GetSize(); i++)
			m_arrList.InsertAt(index + i, strArray[i]);

	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to insert a string to the storage,after a
	//     particular index.
	// Parameters:
	//     index    - The index after which the strings to be inserted.
	//     str      - A reference to the CXTPChartString denoting the string
	//     to be added.
	// Remarks:
	//-----------------------------------------------------------------------
	void Insert(int index, const CXTPChartString& str)
	{
		m_arrList.InsertAt(index, str);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get a string at a particular index.
	// Parameters:
	//     index    - The index from which the string to be extracted.
	// Returns:
	//     A CXTPChartString value.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartString GetAt(int nIndex)
	{
		if (nIndex >= 0 && nIndex < m_arrList.GetSize())
			return m_arrList[nIndex];

		return CXTPChartString();
	}

protected:
	CXTPChartStrings m_arrList;//The collection of strings.
};


CXTPChartQualitativeScaleTypeMap::CXTPChartQualitativeScaleTypeMap(CXTPChartAxis* pAxis)
	: CXTPChartScaleTypeMap(pAxis)
{

	m_pStorage = new CStorage();
}

CXTPChartQualitativeScaleTypeMap::~CXTPChartQualitativeScaleTypeMap()
{
	delete m_pStorage;
}


int CXTPChartStrings::IndexOf(const CXTPChartString& strValue) const
{
	for (int i = 0; i < GetSize(); i++)
	{
		if (GetAt(i) == strValue)
			return i;
	}
	return -1;
}


int CXTPChartQualitativeScaleTypeMap::FillNextsList(CXTPChartSeries* pSeries, int i, CXTPChartStrings& nexts, const CXTPChartString& argument_i)
{
	nexts.RemoveAll();

	for (int j = i + 1; j < pSeries->GetPoints()->GetCount(); j++)
	{
		CXTPChartString argument_j = pSeries->GetPoints()->GetAt(j)->GetArgument();
		if (argument_j == argument_i)
			continue;

		if (nexts.IndexOf(argument_j) >= 0)
			continue;

		if (m_pStorage->Find(argument_j))
			return m_pStorage->IndexOf(argument_j);

		nexts.Add(argument_j);
	}
	return -1;
}


void CXTPChartQualitativeScaleTypeMap::UpdateSeries(CXTPChartSeries* pSeries)
{
	int i = 0;
	CXTPChartStrings nexts;

	while (i < pSeries->GetPoints()->GetCount())
	{
		CXTPChartString argument_i = pSeries->GetPoints()->GetAt(i)->GetArgument();

		if (m_pStorage->Find(argument_i))
		{
			i++;
			continue;
		}

		int index = FillNextsList(pSeries, i, nexts, argument_i);
		if (index == -1)
		{
			m_pStorage->Add(argument_i);
			m_pStorage->AddRange(nexts);
		}
		else
		{
			m_pStorage->InsertRange(index, nexts);
			m_pStorage->Insert(index, argument_i);
		}
		i += (int)nexts.GetSize() + 1;
	}
}

void CXTPChartQualitativeScaleTypeMap::UpdateSeriesPointArgument(CXTPChartSeriesPoint* pPoint)
{
	pPoint->SetInternalArgument(ValueToInternal(pPoint->GetArgument()));
}

double CXTPChartQualitativeScaleTypeMap::ValueToInternal(const CXTPChartString& str) const
{
	return m_pStorage->IndexOf(str);
}

CXTPChartString CXTPChartQualitativeScaleTypeMap::InternalToValue(CXTPChartNumberFormat* pFormat, double dMark) const
{
	CXTPChartString str = m_pStorage->GetAt((int)dMark);

	return pFormat->FormatString(str);
}
