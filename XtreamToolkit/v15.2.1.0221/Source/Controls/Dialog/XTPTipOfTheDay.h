// XTPTipOfTheDay.h interface for the CXTPTipOfTheDay class.
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

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPTIPOFTHEDAY_H__)
#define __XTPTIPOFTHEDAY_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPMarkupContext;
class CXTPMarkupUIElement;
class CXTPImageManagerIcon;

//===========================================================================
// Summary:
//     CXTPTipOfTheDay is a CDialog derived class.
//     CXTPTipOfTheDay is used to create a Visual Studio(tm) style
//     Tip Of The Day dialog. To use, place a file called "tips.txt" in the
//     same directory as your application exe. Place each tip on its own line.
//===========================================================================
class _XTP_EXT_CLASS CXTPTipOfTheDay : public CDialog
{
	DECLARE_DYNAMIC(CXTPTipOfTheDay)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTipOfTheDay object
	// Parameters:
	//     lpszTipFile - A NULL terminated string that represents the path and file name
	//                   of where the tips text file is located. By default, the file name
	//                   is set to "tips.txt".
	//     pParent     - Points to the parent window for the Tip Of The Day Dialog.
	//-----------------------------------------------------------------------
	CXTPTipOfTheDay(LPCTSTR lpszTipFile = NULL, CWnd* pParent = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPTipOfTheDay object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPTipOfTheDay();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPTipOfTheDay object, handles cleanup and deallocation
	//-----------------------------------------------------------------------

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will set the fonts to be used by the "Did you
	//     know..." and the Tip Of The Day text.
	// Parameters:
	//     pFontTitle - Points to a CFont object that represents the new
	//                  font to be used for the "Did you know..." text.
	//     pFontTip   - Points to a CFont object that represents the new
	//                  font to be used for the Tip Of The Day text.
	//-----------------------------------------------------------------------
	virtual void SetDefaultFonts(CFont* pFontTitle, CFont* pFontTip);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will retrieve the next string to be displayed
	//     as the Tip Of The Day.
	// Parameters:
	//     strNext - A CString reference that represents the next
	//     Tip Of The Day text that is to be displayed.
	//-----------------------------------------------------------------------
	virtual void GetNextTipString(CString& strNext);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will set the text that is to be displayed
	//     in place of the "Did you know..." string.
	// Parameters:
	//     lpszTitle - Represents a NULL terminated string that is
	//                 the string to be displayed in place of the
	//                 "Did you know..." text.
	//-----------------------------------------------------------------------
	virtual void SetDefaultTitle(LPCTSTR lpszTitle);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will set the path to where the tips file is
	//     located.
	// Parameters:
	//     lpszTipFile - A NULL terminated string that represents the full
	//                   path to where the tips text file is located.
	//-----------------------------------------------------------------------
	virtual void SetTipsFilePath(LPCTSTR lpszTipFile);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to add all tips manually.
	// Parameters:
	//     lpszTip - Next tooltip to add.
	//-----------------------------------------------------------------------
	void AddTip(LPCTSTR lpszTip);

	// -----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the tip text.
	// Parameters:
	//     lpszText :  NULL terminated string that represents the new tip text
	//                 to display.
	//
	// -----------------------------------------------------------------------
	void SetTipText(LPCTSTR lpszText);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to enable markup for tab captions
	// Parameters:
	//     bEnable - TRUE to enable markup
	//-------------------------------------------------------------------------
	void EnableMarkup(BOOL bEnable);

	//-----------------------------------------------------------------------
	// Summary: Call this method to get Markup element that renders the item.
	// Returns: Pointer to Markup element.
	//-----------------------------------------------------------------------
	CXTPMarkupContext* GetMarkupContext() const;

protected:

//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPTipOfTheDay)
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPTipOfTheDay)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDaytipNext();
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE


public:
//{{AFX_CODEJOCK_PRIVATE
	//{{AFX_DATA(CXTPTipOfTheDay)
	enum { IDD = XTP_IDD_TIPOFTHEDAY };
	CButton m_ok;
	CButton m_showTips;
	CButton m_btnNextTip;
	CStatic m_staticBorder;
	BOOL    m_bStartup;
	//}}AFX_DATA
//}}AFX_CODEJOCK_PRIVATE

	BOOL m_bClassicStyle;   // Classic Style

protected:
	FILE*   m_pStream;      // A pointer to the open file stream.
	CRect   m_rcBorder;     // Size of the total display area.
	CRect   m_rcTipText;    // Size of the display area for tip text.
	CFont   m_fontTitle;    // Default font used for "Did you know..." text.
	CFont   m_fontTip;      // Default font used for tips.
	CFont*  m_pFontTitle;   // User defined font for "Did you know..." text.
	CFont*  m_pFontTip;     // User defined font for tips.
	CString m_strTipTitle;  // Represents the "Did you know" text.
	CString m_strTipText;   // Represents the Tip Of The Day text.
	CString m_strTipFile;   // Represents the file name and path for the tips file.
	CXTPImageManagerIcon* m_pLightBulbIcon; // Bulb bitmap.

	CXTPMarkupContext* m_pMarkupContext;  // Markup Context.
	CXTPMarkupUIElement* m_pMarkupTipText;  // Markup element.

	CStringArray m_arrTips;  // Array of tips to display.
	UINT m_nNextTip;  // Index of next tip to display.
};

//////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTPTipOfTheDay::SetDefaultTitle(LPCTSTR lpszTitle) {
	m_strTipTitle = lpszTitle;
}
AFX_INLINE void CXTPTipOfTheDay::AddTip(LPCTSTR lpszTip) {
	m_arrTips.Add(lpszTip);
}
AFX_INLINE CXTPMarkupContext* CXTPTipOfTheDay::GetMarkupContext() const {
	return m_pMarkupContext;
}

#endif // #if !defined(__XTPTIPOFTHEDAY_H__)
