// XTPMarkupContext.cpp: implementation of the CXTPMarkupContext class.
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

#include "stdafx.h"

#include "Common/XTPToolTipContext.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPMarkupRender.h"
#include "Common/XTPImageManager.h"

#include "XTPMarkupObject.h"

#include "XTPMarkupContext.h"

#include "XTPMarkupInputElement.h"
#include "XTPMarkupUIElement.h"
#include "XTPMarkupFrameworkElement.h"
#include "XTPMarkupDecorator.h"
#include "XTPMarkupControl.h"
#include "XTPMarkupPanel.h"
#include "Text/XTPMarkupInline.h"
#include "Text/XTPMarkupSpan.h"
#include "Text/XTPMarkupRun.h"
#include "XTPMarkupBorder.h"
#include "XTPMarkupStackPanel.h"
#include "XTPMarkupGrid.h"
#include "XTPMarkupCanvas.h"
#include "XTPMarkupPage.h"
#include "XTPMarkupShape.h"
#include "XTPMarkupImage.h"
#include "XTPMarkupTextBlock.h"
#include "XTPMarkupButton.h"
#include "XTPMarkupScrollViewer.h"
#include "XTPMarkupResourceDictionary.h"
#include "XTPMarkupThickness.h"
#include "Path/XTPMarkupPath.h"
#include "Path/XTPMarkupPathData.h"

#include "XTPMarkupBuilder.h"
#include "XTPMarkupParser.h"
#include "XTPMarkupDrawingContext.h"
#include "XTPMarkupKeyboardNavigation.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


class CXTPMarkupContext::CXTPMarkupFontArray : public CXTPMarkupTypedSimpleStack<CXTPMarkupFont>
{

};

CXTPMarkupFont::CXTPMarkupFont()
{
	m_pMarkupContext = NULL;
	m_hFont = NULL;
	m_pNextChain = NULL;
}

CXTPMarkupFont::~CXTPMarkupFont()
{
	if (m_pMarkupContext)
	{
		VERIFY(m_pMarkupContext->m_pFonts->Remove(this));
	}
	DeleteObject(m_hFont);
}

//////////////////////////////////////////////////////////////////////////
// CXTPMarkupDelegateMap

class CXTPMarkupDelegateMap
{
public:
	CXTPMarkupDelegateMap();
	~CXTPMarkupDelegateMap();

public:
	void RemoveAll();
	void Add(LPCWSTR lpszDelegate, CXTPMarkupDelegate* pDelegate);
	CXTPMarkupDelegate* Lookup(LPCWSTR lpszDelegate) const;

protected:
	CMap<LPCWSTR, LPCWSTR, CXTPMarkupDelegate*, CXTPMarkupDelegate*> m_mapDelegates;

public:
	LPDISPATCH m_lpHandler;
};


CXTPMarkupDelegateMap::CXTPMarkupDelegateMap()
{
	m_lpHandler = NULL;
}

CXTPMarkupDelegateMap::~CXTPMarkupDelegateMap()
{
	RemoveAll();

	m_lpHandler = NULL;
}

void CXTPMarkupDelegateMap::RemoveAll()
{
	POSITION pos = m_mapDelegates.GetStartPosition();
	while (pos)
	{
		LPCWSTR lpszDelegate;
		CXTPMarkupDelegate* pDelegate;
		m_mapDelegates.GetNextAssoc(pos, lpszDelegate, pDelegate);

		MARKUP_RELEASE(pDelegate);
	}
	m_mapDelegates.RemoveAll();
}

void CXTPMarkupDelegateMap::Add(LPCWSTR lpszDelegate, CXTPMarkupDelegate* pDelegate)
{
	m_mapDelegates.SetAt(lpszDelegate, pDelegate);
}

CXTPMarkupDelegate* CXTPMarkupDelegateMap::Lookup(LPCWSTR lpszDelegate) const
{
	CXTPMarkupDelegate* pDelegate = NULL;
	if (!m_mapDelegates.Lookup(lpszDelegate, pDelegate))
		return NULL;

	return pDelegate;
}

template<> inline BOOL AFXAPI CompareElements(const LPCWSTR* pElement1, const LPCWSTR* pElement2)
{
	return wcscmp(*pElement1, *pElement2) == 0;
}

class CXTPMarkupContext::CInputElementCollection : public CXTPMarkupCollection
{
public:
	CInputElementCollection()
	{
		m_pElementType = MARKUP_TYPE(CXTPMarkupInputElement);
		m_bLogicalParent = FALSE;
	}

public:
	CXTPMarkupInputElement* GetItem(int nIndex) const {
		return (CXTPMarkupInputElement*)m_arrItems[nIndex];
	}
	CXTPMarkupInputElement* GetTailItem() const {
		return (CXTPMarkupInputElement*)m_arrItems[GetCount() - 1];
	}
};

class CXTPMarkupContext::CTooltipContext : public CXTPToolTipContext
{
public:
	CTooltipContext(CXTPMarkupContext* pMarkupContext)
	{
		m_pMarkupContext = pMarkupContext;
	}

	INT_PTR OnToolHitTest(CWnd* /*pWnd*/, CPoint point, TOOLINFO* pToolInfo)
	{
		return m_pMarkupContext->OnToolHitTest(point, pToolInfo);
	}
protected:
	CXTPMarkupContext* m_pMarkupContext;
};

//////////////////////////////////////////////////////////////////////////
// CXTPMarkupContext

CXTPMarkupContext::CXTPMarkupContext()
{
	NONCLIENTMETRICS ncm = { sizeof(NONCLIENTMETRICS) };
	VERIFY(::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0));
	ncm.lfMessageFont.lfCharSet = XTPResourceManager()->GetFontCharset();

	m_dwRef = 1;
	m_logFont = ncm.lfMessageFont;

	m_clrForeground = 0;
	m_pImageManager = NULL;

	m_pMouseOver = NULL;
	m_pMouseCapture = NULL;
	m_pActiveElement = NULL;
	m_hContextWnd = NULL;

	m_pHandlers = NULL;
	m_pDelegates = NULL;

	m_pFonts = NULL;
	m_pKeyboardNavigation = new CXTPMarkupKeyboardNavigation(this);

	m_pToolTipContext = new CTooltipContext(this);
	m_pToolTipContext->SetStyle(xtpToolTipMarkup);

	m_bEnableGdiPlus = TRUE;


	RegisterClasses();

	CXTPMarkupDrawingContext::Register(TRUE);
}


void CXTPMarkupContext::RegisterClasses()
{
	static BOOL bRegistered = FALSE;
	if (bRegistered)
		return;

	bRegistered = TRUE;

	CXTPMarkupType::RegisterAll();

	CXTPMarkupSetter::RegisterType();
	CXTPMarkupFrameworkElement::RegisterType();
	CXTPMarkupUIElement::RegisterType();
	CXTPMarkupTextElement::RegisterType();
	CXTPMarkupBorder::RegisterType();
	CXTPMarkupCanvas::RegisterType();
	CXTPMarkupSolidColorBrush::RegisterType();
	CXTPMarkupGradientStop::RegisterType();
	CXTPMarkupLinearGradientBrush::RegisterType();
	CXTPMarkupColumnDefinition::RegisterType();
	CXTPMarkupRowDefinition::RegisterType();
	CXTPMarkupGrid::RegisterType();
	CXTPMarkupImage::RegisterType();
	CXTPMarkupPanel::RegisterType();
	CXTPMarkupControl::RegisterType();
	CXTPMarkupContentControl::RegisterType();
	CXTPMarkupButtonBase::RegisterType();
	CXTPMarkupToggleButton::RegisterType();
	CXTPMarkupButton::RegisterType();
	CXTPMarkupFrameworkContentElement::RegisterType();
	CXTPMarkupInline::RegisterType();
	CXTPMarkupRun::RegisterType();
	CXTPMarkupHyperlink::RegisterType();
	CXTPMarkupInputElement::RegisterType();
	CXTPMarkupPage::RegisterType();
	CXTPMarkupShape::RegisterType();
	CXTPMarkupPolygon::RegisterType();
	CXTPMarkupPolyline::RegisterType();
	CXTPMarkupLine::RegisterType();
	CXTPMarkupEllipse::RegisterType();
	CXTPMarkupRectangle::RegisterType();
	CXTPMarkupStackPanel::RegisterType();
	CXTPMarkupWrapPanel::RegisterType();
	CXTPMarkupTextBlock::RegisterType();
	CXTPMarkupScrollViewer::RegisterType();
	CXTPMarkupStyle::RegisterType();
	CXTPMarkupDockPanel::RegisterType();
	CXTPMarkupUniformGrid::RegisterType();

	CXTPMarkupPath::RegisterType();
	CXTPMarkupPathData::RegisterType();
}

CXTPMarkupContext::~CXTPMarkupContext()
{
	CMDTARGET_RELEASE(m_pImageManager);

	m_pMouseOver = NULL;
	MARKUP_RELEASE(m_pKeyboardNavigation);

	ASSERT(m_pFonts == NULL || m_pFonts->IsEmpty());

	SAFE_DELETE(m_pHandlers);
	SAFE_DELETE(m_pDelegates);
	SAFE_DELETE(m_pFonts);

	CMDTARGET_RELEASE(m_pToolTipContext);

	ASSERT(m_dwRef <= 1);

	CXTPMarkupDrawingContext::Register(FALSE);
}

CXTPMarkupObject* CXTPMarkupContext::CreateMarkupObject(CXTPMarkupType* pType)
{
	CXTPMarkupObject* pObject = pType->CreateObject();
	if (!pObject)
		return NULL;

	if (pObject->IsKindOf(MARKUP_TYPE(CXTPMarkupInputElement)))
	{
		((CXTPMarkupInputElement*)pObject)->m_pMarkupContext = this;
		AddRef();
	}

	return (CXTPMarkupObject*)pObject;
}

void CXTPMarkupContext::Cleanup()
{
	m_pMouseOver = NULL;
	m_pActiveElement = NULL;

	if (m_pMouseCapture)
	{
		m_pMouseCapture = NULL;
		ReleaseCapture();
	}
}

void CXTPMarkupContext::FinalizeMarkupObject(CXTPMarkupInputElement* pInputElement)
{
	if (pInputElement == m_pMouseOver)
		m_pMouseOver = NULL;

	if (pInputElement == m_pActiveElement)
		m_pActiveElement = NULL;

	if (pInputElement == m_pMouseCapture)
	{
		m_pMouseCapture = NULL;
		ReleaseCapture();
	}

	pInputElement->m_pMarkupContext = NULL;
	Release();
}

CXTPMarkupUIElement* CXTPMarkupContext::Parse(LPCSTR lpszBuffer)
{
	Cleanup();

	if (!lpszBuffer)
		return NULL;

	CXTPMarkupParser sc;
	sc.SetBuffer(lpszBuffer, lpszBuffer + strlen(lpszBuffer));

	CXTPMarkupBuilder builder(this);
	return builder.Parse(&sc);
}

CXTPMarkupUIElement* CXTPMarkupContext::Parse(LPCWSTR lpszBuffer)
{
	Cleanup();

	if (!lpszBuffer)
		return NULL;

	CXTPMarkupParser sc;
	sc.SetBuffer(lpszBuffer, lpszBuffer + wcslen(lpszBuffer));

	CXTPMarkupBuilder builder(this);
	return builder.Parse(&sc);
}

BOOL CXTPMarkupContext::CompareFont(LOGFONT* pLogFont1, LOGFONT* pLogFont2)
{
	if (pLogFont1->lfHeight != pLogFont2->lfHeight)
		return FALSE;

	if (pLogFont1->lfWeight != pLogFont2->lfWeight)
		return FALSE;

	if (pLogFont1->lfItalic != pLogFont2->lfItalic)
		return FALSE;

	if (pLogFont1->lfQuality != pLogFont2->lfQuality)
		return FALSE;

	if (pLogFont1->lfEscapement != pLogFont2->lfEscapement)
		return FALSE;

	if (pLogFont1->lfOrientation != pLogFont2->lfOrientation)
		return FALSE;

	if (_tcscmp(pLogFont1->lfFaceName, pLogFont2->lfFaceName) != 0)
		return FALSE;

	return TRUE;
}

CXTPMarkupFont* CXTPMarkupContext::GetFont(LOGFONT* pLogFont)
{
	if (m_pFonts == NULL)
		m_pFonts = new CXTPMarkupFontArray();

	CXTPMarkupFont* pFont;

	for (pFont = m_pFonts->GetHead(); pFont != NULL; pFont = pFont->m_pNextChain)
	{
		if (CompareFont(&pFont->m_lf, pLogFont))
		{
			pFont->AddRef();
			return pFont;
		}
	}

	pFont = new CXTPMarkupFont();
	m_pFonts->AddTail(pFont);
	pFont->m_lf = *pLogFont;
	pFont->m_hFont = ::CreateFontIndirect(pLogFont);
	pFont->m_pMarkupContext = this;

	return pFont;
}

BOOL CXTPMarkupContext::IsVisualChild(CXTPMarkupObject* pParent, CXTPMarkupObject* pChild) const
{
	while (pChild)
	{
		if (pChild == pParent)
			return TRUE;

		pChild = pChild->GetLogicalParent();
	}
	return FALSE;
}

void CXTPMarkupContext::BuildInputList(CXTPMarkupObject* pUIElement, CInputElementCollection* arrObjects)
{
	CXTPMarkupObject* pParent = pUIElement;
	while (pParent)
	{
		if (pParent->IsKindOf(MARKUP_TYPE(CXTPMarkupInputElement)))
		{
			arrObjects->Add((CXTPMarkupInputElement*)pParent);
			pParent->AddRef();
		}

		pParent = pParent->GetLogicalParent();
	}
}

INT_PTR CXTPMarkupContext::OnToolHitTest(CPoint /*point*/, TOOLINFO* pToolInfo)
{
	CInputElementCollection listMouseOver;
	BuildInputList(m_pMouseOver, &listMouseOver);

	for (int i = 0; i < listMouseOver.GetCount(); i++)
	{
		CXTPMarkupInputElement* pInputElement = listMouseOver.GetItem(i);

		CXTPMarkupObject* pToolTip = pInputElement->GetToolTip();
		if (pToolTip && IsStringObject(pToolTip))
		{
			CXTPToolTipContext::FillInToolInfo(pToolInfo, m_hContextWnd, GetClientBoundingRect(pInputElement),
				(INT_PTR)pInputElement, (LPCWSTR)*((CXTPMarkupString*)pToolTip));
			return pToolInfo->uId;
		}
	}

	return -1;
}

void CXTPMarkupContext::HandleMouseEnter(CXTPMarkupInputElement* pMouseOver, CPoint point)
{
	if (m_pMouseOver == pMouseOver)
		return;

	if (!pMouseOver)
		m_pActiveElement = NULL;

	CInputElementCollection listOldMouseOver;
	CInputElementCollection listNewMouseOver;

	BuildInputList(m_pMouseOver, &listOldMouseOver);
	BuildInputList(pMouseOver, &listNewMouseOver);

	while (listOldMouseOver.GetCount() > 0 && listNewMouseOver.GetCount() > 0 &&
		listOldMouseOver.GetTailItem() == listNewMouseOver.GetTailItem())
	{
		listNewMouseOver.Remove(listNewMouseOver.GetCount() - 1);
		listOldMouseOver.Remove(listOldMouseOver.GetCount() - 1);
	}

	CXTPMarkupMouseEventArgs* eMouseLeaveEventArgs = new CXTPMarkupMouseEventArgs(CXTPMarkupInputElement::m_pMouseLeaveEvent);
	CXTPMarkupMouseEventArgs* eMouseEnterEventArgs = new CXTPMarkupMouseEventArgs(CXTPMarkupInputElement::m_pMouseEnterEvent);

	eMouseLeaveEventArgs->SetSource(m_pMouseOver);
	eMouseLeaveEventArgs->m_hWnd = m_hContextWnd;
	eMouseLeaveEventArgs->m_point = point;

	eMouseEnterEventArgs->SetSource(pMouseOver);
	eMouseEnterEventArgs->m_hWnd = m_hContextWnd;
	eMouseEnterEventArgs->m_point = point;


	m_pMouseOver = pMouseOver;

	int i;
	for (i = 0; i < listOldMouseOver.GetCount(); i++)
	{
		CXTPMarkupInputElement* pInputElement = listOldMouseOver.GetItem(i);

		pInputElement->RaiseEvent(eMouseLeaveEventArgs);
		if (eMouseLeaveEventArgs->IsHandled())
			break;

		pInputElement->OnMouseLeave(eMouseLeaveEventArgs);
		if (eMouseLeaveEventArgs->IsHandled())
			break;
	}

	for (i = 0; i < listNewMouseOver.GetCount(); i++)
	{
		CXTPMarkupInputElement* pInputElement = listNewMouseOver.GetItem(i);

		pInputElement->RaiseEvent(eMouseEnterEventArgs);
		if (eMouseEnterEventArgs->IsHandled())
			break;

		pInputElement->OnMouseEnter(eMouseEnterEventArgs);
		if (eMouseEnterEventArgs->IsHandled())
			break;
	}

	eMouseLeaveEventArgs->Release();
	eMouseEnterEventArgs->Release();
}


void CXTPMarkupContext::HandleMouseMove(CXTPMarkupUIElement* pUIElement, CPoint point)
{
	CXTPMarkupInputElement* pObject = pUIElement->InputHitTest(point);
	if (m_pMouseCapture)
	{
		if ((pObject == NULL) || !IsVisualChild(m_pMouseCapture, pObject))
			pObject = NULL;
		else
			pObject = m_pMouseCapture;
	}

	if (m_pMouseOver != pObject)
	{
		HandleMouseEnter(pObject, point);

		if (m_pMouseOver)
		{
			TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT), TME_LEAVE, m_hContextWnd, 0 };
			_TrackMouseEvent (&tme);

		}

		if (!HandleSetCursor())
		{
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		}
	}

	m_pActiveElement = m_pMouseOver ? pUIElement : NULL;

	if (m_pMouseOver)
	{
		CInputElementCollection listNewMouseOver;
		BuildInputList(m_pMouseOver, &listNewMouseOver);

		CXTPMarkupMouseEventArgs* eMouseMoveEventArgs = new CXTPMarkupMouseEventArgs(CXTPMarkupInputElement::m_pMouseMoveEvent);
		eMouseMoveEventArgs->SetSource(m_pMouseOver);
		eMouseMoveEventArgs->m_hWnd = m_hContextWnd;
		eMouseMoveEventArgs->m_point = point;

		for (int i = 0; i < listNewMouseOver.GetCount(); i++)
		{
			CXTPMarkupInputElement* pInputElement = listNewMouseOver.GetItem(i);

			pInputElement->RaiseEvent(eMouseMoveEventArgs);
			if (eMouseMoveEventArgs->IsHandled())
				break;

			pInputElement->OnMouseMove(eMouseMoveEventArgs);
			if (eMouseMoveEventArgs->IsHandled())
				break;
		}

		eMouseMoveEventArgs->Release();
	}
}

BOOL CXTPMarkupContext::HandleSetCursor()
{
	if (m_pMouseOver)
	{
		CInputElementCollection listNewMouseOver;
		BuildInputList(m_pMouseOver, &listNewMouseOver);

		CXTPMarkupQueryCursorEventArgs eQueryCursorEventArgs;
		eQueryCursorEventArgs.SetSource(m_pMouseOver);

		for (int i = 0; i < listNewMouseOver.GetCount(); i++)
		{
			CXTPMarkupInputElement* pInputElement = listNewMouseOver.GetItem(i);

			pInputElement->OnQueryCursor(&eQueryCursorEventArgs);
			if (!eQueryCursorEventArgs.IsHandled())
				continue;

			if (eQueryCursorEventArgs.m_hCursor)
			{
				::SetCursor(eQueryCursorEventArgs.m_hCursor);
				return TRUE;
			}

			return FALSE;
		}
	}
	return FALSE;
}

BOOL CXTPMarkupContext::HandleMouseWheel(int nDelta)
{
	if (m_pMouseOver)
	{
		CInputElementCollection listNewMouseOver;
		BuildInputList(m_pMouseOver, &listNewMouseOver);

		CXTPMarkupMouseWheelEventArgs eMouseWheelEventArgs;
		eMouseWheelEventArgs.m_nDelta = nDelta;
		eMouseWheelEventArgs.SetSource(m_pMouseOver);

		for (int i = 0; i < listNewMouseOver.GetCount(); i++)
		{
			CXTPMarkupInputElement* pInputElement = listNewMouseOver.GetItem(i);

			pInputElement->OnMouseWheel(&eMouseWheelEventArgs);
			if (eMouseWheelEventArgs.IsHandled())
				return TRUE;
		}
	}
	return FALSE;
}

BOOL CXTPMarkupContext::HandleMouseUpDown(UINT message, WPARAM /*wParam*/, LPARAM lParam)
{
	CXTPMarkupInputElement* pMouseOver = m_pMouseOver;
	if (m_pMouseCapture)
		pMouseOver = m_pMouseCapture;

	if (!pMouseOver)
		return FALSE;

	CInputElementCollection listNewMouseOver;
	BuildInputList(pMouseOver, &listNewMouseOver);

	CXTPMarkupMouseButtonEventArgs* eMouseButtonEventArgs = new CXTPMarkupMouseButtonEventArgs(
		message == WM_LBUTTONDOWN  || message == WM_LBUTTONDBLCLK ? CXTPMarkupInputElement::m_pMouseLeftButtonDownEvent :
		message == WM_LBUTTONUP ? CXTPMarkupInputElement::m_pMouseLeftButtonUpEvent :
		message == WM_RBUTTONDOWN ? CXTPMarkupInputElement::m_pMouseRightButtonDownEvent :
		message == WM_RBUTTONUP ? CXTPMarkupInputElement::m_pMouseRightButtonUpEvent : NULL);

	eMouseButtonEventArgs->SetSource(pMouseOver);
	eMouseButtonEventArgs->m_hWnd = m_hContextWnd;
	eMouseButtonEventArgs->m_point = lParam;

	for (int i = 0; i < listNewMouseOver.GetCount(); i++)
	{
		CXTPMarkupInputElement* pInputElement = listNewMouseOver.GetItem(i);

		pInputElement->RaiseEvent(eMouseButtonEventArgs);
		if (eMouseButtonEventArgs->IsHandled())
			break;

		if (message == WM_LBUTTONDOWN || message == WM_LBUTTONDBLCLK)
		{
			pInputElement->OnMouseLeftButtonDown(eMouseButtonEventArgs);
		}
		else if (message == WM_LBUTTONUP)
		{
			pInputElement->OnMouseLeftButtonUp(eMouseButtonEventArgs);
		}
		else if (message == WM_RBUTTONDOWN)
		{
			pInputElement->OnMouseRightButtonDown(eMouseButtonEventArgs);
		}
		else if (message == WM_RBUTTONUP)
		{
			pInputElement->OnMouseRightButtonUp(eMouseButtonEventArgs);
		}

		if (eMouseButtonEventArgs->IsHandled())
			break;
	}

	BOOL bHandled = eMouseButtonEventArgs->IsHandled();

	eMouseButtonEventArgs->Release();


	return bHandled;
}

void CXTPMarkupContext::CaptureMouse(CXTPMarkupInputElement* pUIElement)
{
	if (m_pMouseCapture == pUIElement)
		return;

	::SetCapture(m_hContextWnd);
	m_pMouseCapture = pUIElement;
}

void CXTPMarkupContext::ReleaseMouseCapture(CXTPMarkupInputElement* /*pUIElement*/)
{
	m_pMouseCapture = NULL;
	::ReleaseCapture();
}

BOOL CXTPMarkupContext::OnWndMsg(CXTPMarkupUIElement* pUIElement, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if (!m_hContextWnd)
		return FALSE;

	if (!pUIElement)
	{
		if (m_pActiveElement)
		{
			HandleMouseEnter(NULL, CPoint(-1, -1));
		}
		return FALSE;
	}

	if (message == WM_MOUSEMOVE)
	{
		HandleMouseMove(pUIElement, CPoint(lParam));
	}

	if (message == WM_DESTROY || message == WM_MOUSELEAVE)
	{
		HandleMouseEnter(NULL, CPoint(-1, -1));
	}

	if ((message >= WM_MOUSEFIRST && message <= WM_MOUSELAST) || (message == WM_MOUSELEAVE))
	{
		m_pToolTipContext->FilterToolTipMessage(CWnd::FromHandle(m_hContextWnd), message, wParam, lParam);
	}

	if (message == WM_SETCURSOR)
	{
		if (HandleSetCursor())
		{
			*pResult = TRUE;
			return TRUE;
		}
	}

	if (message == WM_MOUSEWHEEL)
	{
		if (HandleMouseWheel((short) HIWORD(wParam)))
		{
			*pResult = TRUE;
			return TRUE;
		}
	}

	if (message == WM_LBUTTONDOWN || message == WM_RBUTTONDOWN || message == WM_LBUTTONUP || message == WM_RBUTTONUP || message == WM_LBUTTONDBLCLK)
	{
		if (HandleMouseUpDown(message, wParam, lParam))
			return TRUE;
	}

	if (message == WM_CAPTURECHANGED && m_pMouseCapture)
	{
		CXTPMarkupMouseEventArgs* eLostMouseCaptureEventArgs = new CXTPMarkupMouseEventArgs(CXTPMarkupInputElement::m_pLostMouseCaptureEvent);
		eLostMouseCaptureEventArgs->SetSource(m_pMouseCapture);

		CXTPMarkupInputElement* pInputElement = m_pMouseCapture;

		pInputElement->RaiseEvent(eLostMouseCaptureEventArgs);

		if (!eLostMouseCaptureEventArgs->IsHandled())
		{
			pInputElement->OnLostMouseCapture(eLostMouseCaptureEventArgs);
		}
		eLostMouseCaptureEventArgs->Release();

		m_pMouseCapture = NULL;
	}

	if (message == WM_KEYDOWN || message == WM_SETFOCUS || message == WM_KILLFOCUS)
	{
		if (m_pKeyboardNavigation->OnWndMsg(message, wParam, lParam, pResult))
			return TRUE;
	}



	return FALSE;
}

void CXTPMarkupContext::RaiseEvent(CXTPMarkupObject* pSource, CXTPMarkupRoutedEventArgs* pEventArgs)
{
	if (m_pHandlers) m_pHandlers->Raise(pSource, pEventArgs);
}

void CXTPMarkupContext::AddHandler(CXTPMarkupRoutedEvent* pEvent, CXTPMarkupDelegate* pDelegate)
{
	if (m_pHandlers == NULL)
		m_pHandlers = new CXTPMarkupEventHandlerMap();

	m_pHandlers->Add(pEvent, pDelegate);
}

void CXTPMarkupContext::SetDelegate(LPWSTR lpszHandler, CXTPMarkupDelegate* pDelegate)
{
	if (m_pDelegates == NULL)
		m_pDelegates = new CXTPMarkupDelegateMap();

	m_pDelegates->Add(lpszHandler, pDelegate);
}

CXTPMarkupDelegate* CXTPMarkupContext::LookupDelegate(LPCWSTR lpszDelegate) const
{
	if (m_pDelegates == NULL)
		return NULL;

	CXTPMarkupDelegate* pDelegate = m_pDelegates->Lookup(lpszDelegate);


	return pDelegate;
}

CRect CXTPMarkupContext::GetClientBoundingRect(CXTPMarkupInputElement* pInputElement) const
{
	CRect rc(0, 0, 0, 0);

	if (!pInputElement)
		return rc;

	CXTPMarkupVisual* pVisual = NULL;
	if (pInputElement->IsKindOf(MARKUP_TYPE(CXTPMarkupVisual)))
	{
		pVisual = (CXTPMarkupVisual*)pInputElement;
		rc = pVisual->GetBoundRect();
	}
	else if (pInputElement->IsKindOf(MARKUP_TYPE(CXTPMarkupFrameworkContentElement)))
	{
		pVisual = ((CXTPMarkupFrameworkContentElement*)pInputElement)->GetParent();
		rc = ((CXTPMarkupFrameworkContentElement*)pInputElement)->GetBoundRect();
	}

	if (!pVisual)
		return rc;

	while (pVisual != 0)
	{
		rc.OffsetRect(pVisual->GetVisualOffset());
		pVisual = pVisual->GetVisualParent();
	}

	return rc;
}


void CXTPMarkupContext::OnInvalidateVisual(CXTPMarkupUIElement* pUIElement)
{
	if (m_hContextWnd)
	{
		InvalidateRect(m_hContextWnd, GetClientBoundingRect(pUIElement), FALSE);
		::SendMessage(m_hContextWnd, XTP_WM_MARKUPREDRAW, 0, 0);
	}
}

void CXTPMarkupContext::OnInvalidateArrange(CXTPMarkupUIElement* /*pUIElement*/)
{
	if (m_hContextWnd)
	{
		InvalidateRect(m_hContextWnd, NULL, FALSE);
		::SendMessage(m_hContextWnd, XTP_WM_MARKUPREDRAW, 0, 0);
	}
}

void CXTPMarkupContext::AddRef()
{
	InterlockedIncrement(&m_dwRef);
}

void CXTPMarkupContext::Release()
{
	if (m_dwRef == 0)
		return;

	LONG lResult = InterlockedDecrement(&m_dwRef);
	if (lResult == 0)
	{
		delete this;
	}
}

void CXTPMarkupContext::SetImageManager(CXTPImageManager* pImageManager)
{
	CMDTARGET_RELEASE(m_pImageManager);

	m_pImageManager = pImageManager;
}

CXTPImageManager* CXTPMarkupContext::GetImageManager() const
{
	if (m_pImageManager)
		return m_pImageManager;

	return XTPImageManager();
}

