#pragma once

#ifndef _SISProjectSearchTypeUserID_H
#define _SISProjectSearchTypeUserID_H

#include <afxwin.h>  
#include <afxext.h>

#include "SISVariableSearchTypeText.h"

class AFX_EXT_CLASS SISProjectSearchTypeUserID : public SISVariableSearchTypeText
{

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISProjectSearchTypeUserID();

	/**
	@brief Destructors
	@param
	@return
	@remark
	*/
	~SISProjectSearchTypeUserID();

	/**
	@brief Creator
	@param c [in]Object of SISNProjectSearchTypeUserID
	@return
	@remark Initialize member variable using value of input object
	*/
	SISProjectSearchTypeUserID(SISProjectSearchTypeUserID& c);

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISProjectSearchTypeUserID& operator = (const SISProjectSearchTypeUserID &other);

private:
	void SetVariableName(CString strVarName);

};

#endif