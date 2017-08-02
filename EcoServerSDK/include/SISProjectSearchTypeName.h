#pragma once

#ifndef _SISProjectSearchTypeName_H
#define _SISProjectSearchTypeName_H

#include <afxwin.h>  
#include <afxext.h>

#include "SISVariableSearchTypeText.h"

class AFX_EXT_CLASS SISProjectSearchTypeName : public SISVariableSearchTypeText
{

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISProjectSearchTypeName();

	/**
	@brief Creator
	@param c [in]Object of SISNProjectSearchTypeName
	@return
	@remark Initialize member variable using value of input object
	*/
	SISProjectSearchTypeName(const SISProjectSearchTypeName& c);

	/**
	@brief Destructors
	@param
	@return
	@remark
	*/
	~SISProjectSearchTypeName();

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISProjectSearchTypeName& operator = (const SISProjectSearchTypeName &other);

private:
	void SetVariableName(CString strVarName);

};

#endif