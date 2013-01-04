// XTPButtonTheme.cpp: implementation of the CXTPButtonTheme class.
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
#include "math.h"

#include "Common/XTPWinThemeWrapper.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPMarkupRender.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPResourceImage.h"
#include "Common/XTPImageManager.h"
#include "../Util/XTPControlTheme.h"

#include "XTPButton.h"
#include "XTPButtonTheme.h"

#ifndef _XTP_ACTIVEX_BUTTON
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif
#endif

#ifndef DT_HIDEPREFIX
#define DT_HIDEPREFIX       0x00100000
#endif

#ifndef WM_QUERYUISTATE
#define WM_QUERYUISTATE     0x0129
#endif

#ifndef UISF_HIDEACCEL
#define UISF_HIDEACCEL      0x2
#define UISF_HIDEFOCUS      0x1
#endif

#define BS_HORZMASK   (BS_LEFT | BS_RIGHT | BS_CENTER)
#define BS_VERTMASK   (BS_TOP | BS_BOTTOM | BS_VCENTER)

#define CBR_CLIENTRECT 0
#define CBR_CHECKBOX   1
#define CBR_RADIOBOX   2
#define CBR_CHECKTEXT  3
#define CBR_RADIOTEXT  4
#define CBR_GROUPTEXT  5
#define CBR_GROUPFRAME 6
#define CBR_PUSHBUTTON 7


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPButtonTheme::CXTPButtonTheme()
{
	m_nBorderWidth = 4;
	m_bOffsetHiliteText = TRUE;
	m_bFlatGlyphs = FALSE;
	m_bToolbarStyle = FALSE;

	m_cxBorder = GetSystemMetrics(SM_CXBORDER);
	m_cyBorder = GetSystemMetrics(SM_CYBORDER);
	m_cxEdge = GetSystemMetrics(SM_CXEDGE);
	m_cyEdge = GetSystemMetrics(SM_CYEDGE);

	m_themeButton = new CXTPWinThemeWrapper();

}

CXTPButtonTheme::~CXTPButtonTheme()
{
	SAFE_DELETE(m_themeButton);
}

void CXTPButtonTheme::RefreshMetrics(CXTPButton* /*pButton*/)
{
	m_themeButton->OpenTheme(0, m_bToolbarStyle ? L"TOOLBAR" : L"BUTTON");

	RefreshXtremeColors();

	// background colors.
	m_crBack.SetStandardValue(GetXtremeColor(COLOR_BTNFACE));

	// text colors.
	m_crText.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_crTextDisabled.SetStandardValue(GetXtremeColor(COLOR_GRAYTEXT));

	// border colors.
	m_crBorderHilite.SetStandardValue(GetXtremeColor(COLOR_BTNFACE));
	m_crBorderShadow.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));
	m_crBorder3DHilite.SetStandardValue(GetXtremeColor(COLOR_3DHILIGHT));
	m_crBorder3DShadow.SetStandardValue(GetXtremeColor(COLOR_3DDKSHADOW));
}

void CXTPButtonTheme::DrawFocusRect(CDC* pDC, CXTPButton* pButton)
{
#ifndef _XTP_ACTIVEX_BUTTON
	if ((pButton->SendMessage(WM_QUERYUISTATE) & UISF_HIDEFOCUS) == 0)
#endif
	{
		pDC->SetTextColor(GetXtremeColor(COLOR_BTNTEXT));
		pDC->SetBkColor(GetXtremeColor(COLOR_WINDOW));

		CRect rc(pButton->GetButtonRect());

		rc.DeflateRect(m_nBorderWidth, m_nBorderWidth);

		pDC->DrawFocusRect(rc);
	}
}

void CXTPButtonTheme::DrawButtonVisualStyleBackground(CDC* pDC, CXTPButton* pButton)
{
	BOOL bPressed = pButton->IsPushed();
	BOOL bHot = pButton->IsHighlighted();
	BYTE bStyle = pButton->GetButtonStyle();
	BOOL bDefault = bStyle == BS_DEFPUSHBUTTON;
	BOOL bEnabled = pButton->IsWindowEnabled();
	BOOL bChecked = pButton->GetChecked();
	CRect rc(pButton->GetButtonRect());

	if (m_bToolbarStyle)
	{
		int nState = !bEnabled? TS_DISABLED: bPressed? TS_PRESSED: bHot? TS_HOT: bChecked ? TS_PRESSED : TS_NORMAL;
		m_themeButton->DrawThemeBackground(pDC->GetSafeHdc(), TP_BUTTON, nState, rc, NULL);
	}
	else
	{
		int nState = !bEnabled? PBS_DISABLED: bPressed? PBS_PRESSED: bHot? PBS_HOT: bChecked ? PBS_PRESSED : bDefault? PBS_DEFAULTED: PBS_NORMAL;
		m_themeButton->DrawThemeBackground(pDC->GetSafeHdc(), BP_PUSHBUTTON, nState, rc, NULL);
	}
}

void CXTPButtonTheme::DrawButtonBackground(CDC* pDC, CXTPButton* pButton)
{
	BOOL bPressed = pButton->IsPushed();
	BOOL bHot = pButton->IsHighlighted();
	BYTE bStyle = pButton->GetButtonStyle();
	BOOL bDefault = bStyle == BS_DEFPUSHBUTTON;
	BOOL bChecked = pButton->GetChecked();
	CRect rc(pButton->GetButtonRect());

	if (pButton->GetFlatStyle() && !bHot && !bPressed && !bChecked)
		return;

	if (pButton->GetUseVisualStyle() && m_themeButton->IsAppThemed())
	{
		DrawButtonVisualStyleBackground(pDC, pButton);
		return;
	}

	pDC->FillSolidRect(rc, pButton->GetButtonBackColor());

	BOOL bSelected = bPressed || bChecked;

	if (bDefault)
	{
		pDC->Draw3dRect(rc, m_crBorder3DShadow, m_crBorder3DShadow);
		rc.DeflateRect(1,1);
	}

	pDC->Draw3dRect(rc,
		bSelected ? m_crBorder3DShadow : m_crBorder3DHilite,
		bSelected ? m_crBorder3DHilite : m_crBorder3DShadow);

	rc.DeflateRect(1, 1);

	pDC->Draw3dRect(rc,
		bSelected ? m_crBorderShadow : m_crBorderHilite,
		bSelected ? m_crBorderHilite : m_crBorderShadow);
}


COLORREF CXTPButtonTheme::GetTextColor(CXTPButton* pButton)
{
	if (!pButton->IsWindowEnabled())
	{
		return m_crTextDisabled;
	}

	return pButton->GetButtonTextColor();
}

AFX_INLINE BOOL IsAlignRight (int nAlign) { return (nAlign & BS_HORZMASK) == BS_RIGHT;}
AFX_INLINE BOOL IsAlignCenter (int nAlign) { return (nAlign & BS_HORZMASK) == BS_CENTER;}
AFX_INLINE BOOL IsAlignTop (int nAlign) { return (nAlign & BS_VERTMASK) == BS_TOP;}
AFX_INLINE BOOL IsAlignBottom (int nAlign) { return (nAlign & BS_VERTMASK) == BS_BOTTOM;}
AFX_INLINE BOOL IsAlignMiddle (int nAlign) { return (nAlign & BS_VERTMASK) == BS_VCENTER;}


void CXTPButtonTheme::DrawButtonText(CDC* pDC, CXTPButton* pButton)
{
	pDC->SetTextColor(pButton->IsWindowEnabled() ?
		pButton->GetButtonTextColor() : m_crTextDisabled);

	static const BYTE buttonsStyles[] =
	{
		CBR_PUSHBUTTON,
		CBR_PUSHBUTTON,
		CBR_CHECKTEXT,
		CBR_CHECKTEXT,
		CBR_RADIOTEXT,
		CBR_CHECKTEXT,
		CBR_CHECKTEXT,
		CBR_GROUPTEXT,
		CBR_CLIENTRECT,
		CBR_RADIOTEXT,
		CBR_CLIENTRECT,
		CBR_PUSHBUTTON,
	};

	RECT    rc;
	int     x, y, cx = 0, cy = 0;

	CString strText;
	UINT dsFlags = DT_SINGLELINE | DT_LEFT | DT_TOP | DT_NOCLIP;
	BYTE bStyle = pButton->GetButtonStyle();

	DWORD dwStyle = pButton->GetStyle();

	BOOL pbfPush = pButton->IsPushButton();
	if (pbfPush)
	{
		CalcRect(pDC, pButton, &rc, CBR_PUSHBUTTON);
	}
	else
	{
		CalcRect(pDC, pButton, &rc, buttonsStyles[bStyle]);
	}

	// Alignment
	strText = pButton->GetButtonText();
	UINT wAlignment = pButton->GetTextAlignment();

	HGDIOBJ hImage = 0;

	if (pButton->GetStyle() & BS_BITMAP)
	{
		hImage = (HGDIOBJ)pButton->SendMessage(BM_GETIMAGE, IMAGE_BITMAP);
		if (!hImage)
			return;

		BITMAP bmp;
		GetObject(hImage, sizeof(BITMAP), &bmp);
		cx = bmp.bmWidth;
		cy = bmp.bmHeight;
	}
	else if (pButton->GetStyle() & BS_ICON)
	{
		hImage = (HGDIOBJ)pButton->SendMessage(BM_GETIMAGE, IMAGE_ICON);
		if (!hImage)
			return;

		cx = cy = 16;
	}
	else
	{
		// Text button
		if (strText.IsEmpty())
			return;

		if (pButton->GetMarkupUIElement())
		{
			XTPMarkupSetDefaultFont(pButton->GetMarkupContext(), (HFONT)pButton->SendMessage(WM_GETFONT), COLORREF_NULL);

			CSize sz = XTPMarkupMeasureElement(pButton->GetMarkupUIElement(), rc.right - rc.left, rc.bottom - rc.top);

			cx = sz.cx;
			cy = sz.cy;

		}
		else
		{
			if (pButton->GetStyle() & BS_MULTILINE)
			{
				dsFlags |= DT_WORDBREAK | DT_EDITCONTROL;
				dsFlags &= ~DT_SINGLELINE;
			}

			CRect rcText(0, 0, rc.right - rc.left, 0);
			pDC->DrawText(strText, rcText, dsFlags | DT_CALCRECT);

			cx = rcText.Width();
			cy = rcText.Height();
		}
	}

	CRect rcText(rc);
	if (pbfPush && ((pButton->GetStyle() & BS_MULTILINE) == 0)) rcText.DeflateRect(m_cxBorder, m_cyBorder);

	// Horizontal
	switch (wAlignment & BS_HORZMASK)
	{
		case BS_LEFT:
			x = rc.left + (pbfPush ? m_cxBorder : 0);
			break;

		case BS_RIGHT:
			x = rc.right - cx -  (pbfPush ? m_cxBorder : 0);
			dsFlags |= DT_RIGHT;
			break;

		default:
			x = (rc.left + rc.right - cx) / 2;
			dsFlags |= DT_CENTER;
			break;
	}

	// Vertical
	switch (wAlignment & BS_VERTMASK)
	{
		case BS_TOP:
			y = rc.top + (pbfPush ? m_cyBorder : 0);
			break;

		case BS_BOTTOM:
			y = rc.bottom - cy - (pbfPush ? m_cyBorder : 0);
			dsFlags |= DT_BOTTOM;
			break;

		default:
			y = (rc.top + rc.bottom - cy) / 2;
			dsFlags |= DT_VCENTER;
			break;
	}

	if (dwStyle & BS_BITMAP)
	{
		pDC->DrawState(CPoint(x, y), CSize(cx, cy), (HBITMAP)hImage, DSS_NORMAL, 0);
	}
	else if (dwStyle & BS_ICON)
	{
		pDC->DrawState(CPoint(x, y), CSize(cx, cy), (HICON)hImage, DSS_NORMAL, (HBRUSH)0);
	}
	else if (bStyle !=  LOBYTE(BS_USERBUTTON))
	{
		if (pButton->GetMarkupUIElement())
		{
			XTPMarkupSetDefaultFont(pButton->GetMarkupContext(), NULL, pDC->GetTextColor());

			XTPMarkupRenderElement(pButton->GetMarkupUIElement(), pDC->GetSafeHdc(), CRect(x, y, x + cx, y + cy));
		}
		else
		{
			if (pButton->GetExStyle() & WS_EX_RTLREADING)
				dsFlags |= DT_RTLREADING;

#ifndef _XTP_ACTIVEX_BUTTON
			if (pButton->SendMessage(WM_QUERYUISTATE) & UISF_HIDEACCEL)
			{
				dsFlags |= DT_HIDEPREFIX;
			}
#endif

			if (dwStyle & BS_MULTILINE)
			{
				pDC->DrawText(strText, CRect(x, y, x + cx, y + cy), dsFlags | DT_NOCLIP);
			}
			else
			{
				pDC->DrawText(strText, rcText, dsFlags);
			}
		}
	}

	if (::GetFocus() == pButton->m_hWnd && pButton->GetShowFocus())
	{
#ifndef _XTP_ACTIVEX_BUTTON
		if ((pButton->SendMessage(WM_QUERYUISTATE) & UISF_HIDEFOCUS) == 0)
#endif
		{
			if (!pbfPush)
			{

				RECT rcClient = pButton->GetButtonRect();

				if (bStyle == LOBYTE(BS_USERBUTTON))
					CopyRect(&rc, &rcClient);
				else
				{
					// Try to leave a border all around text.  That causes
					// focus to hug text.
					rc.top = max(rcClient.top, y - m_cyBorder);
					rc.bottom = min(rcClient.bottom, rc.top + m_cyEdge + cy);

					rc.left = max(rcClient.left, x-m_cxBorder);
					rc.right = min(rcClient.right, rc.left + m_cxEdge + cx);
				}
			}
			else
				InflateRect(&rc, -m_cxBorder, -m_cyBorder);

			pDC->SetTextColor(GetXtremeColor(COLOR_BTNTEXT));
			pDC->SetBkColor(GetXtremeColor(COLOR_WINDOW));

			// Are back & fore colors set properly?
			::DrawFocusRect(pDC->GetSafeHdc(), &rc);
		}
	}
}

void CXTPButtonTheme::CalcRect(CDC* pDC, CXTPButton* pButton, LPRECT lprc, int code)
{
	int dy;
	UINT align;
	CSize extent;

	*lprc = pButton->GetButtonRect();

	align = pButton->GetTextAlignment();

	switch (code)
	{
	case CBR_PUSHBUTTON:
		// Subtract out raised edge all around
		InflateRect(lprc, -m_cxEdge, -m_cyEdge);
		break;

	case CBR_CHECKBOX:
	case CBR_RADIOBOX:
		{
			CSize szGlyph = pButton->GetGlyphSize(code == CBR_CHECKBOX);

			switch (align & BS_VERTMASK)
			{
			case BS_VCENTER:
				lprc->top = (lprc->top + lprc->bottom - szGlyph.cy) / 2;
				break;

			case BS_TOP:
			case BS_BOTTOM:
				{
					extent = pDC->GetTextExtent(_T(" "), 1);
					dy = extent.cy + extent.cy/4;

					// Save vertical extent
					extent.cx = dy;

					// Get centered amount

					dy = (dy - szGlyph.cy) / 2;
					if ((align & BS_VERTMASK) == BS_TOP)
						lprc->top += dy;
					else
						lprc->top = lprc->bottom - extent.cx + dy;
					break;
				}
			}
			lprc->bottom = lprc->top + szGlyph.cy;

			if (pButton->GetStyle() & BS_RIGHTBUTTON)
				lprc->left = lprc->right - szGlyph.cx;
			else
				lprc->right = lprc->left + szGlyph.cx;
		}
		break;

	case CBR_CHECKTEXT:
	case CBR_RADIOTEXT:
		{
			int nOffset = pButton->GetGlyphSize(code == CBR_CHECKBOX).cx + 3;
			if (pButton->GetStyle() & BS_RIGHTBUTTON)
			{
				lprc->right -= nOffset;
			}
			else
			{
				lprc->left += nOffset;
			}
		}
		break;

	case CBR_GROUPTEXT:
		{
			CString strText = pButton->GetButtonText();
			XTPDrawHelpers()->StripMnemonics(strText);

			if (strText.IsEmpty())
			{
				SetRectEmpty(lprc);
				break;
			}

			extent = pDC->GetTextExtent(strText);

			extent.cx += m_cxEdge * 2;

			// Left Align. TODO!
			if (pButton->GetBorderStyle() == 1)
			{
				if (pButton->GetTextAlignment() & BS_RIGHT)
				{
					lprc->left = lprc->right - (int)(extent.cx);
				}
				else
					lprc->right = lprc->left + (int)(extent.cx);
			}
			else if (pButton->GetTextAlignment() & BS_RIGHT)
			{
				lprc->right -= 6 + m_cxBorder;
				lprc->left += lprc->right - (int)(extent.cx);
			}
			else
			{
				lprc->left += 8 + m_cxBorder;
				lprc->right = lprc->left + (int)(extent.cx);
			}

			lprc->bottom = lprc->top + extent.cy;
		}
		break;

	case CBR_GROUPFRAME:
		extent = pDC->GetTextExtent(_T(" "), 1);
		lprc->top += extent.cy / 2;
		break;
	}
}

void CXTPButtonTheme::CalcRects(CDC* pDC, CXTPButton* pButton, CRect* pButtonText, UINT* pDrawFlags, CRect* pButtonIcon)
{
	CString strText = pButton->GetButtonText();

	UINT dsFlags = DT_SINGLELINE | DT_LEFT | DT_TOP;

	if (pButton->GetExStyle() & WS_EX_RTLREADING)
		dsFlags |= DT_RTLREADING;

	if (pButton->GetStyle() & BS_MULTILINE)
	{
		dsFlags |= DT_WORDBREAK | DT_EDITCONTROL;
		dsFlags &= ~DT_SINGLELINE;
	}

	CRect rcItem = pButton->GetButtonRect();
	rcItem.DeflateRect(pButton->GetBorderGap(), pButton->GetBorderGap());

	if (pButton->IsDropDownStyle() && pButton->GetPushButtonStyle() != xtpButtonDropDownNoGlyph)
	{
		rcItem.right -= 12 + (pButton->GetPushButtonStyle() == xtpButtonSplitDropDown ?
			2 : 1) * pButton->GetBorderGap() ;
	}

	int nTextImageRelation = pButton->GetTextImageRelation();
	int nTextAlign = pButton->GetTextAlignment();
	int nImageAlign = pButton->GetImageAlignment();
	CSize sizeImage = pButton->GetImageSize();

	if (sizeImage.cx == 0 || strText.IsEmpty())
		nTextImageRelation = xtpButtonOverlay;

	CSize sizeText = CSize(rcItem.Width(), 0);

	switch (nTextImageRelation)
	{
	case xtpButtonImageBeforeText:
	case xtpButtonTextBeforeImage:
		sizeImage.cx += pButton->GetImageGap();
		sizeText.cx -= sizeImage.cx;
		break;
	case xtpButtonImageAboveText:
	case xtpButtonTextAboveImage:
		sizeImage.cy += pButton->GetImageGap();
		break;
	}

	if (!strText.IsEmpty())
	{
		if (pButton->GetMarkupUIElement())
		{
			XTPMarkupSetDefaultFont(pButton->GetMarkupContext(), (HFONT)pButton->SendMessage(WM_GETFONT), (COLORREF)-1);

			CSize szDesiredSize = XTPMarkupMeasureElement(pButton->GetMarkupUIElement(), sizeText.cx, rcItem.Height());

			sizeText = CSize(min(sizeText.cx, szDesiredSize.cx), szDesiredSize.cy);
		}
		else
		{
			CRect rcTextCalc(0, 0, sizeText.cx, 0);
			pDC->DrawText(strText, rcTextCalc, dsFlags | DT_CALCRECT);
			sizeText = CSize(min(sizeText.cx, rcTextCalc.Width()), rcTextCalc.Height());
		}
	}
	else
	{
		sizeText = CSize(0, 0);
	}

	if (nTextImageRelation == xtpButtonOverlay && pButtonText) sizeImage = CSize(0, 0);
	if (nTextImageRelation == xtpButtonOverlay && pButtonIcon) sizeText = CSize(0, 0);

	CRect rcText(rcItem);
	CPoint ptImage(rcItem.TopLeft());

	ptImage.x = IsAlignRight(nImageAlign) ? rcItem.right - sizeImage.cx :
		IsAlignCenter(nImageAlign) ? (rcItem.left + rcItem.right - sizeImage.cx) /2 : rcItem.left;

	ptImage.y = IsAlignBottom(nImageAlign) ? rcItem.bottom - sizeImage.cy :
		IsAlignMiddle(nImageAlign) ? (rcItem.top + rcItem.bottom - sizeImage.cy) / 2 : rcItem.top;

	rcText.left = IsAlignRight(nTextAlign) ? rcText.right - sizeText.cx :
		IsAlignCenter(nTextAlign) ? (rcText.left + rcText.right - sizeText.cx) / 2 : rcItem.left;

	dsFlags |= IsAlignRight(nTextAlign) ? DT_RIGHT: IsAlignCenter(nTextAlign) ? DT_CENTER : DT_LEFT;

	rcText.top = IsAlignBottom(nTextAlign) ? rcText.bottom - sizeText.cy :
		IsAlignMiddle(nTextAlign) ? (rcText.top + rcText.bottom - sizeText.cy) / 2 : rcItem.top;


	switch (nTextImageRelation)
	{
	case xtpButtonImageBeforeText:
		ptImage.x = IsAlignRight(nImageAlign) ?
						rcItem.right - (sizeImage.cx + sizeText.cx):
					IsAlignCenter(nImageAlign) ?
						(rcItem.left + rcItem.right - (sizeImage.cx + sizeText.cx)) /2 :
						rcItem.left;

		rcText.left = max(rcText.left, ptImage.x + sizeImage.cx);
		break;

	case xtpButtonImageAboveText:
		ptImage.y = IsAlignBottom(nImageAlign) ?
						rcItem.bottom - (sizeImage.cy + sizeText.cy):
					IsAlignMiddle(nImageAlign) ?
						(rcItem.top + rcItem.bottom - (sizeImage.cy + sizeText.cy)) /2 :
						rcItem.top;

		rcText.top = max(rcText.top, ptImage.y + sizeImage.cy);
		break;

	case xtpButtonTextBeforeImage:
		ptImage.x = pButton->GetImageGap() +
			(IsAlignRight(nImageAlign) ?
				rcItem.right - sizeImage.cx :
			IsAlignCenter(nImageAlign) ?
				(rcItem.left + rcItem.right + (-sizeImage.cx + sizeText.cx)) /2 :
				rcItem.left + sizeText.cx);

		if (rcText.left + sizeText.cx + pButton->GetImageGap() > ptImage.x)
			rcText.left = ptImage.x - sizeText.cx - pButton->GetImageGap();
		break;

	case xtpButtonTextAboveImage:
		ptImage.y = pButton->GetImageGap() +
			(IsAlignBottom(nImageAlign) ?
				rcItem.bottom - sizeImage.cy :
			IsAlignMiddle(nImageAlign) ?
				(rcItem.top + rcItem.bottom + (-sizeImage.cy + sizeText.cy)) /2 :
				rcItem.top + sizeText.cy);

		if (rcText.top + sizeText.cy + pButton->GetImageGap() > ptImage.y)
			rcText.top = ptImage.y - sizeText.cy - pButton->GetImageGap();
		break;
	}


	rcText.right = rcText.left + sizeText.cx;
	rcText.bottom = rcText.top + sizeText.cy;

	if (pButtonText) *pButtonText = rcText;
	if (pButtonIcon) *pButtonIcon = CRect(ptImage, sizeImage);
	if (pDrawFlags) *pDrawFlags = dsFlags;
}

BOOL CXTPButtonTheme::IsVisualThemeUsed(CXTPButton* pButton)
{
	return (m_themeButton->IsAppThemed() && pButton->GetUseVisualStyle());
}

void CXTPButtonTheme::DrawPushButtonDropDown(CDC* pDC, CXTPButton* pButton)
{
	CRect rcItem = pButton->GetButtonRect();
	rcItem.DeflateRect(pButton->GetBorderGap(), pButton->GetBorderGap());

	CString strText = pButton->GetButtonText();

	BOOL bEmpty = strText.IsEmpty() && pButton->GetImageSize() == CSize(0, 0);

	if (!bEmpty)
		rcItem.left = rcItem.right - 16;

	BOOL  bSelected = pButton->GetChecked() || pButton->IsPushed();
	if (bSelected && (m_bOffsetHiliteText && !IsVisualThemeUsed(pButton)))
		rcItem.OffsetRect(1, 1);

	CPoint ptCenter = rcItem.CenterPoint();
	BOOL bEnabled = pButton->IsWindowEnabled();

	if (pButton->GetPushButtonStyle() == xtpButtonDropDown)
	{
		XTPDrawHelpers()->Triangle(pDC, CPoint(ptCenter.x - 4, ptCenter.y - 2),
			CPoint(ptCenter.x + 4, ptCenter.y - 2),CPoint(ptCenter.x, ptCenter.y + 2), !bEnabled ? GetXtremeColor(COLOR_GRAYTEXT) : GetXtremeColor(COLOR_BTNTEXT));
	}
	if (pButton->GetPushButtonStyle() == xtpButtonDropDownRight)
	{
		XTPDrawHelpers()->Triangle(pDC, CPoint(ptCenter.x - 2, ptCenter.y - 4),
			CPoint(ptCenter.x - 2, ptCenter.y + 4),CPoint(ptCenter.x + 2, ptCenter.y ), !bEnabled ? GetXtremeColor(COLOR_GRAYTEXT) : GetXtremeColor(COLOR_BTNTEXT));
	}

	if (pButton->GetPushButtonStyle() == xtpButtonSplitDropDown)
	{
		ptCenter.x += 1;
		XTPDrawHelpers()->Triangle(pDC, CPoint(ptCenter.x - 4, ptCenter.y - 2),
			CPoint(ptCenter.x + 4, ptCenter.y - 2),CPoint(ptCenter.x, ptCenter.y + 2), !bEnabled ? GetXtremeColor(COLOR_GRAYTEXT) : GetXtremeColor(COLOR_BTNTEXT));

		COLORREF crBorderShadow = !bEnabled ? GetXtremeColor(COLOR_GRAYTEXT) : IsVisualThemeUsed(pButton) ?  GetSysColor(COLOR_BTNSHADOW) : m_crBorderShadow;

		pDC->FillSolidRect(rcItem.left, rcItem.top, 1, rcItem.Height(), crBorderShadow);
		pDC->FillSolidRect(rcItem.left + 1, rcItem.top, 1, rcItem.Height(), GetSysColor(COLOR_BTNHIGHLIGHT));
	}
}


void CXTPButtonTheme::DrawPushButtonText(CDC* pDC, CXTPButton* pButton)
{
	CString strText = pButton->GetButtonText();

	// if the string is empty just return.
	if (strText.IsEmpty())
		return;

	CRect rcText;
	UINT dsFlags;
	CalcRects(pDC, pButton, &rcText, &dsFlags, NULL);

	BOOL  bSelected = pButton->GetChecked() || pButton->IsPushed();

	if (bSelected && (m_bOffsetHiliteText && !IsVisualThemeUsed(pButton)))
		rcText.OffsetRect(1, 1);

	COLORREF clrText = GetTextColor(pButton);
	pDC->SetTextColor(clrText);

	if (pButton->GetMarkupUIElement())
	{
		XTPMarkupSetDefaultFont(pButton->GetMarkupContext(), NULL, clrText);

		XTPMarkupRenderElement(pButton->GetMarkupUIElement(), pDC->GetSafeHdc(), rcText);

	}
	else
	{
#ifndef _XTP_ACTIVEX_BUTTON
		if (pButton->SendMessage(WM_QUERYUISTATE) & UISF_HIDEACCEL)
		{
			dsFlags |= DT_HIDEPREFIX;
		}
#endif
		pDC->DrawText(strText, rcText, dsFlags);
	}
}

void CXTPButtonTheme::DrawPushButtonIcon(CDC* pDC, CXTPButton* pButton)
{
	CSize sz = pButton->GetImageSize();
	if (sz == CSize(0, 0))
		return;

	CRect rcImage;
	CalcRects(pDC, pButton, NULL, NULL, &rcImage);

	CPoint pt = rcImage.TopLeft();

	BOOL  bSelected = pButton->GetChecked() || pButton->IsPushed();

	if (bSelected && (m_bOffsetHiliteText && !IsVisualThemeUsed(pButton)))
		pt.Offset(1, 1);

	if (m_bToolbarStyle)
	{
		CXTPImageManagerIcon* pIconManager = pButton->GetIcon();
		if (pIconManager && pIconManager->GetIcon())
		{
			if (!pButton->IsWindowEnabled())
			{
				pDC->DrawState(pt, pIconManager->GetExtent(),
					pIconManager->GetIcon(), DSS_NORMAL | DSS_DISABLED, (HBRUSH)0);
			}
			else if (pButton->IsHighlighted() || bSelected)
			{
				pDC->DrawState(pt, pIconManager->GetExtent(),
					pButton->IsPushed() ? pIconManager->GetPressedIcon() : pIconManager->GetHotIcon(), DSS_NORMAL, (HBRUSH)0);
			}
			else
			{
				pDC->DrawState(pt, 0,
					pIconManager->GetIcon(), DSS_NORMAL, (HBRUSH)0);
			}
		}
		else
		{
			pButton->DrawImage(pDC, CRect(pt, sz));
		}
	}
	else
	{
		pButton->DrawImage(pDC, CRect(pt, sz));
	}
}

void CXTPButtonTheme::DrawCheckBoxMark(CDC* pDC, CXTPButton* pButton)
{
	BOOL bPressed = pButton->IsPushed();
	BOOL bHot = pButton->IsHighlighted();
	BYTE bStyle = pButton->GetButtonStyle();
	BOOL bEnabled = pButton->IsWindowEnabled();
	BOOL bChecked = pButton->GetChecked();

	CRect rcRadio;
	CalcRect(pDC, pButton, &rcRadio, CBR_CHECKBOX);

	if (IsVisualThemeUsed(pButton))
	{
		int nState = !bEnabled? RBS_UNCHECKEDDISABLED: bPressed? RBS_UNCHECKEDPRESSED: bHot? RBS_UNCHECKEDHOT: RBS_UNCHECKEDNORMAL;
		if (bChecked)
			nState += 4;
		if ((bChecked == BST_INDETERMINATE) && (bStyle == BS_AUTO3STATE || bStyle == BS_3STATE))
			nState += 4;

		m_themeButton->DrawThemeBackground(pDC->GetSafeHdc(), BP_CHECKBOX, nState, rcRadio, NULL);
		return;
	}

	int nButton = ((bChecked == BST_INDETERMINATE) && (bStyle == BS_AUTO3STATE || bStyle == BS_3STATE))
		? DFCS_BUTTON3STATE : DFCS_BUTTONCHECK;

	DrawFrameControl(pDC->GetSafeHdc(), rcRadio, DFC_BUTTON, nButton | (m_bFlatGlyphs ? DFCS_FLAT : 0) |
		(bChecked ? DFCS_CHECKED : 0) | (bPressed ? DFCS_PUSHED : 0) | (bEnabled ? 0 : DFCS_INACTIVE));
}

void CXTPButtonTheme::DrawRadioButtonMark(CDC* pDC, CXTPButton* pButton)
{
	BOOL bPressed = pButton->IsPushed();
	BOOL bHot = pButton->IsHighlighted();
	BOOL bEnabled = pButton->IsWindowEnabled();
	BOOL bChecked = pButton->GetChecked();

	CRect rcRadio;
	CalcRect(pDC, pButton, &rcRadio, CBR_RADIOBOX);

	if (IsVisualThemeUsed(pButton))
	{
		int nState = !bEnabled? RBS_UNCHECKEDDISABLED: bPressed? RBS_UNCHECKEDPRESSED: bHot? RBS_UNCHECKEDHOT: RBS_UNCHECKEDNORMAL;
		if (bChecked) nState += 4;

		m_themeButton->DrawThemeBackground(pDC->GetSafeHdc(), BP_RADIOBUTTON, nState, rcRadio, NULL);
		return;
	}

	DrawFrameControl(pDC->GetSafeHdc(), rcRadio, DFC_BUTTON, DFCS_BUTTONRADIO | (m_bFlatGlyphs ? DFCS_FLAT : 0) |
		(bChecked ? DFCS_CHECKED : 0) | (bPressed ? DFCS_PUSHED : 0) | (bEnabled ? 0 : DFCS_INACTIVE));

}

void CXTPButtonTheme::DrawGroupBox(CDC* pDC, CXTPButton* pButton)
{
	BOOL bEnabled = pButton->IsWindowEnabled();

	int nState = !bEnabled? GBS_DISABLED: GBS_NORMAL;
	CRect rcGroupBox;
	CalcRect(pDC, pButton, &rcGroupBox, CBR_GROUPFRAME);

	CRect rcGrouText;
	CalcRect(pDC, pButton, &rcGrouText, CBR_GROUPTEXT);

	if (pButton->GetTextAlignment() & BS_RIGHT)
		rcGrouText.OffsetRect(+m_cxEdge, 0);
	else
		rcGrouText.OffsetRect(-m_cxEdge, 0);

	pDC->ExcludeClipRect(&rcGrouText);

	if (pButton->GetBorderStyle() == 1)
	{
		if (IsVisualThemeUsed(pButton))
		{
			COLORREF clrLine = GetSysColor(COLOR_3DSHADOW);
			m_themeButton->GetThemeColor(BP_GROUPBOX, GBS_NORMAL, TMT_BORDERCOLORHINT, &clrLine);
			pDC->FillSolidRect(rcGroupBox.left, rcGroupBox.top, rcGroupBox.Width(), 1, clrLine);
		}
		else
		{
			DrawEdge(pDC->GetSafeHdc(), &rcGroupBox, EDGE_ETCHED, BF_TOP | (m_bFlatGlyphs ? BF_FLAT | BF_MONO : 0) );
		}

	}
	else
	{
		if (IsVisualThemeUsed(pButton))
		{
			m_themeButton->DrawThemeBackground(pDC->GetSafeHdc(), BP_GROUPBOX, nState, rcGroupBox, NULL);
		}
		else
		{
			DrawEdge(pDC->GetSafeHdc(), &rcGroupBox, EDGE_ETCHED, BF_RECT |
				(m_bFlatGlyphs ? BF_FLAT | BF_MONO : 0));
		}
	}

	pDC->SelectClipRgn(NULL);
}




//////////////////////////////////////////////////////////////////////////
// CXTPButtonFlatTheme

CXTPButtonFlatTheme::CXTPButtonFlatTheme()
{
	m_nTheme = xtpControlThemeFlat;
	m_bFlatGlyphs = TRUE;
	m_nBorderWidth = 4;
}

void CXTPButtonFlatTheme::DrawButtonBackground(CDC* pDC, CXTPButton* pButton)
{
	BOOL bPressed = pButton->IsPushed();
	BOOL bHot = pButton->IsHighlighted();
	BYTE bStyle = pButton->GetButtonStyle();
	BOOL bDefault = bStyle == BS_DEFPUSHBUTTON;
	BOOL bChecked = pButton->GetChecked();
	CRect rc(pButton->GetButtonRect());

	if (pButton->GetFlatStyle() && !bHot && !bPressed && !bChecked)
		return;

	if (pButton->GetUseVisualStyle() && m_themeButton->IsAppThemed())
	{
		DrawButtonVisualStyleBackground(pDC, pButton);
		return;
	}

	pDC->FillSolidRect(rc, pButton->GetButtonBackColor());

	COLORREF clrBorder = GetXtremeColor(COLOR_WINDOWFRAME);

	if (bDefault)
	{
		pDC->Draw3dRect(rc, clrBorder, clrBorder);
		rc.DeflateRect(1,1);
	}

	pDC->Draw3dRect(rc, clrBorder, clrBorder);

	rc.DeflateRect(1, 1);
	pDC->Draw3dRect(rc, GetXtremeColor(COLOR_WINDOW), GetXtremeColor(COLOR_WINDOW));

}


//////////////////////////////////////////////////////////////////////////
//

CXTPButtonUltraFlatTheme::CXTPButtonUltraFlatTheme()
{
	m_nTheme = xtpControlThemeUltraFlat;
	m_bFlatGlyphs = TRUE;
	m_nBorderWidth = 3;
	m_bHiglightButtons = TRUE;

}

void CXTPButtonUltraFlatTheme::RefreshMetrics(CXTPButton* pButton)
{
	CXTPButtonTheme::RefreshMetrics(pButton);

	COLORREF clrWindow = GetXtremeColor(COLOR_WINDOW);
	COLORREF clrToolBar = GetXtremeColor(XPCOLOR_TOOLBAR_FACE);
	COLORREF clrFace = pButton->GetButtonBackColor();

	m_crBack.SetStandardValue(clrFace);

	COLORREF clrPushed = GetXtremeColor(COLOR_3DSHADOW);
	clrPushed = XTPColorManager()->LightColor(clrPushed,
			clrFace, 0x32);
	m_crBackPushed.SetStandardValue(clrPushed);

	COLORREF clrHighlight = clrFace;
	clrHighlight = XTPColorManager()->LightColor(clrHighlight, clrWindow, 0x1E);
	m_crBackHilite.SetStandardValue(clrHighlight);

	COLORREF clrChecked =
		RGB(GetRValue(clrWindow) * 40 / 100 + GetRValue(clrHighlight) * 10 / 100 + GetRValue(clrToolBar) * 50 / 100,
		GetGValue(clrWindow) * 40 / 100 + GetGValue(clrHighlight) * 10 / 100 + GetGValue(clrToolBar) * 50 / 100,
		GetBValue(clrWindow) * 40 / 100 + GetBValue(clrHighlight) * 10 / 100 + GetBValue(clrToolBar) * 50 / 100);


	m_crBackChecked.SetStandardValue(clrChecked);

	m_crBorderShadow.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));
	m_crBorderHilite.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));

	COLORREF clrText = pButton->GetButtonTextColor();
	m_crText.SetStandardValue(clrText);
	m_crTextHilite.SetStandardValue(clrText);
	m_crTextPushed.SetStandardValue(clrText);
}

void CXTPButtonUltraFlatTheme::DrawButtonBackground(CDC* pDC, CXTPButton* pButton)
{
	BOOL bPressed = pButton->IsPushed();
	BOOL bHot = pButton->IsHighlighted();
	BYTE bStyle = pButton->GetButtonStyle();
	BOOL bDefault = bStyle == BS_DEFPUSHBUTTON;
	BOOL bChecked = pButton->GetChecked();
	CRect rc(pButton->GetButtonRect());

	if (pButton->GetFlatStyle() && !bHot && !bPressed && !bChecked)
		return;

	if (pButton->GetUseVisualStyle() && m_themeButton->IsAppThemed())
	{
		DrawButtonVisualStyleBackground(pDC, pButton);
		return;
	}

	if (bDefault)
	{
		pDC->Draw3dRect(rc, m_crBorderShadow, m_crBorderShadow);
		rc.DeflateRect(1,1);
	}

	if (bHot || bPressed || bChecked)
	{
		pDC->FillSolidRect(rc, bPressed ? m_crBackPushed :
			bChecked ? (bHot ? m_crBackPushed : m_crBackChecked) : m_crBackHilite);

		pDC->Draw3dRect(rc, m_crBorderHilite,
			m_crBorderHilite);
	}
	else
	{
		pDC->FillSolidRect(rc, pButton->GetButtonBackColor());

		pDC->Draw3dRect(rc, m_crBorderShadow, m_crBorderShadow);
	}
}

void CXTPButtonUltraFlatTheme::DrawGroupBox(CDC* pDC, CXTPButton* pButton)
{
	if (IsVisualThemeUsed(pButton))
	{
		CXTPButtonTheme::DrawGroupBox(pDC, pButton);
		return;
	}

	CRect rcGroupBox;
	CalcRect(pDC, pButton, &rcGroupBox, CBR_GROUPFRAME);

	CRect rcGrouText;
	CalcRect(pDC, pButton, &rcGrouText, CBR_GROUPTEXT);

	if (pButton->GetTextAlignment() & BS_RIGHT)
		rcGrouText.OffsetRect(+m_cxEdge, 0);
	else
		rcGrouText.OffsetRect(-m_cxEdge, 0);

	pDC->ExcludeClipRect(&rcGrouText);

	if (pButton->GetBorderStyle() == 1)
	{
		pDC->FillSolidRect(rcGroupBox.left, rcGroupBox.top, rcGroupBox.Width(), 1, m_crBorderShadow);
	}
	else
	{
		pDC->Draw3dRect(&rcGroupBox, m_crBorderShadow, m_crBorderShadow);
	}

	pDC->SelectClipRgn(NULL);
}


void CXTPButtonUltraFlatTheme::DrawCheckBoxMark(CDC* pDC, CXTPButton* pButton)
{
	if (IsVisualThemeUsed(pButton))
	{
		CXTPButtonTheme::DrawCheckBoxMark(pDC, pButton);
		return;
	}

	BOOL bPressed = pButton->IsPushed();
	BOOL bSelected = pButton->IsHighlighted();
	BOOL bEnabled = pButton->IsWindowEnabled();
	BOOL bChecked = pButton->GetChecked();


	CRect rc;
	CalcRect(pDC, pButton, &rc, CBR_CHECKBOX);

	if (!bEnabled)
	{
		pDC->FillSolidRect(rc, GetXtremeColor(COLOR_3DFACE));
		pDC->Draw3dRect(rc, GetXtremeColor(XPCOLOR_GRAYTEXT), GetXtremeColor(XPCOLOR_GRAYTEXT));
	}
	else
	{
		if (!m_bHiglightButtons)
		{
			pDC->FillSolidRect(rc, bSelected && bPressed ?
				GetXtremeColor(COLOR_3DFACE) : GetXtremeColor(COLOR_WINDOW));

		}
		else
		{
			pDC->FillSolidRect(rc, bSelected && bPressed ?
				m_crBackPushed : bPressed || bSelected ? m_crBackHilite : GetXtremeColor(COLOR_WINDOW));
		}
		COLORREF clrBorder = bSelected || bPressed ? m_crBorderHilite : m_crBorderShadow;

		pDC->Draw3dRect(rc, clrBorder, clrBorder);
	}

	if (bChecked)
	{
		CPoint pt = rc.CenterPoint();
		CXTPPenDC pen(pDC->GetSafeHdc(), GetXtremeColor(!bEnabled || bChecked == 2 ? XPCOLOR_GRAYTEXT : COLOR_BTNTEXT));

		if (CXTPDrawHelpers::IsContextRTL(pDC))  // Revert Check Box
		{
			for (int i = 0; i < 3; i++)
			{
				pDC->MoveTo(pt.x + 3, pt.y - 1 + i);
				pDC->LineTo(pt.x + 1, pt.y + 1 + i);
				pDC->LineTo(pt.x - 4, pt.y - 4 + i);
			}
		}
		else
		{
			for (int i = 0; i < 3; i++)
			{
				pDC->MoveTo(pt.x - 3, pt.y - 1 + i);
				pDC->LineTo(pt.x - 1, pt.y + 1 + i);
				pDC->LineTo(pt.x + 4, pt.y - 4 + i);
			}
		}
	}
}

AFX_INLINE COLORREF Mix(CDC* pDC, int x, int y, COLORREF clrBorder, COLORREF clrFace, double a)
{
	COLORREF clr = clrFace;

	if (a < 0)
		a = -a;
	else
	{
		clr = pDC->GetPixel(x, y);
	}
	int r = int(GetRValue(clrBorder) + a * double(GetRValue(clr) -  GetRValue(clrBorder)));
	int g = int(GetGValue(clrBorder) + a * double(GetGValue(clr) -  GetGValue(clrBorder)));
	int b = int(GetBValue(clrBorder) + a * double(GetBValue(clr) -  GetBValue(clrBorder)));

	return RGB(r, g, b);
}

void CXTPButtonTheme::AlphaEllipse(CDC* pDC, CRect rc, COLORREF clrBorder, COLORREF clrFace)
{
	ASSERT(rc.Width() == rc.Height());
	double x0 = double(rc.left + rc.right - 1) / 2;
	double y0 = double(rc.top + rc.bottom - 1) / 2;
	double radius = double(rc.right - rc.left - 1) / 2 - 0.25;

	for (int x = rc.left; x < rc.right; x++)
	for (int y = rc.top; y < rc.bottom; y++)
	{
		double distance = sqrt(pow(x0 - x, 2) + pow(y0 - y, 2));

		if (distance >= radius - 1 && distance <= radius + 1)
		{
			pDC->SetPixel(x, y, Mix(pDC, x, y, clrBorder, clrFace, distance - radius));
		}
		else if (distance < radius - 1)
		{
			pDC->SetPixel(x, y, clrFace);
		}
	}
}


void CXTPButtonUltraFlatTheme::DrawRadioButtonMark(CDC* pDC, CXTPButton* pButton)
{
	if (IsVisualThemeUsed(pButton))
	{
		CXTPButtonTheme::DrawRadioButtonMark(pDC, pButton);
		return;
	}

	BOOL bPressed = pButton->IsPushed();
	BOOL bSelected = pButton->IsHighlighted();
	BOOL bEnabled = pButton->IsWindowEnabled();
	BOOL bChecked = pButton->GetChecked();

	CRect rc;
	CalcRect(pDC, pButton, &rc, CBR_RADIOBOX);

	if (!bEnabled)
	{
		AlphaEllipse(pDC, rc, GetXtremeColor(XPCOLOR_GRAYTEXT), GetXtremeColor(XPCOLOR_3DFACE));
	}
	else
	{
		if (!m_bHiglightButtons)
		{
			AlphaEllipse(pDC, rc, m_crBorderShadow, bSelected && bPressed ? GetXtremeColor(COLOR_3DFACE) :
				GetXtremeColor(COLOR_WINDOW));
		}
		else
		{
			AlphaEllipse(pDC, rc, bSelected || bPressed ? m_crBorderHilite : m_crBorderShadow,
				bSelected && bPressed ? m_crBackPushed :
					bPressed || bSelected ? m_crBackHilite : GetXtremeColor(COLOR_WINDOW));
		}
	}

	if (bChecked)
	{
		COLORREF clrFace = GetXtremeColor(!bEnabled || bChecked == 2 ? XPCOLOR_GRAYTEXT : COLOR_BTNTEXT);
		AlphaEllipse(pDC, CRect(rc.left + 4, rc.top + 4, rc.left + 9, rc.top + 9), clrFace, clrFace);
	}

}

//////////////////////////////////////////////////////////////////////////
//

CXTPButtonOffice2000Theme::CXTPButtonOffice2000Theme()
{
	m_nTheme = xtpControlThemeOffice2000;
	m_nBorderWidth = 2;
	m_bHiglightButtons = FALSE;
}

void CXTPButtonOffice2000Theme::DrawButtonBackground(CDC* pDC, CXTPButton* pButton)
{
	BOOL bPressed = pButton->IsPushed();
	BOOL bHot = pButton->IsHighlighted();
	BOOL bChecked = pButton->GetChecked();
	CRect rc(pButton->GetButtonRect());

	if (pButton->GetFlatStyle() && !bHot && !bPressed && !bChecked)
		return;

	if (pButton->GetUseVisualStyle() && m_themeButton->IsAppThemed())
	{
		DrawButtonVisualStyleBackground(pDC, pButton);
		return;
	}

	pDC->FillSolidRect(rc, pButton->GetButtonBackColor());

	BOOL bSelected = bPressed || bChecked;

	pDC->Draw3dRect(rc,
		bSelected ? m_crBorderShadow : m_crBorder3DHilite,
		bSelected ? m_crBorder3DHilite : m_crBorderShadow);

}

//////////////////////////////////////////////////////////////////////////
// CXTPButtonResourceTheme

CXTPButtonResourceTheme::CXTPButtonResourceTheme()
{
	m_nTheme = xtpControlThemeResource;
	m_nBorderWidth = 2;
	m_bHiglightButtons = TRUE;
}


void CXTPButtonResourceTheme::RefreshMetrics(CXTPButton* pButton)
{
	CXTPButtonUltraFlatTheme::RefreshMetrics(pButton);

	XTPResourceImages()->AssertValid();

	m_crBorderShadow.SetStandardValue(XTPResourceImages()->GetImageColor(_T("Controls"), _T("GroupBoxFrame")));
}

void CXTPButtonResourceTheme::DrawCheckBoxMark(CDC* pDC, CXTPButton* pButton)
{
	if (IsVisualThemeUsed(pButton))
	{
		CXTPButtonTheme::DrawCheckBoxMark(pDC, pButton);
		return;
	}

	CXTPResourceImage* pImage = XTPResourceImages()->LoadFile(_T("TOOLBARBUTTONCHECKBOX"));

	ASSERT(pImage);
	if (!pImage)
	{
		CXTPButtonTheme::DrawCheckBoxMark(pDC, pButton);
		return;
	}

	BOOL bPressed = pButton->IsPushed();
	BOOL bSelected = pButton->IsHighlighted();
	BOOL bEnabled = pButton->IsWindowEnabled();
	BOOL bChecked = pButton->GetChecked();


	CRect rc;
	CalcRect(pDC, pButton, &rc, CBR_CHECKBOX);


	int nState = 0;
	if (!bEnabled)
		nState = 3;
	else if (bSelected && bPressed)
		nState = 2;
	else if (bSelected)
		nState = 1;

	if (bChecked == 1)
		nState += 4;

	if (bChecked == 2)
		nState += 8;

	pImage->DrawImage(pDC, rc, pImage->GetSource(nState, 12), CRect(0, 0, 0, 0));
}

void CXTPButtonResourceTheme::DrawRadioButtonMark(CDC* pDC, CXTPButton* pButton)
{
	if (IsVisualThemeUsed(pButton))
	{
		CXTPButtonTheme::DrawRadioButtonMark(pDC, pButton);
		return;
	}

	CXTPResourceImage* pImage = XTPResourceImages()->LoadFile(_T("TOOLBARBUTTONRADIOBUTTON"));

	ASSERT(pImage);
	if (!pImage)
	{
		CXTPButtonTheme::DrawRadioButtonMark(pDC, pButton);
		return;
	}

	CRect rc;
	CalcRect(pDC, pButton, &rc, CBR_RADIOBOX);


	BOOL bPressed = pButton->IsPushed();
	BOOL bSelected = pButton->IsHighlighted();
	BOOL bEnabled = pButton->IsWindowEnabled();
	BOOL bChecked = pButton->GetChecked();

	int nState = 0;
	if (!bEnabled)
		nState = 3;
	else if (bSelected && bPressed)
		nState = 2;
	else if (bSelected)
		nState = 1;

	if (bChecked)
		nState += 4;

	pImage->DrawImage(pDC, rc, pImage->GetSource(nState, 8), CRect(0, 0, 0, 0));
}

void CXTPButtonResourceTheme::DrawButtonBackground(CDC* pDC, CXTPButton* pButton)
{
	BOOL bPressed = pButton->IsPushed();
	BOOL bSelected = pButton->IsHighlighted();
	BYTE bStyle = pButton->GetButtonStyle();
	BOOL bDefault = bStyle == BS_DEFPUSHBUTTON;
	BOOL bEnabled = pButton->IsWindowEnabled();
	BOOL bChecked = pButton->GetChecked();
	CRect rc(pButton->GetButtonRect());

	if (pButton->GetFlatStyle() && !bSelected && !bPressed && !bChecked)
		return;

	if (pButton->GetUseVisualStyle() && m_themeButton->IsAppThemed())
	{
		DrawButtonVisualStyleBackground(pDC, pButton);
		return;
	}

	CXTPResourceImage* pImage = XTPResourceImages()->LoadFile(_T("PUSHBUTTON"));

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
}

//////////////////////////////////////////////////////////////////////////
// CXTPButtonOfficeXPTheme

CXTPButtonOfficeXPTheme::CXTPButtonOfficeXPTheme()
{
	m_nTheme = xtpControlThemeOfficeXP;
	m_nBorderWidth = 2;
	m_bOffsetHiliteText = FALSE;
}

void CXTPButtonOfficeXPTheme::RefreshMetrics(CXTPButton* pButton)
{
	CXTPButtonUltraFlatTheme::RefreshMetrics(pButton);

	m_crBack.SetStandardValue(GetXtremeColor(XPCOLOR_TOOLBAR_FACE));
	m_crBackPushed.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_PUSHED));
	m_crBackHilite.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT));
	m_crBorderShadow.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));
	m_crBorderHilite.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER));
	m_crText.SetStandardValue(GetXtremeColor(XPCOLOR_TOOLBAR_TEXT));
	m_crTextHilite.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_TEXT));
	m_crTextPushed.SetStandardValue(GetXtremeColor(XPCOLOR_PUSHED_TEXT));
	m_crBackChecked.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_CHECKED));
}

void CXTPButtonOfficeXPTheme::DrawButtonBackground(CDC* pDC, CXTPButton* pButton)
{
	BOOL bPressed = pButton->IsPushed();
	BOOL bHot = pButton->IsHighlighted();
	BOOL bChecked = pButton->GetChecked();
	CRect rc(pButton->GetButtonRect());

	if (pButton->GetFlatStyle() && !bHot && !bPressed && !bChecked)
		return;

	if (pButton->GetUseVisualStyle() && m_themeButton->IsAppThemed())
	{
		DrawButtonVisualStyleBackground(pDC, pButton);
		return;
	}

	if (bHot || bPressed || bChecked)
	{
		pDC->FillSolidRect(rc, bPressed ? m_crBackPushed :
			bChecked ? (bHot ? m_crBackPushed : m_crBackChecked) : m_crBackHilite);

		pDC->Draw3dRect(rc, m_crBorderHilite,
			m_crBorderHilite);
	}
	else
	{
		pDC->FillSolidRect(rc, m_crBack);

		pDC->Draw3dRect(rc, m_crBorderShadow, m_crBorderShadow);
	}
}



//////////////////////////////////////////////////////////////////////////
//

//////////////////////////////////////////////////////////////////////////
// CXTPButtonOffice2003Theme

CXTPButtonOffice2003Theme::CXTPButtonOffice2003Theme()
{
	m_nTheme = xtpControlThemeOffice2003;
	m_bLunaTheme = FALSE;
}


void CXTPButtonOffice2003Theme::RefreshMetrics(CXTPButton* pButton)
{
	CXTPButtonOfficeXPTheme::RefreshMetrics(pButton);

	XTPCurrentSystemTheme systemTheme = XTPColorManager()->GetCurrentSystemTheme();

	if (systemTheme != xtpSystemThemeUnknown)
	{
		m_crBackHilite.SetStandardValue(RGB(255, 238, 194));
		m_crBackPushed.SetStandardValue(RGB(254, 128, 62));
		m_crBackChecked.SetStandardValue(RGB(255, 192, 111));
		m_crText.SetStandardValue(0);
		m_crTextHilite.SetStandardValue(0);
		m_crTextPushed.SetStandardValue(0);
	}

	switch (systemTheme)
	{
	case xtpSystemThemeBlue:
	case xtpSystemThemeRoyale:
	case xtpSystemThemeAero:
		m_crBack.SetStandardValue(RGB(169, 199, 240));
		m_crBorderShadow.SetStandardValue(RGB(127, 157, 185));
		m_crBorderHilite.SetStandardValue(RGB(0, 0, 128));
		break;

	case xtpSystemThemeOlive:
		m_crBack.SetStandardValue(RGB(197, 212, 159));
		m_crBorderShadow.SetStandardValue(RGB(164, 185, 127));
		m_crBorderHilite.SetStandardValue(RGB(63, 93, 56));
		break;

	case xtpSystemThemeSilver:
		m_crBack.SetStandardValue(RGB(192, 192, 211));
		m_crBorderShadow.SetStandardValue(RGB(165, 172, 178));
		m_crBorderHilite.SetStandardValue(RGB(75, 75, 11));
		break;
	}

	m_bLunaTheme = systemTheme != xtpSystemThemeUnknown;
}

void CXTPButtonOffice2003Theme::DrawButtonBackground(CDC* pDC, CXTPButton* pButton)
{
	if (!m_bLunaTheme)
	{
		CXTPButtonOfficeXPTheme::DrawButtonBackground(pDC, pButton);
		return;
	}
	BOOL bPressed = pButton->IsPushed();
	BOOL bHot = pButton->IsHighlighted();
	BOOL bChecked = pButton->GetChecked();
	CRect rc(pButton->GetButtonRect());

	if (pButton->GetFlatStyle() && !bHot && !bPressed && !bChecked)
		return;

	if (pButton->GetUseVisualStyle() && m_themeButton->IsAppThemed())
	{
		DrawButtonVisualStyleBackground(pDC, pButton);
		return;
	}

	if (bHot || bPressed || bChecked)
	{
		if (bPressed)
		{
			XTPDrawHelpers()->GradientFill(pDC, &rc,
				XTPColorManager()->grcLunaPushed, FALSE);
		}
		else if (bHot)
		{
			XTPDrawHelpers()->GradientFill(pDC, &rc,
				XTPColorManager()->grcLunaSelected, FALSE);
		}
		else if (bChecked)
		{
			XTPDrawHelpers()->GradientFill(pDC, &rc,
				XTPColorManager()->grcLunaChecked, FALSE);
		}

		pDC->Draw3dRect(rc, m_crBorderHilite,
			m_crBorderHilite);

	}
	else
	{
		pDC->FillSolidRect(rc, m_crBack);
		pDC->Draw3dRect(rc, m_crBorderShadow, m_crBorderShadow);
	}
}
