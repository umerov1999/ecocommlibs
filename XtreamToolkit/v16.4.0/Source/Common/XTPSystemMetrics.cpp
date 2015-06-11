#include "StdAfx.h"
#include <Common/XTPSystemMetrics.h>


CXTPSystemMetrics *XTPSystemMetrics()
{
	static CXTPSystemMetrics systemMetrics;
	return &systemMetrics;
}

CXTPSystemMetrics::CXTPSystemMetrics()
	: m_nBorderY   (0)
	, m_nCaptionY  (0)
	, m_nSizeFrameY(0)
{
	RefreshMetrics();
}

void CXTPSystemMetrics::RefreshMetrics()
{
	CXTPSystemMetrics systemMetricsOld(*this);

	m_nBorderY    = ::GetSystemMetrics(SM_CYBORDER);
	m_nCaptionY   = ::GetSystemMetrics(SM_CYCAPTION);
	m_nSizeFrameY = ::GetSystemMetrics(SM_CYSIZEFRAME);

	// Frame icon
	m_nSmallIconX = ::GetSystemMetrics(SM_CXSMICON);
	m_nSmallIconY = ::GetSystemMetrics(SM_CYSMICON);
}


CXTPDeviceCaps *XTPDeviceCaps()
{
	static CXTPDeviceCaps deviceCaps;
	return &deviceCaps;
}

CXTPDeviceCaps::CXTPDeviceCaps()
	: m_nLogPixelsX(0)
	, m_nLogPixelsY(0)
{
	RefreshMetrics();
}

void CXTPDeviceCaps::RefreshMetrics()
{
	// Log pixels
	m_nLogPixelsX = ::GetDeviceCaps(::GetDC(NULL), LOGPIXELSX);
	m_nLogPixelsY = ::GetDeviceCaps(::GetDC(NULL), LOGPIXELSY);
}
