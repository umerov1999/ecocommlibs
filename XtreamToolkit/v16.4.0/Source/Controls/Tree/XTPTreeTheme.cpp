// XTPTreeTheme.cpp: implementation of the CXTPTreeTheme class.
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

#include <Common/XTPWinThemeWrapper.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPDrawHelpers.h>
#include "Common/XTPSystemHelpers.h"

#include <Controls/Util/XTPControlTheme.h>
#include "XTPTreeTheme.h"

#include "XTPTreeBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPTreeTheme::CXTPTreeTheme()
{
	m_clrBackground = COLORREF_NULL;
	m_clrText = COLORREF_NULL;

	m_bDrawFocusRect = TRUE;
}

CXTPTreeTheme::~CXTPTreeTheme()
{

}

void CXTPTreeTheme::RefreshMetrics(CXTPTreeBase* pTreeBase)
{
	m_bDrawFocusRect = TRUE;

	m_clrBackground = GetXtremeColor(COLOR_WINDOW); // return the system window color.
	m_clrText = GetXtremeColor(COLOR_WINDOWTEXT);

	CTreeCtrl* pTreeCtrl = pTreeBase->GetSelfTreeCtrl();

	if (pTreeCtrl)
	{
		// if the tree is disabled return disabled back color.
		if (!pTreeCtrl->IsWindowEnabled())
		{
			m_clrBackground = GetXtremeColor(COLOR_3DFACE);
		}
		else
		{
#if _MSC_VER >= 1200 // MFC 6.0

			if (XTPSystemVersion()->GetComCtlVersion() >= MAKELONG(71, 4))
			{
				// is there a user defined color available ?
				COLORREF crBack = pTreeCtrl->GetBkColor();
				if (crBack != COLORREF_NULL)
					m_clrBackground = crBack;
			}
#endif
		}

		// if the tree is disabled return disabled text color.
		if (!pTreeCtrl->IsWindowEnabled())
			m_clrText = GetXtremeColor(COLOR_GRAYTEXT);
		else
		{
#if _MSC_VER >= 1200 // MFC 6.0
			if (XTPSystemVersion()->GetComCtlVersion() >= MAKELONG(71, 4))
			{
				// is ther a user defined color available ?
				COLORREF crText = pTreeCtrl->GetTextColor();
				if (crText != COLORREF_NULL)
					m_clrText = crText;
			}

#endif
		}
	}

	m_clrItemBackgroundSelected = GetXtremeColor(COLOR_HIGHLIGHT);
	m_clrItemBackgroundSelectedDisabled = GetXtremeColor(COLOR_GRAYTEXT);

	m_clrItemBackgroundNotFocused = GetXtremeColor(COLOR_3DFACE);
	m_clrItemBackgroundNotFocusedDisabled = GetXtremeColor(COLOR_GRAYTEXT);
}

COLORREF CXTPTreeTheme::GetItemBackColor(UINT uState, bool bTreeHasFocus, DWORD dwStyle, COLORREF crBack, CTreeCtrl* pTreeCtrl) const
{
	// if the tree item is selected or drop highlighted.
	if (uState & TVIS_SELECTED || uState & TVIS_DROPHILITED)
	{
		// if the tree has focus and draw the text background hilite color.
		if (bTreeHasFocus || uState & TVIS_DROPHILITED)
		{
			if (pTreeCtrl && pTreeCtrl->IsWindowEnabled())
				return m_clrItemBackgroundSelected;
			else
				return m_clrItemBackgroundSelectedDisabled;
		}

		// if the tree does not have focus and the TVS_SHOWSELALWAYS flag is
		// set draw the text background hilite color.
		else if (dwStyle & TVS_SHOWSELALWAYS)
		{
			if (pTreeCtrl && pTreeCtrl->IsWindowEnabled())
				return m_clrItemBackgroundNotFocused;
			else
				return m_clrItemBackgroundNotFocusedDisabled;
		}
	}

	return crBack;
}

COLORREF CXTPTreeTheme::GetItemTextColor(UINT uState, bool bTreeHasFocus, DWORD dwStyle, COLORREF crText, CTreeCtrl* pTreeCtrl) const
{
	ASSERT(pTreeCtrl);

	// if the tree item is selected or drop highlighted.
	if (uState & TVIS_SELECTED || uState & TVIS_DROPHILITED)
	{
		// if the tree has focus and draw the text hilite color.
		if (bTreeHasFocus || uState & TVIS_DROPHILITED)
		{
			if (pTreeCtrl && pTreeCtrl->IsWindowEnabled())
				return GetXtremeColor(COLOR_HIGHLIGHTTEXT);
			else
				return GetXtremeColor(COLOR_3DFACE);
		}

		// if the tree does not have focus and the TVS_SHOWSELALWAYS flag is
		// set draw the text hilite color.
		else if (dwStyle & TVS_SHOWSELALWAYS)
		{
			if (pTreeCtrl && pTreeCtrl->IsWindowEnabled())
				return GetXtremeColor(COLOR_WINDOWTEXT);
			else
				return GetXtremeColor(COLOR_3DFACE);
		}
	}

	return crText;
}

COLORREF CXTPTreeTheme::GetTreeBackColor() const
{
	return m_clrBackground;
}

COLORREF CXTPTreeTheme::GetTreeTextColor() const
{
	return m_clrText;
}


// VS2012 Theme: will be moved to a separate file when theming of tree control will be complete [ip]

CXTPTreeVisualStudio2012Theme::CXTPTreeVisualStudio2012Theme(BOOL bLight)
	: m_bLight(bLight)
{

}

void CXTPTreeVisualStudio2012Theme::RefreshMetrics(CXTPTreeBase* pTreeBase)
{
	CXTPTreeTheme::RefreshMetrics(pTreeBase);

	m_bDrawFocusRect = FALSE;

	m_clrItemBackgroundSelected = RGB(51,153,255);

	if (m_bLight)
	{
		m_clrBackground = RGB(246,246,246);
		m_clrText = RGB(30,30,30);

		m_clrItemBackgroundNotFocused = RGB(204,206,219);
	}
	else
	{
		m_clrBackground = RGB(45,45,48);
		m_clrText = RGB(241,241,241);

		m_clrItemBackgroundNotFocused = RGB(63,63,70);
	}
}
