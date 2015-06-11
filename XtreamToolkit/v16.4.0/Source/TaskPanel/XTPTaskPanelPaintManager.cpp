// XTPTaskPanelPaintManager.cpp : implementation of the CXTPTaskPanelPaintManager class.
//
// This file is a part of the XTREME TASKPANEL MFC class library.
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

#include <Common/XTPVC80Helpers.h>
#include <Common/XTPDrawHelpers.h>
#include <Common/XTPWinThemeWrapper.h>
#include <Common/XTPImageManager.h>
#include <Common/XTPResourceManager.h>
#include <Common/XTPResourceImage.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPColorManager.h>
#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollBarCtrl.h>
#include <Common/ScrollBar/XTPScrollBarContainer.h>

#include <TaskPanel/Resource.h>
#include <TaskPanel/XTPTaskPanelDefines.h>
#include <TaskPanel/XTPTaskPanel.h>
#include <TaskPanel/XTPTaskPanelItem.h>
#include <TaskPanel/XTPTaskPanelGroup.h>
#include <TaskPanel/XTPTaskPanelGroupItem.h>
#include <TaskPanel/XTPTaskPanelItems.h>
#include <TaskPanel/XTPTaskPanelPaintManager.h>


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#define NORMCOLOR(a) a < 0 ? 0: a > 255 ? 255 : a
#define COLORREF_TRANSPARENT 0x1FFFFFFF

const CRect CXTPTaskPanelPaintManager::rectDefault(CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPTaskPanelPaintManager::CXTPTaskPanelPaintManager()
{
	m_bEmulateEplorerTheme = TRUE;
	m_bBoldCaption = TRUE;
	m_bOfficeHighlight = FALSE;
	m_bInvertDragRect = FALSE;
	m_bCaptionScrollButton = FALSE;

	m_bUseStandardItemsFont = TRUE;
	m_bUseStandardCaptionFont = TRUE;
	m_eGripper = xtpTaskPanelGripperPlain;

	m_hGripperNormal = m_hGripperSpecial = 0;

	m_rcGroupOuterMargins.SetRect(0, 0, 0, 0);
	m_rcGroupInnerMargins.SetRect(11, 8, 11, 8);
	m_rcItemOuterMargins.SetRect(0, 0, 0, 0);
	m_rcItemInnerMargins.SetRect(2, 2, 2, 2);
	m_rcImageLayoutIconPadding.SetRect(6, 6, 6, 6);
	m_rcItemIconPadding.SetRect(0, 0, 0, 0);
	m_rcGroupIconPadding.SetRect(0, 0, 0, 0);

	m_rcControlMargins.SetRect(12, 12, 12, 12);
	m_nGroupSpacing = 15;
	m_bEmbossedDisabledText = FALSE;

	ZeroMemory(m_bmpGrippers, sizeof(m_bmpGrippers));

	m_bUseBitmapGrippers = FALSE;
	m_bOfficeBorder = FALSE;
	m_bLeftRoundedCorner = m_bRightRoundedCorner = FALSE;
	m_nCaptionHeight = 0;

	m_nItemTextFormat = DT_WORDBREAK | DT_NOPREFIX;
	m_nCaptionTextFormat = DT_SINGLELINE | DT_LEFT | DT_END_ELLIPSIS | DT_VCENTER | DT_NOPREFIX;
}

XTP_TASKPANEL_GROUPCOLORS* CXTPTaskPanelPaintManager::GetGroupColors(CXTPTaskPanelGroup* pGroup)
{
	return GetGroupColors(pGroup->IsSpecialGroup());
}

void CXTPTaskPanelPaintManager::FreeBitmaps()
{
	for (int i = 0; i < 8; i++)
	{
		if (m_bmpGrippers[i])
		{
			DeleteObject(m_bmpGrippers[i]);
			m_bmpGrippers[i] = 0;
		}
	}
}

CFont* CXTPTaskPanelPaintManager::GetItemFont(CXTPTaskPanelItem* pItem)
{
	if (!pItem)
		return &m_fntIcon;

	if (pItem->m_fntItem.GetSafeHandle())
		return &pItem->m_fntItem;

	if (pItem->GetType() == xtpTaskItemTypeGroup)
		return &m_fntCaption;

	BOOL bBold = ((CXTPTaskPanelGroupItem*)pItem)->IsBold();

	return (((CXTPTaskPanelGroupItem*)pItem)->IsItemHot() && (pItem->GetTaskPanel()->GetHotTrackStyle() == xtpTaskPanelHighlightText)) ?
		(bBold ? &m_fntIconHotBold: &m_fntIconHot) : (bBold ? &m_fntIconBold : &m_fntIcon);
}

CXTPTaskPanelPaintManager::~CXTPTaskPanelPaintManager()
{
	if (m_hGripperNormal) DestroyIcon(m_hGripperNormal);
	if (m_hGripperSpecial) DestroyIcon(m_hGripperSpecial);
	FreeBitmaps();
}

CPoint CXTPTaskPanelPaintManager::GetHighlightOffset(CXTPTaskPanelGroupItem* pItem)
{
	if (pItem->IsItemSelected() && !m_bOfficeHighlight)
		return CPoint(1, 1);
	return CPoint(0, 0);
}

void CXTPTaskPanelPaintManager::ConvertToHSL(COLORREF clr, double& h, double& s, double& l)
{
	double r = (double)GetRValue(clr)/255;
	double g = (double)GetGValue(clr)/255;
	double b = (double)GetBValue(clr)/255;

	double maxcolor = __max(r, __max(g, b));
	double mincolor = __min(r, __min(g, b));

	l = (maxcolor + mincolor)/2;

	if (maxcolor == mincolor)
	{
		h = 0;
		s = 0;
	}
	else
	{
		if (l < 0.5)
			s = (maxcolor-mincolor)/(maxcolor + mincolor);
		else
			s = (maxcolor-mincolor)/(2.0-maxcolor-mincolor);

		if (r == maxcolor)
			h = (g-b)/(maxcolor-mincolor);
		else if (g == maxcolor)
			h = 2.0+(b-r)/(maxcolor-mincolor);
		else
			h = 4.0+(r-g)/(maxcolor-mincolor);

		h /= 6.0;
		if (h < 0.0) h += 1;
	}
}

double CXTPTaskPanelPaintManager::ConvertHueToRGB(double temp1, double temp2, double temp3)
{
	if (temp3 < 0)
		temp3 = temp3 + 1.0;
	if (temp3 > 1)
		temp3 = temp3-1.0;

	if (6.0*temp3 < 1)
		return (temp1+(temp2-temp1)*temp3 * 6.0);

	else if (2.0*temp3 < 1)
		return temp2;

	else if (3.0*temp3 < 2.0)
		return (temp1+(temp2-temp1)*((2.0/3.0)-temp3)*6.0);

	return temp1;
}

COLORREF  CXTPTaskPanelPaintManager::ConvertFromHSL(double h, double s, double l)
{
	double r, g, b;
	double temp1, temp2;

	if (s == 0)
	{
		r = g = b = l;
	}
	else
	{
		if (l < 0.5)
			temp2 = l*(1.0 + s);
		else
			temp2 = l + s-l*s;

		temp1 = 2.0*l-temp2;

		r = ConvertHueToRGB(temp1, temp2, h + 1.0/3.0);
		g = ConvertHueToRGB(temp1, temp2, h);
		b = ConvertHueToRGB(temp1, temp2, h-1.0/3.0);
	}

	return RGB((BYTE)(r * 255), (BYTE)(g * 255), (BYTE)(b * 255));
}

void CXTPTaskPanelPaintManager::Triangle(CDC* pDC, CPoint pt0, CPoint pt1, CPoint pt2, COLORREF clr)
{
	CXTPPenDC pen (*pDC, clr);
	CXTPBrushDC brush (*pDC, clr);

	CPoint pts[3];
	pts[0] = pt0;
	pts[1] = pt1;
	pts[2] = pt2;
	pDC->Polygon(pts, 3);
}

void CXTPTaskPanelPaintManager::DrawScrollArrow(CDC* pDC, CPoint pt0, CPoint pt1, CPoint pt2, BOOL bEnabled)
{
	if (bEnabled)
	{
		Triangle(pDC, pt0, pt1, pt2, 0);
	}
	else
	{
		CPoint pt(1, 1);
		Triangle(pDC, pt0 + pt, pt1 + pt, pt2 + pt, GetXtremeColor(COLOR_3DHIGHLIGHT));
		Triangle(pDC, pt0, pt1, pt2, GetXtremeColor(COLOR_3DSHADOW));
	}
}


void CXTPTaskPanelPaintManager::DrawScrollButton(CDC* pDC, CXTPTaskPanelScrollButton* pScrollButton)
{
	if (!pScrollButton->IsVisible())
		return;

	CRect rc = pScrollButton->rcButton;

	if (m_bOfficeHighlight && !m_bCaptionScrollButton)
	{
		COLORREF clrNormal = GetGroupColors(pScrollButton->pGroupScroll)->clrClient.clrLight;
		COLORREF clrBrush = pScrollButton->bPressed ? (COLORREF)m_clrHighlightPressed :
			pScrollButton->bHot ? (COLORREF)m_clrHighlightHot : (clrNormal == COLORREF_TRANSPARENT ? (COLORREF)m_clrBackground : clrNormal);

		Rectangle(pDC, rc, m_clrHighlightBorder.clrLight, clrBrush);
	}
	else
	{
		pDC->FillSolidRect(rc, GetXtremeColor(COLOR_3DFACE));

		if (!pScrollButton->bPressed)
			pDC->Draw3dRect(rc, GetXtremeColor(COLOR_3DHIGHLIGHT), GetXtremeColor(COLOR_3DSHADOW));
		else
			pDC->Draw3dRect(rc, GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DHIGHLIGHT));
	}

	CPoint pt = rc.CenterPoint();

	if (!pScrollButton->bScrollUp)
		DrawScrollArrow(pDC, CPoint(pt.x - 5, pt.y - 2), CPoint(pt.x - 1, pt.y + 2), CPoint(pt.x + 3, pt.y - 2), pScrollButton->bEnabled);
	else
		DrawScrollArrow(pDC, CPoint(pt.x - 5, pt.y + 1), CPoint(pt.x - 1, pt.y - 3), CPoint(pt.x + 3, pt.y + 1), pScrollButton->bEnabled);
}


void CXTPTaskPanelPaintManager::CreateGripperIcon(HICON& hIcon, UINT nIDResource, COLORREF clrGripperBack)
{
	if (hIcon)
	{
		DestroyIcon(hIcon);
		hIcon = 0;
	}

	CBitmap bmpGripper;

	double hueBack, satBack, lumBack;
	ConvertToHSL(clrGripperBack, hueBack, satBack, lumBack);


	CBitmap bmp;
	if (XTPResourceManager()->LoadBitmap(&bmp, nIDResource))
	{
		BITMAP bmpInfo;
		bmp.GetBitmap(&bmpInfo);

		CWindowDC dc(CWnd::GetDesktopWindow());

		if (bmpGripper.CreateCompatibleBitmap(&dc, bmpInfo.bmWidth, m_nCaptionHeight))
		{
			CXTPCompatibleDC dcTemplate(&dc, &bmp);
			CXTPCompatibleDC dcGripper(&dc, &bmpGripper);

			COLORREF clrTemplateBack = dcTemplate.GetPixel(0, 0);

			double hueTrans, satTrans, lumTrans;
			ConvertToHSL(clrTemplateBack, hueTrans, satTrans, lumTrans);

			for (int x = 0; x < bmpInfo.bmWidth; x++)
			for (int y = 0; y < bmpInfo.bmWidth; y++)
			{
				COLORREF clrTemplate = dcTemplate.GetPixel(x, y);

				if (clrTemplate == clrTemplateBack)
				{
					clrTemplate = RGB(0, 0xFF, 0);
				}
				else
				{

					double huePixel, satPixel, lumPixel;
					ConvertToHSL(clrTemplate, huePixel, satPixel, lumPixel);

					double fLumOffset = lumPixel / lumTrans;
					if (lumPixel <= 0.9 || lumBack > 0.9)
					{
						lumPixel = min(1.0, lumBack * fLumOffset);
					}

					clrTemplate = ConvertFromHSL(hueBack, satBack, lumPixel);
				}

				dcGripper.SetPixel(x, y, clrTemplate);
			}
		}


		CImageList il;
		il.Create(bmpInfo.bmWidth, bmpInfo.bmHeight, ILC_COLOR24 | ILC_MASK, 0, 1);

		il.Add(&bmpGripper, RGB(0, 0xFF, 0));

		hIcon = il.ExtractIcon(0);
	}



}

void CXTPTaskPanelPaintManager::PremultplyAlpha(HBITMAP& bmp)
{
	HBITMAP hBitmap = bmp;
	bmp = CXTPImageManagerIcon::PreMultiplyAlphaBitmap(hBitmap);
	DeleteObject(hBitmap);
}

void CXTPTaskPanelPaintManager::CreateGripperBitmaps()
{
	m_bUseBitmapGrippers = FALSE;

	XTPCurrentSystemTheme systemTheme = XTPColorManager()->GetCurrentSystemTheme();

	if (systemTheme != xtpSystemThemeUnknown)
	{
		WCHAR pszThemeFileName[MAX_PATH];
		pszThemeFileName[0] = 0;
		CString strShellPath;

		CXTPWinThemeWrapper theme;
		if (SUCCEEDED(theme.GetCurrentThemeName(pszThemeFileName, MAX_PATH, NULL, MAX_PATH, NULL, 0))
			&& wcsstr(WCSLWR_S(pszThemeFileName, MAX_PATH), L"luna.msstyles"))
		{
			CString strThemeFileName(pszThemeFileName);
			strShellPath = strThemeFileName.Left(strThemeFileName.ReverseFind(_T('\\')) + 1);
		}
		else
		{
			CString strThemeFileName;
			if (GetWindowsDirectory(strThemeFileName.GetBufferSetLength(MAX_PATH), MAX_PATH) > 0)
			{
				strThemeFileName.ReleaseBuffer();
				strShellPath = strThemeFileName + _T("\\Resources\\Themes\\Luna\\");
			}

		}
		CString strColorName = systemTheme == xtpSystemThemeOlive ?
			_T("Homestead"): systemTheme == xtpSystemThemeSilver ? _T("Metallic") : _T("NormalColor");

		strShellPath = strShellPath + _T("shell\\") + strColorName + _T("\\shellstyle.dll");

		HMODULE hLib = LoadLibraryEx(strShellPath, 0, LOAD_LIBRARY_AS_DATAFILE);
		if (hLib)
		{

			FreeBitmaps();

			m_bUseBitmapGrippers = TRUE;

			for (int i = 0; i < 8; i++)
			{
				m_bmpGrippers[i] = (HBITMAP)LoadImage(hLib, MAKEINTRESOURCE(100 + i), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);

				if (m_bmpGrippers[i] && m_bUseBitmapGrippers)
				{
					PremultplyAlpha(m_bmpGrippers[i]);
				}
				else m_bUseBitmapGrippers = FALSE;
			}
			FreeLibrary(hLib);
		}

	}

	if (!m_bUseBitmapGrippers)
	{
		CreateGripperIcon(m_hGripperSpecial, XTP_IDB_TASKPANEL_GRIPPER_SPECIAL, m_groupSpecial.clrHead.clrDark);
		CreateGripperIcon(m_hGripperNormal, XTP_IDB_TASKPANEL_GRIPPER, m_groupNormal.clrHead.clrDark);
	}
}

void CXTPTaskPanelPaintManager::DrawCaptionGripperSigns(CDC* pDC, CPoint pt, COLORREF clr, BOOL bExpanded)
{
	CXTPPenDC  pen(*pDC, clr);

	if (bExpanded)
	{
		pDC->MoveTo(pt.x - 3, pt.y - 2); pDC->LineTo(pt.x, pt.y - 3- 2); pDC->LineTo(pt.x + 4, pt.y + 1- 2);
		pDC->MoveTo(pt.x - 2, pt.y - 2); pDC->LineTo(pt.x, pt.y - 2- 2); pDC->LineTo(pt.x + 3, pt.y + 1- 2);

		pDC->MoveTo(pt.x - 3, pt.y + 2); pDC->LineTo(pt.x, pt.y - 3 + 2); pDC->LineTo(pt.x + 4, pt.y + 1 + 2);
		pDC->MoveTo(pt.x - 2, pt.y + 2); pDC->LineTo(pt.x, pt.y - 2 + 2); pDC->LineTo(pt.x + 3, pt.y + 1 + 2);
	}
	else
	{
		pDC->MoveTo(pt.x - 3, pt.y - 4); pDC->LineTo(pt.x, pt.y + 3- 4); pDC->LineTo(pt.x + 4, pt.y - 1- 4);
		pDC->MoveTo(pt.x - 2, pt.y - 4); pDC->LineTo(pt.x, pt.y + 2- 4); pDC->LineTo(pt.x + 3, pt.y - 1- 4);

		pDC->MoveTo(pt.x - 3, pt.y + 0); pDC->LineTo(pt.x, pt.y + 3 + 0); pDC->LineTo(pt.x + 4, pt.y - 1 + 0);
		pDC->MoveTo(pt.x - 2, pt.y + 0); pDC->LineTo(pt.x, pt.y + 2 + 0); pDC->LineTo(pt.x + 3, pt.y - 1 + 0);
	}
}


CRect CXTPTaskPanelPaintManager::DrawCaptionGripperBitmap(CDC* pDC, CXTPTaskPanelGroup* pGroup, BOOL bExpanded, BOOL bHot, CRect rc)
{

	int nTop = max(rc.top, rc.CenterPoint().y - 10);
	CRect rcGripper(rc.right - 3 - 20, nTop, rc.right - 3, nTop + 20);

	if (m_bUseBitmapGrippers)
	{
		int nIndex = pGroup->IsSpecialGroup() ? 4 : 0;
		nIndex += bExpanded ? 0 : 2;
		nIndex += bHot ? 1 : 0;

		CXTPCompatibleDC dcGripper(NULL, CBitmap::FromHandle(m_bmpGrippers[nIndex]));

		if (XTPDrawHelpers()->m_pfnAlphaBlend)
		{
			BITMAP bmpInfo;
			GetObject(m_bmpGrippers[nIndex], sizeof(BITMAP), &bmpInfo);

			BLENDFUNCTION bf = {AC_SRC_OVER, 0, 255, 1};
			XTPDrawHelpers()->m_pfnAlphaBlend(pDC->GetSafeHdc(), rcGripper.left, rcGripper.top, bmpInfo.bmWidth, bmpInfo.bmHeight, dcGripper, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, bf);
		}

	}
	else
	{

		HICON hIcon = pGroup->IsSpecialGroup() ? m_hGripperSpecial : m_hGripperNormal;

		pDC->DrawState(rcGripper.TopLeft(), CSize(19, 19), hIcon, DST_ICON, (CBrush*)NULL);

		XTP_TASKPANEL_GROUPCOLORS* pGroupColors = GetGroupColors(pGroup);

		DrawCaptionGripperSigns(pDC, CPoint(rcGripper.CenterPoint().x - 1, rcGripper.CenterPoint().y), bHot ? pGroupColors->clrHeadTextHot : pGroupColors->clrHeadTextNormal, bExpanded);

	}


	rc.right = rcGripper.left;
	return rc;
}

int CXTPTaskPanelPaintManager::GetCaptionGripperWidth(CXTPTaskPanelGroup* pGroup, CRect rc)
{
	if (!pGroup->IsExpandable())
		return 0;

	if (rc.Width() < 20 + 6)
		return 0;

	if (m_eGripper == xtpTaskPanelGripperBitmap)
		return 23;

	if (m_eGripper == xtpTaskPanelGripperPlain)
		return 23;

	if (m_eGripper == xtpTaskPanelGripperTriangle)
		return 19;

	return 0;
}


CRect CXTPTaskPanelPaintManager::DrawCaptionGripper(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc)
{
	if (!pGroup->IsExpandable())
		return rc;

	if (rc.Width() < 20 + 6)
		return rc;


	if (m_eGripper == xtpTaskPanelGripperBitmap)
	{

		return DrawCaptionGripperBitmap(pDC, pGroup, pGroup->IsExpanded(), pGroup->IsItemHot() , rc);
	}


	if (m_eGripper == xtpTaskPanelGripperPlain)
	{

		XTP_TASKPANEL_GROUPCOLORS* pGroupColors = GetGroupColors(pGroup);

		CRect rcGripper(rc.right - 6 - 17, rc.CenterPoint().y - 8, rc.right - 6, rc.CenterPoint().y + 8);

		if (pGroup->IsItemHot())
		{
			pDC->Draw3dRect(rcGripper, GetXtremeColor(COLOR_3DHIGHLIGHT), GetXtremeColor(COLOR_3DSHADOW));
		}

		DrawCaptionGripperSigns(pDC, rcGripper.CenterPoint(), pGroup->IsItemHot() ? pGroupColors->clrHeadTextHot : pGroupColors->clrHeadTextNormal, pGroup->IsExpanded());

		rc.right = rcGripper.left;
	}

	if (m_eGripper == xtpTaskPanelGripperTriangle)
	{

		COLORREF clr = m_groupNormal.clrClientText;
		CPoint pt(rc.right - 6 - 8 - 5, rc.CenterPoint().y + 2);

		if (pGroup->IsExpanded())
			CXTPDrawHelpers::Triangle(pDC, CPoint(pt.x, pt.y - 2), CPoint(pt.x + 10, pt.y - 2), CPoint(pt.x + 5, pt.y + 5 - 2), clr);
		else
			CXTPDrawHelpers::Triangle(pDC, CPoint(pt.x + 3, pt.y - 5), CPoint(pt.x + 3 + 5, pt.y), CPoint(pt.x + 3, pt.y + 5), clr);
		rc.right = pt.x;
	}
	return rc;
}


COLORREF CXTPTaskPanelPaintManager::BlendColor(COLORREF clrFrom, COLORREF clrTo, int nAlpha)
{
	return RGB(
		NORMCOLOR((GetRValue(clrFrom) * nAlpha + GetRValue(clrTo) * (255 - nAlpha)) / 255),
		NORMCOLOR((GetGValue(clrFrom) * nAlpha + GetGValue(clrTo) * (255 - nAlpha)) / 255),
		NORMCOLOR((GetBValue(clrFrom) * nAlpha + GetBValue(clrTo) * (255 - nAlpha)) / 255));
}

void CXTPTaskPanelPaintManager::SetItemsFontIndirect(LOGFONT* pLogFont, BOOL bUseStandardItemsFont /*= FALSE*/)
{
	m_bUseStandardItemsFont = bUseStandardItemsFont;
	if (!pLogFont)
		return;

	m_fntIcon.DeleteObject();
	m_fntIconBold.DeleteObject();
	m_fntIconHot.DeleteObject();
	m_fntIconHotBold.DeleteObject();


	pLogFont->lfWeight = FW_NORMAL;
	m_fntIcon.CreateFontIndirect(pLogFont);

	pLogFont->lfUnderline = TRUE;
	m_fntIconHot.CreateFontIndirect(pLogFont);

	pLogFont->lfUnderline = FALSE;
	pLogFont->lfWeight = FW_BOLD;
	m_fntIconBold.CreateFontIndirect(pLogFont);

	pLogFont->lfUnderline = TRUE;
	m_fntIconHotBold.CreateFontIndirect(pLogFont);
}

void CXTPTaskPanelPaintManager::SetCaptionFontIndirect(LOGFONT* pLogFont, BOOL bUseStandardCaptionFont /*= FALSE*/)
{
	m_bUseStandardCaptionFont = bUseStandardCaptionFont;
	if (!pLogFont)
		return;

	m_fntCaption.DeleteObject();
	m_fntCaption.CreateFontIndirect(pLogFont);

	CWindowDC dc(CWnd::GetDesktopWindow());
	CXTPFontDC fnt(&dc, &m_fntCaption);
	m_nCaptionHeight = max(25, dc.GetTextExtent(_T(" "), 1).cy);
}

void CXTPTaskPanelPaintManager::RefreshMetrics()
{
	RefreshXtremeColors(FALSE);


	LOGFONT lfIcon;
	VERIFY(CXTPDrawHelpers::GetIconLogFont(&lfIcon));

	if (m_bUseStandardItemsFont)
	{
		SetItemsFontIndirect(&lfIcon, TRUE);
	}

	if (m_bUseStandardCaptionFont)
	{
		lfIcon.lfWeight = m_bBoldCaption ? FW_BOLD : FW_NORMAL;
		lfIcon.lfUnderline = FALSE;
		SetCaptionFontIndirect(&lfIcon, TRUE);
	}

	m_bOfficeBorder = FALSE;

	m_groupSpecial.dHeadGradientFactor = m_groupNormal.dHeadGradientFactor = 0;
	m_groupSpecial.clrClientBorder.SetStandardValue(RGB(255, 255, 255));
	m_groupNormal.clrClientBorder.SetStandardValue(RGB(255, 255, 255));


	m_clrBackground.SetStandardValue(GetXtremeColor(COLOR_WINDOW), GetXtremeColor(COLOR_WINDOW));

	m_groupSpecial.clrHead.SetStandardValue(GetXtremeColor(COLOR_ACTIVECAPTION), GetXtremeColor(COLOR_ACTIVECAPTION));
	m_groupSpecial.clrClientBorder.SetStandardValue(GetXtremeColor(COLOR_ACTIVECAPTION));
	m_groupSpecial.clrHeadTextHot.SetStandardValue(GetXtremeColor(COLOR_CAPTIONTEXT));
	m_groupSpecial.clrHeadTextNormal.SetStandardValue(GetXtremeColor(COLOR_CAPTIONTEXT));
	m_groupSpecial.clrClient.SetStandardValue(GetXtremeColor(COLOR_WINDOW));

	m_groupNormal.clrHead.SetStandardValue(GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_3DFACE));
	m_groupNormal.clrClientBorder.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
	m_groupNormal.clrHeadTextHot.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_groupNormal.clrHeadTextNormal.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_groupNormal.clrClient.SetStandardValue(GetXtremeColor(COLOR_WINDOW));

	m_groupSpecial.clrClientText.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_groupNormal.clrClientText.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_groupSpecial.clrClientLink.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_groupNormal.clrClientLink.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_groupSpecial.clrClientLinkHot.SetStandardValue(GetXtremeColor(COLOR_HIGHLIGHT));
	m_groupNormal.clrClientLinkHot.SetStandardValue(GetXtremeColor(COLOR_HIGHLIGHT));
	m_groupSpecial.clrClientLinkSelected.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_groupNormal.clrClientLinkSelected.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));

	m_bLeftRoundedCorner = m_bRightRoundedCorner = FALSE;

	if (m_bOfficeHighlight)
	{
		m_clrHighlightHot.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT));
		m_clrHighlightSelected.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_CHECKED));
		m_clrHighlightPressed.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_PUSHED));
		m_clrHighlightBorder.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER));

		if (m_bOfficeHighlight == XTP_TASKPANEL_HIGHLIGHT_OFFICE2003)
		{
			XTPCurrentSystemTheme systemTheme = XTPColorManager()->GetCurrentSystemTheme();

			if (systemTheme != xtpSystemThemeUnknown)
			{
				m_clrHighlightHot.SetStandardValue(RGB(255, 238, 194));
				m_clrHighlightPressed.SetStandardValue(RGB(254, 128, 62));
				m_clrHighlightSelected.SetStandardValue(RGB(255, 192, 111));
			}
			switch (systemTheme)
			{
				case xtpSystemThemeBlue:
				case xtpSystemThemeRoyale:
				case xtpSystemThemeAero:
					m_clrHighlightBorder.SetStandardValue(RGB(0, 0, 128)); break;
				case xtpSystemThemeOlive:
					m_clrHighlightBorder.SetStandardValue(RGB(63, 93, 56)); break;
				case xtpSystemThemeSilver:
					m_clrHighlightBorder.SetStandardValue(RGB(75, 75, 111)); break;
			}
		}
	}
	else
	{
		m_clrHighlightHot.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
		m_clrHighlightBorder.SetStandardValue(RGB(128, 128, 128), RGB(198, 198, 198));
	}
}

void CXTPTaskPanelPaintManager::FillTaskPanel(CDC* pDC, CXTPTaskPanel *pTaskPanel)
{
	CXTPClientRect rc(pTaskPanel);

	CRect rcFill(rc.left, 0, rc.right, max(rc.bottom, pTaskPanel->GetClientHeight()));

	XTPDrawHelpers()->GradientFill(pDC, rcFill, m_clrBackground, FALSE);
}

CRect CXTPTaskPanelPaintManager::MergeRect(CRect rc, CRect rcDefault)
{
	if (rc.left == CW_USEDEFAULT) rc.left = rcDefault.left;
	if (rc.top == CW_USEDEFAULT) rc.top = rcDefault.top;
	if (rc.right == CW_USEDEFAULT) rc.right = rcDefault.right;
	if (rc.bottom == CW_USEDEFAULT) rc.bottom = rcDefault.bottom;

	return rc;
}

CRect CXTPTaskPanelPaintManager::GetItemIconPadding(const CXTPTaskPanelItem* pItem)
{
	return MergeRect(const_cast<CXTPTaskPanelItem*>(pItem)->GetIconPadding(),
		pItem->IsGroup() ? m_rcGroupIconPadding : m_rcItemIconPadding);
}

CRect CXTPTaskPanelPaintManager::GetItemOuterMargins(const CXTPTaskPanelGroupItem* pItem)
{
	return MergeRect(const_cast<CXTPTaskPanelGroupItem*>(pItem)->GetMargins(), m_rcItemOuterMargins);
}

CRect CXTPTaskPanelPaintManager::GetItemInnerMargins(const CXTPTaskPanelGroupItem* /*pItem*/)
{
	return m_rcItemInnerMargins;
}

CRect CXTPTaskPanelPaintManager::GetGroupOuterMargins(const CXTPTaskPanelGroup* pGroup)
{
	return MergeRect(const_cast<CXTPTaskPanelGroup*>(pGroup)->GetOuterMargins(), m_rcGroupOuterMargins);
}

CRect CXTPTaskPanelPaintManager::GetGroupInnerMargins(const CXTPTaskPanelGroup* pGroup)
{
	return MergeRect(const_cast<CXTPTaskPanelGroup*>(pGroup)->GetInnerMargins(), m_rcGroupInnerMargins);
}

CRect CXTPTaskPanelPaintManager::GetControlMargins(const CXTPTaskPanel* pPanel)
{
	return MergeRect(const_cast<CXTPTaskPanel*>(pPanel)->GetMargins(), m_rcControlMargins);
}

int CXTPTaskPanelPaintManager::GetGroupSpacing(const CXTPTaskPanel* pPanel)
{
	return pPanel->m_nGroupSpacing == CW_USEDEFAULT ? m_nGroupSpacing : pPanel->m_nGroupSpacing;
}

void CXTPTaskPanelPaintManager::Rectangle(CDC* pDC, CRect rc, COLORREF clrPen, COLORREF clrBrush)
{
	pDC->FillSolidRect(rc, clrBrush);
	pDC->Draw3dRect(rc, clrPen, clrPen);
}

void CXTPTaskPanelPaintManager::DrawGroupItemFrame(CDC* pDC, CXTPTaskPanelGroupItem* pItem, CRect rc)
{
	if (m_bOfficeHighlight)
	{
		if (pItem->IsItemSelected() && !pItem->IsItemHot() && !pItem->IsItemPressed()) Rectangle(pDC, rc, m_clrHighlightBorder.clrLight, m_clrHighlightSelected);
		else if (pItem->IsItemSelected() && pItem->IsItemHot() && !pItem->IsItemPressed()) Rectangle(pDC, rc, m_clrHighlightBorder.clrLight, m_clrHighlightPressed);
		else if (pItem->IsItemHot() && pItem->IsItemPressed()) Rectangle(pDC, rc, m_clrHighlightBorder.clrLight, m_clrHighlightPressed);
		else if (pItem->IsItemHot() || pItem->IsItemPressed()) Rectangle(pDC, rc, m_clrHighlightBorder.clrLight, m_clrHighlightHot);

	}
	else
	{
		if (pItem->IsItemSelected() && (!pItem->IsItemHot() || pItem->IsItemPressed()))
			pDC->FillSolidRect(rc, m_clrHighlightHot);

		if (pItem->IsItemSelected() || (pItem->IsItemPressed() && pItem->IsItemHot()))
			pDC->Draw3dRect(rc, m_clrHighlightBorder.clrLight, m_clrHighlightBorder.clrDark);
		else if (pItem->IsItemHot() || pItem->IsItemPressed())
			pDC->Draw3dRect(rc, m_clrHighlightBorder.clrDark, m_clrHighlightBorder.clrLight);
	}
}

void CXTPTaskPanelPaintManager::DrawDragArrow(CDC* pDC, CXTPTaskPanelItem* pItem, CRect rc, BOOL bAfter)
{
	if (m_bInvertDragRect)
	{
		pDC->InvertRect(rc);
	}
	else
	{
		if (!pItem->IsGroup() && pItem->GetItemGroup()->GetItemLayout() != xtpTaskItemLayoutImages)
		{
			CRect rcMargin = GetControlMargins(pItem->GetTaskPanel());
			rcMargin.InflateRect(GetGroupOuterMargins(pItem->GetItemGroup()));

			CXTPClientRect rcControl(pItem->GetTaskPanel());
			rc.left = rcControl.left + rcMargin.left + 2;
			rc.right = rcControl.right - rcMargin.right - 2;
		}

		int y = bAfter ? rc.bottom - 1 : rc.top - 1;

		COLORREF clr = RGB(0 /*xFF*/, 0, 0);

		pDC->FillSolidRect(rc.left, y, rc.Width(), 1, clr);
		pDC->FillSolidRect(rc.left + 0, y - 3 , 1, 7, clr);
		pDC->FillSolidRect(rc.left + 1, y - 2 , 1, 5, clr);
		pDC->FillSolidRect(rc.left + 2, y - 1 , 1, 3, clr);
		pDC->FillSolidRect(rc.right - 1, y - 3 , 1, 7, clr);
		pDC->FillSolidRect(rc.right - 2, y - 2 , 1, 5, clr);
		pDC->FillSolidRect(rc.right - 3, y - 1 , 1, 3, clr);

	}
}

CRect CXTPTaskPanelPaintManager::DrawGroupItem(CDC* pDC, CXTPTaskPanelGroupItem* pItem, CRect rc, BOOL bDraw)
{
	XTPTaskPanelItemLayout itemLayout = pItem->GetItemGroup()->GetItemLayout();

	CRect rcItem = itemLayout == xtpTaskItemLayoutImagesWithTextBelow ? DrawGroupItemImageWithTextBelow(pDC, pItem, rc, bDraw) :
		itemLayout == xtpTaskItemLayoutImages ? DrawGroupItemImage(pDC, pItem, rc, bDraw) :  DrawGroupItemImageWithText(pDC, pItem, rc, bDraw);

	if (bDraw && pItem->IsItemDragOver() && !pItem->IsItemDragging())
	{
		DrawDragArrow(pDC, pItem, rc);
	}
	if (bDraw && pItem->GetItemGroup()->IsItemDragOver() &&
		pItem->GetParentItems()->GetLastVisibleItem() == pItem)
	{
		DrawDragArrow(pDC, pItem, rc, TRUE);
	}

	return rcItem;
}

void CXTPTaskPanelPaintManager::DrawFocusRect(CDC* pDC, CRect rc)
{
	pDC->SetTextColor(0);
	pDC->SetBkColor(0xFFFFFF);

	pDC->DrawFocusRect(rc);
}

CRect CXTPTaskPanelPaintManager::DrawGroupItemImageWithText(CDC* pDC, CXTPTaskPanelGroupItem* pItem, CRect rc, BOOL bDraw)
{
	CXTPTaskPanelGroup* pGroup = pItem->GetItemGroup();
	ASSERT(pGroup);
	if (!pGroup)
		return 0;

	XTPTaskPanelHotTrackStyle hotTrackStyle = pItem->GetTaskPanel()->GetHotTrackStyle();
	CSize szIcon = pGroup->GetItemIconSize();

	CXTPFontDC font(pDC, GetItemFont(pItem));

	CXTPImageManagerIcon* pImage = pItem->GetImage(szIcon.cx);
	BOOL bDrawIcon = pItem->GetType() != xtpTaskItemTypeControl && pImage;
	CRect rcInnerMargins = GetItemInnerMargins(pItem);

	if (bDraw)
	{
		if (hotTrackStyle == xtpTaskPanelHighlightItem)
		{
			DrawGroupItemFrame(pDC, pItem, rc);
		}
		rc.DeflateRect(rcInnerMargins.left, rcInnerMargins.top, rcInnerMargins.right, rcInnerMargins.bottom);

		CRect rcText(rc);

		if (hotTrackStyle == xtpTaskPanelHighlightItem)
			rcText.OffsetRect(GetHighlightOffset(pItem));

		CRect rcIconPadding(GetItemIconPadding(pItem));

		if (bDrawIcon)
		{
			if (hotTrackStyle == xtpTaskPanelHighlightImage)
			{
				CRect rcImage(rc.left - rcInnerMargins.left, rc.top - rcInnerMargins.top,
					rc.left + szIcon.cx + 2 + rcIconPadding.left + rcIconPadding.right,
					rc.top + szIcon.cy + 2 + rcIconPadding.top + rcIconPadding.bottom);
				DrawGroupItemFrame(pDC, pItem, rcImage);
			}
			CPoint ptIcon(rc.TopLeft());

			if (hotTrackStyle == xtpTaskPanelHighlightImage || hotTrackStyle == xtpTaskPanelHighlightItem)
				ptIcon.Offset(GetHighlightOffset(pItem));

			ptIcon.Offset(rcIconPadding.left, rcIconPadding.top);

			DrawItemImage(pDC, pItem, ptIcon, pImage, szIcon);
		}

		if (bDrawIcon || pItem->GetType() == xtpTaskItemTypeLink)
		{
			rcText.left += szIcon.cx + 7 + rcIconPadding.left + rcIconPadding.right;
		}

		if (rcText.Width() > 0)
		{
			DrawItemCaption(pDC, pItem, rcText, m_nItemTextFormat | DT_LEFT);
		}

		if (pItem->IsItemFocused() && pItem->GetTaskPanel()->IsDrawFocusRect())
		{
			DrawFocusRect(pDC, rc);
		}
		return 0;
	}
	else
	{
		CRect rcText(rc);
		rcText.bottom = rcText.top;
		rcText.DeflateRect(rcInnerMargins.left, 0, rcInnerMargins.right, 0);

		CRect rcIconPadding(GetItemIconPadding(pItem));

		if (pItem->GetType() == xtpTaskItemTypeLink || bDrawIcon)
		{
			rcText.left += szIcon.cx + 7 + rcIconPadding.left + rcIconPadding.right;
		}

		if (rcText.Width() > 0)
		{
			CSize szText = pItem->GetCaptionTextSize(pDC, rcText, (m_nItemTextFormat | DT_LEFT) & ~(DT_VCENTER | DT_CENTER));
			rcText.bottom = rcText.top + szText.cy;
			rcText.right = rcText.left + szText.cx;
		}
		int nImageHeight = bDrawIcon ? szIcon.cy + rcIconPadding.top + rcIconPadding.bottom : 0;

		return CRect(rc.left, rc.top, hotTrackStyle == xtpTaskPanelHighlightItem ? rc.right : min(rc.right, (rcText.right + rcInnerMargins.right)),
			rc.top + max(nImageHeight, rcText.Height()) + rcInnerMargins.top + rcInnerMargins.bottom);

	}
}


CRect CXTPTaskPanelPaintManager::DrawGroupItemImageWithTextBelow(CDC* pDC, CXTPTaskPanelGroupItem* pItem, CRect rc, BOOL bDraw)
{
	CXTPTaskPanelGroup* pGroup = pItem->GetItemGroup();
	ASSERT(pGroup);
	if (!pGroup)
		return 0;

	XTPTaskPanelHotTrackStyle hotTrackStyle = pItem->GetTaskPanel()->GetHotTrackStyle();
	CSize szIcon = pGroup->GetItemIconSize();

	CXTPFontDC font(pDC, GetItemFont(pItem));

	CXTPImageManagerIcon* pImage = pItem->GetImage(szIcon.cx);
	BOOL bDrawIcon = pItem->GetType() == xtpTaskItemTypeLink && pImage;
	CRect rcInnerMargins = GetItemInnerMargins(pItem);

	if (bDraw)
	{
		if (hotTrackStyle == xtpTaskPanelHighlightItem)
		{
			DrawGroupItemFrame(pDC, pItem, rc);
		}
		rc.DeflateRect(rcInnerMargins.left, rcInnerMargins.top, rcInnerMargins.right, rcInnerMargins.bottom);

		CRect rcText(rc);
		if (hotTrackStyle == xtpTaskPanelHighlightItem)
			rcText.OffsetRect(GetHighlightOffset(pItem));

		if (pItem->GetType() == xtpTaskItemTypeLink)
		{
			if (bDrawIcon)
			{
				CPoint ptIcon((rc.right + rc.left + 1 - szIcon.cx) /2, rc.top);

				if (hotTrackStyle == xtpTaskPanelHighlightImage)
				{
					CRect rcImage(ptIcon.x - 2, rc.top - 2, ptIcon.x + szIcon.cx + 2, rc.top + szIcon.cy + 2);
					DrawGroupItemFrame(pDC, pItem, rcImage);
				}

				if (hotTrackStyle == xtpTaskPanelHighlightImage || hotTrackStyle == xtpTaskPanelHighlightItem)
					ptIcon.Offset(GetHighlightOffset(pItem));

				DrawItemImage(pDC, pItem, ptIcon, pImage, szIcon);
			}

			rcText.top += szIcon.cy + 3;
		}

		if (rcText.Width() > 0)
		{
			DrawItemCaption(pDC, pItem, rcText, m_nItemTextFormat | DT_CENTER);
		}

		if (pItem->IsItemFocused() && pItem->GetTaskPanel()->IsDrawFocusRect())
		{
			DrawFocusRect(pDC, rc);
		}
		return 0;
	}
	else
	{
		CRect rcText(rc);
		rcText.bottom = rcText.top;
		rcText.DeflateRect(rcInnerMargins.left, 0, rcInnerMargins.right, 0);

		if (rcText.Width() > 0)
		{
			CSize szText = pItem->GetCaptionTextSize(pDC, rcText, m_nItemTextFormat | DT_LEFT);
			rcText.bottom = rcText.top + szText.cy;
			rcText.right = rcText.left + szText.cx;
		}
		int nHeight = rcText.Height() + (bDrawIcon ? (szIcon.cy + 3) : 0);
		int nWidth = max(bDrawIcon ? (szIcon.cx + 4) : 0, rcText.Width()) + rcInnerMargins.right + + rcInnerMargins.left;
		nWidth = min(nWidth, rc.Width());

		if (hotTrackStyle != xtpTaskPanelHighlightItem)
			return CRect(rc.CenterPoint().x - nWidth + nWidth /2, rc.top, rc.CenterPoint().x + nWidth /2, rc.top + nHeight + rcInnerMargins.top + rcInnerMargins.bottom);

		return CRect(rc.left, rc.top, rc.right, rc.top + nHeight + rcInnerMargins.top + rcInnerMargins.bottom);

	}
}


CRect CXTPTaskPanelPaintManager::DrawGroupItemImage(CDC* pDC, CXTPTaskPanelGroupItem* pItem, CRect rc, BOOL bDraw)
{
	CXTPTaskPanelGroup* pGroup = pItem->GetItemGroup();
	ASSERT(pGroup);
	if (!pGroup)
		return 0;

	XTPTaskPanelHotTrackStyle hotTrackStyle = pItem->GetTaskPanel()->GetHotTrackStyle();
	CSize szIcon = pGroup->GetItemIconSize();

	CXTPImageManagerIcon* pImage = pItem->GetImage(szIcon.cx);
	CRect rcInnerMargins = GetItemInnerMargins(pItem);
	BOOL bDrawIcon = pImage != NULL;

	if (bDraw)
	{
		CPoint ptIcon(rc.left + m_rcImageLayoutIconPadding.left, rc.top + m_rcImageLayoutIconPadding.top);

		if (hotTrackStyle != xtpTaskPanelHighlightNone)
		{
			DrawGroupItemFrame(pDC, pItem, rc);
			ptIcon.Offset(GetHighlightOffset(pItem));
		}
		rc.DeflateRect(rcInnerMargins.left, rcInnerMargins.top, rcInnerMargins.right, rcInnerMargins.bottom);

		if (bDrawIcon)
		{
			DrawItemImage(pDC, pItem, ptIcon, pImage, szIcon);
		}

		if (pItem->IsItemFocused() && pItem->GetTaskPanel()->IsDrawFocusRect())
		{
			DrawFocusRect(pDC, rc);
		}
		return 0;
	}
	else
	{
		int nHeight = szIcon.cy + rcInnerMargins.top + rcInnerMargins.bottom;
		int nWidth = szIcon.cx + rcInnerMargins.right + rcInnerMargins.left;

		return CRect(rc.CenterPoint().x - nWidth /2, rc.top, rc.CenterPoint().x - nWidth /2 + nWidth, rc.top + nHeight);
	}
}



void CXTPTaskPanelPaintManager::DrawGroupClientItems(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc)
{
	int nOffset = rc.top - pGroup->GetScrollOffsetPos();

	for (int i = pGroup->GetOffsetItem(); i < pGroup->GetItemCount(); i++)
	{
		CXTPTaskPanelGroupItem* pItem = pGroup->GetAt(i);
		if (!pItem->IsVisible())
			continue;

		CRect rcItem = pItem->GetItemRect();
		rcItem.OffsetRect(0, nOffset);

		pItem->OnDrawItem(pDC, rcItem);
	}
}

void CXTPTaskPanelPaintManager::FillGroupClientFace(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc)
{
	XTPDrawHelpers()->GradientFill(pDC, rc, GetGroupColors(pGroup)->clrClient, FALSE);
}

void CXTPTaskPanelPaintManager::DrawGroupClientFace(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc)
{
	XTP_TASKPANEL_GROUPCOLORS* pGroupColors = GetGroupColors(pGroup);

	CXTPPenDC pen(*pDC, pGroupColors->clrClientBorder);
	pDC->MoveTo(rc.TopLeft());
	pDC->LineTo(rc.left, rc.bottom - 1);
	if (m_bOfficeBorder) pDC->LineTo(rc.right, rc.bottom - 1); else pDC->LineTo(rc.right - 1, rc.bottom - 1);
	if (!m_bOfficeBorder) pDC->LineTo(rc.right - 1, rc.top - 1); else pDC->MoveTo(rc.right - 1, rc.top);
	if (m_bOfficeBorder) pDC->LineTo(rc.TopLeft());

	DrawGroupClientItems(pDC, pGroup, rc);
}

void CXTPTaskPanelPaintManager::DrawGroupClient(CDC* pDC, CXTPTaskPanelGroup* pGroup)
{
	CRect rc = pGroup->GetClientRect();
	if (rc.IsRectEmpty())
		return;

	int nExpandedClientHeight = pGroup->GetExpandedClientHeight();

	int nOffset = pGroup->IsExpanding() ? nExpandedClientHeight - rc.Height() : 0;

	CRect rcExpaded(rc);
	if (nOffset)
		rcExpaded.bottom = rcExpaded.top + nExpandedClientHeight;

	CDC dc;
	dc.CreateCompatibleDC(pDC);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, rc.right + 1, rcExpaded.Height());

	CGdiObject* pOldBitmap = dc.SelectObject(&bmp);

	dc.SetWindowOrg(0, rc.top);

	int nAlpha = pGroup->IsExpanding() ? (255 * rc.Height()) / max(1, nExpandedClientHeight) : 255;

	if (GetGroupColors(pGroup)->clrClient == COLORREF_TRANSPARENT)
	{
		dc.BitBlt(rc.left, rc.top + nOffset, rc.Width(), rc.Height(), pDC, rc.left, rc.top, SRCCOPY);
	}
	else
	{
		FillGroupClientFace(&dc, pGroup, rcExpaded);
	}
	pGroup->OnFillClient(&dc, rcExpaded);


	if (pGroup->IsExpanded() || pGroup->IsExpanding())
	{
		DrawGroupClientFace(&dc, pGroup, rcExpaded);
	}

	dc.SetWindowOrg(0, 0);

	if (nAlpha != 255)
	{
		if (!XTPDrawHelpers()->m_pfnAlphaBlend || (XTPDrawHelpers()->IsContextRTL(pDC) && XTPSystemVersion()->IsWin9x()))
		{
			pDC->BitBlt(rc.left, rc.top, rc.Width(), rcExpaded.Height() - nOffset, &dc, rc.left, nOffset, SRCCOPY);
		}
		else
		{
			BLENDFUNCTION bf = {AC_SRC_OVER, 0, (BYTE)nAlpha, 0};
			XTPDrawHelpers()->m_pfnAlphaBlend(pDC->GetSafeHdc(), rc.left, rc.top, rc.Width(), rc.Height(), dc, rc.left, nOffset, rc.Width(), rcExpaded.Height() - nOffset, bf);
		}
	}
	else
	{
		pDC->BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), &dc, rc.left, nOffset, SRCCOPY);
	}

	dc.SelectObject(pOldBitmap);

}

void CXTPTaskPanelPaintManager::DrawItemCaption(CDC* pDC, CXTPTaskPanelItem* pItem, CRect rcText, UINT nFormat)
{
	if (!pItem->IsGroup())
	{
		XTPTaskPanelHotTrackStyle hotTrackStyle = pItem->GetTaskPanel()->GetHotTrackStyle();

		CXTPTaskPanelGroup* pGroup = pItem->GetItemGroup();
		XTP_TASKPANEL_GROUPCOLORS* pGroupColors = GetGroupColors(pGroup);

		pDC->SetBkMode(TRANSPARENT);

		if (m_bEmbossedDisabledText && !pItem->GetEnabled())
		{
			pDC->SetTextColor(GetXtremeColor(COLOR_3DHIGHLIGHT));
			pItem->DrawCaptionText(pDC, rcText + CPoint(1, 1), nFormat);
		}
		COLORREF clrNormal = ((CXTPTaskPanelGroupItem*)pItem)->GetTextColor();
		if (clrNormal == COLORREF_NULL)
			clrNormal = pItem->GetType() == xtpTaskItemTypeLink ? pGroupColors->clrClientLink : pGroupColors->clrClientText;

		pDC->SetTextColor(!pItem->GetEnabled() ? GetXtremeColor(COLOR_GRAYTEXT) : pItem->GetType() == xtpTaskItemTypeLink ?
			(pItem->IsItemHot() && (hotTrackStyle == xtpTaskPanelHighlightText || pGroupColors->clrClientLinkHot.IsCustomValue()) ? pGroupColors->clrClientLinkHot :
			(hotTrackStyle == xtpTaskPanelHighlightItem) && (((CXTPTaskPanelGroupItem*)pItem)->IsItemSelected() || (pItem->IsItemHot())) ? pGroupColors->clrClientLinkSelected :
			clrNormal) : clrNormal);
	}

	pItem->DrawCaptionText(pDC, rcText, nFormat);
	pItem->SetTextRect(pDC, rcText, nFormat);
}

void CXTPTaskPanelPaintManager::DrawItemImage(CDC* pDC, CXTPTaskPanelItem* pItem, CPoint ptIcon, CXTPImageManagerIcon* pImage, CSize szIcon)
{
	BOOL bHighlight = pItem->IsItemHot() || pItem->IsItemPressed();
	BOOL bChecked = (!pItem->IsGroup() && ((CXTPTaskPanelGroupItem*)pItem)->IsItemSelected());

	pImage->Draw(pDC, ptIcon, !pItem->GetEnabled() ? pImage->GetDisabledIcon(): bHighlight ? pImage->GetHotIcon(): bChecked ? pImage->GetCheckedIcon() : pImage->GetIcon(), szIcon);
}

int CXTPTaskPanelPaintManager::DrawGroupCaption(CDC* pDC, CXTPTaskPanelGroup* pGroup, BOOL bDraw)
{
	if (bDraw)
	{
		CRect rc = pGroup->GetCaptionRect();

		if (rc.IsRectEmpty())
			return 0;

		XTP_TASKPANEL_GROUPCOLORS* pGroupColors = GetGroupColors(pGroup);

		if (pGroup->IsCaptionVisible())
		{
			HRGN hrgnClip = ::CreateRectRgn(0, 0, 0, 0);

			if (hrgnClip != NULL)
			{
				if (GetClipRgn(pDC->m_hDC, hrgnClip) != 1)
				{
					DeleteObject(hrgnClip);
					hrgnClip = (HRGN)-1;
				}
			}


			if (m_bLeftRoundedCorner)
			{
				pDC->ExcludeClipRect(rc.left, rc.top, rc.left + 2, rc.top + 1);
				pDC->ExcludeClipRect(rc.left, rc.top + 1, rc.left + 1, rc.top + 2);
			}

			if (m_bRightRoundedCorner)
			{
				pDC->ExcludeClipRect(rc.right, rc.top, rc.right - 2, rc.top + 1);
				pDC->ExcludeClipRect(rc.right, rc.top + 1, rc.right - 1, rc.top + 2);
			}

			pDC->FillSolidRect(rc, pGroupColors->clrHead.clrLight);
			CRect rcGradient(rc);
			rcGradient.left += int (rc.Width() * pGroupColors->dHeadGradientFactor);
			XTPDrawHelpers()->GradientFill(pDC, rcGradient, pGroupColors->clrHead, TRUE);

			if (pGroup->IsItemFocused() && pGroup->GetTaskPanel()->IsDrawFocusRect())
			{
				CRect rcFocus(rc);
				rcFocus.DeflateRect(2, 2);
				DrawFocusRect(pDC, rcFocus);
			}


			if (hrgnClip != NULL)
			{
				if (hrgnClip == (HRGN)-1)
				{
					ExtSelectClipRgn(pDC->m_hDC, NULL, RGN_COPY);
				}
				else
				{
					ExtSelectClipRgn(pDC->m_hDC, hrgnClip, RGN_COPY);
					DeleteObject(hrgnClip);
				}
			}


			CXTPFontDC fnt(pDC, GetItemFont(pGroup));

			CRect rcText = DrawCaptionGripper(pDC, pGroup, rc);

			CSize szIcon = pGroup->GetTaskPanel()->GetGroupIconSize();
			CXTPImageManagerIcon* pImage = pGroup->GetImage(szIcon.cx);

			if (rcText.Width() > szIcon.cx && pImage)
			{
				szIcon.cy = pImage->GetHeight() * szIcon.cx / pImage->GetWidth();

				CRect rcIconPadding(GetItemIconPadding(pGroup));
				CPoint ptIcon(rcText.left + rcIconPadding.left,
					min(rcText.CenterPoint().y + szIcon.cy/2, rcText.bottom) - szIcon.cy - rcIconPadding.bottom + rcIconPadding.top);

				DrawItemImage(pDC, pGroup, ptIcon, pImage, szIcon);

				rcText.left += szIcon.cx - 12 + rcIconPadding.right + rcIconPadding.left;
			}

			rcText.left += 13;
			rcText.bottom += 2;

			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(!pGroup->GetEnabled() ? GetXtremeColor(COLOR_GRAYTEXT) :
				pGroup->IsItemHot() ? pGroupColors->clrHeadTextHot : pGroupColors->clrHeadTextNormal);

			DrawItemCaption(pDC, pGroup, rcText, m_nCaptionTextFormat);

		}
		else
		{
			pDC->FillSolidRect(rc, pGroupColors->clrClientBorder);
		}
		if (pGroup->IsItemDragOver() && !(pGroup->IsExpanded() && pGroup->GetItems()->GetFirstVisibleItem() != NULL))
		{
			DrawDragArrow(pDC, pGroup, rc, TRUE);
		}
	}

	return pGroup->IsCaptionVisible() ? m_nCaptionHeight: m_bOfficeBorder ? 0 : 1;
}
