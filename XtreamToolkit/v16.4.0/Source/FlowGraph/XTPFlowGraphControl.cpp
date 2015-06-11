// XTPFlowGraphControl.cpp : implementation of the CXTPFlowGraphControl class.
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

#include "stdafx.h"
#include <math.h>

#include "GraphicLibrary/GdiPlus/GdiPlus.h"
#include "GraphicLibrary/XTGraphicUtils.h"
using namespace Gdiplus;

#include "Common/XTPDrawHelpers.h"
#include "Common/XTPPropExchange.h"
#include "Common/XTPTooltipContext.h"
#include "Common/XTPMarkupRender.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPResourceManager.h"
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/ScrollBar/XTPScrollBarCtrl.h"

#include "XTPFlowGraphElement.h"
#include "XTPFlowGraphTools.h"
#include "XTPFlowGraphControl.h"
#include "XTPFlowGraphMessages.h"
#include "XTPFlowGraphPage.h"
#include "XTPFlowGraphPages.h"
#include "XTPFlowGraphNode.h"
#include "XTPFlowGraphNodeCustom.h"
#include "XTPFlowGraphNodeGroup.h"
#include "XTPFlowGraphNodes.h"
#include "XTPFlowGraphPaintManager.h"
#include "XTPFlowGraphDrawContext.h"
#include "XTPFlowGraphDrawContextGdiPlus.h"
#include "XTPFlowGraphConnection.h"
#include "XTPFlowGraphConnections.h"
#include "XTPFlowGraphConnectionPoint.h"
#include "XTPFlowGraphConnectionPoints.h"
#include "XTPFlowGraphSelectedElements.h"
#include "XTPFlowGraphUndoManager.h"
#include "XTPFlowGraphEditItem.h"
#include "XTPFlowGraphImage.h"
#include "XTPFlowGraphPageHistory.h"
#include "XTPFlowGraphSelectionRect.h"

#include "Markup/XTPMarkupObject.h"
#include "Markup/XTPMarkupInputElement.h"
#include "Markup/XTPMarkupUIElement.h"
#include "Markup/XTPMarkupFrameworkElement.h"
#include "Markup/XTPMarkupPanel.h"
#include "Markup/XTPMarkupStackPanel.h"
#include "Markup/XTPMarkupShape.h"
#include "Markup/XTPMarkupImage.h"
#include "Markup/XTPMarkupDecorator.h"
#include "Markup/XTPMarkupBorder.h"
#include "Markup/XTPMarkupTextBlock.h"
#include "Markup/XTPMarkupCanvas.h"

const TCHAR XTPFLOWGRAPH_CLASSNAME[] = _T("XTPFlowGraph");

CMap<XTPFlowGraphCustomProperty, XTPFlowGraphCustomProperty, CXTPMarkupDependencyProperty *,CXTPMarkupDependencyProperty * &> CXTPFlowGraphControl::m_mapCustomProperties;

CXTPFlowGraphControl::CXTPFlowGraphControl()
{
	CXTPFlowGraphDrawContextGdiPlus::Register(TRUE);

	m_pPaintManager = new CXTPFlowGraphPaintManager();
	m_pPaintManager->RefreshMetrics();

	m_pActivePage = NULL;
	m_pPages = new CXTPFlowGraphPages(this);

	m_dMinZoom = 0.1;
	m_dMaxZoom = 5.0;

	m_bAdjustScrollBars = FALSE;

	m_pUndoManager = new CXTPFlowGraphUndoManager();

	m_rcSelectedArea.SetRectEmpty();

	m_pToolTipContext = new CXTPToolTipContext;

	m_nAnimationDelay = 20;

	m_pHistory = new CXTPFlowGraphPageHistory(this);

	m_bAllowMoveNodes = TRUE;
	m_nAllowResizeNodes = xtpFlowGraphResizeBoth;
	m_bAllowModifyConnections = TRUE;

	m_bInAction = FALSE;

	m_nSmoothingMode = xtpFlowGraphSmoothingModeHighQuality;

	m_pMarkupContext = NULL;

	RegisterWindowClass();

	EnableMarkup(TRUE);

	RegisterCustomMarkupProperties();

	SetupDefaultMarkupTemplates();

	m_bCreateScrollbarOnParent = FALSE;
	m_pCustomScrollBars[SB_HORZ] = m_pCustomScrollBars[SB_VERT] = NULL;

	// no metallic theme
	SetPaintManager(new CXTPFlowGraphPaintManager());

	// initialize images support

	m_pImages = new CXTPFlowGraphImages(this);
}

CXTPFlowGraphControl::~CXTPFlowGraphControl()
{
	m_pUndoManager->EnableUndoManager(FALSE);

	if (m_pPaintManager)
	{
		m_pPaintManager->Cleanup();
		m_pPaintManager->InternalRelease();
		m_pPaintManager = NULL;
	}

	if (m_pImages)
	{
		m_pImages->RemoveAll();
		m_pImages->InternalRelease();
		m_pImages = NULL;
	}

	if (m_pActivePage)
	{
		m_pActivePage->InternalRelease();
		m_pActivePage = NULL;
	}

	if (m_pPages)
	{
		m_pPages->RemoveAll();
		m_pPages->InternalRelease();
		m_pPages = NULL;
	}

	if (m_pHistory)
	{
		m_pHistory->InternalRelease();
		m_pHistory = NULL;
	}

	if (m_pUndoManager)
	{
		m_pUndoManager->Clear();
		m_pUndoManager->InternalRelease();
		m_pUndoManager = NULL;
	}

	CMDTARGET_RELEASE(m_pToolTipContext);

	XTPMarkupReleaseContext(m_pMarkupContext);

	CXTPFlowGraphDrawContextGdiPlus::Register(FALSE);

// free-up custom scrollbars

	DeleteCustomScrollBars();
}

void CXTPFlowGraphControl::RegisterCustomMarkupProperties()
{
	if (!m_mapCustomProperties.GetCount())
	{
		m_mapCustomProperties[xtpFlowGraphPropertyConnectionPoint] =
			CXTPMarkupDependencyProperty::RegisterAttached(XAML_MARKUP_PROPERTY_CPNAME, MARKUP_TYPE(CXTPMarkupString), MARKUP_TYPE(CXTPMarkupRectangle));
		m_mapCustomProperties[xtpFlowGraphPropertyConnectionPointType] =
			CXTPMarkupDependencyProperty::RegisterAttached(XAML_MARKUP_PROPERTY_CPTYPE, MARKUP_TYPE(CXTPMarkupInt),    MARKUP_TYPE(CXTPMarkupRectangle));

		// may be not only border actually?
		m_mapCustomProperties[xtpFlowGraphPropertyCanSetColor] =
			CXTPMarkupDependencyProperty::RegisterAttached(XAML_MARKUP_PROPERTY_CANSETCOLOR, MARKUP_TYPE(CXTPMarkupBool), MARKUP_TYPE(CXTPMarkupBorder));

		m_mapCustomProperties[xtpFlowGraphPropertyConnectionPointIn] =
			CXTPMarkupDependencyProperty::RegisterAttached(XAML_MARKUP_PROPERTY_CPOINTS_IN, MARKUP_TYPE(CXTPMarkupString), MARKUP_TYPE(CXTPMarkupCanvas));

		m_mapCustomProperties[xtpFlowGraphPropertyResizable] =
			CXTPMarkupDependencyProperty::RegisterAttached(XAML_MARKUP_PROPERTY_RESIZABLE, MARKUP_TYPE(CXTPMarkupBool), MARKUP_TYPE(CXTPMarkupCanvas));
	}
}

CString CXTPFlowGraphControl::LoadXamlTemplate(LPCTSTR szTemplate)
{
	CString strTemplate;

	if (!XTPResourceManager()->LoadHTML(&strTemplate, szTemplate))
	{
		ASSERT(FALSE);
	}

	return strTemplate;
}

void CXTPFlowGraphControl::SetupDefaultMarkupTemplates()
{
	m_mapDefaultXamlTemplates[xtpFlowGraphTemplateNode]             = LoadXamlTemplate(XAML_MARKUP_RESOURCENAME_NODE);
	m_mapDefaultXamlTemplates[xtpFlowGraphTemplateNodePointNone]    = LoadXamlTemplate(XAML_MARKUP_RESOURCENAME_NODE_ITEM);
	m_mapDefaultXamlTemplates[xtpFlowGraphTemplateNodePointIn]      = LoadXamlTemplate(XAML_MARKUP_RESOURCENAME_NODE_ITEM_IN);
	m_mapDefaultXamlTemplates[xtpFlowGraphTemplateNodePointOut]     = LoadXamlTemplate(XAML_MARKUP_RESOURCENAME_NODE_ITEM_OUT);
	m_mapDefaultXamlTemplates[xtpFlowGraphTemplateNodePointInOut]   = LoadXamlTemplate(XAML_MARKUP_RESOURCENAME_NODE_ITEM_INOUT);
}

BOOL CXTPFlowGraphControl::RegisterWindowClass(HINSTANCE hInstance /*= NULL*/)
{
	return XTPDrawHelpers()->RegisterWndClass(hInstance, XTPFLOWGRAPH_CLASSNAME, CS_DBLCLKS);
}

BOOL CXTPFlowGraphControl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	return CWnd::Create(XTPFLOWGRAPH_CLASSNAME, NULL, dwStyle, rect, pParentWnd, nID);
}

void CXTPFlowGraphControl::SetPaintManager(CXTPFlowGraphPaintManager* pPaintManager)
{
	if (m_pPaintManager)
	{
		m_pPaintManager->Cleanup();
		m_pPaintManager->InternalRelease();
		m_pPaintManager = NULL;
	}

	m_pPaintManager = pPaintManager;
	m_pPaintManager->RefreshMetrics();

	if (m_pActivePage)
		m_pActivePage->OnGraphChanged();
}

IMPLEMENT_DYNAMIC(CXTPFlowGraphControl, CWnd)

BEGIN_MESSAGE_MAP(CXTPFlowGraphControl, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()

CXTPMarkupDependencyProperty *CXTPFlowGraphControl::GetCustomMarkupProperty(XTPFlowGraphCustomProperty propty) const
{
	CXTPMarkupDependencyProperty *pProperty = NULL;
	m_mapCustomProperties.Lookup(propty, pProperty);

	return pProperty;
}

CString CXTPFlowGraphControl::GetDefaultMarkupTemplate(XTPFlowGraphDefaultXAMLTemplate tmpl) const
{
	CString strTemplate;
	m_mapDefaultXamlTemplates.Lookup(tmpl, strTemplate);

	return strTemplate;
}

CXTPFlowGraphDrawContext* CXTPFlowGraphControl::CreateDrawContext(CDC& dc)
{
	CXTPFlowGraphDrawContext* pDC;
	if ((m_nSmoothingMode == xtpFlowGraphSmoothingModeGDI) || (m_nSmoothingMode == xtpFlowGraphSmoothingModeAuto && GetZoomLevel() < GetPaintManager()->m_dMinZoomLevelGDIQuality))
	{
		pDC = new CXTPFlowGraphDrawContext(dc);
	}
	else
	{
		pDC = new CXTPFlowGraphDrawContextGdiPlus(dc);

		if ((m_nSmoothingMode == xtpFlowGraphSmoothingModeAuto && m_bInAction) || (m_nSmoothingMode == xtpFlowGraphSmoothingModeHighSpeed))
		{
			pDC->SetSmoothingMode(0);
		}
	}

	return pDC;
}

BOOL CXTPFlowGraphControl::SaveToFile(LPCTSTR szFile)
{
	// the method does not respect ZOOM level and scrolling info
	// by intent

	// it will not work with controls inside nodes
	// one need to use PrintWindow (XP+) function to achieve that

	CPaintDC dcPaint(this);      // device context for painting
	CRect rc = m_pActivePage->m_rcWorkRect;
	CRect rcClient;
	GetClientRect(rcClient);

	int x = rc.left;
	int y = rc.top;
	OffsetRect(rc, -x, -y);
	rc.bottom += 2*y;
	rc.right += 2*x;

	// determine maximum size
	rc.right = max(rc.right, rcClient.right);
	rc.bottom = max(rc.bottom, rcClient.bottom);

	// print to bitmap

	CDC memDC;
	memDC.CreateCompatibleDC(&dcPaint);

	CBitmap bmp;
	bmp.CreateCompatibleBitmap(&dcPaint, rc.Width(), rc.Height());

	CBitmap* pOldBitmap = memDC.SelectObject(&bmp);

	CXTPFlowGraphDrawContext* pDC = CreateDrawContext(memDC);

	pDC->SetSaveToFile();

	pDC->SetClipRect(rc);

	// draw background
	m_pPaintManager->DrawControlBackground(pDC, this, TRUE, rc);

	// draw contents
	m_pActivePage->OnDraw(pDC);

	dcPaint.BitBlt(0, 0, rc.Width(), rc.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBitmap);

	delete pDC;

	// save to image

	CXTPGraphicUtils::SaveBitmapToFile((HBITMAP)bmp.GetSafeHandle(), szFile);

	return TRUE;
}

void CXTPFlowGraphControl::OnPaint()
{
	CPaintDC dcPaint(this);      // device context for painting
	CRect rc;
	GetClientRect(rc);


	if (m_bReposition)
	{
		Reposition();
	}

	if (m_bmpCache.GetSafeHandle() == 0)
	{
		CDC memDC;
		memDC.CreateCompatibleDC(&dcPaint);

		m_bmpCache.DeleteObject();
		m_bmpCache.CreateCompatibleBitmap(&dcPaint, rc.Width(), rc.Height());

		CBitmap* pOldBitmap = memDC.SelectObject(&m_bmpCache);

		CXTPFlowGraphDrawContext* pDC = CreateDrawContext(memDC);
		pDC->SetClipRect(rc);

		OnDraw(pDC);


		dcPaint.BitBlt(0, 0, rc.right, rc.bottom, &memDC, 0, 0, SRCCOPY);

		memDC.SelectObject(pOldBitmap);

		delete pDC;
	}
	else
	{
		CDC memDC;
		memDC.CreateCompatibleDC(&dcPaint);

		CBitmap* pOldBitmap = memDC.SelectObject(&m_bmpCache);

		dcPaint.BitBlt(0, 0, rc.right, rc.bottom, &memDC, 0, 0, SRCCOPY);

		memDC.SelectObject(pOldBitmap);

	}
}

void CXTPFlowGraphControl::GetClientRect(LPRECT lpRect) const
{
	::GetClientRect(GetSafeHwnd(), lpRect);

	// when custom scrollbars are defined - client rect is smaller!

	if (m_pCustomScrollBars[SB_VERT])
		lpRect->right -= ::GetSystemMetrics(SM_CXVSCROLL);

	if (m_pCustomScrollBars[SB_HORZ])
		lpRect->bottom -= ::GetSystemMetrics(SM_CYHSCROLL);
}

void CXTPFlowGraphControl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	OnGraphChanged();
	Reposition();
	AdjustScrollBars();

	RepositionCustomScrollBars();

}

//#define PERFOMANCE_TRACE 1

void CXTPFlowGraphControl::OnDraw(CXTPFlowGraphDrawContext* pDC)
{
#ifdef PERFOMANCE_TRACE
	_int64 nPerfomanceEnd;
	_int64 nPerfomanceStart;

	QueryPerformanceCounter((LARGE_INTEGER*)&nPerfomanceStart);
#endif

	m_pPaintManager->DrawControlBackground(pDC, this);

	if (!m_pActivePage)
		return;

	pDC->SetWorldTransform(m_pActivePage->m_ptScrollOffset, m_pActivePage->m_dZoomLevel);

	m_pActivePage->OnDraw(pDC);

	pDC->SetWorldTransform(CPoint(0, 0), 1);

	if (!m_rcSelectedArea.IsRectEmpty())
	{
		pDC->DrawSelectionRect(m_rcSelectedArea);
	}

#ifdef PERFOMANCE_TRACE
	QueryPerformanceCounter((LARGE_INTEGER*)&nPerfomanceEnd);
	TRACE(_T("CXTPFlowGraphControl::OnDraw = %i \n"), int(nPerfomanceEnd - nPerfomanceStart));
#endif
}


double CXTPFlowGraphControl::GetZoomLevel() const
{
	if (!m_pActivePage)
		return 1.0;

	return m_pActivePage->m_dZoomLevel;
}

CPoint CXTPFlowGraphControl::GetScrollOffset() const
{
	if (!m_pActivePage)
		return CPoint(0, 0);

	return m_pActivePage->GetScrollOffset();

}

CPoint CXTPFlowGraphControl::TransformPoint(CPoint pt) const
{
	POINT ptTranslated = { pt.x, pt.y};

	HDC hDC = ::GetDC(GetSafeHwnd());

	if (hDC && m_pActivePage)
	{
		::SetMapMode(hDC, MM_ANISOTROPIC);

		::SetViewportExtEx(hDC, int(m_pActivePage->m_dZoomLevel * 96), int(m_pActivePage->m_dZoomLevel * 96), 0);

		::SetWindowExtEx(hDC, int(96), int(96 ), 0);

		CSize m_sizeWinExt, m_sizeVpExt;

		::GetWindowExtEx(hDC, &m_sizeWinExt);
		::GetViewportExtEx(hDC, &m_sizeVpExt);

		::SetViewportOrgEx(hDC, -m_pActivePage->m_ptScrollOffset.x * m_sizeVpExt.cx / m_sizeWinExt.cx, -m_pActivePage->m_ptScrollOffset.y * m_sizeVpExt.cy / m_sizeWinExt.cy, NULL);

		::DPtoLP(hDC, &ptTranslated, 1);

		::ReleaseDC(GetSafeHwnd(), hDC);
	}

	return ptTranslated;
}

void CXTPFlowGraphControl::SetZoomLevel(double dZoomLevel)
{
	if (m_pActivePage)
	{
		m_pActivePage->m_dZoomLevel = dZoomLevel;
		OnGraphChanged();
		Reposition();
		AdjustScrollBars();
	}
}

void CXTPFlowGraphControl::RedrawControl()
{
	m_bmpCache.DeleteObject();

	if (::IsWindow(m_hWnd))
	{
		Invalidate(FALSE);
	}
}

void CXTPFlowGraphControl::CancelLabelEdit()
{
	CXTPFlowGraphEditItem* pWnd = DYNAMIC_DOWNCAST(CXTPFlowGraphEditItem, CWnd::FromHandlePermanent(::GetFocus()));
	if (pWnd)
	{
		::SetFocus(m_hWnd);
	}
}

void CXTPFlowGraphControl::Reposition()
{
	if (!m_pActivePage)
		return;

	if (!m_hWnd)
		return;

	CancelLabelEdit();

	CClientDC dcClient(this);
	CXTPFlowGraphDrawContext* pDC = CreateDrawContext(dcClient);

	CRect rc;
	GetClientRect(&rc);

	m_pActivePage->Reposition(pDC, rc);

	delete pDC;

	AdjustScrollBars();

	m_bReposition = FALSE;
}

void CXTPFlowGraphControl::OnGraphChanged()
{
	m_bmpCache.DeleteObject();

	m_bReposition = TRUE;

	if (::IsWindow(m_hWnd))
	{
		Invalidate(FALSE);
	}
}

void CXTPFlowGraphControl::SetActivePage(CXTPFlowGraphPage* pPage)
{
	if (m_pActivePage == pPage)
		return;

	if (m_pActivePage)
	{
		m_pActivePage->InternalRelease();
		m_pActivePage = NULL;
	}

	m_pActivePage = pPage;

	if (m_pActivePage)
	{
		m_pActivePage->InternalAddRef();
	}

	m_pUndoManager->Clear();

	SendNotifyMessage(XTP_FGN_ACTIVEPAGECHANGED);

	OnGraphChanged();
}

void CXTPFlowGraphControl::AdjustScrollBars()
{
	if (GetSafeHwnd() == 0 || !m_pActivePage)
		return;

	if (m_bAdjustScrollBars)
		return;

	m_bAdjustScrollBars = TRUE;


	// Vertical
	{
		int nPageSize = int(m_pActivePage->m_rcPage.Height() / m_pActivePage->m_dZoomLevel);
		int nWorkTop = m_pActivePage->m_rcWorkRect.top;
		int nWorkBottom =  m_pActivePage->m_rcWorkRect.bottom;
		int nPos = m_pActivePage->m_ptScrollOffset.y;

		if (nWorkTop - nPos >= 0 && nWorkBottom - nPos <= nPageSize)
		{
			EnableScrollBarCtrl(SB_VERT, FALSE);
		}
		else
		{
			SCROLLINFO  si;
			si.cbSize = sizeof(SCROLLINFO);

			si.nPage = nPageSize;

			si.nMin = min(nPos, nWorkTop);
			si.nMax = max(nPos + nPageSize, nWorkBottom);

			si.fMask = SIF_PAGE | SIF_RANGE | SIF_POS;
			si.nPos = nPos;

			EnableScrollBarCtrl(SB_VERT, TRUE);

			SetScrollInfo(SB_VERT, &si);
		}

	}

	// Horizontal
	{
		int nPageSize = int(m_pActivePage->m_rcPage.Width() / m_pActivePage->m_dZoomLevel);
		int nWorkTop = m_pActivePage->m_rcWorkRect.left;
		int nWorkBottom =  m_pActivePage->m_rcWorkRect.right;
		int nPos = m_pActivePage->m_ptScrollOffset.x;

		if (nWorkTop - nPos >= 0 && nWorkBottom - nPos <= nPageSize)
		{
			EnableScrollBarCtrl(SB_HORZ, FALSE);
		}
		else
		{
			SCROLLINFO  si;
			si.cbSize = sizeof(SCROLLINFO);
			si.nPage = nPageSize;

			si.nMin = min(nPos, nWorkTop);
			si.nMax = max(nPos + nPageSize, nWorkBottom);

			si.fMask = SIF_PAGE | SIF_RANGE | SIF_POS;
			si.nPos = nPos;

			EnableScrollBarCtrl(SB_HORZ, TRUE);

			SetScrollInfo(SB_HORZ, &si);
		}
	}


	m_bAdjustScrollBars = FALSE;
}

void CXTPFlowGraphControl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar != NULL && !m_pCustomScrollBars[SB_HORZ])
	{
		CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
		return;
	}

	if (!m_pActivePage)
		return;

	int nCurPos = m_pActivePage->m_ptScrollOffset.x;

	// decide what to do for each different scroll event
	switch (nSBCode)
	{
	case SB_TOP:
		nCurPos = min(m_pActivePage->m_rcWorkRect.left, min(nCurPos, m_pActivePage->m_rcWorkRect.left));
		break;

	case SB_BOTTOM:
		nCurPos = GetScrollLimit(SB_HORZ);
		break;

	case SB_LINEUP:
		nCurPos = nCurPos - m_pActivePage->m_rcPage.Width() / 10;
		break;

	case SB_LINEDOWN:
		nCurPos = nCurPos + m_pActivePage->m_rcPage.Width() / 10;
		break;

	case SB_PAGEUP:
		nCurPos = max(nCurPos - m_pActivePage->m_rcPage.Width(), m_pActivePage->m_rcWorkRect.left);
		break;

	case SB_PAGEDOWN:
		nCurPos = min(nCurPos + m_pActivePage->m_rcPage.Width(), GetScrollLimit(SB_HORZ));
		break;

	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:

		{
			SCROLLINFO si;
			ZeroMemory(&si, sizeof(SCROLLINFO));
			si.cbSize = sizeof(SCROLLINFO);
			si.fMask = SIF_TRACKPOS;

			if (!GetScrollInfo(SB_HORZ, &si))
				return;
			nCurPos = si.nTrackPos;
		}
		break;
	}

	if (nCurPos != m_pActivePage->m_ptScrollOffset.x)
	{
		m_pActivePage->m_ptScrollOffset.x = nCurPos;
		AdjustScrollBars();

		OnGraphChanged();
	}
}

void CXTPFlowGraphControl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar != NULL && !m_pCustomScrollBars[SB_VERT])
	{
		CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
		return;
	}

	if (!m_pActivePage)
		return;

	int nCurPos = m_pActivePage->m_ptScrollOffset.y;

	// decide what to do for each different scroll event
	switch (nSBCode)
	{
	case SB_TOP:
		nCurPos = min(m_pActivePage->m_rcWorkRect.top, min(nCurPos, m_pActivePage->m_rcWorkRect.top));
		break;

	case SB_BOTTOM:
		nCurPos = GetScrollLimit(SB_VERT);
		break;

	case SB_LINEUP:
		nCurPos = nCurPos - m_pActivePage->m_rcPage.Height() / 10;
		break;

	case SB_LINEDOWN:
		nCurPos = nCurPos + m_pActivePage->m_rcPage.Height() / 10;
		break;

	case SB_PAGEUP:
		nCurPos = max(nCurPos - m_pActivePage->m_rcPage.Height(), m_pActivePage->m_rcWorkRect.top);
		break;

	case SB_PAGEDOWN:
		nCurPos = min(nCurPos + m_pActivePage->m_rcPage.Height(), GetScrollLimit(SB_VERT));
		break;

	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:

		{
			SCROLLINFO si;
			ZeroMemory(&si, sizeof(SCROLLINFO));
			si.cbSize = sizeof(SCROLLINFO);
			si.fMask = SIF_TRACKPOS;

			if (!GetScrollInfo(SB_VERT, &si))
				return;
			nCurPos = si.nTrackPos;
		}
		break;
	}

	if (nCurPos != m_pActivePage->m_ptScrollOffset.y)
	{
		m_pActivePage->m_ptScrollOffset.y = nCurPos;
		//AdjustScrollBars();

		OnGraphChanged();
	}
}


BOOL CXTPFlowGraphControl::OnMouseWheel(UINT /*nFlags*/, short zDelta, CPoint pt)
{
	GetCursorPos(&pt);
	ScreenToClient(&pt);

	if (!m_pActivePage)
		return FALSE;

	double dZoomLevel = m_pActivePage->m_dZoomLevel;

	int nZoom = int(m_pActivePage->m_dZoomLevel * 100);

	if (zDelta < 0)
	{
		int nLineDelta = nZoom <= 100 ? 10 : 50;

		nZoom = max(((nZoom / nLineDelta) - 1) * nLineDelta, int(100 * m_dMinZoom));
	}
	else
	{
		int nLineDelta = nZoom < 100 ? 10 : 50;

		nZoom = min(((nZoom / nLineDelta) + 1) * nLineDelta, int(100 * m_dMaxZoom));
	}

	m_pActivePage->m_dZoomLevel = nZoom / 100.0;

	if (fabs(m_pActivePage->m_dZoomLevel - dZoomLevel) < 1e-6)
		return FALSE;


	double x = pt.x / dZoomLevel + m_pActivePage->GetScrollOffset().x;
	double y = pt.y / dZoomLevel + m_pActivePage->GetScrollOffset().y;

	x = (x - pt.x / m_pActivePage->m_dZoomLevel);
	y = (y - pt.y / m_pActivePage->m_dZoomLevel);

	m_pActivePage->m_ptScrollOffset = CPoint(int(x), int(y));


	OnGraphChanged();
	Reposition();
	AdjustScrollBars();

	SendNotifyMessage(XTP_FGN_PAGEZOOMLEVELCHANGED);
	SendNotifyMessage(XTP_FGN_PAGESCROLLOFFSETCHANGED);

	return TRUE;
}

bool CXTPFlowGraphControl::ResetSelectedNodes(CXTPFlowGraphNode *pNodeToSkip /*= NULL*/)
{
	bool bInvalidate = false;

	for (CXTPFlowGraphNode* pNode = m_pActivePage->m_pFirstVisibleNode; pNode != NULL; pNode = pNode->m_pNextVisibleNode)
	{
		if (pNode != pNodeToSkip && pNode->IsSelected())
		{
			if (pNode->IsLocked())
			{
				// need to skip locked node, a connection is currently dragging

				continue;
			}

			pNode->Select(FALSE);

			bInvalidate = true;
		}
	}

	return bInvalidate;
}

void CXTPFlowGraphControl::UpdateActiveNodeAtPoint(CPoint pt, CXTPFlowGraphNode *pNodeToSkip /*= NULL*/)
{
	CXTPFlowGraphNode* pNode = HitTestNode(pt);

	if (pNodeToSkip && pNodeToSkip == pNode)
		return;

	bool bInvalidate = false;

	if (pNode)
	{
		if (!pNode->IsSelected())
		{
			pNode->Select(TRUE);

			bInvalidate = true;

			ResetSelectedNodes(pNode);
		}
	}
	else
	{
		bInvalidate = ResetSelectedNodes();
	}

	if (bInvalidate)
		RedrawControl();
}

CXTPFlowGraphNode* CXTPFlowGraphControl::HitTestNode(CPoint point) const
{
	if (!m_pActivePage)
		return NULL;

	for (CXTPFlowGraphNode* pNode = m_pActivePage->m_pFirstVisibleNode; pNode != NULL; pNode = pNode->m_pNextVisibleNode)
	{
		if (pNode->GetScreenRect().PtInRect(point))
		{
			return pNode;
		}
	}
	return NULL;
}

CXTPFlowGraphConnection* CXTPFlowGraphControl::HitTestConnection(CPoint point) const
{
	if (!m_pActivePage)
		return NULL;

	for (CXTPFlowGraphConnection* pConnection = m_pActivePage->m_pFirstVisibleConnection;
		pConnection != NULL; pConnection = pConnection->m_pNextVisibleConnection)
	{
		if (pConnection->PtInRect(point))
		{
			return pConnection;
		}
	}
	return NULL;
}

CXTPFlowGraphConnectionPoint* CXTPFlowGraphControl::HitTestConnectionArea(CPoint point) const
{
	if (!m_pActivePage)
		return NULL;

	CXTPFlowGraphNode* pNode = NULL;
	int i;

	for (i = m_pActivePage->GetNodes()->GetCount() - 1; i >= 0; i--)
	{
		CXTPFlowGraphNode* p = m_pActivePage->GetNodes()->GetAt(i);
		CRect rc = m_pPaintManager->GetNodeBoundingRectangle(p);
		rc = m_pActivePage->PageToScreen(rc);

		if (rc.PtInRect(point))
		{
			pNode = p;
			break;
		}
	}

	if (!pNode)
		return NULL;

	for (i = 0; i < pNode->GetConnectionPoints()->GetCount(); i++)
	{
		CXTPFlowGraphConnectionPoint* pPoint = pNode->GetConnectionPoints()->GetAt(i);

		if (pPoint->HitTestConnectionArea(point))
			return pPoint;
	}
	return NULL;
}

BOOL CXTPFlowGraphControl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (nHitTest == HTCLIENT)
	{
		SelectionRectHitTest hitTest;
		POINT point;
		GetCursorPos(&point);
		ScreenToClient(&point);
		CXTPFlowGraphNode*pNode = IsPointInSizeRectOfSelectedNode(point, (UINT &)hitTest);

		if (pNode)
		{
			HCURSOR hCursor = AfxGetApp()->LoadStandardCursor( MAKEINTRESOURCE(CXTPFlowGraphSelectionRect::GetCursorBySelectionHitTest(hitTest)) );

			if (hCursor)
			{
				SetCursor(hCursor);

				return TRUE;
			}
		}
	}

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CXTPFlowGraphControl::OnMouseMove(UINT nFlags, CPoint point)
{
	CWnd::OnMouseMove(nFlags, point);

#ifdef _XTP_ACTIVEX
	((COleControl*)GetParent())->FireMouseMove(AxButtonState(), AxShiftState(), point.x, point.y);
#endif
}

CXTPFlowGraphNode * CXTPFlowGraphControl::IsPointInSizeRectOfSelectedNode(CPoint point, UINT &uHitTest) const
{
	point = TransformPoint(point);

	CXTPFlowGraphNode *pNodeReturn = NULL;

	if (m_pActivePage)
	{
		CXTPFlowGraphSelectedElements* pSelection = m_pActivePage->GetSelection();

		if (pSelection->GetCount() == 1) // 1 node selected
		{
			CXTPFlowGraphNode *pNode = DYNAMIC_DOWNCAST(CXTPFlowGraphNode, pSelection->GetAt(0));

			if (pNode && !pNode->IsLocked())
			{
				SelectionRectHitTest hitTest = CXTPFlowGraphSelectionRect::HitTest(pNode->GetSelectionRect(), point);

				if (hitTest != SelectionRectHitTestNone)
				{
					uHitTest = (UINT)hitTest;

					pNodeReturn = pNode;
				}
			}
		}
	}

	return pNodeReturn;
}

void CXTPFlowGraphControl::OnLButtonDown(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonDown(nFlags, point);

	SetFocus();

	if (!m_pActivePage)
		return;


#ifdef _XTP_ACTIVEX
	((COleControl*)GetParent())->FireMouseDown(LEFT_BUTTON, AxShiftState(), point.x, point.y);
#endif

	CXTPFlowGraphSelectedElements* pSelection = m_pActivePage->GetSelection();

	if (m_nAllowResizeNodes)
	{
		UINT nCursorID = 0;
		CXTPFlowGraphNode *pNodeToResize = IsPointInSizeRectOfSelectedNode(point, nCursorID);

		if (pNodeToResize)
		{
			StartResizeNode(pNodeToResize, nCursorID);

			return;
		}
	}

	if (m_bAllowModifyConnections)
	{
		CXTPFlowGraphConnectionPoint* pPoint = HitTestConnectionArea(point);

		if (pPoint && !pPoint->IsLocked())
		{
			StartDragConnectionPoint(pPoint);

			return;
		}
	}

	CXTPFlowGraphNode* pNode = HitTestNode(point);

	if (pNode)
	{
		if (!pNode->IsSelected())
		{
			if (!(GetKeyState(VK_CONTROL) < 0 || GetKeyState(VK_SHIFT) < 0))
				pSelection->Clear();

			pNode->Select();

			if (pSelection->GetCount() > 1)
			{
				for (int j = 0; j < m_pActivePage->GetConnections()->GetCount(); j++)
				{
					CXTPFlowGraphConnection* pConnection =  m_pActivePage->GetConnections()->GetAt(j);
					if (pConnection->GetInputPoint() && pConnection->GetInputPoint()->GetNode()->IsSelected() &&
						pConnection->GetOutputPoint() && pConnection->GetOutputPoint()->GetNode()->IsSelected() &&
						(pConnection->GetInputPoint()->GetNode() == pNode || pConnection->GetOutputPoint()->GetNode() == pNode))
					{
						pSelection->AddSelection(pConnection);
					}
				}
			}
		}
		else if (GetKeyState(VK_CONTROL) < 0)
		{
			pSelection->Remove(pNode);
		}

		m_pActivePage->GetNodes()->MoveToFront(pNode);

		/*if (m_nAllowResizeNodes && !pNode->IsLocked()) // old resizing code
		{
			CRect rcNode(pNode->GetLocation(), pNode->GetSize());
			//rcNode.DeflateRect(4, 4);

			rcNode = m_pActivePage->PageToScreen(rcNode);

			if (!rcNode.PtInRect(point))
			{
				StartResizeNode(pNode);
				return;
			}
		}*/

		StartDragNodes();
		return;
	}

	CXTPFlowGraphConnection* pConnection = HitTestConnection(point);

	if (pConnection)
	{
		if (!pConnection->IsSelected())
		{
			if (GetKeyState(VK_CONTROL) < 0 || GetKeyState(VK_SHIFT) < 0)
				pSelection->AddSelection(pConnection);
			else
				pSelection->SetSelection(pConnection);
		}
		else if (GetKeyState(VK_CONTROL) < 0)
		{
			pSelection->Remove(pConnection);
		}

		XTPFlowGraphConnectorType type = pConnection->GetStyle() != -1 ? (XTPFlowGraphConnectorType)pConnection->GetStyle() : GetPaintManager()->m_nConnectorType;

		if (!IsElbowConnection(type)) // no control points for elbow connection
			StartDragConnection(pConnection);

		return;
	}

	pSelection->Clear();
	StartDragSelection();
}

void CXTPFlowGraphControl::OnLButtonUp(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonUp(nFlags, point);

	SendNotifyMessage(NM_CLICK);


#ifdef _XTP_ACTIVEX
	((COleControl*)GetParent())->FireMouseUp(LEFT_BUTTON, AxShiftState(), point.x, point.y);
#endif
}

void CXTPFlowGraphControl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonDblClk(nFlags, point);

	CXTPFlowGraphConnection* pConnection = HitTestConnection(point);

	if (pConnection)
	{
		if (pConnection->GetControlPoint() != CPoint(-1, -1))
			pConnection->SetControlPoint(CPoint(-1, -1));
	}

#ifdef _XTP_ACTIVEX
	((COleControl*)GetParent())->FireDblClick();
#else
	SendNotifyMessage(NM_DBLCLK);
#endif
}

void CXTPFlowGraphControl::OnMButtonDown(UINT nFlags, CPoint point)
{
	CWnd::OnMButtonDown(nFlags, point);

	if (!m_pActivePage)
		return;

	StartDragScreen();
}



void CXTPFlowGraphControl::OnMoveSelection(CPoint pt)
{
	CRect rc(m_ptStartDrag, pt);
	rc.NormalizeRect();

	ScreenToClient(&rc);

	m_pActivePage->GetSelection()->Clear();

	int i;

	for (i = m_pActivePage->GetNodes()->GetCount() - 1; i >= 0; i--)
	{
		CXTPFlowGraphNode* pNode = m_pActivePage->GetNodes()->GetAt(i);

		if (CRect().IntersectRect(pNode->GetScreenRect(), rc))
		{
			pNode->Select();
		}
	}

	for (i = 0; i < m_pActivePage->GetConnections()->GetCount(); i++)
	{
		CXTPFlowGraphConnection* pConnection =  m_pActivePage->GetConnections()->GetAt(i);
		if (pConnection->GetInputPoint() && pConnection->GetInputPoint()->GetNode()->IsSelected() &&
			pConnection->GetOutputPoint() && pConnection->GetOutputPoint()->GetNode()->IsSelected())
		{
			m_pActivePage->GetSelection()->AddSelection(pConnection);
		}
	}

	m_rcSelectedArea = rc;
	RedrawControl();
}

void CXTPFlowGraphControl::StartDragConnection(CXTPFlowGraphConnection* pConnection)
{
	if (!m_bAllowModifyConnections)
		return;

	// set capture to the window which received this message
	SetCapture();

	CPoint pt(0, 0);
	GetCursorPos(&pt);

	m_ptStartDrag = pt;

	m_pUndoManager->StartGroup();


	// get messages until capture lost or cancelled/accepted
	while (::GetCapture() == m_hWnd)
	{
		MSG msg;

		while (::PeekMessage(&msg, NULL, WM_PAINT, WM_PAINT, PM_NOREMOVE))
		{
			if (!GetMessage(&msg, NULL, WM_PAINT, WM_PAINT))
				break;
			DispatchMessage(&msg);
		}

		if (!::GetMessage(&msg, NULL, 0, 0))
		{
			AfxPostQuitMessage((int)msg.wParam);
			break;
		}

		if (msg.message == WM_LBUTTONUP)
			break;
		else if (msg.message == WM_MOUSEMOVE && pt != msg.pt)
		{
			pt = msg.pt;

			CPoint point(pt);
			ScreenToClient(&point);

			double x = point.x, y = point.y;
			m_pActivePage->ScreenToPage(x, y);

			pConnection->SetControlPoint(CPoint((int)x, (int)y));
		}
		else if (msg.message == WM_KEYDOWN)
		{
			if (msg.wParam == VK_ESCAPE)
			{
				break;
			}
		}
		else
			DispatchMessage(&msg);

	}

	ReleaseCapture();

	RedrawControl();

	m_pUndoManager->EndGroup();
}

void CXTPFlowGraphControl::StartDragConnectionPoint(CXTPFlowGraphConnectionPoint* pPoint)
{
	CPoint pt(0, 0);
	GetCursorPos(&pt);

	m_ptStartDrag = pt;
	BOOL bOutput = (pPoint->GetType() == xtpFlowGraphPointOutput || pPoint->GetType() == xtpFlowGraphPointBoth);
	CXTPFlowGraphConnectionPoint* pOutputPoint = bOutput ? pPoint : NULL;

	m_pUndoManager->StartGroup();

	if (!pOutputPoint)
	{
		int j = -1;
		for (int i = m_pActivePage->GetConnections()->GetCount() - 1; i >= 0; i--)
		{
			CXTPFlowGraphConnection* pConnection = m_pActivePage->GetConnections()->GetAt(i);

			if (pConnection->GetInputPoint() == pPoint && !pPoint->IsLocked())
			{
				if (pConnection->IsSelected())
				{
					pOutputPoint = pConnection->GetOutputPoint();
					m_pActivePage->GetConnections()->RemoveAt(i);
					break;
				}
				else if (j == -1)
				{
					j = i;
				}
			}
		}

		if (pOutputPoint == NULL && j != -1)
		{
			pOutputPoint = m_pActivePage->GetConnections()->GetAt(j)->GetOutputPoint();
			m_pActivePage->GetConnections()->RemoveAt(j);
		}

	}

	UpdateWindow();

	if (pOutputPoint == NULL)
	{
		m_pUndoManager->EndGroup();
		return;
	}

	if (pOutputPoint->GetMaxConnections() >= 0 && pOutputPoint->GetConnectionsCount() >= pOutputPoint->GetMaxConnections())
	{
		m_pUndoManager->EndGroup();
		return;
	}

	m_bInAction = TRUE;

	CXTPFlowGraphConnection* pDragConnection = new CXTPFlowGraphConnection();
	pDragConnection->SetOutputPoint(pOutputPoint);

	m_pActivePage->GetConnections()->AddConnection(pDragConnection);

	m_pActivePage->GetSelection()->Clear();
	m_pActivePage->GetSelection()->AddSelection(pDragConnection);

	ScreenToClient(&pt);
	pDragConnection->m_ptInputPoint = m_pActivePage->ScreenToPage(pt);

	CXTPFlowGraphConnectionPoint* pConnectionPoint = HitTestConnectionArea(pt);
	CXTPFlowGraphConnectionPoint* pInputConnectionPoint = NULL;

	if (pConnectionPoint && (pConnectionPoint->GetType() == xtpFlowGraphPointInput || pConnectionPoint->GetType() == xtpFlowGraphPointBoth) &&
		pConnectionPoint->GetNode() != pDragConnection->GetOutputPoint()->GetNode())
	{
		pInputConnectionPoint = pConnectionPoint;
		m_pActivePage->GetSelection()->AddSelection(pInputConnectionPoint);
	}

	m_pActivePage->GetSelection()->AddSelection(pDragConnection->GetOutputPoint());

	if (GetPaintManager()->m_bAlwaysDrawConnectionPoints == FALSE)
	{
		// the starting node must be locked not to reset its state in OnMouseMove
		// during the drawing

		pOutputPoint->GetNode()->Select(TRUE);

		pOutputPoint->GetNode()->SetLocked(TRUE);
	}

	// set capture to the window which received this message
	SetCapture();

	BOOL bAccept = FALSE;

	// get messages until capture lost or canceled/accepted
	while (::GetCapture() == m_hWnd)
	{
		MSG msg;

		while (::PeekMessage(&msg, NULL, WM_PAINT, WM_PAINT, PM_NOREMOVE))
		{
			if (!GetMessage(&msg, NULL, WM_PAINT, WM_PAINT))
				break;
			DispatchMessage(&msg);
		}

		if (!::GetMessage(&msg, NULL, 0, 0))
		{
			AfxPostQuitMessage((int)msg.wParam);
			break;
		}

		if (msg.message == WM_LBUTTONUP)
		{
			bAccept = TRUE;
			break;
		}
		else if (msg.message == WM_MOUSEMOVE && pt != msg.pt)
		{
			pt = msg.pt;

			CPoint point(pt);
			ScreenToClient(&point);

			if (GetPaintManager()->m_bAlwaysDrawConnectionPoints == FALSE)
			{
				UpdateActiveNodeAtPoint(point, pDragConnection->GetOutputNode());
			}

			pDragConnection->m_ptInputPoint = m_pActivePage->ScreenToPage(point);
			pDragConnection->OnGraphChanged();

			CXTPFlowGraphConnectionPoint* pConnectionPoint = HitTestConnectionArea(point);

			if (pConnectionPoint && (pConnectionPoint->GetType() == xtpFlowGraphPointInput || pConnectionPoint->GetType() == xtpFlowGraphPointBoth) && !pConnectionPoint->IsLocked() &&
				pConnectionPoint->GetNode() != pDragConnection->GetOutputPoint()->GetNode())
			{
				if ((pConnectionPoint->GetMaxConnections() == -1 || pConnectionPoint->GetConnectionsCount() < pConnectionPoint->GetMaxConnections())
					&& pInputConnectionPoint != pConnectionPoint)
				{
					XTP_NM_FLOWGRAPH_CONNECTIONCHANGING cc;
					cc.pConnection = pDragConnection;
					cc.pInputConnectionPoint = pConnectionPoint;

					if (SendNotifyMessage(XTP_FGN_CONNECTIONCHANGING, &cc.hdr) != -1)
					{
						m_pActivePage->GetSelection()->Remove(pInputConnectionPoint);

						pInputConnectionPoint = pConnectionPoint;
						m_pActivePage->GetSelection()->AddSelection(pInputConnectionPoint);
					}
				}
			}
			else
			{
				if (pInputConnectionPoint != NULL)
				{
					m_pActivePage->GetSelection()->Remove(pInputConnectionPoint);
					pInputConnectionPoint = NULL;
				}
			}


			RedrawControl();
		}
		else if (msg.message == WM_KEYDOWN)
		{
			if (msg.wParam == VK_ESCAPE)
			{
				break;
			}
		}
		else
			DispatchMessage(&msg);

	}

	ReleaseCapture();

	if (GetPaintManager()->m_bAlwaysDrawConnectionPoints == FALSE)
	{
		pOutputPoint->GetNode()->SetLocked(FALSE); // need to unlock the node

		pOutputPoint->GetNode()->Select(FALSE);

		ResetSelectedNodes();
	}

	m_pActivePage->GetSelection()->Remove(pDragConnection->GetOutputPoint());

	if (bAccept && pInputConnectionPoint)
	{
		pDragConnection->SetInputPoint(pInputConnectionPoint);
	}

	if (pInputConnectionPoint != NULL)
	{
		m_pActivePage->GetSelection()->Remove(pInputConnectionPoint);
		pInputConnectionPoint = NULL;
	}

	if (pDragConnection->GetInputPoint() == NULL)
	{
		m_pActivePage->GetConnections()->Remove(pDragConnection);
	}

	m_pUndoManager->EndGroup();

	m_bInAction = FALSE;

	RedrawControl();
}


void CXTPFlowGraphControl::StartDragSelection()
{
	// set capture to the window which received this message
	SetCapture();

	CPoint pt(0, 0);
	GetCursorPos(&pt);

	m_ptStartDrag = pt;

	BOOL bButtonUp = FALSE;


	// get messages until capture lost or cancelled/accepted
	while (::GetCapture() == m_hWnd)
	{
		MSG msg;

		while (::PeekMessage(&msg, NULL, WM_PAINT, WM_PAINT, PM_NOREMOVE))
		{
			if (!GetMessage(&msg, NULL, WM_PAINT, WM_PAINT))
				break;
			DispatchMessage(&msg);
		}

		if (!::GetMessage(&msg, NULL, 0, 0))
		{
			AfxPostQuitMessage((int)msg.wParam);
			break;
		}

		if (msg.message == WM_LBUTTONUP)
		{
			bButtonUp = TRUE;
			break;
		}
		else if (msg.message == WM_MOUSEMOVE && pt != msg.pt)
		{
			pt = msg.pt;
			OnMoveSelection(pt);
		}
		else if (msg.message == WM_KEYDOWN)
		{
			if (msg.wParam == VK_ESCAPE)
			{
				break;
			}
		}
		else
			DispatchMessage(&msg);

	}

	ReleaseCapture();

	m_rcSelectedArea.SetRectEmpty();
	RedrawControl();

#ifdef _XTP_ACTIVEX
	if (bButtonUp)
	{
		CPoint point(0, 0);
		GetCursorPos(&point);
		ScreenToClient(&point);

		((COleControl*)GetParent())->FireMouseUp(LEFT_BUTTON, AxShiftState(), point.x, point.y);
	}
#endif
}

void CXTPFlowGraphControl::StartResizeNode(CXTPFlowGraphNode* pNode, UINT uhitTest)
{
	CPoint pt(0, 0);
	GetCursorPos(&pt);

	m_ptStartDrag = pt;

	CRect rcBorders(pNode->GetLocation(), pNode->GetSize());

	rcBorders = m_pActivePage->PageToScreen(&rcBorders);
	ClientToScreen(&rcBorders);

	CRect rcOrigin(pNode->GetLocation(), pNode->GetSize());

	m_pUndoManager->StartGroup();

	// set capture to the window which received this message
	SetCapture();

	SelectionRectHitTest hitTest = (SelectionRectHitTest)uhitTest;

	// get messages until capture lost or cancelled/accepted
	while (::GetCapture() == m_hWnd)
	{
		MSG msg;

		while (::PeekMessage(&msg, NULL, WM_PAINT, WM_PAINT, PM_NOREMOVE))
		{
			if (!GetMessage(&msg, NULL, WM_PAINT, WM_PAINT))
				break;
			DispatchMessage(&msg);
		}

		if (!::GetMessage(&msg, NULL, 0, 0))
		{
			AfxPostQuitMessage((int)msg.wParam);
			break;
		}

		if (msg.message == WM_LBUTTONUP)
			break;
		else if (msg.message == WM_MOUSEMOVE && pt != msg.pt)
		{
			CPoint ptOffset(int(1 / m_pActivePage->m_dZoomLevel * (msg.pt.x - m_ptStartDrag.x)), int(1 / m_pActivePage->m_dZoomLevel * (msg.pt.y - m_ptStartDrag.y)));

			CRect rcRect(rcOrigin);

			CSize szMinSize = pNode->GetMinSize();

			CXTPFlowGraphSelectionRect::ResizeRect(rcRect, szMinSize, ptOffset, hitTest);

			CSize szNodeSize = rcRect.Size();

			szNodeSize.cx = max(rcRect.Width(), szMinSize.cx);
			szNodeSize.cy = max(rcRect.Height(), szMinSize.cy);

			pNode->SetLocation(rcRect.TopLeft());
			pNode->SetSize(szNodeSize);

			UpdateWindow();

			pt = msg.pt;
		}
		else if (msg.message == WM_KEYDOWN)
		{
			if (msg.wParam == VK_ESCAPE)
			{
				break;
			}
		}
		else
			DispatchMessage(&msg);

	}

	ReleaseCapture();

	m_pUndoManager->EndGroup();
}

void CXTPFlowGraphControl::StartDragNodes()
{
	if (!m_bAllowMoveNodes)
		return;

	m_arrDragNodes.RemoveAll();

	for (int i = 0; i < m_pActivePage->GetSelection()->GetCount(); i++)
	{
		CXTPFlowGraphNode* pNode = DYNAMIC_DOWNCAST(CXTPFlowGraphNode, m_pActivePage->GetSelection()->GetAt(i));
		if (pNode)
		{
			if (!pNode->IsLocked())
			{
				DRAGNODE dn;
				dn.pElement = pNode;
				dn.bNode = TRUE;
				dn.ptOrigin = pNode->GetLocation();
				m_arrDragNodes.Add(dn);
			}
		}
		else
		{
			CXTPFlowGraphConnection* pConnection = DYNAMIC_DOWNCAST(CXTPFlowGraphConnection, m_pActivePage->GetSelection()->GetAt(i));
			if (pConnection && pConnection->GetControlPoint() != CPoint(-1, -1))
			{
				DRAGNODE dn;
				dn.pElement = pConnection;
				dn.bNode = FALSE;
				dn.ptOrigin = pConnection->GetControlPoint();
				m_arrDragNodes.Add(dn);
			}
		}
	}

	if (m_arrDragNodes.GetSize() == 0)
		return;

	m_pUndoManager->StartGroup();

	m_bInAction = TRUE;

	// set capture to the window which received this message
	SetCapture();

	CPoint pt(0, 0);
	GetCursorPos(&pt);

	m_ptStartDrag = pt;

	::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));

	// get messages until capture lost or cancelled/accepted
	while (::GetCapture() == m_hWnd)
	{
		MSG msg;

		while (::PeekMessage(&msg, NULL, WM_PAINT, WM_PAINT, PM_NOREMOVE))
		{
			if (!GetMessage(&msg, NULL, WM_PAINT, WM_PAINT))
				break;
			DispatchMessage(&msg);
		}

		if (!::GetMessage(&msg, NULL, 0, 0))
		{
			AfxPostQuitMessage((int)msg.wParam);
			break;
		}

		if (msg.message == WM_LBUTTONUP)
			break;
		else if (msg.message == WM_MOUSEMOVE && pt != msg.pt)
		{
			CPoint ptOffset(int(1 / m_pActivePage->m_dZoomLevel * (msg.pt.x - m_ptStartDrag.x)), int(1 / m_pActivePage->m_dZoomLevel * (msg.pt.y - m_ptStartDrag.y)));

			for (int i = 0; i < m_arrDragNodes.GetSize(); i++)
			{
				DRAGNODE& dn = m_arrDragNodes[i];

				if (dn.bNode)
				{
					((CXTPFlowGraphNode*)dn.pElement)->SetLocation(CPoint(dn.ptOrigin.x + ptOffset.x, dn.ptOrigin.y + ptOffset.y));

				}
				else
				{
					((CXTPFlowGraphConnection*)dn.pElement)->SetControlPoint(CPoint(dn.ptOrigin.x + ptOffset.x, dn.ptOrigin.y + ptOffset.y));
				}

			}
			UpdateWindow();

			pt = msg.pt;
		}
		else if (msg.message == WM_KEYDOWN)
		{
			if (msg.wParam == VK_ESCAPE)
			{
				break;
			}
		}
		else
			DispatchMessage(&msg);

	}

	ReleaseCapture();

	m_pUndoManager->EndGroup();

	m_bInAction = FALSE;
	RedrawControl();
}


void CXTPFlowGraphControl::StartDragScreen()
{
	m_bInAction = TRUE;

	// set capture to the window which received this message
	SetCapture();

	CPoint pt(0, 0);
	GetCursorPos(&pt);

	m_ptStartDrag = pt;

	CPoint ptOrigin(m_pActivePage->GetScrollOffset());


	::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));


	// get messages until capture lost or cancelled/accepted
	while (::GetCapture() == m_hWnd)
	{
		MSG msg;

		while (::PeekMessage(&msg, NULL, WM_PAINT, WM_PAINT, PM_NOREMOVE))
		{
			if (!GetMessage(&msg, NULL, WM_PAINT, WM_PAINT))
				break;
			DispatchMessage(&msg);
		}

		if (!::GetMessage(&msg, NULL, 0, 0))
		{
			AfxPostQuitMessage((int)msg.wParam);
			break;
		}

		if (msg.message == WM_LBUTTONUP || msg.message == WM_MBUTTONUP)
			break;
		else if (msg.message == WM_MOUSEMOVE && pt != msg.pt)
		{
			CPoint ptOffset(int(1.0 / m_pActivePage->m_dZoomLevel * (msg.pt.x - m_ptStartDrag.x)), int(1.0 / m_pActivePage->m_dZoomLevel * (msg.pt.y - m_ptStartDrag.y)));

			m_pActivePage->m_ptScrollOffset = CPoint(ptOrigin.x - ptOffset.x, ptOrigin.y - ptOffset.y);
			SendNotifyMessage(XTP_FGN_PAGESCROLLOFFSETCHANGED);

			OnGraphChanged();
			UpdateWindow();

			pt = msg.pt;
		}
		else if (msg.message == WM_KEYDOWN)
		{
			if (msg.wParam == VK_ESCAPE)
			{
				break;
			}
		}
		else
			DispatchMessage(&msg);

	}

	ReleaseCapture();

	m_bInAction = FALSE;
	RedrawControl();
}


void CXTPFlowGraphControl::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPPropExchangeSection secPages(pPX->GetSection(_T("Pages")));
	m_pPages->DoPropExchange(&secPages);

	if (pPX->IsLoading())
	{
		m_pUndoManager->Clear();
		m_pHistory->Clear();
	}
}

void CXTPFlowGraphControl::ZoomToSelection()
{
	if (!m_pActivePage)
		return;

	CRect rcWorkRect(0, 0, 0, 0);

	for (int i = 0; i < m_pActivePage->GetSelection()->GetCount(); i++)
	{
		CRect rc;

		if (CXTPFlowGraphNode* pNode = DYNAMIC_DOWNCAST(CXTPFlowGraphNode, m_pActivePage->GetSelection()->GetAt(i)))
		{
			rc = GetPaintManager()->GetNodeBoundingRectangle(pNode);
		}
		else if (CXTPFlowGraphConnection* pConnection = DYNAMIC_DOWNCAST(CXTPFlowGraphConnection, m_pActivePage->GetSelection()->GetAt(i)))
		{
			rc = pConnection->GetBoundingRect();
		}
		else
		{
			continue;
		}

		if (rcWorkRect.IsRectEmpty())
			rcWorkRect = rc;

		rcWorkRect.right = max(rcWorkRect.right, rc.right);
		rcWorkRect.bottom = max(rcWorkRect.bottom, rc.bottom);

		rcWorkRect.left = min(rcWorkRect.left, rc.left);
		rcWorkRect.top = min(rcWorkRect.top, rc.top);
	}
	if (rcWorkRect.IsRectEmpty())
		return;

	ZoomToRect(rcWorkRect);
}

void CXTPFlowGraphControl::ZoomFitToWindow()
{
	if (!m_pActivePage)
		return;

	ZoomToRect(m_pActivePage->m_rcWorkRect);
}

void CXTPFlowGraphControl::AnimatePageTransition(CXTPFlowGraphNode* pNodeFrom, CXTPFlowGraphNode* pNodeTo)
{
	CXTPFlowGraphPage* pPageFrom = pNodeFrom->GetPage();
	ASSERT (pPageFrom == m_pActivePage);

	CXTPFlowGraphPage* pPageTo = pNodeTo->GetPage();

	CPoint ptScrollOffsetToDest;
	double dZoomLevelToDest;


	if (pPageTo->m_rcWorkRect.IsRectEmpty())
	{
		CClientDC dcClient(this);
		CXTPFlowGraphDrawContext*  pDC = CreateDrawContext(dcClient);

		CRect rc;
		GetClientRect(&rc);

		pPageTo->Reposition(pDC, rc);
		GetZoomParam(pPageTo->m_rcWorkRect, ptScrollOffsetToDest, dZoomLevelToDest);

		delete pDC;
	}
	else
	{
		ptScrollOffsetToDest = pPageTo->m_ptScrollOffset;
		dZoomLevelToDest = pPageTo->m_dZoomLevel;
	}

	CRect rcNodeTo(pNodeTo->GetLocation(), pNodeTo->GetSize());

	CRect rcNodeFrom(pNodeFrom->GetLocation(), pNodeFrom->GetSize());

	CPoint ptScrollOffsetFromSrc = pPageFrom->m_ptScrollOffset;
	double dZoomLevelFromSrc = pPageFrom->m_dZoomLevel;

	CPoint ptScrollOffsetFromDest;
	double dZoomLevelFromDest;
	GetZoomParam(rcNodeFrom, ptScrollOffsetFromDest, dZoomLevelFromDest);

	CPoint ptScrollOffsetToSrc;
	double dZoomLevelToSrc;
	GetZoomParam(rcNodeTo, ptScrollOffsetToSrc, dZoomLevelToSrc);


	SetActivePage(pPageTo);

	double xFrom = ptScrollOffsetToSrc.x * dZoomLevelToSrc;
	double yFrom = ptScrollOffsetToSrc.y * dZoomLevelToSrc;

	double xTo = ptScrollOffsetToDest.x * dZoomLevelToDest;
	double yTo = ptScrollOffsetToDest.y * dZoomLevelToDest;

	const int stepCount = 15;
	int step = 0;

	UINT_PTR nIDTimer = SetTimer(1001, 5, NULL);

	CClientDC dcPaint(this);
	CRect rc;
	GetClientRect(rc);

	CDC mem;
	mem.CreateCompatibleDC(&dcPaint);

	const int cx = rc.Width();
	const int cy = rc.Height();

	BITMAPINFOHEADER BMI;
	// Fill in the header info.
	ZeroMemory (&BMI, sizeof(BMI));
	BMI.biSize = sizeof(BITMAPINFOHEADER);
	BMI.biWidth = rc.Width();
	BMI.biHeight = rc.Height();
	BMI.biPlanes = 1;
	BMI.biBitCount = 32;
	BMI.biCompression = BI_RGB;   // No compression

	BYTE * pSrcBits = NULL;

	CBitmap bmpFrom;
	bmpFrom.Attach(CreateDIBSection (NULL, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pSrcBits, 0, 0l));


	CBitmap bmpTo;
	BYTE * pDestBits = NULL;
	bmpTo.Attach(CreateDIBSection (NULL, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pDestBits, 0, 0l));

	DWORD dwTimePer = 10;

	m_bInAction = TRUE;

	for (;;)
	{
		DWORD dwTime = GetTickCount ();

		if (step == stepCount - 1)
		{
			m_pActivePage->m_dZoomLevel = dZoomLevelToDest;
			m_pActivePage->m_ptScrollOffset = ptScrollOffsetToDest;
		}
		else
		{
			m_pActivePage->m_dZoomLevel = dZoomLevelToSrc + (dZoomLevelToDest - dZoomLevelToSrc) * (step + 1) / stepCount;

			m_pActivePage->m_ptScrollOffset =
				CPoint(int((xFrom + (xTo - xFrom) * (step + 1) / stepCount) / m_pActivePage->m_dZoomLevel),
				int((yFrom + (yTo - yFrom) * (step + 1) / stepCount) / m_pActivePage->m_dZoomLevel));
		}


		double dZoomLevelFrom = dZoomLevelFromSrc + (dZoomLevelFromDest - dZoomLevelFromSrc) * (step + 1) / stepCount;
		CPoint ptScroolOffsetFrom =
			CPoint(int((ptScrollOffsetFromSrc.x * dZoomLevelFromSrc + (ptScrollOffsetFromDest.x * dZoomLevelFromDest - ptScrollOffsetFromSrc.x * dZoomLevelFromSrc) * (step + 1) / stepCount) / dZoomLevelFrom),
			int((ptScrollOffsetFromSrc.y * dZoomLevelFromSrc + (ptScrollOffsetFromDest.y * dZoomLevelFromDest - ptScrollOffsetFromSrc.y * dZoomLevelFromSrc) * (step + 1) / stepCount) / dZoomLevelFrom));

		{
			CBitmap* pOldBitmap = mem.SelectObject(&bmpFrom);

			{
				CXTPFlowGraphDrawContext* pDC = CreateDrawContext(mem);
				pDC->SetClipRect(rc);

				m_pPaintManager->DrawControlBackground(pDC, this);

				pDC->SetWorldTransform(ptScroolOffsetFrom, dZoomLevelFrom);

				pPageFrom->OnDraw(pDC);

				pDC->SetWorldTransform(CPoint(0, 0), 1);

				delete pDC;
			}

			mem.SelectObject(pOldBitmap);

			pOldBitmap = mem.SelectObject(&bmpTo);

			{
				CXTPFlowGraphDrawContext* pDC = CreateDrawContext(mem);
				pDC->SetClipRect(rc);

				m_pPaintManager->DrawControlBackground(pDC, this);

				pDC->SetWorldTransform(m_pActivePage->m_ptScrollOffset, m_pActivePage->m_dZoomLevel);

				m_pActivePage->OnDraw(pDC);

				pDC->SetWorldTransform(CPoint(0, 0), 1);

				delete pDC;
			}

			mem.SelectObject(pOldBitmap);


			{
				int byAlpha = 255 * (step + 1) / stepCount;

				byAlpha = 255 - byAlpha;
				const BYTE byDiff = (BYTE)(255 - byAlpha);

				PBYTE pDest = pDestBits;
				PBYTE pSrc = pSrcBits;

				int cnt = cx * cy;
				for (int i = 0; i < cnt; i++)
				{
					pDest[0] = (BYTE)((pDest[0] * byDiff + pSrc[0] * byAlpha) >> 8);
					pDest[1] = (BYTE)((pDest[1] * byDiff + pSrc[1] * byAlpha) >> 8);
					pDest[2] = (BYTE)((pDest[2] * byDiff + pSrc[2] * byAlpha) >> 8);

					pDest += 4;
					pSrc += 4;
				}
			}

			pOldBitmap = mem.SelectObject(&bmpTo);

			dcPaint.BitBlt(0, 0, rc.right, rc.bottom, &mem, 0, 0, SRCCOPY);

			mem.SelectObject(pOldBitmap);
		}
		step++;

		if (step == stepCount)
			break;

		dwTime = GetTickCount () - dwTime;
		if (dwTime < dwTimePer)
		{
			Sleep(dwTimePer - dwTime);
		}
	}
	m_bInAction = FALSE;

	AdjustScrollBars();
	SendNotifyMessage(XTP_FGN_PAGESCROLLOFFSETCHANGED);
	SendNotifyMessage(XTP_FGN_PAGEZOOMLEVELCHANGED);

	KillTimer(nIDTimer);

	RedrawControl();

}

void CXTPFlowGraphControl::AnimateTo(CPoint ptScrollOffset, double dZoomLevel)
{
	if (!m_pActivePage)
		return;

	CPoint ptOldScrollOffset = m_pActivePage->m_ptScrollOffset;
	double dOldZoomLevel = m_pActivePage->m_dZoomLevel;

	if (ptOldScrollOffset == ptScrollOffset && fabs(dOldZoomLevel - dZoomLevel) < 1e-6)
		return;

	double xFrom = ptOldScrollOffset.x * dOldZoomLevel;
	double yFrom = ptOldScrollOffset.y * dOldZoomLevel;

	double xTo = ptScrollOffset.x * dZoomLevel;
	double yTo = ptScrollOffset.y * dZoomLevel;

	const int stepCount = 15;
	int step = 0;

	int dwStart = (int)GetTickCount();

	UINT_PTR nIDTimer = SetTimer(1001, m_nAnimationDelay, NULL);

	m_bInAction = TRUE;

	for (;;)
	{
		if ((int)GetTickCount() - dwStart > m_nAnimationDelay)
		{
			dwStart = (int)GetTickCount();

			if (step == stepCount - 1)
			{
				m_pActivePage->m_dZoomLevel = dZoomLevel;
				m_pActivePage->m_ptScrollOffset = ptScrollOffset;
			}
			else
			{
				m_pActivePage->m_dZoomLevel = dOldZoomLevel + (dZoomLevel - dOldZoomLevel) * (step + 1) / stepCount;

				m_pActivePage->m_ptScrollOffset =
					CPoint(int((xFrom + (xTo - xFrom) * (step + 1) / stepCount) / m_pActivePage->m_dZoomLevel),
					int((yFrom + (yTo - yFrom) * (step + 1) / stepCount) / m_pActivePage->m_dZoomLevel));
			}

			RedrawControl();
			AdjustScrollBars();
			UpdateWindow();
			step++;

			if (step == stepCount)
				break;
		}


		MSG msg;

		while (::PeekMessage(&msg, NULL, WM_PAINT, WM_PAINT, PM_NOREMOVE))
		{
			if (!GetMessage(&msg, NULL, WM_PAINT, WM_PAINT))
				break;
			DispatchMessage(&msg);
		}

		if (!::GetMessage(&msg, NULL, 0, 0))
		{
			AfxPostQuitMessage((int)msg.wParam);
			break;
		}

		DispatchMessage (&msg);
	}

	m_bInAction = FALSE;

	SendNotifyMessage(XTP_FGN_PAGESCROLLOFFSETCHANGED);
	SendNotifyMessage(XTP_FGN_PAGEZOOMLEVELCHANGED);

	KillTimer(nIDTimer);

	RedrawControl();

}

void CXTPFlowGraphControl::GetZoomParam(LPCRECT lpRect, CPoint& ptScrollOffset, double& dZoomLevel)
{
	CRect rc(lpRect);

	CRect rcClient;
	GetClientRect(&rcClient);

	dZoomLevel = 0;
	ptScrollOffset = rc.TopLeft();

	if (rc.Height() * rcClient.Width() > rcClient.Height() * rc.Width())
	{
		dZoomLevel = (double)rcClient.Height() / rc.Height();

		int nWidth = int(rcClient.Width() / dZoomLevel);

		ptScrollOffset.x -= (nWidth - rc.Width()) / 2;
	}
	else
	{
		dZoomLevel = (double)rcClient.Width() / rc.Width();

		int nHeight = int(rcClient.Height() / dZoomLevel);

		ptScrollOffset.y -= (nHeight - rc.Height()) / 2;

	}
}

void CXTPFlowGraphControl::ZoomToRect(LPCRECT lpRect, BOOL bAnimate)
{
	if (!m_pActivePage || !GetSafeHwnd())
		return;

	CPoint ptScrollOffset;
	double dZoomLevel;

	GetZoomParam(lpRect, ptScrollOffset, dZoomLevel);

	if (bAnimate)
	{
		AnimateTo(ptScrollOffset, dZoomLevel);
	}
	else
	{
		m_pActivePage->m_dZoomLevel = dZoomLevel;
		m_pActivePage->m_ptScrollOffset = ptScrollOffset;

		SendNotifyMessage(XTP_FGN_PAGESCROLLOFFSETCHANGED);
		SendNotifyMessage(XTP_FGN_PAGEZOOMLEVELCHANGED);

		OnGraphChanged();
		Reposition();
		AdjustScrollBars();
	}
}

void CXTPFlowGraphControl::SelectAll()
{
	if (!m_pActivePage)
		return;
	m_pActivePage->GetSelection()->Clear();

	int i;

	for (i = 0; i < m_pActivePage->GetNodes()->GetCount(); i++)
		m_pActivePage->GetSelection()->AddSelection(m_pActivePage->GetNodes()->GetAt(i));

	for (i = 0; i < m_pActivePage->GetConnections()->GetCount(); i++)
		m_pActivePage->GetSelection()->AddSelection(m_pActivePage->GetConnections()->GetAt(i));

	//RedrawControl(); // each call of 'AddSelection' does it
}

void CXTPFlowGraphControl::Arrange()
{
	if (!m_pActivePage)
		return;

	m_pActivePage->Arrange();
}

CXTPFlowGraphNodeCustom* CXTPFlowGraphControl::AddCustomNodeFromToolkitResource(UINT uResShapeID, CXTPFlowGraphPage *pPage /*= NULL*/)
{
	if (!pPage)
		pPage = GetActivePage();

	CXTPFlowGraphNodeCustom *pNode = NULL;

	CString xaml;
	XTPResourceManager()->LoadHTML(&xaml, MAKEINTRESOURCE(uResShapeID) );

	ASSERT(xaml.GetLength());

	if (xaml.GetLength() && pPage)
	{
		GetUndoManager()->StartGroup();

		pNode = pPage->GetNodes()->AddCustomNode();

		pNode->SetMarkupText(xaml);

		GetUndoManager()->EndGroup();
	}

	return pNode;
}

INT_PTR CXTPFlowGraphControl::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(m_hWnd));

	// check child windows first by calling CControlBar
	INT_PTR nHit = CWnd::OnToolHitTest(point, pTI);
	if (nHit != -1)
		return nHit;

	CXTPFlowGraphNode* pNode = HitTestNode(point);

	if (pNode)
	{
		CXTPFlowGraphConnectionPoint* pConnectionPoint = pNode->HitTestConnectionPoint(point);

		CString strTip;

		if (pConnectionPoint && !pConnectionPoint->GetTooltip().IsEmpty())
		{
			nHit = (INT_PTR)pConnectionPoint;

			strTip = pConnectionPoint->GetTooltip();
		}
		else
		{
			nHit = (INT_PTR)pNode;

			strTip = pNode->GetTooltip();
		}

		if (strTip.GetLength() == 0)
			return -1;

		CXTPToolTipContext::FillInToolInfo(pTI, m_hWnd, pNode->GetScreenRect(), nHit, strTip);

		return nHit;
	}
	CXTPFlowGraphConnection* pConnection = HitTestConnection(point);

	if (pConnection)
	{
		nHit = (INT_PTR)pConnection;

		CString strTip = pConnection->GetTooltip();
		if (strTip.GetLength() == 0)
			return -1;

		CXTPToolTipContext::FillInToolInfo(pTI, m_hWnd,
			CRect(point.x - 3, point.y - 3, point.x + 3, point.y + 3), nHit, strTip);

		return nHit;
	}


	return -1;
}



void CXTPFlowGraphControl::RenameNode(CXTPFlowGraphNode* pNode)
{
	if (!pNode)
		return;

	CXTPFlowGraphEditItem* pEditItem = new CXTPFlowGraphEditItem;
	pEditItem->Create(pNode);
}

void CXTPFlowGraphControl::RenameConnectionPoint(CXTPFlowGraphConnectionPoint* pConnectionPoint)
{
	if (!pConnectionPoint)
		return;

	CXTPFlowGraphEditItem* pEditItem = new CXTPFlowGraphEditItem;
	pEditItem->Create(pConnectionPoint);
}

void CXTPFlowGraphControl::SetSmoothingMode(XTPFlowGraphSmoothingMode nSmoothingMode)
{
	m_nSmoothingMode = nSmoothingMode;

	if (m_pActivePage) m_pActivePage->OnGraphChanged();
	RedrawControl();
}
void CXTPFlowGraphControl::OnEndLabelEdit(CXTPFlowGraphElement* pItem, LPCTSTR str)
{
	XTP_NM_FLOWGRAPH_ENDLABELEDIT ele;
	ele.pItem = pItem;
	ele.strNewString = str;

	if (SendNotifyMessage(XTP_FGN_ENDLABELEDIT, &ele.hdr) == -1)
		return;

	if (ele.strNewString.GetLength())
		pItem->SetCaption(ele.strNewString);
}


LRESULT CXTPFlowGraphControl::SendNotifyMessage(UINT nMessage, NMHDR* pNMHDR) const
{
	if (!IsWindow(m_hWnd))
		return 0;

	NMHDR nmhdr;
	if (pNMHDR == NULL)
		pNMHDR = &nmhdr;

	pNMHDR->hwndFrom = GetSafeHwnd();
	pNMHDR->idFrom = GetDlgCtrlID();
	pNMHDR->code = nMessage;

	CWnd *pOwner = GetOwner();
	if (pOwner && IsWindow(pOwner->m_hWnd))
		return pOwner->SendMessage(WM_NOTIFY, pNMHDR->idFrom, (LPARAM)pNMHDR);
	else
		return 0;
}


void CXTPFlowGraphControl::OnRButtonDown(UINT nFlags, CPoint point)
{
	CWnd::OnRButtonDown(nFlags, point);

	SetFocus();

#ifdef _XTP_ACTIVEX
	((COleControl*)GetParent())->FireMouseDown(RIGHT_BUTTON, AxShiftState(), point.x, point.y);
#endif

}

void CXTPFlowGraphControl::OnRButtonUp(UINT nFlags, CPoint point)
{
	CWnd::OnRButtonUp(nFlags, point);

	SendNotifyMessage(NM_RCLICK);

#ifdef _XTP_ACTIVEX
	((COleControl*)GetParent())->FireMouseUp(RIGHT_BUTTON, AxShiftState(), point.x, point.y);
#endif
}


BOOL CXTPFlowGraphControl::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if (m_pToolTipContext)
	{
		m_pToolTipContext->FilterToolTipMessage(this, message, wParam, lParam);
	}

	return CWnd::OnWndMsg(message, wParam, lParam, pResult);
}

void CXTPFlowGraphControl::EnableMarkup(BOOL bEnable)
{
	ASSERT(bEnable);

	if (bEnable)
	{
		XTPMarkupReleaseContext(m_pMarkupContext);

		if (bEnable)
		{
			m_pMarkupContext = XTPMarkupCreateContext();
		}
	}
	else
	{
		// markup cannot be disabled now
	}
}

void CXTPFlowGraphControl::SetScrollbarTheme(XTPScrollBarTheme theme)
{
	// default theme - default scrollbars

	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);

	if (theme != xtpScrollBarThemeWindowsDefault)
	{
		if (!m_pCustomScrollBars[SB_VERT])
		{
			m_pCustomScrollBars[SB_VERT] = new CXTPScrollBarCtrl();
			m_pCustomScrollBars[SB_VERT]->Create(WS_CHILD | WS_VISIBLE | SBS_VERT, CRect(0, 0, 0, 0), this, 1);
		}

		if (!m_pCustomScrollBars[SB_HORZ])
		{
			m_pCustomScrollBars[SB_HORZ] = new CXTPScrollBarCtrl();
			m_pCustomScrollBars[SB_HORZ]->Create(WS_CHILD | WS_VISIBLE | SBS_HORZ, CRect(0, 0, 0, 0), this, 2);
		}

		((CXTPScrollBarCtrl *)m_pCustomScrollBars[SB_VERT])->SetTheme(theme);
		((CXTPScrollBarCtrl *)m_pCustomScrollBars[SB_HORZ])->SetTheme(theme);

		// hide default scrollbars!

		dwStyle &= (~(WS_HSCROLL|WS_VSCROLL));
	}
	else
	{
		DeleteCustomScrollBars();

		dwStyle |= (WS_HSCROLL|WS_VSCROLL);

		Invalidate(TRUE);
	}

	::SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);
}

void CXTPFlowGraphControl::DeleteCustomScrollBars()
{
	if (m_pCustomScrollBars[SB_VERT])
		delete m_pCustomScrollBars[SB_VERT];

	if (m_pCustomScrollBars[SB_HORZ])
		delete m_pCustomScrollBars[SB_HORZ];

	m_pCustomScrollBars[SB_HORZ] = m_pCustomScrollBars[SB_VERT] = NULL;
}

void CXTPFlowGraphControl::RepositionCustomScrollBars()
{
	if (!m_pCustomScrollBars[SB_HORZ] & !m_pCustomScrollBars[SB_VERT] )
		return; // no custom scrollbars reposition

	// reposition:

	CRect rcWindow;
	GetWindowRect(rcWindow);

	CWnd *pScrollParent = this;
	CScrollBar *pVertical = GetScrollBarCtrl(SB_VERT);
	CScrollBar *pHorizontal = GetScrollBarCtrl(SB_HORZ);

	if (pVertical)
	{
		CRect vertScrollRect = rcWindow;
		vertScrollRect.DeflateRect(rcWindow.Width() - ::GetSystemMetrics(SM_CXVSCROLL), 0, 0, 0);

		pScrollParent->ScreenToClient(vertScrollRect);

		pVertical->SetWindowPos(NULL, vertScrollRect.left, vertScrollRect.top, vertScrollRect.Width(), vertScrollRect.Height(), SWP_NOZORDER);
	}

	if (pHorizontal)
	{
		CRect horzScrollRect = rcWindow;
		horzScrollRect.DeflateRect(0, rcWindow.Height() - ::GetSystemMetrics(SM_CYHSCROLL), ::GetSystemMetrics(SM_CXVSCROLL), 0);

		pScrollParent->ScreenToClient(horzScrollRect);

		pHorizontal->SetWindowPos(NULL, horzScrollRect.left, horzScrollRect.top, horzScrollRect.Width(), horzScrollRect.Height(), SWP_NOZORDER);
	}
}


void CXTPFlowGraphControl::EnableScrollBarCtrl(int nBar, BOOL bEnable)
{
	// NOTE: we need to call this EnableScrollBar in order not to hide scrollbars completely

	if (m_pCustomScrollBars[nBar])
	{
		m_pCustomScrollBars[nBar]->EnableScrollBar(bEnable ? ESB_ENABLE_BOTH : ESB_DISABLE_BOTH);
		m_pCustomScrollBars[nBar]->EnableWindow(bEnable);
	}
	else
		::EnableScrollBar(m_hWnd, nBar, bEnable ? ESB_ENABLE_BOTH : ESB_DISABLE_BOTH);
}

BOOL CXTPFlowGraphControl::GetScrollInfo(int nBar, LPSCROLLINFO lpScrollInfo, UINT nMask)
{
	if (m_pCustomScrollBars[nBar])
		return m_pCustomScrollBars[nBar]->GetScrollInfo(lpScrollInfo, nMask);
	else
		return CWnd::GetScrollInfo(nBar, lpScrollInfo, nMask);
}

BOOL CXTPFlowGraphControl::SetScrollInfo(int nBar, LPSCROLLINFO lpScrollInfo, BOOL bRedraw)
{
	if (m_pCustomScrollBars[nBar])
		return m_pCustomScrollBars[nBar]->SetScrollInfo(lpScrollInfo, bRedraw);
	else
		return CWnd::SetScrollInfo(nBar, lpScrollInfo, bRedraw);
}

int CXTPFlowGraphControl::GetScrollLimit(int nBar)
{
	if (m_pCustomScrollBars[nBar])
		return m_pCustomScrollBars[nBar]->GetScrollLimit();
	else
		return CWnd::GetScrollLimit(nBar);
}

CScrollBar* CXTPFlowGraphControl::GetScrollBarCtrl(int nBar) const
{
	if (m_pCustomScrollBars[nBar])
		return m_pCustomScrollBars[nBar];
	else
		return CWnd::GetScrollBarCtrl(nBar);
}
