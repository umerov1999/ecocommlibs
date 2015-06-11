// XTPComboBoxThemeResource.cpp: implementation of the CXTPComboBoxThemeResource class.
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
#include "math.h"

#include <Common/XTPWinThemeWrapper.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPDrawHelpers.h>
#include <Common/XTPResourceImage.h>

#include <Controls/Util/XTPControlTheme.h>
#include <Controls/Combo/XTPComboBox.h>
#include <Controls/Combo/XTPComboBoxTheme.h>

#include "XTPComboBoxThemeResource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPComboBoxThemeResource::CXTPComboBoxThemeResource()
{

}

CXTPComboBoxThemeResource::~CXTPComboBoxThemeResource()
{

}

void CXTPComboBoxThemeResource::RefreshMetrics(CXTPComboBox* pComboBox)
{
	CXTPComboBoxTheme::RefreshMetrics(pComboBox);

	const COLORREF clrBorderHilite = XTPResourceImages()->GetImageColor(_T("RibbonComboBox"), _T("BorderHilite"));
	m_clrBorderHot.SetStandardValue(clrBorderHilite);

	const COLORREF clrBorder = XTPResourceImages()->GetImageColor(_T("RibbonComboBox"), _T("Border"));
	m_clrBorderNormal.SetStandardValue(clrBorder);

	const COLORREF clrBorderDisabled = XTPResourceImages()->GetImageColor(_T("RibbonComboBox"), _T("BorderDisabled"));
	m_clrBorderDisabled.SetStandardValue(clrBorderDisabled);
}

BOOL CXTPComboBoxThemeResource::DrawComboBox(CDC* pDC, CXTPComboBox* pComboBox)
{
	CXTPResourceImage* pImage = XTPResourceImages()->LoadFile(_T("TOOLBARBUTTONSCOMBODROPDOWN"));

	if (!pImage)
	{
		return FALSE;
	}
	else
	{
		CRect rc = pComboBox->GetComboBoxRect();
		HBRUSH hBrush = pComboBox->GetClientBrush(pDC);

		BOOL bPressed = pComboBox->SendMessage(CB_GETDROPPEDSTATE, 0, 0L) != 0;
		BOOL bSelected = pComboBox->IsFocused() || pComboBox->IsHighlighted();
		BOOL bEnabled = pComboBox->IsWindowEnabled();
		BOOL bRightAlign = pComboBox->GetExStyle() & WS_EX_RIGHT;

		pDC->SelectClipRgn(NULL);

		DrawFrame(pDC->GetSafeHdc(), rc, 3, hBrush);

		if (bRightAlign)
			FillRect(pDC->GetSafeHdc(), CRect(rc.left, rc.top, rc.left + GetSystemMetrics(SM_CXVSCROLL) + 3, rc.bottom), hBrush);
		else
			FillRect(pDC->GetSafeHdc(), CRect(rc.right - GetSystemMetrics(SM_CXVSCROLL) - 2, rc.top, rc.right, rc.bottom), hBrush);

		CRect rcBtn(rc.right - m_nThumbWidth, rc.top, rc.right, rc.bottom);
		if (bRightAlign)
			rcBtn = CRect(rc.left, rc.top, rc.left + m_nThumbWidth, rc.bottom);

		if (!bEnabled)
		{
			pDC->Draw3dRect(rc, m_clrBorderDisabled, m_clrBorderDisabled);
			pImage->DrawImage(pDC, rcBtn, pImage->GetSource(4, 5), CRect(2, 2, 2, 2));
		}
		else if (bPressed)
		{
			pDC->Draw3dRect(rc, m_clrBorderNormal, m_clrBorderNormal);
			pImage->DrawImage(pDC, rcBtn, pImage->GetSource(3, 5), CRect(2, 2, 2, 2));
		}
		else if (bSelected)
		{
			pDC->Draw3dRect(rc, m_clrBorderNormal, m_clrBorderNormal);
			pImage->DrawImage(pDC, rcBtn, pImage->GetSource(2, 5), CRect(2, 2, 2, 2));
		}
		else
		{
			pImage->DrawImage(pDC, rcBtn, pImage->GetSource(0, 5), CRect(2, 2, 2, 2));

			if (pComboBox->GetFlatStyle())
			{
				pDC->Draw3dRect(rc, ::GetSysColor(COLOR_3DFACE), ::GetSysColor(COLOR_3DFACE));
				rc.DeflateRect(1,1);
				pDC->Draw3dRect(rc, ::GetSysColor(COLOR_WINDOW), ::GetSysColor(COLOR_WINDOW));
			}
			else
			{
				pDC->Draw3dRect(rc, m_clrBorderNormal, m_clrBorderNormal);
			}
		}

		CPoint pt = rcBtn.CenterPoint();

		pImage = XTPResourceImages()->LoadFile(_T("TOOLBARBUTTONDROPDOWNGLYPH"));

		int x = rcBtn.left + ((rcBtn.Width()-5)/2);
		int y = rcBtn.top + ((rcBtn.Height()-4)/2)+1;

		CRect rcArrow(x, y, x + 5, y + 4);

		pImage->DrawImage(pDC, rcArrow, pImage->GetSource(!bEnabled ? 3 : bSelected ? 1 : 0, 4),
			CRect(0, 0, 0, 0), 0xff00ff);

		if (!bSelected && !bPressed)
		{
			if (pComboBox->GetFlatStyle())
			{
				CXTPPenDC pen(pDC->GetSafeHdc(), bEnabled ? ::GetSysColor(COLOR_WINDOW) : ::GetSysColor(COLOR_3DFACE));
				pDC->MoveTo(rcBtn.left,rcBtn.top+1);
				pDC->LineTo(rcBtn.left,rcBtn.bottom-1);
			}
			else
			{
				CXTPPenDC pen(pDC->GetSafeHdc(), bEnabled ? m_clrBorderNormal :  m_clrBorderDisabled);
				pDC->MoveTo(rcBtn.left,rcBtn.top+1);
				pDC->LineTo(rcBtn.left,rcBtn.bottom-1);
			}
		}
	}

	return TRUE;
}