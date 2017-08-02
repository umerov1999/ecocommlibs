#pragma once

#ifndef _SISVariableGroup_H
#define _SISVariableGroup_H

#include <afxwin.h>  
#include <afxext.h>

class SISVariableGroup : public SISAssemLevel
{
public:
	CString m_strVariableGroup;
	CString m_strParentVariableGroup;
	CArray<CString, CString&> m_arrChildVariableGroup;

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISVariableGroup()
	{
		m_strVariableGroup = _T("");
		m_strParentVariableGroup = _T("");
		m_arrChildVariableGroup.RemoveAll();
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISVariableGroup& operator = (const SISVariableGroup &other)
	{
		SISAssemLevel::operator=(other);
		m_strVariableGroup = other.m_strVariableGroup;
		m_strParentVariableGroup = other.m_strParentVariableGroup;
		m_arrChildVariableGroup.Copy(other.m_arrChildVariableGroup);

		return *this;
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

	/**
	@brief Set parent variable group
	@param strParentVarGroup [in]Parent variable group
	@return
	@remark
	*/
	void SetParentVariableGroup(CString strParentVarGroup)
	{
		m_strParentVariableGroup = strParentVarGroup;
	}

	/**
	@brief Get parent variable group
	@param
	@return Parent variable group
	@remark
	*/
	CString GetParentVariableGroup()
	{
		return m_strParentVariableGroup;
	}

	/**
	@brief Set child variable groups
	@param arrChildVarGroup [in]Child variable groups
	@return Child variable groups
	@remark
	*/
	void SetChildVariableGroup(CArray<CString, CString&>& arrChildVarGroup)
	{
		m_arrChildVariableGroup.Copy(arrChildVarGroup);
	}

	/**
	@brief Get child variable groups
	@param arrChildVarGroup [out]Child variable groups
	@return
	@remark
	*/
	void GetChildVariableGroup(CArray<CString, CString&>& arrChildVarGroup)
	{
		arrChildVarGroup.Copy(m_arrChildVariableGroup);
	}
};

#endif