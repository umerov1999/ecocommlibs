// XTPChartDrawThread.h
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

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPCHARTDRAWTHREAD_H__)
#define __XTPCHARTDRAWTHREAD_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartDeviceCommand;
class CXTPChartDeviceContext;
class CXTPChartControl;
class CXTPChartContent;

class _XTP_EXT_CLASS CXTPChartDrawThreadDeviceCommand : public CXTPChartDeviceCommand
{
public:
	CXTPChartDrawThreadDeviceCommand(CXTPChartDeviceCommand* pCommand);
	~CXTPChartDrawThreadDeviceCommand();

public:
	void AddRef();
	void Release();

protected:
	long m_dwRef;
};

class _XTP_EXT_CLASS CXTPChartDrawThread : CWinThread
{
	DECLARE_DYNCREATE(CXTPChartDrawThread)
protected:
	CXTPChartDrawThread();
	~CXTPChartDrawThread();

protected:
	BOOL MonitorNotifications();
	void StopNotifications();

	BOOL InitInstance();
	void DrawContent();
	int Run();

public:
	void StartDraw(CSize sz, CXTPChartDrawThreadDeviceCommand* pCommand);

protected:
	HANDLE          m_dwMonitorEvents[2];   // Change event handles.

	CXTPChartDrawThreadDeviceCommand* m_pCommand;
	CSize m_szImage;
	BOOL m_bImageReady;
	CBitmap m_bmpCache;

	CXTPChartControl* m_pControl;
	CRITICAL_SECTION m_cs;


	friend class CXTPChartControl;
};

#endif //#if !defined(__XTPCHARTDRAWTHREAD_H__)
