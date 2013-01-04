// XTPTaskbarManager.cpp : implementation of the CXTPTaskbarManager class.
//
// This file is a part of the XTREME CONTROLS MFC class library.
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

#include "Common/XTPResourceManager.h"
#include "Common/XTPImageManager.h"

#include "XTPTaskbarManager.h"



#ifndef __ITaskbarList_INTERFACE_DEFINED__
#define __ITaskbarList_INTERFACE_DEFINED__


EXTERN_C const IID IID_ITaskbarList;

	MIDL_INTERFACE("56FDF342-FD6D-11d0-958A-006097C9A090")
	ITaskbarList : public IUnknown
	{
	public:
		virtual HRESULT STDMETHODCALLTYPE HrInit( void) = 0;

		virtual HRESULT STDMETHODCALLTYPE AddTab(
			/* [in] */  HWND hwnd) = 0;

		virtual HRESULT STDMETHODCALLTYPE DeleteTab(
			/* [in] */  HWND hwnd) = 0;

		virtual HRESULT STDMETHODCALLTYPE ActivateTab(
			/* [in] */  HWND hwnd) = 0;

		virtual HRESULT STDMETHODCALLTYPE SetActiveAlt(
			/* [in] */  HWND hwnd) = 0;

	};

#endif  /* __ITaskbarList_INTERFACE_DEFINED__ */


#ifndef __ITaskbarList2_INTERFACE_DEFINED__
#define __ITaskbarList2_INTERFACE_DEFINED__

/* interface ITaskbarList2 */
/* [object][uuid] */


EXTERN_C const IID IID_ITaskbarList2;


	MIDL_INTERFACE("602D4995-B13A-429b-A66E-1935E44F4317")
	ITaskbarList2 : public ITaskbarList
	{
	public:
		virtual HRESULT STDMETHODCALLTYPE MarkFullscreenWindow(
			/* [in] */  HWND hwnd,
			/* [in] */ BOOL fFullscreen) = 0;

	};

#endif  /* __ITaskbarList2_INTERFACE_DEFINED__ */



#ifndef __ITaskbarList3_INTERFACE_DEFINED__
#define __ITaskbarList3_INTERFACE_DEFINED__


typedef /* [v1_enum] */
enum THUMBBUTTONFLAGS
	{   THBF_ENABLED    = 0,
	THBF_DISABLED   = 0x1,
	THBF_DISMISSONCLICK = 0x2,
	THBF_NOBACKGROUND   = 0x4,
	THBF_HIDDEN = 0x8,
	THBF_NONINTERACTIVE = 0x10
	}   THUMBBUTTONFLAGS;

//DEFINE_ENUM_FLAG_OPERATORS(THUMBBUTTONFLAGS)
typedef /* [v1_enum] */
enum THUMBBUTTONMASK
	{   THB_BITMAP  = 0x1,
	THB_ICON    = 0x2,
	THB_TOOLTIP = 0x4,
	THB_FLAGS   = 0x8
	}   THUMBBUTTONMASK;

//DEFINE_ENUM_FLAG_OPERATORS(THUMBBUTTONMASK)
#include <pshpack8.h>
typedef struct THUMBBUTTON
	{
	THUMBBUTTONMASK dwMask;
	UINT iId;
	UINT iBitmap;
	HICON hIcon;
	WCHAR szTip[ 260 ];
	THUMBBUTTONFLAGS dwFlags;
	}   THUMBBUTTON;

typedef struct THUMBBUTTON *LPTHUMBBUTTON;

#include <poppack.h>
#define THBN_CLICKED        0x1800



/* interface ITaskbarList3 */
/* [object][uuid] */

typedef /* [v1_enum] */
enum TBPFLAG
	{   TBPF_NOPROGRESS = 0,
	TBPF_INDETERMINATE  = 0x1,
	TBPF_NORMAL = 0x2,
	TBPF_ERROR  = 0x4,
	TBPF_PAUSED = 0x8
	}   TBPFLAG;


EXTERN_C const IID IID_IXTPTaskbarList3;


	MIDL_INTERFACE("ea1afb91-9e28-4b86-90e9-9e9f8a5eefaf")
	ITaskbarList3 : public ITaskbarList2
	{
	public:
		virtual HRESULT STDMETHODCALLTYPE SetProgressValue(
			/* [in] */  HWND hwnd,
			/* [in] */ ULONGLONG ullCompleted,
			/* [in] */ ULONGLONG ullTotal) = 0;

		virtual HRESULT STDMETHODCALLTYPE SetProgressState(
			/* [in] */  HWND hwnd,
			/* [in] */ TBPFLAG tbpFlags) = 0;

		virtual HRESULT STDMETHODCALLTYPE RegisterTab(
			/* [in] */  HWND hwndTab,
			/* [in] */  HWND hwndMDI) = 0;

		virtual HRESULT STDMETHODCALLTYPE UnregisterTab(
			/* [in] */  HWND hwndTab) = 0;

		virtual HRESULT STDMETHODCALLTYPE SetTabOrder(
			/* [in] */  HWND hwndTab,
			/* [in] */  HWND hwndInsertBefore) = 0;

		virtual HRESULT STDMETHODCALLTYPE SetTabActive(
			/* [in] */  HWND hwndTab,
			/* [in] */  HWND hwndMDI,
			/* [in] */ DWORD dwReserved) = 0;

		virtual HRESULT STDMETHODCALLTYPE ThumbBarAddButtons(
			/* [in] */  HWND hwnd,
			/* [in] */ UINT cButtons,
			/* [size_is][in] */ LPTHUMBBUTTON pButton) = 0;

		virtual HRESULT STDMETHODCALLTYPE ThumbBarUpdateButtons(
			/* [in] */  HWND hwnd,
			/* [in] */ UINT cButtons,
			/* [size_is][in] */ LPTHUMBBUTTON pButton) = 0;

		virtual HRESULT STDMETHODCALLTYPE ThumbBarSetImageList(
			/* [in] */  HWND hwnd,
			/* [in] */ HIMAGELIST himl) = 0;

		virtual HRESULT STDMETHODCALLTYPE SetOverlayIcon(
			/* [in] */  HWND hwnd,
			/* [in] */  HICON hIcon,
			/* [string][unique][in] */ LPCWSTR pszDescription) = 0;

		virtual HRESULT STDMETHODCALLTYPE SetThumbnailTooltip(
			/* [in] */  HWND hwnd,
			/* [string][unique][in] */ LPCWSTR pszTip) = 0;

		virtual HRESULT STDMETHODCALLTYPE SetThumbnailClip(
			/* [in] */  HWND hwnd,
			/* [in] */  RECT *prcClip) = 0;

	};


#endif  /* __ITaskbarList3_INTERFACE_DEFINED__ */


#ifndef __IShellItem_INTERFACE_DEFINED__
#define __IShellItem_INTERFACE_DEFINED__

/* interface IShellItem */
/* [unique][object][uuid] */

typedef /* [v1_enum] */
enum _SIGDN
	{   SIGDN_NORMALDISPLAY = 0,
	SIGDN_PARENTRELATIVEPARSING = ( int  )0x80018001,
	SIGDN_DESKTOPABSOLUTEPARSING    = ( int  )0x80028000,
	SIGDN_PARENTRELATIVEEDITING = ( int  )0x80031001,
	SIGDN_DESKTOPABSOLUTEEDITING    = ( int  )0x8004c000,
	SIGDN_FILESYSPATH   = ( int  )0x80058000,
	SIGDN_URL   = ( int  )0x80068000,
	SIGDN_PARENTRELATIVEFORADDRESSBAR   = ( int  )0x8007c001,
	SIGDN_PARENTRELATIVE    = ( int  )0x80080001
	}   SIGDN;

/* [v1_enum] */
enum _SICHINTF
	{   SICHINT_DISPLAY = 0,
	SICHINT_ALLFIELDS   = ( int  )0x80000000,
	SICHINT_CANONICAL   = 0x10000000,
	SICHINT_TEST_FILESYSPATH_IF_NOT_EQUAL   = 0x20000000
	} ;
typedef DWORD SICHINTF;

typedef ULONG SFGAOF;

EXTERN_C const IID IID_IXTPShellItem;


	MIDL_INTERFACE("43826d1e-e718-42ee-bc55-a1e261c37bfe")
	IShellItem : public IUnknown
	{
	public:
		virtual HRESULT STDMETHODCALLTYPE BindToHandler(
			/* [unique][in] */ IBindCtx *pbc,
			/* [in] */ REFGUID bhid,
			/* [in] */ REFIID riid,
			/* [iid_is][out] */ void **ppv) = 0;

		virtual HRESULT STDMETHODCALLTYPE GetParent(
			/* [out] */  IShellItem **ppsi) = 0;

		virtual HRESULT STDMETHODCALLTYPE GetDisplayName(
			/* [in] */ SIGDN sigdnName,
			/* [string][out] */  LPWSTR *ppszName) = 0;

		virtual HRESULT STDMETHODCALLTYPE GetAttributes(
			/* [in] */ SFGAOF sfgaoMask,
			/* [out] */  SFGAOF *psfgaoAttribs) = 0;

		virtual HRESULT STDMETHODCALLTYPE Compare(
			/* [in] */  IShellItem *psi,
			/* [in] */ SICHINTF hint,
			/* [out] */  int *piOrder) = 0;

	};


#endif  /* __IShellItem_INTERFACE_DEFINED__ */

//nclude "ObjectArray.h"

#ifndef __IObjectArray_INTERFACE_DEFINED__
#define __IObjectArray_INTERFACE_DEFINED__

	MIDL_INTERFACE("92CA9DCD-5622-4bba-A805-5E9F541BD8C9")
	IObjectArray : public IUnknown
	{
	public:
		virtual HRESULT STDMETHODCALLTYPE GetCount(
			/* [out] */  UINT *pcObjects) = 0;

			virtual HRESULT STDMETHODCALLTYPE GetAt(
			/* [in] */ UINT uiIndex,
			/* [in] */  REFIID riid,
			/* [iid_is][out] */  void **ppv) = 0;

	};
#endif  /* __IObjectArray_INTERFACE_DEFINED__ */


#ifndef __IObjectCollection_INTERFACE_DEFINED__
#define __IObjectCollection_INTERFACE_DEFINED__

	/* interface IObjectCollection */
	/* [unique][object][uuid] */

	MIDL_INTERFACE("5632b1a4-e38a-400a-928a-d4cd63230295")
	IObjectCollection : public IObjectArray
	{
	public:
		virtual HRESULT STDMETHODCALLTYPE AddObject(
			/* [in] */  IUnknown *punk) = 0;

		virtual HRESULT STDMETHODCALLTYPE AddFromArray(
			/* [in] */  IObjectArray *poaSource) = 0;

		virtual HRESULT STDMETHODCALLTYPE RemoveObjectAt(
			/* [in] */ UINT uiIndex) = 0;

		virtual HRESULT STDMETHODCALLTYPE Clear( void) = 0;

	};

#endif  /* __IObjectCollection_INTERFACE_DEFINED__ */


#ifndef __ICustomDestinationList_INTERFACE_DEFINED__
#define __ICustomDestinationList_INTERFACE_DEFINED__

/* interface ICustomDestinationList */
/* [unique][object][uuid] */

typedef /* [v1_enum] */
enum KNOWNDESTCATEGORY
	{   KDC_FREQUENT    = 1,
	KDC_RECENT  = ( KDC_FREQUENT + 1 )
	}   KNOWNDESTCATEGORY;


EXTERN_C const IID IID_IXTPCustomDestinationList;


	MIDL_INTERFACE("6332debf-87b5-4670-90c0-5e57b408a49e")
	ICustomDestinationList : public IUnknown
	{
	public:
		virtual HRESULT STDMETHODCALLTYPE SetAppID(
			/* [string][in] */ LPCWSTR pszAppID) = 0;

		virtual HRESULT STDMETHODCALLTYPE BeginList(
			/* [out] */  UINT *pcMinSlots,
			/* [in] */  REFIID riid,
			/* [iid_is][out] */  void **ppv) = 0;

		virtual HRESULT STDMETHODCALLTYPE AppendCategory(
			/* [string][in] */  LPCWSTR pszCategory,
			/* [in] */  IObjectArray *poa) = 0;

		virtual HRESULT STDMETHODCALLTYPE AppendKnownCategory(
			/* [in] */ KNOWNDESTCATEGORY category) = 0;

		virtual HRESULT STDMETHODCALLTYPE AddUserTasks(
			/* [in] */  IObjectArray *poa) = 0;

		virtual HRESULT STDMETHODCALLTYPE CommitList( void) = 0;

		virtual HRESULT STDMETHODCALLTYPE GetRemovedDestinations(
			/* [in] */  REFIID riid,
			/* [iid_is][out] */  void **ppv) = 0;

		virtual HRESULT STDMETHODCALLTYPE DeleteList(
			/* [string][unique][in] */  LPCWSTR pszAppID) = 0;

		virtual HRESULT STDMETHODCALLTYPE AbortList( void) = 0;

	};

#endif  /* __ICustomDestinationList_INTERFACE_DEFINED__ */

#ifndef PROPERTYKEY_DEFINED
#define PROPERTYKEY_DEFINED
	typedef struct _tagpropertykey
	{
		GUID fmtid;
		DWORD pid;
	}   PROPERTYKEY;

#endif


#ifndef _REFPROPVARIANT_DEFINED
#define REFPROPVARIANT const PROPVARIANT &
#endif

#define INITGUID
#include "propkey.h"

#ifndef __IPropertyStore_INTERFACE_DEFINED__
#define __IPropertyStore_INTERFACE_DEFINED__

	/* interface IPropertyStore */
	/* [unique][object][helpstring][uuid] */



	EXTERN_C const IID IID_IXTPPropertyStore;


	MIDL_INTERFACE("886d8eeb-8cf2-4446-8d02-cdba1dbdcf99")
	IPropertyStore : public IUnknown
	{
	public:
		virtual HRESULT STDMETHODCALLTYPE GetCount(
			/* [out] */ DWORD *cProps) = 0;

			virtual HRESULT STDMETHODCALLTYPE GetAt(
			/* [in] */ DWORD iProp,
			/* [out] */ PROPERTYKEY *pkey) = 0;

			virtual HRESULT STDMETHODCALLTYPE GetValue(
			/* [in] */ REFPROPERTYKEY key,
			/* [out] */ PROPVARIANT *pv) = 0;

			virtual HRESULT STDMETHODCALLTYPE SetValue(
			/* [in] */ REFPROPERTYKEY key,
			/* [in] */ REFPROPVARIANT propvar) = 0;

			virtual HRESULT STDMETHODCALLTYPE Commit( void) = 0;

	};

#endif  /* __IPropertyStore_INTERFACE_DEFINED__ */

const IID BASED_CODE IID_IXTPTaskbarList3 =
	{ 0xea1afb91, 0x9e28, 0x4b86, { 0x90, 0xe9, 0x9e, 0x9f, 0x8a, 0x5e, 0xef, 0xaf } };


const CLSID BASED_CODE CLSID_XTPDestinationList =
	{ 0x77f10cf0, 0x3db5, 0x4966, { 0xb5, 0x20, 0xb7, 0xc5, 0x4f, 0xd3, 0x5e, 0xd6 } };

const CLSID BASED_CODE CLSID_XTPEnumerableObjectCollection =
	{ 0x2d3468c1, 0x36a7, 0x43b6, { 0xac, 0x24, 0xd3, 0xf0, 0x2f, 0xd9, 0x60, 0x7a } };

const IID BASED_CODE IID_IXTPObjectArray =
	{ 0x92CA9DCD, 0x5622, 0x4bba, { 0xa8, 0x05, 0x5E, 0x9F, 0x54, 0x1B, 0xD8, 0xC9 } };

const IID BASED_CODE IID_IXTPCustomDestinationList =
	{ 0x6332debf, 0x87b5, 0x4670, { 0x90, 0xc0, 0x5e, 0x57, 0xb4, 0x08, 0xa4, 0x9e } };

const IID BASED_CODE IID_IXTPObjectCollection =
	{ 0x5632b1a4, 0xe38a, 0x400a, { 0x92, 0x8a, 0xd4, 0xcd, 0x63, 0x23, 0x02, 0x95 } };

const IID BASED_CODE IID_IXTPShellItem =
	{ 0x43826d1e, 0xe718, 0x42ee, { 0xbc, 0x55, 0xa1, 0xe2, 0x61, 0xc3, 0x7b, 0xfe } };

const IID BASED_CODE IID_IXTPPropertyStore =
	{ 0x886d8eeb, 0x8cf2, 0x4446, { 0x8d, 0x02, 0xcd, 0xba, 0x1d, 0xbd, 0xcf, 0x99 } };


//////////////////////////////////////////////////////////////////////////
// CXTPTaskbarManagerJumpList

CXTPTaskbarManagerJumpList::CXTPTaskbarManagerJumpList()
{
	m_pcdl = NULL;
	m_poaRemoved = NULL;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPTaskbarManagerJumpList::~CXTPTaskbarManagerJumpList()
{

}

void CXTPTaskbarManagerJumpList::BuildList()
{
	HRESULT hr = CoCreateInstance(CLSID_XTPDestinationList, NULL, CLSCTX_INPROC_SERVER, IID_IXTPCustomDestinationList, (void**)&m_pcdl);
	if (SUCCEEDED(hr))
	{
		UINT cMinSlots;
		hr = m_pcdl->BeginList(&cMinSlots, IID_IXTPObjectArray, (void**)(&m_poaRemoved));
		if (SUCCEEDED(hr))
		{
			hr = AddUserTasks();

			if (SUCCEEDED(hr))
			{
				hr = m_pcdl->CommitList();
			}

			m_poaRemoved->Release();
			m_poaRemoved = NULL;
		}
		m_pcdl->Release();
		m_poaRemoved = NULL;
	}
}

void CXTPTaskbarManagerJumpList::DeleteList()
{
	HRESULT hr = CoCreateInstance(CLSID_XTPDestinationList, NULL, CLSCTX_INPROC_SERVER, IID_IXTPCustomDestinationList, (void**)(&m_pcdl));
	if (SUCCEEDED(hr))
	{
		hr = m_pcdl->DeleteList(NULL);
		m_pcdl->Release();
	}
}

void CXTPTaskbarManagerJumpList::Clear()
{
	m_arrTask.RemoveAll();
}

void CXTPTaskbarManagerJumpList::AddTask(LPCTSTR lpszPath, LPCTSTR lpszArgument, LPCTSTR lpszTitle, LPCTSTR lpszIconPath, int nIconIndex /*= -1*/)
{
	CTask task;
	task.m_strPath = lpszPath;
	task.m_strArguments = lpszArgument;
	task.m_strTitle = lpszTitle;
	task.m_strIconPath = lpszIconPath;
	task.m_nIconIndex = nIconIndex;

	if (task.m_strPath.IsEmpty())
	{
		TCHAR szAppPath[MAX_PATH];
		::GetModuleFileName(NULL, szAppPath, MAX_PATH);

		task.m_strPath = szAppPath;
	}

	if (nIconIndex != -1 && task.m_strIconPath.IsEmpty())
	{
		task.m_strIconPath = task.m_strPath;
	}

	m_arrTask.Add(task);
}

HRESULT CXTPTaskbarManagerJumpList::AddUserTasks()
{
	IObjectCollection *poc;
	HRESULT hr = CoCreateInstance(CLSID_XTPEnumerableObjectCollection, NULL, CLSCTX_INPROC, IID_IXTPObjectCollection, (void**)(&poc));
	if (SUCCEEDED(hr))
	{

		for (int i = 0; i < m_arrTask.GetSize(); i++)
		{
			const CTask& task = m_arrTask[i];

			IShellLinkW *psl = NULL;
			hr = CreateShellLink(XTP_CT2CW(task.m_strPath), XTP_CT2CW(task.m_strArguments),
				XTP_CT2CW(task.m_strTitle), XTP_CT2CW(task.m_strIconPath), task.m_nIconIndex, &psl);

			if (SUCCEEDED(hr))
			{
				hr = poc->AddObject(psl);
				psl->Release();
			}
			else
			{
				break;
			}
		}


		if (SUCCEEDED(hr) && m_arrTask.GetSize())
		{
			IObjectArray* poa;
			hr = poc->QueryInterface(IID_IXTPObjectArray, (void**)(&poa));
			if (SUCCEEDED(hr))
			{
				hr = m_pcdl->AddUserTasks(poa);
				poa->Release();
			}
		}
		poc->Release();
	}
	return hr;
}

HRESULT CXTPTaskbarManagerJumpList::CreateShellLink(LPCWSTR lpszApp, LPCWSTR lpszCommandLine,
	LPCWSTR lpszTitle, LPCWSTR lpszIconPath, int nIconIndex, IShellLinkW** ppSL)
{
	IShellLinkW *psl;
	HRESULT hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLinkW, (void**)(&psl));
	if (SUCCEEDED(hr))
	{
		hr = psl->SetPath(lpszApp);
		if (SUCCEEDED(hr))
		{
			hr = psl->SetArguments(lpszCommandLine);
			if (SUCCEEDED(hr))
			{
				if (nIconIndex != -1)
				{
					psl->SetIconLocation(lpszIconPath, nIconIndex);
				}

				IPropertyStore *pps;
				hr = psl->QueryInterface(IID_IXTPPropertyStore, (void**)(&pps));
				if (SUCCEEDED(hr))
				{
					PROPVARIANT propvar;
					propvar.vt = VT_BSTR;
					propvar.pwszVal = SysAllocString(lpszTitle);

					hr = pps->SetValue(PKEY_Title, propvar);
					if (SUCCEEDED(hr))
					{
						hr = pps->Commit();
						if (SUCCEEDED(hr))
						{
							hr = psl->QueryInterface(IID_IShellLinkW, (void**)ppSL);
						}
					}
					PropVariantClear(&propvar);

					pps->Release();
				}
			}
		}
		else
		{
			hr = HRESULT_FROM_WIN32(GetLastError());
		}
		psl->Release();
	}
	return hr;
}


BOOL CXTPTaskbarManagerJumpList::IsItemInArray(IShellItem* pSI, IObjectArray* pArray)
{
	BOOL fRet = false;
	UINT cItems;
	if (SUCCEEDED(pArray->GetCount(&cItems)))
	{
		IShellItem *psiCompare;
		for (UINT i = 0; !fRet && i < cItems; i++)
		{
			if (SUCCEEDED(pArray->GetAt(i, IID_IXTPShellItem, (void**)(&psiCompare))))
			{
				int iOrder;
				fRet = SUCCEEDED(psiCompare->Compare(pSI, SICHINT_CANONICAL, &iOrder)) && (0 == iOrder);
				psiCompare->Release();
			}
		}
	}
	return fRet;
}

void CXTPTaskbarManagerJumpList::CommitList()
{
	DeleteList();
	BuildList();
}

//////////////////////////////////////////////////////////////////////////
// CXTPTaskbarManager

CXTPTaskbarManager::CXTPTaskbarManager()
{
	m_nProgressBarRangeMin = 0;
	m_nProgressBarRangeMax = 100;


	m_pTaskbarList3 = NULL;

	HRESULT hr = CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER, IID_IXTPTaskbarList3, (void**)(&m_pTaskbarList3));
	if (FAILED(hr) || m_pTaskbarList3 == NULL)
	{
		SAFE_RELEASE(m_pTaskbarList3);
	}


	m_pJumpList = new CXTPTaskbarManagerJumpList();
}


CXTPTaskbarManager::~CXTPTaskbarManager()
{
	SAFE_RELEASE(m_pTaskbarList3);

	CMDTARGET_RELEASE(m_pJumpList);
}


BOOL CXTPTaskbarManager::SetOverlayIcon(HWND hWndParent, HICON hIcon, LPCTSTR lpszDescription)
{
	if (!m_pTaskbarList3)
		return FALSE;

	if (FAILED(m_pTaskbarList3->SetOverlayIcon(hWndParent, hIcon, lpszDescription ? XTP_CT2CW(lpszDescription) : L"")))
		return FALSE;

	return TRUE;
}

BOOL CXTPTaskbarManager::SetOverlayIcon(HWND hWndParent, UINT nIDResource, LPCTSTR lpszDescription)
{

	HICON hIcon = (HICON) LoadImage(AfxGetResourceHandle(), MAKEINTRESOURCE(nIDResource),
		IMAGE_ICON, 16, 16, LR_LOADMAP3DCOLORS);


	if (hIcon == NULL)
	{
		TRACE1("Can't load image from the resource with ID %d.", nIDResource);
		return FALSE;
	}


	BOOL bResult = SetOverlayIcon(hWndParent, hIcon, lpszDescription);
	DestroyIcon(hIcon);

	return bResult;
}


void CXTPTaskbarManager::SetProgressBarRange(HWND /*hWndParent*/, int nRangeMin, int nRangeMax)
{
	m_nProgressBarRangeMax = nRangeMax;
	m_nProgressBarRangeMin = nRangeMin;
}

void CXTPTaskbarManager::SetProgressBarPosition(HWND hWndParent, int nProgressPos)
{
	if (!m_pTaskbarList3)
		return;

	m_pTaskbarList3->SetProgressValue(hWndParent, nProgressPos - m_nProgressBarRangeMin,
		m_nProgressBarRangeMax - m_nProgressBarRangeMin);
}

void CXTPTaskbarManager::SetProgressBarState(HWND hWndParent, int nFlags)
{
	if (!m_pTaskbarList3)
		return;

	m_pTaskbarList3->SetProgressState(hWndParent, (TBPFLAG)nFlags);
}


#ifdef _XTP_ACTIVEX


BEGIN_DISPATCH_MAP(CXTPTaskbarManagerJumpList, CCmdTarget)

	DISP_FUNCTION_ID(CXTPTaskbarManagerJumpList, "AddTask", 1, AddTask, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4)

	DISP_FUNCTION_ID(CXTPTaskbarManagerJumpList, "Clear", 2, Clear, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPTaskbarManagerJumpList, "CommitList", 3, CommitList, VT_EMPTY, VTS_NONE)

END_DISPATCH_MAP()

// {AE890313-E400-4DA2-AEF5-CF1DD59DCD3B}
static const IID IID_ITreeViewNode =
{ 0xae890313, 0xe400, 0x4da2, { 0xae, 0xf5, 0xcf, 0x1d, 0xd5, 0x9d, 0xcd, 0x3b } };

BEGIN_INTERFACE_MAP(CXTPTaskbarManagerJumpList, CCmdTarget)
	INTERFACE_PART(CXTPTaskbarManagerJumpList, IID_ITreeViewNode, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPTaskbarManagerJumpList, IID_ITreeViewNode)


#endif
