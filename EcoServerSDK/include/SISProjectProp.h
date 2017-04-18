#pragma once
#include <afxwin.h>  
#include <afxext.h>

#include "SISShipNoInfo.h"
#include "SISAttachedFile.h"

#define SISProjectProp SISProject

class SISProjectProp : public SISShipNoInfo
{
public:
	CString m_strAssemLevel;
	CString m_strAssemBlock;
	CString m_strVarGroup;
	UINT m_nPjtID;
	CString m_strPjtName;
	CArray<SISAttachedFile, SISAttachedFile&> m_arrAttachedFile;
	CMap<CString, LPCTSTR, CString, LPCTSTR> m_mapProp;
public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISProjectProp() : SISShipNoInfo()
	{
		m_strAssemLevel = _T("");
		m_strAssemBlock = _T("");
		m_strVarGroup = _T("");
		m_nPjtID = 0;
		m_strPjtName = _T("");
		m_arrAttachedFile.RemoveAll();
		m_mapProp.RemoveAll();
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISProjectProp& operator = (const SISProjectProp &other)
	{
		SISShipNoInfo::operator=(other);
		
		m_strAssemLevel = other.m_strAssemLevel;
		m_strAssemBlock = other.m_strAssemBlock;
		m_strVarGroup = other.m_strVarGroup;
		m_nPjtID = other.m_nPjtID;
		m_strPjtName = other.m_strPjtName;
		m_arrAttachedFile.Copy(other.m_arrAttachedFile);
		
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
	@brief Set project name
	@param nPjtID [in]Project name
	@return
	@remark
	*/
	void SetProjectName(CString strPjtName)
	{
		m_strPjtName = strPjtName;
	}

	/**
	@brief Get project name
	@param
	@return Project name
	@remark
	*/
	CString GetProjectName()
	{
		return m_strPjtName;
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
	@brief Get Attached file list
	@param arrAttachedFile [out]Attached file list
	@return
	@remark
	*/
	void GetAttachedFile(CArray<SISAttachedFile, SISAttachedFile&>& arrAttachedFile)
	{
		arrAttachedFile.Copy(m_arrAttachedFile);
	}

	/**
	@brief Set project property list
	@param pjtProp [in]Project property list
	@return
	@remark
	*/
	void SetProjectProp(CMap<CString, LPCTSTR, CString, LPCTSTR>& pjtProp)
	{
		m_mapProp.RemoveAll();

		CString strVarName = _T("");
		CString strData = _T("");
		POSITION mapPos = pjtProp.GetStartPosition();
		while (mapPos)
		{
			pjtProp.GetNextAssoc(mapPos, strVarName, strData);
			m_mapProp.SetAt(strVarName, strData);
		}
	}

	/**
	@brief Get project property list
	@param arrAttachedFile [out]Project property list
	@return
	@remark
	*/
	void GetProjectProp(CMap<CString, LPCTSTR, CString, LPCTSTR>& pjtProp)
	{
		pjtProp.RemoveAll();

		CString strVarName = _T("");
		CString strData = _T("");
		POSITION mapPos = m_mapProp.GetStartPosition();
		while (mapPos)
		{
			m_mapProp.GetNextAssoc(mapPos, strVarName, strData);
			pjtProp.SetAt(strVarName, strData);
		}
	}
};