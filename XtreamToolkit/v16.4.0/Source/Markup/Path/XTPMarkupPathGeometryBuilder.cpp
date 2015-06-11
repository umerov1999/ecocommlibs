// XTPMarkupPathGeometryBuilder.cpp: implementation of the CXTPMarkupPathGeometryBuilder class.
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

#define _USE_MATH_DEFINES // for M_PI
#include <math.h>

#ifndef M_PI
	#define M_PI       3.14159265358979323846
#endif

#include <Common/XTPVC80Helpers.h>

#include <Markup/XTPMarkupObject.h>
#include <Markup/XTPMarkupInputElement.h>
#include <Markup/XTPMarkupUIElement.h>
#include <Markup/XTPMarkupFrameworkElement.h>
#include <Markup/XTPMarkupShape.h>
#include <Markup/XTPMarkupDrawingContext.h>
#include <Markup/XTPMarkupBuilder.h>
#include <Markup/XTPMarkupThickness.h>
#include <Markup/Path/XTPMarkupPathData.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////////
// CXTPMarkupPathGeometryBuilder

CXTPMarkupPathGeometryBuilder::CXTPMarkupPathGeometryBuilder()
{
	m_lpszValue = NULL;
	m_nLength = 0;
	m_nIndex = 0;
	m_cToken = 0;
	m_bFigureStarted = FALSE;
	m_pBuilder = 0;
	m_nFillRule = 0;
}

void CXTPMarkupPathGeometryBuilder::ThrowBadToken()
{
	if (m_pBuilder)
	{
		CString strError;
		strError.Format(_T("Unexpected token encountered at position '%i'"), m_nIndex);
		m_pBuilder->ThrowBuilderException(strError);
	}
	else
	{
		AfxThrowMemoryException();
	}
}

BOOL CXTPMarkupPathGeometryBuilder::SkipWhiteSpace(BOOL allowComma)
{
	BOOL flag = FALSE;

	while (More())
	{
		WCHAR c = m_lpszValue[m_nIndex];

		switch (c)
		{
		case '\t':
		case '\n':
		case '\r':
		case ' ':
			break;

		case ',':
			if (allowComma)
			{
				flag = TRUE;
				allowComma = FALSE;
			}
			else
			{
				ThrowBadToken();
			}
			break;

		default:
			return flag;
			break;
		}
		m_nIndex++;
	}
	return flag;
}

BOOL CXTPMarkupPathGeometryBuilder::More() const
{
	return m_nIndex < m_nLength;
}

BOOL CXTPMarkupPathGeometryBuilder::ReadToken()
{
	SkipWhiteSpace(FALSE);
	if (More())
	{
		m_cToken = m_lpszValue[m_nIndex++];
		return TRUE;
	}
	return FALSE;
}

BOOL CXTPMarkupPathGeometryBuilder::IsNumber(BOOL allowComma)
{
	BOOL flag = SkipWhiteSpace(allowComma);
	if (More())
	{
		m_cToken = m_lpszValue[m_nIndex];
		if ((m_cToken == '.') || (m_cToken == '-') || (m_cToken == '+') || ((m_cToken >= '0') && (m_cToken <= '9')))
		{
			return TRUE;
		}
	}
	if (flag)
	{
		ThrowBadToken();
	}
	return FALSE;
}


MARKUP_POINTF CXTPMarkupPathGeometryBuilder::ReadPoint(WCHAR cmd, BOOL allowcomma)
{
	MARKUP_POINTF pt;
	pt.x = ReadNumber(allowcomma);
	pt.y = ReadNumber(TRUE);
	if (cmd >= 'a')
	{
		pt.x += m_lastPoint.x;
		pt.y += m_lastPoint.y;
	}
	return pt;
}

MARKUP_POINTF CXTPMarkupPathGeometryBuilder::ReadSize(BOOL allowcomma)
{
	MARKUP_POINTF pt;
	pt.x = ReadNumber(allowcomma);
	pt.y = ReadNumber(TRUE);

	return pt;
}

float CXTPMarkupPathGeometryBuilder::ReadNumber(BOOL allowComma)
{
	if (!IsNumber(allowComma))
	{
		ThrowBadToken();
	}

	BOOL bSign = FALSE;

	if (m_lpszValue[m_nIndex] == '+')
	{
		m_nIndex++;
	}
	else if (m_lpszValue[m_nIndex] == '-')
	{
		m_nIndex++;
		bSign = TRUE;
	}

	float dValue = 0;

	while (m_lpszValue[m_nIndex] >= '0' && m_lpszValue[m_nIndex] <= '9')
	{
		WCHAR c = m_lpszValue[m_nIndex];
		dValue = 10 * dValue + (c - '0');

		m_nIndex++;
	}
	if (m_lpszValue[m_nIndex] == '.')
	{
		float dDecimal = 1;

		m_nIndex++;
		while (m_lpszValue[m_nIndex] >= '0' && m_lpszValue[m_nIndex] <= '9')
		{
			WCHAR c = m_lpszValue[m_nIndex];

			dDecimal /= 10;
			dValue += dDecimal * float(c - '0');

			m_nIndex++;
		}

		if (m_lpszValue[m_nIndex] == 'e' || m_lpszValue[m_nIndex] == 'E')
		{
			// scientific notation

			BOOL bNegativeExponent = FALSE;

			m_nIndex++;

			if (m_lpszValue[m_nIndex] == '-')
			{
				bNegativeExponent = TRUE;

				m_nIndex++;
			}

			float exp = 0;

			while (m_lpszValue[m_nIndex] >= '0' && m_lpszValue[m_nIndex] <= '9')
			{
				WCHAR c = m_lpszValue[m_nIndex];
				exp = 10 * exp + (c - '0');

				m_nIndex++;
			}

			ASSERT(exp);

			dValue = (!bNegativeExponent) ? (float)pow(dValue, exp) : (dValue * (float)pow( (float)0.1, exp) );
		}
	}

	return bSign ? -dValue : dValue;
}

void CXTPMarkupPathGeometryBuilder::EnsureFigure()
{
	if (!m_bFigureStarted)
	{
		BeginFigure(m_lastStart.x, m_lastStart.y);
		m_bFigureStarted = TRUE;
	}
}

MARKUP_POINTF CXTPMarkupPathGeometryBuilder::Reflect()
{
	MARKUP_POINTF pt = {(2.0f * m_lastPoint.x) - m_secondLastPoint.x, (2.0f * m_lastPoint.y) - m_secondLastPoint.y};
	return pt;
}

void CXTPMarkupPathGeometryBuilder::Parse(CXTPMarkupBuilder* pBuilder, LPCWSTR lpszValue, int nLength)
{
	m_pBuilder = pBuilder;
	m_lpszValue = lpszValue;
	m_nLength = nLength;

	m_arrPoints.RemoveAll();
	m_arrTypes.RemoveAll();

	MARKUP_POINTF ptZero = {0, 0};
	m_secondLastPoint = ptZero;
	m_lastPoint = ptZero;
	m_lastStart = ptZero;
	m_bFigureStarted = FALSE;

	BOOL bFirstToken = TRUE;
	WCHAR ch3 = ' '; // last command

	MARKUP_POINTF qbzp = {0,0};

	while (ReadToken())
	{
		WCHAR cmd = m_cToken;
		if (bFirstToken)
		{
			if (cmd == 'F')
			{
				if (!IsNumber(FALSE))
					ThrowBadToken();

				if (m_lpszValue[m_nIndex] != '0' && m_lpszValue[m_nIndex] != '1')
					ThrowBadToken();

				m_nFillRule = m_lpszValue[m_nIndex] - '0';
				m_nIndex++;

				continue;
			}
			if ((cmd != 'M') && (cmd != 'm'))
			{
				ThrowBadToken();
			}
			bFirstToken = FALSE;
		}

		switch (cmd)
		{
			case 'L':
			case 'V':
			case 'H':
			case 'h':
			case 'l':
			case 'v':
			{
				EnsureFigure();
				do
				{
					switch (cmd)
					{
					case 'h':
						m_lastPoint.x += ReadNumber(FALSE);
						break;

					case 'l':
						m_lastPoint = ReadPoint(cmd, FALSE);
						break;

					case 'v':
						m_lastPoint.y += ReadNumber(FALSE);
						break;

					case 'H':
						m_lastPoint.x = ReadNumber(FALSE);
						break;

					case 'L':
						m_lastPoint = ReadPoint(cmd, FALSE);
						break;

					case 'V':
						m_lastPoint.y = ReadNumber(FALSE);
						break;
					}
					LineTo(m_lastPoint.x, m_lastPoint.y);
				}
				while (IsNumber(TRUE));
				ch3 = 'L';
				continue;
			}

			case 'M':
			case 'm':
			{
				m_lastPoint = ReadPoint(cmd, FALSE);
				BeginFigure(m_lastPoint.x, m_lastPoint.y);
				m_bFigureStarted = TRUE;
				m_lastStart = m_lastPoint;
				ch3 = 'M';
				while (IsNumber(TRUE))
				{
					m_lastPoint = ReadPoint(cmd, FALSE);
					LineTo(m_lastPoint.x, m_lastPoint.y);
					ch3 = 'L';
				}
				continue;
			}

			case 'a': // Elliptical Arc Command
			case 'A':
			{
				EnsureFigure();

				MARKUP_POINTF size = ReadSize(TRUE);
				float rotationAngle = ReadNumber(TRUE);
				BOOL isLargeArcFlag = (ReadNumber(TRUE) != 0) ? TRUE : FALSE;
				BOOL sweepDirectionFlag = (ReadNumber(TRUE) != 0) ? TRUE : FALSE;
				MARKUP_POINTF endPoint = ReadPoint(cmd, TRUE);

				if (!size.x || !size.y)
				{
					// If rx = 0 or ry = 0 then this arc is treated as a straight line segment (a "lineto") joining the endpoints.
					// See http://www.w3.org/TR/SVG/implnote.html, chapter F.6.2 Out-of-range parameters

					m_lastPoint = endPoint;

					LineTo(m_lastPoint.x, m_lastPoint.y);
				}
				else
				{
					ArcTo(endPoint.x, endPoint.y, rotationAngle, size.x, size.y, isLargeArcFlag, sweepDirectionFlag);
				}

				ch3 = 'A';

				continue;
			}

			case 'q': // quadratic Bezier curve Command
			case 'Q':
			{
				EnsureFigure();

				MARKUP_POINTF controlPoint = ReadPoint(cmd, TRUE);
				m_lastPoint = ReadPoint(cmd, FALSE);

				BezierTo(controlPoint.x, controlPoint.y, m_lastPoint.x, m_lastPoint.y);

				ch3 = 'T'; // if next command is T(t)-smooth, then we should tell it that the command before is also a quadratic bezier curve

				qbzp = controlPoint;

				continue;
			}
			case 'T': // Smooth quadratic Bezier curve Command
			case 't':
			{
				EnsureFigure();

				do
				{
					MARKUP_POINTF controlPoint;
					MARKUP_POINTF point = ReadPoint(cmd, FALSE);

					// The rule is: The control point is assumed to be the reflection of the control point of the previous command relative to the current point.
					// If there is no previous command or if the previous command was not a quadratic Bezier curve command or a smooth quadratic Bezier curve command, \
					// the control point is coincident with the current point.

					if (ch3 == 'T') // last command was also a quadratic Bezier curve (no matter smooth or not)
					{
						// reflect

						controlPoint.x = 2*m_lastPoint.x - qbzp.x;
						controlPoint.y = 2*m_lastPoint.y - qbzp.y;
					}
					else
					{
						controlPoint = m_lastPoint;
					}

					BezierTo(controlPoint.x, controlPoint.y, point.x, point.y);

					m_lastPoint = point;

					qbzp = controlPoint;

					ch3 = 'T'; // if next command is T(t)-smooth, then we should tell it that the command before is also a quadratic bezier curve
				}
				while (IsNumber(TRUE));

				ch3 = 'T'; // if next command is T(t)-smooth, then we should tell it that the command before is also a quadratic bezier curve

				continue;
			}

			case 'S':
			case 'C':
			case 'c':
			case 's':
				{
					EnsureFigure();
					do
					{
						MARKUP_POINTF point;
						switch (cmd)
						{
						case 's':
						case 'S':
							if (ch3 == 'C')
							{
								point = Reflect();
							}
							else
							{
								point = m_lastPoint;
							}
							m_secondLastPoint = ReadPoint(cmd, FALSE);
							break;

						default:
							point = ReadPoint(cmd, FALSE);
							m_secondLastPoint = ReadPoint(cmd, TRUE);
							break;
						}
						m_lastPoint = ReadPoint(cmd, TRUE);
						BezierTo(point.x, point.y, m_secondLastPoint.x, m_secondLastPoint.y, m_lastPoint.x, m_lastPoint.y);
						ch3 = 'C';
					}
					while (IsNumber(TRUE));
					continue;
				}

			case 'Z':
			case 'z':
			{
				EnsureFigure();
				CloseFigure();
				m_bFigureStarted = FALSE;
				ch3 = 'Z';
				m_lastPoint = m_lastStart;
				continue;
			}

			default:
				ThrowBadToken();
		}
	}
}

CXTPMarkupPathData* CXTPMarkupPathGeometryBuilder::CreateData() const
{
	return new CXTPMarkupPathData(m_arrPoints.GetData(), m_arrTypes.GetData(), (int)m_arrTypes.GetSize());
}

void CXTPMarkupPathGeometryBuilder::BeginFigure(float x, float y)
{
	MARKUP_POINTF startPoint = {x, y};
	m_arrPoints.Add(startPoint);
	m_arrTypes.Add(xtpMarkupPathPointTypeStart);
}

void CXTPMarkupPathGeometryBuilder::BezierTo(float x1, float y1, float x2, float y2, float x3, float y3)
{
	MARKUP_POINTF point1 = {x1, y1};
	MARKUP_POINTF point2 = {x2, y2};
	MARKUP_POINTF point3 = {x3, y3};

	m_arrPoints.Add(point1);
	m_arrPoints.Add(point2);
	m_arrPoints.Add(point3);

	m_arrTypes.Add(xtpMarkupPathPointTypeBezier);
	m_arrTypes.Add(xtpMarkupPathPointTypeBezier);
	m_arrTypes.Add(xtpMarkupPathPointTypeBezier);
}

void CXTPMarkupPathGeometryBuilder::BezierTo(float x1, float y1, float x2, float y2)
{
	// quadratic to cubic Bezier, the original control point and the end control point are the same

	BezierTo(x1, y1, x2, y2, x2, y2);
}

void CXTPMarkupPathGeometryBuilder::LineTo(float x, float y)
{
	MARKUP_POINTF point = {x, y};
	m_arrPoints.Add(point);
	m_arrTypes.Add(xtpMarkupPathPointTypeLine);
}

void CXTPMarkupPathGeometryBuilder::ArcSegment(double xc, double yc, double th0, double th1, double rx, double ry, double xAxisRotation)
{
	double sinTh, cosTh;
	double a00, a01, a10, a11;
	double x1, y1, x2, y2, x3, y3;
	double t;
	double thHalf;

	sinTh = sin(xAxisRotation * (M_PI / 180.0));
	cosTh = cos(xAxisRotation * (M_PI / 180.0));

	a00 =  cosTh * rx;
	a01 = -sinTh * ry;
	a10 =  sinTh * rx;
	a11 =  cosTh * ry;

	thHalf = 0.5 * (th1 - th0);
	t = (8.0 / 3.0) * sin(thHalf * 0.5) * sin(thHalf * 0.5) / sin(thHalf);
	x1 = xc + cos(th0) - t * sin(th0);
	y1 = yc + sin(th0) + t * cos(th0);
	x3 = xc + cos(th1);
	y3 = yc + sin(th1);
	x2 = x3 + t * sin(th1);
	y2 = y3 - t * cos(th1);

	BezierTo((float)(a00 * x1 + a01 * y1), (float)(a10 * x1 + a11 * y1),
		(float)(a00 * x2 + a01 * y2), (float)(a10 * x2 + a11 * y2),
		(float)(a00 * x3 + a01 * y3), (float)(a10 * x3 + a11 * y3) );
}

void CXTPMarkupPathGeometryBuilder::ArcTo(float x, float y, float x_axis_rotation, double rx, double ry, BOOL large_arc_flag, BOOL sweep_flag)
{
	double curx = m_lastPoint.x;
	double cury = m_lastPoint.y;

	double sin_th, cos_th;
	double a00, a01, a10, a11;
	double x0, y0, x1, y1, xc, yc;
	double d, sfactor, sfactor_sq;
	double th0, th1, th_arc;
	int i, n_segs;
	double dx, dy, dx1, dy1, Pr1, Pr2, Px, Py, check;

	rx = fabs(rx);
	ry = fabs(ry);

	sin_th = sin(x_axis_rotation * (M_PI / 180.0));
	cos_th = cos(x_axis_rotation * (M_PI / 180.0));

	dx = (curx - x) / 2.0;
	dy = (cury - y) / 2.0;
	dx1 =  cos_th * dx + sin_th * dy;
	dy1 = -sin_th * dx + cos_th * dy;
	Pr1 = rx * rx;
	Pr2 = ry * ry;
	Px = dx1 * dx1;
	Py = dy1 * dy1;

	// Spec : check if radii are large enough
	check = Px / Pr1 + Py / Pr2;
	if (check > 1)
	{
		rx = rx * sqrt(check);
		ry = ry * sqrt(check);
	}

	a00 =  cos_th / rx;
	a01 =  sin_th / rx;
	a10 = -sin_th / ry;
	a11 =  cos_th / ry;
	x0 = a00 * curx + a01 * cury;
	y0 = a10 * curx + a11 * cury;
	x1 = a00 * x + a01 * y;
	y1 = a10 * x + a11 * y;

	/* (x0, y0) is current point in transformed coordinate space.
	   (x1, y1) is new point in transformed coordinate space.

	   The arc fits a unit-radius circle in this space.
	*/

	d = (x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0);
	sfactor_sq = 1.0 / d - 0.25;
	if (sfactor_sq < 0) sfactor_sq = 0;
	sfactor = sqrt(sfactor_sq);
	if (sweep_flag == large_arc_flag) sfactor = -sfactor;
	xc = 0.5 * (x0 + x1) - sfactor * (y1 - y0);
	yc = 0.5 * (y0 + y1) + sfactor * (x1 - x0);
	// (xc, yc) is center of the circle.

	th0 = atan2(y0 - yc, x0 - xc);
	th1 = atan2(y1 - yc, x1 - xc);

	th_arc = th1 - th0;

	if (th_arc < 0 && sweep_flag)
		th_arc += 2 * M_PI;
	else if (th_arc > 0 && !sweep_flag)
		th_arc -= 2 * M_PI;

	// number of arc segments, each segment is drawn using a cubic bezier curve
	n_segs = (int)ceil(fabs(th_arc / (M_PI * 0.5 + 0.001)));

	for (i = 0; i < n_segs; i++)
	{
		ArcSegment(xc, yc, th0 + i * th_arc / n_segs, th0 + (i + 1) * th_arc / n_segs, rx, ry, x_axis_rotation);
	}
}

void CXTPMarkupPathGeometryBuilder::CloseFigure()
{
	if (m_arrTypes.GetSize() > 0)
	{
		m_arrTypes[m_arrTypes.GetSize() - 1] = BYTE(m_arrTypes[m_arrTypes.GetSize() - 1] | xtpMarkupPathPointTypeCloseSubpath);
	}
}
