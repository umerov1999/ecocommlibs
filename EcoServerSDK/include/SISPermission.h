#pragma once

#ifndef _SISPermission_H
#define _SISPermission_H

#include <afxwin.h>  
#include <afxext.h>

class SISPermissionInfo
{
public:
	BOOL m_bDownload;
	BOOL m_bUpload;
	BOOL m_bModify;
	BOOL m_bDelete;

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISPermissionInfo()
	{
		m_bDownload = TRUE;
		m_bUpload = TRUE;
		m_bModify = FALSE;
		m_bDelete = FALSE;
	}

	/**
	@brief Creator
	@param c [in]Object of SISPermissionInfo
	@return
	@remark Initialize member variable using value of input object
	*/
	SISPermissionInfo(const SISPermissionInfo& c)
	{
		*this = c;
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISPermissionInfo& operator = (const SISPermissionInfo &other)
	{
		m_bDownload = other.m_bDownload;
		m_bUpload = other.m_bUpload;
		m_bModify = other.m_bModify;
		m_bDelete = other.m_bDelete;
	
		return *this;
	}

	/**
	@brief Set download permission
	@param bDownload [in]Download permission
	@return
	@remark Project and attached file
	*/
	void SetDownloadPermission(BOOL bDownload)
	{
		m_bDownload = bDownload;
	}

	/**
	@brief Get download permission
	@param
	@return Download permission
	@remark Project and attached file
	*/
	BOOL GetDownloadPermission()
	{
		return m_bDownload;
	}

	/**
	@brief Set upload permission
	@param bUpload [in]Upload permission
	@return
	@remark
	*/
	void SetUploadPermission(BOOL bUpload)
	{
		m_bUpload = bUpload;
	}

	/**
	@brief Get upload permission
	@param
	@return Upload permission
	@remark
	*/
	BOOL GetUploadPermission()
	{
		return m_bUpload;
	}

	/**
	@brief Set modify permission
	@param bModify [in]Modify permission
	@return
	@remark
	*/
	void SetModifyPermission(BOOL bModify)
	{
		m_bModify = bModify;
	}

	/**
	@brief Get modify permission
	@param
	@return Modify permission
	@remark
	*/
	BOOL GetModifyPermission()
	{
		return m_bModify;
	}

	/**
	@brief Set delete permission
	@param bDelete [in]Delete permission
	@return 
	@remark
	*/
	void SetDeletePermission(BOOL bDelete)
	{
		m_bDelete = bDelete;
	}

	/**
	@brief Get delete permission
	@param
	@return Delete permission
	@remark
	*/
	BOOL GetDeletePermission()
	{
		return m_bDelete;
	}
};


class SISAffiliationPermission
{
private:
	UserAffiliation m_userAffiliationType;
	CString m_strAffiliationName;

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISAffiliationPermission()
	{
		m_userAffiliationType = UserAffiliation::UA_DEPARTMENT;
		m_strAffiliationName = _T("");
	}

	/**
	@brief Creator
	@param c [in]Object of SISAffiliationPermission
	@return
	@remark Initialize member variable using value of input object
	*/
	SISAffiliationPermission(const SISAffiliationPermission& c)
	{
		*this = c;
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISAffiliationPermission& operator = (const SISAffiliationPermission &other)
	{
		m_userAffiliationType = other.m_userAffiliationType;
		m_strAffiliationName = other.m_strAffiliationName;

		return *this;
	}

	/**
	@brief Set affiliation type
	@param type [in]Affiliation type
	@return
	@remark
	*/
	void SetAffiliationType(UserAffiliation type)
	{
		m_userAffiliationType = type;
	}

	/**
	@brief Get affiliation type
	@param 
	@return Affiliation type
	@remark
	*/
	UserAffiliation GetAffiliationType()
	{
		return m_userAffiliationType;
	}

	/**
	@brief Set affiliation name
	@param strName [in]Affiliation name
	@return
	@remark
	*/
	void SetAffiliationName(CString strName)
	{
		m_strAffiliationName = strName;
	}

	/**
	@brief Get affiliation name
	@param
	@return Affiliation name
	@remark
	*/
	CString GetAffiliationName()
	{
		return m_strAffiliationName;
	}
};

class SISBlockPermission : public SISAffiliationPermission
{
private:
	CString m_strGroup;
	CString m_strShipNo;
	CString m_strBlockName;
	BOOL m_bSubPermi;

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISBlockPermission()
	{
		m_strGroup = _T("");
		m_strShipNo = _T("");
		m_strBlockName = _T("");
		m_bSubPermi = FALSE;
	}

	/**
	@brief Creator
	@param c [in]Object of SISBlockPermission
	@return
	@remark Initialize member variable using value of input object
	*/
	SISBlockPermission(const SISBlockPermission& c)
	{
		*this = c;
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISBlockPermission& operator = (const SISBlockPermission &other)
	{
		SISAffiliationPermission::operator=(other);

		m_strGroup = other.m_strGroup;
		m_strShipNo = other.m_strShipNo;
		m_strBlockName = other.m_strBlockName;
		m_bSubPermi = other.m_bSubPermi;

		return *this;
	}

	/**
	@brief Set group of ship number
	@param [in]strGroup Group of ship number
	@return
	@remark
	*/
	void SetGroup(CString strGroup)
	{
		m_strGroup = strGroup;
	}

	/**
	@brief Get group of ship number
	@param
	@return group of ship number
	@remark
	*/
	CString GetGroup()
	{
		return m_strGroup;
	}

	/**
	@brief Set ship number
	@param [in]strGroup Group of ship number
	@return
	@remark
	*/
	void SetShipNo(CString strShipNo)
	{
		m_strShipNo = strShipNo;
	}

	/**
	@brief Get ship number
	@param
	@return ship number
	@remark
	*/
	CString GetShipNo()
	{
		return m_strShipNo;
	}

	/**
	@brief Set block name
	@param strName [in]Block name
	@return
	@remark
	*/
	void SetBlockName(CString strName)
	{
		m_strBlockName = strName;
	}

	/**
	@brief Get block name
	@param
	@return Block name
	@remark
	*/
	CString GetBlockName()
	{
		return m_strBlockName;
	}

	/**
	@brief Set permission of sub block
	@param bSubBlockPermi [in]Block name
	@return
	@remark
	*/
	void SetBSubBlockPermission(BOOL bSubBlockPermi)
	{
		m_bSubPermi = bSubBlockPermi;
	}

	/**
	@brief Get permission of sub block
	@param
	@return 
	@remark
	*/
	BOOL GetBSubBlockPermission()
	{
		return m_bSubPermi;
	}
};

class SISShipNoPermission : public SISAffiliationPermission
{
private:
	AssemTreeComposition m_shipNoType;
	CString m_strGroup;
	CString m_strShipNo;

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISShipNoPermission()
	{
		m_strGroup = _T("");
		m_strShipNo = _T("");
		m_shipNoType = AssemTreeComposition::ATC_SHIPNO;
	}

	/**
	@brief Creator
	@param c [in]Object of SISShipNoPermission
	@return
	@remark Initialize member variable using value of input object
	*/
	SISShipNoPermission(const SISShipNoPermission& c)
	{
		*this = c;
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISShipNoPermission& operator = (const SISShipNoPermission &other)
	{
		SISAffiliationPermission::operator=(other);

		m_strGroup = other.m_strGroup;
		m_strShipNo = other.m_strShipNo;
		m_shipNoType = other.m_shipNoType;

		return *this;
	}

	/**
	@brief Set type of ship number
	@param type [in]Composition of ship number(Using 'MASTER', 'GROUP', 'SHIPNO')
	@return
	@remark
	*/
	void SetShipNoType(AssemTreeComposition type)
	{
		m_shipNoType = type;
	}

	/**
	@brief Set type of ship number
	@param 
	@return Composition of ship number
	@remark
	*/
	AssemTreeComposition GetShipNoType()
	{
		return m_shipNoType;
	}

	/**
	@brief Set group of ship number
	@param strGroup [in]strGroup Group of ship number
	@return
	@remark
	*/
	void SetGroup(CString strGroup)
	{
		m_strGroup = strGroup;
	}

	/**
	@brief Get group of ship number
	@param
	@return group of ship number
	@remark
	*/
	CString GetGroup()
	{
		return m_strGroup;
	}

	/**
	@brief Set ship number
	@param strName [in]Ship number
	@return
	@remark
	*/
	void SetShipNo(CString strName)
	{
		m_strShipNo = strName;
	}

	/**
	@brief Get ship number
	@param
	@return Ship number
	@remark
	*/
	CString GetShipNo()
	{
		return m_strShipNo;
	}
};

class SISStatusPermission : public SISPermissionInfo
{
public:
	CString m_strStatus;

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISStatusPermission()
	{
		m_strStatus = _T("");
	}

	/**
	@brief Creator
	@param c [in]Object of SISStatusPermission
	@return
	@remark Initialize member variable using value of input object
	*/
	SISStatusPermission(const SISStatusPermission& c)
	{
		*this = c;
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISStatusPermission& operator = (const SISStatusPermission &other)
	{
		SISPermissionInfo::operator=(other);

		m_strStatus = other.m_strStatus;

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
};

class SISWorkflowPermission
{
public:
	CString m_strWorkflowName;
	CArray<SISStatusPermission, SISStatusPermission&> m_arrStatusPermission;

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISWorkflowPermission()
	{
		m_strWorkflowName = _T("");
		m_arrStatusPermission.RemoveAll();
	}

	/**
	@brief Creator
	@param c [in]Object of SISWorkflowPermission
	@return
	@remark Initialize member variable using value of input object
	*/
	SISWorkflowPermission(const SISWorkflowPermission& c)
	{
		*this = c;
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISWorkflowPermission& operator = (const SISWorkflowPermission &other)
	{
		m_strWorkflowName = other.m_strWorkflowName;
		m_arrStatusPermission.Copy(other.m_arrStatusPermission);

		return *this;
	}

	/**
	@brief Set workflow name
	@param strWorkflow [in]Workflow name
	@return
	@remark
	*/
	void SetWorkflow(CString strWorkflow)
	{
		m_strWorkflowName = strWorkflow;
	}

	/**
	@brief Get workflow name
	@param
	@return Workflow name
	@remark
	*/
	CString GetWorkflow()
	{
		return m_strWorkflowName;
	}

	/**
	@brief Add Permission of status
	@param statusPermission [in]Permission of status
	@return Success presence or absence
	@remark
	*/
	BOOL AddStatusPermission(SISStatusPermission statusPermission)
	{
		if (statusPermission.GetStatus() == _T(""))
			return FALSE;

		BOOL bExist = FALSE;
		for (int i = 0; i < m_arrStatusPermission.GetCount(); i++)
		{
			if (m_arrStatusPermission.GetAt(i).GetStatus() == statusPermission.GetStatus())
			{
				bExist = TRUE;
				break;
			}
		}
		if (bExist == TRUE)
			return FALSE;

		m_arrStatusPermission.Add(statusPermission);

		return TRUE;
	}

	/**
	@brief Remove Permission of status
	@param
	@return
	@remark
	*/
	void RemoveStatusPermission()
	{
		m_arrStatusPermission.RemoveAll();
	}

	/**
	@brief Remove Permission of status
	@param strStatus [in]Status name
	@return
	@remark
	*/
	void RemoveStatusPermission(CString strStatus)
	{
		for (int i = m_arrStatusPermission.GetCount() - 1; i >= 0; i--)
		{
			if (m_arrStatusPermission.GetAt(i).GetStatus() == strStatus)
			{
				m_arrStatusPermission.RemoveAt(i);
			}
		}
	}

	/**
	@brief Get Permission of status
	@param arrStatusPermission [out]Permission list of status
	@return
	@remark
	*/
	void GetStatusPermission(CArray<SISStatusPermission, SISStatusPermission&>& arrStatusPermission)
	{
		arrStatusPermission.Copy(m_arrStatusPermission);
	}

	/**
	@brief Get Permission of status
	@param strStatus [in]Status name
	@param statusPermission [out]Permission of status
	@return Success presence or absence
	@remark
	*/
	BOOL GetStatusPermission(CString strStatus, SISStatusPermission& statusPermission)
	{
		BOOL bExist = FALSE;
		int i = 0;
		for (i; i < m_arrStatusPermission.GetCount(); i++)
		{
			if (m_arrStatusPermission.GetAt(i).GetStatus() == strStatus)
			{
				bExist = TRUE;
				break;
			}
		}

		if (bExist == FALSE)
			return FALSE;

		statusPermission = m_arrStatusPermission.GetAt(i);

		return TRUE;
	}
};

class SISUserDefinePermission : public SISWorkflowPermission
{
public:
	CString m_strUserID;

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISUserDefinePermission()
	{
		m_strUserID = _T("");
	}

	/**
	@brief Creator
	@param c [in]Object of SISUserDefinePermission
	@return
	@remark Initialize member variable using value of input object
	*/
	SISUserDefinePermission(const SISUserDefinePermission& c)
	{
		*this = c;
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISUserDefinePermission& operator = (const SISUserDefinePermission &other)
	{
		SISWorkflowPermission::operator=(other);

		m_strUserID = other.m_strUserID;

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
};

class SISPermissionGroup : public SISWorkflowPermission
{
public:
	CString m_strPermissionGroup;

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISPermissionGroup()
	{
		m_strPermissionGroup = _T("");
	}

	/**
	@brief Creator
	@param c [in]Object of SISPermissionGroup
	@return
	@remark Initialize member variable using value of input object
	*/
	SISPermissionGroup(const SISPermissionGroup& c)
	{
		*this = c;
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISPermissionGroup& operator = (const SISPermissionGroup &other)
	{
		SISWorkflowPermission::operator=(other);

		m_strPermissionGroup = other.m_strPermissionGroup;

		return *this;
	}

	/**
	@brief Set permission group
	@param strGroupName [in]Permission group name
	@return
	@remark
	*/
	void SetPermissionGroup(CString strGroupName)
	{
		m_strPermissionGroup = strGroupName;
	}

	/**
	@brief Get permission group
	@param 
	@return Permission group name
	@remark
	*/
	CString GetPermissionGroup()
	{
		return m_strPermissionGroup;
	}
};


class SISProgramPermission
{
public:
	CString m_strUserID;
	ProgramPermiStatus m_nPjtCreate;
	ProgramPermiStatus m_nPjtDownload;
	ProgramPermiStatus m_nPjtModify;
	ProgramPermiStatus m_nPjtDelete;
	ProgramPermiStatus m_nTplCreate;
	ProgramPermiStatus m_nTplModify;
	ProgramPermiStatus m_nTplDelete;

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISProgramPermission()
	{
		m_strUserID = _T("");
		m_nPjtCreate = ProgramPermiStatus::PPS_NO;
		m_nPjtDownload = ProgramPermiStatus::PPS_NO;
		m_nPjtModify = ProgramPermiStatus::PPS_NO;
		m_nPjtDelete = ProgramPermiStatus::PPS_NO;
		m_nTplCreate = ProgramPermiStatus::PPS_NO;
		m_nTplModify = ProgramPermiStatus::PPS_NO;
		m_nTplDelete = ProgramPermiStatus::PPS_NO;
	}

	/**
	@brief Creator
	@param c [in]Object of SISProgramPermission
	@return
	@remark Initialize member variable using value of input object
	*/
	SISProgramPermission(const SISProgramPermission& c)
	{
		*this = c;
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISProgramPermission& operator = (const SISProgramPermission &other)
	{
		m_strUserID = other.m_strUserID;
		m_nPjtCreate = other.m_nPjtCreate;
		m_nPjtDownload = other.m_nPjtDownload;
		m_nPjtModify = other.m_nPjtModify;
		m_nPjtDelete = other.m_nPjtDelete;
		m_nTplCreate = other.m_nTplCreate;
		m_nTplModify = other.m_nTplModify;
		m_nTplDelete = other.m_nTplDelete;

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
	@brief Set project create permission
	@param permiStatus [in]Permission status(PPS_YES or PPS_NO)
	@return
	@remark 
	*/
	void SetProjectCreatePermi(ProgramPermiStatus permiStatus)
	{
		m_nPjtCreate = permiStatus;
	}

	/**
	@brief Get project create permission
	@param
	@return Permission status(PPS_YES or PPS_NO)
	@remark 
	*/
	ProgramPermiStatus GetProjectCreatePermi()
	{
		return m_nPjtCreate;
	}

	/**
	@brief Set project download permission
	@param bDownload [in]Permission status(PPS_YES or PPS_NO)
	@return
	@remark 
	*/
	void SetProjectDownloadPermi(ProgramPermiStatus permiStatus)
	{
		m_nPjtDownload = permiStatus;
	}

	/**
	@brief Get project download permission
	@param
	@return Permission status(PPS_YES or PPS_NO)
	@remark 
	*/
	ProgramPermiStatus GetProjectDownloadPermi()
	{
		return m_nPjtDownload;
	}

	/**
	@brief Set project modify permission
	@param permiStatus [in]Permission status(PPS_YES or PPS_NO or PPS_OWN)
	@return
	@remark
	*/
	void SetProjectModifyPermi(ProgramPermiStatus permiStatus)
	{
		m_nPjtModify = permiStatus;
	}

	/**
	@brief Get project modify permission
	@param
	@return Permission status(PPS_YES or PPS_NO or PPS_OWN)
	@remark
	*/
	ProgramPermiStatus GetProjectModifyPermi()
	{
		return m_nPjtModify;
	}

	/**
	@brief Set project delete permission
	@param permiStatus [in]Permission status(PPS_YES or PPS_NO or PPS_OWN)
	@return
	@remark
	*/
	void SetProjectDeletePermi(ProgramPermiStatus permiStatus)
	{
		m_nPjtDelete = permiStatus;
	}

	/**
	@brief Get project delete permission
	@param
	@return Permission status(PPS_YES or PPS_NO or PPS_OWN)
	@remark
	*/
	ProgramPermiStatus GetProjectDeletePermi()
	{
		return m_nPjtDelete;
	}

	/**
	@brief Set template create permission
	@param permiStatus [in]Permission status(PPS_YES or PPS_NO)
	@return
	@remark
	*/
	void SetTemplateCreatePermi(ProgramPermiStatus permiStatus)
	{
		m_nTplCreate = permiStatus;
	}

	/**
	@brief Get template create permission
	@param
	@return Permission status(PPS_YES or PPS_NO)
	@remark
	*/
	ProgramPermiStatus GetTemplateCreatePermi()
	{
		return m_nTplCreate;
	}

	/**
	@brief Set template modify permission
	@param permiStatus [in]Permission status(PPS_YES or PPS_NO)
	@return
	@remark
	*/
	void SetTemplateModifyPermi(ProgramPermiStatus permiStatus)
	{
		m_nTplModify = permiStatus;
	}

	/**
	@brief Get template modify permission
	@param
	@return Permission status(PPS_YES or PPS_NO)
	@remark
	*/
	ProgramPermiStatus GetTemplateModifyPermi()
	{
		return m_nTplModify;
	}

	/**
	@brief Set template delete permission
	@param permiStatus [in]Permission status(PPS_YES or PPS_NO)
	@return
	@remark
	*/
	void SetTemplateDeletePermi(ProgramPermiStatus permiStatus)
	{
		m_nTplDelete = permiStatus;
	}

	/**
	@brief Get template delete permission
	@param
	@return Permission status(PPS_YES or PPS_NO)
	@remark
	*/
	ProgramPermiStatus GetTemplateDeletePermi()
	{
		return m_nTplDelete;
	}
};

#endif