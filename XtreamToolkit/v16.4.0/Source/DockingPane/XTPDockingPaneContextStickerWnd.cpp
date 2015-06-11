// XTPDockingPaneContext.cpp : implementation of the CXTPDockingPaneContext class.
//
// This file is a part of the XTREME DOCKINGPANE MFC class library.
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

#include <Common/XTPColorManager.h>
#include <Common/XTPDrawHelpers.h>
#include <Common/XTPResourceManager.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPImageManager.h>

#include <TabManager/Includes.h>

#include <DockingPane/Resource.h>
#include <DockingPane/XTPDockingPaneDefines.h>
#include <DockingPane/XTPDockingPaneBase.h>
#include <DockingPane/XTPDockingPaneBaseContainer.h>
#include <DockingPane/XTPDockingPaneContext.h>
#include <DockingPane/XTPDockingPane.h>
#include <DockingPane/XTPDockingPaneManager.h>
#include <DockingPane/XTPDockingPaneTabbedContainer.h>
#include <DockingPane/XTPDockingPaneSplitterContainer.h>
#include <DockingPane/XTPDockingPaneMiniWnd.h>
#include <DockingPane/XTPDockingPaneSidePanel.h>
#include <DockingPane/XTPDockingPanePaintManager.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneContextStickerWnd

CXTPDockingPaneContextStickerWnd::CXTPDockingPaneContextStickerWnd(CXTPDockingPaneContext* pContext)
	: m_pContext(pContext)
{
	m_typeSticker = m_selectedSticker = xtpPaneStickerNone;
}

CXTPDockingPaneContextStickerWnd::~CXTPDockingPaneContextStickerWnd()
{
}


BEGIN_MESSAGE_MAP(CXTPDockingPaneContextStickerWnd, CWnd)
	//{{AFX_MSG_MAP(CXTPDockingPaneContextStickerWnd)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneContextStickerWnd message handlers

BOOL CXTPDockingPaneContextStickerWnd::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

struct CXTPDockingPaneContextStickerWnd::SPRITEINFO
{
	SPRITEINFO(int x, int y, int left, int top, int cx, int cy)
	{
		ptDest = CPoint(x, y);
		rcSrc.SetRect(left, top, left + cx, top + cy);
	}
	CPoint ptDest;
	CRect rcSrc;
};

#define SPRITE_STICKER_TOP              0
#define SPRITE_STICKER_LEFT             1
#define SPRITE_STICKER_BOTTOM           2
#define SPRITE_STICKER_RIGHT            3
#define SPRITE_STICKER_TOP_SELECTED     4
#define SPRITE_STICKER_LEFT_SELECTED    5
#define SPRITE_STICKER_BOTTOM_SELECTED  6
#define SPRITE_STICKER_RIGHT_SELECTED   7
#define SPRITE_STICKER_CENTER           8
#define SPRITE_STICKER_CENTER_SELECTED  9
#define SPRITE_STICKER_CLIENT           10

static CXTPDockingPaneContextStickerWnd::SPRITEINFO arrSpritesStyckerVisualStudio2005Beta[] =
{
/*  0 */    CXTPDockingPaneContextStickerWnd::SPRITEINFO(25,  0,  0,  0, 43, 30),
/*  1 */    CXTPDockingPaneContextStickerWnd::SPRITEINFO( 0, 25, 30, 33, 30, 43),
/*  2 */    CXTPDockingPaneContextStickerWnd::SPRITEINFO(25, 63, 43,  0, 43, 30),
/*  3 */    CXTPDockingPaneContextStickerWnd::SPRITEINFO(63, 25,  0, 33, 30, 43),
/*  4 */    CXTPDockingPaneContextStickerWnd::SPRITEINFO(25,  0,  0, 76, 43, 30),
/*  5 */    CXTPDockingPaneContextStickerWnd::SPRITEINFO( 0, 25, 90, 33, 30, 43),
/*  6 */    CXTPDockingPaneContextStickerWnd::SPRITEINFO(25, 63, 43, 76, 43, 30),
/*  7 */    CXTPDockingPaneContextStickerWnd::SPRITEINFO(63, 25, 60, 33, 30, 43),
/*  8 */    CXTPDockingPaneContextStickerWnd::SPRITEINFO(30, 30, 86,  0, 33, 33),
/*  9 */    CXTPDockingPaneContextStickerWnd::SPRITEINFO(30, 30, 86, 76, 33, 33),
/* 10 */    CXTPDockingPaneContextStickerWnd::SPRITEINFO(25, 25,  0,  0, 43, 43),

/*  0 */    CXTPDockingPaneContextStickerWnd::SPRITEINFO(25,  0,  0,  0, 43, 30),
/*  1 */    CXTPDockingPaneContextStickerWnd::SPRITEINFO( 0, 25, 30, 33, 30, 43),
/*  2 */    CXTPDockingPaneContextStickerWnd::SPRITEINFO(25, 63, 43,  0, 43, 30),
/*  3 */    CXTPDockingPaneContextStickerWnd::SPRITEINFO(63, 25,  0, 33, 30, 43),
/*  4 */    CXTPDockingPaneContextStickerWnd::SPRITEINFO(25,  0,  0, 76, 43, 30),
/*  5 */    CXTPDockingPaneContextStickerWnd::SPRITEINFO( 0, 25, 90, 33, 30, 43),
/*  6 */    CXTPDockingPaneContextStickerWnd::SPRITEINFO(25, 63, 43, 76, 43, 30),
/*  7 */    CXTPDockingPaneContextStickerWnd::SPRITEINFO(63, 25, 60, 33, 30, 43),
/*  8 */    CXTPDockingPaneContextStickerWnd::SPRITEINFO(30, 30, 86,  0, 33, 33),
/*  9 */    CXTPDockingPaneContextStickerWnd::SPRITEINFO(30, 30, 86, 76, 33, 33),
/* 10 */    CXTPDockingPaneContextStickerWnd::SPRITEINFO(25, 25,  0,  0, 43, 43),
};

static CXTPDockingPaneContextStickerWnd::SPRITEINFO arrSpritesStyckerVisualStudio2005[] =
{
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(30, 0, 61, 29, 29, 30),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(0, 30, 90, 30, 30, 29),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(30, 59, 91, 0, 29, 30),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(60, 30, 62, 0, 30, 29),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(30, 0, 61, 29 + 61, 29, 30),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(0, 30, 90, 30 + 61, 30, 29),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(30, 59, 91, 0 + 61, 29, 30),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(60, 30, 62, 0 + 61, 30, 29),

	CXTPDockingPaneContextStickerWnd::SPRITEINFO(30, 30, 120, 82, 28, 29),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(24, 24, 120, 41, 41, 41),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(24, 24, 120, 0, 41, 41),

	CXTPDockingPaneContextStickerWnd::SPRITEINFO(0, 0, 0, 29, 29, 32),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(0, 0, 29, 32, 32, 29),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(0, 0, 32, 0, 29, 32),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(0, 0, 0, 0, 32, 29),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(0, 0, 0, 29 + 61, 29, 32),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(0, 0, 29, 32 + 61, 32, 29),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(0, 0, 32, 0 + 61, 29, 32),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(0, 0, 0, 0 + 61, 32, 29),

};

static CXTPDockingPaneContextStickerWnd::SPRITEINFO arrSpritesStyckerVisualStudio2008[] =
{
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(36, 0, 142, 35, 35, 35),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(0, 35, 142 + 35, 35, 35, 35),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(36, 70, 142 + 35, 0, 35, 35),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(72, 35, 142, 0, 35, 35),

	CXTPDockingPaneContextStickerWnd::SPRITEINFO(36, 0, 212, 35, 35, 35),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(0, 35, 212 + 35, 35, 35, 35),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(36, 70, 212 + 35, 0, 35, 35),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(72, 35, 212, 0, 35, 35),

	CXTPDockingPaneContextStickerWnd::SPRITEINFO(37, 38, 359, 0, 32, 31),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(37, 38, 359, 31, 32, 31),

	CXTPDockingPaneContextStickerWnd::SPRITEINFO(15, 15, 282, 0, 77, 75),   // Client

	CXTPDockingPaneContextStickerWnd::SPRITEINFO(0, 0, 0, 35, 35, 36),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(0, 0, 35, 36, 36, 35),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(0, 0, 36, 0, 35, 36),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(0, 0, 0, 0, 36, 35),

	CXTPDockingPaneContextStickerWnd::SPRITEINFO(0, 0, 0 + 71, 35, 35, 36),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(0, 0, 35 + 71, 36, 36, 35),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(0, 0, 36 + 71, 0, 35, 36),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(0, 0, 0 + 71, 0, 36, 35),

};


static CXTPDockingPaneContextStickerWnd::SPRITEINFO arrSpritesStyckerVisualStudio2010[] =
{
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(36, 0, 80, 40, 40, 40),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(0, 36, 80 + 40, 40, 40, 40),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(36, 72, 80 + 40, 0, 40, 40),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(72, 36, 80, 0, 40, 40),

	CXTPDockingPaneContextStickerWnd::SPRITEINFO(36, 0, 80, 40 + 80, 40, 40),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(0, 36, 80 + 40, 40 + 80, 40, 40),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(36, 72, 80 + 40, 0 + 80, 40, 40),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(72, 36, 80, 0 + 80, 40, 40),

	CXTPDockingPaneContextStickerWnd::SPRITEINFO(40, 40, 160, 120, 32, 32),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(40, 40, 192, 120, 32, 32),

	CXTPDockingPaneContextStickerWnd::SPRITEINFO(0, 0, 160, 0, 112, 112),   // Client

	CXTPDockingPaneContextStickerWnd::SPRITEINFO(0, 0, 0, 40, 40, 40),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(0, 0, 40, 40, 40, 40),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(0, 0, 40, 0, 40, 40),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(0, 0, 0, 0, 40, 40),

	CXTPDockingPaneContextStickerWnd::SPRITEINFO(0, 0, 0, 40 + 80, 40, 40),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(0, 0, 40, 40 + 80, 40, 40),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(0, 0, 40, 0 + 80, 40, 40),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(0, 0, 0, 0 + 80, 40, 40),
};


static CXTPDockingPaneContextStickerWnd::SPRITEINFO arrSpritesStyckerVisualStudio2012[] =
{
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(36, 0, 80, 40, 40, 40),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(0, 36, 80 + 40, 40, 40, 40),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(36, 72, 80 + 40, 0, 40, 40),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(72, 36, 80, 0, 40, 40),

	CXTPDockingPaneContextStickerWnd::SPRITEINFO(36, 0, 80, 40 + 80, 40, 40),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(0, 36, 80 + 40, 40 + 80, 40, 40),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(36, 72, 80 + 40, 0 + 80, 40, 40),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(72, 36, 80, 0 + 80, 40, 40),

	CXTPDockingPaneContextStickerWnd::SPRITEINFO(40, 40, 160, 120, 32, 32),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(40, 40, 192, 120, 32, 32),

	CXTPDockingPaneContextStickerWnd::SPRITEINFO(0, 0, 160, 0, 112, 112),   // Client

	CXTPDockingPaneContextStickerWnd::SPRITEINFO(0, 0, 0, 40, 40, 40),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(0, 0, 40, 40, 40, 40),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(0, 0, 40, 0, 40, 40),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(0, 0, 0, 0, 40, 40),

	CXTPDockingPaneContextStickerWnd::SPRITEINFO(0, 0, 0, 40 + 80, 40, 40),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(0, 0, 40, 40 + 80, 40, 40),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(0, 0, 40, 0 + 80, 40, 40),
	CXTPDockingPaneContextStickerWnd::SPRITEINFO(0, 0, 0, 0 + 80, 40, 40),
};


void CXTPDockingPaneContextStickerWnd::DrawTransparent(CDC* pDC , const CPoint& ptDest, const CSize& sz, CBitmap* pBitmap)
{
	CImageList il;
	il.Create(sz.cx, sz.cy, ILC_COLOR24 | ILC_MASK, 0, 1);
	il.Add(pBitmap, RGB(0, 0xFF, 0));

	il.Draw(pDC, 0, ptDest, ILD_NORMAL);
}

void CXTPDockingPaneContextStickerWnd::DrawSprite(CDC* pDC, UINT nID, SPRITEINFO* pSpriteInfo, BOOL bClientBitmap)
{
	CBitmap bmp;

	{
		CXTPResourceManager::CManageState state;
		HBITMAP hBitmap = CXTPImageManagerIcon::LoadBitmapFromResource(MAKEINTRESOURCE(nID), NULL);
		ASSERT(NULL != hBitmap); // Bitmap can not be loaded
		bmp.Attach(hBitmap);
	}

	CSize sz(pSpriteInfo->rcSrc.Width(), pSpriteInfo->rcSrc.Height());

	CBitmap bmpSprite;
	bmpSprite.CreateCompatibleBitmap(pDC, sz.cx, sz.cy);

	if (bmpSprite.GetSafeHandle())
	{
		CXTPCompatibleDC dcSprite(pDC, &bmpSprite);
		CXTPCompatibleDC dc(pDC, &bmp);
		dcSprite.BitBlt(0, 0, sz.cx, sz.cy, &dc, pSpriteInfo->rcSrc.left, pSpriteInfo->rcSrc.top, SRCCOPY);
	}

	CPoint ptDest = bClientBitmap ? pSpriteInfo->ptDest : CPoint(0, 0);

	DrawTransparent(pDC, ptDest, sz, &bmpSprite);
}


void CXTPDockingPaneContextStickerWnd::OnDraw(CDC* pDC)
{
	//CXTPDockingPanePaintManager *pPaintManager = m_pContext->m_pManager->GetPaintManager();

	BOOL bClient = (m_typeSticker & xtpPaneStickerClient) == xtpPaneStickerClient;
	XTPDockingContextStickerStyle style = m_pContext->GetStickerStyle();

	if (xtpPaneStickerStyleVisualStudio2005Beta == style)
	{
		XTPCurrentSystemTheme theme = XTPColorManager()->GetCurrentSystemTheme();

		UINT nIDBitmap = theme == xtpSystemThemeOlive ? XTP_IDB_DOCKINGPANE_STICKERS_OLIVE :
			theme == xtpSystemThemeSilver ? XTP_IDB_DOCKINGPANE_STICKERS_SILVER : XTP_IDB_DOCKINGPANE_STICKERS_BLUE;

		SPRITEINFO *sprites = arrSpritesStyckerVisualStudio2005Beta;

		if (bClient)
			DrawSprite(pDC, XTP_IDB_DOCKINGPANE_STICKER_CLIENT, &sprites[SPRITE_STICKER_CLIENT]);
		if (m_typeSticker & xtpPaneStickerTop)
			DrawSprite(pDC, nIDBitmap, &sprites[m_selectedSticker == xtpPaneStickerTop ? SPRITE_STICKER_TOP_SELECTED : SPRITE_STICKER_TOP], bClient);
		if (m_typeSticker & xtpPaneStickerLeft)
			DrawSprite(pDC, nIDBitmap, &sprites[m_selectedSticker == xtpPaneStickerLeft ? SPRITE_STICKER_LEFT_SELECTED : SPRITE_STICKER_LEFT], bClient);
		if (m_typeSticker & xtpPaneStickerBottom)
			DrawSprite(pDC, nIDBitmap, &sprites[m_selectedSticker == xtpPaneStickerBottom ? SPRITE_STICKER_BOTTOM_SELECTED : SPRITE_STICKER_BOTTOM], bClient);
		if (m_typeSticker & xtpPaneStickerRight)
			DrawSprite(pDC, nIDBitmap, &sprites[m_selectedSticker == xtpPaneStickerRight ? SPRITE_STICKER_RIGHT_SELECTED : SPRITE_STICKER_RIGHT], bClient);
		if (m_typeSticker & xtpPaneStickerCenter)
			DrawSprite(pDC, nIDBitmap, &sprites[m_selectedSticker == xtpPaneStickerCenter ? SPRITE_STICKER_CENTER_SELECTED : SPRITE_STICKER_CENTER]);
	}
	else if (xtpPaneStickerStyleVisualStudio2005 == style)
	{
		SPRITEINFO* sprites = arrSpritesStyckerVisualStudio2005;
		int nClientSprite = bClient ? 0 : 11;

		if (bClient)
			DrawSprite(pDC, XTP_IDB_DOCKINGPANE_STICKERS_2005, &sprites[m_selectedSticker == xtpPaneStickerCenter ? SPRITE_STICKER_CENTER_SELECTED : SPRITE_STICKER_CLIENT]);
		if (m_typeSticker & xtpPaneStickerTop)
			DrawSprite(pDC, XTP_IDB_DOCKINGPANE_STICKERS_2005, &sprites[nClientSprite + (m_selectedSticker == xtpPaneStickerTop ? SPRITE_STICKER_TOP_SELECTED : SPRITE_STICKER_TOP)], bClient);
		if (m_typeSticker & xtpPaneStickerLeft)
			DrawSprite(pDC, XTP_IDB_DOCKINGPANE_STICKERS_2005, &sprites[nClientSprite + (m_selectedSticker == xtpPaneStickerLeft ? SPRITE_STICKER_LEFT_SELECTED : SPRITE_STICKER_LEFT)], bClient);
		if (m_typeSticker & xtpPaneStickerBottom)
			DrawSprite(pDC, XTP_IDB_DOCKINGPANE_STICKERS_2005, &sprites[nClientSprite + (m_selectedSticker == xtpPaneStickerBottom ? SPRITE_STICKER_BOTTOM_SELECTED : SPRITE_STICKER_BOTTOM)], bClient);
		if (m_typeSticker & xtpPaneStickerRight)
			DrawSprite(pDC, XTP_IDB_DOCKINGPANE_STICKERS_2005, &sprites[nClientSprite + (m_selectedSticker == xtpPaneStickerRight ? SPRITE_STICKER_RIGHT_SELECTED : SPRITE_STICKER_RIGHT)], bClient);
		if (m_typeSticker & xtpPaneStickerCenter)
			DrawSprite(pDC, XTP_IDB_DOCKINGPANE_STICKERS_2005, &sprites[SPRITE_STICKER_CENTER]);
	}
	else if (xtpPaneStickerStyleVisualStudio2008      == style ||
			  xtpPaneStickerStyleVisualStudio2010      == style ||
				xtpPaneStickerStyleVisualStudio2012Light == style ||
				xtpPaneStickerStyleVisualStudio2012Dark  == style)
	{
		int nClientSprite = bClient ? 0 : 11;

		UINT nIDBitmap = 0;
		SPRITEINFO *sprites = NULL;

		switch(style)
		{
		case xtpPaneStickerStyleVisualStudio2005Beta:
			{
				XTPCurrentSystemTheme theme = XTPColorManager()->GetCurrentSystemTheme();

				switch(theme)
				{
				case xtpSystemThemeBlue:    nIDBitmap = XTP_IDB_DOCKINGPANE_STICKERS_BLUE;   break;
				case xtpSystemThemeOlive:  nIDBitmap = XTP_IDB_DOCKINGPANE_STICKERS_OLIVE;  break;
				case xtpSystemThemeSilver: nIDBitmap = XTP_IDB_DOCKINGPANE_STICKERS_SILVER; break;
				default:                      nIDBitmap = XTP_IDB_DOCKINGPANE_STICKERS_BLUE;   break;
				}
				sprites = arrSpritesStyckerVisualStudio2005Beta;
			}
			break;
		case xtpPaneStickerStyleVisualStudio2005:
			nIDBitmap = XTP_IDB_DOCKINGPANE_STICKERS_2005;
			sprites   = arrSpritesStyckerVisualStudio2005;
			break;
		case xtpPaneStickerStyleVisualStudio2008:
			nIDBitmap = XTP_IDB_DOCKINGPANE_STICKERS_2008;
			sprites   = arrSpritesStyckerVisualStudio2008;
			break;
		case xtpPaneStickerStyleVisualStudio2010:
			nIDBitmap = XTP_IDB_DOCKINGPANE_STICKERS_2010;
			sprites   = arrSpritesStyckerVisualStudio2010;
			break;
		case xtpPaneStickerStyleVisualStudio2012Light:
			nIDBitmap = XTP_IDB_DOCKINGPANE_STICKERS_2012_LIGHT;
			sprites   = arrSpritesStyckerVisualStudio2012;
			break;
		case xtpPaneStickerStyleVisualStudio2012Dark:
			nIDBitmap = XTP_IDB_DOCKINGPANE_STICKERS_2012_DARK;
			sprites   = arrSpritesStyckerVisualStudio2012;
			break;
		}

		if (bClient)
		{
			DrawSprite(pDC, nIDBitmap, &sprites[SPRITE_STICKER_CLIENT]);
		}

		if (m_typeSticker & xtpPaneStickerTop)
			DrawSprite(pDC, nIDBitmap, &sprites[nClientSprite + (m_selectedSticker == xtpPaneStickerTop ? SPRITE_STICKER_TOP_SELECTED : SPRITE_STICKER_TOP)], bClient);
		if (m_typeSticker & xtpPaneStickerLeft)
			DrawSprite(pDC, nIDBitmap, &sprites[nClientSprite + (m_selectedSticker == xtpPaneStickerLeft ? SPRITE_STICKER_LEFT_SELECTED : SPRITE_STICKER_LEFT)], bClient);
		if (m_typeSticker & xtpPaneStickerBottom)
			DrawSprite(pDC, nIDBitmap, &sprites[nClientSprite + (m_selectedSticker == xtpPaneStickerBottom ? SPRITE_STICKER_BOTTOM_SELECTED : SPRITE_STICKER_BOTTOM)], bClient);
		if (m_typeSticker & xtpPaneStickerRight)
			DrawSprite(pDC, nIDBitmap, &sprites[nClientSprite + (m_selectedSticker == xtpPaneStickerRight ? SPRITE_STICKER_RIGHT_SELECTED : SPRITE_STICKER_RIGHT)], bClient);
		if (m_typeSticker & xtpPaneStickerCenter)
			DrawSprite(pDC, nIDBitmap, &sprites[m_selectedSticker == xtpPaneStickerCenter ? SPRITE_STICKER_CENTER_SELECTED : SPRITE_STICKER_CENTER]);
	}
}


void CXTPDockingPaneContextStickerWnd::OnPaint()
{
	CPaintDC dcPaint(this); // device context for painting
	CXTPBufferDC dc(dcPaint, CXTPClientRect(this));

	OnDraw(&dc);
}

XTPDockingPaneStickerType CXTPDockingPaneContextStickerWnd::HitTest(CPoint pt)
{
	CXTPClientRect rc(this);
	ScreenToClient(&pt);

	if (!rc.PtInRect(pt))
		return xtpPaneStickerNone;

	CClientDC dcClient(this);

	CBitmap bmp;
	bmp.CreateCompatibleBitmap(&dcClient, rc.Width(), rc.Height());

	CXTPCompatibleDC dc(&dcClient, &bmp);
	dc.FillSolidRect(rc, 0);

	XTPDockingContextStickerStyle style = m_pContext->GetStickerStyle();

	UINT nIDBitmap = XTP_IDB_DOCKINGPANE_STICKERS_BLUE;

	switch(style)
	{
	case xtpPaneStickerStyleVisualStudio2005:      nIDBitmap = XTP_IDB_DOCKINGPANE_STICKERS_2005;       break;
	case xtpPaneStickerStyleVisualStudio2008:      nIDBitmap = XTP_IDB_DOCKINGPANE_STICKERS_2008;       break;
	case xtpPaneStickerStyleVisualStudio2010:      nIDBitmap = XTP_IDB_DOCKINGPANE_STICKERS_2010;       break;
	case xtpPaneStickerStyleVisualStudio2012Light: nIDBitmap = XTP_IDB_DOCKINGPANE_STICKERS_2012_LIGHT; break;
	case xtpPaneStickerStyleVisualStudio2012Dark:  nIDBitmap = XTP_IDB_DOCKINGPANE_STICKERS_2012_DARK;  break;
	}

	UINT nIDBitmapClient = style == xtpPaneStickerStyleVisualStudio2005Beta ? XTP_IDB_DOCKINGPANE_STICKER_CLIENT : nIDBitmap;

	BOOL bClient = (m_typeSticker & xtpPaneStickerClient) == xtpPaneStickerClient;

	SPRITEINFO *sprites = NULL;

	switch(style)
	{
	case xtpPaneStickerStyleVisualStudio2005Beta:  sprites = arrSpritesStyckerVisualStudio2005Beta; break;
	case xtpPaneStickerStyleVisualStudio2005:      sprites = arrSpritesStyckerVisualStudio2005;     break;
	case xtpPaneStickerStyleVisualStudio2008:      sprites = arrSpritesStyckerVisualStudio2008;     break;
	case xtpPaneStickerStyleVisualStudio2010:      sprites = arrSpritesStyckerVisualStudio2010;     break;
	case xtpPaneStickerStyleVisualStudio2012Light: sprites = arrSpritesStyckerVisualStudio2012;     break;
	case xtpPaneStickerStyleVisualStudio2012Dark:  sprites = arrSpritesStyckerVisualStudio2012;     break;
	}

	if (m_typeSticker & xtpPaneStickerTop)
	{
		DrawSprite(&dc, nIDBitmap, &sprites[SPRITE_STICKER_TOP], bClient);
		if (dc.GetPixel(pt) != 0)
			return xtpPaneStickerTop;
	}
	if (m_typeSticker & xtpPaneStickerLeft)
	{
		DrawSprite(&dc, nIDBitmap, &sprites[SPRITE_STICKER_LEFT], bClient);
		if (dc.GetPixel(pt) != 0)
			return xtpPaneStickerLeft;
	}
	if (m_typeSticker & xtpPaneStickerBottom)
	{
		DrawSprite(&dc, nIDBitmap, &sprites[SPRITE_STICKER_BOTTOM], bClient);
		if (dc.GetPixel(pt) != 0)
			return xtpPaneStickerBottom;
	}
	if (m_typeSticker & xtpPaneStickerRight)
	{
		DrawSprite(&dc, nIDBitmap, &sprites[SPRITE_STICKER_RIGHT], bClient);
		if (dc.GetPixel(pt) != 0)
			return xtpPaneStickerRight;
	}
	if (m_typeSticker & xtpPaneStickerCenter)
	{
		DrawSprite(&dc, nIDBitmapClient, &sprites[SPRITE_STICKER_CLIENT]);
		if (dc.GetPixel(pt) != 0)
			return xtpPaneStickerCenter;
	}

	return xtpPaneStickerNone;
}
