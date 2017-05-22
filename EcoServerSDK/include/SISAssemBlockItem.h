#pragma once
#include <afxwin.h>  
#include <afxext.h>

class SISAssemBlockItem
{
public:
	CString m_strAssemLevel;
	CString m_strAssemBlock;

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISAssemBlockItem()
	{
		m_strAssemLevel = _T("");
		m_strAssemBlock = _T("");
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISAssemBlockItem& operator = (const SISAssemBlockItem &other)
	{
		m_strAssemLevel = other.m_strAssemLevel;
		m_strAssemBlock = other.m_strAssemBlock;
	
		return *this;
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	BOOL operator == (const SISAssemBlockItem &other)
	{
		if (m_strAssemLevel == other.m_strAssemLevel && m_strAssemBlock == other.m_strAssemBlock)
			return TRUE;

		return FALSE;
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

};