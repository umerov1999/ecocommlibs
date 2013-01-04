// XTPCalendarEventLabel.cpp: implementation of the CXTPCalendarEventLabel and
// CXTPCalendarEventLabels classes.
//
// This file is a part of the XTREME CALENDAR MFC class library.
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
#include "Resource.h"

#include "Common/Resource.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPPropExchange.h"
#include "Common/XTPVC80Helpers.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPSmartPtrInternalT.h"

#include "XTPCalendarPtrCollectionT.h"
#include "XTPCalendarEventLabel.h"
#include "XTPCalendarDefines.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CXTPCalendarEventLabel, CCmdTarget)
IMPLEMENT_DYNAMIC(CXTPCalendarEventLabels, CCmdTarget)

IMPLEMENT_DYNCREATE(CXTPCalendarEventCategory, CCmdTarget)
IMPLEMENT_DYNCREATE(CXTPCalendarEventCategories, CCmdTarget)

IMPLEMENT_DYNAMIC(CXTPCalendarUIntArray,    CCmdTarget)
IMPLEMENT_DYNAMIC(CXTPCalendarIconIDs,          CXTPCalendarUIntArray)
IMPLEMENT_DYNAMIC(CXTPCalendarEventCategoryIDs, CXTPCalendarUIntArray)

////////////////////////////////////////////////////////////////////////////
CXTPCalendarEventLabel::CXTPCalendarEventLabel(int nID, COLORREF clrColor, LPCTSTR pcszName)
{
	m_nLabelID = nID;
	m_clrColor = clrColor;
	m_strName = pcszName;

}

////////////////////////////////////////////////////////////////////////////
CXTPCalendarEventLabels::CXTPCalendarEventLabels()
{

}

CXTPCalendarEventLabels::~CXTPCalendarEventLabels()
{
	RemoveAll();
}

CXTPCalendarEventLabel* CXTPCalendarEventLabels::Find(int nLabelID) const
{
	int nFIdx = FindIndex(nLabelID);
	if (nFIdx >= 0)
	{
		return GetAt(nFIdx);
	}
	return NULL;
}

int CXTPCalendarEventLabels::FindIndex(int nLabelID) const
{
	int nCount = GetCount();

	if (nLabelID >= 0 && nLabelID < nCount)
	{
		CXTPCalendarEventLabel* pLabel = GetAt(nLabelID);

		if (pLabel && pLabel->m_nLabelID == nLabelID)
		{
			return nLabelID;
		}
	}

	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarEventLabel* pLabel = GetAt(i);

		if (pLabel && pLabel->m_nLabelID == nLabelID)
		{
			return i;
		}
	}

	return -1;
}

void CXTPCalendarEventLabels::InitDefaultValues()
{
	RemoveAll();

	//XTP_CALENDAR_NONE_LABEL_ID = 0
	CXTPCalendarEventLabel* pLabel;
	pLabel = new CXTPCalendarEventLabel(0, RGB(255, 255, 255), XTPResourceManager()->LoadString(XTP_IDS_CALENDAR_EVENT_LABEL_NONE));
	InsertAt(0, pLabel);

	pLabel = new CXTPCalendarEventLabel(1, RGB(255, 148, 132), XTPResourceManager()->LoadString(XTP_IDS_CALENDAR_EVENT_LABEL_IMPORTANT));
	InsertAt(1, pLabel);

	pLabel = new CXTPCalendarEventLabel(2, RGB(132, 156, 231), XTPResourceManager()->LoadString(XTP_IDS_CALENDAR_EVENT_LABEL_BUSINESS));
	InsertAt(2, pLabel);

	pLabel = new CXTPCalendarEventLabel(3, RGB(165, 222, 99), XTPResourceManager()->LoadString(XTP_IDS_CALENDAR_EVENT_LABEL_PERSONAL));
	InsertAt(3, pLabel);

	pLabel = new CXTPCalendarEventLabel(4, RGB(231, 231, 214), XTPResourceManager()->LoadString(XTP_IDS_CALENDAR_EVENT_LABEL_VACATION));
	InsertAt(4, pLabel);

	pLabel = new CXTPCalendarEventLabel(5, RGB(255, 181, 115), XTPResourceManager()->LoadString(XTP_IDS_CALENDAR_EVENT_LABEL_MUSTATTEND));
	InsertAt(5, pLabel);

	pLabel = new CXTPCalendarEventLabel(6, RGB(132, 239, 247), XTPResourceManager()->LoadString(XTP_IDS_CALENDAR_EVENT_LABEL_TRAVELREQUIRED));
	InsertAt(6, pLabel);

	pLabel = new CXTPCalendarEventLabel(7, RGB(214, 206, 132), XTPResourceManager()->LoadString(XTP_IDS_CALENDAR_EVENT_LABEL_NEEDSPREPARATION));
	InsertAt(7, pLabel);

	pLabel = new CXTPCalendarEventLabel(8, RGB(198, 165, 247), XTPResourceManager()->LoadString(XTP_IDS_CALENDAR_EVENT_LABEL_BIRTHDAY));
	InsertAt(8, pLabel);

	pLabel = new CXTPCalendarEventLabel(9, RGB(165, 206, 198), XTPResourceManager()->LoadString(XTP_IDS_CALENDAR_EVENT_LABEL_ANNIVERSARY));
	InsertAt(9, pLabel);

	pLabel = new CXTPCalendarEventLabel(10, RGB(255, 231, 115), XTPResourceManager()->LoadString(XTP_IDS_CALENDAR_EVENT_LABEL_PHONECALL));
	InsertAt(10, pLabel);
}

//===========================================================================
// CXTPCalendarEventCategory
CXTPCalendarEventCategory::CXTPCalendarEventCategory(UINT nID, LPCTSTR pcszName,
							COLORREF clrBorder, COLORREF clrBkBase)
{
	m_nID = nID;
	m_strName = pcszName;

	m_clrBorder = clrBorder;
	m_clrBkBase = clrBkBase;

}

CXTPCalendarEventCategory::~CXTPCalendarEventCategory()
{
}

CXTPPaintManagerColorGradient& CXTPCalendarEventCategory::GetBackgroundColor()
{
	if (m_grclrBackground.IsNull())
	{
		m_grclrBackground.clrLight.SetStandardValue(XTPDrawHelpers()->LightenColor(156, m_clrBkBase));// 156 - 242, 178, 179;
		m_grclrBackground.clrDark.SetStandardValue(XTPDrawHelpers()->LightenColor(56, m_clrBkBase));  // 56  - 229,100, 107
	}

	return m_grclrBackground;
}

COLORREF CXTPCalendarEventCategory::GetBkColorAllDayEvents() const
{
	return m_clrBkBase;
}

COLORREF CXTPCalendarEventCategory::GetBkColorWorkCell() const
{
	return XTPDrawHelpers()->LightenColor(185, m_clrBkBase); // 185 - 245, 201, 203
}

COLORREF CXTPCalendarEventCategory::GetBkColorNonWorkCell() const
{
	return XTPDrawHelpers()->LightenColor(115, m_clrBkBase); // 115 - 237, 146, 150
}

COLORREF CXTPCalendarEventCategory::GetColorCellBorder() const
{
	return XTPDrawHelpers()->LightenColor(45, m_clrBkBase); // 45 - 227, 91, 98
}


CXTPCalendarEventCategories::CXTPCalendarEventCategories()
{
	InitDefaultValues();

}

CXTPCalendarEventCategories::~CXTPCalendarEventCategories()
{
}

CXTPCalendarEventCategory* CXTPCalendarEventCategories::Find(UINT nID) const
{
	int nFIdx = FindIndex(nID);
	if (nFIdx >= 0)
	{
		return GetAt(nFIdx);
	}
	return NULL;
}

int CXTPCalendarEventCategories::FindIndex(UINT nID) const
{
	int nCount = GetCount();

	//---------------------------------------------------------
	int nDirectIdx = (int)(nID - 1);

	if (nDirectIdx >= 0 && nDirectIdx < nCount)
	{
		CXTPCalendarEventCategory* pEvCat = GetAt(nDirectIdx);

		if (pEvCat && pEvCat->GetID() == nID)
		{
			return nDirectIdx;
		}
	}

	//---------------------------------------------------------
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarEventCategory* pEvCat = GetAt(i);

		if (pEvCat && pEvCat->GetID() == nID)
		{
			return i;
		}
	}

	return -1;
}

CString CXTPCalendarEventCategories::LoadCategoryName(UINT nIDResource) const
{
	CString strFormat = XTPResourceManager()->LoadString(XTP_IDS_CALENDAR_CATEGORY);
	CString strColor = XTPResourceManager()->LoadString(nIDResource);

	CString strCategory;
	strCategory.Format(strFormat, (LPCTSTR)strColor);

	return strCategory;
}

void CXTPCalendarEventCategories::InitDefaultValues()
{
	RemoveAll();

	CXTPCalendarEventCategory* pEvCat;

	//---------------------------------------
	pEvCat = new CXTPCalendarEventCategory(xtpCalendarEventCategoryYellow, LoadCategoryName(XTP_IDS_CLR_YELLOW),
		RGB(154, 149, 25), RGB(255, 251, 155));//RGB(255, 252, 179), RGB(255,249, 91));
	Add(pEvCat);
	//---------------------------------------
	pEvCat = new CXTPCalendarEventCategory(xtpCalendarEventCategoryGreen, LoadCategoryName(XTP_IDS_CLR_GREEN),
		RGB(55, 126, 45), RGB(96, 194, 82)); //RGB(196, 232, 190), RGB(136, 210, 127));
	Add(pEvCat);
	//---------------------------------------
	pEvCat = new CXTPCalendarEventCategory(xtpCalendarEventCategoryRed, LoadCategoryName(XTP_IDS_CLR_RED),
		RGB(167, 29, 35), RGB(220, 56, 65)); //RGB(242, 178, 179), RGB(229,100, 107));
	Add(pEvCat);
	//---------------------------------------
	pEvCat = new CXTPCalendarEventCategory(xtpCalendarEventCategoryPink, LoadCategoryName(XTP_IDS_CLR_PINK),
		RGB(77, 49, 141), RGB(184, 164, 224)); //RGB(202, 187, 232), RGB(150, 125, 210));
	Add(pEvCat);
	//---------------------------------------
	pEvCat = new CXTPCalendarEventCategory(xtpCalendarEventCategoryOrange, LoadCategoryName(XTP_IDS_CLR_ORANGE),
		RGB(173, 77, 13), RGB(247, 187, 137)); //RGB(249, 204, 167), RGB(244, 152, 90));
	Add(pEvCat);
	//---------------------------------------
	pEvCat = new CXTPCalendarEventCategory(xtpCalendarEventCategoryBlue, LoadCategoryName(XTP_IDS_CLR_BLUE),
		RGB(40, 81, 142), RGB(159, 183, 232)); // RGB(183, 201, 238), RGB(119, 158, 218));
	Add(pEvCat);
}


//===========================================================================
int CXTPCalendarUIntArray::Find(UINT uID) const
{
	return TBase::FindElement(uID);
}

BOOL CXTPCalendarUIntArray::RemoveID(UINT uID)
{
	return TBase::RemoveElement(uID);
}

CString CXTPCalendarUIntArray::SaveToString() const
{
	CString strData;

	TCHAR szNumber[34];

	int nCount = GetSize();
	for (int i = 0; i < nCount; i++)
	{
		UINT uElement = GetAt(i);

		ITOT_S((int)uElement, szNumber, _countof(szNumber));
		szNumber[33] = 0;

		if (!strData.IsEmpty())
		{
			strData += _T(",");
		}

		strData += szNumber;
	}

	return strData;
}

void CXTPCalendarUIntArray::LoadFromString(LPCTSTR pcszData)
{
	RemoveAll();

	CString strData = pcszData;

	while (!strData.IsEmpty())
	{
		CString strNumber = strData.SpanExcluding(_T(","));

		UINT uElement = (UINT)_ttoi(strNumber);
		Add(uElement);

		int nDelCount = strNumber.GetLength() + 1;
		nDelCount = min(nDelCount, strData.GetLength());
		strData.Delete(0, nDelCount);
	}
}

void CXTPCalendarUIntArray::DoPropExchange(CXTPPropExchange* pPX, LPCTSTR pcszSection, LPCTSTR pcszElement)
{
	if (!pPX || !pcszSection || !pcszElement)
	{
		ASSERT(FALSE);
		return;
	}

	long nVersion = 1;

	if (pPX->IsStoring())
	{
		CXTPPropExchangeSection secArray(pPX->GetSection(pcszSection));
		secArray->EmptySection();

		PX_Long(&secArray, _T("Version"), nVersion, 1);

		int nCount = GetSize();
		CXTPPropExchangeEnumeratorPtr pEnumerator(secArray->GetEnumerator(pcszElement));
		POSITION posStorage = pEnumerator->GetPosition(nCount);

		for (int i = 0; i < nCount; i++)
		{
			ULONG ulElement = (ULONG)GetAt(i);

			CXTPPropExchangeSection secElement(pEnumerator->GetNext(posStorage));
			PX_ULong(&secElement, _T("Value"), ulElement);
		}
	}
	else if (pPX->IsLoading())
	{
		RemoveAll();

		CXTPPropExchangeSection secArray(pPX->GetSection(pcszSection));

		PX_Long(&secArray, _T("Version"), nVersion, 1);

		if (nVersion != 1)
		{
			TRACE(_T("ERROR! CXTPCalendarIconIDs::DoPropExchange: Unsupported data version: %d \n"), nVersion);
			return;
		}

		CXTPPropExchangeEnumeratorPtr pEnumerator(secArray->GetEnumerator(pcszElement));
		POSITION posStorage = pEnumerator->GetPosition();

		while (posStorage)
		{
			ULONG ulElement = 0;

			CXTPPropExchangeSection secElement(pEnumerator->GetNext(posStorage));
			PX_ULong(&secElement, _T("Value"), ulElement, 0);

			Add((ULONG)ulElement);
		}
	}
	else
	{
		ASSERT(FALSE);
	}
}

//===========================================================================
CXTPCalendarIconIDs::CXTPCalendarIconIDs()
{

}

CXTPCalendarIconIDs::~CXTPCalendarIconIDs()
{
}

//===========================================================================
CXTPCalendarEventCategoryIDs::CXTPCalendarEventCategoryIDs()
{
}

CXTPCalendarEventCategoryIDs::~CXTPCalendarEventCategoryIDs()
{
}


/////////////////////////////////////////////////////////////////////////////
