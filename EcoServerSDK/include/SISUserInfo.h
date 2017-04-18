#pragma once
#include <afxwin.h>  
#include <afxext.h>

class SISUserInfo
{
public:
	CString m_strUserID;
	CString m_strPwd;
	CString m_strCompany;
	CString m_strDepartment;
	CString m_strPosition;
	CString m_strUserName;
	CString m_strPhoneNo;
	CString m_strUserNo;
	CString m_strEmail;
	CString m_strRole;
	BOOL m_bApprovalLogin;
	BOOL m_bApprovalAdmin;

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISUserInfo()
	{
		m_strUserID = _T("");
		m_strPwd = _T("");
		m_strCompany = _T("");
		m_strDepartment = _T("");
		m_strPosition = _T("");
		m_strUserName = _T("");
		m_strPhoneNo = _T("");
		m_strUserNo = _T("");
		m_strEmail = _T("");
		m_strRole = _T("");
		m_bApprovalLogin = FALSE;
		m_bApprovalAdmin = FALSE;
	}

	/**
	@brief Creator
	@param c [in]Object of SISUserInfo
	@return
	@remark Initialize member variable using value of input object
	*/
	SISUserInfo(const SISUserInfo& c)
	{
		*this = c;
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISUserInfo& operator = (const SISUserInfo &other)
	{
		m_strUserID = other.m_strUserID;
		m_strPwd = other.m_strPwd;
		m_strCompany = other.m_strCompany;
		m_strDepartment = other.m_strDepartment;
		m_strPosition = other.m_strPosition;
		m_strUserName = other.m_strUserName;
		m_strPhoneNo = other.m_strPhoneNo;
		m_strUserNo = other.m_strUserNo;
		m_strEmail = other.m_strEmail;
		m_strRole = other.m_strRole;
		m_bApprovalLogin = other.m_bApprovalLogin;
		m_bApprovalAdmin = other.m_bApprovalAdmin;
	
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
	@brief Set user password
	@param strUserID [in]User password
	@return
	@remark
	*/
	void SetPassword(CString strPwd)
	{
		m_strPwd = strPwd;
	}

	/**
	@brief Get user password
	@param
	@return User password
	@remark
	*/
	CString GetPassword()
	{
		return m_strPwd;
	}

	/**
	@brief Set company
	@param strCompany [in]Company
	@return
	@remark
	*/
	void SetCompany(CString strCompany)
	{
		m_strCompany = strCompany;
	}

	/**
	@brief Get company
	@param
	@return Company
	@remark
	*/
	CString GetCompany()
	{
		return m_strCompany;
	}

	/**
	@brief Set department
	@param strDepartment [in]Department
	@return
	@remark
	*/
	void SetDepartment(CString strDepartment)
	{
		m_strDepartment = strDepartment;
	}

	/**
	@brief Get department
	@param
	@return Department
	@remark
	*/
	CString GetDepartment()
	{
		return m_strDepartment;
	}

	/**
	@brief Set position
	@param strPosition [in]Position
	@return 
	@remark
	*/
	void SetPosition(CString strPosition)
	{
		m_strPosition = strPosition;
	}

	/**
	@brief Get position
	@param
	@return Position
	@remark
	*/
	CString GetPosition()
	{
		return m_strPosition;
	}

	/**
	@brief Set user name
	@param strUserName [in]User name
	@return
	@remark
	*/
	void SetUserName(CString strUserName)
	{
		m_strUserName = strUserName;
	}

	/**
	@brief Get user name
	@param
	@return User name
	@remark
	*/
	CString GetUserName()
	{
		return m_strUserName;
	}

	/**
	@brief Set phone number
	@param strPhoneNo [in]Phone number
	@return
	@remark
	*/
	void SetPhoneNo(CString strPhoneNo)
	{
		m_strPhoneNo = strPhoneNo;
	}

	/**
	@brief Get phone number
	@param
	@return Phone number
	@remark
	*/
	CString GetPhoneNo()
	{
		return m_strPhoneNo;
	}

	/**
	@brief Set user number
	@param strUserNo [in]User number
	@return
	@remark
	*/
	void SetUserNo(CString strUserNo)
	{
		m_strUserNo = strUserNo;
	}

	/**
	@brief Get user number
	@param
	@return User number
	@remark
	*/
	CString GetUserNo()
	{
		return m_strUserNo;
	}
	
	/**
	@brief Set user email
	@param strEmail [in]User email
	@return
	@remark
	*/
	void SetEmail(CString strEmail)
	{
		m_strEmail = strEmail;
	}

	/**
	@brief Get user email
	@param
	@return User email
	@remark
	*/
	CString GetEmail()
	{
		return m_strEmail;
	}
	
	/**
	@brief Set role
	@param strRole [in]Role
	@return
	@remark
	*/
	void SetRole(CString strRole)
	{
		m_strRole = strRole;
	}

	/**
	@brief Get role
	@param
	@return Role
	@remark
	*/
	CString GetRole()
	{
		return m_strRole;
	}

	/**
	@brief Set login approval presence or absence
	@param bApprovalLogin [in]Login approval presence or absence
	@return
	@remark
	*/
	void SetApprovalLogin(BOOL bApprovalLogin)
	{
		m_bApprovalLogin = bApprovalLogin;
	}

	/**
	@brief Get login approval presence or absence
	@param
	@return Login approval presence or absence
	@remark
	*/
	BOOL GetApprovalLogin()
	{
		return m_bApprovalLogin;
	}

	/**
	@brief Set administrator approval presence or absence
	@param bApprovalLogin [in]Administrator approval presence or absence
	@return
	@remark
	*/
	void SetApprovalAdmin(BOOL bApprovalAdmin)
	{
		m_bApprovalAdmin = bApprovalAdmin;
	}

	/**
	@brief Get administrator approval presence or absence
	@param
	@return Administrator approval presence or absence
	@remark
	*/
	BOOL GetApprovalAdmin()
	{
		return m_bApprovalAdmin;
	}
};