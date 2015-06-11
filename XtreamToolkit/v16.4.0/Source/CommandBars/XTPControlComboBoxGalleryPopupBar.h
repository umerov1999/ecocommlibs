// XTPControlComboBoxGalleryPopupBar.h
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

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPCONTROLCOMBOBOXGALLERYPOPUPBAR_H__)
#define __XTPCONTROLCOMBOBOXGALLERYPOPUPBAR_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//===========================================================================
// Summary: CXTPControlComboBoxPopupBar object
//===========================================================================
class _XTP_EXT_CLASS CXTPControlComboBoxGalleryPopupBar : public CXTPControlComboBoxPopupBar
{
	DECLARE_XTP_COMMANDBAR(CXTPControlComboBoxGalleryPopupBar)

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPControlComboBoxGalleryPopupBar object
	//-----------------------------------------------------------------------
	CXTPControlComboBoxGalleryPopupBar();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Creates a popup bar object.
	// Parameters:
	//     pCommandBars - Points to a CXTPCommandBars object
	// Returns:
	//     A pointer to a CXTPControlComboBoxGalleryPopupBar object
	//-----------------------------------------------------------------------
	static CXTPControlComboBoxGalleryPopupBar* AFX_CDECL CreateComboBoxGalleryPopupBar(CXTPCommandBars* pCommandBars);

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	BOOL SetTrackingMode(int bMode, BOOL bSelectFirst, BOOL bKeyboard);
	BOOL OnHookKeyDown(UINT nChar, LPARAM lParam);

	CXTPControlGallery* GetGalleryItem() const;

	//{{AFX_VIRUAL(CXTPControlComboBoxList)
	virtual int GetCurSel() const {
		return GetGalleryItem()->GetSelectedItem();
	}
	virtual int FindString(int nStartAfter, LPCTSTR lpszItem) const {
		return GetGalleryItem()->FindItem(nStartAfter, lpszItem, FALSE);
	}
	virtual int FindStringExact(int nIndexStart, LPCTSTR lpsz) const {
		return GetGalleryItem()->FindItem(nIndexStart, lpsz, TRUE);
	}
	virtual int SetTopIndex(int nIndex) {
		return GetGalleryItem()->SetTopIndex(nIndex);
	}
	virtual void SetCurSel(int nIndex) {
		GetGalleryItem()->SetSelectedItem(nIndex);
	}
	virtual void GetText(int nIndex, CString& rString) const {
		rString = GetGalleryItem()->GetItemCaption(nIndex);
	}

	virtual int GetCount( ) const
	{
		return GetGalleryItem()->GetItemCount();
	}

	//}}AFX_VIRUAL

	//{{AFX_MSG(CXTPControlComboBoxList)
	void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE
};



#endif // #if !defined(__XTPCONTROLCOMBOBOXGALLERYPOPUPBAR_H__)
