// XTPReportRecordItemControls.cpp
//
// This file is a part of the XTREME REPORTCONTROL MFC class library.
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

#include "Common/Tmschema.h"
#include "Common/XTPPropExchange.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPCustomHeap.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSmartPtrInternalT.h"

#include "XTPReportDefines.h"
#include "XTPReportControl.h"
#include "XTPReportRecordItem.h"
#include "XTPReportRecordItemControls.h"

#pragma warning(disable: 4097) // TBase' used as synonym for class-name


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPReportRecordItemControl

IMPLEMENT_SERIAL(CXTPReportRecordItemControl, CXTPCmdTarget, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)
IMPLEMENT_SERIAL(CXTPReportRecordItemButton, CXTPReportRecordItemControl, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

IMPLEMENT_DYNAMIC(CXTPReportRecordItemControls, CCmdTarget)
//////////////////////////////////////////////////////////////////////////

CXTPReportRecordItemControl::CXTPReportRecordItemControl(LPCTSTR szCaption)
	: m_strCaption(szCaption)
{
#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif

	m_nType = 0;
	m_nIndex = 0;


	LOGFONT lfIcon;
	VERIFY(CXTPDrawHelpers::GetIconLogFont(&lfIcon));


	m_clrCaption = GetSysColor(COLOR_BTNTEXT);
	m_sizeControl = CSize(0, 0);
	m_rcControl = CRect(0, 0, 0, 0);
	m_unFlags = 0;
	m_Alignment = xtpItemControlUnknown;
	m_bEnabled = TRUE;
	m_nState = 0;
	m_bThemed = FALSE;
}

CXTPReportRecordItemControl::~CXTPReportRecordItemControl()
{
	m_mapIcon.RemoveAll();
}

CFont* CXTPReportRecordItemControl::GetFont()
{
	return &m_fntCaption;
}

void CXTPReportRecordItemControl::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_Int(pPX, _T("Type"), m_nType);
	PX_String(pPX, _T("Caption"), m_strCaption);
	PX_ULong(pPX, _T("CaptionColor"), (ULONG&)m_clrCaption);
	PX_Size(pPX, _T("Size"), m_sizeControl, CSize(-1, -1));
	PX_ULong(pPX, _T("Flags"), (ULONG&)m_unFlags);
	PX_Int(pPX, _T("Alignment"), (int&)m_Alignment);
	PX_Bool(pPX, _T("Enable"), m_bEnabled);
	PX_Bool(pPX, _T("Themed"), m_bThemed);

	CXTPPropExchangeEnumeratorPtr pEnumItems(pPX->GetEnumerator(_T("StateIcons")));

	if (pPX->IsStoring())
	{
		DWORD dwCount = (DWORD)m_mapIcon.GetCount();
		POSITION posItem = pEnumItems->GetPosition(dwCount);

		POSITION posData = m_mapIcon.GetStartPosition();
		while (posData)
		{
			int nKey = 0, nValue = 0;
			m_mapIcon.GetNextAssoc(posData, nKey, nValue);

			CXTPPropExchangeSection secItem(pEnumItems->GetNext(posItem));
			PX_Int(&secItem, _T("State"), nKey);
			PX_Int(&secItem, _T("Icon"), nValue);
		}
	}
	else
	{
		m_mapIcon.RemoveAll();

		POSITION posItem = pEnumItems->GetPosition();

		while (posItem)
		{
			CXTPPropExchangeSection secItem(pEnumItems->GetNext(posItem));
			int nKey = 0, nValue = 0;
			PX_Int(&secItem, _T("State"), nKey, 0);
			PX_Int(&secItem, _T("Icon"), nValue, 0);

			m_mapIcon[nKey] = nValue;
		}
	}
}

void CXTPReportRecordItemControl::SetFont(CFont* pFont)
{
	LOGFONT lf;
	pFont->GetLogFont(&lf);

	m_fntCaption.DeleteObject();
	m_fntCaption.CreateFontIndirect(&lf);
}

int CXTPReportRecordItemControl::GetIconIndex(int nState) const
{
	int nIconIndex = XTP_REPORT_NOICON;
	if (m_mapIcon.Lookup(nState, nIconIndex))
		return nIconIndex;
	m_mapIcon.Lookup(0, nIconIndex);
	return nIconIndex;
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPReportRecordItemControl, CXTPCmdTarget)
	DISP_PROPERTY_EX_ID(CXTPReportRecordItemControl, "Type", 1, GetType, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPReportRecordItemControl, "Index", 2, GetIndex, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPReportRecordItemControl, "Caption", 3, OleGetCaption, OleSetCaption, VT_BSTR)
	DISP_PROPERTY_ID(CXTPReportRecordItemControl, "CaptionColor", 4, m_clrCaption, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPReportRecordItemControl, "Font", 5, OleGetFont, OleSetFont, VT_FONT)
	DISP_PROPERTY_ID(CXTPReportRecordItemControl, "Alignment", 6, m_Alignment, VT_I4)
	DISP_FUNCTION_ID(CXTPReportRecordItemControl, "SetSize", 7, OleSetSize, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_PROPERTY_EX_ID(CXTPReportRecordItemControl, "Width", 8, GetWidth, SetWidth, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPReportRecordItemControl, "Height", 9, GetHeight, SetHeight, VT_I4)
	DISP_FUNCTION_ID(CXTPReportRecordItemControl, "GetRect", 10, OleGetRect, VT_EMPTY, VTS_PI4 VTS_PI4 VTS_PI4 VTS_PI4)
	DISP_PROPERTY_EX_ID(CXTPReportRecordItemControl, "Enable", 11, GetEnable, SetEnable, VT_BOOL)
	DISP_PROPERTY_ID(CXTPReportRecordItemControl, "Themed", 12, m_bThemed, VT_BOOL)
	DISP_FUNCTION_ID(CXTPReportRecordItemControl, "SetIconIndex", 13, SetIconIndex, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CXTPReportRecordItemControl, "GetIconIndex", 14, GetIconIndex, VT_I4, VTS_I4)
END_DISPATCH_MAP()

// {E472E690-31B6-47e4-A0EA-AC8952588573}
static const GUID IID_IReportRecordItemControl =
{ 0xe472e690, 0x31b6, 0x47e4, { 0xa0, 0xea, 0xac, 0x89, 0x52, 0x58, 0x85, 0x73 } };

BEGIN_INTERFACE_MAP(CXTPReportRecordItemControl, CCmdTarget)
	INTERFACE_PART(CXTPReportRecordItemControl, IID_IReportRecordItemControl, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPReportRecordItemControl, IID_IReportRecordItemControl)

BSTR CXTPReportRecordItemControl::OleGetCaption()
{
	return m_strCaption.AllocSysString();
}

void CXTPReportRecordItemControl::OleSetCaption(LPCTSTR pcszCaption)
{
	m_strCaption = pcszCaption;
}

LPFONTDISP CXTPReportRecordItemControl::OleGetFont()
{
	return AxCreateOleFont(GetFont(), this, (LPFNFONTCHANGED)&CXTPReportRecordItemControl::OleSetFont);
}

void CXTPReportRecordItemControl::OleSetFont(LPFONTDISP pFontDisp)
{
	ASSERT((pFontDisp == NULL) || AfxIsValidAddress(pFontDisp, sizeof(IDispatch), FALSE));

	AxCreateFontFromDispatch(&m_fntCaption, pFontDisp);
}

void CXTPReportRecordItemControl::OleSetSize(long cx, long cy)
{
	m_sizeControl = CSize(cx, cy);
}

void CXTPReportRecordItemControl::OleGetRect(long* pnLeft, long* pnTop, long* pnRight, long* pnBottom)
{
	if (pnLeft)
		*pnLeft = m_rcControl.left;
	if (pnTop)
		*pnTop = m_rcControl.top;
	if (pnRight)
		*pnRight = m_rcControl.right;
	if (pnBottom)
		*pnBottom = m_rcControl.bottom;
}

#endif


//////////////////////////////////////////////////////////////////////////
// CXTPReportRecordItemButton

CXTPReportRecordItemButton::CXTPReportRecordItemButton(LPCTSTR szCaption)
	: CXTPReportRecordItemControl(szCaption)
{
#ifdef _XTP_ACTIVEX
	EnableAutomation();
#endif

	m_nState = m_nSavedState = PBS_NORMAL;
}

void CXTPReportRecordItemButton::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPReportRecordItemControl::DoPropExchange(pPX);
}

void CXTPReportRecordItemButton::OnLButtonDown(XTP_REPORTRECORDITEM_CLICKARGS* pClickArgs)
{
	UNREFERENCED_PARAMETER(pClickArgs);

	if (GetEnable())
		m_nState = PBS_PRESSED;
}

void CXTPReportRecordItemButton::OnLButtonUp(XTP_REPORTRECORDITEM_CLICKARGS* pClickArgs)
{
	//UNREFERENCED_PARAMETER(pClickArgs);
//>>attempt for 23588 issue case
	if (pClickArgs
		&& pClickArgs->pControl
		&& pClickArgs->pControl->IsEditMode())
		return;
//>>attempt for 23588 issue case

	m_nState = GetEnable() ? PBS_NORMAL : PBS_DISABLED;
	if (m_rcControl.PtInRect(pClickArgs->ptClient)
		&& pClickArgs->pControl
		&& m_rcControl.PtInRect(pClickArgs->pControl->m_mouseDownState.ptMouse))
	{
		XTP_NM_REPORTITEMCONTROL nm;
		::ZeroMemory(&nm, sizeof(nm));
		nm.pItem = pClickArgs->pItem;
		nm.pRow = pClickArgs->pRow;
		nm.pColumn = pClickArgs->pColumn;
		nm.pt = pClickArgs->ptClient;
		nm.pItemControl = this;
		pClickArgs->pControl->SendNotifyMessage(XTP_NM_REPORT_ITEMBUTTONCLICK, (NMHDR*)&nm);
	}
}

void CXTPReportRecordItemButton::OnMouseEnter(UINT nFlags, CPoint point)
{
	UNREFERENCED_PARAMETER(point);
	if (m_nSavedState == PBS_PRESSED && (nFlags & MK_LBUTTON))
		m_nState = m_nSavedState;
}

void CXTPReportRecordItemButton::OnMouseLeave(UINT nFlags, CPoint point)
{
	UNREFERENCED_PARAMETER(nFlags);
	UNREFERENCED_PARAMETER(point);
	m_nSavedState = m_nState;
	m_nState = GetEnable() ? PBS_NORMAL : PBS_DISABLED;
}

void CXTPReportRecordItemButton::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_rcControl.PtInRect(point) && m_nSavedState == PBS_PRESSED && (nFlags & MK_LBUTTON))
		m_nState = m_nSavedState;
}


//////////////////////////////////////////////////////////////////////////
// CXTPReportRecordItemControls


CXTPReportRecordItemControls::CXTPReportRecordItemControls()
{
	m_pRecordItem = NULL;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPReportRecordItemControls::CXTPReportRecordItemControls(CXTPReportRecordItem* pRecordItem)
: m_pRecordItem(pRecordItem)
{
#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPReportRecordItemControls::~CXTPReportRecordItemControls()
{
	RemoveAll();
}

void CXTPReportRecordItemControls::DoPropExchange(CXTPPropExchange* pPX)
{
	int nCount = (int)GetSize();

	CXTPPropExchangeEnumeratorPtr pEnumItems(pPX->GetEnumerator(_T("RecordItemControl")));

	if (pPX->IsStoring())
	{
		POSITION posItem = pEnumItems->GetPosition((DWORD)nCount);

		for (int i = 0; i < nCount; i++)
		{
			CXTPReportRecordItemControl* pItemControl = GetAt(i);
			ASSERT(pItemControl);
			if (!pItemControl)
				AfxThrowArchiveException(CArchiveException::badClass);

			CXTPPropExchangeSection secItem(pEnumItems->GetNext(posItem));
			PX_Object(&secItem, pItemControl, RUNTIME_CLASS(CXTPReportRecordItemControl));
		}
	}
	else
	{
		RemoveAll();

		POSITION posItem = pEnumItems->GetPosition();

		while (posItem)
		{
			CXTPReportRecordItemControl* pItemControl = NULL;

			CXTPPropExchangeSection secItem(pEnumItems->GetNext(posItem));
			PX_Object(&secItem, pItemControl, RUNTIME_CLASS(CXTPReportRecordItemControl));

			if (!pItemControl)
				AfxThrowArchiveException(CArchiveException::badClass);

			AddControl(pItemControl);
		}
	}
}

void CXTPReportRecordItemControls::RemoveAll()
{
	for (int nItem = (int)GetSize() - 1; nItem >= 0; nItem--)
	{
		CXTPReportRecordItemControl* pItem = GetAt(nItem);
		if (pItem)
			pItem->InternalRelease();
	}

	if (m_pRecordItem)
	{
		m_pRecordItem->m_pFocusedItemControl = 0;
	}

	TBase::RemoveAll();
}

void CXTPReportRecordItemControls::RemoveAt(int nIndex)
{
	if (nIndex < 0 || nIndex >= (int)GetSize())
	{
		ASSERT(FALSE);
		return;
	}

	CXTPReportRecordItemControl* pItem = GetAt(nIndex);

	if (m_pRecordItem && m_pRecordItem->m_pFocusedItemControl == pItem)
	{
		m_pRecordItem->m_pFocusedItemControl = 0;
	}

	if (pItem)
		pItem->InternalRelease();

	TBase::RemoveAt(nIndex);

	RefreshIndexes(nIndex);
}

void CXTPReportRecordItemControls::RefreshIndexes(int nIndexStart)
{
	for (int i=nIndexStart; i<GetSize(); i++)
	{
		CXTPReportRecordItemControl *pItem = GetAt(i);

		if (NULL != pItem)
		{
			pItem->m_nIndex = i;
		}
	}
}

CXTPReportRecordItemControl* CXTPReportRecordItemControls::AddControl(int nType, int nIndex)
{
	CXTPReportRecordItemControl* pControl = NULL;
	switch (nType)
	{
		case xtpItemControlTypeButton :
			pControl = (CXTPReportRecordItemControl*) new CXTPReportRecordItemButton;
			break;
		default:
			ASSERT(FALSE);
	}
	if (pControl)
		pControl->m_nType = nType;

	return AddControl(pControl, nIndex);
}

CXTPReportRecordItemControl* CXTPReportRecordItemControls::AddControl(CXTPReportRecordItemControl* pControl, int nIndex)
{
	if (!pControl)
		return NULL;

	if (nIndex < 0 || nIndex >= GetSize())
		nIndex = Add(pControl);
	else
		InsertAt(nIndex, pControl);

	pControl->m_nIndex = nIndex;

	RefreshIndexes(nIndex + 1);

	return pControl;
}

void CXTPReportRecordItemControls::CopyFrom(CXTPReportRecordItemControls* pSrc)
{
	if (pSrc == this)
		return;

	RemoveAll();

	if (!pSrc)
		return;

	int nCount = pSrc->GetSize();
	for (int i = 0; i < nCount; i++)
	{
		CXTPReportRecordItemControl* pItem = pSrc->GetAt(i);
		if (pItem)
		{
			pItem->InternalAddRef();
			Add(pItem);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CXTPReportRecordItemControlHookWnd, CWnd)
//{{AFX_MSG_MAP(CXTPReportRecordItemControlHookWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CXTPReportRecordItemControlHookWnd::CXTPReportRecordItemControlHookWnd(XTP_REPORTRECORDITEM_CLICKARGS* pClickArgs)
{
	m_ClickArgs = *pClickArgs;

	m_ClickArgs.AddRef();
}

CXTPReportRecordItemControlHookWnd::~CXTPReportRecordItemControlHookWnd()
{
	m_ClickArgs.Release();
}

void CXTPReportRecordItemControlHookWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	UNREFERENCED_PARAMETER(nFlags);
	m_ClickArgs.ptClient = point;
	ClientToScreen(&m_ClickArgs.ptClient);
	m_ClickArgs.pItem->OnLButtonDown(&m_ClickArgs);
}

void CXTPReportRecordItemControlHookWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	UNREFERENCED_PARAMETER(nFlags);
	m_ClickArgs.ptClient = point;
	ClientToScreen(&m_ClickArgs.ptClient);
	m_ClickArgs.pItem->OnLButtonUp(&m_ClickArgs);
}

void CXTPReportRecordItemControlHookWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	m_ClickArgs.pItem->OnMouseMove(nFlags, point, m_ClickArgs.pControl);
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPReportRecordItemButton, CXTPReportRecordItemControl)
END_DISPATCH_MAP()

// {7297FB18-193B-41ce-826B-B5A39EEE90D5}
static const GUID IID_IReportRecordItemButton =
{ 0x7297fb18, 0x193b, 0x41ce, { 0x82, 0x6b, 0xb5, 0xa3, 0x9e, 0xee, 0x90, 0xd5 } };

BEGIN_INTERFACE_MAP(CXTPReportRecordItemButton, CXTPReportRecordItemControl)
	INTERFACE_PART(CXTPReportRecordItemButton, IID_IReportRecordItemButton, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPReportRecordItemButton, IID_IReportRecordItemButton)

//////////////////////////////////////////////////////////////////////////
BEGIN_DISPATCH_MAP(CXTPReportRecordItemControls, CCmdTarget)
	DISP_FUNCTION_ID(CXTPReportRecordItemControls, "Count", 1, OleGetItemCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CXTPReportRecordItemControls, "RemoveAll", 2, RemoveAll, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPReportRecordItemControls, "RemoveAt", 3, RemoveAt, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CXTPReportRecordItemControls, "AddButton", 4, OleAddButton, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPReportRecordItemControls, "Item", DISPID_VALUE, OleGetItem, VT_DISPATCH, VTS_I4)
END_DISPATCH_MAP()

// {3F799C2E-0A2A-42d7-9982-BF524C62C413}
static const GUID IID_IReportRecordItemControls =
{ 0x3f799c2e, 0xa2a, 0x42d7, { 0x99, 0x82, 0xbf, 0x52, 0x4c, 0x62, 0xc4, 0x13 } };

BEGIN_INTERFACE_MAP(CXTPReportRecordItemControls, CCmdTarget)
	INTERFACE_PART(CXTPReportRecordItemControls, IID_IReportRecordItemControls, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPReportRecordItemControls, IID_IReportRecordItemControls)
IMPLEMENT_ENUM_VARIANT(CXTPReportRecordItemControls)


LPDISPATCH CXTPReportRecordItemControls::OleAddButton(int nIndex)
{
	CXTPReportRecordItemControl* pItemControl = AddControl(xtpItemControlTypeButton, nIndex);
	if (!pItemControl)
		return NULL;
	return pItemControl->GetIDispatch(TRUE);

	//CXTPReportRecordItemButton* pItemButton = DYNAMIC_DOWNCAST(CXTPReportRecordItemButton, pItemControl);
	//return pItemButton ? pItemButton->GetIDispatch(TRUE) : NULL;
}

LPDISPATCH CXTPReportRecordItemControls::OleGetItem(long nIndex)
{
	CXTPReportRecordItemControl* pItemControl = GetAt(nIndex);

	if (pItemControl)
		return pItemControl->GetIDispatch(TRUE);

//  if (!pItemControl || pItemControl->GetType() != xtpItemControlTypeButton)
//      return NULL;

	//CXTPReportRecordItemButton* pItemButton = DYNAMIC_DOWNCAST(CXTPReportRecordItemButton, pItemControl);
	//return pItemButton ? pItemButton->GetIDispatch(TRUE) : NULL;

	return NULL;
}

#endif
