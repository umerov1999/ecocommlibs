// XTPMarkupParser.cpp: implementation of the CXTPMarkupParser class.
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
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

#include "stdafx.h"

#include "Common/XTPVc80Helpers.h"
#include "Common/XTPSystemHelpers.h"

#include "XTPMarkupParser.h"

// Based on code of Andrew Fedoniouk @ terrainformatica.com

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPMarkupParser::CXTPMarkupParser()
:   m_cInputChar(0),
	m_nValueLength(0),
	m_nTagNameLength(0),
	m_nAttributeNameLength(0),
	m_bGotTail(FALSE)
{
	m_lpszPos = NULL;
	m_lpszEnd = NULL;

	m_nLine = 0;
	m_nPosition = 0;

	m_bUnicode = FALSE;
	m_nEncoding = CP_ACP;

	m_lpszValue = new WCHAR[m_nValueAlloc = XTP_MAX_TOKEN_SIZE];

	m_scan = &CXTPMarkupParser::ScanBody;
}

CXTPMarkupParser::~CXTPMarkupParser()
{
	delete[] m_lpszValue;

}

void CXTPMarkupParser::SetBuffer(LPCSTR lpszStart, LPCSTR lpszEnd)
{
	m_lpszPos = lpszStart;
	m_lpszEnd = lpszEnd;
	m_bUnicode = FALSE;
}

void CXTPMarkupParser::SetBuffer(LPCWSTR lpszStart, LPCWSTR lpszEnd)
{
	m_lpszPos = (LPCSTR)lpszStart;
	m_lpszEnd = (LPCSTR)lpszEnd;
	m_bUnicode = TRUE;
}

CXTPMarkupParser::TokenType CXTPMarkupParser::GetNextToken()
{
	return (this->*m_scan)();
}

const WCHAR* CXTPMarkupParser::GetValue()
{
	m_lpszValue[m_nValueLength] = 0;
	return m_lpszValue;
}

const WCHAR* CXTPMarkupParser::GetAttributeName()
{
	m_lpszAttributeName[m_nAttributeNameLength] = 0;
	return m_lpszAttributeName;
}

const WCHAR* CXTPMarkupParser::GetTagName()
{
	m_lpszTagName[m_nTagNameLength] = 0;
	return m_lpszTagName;
}

CXTPMarkupParser::TokenType CXTPMarkupParser::ReportError(LPCWSTR lpszError)
{
	WCSNCPY_S(m_lpszValue, 1024, lpszError, 1024);
	m_nValueLength = (int)wcslen(m_lpszValue);

	return tokenError;
}

BOOL CXTPMarkupParser::FindFirstTag()
{
	WCHAR c = GetChar();

	while (c != 0)
	{
		if (c == '<')
		{
			PushBack(c);
			return TRUE;
		}
		else if (!IsWhitespace(c))
		{
			return FALSE;
		}
		c = GetChar();
	}

	return FALSE;
}

CXTPMarkupParser::TokenType CXTPMarkupParser::ScanBody()
{
	WCHAR c = GetChar();

	m_nValueLength = 0;

	BOOL ws = FALSE;

	if (c == 0) return tokenEof;
	else if (c == '<') return ScanTag();
	else if (c == '&')
		c = ScanEntity();
	else
		ws = IsWhitespace(c);

	while (TRUE)
	{
		AppendValue(c);
		c = GetNextChar();
		if (c == 0)  { PushBack(c); break; }
		if (c == '<') { PushBack(c); break; }
		if (c == '&') { PushBack(c); break; }

		if (IsWhitespace(c) != ws)
		{
			PushBack(c);
			break;
		}

	}
	return ws? tokenSpace : tokenWord;
}

CXTPMarkupParser::TokenType CXTPMarkupParser::ScanHead()
{
	WCHAR c = SkipWhitespace();

	if (c == '>') { m_scan = &CXTPMarkupParser::ScanBody; return ScanBody(); }
	if (c == '/')
	{
		WCHAR t = GetChar();
		if (t == '>')   { m_scan = &CXTPMarkupParser::ScanBody; return tokenTagEnd; }
		else { PushBack(t); return ReportError(L"Unexpected token. The expected token is '>'"); }
	}

	m_nAttributeNameLength = 0;
	m_nValueLength = 0;

	// attribute name...
	while (c != '=')
	{
		if ( c == 0) return tokenEof;
		if ( c == '>' ) return ReportError(L"'>' is an unexpected token. The expected token is '='");
		if ( IsWhitespace(c) )
		{
			c = SkipWhitespace();
			if (c != '=') return ReportError(L"Unexpected token. The expected token is '='");
			else break;
		}
		if ( c == '<') return ReportError(L"'<' is an unexpected token. The expected token is '='");
		AppendAttributeName(c);
		c = GetChar();
	}

	c = SkipWhitespace();
	// attribute m_lpszValue...

	if (c == '\"')
	{
		while ((c = GetChar()) != NULL)
		{
			if (c == '\"') return tokenAttribute;
			if (c == '&')
				c = ScanEntity();
			AppendValue(c);
		}
	}
	else if (c == '\'')
	{
		while ((c = GetChar()) != NULL)
		{
			if (c == '\'') return tokenAttribute;
			if (c == '&')
				c = ScanEntity();
			AppendValue(c);
		}
	}

	return ReportError(L"Unexpected token. The expected token is '\"' or '''");
}

// caller already consumed '<'
// scan header start or tag tail
CXTPMarkupParser::TokenType CXTPMarkupParser::ScanTag()
{
	m_nTagNameLength = 0;

	WCHAR c = GetChar();

	BOOL is_tail = c == '/';
	if (is_tail) c = GetChar();
	else if ( c == '?' )
	{
		m_scan = &CXTPMarkupParser::ScanPI;
		return tokenPIStart;
	}

	while (c)
	{
		if (IsWhitespace(c)) { c = SkipWhitespace(); break; }
		if (c == '/' || c == '>') break;
		AppendTagName(c);

		switch (m_nTagNameLength)
		{
		case 3:
			if (wcsncmp(m_lpszTagName, L"!--", 3) == 0)  { m_scan = &CXTPMarkupParser::ScanComment; return tokenCommentStart; }
			break;

		case 8:
			if ( wcsncmp(m_lpszTagName, L"![CDATA[", 8) == 0 ) { m_scan = &CXTPMarkupParser::ScanCData; return tokenCDataStart; }
			break;
		}

		c = GetChar();
	}

	if (c == 0) return ReportError(L"Unexpected end of file has occurred.");

	if (is_tail)
	{
		if (c == '>') return tokenTagEnd;
		return ReportError(L"Unexpected token. The expected token is '>'");
	}
	else
		PushBack(c);

	m_scan = &CXTPMarkupParser::ScanHead;
	return tokenTagStart;
}

// skip whitespaces.
// returns first non-whitespace WCHAR
WCHAR CXTPMarkupParser::SkipWhitespace()
{
	for (WCHAR c = GetChar(); c != 0; c = GetChar())
	{
		if (!IsWhitespace(c)) return c;
	}

	return 0;
}

void CXTPMarkupParser::PushBack(WCHAR c)
{
	m_cInputChar = c;
}

WCHAR CXTPMarkupParser::GetNextChar()
{
	if (m_lpszPos >= m_lpszEnd)
		return NULL;

	WCHAR c = 0;

	if (m_bUnicode)
	{
		c = *((LPCWSTR)m_lpszPos);
		m_lpszPos += sizeof(WCHAR);
	}
	else
	{
		char t = *m_lpszPos;

		if (m_nEncoding == CP_UTF8)
		{
			if ( 0 == ( t & '\x80' ) )
			{
				c = t;
			}
			else if ('\xF0' == (t & '\xF0')) // 1111 - error, more than 16-bit char
			{

			}
			else if ( '\xE0' == (t & '\xF0')) // 1110xxxx 10xxxxxx 10xxxxxx
			{
				char t2 = *(++m_lpszPos);
				char t3 = *(++m_lpszPos);

				c = (WCHAR)((WCHAR(t & '\x0F') << 12 ) | ( WCHAR(t2 & '\x3F' ) << 6 ) | WCHAR(t3 & '\x3F' ));
			}
			else if ( '\xC0' == (t & '\xE0')) // 110xxxxx 10xxxxxx
			{
				char t2 = *(++m_lpszPos);
				c = (WCHAR)((WCHAR( t & '\x1F' ) << 6 ) | ( t2 & '\x3F' ));
			}
			else
			{
			}
		}
		else if (XTPSystemVersion()->GetMaxCharSize() > 1 && _istlead(t))
		{
			MultiByteToWideChar(m_nEncoding, 0, m_lpszPos, 2, &c, 1);
			m_lpszPos++;
		}
		else if (t > 0 && t < 128)
		{
			c = t;
		}
		else
		{
			MultiByteToWideChar(m_nEncoding, 0, m_lpszPos, 1, &c, 1);
		}
		m_lpszPos++;
	}

	m_nPosition++;
	if (c == '\r' || c == '\n')
	{
		m_nLine++;
		m_nPosition = 0;
	}

	return c;
}

WCHAR CXTPMarkupParser::GetChar()
{
	if (m_cInputChar) { WCHAR t(m_cInputChar); m_cInputChar = 0; return t; }
	return GetNextChar();
}

WCHAR CXTPMarkupParser::ResolveEntity(const WCHAR* buf, int buf_size)
{
	if (buf[0] == '#')
	{
		int nAscii = 0;

		if (buf[1] == 'x' && buf_size > 2)
		{
			if (WSCANF_S(buf + 2, L"%x", &nAscii) != 1)
				return 0;

			return (WCHAR)nAscii;
		}
		else
		{
			if (WSCANF_S(buf + 1, L"%i", &nAscii) != 1)
				return 0;

			return (WCHAR)nAscii;
		}
	}

	return 0;
}

// caller consumed '&'
WCHAR CXTPMarkupParser::ScanEntity()
{
	WCHAR buf[32];
	int i = 0;
	WCHAR t;
	for (; i < 31 ; ++i )
	{
		t = GetChar();
		if (t == 0) return tokenEof;

		buf[i] = t;
		if (t == ';')
			break;
	}
	buf[i] = 0;
	if (i == 2)
	{
		if (wcsncmp(buf, L"gt", 2) == 0) return '>';
		if (wcsncmp(buf, L"lt", 2) == 0) return '<';
	}
	else if (i == 3 && (wcsncmp(buf, L"amp", 3) == 0))
		return '&';
	else if (i == 4)
	{
		if (wcsncmp(buf, L"apos", 4) == 0) return '\'';
		if (wcsncmp(buf, L"quot", 4) == 0) return '\"';
	}
	t = ResolveEntity(buf, i);
	if (t) return t;
	// no luck ...
	AppendValue('&');
	for (int n = 0; n < i; ++n)
		AppendValue(buf[n]);
	return ';';
}

BOOL CXTPMarkupParser::IsWhitespace(WCHAR c) const
{
	return c <= ' '
		&& (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f');
}

void CXTPMarkupParser::AppendValue(WCHAR c)
{
	if (m_nValueLength >= m_nValueAlloc - 1)
	{
		m_nValueAlloc *= 2;
		WCHAR* lpszValue = new WCHAR[m_nValueAlloc];
		MEMCPY_S(lpszValue, m_lpszValue, m_nValueLength * sizeof(WCHAR));

		delete[] m_lpszValue;
		m_lpszValue = lpszValue;
	}

	m_lpszValue[m_nValueLength++] = c;
}

void CXTPMarkupParser::AppendAttributeName(WCHAR c)
{
	if (m_nAttributeNameLength < (XTP_MAX_NAME_SIZE - 1))
		m_lpszAttributeName[m_nAttributeNameLength++] = c;
}

void CXTPMarkupParser::AppendTagName(WCHAR c)
{
	if (m_nTagNameLength < (XTP_MAX_NAME_SIZE - 1))
		m_lpszTagName[m_nTagNameLength++] = c;
}

CXTPMarkupParser::TokenType CXTPMarkupParser::ScanComment()
{
	if (m_bGotTail)
	{
		m_scan = &CXTPMarkupParser::ScanBody;
		m_bGotTail = FALSE;
		return tokenCommentEnd;
	}
	for (m_nValueLength = 0; m_nValueLength < (XTP_MAX_TOKEN_SIZE - 1); ++m_nValueLength)
	{
		WCHAR c = GetChar();
		if ( c == 0) return tokenEof;
		m_lpszValue[m_nValueLength] = c;

		if (m_nValueLength >= 2
			&& m_lpszValue[m_nValueLength] == '>'
			&& m_lpszValue[m_nValueLength - 1] == '-'
			&& m_lpszValue[m_nValueLength - 2] == '-')
		{
			m_bGotTail = TRUE;
			m_nValueLength -= 2;
			break;
		}
	}
	return tokenData;
}

CXTPMarkupParser::TokenType CXTPMarkupParser::ScanCData()
{
	if (m_bGotTail)
	{
		m_scan = &CXTPMarkupParser::ScanBody;
		m_bGotTail = FALSE;
		return tokenCDataEnd;
	}
	for (m_nValueLength = 0; m_nValueLength < (XTP_MAX_TOKEN_SIZE - 1); ++m_nValueLength)
	{
		WCHAR c = GetChar();
		if ( c == 0) return tokenEof;
		m_lpszValue[m_nValueLength] = c;

		if (m_nValueLength >= 2
			&& m_lpszValue[m_nValueLength] == '>'
			&& m_lpszValue[m_nValueLength - 1] == ']'
			&& m_lpszValue[m_nValueLength - 2] == ']')
		{
			m_bGotTail = TRUE;
			m_nValueLength -= 2;
			break;
		}
	}
	return tokenData;
}

CXTPMarkupParser::TokenType CXTPMarkupParser::ScanPI()
{
	if (m_bGotTail)
	{
		m_scan = &CXTPMarkupParser::ScanBody;
		m_bGotTail = FALSE;
		return tokenPIEnd;
	}
	for (m_nValueLength = 0; m_nValueLength < (XTP_MAX_TOKEN_SIZE - 1); ++m_nValueLength)
	{
		WCHAR c = GetChar();
		if ( c == 0)
			return tokenEof;

		if (IsWhitespace(c))
		{
			m_nValueLength--;
			continue;
		}

		m_lpszValue[m_nValueLength] = c;

		if (m_nValueLength >= 1
			&& m_lpszValue[m_nValueLength] == '>'
			&& m_lpszValue[m_nValueLength - 1] == '?')
		{
			m_bGotTail = TRUE;
			m_nValueLength -= 1;
			break;
		}
	}
	return tokenData;
}
