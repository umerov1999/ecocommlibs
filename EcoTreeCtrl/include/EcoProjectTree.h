#pragma once

#include "EcoBaseTreeCtrl.h"
#include <iostream>
#include <map>
#include <set>
using namespace std;
typedef set<UINT> SetUINT;
typedef map<UINT, UINT> mapUINT;
// CEcoProjectTree

typedef struct _PERIOD_DATE
{
	COleDateTime oleUpdateStart;
	COleDateTime oleUpdateEnd;
}PERIOD_DATE;


class AFX_EXT_API CEcoProjectTree : public CEcoBaseTreeCtrl
{
	DECLARE_DYNAMIC(CEcoProjectTree)

public:
	CEcoProjectTree();
	virtual ~CEcoProjectTree();

protected:
	DECLARE_MESSAGE_MAP()

public:
	CArray<CString, CString&> m_arrColumns;
	BOOL bUserDefindColumn;

public:
	void CreateColumn();
	void CreateColumn(CArray<CString, CString&>& arrColumns);
// 	int LoadRecord(CDataBaseManager* dbManager, CString strProjGroup, CString strShipNo, BOOL bBlockPjtLoad = TRUE, CString strBlockPjtWhere = _T(""), 
// 					BOOL bOTSPjtLoad = FALSE, CString strOTSPjtWhere = _T(""));
	int LoadRecord(DATABASE_CLASS* dbManager, CString strProjGroup, CString strShipNo, CString strPjtWhere, 
		CArray<CVariableGroupRecordSet, CVariableGroupRecordSet&>& arrLimitVarGroupRS, 
		CArray<CAssemblyTreeRecordSet, CAssemblyTreeRecordSet&>& arrLimitAssemTreeRS, 
		PERIOD_DATE& limitDate, SetUINT* pSetResultPjtIdOfProp = NULL);
	int GetTreeID();

	void CollectParentProject(UINT nParentPjtID, CArray<PROJECT_WITH_PROPERTY, PROJECT_WITH_PROPERTY&>& arrSourcePjtRS, 
													CArray<PROJECT_WITH_PROPERTY, PROJECT_WITH_PROPERTY&>& arrCollectPjtRS);

	void CollectParentAssemTree(UINT nParentTreeID, CArray<CAssemblyTreeRecordSet, CAssemblyTreeRecordSet&>& arrSourceTreeRS, 
													CArray<CAssemblyTreeRecordSet, CAssemblyTreeRecordSet&>& arrCollectTreeRS);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


