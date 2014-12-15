#pragma once

#include "EcoBaseTreeCtrl.h"
// CEcoMeasureDataTree

class AFX_EXT_API CEcoMeasureDataTree : public CEcoBaseTreeCtrl
{
	DECLARE_DYNAMIC(CEcoMeasureDataTree)

public:
	CEcoMeasureDataTree();
	virtual ~CEcoMeasureDataTree();

protected:
	DECLARE_MESSAGE_MAP()

public:
	void CreateColumn();
	int LoadRecord(DATABASE_CLASS* dbManager, CString strProjGroup, CString strShipNo, CString strWhere);
	int GetTreeID();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


