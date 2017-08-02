#pragma once

#ifndef _SISPointStr_H
#define _SISPointStr_H

#include <afxwin.h>  
#include <afxext.h>

class SISPointStr
{
private:
	CString m_strX;
	CString m_strY;
	CString m_strZ;

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISPointStr()
	{
		m_strX = _T("");
		m_strY = _T("");
		m_strZ = _T("");
	}

	/**
	@brief Creator
	@param strX [in]X Value
	@param strY [in]Y Value
	@param strZ [in]Z Value
	@return
	@remark Initialize member variable using parameter
	*/
	SISPointStr(CString strX, CString strY, CString strZ)
	{
		m_strX = strX;
		m_strY = strY;
		m_strZ = strZ;
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISPointStr& operator = (const SISPointStr &other)
	{
		m_strX = other.m_strX;
		m_strY = other.m_strY;
		m_strZ = other.m_strZ;
	
		return *this;
	}

	/**
	@brief Set X value of the string form
	@param strX [in]X value
	@return
	@remark
	*/
	void SetX(CString strX)
	{
		m_strX = strX;
	}

	/**
	@brief Get X value of the string form
	@param
	@return X value
	@remark
	*/
	CString GetX()
	{
		return m_strX;
	}

	/**
	@brief Set Y value of the string form
	@param strY [in]Y value
	@return
	@remark
	*/
	void SetY(CString strY)
	{
		m_strY = strY;
	}

	/**
	@brief Get Y value of the string form
	@param
	@return Y value
	@remark
	*/
	CString GetY()
	{
		return m_strY;
	}

	/**
	@brief Set Z value of the string form
	@param strZ [in]Z value
	@return
	@remark
	*/
	void SetZ(CString strZ)
	{
		m_strZ = strZ;
	}

	/**
	@brief Get Z value of the string form
	@param
	@return Z value
	@remark
	*/
	CString GetZ()
	{
		return m_strZ;
	}

};

#endif