// XTPSkinManager.cpp: implementation of the CXTPSkinManager class.
//
// This file is a part of the XTREME SKINFRAMEWORK MFC class library.
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

#include "Common/Tmschema.h"
#include "Common/Uxtheme.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPSystemHelpers.h"

#include "XTPSkinObject.h"
#include "XTPSkinManagerColorFilter.h"
#include "XTPSkinManager.h"
#include "XTPSkinManagerSchema.h"
#include "XTPSkinImage.h"
#include "XTPSkinManagerResource.h"
#include "XTPSkinObject.h"
#include "XTPSkinObjectFrame.h"
#include "XTPSkinManagerSchema.h"
#include "XTPSkinManagerApiHook.h"
#include "XTPSkinManagerModuleList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define EVENT_OBJECT_REORDER 0x8004

CXTPSkinManagerMetrics::CXTPSkinManagerMetrics(CXTPSkinManagerSchema* pSchema)
{
	m_pSchema = pSchema;

	m_brushDialog = NULL;
	m_brushTabControl = NULL;
	m_brushWindow = NULL;

	ZeroMemory(m_brTheme, sizeof(m_brTheme));
	memset(m_clrTheme, -1, sizeof(m_clrTheme));

	m_cyCaption = m_cySmallCaption = m_cyOsCaption = m_cyOsSmallCaption = 0;

	m_bRefreshMetrics = FALSE;
}

CXTPSkinManagerMetrics::~CXTPSkinManagerMetrics()
{
	DestroyMetrics();
}

COLORREF CXTPSkinManagerMetrics::GetColor(int nIndex) const
{
	return nIndex < XTP_SKINMETRICS_COLORTABLESIZE ? m_clrTheme[nIndex] : 0;
}

BOOL CXTPSkinManagerMetrics::IsMetricObject(HGDIOBJ hObject) const
{
	if (hObject == 0)
		return FALSE;

	if (GetObjectType(hObject) != OBJ_BRUSH)
		return FALSE;

	if (hObject == m_brushWindow || hObject == m_brushDialog || hObject == m_brushTabControl)
		return TRUE;

	for (int i = 0; i < XTP_SKINMETRICS_COLORTABLESIZE; i++)
	{
		if (hObject == m_brTheme[i])
			return TRUE;
	}
	return FALSE;
}

void CXTPSkinManagerMetrics::DeleteSysBrush(HBRUSH* pBrush)
{
	if (pBrush && *pBrush)
	{
		HBRUSH hBrush = *pBrush;
		*pBrush = NULL;
		DeleteObject(hBrush);
	}
}

void CXTPSkinManagerMetrics::DestroyMetrics()
{
	m_fntCaption.DeleteObject();
	m_fntSmCaption.DeleteObject();
	m_fntMenu.DeleteObject();

	for (int iColorId = 0; iColorId < XTP_SKINMETRICS_COLORTABLESIZE; iColorId++)
	{
		DeleteSysBrush(&m_brTheme[iColorId]);
	}

	DeleteSysBrush(&m_brushDialog);
	DeleteSysBrush(&m_brushTabControl);
	DeleteSysBrush(&m_brushWindow);
}

void CXTPSkinManagerMetrics::RefreshMetrics()
{
	static const COLORREF defColors[XTP_SKINMETRICS_COLORTABLESIZE] =
	{
		RGB(192, 192, 192),
		RGB( 58, 110, 165),
		RGB(  0,   0, 128),
		RGB(128, 128, 128),
		RGB(192, 192, 192),
		RGB(255, 255, 255),
		RGB(  0,   0,   0),
		RGB(  0,   0,   0),
		RGB(  0,   0,   0),
		RGB(255, 255, 255),
		RGB(192, 192, 192),
		RGB(192, 192, 192),
		RGB(128, 128, 128),
		RGB(  0,   0, 128),
		RGB(255, 255, 255),
		RGB(192, 192, 192),
		RGB(128, 128, 128),
		RGB(128, 128, 128),
		RGB(  0,   0,   0),
		RGB(192, 192, 192),
		RGB(255, 255, 255),
		RGB(  0,   0,   0),
		RGB(223, 223, 223),
		RGB(  0,   0,   0),
		RGB(255, 255, 225),
		RGB(180, 180, 180),
		RGB(  0,   0, 255),
		RGB( 16, 132, 208),
		RGB(181, 181, 181),
		RGB(  0,   0, 128),
		RGB(192, 192, 192)
	};
	CXTPSkinManagerSchema* pSchema = m_pSchema;
	if (!pSchema)
	{
		for (int iColorId = 0; iColorId < XTP_SKINMETRICS_COLORTABLESIZE; iColorId++)
		{
			m_clrTheme[iColorId] = GetSysColor(iColorId);

			DeleteSysBrush(&m_brTheme[iColorId]);
		}
		return;
	}
	m_bRefreshMetrics = TRUE;

	UINT nSysMetrics = pSchema->GetClassCode(_T("SYSMETRICS"));

	NONCLIENTMETRICS ncm;
	ZeroMemory(&ncm, sizeof(NONCLIENTMETRICS));
	ncm.cbSize = sizeof(NONCLIENTMETRICS);
	VERIFY(::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0));

	m_fntCaption.DeleteObject();
	m_fntCaption.CreateFontIndirect(&ncm.lfCaptionFont);

	m_fntSmCaption.DeleteObject();
	m_fntSmCaption.CreateFontIndirect(&ncm.lfSmCaptionFont);

	m_fntMenu.DeleteObject();
	m_fntMenu.CreateFontIndirect(&ncm.lfMenuFont);

	m_nBorderSize = 1;
	SystemParametersInfo(SPI_GETBORDER, 0, &m_nBorderSize, FALSE);

	m_cxBorder = GetSystemMetrics(SM_CXBORDER);
	m_cyBorder = GetSystemMetrics(SM_CYBORDER);
	m_cxEdge = GetSystemMetrics(SM_CXEDGE);
	m_cyEdge = GetSystemMetrics(SM_CYEDGE);

	m_cxHScroll = GetSystemMetrics(SM_CXHSCROLL);
	m_cyHScroll = GetSystemMetrics(SM_CYHSCROLL);
	m_cxOsVScroll = m_cxVScroll = GetSystemMetrics(SM_CXVSCROLL);
	m_cyVScroll = GetSystemMetrics(SM_CYVSCROLL);

	BOOL bApplyMetrics = pSchema->GetSkinManager()->GetApplyOptions() & xtpSkinApplyMetrics;

	m_cyOsCaption = m_cyCaption = GetSystemMetrics(SM_CYCAPTION);
	m_cyOsSmallCaption = m_cySmallCaption = GetSystemMetrics(SM_CYSMCAPTION);

	if (bApplyMetrics)
	{
		pSchema->GetIntProperty(nSysMetrics, 0, 0, TMT_CAPTIONBARHEIGHT, m_cyCaption);
		m_cyCaption++;

		pSchema->GetIntProperty(nSysMetrics, 0, 0, TMT_SMCAPTIONBARHEIGHT, m_cySmallCaption);
		m_cySmallCaption++;

		int nScrollbarWidth = 0, nScrollbarHeight = 0;
		if (SUCCEEDED(pSchema->GetIntProperty(nSysMetrics, 0, 0, TMT_SCROLLBARWIDTH, nScrollbarWidth)) && nScrollbarWidth > 0)
		{
			m_cxHScroll = m_cxVScroll = nScrollbarWidth;
		}
		if (SUCCEEDED(pSchema->GetIntProperty(nSysMetrics, 0, 0, TMT_SCROLLBARHEIGHT, nScrollbarHeight)) && nScrollbarHeight > 0)
		{
			m_cyHScroll = m_cyVScroll = nScrollbarHeight;
		}

		LOGFONT lfCaption, lfSmCaption, lfMenu;
		if (SUCCEEDED(pSchema->GetFontProperty(nSysMetrics, 0, 0, TMT_CAPTIONFONT, lfCaption)))
		{
			m_fntCaption.DeleteObject();
			m_fntCaption.CreateFontIndirect(&lfCaption);
		}

		if (SUCCEEDED(pSchema->GetFontProperty(nSysMetrics, 0, 0, TMT_SMALLCAPTIONFONT, lfSmCaption)))
		{
			m_fntSmCaption.DeleteObject();
			m_fntSmCaption.CreateFontIndirect(&lfSmCaption);
		}

		if (SUCCEEDED(pSchema->GetFontProperty(nSysMetrics, 0, 0, TMT_MENUFONT, lfMenu)))
		{
			m_fntMenu.DeleteObject();
			m_fntMenu.CreateFontIndirect(&lfMenu);
		}
	}

	for (int iColorId = 0; iColorId < XTP_SKINMETRICS_COLORTABLESIZE; iColorId++)
	{
		COLORREF clrOld = m_clrTheme[iColorId];
		m_clrTheme[iColorId] = (COLORREF)-1;

		COLORREF clrVal;

		if (FAILED(pSchema->GetColorProperty(nSysMetrics, 0, 0, iColorId + TMT_FIRSTCOLOR, clrVal)))
		{
			clrVal = defColors[iColorId];

			pSchema->GetSkinManager()->ApplyColorFilter(clrVal);
		}

		m_clrTheme[iColorId] = clrVal;

		if ((clrOld != clrVal) || (m_brTheme[iColorId] == NULL))
		{
			DeleteSysBrush(&m_brTheme[iColorId]);
			m_brTheme[iColorId] = CreateSolidBrush(clrVal);
		}
	}

	m_clrEdgeHighLight = m_clrTheme[COLOR_BTNHIGHLIGHT];
	m_clrEdgeShadow = m_clrTheme[COLOR_BTNSHADOW];
	m_clrEdgeLight = m_clrTheme[COLOR_3DLIGHT];
	m_clrEdgeDkShadow = m_clrTheme[COLOR_3DDKSHADOW];


	DeleteSysBrush(&m_brushDialog);
	m_brushDialog = ::CreateSolidBrush(m_clrTheme[COLOR_3DFACE]);

	DeleteSysBrush(&m_brushTabControl);

	DeleteSysBrush(&m_brushWindow);

	m_brushWindow = ::CreateSolidBrush(m_clrTheme[COLOR_WINDOW]);

	m_bRefreshMetrics = FALSE;

}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// CXTPSkinManager

class XTP_SKINAMANGER_THREAD_STATE : public CNoTrackObject
{
public:
	XTP_SKINAMANGER_THREAD_STATE();
	virtual ~XTP_SKINAMANGER_THREAD_STATE();

public:
	HHOOK m_hHookOldCbtFilter;
};

XTP_SKINAMANGER_THREAD_STATE::XTP_SKINAMANGER_THREAD_STATE()
{
	m_hHookOldCbtFilter = 0;
}

XTP_SKINAMANGER_THREAD_STATE::~XTP_SKINAMANGER_THREAD_STATE()
{
	if (m_hHookOldCbtFilter)
	{
		UnhookWindowsHookEx(m_hHookOldCbtFilter);
	}
}



THREAD_LOCAL(XTP_SKINAMANGER_THREAD_STATE, _xtpSkinManagerThreadState)


CXTPSkinManager::CXTPSkinManager()
{
	::InitializeCriticalSection(&m_csObjects);

	m_pSchema = NULL;
	m_bEnabled = FALSE;

	m_pResourceFile = new CXTPSkinManagerResourceFile(this);

	m_pClassMap = new CXTPSkinObjectClassMap();
	m_pClassMap->AddStandardClasses();

	m_mapObjects.InitHashTable(199, FALSE);

	m_bAutoApplyWindows = TRUE;
	m_bAutoApplyThreads = TRUE;
	m_dwApplyOptions = xtpSkinApplyFrame | xtpSkinApplyColors | xtpSkinApplyMetrics | xtpSkinApplyMenus;

	m_dwComCtrl = XTPSystemVersion()->GetComCtlVersion();

	m_bWin9x = XTPSystemVersion()->IsWin9x();

	m_hWinEventHook = NULL;
	m_pSetWinEventHook = NULL;
	m_pUnhookWinEvent = NULL;

	CXTPSkinManagerApiHook* pApiHook = CXTPSkinManagerApiHook::GetInstance();
	pApiHook;

	HMODULE hLib = GetModuleHandle(_T("USER32"));
	if (hLib)
	{
		m_pSetWinEventHook = (LPFNSETWINEVENTHOOK)GetProcAddress(hLib, "SetWinEventHook");
		m_pUnhookWinEvent = (LPFNUNHOOKWINEVENT)GetProcAddress(hLib, "UnhookWinEvent");

		if (m_pSetWinEventHook && m_pUnhookWinEvent)
		{
			m_hWinEventHook = m_pSetWinEventHook(EVENT_OBJECT_REORDER, EVENT_OBJECT_REORDER, NULL,
				&CXTPSkinManager::WinEventProc, GetCurrentProcessId(), 0, 0);
		}
	}
	EnableCurrentThread();

#ifdef _AFXDLL
	m_pModuleState = AfxGetModuleState();
#endif

}

void CXTPSkinManager::EnableCurrentThread()
{
	DWORD dwThread = ::GetCurrentThreadId();

	XTP_SKINAMANGER_THREAD_STATE* pThreadState = _xtpSkinManagerThreadState.GetData();
	if (pThreadState)
	{
		if (pThreadState->m_hHookOldCbtFilter == 0)
		{
			pThreadState->m_hHookOldCbtFilter = ::SetWindowsHookEx(WH_CBT, CbtFilterHook, NULL, dwThread);
		}
	}
}


CXTPSkinManager::~CXTPSkinManager()
{
	FreeSkinData();
	RemoveColorFilters();

	CMDTARGET_RELEASE(m_pSchema);
	CMDTARGET_RELEASE(m_pResourceFile);
	SAFE_DELETE(m_pClassMap);

	RemoveAll();

	CXTPSkinManagerApiHook* pApiHook = CXTPSkinManagerApiHook::GetInstance();
	pApiHook->FinalizeHookManagement();

	HMODULE hLib = GetModuleHandle(_T("USER32"));
	if (m_hWinEventHook && hLib && m_pUnhookWinEvent)
	{
		m_pUnhookWinEvent(m_hWinEventHook);
	}

	::DeleteCriticalSection(&m_csObjects);
}

BOOL CXTPSkinManager::IsColorFilterExists() const
{
	return m_arrFilters.GetSize() > 0;
}

void CXTPSkinManager::RemoveColorFilters()
{
	for (int i = 0; i < (int)m_arrFilters.GetSize(); i++)
	{
		delete m_arrFilters[i];
	}
	m_arrFilters.RemoveAll();
}

void CXTPSkinManager::AddColorFilter(CXTPSkinManagerColorFilter* pFilter)
{
	m_arrFilters.Add(pFilter);
}

void CXTPSkinManager::RedrawAllControls()
{
	if (!m_bEnabled)
		return;

	if (!m_pSchema)
		return;

	POSITION pos = m_pSchema->m_mapClasses.GetStartPosition();
	CString strClassName;
	CXTPSkinManagerClass* pClass;
	while (pos != NULL)
	{
		m_pSchema->m_mapClasses.GetNextAssoc( pos, strClassName, (void*&)pClass);
		pClass->GetImages()->RemoveAll();
	}

	m_pSchema->GetMetrics()->RefreshMetrics();
	XTPColorManager()->RefreshColors();

	OnSkinChanged(TRUE, TRUE);
}

void CXTPSkinManager::ApplyColorFilter(COLORREF& clr)
{
	for (int i = 0; i < (int)m_arrFilters.GetSize(); i++)
	{
		m_arrFilters[i]->ApplyColorFilter(clr);
	}
}

CXTPSkinManagerSchema* CXTPSkinManager::CreateSchema(LPCTSTR lpszResourcePath, LPCTSTR lpszIniFileName)
{
	if (XTPColorManager()->IsLowResolution() || !CXTPSkinManagerModuleList::IsEnumeratorExists())
		return NULL;

	CXTPSkinManagerResourceFile* pResourceFile = new CXTPSkinManagerResourceFile(this);
	if (!pResourceFile->Open(lpszResourcePath, lpszIniFileName))
	{
		CMDTARGET_RELEASE(pResourceFile);
		return NULL;
	}

	CXTPSkinManagerSchema* pSchema = pResourceFile->CreateSchema();
	if (!pSchema)
	{
		CMDTARGET_RELEASE(pResourceFile);
		return NULL;

	}

	CMDTARGET_RELEASE(pResourceFile);

	if (FAILED(pSchema->ReadProperties()))
	{
		CMDTARGET_RELEASE(pSchema);
		return NULL;
	}

	pSchema->RefreshMetrcis();
	return pSchema;
}

BOOL CXTPSkinManager::LoadSkin(LPCTSTR lpszResourcePath, LPCTSTR lpszIniFileName)
{
	CMDTARGET_RELEASE(m_pSchema);

	CXTPSkinManagerApiHook* pApiHook = CXTPSkinManagerApiHook::GetInstance();
	pApiHook->FinalizeHookManagement();

	XTPSoundManager()->StopThread();

	BOOL bEnabled = m_bEnabled;

	if (XTPColorManager()->IsLowResolution() ||
		!CXTPSkinManagerModuleList::IsEnumeratorExists() ||
		!ReadSkinData(lpszResourcePath, lpszIniFileName))
	{
		m_bEnabled = FALSE;
	}
	else
	{
		m_bEnabled = TRUE;
		
		pApiHook->InitializeHookManagement();
		
		m_pSchema->RefreshMetrcis();
	}

	XTPColorManager()->RefreshColors();

	OnSkinChanged(bEnabled, m_bEnabled);

	return m_bEnabled;
}

CXTPSkinManagerMetrics* CXTPSkinManager::GetMetrics() const
{
	return m_pSchema ? m_pSchema->GetMetrics() : NULL;
}

CXTPSkinManagerClass* CXTPSkinManager::FromHandle(HTHEME hTheme)
{
	if (!m_pSchema)
		return NULL;

	CMapStringToPtr& mapClasses = m_pSchema->m_mapClasses;

	POSITION pos = mapClasses.GetStartPosition();
	CString strClassName;
	CXTPSkinManagerClass* pClass;

	while (pos != NULL)
	{
		mapClasses.GetNextAssoc( pos, strClassName, (void*&)pClass);
		if (pClass == (CXTPSkinManagerClass*)hTheme)
			return pClass;
	}

	return NULL;
}

void CXTPSkinManager::ExcludeModule(LPCTSTR lpszModule)
{
	CXTPSkinManagerApiHook* pApiHook = CXTPSkinManagerApiHook::GetInstance();
	pApiHook->ExcludeModule(lpszModule, FALSE);
}

void CXTPSkinManager::SetApplyOptions(DWORD dwOptions)
{
	m_dwApplyOptions = dwOptions;

	if (!m_bEnabled)
		return;

	CXTPSkinManagerApiHook* pApiHook = CXTPSkinManagerApiHook::GetInstance();

	pApiHook->FinalizeHookManagement();
	pApiHook->InitializeHookManagement();

	m_pSchema->RefreshMetrcis();
	XTPColorManager()->RefreshColors();

	OnSkinChanged(m_bEnabled, m_bEnabled);
}


void CXTPSkinManager::SetResourceFile(CXTPSkinManagerResourceFile* pResourceFile)
{
	ASSERT(pResourceFile);
	if (!pResourceFile)
		return;

	CMDTARGET_RELEASE(m_pResourceFile);

	m_pResourceFile = pResourceFile;
	m_pResourceFile->m_pManager = this;
}

void CXTPSkinManager::FreeSkinData()
{
	m_pResourceFile->Close();
}

BOOL CXTPSkinManager::ReadSkinData(LPCTSTR strResourcePath, LPCTSTR strIniFileName)
{
	FreeSkinData();

	if (!m_pResourceFile->Open(strResourcePath, strIniFileName))
		return FALSE;

	m_pSchema = m_pResourceFile->CreateSchema();
	if (!m_pSchema)
		return FALSE;

	if (FAILED(m_pSchema->ReadProperties()))
		return FALSE;

	return TRUE;
}

CXTPSkinManager* CXTPSkinManager::s_pInstance = NULL;

static class CXTPSkinManager::CDestructor
{
public:
	~CDestructor()
	{
		if (s_pInstance != NULL)
			delete s_pInstance;
	}

} __xtpSkinManagerDestructor;

LRESULT CALLBACK CXTPSkinManager::CbtFilterHook(int code, WPARAM wParam, LPARAM lParam)
{
	XTP_SKINAMANGER_THREAD_STATE* pThreadState = _xtpSkinManagerThreadState.GetData();

	LRESULT lResult = CallNextHookEx(pThreadState->m_hHookOldCbtFilter, code,
			wParam, lParam);

	if (code == HCBT_CREATEWND && XTPSkinManager()->m_bAutoApplyWindows)
	{
		ASSERT(lParam != NULL);
		LPCREATESTRUCT lpcs = ((LPCBT_CREATEWND)lParam)->lpcs;
		ASSERT(lpcs != NULL);

		ASSERT(wParam != NULL); // should be non-NULL HWND
		HWND hWnd = (HWND)wParam;

		TCHAR lpszClassName[50];
		GetClassName(hWnd, lpszClassName, 50);

		if (XTPSkinManager()->Lookup(hWnd) != NULL)
		{
			TRACE(_T("Warning: Window already attached\n"));
		}
		else
		{
			XTPSkinManager()->SetHook(hWnd, lpszClassName, lpcs, TRUE);
		}
	}

	return lResult;
}

_XTP_EXT_CLASS CXTPSkinManager* AFX_CDECL XTPSkinManager()
{
	if (CXTPSkinManager::s_pInstance == NULL)
		CXTPSkinManager::s_pInstance = new CXTPSkinManager();

	return CXTPSkinManager::s_pInstance;
}

CXTPSkinManager* CXTPSkinManager::SetSkinManager(CXTPSkinManager* pSkinManager, BOOL bDelete /*=TRUE*/)
{
	ASSERT(pSkinManager != NULL);

	if (pSkinManager != NULL)
	{
		CXTPSkinManager* pPrevInstance = NULL;

		if (bDelete) {
			SAFE_DELETE(s_pInstance);
		}
		else {
			pPrevInstance = s_pInstance;
		}

		s_pInstance = pSkinManager;
		return pPrevInstance;
	}

	return NULL;
}

CXTPSkinManagerClass* CXTPSkinManager::GetSkinClass(const CXTPSkinObject* pObject, CString strClassList)
{
	CXTPSkinManagerSchema* pSchema = pObject && pObject->m_pSchema ? pObject->m_pSchema : m_pSchema;

	CMapStringToPtr& mapClasses = pSchema->m_mapClasses;

	strClassList.MakeUpper();

	CXTPSkinManagerClass* pClass;

	if (mapClasses.Lookup(strClassList, (void *&)pClass))
		return pClass;

	int nClassCode = pSchema->GetClassCode(strClassList);
	if (nClassCode == 0)
		return NULL; // Not supported;

	pClass = new CXTPSkinManagerClass(pSchema);
	pClass->m_strClass = strClassList;
	pClass->m_nClassCode =  nClassCode;

	mapClasses.SetAt(strClassList, pClass);

	return pClass;
}

void CXTPSkinManager::GetCurrentThemeName(CString& strThemeFileName, CString& strColorBuff)
{
	if (m_pResourceFile)
	{
		strThemeFileName = m_pResourceFile->GetResourcePath();
		strColorBuff = m_pResourceFile->GetIniFileName();
	}
}


int CXTPSkinManager::GetThemeSysSize(int iSizeId)
{
	int iVal = 0;
	if (!SUCCEEDED(m_pSchema->GetIntProperty(m_pSchema->GetClassCode(_T("SYSMETRICS")), 0, 0, iSizeId, iVal)))
		return 0;
	return iVal;
}

BOOL CXTPSkinManager::GetThemeSysBool(int iBoolId)
{
	BOOL bVal = FALSE;
	if (!SUCCEEDED(m_pSchema->GetBoolProperty(m_pSchema->GetClassCode(_T("SYSMETRICS")), 0, 0, iBoolId, bVal)))
		return FALSE;

	return bVal;
}


COLORREF CXTPSkinManager::GetThemeSysColor(int iColorId)
{
	if (!m_pSchema)
		return GetSysColor(iColorId - TMT_FIRSTCOLOR);

	return iColorId >= TMT_FIRSTCOLOR && iColorId <= TMT_LASTCOLOR ?
		m_pSchema->GetMetrics()->m_clrTheme[iColorId - TMT_FIRSTCOLOR] : (COLORREF)-1;
}

HRESULT CXTPSkinManager::GetThemeSysFont(int iFontId, LOGFONT *plf)
{
	HRESULT hr = m_pSchema->GetFontProperty(m_pSchema->GetClassCode(_T("SYSMETRICS")), 0, 0, iFontId, *plf);
	return hr;

}

HRESULT CXTPSkinManager::EnableThemeDialogTexture(HWND hWnd, DWORD dwFlags)
{
	CXTPSkinObjectFrame* pObject = (CXTPSkinObjectFrame*)Lookup(hWnd);

	if (!pObject)
		return E_INVALIDARG;

	pObject->m_dwDialogTexture = dwFlags;

	if (pObject->m_dwDialogTexture == ETDT_ENABLETAB)
	{
		pObject->SetWindowProc();
	}


	return S_OK;
}

void CXTPSkinManager::SetWindowTheme(HWND hWnd, CXTPSkinManagerSchema* pSchema)
{
	CXTPSkinObject* pSkinObject = Lookup(hWnd);
	if (pSkinObject)
	{
		CMDTARGET_RELEASE(pSkinObject->m_pSchema);
		pSkinObject->m_pSchema = pSchema;
		CMDTARGET_ADDREF(pSchema);

		pSkinObject->OnSkinChanged(m_bEnabled, pSchema ? TRUE : m_bEnabled);
	}

	hWnd = ::GetWindow(hWnd, GW_CHILD);
	while (hWnd)
	{
		SetWindowTheme(hWnd, pSchema);

		hWnd = ::GetWindow(hWnd, GW_HWNDNEXT);
	}
}

//////////////////////////////////////////////////////////////////////////

CXTPSkinManagerClass::CXTPSkinManagerClass(CXTPSkinManagerSchema* pSchema)
{
	m_pImages = new CXTPSkinImages();
	m_pSchema = pSchema;

	m_mapCachedProperties.InitHashTable(199);
}

CXTPSkinManagerClass::~CXTPSkinManagerClass()
{
	delete m_pImages;
}



BOOL CXTPSkinManagerClass::DrawThemeText(CDC* pDC, int iPartId, int iStateId, const CString& strText, DWORD dwFormat, const RECT *pRect)
{
	pDC->SetBkMode(TRANSPARENT);


	CRect rcCaptionMargins = GetThemeRect(iPartId, iStateId, TMT_CAPTIONMARGINS);
	rcCaptionMargins.top = rcCaptionMargins.bottom = 0;

	CRect rc(pRect);
	rc.DeflateRect(rcCaptionMargins);

	if (GetThemeEnumValue(iPartId, iStateId, TMT_CONTENTALIGNMENT) == CA_CENTER)
	{
		dwFormat |=  DT_CENTER;

	}

	pDC->SetTextColor(GetThemeColor(iPartId, iStateId, TMT_TEXTCOLOR, pDC->GetTextColor()));


	if (GetThemeEnumValue(iPartId, iStateId, TMT_TEXTSHADOWTYPE) == TST_SINGLE)
	{
		CSize sz = GetThemeSize(iPartId, iStateId, TMT_TEXTSHADOWOFFSET);

		if (sz != CSize(0, 0))
		{

			COLORREF clrShadow = GetThemeColor(iPartId, iStateId, TMT_TEXTSHADOWCOLOR);
			CRect rcShadow(rc);
			rcShadow.OffsetRect(sz);

			COLORREF clr = pDC->SetTextColor(clrShadow);
			pDC->DrawText(strText, rcShadow, dwFormat);
			pDC->SetTextColor(clr);
		}
	}

	pDC->DrawText(strText, rc, dwFormat);



	return TRUE;
}

CXTPSkinManagerSchemaProperty* CXTPSkinManagerClass::GetProperty(XTPSkinManagerProperty propType, int iPartId, int iStateId, int iPropId)
{
	UINT nCachedProp = iPropId + ((iPartId + (iStateId << 6 )) << 14);
	CXTPSkinManagerSchemaProperty* pProperty = NULL;

	if (m_mapCachedProperties.Lookup(nCachedProp, pProperty))
		return pProperty;

	pProperty = m_pSchema->GetProperty(m_nClassCode, iPartId, iStateId, iPropId);
	if (!pProperty)
	{
		m_mapCachedProperties.SetAt(nCachedProp, NULL);
		return 0;
	}

	if (pProperty->propType != propType)
		return 0;

	m_mapCachedProperties.SetAt(nCachedProp , pProperty);

	return pProperty;
}

CString CXTPSkinManagerClass::GetThemeString(int iPartId, int iStateId, int iPropId, LPCTSTR lpszDefault)
{
	CXTPSkinManagerSchemaProperty* pProperty = GetProperty(XTP_SKINPROPERTY_STRING, iPartId, iStateId, iPropId);

	if (!pProperty)
		return lpszDefault;

	return pProperty->lpszVal;
}

CRect CXTPSkinManagerClass::GetThemeRect(int iPartId, int iStateId, int iPropId, CRect rcDefault)
{
	CXTPSkinManagerSchemaProperty* pProperty = GetProperty(XTP_SKINPROPERTY_RECT, iPartId, iStateId, iPropId);

	if (!pProperty)
		return rcDefault;

	return pProperty->rcVal;
}

int CXTPSkinManagerClass::GetThemeInt(int iPartId, int iStateId, int iPropId, int nDefault)
{
	CXTPSkinManagerSchemaProperty* pProperty = GetProperty(XTP_SKINPROPERTY_INT, iPartId, iStateId, iPropId);

	if (!pProperty)
		return nDefault;

	return pProperty->iVal;
}
BOOL CXTPSkinManagerClass::GetThemeBool(int iPartId, int iStateId, int iPropId, BOOL bDefault)
{
	CXTPSkinManagerSchemaProperty* pProperty = GetProperty(XTP_SKINPROPERTY_BOOL, iPartId, iStateId, iPropId);

	if (!pProperty)
		return bDefault;

	return pProperty->bVal;
}
COLORREF CXTPSkinManagerClass::GetThemeColor(int iPartId, int iStateId, int iPropId, COLORREF clrDefault)
{
	CXTPSkinManagerSchemaProperty* pProperty = GetProperty(XTP_SKINPROPERTY_COLOR, iPartId, iStateId, iPropId);

	if (!pProperty)
		return clrDefault;

	COLORREF clrVal = pProperty->clrVal;
	m_pSchema->GetSkinManager()->ApplyColorFilter(clrVal);

	return clrVal;
}

int CXTPSkinManagerClass::GetThemeEnumValue(int iPartId, int iStateId, int iPropId, int nDefault)
{
	CXTPSkinManagerSchemaProperty* pProperty = GetProperty(XTP_SKINPROPERTY_ENUM, iPartId, iStateId, iPropId);

	if (!pProperty)
		return nDefault;

	return pProperty->iVal;
}

CSize CXTPSkinManagerClass::GetThemeSize(int iPartId, int iStateId, int iPropId, CSize szDefault)
{
	CXTPSkinManagerSchemaProperty* pProperty = GetProperty(XTP_SKINPROPERTY_POSITION, iPartId, iStateId, iPropId);

	if (!pProperty)
		return szDefault;

	return pProperty->szVal;
}


BOOL CXTPSkinManagerClass::DrawThemeBackground(CDC* pDC, int iPartId, int iStateId, const RECT *pRect)
{
	return m_pSchema->DrawThemeBackground(pDC, this, iPartId, iStateId, pRect);
}

BOOL CXTPSkinManagerClass::GetThemePartSize(int iPartId, int iStateId, RECT *pRect, int eSize, SIZE* pSize)
{
	if (GetThemeEnumValue(iPartId, iStateId, TMT_BGTYPE) != BT_IMAGEFILE)
		return FALSE;

	int nImageFile = (GetThemeEnumValue(iPartId, iStateId, TMT_IMAGESELECTTYPE) != IST_NONE) &&
		(GetThemeEnumValue(iPartId, iStateId, TMT_GLYPHTYPE, GT_NONE) == GT_NONE) ?
		TMT_IMAGEFILE1: TMT_IMAGEFILE;

	CString strImageFile = GetThemeString(iPartId, iStateId, nImageFile);
	if (strImageFile.IsEmpty())
	{
		if (nImageFile != TMT_IMAGEFILE1)
			return FALSE;

		strImageFile = GetThemeString(iPartId, iStateId, TMT_IMAGEFILE);
		if (strImageFile.IsEmpty())
			return FALSE;

	}


	CXTPSkinImage* pImage = GetImages()->LoadFile(m_pSchema->GetResourceFile(), strImageFile);
	if (!pImage)
	{
		return FALSE;
	}

	int nImageCount = GetThemeInt(iPartId, iStateId, TMT_IMAGECOUNT, 1);
	if (nImageCount < 1)
		nImageCount = 1;

	BOOL bHorizontalImageLayout = GetThemeEnumValue(iPartId, iStateId, TMT_IMAGELAYOUT, IL_HORIZONTAL) == IL_HORIZONTAL;

	CSize sz(pImage->GetWidth(), pImage->GetHeight());
	if (bHorizontalImageLayout) sz.cx /= nImageCount; else sz.cy /= nImageCount;

	if (eSize == TS_TRUE)
	{
		*pSize = sz;
	}
	if (eSize == TS_DRAW)
	{
		if (GetThemeEnumValue(iPartId, iStateId, TMT_SIZINGTYPE, ST_STRETCH) == ST_TRUESIZE)
		{
			*pSize = sz;
			return TRUE;
		}

		if (!pRect)
		{
			pSize->cy = 0;
			pSize->cx = 0;
		}
		else
		{
			pSize->cy = pRect->bottom - pRect->top;
			pSize->cx = pRect->right - pRect->left;
		}

		if (GetThemeBool(iPartId, iStateId, TMT_UNIFORMSIZING, FALSE))
		{
			pSize->cx = MulDiv(pSize->cy, sz.cx, sz.cy);
		}
	}

	return TRUE;
}
