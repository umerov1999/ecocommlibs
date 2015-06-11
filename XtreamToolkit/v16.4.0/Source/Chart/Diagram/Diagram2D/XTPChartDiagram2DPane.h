// XTPChartDiagram2DPane.h
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
#if !defined(__XTPCHARTDIAGRAM2DPANE_H__)
#define __XTPCHARTDIAGRAM2DPANE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartAxis;
class CXTPChartDiagram2D;
class CXTPChartElementView;
class CXTPChartDeviceContext;
class CXTPChartDeviceCommand;
class CXTPChartAxisView;
class CXTPChartFillStyle;
class CXTPChartDiagram2DAppearance;
class CXTPChartAxisView;
class CXTPChartDiagram2DAxisView;
class CXTPChartBoundElementView;
class CXTPChartSeriesView;


namespace Gdiplus
{
	class Matrix;
};


class _XTP_EXT_CLASS CXTPChartDiagram2DPane : public CXTPChartElement
{
	DECLARE_DYNAMIC(CXTPChartDiagram2DPane);

public:
	CXTPChartDiagram2DPane(CXTPChartDiagram* pDiagram);
	virtual ~CXTPChartDiagram2DPane();

public:
	virtual CXTPChartElementView* CreateView(CXTPChartDeviceContext* pDC, CXTPChartElementView* pParent);

	CXTPChartDiagram2D* GetDiagram() const;

public:
	CXTPChartColor GetBackgroundColor() const;
	CXTPChartColor GetBackgroundColor2() const;
	CXTPChartColor GetBorderColor() const;

	CXTPChartColor GetActualBackgroundColor() const;
	CXTPChartColor GetActualBackgroundColor2() const;
	CXTPChartColor GetActualBorderColor() const;

	void SetBackgroundColor(const CXTPChartColor& color);
	void SetBackgroundColor2(const CXTPChartColor& color);
	void SetBorderColor(const CXTPChartColor& color);


	CXTPChartFillStyle* GetFillStyle() const;

public:
	void DoPropExchange(CXTPPropExchange* pPX);

public:

	CXTPChartDiagram2DAppearance* GetAppearance() const;

	CXTPChartFillStyle* GetActualFillStyle() const;


protected:
	CXTPChartColor m_clrBackgroundColor;
	CXTPChartColor m_clrBackgroundColor2;
	CXTPChartColor m_clrBorderColor;
	CXTPChartFillStyle* m_pBackgroundFillStyle;
};



class _XTP_EXT_CLASS CXTPChartDiagram2DPaneView : public CXTPChartElementView
{
public:
	CXTPChartDiagram2DPaneView(CXTPChartDiagram2DPane* pPane, CXTPChartElementView* pParent);
	~CXTPChartDiagram2DPaneView();

public:
	CRect GetBounds() const;

	CXTPChartPointF GetScreenPoint(const CXTPChartSeriesView* pView, double x, double y) const;

	void CalculateView(CRect rcBounds);

public:
	CXTPChartAxisView* GetAxisView(CXTPChartAxis* pAxis) const;

	CXTPChartElementView* CreateSeriesView();

protected:
	virtual CXTPChartDeviceCommand* CreateDeviceCommand(CXTPChartDeviceContext* pDC);

	virtual CXTPChartDeviceCommand* CreateGridLinesDeviceCommand(CXTPChartDeviceContext* pDC, CXTPChartDiagram2DAxisView* pAxis);
	virtual CXTPChartDeviceCommand* CreateInterlacedDeviceCommand(CXTPChartDeviceContext* pDC, CXTPChartDiagram2DAxisView* pAxis);
	virtual CXTPChartDeviceCommand* CreateConstantLinesDeviceCommand(CXTPChartDeviceContext* pDC, CXTPChartDiagram2DAxisView* pAxis, BOOL bBehind);
	virtual CXTPChartDeviceCommand* CreateStripsDeviceCommand(CXTPChartDeviceContext* pDC, CXTPChartDiagram2DAxisView* pAxis);

public:
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL OnSetCursor(CPoint point);

public:
	CXTPChartDiagram2DPane* m_pPane;
	CXTPChartElementView* m_pSeriesView;
	CRect m_rcBounds;
	CPoint m_ptOldPosition;
};

AFX_INLINE CXTPChartDiagram2D* CXTPChartDiagram2DPane::GetDiagram() const {
	return (CXTPChartDiagram2D*)m_pOwner;
}
AFX_INLINE CRect CXTPChartDiagram2DPaneView::GetBounds() const {
	return m_rcBounds;
}
AFX_INLINE CXTPChartFillStyle* CXTPChartDiagram2DPane::GetFillStyle() const {
	return m_pBackgroundFillStyle;
}
AFX_INLINE void CXTPChartDiagram2DPane::SetBackgroundColor(const CXTPChartColor& color) {
	m_clrBackgroundColor = color;
	OnChartChanged();
}
AFX_INLINE void CXTPChartDiagram2DPane::SetBackgroundColor2(const CXTPChartColor& color) {
	m_clrBackgroundColor2 = color;
	OnChartChanged();
}
AFX_INLINE void CXTPChartDiagram2DPane::SetBorderColor(const CXTPChartColor& color) {
	m_clrBorderColor = color;
	OnChartChanged();
}


#endif //#if !defined(__XTPCHARTDIAGRAM2DPANE_H__)
