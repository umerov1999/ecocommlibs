#include "StdAfx.h"

#include <Common/XTPColorManager.h>
#include <Common/XTPTheme.h>
#include <Common/XTPResourceImage.h>

#include <DockingPane/ColorSets/XTPDockingPaneColorSetVisualStudio2012.h>

void CXTPDockingPaneColorSetVisualStudio2012::RefreshMetrics(CXTPResourceImages *pImages, LPCTSTR lpszSectionName)
{
	if ((NULL != pImages) && (NULL != lpszSectionName))
	{
		m_clrBorder     = pImages->GetImageColor(lpszSectionName, _T("BorderColor"));
		m_clrBackground = pImages->GetImageColor(lpszSectionName, _T("BackgroundColor"));
		m_clrCaption    = pImages->GetImageColor(lpszSectionName, _T("CaptionColor"));
		m_clrText       = pImages->GetImageColor(lpszSectionName, _T("TextColor"));
		m_clrGripper    = pImages->GetImageColor(lpszSectionName, _T("GripperColor"));

		CString sSectionName(lpszSectionName);
		m_csCaptionButtonNormal .RefreshMetrics(pImages, sSectionName + _T(".CaptionButton.Normal"));
		m_csCaptionButtonHot    .RefreshMetrics(pImages, sSectionName + _T(".CaptionButton.Hot"));
		m_csCaptionButtonPressed.RefreshMetrics(pImages, sSectionName + _T(".CaptionButton.Pressed"));
	}
}


void CXTPDockingPaneColorSetVisualStudio2012Light::RefreshMetrics(CXTPResourceImages *pImages, LPCTSTR lpszSectionName)
{
	CXTPDockingPaneColorSetVisualStudio2012::RefreshMetrics(pImages, lpszSectionName);
}


void CXTPDockingPaneColorSetVisualStudio2012Dark::RefreshMetrics(CXTPResourceImages *pImages, LPCTSTR lpszSectionName)
{
	CXTPDockingPaneColorSetVisualStudio2012::RefreshMetrics(pImages, lpszSectionName);
}
