#pragma once

#ifndef _SISAnalysisMngPerpendicular_H
#define _SISAnalysisMngPerpendicular_H

#include <afxwin.h>  
#include <afxext.h>

#include "SISAnalysisPerpendicular.h"

class SISAnalysisMngPerpendicular : public SISAnalysisPerpendicular
{
protected:
	CString m_strDimType;
	SISPerpendicular m_devPerp;

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISAnalysisMngPerpendicular() : SISAnalysisPerpendicular()
	{
		m_strName = _T("");
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISAnalysisMngPerpendicular& operator = (const SISAnalysisMngPerpendicular &other)
	{
		SISAnalysisPerpendicular::operator=(other);

		m_strDimType = other.m_strDimType;
		m_devPerp = other.m_devPerp;

		return *this;
	}

	/**
	@brief Set type of control perpendicular
	@param strDimType [in]Type of control perpendicular
	@return
	@remark
	*/
	void SetDimensionTypeName(CString strDimType)
	{
		m_strDimType = strDimType;
	}

	/**
	@brief Get type of control perpendicular
	@param
	@return Type of control perpendicular
	@remark
	*/
	CString GetDimensionTypeName()
	{
		return m_strDimType;
	}

	/**
	@brief Set deviation of control perpendicular
	@param dblDevAngle [in]Deviation of control perpendicular
	@return
	@remark Deviation of the measurement perpendicular and the design perpendicular
	*/
	void SetDevPerpendicular(SISPerpendicular devLength)
	{
		m_devPerp = devLength;
	}

	/**
	@brief Get deviation of control perpendicular
	@param
	@return Deviation of control perpendicular
	@remark Deviation of the measurement perpendicular and the design perpendicular
	*/
	SISPerpendicular GetDevPerpendicular()
	{
		return m_devPerp;
	}
};

#endif