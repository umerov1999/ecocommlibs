// XTPFlowGraphEditItem.h: interface for the CXTPFlowGraphEditItem class.
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
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
#if !defined(__XTPFLOWGRAPHEDITITEM_H__)
#define __XTPFLOWGRAPHEDITITEM_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000




// --------------------------------------------------------------------
// Summary:
//     Represents an edit control used to edit captions for elements in
//     the flow graph.
// --------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPFlowGraphEditItem : public CEdit
{
public:
	// ----------------------------------------------
	// Summary:
	//     Constructs a CXTPFlowGraphEditItem object.
	// ----------------------------------------------
	CXTPFlowGraphEditItem();

public:
	// ---------------------------------------------------------------------
	// Summary:
	//     This member function is called to create the edit control used to
	//     edit the caption of a connection.
	// Parameters:
	//     pElement :  [in] Parent element to create the edit control for.
	//
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	// ---------------------------------------------------------------------
	BOOL Create(CXTPFlowGraphElement* pElement);

	//{{AFX_CODEJOCK_PRIVATE
protected:
	virtual void PostNcDestroy();

protected:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	// -----------------------------------------------------------------------------
	// Summary:
	//     This member function is used to processes keyboard events.
	// Parameters:
	//     nChar :    A UINT that contains the character code value of the
	//                key.
	//     nRepCnt :  A UINT that is used to count the number of times the keystroke
	//                is repeated as a result of the user holding down the
	//                key.
	//     nFlags :   A UINT that contains the Scan code, key\-transition
	//                code, previous key state, and the context code.
	// Remarks:
	//     This method is called by the flow graph when a keystroke occurs
	//     during an edit operation and translates it to a nonsystem
	//     character.
	// -----------------------------------------------------------------------------
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg UINT OnGetDlgCode();
	//}}AFX_CODEJOCK_PRIVATE

protected:
	BOOL m_bEscapeKey; // True if the escape key was pressed, False otherwise.
	CXTPFlowGraphElement* m_pElement; // Parent element to create the edit control for.
	CFont m_fnt; // Font used in the edit control.


protected:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CXTPFlowGraphEditItem)
	//}}AFX_CODEJOCK_PRIVATE

};

#endif //#if !defined(__XTPFLOWGRAPHEDITITEM_H__)
