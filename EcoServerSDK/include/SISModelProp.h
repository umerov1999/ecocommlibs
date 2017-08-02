#pragma once

#ifndef _SISModelProp_H
#define _SISModelProp_H

#include <afxwin.h>  
#include <afxext.h>

class SISModelProp
{
public:
	CString m_strModelName;
	CMap<CString, LPCTSTR, CString, LPCTSTR> m_mapProp;

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISModelProp() : m_strModelName(_T("")) 
	{
		m_mapProp.RemoveAll();
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISModelProp& operator = (const SISModelProp &other)
	{
		m_strModelName = other.m_strModelName;
		
		CString strVarName = _T("");
		CString strData = _T("");
		m_mapProp.RemoveAll();
		POSITION mapPos = other.m_mapProp.GetStartPosition();
		while (mapPos)
		{
			other.m_mapProp.GetNextAssoc(mapPos, strVarName, strData);
			m_mapProp.SetAt(strVarName, strData);
		}

		return *this;
	}

	/**
	@brief Set model object name
	@param strName [in]Model object name
	@return
	@remark
	*/
	void SetModelName(CString strName)
	{
		m_strModelName = strName;
	}

	/**
	@brief Get model object name
	@param
	@return Model object name
	@remark
	*/
	CString GetModelName()
	{
		return m_strModelName;
	}

	/**
	@brief Set model object properties
	@param modelProp [in]Model object properties
	@return
	@remark
	*/
	void SetModelProperty(CMap<CString, LPCTSTR, CString, LPCTSTR>& modelProp)
	{
		m_mapProp.RemoveAll();

		CString strVarName = _T("");
		CString strData = _T("");
		POSITION mapPos = modelProp.GetStartPosition();
		while (mapPos)
		{
			modelProp.GetNextAssoc(mapPos, strVarName, strData);
			m_mapProp.SetAt(strVarName, strData);
		}
	}

	/**
	@brief Get model object properties
	@param modelProp [out]Model object properties
	@return
	@remark
	*/
	void GetModelProperty(CMap<CString, LPCTSTR, CString, LPCTSTR>& modelProp)
	{
		modelProp.RemoveAll();

		CString strVarName = _T("");
		CString strData = _T("");
		POSITION mapPos = m_mapProp.GetStartPosition();
		while (mapPos)
		{
			m_mapProp.GetNextAssoc(mapPos, strVarName, strData);
			modelProp.SetAt(strVarName, strData);
		}
	}
};

#endif