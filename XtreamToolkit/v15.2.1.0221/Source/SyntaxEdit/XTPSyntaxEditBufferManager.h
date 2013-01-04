// XTPSyntaxEditBufferManager.h
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// (c)1998-2012 Codejock Software, All Rights Reserved.
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
//////////////////////////////////////////////////////////////////////

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPSYNTAXEDITBUFFERMANAGER_H__)
#define __XTPSYNTAXEDITBUFFERMANAGER_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


namespace XTPSyntaxEditLexAnalyser
{
	class CXTPSyntaxEditLexTextSchema;
	class CXTPSyntaxEditLexParser;
};

typedef XTPSyntaxEditLexAnalyser::CXTPSyntaxEditLexTextSchema CXTPSyntaxEditTextSchema;
class CXTPSyntaxEditLineMarksManager;
class CXTPSyntaxEditUndoRedoManager;
class CXTPNotifySink;
class CXTPNotifyConnection;
class CXTPSyntaxEditConfigurationManager;
typedef CXTPSmartPtrInternalT<CXTPSyntaxEditConfigurationManager> CXTPSyntaxEditConfigurationManagerPtr;

//===========================================================================
// Summary:
//      This class is the main class for buffer management. One instance
//      of this class is attached with a CXTPSyntaxEditCtrl object. This class
//      contains a list of CBufferIndex class which in turn contains
//      a position in the file (In case of un-modified index) or a CEditBuffer
//      pointer.
//===========================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditBufferManager : public CXTPCmdTarget
{
	DECLARE_DYNCREATE(CXTPSyntaxEditBufferManager)
	friend class CXTPSyntaxEditCtrl;
public:
	//-----------------------------------------------------------------------
	// Summary:
	//      Default object constructor.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditBufferManager();

	//-----------------------------------------------------------------------
	// Summary:
	//      Destroys a CXTPSyntaxEditBufferManager object, handles cleanup
	//      and de-allocation.
	//-----------------------------------------------------------------------
	~CXTPSyntaxEditBufferManager();

	//-----------------------------------------------------------------------
	// Summary:
	//      Determines if the file has been modified or not.
	// Returns:
	//      TRUE if modified, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL IsModified() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Determines if the file has been modified or not.
	// Returns:
	//      CodePage to be used for conversion.
	//-----------------------------------------------------------------------
	UINT GetCodePage() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Returns the tab size for the current document.
	// Returns:
	//      Selected tab size.
	//-----------------------------------------------------------------------
	int GetTabSize() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Set the tab size between 1-64.
	// Parameters:
	//      nTabSize   : [in] The tab size to set.
	//      bUpdateReg : [in] Set this parameter as TRUE to update this setting
	//                        in the registry. It is FALSE by default.
	// Returns:
	//      TRUE if the operation is successful and FALSE else.
	//-----------------------------------------------------------------------
	BOOL SetTabSize(int nTabSize, BOOL bUpdateReg = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//      Returns the overwrite flag status.
	// Returns:
	//      TRUE if OVR mode is set, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL GetOverwriteFlag() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Sets or resets the OVR flag.
	// Parameters:
	//      bOverwrite : [in] Pass TRUE to set OVR flag, FALSE to INS flag.
	//-----------------------------------------------------------------------
	void SetOverwriteFlag(BOOL bOverwrite);

	//-----------------------------------------------------------------------
	// Summary:
	//      Returns the parser status.
	// Returns:
	//      TRUE if Parser is enabled, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL IsParserEnabled() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Enable or disable parser.
	// Parameters:
	//      bEnable : [in] Pass TRUE to enable parser, FALSE to disable.
	//-----------------------------------------------------------------------
	void EnableParser(BOOL bEnable);

	//-----------------------------------------------------------------------
	// Summary:
	//      Returns the CRLF type of the current document.
	// Returns:
	//      Current CRLF type.
	//-----------------------------------------------------------------------
	int GetCurCRLFType() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Set the current documents CRLF style.
	// Parameters:
	//      nStyle : [in] The CRLF style to set
	//-----------------------------------------------------------------------
	void SetCRLFStyle(int nStyle);

	//-----------------------------------------------------------------------
	// Summary:
	//      Returns the CRLF of the current document.
	// Returns:
	//      Current CRLF text.
	//-----------------------------------------------------------------------
	CString GetCurCRLF() const;

private:
	//-----------------------------------------------------------------------
	// Summary:
	//      Internal implementation of getting CRLF of the current document.
	// Parameters:
	//      nCRLFStyle : [in] Index of the CRLF style string.
	// Returns:
	//      Current CRLF text.
	//-----------------------------------------------------------------------
	LPCTSTR GetCRLF(int nCRLFStyle = -1) const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//      Determines if the supplied text is a CRLF or not
	// Parameters:
	//      szCompText   : [in] The text to be compared.
	//      bFindReverse : [in] Should be match reverse.
	// Returns:
	//      TRUE if the supplied text is a CRLF, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL IsTextCRLF(LPCTSTR szCompText, BOOL bFindReverse = FALSE) const;

#ifdef _UNICODE
	//-----------------------------------------------------------------------
	// Summary:
	//      Determines if the supplied text is a CRLF or not of the non-unicode string.
	// Parameters:
	//      szCompText   : [in] The text to be compared.
	//      bFindReverse : [in] Should be match reverse.
	// Returns:
	//      TRUE if the supplied text is a CRLF, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL IsTextCRLF(LPCSTR szCompText, BOOL bFindReverse = FALSE) const;
#endif

	//-----------------------------------------------------------------------
	// Summary:
	//      Returns the number of rows in the document.
	// Returns:
	//      Number of rows in the document.
	//-----------------------------------------------------------------------
	int GetRowCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Returns pointer to the associated Undo/Redo manager.
	// Returns:
	//      Pointer to a CXTPSyntaxEditUndoRedoManager object.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditUndoRedoManager* GetUndoRedoManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Returns pointer to the associated line marks manager.
	// Returns:
	//      Pointer to a CXTPSyntaxEditLineMarksManager object.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditLineMarksManager* GetLineMarksManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Returns pointer to the associated lexical parser.
	// Returns:
	//      Pointer to a XTPSyntaxEditLexAnalyser::CXTPSyntaxEditLexParser object.
	//-----------------------------------------------------------------------
	XTPSyntaxEditLexAnalyser::CXTPSyntaxEditLexParser* GetLexParser() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Returns pointer to the associated configuration manager.
	// Returns:
	//      A pointer to the configuration manager object.
	//
	//-----------------------------------------------------------------------
	CXTPSyntaxEditConfigurationManager* GetLexConfigurationManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Set pointer to the configuration manager.
	// Parameters:
	//      pMan : [in] Pointer to CXTPSyntaxEditConfigurationManager.
	//-----------------------------------------------------------------------
	void SetLexConfigurationManager(CXTPSyntaxEditConfigurationManager* pMan);

	//-----------------------------------------------------------------------
	// Summary:
	//      Returns events connection pointer.
	// Returns:
	//      A pointer to the notify connection object.
	//-----------------------------------------------------------------------
	CXTPNotifyConnection* GetConnection() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Cleans up all internal objects.
	//-----------------------------------------------------------------------
	void Close();

	//-----------------------------------------------------------------------
	// Summary:
	//      Serializes actual data.
	// Parameters:
	//      ar : [in, out] The archive to save/load text.
	//-----------------------------------------------------------------------
	virtual void Serialize(CArchive& ar);

	//-----------------------------------------------------------------------
	// Summary:
	//      Serializes actual data.
	// Parameters:
	//      ar                      : [in, out] The archive to save/load text.
	//      bUnicode                : [in] Use UNICODE format for text in the archive.
	//      bWriteUnicodeFilePrefix : [in] Write Unicode File Prefix.
	//      nCodePage               : [in] Text code page (or -1 to use default).
	//      pcszFileExt             : [in] File extension to select appropriate parser schema.
	//      nDataSizeLimit          : [in] The number of bytes to stop storing text to archive
	//                                     (or -1 to store all).
	//-----------------------------------------------------------------------
	virtual void SerializeEx(CArchive &ar, BOOL bUnicode = -1, BOOL bWriteUnicodeFilePrefix = TRUE,
					  UINT nCodePage = (UINT)-1, LPCTSTR pcszFileExt = NULL, int nDataSizeLimit = -1);

	//-----------------------------------------------------------------------
	// Summary:
	//      Set the file pointer for the buffer.
	// Parameters:
	//      pFile       : [in] The file pointer to set.
	//      pcszFileExt : [in] Number of lines (Not used).
	// See Also:
	//      SetFileExt()
	//-----------------------------------------------------------------------
	void Load(CFile *pFile, LPCTSTR pcszFileExt = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//      Sets file extension.
	// Parameters:
	//      strExt  : [in] string of extension.
	// See also:
	//      CString GetFileExt()
	//-----------------------------------------------------------------------
	void SetFileExt(const CString& strExt);

	//-----------------------------------------------------------------------
	// Summary:
	//      Returns file extension.
	// Returns:
	//      CString object with file extensions.
	// See also:
	//      void SetFileExt(const CString& strExt);
	//-----------------------------------------------------------------------
	CString GetFileExt() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Gets a line of text either from file or buffer.
	// Parameters:
	//      iLine       : [in] The line for which to get the text.
	//      strText     : [out] A reference to CString object to be filled with text.
	//      bAddCRLF    : [in] Pass TRUE to add CRLF (if exists), otherwise .
	//      iCRLFStyle  : [in] Specify the CRLF style.
	//-----------------------------------------------------------------------
	void GetLineText(int iLine, CString& strText, BOOL bAddCRLF = FALSE, int iCRLFStyle = -1);

	//-----------------------------------------------------------------------
	// Summary:
	//      Returns a line of text either from file or buffer.
	// Parameters:
	//      iLine       : [in] The line for which to get the text.
	//      bAddCRLF    : [in] Pass TRUE to add CRLF (if exists), otherwise .
	//      iCRLFStyle  : [in] Specify the CRLF style.
	// Returns:
	//      CString object with text line.
	//-----------------------------------------------------------------------
	CString GetLineText(int iLine, BOOL bAddCRLF = FALSE, int iCRLFStyle = -1) const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Returns a length for line of text in TCHARs.
	// Parameters:
	//      iLine       : [in] The line for which to get the text.
	//      bAddCRLF    : [in] Pass TRUE to add CRLF (if exsts), otherwise .
	//      iCRLFStyle  : [in] Specify the CRLF style.
	// Returns:
	//      Text line length as integer value.
	//-----------------------------------------------------------------------
	int GetLineTextLength(int iLine, BOOL bAddCRLF = FALSE, int iCRLFStyle = -1) const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Returns a length for line of text in characters.
	// Parameters:
	//      iLine       : [in] The line for which to get the text.
	//      bAddCRLF    : [in] Pass TRUE to add CRLF (if exsts), otherwise .
	//      iCRLFStyle  : [in] Specify the CRLF style.
	// Returns:
	//      Text line length in characters as integer value.
	//-----------------------------------------------------------------------
	int GetLineTextLengthC(int iLine, BOOL bAddCRLF = FALSE, int iCRLFStyle = -1) const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Returns the max allowable length for line of text.
	// Returns:
	//      Text line length as integer value.
	//-----------------------------------------------------------------------
	int GetMaxLineTextLength() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Returns a length for line of text either from file or buffer.
	// Parameters:
	//      nLineFrom   : [in] Start line identifier.
	//      nLineTo     : [in] End line identifier.
	//      bExpandTabs : [in] Expand tab characters.
	// Returns:
	//      Text length as integer value.
	//-----------------------------------------------------------------------
	int CalcMaxLineTextLength(int nLineFrom = -1, int nLineTo = -1, BOOL bExpandTabs = FALSE) const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Retrieve text from the buffer bounded between
	//      row1/col1 and row2/col2 text coordinates.
	// Parameters:
	//      row1    : [in] Start line number.
	//      col1    : [in] Start column number.
	//      row2    : [in] End line number.
	//      col2    : [in] End column number.
	//      file                : [out] A reference to CMemFile object to receive text.
	//      bColumnSelection    : [in]  If this parameter TRUE - the text block started
	//                                  from col1 and ended on col2 for each line
	//                                  between row1 and row2 copied, otherwise full
	//                                  row text copied for rows between row1 and row2.
	//      bForceDOSStyleCRLF  : [in]  If this parameter TRUE the DOS Style carriage
	//                                  return chars will be used to end each line,
	//                                  otherwise current carriage return style is used.
	// Returns:
	//      TRUE if the succeeded, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL GetBuffer(int row1, int col1, int row2, int col2,
					CMemFile& file, BOOL bColumnSelection = FALSE,
					BOOL bForceDOSStyleCRLF = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//      Retrieve text from the buffer bounded between
	//      lcStart and lcEnd text coordinates.
	// Parameters:
	//      lcStart : [in] Start text position.
	//      lcEnd   : [in] End text position.
	//      file                : [out] A reference to CMemFile object to receive text.
	//      bColumnSelection    : [in]  If this parameter TRUE - the text block started
	//                                  from col1 and ended on col2 for each line
	//                                  between row1 and row2 copied, otherwise full
	//                                  row text copied for rows between row1 and row2.
	//      bForceDOSStyleCRLF  : [in]  If this parameter TRUE the DOS Style carriage
	//                                  return chars will be used to end each line,
	//                                  otherwise current carriage return style is used.
	// Returns:
	//      TRUE if the succeeded, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL GetBuffer(const XTP_EDIT_LINECOL& lcStart, const XTP_EDIT_LINECOL& lcEnd,
					CMemFile& file, BOOL bColumnSelection = FALSE,
					BOOL bForceDOSStyleCRLF = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//      Inserts a text.
	// Parameters:
	//      szText           : [in] Text to be inserted.
	//      iRow             : [in] Start row for delete.
	//      iCol             : [in] Start col for delete.
	//      bCanUndo         : [in] Pass TRUE if this can be undone.
	//      pFinalLC         : [out] A pointer to XTP_EDIT_LINECOL variable to
	//                               receive operation final position in the text
	//                               (to set caret).
	// Returns:
	//      TRUE if the succeeded, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL InsertText(LPCTSTR szText, int iRow, int iCol, BOOL bCanUndo = TRUE,
					XTP_EDIT_LINECOL* pFinalLC = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//      Inserts a text block.
	// Parameters:
	//      szText           : [in] Text to be inserted.
	//      nRow             : [in] Start row for delete.
	//      nCol             : [in] Start col for delete.
	//      bCanUndo         : [in] Pass TRUE if this can be undone.
	//      pFinalLC         : [out] A pointer to XTP_EDIT_LINECOL variable to
	//                               receive operation final position in the text
	//                               (to set caret).
	// Returns:
	//      TRUE if the succeeded, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL InsertTextBlock(LPCTSTR szText, int nRow, int nCol, BOOL bCanUndo = TRUE,
					XTP_EDIT_LINECOL* pFinalLC = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//      Deletes a certain range of text
	// Parameters:
	//      iRowFrom         : [in] Start row for delete.
	//      iColFrom         : [in] Start col for delete.
	//      iRowTo           : [in] End row for delete.
	//      iColTo           : [in] End col for delete.
	//      bCanUndo         : [in] Pass TRUE if this can be undone.
	//      bDispCol         : [in] if TRUE iColFrom and iColTo are display cols,
	//                                  otherwise they are string cols.
	// Returns:
	//      TRUE if the succeeded, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL DeleteText(int iRowFrom, int iColFrom, int iRowTo, int iColTo,
					BOOL bCanUndo = TRUE, BOOL bDispCol = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//      Deletes a certain range of text
	// Parameters:
	//      nRow        : [in] Start row for delete.
	//      bCanUndo    : [in] Pass TRUE if this can be undone.
	//      nRowsCount  : [in] Rows count to delete.
	// Returns:
	//      TRUE if the succeeded, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL RemoveLine(int nRow, BOOL bCanUndo = TRUE, int nRowsCount = 1);

	//--------------------------------------------------------------------
	// Summary:
	//      Get main configuration file name.
	// Remarks:
	//      Return configuration file name previously stored by
	//      SetConfigFile().
	// Returns:
	//      Main configuration file name.
	// See also:
	//      CXTPSyntaxEditBufferManager::SetConfigFile().
	//--------------------------------------------------------------------
	CString GetConfigFile() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Set the path to the main configuration file.
	// Parameters:
	//      szPath : [in] A valid path for searching the .ini files.
	// Remarks:
	//      Configuration will be reloaded.
	// Returns:
	//      TRUE if path is found, FALSE otherwise.
	// See also:
	//      CXTPSyntaxEditBufferManager::GetConfigFile().
	//-----------------------------------------------------------------------
	BOOL SetConfigFile(LPCTSTR szPath);

	//-----------------------------------------------------------------------
	// Summary:
	//      Set the path to the main configuration file.
	// Parameters:
	//      strExt : [in] file extension for for schema definition file.
	// Returns:
	//      CXTPSyntaxEditTextSchema
	//-----------------------------------------------------------------------
	CXTPSyntaxEditTextSchema* GetMasterTextSchema(const CString& strExt) const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Changes the case of the specified characters range in the specified row.
	// Parameters:
	//      nRow      : [in] A row to change case at.
	//      nDispFrom : [in] Begin visible position where to change case at.
	//      nDispTo   : [in] End visible position where to change case at.
	//      bUpper    : [in] TRUE when changing selection case to Upper,
	//                       FALSE when changing selection case to Lower.
	//      bCanUndo  : [in] Pass TRUE if this can be undone.
	// Remarks:
	//      Configuration will be reloaded.
	//-----------------------------------------------------------------------
	void ChangeCase(int nRow, int nDispFrom, int nDispTo, BOOL bUpper, BOOL bCanUndo = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//      Changes the case of the specified characters range in the specified row.
	// Parameters:
	//      nRow      : [in] A row to change case at.
	//      nDispFrom : [in] Begin visible position where to change case at.
	//      nDispTo   : [in] End visible position where to change case at.
	//      bTabify   : [in] TRUE when changing spaces to tabs,
	//                       FALSE when changing tabs to spaces.
	//      bCanUndo  : [in] Pass TRUE if this can be undone.
	// Remarks:
	//      Configuration will be reloaded.
	//-----------------------------------------------------------------------
	void ChangeTabification(int nRow, int nDispFrom, int nDispTo, BOOL bTabify, BOOL bCanUndo = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//      Converts column coordinates of text to string position coordinate.
	// Parameters:
	//      nLine   : [in] Text line identifier.
	//      nDispCol: [in] Column coordinate.
	// Returns:
	//      String position identifier corresponding to column identifier.
	// See also:
	//      virtual int StrPosToCol(int nLine, int nStrPos) const;
	//-----------------------------------------------------------------------
	virtual int ColToStrPos(int nLine, int nDispCol) const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Converts string position of text to column coordinates coordinate.
	// Parameters:
	//      nLine   : [in] Text line identifier.
	//      nStrPos : [in] A string position.
	// Returns:
	//      Column identifier corresponding to string position identifier.
	// See also:
	//      virtual int ColToStrPos(int nLine, int nDispCol) const;
	//-----------------------------------------------------------------------
	virtual int StrPosToCol(int nLine, int nStrPos) const;

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//      Cleans up the buffer and all other allocated memories
	//-----------------------------------------------------------------------
	void CleanUp();

	//-----------------------------------------------------------------------
	// Summary:
	//      Returns average data size.
	// Parameters:
	//      nRowStart   : [in] Start row identifier.
	//      nRowEnd     : [in] End row identifier.
	// Returns:
	//      Average data size as integer value.
	// Remarks:
	//      Call this member function to calculate average data size based
	//      on average row length.
	//-----------------------------------------------------------------------
	virtual UINT CalcAveDataSize(int nRowStart, int nRowEnd);

	int m_nAverageLineLen;  // Stores average line length.

	CXTPNotifyConnection*   m_pConnection; // Connection object to send notifications.

protected:

	int m_nTabSize;     // Tab size
	int m_iCRLFStyle;   // CRLF style 0 - DOS, 1 - UNIX, 2 - MAC
	UINT m_nCodePage;                   // CodePage for text conversion
	BOOL m_bUnicodeFileFormat;          // Determine is file format Unicode or ASCII.
	BOOL m_bOverwrite;                  // Overwrite mode status

	CXTPSyntaxEditUndoRedoManager* m_pUndoRedoManager;    // Undo / redo manager.

	CXTPSyntaxEditLineMarksManager* m_pLineMarksManager; // line marks manager

	XTPSyntaxEditLexAnalyser::CXTPSyntaxEditLexParser* m_pLexParser; // Lexical parser

	CXTPSyntaxEditConfigurationManagerPtr m_ptrLexConfigurationManager;
	// Pointer to the Lexical configuration manager

	static CXTPSyntaxEditConfigurationManagerPtr s_ptrLexConfigurationManager_Default;
	// The common default Configuration Manager instance.

	static LONG s_dwLexConfigurationManager_DefaultRefs;
	// The reference count for common default Configuration Manager instance.

	CString m_strFileExt;   // Stores file extension

	BOOL m_bIsParserEnabled; // Stores flag of parser state (enabled/disabled)

	//{{AFX_CODEJOCK_PRIVATE
	CXTPNotifySink* m_pLexConfigManSinkMT;

	virtual void OnLexConfigManEventHandler(XTP_NOTIFY_CODE Event,
											WPARAM wParam, LPARAM lParam);
	//}}AFX_CODEJOCK_PRIVATE

	//------------------------------------------------------------------------
	// Summary:
	//      This is helper class designed to manage a set of strings
	//------------------------------------------------------------------------
	class _XTP_EXT_CLASS CXTPSyntaxEditStringsManager
	{
		//{{AFX_CODEJOCK_PRIVATE
		friend class CXTPSyntaxEditBufferManager;
		//}}AFX_CODEJOCK_PRIVATE
	public:

		//----------------------------------------------------------------------
		// Summary:
		//      Default object constructor.
		//----------------------------------------------------------------------
		CXTPSyntaxEditStringsManager();

		//----------------------------------------------------------------------
		// Summary:
		//      Default object destructor. Handles cleanup and deallocation
		//----------------------------------------------------------------------
		virtual ~CXTPSyntaxEditStringsManager();

		//----------------------------------------------------------------------
		// Summary:
		//      Returns count of managed strings
		// Returns:
		//      An integer value specifying the number of strings.
		//----------------------------------------------------------------------
		int GetCount() const;

		//----------------------------------------------------------------------
		// Summary:
		//      Returns string by given index.
		// Parameters:
		//      nRow : [in] String identifier.
		// Returns:
		//      CString object.
		//----------------------------------------------------------------------
		CString GetStr(int nRow) const;

		//----------------------------------------------------------------------
		// Summary:
		//      Returns length of string in TCHARs.
		// Parameters:
		//      nRow : [in] String identifier.
		// Returns:
		//      Integer value of length of string.
		//----------------------------------------------------------------------
		int GetStrLen(int nRow) const;

		//----------------------------------------------------------------------
		// Summary:
		//      Returns length of string in characters.
		// Parameters:
		//      nRow : [in] String identifier.
		// Returns:
		//      Integer value of length of string in characters.
		//----------------------------------------------------------------------
		int GetStrLenC(int nRow) const;

		//----------------------------------------------------------------------
		// Summary:
		//      Sets string at given id.
		// Parameters:
		//      nRow    : [in] String identifier.
		//      strText : [in] Reference to CString object with text to set.
		// Remarks:
		//      If given string id doesn't exist in collection function does
		//      nothing.
		// See also:
		//      void SetAtGrowStr(int nRow, LPCTSTR pcszText);
		//----------------------------------------------------------------------
		void SetAtGrowStr(int nRow, const CString& strText);

		//----------------------------------------------------------------------
		// Summary:
		//      Sets string at given id.
		// Parameters:
		//      nRow    : [in] String identifier.
		//      pcszText: [in] Pointer to null terminated string with text to set.
		// Remarks:
		//      If given string id doesn't exist in collection function does
		//      nothing.
		// See also:
		//      void SetAtGrowStr(int nRow, const CString& strText);
		//----------------------------------------------------------------------
		void SetAtGrowStr(int nRow, LPCTSTR pcszText);

		//----------------------------------------------------------------------
		// Summary:
		//      Add string.
		// Parameters:
		//      nRow    : [in] String identifier.
		//      strText : [in] Reference to CString object with text to set.
		// Remarks:
		//      If given string id exists in collection function sets string to
		//      given id.
		// See also:
		//      void SetAtGrowStr(int nRow, const CString& strText);
		//----------------------------------------------------------------------
		void InsertStr(int nRow, const CString& strText);

		//----------------------------------------------------------------------
		// Summary:
		//      Removes string.
		// Parameters:
		//      nRow    : [in] String identifier.
		// Returns:
		//      TRUE if success; FALSE if given id doesn't exist.
		//----------------------------------------------------------------------
		BOOL RemoveStr(int nRow);

		//----------------------------------------------------------------------
		// Summary:
		//      Removes all strings from collection.
		//----------------------------------------------------------------------
		void RemoveAllStrs();

		//-----------------------------------------------------------------------
		// Summary: Inserts text.
		// Parameters:
		//      nRow - [in] String identifier.
		//      nPos - [in] Start position in the string.
		//      pcszText          - [in] Text to insert.
		//      bGrowArrayIfNeed  - [in] If nRow greater than strings count and this
		//                          parameter TRUE the strings array will be grown;
		//                          if this parameter FALSE function will do nothing
		//                          and return FALSE.
		//      chLeftSpaceFiller - [in] if nPos parameter greater than string length
		//                          the char specified in this parameter will be added
		//                          to string until nPos.
		// Returns:
		//      TRUE if text successfully inserted, FALSE otherwise.
		// See Also: DeleteText
		//-----------------------------------------------------------------------
		BOOL InsertText(int nRow, int nPos, LPCTSTR pcszText,
						BOOL bGrowArrayIfNeed = FALSE,
						TCHAR chLeftSpaceFiller = _T(' ') );
		//-----------------------------------------------------------------------
		// Summary: Deletes text.
		// Parameters:
		//      nRow    - [in] String identifier.
		//      nPos    - [in] Start position in the string.
		//      nCount  - [in] Chars count to remove;
		// Returns:
		//      TRUE if text successfully deleted, FALSE otherwise.
		// See Also: InsertText
		//-----------------------------------------------------------------------
		BOOL DeleteText(int nRow, int nPos, int nCount);

		//----------------------------------------------------------------------
		// Summary:
		//      Gets string by given index.
		// Parameters:
		//      nRow : [in] id of string.
		//      bGrowArrayIfNeed : [in] Flag to grow array of strings.Default is TRUE
		// Returns:
		//      CString pointer.
		//----------------------------------------------------------------------
		CString* GetStrData(int nRow, BOOL bGrowArrayIfNeed = TRUE);

		//----------------------------------------------------------------------
		// Summary:
		//      Gets string by given index.
		// Parameters:
		//      nRow : [in] id of string.
		// Returns:
		//      CString pointer.
		//----------------------------------------------------------------------
		CString* GetStrDataC(int nRow) const;

		//--------------------------------------------------------------------
		// Summary:
		//          Internal class, implement collection of strings.
		//--------------------------------------------------------------------
		class _XTP_EXT_CLASS CStringPtrArray : public CArray<CString*, CString*>
		{
			//{{AFX_CODEJOCK_PRIVATE
			typedef CArray<CString*, CString*> TBase;
			//}}AFX_CODEJOCK_PRIVATE
		public:
			//----------------------------------------------------------------------
			// Summary:
			//      Default object constructor.
			//----------------------------------------------------------------------
			CStringPtrArray();

			//----------------------------------------------------------------------
			// Summary:
			//      Default object destructor. Handles cleanup and deallocation
			//----------------------------------------------------------------------
			virtual ~CStringPtrArray();

			//----------------------------------------------------------------------
			// Summary:
			//      This method used to delete all strings objects and remove them
			//      from array.
			//----------------------------------------------------------------------
			void RemoveAll();
		};

		CStringPtrArray m_arStrings; // String collection
	};

	CXTPSyntaxEditStringsManager    m_Strings; // A strings manager helper object

protected:


};

////////////////////////////////////////////////////////////////////////////
AFX_INLINE CXTPSyntaxEditLineMarksManager* CXTPSyntaxEditBufferManager::GetLineMarksManager() const {
	return m_pLineMarksManager;
}
AFX_INLINE XTPSyntaxEditLexAnalyser::CXTPSyntaxEditLexParser* CXTPSyntaxEditBufferManager::GetLexParser() const {
	return m_pLexParser;
}
AFX_INLINE CXTPSyntaxEditConfigurationManager* CXTPSyntaxEditBufferManager::GetLexConfigurationManager() const {
	return m_ptrLexConfigurationManager;
}
AFX_INLINE CXTPNotifyConnection* CXTPSyntaxEditBufferManager::GetConnection() const {
	return m_pConnection;
}
AFX_INLINE CXTPSyntaxEditUndoRedoManager* CXTPSyntaxEditBufferManager::GetUndoRedoManager() const {
	return m_pUndoRedoManager;
}
AFX_INLINE int CXTPSyntaxEditBufferManager::GetTabSize() const {
	return m_nTabSize;
}
AFX_INLINE BOOL CXTPSyntaxEditBufferManager::GetBuffer(const XTP_EDIT_LINECOL& lcStart,
					const XTP_EDIT_LINECOL& lcEnd,
					CMemFile& file,
					BOOL bColumnSelection,
					BOOL bForceDOSStyleCRLF)
{
	return GetBuffer(lcStart.nLine, lcStart.nCol, lcEnd.nLine, lcEnd.nCol,
					 file, bColumnSelection, bForceDOSStyleCRLF);
}
AFX_INLINE BOOL CXTPSyntaxEditBufferManager::GetOverwriteFlag() const {
	return m_bOverwrite;
}
AFX_INLINE int CXTPSyntaxEditBufferManager::GetCurCRLFType() const {
	return m_iCRLFStyle;
}
AFX_INLINE UINT CXTPSyntaxEditBufferManager::GetCodePage() const {
	return m_nCodePage;
}
AFX_INLINE CString CXTPSyntaxEditBufferManager::GetFileExt() const {
	return m_strFileExt;
}
AFX_INLINE BOOL CXTPSyntaxEditBufferManager::IsParserEnabled() const {
	return m_bIsParserEnabled;
}



////////////////////////////////////////////////////////////////////////////
#endif // !defined(__XTPSYNTAXEDITBUFFERMANAGER_H__)
