// XTPChartPanel.cpp
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

#include "Common/XTPPropExchange.h"

#include "Types/XTPChartTypes.h"

#include "XTPChartDefines.h"
#include "XTPChartElement.h"
#include "XTPChartPanel.h"
#include "XTPChartTitle.h"


//////////////////////////////////////////////////////////////////////////
// CXTPChartPanel

IMPLEMENT_DYNAMIC(CXTPChartPanel, CXTPChartElement);

CXTPChartPanel::CXTPChartPanel()
{
	m_pTitles = new CXTPChartTitleCollection(this);

}

CXTPChartPanel::~CXTPChartPanel()
{
	SAFE_RELEASE(m_pTitles);
}

void CXTPChartPanel::Remove()
{
	if (m_pOwner)
	{
		CXTPChartElementCollection* pCollection = STATIC_DOWNCAST(CXTPChartElementCollection, m_pOwner);
		pCollection->Remove(this);
	}
}

void CXTPChartPanel::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPPropExchangeSection secTitles(pPX->GetSection(_T("Titles")));
	m_pTitles->DoPropExchange(&secTitles);

}

//////////////////////////////////////////////////////////////////////////
// CXTPChartTitleCollection

CXTPChartPanelCollection::CXTPChartPanelCollection(CXTPChartContent* pOwner)
{
	m_pOwner = (CXTPChartElement*)pOwner;


}

CXTPChartPanelCollection::~CXTPChartPanelCollection()
{
}

CXTPChartPanel* CXTPChartPanelCollection::Add(CXTPChartPanel* pPanel)
{
	InsertAt(GetCount(), pPanel);
	return pPanel;
}

void CXTPChartPanelCollection::DoPropExchange(CXTPPropExchange* pPX)
{
	if (pPX->IsLoading())
	{
		RemoveAll();

		CXTPPropExchangeEnumeratorPtr pEnumerator(pPX->GetEnumerator(_T("Panel")));
		POSITION pos = pEnumerator->GetPosition(0);

		while (pos)
		{
			CXTPChartPanel* pPanel = NULL;

			CXTPPropExchangeSection sec(pEnumerator->GetNext(pos));
			PX_Object(&sec, pPanel, RUNTIME_CLASS(CXTPChartPanel));

			if (!pPanel)
				AfxThrowArchiveException(CArchiveException::badClass);

			Add(pPanel);
		}
	}
	else
	{
		CXTPPropExchangeEnumeratorPtr pEnumerator(pPX->GetEnumerator(_T("Panel")));
		POSITION pos = pEnumerator->GetPosition((int)m_arrElements.GetSize());

		for (int i = 0; i < GetCount(); i++)
		{
			CXTPPropExchangeSection sec(pEnumerator->GetNext(pos));
			PX_Object(&sec, m_arrElements[i], RUNTIME_CLASS(CXTPChartPanel));
		}

	}
}




