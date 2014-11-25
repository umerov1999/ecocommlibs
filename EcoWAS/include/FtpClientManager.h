#pragma once

#include "FtpClient.h"

class CFtpClientManager
{
public:
	CFtpClientManager(void);
	~CFtpClientManager(void);

private:
	CFtpClient m_ecoFtpClient;
	int m_nErrorCode;
	CString m_strErrorMsg;
	BOOL m_bConnect;

private:
	void SetErrorCode(int nErrorCode, CString strErrorMsg);

public:
	BOOL Connect(CString strIP, CString strID, CString strPwd, int nPort = 21);
	BOOL UploadFile(CString strLocalFile, CString strRemoteFile);
	BOOL DownloadFile(CString strRemoteFile, CString strLocalFile);
	BOOL DeleteRemoteFile(CString strRemoteFile);
	BOOL DeleteRemoteDir(CString strRemoteDir);
	void Close();
	void GetLastErrorCode(int& nErrorCode, CString& strErrorMsg);
};

