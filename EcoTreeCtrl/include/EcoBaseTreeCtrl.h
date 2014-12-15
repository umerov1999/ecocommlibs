#pragma once

#include "stdafx.h"
#include <map>
// #include <iostream>     // tolower()
// #include <string>
// #include <functional>   // binary_function, ptr_fun
// #include <algorithm>    // mismatch, lexicographical_compare, find
//extern "C" AFX_EXT_API void WINAPI InitYourExtDLL();

using namespace std;

// CEcoBaseTreeCtrl
#define ID_ASSEMBLY_TREE 1
#define ID_PROJECT_TREE 2
#define ID_VARIABLE_GROUP_TREE 3
#define ID_USER_VARIABLE_TREE 4
#define ID_MEASURE_DATA_TREE 5
#define ID_ADVANCED_SEARCH_TREE 6
#define ID_SHIPNO_TREE 7
#define ID_ASSEMBLY_LEVEL_TREE 8
#define ID_VARIABLE_OF_GROUP_TREE 9

typedef struct _struTreeData
{
	CXTPReportRecord* pReportRecord;
	CEcoTreeBaseRecordSet* pEcoTreeBaseRC;
	UINT parentTreeID;
} TreeData;

typedef CMap<UINT, UINT, TreeData, TreeData> MapTreeData;

typedef multimap<UINT, TreeData> MMapTreeDataDepthSort;
typedef multimap<CString, TreeData> MMapTreeDataNameSort;
typedef multimap<int, TreeData> MMapTreeDataOrderSort;

class AFX_EXT_API CEcoBaseTreeCtrl : public CXTPReportControl
{
	DECLARE_DYNAMIC(CEcoBaseTreeCtrl)

public:
	CEcoBaseTreeCtrl();
	virtual ~CEcoBaseTreeCtrl();

protected:
	DECLARE_MESSAGE_MAP()
	
public:
	CList<UINT, UINT> m_listTreeType;
	CMap<UINT, UINT, MapTreeData*, MapTreeData*> m_mapBaseTree;
	
	CMap<PUINT_PTR, PUINT_PTR, CString, LPCTSTR> m_mapSavedRecord;

public:
//	CXTPOfficeBorder<CXTPReportControl,false> m_wndReport;
	void Init();
	void InsertTreeData(UINT nTreeType, CMap<UINT, UINT, CEcoTreeBaseRecordSet*, CEcoTreeBaseRecordSet*>& mapEcoTreeBaseRC, CMap<UINT, UINT, CXTPReportRecord*, CXTPReportRecord*>& mapReportRC, 
						CMap<UINT, UINT, UINT, UINT>& mapParentTreeID);
 	void InsertHeadTreeData(UINT nTreeType, CMap<UINT, UINT, CEcoTreeBaseRecordSet*, CEcoTreeBaseRecordSet*>& mapEcoTreeBaseRC, CMap<UINT, UINT, CXTPReportRecord*, CXTPReportRecord*>& mapReportRC, 
						CMap<UINT, UINT, UINT, UINT>& mapParentTreeID);
 	void InsertTailTreeData(UINT nTreeType, CMap<UINT, UINT, CEcoTreeBaseRecordSet*, CEcoTreeBaseRecordSet*>& mapEcoTreeBaseRC, CMap<UINT, UINT, CXTPReportRecord*, CXTPReportRecord*>& mapReportRC, 
							CMap<UINT, UINT, UINT, UINT>& mapParentTreeID);
	BOOL MakeTreeDataStru(CMap<UINT, UINT, CEcoTreeBaseRecordSet*, CEcoTreeBaseRecordSet*>& mapEcoTreeBaseRC, CMap<UINT, UINT, CXTPReportRecord*, CXTPReportRecord*>& mapReportRC, 
							CMap<UINT, UINT, UINT, UINT>& mapParentTreeID, MapTreeData* pMapTreeData);

	void DeleteTreeData(UINT nTreeType, UINT nID);

	void RefreshTree();
	CXTPReportRecord* BuildTree(POSITION listPos, UINT id, MapTreeData* pMapTreeData);
	BOOL IsExistTreeRecord(CXTPReportRecord* pReportRecord);
	void SortTree();
	void Sort(CXTPReportRows* pRows);



	void InsertTreeData(UINT nTreeType, CArray<CEcoTreeBaseRecordSet*, CEcoTreeBaseRecordSet*>& arrEcoTreeBaseRC, CMap<UINT, UINT, CXTPReportRecord*, CXTPReportRecord*>& mapReportRC, 
										CMap<UINT, UINT, UINT, UINT>& mapParentTreeID);
	void InsertHeadTreeData(UINT nTreeType, CArray<CEcoTreeBaseRecordSet*, CEcoTreeBaseRecordSet*>& arrEcoTreeBaseRC, CMap<UINT, UINT, CXTPReportRecord*, CXTPReportRecord*>& mapReportRC, 
										  CMap<UINT, UINT, UINT, UINT>& mapParentTreeID);
	void InsertTailTreeData(UINT nTreeType, CArray<CEcoTreeBaseRecordSet*, CEcoTreeBaseRecordSet*>& arrEcoTreeBaseRC, CMap<UINT, UINT, CXTPReportRecord*, CXTPReportRecord*>& mapReportRC, 
											CMap<UINT, UINT, UINT, UINT>& mapParentTreeID);
	BOOL MakeTreeDataStru(CArray<CEcoTreeBaseRecordSet*, CEcoTreeBaseRecordSet*>& arrEcoTreeBaseRC, CMap<UINT, UINT, CXTPReportRecord*, CXTPReportRecord*>& mapReportRC, 
		CMap<UINT, UINT, UINT, UINT>& mapParentTreeID, CArray<TreeData, TreeData>* pArrTreeData);


	CMap<UINT, UINT, CArray<TreeData, TreeData>*, CArray<TreeData, TreeData>*> m_mapBaseTree2;
	void RefreshTree2();
	CXTPReportRecord* BuildTree2(POSITION listPos, UINT id, CArray<TreeData, TreeData>* pArrTreeData);

	void RefreshTree3();
	CXTPReportRecord* BuildTree3(POSITION listPos, UINT id, CArray<TreeData, TreeData>* pArrTreeData);

	void RefreshVarOfGroupTree();
};


