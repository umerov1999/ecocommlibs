#pragma once
#include <afxwin.h>  
#include <afxext.h>

class SISWork : public SISShipNoInfo
{
public:
	CString m_strWorkflow;
	UINT m_nWorkflowID;
	CString m_strAssemLevel;
	CString m_strBlockName;
	UINT m_nIssueID;
	UINT m_nWorkID;
	CString m_strStatus;
	COleDateTime m_dtCreateTime;

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISWork()
	{
		m_strWorkflow = _T("");
		m_nWorkflowID = 0;
		m_strAssemLevel = _T("");
		m_strBlockName = _T("");
		m_nIssueID = 0;
		m_nWorkID = 0;
		m_strStatus = _T("");
		m_dtCreateTime.m_dt = 0;
	}

	/**
	@brief Creator
	@param c [in]Object of SISWork
	@return
	@remark Initialize member variable using value of input object
	*/
	SISWork(const SISWork& c)
	{
		*this = c;
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISWork& operator = (const SISWork &other)
	{
		SISShipNoInfo::operator=(other);

		m_strWorkflow = other.m_strWorkflow;
		m_nWorkflowID = other.m_nWorkflowID;
		m_strAssemLevel = other.m_strAssemLevel;
		m_strBlockName = other.m_strBlockName;
		m_nIssueID = other.m_nIssueID;
		m_nWorkID = other.m_nWorkID;
		m_strStatus = other.m_strStatus;
		m_dtCreateTime = other.m_dtCreateTime;

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
	@brief Set assembly block
	@param strAssemLevel [in]Assembly block
	@return
	@remark
	*/
	void SetAssemblyBlock(CString strAssemBlock)
	{
		m_strBlockName = strAssemBlock;
	}

	/**
	@brief Get assembly block
	@param
	@return Assembly block
	@remark
	*/
	CString GetAssemblyBlock()
	{
		return m_strBlockName;
	}

	/**
	@brief Set assembly level
	@param strAssemLevel [in]Assembly level
	@return
	@remark
	*/
	void SetAssemblyLevel(CString strAssemLevel)
	{
		m_strAssemLevel = strAssemLevel;
	}

	/**
	@brief Get assembly level
	@param
	@return Assembly level
	@remark
	*/
	CString GetAssemblyLevel()
	{
		return m_strAssemLevel;
	}

	/**
	@brief Set issue ID
	@param nIssueID [in]Issue ID
	@return
	@remark
	*/
	void SetIssueID(UINT nIssueID)
	{
		m_nIssueID = nIssueID;
	}

	/**
	@brief Get issue ID
	@param
	@return Issue ID
	@remark
	*/
	UINT GetIssueID()
	{
		return m_nIssueID;
	}

	/**
	@brief Set work ID
	@param nWorkID [in]Work ID
	@return
	@remark
	*/
	void SetWorkID(UINT nWorkID)
	{
		m_nWorkID = nWorkID;
	}

	/**
	@brief Get work ID
	@param
	@return Work ID
	@remark
	*/
	UINT GetWorkID()
	{
		return m_nWorkID;
	}

	/**
	@brief Set status name
	@param strStatus [in]Status name
	@return
	@remark
	*/
	void SetStatus(CString strStatus)
	{
		m_strStatus = strStatus;
	}

	/**
	@brief Get status name
	@param
	@return Status name
	@remark
	*/
	CString GetStatus()
	{
		return m_strStatus;
	}

	/**
	@brief Set create time
	@param dtCreateTime [in]Create time
	@return
	@remark
	*/
	void SetCreateTime(COleDateTime dtCreateTime)
	{
		m_dtCreateTime = dtCreateTime;
	}

	/**
	@brief Get create time
	@param
	@return Create time
	@remark
	*/
	COleDateTime GetCreateTime()
	{
		return m_dtCreateTime;
	}

};

class SISWorkHistory
{
private:
	UINT m_nIssueID;
	CArray<SISWork, SISWork&> m_arrWork;

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISWorkHistory()
	{
		m_nIssueID = 0;
		m_arrWork.RemoveAll();
	}

	/**
	@brief Creator
	@param c [in]Object of SISWorkHistory
	@return
	@remark Initialize member variable using value of input object
	*/
	SISWorkHistory(const SISWorkHistory& c)
	{
		*this = c;
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISWorkHistory& operator = (const SISWorkHistory &other)
	{
		m_nIssueID = other.m_nIssueID;
		m_arrWork.Copy(other.m_arrWork);

		return *this;
	}

	/**
	@brief Set issue ID
	@param nIssueID [in]Issue ID
	@return
	@remark
	*/
	void SetIssueID(UINT nIssueID)
	{
		m_nIssueID = nIssueID;
	}

	/**
	@brief Get issue ID
	@param 
	@return Issue ID
	@remark
	*/
	UINT GetIssueID()
	{
		return m_nIssueID;
	}

	/**
	@brief Add work
	@param work [in]Work
	@return Success presence or absence
	@remark
	*/
	BOOL AddWork(SISWork work)
	{
		BOOL bExist = FALSE;
		for (int i = 0; i < m_arrWork.GetCount(); i++)
		{
			if (m_arrWork.GetAt(i).GetWorkID() == work.GetWorkID())
			{
				bExist = TRUE;
				break;
			}
		}

		if (bExist == TRUE)
			return FALSE;

		m_arrWork.Add(work);

		return TRUE;
	}

	/**
	@brief Delete all work
	@param 
	@return 
	@remark
	*/
	void DeleteAllWork()
	{
		m_arrWork.RemoveAll();
	}

	/**
	@brief Get work
	@param nStep [in]Step
	@param work [out]Work
	@return Success presence or absence
	@remark
	*/
	BOOL GetWork(UINT nStep, SISWork &work)
	{
		if (nStep < 0 || m_arrWork.GetCount() - 1 < nStep)
			return FALSE;

		work = m_arrWork.GetAt(nStep);

		return TRUE;
	}

	/**
	@brief Get work count
	@param 
	@return Work count
	@remark
	*/
	UINT GetWorkStepCnt()
	{
		return m_arrWork.GetCount();
	}

};
