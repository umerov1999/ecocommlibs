#pragma once

#ifndef _SISAssign_H
#define _SISAssign_H

#include <afxwin.h>  
#include <afxext.h>

class SISAssignUser
{
public:
	CArray<CString, CString&> m_arrAssignUser;

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISAssignUser()
	{
		m_arrAssignUser.RemoveAll();
	}

	/**
	@brief Creator
	@param c [in]Object of SISAssignUser
	@return
	@remark Initialize member variable using value of input object
	*/
	SISAssignUser(const SISAssignUser& c)
	{
		*this = c;
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISAssignUser& operator = (const SISAssignUser &other)
	{
		m_arrAssignUser.Copy(other.m_arrAssignUser);
	
		return *this;
	}


	/**
	@brief Set assign user
	@param strAssignUserID [in]User ID
	@param arrCandiUserID [in]Candidate user ID
	@return
	@remark
	arrCandiUserID - High priority user first input
	*/
	void SetAssignUser(CString strAssignUserID, CArray<CString, CString&>& arrCandiUserID)
	{
		m_arrAssignUser.RemoveAll();
		m_arrAssignUser.Add(strAssignUserID);
		m_arrAssignUser.Append(arrCandiUserID);
	}

	/**
	@brief Get assign user
	@param strAssignUserID [out]User ID
	@param arrCandiUserID [out]Candidate user ID
	@return 
	@remark
	arrCandiUserID - High priority user first input
	*/
	void GetAssignUser(CString& strAssignUserID, CArray<CString, CString&>& arrCandiUserID)
	{
		strAssignUserID = _T("");
		arrCandiUserID.RemoveAll();

		if (m_arrAssignUser.GetCount() > 0)
			strAssignUserID = m_arrAssignUser.GetAt(0);

		for (int i = 1; i < m_arrAssignUser.GetCount(); i++)
		{
			arrCandiUserID.Add(m_arrAssignUser.GetAt(i));
		}
	}

};


class SISAssign : public SISAssignUser
{
public:
	CString m_strChangeUser;
	COleDateTime m_dtChangeTime;
	BOOL m_bAssignConfirm;
	COleDateTime m_dtAssignConfirmTime;

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISAssign()
	{
		m_strChangeUser = _T("");
		m_dtChangeTime.m_dt = 0;
		m_bAssignConfirm = FALSE;
		m_dtAssignConfirmTime.m_dt = 0;
	}

	/**
	@brief Creator
	@param c [in]Object of SISAssign
	@return
	@remark Initialize member variable using value of input object
	*/
	SISAssign(const SISAssign& c)
	{
		*this = c;
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISAssign& operator = (const SISAssign &other)
	{
		SISAssignUser::operator=(other);

		m_strChangeUser = other.m_strChangeUser;
		m_dtChangeTime = other.m_dtChangeTime;
		m_bAssignConfirm = other.m_bAssignConfirm;
		m_dtAssignConfirmTime = other.m_dtAssignConfirmTime;

		return *this;
	}

	/**
	@brief Set assign changer
	@param strChangeUserID [in]Assign changer
	@return
	@remark
	*/
	void SetChangeUser(CString strChangeUserID)
	{
		m_strChangeUser = strChangeUserID;
	}

	/**
	@brief Get assign changer
	@param 
	@return Assign changer
	@remark
	*/
	CString GetChangeUser()
	{
		return m_strChangeUser;
	}

	/**
	@brief Set assign change time
	@param dtChangeTime [in]Assign change time
	@return
	@remark
	*/
	void SetChangeTime(COleDateTime dtChangeTime)
	{
		m_dtChangeTime = dtChangeTime;
	}

	/**
	@brief Get assign change time
	@param 
	@return Assign change time
	@remark
	*/
	COleDateTime GetChangeTime()
	{
		return m_dtChangeTime;
	}

	/**
	@brief Set assign confirm
	@param bAssignConfirm [in]Assign confirm presence or absence
	@return
	@remark
	*/
	void SetAssignConfirm(BOOL bAssignConfirm)
	{
		m_bAssignConfirm = bAssignConfirm;
	}

	/**
	@brief Get assign confirm
	@param 
	@return Assign confirm presence or absence
	@remark
	*/
	BOOL GetAssignConfirm()
	{
		return m_bAssignConfirm;
	}

	/**
	@brief Set assign confirm time
	@param dtAssignConfirmTime [in]Assign confirm time
	@return
	@remark
	*/
	void SetAssignConfirmTime(COleDateTime dtAssignConfirmTime)
	{
		m_dtAssignConfirmTime = dtAssignConfirmTime;
	}

	/**
	@brief Get assign confirm time
	@param 
	@return Assign confirm time
	@remark
	*/
	COleDateTime GetAssignConfirmTime()
	{
		return m_dtAssignConfirmTime;
	}
};


class SISAssignHistory
{
private:
	UINT m_nWorkID;
	CArray<SISAssign, SISAssign&> m_arrAssign;

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISAssignHistory()
	{
		m_nWorkID = 0;
		m_arrAssign.RemoveAll();
	}

	/**
	@brief Creator
	@param c [in]Object of SISAssignHistory
	@return
	@remark Initialize member variable using value of input object
	*/
	SISAssignHistory(const SISAssignHistory& c)
	{
		*this = c;
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISAssignHistory& operator = (const SISAssignHistory &other)
	{
		m_nWorkID = other.m_nWorkID;
		m_arrAssign.Copy(other.m_arrAssign);

		return *this;
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
	@brief Add assign information
	@param assign [in]Assign information
	@return
	@remark
	*/
	void AddAssign(SISAssign assign)
	{
		m_arrAssign.Add(assign);
	}

	/**
	@brief Delete all assign information
	@param 
	@return
	@remark
	*/
	void DeleteAllAssign()
	{
		m_arrAssign.RemoveAll();
	}

	/**
	@brief Get assign information
	@param nStep [in]Assign Step
	@param assign [out]Assign information
	@return Success presence or absence
	@remark
	*/
	BOOL GetAssign(UINT nStep, SISAssign &assign)
	{
		if (nStep < 0 || m_arrAssign.GetCount() - 1 < nStep)
			return FALSE;

		assign = m_arrAssign.GetAt(nStep);

		return TRUE;
	}

	/**
	@brief Get assign step count
	@param 
	@return Assign step count
	@remark
	*/
	UINT GetAssignStepCnt()
	{
		return m_arrAssign.GetCount();
	}

};

#endif