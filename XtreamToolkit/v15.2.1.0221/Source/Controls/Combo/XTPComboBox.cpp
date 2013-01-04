// XTPEdit.cpp : implementation of the CXTPEdit class.
//
// This file is a part of the XTREME CONTROLS MFC class library.
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
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPResourceImage.h"
#include "Common/XTPWinThemeWrapper.h"

#include "../Util/XTPControlTheme.h"
#include "XTPComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CXTPComboBox::CXTPComboBox()
{
	m_bHighlighted = FALSE;
	m_bFocused = FALSE;

	m_bFlatStyle = FALSE;
	m_nTheme = xtpControlThemeDefault;
	m_bUseVisualStyle = TRUE;

	m_bPreSubclassInit = TRUE;

	m_bAutoComp = FALSE;
	m_bDisableAC = FALSE;

	m_crBack = COLORREF_NULL;
	m_crText = COLORREF_NULL;

	m_themeComboBox = new CXTPWinThemeWrapper();
}

CXTPComboBox::~CXTPComboBox()
{
	SAFE_DELETE(m_themeComboBox);

}

IMPLEMENT_DYNAMIC(CXTPComboBox, CComboBox)

BEGIN_MESSAGE_MAP(CXTPComboBox, CComboBox)
	ON_WM_CREATE()
	ON_WM_PAINT()

	ON_WM_MOUSEMOVE()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_MESSAGE_VOID(WM_MOUSELEAVE, OnMouseLeave)

	ON_WM_SYSCOLORCHANGE()

	ON_CONTROL_REFLECT_EX(CBN_EDITUPDATE, OnEditUpdate)
	ON_CONTROL_REFLECT_EX(CBN_CLOSEUP, OnEndSel)

END_MESSAGE_MAP()



void CXTPComboBox::RefreshMetrics()
{
	RefreshXtremeColors();
	m_clrBorderDisabled = m_clrBorderNormal = m_clrBorderHot = GetSysColor(COLOR_3DSHADOW);

	m_clrButtonNormal = m_clrButtonHot = m_clrButtonPressed = GetSysColor(COLOR_BTNFACE);
	m_clrButtonArrowNormal = m_clrButtonArrowHot = m_clrButtonArrowPressed =  GetXtremeColor(COLOR_BTNTEXT);

	if (m_nTheme == xtpControlThemeFlat)
	{
		m_clrBorderNormal = m_clrBorderHot = GetSysColor(COLOR_WINDOWFRAME);
	}
	if (m_nTheme == xtpControlThemeVisualStudio2010)
	{
		m_clrBorderNormal = RGB(133, 145, 162);
		m_clrBorderHot = RGB(229, 195, 101);
		m_clrBorderDisabled = RGB(164, 173, 186);
		m_clrButtonNormal = RGB(241, 243, 248);
		m_grcButton = RGB(255,255,255);
		m_grcButtonHilite.SetStandardValue(RGB(255, 252, 242), RGB(255, 236, 181));
		m_grcButtonPushed = RGB(255, 232, 166);
		m_clrButtonArrowNormal = RGB(27, 41, 62);
		m_clrButtonArrowPressed = RGB(27, 41, 62);
	}
	if (m_nTheme == xtpControlThemeOfficeXP ||
		m_nTheme == xtpControlThemeOffice2003 ||
		m_nTheme == xtpControlThemeVisualStudio2005 ||
		m_nTheme == xtpControlThemeVisualStudio2008)
	{
		m_clrBorderHot = GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER);

		m_clrButtonHot = GetXtremeColor(XPCOLOR_HIGHLIGHT);
		m_clrButtonPressed = GetXtremeColor(XPCOLOR_HIGHLIGHT_PUSHED);

		m_clrButtonArrowHot = GetXtremeColor(XPCOLOR_HIGHLIGHT_TEXT);
		m_clrButtonArrowPressed = GetXtremeColor(XPCOLOR_PUSHED_TEXT);

		if (m_nTheme == xtpControlThemeVisualStudio2005 ||
			m_nTheme == xtpControlThemeVisualStudio2008)
		{
			m_grcButton.SetStandardValue(XTPColorManager()->LightColor(GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_WINDOW), 40), GetXtremeColor(COLOR_3DFACE));

			switch (XTPColorManager()->GetCurrentSystemTheme())
			{
			case xtpSystemThemeBlue:
			case xtpSystemThemeRoyale:
				{
					m_grcButtonHilite.SetStandardValue(RGB(193, 210, 238));
					m_grcButtonPushed.SetStandardValue(RGB(152, 181, 226));
				}
				break;

			case xtpSystemThemeAero:
				{
					m_grcButtonHilite.SetStandardValue(RGB(228, 245, 252), RGB(124, 177, 205));
					m_grcButtonPushed.SetStandardValue(RGB(237, 248, 253), RGB(129, 192, 224));
				}
				break;

			case xtpSystemThemeOlive:
				{
					m_grcButtonHilite.SetStandardValue(RGB(182, 198, 141));
					m_grcButtonPushed.SetStandardValue(GetXtremeColor(COLOR_HIGHLIGHT));
				}
				break;

			case xtpSystemThemeSilver:
				{
					m_grcButtonHilite.SetStandardValue(RGB(255, 242, 200), RGB(255, 210, 148));
					m_grcButtonPushed.SetStandardValue(RGB(254, 149, 82), RGB(255, 207, 139));
				}
				break;

			default:
				{
					m_grcButtonHilite.SetStandardValue(XTPColorManager()->LightColor(GetXtremeColor(COLOR_HIGHLIGHT), GetXtremeColor(COLOR_WINDOW), 30));
					m_grcButtonPushed.SetStandardValue(XTPColorManager()->LightColor(GetXtremeColor(COLOR_HIGHLIGHT), GetXtremeColor(COLOR_WINDOW), 50));
				}
				break;
			}
		}
	}
	if (m_nTheme == xtpControlThemeUltraFlat)
	{
		COLORREF clrFace = GetXtremeColor(COLOR_3DFACE);
		COLORREF clrWindow = GetXtremeColor(COLOR_WINDOW);

		m_clrButtonPressed = GetXtremeColor(COLOR_3DSHADOW);
		m_clrButtonPressed = XTPColorManager()->LightColor(m_clrButtonPressed, clrFace, 0x32);

		m_clrButtonHot = clrFace;
		m_clrButtonHot = XTPColorManager()->LightColor(m_clrButtonHot, clrWindow, 0x1E);
	}

	if (m_nTheme == xtpControlThemeOffice2003)
	{
		XTPColorManager()->RefreshColors();
		XTPCurrentSystemTheme systemTheme = XTPColorManager()->GetCurrentSystemTheme();
		switch (systemTheme)
		{
		case xtpSystemThemeBlue:
		case xtpSystemThemeRoyale:
		case xtpSystemThemeAero:
			m_clrBorderNormal = RGB(147, 181, 231);
			m_clrBorderHot = RGB(0, 0, 128);
			m_grcButton.SetStandardValue(RGB(227, 239, 255), RGB(147, 181, 231));
			break;

		case xtpSystemThemeOlive:
			m_clrBorderNormal = RGB(194, 206, 159);
			m_clrBorderHot = RGB(63, 93, 56);
			m_grcButton.SetStandardValue(RGB(236, 240, 213), RGB(194, 206, 159));
			break;

		case xtpSystemThemeSilver:
			m_clrBorderNormal = RGB(186, 185, 205);
			m_clrBorderHot = RGB(75, 75, 11);
			m_grcButton.SetStandardValue(RGB(233, 231, 241), RGB(186, 185, 205));
			break;

		default:
			m_clrBorderNormal = GetXtremeColor(COLOR_3DSHADOW);
			m_clrBorderHot = GetXtremeColor(COLOR_3DSHADOW);
			m_grcButton.SetStandardValue(
				XTPColorManager()->LightColor(GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_WINDOW), 0x14a),
				XTPColorManager()->LightColor(GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_WINDOW), 0x320));
			break;
		}

		m_grcButtonHilite.SetStandardValue(RGB(255, 242, 200), RGB(255, 212, 151));
		m_grcButtonPushed.SetStandardValue(RGB(254, 142, 75), RGB(255, 207, 139));

		if (systemTheme != xtpSystemThemeUnknown)
		{
			m_clrButtonHot = RGB(255, 238, 194);
			m_clrButtonPressed = RGB(254, 128, 62);
			m_clrButtonArrowPressed = m_clrButtonArrowHot = 0;
		}
	}
	if (m_nTheme == xtpControlThemeResource)
	{
		m_clrBorderHot = XTPResourceImages()->GetImageColor(_T("RibbonComboBox"), _T("BorderHilite"));
		m_clrBorderNormal = XTPResourceImages()->GetImageColor(_T("RibbonComboBox"), _T("Border"));
		m_clrBorderDisabled = XTPResourceImages()->GetImageColor(_T("RibbonComboBox"), _T("BorderDisabled"));
	}

	m_themeComboBox->OpenTheme(0, L"COMBOBOX");

	if (!m_hWnd)
		return;

	CClientDC dc(this);
	CXTPFontDC font(&dc, CWnd::GetFont());

	TEXTMETRIC tm;
	dc.GetTextMetrics(&tm);
	m_nThumbWidth = max(15, tm.tmAveCharWidth * 2 + 4);

	m_nThumbWidth = min(m_nThumbWidth, GetSystemMetrics(SM_CXVSCROLL) + 1);
}



void CXTPComboBox::Init()
{
	RefreshMetrics();

	Invalidate(FALSE);

	HWND hWndEdit = 0;
	GetDlgItem(1001, &hWndEdit);

	if (hWndEdit)
	{
		m_wndEdit.SubclassWindow(hWndEdit);
	}
}

void CXTPComboBox::PreSubclassWindow()
{
	CComboBox::PreSubclassWindow();

	if (m_bPreSubclassInit)
	{
		// Initialize the control.
		Init();
	}
}

int CXTPComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Initialize the control.
	Init();

	return 0;
}

BOOL CXTPComboBox::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CComboBox::PreCreateWindow(cs))
		return FALSE;

	// When creating controls dynamically Init() must
	// be called from OnCreate() and not from
	// PreSubclassWindow().

	m_bPreSubclassInit = FALSE;

	return TRUE;
}

void CXTPComboBox::SetTheme(XTPControlTheme nTheme)
{
	m_nTheme = nTheme;

	m_bUseVisualStyle = (m_nTheme == xtpControlThemeDefault);

	RefreshMetrics();

	if (m_hWnd) Invalidate(FALSE);
}

void CXTPComboBox::SetUseVisualStyle(BOOL bUseVisualStyle/* = TRUE*/)
{
	m_bUseVisualStyle = bUseVisualStyle;

	RefreshMetrics();
	if (m_hWnd) Invalidate(FALSE);
}

void CXTPComboBox::OnPaint()
{
	CPaintDC paintDC(this);
	CXTPClientRect rc(this);

	CXTPBufferDC dcMem(paintDC);

	HBRUSH hBrush = GetClientBrush(&dcMem);

	FillRect(dcMem, rc, hBrush);

	if ((GetStyle() & 3) == CBS_SIMPLE)
	{
		CComboBox::DefWindowProc(WM_PAINT, (WPARAM)dcMem.m_hDC, 0);
	}
	else if (m_bUseVisualStyle && m_themeComboBox->IsAppThemed())
	{
		CComboBox::DefWindowProc(WM_PAINT, (WPARAM)dcMem.m_hDC, 0);

		if (!m_themeComboBox->IsAppThemeReady())
		{
			dcMem.SelectClipRgn(NULL);

			BOOL bPressed = (GetKeyState(VK_LBUTTON) < 0) &&  ::SendMessage(m_hWnd, CB_GETDROPPEDSTATE, 0, 0L) != 0;
			BOOL bSelected = m_bHighlighted || m_bFocused || (::GetCapture() == m_hWnd);
			BOOL bEnabled = IsWindowEnabled();
			BOOL bRightAlign = GetExStyle() & WS_EX_RIGHT;

			int nState = !bEnabled? CBXS_DISABLED: bPressed? CBXS_PRESSED: bSelected? CBXS_HOT: CBXS_NORMAL;

			int nThumbWidth = GetSystemMetrics(SM_CXVSCROLL);

			if (bRightAlign)
				FillRect(dcMem, CRect(rc.left, rc.top, rc.left + 3 + nThumbWidth, rc.bottom), hBrush);
			else
				FillRect(dcMem, CRect(rc.right - nThumbWidth - 3, rc.top, rc.right, rc.bottom), hBrush);

			CRect rectClient(rc.left + 2, rc.top + 2, rc.right - nThumbWidth - 2, rc.bottom - 2);
			if (bRightAlign)
				rectClient.OffsetRect(nThumbWidth, 0);

			dcMem.ExcludeClipRect(rectClient);

			m_themeComboBox->DrawThemeBackground(dcMem, 0, nState, rc, NULL);

			CRect rcDropDownButton(rc);
			rcDropDownButton.DeflateRect(1, 1);
			if (bRightAlign) rcDropDownButton.right = rcDropDownButton.left + nThumbWidth;
			else rcDropDownButton.left = rcDropDownButton.right - nThumbWidth;

			m_themeComboBox->DrawThemeBackground(dcMem, CP_DROPDOWNBUTTON, nState, rcDropDownButton, NULL);

		}
	}
	else if (m_nTheme == xtpControlThemeResource)
	{
		CXTPResourceImage* pImage = XTPResourceImages()->LoadFile(_T("TOOLBARBUTTONSCOMBODROPDOWN"));
		if (!pImage)
		{
			CComboBox::DefWindowProc(WM_PAINT, (WPARAM)dcMem.m_hDC, 0);
		}
		else
		{
			BOOL bPressed = ::SendMessage(m_hWnd, CB_GETDROPPEDSTATE, 0, 0L) != 0;
			BOOL bSelected = m_bFocused || m_bHighlighted;
			BOOL bEnabled = IsWindowEnabled();
			BOOL bRightAlign = GetExStyle() & WS_EX_RIGHT;

			DefWindowProc(WM_PAINT, (WPARAM)dcMem.GetSafeHdc(), 0);
			dcMem.SelectClipRgn(NULL);

			DrawFrame(dcMem, rc, 3, hBrush);

			if (bRightAlign)
				FillRect(dcMem, CRect(rc.left, rc.top, rc.left + GetSystemMetrics(SM_CXVSCROLL) + 3, rc.bottom), hBrush);
			else
				FillRect(dcMem, CRect(rc.right - GetSystemMetrics(SM_CXVSCROLL) - 2, rc.top, rc.right, rc.bottom), hBrush);

			CRect rcBtn(rc.right - m_nThumbWidth, rc.top, rc.right, rc.bottom);
			if (bRightAlign)
				rcBtn = CRect(rc.left, rc.top, rc.left + m_nThumbWidth, rc.bottom);

			if (!bEnabled)
			{
				dcMem.Draw3dRect(rc, m_clrBorderDisabled, m_clrBorderDisabled);
				pImage->DrawImage(&dcMem, rcBtn, pImage->GetSource(4, 5), CRect(2, 2, 2, 2));
			}
			else if (bPressed)
			{
				dcMem.Draw3dRect(rc, m_clrBorderNormal, m_clrBorderNormal);
				pImage->DrawImage(&dcMem, rcBtn, pImage->GetSource(3, 5), CRect(2, 2, 2, 2));
			}
			else if (bSelected)
			{
				dcMem.Draw3dRect(rc, m_clrBorderNormal, m_clrBorderNormal);
				pImage->DrawImage(&dcMem, rcBtn, pImage->GetSource(2, 5), CRect(2, 2, 2, 2));
			}
			else
			{
				pImage->DrawImage(&dcMem, rcBtn, pImage->GetSource(0, 5), CRect(2, 2, 2, 2));
				if (m_bFlatStyle)
				{
					dcMem.Draw3dRect(rc, ::GetSysColor(COLOR_3DFACE), ::GetSysColor(COLOR_3DFACE));
					rc.DeflateRect(1,1);
					dcMem.Draw3dRect(rc, ::GetSysColor(COLOR_WINDOW), ::GetSysColor(COLOR_WINDOW));
				}
				else
				{
					dcMem.Draw3dRect(rc, m_clrBorderNormal, m_clrBorderNormal);
				}
			}

			CPoint pt = rcBtn.CenterPoint();

			pImage = XTPResourceImages()->LoadFile(_T("TOOLBARBUTTONDROPDOWNGLYPH"));

			int x = rcBtn.left + ((rcBtn.Width()-5)/2);
			int y = rcBtn.top + ((rcBtn.Height()-4)/2)+1;

			CRect rcArrow(x, y, x + 5, y + 4);

			pImage->DrawImage(&dcMem, rcArrow, pImage->GetSource(!bEnabled ? 3 : bSelected ? 1 : 0, 4),
				CRect(0, 0, 0, 0), 0xff00ff);

			if (!bSelected && !bPressed)
			{
				if (m_bFlatStyle)
				{
					CXTPPenDC pen(dcMem, bEnabled ? ::GetSysColor(COLOR_WINDOW) : ::GetSysColor(COLOR_3DFACE));
					dcMem.MoveTo(rcBtn.left,rcBtn.top+1);
					dcMem.LineTo(rcBtn.left,rcBtn.bottom-1);
				}
				else
				{
					CXTPPenDC pen(dcMem, bEnabled ? m_clrBorderNormal :  m_clrBorderDisabled);
					dcMem.MoveTo(rcBtn.left,rcBtn.top+1);
					dcMem.LineTo(rcBtn.left,rcBtn.bottom-1);
				}
			}
		}
	}
	else if (
		m_nTheme == xtpControlThemeOffice2000 ||
		m_nTheme == xtpControlThemeOfficeXP ||
		m_nTheme == xtpControlThemeOffice2003 ||
		m_nTheme == xtpControlThemeVisualStudio2005 ||
		m_nTheme == xtpControlThemeVisualStudio2008 ||
		m_nTheme == xtpControlThemeVisualStudio2010 ||
		m_nTheme == xtpControlThemeFlat ||
		m_nTheme == xtpControlThemeUltraFlat)
	{
		BOOL bPressed = ::SendMessage(m_hWnd, CB_GETDROPPEDSTATE, 0, 0L) != 0;
		BOOL bSelected = m_bFocused || m_bHighlighted;
		BOOL bEnabled = IsWindowEnabled();
		BOOL bRightAlign = GetExStyle() & WS_EX_RIGHT;

		COLORREF clrBorders = bSelected? m_clrBorderHot : m_clrBorderNormal;

		DefWindowProc(WM_PAINT, (WPARAM)dcMem.GetSafeHdc(), 0);
		dcMem.SelectClipRgn(NULL);

		DrawFrame(dcMem, rc, 3, hBrush);

		if (bRightAlign)
			FillRect(dcMem, CRect(rc.left, rc.top, rc.left + GetSystemMetrics(SM_CXVSCROLL) + 3, rc.bottom), hBrush);
		else
			FillRect(dcMem, CRect(rc.right - GetSystemMetrics(SM_CXVSCROLL) - 3, rc.top, rc.right, rc.bottom), hBrush);

		CRect rcBtn(rc.right - m_nThumbWidth, rc.top, rc.right, rc.bottom);
		if (bRightAlign)
			rcBtn = CRect(rc.left, rc.top, rc.left + m_nThumbWidth, rc.bottom);

		if (!bSelected && !bPressed && bEnabled && m_bFlatStyle)
		{
			dcMem.Draw3dRect(rc, GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_3DFACE));

			if (m_nTheme == xtpControlThemeOffice2003 ||
				m_nTheme == xtpControlThemeVisualStudio2005 ||
				m_nTheme == xtpControlThemeVisualStudio2008 ||
				m_nTheme == xtpControlThemeVisualStudio2010)
			{
				rcBtn.DeflateRect(1, 1);
				XTPDrawHelpers()->GradientFill(&dcMem, &rcBtn, m_grcButton, FALSE);
			}
			else
			{
				COLORREF clrButton = m_clrButtonNormal;
				rcBtn.DeflateRect(1, 1);
				dcMem.FillSolidRect(rcBtn, clrButton);
			}

			DrawFrame(dcMem, rcBtn, 1, hBrush);
		}
		else if (m_nTheme == xtpControlThemeOffice2000)
		{
			if (m_bFlatStyle && !bEnabled)
			{
				dcMem.Draw3dRect(rc, GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DSHADOW));
				rcBtn.DeflateRect(1, 1);
			}
			else
			{
				dcMem.Draw3dRect(rc, GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DHILIGHT));
				dcMem.Draw3dRect(rc.left + 1, rc.top + 1, rc.Width() - 2, rc.Height() - 2,
					GetXtremeColor(COLOR_BTNFACE), GetXtremeColor(COLOR_BTNFACE));

				COLORREF clrButton = bPressed ? m_clrButtonPressed : bSelected ? m_clrButtonHot : m_clrButtonNormal;
				rcBtn.DeflateRect(1, 1);
				dcMem.FillSolidRect(rcBtn, clrButton);

				dcMem.Draw3dRect(rcBtn.left + 1, rcBtn.top + 1, rcBtn.Width() - 2, rcBtn.Height() - 2,
					GetXtremeColor(bPressed ? COLOR_3DSHADOW : COLOR_3DHILIGHT), GetXtremeColor(bPressed ? COLOR_3DHILIGHT : COLOR_3DSHADOW));
			}
		}
		else
		{
			dcMem.Draw3dRect(rc, bEnabled ? clrBorders : GetXtremeColor(COLOR_3DSHADOW), bEnabled ? clrBorders : GetXtremeColor(COLOR_3DSHADOW));

			if ((
				m_nTheme == xtpControlThemeOffice2003 ||
				m_nTheme == xtpControlThemeVisualStudio2005 ||
				m_nTheme == xtpControlThemeVisualStudio2008 ||
				m_nTheme == xtpControlThemeVisualStudio2010) && bEnabled)
			{
				CXTPPaintManagerColorGradient& clrButton = bPressed ? m_grcButtonPushed : bSelected ? m_grcButtonHilite : m_grcButton;

				if (m_nTheme == xtpControlThemeVisualStudio2010 && !bPressed && bSelected)
				{
					CRect rc[2];

					rc[0] = rcBtn;
					rc[0].bottom = rc[0].top + rcBtn.Height()/2;
					dcMem.FillSolidRect(&rc[0], m_grcButtonHilite.clrLight);

					rc[1] = rcBtn;
					rc[1].top += rcBtn.Height()/2;
					dcMem.FillSolidRect(&rc[1], m_grcButtonHilite.clrDark);
				}
				else
				{
					XTPDrawHelpers()->GradientFill(&dcMem, &rcBtn, clrButton, FALSE);
				}
			}
			else
			{
				COLORREF clrButton = bPressed ? m_clrButtonPressed : bSelected ? m_clrButtonHot : m_clrButtonNormal;
				dcMem.FillSolidRect(rcBtn, clrButton);
			}

			dcMem.Draw3dRect(rcBtn, bEnabled ? clrBorders : GetXtremeColor(COLOR_3DSHADOW), bEnabled ? clrBorders : GetXtremeColor(COLOR_3DSHADOW));

			rcBtn.DeflateRect(1, 1);

			if (m_nTheme == xtpControlThemeFlat && !bPressed)
				dcMem.Draw3dRect(rcBtn, GetXtremeColor(COLOR_3DHILIGHT), GetXtremeColor(COLOR_3DHILIGHT));
		}

		CPoint pt(rcBtn.CenterPoint());
		int nWidth = max(2, rcBtn.Width() / 2 - 4);

		XTPDrawHelpers()->Triangle(&dcMem, CPoint(pt.x - nWidth, pt.y - nWidth/2), CPoint(pt.x + nWidth, pt.y - nWidth/2),
			CPoint (pt.x , pt.y - nWidth/2 + nWidth),
			!bEnabled ? GetXtremeColor(XPCOLOR_DISABLED) : bPressed ? m_clrButtonArrowPressed:
			bSelected ? m_clrButtonArrowHot : m_clrButtonArrowNormal);

		if (!bEnabled && m_bFlatStyle)
		{
			CXTPPenDC pen(dcMem, m_clrButtonNormal);
			dcMem.MoveTo(rcBtn.left-1,rcBtn.top);
			dcMem.LineTo(rcBtn.left-1,rcBtn.bottom);
		}
	}
	else
	{
		DefWindowProc(WM_PAINT, (WPARAM)dcMem.GetSafeHdc(), 0);
	}

}


void CXTPComboBox::FillSolidRect(HDC hdc, int x, int y, int cx, int cy, HBRUSH hBrush)
{
	::FillRect(hdc, CRect(x, y, x + cx, y + cy), hBrush);
}

void  CXTPComboBox::DrawFrame(HDC hdc, LPRECT lprc, int nSize, HBRUSH hBrush)
{
	FillSolidRect(hdc, lprc->left, lprc->top, lprc->right - lprc->left - nSize, nSize, hBrush);
	FillSolidRect(hdc, lprc->left, lprc->top, nSize, lprc->bottom - lprc->top - nSize, hBrush);
	FillSolidRect(hdc, lprc->right - nSize, lprc->top, nSize, lprc->bottom - lprc->top, hBrush);
	FillSolidRect(hdc, lprc->left, lprc->bottom - nSize, lprc->right - lprc->left, nSize, hBrush);
}

HBRUSH CXTPComboBox::GetClientBrush(CDC* pDC)
{
	CWnd* pwndEdit = GetWindow(GW_CHILD);

	BOOL bDisabled = !IsWindowEnabled() || (pwndEdit && pwndEdit->GetStyle() & ES_READONLY);

	HBRUSH hBrush = (HBRUSH)::SendMessage(::GetParent(m_hWnd), bDisabled ? WM_CTLCOLORSTATIC : WM_CTLCOLOREDIT, (WPARAM)pDC->GetSafeHdc(), (LPARAM)m_hWnd);

	if (hBrush)
		return hBrush;

	return GetSysColorBrush(bDisabled ? COLOR_3DFACE : COLOR_WINDOW);
}

void CXTPComboBox::OnMouseLeave()
{
	Default();

	if (m_wndEdit)
	{
		CPoint pt;
		GetCursorPos(&pt);

		CXTPWindowRect rcEdit(m_wndEdit);

		if (rcEdit.PtInRect(pt))
		{
			TRACKMOUSEEVENT tme = {sizeof(TRACKMOUSEEVENT), TME_LEAVE, m_wndEdit, HOVER_DEFAULT};
			_TrackMouseEvent(&tme);

			SetHighlighted(TRUE);
			return;
		}
	}

	SetHighlighted(FALSE);
}

void CXTPComboBox::SetHighlighted(BOOL bHot)
{
	if (bHot != m_bHighlighted)
	{
		m_bHighlighted = bHot;
		RedrawFocusedFrame();

		if (bHot)
		{
			TRACKMOUSEEVENT tme = {sizeof(TRACKMOUSEEVENT), TME_LEAVE, m_hWnd, HOVER_DEFAULT};
			_TrackMouseEvent(&tme);
		}
	}
}


void CXTPComboBox::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect rc;
	GetClientRect(&rc);

	BOOL bHot = rc.PtInRect(point) && !m_bFocused;
	SetHighlighted(bHot);

	CComboBox::OnMouseMove(nFlags, point);
}


BEGIN_MESSAGE_MAP(CXTPComboBoxEditCtrl, CEdit)
//{{AFX_MSG_MAP(CComboBoxCtrl)
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_MESSAGE_VOID(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_MOUSEMOVE()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CXTPComboBoxEditCtrl::OnMouseLeave()
{
	CXTPComboBox* pControl = (CXTPComboBox*)GetParent();

	if (pControl->m_bHighlighted)
	{
		TRACKMOUSEEVENT tme = {sizeof(TRACKMOUSEEVENT), TME_LEAVE, pControl->m_hWnd, HOVER_DEFAULT};
		_TrackMouseEvent(&tme);
	}
}

void CXTPComboBoxEditCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect rc;
	GetClientRect(&rc);

	CXTPComboBox* pControl = (CXTPComboBox*)GetParent();

	BOOL bHot = rc.PtInRect(point) && !pControl->m_bFocused;
	if (bHot != pControl->m_bHighlighted)
	{
		pControl->m_bHighlighted = bHot;
		pControl->RedrawFocusedFrame();

		if (bHot)
		{
			TRACKMOUSEEVENT tme = {sizeof(TRACKMOUSEEVENT), TME_LEAVE, m_hWnd, HOVER_DEFAULT};
			_TrackMouseEvent(&tme);
		}
	}


	CEdit::OnMouseMove(nFlags, point);
}

void CXTPComboBoxEditCtrl::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);

	CXTPComboBox* pControl = (CXTPComboBox*)GetParent();
	if (pNewWnd != GetParent())
	{
		pControl->m_bFocused = FALSE;
		pControl->RedrawFocusedFrame();
	}
}

void CXTPComboBoxEditCtrl::OnSetFocus(CWnd* pOldWnd)
{
	CEdit::OnSetFocus(pOldWnd);

	CXTPComboBox* pControl = (CXTPComboBox*)GetParent();

	pControl->m_bFocused = TRUE;
	pControl->m_bHighlighted = FALSE;
	pControl->RedrawFocusedFrame();
}

void CXTPComboBox::OnKillFocus(CWnd* pNewWnd)
{
	CComboBox::OnKillFocus(pNewWnd);

	if (pNewWnd != &m_wndEdit)
	{
		m_bFocused = FALSE;
		RedrawFocusedFrame();
	}
}

void CXTPComboBox::OnSetFocus(CWnd* pOldWnd)
{
	CComboBox::OnSetFocus(pOldWnd);

	m_bFocused = TRUE;
	m_bHighlighted = FALSE;
	RedrawFocusedFrame();
}


BOOL CXTPComboBox::NeedRedraw()
{
	return (m_nTheme != 0) || (m_bUseVisualStyle && m_themeComboBox->IsAppThemed());
}


void CXTPComboBox::RedrawFocusedFrame()
{
	if (((GetStyle() & 3) != CBS_SIMPLE) && NeedRedraw())
	{
		Invalidate(FALSE);
	}
}

void CXTPComboBox::OnSysColorChange()
{
	CComboBox::OnSysColorChange();

	RefreshMetrics();
	Invalidate(FALSE);
}

BOOL CXTPComboBox::PreTranslateMessage(MSG* pMsg)
{
	// Make sure that the keystrokes continue to the edit control.
	if (pMsg->message == WM_KEYDOWN || pMsg->message == WM_KEYUP)
	{
		// if tab, return or escape key, just use default.
		switch (pMsg->wParam)
		{
		case VK_DELETE:
		case VK_BACK:
			{
				if (m_bAutoComp)
				{
					m_bDisableAC = (pMsg->message == WM_KEYDOWN);
				}
				break;
			}
		case VK_TAB:
		case VK_RETURN:
		case VK_ESCAPE:
			{
				return CComboBox::PreTranslateMessage(pMsg);
			}
		}

		// If the combo box has an edit control, don't allow
		// the framework to process accelerators, let the edit
		// control handle it instead.  GetEditSel() will return
		// CB_ERR if there is no edit control present...

		if (GetEditSel() != (DWORD)CB_ERR)
		{
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);

			return TRUE;
		}
	}

	return CComboBox::PreTranslateMessage(pMsg);
}

BOOL CXTPComboBox::OnEditUpdate()
{
	// if we are not to auto update the text, get outta here
	if (m_bAutoComp)
	{
		if (m_bDisableAC)
		{
			Default();
		}
		else
		{
			// Get the text in the edit box
			CString strItemTyped;
			GetWindowText(strItemTyped);
			int nLength = strItemTyped.GetLength();

			if (nLength >= 1)
			{
				// Currently selected range
				DWORD dwCurSel = GetEditSel();
				int nStart = LOWORD(dwCurSel);
				int nEnd = HIWORD(dwCurSel);

				// Search for, and select in, and string in the combo box that is prefixed
				// by the text in the edit box
				if (SelectString(-1, strItemTyped) == CB_ERR)
				{
					SetWindowText(strItemTyped);     // No text selected, so restore what was there before
					if (dwCurSel != (DWORD)CB_ERR)
					{
						SetEditSel(nStart, nEnd);   //restore cursor postion
					}
				}

				// Set the text selection as the additional text that we have added
				if (nEnd < nLength && dwCurSel != (DWORD)CB_ERR)
				{
					SetEditSel(nStart, nEnd);
				}
				else
				{
					SetEditSel(nLength, -1);
				}
			}
		}
	}

	return FALSE;
}

BOOL CXTPComboBox::OnEndSel()
{
	if (NeedRedraw())
	{
		Invalidate();
	}

	return FALSE;
}
