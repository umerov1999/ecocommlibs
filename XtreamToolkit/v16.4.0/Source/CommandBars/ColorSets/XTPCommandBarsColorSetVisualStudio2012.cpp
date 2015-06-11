#include "StdAfx.h"
#include <Common/XTPColorManager.h>
#include <CommandBars/XTPCommandBarsColorSet.h>
#include <CommandBars/ColorSets/XTPCommandBarsColorSetVisualStudio2012.h>


//////////////////////////////////////////////////////////////////////////
// CXTPCommandBarsColorSetVisualStudio2012

CXTPCommandBarsColorSetVisualStudio2012::CXTPCommandBarsColorSetVisualStudio2012()
{

}

void CXTPCommandBarsColorSetVisualStudio2012::RefreshColors(const CXTPResourceImages *pImages)
{
	UNUSED_ALWAYS(pImages);
}


//////////////////////////////////////////////////////////////////////////
// CXTPCommandBarsColorSetVisualStudio2012Light

CXTPCommandBarsColorSetVisualStudio2012Light::CXTPCommandBarsColorSetVisualStudio2012Light()
{

}

void CXTPCommandBarsColorSetVisualStudio2012Light::RefreshColors(const CXTPResourceImages *pImages)
{
	UNUSED_ALWAYS(pImages);

	m_clrAppWorkspace = RGB(239,239,242);

	m_clrToolBarBackground = RGB(239, 239, 242);
	m_clrToolBarGripper    = RGB(153, 153, 153);
	m_clrToolBarText       = RGB( 30, 30, 30);

	m_clrMenuText            = RGB( 30, 30, 30);
	m_clrMenuTextHighlighted = RGB( 30, 30, 30);

	m_clrMenuBorder     = RGB(204, 206, 219);
	m_clrMenuBackground = RGB(231, 232, 236); // Pop-up menu background color

	m_clrButtonBorderHighlighted = RGB(254, 254, 254);
	m_clrButtonBorderPushed      = RGB(  0 ,122, 204);
	m_clrButtonBorderChecked     = RGB( 50, 153, 255);
	m_clrButtonBorderPopped      = RGB(  0 ,122, 204);

	m_clrButtonBackgroundHighlighted = RGB(254, 254, 254);
	m_clrButtonBackgroundPushed      = RGB(  0 ,122, 204);
	m_clrButtonBackgroundChecked     = RGB(240, 239, 241);
	m_clrButtonBackgroundPopped      = RGB(  0 ,122, 204);

	m_clrCheckMark            = RGB(113, 113, 113);
	m_clrCheckMarkHighlighted = RGB( 30,  30,  30);
	m_clrCheckMarkPushed      = RGB( 30,  30,  30);
	m_clrCheckMarkDisabled    = RGB(198, 198, 198);

	m_clrCheckMarkBackground            = RGB(239, 239, 242);
	m_clrCheckMarkBackgroundHighlighted = RGB(254, 254, 254);
	m_clrCheckMarkBackgroundPushed      = RGB(254, 254, 254);
	m_clrCheckMarkBackgroundDisabled    = RGB(239, 239, 242);

	m_clrSeparator = RGB(205,208,214);
	m_clrSeparatorHighlight = RGB(247,246,248);
	m_clrGlyph            = RGB(113, 113, 113);
	m_clrGlyphPushed      = RGB(255, 255, 255);
	m_clrGlyphHighlighted = RGB(  0 ,122, 204);

	// Combo-box

	m_csComboBoxDisabled.m_clrComboBoxBorder                = RGB(198, 198, 198);
	m_csComboBoxDisabled.m_clrComboBoxBackground            = RGB(239, 239, 242);
	m_csComboBoxDisabled.m_dropDown.m_clrDropDownBorder     = RGB(239, 239, 242);
	m_csComboBoxDisabled.m_dropDown.m_clrDropDownBackground = RGB(239, 239, 242);
	m_csComboBoxDisabled.m_dropDown.m_clrDropDownMarker     = RGB(198, 198, 198);

	m_csComboBoxNormal.m_clrComboBoxBorder                = RGB(219, 221, 230);
	m_csComboBoxNormal.m_clrComboBoxBackground            = RGB(252, 252, 252);
	m_csComboBoxNormal.m_dropDown.m_clrDropDownBorder     = RGB(252, 252, 252);
	m_csComboBoxNormal.m_dropDown.m_clrDropDownBackground = RGB(252, 252, 252);
	m_csComboBoxNormal.m_dropDown.m_clrDropDownMarker     = RGB(113, 113, 113);

	m_csComboBoxHighlighted.m_clrComboBoxBorder                = RGB(219, 221, 230);
	m_csComboBoxHighlighted.m_clrComboBoxBackground            = RGB(255, 255, 255);
	m_csComboBoxHighlighted.m_dropDown.m_clrDropDownBorder     = RGB(  0, 122, 204);
	m_csComboBoxHighlighted.m_dropDown.m_clrDropDownBackground = RGB(222, 223, 232);
	m_csComboBoxHighlighted.m_dropDown.m_clrDropDownMarker     = RGB(  0, 122, 204);

	m_csComboBoxDropped.m_clrComboBoxBorder                = RGB(219, 221, 230);
	m_csComboBoxDropped.m_clrComboBoxBackground            = RGB(255, 255, 255);
	m_csComboBoxDropped.m_dropDown.m_clrDropDownBorder     = RGB(255, 255, 255);
	m_csComboBoxDropped.m_dropDown.m_clrDropDownBackground = RGB(  0, 122, 204);
	m_csComboBoxDropped.m_dropDown.m_clrDropDownMarker     = RGB(255, 255, 255);
}


//////////////////////////////////////////////////////////////////////////
// CXTPCommandBarsColorSetVisualStudio2012Dark

CXTPCommandBarsColorSetVisualStudio2012Dark::CXTPCommandBarsColorSetVisualStudio2012Dark()
{

}

void CXTPCommandBarsColorSetVisualStudio2012Dark::RefreshColors(const CXTPResourceImages *pImages)
{
	UNUSED_ALWAYS(pImages);

	m_clrAppWorkspace = RGB(45,45,48);

	m_clrToolBarBackground = RGB( 45,  45,  48);
	m_clrToolBarGripper    = RGB( 70,  70,  74);
	m_clrToolBarText       = RGB(241, 241, 241);

	m_clrMenuText            = RGB(241, 241, 241);
	m_clrMenuTextHighlighted = RGB(241, 241, 241);

	m_clrMenuBorder     = RGB( 51,  51,  55);
	m_clrMenuBackground = RGB( 27,  27,  28); // Pop-up menu background color

	m_clrButtonBorderHighlighted = RGB(62,  62,  64);
	m_clrButtonBorderPushed      = RGB( 0, 122, 204);
	m_clrButtonBorderChecked     = RGB(50, 153, 255);
	m_clrButtonBorderPopped      = RGB( 0, 122, 204);

	m_clrButtonBackgroundHighlighted = RGB(62,  62,  64);
	m_clrButtonBackgroundPushed      = RGB( 1, 120, 208);
	m_clrButtonBackgroundChecked     = RGB( 45,  45, 48);
	m_clrButtonBackgroundPopped      = RGB(  0 ,122, 204);

	m_clrCheckMarkHighlighted = RGB(241, 241, 241);
	m_clrCheckMarkPushed      = RGB(241, 241, 241);
	m_clrCheckMark            = RGB(153, 153, 153);
	m_clrCheckMarkDisabled    = RGB( 67,  67,  70);

	m_clrCheckMarkBackgroundHighlighted = RGB(62, 62, 64);
	m_clrCheckMarkBackgroundPushed      = RGB(62, 62, 64);
	m_clrCheckMarkBackground            = RGB(45, 45, 48);
	m_clrCheckMarkBackgroundDisabled    = RGB(45, 45, 48);

	m_clrControlText            = RGB(255,255,255);
	m_clrControlTextDisabled    = RGB(153,153,153);
	m_clrControlTextHighlighted = RGB(255,255,255);

	m_clrSeparator = RGB(35,34,36);
	m_clrSeparatorHighlight = RGB(72,71,73);

	m_clrGlyph            = RGB(153, 153, 153);
	m_clrGlyphPushed      = RGB(255, 255, 255);
	m_clrGlyphHighlighted = RGB(  0 ,122, 204);

	// Combo-box

	m_csComboBoxDisabled.m_clrComboBoxBorder                = RGB( 67,  67,  70);
	m_csComboBoxDisabled.m_clrComboBoxBackground            = RGB( 45,  45,  48);
	m_csComboBoxDisabled.m_dropDown.m_clrDropDownBorder     = RGB( 45,  45,  48);
	m_csComboBoxDisabled.m_dropDown.m_clrDropDownBackground = RGB( 45,  45,  48);
	m_csComboBoxDisabled.m_dropDown.m_clrDropDownMarker     = RGB( 67,  67,  70);

	m_csComboBoxNormal.m_clrComboBoxBorder                = RGB( 67,  67,  70);
	m_csComboBoxNormal.m_clrComboBoxBackground            = RGB( 51,  51,  55);
	m_csComboBoxNormal.m_dropDown.m_clrDropDownBorder     = RGB( 51,  51,  51);
	m_csComboBoxNormal.m_dropDown.m_clrDropDownBackground = RGB( 51,  51,  51);
	m_csComboBoxNormal.m_dropDown.m_clrDropDownMarker     = RGB(153, 153, 153);

	m_csComboBoxHighlighted.m_clrComboBoxBorder                = RGB( 67,  67,  70);
	m_csComboBoxHighlighted.m_clrComboBoxBackground            = RGB( 63,  63,  70);
	m_csComboBoxHighlighted.m_dropDown.m_clrDropDownBorder     = RGB(  0, 122, 204);
	m_csComboBoxHighlighted.m_dropDown.m_clrDropDownBackground = RGB( 31,  31,  32);
	m_csComboBoxHighlighted.m_dropDown.m_clrDropDownMarker     = RGB(  0, 122, 204);

	m_csComboBoxDropped.m_clrComboBoxBorder                = RGB( 67,  67,  70);
	m_csComboBoxDropped.m_clrComboBoxBackground            = RGB( 63,  63,  70);
	m_csComboBoxDropped.m_dropDown.m_clrDropDownBorder     = RGB( 63,  63,  70);
	m_csComboBoxDropped.m_dropDown.m_clrDropDownBackground = RGB(  0, 122, 204);
	m_csComboBoxDropped.m_dropDown.m_clrDropDownMarker     = RGB(255, 255, 255);
}
