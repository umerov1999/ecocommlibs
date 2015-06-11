class CXTPResourceImages;

class _XTP_EXT_CLASS CXTPCommandBarsColorSetButton
{
public:
	CXTPPaintManagerColor m_clrButtonBorder;      // Button border color
	CXTPPaintManagerColor m_clrButtonBackground;  // Button background color
};

class _XTP_EXT_CLASS CXTPCommandBarsColorSetDropDown
{
public:
	CXTPPaintManagerColor m_clrDropDownBorder;      // Color of Drop-down border
	CXTPPaintManagerColor m_clrDropDownBackground;  // Color of Drop-down background
	CXTPPaintManagerColor m_clrDropDownMarker;      // Color of Drop-down marker
};


class _XTP_EXT_CLASS CXTPCommandBarsColorSetComboBox
{
public:
	CXTPPaintManagerColor m_clrComboBoxBorder;      // Combo-box background color
	CXTPPaintManagerColor m_clrComboBoxBackground;  // Combo-box background color

	CXTPCommandBarsColorSetDropDown m_dropDown;
};

class _XTP_EXT_CLASS CXTPCommandBarsColorSet
{
public:

	virtual void RefreshColors(const CXTPResourceImages *pImages);

public:

	CXTPPaintManagerColor m_clrAppWorkspace;       // Application workspace

	CXTPPaintManagerColor m_clrToolBarBackground;  // Background color or menubar and toolbar
	CXTPPaintManagerColor m_clrToolBarGripper;     // Gripper color
	CXTPPaintManagerColor m_clrToolBarText;

	CXTPPaintManagerColor m_clrMenuText;            // Pop-up menu text color
	CXTPPaintManagerColor m_clrMenuTextHighlighted; // Pop-up menu text color

	CXTPPaintManagerColor m_clrMenuBorder;
	CXTPPaintManagerColor m_clrMenuBackground;

	CXTPPaintManagerColor m_clrButtonBorderHighlighted;
	CXTPPaintManagerColor m_clrButtonBorderPushed;
	CXTPPaintManagerColor m_clrButtonBorderChecked;
	CXTPPaintManagerColor m_clrButtonBorderPopped;

	CXTPPaintManagerColor m_clrButtonBackgroundHighlighted;
	CXTPPaintManagerColor m_clrButtonBackgroundPushed;
	CXTPPaintManagerColor m_clrButtonBackgroundChecked;
	CXTPPaintManagerColor m_clrButtonBackgroundPopped;

	CXTPPaintManagerColor m_clrControlText;
	CXTPPaintManagerColor m_clrControlTextDisabled;
	CXTPPaintManagerColor m_clrControlTextHighlighted;

	CXTPPaintManagerColor m_clrToolbarFace;

	CXTPPaintManagerColor m_clrEditCtrlBorder;

	// check mark.
	CXTPPaintManagerColor m_clrCheckMark;
	CXTPPaintManagerColor m_clrCheckMarkHighlighted;
	CXTPPaintManagerColor m_clrCheckMarkPushed;
	CXTPPaintManagerColor m_clrCheckMarkDisabled;

	CXTPPaintManagerColor m_clrCheckMarkBackground;
	CXTPPaintManagerColor m_clrCheckMarkBackgroundHighlighted;
	CXTPPaintManagerColor m_clrCheckMarkBackgroundPushed;
	CXTPPaintManagerColor m_clrCheckMarkBackgroundDisabled;

	// Combo-box
	CXTPCommandBarsColorSetComboBox m_csComboBoxDisabled;
	CXTPCommandBarsColorSetComboBox m_csComboBoxNormal;
	CXTPCommandBarsColorSetComboBox m_csComboBoxHighlighted;
	CXTPCommandBarsColorSetComboBox m_csComboBoxDropped;

	// command bar
	CXTPPaintManagerColor m_clrSeparator;
	CXTPPaintManagerColor m_clrSeparatorHighlight;
	CXTPPaintManagerColor m_clrGlyph;
	CXTPPaintManagerColor m_clrGlyphPushed;
	CXTPPaintManagerColor m_clrGlyphHighlighted;
};

AFX_INLINE void CXTPCommandBarsColorSet::RefreshColors(const CXTPResourceImages *pImages)
{
	UNREFERENCED_PARAMETER(pImages);
}
