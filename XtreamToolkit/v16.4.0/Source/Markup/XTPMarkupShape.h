// XTPMarkupShape.h: interface for the CXTPMarkupShape class.
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
#if !defined(__XTPMARKUPSHAPE_H__)
#define __XTPMARKUPSHAPE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPMarkupBrush;
class CXTPMarkupStrokeStyle;


enum XTPMarkupStretch
{
	xtpMarkupStretchNone,
	xtpMarkupStretchFill,
	xtpMarkupStretchUniform,
	xtpMarkupStretchUniformToFill
};

class _XTP_EXT_CLASS CXTPMarkupShape : public CXTPMarkupFrameworkElement
{
	DECLARE_MARKUPCLASS(CXTPMarkupShape)

protected:
	CXTPMarkupShape();
	~CXTPMarkupShape();

public:
	void SetFill(CXTPMarkupBrush* brush);
	CXTPMarkupBrush* GetFill() const;

	void SetStroke(CXTPMarkupBrush* brush);
	CXTPMarkupBrush* GetStroke() const;

	void SetStrokeThickness(int nThickness);
	int GetStrokeThickness() const;

	XTPMarkupStretch GetStretch() const;

	CXTPMarkupDoubleCollection* GetStrokeDashArray() const;
	void SetStrokeDashArray(CXTPMarkupDoubleCollection* pStrokeDashArray);

	void GetStrokeStyle(CXTPMarkupStrokeStyle* pStrokeStyle) const;

	XTPMarkupLineCap GetStrokeStartLineCap() const;
	void SetStrokeStartLineCap(XTPMarkupLineCap nLineCap);

	XTPMarkupLineCap GetStrokeEndLineCap() const;
	void SetStrokeEndLineCap(XTPMarkupLineCap nLineCap);

	XTPMarkupLineJoin GetStrokeLineJoin() const;
	void SetStrokeLineJoin(XTPMarkupLineJoin nLineJoin);

	XTPMarkupSmoothingMode GetSmoothingMode() const;
	void SetSmoothingMode(XTPMarkupSmoothingMode smoothingMode);


public:
	static CXTPMarkupObject* AFX_CDECL ConvertLineCap(CXTPMarkupBuilder* /*pBuilder*/, CXTPMarkupObject* pObject);
	static CXTPMarkupObject* AFX_CDECL ConvertLineJoin(CXTPMarkupBuilder* /*pBuilder*/, CXTPMarkupObject* pObject);
	static CXTPMarkupObject* AFX_CDECL ConvertSmoothing(CXTPMarkupBuilder* /*pBuilder*/, CXTPMarkupObject* pObject);
#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
public:
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPMarkupShape);

	afx_msg LPDISPATCH OleGetFill();
	afx_msg void OleSetFill(LPDISPATCH lpBackground);
	afx_msg LPDISPATCH OleGetStroke();
	afx_msg void OleSetStroke(LPDISPATCH lpBackground);
	afx_msg int OleGetStrokeThickness();
	afx_msg void OleSetStrokeThickness(int lpThickness);
	afx_msg int OleGetStrokeStartLineCap();
	afx_msg void OleSetStrokeStartLineCap(int nValue);
	afx_msg int OleGetStrokeEndLineCap();
	afx_msg void OleSetStrokeEndLineCap(int nValue);
	afx_msg int OleGetStrokeLineJoin();
	afx_msg void OleSetStrokeLineJoin(int nValue);
	afx_msg LPDISPATCH OleGetStrokeDashArray();
	afx_msg void OleSetStrokeDashArray(LPDISPATCH lpValue);

//}}AFX_CODEJOCK_PRIVATE
#endif

public:
	static CXTPMarkupDependencyProperty *m_pStrokeProperty;
	static CXTPMarkupDependencyProperty *m_pFillProperty;
	static CXTPMarkupDependencyProperty *m_pStrokeThicknessProperty;
	static CXTPMarkupDependencyProperty *m_pStretchProperty;

	static CXTPMarkupDependencyProperty *m_pStrokeDashArrayProperty;
	static CXTPMarkupDependencyProperty *m_pStrokeStartLineCapProperty;
	static CXTPMarkupDependencyProperty *m_pStrokeEndLineCapProperty;
	static CXTPMarkupDependencyProperty *m_pStrokeLineJoinProperty;
	static CXTPMarkupDependencyProperty *m_pSmoothingMode;
};


class _XTP_EXT_CLASS CXTPMarkupPointCollection : public CXTPMarkupObject
{
	DECLARE_MARKUPCLASS(CXTPMarkupPointCollection)
public:
	typedef CArray<POINT, POINT&> CPointArray;
public:
	CXTPMarkupPointCollection();
	CXTPMarkupPointCollection(CPointArray& arr);

public:
	CRect GetBounds() const;
	const CPointArray& GetPoints() const;

	void Stretch(CPointArray& arr, CSize sz);

protected:
	CXTPMarkupObject* ConvertFrom(CXTPMarkupBuilder* pBuilder, CXTPMarkupObject* pObject) const;
	static BOOL AFX_CDECL ConvertFromString(LPCWSTR lpszValue, CPointArray& arr);
	static BOOL AFX_CDECL GetNextValue(LPCWSTR& lpszValue, int& nValue);

protected:
	CPointArray m_arrPoints;
	CRect m_rcBounds;
};

class _XTP_EXT_CLASS CXTPMarkupPolygon : public CXTPMarkupShape
{
	DECLARE_MARKUPCLASS(CXTPMarkupPolygon)

protected:
	CXTPMarkupPolygon();
	~CXTPMarkupPolygon();

public:
	void SetPoints(CXTPMarkupPointCollection* pPoints);
	CXTPMarkupPointCollection* GetPoints() const;

protected:
	CSize MeasureOverride(CXTPMarkupDrawingContext* pDC, CSize szAvailableSize);
	void OnRender(CXTPMarkupDrawingContext* pDC);

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
public:
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPMarkupPolygon);
//}}AFX_CODEJOCK_PRIVATE
#endif

public:
	static CXTPMarkupDependencyProperty* m_pPointsProperty;

};

class _XTP_EXT_CLASS CXTPMarkupPolyline : public CXTPMarkupShape
{
	DECLARE_MARKUPCLASS(CXTPMarkupPolyline)

protected:
	CXTPMarkupPolyline();
	~CXTPMarkupPolyline();

public:
	void SetPoints(CXTPMarkupPointCollection* pPoints);
	CXTPMarkupPointCollection* GetPoints() const;

protected:
	CSize MeasureOverride(CXTPMarkupDrawingContext* pDC, CSize szAvailableSize);
	void OnRender(CXTPMarkupDrawingContext* pDC);

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
public:
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPMarkupPolyline);
//}}AFX_CODEJOCK_PRIVATE
#endif

public:
	static CXTPMarkupDependencyProperty* m_pPointsProperty;

};

class _XTP_EXT_CLASS CXTPMarkupRectangle : public CXTPMarkupShape
{
	DECLARE_MARKUPCLASS(CXTPMarkupRectangle)

protected:
	CXTPMarkupRectangle();
	~CXTPMarkupRectangle();

public:
	int GetRadiusX() const;
	int GetRadiusY() const;

	void SetRadiusX(int nRarius);
	void SetRadiusY(int nRarius);


protected:
	CSize MeasureOverride(CXTPMarkupDrawingContext* pDC, CSize szAvailableSize);
	void OnRender(CXTPMarkupDrawingContext* pDC);

public:
	static CXTPMarkupDependencyProperty* m_pRadiusXProperty;
	static CXTPMarkupDependencyProperty* m_pRadiusYProperty;

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
public:
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPMarkupRectangle);

	afx_msg int OleGetRadiusX();
	afx_msg void OleSetRadiusX(int nValue);
	afx_msg int OleGetRadiusY();
	afx_msg void OleSetRadiusY(int nValue);

//}}AFX_CODEJOCK_PRIVATE
#endif
};

class _XTP_EXT_CLASS CXTPMarkupEllipse : public CXTPMarkupShape
{
	DECLARE_MARKUPCLASS(CXTPMarkupEllipse)

protected:
	CXTPMarkupEllipse();
	~CXTPMarkupEllipse();

protected:
	virtual CSize MeasureOverride(CXTPMarkupDrawingContext* pDC, CSize szAvailableSize);
	virtual void OnRender(CXTPMarkupDrawingContext* pDC);

	virtual CXTPMarkupInputElement* InputHitTestOverride(CPoint point) const;

public:
#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
public:
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPMarkupEllipse);
//}}AFX_CODEJOCK_PRIVATE
#endif
};

class _XTP_EXT_CLASS CXTPMarkupLine : public CXTPMarkupShape
{
	DECLARE_MARKUPCLASS(CXTPMarkupLine)

protected:
	CXTPMarkupLine();
	~CXTPMarkupLine();

public:
	int GetX1() const;
	int GetX2() const;
	int GetY1() const;
	int GetY2() const;

	void SetX1(int nValue);
	void SetX2(int nValue);
	void SetY1(int nValue);
	void SetY2(int nValue);

protected:
	virtual CSize MeasureOverride(CXTPMarkupDrawingContext* pDC, CSize szAvailableSize);
	virtual void OnRender(CXTPMarkupDrawingContext* pDC);
	virtual CXTPMarkupInputElement* InputHitTestOverride(CPoint point) const;

private:
	void GetPoints(int& x1, int& y1, int& x2, int& y2) const;

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
public:
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPMarkupLine);

	afx_msg int OleGetX1();
	afx_msg void OleSetX1(int nValue);
	afx_msg int OleGetX2();
	afx_msg void OleSetX2(int nValue);
	afx_msg int OleGetY1();
	afx_msg void OleSetY1(int nValue);
	afx_msg int OleGetY2();
	afx_msg void OleSetY2(int nValue);
//}}AFX_CODEJOCK_PRIVATE
#endif

public:
	static CXTPMarkupDependencyProperty* m_pX1Property;
	static CXTPMarkupDependencyProperty* m_pY1Property;
	static CXTPMarkupDependencyProperty* m_pX2Property;
	static CXTPMarkupDependencyProperty* m_pY2Property;
};

enum XTPMarkupPathPointType
{
	xtpMarkupPathPointTypeStart           = 0,    // move
	xtpMarkupPathPointTypeLine            = 1,    // line
	xtpMarkupPathPointTypeBezier          = 3,    // default Bezier (= cubic Bezier)
	xtpMarkupPathPointTypeCloseSubpath    = 0x80, // closed flag
};

struct MARKUP_POINTF
{
	float x;
	float y;
};

class CXTPMarkupPathData;

class _XTP_EXT_CLASS CXTPMarkupPathGeometryBuilder
{
public:
	CXTPMarkupPathGeometryBuilder();

public:
	void Parse(CXTPMarkupBuilder* pBuilder, LPCWSTR lpszValue, int nLength);
	CXTPMarkupPathData* CreateData() const;

public:
	void BeginFigure(float x, float y);
	void BezierTo(float x1, float y1, float x2, float y2, float x3, float y3); // draw bicubic   bezier curve
	void BezierTo(float x1, float y1, float x2, float y2);                     // draw quadratic bezier curve
	void LineTo(float x, float y);

	void ArcSegment(double xc, double yc, double th0, double th1, double rx, double ry, double xAxisRotation);
	void ArcTo(float x, float y, float rotationAngle, double xRadius, double yRadius, BOOL isLargeArcFlag, BOOL sweepDirectionFlag);

	void CloseFigure();

private:
	BOOL ReadToken();
	BOOL SkipWhiteSpace(BOOL allowComma);
	BOOL More() const;

	void ThrowBadToken();

	MARKUP_POINTF ReadSize(BOOL allowcomma);

	MARKUP_POINTF ReadPoint(WCHAR cmd, BOOL allowcomma);
	float ReadNumber(BOOL allowComma);
	BOOL IsNumber(BOOL allowComma);

	void EnsureFigure();
	MARKUP_POINTF Reflect();

private:
	LPCWSTR m_lpszValue;
	int m_nLength;
	int m_nIndex;
	BOOL m_bFigureStarted;
	int m_nFillRule;

	CArray<MARKUP_POINTF, MARKUP_POINTF&> m_arrPoints;
	CArray<BYTE, BYTE> m_arrTypes;
	WCHAR m_cToken;
	CXTPMarkupBuilder* m_pBuilder;

	MARKUP_POINTF m_secondLastPoint;
	MARKUP_POINTF m_lastPoint;
	MARKUP_POINTF m_lastStart;
};



AFX_INLINE CRect CXTPMarkupPointCollection::GetBounds() const {
	return m_rcBounds;
}
AFX_INLINE const CXTPMarkupPointCollection::CPointArray& CXTPMarkupPointCollection::GetPoints() const {
	return m_arrPoints;
}
AFX_INLINE CXTPMarkupDoubleCollection* CXTPMarkupShape::GetStrokeDashArray() const {
	return MARKUP_STATICCAST(CXTPMarkupDoubleCollection, GetValue(m_pStrokeDashArrayProperty));
}
AFX_INLINE void CXTPMarkupShape::SetStrokeDashArray(CXTPMarkupDoubleCollection* pStrokeDashArray) {
	SetValue(m_pStrokeDashArrayProperty, pStrokeDashArray);
}
AFX_INLINE void CXTPMarkupShape::SetStrokeThickness(int nThickness) {
	SetValue(m_pStrokeThicknessProperty, new CXTPMarkupInt(nThickness));
}
AFX_INLINE int CXTPMarkupShape::GetStrokeThickness() const {
	CXTPMarkupInt* pValue =  MARKUP_STATICCAST(CXTPMarkupInt, GetValue(m_pStrokeThicknessProperty));
	return pValue != NULL ? (int)*pValue : 1;
}
AFX_INLINE XTPMarkupStretch CXTPMarkupShape::GetStretch() const {
	CXTPMarkupEnum* pValue =  MARKUP_STATICCAST(CXTPMarkupEnum, GetValue(m_pStretchProperty));
	return pValue != NULL ? (XTPMarkupStretch)(int)*pValue : xtpMarkupStretchNone;
}
AFX_INLINE int CXTPMarkupRectangle::GetRadiusX() const {
	CXTPMarkupInt* pValue =  MARKUP_STATICCAST(CXTPMarkupInt, GetValue(m_pRadiusXProperty));
	return pValue ? (int)*pValue : 0;
}
AFX_INLINE int CXTPMarkupRectangle::GetRadiusY() const {
	CXTPMarkupInt* pValue =  MARKUP_STATICCAST(CXTPMarkupInt, GetValue(m_pRadiusYProperty));
	return pValue ? (int)*pValue : 0;
}
AFX_INLINE void CXTPMarkupRectangle::SetRadiusX(int nRarius) {
	SetValue(m_pRadiusXProperty, new CXTPMarkupInt(nRarius));
}
AFX_INLINE void CXTPMarkupRectangle::SetRadiusY(int nRarius) {
	SetValue(m_pRadiusYProperty, new CXTPMarkupInt(nRarius));
}
AFX_INLINE void CXTPMarkupLine::SetX1(int nValue) {
	SetValue(m_pX1Property, new CXTPMarkupInt(nValue));
}

AFX_INLINE void CXTPMarkupLine::SetX2(int nValue)
{
	SetValue(m_pX2Property, new CXTPMarkupInt(nValue));
}

AFX_INLINE void CXTPMarkupLine::SetY1(int nValue)
{
	SetValue(m_pY1Property, new CXTPMarkupInt(nValue));
}

AFX_INLINE void CXTPMarkupLine::SetY2(int nValue)
{
	SetValue(m_pY2Property, new CXTPMarkupInt(nValue));
}

AFX_INLINE XTPMarkupLineCap CXTPMarkupShape::GetStrokeStartLineCap() const
{
	CXTPMarkupEnum* pValue =  MARKUP_STATICCAST(CXTPMarkupEnum, GetValue(m_pStrokeStartLineCapProperty));
	return (XTPMarkupLineCap)(int)*pValue;
}

AFX_INLINE XTPMarkupLineCap CXTPMarkupShape::GetStrokeEndLineCap() const
{
	CXTPMarkupEnum* pValue =  MARKUP_STATICCAST(CXTPMarkupEnum, GetValue(m_pStrokeEndLineCapProperty));
	return (XTPMarkupLineCap)(int)*pValue;
}

AFX_INLINE XTPMarkupLineJoin CXTPMarkupShape::GetStrokeLineJoin() const
{
	CXTPMarkupEnum* pValue =  MARKUP_STATICCAST(CXTPMarkupEnum, GetValue(m_pStrokeLineJoinProperty));
	return (XTPMarkupLineJoin)(int)*pValue;
}

AFX_INLINE void CXTPMarkupShape::SetStrokeLineJoin(XTPMarkupLineJoin nValue)
{
	SetValue(m_pStrokeLineJoinProperty, CXTPMarkupEnum::CreateValue(nValue));
}

// Smoothing mode

AFX_INLINE XTPMarkupSmoothingMode CXTPMarkupShape::GetSmoothingMode() const
{
	CXTPMarkupEnum* pValue =  MARKUP_STATICCAST(CXTPMarkupEnum, GetValue(m_pSmoothingMode));
	return XTPMarkupSmoothingMode(int(*pValue));
}

AFX_INLINE void CXTPMarkupShape::SetSmoothingMode(XTPMarkupSmoothingMode smoothingMode)
{
	SetValue(m_pSmoothingMode, CXTPMarkupEnum::CreateValue(smoothingMode));
}

AFX_INLINE void CXTPMarkupShape::SetStrokeStartLineCap(XTPMarkupLineCap nValue)
{
	SetValue(m_pStrokeStartLineCapProperty, CXTPMarkupEnum::CreateValue(nValue));
}

AFX_INLINE void CXTPMarkupShape::SetStrokeEndLineCap(XTPMarkupLineCap nValue)
{
	SetValue(m_pStrokeEndLineCapProperty, CXTPMarkupEnum::CreateValue(nValue));
}


#endif // !defined(__XTPMARKUPSHAPE_H__)
