#pragma once
#include <afxwin.h>  
#include <afxext.h>

#include "SISPoint.h"

class SISObjectPoint : public SISPoint
{
protected:
	CString m_strObjName;

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISObjectPoint()
	{
		m_strObjName = _T("");
	}

	/**
	@brief Creator
	@param c [in]Object of SISNObjectPoint
	@return
	@remark Initialize member variable using value of input object
	*/
	SISObjectPoint(SISObjectPoint& c)
	{
		*this = c;
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISObjectPoint& operator = (const SISObjectPoint &other)
	{
		SISPoint::operator=(other);
		m_strObjName = other.m_strObjName;

		return *this;
	}

	/**
	@brief Set point name
	@param strName [in]Point name
	@return
	@remark
	*/
	void SetObjectName(CString strName)
	{
		m_strObjName = strName;
	}

	/**
	@brief Get point name
	@param
	@return Point name
	@remark
	*/
	CString GetObjectName()
	{
		return m_strObjName;
	}
};