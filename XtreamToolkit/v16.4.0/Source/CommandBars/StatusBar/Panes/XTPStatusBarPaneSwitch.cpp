#include <StdAfx.h>

#include <Common/XTPDrawHelpers.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPImageManager.h>
#include <Common/XTPToolTipContext.h>
#include <Common/XTPMarkupRender.h>
#include <Common/XTPPropExchange.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPHookManager.h>
#include <Common/XTPVC80Helpers.h>
#include <Common/Hook/XTPWinEventHook.h>
#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollInfo.h>
#include <Common/ScrollBar/XTPScrollBarPaintManager.h>

#include <CommandBars/XTPCommandBarsDefines.h>
#include <CommandBars/XTPPaintManager.h>
#include <CommandBars/Slider/XTPSliderPaintManager.h>
#include <CommandBars/StatusBar/XTPStatusBar.h>
#include <CommandBars/StatusBar/XTPStatusBarPaintManager.h>
#include <CommandBars/StatusBar/Panes/XTPStatusBarPane.h>
#include <CommandBars/StatusBar/Panes/XTPStatusBarPaneSwitch.h>

//////////////////////////////////////////////////////////////////////////
// CXTPStatusBarSwitchPane

IMPLEMENT_DYNAMIC(CXTPStatusBarSwitchPane, CXTPStatusBarPane)

CXTPStatusBarSwitchPane::CXTPStatusBarSwitchPane()
{
	m_nStyle |= SBPS_NOBORDERS;

	m_pHighlighted = NULL;
	m_bButton = TRUE;

	m_rcMargins.SetRect(0, 0, 0, 0);
	m_rcPadding.SetRect(0, 0, 0, 0);
}

CXTPStatusBarSwitchPane::~CXTPStatusBarSwitchPane()
{
	RemoveAll();
}

void CXTPStatusBarSwitchPane::RemoveAll()
{
	for (int i = 0; i < m_arrSwitches.GetSize(); i++)
	{
		delete m_arrSwitches[i];
	}
	m_arrSwitches.RemoveAll();
	m_pHighlighted = NULL;
}

BOOL CXTPStatusBarSwitchPane::SetSwitches(const UINT* lpIDArray, int nIDCount)
{
	for (int i = 0; i < nIDCount; i++)
	{
		if (!AddSwitch(lpIDArray[i]))
			return FALSE;
	}

	m_pStatusBar->RecalcLayout();

	return TRUE;
}

CXTPStatusBarSwitchPane::SWITCH* CXTPStatusBarSwitchPane::AddSwitch(UINT nID)
{
	CString strToolTip;
	strToolTip.LoadString(nID);

	return AddSwitch(nID, strToolTip);
}

CXTPStatusBarSwitchPane::SWITCH* CXTPStatusBarSwitchPane::AddSwitch(UINT nID, LPCTSTR lpszToolTip)
{
	SWITCH* pSwitch = new SWITCH();
	pSwitch->nWidth = 0;
	pSwitch->strToolTip = lpszToolTip;
	pSwitch->nID = nID;
	pSwitch->bHighlighted = FALSE;
	pSwitch->bChecked = FALSE;
	pSwitch->bPressed = FALSE;
	pSwitch->bEnabled = TRUE;

	m_arrSwitches.Add(pSwitch);
	return pSwitch;
}



void CXTPStatusBarSwitchPane::RemoveSwitch(UINT nID)
{
	for (int i = 0; i < m_arrSwitches.GetSize(); i++)
	{
		if (m_arrSwitches[i]->nID == nID)
		{
			delete m_arrSwitches[i];
			m_arrSwitches.RemoveAt(i);
			return;
		}
	}
}

CXTPStatusBarSwitchPane::SWITCH* CXTPStatusBarSwitchPane::FindSwitch(UINT nID) const
{
	for (int i = 0; i < m_arrSwitches.GetSize(); i++)
	{
		if (m_arrSwitches[i]->nID == nID)
		{
			return m_arrSwitches[i];
		}
	}
	return NULL;
}

void CXTPStatusBarSwitchPane::EnableSwitch(UINT nID, BOOL bEnabled)
{
	for (int i = 0; i < m_arrSwitches.GetSize(); i++)
	{
		if (m_arrSwitches[i]->nID == nID)
		{
			m_arrSwitches[i]->bEnabled = bEnabled;
			return;
		}
	}
}

int CXTPStatusBarSwitchPane::GetWidth()
{
	CXTPStatusBarPaintManager* pPaintManager = m_pStatusBar->GetStatusBarPaintManager();
	if (!pPaintManager)
		return 0 ;

	return m_cxText = pPaintManager->DrawStatusBarSwitchPane(NULL, CRect(0, 0, 0, 0), this);
}

void CXTPStatusBarSwitchPane::OnDraw(CDC* pDC, CRect rcItem)
{
	CXTPStatusBarPaintManager* pPaintManager = m_pStatusBar->GetStatusBarPaintManager();

	if (pPaintManager)
	{
		pPaintManager->DrawStatusBarSwitchPane(pDC, rcItem, this);
	}
}

int CXTPStatusBarSwitchPane::GetSwitchCount() const
{
	return (int)m_arrSwitches.GetSize();
}

CXTPStatusBarSwitchPane::SWITCH* CXTPStatusBarSwitchPane::GetSwitch(int nIndex) const
{
	return m_arrSwitches[nIndex];
}

CXTPStatusBarSwitchPane::SWITCH* CXTPStatusBarSwitchPane::HitTest(CPoint point, CRect& rcItem) const
{
	rcItem = GetRect();

	for (int i = 0; i < m_arrSwitches.GetSize(); i++)
	{
		rcItem.right = rcItem.left + m_arrSwitches[i]->nWidth;

		if (rcItem.PtInRect(point))
			return m_arrSwitches[i];

		rcItem.left = rcItem.right;
	}

	return NULL;
}

INT_PTR CXTPStatusBarSwitchPane::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	CRect rcItem(0, 0, 0, 0);

	SWITCH* pHighlighted = HitTest(point, rcItem);
	if (!pHighlighted)
		return -1;

	INT_PTR nHit = pHighlighted->nID;

	CString strTip = pHighlighted->strToolTip;
	if (strTip.IsEmpty())
		return -1;

	CXTPToolTipContext::FillInToolInfo(pTI, m_pStatusBar->m_hWnd, rcItem, nHit, strTip);

	return nHit;
}

void CXTPStatusBarSwitchPane::OnMouseMove(CPoint point)
{
	CRect rcItem(0, 0, 0, 0);

	SWITCH* pHighlighted = HitTest(point, rcItem);
	if (pHighlighted != m_pHighlighted)
	{
		if (m_pHighlighted)
		{
			m_pHighlighted->bHighlighted = FALSE;
		}

		m_pHighlighted = pHighlighted;

		if (m_pHighlighted)
		{
			m_pHighlighted->bHighlighted = TRUE;
		}

		Redraw();
	}
}

void CXTPStatusBarSwitchPane::OnLButtonDown(CPoint point)
{
	CRect rcItem;

	SWITCH* pHighlighted = HitTest(point, rcItem);
	if (!pHighlighted || !pHighlighted->bEnabled)
		return;

	InternalAddRef();

	pHighlighted->bPressed = TRUE;
	Redraw();

	m_pStatusBar->SetCapture();

	BOOL bClick = FALSE, bPressed;

	while (m_pStatusBar && ::GetCapture() == m_pStatusBar->GetSafeHwnd())
	{
		MSG msg;

		if (!::GetMessage(&msg, NULL, 0, 0))
			break;

		switch (msg.message)
		{
			case WM_MOUSEMOVE:
				point = CPoint((short signed)LOWORD(msg.lParam), (short signed)HIWORD(msg.lParam));

				bPressed = rcItem.PtInRect(point);

				if (bPressed != pHighlighted->bPressed)
				{
					pHighlighted->bPressed = bPressed;
					Redraw();
				}

				break;

			case WM_LBUTTONUP:
				bClick = pHighlighted->bPressed;
				goto ExitLoop;

			case WM_KEYDOWN:
				if (msg.wParam != VK_ESCAPE)
					break;

			case WM_CANCELMODE:
			case WM_RBUTTONDOWN:
				goto ExitLoop;

			default:
				DispatchMessage (&msg);
				break;
		}
	}

ExitLoop:
	ReleaseCapture();

	pHighlighted->bPressed = FALSE;
	Redraw();

	if (bClick && m_pStatusBar)
	{
		m_pStatusBar->OnSwitchPaneClick(this, pHighlighted->nID);
	}

	InternalRelease();
}

void CXTPStatusBarSwitchPane::SetChecked(UINT nID)
{
	for (int i = 0; i < m_arrSwitches.GetSize(); i++)
	{
		SWITCH* pSwitch = m_arrSwitches[i];
		BOOL bChecked = pSwitch->nID == nID;

		if (pSwitch->bChecked != bChecked)
		{
			pSwitch->bChecked = bChecked;
			Redraw();
		}
	}
}


#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPStatusBarSwitchPane, CXTPStatusBarPane)
	DISP_FUNCTION_ID(CXTPStatusBarSwitchPane, "AddSwitch", 120, OleAddSwitch, VT_EMPTY, VTS_I4 VTS_BSTR)
	DISP_FUNCTION_ID(CXTPStatusBarSwitchPane, "RemoveSwitch", 121, OleRemoveSwitch, VT_EMPTY, VTS_I4)
	DISP_PROPERTY_EX_ID(CXTPStatusBarSwitchPane, "Checked", 122, OleGetChecked, SetChecked, VT_I4)
	DISP_FUNCTION_ID(CXTPStatusBarSwitchPane, "EnableSwitch", 123, EnableSwitch, VT_EMPTY, VTS_I4 VTS_BOOL)

	DISP_PROPERTY_PARAM_ID(CXTPStatusBarSwitchPane, "SwitchTooltip", 124, OleGetSwitchTooltip, OleSetSwitchTooltip, VT_BSTR, VTS_I4)
END_DISPATCH_MAP()

// {4569783D-A5D8-4569-8690-592295C5583A}
static const GUID IID_StatusBarSwitchPane =
{ 0x4569783d, 0xa5d8, 0x4569, { 0x86, 0x90, 0x59, 0x22, 0x95, 0xc5, 0x58, 0x3a } };


BEGIN_INTERFACE_MAP(CXTPStatusBarSwitchPane, CXTPStatusBarPane)
	INTERFACE_PART(CXTPStatusBarSwitchPane, IID_StatusBarSwitchPane, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPStatusBarSwitchPane, IID_StatusBarSwitchPane)

void CXTPStatusBarSwitchPane::OleAddSwitch(UINT nID, LPCTSTR lpszToolTip)
{
	AddSwitch(nID, lpszToolTip);

	m_pStatusBar->RecalcLayout();
}

void CXTPStatusBarSwitchPane::OleRemoveSwitch(UINT nID)
{
	RemoveSwitch(nID);

	m_pStatusBar->RecalcLayout();
}

BSTR CXTPStatusBarSwitchPane::OleGetSwitchTooltip(int nID)
{
	SWITCH* pSwitch = FindSwitch(nID);
	if (!pSwitch)
		return 0;

	return pSwitch->strToolTip.AllocSysString();
}


void CXTPStatusBarSwitchPane::OleSetSwitchTooltip(int nID, LPCTSTR lpszTooltip)
{
	SWITCH* pSwitch = FindSwitch(nID);
	if (!pSwitch)
		return;

	pSwitch->strToolTip = lpszTooltip;
}

int CXTPStatusBarSwitchPane::OleGetChecked()
{
	for (int i = 0; i < m_arrSwitches.GetSize(); i++)
	{
		SWITCH* pSwitch = m_arrSwitches[i];

		if (pSwitch->bChecked)
		{
			return pSwitch->nID;
		}
	}

	return 0;
}

#endif
