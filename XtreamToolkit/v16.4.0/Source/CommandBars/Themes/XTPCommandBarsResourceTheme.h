// XTPResourceTheme.h : interface for the CXTPResourceTheme class.
//
// This file is a part of the XTREME COMMANDBARS MFC class library.
// (c)1998-2013 Codejock Software, All Rights Reserved.
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
#if !defined(__XTPRESOURCETHEME_H__)
#define __XTPRESOURCETHEME_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPResourceImage;
class CXTPResourceImages;
class CXTPCommandBarsFrameHook;
class CXTPMarkupContext;


//===========================================================================
// Summary:
//     The CXTPResourceTheme class is used to enable an Office 2007 style theme for Command Bars
// See Also: CXTPPaintManager::SetTheme
//===========================================================================
class _XTP_EXT_CLASS CXTPResourceTheme : public CXTPCommandBarsOffice2003Theme
{
	DECLARE_DYNAMIC(CXTPResourceTheme)
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPResourceTheme object.
	//-----------------------------------------------------------------------
	CXTPResourceTheme();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPResourceTheme object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPResourceTheme();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Loads a bitmap image from the specified path.
	// Parameters:
	//     lpszFileName - The name of the file that contains the bitmaps
	//                    to load.
	// Remarks:
	//     The images are the bitmaps that represent all the visual components
	//     of the Ribbon Bar.  For example tab buttons, group buttons, menu buttons,
	//     toolbar buttons, option button, toolbar dropdown, etc.
	//     The images are loaded using LoadImage and are stored in the m_pImages
	//     image collection.
	//     Images for the Office 2007 theme can be found in the \Source\Ribbon\Res
	//     folder.
	// Returns: New CXTPResourceImage containing the loaded bitmap.
	// See Also: SetImageHandle, m_pImages
	//-----------------------------------------------------------------------
	CXTPResourceImage* LoadImage(LPCTSTR lpszFileName);

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies the resource that contains the bitmaps to be loaded for the
	//     theme.  This allows images to be loaded from something other than bitmap files.
	// Parameters:
	//     hResource - Handle to a visual style dll file handle. Or
	//                 some other resource that contains the images
	//                 to load for the theme.
	//     lpszIniFileName - String identifier of ini file with color specifications
	// Remarks:
	//     The images are the bitmaps that represent all the visual components
	//     of the Ribbon Bar.  For example tab buttons, group buttons, menu buttons,
	//     toolbar buttons, option button, toolbar dropdown, etc.
	//     The images are loaded using LoadImage and are stored in the m_pImages
	//     image collection.
	//     Images for the Office 2007 theme can be found in the \Source\Ribbon\Res
	//     folder.
	// Example:
	//     <code>((CXTPResourceTheme*)XTPPaintManager())->SetImageHandle(XTPSkinManager()->GetResourceFile()->GetModuleHandle());</code>
	// See Also: LoadImage, m_pImages
	//-----------------------------------------------------------------------
	void SetImageHandle(HMODULE hResource, LPCTSTR lpszIniFileName);

	virtual BOOL RequiresResourceImages();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the manager.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the base theme of the manager.
	// Returns:
	//     Returns the base theme.
	//-----------------------------------------------------------------------
	virtual XTPPaintTheme BaseTheme() { return xtpThemeResource; }

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to get CXTPResourceImages pointer
	// Returns:
	//     Pointer to image collection of the class.
	//-----------------------------------------------------------------------
	CXTPResourceImages* GetImages() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set images for theme
	// Parameters:
	//     pImages - new CXTPResourceImages to bet set
	// See Also: GetImages
	//-----------------------------------------------------------------------
	void SetImages(CXTPResourceImages* pImages);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method retrieves buttons text color.
	// Parameters:
	//     bSelected   - TRUE if the control is selected.
	//     bPressed    - TRUE if the control is pushed.
	//     bEnabled    - TRUE if the control is enabled.
	//     bChecked    - TRUE if the control is checked.
	//     bPopuped    - TRUE if the control is popuped.
	//     barType     - Parent's bar type
	//     barPosition - Parent's bar position.
	// Returns:
	//     Buttons text color.
	//-----------------------------------------------------------------------
	virtual COLORREF GetRectangleTextColor(BOOL bSelected, BOOL bPressed, BOOL bEnabled, BOOL bChecked, BOOL bPopuped, XTPBarType barType, XTPBarPosition barPosition);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws workspace space
	// Parameters:
	//     pDC - Pointer to device context
	//     rc - Bounding rectangle
	//     rcExclude - Excluded rectangle
	//-----------------------------------------------------------------------
	virtual void FillWorkspace(CDC* pDC, CRect rc, CRect rcExclude);

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

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if images is available
	// Returns:
	//     TRUE if images exist in application
	//-----------------------------------------------------------------------
	BOOL IsImagesAvailable();


protected:
	int m_nRibbonCaptionHeight;         // Ribbon caption height


protected:
//{{AFX_CODEJOCK_PRIVATE
	CXTPResourceImages* m_pImages;
	COLORREF m_clrMenuBarText;
	COLORREF m_clrStatusBarShadow;
	CXTPPaintManagerColorGradient m_clrStatusBarTop;
	CXTPPaintManagerColorGradient m_clrStatusBarBottom;
	COLORREF m_clrWorkspaceClientTop;
	COLORREF m_clrWorkspaceClientMiddle;
	COLORREF m_clrWorkspaceClientBottom;
	CXTPPaintManagerColorGradient m_clrMessageBar;
	COLORREF m_clrMessageBarFrame;
	COLORREF m_clrMessageBarFace;
//}}AFX_CODEJOCK_PRIVATE

	friend class CXTPStatusBarThemeResource;
	friend class CXTPMessageBarThemeResource;
};

AFX_INLINE BOOL CXTPResourceTheme::RequiresResourceImages()
{
	return TRUE;
}

// Obsolete class
#define CXTPOffice2007Theme CXTPResourceTheme


#endif // #if !defined(__XTPRESOURCETHEME_H__)
