#pragma once
#include <afxwin.h>  
#include <afxext.h>

class SISTime
{
public:
	int m_nHour;
	int m_nMinute;
	int m_nSecond;

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISTime()
	{
		m_nHour = 0;
		m_nMinute = 0;
		m_nSecond = 0;
	}

	/**
	@brief Creator
	@param c [in]Object of SISTime
	@return
	@remark Initialize member variable using value of input object
	*/
	SISTime(const SISTime& c)
	{
		*this = c;
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISTime& operator = (const SISTime &other)
	{
		m_nHour = other.m_nHour;
		m_nMinute = other.m_nMinute;
		m_nSecond = other.m_nSecond;
	
		return *this;
	}

	/**
	@brief Set time
	@param nHour [in]Hour
	@param nMinute [in]Minute
	@param nSecond [in]Second
	@return
	@remark
	*/
	void SetTime(int nHour, int nMinute, int nSecond)
	{
		m_nHour = nHour; m_nMinute = nMinute; m_nSecond = nSecond;
	}

	/**
	@brief Get time
	@param nHour [out]Hour
	@param nMinute [out]Minute
	@param nSecond [out]Second
	@return
	@remark
	*/
	void GetTime(int& nHour, int& nMinute, int& nSecond)
	{
		nHour = m_nHour; nMinute = m_nMinute; nSecond = m_nSecond;
	}

	/**
	@brief Get time
	@param 
	@return String format
	@remark
	*/
	CString GetString()
	{
		CString strTmp = _T("");
		strTmp.Format(_T("%d:%d:%d"), m_nHour, m_nMinute, m_nSecond);
		return strTmp;
	}
};
