// XTPRibbonBackstageView.cpp : implementation file
//
// This file is a part of the XTREME RIBBON MFC class library.
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
#include "Common/XTPResourceImage.h"
#include "Common/XTPVC80Helpers.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPWinThemeWrapper.h"

#include "Controls/Util/XTPControlTheme.h"
#include "Controls/Button/XTPButton.h"
#include "Controls/Button/XTPButtonTheme.h"

#include "Controls/Resize/XTPResizeRect.h"
#include "Controls/Resize/XTPResizePoint.h"
#include "Controls/Resize/XTPResize.h"
#include "Controls/Resize/XTPResizePropertyPage.h"

#include "XTPRibbonBackstageControls.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
//  CXTPRibbonBackstageButton

IMPLEMENT_DYNAMIC(CXTPRibbonBackstageButton, CXTPButton)

CXTPRibbonBackstageButton::CXTPRibbonBackstageButton()
{
	SetTheme(new CXTPRibbonBackstageButtonTheme());
}

void CXTPRibbonBackstageButton::ShowShadow(BOOL bShowShadow)
{
	CXTPRibbonBackstageButtonTheme* pTheme = (CXTPRibbonBackstageButtonTheme*)GetTheme();

	pTheme->m_bShowShadow = bShowShadow;
}

void CXTPRibbonBackstageButton::SetTabStyle(BOOL bTabStyle)
{
	CXTPRibbonBackstageButtonTheme* pTheme = (CXTPRibbonBackstageButtonTheme*)GetTheme();

	pTheme->m_bTabStyle = bTabStyle;
}

BEGIN_MESSAGE_MAP(CXTPRibbonBackstageButton, CXTPButton)
//{{AFX_MSG_MAP(CXTPRibbonBackstageSeparator)
	ON_WM_LBUTTONDOWN()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CXTPRibbonBackstageButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	CXTPButton::OnLButtonDown(nFlags, point);

}


//////////////////////////////////////////////////////////////////////////
// CXTPRibbonBackstageButtonTheme

CXTPRibbonBackstageButtonTheme::CXTPRibbonBackstageButtonTheme()
{
	m_bOffsetHiliteText = FALSE;
	m_bShowShadow = FALSE;
	m_bTabStyle = FALSE;

}


void CXTPRibbonBackstageButtonTheme::DrawButtonBackground(CDC* pDC, CXTPButton* pButton)
{
	BOOL bPressed = pButton->IsPushed();
	BOOL bSelected = pButton->IsHighlighted();
	BYTE bStyle = pButton->GetButtonStyle();
	BOOL bDefault = bStyle == BS_DEFPUSHBUTTON;
	BOOL bEnabled = pButton->IsWindowEnabled();
	BOOL bChecked = pButton->GetChecked();
	CRect rc(pButton->GetButtonRect());


	if (m_bTabStyle)
	{
		CWnd* pWndParent = pButton->GetParent();

		CXTPWindowRect rcButtonScreen(pButton);

		for (CWnd* pChildWnd = pWndParent->GetWindow(GW_CHILD); pChildWnd != NULL; pChildWnd = pChildWnd->GetWindow(GW_HWNDNEXT))
		{
			if (pChildWnd->IsKindOf(RUNTIME_CLASS(CXTPRibbonBackstageSeparator)))

			{
				CXTPWindowRect rcSeparatorScreen(pChildWnd);

				CRect rcIntersect;
				if (rcIntersect.IntersectRect(rcButtonScreen, rcSeparatorScreen))
				{
					pButton->ScreenToClient(rcSeparatorScreen);

					CXTPResourceImage* pImage = XTPResourceImages()->LoadFile(_T("BACKSTAGEPANESEPARATOR"));
					if (pImage)
					{
						pImage->DrawImage(pDC, CRect(rcSeparatorScreen.right - 1, rcSeparatorScreen.top, rcSeparatorScreen.right, rcSeparatorScreen.bottom), pImage->GetSource(0, 1), CRect(0, 30, 0, 30));
					}
				}
			}
		}
	}

	if (pButton->GetFlatStyle() && !bSelected && !bPressed && !bChecked)
		return;

	if (pButton->GetUseVisualStyle() && m_themeButton->IsAppThemed())
	{
		DrawButtonVisualStyleBackground(pDC, pButton);
		return;
	}

	if (m_bShowShadow && bEnabled)
	{
		CXTPResourceImage* pImage = XTPResourceImages()->LoadFile(_T("BACKSTAGEBUTTONSHADOW"));

		if (pImage)
		{
			pImage->DrawImage(pDC, CRect(rc.left, rc.bottom - 2, rc.right, rc.bottom), pImage->GetSource(), CRect(3, 0, 3, 0), 0xFF00FF);
			rc.bottom -= 2;
		}
	}



	CXTPResourceImage* pImage = XTPResourceImages()->LoadFile(_T("BACKSTAGEBUTTON"));
	if (!pImage)
		pImage = XTPResourceImages()->LoadFile(_T("PUSHBUTTON"));

	ASSERT(pImage);
	if (!pImage)
	{
		CXTPButtonTheme::DrawButtonBackground(pDC, pButton);
		return;
	}

	int nState = bDefault ? 4 : 0;

	if (!bEnabled)
	{
		nState = 3;
	}
	else if (bChecked && !bSelected && !bPressed) nState = 2;
	else if (bChecked && bSelected && !bPressed) nState = 1;
	else if ((bSelected && bPressed) || pButton->IsPushed()) nState = 2;
	else if (bSelected || bPressed) nState = 1;

	if (nState != -1)
	{
		pImage->DrawImage(pDC, rc, pImage->GetSource(nState, 5), CRect(4, 4, 4, 4), 0xFF00FF);
	}

	if (bChecked && m_bTabStyle)
	{
		CXTPResourceImage* pImage = XTPResourceImages()->LoadFile(_T("BACKSTAGEBUTTONGLYPH"));

		if (pImage)
		{
			CRect rcSrc(pImage->GetSource());
			int y = rc.CenterPoint().y;
			pImage->DrawImage(pDC, CRect(rc.right - rcSrc.Width(), y - rcSrc.Height() / 2, rc.right, y - rcSrc.Height() / 2 + rcSrc.Height()), rcSrc);
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// CXTPRibbonBackstageSeparator

IMPLEMENT_DYNAMIC(CXTPRibbonBackstageSeparator, CStatic);

CXTPRibbonBackstageSeparator::CXTPRibbonBackstageSeparator()
{
	const unsigned short _cb[] = {0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc};

	CBitmap bmp;
	bmp.CreateBitmap(8, 8, 1, 1, _cb);

	m_brush.CreatePatternBrush(&bmp);

	m_bVerticalStyle = FALSE;
}

CXTPRibbonBackstageSeparator::~CXTPRibbonBackstageSeparator()
{
}


BEGIN_MESSAGE_MAP(CXTPRibbonBackstageSeparator, CStatic)
//{{AFX_MSG_MAP(CXTPRibbonBackstageSeparator)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CXTPRibbonBackstageSeparator::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CXTPRibbonBackstageSeparator message handlers

void CXTPRibbonBackstageSeparator::OnPaint()
{
	CPaintDC dcPaint(this); // device context for painting
	CXTPBufferDC dc(dcPaint);

	CXTPClientRect rc(this);

	HBRUSH hBrush = (HBRUSH)GetParent()->SendMessage(WM_CTLCOLORSTATIC, (WPARAM)dc.GetSafeHdc(), (LPARAM)GetSafeHwnd());
	if (hBrush)
	{
		::FillRect(dc.GetSafeHdc(), rc, hBrush);
	}
	else
	{
		dc.FillSolidRect(rc, RGB(255, 255, 255));
	}

	if (m_bVerticalStyle)
	{
		CXTPResourceImage* pImage = XTPResourceImages()->LoadFile(_T("BACKSTAGEPANESEPARATOR"));
		if (pImage)
		{
			pImage->DrawImage(&dc, CRect(rc.right - 1, rc.top, rc.right, rc.bottom), pImage->GetSource(0, 1), CRect(0, 30, 0, 30));
		}

	}
	else
	{
		CBrush* pOldBrush = (CBrush*)dc.SelectObject(&m_brush);

		dc.SetTextColor(RGB(255, 255, 255));
		dc.SetBkColor(RGB(199, 200, 201));

		PatBlt(dc, rc.left, rc.Height() / 2, rc.Width(), 1, PATCOPY);

		dc.SelectObject(pOldBrush);
	}
}


//////////////////////////////////////////////////////////////////////////
// CXTPRibbonBackstagePage

IMPLEMENT_DYNAMIC(CXTPRibbonBackstagePage, CXTPResizePropertyPage)

CXTPRibbonBackstagePage::CXTPRibbonBackstagePage(UINT nID) : CXTPResizePropertyPage(nID)
{
	LOGFONT lf;
	XTPDrawHelpers()->GetIconLogFont(&lf);

	if (XTPDrawHelpers()->FontExists(_T("Segoe UI")))
	{
		lf.lfHeight = 30;
		STRCPY_S(lf.lfFaceName, LF_FACESIZE, _T("Segoe UI"));
	}
	else
	{
		lf.lfHeight = 25;
	}
	m_fntTitle.CreateFontIndirect(&lf);


	lf.lfHeight = 18;
	lf.lfWeight = FW_BOLD;

	m_fntCaption.CreateFontIndirect(&lf);
}

BEGIN_MESSAGE_MAP(CXTPRibbonBackstagePage, CXTPResizePropertyPage)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


HBRUSH CXTPRibbonBackstagePage::OnCtlColor(CDC* pDC, CWnd* /*pWnd*/, UINT /*nCtlColor*/)
{
	pDC->SetBkColor(RGB(255, 255, 255));

	return (HBRUSH)GetStockObject(WHITE_BRUSH);
}



/////////////////////////////////////////////////////////////////////////////
// CXTPRibbonBackstageLabel

CXTPRibbonBackstageLabel::CXTPRibbonBackstageLabel()
{
	m_clrText = 0;
}

CXTPRibbonBackstageLabel::~CXTPRibbonBackstageLabel()
{
}


BEGIN_MESSAGE_MAP(CXTPRibbonBackstageLabel, CStatic)
//{{AFX_MSG_MAP(CXTPRibbonBackstageLabel)
ON_WM_PAINT()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTPRibbonBackstageLabel message handlers

void CXTPRibbonBackstageLabel::OnPaint()
{
	CPaintDC dcPaint(this); // device context for painting

	CXTPClientRect rc(this);

	CXTPBufferDC dc(dcPaint);

	dc.FillSolidRect(rc, RGB(255, 255, 255));


	CXTPFontDC font(&dc, GetFont());

	CString strText;
	GetWindowText(strText);

	dc.SetTextColor(m_clrText);

	dc.DrawText(strText, rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);
}






