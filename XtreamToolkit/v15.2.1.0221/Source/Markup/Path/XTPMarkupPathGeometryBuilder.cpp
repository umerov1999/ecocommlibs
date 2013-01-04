// XTPMarkupPathGeometryBuilder.cpp: implementation of the CXTPMarkupPathGeometryBuilder class.
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

#include "StdAfx.h"
#include <math.h>

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
	WCHAR ch3 = ' ';

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
	BezierTo(x1, y1, x2, y2, x2, y2);
}

void CXTPMarkupPathGeometryBuilder::LineTo(float x, float y)
{
	MARKUP_POINTF point = {x, y};
	m_arrPoints.Add(point);
	m_arrTypes.Add(xtpMarkupPathPointTypeLine);
}

void CXTPMarkupPathGeometryBuilder::CloseFigure()
{
	if (m_arrTypes.GetSize() > 0)
	{
		m_arrTypes[m_arrTypes.GetSize() - 1] = BYTE(m_arrTypes[m_arrTypes.GetSize() - 1] | xtpMarkupPathPointTypeCloseSubpath);
	}
}
