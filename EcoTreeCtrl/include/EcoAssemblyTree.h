#pragma once

#include "EcoBaseTreeCtrl.h"
// CEcoAssemblyTree

class AFX_EXT_API CEcoAssemblyTree : public CEcoBaseTreeCtrl
{
	DECLARE_DYNAMIC(CEcoAssemblyTree)

public:
	CEcoAssemblyTree();
	virtual ~CEcoAssemblyTree();

protected:
	DECLARE_MESSAGE_MAP()

public:
	CMap<UINT, UINT, CAssemblyTreeTreeRecord*, CAssemblyTreeTreeRecord*> m_mapTree;
	CArray<CAssemblyLevelRecordSet, CAssemblyLevelRecordSet&> m_arrAssemLevelRS;
	CArray<CAssemblyTreeRecordSet, CAssemblyTreeRecordSet&> m_arrAssemTreeRS;
	CImageList m_ilIcons;
public:
	void CreateColumn();
 	int InsertRecord(CAssemblyTreeRecordSet& assemblyTreeRS, CAssemblyLevelRecordSet& assemblyLevelRS);

	int InsertRecord(CMap<UINT, UINT, CAssemblyTreeRecordSet, CAssemblyTreeRecordSet&>& assemblyTreeRSMap, 
		CMap<UINT, UINT, CAssemblyLevelRecordSet, CAssemblyLevelRecordSet&>& assemblyLevelRSMap);

	int LoadRecord(DATABASE_CLASS* dbManager, CString strProjGroup, CString strShipNo, BOOL bHasShipNo = TRUE, CString strWhere = _T(""));

	int GetTreeID();

	CArray<CAssemblyTreeRecordSet, CAssemblyTreeRecordSet&>* GetAssemTreeRS();
	CArray<CAssemblyLevelRecordSet, CAssemblyLevelRecordSet&>* GetAssemLevelRS();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


