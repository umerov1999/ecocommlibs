// XTPShortcutManager.cpp : implementation of the CXTPShortcutManager class.
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
#include "Resource.h"

#include "Common/XTPPropExchange.h"
#include "Common/XTPVC80Helpers.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPHookManager.h"

#include "XTPCommandBarsDefines.h"
#include "XTPShortcutManager.h"
#include "XTPControl.h"
#include "XTPControls.h"
#include "XTPCommandBar.h"
#include "XTPToolBar.h"
#include "XTPCommandBars.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPShortcutManagerAccel

CXTPShortcutManagerAccel::CXTPShortcutManagerAccel()
{
	cmd = 0;
	key[0].fVirt = 0;
	key[0].key = 0;
	key[1].fVirt = 0;
	key[1].key = 0;

	m_pManager = 0;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPShortcutManagerAccel::CXTPShortcutManagerAccel(ACCEL* pAccel)
{
	cmd = pAccel->cmd;

	key[0].fVirt = pAccel->fVirt;
	key[0].key = pAccel->key;

	key[1].fVirt = 0;
	key[1].key = 0;

	m_pManager = 0;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPShortcutManagerAccel::CXTPShortcutManagerAccel(const XTP_SHORTCUTMANAGER_ACCEL& accel)
{
	cmd = accel.cmd;

	key[0] = accel.key[0];
	key[1] = accel.key[1];

	m_pManager = NULL;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPShortcutManagerAccel::CXTPShortcutManagerAccel(const CXTPShortcutManagerAccel& accel)
{
	cmd = accel.cmd;

	key[0] = accel.key[0];
	key[1] = accel.key[1];

	m_pManager = NULL;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}


const CXTPShortcutManagerAccel& CXTPShortcutManagerAccel::operator=(const CXTPShortcutManagerAccel& accel)
{
	cmd = accel.cmd;

	key[0] = accel.key[0];
	key[1] = accel.key[1];

	m_pManager = accel.m_pManager;

	return *this;
}

//////////////////////////////////////////////////////////////////////////
// CXTPShortcutManagerAccelTable
CXTPShortcutManagerAccelTable::CXTPShortcutManagerAccelTable(CXTPShortcutManager* pManager)
{
	m_pManager = pManager;

}

CXTPShortcutManagerAccelTable::~CXTPShortcutManagerAccelTable()
{
	RemoveAll();
}

void CXTPShortcutManagerAccelTable::RemoveAll()
{
	for (int i = 0; i < GetCount(); i++)
	{
		m_arrAccels[i]->InternalRelease();
	}
	m_arrAccels.RemoveAll();
}

void CXTPShortcutManagerAccelTable::RemoveAt(int nIndex)
{
	if (nIndex >= 0 && nIndex < m_arrAccels.GetSize())
	{
		m_arrAccels[nIndex]->InternalRelease();
		m_arrAccels.RemoveAt(nIndex);
	}
}

void CXTPShortcutManagerAccelTable::Add(const XTP_SHORTCUTMANAGER_ACCEL& accel)
{
	CXTPShortcutManagerAccel* pAccel = new CXTPShortcutManagerAccel(accel);
	pAccel->m_pManager = m_pManager;

	m_arrAccels.Add(pAccel);
}

void CXTPShortcutManagerAccelTable::CopyAccelTable(LPACCEL lpAccel, int nSize)
{
	RemoveAll();

	for (int i = 0; i < nSize; i++)
	{
		Add(CXTPShortcutManagerAccel(&lpAccel[i]));
	}
}

XTP_SHORTCUTMANAGER_ACCEL* CXTPShortcutManagerAccelTable::CopyAccels() const
{
	XTP_SHORTCUTMANAGER_ACCEL* pAccels = new XTP_SHORTCUTMANAGER_ACCEL[GetCount()];

	for (int i = 0; i < GetCount(); i++)
	{
		pAccels[i] = *m_arrAccels[i];
	}

	return pAccels;
}

void CXTPShortcutManagerAccelTable::CopyAccelTable(HACCEL hAccelTable)
{
	RemoveAll();

	if (hAccelTable == NULL)
		return;

	int nAccelSize = ::CopyAcceleratorTable (hAccelTable, NULL, 0);
	if (nAccelSize == 0)
		return;

	LPACCEL lpAccel = new ACCEL[nAccelSize];
	::CopyAcceleratorTable (hAccelTable, lpAccel, nAccelSize);

	CopyAccelTable(lpAccel, nAccelSize);

	delete[] lpAccel;
}

int CXTPShortcutManagerAccelTable::GetCount() const
{
	return (int)m_arrAccels.GetSize();
}

CXTPShortcutManagerAccel* CXTPShortcutManagerAccelTable::GetAt(int nIndex) const
{
	return nIndex >= 0 && nIndex < m_arrAccels.GetSize() ?  m_arrAccels.GetAt(nIndex) : NULL;
}

void CXTPShortcutManagerAccelTable::CopyAccelTable(CXTPShortcutManagerAccelTable* pAccelTable)
{
	RemoveAll();

	int nAccelSize = pAccelTable->GetCount();

	for (int i = 0; i < nAccelSize; i++)
	{
		Add(*pAccelTable->GetAt(i));
	}
}

//////////////////////////////////////////////////////////////////////////
// CXTPShortcutManagerKeyNameText::CXTPShortcutManagerKeyNameText

CXTPShortcutManager::CKeyNameText::CKeyNameText()
{
	static const struct
	{
		WORD    wKey;       // Virtual Key Code.
		LPCTSTR szKeyName;  // Display Name (i.e "CTRL").
	}
	virtKeys[] =
	{
		{ _T('0'), _T("0") },
		{ _T('1'), _T("1") },
		{ _T('2'), _T("2") },
		{ _T('3'), _T("3") },
		{ _T('4'), _T("4") },
		{ _T('5'), _T("5") },
		{ _T('6'), _T("6") },
		{ _T('7'), _T("7") },
		{ _T('8'), _T("8") },
		{ _T('9'), _T("9") },
		{ _T('A'), _T("A") },
		{ _T('B'), _T("B") },
		{ _T('C'), _T("C") },
		{ _T('D'), _T("D") },
		{ _T('E'), _T("E") },
		{ _T('F'), _T("F") },
		{ _T('G'), _T("G") },
		{ _T('H'), _T("H") },
		{ _T('I'), _T("I") },
		{ _T('J'), _T("J") },
		{ _T('K'), _T("K") },
		{ _T('L'), _T("L") },
		{ _T('M'), _T("M") },
		{ _T('N'), _T("N") },
		{ _T('O'), _T("O") },
		{ _T('P'), _T("P") },
		{ _T('Q'), _T("Q") },
		{ _T('R'), _T("R") },
		{ _T('S'), _T("S") },
		{ _T('T'), _T("T") },
		{ _T('U'), _T("U") },
		{ _T('V'), _T("V") },
		{ _T('W'), _T("W") },
		{ _T('X'), _T("X") },
		{ _T('Y'), _T("Y") },
		{ _T('Z'), _T("Z") },
		{ VK_LBUTTON, _T("Left Button") },
		{ VK_RBUTTON, _T("Right Button") },
		{ VK_CANCEL, _T("Ctrl+Break") },
		{ VK_MBUTTON, _T("Middle Button") },
		{ VK_BACK, _T("Backspace") },
		{ VK_TAB, _T("Tab") },
		{ VK_CLEAR, _T("Clear") },
		{ VK_RETURN, _T("Enter") },
		{ VK_SHIFT, _T("Shift") },
		{ VK_CONTROL, _T("Ctrl") },
		{ VK_MENU, _T("Alt") },
		{ VK_PAUSE, _T("Pause") },
		{ VK_CAPITAL, _T("Caps Lock") },
		{ VK_ESCAPE, _T("Esc") },
		{ VK_SPACE, _T("Space") },
		{ VK_PRIOR, _T("Page Up") },
		{ VK_NEXT, _T("Page Down") },
		{ VK_END, _T("End") },
		{ VK_HOME, _T("Home") },
		{ VK_LEFT, _T("Left Arrow") },
		{ VK_UP, _T("Up Arrow") },
		{ VK_RIGHT, _T("Right Arrow") },
		{ VK_DOWN, _T("Down Arrow") },
		{ VK_SELECT, _T("Select") },
		{ VK_PRINT, _T("Print") },
		{ VK_EXECUTE, _T("Execute") },
		{ VK_SNAPSHOT, _T("Snapshot") },
		{ VK_INSERT, _T("Ins") },
		{ VK_DELETE, _T("Del") },
		{ VK_HELP, _T("Help") },
		{ VK_LWIN , _T("Win") },
		{ VK_RWIN, _T("Win") },
		{ VK_APPS, _T("Application") },
		{ VK_MULTIPLY, _T("Num *") },
		{ VK_ADD, _T("Num +") },
		{ VK_SEPARATOR, _T("Separator") },
		{ VK_SUBTRACT, _T("Num -") },
		{ VK_DECIMAL, _T("Num .") },
		{ VK_DIVIDE, _T("Num /") },
		{ VK_F1, _T("F1") },
		{ VK_F2, _T("F2") },
		{ VK_F3, _T("F3") },
		{ VK_F4, _T("F4") },
		{ VK_F5, _T("F5") },
		{ VK_F6, _T("F6") },
		{ VK_F7, _T("F7") },
		{ VK_F8, _T("F8") },
		{ VK_F9, _T("F9") },
		{ VK_F10, _T("F10") },
		{ VK_F11, _T("F11") },
		{ VK_F12, _T("F12") },
		{ VK_NUMPAD0, _T("Num 0") },
		{ VK_NUMPAD1, _T("Num 1") },
		{ VK_NUMPAD2, _T("Num 2") },
		{ VK_NUMPAD3, _T("Num 3") },
		{ VK_NUMPAD4, _T("Num 4") },
		{ VK_NUMPAD5, _T("Num 5") },
		{ VK_NUMPAD6, _T("Num 6") },
		{ VK_NUMPAD7, _T("Num 7") },
		{ VK_NUMPAD8, _T("Num 8") },
		{ VK_NUMPAD9, _T("Num 9") },
		{ VK_NUMLOCK, _T("Num Lock") },
		{ VK_SCROLL, _T("Scrl Lock") },
		{ VK_ATTN, _T("Attn") },
		{ VK_CRSEL, _T("Crsel") },
		{ VK_EXSEL, _T("Exsel") },
		{ VK_EREOF, _T("Ereof") },
		{ VK_PLAY, _T("Play") },
		{ VK_ZOOM, _T("Zoom") },
		{ VK_NONAME, _T("No Name") },
		{ VK_PA1, _T("Pa1") },
		{ VK_OEM_CLEAR, _T("Oem Clear") },
	};

	for (int i = 0; i < _countof(virtKeys); i++)
	{
		SetAt(virtKeys[i].wKey, virtKeys[i].szKeyName);
	}
}

WORD CXTPShortcutManager::CKeyNameText::Parse(LPCTSTR lpszKey)
{
	POSITION pos = m_mapVirtualKeys.GetStartPosition();
	while (pos)
	{
		WORD rKey;
		CString strKey;

		m_mapVirtualKeys.GetNextAssoc(pos, rKey, strKey);

		if (strKey.CompareNoCase(lpszKey) == 0)
			return rKey;
	}

	return 0;
}

CString CXTPShortcutManager::CKeyNameText::Translate(UINT nKey)
{
	CString strKeyNameText;

	if (m_mapVirtualKeys.Lookup((WORD)nKey, strKeyNameText))
	{
		return strKeyNameText;
	}

	return _T("");
}

void CXTPShortcutManager::CKeyNameText::SetAt(UINT uiVirtKey, LPCTSTR strKeyNameText)
{
	m_mapVirtualKeys.SetAt((WORD)uiVirtKey, strKeyNameText);
}


#define SAFE_DESTROY_ACCELTABLE(hAccelTable) if (hAccelTable) {::DestroyAcceleratorTable (hAccelTable); hAccelTable = NULL;}

CXTPShortcutManager::CXTPShortcutManager(CXTPCommandBars* pCommandBars)
{
	m_pCommandBars = pCommandBars;

	m_pAccelTable = new CXTPShortcutManagerAccelTable(this);
	m_pOriginalAccelTable = NULL;

	m_bAllowEscapeShortcut = FALSE;
	m_bUseSystemKeyNameText = FALSE;
	m_bAllowDoubleKeyShortcuts = TRUE;

	m_pKeyNameText = new CKeyNameText();

	m_nDisableShortcuts = 0;
	m_bDisableOnCapture = TRUE;

	m_nEnableCustomShortcutText = FALSE;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
	EnableConnections();
#endif
}

CXTPShortcutManager::~CXTPShortcutManager()
{
	SAFE_DELETE(m_pAccelTable);
	SAFE_DELETE(m_pOriginalAccelTable);

	SAFE_DELETE(m_pKeyNameText);

#ifdef _XTP_ACTIVEX
	ReleaseConnections();
#endif
}

void CXTPShortcutManager::SetAccelerators(UINT nIDResource)
{
	m_pAccelTable->RemoveAll();

	LPCTSTR lpszResourceName = MAKEINTRESOURCE(nIDResource);
	HINSTANCE hInst = AfxFindResourceHandle(lpszResourceName, RT_ACCELERATOR);

	if (hInst)
	{
		HACCEL hAccel = ::LoadAccelerators(hInst, lpszResourceName);

		m_pAccelTable->CopyAccelTable(hAccel);
		CreateOriginalAccelTable();
	}
}

void CXTPShortcutManager::SetDefaultAccelerator(HACCEL hAccelTable)
{
	m_pAccelTable->CopyAccelTable(hAccelTable);
	CreateOriginalAccelTable();
}

void CXTPShortcutManager::CreateOriginalAccelTable()
{
	if (m_pOriginalAccelTable == NULL)
		m_pOriginalAccelTable = new CXTPShortcutManagerAccelTable(this);

	m_pOriginalAccelTable->CopyAccelTable(m_pAccelTable);
}

void CXTPShortcutManager::Reset()
{
	ASSERT(m_pOriginalAccelTable);

	if (m_pOriginalAccelTable)
	{
		m_pAccelTable->CopyAccelTable(m_pOriginalAccelTable);
	}
}

CString CXTPShortcutManager::Format(CXTPShortcutManagerAccel* lpAccel, int* pPriority)
{
	CString str;
	CKeyHelper helper (lpAccel, this);
	helper.Format (str);

	if (pPriority)
	{
		*pPriority = helper.Priority();
	}

	return str;
}

BOOL CXTPShortcutManager::OnPreviewEditKey(CXTPShortcutManagerAccel* pAccel)
{
	if ((pAccel->key[0].fVirt & FCONTROL) == 0
		&& (pAccel->key[0].fVirt & FSHIFT) == 0
		&& (pAccel->key[0].fVirt & FALT) == 0
		&& (pAccel->key[0].fVirt & FVIRTKEY)
		&& (pAccel->key[0].key == VK_ESCAPE) && !m_bAllowEscapeShortcut)
		return FALSE;

	return TRUE;
}

void CXTPShortcutManager::DisableShortcuts(BOOL bDisable)
{
	m_nDisableShortcuts += bDisable ? +1 : -1;
}

BOOL CXTPShortcutManager::IsAccelMessage(CXTPShortcutManagerAccel::SHORTCUTACCEL& accel, int nKeyState, LPMSG lpMsg) const
{
	BOOL fVirt = lpMsg->message == WM_KEYDOWN || lpMsg->message == WM_SYSKEYDOWN;
	WORD flags = accel.fVirt;

	if ( (DWORD)accel.key != lpMsg->wParam ||
		((fVirt != 0) != ((flags & FVIRTKEY) != 0)))
	{
		return FALSE;
	}

	if (fVirt && ((nKeyState & (FSHIFT | FCONTROL)) != (flags & (FSHIFT | FCONTROL))))
	{
		return FALSE;
	}

	if ((nKeyState & FALT) != (flags & FALT))
	{
		return FALSE;
	}

	return TRUE;
}

int CXTPShortcutManager::GetAccelKeyState() const
{
	int vkCtrl = VK_CONTROL;
	int vkAlt = VK_MENU;

	if ((GetKeyState(VK_RMENU) & 0x8000) && (GetKeyState(VK_LCONTROL) & 0x8000))
	{
		vkCtrl = VK_RCONTROL;
		vkAlt = VK_LMENU;
	}

	int nKeyState = 0;
	if (GetKeyState(vkCtrl) & 0x8000) nKeyState |= FCONTROL;
	if (GetKeyState(vkAlt) & 0x8000) nKeyState |= FALT;
	if (GetKeyState(VK_SHIFT) & 0x8000) nKeyState |= FSHIFT;

	return nKeyState;
}

BOOL CXTPShortcutManager::TranslateAccelerator(LPMSG lpMsg)
{
	if (!m_pCommandBars || m_pCommandBars->IsCustomizeMode())
		return FALSE;

	HWND hWnd = m_pCommandBars->GetSite()->GetSafeHwnd();

	if (m_nDisableShortcuts > 0)
		return FALSE;

	CXTPShortcutManagerAccelTable* pAccelTable = GetDefaultAccelerator();

	if (pAccelTable == NULL || pAccelTable->GetCount() == 0)
		return FALSE;

	if (lpMsg->message != WM_KEYDOWN && lpMsg->message != WM_SYSKEYDOWN && lpMsg->message != WM_CHAR && lpMsg->message != WM_SYSCHAR)
		return FALSE;

	int nKeyState = GetAccelKeyState();

	BOOL bFound = FALSE;

	int nAccelSize = pAccelTable->GetCount();

	for (int i = 0; i < nAccelSize; i++)
	{
		CXTPShortcutManagerAccel* accel = pAccelTable->GetAt(i);

		if (!IsAccelMessage(accel->key[0], nKeyState, lpMsg))
			continue;

		bFound = TRUE;

		if (IsWindowEnabled(hWnd) && (!m_bDisableOnCapture || ::GetCapture() == NULL) && !IsIconic(hWnd) && accel->cmd != 0)
		{
			if (accel->key[1].key != 0)
			{
				if (m_pCommandBars->GetSite()->IsFrameWnd())
				{
					CKeyHelper key(accel, this);
					CString strMessage, strFormat;

					CXTPResourceManager::AssertValid(XTPResourceManager()->LoadString(&strFormat, XTP_IDS_SHORTCUT_SECONDKEY));
					strMessage.Format(strFormat, (LPCTSTR)key.Format(&accel->key[0]));

					((CFrameWnd*)m_pCommandBars->GetSite())->SetMessageText(strMessage);
				}

				BOOL bAccept = FALSE;

				m_nDisableShortcuts++;

				::SetCapture(hWnd);
				while (::GetCapture() == hWnd)
				{
					MSG msg;
					VERIFY(::GetMessage(&msg, NULL, 0, 0));

					if (msg.message == WM_KEYDOWN || msg.message == WM_SYSKEYDOWN)
					{
						if (msg.wParam == VK_SHIFT || msg.wParam == VK_CONTROL || msg.wParam == VK_MENU)
							continue;

						nKeyState = GetAccelKeyState();

						for (int j = 0; j < nAccelSize; j++)
						{
							CXTPShortcutManagerAccel* secondAccel = pAccelTable->GetAt(j);

							if (secondAccel->key[0].fVirt == accel->key[0].fVirt && secondAccel->key[0].key == accel->key[0].key)
							{
								if (!IsAccelMessage(secondAccel->key[1], nKeyState, &msg))
									continue;

								bAccept = TRUE;
								accel = secondAccel;
								break;
							}
						}


						if (!bAccept)
						{
							MessageBeep(MB_ICONEXCLAMATION);
						}

						break;
					}

					DispatchMessage (&msg);

					if (msg.message == WM_COMMAND || msg.message == WM_ACTIVATE || msg.message == WM_LBUTTONDOWN ||
						msg.message == WM_NCACTIVATE || msg.message == WM_CLOSE || msg.message == WM_SYSCOMMAND)
						break;
				}
				::ReleaseCapture();

				m_nDisableShortcuts--;

				if (IsWindow(hWnd) && m_pCommandBars->GetSite()->IsFrameWnd())
				{
					((CFrameWnd*)m_pCommandBars->GetSite())->SetMessageText((UINT)0);
				}

				if (!bAccept)
					return TRUE;
			}

#ifndef _XTP_ACTIVEX
			SendMessage(hWnd, WM_COMMAND, accel->cmd, 0);
#else
			if (OleBeforeTranslate(accel->cmd))
				return FALSE;

			::PostMessage(hWnd, WM_XTP_DELAYEXECUTE, accel->cmd, 0);
#endif
		}

		break;

	}

	return bFound;
}

BOOL CXTPShortcutManager::FindDefaultFrameAccelerator(int nCmd, CString& strShortcut)
{
	CFrameWnd* pFrame = DYNAMIC_DOWNCAST(CFrameWnd, m_pCommandBars->GetSite());
	HACCEL hAccelTable = pFrame ? pFrame->m_hAccelTable : NULL;

	if (!hAccelTable)
		return FALSE;

	ASSERT(hAccelTable);

	int nAccelSize = ::CopyAcceleratorTable (hAccelTable, NULL, 0);

	LPACCEL lpAccel = new ACCEL[nAccelSize];
	::CopyAcceleratorTable (hAccelTable, lpAccel, nAccelSize);

	BOOL bFound = FALSE;
	BOOL bEqual = FALSE;
	CString strFirst = _T("");
	int nFirstPriorety = 0;


	for (int i = 0; i < nAccelSize; i++)
	{
		if (lpAccel[i].cmd == nCmd)
		{
			int nPriority = 0;
			CXTPShortcutManagerAccel accel(&lpAccel[i]);
			CString str = Format(&accel, &nPriority);

			if (str == strShortcut)
				bEqual = TRUE;

			if (strFirst.IsEmpty() || (nFirstPriorety < nPriority))
			{
				strFirst = str;
				nFirstPriorety = nPriority;
			}

			bFound = TRUE;
		}
	}
	delete[] lpAccel;

	if (!bFound)
		strShortcut = "";
	else if (!bEqual)
		strShortcut = strFirst;

	return bFound;
}

BOOL CXTPShortcutManager::FindDefaultAccelerator(int nCmd, CString& strShortcut)
{
	CXTPShortcutManagerAccelTable* pAccelTable = GetDefaultAccelerator();

	int nAccelSize = pAccelTable ? pAccelTable->GetCount() : 0;

	if (m_pOriginalAccelTable == NULL && nAccelSize == 0)
		return FindDefaultFrameAccelerator(nCmd, strShortcut);

	if (nAccelSize == 0)
		return FALSE;

	BOOL bFound = FALSE;
	BOOL bEqual = FALSE;
	CString strFirst = _T("");
	int nFirstPriorety = 0;

	for (int i = 0; i < nAccelSize; i++)
	{
		CXTPShortcutManagerAccel* accel = pAccelTable->GetAt(i);
		if (accel->cmd == (int)nCmd)
		{
			int nPriority = 0;
			CString str = Format(accel, &nPriority);

			if (str == strShortcut)
				bEqual = TRUE;

			if (strFirst.IsEmpty() || (nFirstPriorety < nPriority))
			{
				strFirst = str;
				nFirstPriorety = nPriority;
			}

			bFound = TRUE;
		}
	}

	if (!bFound)
		strShortcut = "";
	else if (!bEqual)
		strShortcut = strFirst;

	return bFound;
}

CXTPShortcutManager::CKeyHelper::CKeyHelper(const CXTPShortcutManagerAccel* lpAccel, CXTPShortcutManager* pManager/*= NULL*/) :
	m_pManager(pManager), m_lpAccel (lpAccel)
{
	m_bAllowLocaleKey = TRUE;
}

CXTPShortcutManager::CKeyHelper::~CKeyHelper()
{
}

int CXTPShortcutManager::CKeyHelper::Priority()
{
	if (m_lpAccel == NULL)
		return 0;
	if (m_lpAccel->key[0].fVirt & FCONTROL)
		return 3;
	if (m_lpAccel->key[0].fVirt & FALT)
		return 2;
	if (m_lpAccel->key[0].fVirt & FSHIFT)
		return 1;
	return 4;
}

void CXTPShortcutManager::CKeyHelper::Format(CString& str) const
{
	str.Empty ();

	if (m_lpAccel == NULL)
	{
		ASSERT (FALSE);
		return;
	}

	str = Format(&m_lpAccel->key[0]);

	if (m_lpAccel->key[1].key != 0)
	{
		CString str2 = Format(&m_lpAccel->key[1]);

		if (!str2.IsEmpty())
		{
			str += _T(", ") + str2;
		}
	}
}

CString CXTPShortcutManager::CKeyHelper::Format(const CXTPShortcutManagerAccel::SHORTCUTACCEL* pAccel) const
{
	CString str;

	if (pAccel->key != VK_CANCEL)
	{
		if (pAccel->fVirt & FCONTROL)
		{
			AddVirtKeyStr(str, VK_CONTROL);
		}

		if (pAccel->fVirt & FSHIFT)
		{
			AddVirtKeyStr(str, VK_SHIFT);
		}

		if (pAccel->fVirt & FALT)
		{
			AddVirtKeyStr(str, VK_MENU);
		}

		if (pAccel->fVirt & 0x20)
		{
			AddVirtKeyStr (str, VK_LWIN);
		}
	}

	if (pAccel->key)
	{
		if (pAccel->fVirt & FVIRTKEY)
		{
			AddVirtKeyStr(str, pAccel->key, TRUE);
		}
		else if ((pAccel->key >= 1 && pAccel->key <= 26) && (pAccel->fVirt == FNOINVERT))
		{
			AddVirtKeyStr(str, VK_CONTROL);
			str += (char) ('A' + pAccel->key - 1);
		}
		else if (pAccel->key != VK_ESCAPE && pAccel->key != VK_TAB)
		{
			str += (char) pAccel->key;
		}
	}
	else if (str.GetLength() > 0)
	{
		str = str.Left(str.GetLength() - 1);
	}
	return str;
}


void CXTPShortcutManager::CKeyHelper::AddVirtKeyStr(CString& str, UINT uiVirtKey, BOOL bLast) const
{
	ASSERT(m_pManager);
	if (!m_pManager)
		return;

	CString strKey;

	if (m_bAllowLocaleKey)
	{
		strKey = m_pManager->GetKeyNameText(uiVirtKey);
	}
	else
	{
		strKey = m_pManager->m_pKeyNameText->Translate(uiVirtKey);

		if (strKey.IsEmpty())
		{
			strKey.Format(_T("0x%x"), uiVirtKey);
		}

	}

	if (!strKey.IsEmpty())
	{
		str += strKey;

		if (!bLast)
		{
			str += '+';
		}
	}
}

CString CXTPShortcutManager::CKeyHelper::GetLocalKeyNameText(UINT uiVirtKey)
{
	#define BUFFER_LEN 50
	TCHAR szBuffer[BUFFER_LEN + 1];
	ZeroMemory(szBuffer, BUFFER_LEN);

	if (uiVirtKey == VK_CANCEL)
		return _T("");

	UINT nScanCode = ::MapVirtualKeyEx (uiVirtKey, 0,
		::GetKeyboardLayout (0)) << 16 | 0x1;

	if (uiVirtKey >= VK_PRIOR && uiVirtKey <= VK_HELP)
	{
		nScanCode |= 0x01000000;
	}

	::GetKeyNameText(nScanCode, szBuffer, BUFFER_LEN);

	CString strKey = szBuffer;

	if (!strKey.IsEmpty())
	{
		strKey.MakeLower();

		for (int i = 0; i < strKey.GetLength(); i++)
		{
			TCHAR c = strKey[i];
			if (IsCharLower(c))
			{
				strKey.SetAt (i, CXTPShortcutManager::ToUpper(c));
				break;
			}
		}
	}

	return strKey;
}

int CXTPShortcutManager::FindAccelPos(LPCTSTR lpszAccel)
{
	LPCTSTR lpsz = _tcschr(lpszAccel, _T('&'));

	while (lpsz && *(lpsz + 1) == _T('&'))
	{
		lpsz = _tcschr(lpsz + 2, _T('&'));
	}

	return (lpsz == NULL) ? -1 : (int)(lpsz - lpszAccel);
}

BOOL CXTPShortcutManager::CompareAccelKey(TCHAR chAccel, UINT wParam)
{
	if (wParam >= VK_NUMPAD0 && wParam <= VK_NUMPAD9)
		return (UINT)chAccel == (wParam + '0' - VK_NUMPAD0);

	TCHAR tchVirtualKey = (TCHAR)MapVirtualKey(wParam, 2);
	if (tchVirtualKey == NULL)
		return FALSE;

	TCHAR chAccelUpper = ToUpper(chAccel);

	if ((chAccel == (TCHAR)wParam) || (chAccelUpper == (TCHAR)wParam))
		return TRUE;

	int nLayoutCount = GetKeyboardLayoutList(0, 0);
	if (nLayoutCount >= 1)
	{
		CArray<HKL, HKL&> arrLayoutList;
		arrLayoutList.SetSize(nLayoutCount);

		GetKeyboardLayoutList(nLayoutCount, arrLayoutList.GetData());

		BYTE keyState[256];
		if (!GetKeyboardState (keyState))
			return FALSE;

		for (int i = 0; i < nLayoutCount; i++)
		{
			WORD chKey = 0;

#ifdef _UNICODE
			if (ToUnicodeEx((UINT)wParam, 0, keyState, (LPWSTR)&chKey, 1, 0, arrLayoutList[i]) == 1)
#else
			if (ToAsciiEx((UINT)wParam, 0, keyState, &chKey, 0, arrLayoutList[i]) == 1)
#endif
			{
				if ((chAccel == (TCHAR)chKey) || (chAccelUpper == ToUpper((TCHAR)chKey)))
					return TRUE;
			}

			BYTE ksShift = keyState[VK_SHIFT];
			keyState[VK_SHIFT]= 0x81;

#ifdef _UNICODE
			if (ToUnicodeEx((UINT)wParam, 0, keyState, (LPWSTR)&chKey, 1, 0, arrLayoutList[i]) == 1)
#else
			if (ToAsciiEx((UINT)wParam, 0, keyState, &chKey, 0, arrLayoutList[i]) == 1)
#endif
			{
				if ((chAccel == (TCHAR)chKey) || (chAccelUpper == ToUpper((TCHAR)chKey)))
					return TRUE;
			}

			keyState[VK_SHIFT] = ksShift;
		}
	}

	return FALSE;
}

void CXTPShortcutManager::SetKeyNameText(UINT uiVirtKey, LPCTSTR strKeyNameText)
{
	m_pKeyNameText->SetAt(uiVirtKey, strKeyNameText);
}

CString CXTPShortcutManager::GetKeyNameText(UINT uiVirtKey)
{
	CString strKey;

	if (m_bUseSystemKeyNameText)
	{
		strKey = CKeyHelper::GetLocalKeyNameText(uiVirtKey);
	}

	if (strKey.IsEmpty())
	{
		strKey = m_pKeyNameText->Translate(uiVirtKey);
	}

	if (strKey.IsEmpty() && !m_bUseSystemKeyNameText)
	{
		strKey = CKeyHelper::GetLocalKeyNameText(uiVirtKey);
	}

	return strKey;
}

TCHAR CXTPShortcutManager::ToUpper(TCHAR vkTCHAR)
{
	TCHAR szChar[2] = {vkTCHAR, _T('\0') };

	CharUpper(szChar);
	return szChar[0];
}


/////////////////////////////////////////////////////////////////////////////
// CKeyAssign

CXTPShortcutManager::CKeyAssign::CKeyAssign(CXTPShortcutManager* pManager/*= NULL*/)
	: m_keyHelper(&m_accel, pManager)
{
	m_nKeyDefined = 0;
	m_bExtendedOnly = FALSE;

	m_bAllowDoubleKeyShortcuts = pManager ? pManager->m_bAllowDoubleKeyShortcuts : FALSE;

	ResetKey();
}

CXTPShortcutManager::CKeyAssign::~CKeyAssign()
{
}

BOOL CXTPShortcutManager::CKeyAssign::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_MBUTTONDOWN || pMsg->message == WM_RBUTTONDOWN)
	{
		SetFocus ();
		return TRUE;
	}

	if (pMsg->message != WM_KEYDOWN && pMsg->message != WM_SYSKEYDOWN && pMsg->message != WM_KEYUP && pMsg->message != WM_SYSKEYUP)
		return CEdit::PreTranslateMessage(pMsg);

	if (m_bAllowDoubleKeyShortcuts)
		return TranslateDoubleKeyShortcutsMessage(pMsg);

	return TranslateSingleKeyShortcutsMessage(pMsg);
}

BOOL CXTPShortcutManager::CKeyAssign::TranslateDoubleKeyShortcutsMessage(MSG* pMsg)
{
	BOOL bPressed = pMsg->message == WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN;
	if (!bPressed)
		return FALSE;

	BOOL bControlPressed = ::GetKeyState(VK_CONTROL) & 0x8000;
	BOOL bAltPressed = ::GetKeyState(VK_MENU) & 0x8000;

	if (!bControlPressed && !bAltPressed && (pMsg->wParam == VK_TAB))
	{
		return FALSE;
	}

	if (pMsg->wParam == VK_SHIFT || pMsg->wParam == VK_CONTROL || pMsg->wParam == VK_MENU)
		return FALSE;

	if (pMsg->wParam == VK_BACK)
	{
		if (m_nKeyDefined == 2)
		{
			m_accel.key[1].fVirt = 0;
			m_accel.key[1].key = 0;
			m_nKeyDefined = 1;
		}
		else
		{
			ResetKey();
		}
	}
	else
	{
		if (m_nKeyDefined > 1)
			ResetKey();

		if (m_nKeyDefined > 1)
		{
			ASSERT(FALSE);
			return FALSE;
		}

		// read in the actual state because we were not tracking releases if there was a
		// definition
		SetAccelFlag(m_nKeyDefined, FSHIFT, ::GetKeyState(VK_SHIFT) & 0x8000);
		SetAccelFlag(m_nKeyDefined, FCONTROL, bControlPressed);
		SetAccelFlag(m_nKeyDefined, FALT, bAltPressed);

		m_accel.key[m_nKeyDefined].key = (WORD)pMsg->wParam;
		SetAccelFlag(m_nKeyDefined, FVIRTKEY, TRUE);

		m_nKeyDefined++;
	}

	if (m_keyHelper.GetShortcutManager() && !m_keyHelper.GetShortcutManager()->OnPreviewEditKey(&m_accel))
	{
		ResetKey();
		return TRUE;
	}

	CString str;
	m_keyHelper.Format(str);

	SetWindowText(str);

	SetSel(str.GetLength(), str.GetLength());

	return TRUE;
}

BOOL CXTPShortcutManager::CKeyAssign::TranslateSingleKeyShortcutsMessage(MSG* pMsg)
{
	BOOL bPressed = pMsg->message == WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN;

	BOOL bControlPressed = ::GetKeyState(VK_CONTROL) & 0x8000;
	BOOL bAltPressed = ::GetKeyState(VK_MENU) & 0x8000;
	BOOL bWinPressed = (::GetKeyState(VK_LWIN) & 0x8000) || (::GetKeyState(VK_RWIN) & 0x8000);

	if (!bControlPressed && !bAltPressed && (pMsg->wParam == VK_TAB))
	{
		if (m_nKeyDefined == 0)
			ResetKey();

		return FALSE;
	}

	if (bPressed && !((1 << 30) & pMsg->lParam))
	{
		ResetKey();
	}

	if (m_bExtendedOnly)
	{
		if (bPressed)
		{
			SetAccelFlag(0, FSHIFT, GetKeyState(VK_SHIFT) < 0);
			SetAccelFlag(0, FCONTROL, GetKeyState(VK_CONTROL) < 0);
			SetAccelFlag(0, FALT, GetKeyState(VK_MENU) < 0);
			SetAccelFlag(0, 0x20, bWinPressed);
		}
	}
	else if (m_nKeyDefined == 0)
	{
		// read in the actual state because we were not tracking releases if there was a
		// definition
		SetAccelFlag(0, FSHIFT, ::GetKeyState(VK_SHIFT) & 0x8000);
		SetAccelFlag(0, FCONTROL, bControlPressed);
		SetAccelFlag(0, FALT, bAltPressed);
		SetAccelFlag(0, 0x20, bWinPressed);

		if (pMsg->wParam == VK_SHIFT
			|| pMsg->wParam == VK_CONTROL
			|| pMsg->wParam == VK_MENU
			|| pMsg->wParam == VK_LWIN
			|| pMsg->wParam == VK_RWIN
			)
		{
			// all work is already done
		}
		else if (bPressed)
		{
			m_accel.key[0].key = (WORD)pMsg->wParam;
			SetAccelFlag(0, FVIRTKEY, TRUE);
			m_nKeyDefined = 1;
		}
	}

	if (m_keyHelper.GetShortcutManager() && !m_keyHelper.GetShortcutManager()->OnPreviewEditKey(&m_accel))
	{
		ResetKey();
		return TRUE;
	}

	CString str;
	m_keyHelper.Format (str);

	SetWindowText(str);
	return TRUE;
}

void CXTPShortcutManager::CKeyAssign::ResetKey()
{
	m_accel.cmd = 0;
	m_accel.key[0].fVirt = m_accel.key[1].fVirt = 0;
	m_accel.key[0].key = m_accel.key[1].key = 0;


	m_nKeyDefined = 0;

	if (m_hWnd != NULL)
	{
		SetWindowText (_T(""));
	}
}

void CXTPShortcutManager::CKeyAssign::SetAccelFlag(int nKey, BYTE bFlag, BOOL bSet)
{
	ASSERT(nKey == 0 || nKey == 1);
	if (nKey > 1)
		return;

	if (bSet) m_accel.key[nKey].fVirt |= bFlag;
	else m_accel.key[nKey].fVirt &= ~bFlag;
}

BOOL CXTPShortcutManager::CKeyAssign::IsKeyDefined () const
{
	return m_nKeyDefined != 0;
}

CXTPShortcutManagerAccel* CXTPShortcutManager::CKeyAssign::GetAccel()
{
	return &m_accel;
}

void CXTPShortcutManager::CKeyAssign::SetAccel(CXTPShortcutManagerAccel* pAccel)
{
	m_accel = *pAccel;

	m_nKeyDefined = (m_accel.key[0].key != 0 ? 1 : 0) + (m_accel.key[1].key != 0 ? 1 : 0);

	if (m_hWnd != NULL)
	{
		CString str;
		m_keyHelper.Format (str);

		SetWindowText (str);
	}
}

void CXTPShortcutManager::UpdateAcellTable(LPACCEL lpAccel, int nSize)
{
	m_pAccelTable->CopyAccelTable(lpAccel, nSize);
}

void CXTPShortcutManager::UpdateAcellTable(XTP_SHORTCUTMANAGER_ACCEL* lpAccel, int nSize)
{
	m_pAccelTable->RemoveAll();

	for (int i = 0; i < nSize; i++)
	{
		m_pAccelTable->Add(lpAccel[i]);
	}
}

BOOL CXTPShortcutManager::GetShortcut(int ID, CXTPShortcutManagerAccel* pAccel)
{
	CXTPShortcutManagerAccelTable* pAccelTable = GetDefaultAccelerator();

	if (!pAccelTable)
		return FALSE;

	for (int i = 0; i < pAccelTable->GetCount(); i++)
	{
		CXTPShortcutManagerAccel* accel = pAccelTable->GetAt(i);
		if (accel->cmd == ID)
		{
			*pAccel = *accel;
			return TRUE;
		}
	}

	return FALSE;
}

void CXTPShortcutManager::AddShortcut(long cmd, LPCTSTR strKey)
{
	CXTPShortcutManagerAccel accel;
	if (!ParseShortcut(strKey, &accel) && m_nEnableCustomShortcutText)
	{
		if (!m_pCommandBars)
			return;

		for (int i = m_pCommandBars->GetCount() - 1; i >= 0; i--)
		{
			CXTPToolBar* pBar = m_pCommandBars->GetAt(i);
			
			if (pBar->IsVisible())
			{
				CXTPControl* pControl = pBar->GetControls()->FindControl(xtpControlError, cmd, FALSE, TRUE);

				if (pControl != NULL) 
					pControl->SetCustomShortcutText(strKey);
			}
		}

		return;
	}
	else if (!ParseShortcut(strKey, &accel))
		return;
	

	accel.cmd = cmd;

	m_pAccelTable->Add(accel);
}

void CXTPShortcutManager::AddShortcut(long fVirt, long key, long cmd)
{
	CXTPShortcutManagerAccel accel;
	accel.key[0].fVirt = (BYTE)(fVirt | FVIRTKEY);
	accel.key[0].key = (WORD)key;
	accel.cmd = cmd;

	int nAccelSize = m_pAccelTable->GetCount();

	for (int i = 0; i < nAccelSize; i++)
	{
		CXTPShortcutManagerAccel* accelOld = m_pAccelTable->GetAt(i);
		if (CKeyHelper::EqualAccels(&accel, accelOld))
		{
			accelOld->cmd = cmd;
			return;
		}
	}

	m_pAccelTable->Add(accel);
}

void CXTPShortcutManager::SerializeShortcuts(CArchive& ar)
{
	CXTPPropExchangeArchive px(ar);
	DoPropExchange(&px);
}

void CXTPShortcutManager::SaveShortcuts(LPCTSTR lpszProfileName)
{
	if (m_pOriginalAccelTable == NULL)
		return;

	XTP_SHORTCUTMANAGER_ACCEL* lpAccel = m_pAccelTable->CopyAccels();

	AfxGetApp()->WriteProfileBinary(lpszProfileName,  _T("Accelerators"), (LPBYTE)lpAccel, m_pAccelTable->GetCount() * sizeof(XTP_SHORTCUTMANAGER_ACCEL));
	AfxGetApp()->WriteProfileInt(lpszProfileName, _T("AcceleratorsVersion"), 1);

	delete[] lpAccel;
}

void CXTPShortcutManager::LoadShortcuts(LPCTSTR lpszProfileName)
{
	if (m_pOriginalAccelTable == NULL)
		return;

	int nVersion = AfxGetApp()->GetProfileInt(lpszProfileName, _T("AcceleratorsVersion"), 0);

	if (nVersion == 0)
	{
		UINT uiSize;
		LPACCEL lpAccel = 0;
		if (AfxGetApp()->GetProfileBinary(lpszProfileName,  _T("Accelerators"), (LPBYTE*) &lpAccel, &uiSize))
		{
			int nAccelSize = uiSize / sizeof(ACCEL);
			ASSERT (lpAccel != NULL);

			UpdateAcellTable(lpAccel, nAccelSize);

			delete[] lpAccel;
		}
	}
	else if (nVersion == 1)
	{
		UINT uiSize;
		XTP_SHORTCUTMANAGER_ACCEL* lpAccel = 0;

		if (AfxGetApp()->GetProfileBinary(lpszProfileName,  _T("Accelerators"), (LPBYTE*) &lpAccel, &uiSize))
		{
			int nAccelSize = uiSize / sizeof(XTP_SHORTCUTMANAGER_ACCEL);
			ASSERT (lpAccel != NULL);

			UpdateAcellTable(lpAccel, nAccelSize);

			delete[] lpAccel;
		}
	}
}

BOOL CXTPShortcutManager::ParseShortcutVirtKey(CString& strShortcutKey, int nAccel) const
{
	CString str = m_pKeyNameText->Translate(nAccel) + _T('+');

	int nIndex = strShortcutKey.Find(str);
	if (nIndex != -1)
	{
		strShortcutKey = strShortcutKey.Left(nIndex) + strShortcutKey.Mid(nIndex + str.GetLength());
		return TRUE;
	}
	return FALSE;
}

BOOL CXTPShortcutManager::ParseShortcut(CString strShortcutKey, BYTE& fVirt, WORD& key) const
{
	fVirt = FVIRTKEY;
	key = 0;

	if (ParseShortcutVirtKey(strShortcutKey, VK_CONTROL))
	{
		fVirt |= FCONTROL;
	}

	if (ParseShortcutVirtKey(strShortcutKey, VK_MENU))
	{
		fVirt |= FALT;
	}

	if (ParseShortcutVirtKey(strShortcutKey, VK_SHIFT))
	{
		fVirt |= FSHIFT;
	}

	key = m_pKeyNameText->Parse(strShortcutKey);
	if (key != 0)
		return TRUE;

	if (strShortcutKey.GetLength() > 3 && strShortcutKey[0] == _T('0') && strShortcutKey[1] == _T('x'))
	{
		SCANF_S(strShortcutKey, _T("0x%hx"), &key);
	}

	return key != 0;
}

BOOL CXTPShortcutManager::ParseShortcut(CString strShortcut, CXTPShortcutManagerAccel* accel)
{
	int nIndex = strShortcut.Find(_T(", "));
	if (nIndex == -1)
	{
		if (!ParseShortcut(strShortcut, accel->key[0].fVirt, accel->key[0].key))
			return FALSE;

		accel->key[1].fVirt = 0;
		accel->key[1].key = 0;
	}
	else
	{
		if (!ParseShortcut(strShortcut.Left(nIndex), accel->key[0].fVirt, accel->key[0].key))
			return FALSE;

		if (!ParseShortcut(strShortcut.Mid(nIndex + 2), accel->key[1].fVirt, accel->key[1].key))
			return FALSE;
	}


	return TRUE;
}

void CXTPShortcutManager::DoPropExchange(CXTPPropExchange* pPX)
{
	pPX->ExchangeSchemaSafe();

	if (pPX->IsStoring())
	{
		int nAccelSize = m_pAccelTable->GetCount();

		if (pPX->IsAllowBlobValues())
		{
			XTP_SHORTCUTMANAGER_ACCEL* lpAccel = m_pAccelTable->CopyAccels();

			pPX->WriteCount(nAccelSize);
			pPX->Write(_T("Data"), lpAccel, nAccelSize * sizeof(XTP_SHORTCUTMANAGER_ACCEL));

			delete[] lpAccel;
		}
		else
		{
			CXTPPropExchangeEnumeratorPtr pEnumerator(pPX->GetEnumerator(_T("Shortcut")));
			POSITION posEnum = pEnumerator->GetPosition(nAccelSize);

			for (int i = 0; i < nAccelSize; i++)
			{
				CXTPPropExchangeSection secItem(pEnumerator->GetNext(posEnum));

				CXTPShortcutManagerAccel* pAccel = m_pAccelTable->GetAt(i);

				CString strShortcut;

				CKeyHelper keyHelper(pAccel, this);
				keyHelper.m_bAllowLocaleKey = FALSE;

				keyHelper.Format(strShortcut);

				PX_Int(&secItem, _T("Id"), pAccel->cmd, 0);
				PX_String(&secItem, _T("Key"), strShortcut);
			}
		}
	}
	else
	{
		m_pAccelTable->RemoveAll();

		if (pPX->GetSchema() < _XTP_SCHEMA_1200)
		{
			if (pPX->IsAllowBlobValues())
			{
				int nAccelSize = (int)pPX->ReadCount();
				LPACCEL lpAccel = new ACCEL[nAccelSize];
				pPX->Read(_T("Data"), lpAccel, nAccelSize* sizeof(ACCEL));

				UpdateAcellTable(lpAccel, nAccelSize);
				delete[] lpAccel;
			}
			else
			{
				CArray<ACCEL, ACCEL&> accels;
				CXTPPropExchangeEnumeratorPtr pEnumerator(pPX->GetEnumerator(_T("Accel")));
				POSITION posEnum = pEnumerator->GetPosition(0);
				while (posEnum)
				{
					ACCEL accel;
					CXTPPropExchangeSection secItem(pEnumerator->GetNext(posEnum));
					PX_UShort(&secItem, _T("Id"), (USHORT&)(accel.cmd), 0);
					PX_Byte(&secItem, _T("virt"), (accel.fVirt), 0);
					PX_UShort(&secItem, _T("key"), (USHORT&)(accel.key), 0);
					accels.Add(accel);
				}
				UpdateAcellTable(accels.GetData(), (int)accels.GetSize());
			}

		}
		else
		{
			if (pPX->IsAllowBlobValues())
			{
				int nAccelSize = (int)pPX->ReadCount();

				XTP_SHORTCUTMANAGER_ACCEL* lpAccel = new XTP_SHORTCUTMANAGER_ACCEL[nAccelSize];
				pPX->Read(_T("Data"), lpAccel, nAccelSize * sizeof(XTP_SHORTCUTMANAGER_ACCEL));

				UpdateAcellTable(lpAccel, nAccelSize);
				delete[] lpAccel;
			}
			else
			{
				CXTPPropExchangeEnumeratorPtr pEnumerator(pPX->GetEnumerator(_T("Shortcut")));
				POSITION posEnum = pEnumerator->GetPosition(0);
				while (posEnum)
				{
					CXTPShortcutManagerAccel accel;
					CXTPPropExchangeSection secItem(pEnumerator->GetNext(posEnum));
					PX_Int(&secItem, _T("Id"), accel.cmd, 0);

					CString strShortcut;
					// TODO: Add Parse Here
					PX_String(&secItem, _T("Key"), strShortcut, _T(""));
					if (ParseShortcut(strShortcut, &accel))
					{
						m_pAccelTable->Add(accel);
					}
				}
			}
		}
	}
}

BOOL CXTPShortcutManager::OnBeforeAdd(CXTPShortcutManagerAccel* accel)
{
#ifdef _XTP_ACTIVEX
	_variant_t vtCanceled(false);

	FireEvent(2, EVENT_PARAM(VTS_I4 VTS_I4 VTS_I4 VTS_PVARIANT), (long)accel->key[0].fVirt, (long)accel->key[0].key, (long)accel->cmd, &vtCanceled);

	return (bool)vtCanceled;
#endif

	UNREFERENCED_PARAMETER(accel);
	return FALSE;
}

BOOL CXTPShortcutManager::OnBeforeRemove(CXTPShortcutManagerAccel* accel)
{
#ifdef _XTP_ACTIVEX
	_variant_t vtCanceled(false);

	FireEvent(3, EVENT_PARAM(VTS_I4 VTS_I4 VTS_I4 VTS_PVARIANT), (long)accel->key[0].fVirt, (long)accel->key[0].key, (long)accel->cmd, &vtCanceled);

	return (bool)vtCanceled;
#endif

	UNREFERENCED_PARAMETER(accel);
	return FALSE;
}

#ifdef _XTP_ACTIVEX


// {4BE5CC96-7369-4eab-9B25-1D5C52916D86}
static const GUID IID_IKeyBindings =
{ 0x4be5cc96, 0x7369, 0x4eab, { 0x9b, 0x25, 0x1d, 0x5c, 0x52, 0x91, 0x6d, 0x86 } };


BEGIN_INTERFACE_MAP(CXTPShortcutManager, CCmdTarget)
	INTERFACE_PART(CXTPShortcutManager, IID_IKeyBindings, Dispatch)
	INTERFACE_PART(CXTPShortcutManager, IID_IConnectionPointContainer, ConnPtContainer)
END_INTERFACE_MAP()


BEGIN_DISPATCH_MAP(CXTPShortcutManager, CCmdTarget)
	DISP_FUNCTION_ID(CXTPShortcutManager, "Add", dispidAdd, OleAdd, VT_EMPTY, VTS_I4 VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CXTPShortcutManager, "DeleteAll", 2, OleDeleteAll, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPShortcutManager, "SetKeyNameText", 3, SetKeyNameText, VT_EMPTY, VTS_I4 VTS_BSTR)
	DISP_PROPERTY_ID(CXTPShortcutManager, "UseSystemKeyNameText", 4, m_bUseSystemKeyNameText, VT_BOOL)
	DISP_FUNCTION_ID(CXTPShortcutManager, "Delete", 5, OleDelete, VT_EMPTY, VTS_I4)
	DISP_PROPERTY_ID(CXTPShortcutManager, "AllowDoubleKeyShortcuts", 6, m_bAllowDoubleKeyShortcuts, VT_BOOL)
	DISP_FUNCTION_ID(CXTPShortcutManager, "AddShortcut", 7, OleAddShortcut, VT_EMPTY, VTS_I4 VTS_BSTR)
	DISP_PROPERTY_EX_ID(CXTPShortcutManager, "Enabled", 8, OleGetEnabled, OleSetEnahbled, VT_BOOL)
	DISP_PROPERTY_ID(CXTPShortcutManager, "DisableOnCapture", 9, m_bDisableOnCapture, VT_BOOL)

	DISP_FUNCTION_ID(CXTPShortcutManager, "NewEnum", DISPID_NEWENUM, OleNewEnum, VT_DISPATCH, VTS_NONE)
	DISP_PROPERTY_EX_ID(CXTPShortcutManager, "Count", 10, OleGetItemCount, SetNotSupported, VT_I4)
	DISP_FUNCTION_ID(CXTPShortcutManager, "Shortcut", DISPID_VALUE, OleGetItem, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPShortcutManager, "FindShortcut", 11, OleFindShortcut, VT_DISPATCH, VTS_I4)

	DISP_PROPERTY_EX_ID(CXTPShortcutManager, "EnableCustomShortcutText", 12, OleGetEnableCustomShortcutText, OleSetEnableCustomShortcutText, VT_BOOL)
END_DISPATCH_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPShortcutManager, IID_IKeyBindings)

// {380A7B16-7BE8-42ec-870C-8849F5540F14}
static const GUID IID_IKeyBindingsEvents =
{ 0x380a7b16, 0x7be8, 0x42ec, { 0x87, 0xc, 0x88, 0x49, 0xf5, 0x54, 0xf, 0x14 } };


IMPLEMENT_CONNECTION_EX(CXTPShortcutManager, IID_IKeyBindingsEvents)

IMPLEMENT_ENUM_VARIANT(CXTPShortcutManager);

int CXTPShortcutManager::OleGetItemCount()
{
	return m_pAccelTable->GetCount();
}

LPDISPATCH CXTPShortcutManager::OleGetItem(long Index)
{
	return XTPGetDispatch(m_pAccelTable->GetAt(Index));
}

LPDISPATCH CXTPShortcutManager::OleFindShortcut(long nCmd)
{
	CXTPShortcutManagerAccelTable* pAccelTable = GetDefaultAccelerator();
	int nAccelSize = pAccelTable->GetCount();

	int nFirstPriorety;
	CXTPShortcutManagerAccel* pResult = NULL;


	for (int i = 0; i < nAccelSize; i++)
	{
		CXTPShortcutManagerAccel* accel = pAccelTable->GetAt(i);
		if (accel->cmd == (int)nCmd)
		{
			int nPriority = 0;
			CString str = Format(accel, &nPriority);


			if (pResult == NULL || (nFirstPriorety < nPriority))
			{
				pResult = accel;
				nFirstPriorety = nPriority;
			}
		}
	}

	return XTPGetDispatch(pResult);
}

BOOL CXTPShortcutManager::OleGetEnableCustomShortcutText()
{
	return m_nEnableCustomShortcutText == 0;
}

void CXTPShortcutManager::OleSetEnableCustomShortcutText(BOOL bEnabled)
{
	m_nEnableCustomShortcutText = bEnabled;
}

BOOL CXTPShortcutManager::OleGetEnabled()
{
	return m_nDisableShortcuts == 0;
}

void CXTPShortcutManager::OleSetEnahbled(BOOL bEnabled)
{
	if (bEnabled)
		m_nDisableShortcuts = 0;
	else
		m_nDisableShortcuts = 1;
}

void CXTPShortcutManager::OleAdd(long fVirt, long key, long cmd)
{
	AddShortcut(fVirt, key, cmd);
}

void CXTPShortcutManager::OleAddShortcut(long cmd, LPCTSTR lpszShortcut)
{
	AddShortcut(cmd, lpszShortcut);
}

void CXTPShortcutManager::OleDeleteAll()
{
	m_pAccelTable->RemoveAll();
}

void CXTPShortcutManager::OleDelete(long nID)
{
	CXTPShortcutManagerAccelTable* pAccelTable = GetDefaultAccelerator();

	for (int i = pAccelTable->GetCount() - 1; i >= 0; i--)
	{
		if (pAccelTable->GetAt(i)->cmd == nID)
		{
			pAccelTable->RemoveAt(i);
		}
	}
}

BOOL CXTPShortcutManager::OleBeforeTranslate(long cmd)
{
	_variant_t vtCanceled(false);

	FireEvent(1, EVENT_PARAM(VTS_I4 VTS_PVARIANT), cmd, &vtCanceled);

	return (bool)vtCanceled;
}

//////////////////////////////////////////////////////////////////////////


BEGIN_DISPATCH_MAP(CXTPShortcutManagerAccel, CCmdTarget)
	DISP_PROPERTY_ID(CXTPShortcutManagerAccel, "Command", 1, cmd, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPShortcutManagerAccel, "ShortcutText", 2, OleGetShortcutText, OleSetShortcutText, VT_BSTR)
END_DISPATCH_MAP()


// {5BE5CC96-7369-4eab-9B25-1D5C52916D86}
static const GUID IID_IKeyBinding =
{ 0x5be5cc96, 0x7369, 0x4eab, { 0x9b, 0x25, 0x1d, 0x5c, 0x52, 0x91, 0x6d, 0x86 } };


BEGIN_INTERFACE_MAP(CXTPShortcutManagerAccel, CCmdTarget)
	INTERFACE_PART(CXTPShortcutManagerAccel, IID_IKeyBinding, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPShortcutManagerAccel, IID_IKeyBinding)


BSTR CXTPShortcutManagerAccel::OleGetShortcutText()
{
	if (!m_pManager)
		return 0;

	CString strFormat = m_pManager->Format(this, NULL);

	return strFormat.AllocSysString();
}


void CXTPShortcutManagerAccel::OleSetShortcutText(LPCTSTR lpszShortcut)
{
	if (!m_pManager)
		return;

	CXTPShortcutManagerAccel accel;
	if (m_pManager->ParseShortcut(lpszShortcut, &accel))
	{
		key[0] = accel.key[0];
		key[1] = accel.key[1];
	}
}

#endif
