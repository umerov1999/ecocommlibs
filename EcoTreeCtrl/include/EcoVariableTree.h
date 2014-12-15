#pragma once

#include "EcoBaseTreeCtrl.h"
// CEcoVariableTree
// #define PROPERTY_SHIP_NO 1
// #define PROPERTY_BLOCK 2
// #define PROPERTY_PROJECT 3
// 
// #define TYPE_NUMBER 101
// #define TYPE_TEXT 102
// #define TYPE_ENUM 103
// #define TYPE_DATE 104
// #define TYPE_USER 105
#include <iostream>
#include <map>
using namespace std;
typedef map<UINT, CUserVariableRecordSet*> MapSort;
class AFX_EXT_API CEcoVariableTree : public CEcoBaseTreeCtrl
{
	DECLARE_DYNAMIC(CEcoVariableTree)

public:
	CEcoVariableTree();
	virtual ~CEcoVariableTree();

protected:
	DECLARE_MESSAGE_MAP()

public:
	CArray<CUserVariableRecordSet, CUserVariableRecordSet&> m_arrUserVarRS;
public:
	void CreateColumn();
	int LoadRecord(DATABASE_CLASS* dbManager, CString strProjGroup, CString strShipNo, CString strWhere);
	int InsertRecord(DATABASE_CLASS* dbManager, CUserVariableRecordSet& userVarRS);
	
	int DeleteRecord(int nTreeType, UINT nID, CXTPReportRow* pReportRow);
	int GetTreeID();
	CArray<CUserVariableRecordSet, CUserVariableRecordSet&>* GetUserVarRS();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


