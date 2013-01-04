#include "StdAfx.h"
#include "XTPNumberFormat.h"

CXTPNumberFormat::CXTPNumberFormat()
{
	m_numberFormat.lpDecimalSep  = m_sDecimalSep.GetBuffer(BufferSize);
	m_numberFormat.lpThousandSep = m_sThousandSep.GetBuffer(BufferSize);
}

CXTPNumberFormat::~CXTPNumberFormat()
{
	m_sDecimalSep.ReleaseBuffer();
	m_sThousandSep.ReleaseBuffer();
}



BSTR CXTPNumberFormat::OleGetDecimalSeperator()
{
	return m_sDecimalSep.AllocSysString();
}
