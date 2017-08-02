#pragma once

#ifndef _SISAttachedFile_H
#define _SISAttachedFile_H

#include <afxwin.h>  
#include <afxext.h>

class SISAttachedFile
{
public:
	UINT m_nFileID;
	CString m_strFileName;

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISAttachedFile()
	{
		m_nFileID = 0;
		m_strFileName = _T("");
	}

	/**
	@brief Creator
	@param c [in]Object of SISNAttachedFile
	@return
	@remark Initialize member variable using value of input object
	*/
	SISAttachedFile(const SISAttachedFile& c)
	{
		*this = c;
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISAttachedFile& operator = (const SISAttachedFile &other)
	{	
		m_nFileID = other.m_nFileID;
		m_strFileName = other.m_strFileName;
	
		return *this;
	}

	/**
	@brief Set attached file ID
	@param nFileID [in]Attached file ID
	@return
	@remark
	*/
	void SetAttachedFileID(UINT nOrder)
	{
		m_nFileID = nOrder;
	}

	/**
	@brief Get attached file ID
	@param
	@return Attached file ID
	@remark
	*/
	UINT GetAttachedFileID()
	{
		return m_nFileID;
	}

	/**
	@brief Set attached file name
	@param strFileName [in]Attached file name
	@return
	@remark
	*/
	void SetAttachedFileName(CString strFileName)
	{
		m_strFileName = strFileName;
	}

	/**
	@brief Get attached file name
	@param
	@return Attached file name
	@remark
	*/
	CString GetAttachedFileName()
	{
		return m_strFileName;
	}
};

#endif