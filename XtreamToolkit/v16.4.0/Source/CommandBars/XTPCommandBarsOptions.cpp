// XTPCommandBarsOptions.cpp : implementation of the CXTPCommandBarsOptions class.
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

#include "StdAfx.h"

#include <Common/XTPColorManager.h>
#include <Common/XTPPropExchange.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/ScrollBar/XTPScrollBase.h>

#include <CommandBars/XTPCommandBarsDefines.h>
#include <CommandBars/XTPPaintManager.h>
#include <CommandBars/XTPCommandBars.h>
#include <CommandBars/XTPCommandBarsOptions.h>
#include <CommandBars/XTPControl.h>
#include <CommandBars/XTPControlButton.h>
#include <CommandBars/XTPControlPopup.h>
#include <CommandBars/XTPControlGallery.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPCommandBarsOptions

CXTPCommandBarsOptions::CXTPCommandBarsOptions()
{
	bAlwaysShowFullMenus = TRUE;
	bShowFullAfterDelay = TRUE;
	bToolBarScreenTips = TRUE;
	bToolBarAccelTips = FALSE;
	bLargeIcons = FALSE;
	bShowExpandButtonAlways = TRUE;

	szIcons = szLargeIcons = CSize(0);
	szPopupIcons = CSize(0, 0);

	bSyncFloatingBars = TRUE;
	bDblClickFloat = FALSE;
	bShowTextBelowIcons = FALSE;

	animationType = xtpAnimateWindowsDefault;
	bAltDragCustomization = TRUE;

	bAutoHideUnusedPopups = TRUE;
	bShowContextMenuAccelerators = FALSE;

	bDirtyState = FALSE;
	bShowPopupBarToolTips = FALSE;
	bAutoUpdateShortcuts = TRUE;

	keyboardCuesUse = xtpKeyboardCuesUseAll;
	keyboardCuesShow = xtpKeyboardCuesShowAlways;
	bFlyByStatus = TRUE;

	bUseAltNumPadKeys = FALSE;
	bUseSystemSaveBitsStyle = TRUE;

	bDisableCommandIfNoHandler = TRUE;

	bWrapLargePopups = FALSE;

	bShowMiniToolbarTips = FALSE;

	bShowKeyboardTips = FALSE;
	m_pCommandBars = 0;

	// DPI-scalling parameters
	m_bAutoScaleDpiGalleryItems = TRUE;
	m_bAutoScaleDpiToolbarIcons = FALSE;

	EnableAutomation();

#ifdef _XTP_ACTIVEX
	nUpdatePeriod = 100;
	bSyncActiveFloatingFrames = TRUE;
	EnableTypeLib();
#endif
}

#ifndef SPI_GETMENUUNDERLINES
#define SPI_GETMENUUNDERLINES 0x100A
#endif

void CXTPCommandBarsOptions::ShowKeyboardCues(XTPKeyboardCuesShow keyboardCues)
{
	keyboardCuesShow = keyboardCues;

	m_pCommandBars->m_bUseKeyboardCues = FALSE;
	m_pCommandBars->m_bKeyboardCuesVisible = keyboardCues != xtpKeyboardCuesShowNever;

	if (keyboardCues == xtpKeyboardCuesShowWindowsDefault)
	{
		BOOL bRetVal = TRUE;
		if (::SystemParametersInfo(SPI_GETMENUUNDERLINES, 0, &bRetVal, 0))
		{
			if (!bRetVal)
			{
				m_pCommandBars->m_bUseKeyboardCues = TRUE;
				m_pCommandBars->m_bKeyboardCuesVisible = FALSE;
			}
		}
	}
}

XTPKeyboardCuesShow CXTPCommandBarsOptions::GetShowKeyboardCues() const
{
	return keyboardCuesShow;
}

void PX_UintMap(CXTPPropExchange* pPX, LPCTSTR pszPropName, CMap<UINT, UINT, BOOL, BOOL>& mapValue)
{
	if (pPX->IsAllowBlobValues())
	{
		PX_Serialize(pPX, pszPropName, &mapValue);
		return;
	}

	CXTPPropExchangeSection secMap(pPX->GetSection(pszPropName));

	UINT nID;
	if (pPX->IsStoring())
	{
		secMap->WriteCount((DWORD)mapValue.GetCount());
		BOOL bValue;
		int i = 0;
		POSITION pos = mapValue.GetStartPosition();
		while (pos)
		{
			CString strSection;
			strSection.Format(_T("Item%i"), i++);

			mapValue.GetNextAssoc(pos, nID, bValue);

			PX_Long(&secMap, strSection, (long&)nID, 0);
		}
	}
	else
	{
		mapValue.RemoveAll();

		int nCount = secMap->ReadCount();
		for (int i = 0; i < nCount; i++)
		{
			CString strSection;
			strSection.Format(_T("Item%i"), i);
			PX_Long(&secMap, strSection, (long&)nID, 0);

			mapValue.SetAt(nID, TRUE);
		}
	}
}

void CXTPCommandBarsOptions::DoPropExchange(CXTPPropExchange* pPX, BOOL bUserOptionsOnly)
{
	pPX->UseDefaultOnLoad(FALSE);
	pPX->UseDefaultOnSave(FALSE);

	XTP_COMMANDBARS_ICONSINFO* pInfo = m_pCommandBars->GetPaintManager()->GetIconsInfo();
	DWORD dwSchema = _XTP_SCHEMA_CURRENT;

	PX_DWord(pPX, _T("Schema"), dwSchema, _XTP_SCHEMA_CURRENT);
	PX_Bool(pPX, _T("AlwaysShowFullMenus"), bAlwaysShowFullMenus, TRUE);
	PX_Bool(pPX, _T("ShowFullAfterDelay"), bShowFullAfterDelay, TRUE);
	PX_Bool(pPX, _T("ToolBarScreenTips"), bToolBarScreenTips, TRUE);
	PX_Bool(pPX, _T("ToolBarAccelTips"), bToolBarAccelTips, FALSE);
	PX_Bool(pPX, _T("LargeIcons"), bLargeIcons, FALSE);

	if ((dwSchema > _XTP_SCHEMA_875) && !(pPX->IsStoring() && pPX->IsAllowDefaultValues() && m_mapUsedCommands.IsEmpty()))
		PX_UintMap(pPX, _T("UsedCommands"), m_mapUsedCommands);

	if ((dwSchema > _XTP_SCHEMA_875) || bUserOptionsOnly)
		PX_Enum(pPX, _T("Animation"), animationType, xtpAnimateWindowsDefault);


	if (!bUserOptionsOnly)
	{
		PX_Bool(pPX, _T("SyncFloatingBars"), bSyncFloatingBars, TRUE);
		PX_Bool(pPX, _T("ShowExpandButtonAlways"), bShowExpandButtonAlways, TRUE);
		PX_Size(pPX, _T("IconsSize"), szIcons, CSize(0, 0));
		PX_Size(pPX, _T("LargeIconsSize"), szLargeIcons, CSize(0, 0));

		if (!(pPX->IsStoring() && pPX->IsAllowDefaultValues() && m_mapHiddenCommands.IsEmpty()))
			PX_UintMap(pPX, _T("HiddenCommands"), m_mapHiddenCommands);

		PX_Bool(pPX, _T("IconsWithShadow"), pInfo->bIconsWithShadow, FALSE);
		PX_Bool(pPX, _T("UseDisabledIcons"), pInfo->bUseDisabledIcons, TRUE);
		PX_Bool(pPX, _T("UseFadedIcons"), pInfo->bUseFadedIcons, TRUE);

		if (dwSchema > _XTP_SCHEMA_875)
		{
			PX_Bool(pPX, _T("SyncFloatingBars"), bSyncFloatingBars, TRUE);
			PX_Bool(pPX, _T("ShowExpandButtonAlways"), bShowExpandButtonAlways, TRUE);
			PX_Bool(pPX, _T("DblClickFloat"), bDblClickFloat, FALSE);
			PX_Bool(pPX, _T("ShowTextBelowIcons"), bShowTextBelowIcons, FALSE);
			PX_Bool(pPX, _T("AltDragCustomization"), bAltDragCustomization, TRUE);
			PX_Bool(pPX, _T("AutoHideUnusedPopups"), bAutoHideUnusedPopups, TRUE);
			PX_Size(pPX, _T("PopupIconsSize"), szPopupIcons, CSize(0, 0));
		}

		if (dwSchema > _XTP_SCHEMA_95)
		{
			PX_DWord(pPX, _T("KeyboardCuesUse"), keyboardCuesUse, xtpKeyboardCuesUseAll);
			PX_Enum(pPX, _T("KeyboardCuesShow"), keyboardCuesShow, xtpKeyboardCuesShowAlways);

			if (pPX->IsLoading()) ShowKeyboardCues(keyboardCuesShow);
		}

		if (dwSchema > _XTP_SCHEMA_1500)
		{
			PX_Bool(pPX, _T("AutoScaleDpiGalleryItems"), m_bAutoScaleDpiGalleryItems, TRUE);
			PX_Bool(pPX, _T("AutoScaleDpiToolbarIcons"), m_bAutoScaleDpiToolbarIcons, FALSE);
		}
	}
}

BOOL CXTPCommandBarsOptions::GetGalleryItemsScalling() const
{
	return m_bAutoScaleDpiGalleryItems;
}

BOOL CXTPCommandBarsOptions::GetToolbarsIconsScalling() const
{
	return m_bAutoScaleDpiToolbarIcons;
}

void CXTPCommandBarsOptions::SetDPIScallingOptions(BOOL bScaleGalleryItems, BOOL bScaleToolbarIcons /*= FALSE*/)
{
	m_bAutoScaleDpiGalleryItems = bScaleGalleryItems;

	// we have to set a static variable because it's impossible to retrieve CommandBarsOptions pointer for galleries
	CXTPControlGallery::m_bAutoScaleDpi = m_bAutoScaleDpiGalleryItems;

	m_bAutoScaleDpiToolbarIcons = bScaleToolbarIcons;
}

void CXTPCommandBarsOptions::Serialize(CArchive& ar)
{
	CXTPPropExchangeArchive px(ar);
	DoPropExchange(&px);
}
