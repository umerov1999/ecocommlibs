// XTPDockingPane.cpp : implementation of the CXTPDockingPaneclass.
//
// This file is a part of the XTREME DOCKINGPANE MFC class library.
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

#include "Common/XTPColorManager.h"
#include "Common/XTPSystemHelpers.h"

#include <TabManager/Includes.h>

#include "XTPDockingPaneDefines.h"
#include "XTPDockingPaneBase.h"
#include "XTPDockingPaneBaseContainer.h"
#include "XTPDockingPane.h"
#include "XTPDockingPaneManager.h"
#include "XTPDockingPaneTabbedContainer.h"
#include "XTPDockingPaneMiniWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPDockingPane::CXTPDockingPane(CXTPDockingPaneLayout* pLayout)
	: CXTPDockingPaneBase(xtpPaneTypeDockingPane, pLayout)
{
	m_dwOptions = 0;
	m_hwndChild = 0;
	m_nID = 0;
	m_nIconID = -1;
	m_dwData = 0;
	m_clrItemTab = COLORREF_NULL;
	m_nIDHelp = 0;
	m_bEnabled = xtpPaneEnabledAuto;

	m_ptMinTrackSize = CPoint(0, 0);
	m_ptMaxTrackSize = CPoint(32000, 32000);

	EnableAutomation();

#ifdef _XTP_ACTIVEX
	EnableTypeLib();
#endif

}

CXTPDockingPane::~CXTPDockingPane()
{

}

void CXTPDockingPane::SetID(int nID)
{
	ASSERT(nID != 0);

	m_nID = nID;
#ifndef _XTP_ACTIVEX
	CString strTitle;
	if (strTitle.LoadString(nID))
	{
		SetTitle(strTitle);
	}
#endif

}

void CXTPDockingPane::SetWindowRect(CRect rc)
{
	m_rcWindow = rc;
	m_szDocking = m_rcWindow.Size();
}

XTPDockingPaneEnableOptions CXTPDockingPane::GetEnabled() const
{
	if (m_bEnabled == xtpPaneEnabledAuto)
	{
		return xtpPaneEnabled;
	}
	return m_bEnabled;
}

void CXTPDockingPane::SetEnabled(XTPDockingPaneEnableOptions bEnabled)
{
	if (m_bEnabled != bEnabled)
	{
		m_bEnabled = bEnabled;
		InvalidatePane(TRUE);
	}
}

void CXTPDockingPane::OnSizeParent(CWnd* pParent, CRect rect, LPVOID lParam)
{
	AFX_SIZEPARENTPARAMS* lpLayout = (AFX_SIZEPARENTPARAMS*)lParam;
	SetDockingSite(pParent);

	if (lpLayout == 0 || lpLayout->hDWP != NULL)
	{
		m_rcWindow = rect;

		BOOL bVisible = !rect.IsRectEmpty();

		ShowWindow(bVisible);

		if (m_hwndChild && pParent && m_pParentContainer && bVisible)
		{
			MapWindowPoints(pParent->GetSafeHwnd(), m_pParentContainer->GetPaneHwnd(), (LPPOINT)&rect, 2);
			::MoveWindow(m_hwndChild, rect.left, rect.top, rect.Width(), rect.Height(), TRUE);

			if (m_bEnabled != xtpPaneEnabledAuto)
			{
				::EnableWindow(m_hwndChild, m_bEnabled & xtpPaneEnableClient ? TRUE : FALSE);
			}
		}
	}
}

void CXTPDockingPane::SetFocus()
{
	if (m_hwndChild)
	{
		if (!IsChild(m_hwndChild, ::GetFocus()))
			::SetFocus(m_hwndChild);
	}
}
BOOL CXTPDockingPane::IsFocus() const
{
	HWND hwndFocus = ::GetFocus();
	return (m_hwndChild != 0) && (hwndFocus == m_hwndChild || IsChild(m_hwndChild, hwndFocus));

}

void CXTPDockingPane::SetParentContainer(CXTPDockingPaneBase* pContainer)
{
	m_pParentContainer = pContainer;

	if (m_hwndChild == 0)
		return;

	if (pContainer)
	{
		ASSERT(pContainer->GetType() == xtpPaneTypeTabbedContainer);
		::SetParent(m_hwndChild, pContainer->GetPaneHwnd());

	}
	else
	{
		::ShowWindow(m_hwndChild, SW_HIDE);
		::SetParent(m_hwndChild, GetDockingPaneManager()->GetSafeHwnd());

		m_pDockingSite = NULL;
	}
}

void CXTPDockingPane::ShowWindow(BOOL bShow)
{
	if (bShow)
	{
		GetDockingPaneManager()->NotifyOwner(XTP_DPN_SHOWWINDOW, (LPARAM)this);
	}
	if (m_hwndChild)
	{
		::ShowWindow(m_hwndChild, bShow ? SW_SHOW : SW_HIDE);
	}
}

void CXTPDockingPane::Close()
{
	GetDockingPaneManager()->ClosePane(this);
}

void CXTPDockingPane::Hide()
{
	GetDockingPaneManager()->HidePane(this);
}

void CXTPDockingPane::Select()
{
	GetDockingPaneManager()->ShowPane(this);
}

CWnd* CXTPDockingPane::GetDockingSite() const
{
	return m_pParentContainer ? m_pParentContainer->m_pDockingSite : 0;

}

CFrameWnd* CXTPDockingPane::GetParentFrame() const
{
	CWnd* pSite = GetDockingSite();

	return pSite && pSite->IsFrameWnd() ? (CFrameWnd*)pSite : NULL;
}

void CXTPDockingPane::Copy(CXTPDockingPaneBase* pCloneBase, CXTPPaneToPaneMap* /*pMap*/, DWORD /*dwIgnoredOptions*/)
{
	Copy((CXTPDockingPane*)pCloneBase);
}

void CXTPDockingPane::Copy(CXTPDockingPane* pClone)
{
	m_szDocking = pClone->m_szDocking;

	m_strTitle = pClone->m_strTitle;
	m_strTabCaption = pClone->m_strTabCaption;
	m_strTitleToolTip = pClone->m_strTitleToolTip;

	m_nIconID = pClone->m_nIconID;
	m_dwOptions = pClone->m_dwOptions;
	m_dwData = pClone->m_dwData;
	m_nID = pClone->m_nID;
	m_bEnabled = pClone->m_bEnabled;

	m_ptMinTrackSize = pClone->m_ptMinTrackSize;
	m_ptMaxTrackSize = pClone->m_ptMaxTrackSize;

	m_hwndChild = 0;
}

BOOL CXTPDockingPane::IsValid() const
{
	return m_hwndChild != NULL;

}
void CXTPDockingPane::Attach(CWnd* pWnd)
{
	if (pWnd)
	{
		CXTPDockingPaneManager* pManager = GetDockingPaneManager();

		m_hwndChild = pWnd->GetSafeHwnd();
		pManager->_Redraw();

		if (m_pParentContainer)
		{
			::SetParent(m_hwndChild, m_pParentContainer->GetPaneHwnd());
		}

		if (pManager->m_bInitialUpdateCalled && pManager->m_bAutoInitialUpdate)
		{
			pWnd->SendMessage(WM_INITIALUPDATE, 0, 0);
			pWnd->SendMessageToDescendants(WM_INITIALUPDATE, 0, 0, TRUE, TRUE);
		}
	}
}

CWnd* CXTPDockingPane::AttachView(CWnd* pParentWnd, CRuntimeClass* pViewClass, CDocument* pDocument/*= NULL*/, CCreateContext* pContext/*= NULL*/)
{
	if (!pContext && !pViewClass)
		return NULL;

#ifdef _DEBUG
	ASSERT(pContext != NULL || pViewClass != NULL);
	ASSERT(pContext != NULL || pViewClass->IsDerivedFrom(RUNTIME_CLASS(CWnd)));
	ASSERT(pContext != NULL || AfxIsValidAddress(pViewClass, sizeof(CRuntimeClass), FALSE));
#endif

	CCreateContext contextT;
	if (pContext == NULL)
	{
		// if no context specified, generate one from the
		// currently selected client if possible.
		contextT.m_pLastView = NULL;
		contextT.m_pCurrentFrame = NULL;
		contextT.m_pNewDocTemplate = NULL;
		contextT.m_pCurrentDoc = pDocument;
		contextT.m_pNewViewClass = pViewClass;

		if (pDocument != NULL)
			contextT.m_pNewDocTemplate = pDocument->GetDocTemplate();

		pContext = &contextT;
	}

	CFrameWnd* pFrame = new CFrameWnd;

	if (!pFrame->Create(NULL, NULL, WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		CRect(0, 0, 0, 0), pParentWnd, NULL, 0, pContext))
	{
		delete pFrame;
		return NULL;
	}

	pFrame->ModifyStyleEx(WS_EX_CLIENTEDGE, 0);

	Attach(pFrame);

	CWnd* pView = pFrame->GetDescendantWindow(AFX_IDW_PANE_FIRST, TRUE);

	if (DYNAMIC_DOWNCAST(CView, pView))
	{
		pFrame->SetActiveView((CView*)pView);
	}

	return pView;
}

void CXTPDockingPane::Detach()
{
	if (m_hwndChild)
	{
		::ShowWindow(m_hwndChild, SW_HIDE);
		::SetParent(m_hwndChild, GetDockingPaneManager()->GetSite()->GetSafeHwnd());
		m_hwndChild = 0;
	}
	GetDockingPaneManager()->_Redraw();
}

CString CXTPDockingPane::GetTitle() const
{
	return m_strTitle;
}

void CXTPDockingPane::SetTitleToolTip(LPCTSTR lpszTitleToolTip)
{
	m_strTitleToolTip = lpszTitleToolTip;
}

CString CXTPDockingPane::GetTitleToolTip() const
{
	return m_strTitleToolTip;
}

void CXTPDockingPane::SetTabCaption(LPCTSTR lpszTabCaption)
{
	CString strTabCaption(lpszTabCaption);
	if (m_strTabCaption != strTabCaption)
	{
		m_strTabCaption = strTabCaption;
		InvalidatePane(FALSE);
	}

}

CString CXTPDockingPane::GetTabCaption() const
{
	if (m_strTabCaption.IsEmpty())
		return m_strTitle;

	return m_strTabCaption;
}

void CXTPDockingPane::SetTitle(LPCTSTR lpszTitle)
{
	CString strTitle = lpszTitle;

	if (m_strTitle != strTitle)
	{
		int nIndex = strTitle.Find('\n');

		if (nIndex != -1)
		{
			AfxExtractSubString(m_strTitle, strTitle, 0, '\n');
			AfxExtractSubString(m_strTabCaption, strTitle, 1, '\n');
		}
		else
		{
			m_strTitle = strTitle;
		}


		InvalidatePane(FALSE);
	}
}

BOOL CXTPDockingPane::IsClosed() const
{
	return m_pParentContainer == NULL;
}

BOOL CXTPDockingPane::IsHidden() const
{
	if (m_pParentContainer == NULL)
		return FALSE;

	return m_pParentContainer->IsHidden();
}

BOOL CXTPDockingPane::IsSelected() const
{
	if (m_pParentContainer == NULL) return FALSE;
	ASSERT(m_pParentContainer->GetType() == xtpPaneTypeTabbedContainer);
	return ((CXTPDockingPaneTabbedContainer*)m_pParentContainer)->GetSelected() == this;
}

BOOL CXTPDockingPane::IsFloating() const
{
	if (m_pParentContainer == NULL) return FALSE;
	if (IsHidden()) return FALSE;
	ASSERT(m_pParentContainer->GetType() == xtpPaneTypeTabbedContainer);

	if (m_pParentContainer->GetPaneHwnd() == 0) return FALSE;

	CWnd* pFrame = m_pParentContainer->GetDockingSite();
	return pFrame && pFrame->IsKindOf(RUNTIME_CLASS(CXTPDockingPaneMiniWnd));
}


CXTPImageManagerIcon* CXTPDockingPane::GetIcon(int nWidth) const
{
	return GetDockingPaneManager()->GetIcon(GetIconID(), nWidth);
}


void CXTPDockingPane::SetItemColor(COLORREF clr)
{
	m_clrItemTab = clr;
	InvalidatePane(FALSE);
}

COLORREF CXTPDockingPane::GetItemColor() const
{
	if (m_clrItemTab != COLORREF_NULL)
		return m_clrItemTab;

	if (m_hwndChild)
	{
		COLORREF clr = (COLORREF)::SendMessage(m_hwndChild, WM_XTP_GETTABCOLOR, 0, 0);
		if (clr != 0)
			return clr;
	}
	return xtpTabColorBlue + GetID() % 8;
}

void CXTPDockingPane::GetMinMaxInfo(LPMINMAXINFO pMinMaxInfo) const
{
	ZeroMemory(pMinMaxInfo, sizeof(MINMAXINFO));
	pMinMaxInfo->ptMinTrackSize = m_ptMinTrackSize;
	pMinMaxInfo->ptMaxTrackSize = m_ptMaxTrackSize;
}

DWORD CXTPDockingPane::GetOptions() const
{
	return GetDockingPaneManager()->m_dwDefaultPaneOptions | m_dwOptions;
}

void CXTPDockingPane::DeletePane()
{
	InternalRelease();
}

//////////////////////////////////////////////////////////////////////////
// Accessible

CCmdTarget* CXTPDockingPane::GetAccessible()
{
	return this;
}

HRESULT CXTPDockingPane::GetAccessibleParent(IDispatch* FAR* ppdispParent)
{
	SAFE_MANAGE_STATE(m_pModuleState);

	*ppdispParent = NULL;

	if (m_pParentContainer)
	{
		*ppdispParent = ((CXTPDockingPaneTabbedContainer*)m_pParentContainer)->GetIDispatch(TRUE);
		return S_OK;
	}
	return E_FAIL;
}

HRESULT CXTPDockingPane::GetAccessibleChildCount(long FAR* pChildCount)
{
	if (pChildCount == 0)
		return E_INVALIDARG;

	*pChildCount = m_hwndChild ? 1 : 0;

	return S_OK;
}

HRESULT CXTPDockingPane::GetAccessibleChild(VARIANT varChild, IDispatch* FAR* ppdispChild)
{
	SAFE_MANAGE_STATE(m_pModuleState);

	*ppdispChild = NULL;

	if (GetChildIndex(&varChild) == 1)
	{
		return AccessibleObjectFromWindow(m_hwndChild, OBJID_WINDOW, IID_IDispatch, (void**)ppdispChild);
	}

	return S_OK;
}

HRESULT CXTPDockingPane::GetAccessibleName(VARIANT varChild, BSTR* pszName)
{
	if (GetChildIndex(&varChild) != CHILDID_SELF)
		return E_INVALIDARG;

	*pszName = GetTabCaption().AllocSysString();
	return S_OK;
}

HRESULT CXTPDockingPane::GetAccessibleDescription(VARIANT varChild, BSTR* pszDescription)
{
	if (GetChildIndex(&varChild) != CHILDID_SELF)
		return E_INVALIDARG;

	*pszDescription = GetTitle().AllocSysString();
	return S_OK;
}

HRESULT CXTPDockingPane::GetAccessibleRole(VARIANT varChild, VARIANT* pvarRole)
{
	pvarRole->vt = VT_EMPTY;

	if (GetChildIndex(&varChild) == CHILDID_SELF)
	{
		pvarRole->vt = VT_I4;
		pvarRole->lVal = ROLE_SYSTEM_PAGETAB;
		return S_OK;
	}

	return E_INVALIDARG;
}

HRESULT CXTPDockingPane::AccessibleSelect(long /*flagsSelect*/, VARIANT varChild)
{
	SAFE_MANAGE_STATE(m_pModuleState);

	if (GetChildIndex(&varChild) != CHILDID_SELF)
		return E_INVALIDARG;

	Select();

	return S_OK;
}


HRESULT CXTPDockingPane::GetAccessibleState(VARIANT varChild, VARIANT* pvarState)
{
	if (GetChildIndex(&varChild) != CHILDID_SELF)
		return E_INVALIDARG;

	pvarState->vt = VT_I4;
	pvarState->lVal = STATE_SYSTEM_SELECTABLE;

	if (!m_pParentContainer)
		pvarState->lVal |= STATE_SYSTEM_INVISIBLE;

	if (m_pParentContainer && ((CXTPDockingPaneTabbedContainer*)m_pParentContainer)->GetSelected() == this)
		pvarState->lVal |= STATE_SYSTEM_SELECTED;

	return S_OK;
}

HRESULT CXTPDockingPane::GetAccessibleDefaultAction(VARIANT varChild, BSTR* pszDefaultAction)
{
	if (GetChildIndex(&varChild) != CHILDID_SELF)
		return E_INVALIDARG;

	*pszDefaultAction = SysAllocString(L"Switch");

	return S_OK;
}

HRESULT CXTPDockingPane::AccessibleDoDefaultAction(VARIANT varChild)
{
	SAFE_MANAGE_STATE(m_pModuleState);

	if (GetChildIndex(&varChild) != CHILDID_SELF)
		return E_INVALIDARG;

	Select();

	return S_OK;
}

HRESULT CXTPDockingPane::AccessibleLocation(long* pxLeft, long* pyTop, long* pcxWidth, long* pcyHeight, VARIANT varChild)
{
	CRect rc;

	if (GetChildIndex(&varChild) == 1)
	{
		GetWindowRect(m_hwndChild, &rc);
	}
	else if (GetChildIndex(&varChild) != CHILDID_SELF)
	{
		return E_INVALIDARG;
	}
	else
	{
		CXTPDockingPaneTabbedContainer* pContainer = (CXTPDockingPaneTabbedContainer*)m_pParentContainer;
		if (!pContainer)
			return S_FALSE;

		pContainer->GetWindowRect(&rc);

		if (pContainer->GetItemCount() > 1)
		{
			for (int i = 0; i < pContainer->GetItemCount(); i++)
			{
				CXTPTabManagerItem* pItem = pContainer->GetItem(i);
				if (pItem->GetData() == (DWORD_PTR)this)
				{
					CRect rcItem = pItem->GetRect();
					rc = CRect(CPoint(rc.left + rcItem.left, rc.top + rcItem.top), rcItem.Size());
					break;
				}
			}
		}
	}


	*pxLeft = rc.left;
	*pyTop = rc.top;
	*pcxWidth = rc.Width();
	*pcyHeight = rc.Height();

	return S_OK;
}

HRESULT CXTPDockingPane::AccessibleHitTest(long /*xLeft*/, long /*yTop*/, VARIANT* pvarID)
{
	pvarID->vt = VT_I4;
	pvarID->lVal = 0;

	return S_OK;
}



#ifdef _XTP_ACTIVEX

IMPLEMENT_DYNAMIC(CXTPDockingPane, CCmdTarget)

BEGIN_DISPATCH_MAP(CXTPDockingPane, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CXTPDockingPane)
	DISP_PROPERTY_EX_ID(CXTPDockingPane, "Title",  dispidTitle,  OleGetTitle,  OleSetTitle,  VT_BSTR)
	DISP_PROPERTY_ID   (CXTPDockingPane, "Id",     dispidId,     m_nID, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPDockingPane, "Handle", DISPID_VALUE, OleGetHandle, OleSetHandle, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPDockingPane, "IconId", dispidIconId, GetIconID, SetIconID, VT_I4)

	DISP_PROPERTY_EX_ID(CXTPDockingPane, "Hidden", dispidHidden, OleGetHidden, OleSetHidden, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPDockingPane, "Closed", dispidClosed, IsClosed, OleSetClosed, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPDockingPane, "Selected", dispidSelected, IsSelected, OleSetSelected, VT_BOOL)
	DISP_PROPERTY_ID(CXTPDockingPane, "Options", dispidOptions, m_dwOptions, VT_I4)

	DISP_PROPERTY_EX_ID(CXTPDockingPane, "Floating", dispidFloating, IsFloating, OleSetFloating, VT_BOOL)

	DISP_FUNCTION_ID(CXTPDockingPane, "Hide", dispidHide, Hide, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPDockingPane, "Select", dispidSelect, Select, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPDockingPane, "Close", dispidClose, Close, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPDockingPane, "AttachTo", dispidAttachTo, OleAttachTo, VT_EMPTY, VTS_DISPATCH)

	DISP_PROPERTY_ID(CXTPDockingPane, "Tag", dispidTag, m_dwData, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPDockingPane, "MinTrackSize", 15, OleGetMinTrackSize, SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPDockingPane, "MaxTrackSize", 16, OleGetMaxTrackSize, SetNotSupported, VT_DISPATCH)
	DISP_FUNCTION_ID(CXTPDockingPane, "SetHandle", 17, OleSetHandle, VT_EMPTY, VTS_HANDLE)
	DISP_PROPERTY_EX_ID(CXTPDockingPane, "TabColor", 18, OleGetTabColor, OleSetTabColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPDockingPane, "Enabled", 19, GetEnabled, SetEnabled, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPDockingPane, "ParentContainer", 20, OleGetContainer, SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPDockingPane, "Type", 21, OleGetType, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPDockingPane, "Position", 23, OleGetPosition, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPDockingPane, "Maximized", 24, OleGetMaximized, OleSetMaximized, VT_BOOL)
	DISP_FUNCTION_ID(CXTPDockingPane, "GetClientRect", 25, OleGetClientRect, VT_EMPTY, VTS_PI4 VTS_PI4 VTS_PI4 VTS_PI4)

	DISP_PROPERTY_EX_ID(CXTPDockingPane, "TabCaption", 26, OleGetTabCaption, SetTabCaption, VT_BSTR)
	DISP_PROPERTY_EX_ID(CXTPDockingPane, "TitleToolTip", 27, OleGetTitleToolTip, SetTitleToolTip, VT_BSTR)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IXTPDockingPane to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the
//  dispinterface in the .ODL file.

// {24A0C93B-61BC-4172-893C-CD3463CF2E01}
static const IID IID_IDockingPane =
{ 0x24a0c93b, 0x61bc, 0x4172, { 0x89, 0x3c, 0xcd, 0x34, 0x63, 0xcf, 0x2e, 0x1 } };

// {ABC0C93B-61BC-4172-893C-CD3463CF2E01}
static const IID IID_IDockingPaneContainer =
{ 0xabc0c93b, 0x61bc, 0x4172, { 0x89, 0x3c, 0xcd, 0x34, 0x63, 0xcf, 0x2e, 0x1 } };


IMPLEMENT_OLETYPELIB_EX(CXTPDockingPane, IID_IDockingPane)

BSTR CXTPDockingPane::OleGetTabCaption()
{
	return GetTabCaption().AllocSysString();
}

BSTR CXTPDockingPane::OleGetTitleToolTip()
{
	return GetTitleToolTip().AllocSysString();
}

LPDISPATCH CXTPDockingPane::OleGetContainer()
{
	return m_pParentContainer ? m_pParentContainer->OleGetDispatch(TRUE) : NULL;
}

int CXTPDockingPane::OleGetPosition()
{
	return GetDockingPaneManager()->GetPaneDirection(this);
}

int CXTPDockingPane::OleGetType()
{
	return m_type;
}

void CXTPDockingPane::OleSetHandle(long hWnd)
{
	if ((m_hwndChild != NULL) && (m_hwndChild != (HWND)hWnd))
	{
		Detach();
	}

	m_hwndChild = (HWND)hWnd;

	if (m_hwndChild)
	{
		::ShowWindow(m_hwndChild, SW_HIDE);
		GetDockingPaneManager()->_Redraw();
		if (m_pParentContainer)
		{
			SetWindowLong(m_hwndChild, GWL_STYLE,
				GetWindowLong(m_hwndChild, GWL_STYLE) | WS_CHILD);

			::SetParent(m_hwndChild, m_pParentContainer->GetPaneHwnd());
		}

	}
}

long CXTPDockingPane::OleGetHandle()
{
	return (long)m_hwndChild;
}


BSTR CXTPDockingPane::OleGetTitle()
{
	return m_strTitle.AllocSysString();
}

void CXTPDockingPane::OleSetTitle(LPCTSTR bstrTitle)
{
	SetTitle(bstrTitle);
}


COLORREF CXTPDockingPane::OleGetTabColor()
{
	return m_clrItemTab;
}

void CXTPDockingPane::OleSetTabColor(COLORREF clr)
{
	m_clrItemTab = AxTranslateColor(clr);
	InvalidatePane(FALSE);
}

void CXTPDockingPane::OleAttachTo(LPDISPATCH pNeighbourDisp)
{
	ASSERT(pNeighbourDisp);
	if (!pNeighbourDisp)
	{
		AfxThrowOleException(E_INVALIDARG);
		return;
	}

	GetDockingPaneManager()->AttachPane(this,
		CXTPDockingPane::FromDispatch(pNeighbourDisp));
}

void CXTPDockingPane::OleSetHidden(BOOL bHidden)
{
	if (bHidden) Hide();
	else if (IsHidden()) GetDockingPaneManager()->ToggleAutoHide(this);

}
void CXTPDockingPane::OleSetClosed(BOOL bClosed)
{
	if (bClosed)
		Close();
	else if (IsClosed())
		Select();

}
void CXTPDockingPane::OleSetSelected(BOOL bSelected)
{
	if (bSelected) GetDockingPaneManager()->ShowPane(this, FALSE);
}

void CXTPDockingPane::OleSetFloating(BOOL bFloating)
{
	if (IsFloating() == bFloating)
		return;

	if (bFloating)
	{
		if (IsHidden())
		{
			GetDockingPaneManager()->ToggleDocking(this);
		}
	}

	GetDockingPaneManager()->ToggleDocking(this);
}

LPDISPATCH CXTPDockingPane::OleGetMinTrackSize()
{
	CXTPDockingPaneTrackSize* pSize = new CXTPDockingPaneTrackSize(&m_ptMinTrackSize);
	return pSize->GetIDispatch(FALSE);
}

LPDISPATCH CXTPDockingPane::OleGetMaxTrackSize()
{
	CXTPDockingPaneTrackSize* pSize = new CXTPDockingPaneTrackSize(&m_ptMaxTrackSize);
	return pSize->GetIDispatch(FALSE);
}

BOOL CXTPDockingPane::OleGetHidden()
{
	return IsHidden();
}

LPDISPATCH CXTPDockingPane::OleGetDispatch(BOOL bAddRef)
{
	return GetIDispatch(bAddRef);
}

BOOL CXTPDockingPane::OleGetMaximized()
{
	return m_pParentContainer ? ((CXTPDockingPaneTabbedContainer*)m_pParentContainer)->IsPaneMaximized() : FALSE;
}

void CXTPDockingPane::OleSetMaximized(BOOL bMaximized)
{
	if (!m_pParentContainer)
		return;

	if (bMaximized)
	{
		((CXTPDockingPaneTabbedContainer*)m_pParentContainer)->Maximize();
	}
	else
	{
		((CXTPDockingPaneTabbedContainer*)m_pParentContainer)->Restore();
	}
}

void CXTPDockingPane::OleGetClientRect(long* pLeft, long* pTop, long* pRight, long* pBottom)
{
	if (pLeft == NULL || pTop == NULL || pRight == NULL || pBottom == NULL)
	{
		return;
	}

	CRect rc = m_rcWindow;

	CWnd* pSite = GetDockingPaneManager()->GetSite();
	CXTPOleControl* pControl = SiteToControl(pSite);
	if (pControl)
	{
		pControl->PixelToUnits(rc);
	}

	*pLeft = rc.left;
	*pRight = rc.right;
	*pTop = rc.top;
	*pBottom = rc.bottom;

}

//////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneTrackSize


CXTPDockingPaneTrackSize::CXTPDockingPaneTrackSize(CPoint* pPoint)
	: m_pPoint(pPoint)
{
	EnableAutomation();
	EnableTypeLib();

	ASSERT(m_pPoint);
	m_nWidth = m_pPoint->x;
	m_nHeight = m_pPoint->y;
}


BEGIN_DISPATCH_MAP(CXTPDockingPaneTrackSize, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CXTPDockingPaneTrackSize)
	DISP_PROPERTY_NOTIFY(CXTPDockingPaneTrackSize, "Width", m_nWidth, OnPropertyChanged, VT_I4)
	DISP_PROPERTY_NOTIFY(CXTPDockingPaneTrackSize, "Height", m_nHeight, OnPropertyChanged, VT_I4)
	DISP_FUNCTION(CXTPDockingPaneTrackSize, "SetSize", OleSetSize, VT_EMPTY, VTS_I4 VTS_I4)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// {2235891D-B4E9-45c7-AAF6-EF340E785590}
static const GUID IID_IDockingPaneTrackSize =
{ 0x2235891d, 0xb4e9, 0x45c7, { 0xaa, 0xf6, 0xef, 0x34, 0xe, 0x78, 0x55, 0x90 } };

BEGIN_INTERFACE_MAP(CXTPDockingPaneTrackSize, CCmdTarget)
	INTERFACE_PART(CXTPDockingPaneTrackSize, IID_IDockingPaneTrackSize, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPDockingPaneTrackSize, IID_IDockingPaneTrackSize)

void CXTPDockingPaneTrackSize::OnPropertyChanged()
{
	OleSetSize(m_nWidth, m_nHeight);
}

void CXTPDockingPaneTrackSize::OleSetSize(long nWidth, long nHeight)
{
	m_pPoint->x = m_nWidth = nWidth;
	m_pPoint->y = m_nHeight = nHeight;
}

#endif


BEGIN_INTERFACE_MAP(CXTPDockingPane, CCmdTarget)
	INTERFACE_PART(CXTPDockingPane, IID_IAccessible, ExternalAccessible)
#ifdef _XTP_ACTIVEX
	INTERFACE_PART(CXTPDockingPane, IID_IDockingPane, Dispatch)
	INTERFACE_PART(CXTPDockingPane, IID_IDockingPaneContainer, Dispatch)
#endif
END_INTERFACE_MAP()
