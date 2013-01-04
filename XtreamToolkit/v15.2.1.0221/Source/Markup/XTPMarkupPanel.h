// XTPMarkupPanel.h: interface for the CXTPMarkupPanel class.
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
#if !defined(__XTPMARKUPPANEL_H__)
#define __XTPMARKUPPANEL_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CXTPMarkupBrush;

//===========================================================================
// Summary: CXTPMarkupUIElementCollection is CXTPMarkupCollection derived class. It implements basic collection of CXTPMarkupUIElement elements
//===========================================================================
class _XTP_EXT_CLASS CXTPMarkupUIElementCollection : public CXTPMarkupCollection
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPMarkupUIElementCollection object
	//-----------------------------------------------------------------------
	CXTPMarkupUIElementCollection();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPMarkupUIElementCollection object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	~CXTPMarkupUIElementCollection();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get CXTPMarkupUIElement element by its index
	// Parameters:
	//     nIndex - Item index to retrieve
	//-----------------------------------------------------------------------
	CXTPMarkupUIElement* GetItem(int nIndex) const;

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This virtual method is called when collection was changed
	//-----------------------------------------------------------------------
	void OnChanged();

};

//===========================================================================
// Summary: CXTPMarkupPanel is CXTPMarkupFrameworkElement derived class.
// It implements base class for all container objects with multiple children
//===========================================================================
class _XTP_EXT_CLASS CXTPMarkupPanel : public CXTPMarkupFrameworkElement
{
	DECLARE_MARKUPCLASS(CXTPMarkupPanel)

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPMarkupPanel object
	//-----------------------------------------------------------------------
	CXTPMarkupPanel();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPMarkupPanel object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPMarkupPanel();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves Children collection
	// Returns: Pointer to CXTPMarkupUIElementCollection collection contained children elements
	//-----------------------------------------------------------------------
	CXTPMarkupUIElementCollection* GetChildren() const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set background of the panel object
	// Parameters: brush - New Brush to be set as background brush
	//-----------------------------------------------------------------------
	void SetBackground(CXTPMarkupBrush* brush);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get background brush of the panel object
	// Returns: Pointer to CXTPMarkupBrush object contained current brush of panel's object
	//-----------------------------------------------------------------------
	CXTPMarkupBrush* GetBackground() const;

protected:

//{{AFX_CODEJOCK_PRIVATE
	// Implementation
	virtual int GetVisualChildrenCount() const;
	virtual CXTPMarkupVisual* GetVisualChild(int nIndex) const;

protected:
	virtual void SetContentObject(CXTPMarkupBuilder* pBuilder, CXTPMarkupObject* pContent);
	virtual BOOL HasContentObject() const;
	virtual void OnRender(CXTPMarkupDrawingContext* drawingContext);
	virtual CXTPMarkupInputElement* InputHitTestOverride(CPoint point) const;

protected:
	CXTPMarkupUIElementCollection* m_pChildren;

public:
	static CXTPMarkupDependencyProperty* m_pBackgroundProperty;

//}}AFX_CODEJOCK_PRIVATE

};


AFX_INLINE CXTPMarkupUIElementCollection* CXTPMarkupPanel::GetChildren() const {
	return m_pChildren;
}
AFX_INLINE int CXTPMarkupPanel::GetVisualChildrenCount() const {
	return m_pChildren->GetCount();
}
AFX_INLINE CXTPMarkupVisual* CXTPMarkupPanel::GetVisualChild(int nIndex) const {
	return m_pChildren->GetItem(nIndex);
}

AFX_INLINE CXTPMarkupUIElement* CXTPMarkupUIElementCollection::GetItem(int nIndex) const {
	return nIndex >= 0 && nIndex < m_arrItems.GetSize() ? (CXTPMarkupUIElement*)m_arrItems[nIndex] : NULL;
}


#endif // !defined(__XTPMARKUPPANEL_H__)
