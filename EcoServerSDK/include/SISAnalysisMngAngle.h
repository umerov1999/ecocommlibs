#pragma once
#include <afxwin.h>  
#include <afxext.h>

#include "SISAnalysisAngle.h"

class SISAnalysisMngAngle : public SISAnalysisAngle
{
public:
	CString m_strDimType;
	double m_dblDevAngle;

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISAnalysisMngAngle() : SISAnalysisAngle()
	{
		m_strDimType = _T("");
		m_dblDevAngle = 0.0;
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISAnalysisMngAngle& operator = (const SISAnalysisMngAngle &other)
	{
		SISAnalysisAngle::operator=(other);

		m_strDimType = other.m_strDimType;
		m_dblDevAngle = other.m_dblDevAngle;

		return *this;
	}

	/**
	@brief Set type of control angle
	@param strDimType [in]Type of control angle
	@return
	@remark
	*/
	void SetDimensionTypeName(CString strDimType)
	{
		m_strDimType = strDimType;
	}

	/**
	@brief Get type of control angle
	@param
	@return Type of control angle
	@remark
	*/
	CString GetDimensionTypeName()
	{
		return m_strDimType;
	}

	/**
	@brief Set deviation of control angle
	@param dblDevAngle [in]Deviation of control angle
	@return
	@remark Deviation of the measurement angle and the design angle
	*/
	void SetDevAngle(double dblDevAngle)
	{
		m_dblDevAngle = dblDevAngle;
	}

	/**
	@brief Get deviation of control angle
	@param
	@return Deviation of control angle
	@remark Deviation of the measurement angle and the design angle
	*/
	double GetDevAngle()
	{
		return m_dblDevAngle;
	}
};