// XTPMarkupParser.h: interface for the CXTPMarkupParser class.
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
#if !defined(__XTPMARKUPPARSER_H__)
#define __XTPMARKUPPARSER_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//{{AFX_CODEJOCK_PRIVATE

// Internal class used to parse XAML text to Markup tree

#define XTP_MAX_TOKEN_SIZE 1024
#define XTP_MAX_NAME_SIZE 128


class _XTP_EXT_CLASS CXTPMarkupParser
{
public:
	CXTPMarkupParser();
	virtual ~CXTPMarkupParser();

public:
	enum TokenType
	{
		tokenError = -1,
		tokenEof = 0,
		tokenTagStart,
		tokenTagEnd,
		tokenAttribute,
		tokenWord,
		tokenSpace,
		tokenData,
		tokenCommentStart,
		tokenCommentEnd,
		tokenCDataStart,
		tokenCDataEnd,
		tokenPIStart,
		tokenPIEnd
	};

	void SetBuffer(LPCSTR lpszStart, LPCSTR lpszEnd);
	void SetBuffer(LPCWSTR lpszStart, LPCWSTR lpszEnd);

public:
	TokenType GetNextToken();
	const WCHAR* GetValue();
	const WCHAR* GetAttributeName();
	const WCHAR* GetTagName();

	// should be overrided to resolve entities, e.g. &nbsp;
	virtual WCHAR ResolveEntity(const WCHAR* buf, int buf_size);

	int GetLine() const;
	int GetPosition() const;

	BOOL FindFirstTag();
	void SetEncoding(int nEncoding);

private:

	typedef TokenType (CXTPMarkupParser::*PFNSCANNEXTTOKEN)();
	PFNSCANNEXTTOKEN m_scan;

	// content 'readers'
	TokenType ScanBody();
	TokenType ScanHead();
	TokenType ScanComment();
	TokenType ScanCData();
	TokenType ScanPI();
	TokenType ScanTag();

	WCHAR SkipWhitespace();
	void PushBack(WCHAR c);

	WCHAR GetChar();
	WCHAR ScanEntity();
	BOOL IsWhitespace(WCHAR c) const;

	void AppendValue(WCHAR c);
	void AppendAttributeName(WCHAR c);
	void AppendTagName(WCHAR c);

	WCHAR GetNextChar();

	TokenType ReportError(LPCWSTR lpszError);

private:

	TokenType  m_token;

	WCHAR* m_lpszValue;
	int m_nValueAlloc;
	int m_nValueLength;

	WCHAR m_lpszTagName[XTP_MAX_NAME_SIZE];
	int m_nTagNameLength;

	WCHAR m_lpszAttributeName[XTP_MAX_NAME_SIZE];
	int m_nAttributeNameLength;

	WCHAR m_cInputChar;
	BOOL m_bGotTail;

	int m_nLine;
	int m_nPosition;

private:
	BOOL m_bUnicode;
	int m_nEncoding;
	LPCSTR m_lpszEnd;
	LPCSTR m_lpszPos;
};

AFX_INLINE int CXTPMarkupParser::GetLine() const {
	return m_nLine;
}
AFX_INLINE int CXTPMarkupParser::GetPosition() const {
	return m_nPosition;
}
AFX_INLINE void CXTPMarkupParser::SetEncoding(int nEncoding) {
	m_nEncoding = nEncoding;
}
//}}AFX_CODEJOCK_PRIVATE

#endif // !defined(__XTPMARKUPPARSER_H__)
