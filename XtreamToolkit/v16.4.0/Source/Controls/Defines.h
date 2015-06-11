// Defines.h : notification handlers
//
// This file is a part of the XTREME CONTROLS MFC class library.
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
#if !defined(__XTPCONTROLSDEFINES_H__)
#define __XTPCONTROLSDEFINES_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//{{AFX_CODEJOCK_PRIVATE

#ifndef BTNS_WHOLEDROPDOWN
#define BTNS_WHOLEDROPDOWN              0x80  /* draw dropdown arrow, but without split arrow section */
#endif

// Summary: The I_IMAGENONE constant is used when defining text-only toolbar buttons with no
//          space allocated to the button icon
//          The constant is normally defined in version 5.81 and greater SDK headers
#ifndef I_IMAGENONE
#define I_IMAGENONE (-2)
#endif//I_IMAGENONE

#ifndef IDC_HAND
#define IDC_HAND            MAKEINTRESOURCE(32649)
#endif

const UINT WM_XTP_CONTROLS_BASE = (WM_USER + 9000);

#ifndef ON_WM_MOUSELEAVE
#define ON_WM_MOUSELEAVE() \
	{ WM_MOUSELEAVE, 0, 0, 0, AfxSig_bv, \
		(AFX_PMSG)(AFX_PMSGW)(BOOL (AFX_MSG_CALL CWnd::*)(void))&OnMouseLeave },
#endif

#ifndef ON_WM_PRINTCLIENT
#define ON_WM_PRINTCLIENT() \
	{ WM_PRINTCLIENT, 0, 0, 0, AfxSig_vh, \
		(AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(HDC))&OnPrintClient },
#endif

#ifndef ON_WM_XTP_SETCONTROLTHEME
#define ON_WM_XTP_SETCONTROLTHEME() \
	{ WM_XTP_SETCONTROLTHEME, 0, 0, 0, AfxSig_vw, \
		(AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(XTPControlTheme))&OnSetControlTheme },
#endif

//}}AFX_CODEJOCK_PRIVATE

// --------------------------------------------------------------------
// Summary:
//     CPN_XTP_SELCHANGE notification message handler.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The CPN_XTP_SELCHANGE notification message is sent to inform
//     the owner window that the current color selection has changed.
//     The owner window of the color picker receives this notification
//     through the WM_COMMAND message.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CColorDialog, CDialog
//     //{{AFX_MSG_MAP(CColorDialog)
//     ON_CPN_XTP_SELCHANGE(IDC_CTRL_ID, OnSelChange)
//     //}}AFX_MSG_MAP
// END_MESSAGE_MAP()
//
// void CColorDialog::OnSelChange()
// {
//     // TODO: Handle command.
// }
// </code>
// See Also:
//     CXTPColorPicker, CXTPColorSelectorCtrl, CPN_XTP_DROPDOWN, CPN_XTP_CLOSEUP, CPN_XTP_SELENDOK, CPN_XTP_SELENDCANCEL, CPN_XTP_SELNOFILL
// --------------------------------------------------------------------
const UINT CPN_XTP_SELCHANGE = (WM_XTP_CONTROLS_BASE + 1);
//<ALIAS CPN_XTP_SELCHANGE>
#define ON_CPN_XTP_SELCHANGE(id, memberFxn)
//{{AFX_CODEJOCK_PRIVATE
#undef ON_CPN_XTP_SELCHANGE
#define ON_CPN_XTP_SELCHANGE(id, memberFxn) \
	ON_CONTROL(CPN_XTP_SELCHANGE, id, memberFxn)
//}}AFX_CODEJOCK_PRIVATE

// --------------------------------------------------------------------
// Summary:
//     CPN_XTP_DROPDOWN notification message handler.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The CPN_XTP_DROPDOWN notification message is sent to inform the
//     owner window that the color selection window is has been displayed.
//     The owner window of the color picker receives this notification
//     through the WM_COMMAND message.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CColorDialog, CDialog
//     //{{AFX_MSG_MAP(CColorDialog)
//     ON_CPN_XTP_DROPDOWN(IDC_CTRL_ID, OnDropDown)
//     //}}AFX_MSG_MAP
// END_MESSAGE_MAP()
//
// void CColorDialog::OnDropDown()
// {
//     // TODO: Handle command.
// }
// </code>
// See Also:
//     CXTPColorPicker, CXTPColorSelectorCtrl, CPN_XTP_SELCHANGE, CPN_XTP_CLOSEUP, CPN_XTP_SELENDOK, CPN_XTP_SELENDCANCEL, CPN_XTP_SELNOFILL
// --------------------------------------------------------------------
const UINT CPN_XTP_DROPDOWN = (WM_XTP_CONTROLS_BASE + 2);
//<ALIAS CPN_XTP_DROPDOWN>
#define ON_CPN_XTP_DROPDOWN(id, memberFxn)
//{{AFX_CODEJOCK_PRIVATE
#undef ON_CPN_XTP_DROPDOWN
#define ON_CPN_XTP_DROPDOWN(id, memberFxn) \
	ON_CONTROL(CPN_XTP_DROPDOWN, id, memberFxn)
//}}AFX_CODEJOCK_PRIVATE

// --------------------------------------------------------------------
// Summary:
//     CPN_XTP_CLOSEUP notification message handler.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The CPN_XTP_CLOSEUP notification message is sent to inform the
//     owner window that the color selection window has closed.
//     The owner window of the color picker receives this notification
//     through the WM_COMMAND message.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CColorDialog, CDialog
//     //{{AFX_MSG_MAP(CColorDialog)
//     ON_CPN_XTP_CLOSEUP(IDC_CTRL_ID, OnCloseUp)
//     //}}AFX_MSG_MAP
// END_MESSAGE_MAP()
//
// void CColorDialog::OnCloseUp()
// {
//     // TODO: Handle command.
// }
// </code>
// See Also:
//     CXTPColorPicker, CXTPColorSelectorCtrl, CPN_XTP_SELCHANGE, CPN_XTP_DROPDOWN, CPN_XTP_SELENDOK, CPN_XTP_SELENDCANCEL, CPN_XTP_SELNOFILL
// --------------------------------------------------------------------
const UINT CPN_XTP_CLOSEUP = (WM_XTP_CONTROLS_BASE + 3);
//<ALIAS CPN_XTP_CLOSEUP>
#define ON_CPN_XTP_CLOSEUP(id, memberFxn)
//{{AFX_CODEJOCK_PRIVATE
#undef ON_CPN_XTP_CLOSEUP
#define ON_CPN_XTP_CLOSEUP(id, memberFxn) \
	ON_CONTROL(CPN_XTP_CLOSEUP, id, memberFxn)
//}}AFX_CODEJOCK_PRIVATE

// --------------------------------------------------------------------
// Summary:
//     CPN_XTP_SELENDOK notification message handler.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The CPN_XTP_SELENDOK notification message is sent to inform the
//     owner window that a color selection has been made.
//     The owner window of the color picker receives this notification
//     through the WM_COMMAND message.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CColorDialog, CDialog
//     //{{AFX_MSG_MAP(CColorDialog)
//     ON_CPN_XTP_SELENDOK(IDC_CTRL_ID, OnSelEndOk)
//     //}}AFX_MSG_MAP
// END_MESSAGE_MAP()
//
// void CColorDialog::OnSelEndOk()
// {
//     // TODO: Handle command.
// }
// </code>
// See Also:
//     CXTPColorPicker, CXTPColorSelectorCtrl, CPN_XTP_SELCHANGE, CPN_XTP_DROPDOWN, CPN_XTP_CLOSEUP, CPN_XTP_SELENDCANCEL, CPN_XTP_SELNOFILL
// --------------------------------------------------------------------
const UINT CPN_XTP_SELENDOK = (WM_XTP_CONTROLS_BASE + 4);
//<ALIAS CPN_XTP_SELENDOK>
#define ON_CPN_XTP_SELENDOK(id, memberFxn)
//{{AFX_CODEJOCK_PRIVATE
#undef ON_CPN_XTP_SELENDOK
#define ON_CPN_XTP_SELENDOK(id, memberFxn) \
	ON_CONTROL(CPN_XTP_SELENDOK, id, memberFxn)
//}}AFX_CODEJOCK_PRIVATE

// --------------------------------------------------------------------
// Summary:
//     CPN_XTP_SELENDCANCEL notification message handler.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The CPN_XTP_SELENDCANCEL notification message is sent to inform the
//     owner window that a color selection has been canceled.
//     The owner window of the color picker receives this notification
//     through the WM_COMMAND message.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CColorDialog, CDialog
//     //{{AFX_MSG_MAP(CColorDialog)
//     ON_CPN_XTP_SELENDCANCEL(IDC_CTRL_ID, OnSelEndCancel)
//     //}}AFX_MSG_MAP
// END_MESSAGE_MAP()
//
// void CColorDialog::OnSelEndCancel()
// {
//     // TODO: Handle command.
// }
// </code>
// See Also:
//     CXTPColorPicker, CXTPColorSelectorCtrl, CPN_XTP_SELCHANGE, CPN_XTP_DROPDOWN, CPN_XTP_CLOSEUP, ON_CPN_XTP_SELENDOK, CPN_XTP_SELNOFILL
// --------------------------------------------------------------------
const UINT CPN_XTP_SELENDCANCEL = (WM_XTP_CONTROLS_BASE + 5);
//<ALIAS CPN_XTP_SELENDCANCEL>
#define ON_CPN_XTP_SELENDCANCEL(id, memberFxn)
//{{AFX_CODEJOCK_PRIVATE
#undef ON_CPN_XTP_SELENDCANCEL
#define ON_CPN_XTP_SELENDCANCEL(id, memberFxn) \
	ON_CONTROL(CPN_XTP_SELENDCANCEL, id, memberFxn)
//}}AFX_CODEJOCK_PRIVATE

// --------------------------------------------------------------------
// Summary:
//     CPN_XTP_SELNOFILL notification message handler.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The CPN_XTP_SELNOFILL notification message is sent to inform the
//     owner window that a "No Fill" or "Automatic Color" selection has been made.
//     The owner window of the color picker receives this notification
//     through the WM_COMMAND message.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CColorDialog, CDialog
//     //{{AFX_MSG_MAP(CColorDialog)
//     ON_CPN_XTP_SELNOFILL(IDC_CTRL_ID, OnSelNoFill)
//     //}}AFX_MSG_MAP
// END_MESSAGE_MAP()
//
// void CColorDialog::OnSelNoFill()
// {
//     // TODO: Handle command.
// }
// </code>
// See Also:
//     CXTPColorPicker, CXTPColorSelectorCtrl, CPN_XTP_SELCHANGE, CPN_XTP_DROPDOWN, CPN_XTP_CLOSEUP, ON_CPN_XTP_SELENDOK, CPN_XTP_SELENDCANCEL
// --------------------------------------------------------------------
const UINT CPN_XTP_SELNOFILL = (WM_XTP_CONTROLS_BASE + 6);
//<ALIAS CPN_XTP_SELNOFILL>
#define ON_CPN_XTP_SELNOFILL(id, memberFxn)
//{{AFX_CODEJOCK_PRIVATE
#undef ON_CPN_XTP_SELNOFILL
#define ON_CPN_XTP_SELNOFILL(id, memberFxn) \
	ON_CONTROL(CPN_XTP_SELNOFILL, id, memberFxn)
//}}AFX_CODEJOCK_PRIVATE

// --------------------------------------------------------------------
// Summary:
//     BEN_XTP_LABELEDITEND notification message handler.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The BEN_XTP_LABELEDITEND notification message is sent when
//     the user performs a browse operation successfully. It indicates
//     the user's selection is to be processed. The owner window of the
//     browse edit receives this notification through the WM_COMMAND
//     message.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CBrowseDialog, CDialog
//     //{{AFX_MSG_MAP(CBrowseDialog)
//     ON_BEN_XTP_LABELEDITEND(IDC_CTRL_ID, OnLabelEditEnd)
//     //}}AFX_MSG_MAP
// END_MESSAGE_MAP()
//
// void CBrowseDialog::OnLabelEditEnd()
// {
//     // TODO: Handle command.
// }
// </code>
// See Also:
//     CXTPBrowseEdit, BEN_XTP_LABELEDITCANCEL
// --------------------------------------------------------------------
const UINT BEN_XTP_LABELEDITEND = (WM_XTP_CONTROLS_BASE + 7);
//<ALIAS BEN_XTP_LABELEDITEND>
#define ON_BEN_XTP_LABELEDITEND(id, memberFxn)
//{{AFX_CODEJOCK_PRIVATE
#undef ON_BEN_XTP_LABELEDITEND
#define ON_BEN_XTP_LABELEDITEND(id, memberFxn) \
	ON_CONTROL(BEN_XTP_LABELEDITEND, id, memberFxn)
//}}AFX_CODEJOCK_PRIVATE

// ----------------------------------------------------------------------
// Summary:
//     BEN_XTP_LABELEDITCANCEL notification message handler.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The BEN_XTP_LABELEDITCANCEL notification message is sent
//     when the user performs a browse operation, but then selects
//     another control or closes the dialog box. It indicates the user's
//     initial operation was ignored. The owner window of the browse edit
//     receives this notification through the WM_COMMAND message.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CBrowseDialog, CDialog
//     //{{AFX_MSG_MAP(CBrowseDialog)
//     ON_BEN_XTP_LABELEDITCANCEL(IDC_CTRL_ID, OnLabelEditCancel)
//     //}}AFX_MSG_MAP
// END_MESSAGE_MAP()
//
// void CBrowseDialog::OnLabelEditCancel()
// {
//     // TODO: Handle command.
// }
// </code>
// See Also:
//     CXTPBrowseEdit, BEN_XTP_LABELEDITEND
// ----------------------------------------------------------------------
const UINT BEN_XTP_LABELEDITCANCEL = (WM_XTP_CONTROLS_BASE + 8);
//<ALIAS BEN_XTP_LABELEDITCANCEL>
#define ON_BEN_XTP_LABELEDITCANCEL(id, memberFxn)
//{{AFX_CODEJOCK_PRIVATE
#undef ON_BEN_XTP_LABELEDITCANCEL
#define ON_BEN_XTP_LABELEDITCANCEL(id, memberFxn) \
	ON_CONTROL(BEN_XTP_LABELEDITCANCEL, id, memberFxn)
//}}AFX_CODEJOCK_PRIVATE

// ----------------------------------------------------------------------
// Summary:
//     LBN_XTP_LABELEDITEND notification message handler.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The LBN_XTP_LABELEDITEND notification message is sent
//     to inform the owner whenever the label edit operation has ended.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CAppDialog, CDialog
//     //{{AFX_MSG_MAP(CAppDialog)
//     ON_LBN_XTP_LABELEDITEND(IDC_CTRL_ID, OnLabelEditEnd)
//     //}}AFX_MSG_MAP
// END_MESSAGE_MAP()
//
// void CAppDialog::OnLabelEditEnd()
// {
//     // TODO: Handle command.
// }
// </code>
// See Also:
//     CXTPEditListBox, LBN_XTP_LABELEDITCANCEL, LBN_XTP_NEWITEM, LBN_XTP_PREDELETEITEM, LBN_XTP_DELETEITEM, LBN_XTP_MOVEITEMUP, LBN_XTP_MOVEITEMDOWN, LBN_XTP_PRENEWITEM
// ----------------------------------------------------------------------
const DWORD LBN_XTP_LABELEDITEND = BEN_XTP_LABELEDITEND;
//<ALIAS LBN_XTP_LABELEDITEND>
#define ON_LBN_XTP_LABELEDITEND(id, memberFxn)
//{{AFX_CODEJOCK_PRIVATE
#undef ON_LBN_XTP_LABELEDITEND
#define ON_LBN_XTP_LABELEDITEND(id, memberFxn) \
	ON_CONTROL(LBN_XTP_LABELEDITEND, id, memberFxn)
//}}AFX_CODEJOCK_PRIVATE

// ----------------------------------------------------------------------
// Summary:
//     LBN_XTP_LABELEDITCANCEL notification message handler.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The LBN_XTP_LABELEDITCANCEL notification message is sent
//     to inform the owner whenever the label edit operation has been canceled.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CAppDialog, CDialog
//     //{{AFX_MSG_MAP(CAppDialog)
//     ON_LBN_XTP_LABELEDITCANCEL(IDC_CTRL_ID, OnLabelEditCancel)
//     //}}AFX_MSG_MAP
// END_MESSAGE_MAP()
//
// void CAppDialog::OnLabelEditCancel()
// {
//     // TODO: Handle command.
// }
// </code>
// See Also:
//     CXTPEditListBox, LBN_XTP_LABELEDITEND, LBN_XTP_NEWITEM, LBN_XTP_PREDELETEITEM, LBN_XTP_DELETEITEM, LBN_XTP_MOVEITEMUP, LBN_XTP_MOVEITEMDOWN, LBN_XTP_PRENEWITEM
// ----------------------------------------------------------------------
const DWORD LBN_XTP_LABELEDITCANCEL = BEN_XTP_LABELEDITCANCEL;
//<ALIAS LBN_XTP_LABELEDITCANCEL>
#define ON_LBN_XTP_LABELEDITCANCEL(id, memberFxn)
//{{AFX_CODEJOCK_PRIVATE
#undef ON_LBN_XTP_LABELEDITCANCEL
#define ON_LBN_XTP_LABELEDITCANCEL(id, memberFxn) \
	ON_CONTROL(LBN_XTP_LABELEDITCANCEL, id, memberFxn)
//}}AFX_CODEJOCK_PRIVATE

// ----------------------------------------------------------------------
// Summary:
//     LBN_XTP_NEWITEM notification message handler.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The LBN_XTP_NEWITEM notification message is sent
//     to inform the owner whenever the new item button has been pressed.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CAppDialog, CDialog
//     //{{AFX_MSG_MAP(CAppDialog)
//     ON_LBN_XTP_NEWITEM(IDC_CTRL_ID, OnNewItem)
//     //}}AFX_MSG_MAP
// END_MESSAGE_MAP()
//
// void CAppDialog::OnNewItem()
// {
//     // TODO: Handle command.
// }
// </code>
// See Also:
//     CXTPEditListBox, LBN_XTP_LABELEDITEND, LBN_XTP_LABELEDITCANCEL, LBN_XTP_PREDELETEITEM, LBN_XTP_DELETEITEM, LBN_XTP_MOVEITEMUP, LBN_XTP_MOVEITEMDOWN, LBN_XTP_PRENEWITEM
// ----------------------------------------------------------------------
const UINT LBN_XTP_NEWITEM = (WM_XTP_CONTROLS_BASE + 9);
//<ALIAS LBN_XTP_NEWITEM>
#define ON_LBN_XTP_NEWITEM(id, memberFxn)
//{{AFX_CODEJOCK_PRIVATE
#undef ON_LBN_XTP_NEWITEM
#define ON_LBN_XTP_NEWITEM(id, memberFxn) \
	ON_CONTROL(LBN_XTP_NEWITEM, id, memberFxn)
//}}AFX_CODEJOCK_PRIVATE

// ----------------------------------------------------------------------
// Summary:
//     LBN_XTP_PREDELETEITEM notification message handler.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The LBN_XTP_PREDELETEITEM notification message is sent
//     to inform the owner just before an item is deleted.  This is
//     useful for retrieving information about the selected item, such
//     as item data, before it is removed from the edit list control.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CAppDialog, CDialog
//     //{{AFX_MSG_MAP(CAppDialog)
//     ON_LBN_XTP_PREDELETEITEM(IDC_CTRL_ID, OnPreDeleteItem)
//     //}}AFX_MSG_MAP
// END_MESSAGE_MAP()
//
// void CAppDialog::OnPreDeleteItem()
// {
//     // TODO: Handle command.
// }
// </code>
// See Also:
//     CXTPEditListBox, LBN_XTP_LABELEDITEND, LBN_XTP_LABELEDITCANCEL, LBN_XTP_NEWITEM, LBN_XTP_DELETEITEM, LBN_XTP_MOVEITEMUP, LBN_XTP_MOVEITEMDOWN, LBN_XTP_PRENEWITEM
// ----------------------------------------------------------------------
const UINT LBN_XTP_PREDELETEITEM = (WM_XTP_CONTROLS_BASE + 10);
//<ALIAS LBN_XTP_PREDELETEITEM>
#define ON_LBN_XTP_PREDELETEITEM(id, memberFxn)
//{{AFX_CODEJOCK_PRIVATE
#undef ON_LBN_XTP_PREDELETEITEM
#define ON_LBN_XTP_PREDELETEITEM(id, memberFxn) \
	ON_CONTROL(LBN_XTP_PREDELETEITEM, id, memberFxn)
//}}AFX_CODEJOCK_PRIVATE

// ----------------------------------------------------------------------
// Summary:
//     LBN_XTP_DELETEITEM notification message handler.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The LBN_XTP_DELETEITEM notification message is sent
//     to inform the owner whenever the delete item button has been pressed.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CAppDialog, CDialog
//     //{{AFX_MSG_MAP(CAppDialog)
//     ON_LBN_XTP_DELETEITEM(IDC_CTRL_ID, OnDeleteItem)
//     //}}AFX_MSG_MAP
// END_MESSAGE_MAP()
//
// void CAppDialog::OnDeleteItem()
// {
//     // TODO: Handle command.
// }
// </code>
// See Also:
//     CXTPEditListBox, LBN_XTP_LABELEDITEND, LBN_XTP_LABELEDITCANCEL, LBN_XTP_NEWITEM, LBN_XTP_PREDELETEITEM, LBN_XTP_MOVEITEMUP, LBN_XTP_MOVEITEMDOWN, LBN_XTP_PRENEWITEM
// ----------------------------------------------------------------------
const UINT LBN_XTP_DELETEITEM = (WM_XTP_CONTROLS_BASE + 11);
//<ALIAS LBN_XTP_DELETEITEM>
#define ON_LBN_XTP_DELETEITEM(id, memberFxn)
//{{AFX_CODEJOCK_PRIVATE
#undef ON_LBN_XTP_DELETEITEM
#define ON_LBN_XTP_DELETEITEM(id, memberFxn) \
	ON_CONTROL(LBN_XTP_DELETEITEM, id, memberFxn)
//}}AFX_CODEJOCK_PRIVATE

// ----------------------------------------------------------------------
// Summary:
//     LBN_XTP_MOVEITEMUP notification message handler.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The LBN_XTP_MOVEITEMUP notification message is sent
//     to inform the owner whenever the move item up button has been pressed.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CAppDialog, CDialog
//     //{{AFX_MSG_MAP(CAppDialog)
//     ON_LBN_XTP_MOVEITEMUP(IDC_CTRL_ID, OnMoveItemUp)
//     //}}AFX_MSG_MAP
// END_MESSAGE_MAP()
//
// void CAppDialog::OnMoveItemUp()
// {
//     // TODO: Handle command.
// }
// </code>
// See Also:
//     CXTPEditListBox, LBN_XTP_LABELEDITEND, LBN_XTP_LABELEDITCANCEL, LBN_XTP_NEWITEM, LBN_XTP_PREDELETEITEM, LBN_XTP_DELETEITEM, LBN_XTP_MOVEITEMDOWN, LBN_XTP_PRENEWITEM
// ----------------------------------------------------------------------
const UINT LBN_XTP_MOVEITEMUP = (WM_XTP_CONTROLS_BASE + 12);
//<ALIAS LBN_XTP_MOVEITEMUP>
#define ON_LBN_XTP_MOVEITEMUP(id, memberFxn)
//{{AFX_CODEJOCK_PRIVATE
#undef ON_LBN_XTP_MOVEITEMUP
#define ON_LBN_XTP_MOVEITEMUP(id, memberFxn) \
	ON_CONTROL(LBN_XTP_MOVEITEMUP, id, memberFxn)
//}}AFX_CODEJOCK_PRIVATE

// ----------------------------------------------------------------------
// Summary:
//     LBN_XTP_MOVEITEMDOWN notification message handler.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The LBN_XTP_MOVEITEMDOWN notification message is sent
//     to inform the owner whenever the move item down button has been pressed.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CAppDialog, CDialog
//     //{{AFX_MSG_MAP(CAppDialog)
//     ON_LBN_XTP_MOVEITEMDOWN(IDC_CTRL_ID, OnMoveItemDown)
//     //}}AFX_MSG_MAP
// END_MESSAGE_MAP()
//
// void CAppDialog::OnMoveItemDown()
// {
//     // TODO: Handle command.
// }
// </code>
// See Also:
//     CXTPEditListBox, LBN_XTP_LABELEDITEND, LBN_XTP_LABELEDITCANCEL, LBN_XTP_NEWITEM, LBN_XTP_PREDELETEITEM, LBN_XTP_DELETEITEM, LBN_XTP_MOVEITEMUP, LBN_XTP_PRENEWITEM
// ----------------------------------------------------------------------
const UINT LBN_XTP_MOVEITEMDOWN = (WM_XTP_CONTROLS_BASE + 13);
//<ALIAS LBN_XTP_MOVEITEMDOWN>
#define ON_LBN_XTP_MOVEITEMDOWN(id, memberFxn)
//{{AFX_CODEJOCK_PRIVATE
#undef ON_LBN_XTP_MOVEITEMDOWN
#define ON_LBN_XTP_MOVEITEMDOWN(id, memberFxn) \
	ON_CONTROL(LBN_XTP_MOVEITEMDOWN, id, memberFxn)
//}}AFX_CODEJOCK_PRIVATE

// ----------------------------------------------------------------------
// Summary:
//     LBN_XTP_PRENEWITEM notification message handler.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The LBN_XTP_PRENEWITEM notification message is sent
//     to inform the owner whenever that a new item is about to be
//     edited in EditListBox.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CAppDialog, CDialog
//     //{{AFX_MSG_MAP(CAppDialog)
//     ON_LBN_XTP_PRENEWITEM(IDC_CTRL_ID, OnPreNewItem)
//     //}}AFX_MSG_MAP
// END_MESSAGE_MAP()
//
// void CAppDialog::OnPreNewItem()
// {
//     // TODO: Handle command.
// }
// </code>
// See Also:
//     CXTPEditListBox, LBN_XTP_LABELEDITEND, LBN_XTP_LABELEDITCANCEL, LBN_XTP_NEWITEM, LBN_XTP_PREDELETEITEM, LBN_XTP_DELETEITEM, LBN_XTP_MOVEITEMUP, LBN_XTP_MOVEITEMDOWN
// ----------------------------------------------------------------------
const UINT LBN_XTP_PRENEWITEM = (WM_XTP_CONTROLS_BASE + 14);
//<ALIAS LBN_XTP_PRENEWITEM>
#define ON_LBN_XTP_PRENEWITEM(id, memberFxn)
//{{AFX_CODEJOCK_PRIVATE
#undef ON_LBN_XTP_PRENEWITEM
#define ON_LBN_XTP_PRENEWITEM(id, memberFxn) \
	ON_CONTROL(LBN_XTP_PRENEWITEM, id, memberFxn)
//}}AFX_CODEJOCK_PRIVATE

// ----------------------------------------------------------------------
// Summary:
//     LBN_XTP_ONBROWSE notification message handler.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The LBN_XTP_ONBROWSE notification message is sent
//     to inform the owner when the browse button is pressed.
// See Also:
//     CXTPEditListBox, LBN_XTP_LABELEDITEND, LBN_XTP_LABELEDITCANCEL, LBN_XTP_NEWITEM, LBN_XTP_PREDELETEITEM, LBN_XTP_DELETEITEM, LBN_XTP_MOVEITEMUP, LBN_XTP_MOVEITEMDOWN
// ----------------------------------------------------------------------
const UINT LBN_XTP_ONBROWSE = (WM_XTP_CONTROLS_BASE + 27);
//<ALIAS LBN_XTP_ONBROWSE>
#define ON_LBN_XTP_ONBROWSE(id, memberFxn)
//{{AFX_CODEJOCK_PRIVATE
#undef ON_LBN_XTP_ONBROWSE
#define ON_LBN_XTP_ONBROWSE(id, memberFxn) \
	ON_CONTROL(LBN_XTP_ONBROWSE, id, memberFxn)
//}}AFX_CODEJOCK_PRIVATE

// ----------------------------------------------------------------------
// Summary:
//     SON_XTP_ITEMEXPANDING notification message handler.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The SON_XTP_ITEMEXPANDING notification message is sent
//     to inform the owner whenever the search options control is expanding.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CAppDialog, CDialog
//     //{{AFX_MSG_MAP(CAppDialog)
//     ON_SON_XTP_ITEMEXPANDING(IDC_CTRL_ID, OnItemExpanding)
//     //}}AFX_MSG_MAP
// END_MESSAGE_MAP()
//
// void CAppDialog::OnItemExpanding()
// {
//     // TODO: Handle command.
// }
// </code>
// See Also:
//     CXTPSearchOptionsCtrl, SON_XTP_ITEMEXPAND, SON_XTP_ITEMCONTRACTING, SON_XTP_ITEMCONTRACT
// ----------------------------------------------------------------------
const UINT SON_XTP_ITEMEXPANDING = (WM_XTP_CONTROLS_BASE + 15);
//<ALIAS SON_XTP_ITEMEXPANDING>
#define ON_SON_XTP_ITEMEXPANDING(id, memberFxn)
//{{AFX_CODEJOCK_PRIVATE
#undef ON_SON_XTP_ITEMEXPANDING
#define ON_SON_XTP_ITEMEXPANDING(id, memberFxn) \
	ON_CONTROL(SON_XTP_ITEMEXPANDING, id, memberFxn)
//}}AFX_CODEJOCK_PRIVATE

// ----------------------------------------------------------------------
// Summary:
//     SON_XTP_ITEMEXPAND notification message handler.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The SON_XTP_ITEMEXPAND notification message is sent
//     to inform the owner whenever the search options control has expanded.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CAppDialog, CDialog
//     //{{AFX_MSG_MAP(CAppDialog)
//     ON_SON_XTP_ITEMEXPAND(IDC_CTRL_ID, OnItemExpand)
//     //}}AFX_MSG_MAP
// END_MESSAGE_MAP()
//
// void CAppDialog::OnItemExpand()
// {
//     // TODO: Handle command.
// }
// </code>
// See Also:
//     CXTPSearchOptionsCtrl, SON_XTP_ITEMEXPANDING, SON_XTP_ITEMCONTRACTING, SON_XTP_ITEMCONTRACT
// ----------------------------------------------------------------------
const UINT SON_XTP_ITEMEXPAND = (WM_XTP_CONTROLS_BASE + 16);
//<ALIAS SON_XTP_ITEMEXPAND>
#define ON_SON_XTP_ITEMEXPAND(id, memberFxn)
//{{AFX_CODEJOCK_PRIVATE
#undef ON_SON_XTP_ITEMEXPAND
#define ON_SON_XTP_ITEMEXPAND(id, memberFxn) \
	ON_CONTROL(SON_XTP_ITEMEXPAND, id, memberFxn)
//}}AFX_CODEJOCK_PRIVATE

// ----------------------------------------------------------------------
// Summary:
//     SON_XTP_ITEMCONTRACTING notification message handler.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The SON_XTP_ITEMCONTRACTING notification message is sent
//     to inform the owner whenever the search options control is contracting.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CAppDialog, CDialog
//     //{{AFX_MSG_MAP(CAppDialog)
//     ON_SON_XTP_ITEMCONTRACTING(IDC_CTRL_ID, OnItemContracting)
//     //}}AFX_MSG_MAP
// END_MESSAGE_MAP()
//
// void CAppDialog::OnItemContracting()
// {
//     // TODO: Handle command.
// }
// </code>
// See Also:
//     CXTPSearchOptionsCtrl, SON_XTP_ITEMEXPANDING, SON_XTP_ITEMEXPAND, SON_XTP_ITEMCONTRACT
// ----------------------------------------------------------------------
const UINT SON_XTP_ITEMCONTRACTING = (WM_XTP_CONTROLS_BASE + 17);
//<ALIAS SON_XTP_ITEMCONTRACTING>
#define ON_SON_XTP_ITEMCONTRACTING(id, memberFxn)
//{{AFX_CODEJOCK_PRIVATE
#undef ON_SON_XTP_ITEMCONTRACTING
#define ON_SON_XTP_ITEMCONTRACTING(id, memberFxn) \
	ON_CONTROL(SON_XTP_ITEMCONTRACTING, id, memberFxn)
//}}AFX_CODEJOCK_PRIVATE

// ----------------------------------------------------------------------
// Summary:
//     SON_XTP_ITEMCONTRACT notification message handler.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The SON_XTP_ITEMCONTRACT notification message is sent
//     to inform the owner whenever the search options control has contracted.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CAppDialog, CDialog
//     //{{AFX_MSG_MAP(CAppDialog)
//     ON_SON_XTP_ITEMCONTRACT(IDC_CTRL_ID, OnItemContract)
//     //}}AFX_MSG_MAP
// END_MESSAGE_MAP()
//
// void CAppDialog::OnItemContract()
// {
//     // TODO: Handle command.
// }
// </code>
// See Also:
//     CXTPSearchOptionsCtrl, SON_XTP_ITEMEXPANDING, SON_XTP_ITEMEXPAND, SON_XTP_ITEMCONTRACTING
// ----------------------------------------------------------------------
const UINT SON_XTP_ITEMCONTRACT = (WM_XTP_CONTROLS_BASE + 18);
//<ALIAS SON_XTP_ITEMCONTRACT>
#define ON_SON_XTP_ITEMCONTRACT(id, memberFxn)
//{{AFX_CODEJOCK_PRIVATE
#undef ON_SON_XTP_ITEMCONTRACT
#define ON_SON_XTP_ITEMCONTRACT(id, memberFxn) \
	ON_CONTROL(SON_XTP_ITEMCONTRACT, id, memberFxn)
//}}AFX_CODEJOCK_PRIVATE


const UINT WM_XTP_SHORTCUTLBOX_NOTIFY = (WM_XTP_CONTROLS_BASE + 19);

// ----------------------------------------------------------------------------------------------------
// Summary:
//     The WM_XTP_SHELL_NOTIFY message is sent to the CXTPShellTreeCtrl
//     or CXTPShellListCtrl owner whenever an action occurs within
//     the control.
// Parameters:
//     nShellAction -  (int) wParam; - value of <i>wParam</i> specifies a shell tree
//                     value that indicates the users request. See Remarks section for a list of values.
//     pItemData -     (XTP_TVITEMDATA*) lParam; - value of <i>lParam</i> points to an XTP_TVITEMDATA
//                     structure that contains information for the specified item.
//                     Depending on the action, this pointer can be NULL.
// Remarks:
//     When the user performs an action in the shell tree, the WM_XTP_SHELL_NOTIFY
//     message is sent to the shell tree's owner window.<p/>
//
//     The nShellAction parameter can be one of the following values:<p/>
//
//      * <b>SHN_XTP_SHELLMENU</b> Shell context menu selection made.
//      * <b>SHN_XTP_TREESELCHANGE</b> Tree selection made.
//      * <b>SHN_XTP_SELECTCHILD</b> Child tree node selected.
//      * <b>SHN_XTP_NOFOLDER</b> Item selected was not a folder.
//      * <b>SHN_XTP_INETFOLDER</b> Item selected was the Internet
//        folder.
//      * <b>SHN_XTP_CONTENTSCHANGED</b> Indicates the folder's contents
//        have changed.
//      * <b>SHN_XTP_REFRESHFOLDER</b> Indicates the folder needs to be
//        updated.
//      * <b>SHN_XTP_REFRESHTREE</b> Indicates the tree needs to be
//        updated.
// Returns:
//     If the application is to process this message, the return value
//     should be 0.
// Example:
//     Here is an example of how an application would process the WM_XTP_SHELL_NOTIFY
//     message.
// <code>
// int nShellAction = (int)wParam;
//
// // Cast the lParam to an XTP_TVITEMDATA* struct pointer.
// XTP_TVITEMDATA* pItemData = (XTP_TVITEMDATA*)lParam;
// ASSERT(pItemData);
//
// switch (nShellAction)
// {
//     case SHN_XTP_SHELLMENU:
//     case SHN_XTP_TREESELCHANGE:
//     case SHN_XTP_SELECTCHILD:
//     case SHN_XTP_NOFOLDER:
//     case SHN_XTP_INETFOLDER:
//     case SHN_XTP_CONTENTSCHANGED:
//     case SHN_XTP_REFRESHFOLDER:
//     case SHN_XTP_REFRESHTREE:
//         // TODO: Handle shell notification message.
//         TRACE0("Shell notification was sent.n");
//         break;
// }
// return 0;
// </code>
// See Also:
//     CXTPShellTreeCtrl, CXTPShellListCtrl, CXTPShellListBase,
//     CXTPDirWatcher
// ----------------------------------------------------------------------------------------------------
const UINT WM_XTP_SHELL_NOTIFY = (WM_XTP_CONTROLS_BASE + 20);
const int SHN_XTP_SHELLMENU       = 1; //<ALIAS WM_XTP_SHELL_NOTIFY>
const int SHN_XTP_TREESELCHANGE   = 2; //<ALIAS WM_XTP_SHELL_NOTIFY>
const int SHN_XTP_SELECTCHILD     = 3; //<ALIAS WM_XTP_SHELL_NOTIFY>
const int SHN_XTP_NOFOLDER        = 4; //<ALIAS WM_XTP_SHELL_NOTIFY>
const int SHN_XTP_INETFOLDER      = 5; //<ALIAS WM_XTP_SHELL_NOTIFY>
const int SHN_XTP_CONTENTSCHANGED = 6; //<ALIAS WM_XTP_SHELL_NOTIFY>
const int SHN_XTP_REFRESHFOLDER   = 7; //<ALIAS WM_XTP_SHELL_NOTIFY>
const int SHN_XTP_REFRESHTREE     = 8; //<ALIAS WM_XTP_SHELL_NOTIFY>

// -------------------------------------------------------------------------------------
// Summary:
//     The TIN_XTP_TRAYICON message is sent to the CXTPTrayIcon
//     window's owner whenever a mouse event occurs in the CXTPTrayIcon.
// Parameters:
//     uID -        (UINT) wParam; - value of <i>wParam</i> specifies the
//                  resource ID of the icon associated with the CXTPTrayIcon
//                  object.
//     uMouseMsg -  (UINT) lParam; - value of <i>lParam</i> specifies the mouse
//                  or keyboard message associated with the event.
// Remarks:
//     When a mouse or keyboard event occurs on a tray icon, the TIN_XTP_TRAYICON
//     message is sent to the tray icon's owner window.
// Returns:
//     If the application is to process this message, the return value
//     should be 0.
// Example:
//     Here is an example of how an application would process the TIN_XTP_TRAYICON
//     message.
// <code>
// BEGIN_MESSAGE_MAP(CMainFrame, CXTMDIFrameWnd)
//     //{{AFX_MSG_MAP(CMainFrame)
//     ON_MESSAGE(TIN_XTP_TRAYICON, OnTrayIconNotify)
//     //}}AFX_MSG_MAP
// END_MESSAGE_MAP()
//
// LRESULT CMainFrame::OnTrayIconNotify(WPARAM wParam, LPARAM lParam)
// {
//     UINT uID = (UINT) wParam;        // resource ID of the tray icon.
//     UINT uMouseMsg = (UINT) lParam;  // mouse message that was sent.
//
//     switch (uMouseMsg)
//     {
//         case WM_RBUTTONUP:
//         {
//             CMenu menu;
//             VERIFY(menu.LoadMenu(IDR_MAINFRAME));
//
//             CMenu* pPopup = menu.GetSubMenu(0);
//             ASSERT(pPopup != NULL);
//             CWnd* pWndPopupOwner = this;
//
//             // Insert the restore menu command into the popup.
//             if (m_bIsVisible == false)
//             {
//                 pPopup-\>InsertMenu(0, MF_BYPOSITION,
//                     IDR_RESTORE, _T("&Restore Window..."));
//
//                 pPopup-\>InsertMenu(1, MF_BYPOSITION | MF_SEPARATOR,
//                     IDR_RESTORE);
//
//                 // Make restore command bold.
//                 \::SetMenuDefaultItem(pPopup-\>m_hMenu, IDR_RESTORE, FALSE);
//             }
//             else
//             {
//                 // Make the exit command bold.
//                 \::SetMenuDefaultItem(pPopup-\>m_hMenu, ID_APP_EXIT, FALSE);
//             }
//
//             // Display the menu at the current mouse location. There's a "bug"
//             // (Microsoft calls it a feature) in Windows 95 that requires calling
//             // SetForegroundWindow. To find out more, search for Q135788 in MSDN.
//             //
//             CPoint point;
//             GetCursorPos(&point);
//             \::SetForegroundWindow(m_hWnd);
//
//             while (pWndPopupOwner-\>GetStyle() & WS_CHILD)
//                 pWndPopupOwner = pWndPopupOwner-\>GetParent();
//
//             int iCmd = pPopup-\>TrackPopupMenu(
//                 TPM_RETURNCMD | TPM_LEFTALIGN | TPM_RIGHTBUTTON,
//                 point.x, point.y, pWndPopupOwner);
//
//             // at this point we want to make sure that the app is visible
//             // after the user makes a menu selection - this is just one way
//             // of doing so:
//
//             switch (iCmd)
//             {
//                 case 0:
//                     // user canceled menu - do nothing
//                     break;
//
//                 // these are commands for which we don't need to make the
//                 // main app window visible
//                 case ID_APP_EXIT:
//                     PostMessage(WM_COMMAND, iCmd, 0);  // just post the command
//                     break;
//
//                 // for all other menu commands - make sure the window is
//                 // visible before we continue
//                 default:
//                     // make main window visible
//                     OnRestore();
//                     PostMessage(WM_COMMAND, iCmd, 0); // post the command
//                     break;
//             }
//
//             return 1; // let the tray icon know that we handled this message.
//         }
//
//         case WM_LBUTTONDBLCLK:
//             OnRestore();
//             return 1; // let the tray icon know that we handled this message.
//     }
//
//     return 0;
// }
// </code>
// See Also:
//     CXTPTrayIcon
// -------------------------------------------------------------------------------------
const UINT TIN_XTP_TRAYICON = (WM_XTP_CONTROLS_BASE + 21);

// ----------------------------------------------------------------------
// Summary:
//     The CPN_XTP_PUSHPINBUTTON message is sent to the owner of a CXTPCaptionPopupWnd whenever
//     the push pin button selection has been made.
// Remarks:
//     When the user makes a selection of the push pin button, the CPN_XTP_PUSHPINBUTTON message is
//     sent to the caption popup window's owner window.
// Example:
//     Here is an example of how an application would process the CPN_XTP_PUSHPINBUTTON
//     message.
// <code>
// BEGIN_MESSAGE_MAP(CMainFrame, CXTMDIFrameWnd)
//     //{{AFX_MSG_MAP(CMainFrame)
//     ON_MESSAGE_VOID(CPN_XTP_PUSHPINBUTTON, OnPushPinButton)
//     //}}AFX_MSG_MAP
// END_MESSAGE_MAP()
//
// void CMainFrame::OnPushPinButton()
// {
//     // TODO: Add your message handler code here and/or call default
//     m_wndSplitter2.ShowColumn();
//     m_nColumn = 1;
// }
// </code>
// See Also:
//     CXTPCaption, CXTPCaptionPopupWnd
// ----------------------------------------------------------------------
const UINT CPN_XTP_PUSHPINBUTTON = (WM_XTP_CONTROLS_BASE + 22);

// ----------------------------------------------------------------------
// Summary:
//     The CPN_XTP_PUSHPINCANCEL message is sent to the owner of a CXTPCaptionPopupWnd whenever
//     the push pin button selection has been canceled.
// Remarks:
//     When the user cancels a selection of the push pin button, the CPN_XTP_PUSHPINCANCEL message is
//     sent to the caption popup window's owner window.
// Example:
//     Here is an example of how an application would process the CPN_XTP_PUSHPINCANCEL
//     message.
// <code>
// BEGIN_MESSAGE_MAP(CMainFrame, CXTMDIFrameWnd)
//     //{{AFX_MSG_MAP(CMainFrame)
//     ON_MESSAGE_VOID(CPN_XTP_PUSHPINCANCEL, OnPushPinCancel)
//     //}}AFX_MSG_MAP
// END_MESSAGE_MAP()
//
// void CMainFrame::OnPushPinCancel()
// {
//     // TODO: Add your message handler code here and/or call default
// }
// </code>
// See Also:
//     CXTPCaption, CXTPCaptionPopupWnd
// ----------------------------------------------------------------------
const UINT CPN_XTP_PUSHPINCANCEL = (WM_XTP_CONTROLS_BASE + 23);

// ----------------------------------------------------------------------
// Summary:
//     The WM_XTP_UPDATECOLOR message is used to send notification that
//     the color selection has changed.
// Parameters:
//     crNew - (COLORREF) wParam; - newly selected RGB color.
// Remarks:
//     This message is sent to notify the owner that a color selection has changed.
// Example:
//     Here is an example of how an application would process the WM_XTP_UPDATECOLOR
//     message.
// <code>
// BEGIN_MESSAGE_MAP(CXTPColorPageCustom, CPropertyPage)
//     //{{AFX_MSG_MAP(CXTPColorPageCustom)
//      ON_MESSAGE(WM_XTP_UPDATECOLOR, OnUpdateColor)
//     //}}AFX_MSG_MAP
// END_MESSAGE_MAP()
//
// LRESULT CXTPColorPageCustom::OnUpdateColor(WPARAM wParam, LPARAM lParam)
// {
//     COLORREF crNew = (COLORREF)wParam;
//     HWND hWnd = (HWND)lParam;
//
//     return 0;
// }
// </code>
// See Also:
//     CXTPColorDialog, CXTPColorBase, CXTPColorLum, CXTPColorPageCustom, CXTPColorHex, CXTPColorPageStandard
// ----------------------------------------------------------------------
const DWORD WM_XTP_UPDATECOLOR = (WM_XTP_CONTROLS_BASE + 26);

// ----------------------------------------------------------------------
// Summary:
//     The WM_XTP_EYEDROPPER_MOVING message is sent to the owner of a CXTPEyeDropper
//     control whenever the mouse moves over a color on the screen.
// Parameters:
//     color - (wParam) An RGB value representing the color that was double clicked.
//     point - (lParam) The low-order word specifies the x-coordinate of the cursor.
//                      The high-order word specifies the y-coordinate of the cursor.
// Example:
//     Here is an example of how an application would process the WM_XTP_EYEDROPPER_MOVING
//     message.
// <code>
// BEGIN_MESSAGE_MAP(CMainFrame, CXTMDIFrameWnd)
//     //{{AFX_MSG_MAP(CMainFrame)
//     ON_MESSAGE(WM_XTP_EYEDROPPER_MOVING, OnEyeDropperMoving)
//     //}}AFX_MSG_MAP
// END_MESSAGE_MAP()
//
// LRESULT CTestDialogDlg::OnEyeDropperMoving(WPARAM wParam, LPARAM lParam)
// {
//     COLORREF color = (WPARAM)wParam;
//
//     CPoint point;
//     point.x = LOWORD(lParam);
//     point.y = HIWORD(lParam);
//
//     return 0;
// }
// </code>
// See Also:
//     CXTPEyeDropper, CXTPColorDialog, WM_XTP_EYEDROPPER_SELECT
// ----------------------------------------------------------------------
const UINT WM_XTP_EYEDROPPER_MOVING = (WM_XTP_CONTROLS_BASE + 32);

// ----------------------------------------------------------------------
// Summary:
//     The WM_XTP_EYEDROPPER_SELECT message is sent to the owner of a CXTPEyeDropper
//     control whenever the user selects a color on the screen.
// Parameters:
//     color - (wParam) An RGB value representing the color that was double clicked.
//     point - (lParam) The low-order word specifies the x-coordinate of the cursor.
//                      The high-order word specifies the y-coordinate of the cursor.
// Example:
//     Here is an example of how an application would process the WM_XTP_EYEDROPPER_SELECT
//     message.
// <code>
// BEGIN_MESSAGE_MAP(CMainFrame, CXTMDIFrameWnd)
//     //{{AFX_MSG_MAP(CMainFrame)
//     ON_MESSAGE(WM_XTP_EYEDROPPER_SELECT, OnEyeDropperSelect)
//     //}}AFX_MSG_MAP
// END_MESSAGE_MAP()
//
// LRESULT CTestDialogDlg::OnEyeDropperSelect(WPARAM wParam, LPARAM lParam)
// {
//     COLORREF color = (WPARAM)wParam;
//
//     CPoint point;
//     point.x = LOWORD(lParam);
//     point.y = HIWORD(lParam);
//
//     return 0;
// }
// </code>
// See Also:
//     CXTPEyeDropper, CXTPColorDialog, WM_XTP_EYEDROPPER_MOVING
// ----------------------------------------------------------------------
const UINT WM_XTP_EYEDROPPER_SELECT = (WM_XTP_CONTROLS_BASE + 33);

// ----------------------------------------------------------------------
// Summary:
//     The WM_XTP_COLORDBLCLK message is used to send notification that
//     a color has been double clicked in the CXTPColorHex control.
// Parameters:
//     color - (wParam) An RGB value representing the color that was double clicked.
//     point - (lParam) The low-order word specifies the x-coordinate of the cursor.
//                      The high-order word specifies the y-coordinate of the cursor.
// Remarks:
//     This message is sent to notify the owner that a color selection has changed.
// Example:
//     Here is an example of how an application would process the WM_XTP_COLORDBLCLK
//     message.
// <code>
// BEGIN_MESSAGE_MAP(CXTPColorPageStandard, CPropertyPage)
//     //{{AFX_MSG_MAP(CXTPColorPageStandard)
//      ON_MESSAGE(WM_XTP_COLORDBLCLK, OnColorDblClick)
//     //}}AFX_MSG_MAP
// END_MESSAGE_MAP()
//
// LRESULT CXTPColorPageCustom::OnColorDblClick(WPARAM wParam, LPARAM lParam)
// {
//     COLORREF color = (COLORREF)wParam;
//
//     CPoint point;
//     point.x = LOWORD(lParam);
//     point.y = HIWORD(lParam);
//
//     return 0;
// }
// </code>
// See Also:
//     CXTPColorDialog, CXTPColorBase, CXTPColorLum, CXTPColorPageCustom, CXTPColorHex, CXTPColorPageStandard
// ----------------------------------------------------------------------
const DWORD WM_XTP_COLORDBLCLK   = (WM_XTP_CONTROLS_BASE + 34);

// ----------------------------------------------------------------------
// Summary:
//     The WM_XTP_SETCONTROLTHEME message is sent to notify a window the
//     control theme has changed..
// Parameters:
//     eTheme - (wParam) An integer value representing the active theme to
//              set. Can be any of the values listed in the Remarks section.
// Remarks:
//     eTheme can be one of the following values:
//     * <b>xtpControlThemeDefault</b> Use default theme.
//     * <b>xtpControlThemeFlat</b> Flat appearance style.
//     * <b>xtpControlThemeUltraFlat</b> Ultra flat appearance style.
//     * <b>xtpControlThemeOffice2000</b> Use Office 2000 theme.
//     * <b>xtpControlThemeOfficeXP</b> Use Office XP theme.
//     * <b>xtpControlThemeOffice2003</b> Use Office 2003 theme.
//     * <b>xtpControlThemeResource</b> Use Office 2007 theme.
//     * <b>xtpControlThemeOffice2010</b> Use Office 2010 theme.
//     * <b>xtpControlThemeVisualStudio2005</b> Use Visual Studio 2008 theme.
//     * <b>xtpControlThemeVisualStudio2008</b> Use Visual Studio 2008 theme.
//     * <b>xtpControlThemeVisualStudio2010</b> Use Visual Studio 2008 theme.
//     * <b>xtpControlThemeNativeWinXP</b> Use Native Windows theme.
//     * <b>xtpControlThemeCustom</b> User defined custom theme.
//     * xtpControlThemeOffice2013</b> Office 2013 control theme.
//     * xtpControlThemeVisualStudio2012Light</b> VS 2012 Light style theme.
//     * xtpControlThemeVisualStudio2012Dark</b> VS 2012 Dark style theme.

// Example:
//     Here is an example of how an application would process the WM_XTP_SETCONTROLTHEME
//     message.
// <code>
// BEGIN_MESSAGE_MAP(CMainFrame, CXTMDIFrameWnd)
//     //{{AFX_MSG_MAP(CMainFrame)
//     ON_MESSAGE(WM_XTP_SETCONTROLTHEME, OnSetTheme)
//     //}}AFX_MSG_MAP
// END_MESSAGE_MAP()
//
// LRESULT CTestDialogDlg::OnSetTheme(WPARAM wParam, LPARAM /*lParam*/)
// {
//     XTPControlTheme eTheme = (XTPControlTheme)wParam;
//
//     SendMessageToDescendants(WM_XTP_SETCONTROLTHEME, wParam, 0L, TRUE, FALSE);
//
//     return 0;
// }
// </code>
// See Also:
//     XTPControlTheme
// ----------------------------------------------------------------------
const DWORD WM_XTP_SETCONTROLTHEME = (WM_XTP_CONTROLS_BASE + 35);

//////////////////////////////////////////////////////////////////////

#endif // #if !defined(__XTPCONTROLSDEFINES_H__)
