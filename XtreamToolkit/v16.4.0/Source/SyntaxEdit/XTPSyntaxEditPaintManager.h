// XTPSyntaxEditPaintManager.h interface for the CXTPSyntaxEditPaintManager class.
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
#if !defined(__XTPSYNTAXEDITPAINTMANAGER_H__)
#define __XTPSYNTAXEDITPAINTMANAGER_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPSyntaxEditCtrl;

//{{AFX_CODEJOCK_PRIVATE
#define XTP_DECLARE_COLOR_PROP(propName) \
	OLE_COLOR OleGet##propName();\
	void OleSet##propName(OLE_COLOR oleColor);
//}}AFX_CODEJOCK_PRIVATE

//===========================================================================
// Summary:
//     This class used by CXTPSyntaxEditCtrl as the paint manager.
//===========================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditPaintManager : public CXTPCmdTarget
{
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DYNAMIC(CXTPSyntaxEditPaintManager)
	//}}AFX_CODEJOCK_PRIVATE
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Default object constructor.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditPaintManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPSyntaxEditPaintManager object, handles cleanup
	//     and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPSyntaxEditPaintManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Initializes default paint manager bitmaps, should be called after
	//     creating control window.
	//-----------------------------------------------------------------------
	virtual void InitBitmaps();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of manager.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//      Update internal text font object (if need) and select it to DC.
	// Parameters:
	//      pEditCtrl   : [in] A pointer to edit control.
	//      lf          : [in] Reference to font options structure.
	// Returns:
	//      TRUE if the edit control font was updated, otherwise FALSE.
	// See also:
	//      struct LOGFONT
	//-----------------------------------------------------------------------
	virtual BOOL UpdateTextFont(CXTPSyntaxEditCtrl* pEditCtrl, const XTP_EDIT_FONTOPTIONS& lf);

	//-----------------------------------------------------------------------
	// Summary:
	//      Draw the line numbers and returns the column position to start
	//      rest of the drawing.
	// Parameters:
	//      pDC         : [in] Pointer to device context to draw to.
	//      rcRect      : [in] A rectangle to draw.
	//      nTextRow    : [in] The text row number for which to draw.
	//      pEditCtrl   : [in] A pointer to edit control.
	//-----------------------------------------------------------------------
	virtual void DrawLineNumber(CDC *pDC, const CRect& rcRect, int nTextRow,
								CXTPSyntaxEditCtrl* pEditCtrl);

	//-----------------------------------------------------------------------
	// Summary:
	//      Draw the line Collapsible node icon
	// Parameters:
	//      pDC         : [in] Pointer to device context to draw to.
	//      rcNode      : [in] A rectangle to draw node.
	//      rcNodeFull  : [in] A full node area rectangle.
	//      dwType      : [in] Line Node type flags.
	//      nTextRow    : [in] The text row number for which to draw.
	//      pEditCtrl   : [in] A pointer to edit control.
	//-----------------------------------------------------------------------
	virtual void DrawLineNode(CDC *pDC, const CRect& rcNode, const CRect& rcNodeFull,
							  DWORD dwType, int nTextRow, CXTPSyntaxEditCtrl* pEditCtrl);

	//-----------------------------------------------------------------------
	// Summary:
	//      Fill the line node background.
	// Parameters:
	//      pDC         : [in] Pointer to device context to draw to.
	//      rcNodeFull  : [in] A full node area rectangle.
	//      dwType      : [in] Line Node type flags.
	//      nTextRow    : [in] The text row number for which to draw.
	//      pEditCtrl   : [in] A pointer to edit control.
	//-----------------------------------------------------------------------
	virtual void DrawLineNodeBackground(CDC *pDC, const CRect& rcNodeFull, DWORD dwType,
										int nTextRow, CXTPSyntaxEditCtrl* pEditCtrl);

	//-----------------------------------------------------------------------
	// Summary:
	//      Draws frames around collapsed text blocks.
	// Parameters:
	//      pEditCtrl   : [in] A pointer to edit control.
	//      pDC         : [in] Pointer to device context.
	//-----------------------------------------------------------------------
	virtual void DrawCollapsedTextMarks(CXTPSyntaxEditCtrl* pEditCtrl, CDC *pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//      Draws marks for line.
	// Parameters:
	//      pDC         : [in] Pointer to device context to draw to.
	//      rcRect      : [in] A rectangle to draw.
	//      nTextRow    : [in] The text row number for which to draw.
	//      pEditCtrl   : [in] A pointer to edit control.
		//-----------------------------------------------------------------------
	virtual void DrawLineMarks(CDC *pDC, const CRect& rcRect, int nTextRow,
								CXTPSyntaxEditCtrl* pEditCtrl);

	//-----------------------------------------------------------------------
	// Summary:
	//      Draws a single line.
	// Parameters:
	//      pDC         : [in] Pointer to device context to draw to.
	//      rcTextLine  : [in] A rectangle to draw.
	//      nTextRow    : [in] The text row number.
	//      nLine       : [in] The screen (page) line number.
	//      pEditCtrl   : [in] A pointer to edit control.
	// Returns:
	//      The height to draw row text.
	//-----------------------------------------------------------------------
	virtual int DrawLineTextEx(CDC *pDC, const CRect& rcTextLine, int nTextRow, int nLine,
							   CXTPSyntaxEditCtrl* pEditCtrl);

	//-----------------------------------------------------------------------
	// Summary:
	//      Print a row text.
	// Parameters:
	//      pDC         : [in] Pointer to device context to draw to.
	//      rcTextLine  : [in] A rectangle to draw.
	//      nTextRow    : [in] The text row number.
	//      nLine       : [in] The screen (page) line number.
	//      pEditCtrl   : [in] A pointer to edit control.
	//      nFlags      : [in] Additional printing options. The folowing are supported:
	//                         DT_CALCRECT, DT_SINGLELINE, DT_WORDBREAK
	// Returns:
	//      The height to print row text.
	//-----------------------------------------------------------------------
	virtual int PrintLineTextEx(CDC *pDC, const CRect& rcTextLine, int nTextRow,
								int nLine, CXTPSyntaxEditCtrl* pEditCtrl, int nFlags = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//      Draws editor text line and calculates selection position.
	// Parameters:
	//      txtBlk      : [in] Reference to XTP_EDIT_TEXTBLOCK structure.
	//      nSelStartX  : [in] Start coordinate.
	//      nSelEndX    : [in] End coordinate.
	//      arTxtBlk    : [in] Array of references to XTP_EDIT_TEXTBLOCK structures.
	//      bUseBlk     : [in] Array of references to BOOL.
	// See also:
	//  struct XTP_EDIT_TEXTBLOCK
	//-----------------------------------------------------------------------
	virtual void DrawLineCalcSel(const XTP_EDIT_TEXTBLOCK& txtBlk,
		int nSelStartX, int nSelEndX,
		XTP_EDIT_TEXTBLOCK (&arTxtBlk)[4], BOOL (&bUseBlk)[4]);


protected:
	//-----------------------------------------------------------------------
	// Summary:
	//      Draws marks for line.
	// Parameters:
	//      pEditCtrl   : [in] A pointer to edit control.
	//      pBookmark   : [in] Pointer to structure with mark parameters.
	// See also:
	//      LPXTP_EDIT_SENMBOOKMARK
	//-----------------------------------------------------------------------
	virtual void DrawLineMark(CXTPSyntaxEditCtrl* pEditCtrl, XTP_EDIT_SENMBOOKMARK* pBookmark);

	//-----------------------------------------------------------------------
	// Summary:
	//      Draw the line numbers border    .
	// Parameters:
	//          pDC         : [in] Pointer to device context to print to
	//          rcLineNum   : [in] Line number rect
	//          clrBorder   : [in] Border color
	//-----------------------------------------------------------------------
	virtual void DrawLineNumbersBorder(CDC *pDC, const CRect& rcLineNum, const COLORREF clrBorder);

public:
	//-----------------------------------------------------------------------
	// Summary :
	//      Sets line selection cursor.
	// Parameters:
	//      hCurLine : [in] Handle to the cursor. The cursor must have been
	//                          created by the CreateCursor function or loaded
	//                          by the LoadCursor or LoadImage function.Cursor
	//                          appears in lines selection area.
	// Returns:
	//      Handle of previous cursor.
	//-----------------------------------------------------------------------
	HCURSOR SetLineSelCursor(HCURSOR hCurLine);

	//-----------------------------------------------------------------------
	// Summary:
	//      Sets cursor shape for "copy" cursor.
	// Parameters:
	//      hCurCopy : [in] Handle to the cursor. The cursor must have been
	//                      created by the CreateCursor function or loaded
	//                      by the LoadCursor or LoadImage function.
	// Returns:
	//      Handle of previous cursor.
	//-----------------------------------------------------------------------
	HCURSOR SetCopyCursor(HCURSOR hCurCopy);

	//-----------------------------------------------------------------------
	// Summary:
	//      Sets cursor shape for "move" cursor.
	// Parameters:
	//      hCurMove : [in] Handle to the cursor. The cursor must have been
	//                      created by the CreateCursor function or loaded
	//                      by the LoadCursor or LoadImage function.
	// Returns:
	//      Handle of previous cursor.
	//-----------------------------------------------------------------------
	HCURSOR SetMoveCursor(HCURSOR hCurMove);

	//-----------------------------------------------------------------------
	// Summary: Get text color.
	// Returns: Text color.
	// See Also: SetTextColor
	//-----------------------------------------------------------------------
	COLORREF GetTextColor() const;

	//-----------------------------------------------------------------------
	// Summary: Get background color.
	// Returns: Background color.
	// See Also: SetBackColor
	//-----------------------------------------------------------------------
	COLORREF GetBackColor() const;

	//-----------------------------------------------------------------------
	// Summary: Get corresponding background color for edit mode or for
	//          read-only mode.
	// Parameters:
	//  pEditCtrl : pointer to CXTPSyntaxEditCtrl
	// Returns: Background color.
	// See Also: SetBackColor, GetBackColor
	//-----------------------------------------------------------------------
	COLORREF GetBackColorEx(CXTPSyntaxEditCtrl* pEditCtrl);

	//-----------------------------------------------------------------------
	// Summary: Get selected text color.
	// Returns: Selected text color.
	// See Also: SetHiliteTextColor
	//-----------------------------------------------------------------------
	COLORREF GetHiliteTextColor() const;

	//-----------------------------------------------------------------------
	// Summary: Get Hilite back color.
	// Returns: Selected Hilite back color.
	//-----------------------------------------------------------------------
	COLORREF GetHiliteBackColor() const;

	//-----------------------------------------------------------------------
	// Summary: Get selected text color when control has no focus.
	// Returns: Selected text color when control has no focus.
	//-----------------------------------------------------------------------
	COLORREF GetInactiveHiliteTextColor() const;

	//-----------------------------------------------------------------------
	// Summary: Get selected background color when control has no focus.
	// Returns: Selected background color when control has no focus.
	//-----------------------------------------------------------------------
	COLORREF GetInactiveHiliteBackColor() const;

	//-----------------------------------------------------------------------
	// Summary: Get Line Numbers text color.
	// Returns: Line Numbers text color.
	// See Also: SetLineNumberTextColor
	//-----------------------------------------------------------------------
	COLORREF GetLineNumberTextColor() const;

	//-----------------------------------------------------------------------
	// Summary: Get Line Numbers background color.
	// Returns: Line Numbers background color.
	// See Also: SetLineNumberBackColor
	//-----------------------------------------------------------------------
	COLORREF GetLineNumberBackColor() const;

	//-----------------------------------------------------------------------
	// Summary: Set text color.
	// Parameters:
	//      crText      - A color value.
	//      bUpdateReg  - Set TRUE to save color in registry.
	// Returns: TRUE if successful, FALSE otherwise.
	// See Also: GetTextColor
	//-----------------------------------------------------------------------
	BOOL SetTextColor(COLORREF crText, BOOL bUpdateReg=FALSE);

	//-----------------------------------------------------------------------
	// Summary: Set background color.
	// Parameters:
	//      crBack      - A color value.
	//      bUpdateReg  - Set TRUE to save color in registry.
	// Returns: TRUE if successful, FALSE otherwise.
	// See Also: GetBackColor
	//-----------------------------------------------------------------------
	BOOL SetBackColor(COLORREF crBack, BOOL bUpdateReg = FALSE);

	//-----------------------------------------------------------------------
	// Summary: Set background color for read-onlymode.
	// Parameters:
	//      crBack      - A color value.
	//      bUpdateReg  - Set TRUE to save color in registry.
	// Returns: TRUE if successful, FALSE otherwise.
	// See Also: GetBackColor
	//-----------------------------------------------------------------------
	BOOL SetReadOnlyBackColor(COLORREF crBack, BOOL bUpdateReg = FALSE);

	//-----------------------------------------------------------------------
	// Summary: Set selected text color.
	// Parameters:
	//      color - A color value.
	//      bUpdateReg - Set TRUE to save color in registry.
	// Returns: TRUE if successful, FALSE otherwise.
	// See Also: GetHiliteTextColor
	//-----------------------------------------------------------------------
	BOOL SetHiliteTextColor(COLORREF color, BOOL bUpdateReg = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Set selected background color.
	// Parameters:
	//      color      - A color value.
	//      bUpdateReg  - Set TRUE to save color in registry.
	// Returns: TRUE if successful, FALSE otherwise.
	// See Also: GetHiliteBackColor
	//-----------------------------------------------------------------------
	BOOL SetHiliteBackColor(COLORREF color, BOOL bUpdateReg = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Set selected text color when control has no focus.
	// Parameters:
	//      color      - A color value.
	//      bUpdateReg  - Set TRUE to save color in registry.
	// Returns: TRUE if successful, FALSE otherwise.
	// See Also: GetInactiveHiliteTextColor
	//-----------------------------------------------------------------------
	BOOL SetInactiveHiliteTextColor(COLORREF color, BOOL bUpdateReg = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Set selected background color when control has no focus.
	// Parameters:
	//      color      - A color value.
	//      bUpdateReg  - Set TRUE to save color in registry.
	// Returns: TRUE if successful, FALSE otherwise.
	// See Also: GetInactiveHiliteBackColor
	//-----------------------------------------------------------------------
	BOOL SetInactiveHiliteBackColor(COLORREF color, BOOL bUpdateReg = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Set Line Numbers text color.
	// Parameters:
	//      color - A color value.
	//      bUpdateReg  - Set TRUE to save color in registry.
	// Returns: TRUE if successful, FALSE otherwise.
	// See Also: GetLineNumberTextColor
	//-----------------------------------------------------------------------
	BOOL SetLineNumberTextColor(COLORREF color, BOOL bUpdateReg=FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Set Line Numbers background color.
	// Parameters:
	//      color - A color value.
	//      bUpdateReg  - Set TRUE to save color in registry.
	// Returns: TRUE if successful, FALSE otherwise.
	// See Also: GetLineNumberBackColor
	//-----------------------------------------------------------------------
	BOOL SetLineNumberBackColor(COLORREF color, BOOL bUpdateReg=FALSE);

public:
	//-----------------------------------------------------------------------
	// Summary: Get common font.
	// Returns: A pointer to CFont object.
	// See Also: SetFont
	//-----------------------------------------------------------------------
	CFont* GetFont();

	//-----------------------------------------------------------------------
	// Summary: Get editor text font.
	// Returns: A pointer to CFont object.
	// See Also: SetFontText
	//-----------------------------------------------------------------------
	CFont* GetFontText();

	//-----------------------------------------------------------------------
	// Summary: Get line numbers font.
	// Returns: A pointer to CFont object.
	// See Also: SetFontLineNumber
	//-----------------------------------------------------------------------
	CFont* GetFontLineNumber();

	//-----------------------------------------------------------------------
	// Summary: Get ToolTip font.
	// Returns: A pointer to CFont object.
	// See Also: SetFontToolTip
	//-----------------------------------------------------------------------
	CFont* GetFontToolTip();    // ToolTip font.

	//-----------------------------------------------------------------------
	// Summary: Set common font.
	// Parameters:
	//      pLogfont -  A pointer to LPLOGFONT structure.
	// Returns: TRUE if successful, FALSE otherwise.
	// See Also: GetFont
	//-----------------------------------------------------------------------
	BOOL SetFont(LPLOGFONT pLogfont);

	//-----------------------------------------------------------------------
	// Summary: Set editor text font.
	// Parameters:
	//      pLogfont -  A pointer to LPLOGFONT structure.
	// Returns: TRUE if successful, FALSE otherwise.
	// See Also: GetFontText
	//-----------------------------------------------------------------------
	BOOL SetFontText(LPLOGFONT pLogfont);

	//-----------------------------------------------------------------------
	// Summary: Set line numbers font.
	// Parameters:
	//      pLogfont -  A pointer to LPLOGFONT structure.
	// Returns: TRUE if successful, FALSE otherwise.
	// See Also: GetFontLineNumber
	//-----------------------------------------------------------------------
	BOOL SetFontLineNumber(LPLOGFONT pLogfont);

	//-----------------------------------------------------------------------
	// Summary: Set ToolTip font.
	// Parameters:
	//      pLogfont -  A pointer to LPLOGFONT structure.
	// Returns: TRUE if successful, FALSE otherwise.
	// See Also: GetFontToolTip
	//-----------------------------------------------------------------------
	BOOL SetFontToolTip(LPLOGFONT pLogfont);

	//-----------------------------------------------------------------------
	// Summary:
	//  Set all editor fonts.
	// Parameters:
	//  pLogfont -  A pointer to LPLOGFONT structure.
	//  bUpdateReg - flag for registry update
	// Returns:
	//  TRUE if successful, FALSE otherwise.
	// See Also: SetFont, SetFontText, SetFontLineNumber, SetFontToolTip
	//-----------------------------------------------------------------------
	BOOL CreateFontIndirect(LPLOGFONT pLogfont, BOOL bUpdateReg = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//  Get Cursor to show on line selection area.
	// Returns:
	//  HCURSOR handle to cursor.
	// See Also: SetLineSelCursor
	//-----------------------------------------------------------------------
	HCURSOR GetCurLine();

	//-----------------------------------------------------------------------
	// Summary:
	//  Get Cursor for moving text.
	// Returns:
	//  HCURSOR handle to cursor.
	// See Also: SetMoveCursor
	//-----------------------------------------------------------------------
	HCURSOR GetCurMove();

	//-----------------------------------------------------------------------
	// Summary: Get Cursor for copying text.
	// Returns: A HCURSOR handle to cursor.
	// See Also: SetCopyCursor
	//-----------------------------------------------------------------------
	HCURSOR GetCurCopy();

	//-----------------------------------------------------------------------
	// Summary: Get standard NO cursor.
	// Returns: A HCURSOR handle to cursor.
	//-----------------------------------------------------------------------
	HCURSOR GetCurNO();

	//-----------------------------------------------------------------------
	// Summary: Get standard IBEAM cursor.
	// Returns: A HCURSOR handle to cursor.
	//-----------------------------------------------------------------------
	HCURSOR GetCurIBeam();

	//-----------------------------------------------------------------------
	// Summary: Get standard ARROW cursor.
	// Returns: A HCURSOR handle to cursor.
	//-----------------------------------------------------------------------
	HCURSOR GetCurArrow();

	CString m_sLineNumberFormat;
	//String to customize Line Number format (e.g. keep it fixed legnth like %04d)
	// If empty - Control used default - variable legnth Line Numbers)

protected:
	// fonts
	CFont m_font;       // Default font for normal text.
	CFont m_fontText;   // Current text font
	CFont m_fontLineNumber; // The font for line numbering
	CFont m_fontToolTip;    // The font for tool tip text

	// cursors
	HCURSOR m_hCurLine; // Cursor to show on line selection area
	HCURSOR m_hCurMove; // Cursor for moving text
	HCURSOR m_hCurCopy; // Cursor for copying text
	HCURSOR m_hCurNO;   // Standard NO cursor
	HCURSOR m_hCurIBeam;    // Standard IBEAM cursor
	HCURSOR m_hCurArrow;    // Standard ARROW cursor

	CImageList m_ilBookmark;    // Image list with all bookmark images

	CPen m_penGray;             // Standard common gray pen

	XTP_EDIT_COLORVALUES   m_clrValues;    // Default color

	//-----------------------------------------------------------------------
	// Summary: Get Bookmarks image list.
	// Returns: A pointer to CImageList.
	//-----------------------------------------------------------------------
	virtual CImageList* GetBookmarks();

protected:
#ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPSyntaxEditPaintManager)

	XTP_DECLARE_COLOR_PROP(TextColor)
	XTP_DECLARE_COLOR_PROP(BackColor)
	XTP_DECLARE_COLOR_PROP(ReadOnlyBackColor)
	XTP_DECLARE_COLOR_PROP(LineNumberTextColor)
	XTP_DECLARE_COLOR_PROP(LineNumberBackColor)
	XTP_DECLARE_COLOR_PROP(SelectedTextColor)
	XTP_DECLARE_COLOR_PROP(SelectedBackColor)

	//}}AFX_CODEJOCK_PRIVATE
#endif
};

/////////////////////////////////////////////////////////////////////////////
AFX_INLINE COLORREF CXTPSyntaxEditPaintManager::GetTextColor() const {
	return m_clrValues.crText;
}
AFX_INLINE COLORREF CXTPSyntaxEditPaintManager::GetBackColor() const {
	return m_clrValues.crBack;
}
AFX_INLINE COLORREF CXTPSyntaxEditPaintManager::GetBackColorEx(CXTPSyntaxEditCtrl* pEditCtrl) {
	return m_clrValues.GetBackColorEx(pEditCtrl);
}
AFX_INLINE COLORREF CXTPSyntaxEditPaintManager::GetHiliteTextColor() const {
	return m_clrValues.crHiliteText;
}
AFX_INLINE COLORREF CXTPSyntaxEditPaintManager::GetHiliteBackColor() const {
	return m_clrValues.crHiliteBack;
}
AFX_INLINE COLORREF CXTPSyntaxEditPaintManager::GetInactiveHiliteTextColor() const {
	return m_clrValues.crInactiveHiliteText;
}
AFX_INLINE COLORREF CXTPSyntaxEditPaintManager::GetInactiveHiliteBackColor() const {
	return m_clrValues.crInactiveHiliteBack;
}
AFX_INLINE COLORREF CXTPSyntaxEditPaintManager::GetLineNumberTextColor() const {
	return m_clrValues.crLineNumberText;
}
AFX_INLINE COLORREF CXTPSyntaxEditPaintManager::GetLineNumberBackColor() const {
	return m_clrValues.crLineNumberBack;
}

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__XTPSYNTAXEDITPAINTMANAGER_H__)
