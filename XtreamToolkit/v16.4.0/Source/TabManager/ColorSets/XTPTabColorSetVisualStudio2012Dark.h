class _XTP_EXT_CLASS CXTPTabColorSetVisualStudio2012Dark : public CXTPTabColorSetVisualStudio2012
{
public:
	CXTPTabColorSetVisualStudio2012Dark();

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
