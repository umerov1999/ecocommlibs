#pragma once
#include <afxwin.h>  
#include <afxext.h>

#include "SISVariableSearchObject.h"

class AFX_EXT_CLASS SISVariableSearchTypeNumber : public SISVariableSearchObject
{

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISVariableSearchTypeNumber();

	/**
	@brief Destructors
	@param
	@return
	@remark
	*/
	~SISVariableSearchTypeNumber();

	/**
	@brief Creator
	@param c [in]Object of SISNVariableSearchTypeNumber
	@return
	@remark Initialize member variable using value of input object
	*/
	SISVariableSearchTypeNumber(SISVariableSearchTypeNumber& c);

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISVariableSearchTypeNumber& operator = (const SISVariableSearchTypeNumber &other);


public:
	/**
	@brief Make search condition
	@param
	@return Condition string
	@remark
	*/
	CString MakeCondition();

	/**
	@brief Add number type variable search condition
	@param dblStart [in]Start number
	@param dblEnd [in]End number
	@return Condition string
	@remark
	*/
	CString AddCondition(double dblStart, double dblEnd);

	/**
	@brief Add number type variable search condition
	@param dblNum [in]Standard number
	@param strInequality [in]Inequality
	@return Condition string
	@remark
	*/
	CString AddCondition(double dblNum, CString strInequality);
};
