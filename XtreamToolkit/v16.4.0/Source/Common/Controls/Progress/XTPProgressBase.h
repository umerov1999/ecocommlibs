class CXTPProgressPaintManager;

//===========================================================================
// Summary:
//     CXTPControlProgressBase is base class for all progress objects
//===========================================================================
class _XTP_EXT_CLASS CXTPProgressBase
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPProgressBase object
	//-----------------------------------------------------------------------
	CXTPProgressBase();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPProgressBase object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPProgressBase();

public:

	//----------------------------------------------------------------------
	// Summary:
	//     Sets the upper and lower limits of the progress bar control's range.
	// Parameters:
	//     nLower - Lower limit of the progress bar control's range.
	//     nUpper - Upper limit of the progress bar control's range.
	//----------------------------------------------------------------------
	void SetRange(int nLower, int nUpper);

	//----------------------------------------------------------------------
	// Summary:
	//     Gets the upper and lower limits of the progress bar control's range.
	// Parameters:
	//     nLower - Lower limit of the progress bar control's range.
	//     nUpper - Upper limit of the progress bar control's range.
	//----------------------------------------------------------------------
	void GetRange(int& nLower, int& nUpper) const;

	//----------------------------------------------------------------------
	// Summary:
	//     Gets the current position of the progress bar, whose return value
	//     is always between the values for the Max and Min properties, inclusive.
	// Returns:
	//     Current position of the progress bar.
	//----------------------------------------------------------------------
	int GetPos() const;

	//----------------------------------------------------------------------
	// Summary:
	//     Sets the current position of the progress bar, whose return value
	//     is always between the values for the Max and Min properties, inclusive.
	// Parameters:
	//     nPos - Position to move progress bar.
	//----------------------------------------------------------------------
	void SetPos(int nPos);

public:
	//----------------------------------------------------------------------
	// Summary:
	//     This method is called to get bounding rectangle of progressbar
	//----------------------------------------------------------------------
	virtual CRect GetProgressRect() = 0;

protected:
	//----------------------------------------------------------------------
	// Summary:
	//     This method is called to redraw progress bar
	//----------------------------------------------------------------------
	virtual void RedrawProgress() = 0;

	//----------------------------------------------------------------------
	// Summary:
	//     This method is called to retrieve pointer to CXTPProgressPaintManager
	//----------------------------------------------------------------------
	virtual CXTPProgressPaintManager* GetProgressPaintManager() const = 0;

protected:
	int m_nMin;     // Minimum
	int m_nMax;     // Maximum
	int m_nPos;     // Current position
};
