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
#include <Common/Controls/Progress/XTPProgressBase.h>

#include <CommandBars/XTPCommandBarsDefines.h>
#include <CommandBars/XTPPaintManager.h>
#include <CommandBars/XTPControl.h>
#include <CommandBars/XTPControlProgress.h>
#include <CommandBars/Progress/XTPProgressPaintManager.h>
#include <CommandBars/Slider/XTPSliderPaintManager.h>
#include <CommandBars/StatusBar/XTPStatusBar.h>
#include <CommandBars/StatusBar/Panes/XTPStatusBarPane.h>
#include <CommandBars/StatusBar/Panes/XTPStatusBarPaneProgress.h>


//////////////////////////////////////////////////////////////////////////
// CXTPStatusBarProgressPane

IMPLEMENT_DYNAMIC(CXTPStatusBarProgressPane, CXTPStatusBarPane)

CXTPStatusBarProgressPane::CXTPStatusBarProgressPane()
{
	m_nStyle |= SBPS_NOBORDERS;
	m_rcMargins.SetRect(0, 0, 0, 0);
	m_rcPadding.SetRect(0, 0, 0, 0);
	m_cxText = 100;
}

CXTPStatusBarProgressPane::~CXTPStatusBarProgressPane()
{

}

CXTPProgressPaintManager* CXTPStatusBarProgressPane::GetProgressPaintManager() const
{
	return m_pStatusBar->GetPaintManager()->GetProgressPaintManager();
}

void CXTPStatusBarProgressPane::RedrawProgress()
{
	Redraw();
}

CRect CXTPStatusBarProgressPane::GetProgressRect()
{
	CRect rcItem(GetRect());
	rcItem.DeflateRect(m_rcPadding);

	int nHeight = GetProgressPaintManager()->m_cyProgress;
	if (rcItem.Height() > nHeight)
	{
		rcItem.top = (rcItem.top + rcItem.bottom - nHeight) / 2;
		rcItem.bottom = rcItem.top + nHeight;
	}

	return rcItem;
}

void CXTPStatusBarProgressPane::OnDraw(CDC* pDC, CRect /*rcItem*/)
{
	CXTPProgressPaintManager* pPaintManager = GetProgressPaintManager();

	if (pPaintManager)
	{
		pPaintManager->DrawProgress(pDC, this);
	}
}


#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPStatusBarProgressPane, CXTPStatusBarPane)
	DISP_PROPERTY_ID(CXTPStatusBarProgressPane, "Min", 120, m_nMin, VT_I4)
	DISP_PROPERTY_ID(CXTPStatusBarProgressPane, "Max", 121, m_nMax, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPStatusBarProgressPane, "Value", 122, OleGetPos, OleSetPos, VT_I4)
	DISP_FUNCTION_ID(CXTPStatusBarProgressPane, "RedrawProgress", 333, OleRedrawProgress, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()

// {7889783D-A5D8-4569-8690-592295C5583A}
static const GUID IID_StatusBarProgressPane =
{ 0x7889783d, 0xa5d8, 0x4569, { 0x86, 0x90, 0x59, 0x22, 0x95, 0xc5, 0x58, 0x3a } };

BEGIN_INTERFACE_MAP(CXTPStatusBarProgressPane, CXTPStatusBarPane)
	INTERFACE_PART(CXTPStatusBarProgressPane, IID_StatusBarProgressPane, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPStatusBarProgressPane, IID_StatusBarProgressPane)

int CXTPStatusBarProgressPane::OleGetPos()
{
	return GetPos();
}

void CXTPStatusBarProgressPane::OleSetPos(int nPos)
{
	SetPos(nPos);
}

void CXTPStatusBarProgressPane::OleRedrawProgress()
{
	if (this && m_pStatusBar && m_pStatusBar->GetSafeHwnd() && IsVisible())
	{
		m_pStatusBar->InvalidateRect(GetRect());
		UpdateWindow(m_pStatusBar->GetSafeHwnd());
	}
}

#endif
