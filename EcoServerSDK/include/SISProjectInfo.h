#pragma once

#ifndef _SISProjectInfo_H
#define _SISProjectInfo_H

#include <afxwin.h>  
#include <afxext.h>

#include "SISShipNoInfo.h"
#include "SISAttachedFile.h"


class SISProjectInfo : public SISShipNoInfo
{
public:
	UINT m_nPjtID;
	CString m_strAssemLevel;
	CString m_strAssemBlock;
	CString m_strVarGroup;
	CString m_strPjtType;
	CString m_strPjtName;
	CString m_strUserID;
	COleDateTime m_dtUpdateDate;
	CArray<SISAttachedFile, SISAttachedFile&> m_arrAttachedFile;

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISProjectInfo() : SISShipNoInfo()
	{
		m_nPjtID = 0;
		m_strAssemLevel = _T("");
		m_strAssemBlock = _T("");
		m_strVarGroup = _T("");
		m_strPjtType = _T("");
		m_strPjtName = _T("");
		m_strUserID = _T("");
		m_dtUpdateDate.m_status = m_dtUpdateDate.invalid;
		m_arrAttachedFile.RemoveAll();
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISProjectInfo& operator = (const SISProjectInfo &other)
	{
		SISShipNoInfo::operator=(other);

		m_nPjtID = other.m_nPjtID;
		m_strAssemLevel = other.m_strAssemLevel;
		m_strAssemBlock = other.m_strAssemBlock;
		m_strVarGroup = other.m_strVarGroup;
		m_strPjtType = other.m_strPjtType;
		m_strPjtName = other.m_strPjtName;
		m_strUserID = other.m_strUserID;
		m_dtUpdateDate = other.m_dtUpdateDate;
		m_arrAttachedFile.Copy(other.m_arrAttachedFile);

		return *this;
	}

	/**
	@brief Set project ID
	@param nPjtID [in]Project ID
	@return
	@remark
	*/
	void SetProjectID(UINT nPjtID)
	{
		m_nPjtID = nPjtID;
	}

	/**
	@brief Get project ID
	@param
	@return Project ID
	@remark
	*/
	UINT GetProjectID()
	{
		return m_nPjtID;
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

	/**
	@brief Set project type
	@param strPjtType [in]Project type
	@return
	@remark
	*/
	void SetProjectType(CString strPjtType)
	{
		m_strPjtType = strPjtType;
	}

	/**
	@brief Get project type
	@param
	@return Project type
	@remark
	*/
	CString GetProjectType()
	{
		return m_strPjtType;
	}

	/**
	@brief Set project name
	@param strPjtName [in]Project name
	@return
	@remark
	*/
	void SetProjectName(CString strPjtName)
	{
		m_strPjtName = strPjtName;
	}

	/**
	@brief Get project name ¹ÝÈ¯
	@param
	@return Project name
	@remark
	*/
	CString GetProjectName()
	{
		return m_strPjtName;
	}

	/**
	@brief Set user ID
	@param strUserID [in]User ID
	@return
	@remark
	*/
	void SetUserID(CString strUserID)
	{
		m_strUserID = strUserID;
	}

	/**
	@brief Get user ID
	@param
	@return User ID
	@remark
	*/
	CString GetUserID()
	{
		return m_strUserID;
	}

	/**
	@brief Set update date
	@param dtUpdateDate [in]Update date
	@return
	@remark
	*/
	void SetUpdateDate(COleDateTime dtUpdateDate)
	{
		m_dtUpdateDate = dtUpdateDate;
	}

	/**
	@brief Get update date
	@param
	@return Update date
	@remark
	*/
	COleDateTime GetUpdateDate()
	{
		return m_dtUpdateDate;
	}

	/**
	@brief Set attached file list
	@param arrAttachedFile [in]Attached file list
	@return
	@remark
	*/
	void SetAttachedFile(CArray<SISAttachedFile, SISAttachedFile&>& arrAttachedFile)
	{
		m_arrAttachedFile.Copy(arrAttachedFile);
	}

	/**
	@brief Get attached file list
	@param arrAttachedFile [out]Attached file list
	@return
	@remark
	*/
	void GetAttachedFile(CArray<SISAttachedFile, SISAttachedFile&>& arrAttachedFile)
	{
		arrAttachedFile.Copy(m_arrAttachedFile);
	}
};

#endif