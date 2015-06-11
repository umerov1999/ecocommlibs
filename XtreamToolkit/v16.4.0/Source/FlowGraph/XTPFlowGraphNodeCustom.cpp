// XTPFlowGraphNodeCustom.cpp : implementation of the CXTPFlowGraphNodeCustom class.
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

#include "Common/XTPPropExchange.h"
#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollBarContainer.h>

#include "XTPFlowGraphElement.h"
#include "XTPFlowGraphNode.h"
#include "XTPFlowGraphNodeCustom.h"
#include "XTPFlowGraphControl.h"
#include "XTPFlowGraphPaintManager.h"
#include "XTPFlowGraphImage.h"
#include "XTPFlowGraphPage.h"

#include "Markup/XTPMarkupObject.h"
#include "Markup/XTPMarkupInputElement.h"
#include "Markup/XTPMarkupUIElement.h"
#include "Markup/XTPMarkupFrameworkElement.h"
#include "Markup/XTPMarkupShape.h"
#include "Markup/Path/XTPMarkupPath.h"
#include "Markup/Path/XTPMarkupPathData.h"
#include "Markup/XTPMarkupTextBlock.h"
#include "Markup/XTPMarkupDrawingContext.h"
#include "Markup/XTPMarkupImage.h"
#include "Markup/XTPMarkupPanel.h"
#include "Markup/XTPMarkupCanvas.h"
#include "Markup/XTPMarkupDecorator.h"
#include "Markup/XTPMarkupBorder.h"

#include "XTPFlowGraphConnectionPoint.h"
#include "XTPFlowGraphConnectionPoints.h"

#include "XTPFlowGraphTools.h"
#include "XTPFlowGraphDrawContext.h"

IMPLEMENT_SERIAL(CXTPFlowGraphNodeCustom, CXTPFlowGraphNode, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPFlowGraphNodeCustom::CXTPFlowGraphNodeCustom()
	: CXTPFlowGraphNode()
{
	m_CaptionPosition = xtpFlowGraphCustomNodeCaptionCenter;

	m_clrSecondColor = (COLORREF)-1;
	m_FillType = xtpFlowGraphCustomNodeFillSolid;
}

CXTPFlowGraphNodeCustom::~CXTPFlowGraphNodeCustom()
{

}

void CXTPFlowGraphNodeCustom::DoPropExchange(CXTPPropExchange* pPX)
{
	DoCommonPropExchange(pPX);

	PX_Int(pPX, _T("CaptionPosition"), (int &)m_CaptionPosition, (int)xtpFlowGraphCustomNodeCaptionCenter);
	PX_String(pPX, _T("Markup"), m_strMarkup, _T(""));

	PX_DWord(pPX, _T("ColorTwo"), (DWORD)m_clrSecondColor, (COLORREF)-1);
	PX_Int(pPX, _T("FillType"), (int &)m_FillType, (int)xtpFlowGraphCustomNodeFillSolid);

	if (pPX->IsLoading())
	{
		SetMarkupText(m_strMarkup);

		SetSize(m_szUserSize);

		UpdateMarkupCaption();

		SetColor(m_clrNode, m_clrSecondColor, m_FillType, m_bAlpha);
	}
}

CXTPMarkupPathData *CXTPFlowGraphNodeCustom::CreateStretchedPathData(const CXTPMarkupPath* pNodePath) const
{
	CXTPMarkupPathData *pPathData = NULL;

	ASSERT(pNodePath->GetData());

	if (pNodePath->GetData())
	{
		pPathData = pNodePath->GetData()->Stretch(pNodePath->GetRenderSize());
	}

	return pPathData;
}

CRect CXTPFlowGraphNodeCustom::GetPointRect(CPoint point) const
{
	int nHalfEllipseSize = 12/2;

	if (GetControl() && GetControl()->GetPaintManager())
		nHalfEllipseSize = GetControl()->GetPaintManager()->m_nEllipseSize/2;

	return CRect( point.x - nHalfEllipseSize, point.y - nHalfEllipseSize, point.x + nHalfEllipseSize, point.y+nHalfEllipseSize );
}

void CXTPFlowGraphNodeCustom::UpdateColorRecursive(CXTPMarkupObject * /*pRootElement*/, COLORREF /*clrColor*/)
{
	SetColor(m_clrNode, m_clrSecondColor, m_FillType, m_bAlpha);
}

void CXTPFlowGraphNodeCustom::UpdateConnectionPoints(CXTPMarkupObject *pRootElement)
{
	ASSERT(pRootElement);

	if (pRootElement)
	{
		bool bFirstRun = (GetConnectionPoints()->GetCount() == 0);

		const CXTPMarkupPath* pNodePath = MARKUP_DYNAMICCAST(CXTPMarkupPath, pRootElement->FindName(XAML_MARKUP_TAG_NODEPATH) );
		const CXTPMarkupCanvas* pNodeCanvas = MARKUP_DYNAMICCAST(CXTPMarkupCanvas, m_pMarkup->FindName(XAML_MARKUP_TAG_NODECANVAS) );

		CXTPMarkupString *pConnectionPointsIn = NULL;

		if (pNodeCanvas)
		{
			pConnectionPointsIn = MARKUP_STATICCAST(CXTPMarkupString, pNodeCanvas->GetValue(GetControl()->GetCustomMarkupProperty(xtpFlowGraphPropertyConnectionPointIn)));

			// probably not the best place to get this property (Resizable) according to the logic

			CXTPMarkupBool *pResizableProperty = MARKUP_STATICCAST(CXTPMarkupBool, pNodeCanvas->GetValue(GetControl()->GetCustomMarkupProperty(xtpFlowGraphPropertyResizable)));

			if (pResizableProperty && ((BOOL)*pResizableProperty) == FALSE)
				m_bResizable = FALSE;
		}

		if (pNodePath && !pConnectionPointsIn)
		{
			ASSERT(pNodePath->GetStretch() != xtpMarkupStretchNone); // Path must be with Stretch property to support correct resizing of the node

			const CRect pathRect = GetMarkupVisualElementRect(pNodePath);

			const CXTPMarkupPathData* pGeometryStretch = CreateStretchedPathData(pNodePath); // need to delete afterwards

			ASSERT(pGeometryStretch);

			if (pGeometryStretch)
			{
				for (int i=0,c=pGeometryStretch->GetCount(); i<c; i++)
				{
					MARKUP_POINTF pointF = pGeometryStretch->GetPoints()[i];
					CPoint point((int)pointF.x, (int)pointF.y);

					CRect pointRect = GetPointRect(point);
					pointRect.OffsetRect(pathRect.TopLeft());

					CXTPFlowGraphConnectionPoint *pPoint = NULL;

					if (bFirstRun)
					{
						pPoint = new CXTPFlowGraphConnectionPoint();

						pPoint->SetType(xtpFlowGraphPointBoth);

						m_pConnectionPoints->AddConnectionPoint(pPoint);
					}
					else
					{
						ASSERT(i < GetConnectionPoints()->GetCount());

						if (i < GetConnectionPoints()->GetCount())
							pPoint = GetConnectionPoints()->GetAt(i);
					}

					ASSERT(pPoint);

					if (pPoint)
					{
						pPoint->SetRect(pointRect);
					}
				}

				delete pGeometryStretch;
			}
		}
		else
		{
			// examples of nodes with 0 connection points: ellipse and image

			ASSERT(pConnectionPointsIn); // this property should be defined for nodes without connection points defined by a path

			if (pConnectionPointsIn)
			{
				CString placePointsAs = *pConnectionPointsIn;

				if (placePointsAs == XAML_MARKUP_PROPERTY_CONNECTIONPOINTS_AS_RHOMBUS)
				{
					CRect pointOfRhombus[4];
					pointOfRhombus[0] = GetPointRect(CPoint(0,pNodeCanvas->GetHeight()/2));
					pointOfRhombus[1] = GetPointRect(CPoint(pNodeCanvas->GetWidth(), pNodeCanvas->GetHeight()/2 ));
					pointOfRhombus[2] = GetPointRect(CPoint(pNodeCanvas->GetWidth()/2, 0));
					pointOfRhombus[3] = GetPointRect(CPoint(pNodeCanvas->GetWidth()/2, pNodeCanvas->GetHeight()));

					for (int i=0; i<4; i++)
					{
						CXTPFlowGraphConnectionPoint *pPoint = NULL;

						if (bFirstRun)
						{
							pPoint = new CXTPFlowGraphConnectionPoint();

							pPoint->SetType(xtpFlowGraphPointBoth);

							m_pConnectionPoints->AddConnectionPoint(pPoint);
						}
						else
						{
							ASSERT(i < GetConnectionPoints()->GetCount());

							if (i < GetConnectionPoints()->GetCount())
								pPoint = GetConnectionPoints()->GetAt(i);
						}

						ASSERT(pPoint);

						if (pPoint)
						{
							pPoint->SetRect(pointOfRhombus[i]);
						}
					}
				}
				else if (placePointsAs == XAML_MARKUP_PROPERTY_CONNECTIONPOINTS_IN_CENTER)
				{
					// 1 point

					CRect pointRect = GetPointRect(CPoint(pNodeCanvas->GetWidth()/2, pNodeCanvas->GetHeight()/2));

					CXTPFlowGraphConnectionPoint *pPoint = NULL;

					if (bFirstRun)
					{
						pPoint = new CXTPFlowGraphConnectionPoint();

						pPoint->SetType(xtpFlowGraphPointBoth);

						m_pConnectionPoints->AddConnectionPoint(pPoint);
					}
					else
					{
						ASSERT(GetConnectionPoints()->GetCount() == 1);

						if (GetConnectionPoints()->GetCount() == 1)
							pPoint = GetConnectionPoints()->GetAt(0);
					}

					ASSERT(pPoint);

					if (pPoint)
					{
						pPoint->SetRect(pointRect);
					}
				}
			}
		}
	}
}

void CXTPFlowGraphNodeCustom::SetSize(CSize sz)
{
	if (!IsResizable())
		return;

	ASSERT(m_pMarkup);

	if (m_pMarkup)
	{
		CXTPMarkupCanvas* pNodeCanvas = MARKUP_DYNAMICCAST(CXTPMarkupCanvas, m_pMarkup->FindName(XAML_MARKUP_TAG_NODECANVAS) );
		CXTPMarkupShape* pNodeShape = MARKUP_DYNAMICCAST(CXTPMarkupShape, m_pMarkup->FindName(XAML_MARKUP_TAG_NODEPATH) );
		CXTPMarkupImage* pNodeImage = MARKUP_DYNAMICCAST(CXTPMarkupImage, m_pMarkup->FindName(XAML_MARKUP_TAG_NODEIMAGE) );

		CXTPMarkupBorder* pNodeBorder = MARKUP_DYNAMICCAST(CXTPMarkupBorder, m_pMarkup->FindName(XAML_MARKUP_TAG_NODEBORDER) );

		//ASSERT(pNodeCanvas && pNodeShape);

		if (pNodeShape)
		{
			pNodeShape->SetWidth(sz.cx);
			pNodeShape->SetHeight(sz.cy);
		}

		if (pNodeCanvas)
		{
			pNodeCanvas->SetWidth(sz.cx);
			pNodeCanvas->SetHeight(sz.cy);
		}

		if (pNodeImage)
		{
			pNodeImage->SetWidth(sz.cx);
			pNodeImage->SetHeight(sz.cy);
		}

		if (pNodeBorder)
		{
			pNodeBorder->SetWidth(sz.cx);
			pNodeBorder->SetHeight(sz.cy);
		}
	}

	SetSizeCore(sz);
}

CRect CXTPFlowGraphNodeCustom::GetBoundingRectangle(BOOL bIncludeConnectionPointsEllipses /*= TRUE*/) const
{
	// CXTPFlowGraphNodeCustom node can have connection points on top and bottom of the rectangle unlike CXTPFlowGraphNode
	// so we need to inflate rect once again

	CRect rc = CXTPFlowGraphNode::GetBoundingRectangle(bIncludeConnectionPointsEllipses);

	if (bIncludeConnectionPointsEllipses)
	{
		if (GetControl() && GetControl()->GetPaintManager())
		{
			rc.InflateRect(0, GetControl()->GetPaintManager()->m_nEllipseSize / 2);
		}
		else
		{
			ASSERT(0);
		}
	}

	return rc;
}

void CXTPFlowGraphNodeCustom::SetCaptionPosition(XTPFlowGraphCustomNodeCaptionPosition pos)
{
	m_CaptionPosition = pos;

	UpdateMarkupCaption();
}

XTPFlowGraphCustomNodeCaptionPosition CXTPFlowGraphNodeCustom::GetCaptionPosition() const
{
	return m_CaptionPosition;
}

CXTPMarkupTextBlock* CXTPFlowGraphNodeCustom::UpdateMarkupCaption()
{
	CXTPMarkupTextBlock* pNodeCaption = CXTPFlowGraphNode::UpdateMarkupCaption();

	if (m_pMarkup)
	{
		CXTPMarkupCanvas* pNodeCanvas = MARKUP_DYNAMICCAST(CXTPMarkupCanvas, m_pMarkup->FindName(XAML_MARKUP_TAG_NODECANVAS) );

		if (pNodeCaption && pNodeCanvas && GetControl())
		{
			// seems to be a bug(?) of markup, need to manually call Arrange for correct caption positioning
			pNodeCanvas->Arrange(CRect(CPoint(0,0), pNodeCanvas->GetRenderSize() ) );

			CSize actualSize = pNodeCaption->GetActualSize();

			if (actualSize.cx)
			{
				int iCanvasLeft = 0;
				int iCanvasTop = 0;

				switch (m_CaptionPosition)
				{
					case xtpFlowGraphCustomNodeCaptionCenter:
					{
						iCanvasLeft = pNodeCanvas->GetWidth() / 2 - actualSize.cx/2;
						iCanvasTop = pNodeCanvas->GetHeight() / 2 - actualSize.cy/2;

						break;
					}

					case xtpFlowGraphCustomNodeCaptionBelow:
					{
						iCanvasLeft = pNodeCanvas->GetWidth() / 2 - actualSize.cx/2;
						iCanvasTop = pNodeCanvas->GetHeight() + actualSize.cy/2;

						break;
					}
				}

				// TODO: potential memory leaks?
				pNodeCaption->SetValue(CXTPMarkupCanvas::m_pTopProperty, new CXTPMarkupInt(iCanvasTop));
				pNodeCaption->SetValue(CXTPMarkupCanvas::m_pLeftProperty, new CXTPMarkupInt(iCanvasLeft));
			}
		}
	}

	return pNodeCaption;
}

void CXTPFlowGraphNodeCustom::UpdateCaptionColor()
{
	// custom nodes do not change caption color when selected
}

void CXTPFlowGraphNodeCustom::SetColor(COLORREF clr, BYTE bAlpha /*= 255*/)
{
	m_bAlpha = bAlpha;

	SetColor(clr, clr, xtpFlowGraphCustomNodeFillSolid, m_bAlpha);
}

COLORREF CXTPFlowGraphNodeCustom::GetSecondColor() const
{
	return m_clrSecondColor;
}

void CXTPFlowGraphNodeCustom::SetSecondColor(COLORREF clrSecondColor)
{
	m_clrSecondColor = clrSecondColor;

	SetColor(m_clrNode, m_clrSecondColor, m_FillType, m_bAlpha);
}

XTPFlowGraphCustomNodeFillType CXTPFlowGraphNodeCustom::GetFillType() const
{
	return m_FillType;
}

void CXTPFlowGraphNodeCustom::SetFillType(XTPFlowGraphCustomNodeFillType newType)
{
	m_FillType = newType;

	SetColor(m_clrNode, m_clrSecondColor, m_FillType, m_bAlpha);
}

class dPoint // a helper class
{
public:

	double m_x;
	double m_y;

	dPoint() : m_x(0), m_y() {}
	dPoint(double x, double y) : m_x(x), m_y(y) {}
};

class dColorStop // one more helper class
{
public:

	COLORREF m_clr;
	double m_offset;

	dColorStop() : m_clr((COLORREF)-1), m_offset() {}
	dColorStop(COLORREF clr, double offset) : m_clr(clr), m_offset(offset) {}

	bool IsEmpty() const { return (m_clr == -1); }
};

CXTPMarkupLinearGradientBrush *CreateLinearGradient(dColorStop clr1, dColorStop clr2, dPoint p1, dPoint p2, dColorStop clr3 = dColorStop())
{
	CXTPMarkupGradientStops *pStops = new CXTPMarkupGradientStops();

	pStops->Add(new CXTPMarkupGradientStop(clr1.m_clr, clr1.m_offset) );
	pStops->Add(new CXTPMarkupGradientStop(clr2.m_clr, clr2.m_offset) );

	if (!clr3.IsEmpty())
		pStops->Add(new CXTPMarkupGradientStop(clr3.m_clr, clr3.m_offset) );

	CXTPMarkupLinearGradientBrush *pGradient = new CXTPMarkupLinearGradientBrush(pStops);

	pGradient->SetStartPoint(p1.m_x, p1.m_y);
	pGradient->SetEndPoint(p2.m_x, p2.m_y);

	return pGradient;
}

void CXTPFlowGraphNodeCustom::SetColor(COLORREF clrOne, COLORREF clrTwo, XTPFlowGraphCustomNodeFillType fType, BYTE bAlpha /*= 255*/)
{
	m_clrNode = clrOne;
	m_clrSecondColor = clrTwo;
	m_FillType = fType;

	// the method will actually work only if xaml shape or path is defined

	ASSERT(m_pMarkup);

	if (m_pMarkup)
	{
		CXTPMarkupShape* pNodeShape = MARKUP_DYNAMICCAST(CXTPMarkupShape, m_pMarkup->FindName(XAML_MARKUP_TAG_NODEPATH) );
		CXTPMarkupBorder* pNodeBorder = MARKUP_DYNAMICCAST(CXTPMarkupBorder, m_pMarkup->FindName(XAML_MARKUP_TAG_NODEBORDER) );

		if (pNodeBorder)
		{
			// node markup can decline setting color by using 'CanSetColor' property of nodeBorder
			// check is this node can have a color
			CXTPMarkupDependencyProperty *pPropertyCanSetColor = GetControl()->GetCustomMarkupProperty(xtpFlowGraphPropertyCanSetColor);

			if (pPropertyCanSetColor)
			{
				CXTPMarkupBool *pCanSetColor = MARKUP_STATICCAST(CXTPMarkupBool, pNodeBorder->GetValue(pPropertyCanSetColor));

				if (pCanSetColor && (BOOL)(*pCanSetColor) == FALSE)
				{
					return; // e.g. used XAML cards
				}
			}
		}

		if (pNodeShape || pNodeBorder)
		{
			CXTPMarkupBrush *pBrush = NULL;

			switch (m_FillType)
			{
				case xtpFlowGraphCustomNodeFillSolid:
				{
					pBrush = new CXTPMarkupSolidColorBrush(bAlpha, m_clrNode);

					break;
				}

				case xtpFlowGraphCustomNodeFillGradientVertical:
				{
					pBrush = CreateLinearGradient( dColorStop(m_clrNode, 0), dColorStop(m_clrSecondColor, 1), dPoint(0.5, 0), dPoint(0.5, 1));

					break;
				}

				case xtpFlowGraphCustomNodeFillGradientHorizontal:
				{
					pBrush = CreateLinearGradient(dColorStop(m_clrNode, 0), dColorStop(m_clrSecondColor, 1), dPoint(0, 0.5), dPoint(1,0.5));

					break;
				}

				case xtpFlowGraphCustomNodeFillGradientDiagonalLeft:
				{
					pBrush = CreateLinearGradient(dColorStop(m_clrNode, 0), dColorStop(m_clrSecondColor, 1), dPoint(0, 0), dPoint(1,1));

					break;
				}

				case xtpFlowGraphCustomNodeFillGradientDiagonalRight:
				{
					pBrush = CreateLinearGradient(dColorStop(m_clrNode, 0), dColorStop(m_clrSecondColor, 1), dPoint(0, 1), dPoint(1,0));

					break;
				}

				case xtpFlowGraphCustomNodeFillGradientCenterHorizontal:
				{
					pBrush = CreateLinearGradient(dColorStop(m_clrNode, 0), dColorStop(m_clrSecondColor, 0.5), dPoint(0, 0.5), dPoint(1, 0.5), dColorStop(m_clrNode, 1) );

					break;
				}

				case xtpFlowGraphCustomNodeFillGradientCenterVertical:
				{
					pBrush = CreateLinearGradient(dColorStop(m_clrNode, 0), dColorStop(m_clrSecondColor, 0.5), dPoint(0.5, 0), dPoint(0.5, 1), dColorStop(m_clrNode, 1) );

					break;
				}
			}

			ASSERT(pBrush);

			if (pBrush)
			{
				if (pNodeBorder)
					pNodeBorder->SetBackground(pBrush); // border has priority
				else if (pNodeShape)
					pNodeShape->SetFill(pBrush);
			}
		}
	}

	OnGraphChanged();
}

void CXTPFlowGraphNodeCustom::CalcMinSize()
{
	int iMinWidth = -1;
	int iMinHeight = -1;

	if (m_CaptionPosition == xtpFlowGraphCustomNodeCaptionCenter)
	{
		if (m_pMarkup)
		{
			CXTPMarkupTextBlock* pNodeCaption = MARKUP_DYNAMICCAST(CXTPMarkupTextBlock, m_pMarkup->FindName(XAML_MARKUP_TAG_NODECAPTION) );;

			if (pNodeCaption)
			{
				iMinWidth = pNodeCaption->GetActualSize().cx + 30;

				iMinHeight = pNodeCaption->GetActualSize().cy + 20;
			}
		}
	}
	else
	{
		iMinWidth = 50;
		iMinHeight = 50;

		CXTPMarkupImage* pNodeImage = MARKUP_DYNAMICCAST(CXTPMarkupImage, m_pMarkup->FindName(XAML_MARKUP_TAG_NODEIMAGE) );

		if (pNodeImage)
		{
			CSize sz = pNodeImage->GetOriginalSize();

			if (sz != CSize(0,0))
			{
				iMinWidth = sz.cx;
				iMinHeight = sz.cy;
			}
		}
	}

	m_szMinSize = CSize(iMinWidth, iMinHeight);
}

CXTPFlowGraphConnectionPoint *CXTPFlowGraphNodeCustom::HitTestConnectionPoint(CPoint point)
{
	point = m_pPage->ScreenToPage(point);
	point.Offset(-m_ptLocation);

	for (int i = 0; i < m_pConnectionPoints->GetCount(); i++)
	{
		CXTPFlowGraphConnectionPoint* pPoint = m_pConnectionPoints->GetAt(i);

		if (pPoint->GetRect().PtInRect(point))
			return pPoint;
	}

	return NULL;
}

#ifdef _XTP_ACTIVEX


BEGIN_DISPATCH_MAP(CXTPFlowGraphNodeCustom, CXTPFlowGraphNode)

	DISP_PROPERTY_EX_ID(CXTPFlowGraphNodeCustom, "CaptionPosition", 30, OleGetCaptionPosition, OleSetCaptionPosition, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPFlowGraphNodeCustom, "MarkupText", 31, OleGetMarkupText, OleSetMarkupText, VT_BSTR)

	DISP_PROPERTY_EX_ID(CXTPFlowGraphNodeCustom, "SecondColor", 32, OleGetSecondColor, OleSetSecondColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPFlowGraphNodeCustom, "FillType", 33, OleGetFillType, OleSetFillType, VT_I4)

END_DISPATCH_MAP()

// {08853C2D-B341-4329-9D52-97B0BF66B90E}
static const GUID IID_IFlowGraphNodeCustom =
{ 0x08853C2D, 0xb341, 0x4329, { 0x9d, 0x52, 0x97, 0xb0, 0xbf, 0x66, 0xb9, 0xe } };

BEGIN_INTERFACE_MAP(CXTPFlowGraphNodeCustom, CXTPFlowGraphNode)
	INTERFACE_PART(CXTPFlowGraphNodeCustom, IID_IFlowGraphNodeCustom, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPFlowGraphNodeCustom, IID_IFlowGraphNodeCustom)

void CXTPFlowGraphNodeCustom::OleSetCaptionPosition(int nStyle)
{
	SetCaptionPosition( (XTPFlowGraphCustomNodeCaptionPosition)nStyle);
}

int CXTPFlowGraphNodeCustom::OleGetCaptionPosition()
{
	return GetCaptionPosition();
}

void CXTPFlowGraphNodeCustom::OleSetMarkupText(LPCTSTR lpszMarkupText)
{
	SetMarkupText(lpszMarkupText);
}

BSTR CXTPFlowGraphNodeCustom::OleGetMarkupText()
{
	return m_strMarkup.AllocSysString();
}

void CXTPFlowGraphNodeCustom::OleSetSecondColor(OLE_COLOR clr)
{
	SetSecondColor(AxTranslateColor(clr));
}

OLE_COLOR CXTPFlowGraphNodeCustom::OleGetSecondColor()
{
	return GetSecondColor();
}

void CXTPFlowGraphNodeCustom::OleSetFillType(int nType)
{
	SetFillType( (XTPFlowGraphCustomNodeFillType)nType);
}

int CXTPFlowGraphNodeCustom::OleGetFillType()
{
	return (int)GetFillType();
}

#endif // _XTP_ACTIVEX
