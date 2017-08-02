#pragma once

#ifndef _SISAssembly_H
#define _SISAssembly_H

#include <afxwin.h>  
#include <afxext.h>

class SISAssembly
{
private:
	CString m_strAssemName;
	CString m_strAssemPath;

public:

	SISAssembly()
	{
		m_strAssemName = _T("");
		m_strAssemPath = _T("");
	}

	SISAssembly& operator = (const SISAssembly &other)
	{
		m_strAssemName = other.m_strAssemName;
		m_strAssemPath = other.m_strAssemPath;

		return *this;
	}

	void SetAssemblyName(CString strName)
	{
		m_strAssemName = strName;
	}

	CString GetAssemblyName()
	{
		return m_strAssemName;
	}

	void SetAssemblyPath(CString strPath)
	{
		m_strAssemPath = strPath;
	}

	CString GetAssemblyPath()
	{
		return m_strAssemPath;
	}
};

#endif