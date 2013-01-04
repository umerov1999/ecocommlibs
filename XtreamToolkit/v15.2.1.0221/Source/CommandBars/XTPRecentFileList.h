// XTPRecentFileList.h: interface for the CXTPRecentFileList class.
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
#if !defined(__XTPRECENTFILELIST_H__)
#define __XTPRECENTFILELIST_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPRecentFileList;

class _XTP_EXT_CLASS CXTPRecentFileListItem : public CXTPCmdTarget
{
public:
	CXTPRecentFileListItem(CXTPRecentFileList* pList);

public:
	BOOL IsPinned() const;
	void SetPinned(BOOL bPinned);

	int GetIconId() const;
	void SetIconId(int nIconId);

	void SetCaption(LPCTSTR lpszCaption);
	CString GetCaption() const;

	void SetTag(LPCTSTR lpszTag);
	CString GetTag() const;

	CString GetPathName() const;

	CXTPRecentFileList* GetList() const;



protected:
	BOOL m_bPinned;
	int m_nIconId;

	CString m_strPathName;
	CString m_strCaption;
	CString m_strTag;
	CXTPRecentFileList* m_pList;

	friend class CXTPRecentFileList;
};


//===========================================================================
// Summary:
//     CXTPRecentFileList is CRecentFileList derived class.  This is used as a
//     pinnable MRU for the ribbon.
//===========================================================================
class _XTP_EXT_CLASS CXTPRecentFileList : public CXTPCmdTarget, public CRecentFileList
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPinableRecentFileList object
	// Parameters:
	//     nStart - Index of item to start pinned
	//     lpszSection - Section
	//     lpszEntryFormat - Format
	//     nSize - Initial list size
	//     nMaxDispLen - Maximum list size
	//-----------------------------------------------------------------------
	CXTPRecentFileList(UINT nStart, LPCTSTR lpszSection, LPCTSTR lpszEntryFormat, int nSize, int nMaxDispLen = AFX_ABBREV_FILENAME_LEN);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPinableRecentFileList object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	~CXTPRecentFileList();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Removes an item from the MRU.
	// Parameters:
	//     nIndex - Index of the item to remove from the MRU.
	//-----------------------------------------------------------------------
	virtual void Remove(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds an item to the MRU.
	// Parameters:
	//     lpszPathName - Path of the filename, including file name, to include
	//     in the MRU.  Only the filename will be drawn in the MRU.  The entire
	//     path will be displayed in a tooltip on mouseover.
	//-----------------------------------------------------------------------
	virtual void Add(LPCTSTR lpszPathName);

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads from registry or ini file.
	//-----------------------------------------------------------------------
	virtual void ReadList();

	//-----------------------------------------------------------------------
	// Summary:
	//     Writes to registry or ini file.
	//-----------------------------------------------------------------------
	virtual void WriteList();

	CXTPRecentFileListItem* GetItem(int nIndex) const;
	int GetCount() const;

protected:

	virtual void OnNewItem(CXTPRecentFileListItem* pItem);
	CXTPRecentFileListItem* AddItem(LPCTSTR lpszPathName);


public:
	BOOL m_bPinable;        // True if the MRU is a pinnable MRU (for ribbon).

protected:
	CXTPRecentFileListItem** m_pItems;
};

AFX_INLINE BOOL CXTPRecentFileListItem::IsPinned() const {
	return m_bPinned;
}
AFX_INLINE void CXTPRecentFileListItem::SetPinned(BOOL bPinned) {
	m_bPinned = bPinned;
}
AFX_INLINE int CXTPRecentFileListItem::GetIconId() const {
	return m_nIconId;
}
AFX_INLINE void CXTPRecentFileListItem::SetIconId(int nIconId) {
	m_nIconId = nIconId;
}
AFX_INLINE void CXTPRecentFileListItem::SetCaption(LPCTSTR lpszCaption) {
	m_strCaption = lpszCaption;
}
AFX_INLINE CString CXTPRecentFileListItem::GetCaption() const {
	return m_strCaption;
}
AFX_INLINE void CXTPRecentFileListItem::SetTag(LPCTSTR lpszTag) {
	m_strTag = lpszTag;
}
AFX_INLINE CString CXTPRecentFileListItem::GetTag() const {
	return m_strTag;
}
AFX_INLINE CString CXTPRecentFileListItem::GetPathName() const {
	return m_strPathName;
}
AFX_INLINE CXTPRecentFileList* CXTPRecentFileListItem::GetList() const {
	return m_pList;
}


#endif // !defined(__XTPRECENTFILELIST_H__)
