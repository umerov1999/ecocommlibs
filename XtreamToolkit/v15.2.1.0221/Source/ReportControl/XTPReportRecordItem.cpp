// XTPReportRecordItem.cpp : implementation of the CXTPReportRecordItem class.
//
// This file is a part of the XTREME REPORTCONTROL MFC class library.
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

#include "StdAfx.h"
#include <locale.h>

#include "Common/XTPDrawHelpers.h"
#include "Common/XTPMarkupRender.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPPropExchange.h"
#include "Common/XTPCustomHeap.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPColorManager.h"

#include "XTPReportDefines.h"
#include "XTPReportRow.h"
#include "XTPReportRecords.h"
#include "XTPReportColumns.h"
#include "XTPReportColumn.h"
#include "XTPReportControl.h"
#include "XTPReportPaintManager.h"
#include "XTPReportHyperlink.h"
#include "XTPReportRecord.h"
#include "XTPReportRecordItem.h"
#include "XTPReportRecordItemConstraint.h"
#include "XTPReportInplaceControls.h"
#include "XTPReportRecordItemControls.h"
#include "XTPReportBorder.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef IDC_HAND
#define IDC_HAND MAKEINTRESOURCE(32649)
#endif

//////////////////////////////////////////////////////////////////////////
// XTP_REPORTRECORDITEM_ARGS

XTP_REPORTRECORDITEM_ARGS::XTP_REPORTRECORDITEM_ARGS()
{
	pControl = NULL;
	pRow = NULL;
	pColumn = NULL;
	pItem = NULL;
	rcItem.SetRectEmpty();
}

XTP_REPORTRECORDITEM_ARGS::XTP_REPORTRECORDITEM_ARGS(
	CXTPReportControl *pControl,
	CXTPReportRow     *pRow,
	CXTPReportColumn  *pColumn)
{
	ASSERT(pControl && pRow && pColumn);

	this->pControl = pControl;
	this->pRow     = pRow;
	this->pColumn  = pColumn;
	this->pItem    = pRow->GetRecord()->GetItem(pColumn);
	this->rcItem   = pRow->GetItemRect(pItem);
}

XTP_REPORTRECORDITEM_ARGS::~XTP_REPORTRECORDITEM_ARGS()
{

}

void XTP_REPORTRECORDITEM_ARGS::AddRef()
{
	CMDTARGET_ADDREF(pRow);
	CMDTARGET_ADDREF(pColumn);
	CMDTARGET_ADDREF(pItem);

}

void XTP_REPORTRECORDITEM_ARGS::Release()
{
	if (pRow)
		pRow->InternalRelease();

	if (pColumn)
		pColumn->InternalRelease();

	if (pItem)
		pItem->InternalRelease();
}

const XTP_REPORTRECORDITEM_ARGS& XTP_REPORTRECORDITEM_ARGS::operator=(const XTP_REPORTRECORDITEM_ARGS& src)
{
	pControl = src.pControl;
	pRow     = src.pRow;
	pColumn  = src.pColumn;
	pItem    = src.pItem;
	rcItem   = src.rcItem;

	return *this;
}

CXTPReportInplaceButtons::CXTPReportInplaceButtons()
{

}



//////////////////////////////////////////////////////////////////////////
// CXTPReportRecordItem


IMPLEMENT_SERIAL(CXTPReportRecordItem, CCmdTarget, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPReportRecordItem::CXTPReportRecordItem()
	: m_pMergeItem(NULL)
	, m_checkState(xtpReportCheckStateUnchecked)

	, m_pControl(NULL)
	, m_pRecord (NULL)
	, m_pBorder (NULL)
{
	static const CString g_strFormatStringDef = _T("%s");

	m_strFormatString = g_strFormatStringDef;
	m_strFormula.Empty();

	m_bEditable = TRUE;
	m_pFontCaption = NULL;
	m_bBoldText = FALSE;

	m_clrText = XTP_REPORT_COLOR_DEFAULT;
	m_clrBackground = XTP_REPORT_COLOR_DEFAULT;

	m_nIconIndex = XTP_REPORT_NOICON;

	m_dwData = 0;

	m_nSortPriority = -1;
	m_nGroupPriority = -1;

	m_Alignment = (XTPReportColumnIconAlignment)(-1);

	m_bHasCheckbox = FALSE;
	m_bCanEditCheckboxItem = FALSE;
	m_bTristateCheckbox = FALSE;
	m_pEditOptions = NULL;
	m_pItemControls = NULL;
	m_pFocusedItemControl = NULL;
	m_bItemControlUnderMouse = FALSE;
	m_pItemControlHookWnd = NULL;

	m_bFocusable = TRUE;

	m_pHyperlinks = NULL;

	m_pMarkupUIElement = NULL;

	m_rcGlyph.SetRectEmpty();
}

CXTPReportRecordItem::~CXTPReportRecordItem()
{
	if (m_pItemControlHookWnd)
	{
		m_pItemControlHookWnd->DestroyWindow();
		delete m_pItemControlHookWnd;
		m_pItemControlHookWnd = NULL;
	}

	CMDTARGET_RELEASE(m_pEditOptions);
	CMDTARGET_RELEASE(m_pItemControls);
	CMDTARGET_RELEASE(m_pHyperlinks);

	XTPMarkupReleaseElement(m_pMarkupUIElement);
}

double CXTPReportRecordItem::StringToDouble(CString strText)
{
	TCHAR* locale = _tsetlocale(LC_NUMERIC, NULL);

	if (locale == NULL || _tcscmp(locale, _T("C")) == 0) // Not set
	{
		strText.Replace(',', '.');
	}

	double res = atof(XTP_CT2CA(strText));

	return res;
}


void CXTPReportRecordItem::ParseBBCode(CString &strText)
{
	GetHyperlinks()->RemoveAll();

	//////////////////////////////////////////////////////////////////////////
	// Left [url=address]name[/url] Right
	//////////////////////////////////////////////////////////////////////////

	const CString sUrlTagOpen  = _T("[url");
	const CString sUrlTagClose = _T("[/url]");

	int nOpenTagStart = strText.Find(sUrlTagOpen);
	int nOpenTagEnd;

	int nCloseTagStart;
	int nCloseTagEnd;

	while (-1 != nOpenTagStart)
	{
		nOpenTagEnd = strText.Find(_T("]"), nOpenTagStart);
		CString sLeft = strText.Left(nOpenTagStart);

		nCloseTagStart = strText.Find(sUrlTagClose);
		CString sName  = strText.Mid(nOpenTagEnd+1, nCloseTagStart-nOpenTagEnd-1);

		nCloseTagEnd = nCloseTagStart + sUrlTagClose.GetLength();
		CString sRight = strText.Right(strText.GetLength()-nCloseTagEnd);

		strText = sLeft + sName + sRight;

		GetHyperlinks()->Add(new CXTPReportHyperlink(nOpenTagStart, nCloseTagStart-nOpenTagEnd-1));

		nOpenTagStart = strText.Find(sUrlTagOpen);
	}
}

void CXTPReportRecordItem::SetCaption(LPCTSTR lpszCaption)
{
	CString strCaption(lpszCaption);

	if (!m_strFormula.IsEmpty()
		&& m_pRecord != NULL)
	{
		COLORREF clrSum = RGB(255,0,0);
		COLORREF clrSubSum = RGB(0,0,255);
		if (m_pControl)
		{
			clrSum = m_pControl->GetPaintManager()->m_clrFormulaSum;
			clrSubSum = m_pControl->GetPaintManager()->m_clrFormulaSubSum;
		}
//CODE FOR RECALC
		double d(0.0);
		int m(0), M(0), n(0), N(0);
		CString s, t, u, v;
		CString sFormula(m_strFormula);
		int pos = sFormula.Find(_T("SUMSUB("));
		//pattern SUMSUB(R#C#:R#C#) or * instead of # - means all row or column
		if (pos > -1 && m_pRecord->GetChilds() != NULL)
		{
			int mMax = m_pRecord->GetChilds()->GetCount();
			int nMax = m_pRecord->GetItemCount();
			sFormula.Replace(_T(" "), _T(""));
			sFormula = sFormula.Mid(pos + 7);
			pos = sFormula.Find(_T(":"));
			if (pos > -1)
			{
				s = sFormula.Left(pos);
				t = sFormula.Mid(pos + 1);
				pos = s.Find(_T("C"));
				if (pos > -1)
				{
					u = s.Left(pos);
					s = s.Mid(pos + 1);
					u.Replace(_T("R"), _T(""));
					if (u == _T("*"))
						m = 0;
					else
						m = _ttoi(u);
					if (s == _T("*"))
						n = 0;
					else
						n = _ttoi(s);

					pos = t.Find(_T("C"));
					if (pos > -1)
					{
						u = t.Left(pos);
						t = t.Mid(pos + 1);
						u.Replace(_T("R"), _T(""));
						if (u == _T("*"))
							M = mMax;
						else
							M = _ttoi(u);
						t.Replace(_T(")"), _T(""));
						if (t == _T("*"))
							N = nMax;
						else
							N = _ttoi(t);

						CXTPReportRecord* pRec = NULL;
						CXTPReportRecordItem* pItem = NULL;
						if (mMax > 0)
						{
							for (int row = m; row < M; row++)
							{
								for (int col = n; col < N; col++)
								{
									if (row < mMax)
									{
										pRec = m_pRecord->GetChilds()->GetAt(row);
										if (pRec)
										{
											if (col < pRec->GetItemCount())
											{
												pItem = pRec->GetItem(col);
												if (pItem && pItem != this) //no circular ref!)
												{
													s = pItem->GetCaption(NULL);
													d += StringToDouble(s);
												}
											}
										}
									}
								}
							}
							CString sFmt = GetFormatString();
							if (sFmt.IsEmpty() || sFmt == _T("%s"))
								sFmt = _T("%f");
							else if (sFmt == _T("%d"))
								sFmt = _T("%.0f");

							strCaption.Format(sFmt, d);
							SetTextColor(clrSubSum );
						}
					}
				}
			}
		}
		sFormula = m_strFormula;
		pos = sFormula.Find(_T("SUM("));
		//pattern SUM(R#C#:R#C#) or * instead of # - means all row or column
		if (pos > -1 && m_pRecord->GetRecords() != NULL)
		{
			int mMax = m_pRecord->GetRecords()->GetCount();
			int nMax = m_pRecord->GetItemCount();
			sFormula.Replace(_T(" "), _T(""));
			sFormula = sFormula.Mid(pos + 4);
			pos = sFormula.Find(_T(":"));
			if (pos > -1)
			{
				s = sFormula.Left(pos);
				t = sFormula.Mid(pos + 1);
				pos = s.Find(_T("C"));
				if (pos > -1)
				{
					u = s.Left(pos);
					s = s.Mid(pos + 1);
					u.Replace(_T("R"), _T(""));
					if (u == _T("*"))
						m = 0;
					else
						m = _ttoi(u);
					if (s == _T("*"))
						n = 0;
					else
						n = _ttoi(s);

					pos = t.Find(_T("C"));
					if (pos > -1)
					{
						u = t.Left(pos);
						t = t.Mid(pos + 1);
						u.Replace(_T("R"), _T(""));
						if (u == _T("*"))
							M = mMax;
						else
							M = _ttoi(u);
						t.Replace(_T(")"), _T(""));
						if (t == _T("*"))
							N = nMax;
						else
							N = _ttoi(t);

						CXTPReportRecord* pRec = NULL;
						CXTPReportRecordItem* pItem = NULL;
						if (m_pRecord->GetRecords())
						{
							for (int row = m; row < M; row++)
							{
								for (int col = n; col < N; col++)
								{
									if (row < mMax)
									{
										pRec = m_pRecord->GetRecords()->GetAt(row);
										if (pRec)
										{
											if (col < pRec->GetItemCount())
											{
												pItem = pRec->GetItem(col);
												if (pItem && pItem != this) //no circular ref!
												{
													s = pItem->GetCaption(NULL);
													d += StringToDouble(s);
												}
											}
										}
									}
								}
							}
							CString sFmt = GetFormatString();
							if (sFmt.IsEmpty() || sFmt == _T("%s"))
								sFmt = _T("%f");
							else if (sFmt == _T("%d"))
								sFmt = _T("%.0f");

							strCaption.Format(sFmt, d);
							SetTextColor(clrSum );
						}
					}
				}
			}
		}
	}
	//else if (m_strFormula.IsEmpty())
	//{
	//  CString sFmt = GetFormatString();
	//  if (sFmt.Find(_T("%")) > -1)
	//  {
	//      if (sFmt.Find(_T("d")) > -1)
	//          strCaption.Format(sFmt, _ttoi(strCaption));
	//      else if (sFmt.Find(_T("f")) > -1 || sFmt.Find(_T("g")) > -1)
	//          strCaption.Format(sFmt, _tstof(strCaption));
	//      else
	//          strCaption.Format(sFmt, strCaption);
	//  }
	//}
	if (m_strCaption == strCaption)
		return;

	m_strCaption = strCaption;

	XTPMarkupReleaseElement(m_pMarkupUIElement);

	if (m_pRecord == NULL) return;

	CXTPMarkupContext* pMarkupContext = m_pRecord->GetMarkupContext();
	if (pMarkupContext)
		m_pMarkupUIElement = XTPMarkupParseText(pMarkupContext, strCaption);
	else if (m_pRecord->m_pRecords) //extra source of markup
	{
		pMarkupContext = m_pRecord->m_pRecords->GetMarkupContext();
		if (pMarkupContext)
			m_pMarkupUIElement = XTPMarkupParseText(pMarkupContext, strCaption);
	}
}


void CXTPReportRecordItem::DoMouseButtonClick()
{
	mouse_event(GetSystemMetrics(SM_SWAPBUTTON) ? MOUSEEVENTF_RIGHTDOWN : MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(GetSystemMetrics(SM_SWAPBUTTON) ? MOUSEEVENTF_RIGHTUP : MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

void CXTPReportRecordItem::GetCaptionRect(XTP_REPORTRECORDITEM_ARGS* pDrawArgs, CRect& rcItem)
{
	ASSERT(pDrawArgs->pControl);
	if (!pDrawArgs->pControl)
		return;

	m_pControl = pDrawArgs->pControl;

	CXTPReportPaintManager*  pPaintManager = pDrawArgs->pControl->GetPaintManager();

	if (GetIconIndex() != XTP_REPORT_NOICON)
	{
		if (GetAlignment() != -1 && (GetAlignment() & xtpColumnIconRight) != 0)
			rcItem.right -= pPaintManager->DrawBitmap(NULL, pDrawArgs->pControl, rcItem, GetIconIndex());
		else
			rcItem.left += pPaintManager->DrawBitmap(NULL, pDrawArgs->pControl, rcItem, GetIconIndex());
	}

	if (m_bHasCheckbox)
		rcItem.left += 15;

	if (m_pItemControls && m_pItemControls->GetSize())
	{
		for (int i = 0; i < m_pItemControls->GetSize() && rcItem.Width() > 0; i++)
		{
			int nWidth = m_pItemControls->GetAt(i)->GetSize().cx;
			nWidth = nWidth < 0 ? rcItem.Width() : nWidth;
			if (m_pItemControls->GetAt(i)->GetAlignment() == xtpItemControlRight)
				rcItem.right -= nWidth;
			else
				rcItem.left += nWidth;
		}
	}

	// Adjust for new lock icon
	BOOL bShowLock = !IsEditable() && pPaintManager->m_bShowLockIcon;

	if (bShowLock && !pDrawArgs->pControl->IsIconView()) //to prevent reaction on always read-only IconColumn / NumberColumn
	{
		rcItem.left += 18;
	}

}

void CXTPReportRecordItem::DrawCheckBox(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, CRect& rcItem)
{
	BOOL bEditable = IsEditable()
		&& (pDrawArgs->pColumn == NULL || pDrawArgs->pColumn->IsEditable());

	CDC *pDC = pDrawArgs->pDC;

	XTPReportGlyph eState = xtpReportGlyphInvalid;

		  if ( bEditable && !IsChecked()) { eState = xtpReportGlyphEnabledUnchecked;  }
	else if ( bEditable &&  IsChecked()) { eState = xtpReportGlyphEnabledChecked;    }
	else if (!bEditable && !IsChecked()) { eState = xtpReportGlyphDisabledUnchecked; }
	else if (!bEditable &&  IsChecked()) { eState = xtpReportGlyphDisabledChecked;   }

	if (m_bTristateCheckbox && GetCheckedState() == xtpReportCheckStateIndeterminate)
	{
		eState = xtpReportGlyphEnabledTristate;
	}

	CXTPReportPaintManager *pPaintManager = pDrawArgs->pControl->GetPaintManager();

	int nIconAlign = pDrawArgs->nTextAlign & xtpColumnIconMask;
	int nIconAlignV = nIconAlign & 0x0F000000;
	int nIconAlignH = nIconAlign & 0x00F00000;

	m_rcGlyph = rcItem;
	CSize sizeGlyph = pPaintManager->DrawGlyph(NULL, m_rcGlyph, eState);

	CRect rcGlyph(m_rcGlyph);
	// horizontal alignment
	switch (nIconAlignH)
	{
	case xtpColumnIconRight:
		rcGlyph.left = rcItem.right - sizeGlyph.cx - 2;
		// shift text box to the left
		rcItem.right -= (2 + sizeGlyph.cx);
		break;
	case xtpColumnIconCenter:
		rcGlyph.left = rcItem.CenterPoint().x - sizeGlyph.cx / 2 - 1;
		rcGlyph.right = rcGlyph.left + sizeGlyph.cx;
		break; // left text box as is - draw on image...
	case xtpColumnIconLeft:
	default:
		// shift text box to the right
		rcItem.left += 2 + sizeGlyph.cx;
		rcGlyph.left += 2;
		rcGlyph.right = rcItem.left - 1;
		break;
	}
	// vertical alignment
	switch (nIconAlignV)
	{
	case xtpColumnIconTop:
		rcGlyph.bottom = rcItem.top + sizeGlyph.cy + 2;
		break;
	case xtpColumnIconBottom:
		rcGlyph.top = rcItem.bottom - sizeGlyph.cy - 2;
		break;
	}
	// draw the glyph
	pPaintManager->DrawGlyph(pDC, rcGlyph, eState);

	m_rcGlyph = rcGlyph; //to restrict click on Glyph area only
}

void CXTPReportRecordItem::OnDrawCaption(
	XTP_REPORTRECORDITEM_DRAWARGS *pDrawArgs,
	XTP_REPORTRECORDITEM_METRICS  *pMetrics)
{
	ASSERT(pDrawArgs->pItem == this);

	CXTPReportPaintManager *pPaintManager = pDrawArgs->pControl->GetPaintManager();
	BOOL bForceMarkUp(FALSE);
	if (pPaintManager)
		bForceMarkUp = pPaintManager->m_bForceDynamicMarkupForCell;
	//THIS CONDITION - IsVirtualMode() [Rev 7687 Oct 2008] PREVENTS MARKUP in Non-Virtual mode
	//if (pDrawArgs->pControl->IsVirtualMode() && pDrawArgs->pControl->GetMarkupContext())
	//if (pDrawArgs->pControl->GetMarkupContext()
	//  && !m_pMarkupUIElement) - rev 9113
	//if (pDrawArgs->pControl->IsVirtualMode()
	//  && pDrawArgs->pControl->GetMarkupContext() && !m_pMarkupUIElement) - rev 9114
	// FIXED BACK! Only for virtual mode! Developer have to set Caption of Item to have markup
	// FIXED AGAIN WITH SPECIAL Flag to use
	if ((pDrawArgs->pControl->IsVirtualMode() || bForceMarkUp)
		&& pDrawArgs->pControl->GetMarkupContext() && !m_pMarkupUIElement)
	{
		CXTPMarkupUIElement* pMarkupUIElement = XTPMarkupParseText(pDrawArgs->pControl->GetMarkupContext(), pMetrics->strText);
		if (pMarkupUIElement)
		{

			CRect rcItem = pDrawArgs->rcItem;
			rcItem.DeflateRect(2, 1, 2, 0);

			XTPMarkupSetDefaultFont(XTPMarkupElementContext(pMarkupUIElement), (HFONT)pMetrics->pFont->GetSafeHandle(), pMetrics->clrForeground);
			XTPMarkupMeasureElement(pMarkupUIElement, rcItem.Width(), INT_MAX);
			XTPMarkupRenderElement(pMarkupUIElement, pDrawArgs->pDC->GetSafeHdc(), &rcItem);
			XTPMarkupReleaseElement(pMarkupUIElement);
			return;
		}
	}

	if (m_pMarkupUIElement)
	{
		CRect rcItem = pDrawArgs->rcItem;
		rcItem.DeflateRect(2, 1, 2, 0);

		XTPMarkupSetDefaultFont(XTPMarkupElementContext(m_pMarkupUIElement), (HFONT)pMetrics->pFont->GetSafeHandle(), pMetrics->clrForeground);
		XTPMarkupMeasureElement(m_pMarkupUIElement, rcItem.Width(), INT_MAX);
		XTPMarkupRenderElement(m_pMarkupUIElement, pDrawArgs->pDC->GetSafeHdc(), &rcItem);
	}
	else
	{
		if (pPaintManager)
		{
			pPaintManager->DrawItemCaption(pDrawArgs, pMetrics);
		}
	}
}

void CXTPReportRecordItem::OnDrawControls(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, CRect& rcItem)
{
	ASSERT(pDrawArgs->pItem == this);
	if (!m_pItemControls)
		return;
	for (int i = 0; i < m_pItemControls->GetSize() && rcItem.Width(); i++)
	{
		pDrawArgs->pControl->GetPaintManager()->DrawItemControl(pDrawArgs, m_pItemControls->GetAt(i), rcItem);
	}
}

int CXTPReportRecordItem::Draw(XTP_REPORTRECORDITEM_DRAWARGS *pDrawArgs)
{
	ASSERT(pDrawArgs->pControl);
	if (!pDrawArgs->pControl)
		return 0;
	else
		m_pControl = pDrawArgs->pControl;

	CDC* pDC = pDrawArgs->pDC;

	CRect& rcItem = pDrawArgs->rcItem;
	CXTPReportPaintManager* pPaintManager = pDrawArgs->pControl->GetPaintManager();

	XTP_REPORTRECORDITEM_METRICS* pMetrics = new XTP_REPORTRECORDITEM_METRICS();
	pMetrics->strText = GetCaption(pDrawArgs->pColumn);
	pDrawArgs->pRow->GetItemMetrics(pDrawArgs, pMetrics);

	ASSERT(pMetrics->pFont);
	ASSERT(pMetrics->clrForeground != XTP_REPORT_COLOR_DEFAULT);

	if (pMetrics->clrBackground != XTP_REPORT_COLOR_DEFAULT)
	{
		pDC->FillSolidRect(rcItem, pMetrics->clrBackground);
	}

	int nItemGlyphs = rcItem.left;

	// draw tree inside item rect (see also HitTest function)
	if (pDrawArgs->pColumn && pDrawArgs->pColumn->IsTreeColumn())
	{
		int nTreeDepth = pDrawArgs->pRow->GetTreeDepth() - pDrawArgs->pRow->GetGroupLevel();
		if (nTreeDepth > 0)
			nTreeDepth++;

		BOOL bHasChildren = TRUE;
		if (pDrawArgs->pControl->IsVirtualMode())
		{
			bHasChildren = (pMetrics->nVirtRowFlags & xtpVirtRowHasChildren) > 0;
			if (!bHasChildren)
				pDrawArgs->pRow->m_rcCollapse = CRect(NULL);

			if (pDrawArgs->pColumn == pDrawArgs->pControl->GetColumns()->GetIndentColumn())
				pDrawArgs->nTextAlign = DT_LEFT; //to make compact tree presentation
		}
		else
		{
			bHasChildren = pDrawArgs->pRow->HasChildren();
		}
		rcItem.left += pDrawArgs->pControl->GetIndent(nTreeDepth);

		if (pPaintManager->m_nBaseParentRecordIndent != 0)
		{
			if ((pDrawArgs->pRow->GetParentRow() != NULL ? pDrawArgs->pRow->GetParentRow()->IsGroupRow() : FALSE)
					&& pDrawArgs->pRow->HasChildren() && (nTreeDepth == 0))
				rcItem.left += pPaintManager->m_nBaseParentRecordIndent;

			if ((pDrawArgs->pRow->GetParentRow() != NULL ? !pDrawArgs->pRow->GetParentRow()->IsGroupRow() : FALSE)
					&& (nTreeDepth > 0) && pDrawArgs->pControl->GetColumns()->GetGroupsOrder()->GetCount() > 0)
				rcItem.left += pPaintManager->m_nBaseParentRecordIndent;
		}

		// EXTRA TREE INDENT
		if (pPaintManager->m_bBaseTreeIndent)
		{
			rcItem.left += pPaintManager->m_nTreeIndent;
		}

		CRect rcBitmap(rcItem);
		CSize sizeBitmap = pPaintManager->DrawCollapsedBitmap(NULL, pDrawArgs->pRow, rcBitmap);

		pPaintManager->DrawTreeStructure(pDrawArgs, pMetrics, rcItem, sizeBitmap);

		int nIconAlign = pDrawArgs->nTextAlign & xtpColumnIconMask;
		// horizontal alignment
		switch (nIconAlign)
		{
		case xtpColumnIconRight:
			rcBitmap.left = rcBitmap.right - sizeBitmap.cx - 2;
			break;
		case xtpColumnIconLeft:
			rcBitmap.right = rcBitmap.left + sizeBitmap.cx + 2;
			break;
		}
		// vertical alignment
		switch (nIconAlign)
		{
		case xtpColumnIconTop:
			rcBitmap.bottom = rcBitmap.top + sizeBitmap.cy + 2;
			break;
		case xtpColumnIconBottom:
			rcBitmap.top = rcBitmap.bottom - sizeBitmap.cy - 2;
			break;
		}

		int nTreeTextIndent = 0;

		if (bHasChildren)
		{
			sizeBitmap = pPaintManager->DrawCollapsedBitmap(pDC, pDrawArgs->pRow, rcBitmap);

			if (!pDC->IsPrinting())
			{
				pDrawArgs->pRow->SetCollapseRect(rcBitmap);
			}

			nTreeTextIndent = pPaintManager->m_nTreeTextIndentRowParent;
		}
		else
		{
			nTreeTextIndent = pPaintManager->m_nTreeTextIndentRowChildren;
		}

		rcItem.left  = rcBitmap.left;
		rcItem.left += sizeBitmap.cx;
		rcItem.left += nTreeTextIndent;
	}

	pDC->SetTextColor(pMetrics->clrForeground);

	CFont* pOldFont = (CFont*)pDC->SelectObject(pMetrics->pFont);

	if (pMetrics->clrBackground != XTP_REPORT_COLOR_DEFAULT)
	{
		pDC->SetBkColor(pMetrics->clrBackground);
	}
	else
	{
		pDC->SetBkColor(pPaintManager->m_clrControlBack);
	}

	// Draw checkbox
	if (m_bHasCheckbox)
	{
		DrawCheckBox(pDrawArgs, rcItem);
	}

	BOOL bShowLock = !IsEditable() && pDrawArgs->pControl->GetPaintManager()->m_bShowLockIcon;
	// Do the draw bitmap pDC, rcItem, GetIconIndex()
	if (bShowLock && !pDrawArgs->pControl->IsIconView()) //to prevent reaction on always read-only IconColumn / NumberColumn
	{
		rcItem.left++;

		if (pDrawArgs->pControl->GetPaintManager()->m_bUseShellIcon) //variant without resources
		{
			pPaintManager->DrawShellIcon(pDC, rcItem, xtpReportShellIconLock);
		}
		else                                                         //variant with resources
		{
			pPaintManager->DrawGlyph(pDC, rcItem, xtpReportGlyphItemLocked);
		}

		rcItem.left += 17;
	}
	else if (pMetrics->nItemIcon != XTP_REPORT_NOICON || GetIconIndex() != XTP_REPORT_NOICON)
	{
		pPaintManager->DrawItemBitmap(pDrawArgs, rcItem, pMetrics->nItemIcon != XTP_REPORT_NOICON ? pMetrics->nItemIcon : GetIconIndex());
	}
	else
	{
		CXTPReportRecordItemEditOptions* pEditOptions = GetEditOptions(pDrawArgs->pColumn);
		if (pEditOptions && pEditOptions->arrInplaceButtons.GetSize() > 0)
		{
			CXTPReportInplaceButton *pButton = pEditOptions->GetButton(0);
			if (pButton && pButton->IsInsideCellButton())
			{
				CRect rc(rcItem);
				rc.left = rc.right - 17;//like CXTPReportInplaceButton::m_nWidth

				if (pPaintManager->IsFixedInplaceButtonHeight())
					rc.bottom = min(rc.bottom, rc.top + 19); //like CXTPReportInplaceButton::m_nFixedHeight

				BOOL bShift(FALSE);
				//to keep focused frame not touched
				rc.top++;
				rc.bottom -= 2;
				rc.left--;
				if (pButton->GetID() == XTP_ID_REPORT_COMBOBUTTON)
					rc.right--;

				if (pButton->GetID() == XTP_ID_REPORT_COMBOBUTTON)
					bShift = pPaintManager->DrawInplaceComboButton(pDC, &rc);
				else if (pButton->GetID() == XTP_ID_REPORT_EXPANDBUTTON)
					bShift = pPaintManager->DrawInplaceExpandButton(pDC, &rc);
				else if (pButton->GetID() == XTP_ID_REPORT_SPINBUTTON)
					bShift = pPaintManager->DrawInplaceSpinButton(pDC, &rc);

				if (bShift)
					pDrawArgs->rcItem.right -= 17;
			}
		}
	}
	nItemGlyphs = (rcItem.left - nItemGlyphs);

	OnDrawControls(pDrawArgs, rcItem);

	if (rcItem.Width() > 2)
	{
		OnDrawCaption(pDrawArgs, pMetrics);
	}

	int nItemTextWidth = nItemGlyphs + 7;
	if (m_pMarkupUIElement)
	{
		nItemTextWidth += XTPMarkupMeasureElement(m_pMarkupUIElement, rcItem.Width() - 4, INT_MAX).cx;
	}
	else
	{
		CString sFirst(pMetrics->strText);
		CString sLast;
		int jPos = sFirst.Find(_T('\n'), 0);
		int wd(0);
		while (jPos > -1)
		{
			sLast = sFirst.Mid(jPos + 1);
			sFirst = sFirst.Left(jPos - 1);
			wd = max(wd, pDC->GetTextExtent(sFirst).cx + 7);
			sFirst = sLast;
			jPos = sFirst.Find(_T('\n'), 0);
		}

		if (!sFirst.IsEmpty())
		{
			nItemTextWidth = max(wd, pDC->GetTextExtent(sFirst).cx + 7);
		}
	}
	pDC->SelectObject(pOldFont);

	pMetrics->InternalRelease();

	return nItemTextWidth;
}

CXTPReportRecordItemControl* CXTPReportRecordItem::HitTest(CPoint ptPoint)
{
	if (!(m_pItemControls && m_pItemControls->GetSize()))
		return NULL;

	for (int i = 0; i < m_pItemControls->GetSize(); i++)
	{
		CRect rcControl = m_pItemControls->GetAt(i)->GetRect();
		if (rcControl.PtInRect(ptPoint))
			return m_pItemControls->GetAt(i);
	}

	return NULL;
}

BOOL CXTPReportRecordItem::OnLButtonDown(XTP_REPORTRECORDITEM_CLICKARGS* pClickArgs)
{
	CXTPReportRecordItemControl* pItemControl = HitTest(pClickArgs->ptClient);
	if (!pItemControl)
		return FALSE;

	pItemControl->OnLButtonDown(pClickArgs);

	// create item control window
	if (m_pItemControlHookWnd == NULL)
	{
		m_pItemControlHookWnd = new CXTPReportRecordItemControlHookWnd(pClickArgs);
		m_pItemControlHookWnd->Create(AfxRegisterWndClass(0), _T(""), WS_CHILD, pItemControl->GetRect(), pClickArgs->pControl, 0);
		m_pItemControlHookWnd->SetCapture();
	}
	// redraw control
		pClickArgs->pControl->RedrawControl();
	return TRUE;
}

BOOL CXTPReportRecordItem::OnLButtonUp(XTP_REPORTRECORDITEM_CLICKARGS* pClickArgs)
{
//>>attempt for 23588 issue case
	CXTPSmartPtrInternalT<CXTPReportRecordItem> xSafety(this, TRUE);
//>>attempt for 23588 issue case

	if (m_pItemControlHookWnd && m_pFocusedItemControl)
	{
		ReleaseCapture();

		pClickArgs->pControl->ScreenToClient(&pClickArgs->ptClient);

		m_pFocusedItemControl->OnLButtonUp(pClickArgs);

		pClickArgs->pControl->RedrawControl();

		// destroy item control window
		m_pItemControlHookWnd->DestroyWindow();

		delete m_pItemControlHookWnd;
		m_pItemControlHookWnd = NULL;

		return TRUE;
	}
	CXTPReportRecordItemControl* pItemControl = HitTest(pClickArgs->ptClient);
	if (!pItemControl)
		return FALSE;
	pItemControl->OnLButtonUp(pClickArgs);
	pClickArgs->pControl->RedrawControl();
	return TRUE;
}

void CXTPReportRecordItem::OnClick(XTP_REPORTRECORDITEM_CLICKARGS* pClickArgs)
{
	if (!pClickArgs || !pClickArgs->pControl)
		return;

	if (OnLButtonUp(pClickArgs)) //old version (10) did not have this call
		return;

	InternalAddRef();
	CMDTARGET_ADDREF(pClickArgs->pColumn);
	CMDTARGET_ADDREF(pClickArgs->pControl);

	CXTPReportRow* pRow = pClickArgs->pRow;
	CMDTARGET_ADDREF(pRow);

	BOOL bCheckBoxClicked = FALSE;
	CXTPReportControl* pControl = pClickArgs->pControl;

	if (IsEditable()
		&& m_bHasCheckbox &&
		(pClickArgs->pColumn == NULL || pClickArgs->pColumn->IsEditable())
		&& OnRequestEdit(pClickArgs))
	{
		// adjust vertical coordinates if virtual mode
		if (pControl->IsVirtualMode())
		{
			m_rcGlyph.top = pClickArgs->rcItem.top;
			m_rcGlyph.bottom = pClickArgs->rcItem.bottom;
		}
		// check whether we're clicking glyph area
		if (m_rcGlyph.PtInRect(pClickArgs->ptClient) &&
			m_rcGlyph.PtInRect(pControl->m_mouseDownState.ptMouse))
		{
			if (pControl->IsAutoCheckItems())
			{
				if (!m_bTristateCheckbox)
				{
					SetChecked(!IsChecked());
				}
				else
				{
					SetCheckedState((GetCheckedState() + 1) % xtpReportCheckStateCount);
				}
			}
			pControl->RedrawControl();

			pControl->SendMessageToParent(pClickArgs->pRow, this, pClickArgs->pColumn, XTP_NM_REPORT_CHECKED, &pClickArgs->ptClient);
			bCheckBoxClicked = TRUE;
		}
	}

	if (!bCheckBoxClicked && IsAllowEdit(pClickArgs))
	{
		if (pControl->IsEditOnDelayClick())
		{
			if (  pControl->GetLastRqstEditCol() != pClickArgs->pColumn->GetItemIndex()
				|| pControl->GetLastRqstEditRow() != pClickArgs->pRow->GetIndex())
			{
				pControl->EnsureStopLastRqstTimer();
				pControl->SetLastRqstEdit(pClickArgs->pRow->GetIndex(), pClickArgs->pColumn->GetItemIndex());
			}
			else if(!pControl->GetDelayEditTimer())
			{
				// Start the timer for this request.
				pControl->StartDelayEditTimer();
			}
		}
		if (pControl->IsEditOnClick())
		{
			pControl->EditItem(pClickArgs);

			CPoint pt(pClickArgs->ptClient);
			pControl->ClientToScreen(&pt);

			CXTPReportInplaceEdit* pEdit = DYNAMIC_DOWNCAST(CXTPReportInplaceEdit, CWnd::FromHandle(WindowFromPoint(pt)));
			if (pEdit && pEdit->GetItem() == this)
			{
				CXTPReportRecordItemEditOptions* pEditOptions = GetEditOptions(pClickArgs->pColumn);

				if (pEditOptions->m_bSelectTextOnEdit)
					pEdit->SetSel(0, -1);
				else
					DoMouseButtonClick();
			}
		}
	}


	pClickArgs->pControl->SendMessageToParent(pClickArgs->pRow, this, pClickArgs->pColumn,
		NM_CLICK, &pClickArgs->ptClient);


	// Determine Hyperlink Click
	int nHyperlink = HitTestHyperlink(pClickArgs->ptClient);
	int mHyperlink = HitTestHyperlink(pControl->m_mouseDownState.ptMouse);
	if (nHyperlink >= 0 && mHyperlink >= 0)
		pClickArgs->pControl->SendMessageToParent(pClickArgs->pRow, this, pClickArgs->pColumn, XTP_NM_REPORT_HYPERLINK, &pClickArgs->ptClient, nHyperlink);

	CMDTARGET_RELEASE(pRow);
	CMDTARGET_RELEASE(pClickArgs->pColumn);
	CMDTARGET_RELEASE(pClickArgs->pControl);
	InternalRelease();
}

int CXTPReportRecordItem::HitTestHyperlink(CPoint ptClick)
{
	int nHitTest = -1;

	int nHyperlinks = GetHyperlinksCount();
	CXTPReportHyperlink *pHyperlink;

	for (int nHyperlink = 0; nHyperlink < nHyperlinks; nHyperlink++)
	{
		pHyperlink = GetHyperlinkAt(nHyperlink);
		ASSERT(pHyperlink);
		if (pHyperlink && pHyperlink->m_rcHyperSpot.PtInRect(ptClick))
		{
			ASSERT(-1 == nHitTest);
			nHitTest = nHyperlink;
		}
	}
	return nHitTest;
}

void CXTPReportRecordItem::OnDblClick(XTP_REPORTRECORDITEM_CLICKARGS *pClickArgs)
{
	if (!pClickArgs || !pClickArgs->pControl)
		return;

	if (OnLButtonDown(pClickArgs))  //old version (10) did not have this call
		return;

	CXTPReportControl *pControl = pClickArgs->pControl;

	if (IsAllowEdit(pClickArgs)
		&& !pControl->IsEditOnClick()
		&&  pControl->IsEditOnDoubleClick()
		&&  pControl->GetInplaceEdit()->GetItem() != this)
	{
		pControl->EditItem(pClickArgs);

		CPoint pt(pClickArgs->ptClient);
		pControl->ClientToScreen(&pt);

		CXTPReportInplaceEdit* pEdit = DYNAMIC_DOWNCAST(CXTPReportInplaceEdit, CWnd::FromHandle(WindowFromPoint(pt)));
		if (pEdit && pEdit->GetItem() == this)
		{
			CXTPReportRecordItemEditOptions* pEditOptions = GetEditOptions(pClickArgs->pColumn);

			if (pEditOptions->m_bSelectTextOnEdit)
				pEdit->SetSel(0, -1);
			else
				DoMouseButtonClick();
		}
	}

	pControl->SendMessageToParent(pClickArgs->pRow, this, pClickArgs->pColumn, NM_DBLCLK, &pClickArgs->ptClient, -1);
}

void CXTPReportRecordItem::OnMouseMove(UINT nFlags, CPoint point, CXTPReportControl* pControl)
{
	int nHyperlink = HitTestHyperlink(point);
	if (nHyperlink >= 0)
	{
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));
		return;
	}

	CXTPReportRecordItemControl* pItemControl = HitTest(point);

	if (m_pItemControlHookWnd && m_pFocusedItemControl && (nFlags & MK_LBUTTON))
	{
		pControl->ScreenToClient(&point);
		if (m_bItemControlUnderMouse && !m_pFocusedItemControl->GetRect().PtInRect(point))
		{
			m_pFocusedItemControl->OnMouseLeave(nFlags, point);
			m_bItemControlUnderMouse = FALSE;
		}
		else if (!m_bItemControlUnderMouse && m_pFocusedItemControl->GetRect().PtInRect(point))
		{
			m_pFocusedItemControl->OnMouseEnter(nFlags, point);
			m_bItemControlUnderMouse = TRUE;
		}
		else
			m_pFocusedItemControl->OnMouseMove(nFlags, point);
		if (pControl != NULL && !pControl->IsEditMode())
			pControl->RedrawControl();
		return;
	}

	if (pItemControl)
	{
		if (pItemControl != m_pFocusedItemControl)
		{
			if (m_pFocusedItemControl)
			{
				m_pFocusedItemControl->OnMouseLeave(nFlags, point);
				m_bItemControlUnderMouse = FALSE;
			}
			pItemControl->OnMouseEnter(nFlags, point);
			m_bItemControlUnderMouse = TRUE;
		}
		pItemControl->OnMouseMove(nFlags, point);
		if (pControl != NULL && !pControl->IsEditMode())
			pControl->RedrawControl();
	}
	else
	{
		if (m_pFocusedItemControl)
		{
			m_pFocusedItemControl->OnMouseLeave(nFlags, point);
			m_bItemControlUnderMouse = FALSE;
			if (pControl != NULL && !pControl->IsEditMode())
				pControl->RedrawControl();
		}
	}
	m_pFocusedItemControl = pItemControl;
}

void CXTPReportRecordItem::SetFont(CFont* pFont)
{
	m_pFontCaption = pFont;
}

void CXTPReportRecordItem::SetBold(BOOL bBold)
{
	m_bBoldText = bBold;
}

void CXTPReportRecordItem::SetTextColor(COLORREF clrText)
{
	m_clrText = clrText;
}

COLORREF CXTPReportRecordItem::GetTextColor()
{
	return m_clrText;
}

void CXTPReportRecordItem::SetBackgroundColor(COLORREF clrBackground)
{
	m_clrBackground = clrBackground;
}

COLORREF CXTPReportRecordItem::GetBackgroundColor()
{
	return m_clrBackground;
}

int CXTPReportRecordItem::Compare(CXTPReportColumn* pColumn, CXTPReportRecordItem* pItem)
{
	if (!pItem)
		return 0;

	if (m_nSortPriority != -1 || pItem->m_nSortPriority != -1)
		return m_nSortPriority - pItem->m_nSortPriority;

	return m_pRecord->GetRecords()->Compare(GetCaption(pColumn), pItem->GetCaption(pColumn));
}

CFont* CXTPReportRecordItem::GetFont()
{
	return m_pFontCaption;
}

CString CXTPReportRecordItem::GetGroupCaption(CXTPReportColumn* pColumn)
{
	if (!m_strGroupCaption.IsEmpty())
		return m_strGroupCaption;

	int nID = GetGroupCaptionID(pColumn);

	if (nID > 0)
	{
		CString str;
		if (str.LoadString(nID))
			return str;
	}

	if (m_pMarkupUIElement || pColumn->GetMarkupUIElement())
		return GetCaption(pColumn);

	return pColumn->GetCaption() + _T(": ") + GetCaption(pColumn);
}

int CXTPReportRecordItem::CompareGroupCaption(CXTPReportColumn* pColumn, CXTPReportRecordItem* pItem)
{
	if (m_nGroupPriority != -1)
		return m_nGroupPriority - pItem->m_nGroupPriority;

	if (!m_strGroupCaption.IsEmpty())
		return m_pRecord->GetRecords()->Compare(m_strGroupCaption, pItem->m_strGroupCaption);

	int nID = GetGroupCaptionID(pColumn);

	if (nID > 0)
		return nID - pItem->GetGroupCaptionID(pColumn);

	return Compare(pColumn, pItem);
}

int CXTPReportRecordItem::GetGroupCaptionID(CXTPReportColumn* /*pColumn*/)
{
	return m_nGroupPriority;
}


void CXTPReportRecordItem::GetItemMetrics(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, XTP_REPORTRECORDITEM_METRICS* pItemMetrics)
{
	if (m_clrBackground != XTP_REPORT_COLOR_DEFAULT)
		pItemMetrics->clrBackground = m_clrBackground;

	if (m_clrText != XTP_REPORT_COLOR_DEFAULT)
		pItemMetrics->clrForeground = m_clrText;

	if (m_pFontCaption != NULL)
		pItemMetrics->pFont = m_pFontCaption;
	else if (m_bBoldText)
		pItemMetrics->pFont = &pDrawArgs->pControl->GetPaintManager()->m_fontBoldText;

	if (m_Alignment != (XTPReportColumnIconAlignment)(-1))
		pItemMetrics->nColumnAlignment = m_Alignment;
}

int CXTPReportRecordItem::GetIndex() const
{
	return m_pRecord ? m_pRecord->IndexOf(this) : -1;
}

CXTPReportColumn* CXTPReportRecordItem::GetColumn()
{
	if (m_pControl)
	{
		int nItemIndex = GetIndex();
		CXTPReportColumns* pCols = m_pControl->GetColumns();
		if (pCols)
			return pCols->Find(nItemIndex);
	}

	return NULL;
}

BOOL CXTPReportRecordItem::IsEditable() const
{
	if (this == NULL)
		return FALSE;

	if (!m_pRecord)
		return FALSE;

	return m_pRecord ? m_pRecord->m_bEditable && m_bEditable && IsFocusable() : FALSE;
}


BOOL CXTPReportRecordItem::IsFocusable() const
{
	if (this == NULL)
		return FALSE;

	return m_bFocusable;
}

BOOL CXTPReportRecordItem::GetCanEditCheckboxItem() const
{
	if (this == NULL)
		return FALSE;

	return m_bCanEditCheckboxItem;
}

void CXTPReportRecordItem::SetCanEditCheckboxItem(BOOL bSet)
{
	m_bCanEditCheckboxItem = bSet;
}

void CXTPReportRecordItem::OnBeginEdit(XTP_REPORTRECORDITEM_ARGS* pItemArgs)
{
	ASSERT(pItemArgs);
	if (!pItemArgs)
		return;

	if (IsEditable() && OnRequestEdit(pItemArgs))
	{
		CXTPReportControl* pControl = pItemArgs->pControl;
		CXTPReportInplaceEdit* pEdit = pControl->GetInplaceEdit();
		CRect rcCaption(pItemArgs->rcItem);
		GetCaptionRect(pItemArgs, rcCaption);

		CXTPReportRecordItemEditOptions* pEditOptions = GetEditOptions(pItemArgs->pColumn);
		ASSERT(pEditOptions);

		CXTPReportInplaceButtons* pInpaceButtons = &pEditOptions->arrInplaceButtons;

		CRect rcButtons(pItemArgs->rcItem);
		CXTPClientRect rcControl(pItemArgs->pControl);

		int i;
		// calculate right-aligned item controls width
		int nControlsWidthRight = 0;
		if (m_pItemControls && m_pItemControls->GetSize() > 0)
		{
			for (i = 0; m_pItemControls && i < m_pItemControls->GetSize(); i++)
			{
				CXTPReportRecordItemControl* pControl = m_pItemControls->GetAt(i);

				if (pControl->GetAlignment() == xtpItemControlRight)
					nControlsWidthRight += pControl->GetRect().Width();
				else
					pItemArgs->rcItem.left += pControl->GetRect().Width();

			}
		}
		// calculate inplace buttons widths
		int nButtonsWidthIn = 0, nButtonsWidthOut = 0;
		for (i = 0; i < pInpaceButtons->GetSize(); i++)
		{
			CXTPReportInplaceButton* pButton = pInpaceButtons->GetAt(i);
			if (pButton->IsInsideCellButton())
				nButtonsWidthIn += pButton->GetWidth();
			else
				nButtonsWidthOut += pButton->GetWidth();
		}

		// adjust buttons rect
		if (rcButtons.right + nButtonsWidthOut > rcControl.right)
			rcButtons.right = rcControl.right - nButtonsWidthOut;

		rcButtons.left = min(rcButtons.right, pItemArgs->rcItem.right - nControlsWidthRight);

		// create buttons
		for (i = 0; i < pInpaceButtons->GetSize(); i++)
		{
			CXTPReportInplaceButton* pButton = pInpaceButtons->GetAt(i);
			pButton->Create(pItemArgs, rcButtons);

			pControl->GetInplaceButtons()->Add(pButton);
		}


		// adjust item rect
		pItemArgs->rcItem.right = rcButtons.left;

		if (pEditOptions->m_bAllowEdit)
		{   if (m_bHasCheckbox && !m_bCanEditCheckboxItem)
				return;
			else
				pEdit->Create(pItemArgs);
		}

		if (!pEditOptions->m_bAllowEdit && pInpaceButtons->GetSize() > 0)
		{
			CXTPReportInplaceButton *pButton = pInpaceButtons->GetAt(0);
			OnInplaceButtonDown(pButton);
		}
		
		XTP_NM_REPORTRECORDITEM nm;
		::ZeroMemory(&nm, sizeof(nm));

		nm.pItem = this;
		nm.pRow = pItemArgs->pRow;
		nm.pColumn = pItemArgs->pColumn;

		pControl->SendNotifyMessage(XTP_NM_REPORT_BEGINEDIT, (NMHDR*)&nm);
	}
}

void CXTPReportRecordItem::OnCancelEdit(CXTPReportControl *pControl, BOOL bApply)
{
	if (NULL == pControl)
		return;

	CXTPReportInplaceButtons* pInpaceButtons = pControl->GetInplaceButtons();

	for (int i = 0; i < pInpaceButtons->GetSize(); i++)
	{
		CXTPReportInplaceButton* pButton = pInpaceButtons->GetAt(i);
		if (pButton->GetSafeHwnd())
			pButton->DestroyWindow();
		if (pButton)
		{
			pButton->SetItemArgs(0);
		}
	}
	pInpaceButtons->RemoveAll();

	CXTPReportInplaceList* pInpaceList = pControl->GetInplaceList();
	if (pInpaceList->GetSafeHwnd())
		pInpaceList->DestroyWindow();

	CXTPReportInplaceEdit* pEdit = pControl->GetInplaceEdit();
	if (pEdit == NULL) return;

	if (pEdit->GetSafeHwnd())
	{
		if (bApply && pEdit->GetItem() == this)
			OnValidateEdit((XTP_REPORTRECORDITEM_ARGS*) pEdit);
		pEdit->HideWindow();
		pEdit->SetItemArgs(0);
		pEdit->DestroyWindow();
	}
}

void CXTPReportRecordItem::OnConstraintChanged(XTP_REPORTRECORDITEM_ARGS* pItemArgs, CXTPReportRecordItemConstraint* pConstraint)
{
	OnEditChanged(pItemArgs, pConstraint->m_strConstraint);
}

BOOL CXTPReportRecordItem::OnEditChanging(XTP_REPORTRECORDITEM_ARGS* pItemArgs, CString& rstrNewText)
{
	UNREFERENCED_PARAMETER(pItemArgs);
	UNREFERENCED_PARAMETER(rstrNewText);
	return TRUE;
}

void CXTPReportRecordItem::OnEditChanged(XTP_REPORTRECORDITEM_ARGS* pItemArgs, LPCTSTR szText)
{
	if (pItemArgs)
	{
		int r = pItemArgs->pRow->GetIndex();
		int i = pItemArgs->pItem->GetIndex();
		CXTPReportRecord* pRec = pItemArgs->pRow->GetRecord();
		if (pRec)
			pRec->UpdateRecordField(r, i, szText);
	}
}

void CXTPReportRecordItem::OnValidateEdit(XTP_REPORTRECORDITEM_ARGS* pItemArgs)
{
	ASSERT(pItemArgs);
	if (!pItemArgs)
		return;

	CXTPReportControl* pControl = pItemArgs->pControl;
	CXTPReportInplaceEdit* pEdit = pControl->GetInplaceEdit();
	XTP_REPORTRECORDITEM_ARGS args = *pItemArgs;

	if (pEdit->GetSafeHwnd() && pEdit->GetItem() == this)
	{
		args.AddRef();

		BOOL bRedraw = FALSE;
		CMDTARGET_RELEASE(pEdit->pItem); //pEdit->pItem = NULL;

		if (GetEditOptions(args.pColumn)->m_bConstraintEdit)
		{
			if (pEdit->m_pSelectedConstraint)
			{
				OnConstraintChanged(&args, pEdit->m_pSelectedConstraint);
				bRedraw = TRUE;
			}
		}
		else
		{
			CString strValue;
			pEdit->GetWindowText(strValue);

			CString  sOldValue = GetCaption(args.pColumn);
			if (sOldValue != strValue || pControl->IsVirtualMode())
			{
				OnEditChanged(&args, strValue);
				bRedraw = TRUE;
			}
			//else if (GetCaption(args.pColumn) != strValue) //<<>>
			//{
			//  OnEditChanged(&args, strValue);
			//  bRedraw = TRUE;
			//}
		}
		if (bRedraw)
		{
			pControl->RedrawControl();
			pControl->SendMessageToParent(args.pRow, this, args.pColumn, XTP_NM_REPORT_VALUECHANGED, 0);
		}
		else
		{
			OnEditCanceled(pItemArgs);
			pControl->SendMessageToParent(args.pRow, this, args.pColumn, XTP_NM_REPORT_EDIT_CANCELED, 0);
		}

		pEdit->SetItemArgs(0);

		args.Release();
	}
}

BOOL CXTPReportRecordItem::IsAllowEdit(XTP_REPORTRECORDITEM_ARGS *pItemArgs)
{
	BOOL bIsAllowEdit = pItemArgs->pControl->IsAllowEdit();

	if (m_pRecord)
	{
		 // Header record
		if (m_pRecord->m_pRecords == pItemArgs->pControl->GetHeaderRecords())
		{
			bIsAllowEdit = pItemArgs->pControl->IsHeaderRowsAllowEdit();
		}
		// Footer record
		else if (m_pRecord->m_pRecords == pItemArgs->pControl->GetFooterRecords())
		{
			bIsAllowEdit = pItemArgs->pControl->IsFooterRowsAllowEdit();
		}
	}

	bIsAllowEdit = bIsAllowEdit && IsEditable();

	if (pItemArgs->pColumn)
	{
		bIsAllowEdit = bIsAllowEdit && pItemArgs->pColumn->IsEditable();
	}
	else
	{
		if (m_pEditOptions)
		{
			bIsAllowEdit = bIsAllowEdit && m_pEditOptions->m_bAllowEdit;
		}
	}

	return bIsAllowEdit;
}

BOOL CXTPReportRecordItem::OnRequestEdit(XTP_REPORTRECORDITEM_ARGS* pItemArgs)
{
	CXTPReportControl* pControl = pItemArgs->pControl;

	XTP_NM_REPORTREQUESTEDIT nm;
	::ZeroMemory(&nm, sizeof(nm));

	nm.bCancel = FALSE;
	nm.pItem = this;
	nm.pRow = pItemArgs->pRow;
	nm.pColumn = pItemArgs->pColumn;

	pControl->SendNotifyMessage(XTP_NM_REPORT_REQUESTEDIT, (NMHDR*)&nm);

	return !nm.bCancel;
}

BOOL CXTPReportRecordItem::OnChar(XTP_REPORTRECORDITEM_ARGS *pItemArgs, UINT nChar)
{
	CXTPReportControl* pControl = pItemArgs->pControl;

	if ((nChar == VK_SPACE) && IsEditable() && m_bHasCheckbox && (pItemArgs->pColumn == NULL || pItemArgs->pColumn->IsEditable())
		&& OnRequestEdit(pItemArgs))
	{
		if (pControl->IsAutoCheckItems())
		{
			if (!m_bTristateCheckbox)
				SetChecked(!IsChecked());
			else
				SetCheckedState((GetCheckedState() + 1) % 3);
		}
		pControl->RedrawControl();

		pControl->SendMessageToParent(pItemArgs->pRow, this, pItemArgs->pColumn, XTP_NM_REPORT_CHECKED, NULL);
		return TRUE;
	}

	if (IsAllowEdit(pItemArgs))
	{
		// ^^ causes CXTPReportRecordItem::OnChar do be called twice
		BOOL bSameItem = pItemArgs->pItem == pControl->GetActiveItem();

		if (!bSameItem)
		{
			pControl->EditItem(pItemArgs);
		}

		CXTPReportInplaceEdit *pEdit = pControl->GetInplaceEdit();

		if (pEdit->GetSafeHwnd() && pEdit->GetItem() == this)
		{
			if (!bSameItem)
			{
				pEdit->SetFocus();
				pEdit->SetSel(0, -1);
			}
			if (VK_TAB != nChar)
			{
				pEdit->SendMessage(WM_CHAR, nChar);
			}
		}
		return TRUE;

	}
	return FALSE;
}

CXTPReportRecordItemEditOptions* CXTPReportRecordItem::GetEditOptions(CXTPReportColumn* pColumn)
{
	if (m_pEditOptions)
		return m_pEditOptions;

	if (pColumn)
		return pColumn->GetEditOptions();

	return m_pEditOptions = new CXTPReportRecordItemEditOptions();
}

CXTPReportRecordItemControls* CXTPReportRecordItem::GetItemControls()
{
	if (!m_pItemControls)
		m_pItemControls = new CXTPReportRecordItemControls(this);
	return m_pItemControls;
}

void CXTPReportRecordItem::OnInplaceButtonDown(CXTPReportInplaceButton* pButton)
{
	CXTPReportControl* pControl = pButton->pControl;

	XTP_NM_REPORTINPLACEBUTTON nm;
	::ZeroMemory(&nm, sizeof(nm));

	nm.pButton = pButton;
	nm.pItem = this;

	if (pControl->SendNotifyMessage(XTP_NM_REPORT_INPLACEBUTTONDOWN, (NMHDR*)&nm) == TRUE)
		return;

	if (pButton->GetID() == XTP_ID_REPORT_COMBOBUTTON)
	{
		CXTPReportInplaceList* pList = pControl->GetInplaceList();

		XTP_REPORTRECORDITEM_ARGS itemArgs = *pButton;
		if (!itemArgs.pColumn
			&& !itemArgs.pControl
			&& !itemArgs.pItem
			&& !itemArgs.pRow)
			return;
		ASSERT(itemArgs.pItem == this);

		CXTPWindowRect rcButton(pButton);
		pControl->ScreenToClient(&rcButton);
		itemArgs.rcItem.right = rcButton.right;
		itemArgs.rcItem.bottom = rcButton.bottom;

		CXTPReportRecordItemEditOptions* pEditOptions = GetEditOptions(itemArgs.pColumn);
		pList->m_Items2Show = pButton->m_Items2Show;
		if (pEditOptions->GetConstraints()->GetCount() > 0)
			pList->Create(&itemArgs, pEditOptions->GetConstraints());
	}
	else if (pButton->GetID() == XTP_ID_REPORT_SPINBUTTON)
	{
		XTP_REPORTRECORDITEM_ARGS itemArgs = *pButton;
		ASSERT(itemArgs.pItem == this);
		CXTPReportInplaceEdit* pEdit = pControl->GetInplaceEdit();
		if (itemArgs.pRow && itemArgs.pColumn && pEdit->GetSafeHwnd() && pEdit->GetItem() == this)
		{
			CString strValue;
			pEdit->GetWindowText(strValue);

			int nValue = _ttoi(strValue);

			int nValue2 = nValue + pButton->m_nSpinIncrement;
			nValue2 = max(nValue2, pButton->m_nSpinMin);
			nValue2 = min(nValue2, pButton->m_nSpinMax);

			if (nValue2 != nValue)
			{
				strValue.Format(_T("%d"), nValue2);

				BOOL bCommit = OnEditChanging(&itemArgs, strValue);
				if (bCommit)
					pEdit->SetWindowText(strValue);

				pControl->RedrawControl();
				//pControl->SendMessageToParent(itemArgs.pRow, this, itemArgs.pColumn, XTP_NM_REPORT_VALUECHANGED, 0);
			}
		}
	}
}

CXTPReportHyperlinks* CXTPReportRecordItem::GetHyperlinks()
{
	if (!m_pHyperlinks)
		m_pHyperlinks = new CXTPReportHyperlinks();

	return m_pHyperlinks;
}

void CXTPReportRecordItem::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_DWord(pPX, _T("TextColor"), (DWORD&)m_clrText, XTP_REPORT_COLOR_DEFAULT);
	PX_DWord(pPX, _T("BackgroundColor"), (DWORD&)m_clrBackground, XTP_REPORT_COLOR_DEFAULT);

	PX_Bool(pPX, _T("BoldText"), m_bBoldText, FALSE);

	PX_String(pPX, _T("Format"), m_strFormatString, _T("%s"));

	PX_Bool(pPX, _T("Editable"), m_bEditable, TRUE);

	PX_Int(pPX, _T("IconIndex"), m_nIconIndex, XTP_REPORT_NOICON);

	PX_Int(pPX, _T("SortPriority"), m_nSortPriority, -1);
	PX_Int(pPX, _T("GroupPriority"), m_nGroupPriority, -1);

	PX_String(pPX, _T("GroupCaption"), m_strGroupCaption, _T(""));

	PX_Bool(pPX, _T("Focusable"), m_bFocusable, TRUE);

	PX_Int(pPX, _T("Checked"), m_checkState, 0);
	PX_Bool(pPX, _T("HasCheckbox"), m_bHasCheckbox, FALSE);
	PX_Bool(pPX, _T("TristateCheckbox"), m_bTristateCheckbox, FALSE);

	ULONGLONG ullData = m_dwData;
	PX_UI8(pPX, _T("Data"), ullData, 0);
	m_dwData = (DWORD_PTR)ullData;

	PX_String(pPX, _T("Tooltip"), m_strTooltip, _T(""));

	if (pPX->GetSchema() > _XTP_SCHEMA_98)
	{
		PX_String(pPX, _T("Caption"), m_strCaption, _T(""));
	}


	// Hyperlinks
	CXTPPropExchangeSection secHyperlinks(pPX->GetSection(_T("Hyperlinks")));

	CXTPReportHyperlinks arHyperlinks;

	if (pPX->IsStoring() && m_pHyperlinks)
	{
		arHyperlinks.CopyFrom(m_pHyperlinks);
	}

	arHyperlinks.DoPropExchange(&secHyperlinks);

	if (pPX->IsLoading())
	{
		if (arHyperlinks.GetSize())
		{
			if (GetHyperlinks())
				GetHyperlinks()->CopyFrom(&arHyperlinks);
		}
		else if (m_pHyperlinks)
		{
			m_pHyperlinks->RemoveAll();
		}
	}

	// Item controls
	CXTPReportRecordItemControls arItemControls;
	if (pPX->IsStoring() && m_pItemControls)
		arItemControls.CopyFrom(m_pItemControls);

	if (pPX->GetSchema() >= _XTP_SCHEMA_1200)
	{
		CXTPPropExchangeSection secItemControls(pPX->GetSection(_T("ItemControls")));
		arItemControls.DoPropExchange(&secItemControls);
	}
	if (pPX->IsLoading())
	{
		if (arItemControls.GetSize())
		{
			if (GetItemControls())
				GetItemControls()->CopyFrom(&arItemControls);
		}
		else if (m_pItemControls)
		{
			m_pItemControls->RemoveAll();
		}
	}

	// Update Markup data
	if (pPX->IsLoading())
	{
		XTPMarkupReleaseElement(m_pMarkupUIElement);

		CXTPMarkupContext* pMarkupContext = m_pRecord ? m_pRecord->GetMarkupContext() : NULL;

		if (pMarkupContext)
		{
			m_pMarkupUIElement = XTPMarkupParseText(pMarkupContext, m_strCaption);
		}
	}
}

INT_PTR CXTPReportRecordItem::OnToolHitTest(CPoint /*point*/, TOOLINFO* /*pTI*/)
{
	return -1;
}


void CXTPReportRecordItem::Merge(CXTPReportRecordItem *pMergeItem)
{
	m_pMergeItem = pMergeItem;
}

CXTPReportRecordItem *CXTPReportRecordItem::GetMergeItem() const
{
	return m_pMergeItem;
}

BOOL CXTPReportRecordItem::IsMerged() const
{
	return (NULL != m_pMergeItem);
}


int CXTPReportRecordItem::AddHyperlink(CXTPReportHyperlink* pHyperlink)
{
	return GetHyperlinks() ? (int)GetHyperlinks()->Add(pHyperlink) : -1;
}

int CXTPReportRecordItem::GetHyperlinksCount() const
{
	return m_pHyperlinks ? (int)m_pHyperlinks->GetSize() : 0;
}

CXTPReportHyperlink* CXTPReportRecordItem::GetHyperlinkAt(int nHyperlink) const
{
	return m_pHyperlinks ? m_pHyperlinks->GetAt(nHyperlink) : NULL;
}

void CXTPReportRecordItem::RemoveHyperlinkAt(int nHyperlink)
{
	if (m_pHyperlinks)
		m_pHyperlinks->RemoveAt(nHyperlink);
}


BOOL CXTPReportRecordItem::CreateBorder()
{
	if (NULL == m_pBorder)
	{
		m_pBorder = new CXTPReportBorder();
	}

	return (NULL != m_pBorder);
}

BOOL CXTPReportRecordItem::DestroyBorder()
{
	if (NULL != m_pBorder)
	{
		delete m_pBorder;
		m_pBorder = NULL;
	}

	return (NULL == m_pBorder);
}

CXTPReportBorder* CXTPReportRecordItem::GetBorder() const
{
	return m_pBorder;
}


