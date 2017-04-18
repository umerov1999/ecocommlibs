#pragma once
#include <afxwin.h>  
#include <afxext.h>

class SISShipNoInfo
{
public:
	CString m_strGroup;
	CString m_strShipNo;

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISShipNoInfo() 
	{
		m_strGroup = _T("");
		m_strShipNo = _T("");
	}

	/**
	@brief Creator
	@param c [in]Object of SISNShipNoInfo
	@return
	@remark Initialize member variable using value of input object
	*/
	SISShipNoInfo(const SISShipNoInfo& c)
	{
		*this = c;
	}

	/**
	@brief Creator
	@param [in]strGroup Group of ship number
	@param [in]strShipNo Ship number
	@return
	@remark Initialize member variable using parameter using parameter
	*/
	SISShipNoInfo(CString strGroup, CString strShipNo)
	{
		m_strGroup = strGroup;
		m_strShipNo = strShipNo;
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISShipNoInfo& operator = (const SISShipNoInfo &other)
	{
		m_strGroup = other.m_strGroup;
		m_strShipNo = other.m_strShipNo;
	
		return *this;
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	BOOL operator == (const SISShipNoInfo &other)
	{
		if (m_strGroup == other.m_strGroup && m_strShipNo == other.m_strShipNo)
			return TRUE;

		return FALSE;
	}

	/**
	@brief Set group of ship number
	@param [in]strGroup Group of ship number
	@return
	@remark
	*/
	void SetGroup(CString strGroup)
	{
		m_strGroup = strGroup;
	}

	/**
	@brief Get group of ship number
	@param
	@return group of ship number
	@remark
	*/
	CString GetGroup()
	{
		return m_strGroup;
	}

	/**
	@brief Set ship number
	@param [in]strGroup Group of ship number
	@return
	@remark
	*/
	void SetShipNo(CString strShipNo)
	{
		m_strShipNo = strShipNo;
	}

	/**
	@brief Get ship number
	@param
	@return ship number
	@remark
	*/
	CString GetShipNo()
	{
		return m_strShipNo;
	}
};