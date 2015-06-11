// XTPSkinManager.h: interface for the CXTPSkinManager class.
//
// This file is a part of the XTREME SKINFRAMEWORK MFC class library.
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
#if !defined(__XTPSKINMANAGER_H__)
#define __XTPSKINMANAGER_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CXTPSkinManagerSchemaProperty;
class CXTPSkinManagerResourceFile;
class CXTPSkinManagerColorFilter;
class CXTPSkinObjectClassMap;
class CXTPSkinManagerApiHook;
typedef HANDLE HTHEME;

//-----------------------------------------------------------------------
// Summary:
//     Apply options of selected skin
// Example:
// <code>
// XTPSkinManager()->SetApplyOptions(xtpSkinApplyMetrics | xtpSkinApplyFrame | xtpSkinApplyColors);
// </code>
// See Also:
//     CXTPSkinManager, CXTPSkinManager::SetApplyOptions
//
// <KEYWORDS xtpSkinApplyMetrics, xtpSkinApplyFrame, xtpSkinApplyColors>
//-----------------------------------------------------------------------
enum XTPSkinApplyOptions
{
	xtpSkinApplyMetrics     = 1,    // TRUE to apply frame metrics (Caption height/font, scrollbars width/with, etc)
	xtpSkinApplyFrame       = 2,    // TRUE to apply frame skins.
	xtpSkinApplyColors      = 4,    // TRUE to use skin colors.
	xtpSkinApplyMenus       = 8     // TRUE to apply skin for Popup Menus.
};


//-----------------------------------------------------------------------
// Summary:
//     Available property for CXTPSkinManagerSchemaProperty class
// Example:
// <code>
// XTPSkinManager()->GetProperty(XTP_SKINPROPERTY_BOOL, iPartId, iStateId, TMT_TRANSPARENT);
// </code>
// See Also:
//     CXTPSkinManagerSchemaProperty
//-----------------------------------------------------------------------
enum XTPSkinManagerProperty
{
	XTP_SKINPROPERTY_UNKNOWN,       // Not defined
	XTP_SKINPROPERTY_STRING,        // CString property
	XTP_SKINPROPERTY_INT,           // int property
	XTP_SKINPROPERTY_BOOL,          // BOOL property
	XTP_SKINPROPERTY_COLOR,         // COLORREF property
	XTP_SKINPROPERTY_RECT,          // CRect property
	XTP_SKINPROPERTY_FONT,          // LOGFONT property
	XTP_SKINPROPERTY_INTLIST,       // CIntArray property
	XTP_SKINPROPERTY_ENUM,          // enum property
	XTP_SKINPROPERTY_POSITION,      // int property
	XTP_SKINPROPERTY_FILENAME = XTP_SKINPROPERTY_STRING,    // FileName property
	XTP_SKINPROPERTY_MARGINS = XTP_SKINPROPERTY_RECT,       // Margins property
	XTP_SKINPROPERTY_SIZE = XTP_SKINPROPERTY_INT            // Size property
};



class CXTPSkinManager;
class CXTPSkinManagerSchema;
class CXTPSkinImages;
class CXTPSkinObject;

//===========================================================================
// Summary:
//     CXTPSkinManagerClass is a standalone class that represents a
//     single visual class of skin manager.
//===========================================================================
class _XTP_EXT_CLASS CXTPSkinManagerClass : public CXTPSynchronized
{
protected:
	// ---------------------------------------------
	// Summary:
	//     Constructs a CXTPSkinManagerClass object.
	// Parameters:
	//     pSchema :  Parent skin manager class.
	//
	// ---------------------------------------------
	CXTPSkinManagerClass(CXTPSkinManagerSchema* pSchema);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPSkinManagerClass object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPSkinManagerClass();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Get the value for the specified string property
	// Parameters:
	//     iPartId     - Part number
	//     iStateId    - State number of part
	//     iPropId     - The property number to get the value for
	//     lpszDefault - Default value
	// Returns:
	//     CString value for the specified string property
	//-----------------------------------------------------------------------
	CString GetThemeString(int iPartId, int iStateId, int iPropId, LPCTSTR lpszDefault = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Get the value for the specified RECT property
	// Parameters:
	//     iPartId    - Part number
	//     iStateId   - State number of part
	//     iPropId    - The property number to get the value for
	//     rcDefault  - Default value
	// Returns:
	//     CRect value for the specified RECT property
	//-----------------------------------------------------------------------
	CRect GetThemeRect(int iPartId, int iStateId, int iPropId, CRect rcDefault = CRect(0, 0, 0, 0));

	//-----------------------------------------------------------------------
	// Summary:
	//     Get the value for the specified int property
	// Parameters:
	//     iPartId    - Part number
	//     iStateId   - State number of part
	//     iPropId    - The property number to get the value for
	//     nDefault   - Default value
	// Returns:
	//     int value for the specified property
	//-----------------------------------------------------------------------
	int GetThemeInt(int iPartId, int iStateId, int iPropId, int nDefault = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     Get the value for the specified Size property
	// Parameters:
	//     iPartId    - Part number
	//     iStateId   - State number of part
	//     iPropId    - The property number to get the value for
	//     szDefault  - Default value
	// Returns:
	//     CSize value for the specified property
	//-----------------------------------------------------------------------
	CSize GetThemeSize(int iPartId, int iStateId, int iPropId, CSize szDefault = CSize(0));

	//-----------------------------------------------------------------------
	// Summary:
	//     Get the value for the specified BOOL property
	// Parameters:
	//     iPartId    - Part number
	//     iStateId   - State number of part
	//     iPropId    - The property number to get the value for
	//     bDefault  - Default value
	// Returns:
	//     BOOL value for the specified property
	//-----------------------------------------------------------------------
	BOOL GetThemeBool(int iPartId, int iStateId, int iPropId, BOOL bDefault = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Get the value for the specified color property
	// Parameters:
	//     iPartId    - Part number
	//     iStateId   - State number of part
	//     iPropId    - The property number to get the value for
	//     clrDefault - Default value
	// Returns:
	//     COLORREF value for the specified property
	//-----------------------------------------------------------------------
	COLORREF GetThemeColor(int iPartId, int iStateId, int iPropId, COLORREF clrDefault = (COLORREF)-1);

	//-----------------------------------------------------------------------
	// Summary:
	//     Get the value for the specified ENUM property
	// Parameters:
	//     iPartId    - Part number
	//     iStateId   - State number of part
	//     iPropId    - The property number to get the value for
	//     nDefault - Default value
	// Returns:
	//     int value for the specified property
	//-----------------------------------------------------------------------
	int GetThemeEnumValue(int iPartId, int iStateId, int iPropId, int nDefault = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the specified size of the theme part
	// Parameters:
	//     iPartId    - Part number to retrieve size for
	//     iStateId   - State number of part
	//     pRect      - rect for part drawing destination
	//     eSize      - the type of size to be retrieved
	//     pSize      - receives the specified size of the part
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL GetThemePartSize(int iPartId, int iStateId, RECT *pRect, int eSize, SIZE* pSize);


public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the background image defined by the visual style for the specified control part.
	// Parameters:
	//     pDC      - Pointer to valid device context
	//     iPartId  - Value of type int that specifies the part to draw
	//     iStateId - Value of type int that specifies the state of the part to draw.
	//     pRect    - Pointer to a RECT structure that contains the rectangle,
	//                in logical coordinates, in which the background image is drawn.
	// Returns:
	//     Returns TRUE if successful, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL DrawThemeBackground(CDC* pDC, int iPartId, int iStateId, const RECT *pRect);


	//-----------------------------------------------------------------------
	// Summary:
	//     Draws text using the color and font defined by the visual style.
	// Parameters:
	//     pDC      - Pointer to valid device context
	//     iPartId  - Value of type int that specifies the part that contains the text.
	//     iStateId - Value of type int that specifies the state of the part to draw.
	//     strText  - String that contains the text to draw.
	//     dwFormat - DWORD that contains one or more values that specify the string's formatting.
	//     pRect    - Pointer to a RECT structure that contains the rectangle,
	//                in logical coordinates, in which the text is to be drawn.
	// Returns:
	//     Returns TRUE if successful, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL DrawThemeText(CDC* pDC, int iPartId, int iStateId, const CString& strText, DWORD dwFormat, const RECT *pRect);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves image collection of the class
	// Returns:
	//     Pointer to image collection of the class.
	//-----------------------------------------------------------------------
	CXTPSkinImages* GetImages() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves hash value of class name
	// Returns:
	//     Hash value of class name
	//-----------------------------------------------------------------------
	int GetClassCode() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves class name
	// Returns:
	//     CString value contained class name
	//-----------------------------------------------------------------------
	CString GetClassName() const;

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns property value for specified part and state
	// Parameters:
	//     propType   - Property type
	//     iPartId    - Part number to retrieve size for
	//     iStateId   - State number of part
	//     iPropId    - The property number to get the value for
	// Returns:
	//     Returns CXTPSkinManagerSchemaProperty class contained value of the property
	// See Also:
	//     XTPSkinManagerProperty, CXTPSkinManagerSchemaProperty
	//-----------------------------------------------------------------------
	CXTPSkinManagerSchemaProperty* GetProperty(XTPSkinManagerProperty propType, int iPartId, int iStateId, int iPropId);

protected:
	CXTPSkinManagerSchema* m_pSchema;    // Parent CXTPSkinManager object.

	CString m_strClass;             // Class name
	UINT m_nClassCode;              // Hashed value of class name

	CXTPSkinImages* m_pImages;      // Image collection
	CMap<UINT, UINT, CXTPSkinManagerSchemaProperty*, CXTPSkinManagerSchemaProperty*> m_mapCachedProperties; // Cached collection of class properties

	friend class CXTPSkinManager;
	friend class CXTPSkinManagerSchema;
};

#define XTP_SKINMETRICS_COLORTABLESIZE 31

//===========================================================================
// Summary:
//     Implement this interface in order to provide custom configured 
//     window filter functionality. The filtered out windows will not be skinned.
//     If you find yourself frequently filtering out some system window in order
//     to prevent application crashes, please report us its class name and
//     ideally the contents of WNDCLASS structure filled out by a call to
//     GetWindowClass for this window handle.
//     Use CXTPSkinManager::SetWindowFilter for setting or disabling a
//     custom provided window filter implementation.
//===========================================================================
struct IXTPSkinManagerWindowFilter
{
	//-----------------------------------------------------------------------
	// Summary:
	//     The implementation has to make a decision on whether a window
	//     has to be filtered out (not skinned) or not depending on certain window
	//     characteristics.
	// Parameters:
	//     hWnd - Window handle.
	//     lpszClassName - Window class name.
	//     lpcs - Pointer to window's CREATUSTRUCT structure.
	// Returns:
	//     TRUE for filtering a window out, otherwise return FALSE (default).
	//-----------------------------------------------------------------------
	virtual BOOL FilterWindow(HWND hWnd, LPCTSTR lpszClassName, LPCREATESTRUCT lpcs) = 0;
};

//===========================================================================
// Summary:
//     CXTPSkinManagerMetrics is a standalone class that represents a
//     collection of metrics of the current skin
//===========================================================================
class _XTP_EXT_CLASS CXTPSkinManagerMetrics : public CXTPSynchronized
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPSkinManagerMetrics object.
	// Parameters:
	//     pSchema - Parent skin manager class.
	//-----------------------------------------------------------------------
	CXTPSkinManagerMetrics(CXTPSkinManagerSchema* pSchema);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPSkinManagerMetrics object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPSkinManagerMetrics();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh visual metrics.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary: Determines if handle belongs to metrics
	// Input:   hObject - Handle to test
	//-----------------------------------------------------------------------
	BOOL IsMetricObject(HGDIOBJ hObject) const;

	//-------------------------------------------------------------------------
	// Summary: Destroys all GDI objects of metrics
	//-------------------------------------------------------------------------
	void DestroyMetrics();

	//-----------------------------------------------------------------------
	// Summary: Call this method to get skinned color
	// Input:   nIndex - Index of color to retrieve
	// Returns: Skinned color for specified index
	//-----------------------------------------------------------------------
	COLORREF GetColor(int nIndex) const;

private:
	void DeleteSysBrush(HBRUSH* pBrush);

public:
	CFont m_fntCaption;             // Caption font
	CFont m_fntMenu;                // Menu font
	CFont m_fntSmCaption;           // Small caption font
	CXTPSkinManagerSchema* m_pSchema;     // Parent CXTPSkinManager object

	HBRUSH m_brushDialog;           // Dialog brush
	HBRUSH m_brushTabControl;       // Tab Control brush;
	HBRUSH m_brushWindow;           // Window brush;

	COLORREF m_clrEdgeLight;        // Light edge color
	COLORREF m_clrEdgeHighLight;    // HighLight edge color
	COLORREF m_clrEdgeShadow;       // Shadow edge color
	COLORREF m_clrEdgeDkShadow;     // Dark shadow edge color

	COLORREF m_clrTheme[XTP_SKINMETRICS_COLORTABLESIZE];        // Colors of the theme
	HBRUSH m_brTheme[XTP_SKINMETRICS_COLORTABLESIZE];           // Brushes of the theme

	int m_nBorderSize;              // Border size
	int m_cxBorder;                 // SM_CXBORDER system metric
	int m_cyBorder;                 // SM_CYBORDER system metric
	int m_cxEdge;                   // SM_CXEDGE system metric
	int m_cyEdge;                   // SM_CYEDGE system metric

	int m_cxHScroll;                // SM_CXHSCROLL system metric
	int m_cyHScroll;                // SM_CYHSCROLL system metric
	int m_cxVScroll;                // SM_CXVSCROLL system metric
	int m_cyVScroll;                // SM_CYVSCROLL system metric

	int m_cyCaption;                // Height of captions
	int m_cySmallCaption;           // Height of small captions

	int m_cyOsCaption;              // System caption height
	int m_cyOsSmallCaption;         // System small caption height
	int m_cxOsVScroll;              // System SM_CXVSCROLL metric

	BOOL m_bRefreshMetrics;         // TRUE if refresh metrics currently executed
};

//===========================================================================
// Summary:
//     CXTPSkinManager is a standalone class that represents a
//     global object managing skins.
//===========================================================================
class _XTP_EXT_CLASS CXTPSkinManager : public CXTPSynchronized
{
private:
	DECLARE_HANDLE(HWINEVENTHOOK);

	typedef void (CALLBACK* WINEVENTPROC) ( HWINEVENTHOOK hWinEventHook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD idEventThread, DWORD dwmsEventTime);
	typedef HWINEVENTHOOK (WINAPI* LPFNSETWINEVENTHOOK)(UINT eventMin, UINT eventMax, HMODULE hmodWinEventProc,
		WINEVENTPROC lpfnWinEventProc, DWORD idProcess, DWORD idThread, UINT dwflags);
	typedef BOOL (WINAPI* LPFNUNHOOKWINEVENT)(HWINEVENTHOOK hWinEventHook);


protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPSkinManager object.
	//-----------------------------------------------------------------------
	CXTPSkinManager();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPSkinManager object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPSkinManager();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Loads skin from cjstyles or msstyles file.
	// Parameters:
	//     lpszResourcePath - Path to skin file
	//     lpszIniFileName - Ini file of skin to load
	// Returns:
	//     TRUE if successful; otherwise returns FALSE
	//-----------------------------------------------------------------------
	BOOL LoadSkin(LPCTSTR lpszResourcePath, LPCTSTR lpszIniFileName = NULL);

	CXTPSkinManagerSchema* CreateSchema(LPCTSTR lpszResourcePath, LPCTSTR lpszIniFileName = NULL);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves what parts of application skin must apply.
	// Returns:
	//     One or more of XTPSkinApplyOptions values
	// See Also: XTPSkinApplyOptions, SetApplyOptions
	//-----------------------------------------------------------------------
	DWORD GetApplyOptions() const;

	BOOL HasApplyOptions(DWORD dwOptions) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set what parts of application skin must apply.
	// Parameters:
	//     dwOptions - combined by using the bitwise OR (|) operator of following:
	//         * <b>xtpSkinApplyMetrics</b> To apply skin metrics (caption height, scroll bar with, etc)
	//         * <b>xtpSkinApplyFrame</b> To apply window frames.
	// See Also: XTPSkinApplyOptions, GetApplyOptions
	//-----------------------------------------------------------------------
	void SetApplyOptions(DWORD dwOptions);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to manually set window which appearance must be skinned.
	// Parameters:
	//     hWnd - Handle of window which appearance must be skinned.
	//-----------------------------------------------------------------------
	void ApplyWindow(HWND hWnd);
	void ApplyWindow(HWND hWnd, BOOL bApplyChilds);

	void SetWindowTheme(HWND hWnd, CXTPSkinManagerSchema* pSchema);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to enable/disable automatically apply each new window with current skin
	// Parameters:
	//     bAutoApply - TRUE to automatically apply each new window.
	//-----------------------------------------------------------------------
	void SetAutoApplyNewWindows(BOOL bAutoApply);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if all new windows will be automatically skinned
	//-----------------------------------------------------------------------
	BOOL GetAutoApplyNewWindows() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set custom skin manager class
	// Parameters:
	//     pSkinManager - new skin manager to be set
	//     bDelete - TRUE to delete the existing skin manager.
	// Returns:
	//     A pointer to the previous CXTPSkinManager object if bDelete is
	//     set to FALSE, otherwise returns NULL.
	// Example:
	// <code>CXTPSkinManager::SetSkinManager(new CMySkinManager());</code>
	//-----------------------------------------------------------------------
	static CXTPSkinManager* AFX_CDECL SetSkinManager(CXTPSkinManager* pSkinManager, BOOL bDelete = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Exclude some dll from API hooking
	// Parameters:
	//     lpszModule - File name to exclude
	//-----------------------------------------------------------------------
	void ExcludeModule(LPCTSTR lpszModule);

public:
	// -------------------------------------------------------------------
	// Summary:
	//     Retrieves skin class by its name
	// Parameters:
	//     strClassList :  Name of the class
	//     pObject :       Reference to the skin object to apply the skin.
	// Returns:
	//     Pointer to CXTPSkinManagerClass class object.
	// -------------------------------------------------------------------
	virtual CXTPSkinManagerClass* GetSkinClass(const CXTPSkinObject* pObject, CString strClassList);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to find CXTPSkinObject by its handle
	// Parameters:
	//     hWnd - Handle of skinned window
	// Returns:
	//     Pointer to CXTPSkinObject that used to draw the window
	//-----------------------------------------------------------------------
	CXTPSkinObject* Lookup(HWND hWnd);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get pointer to skin metrics
	// Returns:
	//     Pointer to CXTPSkinManagerMetrics class contained metrics of the skin
	//-----------------------------------------------------------------------
	CXTPSkinManagerMetrics* GetMetrics() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if skinning is currently enabled.
	// Returns:
	//     TRUE if skinning is enabled.
	//-----------------------------------------------------------------------
	BOOL IsEnabled() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get pointer to skin schema
	// Returns:
	//     Pointer to CXTPSkinManagerSchema class used to draw window parts.
	//-----------------------------------------------------------------------
	CXTPSkinManagerSchema* GetSchema() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get pointer to resource file
	// Returns:
	//     Pointer to CXTPSkinManagerResourceFile that managed resources of skins.
	// See Also:
	//     SetResourceFile
	//-----------------------------------------------------------------------
	CXTPSkinManagerResourceFile* GetResourceFile() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set custom resource file
	// Parameters:
	//     pResourceFile - new resource file
	// See Also:
	//     GetResourceFile
	// Example:
	//     <code>XTPSkinManager()->SetResourceFile(new CMySkinManagerResourceFile);</code>
	//-----------------------------------------------------------------------
	void SetResourceFile(CXTPSkinManagerResourceFile* pResourceFile);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables dialog background theme.  This method can be used to
	//     tailor dialog compatibility with child windows and controls that
	//     may or may not coordinate the rendering of their client area backgrounds
	//     with that of their parent dialog in a manner that supports seamless
	//     background texturing.
	// Parameters:
	//     hWnd    - The window handle of the target dialog
	//     dwFlags - One of the following
	///              * ETDT_ENABLE to enable the theme-defined dialog background texturing,
	//               * ETDT_DISABLE to disable background texturing,
	//               * ETDT_ENABLETAB to enable the theme-defined background
	//                                texturing using the Tab texture
	// Returns:
	//     S_OK if successful
	//-----------------------------------------------------------------------
	HRESULT EnableThemeDialogTexture(HWND hWnd, DWORD dwFlags);

	//-----------------------------------------------------------------------
	// Summary:
	//     Get the value of the specified System color.
	// Parameters:
	//     iColorId -  the system color index defined in winuser.h
	// Returns:
	//     COLORREF value if specified system color
	//-----------------------------------------------------------------------
	COLORREF GetThemeSysColor(int iColorId);

	//-----------------------------------------------------------------------
	// Summary:
	//     Get the LOGFONT for the specified System font.
	// Parameters:
	//     iFontId - The TMT_XXX font number (first font
	//               is TMT_CAPTIONFONT)
	//     plf     - Pointer to LOGFONT to receive the font value.
	//               (scaled for the current logical screen dpi)
	// Returns:
	//     S_OK if successful
	//-----------------------------------------------------------------------
	HRESULT GetThemeSysFont(int iFontId, LOGFONT *plf);

	//-----------------------------------------------------------------------
	// Summary:
	//     Get the value of the specified System size metric.
	//     (scaled for the current logical screen dpi)
	// Parameters:
	//     iSizeId  - The TMT_XXX size value.
	// Returns:
	//     Size value of the property
	//-----------------------------------------------------------------------
	int GetThemeSysSize(int iSizeId);

	//-----------------------------------------------------------------------
	// Summary:
	//     Get the boolean value of specified System metric.
	// Parameters:
	//     iBoolId - the TMT_XXX BOOL number (first BOOL
	//               is TMT_FLATMENUS)
	// Returns:
	//     BOOL value of specified System metric
	//-----------------------------------------------------------------------
	BOOL GetThemeSysBool(int iBoolId);

	//-----------------------------------------------------------------------
	// Summary:
	//     Get the name of the current theme in-use.
	// Parameters:
	//     strThemeFileName - receives the theme path & filename
	//     strColorBuff     - receives the canonical color scheme name
	//-----------------------------------------------------------------------
	void GetCurrentThemeName(CString& strThemeFileName, CString& strColorBuff);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns CXTPSkinManagerClass class from specified HTHEME handle
	// Parameters:
	//     hTheme - Theme identifier
	// Returns:
	//     CXTPSkinManagerClass class if found.
	//-----------------------------------------------------------------------
	CXTPSkinManagerClass* FromHandle(HTHEME hTheme);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines of ComCtl32.dll version 6.0 found
	// Returns:
	//     TRUE if ComCtl32.dll has version 6.0 and above
	// See Also: IsWin9x
	//-----------------------------------------------------------------------
	BOOL IsComCtlV6() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Call this member function to check whether or not the
	//      operating system is of the Windows 9x family, and if it is
	//      Windows 95 or a later version.
	// Returns:
	//      TRUE if the OS is of the Windows 9x family, and is Windows 95
	///     or a later version, otherwise returns false.
	// See Also: IsComCtlV6
	//-----------------------------------------------------------------------
	BOOL IsWin9x() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to redraw all controls currently skinned
	//-----------------------------------------------------------------------
	void RedrawAllControls();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method for each thread in multi threaded application to enable skin framework for all threads
	//-----------------------------------------------------------------------
	void EnableCurrentThread();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to enable/disable automatically apply each new thread with current skin
	// Parameters:
	//     bAutoApply - TRUE to automatically apply each new thread.
	//-----------------------------------------------------------------------
	void SetAutoApplyNewThreads(BOOL bAutoApply);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if all new threads will be automatically skinned
	//-----------------------------------------------------------------------
	BOOL GetAutoApplyNewThreads() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves class map object that porpoise to map window classes to skin framework classes
	//-----------------------------------------------------------------------
	CXTPSkinObjectClassMap* GetClassMap() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Removed hook for specified window
	// Parameters:
	//     hWnd - Window handle to remove
	// See Also:
	//     RemoveAll
	//-----------------------------------------------------------------------
	void Remove(HWND hWnd);

	//-------------------------------------------------------------------------
	// Summary:
	//     Removes all hooks
	// Parameters:
	//     bDetach - TRUE to detach hooks before remove.
	// See Also:
	//     Remove
	//-------------------------------------------------------------------------
	void RemoveAll(BOOL bDetach = TRUE);

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Determines if any color filter installed
	//-------------------------------------------------------------------------
	BOOL IsColorFilterExists() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called internally to transform color using installed color filters
	// Parameters:
	//     clr - Color to transform
	//-------------------------------------------------------------------------
	void ApplyColorFilter(COLORREF& clr);

	//-------------------------------------------------------------------------
	// Summary:
	//     Adds new color filters
	// Parameters:
	//     pFilter - New filter to add
	//-------------------------------------------------------------------------
	void AddColorFilter(CXTPSkinManagerColorFilter* pFilter);

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys all added color filters
	//-------------------------------------------------------------------------
	void RemoveColorFilters();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when skin is changed
	// Parameters:
	//     bPrevState - TRUE if previously skin was installed
	//     bNewState - TRUE if skin is installed
	//-----------------------------------------------------------------------
	virtual void OnSkinChanged(BOOL bPrevState, BOOL bNewState);

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to parse visual styles ini file
	// Parameters:
	//     lpszResourcePath - Resource file name
	//     lpszIniFileName - Ini file name
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	// See Also: FreeSkinData
	//-----------------------------------------------------------------------
	BOOL ReadSkinData(LPCTSTR lpszResourcePath, LPCTSTR lpszIniFileName);

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called to free skin resources
	// See Also:
	//     ReadSkinData
	//-------------------------------------------------------------------------
	void FreeSkinData();

protected:

	// ----------------------------------------------------------------------
	// Summary:
	//     This method is called to create hook class for specified window
	// Parameters:
	//     hWnd :           A handle to a window that represents that
	//                      represents the window to hook.
	//     lpszClassName :  Class name of specified window
	//     lpcs :           Points to a CREATESTRUCT structure that contains
	//                      information about the CWnd object being created.
	//     bAuto :          Indicates that Window was automatically skinned
	//                      when new window was create or when user skin
	//                      manually existing window
	// Returns:
	//     New CXTPSkinObject file hooked specified window
	// See Also:
	//     CXTPSkinObject
	// ----------------------------------------------------------------------
	virtual CXTPSkinObject* SetHook(HWND hWnd, LPCTSTR lpszClassName, LPCREATESTRUCT lpcs, BOOL bAuto);

	//-------------------------------------------------------------------------
	// Summary:
	//     Sets window filter implementation. NULL disables any previously set
	//     window filter implementation.
	// See Also:
	//     IXTPSkinManagerWindowFilter
	//-------------------------------------------------------------------------
	void SetWindowFilter(IXTPSkinManagerWindowFilter* pFilter);

protected:
	static BOOL IsClassKnownIgnoredClass(LPCTSTR lpszClassName);
	static LRESULT CALLBACK HookWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK CbtFilterHook(int code, WPARAM wParam, LPARAM lParam);
	static void CALLBACK WinEventProc(HWINEVENTHOOK hWinEventHook,
		DWORD event, HWND hWnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime);
	static LRESULT CALLBACK DoCallWindowProc(WNDPROC lpPrevWndFunc, HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	static BOOL CALLBACK EnumWindowsProcNetBroadcast(HWND hwnd, LPARAM lParam);
	static BOOL AreAppVisualThemesDisabled();
	void Remove(HWND hWnd, BOOL bAuto);

	BOOL IsWindowFilteredOut(HWND hWnd, LPCTSTR lpszClassName, LPCREATESTRUCT lpcs);

protected:
	DWORD m_dwComCtrl;      // Version of ComCtl.dll
	const BOOL m_bAppVisualThemesDisabled;
	BOOL m_bWin9x;          // TRUE if OS is Win9x
	BOOL m_bEnabled;        // TRIE if skinning is enabled
	BOOL m_bAutoApplyWindows;      // TRUE to auto skin all new windows
	BOOL m_bAutoApplyThreads;      // TRUE to auto skin all new windows

	CXTPSkinManagerSchema* m_pSchema;   // Current schema
	CXTPSkinManagerResourceFile* m_pResourceFile;   // Resource file

	BOOL m_dwApplyOptions;  // Apply options.

	HWINEVENTHOOK m_hWinEventHook;      // HWINEVENTHOOK value that identifies event hook instance
	CMap<HWND, HWND, CXTPSkinObject*, CXTPSkinObject*> m_mapObjects;  // Collection of CXTPSkinObject classes
	CRITICAL_SECTION m_csObjects;            // Helper critical section object

	LPFNSETWINEVENTHOOK m_pSetWinEventHook;         // Pointer to SetWinEventHook API
	LPFNUNHOOKWINEVENT m_pUnhookWinEvent;           // Pointer to UnhookWinEvent API

	CArray<CXTPSkinManagerColorFilter*, CXTPSkinManagerColorFilter*> m_arrFilters;          // Array of filters to apply

	CXTPSkinObjectClassMap* m_pClassMap;            // Class map helper

	IXTPSkinManagerWindowFilter* m_pWindowFilter;   // Custom provided window filter implementation.

	CXTPSkinManagerApiHook* m_pApiHook;             // API hook instance.
	static CXTPSkinManager* s_pInstance;            // Instance of SkinManager

public:
	AFX_MODULE_STATE* m_pModuleState;

private:
	class CDestructor;
	friend _XTP_EXT_CLASS CXTPSkinManager* AFX_CDECL XTPSkinManager();
	friend class CXTPSkinObject;
	friend class CDestructor;
	friend class CXTPSkinManagerApiHook;
};

AFX_INLINE CXTPSkinImages* CXTPSkinManagerClass::GetImages() const {
	return m_pImages;
}
AFX_INLINE int CXTPSkinManagerClass::GetClassCode() const {
	return m_nClassCode;
}
AFX_INLINE CString CXTPSkinManagerClass::GetClassName() const {
	return m_strClass;
}

AFX_INLINE BOOL CXTPSkinManager::IsEnabled() const {
	return m_bEnabled;
}
AFX_INLINE CXTPSkinManagerResourceFile* CXTPSkinManager::GetResourceFile() const {
	return m_pResourceFile;
}
AFX_INLINE CXTPSkinManagerSchema* CXTPSkinManager::GetSchema() const {
	return m_pSchema;
}
AFX_INLINE DWORD CXTPSkinManager::GetApplyOptions() const {
	return m_dwApplyOptions;
}
AFX_INLINE BOOL CXTPSkinManager::HasApplyOptions(DWORD dwOptions) const {
	return (m_dwApplyOptions & dwOptions) == dwOptions;
}
AFX_INLINE BOOL CXTPSkinManager::IsComCtlV6() const {
	return m_dwComCtrl >= MAKELONG(0, 6);
}
AFX_INLINE BOOL CXTPSkinManager::IsWin9x() const {
	return m_bWin9x;
}
AFX_INLINE CXTPSkinObjectClassMap* CXTPSkinManager::GetClassMap() const {
	return m_pClassMap;
}
AFX_INLINE BOOL CXTPSkinManager::GetAutoApplyNewWindows() const {
	return m_bAutoApplyWindows;
}
AFX_INLINE BOOL CXTPSkinManager::GetAutoApplyNewThreads()const {
	return m_bAutoApplyThreads;
}
AFX_INLINE void CXTPSkinManager::SetAutoApplyNewWindows(BOOL bAutoApply) {
	m_bAutoApplyWindows = bAutoApply;
}
AFX_INLINE void CXTPSkinManager::SetAutoApplyNewThreads(BOOL bAutoApply) {
	m_bAutoApplyThreads = bAutoApply;
}
AFX_INLINE void CXTPSkinManager::SetWindowFilter(IXTPSkinManagerWindowFilter* pFilter) {
	m_pWindowFilter = pFilter;
}

//---------------------------------------------------------------------------
// Summary:
//     Call this function to access CXTPSkinManager members.
//     Since this class is designed as a single instance object you can
//     only access version info through this method. You <b>cannot</b>
//     directly instantiate an object of type CXTPSkinManager.
// Example:
// <code>
// XTPSkinManager()->SetApplyOptions(xtpSkinApplyMetrics | xtpSkinApplyFrame | xtpSkinApplyColors);
// </code>
//---------------------------------------------------------------------------
_XTP_EXT_CLASS CXTPSkinManager* AFX_CDECL XTPSkinManager();

//{{AFX_CODEJOCK_PRIVATE

_XTP_EXT_CLASS CXTPRWCriticalSection* AFX_CDECL XTPSkinGlobalAccess();

#ifdef _AFXDLL

class CXTPSkinScopeModuleStateHolder
{
	AFX_MODULE_STATE* m_pPrevState;

	CXTPSkinScopeModuleStateHolder(const CXTPSkinScopeModuleStateHolder&);
	CXTPSkinScopeModuleStateHolder& operator = (const CXTPSkinScopeModuleStateHolder&);

public:
	CXTPSkinScopeModuleStateHolder(AFX_MODULE_STATE* pNewState)
	{
		m_pPrevState = AfxSetModuleState(pNewState);
	}

	~CXTPSkinScopeModuleStateHolder()
	{
		AfxSetModuleState(m_pPrevState);
	}

	operator int()
	{
		return 0;
	}
};

#endif /*_AFXDLL*/

#define XTP_SKINFRAMEWORK_GLOBALLOCK_SHARED_SCOPE() \
	XTP_RWCS_LOCK_SHARED_SCOPE(*XTPSkinGlobalAccess())
#define XTP_SKINFRAMEWORK_GLOBALLOCK_EXCLUSIVE_SCOPE() \
	XTP_RWCS_LOCK_EXCLUSIVE_SCOPE(*XTPSkinGlobalAccess())

#define XTP_SKINFRAMEWORK_MANAGE_STATE() \
	AFX_MODULE_STATE* $__xtpSkinOrigModuleState = AfxGetModuleState(); \
	UNREFERENCED_PARAMETER($__xtpSkinOrigModuleState); \
	SAFE_MANAGE_STATE(XTPSkinManager()->m_pModuleState);

#ifdef _AFXDLL
#	define XTP_SKINFRAMEWORK_ORIGINAL_STATE() \
		XTP_BLOCKSTATEMENT(CXTPSkinScopeModuleStateHolder, $__xtpSkinScopeModuleStateHolder, $__xtpSkinOrigModuleState)
#else
#	define XTP_SKINFRAMEWORK_ORIGINAL_STATE()
#endif /*_AFXDLL*/

#define XTP_SKINFRAMEWORK_ASSERT_WINDOW_THREAD(hWnd) \
	ASSERT(::IsWindow(hWnd) ? GetWindowThreadProcessId(hWnd, NULL) == GetCurrentThreadId() : TRUE)

//}}AFX_CODEJOCK_PRIVATE

#endif // !defined(__XTPSKINMANAGER_H__)
