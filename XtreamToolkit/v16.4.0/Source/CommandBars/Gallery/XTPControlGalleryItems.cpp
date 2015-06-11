// XTPControlGallery.cpp : implementation file.
//
// This file is a part of the XTREME COMMANDBARS MFC class library.
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
#include <math.h>

#include <Common/XTPImageManager.h>
#include <Common/XTPPropExchange.h>
#include <Common/XTPToolTipContext.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPHookManager.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPDrawHelpers.h>
#include <Common/XTPMarkupRender.h>
#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollInfo.h>
#include <Common/ScrollBar/XTPScrollBarPaintManager.h>

#include <CommandBars/XTPCommandBarsDefines.h>
#include <CommandBars/XTPCommandBar.h>
#include <CommandBars/XTPCommandBars.h>
#include <CommandBars/XTPCommandBarsOptions.h>

#include <CommandBars/XTPPopupBar.h>
#include <CommandBars/XTPControl.h>
#include <CommandBars/XTPControlButton.h>
#include <CommandBars/XTPControlPopup.h>
#include <CommandBars/XTPControlEdit.h>
#include <CommandBars/XTPControlComboBox.h>

#include <CommandBars/XTPControls.h>
#include <CommandBars/XTPControlGallery.h>

#include "XTPControlGalleryItem.h"
#include "XTPControlGalleryItems.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
// CXTPControlGalleryItems

IMPLEMENT_DYNAMIC(CXTPControlGalleryItems, CCmdTarget)

CXTPControlGalleryItems::CXTPControlGalleryItems(CXTPControlAction* pAction, CXTPCommandBars* pCommandBars)
	: m_pAction(pAction), m_pCommandBars(pCommandBars)
{
	//ASSERT(pCommandBars);

	if (pAction)
	{
		ASSERT(m_pAction->m_pTarget == NULL);
		m_pAction->m_pTarget = this;
	}

	m_szItem = CSize(0, 20);

	m_pImageManager = new CXTPImageManager();
	m_pImageManager->DrawReverted(2);
	m_bClipItems = TRUE;

	m_pMarkupContext = NULL;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
	EnableConnections();
#endif
}

CXTPControlGalleryItems::~CXTPControlGalleryItems()
{
	m_pAction = NULL;

	RemoveAll();
	CMDTARGET_RELEASE(m_pImageManager);

	XTPMarkupReleaseContext(m_pMarkupContext);

#ifdef _XTP_ACTIVEX
	ReleaseConnections();
#endif
}


void CXTPControlGalleryItems::EnableMarkup(BOOL bEnableMarkup)
{
	XTPMarkupReleaseContext(m_pMarkupContext);

	if (bEnableMarkup)
	{
		m_pMarkupContext = XTPMarkupCreateContext();
	}
}



void CXTPControlGalleryItems::RemoveAll()
{
	for (int i = 0; i < m_arrItems.GetSize(); i++)
	{
		m_arrItems[i]->InternalRelease();
	}

	m_arrItems.RemoveAll();
	OnItemsChanged();
}

void CXTPControlGalleryItems::Remove(int nIndex)
{
	CXTPControlGalleryItem* pItem = GetItem(nIndex);
	if (!pItem)
		return;

	m_arrItems.RemoveAt(nIndex);
	pItem->InternalRelease();

	UpdateIndexes(nIndex);
	OnItemsChanged();
}

void CXTPControlGalleryItems::UpdateIndexes(int nStart /*= 0*/)
{
	for (int i = nStart; i < GetItemCount(); i++)
		GetItem(i)->m_nIndex = i;
}

CXTPControlGalleryItem* CXTPControlGalleryItems::InsertItem(int nIndex, CXTPControlGalleryItem* pItem, int nId, int nImage)
{
	if (!pItem)
		return NULL;

	if (nIndex < 0 || nIndex > GetItemCount())
		nIndex = GetItemCount();

	m_arrItems.InsertAt(nIndex, pItem);
	UpdateIndexes(nIndex);

	pItem->m_pItems = this;
	pItem->m_nImage = nImage;

	if (nId != -1)
	{
		pItem->m_nId = nId;

#ifndef _XTP_ACTIVEX
		CString strCaption;
		if (nId > 0 && pItem->GetCaption().IsEmpty() && strCaption.LoadString(nId))
		{
			pItem->SetCaption(strCaption);

			if (pItem->GetMarkupUIElement() == NULL)
			{
				pItem->SetToolTip(strCaption);
			}
		}
#endif
	}

	OnItemsChanged();

	return pItem;
}

void CXTPControlGalleryItems::OnItemsChanged()
{
	if (!m_pAction)
		return;

	for (int i = 0; i < m_pAction->GetCount(); i++)
	{
		CXTPControlGallery* pGallery = DYNAMIC_DOWNCAST(CXTPControlGallery, m_pAction->GetControl(i));
		if (pGallery)
		{
			pGallery->OnItemsChanged();
		}
	}
}

void CXTPControlGallery::OnItemsChanged()
{
	m_nSelected = -1;
	m_nScrollPos = 0;
	Reposition();

	if (m_bPreview)
	{
		m_bPreview = FALSE;
		NotifySite(XTP_GN_PREVIEWCANCEL);
	}
}

CXTPControlGalleryItem* CXTPControlGalleryItems::AddItem(CXTPControlGalleryItem* pItem, int nId, int nImage)
{
	return InsertItem(GetItemCount(), pItem, nId, nImage);
}

CXTPControlGalleryItem* CXTPControlGalleryItems::AddItem(int nId, int nImage)
{
	return AddItem(new CXTPControlGalleryItem(), nId, nImage);
}

CXTPControlGalleryItem* CXTPControlGalleryItems::AddItem(LPCTSTR lpszCaption, int nImage)
{
	CXTPControlGalleryItem* pItem = AddItem(new CXTPControlGalleryItem(), -1, nImage);
	pItem->SetCaption(lpszCaption);

	return pItem;
}

CXTPControlGalleryItem* CXTPControlGalleryItems::AddItem(LPCTSTR lpszCaption, int nId, int nImage)
{
	CXTPControlGalleryItem* pItem = AddItem(nId, nImage);
	pItem->SetCaption(lpszCaption);

	return pItem;
}

CXTPControlGalleryItem* CXTPControlGalleryItems::AddLabel(int nId)
{
	CXTPControlGalleryItem* pItem = AddItem(new CXTPControlGalleryItem(), nId);
	pItem->m_bLabel = TRUE;

	return pItem;
}

CXTPControlGalleryItem* CXTPControlGalleryItems::AddLabel(LPCTSTR lpszCaption)
{
	CXTPControlGalleryItem* pItem = AddItem(new CXTPControlGalleryItem());
	pItem->SetCaption(lpszCaption);
	pItem->m_bLabel = TRUE;

	return pItem;
}

int CXTPControlGalleryItems::GetItemCount() const
{
	return (int)m_arrItems.GetSize();
}

CXTPControlGalleryItem* CXTPControlGalleryItems::GetItem(int nIndex) const
{
	return nIndex >= 0 && nIndex < GetItemCount() ? m_arrItems.GetAt(nIndex) : NULL;
}

void CXTPControlGalleryItems::SetImageManager(CXTPImageManager* pImageManager)
{
	CMDTARGET_RELEASE(m_pImageManager);

	m_pImageManager = pImageManager;
}

void CXTPControlGalleryItems::SetItemSize(CSize szItem)
{
	if (CXTPControlGallery::m_bAutoScaleDpi)
	{
		szItem = CSize(XTPDpiHelper()->ScaleX(szItem.cx), XTPDpiHelper()->ScaleY(szItem.cy));
	}

	m_szItem = szItem;
}

BOOL CXTPControlGalleryItems::PreDrawItem(CDC* pDC, CXTPControlGallery* pGallery, CXTPControlGalleryItem* pItem, CRect rcItem,
	BOOL bEnabled, BOOL bSelected, BOOL bPressed, BOOL bChecked)
{
	UNREFERENCED_PARAMETER(pDC);
	UNREFERENCED_PARAMETER(pGallery);
	UNREFERENCED_PARAMETER(pItem);
	UNREFERENCED_PARAMETER(rcItem);
	UNREFERENCED_PARAMETER(bEnabled);
	UNREFERENCED_PARAMETER(bSelected);
	UNREFERENCED_PARAMETER(bPressed);
	UNREFERENCED_PARAMETER(bChecked);

#ifdef _XTP_ACTIVEX
	_variant_t vtCanceled(false);

	FireEvent(1, EVENT_PARAM(VTS_HANDLE VTS_DISPATCH VTS_DISPATCH
		VTS_I4 VTS_I4 VTS_I4 VTS_I4
		VTS_BOOL VTS_BOOL VTS_BOOL VTS_BOOL VTS_PVARIANT),
		pDC->GetSafeHdc(), pGallery->GetIDispatch(FALSE), pItem->GetIDispatch(FALSE),
		rcItem.left, rcItem.top, rcItem.Width(), rcItem.Height(),
		bEnabled, bSelected, bPressed, bChecked, &vtCanceled);

	return (bool)vtCanceled;

#endif

	return FALSE;
}

CXTPControlGalleryItems* CXTPControlGalleryItems::CreateItems(CXTPCommandBars* pCommandBars, int nId)
{
	CXTPControlAction* pAction = pCommandBars->CreateAction(nId);

	CXTPControlGalleryItems* pItems = NULL;

	if (pAction->m_pTarget == NULL)
	{
		pItems = new CXTPControlGalleryItems(pAction, pCommandBars);
	}
	else
	{
		pItems = DYNAMIC_DOWNCAST(CXTPControlGalleryItems, pAction->m_pTarget);
		ASSERT(pItems);
	}

	return pItems;
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPControlGalleryItems, CCmdTarget)
	DISP_FUNCTION_ID(CXTPControlGalleryItems, "AddItem", 1, OleAddItem, VT_DISPATCH, VTS_I4 VTS_BSTR)
	DISP_FUNCTION_ID(CXTPControlGalleryItems, "AddLabel", 2, OleAddLabel, VT_DISPATCH, VTS_BSTR)
	DISP_FUNCTION_ID(CXTPControlGalleryItems, "Count", 3, OleGetItemCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CXTPControlGalleryItems, "AddSeparator", 4, OleAddSeparator, VT_DISPATCH, VTS_NONE)
	DISP_FUNCTION_ID(CXTPControlGalleryItems, "DeleteAll", 5, RemoveAll, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPControlGalleryItems, "DeleteAt", 10, Remove, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CXTPControlGalleryItems, "_NewEnum", DISPID_NEWENUM, OleNewEnum, VT_UNKNOWN, VTS_NONE)
	DISP_FUNCTION_ID(CXTPControlGalleryItems, "Item", DISPID_VALUE, OleGetItem, VT_DISPATCH, VTS_I4)

	DISP_PROPERTY_EX_ID(CXTPControlGalleryItems, "Icons", 6, OleGetIcons, OleSetIcons, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPControlGalleryItems, "ItemWidth", 7, OleGetItemWidth, OleSetItemWidth, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPControlGalleryItems, "ItemHeight", 8, OleGetItemHeight, OleSetItemHeight, VT_I4)
	DISP_PROPERTY_ID(CXTPControlGalleryItems, "ClipItems", 9, m_bClipItems, VT_BOOL)

	DISP_PROPERTY_EX_ID(CXTPControlGalleryItems, "EnableMarkup", 30, OleGetEnableMarkup, OleSetEnableMarkup, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPControlGalleryItems, "MarkupContext", 40, OleGetMarkupContext, SetNotSupported, VT_DISPATCH)
END_DISPATCH_MAP()

// {ADC28A35-789A-40d9-B564-7577DA549319}
static const GUID IID_ICommandBarGalleryItems =
{ 0xadc28a35, 0x789a, 0x40d9, { 0xb5, 0x64, 0x75, 0x77, 0xda, 0x54, 0x93, 0x19 } };

BEGIN_INTERFACE_MAP(CXTPControlGalleryItems, CCmdTarget)
	INTERFACE_PART(CXTPControlGalleryItems, IID_ICommandBarGalleryItems, Dispatch)
	//INTERFACE_PART(CXTPControlGalleryItems, IID_IEnumVARIANT, EnumVARIANT)
	INTERFACE_PART(CXTPControlGalleryItems, IID_IConnectionPointContainer, ConnPtContainer)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPControlGalleryItems, IID_ICommandBarGalleryItems)

IMPLEMENT_ENUM_VARIANT(CXTPControlGalleryItems)

// {ADAA7B16-7BE8-42ec-870C-8849F5540F14}
static const GUID IID_ICommandBarGalleryItemsEvents =
{ 0xadaa7b16, 0x7be8, 0x42ec, { 0x87, 0xc, 0x88, 0x49, 0xf5, 0x54, 0xf, 0x14 } };


IMPLEMENT_CONNECTION_EX(CXTPControlGalleryItems, IID_ICommandBarGalleryItemsEvents)


LPDISPATCH CXTPControlGalleryItems::OleAddItem(int nId, LPCTSTR lpszCaption)
{
	CXTPControlGalleryItem* pItem = AddItem(nId, nId);
	pItem->SetCaption(lpszCaption);

	if (pItem->GetMarkupUIElement() == NULL)
	{
		pItem->SetToolTip(lpszCaption);
	}

	return XTPGetDispatch(pItem);
}

LPDISPATCH CXTPControlGalleryItems::OleAddLabel(LPCTSTR lpszCaption)
{
	return XTPGetDispatch(AddLabel(lpszCaption));
}

long CXTPControlGalleryItems::OleGetItemCount()
{
	return GetItemCount();
}

LPDISPATCH CXTPControlGalleryItems::OleGetItem(int nIndex)
{
	return XTPGetDispatch(GetItem(nIndex));
}

LPDISPATCH CXTPControlGalleryItems::OleGetIcons()
{
	return XTPGetDispatch(m_pImageManager);
}

void CXTPControlGalleryItems::OleSetIcons(LPDISPATCH lpDispatch)
{
	if (m_pImageManager)
		m_pImageManager->InternalRelease();

	m_pImageManager = CXTPImageManager::FromDispatch(lpDispatch);
	m_pImageManager->DrawReverted(2);

	if (lpDispatch)lpDispatch->AddRef();
}

int CXTPControlGalleryItems::OleGetItemWidth()
{
	return m_szItem.cx;
}

void CXTPControlGalleryItems::OleSetItemWidth(int cx)
{
	m_szItem.cx = cx;

	if (CXTPControlGallery::m_bAutoScaleDpi)
	{
		m_szItem.cx = XTPDpiHelper()->ScaleX(m_szItem.cx);
	}
}

int CXTPControlGalleryItems::OleGetItemHeight()
{
	return m_szItem.cy;
}

void CXTPControlGalleryItems::OleSetItemHeight(int cy)
{
	m_szItem.cy = cy;

	if (CXTPControlGallery::m_bAutoScaleDpi)
	{
		m_szItem.cy = XTPDpiHelper()->ScaleY(m_szItem.cy);
	}
}


void CXTPControlGalleryItems::OleSetEnableMarkup(BOOL bEnableMarkup)
{
	EnableMarkup(bEnableMarkup);
}

BOOL CXTPControlGalleryItems::OleGetEnableMarkup()
{
	return m_pMarkupContext != NULL;
}

LPDISPATCH CXTPControlGalleryItems::OleGetMarkupContext()
{
	return XTPGetDispatch((CCmdTarget*)m_pMarkupContext);
}

#endif // _XTP_ACTIVEX
