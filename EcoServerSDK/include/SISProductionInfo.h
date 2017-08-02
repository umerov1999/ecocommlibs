#pragma once

#ifndef _SISProductionInfo_H
#define _SISProductionInfo_H

#include <afxwin.h>  
#include <afxext.h>

#include "SISShipNoInfo.h"

class SISProductionInfo : public SISShipNoInfo
{
public:
	CString m_strAssemLevel;
	CString m_strAssemBlock;
	CString m_strVarGroup;

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISProductionInfo() : SISShipNoInfo()
	{
		m_strAssemLevel = _T("");
		m_strAssemBlock = _T("");
		m_strVarGroup = _T("");
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISProductionInfo& operator = (const SISProductionInfo &other)
	{
		SISShipNoInfo::operator=(other);

		m_strAssemLevel = other.m_strAssemLevel;
		m_strAssemBlock = other.m_strAssemBlock;
		m_strVarGroup = other.m_strVarGroup;
	
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
	@brief Set assembly block
	@param strAssemBlock [in]Assembly block
	@return
	@remark
	*/
	void SetAssemBlock(CString strAssemBlock)
	{
		m_strAssemBlock = strAssemBlock;
	}

	/**
	@brief Get assembly block
	@param
	@return Assembly block
	@remark
	*/
	CString GetAssemBlock()
	{
		return m_strAssemBlock;
	}

	/**
	@brief Set variable group
	@param strVarGroup [in]Variable group
	@return
	@remark
	*/
	void SetVariableGroup(CString strVarGroup)
	{
		m_strVarGroup = strVarGroup;
	}

	/**
	@brief Get variable group
	@param
	@return Variable group
	@remark
	*/
	CString GetVariableGroup()
	{
		return m_strVarGroup;
	}
};

#endif