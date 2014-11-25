// FtpClient.h: interface for the CFtpClient class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FTPCLIENT_H__F196D430_806C_4A00_B5BE_04AC559B59A2__INCLUDED_)
#define AFX_FTPCLIENT_H__F196D430_806C_4A00_B5BE_04AC559B59A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxinet.h>	//ftp 를 위해 추가해야됨

class CFTPFileFind  
{
public:
	CFTPFileFind(CFtpConnection* pConnection);
	virtual ~CFTPFileFind();

private:
	int m_nErrorCode;
	CString m_strErrorMsg;

private:
	CFtpFileFind* m_pRemoteFindFile;
	CString	m_strAddress;

private:
	void SetErrorCode();
	void SetErrorCode(int nErrorCode, CString strErrorMsg);

public:	
	void GetLastErrorCode(int& nError, CString& strMsg);

	BOOL MatchsMask(DWORD dwMask);
	BOOL IsTemporary();
	BOOL IsSystem();
	BOOL IsReadOnly();
	BOOL IsNormal();
	BOOL IsHidden();
	BOOL IsDots();
	BOOL IsDirectory();
	BOOL IsCompressed();
	BOOL IsArchived();
	DWORD GetLength();
	BOOL GetLastWriteTime(CTime& Time);
	BOOL GetLastAccessTime(CTime& Time);
	CString GetFileTitle();
	CString GetFilePath();
	CString GetFileName();
	BOOL GetCreationTime(CTime& Time);
	CString GetFileURL();
	BOOL FindNextFile();
	BOOL FindFile(LPCTSTR RemoteFile = NULL, DWORD dwFlags = INTERNET_FLAG_RELOAD);
};

class CFtpClient  
{
public:
	CFtpClient();
	virtual ~CFtpClient();

private:
	int m_nErrorCode;
	CString m_strErrorMsg;

public:
	CInternetSession m_Session;
	CFtpConnection *m_pConnection;
	CString	m_strLocalDirText;
	CString	m_strRemoteDirText;
	CString	m_strAddress;

private:
	void SetErrorCode();
	void SetErrorCode(int nErrorCode, CString strErrorMsg);

public:
	void GetLastErrorCode(int& nError, CString& strMsg);

	BOOL DownLoadDir(LPCTSTR rDir, LPCTSTR lDir);
	BOOL UpLoadDir(LPCTSTR lDir, LPCTSTR rDir);

	BOOL GetURL(CString& RemoteURL);
	BOOL LPWD();	//현재 디렉토리 정보(CString LCD or RCD 때 쓰임)
	BOOL RPWD();
	BOOL LRen(CString LocalOldFile, CString LocalNewFile);	//이름 수정
	BOOL RRen(CString RemoteOldFile, CString RemoteNewFile);
	BOOL LDel(CString LocalFile);	//파일 삭제
	BOOL RDel(CString RemoteFile);
	BOOL LRD(CString LocalDir);		//디렉토리 삭제
	BOOL RRD(CString RemoteDir);
	BOOL LMD(CString LocalDir);		//디렉토리 생성
	BOOL RMD(CString RemoteDir);
	BOOL LCD(CString LocalDir);		//현재 디렉토리 정보
	BOOL RCD(CString RemoteDir);
	CString LCD();					//현재 디렉토리 경로 얻기
	CString RCD();

	BOOL DownLoad(CString RemoteFile, CString LocalFile);		//다운 받을 파일 경로(Server)와 다운로드 할 곳의 경로를 이용해 다운(Client)
	BOOL UpLoad(CString LocalFile, CString RemoteFile);

	BOOL Close();
	BOOL Open(CString strAddress, CString strName, CString strPassword, INTERNET_PORT nPort = INTERNET_DEFAULT_FTP_PORT);
	BOOL Open(CString strAddress, INTERNET_PORT nPort = INTERNET_DEFAULT_FTP_PORT);
	

};

#endif // !defined(AFX_FTPCLIENT_H__F196D430_806C_4A00_B5BE_04AC559B59A2__INCLUDED_)
