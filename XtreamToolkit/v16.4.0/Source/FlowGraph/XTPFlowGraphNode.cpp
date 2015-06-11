// XTPFlowGraphNode.cpp : implementation of the CXTPFlowGraphNode class.
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

#include "StdAfx.h"

#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollBarContainer.h>

#include "Common/XTPPropExchange.h"
#include "Common/XTPMarkupRender.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPVC80Helpers.h"
#include "Common/XTPWinThemeWrapper.h"

#include "Markup/XTPMarkupObject.h"
#include "Markup/XTPMarkupInputElement.h"
#include "Markup/XTPMarkupUIElement.h"
#include "Markup/XTPMarkupFrameworkElement.h"
#include "Markup/XTPMarkupPanel.h"
#include "Markup/XTPMarkupStackPanel.h"
#include "Markup/XTPMarkupShape.h"
#include "Markup/XTPMarkupImage.h"
#include "Markup/XTPMarkupTextBlock.h"
#include "Markup/XTPMarkupDecorator.h"
#include "Markup/XTPMarkupBorder.h"
#include "Markup/XTPMarkupDrawingContext.h"

#include "XTPFlowGraphElement.h"
#include "XTPFlowGraphNode.h"
#include "XTPFlowGraphPage.h"
#include "XTPFlowGraphControl.h"
#include "XTPFlowGraphPaintManager.h"
#include "XTPFlowGraphConnectionPoint.h"
#include "XTPFlowGraphConnectionPoints.h"
#include "XTPFlowGraphConnection.h"
#include "XTPFlowGraphConnections.h"
#include "XTPFlowGraphSelectedElements.h"
#include "XTPFlowGraphNodes.h"
#include "XTPFlowGraphNodeGroup.h"
#include "XTPFlowGraphUndoManager.h"
#include "XTPFlowGraphMessages.h"
#include "XTPFlowGraphImage.h"
#include "XTPFlowGraphTools.h"
#include "XTPFlowGraphDrawContext.h"

#include "XTPFlowGraphSelectionRect.h"

#include "Resource.h"

IMPLEMENT_SERIAL(CXTPFlowGraphNode, CXTPFlowGraphElement, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPFlowGraphNode::CXTPFlowGraphNode()
{
	m_pPage = NULL;
	m_ptLocation = CPoint(0, 0);

	m_szUserSize = CSize(0, 0);
	m_szActualSize = CSize(0, 0);

	m_clrNode = RGB(172, 172, 172); // from metallic theme

	m_bAlpha = 255;

	m_nStyle = -1;

	m_dwTag = 0;
	m_nId = (int)(INT_PTR)this;
	m_hWnd = NULL;
	m_pOriginalWindowProc = NULL;
	m_szWindow = CSize(0, 0);
	m_szUserMinSize = CSize(0,0);

	m_bResizable = TRUE;

	m_rcWindow.SetRectEmpty();

	m_pGroup = NULL;
	m_nGroupId = NULL;

	m_nArrangeIndex = -1;
	m_nArrangeLevel = -1;
	m_nArrangeComponent = -1;

	m_nImageIndex = -1;

	m_bLocked = FALSE;

	m_bVisible = TRUE;

	m_pConnectionPoints = new CXTPFlowGraphConnectionPoints(this);

	m_pNextVisibleNode = NULL;

	m_pMarkup = NULL;

	m_pSelectionRect = new CXTPFlowGraphSelectionRect();

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPFlowGraphNode::~CXTPFlowGraphNode()
{
	if (m_pConnectionPoints)
	{
		m_pConnectionPoints->RemoveAll();

		m_pConnectionPoints->InternalRelease();
		m_pConnectionPoints = NULL;
	}

	SAFE_DELETE(m_pSelectionRect);

	XTPMarkupReleaseElement(m_pMarkup);

	ResetCustomWndProc();
}

CXTPFlowGraphControl* CXTPFlowGraphNode::GetControl() const
{
	if (!m_pPage)
		return NULL;

	return m_pPage->GetControl();
}

BOOL CXTPFlowGraphNode::SetMarkupText(LPCTSTR szMarkupText)
{
	BOOL bSuccess = FALSE;

	XTPMarkupReleaseElement(m_pMarkup);

	CXTPFlowGraphControl* pControl = GetControl();

	ASSERT(pControl && pControl->GetMarkupContext());

	if (pControl && pControl->GetMarkupContext())
		m_pMarkup = XTPMarkupParseText(pControl->GetMarkupContext(), szMarkupText);

	ASSERT(m_pMarkup);

	if (m_pMarkup)
	{
		m_strMarkup = szMarkupText;

		RecalLayout();

		bSuccess = TRUE;
	}

	return bSuccess;
}


void CXTPFlowGraphNode::SetDefaultMarkup()
{
	SetMarkupText(GetControl()->GetDefaultMarkupTemplate(xtpFlowGraphTemplateNode));
}

CRect CXTPFlowGraphNode::GetMarkupVisualElementRect(const CXTPMarkupVisual *pElementVisual) const
{
	CRect rect = pElementVisual->GetBoundRect();
	rect.OffsetRect(pElementVisual->GetVisualOffset());

	// need to calculate the position of the connection point relative to the whole markup
	// the algorithm is a subject that may change

	CXTPMarkupVisual *pParent = pElementVisual->GetVisualParent();

	while (pParent)
	{
		CPoint visualOffset = pParent->GetVisualOffset();

		if (!pParent->GetVisualParent()) // here we need to stop
			break;

		rect.OffsetRect(visualOffset);

		pParent = pParent->GetVisualParent();
	}

	return rect;
}

// recursive method
void CXTPFlowGraphNode::UpdateConnectionPoints(CXTPMarkupObject *pRootElement)
{
	CXTPFlowGraphControl* pControl = GetControl();

	if (pControl && pRootElement)
	{
		// enumerating logical children (not visible), because some XAML objects (e.g. textblock)
		// are not designed to have visual elements inside, though they have
		// so we're to convert them to CXTPMarkupObject to CXTPMarkupVisual when applicable

		for (int i=0, c = pRootElement->GetLogicalChildrenCount(); i<c; i++)
		{
			CXTPMarkupObject *pElement = pRootElement->GetLogicalChild(i);

			if (pElement->IsKindOf(MARKUP_TYPE(CXTPMarkupVisual)))
			{
				CXTPMarkupVisual *pElementVisual = ((CXTPMarkupVisual*)pElement);

				CXTPMarkupString* pValueCPName = MARKUP_STATICCAST(CXTPMarkupString, pElementVisual->GetValue(pControl->GetCustomMarkupProperty(xtpFlowGraphPropertyConnectionPoint)));
				CXTPMarkupInt*    pValueCPType = MARKUP_STATICCAST(CXTPMarkupInt, pElementVisual->GetValue(pControl->GetCustomMarkupProperty(xtpFlowGraphPropertyConnectionPointType)));

				if (pValueCPName)
				{
					CRect rect = GetMarkupVisualElementRect(pElementVisual);

					// rect can be empty if connection point is xtpFlowGraphPointNone

					//ASSERT(!rect.IsRectEmpty());

					//if (!rect.IsRectEmpty())
					{
						CString cpName;

#ifdef _UNICODE
						cpName = (LPCWSTR)*pValueCPName;
#else
						cpName = XTP_CW2CT((LPCWSTR)*pValueCPName);
#endif

						XTPFlowGraphConnectionPointType type = (XTPFlowGraphConnectionPointType)(int)*pValueCPType;

						CXTPFlowGraphConnectionPoint *pPoint = GetConnectionPoints()->FindConnectionPoint(cpName, type);

						if (!pPoint) // need to create a new point
						{
							pPoint = new CXTPFlowGraphConnectionPoint();
							pPoint->SetType(type);
							pPoint->SetName(cpName);
							pPoint->SetCaption(cpName);

							// we get textblock text as node's caption
							CXTPMarkupTextBlock* pNodeRowTextBlock = GetTableRowTextBlock(cpName);

							ASSERT(pNodeRowTextBlock);

							if (pNodeRowTextBlock)
							{
								pPoint->SetCaption(pNodeRowTextBlock->GetText());
							}

							m_pConnectionPoints->AddConnectionPoint(pPoint);
						}
						else
						{
							// existing point
						}

						pPoint->m_bMarkupPoint = TRUE; // must be here, not when creating

						pPoint->SetRect(rect);
					}
				}
			}

			UpdateConnectionPoints(pElement);
		}
	}
}

void CXTPFlowGraphNode::SetWindowSize(CSize sz)
{
	ASSERT(m_pMarkup);

	if (m_pMarkup)
	{
		RecalLayout();

		m_szWindow.cx = max(m_szActualSize.cx, sz.cx);
		m_szWindow.cy = sz.cy;

		CXTPMarkupPanel* pNodeCustomControl = MARKUP_DYNAMICCAST(CXTPMarkupPanel, m_pMarkup->FindName(XAML_MARKUP_TAG_CUSTOMCONTROL) );

		ASSERT(pNodeCustomControl);

		if (pNodeCustomControl)
		{
			/*CXTPMarkupVisual *pParent = pNodeCustomControl->GetVisualParent();

			ASSERT(pParent);

			int iMinWidth = pNodeCustomControl->GetMinWidth();
			int iWidth = pNodeCustomControl->GetWidth();
			int iParentRenderWidth = pParent->GetRenderSize().cx;*/

			pNodeCustomControl->SetWidth(m_szWindow.cx);
			pNodeCustomControl->SetHeight(m_szWindow.cy);
		}

		OnGraphChanged();
	}
}

void CXTPFlowGraphNode::UpdateMarkupImage()
{
	ASSERT(m_pMarkup);

	if (m_pMarkup && m_nImageIndex != -1)
	{
		CXTPMarkupImage* pNodeImage = MARKUP_DYNAMICCAST(CXTPMarkupImage, m_pMarkup->FindName(XAML_MARKUP_TAG_NODEIMAGE) );

		if (pNodeImage)
		{
			TCHAR szBuff[11];
			ITOT_S(m_nImageIndex, szBuff, sizeof(szBuff)/sizeof(TCHAR));

			pNodeImage->SetSource(szBuff);
		}
	}
}

void CXTPFlowGraphNode::SetImageIndex(int nImageIndex)
{
	m_nImageIndex = nImageIndex;

	UpdateMarkupImage();
}

void CXTPFlowGraphNode::UpdateWindowPosition()
{
	if (!m_hWnd)
		return;

	CXTPMarkupPanel* pNodeCustomControl = MARKUP_DYNAMICCAST(CXTPMarkupPanel, m_pMarkup->FindName(XAML_MARKUP_TAG_CUSTOMCONTROL) );

	if (pNodeCustomControl)
	{
		CRect rc = GetMarkupVisualElementRect(pNodeCustomControl);
		rc.OffsetRect(GetLocation());
		rc = m_pPage->PageToScreen(rc);

		// position
		::SetWindowPos(m_hWnd, NULL, rc.left, rc.top, rc.Width(), rc.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
	}
}

int CXTPFlowGraphNode::RemoveMarkupTableRow(CXTPFlowGraphConnectionPoint *pPoint)
{
	int index = -1;

	LPCWSTR szNameToSearch = NULL;

#ifdef _UNICODE
	szNameToSearch = pPoint->GetName();
#else
	CXTPResourceManager::CXTPA2W conversion(pPoint->GetName());
	szNameToSearch = conversion;
#endif

	CXTPMarkupBorder* pNodeItem = MARKUP_DYNAMICCAST(CXTPMarkupBorder, m_pMarkup->FindName(szNameToSearch) );

	//ASSERT(pNodeItem);
	// can be NULL when removing IN & OUT point, markup will be removed after first point removal

	if (pNodeItem)
	{
		CXTPMarkupUIElementCollection *pChildren = NULL;

		CXTPFlowGraphControl* pControl = GetControl();

		ASSERT(m_pMarkup && pControl);

		if (m_pMarkup && pControl)
		{
			CXTPMarkupPanel* pNodeItemsCollection = MARKUP_DYNAMICCAST(CXTPMarkupPanel, m_pMarkup->FindName(XAML_MARKUP_TAG_ITEMSCOLLECTION) ) ;

			if (pNodeItemsCollection)
			{
				pChildren = pNodeItemsCollection->GetChildren();
			}
		}

		ASSERT(pChildren);

		if (pChildren)
		{
			index = pChildren->IndexOf(pNodeItem);

			ASSERT(index != -1);

			if (index != -1)
			{
				pChildren->Remove(index);
			}
		}
	}

	return index;
}

CXTPMarkupTextBlock *CXTPFlowGraphNode::GetTableRowTextBlock(const CString &strConnectionPointName) const
{
	CXTPMarkupTextBlock *pReturnTextBlock = NULL;

	LPCWSTR szNameToSearch = NULL;

#ifdef _UNICODE
	szNameToSearch = strConnectionPointName;
#else
	CXTPResourceManager::CXTPA2W conversion(strConnectionPointName);
	szNameToSearch = conversion;
#endif

	CXTPMarkupBorder* pNodeItem = MARKUP_DYNAMICCAST(CXTPMarkupBorder, m_pMarkup->FindName(szNameToSearch) );

	ASSERT(pNodeItem);

	if (pNodeItem)
	{
		pReturnTextBlock = MARKUP_DYNAMICCAST(CXTPMarkupTextBlock, pNodeItem->FindName(XAML_MARKUP_TAG_NODE_ITEM_CAPTION) );
	}

	return pReturnTextBlock;
}

CRect CXTPFlowGraphNode::GetTableRowCaptionRect(CXTPFlowGraphConnectionPoint *pPoint) const
{
	CRect rect;
	rect.SetRectEmpty();

	CXTPMarkupTextBlock* pNodeItemCaption = GetTableRowTextBlock( pPoint->GetName() );

	ASSERT(pNodeItemCaption);

	if (pNodeItemCaption)
	{
		rect = GetMarkupVisualElementRect(pNodeItemCaption);
		rect.OffsetRect(GetLocation());
	}

	return rect;
}

void CXTPFlowGraphNode::UpdateMarkupPointCaption(CXTPFlowGraphConnectionPoint *pPoint)
{
	CXTPMarkupTextBlock* pNodeItemCaption = GetTableRowTextBlock(pPoint->GetName());

	ASSERT(pNodeItemCaption);

	if (pNodeItemCaption)
	{
		LPCWSTR szNameToSet = NULL;

#ifdef _UNICODE
		szNameToSet = pPoint->GetCaption();
#else
		CXTPResourceManager::CXTPA2W conversion(pPoint->GetCaption());
		szNameToSet = conversion;
#endif
		pNodeItemCaption->SetText(szNameToSet);
	}
}

void CXTPFlowGraphNode::UpdateMarkupPointName(CXTPFlowGraphConnectionPoint *pPoint)
{
	// TODO: update CPName property of rectangles

	CXTPMarkupTextBlock* pNodeItemCaption = GetTableRowTextBlock(pPoint->GetName());

	ASSERT(pNodeItemCaption);

	if (pNodeItemCaption)
	{
		pNodeItemCaption->SetName(pPoint->GetName());
	}
}

void CXTPFlowGraphNode::UpdateMarkupPointColor(CXTPFlowGraphConnectionPoint *pPoint)
{
	CXTPMarkupTextBlock* pNodeItemCaption = GetTableRowTextBlock(pPoint->GetName());

	ASSERT(pNodeItemCaption);

	if (pNodeItemCaption)
	{
		CXTPMarkupSolidColorBrush *pBrush = new CXTPMarkupSolidColorBrush(CXTPFlowGraphPaintManager::GetDarkColor(pPoint->GetColor()));

		pNodeItemCaption->SetForeground(pBrush);
	}
}

void CXTPFlowGraphNode::UpdateMarkupPointImage(CXTPFlowGraphConnectionPoint *pPoint)
{
	int iImageIndex = pPoint->GetImageIndex();

	ASSERT(m_pMarkup);

	if (m_pMarkup && iImageIndex != -1)
	{
		LPCWSTR szNameToSearch = NULL;

#ifdef _UNICODE
		szNameToSearch = pPoint->GetName();
#else
		CXTPResourceManager::CXTPA2W conversion(pPoint->GetName());
		szNameToSearch = conversion;
#endif

		CXTPMarkupBorder* pNodeItem = MARKUP_DYNAMICCAST(CXTPMarkupBorder, m_pMarkup->FindName(szNameToSearch) );

		ASSERT(pNodeItem);

		if (pNodeItem)
		{
			CXTPMarkupImage* pNodeImage = MARKUP_DYNAMICCAST(CXTPMarkupImage, pNodeItem->FindName(XAML_MARKUP_TAG_NODE_ITEM_IMAGE) );

			if (pNodeImage)
			{
				TCHAR szBuff[11];
				ITOT_S(iImageIndex, szBuff, sizeof(szBuff)/sizeof(TCHAR));

				pNodeImage->SetSource(szBuff);
			}
		}
	}
}

int CXTPFlowGraphNode::GetMarkupTableRowIndex(CXTPFlowGraphConnectionPoint *pPoint, CXTPMarkupUIElementCollection *pChildren)
{
	int index = -1;

	ASSERT(pChildren);

	if (pChildren)
	{
		LPCWSTR szNameToSearch = NULL;

#ifdef _UNICODE
		szNameToSearch = pPoint->GetName();
#else
		CXTPResourceManager::CXTPA2W conversion(pPoint->GetName());
		szNameToSearch = conversion;
#endif

		CXTPMarkupBorder* pNodeItem = MARKUP_DYNAMICCAST(CXTPMarkupBorder, m_pMarkup->FindName(szNameToSearch) );

		ASSERT(pNodeItem);

		if (pNodeItem)
		{
			index = pChildren->IndexOf(pNodeItem);
		}
	}

	return index;
}

void CXTPFlowGraphNode::AddNamedConnectionPoints(LPCTSTR szName, XTPFlowGraphConnectionPointType type /*= xtpFlowGraphPointNone*/, LPCTSTR szCaption /*= NULL*/)
{
	BOOL bOk = FALSE;

	CXTPFlowGraphControl* pControl = GetControl();

	ASSERT(m_pMarkup && pControl);

	if (m_pMarkup && pControl)
	{
		CXTPMarkupPanel* pNodeItemsCollection = MARKUP_DYNAMICCAST(CXTPMarkupPanel, m_pMarkup->FindName(XAML_MARKUP_TAG_ITEMSCOLLECTION) ) ;

		if (pNodeItemsCollection)
		{
			CXTPMarkupUIElementCollection *pChildren = pNodeItemsCollection->GetChildren();

			ASSERT(pChildren);

			if (pChildren)
			{
				CString markupTemplate;
				int iIndexToInsert = pChildren->GetCount(); // append

				CXTPFlowGraphConnectionPoint *pExistingPoint = NULL;

				// now need to check if we should merge this point with other (same caption)
				if (type == xtpFlowGraphPointInput || type == xtpFlowGraphPointOutput)
				{
					pExistingPoint = GetConnectionPoints()->FindConnectionPoint(szName, (type == xtpFlowGraphPointInput) ? xtpFlowGraphPointOutput : xtpFlowGraphPointInput);

					if (pExistingPoint)
						type = xtpFlowGraphPointInputAndOutput;
				}
				else if (type == xtpFlowGraphPointInputAndOutput)
				{
					pExistingPoint = GetConnectionPoints()->FindConnectionPoint(szName, xtpFlowGraphPointInput);

					if (!pExistingPoint)
						pExistingPoint = GetConnectionPoints()->FindConnectionPoint(szName, xtpFlowGraphPointOutput);
				}

				if (pExistingPoint)
				{
					// now we need to remove the existing point's XAML markup from the children list
					iIndexToInsert = GetMarkupTableRowIndex(pExistingPoint, pChildren);

					// the XAML markup will be removed when calling CXTPFlowGraphConnectionPoint::OnRemoved()
					// do not remove pairs !
					GetConnectionPoints()->Remove(pExistingPoint, FALSE);
				}

				switch (type)
				{
					case xtpFlowGraphPointNone:
						markupTemplate = GetControl()->GetDefaultMarkupTemplate(xtpFlowGraphTemplateNodePointNone);
						break;
					case xtpFlowGraphPointInput:
						markupTemplate = GetControl()->GetDefaultMarkupTemplate(xtpFlowGraphTemplateNodePointIn);
						break;
					case xtpFlowGraphPointOutput:
						markupTemplate = GetControl()->GetDefaultMarkupTemplate(xtpFlowGraphTemplateNodePointOut);
						break;
					case xtpFlowGraphPointInputAndOutput:
						markupTemplate = GetControl()->GetDefaultMarkupTemplate(xtpFlowGraphTemplateNodePointInOut);
						break;
				}

				ASSERT(markupTemplate.GetLength());

				if (markupTemplate.GetLength())
				{
					CString markup = markupTemplate;
					markup.Replace(XAML_MARKUP_CPNAME_PLACEHOLDER, szName);
					markup.Replace(XAML_MARKUP_CPCAPTION_PLACEHOLDER, szCaption ? szCaption : szName);

					CXTPMarkupUIElement *pTableRow = XTPMarkupParseText(pControl->GetMarkupContext(), markup);

					if (pTableRow)
					{
						pChildren->Insert(iIndexToInsert, pTableRow);

						bOk = TRUE;
					}
					else
					{
						ASSERT(pTableRow);
					}
				}
			}
		}
	}

	if (bOk)
		RecalLayout();
}

void CXTPFlowGraphNode::UpdateMarkupFont()
{
	if (m_pMarkup && GetControl() && GetControl()->GetPaintManager())
	{
		// apply font

		HFONT m_hTextFont = CreateFontIndirect(&GetControl()->GetPaintManager()->m_lfText);

		XTPMarkupSetDefaultFont(XTPMarkupElementContext(m_pMarkup),
			(HFONT)m_hTextFont, GetControl()->GetPaintManager()->m_clrTextColor);

		DeleteObject(m_hTextFont);
	}
}

void CXTPFlowGraphNode::CalcMinSize()
{
	int iMarkupRowHeight = 0;

	int iMinHeight = -1;
	int iMinWidth = -1;

	ASSERT(m_pMarkup);

	if (m_pMarkup)
	{
		CXTPMarkupBorder* pNodeCaptionBorder = MARKUP_DYNAMICCAST(CXTPMarkupBorder, m_pMarkup->FindName(XAML_MARKUP_TAG_NODECAPTIONBORDER) );

		if (pNodeCaptionBorder)
		{
			iMarkupRowHeight = pNodeCaptionBorder->GetActualSize().cy;
		}

		CXTPMarkupPanel* pNodeItemsCollection = MARKUP_DYNAMICCAST(CXTPMarkupPanel, m_pMarkup->FindName(XAML_MARKUP_TAG_ITEMSCOLLECTION) ) ;

		if (pNodeItemsCollection)
		{
			if (iMarkupRowHeight)
				iMinHeight = (pNodeItemsCollection->GetChildren()->GetCount() + 1) * iMarkupRowHeight;
		}

		// calculate min width
		// take caption width at first

		CXTPMarkupTextBlock* pNodeCaption = GetCaptionTextBlock();

		if (pNodeCaption)
		{
			iMinWidth = pNodeCaption->GetActualSize().cx + 40; // 40 is 20 left and right
		}

		// calculate depending on table row captions
		if (pNodeItemsCollection && pNodeItemsCollection->GetChildren())
		{
			for (int i=0, c = pNodeItemsCollection->GetChildren()->GetCount(); i<c; i++)
			{
				CSize sz = pNodeItemsCollection->GetChildren()->GetItem(i)->GetDesiredSize();

				if (sz.cx > iMinWidth)
					iMinWidth = sz.cx;
			}
		}
	}

	if (m_hWnd)
	{
		CRect rc;
		if (::GetWindowRect(m_hWnd, rc))
		{
			iMinHeight += rc.Height();

			iMinWidth = max(iMinWidth, rc.Width());
		}
	}

	m_szMinSize = CSize(iMinWidth, iMinHeight);
}

void CXTPFlowGraphNode::RecalLayout()
{
	ASSERT(m_pMarkup);

	if (m_pMarkup)
	{
		UpdateMarkupFont();

		CSize szMarkupSize = XTPMarkupMeasureElement(m_pMarkup, INT_MAX, INT_MAX);

		m_szActualSize = szMarkupSize;

		CRect rc(GetLocation(), GetSize());

		// connection points markup (rects) size and position is actually CRect(0,0,0,0) before it's first arranged
		// so we need to arrange the markup within a rect
		m_pMarkup->Arrange(rc);

		UpdateMarkupCaption();

		if (m_szUserMinSize == CSize(0,0))
			CalcMinSize();
		else
		{
			m_szMinSize = m_szUserMinSize;
		}

		UpdateConnectionPoints(m_pMarkup);

		COLORREF clrColorCurrent = m_clrNode;

		if (GetControl()->GetPaintManager() && GetControl()->GetPaintManager()->m_bChangeNodeColorWhenSelected)
		{
			if (IsSelected())
				clrColorCurrent = GetControl()->GetPaintManager()->m_clrSelection;
		}
		else
		{
			UpdateCaptionColor();
		}

		UpdateColorRecursive(m_pMarkup, clrColorCurrent);

		UpdateMarkupImage();
	}
}

void CXTPFlowGraphNode::UpdateColorRecursive(CXTPMarkupObject *pRootElement, COLORREF clrColor)
{
	CXTPFlowGraphControl* pControl = GetControl();

	if (pControl && pRootElement)
	{
		for (int i=0, c = pRootElement->GetLogicalChildrenCount(); i<c; i++)
		{
			CXTPMarkupObject *pElement = pRootElement->GetLogicalChild(i);

			if (pElement->IsKindOf(MARKUP_TYPE(CXTPMarkupBorder)))
			{
				CXTPMarkupBorder *pBorder = MARKUP_STATICCAST(CXTPMarkupBorder, pElement);

				CXTPMarkupSolidColorBrush *pBrush = new CXTPMarkupSolidColorBrush(CXTPFlowGraphPaintManager::GetDarkColor(clrColor));

				pBorder->SetBorderBrush(pBrush);

				CXTPMarkupDependencyProperty *pPropertyCanSetColor = pControl->GetCustomMarkupProperty(xtpFlowGraphPropertyCanSetColor);

				if (pPropertyCanSetColor)
				{
					CXTPMarkupBool *pCanSetColor = MARKUP_STATICCAST(CXTPMarkupBool, pElement->GetValue(pPropertyCanSetColor));

					if (pCanSetColor)
					{
						CXTPMarkupSolidColorBrush *pBrushBackground = new CXTPMarkupSolidColorBrush(clrColor);

						pBorder->SetBackground(pBrushBackground);
					}
				}
			}

			UpdateColorRecursive(pElement, clrColor);
		}
	}
}

void CXTPFlowGraphNode::SetColor(COLORREF clr, BYTE bAlpha /*= 255*/)
{
	// default markup nodes does not support alpha channel

	m_clrNode = clr;
	m_bAlpha = bAlpha;

	ASSERT(m_pMarkup);

	if (m_pMarkup)
		UpdateColorRecursive(m_pMarkup, m_clrNode);

	OnGraphChanged();
}

void CXTPFlowGraphNode::DrawConnectionPoints(CXTPFlowGraphDrawContext* pDC)
{
	// TODO: get rid of color constants

	bool bDrawConnectionPoints = true;

	if (GetControl())
	{
		if (GetControl()->GetPaintManager()->m_bAlwaysDrawConnectionPoints == FALSE && IsSelected() == FALSE)
		{
			bDrawConnectionPoints = false;
		}
	}

	if (bDrawConnectionPoints)
	{
		int nCount = GetConnectionPoints()->GetCount();

		for (int i = 0; i < nCount; i++)
		{
			CXTPFlowGraphConnectionPoint* pPoint = GetConnectionPoints()->GetAt(i);
			CRect rc = pPoint->GetRect();
			rc.OffsetRect(GetLocation());

			COLORREF clrSelection = RGB(147, 148, 198);

			COLORREF clrEllipse = pPoint->GetColor();
			if (clrEllipse == (COLORREF)RGB(0,0,0))
				clrEllipse = RGB(182, 182, 182);

			COLORREF clrPen = CXTPFlowGraphPaintManager::GetDarkColor(m_clrNode);
			COLORREF clrBrush = pPoint->IsSelected() ? clrSelection : pPoint->GetConnectionsCount() > 0 ? RGB(250, 252, 58) : clrEllipse;

			pDC->SetPen(clrPen, 1);
			pDC->SetBrush(clrBrush);
			pDC->Ellipse(rc);
		}

		/* // draw simple rectangle for testing
		for (int i=0, c = GetConnectionPoints()->GetCount(); i<c; i++)
		{
			CXTPFlowGraphConnectionPoint *pPoint = GetConnectionPoints()->GetAt(i);

			CRect connectionPointRect = pPoint->GetRect();
			connectionPointRect.OffsetRect(GetLocation());

			pDC->FillSolidRect(connectionPointRect, RGB(255,0,0));
		}
		*/
	}
}

CRect CXTPFlowGraphNode::GetSelectionRect() const
{
	CRect rc(GetLocation(), GetSize());
	rc.InflateRect(GetControl()->GetPaintManager()->m_nEllipseSize, GetControl()->GetPaintManager()->m_nEllipseSize); // for connection points ellipses

	return rc;
}

void CXTPFlowGraphNode::DrawSelectionRect(CXTPFlowGraphDrawContext* pDC)
{
	if (!IsResizable())
		return;

	if (GetControl() && !GetControl()->GetAllowResizeNodes() )
		return;

	CXTPFlowGraphSelectedElements* pSelection = GetControl()->GetActivePage()->GetSelection();

	if (!IsSelected() || (pSelection && pSelection->GetCount() > 1) )
		return;

	m_pSelectionRect->Draw(pDC, GetSelectionRect());
}

void CXTPFlowGraphNode::Draw(CXTPFlowGraphDrawContext* pDC)
{
	if (!IsVisible())
		return;

	ASSERT(m_pMarkup);

	if (m_pMarkup)
	{
		CRect rc(GetLocation(), GetSize());

		pDC->DrawMarkupElement(m_pMarkup, rc, 0);

		DrawConnectionPoints(pDC);

		DrawWindowContent(pDC);

		DrawSelectionRect(pDC);
	}

	UpdateWindowPosition();
}

CRect CXTPFlowGraphNode::GetBoundingRectangle(BOOL bIncludeConnectionPointsEllipses /*= TRUE*/) const
{
	CRect rc(GetLocation(), GetSize());

	if (bIncludeConnectionPointsEllipses)
	{
		if (GetControl() && GetControl()->GetPaintManager())
		{
			rc.InflateRect(GetControl()->GetPaintManager()->m_nEllipseSize / 2, 0);
		}
		else
		{
			ASSERT(FALSE);
		}
	}

	return rc;
}

CRect CXTPFlowGraphNode::GetScreenRect(BOOL bIncludeConnectionPointsEllipses /*= TRUE*/) const
{
	CRect rc = GetBoundingRectangle(bIncludeConnectionPointsEllipses);

	return m_pPage->PageToScreen(rc);
}

CXTPFlowGraphImage* CXTPFlowGraphNode::GetImage() const
{
	CXTPFlowGraphControl* pControl = GetControl();
	if (!pControl)
		return NULL;

	return pControl->GetImages()->GetAt(m_nImageIndex);
}

void CXTPFlowGraphNode::DoCommonPropExchange(CXTPPropExchange* pPX)
{
	PX_String(pPX, _T("Caption"), m_strCaption, _T(""));
	PX_String(pPX, _T("Tooltip"), m_strTooltip, _T(""));
	PX_Point(pPX, _T("Location"), m_ptLocation, CPoint(0, 0));
	PX_DWord(pPX, _T("Color"), (DWORD)m_clrNode, (COLORREF)-1);
	PX_Byte(pPX, _T("Alpha"), m_bAlpha, 255);
	PX_Int(pPX, _T("Style"), m_nStyle, (int)-1);
	PX_Int(pPX, _T("ImageIndex"), m_nImageIndex, (int)-1);

	if (pPX->IsStoring())
	{
		if (m_szUserSize == CSize(0,0))
		{
			m_szUserSize = m_szActualSize;
		}

		// when saving it's required to check, because XTPMeasureMarkupElement
		// returns 2px more for the whole markup

		m_szUserSize.cy -=2;
	}

	PX_Size(pPX, _T("UserSize"), m_szUserSize, CSize(0, 0));
	PX_Bool(pPX, _T("Locked"), m_bLocked, FALSE);

	DWORD dwTag = (DWORD)m_dwTag;
	PX_DWord(pPX, _T("Tag"), (DWORD)dwTag, 0);
	if (pPX->IsLoading())
		m_dwTag = dwTag;

	PX_Int(pPX, _T("Id"), m_nId, 0);

	if (pPX->IsLoading())
	{
		PX_Int(pPX, _T("GroupId"), m_nGroupId, 0);
	}
	else
	{
		m_nGroupId = m_pGroup ? (int)(INT_PTR)m_pGroup : 0;
		PX_Int(pPX, _T("GroupId"), m_nGroupId, 0);
	}
}

void CXTPFlowGraphNode::DoPropExchange(CXTPPropExchange* pPX)
{
	DoCommonPropExchange(pPX);

	m_pConnectionPoints->DoPropExchange(pPX); // only for default nodes

	if (pPX->IsLoading())
	{
		//SetMarkupText(m_strMarkup); // only in custom nodes

		SetSize(m_szUserSize); // apply size to markup

		UpdateMarkupCaption();

		UpdateCaptionColor();
	}
}

CXTPFlowGraphConnectionPoint* CXTPFlowGraphNode::HitTestConnectionPoint(CPoint point)
{
	point = m_pPage->ScreenToPage(point);
	point.Offset(-m_ptLocation);

	// we should check for a XAML table row for default nodes

	CXTPMarkupUIElementCollection *pChildren = NULL;

	CXTPFlowGraphControl* pControl = GetControl();

	ASSERT(m_pMarkup && pControl);

	if (m_pMarkup && pControl)
	{
		CXTPMarkupPanel* pNodeItemsCollection = MARKUP_DYNAMICCAST(CXTPMarkupPanel, m_pMarkup->FindName(XAML_MARKUP_TAG_ITEMSCOLLECTION) ) ;

		if (pNodeItemsCollection)
		{
			pChildren = pNodeItemsCollection->GetChildren();
		}
	}

	ASSERT(pChildren);

	if (pChildren)
	{
		for (int i=0, c = pChildren->GetCount(); i<c; i++)
		{
			CXTPMarkupUIElement *pChild = pChildren->GetItem(i);

			if (pChild)
			{
				CRect rc = GetMarkupVisualElementRect(pChild);

				if (rc.PtInRect(point))
				{
					CXTPMarkupString *pName = pChild->GetName();

					if (pName)
					{
						for (int k = 0; k < m_pConnectionPoints->GetCount(); k++)
						{
							CXTPFlowGraphConnectionPoint* pPoint = m_pConnectionPoints->GetAt(k);

							LPCWSTR szNameToCompare = NULL;

#ifdef _UNICODE
							szNameToCompare = pPoint->GetName();
#else
							CXTPResourceManager::CXTPA2W conversion(pPoint->GetName());
							szNameToCompare = conversion;
#endif

							if (!wcscmp(*pName,szNameToCompare))
								return pPoint;
						}
					}
				}
			}
		}
	}

	return NULL;
}

void CXTPFlowGraphNode::Remove()
{
	if (!m_pPage)
		return;

	m_pPage->GetNodes()->Remove(this);
}

void CXTPFlowGraphNode::OnRemoved()
{
	m_pConnectionPoints->RemoveAll();

	if (m_pGroup)
	{
		m_pGroup->RemoveNode(this);
	}

	CXTPFlowGraphElement::OnRemoved();
}

void CXTPFlowGraphNode::SetLocation(CPoint ptLocation)
{
	CXTPFlowGraphControl* pControl = GetControl();
	if (pControl)
	{
		pControl->GetUndoManager()->AddUndoCommand(new CXTPFlowGraphUndoSetNodePositionCommand(this, m_ptLocation));
	}

	m_ptLocation = ptLocation;
	OnGraphChanged();

	if (pControl)
	{
		XTP_NM_FLOWGRAPH_NODECHANGED nc;
		nc.pNode = this;

		pControl->SendNotifyMessage(XTP_FGN_NODELOCATIONCHANGED, &nc.hdr);
	}
}

void CXTPFlowGraphNode::SetSizeCore(CSize sz)
{
	CXTPFlowGraphControl* pControl = GetControl();
	if (pControl)
	{
		pControl->GetUndoManager()->AddUndoCommand(new CXTPFlowGraphUndoSetNodeSizeCommand(this, m_szUserSize));
	}

	m_szUserSize = sz;
	OnGraphChanged();

	if (pControl)
	{
		XTP_NM_FLOWGRAPH_NODECHANGED nc;
		nc.pNode = this;

		pControl->SendNotifyMessage(XTP_FGN_NODELOCATIONCHANGED, &nc.hdr);
	}

	RecalLayout();
}

void CXTPFlowGraphNode::SetSize(CSize sz)
{
	ASSERT(m_pMarkup);

	if (m_pMarkup)
	{
		CXTPMarkupStackPanel* pNodeContent = MARKUP_DYNAMICCAST(CXTPMarkupStackPanel, m_pMarkup->FindName(XAML_MARKUP_TAG_NODECONTENT) );

		ASSERT(pNodeContent);

		if (pNodeContent)
		{
			pNodeContent->SetWidth(sz.cx);

			pNodeContent->SetHeight(sz.cy);
		}
	}

	SetSizeCore(sz);
}

CSize CXTPFlowGraphNode::GetSize() const
{
	return m_szActualSize;
}

CSize CXTPFlowGraphNode::GetUserSize() const
{
	return m_szUserSize;
}

CSize CXTPFlowGraphNode::GetMinSize() const
{
	return m_szMinSize;
}

void CXTPFlowGraphNode::SetMinSize(CSize sz)
{
	m_szUserMinSize = sz;

	RecalLayout();
}

void CXTPFlowGraphNode::SetCaption(LPCTSTR lpszCaption)
{
	CXTPFlowGraphControl* pControl = GetControl();
	if (pControl)
	{
		pControl->GetUndoManager()->AddUndoCommand(new CXTPFlowGraphUndoSetNodeCaptionCommand(this, m_strCaption));
	}

	m_strCaption = lpszCaption;

	UpdateMarkupCaption();

	OnGraphChanged();
}

CXTPMarkupTextBlock* CXTPFlowGraphNode::UpdateMarkupCaption()
{
	CXTPMarkupTextBlock* pNodeCaption = GetCaptionTextBlock();

	if (pNodeCaption)
	{
		CString strCaption = GetCaption();

#ifdef _UNICODE
		pNodeCaption->SetText(strCaption);
#else
		pNodeCaption->SetText(XTP_CA2CW(strCaption));
#endif
	}

	return pNodeCaption;
}

CRect CXTPFlowGraphNode::GetCaptionRect() const
{
	CRect rect;
	rect.SetRectEmpty();

	CXTPMarkupTextBlock* pNodeCaption = GetCaptionTextBlock();

	if (pNodeCaption)
	{
		rect = GetMarkupVisualElementRect(pNodeCaption);
		rect.OffsetRect(GetLocation());
	}

	return rect;
}

CXTPMarkupTextBlock* CXTPFlowGraphNode::GetCaptionTextBlock() const
{
	CXTPMarkupTextBlock* pNodeCaption = NULL;

	ASSERT(m_pMarkup);

	if (m_pMarkup)
	{
		pNodeCaption = MARKUP_DYNAMICCAST(CXTPMarkupTextBlock, m_pMarkup->FindName(XAML_MARKUP_TAG_NODECAPTION) );
	}

	return pNodeCaption;
}

void CXTPFlowGraphNode::UpdateCaptionColor()
{
	CXTPMarkupTextBlock* pNodeCaption = GetCaptionTextBlock();

	if (pNodeCaption)
	{
		if (GetControl() && GetControl()->GetPaintManager())
		{
			CXTPMarkupSolidColorBrush *pBrush = new CXTPMarkupSolidColorBrush(IsSelected() ?
				GetControl()->GetPaintManager()->m_clrTextColorSelected :
			GetControl()->GetPaintManager()->m_clrTextColor);

			pNodeCaption->SetForeground(pBrush);
		}
		else
		{
			ASSERT(FALSE);
		}
	}
}

void CXTPFlowGraphNode::DrawWindowContent(CXTPFlowGraphDrawContext* pDC)
{
	if (m_hWnd && pDC->IsSavingToFile())
	{
		CXTPMarkupPanel* pNodeCustomControl = MARKUP_DYNAMICCAST(CXTPMarkupPanel, m_pMarkup->FindName(XAML_MARKUP_TAG_CUSTOMCONTROL) );

		if (pNodeCustomControl)
		{
			CRect rc;
			::GetClientRect(m_hWnd, &rc);

			if (!rc.IsRectEmpty())
			{
				CRect wrc = GetMarkupVisualElementRect(pNodeCustomControl);
				wrc.OffsetRect(GetLocation());

				HDC hDC = ::GetDC(m_hWnd);

				BitBlt(pDC->GetHDC(), wrc.left, wrc.top, rc.right, rc.bottom, hDC, 0, 0, SRCCOPY);

				::ReleaseDC(m_hWnd, hDC);
			}
		}
	}
}


void CXTPFlowGraphNode::Select(BOOL bSelect /*= TRUE*/)
{
	CXTPFlowGraphControl* pControl = GetControl();
	if (!pControl)
		return;

	CXTPFlowGraphSelectedElements* pSelectedElements = m_pPage->GetSelection();

	if (GetGroup() != NULL)
	{
		for (int j = 0; j < GetGroup()->GetCount(); j++)
		{
			CXTPFlowGraphNode* pNode = GetGroup()->GetAt(j);

			bSelect ? pSelectedElements->AddSelection(pNode) : pSelectedElements->Remove(pNode);
		}
	}
	else
	{
		bSelect ? pSelectedElements->AddSelection(this) : pSelectedElements->Remove(this);
	}
}

CMap<HWND, HWND, CXTPFlowGraphNode*, CXTPFlowGraphNode *> CXTPFlowGraphNode::m_WindowToNodeHash; // used in the custom window 'ControlWindowProc'

LRESULT CALLBACK CXTPFlowGraphNode::ControlWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) // new control window procedure, static
{
	CXTPFlowGraphNode *pThis = NULL;
	CXTPFlowGraphNode::m_WindowToNodeHash.Lookup(hwnd, pThis);

	ASSERT(pThis);

	if (pThis) // shoudn't be NULL, really
	{
#ifdef _DEBUG // debug check
		ASSERT(!IsBadReadPtr (pThis, sizeof(CXTPFlowGraphNode)));
#endif

		switch (uMsg)
		{
			case WM_LBUTTONDOWN:
			{
				// unselect all previous items. If control or shift key is pressed - don't
				if (pThis->m_pPage && !(GetKeyState(VK_CONTROL) < 0 || GetKeyState(VK_SHIFT) < 0))
					pThis->m_pPage->GetSelection()->Clear();

				pThis->Select(!pThis->IsSelected()); // invert

				break;
			}

			case WM_RBUTTONDOWN:
			{
				// unselect all previous items
				if (pThis->m_pPage)
					pThis->m_pPage->GetSelection()->Clear();

				pThis->Select(!pThis->IsSelected()); // just set selected

				break;
			}
		}

		return ::CallWindowProc(pThis->m_pOriginalWindowProc, hwnd, uMsg, wParam, lParam);
	}

	return 1; // unsupported behaviour, though better than a crash
}

void CXTPFlowGraphNode::SetCustomWndProc()
{
	if (m_hWnd)
	{
		DWORD dwWindowProcessID = 0;
		GetWindowThreadProcessId(m_hWnd, &dwWindowProcessID);

		if (dwWindowProcessID == GetCurrentProcessId())
		{
			CXTPFlowGraphNode::m_WindowToNodeHash[m_hWnd] = this; // store a pointer to this

			m_pOriginalWindowProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR> (ControlWindowProc) ) );

			::CallWindowProc(m_pOriginalWindowProc, m_hWnd, WM_NULL, 0, 0);
		}
	}
}

void CXTPFlowGraphNode::ResetCustomWndProc()
{
	if (m_hWnd && m_pOriginalWindowProc) // some control already exist
	{
		// restore previous window proc
		SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR> (m_pOriginalWindowProc) );

		CXTPFlowGraphNode::m_WindowToNodeHash.RemoveKey(m_hWnd);

		m_pOriginalWindowProc = NULL;
	}
}

void CXTPFlowGraphNode::SetWindowHandle(HWND hWnd)
{
	ResetCustomWndProc(); // if any

	m_hWnd = hWnd;

	SetCustomWndProc();

	OnGraphChanged();
}

void CXTPFlowGraphNode::SetVisible(BOOL bVisible /*= TRUE*/)
{
	if (m_bVisible != bVisible)
	{
		m_bVisible = bVisible;

		OnGraphChanged();
	}
}

#ifdef _XTP_ACTIVEX


BEGIN_DISPATCH_MAP(CXTPFlowGraphNode, CXTPFlowGraphElement)
	DISP_PROPERTY_EX_ID(CXTPFlowGraphNode, "Caption", DISPID_CAPTION, OleGetCaption, OleSetCaption, VT_BSTR)
	DISP_PROPERTY_EX_ID(CXTPFlowGraphNode, "Tooltip", 2, OleGetTooltip, OleSetTooltip, VT_BSTR)
	DISP_PROPERTY_EX_ID(CXTPFlowGraphNode, "Color", 3, OleGetColor, OleSetColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPFlowGraphNode, "Style", 4, OleGetStyle, OleSetStyle, VT_I4)
	DISP_PROPERTY_ID(CXTPFlowGraphNode, "Id", 5, m_nId, VT_I4)

	DISP_PROPERTY_EX_ID(CXTPFlowGraphNode, "ConnectionPoints", 6, OleGetConnectionPoints, SetNotSupported, VT_DISPATCH)

	DISP_PROPERTY_EX_ID(CXTPFlowGraphNode, "WindowHandle", 18, OleGetWindowHandle, OleSetWindowHandle, VT_HANDLE)

	DISP_PROPERTY_EX_ID(CXTPFlowGraphNode, "LocationX", 7, OleGetLeft, OleSetLeft, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPFlowGraphNode, "LocationY", 8, OleGetTop, OleSetTop, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPFlowGraphNode, "Width", 9, OleGetWidth, OleSetWidth, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPFlowGraphNode, "Height", 10, OleGetHeight, OleSetHeight, VT_I4)

	DISP_PROPERTY_EX_ID(CXTPFlowGraphNode, "WindowWidth", 19, OleGetWindowWidth, OleSetWindowWidth, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPFlowGraphNode, "WindowHeight", 20, OleGetWindowHeight, OleSetWindowHeight, VT_I4)

	DISP_PROPERTY_EX_ID(CXTPFlowGraphNode, "Page", 11, OleGetPage, SetNotSupported, VT_DISPATCH)

	DISP_FUNCTION_ID(CXTPFlowGraphNode, "Remove", 12, Remove, VT_EMPTY, VTS_NONE)

	DISP_PROPERTY_EX_ID(CXTPFlowGraphNode, "Selected", 13, OleGetSelected, OleSetSelected, VT_BOOL)

	DISP_PROPERTY_ID(CXTPFlowGraphNode, "ImageIndex", 14, m_nImageIndex, VT_I4)
	DISP_PROPERTY_ID(CXTPFlowGraphNode, "Locked", 15, m_bLocked, VT_BOOL)

	DISP_PROPERTY_EX_ID(CXTPFlowGraphNode, "Group", 16, OleGetGroup, SetNotSupported, VT_DISPATCH)

	DISP_FUNCTION_ID(CXTPFlowGraphNode, "HitTestConnectionPoint", 17, OleHitTestConnectionPoint, VT_DISPATCH, VTS_XPOS_PIXELS VTS_YPOS_PIXELS)

	DISP_FUNCTION_ID(CXTPFlowGraphNode, "AddNamedConnectionPoints", 21, OleAddNamedConnectionPoints, VT_EMPTY, VTS_BSTR VTS_I4)
	DISP_FUNCTION_ID(CXTPFlowGraphNode, "AddNamedConnectionPointsWithCaption", 22, OleAddNamedConnectionPointsWithCaption, VT_EMPTY, VTS_BSTR VTS_I4 VTS_BSTR)

END_DISPATCH_MAP()

// {03353C2D-B341-4329-9D52-97B0BF66B90E}
static const GUID IID_IFlowGraphNode =
{ 0x03353c2d, 0xb341, 0x4329, { 0x9d, 0x52, 0x97, 0xb0, 0xbf, 0x66, 0xb9, 0xe } };


BEGIN_INTERFACE_MAP(CXTPFlowGraphNode, CXTPFlowGraphElement)
	INTERFACE_PART(CXTPFlowGraphNode, IID_IFlowGraphNode, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPFlowGraphNode, IID_IFlowGraphNode)

void CXTPFlowGraphNode::OleSetCaption(LPCTSTR lpszCaption)
{
	SetCaption(lpszCaption);
}

BSTR CXTPFlowGraphNode::OleGetCaption()
{
	return m_strCaption.AllocSysString();
}

void CXTPFlowGraphNode::OleSetTooltip(LPCTSTR lpszTooltip)
{
	SetTooltip(lpszTooltip);
}

BSTR CXTPFlowGraphNode::OleGetTooltip()
{
	return m_strTooltip.AllocSysString();
}

void CXTPFlowGraphNode::OleSetColor(OLE_COLOR clr)
{
	SetColor(AxTranslateColor(clr));
}

OLE_COLOR CXTPFlowGraphNode::OleGetColor()
{
	return m_clrNode;
}

void CXTPFlowGraphNode::OleSetStyle(int nStyle)
{
	SetStyle(nStyle);
}

int CXTPFlowGraphNode::OleGetStyle()
{
	return GetStyle();
}

LPDISPATCH CXTPFlowGraphNode::OleGetConnectionPoints()
{
	return XTPGetDispatch(GetConnectionPoints());
}

int CXTPFlowGraphNode::OleGetLeft()
{
	return m_ptLocation.x;
}

void CXTPFlowGraphNode::OleSetLeft(int nLeft)
{
	SetLocation(CPoint(nLeft, m_ptLocation.y));
}

int CXTPFlowGraphNode::OleGetTop()
{
	return m_ptLocation.y;
}

void CXTPFlowGraphNode::OleSetTop(int nTop)
{
	SetLocation(CPoint(m_ptLocation.x, nTop));
}


int CXTPFlowGraphNode::OleGetWidth()
{
	return GetSize().cx;
}


int CXTPFlowGraphNode::OleGetHeight()
{
	return GetSize().cy;
}

LPDISPATCH CXTPFlowGraphNode::OleGetPage()
{
	return XTPGetDispatch(GetPage());
}

BOOL CXTPFlowGraphNode::OleGetSelected()
{
	return IsSelected();
}

void CXTPFlowGraphNode::OleSetSelected(BOOL bSelected)
{
	if (bSelected)
	{
		Select();
	}
	else
	{
		CXTPFlowGraphControl* pControl = GetControl();
		if (!pControl)
			return;

		CXTPFlowGraphSelectedElements* pSelectedElements = GetPage()->GetSelection();
		pSelectedElements->Remove(this);
	}

}

LPDISPATCH CXTPFlowGraphNode::OleGetGroup()
{
	return XTPGetDispatch(GetGroup());
}

LPDISPATCH CXTPFlowGraphNode::OleHitTestConnectionPoint(long x, long y)
{
	CXTPFlowGraphConnectionPoint* pConnectionPoint = HitTestConnectionPoint(CPoint(x, y));

	return XTPGetDispatch(pConnectionPoint);
}

void CXTPFlowGraphNode::OleSetWindowHandle(HWND hWnd)
{
	SetWindowHandle(hWnd);
}

HWND CXTPFlowGraphNode::OleGetWindowHandle()
{
	return GetWindowHandle();
}

int CXTPFlowGraphNode::OleGetWindowWidth()
{
	return m_szWindow.cx;
}

void CXTPFlowGraphNode::OleSetWindowWidth(int nWidth)
{
	m_szWindow.cx = nWidth;

	SetWindowSize(m_szWindow);
}

int CXTPFlowGraphNode::OleGetWindowHeight()
{
	return m_szWindow.cy;
}

void CXTPFlowGraphNode::OleSetWindowHeight(int nHeight)
{
	m_szWindow.cy = nHeight;

	SetWindowSize(m_szWindow);
}

void CXTPFlowGraphNode::OleSetWidth(int nWidth)
{
	if (nWidth < 0)
		return;

	CSize szSize;
	szSize = GetSize();

	szSize.cx = nWidth;

	SetSize(szSize);
}

void CXTPFlowGraphNode::OleSetHeight(int nHeight)
{
	if (nHeight < 0)
		return;

	CSize szSize;
	szSize = GetSize();

	szSize.cy = nHeight;

	SetSize(szSize);
}

void CXTPFlowGraphNode::OleAddNamedConnectionPoints(LPCTSTR lpszName, long type)
{
	// more info at
	// http://support.microsoft.com/kb/154039

	AddNamedConnectionPoints(lpszName, (XTPFlowGraphConnectionPointType)type);
}

void CXTPFlowGraphNode::OleAddNamedConnectionPointsWithCaption(LPCTSTR lpszName, long type, LPCTSTR lpszCaption)
{
	// more info at
	// http://support.microsoft.com/kb/154039

	AddNamedConnectionPoints(lpszName, (XTPFlowGraphConnectionPointType)type, lpszCaption);
}

#endif
