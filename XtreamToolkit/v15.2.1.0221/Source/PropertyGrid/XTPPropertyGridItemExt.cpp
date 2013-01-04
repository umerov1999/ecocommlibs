// XTPPropertyGridItemExt.cpp
//
// This file is a part of the XTREME PROPERTYGRID MFC class library.
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

#include "Common/XTPVC80Helpers.h"

#include "Common/XTPResourceManager.h"
#include "Common/XTPSystemHelpers.h"

#include "XTPPropertyGridDefines.h"
#include "XTPPropertyGridInplaceList.h"
#include "XTPPropertyGridInplaceButton.h"
#include "XTPPropertyGridItem.h"
#include "XTPPropertyGridItemExt.h"
#include "XTPPropertyGridView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CXTPPropertyGridInplaceMonthCal, CWnd)
	ON_NOTIFY_REFLECT(MCN_SELECT, OnSelect)
END_MESSAGE_MAP()

void CXTPPropertyGridInplaceMonthCal::OnSelect(NMHDR*, LRESULT*)
{
	OnAccept();
}

void CXTPPropertyGridInplaceMonthCal::OnCancel()
{
	if (m_pItem)
	{
		m_pItem->OnCancelEdit();

		DestroyWindow();
	}
}

void CXTPPropertyGridInplaceMonthCal::OnAccept()
{
	SYSTEMTIME sysTime;
	::SendMessage(m_hWnd, MCM_GETCURSEL, 0, (LPARAM) &sysTime);

	sysTime.wHour = sysTime.wMinute = sysTime.wSecond = sysTime.wMilliseconds = 0;

	COleDateTime dtSelected(sysTime);

	CString str = m_pItem->Format(dtSelected);
	CString strBeforeEdit = str;

	CXTPPropertyGridItemDate* pItem = m_pItem;
	m_pItem = NULL;

	ShowWindow(SW_HIDE);
	ReleaseCapture();
	PostMessage(WM_CLOSE);

	if (pItem->OnAfterEdit(str))
	{
		if (strBeforeEdit == str) // Can changed in OnAfterEdit handler
		{
			pItem->SetDate(dtSelected);
			pItem->NotifyValueChanged();
		}
		else
		{
			pItem->OnValueChanged(str);
		}
	}
}

void CXTPPropertyGridInplaceMonthCal::PostNcDestroy()
{
	delete this;
}

BOOL CXTPPropertyGridInplaceMonthCal::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if (message == WM_KILLFOCUS)
	{
		CWnd* pWnd = CWnd::FromHandle((HWND)wParam);
		if (pWnd && IsChild(pWnd))
		{
			return CWnd::OnWndMsg(message, wParam, lParam, pResult);
		}

		OnCancel();
		return TRUE;
	}
	if (message == WM_KEYDOWN && wParam == VK_ESCAPE)
	{
		OnCancel();
		return TRUE;
	}
	if (message == WM_KEYDOWN && wParam == VK_RETURN)
	{
		OnAccept();
		return TRUE;
	}

	return CWnd::OnWndMsg(message, wParam, lParam, pResult);
}

IMPLEMENT_DYNAMIC(CXTPPropertyGridItemDate, CXTPPropertyGridItem)

CXTPPropertyGridItemDate::CXTPPropertyGridItemDate(LPCTSTR strCaption, const COleDateTime& oleDate, COleDateTime* pBindDate)
	: CXTPPropertyGridItem(strCaption)
{
	m_pBindDate = pBindDate;
	Init(oleDate);
}

CXTPPropertyGridItemDate::CXTPPropertyGridItemDate(UINT nID, const COleDateTime& oleDate, COleDateTime* pBindDate)
	: CXTPPropertyGridItem(nID)
{
	m_pBindDate = pBindDate;
	Init(oleDate);
}

CString CXTPPropertyGridItemDate::GetAllowNullDateString() const
{
	return m_strNullValue;
}

void CXTPPropertyGridItemDate::Init(const COleDateTime& oleDate)
{
	m_nFlags = xtpGridItemHasComboButton | xtpGridItemHasEdit;
	m_strFormat = _T("%d/%m/%Y");
	m_strValue = _T("00/00/0000");
	m_strNullValue.Empty();

	SetMask(_T("00/00/0000"), _T("__/__/____"));

	SetDate(oleDate);

	EnableAutomation();
	m_strDefaultValue = m_strValue;
}

BOOL CXTPPropertyGridItemDate::ParseDateTime(COleDateTime& dt, LPCTSTR strValue)
{
	if (m_strFormat.IsEmpty())
		return FALSE;

	SYSTEMTIME sysTime;
	ZeroMemory(&sysTime, sizeof(SYSTEMTIME));

	WORD* ptrDate[3] = {0, 0, 0};
	int nResult[3] = {0, 0, 0};

	CString strFormat(m_strFormat);
	strFormat.MakeLower();

	int nIndex = -1, i = 0;

	for (i = 0; i < 3; i++)
	{
		nIndex = strFormat.Find(_T('%'), nIndex + 1);

		if (nIndex == -1 || nIndex == strFormat.GetLength() - 1)
			return FALSE;

		switch (strFormat[nIndex + 1])
		{
			case _T('d'):
				ptrDate[i] = &sysTime.wDay;
				break;

			case _T('y'):
				ptrDate[i] = &sysTime.wYear;
				break;

			case _T('m'):
			case _T('b'):
				ptrDate[i] = &sysTime.wMonth;
				break;

			default:
				return FALSE;
		}

		strFormat.SetAt(nIndex + 1, _T('d'));
	}

	if (SCANF_S(strValue, strFormat, &nResult[0], &nResult[1], &nResult[2]) != 3)
		return FALSE;

	for (i = 0; i < 3; i++)
	{
		if (!ptrDate[i])
			return FALSE;

		*ptrDate[i] = (WORD)nResult[i];
	}

	dt = sysTime;
	return dt.GetStatus() == COleDateTime::valid;
}

void CXTPPropertyGridItemDate::OnValueChanged(CString strValue)
{
	if (!m_strNullValue.IsEmpty() && m_bUseMask && strValue == m_strLiteral && m_oleDate.GetStatus() == COleDateTime::null)
		return;

	CXTPPropertyGridItem::OnValueChanged(strValue);
}

void CXTPPropertyGridItemDate::SetValue(CString strValue)
{
	COleDateTime dt;
	TRY
	{
		if (ParseDateTime(dt, strValue))
		{
			SetDate(dt);
		}
		else if (dt.ParseDateTime(strValue, VAR_DATEVALUEONLY))
		{
			SetDate(dt);
		}
		else if (!m_strNullValue.IsEmpty())
		{
			COleDateTime dtNull;
			dtNull.SetStatus(COleDateTime::null);

			SetDate(dtNull);
		}
	}
	CATCH(COleException, e)
	{

	}
	END_CATCH
}


void CXTPPropertyGridItemDate::SetDateFormat(LPCTSTR strFormat)
{
	m_strFormat = strFormat;
	SetDate(m_oleDate);
}

CString CXTPPropertyGridItemDate::Format(const COleDateTime& oleDate)
{
	if (oleDate.GetStatus() != COleDateTime::valid)
		return _T("");

#if _MSC_VER > 1310
	if (oleDate.GetYear() <= 1900) // Visual Studio 2005 bug.
	{
		CString strDate;
		if (m_strFormat == _T("%d/%m/%Y"))
		{
			strDate.Format(_T("%.2i/%.2i/%.4i"), oleDate.GetDay(), oleDate.GetMonth(), oleDate.GetYear());
			return strDate;
		}
		if (m_strFormat == _T("%m/%d/%Y"))
		{
			strDate.Format(_T("%.2i/%.2i/%.4i"), oleDate.GetMonth(), oleDate.GetDay(), oleDate.GetYear());
			return strDate;
		}
	}
#endif
	if (m_strFormat.IsEmpty())
		return oleDate.Format();

	return oleDate.Format(m_strFormat);
}

void CXTPPropertyGridItemDate::SetDate(const COleDateTime& oleDate)
{
	if (oleDate.GetStatus() != COleDateTime::valid && !m_strNullValue.IsEmpty())
	{
		m_oleDate = oleDate;
		if (m_pBindDate)
			*m_pBindDate = oleDate;

		CXTPPropertyGridItem::SetValue(m_strNullValue);
		return;
	}

	ASSERT(oleDate.GetStatus() == COleDateTime::valid);
	m_oleDate = oleDate;

	if (m_pBindDate)
		*m_pBindDate = oleDate;

	CString strValue = Format(oleDate);

	CXTPPropertyGridItem::SetValue(strValue);
}

void CXTPPropertyGridItemDate::BindToDate(COleDateTime* pBindDate)
{
	m_pBindDate = pBindDate;
	if (m_pBindDate)
		*m_pBindDate = m_oleDate;
}

void CXTPPropertyGridItemDate::OnBeforeInsert()
{
	if (m_pBindDate && *m_pBindDate != m_oleDate)
		SetDate(*m_pBindDate);
}


#if _MSC_VER < 1200 // MFC 5.0


BOOL CXTPPropertyGridItemDate::GetAsSystemTime(SYSTEMTIME& sysTime)
{
	BOOL bRetVal = FALSE;
	if (m_oleDate.GetStatus() == COleDateTime::valid)
	{
		struct tm tmTemp;
		if (CXTPDateTimeHelper::TmFromOleDate(m_oleDate, tmTemp))
		{
			sysTime.wYear = (WORD) tmTemp.tm_year;
			sysTime.wMonth = (WORD) tmTemp.tm_mon;
			sysTime.wDayOfWeek = (WORD) tmTemp.tm_wday;
			sysTime.wDay = (WORD) tmTemp.tm_mday;
			sysTime.wMinute = sysTime.wMilliseconds = sysTime.wSecond = sysTime.wHour = 0;

			bRetVal = TRUE;
		}
	}

	return bRetVal;
}
#else

BOOL CXTPPropertyGridItemDate::GetAsSystemTime(SYSTEMTIME& sysTime)
{
	BOOL bRetVal = FALSE;
	if (m_oleDate.GetStatus() == COleDateTime::valid)
	{
		bRetVal = m_oleDate.GetAsSystemTime(sysTime);
		sysTime.wMinute = sysTime.wMilliseconds = sysTime.wSecond = sysTime.wHour = 0;
	}

	return bRetVal;
}
#endif

void CXTPPropertyGridItemDate::OnInplaceButtonDown(CXTPPropertyGridInplaceButton* pButton)
{
	if (m_pGrid->SendNotifyMessage(XTP_PGN_INPLACEBUTTONDOWN, (LPARAM)pButton) == TRUE)
		return;

	if (!OnRequestEdit())
		return;

	CXTPPropertyGridInplaceMonthCal* pMonthCtrl = new CXTPPropertyGridInplaceMonthCal(this);


#if _MSC_VER < 1200 // MFC 5.0
	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(icex);
	icex.dwICC = ICC_DATE_CLASSES;
	VERIFY(InitCommonControlsEx(&icex));
#else
	VERIFY(AfxDeferRegisterClass(AFX_WNDCOMMCTL_DATE_REG));
#endif // _MSC_VER < 1200


	CRect rect(0, 0, 0, 0);
	pMonthCtrl->CreateEx(WS_EX_TOPMOST | (m_pGrid->GetExStyle() & WS_EX_LAYOUTRTL), MONTHCAL_CLASS, NULL, WS_BORDER | WS_POPUP | WS_VISIBLE, rect, m_pGrid, 0);

	SYSTEMTIME sysTime;
	if (GetAsSystemTime(sysTime))
		pMonthCtrl->SendMessage(MCM_SETCURSEL, 0, (LPARAM) &sysTime);

	if (pMonthCtrl->GetMinReqRect(rect))
	{
		CRect rcItem = GetItemRect();
		rect.SetRect(rcItem.right - rect.Width(), rcItem.bottom, rcItem.right, rcItem.bottom + rect.Height());

		m_pGrid->ClientToScreen(&rect);

		CRect rcWork = XTPMultiMonitor()->GetWorkArea(rect);
		if (rect.bottom > rcWork.bottom && rect.top > rcWork.CenterPoint().y)
			rect.OffsetRect(0, - rect.Height() - rcItem.Height() - 1);

		if (rect.left < rcWork.left)
			rect.OffsetRect(rcWork.left - rect.left, 0);

		if (rect.right > rcWork.right)
			rect.OffsetRect(rcWork.right - rect.right, 0);

		pMonthCtrl->SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
	}


	pMonthCtrl->SetOwner(m_pGrid);
	pMonthCtrl->SetFocus();
}

long CXTPPropertyGridItemDate::GetDay()
{
	return m_oleDate.GetDay();
}

void CXTPPropertyGridItemDate::SetDay(long nDay)
{
	COleDateTime oleDate(GetYear(), GetMonth(), nDay, 0, 0, 0);
	SetDate(oleDate);
}

long CXTPPropertyGridItemDate::GetMonth()
{
	return m_oleDate.GetMonth();
}

void CXTPPropertyGridItemDate::SetMonth(long nMonth)
{
	COleDateTime oleDate(GetYear(), nMonth, GetDay(), 0, 0, 0);
	SetDate(oleDate);
}

long CXTPPropertyGridItemDate::GetYear()
{
	return m_oleDate.GetYear();
}

void CXTPPropertyGridItemDate::SetYear(long nYear)
{
	COleDateTime oleDate(nYear, GetMonth(), GetDay(), 0, 0, 0);
	SetDate(oleDate);
}

void CXTPPropertyGridItemDate::AllowNullDate(LPCTSTR lpszNullValue)
{
	m_strNullValue = lpszNullValue;
}


//////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridItemMultilineString

CXTPPropertyGridItemMultilineString::CXTPPropertyGridItemMultilineString(LPCTSTR lpszCaption, LPCTSTR strValue, CString* pBindString)
	: CXTPPropertyGridItem(lpszCaption, strValue, pBindString)
{
	m_nFlags = xtpGridItemHasComboButton | xtpGridItemHasEdit;
	m_nDropDownItemCount = 8;
}

CXTPPropertyGridItemMultilineString::CXTPPropertyGridItemMultilineString(UINT nID, LPCTSTR strValue, CString* pBindString)
	: CXTPPropertyGridItem(nID, strValue, pBindString)
{
	m_nFlags = xtpGridItemHasComboButton | xtpGridItemHasEdit;
	m_nDropDownItemCount = 8;
}

void CXTPPropertyGridItemMultilineString::OnInplaceButtonDown(CXTPPropertyGridInplaceButton* pButton)
{
	if (!IsAllowEdit())
		return;

	if (m_pGrid->SendNotifyMessage(XTP_PGN_INPLACEBUTTONDOWN, (LPARAM)pButton) == TRUE)
		return;

	if ((pButton->GetID() == XTP_ID_PROPERTYGRID_COMBOBUTTON) && OnRequestEdit())
	{
		CXTPPropertyGridInplaceMultilineEdit* pEdit = new CXTPPropertyGridInplaceMultilineEdit();
		pEdit->Create(this, GetItemRect());
		m_pGrid->Invalidate(FALSE);
	}
}



#ifdef __AFXCTL_H__

//////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridItemPicture

CXTPPropertyGridItemPicture::CXTPPropertyGridItemPicture(LPCTSTR strCaption)
	: CXTPPropertyGridItem(strCaption)
{
	m_nFlags = xtpGridItemHasExpandButton;
	m_nPreviewWidth = 20;
}

BOOL CXTPPropertyGridItemPicture::OnDrawItemValue(CDC& dc, CRect rcValue)
{
	COLORREF clr = dc.GetTextColor();
	CRect rcSample(rcValue.left - 2, rcValue.top + 1, rcValue.left - 2 + m_nPreviewWidth, rcValue.bottom - 1);

	m_olePicture.Render(&dc, rcSample, rcSample);
	dc.Draw3dRect(rcSample, clr, clr);

	CRect rcText(rcValue);
	rcText.left += m_nPreviewWidth + 5;

	short type = m_olePicture.GetType();
	CString str = type == PICTYPE_ICON ? _T("(Icon)") :
		type == PICTYPE_BITMAP ? _T("(Bitmap)") : _T("None");

	dc.DrawText(str, rcText, DT_SINGLELINE | DT_VCENTER);

	return TRUE;
}

void CXTPPropertyGridItemPicture::OnInplaceButtonDown(CXTPPropertyGridInplaceButton* pButton)
{
	if (m_pGrid->SendNotifyMessage(XTP_PGN_INPLACEBUTTONDOWN, (LPARAM)pButton) == TRUE)
		return;

	if (!OnRequestEdit())
		return;

	CString strFilter = _T("All Picture Types|*.bmp;*.cur;*.dib;*.emf;*.ico;*.wmf;*.gif;*.jpg|Bitmaps (*.bmp;*.dib;*.gif;*.jpg)|*.bmp;*.dib;*.gif;*.jpg |Icons/Cursors (*.ico;*.cur)|*.ico;*.cur|Metafiles (*.wmf;*.emf)|*.wmf;*.emf|All files (*.*)|*.*||");

	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, strFilter);

	if (dlg.DoModal() == IDOK)
	{
		CString strPath(dlg.GetPathName());

		if (OnAfterEdit(strPath))
		{
			SetPicturePath(strPath);
		}
	}
	else
	{
		OnCancelEdit();
	}
}

void CXTPPropertyGridItemPicture::SetPicturePath(LPCTSTR lpszPath)
{
	m_strPicturePath = lpszPath;
	LPPICTUREDISP pPict = NULL;

	if (OleLoadPicturePath((LPOLESTR)XTP_CT2CW(m_strPicturePath), NULL, 0, 0, IID_IPictureDisp, (LPVOID*)&pPict) == S_OK)
	{
		m_olePicture.SetPictureDispatch(pPict);
		pPict->Release();

		OnValueChanged(_T(""));
		((CWnd*)m_pGrid)->Invalidate(FALSE);
	}
	else if (m_olePicture.GetPictureDispatch())
	{
		m_olePicture.SetPictureDispatch(NULL);

		OnValueChanged(_T(""));
		((CWnd*)m_pGrid)->Invalidate(FALSE);
	}
}



#endif
