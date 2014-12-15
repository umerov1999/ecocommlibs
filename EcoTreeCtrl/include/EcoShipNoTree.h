#pragma once

#include "EcoBaseTreeCtrl.h"
// CEcoShipNoTree

class AFX_EXT_API CEcoShipNoTree : public CEcoBaseTreeCtrl
{
	DECLARE_DYNAMIC(CEcoShipNoTree)
public:
	CEcoShipNoTree();
	virtual ~CEcoShipNoTree();

protected:
	DECLARE_MESSAGE_MAP()

public:
	CMap<UINT, UINT, CAssemblyTreeTreeRecord*, CAssemblyTreeTreeRecord*> m_mapTree;

public:
	void CreateColumn();
 	int InsertRecord(CAssemblyTreeRecordSet& assemblyTreeRS, CAssemblyLevelRecordSet& assemblyLevelRS);
	int InsertRecord(CMap<UINT, UINT, CAssemblyTreeRecordSet, CAssemblyTreeRecordSet&>& assemblyTreeRSMap, 
						CMap<UINT, UINT, CAssemblyLevelRecordSet, CAssemblyLevelRecordSet&>& assemblyLevelRSMap);
	int LoadRecord(DATABASE_CLASS* dbManager, CString strWhere);

	int GetTreeID();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


