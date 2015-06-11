#include "StdAfx.h"

#include <Common/XTPResourceImage.h>
#include <Common/XTPWinThemeWrapper.h>
#include <Common/XTPColorManager.h>
#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollBarPaintManager.h>
#include <Common/ScrollBar/Themes/XTPScrollBarThemeWindowsClassic.h>
#include <Common/ScrollBar/Themes/XTPScrollBarThemeWindowsUx.h>
#include <Common/ScrollBar/Themes/XTPScrollBarThemeFlat.h>
#include <Common/ScrollBar/Themes/XTPScrollBarThemeUltraFlat.h>


void CXTPScrollBarThemeUltraFlat::RefreshMetrics()
{
	CXTPScrollBarThemeFlat::RefreshMetrics();
	m_crBorderHilite = m_crBorder;

	COLORREF clrWindow = GetXtremeColor(COLOR_WINDOW);
	COLORREF clrFace   = GetXtremeColor(COLOR_3DFACE);
	COLORREF clrPushed = GetXtremeColor(COLOR_3DSHADOW);

	m_crBackPushed = XTPColorManager()->LightColor(clrPushed, clrFace, 0x32);
	m_crBackHilite = XTPColorManager()->LightColor(clrFace, clrWindow, 0x1E);
}
