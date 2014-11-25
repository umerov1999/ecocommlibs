#pragma once

#include "EcoSocketServer.h"

class CEcoSocketServerManager
{
public:
	CEcoSocketServerManager(void);
	~CEcoSocketServerManager(void);

private:
	boost::asio::io_service m_ioService;
	boost::system::error_code m_errorCode;
	CString m_strIP;
	int m_nPort;
	CEcoSocketServer* m_pSocketServer;

public:
	void SetHost(CString strIP, int nPort);
	BOOL Connect();
	BOOL SendResponseData(CEcoPacket& packet);
	BOOL GetCommandData(CEcoPacket& packet);
	BOOL GetLastErrorCode(int& nError, CString& strMsg);
	BOOL Close();
};

