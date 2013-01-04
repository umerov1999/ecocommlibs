// XTPMarkupStackPanel.cpp: implementation of the CXTPMarkupStackPanel class.
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

#include "stdafx.h"
#include "math.h"

#include "XTPMarkupObject.h"
#include "XTPMarkupInputElement.h"
#include "XTPMarkupUIElement.h"
#include "XTPMarkupFrameworkElement.h"
#include "XTPMarkupPanel.h"
#include "XTPMarkupStackPanel.h"
#include "XTPMarkupBuilder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPMarkupDependencyProperty* CXTPMarkupStackPanel::m_pOrientationProperty = NULL;

IMPLEMENT_MARKUPCLASS(L"StackPanel", CXTPMarkupStackPanel, CXTPMarkupPanel)

void CXTPMarkupStackPanel::RegisterMarkupClass()
{
	m_pOrientationProperty = CXTPMarkupDependencyProperty::Register(L"Orientation", MARKUP_TYPE(CXTPMarkupEnum), MARKUP_TYPE(CXTPMarkupStackPanel),
		new CXTPMarkupPropertyMetadata(NULL, &CXTPMarkupBuilder::ConvertOrientation, CXTPMarkupPropertyMetadata::flagAffectsMeasure));
}

CXTPMarkupStackPanel::CXTPMarkupStackPanel()
{
}

CXTPMarkupStackPanel::~CXTPMarkupStackPanel()
{

}

XTPMarkupOrientation CXTPMarkupStackPanel::GetOrientation() const
{
	CXTPMarkupEnum* pOrientation = MARKUP_STATICCAST(CXTPMarkupEnum, GetValue(m_pOrientationProperty));
	return pOrientation ? (XTPMarkupOrientation)(int)(*pOrientation) : xtpMarkupOrientationVertical;
}

void CXTPMarkupStackPanel::SetOrientation(XTPMarkupOrientation orientation)
{
	SetValue(m_pOrientationProperty, CXTPMarkupEnum::CreateValue(orientation));
}

CSize CXTPMarkupStackPanel::MeasureOverride(CXTPMarkupDrawingContext* pDC, CSize constraint)
{
	CSize size(0, 0);
	CSize availableSize = constraint;

	bool bHorizontal = GetOrientation() == xtpMarkupOrientationHorizontal;

	if (bHorizontal)
	{
		availableSize.cx = INT_MAX;
	}
	else
	{
		availableSize.cy = INT_MAX;
	}

	int nCount = m_pChildren->GetCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPMarkupUIElement* pElement = m_pChildren->GetItem(i);
		if (pElement == NULL)
			continue;

		pElement->Measure(pDC, availableSize);
		CSize desiredSize = pElement->GetDesiredSize();

		if (bHorizontal)
		{
			size.cx += desiredSize.cx;
			size.cy = max(size.cy, desiredSize.cy);
		}
		else
		{
			size.cx = max(size.cx, desiredSize.cx);
			size.cy += desiredSize.cy;
		}
	}

	return size;
}

CSize CXTPMarkupStackPanel::ArrangeOverride(CSize arrangeSize)
{
	bool bHorizontal = GetOrientation() == xtpMarkupOrientationHorizontal;

	CRect finalRect(CPoint(0), arrangeSize);
	int width = 0;

	int nCount = m_pChildren->GetCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPMarkupUIElement* pElement = m_pChildren->GetItem(i);
		if (pElement == NULL)
			continue;

		if (bHorizontal)
		{
			finalRect.left += width;
			width = pElement->GetDesiredSize().cx;
			finalRect.right = finalRect.left + width;
			finalRect.bottom = max(arrangeSize.cy, pElement->GetDesiredSize().cy);
		}
		else
		{
			finalRect.top += width;
			width = pElement->GetDesiredSize().cy;
			finalRect.bottom = finalRect.top + width;
			finalRect.right = max(arrangeSize.cx, pElement->GetDesiredSize().cx);
		}

		pElement->Arrange(finalRect);
	}

	return arrangeSize;
}

//////////////////////////////////////////////////////////////////////////
// CXTPMarkupWrapPanel

CXTPMarkupDependencyProperty* CXTPMarkupWrapPanel::m_pOrientationProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupWrapPanel::m_pItemHeightProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupWrapPanel::m_pItemWidthProperty = NULL;


IMPLEMENT_MARKUPCLASS(L"WrapPanel", CXTPMarkupWrapPanel, CXTPMarkupPanel)

void CXTPMarkupWrapPanel::RegisterMarkupClass()
{
	m_pOrientationProperty = CXTPMarkupDependencyProperty::Register(L"Orientation", MARKUP_TYPE(CXTPMarkupEnum), MARKUP_TYPE(CXTPMarkupWrapPanel),
		new CXTPMarkupPropertyMetadata(NULL, &CXTPMarkupBuilder::ConvertOrientation, CXTPMarkupPropertyMetadata::flagAffectsMeasure));

	m_pItemHeightProperty = CXTPMarkupDependencyProperty::Register(L"ItemHeight", MARKUP_TYPE(CXTPMarkupInt), MARKUP_TYPE(CXTPMarkupWrapPanel),
		new CXTPMarkupPropertyMetadata(NULL,CXTPMarkupPropertyMetadata::flagAffectsMeasure));

	m_pItemWidthProperty = CXTPMarkupDependencyProperty::Register(L"ItemWidth", MARKUP_TYPE(CXTPMarkupInt), MARKUP_TYPE(CXTPMarkupWrapPanel),
		new CXTPMarkupPropertyMetadata(NULL, CXTPMarkupPropertyMetadata::flagAffectsMeasure));

}

CXTPMarkupWrapPanel::CXTPMarkupWrapPanel()
{
}

CXTPMarkupWrapPanel::~CXTPMarkupWrapPanel()
{

}

XTPMarkupOrientation CXTPMarkupWrapPanel::GetOrientation() const
{
	CXTPMarkupEnum* pOrientation = MARKUP_STATICCAST(CXTPMarkupEnum, GetValue(m_pOrientationProperty));
	return pOrientation ? (XTPMarkupOrientation)(int)(*pOrientation) : xtpMarkupOrientationHorizontal;
}

void CXTPMarkupWrapPanel::SetOrientation(XTPMarkupOrientation orientation)
{
	SetValue(m_pOrientationProperty, CXTPMarkupEnum::CreateValue(orientation));
}

inline int OrientationWidth(BOOL bHorizontal, const CSize& sz) { return bHorizontal ? sz.cx : sz.cy;};
inline int OrientationHeight(BOOL bHorizontal, const CSize& sz) { return bHorizontal ? sz.cy : sz.cx;};

CSize CXTPMarkupWrapPanel::MeasureOverride(CXTPMarkupDrawingContext* pDC, CSize constraint)
{
	BOOL bHorizontal = GetOrientation() == xtpMarkupOrientationHorizontal;

	CXTPMarkupInt* pItemWidth = MARKUP_STATICCAST(CXTPMarkupInt, GetValue(m_pItemWidthProperty));
	CXTPMarkupInt* pItemHeight = MARKUP_STATICCAST(CXTPMarkupInt, GetValue(m_pItemHeightProperty));

	CSize availableSize(pItemWidth != NULL ? (int)*pItemWidth : constraint.cx, pItemHeight != NULL ? (int)*pItemHeight : constraint.cy);

	int nTotalWidth = OrientationWidth(bHorizontal, constraint);

	int nLineWidth = 0;
	int nLineHeight = 0;

	int nPanelWidth = 0;
	int nPanelHeight = 0;

	int nCount = m_pChildren->GetCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPMarkupUIElement* pElement = m_pChildren->GetItem(i);
		if (pElement == NULL)
			continue;

		pElement->Measure(pDC, availableSize);
		CSize desiredSize(pItemWidth != NULL ? (int)*pItemWidth : pElement->GetDesiredSize().cx, pItemHeight != NULL ? (int)*pItemHeight : pElement->GetDesiredSize().cy);

		int nDesiredWidth = OrientationWidth(bHorizontal, desiredSize);
		int nDesiredHeight = OrientationHeight(bHorizontal, desiredSize);

		if (nLineWidth + nDesiredWidth > nTotalWidth)
		{
			nPanelWidth = max(nPanelWidth, nLineWidth);
			nPanelHeight += nLineHeight;

			nLineWidth = nDesiredWidth;
			nLineHeight = nDesiredHeight;
		}
		else
		{
			nLineWidth += nDesiredWidth;
			nLineHeight = max(nLineHeight, nDesiredHeight);
		}
	}

	nPanelWidth = max(nPanelWidth, nLineWidth);
	nPanelHeight += nLineHeight;

	return bHorizontal ? CSize(nPanelWidth, nPanelHeight) : CSize(nPanelHeight, nPanelWidth);
}

void CXTPMarkupWrapPanel::ArrangeLine(int nPanelHeight, int nLineHeight, int nFirstElement, int nLastElement)
{
	BOOL bHorizontal = GetOrientation() == xtpMarkupOrientationHorizontal;

	CXTPMarkupInt* pItemWidth = MARKUP_STATICCAST(CXTPMarkupInt, GetValue(bHorizontal ? m_pItemWidthProperty : m_pItemHeightProperty));

	int nTotalWidth = 0;

	for (int i = nFirstElement; i < nLastElement; i++)
	{
		CXTPMarkupUIElement* pElement = m_pChildren->GetItem(i);
		if (pElement == NULL)
			continue;

		int nWidth = pItemWidth != NULL ? (int)*pItemWidth : (bHorizontal ? pElement->GetDesiredSize().cx : pElement->GetDesiredSize().cy);

		if (bHorizontal)
		{
			pElement->Arrange(CRect(nTotalWidth, nPanelHeight, nTotalWidth + nWidth, nPanelHeight + nLineHeight));
		}
		else
		{
			pElement->Arrange(CRect(nPanelHeight, nTotalWidth, nPanelHeight + nLineHeight, nTotalWidth + nWidth));
		}

		nTotalWidth += nWidth;
	}
}

CSize CXTPMarkupWrapPanel::ArrangeOverride(CSize arrangeSize)
{
	BOOL bHorizontal = GetOrientation() == xtpMarkupOrientationHorizontal;

	CXTPMarkupInt* pItemWidth = MARKUP_STATICCAST(CXTPMarkupInt, GetValue(m_pItemWidthProperty));
	CXTPMarkupInt* pItemHeight = MARKUP_STATICCAST(CXTPMarkupInt, GetValue(m_pItemHeightProperty));

	int nTotalWidth = OrientationWidth(bHorizontal, arrangeSize);

	int nLineWidth = 0;
	int nLineHeight = 0;

	int nPanelHeight = 0;

	int nFirstElement = 0;

	int nCount = m_pChildren->GetCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPMarkupUIElement* pElement = m_pChildren->GetItem(i);
		if (pElement == NULL)
			continue;

		CSize desiredSize(pItemWidth != NULL ? (int)*pItemWidth : pElement->GetDesiredSize().cx, pItemHeight != NULL ? (int)*pItemHeight : pElement->GetDesiredSize().cy);

		int nDesiredWidth = OrientationWidth(bHorizontal, desiredSize);
		int nDesiredHeight = OrientationHeight(bHorizontal, desiredSize);

		if (nLineWidth + nDesiredWidth > nTotalWidth)
		{
			ArrangeLine(nPanelHeight, nLineHeight, nFirstElement, i);

			nPanelHeight += nLineHeight;

			nLineWidth = nDesiredWidth;
			nLineHeight = nDesiredHeight;

			nFirstElement = i;
		}
		else
		{
			nLineWidth += nDesiredWidth;
			nLineHeight = max(nLineHeight, nDesiredHeight);
		}
	}

	ArrangeLine(nPanelHeight, nLineHeight, nFirstElement, nCount);

	return arrangeSize;
}


//////////////////////////////////////////////////////////////////////////
// CXTPMarkupDockPanel

CXTPMarkupDependencyProperty* CXTPMarkupDockPanel::m_pLastChildFillProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupDockPanel::m_pDockProperty = NULL;


IMPLEMENT_MARKUPCLASS(L"DockPanel", CXTPMarkupDockPanel, CXTPMarkupPanel)

void CXTPMarkupDockPanel::RegisterMarkupClass()
{
	m_pLastChildFillProperty = CXTPMarkupDependencyProperty::Register(L"LastChildFill", MARKUP_TYPE(CXTPMarkupBool), MARKUP_TYPE(CXTPMarkupDockPanel),
		new CXTPMarkupPropertyMetadata(CXTPMarkupBool::CreateTrueValue(),CXTPMarkupPropertyMetadata::flagAffectsArrange));

	m_pDockProperty = CXTPMarkupDependencyProperty::RegisterAttached(L"Dock", MARKUP_TYPE(CXTPMarkupEnum), MARKUP_TYPE(CXTPMarkupDockPanel),
		new CXTPMarkupPropertyMetadata(CXTPMarkupEnum::CreateValue(xtpMarkupDockLeft), &CXTPMarkupDockPanel::ConvertDock, &CXTPMarkupDockPanel::OnDockChanged));

}

CXTPMarkupDockPanel::CXTPMarkupDockPanel()
{
}

CXTPMarkupDockPanel::~CXTPMarkupDockPanel()
{

}

CXTPMarkupObject* CXTPMarkupDockPanel::ConvertDock(CXTPMarkupBuilder* /*pBuilder*/, CXTPMarkupObject* pObject)
{
	if (IsStringObject(pObject))
	{
		LPCWSTR lpszValue = *((CXTPMarkupString*)pObject);
		int nLength = ((CXTPMarkupString*)pObject)->GetLength();

		if (nLength == 4 && _wcsicmp(lpszValue, L"Left") == 0)
			return CXTPMarkupEnum::CreateValue(xtpMarkupDockLeft);

		if (nLength == 5 && _wcsicmp(lpszValue, L"Right") == 0)
			return CXTPMarkupEnum::CreateValue(xtpMarkupDockRight);

		if (nLength == 3 && _wcsicmp(lpszValue, L"Top") == 0)
			return CXTPMarkupEnum::CreateValue(xtpMarkupDockTop);

		if (nLength == 6 && _wcsicmp(lpszValue, L"Bottom") == 0)
			return CXTPMarkupEnum::CreateValue(xtpMarkupDockBottom);
	}

	return NULL;
}

void CXTPMarkupDockPanel::OnDockChanged(CXTPMarkupObject* d, CXTPMarkupPropertyChangedEventArgs* /*e*/)
{
	CXTPMarkupUIElement* pVisual = MARKUP_DYNAMICCAST(CXTPMarkupUIElement, d);
	if (!pVisual)
		return;

	CXTPMarkupDockPanel* pPanel = MARKUP_DYNAMICCAST(CXTPMarkupDockPanel, pVisual->GetVisualParent());
	if (!pPanel)
		return;

	pPanel->InvalidateMeasure();
}

XTPMarkupDock CXTPMarkupDockPanel::GetDock(CXTPMarkupUIElement* pElement)
{
	CXTPMarkupEnum* pDock = MARKUP_STATICCAST(CXTPMarkupEnum, pElement->GetValue(m_pDockProperty));
	return pDock != NULL ? (XTPMarkupDock)(int)*pDock : xtpMarkupDockLeft;
}

void CXTPMarkupDockPanel::SetDock(CXTPMarkupUIElement* pElement, XTPMarkupDock dock)
{
	pElement->SetValue(m_pDockProperty, new CXTPMarkupEnum(dock));
}


CSize CXTPMarkupDockPanel::MeasureOverride(CXTPMarkupDrawingContext* pDC, CSize constraint)
{
	int nMaxWidth = 0;
	int nMaxHeight = 0;
	int nTotalWidth = 0;
	int nTotalHeight = 0;

	int nCount = m_pChildren->GetCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPMarkupUIElement* pElement = m_pChildren->GetItem(i);
		if (pElement == NULL)
			continue;

		CSize availableSize(max(0, constraint.cx - nTotalWidth), max(0, constraint.cy - nTotalHeight));
		pElement->Measure(pDC, availableSize);

		CSize desiredSize = pElement->GetDesiredSize();
		switch (GetDock(pElement))
		{
		case xtpMarkupDockLeft:
		case xtpMarkupDockRight:
			nMaxHeight = max(nMaxHeight, nTotalHeight + desiredSize.cy);
			nTotalWidth += desiredSize.cx;
			break;

		case xtpMarkupDockTop:
		case xtpMarkupDockBottom:
			nMaxWidth = max(nMaxWidth, nTotalWidth + desiredSize.cx);
			nTotalHeight += desiredSize.cy;
			break;
		}
	}

	return CSize(max(nMaxWidth, nTotalWidth), max(nMaxHeight, nTotalHeight));
}


CSize CXTPMarkupDockPanel::ArrangeOverride(CSize arrangeSize)
{
	int nCount = m_pChildren->GetCount();

	int nLastChild = nCount - (GetLastChildFill() ? 1 : 0);
	int xLeft = 0, yTop = 0, xRight = 0, yBottom = 0;

	for (int i = 0; i < nCount; i++)
	{
		CXTPMarkupUIElement* pElement = m_pChildren->GetItem(i);
		if (pElement == NULL)
			continue;

		CSize desiredSize = pElement->GetDesiredSize();
		CRect rcFinalRect(xLeft, yTop, xLeft + max(0, arrangeSize.cx - (xLeft + xRight)), yTop + max(0, arrangeSize.cy - (yTop + yBottom)));

		if (i < nLastChild)
		{
			switch (GetDock(pElement))
			{
			case xtpMarkupDockLeft:
				xLeft += desiredSize.cx;
				rcFinalRect.right = rcFinalRect.left + desiredSize.cx;
				break;

			case xtpMarkupDockTop:
				yTop += desiredSize.cy;
				rcFinalRect.bottom = rcFinalRect.top + desiredSize.cy;
				break;

			case xtpMarkupDockRight:
				xRight += desiredSize.cx;
				rcFinalRect.left = max(0, arrangeSize.cx - xRight);
				rcFinalRect.right = rcFinalRect.left + desiredSize.cx;
				break;

			case xtpMarkupDockBottom:
				yBottom += desiredSize.cy;
				rcFinalRect.top = max(0, arrangeSize.cy - yBottom);
				rcFinalRect.bottom = rcFinalRect.top + desiredSize.cy;
				break;
			}
		}
		pElement->Arrange(rcFinalRect);
	}
	return arrangeSize;
}

//////////////////////////////////////////////////////////////////////////
// CXTPMarkupUniformGrid

CXTPMarkupDependencyProperty* CXTPMarkupUniformGrid::m_pFirstColumnProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupUniformGrid::m_pColumnsProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupUniformGrid::m_pRowsProperty = NULL;


IMPLEMENT_MARKUPCLASS(L"UniformGrid", CXTPMarkupUniformGrid, CXTPMarkupPanel)

void CXTPMarkupUniformGrid::RegisterMarkupClass()
{
	m_pFirstColumnProperty = CXTPMarkupDependencyProperty::Register(L"FirstColumn", MARKUP_TYPE(CXTPMarkupInt), MARKUP_TYPE(CXTPMarkupUniformGrid),
		new CXTPMarkupPropertyMetadata(NULL, CXTPMarkupPropertyMetadata::flagAffectsMeasure));

	m_pColumnsProperty = CXTPMarkupDependencyProperty::Register(L"Columns", MARKUP_TYPE(CXTPMarkupInt), MARKUP_TYPE(CXTPMarkupUniformGrid),
		new CXTPMarkupPropertyMetadata(NULL, CXTPMarkupPropertyMetadata::flagAffectsMeasure));

	m_pRowsProperty = CXTPMarkupDependencyProperty::Register(L"Rows", MARKUP_TYPE(CXTPMarkupInt), MARKUP_TYPE(CXTPMarkupUniformGrid),
		new CXTPMarkupPropertyMetadata(NULL, CXTPMarkupPropertyMetadata::flagAffectsMeasure));
}

CXTPMarkupUniformGrid::CXTPMarkupUniformGrid()
{
	m_nColumns = m_nRows = 1;
	m_nFirstColumn = 0;
}

CXTPMarkupUniformGrid::~CXTPMarkupUniformGrid()
{

}


void CXTPMarkupUniformGrid::UpdateComputedValues()
{
	m_nColumns = GetColumns();
	m_nRows = GetRows();
	m_nFirstColumn = GetFirstColumn();

	if (m_nFirstColumn >= m_nColumns)
	{
		m_nFirstColumn = 0;
	}

	if ((m_nRows == 0) || (m_nColumns == 0))
	{
		int num = 0;
		int nCount = m_pChildren->GetCount();

		for (int i = 0; i < nCount; i++)
		{
			CXTPMarkupUIElement* pElement = m_pChildren->GetItem(i);
			if (pElement && pElement->GetVisibility() != xtpMarkupVisibilityCollapsed)
			{
				num++;
			}
		}

		if (num == 0)
		{
			num = 1;
		}

		if (m_nRows == 0)
		{
			if (m_nColumns > 0)
			{
				m_nRows = ((num + m_nFirstColumn) + (m_nColumns - 1)) / m_nColumns;
			}
			else
			{
				m_nRows = (int) sqrt((double)num);
				if ((m_nRows * m_nRows) < num)
				{
					m_nRows++;
				}
				m_nColumns = m_nRows;
			}
		}
		else if (m_nColumns == 0)
		{
			m_nColumns = (num + (m_nRows - 1)) / m_nRows;
		}
	}
}

CSize CXTPMarkupUniformGrid::MeasureOverride(CXTPMarkupDrawingContext* pDC, CSize constraint)
{
	UpdateComputedValues();

	CSize availableSize(constraint.cx / m_nColumns, constraint.cy / m_nRows);

	int nWidth = 0, nHeight = 0;

	int nCount = m_pChildren->GetCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPMarkupUIElement* pElement = m_pChildren->GetItem(i);
		if (pElement == NULL)
			continue;

		pElement->Measure(pDC, availableSize);

		CSize desiredSize = pElement->GetDesiredSize();

		if (nWidth < desiredSize.cx)
		{
			nWidth = desiredSize.cx;
		}
		if (nHeight < desiredSize.cy)
		{
			nHeight = desiredSize.cy;
		}
	}
	return CSize(nWidth * m_nColumns, nHeight * m_nRows);
}

CSize CXTPMarkupUniformGrid::ArrangeOverride(CSize arrangeSize)
{
	CRect rcFinalRect(0, 0, arrangeSize.cx / m_nColumns, arrangeSize.cy / m_nRows);

	int nWidth = rcFinalRect.Width();

	rcFinalRect.OffsetRect(nWidth * m_nFirstColumn, 0);

	int nCount = m_pChildren->GetCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPMarkupUIElement* pElement = m_pChildren->GetItem(i);
		if (pElement == NULL)
			continue;

		pElement->Arrange(rcFinalRect);

		if (pElement->GetVisibility() != xtpMarkupVisibilityCollapsed)
		{
			rcFinalRect.OffsetRect(nWidth, 0);

			if (rcFinalRect.right > arrangeSize.cx)
			{
				rcFinalRect = CRect(0, rcFinalRect.top + rcFinalRect.Height(), nWidth, rcFinalRect.bottom + rcFinalRect.Height());
			}
		}
	}
	return arrangeSize;
}

