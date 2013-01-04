// XTPShortcutManager.h : interface for the CXTPShortcutManager class.
//
// This file is a part of the XTREME COMMANDBARS MFC class library.
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

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPSHORTCUTMANAGER_H__)
#define __XTPSHORTCUTMANAGER_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPPropExchange;
class CXTPCommandBars;
class CXTPShortcutManager;

struct XTP_SHORTCUTMANAGER_ACCEL
{
	struct SHORTCUTACCEL
	{
		BYTE   fVirt;       // Specifies the accelerator behavior
		WORD   key;         // Specifies the accelerator key.
	};

	int cmd;                // Specifies the accelerator identifier
	SHORTCUTACCEL key[2];   // Double key combination
};

//===========================================================================
// Summary: CXTPShortcutManagerAccel represents accelerator key used in an accelerator table.
//===========================================================================
class _XTP_EXT_CLASS CXTPShortcutManagerAccel : public CXTPCmdTarget, public XTP_SHORTCUTMANAGER_ACCEL
{
public:

	//-------------------------------------------------------------------------
	// Summary: Constructs CXTPShortcutManagerAccel class
	//-------------------------------------------------------------------------
	CXTPShortcutManagerAccel();
	CXTPShortcutManagerAccel(ACCEL* pAccel);
	CXTPShortcutManagerAccel(const XTP_SHORTCUTMANAGER_ACCEL& accel);
	CXTPShortcutManagerAccel(const CXTPShortcutManagerAccel& accel);

public:
	const CXTPShortcutManagerAccel& operator=(const CXTPShortcutManagerAccel& accel);



public:
	CXTPShortcutManager* m_pManager;
};


//===========================================================================
// Summary: CXTPShortcutManagerAccelTable class is Accelerator table for CXTPShortcutManager class
//===========================================================================
class _XTP_EXT_CLASS CXTPShortcutManagerAccelTable
{
public:

	//-------------------------------------------------------------------------
	// Summary: Constructs CXTPShortcutManagerAccelTable
	//-------------------------------------------------------------------------
	CXTPShortcutManagerAccelTable(CXTPShortcutManager* pManager);

	//-------------------------------------------------------------------------
	// Destroys a CXTPShortcutManagerAccelTable object, handles cleanup and deallocation
	//-------------------------------------------------------------------------
	~CXTPShortcutManagerAccelTable();

public:

	//-----------------------------------------------------------------------
	// Summary: Determines count of accels in table
	// Returns: Number of accels in table
	//-----------------------------------------------------------------------
	int GetCount() const;

	//-----------------------------------------------------------------------
	// Summary: Retrieves accel by its index
	// Input:   nIndex - Index of accel to retrieve
	// Returns: Pointer to CXTPShortcutManagerAccel class
	//-----------------------------------------------------------------------
	CXTPShortcutManagerAccel* GetAt(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary: Adds new accelerator in table
	// Input:   accel - Point to accel to add
	//-----------------------------------------------------------------------
	void Add(const XTP_SHORTCUTMANAGER_ACCEL& accel);

	//-----------------------------------------------------------------------
	// Summary: Call this method to create array of all elements in table.
	// Returns: POinter to CXTPShortcutManagerAccel array
	//-----------------------------------------------------------------------
	XTP_SHORTCUTMANAGER_ACCEL* CopyAccels() const;

public:

	//-----------------------------------------------------------------------
	// Summary: Call this method to copy accels
	// Input:   hAccelTable - Windows HACCEL structure
	//          pAccelTable - Another accel table to copy from
	//          lpAccel - Array of ACCEL structure
	//          nSize - Count of elements in lpAccel.
	//-----------------------------------------------------------------------
	void CopyAccelTable(HACCEL hAccelTable);
	void CopyAccelTable(CXTPShortcutManagerAccelTable* pAccelTable);
	void CopyAccelTable(LPACCEL lpAccel, int nSize);

public:

	//-----------------------------------------------------------------------
	// Summary: Removes all elements in table
	//-----------------------------------------------------------------------
	void RemoveAll();

	//-----------------------------------------------------------------------
	// Summary: Call this method to remove accel by its index
	// Input:   nIndex - Index of accel to remove.
	//-----------------------------------------------------------------------
	void RemoveAt(int nIndex);

protected:
	CArray<CXTPShortcutManagerAccel*, CXTPShortcutManagerAccel*> m_arrAccels;           // Array of accels.
	CXTPShortcutManager* m_pManager; // Parent manager;
};

//===========================================================================
// Summary:
//     CXTPShortcutManager is standalone class used to manipulate accelerators of frame.
//===========================================================================
class _XTP_EXT_CLASS CXTPShortcutManager: public CXTPCmdTarget
{
public:

//{{AFX_CODEJOCK_PRIVATE
	class _XTP_EXT_CLASS CKeyNameText
	{
	public:
		CKeyNameText();
		CString Translate(UINT nKey);
		void SetAt(UINT uiVirtKey, LPCTSTR lpszKeyNameText);
		WORD Parse(LPCTSTR lpszKey);

	protected:
		CMap<WORD, WORD, CString, CString> m_mapVirtualKeys;
	};

	class _XTP_EXT_CLASS CKeyHelper
	{
	public:

		//-----------------------------------------------------------------------
		// Summary:
		//     Constructs a CKeyHelper object.
		//-----------------------------------------------------------------------
		CKeyHelper(const CXTPShortcutManagerAccel* lpAccel, CXTPShortcutManager* pManager = NULL);

		//-------------------------------------------------------------------------
		// Summary:
		//     Destroys a CKeyHelper object, handles cleanup and deallocation
		//-------------------------------------------------------------------------
		virtual ~CKeyHelper();

	public:

		void Format (CString& str) const;
		CString Format(const CXTPShortcutManagerAccel::SHORTCUTACCEL* pAccel) const;
		int Priority();

		static BOOL AFX_CDECL EqualAccels(const CXTPShortcutManagerAccel* pFirst, const CXTPShortcutManagerAccel* pSecond)
		{
			return ((pFirst->key[0].fVirt | FNOINVERT) == (pSecond->key[0].fVirt | FNOINVERT)) && (pFirst->key[0].key == pSecond->key[0].key) &&
				((pFirst->key[1].fVirt | FNOINVERT) == (pSecond->key[1].fVirt | FNOINVERT)) && (pFirst->key[1].key == pSecond->key[1].key);
		}

		CXTPShortcutManager* GetShortcutManager() const
		{
			return m_pManager;
		}

		//-----------------------------------------------------------------------
		// Summary:
		//     The GetKeyNameText function retrieves a string that represents the name of a key.
		// Parameters:
		//     uiVirtKey - virtual-key of accelerator.
		//-----------------------------------------------------------------------
		static CString AFX_CDECL GetLocalKeyNameText(UINT uiVirtKey);

	protected:
		void AddVirtKeyStr (CString& str, UINT uiVirtKey, BOOL bLast = FALSE) const;

	protected:
		const CXTPShortcutManagerAccel* m_lpAccel;
		CXTPShortcutManager* m_pManager;
	public:
		BOOL m_bAllowLocaleKey;
	};

	class _XTP_EXT_CLASS CKeyAssign : public CEdit
	{
	public:

		//-------------------------------------------------------------------------
		// Summary:
		//     Constructs a CKeyAssign object.
		//-------------------------------------------------------------------------
		CKeyAssign(CXTPShortcutManager* pManager = NULL);

		//-------------------------------------------------------------------------
		// Summary:
		//     Destroys a CKeyAssign object, handles cleanup and deallocation
		//-------------------------------------------------------------------------
		virtual ~CKeyAssign();

	public:
		BOOL IsKeyDefined() const;
		CXTPShortcutManagerAccel* GetAccel();
		void SetAccel(CXTPShortcutManagerAccel* pAccel);
		void ResetKey();

	protected:
		void SetAccelFlag(int nKey, BYTE bFlag, BOOL bOn);
		virtual BOOL PreTranslateMessage(MSG* pMsg);

		BOOL TranslateDoubleKeyShortcutsMessage(MSG* pMsg);
		BOOL TranslateSingleKeyShortcutsMessage(MSG* pMsg);


	public:
		BOOL m_bExtendedOnly;
		BOOL m_bAllowDoubleKeyShortcuts;

	protected:
		int m_nKeyDefined;
		CXTPShortcutManagerAccel m_accel;
		CKeyHelper  m_keyHelper;

	};
//}}AFX_CODEJOCK_PRIVATE

	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPShortcutManager object.
	// Parameters:
	//     pCommandBars - Owner CommandBars object
	//-------------------------------------------------------------------------
	CXTPShortcutManager(CXTPCommandBars* pCommandBars);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPShortcutManager object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	~CXTPShortcutManager();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves shortcut text for specified command.
	// Parameters:
	//     nCmd         - Command which shortcut text need to retrieve
	//     strShortcut  - Shortcut text reference.
	// Returns:
	//     TRUE if shortcut found.
	//-----------------------------------------------------------------------
	virtual BOOL FindDefaultAccelerator (int nCmd, CString& strShortcut);

	//-----------------------------------------------------------------------
	// Summary: Find default shortcut fo command
	// Input:   nCmd - Command to retrieve accel for
	//          strShortcut - Reference to Shortcut for output
	// Returns: TRUE if command was found
	//-----------------------------------------------------------------------
	BOOL FindDefaultFrameAccelerator (int nCmd, CString& strShortcut);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to retrieve text presentation of ACCEL.
	//     You can override this method.
	// Parameters:
	//     lpAccel - Accelerator need to format.
	//     pPriority - Returns priority of accelerator to be used to determine what accelerator used as default.
	//-----------------------------------------------------------------------
	virtual CString Format(CXTPShortcutManagerAccel* lpAccel, int* pPriority);

	//-----------------------------------------------------------------------
	// Summary:
	//     The GetKeyNameText function retrieves a string that represents the name of a key. Override
	//     this member function to provide additional functionality.
	// Parameters:
	//     uiVirtKey - virtual-key of accelerator.
	//-----------------------------------------------------------------------
	virtual CString GetKeyNameText(UINT uiVirtKey);

	// ---------------------------------------------------------------------
	// Summary:
	//     The OnPreviewEditKey function called to determine if user defined
	//     shortcut is allowed.
	// Parameters:
	//     pAccel :  User pressed key.
	// Returns:
	//     TRUE if key is allowed
	// ---------------------------------------------------------------------
	virtual BOOL OnPreviewEditKey(CXTPShortcutManagerAccel* pAccel);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called before user add new shortcut
	// Parameters:
	//     pAccel - New shortcut to add
	// See Also: OnBeforeRemove
	//-----------------------------------------------------------------------
	virtual BOOL OnBeforeAdd(CXTPShortcutManagerAccel* pAccel);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called before user remove shortcut
	// Parameters:
	//     pAccel - Shortcut to be removed
	// See Also: OnBeforeAdd
	//-----------------------------------------------------------------------
	virtual BOOL OnBeforeRemove(CXTPShortcutManagerAccel* pAccel);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to localize shortcut key name text.
	// Parameters:
	//     uiVirtKey - virtual-key of accelerator.
	//     lpszKeyNameText - New text that will be associated with virtual-key
	// Example:
	//     <code>pCommandBars->GetShortcutManager()->SetKeyNameText(VK_CONTROL, _T("Strg"));</code>
	//-----------------------------------------------------------------------
	void SetKeyNameText(UINT uiVirtKey, LPCTSTR lpszKeyNameText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to use system localized shortcuts
	// Parameters:
	//     bSystemKey - TRUE to allow system to localize shortcuts instead of predefined shortcuts table.
	// Example:
	//     <code>pCommandBars->GetShortcutManager()->UseSystemKeyNameText(TRUE);</code>
	//-----------------------------------------------------------------------
	void UseSystemKeyNameText(BOOL bSystemKey);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to enable/disable shortcuts usage
	// Parameters:
	//     bDisable - TRUE to disable shortcuts
	//-----------------------------------------------------------------------
	void DisableShortcuts(BOOL bDisable);

	//-----------------------------------------------------------------------
	// Summary:
	//     The TranslateAccelerator function processes accelerator keys for menu commands.
	// Parameters:
	//     lpMsg - Pointer to an MSG structure that contains message information
	//-----------------------------------------------------------------------
	BOOL TranslateAccelerator(LPMSG lpMsg);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to save the accelerators to the registry
	//     or .INI file.
	// Parameters:
	//     lpszProfileName - Points to a null-terminated string that
	//                       specifies the name of a section in the
	//                       initialization file or a key in the Windows
	//                       registry where state information is stored.
	//-----------------------------------------------------------------------
	void SaveShortcuts(LPCTSTR lpszProfileName);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to retrieve accelerators from the registry
	//     or .INI file.
	// Parameters:
	//     lpszProfileName - Points to a null-terminated string that
	//                       specifies the name of a section in the
	//                       initialization file or a key in the Windows
	//                       registry where state information is stored.
	//-----------------------------------------------------------------------
	void LoadShortcuts(LPCTSTR lpszProfileName);

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads or writes this object from or to an archive.
	// Parameters:
	//     pPX    - A CXTPPropExchange object to serialize to or from.
	//----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);


	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to create original accelerators
	// Parameters:
	//     nIDResource - Resource identifier of Accelerator table.
	//----------------------------------------------------------------------
	void SetAccelerators(UINT nIDResource);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set accelerator table
	// Parameters:
	//     hAccelTable - New accelerators to be set.
	//----------------------------------------------------------------------
	void SetDefaultAccelerator(HACCEL hAccelTable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get accelerator table
	// Returns:
	//     Accelerator table handle
	//----------------------------------------------------------------------
	virtual CXTPShortcutManagerAccelTable* GetDefaultAccelerator() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get original accelerators
	// Parameters:
	//     Accelerator table handle
	//----------------------------------------------------------------------
	CXTPShortcutManagerAccelTable* GetOriginalAccelerator() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to reset shortcuts
	//----------------------------------------------------------------------
	virtual void Reset();

public:
//{{AFX_CODEJOCK_PRIVATE
	void CreateOriginalAccelTable();

	static TCHAR AFX_CDECL ToUpper(TCHAR vkTCHAR);
	static BOOL AFX_CDECL CompareAccelKey(TCHAR chAccel, UINT wParam);
	static int  AFX_CDECL FindAccelPos(LPCTSTR lpszString);

	void UpdateAcellTable(LPACCEL lpAccel, int nSize);
	void UpdateAcellTable(XTP_SHORTCUTMANAGER_ACCEL* lpAccel, int nSize);

	void SerializeShortcuts(CArchive& ar);
	void AddShortcut(long fVirt, long key, long cmd);
	void AddShortcut(long cmd, LPCTSTR lpszKey);
	BOOL GetShortcut(int ID, CXTPShortcutManagerAccel* pAccel);

	BOOL ParseShortcut(CString strShortcut, CXTPShortcutManagerAccel* accel);

private:
	BOOL ParseShortcut(CString strShortcutKey, BYTE& fVirt, WORD& key) const;
	BOOL ParseShortcutVirtKey(CString& strShortcutKey, int nAccel) const;
	BOOL IsAccelMessage(CXTPShortcutManagerAccel::SHORTCUTACCEL& accel, int nKeyState, LPMSG lpMsg) const;
	int GetAccelKeyState() const;


//}}AFX_CODEJOCK_PRIVATE


public:
	BOOL m_bAllowEscapeShortcut;        // TRUE to allow escape to use as shortcut
	BOOL m_bUseSystemKeyNameText;       // TRUE to use localized shortcuts
	BOOL m_bAllowDoubleKeyShortcuts;    // TRUE to allow double key shortcuts(ex: Ctrl+K, Ctrl+V)
	int m_nDisableShortcuts;            // Number of shortcuts that have been disabled.
	BOOL m_bDisableOnCapture;           // Specifies whether the commandbars will process keybindings while another control is capturing input.

protected:

	CXTPShortcutManagerAccelTable* m_pAccelTable;               // Accelerator table
	CXTPShortcutManagerAccelTable* m_pOriginalAccelTable;       // Original accelerator table.
	CXTPCommandBars* m_pCommandBars;    // Parent CommandBars pointer
	CKeyNameText* m_pKeyNameText;       // KeyNameText map.

	friend class CKeyHelper;
};

AFX_INLINE CXTPShortcutManagerAccelTable* CXTPShortcutManager::GetDefaultAccelerator() const {
	return m_pAccelTable;
}
AFX_INLINE CXTPShortcutManagerAccelTable* CXTPShortcutManager::GetOriginalAccelerator() const {
	return m_pOriginalAccelTable;
}
AFX_INLINE void CXTPShortcutManager::UseSystemKeyNameText(BOOL bSystemKey) {
	m_bUseSystemKeyNameText = bSystemKey;
}


#endif //#if !defined(__XTPSHORTCUTMANAGER_H__)
