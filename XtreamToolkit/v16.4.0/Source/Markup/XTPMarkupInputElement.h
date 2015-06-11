// XTPMarkupInputElement.h: interface for the CXTPMarkupInputElement class.
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
#if !defined(__XTPMARKUPINPUT_H__)
#define __XTPMARKUPINPUT_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CXTPMarkupContext;
class CXTPMarkupTriggerCollection;

class _XTP_EXT_CLASS CXTPMarkupRoutedEvent : public CXTPMarkupDependencyProperty
{
public:
	enum RoutingStrategy
	{
		routingDirect = 0,
		routingBubble = 1
	};

protected:
	CXTPMarkupRoutedEvent();

public:
	virtual BOOL IsEvent() const;

public:
	static CXTPMarkupRoutedEvent* RegisterRoutedEvent(LPCWSTR lpszName, RoutingStrategy routingStrategy, CXTPMarkupType* pOwnerType);

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
public:
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPMarkupRoutedEvent);
	afx_msg BSTR OleGetName();

//}}AFX_CODEJOCK_PRIVATE
#endif
};

class _XTP_EXT_CLASS CXTPMarkupRoutedEventArgs : public CXTPMarkupObject
{
public:
	CXTPMarkupRoutedEventArgs();
	CXTPMarkupRoutedEventArgs(CXTPMarkupRoutedEvent* pEvent, CXTPMarkupObject* pSource);

public:
	CXTPMarkupRoutedEvent* GetEvent() const;

	BOOL IsHandled() const;
	void SetHandled();

	CXTPMarkupObject* GetSource() const;
	void SetSource(CXTPMarkupObject* pSource);

protected:
	CXTPMarkupRoutedEvent* m_pEvent;
	CXTPMarkupObject* m_pSource;
	BOOL m_bHandled;

	friend class CXTPMarkupInputElement;

#ifdef _XTP_ACTIVEX
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPMarkupRoutedEventArgs);


	afx_msg BOOL OleGetHandled();
	afx_msg void OleSetHandled(BOOL bHandled);
	afx_msg LPDISPATCH OleGetEvent();
	afx_msg LPDISPATCH OleGetSource();
#endif
};

class _XTP_EXT_CLASS CXTPMarkupDelegate : public CXTPMarkupObject
{
public:
	CXTPMarkupDelegate()
	{
	}

	virtual void Execute(CXTPMarkupObject* /*pSender*/, CXTPMarkupRoutedEventArgs* /*pArgs*/)
	{
		ASSERT(FALSE);
	}
};

template<class T>
class CXTPMarkupClassDelegate : public CXTPMarkupDelegate
{
public:
	typedef void (T::*ROUTEDEVENTHANDLER)(CXTPMarkupObject* pSender, CXTPMarkupRoutedEventArgs* pArgs);

public:
	CXTPMarkupClassDelegate(T* pObject, ROUTEDEVENTHANDLER pHandler)
	{
		m_pObject = pObject;
		m_pHandler = pHandler;
	}

	virtual void Execute(CXTPMarkupObject* pSender, CXTPMarkupRoutedEventArgs* pArgs)
	{
		(m_pObject->*m_pHandler)(pSender, pArgs);

	}
protected:
	T* m_pObject;
	ROUTEDEVENTHANDLER m_pHandler;
};

class CXTPMarkupFunctionDelegate : public CXTPMarkupDelegate
{
public:
	typedef void (*ROUTEDEVENTHANDLER)(CXTPMarkupObject* pSender, CXTPMarkupRoutedEventArgs* pArgs);

public:
	CXTPMarkupFunctionDelegate(ROUTEDEVENTHANDLER pHandler)
	{
		m_pHandler = pHandler;
	}

	virtual void Execute(CXTPMarkupObject* pSender, CXTPMarkupRoutedEventArgs* pArgs)
	{
		(*m_pHandler)(pSender, pArgs);

	}
protected:
	ROUTEDEVENTHANDLER m_pHandler;
};

template<class T, typename EVENTHANDLER>
CXTPMarkupDelegate* CreateMarkupClassDelegate(T* pClass, EVENTHANDLER pfnDelegate)
{
	return new CXTPMarkupClassDelegate<T>(pClass, (CXTPMarkupClassDelegate<T>::ROUTEDEVENTHANDLER)pfnDelegate);
}

template<typename EVENTHANDLER>
CXTPMarkupDelegate* CreateMarkupFunctionDelegate(EVENTHANDLER pfnDelegate)
{
	return new CXTPMarkupFunctionDelegate((CXTPMarkupFunctionDelegate::ROUTEDEVENTHANDLER)pfnDelegate);
}


class _XTP_EXT_CLASS CXTPMarkupEventHandlerMap
{
public:
	CXTPMarkupEventHandlerMap();
	~CXTPMarkupEventHandlerMap();

public:
	void Add(CXTPMarkupRoutedEvent* pEvent, CXTPMarkupDelegate* pDelegate);
	void RemoveAll();
	void Raise(CXTPMarkupObject* pSender, CXTPMarkupRoutedEventArgs* pEventArgs);

protected:

	CMap<CXTPMarkupRoutedEvent*, CXTPMarkupRoutedEvent*, CPtrList*, CPtrList*> m_mapHandlers;
};

class _XTP_EXT_CLASS CXTPMarkupMouseEventArgs : public CXTPMarkupRoutedEventArgs
{
public:
	CXTPMarkupMouseEventArgs(CXTPMarkupRoutedEvent* pEvent);

public:
	CPoint GetPosition(CXTPMarkupInputElement* relativeTo);

public:
	HWND m_hWnd;
	CPoint m_point;
};

class _XTP_EXT_CLASS CXTPMarkupMouseWheelEventArgs : public CXTPMarkupMouseEventArgs
{
public:
	CXTPMarkupMouseWheelEventArgs();

public:
	int m_nDelta;
};

class _XTP_EXT_CLASS CXTPMarkupQueryCursorEventArgs : public CXTPMarkupMouseEventArgs
{
public:
	CXTPMarkupQueryCursorEventArgs();

public:
	HCURSOR m_hCursor;

};

class _XTP_EXT_CLASS CXTPMarkupMouseButtonEventArgs : public CXTPMarkupMouseEventArgs
{
public:
	CXTPMarkupMouseButtonEventArgs(CXTPMarkupRoutedEvent* pEvent);
};

class _XTP_EXT_CLASS CXTPMarkupInputElement : public CXTPMarkupObject
{
	DECLARE_MARKUPCLASS(CXTPMarkupInputElement);

protected:
	CXTPMarkupInputElement();
	~CXTPMarkupInputElement();

public:
	virtual void OnMouseLeave(CXTPMarkupMouseEventArgs* e);
	virtual void OnMouseEnter(CXTPMarkupMouseEventArgs* e);
	virtual void OnMouseMove(CXTPMarkupMouseEventArgs* e);
	virtual void OnQueryCursor(CXTPMarkupQueryCursorEventArgs* e);

	virtual void OnMouseLeftButtonUp(CXTPMarkupMouseButtonEventArgs* e);
	virtual void OnMouseRightButtonUp(CXTPMarkupMouseButtonEventArgs* e);
	virtual void OnMouseLeftButtonDown(CXTPMarkupMouseButtonEventArgs* e);
	virtual void OnMouseRightButtonDown(CXTPMarkupMouseButtonEventArgs* e);

	virtual void OnLostMouseCapture(CXTPMarkupMouseEventArgs* e);
	CXTPMarkupContext* GetMarkupContext() const;

	virtual void OnMouseWheel(CXTPMarkupMouseWheelEventArgs* e);

	void Apply();


public:
	virtual CXTPMarkupInputElement* InputHitTest(CPoint point) const;

	BOOL IsMouseOver() const;
	CXTPMarkupObject* GetToolTip() const;
	void SetToolTip(LPCTSTR lpszTooltip);

	void AddHandler(CXTPMarkupRoutedEvent* pEvent, CXTPMarkupDelegate* pDelegate);

	CXTPMarkupObject* FindResource(const CXTPMarkupObject* pKey) const;

	void Focus();
	BOOL IsKeyboardFocused() const;

	CXTPMarkupString* GetName() const;
	void SetName(LPCTSTR lpszName);

protected:
	void RaiseEvent(CXTPMarkupRoutedEventArgs* pEventArgs);

protected:
	void CaptureMouse();
	void ReleaseMouseCapture();

protected:
	void FireTriggers(CXTPMarkupDependencyProperty* pProperty, CXTPMarkupObject* pNewValue);
	BOOL AddStyleTriggers(CXTPMarkupStyle* pStyle, CXTPMarkupDependencyProperty* pProperty, CXTPMarkupObject* pNewValue);

protected:
	void SetPropertyObject(CXTPMarkupBuilder* pBuilder, CXTPMarkupDependencyProperty* pProperty, CXTPMarkupObject* pValue);
	virtual void OnFinalRelease();
	virtual void OnApply();
	void ApplyTree(CXTPMarkupObject* pChild);

	static void AFX_CDECL OnStylePropertyChanged(CXTPMarkupObject* d, CXTPMarkupPropertyChangedEventArgs* /*e*/);
	static void AFX_CDECL OnCursorPropertyChanged(CXTPMarkupObject* d, CXTPMarkupPropertyChangedEventArgs* e);
	virtual CXTPMarkupObject* GetValueCore(CXTPMarkupDependencyProperty* pProperty) const;


#ifdef _XTP_ACTIVEX
protected:
//{{AFX_CODEJOCK_PRIVATE
public:
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPMarkupInputElement);

	afx_msg long OleGetCursor();
	afx_msg void OleSetCursor(long nValue);
	afx_msg VARIANT OleGetToolTip();
	afx_msg void OleSetToolTip(const VARIANT& va);
	afx_msg BOOL OleGetIsMouseOver();
	afx_msg void OleAddHandler(LPDISPATCH lpEventDisp, LPCOLESTR Method);
	afx_msg LPDISPATCH OleFindName(LPCOLESTR lpszName);
	void OleSetName(LPCTSTR lpszName);
	BSTR OleGetName();

	afx_msg LPDISPATCH OleGetMouseLeaveEvent();
	afx_msg LPDISPATCH OleGetMouseEnterEvent();
	afx_msg LPDISPATCH OleGetMouseLeftButtonUpEvent();
	afx_msg LPDISPATCH OleGetMouseLeftButtonDownEvent();
	afx_msg LPDISPATCH OleGetMouseRightButtonUpEvent();
	afx_msg LPDISPATCH OleGetMouseRightButtonDownEvent();
	afx_msg LPDISPATCH OleGetMouseMoveEvent();
	afx_msg LPDISPATCH OleGetLostMouseCaptureEvent();

//}}AFX_CODEJOCK_PRIVATE
#endif

protected:
	CXTPMarkupEventHandlerMap* m_pHandlers;
	CXTPMarkupContext* m_pMarkupContext;
	CXTPMarkupTriggerCollection* m_pActiveTriggers;
	CXTPMarkupProperties* m_pTriggerProperties;
	CXTPMarkupStyle* m_pStyleCache;

public:
	static CXTPMarkupDependencyProperty* m_pCursorProperty;
	static CXTPMarkupDependencyProperty* m_pIsMouseOverProperty;
	static CXTPMarkupDependencyProperty* m_pStyleProperty;
	static CXTPMarkupDependencyProperty* m_pResourcesProperty;
	static CXTPMarkupDependencyProperty* m_pToolTipProperty;

	static CXTPMarkupDependencyProperty* m_pIsKeyboardFocusedProperty;
	static CXTPMarkupDependencyProperty* m_pIsFocusableProperty;

	static CXTPMarkupRoutedEvent* m_pMouseLeaveEvent;
	static CXTPMarkupRoutedEvent* m_pMouseEnterEvent;
	static CXTPMarkupRoutedEvent* m_pMouseLeftButtonUpEvent;
	static CXTPMarkupRoutedEvent* m_pMouseLeftButtonDownEvent;
	static CXTPMarkupRoutedEvent* m_pMouseRightButtonUpEvent;
	static CXTPMarkupRoutedEvent* m_pMouseRightButtonDownEvent;
	static CXTPMarkupRoutedEvent* m_pMouseMoveEvent;
	static CXTPMarkupRoutedEvent* m_pLostMouseCaptureEvent;

private:
	friend class CXTPMarkupContext;
};


AFX_INLINE CXTPMarkupRoutedEvent* CXTPMarkupRoutedEventArgs::GetEvent() const {
	return m_pEvent;
}
AFX_INLINE BOOL CXTPMarkupRoutedEventArgs::IsHandled() const {
	return m_bHandled;
}
AFX_INLINE void CXTPMarkupRoutedEventArgs::SetHandled() {
	m_bHandled = TRUE;
}
AFX_INLINE BOOL CXTPMarkupRoutedEvent::IsEvent() const {
	return TRUE;
}
AFX_INLINE CXTPMarkupContext* CXTPMarkupInputElement::GetMarkupContext() const {
	return m_pMarkupContext;
}
AFX_INLINE CXTPMarkupObject* CXTPMarkupRoutedEventArgs::GetSource() const {
	return m_pSource;
}
AFX_INLINE void CXTPMarkupRoutedEventArgs::SetSource(CXTPMarkupObject* pSource) {
	m_pSource = pSource;
}
AFX_INLINE BOOL CXTPMarkupInputElement::IsMouseOver() const {
	CXTPMarkupBool* pValue = MARKUP_STATICCAST(CXTPMarkupBool, GetValue(m_pIsMouseOverProperty));
	return pValue ? (BOOL)*pValue : FALSE;
}
AFX_INLINE BOOL CXTPMarkupInputElement::IsKeyboardFocused() const {
	CXTPMarkupBool* pValue = MARKUP_STATICCAST(CXTPMarkupBool, GetValue(m_pIsKeyboardFocusedProperty));
	return pValue ? (BOOL)*pValue : FALSE;
}
AFX_INLINE CXTPMarkupObject* CXTPMarkupInputElement::GetToolTip() const {
	return GetValue(m_pToolTipProperty);
}



#endif // !defined(__XTPMARKUPINPUT_H__)
