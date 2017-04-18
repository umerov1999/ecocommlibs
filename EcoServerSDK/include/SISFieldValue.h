#pragma once
#include <afxwin.h>  
#include <afxext.h>

#include "SISDefines.h"

class SISFileValue
{
private:
	CString m_strFileName;
	UINT m_nFileID;

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISFileValue()
	{
		m_strFileName = _T("");
		m_nFileID = 0;
	}

	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISFileValue(const SISFileValue& c)
	{
		*this = c;
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISFileValue& operator = (const SISFileValue &other)
	{
		m_strFileName = other.m_strFileName;
		m_nFileID = other.m_nFileID;

		return *this;
	}

	/**
	@brief Set file name or pathname
	@param strName [in]File name or pathname
	@return 
	@remark
	*/
	void SetFileName(CString strName)
	{
		m_strFileName = strName;
	}

	/**
	@brief Get file name or pathname
	@param 
	@return File name or pathname
	@remark
	*/
	CString GetFileName()
	{
		return m_strFileName;
	}

	/**
	@brief Set file ID
	@param nFileID [in]File ID
	@return
	@remark
	*/
	void SetFileID(UINT nFileID)
	{
		m_nFileID = nFileID;
	}

	/**
	@brief Get file ID
	@param
	@return File ID
	@remark
	*/
	UINT GetFileID()
	{
		return m_nFileID;
	}
};

class SISFieldValue
{
private:
	CString m_strFieldName;
	CArray<FieldType, FieldType&> m_arrFieldType;
	CArray<CString, CString&> m_arrValue;

private:
	BOOL CheckIndex(UINT nIndex)
	{
		if (nIndex < 0 || m_arrFieldType.GetCount() - 1 < nIndex)
			return FALSE;

		return TRUE;
	}

	BOOL GetValue(UINT nIndex, FieldType fieldType, CString& strValue)
	{
		if (m_arrFieldType.GetAt(nIndex) != fieldType)
			return FALSE;

		strValue = m_arrValue.GetAt(nIndex);

		return TRUE;
	}


public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISFieldValue()
	{
		m_strFieldName = _T("");
		m_arrFieldType.RemoveAll();
		m_arrValue.RemoveAll();
	}

	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISFieldValue(const SISFieldValue& c)
	{
		*this = c;
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISFieldValue& operator = (const SISFieldValue &other)
	{
		m_strFieldName = other.m_strFieldName;
		m_arrFieldType.Copy(other.m_arrFieldType);
		m_arrValue.Copy(other.m_arrValue);

		return *this;
	}

	/**
	@brief Initialize member variable
	@param
	@return
	@remark
	*/
	void Init()
	{
		m_strFieldName = _T("");
		m_arrFieldType.RemoveAll();
		m_arrValue.RemoveAll();
	}

	/**
	@brief Set field name
	@param strFieldName [in]Field name
	@return
	@remark
	*/
	void SetFieldName(CString strFieldName)
	{
		m_strFieldName = strFieldName;
	}

	/**
	@brief Get field name
	@param
	@return Field name
	@remark
	*/
	CString GetFieldName()
	{
		return m_strFieldName;
	}

	/**
	@brief Get field type id
	@param arrFieldType [out]Field type id list
	@return
	@remark
	*/
	void GetFieldTypeID(CArray<FieldType, FieldType&>& arrFieldType)
	{
		arrFieldType.Copy(m_arrFieldType);
	}

	/**
	@brief Get field count
	@param
	@return Field coun
	@remark
	*/
	UINT GetFieldCount()
	{
		return m_arrFieldType.GetCount();
	}

	/**
	@brief Set value of select box type
	@param value [in]Value of select box type
	@return 
	@remark
	*/
	void SetSelectBoxValue(CString value)
	{
		FieldType fieldType = FieldType::FT_SELECT_BOX;
		m_arrFieldType.Add(fieldType);
		m_arrValue.Add(value);
	}

	/**
	@brief Get value of select box type
	@param nIndex [in]Index
	@param value [out]Value of select box type
	@return Success presence or absence
	@remark
	*/
	BOOL GetSelectBoxValue(UINT nIndex, CString& value)
	{
		value = _T("");

		if (CheckIndex(nIndex) == FALSE)
			return FALSE;

		if(GetValue(nIndex, FieldType::FT_SELECT_BOX, value) == FALSE)
			return FALSE;

		return TRUE;
	}

	/**
	@brief Set value of text type
	@param value [in]Value of text type
	@return
	@remark
	*/
	void SetTextValue(CString value)
	{
		FieldType fieldType = FieldType::FT_TEXT;
		m_arrFieldType.Add(fieldType);
		m_arrValue.Add(value);
	}

	/**
	@brief Get value of text type
	@param nIndex [in]Index
	@param value [out]Value of text type
	@return Success presence or absence
	@remark
	*/
	BOOL GetTextValue(UINT nIndex, CString& value)
	{
		value = _T("");

		if (CheckIndex(nIndex) == FALSE)
			return FALSE;

		if (GetValue(nIndex, FieldType::FT_TEXT, value) == FALSE)
			return FALSE;

		return TRUE;
	}

	/**
	@brief Set value of multitext type
	@param value [in]Value of multitext type
	@return
	@remark
	*/
	void SetMultiTextValue(CString value)
	{
		FieldType fieldType = FieldType::FT_MULTITEXT;
		m_arrFieldType.Add(fieldType);
		m_arrValue.Add(value);
	}

	/**
	@brief Get value of multitext type
	@param nIndex [in]Index
	@param value [out]Value of multitext type
	@return Success presence or absence
	@remark
	*/
	BOOL GetMultiTextValue(UINT nIndex, CString& value)
	{
		value = _T("");

		if (CheckIndex(nIndex) == FALSE)
			return FALSE;

		if (GetValue(nIndex, FieldType::FT_MULTITEXT, value) == FALSE)
			return FALSE;

		return TRUE;
	}

	/**
	@brief Set value of file type
	@param value [in]Value of file type
	@return
	@remark
	*/
	void SetFileValue(SISFileValue value)
	{
		CString strValue = _T("");

		strValue.Format(_T("%d|%s"), value.GetFileID(), value.GetFileName());

		FieldType fieldType = FieldType::FT_FILE;
		m_arrFieldType.Add(fieldType);
		m_arrValue.Add(strValue);
	}

	/**
	@brief Get value of file type
	@param nIndex [in]Index
	@param value [out]Value of file type
	@return Success presence or absence
	@remark
	*/
	BOOL GetFileValue(UINT nIndex, SISFileValue& value)
	{
		CString strValue = _T("");

		if (CheckIndex(nIndex) == FALSE)
			return FALSE;

		if (GetValue(nIndex, FieldType::FT_FILE, strValue) == FALSE)
			return FALSE;

		UINT nFileID = 0;
		CString strFileName = _T("");
		int nStart = 0;

		nFileID = _wtoi(strValue.Tokenize(_T("|"), nStart));
		strFileName = strValue.Tokenize(_T("|"), nStart);
		
		value.SetFileID(nFileID);
		value.SetFileName(strFileName);

		return TRUE;
	}

	/**
	@brief Set value of multifile type
	@param value [in]Value of multifile type
	@return
	@remark
	*/
	void SetMultiFileValue(CArray<SISFileValue, SISFileValue&>& value)
	{
		FieldType fieldType = FieldType::FT_MULTIFILE;
		CString strVal = _T("");
		CString strTmpVal = _T("");

		m_arrFieldType.Add(fieldType);
		for (int i = 0; i < value.GetCount(); i++)
		{
			strTmpVal.Format(_T("%d|%s"), value.GetAt(i).GetFileID(), value.GetAt(i).GetFileName());
			if (i == 0)
				strVal.Format(_T("%s"), strTmpVal);
			else
				strVal.AppendFormat(_T("@@%s"), strTmpVal);
		}

		m_arrValue.Add(strVal);
	}

	/**
	@brief Get value of multifile type
	@param nIndex [in]Index
	@param value [out]Value of multifile type
	@return Success presence or absence
	@remark
	*/
	BOOL GetMultiFileValue(UINT nIndex, CArray<SISFileValue, SISFileValue&>& value)
	{
		value.RemoveAll();
		CString strValue = _T("");

		if (CheckIndex(nIndex) == FALSE)
			return FALSE;

		if (GetValue(nIndex, FieldType::FT_MULTIFILE, strValue) == FALSE)
			return FALSE;

		SISFileValue tmpFileVal;
		UINT nFileID = 0;
		CString strFileName = _T("");

		CString strTok = _T("");
		int nStart = 0;
		while (!(strTok = strValue.Tokenize(L"@@", nStart)).IsEmpty())
		{
			if (nStart < 0)
				break;

			int nStart2 = 0;
			nFileID = _wtoi(strTok.Tokenize(_T("|"), nStart2));
			strFileName = strTok.Tokenize(_T("|"), nStart2);

			tmpFileVal.SetFileID(nFileID);
			tmpFileVal.SetFileName(strFileName);

			value.Add(tmpFileVal);
		}

		return TRUE;
	}

	/**
	@brief Set value of user type
	@param value [in]Value of user type
	@return
	@remark
	*/
	void SetUserValue(CString value)
	{
		FieldType fieldType = FieldType::FT_USER;
		m_arrFieldType.Add(fieldType);
		m_arrValue.Add(value);
	}

	/**
	@brief Get value of user type
	@param nIndex [in]Index
	@param value [out]Value of user type
	@return Success presence or absence
	@remark
	*/
	BOOL GetUserValue(UINT nIndex, CString& value)
	{
		value = _T("");

		if (CheckIndex(nIndex) == FALSE)
			return FALSE;

		if (GetValue(nIndex, FieldType::FT_USER, value) == FALSE)
			return FALSE;

		return TRUE;
	}

	/**
	@brief Set value of company type
	@param value [in]Value of company type
	@return
	@remark
	*/
	void SetCompanyValue(CString value)
	{
		FieldType fieldType = FieldType::FT_COMPANY;
		m_arrFieldType.Add(fieldType);
		m_arrValue.Add(value);
	}

	/**
	@brief Get value of company type
	@param nIndex [in]Index
	@param value [out]Value of company type
	@return Success presence or absence
	@remark
	*/
	BOOL GetCompanyValue(UINT nIndex, CString& value)
	{
		value = _T("");

		if (CheckIndex(nIndex) == FALSE)
			return FALSE;

		if (GetValue(nIndex, FieldType::FT_COMPANY, value) == FALSE)
			return FALSE;

		return TRUE;
	}

	/**
	@brief Set value of department type
	@param value [in]Value of department type
	@return
	@remark
	*/
	void SetDepartmentValue(CString value)
	{
		FieldType fieldType = FieldType::FT_DEPARTMENT;
		m_arrFieldType.Add(fieldType);
		m_arrValue.Add(value);
	}

	/**
	@brief Get value of department type
	@param nIndex [in]Index
	@param value [out]Value of department type
	@return Success presence or absence
	@remark
	*/
	BOOL GetDepartmentValue(UINT nIndex, CString& value)
	{
		value = _T("");

		if (CheckIndex(nIndex) == FALSE)
			return FALSE;

		if (GetValue(nIndex, FieldType::FT_DEPARTMENT, value) == FALSE)
			return FALSE;

		return TRUE;
	}

	/**
	@brief Set value of position type
	@param value [in]Value of position type
	@return
	@remark
	*/
	void SetPositionValue(CString value)
	{
		FieldType fieldType = FieldType::FT_POSITION;
		m_arrFieldType.Add(fieldType);
		m_arrValue.Add(value);
	}

	/**
	@brief Get value of position type
	@param nIndex [in]Index
	@param value [out]Value of position type
	@return Success presence or absence
	@remark
	*/
	BOOL GetPositionValue(UINT nIndex, CString& value)
	{
		value = _T("");

		if (CheckIndex(nIndex) == FALSE)
			return FALSE;

		if (GetValue(nIndex, FieldType::FT_POSITION, value) == FALSE)
			return FALSE;

		return TRUE;
	}

	/**
	@brief Set value of role type
	@param value [in]Value of role type
	@return
	@remark
	*/
	void SetRoleValue(CString value)
	{
		FieldType fieldType = FieldType::FT_ROLE;
		m_arrFieldType.Add(fieldType);
		m_arrValue.Add(value);
	}

	/**
	@brief Get value of role type
	@param nIndex [in]Index
	@param value [out]Value of role type
	@return Success presence or absence
	@remark
	*/
	BOOL GetRoleValue(UINT nIndex, CString& value)
	{
		value = _T("");

		if (CheckIndex(nIndex) == FALSE)
			return FALSE;

		if (GetValue(nIndex, FieldType::FT_ROLE, value) == FALSE)
			return FALSE;

		return TRUE;
	}

	/**
	@brief Set value of check box type
	@param value [in]Value of check box type
	@return
	@remark
	*/
	void SetCheckBoxValue(BOOL value)
	{
		FieldType fieldType = FieldType::FT_CHECK_BOX;
		CString strVal = _T("");

		m_arrFieldType.Add(fieldType);
		if (value == TRUE)
			strVal = _T("TRUE");
		else
			strVal = _T("FALSE");

		m_arrValue.Add(strVal);
	}

	/**
	@brief Get value of check box type
	@param nIndex [in]Index
	@param value [out]Value of check box type
	@return Success presence or absence
	@remark
	*/
	BOOL GetCheckBoxValue(UINT nIndex, BOOL& value)
	{
		value = FALSE;
		CString strValue = _T("");

		if (CheckIndex(nIndex) == FALSE)
			return FALSE;

		if (GetValue(nIndex, FieldType::FT_CHECK_BOX, strValue) == FALSE)
			return FALSE;

		if (strValue == _T("TRUE"))
			value = TRUE;
		else
			value = FALSE;

		return TRUE;
	}

	/**
	@brief Set value of check list type
	@param value [in]Value of check list type
	@return
	@remark
	*/
	void SetCheckListValue(CArray<CString, CString&>& value)
	{
		FieldType fieldType = FieldType::FT_CHECK_LIST;
		CString strVal = _T("");

		m_arrFieldType.Add(fieldType);
		for (int i = 0; i < value.GetCount(); i++)
		{
			if (i == 0)
				strVal.Format(_T("%s"), value.GetAt(i));
			else
				strVal.AppendFormat(_T("@@%s"), value.GetAt(i));
		}

		m_arrValue.Add(strVal);
	}

	/**
	@brief Get value of check list type
	@param nIndex [in]Index
	@param value [out]Value of check list type
	@return Success presence or absence
	@remark
	*/
	BOOL GetCheckListValue(UINT nIndex, CArray<CString, CString&>& value)
	{
		value.RemoveAll();
		CString strValue = _T("");

		if (CheckIndex(nIndex) == FALSE)
			return FALSE;

		if (GetValue(nIndex, FieldType::FT_CHECK_LIST, strValue) == FALSE)
			return FALSE;

		CString strTok = _T("");
		int nStart = 0;
		while (!(strTok = strValue.Tokenize(L"@@", nStart)).IsEmpty())
		{
			if (nStart < 0)
				break;

			value.Add(strTok);
		}

		return TRUE;
	}

	/**
	@brief Set value of date type
	@param nYear [in]Year
	@param nMonth [in]Month
	@param nDay [in]Day
	@return
	@remark
	*/
	void SetDateValue(UINT nYear, UINT nMonth, UINT nDay)
	{
		FieldType fieldType = FieldType::FT_DATE;
		CString strVal = _T("");

		strVal.Format(_T("%4d-%02d-%02d"), nYear, nMonth, nDay);

		m_arrFieldType.Add(fieldType);
		m_arrValue.Add(strVal);
	}

	/**
	@brief Get value of date type
	@param nIndex [in]Index
	@param nYear [out]Year
	@param nMonth [out]Month
	@param nDay [out]Day
	@return Success presence or absence
	@remark
	*/
	BOOL GetDateValue(UINT nIndex, UINT& nYear, UINT& nMonth, UINT& nDay)
	{
		CString strValue = _T("");

		if (CheckIndex(nIndex) == FALSE)
			return FALSE;

		if (GetValue(nIndex, FieldType::FT_DATE, strValue) == FALSE)
			return FALSE;


		CString strTok = _T("");
		int nStart = 0;
		strTok = strValue.Tokenize(L"-", nStart);
		if (strTok == _T(""))
			return FALSE;
		nYear = _wtoi(strTok);

		strTok = strValue.Tokenize(L"-", nStart);
		if (strTok == _T(""))
			return FALSE;
		nMonth = _wtoi(strTok);

		strTok = strValue.Tokenize(L"-", nStart);
		if (strTok == _T(""))
			return FALSE;
		nDay = _wtoi(strTok);

		return TRUE;
	}

	/**
	@brief Set value of time type
	@param nHour [in]Hour
	@param nMinute [in]Minute
	@param nSecond [in]Second
	@return
	@remark
	*/
	void SetTimeValue(UINT nHour, UINT nMinute, UINT nSecond)
	{
		FieldType fieldType = FieldType::FT_TIME;
		CString strVal = _T("");

		strVal.Format(_T("%02d:%02d:%02d"), nHour, nMinute, nSecond);

		m_arrFieldType.Add(fieldType);
		m_arrValue.Add(strVal);
	}

	/**
	@brief Get value of time type
	@param nIndex [in]Index
	@param nHour [out]Hour
	@param nMinute [out]Minute
	@param nSecond [out]Second
	@return Success presence or absence
	@remark
	*/
	BOOL GetTimeValue(UINT nIndex, UINT& nHour, UINT& nMinute, UINT& nSecond)
	{
		CString strValue = _T("");

		if (CheckIndex(nIndex) == FALSE)
			return FALSE;

		if (GetValue(nIndex, FieldType::FT_TIME, strValue) == FALSE)
			return FALSE;


		CString strTok = _T("");
		int nStart = 0;
		strTok = strValue.Tokenize(L":", nStart);
		if (strTok == _T(""))
			return FALSE;
		nHour = _wtoi(strTok);

		strTok = strValue.Tokenize(L":", nStart);
		if (strTok == _T(""))
			return FALSE;
		nMinute = _wtoi(strTok);

		strTok = strValue.Tokenize(L":", nStart);
		if (strTok == _T(""))
			return FALSE;
		nSecond = _wtoi(strTok);

		return TRUE;
	}

	/**
	@brief Set value of datetime type
	@param value [in]datetime
	@return
	@remark
	*/
	void SetDateTimeValue(COleDateTime value)
	{
		FieldType fieldType = FieldType::FT_DATE_TIME;
		CString strVal = _T("");
		UINT nHour = value.GetHour();
		UINT nMinute = value.GetMinute();
		UINT nSecond = value.GetSecond();
		UINT nYear = value.GetYear();
		UINT nMonth = value.GetMonth();
		UINT nDay = value.GetDay();
		
		strVal.Format(_T("%4d-%02d-%02d %02d:%02d:%02d"), nYear, nMonth, nDay, nHour, nMinute, nSecond);

		m_arrFieldType.Add(fieldType);
		m_arrValue.Add(strVal);
	}

	/**
	@brief Get value of datetime type
	@param nIndex [in]Index
	@param value [out]Datetime
	@return Success presence or absence
	@remark
	*/
	BOOL GetDateTimeValue(UINT nIndex, COleDateTime& value)
	{
		CString strValue = _T("");

		if (CheckIndex(nIndex) == FALSE)
			return FALSE;

		if (GetValue(nIndex, FieldType::FT_DATE_TIME, strValue) == FALSE)
			return FALSE;

		UINT nHour, nMinute, nSecond;
		UINT nYear, nMonth, nDay;
		CString strDate = _T("");
		CString strTime = _T("");
		CString strTok = _T("");
		int nStart = 0;

		//Date
		strDate = strValue.Tokenize(L" ", nStart);
		if (strDate == _T(""))
			return FALSE;

		//Time
		strTime = strValue.Tokenize(L" ", nStart);
		if (strTime == _T(""))
			return FALSE;

		//Parsing Date
		nStart = 0;
		strTok = strDate.Tokenize(L"-", nStart);
		if (strTok == _T(""))
			return FALSE;
		nYear = _wtoi(strTok);

		strTok = strDate.Tokenize(L"-", nStart);
		if (strTok == _T(""))
			return FALSE;
		nMonth = _wtoi(strTok);

		strTok = strDate.Tokenize(L"-", nStart);
		if (strTok == _T(""))
			return FALSE;
		nDay = _wtoi(strTok);

		//Parsing Time
		nStart = 0;
		strTok = strTime.Tokenize(L":", nStart);
		if (strTok == _T(""))
			return FALSE;
		nHour = _wtoi(strTok);

		strTok = strTime.Tokenize(L":", nStart);
		if (strTok == _T(""))
			return FALSE;
		nMinute = _wtoi(strTok);

		strTok = strTime.Tokenize(L":", nStart);
		if (strTok == _T(""))
			return FALSE;
		nSecond = _wtoi(strTok);

		value.SetDateTime(nYear, nMonth, nDay, nHour, nMinute, nSecond);

		return TRUE;
	}

	/**
	@brief Set value of number type
	@param value [in]number
	@return
	@remark
	*/
	void SetNumberValue(double value)
	{
		FieldType fieldType = FieldType::FT_NUMBER;
		CString strVal = _T("");

		strVal.Format(_T("%.7f"), value);

		m_arrFieldType.Add(fieldType);
		m_arrValue.Add(strVal);
	}

	/**
	@brief Get value of number type
	@param nIndex [in]Index
	@param value [out]Value of number type
	@return Success presence or absence
	@remark
	*/
	BOOL GetNumberValue(UINT nIndex, double& value)
	{
		value = 0.0;
		CString strValue = _T("");

		if (CheckIndex(nIndex) == FALSE)
			return FALSE;

		if (GetValue(nIndex, FieldType::FT_NUMBER, strValue) == FALSE)
			return FALSE;

		value = _wtof(strValue);

		return TRUE;
	}

	/**
	@brief Set value of project link type
	@param nValue [in]Value of project link type(Project ID)
	@return
	@remark
	*/
	void SetProjectLinkValue(UINT nValue)
	{
		FieldType fieldType = FieldType::FT_PROJECT_LINK;
		m_arrFieldType.Add(fieldType);

		CString strValue = _T("");
		strValue.Format(_T("%d"), nValue);

		m_arrValue.Add(strValue);
	}

	/**
	@brief Get value of project link type
	@param nIndex [in]Index
	@param nValue [out]Value of project link type(Project ID)
	@return Success presence or absence
	@remark
	*/
	BOOL GetProjectLinkValue(UINT nIndex, UINT& nValue)
	{
		nValue = 0;
		CString strValue = _T("");

		if (CheckIndex(nIndex) == FALSE)
			return FALSE;

		if (GetValue(nIndex, FieldType::FT_PROJECT_LINK, strValue) == FALSE)
			return FALSE;

		nValue = _wtoi(strValue);

		return TRUE;
	}
};