#pragma once

#include "EcoBaseTreeCtrl.h"
// CEcoVariableOfGroupTree
#include <iostream>
#include <map>
using namespace std;
typedef map<UINT, CUserVariableRecordSet*> MapSort;
typedef map<UINT, UINT> MapUINTSort;
class AFX_EXT_API CEcoVariableOfGroupTree : public CEcoBaseTreeCtrl
{
	DECLARE_DYNAMIC(CEcoVariableOfGroupTree)

public:
	CEcoVariableOfGroupTree();
	virtual ~CEcoVariableOfGroupTree();

protected:
	DECLARE_MESSAGE_MAP()

public:
	CArray<CVariableGroupRecordSet, CVariableGroupRecordSet&> m_arrVarGroupRS;
	CArray<CAssemblyLevelRecordSet, CAssemblyLevelRecordSet&> m_arrAssemLevelRS;
	CArray<CUserVariableRecordSet, CUserVariableRecordSet&> m_arrUserVarRS;
	CArray<CVariableOfGroupRecordSet, CVariableOfGroupRecordSet&> m_arrVarOfGroupRS;
	
public:
	void CreateColumn();
	int LoadRecord(DATABASE_CLASS* dbManager, CString strProjGroup, CString strShipNo, CString strWhere);
	int GetTreeID();
	void CollectChildVarGroupRows(CXTPReportRow* pPrarentRow, CArray<CXTPReportRow*, CXTPReportRow*>& pArrChildVarGroupRows, int nSetp = 0);
	CArray<CVariableGroupRecordSet, CVariableGroupRecordSet&>* GetVarGroupRS();
	CArray<CAssemblyLevelRecordSet, CAssemblyLevelRecordSet&>* GetAssemLevelRS();
	CArray<CVariableOfGroupRecordSet, CVariableOfGroupRecordSet&>* GetVarOfGroupRS();
	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


