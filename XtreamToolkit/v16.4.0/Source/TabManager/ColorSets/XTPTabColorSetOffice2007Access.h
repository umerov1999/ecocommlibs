//===========================================================================
// Summary:
//     CXTPTabColorSetOffice2007Access is a CXTPTabColorSetOffice2007 derived class that represents the
//     Office 2007 tab color set.
//===========================================================================
class _XTP_EXT_CLASS CXTPTabColorSetOffice2007Access : public CXTPTabColorSetOffice2007
{
public:
	//-------------------------------------------------------------------------
	// Summary:
	//     This member is called to refresh the visual metrics of the tabs.
	// Remarks:
	//     All of the color members are refreshed when this is called.
	//     This member can be override this member to change the colors of
	//     the color members.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();
};

//{{AFX_CODEJOCK_PRIVATE
// obsolete, for backward compatibility only.
class _XTP_EXT_CLASS CColorSetAccess2007 : public CXTPTabColorSetOffice2007Access {};
//}}AFX_CODEJOCK_PRIVATE
