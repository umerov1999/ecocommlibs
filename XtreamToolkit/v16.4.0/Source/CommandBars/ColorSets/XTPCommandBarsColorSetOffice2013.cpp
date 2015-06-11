#include "StdAfx.h"
#include <Common/XTPColorManager.h>
#include <CommandBars/XTPCommandBarsColorSet.h>
#include <CommandBars/ColorSets/XTPCommandBarsColorSetOffice2013.h>


//////////////////////////////////////////////////////////////////////////
// CXTPCommandBarsColorSetOffice2013

CXTPCommandBarsColorSetOffice2013::CXTPCommandBarsColorSetOffice2013()
{
}

void CXTPCommandBarsColorSetOffice2013::RefreshColors(const CXTPResourceImages *pImages)
{
	UNUSED_ALWAYS(pImages);
}


//////////////////////////////////////////////////////////////////////////
// CXTPCommandBarsColorSetOffice2013Word

CXTPCommandBarsColorSetOffice2013Word::CXTPCommandBarsColorSetOffice2013Word()
{
}

void CXTPCommandBarsColorSetOffice2013Word::RefreshColors(const CXTPResourceImages *pImages)
{
	UNUSED_ALWAYS(pImages);

	m_clrMenuBorder = RGB(212, 212, 212);
	m_clrMenuBackground = RGB(255, 255, 255);

	m_clrButtonBorderHighlighted = RGB(213, 225, 242);
	m_clrButtonBackgroundHighlighted = RGB(213, 225, 242);

	m_clrButtonBorderPushed = RGB(163, 189, 227);
	m_clrButtonBackgroundPushed = RGB(163, 189, 227);

	m_clrButtonBorderChecked = RGB(194, 213, 242);
	m_clrButtonBackgroundChecked = RGB(194, 213, 242);

	m_clrSeparator = RGB(225,225,225);

	m_clrToolbarFace = RGB(255, 255, 255);

	m_clrEditCtrlBorder = RGB(171, 171, 171);
}
