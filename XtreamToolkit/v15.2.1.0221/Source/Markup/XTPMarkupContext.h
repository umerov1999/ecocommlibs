// XTPMarkupContext.h: interface for the CXTPMarkupBorder class.
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
#if !defined(__XTPMARKUPCONTEXT_H__)
#define __XTPMARKUPCONTEXT_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPMarkupObject;
class CXTPImageManager;
class CXTPMarkupUIElement;
class CXTPMarkupInputElement;
class CXTPMarkupContext;
class CXTPMarkupEventHandlerMap;
class CXTPMarkupRoutedEventArgs;
class CXTPMarkupRoutedEvent;
class CXTPMarkupDelegate;
class CXTPMarkupFont;
class CXTPMarkupType;
class CXTPMarkupKeyboardNavigation;
class CXTPToolTipContext;

class CXTPMarkupDelegateMap;



//===========================================================================
// Summary: CXTPMarkupContext is standalone class that used to provide common settings for Markup objects.
//===========================================================================
class _XTP_EXT_CLASS CXTPMarkupContext
{
	class CXTPMarkupFontArray;
protected:
	class CInputElementCollection;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPMarkupContext object
	//-----------------------------------------------------------------------
	CXTPMarkupContext();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPMarkupContext object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPMarkupContext();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get default font properties for markup objects
	// Returns: LOGFONT object contained properties for default font
	//-----------------------------------------------------------------------
	LOGFONT* GetDefaultLogFont();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get default text color of text block object
	// Returns: Default text color
	//-----------------------------------------------------------------------
	COLORREF GetDefaultForeground();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set default text color that will be used for TextBlock objects
	// Parameters: clr - Default text color
	//-----------------------------------------------------------------------
	void SetDefaultTextColor(COLORREF clr);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set default font that will be used for TextBlock objects
	// Parameters: font - Font to be set as default
	//-----------------------------------------------------------------------
	void SetDefaultFont(HFONT font);
	void SetDefaultFont(LOGFONT* font);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to create Markup tree from its XML sting
	// Parameters: lpszBuffer - XML Markup string.
	// Returns: Returns toplevel Markup element.
	//-----------------------------------------------------------------------
	CXTPMarkupUIElement* Parse(LPCSTR lpszBuffer);
	CXTPMarkupUIElement* Parse(LPCWSTR lpszBuffer);


	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to connect Markup event with class member or static method delegate
	// Parameters:
	//     pEvent - Event to subscribe
	//     pDelegate - method Delegate
	// Example:
	//      <code>m_wndStatic.AddHandler(CXTPMarkupHyperlink::m_pClickEvent, CreateMarkupClassDelegate(this, &CMarkupSampleDlg::OnHyperlinkClick)); </code>
	//-----------------------------------------------------------------------
	void AddHandler(CXTPMarkupRoutedEvent* pEvent, CXTPMarkupDelegate* pDelegate);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set Delegate for Markup text event
	// Parameters:
	//     lpszDelegate - Text even delegate
	//     pDelegate - method Delegate
	// Example:
	//      <code><TextBlock><Hyperlink Click='OnClick'>FTSE 100</Hyperlink></TextBlock></code>
	//      <code>.... </code>
	//      <code>pPopup->GetMarkupContext()->SetDelegate(L"OnClick", CreateMarkupFunctionDelegate(&HyperlinkClick));</code>
	//-----------------------------------------------------------------------
	void SetDelegate(LPWSTR lpszDelegate, CXTPMarkupDelegate* pDelegate);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to enable/disable GDI+ usage
	// Parameters:
	//     bEnable - TRUE to enable; FALSE to disable GDI+
	//-----------------------------------------------------------------------
	void EnableGdiPlus(BOOL bEnable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to determin if GDI+ is enabled/disabled.
	// Returns: TRUE if enabled; FALSE if GDI+ is disabled.
	//-----------------------------------------------------------------------
	BOOL IsGdiPlusEnabled() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get tooltip context pointer.
	//-----------------------------------------------------------------------
	CXTPToolTipContext* GetToolTipContext() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set isolated image manager for elements inside Markup context.
	// Parameters:
	//     pImageManager - new ImageManager to use
	// Example:
	//     <code>m_pMarkupContext->SetImageManager(new CXTPImageManager());</code>
	//-----------------------------------------------------------------------
	void SetImageManager(CXTPImageManager* pImageManager);


	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve image manager associated with context.
	// Returns:
	//     Pointer to CXTPImageManager that selected in context.
	// Remarks:
	//      Function return XTPImageManager(); by default, call SetImageManager to set isolated images
	//-----------------------------------------------------------------------
	CXTPImageManager* GetImageManager() const;


//{{AFX_CODEJOCK_PRIVATE
	// Internal methods
	CXTPMarkupFont* GetFont(LOGFONT* pLogFont);

	static BOOL AFX_CDECL CompareFont(LOGFONT* pLogFont1, LOGFONT* pLogFont2);

	virtual BOOL OnWndMsg(CXTPMarkupUIElement* pUIElement, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	void RaiseEvent(CXTPMarkupObject* pSource, CXTPMarkupRoutedEventArgs* pEventArgs);

	CXTPMarkupDelegate* LookupDelegate(LPCWSTR lpszDelegate) const;

	void CaptureMouse(CXTPMarkupInputElement* pUIElement);
	void ReleaseMouseCapture(CXTPMarkupInputElement* pUIElement);
	CXTPMarkupInputElement* GetMouseCapture() const;

	CXTPMarkupKeyboardNavigation* GetKeyboardNavigation() const;

	void AddRef();
	void Release();
	INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pToolInfo);

public:
	CRect GetClientBoundingRect(CXTPMarkupInputElement* pUIElement) const;
	BOOL IsVisualChild(CXTPMarkupObject* pParent, CXTPMarkupObject* pChild) const;

private:
	void HandleMouseMove(CXTPMarkupUIElement* pUIElement, CPoint point);
	void HandleMouseEnter(CXTPMarkupInputElement* pUIElement, CPoint point);
	BOOL HandleSetCursor();
	BOOL HandleMouseWheel(int nDelta);
	BOOL HandleMouseUpDown(UINT message, WPARAM wParam, LPARAM lParam);

	void BuildInputList(CXTPMarkupObject* pUIElement, CInputElementCollection* list);

	void Cleanup();


public:
	virtual void OnInvalidateVisual(CXTPMarkupUIElement* pUIElement);
	virtual void OnInvalidateArrange(CXTPMarkupUIElement* pUIElement);

public:
	CXTPMarkupObject* CreateMarkupObject(CXTPMarkupType* pRuntimeClass);
	void FinalizeMarkupObject(CXTPMarkupInputElement* pInputElement);
	static void AFX_CDECL RegisterClasses();

public:
	HWND m_hContextWnd;

protected:
	COLORREF m_clrForeground;
	LOGFONT m_logFont;

	CXTPMarkupInputElement* m_pMouseOver;
	CXTPMarkupInputElement* m_pMouseCapture;
	CXTPMarkupUIElement* m_pActiveElement;

	CXTPMarkupFontArray* m_pFonts;

	CXTPMarkupEventHandlerMap* m_pHandlers;
	CXTPMarkupDelegateMap* m_pDelegates;

	CXTPMarkupKeyboardNavigation* m_pKeyboardNavigation;

	CXTPToolTipContext* m_pToolTipContext;

	friend class CXTPMarkupFont;
	class CTooltipContext;
	long m_dwRef;

	CXTPImageManager* m_pImageManager;

	BOOL m_bEnableGdiPlus;
//}}AFX_CODEJOCK_PRIVATE

	friend class CXTPMarkupInputElement;
};


template<class TYPE>
class CXTPMarkupTypedSimpleStack
{
public:
	CXTPMarkupTypedSimpleStack()
	{
		m_pHead = NULL;
		m_pTail = NULL;
	}

	BOOL IsEmpty() const
	{
		return m_pHead == NULL;
	}

	void AddTail(TYPE* pPart)
	{
		if (m_pTail != NULL)
		{
			m_pTail->m_pNextChain = pPart;
		}
		else
		{
			m_pTail = m_pHead = pPart;
		}
		m_pTail = pPart;
	}

	void AddHead(TYPE* pPart)
	{
		if (m_pHead != NULL)
		{
			pPart->m_pNextChain = m_pHead;
			m_pHead = pPart;
		}
		else
		{
			m_pTail = m_pHead = pPart;
		}
	}

	TYPE* RemoveHead()
	{
		ASSERT(m_pHead);
		if (!m_pHead)
			return NULL;

		TYPE* pHead = m_pHead;

		if (m_pHead == m_pTail)
		{
			m_pHead = m_pTail = NULL;
		}
		else
		{
			m_pHead = pHead->m_pNextChain;
			ASSERT(m_pHead);
		}

		pHead->m_pNextChain = NULL;

		return pHead;
	}

	void RemoveAll()
	{
		m_pHead = m_pTail = NULL;
	}

	TYPE* GetHead() const
	{
		return m_pHead;
	}

	TYPE* GetTail() const
	{
		return m_pTail;
	}

	BOOL Remove(TYPE* p)
	{
		ASSERT(p != NULL);
		if (!p)
			return FALSE;

		if (m_pHead == m_pTail)
		{
			if (p != m_pHead)
				return FALSE;

			p->m_pNextChain = NULL;
			m_pHead = m_pTail = NULL;
			return TRUE;
		}

		if (m_pHead == p)
		{
			m_pHead = p->m_pNextChain;
			p->m_pNextChain = NULL;
			return TRUE;
		}

		TYPE* pTest = m_pHead;
		while (pTest != NULL && pTest->m_pNextChain != p)
			pTest = pTest->m_pNextChain;

		if (pTest != NULL)
		{
			if (m_pTail == p) m_pTail = pTest;
			pTest->m_pNextChain = p->m_pNextChain;
			p->m_pNextChain = NULL;
			return TRUE;
		}
		return FALSE;
	}

protected:
	TYPE* m_pHead;
	TYPE* m_pTail;
};

AFX_INLINE LOGFONT* CXTPMarkupContext::GetDefaultLogFont() {
	return &m_logFont;
}
AFX_INLINE COLORREF CXTPMarkupContext::GetDefaultForeground() {
	return m_clrForeground;
}
AFX_INLINE void CXTPMarkupContext::SetDefaultTextColor(COLORREF clr) {
	m_clrForeground = clr;
}
AFX_INLINE void CXTPMarkupContext::SetDefaultFont(HFONT font) {
	LOGFONT lf;
	GetObject(font, sizeof(LOGFONT), &lf);
	m_logFont = lf;
}
AFX_INLINE void CXTPMarkupContext::SetDefaultFont(LOGFONT* pLogFont) {
	m_logFont = *pLogFont;
}
AFX_INLINE CXTPMarkupKeyboardNavigation* CXTPMarkupContext::GetKeyboardNavigation() const {
	return m_pKeyboardNavigation;
}
AFX_INLINE CXTPMarkupInputElement* CXTPMarkupContext::GetMouseCapture() const {
	return m_pMouseCapture;
}
AFX_INLINE BOOL CXTPMarkupContext::IsGdiPlusEnabled() const {
	return m_bEnableGdiPlus;
}
AFX_INLINE CXTPToolTipContext* CXTPMarkupContext::GetToolTipContext() const {
	return m_pToolTipContext;
}

#define MARKUP_CREATE(object_class, context) (object_class*)context->CreateMarkupObject(MARKUP_TYPE(object_class))

#endif // !defined(__XTPMARKUPCONTEXT_H__)
