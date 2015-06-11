// XTPReportDataManager.h: interface for the CXTPReportDataManager class.
//
// This file is a part of the XTREME REPORTCONTROL MFC class library.
// (c)1998-2013 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
/////////////////////////////////////////////////////////////////////////////

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPREPORTDATAMANAGER_H__)
#define __XTPREPORTDATAMANAGER_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CXTPReportRecord;
class CXTPReportRecords;
class CXTPReportRecordItemVariant;
class CXTPReportDataManager;
class CXTPReportControl;
class CXTPReportRecordItem;

#define XTP_REPORTDATAMANAGER_MAX_LAST_COM_ERRORS 3

//===========================================================================
// Summary:
//     Basic set of parameters transferred to item handlers.
//     This structure groups together essential parameters used in
//     items processing activities. It is parent for all other structures:
//     XTP_REPORTRECORDITEM_DRAWARGS, XTP_REPORTRECORDITEM_CLICKARGS
// Example:
// <code>
// XTP_REPORTRECORDITEM_ARGS itemArgs(pControl, pRow, pColumn);
// pItem->OnBeginEdit(&itemArgs);
// </code>
// See Also: CXTPReportRecordItem::OnBeginEdit
//===========================================================================
struct XTP_REPORTDATAMANAGER_COM_ERROR
{
	HRESULT hResult;        //The HRESULT error value.
	CString strMessage;     //The error message.
	CString strSource;      //The error source.

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPReportDataManager object.
	//-----------------------------------------------------------------------
	XTP_REPORTDATAMANAGER_COM_ERROR()
	{
		hResult = 0;
		strMessage.Empty();
		strSource.Empty();
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Overloaded assignment operator, copies the data values from an existing
	//     CXTPReportDataManager object.
	//-----------------------------------------------------------------------
	const XTP_REPORTDATAMANAGER_COM_ERROR& operator =(const XTP_REPORTDATAMANAGER_COM_ERROR& errorSrc)
	{
		this->hResult = errorSrc.hResult;
		this->strMessage = errorSrc.strMessage;
		this->strSource = errorSrc.strSource;

		return *this;
	}
};

//===========================================================================
// Summary:
// Example:
// <code>
// </code>
// See Also:
//===========================================================================
class CRstEvent : public XTPREPORTADODB::RecordsetEventsVt
{
private:
	ULONG m_cRef;
	CXTPReportDataManager* m_pDataManager;
public:
	CRstEvent(CXTPReportDataManager* pDataManager);
	~CRstEvent();

	STDMETHODIMP QueryInterface(REFIID riid, void ** ppv);
	STDMETHODIMP_(ULONG) AddRef(void);
	STDMETHODIMP_(ULONG) Release(void);

	virtual HRESULT __stdcall raw_WillChangeField(
		long cFields,
		VARIANT Fields,
		enum XTPREPORTADODB::EventStatusEnum* adStatus,
		struct XTPREPORTADODB::_Recordset* pRecordset);
	virtual HRESULT __stdcall raw_FieldChangeComplete(
		long cFields,
		VARIANT Fields,
		struct XTPREPORTADODB::Error* pError,
		enum XTPREPORTADODB::EventStatusEnum* adStatus,
		struct XTPREPORTADODB::_Recordset* pRecordset);
	virtual HRESULT __stdcall raw_WillChangeRecord(
		enum XTPREPORTADODB::EventReasonEnum adReason,
		long cRecords,
		enum XTPREPORTADODB::EventStatusEnum* adStatus,
		struct XTPREPORTADODB::_Recordset* pRecordset);
	virtual HRESULT __stdcall raw_RecordChangeComplete(
		enum XTPREPORTADODB::EventReasonEnum adReason,
		long cRecords,
		struct XTPREPORTADODB::Error* pError,
		enum XTPREPORTADODB::EventStatusEnum* adStatus,
		struct XTPREPORTADODB::_Recordset* pRecordset);
	virtual HRESULT __stdcall raw_WillChangeRecordset(
		enum XTPREPORTADODB::EventReasonEnum adReason,
		enum XTPREPORTADODB::EventStatusEnum* adStatus,
		struct XTPREPORTADODB::_Recordset* pRecordset);
	virtual HRESULT __stdcall raw_RecordsetChangeComplete(
		enum XTPREPORTADODB::EventReasonEnum adReason,
		struct XTPREPORTADODB::Error* pError,
		enum XTPREPORTADODB::EventStatusEnum* adStatus,
		struct XTPREPORTADODB::_Recordset* pRecordset);
	virtual HRESULT __stdcall raw_WillMove(
		enum XTPREPORTADODB::EventReasonEnum adReason,
		enum XTPREPORTADODB::EventStatusEnum* adStatus,
		struct XTPREPORTADODB::_Recordset* pRecordset);
	virtual HRESULT __stdcall raw_MoveComplete(
		enum XTPREPORTADODB::EventReasonEnum adReason,
		struct XTPREPORTADODB::Error* pError,
		enum XTPREPORTADODB::EventStatusEnum* adStatus,
		struct XTPREPORTADODB::_Recordset* pRecordset);
	virtual HRESULT __stdcall raw_EndOfRecordset(
		VARIANT_BOOL* fMoreData,
		enum XTPREPORTADODB::EventStatusEnum* adStatus,
		struct XTPREPORTADODB::_Recordset* pRecordset);
	virtual HRESULT __stdcall raw_FetchProgress(
		long Progress,
		long MaxProgress,
		enum XTPREPORTADODB::EventStatusEnum* adStatus,
		struct XTPREPORTADODB::_Recordset* pRecordset);
	virtual HRESULT __stdcall raw_FetchComplete(
		struct XTPREPORTADODB::Error* pError,
		enum XTPREPORTADODB::EventStatusEnum* adStatus,
		struct XTPREPORTADODB::_Recordset* pRecordset);

protected:
	CArray<XTPREPORTADODB::EventReasonEnum, XTPREPORTADODB::EventReasonEnum&> m_adReasonStack;
};

//===========================================================================
// Summary:
//     Represents report data manager.
// Remarks:
// See Also:
//===========================================================================
class _XTP_EXT_CLASS CXTPReportDataManager : public CXTPCmdTarget
{
	//{{AFX_CODEJOCK_PRIVATE
	friend class CXTPReportControl;
	friend class CRstEvent;
	void AFX_CDECL TRACE_ComError(_com_error &e);
	void AFX_CDECL TRACE_ProviderError(XTPREPORTADODB::_Connection* pConnDB);
	//}}AFX_CODEJOCK_PRIVATE

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPReportDataManager object.
	// Parameters:
	// pReportControl - pointer to CXTPReportControl object
	// Remarks:
	// Example:
	// <code>
	// </code>
	//-----------------------------------------------------------------------
	CXTPReportDataManager(CXTPReportControl* pReportControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPReportDataManager object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPReportDataManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the data source.
	// Parameter:
	//     pDataSource - A pointer to the custom ADODB object.
	// Remarks:
	//     It is a virtual function.
	//-----------------------------------------------------------------------
	virtual void SetDataSource(XTPREPORTADODB::_RecordsetPtr pDataSource);

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to get the data source.
	// Returns:
	//     A pointer to the custom ADODB object.
	// Remarks:
	//     It is a virtual function.
	//-----------------------------------------------------------------------
	virtual XTPREPORTADODB::_RecordsetPtr GetDataSource() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to bind(bring the data to control) the data.
	// Returns:
	//     A BOOL value of TRUE if the operation is successful FALSE else.
	// Remarks:
	//     It is a virtual function.
	//-----------------------------------------------------------------------
	virtual BOOL DataBind();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to create the columns, based on the fileds.
	// Returns:
	//     A HRESULT value specifying the result of the operation.S_OK if
	//     the operation is successful E_FAIL else.
	// Remarks:
	//     It is a virtual function.
	//-----------------------------------------------------------------------
	virtual HRESULT CreateColumns();

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to update the report records.
	// Parameter:
	//     pRecordset - A pointer to custom ADODB record set
	// Returns:
	//     A HRESULT value specifying the result of the operation.S_OK if
	//     the operation is successful, E_FAIL else.
	// Remarks:
	//     It is a virtual function.
	//-----------------------------------------------------------------------
	virtual HRESULT UpdateReportRecords(XTPREPORTADODB::_RecordsetPtr pRecordset);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to add record to the report control.
	// Parameter:
	//     pRecordset - A pointer to custom ADODB record set
	//     bAddNow    - Add now.
	// Returns:
	//     A HRESULT value specifying the result of the operation.S_OK if
	//     the operation is successful, E_FAIL else.
	// Remarks:
	//     It is a virtual function.
	//-----------------------------------------------------------------------
	virtual HRESULT AddReportRecords(XTPREPORTADODB::_RecordsetPtr pRecordset, BOOL bAddNow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this method to do preparation before deleting report records.
	// Parameter:
	//     pRecordset - A pointer to custom ADODB record set
	// Returns:
	//     A HRESULT value specifying the result of the operation.S_OK if
	//     the operation is successful, E_FAIL else.
	// Remarks:
	//     It is a virtual function.
	//-----------------------------------------------------------------------
	virtual HRESULT PrepareDeleteReportRecords(XTPREPORTADODB::_RecordsetPtr pRecordset);

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this method to clear the deleted records.
	// Remarks:
	//     It is a virtual function.
	//-----------------------------------------------------------------------
	virtual void ClearDeleteReportRecords();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to delete the report records.
	// Returns:
	//     A HRESULT value specifying the result of the operation.S_OK if
	//     the operation is successful, E_FAIL else.
	// Remarks:
	//     It is a virtual function.
	//-----------------------------------------------------------------------
	virtual HRESULT DeleteReportRecords();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to delete the report records.
	// Parameter:
	//     pRecordset - A pointer to custom ADODB record set
	// Returns:
	//     A HRESULT value specifying the result of the operation.S_OK if
	//     the operation is successful, E_FAIL else.
	// Remarks:
	//     It is a virtual function.
	//-----------------------------------------------------------------------
	virtual HRESULT DeleteReportRecords(XTPREPORTADODB::_RecordsetPtr pRecordset);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to update a field.
	// Parameter:
	//     pItem - A pointer to report record item variant.
	// Returns:
	//     A HRESULT value specifying the result of the operation.S_OK if
	//     the operation is successful, E_FAIL else.
	// Remarks:
	//     It is a virtual function.
	//-----------------------------------------------------------------------
	virtual HRESULT UpdateField(CXTPReportRecordItemVariant* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to update a record.
	// Parameter:
	//     pRecord - A pointer to report record object.
	// Returns:
	//     A HRESULT value specifying the result of the operation.S_OK if
	//     the operation is successful, E_FAIL else.
	// Remarks:
	//     It is a virtual function.
	//-----------------------------------------------------------------------
	virtual HRESULT UpdateRecord(CXTPReportRecord* pRecord, CXTPReportRecordItem* pItemChanged = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to add a record.
	// Parameter:
	//     pRecord      - A pointer to report record object.
	//     bAddToReport - TURE if the record to be added to the report control
	//                    as well, FALSE else.
	// Returns:
	//     A HRESULT value specifying the result of the operation.S_OK if
	//     the operation is successful, E_FAIL else.
	// Remarks:
	//     It is a virtual function.
	//-----------------------------------------------------------------------
	virtual HRESULT AddRecord(CXTPReportRecord* pRecord, BOOL bAddToReport);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to create an empty record.
	// Parameter:
	//     ppRecord     - A pointer to report record pointer.
	// Returns:
	//     A HRESULT value specifying the result of the operation.S_OK if
	//     the operation is successful, E_FAIL else.
	// Remarks:
	//     It is a virtual function.
	//-----------------------------------------------------------------------
	virtual HRESULT CreateEmptyRecord(CXTPReportRecord** ppRecord);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to delete a record.
	// Parameter:
	//     pRecord      - A pointer to report record object.
	// Returns:
	//     A HRESULT value specifying the result of the operation.S_OK if
	//     the operation is successful, E_FAIL else.
	// Remarks:
	//     It is a virtual function.
	//-----------------------------------------------------------------------
	virtual HRESULT DeleteRecord(CXTPReportRecord* pRecord);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the last COM error.
	// Returns:
	//     XTP_REPORTDATAMANAGER_COM_ERROR value specifying the COM error.
	// Remarks:
	//     It is a virtual function.
	//-----------------------------------------------------------------------
	virtual XTP_REPORTDATAMANAGER_COM_ERROR GetLastComError();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to remove the data source.
	//-----------------------------------------------------------------------
	void RemoveDataSource();
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to clear the report control.
	//-----------------------------------------------------------------------
	void ClearReportControl();

protected:
	CXTPReportControl* m_pReportControl;            //Pointer to the report control.
	XTPREPORTADODB::_ConnectionPtr m_pConnection;   //The custom ADODB connection point.
	XTPREPORTADODB::_RecordsetPtr m_pDataSource;    //The data source.
	CRstEvent* m_pRstEvent;                         //Pointer to the record set event object.
	DWORD m_dwRstEvent;                             //The record set event.
	CXTPReportRecords* m_pDeleteRecords;            //The delete records.
	volatile BOOL m_bLocalUpdate;                   //Tells whether the update is local only.
	CArray<XTP_REPORTDATAMANAGER_COM_ERROR*, XTP_REPORTDATAMANAGER_COM_ERROR*> m_LastComError;
public:

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPReportDataManager);

public:
	static CXTPReportDataManager* FromDispatch(LPDISPATCH pDisp)
	{
		if (pDisp == 0) return 0;
		return ((CXTPReportDataManager*)((BYTE*)pDisp - offsetof(CXTPReportDataManager, m_xDispatch)));
	}
	LPDISPATCH OleGetDataSource();
	void OleSetDataSource(LPDISPATCH lpDispatch);
	BOOL OleUpdateField(LPDISPATCH lpDispatch);
	BOOL OleUpdateRecord(LPDISPATCH lpDispatch);
	BOOL OleUpdateRecordItem(LPDISPATCH lpDispatch);
	BOOL OleAddRecord(LPDISPATCH lpDispatch, BOOL bAddToReport);
	BOOL OleDeleteRecord(LPDISPATCH lpDispatch);
	LPDISPATCH OleCreateEmptyRecord();

//}}AFX_CODEJOCK_PRIVATE
#endif

// CXTPReportDataManager diagnostics
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
	friend CDumpContext& AFXAPI operator<<(CDumpContext& dc,
				const CXTPReportDataManager& obj);
#endif

}; // class _XTP_EXT_CLASS CXTPReportDataManager


AFX_INLINE XTPREPORTADODB::_RecordsetPtr CXTPReportDataManager::GetDataSource() const{
	return m_pDataSource;
}

#ifdef _DEBUG
CDumpContext& AFXAPI operator<<(CDumpContext& dc, const CXTPReportDataManager& obj);
#endif

#endif //#if !defined(__XTPREPORTDATAMANAGER_H__)
