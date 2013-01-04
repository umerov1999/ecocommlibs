// XTPMarkupButton.h: interface for the CXTPMarkupButton class.
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
#if !defined(__XTPMARKUPBUTTON_H__)
#define __XTPMARKUPBUTTON_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CXTPMarkupBrush;
class CXTPMarkupBuilder;
class CXTPWinThemeWrapper;

//===========================================================================
// Summary: CXTPMarkupButtonBase is CXTPMarkupContentControl derived class. It implements base class for Button element
//===========================================================================
class _XTP_EXT_CLASS CXTPMarkupButtonBase : public CXTPMarkupContentControl
{
	DECLARE_MARKUPCLASS(CXTPMarkupButtonBase);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPMarkupButtonBase object
	//-----------------------------------------------------------------------
	CXTPMarkupButtonBase();
	~CXTPMarkupButtonBase();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if button is currently pressed
	// Returns: TRUE if button is pressed by mouse
	//-----------------------------------------------------------------------
	BOOL IsPressed() const;

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Virtual method called when user click button
	// Returns: TRUE if button is pressed by mouse
	//-----------------------------------------------------------------------
	virtual void OnClick();

//{{AFX_CODEJOCK_PRIVATE
// Implementation
protected:
	void OnMouseLeave(CXTPMarkupMouseEventArgs* e);
	void OnMouseEnter(CXTPMarkupMouseEventArgs* e);

	void OnMouseLeftButtonUp(CXTPMarkupMouseButtonEventArgs* e);
	void OnMouseLeftButtonDown(CXTPMarkupMouseButtonEventArgs* e);

	void OnLostMouseCapture(CXTPMarkupMouseEventArgs* e);

protected:
	void SetPressed(BOOL bPressed);


protected:
	BOOL m_bPushed;
	BOOL m_bMouseOver;
	CXTPWinThemeWrapper* m_themeButton;

public:
	static CXTPMarkupDependencyProperty* m_pIsPressedProperty;
	static CXTPMarkupRoutedEvent* m_pClickEvent;

//}}AFX_CODEJOCK_PRIVATE

};


//===========================================================================
// Summary: CXTPMarkupButton is CXTPMarkupButtonBase derived class. It implements Button XAML Tag.
//===========================================================================
class _XTP_EXT_CLASS CXTPMarkupButton : public CXTPMarkupButtonBase
{
	DECLARE_MARKUPCLASS(CXTPMarkupButton);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPMarkupButton object
	//-----------------------------------------------------------------------
	CXTPMarkupButton();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPMarkupButton object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPMarkupButton();

protected:
//{{AFX_CODEJOCK_PRIVATE
	// Implementation
	void OnRender(CXTPMarkupDrawingContext* pDC);
	void OnRenderFocusVisual(CXTPMarkupDrawingContext* drawingContext);
	CSize ArrangeOverride(CSize szFinalSize);
	CSize MeasureOverride(CXTPMarkupDrawingContext* pDC, CSize szAvailableSize);
//}}AFX_CODEJOCK_PRIVATE

};

//===========================================================================
// Summary: CXTPMarkupToggleButton is CXTPMarkupButtonBase derived class. It implements base class for Radio and CheckBox buttons
//===========================================================================
class _XTP_EXT_CLASS CXTPMarkupToggleButton : public CXTPMarkupButtonBase
{
	DECLARE_MARKUPCLASS(CXTPMarkupToggleButton);

public:
	// ----------------------------------------------------------------------
	// Summary:
	//     Call this method to check or uncheck button
	// Parameters:
	//     bChecked - TRUE to check button
	//-----------------------------------------------------------------------
	void SetChecked(BOOL bChecked);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if button is currently checked
	// Returns: TRUE if button is in checked state
	//-----------------------------------------------------------------------
	BOOL GetChecked() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines whether the button supports two or three states.
	// Parameters: bThreeState - TRUE to set 3 state button (Checked, Normal and Indeterminate)
	//-----------------------------------------------------------------------
	void SetThreeState(BOOL bThreeState);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines whether the button supports two or three states.
	// Returns: TRUE if button has 3 states (Checked, Normal and Indeterminate)
	//-----------------------------------------------------------------------
	BOOL IsThreeState() const;


protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This virtual method is called when user click the button
	//-----------------------------------------------------------------------
	virtual void OnClick();

	//-----------------------------------------------------------------------
	// Summary:
	//     This virtual method is called when state is toggled by user
	//-----------------------------------------------------------------------
	virtual void OnToggle();

	//-----------------------------------------------------------------------
	// Summary:
	//     This virtual method is called when user set state to checked
	// Parameters:
	//     e - Mouse pointer information
	//-----------------------------------------------------------------------
	virtual void OnChecked(CXTPMarkupRoutedEventArgs* e);

	//-----------------------------------------------------------------------
	// Summary:
	//     This virtual method is called when user set state to unchecked
	// Parameters:
	//     e - Mouse pointer information
	//-----------------------------------------------------------------------
	virtual void OnUnchecked(CXTPMarkupRoutedEventArgs* e);

	//-----------------------------------------------------------------------
	// Summary:
	//     This virtual method is called when user set state to indeterminate
	// Parameters:
	//     e - Mouse pointer information
	//-----------------------------------------------------------------------
	virtual void OnIndeterminate(CXTPMarkupRoutedEventArgs* e);

//{{AFX_CODEJOCK_PRIVATE
// Implementation
protected:
	static void AFX_CDECL OnIsCheckedChanged(CXTPMarkupObject* d, CXTPMarkupPropertyChangedEventArgs* e);
	static CXTPMarkupObject* AFX_CDECL ConvertIsChecked(CXTPMarkupBuilder* pBuilder, CXTPMarkupObject* pObject);


public:
	static CXTPMarkupDependencyProperty* m_pIsThreeStateProperty;
	static CXTPMarkupDependencyProperty* m_pIsCheckedProperty;
	static CXTPMarkupRoutedEvent* m_pCheckedEvent;
	static CXTPMarkupRoutedEvent* m_pUncheckedEvent;
	static CXTPMarkupRoutedEvent* m_pIndeterminateEvent;
//}}AFX_CODEJOCK_PRIVATE

};

//===========================================================================
// Summary: CXTPMarkupCheckBox is CXTPMarkupToggleButton derived class. It implements CheckBox XAML Tag.
//===========================================================================
class _XTP_EXT_CLASS CXTPMarkupCheckBox : public CXTPMarkupToggleButton
{
	DECLARE_MARKUPCLASS(CXTPMarkupCheckBox);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPMarkupCheckBox object
	//-----------------------------------------------------------------------
	CXTPMarkupCheckBox();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPMarkupCheckBox object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	~CXTPMarkupCheckBox();

protected:
//{{AFX_CODEJOCK_PRIVATE
	// Implementation
	void OnRender(CXTPMarkupDrawingContext* pDC);
	void OnRenderFocusVisual(CXTPMarkupDrawingContext* drawingContext);
	CSize ArrangeOverride(CSize szFinalSize);
	CSize MeasureOverride(CXTPMarkupDrawingContext* pDC, CSize szAvailableSize);
//}}AFX_CODEJOCK_PRIVATE

};

//===========================================================================
// Summary: CXTPMarkupRadioButton is CXTPMarkupToggleButton derived class. It implements RadioButton XAML Tag.
//===========================================================================
class _XTP_EXT_CLASS CXTPMarkupRadioButton : public CXTPMarkupToggleButton
{
	DECLARE_MARKUPCLASS(CXTPMarkupRadioButton);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPMarkupRadioButton object
	//-----------------------------------------------------------------------
	CXTPMarkupRadioButton();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPMarkupRadioButton object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	~CXTPMarkupRadioButton();

protected:
//{{AFX_CODEJOCK_PRIVATE
	// Implementation
	void OnRender(CXTPMarkupDrawingContext* pDC);
	void OnRenderFocusVisual(CXTPMarkupDrawingContext* drawingContext);
	CSize ArrangeOverride(CSize szFinalSize);
	CSize MeasureOverride(CXTPMarkupDrawingContext* pDC, CSize szAvailableSize);

protected:
	void OnToggle();
	void OnChecked(CXTPMarkupRoutedEventArgs* e);

private:
	void UpdateRadioButtonGroup();
//}}AFX_CODEJOCK_PRIVATE

};


AFX_INLINE void CXTPMarkupToggleButton::SetChecked(BOOL bChecked) {
	SetValue(m_pIsCheckedProperty, CXTPMarkupEnum::CreateValue(bChecked));
}
AFX_INLINE BOOL CXTPMarkupToggleButton::GetChecked() const {
	CXTPMarkupEnum* pValue = MARKUP_STATICCAST(CXTPMarkupEnum, GetValue(m_pIsCheckedProperty));
	return pValue != NULL ? (BOOL)(int)*pValue : FALSE;
}
AFX_INLINE BOOL CXTPMarkupToggleButton::IsThreeState() const {
	CXTPMarkupBool* pValue = MARKUP_STATICCAST(CXTPMarkupBool, GetValue(m_pIsThreeStateProperty));
	return pValue != NULL ? (BOOL)*pValue : FALSE;
}
AFX_INLINE void CXTPMarkupToggleButton::SetThreeState(BOOL bThreeState) {
	SetValue(m_pIsThreeStateProperty, CXTPMarkupBool::CreateValue(bThreeState));
}
AFX_INLINE BOOL CXTPMarkupButtonBase::IsPressed() const {
	CXTPMarkupBool* pValue = MARKUP_STATICCAST(CXTPMarkupBool, GetValue(m_pIsPressedProperty));
	return pValue ? (BOOL)*pValue : FALSE;
}
#endif // !defined(__XTPMARKUPBUTTON_H__)
