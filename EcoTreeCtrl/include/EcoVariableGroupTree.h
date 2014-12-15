#pragma once

#include "EcoBaseTreeCtrl.h"
// CEcoVariableGroupTree
#include <iostream>
#include <map>
using namespace std;
typedef map<UINT, CUserVariableRecordSet*> MapSort;
typedef map<UINT, UINT> MapUINTSort;
class AFX_EXT_API CEcoVariableGroupTree : public CEcoBaseTreeCtrl
{
	DECLARE_DYNAMIC(CEcoVariableGroupTree)

public:
	CEcoVariableGroupTree();
	virtual ~CEcoVariableGroupTree();

protected:
	DECLARE_MESSAGE_MAP()

public:
	CArray<CVariableGroupRecordSet, CVariableGroupRecordSet&> m_arrVarGroupRS;
	CArray<CAssemblyLevelRecordSet, CAssemblyLevelRecordSet&> m_arrAssemLevelRS;
	CArray<CUserVariableRecordSet, CUserVariableRecordSet&> m_arrUserVarRS;

public:
	void CreateColumn();
	int LoadRecord(DATABASE_CLASS* dbManager, CString strProjGroup, CString strShipNo, CString strWhere);
	int GetTreeID();
	void CollectChildVarGroupRows(CXTPReportRow* pPrarentRow, CArray<CXTPReportRow*, CXTPReportRow*>& pArrChildVarGroupRows, int nSetp = 0);
	CArray<CVariableGroupRecordSet, CVariableGroupRecordSet&>* GetVarGroupRS();
	CArray<CAssemblyLevelRecordSet, CAssemblyLevelRecordSet&>* GetAssemLevelRS();
	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


