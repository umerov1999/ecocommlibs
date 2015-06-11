// XTPMarkupDecorator.h: interface for the CXTPMarkupDecorator class.
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
#if !defined(__XTPMARKUPDECORATOR_H__)
#define __XTPMARKUPDECORATOR_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CXTPMarkupBuilder;

//===========================================================================
// Summary: CXTPMarkupDecorator is CXTPMarkupFrameworkElement derived class. It implements basic framework element with single child
//===========================================================================
class _XTP_EXT_CLASS CXTPMarkupDecorator : public CXTPMarkupFrameworkElement
{
protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPMarkupDecorator object
	//-----------------------------------------------------------------------
	CXTPMarkupDecorator();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPMarkupDecorator object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPMarkupDecorator();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set new Child object for the Decorator element
	// Parameters:
	//     pChild - New Child element
	// See Also: SetChild
	//-----------------------------------------------------------------------
	void SetChild(CXTPMarkupUIElement* pChild);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns Child object of the Decorator element
	// Returns:
	//     Child CXTPMarkupUIElement object
	// See Also: SetChild
	//-----------------------------------------------------------------------
	CXTPMarkupUIElement* GetChild() const;

protected:
//{{AFX_CODEJOCK_PRIVATE

	// Implementation
	virtual void SetContentObject(CXTPMarkupBuilder* pBuilder, CXTPMarkupObject* pContent);
	virtual BOOL HasContentObject() const;
public:
	virtual int GetVisualChildrenCount() const;
	virtual CXTPMarkupVisual* GetVisualChild(int nIndex) const;


protected:
	CXTPMarkupUIElement* m_pChild;
//}}AFX_CODEJOCK_PRIVATE

};

AFX_INLINE int CXTPMarkupDecorator::GetVisualChildrenCount() const {
	return m_pChild != NULL ? 1 : 0;
}
AFX_INLINE CXTPMarkupVisual* CXTPMarkupDecorator::GetVisualChild(int nIndex) const {
	return nIndex == 0 ? m_pChild : NULL;
}


#endif // !defined(__XTPMARKUPDECORATOR_H__)
