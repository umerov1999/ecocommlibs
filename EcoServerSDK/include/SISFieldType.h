#pragma once
#include <afxwin.h>  
#include <afxext.h>

#include "SISDefines.h"

class SISFieldType
{
public:
	UINT m_nFieldTypeID;
	CString m_strFieldTypeName;
	CArray<FieldType, FieldType&> m_arrFieldType;

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISFieldType()
	{
		m_nFieldTypeID = 0;
		m_strFieldTypeName = _T("");
		m_arrFieldType.RemoveAll();
	}

	/**
	@brief Creator
	@param c [in]Object of SISFieldType
	@return
	@remark Initialize member variable using value of input object
	*/
	SISFieldType(const SISFieldType& c)
	{
		*this = c;
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISFieldType& operator = (const SISFieldType &other)
	{
		m_nFieldTypeID = other.m_nFieldTypeID;
		m_strFieldTypeName = other.m_strFieldTypeName;
		m_arrFieldType.Copy(other.m_arrFieldType);

		return *this;
	}

	/**
	@brief Set field ID
	@param nFieldTypeID [in]Field type ID
	@return
	@remark
	*/
	void SetFieldTypeID(UINT nFieldTypeID)
	{
		m_nFieldTypeID = nFieldTypeID;
	}

	/**
	@brief Get field type ID
	@param
	@return Field type ID
	@remark
	*/
	UINT GetFieldTypeID()
	{
		return m_nFieldTypeID;
	}

	/**
	@brief Set field type name
	@param strFieldTypeName [in]Field type name
	@return
	@remark
	*/
	void SetFieldTypeName(CString strFieldTypeName)
	{
		m_strFieldTypeName = strFieldTypeName;
	}

	/**
	@brief Get field type name
	@param
	@return Field type name
	@remark
	*/
	CString GetFieldTypeName()
	{
		return m_strFieldTypeName;
	}

	/**
	@brief Set field type list
	@param arrFieldType [in]Field type list
	@return
	@remark
	*/
	void SetFieldType(CArray<FieldType, FieldType&>& arrFieldType)
	{
		m_arrFieldType.Copy(arrFieldType);
	}

	/**
	@brief Get field type list
	@param
	@return Field type list
	@remark
	*/
	void GetFieldType(CArray<FieldType, FieldType&>& arrFieldType)
	{
		arrFieldType.Copy(m_arrFieldType);
	}
};