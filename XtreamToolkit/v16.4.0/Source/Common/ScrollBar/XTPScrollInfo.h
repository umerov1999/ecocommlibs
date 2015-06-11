// XTPScrollInfo.h : Menu scroll support class
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

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPSCROLLINFO_H__)
#define __XTPSCROLLINFO_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define XTP_TID_HOVERUP   5001
#define XTP_TID_HOVERDOWN 5002

class XTP_SCROLLINFO
{
public:
	class XTP_BTNSCROLL
	{
	public:
		void Init(CWnd *pParent, UINT nID)
		{
			m_pParent = pParent;
			m_nID = nID;
			m_rc.SetRectEmpty();
			m_nTimer = 0;
		}

		UINT m_nID;
		UINT m_nTimer;
		CRect m_rc;
		CWnd *m_pParent;

		void KillTimer()
		{
			if (0 != m_nTimer)
			{
				m_pParent->KillTimer(m_nTimer);
				m_nTimer = 0;
			}
		}

		void SetTimer()
		{
			if (m_nTimer == 0)
			{
				m_nTimer = (UINT)m_pParent->SetTimer(m_nID, 200, 0);
			}
		}

		BOOL OnMouseMove(CPoint point)
		{
			if (m_rc.PtInRect(point))
			{
				SetTimer();
				return TRUE;
			}
			else if (m_nTimer)
			{
				KillTimer();
			}
			return FALSE;
		}
	};

	BOOL bScroll;
	int nScrollFirst, nScrollLast;
	XTP_BTNSCROLL btnUp;
	XTP_BTNSCROLL btnDown;

	void Init(CWnd *pWnd, UINT nIDUp, UINT nIDDown)
	{
		btnUp.Init(pWnd, nIDUp);
		btnDown.Init(pWnd, nIDDown);
		bScroll = nScrollFirst = nScrollLast = 0;
	}
};

#endif // #if !defined(__XTPSCROLLINFO_H__)
