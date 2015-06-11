// XTPSyntaxEditView.h : header file
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// (c)1998-2013 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME SYNTAX EDIT LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
/////////////////////////////////////////////////////////////////////////////

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPSYNTAXEDITSYNTAXEDITVIEW_H__)
#define __XTPSYNTAXEDITSYNTAXEDITVIEW_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPSyntaxEditFindReplaceDlg;

namespace XTPSyntaxEditLexAnalyser
{
	class CXTPSyntaxEditLexParser;
}

//===========================================================================
// Summary:
//     Describes printing options for the Calendar Control.
// Remarks:
//     Use this class when you want to programmatically customize printing
//     options for the Calendar control.
//
//     See each property description to examine which customization options
//     are available.
// See Also:
//     CXTPPrintOptions
//===========================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditViewPrintOptions : public CXTPPrintOptions
{
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DYNAMIC(CXTPSyntaxEditViewPrintOptions)
	//}}AFX_CODEJOCK_PRIVATE

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default constructor.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditViewPrintOptions();

protected:

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPSyntaxEditViewPrintOptions);
//}}AFX_CODEJOCK_PRIVATE
#endif
};

//===========================================================================
// Summary: This class represents a View portion of the Edit Control. It
//          extends functionality provided by CView class from MFC's document-view
//          model implementation. CXTPSyntaxEditView class works together with
//          CXTPSyntaxEditDoc and provides facilities for it to be displayed on
//          a screen or printed on a printer.
// See Also: CXTPSyntaxEditDoc
//===========================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditView : public CView
{
	//{{AFX_CODEJOCK_PRIVATE
	friend class CXTPSyntaxEditDoc;
	DECLARE_DYNCREATE(CXTPSyntaxEditView)
	//}}AFX_CODEJOCK_PRIVATE

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//      Protected object constructor. Used by dynamic creation.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditView();

	//-----------------------------------------------------------------------
	// Summary:
	//      Destroys a CXTPSyntaxEditView() object, handles cleanup and
	//          de-allocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPSyntaxEditView();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//      Set the font of the edit control pointer.
	// Parameters:
	//      pLogFont   - [in] The LOGFONT pointer to be set.
	//      bUpdateReg - [in] Set TRUE to save font in registry.
	//-----------------------------------------------------------------------
	void SetFontIndirect(LOGFONT *pLogFont, BOOL bUpdateReg=FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//      Enable/Disable the line numbering.
	// Parameters:
	//      bEnable : [in] The LOGFONT pointer to be set.
	//-----------------------------------------------------------------------
	void SetLineNumbers(BOOL bEnable);

	//-----------------------------------------------------------------------
	// Summary:
	//      Returns the if horizontal scrollbar is enabled.
	// Returns:
	//      TRUE if horizontal scrollbar is enabled, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL GetHorzScrollBar() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Returns the if vertical scrollbar is enabled.
	// Returns:
	//      TRUE if vertical scrollbar is enabled, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL GetVertScrollBar() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Enable/disable scroll bar.
	// Parameters:
	//      bHorz         - [in] TRUE if horizontal scroll bar need to be enabled/disabled.
	//      bVert         - [in] TRUE if vertical scroll bar need to be enabled/disabled.
	//      bUpdateReg    - [in] Set TRUE to save these options in registry.
	//      bRecalcLayout - [in] Set TRUE to recalculate layout.
	// Returns:
	//      TRUE if scrollbars were updated, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL SetScrollBars(BOOL bHorz, BOOL bVert, BOOL bUpdateReg=FALSE, BOOL bRecalcLayout=TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//      Enable/disables gutter.
	// Parameters:
	//      bEnable : [in] Pass TRUE/FALSE if gutter is to be enabled/disbaled.
	//-----------------------------------------------------------------------
	void SetSelMargin(BOOL bEnable);

	//-----------------------------------------------------------------------
	// Summary:
	//      Enable/disables auto indent.
	// Parameters:
	//      bEnable : [in] Pass TRUE/FALSE if auto indent is to be enabled/disbaled.
	//-----------------------------------------------------------------------
	void SetAutoIndent(BOOL bEnable);

	//-----------------------------------------------------------------------
	// Summary:
	//      Enable/disables syntax colorization.
	// Parameters:
	//      bEnable : [in] Pass TRUE/FALSE if syntax color is to be enabled/disbaled.
	//-----------------------------------------------------------------------
	void SetSyntaxColor(BOOL bEnable);

	//-----------------------------------------------------------------------
	// Summary:
	//      Updates all active sibling views.
	//-----------------------------------------------------------------------
	void UpdateAllViews();

	//-----------------------------------------------------------------------
	// Summary:
	//      Set the view ready for redraw.
	//-----------------------------------------------------------------------
	void SetDirty();

	//-----------------------------------------------------------------------
	// Summary:
	//      Returns the smart edit control pointer for this editor.
	// Returns:
	//      Edit control pointer.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditCtrl& GetEditCtrl();

	const CXTPSyntaxEditCtrl& GetEditCtrl() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Set external smart edit control for the view.
	// Parameters:
	//      pControl - A pointer to smart edit control object.
	// Remarks:
	//      Default edit control window is destroyed.
	//      InternalRelease will be called for external control object in the
	//      view object destructor.
	// See Also:
	//      GetEditCtrl
	//-----------------------------------------------------------------------
	virtual void SetEditCtrl(CXTPSyntaxEditCtrl* pControl);

	//-----------------------------------------------------------------------
	// Summary:
	//      Returns a buffer manager which maintains a buffer of a smart edit
	//      control.
	// Returns:
	//      Pointer to the CXTPSyntaxEditBufferManager object.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditBufferManager* GetEditBuffer() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Use this function to get associated configuration manager.
	// Returns:
	//      Pointer to the associated configuration manager.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditConfigurationManager* GetLexConfigurationManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Refreshes the whole document, recalculates the scrollbar.
	//-----------------------------------------------------------------------
	void Refresh();

	//-----------------------------------------------------------------------
	// Summary:
	//      Set the current top row.
	// Parameters:
	//      iRow : [in] Row to set as top row.
	// Returns:
	//      TRUE if successful, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL SetTopRow(int iRow);

	//-----------------------------------------------------------------------
	// Summary:
	//      Return the topmost row.
	// Returns:
	//      The top row integer identifier.
	//-----------------------------------------------------------------------
	int GetTopRow() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Updates the sibling views while in split mode.
	// Parameters:
	//      dwUpdate : [in] Specifies the view update flags.
	//      pSender  : [in] Pointer to a sender object (or NULL).
	//-----------------------------------------------------------------------
	void UpdateScrollPos(CView* pSender, DWORD dwUpdate = XTP_EDIT_UPDATE_ALL);

	//-----------------------------------------------------------------------
	// Summary:
	//      Returns Document.
	// Returns:
	//      Pointer to CXTPSyntaxEditDoc.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditDoc* GetDocument() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Returns the edit buffer manager pointer.
	// Returns:
	//      Buffer manager pointer.
	// See also:
	//      class CXTPSyntaxEditBufferManager
	//-----------------------------------------------------------------------
	CXTPSyntaxEditBufferManager * GetDataManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Returns pointer to the associated lexical parser.
	// Returns:
	//      Pointer to a XTPSyntaxEditLexAnalyser::CXTPSyntaxEditLexParser object.
	//-----------------------------------------------------------------------
	XTPSyntaxEditLexAnalyser::CXTPSyntaxEditLexParser* GetLexParser() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Returns pointer to the attached Find/Replace dialog object.
	// Returns:
	//      Pointer to a CXTPSyntaxEditFindReplaceDlg object.
	//-----------------------------------------------------------------------
	virtual CXTPSyntaxEditFindReplaceDlg* GetFindReplaceDlg();

	//-----------------------------------------------------------------------
	// Summary:
	//      Used to get document title when printing.
	// Returns:
	//      Returns document title string.
	//-----------------------------------------------------------------------
	virtual CString GetPageTitle();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to access print options for this view.
	// Returns:
	//     Pointer to an object describing print options of this view.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditViewPrintOptions* GetPrintOptions() const;

	//{{AFX_CODEJOCK_PRIVATE
public:
	virtual void OnInitialUpdate();
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual void OnDragLeave();
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);

protected:
	afx_msg void OnPaint();
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL PaginateTo(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual void OnDropFiles(HDROP hDropInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint=0L, CObject* pHint=NULL);
	//}}AFX_CODEJOCK_PRIVATE

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//      This function is a default handler for XTP_EDIT_NM_EDITCHANGING
	//      message which sent before editing applied.
	// Parameters:
	//      pNMHDR  - [in] Pointer to NMHDR structure.
	//      pResult - [out] Pointer to a LRESULT value. Set as TRUE to cancel editing
	//                or FALSE to allow editing.
	// Returns:
	//      TRUE is message handled, FALSE otherwise.
	// See Also: OnEditChanged, XTP_EDIT_NM_EDITCHANGING, XTP_EDIT_NM_EDITCHANGED
	//-----------------------------------------------------------------------
	virtual BOOL OnEditChanging(NMHDR* pNMHDR, LRESULT* pResult);

	//-----------------------------------------------------------------------
	// Summary:
	//      This function is a default handler for XTP_EDIT_NM_EDITCHANGED
	//       message which sent when editing applied.
	// Parameters:
	//      pNMHDR  - [in] Pointer to XTP_EDIT_NMHDR_EDITCHANGED structure.
	//      pResult - unused for this handler.
	// Returns:
	//      TRUE is message handled, FALSE otherwise.
	// See Also: XTP_EDIT_NM_EDITCHANGING, XTP_EDIT_NM_EDITCHANGED,
	//           XTP_EDIT_NMHDR_EDITCHANGED
	//-----------------------------------------------------------------------
	virtual BOOL OnEditChanged(NMHDR* pNMHDR, LRESULT* pResult);

	//-----------------------------------------------------------------------
	// Summary:
	//      This function is a default handler for XTP_EDIT_NM_SETDOCMODIFIED
	//      message which sent when document (edit control) modified state changed.
	// Parameters:
	//      pNMHDR  - [in] Pointer to XTP_EDIT_NMHDR_DOCMODIFIED structure.
	//      pResult - unused for this handler.
	// Returns:
	//      TRUE is message handled, FALSE otherwise.
	// See Also: XTP_EDIT_NM_SETDOCMODIFIED, XTP_EDIT_NMHDR_DOCMODIFIED
	//-----------------------------------------------------------------------
	virtual BOOL OnSetDocModified(NMHDR* pNMHDR, LRESULT* pResult);

	//-----------------------------------------------------------------------
	// Summary:
	//      This function is a default handler for XTP_EDIT_NM_DRAWBOOKMARK
	//      message which sent before draw bookmark.
	// Parameters:
	//      pNMHDR  - [in] Pointer to XTP_EDIT_SENMBOOKMARK structure.
	//      pResult - [out] Pointer to a LRESULT value. Set as TRUE to cancel
	//                default processing (bookmark drawing),
	//                or FALSE to allow default processing.
	// Returns:
	//      TRUE is message handled, FALSE otherwise.
	// See Also: XTP_EDIT_NM_DRAWBOOKMARK, XTP_EDIT_SENMBOOKMARK
	//-----------------------------------------------------------------------
	virtual BOOL OnDrawBookmark(NMHDR* pNMHDR, LRESULT* pResult);

	//-----------------------------------------------------------------------
	// Summary:
	//      This function is a default handler for XTP_EDIT_NM_ROWCOLCHANGED
	//      message which sent current row or col changed.
	// Parameters:
	//      pNMHDR  - [in] Pointer to XTP_EDIT_NMHDR_ROWCOLCHANGED  structure.
	//      pResult - unused for this handler.
	// Returns:
	//      TRUE is message handled, FALSE otherwise.
	// See Also: XTP_EDIT_NM_ROWCOLCHANGED, XTP_EDIT_NMHDR_ROWCOLCHANGED
	//-----------------------------------------------------------------------
	virtual BOOL OnRowColChanged(NMHDR* pNMHDR, LRESULT* pResult);

	//-----------------------------------------------------------------------
	// Summary:
	//      This function is a default handler for XTP_EDIT_NM_UPDATESCROLLPOS
	//      message which sent scroll bar position changed.
	// Parameters:
	//      pNMHDR  - [in] Pointer to XTP_EDIT_NM_UPDATESCROLLPOS structure.
	//      pResult - unused for this handler.
	// Returns:
	//      TRUE is message handled, FALSE otherwise.
	// See Also: XTP_EDIT_NM_UPDATESCROLLPOS
	//-----------------------------------------------------------------------
	virtual BOOL OnUpdateScrollPos(NMHDR* pNMHDR, LRESULT* pResult);

		//-----------------------------------------------------------------------
	// Summary:
	//      This function is a default handler for XTP_EDIT_NM_ENABLESCROLLBAR
	//      message which sent scroll bar state changed.
	// Parameters:
	//      pNMHDR  - [in] Pointer to XTP_EDIT_NMHDR_ENABLESCROLLBAR structure.
	//      pResult - unused for this handler.
	// Returns:
	//      TRUE is message handled, FALSE otherwise.
	// See Also: XTP_EDIT_NM_ENABLESCROLLBAR
	//-----------------------------------------------------------------------
	virtual BOOL OnEnableScrollBar(NMHDR* pNMHDR, LRESULT* pResult);

	//-----------------------------------------------------------------------
	// Summary:
	//      This function is a default handler for XTP_EDIT_NM_INSERTKEY
	//      message which sent when 'Ins' (VK_INSERT) key down (insert mode changed).
	// Parameters:
	//      pNMHDR  - [in] Pointer to NMHDR structure.
	//      pResult - unused for this handler.
	// Returns:
	//      TRUE is message handled, FALSE otherwise.
	// See Also: XTP_EDIT_NM_INSERTKEY
	//-----------------------------------------------------------------------
	virtual BOOL OnInsertKey(NMHDR* pNMHDR, LRESULT* pResult);

	//-----------------------------------------------------------------------
	// Summary:
	//      This function is a default handler for XTP_EDIT_NM_SELINIT
	//      message which sent when selection reseted.
	// Parameters:
	//      pNMHDR  - [in] Pointer to NMHDR structure.
	//      pResult - unused for this handler.
	// Returns:
	//      TRUE is message handled, FALSE otherwise.
	// See Also: XTP_EDIT_NM_SELINIT
	//-----------------------------------------------------------------------
	virtual BOOL OnSelInit(NMHDR* pNMHDR, LRESULT* pResult);

	//-----------------------------------------------------------------------
	// Summary:
	//      This function is a default handler for XTP_EDIT_NM_STARTOLEDRAG
	//      message which sent when OLE dragging started.
	// Parameters:
	//      pNMHDR  - [in] Pointer to NMHDR structure.
	//      pResult - unused for this handler.
	// Returns:
	//      TRUE is message handled, FALSE otherwise.
	// See Also: XTP_EDIT_NM_STARTOLEDRAG
	//-----------------------------------------------------------------------
	virtual BOOL OnStartOleDrag(NMHDR* pNMHDR, LRESULT* pResult);

	//-----------------------------------------------------------------------
	// Summary:
	//      This function is a default handler for XTP_EDIT_NM_MARGINCLICKED
	//      message which sent when left mouse button clicked on bookmarks area.
	// Parameters:
	//      pNMHDR  - [in] Pointer to XTP_EDIT_NMHDR_MARGINCLICKED structure.
	//      pResult - [out] Pointer to a LRESULT value. Set as TRUE to cancel
	//                default processing (breakpoint set/reset),
	//                or FALSE to allow default processing.
	// Returns:
	//      TRUE is message handled, FALSE otherwise.
	// See Also: XTP_EDIT_NM_MARGINCLICKED, XTP_EDIT_NMHDR_MARGINCLICKED
	//-----------------------------------------------------------------------
	virtual BOOL OnMarginClicked(NMHDR* pNMHDR, LRESULT* pResult);

	//-----------------------------------------------------------------------
	// Summary:
	//      This function is a default handler for XTP_EDIT_NM_PARSEEVENT
	//      message which reflect parser event.
	// Parameters:
	//      pNMHDR  - [in] Pointer to XTP_EDIT_NMHDR_PARSEEVENT structure.
	//      pResult - unused for this handler.
	// Returns:
	//      TRUE is message handled, FALSE otherwise.
	// See Also: XTP_EDIT_NM_PARSEEVENT, XTP_EDIT_NMHDR_PARSEEVENT
	//-----------------------------------------------------------------------
	virtual BOOL OnParseEvent(NMHDR* pNMHDR, LRESULT* pResult);

protected:

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	//-----------------------------------------------------------------------
	// Summary: Overwrite this method to customize the read-only file handling
	// Returns: TRUE if read-only file can be changed, FALSE otherwise
	//-----------------------------------------------------------------------
	virtual BOOL CanChangeReadonlyFile();

	//{{AFX_CODEJOCK_PRIVATE
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnEditDelete();
	afx_msg void OnUpdateEditDelete(CCmdUI* pCmdUI);
	afx_msg void OnEditSelectAll();
	afx_msg void OnUpdateEditSelectAll(CCmdUI* pCmdUI);
	afx_msg void OnEditFind();
	afx_msg void OnUpdateEditFind(CCmdUI* pCmdUI);
	afx_msg void OnEditReplace();
	afx_msg void OnUpdateEditReplace(CCmdUI* pCmdUI);
	afx_msg void OnEditRepeat();
	afx_msg void OnUpdateEditRepeat(CCmdUI* pCmdUI);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnFilePageSetup();
	afx_msg void OnUpdateKeyIndicator(CCmdUI* pCmdUI);

	virtual void _EditFindReplace(BOOL bReplaceDlg);

	DECLARE_MESSAGE_MAP()
	//}}AFX_CODEJOCK_PRIVATE

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//      Performs specific update for all sibling views.
	// Parameters:
	//      pNMHDR_EC :     [in] Pointer to the parameters header.
	//      bTextChanged:   [in] This parameters is TRUE if document text was changed,
	//                           FALSE otherwise.
	// Remarks:
	//      This method process events from parser, updates the editing results
	//      or selection for all sibling views. It is called when an user edits
	//      text or switches between views or parser is running.
	//-----------------------------------------------------------------------
	void UpdateSiblings(XTP_EDIT_NMHDR_EDITCHANGED* pNMHDR_EC = NULL, BOOL bTextChanged = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//      Called when a sibling view is needed to be updated.
	// Parameters:
	//      pSender : [in] The source to be updated from
	//      dwUpdate      : [in] Type of update. Allowed values are:
	//                      XTP_EDIT_UPDATE_HORZ, XTP_EDIT_UPDATE_VERT, XTP_EDIT_UPDATE_DIAG.
	// See also
	//  class CXTPSyntaxEditView
	//-----------------------------------------------------------------------
	void UpdateSiblingScrollPos(CXTPSyntaxEditView* pSender, DWORD dwUpdate);

	//-----------------------------------------------------------------------
	// Summary:
	//      Returns the view pointer of a certain pane of the splitter
	// Parameters:
	//      nRow : [in] Row of the splitter frame.
	//      nCol : [in] Column of the splitter frame.
	// Returns:
	//      Pointer to CXTPSyntaxEditView.
	// See also:
	//      class CXTPSyntaxEditView
	//-----------------------------------------------------------------------
	CXTPSyntaxEditView* GetSplitterView(int nRow, int nCol);

	//-----------------------------------------------------------------------
	// Summary:
	//      Starts OLE drag
	//-----------------------------------------------------------------------
	void StartOleDrag();

	LOGFONT m_lfPrevFont; // Temporarily stores editor font
	int m_nPrevTopRow;  // Temporarily stores previous top row

protected:
	int m_nParserThreadPriority_WhenActive;     // Parser priority (active state).
	int m_nParserThreadPriority_WhenInactive;   // Parser priority (inactive state).

	BOOL m_bDraggingOver;           // TRUE if dragover is going on, FALSE otherwise
	BOOL m_bDraggingStartedHere;    // TRUE if dragging is started in this view, FALSE otherwise

	BOOL m_bScrollBars; // Whether to create control with own scrollbars, or
	                    // let to manage scrolling for parent window

	//-----------------------------------------------------------------------

	static CXTPSyntaxEditView *ms_pTargetView;  // Target view pointer to be filled in by drag-drop routine
	static BOOL ms_bDroppedHere;        // TRUE if text is dropped in this view, FALSE otherwise
	static POINT ms_ptDropPos;          // Stores drop mouse position.
	static DWORD_PTR ms_dwSignature;    // A signature used during drag-drop operation

	BOOL m_bOleDragging;                // TRUE if OLE dragging is enabled, FALSE otherwise
	BOOL m_bFilesDragging;              // TRUE when dragging files, FALSE otherwise.
	COleDropTarget m_dropTarget;        // OLE drop target

	CSize m_szPage;                     // Page size for printing or preview
	int m_iTopRow;                      // Top row for display

	CWnd* m_pParentWnd;
	CXTPSyntaxEditCtrl* m_pEditCtrl;    // Edit control instance

	static CXTPSyntaxEditFindReplaceDlg* m_pFindReplaceDlg; // Pointer to find or replace dialog.

	BOOL m_bOnSizeRunning;          // Indicate that OnSize message handler is in progress.

	CXTPSyntaxEditViewPrintOptions* m_pPrintOptions; // Printing options.
	CUIntArray m_aPageStart;            // Stores Printed page start row indexes.

public:
	BOOL    m_bPrintDirect;             // if TRUE - the Print Dialog is bypassed. FALSE by default.
	BOOL    m_bResizeControlWithView;   // if FALSE - attached Calendar control will not be resized with view. TRUE by default.
};

/////////////////////////////////////////////////////////////////////////////

AFX_INLINE CXTPSyntaxEditBufferManager* CXTPSyntaxEditView::GetEditBuffer() const {
	return GetEditCtrl().GetEditBuffer();
}
AFX_INLINE CXTPSyntaxEditConfigurationManager* CXTPSyntaxEditView::GetLexConfigurationManager() const {
	return GetEditCtrl().GetLexConfigurationManager();
}
AFX_INLINE CXTPSyntaxEditViewPrintOptions* CXTPSyntaxEditView::GetPrintOptions() const {
	return m_pPrintOptions;
}

#endif // !defined(__XTPSYNTAXEDITSYNTAXEDITVIEW_H__)
