#include "StdAfx.h"

#include <Common/XTPResourceImage.h>
#include <Common/XTPWinThemeWrapper.h>
#include <Common/XTPColorManager.h>
#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollBarPaintManager.h>
#include <Common/ScrollBar/Themes/XTPScrollBarThemeWindowsClassic.h>
#include <Common/ScrollBar/Themes/XTPScrollBarThemeWindowsUx.h>
#include <Common/ScrollBar/Themes/XTPScrollBarThemeOffice2003.h>


void CXTPScrollBarThemeOffice2003::RefreshMetrics()
{
	CXTPScrollBarThemeWindowsUx::RefreshMetrics();

	m_crBackPushed = GetXtremeColor(XPCOLOR_HIGHLIGHT_PUSHED);
	m_crBackHilite = GetXtremeColor(XPCOLOR_HIGHLIGHT);
	m_crBorderHilite = GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER);
}
