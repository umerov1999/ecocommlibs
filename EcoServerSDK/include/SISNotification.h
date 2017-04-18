#pragma once
#include <afxwin.h>  
#include <afxext.h>

class SISNotification
{
public:
	UINT m_nNotificationID;
	CString m_strSendUserID;
	CString m_strRecvUserID;
	COleDateTime m_oleSendTime;
	CString m_strNotification;
	NotificationState m_notificationState;

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISNotification()
	{
		m_nNotificationID = 0;
		m_strSendUserID = _T("");
		m_strRecvUserID = _T("");
		m_oleSendTime.m_dt = 0;
		m_strNotification = _T("");
		m_notificationState = NotificationState::NS_NEW;
	}

	/**
	@brief Creator
	@param c [in]Object of SISNotification
	@return
	@remark Initialize member variable using value of input object
	*/
	SISNotification(const SISNotification& c)
	{
		*this = c;
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISNotification& operator = (const SISNotification &other)
	{
		m_nNotificationID = other.m_nNotificationID;
		m_strSendUserID = other.m_strSendUserID;
		m_strRecvUserID = other.m_strRecvUserID;
		m_oleSendTime = other.m_oleSendTime;
		m_strNotification = other.m_strNotification;
		m_notificationState = other.m_notificationState;

		return *this;
	}

	/**
	@brief Set notification ID
	@param nNotiID [in]Notification ID
	@return
	@remark
	*/
	void SetNotificationID(UINT nNotiID)
	{
		m_nNotificationID = nNotiID;
	}

	/**
	@brief Get notification ID
	@param 
	@return Notification ID
	@remark
	*/
	UINT GetNotificationID()
	{
		return m_nNotificationID;
	}

	/**
	@brief Set notification sender
	@param strUserID [in]Sender ID
	@return
	@remark
	*/
	void SetSender(CString strUserID)
	{
		m_strSendUserID = strUserID;
	}

	/**
	@brief Get notification sender
	@param 
	@return Sender ID
	@remark
	*/
	CString GetSender()
	{
		return m_strSendUserID;
	}

	/**
	@brief Set notification receive user
	@param strUserID [in]receive user ID
	@return
	@remark
	*/
	void SetRecvUser(CString strUserID)
	{
		m_strRecvUserID = strUserID;
	}

	/**
	@brief Get notification receive user
	@param 
	@return Receive user ID
	@remark
	*/
	CString GetRecvUser()
	{
		return m_strRecvUserID;
	}

	/**
	@brief Set notification send time
	@param oleSendTime [in]Notification send time
	@return
	@remark
	*/
	void SetSendTime(COleDateTime oleSendTime)
	{
		m_oleSendTime = oleSendTime;
	}

	/**
	@brief Get notification send time
	@param 
	@return Notification send time
	@remark
	*/
	COleDateTime GetSendTime()
	{
		return m_oleSendTime;
	}

	/**
	@brief Set notification message
	@param strMsg [in]Notification message
	@return
	@remark
	*/
	void SetNotification(CString strMsg)
	{
		m_strNotification = strMsg;
	}

	/**
	@brief Get notification
	@param 
	@return Notification message
	@remark
	*/
	CString GetNotification()
	{
		return m_strNotification;
	}
	
	/**
	@brief Set notification state
	@param notiState [in]Notification state
	@return
	@remark
	*/
	void SetNotificationState(NotificationState notiState)
	{
		m_notificationState = notiState;
	}

	/**
	@brief Get notification state
	@param 
	@return Notification state
	@remark
	*/
	NotificationState GetNotificationState()
	{
		return m_notificationState;
	}
};