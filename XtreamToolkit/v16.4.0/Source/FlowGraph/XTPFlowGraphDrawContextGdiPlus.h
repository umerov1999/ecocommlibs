// XTPFlowGraphDrawContextGdiPlus.h: interface for the CXTPFlowGraphDrawContextGdiPlus class.
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
#if !defined(__XTPFLOWGRAPHDRAWCONTEXTGDIPLUS_H__)
#define __XTPFLOWGRAPHDRAWCONTEXTGDIPLUS_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPFlowGraphControl;
class CXTPFlowGraphImage;


namespace Gdiplus
{
	class Brush;
	class Pen;
	class Graphics;
	class GraphicsPath;
};


class _XTP_EXT_CLASS CXTPFlowGraphDrawContextGdiPlus : public CXTPFlowGraphDrawContext
{
	class CGdiPlus;
public:
	// --------------------------------------------------------
	// Summary:
	//     Constructs a CXTPFlowGraphDrawContextGdiPlus object.
	// --------------------------------------------------------
	CXTPFlowGraphDrawContextGdiPlus(HDC hDC, HDC hAttribDC = NULL, BOOL bPrinting = FALSE);
	// ----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPFlowGraphDrawContextGdiPlus object, handles cleanup
	//     and deallocation.
	// ----------------------------------------------------------------------
	virtual ~CXTPFlowGraphDrawContextGdiPlus();

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to Intialize/Uninitialize the GDI+ library, and
	//     keep track of the usage count.
	// Parameters:
	//     bInit - TRUE to initialize the GDI+ library, FALSE to uninitialized
	//             Also a TRUE value will increase the usage count and FALSE will
	//             decrease the usage count.
	// Remarks
	//     This is a static function.
	//-------------------------------------------------------------------------
	static void AFX_CDECL Register(BOOL bInit);

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the GDI+ graphics class.
	// Returns:
	//     A pointer to GpGraphics object.
	//-------------------------------------------------------------------------
	virtual Gdiplus::Graphics* GetGraphics() const;

public:
	virtual CSize MeasureMarkupElement(CXTPMarkupUIElement* pMarkupUIElement);
	virtual void DrawMarkupElement(CXTPMarkupUIElement* pMarkupUIElement, LPCRECT lpRect, UINT uFormat);


	virtual CSize MeasureString(const CString& str);

	virtual void SetClipRect(CRect rc);
	virtual CRect GetClipRect() const;

	virtual void DrawGrid(CRect rc, CPoint ptScrollOffset, double dZoomLevel, COLORREF clrGridColor);

	virtual void SetSmoothingMode(int nMode);

public:

	virtual void FillSolidRect(LPCRECT lpRect, COLORREF clr);
	virtual void FillSolidRect(int x, int y, int cx, int cy, COLORREF clr);

	virtual void GradientFill(CRect rc, COLORREF clrFrom, COLORREF clrTo, BOOL bHoriz);

	virtual void Draw3dRect(int x, int y, int cx, int cy, COLORREF clrTopLeft, COLORREF clrBottomRight);
	virtual void Draw3dRect(LPCRECT lpRect, COLORREF clrTopLeft, COLORREF clrBottomRight);

	virtual void DrawFrame(int x, int y, int cx, int cy, int sz, COLORREF clr);

	virtual void DrawText(LPCTSTR lpszText, LPCRECT lpRect, UINT uFormat);

	virtual void DrawCurve(Gdiplus::GraphicsPath* pPath);

	virtual void Clear(COLORREF clr);

	virtual void Line(float x1, float y1, float x2, float y2);

	virtual void Ellipse(LPCRECT lpRect);

	virtual void SetWorldTransform(CPoint ptOffset, double dZoomLevel);

	virtual void SetBrush(COLORREF clr);
	virtual void SetPen(COLORREF clrPen, int nWidth, BOOL bDashed = FALSE);
	virtual void SetFont(LOGFONT* lf);
	virtual void SetTextColor(COLORREF clr);

	virtual void DrawImage(CXTPFlowGraphImage* pImage, CRect rc);
	virtual void FillPolygon(const POINT* pts, int nCount);

public:
	Gdiplus::Brush* CreateBrush(COLORREF clr);
	static DWORD AFX_CDECL ColorRefToARGB(COLORREF clr);

#ifdef _XTP_DEMOMODE
	virtual void CXTPFlowGraphDrawContextGdiPlus::DrawWatermarkBackground(CRect rc);
#endif

protected:

	Gdiplus::Brush* m_pActiveBrush;
	Gdiplus::Brush* m_pActiveTextBrush;
	Gdiplus::Pen* m_pActivePen;
	Gdiplus::Font* m_pActiveFont;
	LOGFONT m_lfActiveFont;
	COLORREF m_clrTextColor;
	Gdiplus::Graphics* m_pGraphics;


	struct BRUSHREC
	{
		COLORREF clr;
		Gdiplus::Brush* pBrush;
	};

	enum
	{
		MAX_BRUSH = 6
	};

	int m_nBrushCount;
	BRUSHREC m_arrBrushes[MAX_BRUSH];
};

AFX_INLINE Gdiplus::Graphics* CXTPFlowGraphDrawContextGdiPlus::GetGraphics() const {
	return m_pGraphics;
}


#endif //#if !defined(__XTPFLOWGRAPHDRAWCONTEXTGDIPLUS_H__)
