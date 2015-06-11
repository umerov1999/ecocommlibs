// XTPControlProgress.h : interface for the CXTPControlProgress class.
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
#if !defined(__XTPCONTOLPROGRESS_H__)
#define __XTPCONTOLPROGRESS_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CXTPProgressPaintManager;

//===========================================================================
// Summary:
//     CXTPControlProgress is a CXTPControl derived class.
//     It represents an progress control.
//===========================================================================
class _XTP_EXT_CLASS CXTPControlProgress : public CXTPControl, public CXTPProgressBase
{
	DECLARE_XTP_CONTROL(CXTPControlProgress)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPControlProgress object
	//-----------------------------------------------------------------------
	CXTPControlProgress();

protected:
	//----------------------------------------------------------------------
	// Summary:
	//     This method is called to determine the size of the control.
	// Parameters:
	//     pDC - Pointer to a valid device context
	// Returns:
	//     Size of the control.
	//----------------------------------------------------------------------
	virtual CSize GetSize(CDC* pDC);

	//----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the control.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//----------------------------------------------------------------------
	virtual void Draw(CDC* pDC);

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to redraw the progress bar.
	//-----------------------------------------------------------------------
	virtual void RedrawProgress();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the paint manager of the progress
	//     bar.
	// Returns:
	//     A pointer to a CXTPProgressPaintManager object
	//-----------------------------------------------------------------------
	virtual CXTPProgressPaintManager* GetProgressPaintManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the bounding rectangle of the control.
	// Returns:
	//     The bounding rectangle of the control.
	//-----------------------------------------------------------------------
	virtual CRect GetProgressRect();

	//----------------------------------------------------------------------
	// Summary:
	//     This method is called when the user clicks the control.
	// Parameters:
	//     bKeyboard - TRUE if the control is selected using the keyboard.
	//     pt        - Mouse cursor position.
	//----------------------------------------------------------------------
	virtual void OnClick(BOOL bKeyboard = FALSE, CPoint pt = CPoint(0, 0));


protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method makes a copy of the progress bar.
	// Parameters:
	//     pControl - Control needed to be copied.
	//     bRecursive - TRUE to copy recursively.
	//-----------------------------------------------------------------------
	void Copy(CXTPControl* pControl, BOOL bRecursive = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads or writes this object from or to an archive.
	// Parameters:
	//     pPX - A CXTPPropExchange object to serialize to or from.
	//----------------------------------------------------------------------
	void DoPropExchange(CXTPPropExchange* pPX);

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPControlProgress);
	afx_msg void OleRangeChanged();
	afx_msg void OleSetPos(int nPos);
	afx_msg int OleGetPos();
//}}AFX_CODEJOCK_PRIVATE
#endif
};



AFX_INLINE  int CXTPProgressBase::GetPos() const
{
	return m_nPos;
}


#endif //#if !defined(__XTPCONTOLPROGRESS_H__)
