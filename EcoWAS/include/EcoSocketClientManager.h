#pragma once

#include "EcoSocketClient.h"

class CEcoSocketClientManager
{
public:
	CEcoSocketClientManager(void);
	~CEcoSocketClientManager(void);

private:
	boost::asio::io_service m_ioService;
	boost::system::error_code m_errorCode;
	int m_nPort;
	CString m_strUserID;
	CEcoSocketClient* m_pSocketClient;

public:
	void SetPort(int nPort);
	void SetUser(CString strUserID);
	BOOL ConnectWait();
	BOOL SendCommand(CEcoPacket& packet);
	BOOL GetResponseData(CEcoPacket& packet);
	BOOL Close();
	BOOL GetLastErrorCode(int& nError, CString& strMsg);
};

