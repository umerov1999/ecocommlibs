// XTPRibbonBuilder.cpp : implementation file
//
// This file is a part of the XTREME RIBBON MFC class library.
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
#include "Common/XTPImageManager.h"
#include "Common/XTPHookManager.h"
#include "Common/XTPSystemHelpers.h"

#include "TabManager/XTPTabManager.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPCommandBars.h"
#include "CommandBars/XTPCommandBar.h"
#include "CommandBars/XTPToolBar.h"
#include "CommandBars/XTPMenuBar.h"
#include "CommandBars/XTPPopupBar.h"
#include "CommandBars/XTPControls.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPControlButton.h"
#include "CommandBars/XTPControlPopup.h"
#include "CommandBars/XTPControlExt.h"

#include "XTPRibbonGroups.h"
#include "XTPRibbonGroup.h"
#include "XTPRibbonPopups.h"
#include "XTPRibbonTab.h"
#include "XTPRibbonBar.h"
#include "XTPRibbonQuickAccessControls.h"
#include "XTPRibbonSystemButton.h"

#include "XTPRibbonBuilder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPRibbonBuilder::CXTPRibbonBuilder()
{
	m_pPX = NULL;
}

CXTPRibbonBuilder::~CXTPRibbonBuilder()
{
	SAFE_DELETE(m_pPX);

}


BOOL CXTPRibbonBuilder::LoadFromResource(UINT uiXMLResID, LPCTSTR lpszResType /*= RT_RIBBON*/, HINSTANCE hInstance /*= NULL*/)
{
	SAFE_DELETE(m_pPX);

	m_pPX = new CXTPPropExchangeXMLNode(TRUE, NULL, _T("AFX_RIBBON"));

	if (hInstance == NULL)
		hInstance = AfxGetResourceHandle();

	if (!m_pPX->LoadFromResource(hInstance, MAKEINTRESOURCE(uiXMLResID), lpszResType))
	{
		if (!m_pPX->LoadFromResource(hInstance, MAKEINTRESOURCE(uiXMLResID), _T("RT_RIBBON_XML")))
		{
			SAFE_DELETE(m_pPX);

			return FALSE;
		}
	}


	return TRUE;
}



CCmdTarget* CXTPRibbonBuilder::CreateElement(const CString& strElementName)
{
	if (strElementName == "Category")
		return new CXTPRibbonTab();

	if (strElementName == _T("Panel"))
		return new CXTPRibbonGroup();

	if (strElementName == _T("Button"))
		return new CXTPControlButton();

	if (strElementName == _T("Button_Check"))
		return new CXTPControlCheckBox();

	if (strElementName == _T("Button_Main_Panel"))
		return new CXTPRibbonControlSystemPopupBarButton();

	if (strElementName == _T("Label"))
	{
		CXTPControlLabel* pControlLabel = new CXTPControlLabel();
		pControlLabel->SetStyle(xtpButtonCaption);
		pControlLabel->SetItemDefault(TRUE);
		return pControlLabel;
	}

	return NULL;
}

void CXTPRibbonBuilder::AddImageIndex(int nIndex, int nId, int nImageType)
{
	CUIntArray& arrImages = nImageType == ICON_SMALL ? m_arrImageSmall : m_arrImageLarge;
	while (arrImages.GetSize() <= nIndex)
		arrImages.Add(0);

	arrImages[nIndex] = nId;
}

void CXTPRibbonBuilder::BuildControl(CXTPPropExchange* pPX, CXTPControl* pControl)
{
	CString strName;
	PX_String(pPX, _T("TEXT"), strName);


	CXTPPropExchangeSection pxId(pPX->GetSection(_T("ID")));
	int nId = 0;
	PX_Int(&pxId, _T("VALUE"), nId);


	pControl->SetCaption(strName);
	pControl->SetID(nId);

	CString strKeys;
	PX_String(pPX, _T("KEYS"), strKeys);

	pControl->SetKeyboardTip(strKeys);

	CString strAlwaysLarge;
	PX_String(pPX, _T("ALWAYS_LARGE"), strAlwaysLarge);
	if (strAlwaysLarge == _T("TRUE"))
	{
		pControl->SetStyle(xtpButtonIconAndCaptionBelow);
	}

	int nIndexSmall = -1;
	PX_Int(pPX, _T("INDEX_SMALL"), nIndexSmall);
	if (nIndexSmall >= 0)
	{
		AddImageIndex(nIndexSmall, nId, ICON_SMALL);
	}

	int nIndexLarge = -1;
	PX_Int(pPX, _T("INDEX_LARGE"), nIndexLarge);
	if (nIndexLarge >= 0)
	{
		AddImageIndex(nIndexLarge, nId, ICON_BIG);
	}

}

BOOL CXTPRibbonBuilder::BuildGroupControls(CXTPPropExchange* pPX, CXTPRibbonGroup* pRibbonGroup)
{
	CXTPPropExchangeEnumeratorPtr pEnumerator(pPX->GetEnumerator(_T("ELEMENT")));

	POSITION pos = pEnumerator->GetPosition(0);
	while (pos)
	{
		CXTPPropExchangeSection pxControl(pEnumerator->GetNext(pos));

		CString strElementName;
		PX_String(&pxControl, _T("ELEMENT_NAME"), strElementName);

		CCmdTarget* pElement = CreateElement(strElementName);
		if (pElement == NULL)
			continue;

		CXTPControl* pControl = DYNAMIC_DOWNCAST(CXTPControl, pElement);

		if (!pControl)
		{
			delete pElement;
			continue;

		}
		CXTPPropExchangeSection pxElements(pxControl->GetSection(_T("ELEMENTS")));
		BuildControlPopupBar(&pxElements, pControl, RUNTIME_CLASS(CXTPPopupBar));

		BuildControl(&pxControl, pControl);

		pRibbonGroup->Add(pControl, pControl->GetID());

	}

	return TRUE;
}

BOOL CXTPRibbonBuilder::BuildGroups(CXTPPropExchange* pPX, CXTPRibbonTab* pRibbonTab)
{
	CXTPPropExchangeEnumeratorPtr pEnumerator(pPX->GetEnumerator(_T("PANEL")));

	POSITION pos = pEnumerator->GetPosition(0);
	while (pos)
	{
		CXTPPropExchangeSection pxGroup(pEnumerator->GetNext(pos));

		CString strElementName;
		PX_String(&pxGroup, _T("ELEMENT_NAME"), strElementName);

		CCmdTarget* pElement = CreateElement(strElementName);
		if (pElement == NULL)
			continue;

		CXTPRibbonGroup* pGroup = DYNAMIC_DOWNCAST(CXTPRibbonGroup, pElement);

		if (!pGroup)
		{
			delete pElement;
			continue;
		}

		CString strName;
		PX_String(&pxGroup, _T("NAME"), strName);

		CString strKey;
		PX_String(&pxGroup, _T("KEYS"), strKey);


		pGroup->SetCaption(strName);
		pGroup->GetControlGroupPopup()->SetKeyboardTip(strKey);

		pRibbonTab->GetGroups()->InsertAt(pRibbonTab->GetGroups()->GetCount(), pGroup);

		CXTPPropExchangeSection pxGroupElements(pxGroup->GetSection(_T("ELEMENTS")));
		BuildGroupControls(&pxGroupElements, pGroup);
	}

	return TRUE;
}

void CXTPRibbonBuilder::LoadIcons(int nId, CUIntArray& arrIcons)
{
	CXTPImageManager* pImageManager = m_pCommandBars->GetImageManager();


	HBITMAP hBitmap = CXTPImageManagerIcon::LoadBitmapFromResource(MAKEINTRESOURCE(nId), NULL);
	if (hBitmap)
	{
		BITMAP bmpInfo;
		::GetObject(hBitmap, sizeof(BITMAP), &bmpInfo);

		CSize szBitmap = CSize(bmpInfo.bmWidth, bmpInfo.bmHeight);

		int nCount = 0;

		if (szBitmap.cy >= 15 && szBitmap.cy <= 16)
		{
			nCount = szBitmap.cx / 16;
		}
		else if (szBitmap.cy == 32)
		{
			nCount = szBitmap.cx / 32;
		}

		while (arrIcons.GetSize() < nCount)
			arrIcons.Add(0);

		DeleteObject(hBitmap);
	}

	pImageManager->SetIcons(nId, arrIcons.GetData(), (int)arrIcons.GetSize(), CSize(0, 0), xtpImageNormal);

}

void CXTPRibbonBuilder::LoadIcons(CXTPPropExchange* pPX)
{
	CXTPPropExchangeSection pxImage(pPX->GetSection(_T("IMAGE")));
	CXTPPropExchangeSection pxImageId(pxImage->GetSection(_T("ID")));

	int nId = -1;
	PX_Int(&pxImageId, _T("VALUE"), nId);

	if (nId != -1)
	{
		LoadIcons(nId, m_arrImageSmall);
	}


	CXTPPropExchangeSection pxImageSmall(pPX->GetSection(_T("IMAGE_SMALL")));
	CXTPPropExchangeSection pxImageSmallId(pxImageSmall->GetSection(_T("ID")));

	nId = -1;
	PX_Int(&pxImageSmallId, _T("VALUE"), nId);

	if (nId != -1)
	{
		LoadIcons(nId, m_arrImageSmall);
	}


	CXTPPropExchangeSection pxImageLarge(pPX->GetSection(_T("IMAGE_LARGE")));
	CXTPPropExchangeSection pxImageLargeId(pxImageLarge->GetSection(_T("ID")));

	nId = -1;
	PX_Int(&pxImageLargeId, _T("VALUE"), nId);

	if (nId != -1)
	{
		LoadIcons(nId, m_arrImageLarge);
	}
}

BOOL CXTPRibbonBuilder::BuildCategories(CXTPPropExchange* pPX, CXTPRibbonBar* pRibbonBar)
{
	CXTPPropExchangeEnumeratorPtr pEnumerator(pPX->GetEnumerator(_T("CATEGORY")));

	POSITION pos = pEnumerator->GetPosition(0);
	while (pos)
	{
		CXTPPropExchangeSection pxTab(pEnumerator->GetNext(pos));

		CString strElementName;
		PX_String(&pxTab, _T("ELEMENT_NAME"), strElementName);

		CCmdTarget* pElement = CreateElement(strElementName);
		if (pElement == NULL)
			continue;

		CXTPRibbonTab* pTab = DYNAMIC_DOWNCAST(CXTPRibbonTab, pElement);

		if (!pTab)
		{
			delete pElement;
			continue;
		}


		CString strName;
		PX_String(&pxTab, _T("NAME"), strName);

		pTab->SetCaption(strName);


		CString strKeys;
		PX_String(&pxTab, _T("KEYS"), strKeys);
		pTab->SetKeyboardTip(strKeys);


		pRibbonBar->InsertTab(pRibbonBar->GetTabCount(), pTab);


		m_arrImageLarge.RemoveAll();
		m_arrImageSmall.RemoveAll();

		CXTPPropExchangeSection pxPanels(pxTab->GetSection(_T("PANELS")));
		BuildGroups(&pxPanels, pTab);

		LoadIcons(&pxTab);
	}


	return TRUE;
}

BOOL CXTPRibbonBuilder::BuildMainButton(CXTPPropExchange* pPX, CXTPRibbonBar* pRibbonBar)
{
	CString strElementName;
	PX_String(pPX, _T("ELEMENT_NAME"), strElementName);

	if (strElementName != _T("Button_Main"))
		return FALSE;

	CXTPControl* pControl = pRibbonBar->AddSystemButton(0);


	CXTPPropExchangeSection pxImage(pPX->GetSection(_T("IMAGE")));
	CXTPPropExchangeSection pxImageId(pxImage->GetSection(_T("ID")));

	int nId = -1;
	PX_Int(&pxImageId, _T("VALUE"), nId);

	if (nId != -1)
	{
		UINT nIcons[] = {(UINT)nId};

		pRibbonBar->GetImageManager()->SetIcons(nId, nIcons, 1, CSize(0, 0), xtpImageNormal);

		pControl->SetID(nId);
	}

	CString strKeys;
	PX_String(pPX, _T("KEYS"), strKeys);
	pControl->SetKeyboardTip(strKeys);


	return TRUE;
}

void CXTPRibbonBuilder::BuildControlPopupBar(CXTPPropExchange* pPX, CXTPControl*& pParent, CRuntimeClass* pPopupBarClass)
{
	CXTPPropExchangeEnumeratorPtr pEnumerator(pPX->GetEnumerator(_T("ELEMENT")));

	BOOL bBeginGroup = FALSE;
	BOOL bEmpty = TRUE;

	POSITION pos = pEnumerator->GetPosition(0);
	while (pos)
	{
		CXTPPropExchangeSection pxControl(pEnumerator->GetNext(pos));

		CString strElementName;
		PX_String(&pxControl, _T("ELEMENT_NAME"), strElementName);

		if (strElementName == _T("Separator"))
		{
			bBeginGroup = TRUE;
			continue;
		}


		CCmdTarget* pElement = CreateElement(strElementName);
		if (pElement == NULL)
			continue;

		CXTPControl* pControl = DYNAMIC_DOWNCAST(CXTPControl, pElement);

		if (!pControl)
		{
			delete pElement;
			continue;
		}

		if (bEmpty)
		{
			CXTPControlPopup* pPopupButton = CXTPControlPopup::CreateControlPopup(xtpControlPopup);

			CXTPPopupBar* pPopupBar = (CXTPPopupBar*)pPopupBarClass->CreateObject();

			if (pPopupBar->IsKindOf(RUNTIME_CLASS(CXTPRibbonSystemPopupBarPage)))
			{
				pPopupBar->SetIconSize(CSize(32, 32));
				pPopupBar->SetWidth(300);
				pPopupBar->SetShowGripper(FALSE);
				pPopupBar->SetDefaultButtonStyle(xtpButtonCaptionAndDescription);
			}


			pPopupButton->SetCommandBar(pPopupBar);
			pPopupBar->InternalRelease();

			pParent->InternalRelease();
			pParent = pPopupButton;

			bEmpty = FALSE;
		}


		BuildControl(&pxControl, pControl);

		pParent->GetCommandBar()->GetControls()->Add(pControl, pControl->GetID());

		pParent->SetBeginGroup(bBeginGroup);
		bBeginGroup = FALSE;
	}
}

BOOL CXTPRibbonBuilder::BuildMainButtonPopupBar(CXTPPropExchange* pPX, CXTPRibbonBar* pRibbonBar)
{
	if (!pRibbonBar->GetSystemButton())
		return FALSE;

	CXTPCommandBar* pPopupBar = pRibbonBar->GetSystemButton()->GetCommandBar();


	CString strElementName;
	PX_String(pPX, _T("ELEMENT_NAME"), strElementName);

	if (strElementName != _T("Category_Main"))
		return FALSE;

	CString strCaption;
	PX_String(pPX, _T("NAME"), strCaption);

	pRibbonBar->GetSystemButton()->SetCaption(strCaption);


	pPopupBar->SetIconSize(CSize(32, 32));

	m_arrImageLarge.RemoveAll();
	m_arrImageSmall.RemoveAll();

	CXTPPropExchangeSection pxElements(pPX->GetSection(_T("ELEMENTS")));

	CXTPPropExchangeEnumeratorPtr pEnumerator(pxElements->GetEnumerator(_T("ELEMENT")));

	BOOL bBeginGroup = FALSE;

	POSITION pos = pEnumerator->GetPosition(0);
	while (pos)
	{
		CXTPPropExchangeSection pxControl(pEnumerator->GetNext(pos));

		CString strElementName;
		PX_String(&pxControl, _T("ELEMENT_NAME"), strElementName);

		if (strElementName == _T("Separator"))
		{
			bBeginGroup = TRUE;
			continue;
		}


		CCmdTarget* pElement = CreateElement(strElementName);
		if (pElement == NULL)
			continue;

		CXTPControl* pControl = DYNAMIC_DOWNCAST(CXTPControl, pElement);

		if (!pControl)
		{
			delete pElement;
			continue;
		}

		CXTPPropExchangeSection pxElements(pxControl->GetSection(_T("ELEMENTS")));
		BuildControlPopupBar(&pxElements, pControl, RUNTIME_CLASS(CXTPRibbonSystemPopupBarPage));

		BuildControl(&pxControl, pControl);

		pPopupBar->GetControls()->Add(pControl, pControl->GetID());

		pControl->SetBeginGroup(bBeginGroup);
		bBeginGroup = FALSE;

	}

	CXTPPropExchangeSection pxRecentFileList(pPX->GetSection(_T("RECENT_FILE_LIST")));

	CString strEnabled;
	PX_String(&pxRecentFileList, _T("ENABLE"), strEnabled);
	if (strEnabled == _T("TRUE"))
	{
		CXTPControl* pControl = pPopupBar->GetControls()->Add(new CXTPRibbonControlSystemRecentFileList());

		CString strLabel;
		PX_String(&pxRecentFileList, _T("LABEL"), strLabel);
		pControl->SetCaption(strLabel);
	}


	LoadIcons(pPX);


	return TRUE;
}

void CXTPRibbonBuilder::BuildQATElements(CXTPPropExchange* pPX, CXTPRibbonBar* pRibbonBar)
{
	CXTPPropExchangeSection pxItems(pPX->GetSection(_T("ITEMS")));

	CXTPPropExchangeEnumeratorPtr pEnumerator(pxItems->GetEnumerator(_T("ITEM")));


	POSITION pos = pEnumerator->GetPosition(0);
	while (pos)
	{
		CXTPPropExchangeSection pxItem(pEnumerator->GetNext(pos));

		CXTPPropExchangeSection pxItemId(pxItem->GetSection(_T("ID")));

		int nId = 0;
		PX_Int(&pxItemId, _T("VALUE"), nId);

		if (nId > 0)
		{
			pRibbonBar->GetQuickAccessControls()->Add(xtpControlButton, nId);
		}

	}

	pRibbonBar->GetQuickAccessControls()->CreateOriginalControls();
}

BOOL CXTPRibbonBuilder::BuildTabElements(CXTPPropExchange* pPX, CXTPRibbonBar* pRibbonBar)
{

	CXTPPropExchangeSection pxElements(pPX->GetSection(_T("ELEMENTS")));

	CXTPPropExchangeEnumeratorPtr pEnumerator(pxElements->GetEnumerator(_T("ELEMENT")));

	BOOL bBeginGroup = FALSE;

	POSITION pos = pEnumerator->GetPosition(0);
	while (pos)
	{
		CXTPPropExchangeSection pxControl(pEnumerator->GetNext(pos));

		CString strElementName;
		PX_String(&pxControl, _T("ELEMENT_NAME"), strElementName);

		if (strElementName == _T("Separator"))
		{
			bBeginGroup = TRUE;
			continue;
		}


		CCmdTarget* pElement = CreateElement(strElementName);
		if (pElement == NULL)
			continue;

		CXTPControl* pControl = DYNAMIC_DOWNCAST(CXTPControl, pElement);

		if (!pControl)
		{
			delete pElement;
			continue;
		}

		CXTPPropExchangeSection pxElements(pxControl->GetSection(_T("ELEMENTS")));
		BuildControlPopupBar(&pxElements, pControl, RUNTIME_CLASS(CXTPPopupBar));

		BuildControl(&pxControl, pControl);

		pRibbonBar->GetControls()->Add(pControl);

		pControl->SetFlags(pControl->GetFlags() | xtpFlagRightAlign);

		pControl->SetBeginGroup(bBeginGroup);
		bBeginGroup = FALSE;

	}


	return TRUE;
}

BOOL CXTPRibbonBuilder::Build(CXTPRibbonBar* pRibbonBar)
{
	if (!m_pPX)
		return FALSE;

	m_pCommandBars = pRibbonBar->GetCommandBars();

	CXTPPropExchangeSection pxRibbonBar(m_pPX->GetSection(_T("RIBBON_BAR")));

	CString strElementName;
	PX_String(&pxRibbonBar, _T("ELEMENT_NAME"), strElementName);
	if (strElementName != _T("RibbonBar"))
		return FALSE;

	CXTPPropExchangeSection pxButtonMain(pxRibbonBar->GetSection(_T("BUTTON_MAIN")));
	BuildMainButton(&pxButtonMain, pRibbonBar);

	CXTPPropExchangeSection pxCategoryMain(pxRibbonBar->GetSection(_T("CATEGORY_MAIN")));
	BuildMainButtonPopupBar(&pxCategoryMain, pRibbonBar);

	CXTPPropExchangeSection pxQATElements(pxRibbonBar->GetSection(_T("QAT_ELEMENTS")));
	BuildQATElements(&pxQATElements, pRibbonBar);


	m_arrImageLarge.RemoveAll();
	m_arrImageSmall.RemoveAll();

	CXTPPropExchangeSection pxTabElements(pxRibbonBar->GetSection(_T("TAB_ELEMENTS")));
	BuildTabElements(&pxTabElements, pRibbonBar);

	LoadIcons(&pxRibbonBar);

	CXTPPropExchangeSection pxCategories(pxRibbonBar->GetSection(_T("CATEGORIES")));
	BuildCategories(&pxCategories, pRibbonBar);



	return TRUE;
}
