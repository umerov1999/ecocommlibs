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


//////////////////////////////////////////////////////////////////////////
// CXTPStatusBarPane

IMPLEMENT_DYNAMIC(CXTPStatusBarPane, CCmdTarget)

CXTPStatusBarPane::CXTPStatusBarPane()
{
	m_nID = 0;
	m_cxText = 0;
	m_nStyle = 0;
	m_nBestFit = 0;

	m_nIndex = -1;
	m_pStatusBar = NULL;

	m_hWndPane = NULL;
	m_bAutoDeleteWnd = FALSE;
	m_dwHideFlags = 0;

	m_clrText = (COLORREF)-1;
	m_clrTextGray = (COLORREF)-1;
	m_clrBackground = (COLORREF)-1;

	m_nIconIndex = -1;
	m_nAlignment = DT_LEFT;

	m_rcMargins.SetRect(0, 0, 0, 0);
	m_rcPadding.SetRect(3, 1, 3, 1);

	m_bButton = FALSE;
	m_bPressed = FALSE;
	m_pMarkupUIElement = NULL;

	m_rcPane.SetRectEmpty();

	m_bBeginGroup = TRUE;

	m_bCustomizable = TRUE;

	m_bIsDark = FALSE;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPStatusBarPane::~CXTPStatusBarPane()
{
	CWnd* pWnd = CWnd::FromHandlePermanent(m_hWndPane);

	if (pWnd && ::IsWindow(pWnd->GetSafeHwnd()))
	{
		pWnd->DestroyWindow();

		if (m_bAutoDeleteWnd)
		{
			delete pWnd;
		}
	}

	XTPMarkupReleaseElement(m_pMarkupUIElement);
}

void CXTPStatusBarPane::SetButton(BOOL bButton)
{
	m_bButton = bButton;
	ShowBorders(FALSE);
}

BOOL CXTPStatusBarPane::IsButton() const
{
	return m_bButton;
}


CRect CXTPStatusBarPane::GetRect() const
{
	return m_rcPane;
}

void CXTPStatusBarPane::Redraw() const
{
	if (this && m_pStatusBar && m_pStatusBar->GetSafeHwnd() && IsVisible())
	{
		m_pStatusBar->InvalidateRect(GetRect());
	}
}

CRect CXTPStatusBarPane::GetMargins() const
{
	return m_rcMargins;
}

CRect CXTPStatusBarPane::GetPadding() const
{
	return m_rcPadding;
}

void CXTPStatusBarPane::SetMargins(int nLeft, int nTop, int nRight, int nBottom)
{
	m_rcMargins.SetRect(nLeft, nTop, nRight, nBottom);
}

void CXTPStatusBarPane::SetPadding(int nLeft, int nTop, int nRight, int nBottom)
{
	m_rcPadding.SetRect(nLeft, nTop, nRight, nBottom);
}

void CXTPStatusBarPane::SetText(LPCTSTR lpszText)
{
	CString strText = lpszText;
	if (m_strText == strText)
		return;

	XTPMarkupReleaseElement(m_pMarkupUIElement);
	m_strText = strText;

	if (m_pStatusBar->GetMarkupContext())
	{
		m_pMarkupUIElement = XTPMarkupParseText(m_pStatusBar->GetMarkupContext(), lpszText);
	}

	if (!IsVisible())
		return;

	if (m_cxText == 0 && m_nBestFit != GetBestFit())
	{
		m_pStatusBar->RecalcLayout();
	}
	else
	{
		Redraw();
	}
}

CString CXTPStatusBarPane::GetText() const
{
	CString strText = m_strText;

	if (GetStatusBar()->IsAllCaps())
	{
		strText.MakeUpper();
	}

	return strText;
}

void CXTPStatusBarPane::SetWidth(int cxText)
{
	if (m_cxText != cxText)
	{
		m_cxText = cxText;
		m_pStatusBar->RecalcLayout();
	}
}

int CXTPStatusBarPane::GetWidth()
{
	return m_cxText <= 0 ? GetBestFit() : m_cxText;
}

void CXTPStatusBarPane::SetStyle(UINT nStyle)
{
	if (m_nStyle != nStyle)
	{
		// if the pane is changing SBPS_STRETCH, then...
		if ((m_nStyle ^ nStyle) & SBPS_STRETCH)
		{
			// ... we need to re-layout the panes
			m_nStyle = nStyle;
			m_pStatusBar->RecalcLayout();
		}

		// use SetPaneText, since it updates the style and text
		m_nStyle = nStyle;
		Redraw();
	}
}
UINT CXTPStatusBarPane::GetStyle() const
{
	return m_nStyle;
}

UINT CXTPStatusBarPane::GetID() const
{
	return m_nID;
}

int CXTPStatusBarPane::GetIndex() const
{
	return m_nIndex;
}

CXTPStatusBar* CXTPStatusBarPane::GetStatusBar() const
{
	return m_pStatusBar;
}

void CXTPStatusBarPane::Remove()
{
	m_pStatusBar->RemoveAt(m_nIndex);
}

void CXTPStatusBarPane::SetVisible(BOOL bVisible, BOOL bRecalcLayout)
{
	DWORD dwHideFlags = bVisible ? m_dwHideFlags &~ xtpHideGeneric : m_dwHideFlags | xtpHideGeneric;

	if (m_dwHideFlags != dwHideFlags)
	{
		m_dwHideFlags = dwHideFlags;

		if (bRecalcLayout) m_pStatusBar->RecalcLayout();
	}
}

BOOL CXTPStatusBarPane::IsVisible() const
{
	return m_dwHideFlags == xtpNoHide;
}

void CXTPStatusBarPane::OnDraw(CDC* pDC, CRect rcItem)
{
	CXTPStatusBarPaintManager* pPaintManager = m_pStatusBar->GetStatusBarPaintManager();

	if (pPaintManager)
	{
		pPaintManager->DrawStatusBarPaneEntry(pDC, rcItem, this);
	}
}

void CXTPStatusBarPane::BestFit()
{
	SetWidth(GetBestFit());
}

int CXTPStatusBarPane::GetBestFit()
{
	int cxText = 0;

	if (m_pMarkupUIElement)
	{
		XTPMarkupSetDefaultFont(m_pStatusBar->m_pMarkupContext, (HFONT)GetTextFont()->GetSafeHandle(), COLORREF_NULL);
		cxText = XTPMarkupMeasureElement(m_pMarkupUIElement).cx + m_rcPadding.left + m_rcPadding.right;
	}
	else
	{
		CClientDC dc(m_pStatusBar);
		CXTPFontDC font(&dc, GetTextFont());
		cxText = dc.GetTextExtent(GetText()).cx + m_rcPadding.left + m_rcPadding.right;
	}

	CXTPImageManagerIcon* pIcon = GetImage();
	if (pIcon) cxText += pIcon->GetWidth() + (!m_strText.IsEmpty() ? 2 : 0);

	return cxText;
}

void CXTPStatusBarPane::SetTextColor(COLORREF clrText)
{
#ifdef _XTP_ACTIVEX
	m_clrText = AxTranslateColor(clrText);
#else
	m_clrText = clrText;
#endif
	Redraw();
}

COLORREF CXTPStatusBarPane::GetTextColor() const
{
	return m_clrText;
}

COLORREF CXTPStatusBarPane::GetTextGrayColor() const
{
	return m_clrTextGray;
}

void CXTPStatusBarPane::SetTextGrayColor(COLORREF clrText)
{
#ifdef _XTP_ACTIVEX
	m_clrTextGray = AxTranslateColor(clrText);
#else
	m_clrTextGray = clrText;
#endif
	Redraw();
}

void CXTPStatusBarPane::SetBackgroundColor(COLORREF clrBackground)
{
#ifdef _XTP_ACTIVEX
	m_clrBackground = AxTranslateColor(clrBackground);
#else
	m_clrBackground = clrBackground;
#endif
	Redraw();
}

COLORREF CXTPStatusBarPane::GetBackgroundColor() const
{
	return m_clrBackground;
}

CFont* CXTPStatusBarPane::GetTextFont()
{
	ASSERT(m_pStatusBar);
	return (m_pStatusBar && m_fntText.m_hObject == NULL) ?
		m_pStatusBar->GetFont(): &m_fntText;
}

void CXTPStatusBarPane::SetTextFont(CFont* pFntText)
{
	ASSERT(pFntText);
	if (!pFntText)
		return;

	LOGFONT lf;
	pFntText->GetLogFont(&lf);

	SetTextFont(&lf);
}

void CXTPStatusBarPane::SetTextFont(PLOGFONT pLogfText)
{
	ASSERT(pLogfText);

	m_fntText.DeleteObject();
	m_fntText.CreateFontIndirect(pLogfText);

	Redraw();
}

CXTPImageManagerIcon* CXTPStatusBarPane::GetImage() const
{
	if (m_nIconIndex != -1)
		return m_pStatusBar->GetImageManager()->GetImage(m_nIconIndex, 0);

	return NULL;
}

int CXTPStatusBarPane::GetIconIndex() const
{
	return m_nIconIndex;
}

void CXTPStatusBarPane::SetIconIndex(int nIconIndex)
{
	if (m_nIconIndex != nIconIndex)
	{
		m_nIconIndex = nIconIndex;
		m_pStatusBar->RecalcLayout();
	}
}

void CXTPStatusBarPane::SetTextAlignment(int nAlign)
{
	if (m_nAlignment != nAlign)
	{
		m_nAlignment = nAlign;
		Redraw();
	}
}

int CXTPStatusBarPane::GetTextAlignment() const
{
	return m_nAlignment;
}

void CXTPStatusBarPane::SetTooltip(LPCTSTR lpszTooltip)
{
	m_strToolTip = lpszTooltip;
}

CString CXTPStatusBarPane::GetTooltip() const
{
	return m_strToolTip;
}

void CXTPStatusBarPane::SetEnabled(BOOL bEnabled)
{
	SetStyle(bEnabled ? GetStyle() & ~SBPS_DISABLED : GetStyle() | SBPS_DISABLED);
}

BOOL CXTPStatusBarPane::IsEnabled() const
{
	return ((m_nStyle & SBPS_DISABLED) == 0);
}

BOOL CXTPStatusBarPane::IsChecked() const
{
	return ((m_nStyle & SBPS_POPOUT) == SBPS_POPOUT);
}

void CXTPStatusBarPane::SetChecked(BOOL bChecked)
{
	SetStyle(!bChecked ? GetStyle() & ~SBPS_POPOUT : GetStyle() | SBPS_POPOUT);
}

void CXTPStatusBarPane::OnMouseMove(CPoint /*point*/)
{

}

INT_PTR CXTPStatusBarPane::OnToolHitTest(CPoint /*point*/, TOOLINFO* /*pTI*/) const
{
	return -1;
}

void CXTPStatusBarPane::OnLButtonDown(CPoint point)
{
	if (!m_bButton)
		return;

	InternalAddRef();

	m_bPressed = TRUE;
	Redraw();

	CRect rcButton = GetRect();
	BOOL bClick = FALSE, bPressed;

	m_pStatusBar->SetCapture();

	while (m_pStatusBar && ::GetCapture() == m_pStatusBar->GetSafeHwnd())
	{
		MSG msg;

		if (!::GetMessage(&msg, NULL, 0, 0))
			break;

		switch (msg.message)
		{
			case WM_MOUSEMOVE:
				point = CPoint((short signed)LOWORD(msg.lParam), (short signed)HIWORD(msg.lParam));

				bPressed = rcButton.PtInRect(point);

				if (bPressed != m_bPressed)
				{
					m_bPressed = bPressed;
					Redraw();
				}
				break;

			case WM_LBUTTONUP:
				bClick = m_bPressed;
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

	m_bPressed = FALSE;
	Redraw();

	if (bClick && m_pStatusBar)
	{
		m_pStatusBar->OnPaneClick(this);
	}

	InternalRelease();
}



#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPStatusBarPane, CCmdTarget)
	DISP_PROPERTY_EX_ID(CXTPStatusBarPane, "Text", 1, OleGetText, SetText, VT_BSTR)
	DISP_PROPERTY_EX_ID(CXTPStatusBarPane, "Width", 2, GetWidth, SetWidth, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPStatusBarPane, "Style", 3, GetStyle, SetStyle, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPStatusBarPane, "Id", 4, GetID, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPStatusBarPane, "Index", 5, GetIndex, SetNotSupported, VT_I4)

	DISP_FUNCTION_ID(CXTPStatusBarPane, "Remove", 6, Remove, VT_EMPTY, VTS_NONE)
	DISP_PROPERTY_EX_ID(CXTPStatusBarPane, "Visible", 7, IsVisible, OleSetVisible, VT_BOOL)

	DISP_PROPERTY_EX_ID(CXTPStatusBarPane, "BackgroundColor", DISPID_BACKCOLOR, GetBackgroundColor, SetBackgroundColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPStatusBarPane, "TextColor", DISPID_FORECOLOR, GetTextColor, SetTextColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPStatusBarPane, "TextGrayColor", 25, GetTextGrayColor, SetTextGrayColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPStatusBarPane, "Font", DISPID_FONT, OleGetFont, OleSetFont, VT_FONT)
	DISP_PROPERTY_EX_ID(CXTPStatusBarPane, "IconIndex", 8, GetIconIndex, SetIconIndex, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPStatusBarPane, "Alignment", 9, GetTextAlignment, SetTextAlignment, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPStatusBarPane, "Handle", 10, OleGetHandle, OleSetHandle, VT_HANDLE)
	DISP_PROPERTY_ID(CXTPStatusBarPane, "Tooltip", 11, m_strToolTip, VT_BSTR)
	DISP_PROPERTY_EX_ID(CXTPStatusBarPane, "Button", 12, IsButton, SetButton, VT_BOOL)

	DISP_PROPERTY_ID(CXTPStatusBarPane, "BeginGroup", 15, m_bBeginGroup, VT_BOOL)
	DISP_FUNCTION_ID(CXTPStatusBarPane, "SetPadding", 14, SetPadding, VT_EMPTY, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CXTPStatusBarPane, "SetMargins", 20, SetMargins, VT_EMPTY, VTS_I4 VTS_I4 VTS_I4 VTS_I4)

	DISP_PROPERTY_ID(CXTPStatusBarPane, "Caption", 16, m_strCaption, VT_BSTR)
	DISP_PROPERTY_ID(CXTPStatusBarPane, "Value", 17, m_strValue, VT_BSTR)

	DISP_PROPERTY_EX_ID(CXTPStatusBarPane, "Enabled", 18, IsEnabled, SetEnabled, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPStatusBarPane, "Checked", 19, IsChecked, SetChecked, VT_BOOL)

	DISP_PROPERTY_ID(CXTPStatusBarPane, "Customizable", 21, m_bCustomizable, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPStatusBarPane, "HideFlags", 22, OleGetHideFlags, OleSetHideFlags, VT_I4)
	DISP_FUNCTION_ID(CXTPStatusBarPane, "RedrawPane", 333, OleRedrawPane, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPStatusBarPane, "GetRect", 23, OleGetRect, VT_EMPTY, VTS_PI4 VTS_PI4 VTS_PI4 VTS_PI4)

END_DISPATCH_MAP()

// {2B69783D-A5D8-4569-8690-592295C5583A}
static const GUID IID_StatusBarPane =
{ 0x2b69783d, 0xa5d8, 0x4569, { 0x86, 0x90, 0x59, 0x22, 0x95, 0xc5, 0x58, 0x3a } };


BEGIN_INTERFACE_MAP(CXTPStatusBarPane, CCmdTarget)
	INTERFACE_PART(CXTPStatusBarPane, IID_StatusBarPane, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPStatusBarPane, IID_StatusBarPane)

void CXTPStatusBarPane::OleRedrawPane()
{
	if (this && m_pStatusBar && m_pStatusBar->GetSafeHwnd() && IsVisible())
	{
		m_pStatusBar->InvalidateRect(GetRect());
		UpdateWindow(m_pStatusBar->GetSafeHwnd());
	}
}

int CXTPStatusBarPane::OleGetHideFlags()
{
	return m_dwHideFlags;
}

void CXTPStatusBarPane::OleSetHideFlags(int dwHideFlags)
{
	if (m_dwHideFlags != (DWORD)dwHideFlags)
	{
		m_dwHideFlags = (DWORD)dwHideFlags;

		m_pStatusBar->RecalcLayout();
	}
}

void CXTPStatusBarPane::OleSetVisible(BOOL bVisible)
{
	SetVisible(bVisible);
}

BSTR CXTPStatusBarPane::OleGetText()
{
	return GetText().AllocSysString();
}

LPFONTDISP CXTPStatusBarPane::OleGetFont()
{
	return m_fntText.GetSafeHandle() ? AxCreateOleFont(&m_fntText, this, (LPFNFONTCHANGED)&CXTPStatusBarPane::OleSetFont)
		: AxCreateOleFont(m_pStatusBar->GetFont(), this, (LPFNFONTCHANGED)&CXTPStatusBarPane::OleSetFont);
}

void CXTPStatusBarPane::OleSetFont(LPFONTDISP pFontDisp)
{
	ASSERT((pFontDisp == NULL) ||
		AfxIsValidAddress(pFontDisp, sizeof(IDispatch), FALSE));

	AxCreateFontFromDispatch(&m_fntText, pFontDisp);
}

HWND CXTPStatusBarPane::OleGetHandle()
{
	return m_hWndPane;
}

void CXTPStatusBarPane::OleSetHandle(HWND hWnd)
{
	CWnd* pWnd = CWnd::FromHandle((HWND)hWnd);

	if (pWnd)
	{
		pWnd->SetParent(m_pStatusBar);
		pWnd->ModifyStyle(WS_POPUP, WS_CHILD);

		::ShowWindow(hWnd, IsVisible() ? SW_SHOW : SW_HIDE);
	}

	m_hWndPane = hWnd;

	m_pStatusBar->PositionControls();
}

void OlePixelToUnits(CWnd* pWnd, CRect& rc);

void CXTPStatusBarPane::OleGetRect(long* pLeft, long* pTop, long* pRight, long* pBottom)
{
	if (pLeft == NULL || pTop == NULL || pRight == NULL || pBottom == NULL)
	{
		return;
	}

	CRect rc(GetRect());

	if (!IsVisible())
	{
		rc.SetRectEmpty();
	}
	else
	{
		OlePixelToUnits(m_pStatusBar->GetParent(), rc);
	}

	*pLeft = rc.left;
	*pRight = rc.right;
	*pTop = rc.top;
	*pBottom = rc.bottom;
}

#endif
