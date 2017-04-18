#pragma once
#include <afxwin.h>  
#include <afxext.h>

class SISAlarmInfo
{
private:
	BOOL m_bDownloadAlarmEmail;
	BOOL m_bDownloadAlarmMobile;
	BOOL m_bUploadAlarmEmail;
	BOOL m_bUploadAlarmMobile;
	BOOL m_bModifyAlarmEmail;
	BOOL m_bModifyAlarmMobile;
	BOOL m_bDeleteAlarmEmail;
	BOOL m_bDeleteAlarmMobile;

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISAlarmInfo()
	{
		m_bDownloadAlarmEmail = TRUE;
		m_bDownloadAlarmMobile = TRUE;
		m_bUploadAlarmEmail = TRUE;
		m_bUploadAlarmMobile = TRUE;
		m_bModifyAlarmEmail = TRUE;
		m_bModifyAlarmMobile = TRUE;
		m_bDeleteAlarmEmail = TRUE;
		m_bDeleteAlarmMobile = TRUE;
	}

	/**
	@brief Creator
	@param c [in]Object of SISAlarmInfo
	@return
	@remark Initialize member variable using value of input object
	*/
	SISAlarmInfo(const SISAlarmInfo& c)
	{
		*this = c;
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISAlarmInfo& operator = (const SISAlarmInfo &other)
	{
		m_bDownloadAlarmEmail = other.m_bDownloadAlarmEmail;
		m_bDownloadAlarmMobile = other.m_bDownloadAlarmMobile;
		m_bUploadAlarmEmail = other.m_bUploadAlarmEmail;
		m_bUploadAlarmMobile = other.m_bUploadAlarmMobile;
		m_bModifyAlarmEmail = other.m_bModifyAlarmEmail;
		m_bModifyAlarmMobile = other.m_bModifyAlarmMobile;
		m_bDeleteAlarmEmail = other.m_bDeleteAlarmEmail;
		m_bDeleteAlarmMobile = other.m_bDeleteAlarmMobile;
	
		return *this;
	}

	/**
	@brief Set download email alram setting value
	@param bDownloadAlramEmail [in]Download email alram setting value
	@return
	@remark
	*/
	void SetDownloadAlramEmail(BOOL bDownloadAlramEmail)
	{
		m_bDownloadAlarmEmail = bDownloadAlramEmail;
	}

	/**
	@brief Get download email alram setting value
	@param
	@return Download email alram setting value
	@remark
	*/
	BOOL GetDownloadAlramEmail()
	{
		return m_bDownloadAlarmEmail;
	}

	/**
	@brief Set download mobile alram setting value
	@param bDownloadAlramMobile [in]Download mobile alram setting value
	@return
	@remark
	*/
	void SetDownloadAlramMobile(BOOL bDownloadAlramMobile)
	{
		m_bDownloadAlarmMobile = bDownloadAlramMobile;
	}

	/**
	@brief Get download mobile alram setting value
	@param
	@return Download mobile alram setting value
	@remark
	*/
	BOOL GetDownloadAlramMobile()
	{
		return m_bDownloadAlarmMobile;
	}

	/**
	@brief Set upload email alram setting value 
	@param bUploadAlramEmail [in]Upload email alram setting value
	@return
	@remark
	*/
	void SetUploadAlramEmail(BOOL bUploadAlramEmail)
	{
		m_bUploadAlarmEmail = bUploadAlramEmail;
	}

	/**
	@brief Get upload email alram setting value 
	@param
	@return Upload email alram setting value
	@remark
	*/
	BOOL GetUploadAlramEmail()
	{
		return m_bUploadAlarmEmail;
	}

	/**
	@brief Set upload mobile alram setting value
	@param bUploadAlramEmail [in]Upload mobile alram setting value
	@return
	@remark
	*/
	void SetUploadAlramMobile(BOOL bUploadAlramMobile)
	{
		m_bUploadAlarmMobile = bUploadAlramMobile;
	}

	/**
	@brief Get upload mobile alram setting value
	@param
	@return Upload mobile alram setting value
	@remark
	*/
	BOOL GetUploadAlramMobile()
	{
		return m_bUploadAlarmMobile;
	}

	/**
	@brief Set modify email alram setting value
	@param bModifyAlramEmail [in]Modify email alram setting value
	@return
	@remark
	*/
	void SetModifyAlramEmail(BOOL bModifyAlramEmail)
	{
		m_bModifyAlarmEmail = bModifyAlramEmail;
	}

	/**
	@brief Get modify email alram setting value
	@param
	@return Modify email alram setting value
	@remark
	*/
	BOOL GetModifyAlramEmail()
	{
		return m_bModifyAlarmEmail;
	}

	/**
	@brief Set modify mobile alram setting value
	@param bModifyAlramEmail [in]Modify mobile alram setting value
	@return
	@remark
	*/
	void SetModifyAlramMobile(BOOL bModifyAlramMobile)
	{
		m_bModifyAlarmMobile = bModifyAlramMobile;
	}

	/**
	@brief Get modify mobile alram setting value
	@param
	@return Modify mobile alram setting value
	@remark
	*/
	BOOL GetModifyAlramMobile()
	{
		return m_bModifyAlarmMobile;
	}

	/**
	@brief Set delete email alram setting value
	@param bDeleteAlramEmail [in]Delete email alram setting value
	@return
	@remark
	*/
	void SetDeleteAlramEmail(BOOL bDeleteAlramEmail)
	{
		m_bDeleteAlarmEmail = bDeleteAlramEmail;
	}

	/**
	@brief Get delete email alram setting value
	@param
	@return Delete email alram setting value
	@remark
	*/
	BOOL GetDeleteAlramEmail()
	{
		return m_bDeleteAlarmEmail;
	}

	/**
	@brief Set delete mobile alram setting value
	@param bDeleteAlramEmail [in]Delete mobile alram setting value
	@return
	@remark
	*/
	void SetDeleteAlramMobile(BOOL bDeleteAlramMobile)
	{
		m_bDeleteAlarmMobile = bDeleteAlramMobile;
	}

	/**
	@brief Get delete mobile alram setting value
	@param
	@return Delete mobile alram setting value
	@remark
	*/
	BOOL GetDeleteAlramMobile()
	{
		return m_bDeleteAlarmMobile;
	}
};

class SISStatusAlarm : public SISAlarmInfo
{
private:
	CString m_strWorkflow;
	UINT m_nWorkflowID;
	CString m_strStatus;

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISStatusAlarm()
	{
		m_strWorkflow = _T("");
		m_nWorkflowID = 0;
		m_strStatus = _T("");
	}

	/**
	@brief Creator
	@param c [in]Object of SISStatusAlarm
	@return
	@remark Initialize member variable using value of input object
	*/
	SISStatusAlarm(const SISStatusAlarm& c)
	{
		*this = c;
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISStatusAlarm& operator = (const SISStatusAlarm &other)
	{
		SISAlarmInfo::operator=(other);

		m_strWorkflow = other.m_strWorkflow;
		m_nWorkflowID = other.m_nWorkflowID;
		m_strStatus = other.m_strStatus;

		return *this;
	}

	/**
	@brief Set workflow name
	@param strWorkflow [in]Workflow name
	@return
	@remark
	*/
	void SetWorkflowName(CString strWorkflow)
	{
		m_strWorkflow = strWorkflow;
	}

	/**
	@brief Get workflow name
	@param
	@return Workflow name
	@remark
	*/
	CString GetWorkflowName()
	{
		return m_strWorkflow;
	}

	/**
	@brief Set workflow ID
	@param nWorkflowID [in]Workflow ID
	@return
	@remark
	*/
	void SetWorkflowID(UINT nWorkflowID)
	{
		m_nWorkflowID = nWorkflowID;
	}

	/**
	@brief Get workflow ID
	@param
	@return Workflow ID
	@remark
	*/
	UINT GetWorkflowID()
	{
		return m_nWorkflowID;
	}

	/**
	@brief Set status
	@param strStatus [in]Status name
	@return
	@remark
	*/
	void SetStatus(CString strStatus)
	{
		m_strStatus = strStatus;
	}

	/**
	@brief Get status
	@param 
	@return Status name
	@remark
	*/
	CString GetStatus()
	{
		return m_strStatus;
	}
};

class SISUserAlarm
{
private:
	CString m_strUserID;
	CArray<SISStatusAlarm, SISStatusAlarm&> m_arrStatusAlarm;

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISUserAlarm()
	{
		m_strUserID = _T("");
		m_arrStatusAlarm.RemoveAll();
	}

	/**
	@brief Creator
	@param c [in]Object of SISUserAlarm
	@return
	@remark Initialize member variable using value of input object
	*/
	SISUserAlarm(const SISUserAlarm& c)
	{
		*this = c;
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISUserAlarm& operator = (const SISUserAlarm &other)
	{
		m_strUserID = other.m_strUserID;
		m_arrStatusAlarm.Copy(other.m_arrStatusAlarm);

		return *this;
	}

	/**
	@brief Set user ID
	@param strUserID [in]User ID
	@return
	@remark
	*/
	void SetUserID(CString strUserID)
	{
		m_strUserID = strUserID;
	}

	/**
	@brief Get user ID
	@param 
	@return User ID
	@remark
	*/
	CString GetUserID()
	{
		return m_strUserID;
	}

	/**
	@brief Add alram imformation of each status 
	@param statusAlarm [in]Alram imformation of each status 
	@return Success presence or absence
	@remark
	*/
	BOOL AddStatusAlarm(SISStatusAlarm statusAlarm)
	{
		if (statusAlarm.GetStatus() == _T(""))
			return FALSE;

		BOOL bExist = FALSE;
		for (int i = 0; i < m_arrStatusAlarm.GetCount(); i++)
		{
			if (m_arrStatusAlarm.GetAt(i).GetStatus() == statusAlarm.GetStatus() && m_arrStatusAlarm.GetAt(i).GetWorkflowID() == statusAlarm.GetWorkflowID())
			{
				bExist = TRUE;
				break;
			}
		}
		if (bExist == TRUE)
			return FALSE;

		m_arrStatusAlarm.Add(statusAlarm);

		return TRUE;
	}

	/**
	@brief Remove alram imformation of each status
	@param 
	@return
	@remark
	*/
	void RemoveStatusAlarm()
	{
		m_arrStatusAlarm.RemoveAll();
	}

	/**
	@brief Remove alram imformation of each status
	@param strWorkflowName [in]Workflow name
	@param strStatus [in]Status name
	@return
	@remark
	*/
	void RemoveStatusAlarm(CString strWorkflowName, CString strStatus)
	{
		for (int i = m_arrStatusAlarm.GetCount() - 1; i >= 0; i--)
		{
			if (m_arrStatusAlarm.GetAt(i).GetStatus() == strStatus && m_arrStatusAlarm.GetAt(i).GetWorkflowName() == strWorkflowName)
			{
				m_arrStatusAlarm.RemoveAt(i);
			}
		}
	}

	/**
	@brief Get alram imformation of each status
	@param arrStatusAlarm [out]Alram imformation of each status
	@return
	@remark
	*/
	void GetStatusAlarm(CArray<SISStatusAlarm, SISStatusAlarm&>& arrStatusAlarm)
	{
		arrStatusAlarm.Copy(m_arrStatusAlarm);
	}

	/**
	@brief Get alram imformation of status
	@param strStatus [in]Status name
	@param statusAlarm [out]Alram imformation of status
	@return Success presence or absence
	@remark
	*/
	BOOL GetStatusAlarm(CString strStatus, SISStatusAlarm& statusAlarm)
	{
		BOOL bExist = FALSE;
		int i = 0;
		for (i; i < m_arrStatusAlarm.GetCount(); i++)
		{
			if (m_arrStatusAlarm.GetAt(i).GetStatus() == strStatus)
			{
				bExist = TRUE;
				break;
			}
		}

		if (bExist == FALSE)
			return FALSE;

		statusAlarm = m_arrStatusAlarm.GetAt(i);

		return TRUE;
	}
};