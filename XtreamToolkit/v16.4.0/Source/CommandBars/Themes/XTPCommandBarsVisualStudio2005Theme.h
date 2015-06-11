//===========================================================================
// Summary:
//     The CXTPVisualStudio2005Theme class is used to enable a Visual Studio 2005 style theme for Command Bars
// See Also: CXTPPaintManager::SetTheme
//===========================================================================
class _XTP_EXT_CLASS CXTPVisualStudio2005Theme : public CXTPCommandBarsOffice2003Theme
{
	DECLARE_DYNAMIC(CXTPVisualStudio2005Theme)
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPVisualStudio2005Theme object.
	//-----------------------------------------------------------------------
	CXTPVisualStudio2005Theme();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPVisualStudio2005Theme object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPVisualStudio2005Theme();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the manager.
	//-----------------------------------------------------------------------
	void RefreshMetrics();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the base theme of the manager.
	// Returns:
	//     Returns the base theme.
	//-----------------------------------------------------------------------
	virtual XTPPaintTheme BaseTheme() { return xtpThemeWhidbey; }
};

typedef CXTPVisualStudio2005Theme CXTPWhidbeyTheme; // Old name
