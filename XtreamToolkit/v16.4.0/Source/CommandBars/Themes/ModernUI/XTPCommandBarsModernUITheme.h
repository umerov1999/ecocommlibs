//===========================================================================
// Summary:
//     The CXTPCommandBarsModernUITheme class is the base class for the
//      Modern UI themes for Command Bars.
// See Also: CXTPPaintManager::SetTheme
//===========================================================================
class _XTP_EXT_CLASS CXTPCommandBarsModernUITheme : public CXTPCommandBarsOfficeXPTheme
{
	DECLARE_DYNAMIC(CXTPCommandBarsModernUITheme)
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPCommandBarsModernUITheme object.
	//-----------------------------------------------------------------------
	CXTPCommandBarsModernUITheme();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPCommandBarsModernUITheme object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPCommandBarsModernUITheme();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the manager.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

// Paint manager
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns paint manager used to draw frames.
	// Returns:
	//     Returns paint manager used to draw frames.
	//-----------------------------------------------------------------------
	virtual CXTPFramePaintManager* GetFramePaintManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves ribbon paint manager.
	// Returns:
	//     Pointer to CXTPRibbonPaintManager object.
	//-----------------------------------------------------------------------
	virtual CXTPRibbonPaintManager* GetRibbonPaintManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves markup context.
	// Returns:
	//     Pointer to CXTPMarkupContext object.
	//-----------------------------------------------------------------------
	virtual CXTPMarkupContext* GetMarkupContext();

// Status bar
public:


protected:

	CXTPResourceImages *m_pImages;
	CXTPMarkupContext  *m_pMarkupContext;
};
