#pragma once

#include "EcoBaseTreeCtrl.h"
// CEcoAdvancedSearchTree

class AFX_EXT_API CEcoAdvancedSearchTree : public CEcoBaseTreeCtrl
{
	DECLARE_DYNAMIC(CEcoAdvancedSearchTree)

public:
	CEcoAdvancedSearchTree();
	virtual ~CEcoAdvancedSearchTree();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


