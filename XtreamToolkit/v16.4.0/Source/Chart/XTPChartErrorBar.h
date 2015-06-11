// XTPChartErrorBar.h
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
#if !defined(__XTPCHARTERRORBAR_H__)
#define __XTPCHARTERRORBAR_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CXTPChartLineStyle;


class _XTP_EXT_CLASS CXTPChartErrorBar : public CXTPChartElement
{
	DECLARE_SERIAL(CXTPChartErrorBar)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartErrorBar object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartErrorBar();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartErrorBar object, handles cleanup
	//-------------------------------------------------------------------------
	virtual ~CXTPChartErrorBar();

public:
	virtual CXTPChartElementView* CreateView(CXTPChartDeviceContext* pDC, CXTPChartSeriesPointView* pPointView, CXTPChartElementView* pParentView);

	BOOL IsVisible() const;

	void SetVisible(BOOL bVisible);

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------
	XTPChartErrorBarEndStyle GetEndStyle() const;

	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------
	void SetEndStyle(XTPChartErrorBarEndStyle endStyle);

	XTPChartErrorBarType GetType() const;

	void SetType(XTPChartErrorBarType type);

	XTPChartErrorBarDirection GetDirection() const;

	void SetDirection(XTPChartErrorBarDirection direction);

	double GetAmount() const;

	void SetAmount(double dAmount);

	double GetValuePlus() const;

	void SetValuePlus(double dValuePlus);

	double GetValueMinus() const;

	void SetValueMinus(double dValueMinus);

	CXTPChartLineStyle* GetLineStyle() const;

	CXTPChartColor GetLineColor() const;

public:

	XTPChartErrorBarPosition GetPosition() const;

private:

	BOOL                       m_bVisible;
	XTPChartErrorBarEndStyle   m_endStyle;
	XTPChartErrorBarType       m_type;
	XTPChartErrorBarDirection  m_direction;

	int                        m_nCapLength;

	double                     m_dAmount;
	double                     m_dValuePlus;   // For custom error bar
	double                     m_dValueMinus;  // For custom error bar

	CXTPChartLineStyle        *m_pLineStyle;
	CXTPChartColor             m_clrLine;

protected:
	friend class CXTPChartSeriesStyle;
	friend class CXTPChartErrorBarView;

#ifdef _XTP_ACTIVEX
public:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartErrorBar);
	LPDISPATCH OleGetLineStyle();
	OLE_COLOR OleGetLineColor();
	void OleSetLineColor(OLE_COLOR clr);
//}}AFX_CODEJOCK_PRIVATE
#endif
};

AFX_INLINE BOOL CXTPChartErrorBar::IsVisible() const
{
	return m_bVisible;
}

AFX_INLINE void CXTPChartErrorBar::SetVisible(BOOL bVisible)
{
	m_bVisible = bVisible;
	OnChartChanged();
}

AFX_INLINE XTPChartErrorBarType CXTPChartErrorBar::GetType() const
{
	return m_type;
}

AFX_INLINE void CXTPChartErrorBar::SetType(XTPChartErrorBarType type)
{
	m_type = type;
	OnChartChanged();
}

AFX_INLINE XTPChartErrorBarEndStyle CXTPChartErrorBar::GetEndStyle() const
{
	return m_endStyle;
}

AFX_INLINE void CXTPChartErrorBar::SetEndStyle(XTPChartErrorBarEndStyle endStyle)
{
	m_endStyle = endStyle;
	OnChartChanged();
}

AFX_INLINE XTPChartErrorBarDirection CXTPChartErrorBar::GetDirection() const
{
	return m_direction;
}

AFX_INLINE void CXTPChartErrorBar::SetDirection(XTPChartErrorBarDirection direction)
{
	m_direction = direction;
}

AFX_INLINE double CXTPChartErrorBar::GetAmount() const
{
	return m_dAmount;
}

AFX_INLINE void CXTPChartErrorBar::SetAmount(double dAmount)
{
	m_dAmount = dAmount;
}

AFX_INLINE double CXTPChartErrorBar::GetValuePlus() const
{
	return m_dValuePlus;
}

AFX_INLINE void CXTPChartErrorBar::SetValuePlus(double dValuePlus)
{
	m_dValuePlus = dValuePlus;
}

AFX_INLINE double CXTPChartErrorBar::GetValueMinus() const
{
	return m_dValueMinus;
}

AFX_INLINE void CXTPChartErrorBar::SetValueMinus(double dValueMinus)
{
	m_dValueMinus = dValueMinus;
}

AFX_INLINE CXTPChartLineStyle* CXTPChartErrorBar::GetLineStyle() const
{
	return m_pLineStyle;
}

AFX_INLINE CXTPChartColor CXTPChartErrorBar::GetLineColor() const
{
	return m_clrLine;
}



class _XTP_EXT_CLASS CXTPChartErrorBarView : public CXTPChartElementView
{
public:
	CXTPChartErrorBarView(CXTPChartErrorBar* pErrorBar, CXTPChartSeriesPointView* pPointView, CXTPChartElementView* pParentView);

	void GetValues(double &dCenter, double &dPlus, double &dMinus) const;

protected:

	virtual CXTPChartDeviceCommand* CreateDeviceCommand(CXTPChartDeviceContext *pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the anchor angle of the error bar.
	//-----------------------------------------------------------------------
	virtual double GetAnchorAngle(XTPChartErrorBarPosition position) const;

protected:

	CXTPChartErrorBar        *m_pErrorBar;
	CXTPChartSeriesPointView *m_pPointView;
};

#endif //#if !defined(__XTPCHARTERRORBAR_H__)
