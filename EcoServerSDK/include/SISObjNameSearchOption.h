#pragma once

#ifndef _SISObjNameSearchOption_H
#define _SISObjNameSearchOption_H

#include <afxwin.h>  
#include <afxext.h>

class SISObjNameSearchOption
{
public:
	CString m_strObjType;
	CString m_strObjName;
	CString m_strObjProductName;
	CString m_strObjSuperiorName;
	CString m_strObjModelName;
	CString m_strObjModuleName;

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISObjNameSearchOption()
	{
		m_strObjType = _T("");
		m_strObjName = _T("");
		m_strObjProductName = _T("");
		m_strObjSuperiorName = _T("");
		m_strObjModelName = _T("");
		m_strObjModuleName = _T("");
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISObjNameSearchOption& operator = (const SISObjNameSearchOption &other)
	{
		m_strObjType = other.m_strObjType;
		m_strObjName = other.m_strObjName;
		m_strObjProductName = other.m_strObjProductName;
		m_strObjSuperiorName = other.m_strObjSuperiorName;
		m_strObjModelName = other.m_strObjModelName;
		m_strObjModuleName = other.m_strObjModuleName;

		return *this;
	}

	/**
	@brief Set model object type search condition
	@param strObjType [in]Model object type search condition
	@return
	@remark Type of model object in tribon
	*/
	void SetObjType(CString strObjType)
	{
		m_strObjType = strObjType;
	}

	/**
	@brief Get model object type search condition
	@param
	@return Model object type search condition
	@remark Type of model object in tribon
	*/
	CString GetObjType()
	{
		return m_strObjType;
	}

	/**
	@brief Set model object name search condition
	@param strObjName [in]Model object name search condition
	@return
	@remark Name of model object in tribon
	*/
	void SetObjName(CString strObjName)
	{
		m_strObjName = strObjName;
	}

	/**
	@brief Get model object name search condition
	@param
	@return Model object name search condition
	@remark Name of model object in tribon
	*/
	CString GetObjName()
	{
		return m_strObjName;
	}

	/**
	@brief Set product name search condition
	@param strObjProductName [in]Product name search condition
	@return
	@remark Product name of model object in tribon
	*/
	void SetObjProductName(CString strObjProductName)
	{
		m_strObjProductName = strObjProductName;
	}

	/**
	@brief Get product name search condition
	@param
	@return Product name search condition
	@remark Product name of model object in tribon
	*/
	CString GetObjProductName()
	{
		return m_strObjProductName;
	}

	/**
	@brief Set superior name search condition
	@param strObjSuperiorName [in]Superior name search condition
	@return
	@remark Superior name of model object in tribon
	*/
	void SetObjSuperiorName(CString strObjSuperiorName)
	{
		m_strObjSuperiorName = strObjSuperiorName;
	}

	/**
	@brief Get superior name search condition
	@param
	@return Superior name search condition
	@remark Superior name of model object in tribon
	*/
	CString GetObjSuperiorName()
	{
		return m_strObjSuperiorName;
	}

	/**
	@brief Set model object name search condition
	@param strObjModelName [in]Model object name search condition
	@return
	@remark Name of model object in EcoMarine
	*/
	void SetObjModelName(CString strObjModelName)
	{
		m_strObjModelName = strObjModelName;
	}

	/**
	@brief Get model object name search condition
	@param
	@return Model object name search condition
	@remark Name of model object in EcoMarine
	*/
	CString GetObjModelName()
	{
		return m_strObjModelName;
	}

	/**
	@brief Set model module name search condition
	@param strObjModuleName [in]Module name search condition
	@return
	@remark Module name of model object in tribon
	*/
	void SetObjModuleName(CString strObjModuleName)
	{
		m_strObjModuleName = strObjModuleName;
	}

	/**
	@brief Get model module name search condition
	@param
	@return Module name search condition
	@remark Module name of model object in tribon
	*/
	CString GetObjModuleName()
	{
		return m_strObjModuleName;
	}


};

#endif