#include "StdAfx.h"

#include <Common/XTPResourceImage.h>
#include <Common/XTPWinThemeWrapper.h>
#include <Common/XTPColorManager.h>
#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollBarPaintManager.h>
#include <Common/ScrollBar/Themes/XTPScrollBarThemeResource.h>

CXTPScrollBarThemeResource::CXTPScrollBarThemeResource()
	: CXTPScrollBarPaintManager()
{
}

void CXTPScrollBarThemeResource::RefreshMetrics()
{
	CXTPScrollBarPaintManager::RefreshMetrics();
}

CXTPResourceImage* CXTPScrollBarThemeResource::LoadImage(LPCTSTR lpszImageFile)
{
	CXTPResourceImages *pImages = XTPResourceImages();
	return pImages->LoadFile(lpszImageFile);
}

void CXTPScrollBarThemeResource::DrawScrollBar(CDC *pDC, CXTPScrollBase *pScrollBar)
{
	if(!XTPResourceImages()->IsValid())
		return;

	XTPScrollBarStyle barStyle = pScrollBar->GetScrollBarStyle();

	BOOL bLight = (barStyle != xtpScrollStyleOffice2007Dark);


#define GETPARTSTATE2(ht) \
	(!bEnabled ? 0 : nPressedHt == ht ? 3 : nHotHt == ht ? 2 : nHotHt > 0 || nPressedHt > 0 ? 1 : 0)

	XTP_SCROLLBAR_TRACKINFO *pSBTrack = pScrollBar->GetScrollBarTrackInfo();
	XTP_SCROLLBAR_POSINFO   *pSBInfo  = pScrollBar->GetScrollBarPosInfo();

	if(!pSBInfo->fSizebox)
	{
		BOOL nPressedHt = pSBTrack ? pSBInfo->ht : -1;
		BOOL nHotHt = pSBTrack ? -1 : pSBInfo->ht;

		int cWidth = (pSBInfo->pxRight - pSBInfo->pxLeft);

		if (cWidth <= 0)
		{
			return;
		}

		BOOL bEnabled = (pSBInfo->posMax - pSBInfo->posMin - pSBInfo->page + 1 > 0) && pScrollBar->IsScrollBarEnabled();

		int nBtnTrackSize = pSBInfo->pxThumbBottom - pSBInfo->pxThumbTop;
		int nBtnTrackPos  = pSBInfo->pxThumbTop    - pSBInfo->pxUpArrow;

		if (!bEnabled || pSBInfo->pxThumbBottom > pSBInfo->pxDownArrow)
		{
			nBtnTrackPos  = 0;
			nBtnTrackSize = 0;
		}



		CXTPResourceImage* pImageArrowGlyphs = NULL;
		if (!bLight)
		{
			pImageArrowGlyphs = LoadImage(_T("CONTROLGALLERYSCROLLARROWGLYPHSDARK"));
		}
		if (!pImageArrowGlyphs) pImageArrowGlyphs = LoadImage(_T("CONTROLGALLERYSCROLLARROWGLYPHS"));

		ASSERT(pImageArrowGlyphs);
		if (!pImageArrowGlyphs)
			return;
		CRect rcArrowGripperSrc(0, 0, 9, 9);

		if (pSBInfo->fVert)
		{
			CXTPResourceImage *pImageBackground = LoadImage(
				bLight ? _T("CONTROLGALLERYSCROLLVERTICALLIGHT") : _T("CONTROLGALLERYSCROLLVERTICALDARK"));
			ASSERT(pImageBackground);
			if (!pImageBackground)
				return;

			pImageBackground->DrawImage(pDC, pSBInfo->rc, pImageBackground->GetSource(0, 2), CRect(1, 0, 1, 0));


			CRect rcVScroll(pSBInfo->rc);
			rcVScroll.DeflateRect(1, 0);

			CRect rcArrowUp(rcVScroll.left, rcVScroll.top, rcVScroll.right, pSBInfo->pxUpArrow);
			CRect rcArrowDown(rcVScroll.left, pSBInfo->pxDownArrow, rcVScroll.right, rcVScroll.bottom);

			CXTPResourceImage* pImage =  LoadImage(
				bLight ? _T("CONTROLGALLERYSCROLLARROWSVERTICALLIGHT") : _T("CONTROLGALLERYSCROLLARROWSVERTICALDARK"));
			ASSERT(pImage);
			if (!pImage)
				return;

			int nState = GETPARTSTATE2(XTP_HTSCROLLUP);
			pImage->DrawImage(pDC, rcArrowUp, pImage->GetSource(nState, 4), CRect(3, 3, 3, 3), 0xFF00FF);

			CRect rcArrowUpGripper(CPoint((rcArrowUp.left + rcArrowUp.right - 9) / 2, (rcArrowUp.top + rcArrowUp.bottom - 9) / 2), CSize(9, 9));
			pImageArrowGlyphs->DrawImage(pDC, rcArrowUpGripper, OffsetSourceRect(rcArrowGripperSrc, !bEnabled ? ABS_UPDISABLED : nState != 0 ? ABS_UPHOT : ABS_UPNORMAL), CRect(0, 0, 0, 0), RGB(255, 0, 255));

			nState = GETPARTSTATE2(XTP_HTSCROLLDOWN);
			pImage->DrawImage(pDC, rcArrowDown, pImage->GetSource(nState, 4), CRect(3, 3, 3, 3), 0xFF00FF);

			CRect rcArrowDownGripper(CPoint((rcArrowDown.left + rcArrowDown.right - 9) / 2, (rcArrowDown.top + rcArrowDown.bottom - 9) / 2), CSize(9, 9));
			pImageArrowGlyphs->DrawImage(pDC, rcArrowDownGripper, OffsetSourceRect(rcArrowGripperSrc, !bEnabled ? ABS_DOWNDISABLED : nState != 0 ? ABS_DOWNHOT : ABS_DOWNNORMAL), CRect(0, 0, 0, 0), RGB(255, 0, 255));


			CRect rcTrack(rcVScroll.left, rcArrowUp.bottom, rcVScroll.right, rcArrowDown.top);

			if (!rcTrack.IsRectEmpty())
			{
				CRect rcLowerTrack(rcTrack.left - 1, rcTrack.top, rcTrack.right + 1, rcTrack.top + nBtnTrackPos);
				CRect rcBtnTrack(rcTrack.left, rcLowerTrack.bottom, rcTrack.right, rcLowerTrack.bottom + nBtnTrackSize);
				CRect rcUpperTrack(rcTrack.left - 1, rcBtnTrack.bottom, rcTrack.right + 1, rcTrack.bottom);

				if (!rcLowerTrack.IsRectEmpty() && (GETPARTSTATE2(XTP_HTSCROLLUPPAGE) == 3))
				{
					pImageBackground->DrawImage(pDC, rcLowerTrack,
						pImageBackground->GetSource(1, 2), CRect(1, 0, 1, 0));
				}

				if (!rcBtnTrack.IsRectEmpty())
				{
					nState = GETPARTSTATE2(XTP_HTSCROLLTHUMB);
					if (nState > 0) nState--;

					if (!bLight)
					{
						pImage =  LoadImage(_T("CONTROLGALLERYSCROLLTHUMBVERTICALDARK"));

						if (!pImage) pImage =  LoadImage(_T("CONTROLGALLERYSCROLLTHUMBVERTICAL"));
					}
					else
					{
						pImage =  LoadImage(_T("CONTROLGALLERYSCROLLTHUMBVERTICAL"));
					}
					ASSERT(pImage);
					if (!pImage)
						return;

					pImage->DrawImage(pDC, rcBtnTrack, pImage->GetSource(nState, 3), CRect(5, 5, 5, 5));


					if (rcBtnTrack.Height() > 10)
					{
						pImage =  LoadImage(_T("CONTROLGALLERYSCROLLTHUMBGRIPPERVERTICAL"));
						CRect rcGripper(CPoint(rcBtnTrack.CenterPoint().x - 4, rcBtnTrack.CenterPoint().y - 4), CSize(8, 8));

						pImage->DrawImage(pDC, rcGripper, pImage->GetSource(nState, 3), CRect(0, 0, 0, 0));
					}
				}

				if (!rcUpperTrack.IsRectEmpty() && (GETPARTSTATE2(XTP_HTSCROLLDOWNPAGE) == 3))
				{
					pImageBackground->DrawImage(pDC, rcUpperTrack,
						pImageBackground->GetSource(1, 2), CRect(1, 0, 1, 0));
				}
			}
		}
		else
		{
			CXTPResourceImage* pImageBackground = LoadImage(
				bLight ? _T("CONTROLGALLERYSCROLLHORIZONTALLIGHT") : _T("CONTROLGALLERYSCROLLHORIZONTALDARK"));
			if (!pImageBackground)
			{
				CXTPScrollBarPaintManager::DrawScrollBar(pDC, pScrollBar);
				return;
			}

			pImageBackground->DrawImage(pDC, pSBInfo->rc, pImageBackground->GetSource(0, 2), CRect(0, 1, 0, 1));

			CRect rcHScroll(pSBInfo->rc);
			rcHScroll.DeflateRect(0, 1);

			CRect rcArrowLeft(rcHScroll.left, rcHScroll.top, pSBInfo->pxUpArrow, rcHScroll.bottom);
			CRect rcArrowRight(pSBInfo->pxDownArrow, rcHScroll.top, rcHScroll.right, rcHScroll.bottom);

			CXTPResourceImage* pImage =  LoadImage(
				bLight ? _T("CONTROLGALLERYSCROLLARROWSHORIZONTALLIGHT") : _T("CONTROLGALLERYSCROLLARROWSHORIZONTALDARK"));
			ASSERT(pImage);
			if (!pImage)
				return;

			int nState = GETPARTSTATE2(XTP_HTSCROLLUP);
			pImage->DrawImage(pDC, rcArrowLeft, pImage->GetSource(nState, 4), CRect(3, 3, 3, 3), 0xFF00FF);

			CRect rcArrowLeftGripper(CPoint((rcArrowLeft.left + rcArrowLeft.right - 9) / 2, (rcArrowLeft.top + rcArrowLeft.bottom - 9) / 2), CSize(9, 9));
			pImageArrowGlyphs->DrawImage(pDC, rcArrowLeftGripper, OffsetSourceRect(rcArrowGripperSrc, !bEnabled ? ABS_LEFTDISABLED : nState != 0 ? ABS_LEFTHOT : ABS_LEFTNORMAL), CRect(0, 0, 0, 0), RGB(255, 0, 255));

			nState = GETPARTSTATE2(XTP_HTSCROLLDOWN);
			pImage->DrawImage(pDC, rcArrowRight, pImage->GetSource(nState, 4), CRect(3, 3, 3, 3), 0xFF00FF);

			CRect rcArrowRightGripper(CPoint((rcArrowRight.left + rcArrowRight.right - 9) / 2, (rcArrowRight.top + rcArrowRight.bottom - 9) / 2), CSize(9, 9));
			pImageArrowGlyphs->DrawImage(pDC, rcArrowRightGripper, OffsetSourceRect(rcArrowGripperSrc, !bEnabled ? ABS_RIGHTDISABLED : nState != 0 ? ABS_RIGHTHOT : ABS_RIGHTNORMAL), CRect(0, 0, 0, 0), RGB(255, 0, 255));



			CRect rcTrack(rcArrowLeft.right, rcHScroll.top, rcArrowRight.left, rcHScroll.bottom);

			if (!rcTrack.IsRectEmpty())
			{
				CRect rcLowerTrack(rcTrack.left, rcTrack.top - 1, rcTrack.left + nBtnTrackPos, rcTrack.bottom + 1);
				CRect rcBtnTrack(rcLowerTrack.right, rcTrack.top, rcLowerTrack.right + nBtnTrackSize, rcTrack.bottom);
				CRect rcUpperTrack(rcBtnTrack.right, rcTrack.top - 1, rcTrack.right, rcTrack.bottom + 1);

				if (!rcLowerTrack.IsRectEmpty() && (GETPARTSTATE2(XTP_HTSCROLLUPPAGE) == 3))
				{
					pImageBackground->DrawImage(pDC, rcLowerTrack,
						pImageBackground->GetSource(1, 2), CRect(0, 1, 0, 1));
				}

				if (!rcBtnTrack.IsRectEmpty())
				{
					nState = GETPARTSTATE2(XTP_HTSCROLLTHUMB);
					if (nState > 0) nState--;

					if (!bLight)
					{
						pImage =  LoadImage(_T("CONTROLGALLERYSCROLLTHUMBHORIZONTALDARK"));

						if (!pImage) pImage =  LoadImage(_T("CONTROLGALLERYSCROLLTHUMBHORIZONTAL"));
					}
					else
					{
						pImage =  LoadImage(_T("CONTROLGALLERYSCROLLTHUMBHORIZONTAL"));
					}
					ASSERT(pImage);
					if (!pImage)
						return;

					pImage->DrawImage(pDC, rcBtnTrack, pImage->GetSource(nState, 3), CRect(5, 5, 5, 5));


					if (rcBtnTrack.Width() > 10)
					{
						pImage =  LoadImage(_T("CONTROLGALLERYSCROLLTHUMBGRIPPERHORIZONTAL"));
						CRect rcGripper(CPoint(rcBtnTrack.CenterPoint().x - 3, rcBtnTrack.CenterPoint().y - 4), CSize(8, 8));

						pImage->DrawImage(pDC, rcGripper, pImage->GetSource(nState, 3), CRect(0, 0, 0, 0));
					}
				}

				if (!rcUpperTrack.IsRectEmpty() && (GETPARTSTATE2(XTP_HTSCROLLDOWNPAGE) == 3))
				{
					pImageBackground->DrawImage(pDC, rcUpperTrack,
						pImageBackground->GetSource(1, 2), CRect(0, 1, 0, 1));
				}
			}
		}
	}
	else
	{
		CXTPResourceImage *pImageBackground = LoadImage(bLight
			? _T("CONTROLGALLERYSCROLLSIZEBOXLIGHT")
			: _T("CONTROLGALLERYSCROLLSIZEBOXDARK"));
		ASSERT(NULL != pImageBackground);
		if(NULL != pImageBackground)
		{
			pImageBackground->DrawImage(pDC, pSBInfo->rc, pImageBackground->GetSource());

			CXTPResourceImage *pImageGripper = LoadImage(_T("STATUSBARGRIPPER"));
			ASSERT(NULL != pImageGripper);
			if(NULL != pImageGripper)
			{
				CRect rcSrc(0, 0, pImageGripper->GetWidth(), pImageGripper->GetHeight());
				int cxDst = pSBInfo->rc.right - pSBInfo->rc.left;
				int cyDst = pSBInfo->rc.bottom - pSBInfo->rc.top;
				CRect rcDst(cxDst - rcSrc.Width(), cyDst - rcSrc.Height(), cxDst, cyDst);
				pImageGripper->DrawImage(pDC, rcDst, rcSrc, CRect(0, 0, 0, 0), RGB(0xFF, 0, 0xFF));
			}
		}
	}
}
