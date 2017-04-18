#pragma once
#include <afxwin.h>  
#include <afxext.h>

#include "SISAnalysisLength.h"

class SISAnalysisMngLength : public SISAnalysisLength
{
public:
	CString m_strDimType;
	SISLength m_devLength;
	

public:

	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISAnalysisMngLength() : SISAnalysisLength()
	{
		m_strDimType = _T("");
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISAnalysisMngLength& operator = (const SISAnalysisMngLength &other)
	{
		SISAnalysisLength::operator=(other);

		m_strDimType = other.m_strDimType;
		m_devLength = other.m_devLength;

		return *this;
	}

	/**
	@brief Ggt type of control length
	@param strDimType [in]Type of control length
	@return
	@remark
	*/
	void SetDimensionTypeName(CString strDimType)
	{
		m_strDimType = strDimType;
	}

	/**
	@brief Get type of control length
	@param
	@return Type of control length
	@remark
	*/
	CString GetDimensionTypeName()
	{
		return m_strDimType;
	}

	/**
	@brief Set deviation of control length
	@param devLength [in]Deviation of control length
	@return
	@remark Deviation of the measurement length and the design length
	*/
	void SetDevLength(SISLength devLength)
	{
		m_devLength = devLength;
	}

	/**
	@brief Get deviation of control length
	@param
	@return Deviation of control length
	@remark Deviation of the measurement length and the design length
	*/
	SISLength GetDevLength()
	{
		return m_devLength;
	}

};