// XTPFlowGraphElement.h: interface for the CXTPFlowGraphElement class.
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
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
#if !defined(__XTPFLOWGRAPHELEMENT_H__)
#define __XTPFLOWGRAPHELEMENT_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPFlowGraphControl;
class CXTPFlowGraphPage;

// -----------------------------------------------------------------------
//
// Summary:
//     Base class for all elements in the flow graph.
//
// Remarks:
//     Every object in the flow graph is derived from FlowGraphElement. It
//     is commonly used when referencing an object that can be many
//     different types, for example when using HitTest the returned
//     \object is a FlowGraphElement and then you can cast it to the
//     appropriate type.
// -----------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPFlowGraphElement : public CXTPCmdTarget
{
	DECLARE_DYNAMIC(CXTPFlowGraphElement);

public:
	// ---------------------------------------------
	// Summary:
	//     Constructs a CXTPFlowGraphElement object.
	// ---------------------------------------------
	CXTPFlowGraphElement();
	// ---------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPFlowGraphElement object, handles cleanup and
	//     deallocation.
	// ---------------------------------------------------------------
	virtual ~CXTPFlowGraphElement();

public:
	// -----------------------------------------------------
	// Summary:
	//     Gets a reference to the parent FlowGraph control.
	// Returns:
	//     A reference to the parent FlowGraph control.
	// -----------------------------------------------------
	virtual CXTPFlowGraphControl* GetControl() const = 0;
	// ---------------------------------------------------------------------
	//
	// Summary:
	//     Reference to the page that this element is displayed on.
	// Returns:
	//     Reference to the FlowGraphPage that this element is displayed on.
	// ---------------------------------------------------------------------
	virtual CXTPFlowGraphPage* GetPage() const = 0;
	// -------------------------------------------------------------
	//
	// Summary:
	//     Specifies whether the node is currently selected.
	// Returns:
	//     \Returns True is the element is currently selected, False
	//     \otherwise.
	//
	//
	// -------------------------------------------------------------
	BOOL IsSelected() const;

	// -----------------------------------------------------------------
	// Summary:
	//     This method is called when the flow graph layout has changed.
	// Remarks:
	//     This is called when the page zoom level, page scroll offset,
	//     window\\node size, window handle, node style, node color, or
	//     connection style is changed.
	// -----------------------------------------------------------------
	void OnGraphChanged();

	// ------------------------------------------------------
	// Summary:
	//     This method is called when the element is removed.
	// ------------------------------------------------------
	virtual void OnRemoved();

	// --------------------------------------------------------------
	//
	// Summary:
	//     Gets\\sets the caption that is displayed for this element.
	// Parameters:
	//     lpszCaption :  The caption to displayed for this element.
	// --------------------------------------------------------------
	virtual void SetCaption(LPCTSTR lpszCaption) = 0;
	// ------------------------------------------------------------
	//
	// Summary:
	//     Gets the caption that is displayed for this element.
	// Returns:
	//     \Returns the caption that is displayed for this element.
	// ------------------------------------------------------------
	virtual CString GetCaption() const = 0;

public:
	BOOL m_bDirty;  // Internally used to tell the flow graph that the element has changed.  This will tell Reposition to handle the element.
protected:
	BOOL m_bSelected;  // Specifies whether the element is selected.  True if selected, False otherwise.
	friend class CXTPFlowGraphSelectedElements;

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPFlowGraphElement)

//}}AFX_CODEJOCK_PRIVATE
#endif
};


#endif //#if !defined(__XTPFLOWGRAPHELEMENT_H__)
