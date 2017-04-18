#pragma once
#include <afxwin.h>  
#include <afxext.h>

#include "SISDefines.h"

class SISFieldInfo
{
public:
	CString m_strFieldName;
	CString m_strFieldType;
	BOOL m_bEssential;
	CArray<CString, CString&> m_arrData;
	CString m_strComment;

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISFieldInfo()
	{
		m_strFieldName = _T("");
		m_strFieldType = _T("");
		m_bEssential = FALSE;
		m_arrData.RemoveAll();
		m_strComment = _T("");
	}

	/**
	@brief Creator
	@param c [in]Object of SISFieldInfo
	@return
	@remark Initialize member variable using value of input object
	*/
	SISFieldInfo(const SISFieldInfo& c)
	{
		*this = c;
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISFieldInfo& operator = (const SISFieldInfo &other)
	{
		m_strFieldName = other.m_strFieldName;

		m_strFieldType = other.m_strFieldType;
		m_bEssential = other.m_bEssential;
		m_arrData.Copy(other.m_arrData);
		m_strComment = other.m_strComment;

		return *this;
	}

	/**
	@brief Set field name
	@param strFieldName [in]Field name
	@return
	@remark
	*/
	void SetFieldName(CString strFieldName)
	{
		m_strFieldName = strFieldName;
	}

	/**
	@brief Get field name
	@param
	@return Field name
	@remark
	*/
	CString GetFieldName()
	{
		return m_strFieldName;
	}

	/**
	@brief Set field type
	@param strFieldType [in]Field type name
	@return
	@remark
	*/
	void SetFieldType(CString strFieldType)
	{
		m_strFieldType = strFieldType;
	}

	/**
	@brief Get field type
	@param 
	@return Field type name
	@remark
	*/
	CString GetFieldType()
	{
		return m_strFieldType;
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
	@brief Set data(Use Only when selectbox, checklist type)
	@param arrData [in]Data
	@return
	@remark
	*/
	void SetData(CArray<CString, CString&>& arrData)
	{
		m_arrData.Copy(arrData);
	}

	/**
	@brief Get data(Use Only when selectbox, checklist type)
	@param arrData [out]Data
	@return
	@remark
	*/
	void GetData(CArray<CString, CString&>& arrData)
	{
		arrData.Copy(m_arrData);
	}

	/**
	@brief Set comment
	@param strComment [in]Comment
	@return
	@remark
	*/
	void SetComment(CString strComment)
	{
		m_strComment = strComment;
	}

	/**
	@brief Get comment
	@param
	@return Comment
	@remark
	*/
	CString GetComment()
	{
		return m_strComment;
	}
};