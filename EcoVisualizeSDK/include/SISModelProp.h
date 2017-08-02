#pragma once

#ifndef _SISModelProp_H
#define _SISModelProp_H

#include <afxwin.h>  
#include <afxext.h>

class SISModelProp
{
public:
	CString m_strModelName;
	CMap<CString, LPCTSTR, CString, LPCTSTR> m_mapProp;

public:

	SISModelProp() : m_strModelName(_T("")) 
	{
		m_mapProp.RemoveAll();
	}

	SISModelProp& operator = (const SISModelProp &other)
	{
		m_strModelName = other.m_strModelName;
		
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
};

#endif