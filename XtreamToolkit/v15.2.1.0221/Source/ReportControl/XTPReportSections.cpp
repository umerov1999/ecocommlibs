// XTPReportSections.cpp : implementation of the CXTPReportSections class.
//
// This file is a part of the XTREME REPORTCONTROL MFC class library.
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

#include "Common/XTPDrawHelpers.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPPropExchange.h"
#include "Common/XTPToolTipContext.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPMarkupRender.h"
#include "Common/XTPCustomHeap.h"
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPColorManager.h"

#include "XTPReportDefines.h"
#include "XTPReportControl.h"
#include "XTPReportSection.h"
#include "XTPReportSections.h"
#include "XTPReportRecord.h"
#include "XTPReportRecords.h"
#include "XTPReportRows.h"
#include "XTPReportRow.h"
#include "XTPReportPaintManager.h"


CXTPReportSections::CXTPReportSections(CXTPReportControl *pControl)
	: m_pControl  (pControl)
	, m_rcSections(0,0,0,0)
{
	Add(new CXTPReportSection(pControl, xtpRowTypeHeader));
	Add(new CXTPReportSection(pControl, xtpRowTypeBody));
	Add(new CXTPReportSection(pControl, xtpRowTypeFooter));
}

CXTPReportSections::~CXTPReportSections()
{
	for (int nSection=0; nSection<GetSize(); nSection++)
	{
		CXTPReportSection *pSection = GetAt(nSection);
		SAFE_DELETE(pSection);
	}

	RemoveAll();
}


CXTPReportSection *CXTPReportSections::FindSection(CXTPReportRow *pRow) const
{
	for (int nSection=0; nSection<GetSize(); nSection++)
	{
		CXTPReportSection *pSection = GetAt(nSection);

		if (pSection->GetRows()->Contains(pRow))
		{
			return pSection;
		}
	}

	return NULL;
}


BOOL CXTPReportSections::EnsureVisible(CDC *pDC, CXTPReportRow *pRow)
{
	// None of the parameters may be null
	ASSERT(NULL != pDC);
	ASSERT(NULL != pRow);

	BOOL bResult = FALSE;

	if (NULL != pRow)
	{
		CXTPReportSection *pSection = pRow->GetSection();
		ASSERT(NULL != pSection); // Section must always be valid

		if (NULL != pSection)
		{
			bResult = pSection->EnsureVisible(pDC, pRow);
		}
	}

	return bResult;
}


CXTPReportRow* CXTPReportSections::GetFocusedRow() const
{
	CXTPReportRow *pFocusedRow = NULL;

	for (int nSection=0; nSection<GetSize(); nSection++)
	{
		CXTPReportSection *pSection = GetAt(nSection);
		pFocusedRow = pSection->GetRows()->GetFocusedRow();

		if (NULL != pFocusedRow)
		{
			return pFocusedRow;
		}
	}

	return NULL;
}


CRect CXTPReportSections::GetRect() const
{
	return m_rcSections;
}


CXTPReportSection* CXTPReportSections::HitTest(CPoint pt) const
{
	CXTPReportSection *pHitSection = NULL;

	for (int nSection=0; nSection<GetSize(); nSection++)
	{
		CXTPReportSection *pSection = GetAt(nSection);

		if (pSection->m_rcSection.PtInRect(pt))
		{
			ASSERT(NULL == pHitSection); // There can only be one
			pHitSection = pSection;
		}
	}

	return pHitSection;
}


void CXTPReportSections::Draw(CDC *pDC)
{
	for (int nSection=0; nSection<GetSize(); nSection++)
	{
		CXTPReportSection *pSection = GetAt(nSection);

		if (pSection->IsVisible())
		{
			pSection->Draw(pDC);
		}
	}
}


void CXTPReportSections::AdjustLayout(CDC *pDC, CRect rcSections)
{
	m_rcSections = rcSections;

	CArray<int, int> arrHeightSection;
	CArray<int ,int> arrHeightDivider;

	arrHeightSection.SetSize(GetSize());
	arrHeightDivider.SetSize(GetSize());

	int nSection;

	//////////////////////////////////////////////////////////////////////////
	// Calculate height of sections
	//////////////////////////////////////////////////////////////////////////

	for (nSection=0; nSection<GetSize(); nSection++)
	{
		CXTPReportSection *pSection = GetAt(nSection);

		int nHeightSection = 0; // Height of the section in pixels
		int nHeightDivider = 0; // Height of the divider in pixels

		if (pSection->IsVisible())
		{
			// Divider
			XTPReportFixedRowsDividerStyle dividerStyle = pSection->GetDividerStyle();
			nHeightDivider = m_pControl->GetPaintManager()->GetSectionDividerHeight(dividerStyle);

			// Section
			switch (pSection->GetHeightMode())
			{
				case xtpReportSectionHeightModeFixed:
				{
					nHeightSection = pSection->GetHeight();
				}
				break;

				case xtpReportSectionHeightModeRelative:
				{
					double dPercent = rcSections.Height() / 100.0; // Pixels per percent
					nHeightSection = static_cast<int>(dPercent * pSection->GetHeight());
				}
				break;

				case xtpReportSectionHeightModeAuto:
				case xtpReportSectionHeightModeAutoExpand:
				case xtpReportSectionHeightModeAutoShrink:
				{
					nHeightSection = pSection->GetTotalHeight(pDC);
				}
				break;

				case xtpReportSectionHeightModeExpand:
				{
					 // Remaining height is calculated in the next step
					nHeightSection = 0;
				}
				break;
			}
		}

		arrHeightSection[nSection] = nHeightSection;
		arrHeightDivider[nSection] = nHeightDivider;
	}

	//////////////////////////////////////////////////////////////////////////
	// Calculate remaining height
	//////////////////////////////////////////////////////////////////////////

	int nHeightExpand = rcSections.Height();

	for (nSection=0; nSection<GetSize(); nSection++)
	{
		nHeightExpand -= arrHeightSection[nSection];
		nHeightExpand -= arrHeightDivider[nSection];
	}

	if (nHeightExpand < 0)
	{
		// Shrink auto shrink section
		for (nSection=0; nSection<GetSize(); nSection++)
		{
			CXTPReportSection *pSection = GetAt(nSection);

			if (xtpReportSectionHeightModeAutoShrink == pSection->GetHeightMode())
			{
				int nHeightSection = arrHeightSection[nSection];

				nHeightSection += nHeightExpand; // nHeightExpand is negative
				nHeightSection = max(0, nHeightSection);

				arrHeightSection[nSection] = nHeightSection;
			}
		}
	}

	nHeightExpand = max(0, nHeightExpand);

	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////

	for (nSection=0; nSection<GetSize(); nSection++)
	{
		CXTPReportSection *pSection = GetAt(nSection);

		if (xtpReportSectionHeightModeExpand == pSection->GetHeightMode())
		{
			arrHeightSection[nSection] = nHeightExpand;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////

	int nTop = rcSections.top;

	for (nSection=0; nSection<GetSize(); nSection++)
	{
		CXTPReportSection *pSection = GetAt(nSection);

		// Divider at the top
		if (xtpReportSectionDividerStyleNone   != pSection->GetDividerStyle() &&
			 xtpReportSectionDividerPositionTop == pSection->GetDividerPosition())
		{
			pSection->m_rcDivider.left   = rcSections.left;
			pSection->m_rcDivider.right  = rcSections.right;
			pSection->m_rcDivider.top    = nTop;
			pSection->m_rcDivider.bottom = nTop + arrHeightDivider[nSection];

			nTop += arrHeightDivider[nSection];
		}

		pSection->m_rcSection.left   = rcSections.left;
		pSection->m_rcSection.right  = rcSections.right;
		pSection->m_rcSection.top    = nTop;
		pSection->m_rcSection.bottom = nTop + arrHeightSection[nSection];

		nTop += arrHeightSection[nSection];

		// Divider at the bottom
		if (xtpReportSectionDividerStyleNone      != pSection->GetDividerStyle() &&
			 xtpReportSectionDividerPositionBottom == pSection->GetDividerPosition())
		{
			pSection->m_rcDivider.left   = rcSections.left;
			pSection->m_rcDivider.right  = rcSections.right;
			pSection->m_rcDivider.top    = nTop;
			pSection->m_rcDivider.bottom = nTop + arrHeightDivider[nSection];

			nTop += arrHeightDivider[nSection];
		}
	}
}


void CXTPReportSections::SetMarkupContext(CXTPMarkupContext *pMarkupContext)
{
	// Set markup context for all sections
	for (int nSection=0; nSection<GetSize(); nSection++)
	{
		CXTPReportSection *pSection = GetAt(nSection);
		pSection->SetMarkupContext(pMarkupContext);
	}
}


void CXTPReportSections::ReSortRows()
{
	// Re-sort rows of all sections
	for (int nSection=0; nSection<GetSize(); nSection++)
	{
		CXTPReportSection *pSection = GetAt(nSection);
		ASSERT(NULL != pSection);

		if (pSection->IsAllowSort())
		{
			pSection->ReSortRows();
		}
	}

	m_pControl->AdjustIndentation();
	m_pControl->AdjustLayout();
	m_pControl->RedrawControl();
}


void CXTPReportSections::RefreshIndexes(BOOL bAdjustLayout, BOOL bReverseOrder)
{
	// Re-sort rows of all sections
	for (int nSection=0; nSection<GetSize(); nSection++)
	{
		CXTPReportSection *pSection = GetAt(nSection);
		ASSERT(NULL != pSection);

		pSection->RefreshIndexes(FALSE, bReverseOrder);
	}

	if (bAdjustLayout)
	{
		m_pControl->AdjustScrollBars();
	}
}
