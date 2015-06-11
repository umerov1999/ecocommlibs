// XTPCommandBarsOffice2013Theme.h : interface for the CXTPCommandBarsOffice2013Theme class.
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
#if !defined(__XTPCOMMANDBARSOFFICE2013THEME_H__)
#define __XTPCOMMANDBARSOFFICE2013THEME_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPCommandBarsColorSet;
class CXTPStatusBarPaintPanager;

//===========================================================================
// Summary:
//     The CXTPCommandBarsOffice2013Theme class is used to enable an Office 2013 style theme for Command Bars
// See Also: CXTPPaintManager::SetTheme
//===========================================================================
class _XTP_EXT_CLASS CXTPCommandBarsOffice2013Theme : public CXTPCommandBarsModernUITheme
{
	DECLARE_DYNAMIC(CXTPCommandBarsOffice2013Theme)
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPCommandBarsOffice2013Theme object.
	//-----------------------------------------------------------------------
	CXTPCommandBarsOffice2013Theme();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPCommandBarsOffice2013Theme object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPCommandBarsOffice2013Theme();


// Status bar
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to fill the command bar's face.
	// Parameters:
	//     pDC  - Pointer to a valid device context
	//     pBar - Points to a CXTPCommandBar object to draw.
	//-----------------------------------------------------------------------
	virtual void FillCommandBarEntry(CDC *pDC, CXTPCommandBar *pBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to fill the control's face
	// Parameters:
	//     pDC         - Pointer to a valid device context
	//     rc          - Rectangle to draw.
	//     bSelected   - TRUE if the control is selected.
	//     bPressed    - TRUE if the control is pushed.
	//     bEnabled    - TRUE if the control is enabled.
	//     bChecked    - TRUE if the control is checked.
	//     bPopuped    - TRUE if the control is popuped.
	//     barType     - Parent's bar type
	//     barPosition - Parent's bar position.
	//     nID         - Bar's control ID.
	//-----------------------------------------------------------------------
	virtual void DrawRectangle(CDC* pDC, CRect rc, BOOL bSelected, BOOL bPressed, BOOL bEnabled, BOOL bChecked, BOOL bPopuped, XTPBarType barType, XTPBarPosition barPosition);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to fill a dockbar.
	// Parameters:
	//     pDC  - Pointer to a valid device context
	//     pBar - Points to a CXTPDockBar object.
	//-----------------------------------------------------------------------
	virtual void FillDockBar(CDC *pDC, CXTPDockBar *pBar);

	// Summary:
	//     This method is called to draw a command bar's group separator.
	// Parameters:
	//     pDC      - Pointer to a valid device context
	//     pBar     - Points to a CXTPCommandBar object
	//     pLeftControl - Points to a left CXTPControl object
	//     pRightControl - Points to a left CXTPControl object
	//     bDraw    - TRUE to draw; FALSE to retrieve the size of the separator.
	// Returns:
	//     This method is called to draw a command bar's separator.
	//-----------------------------------------------------------------------
	virtual void DrawCommandBarGroupSeparator(CDC* pDC, CXTPCommandBar* pBar, CXTPControl* pLeftControl, CXTPControl* pRightControl, BOOL bDraw);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the control.
	// Parameters:
	//     pDC   - Pointer to a valid device context
	//     bDraw - TRUE to draw; FALSE to retrieve the size of the control.
	//     pEdit - Edit control to draw.
	// Returns:
	//     Size of the control.
	//-----------------------------------------------------------------------
	virtual CSize DrawControlEdit(CDC* pDC, CXTPControlEdit* pEdit, BOOL bDraw);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the base theme of the manager.
	// Returns:
	//     Returns the base theme.
	//-----------------------------------------------------------------------
	virtual XTPPaintTheme BaseTheme() { return xtpThemeOffice2013; }

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws workspace space
	// Parameters:
	//     pDC - Pointer to device context
	//     rc - Bounding rectangle
	//     rcExclude - Excluded rectangle
	//-----------------------------------------------------------------------
	virtual void FillWorkspace(CDC *pDC, CRect rc, CRect rcExclude);

public:
	virtual void SetImageHandle(HMODULE hResource, LPCTSTR lpszIniFileName);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the manager.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

	virtual void SetColorSet(const CXTPCommandBarsColorSet *pColorSet);

protected:

	virtual BOOL RequiresResourceImages();

	virtual void LoadResource();

	BOOL m_bIntersectMenuBorder;
};

AFX_INLINE BOOL CXTPCommandBarsOffice2013Theme::RequiresResourceImages()
{
	return TRUE;
}

#endif // !defined(__XTPCOMMANDBARSOFFICE2013THEME_H__)
