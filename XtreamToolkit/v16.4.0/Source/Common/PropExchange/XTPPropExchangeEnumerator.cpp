#include "StdAfx.h"
#include <Common/XTPPropExchange.h>
#include <Common/XTPVC80Helpers.h>


//////////////////////////////////////////////////////////////////////////
// CXTPPropExchangeEnumerator

CXTPPropExchangeEnumerator::CXTPPropExchangeEnumerator(CXTPPropExchange* pContainer, LPCTSTR lpszSection)
{
	m_pContainer = pContainer;
	m_strSectionName = lpszSection;
	m_nCount = 0;
}

CXTPPropExchangeEnumerator::~CXTPPropExchangeEnumerator()
{

}

POSITION CXTPPropExchangeEnumerator::GetPosition(DWORD dwCount /*= 0*/, BOOL bCompactMode /*= TRUE*/)
{
	m_nCount = dwCount;

	if (!m_pContainer)
		return NULL;

	if (!bCompactMode)
	{
		PX_DWord(m_pContainer, _T("Count"), m_nCount, 0);
	}
	else
	{
		if (m_pContainer->IsStoring())
			m_pContainer->WriteCount(m_nCount);
		else
			m_nCount = m_pContainer->ReadCount();
	}

	return m_nCount == 0 ? NULL : (POSITION)1;
}

CXTPPropExchange* CXTPPropExchangeEnumerator::GetNext(POSITION& pos)
{
	CString strSection;
	strSection.Format(_T("%s%i"), (LPCTSTR)m_strSectionName, (DWORD)(DWORD_PTR)pos - 1);

	CXTPPropExchange* pSection = m_pContainer->GetSection(strSection);

	pos++;
	if ((DWORD)(DWORD_PTR)pos > m_nCount) pos = NULL;

	return pSection;
}
