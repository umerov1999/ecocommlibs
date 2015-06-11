// XTPMarkupDeviceContext.h: interface for the CXTPMarkupDeviceContext class.
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

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPMARKUPDEVICECONTEXT_H__)
#define __XTPMARKUPDEVICECONTEXT_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace Gdiplus
{
	class GpGraphics;
	class GpBrush;
	class GpFont;
	class GpPen;
	class GpStringFormat;
	class GpBitmap;
};

class CXTPMarkupStrokeStyle;

class _XTP_EXT_CLASS CXTPMarkupDeviceContext
{
public:
	CXTPMarkupDeviceContext(HDC hDC, HDC hAttribDC);
	virtual ~CXTPMarkupDeviceContext();

public:
	virtual void OffsetViewport(const POINT& ptViewortOrg);
	virtual void SetSmoothingMode(XTPMarkupSmoothingMode smoothingMode);

	POINT TranslatePoint(const POINT& ptVisualOffset) const;

	HDC GetSafeHdc() const;

public:
	virtual void Ellipse(CRect rc, CXTPMarkupStrokeStyle* pStrokeStyle, CXTPMarkupBrush* pFillBrush);
	virtual void DrawRectangle(CRect rc, CXTPMarkupBrush* pBrush, CXTPMarkupThickness* pThickness);
	virtual void FillRectangle(CRect rc, CXTPMarkupBrush* pBrush);
	virtual void DrawLine(int x1, int y1, int x2, int y2, CXTPMarkupStrokeStyle* pStrokeStyle);
	virtual void Polyline(const POINT* points, int nCount, CXTPMarkupStrokeStyle* pStrokeStyle);
	virtual void Polygon(const POINT* points, int nCount, CXTPMarkupStrokeStyle* pStrokeStyle, CXTPMarkupBrush* pFillBrush);
	virtual void DrawPath(CXTPMarkupPathData *pData, CXTPMarkupStrokeStyle* pStrokeStyle, CXTPMarkupBrush* pFillBrush);

	virtual void DrawImage(CXTPImageManagerIcon* pIcon, CRect rc);

	virtual void FillRoundRectangle(CRect rc, CXTPMarkupBrush* pBrush, double* pCornerRadius);
	virtual void DrawRoundRectangle(CRect rc, CXTPMarkupBrush* pBrush, CXTPMarkupThickness* pThickness, double* pCornerRadius);


	virtual void DrawString(LPCWSTR lpszText, UINT nCount, LPCRECT lpRect);
	virtual SIZE MeasureString(LPCWSTR lpszText, int nCount) const;

	virtual void SetTextColor(CXTPMarkupBrush* pBrush);
	virtual void SetTextFont(CXTPMarkupFont* pFont);

public:
	virtual void IntersectClipRect(const RECT& rcLayoutClip);
	virtual void GetClipBox(LPRECT lpRect);

	virtual HRGN SaveClipRegion();
	virtual void RestoreClipRegion(HRGN hRgn);

public:
	virtual HDC GetDC();
	virtual void ReleaseDC(HDC hDC);


protected:
	BOOL IsSolidBrush(CXTPMarkupBrush* pBrush) const;
	BOOL IsLinearGradientBrush(CXTPMarkupBrush* pBrush) const;

private:
	CSize ComputeDeltas(int& x, LPCWSTR lpszString, UINT &nCount,
		BOOL bTabbed, UINT nTabStops, LPINT lpnTabStops, int nTabOrigin,
		LPWSTR lpszOutputString, int* pnDxWidths, int& nRightFixup) const;

	int ComputeNextTab(int x, UINT nTabStops, LPINT lpnTabStops, int nTabOrigin, int nTabWidth) const;

	HFONT SelectFontObject(HFONT hFont);
	void CreatePen(CPen& pen, CXTPMarkupStrokeStyle* pStrokeStyle);


protected:
	struct GRADIENTSTOP
	{
		RGBQUAD clr;
		double dOffset;
		double dDiff;
		int nIndex;
	};
	static int _cdecl _GradientStopCompare(const void *arg1, const void *arg2);

protected:
	HDC m_hDC;
	HDC m_hAttribDC;
	SIZE m_sizeWinExt;
	SIZE m_sizeVpExt;

	HFONT m_hOldFont;

};

AFX_INLINE HDC CXTPMarkupDeviceContext::GetSafeHdc() const
{
	return m_hDC;
}


#endif // !defined(__XTPMARKUPDEVICECONTEXT_H__)
