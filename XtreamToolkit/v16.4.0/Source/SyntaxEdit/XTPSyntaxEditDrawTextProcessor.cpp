// XTPSyntaxEditDrawTextProcessor.cpp: implementation of the CXTPSyntaxEditDrawTextProcessor class.
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
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

// common includes
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPSmartPtrInternalT.h"


#include "XTPSyntaxEditDrawTextProcessor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CXTPSyntaxEditDrawTextProcessor::CXTPSyntaxEditDrawTextProcessor()
{
	m_nTabSize = 4;
	ZeroMemory(&m_tmText, sizeof(m_tmText));
	m_nSpaceWidth = 6; // 6 px

	m_rcTextRect.SetRect(0, 0, 0, 0);
	m_nRowHeight = 16;
	m_nScrollXOffset = 0;

	m_nDrawingRow = -1;
	m_nNextRowPosX = 0;

	m_nPrintingRow = -1;
	m_ptNextPrintPos = CPoint(0, 0);
	m_bUseOutputDC = FALSE;

	m_arExpandCharsBuffer.SetDataSize(0, 1024, 1024);
}

int CXTPSyntaxEditDrawTextProcessor::RecalcRowHeight(CDC* pDC, CFont* pFont)
{
	ASSERT(pDC && pFont);
	if (!pDC || !pFont)
		return m_nRowHeight;

	m_bUseOutputDC = FALSE;

	CXTPFontDC fontDC(pDC, pFont);

	BOOL bRes = pDC->GetTextMetrics(&m_tmText);
	ASSERT(bRes);

	if (bRes)
		m_nRowHeight = m_tmText.tmHeight;

	if (pDC->IsPrinting())
	{
		TEXTMETRIC tmText;
		bRes = pDC->GetOutputTextMetrics(&tmText);
		ASSERT(bRes);
		if (bRes)
		{
			if (tmText.tmHeight > m_nRowHeight)
			{
				m_nRowHeight = tmText.tmHeight;
			}

			m_bUseOutputDC = //tmText.tmHeight > m_tmText.tmHeight ||
				tmText.tmAveCharWidth > m_tmText.tmAveCharWidth ||
				tmText.tmMaxCharWidth > m_tmText.tmMaxCharWidth;

			if (m_bUseOutputDC)
				m_tmText = tmText;
		}
	}

	if (m_bUseOutputDC)
		m_nSpaceWidth = pDC->GetOutputTextExtent(_T(" "), 1).cx;
	else
		m_nSpaceWidth = pDC->GetTextExtent(_T(" "), 1).cx;

	return m_nRowHeight;
}

CXTPSyntaxEditDrawTextProcessor::CXTPRowInfo* CXTPSyntaxEditDrawTextProcessor::GetRowInfo(int nRow) const
{
	if (nRow < 0)
		return NULL;

	if (nRow > GetRowsCount(TRUE))
		nRow = GetRowsCount(TRUE) + 10;

	if (nRow >= m_arRows.GetSize())
		m_arRows.SetSize(nRow + 100);

	CXTPRowInfo* pRI = m_arRows.GetAt(nRow, FALSE);
	if (!pRI)
	{
		pRI = new CXTPRowInfo();

		CXTPRowsInfoArray::TObjectPtr ptrRowI(pRI, FALSE);
		m_arRows.SetAt(nRow, ptrRowI);
	}
	return pRI;
}

void CXTPSyntaxEditDrawTextProcessor::ResetRowInfo(int nRow)
{
	m_nDrawingRow = nRow;
	m_nNextRowPosX = 0;

	m_nPrintingRow = -1;
	m_ptNextPrintPos = CPoint(0, 0);

	if (nRow < 0 || nRow >= m_arRows.GetSize())
	{
		return;
	}

	CXTPRowInfo* pRI = m_arRows.GetAt(nRow, FALSE);
	if (pRI)
		pRI->Reset();
}

int CXTPSyntaxEditDrawTextProcessor::AlignColIdxToTabs(int nRow, int nCol, BOOL bVirtualSpace)
{
	CXTPRowInfo* pRI = GetRowInfo(nRow);
	ASSERT(pRI);
	if (!pRI)
		return nCol;
	if (nCol >= 0 && nCol < pRI->arTabs.GetDataSize())
	{
		nCol += pRI->arTabs[nCol];
	}

	if (!bVirtualSpace && nCol > pRI->arCharsEnds.GetDataSize())
	{
		nCol = pRI->arCharsEnds.GetDataSize();
	}

	return nCol;
}

void CXTPSyntaxEditDrawTextProcessor::SetRowTabPositions(int nRow, LPCTSTR pcszOrigRowText)
{
	CXTPRowInfo* pRowI = GetRowInfo(nRow);
	ASSERT(pRowI);
	if (!pRowI)
		return;

	pRowI->arTabs.RemoveAll();
	pRowI->arDispCol2StrPos.RemoveAll();
	pRowI->arStrPos2DispCol.RemoveAll();

	int nDispPos = 0;
	int nStrPos = 0;
	for (LPCTSTR p = pcszOrigRowText; p && *p != 0; p = _tcsinc(p), nStrPos++)
	{
		pRowI->arStrPos2DispCol.AddData(nDispPos);

		if (*p == _T('\t'))
		{
			int nSpaces = m_nTabSize - (nDispPos % m_nTabSize);
			for (int i = 0; i < nSpaces; i++)
			{
				if (i == 0)
				{
					pRowI->arTabs.AddData(0);
					pRowI->arDispCol2StrPos.AddData(nStrPos);
				}
				else
				{
					pRowI->arTabs.AddData((BYTE)(nSpaces - i));
					//pRowI->arDispCol2StrPos.AddData(nStrPos + 1);
//Corrected, only with the next real character we have the next nStrPos.
					pRowI->arDispCol2StrPos.AddData(nStrPos);
				}
			}
			nDispPos += nSpaces;
		}
		else
		{
#ifdef XTP_FIXED
// multi-byte character incorrect.
			for (int index = 0; index < ( isleadbyte(*p) != 0 ? 2 : 1 ); index ++)
			{
				pRowI->arTabs.AddData(0);
				pRowI->arDispCol2StrPos.AddData(nStrPos);
				nDispPos++;
			}
#else
			pRowI->arTabs.AddData(0);
			pRowI->arDispCol2StrPos.AddData(nStrPos);
			nDispPos++;
#endif
		}
	}
	pRowI->arStrPos2DispCol.AddData(nDispPos);
	pRowI->arDispCol2StrPos.AddData(nStrPos);
}

int CXTPSyntaxEditDrawTextProcessor::DispPosToStrPos(int nRow, int nDispPos, BOOL bVirtualSpace) const
{
	CXTPRowInfo* pRowI = GetRowInfo(nRow);
	ASSERT(pRowI);
	if (!pRowI)
		return 0;

	ASSERT(nDispPos >= 0);
	if (nDispPos < 0)
		return 0;

	int nCount = pRowI->arDispCol2StrPos.GetDataSize();
	if (nDispPos >= nCount)
	{
		int nStrPos = nCount > 0 ? pRowI->arDispCol2StrPos[nCount - 1] : 0;
		if (bVirtualSpace)
		{
			int nCount2 = pRowI->arStrPos2DispCol.GetDataSize();
			ASSERT(nStrPos <= nCount2);

			int nStrPos2 = min(nStrPos, nCount2 - 1);
			int nLastDispPos = (nStrPos2 >= 0) ? (pRowI->arStrPos2DispCol[nStrPos2]) : nCount;
			ASSERT(nDispPos >= nLastDispPos);

			nStrPos += nDispPos - nLastDispPos /* + 1*/;
		}
		else
		{
			nStrPos++;
		}
		return nStrPos;
	}

	return pRowI->arDispCol2StrPos[nDispPos];
}

int CXTPSyntaxEditDrawTextProcessor::StrPosToDispPos(int nRow, int nStrPos, BOOL bVirtualSpace) const
{
	CXTPRowInfo* pRowI = GetRowInfo(nRow);
	ASSERT(pRowI);
	if (!pRowI)
		return 0;

	ASSERT(nStrPos >= 0);
	if (nStrPos < 0)
		return 0;

	int nCount = pRowI->arStrPos2DispCol.GetDataSize();
	if (nStrPos >= nCount)
	{
		int nDispPos = nCount > 0 ? pRowI->arStrPos2DispCol[nCount - 1] + 1 : 0;
		if (bVirtualSpace)
		{
			nDispPos += nStrPos - nCount + 1;
		}
		return nDispPos;
	}

	return pRowI->arStrPos2DispCol[nStrPos];
}

int CXTPSyntaxEditDrawTextProcessor::ExpandChars(LPCTSTR pszChars, CString& strBuffer,
									  int nDispPos, BOOL bEnableWhiteSpace)
{
	m_arExpandCharsBuffer.RemoveAll();

	for (LPCTSTR p = pszChars; p && *p != 0; p = _tcsinc(p))
	{
		if (*p == _T('\t'))
		{
			int nSpaces = m_nTabSize - (nDispPos % m_nTabSize);

			BOOL bFirstWhiteSpaceChar = bEnableWhiteSpace;
			for (int i = 0; i < nSpaces; i++)
			{
				if (bFirstWhiteSpaceChar)
				{
					//strBuffer += (TCHAR)(unsigned char)0xBB;
					m_arExpandCharsBuffer.AddData((TCHAR)(unsigned char)0xBB);
					bFirstWhiteSpaceChar = FALSE;
				}
				else
				{
					//strBuffer += _T(' ');
					m_arExpandCharsBuffer.AddData(_T(' '));
				}
			}
			nDispPos += nSpaces;
		}
		else
		{
			if (bEnableWhiteSpace && *p == _T(' '))
				m_arExpandCharsBuffer.AddData((TCHAR)(unsigned char)0xB7); //strBuffer += (TCHAR)(unsigned char)0xB7;
			else
			{
				m_arExpandCharsBuffer.AddData(*p); //strBuffer += *p;

				#ifndef _UNICODE
				if (_tcsinc(p) > p + 1)
				{
					ASSERT(_tcsinc(p) == p + 2);
					m_arExpandCharsBuffer.AddData(*(p+1));
				}
				#endif
			}
#ifdef XTP_FIXED
// multi-byte character : display length  2
// single-byte character : display length 1
			if (isleadbyte( *p ))
				nDispPos += 2;
			else
				nDispPos++;
#else
			nDispPos++;
#endif
		}
	}
	m_arExpandCharsBuffer.AddData(_T('\0'));

	strBuffer = m_arExpandCharsBuffer.GetData();

	return nDispPos;
}


int CXTPSyntaxEditDrawTextProcessor::DrawRowPart(CDC* pDC, int nRow, LPCTSTR pcszText, int nchCount)
{
	if (m_nDrawingRow != nRow)
	{
		if (nRow >= 0) // the new row started
			ResetRowInfo(nRow);

		m_nDrawingRow = nRow;
		m_nNextRowPosX = 0;
	}

	if (nRow == -1)
		return 0;


	//--------------------------------------------------------
	int nTextLen = 0;

	if (nchCount < 0)
	{
		nchCount = (int) _tcsclen(pcszText);
		nTextLen = (int) _tcslen(pcszText);
	}
	else
	{
#ifdef XTP_FIXED
// "nchCount" meen byte length.
// so, need not translate form TextLength to ByteLength.
		nTextLen = nchCount;
#else
		nTextLen = (int) _tcsnbcnt(pcszText, nchCount);
#endif
	}
//  ASSERT(nchCount <= (int) _tcsclen(pcszText));

	//--------------------------------------------------------
	int nY = m_rcTextRect.top + nRow * m_nRowHeight;
	int nX = m_rcTextRect.left - m_nScrollXOffset + m_nNextRowPosX;

	CRect rcText = m_rcTextRect;
	rcText.left = max(rcText.left, nX); // ENSURE THE TEXT IS CLIPPED APPROPRIATELY
	pDC->ExtTextOut(nX, nY, ETO_CLIPPED, &rcText, pcszText, nTextLen, NULL);

	//--------------------------------------------------------
	if (m_arBuf_aDx.GetSize() < nTextLen)
		m_arBuf_aDx.SetSize(nTextLen + 100);

	int nMaxExtent = m_rcTextRect.Width() + m_nScrollXOffset - m_nNextRowPosX + 30; // 30 is a gap to be sure
	nMaxExtent = max(nMaxExtent, 30);

	int nFit = 0;
	LPINT alpDx = (int*)m_arBuf_aDx.GetData();
	CSize szText(0, 0);

	BOOL bResExt = GetTextExtentExPoint(pDC->m_hAttribDC, pcszText, nTextLen,
										0,      // nMaxExtent,
										NULL,   // &nFit,
										alpDx,  // array of partial string widths
										&szText // string dimensions
									);
	nFit = nTextLen;

	VERIFY(bResExt);

	DrawTextOfSize(pDC, nX, nY, szText, rcText, pcszText, nTextLen);

	CXTPRowInfo* pRI = GetRowInfo(nRow);
	if (pRI)
	{
		if (nFit >= 0 && nFit <= nTextLen)
		{
			int i;

			LPCTSTR p = pcszText;
			LPCTSTR p_prev = pcszText;
			for (i = 0; i < nFit; i++)
			{
				pRI->arCharsEnds.AddData(m_nNextRowPosX + alpDx[i]);

				p_prev = p;
				p = _tcsinc(p);
				int nCharLen = int(p - p_prev);
#ifdef XTP_FIXED
				if (nCharLen > 1)
					pRI->arCharsEnds.AddData(m_nNextRowPosX + alpDx[i]);
				// ASSERT( nCharLen > 2 );
#endif
				i += nCharLen - 1; // skip second byte for MBCS chars.
			}
		}
		else
		{
			ASSERT(FALSE);
		}

		pRI->nMaxWidth += szText.cx;
	}

	m_nNextRowPosX += szText.cx;

	return m_nNextRowPosX;
}

void CXTPSyntaxEditDrawTextProcessor::DrawTextOfSize(
	CDC* pDC,
	int nX, int nY,
	const SIZE& textSize,
	const RECT& rcText,
	LPCTSTR pcszText,
	int nchCount) const
{
	const int MaxTTGridSize = 16384;
	if(textSize.cx < MaxTTGridSize)
	{
		pDC->ExtTextOut(nX, nY, ETO_CLIPPED, &rcText, pcszText, nchCount, NULL);
	}
	else
	{
		int nLeftPartLength = nchCount / 2;
		int nRightPartLength = nchCount - nLeftPartLength;

		// Draw left part.
		CSize leftPartSize = pDC->GetTextExtent(pcszText, nLeftPartLength);
		DrawTextOfSize(pDC, nX, nY, leftPartSize, rcText, pcszText, nLeftPartLength);

		// Draw right part.
		CSize rightPartSize = pDC->GetTextExtent(pcszText + nLeftPartLength, nRightPartLength);
		DrawTextOfSize(pDC, nX + leftPartSize.cx, nY, rightPartSize,
			rcText, pcszText + nLeftPartLength, nRightPartLength);
	}
}

int CXTPSyntaxEditDrawTextProcessor::PrintRowPart(CDC* pDC, int nRow, int nPosY, UINT nFlags,
												  LPCTSTR pcszText, int nchCount, int *pnPrintedTextLen)
{
	if (pnPrintedTextLen)
		*pnPrintedTextLen = 0;

	if (m_nPrintingRow != nRow)
	{
		m_nPrintingRow = nRow;
		m_ptNextPrintPos = CPoint(0, 0);
	}

	if (nRow == -1)
		return 0;

	//--------------------------------------------------------
	int nTextLen = 0;

	if (nchCount < 0)
	{
		nchCount = (int)_tcsclen(pcszText);
		nTextLen = (int)_tcslen(pcszText);
	}
	else
	{
		nTextLen = (int)_tcsnbcnt(pcszText, nchCount);
	}
	ASSERT(nchCount <= (int)_tcsclen(pcszText));

	//--------------------------------------------------------
	if (m_arBuf_aDx.GetSize() < nTextLen)
		m_arBuf_aDx.SetSize(nTextLen + 100);

	LPINT alpDx = (int*)m_arBuf_aDx.GetData();

	//--------------------------------------------------------
	CRect rcText = m_rcTextRect;
	int nTextLen_rest = nTextLen;
	LPCTSTR pTxt = NULL;

	for (pTxt = pcszText; nTextLen_rest;)
	{
		int nY = m_rcTextRect.top + nPosY + m_ptNextPrintPos.y;
		int nX = m_rcTextRect.left + m_ptNextPrintPos.x;

		if (nY + GetRowHeight() > m_rcTextRect.bottom)
			break;

		//--------------------------------------------------------
		int nMaxExtent = max(0, rcText.right - nX - 20);
		int nFit = 0;

		CSize szText(0, 0);

		BOOL bResExt = GetTextExtentExPoint(
						m_bUseOutputDC ? pDC->m_hDC : pDC->m_hAttribDC,  // HDC hdc, // handle to DC
						pTxt,               // LPCTSTR lpString,         // character string
						nTextLen_rest,      // int nCount,               // number of characters
						nMaxExtent,         // maximum extent for string
						&nFit,              // LPINT lpnFit,    // maximum number of characters
						alpDx,     // array of partial string widths
						&szText //LPSIZE lpSize    // string dimensions
					);
		VERIFY(bResExt);

		//nFit = min(nFit, nTextLen_rest);

		LPCTSTR pTextToDtaw = pTxt;
		BOOL bAllTextDrawn = (nFit == nTextLen_rest);

		if (nFit > 0 && nFit <= nTextLen_rest)
		{
			CString strSeps = _T(" .,!?)-+=;\\"); //_T(" .,!?)-+=*&^%$#@~`:;\\|/");
			int nFit_wb = 0;
			int nTextLen_wb = nTextLen_rest;
			LPCTSTR pTxt_wb = pTxt;

			LPCTSTR p_prev = NULL;
			int i = 0;
			for (i = 0; i < nFit; i++)
			{
				p_prev = pTxt;
				pTxt = _tcsinc(pTxt);
				int nCharLen = int(pTxt - p_prev);
				i += nCharLen - 1; // skip second byte for MBCS chars.

				nTextLen_rest--;

				if ((nFlags & DT_WORDBREAK) && strSeps.Find(*p_prev, 0) >= 0)
				{
					nFit_wb = i+1;
					nTextLen_wb = nTextLen_rest;
					pTxt_wb = pTxt;
				}
			}

			if (!bAllTextDrawn && (nFlags & DT_WORDBREAK))
			{
				if (nFit_wb >= 0)
				{
					nFit = nFit_wb;
					nTextLen_rest = nTextLen_wb;
					pTxt = pTxt_wb;
				}
			}

			if (nFit)
			{
				m_ptNextPrintPos.x += alpDx[nFit - 1] + 1;
			}
		}

		// Draw text (if need)
		if (nFit && (nFlags & DT_CALCRECT) == 0)
		{
			rcText.left = max(rcText.left, nX); // ENSURE THE TEXT IS CLIPPED APPROPRIATELY
			pDC->ExtTextOut(nX, nY, ETO_CLIPPED, &rcText, pTextToDtaw, nFit, NULL);
		}

		CXTPRowInfo* pRI = GetRowInfo(nRow);
		if (pRI)
			pRI->nMaxWidth = m_ptNextPrintPos.x;

		// Move point to the next line (if need)
		if (!bAllTextDrawn && (nFlags & DT_SINGLELINE) == 0)
		{
			int nIconX = m_rcTextRect.left + m_ptNextPrintPos.x + 3;
			int nIconY = m_rcTextRect.top + nPosY + m_ptNextPrintPos.y;

			int nDelta = GetRowHeight() - GetRowHeight()/4;

			CPen pen1(PS_SOLID, 1, RGB(0, 0, 0));
			CPen pen2(PS_SOLID, 2, RGB(0, 0, 0));

			CPen* pPen_old = pDC->SelectObject(&pen1);

			pDC->MoveTo(nIconX, nIconY + nDelta/2);
			pDC->LineTo(nIconX, nIconY + nDelta);
			pDC->LineTo(nIconX + nDelta/2 - 1, nIconY + nDelta);

			pDC->SelectObject(&pen2);
			pDC->MoveTo(nIconX + 1, nIconY + nDelta - 1);
			pDC->LineTo(nIconX + nDelta/2, nIconY + nDelta/2);

			pDC->SelectObject(pPen_old);

			int nSublineOffsetX = m_tmText.tmAveCharWidth * 3;
			m_ptNextPrintPos.y += GetRowHeight();
			m_ptNextPrintPos.x = nSublineOffsetX;

			if (pRI)
				pRI->nMaxWidth = m_ptNextPrintPos.x;
		}
		else
		{
			break;
		}
	}

	if (pnPrintedTextLen)
	{
		int nLen0 = int(pTxt - pcszText);
		*pnPrintedTextLen = (int)_tcsnccnt(pcszText, nLen0);
	}

	return m_ptNextPrintPos.y + GetRowHeight();
}

int CXTPSyntaxEditDrawTextProcessor::GetColPosX(int nRow, int nCol, int* pnChawWidth,
												 BOOL bVirtualSpace) const
{
	CXTPRowInfo* pRI = GetRowInfo(nRow);
	if (!pRI)
		return 0;

	int nPosX = 0;
	if (pnChawWidth)
		*pnChawWidth = 0;

	int nColsCount = pRI->arCharsEnds.GetDataSize();

	if (nCol <= 0)
	{
		nPosX = 0;
	}
	else if (nCol < nColsCount)
	{
		nPosX = pRI->arCharsEnds[nCol - 1];
		if (pnChawWidth)
			*pnChawWidth = pRI->arCharsEnds[nCol] - pRI->arCharsEnds[nCol - 1];
	}
	else
	{
		if (nColsCount)
			nPosX = pRI->arCharsEnds[nColsCount - 1];

		if (bVirtualSpace)
		{
			ASSERT(m_nSpaceWidth);

			nPosX += m_nSpaceWidth * (nCol - nColsCount);
		}

		if (pnChawWidth)
			*pnChawWidth = m_nSpaceWidth;
	}

	nPosX = m_rcTextRect.left - m_nScrollXOffset + nPosX;

	return nPosX;
}

BOOL CXTPSyntaxEditDrawTextProcessor::ColFromXPos(int nRow, int nX, int& rnCol, BOOL bVirtualSpace) const
{
	rnCol = 0;

	CXTPRowInfo* pRI = GetRowInfo(nRow);
	ASSERT(pRI);
	if (!pRI)
		return FALSE;

	int nX2 = nX - m_rcTextRect.left + m_nScrollXOffset;
	if (nX2 < 0)
		return FALSE;

	int nCharsCount = pRI->arCharsEnds.GetDataSize();
	int nPrevCharEnd = 0;

	for (int i = 0; i < nCharsCount; i++)
	{
		int nCharEnd = pRI->arCharsEnds[i];
		if (nX2 >= nPrevCharEnd && nX2 <= nCharEnd)
		{
			int nMiddle = nPrevCharEnd + (nCharEnd - nPrevCharEnd) / 2;
			rnCol = i + ((nX2 < nMiddle) ? 0 : 1);
			return TRUE;
		}
		nPrevCharEnd = nCharEnd;
	}

	rnCol = nCharsCount;

	ASSERT(m_nSpaceWidth);
	ASSERT(nX2 >= nPrevCharEnd);
	int nFreeWidth = nX2 - nPrevCharEnd;

	if (bVirtualSpace)
	{
		rnCol = nCharsCount + nFreeWidth / max(1, m_nSpaceWidth);
	}

	return FALSE;
}

BOOL CXTPSyntaxEditDrawTextProcessor::HitTest(const CPoint& pt, int& rnRow, int& rnCol, BOOL bVirtualSpace) const
{
	if (!m_rcTextRect.PtInRect(pt))
		return FALSE;

	VERIFY(HitTestRow(pt.y, rnRow));

	ColFromXPos(rnRow, pt.x, rnCol, bVirtualSpace);

	return TRUE;
}

BOOL CXTPSyntaxEditDrawTextProcessor::HitTestRow(int nY, int& rnRow) const
{
	if (nY < m_rcTextRect.top || nY >= m_rcTextRect.bottom)
		return FALSE;

	int nY2 = nY - m_rcTextRect.top;
	rnRow = nY2 / max(1, m_nRowHeight);

	return TRUE;
}

CPoint CXTPSyntaxEditDrawTextProcessor::SetCaretByPoint(CWnd* pWnd, const CPoint& pt, const CSize& szSize,
						 int& rnRow, int& rnCol, BOOL bVirtualSpace)
{
	BOOL bHitRes = HitTest(pt, rnRow, rnCol, bVirtualSpace);
	VERIFY(bHitRes);

	return SetCaretPos(pWnd, szSize, rnRow, rnCol);
}

CPoint CXTPSyntaxEditDrawTextProcessor::SetCaretPos(CWnd* pWnd, const CSize& szSize,
						 int nRow, int& rnCol, BOOL bHideCaret, BOOL bVirtualSpace)
{
	rnCol = AlignColIdxToTabs(nRow, rnCol, bVirtualSpace);

	int nRowY = m_rcTextRect.top + nRow * m_nRowHeight;
	int nCaretX = GetColPosX(nRow, rnCol, NULL, bVirtualSpace);

	CPoint ptCaret(nCaretX, nRowY);

	if (m_rcTextRect.PtInRect(ptCaret) && !bHideCaret)
	{
		pWnd->CreateSolidCaret(szSize.cx, szSize.cy);
		pWnd->SetCaretPos(CPoint(nCaretX, nRowY));

		pWnd->ShowCaret();
	}
	else
	{
		pWnd->HideCaret();
	}

	return ptCaret;
}

void CXTPSyntaxEditDrawTextProcessor::SetScrollXOffset(int nOffsetX)
{
	m_nScrollXOffset = nOffsetX;
}

int CXTPSyntaxEditDrawTextProcessor::GetRowWidth(int nRow) const
{
	CXTPRowInfo* pRI = GetRowInfo(nRow);
	ASSERT(pRI);
	if (pRI)
		return pRI->nMaxWidth;

	return 0;
}

int CXTPSyntaxEditDrawTextProcessor::GetRowsMaxWidth() const
{
	int nMaxWidth = 0;

	int nRowsCount = GetRowsCount(TRUE);
	for (int i = 0; i < nRowsCount; i++)
	{
		CXTPRowInfo* pRI = GetRowInfo(i);
		if (pRI && pRI->nMaxWidth > nMaxWidth)
		{
			nMaxWidth = pRI->nMaxWidth;
		}
	}

	return nMaxWidth;
}
