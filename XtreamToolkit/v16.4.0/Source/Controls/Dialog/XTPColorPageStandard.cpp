// XTPColorPageStandard.cpp : implementation file
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

#include "stdafx.h"

#include <Common/XTPDrawHelpers.h>

#include "../Resource.h"
#include "../Defines.h"
#include "XTPColorDialog.h"
#include "XTPColorPageStandard.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPColorHex::CRgnCell

// Color cell width in normal dpi (96)
#define XTP_CELL_WIDTH 14

void CXTPColorHex::CRgnCell::ComputeHexagon(
	const CTX_DATA::HEXAGON_DATA& data,
	float left,
	float top,
	CXTPPoint2f(&points)[6])
{
	float right = left + data.width;
	float bottom = top + data.height;
	points[0].SetPoint(left + data.half, top);
	points[1].SetPoint(right, top + data.roof);
	points[2].SetPoint(right, bottom - data.roof);
	points[3].SetPoint(points[0].X, top + data.height);
	points[4].SetPoint(left, points[2].Y);
	points[5].SetPoint(left, points[1].Y);
}

void CXTPColorHex::CRgnCell::ConvertFloatToIntPoints(
	const CXTPPoint2f(&from)[6],
	CPoint(&to)[6])
{
	to[0].x = static_cast<int>(from[0].X);
	to[0].y = static_cast<int>(from[0].Y);

	to[1].x = static_cast<int>(from[1].X);
	to[1].y = static_cast<int>(from[1].Y);

	to[2].x = static_cast<int>(from[2].X);
	to[2].y = static_cast<int>(from[2].Y);

	to[3].x = static_cast<int>(from[3].X);
	to[3].y = static_cast<int>(from[3].Y);

	to[4].x = static_cast<int>(from[4].X);
	to[4].y = static_cast<int>(from[4].Y);

	to[5].x = static_cast<int>(from[5].X);
	to[5].y = static_cast<int>(from[5].Y);
}

CXTPColorHex::CRgnCell::CRgnCell(
	const CTX_DATA& data,
	CXTPRectF& rect,
	COLORREF color,
	float u,
	float d)
	: m_color(color)
{
	m_iCell[0] = static_cast<int>(u);
	m_iCell[1] = static_cast<int>(d);

	rect.Offset(data.sm.width, 0);
	m_rect = rect;

	ComputeHexagon(data.sm, rect.X, rect.Y, m_pts);
	CPoint pts[6];
	ConvertFloatToIntPoints(m_pts, pts);
	VERIFY(CreatePolygonRgn(pts, 6, WINDING));
}

CXTPColorHex::CRgnCell::CRgnCell(
	const CTX_DATA& data,
	CXTPRectF& rect,
	COLORREF color,
	float u,
	float d,
	float x,
	float y,
	BOOL bLarge /*= FALSE*/)
	: m_color(color)
{
	m_iCell[0] = static_cast<int>(u);
	m_iCell[1] = static_cast<int>(d);

	if (bLarge)
	{
		rect = CXTPRectF(x, y, x + data.lg.width, y + data.lg.height);
		ComputeHexagon(data.lg, rect.X, rect.Y, m_pts);
	}
	else
	{
		rect = CXTPRectF(x, y, x + data.sm.width, y + data.sm.height);
		ComputeHexagon(data.sm, rect.X, rect.Y, m_pts);
	}

	m_rect = rect;
	CPoint pts[6];
	ConvertFloatToIntPoints(m_pts, pts);
	VERIFY(CreatePolygonRgn(pts, 6, WINDING));
}

BOOL CXTPColorHex::CRgnCell::DrawCell(CDC* pDC)
{
	if (pDC->GetSafeHdc())
	{
		CBrush brush(m_color);
		return pDC->FillRgn(this, &brush);
	}
	return FALSE;
}

BOOL CXTPColorHex::CRgnCell::DrawSelection(CDC* pDC)
{
	if (!pDC->GetSafeHdc())
		return FALSE;

	CBrush brush(RGB(0xff,0xff,0xff));
	if (pDC->FrameRgn(this, &brush, 4, 2))
	{
		CXTPBrushDC brushDC(*pDC, RGB(0x00,0x00,0x00));

		CPoint pts[6];

		int i;
		for (i = 0; i < 6; ++i)
		{
			pts[i].x = static_cast<int>(m_pts[i].X);
			pts[i].y = static_cast<int>(m_pts[i].Y);
		}

		pts[0].Offset( 0, 4);
		pts[1].Offset(-4, 2);
		pts[2].Offset(-4,-2);
		pts[3].Offset( 0,-4);
		pts[4].Offset( 4,-2);
		pts[5].Offset( 4, 2);

		for (i = 0; i < 6; ++i)
		{
			// Draw inner contour
			int toIndex = (i + 1) < 6 ? i + 1 : 0;
			pDC->MoveTo(
				static_cast<int>(m_pts[i].X),
				static_cast<int>(m_pts[i].Y));
			pDC->LineTo(
				static_cast<int>(m_pts[toIndex].X),
				static_cast<int>(m_pts[toIndex].Y));

			// Draw outer contour
			pDC->MoveTo(pts[i]);
			pDC->LineTo(pts[toIndex]);
		}

		return TRUE;
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CXTPColorHex

bool CXTPColorHex::IsValidColor(COLORREF color) const
{
	for (POSITION pos = m_rgnList.GetHeadPosition(); pos;)
	{
		CRgnCell* pCell = m_rgnList.GetNext(pos);
		if (pCell && pCell->m_color == color)
		{
			return true;
		}
	}
	return false;
}

CXTPColorHex::CXTPColorHex()
: m_clrColor((COLORREF)-1)
, m_bPreSubclassInit(true)
, m_pCell(NULL)
{

}

CXTPColorHex::~CXTPColorHex()
{
	while (!m_rgnList.IsEmpty())
	{
		CRgnCell* pCell = m_rgnList.RemoveHead();
		SAFE_DELETE(pCell);
	}
}

IMPLEMENT_DYNAMIC(CXTPColorHex, CStatic)

BEGIN_MESSAGE_MAP(CXTPColorHex, CStatic)
	//{{AFX_MSG_MAP(CXTPColorHex)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

#define CX_HEXDISP  182
#define CY_HEXDISP  199

void CXTPColorHex::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CXTPClientRect rc(this);
	CXTPBufferDC memDC(dc);

	if (m_bmpPicker.GetSafeHandle() == NULL)
	{
		// create bitmap
		m_bmpPicker.DeleteObject();
		m_bmpPicker.CreateCompatibleBitmap(&memDC,
			rc.Width(), rc.Height());

		// create picker DC
		CDC dcPicker;
		dcPicker.CreateCompatibleDC(&memDC);
		CBitmap* pOldBitmap = dcPicker.SelectObject(&m_bmpPicker);

		dcPicker.SetBkMode(TRANSPARENT);

		HBRUSH hBrush = (HBRUSH)GetParent()->SendMessage(WM_CTLCOLORSTATIC, (WPARAM)dcPicker.GetSafeHdc(), (LPARAM)m_hWnd);
		if (hBrush)
		{
			::FillRect(dcPicker.GetSafeHdc(), rc, hBrush);
		}
		else
		{
			dcPicker.FillSolidRect(rc, ::GetSysColor(COLOR_3DFACE));
		}

		for (POSITION pos = m_rgnList.GetHeadPosition(); pos;)
		{
			CRgnCell* pCell = m_rgnList.GetNext(pos);
			if (pCell)
			{
				pCell->DrawCell(&dcPicker);
			}
		}

		dcPicker.SelectObject(pOldBitmap);
	}

	memDC.DrawState(CPoint(0, 0), rc.Size(), &m_bmpPicker, DSS_NORMAL, 0);

	if (m_pCell)
	{
		// if white is selected we need to draw a selection for
		// both the large and small cells.
		if (m_pCell->m_color == RGB(0xff,0xff,0xff))
		{
			for (POSITION pos = m_rgnList.GetHeadPosition(); pos;)
			{
				CRgnCell* pCell = m_rgnList.GetNext(pos);
				if (pCell && pCell->m_color == RGB(0xff,0xff,0xff))
				{
					pCell->DrawSelection(&memDC);
				}
			}
		}

		// otherwise just draw selection the color.
		else
		{
			m_pCell->DrawSelection(&memDC);
		}
	}
}

BOOL CXTPColorHex::OnEraseBkgnd(CDC* pDC)
{
	UNREFERENCED_PARAMETER(pDC);
	return TRUE;
}

void CXTPColorHex::CreateCellArray()
{
	HDC hScreenDC = ::GetDC(HWND_DESKTOP);
	int nDPI = GetDeviceCaps(hScreenDC, LOGPIXELSX);
	::ReleaseDC(HWND_DESKTOP, hScreenDC);

	const float TanPi6 = 0.57735026918962576450914878050196f; // tan(Pi / 6)

	CRgnCell::CTX_DATA ctxData;
	ctxData.sm.width = (float(XTP_CELL_WIDTH) * nDPI) / XTP_DEFAULT_DISPLAY_DPI;
	ctxData.sm.half = ctxData.sm.width / 2;
	ctxData.sm.roof = TanPi6 * ctxData.sm.half;
	ctxData.sm.side = static_cast<float>(sqrt(
		ctxData.sm.half * ctxData.sm.half 
		+ ctxData.sm.roof * ctxData.sm.roof));
	ctxData.sm.height = ctxData.sm.roof + ctxData.sm.side + ctxData.sm.roof;

	ctxData.lg.width = ctxData.sm.width * 2;
	ctxData.lg.half = ctxData.sm.half * 2;
	ctxData.lg.height = ctxData.sm.height * 2;
	ctxData.lg.roof = ctxData.sm.roof * 2;
	ctxData.lg.side = ctxData.sm.side * 2;

	CXTPClientRect rcClient(this);
	float x = static_cast<float>(rcClient.Width() - ctxData.sm.width * 13) / 2;
	float y = 10;
	float dy = ctxData.sm.side + ctxData.sm.roof;

#define XOFFSET(x,cx) (x + ((ctxData.sm.width / 2) * cx))

	CXTPRectF rect; // row 1
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x00, 0x80, 0x80), -1, 8, XOFFSET(x, 6), y));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x00, 0x69, 0x80), -1, 9));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x00, 0x54, 0x80), -1, 10));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x00, 0x3d, 0x80), -1, 11));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x00, 0x27, 0x80), -1, 12));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x00, 0x12, 0x80), -1, 13));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x00, 0x00, 0x80), -1, 14));

	y += dy; // row 2
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x00, 0x80, 0x69), -1, 16, XOFFSET(x, 5), y));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x00, 0xc0, 0xc0), 0, 17));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x00, 0xa0, 0xc0), 1, 18));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x00, 0x80, 0xc0), 2, 19));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x00, 0x4b, 0xc0), 3, 20));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x00, 0x24, 0xc0), 4, 21));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x00, 0x00, 0xc0), 5, 22));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x12, 0x00, 0x80), 6, 23));

	y += dy; // row 3
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x00, 0x80, 0x54), 6, 25, XOFFSET(x, 4), y));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x00, 0xc0, 0xa0), 7, 26));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x00, 0xff, 0xff), 8, 27));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x00, 0xc0, 0xff), 9, 28));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x00, 0x80, 0xff), 10, 29));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x00, 0x46, 0xff), 11, 30));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x00, 0x00, 0xff), 12, 31));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x24, 0x00, 0xc0), 13, 32));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x27, 0x00, 0x80), 14, 33));

	y += dy; // row 4
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x00, 0x80, 0x3d), 14, 35, XOFFSET(x, 3), y));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x00, 0xc0, 0x80), 15, 36));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x00, 0xff, 0xc0), 16, 37));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x57, 0xff, 0xff), 17, 38));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x57, 0xc0, 0xff), 18, 39));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x57, 0x80, 0xff), 19, 40));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x57, 0x57, 0xff), 20, 41));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x46, 0x00, 0xff), 21, 42));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x4e, 0x00, 0xc0), 22, 43));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x3d, 0x00, 0x80), 23, 44));

	y += dy; // row 5
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x00, 0x80, 0x27), 23, 46, XOFFSET(x, 2), y));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x00, 0xc0, 0x4b), 24, 47));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x00, 0xff, 0x80), 25, 48));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x57, 0xff, 0xc0), 26, 49));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xa0, 0xff, 0xff), 27, 50));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xa0, 0xc0, 0xff), 28, 51));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xa0, 0xa0, 0xff), 29, 52));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x80, 0x57, 0xff), 30, 53));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x80, 0x00, 0xff), 31, 54));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x80, 0x00, 0xc0), 32, 55));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x56, 0x00, 0x80), 33, 56));

	y += dy; // row 6
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x00, 0x80, 0x12), 33, 58, XOFFSET(x, 1), y));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x00, 0xc0, 0x24), 34, 59));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x00, 0xff, 0x46), 35, 60));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x57, 0xff, 0x80), 36, 61));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xa0, 0xff, 0xc0), 37, 62));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xcc, 0xff, 0xff), 38, 63));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xc3, 0xda, 0xed), 39, 64));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xc0, 0xa0, 0xff), 40, 65));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xc0, 0x57, 0xff), 41, 66));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xc0, 0x00, 0xff), 42, 67));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xa3, 0x00, 0xc0), 43, 68));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x6b, 0x00, 0x80), 44, 69));

	y += dy; // row 7
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x00, 0x80, 0x00), 44, 70, x, y));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x00, 0xc0, 0x00), 45, 71));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x00, 0xff, 0x00), 46, 72));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x57, 0xff, 0x57), 47, 73));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xa0, 0xff, 0xa0), 48, 74));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xcc, 0xff, 0xcc), 49, 75));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xff, 0xff, 0xff), 50, 76));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xff, 0xcc, 0xff), 51, 77));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xff, 0xa0, 0xff), 52, 78));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xff, 0x57, 0xff), 53, 79));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xff, 0x00, 0xff), 54, 80));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xc0, 0x00, 0xc0), 55, 81));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x80, 0x00, 0x80), 56, 82));

	y += dy; // row 8
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x12, 0x80, 0x00), 57, 82, XOFFSET(x, 1), y));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x24, 0xc0, 0x00), 58, 83));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x46, 0xff, 0x00), 59, 84));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x80, 0xff, 0x57), 60, 85));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xc0, 0xff, 0xa0), 61, 86));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xff, 0xff, 0xcc), 62, 87));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xff, 0xcc, 0xcc), 63, 88));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xff, 0xa0, 0xc0), 64, 89));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xff, 0x57, 0xc0), 65, 90));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xff, 0x00, 0xc0), 66, 91));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xc0, 0x00, 0x9d), 67, 92));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x80, 0x00, 0x67), 68, 93));

	y += dy; // row 9
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x27, 0x80, 0x00), 70, 93, XOFFSET(x, 2), y));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x4e, 0xc0, 0x00), 71, 94));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x80, 0xff, 0x00), 72, 95));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xc0, 0xff, 0x57), 73, 96));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xff, 0xff, 0xa0), 74, 97));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xff, 0xc0, 0xa0), 75, 98));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xff, 0xa0, 0xa0), 76, 99));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xff, 0x57, 0x80), 77, 100));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xff, 0x00, 0x80), 78, 101));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xc0, 0x00, 0x80), 79, 102));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x80, 0x00, 0x52), 80, 103));

	y += dy; // row 10
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x3d, 0x80, 0x00), 82, 103, XOFFSET(x, 3), y));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x80, 0xc0, 0x00), 83, 104));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xc0, 0xff, 0x00), 84, 105));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xff, 0xff, 0x57), 85, 106));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xff, 0xc0, 0x57), 86, 107));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xff, 0x80, 0x57), 87, 108));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xff, 0x57, 0x57), 88, 109));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xff, 0x00, 0x42), 89, 110));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xc0, 0x00, 0x4b), 90, 111));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x80, 0x00, 0x3b), 91, 112));

	y += dy; // row 11
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x56, 0x80, 0x00), 93, 112, XOFFSET(x, 4), y));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xa3, 0xc0, 0x00), 94, 113));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xff, 0xff, 0x00), 95, 114));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xff, 0xc0, 0x00), 96, 115));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xff, 0x80, 0x00), 97, 116));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xff, 0x42, 0x00), 98, 117));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xff, 0x00, 0x00), 99, 118));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xc0, 0x00, 0x20), 100, 119));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x80, 0x00, 0x25), 101, 120));

	y += dy; // row 12
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x6b, 0x80, 0x00), 103, 120, XOFFSET(x, 5), y));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xc0, 0xc0, 0x00), 104, 121));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xc0, 0x9d, 0x00), 105, 122));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xc0, 0x80, 0x00), 106, 123));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xc0, 0x4b, 0x00), 107, 124));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xc0, 0x20, 0x00), 108, 125));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xc0, 0x00, 0x00), 109, 126));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x80, 0x00, 0x10), 110, 142));

	y += dy; // row 13
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x80, 0x80, 0x00), 112, 121, XOFFSET(x, 6), y));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x80, 0x67, 0x00), 113, 122));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x80, 0x52, 0x00), 114, 123));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x80, 0x3b, 0x00), 115, 124));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x80, 0x25, 0x00), 116, 125));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x80, 0x10, 0x00), 117, 126));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x80, 0x00, 0x00), 118, 127));

	y += dy * 2; // large white cell
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xff, 0xff, 0xff), 126, 128, x, y, TRUE));

	float sx = ctxData.sm.width;
	float sy1 = y + ctxData.sm.roof;
	float sy2 = y + ctxData.sm.height;
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xf8, 0xf8, 0xf8), 127, 129, XOFFSET(x, 5), sy1));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xea, 0xea, 0xea), 128, 130, XOFFSET(x, 6), sy2));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xdd, 0xdd, 0xdd), 129, 131, XOFFSET(x, 5) + sx, sy1));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xc0, 0xc0, 0xc0), 130, 132, XOFFSET(x, 6) + sx, sy2));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0xb2, 0xb2, 0xb2), 131, 133, XOFFSET(x, 5) + sx * 2, sy1));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x96, 0x96, 0x96), 132, 134, XOFFSET(x, 6) + sx * 2, sy2));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x80, 0x80, 0x80), 133, 135, XOFFSET(x, 5) + sx * 3, sy1));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x77, 0x77, 0x77), 134, 136, XOFFSET(x, 6) + sx * 3, sy2));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x5f, 0x5f, 0x5f), 135, 137, XOFFSET(x, 5) + sx * 4, sy1));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x4d, 0x4d, 0x4d), 136, 138, XOFFSET(x, 6) + sx * 4, sy2));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x33, 0x33, 0x33), 137, 139, XOFFSET(x, 5) + sx * 5, sy1));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x29, 0x29, 0x29), 138, 140, XOFFSET(x, 6) + sx * 5, sy2));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x1c, 0x1c, 0x1c), 139, 141, XOFFSET(x, 5) + sx * 6, sy1));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x11, 0x11, 0x11), 140, 142, XOFFSET(x, 6) + sx * 6, sy2));
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x08, 0x08, 0x08), 141, 143, XOFFSET(x, 5) + sx * 7, sy1));

	// large black cell
	m_rgnList.AddTail(new CRgnCell(ctxData, rect, RGB(0x00, 0x00, 0x00), 142, -1, XOFFSET(x, 22), y, TRUE));

	// the color can be set using SetSelectedColor(color) before the
	// window is initialized, so check here and set the active color
	// pointer as well.

	if (m_clrColor != (COLORREF)-1)
	{
		if (!m_pCell)
		{
			m_pCell = GetColorCell(m_clrColor);
		}
	}
}

BOOL CXTPColorHex::UpdateSelection(CPoint point, BOOL bNotify /*=TRUE*/)
{
	for (POSITION pos = m_rgnList.GetHeadPosition(); pos;)
	{
		CRgnCell* pCell = m_rgnList.GetNext(pos);
		if (pCell && pCell->PtInRegion(point))
		{
			return UpdateSelection(pCell, bNotify);
		}
	}
	return FALSE;
}

BOOL CXTPColorHex::UpdateSelection(CRgnCell* pCell, BOOL bNotify /*=TRUE*/)
{
	if (pCell)
	{
		COLORREF color = pCell->m_color;

		if (m_clrColor != color)
		{
			m_pCell = pCell;
			m_clrColor = color;

			CWnd* pWnd = GetOwner();
			if (pWnd && bNotify)
			{
				pWnd->SendMessage(WM_XTP_UPDATECOLOR,
					(WPARAM)m_clrColor, (LPARAM)m_hWnd);
			}
		}
	}
	else
	{
		m_clrColor = (COLORREF)-1;
		m_pCell = NULL;
	}

	if (::IsWindow(m_hWnd))
	{
		RedrawWindow();
	}

	return (m_pCell != NULL);
}

void CXTPColorHex::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (GetFocus() != this)
		SetFocus();

	if (UpdateSelection(point))
	{
		SetCapture();
	}

	CStatic::OnLButtonDown(nFlags, point);
}

void CXTPColorHex::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (GetFocus() != this)
		SetFocus();

	if (UpdateSelection(point))
	{
		CWnd* pWnd = GetOwner();
		if (pWnd)
		{
			pWnd->SendMessage(WM_XTP_COLORDBLCLK,
				(WPARAM)m_clrColor, (LPARAM)MAKELPARAM(LOWORD(point.x),HIWORD(point.y)));
		}
	}

	CStatic::OnLButtonDblClk(nFlags, point);
}

COLORREF CXTPColorHex::ColorFromPoint(CPoint point)
{
	for (POSITION pos = m_rgnList.GetHeadPosition(); pos;)
	{
		CRgnCell* pCell = m_rgnList.GetNext(pos);
		if (pCell && pCell->PtInRegion(point))
		{
			return pCell->m_color;
		}
	}
	return (COLORREF)-1;
}

void CXTPColorHex::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (GetCapture() == this)
	{
		ReleaseCapture();
	}

	CStatic::OnLButtonUp(nFlags, point);
}

void CXTPColorHex::OnMouseMove(UINT nFlags, CPoint point)
{
	if (GetCapture() == this)
	{
		UpdateSelection(point);
	}

	CStatic::OnMouseMove(nFlags, point);
}

bool CXTPColorHex::Init()
{
	if (::IsWindow(m_hWnd))
	{
		// create each color cell and add it to our list.
		CreateCellArray();
		ModifyStyle(NULL, SS_NOTIFY);
		return true;
	}

	return false;
}

void CXTPColorHex::PreSubclassWindow()
{
	CStatic::PreSubclassWindow();

	if (m_bPreSubclassInit)
	{
		// Initialize the control.
		Init();
	}
}

int CXTPColorHex::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Initialize the control.
	Init();

	return 0;
}

BOOL CXTPColorHex::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CStatic::PreCreateWindow(cs))
		return FALSE;

	// When creating controls dynamically Init() must
	// be called from OnCreate() and not from
	// PreSubclassWindow().

	m_bPreSubclassInit = false;

	return TRUE;
}

CXTPColorHex::CRgnCell* CXTPColorHex::GetColorCell(int iIndex)
{
	POSITION pos = m_rgnList.FindIndex(iIndex);
	if (pos) {
		return m_rgnList.GetAt(pos);
	}
	return NULL;
}

CXTPColorHex::CRgnCell* CXTPColorHex::GetColorCell(COLORREF color)
{
	for (POSITION pos = m_rgnList.GetHeadPosition(); pos;)
	{
		CRgnCell* pCell = m_rgnList.GetNext(pos);
		if (pCell && pCell->m_color == color) {
			return pCell;
		}
	}
	return NULL;
}

BOOL CXTPColorHex::SetSelectedColor(int iIndex, BOOL bNotify /*=TRUE*/)
{
	if (iIndex != -1)
	{
		return UpdateSelection(GetColorCell(iIndex), bNotify);
	}

	return FALSE;
}

BOOL CXTPColorHex::SetSelectedColor(COLORREF color, BOOL bNotify /*=TRUE*/)
{
	if (!::IsWindow(m_hWnd))
	{
		m_clrColor = color;
		return TRUE;
	}

	if (color != (COLORREF)-1)
	{
		return UpdateSelection(GetColorCell(color), bNotify);
	}

	return FALSE;
}

BOOL CXTPColorHex::PreTranslateMessage(MSG* pMsg)
{
	switch (pMsg->message)
	{
	case WM_KEYDOWN:
		{
			if (m_pCell)
			{
				TCHAR vkey = (TCHAR)pMsg->wParam;
				switch (vkey)
				{
				case VK_LEFT:
				case VK_RIGHT:
					{
						POSITION pos = m_rgnList.Find(m_pCell);
						if (pos)
						{
							if (vkey == VK_LEFT)
							{
								m_rgnList.GetPrev(pos);
							}
							else
							{
								m_rgnList.GetNext(pos);
							}

							if (pos)
							{
								UpdateSelection(m_rgnList.GetAt(pos));
							}
						}
						return TRUE;
					}

				case VK_UP:
				case VK_DOWN:
					{
						SetSelectedColor(m_pCell->m_iCell[(vkey == VK_UP) ? 0 : 1]);
						return TRUE;
					}
				}
			}
		}
		break;
	}

	return CStatic::PreTranslateMessage(pMsg);
}

/////////////////////////////////////////////////////////////////////////////
// CXTPColorPageStandard

BEGIN_MESSAGE_MAP(CXTPColorPageStandard, CPropertyPage)
	//{{AFX_MSG_MAP(CXTPColorPageStandard)
	//ON_WM_PAINT()
	//ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_XTP_UPDATECOLOR, OnUpdateColor)
	ON_MESSAGE(WM_XTP_COLORDBLCLK, OnColorDblClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTPColorPageStandard construction/destruction

CXTPColorPageStandard::CXTPColorPageStandard(CXTPColorDialog* pParentSheet)
: CPropertyPage(CXTPColorPageStandard::IDD)
{
	m_psp.dwFlags &= ~PSP_HASHELP;
	m_pParentSheet = pParentSheet;

	//{{AFX_DATA_INIT(CXTPColorPageStandard)
	//}}AFX_DATA_INIT
}

CXTPColorPageStandard::~CXTPColorPageStandard()
{
}

void CXTPColorPageStandard::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXTPColorPageStandard)
	DDX_Control(pDX, XTP_IDC_CLR_HEX, m_colorHex);
	//}}AFX_DATA_MAP
}

LRESULT CXTPColorPageStandard::OnUpdateColor(WPARAM wParam, LPARAM lParam)
{
	if ((HWND)lParam == m_colorHex.m_hWnd)
	{
		m_pParentSheet->SetNewColor((COLORREF)wParam, FALSE);
	}
	else
	{
		m_colorHex.SetSelectedColor((COLORREF)wParam, FALSE);
	}

	return 0;
}

LRESULT CXTPColorPageStandard::OnColorDblClick(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	m_pParentSheet->EndDialog(IDOK);

	return 0;
}

BOOL CXTPColorPageStandard::OnSetActive()
{
	COLORREF color = m_pParentSheet->GetColor();
	m_colorHex.SetSelectedColor(color, FALSE);

	return CPropertyPage::OnSetActive();
}
