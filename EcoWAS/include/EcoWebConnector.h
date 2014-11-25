#pragma once

#include <afxinet.h>
#include <wininet.h>

class CEcoWebConnector
{
public:
	CEcoWebConnector(void);
	~CEcoWebConnector(void);

private:
	HINTERNET m_hSession;
	HINTERNET m_hConnect;
	HINTERNET m_hRequest;
	int m_nErrorCode;
	CString m_strErrorMsg;

private:
	void SetLastError(int nErrorCode, CString strErrorMsg);

public:
	BOOL ConnectWebServer(CString strURL);
	void GetLastError(int& nErrorCode, CString& strMsg);
	void Close();
};

