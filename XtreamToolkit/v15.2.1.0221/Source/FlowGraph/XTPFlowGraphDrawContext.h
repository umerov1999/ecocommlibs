// XTPFlowGraphDrawContext.h: interface for the CXTPFlowGraphDrawContext class.
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
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

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPFLOWGRAPHDRAWCONTEXT_H__)
#define __XTPFLOWGRAPHDRAWCONTEXT_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPFlowGraphControl;
class CXTPFlowGraphImage;
class CXTPMarkupUIElement;


namespace Gdiplus
{
	class GraphicsPath;
};

class _XTP_EXT_CLASS CXTPFlowGraphDrawContext
{
public:
	// -------------------------------------------------
	// Summary:
	//     Constructs a CXTPFlowGraphDrawContext object.
	// -------------------------------------------------
	CXTPFlowGraphDrawContext(HDC hDC, HDC hAttribDC = NULL, BOOL bPrinting = FALSE);
	// -------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPFlowGraphDrawContext object, handles cleanup and
	//     deallocation.
	// -------------------------------------------------------------------
	virtual ~CXTPFlowGraphDrawContext();

public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the windows device context.
	// Returns:
	//     A handle to the windows device context.
	//-------------------------------------------------------------------------
	HDC GetHDC() const;

	BOOL IsPrinting() const;
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
	virtual void SetPen(COLORREF clrPen, int nWidth);
	virtual void SetFont(LOGFONT* lf);
	virtual void SetTextColor(COLORREF clr);

	virtual void DrawImage(CXTPFlowGraphImage* pImage, CRect rc);
	virtual void DrawSelectionRect(CRect rc);

	virtual void FillPolygon(const POINT* pts, int nCount);

#ifdef _XTP_DEMOMODE
	virtual void DrawWatermarkBackground(CRect rc);
#endif

public:

protected:
	HDC m_hDC;                  //The windows device context.
	BOOL m_bPrinting;

	HBRUSH m_hActiveBrush;
	HPEN m_hActivePen;
	HFONT m_hActiveFont;

	CRect m_rcClipRect;

	CXTPFlowGraphGDIHandles<CXTPFlowGraphFontTraits> m_arrFonts;
	CXTPFlowGraphGDIHandles<CXTPFlowGraphBrushTraits> m_arrBrushes;

};

AFX_INLINE HDC CXTPFlowGraphDrawContext::GetHDC() const {
	return m_hDC;
}
AFX_INLINE BOOL CXTPFlowGraphDrawContext::IsPrinting() const {
	return m_bPrinting;
}

#endif //#if !defined(__XTPFLOWGRAPHDRAWCONTEXT_H__)
