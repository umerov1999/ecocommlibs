// XTPTabBase.cpp : implementation file
//
// This file is a part of the XTREME CONTROLS MFC class library.
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

#include "Common/XTPVC80Helpers.h"  // Visual Studio 2005 helper functions
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPColorManager.h"
#include "../Util/XTPControlTheme.h"
#include "Common/XTPDrawHelpers.h"

#include "../Util/XTPGlobal.h"
#include "XTPTabCtrlButtons.h"
#include "XTPTabBase.h"
#include "XTPTabBaseTheme.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int TABVIEW_BORDER = 4;

/////////////////////////////////////////////////////////////////////////////
// CXTPTabBase

CXTPTabBase::CXTPTabBase()
: m_pTabCtrl(NULL)
, m_pTheme(NULL)
, m_bSubclassed(TRUE)
, m_bAutoCondensing(FALSE)
, m_bXPBorder(false)
, m_bBoldFont(true)
{
	VERIFY(SetTheme(xtpControlThemeDefault));
}

CXTPTabBase::~CXTPTabBase()
{
	SAFE_DELETE(m_pNavBtns);
	CMDTARGET_RELEASE(m_pTheme);
}

void CXTPTabBase::OnPaint()
{
	CPaintDC dc(m_pTabCtrl); // device context for painting

	CXTPClientRect rcClient(m_pTabCtrl);
	CXTPBufferDC memDC(dc, rcClient);

	m_pTheme->DrawTabCtrl(&memDC, this);
}

BOOL CXTPTabBase::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void CXTPTabBase::RefreshMetrics()
{
	if (m_pTheme)
		m_pTheme->RefreshMetrics(m_pTabCtrl);

	if (::IsWindow(m_pTabCtrl->GetSafeHwnd()))
	{
		m_pTabCtrl->RedrawWindow();
	}
}

BOOL CXTPTabBase::SetTheme(XTPControlTheme eTheme)
{
	SAFE_DELETE(m_pTheme);

	switch (eTheme)
	{
	case xtpControlThemeOfficeXP:
		m_pTheme = new CXTPTabBaseThemeOfficeXP;
		break;

	case xtpControlThemeOffice2003:
		m_pTheme = new CXTPTabBaseThemeOffice2003;
		break;

	default:
		m_pTheme = new CXTPTabBaseTheme;
		break;
	}

	RefreshMetrics();

	return (m_pTheme != NULL);
}

LRESULT CXTPTabBase::OnSetTheme(WPARAM wParam, LPARAM /*lParam*/)
{
	XTPControlTheme eTheme = (XTPControlTheme)wParam;

	SetTheme(eTheme);

	return 0;
}

void CXTPTabBase::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	WPARAM wParam = (WPARAM)uFlags;
	LPARAM lParam = (LPARAM)(LPCTSTR)lpszSection;

	RefreshMetrics();
	m_pTabCtrl->SendMessageToDescendants(WM_SETTINGCHANGE, wParam, lParam, TRUE, FALSE);
}

void CXTPTabBase::OnSysColorChange()
{
	RefreshMetrics();
	m_pTabCtrl->SendMessageToDescendants(WM_SYSCOLORCHANGE, 0, 0L, TRUE, FALSE);
}

BOOL CXTPTabBase::Init()
{
	if (::IsWindow(m_pTabCtrl->GetSafeHwnd()))
	{
		m_pTabCtrl->SetFont(&XTPAuxData().font);
		m_pTabCtrl->Invalidate(FALSE);
	}

	return TRUE;
}

void CXTPTabBase::PreSubclassWindow()
{
	if (m_bSubclassed)
	{
		// Initialize the control.
		Init();
	}
}

BOOL CXTPTabBase::PreCreateWindow(CREATESTRUCT& /*cs*/)
{
	// When creating controls dynamically Init() must
	// be called from OnCreate() and not from
	// PreSubclassWindow().

	m_bSubclassed = FALSE;

	return TRUE;
}

int CXTPTabBase::OnCreate(LPCREATESTRUCT /*lpCreateStruct*/)
{
	// Initialize the control.
	Init();

	return 0;
}

void CXTPTabBase::ImplAttach(CTabCtrl* pTabCtrl)
{
	ASSERT_VALID(pTabCtrl); // must be valid

	if (pTabCtrl != NULL)
	{
		m_pTabCtrl = pTabCtrl;

		m_pNavBtns = new CXTPTabCtrlButtons;
		m_pNavBtns->Create(this);
	}
}

void CXTPTabBase::ShowNavButtons(DWORD dwFlags)
{
	if (!m_pNavBtns)
		return;

	m_pNavBtns->m_dwFlags = dwFlags;

	if (::IsWindow(m_pNavBtns->GetSafeHwnd()))
	{
		if (m_pNavBtns->ShowButtons())
		{
			m_pNavBtns->Refresh();
		}
	}
}

void CXTPTabBase::OnAddPadding(CString& strLabelText)
{
	if (!strLabelText.IsEmpty())
	{
		CClientDC dc(NULL);
		int iSaveDC = dc.SaveDC();

		CFont* pfontNormal = NULL;
		CFont* pfontSelect = NULL;

		if (m_pTabCtrl->GetStyle() & TCS_VERTICAL)
		{
			pfontNormal = &XTPAuxData().fontVert;
			pfontSelect = &XTPAuxData().fontVertBold;
		}
		else
		{
			pfontNormal = &XTPAuxData().font;
			pfontSelect = &XTPAuxData().fontBold;
		}

		dc.SelectObject(pfontSelect);
		CSize sizeSelect = dc.GetTextExtent(strLabelText);

		dc.SelectObject(pfontNormal);
		CSize sizeNormal = dc.GetTextExtent(strLabelText);

		bool bFront = true;

		while (sizeNormal.cx < sizeSelect.cx)
		{
			if (bFront)
			{
				strLabelText.Insert(0, _T(' '));
				bFront = false;
			}
			else
			{
				strLabelText += _T(' ');
				bFront = true;
			}
			sizeNormal = dc.GetTextExtent(strLabelText);
		}

		dc.RestoreDC(iSaveDC);
	}
}

void CXTPTabBase::GetChildRect(CRect& rcChild) const
{
	ASSERT_VALID(m_pTabCtrl);

	// Get the selected tab index.
	int nCurSel = m_pTabCtrl->GetCurSel();
	if (nCurSel == -1)
	{
		rcChild.SetRectEmpty();
		return;
	}

	DWORD dwStyle = m_pTabCtrl->GetStyle();

	m_pTabCtrl->GetClientRect(&rcChild);

	CRect rcTab;
	m_pTabCtrl->GetItemRect(nCurSel, &rcTab);
	rcTab.InflateRect(1, 1);

	int cy = rcTab.Height() * m_pTabCtrl->GetRowCount();
	int cx = rcTab.Width () * m_pTabCtrl->GetRowCount();

	// vertical tabs
	if (dwStyle & TCS_VERTICAL)
	{
		// Right
		if (dwStyle & TCS_RIGHT)
		{
			rcChild.top += TABVIEW_BORDER;
			rcChild.left += TABVIEW_BORDER;
			rcChild.right -= TABVIEW_BORDER + cx;
			rcChild.bottom -= TABVIEW_BORDER;
		}

		// Left
		else
		{
			rcChild.top += TABVIEW_BORDER;
			rcChild.left += TABVIEW_BORDER + cx;
			rcChild.right -= TABVIEW_BORDER;
			rcChild.bottom -= TABVIEW_BORDER;
		}
	}

	// horizontal tabs
	else
	{
		// Bottom
		if (dwStyle & TCS_BOTTOM)
		{
			rcChild.top += TABVIEW_BORDER;
			rcChild.left += TABVIEW_BORDER;
			rcChild.right -= TABVIEW_BORDER;
			rcChild.bottom -= TABVIEW_BORDER + cy;
		}

		// Top
		else
		{
			rcChild.top += TABVIEW_BORDER + cy;
			rcChild.left += TABVIEW_BORDER;
			rcChild.right -= TABVIEW_BORDER;
			rcChild.bottom -= TABVIEW_BORDER;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CXTPTabExBase

CXTPTabExBase::CXTPTabExBase()
: m_bInitialUpdate(FALSE)
, m_pParentWnd(NULL)
, m_pParentFrame(NULL)
, m_popupMenuID(0)
, m_nPos(0)
, m_pLastActiveView(NULL)
, m_nOldIndex(-1)
{
	m_bBoldFont = false;

	// force VC++ to generate the following template functions:
	if (GetAutoCondense()) // should always be false!
	{
		// this block will never be executed, but the compiler can't know that
		SetAutoCondense(FALSE);
		CXTPTabBase::OnPaint();
		CreateTabView(NULL, NULL, NULL);
	}
}

CXTPTabExBase::~CXTPTabExBase()
{
}

void CXTPTabExBase::OnThemeChanged()
{
	POSITION pos;
	for (pos = m_tcbItems.GetHeadPosition(); pos; m_tcbItems.GetNext(pos))
	{
		CXTPTcbItem* pMember = m_tcbItems.GetAt(pos);
		GetTheme()->AdjustBorders(this, pMember);
	}
}

BOOL CXTPTabExBase::Init()
{
	if (!CXTPTabBase::Init())
		return FALSE;

	if (m_bInitialUpdate)
	{
		// fire off initial updates
		// NB: this works only if WM_INITIALUPDATE has not been sent to us
		//     since its handler invalidates the signature
		m_pTabCtrl->SendMessageToDescendants(WM_INITIALUPDATE, 0, 0, TRUE, TRUE);
	}

	// Set the font for the tab control.
	InitializeFont();

	// Get the pointer to the parent window.
	m_pParentWnd = m_pTabCtrl->GetParent();
	ASSERT_VALID(m_pParentWnd);

	// Get the pointer to the parent frame window.
	m_pParentFrame = m_pTabCtrl->GetParentFrame();
	if (::IsWindow(m_pParentFrame->GetSafeHwnd()))
	{
		// If floating, get parent frame of the floating frame window.
		if (m_pParentFrame->IsKindOf(RUNTIME_CLASS(CMiniDockFrameWnd)))
		{
			m_pParentFrame = m_pParentFrame->GetParentFrame();
		}
	}

	// Resize all views.
	RecalcLayout();

	if (m_bAutoCondensing)
	{
		Condense();
	}

	// make sure children are clipped.
	m_pTabCtrl->ModifyStyle(NULL, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

	return TRUE;
}

BOOL CXTPTabExBase::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if (nCode < 1)
	{
		CWnd* pView = GetActiveView();
		if (pView && ::IsWindow(pView->m_hWnd))
		{
			if (pView->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
				return TRUE;
		}
	}

	return FALSE;
}

BOOL CXTPTabExBase::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		bool bShift = (::GetKeyState(VK_SHIFT) < 0);
		bool bCtrl = (::GetKeyState(VK_CONTROL) < 0);

		if (bCtrl && (pMsg->wParam == VK_TAB) || (pMsg->wParam == VK_F6))
		{
			if (bShift)
			{
				PrevView();
				m_pTabCtrl->SetFocus();
			}
			else
			{
				NextView();
				m_pTabCtrl->SetFocus();
			}

			return TRUE;
		}
	}

	return FALSE; // Allow messages to be passed to base class
}

void CXTPTabExBase::OnRButtonDown(UINT /*nFlags*/, CPoint point)
{
	if (m_popupMenuID)
	{
		CPoint pt = point;
		m_pTabCtrl->ClientToScreen(&pt);

		CMenu menu;
		VERIFY(menu.LoadMenu(m_popupMenuID));

		CMenu* pPopup = menu.GetSubMenu(m_nPos);
		ASSERT(pPopup != NULL);
		if (!pPopup)
			return;

		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y,
			m_pParentFrame);
	}
}

void CXTPTabExBase::OnDestroy()
{
	// Cleanup
	RemoveAllTabs();
}

void CXTPTabExBase::EnableRedraw(BOOL bRedraw)
{
	m_pTabCtrl->SetRedraw(bRedraw);

	if (bRedraw)
	{
		m_pTabCtrl->Invalidate();
		m_pTabCtrl->UpdateWindow();
	}
}

BOOL CXTPTabExBase::OnSelchange(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;

	CWnd* pOwner = m_pTabCtrl->GetOwner();
	if (::IsWindow(pOwner->GetSafeHwnd()))
	{
		*pResult = pOwner->SendMessage(TCN_SELCHANGE,
			pNMHDR->idFrom, (LPARAM)m_pTabCtrl);
	}

	if (*pResult == 0)
	{
		OnSelChange();
	}

	return FALSE;
}

void CXTPTabExBase::OnSelChange()
{
	// Get the index to the newly selected tab.
	int nIndex = m_pTabCtrl->GetCurSel();
	if (nIndex == -1)
		return;

	// Get a pointer to the selected view.
	if (m_nOldIndex != -1)
	{
		CWnd* pOldView = GetView(m_nOldIndex);
		if (pOldView != NULL)
		{
			pOldView->ShowWindow(SW_HIDE);
			pOldView->EnableWindow(FALSE);

			if (m_pParentFrame && pOldView == m_pParentFrame->GetActiveView())
			{
				if (m_pLastActiveView && ::IsWindow(m_pLastActiveView->m_hWnd))
				{
					m_pParentFrame->SetActiveView(m_pLastActiveView);
				}
				else
				{
					m_pParentFrame->SetActiveView(NULL, FALSE);
					m_pLastActiveView = NULL;
				}
			}
		}
	}

	// Get a pointer to the selected view.
	CWnd* pView = GetView(nIndex);
	if (pView != NULL)
	{
		// If this is a CView object then set the active view
		// the parent frame.
		if (pView->IsKindOf(RUNTIME_CLASS(CView)))
		{
			if (m_pParentFrame && m_pParentFrame->IsChild(pView))
			{
				// Save a pointer to the view if it is not a child of the
				// tab control, we will use this rather than setting the view
				// to NULL for the frame window.
				CView* pActiveView = m_pParentFrame->GetActiveView();
				if (pActiveView && pActiveView->GetParent() != m_pTabCtrl)
				{
					m_pLastActiveView = pActiveView;
				}
				m_pParentFrame->SetActiveView((CView*)pView);
			}
		}

		ActivateView(pView);
	}
}

BOOL CXTPTabExBase::OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;

	CWnd* pOwner = m_pTabCtrl->GetOwner();
	if (::IsWindow(pOwner->GetSafeHwnd()))
	{
		*pResult = pOwner->SendMessage(TCN_SELCHANGING,
			pNMHDR->idFrom, (LPARAM)m_pTabCtrl);
	}

	if (*pResult == 0)
	{
		OnSelChanging();
	}

	return FALSE;
}

void CXTPTabExBase::OnSelChanging()
{
	// Get the index to the selected tab, we will
	// need this later to hide old view.
	m_nOldIndex = m_pTabCtrl->GetCurSel();
}

void CXTPTabExBase::OnPreWindowPosChanged(WINDOWPOS FAR* lpwndpos)
{
	if (lpwndpos->flags & SWP_HIDEWINDOW)
	{
		POSITION pos;
		for (pos = m_tcbItems.GetHeadPosition(); pos; m_tcbItems.GetNext(pos))
		{
			CWnd* pView = m_tcbItems.GetAt(pos)->pWnd;
			ASSERT_VALID(pView);

			if (m_pParentFrame && m_pParentFrame->GetActiveView() == pView)
			{
				// To avoid main window freezing, we must deactivate the view,
				// because it's not visible now.

				if (m_pLastActiveView && ::IsWindow(m_pLastActiveView->m_hWnd))
				{
					m_pParentFrame->SetActiveView(m_pLastActiveView);
				}
				else
				{
					m_pParentFrame->SetActiveView(NULL, FALSE);
					m_pLastActiveView = NULL;
				}
			}
		}
	}
	else
	{
		// resize all child views.
		RecalcLayout();
	}
}

void CXTPTabExBase::OnPostWindowPosChanged()
{
	if (m_bAutoCondensing)
	{
		Condense();
	}
}

BOOL CXTPTabExBase::ModifyTabStyle(DWORD dwRemove, DWORD dwAdd, UINT nFlags)
{
	if (!m_pTabCtrl->ModifyStyle(dwRemove, dwAdd, nFlags))
		return FALSE;

	// Set the font for the tab control.
	InitializeFont();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CXTPTabExBase tooltip related functions

#if (_MSC_VER <= 1100) // Using Visual C++ 5.0

CToolTipCtrl* CXTPTabExBase::GetTips()
{
	return m_pTabCtrl->GetTooltips();
}

void CXTPTabExBase::SetTips(CToolTipCtrl* pWndTip)
{
	m_pTabCtrl->SetTooltips(pWndTip);
}

#else

CToolTipCtrl* CXTPTabExBase::GetTips()
{
	return m_pTabCtrl->GetToolTips();
}

void CXTPTabExBase::SetTips(CToolTipCtrl* pWndTip)
{
	m_pTabCtrl->SetToolTips(pWndTip);
}

#endif//#if (_MSC_VER <= 1100)

void CXTPTabExBase::AddToolTip(UINT nIDTab, LPCTSTR lpszText)
{
	// Get the tool tips for the tab control.
	CToolTipCtrl* pToolTips = GetTips();

	if (pToolTips != NULL)
	{
		// Add the tool tip.
		if (nIDTab == 0)
		{
			pToolTips->AddTool(m_pTabCtrl, lpszText, NULL, nIDTab);
		}
		else
		{
			pToolTips->AddTool(m_pTabCtrl, lpszText, CRect(0, 0, 0, 0), nIDTab);
		}
	}
}

void CXTPTabExBase::UpdateToolTip(int nIDTab, LPCTSTR lpszText)
{
	ASSERT_VALID(m_pTabCtrl);
	ASSERT((nIDTab >= 0) && (m_pTabCtrl->GetItemCount() > nIDTab));

	// Find the view we want to change the tooltip for and
	// reset its tooltip text
	POSITION pos = m_tcbItems.FindIndex(nIDTab);
	if (pos != NULL)
	{
		// Get the private data for the view
		CXTPTcbItem *pMember = m_tcbItems.GetAt(pos);
		ASSERT(pMember);
		if (!pMember)
			return;

		// Reset the tooltip text to the new value
		pMember->szToolTipLabel = lpszText;

		// Get the tool tips for the tab control.
		CToolTipCtrl* pToolTips = GetTips();

		if (pToolTips != NULL)
		{
			// Update the tooltip for the view
			pToolTips->UpdateTipText(pMember->szToolTipLabel,
				m_pTabCtrl, pMember->uiToolTipId);
		}
	}
}

void CXTPTabExBase::UpdateToolTip(CRuntimeClass* pViewClass, LPCTSTR lpszText)
{
	ASSERT_VALID(m_pTabCtrl);
	ASSERT(pViewClass != NULL);
	if (!pViewClass)
		return;
	ASSERT(pViewClass->IsDerivedFrom(RUNTIME_CLASS(CWnd)));
	ASSERT(AfxIsValidAddress(pViewClass, sizeof(CRuntimeClass), FALSE));

	int nIndex = 0;
	POSITION pos;
	for (pos = m_tcbItems.GetHeadPosition(); pos; m_tcbItems.GetNext(pos))
	{
		// Get the private data for the view
		CXTPTcbItem *pMember = m_tcbItems.GetAt(pos);
		ASSERT(pMember);
		if (!pMember)
			continue;

		if (pMember->pWnd->IsKindOf(pViewClass))
		{
			UpdateToolTip(nIndex, lpszText);
			return;
		}
		nIndex++;
	}
}

void CXTPTabExBase::ResetToolTips()
{
	// Get the tool tips for the tab control.
	CToolTipCtrl* pToolTips = GetTips();

	if (pToolTips != NULL)
	{
		int nIndex = 0;

		POSITION pos;
		for (pos = m_tcbItems.GetHeadPosition(); pos; m_tcbItems.GetNext(pos))
		{
			// Get the next item in the list.
			CXTPTcbItem* pMember = m_tcbItems.GetAt(pos);

			// Update the tooltip for the view
			pMember->uiToolTipId = nIndex;

			pToolTips->UpdateTipText(pMember->szToolTipLabel,
				m_pTabCtrl, nIndex);

			++nIndex;
		}
	}
}

BOOL CXTPTabExBase::EnableToolTipsEx(BOOL bEnable)
{
	// Get the tool tips for the tab control.
	CToolTipCtrl* pToolTips = GetTips();

	if (pToolTips != NULL)
	{
		pToolTips->Activate(bEnable);
		return TRUE;
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CXTPTabExBase tab view methods.

CWnd* CXTPTabExBase::CreateTabView(CRuntimeClass* pViewClass, CDocument* pDocument, CCreateContext* pContext)
{
	if (!pContext && !pViewClass)
		return NULL;

	ASSERT_VALID(m_pTabCtrl);
	ASSERT(pContext || pViewClass != NULL);
	ASSERT(pContext || pViewClass->IsDerivedFrom(RUNTIME_CLASS(CWnd)));
	ASSERT(pContext || AfxIsValidAddress(pViewClass, sizeof(CRuntimeClass), FALSE));


	CCreateContext contextT;
	if (pContext == NULL)
	{
		// if no context specified, generate one from the currently selected
		// client if possible.
		contextT.m_pCurrentDoc = pDocument;
		contextT.m_pNewViewClass = pViewClass;
		contextT.m_pLastView = NULL;
		contextT.m_pCurrentFrame = NULL;
		contextT.m_pNewDocTemplate = NULL;

		if (pDocument)
		{
			contextT.m_pNewDocTemplate = pDocument->GetDocTemplate();
		}

		pContext = &contextT;
	}

	if (!pContext->m_pNewViewClass)
		return NULL;

	CWnd* pWnd = NULL;
	TRY
	{
		pWnd = (CWnd*)pContext->m_pNewViewClass->CreateObject();
		if (pWnd == NULL)
		{
			AfxThrowMemoryException();
		}
	}
	CATCH_ALL(e)
	{
		TRACE0("Out of memory creating a view.\n");
		// Note: DELETE_EXCEPTION(e) not required
		return NULL;
	}
	END_CATCH_ALL

	ASSERT_KINDOF(CWnd, pWnd);
	if (!pWnd)
		return NULL;
	ASSERT(pWnd->m_hWnd == NULL); // not yet created.

	DWORD dwStyle = AFX_WS_DEFAULT_VIEW;

	if (m_pTheme->GetThemeStyle() != xtpControlThemeOffice2000)
	{
		dwStyle &= ~WS_BORDER;
	}

	int nTab = (int)m_tcbItems.GetCount();

	// Create with the right size (wrong position)
	CRect rect(0, 0, 0, 0);
	if (!pWnd->Create(NULL, NULL, dwStyle,
		rect, m_pTabCtrl, (AFX_IDW_PANE_FIRST + nTab), pContext))
	{
		TRACE0("Warning: couldn't create client tab for view.\n");
		// pWnd will be cleaned up by PostNcDestroy
		return NULL;
	}
	if (pWnd->m_hWnd == NULL)
		return NULL;

	ASSERT((int)_AfxGetDlgCtrlID(pWnd->m_hWnd) == (AFX_IDW_PANE_FIRST + nTab));

	// send initial notification message for the window just created
	// only if our initialization has been completed
	if (m_bInitialUpdate)
	{
		pWnd->SendMessage(WM_INITIALUPDATE);
	}

	pWnd->SetOwner(m_pParentWnd);
	return pWnd;
}

BOOL CXTPTabExBase::AddView(LPCTSTR lpszLabel, CRuntimeClass* pViewClass, CDocument* pDoc/*= NULL*/,
	CCreateContext *pContext/*= NULL*/, int iIndex/*= -1*/, int iIconIndex/*= -1*/, LPARAM lParam)
{
	if (!pContext && !pViewClass)
		return NULL;

	ASSERT_VALID(m_pTabCtrl);
	ASSERT(pContext || pViewClass);
	ASSERT(pContext || pViewClass->IsDerivedFrom(RUNTIME_CLASS(CWnd)));
	ASSERT(pContext || AfxIsValidAddress(pViewClass, sizeof(CRuntimeClass), FALSE));

	// Create the view to be associate with tab.
	CWnd* pWnd = CreateTabView(pViewClass, pDoc, pContext);
	if (pWnd == NULL) // tabs can be a CWnd, but are usually CViews
		return FALSE;

	return AddControl(lpszLabel, (CView *)pWnd, iIndex, iIconIndex, lParam);
}

BOOL CXTPTabExBase::AddView(LPCTSTR lpszLabel, CView* pView, int iIndex/*= -1*/, int iIconIndex/*= -1*/, LPARAM lParam)
{
	ASSERT(pView != NULL);
	if (!pView)
		return FALSE;

	ASSERT(pView->GetRuntimeClass()->IsDerivedFrom(RUNTIME_CLASS(CView)));
	return AddControl(lpszLabel, pView, iIndex, iIconIndex, lParam);
}

BOOL CXTPTabExBase::AddControl(LPCTSTR lpszLabel, CWnd* pWnd, int iIndex/*= -1*/, int iIconIndex/*= -1*/, LPARAM lParam)
{
	ASSERT_VALID(m_pTabCtrl);
	ASSERT_VALID(pWnd);
	ASSERT(pWnd != NULL);
	ASSERT(lpszLabel != NULL);
	ASSERT(iIndex >= -1);
	ASSERT(iIndex <= m_pTabCtrl->GetItemCount());

	if (!pWnd)
		return FALSE;

	pWnd->SetParent(m_pTabCtrl);

	// Allocate a new CXTPTcbItem object.
	CXTPTcbItem* pMember = new CXTPTcbItem;

	if (pMember == NULL)
		return FALSE;

	pMember->pWnd = pWnd;
	pMember->szTabLabel = lpszLabel;
	pMember->szToolTipLabel = lpszLabel;
	pMember->uiToolTipId = m_pTabCtrl->GetItemCount();
	pMember->dwStyle = ::GetWindowLong(pWnd->GetSafeHwnd(), GWL_STYLE);
	pMember->dwExStyle = ::GetWindowLong(pWnd->GetSafeHwnd(), GWL_EXSTYLE);

	// If no index was specified add the view tab
	// to the end.
	if (iIndex == -1)
	{
		iIndex = (int)m_tcbItems.GetCount();
	}

	// Add the new CXTPTcbItem to the tab item list.
	const POSITION pos = m_tcbItems.FindIndex(iIndex);
	if (pos != NULL)
	{
		m_tcbItems.InsertBefore(pos, pMember);
	}
	else
	{
		ASSERT(iIndex == m_pTabCtrl->GetItemCount());
		m_tcbItems.AddTail(pMember);
	}

	// Insert the new tab item.
	TC_ITEM tci;
	tci.mask = TCIF_TEXT | TCIF_IMAGE | TCIF_PARAM;
	tci.pszText = (LPTSTR)lpszLabel;

	if (iIconIndex != -1)
	{
		tci.iImage = iIconIndex;
	}
	else
	{
		tci.iImage = iIndex;
	}

	tci.lParam = lParam;

	// set window borders based on current theme.
	GetTheme()->AdjustBorders(this, pMember);
	m_pTabCtrl->InsertItem(iIndex, &tci);

	// Create the ToolTip for the tab.
	AddToolTip(pMember->uiToolTipId, lpszLabel);

	// Since the tool tip count has changed,
	// re-initialize the tooltips.
	ResetToolTips();

	// hide/show views after insertion
	SetActiveView(iIndex);

	return TRUE;
}

BOOL CXTPTabExBase::UpdateTabLabel(int iTab, CXTPTcbItem* pMember, LPCTSTR lpszLabel)
{
	// Text length cannot be longer than 255 characters.
	ASSERT(_tcslen(lpszLabel) < 255);

	CString strLabel = lpszLabel;
	if (strLabel.GetLength() > 255)
	{
		strLabel = strLabel.Left(255);
	}

	// if the string is empty return FALSE.
	else if (strLabel.IsEmpty())
	{
		return FALSE;
	}

	// if CXTPTcbItem pointer is NULL, return FALSE.
	if (pMember == NULL)
		return FALSE;

	TCHAR szText[256];

	TC_ITEM tci;
	tci.mask = TCIF_TEXT;
	tci.cchTextMax = _countof(szText);
	tci.pszText = szText;

	// get the tab control item and update the text.
	m_pTabCtrl->GetItem(iTab, &tci);
	STRCPY_S(szText, 256, strLabel);
	m_pTabCtrl->SetItem(iTab, &tci);

	// Get the tool tips for the tab control and update
	// the tip text.
	CToolTipCtrl* pToolTips = GetTips();
	if (pToolTips == NULL)
		return FALSE;

	pToolTips->UpdateTipText(strLabel, m_pTabCtrl, iTab);

	// update the CXTPTcbItem item label and tip text.
	pMember->szTabLabel = strLabel;
	pMember->szToolTipLabel = strLabel;

	// update other information
	pMember->szCondensedLabel = strLabel;

	return TRUE;
}

BOOL CXTPTabExBase::SetTabText(int iTab, LPCTSTR lpszLabel)
{
	// Get the item at the specified index.
	POSITION pos = m_tcbItems.FindIndex (iTab);
	if (pos == NULL)
		return FALSE;

	return UpdateTabLabel (iTab, m_tcbItems.GetAt(pos), lpszLabel);
}

BOOL CXTPTabExBase::SetTabText(CWnd* pView, LPCTSTR lpszLabel)
{
	int iTab = 0;

	POSITION pos;
	for (pos = m_tcbItems.GetHeadPosition(); pos; m_tcbItems.GetNext(pos))
	{
		// Get the next item in the list.
		CXTPTcbItem* pMember = m_tcbItems.GetAt(pos);

		if (pView == pMember->pWnd)
			return UpdateTabLabel (iTab, m_tcbItems.GetAt(pos), lpszLabel);

		++iTab;
	}

	return FALSE;
}

BOOL CXTPTabExBase::SetTabText(CRuntimeClass* pViewClass, LPCTSTR lpszLabel)
{
	int iTab = 0;

	POSITION pos;
	for (pos = m_tcbItems.GetHeadPosition(); pos; m_tcbItems.GetNext(pos))
	{
		// Get the next item in the list.
		CXTPTcbItem* pMember = m_tcbItems.GetAt(pos);

		if (pMember->pWnd->IsKindOf(pViewClass))
			return UpdateTabLabel (iTab, m_tcbItems.GetAt(pos), lpszLabel);

		++iTab;
	}

	return FALSE;
}

CWnd* CXTPTabExBase::NextView()
{
	ASSERT_VALID(m_pTabCtrl);
	ASSERT(m_tcbItems.GetCount() > 0);

	// get the current selection and increment by 1.
	int iIndex = m_pTabCtrl->GetCurSel();
	iIndex++;

	// if the new index is greater than or equal to
	// the tab count, set the index to 0.
	if (iIndex > m_pTabCtrl->GetItemCount()-1)
		iIndex = 0;

	// set the active view and return a pointer to
	// the newly activated view.
	SetActiveView(iIndex);

	return GetActiveView();
}

CWnd* CXTPTabExBase::PrevView()
{
	ASSERT_VALID(m_pTabCtrl);
	ASSERT(m_tcbItems.GetCount() > 0);

	// get the current selection and increment by 1.
	int iIndex = m_pTabCtrl->GetCurSel();
	iIndex--;

	// if the new index is greater than or equal to
	// the tab count, set the index to 0.
	if (iIndex < 0)
		iIndex = m_pTabCtrl->GetItemCount()-1;

	// set the active view and return a pointer to
	// the newly activated view.
	SetActiveView(iIndex);


	return GetActiveView();
}

CWnd* CXTPTabExBase::GetView(int nView)
{
	ASSERT_VALID(m_pTabCtrl);

	if (nView > -1 && nView < m_pTabCtrl->GetItemCount())
	{
		POSITION pos = m_tcbItems.FindIndex(nView);
		if (pos != NULL)
		{
			CWnd* pWnd = m_tcbItems.GetAt(pos)->pWnd;
			ASSERT_VALID(pWnd);

			if (pWnd && ::IsWindow(pWnd->m_hWnd))
			{
				return pWnd;
			}
		}
	}

	return NULL;
}

CWnd* CXTPTabExBase::GetView(CRuntimeClass* pViewClass)
{
	ASSERT_VALID(m_pTabCtrl);
	ASSERT(pViewClass != NULL);
	if (!pViewClass)
		return 0;

	ASSERT(pViewClass->IsDerivedFrom(RUNTIME_CLASS(CWnd)));
	ASSERT(AfxIsValidAddress(pViewClass, sizeof(CRuntimeClass), FALSE));

	POSITION pos;
	for (pos = m_tcbItems.GetHeadPosition(); pos; m_tcbItems.GetNext(pos))
	{
		CWnd* pWnd = m_tcbItems.GetAt(pos)->pWnd;
		ASSERT_VALID(pWnd);

		if (pWnd && ::IsWindow(pWnd->m_hWnd) && pWnd->IsKindOf(pViewClass))
		{
			return pWnd;
		}
	}

	return NULL;
}

void CXTPTabExBase::ActivateView(CWnd* pNewView)
{
	ASSERT_VALID(pNewView); // must be valid

	// Hide all views.
	int iIndex;
	for (iIndex = 0; iIndex < m_tcbItems.GetCount(); ++iIndex)
	{
		POSITION pos = m_tcbItems.FindIndex(iIndex);
		if (pos != NULL)
		{
			CXTPTcbItem* pTCBItem = m_tcbItems.GetAt(pos);
			ASSERT(pTCBItem != NULL);

			if (pTCBItem && pTCBItem->pWnd)
			{
				pTCBItem->pWnd->ShowWindow(SW_HIDE);
				pTCBItem->pWnd->EnableWindow(FALSE);
			}
		}
	}

	////////// ULF 2003-08-07
	if (m_pParentFrame && m_pParentFrame->IsChild(pNewView))
	{
		CView* pActiveView = m_pParentFrame->GetActiveView();
		if (pActiveView && pActiveView->GetParent() == m_pTabCtrl)
		{
			if (pActiveView != pNewView)
			{
				// Make the frame window aware of the active view (for message routing)
				m_pParentFrame->SetActiveView(DYNAMIC_DOWNCAST(CView, pNewView));
			}
		}
	}
	/////////

	// Show and enable the window.
	pNewView->ShowWindow(SW_SHOW);
	pNewView->EnableWindow(TRUE);
	pNewView->SetFocus();

	// Resize the window.
	ResizeTabView(pNewView);
}

void CXTPTabExBase::SetActiveView(CRuntimeClass* pViewClass)
{
	ASSERT_VALID(m_pTabCtrl);
	ASSERT(pViewClass != NULL);
	if (!pViewClass)
		return;
	ASSERT(pViewClass->IsDerivedFrom(RUNTIME_CLASS(CWnd)));
	ASSERT(AfxIsValidAddress(pViewClass, sizeof(CRuntimeClass), FALSE));

	// Walk through the tabs
	int iIndex;
	for (iIndex = 0; iIndex < m_tcbItems.GetCount(); ++iIndex)
	{
		POSITION pos = m_tcbItems.FindIndex(iIndex);
		if (pos != NULL)
		{
			CXTPTcbItem* pTCBItem = m_tcbItems.GetAt(pos);
			ASSERT(pTCBItem != NULL);

			if (pTCBItem && pTCBItem->pWnd->IsKindOf(pViewClass))
			{
				// Select new the tab
				m_pTabCtrl->SetCurSel(iIndex);

				// now active the view and give it focus.
				ActivateView(pTCBItem->pWnd);
				break;
			}
		}
	}
}

void CXTPTabExBase::SetActiveView(CWnd* pTabView)
{
	ASSERT_VALID(m_pTabCtrl);
	ASSERT(pTabView != NULL);

	// Walk through the tabs
	int iIndex;
	for (iIndex = 0; iIndex < m_tcbItems.GetCount(); ++iIndex)
	{
		POSITION pos = m_tcbItems.FindIndex(iIndex);
		if (pos != NULL)
		{
			CXTPTcbItem* pTCBItem = m_tcbItems.GetAt(pos);
			ASSERT(pTCBItem != NULL);

			// Does the tab view match the requested view ?
			if (pTCBItem && pTCBItem->pWnd == pTabView)
			{
				// Select new the tab
				m_pTabCtrl->SetCurSel(iIndex);

				// now active the view and give it focus.
				ActivateView(pTCBItem->pWnd);
				break;
			}
		}
	}
}

void CXTPTabExBase::SetActiveView(int nActiveTab)
{
	ASSERT_VALID(m_pTabCtrl);
	ASSERT(nActiveTab >= 0);

	// Find the item at the specified index.
	POSITION pos = m_tcbItems.FindIndex(nActiveTab);
	if (pos != NULL)
	{
		CXTPTcbItem* pTCBItem = m_tcbItems.GetAt(pos);
		ASSERT(pTCBItem != NULL);

		if (pTCBItem && pTCBItem->pWnd != NULL)
		{
			ASSERT_VALID(pTCBItem->pWnd);

			// Select new the tab
			m_pTabCtrl->SetCurSel(nActiveTab);

			// now active the view and give it focus.
			ActivateView(pTCBItem->pWnd);
		}
	}
}

void CXTPTabExBase::RemoveListItem(POSITION pos, BOOL bDestroyWnd/*= TRUE*/)
{
	// invalid position
	if (pos == NULL)
		return;

	// remove the page from internal list
	CXTPTcbItem *pMember = m_tcbItems.GetAt(pos);

	// if this item is invalid, return.
	if (!pMember)
		return;

	if (pMember->pWnd && ::IsWindow(pMember->pWnd->m_hWnd))
	{
		// And delete the member window, freeing our stored
		// values relating to it
		if (bDestroyWnd)
		{
			pMember->pWnd->DestroyWindow();
			pMember->pWnd = NULL;
		}
		else
		{
			pMember->pWnd->ShowWindow(SW_HIDE);
		}
	}

	// Remove from list and free memory.
	m_tcbItems.RemoveAt(pos);
	SAFE_DELETE(pMember);
}

void CXTPTabExBase::DeleteView(int nView, BOOL bDestroyWnd/*= TRUE*/)
{
	ASSERT_VALID(m_pTabCtrl);
	ASSERT((nView >= 0) && (m_pTabCtrl->GetItemCount() > nView));

	// Now find the view we want to delete and remove it
	POSITION pos = m_tcbItems.FindIndex(nView);
	if (pos != NULL)
	{
		CWnd* pView = m_tcbItems.GetAt(pos)->pWnd;

		// Ensure that we get no dangling pointers
		if (m_pParentFrame && m_pParentFrame->GetActiveView() == pView)
		{
			if (m_pLastActiveView && ::IsWindow(m_pLastActiveView->m_hWnd))
			{
				m_pParentFrame->SetActiveView(m_pLastActiveView);
			}
			else
			{
				m_pParentFrame->SetActiveView(NULL, FALSE);
				m_pLastActiveView = NULL;
			}
		}

		// Remove item from list, and free memory.
		RemoveListItem(pos, bDestroyWnd);

		// Remove it from the tab control
		m_pTabCtrl->DeleteItem(nView);

		int nCount = m_pTabCtrl->GetItemCount();

		if (nCount > 0)
		{
			// Finally, if we have just deleted the active view, reset the
			// active tab to be the first view in the list
			if (nView == 0)
			{
				SetActiveView(nView);
			}
			else if (nView >= nCount)
			{
				SetActiveView(nCount-1);
			}
			else
			{
				SetActiveView(nView-1);
			}

			// Reset the tooltips for the views we have left...
			ResetToolTips();
		}
	}
}

void CXTPTabExBase::DeleteView(CRuntimeClass* pViewClass, BOOL bDestroyWnd/*= TRUE*/)
{
	ASSERT_VALID(m_pTabCtrl);
	ASSERT(pViewClass != NULL);
	if (!pViewClass)
		return;
	ASSERT(pViewClass->IsDerivedFrom(RUNTIME_CLASS(CWnd)));
	ASSERT(AfxIsValidAddress(pViewClass, sizeof(CRuntimeClass), FALSE));

	int nView = 0;
	POSITION pos;
	for (pos = m_tcbItems.GetHeadPosition(); pos; m_tcbItems.GetNext(pos))
	{
		CWnd* pView = m_tcbItems.GetAt(pos)->pWnd;
		ASSERT_VALID(pView);

		if (pView->IsKindOf(pViewClass))
		{
			DeleteView(nView, bDestroyWnd);
			return;
		}
		nView++;
	}
}

void CXTPTabExBase::DeleteView(CWnd* pView, BOOL bDestroyWnd/*= TRUE*/)
{
	ASSERT_VALID(m_pTabCtrl);
	ASSERT(pView != NULL);
	ASSERT(AfxIsValidAddress(pView, sizeof(CWnd), FALSE));

	int nView = 0;
	POSITION pos;
	for (pos = m_tcbItems.GetHeadPosition(); pos; m_tcbItems.GetNext(pos))
	{
		if (m_tcbItems.GetAt(pos)->pWnd == pView)
		{
			DeleteView(nView, bDestroyWnd);
			return;
		}
		nView++;
	}
}

LPCTSTR CXTPTabExBase::GetViewName(int nView)
{
	ASSERT_VALID(m_pTabCtrl);
	ASSERT((nView >= 0) && (m_pTabCtrl->GetItemCount() > nView));

	if (nView != -1)
	{
		POSITION pos = m_tcbItems.FindIndex(nView);
		if (pos != NULL)
		{
			return m_tcbItems.GetAt(pos)->szTabLabel;
		}
	}

	return NULL;
}

LPCTSTR CXTPTabExBase::GetViewName(CRuntimeClass* pViewClass)
{
	ASSERT_VALID(m_pTabCtrl);
	ASSERT(pViewClass != NULL);
	if (!pViewClass)
		return NULL;
	ASSERT(pViewClass->IsDerivedFrom(RUNTIME_CLASS(CWnd)));
	ASSERT(AfxIsValidAddress(pViewClass, sizeof(CRuntimeClass), FALSE));

	POSITION pos;
	for (pos = m_tcbItems.GetHeadPosition(); pos; m_tcbItems.GetNext(pos))
	{
		CXTPTcbItem *pMember = m_tcbItems.GetAt(pos);
		ASSERT(pMember);

		if (pMember && pMember->pWnd->IsKindOf(pViewClass))
		{
			return (LPCTSTR)pMember->szTabLabel;
		}
	}
	return NULL;
}

BOOL CXTPTabExBase::IsChildView(CWnd* pWnd)
{
	POSITION pos = m_tcbItems.GetHeadPosition();
	while (pos)
	{
		CXTPTcbItem *pItem = m_tcbItems.GetNext(pos);
		if (pItem && pItem->pWnd == pWnd)
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CXTPTabExBase::RemoveAllTabs(BOOL bDestroyWnd/*= TRUE*/)
{
	if (m_pParentFrame)
	{
		// If the parent frames active view belongs to
		// us set the active view to NULL.
		CView * pActiveView = m_pParentFrame->GetActiveView();
		if (IsChildView(pActiveView))
		{
			if (m_pLastActiveView && ::IsWindow(m_pLastActiveView->m_hWnd) && !IsChildView(m_pLastActiveView))
			{
				m_pParentFrame->SetActiveView(m_pLastActiveView);
			}
			else
			{
				m_pParentFrame->SetActiveView(NULL, FALSE);
			}
		}
	}

	m_pLastActiveView = NULL;
	m_nOldIndex = -1;

	// Free memory and clear tab view list.
	while (!m_tcbItems.IsEmpty())
	{
		POSITION pos = m_tcbItems.FindIndex(0);
		if (pos != NULL)
		{
			RemoveListItem(pos, bDestroyWnd);
		}
	}

	// Remove all tabs from control.
	m_pTabCtrl->DeleteAllItems();

	return TRUE;
}

int CXTPTabExBase::GetTabFromPoint(CPoint point)
{
	TCHITTESTINFO tch;
	memset(&tch, 0, sizeof(TCHITTESTINFO));

	if (point == CPoint(0, 0))
	{
		CPoint pt;
		GetCursorPos(&pt);
		m_pTabCtrl->ScreenToClient(&pt);
		tch.pt = pt;
	}
	else
	{
		tch.pt = point;
	}

	return m_pTabCtrl->HitTest(&tch);
}

BOOL CXTPTabExBase::Defer(HDWP& hDWP, CWnd* pView)
{
	if (pView && ::IsWindow(pView->m_hWnd))
	{
		HWND hWnd = pView->m_hWnd;

		CRect rcNewSize;
		GetChildRect(rcNewSize);

		CRect rcOldSize;
		pView->GetWindowRect(&rcOldSize);
		m_pTabCtrl->ScreenToClient(&rcOldSize);

		if (rcNewSize != rcOldSize)
		{
			int x = (int) rcNewSize.left;
			int y = (int) rcNewSize.top;
			int cx = (int) rcNewSize.Width();
			int cy = (int) rcNewSize.Height();

			// Set positioning flags
			UINT uFlags = SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOOWNERZORDER;

			// if the x-y coordinates have not changed, there is no reason
			// to move the dialog item.
			if (rcNewSize.TopLeft() == rcOldSize.TopLeft())
				uFlags |= SWP_NOMOVE;

			// if the cx-cy size has not changed, there is no reason to
			// size the dialog item.  If size has changed, don't
			// copy bits of the client area (i.e. have them invalidated/redrawn)
			if (rcNewSize.Size() == rcOldSize.Size())
				uFlags |= SWP_NOSIZE;
			else
				uFlags |= SWP_NOCOPYBITS;

			hDWP = ::DeferWindowPos(hDWP, hWnd, 0, x, y, cx, cy, uFlags);
			if (hDWP == NULL)
			{
				TRACE(_T("DeferWindowPos failed for ID %i\n"), GetDlgCtrlID(hWnd));
				return FALSE;
			}

			return TRUE;
		}
	}

	return FALSE;
}

void CXTPTabExBase::RecalcLayout()
{
	HDWP hDWP = ::BeginDeferWindowPos((int)m_tcbItems.GetCount());

	// Resize all views.
	POSITION pos;
	for (pos = m_tcbItems.GetHeadPosition(); pos; m_tcbItems.GetNext(pos))
	{
		CXTPTcbItem* pTCBItem = m_tcbItems.GetAt(pos);
		ASSERT(pTCBItem != NULL);
		if (!pTCBItem)
			continue;

		CWnd* pWnd = pTCBItem->pWnd;
		ASSERT_VALID(pWnd);

		Defer(hDWP, pWnd);
	}

	::EndDeferWindowPos(hDWP);
}

void CXTPTabExBase::ResizeTabView(CWnd* pView)
{
	if (pView && ::IsWindow(pView->m_hWnd))
	{
		CRect rcChild;
		GetChildRect(rcChild);
		pView->MoveWindow(rcChild);
	}
}

void CXTPTabExBase::InitializeFont()
{
	HGDIOBJ hFont = XTPAuxData().font.GetSafeHandle();

	// if the tabs are vertical, use the vertical menu font.
	if ((m_pTabCtrl->GetStyle() & TCS_VERTICAL) == TCS_VERTICAL)
		hFont = XTPAuxData().fontVert.GetSafeHandle();

	if (hFont != NULL)
	{
		if (XTPSystemVersion()->IsWin95())
			m_pTabCtrl->PostMessage(WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
		else
			m_pTabCtrl->SendMessage(WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
	}
}

void CXTPTabExBase::SetAutoCondense(BOOL bEnable)
{
	m_bAutoCondensing = bEnable;
	Condense();
}

BOOL CXTPTabExBase::GetAutoCondense()
{
	return m_bAutoCondensing;
}

int CXTPTabExBase::CalculateTabWidth(CDC* pDC, CString& sLabel, bool bHasIcon)
{
	int len = pDC->GetTextExtent(sLabel).cx + 6 + 6;
	if (sLabel.GetLength() == 1) ++len;
	if (bHasIcon)
	{
		len += 16 + 6;
	}
	return len;
}

// used by tab condensation code
struct CXTPTabExBase::ITEMANDWIDTH
{
	CXTPTcbItem*    pItem;
	int             iWidth;
	bool            bHasIcon;
	bool            bAtMinLength;
	int             iItemIndex;
	CString sNewLabel;
};

void CXTPTabExBase::Condense()
{
	if (!m_pTabCtrl->m_hWnd || m_pTabCtrl->GetItemCount() == 0)
	{
		return;
	}

	// create a device context with the tab control's font (used for label
	// widths calculations)
	CFont *pFont = m_pTabCtrl->GetFont();
	VERIFY(pFont != NULL);
	CDC *pDC = m_pTabCtrl->GetDC();
	CFont *pOldFont = pDC->SelectObject(pFont);

	int iTotalWidth = 0;
	CList <ITEMANDWIDTH, ITEMANDWIDTH&> lstItemsByWidth;

	// the formula for iMinTabWidth is based on empirical data
	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);
	const int iMinTabWidth = tm.tmAveCharWidth * 6 + 12;

	// calculate length of tab labels and fill our tracking list
	POSITION p = m_tcbItems.GetHeadPosition();
	ASSERT(p != NULL);  // there should be items
	int idx = 0;
	TCITEM tci;
	tci.mask = TCIF_IMAGE;
	while (p)
	{
		m_pTabCtrl->GetItem(idx, &tci);
		ITEMANDWIDTH iaw;
		iaw.bHasIcon = (tci.iImage >= 0);
		iaw.pItem = m_tcbItems.GetNext(p);
		iaw.iWidth = CalculateTabWidth(pDC, iaw.pItem->szTabLabel, iaw.bHasIcon);
		iaw.sNewLabel = iaw.pItem->szTabLabel;
		iaw.iItemIndex = idx;
		iaw.bAtMinLength = FALSE;
		++idx;
		lstItemsByWidth.AddTail(iaw);
		iTotalWidth += iaw.iWidth;
	}

	const bool bVertical = (m_pTabCtrl->GetStyle() & TCS_VERTICAL) == TCS_VERTICAL;
	const bool bMultiLine =
		bVertical || ((m_pTabCtrl->GetStyle() & TCS_MULTILINE) == TCS_MULTILINE);

	CRect rc;
	m_pTabCtrl->GetClientRect(&rc);
	const int client_width = bVertical ? rc.Height() : rc.Width();
	POSITION p2;

	if (client_width == 0)
	{
		pDC->SelectObject(pOldFont);
		m_pTabCtrl->ReleaseDC(pDC);
		return;
	}

	if (m_bAutoCondensing)
	{
		iTotalWidth += 4; // 4 pixels extra for the selected tab
		if (bMultiLine) ++iTotalWidth;

		// start condensing labels until the total width fits the client area,
		// or until we can't condense any more
		while (iTotalWidth > client_width)
		{
			// look for widest item
			p = lstItemsByWidth.GetHeadPosition();
			int iWidest = -1;
			while (p)
			{
				const POSITION p3 = p;
				ITEMANDWIDTH& iaw = lstItemsByWidth.GetNext(p);
				if (iaw.iWidth > iWidest && !iaw.bAtMinLength)
				{
					iWidest = iaw.iWidth;
					p2 = p3;
				}
			}
			if (iWidest == -1)
			{
				break;
			}
			ITEMANDWIDTH& iaw = lstItemsByWidth.GetNext(p2);
			const int iOldWidth = iaw.iWidth;
			CString& label = iaw.sNewLabel;

			// shorten name
			if (label.Right(3) != _T("..."))
			{
				label += _T("...");
			}
			do
			{
				if (label.GetLength() == 3 || (iaw.iWidth <= iMinTabWidth))
				{
					iaw.bAtMinLength = true;
				}
				else
				{
					int iLen = label.GetLength();
					if ((iLen > 0) && ((iLen-4) >= 0))
					{
						label.Delete((iLen-4), 1);
					}
				}
				iaw.iWidth = CalculateTabWidth(pDC, label, iaw.bHasIcon);
			}
			while (!iaw.bAtMinLength && iaw.iWidth >= iOldWidth);

			iTotalWidth -= (iOldWidth - iaw.iWidth);
		}
	}

	// update actual tab labels based on new condensed labels
	tci.mask = TCIF_TEXT;
	p = lstItemsByWidth.GetHeadPosition();
	while (p)
	{
		ITEMANDWIDTH& iaw = lstItemsByWidth.GetNext(p);
		// only change if actually different
		if (iaw.pItem->szCondensedLabel != iaw.sNewLabel)
		{
			iaw.pItem->szCondensedLabel = iaw.sNewLabel;
			tci.pszText = (TCHAR*)(LPCTSTR)iaw.sNewLabel;
			m_pTabCtrl->SetItem(iaw.iItemIndex, &tci);
		}
	}

	pDC->SelectObject(pOldFont);
	m_pTabCtrl->ReleaseDC(pDC);
}
