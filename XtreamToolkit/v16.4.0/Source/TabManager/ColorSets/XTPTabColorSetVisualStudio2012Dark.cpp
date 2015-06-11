#include "StdAfx.h"

#include <Common/XTPColorManager.h>
#include <TabManager/Includes.h>

#define Base CXTPTabColorSetVisualStudio2012

CXTPTabColorSetVisualStudio2012Dark::CXTPTabColorSetVisualStudio2012Dark()
	: Base()
{
}

void CXTPTabColorSetVisualStudio2012Dark::RefreshMetrics()
{
	Base::RefreshMetrics();

	m_clrButtonNormal      = RGB(45,45,48);
	m_clrButtonSelected    = RGB(  0, 122, 204);
	m_clrButtonHighlighted = RGB( 28, 151, 234);
	m_clrButtonPressed     = RGB(  0, 122, 204);

	m_clrTextNormal    = RGB(  211,239,239);
	m_clrTextHighlight = RGB(255, 255, 255);
	m_clrTextSelected  = RGB(255, 255, 255);

	m_clrActiveFilesButton            = RGB(241, 241, 241);
	m_clrActiveFilesButtonHighlighted = RGB(  0, 122, 204);
	m_clrActiveFilesButtonPressed     = RGB(255, 255, 255);

	m_clrActiveFilesButtonBackground            = RGB( 45,  45,  48);
	m_clrActiveFilesButtonBackgroundHighlighted = RGB( 62,  62,  64);
	m_clrActiveFilesButtonBackgroundPressed     = RGB(  0, 122, 204);

	m_clrFrameBorder = RGB(  0, 122, 204);
	m_clrAutoHideFace = RGB(239, 239, 242);

	m_clrHeaderFace = m_clrButtonNormal;
}
