// XTPShellPidl.h : header file
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

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPSHELLPIDL_H__)
#define __XTPSHELLPIDL_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// ----------------------------------------------------------------------
// Summary:
//     XTP_LVITEMDATA structure is used to maintain information for
//     a particular list item in a CXTPShellListCtrl or CXTPShellListView
//     window.
// See Also:
//     CXTPShellListCtrl, CXTPShellListView
// ----------------------------------------------------------------------
struct XTP_LVITEMDATA
{
	ULONG           ulAttribs;  // Shell item attributes.
	LPITEMIDLIST    lpi;        // Pointer to an item ID list.
	LPSHELLFOLDER   lpsfParent; // Points to the parent shell folder item.
};

// ----------------------------------------------------------------------
// Summary:
//     XTP_TVITEMDATA structure is used to maintain information for
//     a particular tree item in a CXTPShellTreeCtrl or CXTPShellTreeView
//     window.
// See Also:
//     CXTPShellTreeCtrl, CXTPShellTreeView
// ----------------------------------------------------------------------
struct XTP_TVITEMDATA
{
	LPITEMIDLIST    lpi;        // Pointer to an item ID list.
	LPITEMIDLIST    lpifq;      // Pointer to an item ID list.
	LPSHELLFOLDER   lpsfParent; // Pointer to the parent shell folder item.
};

//===========================================================================
// Summary:
//     CXTPShellPidl is a stand alone base class. This class is used by the
//     shell tree and list controls to handle PIDL creation and management.
//===========================================================================
class _XTP_EXT_CLASS CXTPShellPidl
{
public:
	//=======================================================================
	// Summary:
	//     CXTPShellPidl::CShellMalloc is a helper class that is used by
	//     CXTPShellPidl to initialize a pointer to the Shell's IMalloc
	//     interface used for allocating memory blocks.
	//=======================================================================
	class _XTP_EXT_CLASS CShellMalloc
	{
	public:
		//-------------------------------------------------------------------
		// Summary:
		//     Constructs a CXTPShellPidl::CShellMalloc object and initializes
		//     a pointer to the Shell's IMalloc interface.
		//-------------------------------------------------------------------
		CShellMalloc()
		{
			m_lpMalloc = NULL;

			if (FAILED(::SHGetMalloc(&m_lpMalloc)))
			{
				m_lpMalloc = NULL;
			}
		}

		//-------------------------------------------------------------------
		// Summary:
		//     Destroys a CXTPShellPidl::CShellMalloc object, handles cleanup
		//     and deallocation
		//-------------------------------------------------------------------
		virtual ~CShellMalloc()
		{
			if (m_lpMalloc)
			{
				m_lpMalloc->Release();
			}
		}

		//-------------------------------------------------------------------
		// Summary:
		//     Call this member function to allocate a block of memory.
		// Parameters:
		//     nBytes - Size, in bytes, of the memory block to be allocated.
		// Returns:
		//     If successful, Alloc returns a pointer to the allocated memory
		//     block. NULL If insufficient memory is available, Alloc returns
		//     NULL.
		//-------------------------------------------------------------------
		void* Alloc (DWORD nBytes)
		{
			ASSERT(m_lpMalloc != NULL);
			return m_lpMalloc ? m_lpMalloc->Alloc(nBytes) : NULL;
		}

		//-------------------------------------------------------------------
		// Summary:
		//     Call this member function to free a block of previously
		//     allocated.
		// Parameters:
		//     lpMem - Pointer to the memory block to be freed.
		//-------------------------------------------------------------------
		void Free(void* lpMem)
		{
			ASSERT(m_lpMalloc != NULL);
			if (m_lpMalloc)
			{
				m_lpMalloc->Free(lpMem);
			}
		}

		//-------------------------------------------------------------------
		// Summary:
		//     Returns a pointer to Shell's IMalloc interface for this object.
		//-------------------------------------------------------------------
		operator LPMALLOC()
		{
			return m_lpMalloc;
		}

	protected:
		LPMALLOC m_lpMalloc; // Points to Shell's IMalloc interface for this object.
	};

	//=======================================================================
	// Summary:
	//     CXTPShellPidl::CShellSpecialFolder is a helper class that is used by
	//     CXTPShellPidl to initialize a pointer to an IDTEMIDLIST structure of
	//     a special folder.
	//=======================================================================
	class _XTP_EXT_CLASS CShellSpecialFolder
	{
	public:
		//-------------------------------------------------------------------
		// Summary:
		//     Constructs a CXTPShellPidl::CShellSpecialFolder object and
		//     initializes a pointer to the ITEMIDLIST structure of the
		//     special folder specified by nFolder.
		// Parameters:
		//     nFolder - value that identifies the folder of interest, see
		//               CSIDL in MSDN documentation for a list of constants
		//               that represent special folder locations.
		//-------------------------------------------------------------------
		CShellSpecialFolder(int nFolder = CSIDL_DESKTOP);

		//-------------------------------------------------------------------
		// Summary:
		//     Destroys a CXTPShellPidl::CShellSpecialFolder object, handles
		//     cleanup and deallocation
		//-------------------------------------------------------------------
		virtual ~CShellSpecialFolder()
		{
			if (m_lpFolder)
			{
				m_lpFolder->Release();
			}
		}

		//-------------------------------------------------------------------
		// Summary:
		//     Returns a pointer to interface for the desktop folder, which is
		//     the root of the Shell's namespace.
		//-------------------------------------------------------------------
		operator LPSHELLFOLDER()
		{
			return m_lpFolder;
		}

	protected:
		LPSHELLFOLDER m_lpFolder; // Points to the interface for the desktop folder.
	};

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPShellPidl object
	//-----------------------------------------------------------------------
	CXTPShellPidl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPShellPidl object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPShellPidl();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function gets the fully qualified PIDL for the path string.
	// Parameters:
	//     path - File system path string.
	// Returns:
	//     A pointer to an item ID list. Returns NULL if it fails.
	//-----------------------------------------------------------------------
	LPITEMIDLIST IDLFromPath(LPCTSTR path);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function performs the OneUp or back function.
	// Parameters:
	//     pidlPath - Fully qualified PIDL.
	// Returns:
	//     A fully qualified parent PIDL.
	//-----------------------------------------------------------------------
	LPITEMIDLIST OneUpPIDL(LPITEMIDLIST pidlPath);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function gets the parent folder using PIDLs.
	// Parameters:
	//     path - Path string.
	// Returns:
	//     A path string to the parent.
	//-----------------------------------------------------------------------
	CString OneUpPATH(const CString& path);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function walks an ITEMIDLIST and points to the last one.
	// Parameters:
	//     pidl - PIDL list.
	// Returns:
	//     A pointer to the last IDL in the list.
	//-----------------------------------------------------------------------
	LPITEMIDLIST GetLastITEM(LPITEMIDLIST pidl);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function copies a whole ITEMIDLIST. Remember to Free()
	//     the old one if it is no longer needed.
	// Parameters:
	//     pidl - Pointer to an ITEMIDLIST.
	// Returns:
	//     A new pointer to a copy of the PIDL.
	//-----------------------------------------------------------------------
	LPITEMIDLIST CopyIDList(LPITEMIDLIST pidl);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function concatenates two PIDLs.
	// Parameters:
	//     lpMalloc - Points to the shell's IMalloc interface.
	//     pidl1 - Pointer to an item ID list.
	//     pidl2 - Pointer to an item ID list.
	// Returns:
	//     A pointer to an item ID list.
	//-----------------------------------------------------------------------
	LPITEMIDLIST ConcatPidls(LPCITEMIDLIST pidl1, LPCITEMIDLIST pidl2);
	LPITEMIDLIST ConcatPidls(LPMALLOC lpMalloc, LPCITEMIDLIST pidl1, LPCITEMIDLIST pidl2); // <combine CXTPShellPidl::ConcatPidls@LPCITEMIDLIST@LPCITEMIDLIST>

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function gets the fully qualified PIDLs for the specified
	//     folder.
	// Parameters:
	//     lpsf - Pointer to the parent shell folder.
	//     lpi  - Pointer to the item ID that is relative to 'lpsf'.
	// Returns:
	//     A pointer to an item ID list.
	//-----------------------------------------------------------------------
	LPITEMIDLIST GetFullyQualPidl(LPSHELLFOLDER lpsf, LPITEMIDLIST lpi);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function copies the ITEMID.
	// Parameters:
	//     lpMalloc - Points to the shell's IMalloc interface.
	//     lpi      - Pointer to item ID that is to be copied.
	// Returns:
	//     A pointer to an item ID list.
	//-----------------------------------------------------------------------
	LPITEMIDLIST DuplicateItem(LPMALLOC lpMalloc, LPITEMIDLIST lpi);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function gets the friendly name for the folder or file.
	// Parameters:
	//     lpsf           - Pointer to the parent shell folder.
	//     lpi            - Pointer to the item ID that is relative to 'lpsf'.
	//     dwFlags        - Flags to determine which value to return. See SHGNO for more details.
	//     lpFriendlyName - Buffer to receive the friendly name of the folder.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL GetName(LPSHELLFOLDER lpsf, LPITEMIDLIST  lpi, DWORD dwFlags, CString& lpFriendlyName);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function allocates a PIDL.
	// Parameters:
	//     lpMalloc - Points to the shell's IMalloc interface.
	//     cbSize - Initial size of the PIDL.
	// Returns:
	//     A pointer to an item ID list.
	//-----------------------------------------------------------------------
	LPITEMIDLIST CreatePidl(LPMALLOC lpMalloc, UINT cbSize);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function computes the number of item IDs in an item ID
	//     list.
	// Parameters:
	//     pidl - Pointer to an item ID list.
	// Returns:
	//     The number of item IDs in the list.
	//-----------------------------------------------------------------------
	UINT GetPidlItemCount(LPCITEMIDLIST pidl);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function retrieves the next PIDL in the list.
	// Parameters:
	//     pidl - Pointer to an item ID list.
	// Returns:
	//     A pointer to the next PIDL item in the list.
	//-----------------------------------------------------------------------
	LPITEMIDLIST GetNextPidlItem(LPCITEMIDLIST pidl);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function displays a popup context menu if given a parent
	//     shell folder, relative item ID, and screen location.
	// Parameters:
	//     hwnd       - Context menu owner.
	//     lpsfParent - Pointer to the parent shell folder.
	//     lpi        - Pointer to the item ID that is relative to 'lpsfParent'.
	//     lppi       - Pointer to the item ID that is relative to 'lpsfParent'.
	//     nCount     - Number of PIDLs.
	//     lppt       - Screen location of where to popup the menu.
	// Returns:
	//     TRUE on success, or FALSE on failure.
	//-----------------------------------------------------------------------
	BOOL ShowContextMenu(HWND hwnd, LPSHELLFOLDER lpsfParent, LPITEMIDLIST lpi, LPPOINT lppt);
	BOOL ShowContextMenu(HWND hwnd, LPSHELLFOLDER lpsfParent, LPCITEMIDLIST* lppi, int nCount, LPPOINT lppt); // <combine CXTPShellPidl::ShowContextMenu@HWND@LPSHELLFOLDER@LPITEMIDLIST@LPPOINT>

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function gets the index for the current icon. Index is
	//     the index into the system image list.
	// Parameters:
	//     lpi    - Fully qualified item ID list for the current item.
	//     uFlags - Flags that specify the image list retrieve, can be a
	//              combination of the following values:<p/>
	//              * <b>SHGFI_SMALLICON</b> - Return the index of small icon
	//                   from the system image list.
	//              * <b>SHGFI_LARGEICON</b> - Return the index of large icon
	//                   from the system image list.
	//              * <b>SHGFI_OPENICON</b> - Return the index of selected icon
	//                   from the system image list.
	// Returns:
	//     An icon index for the current item.
	//-----------------------------------------------------------------------
	int GetItemIcon(LPITEMIDLIST lpi, UINT uFlags) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve a string that describes the
	//     type of file specified by lpi from the Windows Shell.
	// Parameters:
	//     uFlags - Flags that specify the image list retrieve, can be a
	//              combination of the following values:<p/>
	//              * <b>SHGFI_SMALLICON</b> - Return the handle to the system
	//                   image list that contains small icon images.
	//              * <b>SHGFI_LARGEICON</b> - return the handle to the system
	//                   image list that contains large icon images.
	// Returns:
	//     A handle to the system image list that contains the specified icon
	//     images.
	//-----------------------------------------------------------------------
	HIMAGELIST GetSystemImageList(UINT uFlags) const;

	// ------------------------------------------------------------------------
	// Summary:
	//     Retrieves the attributes of one or more file objects or
	//     subfolders.
	// Parameters:
	//     lpsf :    Pointer to the parent shell folder.
	//     lpi :     Fully qualified item ID list for the current item.
	//     uFlags :  Single ULONG value contains the attributes that the caller
	//               is requesting.
	// Returns:
	//     Requested attributes that are common to all of the specified
	//     \objects.
	// ------------------------------------------------------------------------
	ULONG GetAttributes(LPSHELLFOLDER lpsf, LPITEMIDLIST lpi, ULONG uFlags) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function retrieves the IContextMenu, IContextMenu2 or IContextMenu3
	//     interface.
	// Parameters:
	//     psfFolder - A pointer to a valid IShellFolder data type.
	//     localPidl - A pointer to a valid _ITEMIDLIST structure.
	//     nCount    - Number of items in the context menu.
	//     ppCM      - Long pointer to a CONTEXTMENU struct.
	//     pcmType   - A pointer to a valid int data type that represents the version number
	//     of the context menu.
	// Returns:
	//     An HRESULT value.
	//-----------------------------------------------------------------------
	HRESULT GetSHContextMenu(LPSHELLFOLDER psfFolder, LPCITEMIDLIST* localPidl, int nCount, void** ppCM, int* pcmType);

	// ---------------------------------------------------------------------------
	// Summary:
	//     CXTPShellTreeCtrl callback sort function.
	// Parameters:
	//     lparam1 -     Corresponds to the lParam member of the TV_ITEM structure
	//                   for the two items being compared.
	//     lparam2 -     Corresponds to the lParam member of the TV_ITEM structure
	//                   for the two items being compared.
	//     lparamSort -  The 'lParamSort' member corresponds to the lParam member of
	//                   TV_SORTCB.
	// Remarks:
	//     This member function is a callback function used by
	//     CXTPShellTreeCtrl and CXTPShellTreeView, and is called during a sort
	//     operation each time the relative order of two list items needs to
	//     be compared.
	// Returns:
	//     A negative value if the first item should precede the second, a
	//     positive value if the first item should follow the second, or zero
	//     if the two items are equivalent.
	// ---------------------------------------------------------------------------
	static int CALLBACK TreeViewCompareProc(LPARAM lparam1, LPARAM lparam2, LPARAM lparamSort);

	// ---------------------------------------------------------------------------
	// Summary:
	//     CXTPShellListCtrl callback sort function.
	// Parameters:
	//     lparam1 -     Corresponds to the lParam member of the LV_ITEM structure
	//                   for the two items being compared.
	//     lparam2 -     Corresponds to the lParam member of the LV_ITEM structure
	//                   for the two items being compared.
	//     lparamSort -  The 'lParamSort' member corresponds to the lParam member of
	//                   LV_SORTCB.
	// Remarks:
	//     This member function is a callback function used by
	//     CXTPShellListCtrl and CXTPShellListView, and is called during a sort
	//     operation each time the relative order of two list items needs to
	//     be compared.
	// Returns:
	//     A negative value if the first item should precede the second, a
	//     positive value if the first item should follow the second, or zero
	//     if the two items are equivalent.
	// ---------------------------------------------------------------------------
	static int CALLBACK ListViewCompareProc(LPARAM lparam1, LPARAM lparam2, LPARAM lparamSort);

	// ---------------------------------------------------------------------------
	// Summary:
	//     Call this member function to override shortcut link display for each item.
	// Parameters:
	//     bShowLinkIcons - Set to FALSE to disable shortcut link display, TRUE to
	//                      enable.
	// ---------------------------------------------------------------------------
	void ShowShellLinkIcons(BOOL bShowLinkIcons = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Calculates number of item IDs in the list.
	// Parameters:
	//     pidl - Pointer to an item ID list.
	// Returns:
	//     The number of item IDs in the list.
	//-----------------------------------------------------------------------
	UINT GetPidlCount(LPCITEMIDLIST pidl);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function frees memory for allocated for a PIDL.
	// Parameters:
	//     pidl - Pointer to an item ID list.
	//-----------------------------------------------------------------------
	void FreePidl(LPITEMIDLIST pidl);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function copies a single PIDL out of a list. Remember to Free()
	//     the old one if it is no longer needed.
	// Parameters:
	//     pidl - Pointer to an ITEMIDLIST.
	//     nItem - PIDL in the list to copy.
	// Returns:
	//     A new pointer to a copy of the PIDL with the single item.
	//-----------------------------------------------------------------------
	LPITEMIDLIST CopyPidlItem(LPITEMIDLIST pidl, UINT nItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function compares two PIDLs to determine if they match.
	// Parameters:
	//     pidl1        - Pointer to an item ID list.
	//     pidl2        - Pointer to an item ID list.
	//     pShellFolder - Pointer to the shell folder to use for the comparison (default Desktop)
	// Returns:
	//     TRUE on success, or FALSE on failure.
	//-----------------------------------------------------------------------
	BOOL ComparePidls(LPCITEMIDLIST pidl1, LPCITEMIDLIST pidl2, LPSHELLFOLDER pShellFolder = NULL);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when item of context menu was executed
	// Parameters:
	//     idCmd - Item identifier was executed.
	//     cmi   - CMINVOKECOMMANDINFO structure reference contained additional information about item was executed.
	//-----------------------------------------------------------------------
	virtual void OnShowContextMenu(int idCmd, CMINVOKECOMMANDINFO& cmi);

	//-----------------------------------------------------------------------
	// Summary:
	//     This function translates shell attributes into a tree item state
	// Parameters:
	//     pTreeCtrl    - Tree Control that owns an item
	//     pListCtrl    - List Control that owns an item
	//     hItem        - Handle to identify the item, passed verbatim to SetItemState()
	//     iItem        - Index to identify the item, passed verbatim to SetItemState()
	//     dwAttributes - Shell attributes to translate
	//-----------------------------------------------------------------------
	virtual void MapShellFlagsToItemAttributes(CListCtrl* pListCtrl, int iItem, DWORD dwAttributes);
	virtual void MapShellFlagsToItemAttributes(CTreeCtrl* pTreeCtrl, HTREEITEM hItem, DWORD dwAttributes); // <combine CXTPShellPidl::MapShellFlagsToItemAttributes@CListCtrl*@int@DWORD>

protected:
	ULONG m_ulSFGAOFlags;        // SFGAO flags used to retrieve shell item attributes.
	BOOL  m_bShowShellLinkIcons; // Set to FALSE to override shortcut link display.

private:
	static WNDPROC          m_pOldWndProc; // regular window proc
	static LPCONTEXTMENU2   m_pIContext2;  // active shell context menu
	static LRESULT CALLBACK HookWndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
};

AFX_INLINE void CXTPShellPidl::ShowShellLinkIcons(BOOL bShowLinkIcons /*= TRUE*/) {
	m_bShowShellLinkIcons = bShowLinkIcons;
}

//{{AFX_CODEJOCK_PRIVATE
#ifndef CSIDL_MYPICTURES
#define CSIDL_MYPICTURES        0x0027
#endif

#ifndef SFGAO_STREAM
#define SFGAO_STREAM            0x00400000L
#endif

#ifndef SFGAO_ENCRYPTED
#define SFGAO_ENCRYPTED         0x00002000L
#endif

#ifndef SHCIDS_ALLFIELDS
#define SHCIDS_ALLFIELDS        0x80000000L
#endif
//}}AFX_CODEJOCK_PRIVATE

#endif // __XTPSHELLPIDL_H__
