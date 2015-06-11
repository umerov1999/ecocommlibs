#include "StdAfx.h"

#include <GraphicLibrary/unzip/unzip.h>
#include <Common/XTPMarkupTheme.h>

#include <Common/XTPDrawHelpers.h>
#include <Common/XTPResourceImage.h>
#include <Common/XTPHookManager.h>
#include <Common/XTPMarkupRender.h>
#include <Common/XTPResourceManager.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPWinThemeWrapper.h>
#include <Common/XTPTheme.h>
#include <Common/XTPVC80Helpers.h>
#include <Common/ScrollBar/XTPScrollInfo.h>
#include <Common/Hook/XTPWinEventHook.h>

#include <TabManager/Includes.h>

#include <CommandBars/XTPCommandBarsDefines.h>
#include <CommandBars/XTPCommandBar.h>
#include <CommandBars/XTPCommandBars.h>
#include <CommandBars/XTPControls.h>
#include <CommandBars/XTPControl.h>
#include <CommandBars/XTPPaintManager.h>
#include <CommandBars/XTPTabClientWnd.h>

#include <CommandBars/Frame/XTPFrameHook.h>
#include <CommandBars/Frame/XTPFramePaintManager.h>
#ifdef _XTP_INCLUDE_RIBBON
#include <Ribbon/XTPRibbonMarkupFrameTheme.h>
#endif
#include <CommandBars/Frame/XTPFrameThemeModernUI.h>
#include <CommandBars/Frame/XTPFrameThemeOffice2013.h>

#include <Markup/XTPMarkupObject.h>
#include <Markup/XTPMarkupDrawingContext.h>
#include <Markup/XTPMarkupInputElement.h>           // CXTPMarkupUIElement
#include <Markup/XTPMarkupUIElement.h>              // CXTPMarkupFrameworkElement
#include <Markup/XTPMarkupFrameworkElement.h>   // CXTPMarkupDecorator
#include <Markup/XTPMarkupDecorator.h>              // CXTPMarkupBorder
#include <Markup/XTPMarkupBorder.h>                 // CXTPMarkupBorder
#include <Markup/XTPMarkupPanel.h>                  // CXTPMarkupCanvas
#include <Markup/XTPMarkupCanvas.h>                 // CXTPMarkupCanvas
#include <Markup/XTPMarkupShape.h>                  // CXTPMarkupShape


#define XTP_FRAMETHEMEOFFICE2013_MIN_CAPTION_SIZE 26


CXTPFrameThemeOffice2013::CXTPFrameThemeOffice2013(CXTPPaintManager *pPaintManager)
	: CXTPFrameThemeModernUI(pPaintManager)
{
	m_rcIconMargin.SetRect(10,0,10,0);
	m_bFlatFrame = FALSE;

	m_clrFrameCaptionActive   = RGB(255, 255, 255);
	m_clrFrameCaptionInactive = RGB(255, 255, 255);

	m_bCaptionCenterAligned = TRUE;
}


CXTPFrameThemeOffice2013::~CXTPFrameThemeOffice2013()
{
}


void CXTPFrameThemeOffice2013::RefreshMetrics()
{
	CXTPFrameThemeModernUI::RefreshMetrics();

	m_nFrameCaptionHeight = GetSystemMetrics(SM_CYCAPTION);
	if (m_nFrameCaptionHeight < XTP_FRAMETHEMEOFFICE2013_MIN_CAPTION_SIZE)
	{
		m_nFrameCaptionHeight = XTP_FRAMETHEMEOFFICE2013_MIN_CAPTION_SIZE;
	}
}

LPCTSTR CXTPFrameThemeOffice2013::GetStyleResourceName() const
{
	return _T("XTP_IDR_OFFICE2013_STYLE");
}

CSize CXTPFrameThemeOffice2013::GetCaptionButtonSize(UINT nId) const
{
	UNUSED_ALWAYS(nId); // All buttons have the same size
	return CSize(29, 25);
}

CRect CXTPFrameThemeOffice2013::GetCaptionButtonMargin() const
{
	return CRect(0, 3, 0, 0);
}

CRect CXTPFrameThemeOffice2013::CalcFrameBorder(const CXTPCommandBarsFrameHook *pFrameHook) const
{
	ASSERT(NULL != pFrameHook);

	CRect rc = CXTPFrameThemeModernUI::CalcFrameBorder(pFrameHook);

	if (pFrameHook->IsMDIMaximized())
	{
		// Ensure MDI child title is completely hidden under Tab Manager header, if exists.
		CWnd* pSite = pFrameHook->GetSite();
		if(NULL != pSite)
		{
			CWnd* pFrame = pSite->GetParent();
			if(NULL != pFrame)
			{
				CXTPTabClientWnd* pTabClient = DYNAMIC_DOWNCAST(CXTPTabClientWnd, pFrame);
				if (NULL != pTabClient 
					&& 0 < pTabClient->GetWorkspaceCount()
					&& 0 < pTabClient->GetItemCount())
				{
					CXTPClientRect rcTabClient(pTabClient);
					pTabClient->ClientToScreen(&rcTabClient);

					CXTPTabManagerItem* pItem = pTabClient->GetItem(0);
					if (NULL != pItem)
					{
						CXTPTabManager* pTabManager = pItem->GetTabManager();
						if (NULL != pTabManager && xtpTabPositionTop == pTabManager->GetPosition())
						{
							CXTPClientRect rcSite(pFrameHook->GetSite());
							pFrameHook->GetSite()->ClientToScreen(&rcSite);

							CRect rcControl = pTabManager->GetControlRect();
							rcControl.IsRectEmpty();

							if (rcSite.left == rcTabClient.left
								&& rcSite.right == rcTabClient.right
								&& rcSite.bottom == rcTabClient.bottom)
							{
								int nDiff = rcSite.top - rcTabClient.top;
								rc.top -= nDiff;
							}
						}
					}
				}
			}
		}
	}

	return rc;
}
