#pragma once

#ifndef _SISUserVariableOfGroup_H
#define _SISUserVariableOfGroup_H

#include <afxwin.h>  
#include <afxext.h>

#include "SISUserVariable.h"

class SISUserVariableOfGroup : public SISUserVariable
{
public:
	CString m_strAssemLevel;
	CString m_strVariableGroup;

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISUserVariableOfGroup()
	{
		m_strAssemLevel = _T("");
		m_strVariableGroup = _T("");
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISUserVariableOfGroup& operator = (const SISUserVariableOfGroup &other)
	{
		SISUserVariable::operator=(other);
		m_strAssemLevel = other.m_strAssemLevel;
		m_strVariableGroup = other.m_strVariableGroup;

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
	@brief Set variable group
	@param strVariableGroup [in]Variable group
	@return
	@remark
	*/
	void SetVariableGroup(CString strVariableGroup)
	{
		m_strVariableGroup = strVariableGroup;
	}

	/**
	@brief Get variable group
	@param
	@return Variable group
	@remark
	*/
	CString GetVariableGroup()
	{
		return m_strVariableGroup;
	}
};

#endif