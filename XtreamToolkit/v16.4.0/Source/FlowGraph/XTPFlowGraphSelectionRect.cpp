// XTPFlowGraphSelectionRect.cpp : implementation of the CXTPFlowGraphSelectionRect class.
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
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

#include "XTPFlowGraphImage.h"
#include "XTPFlowGraphPage.h"

#include "XTPFlowGraphSelectionRect.h"

#include "XTPFlowGraphTools.h"
#include "XTPFlowGraphDrawContext.h"

#define COLOR_1         RGB (0,   168, 255)
#define COLOR_2         RGB (40,  146, 201)
#define COLOR_3         RGB (204, 230, 255)
#define COLOR_4         RGB (153, 204, 255)

int CXTPFlowGraphSelectionRect::m_iSizeSquareSize = 7;

CXTPFlowGraphSelectionRect::CXTPFlowGraphSelectionRect()
{
	m_pSizeSquare = new CXTPFlowGraphImage();

	CreateSizeSquare();
}

CXTPFlowGraphSelectionRect::~CXTPFlowGraphSelectionRect()
{
	delete m_pSizeSquare;
}

void CXTPFlowGraphSelectionRect::CreateSizeSquare()
{
	if (m_pSizeSquare->GetSize() == CSize(0,0))
	{
#define FreeCreateAndSelectPen(d, p, c) if (p) ::DeleteObject(p); p = ::CreatePen(PS_SOLID, 1, c); ::SelectObject(d, p);

		HPEN hPen = NULL;
		HDC hMemDC = ::CreateCompatibleDC(::GetDC(NULL));

		HDC hDC = ::GetDC(NULL);
		HBITMAP hBitmap = ::CreateCompatibleBitmap(hDC, m_iSizeSquareSize, m_iSizeSquareSize);
		::SelectObject ( hMemDC, hBitmap );

		HBRUSH hBrush = ::CreateSolidBrush(COLOR_4);

		CRect rctmp(0,0,7,7);
		FillRect(hMemDC, rctmp, hBrush );

		// draw

		MoveToEx(hMemDC, 0, 0, NULL);
		FreeCreateAndSelectPen(hMemDC, hPen, COLOR_1);
		::LineTo(hMemDC, 6, 0);
		MoveToEx(hMemDC, 0, 0, NULL);
		::LineTo(hMemDC, 0, 7);
		::MoveToEx(hMemDC, 1, 6, NULL);
		FreeCreateAndSelectPen(hMemDC, hPen, COLOR_2);
		::LineTo(hMemDC, 6, 6);
		::LineTo(hMemDC, 6, -1);
		::MoveToEx(hMemDC, 1, 1, NULL);
		FreeCreateAndSelectPen(hMemDC, hPen, COLOR_3);
		::LineTo(hMemDC, 6, 1);
		::MoveToEx(hMemDC, 1, 1, NULL);
		::LineTo(hMemDC, 1, 6);

		// transfer bitmap to image

		m_pSizeSquare->SetBitmap(hBitmap);

		// free resources
		::ReleaseDC(NULL, hDC);
		::DeleteObject(hBrush);
		::DeleteObject(hPen);
		::DeleteObject(hBitmap);
		::DeleteDC(hMemDC);
	}
}

void CXTPFlowGraphSelectionRect::GetSizingRects(const CRect &rc, CRect *rects)
{
	int nRectHalfSize = m_iSizeSquareSize/2;

	#define RectFromPoint(p) CRect(p.x - nRectHalfSize, p.y - nRectHalfSize, p.x + nRectHalfSize, p.y + nRectHalfSize)

	rects[0] = RectFromPoint( rc.TopLeft() );
	rects[1] = RectFromPoint( CPoint(rc.TopLeft().x + rc.Width()/2, rc.TopLeft().y) );
	rects[2] = RectFromPoint( CPoint(rc.TopLeft().x + rc.Width(), rc.TopLeft().y) );
	rects[3] = RectFromPoint( CPoint(rc.TopLeft().x + rc.Width(), rc.TopLeft().y+rc.Height()/2) );
	rects[4] = RectFromPoint( rc.BottomRight() );
	rects[5] = RectFromPoint( CPoint(rc.BottomRight().x - rc.Width()/2, rc.BottomRight().y) );
	rects[6] = RectFromPoint( CPoint(rc.BottomRight().x - rc.Width(), rc.BottomRight().y) );
	rects[7] = RectFromPoint( CPoint(rc.BottomRight().x - rc.Width(), rc.BottomRight().y-rc.Height()/2) );
}

void CXTPFlowGraphSelectionRect::Draw(CXTPFlowGraphDrawContext* pDC, const CRect &rc)
{
	CRect smallRects[8];
	GetSizingRects(rc, smallRects);

	const int nMargin = 4;

	for (int i=0; i<8; i++)
	{
		// draw rectangles
		pDC->DrawImage(m_pSizeSquare, smallRects[i]);

		// draw lines
		CPoint p1 = smallRects[i].CenterPoint();
		CPoint p2 = (i != 7) ? smallRects[i+1].CenterPoint() : smallRects[0].CenterPoint();

		if (p1.y == p2.y) // horizontal
		{
			if (p2.x > p1.x)
			{
				p1.x += nMargin;
				p2.x -= nMargin;
			}
			else
			{
				p2.x += nMargin;
				p1.x -= nMargin;
			}
		}
		else // vertical
		{
			if (p2.y > p1.y)
			{
				p1.y += nMargin;
				p2.y -= nMargin;
			}
			else
			{
				p2.y += nMargin;
				p1.y -= nMargin;
			}
		}

		pDC->SetPen(COLOR_1, 1, TRUE);
		pDC->Line(p1, p2);
	}
}

SelectionRectHitTest CXTPFlowGraphSelectionRect::HitTest(const CRect &rc, const CPoint &point)
{
	CRect smallRects[8];
	GetSizingRects(rc, smallRects);

	for (int i=0; i<8; i++)
		if (smallRects[i].PtInRect(point))
			return (SelectionRectHitTest)i;

	return SelectionRectHitTestNone;
}

void CXTPFlowGraphSelectionRect::ResizeRect(CRect &rcRect, CSize szMinSize, CPoint ptOffset, SelectionRectHitTest hitPoint)
{
	switch (hitPoint)
	{
		case SelectionRectHitTestBottomRight:
		{
			rcRect.right += ptOffset.x;
			rcRect.bottom += ptOffset.y;

			break;
		}

		case SelectionRectHitTestBottomCenter:
		{
			rcRect.bottom += ptOffset.y;

			break;
		}

		case SelectionRectHitTestMiddleRight:
		{
			rcRect.right += ptOffset.x;

			break;
		}

		case SelectionRectHitTestTopLeft:
		{
			CRect rectTest = rcRect;
			rectTest.left += ptOffset.x;
			rectTest.top += ptOffset.y;

			if ( rectTest.Width() >= szMinSize.cx)
				rcRect.left += ptOffset.x;
			else
			{
				rcRect.left = rcRect.right - szMinSize.cx;
			}

			if ( rectTest.Height() >= szMinSize.cy)
				rcRect.top += ptOffset.y;
			else
			{
				rcRect.top = rcRect.bottom - szMinSize.cy;
			}

			break;
		}

		case SelectionRectHitTestTopCenter:
		{
			CRect rectTest = rcRect;
			rectTest.left += ptOffset.x;
			rectTest.top += ptOffset.y;

			if ( rectTest.Height() >= szMinSize.cy)
				rcRect.top += ptOffset.y;
			else
			{
				rcRect.top = rcRect.bottom - szMinSize.cy;
			}

			break;
		}

		case SelectionRectHitTestMiddleLeft:
		{
			CRect rectTest = rcRect;
			rectTest.left += ptOffset.x;
			rectTest.top += ptOffset.y;

			if ( rectTest.Width() >= szMinSize.cx)
				rcRect.left += ptOffset.x;
			else
			{
				rcRect.left = rcRect.right - szMinSize.cx;
			}

			break;
		}

		case SelectionRectHitTestBottomLeft:
		{
			CRect rectTest = rcRect;
			rectTest.left += ptOffset.x;
			rectTest.bottom += ptOffset.y;

			if ( rectTest.Width() >= szMinSize.cx)
				rcRect.left += ptOffset.x;
			else
			{
				rcRect.left = rcRect.right - szMinSize.cx;
			}

			if ( rectTest.Height() >= szMinSize.cy)
				rcRect.bottom += ptOffset.y;
			else
			{
				rcRect.bottom = rcRect.top + szMinSize.cy;
			}

			break;
		}

		case SelectionRectHitTestTopRight:
		{
			CRect rectTest = rcRect;
			rectTest.right += ptOffset.x;
			rectTest.top += ptOffset.y;

			if ( rectTest.Width() >= szMinSize.cx)
				rcRect.right += ptOffset.x;
			else
			{
				rcRect.right = rcRect.left + szMinSize.cx;
			}

			if ( rectTest.Height() >= szMinSize.cy)
				rcRect.top += ptOffset.y;
			else
			{
				rcRect.top = rcRect.bottom - szMinSize.cy;
			}

			break;
		}
	}
}

UINT CXTPFlowGraphSelectionRect::GetCursorBySelectionHitTest(SelectionRectHitTest hitTest)
{
	UINT uCursorID = 0;
	switch (hitTest)
	{
		case SelectionRectHitTestTopLeft:
		case SelectionRectHitTestBottomRight:
		{
			uCursorID = 32642; // IDC_SIZENWSE

			break;
		}

		case SelectionRectHitTestTopRight:
		case SelectionRectHitTestBottomLeft:
		{
			uCursorID = 32643; // IDC_SIZENWSW

			break;
		}

		case SelectionRectHitTestTopCenter:
		case SelectionRectHitTestBottomCenter:
		{
			uCursorID = 32645; // IDC_SIZENS

			break;
		}

		case SelectionRectHitTestMiddleRight:
		case SelectionRectHitTestMiddleLeft:
		{
			uCursorID = 32644; // IDC_SIZEWE

			break;
		}
	}

	return uCursorID;
}
