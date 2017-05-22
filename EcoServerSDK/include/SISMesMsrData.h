#pragma once
#include <afxwin.h>  
#include <afxext.h>

class SISMesMsrData
{
public:
	UINT m_nMesMsrDataID;
	CString m_strAssemLevel;
	CString m_strAssemBlock;
	CString m_strMesMsrDataName;
	UINT m_nMsrDataCnt;
	CString m_strCreateUserID;
	COleDateTime m_dtCreateDate;
	CString m_strUpdateUserID;
	COleDateTime m_dtUpdateDate;

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISMesMsrData()
	{
		m_nMesMsrDataID = 0;
		m_strAssemLevel = _T("");
		m_strAssemBlock = _T("");
		m_strMesMsrDataName = _T("");
		m_nMsrDataCnt = 0;
		m_strCreateUserID = _T("");
		m_dtCreateDate.m_status = m_dtCreateDate.invalid;
		m_strUpdateUserID = _T("");
		m_dtUpdateDate.m_status = m_dtUpdateDate.invalid;
	}

	/**
	@brief Creator
	@param c [in]Object of SISMesMsrData
	@return
	@remark Initialize member variable using value of input object
	*/
	SISMesMsrData(const SISMesMsrData& c)
	{
		*this = c;
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISMesMsrData& operator = (const SISMesMsrData &other)
	{	
		m_nMesMsrDataID = other.m_nMesMsrDataID;
		m_strAssemLevel = other.m_strAssemLevel;
		m_strAssemBlock = other.m_strAssemBlock;
		m_strMesMsrDataName = other.m_strMesMsrDataName;
		m_nMsrDataCnt = other.m_nMsrDataCnt;
		m_strCreateUserID = other.m_strCreateUserID;
		m_dtCreateDate = other.m_dtCreateDate;
		m_strUpdateUserID = other.m_strUpdateUserID;
		m_dtUpdateDate = other.m_dtUpdateDate;

		return *this;
	}

	/**
	@brief Set measurement data ID
	@param nID [in]Measurement data ID
	@return
	@remark
	*/
	void SetMesMsrDataID(UINT nID)
	{
		m_nMesMsrDataID = nID;
	}

	/**
	@brief Get measurement data ID
	@param
	@return Measurement data ID
	@remark
	*/
	UINT GetMesMsrDataID()
	{
		return m_nMesMsrDataID;
	}

	/**
	@brief Set assembly level
	@param strAssemLevel [in]Assembly level
	@return
	@remark
	*/
	void SetAssemLevel(CString strAssemLevel)
	{
		m_strAssemLevel = strAssemLevel;
	}

	/**
	@brief Get assembly level
	@param
	@return Assembly level
	@remark
	*/
	CString GetAssemLevel()
	{
		return m_strAssemLevel;
	}

	/**
	@brief Set assembly block
	@param strAssemBlock [in]Assembly block
	@return
	@remark
	*/
	void SetAssemBlock(CString strAssemBlock)
	{
		m_strAssemBlock = strAssemBlock;
	}

	/**
	@brief Get assembly block
	@param
	@return Assembly block
	@remark
	*/
	CString GetAssemBlock()
	{
		return m_strAssemBlock;
	}

	/**
	@brief Set measurement data name
	@param strMesMsrDataName [in]Measurement data name
	@return
	@remark
	*/
	void SetMesMsrDataName(CString strMesMsrDataName)
	{
		m_strMesMsrDataName = strMesMsrDataName;
	}

	/**
	@brief Get measurement data name
	@param
	@return Measurement data name
	@remark
	*/
	CString GetMesMsrDataName()
	{
		return m_strMesMsrDataName;
	}

	/**
	@brief Set measurement data count
	@param nCnt [in]Measurement data count
	@return
	@remark
	*/
	void SetMsrDataCount(UINT nCnt)
	{
		m_nMsrDataCnt = nCnt;
	}

	/**
	@brief Get measurement data count
	@param 
	@return Measurement data count
	@remark
	*/
	UINT GetMsrDataCount()
	{
		return m_nMsrDataCnt;
	}

	/**
	@brief Set creator ID
	@param strUserID [in]Creator ID
	@return
	@remark
	*/
	void SetCreateUserID(CString strUserID)
	{
		m_strCreateUserID = strUserID;
	}

	/**
	@brief Get creator ID
	@param
	@return Creator ID
	@remark
	*/
	CString GetCreateUserID()
	{
		return m_strCreateUserID;
	}

	/**
	@brief Set create date
	@param dtCreateDate [in]Create date
	@return
	@remark
	*/
	void SetCreateDate(COleDateTime dtCreateDate)
	{
		m_dtCreateDate = dtCreateDate;
	}

	/**
	@brief Get create date
	@param
	@return Create date
	@remark
	*/
	COleDateTime GetCreateDate()
	{
		return m_dtCreateDate;
	}

	/**
	@brief Set updater ID
	@param strUserID [in]Updater ID
	@return
	@remark
	*/
	void SetUpdateUserID(CString strUserID)
	{
		m_strUpdateUserID = strUserID;
	}

	/**
	@brief Get updater ID
	@param
	@return Updater ID
	@remark
	*/
	CString GetUpdateUserID()
	{
		return m_strUpdateUserID;
	}

	/**
	@brief Set update date
	@param dtUpdateDate [in]Update date
	@return
	@remark
	*/
	void SetUpdateDate(COleDateTime dtUpdateDate)
	{
		m_dtUpdateDate = dtUpdateDate;
	}

	/**
	@brief Get update date
	@param
	@return Update date
	@remark
	*/
	COleDateTime GetUpdateDate()
	{
		return m_dtUpdateDate;
	}
};