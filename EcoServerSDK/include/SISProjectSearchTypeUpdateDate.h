#pragma once
#include <afxwin.h>  
#include <afxext.h>

#include "SISVariableSearchTypeDate.h"

class AFX_EXT_CLASS SISProjectSearchTypeUpdateDate : public SISVariableSearchTypeDate
{

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISProjectSearchTypeUpdateDate();

	/**
	@brief Destructors
	@param
	@return
	@remark
	*/
	~SISProjectSearchTypeUpdateDate();

	/**
	@brief Creator
	@param c [in]Object of SISNProjectSearchTypeUpdateDate
	@return
	@remark Initialize member variable using value of input object
	*/
	SISProjectSearchTypeUpdateDate(SISProjectSearchTypeUpdateDate& c);

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISProjectSearchTypeUpdateDate& operator = (const SISProjectSearchTypeUpdateDate &other);

private:
	void SetVariableName(CString strVarName);

};
