// XTPMarkupControl.h: interface for the CXTPMarkupControl class.
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
#if !defined(__XTPMARKUPCONTROL_H__)
#define __XTPMARKUPCONTROL_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CXTPMarkupBrush;
class CXTPMarkupBuilder;

//===========================================================================
// Summary: CXTPMarkupControl is CXTPMarkupFrameworkElement derived class. It implements base class for all control elements
//===========================================================================
class _XTP_EXT_CLASS CXTPMarkupControl : public CXTPMarkupFrameworkElement
{
	DECLARE_MARKUPCLASS(CXTPMarkupControl);

public:
	// ---------------------------------------------------------------------
	// Summary:
	//     Call this method to set a value that indicates the thickness of padding space between the boundaries of the content area,
	//     and the content displayed by a control
	// Parameters
	//     nLeft - Left space
	//     nTop - Top space
	//     nRight - Right space
	//     nBottom - Bottom space
	//     nPadding - All borders
	//-----------------------------------------------------------------------
	void SetPadding(int nLeft, int nTop, int nRight, int nBottom);
	void SetPadding(int nPadding);  // <Combine CXTPMarkupControl::SetPadding@int@int@int@int>

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets a value that indicates the thickness of padding space between the boundaries of the content area,
	//     and the content displayed by a control
	// Returns:
	//     CXTPMarkupThickness object contains padding space.
	// See Also: SetPadding
	//-----------------------------------------------------------------------
	CXTPMarkupThickness* GetPadding() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set background of the Control object
	// Parameters: brush - New Brush to be set as background brush
	//-----------------------------------------------------------------------
	void SetBackground(CXTPMarkupBrush* brush);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get background brush of the control object
	// Returns: Pointer to CXTPMarkupBrush object contained current brush of TextBlock's object
	//-----------------------------------------------------------------------
	CXTPMarkupBrush* GetBackground() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set default text brush of the control object
	// Parameters: brush - New Brush to be set as text brush
	//-----------------------------------------------------------------------
	void SetForeground(CXTPMarkupBrush* brush);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get default text brush of the control object
	// Returns: Pointer to CXTPMarkupBrush object contained current text brush of control's object
	//-----------------------------------------------------------------------
	CXTPMarkupBrush* GetForeground() const;


protected:
//{{AFX_CODEJOCK_PRIVATE
	// Implementation
	CSize ArrangeOverride(CSize szFinalSize);
	CSize MeasureOverride(CXTPMarkupDrawingContext* pDC, CSize szAvailableSize);

public:
	static CXTPMarkupDependencyProperty* m_pPaddingProperty;
	static CXTPMarkupDependencyProperty* m_pBackgroundProperty;
	static CXTPMarkupDependencyProperty* m_pForegroundProperty;
//}}AFX_CODEJOCK_PRIVATE

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
public:
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPMarkupControl);

	afx_msg LPDISPATCH OleGetBackground();
	afx_msg void OleSetBackground(LPDISPATCH lpBackground);
	afx_msg LPDISPATCH OleGetForeground();
	afx_msg void OleSetForeground(LPDISPATCH lpBackground);
	afx_msg LPDISPATCH OleGetPadding();
	afx_msg void OleSetPadding(LPDISPATCH lpThickness);

//}}AFX_CODEJOCK_PRIVATE
#endif

};

//===========================================================================
// Summary: CXTPMarkupContentControl is CXTPMarkupControl derived class. It implements base class for all control elements with single child
//===========================================================================
class _XTP_EXT_CLASS CXTPMarkupContentControl : public CXTPMarkupControl
{
	DECLARE_MARKUPCLASS(CXTPMarkupContentControl);
protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPMarkupContentControl object
	//-----------------------------------------------------------------------
	CXTPMarkupContentControl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPMarkupContentControl object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	~CXTPMarkupContentControl();
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Returns Child object of the ContentControl element
	// Returns:
	//     Child CXTPMarkupUIElement object
	// See Also: SetContent
	//-----------------------------------------------------------------------
	CXTPMarkupUIElement* GetContent() const;

	// -------------------------------------------------------------------
	// Summary:
	//     Call this method to set new Child object for the ContentControl
	//     element
	// Parameters:
	//     pContent - New Child element
	// See Also: GetContent
	//-----------------------------------------------------------------------
	void SetContent(CXTPMarkupUIElement* pContent);

protected:
//{{AFX_CODEJOCK_PRIVATE
	// Implementation
	virtual void SetContentObject(CXTPMarkupBuilder* pBuilder, CXTPMarkupObject* pContent);
	virtual BOOL HasContentObject() const;

public:
	virtual int GetVisualChildrenCount() const;
	virtual CXTPMarkupVisual* GetVisualChild(int nIndex) const;

protected:
	static CXTPMarkupDependencyProperty* m_pContentProperty;
//}}AFX_CODEJOCK_PRIVATE


#ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
public:
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPMarkupContentControl);

	afx_msg LPDISPATCH OleGetContent();
	afx_msg void OleSetContent(LPDISPATCH lpContent);
	//}}AFX_CODEJOCK_PRIVATE
#endif
};


AFX_INLINE int CXTPMarkupContentControl::GetVisualChildrenCount() const {
	return GetContent() != NULL ? 1 : 0;
}
AFX_INLINE CXTPMarkupVisual* CXTPMarkupContentControl::GetVisualChild(int nIndex) const {
	return nIndex == 0 ? GetContent() : NULL;
}

#endif // !defined(__XTPMARKUPCONTROL_H__)
