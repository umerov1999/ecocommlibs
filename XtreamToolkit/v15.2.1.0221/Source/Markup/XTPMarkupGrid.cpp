// XTPMarkupGrid.cpp: implementation of the CXTPMarkupGrid class.
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

#include "XTPMarkupObject.h"
#include "XTPMarkupInputElement.h"
#include "XTPMarkupUIElement.h"
#include "XTPMarkupFrameworkElement.h"
#include "XTPMarkupPanel.h"
#include "XTPMarkupGrid.h"
#include "XTPMarkupBuilder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPMarkupDependencyProperty* CXTPMarkupGrid::m_pColumnDefinitionsProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupGrid::m_pRowDefinitionsProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupGrid::m_pColumnProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupGrid::m_pRowProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupGrid::m_pColumnSpanProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupGrid::m_pRowSpanProperty = NULL;

IMPLEMENT_MARKUPCLASS(L"Grid", CXTPMarkupGrid, CXTPMarkupPanel)

void CXTPMarkupGrid::RegisterMarkupClass()
{
	m_pColumnDefinitionsProperty = CXTPMarkupDependencyProperty::Register(L"ColumnDefinitions", MARKUP_TYPE(CXTPMarkupColumnDefinitionCollection), MARKUP_TYPE(CXTPMarkupGrid));

	m_pRowDefinitionsProperty = CXTPMarkupDependencyProperty::Register(L"RowDefinitions", MARKUP_TYPE(CXTPMarkupRowDefinitionCollection), MARKUP_TYPE(CXTPMarkupGrid));

	m_pColumnProperty = CXTPMarkupDependencyProperty::RegisterAttached(L"Column", MARKUP_TYPE(CXTPMarkupInt), MARKUP_TYPE(CXTPMarkupGrid),
		new CXTPMarkupPropertyMetadata(NULL, &CXTPMarkupGrid::OnCellAttachedPropertyChanged));

	m_pRowProperty = CXTPMarkupDependencyProperty::RegisterAttached(L"Row", MARKUP_TYPE(CXTPMarkupInt), MARKUP_TYPE(CXTPMarkupGrid),
		new CXTPMarkupPropertyMetadata(NULL, &CXTPMarkupGrid::OnCellAttachedPropertyChanged));

	m_pColumnSpanProperty = CXTPMarkupDependencyProperty::RegisterAttached(L"ColumnSpan", MARKUP_TYPE(CXTPMarkupInt), MARKUP_TYPE(CXTPMarkupGrid),
		new CXTPMarkupPropertyMetadata(NULL, &CXTPMarkupGrid::OnCellAttachedPropertyChanged));

	m_pRowSpanProperty = CXTPMarkupDependencyProperty::RegisterAttached(L"RowSpan", MARKUP_TYPE(CXTPMarkupInt), MARKUP_TYPE(CXTPMarkupGrid),
		new CXTPMarkupPropertyMetadata(NULL, &CXTPMarkupGrid::OnCellAttachedPropertyChanged));
}


CXTPMarkupGrid::CXTPMarkupGrid()
{
	m_pRowDefinitions = new CXTPMarkupRowDefinitionCollection();
	m_pRowDefinitions->SetLogicalParent(this);

	m_pColumnDefinitions = new CXTPMarkupColumnDefinitionCollection();
	m_pColumnDefinitions->SetLogicalParent(this);

	m_pCellCachesCollection = NULL;

	m_pDefinitionsU = NULL;
	m_pDefinitionsV = NULL;
}

CXTPMarkupGrid::~CXTPMarkupGrid()
{
	MARKUP_RELEASE(m_pColumnDefinitions);
	MARKUP_RELEASE(m_pRowDefinitions);

	SAFE_DELETE_AR(m_pCellCachesCollection);

	MARKUP_RELEASE(m_pDefinitionsU);
	MARKUP_RELEASE(m_pDefinitionsV);
}

void CXTPMarkupGrid::OnCellAttachedPropertyChanged(CXTPMarkupObject* d, CXTPMarkupPropertyChangedEventArgs* /*e*/)
{
	CXTPMarkupUIElement* pVisual = MARKUP_DYNAMICCAST(CXTPMarkupUIElement, d);
	if (!pVisual)
		return;

	CXTPMarkupGrid* pGrid = MARKUP_DYNAMICCAST(CXTPMarkupGrid, pVisual->GetVisualParent());
	if (!pGrid)
		return;

	pGrid->InvalidateMeasure();
}

int CXTPMarkupGrid::GetColumn(CXTPMarkupUIElement* pElement)
{
	CXTPMarkupInt* pColumn = MARKUP_STATICCAST(CXTPMarkupInt, pElement->GetValue(m_pColumnProperty));
	return pColumn != NULL ? (int)*pColumn : 0;
}

int CXTPMarkupGrid::GetRow(CXTPMarkupUIElement* pElement)
{
	CXTPMarkupInt* pRow = MARKUP_STATICCAST(CXTPMarkupInt, pElement->GetValue(m_pRowProperty));
	return pRow != NULL ? (int)*pRow : 0;
}

void CXTPMarkupGrid::SetColumn(CXTPMarkupUIElement* pElement, int nColumn)
{
	pElement->SetValue(m_pColumnProperty, new CXTPMarkupInt(nColumn));
}

void CXTPMarkupGrid::SetRow(CXTPMarkupUIElement* pElement, int nRow)
{
	pElement->SetValue(m_pRowProperty, new CXTPMarkupInt(nRow));
}

void CXTPMarkupGrid::SetColumnSpan(CXTPMarkupUIElement* pElement, int nColumn)
{
	pElement->SetValue(m_pColumnSpanProperty, new CXTPMarkupInt(nColumn));
}

void CXTPMarkupGrid::SetRowSpan(CXTPMarkupUIElement* pElement, int nRow)
{
	pElement->SetValue(m_pRowSpanProperty, new CXTPMarkupInt(nRow));
}

int CXTPMarkupGrid::GetColumnSpan(CXTPMarkupUIElement* pElement)
{
	CXTPMarkupInt* pColumnSpan = MARKUP_STATICCAST(CXTPMarkupInt, pElement->GetValue(m_pColumnSpanProperty));
	if (!pColumnSpan)
		return 1;

	return max(1, (int)*pColumnSpan);
}

int CXTPMarkupGrid::GetRowSpan(CXTPMarkupUIElement* pElement)
{
	CXTPMarkupInt* pRowSpan = MARKUP_STATICCAST(CXTPMarkupInt, pElement->GetValue(m_pRowSpanProperty));
	if (!pRowSpan)
		return 1;

	return max(1, (int)*pRowSpan);
}


void CXTPMarkupGrid::SetPropertyObject(CXTPMarkupBuilder* pBuilder, CXTPMarkupDependencyProperty* pProperty, CXTPMarkupObject* pValue)
{
	if (pProperty == m_pColumnDefinitionsProperty)
	{
		if (!pValue || !pValue->IsKindOf(MARKUP_TYPE(CXTPMarkupColumnDefinitionCollection)))
		{
			pBuilder->ThrowBuilderException(_T("CXTPMarkupColumnDefinitionCollection class expected"));
		}

		MARKUP_RELEASE(m_pColumnDefinitions);
		m_pColumnDefinitions = (CXTPMarkupColumnDefinitionCollection*)pValue;
		m_pColumnDefinitions->SetLogicalParent(this);
	}
	else if (pProperty == m_pRowDefinitionsProperty)
	{
		if (!pValue || !pValue->IsKindOf(MARKUP_TYPE(CXTPMarkupRowDefinitionCollection)))
		{
			pBuilder->ThrowBuilderException(_T("CXTPMarkupRowDefinitionCollection class expected"));
		}

		MARKUP_RELEASE(m_pRowDefinitions);
		m_pRowDefinitions = (CXTPMarkupRowDefinitionCollection*)pValue;
		m_pRowDefinitions->SetLogicalParent(this);

	}
	else
	{
		CXTPMarkupPanel::SetPropertyObject(pBuilder, pProperty, pValue);
	}
}

AFX_INLINE BOOL IsPositiveInfinity(int size) {
	return size > 32000;
}

AFX_INLINE BOOL IsNan(int nSize) {
	return nSize == INT_MAX;
}

void CXTPMarkupGrid::ValidateDefinitionsLayout(CXTPMarkupDefinitionCollection* pDefinitions, BOOL bTreatStarAsAuto)
{
	for (int i = 0; i < pDefinitions->GetCount(); i++)
	{
		CXTPMarkupDefinitionBase* pDefinition = pDefinitions->GetItem(i);
		pDefinition->OnBeforeLayout();
		int nUserMinSize = pDefinition->GetUserMinSize();
		int nUserMaxSize = pDefinition->GetUserMaxSize();
		int nPositiveInfinity = INT_MAX;
		switch (pDefinition->GetUserSize()->GetUnitType())
		{
		case CXTPMarkupGridLength::unitTypeAuto:
			pDefinition->m_nSizeType = sizeTypeAuto;
			break;

		case  CXTPMarkupGridLength::unitTypePixel:
			pDefinition->m_nSizeType = sizeTypePixel;
			nPositiveInfinity = (int)pDefinition->GetUserSize()->GetValue();
			nUserMinSize = max(nUserMinSize, min(nPositiveInfinity, nUserMaxSize));
			break;

		case CXTPMarkupGridLength::unitTypeStar:
			pDefinition->m_nSizeType = bTreatStarAsAuto ? sizeTypeAuto : sizeTypeStar;
			break;
		}

		pDefinition->UpdateMinSize(nUserMinSize);
		pDefinition->m_nMeasureSize = max(nUserMinSize, min(nPositiveInfinity, nUserMaxSize));
	}
}

int CXTPMarkupGrid::GetLengthTypeForRange(CXTPMarkupDefinitionCollection* pDefinitions, int start, int count)
{
	ASSERT(pDefinitions->GetCount() > 0);
	int nSizeType = sizeTypeNone;

	for (int i = start; i < start + count; i++)
	{
		nSizeType = (nSizeType | pDefinitions->GetItem(i)->m_nSizeType);
	}

	return nSizeType;
}

BOOL CXTPMarkupGrid::CELLCACHE::IsStarU() const
{
	return nSizeTypeU & CXTPMarkupGrid::sizeTypeStar;
}

BOOL CXTPMarkupGrid::CELLCACHE::IsStarV() const
{
	return nSizeTypeV & CXTPMarkupGrid::sizeTypeStar;
}

BOOL CXTPMarkupGrid::CELLCACHE::IsAutoU() const
{
	return nSizeTypeU & CXTPMarkupGrid::sizeTypeAuto;
}

BOOL CXTPMarkupGrid::CELLCACHE::IsAutoV() const
{
	return nSizeTypeV & CXTPMarkupGrid::sizeTypeAuto;
}

void CXTPMarkupGrid::ValidateCells()
{
	SAFE_DELETE_AR(m_pCellCachesCollection);

	m_pCellCachesCollection = new CELLCACHE[GetChildren()->GetCount()];
	m_nCellGroup[0] = INT_MAX;
	m_nCellGroup[1] = INT_MAX;
	m_nCellGroup[2] = INT_MAX;
	m_nCellGroup[3] = INT_MAX;
	BOOL flag = FALSE;
	BOOL flag2 = FALSE;
	BOOL flag3 = FALSE;

	for (int i = GetChildren()->GetCount() - 1; i >= 0; i--)
	{
		CXTPMarkupUIElement* pElement = GetChildren()->GetItem(i);
		if (!pElement)
			continue;

		CELLCACHE& cache = m_pCellCachesCollection[i];
		cache.nColumnIndex = min(GetColumn(pElement), m_pDefinitionsU->GetCount() - 1);
		cache.nRowIndex = min(GetRow(pElement), m_pDefinitionsV->GetCount() - 1);
		cache.nColumnSpan = min(GetColumnSpan(pElement), m_pDefinitionsU->GetCount() - cache.nColumnIndex);
		cache.nRowSpan = min(GetRowSpan(pElement),  m_pDefinitionsV->GetCount() - cache.nRowIndex);
		cache.nSizeTypeU = GetLengthTypeForRange(m_pDefinitionsU, cache.nColumnIndex, cache.nColumnSpan);
		cache.nSizeTypeV = GetLengthTypeForRange(m_pDefinitionsV, cache.nRowIndex, cache.nRowSpan);
		flag |= cache.IsStarU();
		flag2 |= cache.IsStarV();
		if (!cache.IsStarV())
		{
			if (!cache.IsStarU())
			{
				cache.nNext = m_nCellGroup[0];
				m_nCellGroup[0] = i;
			}
			else
			{
				cache.nNext = m_nCellGroup[2];
				m_nCellGroup[2] = i;
				flag3 |= cache.IsAutoV();
			}
		}
		else if (cache.IsAutoU() && !cache.IsStarU())
		{
			cache.nNext = m_nCellGroup[1];
			m_nCellGroup[1] = i;
		}
		else
		{
			cache.nNext = m_nCellGroup[3];
			m_nCellGroup[3] = i;
		}
	}

	m_bHasStarCellsU = flag;
	m_bHasStarCellsV = flag2;
	m_bHasGroup2CellsInAutoRows = flag3;
}

int CXTPMarkupGrid::GetMeasureSizeForRange(CXTPMarkupDefinitionCollection* pDefinitions, int start, int count)
{
	double num = 0;
	for (int nIndex = start; nIndex < start + count; nIndex++)
	{
		CXTPMarkupDefinitionBase* pDefinition = pDefinitions->GetItem(nIndex);
		num += pDefinition->m_nSizeType == sizeTypeAuto ? pDefinition->m_nMinSize : pDefinition->m_nMeasureSize;
	}

	return (int)num;
}

void CXTPMarkupGrid::MeasureCell(CXTPMarkupDrawingContext* pDC, int cell, BOOL forceInfinityV)
{
	int nPositiveInfinity;
	int num2;
	if (m_pCellCachesCollection[cell].IsAutoU() && !m_pCellCachesCollection[cell].IsStarU())
	{
		nPositiveInfinity = INT_MAX;
	}
	else
	{
		nPositiveInfinity = GetMeasureSizeForRange(m_pDefinitionsU, m_pCellCachesCollection[cell].nColumnIndex, m_pCellCachesCollection[cell].nColumnSpan);
	}

	if (forceInfinityV)
	{
		num2 = INT_MAX;
	}
	else if (m_pCellCachesCollection[cell].IsAutoV() && !m_pCellCachesCollection[cell].IsStarV())
	{
		num2 = INT_MAX;
	}
	else
	{
		num2 = GetMeasureSizeForRange(m_pDefinitionsV, m_pCellCachesCollection[cell].nRowIndex, m_pCellCachesCollection[cell].nRowSpan);
	}

	CXTPMarkupUIElement* pElement = GetChildren()->GetItem(cell);
	if (pElement != NULL)
	{
		pElement->Measure(pDC, CSize(nPositiveInfinity, num2));
	}
}

void CXTPMarkupGrid::MeasureCellsGroup(CXTPMarkupDrawingContext* pDC, int cellsHead, CSize /*referenceSize*/, BOOL ignoreDesiredSizeU, BOOL forceInfinityV)
{
	CArray<SPANKEY, SPANKEY&> arrSpans;

	int nCount = GetChildren()->GetCount();
	if (cellsHead < nCount)
	{
		int cell = cellsHead;

		do
		{
			CELLCACHE& cellCache =  m_pCellCachesCollection[cell];

			MeasureCell(pDC, cell, forceInfinityV);
			if (!ignoreDesiredSizeU)
			{
				if (cellCache.nColumnSpan == 1)
				{
					m_pDefinitionsU->GetItem(cellCache.nColumnIndex)->UpdateMinSize(GetChildren()->GetItem(cell)->GetDesiredSize().cx);
				}
				else
				{
					SPANKEY sk;
					sk.nCount = cellCache.nColumnSpan;
					sk.nStart = cellCache.nColumnIndex;
					sk.nValue = GetChildren()->GetItem(cell)->GetDesiredSize().cx;
					sk.bU = TRUE;
					arrSpans.Add(sk);
				}
			}
			if (!forceInfinityV)
			{
				if (cellCache.nRowSpan == 1)
				{
					m_pDefinitionsV->GetItem(cellCache.nRowIndex)->UpdateMinSize(GetChildren()->GetItem(cell)->GetDesiredSize().cy);
				}
				else
				{
					SPANKEY sk;
					sk.nCount = cellCache.nRowSpan;
					sk.nStart = cellCache.nRowIndex;
					sk.nValue = GetChildren()->GetItem(cell)->GetDesiredSize().cy;
					sk.bU = FALSE;
					arrSpans.Add(sk);
				}
			}
			cell = cellCache.nNext;
		}
		while (cell < nCount);
	}

	for (int i = 0; i < (int)arrSpans.GetSize(); i++)
	{
		const SPANKEY& key = arrSpans[i];

		EnsureMinSizeInDefinitionRange(key.bU ? m_pDefinitionsU : m_pDefinitionsV, key.nStart, key.nCount, key.nValue);
	}
}

void CXTPMarkupGrid::EnsureMinSizeInDefinitionRange(CXTPMarkupDefinitionCollection* pDefinitions, int start, int count, int requestedSize)
{
	if (requestedSize == 0)
		return;

	CXTPMarkupDefinitionBase** pTempDefinitions = new CXTPMarkupDefinitionBase*[pDefinitions->GetCount()];

	int num = start + count;
	int nCountAuto = 0;
	double dTotalMinSize = 0;
	double dTotalPreferedSize = 0;
	double dTotalMaxSize = 0;
	double dMaxSize = 0;

	for (int i = start; i < num; i++)
	{
		CXTPMarkupDefinitionBase* pDefinition = pDefinitions->GetItem(i);
		double minSize = pDefinition->m_nMinSize;
		double preferredSize = pDefinition->GetPreferredSize();
		double maxSize = IsNan(pDefinition->GetUserMaxSize()) ? minSize : max(minSize, pDefinition->GetUserMaxSize());

		dTotalMinSize += minSize;
		dTotalPreferedSize += preferredSize;
		dTotalMaxSize += maxSize;
		pDefinition->m_nSizeCache = maxSize;
		if (dMaxSize < maxSize)
		{
			dMaxSize = maxSize;
		}
		if (pDefinition->GetUserSize()->IsAuto())
		{
			nCountAuto++;
		}
		pTempDefinitions[i - start] = pDefinition;
	}

	if (requestedSize > dTotalMinSize)
	{
		if (requestedSize <= dTotalPreferedSize)
		{
			qsort(pTempDefinitions, count, sizeof(CXTPMarkupDefinitionBase*), SpanPreferredDistributionOrderComparer);

			int index = 0;
			double d = requestedSize;
			while (index < nCountAuto)
			{
				d -= pTempDefinitions[index]->m_nMinSize;
				index++;
			}
			while (index < count)
			{
				double a = min(d / ((double) (count - index)), pTempDefinitions[index]->GetPreferredSize());
				if (a > pTempDefinitions[index]->m_nMinSize)
				{
					pTempDefinitions[index]->UpdateMinSize((int)a);
				}
				d -= a;
				index++;
			}
		}
		else if (requestedSize <= dTotalMaxSize)
		{
			qsort(pTempDefinitions, count, sizeof(CXTPMarkupDefinitionBase*), SpanMaxDistributionOrderComparer);
			int n = 0;
			double d = requestedSize - dTotalPreferedSize;
			while (n < (count - nCountAuto))
			{
				double num16 = pTempDefinitions[n]->GetPreferredSize();
				double num17 = num16 + (d / ((double) ((count - nCountAuto) - n)));
				pTempDefinitions[n]->UpdateMinSize((int)min(num17, pTempDefinitions[n]->m_nSizeCache));
				d -= pTempDefinitions[n]->m_nMinSize - num16;
				n++;
			}
			while (n < count)
			{
				double num18 = pTempDefinitions[n]->m_nMinSize;
				double num19 = num18 + (d / ((double) (count - n)));
				pTempDefinitions[n]->UpdateMinSize((int)(min(num19, pTempDefinitions[n]->m_nSizeCache)));
				d -= pTempDefinitions[n]->m_nMinSize - num18;
				n++;
			}
		}
		else
		{
			double dAverage = (double)requestedSize / ((double) count);
			if (dAverage < dMaxSize)
			{
				double num21 = (dMaxSize * count) - dTotalMaxSize;
				double num22 = requestedSize - dTotalMaxSize;
				for (int j = 0; j < count; j++)
				{
					double num24 = ((dMaxSize - pTempDefinitions[j]->m_nSizeCache) * num22) / num21;
					pTempDefinitions[j]->UpdateMinSize(int(pTempDefinitions[j]->m_nSizeCache + num24));
				}
			}
			else
			{
				for (int k = 0; k < count; k++)
				{
					pTempDefinitions[k]->UpdateMinSize((int)dAverage);
				}
			}
		}
	}

	delete[] pTempDefinitions;
}

void CXTPMarkupGrid::ValidateDefinitions()
{
	MARKUP_RELEASE(m_pDefinitionsU);
	MARKUP_RELEASE(m_pDefinitionsV);

	if (m_pColumnDefinitions->GetCount() == 0)
	{
		m_pDefinitionsU = new CXTPMarkupColumnDefinitionCollection();
		m_pDefinitionsU->Add(new CXTPMarkupColumnDefinition());
	}
	else
	{
		m_pDefinitionsU = m_pColumnDefinitions;
		m_pDefinitionsU->AddRef();
	}

	if (m_pRowDefinitions->GetCount() == 0)
	{
		m_pDefinitionsV = new CXTPMarkupRowDefinitionCollection();
		m_pDefinitionsV->Add(new CXTPMarkupRowDefinition());
	}
	else
	{
		m_pDefinitionsV = m_pRowDefinitions;
		m_pDefinitionsV->AddRef();
	}
}

int CXTPMarkupGrid::CalculateDesiredSize(CXTPMarkupDefinitionCollection* pDefinitions)
{
	int num = 0;
	for (int i = 0; i < pDefinitions->GetCount(); i++)
	{
		num += pDefinitions->GetItem(i)->m_nMinSize;
	}
	return num;
}

void CXTPMarkupGrid::ResolveStar(CXTPMarkupDefinitionCollection* pDefinitions, double availableSize)
{
	CXTPMarkupDefinitionBase** pTempDefinitions = new CXTPMarkupDefinitionBase*[pDefinitions->GetCount()];

	int length = 0;
	double num2 = 0;

	for (int i = 0; i < pDefinitions->GetCount(); i++)
	{
		CXTPMarkupDefinitionBase* pDefinition = pDefinitions->GetItem(i);

		switch (pDefinition->m_nSizeType)
		{
		case sizeTypePixel:
			num2 += pDefinition->m_nMeasureSize;
			continue;

		case sizeTypeAuto:
			num2 +=  pDefinition->m_nMinSize;
			continue;

		case sizeTypeStar:
			{
				pTempDefinitions[length++] = pDefinition;
				double num4 = pDefinition->GetUserSize()->GetValue();
				if (num4 != 0)
				{
					pDefinition->m_nMeasureSize = num4;
					double num5 = IsNan(pDefinition->GetUserMaxSize()) ? pDefinition->m_nMinSize : max(pDefinition->m_nMinSize, pDefinition->GetUserMaxSize());
					pDefinition->m_nSizeCache = num5 / num4;
				}
				else
				{
					pDefinition->m_nMeasureSize = 0;
					pDefinition->m_nSizeCache = 0;
				}
				continue;
			}
		}
	}
	if (length > 0)
	{
		qsort(pTempDefinitions, length, sizeof(CXTPMarkupDefinitionBase*), StarDistributionOrderComparer);

		int nIndex;
		double num6 = 0;
		for (nIndex = length - 1; nIndex >= 0; nIndex--)
		{
			num6 += pTempDefinitions[nIndex]->m_nMeasureSize;
			pTempDefinitions[nIndex]->m_nSizeCache = num6;
		}

		for (nIndex = 0; nIndex < length; nIndex++)
		{
			CXTPMarkupDefinitionBase* pDefinition = pTempDefinitions[nIndex];

			int nMinSize;
			double nMeasureSize = pDefinition->m_nMeasureSize;
			if (nMeasureSize == 0)
			{
				nMinSize = pDefinition->m_nMinSize;
			}
			else
			{
				int num10 = (int)(max((double) (availableSize - num2), (double) 0) * (nMeasureSize / pDefinition->m_nSizeCache));
				nMinSize = min(num10, pDefinition->GetUserMaxSize());
				nMinSize = max(pDefinition->m_nMinSize, nMinSize);
			}
			pDefinition->m_nMeasureSize = nMinSize;
			num2 += nMinSize;
		}
	}

	delete[] pTempDefinitions;
}


CSize CXTPMarkupGrid::MeasureOverride(CXTPMarkupDrawingContext* pDC, CSize constraint)
{
	CSize size(0, 0);

	if (m_pColumnDefinitions->GetCount() == 0 && m_pRowDefinitions->GetCount() == 0)
	{
		int nCount = m_pChildren->GetCount();
		for (int i = 0; i < nCount; i++)
		{
			CXTPMarkupUIElement* pElement = m_pChildren->GetItem(i);
			if (pElement == NULL)
				continue;

			pElement->Measure(pDC, constraint);

			size.cx = max(size.cx, pElement->GetDesiredSize().cx);
			size.cy = max(size.cy, pElement->GetDesiredSize().cy);
		}

		return size;
	}

	BOOL bTreatStarAsAuto = IsPositiveInfinity(constraint.cx);
	BOOL bFlag2 = IsPositiveInfinity(constraint.cy);

	ValidateDefinitions();
	ValidateDefinitionsLayout(m_pDefinitionsU, bTreatStarAsAuto);
	ValidateDefinitionsLayout(m_pDefinitionsV, bFlag2);

	ValidateCells();

	MeasureCellsGroup(pDC, m_nCellGroup[0], constraint, FALSE, FALSE);

	if (!m_bHasGroup2CellsInAutoRows)
	{
		if (m_bHasStarCellsV)
		{
			ResolveStar(m_pDefinitionsV, constraint.cy);
		}
		MeasureCellsGroup(pDC, m_nCellGroup[1], constraint, FALSE, FALSE);
		if (m_bHasStarCellsU)
		{
			ResolveStar(m_pDefinitionsU, constraint.cx);
		}
		MeasureCellsGroup(pDC, m_nCellGroup[2], constraint, FALSE, FALSE);
	}
	else if (m_nCellGroup[1] > GetChildren()->GetCount())
	{
		if (m_bHasStarCellsU)
		{
			ResolveStar(m_pDefinitionsU, constraint.cx);
		}
		MeasureCellsGroup(pDC, m_nCellGroup[2], constraint, FALSE, FALSE);
		if (m_bHasStarCellsV)
		{
			ResolveStar(m_pDefinitionsV, constraint.cy);
		}
	}
	else
	{
		MeasureCellsGroup(pDC, m_nCellGroup[1], constraint, FALSE, TRUE);
		if (m_bHasStarCellsU)
		{
			ResolveStar(m_pDefinitionsU, constraint.cx);
		}
		MeasureCellsGroup(pDC, m_nCellGroup[2], constraint, FALSE, FALSE);
		if (m_bHasStarCellsV)
		{
			ResolveStar(m_pDefinitionsV, constraint.cy);
		}
		MeasureCellsGroup(pDC, m_nCellGroup[1], constraint, TRUE, FALSE);
	}

	MeasureCellsGroup(pDC, m_nCellGroup[3], constraint, FALSE, FALSE);
	size = CSize(CalculateDesiredSize(m_pDefinitionsU), CalculateDesiredSize(m_pDefinitionsV));

	return size;
}


int _cdecl CXTPMarkupGrid::DistributionOrderComparer(const void *arg1, const void *arg2)
{
	CXTPMarkupDefinitionBase* base2 = *((CXTPMarkupDefinitionBase**)arg1);
	CXTPMarkupDefinitionBase* base3 = *((CXTPMarkupDefinitionBase**)arg2);

	if ((base2->m_nSizeCache - base2->m_nMinSize) == (base3->m_nSizeCache - base3->m_nMinSize))
	{
		return 0;
	}
	else if ((base2->m_nSizeCache - base2->m_nMinSize) < (base3->m_nSizeCache - base3->m_nMinSize))
	{
		return -1;
	}
	return 1;
};

int _cdecl CXTPMarkupGrid::StarDistributionOrderComparer(const void *arg1, const void *arg2)
{
	CXTPMarkupDefinitionBase* base2 = *((CXTPMarkupDefinitionBase**)arg1);
	CXTPMarkupDefinitionBase* base3 = *((CXTPMarkupDefinitionBase**)arg2);

	if (base2->m_nSizeCache == base3->m_nSizeCache)
	{
		return 0;
	}
	else if (base2->m_nSizeCache < base3->m_nSizeCache)
	{
		return -1;
	}
	return 1;
};

int _cdecl CXTPMarkupGrid::SpanMaxDistributionOrderComparer(const void *arg1, const void *arg2)
{
	CXTPMarkupDefinitionBase* base2 = *((CXTPMarkupDefinitionBase**)arg1);
	CXTPMarkupDefinitionBase* base3 = *((CXTPMarkupDefinitionBase**)arg2);

	if (base2 == base3)
		return 0;

	if (base2->GetUserSize()->IsAuto())
	{
		if (base3->GetUserSize()->IsAuto() && (base2->m_nSizeCache <= base3->m_nSizeCache))
			return -1;

		return 1;
	}

	if (base3->GetUserSize()->IsAuto())
	{
		return -1;
	}

	if (base2->m_nSizeCache <= base3->m_nSizeCache)
	{
		return -1;
	}
	return 1;

};

int _cdecl CXTPMarkupGrid::SpanPreferredDistributionOrderComparer(const void *arg1, const void *arg2)
{
	CXTPMarkupDefinitionBase* base2 = *((CXTPMarkupDefinitionBase**)arg1);
	CXTPMarkupDefinitionBase* base3 = *((CXTPMarkupDefinitionBase**)arg2);

	if (base2 == base3)
		return 0;

	if (base2->GetUserSize()->IsAuto())
	{
		if (!base3->GetUserSize()->IsAuto())
			return -1;

		if (base2->m_nMinSize <= base3->m_nMinSize)
			return -1;

		return 1;
	}

	if (!base3->GetUserSize()->IsAuto() && (base2->GetPreferredSize() <= base3->GetPreferredSize()))
	{
		return -1;
	}

	return 1;

};


void CXTPMarkupGrid::SetFinalSize(CXTPMarkupDefinitionCollection* pDefinitions, int finalSize)
{
	CXTPMarkupDefinitionBase** pTempDefinitions = new CXTPMarkupDefinitionBase*[pDefinitions->GetCount()];

	int length = 0;
	int num2 = pDefinitions->GetCount();
	double num3 = 0;
	for (int i = 0; i < pDefinitions->GetCount(); i++)
	{
		CXTPMarkupDefinitionBase* pDefinition = pDefinitions->GetItem(i);

		if (pDefinition->GetUserSize()->IsStar())
		{
			pTempDefinitions[length++] = pDefinition;
			double d = pDefinition->GetUserSize()->GetValue();
			if (d == 0)
			{
				pDefinition->m_nMeasureSize = 0;
				pDefinition->m_nSizeCache = 0;
			}
			else
			{
				pDefinition->m_nMeasureSize = d;
				int num6 = IsNan(pDefinition->GetUserMaxSize()) ? pDefinition->m_nMinSize : max(pDefinition->m_nMinSize, pDefinition->GetUserMaxSize());
				pDefinition->m_nSizeCache = (double)num6 / (double)d;
			}
		}
		else
		{
			pTempDefinitions[--num2] = pDefinition;
			double nMinSizeForArrange = 0;
			switch (pDefinition->GetUserSize()->GetUnitType())
			{
			case CXTPMarkupGridLength::unitTypeAuto:
				nMinSizeForArrange = pDefinition->m_nMinSize;
				break;

			case CXTPMarkupGridLength::unitTypePixel:
				nMinSizeForArrange = pDefinition->GetUserSize()->GetValue();
				break;
			}
			int nUserMaxSize = pDefinition->GetUserMaxSize();
			pDefinition->m_nSizeCache = max(pDefinition->m_nMinSize, min(nMinSizeForArrange, nUserMaxSize));
			num3 += pDefinition->m_nSizeCache;
		}
	}
	if (length > 0)
	{
		qsort(pTempDefinitions, length, sizeof(CXTPMarkupDefinitionBase*), StarDistributionOrderComparer);

		double num9 = 0;
		int nIndex;
		for (nIndex = length - 1; nIndex >= 0; nIndex--)
		{
			num9 += pTempDefinitions[nIndex]->m_nMeasureSize;
			pTempDefinitions[nIndex]->m_nSizeCache = num9;
		}
		for (nIndex = 0; nIndex < length; nIndex++)
		{
			CXTPMarkupDefinitionBase* pDefinition = pTempDefinitions[nIndex];

			int num11;
			double nMeasureSize = pDefinition->m_nMeasureSize;
			if (nMeasureSize == 0)
			{
				num11 = pDefinition->m_nMinSize;
			}
			else
			{
				double num13 = max((double) (finalSize - num3), (double) 0) * (nMeasureSize / pDefinition->m_nSizeCache);
				num11 = min((int)num13, pDefinition->GetUserMaxSize());
				num11 = max(pDefinition->m_nMinSize, num11);
			}
			pDefinition->m_nSizeCache = num11;
			num3 += num11;
		}
	}
	if (num3 > finalSize)
	{
		qsort(pTempDefinitions, length, sizeof(CXTPMarkupDefinitionBase*), DistributionOrderComparer);

		double num14 = finalSize - num3;
		for (int k = 0; k < pDefinitions->GetCount(); k++)
		{
			double num16 = pTempDefinitions[k]->m_nSizeCache + (num14 / ((double) (pDefinitions->GetCount() - k)));
			num16 = min(max(num16, pTempDefinitions[k]->m_nMinSize), pTempDefinitions[k]->m_nSizeCache);
			num14 -= num16 - pTempDefinitions[k]->m_nSizeCache;
			pTempDefinitions[k]->m_nSizeCache = num16;
		}
	}
	pDefinitions->GetItem(0)->m_nFinalOffset = 0;
	for (int j = 1; j < pDefinitions->GetCount(); j++)
	{
		pDefinitions->GetItem(j)->m_nFinalOffset = pDefinitions->GetItem(j - 1)->m_nFinalOffset + (int)pDefinitions->GetItem(j - 1)->m_nSizeCache;
	}

	delete[] pTempDefinitions;
}


int CXTPMarkupGrid::GetFinalSizeForRange(CXTPMarkupDefinitionCollection* pDefinitions, int start, int count)
{
	int num = 0;
	for (int nIndex = start; nIndex < start + count; nIndex++)
	{
		num += (int)pDefinitions->GetItem(nIndex)->m_nSizeCache;
	}
	return num;
}

CSize CXTPMarkupGrid::ArrangeOverride(CSize arrangeSize)
{
	if ((m_pColumnDefinitions->GetCount() == 0 && m_pRowDefinitions->GetCount() == 0) || (m_pCellCachesCollection == NULL))
	{
		int nCount = m_pChildren->GetCount();
		for (int i = 0; i < nCount; i++)
		{
			CXTPMarkupUIElement* pElement = m_pChildren->GetItem(i);
			if (pElement == NULL)
				continue;

			pElement->Arrange(CRect(0, 0, arrangeSize.cx, arrangeSize.cy));
		}
		return arrangeSize;
	}

	SetFinalSize(m_pDefinitionsU, arrangeSize.cx);
	SetFinalSize(m_pDefinitionsV, arrangeSize.cy);
	int nCount = m_pChildren->GetCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPMarkupUIElement* pElement = m_pChildren->GetItem(i);
		if (pElement == NULL)
			continue;

		CELLCACHE& cellCache =  m_pCellCachesCollection[i];

		int columnIndex = cellCache.nColumnIndex;
		int rowIndex = cellCache.nRowIndex;
		int columnSpan = cellCache.nColumnSpan;
		int rowSpan = cellCache.nRowSpan;

		CRect finalRect(CPoint(m_pDefinitionsU->GetItem(columnIndex)->m_nFinalOffset,
			m_pDefinitionsV->GetItem(rowIndex)->m_nFinalOffset),
			CSize(GetFinalSizeForRange(m_pDefinitionsU, columnIndex, columnSpan),
			GetFinalSizeForRange(m_pDefinitionsV, rowIndex, rowSpan)));

		pElement->Arrange(finalRect);
	}

	return arrangeSize;
}

