// XTPCaptionTheme.cpp: implementation of the CXTPCaptionTheme class.
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

#include "StdAfx.h"

#include <Common/XTPDrawHelpers.h>
#include <Common/XTPColorManager.h>

#include <Controls/Util/XTPControlTheme.h>
#include <Controls/Button/XTPButtonTheme.h>
#include <Controls/Button/XTPButton.h>

#include "XTPCaption.h"
#include "XTPCaptionTheme.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPCaptionTheme

CXTPCaptionTheme::CXTPCaptionTheme()
{
}

void CXTPCaptionTheme::RefreshMetrics(CXTPCaption* /*pCaption*/)
{
	m_clrEdgeShadow.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));
	m_clrEdgeLight.SetStandardValue(GetXtremeColor(COLOR_3DHILIGHT));
}

void CXTPCaptionTheme::DrawCaptionBack(CDC* pDC, CXTPCaption* pCaption, CRect& rcItem)
{
	CRect rc = rcItem;

	// Set the background mode to transparent.
	pDC->SetBkMode(TRANSPARENT);

	// repaint the background.
	pDC->FillSolidRect(rc, pCaption->m_clrBorder);

	// draw the caption border.
	if (pCaption->m_dwExStyle & CPWS_EX_GROOVE_EDGE)
	{
		pDC->Draw3dRect(rc, m_clrEdgeShadow, m_clrEdgeLight);
		rc.DeflateRect(1, 1);
		pDC->Draw3dRect(rc, m_clrEdgeLight, m_clrEdgeShadow);

		if (pCaption->m_nBorder == 0)
		{
			rc.DeflateRect(1, 1);
		}
	}

	else
	if (pCaption->m_dwExStyle & CPWS_EX_RAISED_EDGE)
	{
		pDC->Draw3dRect(rc, m_clrEdgeLight, m_clrEdgeShadow);

		if (pCaption->m_nBorder == 0)
		{
			rc.DeflateRect(1, 1);
		}
	}

	// draw the caption banner border.
	rc.DeflateRect(pCaption->m_nBorder, pCaption->m_nBorder);
	pDC->FillSolidRect(rc, pCaption->m_clrFace);
}

void CXTPCaptionTheme::DrawCaptionText(CDC* pDC, CXTPCaption* pCaption)
{
		// if the caption text is empty, return.
	if (pCaption->GetCaptionText().IsEmpty())
	{
		return;
	}

	// Get the display area for the text.
	CRect rcText(pCaption->GetTextRect());

	// Set the text color and select the caption font.
	CXTPFontDC fontDC(pDC, pCaption->GetFont());
	pDC->SetTextColor(pCaption->m_clrText);

	// draw the text, and select the original font.
	pDC->DrawText(pCaption->GetCaptionText(), rcText,
		DT_SINGLELINE | DT_END_ELLIPSIS | DT_VCENTER | DT_CENTER | DT_NOPREFIX);
}

void CXTPCaptionTheme::DrawCaptionIcon(CDC* pDC, CXTPCaption* pCaption, CRect& rcItem)
{
	// If no icon was defined, just return.
	if (pCaption->m_hIcon == NULL)
		return;

	CRect rc = rcItem;
	rc.left = (rc.right - pCaption->m_sizeIcon.cx)-(pCaption->m_nBorder + 2);

	// stop the icon at the left hand border
	if (rc.left < pCaption->m_nBorder)
	{
		rc.left = pCaption->m_nBorder;
	}

	rc.top = (rcItem.Height() - pCaption->m_sizeIcon.cy)/2;
	rc.right = rc.left + pCaption->m_sizeIcon.cx;
	rc.bottom = rc.top + pCaption->m_sizeIcon.cy;

	// Only draw the icon if it will fit into the space between the borders
	if (rc.right < rcItem.right - pCaption->m_nBorder)
	{
		// draw the icon associated with the caption.
		::DrawIconEx(pDC->GetSafeHdc(), rc.left, rc.top, pCaption->m_hIcon,
			rc.Width(), rc.Height(), NULL, (HBRUSH)NULL, DI_NORMAL);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CXTPCaptionThemeOfficeXP

CXTPCaptionThemeOfficeXP::CXTPCaptionThemeOfficeXP()
{
	m_nTheme = xtpControlThemeOfficeXP;
}

void CXTPCaptionThemeOfficeXP::DrawCaptionBack(CDC* pDC, CXTPCaption* pCaption, CRect& rcItem)
{
	CRect rc = rcItem;

	pDC->SetBkMode(TRANSPARENT);

	// repaint the background.
	pDC->FillSolidRect(rc, pCaption->m_clrBorder);

	if (!pCaption->IsAppCaption())
	{
		pDC->Draw3dRect(rc, GetXtremeColor(COLOR_3DHILIGHT), GetXtremeColor(COLOR_3DSHADOW));
	}
	else
	{
		// draw the caption banner border.
		rc.DeflateRect(pCaption->m_nBorder, pCaption->m_nBorder);
		pDC->FillSolidRect(rc, pCaption->m_clrFace);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CXTPCaptionThemeOffice2003

CXTPCaptionThemeOffice2003::CXTPCaptionThemeOffice2003()
{
	m_nTheme = xtpControlThemeOffice2003;
}

void CXTPCaptionThemeOffice2003::DrawCaptionBack(CDC* pDC, CXTPCaption* pCaption, CRect& rcItem)
{
	pDC->SetBkMode(TRANSPARENT);

	if (pCaption->IsAppCaption())
	{
		XTPDrawHelpers()->GradientFill(pDC, rcItem, XTPColorManager()->grcCaption, FALSE);
		pDC->Draw3dRect(rcItem, GetXtremeColor(XPCOLOR_FRAME), GetXtremeColor(XPCOLOR_FRAME));
	}
	else
	{
		XTPDrawHelpers()->GradientFill(pDC, rcItem, XTPColorManager()->grcToolBar, FALSE);
		pDC->FillSolidRect(rcItem.left, rcItem.bottom - 1, rcItem.Width(), 1, GetXtremeColor(XPCOLOR_FRAME));
	}
}

/////////////////////////////////////////////////////////////////////////////
// CXTPCaptionButtonTheme

CXTPCaptionButtonTheme::CXTPCaptionButtonTheme()
{
	m_bOffsetHiliteText = FALSE;
}

void CXTPCaptionButtonTheme::DrawButtonBackground(CDC* pDC, CXTPButton* pButton)
{
	CRect rcItem = pButton->GetButtonRect();

	CXTPCaption* pCaption = ((CXTPCaptionButton*)pButton)->GetCaption();

	// Paint the background.
	if (pButton->IsHighlighted() || (pButton->IsPushed() || pButton->GetChecked()))
	{
		pDC->FillSolidRect(rcItem, GetXtremeColor(COLOR_3DFACE));


		COLORREF clrTopLeft = GetXtremeColor(COLOR_3DHILIGHT);
		COLORREF clrBottomRight = GetXtremeColor(COLOR_3DSHADOW);

		if (pButton->IsPushed() || pButton->GetChecked())
		{
			COLORREF c = clrTopLeft;
			clrTopLeft = clrBottomRight;
			clrBottomRight = c;
		}

		pDC->Draw3dRect(rcItem, clrTopLeft, clrBottomRight);
	}
	else
	{
		if (::IsWindow(pCaption->GetSafeHwnd()))
		{
			m_crBack = pCaption->m_clrFace;
		}

		pDC->FillSolidRect(rcItem, m_crBack);
	}
}

COLORREF CXTPCaptionButtonTheme::GetTextColor(CXTPButton* pButton)
{
	if (!pButton->IsWindowEnabled())
		return GetXtremeColor(COLOR_GRAYTEXT);

	if (pButton->IsPushed() || pButton->GetChecked() || pButton->IsHighlighted())
		return GetXtremeColor(COLOR_BTNTEXT);

	CXTPCaption* pCaption = ((CXTPCaptionButton*)pButton)->GetCaption();
	if (::IsWindow(pCaption->GetSafeHwnd()))
	{
		return pCaption->m_clrText;
	}

	return m_crText;
}

void CXTPCaptionButtonTheme::DrawButtonText(CDC* pDC, CXTPButton* pButton)
{
	CXTPCaption* pCaption = ((CXTPCaptionButton*)pButton)->GetCaption();
	if (::IsWindow(pCaption->GetSafeHwnd()))
	{
		if (pCaption->IsAppCaption() && !pCaption->GetCaptionText().IsEmpty())
		{
			// select font into device context.
			CXTPFontDC fontDC(pDC, pButton->GetFont());
			pDC->SetTextColor(GetTextColor(pButton));

			CXTPClientRect rcItem(pButton);

			if (pButton->IsHighlighted() && m_bOffsetHiliteText)
				rcItem.OffsetRect(1, 1);

			// draw the text, and select the original font.
			pDC->DrawText(pCaption->GetCaptionText(), rcItem,
				DT_SINGLELINE | DT_END_ELLIPSIS | DT_VCENTER | DT_CENTER | DT_NOPREFIX);
		}
	}
}
