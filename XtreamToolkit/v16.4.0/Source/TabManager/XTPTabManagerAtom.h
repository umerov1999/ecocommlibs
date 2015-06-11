//===========================================================================
// Summary:
//     CXTPTabManagerAtom is a class that has only one purpose, which
//     is to catch the event when a property of the PaintManager has changed.
//     When a property of the PaintManager has changed, the OnPropertyChanged
//     event if called. When the OnPropertyChanged is caught, Reposition
//     is called to recalculate self layout.
// Remarks:
//     This is the base class for CXTPTabManager.  When a property of the
//     CXTPTabPaintManager has changed, then OnPropertyChanged is called
//     to recalculate the layout.  The paint manager can be accessed
//     with the CXTPTabManager::GetPaintManager member.
// See Also: CXTPTabManager::GetPaintManager
//===========================================================================
class CXTPTabManagerAtom
{
public:

	//-------------------------------------------------------------------------
	// Summary:
	//     This member is called when a property of the PaintManager has
	//     changed.
	// See Also: CXTPTabManager::GetPaintManager
	//-------------------------------------------------------------------------
	virtual void OnPropertyChanged()
	{
		Reposition();
	}

	//-------------------------------------------------------------------------
	// Summary:
	//     This member recalculates the layout of the tab manager and
	//     then repositions itself.  This member must be overridden in
	//     derived classes.
	//-------------------------------------------------------------------------
	virtual void Reposition() = 0;
protected:

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPTabManagerAtom object, handles cleanup and deallocation.
	//-------------------------------------------------------------------------
	virtual ~CXTPTabManagerAtom()
	{

	}
};
