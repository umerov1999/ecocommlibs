#pragma once
#include <afxwin.h>  
#include <afxext.h>

#include "SISAnalysisPoint.h"

class SISAnalysisAngle
{
protected:
	CString m_strName;
	SISAnalysisPoint m_originPoint;
	SISAnalysisPoint m_startPoint;
	SISAnalysisPoint m_endPoint;
	double m_dblDsnAngle;
	double m_dblMsrAngle;
	BOOL m_bConnect;

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISAnalysisAngle()
	{
		m_strName = _T("");
		m_bConnect = FALSE;
		m_dblDsnAngle = 0.0;
		m_dblMsrAngle = 0.0;
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISAnalysisAngle& operator = (const SISAnalysisAngle &other)
	{
		m_strName = other.m_strName;
		m_originPoint = other.m_originPoint;
		m_startPoint = other.m_startPoint;
		m_endPoint = other.m_endPoint;
		m_dblDsnAngle = other.m_dblDsnAngle;
		m_dblMsrAngle = other.m_dblMsrAngle;
		m_bConnect = other.m_bConnect;

		return *this;
	}

	/**
	@brief Set name of analysis angle
	@param strName [in]Name of analysis angle
	@return
	@remark
	*/
	void SetName(CString strName)
	{
		m_strName = strName;
	}

	/**
	@brief Get name of analysis angle
	@param
	@return Name of analysis angle
	@remark
	*/
	CString GetName()
	{
		return m_strName;
	}

	/**
	@brief Set center point of analysis angle
	@param centerPoint [in]Center point of analysis angle
	@return
	@remark
	*/
	void SetCenterPoint(SISAnalysisPoint centerPoint)
	{
		m_originPoint = centerPoint;
	}

	/**
	@brief Get center point of analysis angle
	@param
	@return Center point of analysis angle
	@remark
	*/
	SISAnalysisPoint GetCenterPoint()
	{
		return m_originPoint;
	}

	/**
	@brief Set start point of analysis angle
	@param startPoint [in]Start point of analysis angle
	@return
	@remark
	*/
	void SetStartPoint(SISAnalysisPoint startPoint)
	{
		m_startPoint = startPoint;
	}

	/**
	@brief Get start point of analysis angle
	@param
	@return Start point of analysis angle
	@remark
	*/
	SISAnalysisPoint GetStartPoint()
	{
		return m_startPoint;
	}

	/**
	@brief Set end point of analysis angle
	@param endPoint [in]End point of analysis angle
	@return
	@remark
	*/
	void SetEndPoint(SISAnalysisPoint endPoint)
	{
		m_endPoint = endPoint;
	}

	/**
	@brief Get end point of analysis angle
	@param
	@return End point of analysis angle
	@remark
	*/
	SISAnalysisPoint GetEndPoint()
	{
		return m_endPoint;
	}

	/**
	@brief Set design angle
	@param dsnAngle [in]Design angle
	@return
	@remark
	*/
	void SetDesignAngle(double dsnAngle)
	{
		m_dblDsnAngle = dsnAngle;
	}

	/**
	@brief Get design angle
	@param
	@return Design angle
	@remark
	*/
	double GetDesignAngle()
	{
		return m_dblDsnAngle;
	}

	/**
	@brief Set measurement angle
	@param msrAngle [in]Measurement angle
	@return
	@remark
	*/
	void SetMeasureAngle(double msrAngle)
	{
		m_dblMsrAngle = msrAngle;
	}

	/**
	@brief Get measurement angle
	@param
	@return Measurement angle
	@remark
	*/
	double GetMeasureAngle()
	{
		return m_dblMsrAngle;
	}

	/**
	@brief Sets a connection whether the measurement angle and the design angle
	@param bConnect [in]Connection presence or absence
	@return
	@remark
	*/
	void SetIsConnected(BOOL bConnect)
	{
		m_bConnect = bConnect;
	}

	/**
	@brief Gets a connection whether the measurement angle and the design angle
	@param
	@return Connection presence or absence
	@remark
	*/
	BOOL GetIsConnected()
	{
		return m_bConnect;
	}
};