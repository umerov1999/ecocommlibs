#pragma once
#include <afxwin.h>  
#include <afxext.h>

#include "SISShipNoInfo.h"

class SISAssemLevel : public SISShipNoInfo
{
public:
	CString m_strAssemLevel;
	UINT m_nOrder;

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISAssemLevel()
	{
		m_strAssemLevel = _T("");
		m_nOrder = 0;
	}

	/**
	@brief Creator
	@param c [in]Object of SISNAssemLevel
	@return
	@remark Initialize member variable using value of input object
	*/
	SISAssemLevel(const SISAssemLevel& c)
	{
		*this = c;
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISAssemLevel& operator = (const SISAssemLevel &other)
	{
		SISShipNoInfo::operator=(other);
		
		m_strAssemLevel = other.m_strAssemLevel;
		m_nOrder = other.m_nOrder;
	
		return *this;
	}

	/**
	@brief Set assembly level
	@param strAssemLevel [in]Assembly level
	@return
	@remark
	*/
	void SetAssemLevel(CString strAssemLevel)
	{
		m_strAssemLevel = strAssemLevel;
	}

	/**
	@brief Get assembly level
	@param
	@return Assembly level
	@remark
	*/
	CString GetAssemLevel()
	{
		return m_strAssemLevel;
	}

	/**
	@brief Set assembly level order
	@param nOrder [in]Assembly level order
	@return
	@remark
	*/
	void SetAssemLevelOrder(UINT nOrder)
	{
		m_nOrder = nOrder;
	}

	/**
	@brief Get assembly level order
	@param
	@return Assembly level order
	@remark
	*/
	UINT GetAssemLevelOrder()
	{
		return m_nOrder;
	}

};