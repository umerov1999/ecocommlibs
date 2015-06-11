// XTPChartDrawThread.cpp
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

#include "stdafx.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPSystemHelpers.h"

#include "../Types/XTPChartTypes.h"
#include "../XTPChartElement.h"
#include <Chart/XTPChartLegendItem.h>
#include "../XTPChartContent.h"
#include "../XTPChartControl.h"

#include "XTPChartDeviceCommand.h"
#include "XTPChartDeviceContext.h"
#include "XTPChartDrawThread.h"


//////////////////////////////////////////////////////////////////////////
// CXTPChartDrawThreadDeviceCommand

CXTPChartDrawThreadDeviceCommand::CXTPChartDrawThreadDeviceCommand(CXTPChartDeviceCommand* pCommand)
{
	AddChildCommand(pCommand);
	m_dwRef = 1;
}

CXTPChartDrawThreadDeviceCommand::~CXTPChartDrawThreadDeviceCommand()
{
}


void CXTPChartDrawThreadDeviceCommand::AddRef()
{
	InterlockedIncrement(&m_dwRef);
}
void CXTPChartDrawThreadDeviceCommand::Release()
{
	if (m_dwRef == 0)
		return;

	LONG lResult = InterlockedDecrement(&m_dwRef);
	if (lResult == 0)
	{
		delete this;
	}
}




//////////////////////////////////////////////////////////////////////////
// CXTPChartDrawThread

IMPLEMENT_DYNCREATE(CXTPChartDrawThread, CWinThread)

CXTPChartDrawThread::CXTPChartDrawThread()
{
	m_pCommand = NULL;
	m_pControl = NULL;

	m_szImage = CSize(0, 0);
	m_bImageReady = FALSE;

	::InitializeCriticalSection(&m_cs);

	m_dwMonitorEvents[0] = CreateEvent(NULL, TRUE, FALSE, 0);   // Path was changed event.
	m_dwMonitorEvents[1] = CreateEvent(NULL, TRUE, FALSE, 0);   // Stop notifications event.
}

CXTPChartDrawThread::~CXTPChartDrawThread()
{
	CloseHandle(m_dwMonitorEvents[0]);
	CloseHandle(m_dwMonitorEvents[1]);

	SAFE_RELEASE(m_pCommand);

	::DeleteCriticalSection(&m_cs);
}

void CXTPChartDrawThread::StopNotifications()
{
	SetEvent(m_dwMonitorEvents[1]);
	WaitForSingleObject(m_hThread, INFINITE);
}

void CXTPChartDrawThread::StartDraw(CSize sz, CXTPChartDrawThreadDeviceCommand* pCommand)
{
	CXTPLockGuard lock(m_cs);

	ASSERT(pCommand);

	m_bImageReady = FALSE;
	m_szImage = sz;

	SAFE_RELEASE(m_pCommand);

	m_pCommand = pCommand;
	m_pCommand->AddRef();

	SetEvent(m_dwMonitorEvents[0]);
}

BOOL CXTPChartDrawThread::MonitorNotifications()
{
	BOOL bContinueThread = TRUE;


	BOOL bConinueNotifications = TRUE;

	while (bConinueNotifications)
	{
		// Wait for notification.

		DWORD dwWaitStatus = ::WaitForMultipleObjects(_countof(m_dwMonitorEvents),
			m_dwMonitorEvents, FALSE, INFINITE);

		switch (dwWaitStatus)
		{
		case WAIT_OBJECT_0:
			::ResetEvent(m_dwMonitorEvents[0]);
			DrawContent();
			break;

		case WAIT_OBJECT_0 + 1:
			bContinueThread = FALSE;
			bConinueNotifications = FALSE;
			break;

		}
	}
	return bContinueThread;
}

BOOL CXTPChartDrawThread::InitInstance()
{
	return TRUE;
}

int CXTPChartDrawThread::Run()
{
	BOOL bContinueThread = TRUE;

	while (bContinueThread)
	{
		bContinueThread = MonitorNotifications();
	}

	return 0;
}

void CXTPChartDrawThread::DrawContent()
{
	CXTPLockGuard lock(m_cs);

	CXTPChartDrawThreadDeviceCommand* pCommand = m_pCommand;
	m_pCommand = NULL;

	lock.UnLockThread();

	CDC memDC;
	memDC.CreateCompatibleDC(NULL);

	m_bmpCache.DeleteObject();
	m_bmpCache.Attach(CXTPImageManager::Create32BPPDIBSection(0, m_szImage.cx, m_szImage.cy, 0));

	CBitmap* pOldBitmap = memDC.SelectObject(&m_bmpCache);

	{
		CXTPChartDeviceContext* pDC = m_pControl->GetContent()->CreateDeviceContext(m_pControl, memDC, CRect(0, 0, m_szImage.cx, m_szImage.cy), FALSE);
		pDC->Execute(pCommand);

		delete pDC;
	}

	memDC.SelectObject(pOldBitmap);

	lock.LockThread();

	pCommand->Release();

	if (m_pCommand == NULL)
	{
		m_bImageReady = TRUE;
		if (m_pControl->GetSafeHwnd()) m_pControl->Invalidate(FALSE);
	}

	// Unlocked in destructor
}
