// XTPScrollBarContainer.h: CXTPScrollBarContainer class.
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
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
#if !defined(__XTPSCROLLBARCONTAINER_H__)
#define __XTPSCROLLBARCONTAINER_H__
//}}AFX_CODEJOCK_PRIVATE

class CXTPScrollBarCtrl;


//{{AFX_CODEJOCK_PRIVATE
class _XTP_EXT_CLASS CXTPScrollBarContainerImpl {
public:
	CXTPScrollBarContainerImpl();
	~CXTPScrollBarContainerImpl();

	void SetContainer(CWnd* pContainer);

	BOOL CreateScrollBarCtrl(DWORD dwStyle, UINT nID);
	void DestroyScrollBarCtrl(UINT nID);

	CScrollBar* GetScrollBarCtrl(int nBar);
	void SetScrollBarTheme(XTPScrollBarTheme theme);
	void ShowScrollBar(UINT nBar, BOOL bShow);
	BOOL EnableScrollBar(int nSBFlags, UINT nArrowFlags = ESB_ENABLE_BOTH);

	void RefreshMetrics();
	void OnWindowProc(UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lResult);

private:
	struct STDSCROLLBAR {
		STDSCROLLBAR();
		~STDSCROLLBAR();

		CXTPScrollBarCtrl* operator -> () throw();
		const CXTPScrollBarCtrl* operator -> () const throw();

		CXTPScrollBarCtrl *pCtrl;
		BOOL bVisible;
	};

	enum StdScrollBarId
	{
		SbInvalid = -1,
		SbHorz = 0,
		SbVert = 1,
		SbGripper = 2
	};

	void OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnWindowPosChange(WINDOWPOS* lpWndPos);
	void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS *lpncsp);
	void OnStyleChanged(LONG nType, LPSTYLESTRUCT pStyleStruct);

	BOOL HasStdBar(StdScrollBarId nBar) const;
	CScrollBar* CreateStdScrollBar(StdScrollBarId nBar);
	void DestroyStdScrollBar(StdScrollBarId nBar);
	void RepositionStdScrollBars(const RECT* lpRect = NULL);
	static StdScrollBarId SBtoSBId(int nBar);
	static UINT SBIdToSBWs(StdScrollBarId nBar);
	static int SBIdToSB(StdScrollBarId nBar);
	void ShowScrollBarInternal(StdScrollBarId nBar, BOOL bShow);
	void SyncStdScrollBarStates();

	BOOL m_bCreated;
	STDSCROLLBAR m_StdScrollBar[3];
	CWnd* m_pContainer;
	XTPScrollBarTheme m_nTheme;
	CList<CXTPScrollBarCtrl*, CXTPScrollBarCtrl*> m_ScrollBars;
	BOOL m_bRightFrameAdjusted;
	BOOL m_bBottomFrameAdjusted;
};
//}}AFX_CODEJOCK_PRIVATE

//-----------------------------------------------------------------------
// Summary:
//     CXTPScrollBarContainer class adds XTP scroll bars to a class
///    derived from CWnd.
//-----------------------------------------------------------------------
template<class Parent = CWnd>
class CXTPScrollBarContainer : public Parent
{
protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPScrollBarContainer object.
	//     Forces the class to be inherited only, not instantiated.
	//-----------------------------------------------------------------------
	CXTPScrollBarContainer()
	{
		m_Impl.SetContainer(this);
	}

public:
	//{{AFX_VIRTUAL(CXTPScrollBarContainer)

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lResult = Parent::WindowProc(message, wParam, lParam);
		m_Impl.OnWindowProc(message, wParam, lParam, lResult);
		return lResult;
	}

	//}}AFX_VIRTUAL

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets scroll bar theme.
	// Parameters:
	//     theme - scroll bar theme identifier.
	//-----------------------------------------------------------------------
	void SetScrollBarTheme(XTPScrollBarTheme theme)
	{
		m_Impl.SetScrollBarTheme(theme);
		RefreshScrollBarMetrics();
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates a scroll bar control with ID and style specied.
	// Parameters:
	//     dwStyle - scroll bar style.
	//     nID     - scroll bar control ID.
	// Returns:
	//     TRUE if succeeded.
	//-----------------------------------------------------------------------
	BOOL CreateScrollBarCtrl(DWORD dwStyle, UINT nID)
	{
		return m_Impl.CreateScrollBarCtrl(dwStyle, nID);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to obtain a pointer to the specified
	//     sibling scroll bar or splitter window.
	// Parameters:
	//     nBar - Specifies the type of scroll bar.
	//-----------------------------------------------------------------------
	virtual CScrollBar* GetScrollBarCtrl(int nBar) const
	{
		return const_cast<CXTPScrollBarContainerImpl&>(m_Impl).GetScrollBarCtrl(nBar);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Shows or hides a scroll bar.
	// Parameters:
	//     nBar - Specifies the type of scroll bar.
	//     bShow - Specifies whether Windows shows or hides the scroll bar.
	//-----------------------------------------------------------------------
	void ShowScrollBar(UINT nBar, BOOL bShow = TRUE)
	{
		m_Impl.ShowScrollBar(nBar, bShow);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables or disables one or both arrows of a scroll bar.
	// Parameters:
	//     nSBFlags - Specifies the scroll-bar type.
	//     nArrowFlags - Specifies whether the scroll-bar arrows are enabled
	//                   or disabled and which arrows are enabled or disabled.
	//-----------------------------------------------------------------------
	BOOL EnableScrollBar(int nSBFlags, UINT nArrowFlags = ESB_ENABLE_BOTH)
	{
		return m_Impl.EnableScrollBar(nSBFlags, nArrowFlags);
	}

	// -----------------------------------------------------------------
	// Summary:
	//     Triggers update of color and size information for themed
	//     scroll bars.
	// -----------------------------------------------------------------
	void RefreshScrollBarMetrics()
	{
		m_Impl.RefreshMetrics();
	}

private:
	CXTPScrollBarContainerImpl m_Impl;
};


//-----------------------------------------------------------------------
// Summary:
//     CXTPDialogScrollBarContainer class adds XTP scroll bars to a class
///    derived from CDialog. The parent class must have CDialog constructors.
//-----------------------------------------------------------------------
template<class Parent>
class CXTPDialogScrollBarContainer : public CXTPScrollBarContainer<Parent>
{
protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPDialogScrollBarContainer object.
	//-----------------------------------------------------------------------
	CXTPDialogScrollBarContainer()
	{
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPDialogScrollBarContainer object.
	// Parameters:
	//     lpszTemplateName - Contains a null-terminated string that is the
	//                        name of a dialog-box template resource.
	//     pParentWnd       - Contains the ID number of a dialog-box template
	//                        resource.
	//-----------------------------------------------------------------------
	explicit CXTPDialogScrollBarContainer(
		LPCTSTR lpszTemplateName,
		CWnd* pParentWnd = NULL)
	{
		new (static_cast<Parent*>(this)) Parent(lpszTemplateName, pParentWnd);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPDialogScrollBarContainer object.
	// Parameters:
	//     lpszTemplateName - Contains a null-terminated string that is the
	//                        name of a dialog-box template resource.
	//     pParentWnd       - Points to the parent or owner window object
	//                        (of type CWnd) to which the dialog object belongs.
	//                        If it is NULL, the dialog object's parent window is set
	//                        to the main application window.
	//-----------------------------------------------------------------------
	explicit CXTPDialogScrollBarContainer(
		UINT nIDTemplate,
		CWnd* pParentWnd = NULL)
	{
		new (static_cast<Parent*>(this)) Parent(nIDTemplate, pParentWnd);
	}
};

#endif // !defined(__XTPSCROLLBARCONTAINER_H__
