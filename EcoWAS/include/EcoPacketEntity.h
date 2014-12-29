#pragma once

#include "EcoWASDatabaseRecordSet.h"
#include "StringConverter.h"

/////////////////////////////////////////////////////////////////////////
//각 Packet Class들이 공용으로 사용하는 변수들을 묶은 class들.

class CPKShipNo
{
public:
	CPKShipNo(void)
	{

	}
	~CPKShipNo(void)
	{

	}

	CPKShipNo& operator = (const CPKShipNo &other)
	{
		m_strShipNoGroup = other.m_strShipNoGroup;
		m_strShipNo = other.m_strShipNo;
		return *this;
	}

	CPKShipNo( const CPKShipNo &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_strShipNoGroup);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_strShipNo);
			ar & stdStr;
		}
		else
		{
			ar & stdStr;
			m_strShipNoGroup = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_strShipNo = CStringConverter::CStringAToCStringW(stdStr.c_str());
		}
	}

public:
	void Init(void)
	{
		m_strShipNoGroup = _T("");
		m_strShipNo = _T("");
	}

public:
	CString m_strShipNoGroup;
	CString m_strShipNo;
};

class CPKWhereInherit : public CPKShipNo
{
public:
	CPKWhereInherit(void)
	{

	}
	~CPKWhereInherit(void)
	{

	}

	CPKWhereInherit& operator = (const CPKWhereInherit &other)
	{
		CPKShipNo::operator=(other);
		m_strWhere = other.m_strWhere;
		return *this;
	}

	CPKWhereInherit( const CPKWhereInherit &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKShipNo::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_strWhere);
			ar & stdStr;
		}
		else
		{
			ar & stdStr;
			m_strWhere = CStringConverter::CStringAToCStringW(stdStr.c_str());
		}
	}

public:
	void Init(void)
	{
		CPKShipNo::Init();
		m_strWhere = _T("");
	}

public:
	CString m_strWhere;
};

class CPKUserIDNIDInherit : public CPKWhereInherit
{
public:
	CPKUserIDNIDInherit(void)
	{

	}
	~CPKUserIDNIDInherit(void)
	{

	}

	CPKUserIDNIDInherit& operator = (const CPKUserIDNIDInherit &other)
	{
		CPKWhereInherit::operator=(other);
		m_nID = other.m_nID;
		m_strUserID = other.m_strUserID;
		return *this;
	}

	CPKUserIDNIDInherit( const CPKUserIDNIDInherit &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKWhereInherit::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			ar & m_nID;
			stdStr = CStringConverter::CStringWToCStringA(m_strUserID);
			ar & stdStr;
		}
		else
		{
			ar & m_nID;
			ar & stdStr;
			m_strUserID = CStringConverter::CStringAToCStringW(stdStr.c_str());
		}
	}

public:
	void Init(void)
	{
		CPKWhereInherit::Init();
		m_nID = 0;
		m_strUserID = _T("");
	}

public:
	UINT m_nID;
	CString m_strUserID;
};

class CPKIDInherit : public CPKWhereInherit
{
public:
	CPKIDInherit(void)
	{

	}
	~CPKIDInherit(void)
	{

	}

	CPKIDInherit& operator = (const CPKIDInherit &other)
	{
		CPKWhereInherit::operator=(other);
		m_nID = other.m_nID;
		return *this;
	}

	CPKIDInherit( const CPKIDInherit &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKWhereInherit::serialize(ar, version, bSendNRecv);
		ar & m_nID;
	}

public:
	void Init(void)
	{
		CPKWhereInherit::Init();
		m_nID = 0;
	}

public:
	UINT m_nID;
};


class CPKID
{
public:
	CPKID(void)
	{

	}
	~CPKID(void)
	{

	}

	CPKID& operator = (const CPKID &other)
	{
		m_nID = other.m_nID;
		return *this;
	}

	CPKID( const CPKID &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		ar & m_nID;
	}

public:
	void Init(void)
	{
		m_nID = 0;
	}

public:
	UINT m_nID;
};

class CPKWhere
{
public:
	CPKWhere(void)
	{

	}
	~CPKWhere(void)
	{

	}

	CPKWhere& operator = (const CPKWhere &other)
	{
		m_strWhere = other.m_strWhere;
		return *this;
	}

	CPKWhere( const CPKWhere &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_strWhere);
			ar & stdStr;
		}
		else
		{
			ar & stdStr;
			m_strWhere = CStringConverter::CStringAToCStringW(stdStr.c_str());
		}
	}

public:
	void Init(void)
	{
		m_strWhere = _T("");
	}

public:
	CString m_strWhere;
};

class CPKUserIDWhere
{
public:
	CPKUserIDWhere(void)
	{
		m_strUserID = _T("");
		m_strPwd = _T("");
		m_strWhere = _T("");
	}
	~CPKUserIDWhere(void)
	{

	}

	CPKUserIDWhere& operator = (const CPKUserIDWhere &other)
	{
		m_strUserID = other.m_strUserID;
		m_strPwd = other.m_strPwd;
		m_strWhere = other.m_strWhere;
		return *this;
	}

	CPKUserIDWhere( const CPKUserIDWhere &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_strUserID);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_strPwd);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_strWhere);
			ar & stdStr;
		}
		else
		{
			ar & stdStr;
			m_strUserID = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_strPwd = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_strWhere = CStringConverter::CStringAToCStringW(stdStr.c_str());
		}
	}

public:
	void Init(void)
	{
		m_strUserID = _T("");
		m_strPwd = _T("");
		m_strWhere = _T("");
	}

public:
	CString m_strUserID;
	CString m_strPwd;
	CString m_strWhere;
};


/////////////////////////////////////////////////////////////////////////
//기타 Packet들

class CPKNetFolderInfo
{
public:
	CPKNetFolderInfo(void)
	{
		m_strUserID = _T("");
		m_strPwd = _T("");
		m_strUnc = _T("");
	}
	~CPKNetFolderInfo(void)
	{

	}

	CPKNetFolderInfo& operator = (const CPKNetFolderInfo &other)
	{
		m_strUserID = other.m_strUserID;
		m_strPwd = other.m_strPwd;
		m_strUnc = other.m_strUnc;
		return *this;
	}

	CPKNetFolderInfo( const CPKNetFolderInfo &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_strUserID);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_strPwd);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_strUnc);
			ar & stdStr;
		}
		else
		{
			ar & stdStr;
			m_strUserID = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_strPwd = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_strUnc = CStringConverter::CStringAToCStringW(stdStr.c_str());
		}
	}

public:
	void Init(void)
	{
		m_strUserID = _T("");
		m_strPwd = _T("");
		m_strUnc = _T("");
	}

public:
	CString m_strUserID;
	CString m_strPwd;
	CString m_strUnc;
};

class CPKCreateTable : public CPKShipNo
{
public:
	CPKCreateTable(void)
	{
		m_strDB = _T("");
		m_bExistShipNo = FALSE;
	}
	~CPKCreateTable(void)
	{

	}

	CPKCreateTable& operator = (const CPKCreateTable &other)
	{
		CPKShipNo::operator=(other);
		m_strDB = other.m_strDB;
		m_bExistShipNo = other.m_bExistShipNo;
		return *this;
	}

	CPKCreateTable( const CPKCreateTable &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKShipNo::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_strDB);
			ar & stdStr;

			ar & m_bExistShipNo;
		}
		else
		{
			ar & stdStr;
			m_strDB = CStringConverter::CStringAToCStringW(stdStr.c_str());

			ar & m_bExistShipNo;
		}
	}

public:
	void Init(void)
	{
		CPKShipNo::Init();
		m_strDB = _T("");
		m_bExistShipNo = FALSE;
	}

public:
	CString m_strDB;
	BOOL m_bExistShipNo;
};

class CPKDuplicateData : public CPKWhere
{
public:
	CPKDuplicateData(void)
	{

	}
	~CPKDuplicateData(void)
	{

	}

	CPKDuplicateData& operator = (const CPKDuplicateData &other)
	{
		CPKWhere::operator=(other);
		m_strDB = other.m_strDB;
		m_strTableName = other.m_strTableName;
		m_bDuplicate = other.m_bDuplicate;
		return *this;
	}

	CPKDuplicateData( const CPKDuplicateData &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKWhere::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_strDB);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_strTableName);
			ar & stdStr;
			ar & m_bDuplicate;
		}
		else
		{
			ar & stdStr;
			m_strDB = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_strTableName = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & m_bDuplicate;
		}
	}

public:
	void Init(void)
	{
		m_strDB = _T("");
		m_strTableName = _T("");
		m_bDuplicate = false;
	}

public:
	CString m_strDB;
	CString m_strTableName;
	bool m_bDuplicate;
};

class CPKTableCopy
{
public:
	CPKTableCopy(void)
	{

	}
	~CPKTableCopy(void)
	{

	}

	CPKTableCopy& operator = (const CPKTableCopy &other)
	{
		m_strNewDB = other.m_strNewDB;
		m_strTmp = other.m_strTmp;
		m_strDBNameOri = other.m_strDBNameOri;
		return *this;
	}

	CPKTableCopy( const CPKTableCopy &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_strNewDB);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_strTmp);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_strDBNameOri);
			ar & stdStr;
		}
		else
		{
			ar & stdStr;
			m_strNewDB = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_strTmp = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_strDBNameOri = CStringConverter::CStringAToCStringW(stdStr.c_str());
		}
	}

public:
	void Init(void)
	{
		m_strNewDB = _T("");
		m_strTmp = _T("");
		m_strDBNameOri = _T("");
	}

public:
	CString m_strNewDB;
	CString m_strTmp;
	CString m_strDBNameOri;
};

/////////////////////////////////////////////////////////////////////////
//Common Table 관련 Packet들

class CPKShipNoInfo : public CPKIDInherit
{
public:
	CPKShipNoInfo(void)
	{

	}
	~CPKShipNoInfo(void)
	{

	}

	CPKShipNoInfo& operator = (const CPKShipNoInfo &other)
	{
		CPKIDInherit::operator=(other);
		m_nShipNoGroup = other.m_nShipNoGroup;
		m_nShipNo = other.m_nShipNo;
		m_bActive = other.m_bActive;
		m_strFilePath = other.m_strFilePath;
		m_arrShipNoInfo.Copy(other.m_arrShipNoInfo);
		return *this;
	}

	CPKShipNoInfo( const CPKShipNoInfo &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKIDInherit::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			ar & m_nShipNoGroup;
			ar & m_nShipNo;
			ar & m_bActive;
			stdStr = CStringConverter::CStringWToCStringA(m_strFilePath);
			ar & stdStr;

			m_nShipNoInfoCnt = m_arrShipNoInfo.GetCount();
			ar & m_nShipNoInfoCnt;
			for(int i = 0; i < m_nShipNoInfoCnt; i++)
				m_arrShipNoInfo.GetAt(i).serialize(ar, version, bSendNRecv);
		}
		else
		{
			ar & m_nShipNoGroup;
			ar & m_nShipNo;
			ar & m_bActive;
			ar & stdStr;
			m_strFilePath = CStringConverter::CStringAToCStringW(stdStr.c_str());

			ar & m_nShipNoInfoCnt;
			CShipNoInfoRecordSet rs; m_arrShipNoInfo.RemoveAll();
			for(int i = 0; i < m_nShipNoInfoCnt; i++)
			{
				rs.serialize(ar, version, bSendNRecv);
				m_arrShipNoInfo.Add(rs);
			}
		}
	}

public:
	void Init(void)
	{
		CPKIDInherit::Init();
		m_nShipNoGroup = 0;
		m_nShipNo = 0;
		m_bActive = FALSE;
		m_strFilePath = _T("");
		m_nShipNoInfoCnt = 0;
		m_arrShipNoInfo.RemoveAll();
	}

private:
	int m_nShipNoInfoCnt;

public:
	UINT m_nShipNoGroup;
	UINT m_nShipNo;
	BOOL m_bActive;
	CString m_strFilePath;
	CArray<CShipNoInfoRecordSet, CShipNoInfoRecordSet&> m_arrShipNoInfo;
};

class CPKDrawingTemplate : public CPKUserIDWhere
{
public:
	CPKDrawingTemplate(void)
	{

	}
	~CPKDrawingTemplate(void)
	{

	}

	CPKDrawingTemplate& operator = (const CPKDrawingTemplate &other)
	{
		CPKUserIDWhere::operator=(other);
		m_strCategory = other.m_strCategory;
		m_strTemplateName = other.m_strTemplateName;
		m_arrTemplateRS.Copy(other.m_arrTemplateRS);
		return *this;
	}

	CPKDrawingTemplate( const CPKDrawingTemplate &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKUserIDWhere::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_strCategory);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_strTemplateName);
			ar & stdStr;

			m_nTemplateRSCnt = m_arrTemplateRS.GetCount();
			ar & m_nTemplateRSCnt;
			for(int i = 0; i < m_nTemplateRSCnt; i++)
				m_arrTemplateRS.GetAt(i).serialize(ar, version, bSendNRecv);
		}
		else
		{
			ar & stdStr;
			m_strCategory = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_strTemplateName = CStringConverter::CStringAToCStringW(stdStr.c_str());

			ar & m_nTemplateRSCnt;
			CTemplateTbRecordSet rs; m_arrTemplateRS.RemoveAll();
			for(int i = 0; i < m_nTemplateRSCnt; i++)
			{
				rs.serialize(ar, version, bSendNRecv);
				m_arrTemplateRS.Add(rs);
			}
		}
	}

public:
	void Init(void)
	{
		CPKUserIDWhere::Init();
		m_strCategory = _T("");
		m_strTemplateName = _T("");
		m_nTemplateRSCnt = 0;
		m_arrTemplateRS.RemoveAll();
	}

private:
	int m_nTemplateRSCnt;

public:
	CString m_strCategory;
	CString m_strTemplateName;
	CArray<CTemplateTbRecordSet, CTemplateTbRecordSet&> m_arrTemplateRS;
};

class CPKUserTemplateInfo : public CPKUserIDWhere
{
public:
	CPKUserTemplateInfo(void)
	{

	}
	~CPKUserTemplateInfo(void)
	{

	}

	CPKUserTemplateInfo& operator = (const CPKUserTemplateInfo &other)
	{
		CPKUserIDWhere::operator=(other);
		m_strTmpName = other.m_strTmpName;
		m_nUsedCount = other.m_nUsedCount;
		m_arrUserTemplateInfoRS.Copy(other.m_arrUserTemplateInfoRS);
		return *this;
	}

	CPKUserTemplateInfo( const CPKUserTemplateInfo &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKUserIDWhere::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			ar & m_nUsedCount;
			stdStr = CStringConverter::CStringWToCStringA(m_strTmpName);
			ar & stdStr;

			m_nUserTemplateInfoRSCnt = m_arrUserTemplateInfoRS.GetCount();
			ar & m_nUserTemplateInfoRSCnt;
			for(int i = 0; i < m_nUserTemplateInfoRSCnt; i++)
				m_arrUserTemplateInfoRS.GetAt(i).serialize(ar, version, bSendNRecv);
		}
		else
		{
			ar & m_nUsedCount;
			ar & stdStr;
			m_strTmpName = CStringConverter::CStringAToCStringW(stdStr.c_str());

			ar & m_nUserTemplateInfoRSCnt;
			CEcoUserTemplateInfo rs; m_arrUserTemplateInfoRS.RemoveAll();
			for(int i = 0; i < m_nUserTemplateInfoRSCnt; i++)
			{
				rs.serialize(ar, version, bSendNRecv);
				m_arrUserTemplateInfoRS.Add(rs);
			}
		}
	}

public:
	void Init(void)
	{
		CPKUserIDWhere::Init();
		m_nUsedCount = 0;
		m_strTmpName = _T("");
		m_nUserTemplateInfoRSCnt = 0;
		m_arrUserTemplateInfoRS.RemoveAll();
	}

private:
	int m_nUserTemplateInfoRSCnt;

public:
	int m_nUsedCount;
	CString m_strTmpName;
	CArray<CEcoUserTemplateInfo, CEcoUserTemplateInfo&> m_arrUserTemplateInfoRS;
};

class CPKUserInfo : public CPKUserIDWhere
{
public:
	CPKUserInfo(void)
	{

	}
	~CPKUserInfo(void)
	{

	}

	CPKUserInfo& operator = (const CPKUserInfo &other)
	{
		CPKUserIDWhere::operator=(other);
		m_userInfoRS = other.m_userInfoRS;
		m_arrUserInfoRS.Copy(other.m_arrUserInfoRS);
		return *this;
	}

	CPKUserInfo( const CPKUserInfo &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKUserIDWhere::serialize(ar, version, bSendNRecv);
		if(bSendNRecv == TRUE)
		{
			ar & m_bAdmin;

			m_userInfoRS.serialize(ar, version, bSendNRecv);

			m_nUserInfoRSCnt = m_arrUserInfoRS.GetCount();
			ar & m_nUserInfoRSCnt;
			for(int i = 0; i < m_nUserInfoRSCnt; i++)
				m_arrUserInfoRS.GetAt(i).serialize(ar, version, bSendNRecv);
		}
		else
		{
			ar & m_bAdmin;

			m_userInfoRS.serialize(ar, version, bSendNRecv);

			ar & m_nUserInfoRSCnt;
			CUserInfoRecordSet rs; m_arrUserInfoRS.RemoveAll();
			for(int i = 0; i < m_nUserInfoRSCnt; i++)
			{
				rs.serialize(ar, version, bSendNRecv);
				m_arrUserInfoRS.Add(rs);
			}
		}
	}

public:
	void Init(void)
	{
		CPKUserIDWhere::Init();
		m_bAdmin = FALSE;
		m_nUserInfoRSCnt = 0;
		m_arrUserInfoRS.RemoveAll();
	}

private:
	int m_nUserInfoRSCnt;

public:
	BOOL m_bAdmin;
	CUserInfoRecordSet m_userInfoRS;
	CArray<CUserInfoRecordSet, CUserInfoRecordSet&> m_arrUserInfoRS;
};

class CPKAutoUpdateFile : public CPKWhere
{
public:
	CPKAutoUpdateFile(void)
	{

	}
	~CPKAutoUpdateFile(void)
	{

	}

	CPKAutoUpdateFile& operator = (const CPKAutoUpdateFile &other)
	{
		CPKWhere::operator=(other);
		m_strFileName = other.m_strFileName;
		m_strPlatform = other.m_strPlatform;
		m_arrAutoUpdateFileRS.Copy(other.m_arrAutoUpdateFileRS);
		return *this;
	}

	CPKAutoUpdateFile( const CPKAutoUpdateFile &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKWhere::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_strFileName);
			ar & stdStr;

			stdStr = CStringConverter::CStringWToCStringA(m_strPlatform);
			ar & stdStr;

			m_nAutoUpdateFileRSCnt = m_arrAutoUpdateFileRS.GetCount();
			ar & m_nAutoUpdateFileRSCnt;
			for(int i = 0; i < m_nAutoUpdateFileRSCnt; i++)
				m_arrAutoUpdateFileRS.GetAt(i).serialize(ar, version, bSendNRecv);
		}
		else
		{
			ar & stdStr;
			m_strFileName = CStringConverter::CStringAToCStringW(stdStr.c_str());

			ar & stdStr;
			m_strPlatform = CStringConverter::CStringAToCStringW(stdStr.c_str());

			ar & m_nAutoUpdateFileRSCnt;
			CAutoUpdateFileRercordSet rs; m_arrAutoUpdateFileRS.RemoveAll();
			for(int i = 0; i < m_nAutoUpdateFileRSCnt; i++)
			{
				rs.serialize(ar, version, bSendNRecv);
				m_arrAutoUpdateFileRS.Add(rs);
			}
		}
	}

public:
	void Init(void)
	{
		CPKWhere::Init();
		m_strFileName = _T("");
		m_strPlatform = _T("");
		m_nAutoUpdateFileRSCnt = 0;
		m_arrAutoUpdateFileRS.RemoveAll();
	}

private:
	int m_nAutoUpdateFileRSCnt;

public:
	CString m_strFileName;
	CString m_strPlatform;
	CArray<CAutoUpdateFileRercordSet, CAutoUpdateFileRercordSet&> m_arrAutoUpdateFileRS;
};

class CPKConcurrentInfo : public CPKID
{
public:
	CPKConcurrentInfo(void)
	{

	}
	~CPKConcurrentInfo(void)
	{

	}

	CPKConcurrentInfo& operator = (const CPKConcurrentInfo &other)
	{
		CPKID::operator=(other);
		m_strProductName = other.m_strProductName;
		return *this;
	}

	CPKConcurrentInfo( const CPKConcurrentInfo &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKID::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_strProductName);
			ar & stdStr;
		}
		else
		{
			ar & stdStr;
			m_strProductName = CStringConverter::CStringAToCStringW(stdStr.c_str());
		}
	}

public:
	void Init(void)
	{
		CPKID::Init();
		m_strProductName = _T("");
	}

public:
	CString m_strProductName;
};

/////////////////////////////////////////////////////////////////////////
//EcoBlock 프로젝트 관련 Packet들

class CPKProject : public CPKIDInherit
{
public:
	CPKProject(void)
	{

	}
	~CPKProject(void)
	{

	}

	CPKProject& operator = (const CPKProject &other)
	{
		CPKIDInherit::operator=(other);
		m_arrProjectRS.Copy(other.m_arrProjectRS);
		m_arrProjectNPropRS.Copy(other.m_arrProjectNPropRS);
		return *this;
	}

	CPKProject( const CPKProject &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKIDInherit::serialize(ar, version, bSendNRecv);
		if(bSendNRecv == TRUE)
		{
			m_nProjectRSCnt = m_arrProjectRS.GetCount();
			ar & m_nProjectRSCnt;
			for(int i = 0; i < m_nProjectRSCnt; i++)
				m_arrProjectRS.GetAt(i).serialize(ar, version, bSendNRecv);

			m_nProjectNPropRSCnt = m_arrProjectNPropRS.GetCount();
			ar & m_nProjectNPropRSCnt;
			for(int i = 0; i < m_nProjectNPropRSCnt; i++)
				m_arrProjectNPropRS.GetAt(i).serialize(ar, version, bSendNRecv);
		}
		else
		{
			ar & m_nProjectRSCnt;
			CProjectRecordSet rs; m_arrProjectRS.RemoveAll();
			for(int i = 0; i < m_nProjectRSCnt; i++)
			{
				rs.serialize(ar, version, bSendNRecv);
				m_arrProjectRS.Add(rs);
			}

			ar & m_nProjectNPropRSCnt;
			PROJECT_WITH_PROPERTY stru; m_arrProjectNPropRS.RemoveAll();
			for(int i = 0; i < m_nProjectNPropRSCnt; i++)
			{
				stru.serialize(ar, version, bSendNRecv);
				m_arrProjectNPropRS.Add(stru);
			}
		}
	}

public:
	void Init(void)
	{
		CPKIDInherit::Init();
		m_nProjectRSCnt = 0;
		m_arrProjectRS.RemoveAll();
		m_nProjectNPropRSCnt = 0;
		m_arrProjectNPropRS.RemoveAll();
	}

private:
	int m_nProjectRSCnt;
	int m_nProjectNPropRSCnt;

public:
	CArray<CProjectRecordSet, CProjectRecordSet&> m_arrProjectRS;
	CArray<PROJECT_WITH_PROPERTY, PROJECT_WITH_PROPERTY&> m_arrProjectNPropRS;
};

class CPKEcoBlockProjectTree : public CPKIDInherit
{
public:
	CPKEcoBlockProjectTree(void)
	{

	}
	~CPKEcoBlockProjectTree(void)
	{

	}

	CPKEcoBlockProjectTree& operator = (const CPKEcoBlockProjectTree &other)
	{
		CPKIDInherit::operator=(other);
		m_arrBlockPjtTreeRS.Copy(other.m_arrBlockPjtTreeRS);
		return *this;
	}

	CPKEcoBlockProjectTree( const CPKEcoBlockProjectTree &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKIDInherit::serialize(ar, version, bSendNRecv);
		if(bSendNRecv == TRUE)
		{
			m_nBlockPjtTreeRSCnt = m_arrBlockPjtTreeRS.GetCount();
			ar & m_nBlockPjtTreeRSCnt;
			for(int i = 0; i < m_nBlockPjtTreeRSCnt; i++)
				m_arrBlockPjtTreeRS.GetAt(i).serialize(ar, version, bSendNRecv);
		}
		else
		{
			ar & m_nBlockPjtTreeRSCnt;
			CEcoBlockProjectTreeRecordSet rs; m_arrBlockPjtTreeRS.RemoveAll();
			for(int i = 0; i < m_nBlockPjtTreeRSCnt; i++)
			{
				rs.serialize(ar, version, bSendNRecv);
				m_arrBlockPjtTreeRS.Add(rs);
			}
		}
	}

public:
	void Init(void)
	{
		CPKIDInherit::Init();
		m_nBlockPjtTreeRSCnt = 0;
		m_arrBlockPjtTreeRS.RemoveAll();
	}

private:
	int m_nBlockPjtTreeRSCnt;

public:
	CArray<CEcoBlockProjectTreeRecordSet, CEcoBlockProjectTreeRecordSet&> m_arrBlockPjtTreeRS;
};

class CPKMeasurePoint : public CPKIDInherit
{
public:
	CPKMeasurePoint(void)
	{

	}
	~CPKMeasurePoint(void)
	{

	}

	CPKMeasurePoint& operator = (const CPKMeasurePoint &other)
	{
		CPKIDInherit::operator=(other);
		m_arrMsrPtRS.Copy(other.m_arrMsrPtRS);
		return *this;
	}

	CPKMeasurePoint( const CPKMeasurePoint &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKIDInherit::serialize(ar, version, bSendNRecv);
		if(bSendNRecv == TRUE)
		{
			m_nMsrPtRSCnt = m_arrMsrPtRS.GetCount();
			ar & m_nMsrPtRSCnt;
			for(int i = 0; i < m_nMsrPtRSCnt; i++)
				m_arrMsrPtRS.GetAt(i).serialize(ar, version, bSendNRecv);
		}
		else
		{
			ar & m_nMsrPtRSCnt;
			CMeasurePointRecordSet rs; m_arrMsrPtRS.RemoveAll();
			for(int i = 0; i < m_nMsrPtRSCnt; i++)
			{
				rs.serialize(ar, version, bSendNRecv);
				m_arrMsrPtRS.Add(rs);
			}
		}
	}

public:
	void Init(void)
	{
		CPKIDInherit::Init();
		m_nMsrPtRSCnt = 0;
		m_arrMsrPtRS.RemoveAll();
	}

private:
	int m_nMsrPtRSCnt;

public:
	CArray<CMeasurePointRecordSet, CMeasurePointRecordSet&> m_arrMsrPtRS;
};

class CPKEcoBlockProject : public CPKUserIDNIDInherit
{
public:
	CPKEcoBlockProject(void)
	{

	}
	~CPKEcoBlockProject(void)
	{

	}

	CPKEcoBlockProject& operator = (const CPKEcoBlockProject &other)
	{
		CPKUserIDNIDInherit::operator=(other);
		m_strNewSuccessInfo = other.m_strNewSuccessInfo;
		m_bExist = other.m_bExist;
		m_strUpdateDate = other.m_strUpdateDate;
		m_strUpdateUser = other.m_strUpdateUser;
		m_olePreDate.m_dt = other.m_olePreDate.m_dt;
		m_arrBlockProjectRS.Copy(other.m_arrBlockProjectRS);
		return *this;
	}

	CPKEcoBlockProject( const CPKEcoBlockProject &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKUserIDNIDInherit::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			ar & m_bExist;
			ar & m_olePreDate.m_dt;
			stdStr = CStringConverter::CStringWToCStringA(m_strNewSuccessInfo);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_strUpdateDate);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_strUpdateUser);
			ar & stdStr;

			m_nBlockProjectRSCnt = m_arrBlockProjectRS.GetCount();
			ar & m_nBlockProjectRSCnt;
			for(int i = 0; i < m_nBlockProjectRSCnt; i++)
				m_arrBlockProjectRS.GetAt(i).serialize(ar, version, bSendNRecv);
		}
		else
		{
			ar & m_bExist;
			ar & m_olePreDate.m_dt;
			ar & stdStr;
			m_strNewSuccessInfo = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_strUpdateDate = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_strUpdateUser = CStringConverter::CStringAToCStringW(stdStr.c_str());

			ar & m_nBlockProjectRSCnt;
			CEcoBlockProjectRecordSet rs; m_arrBlockProjectRS.RemoveAll();
			for(int i = 0; i < m_nBlockProjectRSCnt; i++)
			{
				rs.serialize(ar, version, bSendNRecv);
				m_arrBlockProjectRS.Add(rs);
			}
		}
	}

public:
	void Init(void)
	{
		CPKUserIDNIDInherit::Init();
		m_strNewSuccessInfo = _T("");
		m_bExist = FALSE;
		m_strUpdateDate = _T("");
		m_strUpdateUser = _T("");
		m_olePreDate.m_dt = 0;
		m_nBlockProjectRSCnt = 0;
		m_arrBlockProjectRS.RemoveAll();
	}

private:
	int m_nBlockProjectRSCnt;

public:
	BOOL m_bExist;
	COleDateTime m_olePreDate;
	CString m_strNewSuccessInfo;
	CString m_strUpdateDate;
	CString m_strUpdateUser;
	CArray<CEcoBlockProjectRecordSet, CEcoBlockProjectRecordSet&> m_arrBlockProjectRS;
};

class CPKEcoBlockRelationInfo : public CPKIDInherit
{
public:
	CPKEcoBlockRelationInfo(void)
	{

	}
	~CPKEcoBlockRelationInfo(void)
	{

	}

	CPKEcoBlockRelationInfo& operator = (const CPKEcoBlockRelationInfo &other)
	{
		CPKIDInherit::operator=(other);
		m_arrBlockRelaInfoRS.Copy(other.m_arrBlockRelaInfoRS);
		return *this;
	}

	CPKEcoBlockRelationInfo( const CPKEcoBlockRelationInfo &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKIDInherit::serialize(ar, version, bSendNRecv);
		if(bSendNRecv == TRUE)
		{
			m_nBlockRelaInfoCnt = m_arrBlockRelaInfoRS.GetCount();
			ar & m_nBlockRelaInfoCnt;
			for(int i = 0; i < m_nBlockRelaInfoCnt; i++)
				m_arrBlockRelaInfoRS.GetAt(i).serialize(ar, version, bSendNRecv);
		}
		else
		{
			ar & m_nBlockRelaInfoCnt;
			CEcoBlockRelationInfoRecordSet rs; m_arrBlockRelaInfoRS.RemoveAll();
			for(int i = 0; i < m_nBlockRelaInfoCnt; i++)
			{
				rs.serialize(ar, version, bSendNRecv);
				m_arrBlockRelaInfoRS.Add(rs);
			}
		}
	}

public:
	void Init(void)
	{
		CPKIDInherit::Init();
		m_nBlockRelaInfoCnt = 0;
		m_arrBlockRelaInfoRS.RemoveAll();
	}

private:
	int m_nBlockRelaInfoCnt;

public:
	CArray<CEcoBlockRelationInfoRecordSet, CEcoBlockRelationInfoRecordSet&> m_arrBlockRelaInfoRS;
};

class CPKEcoBlockManualMesManager : public CPKIDInherit
{
public:
	CPKEcoBlockManualMesManager(void)
	{

	}
	~CPKEcoBlockManualMesManager(void)
	{

	}

	CPKEcoBlockManualMesManager& operator = (const CPKEcoBlockManualMesManager &other)
	{
		CPKIDInherit::operator=(other);
		m_arrBlockManualMesManagerRS.Copy(other.m_arrBlockManualMesManagerRS);
		return *this;
	}

	CPKEcoBlockManualMesManager( const CPKEcoBlockManualMesManager &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKIDInherit::serialize(ar, version, bSendNRecv);
		if(bSendNRecv == TRUE)
		{
			m_nBlockManualMesManagerRSCnt = m_arrBlockManualMesManagerRS.GetCount();
			ar & m_nBlockManualMesManagerRSCnt;
			for(int i = 0; i < m_nBlockManualMesManagerRSCnt; i++)
				m_arrBlockManualMesManagerRS.GetAt(i).serialize(ar, version, bSendNRecv);
		}
		else
		{
			ar & m_nBlockManualMesManagerRSCnt;
			CEcoBlockManualMesManagerRecordSet rs; m_arrBlockManualMesManagerRS.RemoveAll();
			for(int i = 0; i < m_nBlockManualMesManagerRSCnt; i++)
			{
				rs.serialize(ar, version, bSendNRecv);
				m_arrBlockManualMesManagerRS.Add(rs);
			}
		}
	}

public:
	void Init(void)
	{
		CPKIDInherit::Init();
		m_nBlockManualMesManagerRSCnt = 0;
		m_arrBlockManualMesManagerRS.RemoveAll();
	}

private:
	int m_nBlockManualMesManagerRSCnt;

public:
	CArray<CEcoBlockManualMesManagerRecordSet, CEcoBlockManualMesManagerRecordSet&> m_arrBlockManualMesManagerRS;
};

class CPKEcoBlockDesignPointGeneralInfo : public CPKIDInherit
{
public:
	CPKEcoBlockDesignPointGeneralInfo(void)
	{

	}
	~CPKEcoBlockDesignPointGeneralInfo(void)
	{

	}

	CPKEcoBlockDesignPointGeneralInfo& operator = (const CPKEcoBlockDesignPointGeneralInfo &other)
	{
		CPKIDInherit::operator=(other);
		m_arrBlockDsnPtGeneralInfoRS.Copy(other.m_arrBlockDsnPtGeneralInfoRS);
		return *this;
	}

	CPKEcoBlockDesignPointGeneralInfo( const CPKEcoBlockDesignPointGeneralInfo &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKIDInherit::serialize(ar, version, bSendNRecv);
		if(bSendNRecv == TRUE)
		{
			m_nBlockDsnPtGeneralInfoRSCnt = m_arrBlockDsnPtGeneralInfoRS.GetCount();
			ar & m_nBlockDsnPtGeneralInfoRSCnt;
			for(int i = 0; i < m_nBlockDsnPtGeneralInfoRSCnt; i++)
				m_arrBlockDsnPtGeneralInfoRS.GetAt(i).serialize(ar, version, bSendNRecv);
		}
		else
		{
			ar & m_nBlockDsnPtGeneralInfoRSCnt;
			CEcoBlockDesignPointGeneralInfoRecordSet rs; m_arrBlockDsnPtGeneralInfoRS.RemoveAll();
			for(int i = 0; i < m_nBlockDsnPtGeneralInfoRSCnt; i++)
			{
				rs.serialize(ar, version, bSendNRecv);
				m_arrBlockDsnPtGeneralInfoRS.Add(rs);
			}
		}
	}

public:
	void Init(void)
	{
		CPKIDInherit::Init();
		m_nBlockDsnPtGeneralInfoRSCnt = 0;
		m_arrBlockDsnPtGeneralInfoRS.RemoveAll();
	}

private:
	int m_nBlockDsnPtGeneralInfoRSCnt;

public:
	CArray<CEcoBlockDesignPointGeneralInfoRecordSet, CEcoBlockDesignPointGeneralInfoRecordSet&> m_arrBlockDsnPtGeneralInfoRS;
};

class CPKEcoBlockControlPointCoordInfo : public CPKIDInherit
{
public:
	CPKEcoBlockControlPointCoordInfo(void)
	{

	}
	~CPKEcoBlockControlPointCoordInfo(void)
	{

	}

	CPKEcoBlockControlPointCoordInfo& operator = (const CPKEcoBlockControlPointCoordInfo &other)
	{
		CPKIDInherit::operator=(other);
		m_arrBlockCtrlPtCoordInfoRS.Copy(other.m_arrBlockCtrlPtCoordInfoRS);
		return *this;
	}

	CPKEcoBlockControlPointCoordInfo( const CPKEcoBlockControlPointCoordInfo &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKIDInherit::serialize(ar, version, bSendNRecv);
		if(bSendNRecv == TRUE)
		{
			m_nBlockCtrlPtCoordInfoRSCnt = m_arrBlockCtrlPtCoordInfoRS.GetCount();
			ar & m_nBlockCtrlPtCoordInfoRSCnt;
			for(int i = 0; i < m_nBlockCtrlPtCoordInfoRSCnt; i++)
				m_arrBlockCtrlPtCoordInfoRS.GetAt(i).serialize(ar, version, bSendNRecv);
		}
		else
		{
			ar & m_nBlockCtrlPtCoordInfoRSCnt;
			CEcoBlockControlPointCoordInfoRecordSet rs; m_arrBlockCtrlPtCoordInfoRS.RemoveAll();
			for(int i = 0; i < m_nBlockCtrlPtCoordInfoRSCnt; i++)
			{
				rs.serialize(ar, version, bSendNRecv);
				m_arrBlockCtrlPtCoordInfoRS.Add(rs);
			}
		}
	}

public:
	void Init(void)
	{
		CPKIDInherit::Init();
		m_nBlockCtrlPtCoordInfoRSCnt = 0;
		m_arrBlockCtrlPtCoordInfoRS.RemoveAll();
	}

private:
	int m_nBlockCtrlPtCoordInfoRSCnt;

public:
	CArray<CEcoBlockControlPointCoordInfoRecordSet, CEcoBlockControlPointCoordInfoRecordSet&> m_arrBlockCtrlPtCoordInfoRS;
};

class CPKEcoBlockControlPointDistanceInfo : public CPKIDInherit
{
public:
	CPKEcoBlockControlPointDistanceInfo(void)
	{

	}
	~CPKEcoBlockControlPointDistanceInfo(void)
	{

	}

	CPKEcoBlockControlPointDistanceInfo& operator = (const CPKEcoBlockControlPointDistanceInfo &other)
	{
		CPKIDInherit::operator=(other);
		m_arrBlockCtrlPtDistInfoRS.Copy(other.m_arrBlockCtrlPtDistInfoRS);
		return *this;
	}

	CPKEcoBlockControlPointDistanceInfo( const CPKEcoBlockControlPointDistanceInfo &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKIDInherit::serialize(ar, version, bSendNRecv);
		if(bSendNRecv == TRUE)
		{
			m_nBlockCtrlPtDistInfoRSCnt = m_arrBlockCtrlPtDistInfoRS.GetCount();
			ar & m_nBlockCtrlPtDistInfoRSCnt;
			for(int i = 0; i < m_nBlockCtrlPtDistInfoRSCnt; i++)
				m_arrBlockCtrlPtDistInfoRS.GetAt(i).serialize(ar, version, bSendNRecv);
		}
		else
		{
			ar & m_nBlockCtrlPtDistInfoRSCnt;
			CEcoBlockControlPointDistanceInfoRecordSet rs; m_arrBlockCtrlPtDistInfoRS.RemoveAll();
			for(int i = 0; i < m_nBlockCtrlPtDistInfoRSCnt; i++)
			{
				rs.serialize(ar, version, bSendNRecv);
				m_arrBlockCtrlPtDistInfoRS.Add(rs);
			}
		}
	}

public:
	void Init(void)
	{
		CPKIDInherit::Init();
		m_nBlockCtrlPtDistInfoRSCnt = 0;
		m_arrBlockCtrlPtDistInfoRS.RemoveAll();
	}

private:
	int m_nBlockCtrlPtDistInfoRSCnt;

public:
	CArray<CEcoBlockControlPointDistanceInfoRecordSet, CEcoBlockControlPointDistanceInfoRecordSet&> m_arrBlockCtrlPtDistInfoRS;
};

class CPKEcoBlockContorlPointCurveInfo : public CPKIDInherit
{
public:
	CPKEcoBlockContorlPointCurveInfo(void)
	{

	}
	~CPKEcoBlockContorlPointCurveInfo(void)
	{

	}

	CPKEcoBlockContorlPointCurveInfo& operator = (const CPKEcoBlockContorlPointCurveInfo &other)
	{
		CPKIDInherit::operator=(other);
		m_arrBlockCtrlPtCurveInfoRS.Copy(other.m_arrBlockCtrlPtCurveInfoRS);
		return *this;
	}

	CPKEcoBlockContorlPointCurveInfo( const CPKEcoBlockContorlPointCurveInfo &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKIDInherit::serialize(ar, version, bSendNRecv);
		if(bSendNRecv == TRUE)
		{
			m_nBlockCtrlPtCurveInfoRSCnt = m_arrBlockCtrlPtCurveInfoRS.GetCount();
			ar & m_nBlockCtrlPtCurveInfoRSCnt;
			for(int i = 0; i < m_nBlockCtrlPtCurveInfoRSCnt; i++)
				m_arrBlockCtrlPtCurveInfoRS.GetAt(i).serialize(ar, version, bSendNRecv);
		}
		else
		{
			ar & m_nBlockCtrlPtCurveInfoRSCnt;
			CEcoBlockControlPointCurveInfoRecordSet rs; m_arrBlockCtrlPtCurveInfoRS.RemoveAll();
			for(int i = 0; i < m_nBlockCtrlPtCurveInfoRSCnt; i++)
			{
				rs.serialize(ar, version, bSendNRecv);
				m_arrBlockCtrlPtCurveInfoRS.Add(rs);
			}
		}
	}

public:
	void Init(void)
	{
		CPKIDInherit::Init();
		m_nBlockCtrlPtCurveInfoRSCnt = 0;
		m_arrBlockCtrlPtCurveInfoRS.RemoveAll();
	}

private:
	int m_nBlockCtrlPtCurveInfoRSCnt;

public:
	CArray<CEcoBlockControlPointCurveInfoRecordSet, CEcoBlockControlPointCurveInfoRecordSet&> m_arrBlockCtrlPtCurveInfoRS;
};

class CPKEcoBlockControlPointPerpInfo : public CPKIDInherit
{
public:
	CPKEcoBlockControlPointPerpInfo(void)
	{

	}
	~CPKEcoBlockControlPointPerpInfo(void)
	{

	}

	CPKEcoBlockControlPointPerpInfo& operator = (const CPKEcoBlockControlPointPerpInfo &other)
	{
		CPKIDInherit::operator=(other);
		m_arrBlockCtrlPtPerpInfoRS.Copy(other.m_arrBlockCtrlPtPerpInfoRS);
		return *this;
	}

	CPKEcoBlockControlPointPerpInfo( const CPKEcoBlockControlPointPerpInfo &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKIDInherit::serialize(ar, version, bSendNRecv);
		if(bSendNRecv == TRUE)
		{
			m_nBlockCtrlPtPerpInfoRSCnt = m_arrBlockCtrlPtPerpInfoRS.GetCount();
			ar & m_nBlockCtrlPtPerpInfoRSCnt;
			for(int i = 0; i < m_nBlockCtrlPtPerpInfoRSCnt; i++)
				m_arrBlockCtrlPtPerpInfoRS.GetAt(i).serialize(ar, version, bSendNRecv);
		}
		else
		{
			ar & m_nBlockCtrlPtPerpInfoRSCnt;
			CEcoBlockControlPointPerpInfoRecordSet rs; m_arrBlockCtrlPtPerpInfoRS.RemoveAll();
			for(int i = 0; i < m_nBlockCtrlPtPerpInfoRSCnt; i++)
			{
				rs.serialize(ar, version, bSendNRecv);
				m_arrBlockCtrlPtPerpInfoRS.Add(rs);
			}
		}
	}

public:
	void Init(void)
	{
		CPKIDInherit::Init();
		m_nBlockCtrlPtPerpInfoRSCnt = 0;
		m_arrBlockCtrlPtPerpInfoRS.RemoveAll();
	}

private:
	int m_nBlockCtrlPtPerpInfoRSCnt;

public:
	CArray<CEcoBlockControlPointPerpInfoRecordSet, CEcoBlockControlPointPerpInfoRecordSet&> m_arrBlockCtrlPtPerpInfoRS;
};

class CPKEcoBlockControlPointAngleInfo : public CPKIDInherit
{
public:
	CPKEcoBlockControlPointAngleInfo(void)
	{

	}
	~CPKEcoBlockControlPointAngleInfo(void)
	{

	}

	CPKEcoBlockControlPointAngleInfo& operator = (const CPKEcoBlockControlPointAngleInfo &other)
	{
		CPKIDInherit::operator=(other);
		m_arrBlockCtrlPtAngleInfoRS.Copy(other.m_arrBlockCtrlPtAngleInfoRS);
		return *this;
	}

	CPKEcoBlockControlPointAngleInfo( const CPKEcoBlockControlPointAngleInfo &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKIDInherit::serialize(ar, version, bSendNRecv);
		if(bSendNRecv == TRUE)
		{
			m_nBlockCtrlPtAngleInfoRSCnt = m_arrBlockCtrlPtAngleInfoRS.GetCount();
			ar & m_nBlockCtrlPtAngleInfoRSCnt;
			for(int i = 0; i < m_nBlockCtrlPtAngleInfoRSCnt; i++)
				m_arrBlockCtrlPtAngleInfoRS.GetAt(i).serialize(ar, version, bSendNRecv);
		}
		else
		{
			ar & m_nBlockCtrlPtAngleInfoRSCnt;
			CEcoBlockControlPointAngleInfoRecordSet rs; m_arrBlockCtrlPtAngleInfoRS.RemoveAll();
			for(int i = 0; i < m_nBlockCtrlPtAngleInfoRSCnt; i++)
			{
				rs.serialize(ar, version, bSendNRecv);
				m_arrBlockCtrlPtAngleInfoRS.Add(rs);
			}
		}
	}

public:
	void Init(void)
	{
		CPKIDInherit::Init();
		m_nBlockCtrlPtAngleInfoRSCnt = 0;
		m_arrBlockCtrlPtAngleInfoRS.RemoveAll();
	}

private:
	int m_nBlockCtrlPtAngleInfoRSCnt;

public:
	CArray<CEcoBlockControlPointAngleInfoRecordSet, CEcoBlockControlPointAngleInfoRecordSet&> m_arrBlockCtrlPtAngleInfoRS;
};

class CPKEcoBlockControlPointFlatInfo : public CPKIDInherit
{
public:
	CPKEcoBlockControlPointFlatInfo(void)
	{

	}
	~CPKEcoBlockControlPointFlatInfo(void)
	{

	}

	CPKEcoBlockControlPointFlatInfo& operator = (const CPKEcoBlockControlPointFlatInfo &other)
	{
		CPKIDInherit::operator=(other);
		m_arrBlockCtrlPtFlatInfoRS.Copy(other.m_arrBlockCtrlPtFlatInfoRS);
		return *this;
	}

	CPKEcoBlockControlPointFlatInfo( const CPKEcoBlockControlPointFlatInfo &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKIDInherit::serialize(ar, version, bSendNRecv);
		if(bSendNRecv == TRUE)
		{
			m_nBlockCtrlPtFlatInfoRSCnt = m_arrBlockCtrlPtFlatInfoRS.GetCount();
			ar & m_nBlockCtrlPtFlatInfoRSCnt;
			for(int i = 0; i < m_nBlockCtrlPtFlatInfoRSCnt; i++)
				m_arrBlockCtrlPtFlatInfoRS.GetAt(i).serialize(ar, version, bSendNRecv);
		}
		else
		{
			ar & m_nBlockCtrlPtFlatInfoRSCnt;
			CEcoBlockControlPointFlatInfoRecordSet rs; m_arrBlockCtrlPtFlatInfoRS.RemoveAll();
			for(int i = 0; i < m_nBlockCtrlPtFlatInfoRSCnt; i++)
			{
				rs.serialize(ar, version, bSendNRecv);
				m_arrBlockCtrlPtFlatInfoRS.Add(rs);
			}
		}
	}

public:
	void Init(void)
	{
		CPKIDInherit::Init();
		m_nBlockCtrlPtFlatInfoRSCnt = 0;
		m_arrBlockCtrlPtFlatInfoRS.RemoveAll();
	}

private:
	int m_nBlockCtrlPtFlatInfoRSCnt;

public:
	CArray<CEcoBlockControlPointFlatInfoRecordSet, CEcoBlockControlPointFlatInfoRecordSet&> m_arrBlockCtrlPtFlatInfoRS;
};

class CPKEcoBlockCameraInfo : public CPKUserIDNIDInherit
{
public:
	CPKEcoBlockCameraInfo(void)
	{

	}
	~CPKEcoBlockCameraInfo(void)
	{

	}

	CPKEcoBlockCameraInfo& operator = (const CPKEcoBlockCameraInfo &other)
	{
		CPKUserIDNIDInherit::operator=(other);
		m_arrBlockCameraInfoRS.Copy(other.m_arrBlockCameraInfoRS);
		return *this;
	}

	CPKEcoBlockCameraInfo( const CPKEcoBlockCameraInfo &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKUserIDNIDInherit::serialize(ar, version, bSendNRecv);
		if(bSendNRecv == TRUE)
		{
			m_nBlockCameraInfoRSCnt = m_arrBlockCameraInfoRS.GetCount();
			ar & m_nBlockCameraInfoRSCnt;
			for(int i = 0; i < m_nBlockCameraInfoRSCnt; i++)
				m_arrBlockCameraInfoRS.GetAt(i).serialize(ar, version, bSendNRecv);
		}
		else
		{
			ar & m_nBlockCameraInfoRSCnt;
			CEcoBlockCameraInfoRecordSet rs; m_arrBlockCameraInfoRS.RemoveAll();
			for(int i = 0; i < m_nBlockCameraInfoRSCnt; i++)
			{
				rs.serialize(ar, version, bSendNRecv);
				m_arrBlockCameraInfoRS.Add(rs);
			}
		}
	}

public:
	void Init(void)
	{
		CPKUserIDNIDInherit::Init();
		m_nBlockCameraInfoRSCnt = 0;
		m_arrBlockCameraInfoRS.RemoveAll();
	}

private:
	int m_nBlockCameraInfoRSCnt;

public:
	CArray<CEcoBlockCameraInfoRecordSet, CEcoBlockCameraInfoRecordSet&> m_arrBlockCameraInfoRS;
};

class CPKEcoBlockProjectionFaceList : public CPKUserIDNIDInherit
{
public:
	CPKEcoBlockProjectionFaceList(void)
	{

	}
	~CPKEcoBlockProjectionFaceList(void)
	{

	}

	CPKEcoBlockProjectionFaceList& operator = (const CPKEcoBlockProjectionFaceList &other)
	{
		CPKUserIDNIDInherit::operator=(other);
		m_arrBlockProjectionFaceListRS.Copy(other.m_arrBlockProjectionFaceListRS);
		return *this;
	}

	CPKEcoBlockProjectionFaceList( const CPKEcoBlockProjectionFaceList &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKUserIDNIDInherit::serialize(ar, version, bSendNRecv);
		if(bSendNRecv == TRUE)
		{
			m_nBlockProjectionFaceListRSCnt = m_arrBlockProjectionFaceListRS.GetCount();
			ar & m_nBlockProjectionFaceListRSCnt;
			for(int i = 0; i < m_nBlockProjectionFaceListRSCnt; i++)
				m_arrBlockProjectionFaceListRS.GetAt(i).serialize(ar, version, bSendNRecv);
		}
		else
		{
			ar & m_nBlockProjectionFaceListRSCnt;
			CEcoBlockProjectionFaceListRecordSet rs; m_arrBlockProjectionFaceListRS.RemoveAll();
			for(int i = 0; i < m_nBlockProjectionFaceListRSCnt; i++)
			{
				rs.serialize(ar, version, bSendNRecv);
				m_arrBlockProjectionFaceListRS.Add(rs);
			}
		}
	}

public:
	void Init(void)
	{
		CPKUserIDNIDInherit::Init();
		m_nBlockProjectionFaceListRSCnt = 0;
		m_arrBlockProjectionFaceListRS.RemoveAll();
	}

private:
	int m_nBlockProjectionFaceListRSCnt;

public:
	CArray<CEcoBlockProjectionFaceListRecordSet, CEcoBlockProjectionFaceListRecordSet&> m_arrBlockProjectionFaceListRS;
};

class CPKEcoBlockAddMeasurePointInfo : public CPKIDInherit
{
public:
	CPKEcoBlockAddMeasurePointInfo(void)
	{

	}
	~CPKEcoBlockAddMeasurePointInfo(void)
	{

	}

	CPKEcoBlockAddMeasurePointInfo& operator = (const CPKEcoBlockAddMeasurePointInfo &other)
	{
		CPKIDInherit::operator=(other);
		m_arrBlockAddMsrPtRS.Copy(other.m_arrBlockAddMsrPtRS);
		return *this;
	}

	CPKEcoBlockAddMeasurePointInfo( const CPKEcoBlockAddMeasurePointInfo &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKIDInherit::serialize(ar, version, bSendNRecv);
		if(bSendNRecv == TRUE)
		{
			m_nBlockAddMsrPtRSCnt = m_arrBlockAddMsrPtRS.GetCount();
			ar & m_nBlockAddMsrPtRSCnt;
			for(int i = 0; i < m_nBlockAddMsrPtRSCnt; i++)
				m_arrBlockAddMsrPtRS.GetAt(i).serialize(ar, version, bSendNRecv);
		}
		else
		{
			ar & m_nBlockAddMsrPtRSCnt;
			CEcoBlockAddMeasurePointInfoRecordSet rs; m_arrBlockAddMsrPtRS.RemoveAll();
			for(int i = 0; i < m_nBlockAddMsrPtRSCnt; i++)
			{
				rs.serialize(ar, version, bSendNRecv);
				m_arrBlockAddMsrPtRS.Add(rs);
			}
		}
	}

public:
	void Init(void)
	{
		CPKIDInherit::Init();
		m_nBlockAddMsrPtRSCnt = 0;
		m_arrBlockAddMsrPtRS.RemoveAll();
	}

private:
	int m_nBlockAddMsrPtRSCnt;

public:
	CArray<CEcoBlockAddMeasurePointInfoRecordSet, CEcoBlockAddMeasurePointInfoRecordSet&> m_arrBlockAddMsrPtRS;
};

class CPKEcoBlockDeleteMeasurePointInfo : public CPKIDInherit
{
public:
	CPKEcoBlockDeleteMeasurePointInfo(void)
	{

	}
	~CPKEcoBlockDeleteMeasurePointInfo(void)
	{

	}

	CPKEcoBlockDeleteMeasurePointInfo& operator = (const CPKEcoBlockDeleteMeasurePointInfo &other)
	{
		CPKIDInherit::operator=(other);
		m_arrBlockDelMsrPtRS.Copy(other.m_arrBlockDelMsrPtRS);
		return *this;
	}

	CPKEcoBlockDeleteMeasurePointInfo( const CPKEcoBlockDeleteMeasurePointInfo &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKIDInherit::serialize(ar, version, bSendNRecv);
		if(bSendNRecv == TRUE)
		{
			m_nBlockDelMsrPtRSCnt = m_arrBlockDelMsrPtRS.GetCount();
			ar & m_nBlockDelMsrPtRSCnt;
			for(int i = 0; i < m_nBlockDelMsrPtRSCnt; i++)
				m_arrBlockDelMsrPtRS.GetAt(i).serialize(ar, version, bSendNRecv);
		}
		else
		{
			ar & m_nBlockDelMsrPtRSCnt;
			CEcoBlockDeleteMeasurePointInfoRecordSet rs; m_arrBlockDelMsrPtRS.RemoveAll();
			for(int i = 0; i < m_nBlockDelMsrPtRSCnt; i++)
			{
				rs.serialize(ar, version, bSendNRecv);
				m_arrBlockDelMsrPtRS.Add(rs);
			}
		}
	}

public:
	void Init(void)
	{
		CPKIDInherit::Init();
		m_nBlockDelMsrPtRSCnt = 0;
		m_arrBlockDelMsrPtRS.RemoveAll();
	}

private:
	int m_nBlockDelMsrPtRSCnt;

public:
	CArray<CEcoBlockDeleteMeasurePointInfoRecordSet, CEcoBlockDeleteMeasurePointInfoRecordSet&> m_arrBlockDelMsrPtRS;
};

class CPKSystemDrawing : public CPKWhereInherit
{
public:
	CPKSystemDrawing(void)
	{

	}
	~CPKSystemDrawing(void)
	{

	}

	CPKSystemDrawing& operator = (const CPKSystemDrawing &other)
	{
		CPKWhereInherit::operator=(other);
		m_strDrawingName = other.m_strDrawingName;
		m_arrSystemDrawingRS.Copy(other.m_arrSystemDrawingRS);
		return *this;
	}

	CPKSystemDrawing( const CPKSystemDrawing &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKWhereInherit::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_strDrawingName);
			ar & stdStr;

			m_nSystemDrawingRSCnt = m_arrSystemDrawingRS.GetCount();
			ar & m_nSystemDrawingRSCnt;
			for(int i = 0; i < m_nSystemDrawingRSCnt; i++)
				m_arrSystemDrawingRS.GetAt(i).serialize(ar, version, bSendNRecv);
		}
		else
		{
			ar & stdStr;
			m_strDrawingName = CStringConverter::CStringAToCStringW(stdStr.c_str());

			ar & m_nSystemDrawingRSCnt;
			CSystemDrawingRecordSet rs; m_arrSystemDrawingRS.RemoveAll();
			for(int i = 0; i < m_nSystemDrawingRSCnt; i++)
			{
				rs.serialize(ar, version, bSendNRecv);
				m_arrSystemDrawingRS.Add(rs);
			}
		}
	}

public:
	void Init(void)
	{
		CPKWhereInherit::Init();
		m_strDrawingName = _T("");
		m_nSystemDrawingRSCnt = 0;
		m_arrSystemDrawingRS.RemoveAll();
	}

private:
	int m_nSystemDrawingRSCnt;

public:
	CString m_strDrawingName;
	CArray<CSystemDrawingRecordSet, CSystemDrawingRecordSet&> m_arrSystemDrawingRS;
};

class CPKSystemBlockList : public CPKWhereInherit
{
public:
	CPKSystemBlockList(void)
	{

	}
	~CPKSystemBlockList(void)
	{

	}

	CPKSystemBlockList& operator = (const CPKSystemBlockList &other)
	{
		CPKWhereInherit::operator=(other);
		m_strBlockName = other.m_strBlockName;
		m_strPreBlockName = other.m_strPreBlockName;
		m_strNewBlockName = other.m_strNewBlockName;
		m_arrBlockName.Copy(other.m_arrBlockName);
		m_arrSystemBlockListRS.Copy(other.m_arrSystemBlockListRS);
		return *this;
	}

	CPKSystemBlockList( const CPKSystemBlockList &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKWhereInherit::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_strBlockName);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_strPreBlockName);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_strNewBlockName);
			ar & stdStr;

			m_nBlockNameCnt = m_arrBlockName.GetCount();
			ar & m_nBlockNameCnt;
			for(int i = 0; i < m_nBlockNameCnt; i++)
			{
				stdStr = CStringConverter::CStringWToCStringA(m_arrBlockName.GetAt(i));
				ar & stdStr;
			}

			m_nSystemBlockListRSCnt = m_arrSystemBlockListRS.GetCount();
			ar & m_nSystemBlockListRSCnt;
			for(int i = 0; i < m_nSystemBlockListRSCnt; i++)
				m_arrSystemBlockListRS.GetAt(i).serialize(ar, version, bSendNRecv);
		}
		else
		{
			ar & stdStr;
			m_strBlockName = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_strPreBlockName = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_strNewBlockName = CStringConverter::CStringAToCStringW(stdStr.c_str());

			ar & m_nBlockNameCnt;
			CString str; m_arrBlockName.RemoveAll();
			for(int i = 0; i < m_nBlockNameCnt; i++)
			{
				ar & stdStr;
				str = CStringConverter::CStringAToCStringW(stdStr.c_str());
				m_arrBlockName.Add(str);
			}

			ar & m_nSystemBlockListRSCnt;
			CSystemBlockListRecordSet rs; m_arrSystemBlockListRS.RemoveAll();
			for(int i = 0; i < m_nSystemBlockListRSCnt; i++)
			{
				rs.serialize(ar, version, bSendNRecv);
				m_arrSystemBlockListRS.Add(rs);
			}
		}
	}

public:
	void Init(void)
	{
		CPKWhereInherit::Init();
		m_strBlockName = _T("");
		m_strPreBlockName = _T("");
		m_strNewBlockName = _T("");
		m_nBlockNameCnt = 0;
		m_arrBlockName.RemoveAll();
		m_nSystemBlockListRSCnt = 0;
		m_arrSystemBlockListRS.RemoveAll();
	}

private:
	int m_nBlockNameCnt;
	int m_nSystemBlockListRSCnt;

public:
	CString m_strBlockName;
	CString m_strPreBlockName;
	CString m_strNewBlockName;
	CArray<CString, CString&> m_arrBlockName;
	CArray<CSystemBlockListRecordSet, CSystemBlockListRecordSet&> m_arrSystemBlockListRS;
};

/////////////////////////////////////////////////////////////////////////
//EcoOTS 프로젝트 관련 Packet들

class CPKEcoOtsProject : public CPKUserIDNIDInherit
{
public:
	CPKEcoOtsProject(void)
	{

	}
	~CPKEcoOtsProject(void)
	{

	}

	CPKEcoOtsProject& operator = (const CPKEcoOtsProject &other)
	{
		CPKUserIDNIDInherit::operator=(other);
		m_arrOtsProjectRS.Copy(other.m_arrOtsProjectRS);
		return *this;
	}

	CPKEcoOtsProject( const CPKEcoOtsProject &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKUserIDNIDInherit::serialize(ar, version, bSendNRecv);
		if(bSendNRecv == TRUE)
		{
			m_nOtsProjectRSCnt = m_arrOtsProjectRS.GetCount();
			ar & m_nOtsProjectRSCnt;
			for(int i = 0; i < m_nOtsProjectRSCnt; i++)
				m_arrOtsProjectRS.GetAt(i).serialize(ar, version, bSendNRecv);
		}
		else
		{
			ar & m_nOtsProjectRSCnt;
			CEcoOtsProjectRecordSet rs; m_arrOtsProjectRS.RemoveAll();
			for(int i = 0; i < m_nOtsProjectRSCnt; i++)
			{
				rs.serialize(ar, version, bSendNRecv);
				m_arrOtsProjectRS.Add(rs);
			}
		}
	}

public:
	void Init(void)
	{
		CPKUserIDNIDInherit::Init();
		m_nOtsProjectRSCnt = 0;
		m_arrOtsProjectRS.RemoveAll();
	}

private:
	int m_nOtsProjectRSCnt;

public:
	CArray<CEcoOtsProjectRecordSet, CEcoOtsProjectRecordSet&> m_arrOtsProjectRS;
};

class CPKEcoOtsBlockList : public CPKUserIDNIDInherit
{
public:
	CPKEcoOtsBlockList(void)
	{

	}
	~CPKEcoOtsBlockList(void)
	{

	}

	CPKEcoOtsBlockList& operator = (const CPKEcoOtsBlockList &other)
	{
		CPKUserIDNIDInherit::operator=(other);
		m_arrOtsBlockListRS.Copy(other.m_arrOtsBlockListRS);
		return *this;
	}

	CPKEcoOtsBlockList( const CPKEcoOtsBlockList &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKUserIDNIDInherit::serialize(ar, version, bSendNRecv);
		if(bSendNRecv == TRUE)
		{
			m_nOtsBlockListRSCnt = m_arrOtsBlockListRS.GetCount();
			ar & m_nOtsBlockListRSCnt;
			for(int i = 0; i < m_nOtsBlockListRSCnt; i++)
				m_arrOtsBlockListRS.GetAt(i).serialize(ar, version, bSendNRecv);
		}
		else
		{
			ar & m_nOtsBlockListRSCnt;
			CEcoOtsBlockListRecordSet rs; m_arrOtsBlockListRS.RemoveAll();
			for(int i = 0; i < m_nOtsBlockListRSCnt; i++)
			{
				rs.serialize(ar, version, bSendNRecv);
				m_arrOtsBlockListRS.Add(rs);
			}
		}
	}

public:
	void Init(void)
	{
		CPKUserIDNIDInherit::Init();
		m_nOtsBlockListRSCnt = 0;
		m_arrOtsBlockListRS.RemoveAll();
	}

private:
	int m_nOtsBlockListRSCnt;

public:
	CArray<CEcoOtsBlockListRecordSet, CEcoOtsBlockListRecordSet&> m_arrOtsBlockListRS;
};

class CPKEcoOtsAddMeasurePointInfo : public CPKIDInherit
{
public:
	CPKEcoOtsAddMeasurePointInfo(void)
	{

	}
	~CPKEcoOtsAddMeasurePointInfo(void)
	{

	}

	CPKEcoOtsAddMeasurePointInfo& operator = (const CPKEcoOtsAddMeasurePointInfo &other)
	{
		CPKIDInherit::operator=(other);
		m_arrOtsAddMsrPtRS.Copy(other.m_arrOtsAddMsrPtRS);
		return *this;
	}

	CPKEcoOtsAddMeasurePointInfo( const CPKEcoOtsAddMeasurePointInfo &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKIDInherit::serialize(ar, version, bSendNRecv);
		if(bSendNRecv == TRUE)
		{
			m_nOtsAddMsrPtRSCnt = m_arrOtsAddMsrPtRS.GetCount();
			ar & m_nOtsAddMsrPtRSCnt;
			for(int i = 0; i < m_nOtsAddMsrPtRSCnt; i++)
				m_arrOtsAddMsrPtRS.GetAt(i).serialize(ar, version, bSendNRecv);
		}
		else
		{
			ar & m_nOtsAddMsrPtRSCnt;
			CEcoOtsAddMeasurePointInfoRecordSet rs; m_arrOtsAddMsrPtRS.RemoveAll();
			for(int i = 0; i < m_nOtsAddMsrPtRSCnt; i++)
			{
				rs.serialize(ar, version, bSendNRecv);
				m_arrOtsAddMsrPtRS.Add(rs);
			}
		}
	}

public:
	void Init(void)
	{
		CPKIDInherit::Init();
		m_nOtsAddMsrPtRSCnt = 0;
		m_arrOtsAddMsrPtRS.RemoveAll();
	}

private:
	int m_nOtsAddMsrPtRSCnt;

public:
	CArray<CEcoOtsAddMeasurePointInfoRecordSet, CEcoOtsAddMeasurePointInfoRecordSet&> m_arrOtsAddMsrPtRS;
};

class CPKEcoOtsDeleteMeasurePointInfo : public CPKIDInherit
{
public:
	CPKEcoOtsDeleteMeasurePointInfo(void)
	{

	}
	~CPKEcoOtsDeleteMeasurePointInfo(void)
	{

	}

	CPKEcoOtsDeleteMeasurePointInfo& operator = (const CPKEcoOtsDeleteMeasurePointInfo &other)
	{
		CPKIDInherit::operator=(other);
		m_arrOtsDelMsrPtRS.Copy(other.m_arrOtsDelMsrPtRS);
		return *this;
	}

	CPKEcoOtsDeleteMeasurePointInfo( const CPKEcoOtsDeleteMeasurePointInfo &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKIDInherit::serialize(ar, version, bSendNRecv);
		if(bSendNRecv == TRUE)
		{
			m_nOtsDelMsrPtRSCnt = m_arrOtsDelMsrPtRS.GetCount();
			ar & m_nOtsDelMsrPtRSCnt;
			for(int i = 0; i < m_nOtsDelMsrPtRSCnt; i++)
				m_arrOtsDelMsrPtRS.GetAt(i).serialize(ar, version, bSendNRecv);
		}
		else
		{
			ar & m_nOtsDelMsrPtRSCnt;
			CEcoOtsDeleteMeasurePointInfoRecordSet rs; m_arrOtsDelMsrPtRS.RemoveAll();
			for(int i = 0; i < m_nOtsDelMsrPtRSCnt; i++)
			{
				rs.serialize(ar, version, bSendNRecv);
				m_arrOtsDelMsrPtRS.Add(rs);
			}
		}
	}

public:
	void Init(void)
	{
		CPKIDInherit::Init();
		m_nOtsDelMsrPtRSCnt = 0;
		m_arrOtsDelMsrPtRS.RemoveAll();
	}

private:
	int m_nOtsDelMsrPtRSCnt;

public:
	CArray<CEcoOtsDeleteMeasurePointInfoRecordSet, CEcoOtsDeleteMeasurePointInfoRecordSet&> m_arrOtsDelMsrPtRS;
};

class CPKEcoOtsDesignPointGeneralInfo : public CPKIDInherit
{
public:
	CPKEcoOtsDesignPointGeneralInfo(void)
	{

	}
	~CPKEcoOtsDesignPointGeneralInfo(void)
	{

	}

	CPKEcoOtsDesignPointGeneralInfo& operator = (const CPKEcoOtsDesignPointGeneralInfo &other)
	{
		CPKIDInherit::operator=(other);
		m_arrOtsDsnPtGeneralInfoRS.Copy(other.m_arrOtsDsnPtGeneralInfoRS);
		return *this;
	}

	CPKEcoOtsDesignPointGeneralInfo( const CPKEcoOtsDesignPointGeneralInfo &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKIDInherit::serialize(ar, version, bSendNRecv);
		if(bSendNRecv == TRUE)
		{
			m_nOtsDsnPtGeneralInfoRSCnt = m_arrOtsDsnPtGeneralInfoRS.GetCount();
			ar & m_nOtsDsnPtGeneralInfoRSCnt;
			for(int i = 0; i < m_nOtsDsnPtGeneralInfoRSCnt; i++)
				m_arrOtsDsnPtGeneralInfoRS.GetAt(i).serialize(ar, version, bSendNRecv);
		}
		else
		{
			ar & m_nOtsDsnPtGeneralInfoRSCnt;
			CEcoOtsDesignPointGeneralInfoRecordSet rs; m_arrOtsDsnPtGeneralInfoRS.RemoveAll();
			for(int i = 0; i < m_nOtsDsnPtGeneralInfoRSCnt; i++)
			{
				rs.serialize(ar, version, bSendNRecv);
				m_arrOtsDsnPtGeneralInfoRS.Add(rs);
			}
		}
	}

public:
	void Init(void)
	{
		CPKIDInherit::Init();
		m_nOtsDsnPtGeneralInfoRSCnt = 0;
		m_arrOtsDsnPtGeneralInfoRS.RemoveAll();
	}

private:
	int m_nOtsDsnPtGeneralInfoRSCnt;

public:
	CArray<CEcoOtsDesignPointGeneralInfoRecordSet, CEcoOtsDesignPointGeneralInfoRecordSet&> m_arrOtsDsnPtGeneralInfoRS;
};

class CPKEcoOtsRelationByDesMsr : public CPKIDInherit
{
public:
	CPKEcoOtsRelationByDesMsr(void)
	{

	}
	~CPKEcoOtsRelationByDesMsr(void)
	{

	}

	CPKEcoOtsRelationByDesMsr& operator = (const CPKEcoOtsRelationByDesMsr &other)
	{
		CPKIDInherit::operator=(other);
		m_arrOtsRelaByDesMsrRS.Copy(other.m_arrOtsRelaByDesMsrRS);
		return *this;
	}

	CPKEcoOtsRelationByDesMsr( const CPKEcoOtsRelationByDesMsr &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKIDInherit::serialize(ar, version, bSendNRecv);
		if(bSendNRecv == TRUE)
		{
			m_nOtsRelaByDesMsrRSCnt = m_arrOtsRelaByDesMsrRS.GetCount();
			ar & m_nOtsRelaByDesMsrRSCnt;
			for(int i = 0; i < m_nOtsRelaByDesMsrRSCnt; i++)
				m_arrOtsRelaByDesMsrRS.GetAt(i).serialize(ar, version, bSendNRecv);
		}
		else
		{
			ar & m_nOtsRelaByDesMsrRSCnt;
			CEcoOtsRelationByDesMsrRecordSet rs; m_arrOtsRelaByDesMsrRS.RemoveAll();
			for(int i = 0; i < m_nOtsRelaByDesMsrRSCnt; i++)
			{
				rs.serialize(ar, version, bSendNRecv);
				m_arrOtsRelaByDesMsrRS.Add(rs);
			}
		}
	}

public:
	void Init(void)
	{
		CPKIDInherit::Init();
		m_nOtsRelaByDesMsrRSCnt = 0;
		m_arrOtsRelaByDesMsrRS.RemoveAll();
	}

private:
	int m_nOtsRelaByDesMsrRSCnt;

public:
	CArray<CEcoOtsRelationByDesMsrRecordSet, CEcoOtsRelationByDesMsrRecordSet&> m_arrOtsRelaByDesMsrRS;
};

class CPKEcoOtsRelationByMsrMsr : public CPKIDInherit
{
public:
	CPKEcoOtsRelationByMsrMsr(void)
	{

	}
	~CPKEcoOtsRelationByMsrMsr(void)
	{

	}

	CPKEcoOtsRelationByMsrMsr& operator = (const CPKEcoOtsRelationByMsrMsr &other)
	{
		CPKIDInherit::operator=(other);
		m_arrOtsRelaByMsrMsrRS.Copy(other.m_arrOtsRelaByMsrMsrRS);
		return *this;
	}

	CPKEcoOtsRelationByMsrMsr( const CPKEcoOtsRelationByMsrMsr &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKIDInherit::serialize(ar, version, bSendNRecv);
		if(bSendNRecv == TRUE)
		{
			m_nOtsRelaByMsrMsrRSCnt = m_arrOtsRelaByMsrMsrRS.GetCount();
			ar & m_nOtsRelaByMsrMsrRSCnt;
			for(int i = 0; i < m_nOtsRelaByMsrMsrRSCnt; i++)
				m_arrOtsRelaByMsrMsrRS.GetAt(i).serialize(ar, version, bSendNRecv);
		}
		else
		{
			ar & m_nOtsRelaByMsrMsrRSCnt;
			CEcoOtsRelationByMsrMsrRecordSet rs; m_arrOtsRelaByMsrMsrRS.RemoveAll();
			for(int i = 0; i < m_nOtsRelaByMsrMsrRSCnt; i++)
			{
				rs.serialize(ar, version, bSendNRecv);
				m_arrOtsRelaByMsrMsrRS.Add(rs);
			}
		}
	}

public:
	void Init(void)
	{
		CPKIDInherit::Init();
		m_nOtsRelaByMsrMsrRSCnt = 0;
		m_arrOtsRelaByMsrMsrRS.RemoveAll();
	}

private:
	int m_nOtsRelaByMsrMsrRSCnt;

public:
	CArray<CEcoOtsRelationByMsrMsrRecordSet, CEcoOtsRelationByMsrMsrRecordSet&> m_arrOtsRelaByMsrMsrRS;
};

class CPKEcoOtsDistanceCond : public CPKIDInherit
{
public:
	CPKEcoOtsDistanceCond(void)
	{

	}
	~CPKEcoOtsDistanceCond(void)
	{

	}

	CPKEcoOtsDistanceCond& operator = (const CPKEcoOtsDistanceCond &other)
	{
		CPKIDInherit::operator=(other);
		m_arrOtsDistCondRS.Copy(other.m_arrOtsDistCondRS);
		return *this;
	}

	CPKEcoOtsDistanceCond( const CPKEcoOtsDistanceCond &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKIDInherit::serialize(ar, version, bSendNRecv);
		if(bSendNRecv == TRUE)
		{
			m_nOtsDistCondRSCnt = m_arrOtsDistCondRS.GetCount();
			ar & m_nOtsDistCondRSCnt;
			for(int i = 0; i < m_nOtsDistCondRSCnt; i++)
				m_arrOtsDistCondRS.GetAt(i).serialize(ar, version, bSendNRecv);
		}
		else
		{
			ar & m_nOtsDistCondRSCnt;
			CEcoOtsDistanceCondRecordSet rs; m_arrOtsDistCondRS.RemoveAll();
			for(int i = 0; i < m_nOtsDistCondRSCnt; i++)
			{
				rs.serialize(ar, version, bSendNRecv);
				m_arrOtsDistCondRS.Add(rs);
			}
		}
	}

public:
	void Init(void)
	{
		CPKIDInherit::Init();
		m_nOtsDistCondRSCnt = 0;
		m_arrOtsDistCondRS.RemoveAll();
	}

private:
	int m_nOtsDistCondRSCnt;

public:
	CArray<CEcoOtsDistanceCondRecordSet, CEcoOtsDistanceCondRecordSet&> m_arrOtsDistCondRS;
};

class CPKEcoOtsPointCond : public CPKIDInherit
{
public:
	CPKEcoOtsPointCond(void)
	{

	}
	~CPKEcoOtsPointCond(void)
	{

	}

	CPKEcoOtsPointCond& operator = (const CPKEcoOtsPointCond &other)
	{
		CPKIDInherit::operator=(other);
		m_arrOtsPointCondRS.Copy(other.m_arrOtsPointCondRS);
		return *this;
	}

	CPKEcoOtsPointCond( const CPKEcoOtsPointCond &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKIDInherit::serialize(ar, version, bSendNRecv);
		if(bSendNRecv == TRUE)
		{
			m_nOtsPointCondRSCnt = m_arrOtsPointCondRS.GetCount();
			ar & m_nOtsPointCondRSCnt;
			for(int i = 0; i < m_nOtsPointCondRSCnt; i++)
				m_arrOtsPointCondRS.GetAt(i).serialize(ar, version, bSendNRecv);
		}
		else
		{
			ar & m_nOtsPointCondRSCnt;
			CEcoOtsPointCondRecordSet rs; m_arrOtsPointCondRS.RemoveAll();
			for(int i = 0; i < m_nOtsPointCondRSCnt; i++)
			{
				rs.serialize(ar, version, bSendNRecv);
				m_arrOtsPointCondRS.Add(rs);
			}
		}
	}

public:
	void Init(void)
	{
		CPKIDInherit::Init();
		m_nOtsPointCondRSCnt = 0;
		m_arrOtsPointCondRS.RemoveAll();
	}

private:
	int m_nOtsPointCondRSCnt;

public:
	CArray<CEcoOtsPointCondRecordSet, CEcoOtsPointCondRecordSet&> m_arrOtsPointCondRS;
};

class CPKEcoOtsDrawingElement : public CPKUserIDNIDInherit
{
public:
	CPKEcoOtsDrawingElement(void)
	{

	}
	~CPKEcoOtsDrawingElement(void)
	{

	}

	CPKEcoOtsDrawingElement& operator = (const CPKEcoOtsDrawingElement &other)
	{
		CPKUserIDNIDInherit::operator=(other);
		m_arrOtsDrawingElementRS.Copy(other.m_arrOtsDrawingElementRS);
		return *this;
	}

	CPKEcoOtsDrawingElement( const CPKEcoOtsDrawingElement &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKUserIDNIDInherit::serialize(ar, version, bSendNRecv);
		if(bSendNRecv == TRUE)
		{
			m_nOtsDrawingElementRSCnt = m_arrOtsDrawingElementRS.GetCount();
			ar & m_nOtsDrawingElementRSCnt;
			for(int i = 0; i < m_nOtsDrawingElementRSCnt; i++)
				m_arrOtsDrawingElementRS.GetAt(i).serialize(ar, version, bSendNRecv);
		}
		else
		{
			ar & m_nOtsDrawingElementRSCnt;
			CEcoOtsDrawingElementRecordSet rs; m_arrOtsDrawingElementRS.RemoveAll();
			for(int i = 0; i < m_nOtsDrawingElementRSCnt; i++)
			{
				rs.serialize(ar, version, bSendNRecv);
				m_arrOtsDrawingElementRS.Add(rs);
			}
		}
	}

public:
	void Init(void)
	{
		CPKUserIDNIDInherit::Init();
		m_nOtsDrawingElementRSCnt = 0;
		m_arrOtsDrawingElementRS.RemoveAll();
	}

private:
	int m_nOtsDrawingElementRSCnt;

public:
	CArray<CEcoOtsDrawingElementRecordSet, CEcoOtsDrawingElementRecordSet&> m_arrOtsDrawingElementRS;
};

class CPKEcoOtsProjectionFaceList : public CPKUserIDNIDInherit
{
public:
	CPKEcoOtsProjectionFaceList(void)
	{

	}
	~CPKEcoOtsProjectionFaceList(void)
	{

	}

	CPKEcoOtsProjectionFaceList& operator = (const CPKEcoOtsProjectionFaceList &other)
	{
		CPKUserIDNIDInherit::operator=(other);
		m_arrOtsProjectionFaceListRS.Copy(other.m_arrOtsProjectionFaceListRS);
		return *this;
	}

	CPKEcoOtsProjectionFaceList( const CPKEcoOtsProjectionFaceList &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKUserIDNIDInherit::serialize(ar, version, bSendNRecv);
		if(bSendNRecv == TRUE)
		{
			m_nOtsProjectionFaceListRSCnt = m_arrOtsProjectionFaceListRS.GetCount();
			ar & m_nOtsProjectionFaceListRSCnt;
			for(int i = 0; i < m_nOtsProjectionFaceListRSCnt; i++)
				m_arrOtsProjectionFaceListRS.GetAt(i).serialize(ar, version, bSendNRecv);
		}
		else
		{
			ar & m_nOtsProjectionFaceListRSCnt;
			CEcoOtsProjectionFaceListRecordSet rs; m_arrOtsProjectionFaceListRS.RemoveAll();
			for(int i = 0; i < m_nOtsProjectionFaceListRSCnt; i++)
			{
				rs.serialize(ar, version, bSendNRecv);
				m_arrOtsProjectionFaceListRS.Add(rs);
			}
		}
	}

public:
	void Init(void)
	{
		CPKUserIDNIDInherit::Init();
		m_nOtsProjectionFaceListRSCnt = 0;
		m_arrOtsProjectionFaceListRS.RemoveAll();
	}

private:
	int m_nOtsProjectionFaceListRSCnt;

public:
	CArray<CEcoOtsProjectionFaceListRecordSet, CEcoOtsProjectionFaceListRecordSet&> m_arrOtsProjectionFaceListRS;
};

class CPKEcoOtsCameraInfo : public CPKUserIDNIDInherit
{
public:
	CPKEcoOtsCameraInfo(void)
	{

	}
	~CPKEcoOtsCameraInfo(void)
	{

	}

	CPKEcoOtsCameraInfo& operator = (const CPKEcoOtsCameraInfo &other)
	{
		CPKUserIDNIDInherit::operator=(other);
		m_arrOtsCameraInfoRS.Copy(other.m_arrOtsCameraInfoRS);
		return *this;
	}

	CPKEcoOtsCameraInfo( const CPKEcoOtsCameraInfo &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKUserIDNIDInherit::serialize(ar, version, bSendNRecv);
		if(bSendNRecv == TRUE)
		{
			m_nOtsCameraInfoRSCnt = m_arrOtsCameraInfoRS.GetCount();
			ar & m_nOtsCameraInfoRSCnt;
			for(int i = 0; i < m_nOtsCameraInfoRSCnt; i++)
				m_arrOtsCameraInfoRS.GetAt(i).serialize(ar, version, bSendNRecv);
		}
		else
		{
			ar & m_nOtsCameraInfoRSCnt;
			CEcoOtsCameraInfoRecordSet rs; m_arrOtsCameraInfoRS.RemoveAll();
			for(int i = 0; i < m_nOtsCameraInfoRSCnt; i++)
			{
				rs.serialize(ar, version, bSendNRecv);
				m_arrOtsCameraInfoRS.Add(rs);
			}
		}
	}

public:
	void Init(void)
	{
		CPKUserIDNIDInherit::Init();
		m_nOtsCameraInfoRSCnt = 0;
		m_arrOtsCameraInfoRS.RemoveAll();
	}

private:
	int m_nOtsCameraInfoRSCnt;

public:
	CArray<CEcoOtsCameraInfoRecordSet, CEcoOtsCameraInfoRecordSet&> m_arrOtsCameraInfoRS;
};

class CPKEcoOtsProjectionFaceModelingMatrixList : public CPKUserIDNIDInherit
{
public:
	CPKEcoOtsProjectionFaceModelingMatrixList(void)
	{

	}
	~CPKEcoOtsProjectionFaceModelingMatrixList(void)
	{

	}

	CPKEcoOtsProjectionFaceModelingMatrixList& operator = (const CPKEcoOtsProjectionFaceModelingMatrixList &other)
	{
		CPKUserIDNIDInherit::operator=(other);
		m_arrOtsProjectionFaceMMListRS.Copy(other.m_arrOtsProjectionFaceMMListRS);
		return *this;
	}

	CPKEcoOtsProjectionFaceModelingMatrixList( const CPKEcoOtsProjectionFaceModelingMatrixList &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKUserIDNIDInherit::serialize(ar, version, bSendNRecv);
		if(bSendNRecv == TRUE)
		{
			m_nOtsProjectionFaceMMListRSCnt = m_arrOtsProjectionFaceMMListRS.GetCount();
			ar & m_nOtsProjectionFaceMMListRSCnt;
			for(int i = 0; i < m_nOtsProjectionFaceMMListRSCnt; i++)
				m_arrOtsProjectionFaceMMListRS.GetAt(i).serialize(ar, version, bSendNRecv);
		}
		else
		{
			ar & m_nOtsProjectionFaceMMListRSCnt;
			CEcoOtsProjectionFaceModelingMatrixListRecordSet rs; m_arrOtsProjectionFaceMMListRS.RemoveAll();
			for(int i = 0; i < m_nOtsProjectionFaceMMListRSCnt; i++)
			{
				rs.serialize(ar, version, bSendNRecv);
				m_arrOtsProjectionFaceMMListRS.Add(rs);
			}
		}
	}

public:
	void Init(void)
	{
		CPKUserIDNIDInherit::Init();
		m_nOtsProjectionFaceMMListRSCnt = 0;
		m_arrOtsProjectionFaceMMListRS.RemoveAll();
	}

private:
	int m_nOtsProjectionFaceMMListRSCnt;

public:
	CArray<CEcoOtsProjectionFaceModelingMatrixListRecordSet, CEcoOtsProjectionFaceModelingMatrixListRecordSet&> m_arrOtsProjectionFaceMMListRS;
};

/////////////////////////////////////////////////////////////////////////
//Production관련 Packet들

class CPKAssemblyLevel : public CPKIDInherit
{
public:
	CPKAssemblyLevel(void)
	{

	}
	~CPKAssemblyLevel(void)
	{

	}

	CPKAssemblyLevel& operator = (const CPKAssemblyLevel &other)
	{
		CPKIDInherit::operator=(other);
		m_arrAssemLevelRS.Copy(other.m_arrAssemLevelRS);
		return *this;
	}

	CPKAssemblyLevel( const CPKAssemblyLevel &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKIDInherit::serialize(ar, version, bSendNRecv);
		if(bSendNRecv == TRUE)
		{
			m_nAssemLevelRSCnt = m_arrAssemLevelRS.GetCount();
			ar & m_nAssemLevelRSCnt;
			for(int i = 0; i < m_nAssemLevelRSCnt; i++)
				m_arrAssemLevelRS.GetAt(i).serialize(ar, version, bSendNRecv);
		}
		else
		{
			ar & m_nAssemLevelRSCnt;
			CAssemblyLevelRecordSet rs; m_arrAssemLevelRS.RemoveAll();
			for(int i = 0; i < m_nAssemLevelRSCnt; i++)
			{
				rs.serialize(ar, version, bSendNRecv);
				m_arrAssemLevelRS.Add(rs);
			}
		}
	}

public:
	void Init(void)
	{
		CPKIDInherit::Init();
		m_nAssemLevelRSCnt = 0;
		m_arrAssemLevelRS.RemoveAll();
	}

private:
	int m_nAssemLevelRSCnt;

public:
	CArray<CAssemblyLevelRecordSet, CAssemblyLevelRecordSet&> m_arrAssemLevelRS;
};

class CPKAssemblyTree : public CPKIDInherit
{
public:
	CPKAssemblyTree(void)
	{

	}
	~CPKAssemblyTree(void)
	{

	}

	CPKAssemblyTree& operator = (const CPKAssemblyTree &other)
	{
		CPKIDInherit::operator=(other);
		m_arrAssemTreeRS.Copy(other.m_arrAssemTreeRS);
		return *this;
	}

	CPKAssemblyTree( const CPKAssemblyTree &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKIDInherit::serialize(ar, version, bSendNRecv);
		if(bSendNRecv == TRUE)
		{
			m_nAssemTreeRSCnt = m_arrAssemTreeRS.GetCount();
			ar & m_nAssemTreeRSCnt;
			for(int i = 0; i < m_nAssemTreeRSCnt; i++)
				m_arrAssemTreeRS.GetAt(i).serialize(ar, version, bSendNRecv);
		}
		else
		{
			ar & m_nAssemTreeRSCnt;
			CAssemblyTreeRecordSet rs; m_arrAssemTreeRS.RemoveAll();
			for(int i = 0; i < m_nAssemTreeRSCnt; i++)
			{
				rs.serialize(ar, version, bSendNRecv);
				m_arrAssemTreeRS.Add(rs);
			}
		}
	}

public:
	void Init(void)
	{
		CPKIDInherit::Init();
		m_nAssemTreeRSCnt = 0;
		m_arrAssemTreeRS.RemoveAll();
	}

private:
	int m_nAssemTreeRSCnt;

public:
	CArray<CAssemblyTreeRecordSet, CAssemblyTreeRecordSet&> m_arrAssemTreeRS;
};

class CPKVariableOfGroup : public CPKIDInherit
{
public:
	CPKVariableOfGroup(void)
	{

	}
	~CPKVariableOfGroup(void)
	{

	}

	CPKVariableOfGroup& operator = (const CPKVariableOfGroup &other)
	{
		CPKIDInherit::operator=(other);
		m_arrVarOfGroupRS.Copy(other.m_arrVarOfGroupRS);
		return *this;
	}

	CPKVariableOfGroup( const CPKVariableOfGroup &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKIDInherit::serialize(ar, version, bSendNRecv);
		if(bSendNRecv == TRUE)
		{
			m_nVarOfGroupRSCnt = m_arrVarOfGroupRS.GetCount();
			ar & m_nVarOfGroupRSCnt;
			for(int i = 0; i < m_nVarOfGroupRSCnt; i++)
				m_arrVarOfGroupRS.GetAt(i).serialize(ar, version, bSendNRecv);
		}
		else
		{
			ar & m_nVarOfGroupRSCnt;
			CVariableOfGroupRecordSet rs; m_arrVarOfGroupRS.RemoveAll();
			for(int i = 0; i < m_nVarOfGroupRSCnt; i++)
			{
				rs.serialize(ar, version, bSendNRecv);
				m_arrVarOfGroupRS.Add(rs);
			}
		}
	}

public:
	void Init(void)
	{
		CPKIDInherit::Init();
		m_nVarOfGroupRSCnt = 0;
		m_arrVarOfGroupRS.RemoveAll();
	}

private:
	int m_nVarOfGroupRSCnt;

public:
	CArray<CVariableOfGroupRecordSet, CVariableOfGroupRecordSet&> m_arrVarOfGroupRS;
};

class CPKProjectProperty : public CPKIDInherit
{
public:
	CPKProjectProperty(void)
	{

	}
	~CPKProjectProperty(void)
	{

	}

	CPKProjectProperty& operator = (const CPKProjectProperty &other)
	{
		CPKIDInherit::operator=(other);
		m_arrPjtPropRS.Copy(other.m_arrPjtPropRS);
		return *this;
	}

	CPKProjectProperty( const CPKProjectProperty &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKIDInherit::serialize(ar, version, bSendNRecv);
		if(bSendNRecv == TRUE)
		{
			m_nPjtPropRSCnt = m_arrPjtPropRS.GetCount();
			ar & m_nPjtPropRSCnt;
			for(int i = 0; i < m_nPjtPropRSCnt; i++)
				m_arrPjtPropRS.GetAt(i).serialize(ar, version, bSendNRecv);
		}
		else
		{
			ar & m_nPjtPropRSCnt;
			CProjectPropertyRecordSet rs; m_arrPjtPropRS.RemoveAll();
			for(int i = 0; i < m_nPjtPropRSCnt; i++)
			{
				rs.serialize(ar, version, bSendNRecv);
				m_arrPjtPropRS.Add(rs);
			}
		}
	}

public:
	void Init(void)
	{
		CPKIDInherit::Init();
		m_nPjtPropRSCnt = 0;
		m_arrPjtPropRS.RemoveAll();
	}

private:
	int m_nPjtPropRSCnt;

public:
	CArray<CProjectPropertyRecordSet, CProjectPropertyRecordSet&> m_arrPjtPropRS;
};

class CPKShipNoProperty : public CPKIDInherit
{
public:
	CPKShipNoProperty(void)
	{

	}
	~CPKShipNoProperty(void)
	{

	}

	CPKShipNoProperty& operator = (const CPKShipNoProperty &other)
	{
		CPKIDInherit::operator=(other);
		m_arrShipNoPropRS.Copy(other.m_arrShipNoPropRS);
		return *this;
	}

	CPKShipNoProperty( const CPKShipNoProperty &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKIDInherit::serialize(ar, version, bSendNRecv);
		if(bSendNRecv == TRUE)
		{
			m_nShipNoPropRSCnt = m_arrShipNoPropRS.GetCount();
			ar & m_nShipNoPropRSCnt;
			for(int i = 0; i < m_nShipNoPropRSCnt; i++)
				m_arrShipNoPropRS.GetAt(i).serialize(ar, version, bSendNRecv);
		}
		else
		{
			ar & m_nShipNoPropRSCnt;
			CShipnoPropertyRecordSet rs; m_arrShipNoPropRS.RemoveAll();
			for(int i = 0; i < m_nShipNoPropRSCnt; i++)
			{
				rs.serialize(ar, version, bSendNRecv);
				m_arrShipNoPropRS.Add(rs);
			}
		}
	}

public:
	void Init(void)
	{
		CPKIDInherit::Init();
		m_nShipNoPropRSCnt = 0;
		m_arrShipNoPropRS.RemoveAll();
	}

private:
	int m_nShipNoPropRSCnt;

public:
	CArray<CShipnoPropertyRecordSet, CShipnoPropertyRecordSet&> m_arrShipNoPropRS;
};

class CPKVariableGroup : public CPKIDInherit
{
public:
	CPKVariableGroup(void)
	{

	}
	~CPKVariableGroup(void)
	{

	}

	CPKVariableGroup& operator = (const CPKVariableGroup &other)
	{
		CPKIDInherit::operator=(other);
		m_arrVarGroupRS.Copy(other.m_arrVarGroupRS);
		return *this;
	}

	CPKVariableGroup( const CPKVariableGroup &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKIDInherit::serialize(ar, version, bSendNRecv);
		if(bSendNRecv == TRUE)
		{
			m_nVarGroupRSCnt = m_arrVarGroupRS.GetCount();
			ar & m_nVarGroupRSCnt;
			for(int i = 0; i < m_nVarGroupRSCnt; i++)
				m_arrVarGroupRS.GetAt(i).serialize(ar, version, bSendNRecv);
		}
		else
		{
			ar & m_nVarGroupRSCnt;
			CVariableGroupRecordSet rs; m_arrVarGroupRS.RemoveAll();
			for(int i = 0; i < m_nVarGroupRSCnt; i++)
			{
				rs.serialize(ar, version, bSendNRecv);
				m_arrVarGroupRS.Add(rs);
			}
		}
	}

public:
	void Init(void)
	{
		CPKIDInherit::Init();
		m_nVarGroupRSCnt = 0;
		m_arrVarGroupRS.RemoveAll();
	}

private:
	int m_nVarGroupRSCnt;

public:
	CArray<CVariableGroupRecordSet, CVariableGroupRecordSet&> m_arrVarGroupRS;
};

class CPKUserVariable : public CPKIDInherit
{
public:
	CPKUserVariable(void)
	{

	}
	~CPKUserVariable(void)
	{

	}

	CPKUserVariable& operator = (const CPKUserVariable &other)
	{
		CPKIDInherit::operator=(other);
		m_arrUserVarRS.Copy(other.m_arrUserVarRS);
		return *this;
	}

	CPKUserVariable( const CPKUserVariable &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKIDInherit::serialize(ar, version, bSendNRecv);
		if(bSendNRecv == TRUE)
		{
			m_nUserVarRSCnt = m_arrUserVarRS.GetCount();
			ar & m_nUserVarRSCnt;
			for(int i = 0; i < m_nUserVarRSCnt; i++)
				m_arrUserVarRS.GetAt(i).serialize(ar, version, bSendNRecv);
		}
		else
		{
			ar & m_nUserVarRSCnt;
			CUserVariableRecordSet rs; m_arrUserVarRS.RemoveAll();
			for(int i = 0; i < m_nUserVarRSCnt; i++)
			{
				rs.serialize(ar, version, bSendNRecv);
				m_arrUserVarRS.Add(rs);
			}
		}
	}

public:
	void Init(void)
	{
		CPKIDInherit::Init();
		m_nUserVarRSCnt = 0;
		m_arrUserVarRS.RemoveAll();
	}

private:
	int m_nUserVarRSCnt;

public:
	CArray<CUserVariableRecordSet, CUserVariableRecordSet&> m_arrUserVarRS;
};

class CPKBlockProperty : public CPKIDInherit
{
public:
	CPKBlockProperty(void)
	{

	}
	~CPKBlockProperty(void)
	{

	}

	CPKBlockProperty& operator = (const CPKBlockProperty &other)
	{
		CPKIDInherit::operator=(other);
		m_arrBlockPropRS.Copy(other.m_arrBlockPropRS);
		return *this;
	}

	CPKBlockProperty( const CPKBlockProperty &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKIDInherit::serialize(ar, version, bSendNRecv);
		if(bSendNRecv == TRUE)
		{
			m_nBlockPropRSCnt = m_arrBlockPropRS.GetCount();
			ar & m_nBlockPropRSCnt;
			for(int i = 0; i < m_nBlockPropRSCnt; i++)
				m_arrBlockPropRS.GetAt(i).serialize(ar, version, bSendNRecv);
		}
		else
		{
			ar & m_nBlockPropRSCnt;
			CBlockPropertyRecordSet rs; m_arrBlockPropRS.RemoveAll();
			for(int i = 0; i < m_nBlockPropRSCnt; i++)
			{
				rs.serialize(ar, version, bSendNRecv);
				m_arrBlockPropRS.Add(rs);
			}
		}
	}

public:
	void Init(void)
	{
		CPKIDInherit::Init();
		m_nBlockPropRSCnt = 0;
		m_arrBlockPropRS.RemoveAll();
	}

private:
	int m_nBlockPropRSCnt;

public:
	CArray<CBlockPropertyRecordSet, CBlockPropertyRecordSet&> m_arrBlockPropRS;
};

class CPKUploadFile : public CPKIDInherit
{
public:
	CPKUploadFile(void)
	{

	}
	~CPKUploadFile(void)
	{

	}

	CPKUploadFile& operator = (const CPKUploadFile &other)
	{
		CPKIDInherit::operator=(other);
		m_uploadFileRS = other.m_uploadFileRS;
		m_arrUploadFileRS.Copy(other.m_arrUploadFileRS);
		return *this;
	}

	CPKUploadFile( const CPKUploadFile &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKIDInherit::serialize(ar, version, bSendNRecv);
		if(bSendNRecv == TRUE)
		{
			m_uploadFileRS.serialize(ar, version, bSendNRecv);

			m_nUploadFileRSCnt = m_arrUploadFileRS.GetCount();
			ar & m_nUploadFileRSCnt;
			for(int i = 0; i < m_nUploadFileRSCnt; i++)
				m_arrUploadFileRS.GetAt(i).serialize(ar, version, bSendNRecv);
		}
		else
		{
			m_uploadFileRS.serialize(ar, version, bSendNRecv);

			ar & m_nUploadFileRSCnt;
			CUploadFileRecordSet rs; m_arrUploadFileRS.RemoveAll();
			for(int i = 0; i < m_nUploadFileRSCnt; i++)
			{
				rs.serialize(ar, version, bSendNRecv);
				m_arrUploadFileRS.Add(rs);
			}
		}
	}

public:
	void Init(void)
	{
		CPKIDInherit::Init();
		m_nUploadFileRSCnt = 0;
		m_arrUploadFileRS.RemoveAll();
	}

private:
	int m_nUploadFileRSCnt;

public:
	CUploadFileRecordSet m_uploadFileRS;
	CArray<CUploadFileRecordSet, CUploadFileRecordSet&> m_arrUploadFileRS;
};

class CPKUserInputBlockList : public CPKUserIDNIDInherit
{
public:
	CPKUserInputBlockList(void)
	{

	}
	~CPKUserInputBlockList(void)
	{

	}

	CPKUserInputBlockList& operator = (const CPKUserInputBlockList &other)
	{
		CPKUserIDNIDInherit::operator=(other);
		m_strName = other.m_strName;
		m_arrUserInputBlockListRS.Copy(other.m_arrUserInputBlockListRS);
		return *this;
	}

	CPKUserInputBlockList( const CPKUserInputBlockList &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKUserIDNIDInherit::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_strName);
			ar & stdStr;

			m_nUserInputBlockListRSCnt = m_arrUserInputBlockListRS.GetCount();
			ar & m_nUserInputBlockListRSCnt;
			for(int i = 0; i < m_nUserInputBlockListRSCnt; i++)
				m_arrUserInputBlockListRS.GetAt(i).serialize(ar, version, bSendNRecv);
		}
		else
		{
			ar & stdStr;
			m_strName = CStringConverter::CStringAToCStringW(stdStr.c_str());

			ar & m_nUserInputBlockListRSCnt;
			CUserInputBlockListRecordSet rs; m_arrUserInputBlockListRS.RemoveAll();
			for(int i = 0; i < m_nUserInputBlockListRSCnt; i++)
			{
				rs.serialize(ar, version, bSendNRecv);
				m_arrUserInputBlockListRS.Add(rs);
			}
		}
	}

public:
	void Init(void)
	{
		CPKUserIDNIDInherit::Init();
		m_strName = _T("");
		m_nUserInputBlockListRSCnt = 0;
		m_arrUserInputBlockListRS.RemoveAll();
	}

private:
	int m_nUserInputBlockListRSCnt;

public:
	CString m_strName;
	CArray<CUserInputBlockListRecordSet, CUserInputBlockListRecordSet&> m_arrUserInputBlockListRS;
};

/////////////////////////////////////////////////////////////////////////
//모델관련 Packet들

class CPKEcoModel : public CPKWhereInherit
{
public:
	CPKEcoModel(void)
	{

	}
	~CPKEcoModel(void)
	{

	}

	CPKEcoModel& operator = (const CPKEcoModel &other)
	{
		CPKWhereInherit::operator=(other);
		m_strModelType = other.m_strModelType;
		m_strSide = other.m_strSide;
		m_strQueryType = other.m_strQueryType;
		m_strHsfDataPath = other.m_strHsfDataPath;
		m_strDB = other.m_strDB;
		m_strOriDB = other.m_strOriDB;
		m_strResDB = other.m_strResDB;
		m_strDownloadPath = other.m_strDownloadPath;
		m_strName = other.m_strName;
		m_strProductName = other.m_strProductName;
		m_strModulName = other.m_strModulName;
		m_strAssName = other.m_strAssName;
		m_strSuperiorName = other.m_strSuperiorName;
		m_strQIndex = other.m_strQIndex;
		m_strContourDataPath = other.m_strContourDataPath;
		m_strTableName = other.m_strTableName;
		m_arrModelList.Copy(other.m_arrModelList);
		m_arrExceptionList.Copy(other.m_arrExceptionList);
		m_arrPanelList.Copy(other.m_arrPanelList);

		CString key;
		int val;
		POSITION pos = other.m_mapPanelList.GetStartPosition();
		while(pos != NULL)
		{
			other.m_mapPanelList.GetNextAssoc(pos, key, val);
			m_mapPanelList.SetAt(key, val);
		}

		m_arrModelRS.Copy(other.m_arrModelRS);
		
		return *this;
	}

	CPKEcoModel( const CPKEcoModel &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKWhereInherit::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_strModelType);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_strSide);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_strQueryType);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_strHsfDataPath);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_strDB);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_strOriDB);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_strResDB);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_strDownloadPath);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_strName);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_strProductName);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_strModulName);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_strAssName);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_strSuperiorName);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_strQIndex);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_strContourDataPath);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_strTableName);
			ar & stdStr;

			m_nModelListCnt = m_arrModelList.GetCount();
			ar & m_nModelListCnt;
			for(int i = 0; i < m_nModelListCnt; i++)
			{
				stdStr = CStringConverter::CStringWToCStringA(m_arrModelList.GetAt(i));
				ar & stdStr;
			}

			m_nExceptionListCnt = m_arrExceptionList.GetCount();
			ar & m_nExceptionListCnt;
			for(int i = 0; i < m_nExceptionListCnt; i++)
			{
				stdStr = CStringConverter::CStringWToCStringA(m_arrExceptionList.GetAt(i));
				ar & stdStr;
			}

			m_nPanelListCnt = m_arrPanelList.GetCount();
			ar & m_nPanelListCnt;
			for(int i = 0; i < m_nPanelListCnt; i++)
				m_arrPanelList.GetAt(i).serialize(ar, version, bSendNRecv);

			m_nMapPanelListCnt = m_mapPanelList.GetCount();
			ar & m_nMapPanelListCnt;
			CString key = _T(""); int val = 0;
			POSITION pos = m_mapPanelList.GetStartPosition();
			while(pos != NULL)
			{
				m_mapPanelList.GetNextAssoc(pos, key, val);
				stdStr = CStringConverter::CStringWToCStringA(key);
				ar & stdStr;
				ar & val;
			}

			m_nModelRSCnt = m_arrModelRS.GetCount();
			ar & m_nModelRSCnt;
			for(int i = 0; i < m_nModelRSCnt; i++)
				m_arrModelRS.GetAt(i).serialize(ar, version, bSendNRecv);

		}
		else
		{
			ar & stdStr;
			m_strModelType = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_strSide = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_strQueryType = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_strHsfDataPath = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_strDB = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_strOriDB = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_strResDB = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_strDownloadPath = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_strName = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_strProductName = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_strModulName = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_strAssName = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_strSuperiorName = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_strQIndex = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_strContourDataPath = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_strTableName = CStringConverter::CStringAToCStringW(stdStr.c_str());

			ar & m_nModelListCnt;
			CString str; m_arrModelList.RemoveAll();
			for(int i = 0; i < m_nModelListCnt; i++)
			{
				ar & stdStr;
				str = CStringConverter::CStringAToCStringW(stdStr.c_str());
				m_arrModelList.Add(str);
			}

			ar & m_nExceptionListCnt;
			m_arrExceptionList.RemoveAll();
			for(int i = 0; i < m_nExceptionListCnt; i++)
			{
				ar & stdStr;
				str = CStringConverter::CStringAToCStringW(stdStr.c_str());
				m_arrExceptionList.Add(str);
			}

			ar & m_nPanelListCnt;
			UPDATE_MODEL_INFO stru; m_arrPanelList.RemoveAll();
			for(int i = 0; i < m_nPanelListCnt; i++)
			{
				stru.serialize(ar, version, bSendNRecv);
				m_arrPanelList.Add(stru);
			}

			ar & m_nMapPanelListCnt;
			CString key = _T(""); int val = 0; m_mapPanelList.RemoveAll();
			for(int i = 0; i < m_nMapPanelListCnt; i++)
			{
				ar & stdStr;
				key = CStringConverter::CStringAToCStringW(stdStr.c_str());
				ar & val;
				m_mapPanelList.SetAt(key, val);
			}

			ar & m_nModelRSCnt;
			CEcoModelRecordSet modelRS;
			m_arrModelRS.RemoveAll();
			for(int i = 0; i < m_nModelRSCnt; i++)
			{
				modelRS.serialize(ar, version, bSendNRecv);
				m_arrModelRS.Add(modelRS);
			}
		}
	}

public:
	void Init(void)
	{
		CPKWhereInherit::Init();
		m_strModelType = _T("");
		m_strSide = _T("");
		m_strQueryType = _T("");
		m_strHsfDataPath = _T("");
		m_strDB = _T("");
		m_strOriDB = _T("");
		m_strResDB = _T("");
		m_strDownloadPath = _T("");
		m_strName = _T("");
		m_strProductName = _T("");
		m_strModulName = _T("");
		m_strAssName = _T("");
		m_strSuperiorName = _T("");
		m_strQIndex = _T("");
		m_strContourDataPath = _T("");
		m_strTableName = _T("");
		m_nModelListCnt = 0;
		m_arrModelList.RemoveAll();
		m_nExceptionListCnt = 0;
		m_arrExceptionList.RemoveAll();
		m_nPanelListCnt = 0;
		m_arrPanelList.RemoveAll();
		m_nMapPanelListCnt = 0;
		m_mapPanelList.RemoveAll();
		m_nModelRSCnt = 0;
		m_arrModelRS.RemoveAll();
	}

private:
	int m_nModelListCnt;
	int m_nExceptionListCnt;
	int m_nPanelListCnt;
	int m_nMapPanelListCnt;
	int m_nModelRSCnt;

public:
	CString m_strModelType;
	CString m_strSide;
	CString m_strQueryType;
	CString m_strHsfDataPath;
	CString m_strDB;
	CString m_strOriDB;
	CString m_strResDB;
	CString m_strDownloadPath;
	CString m_strName;
	CString m_strProductName;
	CString m_strModulName;
	CString m_strAssName;
	CString m_strSuperiorName;
	CString m_strQIndex;
	CString m_strContourDataPath;
	CString m_strTableName;
	CArray<CString, CString&> m_arrModelList;
	CArray<CString, CString&> m_arrExceptionList;
	CArray<UPDATE_MODEL_INFO, UPDATE_MODEL_INFO&> m_arrPanelList;
	CMap<CString, LPCTSTR, int, int> m_mapPanelList;
	CArray<CEcoModelRecordSet, CEcoModelRecordSet&> m_arrModelRS;
};

class CPKTribonToEcoModel : public CPKShipNo
{
public:
	CPKTribonToEcoModel(void)
	{

	}
	~CPKTribonToEcoModel(void)
	{

	}

	CPKTribonToEcoModel& operator = (const CPKTribonToEcoModel &other)
	{
		CPKShipNo::operator=(other);
		m_strModelType = other.m_strModelType;
		m_nByUsing = other.m_nByUsing;
		m_nInsertModel = other.m_nInsertModel;
		m_strFilePath = other.m_strFilePath;
		m_strFilePathHsf = other.m_strFilePathHsf;
		m_strSide = other.m_strSide;
		m_strCPanelName = other.m_strCPanelName;
		return *this;
	}

	CPKTribonToEcoModel( const CPKTribonToEcoModel &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKShipNo::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			ar & m_nByUsing;
			ar & m_nInsertModel;
			stdStr = CStringConverter::CStringWToCStringA(m_strModelType);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_strFilePath);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_strFilePathHsf);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_strSide);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_strCPanelName);
			ar & stdStr;
		}
		else
		{
			ar & m_nByUsing;
			ar & m_nInsertModel;
			ar & stdStr;
			m_strModelType = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_strFilePath = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_strFilePathHsf = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_strSide = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_strCPanelName = CStringConverter::CStringAToCStringW(stdStr.c_str());

		}
	}

public:
	void Init(void)
	{
		CPKShipNo::Init();
		m_strModelType = _T("");
		m_nByUsing = 0;
		m_nInsertModel = 0;
		m_strFilePath = _T("");
		m_strFilePathHsf = _T("");
		m_strSide = _T("");
		m_strCPanelName = _T("");
	}

public:
	int m_nByUsing;
	int m_nInsertModel;
	CString m_strModelType;
	CString m_strFilePath;
	CString m_strFilePathHsf;
	CString m_strSide;
	CString m_strCPanelName;
};

class CPKEcoModelErrorList : public CPKShipNo
{
public:
	CPKEcoModelErrorList(void)
	{

	}
	~CPKEcoModelErrorList(void)
	{

	}

	CPKEcoModelErrorList& operator = (const CPKEcoModelErrorList &other)
	{
		CPKShipNo::operator=(other);
		m_strModelType = other.m_strModelType;
		m_strSide = other.m_strSide;
		m_strErrortype = other.m_strErrortype;
		m_strObjectName = other.m_strObjectName;
		m_arrObjectName.Copy(other.m_arrObjectName);
		return *this;
	}

	CPKEcoModelErrorList( const CPKEcoModelErrorList &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKShipNo::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_strModelType);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_strSide);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_strErrortype);
			ar & stdStr;
			stdStr = CStringConverter::CStringWToCStringA(m_strObjectName);
			ar & stdStr;

			m_nObjectNameCnt = m_arrObjectName.GetCount();
			ar & m_nObjectNameCnt;
			for(int i = 0; i < m_nObjectNameCnt; i++)
			{
				stdStr = CStringConverter::CStringWToCStringA(m_arrObjectName.GetAt(i));
				ar & stdStr;
			}
		}
		else
		{
			ar & stdStr;
			m_strModelType = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_strSide = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_strErrortype = CStringConverter::CStringAToCStringW(stdStr.c_str());
			ar & stdStr;
			m_strObjectName = CStringConverter::CStringAToCStringW(stdStr.c_str());

			ar & m_nObjectNameCnt;
			CString str; m_arrObjectName.RemoveAll();
			for(int i = 0; i < m_nObjectNameCnt; i++)
			{
				ar & stdStr;
				str = CStringConverter::CStringAToCStringW(stdStr.c_str());
				m_arrObjectName.Add(str);
			}
		}
	}

public:
	void Init(void)
	{
		CPKShipNo::Init();
		m_strModelType = _T("");
		m_strSide = _T("");
		m_strErrortype = _T("");
		m_strObjectName = _T("");
		m_nObjectNameCnt = 0;
		m_arrObjectName.RemoveAll();
	}

private:
	int m_nObjectNameCnt;

public:
	CString m_strModelType;
	CString m_strSide;
	CString m_strErrortype;
	CString m_strObjectName;
	CArray<CString, CString&> m_arrObjectName;
};


/////////////////////////////////////////////////////////////////////////
//MEI모델관련 Packet들

class CPKCreateMEIModelTable : public CPKShipNo
{
public:
	CPKCreateMEIModelTable(void)
	{

	}
	~CPKCreateMEIModelTable(void)
	{

	}

	CPKCreateMEIModelTable& operator = (const CPKCreateMEIModelTable &other)
	{
		CPKShipNo::operator=(other);
		m_strModelType = other.m_strModelType;
		return *this;
	}

	CPKCreateMEIModelTable( const CPKCreateMEIModelTable &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKShipNo::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_strModelType);
			ar & stdStr;
		}
		else
		{
			ar & stdStr;
			m_strModelType = CStringConverter::CStringAToCStringW(stdStr.c_str());
		}
	}

public:
	void Init(void)
	{
		CPKShipNo::Init();
		m_strModelType = _T("");
	}

public:
	CString m_strModelType;
};

class CPKMEIEcoModel : public CPKShipNo
{
public:
	CPKMEIEcoModel(void)
	{

	}
	~CPKMEIEcoModel(void)
	{

	}

	CPKMEIEcoModel& operator = (const CPKMEIEcoModel &other)
	{
		CPKShipNo::operator=(other);
		m_nNickTable = other.m_nNickTable;
		m_nFID = other.m_nFID;
		m_strHsfDataPath = other.m_strHsfDataPath;
		m_bFolder = other.m_bFolder;
		m_modelDataTableRS = other.m_modelDataTableRS;
		return *this;
	}

	CPKMEIEcoModel( const CPKMEIEcoModel &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKShipNo::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			ar & m_nNickTable;
			ar & m_nFID;
			ar & m_bFolder;
			stdStr = CStringConverter::CStringWToCStringA(m_strHsfDataPath);
			ar & stdStr;
			m_modelDataTableRS.serialize(ar, version, bSendNRecv);
		}
		else
		{
			ar & m_nNickTable;
			ar & m_nFID;
			ar & m_bFolder;
			ar & stdStr;
			m_strHsfDataPath = CStringConverter::CStringAToCStringW(stdStr.c_str());
			m_modelDataTableRS.serialize(ar, version, bSendNRecv);
		}
	}

public:
	void Init(void)
	{
		CPKShipNo::Init();
		m_nNickTable = 0;
		m_nFID = 0;
		m_strHsfDataPath = _T("");
		m_bFolder = FALSE;
	}

public:
	int m_nNickTable;
	UINT m_nFID;
	BOOL m_bFolder;
	CString m_strHsfDataPath;
	CEcoModelDataTableSet m_modelDataTableRS;
};

class CPKMEIEcoModelManager : public CPKWhereInherit
{
public:
	CPKMEIEcoModelManager(void)
	{

	}
	~CPKMEIEcoModelManager(void)
	{

	}

	CPKMEIEcoModelManager& operator = (const CPKMEIEcoModelManager &other)
	{
		CPKWhereInherit::operator=(other);
		m_arrModelNameManagerRS.Copy(other.m_arrModelNameManagerRS);
		return *this;
	}

	CPKMEIEcoModelManager( const CPKMEIEcoModelManager &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKWhereInherit::serialize(ar, version, bSendNRecv);
		if(bSendNRecv == TRUE)
		{
			m_nModelNameManagerRSCnt = m_arrModelNameManagerRS.GetCount();
			ar & m_nModelNameManagerRSCnt;
			for(int i = 0; i < m_nModelNameManagerRSCnt; i++)
				m_arrModelNameManagerRS.GetAt(i).serialize(ar, version, bSendNRecv);
		}
		else
		{
			ar & m_nModelNameManagerRSCnt;
			CEcoModelNameManagerRecordSet rs; m_arrModelNameManagerRS.RemoveAll();
			for(int i = 0; i < m_nModelNameManagerRSCnt; i++)
			{
				rs.serialize(ar, version, bSendNRecv);
				m_arrModelNameManagerRS.Add(rs);
			}
		}
	}

public:
	void Init(void)
	{
		CPKWhereInherit::Init();
		m_nModelNameManagerRSCnt = 0;
		m_arrModelNameManagerRS.RemoveAll();
	}

private:
	int m_nModelNameManagerRSCnt;

public:
	CArray<CEcoModelNameManagerRecordSet, CEcoModelNameManagerRecordSet&> m_arrModelNameManagerRS;
};

class CPKMEIHierarchyTree : public CPKWhereInherit
{
public:
	CPKMEIHierarchyTree(void)
	{

	}
	~CPKMEIHierarchyTree(void)
	{

	}

	CPKMEIHierarchyTree& operator = (const CPKMEIHierarchyTree &other)
	{
		CPKWhereInherit::operator=(other);
		m_modelHierarchyTreeRS = other.m_modelHierarchyTreeRS;
		m_arrModelHierarchyTreeRS.Copy(other.m_arrModelHierarchyTreeRS);
		return *this;
	}

	CPKMEIHierarchyTree( const CPKMEIHierarchyTree &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKWhereInherit::serialize(ar, version, bSendNRecv);
		if(bSendNRecv == TRUE)
		{
			m_modelHierarchyTreeRS.serialize(ar, version, bSendNRecv);
			m_nModelHierarchyTreeRSCnt = m_arrModelHierarchyTreeRS.GetCount();
			ar & m_nModelHierarchyTreeRSCnt;
			for(int i = 0; i < m_nModelHierarchyTreeRSCnt; i++)
				m_arrModelHierarchyTreeRS.GetAt(i).serialize(ar, version, bSendNRecv);
		}
		else
		{
			m_modelHierarchyTreeRS.serialize(ar, version, bSendNRecv);
			ar & m_nModelHierarchyTreeRSCnt;
			CEcoModelHierarchytreeRecordSet rs; m_arrModelHierarchyTreeRS.RemoveAll();
			for(int i = 0; i < m_nModelHierarchyTreeRSCnt; i++)
			{
				rs.serialize(ar, version, bSendNRecv);
				m_arrModelHierarchyTreeRS.Add(rs);
			}
		}
	}

public:
	void Init(void)
	{
		CPKWhereInherit::Init();
		m_nModelHierarchyTreeRSCnt = 0;
		m_arrModelHierarchyTreeRS.RemoveAll();
	}

private:
	int m_nModelHierarchyTreeRSCnt;

public:
	CEcoModelHierarchytreeRecordSet m_modelHierarchyTreeRS;
	CArray<CEcoModelHierarchytreeRecordSet, CEcoModelHierarchytreeRecordSet&> m_arrModelHierarchyTreeRS;
};



//-->2014-11-20 최조영 JT관련 코드 추가 (현대전용)
class CPKJTServerProperty : public CPKWhereInherit
{
public:
	CPKJTServerProperty(void)
	{

	}
	~CPKJTServerProperty(void)
	{

	}

	CPKJTServerProperty& operator = (const CPKJTServerProperty &other)
	{
		CPKWhereInherit::operator=(other);
		m_arrShipnoList.Copy(other.m_arrShipnoList);
		m_arrSelModelName.Copy(other.m_arrSelModelName);
		m_nSelModelCnt = other.m_nSelModelCnt;
		m_nMapListCnt = other.m_nMapListCnt;
		m_nShipNoCnt = other.m_nShipNoCnt;
		m_strSelShipNo = other.m_strSelShipNo;
		m_strJtFileFTPPath = other.m_strJtFileFTPPath;

		CString key;
		CString val;
		POSITION pos = other.m_mapJtList.GetStartPosition();
		while(pos != NULL)
		{
			other.m_mapJtList.GetNextAssoc(pos, key, val);
			m_mapJtList.SetAt(key, val);
		}

		return *this;
	}

	CPKJTServerProperty( const CPKJTServerProperty &s )
	{ 
		*this = s; 
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		CPKWhereInherit::serialize(ar, version, bSendNRecv);

		std::string stdStr;
		if(bSendNRecv == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_strSelShipNo);
			ar & stdStr;

			stdStr = CStringConverter::CStringWToCStringA(m_strJtFileFTPPath);
			ar & stdStr;


			//sel model
			m_nSelModelCnt = m_arrSelModelName.GetCount();
			ar & m_nSelModelCnt;


			for(int i = 0; i < m_nSelModelCnt; i++)
			{
				stdStr = CStringConverter::CStringWToCStringA(m_arrSelModelName.GetAt(i));
				ar & stdStr;
			}

			//sel shipno
			m_nShipNoCnt = m_arrShipnoList.GetCount();
			ar & m_nShipNoCnt;


			for(int i = 0; i < m_nShipNoCnt; i++)
			{
				stdStr = CStringConverter::CStringWToCStringA(m_arrShipnoList.GetAt(i));
				ar & stdStr;
			}



			m_nMapListCnt = m_mapPanelList.GetCount();
			ar & m_nMapListCnt;
			CString key = _T(""); int val = 0;
			POSITION pos = m_mapJtList.GetStartPosition();
			while(pos != NULL)
			{
				m_mapJtList.GetNextAssoc(pos, key, val);
				stdStr = CStringConverter::CStringWToCStringA(key);
				ar & stdStr;
				ar & val;
			}
		}
		else
		{

			ar & stdStr;
			m_strSelShipNo = CStringConverter::CStringAToCStringW(stdStr.c_str());

			ar & stdStr;
			m_strJtFileFTPPath = CStringConverter::CStringAToCStringW(stdStr.c_str());

			ar & m_nShipNoCnt;
			CString str; m_arrShipnoList.RemoveAll();
			for(int i = 0; i < m_nShipNoCnt; i++)
			{
				ar & stdStr;
				str = CStringConverter::CStringAToCStringW(stdStr.c_str());
				m_arrShipnoList.Add(str);
			}

			ar & m_nSelModelCnt;
			CString str; m_arrSelModelName.RemoveAll();
			for(int i = 0; i < m_nSelModelCnt; i++)
			{
				ar & stdStr;
				str = CStringConverter::CStringAToCStringW(stdStr.c_str());
				m_arrSelModelName.Add(str);
			}

			ar & m_nMapListCnt;
			CString key = _T(""); int val = 0; m_mapJtList.RemoveAll();
			for(int i = 0; i < m_nMapListCnt; i++)
			{
				ar & stdStr;
				key = CStringConverter::CStringAToCStringW(stdStr.c_str());
				ar & val;
				m_mapJtList.SetAt(key, val);
			}

		}
	}

public:
	void Init(void)
	{
		CPKWhereInherit::Init();
		m_mapJtList.RemoveAll();
		m_arrShipnoList.RemoveAll();
		m_arrSelModelName.RemoveAll();
		m_nShipNoCnt = 0;
		m_nMapListCnt = 0;
		m_nSelModelCnt = 0;
		m_strSelShipNo = L"";
		m_strJtFileFTPPath = L"";
	}

public:
	CString					  m_strJtFileFTPPath;
	CString					  m_strSelShipNo;
	int						  m_nMapListCnt;
	int						  m_nShipNoCnt;
	int						  m_nSelModelCnt;
	CArray<CString, CString&> m_arrSelModelName;	
	CArray<CString, CString&> m_arrShipnoList;
	CMap<CString, LPCTSTR, CString, LPCTSTR> m_mapJtList;
};


//<--2014-11-20 최조영 JT관련 코드 추가