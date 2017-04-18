#pragma once
#include <afxwin.h>  
#include <afxext.h>

#include "SISAnalysisPoint.h"

class SISAnalysisMngPoint : public SISAnalysisPoint
{
public:
	CString m_strDimType;

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISAnalysisMngPoint() : SISAnalysisPoint()
	{
		m_strDimType = _T("");
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISAnalysisMngPoint& operator = (const SISAnalysisMngPoint &other)
	{
		SISAnalysisPoint::operator=(other);
		m_strDimType = other.m_strDimType;

		return *this;
	}

	/**
	@brief Set type of control point
	@param strDimType [in]Type of control point
	@return
	@remark
	*/
	void SetDimensionTypeName(CString strDimType)
	{
		m_strDimType = strDimType;
	}

	/**
	@brief Get type of control point
	@param
	@return Type of control point
	@remark
	*/
	CString GetDimensionTypeName()
	{
		return m_strDimType;
	}

};