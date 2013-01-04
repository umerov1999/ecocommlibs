// XTPShellSettings.h: interface for the CXTPShellSettings class.
//
// This file is a part of the XTREME CONTROLS MFC class library.
// (c)1998-2012 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
/////////////////////////////////////////////////////////////////////////////

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPSHELLSETTINGS_H__)
#define __XTPSHELLSETTINGS_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Summary:
//     CXTPShellSettings is a SHELLFLAGSTATE struct derived class. CXTPShellSettings
//     extends the SHELLFLAGSTATE struct for easy access to the shell flag
//     state settings.
//===========================================================================
class _XTP_EXT_CLASS CXTPShellSettings : public SHELLFLAGSTATE
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPShellSettings object
	//-----------------------------------------------------------------------
	CXTPShellSettings();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPShellSettings object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	~CXTPShellSettings();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function checks whether or not the Show All Files option is
	//     enabled.
	// Returns:
	//     true if enabled, otherwise returns false.
	//-----------------------------------------------------------------------
	bool ShowAllFiles();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function checks whether or not the Hide File Extensions for
	//     Known File Types option is disabled.
	// Returns:
	//     true if enabled, otherwise returns false.
	//-----------------------------------------------------------------------
	bool ShowExtensions();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function checks to see if the Display Delete Confirmation
	//     Dialog box in the Recycle Bin is enabled.
	// Returns:
	//     true if enabled, otherwise returns false.
	//-----------------------------------------------------------------------
	bool NoConfirmRecycle();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function checks whether or not the Do Not Show Hidden Files
	//     option is selected.
	// Returns:
	//     true if enabled, otherwise returns false.
	//-----------------------------------------------------------------------
	bool ShowSysFiles();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function checks whether or not the Display Compressed Files
	//     and Folders with Alternate Color option is enabled.
	// Returns:
	//     true if enabled, otherwise returns false.
	//-----------------------------------------------------------------------
	bool ShowCompColor();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function checks whether or not the Double-Click to Open an
	//     Item option is enabled.
	// Returns:
	//     true if enabled, otherwise returns false.
	//-----------------------------------------------------------------------
	bool DoubleClickOpen();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function checks whether or not the Active Desktop - View as
	//     Web Page option is enabled.
	// Returns:
	//     true if enabled, otherwise returns false.
	//-----------------------------------------------------------------------
	bool HTMLDesktop();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function checks whether or not the Classic Style option is
	//     enabled.
	// Returns:
	//     true if enabled, otherwise returns false.
	//-----------------------------------------------------------------------
	bool Win95Classic();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function checks whether or not the Allow All Uppercase Names
	//     option is enabled.
	// Returns:
	//     true if enabled, otherwise returns false.
	//-----------------------------------------------------------------------
	bool AllowUpperCase();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function checks whether or not the Show File Attributes in
	//     Detail View option is enabled.
	// Returns:
	//     true if enabled, otherwise returns false.
	//-----------------------------------------------------------------------
	bool ShowAttributes();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function checks whether or not the Show Map Network Drive
	//     Button in Toolbar option is enabled.
	// Returns:
	//     true if enabled, otherwise returns false.
	//-----------------------------------------------------------------------
	bool ShowMapNetworkDrive();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function checks whether or not the Show Info Tips for Items
	//     in Folders & Desktop option is enabled.
	// Returns:
	//     true if enabled, otherwise returns false.
	//-----------------------------------------------------------------------
	bool ShowInfoTips();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to update the SHELLFLAGSTATE values for
	//     this object.
	//-----------------------------------------------------------------------
	void RefreshSettings();

	COLORREF m_crCompColor; // RGB color value that represents the system alternate compressed color.
	COLORREF m_crEncrColor; // RGB color value that represents the system alternate encrypted color.
};

//////////////////////////////////////////////////////////////////////

AFX_INLINE bool CXTPShellSettings::ShowAllFiles() {
	return fShowAllObjects != 0 ? true : false;
}
AFX_INLINE bool CXTPShellSettings::ShowExtensions() {
	return fShowExtensions != 0 ? true : false;
}
AFX_INLINE bool CXTPShellSettings::NoConfirmRecycle() {
	return fNoConfirmRecycle != 0 ? true : false;
}
AFX_INLINE bool CXTPShellSettings::ShowSysFiles() {
	return fShowSysFiles != 0 ? true : false;
}
AFX_INLINE bool CXTPShellSettings::ShowCompColor() {
	return fShowCompColor != 0 ? true : false;
}
AFX_INLINE bool CXTPShellSettings::DoubleClickOpen() {
	return fDoubleClickInWebView != 0 ? true : false;
}
AFX_INLINE bool CXTPShellSettings::HTMLDesktop() {
	return fDesktopHTML != 0 ? true : false;
}
AFX_INLINE bool CXTPShellSettings::Win95Classic() {
	return fWin95Classic != 0 ? true : false;
}
AFX_INLINE bool CXTPShellSettings::AllowUpperCase() {
	return fDontPrettyPath != 0 ? true : false;
}
AFX_INLINE bool CXTPShellSettings::ShowAttributes() {
	return fShowAttribCol != 0 ? true : false;
}
AFX_INLINE bool CXTPShellSettings::ShowMapNetworkDrive() {
	return fMapNetDrvBtn != 0 ? true : false;
}
AFX_INLINE bool CXTPShellSettings::ShowInfoTips() {
	return fShowInfoTip != 0 ? true : false;
}

#endif // !defined(__XTPSHELLSETTINGS_H__)
