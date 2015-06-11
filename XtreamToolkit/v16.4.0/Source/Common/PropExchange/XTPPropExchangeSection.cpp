#include "StdAfx.h"
#include <Common/XTPPropExchange.h>


CXTPPropExchangeSection::CXTPPropExchangeSection(CXTPPropExchange *pPX)
	: m_pPX(pPX)
{
}

CXTPPropExchangeSection::CXTPPropExchangeSection(BOOL bLoading, LPCTSTR lpszSection)
{
	if (AfxGetApp()->m_pszRegistryKey)
	{
		m_pPX = new CXTPPropExchangeRegistry(bLoading, 0, lpszSection);
	}
	else
	{
		CXTPPropExchangeIniFile *pPX = new CXTPPropExchangeIniFile(bLoading, 0, lpszSection);
		pPX->AssignFile(AfxGetApp()->m_pszProfileName);
		m_pPX = pPX;
	}
}

CXTPPropExchangeSection::~CXTPPropExchangeSection()
{
	if (m_pPX)
	{
		m_pPX->InternalRelease();
	}
}

CXTPPropExchange& CXTPPropExchangeSection::operator*() const
{
	return *m_pPX;
}

CXTPPropExchange* CXTPPropExchangeSection::operator->() const
{
	return m_pPX;
}

CXTPPropExchange* CXTPPropExchangeSection::operator&() const
{
	return m_pPX;
}
