#pragma once

#ifndef _SISAnalysisPerpendicular_H
#define _SISAnalysisPerpendicular_H

#include <afxwin.h>  
#include <afxext.h>

#include "SISPerpendicular.h"
#include "SISAnalysisPoint.h"

class SISAnalysisPerpendicular
{
protected:
	CString m_strName;
	SISAnalysisPoint m_startPoint;
	SISAnalysisPoint m_endPoint;
	SISPerpendicular m_dsnPerp;
	SISPerpendicular m_msrPerp;
	BOOL m_bConnect;

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISAnalysisPerpendicular()
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
	SISAnalysisPerpendicular& operator = (const SISAnalysisPerpendicular &other)
	{
		m_strName = other.m_strName;
		m_startPoint = other.m_startPoint;
		m_endPoint = other.m_endPoint;
		m_dsnPerp = other.m_dsnPerp;
		m_msrPerp = other.m_msrPerp;
		m_bConnect = other.m_bConnect;

		return *this;
	}

	/**
	@brief Set name of analysis perpendicular
	@param strName [in]Name of analysis perpendicular
	@return
	@remark
	*/
	void SetName(CString strName)
	{
		m_strName = strName;
	}

	/**
	@brief Get name of analysis perpendicular
	@param
	@return Name of analysis perpendicular
	@remark
	*/
	CString GetName()
	{
		return m_strName;
	}

	/**
	@brief Set start point of analysis perpendicular
	@param startPoint [in]Start point of analysis perpendicular
	@return
	@remark
	*/
	void SetStartPoint(SISAnalysisPoint startPoint)
	{
		m_startPoint = startPoint;
	}

	/**
	@brief Get start point of analysis perpendicular
	@param
	@return Start point of analysis perpendicular
	@remark
	*/
	SISAnalysisPoint GetStartPoint()
	{
		return m_startPoint;
	}

	/**
	@brief Set end point of analysis perpendicular
	@param endPoint [in]End point of analysis perpendicular
	@return
	@remark
	*/
	void SetEndPoint(SISAnalysisPoint endPoint)
	{
		m_endPoint = endPoint;
	}

	/**
	@brief Get end point of analysis perpendicular
	@param
	@return End point of analysis perpendicular
	@remark
	*/
	SISAnalysisPoint GetEndPoint()
	{
		return m_endPoint;
	}

	/**
	@brief Set design perpendicular
	@param dsnPerp [in]Design perpendicular
	@return
	@remark
	*/
	void SetDesignPerpendicular(SISPerpendicular dsnPerp)
	{
		m_dsnPerp = dsnPerp;
	}

	/**
	@brief Get design perpendicular
	@param
	@return Design perpendicular
	@remark
	*/
	SISPerpendicular GetDesignPerpendicular()
	{
		return m_dsnPerp;
	}

	/**
	@brief Set measurement perpendicular
	@param msrPerp [in]Measurement perpendicular
	@return
	@remark
	*/
	void SetMeasurePerpendicular(SISPerpendicular msrPerp)
	{
		m_msrPerp = msrPerp;
	}

	/**
	@brief Get measurement perpendicular
	@param
	@return Measurement perpendicular
	@remark
	*/
	SISPerpendicular GetMeasurePerpendicular()
	{
		return m_msrPerp;
	}

	/**
	@brief Sets a connection whether the measurement perpendicular and the design perpendicular
	@param bConnect [in]Connection presence or absence
	@return
	@remark
	*/
	void SetIsConnected(BOOL bConnect)
	{
		m_bConnect = bConnect;
	}

	/**
	@brief Gets a connection whether the measurement perpendicular and the design perpendicular
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