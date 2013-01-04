// XTPReportDataManager.cpp : implementation of the CXTPReportDataManager class.
//
// This file is a part of the XTREME REPORTCONTROL MFC class library.
// (c)1998-2012 Codejock Software, All Rights Reserved.
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

#include "stdafx.h"

#include "Common/XTPCustomHeap.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSmartPtrInternalT.h"

#include "XTPReportDefines.h"
#include "XTPReportADO.h"
#include "XTPReportDataManager.h"
#include "XTPReportControl.h"
#include "XTPReportRecordItem.h"
#include "ItemTypes/XTPReportRecordItemVariant.h"
#include "XTPReportRecord.h"
#include "XTPReportRecords.h"
#include "XTPReportColumn.h"
#include "XTPReportColumns.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
// CRstEvent

CRstEvent::CRstEvent(CXTPReportDataManager* pDataManager)
{
	m_cRef = 0;
	m_pDataManager = pDataManager;
}

CRstEvent::~CRstEvent()
{
	ASSERT(m_adReasonStack.GetSize() == 0);
}

HRESULT __stdcall CRstEvent::QueryInterface(REFIID riid, void ** ppv)
{
	*ppv = NULL;
	if (riid == __uuidof(IUnknown) || riid == __uuidof(XTPREPORTADODB::RecordsetEventsVt))
		*ppv = this;
	if (*ppv == NULL)
		return ResultFromScode(E_NOINTERFACE);
	AddRef();
	return NOERROR;
}

ULONG __stdcall CRstEvent::AddRef()
{
	return ++m_cRef;
}

ULONG __stdcall CRstEvent::Release()
{
	if (--m_cRef != 0)
		return m_cRef;
	delete this;
	return 0;
}

HRESULT __stdcall CRstEvent::raw_WillChangeField(
	long ,//cFields,
	VARIANT ,//Fields,
	enum XTPREPORTADODB::EventStatusEnum* adStatus,
	struct XTPREPORTADODB::_Recordset* /*pRecordset*/)
{
	*adStatus = XTPREPORTADODB::adStatusUnwantedEvent;
	return S_OK;
}

HRESULT __stdcall CRstEvent::raw_FieldChangeComplete(
	long ,//cFields,
	VARIANT ,//Fields,
	struct XTPREPORTADODB::Error* ,//pError,
	enum XTPREPORTADODB::EventStatusEnum* adStatus,
	struct XTPREPORTADODB::_Recordset* //pRecordset
	)
{
	*adStatus = XTPREPORTADODB::adStatusUnwantedEvent;
	return S_OK;
}

HRESULT __stdcall CRstEvent::raw_WillChangeRecord(
	enum XTPREPORTADODB::EventReasonEnum adReason,
	long ,//cRecords,
	enum XTPREPORTADODB::EventStatusEnum* ,//adStatus,
	struct XTPREPORTADODB::_Recordset* pRecordset)
{
	if (m_pDataManager->m_bLocalUpdate)
		return S_OK;
	switch (adReason)
	{
		case XTPREPORTADODB::adRsnUpdate :
		case XTPREPORTADODB::adRsnResynch :
			break;
		case XTPREPORTADODB::adRsnDelete :
			m_adReasonStack.Add(adReason);
			m_pDataManager->PrepareDeleteReportRecords(pRecordset);
			break;
		default :
			m_adReasonStack.Add(adReason);
			break;
	}

	return S_OK;
}

HRESULT __stdcall CRstEvent::raw_RecordChangeComplete(
	enum XTPREPORTADODB::EventReasonEnum adReason,
	long ,//cRecords,
	struct XTPREPORTADODB::Error* /*pError*/,
	enum XTPREPORTADODB::EventStatusEnum* adStatus,
	struct XTPREPORTADODB::_Recordset* pRecordset)
{
	if (*adStatus == XTPREPORTADODB::adStatusErrorsOccurred)
	{
/*      if (pError != NULL)
		{
			BSTR bstrDesc;
			pError->get_Description(&bstrDesc);
			CString strErr(bstrDesc);
			::SysFreeString(bstrDesc);
			AfxMessageBox(strErr);
		}*/
		if (m_adReasonStack.GetSize() > 0)
			m_adReasonStack.RemoveAt(m_adReasonStack.GetSize() - 1);
		return S_OK;
	}
	if (m_pDataManager->m_bLocalUpdate)
		return S_OK;
	if (adReason == XTPREPORTADODB::adRsnUpdate)
	{
		int adPushedReason = 0;
		if (m_adReasonStack.GetSize() > 0)
		{
			adPushedReason = m_adReasonStack.GetAt(m_adReasonStack.GetSize() - 1);
			m_adReasonStack.RemoveAt(m_adReasonStack.GetSize() - 1);
		}
		switch (adPushedReason)
		{
//          case 0 :
			case XTPREPORTADODB::adRsnUpdate :
				m_pDataManager->UpdateReportRecords(pRecordset);
				break;
			case XTPREPORTADODB::adRsnAddNew :
				m_pDataManager->AddReportRecords(pRecordset, TRUE);
				break;
			case XTPREPORTADODB::adRsnDelete :
				m_pDataManager->DeleteReportRecords();
				break;
			case XTPREPORTADODB::adRsnUndoDelete :
//              m_pDataManager->AddReportRecords(pRecordset, TRUE);
				m_pDataManager->ClearDeleteReportRecords();
				break;
		}
	}
	else if (adReason == XTPREPORTADODB::adRsnResynch)
	{
		m_pDataManager->UpdateReportRecords(pRecordset);
	}
	else
	{
//      if (!m_adReasonStack.empty())
//          m_adReasonStack.pop();
	}

	return S_OK;
}

HRESULT __stdcall CRstEvent::raw_WillChangeRecordset(
	enum XTPREPORTADODB::EventReasonEnum ,//adReason,
	enum XTPREPORTADODB::EventStatusEnum* adStatus,
	struct XTPREPORTADODB::_Recordset* //pRecordset
	)
{
	*adStatus = XTPREPORTADODB::adStatusUnwantedEvent;
	return S_OK;
}

HRESULT __stdcall CRstEvent::raw_RecordsetChangeComplete(
	enum XTPREPORTADODB::EventReasonEnum adReason,
	struct XTPREPORTADODB::Error* /*pError*/,
	enum XTPREPORTADODB::EventStatusEnum* /*adStatus*/,
	struct XTPREPORTADODB::_Recordset* /*pRecordset*/)
{
	switch (adReason)
	{
		case XTPREPORTADODB::adRsnClose :
			m_pDataManager->ClearReportControl();
			break;
//      case XTPREPORTADODB::adRsnOpen :
//          m_pDataManager->AddReportRecords(pRecordset);
//          break;
	}

	return S_OK;
}

HRESULT __stdcall CRstEvent::raw_WillMove(
	enum XTPREPORTADODB::EventReasonEnum /*adReason*/,
	enum XTPREPORTADODB::EventStatusEnum* adStatus,
	struct XTPREPORTADODB::_Recordset* /*pRecordset*/)
{
	*adStatus = XTPREPORTADODB::adStatusUnwantedEvent;
	return S_OK;
}

HRESULT __stdcall CRstEvent::raw_MoveComplete(
	enum XTPREPORTADODB::EventReasonEnum /*adReason*/,
	struct XTPREPORTADODB::Error* /*pError*/,
	enum XTPREPORTADODB::EventStatusEnum* adStatus,
	struct XTPREPORTADODB::_Recordset* /*pRecordset*/)
{
	*adStatus = XTPREPORTADODB::adStatusUnwantedEvent;
	return S_OK;
}

HRESULT __stdcall CRstEvent::raw_EndOfRecordset(
	VARIANT_BOOL* /*fMoreData*/,
	enum XTPREPORTADODB::EventStatusEnum* adStatus,
	struct XTPREPORTADODB::_Recordset* /*pRecordset*/)
{
	*adStatus = XTPREPORTADODB::adStatusUnwantedEvent;
	return S_OK;
}

HRESULT __stdcall CRstEvent::raw_FetchProgress(
	long /*Progress*/,
	long /*MaxProgress*/,
	enum XTPREPORTADODB::EventStatusEnum* adStatus,
	struct XTPREPORTADODB::_Recordset* /*pRecordset*/)
{
	*adStatus = XTPREPORTADODB::adStatusUnwantedEvent;
	return S_OK;
}

HRESULT __stdcall CRstEvent::raw_FetchComplete(
	struct XTPREPORTADODB::Error* /*pError*/,
	enum XTPREPORTADODB::EventStatusEnum* adStatus,
	struct XTPREPORTADODB::_Recordset* /*pRecordset*/)
{
	*adStatus = XTPREPORTADODB::adStatusUnwantedEvent;
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
// CXTPReportDataManager

CXTPReportDataManager::CXTPReportDataManager(CXTPReportControl* pReportControl):
	m_pReportControl(pReportControl),
	m_pConnection(NULL),
	m_pDataSource(NULL),
	m_pRstEvent(NULL),
	m_dwRstEvent(0),
	m_pDeleteRecords(NULL),
	m_bLocalUpdate(FALSE)
{
}

CXTPReportDataManager::~CXTPReportDataManager()
{
	for (int i = 0; i < m_LastComError.GetSize(); i++)
	{
		XTP_REPORTDATAMANAGER_COM_ERROR* pError = m_LastComError.GetAt(i);
		if (pError)
			delete pError;
	}
	m_LastComError.RemoveAll();
	if (m_pDeleteRecords)
	{
		m_pDeleteRecords->RemoveAll();
		delete m_pDeleteRecords;
	}
	RemoveDataSource();
}

void AFX_CDECL CXTPReportDataManager::TRACE_ComError(_com_error &e)
{
	LPCWSTR bstrSource(e.Source());

	// Print COM errors.
	TRACE(_T("COM_ERROR: HRESULT = %08lx, Code meaning = %s, Source = %ls\n"),
		e.Error(), (LPCTSTR)e.ErrorMessage(), (LPCWSTR)bstrSource);
	// Save last COM error.
	XTP_REPORTDATAMANAGER_COM_ERROR* pError = NULL;
	if (m_LastComError.GetSize() >= XTP_REPORTDATAMANAGER_MAX_LAST_COM_ERRORS)
	{
		pError = m_LastComError.GetAt(m_LastComError.GetSize() - 1);
		m_LastComError.RemoveAt(m_LastComError.GetSize() - 1);
	}
	if (!pError)
		pError = new XTP_REPORTDATAMANAGER_COM_ERROR;
	if (!pError)
		return;
	pError->hResult = e.Error();
	pError->strMessage = e.ErrorMessage();
	pError->strSource.Format(_T("%ls"), (LPCWSTR)bstrSource);
	m_LastComError.Add(pError);
}

void AFX_CDECL CXTPReportDataManager::TRACE_ProviderError(XTPREPORTADODB::_Connection* pConnDB)
{
	// Print Provider Errors from Connection object.

	ASSERT(pConnDB);
	if (!pConnDB)
		return;

	if (pConnDB->Errors->Count > 0)
	{
		long nCount = pConnDB->Errors->Count;

		TRACE(_T("DB ERROR(s): "));

		for (long i = 0; i < nCount; i++)
		{
			XTPREPORTADODB::ErrorPtr ptrErr = pConnDB->Errors->GetItem(i);
			TRACE(_T(" (Error- %x, '%ls'); "), ptrErr->Number, (LPCWSTR)ptrErr->Description);
		}
		TRACE(_T(" \n"));
	}
}

void CXTPReportDataManager::RemoveDataSource()
{
	try
	{
		// unadvise events
		if (m_pRstEvent)
		{
			IConnectionPointContainer *pCPC = NULL;
			IConnectionPoint *pCP = NULL;
			HRESULT hr = m_pDataSource->QueryInterface(__uuidof(IConnectionPointContainer), (void**)&pCPC);
			if (SUCCEEDED(hr))
			{
				hr = pCPC->FindConnectionPoint(__uuidof(XTPREPORTADODB::RecordsetEvents), &pCP);
				pCPC->Release();
				if (SUCCEEDED(hr))
				{
					hr = pCP->Unadvise(m_dwRstEvent);
					pCP->Release();
				}
			}
			//delete m_pRstEvent;
			m_pRstEvent->Release();
			m_pRstEvent = NULL;
		}
		// set data source pointer to NULL
		m_pDataSource = NULL;

	}
	catch(_com_error &e)
	{
		TRACE_ProviderError(m_pConnection);
		TRACE_ComError(e);
	}
	catch(...)
	{
		TRACE(_T("Unknown error in RemoveDataSource()\n"));
	}
}

void CXTPReportDataManager::ClearReportControl()
{
	m_pReportControl->GetColumns()->Clear();
	m_pReportControl->ResetContent();
}

void CXTPReportDataManager::SetDataSource(XTPREPORTADODB::_RecordsetPtr pDataSource)
{
	// remove data source first
	if (m_pDataSource)
		RemoveDataSource();
	// set new data source
	if (pDataSource)
		m_pDataSource = pDataSource;
}

BOOL CXTPReportDataManager::DataBind()
{
	HRESULT hr = E_FAIL;
	try
	{
		if (m_pDataSource == NULL)
			return FALSE;

		m_pConnection = NULL;

		// get connection
		_variant_t vtConnection;
		if (SUCCEEDED(hr = m_pDataSource->get_ActiveConnection(&vtConnection)) && vtConnection.pdispVal)
		{
			m_pConnection = vtConnection;
		}
		// clear report control
		ClearReportControl();
		// create columns
		if (FAILED(hr = CreateColumns()))
			return FALSE;
		// create records
		if (FAILED(hr = AddReportRecords(m_pDataSource, FALSE)))
			return FALSE;
		m_pReportControl->Populate();
		// advise events
		m_pRstEvent = new CRstEvent(this);
		if (!m_pRstEvent)
			return FALSE;
		IUnknown *pUnk = NULL;
		hr = m_pRstEvent->QueryInterface(__uuidof(IUnknown), (void **) &pUnk);
		if (FAILED(hr))
			return FALSE;
		IConnectionPointContainer *pCPC = NULL;
		IConnectionPoint *pCP = NULL;
		hr = m_pDataSource->QueryInterface(__uuidof(IConnectionPointContainer), (void**)&pCPC);
		if (FAILED(hr))
			return FALSE;
		hr = pCPC->FindConnectionPoint(__uuidof(XTPREPORTADODB::RecordsetEvents), &pCP);
		pCPC->Release();
		if (FAILED(hr))
			return FALSE;
		hr = pCP->Advise(pUnk, &m_dwRstEvent);
		pCP->Release();
	}
	catch(_com_error &e)
	{
		TRACE_ProviderError(m_pConnection);
		TRACE_ComError(e);
		hr = FAILED(hr) ? hr : E_FAIL;
	}
	catch(...)
	{
		TRACE(_T("Unknown error in DataBind()\n"));
		hr = FAILED(hr) ? hr : E_FAIL;
	}

	return FAILED(hr) ? FALSE : TRUE;
}

HRESULT CXTPReportDataManager::CreateColumns()
{
	HRESULT hr = E_FAIL;

	try
	{
		XTPREPORTADODB::FieldsPtr pFields;
		if (FAILED(hr = m_pDataSource->get_Fields(&pFields)) || pFields == NULL)
			return hr;
		long lFields;
		if (FAILED(hr = pFields->get_Count(&lFields)) || lFields == 0)
			return hr;
		VARIANT vtIdx;
		vtIdx.vt = VT_I4;
		for (int i = 0; i < lFields; i++)
		{
			vtIdx.lVal = i;
			XTPREPORTADODB::FieldPtr pField;
			if (FAILED(hr = pFields->get_Item(vtIdx, &pField)) || pField == NULL)
				break;
/*          XTPREPORTADODB::DataTypeEnum dteType;
			if (FAILED(hr = pField->get_Type(&dteType)))
				break;
			if (dteType == XTPREPORTADODB::adLongVarBinary)
				continue;*/
			BSTR bstrName;
			pField->get_Name(&bstrName);
			CString strName(bstrName);
			::SysFreeString(bstrName);
			CXTPReportColumn* pColumn = new CXTPReportColumn(i, strName, 100);
			if (!pColumn)
				break;
			long lAttrs;
			if (SUCCEEDED(hr = pField->get_Attributes(&lAttrs)) && (lAttrs & XTPREPORTADODB::adFldUpdatable))
				pColumn->SetEditable(TRUE);
			m_pReportControl->AddColumn(pColumn);
		}
	}
	catch(_com_error &e)
	{
		TRACE_ProviderError(m_pConnection);
		TRACE_ComError(e);
		hr = FAILED(hr) ? hr : E_FAIL;
	}
	catch(...)
	{
		TRACE(_T("Unknown error in CreateColumns()\n"));
		hr = FAILED(hr) ? hr : E_FAIL;
	}

	return FAILED(hr) ? hr : S_OK;
}

HRESULT CXTPReportDataManager::UpdateReportRecords(XTPREPORTADODB::_RecordsetPtr pRecordset)
{
	HRESULT hr = E_FAIL;

	try
	{

	CXTPReportRecords* pRecords = m_pReportControl->GetRecords();
	if (!(pRecordset != NULL && pRecords))
		return E_FAIL;
	XTPREPORTADODB::FieldsPtr pFields;
	if (FAILED(hr = pRecordset->get_Fields(&pFields)) || pFields == NULL)
		return hr;
	long lFields;
	if (FAILED(hr = pFields->get_Count(&lFields)) || lFields == 0)
		return hr;
	long lRecords;
	if (FAILED(hr = pRecordset->get_RecordCount(&lRecords)))
		return hr;
	if (lRecords <= 0)
		return E_FAIL;
	pRecordset->MoveFirst();
	VARIANT_BOOL vtRecordsEof;
	pRecordset->get_bEOF(&vtRecordsEof);
	while (!vtRecordsEof)
	{
		// get record bookmark
		VARIANT vtBookmark;
		if (FAILED(hr = pRecordset->get_Bookmark(&vtBookmark)))
			break;
		CXTPReportRecord* pRecord = pRecords->FindRecordByBookmark(vtBookmark, TRUE);
		if (pRecord)
		{
			// get field values
			VARIANT vtIdx;
			vtIdx.vt = VT_I4;
			for (int i = 0; i < lFields; i++)
			{
				vtIdx.lVal = i;
				XTPREPORTADODB::FieldPtr pField;
				if (FAILED(hr = pFields->get_Item(vtIdx, &pField)) || pField == NULL)
					break;
				VARIANT vtValue;
				if (FAILED(hr = pField->get_Value(&vtValue)))
					break;
				CXTPReportRecordItemVariant* pItem = (CXTPReportRecordItemVariant*)pRecord->GetItem(i);
				if (!pItem)
					break;
				pItem->SetValue(COleVariant(vtValue));
			}
			m_pReportControl->UpdateRecord(pRecord, TRUE);
		}
		pRecordset->MoveNext();
		pRecordset->get_bEOF(&vtRecordsEof);
	}
	}
	catch(_com_error &e)
	{
		TRACE_ProviderError(m_pConnection);
		TRACE_ComError(e);
		hr = FAILED(hr) ? hr : E_FAIL;
	}
	catch(...)
	{
		TRACE(_T("Unknown error in UpdateReportRecords()\n"));
		hr = FAILED(hr) ? hr : E_FAIL;
	}

	return FAILED(hr) ? hr : S_OK;
}

HRESULT CXTPReportDataManager::AddReportRecords(XTPREPORTADODB::_RecordsetPtr pRecordset, BOOL bAddNow)
{
	HRESULT hr = E_FAIL;

	try
	{
		if (pRecordset == NULL)
			return E_FAIL;
		XTPREPORTADODB::FieldsPtr pFields;
		if (FAILED(hr = pRecordset->get_Fields(&pFields)) || pFields == NULL)
			return hr;
		long lFields;
		if (FAILED(hr = pFields->get_Count(&lFields)) || lFields == 0)
			return hr;

		long lRecords;
		if (FAILED(hr = pRecordset->get_RecordCount(&lRecords)))
			return hr;
		if (lRecords <= 0)
			return E_FAIL;
		pRecordset->MoveFirst();
		VARIANT_BOOL vtRecordsEof;
		pRecordset->get_bEOF(&vtRecordsEof);
		while (!vtRecordsEof)
		{

			// create record
			CXTPReportRecord* pRecord = new CXTPReportRecord;
			if (!pRecord)
				break;
			// get field values
			VARIANT vtIdx;
			vtIdx.vt = VT_I4;
			for (int i = 0; i < lFields; i++)
			{
				vtIdx.lVal = i;
				XTPREPORTADODB::FieldPtr pField;
				if (FAILED(hr = pFields->get_Item(vtIdx, &pField)) || pField == NULL)
					break;
				VARIANT vtValue;
				if (FAILED(hr = pField->get_Value(&vtValue)))
					break;
				CXTPReportRecordItemVariant* pItem = new CXTPReportRecordItemVariant(vtValue);
				if (!pItem)
					break;
				pRecord->AddItem(pItem);
			}
			// bind report record to data source record by bookmark
			VARIANT vtBookmark;
			if (FAILED(hr = pRecordset->get_Bookmark(&vtBookmark)))
				break;
			pRecord->SetBookmark(vtBookmark);
			// add record to report records collection
			if (bAddNow)
				m_pReportControl->AddRecordEx(pRecord);
			else
				m_pReportControl->AddRecord(pRecord);
			if (FAILED(hr))
				break;
			pRecordset->MoveNext();
			pRecordset->get_bEOF(&vtRecordsEof);
		}
	}
	catch(_com_error &e)
	{
		TRACE_ProviderError(m_pConnection);
		TRACE_ComError(e);
		hr = FAILED(hr) ? hr : E_FAIL;
	}
	catch(...)
	{
		TRACE(_T("Unknown error in AddReportRecords()\n"));
		hr = FAILED(hr) ? hr : E_FAIL;
	}

	return FAILED(hr) ? hr : S_OK;
}

HRESULT CXTPReportDataManager::PrepareDeleteReportRecords(XTPREPORTADODB::_RecordsetPtr pRecordset)
{
	HRESULT hr = E_FAIL;

	try
	{
		if (m_pDeleteRecords)
			m_pDeleteRecords->RemoveAll();
		else
			m_pDeleteRecords = new CXTPReportRecords(TRUE);
		if (!m_pDeleteRecords)
			return E_FAIL;
		CXTPReportRecords* pRecords = m_pReportControl->GetRecords();
		if (!(pRecordset != NULL && pRecords))
			return E_FAIL;
		long lRecords;
		if (FAILED(hr = pRecordset->get_RecordCount(&lRecords)))
			return hr;
		if (lRecords <= 0)
			return E_FAIL;
		pRecordset->MoveFirst();
		VARIANT_BOOL vtRecordsEof;
		pRecordset->get_bEOF(&vtRecordsEof);
		while (!vtRecordsEof)
		{
			// get record bookmark
			VARIANT vtBookmark;
			if (FAILED(hr = pRecordset->get_Bookmark(&vtBookmark)))
				break;
			CXTPReportRecord* pRecord = pRecords->FindRecordByBookmark(vtBookmark, TRUE);
			if (pRecord)
				m_pDeleteRecords->Add(pRecord);
			pRecordset->MoveNext();
			pRecordset->get_bEOF(&vtRecordsEof);
		}

	}
	catch(_com_error &e)
	{
		TRACE_ProviderError(m_pConnection);
		TRACE_ComError(e);
		hr = FAILED(hr) ? hr : E_FAIL;
	}
	catch(...)
	{
		TRACE(_T("Unknown error in PrepareDeleteReportRecords()\n"));
		hr = FAILED(hr) ? hr : E_FAIL;
	}

	return FAILED(hr) ? hr : S_OK;
}

void CXTPReportDataManager::ClearDeleteReportRecords()
{
	if (m_pDeleteRecords)
		m_pDeleteRecords->RemoveAll();
}

HRESULT CXTPReportDataManager::DeleteReportRecords()
{
	if (!m_pDeleteRecords)
		return E_FAIL;
	for (int i = 0; i < m_pDeleteRecords->GetCount(); i++)
	{
		m_pReportControl->RemoveRecordEx(m_pDeleteRecords->GetAt(i));
	}
	m_pDeleteRecords->RemoveAll();

	return S_OK;
}

HRESULT CXTPReportDataManager::DeleteReportRecords(XTPREPORTADODB::_RecordsetPtr pRecordset)
{
	HRESULT hr = E_FAIL;

	try
	{
		CXTPReportRecords* pRecords = m_pReportControl->GetRecords();
		if (!(pRecordset != NULL && pRecords))
			return E_FAIL;
		long lRecords;
		if (FAILED(hr = pRecordset->get_RecordCount(&lRecords)))
			return hr;
		if (lRecords <= 0)
			return E_FAIL;
		pRecordset->MoveFirst();
		VARIANT_BOOL vtRecordsEof;
		pRecordset->get_bEOF(&vtRecordsEof);
		while (!vtRecordsEof)
		{
			// get record bookmark
			VARIANT vtBookmark;
			if (FAILED(hr = pRecordset->get_Bookmark(&vtBookmark)))
				break;
			CXTPReportRecord* pRecord = pRecords->FindRecordByBookmark(vtBookmark, TRUE);
			if (pRecord)
				m_pReportControl->RemoveRecordEx(pRecord);
			pRecordset->MoveNext();
			pRecordset->get_bEOF(&vtRecordsEof);
		}

	}
	catch(_com_error &e)
	{
		TRACE_ProviderError(m_pConnection);
		TRACE_ComError(e);
		hr = FAILED(hr) ? hr : E_FAIL;
	}
	catch(...)
	{
		TRACE(_T("Unknown error in DeleteReportRecords()\n"));
		hr = FAILED(hr) ? hr : E_FAIL;
	}

	return FAILED(hr) ? hr : S_OK;
}

HRESULT CXTPReportDataManager::UpdateField(CXTPReportRecordItemVariant* pItem)
{
	BOOL bOk = FALSE;
	HRESULT hr = E_FAIL;

	m_bLocalUpdate = TRUE;

	try
	{
		while (TRUE)
		{
			if (!pItem)
				break;
			CXTPReportRecord* pRecord = pItem->GetRecord();
			if (!pRecord)
				break;
			// get data source record bookmark
			VARIANT vtBookmark = pRecord->GetBookmark();
			if (vtBookmark.vt == VT_EMPTY)
				break;
			// move to specified record
			if (FAILED(hr = m_pDataSource->put_Bookmark(vtBookmark)))
				break;
			// update item
			XTPREPORTADODB::FieldsPtr pFields;
			if (FAILED(hr = m_pDataSource->get_Fields(&pFields)) || pFields == NULL)
				break;
			long lFields;
			if (FAILED(hr = pFields->get_Count(&lFields)) || lFields == 0 || pItem->GetIndex() >= lFields)
				break;
			VARIANT vtIdx;
			vtIdx.vt = VT_I4;
			vtIdx.lVal = pItem->GetIndex();
			XTPREPORTADODB::FieldPtr pField;
			if (FAILED(hr = pFields->get_Item(vtIdx, &pField)) || pField == NULL)
				break;
			long lAttrs;
			if (FAILED(hr = pField->get_Attributes(&lAttrs)))
				break;
			if (!(lAttrs & (XTPREPORTADODB::adFldUpdatable | XTPREPORTADODB::adFldUnknownUpdatable)))
				break;
			XTPREPORTADODB::DataTypeEnum dteType;
			if (FAILED(hr = pField->get_Type(&dteType)))
				break;
			if (dteType == XTPREPORTADODB::adLongVarBinary)
				break;
			VARIANT vtValue = pItem->GetValue();
			if (FAILED(hr = pField->put_Value(vtValue)))
				break;
			if (FAILED(hr = m_pDataSource->Update()))
			{
				m_pDataSource->CancelUpdate();
				break;
			}
			bOk = TRUE;
			break;
		}

	}
	catch(_com_error &e)
	{
		TRACE_ProviderError(m_pConnection);
		TRACE_ComError(e);
		hr = FAILED(hr) ? hr : E_FAIL;
	}
	catch(...)
	{
		TRACE(_T("Unknown error in UpdateField()\n"));
		hr = FAILED(hr) ? hr : E_FAIL;
	}

	m_bLocalUpdate = FALSE;

	return FAILED(hr) ? hr : bOk ? S_OK : E_FAIL;
}

HRESULT CXTPReportDataManager::UpdateRecord(CXTPReportRecord* pRecord, CXTPReportRecordItem* pItemChanged /* = NULL */)
{
	BOOL bOk = FALSE;
	HRESULT hr = E_FAIL;

	m_bLocalUpdate = TRUE;

	try
	{
		while (TRUE)
		{
			if (!pRecord)
				break;
			// get data source record bookmark
			VARIANT vtBookmark = pRecord->GetBookmark();
			if (vtBookmark.vt == VT_EMPTY)
				break;
			// move to specified record
			if (FAILED(hr = m_pDataSource->put_Bookmark(vtBookmark)))
				break;
			// update record
			XTPREPORTADODB::FieldsPtr pFields;
			if (FAILED(hr = m_pDataSource->get_Fields(&pFields)) || pFields == NULL)
				break;
			long lFields;
			if (FAILED(hr = pFields->get_Count(&lFields)) || lFields == 0)
				break;
			VARIANT vtIdx;
			vtIdx.vt = VT_I4;
			int i;
			for (i = 0; i < lFields; i++)
			{
				vtIdx.lVal = i;
				XTPREPORTADODB::FieldPtr pField;
				if (FAILED(hr = pFields->get_Item(vtIdx, &pField)) || pField == NULL)
					break;
				long lAttrs;
				if (FAILED(hr = pField->get_Attributes(&lAttrs)))
					break;
				if (!(lAttrs & (XTPREPORTADODB::adFldUpdatable | XTPREPORTADODB::adFldUnknownUpdatable)))
					continue;
				XTPREPORTADODB::DataTypeEnum dteType;
				if (FAILED(hr = pField->get_Type(&dteType)))
					break;
				if (dteType == XTPREPORTADODB::adLongVarBinary)
					continue;
				CXTPReportRecordItemVariant* pItem = (CXTPReportRecordItemVariant*)pRecord->GetItem(i);
				if (!pItem)
					break;
				if (pItemChanged != NULL && pItemChanged != pItem)
					continue;

				VARIANT vtValue = pItem->GetValue();
				if (FAILED(hr = pField->put_Value(vtValue)))
					continue;//break;
			}
			if (FAILED(hr) || i < lFields)
			{
				m_pDataSource->CancelUpdate();
				break;
			}
			if (FAILED(hr = m_pDataSource->Update()))
			{
				m_pDataSource->CancelUpdate();
				return hr;
			}
			bOk = TRUE;
			break;
		}

	}
	catch(_com_error &e)
	{
		TRACE_ProviderError(m_pConnection);
		TRACE_ComError(e);
		m_pDataSource->CancelUpdate();
		hr = FAILED(hr) ? hr : E_FAIL;
	}
	catch(...)
	{
		TRACE(_T("Unknown error in UpdateRecord()\n"));
		hr = FAILED(hr) ? hr : E_FAIL;
	}

	m_bLocalUpdate = FALSE;

	return FAILED(hr) ? hr : bOk ? S_OK : E_FAIL;
}

HRESULT CXTPReportDataManager::AddRecord(CXTPReportRecord* pRecord, BOOL bAddToReport)
{
	BOOL bOk = FALSE;
	HRESULT hr = E_FAIL;

	m_bLocalUpdate = TRUE;

	try
	{
		while (TRUE)
		{
			if (!pRecord)
				break;
			// add record without field list
			if (FAILED(hr = m_pDataSource->raw_AddNew()))
				break;
			// update fields
			XTPREPORTADODB::FieldsPtr pFields;
			if (FAILED(hr = m_pDataSource->get_Fields(&pFields)) || pFields == NULL)
				break;
			long lFields;
			if (FAILED(hr = pFields->get_Count(&lFields)) || lFields == 0)
				break;
			VARIANT vtIdx;
			vtIdx.vt = VT_I4;
			int i;
			for (i = 0; i < lFields; i++)
			{
				vtIdx.lVal = i;
				XTPREPORTADODB::FieldPtr pField;
				if (FAILED(hr = pFields->get_Item(vtIdx, &pField)) || pField == NULL)
					break;
				long lAttrs;
				if (FAILED(hr = pField->get_Attributes(&lAttrs)))
					break;
				if (!(lAttrs & (XTPREPORTADODB::adFldUpdatable | XTPREPORTADODB::adFldUnknownUpdatable)))
					continue;
				CXTPReportRecordItemVariant* pItem = (CXTPReportRecordItemVariant*)pRecord->GetItem(i);
				if (!pItem)
					break;
				VARIANT vtValue = pItem->GetValue();
			  //if (FAILED(hr = pField->put_Value(vtValue))) // DEBUG comment
			  //    break;
				pField->put_Value(vtValue);
			}
			if (FAILED(hr) || i < lFields)
			{
				m_pDataSource->CancelUpdate();
				break;
			}
			// bind report record to data source record by bookmark
			VARIANT vtBookmark;
			VariantInit(&vtBookmark);
			if (FAILED(hr = m_pDataSource->get_Bookmark(&vtBookmark)))
				break;
			pRecord->SetBookmark(vtBookmark);
			// update recordset
			if (FAILED(hr = m_pDataSource->Update()))
			{
				m_pDataSource->CancelUpdate();
				pRecord->SetBookmark(COleVariant());
				break;
			}
			// add report record if required
			if (bAddToReport)
				m_pReportControl->AddRecordEx(pRecord);
			bOk = TRUE;
			break;
		}

	}
	catch(_com_error &e)
	{
		TRACE_ProviderError(m_pConnection);
		TRACE_ProviderError(m_pConnection);
		TRACE_ComError(e);
		m_pDataSource->CancelUpdate();
		pRecord->SetBookmark(COleVariant());
		hr = FAILED(hr) ? hr : E_FAIL;
	}
	catch(...)
	{
		TRACE(_T("Unknown error in AddRecord()\n"));
		hr = FAILED(hr) ? hr : E_FAIL;
	}

	m_bLocalUpdate = FALSE;

	return FAILED(hr) ? hr : bOk ? S_OK : E_FAIL;
}

HRESULT CXTPReportDataManager::CreateEmptyRecord(CXTPReportRecord** ppRecord)
{
	BOOL bOk = FALSE;
	HRESULT hr = E_FAIL;
	CXTPReportRecord* pRecord = NULL;

	try
	{
		while (TRUE)
		{
			if (m_pDataSource == NULL || !ppRecord)
				break;
			// create empty record
			pRecord = new CXTPReportRecord;
			if (!pRecord)
				break;
			XTPREPORTADODB::FieldsPtr pFields;
			if (FAILED(hr = m_pDataSource->get_Fields(&pFields)) || pFields == NULL)
				break;
			long lFields;
			if (FAILED(hr = pFields->get_Count(&lFields)) || lFields == 0)
				break;
			long lRecords;
			if (FAILED(hr = m_pDataSource->get_RecordCount(&lRecords)))
				break;
			if (lRecords > 0)
			{
				m_pDataSource->MoveFirst();
				VARIANT_BOOL vtRecordsEof;
				if (FAILED(hr = m_pDataSource->get_bEOF(&vtRecordsEof)))
					break;
				if (vtRecordsEof)
					break;
			}
			VARIANT vtIdx;
			vtIdx.vt = VT_I4;
			int i;
			for (i = 0; i < lFields; i++)
			{
				vtIdx.lVal = i;
				XTPREPORTADODB::FieldPtr pField;
				if (FAILED(hr = pFields->get_Item(vtIdx, &pField)) || pField == NULL)
					break;

				VARIANT vtValue;
				if (lRecords == 0)
				{
					XTPREPORTADODB::DataTypeEnum dt;
					if (FAILED(hr = pField->get_Type(&dt)))
						break;

					if (dt == XTPREPORTADODB::adInteger || dt == XTPREPORTADODB::adTinyInt || dt == XTPREPORTADODB::adSmallInt || dt == XTPREPORTADODB::adBigInt ||
						dt == XTPREPORTADODB::adUnsignedTinyInt || dt == XTPREPORTADODB::adUnsignedSmallInt || dt == XTPREPORTADODB::adUnsignedInt || dt == XTPREPORTADODB::adUnsignedBigInt)
						vtValue.vt = VT_I4;
					else if (dt == XTPREPORTADODB::adBoolean)
						vtValue.vt = VT_BOOL;
					else if (dt == XTPREPORTADODB::adDate)
						vtValue.vt = VT_DATE;
					else if (dt == XTPREPORTADODB::adDouble)
						vtValue.vt = VT_R8;
					else if (dt == XTPREPORTADODB::adSingle)
						vtValue.vt = VT_R4;
					else if (dt == XTPREPORTADODB::adCurrency)
						vtValue.vt = VT_CY;
					else
						vtValue.vt = VT_BSTR;
				}
				else
				{
					if (FAILED(hr = pField->get_Value(&vtValue)))
						break;
				}

				vtValue.lVal = 0;
				CXTPReportRecordItemVariant* pItem = new CXTPReportRecordItemVariant(vtValue);
				if (!pItem)
					break;
				pRecord->AddItem(pItem);
			}
			if (FAILED(hr) || i < lFields)
				break;
			*ppRecord = pRecord;
			bOk = TRUE;
			break;
		}

	}
	catch(_com_error &e)
	{
		if (pRecord)
			delete pRecord;
		TRACE_ProviderError(m_pConnection);
		TRACE_ComError(e);
		hr = FAILED(hr) ? hr : E_FAIL;
	}
	catch(...)
	{
		if (pRecord)
			delete pRecord;
		TRACE(_T("Unknown error in CreateEmptyRecord()\n"));
		hr = FAILED(hr) ? hr : E_FAIL;
	}

	if (!bOk && pRecord)
		delete pRecord;

	return FAILED(hr) ? hr : bOk ? S_OK : E_FAIL;
}

HRESULT CXTPReportDataManager::DeleteRecord(CXTPReportRecord* pRecord)
{
	HRESULT hr = E_FAIL;

	try
	{
		if (!pRecord)
			return E_FAIL;
		// get data source record bookmark
		VARIANT vtBookmark = pRecord->GetBookmark();
		if (vtBookmark.vt == VT_EMPTY)
			return E_FAIL;
		// move to specified record
		if (FAILED(hr = m_pDataSource->put_Bookmark(vtBookmark)))
			return hr;
		// delete record
		if (FAILED(hr = m_pDataSource->Delete(XTPREPORTADODB::adAffectCurrent)))
		{
			m_pDataSource->CancelUpdate();
			return hr;
		}

	}
	catch(_com_error &e)
	{
		TRACE_ProviderError(m_pConnection);
		TRACE_ComError(e);
		m_pDataSource->CancelUpdate();
		hr = FAILED(hr) ? hr : E_FAIL;
	}
	catch(...)
	{
		TRACE(_T("Unknown error in CreateEmptyRecord()\n"));
		hr = FAILED(hr) ? hr : E_FAIL;
	}

	return FAILED(hr) ? hr : S_OK;
}

XTP_REPORTDATAMANAGER_COM_ERROR CXTPReportDataManager::GetLastComError()
{
	XTP_REPORTDATAMANAGER_COM_ERROR Error;

	if (m_LastComError.GetSize() > 0)
	{
		XTP_REPORTDATAMANAGER_COM_ERROR* pError = m_LastComError.GetAt(m_LastComError.GetSize() - 1);
		m_LastComError.RemoveAt(m_LastComError.GetSize() - 1);
		if (pError)
		{
			Error = *pError;
			delete pError;
		}
	}

	return Error;
}


/////////////////////////////////////////////////////////////////////////////
// CXTPReportDataManager diagnostics

#ifdef _DEBUG
void CXTPReportDataManager::AssertValid() const
{
	CXTPCmdTarget::AssertValid();

	ASSERT(m_pReportControl);
}

void CXTPReportDataManager::Dump(CDumpContext& dc) const
{
	// The overridden Dump usually calls the Dump
	// function of its base class before printing
	// data members unique to the derived class.

	CXTPCmdTarget::Dump(dc);


	if (m_pConnection)
	{
		try
		{
			dc << _T("Connection String: ") << (LPCTSTR)m_pConnection->GetConnectionString() << _T("\n");
			dc << _T("Connection State: ");
			switch (m_pConnection->GetState())
			{
				case XTPREPORTADODB::adStateClosed:
					dc << _T("Closed");
					break;
				case XTPREPORTADODB::adStateOpen:
					dc << _T("Open");
					break;
				case XTPREPORTADODB::adStateConnecting:
					dc << _T("Connecting");
					break;
				case XTPREPORTADODB::adStateExecuting:
					dc << _T("Executing");
					break;
				case XTPREPORTADODB::adStateFetching:
					dc << _T("Fetching");
					break;
				default:
					dc << _T("Unknown");
			}
		}
		catch (_com_error &e)
		{
			UNREFERENCED_PARAMETER(e);
		}
		dc << _T("\n");
	}
	if (m_pDataSource)
	{
		try
		{
			dc << _T("Recordset State: ");
			switch (m_pDataSource->GetState())
			{
				case XTPREPORTADODB::adStateClosed:
					dc << _T("Closed");
					break;
				case XTPREPORTADODB::adStateOpen:
					dc << _T("Open");
					break;
				case XTPREPORTADODB::adStateConnecting:
					dc << _T("Connecting");
					break;
				case XTPREPORTADODB::adStateExecuting:
					dc << _T("Executing");
					break;
				case XTPREPORTADODB::adStateFetching:
					dc << _T("Fetching");
					break;
				default:
					dc << _T("Unknown");
			}
			dc << _T("\n");
			dc << _T("Record Count: ") << (LONG)m_pDataSource->GetRecordCount();
		}
		catch (_com_error &e)
		{
			UNREFERENCED_PARAMETER(e);
		}
		dc << _T("\n");
	}
}

CDumpContext& AFXAPI operator<<(CDumpContext& dc, const CXTPReportDataManager& obj)
{
	obj.Dump(dc);

	return dc;
}
#endif //_DEBUG
