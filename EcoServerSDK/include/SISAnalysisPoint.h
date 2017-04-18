#pragma once
#include <afxwin.h>  
#include <afxext.h>

#include "SISObjectPoint.h"

class SISAnalysisPoint
{
protected:
	CString m_strName;
	SISObjectPoint m_dsnPoint;
	SISObjectPoint m_msrPoint;
	SISPoint m_deviation;
	BOOL m_bConnect;

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISAnalysisPoint()
	{
		m_strName = _T("");
		m_bConnect = FALSE;
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISAnalysisPoint& operator = (const SISAnalysisPoint &other)
	{
		m_strName = other.m_strName;
		m_dsnPoint = other.m_dsnPoint;
		m_msrPoint = other.m_msrPoint;
		m_deviation = other.m_deviation;
		m_bConnect = other.m_bConnect;

		return *this;
	}

	/**
	@brief Set analysis point name
	@param strName [in]Analysis point name
	@return
	@remark
	*/
	void SetName(CString strName)
	{
		m_strName = strName;
	}

	/**
	@brief Get analysis point name
	@param
	@return Analysis point name
	@remark
	*/
	CString GetName()
	{
		return m_strName;
	}

	/**
	@brief Set design point information
	@param dsnPoint [in]Design point information
	@return
	@remark
	*/
	void SetDsnPoint(SISObjectPoint dsnPoint)
	{
		m_dsnPoint = dsnPoint;
	}

	/**
	@brief Get design point information
	@param
	@return Design point information
	@remark
	*/
	SISObjectPoint GetDsnPoint()
	{
		return m_dsnPoint;
	}

	/**
	@brief Set measurement point information
	@param msrPoint [in]measurement point information
	@return
	@remark
	*/
	void SetMsrPoint(SISObjectPoint msrPoint)
	{
		m_msrPoint = msrPoint;
	}

	/**
	@brief Get measurement point information
	@param
	@return measurement point information
	@remark
	*/
	SISObjectPoint GetMsrPoint()
	{
		return m_msrPoint;
	}

	/**
	@brief Set deviation
	@param deviation [in]Deviation information
	@return
	@remark Deviation of the measurement point and design point
	*/
	void SetDeviation(SISPoint deviation)
	{
		m_deviation = deviation;
	}

	/**
	@brief Get deviation
	@param
	@return Deviation
	@remark Deviation of the measurement point and design point
	*/
	SISPoint GetDeviation()
	{
		return m_deviation;
	}

	/**
	@brief Sets a connection whether the measurement point and the design point.
	@param bConnect [in]Connection presence or absence
	@return
	@remark
	*/
	void SetIsConnected(BOOL bConnect)
	{
		m_bConnect = bConnect;
	}

	/**
	@brief Gets a connection whether the measurement point and the design point.
	@param
	@return Connection presence or absence
	@remark
	*/
	BOOL GetIsConnected()
	{
		return m_bConnect;
	}
};