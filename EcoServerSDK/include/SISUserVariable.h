#pragma once

#ifndef _SISUserVariable_H
#define _SISUserVariable_H

#include <afxwin.h>  
#include <afxext.h>

#include "SISShipNoInfo.h"

class SISUserVariable : public SISShipNoInfo
{
public:
	CString m_strVariable;

	CString m_strTargetProperty;
	CString m_strVariableType;
	BOOL m_bEssential;
	CString m_strData;
	BOOL m_bDuplicate;
	CString m_strInitValue;
	UINT m_nViewOrder;
	BOOL m_bUsePjtID;
public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISUserVariable()
	{
		m_strVariable = _T("");

		m_strTargetProperty = _T("");
		m_strVariableType = _T("");
		m_bEssential = FALSE;
		m_strData = _T("");
		m_bDuplicate = FALSE;
		m_strInitValue = _T("");
		m_nViewOrder = 0;
		m_bUsePjtID = FALSE;
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISUserVariable& operator = (const SISUserVariable &other)
	{
		SISShipNoInfo::operator=(other);
		m_strVariable = other.m_strVariable;

		m_strTargetProperty = other.m_strTargetProperty;
		m_strVariableType = other.m_strVariableType;
		m_bEssential = other.m_bEssential;
		m_strData = other.m_strData;
		m_bDuplicate = other.m_bDuplicate;
		m_strInitValue = other.m_strInitValue;
		m_nViewOrder = other.m_nViewOrder;
		m_bUsePjtID = other.m_bUsePjtID;

		return *this;
	}

	/**
	@brief Set variable
	@param strVariable [in]Variable
	@return
	@remark
	*/
	void SetVariable(CString strVariable)
	{
		m_strVariable = strVariable;
	}

	/**
	@brief Get variable
	@param
	@return Variable
	@remark
	*/
	CString GetVariable()
	{
		return m_strVariable;
	}

	/**
	@brief Set property target
	@param strTargetProp [in]Property target
	@return
	@remark
	*/
	void SetTargetProperty(CString strTargetProp)
	{
		m_strTargetProperty = strTargetProp;
	}

	/**
	@brief Get property target
	@param
	@return Property target
	@remark
	*/
	CString GetTargetProperty()
	{
		return m_strTargetProperty;
	}

	/**
	@brief Set variable type
	@param strVarType [in]Variable type
	@return
	@remark
	*/
	void SetVariableType(CString strVarType)
	{
		m_strVariableType = strVarType;
	}

	/**
	@brief Get variable type
	@param
	@return Variable type
	@remark
	*/
	CString GetVariableType()
	{
		return m_strVariableType;
	}

	/**
	@brief Set essential presence or absence
	@param bEssential [in]Essential presence or absence
	@return
	@remark
	*/
	void SetEssential(BOOL bEssential)
	{
		m_bEssential = bEssential;
	}

	/**
	@brief Get essential presence or absence
	@param
	@return Essential presence or absence
	@remark
	*/
	BOOL GetEssential()
	{
		return m_bEssential;
	}

	/**
	@brief Set data
	@param strData [in]Data
	@return
	@remark
	*/
	void SetData(CString strData)
	{
		m_strData = strData;
	}

	/**
	@brief Get data
	@param
	@return Data
	@remark
	*/
	CString GetData()
	{
		return m_strData;
	}

	/**
	@brief Set duplicate presence or absence
	@param bDuplicate [in]Duplicate presence or absence
	@return
	@remark
	*/
	void SetDuplicate(BOOL bDuplicate)
	{
		m_bDuplicate = bDuplicate;
	}

	/**
	@brief Get duplicate presence or absence
	@param
	@return Duplicate presence or absence
	@remark
	*/
	BOOL GetDuplicate()
	{
		return m_bDuplicate;
	}

	/**
	@brief Set initialize value
	@param strInitVal [in]Initialize value
	@return
	@remark
	*/
	void SetInitValue(CString strInitVal)
	{
		m_strInitValue = strInitVal;
	}

	/**
	@brief Get initialize value
	@param
	@return Initialize value
	@remark
	*/
	CString GetInitValue()
	{
		return m_strInitValue;
	}

	/**
	@brief Set view order
	@param nViewOrder [in]View order
	@return
	@remark
	*/
	void SetViewOrder(UINT nViewOrder)
	{
		m_nViewOrder = nViewOrder;
	}

	/**
	@brief Get view order
	@param
	@return View order
	@remark
	*/
	UINT GetViewOrder()
	{
		return m_nViewOrder;
	}

	/**
	@brief Set use project ID presence or absence
	@param bUsePjtID [in]Use project ID presence or absence
	@return
	@remark
	*/
	void SetbUseProjectID(BOOL bUsePjtID)
	{
		m_bUsePjtID = bUsePjtID;
	}

	/**
	@brief Get use project ID presence or absence
	@param
	@return Use project ID presence or absence
	@remark
	*/
	BOOL GetbUseProjectID()
	{
		return m_bUsePjtID;
	}
};

#endif