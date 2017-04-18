#pragma once
#include <afxwin.h>  
#include <afxext.h>

class SISWorkFlowRelation
{
public:
	CString m_strStartStatus;
	CString m_strEndStatus;
	CString m_strStartPoint;
	CString m_strEndPoint;

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISWorkFlowRelation()
	{
		m_strStartStatus = _T("");
		m_strEndStatus = _T("");
		m_strStartPoint = _T("");
		m_strEndPoint = _T("");
	}

	/**
	@brief Creator
	@param c [in]Object of SISWorkFlowRelation
	@return
	@remark Initialize member variable using value of input object
	*/
	SISWorkFlowRelation(const SISWorkFlowRelation& c)
	{
		*this = c;
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISWorkFlowRelation& operator = (const SISWorkFlowRelation &other)
	{
		m_strStartStatus = other.m_strStartStatus;
		m_strEndStatus = other.m_strEndStatus;
		m_strStartPoint = other.m_strStartPoint;
		m_strEndPoint = other.m_strEndPoint;

		return *this;
	}

	/**
	@brief Set start status in workflow
	@param strStatus [in]Status name
	@return
	@remark
	*/
	void SetStartStatus(CString strStatus)
	{
		m_strStartStatus = strStatus;
	}

	/**
	@brief Get start status in workflow
	@param
	@return Status name
	@remark
	*/
	CString GetStartStatus()
	{
		return m_strStartStatus;
	}

	/**
	@brief Set end status in workflow
	@param strStatus [in]Status name
	@return
	@remark
	*/
	void SetEndStatus(CString strStatus)
	{
		m_strEndStatus = strStatus;
	}

	/**
	@brief Get end status in workflow
	@param
	@return Status name
	@remark
	*/
	CString GetEndStatus()
	{
		return m_strEndStatus;
	}

	/**
	@brief Set point of start status on visualized screen
	@param strPoint [in]Start position
	@return
	@remark
	*/
	void SetStartPoint(CString strPoint)
	{
		m_strStartPoint = strPoint;
	}

	/**
	@brief Get point of start status on visualized screen
	@param
	@return Start position
	@remark
	*/
	CString GetStartPoint()
	{
		return m_strStartPoint;
	}

	/**
	@brief Set point of end status on visualized screen
	@param strPoint [in]End position
	@return
	@remark
	*/
	void SetEndtPoint(CString strPoint)
	{
		m_strEndPoint = strPoint;
	}

	/**
	@brief Get point of end status on visualized screen
	@param
	@return End position
	@remark
	*/
	CString GetEndtPoint()
	{
		return m_strEndPoint;
	}
};

class SISStatusPosition
{
public:
	CString m_strStatus;
	int m_nPosX;
	int m_nPosY;

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISStatusPosition()
	{
		m_strStatus = _T("");
		m_nPosX = 0;
		m_nPosY = 0;
	}

	/**
	@brief Creator
	@param c [in]Object of SISWorkFlowRelation
	@return
	@remark Initialize member variable using value of input object
	*/
	SISStatusPosition(const SISStatusPosition& c)
	{
		*this = c;
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISStatusPosition& operator = (const SISStatusPosition &other)
	{
		m_strStatus = other.m_strStatus;
		m_nPosX = other.m_nPosX;
		m_nPosY = other.m_nPosY;

		return *this;
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
	@brief Set status position on visualized screen
	@param nPosX [in]X position
	@param nPosY [in]Y position
	@return
	@remark
	*/
	void SetPosition(int nPosX, int nPosY)
	{
		m_nPosX = nPosX;
		m_nPosY = nPosY;
	}

	/**
	@brief Get status position on visualized screen
	@param nPosX [out]X position
	@param nPosY [out]Y position
	@return
	@remark
	*/
	void GetPosition(int &nPosX, int &nPosY)
	{
		nPosX = m_nPosX;
		nPosY = m_nPosY;
	}
};

class SISWorkFlow
{
public:
	CString m_strFlowName;
	UINT m_nWorkflowID;
	CArray<SISWorkFlowRelation, SISWorkFlowRelation&> m_arrWorkflowRelation;
	CArray<SISStatusPosition, SISStatusPosition&> m_arrStatusPos;

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISWorkFlow()
	{
		m_strFlowName = _T("");
		m_nWorkflowID = 0;
	}

	/**
	@brief Creator
	@param c [in]Object of SISWorkFlow
	@return
	@remark Initialize member variable using value of input object
	*/
	SISWorkFlow(const SISWorkFlow& c)
	{
		*this = c;
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISWorkFlow& operator = (const SISWorkFlow &other)
	{
		m_strFlowName = other.m_strFlowName;
		m_nWorkflowID = other.m_nWorkflowID;
		m_arrWorkflowRelation.Copy(other.m_arrWorkflowRelation);
		m_arrStatusPos.Copy(other.m_arrStatusPos);

		return *this;
	}

	/**
	@brief Set work flow name
	@param strFlowName [in]Work flow name
	@return
	@remark
	*/
	void SetFlowName(CString strFlowName)
	{
		m_strFlowName = strFlowName;
	}

	/**
	@brief Set work flow name
	@param
	@return Work flow name
	@remark
	*/
	CString GetFlowName()
	{
		return m_strFlowName;
	}

	/**
	@brief Set work flow ID
	@param nWorkflowID [out]Work flow ID
	@return
	@remark
	*/
	void SetWorkflowID(UINT nWorkflowID)
	{
		m_nWorkflowID = nWorkflowID;
	}

	/**
	@brief Get work flow ID
	@param
	@return Work flow ID
	@remark
	*/
	UINT GetWorkflowID()
	{
		return m_nWorkflowID;
	}


	/**
	@brief Set relation information of work flow
	@param arrWFRelation [in]Relations information of work flow
	@return
	@remark
	*/
	void SetWorkflowRelation(CArray<SISWorkFlowRelation, SISWorkFlowRelation&>& arrWFRelation)
	{
		m_arrWorkflowRelation.Copy(arrWFRelation);
	}

	/**
	@brief Add relation information of work flow
	@param wfRelation [in]Relation information of work flow
	@return
	@remark
	*/
	void AddWorkflowRelation(SISWorkFlowRelation& wfRelation)
	{
		m_arrWorkflowRelation.Add(wfRelation);
	}

	/**
	@brief Get relation information of work flow
	@param wfRelation [out]Relations information of work flow
	@return 
	@remark
	*/
	void GetWorkflowRelation(CArray<SISWorkFlowRelation, SISWorkFlowRelation&>& arrWFRelation)
	{
		arrWFRelation.Copy(m_arrWorkflowRelation);
	}

	/**
	@brief Remove relation information of work flow
	@param 
	@return
	@remark
	*/
	void RemoveAllWorkflowRelation()
	{
		m_arrWorkflowRelation.RemoveAll();
	}

	/**
	@brief Set status position
	@param arrStatusPos [in]Status position list
	@return
	@remark
	*/
	void SetStatusPosition(CArray<SISStatusPosition, SISStatusPosition&>& arrStatusPos)
	{
		m_arrStatusPos.Copy(arrStatusPos);
	}

	/**
	@brief Add status position
	@param statusPos [in]Status position
	@return
	@remark
	*/
	void AddStatusPosition(SISStatusPosition& statusPos)
	{
		m_arrStatusPos.Add(statusPos);
	}

	/**
	@brief Get status position
	@param arrStatusPos [out]Status position list
	@return
	@remark
	*/
	void GetStatusPosition(CArray<SISStatusPosition, SISStatusPosition&>& arrStatusPos)
	{
		arrStatusPos.Copy(m_arrStatusPos);
	}

	/**
	@brief Remove status position
	@param
	@return
	@remark
	*/
	void RemoveAllStatusPosition()
	{
		m_arrStatusPos.RemoveAll();
	}
};