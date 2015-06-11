#include "StdAfx.h"

#include <Common/XTPColorManager.h>
#include <Common/XTPResourceImage.h>
#include <Common/XTPTheme.h>

void CXTPColorSetCaptionButton::RefreshMetrics(
	CXTPResourceImages *pImages, LPCTSTR lpszSectionName)
{
	m_clrBackground = pImages->GetImageColor(lpszSectionName, _T("BackgroundColor"));
	m_clrOutline    = pImages->GetImageColor(lpszSectionName, _T("OutlineColor"));
	m_clrFill       = pImages->GetImageColor(lpszSectionName, _T("FillColor"));
}
