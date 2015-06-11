#include "StdAfx.h"
#include <Common/XTPPropExchange.h>

CXTPPropExchangeEnumeratorPtr::CXTPPropExchangeEnumeratorPtr(CXTPPropExchangeEnumerator *pPX)
	: m_pPX(pPX)
{
}

CXTPPropExchangeEnumeratorPtr::~CXTPPropExchangeEnumeratorPtr()
{
	delete m_pPX;
}

CXTPPropExchangeEnumerator& CXTPPropExchangeEnumeratorPtr::operator*() const
{
	return *m_pPX;
}

CXTPPropExchangeEnumerator* CXTPPropExchangeEnumeratorPtr::operator->() const
{
	return m_pPX;
}

CXTPPropExchangeEnumerator* CXTPPropExchangeEnumeratorPtr::operator&() const
{
	return m_pPX;
}
