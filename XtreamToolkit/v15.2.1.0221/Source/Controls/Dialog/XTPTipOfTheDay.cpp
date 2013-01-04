// XTPTipOfTheDay.cpp : implementation of the CXTPTipOfTheDay class.
//
// This file is a part of the XTREME CONTROLS MFC class library.
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
#include <sys\stat.h>

#include "Common/XTPVC80Helpers.h"  // Visual Studio 2005 helper functions
#include "Common/XTPResourceManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPMarkupRender.h"
#include "Common/XTPImageManager.h"

#include "../Resource.h"
#include "XTPTipOfTheDay.h"
#include "../Util/XTPRegistryManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define MAX_BUFLEN 1000

static const TCHAR szSection[] = _T("Tip");
static const TCHAR szIntFilePos[] = _T("FilePos");
static const TCHAR szTimeStamp[] = _T("TimeStampLong");
static const TCHAR szIntStartup[] = _T("StartUp");
/////////////////////////////////////////////////////////////////////////////
// CXTPTipOfTheDay dialog
/////////////////////////////////////////////////////////////////////////////
CXTPTipOfTheDay::CXTPTipOfTheDay(LPCTSTR lpszTipFile/*= NULL*/, CWnd* pParent/*= NULL*/)
{
	InitModalIndirect(XTPResourceManager()->LoadDialogTemplate(IDD), pParent);

	//{{AFX_DATA_INIT(CXTPTipOfTheDay)
	m_bStartup = TRUE;
	//}}AFX_DATA_INIT


	SetTipsFilePath(lpszTipFile);
	CXTPResourceManager::AssertValid(XTPResourceManager()->LoadString(&m_strTipTitle, XTP_IDS_DIDYOUKNOW));

	m_strTipText = _T("");
	m_pFontTitle = NULL;
	m_pFontTip = NULL;
	m_nNextTip = 0;

	m_pMarkupContext = NULL;
	m_pMarkupTipText = NULL;

	m_bClassicStyle = FALSE;

	m_pLightBulbIcon = 0;
	{
		CXTPResourceManager::CManageState state;

		CXTPImageManagerIconHandle iconHandle;
		iconHandle = CXTPImageManagerIcon::LoadBitmapFromResource(MAKEINTRESOURCE(XTP_IDI_LIGHTBULB), NULL); // Destructor will free resource

		XTPResourceManager()->AssertValid(!iconHandle.IsEmpty());

		if (!iconHandle.IsEmpty())
		{
			m_pLightBulbIcon = new CXTPImageManagerIcon(0, iconHandle.GetExtent().cx, iconHandle.GetExtent().cy);
			m_pLightBulbIcon->SetNormalIcon(iconHandle);
		}
	}

	// We need to find out what the startup and file position parameters are
	// If startup does not exist, we assume that the Tips on startup is checked TRUE.
	CXTPRegistryManager regManager;
	m_bStartup = !regManager.GetProfileInt(szSection, szIntStartup, 0);

	m_nNextTip = regManager.GetProfileInt(szSection, szIntFilePos, 0);

	// Now try to open the tips file
	m_pStream = m_strTipFile.IsEmpty() ? NULL : FOPEN_S((TCHAR*)(LPCTSTR)m_strTipFile, _T("r"));

	if (m_pStream != NULL)
	{
		// If the timestamp in the INI file is different from the timestamp of
		// the tips file, then we know that the tips file has been modified
		// Reset the file position to 0 and write the latest timestamp to the
		// ini file
		struct _stat buf;
		_fstat(_fileno(m_pStream), &buf);

		int nCurrentTime = (int)(buf.st_ctime);

		int nStoredTime = (int)regManager.GetProfileInt(szSection, szTimeStamp, NULL);

		if (nCurrentTime != nStoredTime)
		{
			m_nNextTip = 0;
			regManager.WriteProfileInt(szSection, szTimeStamp, nCurrentTime);
		}
	}
}

CXTPTipOfTheDay::~CXTPTipOfTheDay()
{
	// This destructor is executed whether the user had pressed the escape key
	// or clicked on the close button. If the user had pressed the escape key,
	// it is still required to update the filepos in the ini file with the
	// latest position so that we don't repeat the tips!

	CXTPRegistryManager regManager;
	regManager.WriteProfileInt(szSection, szIntFilePos, m_nNextTip);

	// But make sure the tips file existed in the first place....
	if (m_pStream != NULL)
	{
		fclose(m_pStream);
		m_pStream = NULL;
	}

	if (m_fontTitle.GetSafeHandle())
		m_fontTitle.DeleteObject();

	if (m_fontTip.GetSafeHandle())
		m_fontTip.DeleteObject();

	XTPMarkupReleaseElement(m_pMarkupTipText);
	XTPMarkupReleaseContext(m_pMarkupContext);

	SAFE_DELETE(m_pLightBulbIcon);
}

void CXTPTipOfTheDay::EnableMarkup(BOOL bEnable)
{
	XTPMarkupReleaseContext(m_pMarkupContext);

	if (bEnable)
	{
		m_pMarkupContext = XTPMarkupCreateContext(m_hWnd);
	}
}

void CXTPTipOfTheDay::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXTPTipOfTheDay)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, XTP_IDC_CHK_DAYTIP_SHOW, m_showTips);
	DDX_Control(pDX, XTP_IDC_BTN_DAYTIP_NEXT, m_btnNextTip);
	DDX_Control(pDX, XTP_IDC_BTN_DAYTIP_BORDER, m_staticBorder);
	DDX_Check(pDX, XTP_IDC_CHK_DAYTIP_SHOW, m_bStartup);
	//}}AFX_DATA_MAP
}

IMPLEMENT_DYNAMIC(CXTPTipOfTheDay, CDialog)

BEGIN_MESSAGE_MAP(CXTPTipOfTheDay, CDialog)
	//{{AFX_MSG_MAP(CXTPTipOfTheDay)
	ON_WM_PAINT()
	ON_BN_CLICKED(XTP_IDC_BTN_DAYTIP_NEXT, OnDaytipNext)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CXTPTipOfTheDay::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	dc.SetBkMode(TRANSPARENT);

	if (m_bClassicStyle)
	{
		CRect rcBorder(m_rcBorder);
		rcBorder.DeflateRect(10, 10, 10, 0);

		dc.FillSolidRect(rcBorder.left + 50, rcBorder.top, rcBorder.Width() - 50, rcBorder.Height(), GetXtremeColor(COLOR_WINDOW));
		dc.FillSolidRect(rcBorder.left, rcBorder.top, 50, rcBorder.Height(), RGB(128, 128, 128));

		dc.Draw3dRect(rcBorder, GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DHIGHLIGHT));

		// Draw the light bulb bitmap to the screen.
		if (m_pLightBulbIcon) m_pLightBulbIcon->Draw(&dc, CPoint(rcBorder.left + 8, rcBorder.top + 8));

		// Draw the title text.
		CXTPFontDC font(&dc, m_pFontTitle);
		dc.SetTextColor(GetXtremeColor(COLOR_WINDOWTEXT));
		dc.TextOut(52 + 10 + 5, 14 + 5, m_strTipTitle);

	}
	else
	{
		dc.FillSolidRect(m_rcBorder, GetXtremeColor(COLOR_WINDOW));
		dc.FillSolidRect(m_rcBorder.left, m_rcBorder.bottom, m_rcBorder.Width(), 1, GetXtremeColor(COLOR_3DSHADOW));

		// Draw the light bulb bitmap to the screen.
		if (m_pLightBulbIcon) m_pLightBulbIcon->Draw(&dc, CPoint(8, 8));

		// Draw the title text.
		CXTPFontDC font(&dc, m_pFontTitle);
		dc.SetTextColor(RGB(0x00, 0x33, 0x99));
		dc.TextOut(52, 14, m_strTipTitle);

		dc.SetTextColor(GetXtremeColor(COLOR_WINDOWTEXT));
	}

	if (m_pMarkupTipText)
	{
		XTPMarkupSetDefaultFont(m_pMarkupContext, (HFONT)m_pFontTip->GetSafeHandle(), dc.GetTextColor());
		XTPMarkupMeasureElement(m_pMarkupTipText, m_rcTipText.Width(), m_rcTipText.Height());
		XTPMarkupRenderElement(m_pMarkupTipText, dc, m_rcTipText);
	}
	else
	{
		// Draw the tip of the day text.
		CXTPFontDC font(&dc,m_pFontTip);
		dc.DrawText(m_strTipText, m_rcTipText, DT_END_ELLIPSIS | DT_WORDBREAK);
	}

}

BOOL CXTPTipOfTheDay::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (m_pStream)
	{
		if (fseek(m_pStream, m_nNextTip, SEEK_SET) != 0)
		{
			fseek(m_pStream, 0, SEEK_SET);
		}
	}

	if (m_pMarkupContext)
	{
		XTPMarkupAssignHandle(m_pMarkupContext, m_hWnd);
	}

	CString strTipText;
	GetNextTipString(strTipText);
	SetTipText(strTipText);


	// Set font title if it has not been already set.
	if (m_pFontTitle == NULL && m_pFontTip == NULL)
	{
		CWindowDC dc(NULL);

		NONCLIENTMETRICS ncm;
		ncm.cbSize = sizeof(NONCLIENTMETRICS);
		::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, &ncm, 0);
		ncm.lfMenuFont.lfCharSet = XTPResourceManager()->GetFontCharset();

		if (_tcscmp(ncm.lfMenuFont.lfFaceName, _T("Segoe UI")) == 0)
			ncm.lfMenuFont.lfQuality = 5;

		ncm.lfMenuFont.lfWeight = FW_NORMAL;
		ncm.lfMenuFont.lfItalic = 0;
		m_fontTip.CreateFontIndirect(&ncm.lfMenuFont);

		ncm.lfMenuFont.lfHeight = ::MulDiv(-12, ::GetDeviceCaps(dc.m_hDC, LOGPIXELSY), 72);

		if (_tcscmp(ncm.lfMenuFont.lfFaceName, _T("Segoe UI")) != 0)
			ncm.lfMenuFont.lfWeight = FW_BOLD;
		m_fontTitle.CreateFontIndirect(&ncm.lfMenuFont);

		m_pFontTip = &m_fontTip;
		m_pFontTitle = &m_fontTitle;
	}

	// If Tips file does not exist then disable NextTip
	m_btnNextTip.EnableWindow(m_pStream != NULL || m_arrTips.GetSize() > 0);

	// Get the border size, this will be the base for our
	// painting routines.

	CRect rcBorder;
	m_staticBorder.GetWindowRect(&rcBorder);
	ScreenToClient(&rcBorder);

	m_rcBorder = CXTPClientRect(this);
	m_rcBorder.bottom = rcBorder.bottom - 1;

	if (!m_bClassicStyle)
		m_rcTipText.SetRect(52, 44, m_rcBorder.right - 6, m_rcBorder.bottom - 6);
	else
		m_rcTipText.SetRect(52 + 10 + 5, 44 + 5, m_rcBorder.right - 6 - 10, m_rcBorder.bottom - 6);

	return TRUE;
}

void CXTPTipOfTheDay::SetDefaultFonts(CFont* pFontTitle, CFont* pFontTip)
{
	m_pFontTitle = pFontTitle;
	m_pFontTip = pFontTip;
}

void CXTPTipOfTheDay::GetNextTipString(CString& strNext)
{
	if (!m_pStream)
	{
		if (m_arrTips.GetSize() > 0)
		{
			strNext = m_arrTips[m_nNextTip % m_arrTips.GetSize()];

			m_nNextTip++;
			return;
		}

		CXTPResourceManager::AssertValid(XTPResourceManager()->LoadString(&strNext, XTP_IDS_FILE_ABSENT));
		return;
	}


	LPTSTR lpsz = strNext.GetBuffer(MAX_BUFLEN);

	// This routine identifies the next string that needs to be
	// read from the tips file
	BOOL bStop = FALSE;
	while (!bStop)
	{
		if (_fgetts(lpsz, MAX_BUFLEN, m_pStream) == NULL)
		{
			// We have either reached EOF or encountered some problem
			// In both cases reset the pointer to the beginning of the file
			// This behavior is same as VC++ Tips file
			if (fseek(m_pStream, 0, SEEK_SET) != 0)
				XTPResourceManager()->ShowMessageBox(XTP_IDS_FILE_CORRUPT);
		}
		else
		{
			if (*lpsz != ' ' && *lpsz != '\t' &&
				*lpsz != '\n' && *lpsz != ';')
			{
				// There should be no space at the beginning of the tip
				// This behavior is same as VC++ Tips file
				// Comment lines are ignored and they start with a semicolon
				bStop = TRUE;
			}
		}
	}
	strNext.ReleaseBuffer();

	m_nNextTip = ftell(m_pStream);
}

void CXTPTipOfTheDay::OnOK()
{
	// Update the startup information stored in the INI file
	UpdateData();
	CXTPRegistryManager regManager;
	regManager.WriteProfileInt(szSection, szIntStartup, !m_bStartup);

	CDialog::OnOK();
}

void CXTPTipOfTheDay::OnDaytipNext()
{
	UpdateData();

	CString strTipText;
	GetNextTipString(strTipText);
	SetTipText(strTipText);

	Invalidate(FALSE);
	UpdateData(FALSE);
}

void CXTPTipOfTheDay::SetTipText(LPCTSTR lpszText)
{
	m_strTipText = lpszText;

	XTPMarkupReleaseElement(m_pMarkupTipText);

	if (m_pMarkupContext)
	{
		m_pMarkupTipText = XTPMarkupParseText(m_pMarkupContext, m_strTipText);
	}
}

void CXTPTipOfTheDay::SetTipsFilePath(LPCTSTR lpszTipFile)
{
	TCHAR* lpszNextTip = lpszTipFile ? (TCHAR*)_tcschr(lpszTipFile, _T('\n')) : NULL;
	if (lpszNextTip != NULL)
	{
		while (lpszNextTip != NULL)
		{
			m_arrTips.Add(CString(lpszTipFile, int(lpszNextTip - lpszTipFile)));

			lpszTipFile = lpszNextTip + 1;
			lpszNextTip = (TCHAR*)_tcschr(lpszTipFile, _T('\n'));
		}

		m_strTipFile.Empty();
		return;
	}

	CFileFind file;
	if (lpszTipFile && file.FindFile(lpszTipFile))
	{
		m_strTipFile = lpszTipFile;
	}
	else
	{
		m_strTipFile = _T("tips.txt");
	}
}

BOOL CXTPTipOfTheDay::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// To Handle Hyperlinks:
	if (m_pMarkupTipText)
	{
		if (XTPMarkupRelayMessage(m_pMarkupTipText, message, wParam, lParam, pResult))
			return TRUE;
	}

	return CDialog::OnWndMsg(message, wParam, lParam, pResult);
}
