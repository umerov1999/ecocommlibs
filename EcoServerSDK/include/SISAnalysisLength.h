#pragma once

#ifndef _SISAnalysisLength_H
#define _SISAnalysisLength_H

#include <afxwin.h>  
#include <afxext.h>

#include "SISLength.h"
#include "SISAnalysisPoint.h"

class SISAnalysisLength
{
protected:
	CString m_strName;
	SISAnalysisPoint m_startPoint;
	SISAnalysisPoint m_endPoint;
	SISLength m_dsnLength;
	SISLength m_msrLength;
	BOOL m_bConnect;

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISAnalysisLength()
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
	SISAnalysisLength& operator = (const SISAnalysisLength &other)
	{
		m_strName = other.m_strName;
		m_startPoint = other.m_startPoint;
		m_endPoint = other.m_endPoint;
		m_dsnLength = other.m_dsnLength;
		m_msrLength = other.m_msrLength;
		m_bConnect = other.m_bConnect;

		return *this;
	}

	/**
	@brief Set name of analysis length
	@param strName [in]Name of analysis length
	@return
	@remark
	*/
	void SetName(CString strName)
	{
		m_strName = strName;
	}

	/**
	@brief Get name of analysis length
	@param
	@return Name of analysis length
	@remark
	*/
	CString GetName()
	{
		return m_strName;
	}

	/**
	@brief Set start point of analysis length
	@param startPoint [in]Start point of analysis length
	@return
	@remark
	*/
	void SetStartPoint(SISAnalysisPoint startPoint)
	{
		m_startPoint = startPoint;
	}

	/**
	@brief Get start point of analysis length
	@param
	@return Start point of analysis length
	@remark
	*/
	SISAnalysisPoint GetStartPoint()
	{
		return m_startPoint;
	}

	/**
	@brief Set end point of analysis length
	@param endPoint [in]End point of analysis length
	@return
	@remark
	*/
	void SetEndPoint(SISAnalysisPoint endPoint)
	{
		m_endPoint = endPoint;
	}

	/**
	@brief Get end point of analysis length
	@param
	@return End point of analysis length
	@remark
	*/
	SISAnalysisPoint GetEndPoint()
	{
		return m_endPoint;
	}

	/**
	@brief Set design length
	@param dsnLength [in]Design length
	@return
	@remark
	*/
	void SetDesignLength(SISLength dsnLength)
	{
		m_dsnLength = dsnLength;
	}

	/**
	@brief Get design length
	@param
	@return Design length
	@remark
	*/
	SISLength GetDesignLength()
	{
		return m_dsnLength;
	}

	/**
	@brief Set measurement length
	@param msrLength [in]Measurement length
	@return
	@remark
	*/
	void SetMeasureLength(SISLength msrLength)
	{
		m_msrLength = msrLength;
	}

	/**
	@brief Get measurement length
	@param
	@return Measurement length
	@remark
	*/
	SISLength GetMeasureLength()
	{
		return m_msrLength;
	}

	/**
	@brief Sets a connection whether the measurement length and the design length
	@param bConnect [in]Connection presence or absence
	@return
	@remark
	*/
	void SetIsConnected(BOOL bConnect)
	{
		m_bConnect = bConnect;
	}

	/**
	@brief Gets a connection whether the measurement length and the design length
	@param
	@return Connection presence or absence
	@remark
	*/
	BOOL GetIsConnected()
	{
		return m_bConnect;
	}
};

#endif