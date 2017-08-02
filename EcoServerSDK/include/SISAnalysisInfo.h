#pragma once

#ifndef _SISAnalysisInfo_H
#define _SISAnalysisInfo_H

#include <afxwin.h>  
#include <afxext.h>

#include "SISProductionInfo.h"
#include "SISAnalysisMngPoint.h"
#include "SISAnalysisMngLength.h"
#include "SISAnalysisMngAngle.h"
#include "SISAnalysisMngPerpendicular.h"

class SISAnalysisInfo : public SISProductionInfo
{
protected:
	CString m_strPjtName;
	CArray<SISAnalysisMngPoint, SISAnalysisMngPoint&> m_arrMngPoint;
	CArray<SISAnalysisMngLength, SISAnalysisMngLength&> m_arrMngLength;
	CArray<SISAnalysisMngAngle, SISAnalysisMngAngle&> m_arrMngAngle;
	CArray<SISAnalysisMngPerpendicular, SISAnalysisMngPerpendicular&> m_arrMngPerp;

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISAnalysisInfo() : SISProductionInfo()
	{
		m_strPjtName = _T("");
		m_arrMngPoint.RemoveAll();
		m_arrMngLength.RemoveAll();
		m_arrMngAngle.RemoveAll();
		m_arrMngPerp.RemoveAll();
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISAnalysisInfo& operator = (const SISAnalysisInfo &other)
	{
		SISProductionInfo::operator=(other);
		m_strPjtName = other.m_strPjtName;
		m_arrMngPoint.Copy(other.m_arrMngPoint);
		m_arrMngLength.Copy(other.m_arrMngLength);
		m_arrMngAngle.Copy(other.m_arrMngAngle);
		m_arrMngPerp.Copy(other.m_arrMngPerp);

		return *this;
	}

	/**
	@brief Set project name
	@param strPjtName [in]Project name
	@return
	@remark
	*/
	void SetProjectName(CString strPjtName)
	{
		m_strPjtName = strPjtName;
	}

	/**
	@brief Get project name
	@param
	@return Project name
	@remark
	*/
	CString GetProjectName()
	{
		return m_strPjtName;
	}

	/**
	@brief Set control point list
	@param arrAnalMngPoint [in]Control point list
	@return
	@remark
	*/
	void SetAnalysisMngPoint(CArray<SISAnalysisMngPoint, SISAnalysisMngPoint&>& arrAnalMngPoint)
	{
		m_arrMngPoint.Copy(arrAnalMngPoint);
	}

	/**
	@brief Get control point list
	@param arrAnalMngPoint [out]Control point list
	@return
	@remark
	*/
	void GetAnalysisMngPoint(CArray<SISAnalysisMngPoint, SISAnalysisMngPoint&>& arrAnalMngPoint)
	{
		arrAnalMngPoint.Copy(m_arrMngPoint);
	}

	/**
	@brief Set control length list
	@param arrAnalMngLength [in]Control length list
	@return
	@remark
	*/
	void SetAnalysisMngLength(CArray<SISAnalysisMngLength, SISAnalysisMngLength&>& arrAnalMngLength)
	{
		m_arrMngLength.Copy(arrAnalMngLength);
	}

	/**
	@brief Get control length list
	@param arrAnalMngLength [out]Control length list
	@return
	@remark
	*/
	void GetAnalysisMngLength(CArray<SISAnalysisMngLength, SISAnalysisMngLength&>& arrAnalMngLength)
	{
		arrAnalMngLength.Copy(m_arrMngLength);
	}

	/**
	@brief Set control angle list
	@param arrAnalMngAngle [in]Control angle list
	@return
	@remark
	*/
	void SetAnalysisMngAngle(CArray<SISAnalysisMngAngle, SISAnalysisMngAngle&>& arrAnalMngAngle)
	{
		m_arrMngAngle.Copy(arrAnalMngAngle);
	}

	/**
	@brief Get control angle list
	@param arrAnalMngAngle [out]Control angle list
	@return
	@remark
	*/
	void GetAnalysisMngAngle(CArray<SISAnalysisMngAngle, SISAnalysisMngAngle&>& arrAnalMngAngle)
	{
		arrAnalMngAngle.Copy(m_arrMngAngle);
	}

	/**
	@brief Set control perpendicular list
	@param arrAnalMngPerp [in]Control perpendicular list
	@return
	@remark
	*/
	void SetAnalysisMngPerpendicular(CArray<SISAnalysisMngPerpendicular, SISAnalysisMngPerpendicular&>& arrAnalMngPerp)
	{
		m_arrMngPerp.Copy(arrAnalMngPerp);
	}

	/**
	@brief Get control perpendicular list
	@param arrAnalMngPerp [out]Control perpendicular list
	@return
	@remark
	*/
	void GetAnalysisMngPerpendicular(CArray<SISAnalysisMngPerpendicular, SISAnalysisMngPerpendicular&>& arrAnalMngPerp)
	{
		arrAnalMngPerp.Copy(m_arrMngPerp);
	}
};

#endif