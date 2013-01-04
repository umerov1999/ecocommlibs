// XTPReportRecordTrack.cpp : implementation of the CXTPReportRecordTrackMod class.
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

#include "stdafx.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPPropExchange.h"
#include "Common/XTPMarkupRender.h"
#include "Common/XTPCustomHeap.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPColorManager.h"

#include "../XTPReportDefines.h"
#include "../XTPReportControl.h"


#include "../XTPReportRecordItem.h"

#include "XTPTrackBlock.h"
#include "XTPTrackControlItem.h"
#include "XTPTrackControl.h"
#include "XTPTrackUndoManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_SERIAL(CXTPTrackBlock, CCmdTarget, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPTrackBlock::CXTPTrackBlock()
{
	m_nIndex = -1;
	m_nMRUPosition = m_nPosition = 0;
	m_nLength = 0;

	m_bLocked = FALSE;

	m_rcBlock.SetRectEmpty();

	m_clrBlock = RGB(240, 158, 159);
	m_pItem = 0;

	m_nLastDragTime = 0;

	m_dHeightPercent = 1;
	m_nHeightFixed = 0;

	m_nVerticalAlignment = DT_VCENTER;

	m_nMinLength = 1;
	m_nMaxLength = 1000;

	m_bDragging = FALSE;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}


void CXTPTrackBlock::SetPosition(int nPosition)
{
	if (m_nMRUPosition == nPosition)
		return;

	if (m_pItem && m_pItem->GetTrackControl())
	{
		m_pItem->GetTrackControl()->GetUndoManager()->AddUndoCommand(new CXTPTrackUndoSetBlockPositionCommand(this, m_nMRUPosition, m_nLength));
	}

	m_nPosition = m_nMRUPosition = nPosition;

	if (m_pItem) m_pItem->RedrawControl();

}

void CXTPTrackBlock::SetLength(int nLength)
{
	if (m_nLength == nLength)
		return;

	if (m_pItem && m_pItem->GetTrackControl())
	{
		m_pItem->GetTrackControl()->GetUndoManager()->AddUndoCommand(new CXTPTrackUndoSetBlockPositionCommand(this, m_nMRUPosition, m_nLength));
	}

	m_nLength = nLength;

	if (m_pItem) m_pItem->RedrawControl();
}

BOOL CXTPTrackBlock::IsLocked() const
{
	return m_bLocked || m_pItem->m_bLocked;
}

int CXTPTrackBlock::Draw(CDC* pDC, CRect rc, BOOL bSelected)
{
	BOOL bLocked = IsLocked();

	int nHeight;

	if (m_nHeightFixed != 0)
		nHeight = m_nHeightFixed;
	else
		nHeight = int((rc.Height()) * m_dHeightPercent);

	if (nHeight != rc.Height())
	{
		if (m_nVerticalAlignment == DT_TOP)
		{
			rc.bottom = rc.top + nHeight;
		}
		else if (m_nVerticalAlignment == DT_BOTTOM)
		{
			rc.top = rc.bottom - nHeight;
		}
		else
		{
			rc.top = (rc.top + rc.bottom - nHeight) / 2;
			rc.bottom = rc.top + nHeight;
		}
	}
	if (m_bLocked)
		bLocked = TRUE;

	CXTPTrackControl* pTrackControl = m_pItem->GetTrackControl();


	rc.left  = pTrackControl->PositionToTrack(m_nPosition);
	rc.right = pTrackControl->PositionToTrack(m_nPosition + m_nLength);


	COLORREF clrBackground = m_clrBlock;
	DWORD dwHSLBackground = CXTPDrawHelpers::RGBtoHSL(clrBackground);
	DWORD dwL = GetBValue(dwHSLBackground);
	DWORD dwLight = (dwL + 240) / 2;

	COLORREF clrLight = CXTPDrawHelpers::HSLtoRGB(RGB(GetRValue(dwHSLBackground), GetGValue(dwHSLBackground), dwLight));
	DWORD dwDark = MulDiv(dwL, 2, 3);
	COLORREF clrDark = CXTPDrawHelpers::HSLtoRGB(RGB(GetRValue(dwHSLBackground), GetGValue(dwHSLBackground), dwDark));


	if (bSelected && !bLocked)
	{
		XTPDrawHelpers()->GradientFill(pDC, rc, clrLight, clrDark, FALSE);
	}
	else
	{
		pDC->FillSolidRect(rc, m_clrBlock);
	}

	if (bLocked)
	{
		pDC->Draw3dRect(rc, GetXtremeColor(RGB(128,128,128)), GetXtremeColor(RGB(128,128,128)));
	}
	else if (bSelected)
	{
		pDC->Draw3dRect(rc, clrLight, clrLight);
	}
	else
	{
		pDC->Draw3dRect(rc, clrLight, clrDark);
	}

	if (!m_strCaption.IsEmpty())
	{
		pDC->SetTextColor(bLocked ? RGB(128,128,128) : RGB(0, 0, 0));

		CRect rcText(rc);
		rcText.DeflateRect(4, 0);

		XTPMarkupDrawText(m_pItem->GetTrackControl()->GetMarkupContext(), pDC->GetSafeHdc(),
			m_strCaption, rcText, DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);
	}

	m_rcBlock = rc;
	return 1;
}

void CXTPTrackBlock::Remove()
{
	if (m_pItem)
	{
		m_pItem->Remove(this);
	}
	else
	{
		InternalRelease();
	}
}

void CXTPTrackBlock::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_String(pPX, _T("ToolTip"), m_strToolTip, _T(""));
	PX_String(pPX, _T("DescriptionText"), m_strDescription, _T(""));
	PX_String(pPX, _T("Caption"), m_strCaption, _T(""));
	PX_Int(pPX, _T("Position"), m_nMRUPosition);
	PX_Int(pPX, _T("Length"), m_nLength, 0);
	PX_Bool(pPX, _T("Locked"), m_bLocked, FALSE);
	PX_DWord(pPX, _T("Color"), m_clrBlock);
	PX_Int(pPX, _T("VerticalAlignment"), m_nVerticalAlignment, DT_VCENTER);
	PX_Double(pPX, _T("HeightPercent"), m_dHeightPercent, 1.0);
	PX_Int(pPX, _T("LastDragTime"), m_nLastDragTime, 0);

	PX_Int(pPX, _T("MinLength"), m_nMinLength, 1);
	PX_Int(pPX, _T("MaxLength"), m_nMaxLength, 1000);
}

void CXTPTrackBlock::Select(BOOL bSelect /* = TRUE */)
{
	CXTPTrackControl* pTrackControl = m_pItem->GetTrackControl();
	if (!pTrackControl)
		return;

	if (bSelect)
		pTrackControl->GetSelectedBlocks()->Add(this);
	else
		pTrackControl->GetSelectedBlocks()->Remove(this);
}

BOOL CXTPTrackBlock::IsSelected() const
{
	CXTPTrackControl* pTrackControl = m_pItem->GetTrackControl();
	if (!pTrackControl)
		return FALSE;

	return pTrackControl->GetSelectedBlocks()->IsSelected(this);
}


//////////////////////////////////////////////////////////////////////////
// CXTPTrackKey

IMPLEMENT_SERIAL(CXTPTrackKey, CXTPTrackBlock, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPTrackKey::CXTPTrackKey()
{

}


int CXTPTrackKey::Draw(CDC* pDC, CRect rc, BOOL bSelected)
{
	CXTPTrackControl* pTrackControl = m_pItem->GetTrackControl();

	rc.left  = pTrackControl->PositionToTrack(m_nPosition) - 4;
	rc.right = rc.left + 8;

	int X = (rc.left + rc.right) / 2;
	int Y = rc.CenterPoint().y;

	if (m_nVerticalAlignment == DT_TOP)
		Y = rc.top + 4;
	if (m_nVerticalAlignment == DT_BOTTOM)
		Y = rc.bottom - 6;

	rc.top = Y - 4;
	rc.bottom = Y + 4;

	COLORREF clrBackground = m_clrBlock;
	DWORD dwHSLBackground = CXTPDrawHelpers::RGBtoHSL(clrBackground);
	DWORD dwL = GetBValue(dwHSLBackground);
	DWORD dwLight = (dwL + 240) / 2;

	COLORREF clrLight = CXTPDrawHelpers::HSLtoRGB(RGB(GetRValue(dwHSLBackground), GetGValue(dwHSLBackground), dwLight));
	DWORD dwDark = MulDiv(dwL, 2, 3);
	COLORREF clrDark = CXTPDrawHelpers::HSLtoRGB(RGB(GetRValue(dwHSLBackground), GetGValue(dwHSLBackground), dwDark));
	COLORREF clrDarkDark = RGB(GetRValue(clrDark) / 2, GetGValue(clrDark) / 2, GetBValue(clrDark) / 2);


	CXTPPenDC pen (*pDC, IsLocked() ? RGB(128,128,128) : bSelected ? clrLight : clrDarkDark);
	CXTPBrushDC brush (*pDC, m_clrBlock);

	CPoint pts[] = {CPoint(X - 4, Y), CPoint(X, Y - 4), CPoint(X + 4, Y), CPoint(X + 4, Y + 4), CPoint(X - 4, Y + 4)};
	pDC->Polygon(pts, 5);

	m_rcBlock = rc;

	return 1;
}


//////////////////////////////////////////////////////////////////////////
// CXTPTrackMarker

IMPLEMENT_SERIAL(CXTPTrackMarker, CCmdTarget, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPTrackMarker::CXTPTrackMarker()
{
	m_rcMarker.SetRectEmpty();
	m_nPosition = 0;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

void CXTPTrackMarker::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_String(pPX, _T("Caption"), m_strCaption);
	PX_Int(pPX, _T("Position"), m_nPosition);
}

void CXTPTrackMarker::SetPosition(int nPosition)
{
	CXTPTrackUndoManager* pUndoManager = m_pControl->GetUndoManager();
	if (pUndoManager)
	{
		pUndoManager->AddUndoCommand(new CXTPTrackUndoSetMarkerPositionCommand(this, nPosition));
	}

	m_nPosition = nPosition;

	if (m_pControl)
	{
		m_pControl->RedrawControl();
	}
}

void CXTPTrackMarker::Remove()
{
	CXTPTrackMarkers* pMarkers = m_pControl->GetMarkers();
	pMarkers->Remove(this);
}

//////////////////////////////////////////////////////////////////////////
// CXTPTrackMarkers

CXTPTrackMarkers::CXTPTrackMarkers(CXTPTrackControl* pControl)
{
	m_pControl = pControl;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPTrackMarkers::~CXTPTrackMarkers()
{
	for (int i = 0; i < m_arrMarkers.GetSize(); i++)
	{
		m_arrMarkers[i]->InternalRelease();
	}
	m_arrMarkers.RemoveAll();
}

CXTPTrackMarker* CXTPTrackMarkers::Add(int nPosition, LPCTSTR lpszCaption)
{
	CXTPTrackMarker* pMarker = new CXTPTrackMarker();
	pMarker->m_nPosition = nPosition;
	pMarker->m_strCaption = lpszCaption;

	return Add(pMarker);
}

CXTPTrackMarker* CXTPTrackMarkers::Add(CXTPTrackMarker* pMarker)
{
	pMarker->m_pControl = m_pControl;

	m_arrMarkers.Add(pMarker);

	m_pControl->GetUndoManager()->AddUndoCommand(new CXTPTrackUndoAddMarkerCommand(pMarker));

	m_pControl->RedrawControl();

	return pMarker;

}

CXTPTrackMarker* CXTPTrackMarkers::GetAt(int nIndex) const
{
	if (m_arrMarkers.GetSize() > nIndex)
		return m_arrMarkers[nIndex];
	return NULL;

}

int CXTPTrackMarkers::IndexOf(CXTPTrackMarker* pMarker)
{
	for (int i = 0; i < m_arrMarkers.GetSize(); i++)
		if (m_arrMarkers[i] == pMarker)
			return i;
	return -1;
}
void CXTPTrackMarkers::RemoveAt(int nIndex)
{
	if (nIndex >= 0 && nIndex < m_arrMarkers.GetSize())
	{
		m_pControl->GetUndoManager()->AddUndoCommand(new CXTPTrackUndoDeleteMarkerCommand(m_arrMarkers[nIndex]));

		m_arrMarkers[nIndex]->InternalRelease();
		m_arrMarkers.RemoveAt(nIndex);
	}
	m_pControl->RedrawControl();
}

void CXTPTrackMarkers::Remove(CXTPTrackMarker* pMarker)
{
	RemoveAt(IndexOf(pMarker));
}

void CXTPTrackMarkers::RemoveAll()
{
	m_pControl->GetUndoManager()->StartGroup();

	for (int i = 0; i < m_arrMarkers.GetSize(); i++)
	{
		m_pControl->GetUndoManager()->AddUndoCommand(new CXTPTrackUndoDeleteMarkerCommand(m_arrMarkers[i]));
		m_arrMarkers[i]->InternalRelease();
	}

	m_pControl->GetUndoManager()->EndGroup();

	m_arrMarkers.RemoveAll();

	m_pControl->RedrawControl();
}

int CXTPTrackMarkers::GetCount() const
{
	return (int)m_arrMarkers.GetSize();
}

int CXTPTrackMarkers::HitTest(CPoint pt) const
{
	for (int i = (int)m_arrMarkers.GetSize() - 1; i >= 0; i--)
	{
		if (m_arrMarkers.GetAt(i)->GetRect().PtInRect(pt))
			return i;
	}
	return -1;
}

void CXTPTrackMarkers::DoPropExchange(CXTPPropExchange* pPX)
{

	CXTPPropExchangeEnumeratorPtr pEnumRecords(pPX->GetEnumerator(_T("Marker")));

	if (pPX->IsStoring())
	{
		int nCount = (int)GetCount();
		POSITION pos = pEnumRecords->GetPosition((DWORD)nCount);

		for (int i = 0; i < nCount; i++)
		{
			CXTPTrackMarker* pMarker = GetAt(i);
			ASSERT(pMarker);

			CXTPPropExchangeSection sec(pEnumRecords->GetNext(pos));
			PX_Object(&sec, pMarker, RUNTIME_CLASS(CXTPTrackMarker));
		}
	}
	else
	{
		RemoveAll();

		POSITION pos = pEnumRecords->GetPosition();

		while (pos)
		{
			CXTPTrackMarker* pMarker = NULL;

			CXTPPropExchangeSection sec(pEnumRecords->GetNext(pos));
			PX_Object(&sec, pMarker, RUNTIME_CLASS(CXTPTrackMarker));

			if (!pMarker)
				AfxThrowArchiveException(CArchiveException::badClass);

			pMarker->m_pControl = m_pControl;
			m_arrMarkers.Add(pMarker);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// CXTPTrackSelectedBlocks

CXTPTrackSelectedBlocks::CXTPTrackSelectedBlocks()
{
#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

int CXTPTrackSelectedBlocks::GetCount() const
{
	return (int)m_arrBlocks.GetSize();
}

void CXTPTrackSelectedBlocks::Add(CXTPTrackBlock* pBlock)
{
	for (int i = 0; i < m_arrBlocks.GetSize(); i++)
		if (m_arrBlocks[i] == pBlock)
			return;

	m_arrBlocks.Add(pBlock);
}

BOOL CXTPTrackSelectedBlocks::IsSelected(const CXTPTrackBlock* pBlock) const
{
	for (int i = 0; i < m_arrBlocks.GetSize(); i++)
		if (m_arrBlocks[i] == pBlock)
			return TRUE;

	return FALSE;
}

CXTPTrackBlock* CXTPTrackSelectedBlocks::GetAt(int nIndex) const
{
	if (nIndex >= 0 && nIndex < m_arrBlocks.GetSize())
		return m_arrBlocks[nIndex];

	return NULL;
}

void CXTPTrackSelectedBlocks::RemoveAll()
{
	m_arrBlocks.RemoveAll();
}

void CXTPTrackSelectedBlocks::RemoveAt(int nIndex)
{
	if (nIndex >= 0 && nIndex < m_arrBlocks.GetSize())
		m_arrBlocks.RemoveAt(nIndex);
}

void CXTPTrackSelectedBlocks::Remove(CXTPTrackBlock* pBlock)
{
	for (int i = 0; i < m_arrBlocks.GetSize(); i++)
	{
		if (m_arrBlocks[i] == pBlock)
		{
			m_arrBlocks.RemoveAt(i);
		}
	}
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPTrackBlock, CCmdTarget)
	DISP_PROPERTY_EX_ID(CXTPTrackBlock, "Position", 1000, OleGetPosition, OleSetPosition, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPTrackBlock, "Length", 1001, OleGetLength, OleSetLength, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPTrackBlock, "Locked", 1002, OleGetLocked, OleSetLocked, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPTrackBlock, "Color", 1003, OleGetColor, OleSetColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPTrackBlock, "Tooltip", 1004, OleGetTooltip, OleSetTooltip, VT_BSTR)
	DISP_PROPERTY_EX_ID(CXTPTrackBlock, "DescriptionText", 1005, OleGetDescriptionText, OleSetDescriptionText, VT_BSTR)
	DISP_PROPERTY_EX_ID(CXTPTrackBlock, "HeightPercent", 1006, OleGetHeightPercent, OleSetHeightPercent, VT_R8)
	DISP_PROPERTY_EX_ID(CXTPTrackBlock, "VerticalAlignment", 1007, OleGetVerticalAlignment, OleSetVerticalAlignment, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPTrackBlock, "MinLength", 1008, OleGetMinLength, OleSetMinLength, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPTrackBlock, "MaxLength", 1009, OleGetMaxLength, OleSetMaxLength, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPTrackBlock, "Item", 1010, OleGetItem, SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPTrackBlock, "Index", 1011, OleGetIndex, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPTrackBlock, "IsKey", 1012, OleGetIsKey, SetNotSupported, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPTrackBlock, "Caption", 1013, OleGetCaption, OleSetCaption, VT_BSTR)
	DISP_PROPERTY_EX_ID(CXTPTrackBlock, "Selected", 1014, IsSelected, Select, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPTrackBlock, "HeightFixed", 1015, OleGetHeightFixed, OleSetHeightFixed, VT_I4)

END_DISPATCH_MAP()

// {ABC12CE5-E015-4f87-885D-DE3326A63BEA}
static const GUID IID_ITrackBlock =
{ 0xabc12ce5, 0xe015, 0x4f87, { 0x88, 0x5d, 0xde, 0x33, 0x26, 0xa6, 0x3b, 0xea } };

BEGIN_INTERFACE_MAP(CXTPTrackBlock, CCmdTarget)
INTERFACE_PART(CXTPTrackBlock, IID_ITrackBlock, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPTrackBlock, IID_ITrackBlock)

int CXTPTrackBlock::OleGetPosition()
{
	return m_nPosition;
}

void CXTPTrackBlock::OleSetPosition(int nPosition)
{
	SetPosition(nPosition);
	if (m_pItem) m_pItem->RecalcLayout();
}

int CXTPTrackBlock::OleGetLength()
{
	return m_nLength;
}

void CXTPTrackBlock::OleSetLength(int nLength)
{
	SetLength(nLength);
	if (m_pItem) m_pItem->RecalcLayout();

}

BOOL CXTPTrackBlock::OleGetLocked()
{
	return m_bLocked;
}

void CXTPTrackBlock::OleSetLocked(BOOL bLocked)
{
	m_bLocked = bLocked;
	if (m_pItem) m_pItem->RedrawControl();

}


COLORREF CXTPTrackBlock::OleGetColor()
{
	return m_clrBlock;
}

void CXTPTrackBlock::OleSetColor(OLE_COLOR clr)
{
	m_clrBlock = AxTranslateColor(clr);
	if (m_pItem) m_pItem->RedrawControl();
}

BSTR CXTPTrackBlock::OleGetTooltip()
{
	return m_strToolTip.AllocSysString();
}

void CXTPTrackBlock::OleSetTooltip(LPCTSTR lpszTooltip)
{
	m_strToolTip = lpszTooltip;
}

BSTR CXTPTrackBlock::OleGetDescriptionText()
{
	return m_strDescription.AllocSysString();
}

void CXTPTrackBlock::OleSetDescriptionText(LPCTSTR lpszDescription)
{
	m_strDescription = lpszDescription;
}

BSTR CXTPTrackBlock::OleGetCaption()
{
	return m_strCaption.AllocSysString();
}

void CXTPTrackBlock::OleSetCaption(LPCTSTR lpszCaption)
{
	m_strCaption = lpszCaption;
	if (m_pItem) m_pItem->RedrawControl();
}

double CXTPTrackBlock::OleGetHeightPercent()
{
	return m_dHeightPercent;
}

void CXTPTrackBlock::OleSetHeightPercent(double dHeightPercent)
{
	m_dHeightPercent = dHeightPercent;
	if (m_pItem) m_pItem->RedrawControl();
}

int CXTPTrackBlock::OleGetHeightFixed()
{
	return m_nHeightFixed;
}

void CXTPTrackBlock::OleSetHeightFixed(int dHeightFixed)
{
	m_nHeightFixed = dHeightFixed;
	if (m_pItem) m_pItem->RedrawControl();
}


int CXTPTrackBlock::OleGetVerticalAlignment()
{
	return m_nVerticalAlignment;
}

void CXTPTrackBlock::OleSetVerticalAlignment(int nVerticalAlignment)
{
	m_nVerticalAlignment = nVerticalAlignment;
	if (m_pItem) m_pItem->RedrawControl();
}


int CXTPTrackBlock::OleGetMinLength()
{
	return m_nMinLength;
}

void CXTPTrackBlock::OleSetMinLength(int nMinLength)
{
	m_nMinLength = nMinLength;
}

int CXTPTrackBlock::OleGetMaxLength()
{
	return m_nMaxLength;
}

void CXTPTrackBlock::OleSetMaxLength(int nMaxLength)
{
	m_nMaxLength = nMaxLength;
}

LPDISPATCH CXTPTrackBlock::OleGetItem()
{
	return XTPGetDispatch(m_pItem);
}

int CXTPTrackBlock::OleGetIndex()
{
	return m_nIndex;
}

BOOL CXTPTrackBlock::OleGetIsKey()
{
	return IsKey();
}



//////////////////////////////////////////////////////////////////////////
//


BEGIN_DISPATCH_MAP(CXTPTrackMarker, CCmdTarget)
	DISP_PROPERTY_ID(CXTPTrackMarker, "Position", 1000, m_nPosition, VT_I4)
	DISP_PROPERTY_ID(CXTPTrackMarker, "Caption", 1001, m_strCaption, VT_BSTR)
END_DISPATCH_MAP()

// {ABC10CE5-E015-4f87-885D-DE3326A63BEA}
static const GUID IID_ITrackMarker =
{ 0xabc10ce5, 0xe015, 0x4f87, { 0x88, 0x5d, 0xde, 0x33, 0x26, 0xa6, 0x3b, 0xea } };

BEGIN_INTERFACE_MAP(CXTPTrackMarker, CCmdTarget)
	INTERFACE_PART(CXTPTrackMarker, IID_ITrackMarker, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPTrackMarker, IID_ITrackMarker)



//////////////////////////////////////////////////////////////////////////
// CXTPTrackMarkers

BEGIN_DISPATCH_MAP(CXTPTrackMarkers, CCmdTarget)
	DISP_FUNCTION_ID(CXTPTrackMarkers, "Count", 1, OleGetItemCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CXTPTrackMarkers, "Marker", DISPID_VALUE, OleGetItem, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPTrackMarkers, "_NewEnum", DISPID_NEWENUM, OleNewEnum, VT_UNKNOWN, VTS_NONE)
	DISP_FUNCTION_ID(CXTPTrackMarkers, "Add", 2, OleAdd, VT_DISPATCH, VTS_I4 VTS_BSTR)
	DISP_FUNCTION_ID(CXTPTrackMarkers, "DeleteAll", 5, RemoveAll, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPTrackMarkers, "RemoveAt", 6, Remove, VT_EMPTY, VTS_I4)
END_DISPATCH_MAP()

// {ABC11CE5-E015-4f87-885D-DE3326A63BEA}
static const GUID IID_ITrackMarkers =
{ 0xabc11ce5, 0xe015, 0x4f87, { 0x88, 0x5d, 0xde, 0x33, 0x26, 0xa6, 0x3b, 0xea } };

BEGIN_INTERFACE_MAP(CXTPTrackMarkers, CCmdTarget)
	INTERFACE_PART(CXTPTrackMarkers, IID_ITrackMarkers, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPTrackMarkers, IID_ITrackMarkers)
IMPLEMENT_ENUM_VARIANT(CXTPTrackMarkers)



int CXTPTrackMarkers::OleGetItemCount()
{
	return GetCount();
}

LPDISPATCH CXTPTrackMarkers::OleGetItem(long nIndex)
{
	if (nIndex >= 0 && nIndex < GetCount())
	{
		return GetAt(nIndex)->GetIDispatch(TRUE);
	}
	AfxThrowOleException(E_INVALIDARG);
	return 0;
}


LPDISPATCH CXTPTrackMarkers::OleAdd(int Position, LPCTSTR lpszCaption)
{
	CXTPTrackMarker* pMarker = Add(Position, lpszCaption);
	return XTPGetDispatch(pMarker);
}

//////////////////////////////////////////////////////////////////////////
//


BEGIN_DISPATCH_MAP(CXTPTrackSelectedBlocks, CCmdTarget)
	DISP_FUNCTION_ID(CXTPTrackSelectedBlocks, "Count", 1, OleGetItemCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CXTPTrackSelectedBlocks, "Block", DISPID_VALUE, OleGetItem, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPTrackSelectedBlocks, "_NewEnum", DISPID_NEWENUM, OleNewEnum, VT_UNKNOWN, VTS_NONE)
	DISP_FUNCTION_ID(CXTPTrackSelectedBlocks, "DeleteAll", 5, RemoveAll, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()

// {ABC23CE5-E015-4f87-885D-DE3326A63BEA}
static const GUID IID_ITrackSelectedBlocks =
{ 0xabc23ce5, 0xe015, 0x4f87, { 0x88, 0x5d, 0xde, 0x33, 0x26, 0xa6, 0x3b, 0xea } };

BEGIN_INTERFACE_MAP(CXTPTrackSelectedBlocks, CCmdTarget)
INTERFACE_PART(CXTPTrackSelectedBlocks, IID_ITrackSelectedBlocks, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPTrackSelectedBlocks, IID_ITrackSelectedBlocks)
IMPLEMENT_ENUM_VARIANT(CXTPTrackSelectedBlocks)



int CXTPTrackSelectedBlocks::OleGetItemCount()
{
	return GetCount();
}

LPDISPATCH CXTPTrackSelectedBlocks::OleGetItem(long nIndex)
{
	if (nIndex >= 0 && nIndex < GetCount())
	{
		return GetAt(nIndex)->GetIDispatch(TRUE);
	}
	AfxThrowOleException(E_INVALIDARG);
	return 0;
}





#endif
