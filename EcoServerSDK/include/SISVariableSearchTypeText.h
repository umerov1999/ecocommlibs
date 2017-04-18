#pragma once
#include <afxwin.h>  
#include <afxext.h>

#include "SISVariableSearchObject.h"

class AFX_EXT_CLASS SISVariableSearchTypeText : public SISVariableSearchObject
{

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISVariableSearchTypeText();

	/**
	@brief Creator
	@param c [in]Object of SISNVariableSearchTypeText
	@return
	@remark Initialize member variable using value of input object
	*/
	SISVariableSearchTypeText(SISVariableSearchTypeText& c);

	/**
	@brief Destructors
	@param
	@return
	@remark
	*/
	~SISVariableSearchTypeText();

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISVariableSearchTypeText& operator = (const SISVariableSearchTypeText &other);

public:
	/**
	@brief Make search condition
	@param
	@return Condition string
	@remark
	*/
	CString MakeCondition();

	/**
	@brief Add text type variable search condition
	@param strCond [in]variable search condition
	@return Condition string
	@remark
	*/
	CString AddCondition(CString strCond);
};
