#pragma once

#ifndef _SISProjectSearchTypePjtType_H
#define _SISProjectSearchTypePjtType_H

#include <afxwin.h>  
#include <afxext.h>

#include "SISVariableSearchTypeText.h"

class AFX_EXT_CLASS SISProjectSearchTypePjtType : public SISVariableSearchTypeText
{

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISProjectSearchTypePjtType();

	/**
	@brief Creator
	@param c [in]Object of SISNProjectSearchTypePjtType
	@return
	@remark Initialize member variable using value of input object
	*/
	SISProjectSearchTypePjtType(SISProjectSearchTypePjtType& c);

	/**
	@brief Destructors
	@param
	@return
	@remark
	*/
	~SISProjectSearchTypePjtType();

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISProjectSearchTypePjtType& operator = (const SISProjectSearchTypePjtType &other);

private:
	void SetVariableName(CString strVarName);

};

#endif