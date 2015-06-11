// XTPTextOnPath.cpp : implementation of the CXTPTextOnPath class.
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

#include "stdafx.h"

#define _USE_MATH_DEFINES // for M_PI
#include <math.h>

#ifndef M_PI
#define M_PI       3.14159265358979323846
#endif

#include "GraphicLibrary/GdiPlus/GdiPlus.h"

#include "Common/XTPDrawHelpers.h"
#include "Common/XTPResourceManager.h"

using namespace Gdiplus;

#include "XTPTextOnPath.h"

CXTPTextOnPath::CXTPTextOnPath(const CString &text, Gdiplus::Graphics* pGraphics, GraphicsPath *pPath)
	: m_pFont(NULL), m_pFillBrush(NULL), m_pGraphics(pGraphics), m_sText(text), m_pPath(NULL)
{
	m_TmpPointsReserved.SetSize(10001);

	m_characterRanges[0] = CharacterRange(0, 1);
	m_characterRanges[1] = CharacterRange(0, 0);

	m_TextSize = SizeF(0,0);

	m_pPath = pPath->Clone();

	m_pPath->GetPathData(&m_Pathdata);

	m_TextPathPathPosition = CXTPTextOnPath::OverPath;

	m_pathalign = CXTPTextOnPath::Center;

	m_LetterSpacingPercentage = 100;

	// set font
	m_pFont = new Font(FontFamily::GenericMonospace(), 11, FontStyleBold );

	m_pFillBrush = new SolidBrush((ARGB)Color::Black);
}

CXTPTextOnPath::~CXTPTextOnPath()
{
	SAFE_DELETE(m_pFont);
	SAFE_DELETE(m_pFillBrush);
	SAFE_DELETE(m_pPath);

	#ifdef _XTP_ACTIVEX // required at least for VB6, when running a program second time within the same IDE instance. The pointer becomes invalid (GenericMonospace caches pointer)

	if (GenericMonospaceFontFamily)
	{
		ZeroMemory(GenericMonospaceFontFamilyBuffer, sizeof(FontFamily));

		GenericMonospaceFontFamily = NULL;
	}

#endif
}

void CXTPTextOnPath::ReversePath()
{
	if (m_pPath)
	{
		m_pPath->Reverse();

		m_pPath->GetPathData(&m_Pathdata);
	}
}

void CXTPTextOnPath::SetFont(LOGFONT* lf)
{
	SAFE_DELETE(m_pFont);

	if (!lf)
		return;

	HDC hDC = m_pGraphics->GetHDC();

	ASSERT(hDC);

	if (hDC)
	{
		m_pFont = new Gdiplus::Font(0, lf);

		m_pGraphics->ReleaseHDC(hDC);
	}
}

SizeF CXTPTextOnPath::MeasureString(const CString& str, Gdiplus::StringFormat *pFormat)
{
	ASSERT(m_pFont);

	if (!m_pFont)
		return SizeF(0, 0);

	RectF boundingBox;
	RectF layoutRect(0, 0, 0.0f, 0.0f);

	m_pGraphics->MeasureString(XTP_CT2CW(str), -1, m_pFont, layoutRect, pFormat, &boundingBox);

	return SizeF(boundingBox.Width, boundingBox.Height);
}

void CXTPTextOnPath::GetLinePoints(PointF p1, PointF p2, int stepWitdth, CCArray<PointF> &ret)
{
	int lCount = 0;

	long ix;
	long iy;
	int dd;
	int id;
	int lStep = stepWitdth;

	/*p1.X = (int)p1.X;
	p1.Y = (int)p1.Y;
	p2.X = (int)p2.X;
	p2.Y = (int)p2.Y;*/

	long width = (long)(p2.X - p1.X);
	long height = (long)(p2.Y - p1.Y);
	long d = 0;

	if (width < 0)
	{
		width = -width;
		ix = -1;
	}
	else
	{
		ix = 1;
	}

	if (height < 0)
	{
		height = -height;
		iy = -1;
	}
	else
	{
		iy = 1;
	}

	if (!width && !height)
		return; // this check MUST be here for some steep paths

	if (width > height)
	{
		dd = (int)(width + width);
		id = (int)(height + height);

		do
		{
			if (lCount == m_TmpPointsReserved.GetSize()) // crash-safe check, just to be sure
			{
				ASSERT(FALSE);

				break;
			}

			if (lStep == stepWitdth)
			{
				m_TmpPointsReserved[lCount].X = p1.X;
				m_TmpPointsReserved[lCount].Y = p1.Y;

				lCount += 1;
			}
			else
			{
				lStep = lStep + stepWitdth;
			}
			if ((int)p1.X == (int)p2.X) break;

			p1.X = p1.X + ix;
			d = d + id;

			if (d > width)
			{
				p1.Y = p1.Y + iy;
				d = d - dd;
			}
		}

		while (true);
	}
	else
	{
		dd = (int)(height + height);
		id = (int)(width + width);

		do
		{
			if (lCount == m_TmpPointsReserved.GetSize()) // crash-safe check, just to be sure
			{
				ASSERT(FALSE);

				break;
			}

			if (lStep == stepWitdth)
			{
				m_TmpPointsReserved[lCount].X = p1.X;
				m_TmpPointsReserved[lCount].Y = p1.Y;

				lCount += 1;
			}
			else
			{
				lStep = lStep + stepWitdth;
			}
			if ((int)p1.Y == (int)p2.Y) break;

			p1.Y = p1.Y + iy;
			d = d + id;

			if (d > height)
			{
				p1.X = p1.X + ix;
				d = d - dd;
			}
		}
		while (true);
	}

	ret.SetSize(lCount);

	for (int i=0, c=lCount; i<c; i++)
		ret.SetAt(i, m_TmpPointsReserved[i]);
}

CCArray<PointF> CXTPTextOnPath::CleanPoints(CCArray<PointF> points)
{
	CCArray<PointF> tmppoints;
	tmppoints.SetSize(points.GetSize() + 1);

	PointF lastpoint(0,0);
	INT_PTR iCount = 0;

	for (INT_PTR i=0, c=points.GetSize(); i<c; i++)
	{
		// we count only points with different X or Y

		if ( (i == 0) || (points[i].X != lastpoint.X) || (points[i].Y != lastpoint.Y) )
		{
			tmppoints[iCount] = points[i];

			iCount++;
		}

		lastpoint = points[i];
	}

	CCArray<PointF> retpoints;
	retpoints.SetSize(iCount);
	tmppoints.SetSize(iCount);

	retpoints.Copy(tmppoints);

	return retpoints;
}

double CXTPTextOnPath::GetAngle(PointF point1, PointF point2)
{
	if (point1.Y == point2.Y)
		return 0.0; // optimization

	double c = sqrt(pow((point2.X - point1.X), 2) + pow((point2.Y - point1.Y), 2));

	if (c == 0)
		return 0;

	if (point1.X > point2.X)
	{
		// change the side of the triangle
		return asin((point1.Y - point2.Y) / c) * 180 / M_PI - 180;
	}

	return asin((point2.Y - point1.Y) / c) * 180 / M_PI;
}

void CXTPTextOnPath::DrawSingleRotatedLetter(const CString &letter, REAL angle, PointF pointCenter)
{
	if (!m_pFont)
		return;

	FontFamily activeFontFamily;
	m_pFont->GetFamily(&activeFontFamily);

	StringFormat stringFormat(StringFormat::GenericTypographic());
	stringFormat.SetLineAlignment(StringAlignmentCenter);

	GraphicsPath *pGraphicsPath = new GraphicsPath();
	REAL x = pointCenter.X;
	REAL y = pointCenter.Y;

	PointF pOrigin;

	switch (m_TextPathPathPosition)
	{
	case CXTPTextOnPath::OverPath:
		pOrigin = PointF(x, (y - m_pFont->GetSize()));
		break;
	case CXTPTextOnPath::CenterPath:
		pOrigin = PointF(x, (y - m_pFont->GetSize() / 2));
		break;
	case CXTPTextOnPath::UnderPath:
		pOrigin = PointF(x, y);
		break;
	}

	LPCWSTR szUnicodeNameLetter = NULL;

#ifdef _UNICODE
	szUnicodeNameLetter = letter;
#else
	CXTPResourceManager::CXTPA2W conversion(letter);
	szUnicodeNameLetter = conversion;
#endif

	pGraphicsPath->AddString(szUnicodeNameLetter, 1, &activeFontFamily, m_pFont->GetStyle(), m_pFont->GetSize(), pOrigin, &stringFormat);

	Matrix oldTransformationMatrix;
	m_pGraphics->GetTransform(&oldTransformationMatrix);

	Matrix *pNewRotationMatrix = oldTransformationMatrix.Clone();

	pNewRotationMatrix->RotateAt(angle, PointF(x, y), MatrixOrderPrepend); // MatrixOrderPrepend - !

	m_pGraphics->SetTransform(pNewRotationMatrix);

	m_pGraphics->FillPath(m_pFillBrush, pGraphicsPath);

	m_pGraphics->SetTransform(&oldTransformationMatrix);

#ifdef _TEXTONPATH_TESTING

	RectF bounds;
	pGraphicsPath->GetBounds(&bounds);

	_regionList.Add(bounds);
	_angles.Add(angle);

	_pointText.Add(pointCenter);
	_pointTextUp.Add(PointF(pOrigin.X, pOrigin.Y) );

#endif

	delete pNewRotationMatrix;
	delete pGraphicsPath;
}

REAL CXTPTextOnPath::StringRegionWidth(int textpos)
{
	ASSERT(m_pFont);

	if (!m_pFont)
		return 0;

	CString measureString = m_sText.Mid(textpos, 1);

	int numChars = measureString.GetLength();
	int numRanges = numChars + 1;

	ASSERT(numRanges == 2);

	if (numRanges != 2)
		return 0;

	StringFormat stringFormat(StringFormatFlagsNoClip | StringFormatFlagsNoWrap | StringFormatFlagsLineLimit);
	stringFormat.SetTrimming(StringTrimmingNone);

	if (m_TextSize.Empty())
		m_TextSize = MeasureString(m_sText, &stringFormat); // optimization

	RectF layoutRect(0, 0, m_TextSize.Width, m_TextSize.Height);

	stringFormat.SetFormatFlags(StringFormatFlagsNoClip);
	stringFormat.SetMeasurableCharacterRanges(numRanges, m_characterRanges);
	stringFormat.SetAlignment(StringAlignmentNear);

	CString midText = m_sText.Mid(textpos);
	LPCWSTR szUnicodeText = NULL;

#ifdef _UNICODE
	szUnicodeText = midText;
#else
	CXTPResourceManager::CXTPA2W conversion(midText);
	szUnicodeText = conversion;
#endif

	m_pGraphics->MeasureCharacterRanges(szUnicodeText, -1, m_pFont, layoutRect, &stringFormat, numRanges, m_stringRegions);

	RectF ret;
	m_stringRegions[0].GetBounds(&ret, m_pGraphics);

	return (m_LetterSpacingPercentage != 100) ? ret.Width * m_LetterSpacingPercentage / 100 : ret.Width; // one more optimization
}

void CXTPTextOnPath::DrawText(CCArray<PointF> points, int maxPoints)
{
#ifdef _TEXTONPATH_TESTING

	static Gdiplus::Color _pathColorTop = (ARGB)(Color::BlanchedAlmond);

	Pen *pTestpen = new Pen(_pathColorTop);

	for (int i=0; i<points.GetSize(); i++)
	{
		m_pGraphics->DrawEllipse(pTestpen, points[i].X, points[i].Y, (REAL)1, (REAL)1);
	}

	delete pTestpen;

#endif

	int iCount = 0;
	PointF point1(0,0);
	double dMaxWidthText = 0;

	for (int i = 0; i < m_sText.GetLength(); i++)
	{
		dMaxWidthText += StringRegionWidth(i);
	}

	switch (m_pathalign)
	{
	case CXTPTextOnPath::Left:
		point1 = points[0];
		iCount = 0;
		break;
	case CXTPTextOnPath::Center:
		iCount = (int)((maxPoints - dMaxWidthText) / 2);
		if (iCount > 0)
		{
			point1 = points[iCount];
		}
		else
		{
			point1 = points[0];
		}

		break;
	case CXTPTextOnPath::Right:
		iCount = (int)(maxPoints - dMaxWidthText - (double)StringRegionWidth(m_sText.GetLength() - 1));
		if (iCount > 0)
		{
			point1 = points[iCount];
		}
		else
		{
			point1 = points[0];
		}

		break;
	}

	//double previous_angle = 500.0;

	for (int charStep=0; charStep<m_sText.GetLength(); charStep++)
	{
		int lStrWidth = (int)(StringRegionWidth(charStep));

		if ((iCount + lStrWidth / 2) >= 0 && (iCount + lStrWidth) <= maxPoints)
		{
			iCount += lStrWidth;

			PointF point2 = points[iCount];
			PointF point = PointF((point2.X+point1.X)/2,(point2.Y+point1.Y)/2);

			double angle = GetAngle(point1, point2);

			//if (previous_angle != 500.0) // used for testing angles smoothing
			//{
			//  if (abs(previous_angle-angle) > 3.0)
			//  {
			//      //angle = previous_angle;
			//  }
			//}

			//previous_angle = angle;

			DrawSingleRotatedLetter(CString(m_sText[charStep]), (REAL)angle, point);

			point1 = points[iCount];
		}
		else
		{
			iCount += lStrWidth;
		}
	}
}

int CXTPTextOnPath::CalculatePathPoints(PointF *pPathPoints, int maxCount)
{
	int iPointsCount = 0;

	GraphicsPath *pGraphicPath = new GraphicsPath(m_Pathdata.Points, m_Pathdata.Types, m_Pathdata.Count, FillModeWinding);

	pGraphicPath->Flatten(NULL, 1);

	PathData pd;
	pGraphicPath->GetPathData(&pd);

	if (pd.Count)
	{
		BYTE *pTypes = new BYTE[pd.Count];
		pGraphicPath->GetPathTypes(pTypes, pd.Count);

		PointF tmpPoint = pd.Points[0];

		for (int i = 0; i < pd.Count-1; i++)
		{
			if ( pTypes[i + 1] == PathPointTypeStart || (pTypes[i] & PathPointTypeCloseSubpath))
			{
				CCArray<PointF> tmpPoints;
				GetLinePoints(pd.Points[i], tmpPoint, 1, tmpPoints);

				if (tmpPoints.GetSize()) // 0 when points are too close
				{
					if (tmpPoints.GetSize() + iPointsCount >= maxCount)
					{
						ASSERT(FALSE);
						break; // fatal error, pPathPoints size is not enough
					}

					for (int x=0; x<tmpPoints.GetSize(); x++)
						pPathPoints[x+iPointsCount] = tmpPoints[x];

					iPointsCount += 1;

					tmpPoint = pd.Points[i + 1];
				}
			}
			else
			{
				CCArray<PointF> tmpPoints;
				GetLinePoints(pd.Points[i], pd.Points[i + 1], 1, tmpPoints);

				if (tmpPoints.GetSize()) // 0 when points are too close
				{
					if (tmpPoints.GetSize() + iPointsCount >= maxCount)
					{
						ASSERT(FALSE);
						break; // fatal error, pPathPoints size is not enough
					}

					for (int x=0; x<tmpPoints.GetSize(); x++)
						pPathPoints[x+iPointsCount] = tmpPoints[x];

					iPointsCount += (int)tmpPoints.GetSize() - 1;
				}
			}
		}

		delete [] pTypes;
	}

	delete pGraphicPath;

	return iPointsCount;
}

void CXTPTextOnPath::Draw()
{
#ifdef _TEXTONPATH_TESTING

	_regionList.RemoveAll();
	_angles.RemoveAll();
	_pointText.RemoveAll();
	_pointTextUp.RemoveAll();

#endif

	PointF pathPoints[25001];
	int iPointsCount = CalculatePathPoints(pathPoints, 25001);

	if (!iPointsCount)
		return;

	CCArray<PointF> finalPoints;
	finalPoints.SetSize(iPointsCount);

	for (int i=0; i<iPointsCount; i++)
		finalPoints[i] = pathPoints[i];

	if (0) // OPTIMIZATION: CleanPoints seems to not useful in the FlowGraph
	{
		finalPoints = CleanPoints(finalPoints);
	}

	DrawText(finalPoints, (int)finalPoints.GetSize() - 1);

	DoTests();
}

void CXTPTextOnPath::DoTests()
{
#ifdef _TEXTONPATH_TESTING

	Brush *pBrush = new SolidBrush(Color::Red);

	for (int i=0; i<_regionList.GetSize(); i++)
	{
		RectF region = _regionList[i];

		m_pGraphics->FillRectangle(pBrush, region);
	}

	delete pBrush;

	for (int i=0; i<_angles.GetSize(); i++)
	{
		TRACE(_T("Angle %d is %f\n"), i, _angles[i]);
	}

	for (int i=0; i<_pointText.GetSize(); i++)
	{
		TRACE(_T("Point %d is (%f,%f)\n"), i, _pointText[i].X, _pointText[i].Y);
	}

	for (int i=0; i<_pointTextUp.GetSize(); i++)
	{
		TRACE(_T("Point UP %d is (%f,%f)\n"), i, _pointTextUp[i].X, _pointTextUp[i].Y);
	}

#endif
}
