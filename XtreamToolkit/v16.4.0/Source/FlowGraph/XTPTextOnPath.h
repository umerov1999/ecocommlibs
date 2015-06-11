// XTPTextOnPath.h: interface for the CXTPTextOnPath class.
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
#if !defined(__XTPTEXTONPATH_H__)
#define __XTPTEXTONPATH_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// an array that can be copied with a copy constructor (e.g. returned from a function/method)
template<typename TYPE, class ARG_TYPE = const TYPE&>
class CCArray : public CArray<TYPE, ARG_TYPE>
{
public:
	CCArray(const CCArray& rhs) { Copy(rhs); }
	CCArray() {}

	bool operator =(const CCArray& rhs) { Copy(rhs); return true; }
};

// -----------------------------------------------------------------------
//
// Summary:
//     GDI+ drawing of text on path
//
// -----------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPTextOnPath
{
	enum TextPathAlign { Left = 0, Center = 1, Right = 2};

	enum TextPathPosition { OverPath = 0, CenterPath = 1, UnderPath = 2 };

protected:

	void GetLinePoints(Gdiplus::PointF p1, Gdiplus::PointF p2, int stepWitdth, CCArray<Gdiplus::PointF> &ret);

	Gdiplus::SizeF MeasureString(const CString& str, Gdiplus::StringFormat *pFormat);

	static CCArray<Gdiplus::PointF> CleanPoints(CCArray<Gdiplus::PointF> points);
	static double GetAngle(Gdiplus::PointF point1, Gdiplus::PointF point2);

	void DrawSingleRotatedLetter(const CString &letter, Gdiplus::REAL angle, Gdiplus::PointF pointCenter);

	Gdiplus::REAL StringRegionWidth(int textpos);

	void DrawText(CCArray<Gdiplus::PointF> points, int maxPoints);

	int CalculatePathPoints(Gdiplus::PointF *pPathPoints, int maxCount);

	void DoTests();

public:

	void Draw();

	CXTPTextOnPath(const CString &text, Gdiplus::Graphics* pGraphics, Gdiplus::GraphicsPath *pPath);
	~CXTPTextOnPath();

	void SetFont(LOGFONT* lf);

	void ReversePath();

private:

	Gdiplus::Font *m_pFont;         // drawing font
	Gdiplus::Graphics *m_pGraphics; // GDI+ graphics object

	Gdiplus::GraphicsPath *m_pPath;
	Gdiplus::PathData m_Pathdata;

	CString m_sText; // the actual text

	TextPathPosition m_TextPathPathPosition;
	TextPathAlign    m_pathalign;

	Gdiplus::Brush *m_pFillBrush;

	int m_LetterSpacingPercentage; // 100 by default

	//////////////////////////////////////////////////////////////////////////

	CCArray<Gdiplus::PointF> m_TmpPointsReserved;

	Gdiplus::CharacterRange m_characterRanges[2];
	Gdiplus::Region         m_stringRegions[2];

	Gdiplus::SizeF m_TextSize; // should be calculated once

#ifdef _TEXTONPATH_TESTING

	//////////////////////////////////////////////////////////////////////////
	// testing mode
	//////////////////////////////////////////////////////////////////////////

	CCArray<Gdiplus::RectF>         _regionList;
	CCArray<float>                  _angles;
	CCArray<Gdiplus::PointF>        _pointText;
	CCArray<Gdiplus::PointF>        _pointTextUp;

#endif

};

#endif // __XTPTEXTONPATH_H__
