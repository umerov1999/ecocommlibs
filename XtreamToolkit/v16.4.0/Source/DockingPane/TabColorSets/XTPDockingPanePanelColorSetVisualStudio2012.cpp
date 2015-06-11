#include "StdAfx.h"

#include <Common/XTPColorManager.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPResourceImage.h>

#include <TabManager/Includes.h>

#include <DockingPane/XTPDockingPaneDefines.h>
#include <DockingPane/XTPDockingPaneBase.h>
#include <DockingPane/XTPDockingPaneBaseContainer.h>
#include <DockingPane/XTPDockingPaneTabbedContainer.h>
#include <DockingPane/XTPDockingPanePaintManager.h>
#include <DockingPane/TabColorSets/XTPDockingPanePanelColorSetVisualStudio2012.h>



CXTPDockingPanePanelColorSetVisualStudio2012::CXTPDockingPanePanelColorSetVisualStudio2012()
	: CXTPTabPaintManagerColorSet()
{
}

void CXTPDockingPanePanelColorSetVisualStudio2012::RefreshMetrics()
{
	CXTPTabPaintManagerColorSet ::RefreshMetrics();

	CXTPResourceImages *pImages = XTPResourceImages();

	if (pImages->IsValid())
	{
		static LPCTSTR lpszSectionName = _T("DockingPane.Panel");

		m_clrButtonNormal      = pImages->GetImageColor(lpszSectionName, _T("ButtonNormal"));
		m_clrButtonHighlighted = pImages->GetImageColor(lpszSectionName, _T("ButtonHighlighted"));
		m_clrButtonSelected    = pImages->GetImageColor(lpszSectionName, _T("ButtonSelected"));
		m_clrButtonPressed     = pImages->GetImageColor(lpszSectionName, _T("ButtonPressed"));

		m_clrTextNormal        = pImages->GetImageColor(lpszSectionName, _T("TextNormal"));
		m_clrTextInactive      = pImages->GetImageColor(lpszSectionName, _T("TextInactive"));
		m_clrTextHighlight     = pImages->GetImageColor(lpszSectionName, _T("TextHighlight"));
		m_clrTextSelected      = pImages->GetImageColor(lpszSectionName, _T("TextSelected"));
		m_clrTextDisabled      = pImages->GetImageColor(lpszSectionName, _T("TextDisabled"));

		m_clrAutoHideFace      = pImages->GetImageColor(lpszSectionName, _T("AutoHideFace"));
	}
}

COLORREF CXTPDockingPanePanelColorSetVisualStudio2012::GetItemColor(CXTPTabManagerItem *pItem)
{
	COLORREF clrItem;

		 if (pItem->IsSelected())    { clrItem = m_clrButtonSelected;    }
	else if (pItem->IsPressed())     { clrItem = m_clrButtonPressed;     }
	else if (pItem->IsHighlighted()) { clrItem = m_clrButtonHighlighted; }
	else                             { clrItem = m_clrButtonNormal;      }

	return clrItem;
}

void CXTPDockingPanePanelColorSetVisualStudio2012::FillNavigateButton(CDC *pDC, CXTPTabManagerNavigateButton *pButton, CRect &rc)
{
	UNREFERENCED_PARAMETER(pDC);
	UNREFERENCED_PARAMETER(pButton);
	UNREFERENCED_PARAMETER(rc);
}


/////////////////////////////////////////////////////////////////////////////
// CXTPDockingPanePanelColorSetVisualStudio2012Light

void CXTPDockingPanePanelColorSetVisualStudio2012Light::RefreshMetrics()
{
	CXTPDockingPanePanelColorSetVisualStudio2012::RefreshMetrics();

	m_clrButtonNormal      = RGB(204, 206, 219);
	m_clrButtonHighlighted = RGB( 28, 151, 234);
	m_clrButtonSelected    = RGB(  0, 122, 204);
	m_clrButtonPressed     = RGB(  0, 122, 204);

	m_clrTextNormal        = RGB(  0,   0,   0);
	m_clrTextInactive      = RGB(  0,   0,   0);
	m_clrTextHighlight     = RGB(  0, 122, 204);
	m_clrTextSelected      = RGB(  0, 122, 204);
	m_clrTextDisabled      = RGB(  0,   0,   0);

	m_clrAutoHideFace      = RGB(239, 239, 242);
}


/////////////////////////////////////////////////////////////////////////////
// CXTPDockingPanePanelColorSetVisualStudio2012Dark

void CXTPDockingPanePanelColorSetVisualStudio2012Dark::RefreshMetrics()
{
	CXTPDockingPanePanelColorSetVisualStudio2012::RefreshMetrics();

	m_clrButtonNormal      = RGB( 63,  63,  70);
	m_clrButtonHighlighted = RGB( 28, 151, 234);
	m_clrButtonSelected    = RGB(  0, 122, 204);
	m_clrButtonPressed     = RGB(  0, 122, 204);

	m_clrTextNormal        = RGB(255, 255, 255);
	m_clrTextInactive      = RGB(255, 255, 255);
	m_clrTextHighlight     = RGB(  0, 122, 204);
	m_clrTextSelected      = RGB(  0, 122, 204);
	m_clrTextDisabled      = RGB(255, 255, 255);

	m_clrAutoHideFace      = RGB( 45,  45,  48);
}
