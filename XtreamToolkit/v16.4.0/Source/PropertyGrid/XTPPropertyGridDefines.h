// XTPPropertyGridDefines.h
//
// This file is a part of the XTREME PROPERTYGRID MFC class library.
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
#if !defined(__XTPPROPERTYGRIDDEFINES_H__)
#define __XTPPROPERTYGRIDDEFINES_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//-----------------------------------------------------------------------
// Summary:
//     XTPPropertyGridSortOrder is an enumeration used by CXTPPropertyGrid to
//     determine the grouping style.  This will specify how the items
//     in the grid are displayed, I.e. Alphabetical, not sort, and
//     grouped by category.
// See Also: CXTPPropertyGrid::SetPropertySort, CXTPPropertyGrid::GetPropertySort
// Example:
//     <code>XTPPropertyGridSortOrder sort = m_wndPropertyGrid.GetPropertySort();</code>
//
// <KEYWORDS xtpGridSortCategorized, xtpGridSortAlphabetical, xtpGridSortNoSort>
//-----------------------------------------------------------------------
enum XTPPropertyGridSortOrder
{
	xtpGridSortCategorized,     // Group items by category.
	xtpGridSortAlphabetical,    // Sort items alphabetically.
	xtpGridSortNoSort           // Disable sorting and grouping, all items are displayed in
	                            // the order that they are added to the grid.
};


//-----------------------------------------------------------------------
// Summary:
//     Visual theme enumeration
// Example:
//     <code> m_wndPropertyGrid.SetTheme(xtpGridThemeWhidbey); </code>
// See Also: CXTPPropertyGrid::SetTheme, CXTPPropertyGrid::GetCurrentTheme
//
// <KEYWORDS xtpGridThemeDefault, xtpGridThemeNativeWinXP, xtpGridThemeOffice2003, xtpGridThemeCool, xtpGridThemeSimple, xtpGridThemeDelphi, xtpGridThemeWhidbey>
//-----------------------------------------------------------------------
enum XTPPropertyGridPaintTheme
{
	xtpGridThemeDefault,          // Default theme.
	xtpGridThemeNativeWinXP,      // WinXP style theme.
	xtpGridThemeOffice2003,       // Office 2003 style theme.
	xtpGridThemeCool,             // Cool theme.
	xtpGridThemeSimple,           // Visual Basic style theme.
	xtpGridThemeDelphi,           // Delphi style theme.
	xtpGridThemeVisualStudio2005, // Visual Studio 2005 "Whidbey" style theme.
	xtpGridThemeOfficeXP,         // Office XP style theme.
	xtpGridThemeResource,         // Office 2007 / 2010 style theme.
	xtpGridThemeVisualStudio2010, // Visual Studio 2010 style theme.
	xtpGridThemeVisualStudio2012Light, // VS 2012 Light style Property Grid theme.
	xtpGridThemeVisualStudio2012Dark,  // VS 2012 Dark style Property Grid theme.
	xtpGridThemeOffice2013,       // Office 2013 style theme.

	// Old names
	xtpGridThemeWhidbey = xtpGridThemeVisualStudio2005,
};

#define xtpGridThemeOffice2007 xtpGridThemeResource

//-----------------------------------------------------------------------
// Summary:
//     Border Style enumeration
// Example:
//     <code>m_wndPropertyGrid.SetBorderStyle(xtpGridBorderClientEdge);</code>
// See Also: CXTPPropertyGrid::SetBorderStyle
//-----------------------------------------------------------------------
enum XTPPropertyGridBorderStyle
{
	xtpGridBorderNone,      // Without borders
	xtpGridBorderFlat,      // Black flat borders
	xtpGridBorderStaticEdge,// Static edge
	xtpGridBorderClientEdge // Client edge
};

//-----------------------------------------------------------------------
// Summary:
//     InplaceButtons style
// Example:
//     pPaintManager->m_buttonsStyle = xtpGridButtonsOfficeXP;
// See Also: CXTPPropertyGrid::SetTheme, CXTPPropertyGrid::GetCurrentTheme
//-----------------------------------------------------------------------
enum XTPPropertyGridInplaceButtonsStyle
{
	xtpGridButtonsDefault,          // Default button style
	xtpGridButtonsThemed,           // Themed button style
	xtpGridButtonsOfficeXP,         // Office XP button style
};


class CXTPPropertyGridItem;


//-----------------------------------------------------------------------
// Summary:
//     The XTPWM_PROPERTYGRID_NOTIFY message is sent to the CXTPPropertyGrid owner window
//     whenever an action occurs within the CXTPPropertyGrid
//
// <code>XTPWM_PROPERTYGRID_NOTIFY
// nGridAction = (int) wParam;              // Property grid action
// pItem = (CXTPPropertyGridItem*) lParam;  // pointer to a CXTPPropertyGridItem object</code>
//
//     When the user performs an action in the property grid, the XTPWM_PROPERTYGRID_NOTIFY message is
//     sent to the property grid's owner window.
// Parameters:
//     nGridAction -  Value of wParam specifies an value that indicates the user's
//                    request .
//     pItem - The value of lParam points to a CXTPPropertyGridItem object that contains information for the
//             specified item. This pointer should <b>never</b> be NULL.
// Returns:
//     If the application is to process this message, the return value should be TRUE, otherwise the
//     return value is FALSE.
// Remarks:
//     nGridAction parameter can be one of the following values:
//          * <b>XTP_PGN_SORTORDER_CHANGED</b> The sort order has changed in the property grid.
//          * <b>XTP_PGN_SELECTION_CHANGED</b> The selection has changed in the property grid.
//          * <b>XTP_PGN_ITEMVALUE_CHANGED</b> The value has changed for pItem in the property grid.
//          * <b>XTP_PGN_EDIT_CHANGED</b> The edit value has changed in the property grid.
//          * <b>XTP_PGN_INPLACEBUTTONDOWN</b> Item's in-place button (combo or expand) down.
//          * <b>XTP_PGN_ITEMEXPANDCHANGED</b> User expand or collapse item.
//          * <b>XTP_PGN_DBLCLICK</b> The user double-clicks the left mouse button in the property grid.
//          * <b>XTP_PGN_RCLICK</b> The user pressed the right mouse button in the property grid.
//          * <b>XTP_PGN_VERB_CLICK</b> The user click verb in the property grid.
//          * <b>XTP_PGN_REQUESTEDIT</b> The user changing value of the item.
//
// Example:
//     Here is an example of how an application would process the XTPWM_PROPERTYGRID_NOTIFY
//     message.
//
// <code>
// int nGridAction = (int)wParam;
//
//
// switch (nGridAction)
// {
//     case XTP_PGN_SORTORDER_CHANGED:
//           {
//              m_nSort = m_wndXTPPropertyGrid.GetPropertySort();
//              UpdateData(FALSE);
//           }
//         break;
//     case XTP_PGN_ITEMVALUE_CHANGED:
//           {
//              CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)lParam;
//              TRACE(_T("Value Changed. Caption = %s, ID = %i, Value = %s\n"),
//                  pItem->GetCaption(), pItem->GetID(), pItem->GetValue());
//           }
//         break;
//     case XTP_PGN_SELECTION_CHANGED:
//           {
//              CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)lParam;
//              TRACE(_T("Selection Changed. Item = %s\n"), pItem->GetCaption());
//           }
//     case XTP_PGN_INPLACEBUTTONDOWN:
//           {
//              CXTPPropertyGridInplaceButton* pButton = (CXTPPropertyGridInplaceButton*)lParam;
//           }
//         break;
// }
// return FALSE;
// </code>
//-----------------------------------------------------------------------
const UINT XTPWM_PROPERTYGRID_NOTIFY = (WM_USER + 9950 + 1);


const UINT XTP_PGN_SORTORDER_CHANGED = 1;  //<ALIAS XTPWM_PROPERTYGRID_NOTIFY>
const UINT XTP_PGN_SELECTION_CHANGED = 2;  //<ALIAS XTPWM_PROPERTYGRID_NOTIFY>
const UINT XTP_PGN_ITEMVALUE_CHANGED = 3;  //<ALIAS XTPWM_PROPERTYGRID_NOTIFY>
const UINT XTP_PGN_EDIT_CHANGED = 4;       //<ALIAS XTPWM_PROPERTYGRID_NOTIFY>
const UINT XTP_PGN_INPLACEBUTTONDOWN = 5;  //<ALIAS XTPWM_PROPERTYGRID_NOTIFY>
const UINT XTP_PGN_DRAWITEM = 6;           //<ALIAS XTPWM_PROPERTYGRID_NOTIFY>
const UINT XTP_PGN_ITEMEXPANDCHANGED = 7;  //<ALIAS XTPWM_PROPERTYGRID_NOTIFY>
const UINT XTP_PGN_DBLCLICK = 8;           //<ALIAS XTPWM_PROPERTYGRID_NOTIFY>
const UINT XTP_PGN_RCLICK = 9;             //<ALIAS XTPWM_PROPERTYGRID_NOTIFY>
const UINT XTP_PGN_VERB_CLICK = 10;        //<ALIAS XTPWM_PROPERTYGRID_NOTIFY>


const UINT XTP_PGN_REQUESTEDIT = 11;       //<ALIAS XTPWM_PROPERTYGRID_NOTIFY>
const UINT XTP_PGN_AFTEREDIT = 12;         //<ALIAS XTPWM_PROPERTYGRID_NOTIFY>
const UINT XTP_PGN_CANCELEDIT = 13;        //<ALIAS XTPWM_PROPERTYGRID_NOTIFY>
const UINT XTP_PGN_USER = 14;              //<ALIAS XTPWM_PROPERTYGRID_NOTIFY>


//===========================================================================
// Summary:
//     This struct passed with XTP_PGN_REQUESTEDIT, XTP_PGN_AFTEREDIT, XTP_PGN_CANCELEDIT notifications to
//     determine item and its new value
// See Also: XTP_PGN_REQUESTEDIT
//===========================================================================
struct XTP_PROPERTYGRID_NOTIFYEDIT
{
	BOOL bCancel;                   // TRUE to cancel user edit
	CString strValue;               // New caption value to be set
	CXTPPropertyGridItem* pItem;    // Item to be edit
};


//-----------------------------------------------------------------------
// Summary:
//     Apply this style to the property grid to use the owner draw feature.
// See Also:
//     CXTPPropertyGrid::Create
// Example:
//     The following example illustrates using XTP_PGS_OWNERDRAW:
// <code>
// m_wndPropertyGrid.ModifyStyle(0, XTP_PGS_OWNERDRAW);
// </code>
//-----------------------------------------------------------------------
const UINT XTP_PGS_OWNERDRAW = 0x0010L;


//-----------------------------------------------------------------------
// Summary:
//     NULL terminated string used by CXTPPropertyGrid which specifies
//     the window class name for the property grid.
// Example:
// <code>
// BOOL CXTPPropertyGrid::RegisterWindowClass()
// {
//     WNDCLASS wndcls;
//     HINSTANCE hInst = XTPGetInstanceHandle();
//
//     if (!(::GetClassInfo(hInst, XTPREPORTCTRL_CLASSNAME, &wndcls)))
//     {
//         // otherwise we need to register a new class
//         wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
//         wndcls.lpfnWndProc      = ::DefWindowProc;
//         wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
//         wndcls.hInstance        = hInst;
//         wndcls.hIcon            = NULL;
//         wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
//         wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
//         wndcls.lpszMenuName     = NULL;
//         wndcls.lpszClassName    = XTPREPORTCTRL_CLASSNAME;
//
//         if (!AfxRegisterClass(&wndcls))
//         {
//             AfxThrowResourceException();
//             return FALSE;
//         }
//     }
//
//     return TRUE;
// }
// </code>
// See Also:
//     CXTPPropertyGrid::RegisterWindowClass
//-----------------------------------------------------------------------
const TCHAR XTPPROPERTYGRID_CLASSNAME[] = _T("XTPPropertyGrid");

#endif //#define __XTPPROPERTYGRIDDEFINES_H__
