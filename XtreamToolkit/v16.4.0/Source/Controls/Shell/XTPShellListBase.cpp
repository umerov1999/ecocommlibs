// XTPShellListBase.cpp : implementation file
//
// This file is a part of the XTREME CONTROLS MFC class library.
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
#include "../Resource.h"

#include "Common/XTPVC80Helpers.h"  // Visual Studio 2005 helper functions
#include "Common/XTPResourceManager.h"
#include "Common/XTPWinThemeWrapper.h"
#include "Common/XTPSystemHelpers.h"
#include "../Util/XTPControlTheme.h"

#include "../Defines.h"
#include "../Header/XTPHeaderCtrl.h"
#include "../List/XTPListBase.h"
#include "XTPShellPidl.h"
#include "XTPShellSettings.h"
#include "XTPDirWatcher.h"
#include "XTPShellListBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ISZIPFILE(dw) (BOOL)(dw & SFGAO_FOLDER) && (dw & SFGAO_STREAM)
#define ISFOLDER(dw)  (BOOL)(dw & SFGAO_FOLDER)

/////////////////////////////////////////////////////////////////////////////
// CXTPShellListBase

CXTPShellListBase::CXTPShellListBase()
: m_pDirThread(0)
{
	m_bRowColor = TRUE;
	m_bContextMenu = TRUE;
	m_uFlags = SHCONTF_FOLDERS | SHCONTF_NONFOLDERS;
	m_ulSFGAOFlags = SFGAO_FOLDER | SFGAO_STREAM | SFGAO_FILESYSANCESTOR | SFGAO_DISPLAYATTRMASK | SFGAO_REMOVABLE | SFGAO_COMPRESSED | SFGAO_ENCRYPTED;

	if (m_shSettings.ShowAllFiles() && !m_shSettings.ShowSysFiles())
	{
		m_uFlags |= SHCONTF_INCLUDEHIDDEN;
	}

	if (!SUCCEEDED(::SHGetSpecialFolderLocation(NULL, CSIDL_INTERNET, &m_pidlINet)))
	{
		m_pidlINet = NULL;
	}

	m_nNameColumnWidth = 200;

	m_lpsfFolder = NULL;
}

CXTPShellListBase::~CXTPShellListBase()
{
	// End the directory monitoring thread.
	if (m_pDirThread)
	{
		m_pDirThread->StopNotifications();
		m_pDirThread = NULL;
	}

	if (m_pidlINet)
	{
		CShellMalloc lpMalloc;
		lpMalloc.Free(m_pidlINet);
	}

	SAFE_RELEASE(m_lpsfFolder);
}

/////////////////////////////////////////////////////////////////////////////
// CXTPShellListBase message handlers

BOOL CXTPShellListBase::PopulateListView(XTP_TVITEMDATA* lptvid, LPSHELLFOLDER lpsf)
{
	// Turn off redraw so the user does't see resorting
	m_pListCtrl->SetRedraw(false);

	//clear the view for new items
	m_pListCtrl->DeleteAllItems();

	SAFE_RELEASE(m_lpsfFolder);

	if (InitListViewItems(lptvid, lpsf))
	{
		SortList((m_nSortedCol > -1) ? m_nSortedCol : 0,
			(m_nSortedCol > -1) ? m_bAscending : 1);

		// create the directory monitoring thread.
		if (m_pDirThread == NULL)
		{
			m_pDirThread = (CXTPDirWatcher*)AfxBeginThread(RUNTIME_CLASS(CXTPDirWatcher),
				THREAD_PRIORITY_LOWEST, 0, CREATE_SUSPENDED, NULL);

			m_pDirThread->SetFolderData(m_pListCtrl, lptvid);
			m_pDirThread->ResumeThread();
		}

		// if the folder changed, update the folder data.
		else
		{
			TCHAR szFolderPath[_MAX_PATH];
			if (::SHGetPathFromIDList(lptvid->lpifq, szFolderPath))
			{
				CString strFolderPath = m_pDirThread->GetFolderPath();
				if (strFolderPath.CompareNoCase(szFolderPath) != 0)
				{
					m_pDirThread->SuspendThread();
					m_pDirThread->SetFolderData(m_pListCtrl, lptvid);
					m_pDirThread->ResumeThread();
				}
			}
		}

		m_lpsfFolder = lpsf;
		m_lpsfFolder->AddRef();

		m_pListCtrl->SetRedraw(true);
		return TRUE;
	}

	m_pListCtrl->SetRedraw(true);
	return FALSE;
}

void CXTPShellListBase::BuildDefaultColumns()
{
	CString strLabel;
	CXTPResourceManager::AssertValid(XTPResourceManager()->LoadString(&strLabel, XTP_IDS_NAME));
	m_pListCtrl->InsertColumn(0, strLabel, LVCFMT_LEFT, m_nNameColumnWidth, 0);

	CXTPResourceManager::AssertValid(XTPResourceManager()->LoadString(&strLabel, XTP_IDS_SIZE));
	m_pListCtrl->InsertColumn(1, strLabel, LVCFMT_RIGHT, 90, 1);

	CXTPResourceManager::AssertValid(XTPResourceManager()->LoadString(&strLabel, XTP_IDS_TYPE));
	m_pListCtrl->InsertColumn(2, strLabel, LVCFMT_LEFT, 120, 2);

	CXTPResourceManager::AssertValid(XTPResourceManager()->LoadString(&strLabel, XTP_IDS_MODIFIED));
	m_pListCtrl->InsertColumn(3, strLabel, LVCFMT_LEFT, 120, 3);
}

BOOL CXTPShellListBase::InitSystemImageLists()
{
	HIMAGELIST himlSmall = GetSystemImageList(SHGFI_SMALLICON);

	HIMAGELIST himlLarge = GetSystemImageList(SHGFI_LARGEICON);

	if (himlSmall && himlLarge)
	{
		ListView_SetImageList(m_pListCtrl->GetSafeHwnd(), himlSmall, LVSIL_SMALL);
		ListView_SetImageList(m_pListCtrl->GetSafeHwnd(), himlLarge, LVSIL_NORMAL);
		return TRUE;
	}

	return FALSE;
}

BOOL CXTPShellListBase::IsItemFiltered(LPCTSTR lpszItemName, ULONG ulItemAttrs)
{
	if (ISFOLDER(ulItemAttrs))
		return FALSE;

	if (!m_csIncludeEXT.IsEmpty())
	{
		TCHAR szDrive[_MAX_DRIVE];
		TCHAR szDir[_MAX_DIR];
		TCHAR szFileName[_MAX_FNAME];
		TCHAR szExt[_MAX_EXT];

		SPLITPATH_S(lpszItemName, szDrive, szDir, szFileName, szExt);

		if (_tcsclen(szExt) == 0)
			return m_csIncludeEXT.Find(_T("*.;")) == -1;

		return m_csIncludeEXT.Find(szExt) == -1;
	}

	return FALSE;
}


BOOL CXTPShellListBase::InitListViewItems(XTP_TVITEMDATA* lptvid, LPSHELLFOLDER lpsf)
{
	CShellMalloc lpMalloc;

	if (!lpMalloc)
		return FALSE;

	LPENUMIDLIST lpe = NULL;

	if (FAILED(lpsf->EnumObjects(::GetParent(m_pListCtrl->m_hWnd), m_uFlags, &lpe)))
		return FALSE;

	if (lpe == NULL)
		return FALSE;

	int          iCtr = 0;
	ULONG        ulFetched = 0;
	LPITEMIDLIST lpi = NULL;

	while (lpe->Next(1, &lpi, &ulFetched) == S_OK)
	{
		// Now get the friendly name that we'll put in the treeview...
		CString szFileName, szFullFilePath;
		GetName(lpsf, lpi, SHGDN_NORMAL, szFileName);
		GetName(lpsf, lpi, SHGDN_FORPARSING, szFullFilePath);

		// Note that since we are interested in the display attributes as well as
		// the other attributes, we need to set ulAttrs to SFGAO_DISPLAYATTRMASK
		// before calling GetAttributesOf();

		// Determine what type of object we have.
		ULONG ulAttrs = GetAttributes(lpsf, lpi, m_ulSFGAOFlags);

		BOOL bIncludeFile = !IsItemFiltered(szFullFilePath, ulAttrs);

		if (bIncludeFile)
		{
			// allocate memory for ITEMDATA struct
			XTP_LVITEMDATA* lplvid = new XTP_LVITEMDATA;
			if (lplvid == NULL)
			{
				if (lpe)
				{
					lpe->Release();
				}

				return FALSE;
			}

			lplvid->ulAttribs = ulAttrs;
			LPITEMIDLIST lpifqThisItem = ConcatPidls(lpMalloc, lptvid->lpifq, lpi);

			LV_ITEM lvi;
			lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
			lvi.iItem = iCtr++;
			lvi.iSubItem = 0;
			lvi.pszText = (LPTSTR)(LPCTSTR)szFileName;
			lvi.cchTextMax = 0;
			lvi.iImage = GetItemIcon(lpifqThisItem, SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON);

			lplvid->lpsfParent = lpsf;
			lpsf->AddRef();

			// Make a copy of the ITEMIDLIST
			lplvid->lpi = DuplicateItem(lpMalloc, lpi);
			lvi.lParam = (LPARAM)lplvid;

			// Add the item to the listview
			int iIndex = m_pListCtrl->InsertItem(&lvi);
			SetAttributes(iIndex, ulAttrs);
			if (iIndex >= 0)
			{
				TCHAR szItemPath[_MAX_PATH];
				::SHGetPathFromIDList(lpifqThisItem, szItemPath);

				WIN32_FIND_DATA fdata;
				HANDLE handle = ::FindFirstFile(szItemPath, &fdata);

				if (handle != INVALID_HANDLE_VALUE)
				{
					if (!ISFOLDER(ulAttrs) || ISZIPFILE(ulAttrs))
					{
						LONGLONG fsize = fdata.nFileSizeHigh*((LONGLONG)ULONG_MAX + 1) + fdata.nFileSizeLow;

						TCHAR szBuffer[16];
						CString strSize;
						strSize.Format(_T("%s KB"), InsertCommas((fsize + 1024)/1024, szBuffer, 15));

						m_pListCtrl->SetItemText(iIndex, 1, strSize);
					}

					// insert date modified.
					FILETIME ltime;
					::FileTimeToLocalFileTime(&fdata.ftLastWriteTime, &ltime);

					SYSTEMTIME time;
					::FileTimeToSystemTime(&ltime, &time);

					if ((time.wYear >= 1970 && time.wYear <= 2038) &&
						(time.wMonth >= 1 && time.wMonth <= 12))
					{
						CTime cTime;
						cTime = CTime(
							time.wYear,
							time.wMonth,
							time.wDay,
							time.wHour,
							time.wMinute,
							time.wSecond);

						m_pListCtrl->SetItemText(iIndex, 3, cTime.Format(_T("%m/%d/%y %I:%M %p")));
					}

					::FindClose(handle);
				}

				SHFILEINFO sfi;
				::SHGetFileInfo((TCHAR*)lpifqThisItem, 0, &sfi,
					sizeof(SHFILEINFO), SHGFI_PIDL | SHGFI_TYPENAME);

				m_pListCtrl->SetItemText(iIndex, 2, sfi.szTypeName);
			}
			else
			{
				if (lpifqThisItem)
				{
					lpMalloc.Free(lpifqThisItem);
				}

				return FALSE;
			}


			if (lpifqThisItem)
			{
				lpMalloc.Free(lpifqThisItem);
			}
		}

		// Free the pidl that the shell gave us.
		if (lpi)
		{
			lpMalloc.Free(lpi);
			lpi = NULL;
		}
	}

	if (lpe)
	{
		lpe->Release();
	}

	return TRUE;
}

TCHAR* CXTPShellListBase::InsertCommas(LONGLONG value, TCHAR* szBufferOut, UINT nSize)
{
	CString strValue; // 30 digits is a really big number
	TCHAR szBufferIn[30]; // 30 digits is a really big number
	TCHAR szDecimalSep[ 5 ];
	TCHAR szThousandSep[ 5 ];

	NUMBERFMT fmt;
	fmt.NumDigits = 0; // No decimal places
	::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_ILZERO, szBufferIn, 2);
	fmt.LeadingZero = _ttoi(szBufferIn);
	fmt.Grouping = 3;
	::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDECIMAL, szDecimalSep, 4);
	fmt.lpDecimalSep = szDecimalSep;
	::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_STHOUSAND, szThousandSep, 4);
	fmt.lpThousandSep = szThousandSep;
	::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_INEGNUMBER, szBufferIn, 2);
	fmt.NegativeOrder = _ttoi(szBufferIn);
	strValue.Format(_T("%I64d"), value);
	::GetNumberFormat(LOCALE_USER_DEFAULT, 0, strValue, &fmt, szBufferOut, nSize);

	return szBufferOut;
}

void CXTPShellListBase::GetNormalAndSelectedIcons(LPITEMIDLIST lpifq, LPTV_ITEM lptvitem)
{
	// Note that we don't check the return value here because if GetIcon()
	// fails, then we're in big trouble...
	lptvitem->iImage = GetItemIcon(lpifq,
		SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON);

	lptvitem->iSelectedImage = GetItemIcon(lpifq,
		SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON | SHGFI_OPENICON);
}

int CXTPShellListBase::GetDoubleClickedItem()
{
	CPoint point;
	::GetCursorPos(&point);
	m_pListCtrl->ScreenToClient(&point);

	LV_HITTESTINFO  lvhti;
	lvhti.pt = point;
	m_pListCtrl->HitTest(&lvhti);

	if ((lvhti.flags & LVHT_ONITEM) == 0)
	{
		return -1;
	}

	return lvhti.iItem;
}

void CXTPShellListBase::ShowShellContextMenu(CPoint point)
{
	if (m_bContextMenu == FALSE)
		return;

	CPoint ptClient(point);
	m_pListCtrl->ScreenToClient(&ptClient);

	int nIndex = point == CPoint(-1, -1) ? m_pListCtrl->GetNextItem(-1, LVNI_FOCUSED) : m_pListCtrl->HitTest(ptClient);

	if (nIndex != -1)
	{
		if (point == CPoint(-1, -1))
		{
			CRect rcItem;
			m_pListCtrl->GetItemRect(nIndex, &rcItem, LVIR_ICON);
			point = rcItem.CenterPoint();
			m_pListCtrl->ClientToScreen(&point);
		}

		XTP_LVITEMDATA* lplvid = (XTP_LVITEMDATA*)m_pListCtrl->GetItemData(nIndex);
		ASSERT(lplvid != NULL);
		if (!lplvid)
			return;

		CPtrArray arrItems;
		arrItems.Add(lplvid->lpi);

		POSITION pos = m_pListCtrl->GetFirstSelectedItemPosition();

		while (NULL != pos)
		{
			int nSelItem = m_pListCtrl->GetNextSelectedItem(pos);

			XTP_LVITEMDATA* lplvidSelected = (XTP_LVITEMDATA*)m_pListCtrl->GetItemData(nSelItem);
			ASSERT(lplvidSelected != NULL);

			if (lplvidSelected != lplvid && lplvidSelected)
				arrItems.Add(lplvidSelected->lpi);
		}

		CXTPShellPidl::ShowContextMenu(m_pListCtrl->m_hWnd,
			lplvid->lpsfParent, (LPCITEMIDLIST*)arrItems.GetData(), (int)arrItems.GetSize(), &point);

	}
	else if (m_lpsfFolder)
	{
		ShowContextMenu(m_pListCtrl->m_hWnd, m_lpsfFolder, 0, 0, &point);
	}
}

bool CXTPShellListBase::ShellOpenItem(int iItem)
{
	// Long pointer to ListView item data
	XTP_LVITEMDATA*  lplvid = (XTP_LVITEMDATA*)m_pListCtrl->GetItemData(iItem);
	return ShellOpenItem(lplvid);
}

bool CXTPShellListBase::ShellOpenItem(XTP_LVITEMDATA* lplvid)
{
	// Long pointer to ListView item data
	if (!ISFOLDER(lplvid->ulAttribs))
	{
		if (lplvid->ulAttribs & (SFGAO_FILESYSANCESTOR|SFGAO_REMOVABLE))
			return false;

		SHELLEXECUTEINFO sei;
		::ZeroMemory(&sei, sizeof(SHELLEXECUTEINFO));

		sei.cbSize = sizeof(SHELLEXECUTEINFO);
		sei.fMask = SEE_MASK_INVOKEIDLIST;
		sei.hwnd = ::GetParent(m_pListCtrl->GetSafeHwnd());
		sei.nShow = SW_SHOWNORMAL;
		sei.hInstApp = XTPGetInstanceHandle();
		sei.lpIDList = GetFullyQualPidl(lplvid->lpsfParent, lplvid->lpi);

		if (::ShellExecuteEx(&sei))
		{
			return true;
		}
	}

	return false;
}

BOOL CXTPShellListBase::GetItemPath(int iItem, CString& strItemPath)
{
	if (iItem >= 0)
	{
		// Long pointer to TreeView item data
		XTP_LVITEMDATA*  lplvid = (XTP_LVITEMDATA*)m_pListCtrl->GetItemData(iItem);
		if (lplvid != 0)
		{
			LPITEMIDLIST lpid = GetFullyQualPidl(lplvid->lpsfParent, lplvid->lpi);

			TCHAR szItemPath[_MAX_PATH];
			if (::SHGetPathFromIDList(lpid, szItemPath))
			{
				strItemPath = szItemPath;
				return TRUE;
			}
		}
	}

	return FALSE;
}

void CXTPShellListBase::OnDeleteListItem(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMTreeView = (NM_LISTVIEW*)pNMHDR;

	XTP_LVITEMDATA* lplvid = (XTP_LVITEMDATA*)pNMTreeView->lParam;
	if (lplvid != NULL)
	{
		CShellMalloc lpMalloc;

		if (lplvid->lpi)
		{
			lpMalloc.Free(lplvid->lpi);
			lplvid->lpi = NULL;
		}
		if (lplvid->lpsfParent)
		{
			lplvid->lpsfParent->Release();
			lplvid->lpsfParent = NULL;
		}

		delete lplvid;
	}

	*pResult = 0;
}


void CXTPShellListBase::UpdateList(int nMessage, XTP_TVITEMDATA* pItemData)
{
	switch (nMessage)
	{
		case SHN_XTP_CONTENTSCHANGED:
		case SHN_XTP_TREESELCHANGE:
			{
				CWnd* pOwner = m_pListCtrl->GetOwner();
				ASSERT_VALID(pOwner);
				if (!pOwner)
					return;

				// The tree view selection has changed, so update the contents
				// of the list view
				XTP_TVITEMDATA*  lptvid = (XTP_TVITEMDATA*)pItemData;
				ASSERT(lptvid != NULL);
				if (!lptvid)
					return;

				if (lptvid->lpsfParent == NULL)
				{
					CShellMalloc lpMalloc;
					if (!lpMalloc)
					{
						return;
					}

					LPSHELLFOLDER lpShellFolder;
					if (FAILED(::SHGetDesktopFolder(&lpShellFolder)))
					{
						return;
					}

					LPITEMIDLIST pidlDesktop = NULL;
					if (FAILED(::SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP, &pidlDesktop)))
					{
						lpShellFolder->Release();
						return;
					}

					IShellFolder *pFolder = NULL;
					if (lpShellFolder->CompareIDs(0, lptvid->lpifq, pidlDesktop) == 0)
					{
						pFolder = lpShellFolder;
					}
					else
					{
						lpShellFolder->BindToObject(lptvid->lpifq, 0, IID_IShellFolder, (LPVOID*)&pFolder);
						lpShellFolder->Release();
					}
					ASSERT (pFolder != 0);

					PopulateListView(lptvid, pFolder);

					if (m_pidlINet && (pFolder->CompareIDs(0, lptvid->lpifq, m_pidlINet) == 0))
					{
						pOwner->SendMessage(WM_XTP_SHELL_NOTIFY, SHN_XTP_INETFOLDER);
					}
					else
					{
						pOwner->SendMessage(WM_XTP_SHELL_NOTIFY);
					}

					if (pidlDesktop)
					{
						lpMalloc.Free(pidlDesktop);
					}
					if (pFolder)
					{
						pFolder->Release();
					}
				}
				else
				{
					LPSHELLFOLDER lpsf = NULL;
					if (SUCCEEDED(lptvid->lpsfParent->BindToObject(lptvid->lpi,
						0, IID_IShellFolder, (LPVOID*)&lpsf)))
					{
						PopulateListView(lptvid, lpsf);
						lpsf->Release();

						if (SUCCEEDED(::SHGetDesktopFolder(&lpsf)))
						{
							if (m_pidlINet && (lpsf->CompareIDs(0, lptvid->lpifq, m_pidlINet) == 0))
							{
								pOwner->SendMessage(WM_XTP_SHELL_NOTIFY, SHN_XTP_INETFOLDER);
							}
							else
							{
								pOwner->SendMessage(WM_XTP_SHELL_NOTIFY);
							}

							lpsf->Release();
						}
					}
					else
					{
						if (SUCCEEDED(::SHGetDesktopFolder(&lpsf)))
						{
							if (m_pidlINet && (lpsf->CompareIDs(0, lptvid->lpifq, m_pidlINet) == 0))
							{
								pOwner->SendMessage(WM_XTP_SHELL_NOTIFY, SHN_XTP_INETFOLDER);
							}

							lpsf->Release();
						}
					}
				}
				break;
			}

		case SHN_XTP_REFRESHFOLDER:
		case SHN_XTP_REFRESHTREE:
			{
				// Directory monitory thread has issued an update notification,
				// refresh the list control.
				XTP_TVITEMDATA*  lpTVID = (XTP_TVITEMDATA*)pItemData;
				ASSERT(lpTVID);
				if (!lpTVID)
					return;

				PopulateListView(lpTVID, lpTVID->lpsfParent);
				break;
			}

		case SHN_XTP_NOFOLDER:
			{
				// The item double clicked was not found in the treeview
				// so it sent us back a confirmation to execute it
				XTP_LVITEMDATA*  lplvid = (XTP_LVITEMDATA*)pItemData;
				ASSERT(lplvid);
				if (!lplvid)
					return;

				ShellOpenItem(lplvid);
			}
			break;

		default:
			break;
	}
}

BOOL CXTPShellListBase::RefreshRowColors()
{
	// if show compressed or encrypted disabled, return FALSE.
	if (!m_shSettings.ShowCompColor())
		return FALSE;

	// reset color list and settings values.
	m_arRowColor.RemoveAll();
	m_shSettings.RefreshSettings();

	for (int i = 0; i < m_pListCtrl->GetItemCount(); ++i)
	{
		XTP_LVITEMDATA* lplvid = (XTP_LVITEMDATA*)m_pListCtrl->GetItemData(i);
		if (lplvid)
		{
			if ((lplvid->ulAttribs & SFGAO_COMPRESSED) ||
				(lplvid->ulAttribs & SFGAO_ENCRYPTED))
			{
				ROWCOLOR rowColor;
				::ZeroMemory(&rowColor, sizeof(ROWCOLOR));

				rowColor.iRow   = i;
				rowColor.crBack = GetListBackColor();
				rowColor.lParam = lplvid->ulAttribs;

				if (lplvid->ulAttribs & SFGAO_COMPRESSED)
				{
					rowColor.crText = m_shSettings.m_crCompColor;
					SetRowColor(&rowColor, FALSE);
				}

				else if (lplvid->ulAttribs & SFGAO_ENCRYPTED)
				{
					rowColor.crText = m_shSettings.m_crEncrColor;
					SetRowColor(&rowColor, FALSE);
				}
			}
		}
	}

	return TRUE;
}

bool CXTPShellListBase::SortList(int nCol, bool bAscending)
{
	if (m_nSortedCol >= 0)
	{
		CXTPHeaderCtrl* pHeaderCtrl = XTPGetHeaderCtrl();
		if (pHeaderCtrl && pHeaderCtrl->GetSortedCol() < 0)
		{
			pHeaderCtrl->SetSortImage(nCol, bAscending);
		}
	}

	m_pListCtrl->SortItems(
		ListViewCompareProc, (nCol + 1)*(bAscending? 1: -1));

	RefreshRowColors();

	return true;
}

void CXTPShellListBase::SetAttributes(int iItem, DWORD dwAttributes)
{
	MapShellFlagsToItemAttributes(m_pListCtrl, iItem, dwAttributes);
}

void CXTPShellListBase::OnDragDrop(NM_LISTVIEW* /*pNMListView*/)
{
	COleDataSource oleDataSource;
	HGLOBAL        hgDrop;
	DROPFILES*     pDrop;
	CStringList    lsDraggedFiles;
	POSITION       pos;
	int            nSelItem;
	CString        sFile;
	UINT           uBuffSize = 0;
	TCHAR*         pszBuff;

	FORMATETC      etc = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };

	// For every selected item in the list, put the filename into lsDraggedFiles.
	pos = m_pListCtrl->GetFirstSelectedItemPosition();

	while (NULL != pos)
	{
		nSelItem = m_pListCtrl->GetNextSelectedItem(pos);
		GetItemPath(nSelItem, sFile);
		lsDraggedFiles.AddTail (sFile);

		// Calculate the # of chars required to hold this string.
		uBuffSize += lstrlen (sFile) + 1;
	}

	// Add 1 extra for the final null char, and the size of the DROPFILES struct.
	uBuffSize = sizeof(DROPFILES) + sizeof(TCHAR) * (uBuffSize + 1);

	// Allocate memory from the heap for the DROPFILES struct.
	hgDrop = ::GlobalAlloc (GHND | GMEM_SHARE, uBuffSize);

	if (NULL == hgDrop)
		return;

	pDrop = (DROPFILES*) ::GlobalLock (hgDrop);

	if (NULL == pDrop)
	{
		::GlobalFree (hgDrop);
		return;
	}

	// Fill in the DROPFILES struct.
	pDrop->pFiles = sizeof(DROPFILES);

#ifdef _UNICODE
	// If we're compiling for Unicode, set the Unicode flag in the struct to
	// indicate it contains Unicode strings.
	pDrop->fWide = TRUE;
#endif

	// Copy all the filenames into memory after the end of the DROPFILES struct.
	pos = lsDraggedFiles.GetHeadPosition();
	pszBuff = (TCHAR*) (LPBYTE(pDrop) + sizeof(DROPFILES));

	while (NULL != pos)
	{
		lstrcpy (pszBuff, (LPCTSTR) lsDraggedFiles.GetNext (pos));
		pszBuff = 1 + _tcschr (pszBuff, '\0');
	}

	::GlobalUnlock (hgDrop);

	// Put the data in the data source.
	oleDataSource.CacheGlobalData (CF_HDROP, hgDrop, &etc);

	// Add in our own custom data, so we know that the drag originated from our
	// window.  OnDragEnter() checks for this custom format, and
	// doesn't allow the drop if it's present.  This is how we prevent the user
	// from dragging and then dropping in our own window.
	// The data will just be a dummy bool.
	HGLOBAL hgBool;

	hgBool = ::GlobalAlloc (GHND | GMEM_SHARE, sizeof(bool));

	if (NULL == hgBool)
	{
		::GlobalFree (hgDrop);
		return;
	}

	static CLIPFORMAT clpFormat = (CLIPFORMAT)
		::RegisterClipboardFormat(_T("{B0D76F7A-B5D9-436c-8F10-BA16AEE69D42}"));

	// Put the data in the data source.
	etc.cfFormat = clpFormat;
	oleDataSource.CacheGlobalData(clpFormat, hgBool, &etc);

	// Start the drag 'n' drop!
	DROPEFFECT dwEffect = oleDataSource.DoDragDrop (DROPEFFECT_COPY | DROPEFFECT_MOVE);

	// If the DnD completed OK, we remove all of the dragged items from our
	//
	switch (dwEffect)
	{
	case DROPEFFECT_MOVE:
		{
			// The files were copied or moved.
			// Note: Don't call ::GlobalFree() because the data will be freed by the drop target.
			for (nSelItem = m_pListCtrl->GetNextItem (-1, LVNI_SELECTED);
			nSelItem != -1;
			nSelItem = m_pListCtrl->GetNextItem (nSelItem, LVNI_SELECTED))
			{
				m_pListCtrl->DeleteItem (nSelItem);
				nSelItem--;
			}
		}
		break;

	case DROPEFFECT_COPY:
		break;

	case DROPEFFECT_NONE:
		{
			// This needs special handling, because on NT, DROPEFFECT_NONE
			// is returned for move operations, instead of DROPEFFECT_MOVE.
			// See Q182219 for the details.
			// So if we're on NT, we check each selected item, and if the
			// file no longer exists, it was moved successfully and we can
			// remove it from the
			if (XTPSystemVersion()->IsWinNT4OrGreater())
			{
				bool bDeletedAnything = false;

				for (nSelItem = m_pListCtrl->GetNextItem (-1, LVNI_SELECTED);
				nSelItem != -1;
				nSelItem = m_pListCtrl->GetNextItem(nSelItem, LVNI_SELECTED))
				{
					GetItemPath(nSelItem, sFile);

					if (GetFileAttributes(sFile) == DWORD(-1) &&
						GetLastError() == ERROR_FILE_NOT_FOUND)
					{
						// We couldn't read the file's attributes, and GetLastError()
						// says the file doesn't exist, so remove the corresponding
						// item from the
						m_pListCtrl->DeleteItem(nSelItem);

						nSelItem--;
						bDeletedAnything = true;
					}
				}

				// Resize the list columns if we deleted any items.
				if (bDeletedAnything)
				{
					m_pListCtrl->SetColumnWidth (0, LVSCW_AUTOSIZE_USEHEADER);
					m_pListCtrl->SetColumnWidth (1, LVSCW_AUTOSIZE_USEHEADER);
					m_pListCtrl->SetColumnWidth (2, LVSCW_AUTOSIZE_USEHEADER);

					// Note: Don't call ::GlobalFree() because the data belongs to
					// the caller.
				}
				else
				{
					// The DnD operation wasn't accepted, or was canceled, so we
					// should call ::GlobalFree() to clean up.
					::GlobalFree (hgDrop);
					::GlobalFree (hgBool);
				}
			}   // end if (NT)
			else
			{
				// We're on 9x, and a return of DROPEFFECT_NONE always means
				// that the DnD operation was aborted.  We need to free the
				// allocated memory.
				::GlobalFree (hgDrop);
				::GlobalFree (hgBool);
			}
		}
		break;  // end case DROPEFFECT_NONE
	}   // end switch
}


bool CXTPShellListBase::Init()
{
	if (!CXTPListBase::Init())
		return false;

	if (!m_pListCtrl->GetImageList(LVSIL_SMALL))
	{
		// Initialize the columns and image list for the list control.
		BuildDefaultColumns();
		InitSystemImageLists();
		m_pListCtrl->ModifyStyle(NULL, LVS_REPORT | LVS_SHAREIMAGELISTS);

		SubclassHeader(FALSE);
		XTPGetHeaderCtrl()->ShowSortArrow(TRUE);

		m_pListCtrl->DragAcceptFiles(TRUE);
	}

	return true;
}

#ifndef LVS_EX_BORDERSELECT
#define LVS_EX_BORDERSELECT     0x00008000 // border selection style instead of highlight
#endif

#ifndef LVS_EX_DOUBLEBUFFER
#define LVS_EX_DOUBLEBUFFER     0x00010000
#endif


void CXTPShellListBase::SetExplorerStyle()
{
	if (XTPSystemVersion()->IsWinVistaOrGreater())
	{
		CXTPWinThemeWrapper().SetWindowTheme(m_pListCtrl->GetSafeHwnd(), L"EXPLORER", NULL);
	}
}
