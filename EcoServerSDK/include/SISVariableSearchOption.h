#pragma once

#ifndef _SISVariableSearchOption_H
#define _SISVariableSearchOption_H

#include <afxwin.h>  
#include <afxext.h>

#include "SISVariableSearchObject.h"
#include "SISVariableSearchTypeText.h"
#include "SISVariableSearchTypeDate.h"
#include "SISVariableSearchTypeNumber.h"

class AFX_EXT_CLASS SISVariableSearchOption
{
public:
	CArray<SISVariableSearchObject, SISVariableSearchObject&> m_arrVarSearchOpt;
	
public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISVariableSearchOption();

	/**
	@brief Creator
	@param c [in]Object of SISNVariableSearchOption
	@return
	@remark Initialize member variable using value of input object
	*/
	SISVariableSearchOption(const SISVariableSearchOption &c);

	/**
	@brief Destructors
	@param
	@return
	@remark
	*/
	~SISVariableSearchOption();

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISVariableSearchOption& operator = (const SISVariableSearchOption &other);

public:
	/**
	@brief Get search condition
	@param
	@return Search condition string
	@remark
	*/
	CString GetSearchCondition();

	/**
	@brief Add variable search condition
	@param textType [in]Text type variable search condition
	@return Condition string
	@remark
	*/
	CString AddCondition(SISVariableSearchTypeText textType);

	/**
	@brief Add variable search condition
	@param dateType [in]Date type variable search condition
	@return Condition string
	@remark
	*/
	CString AddCondition(SISVariableSearchTypeDate dateType);

	/**
	@brief Add variable search condition
	@param numberType [in]Number type variable search condition
	@return Condition string
	@remark
	*/
	CString AddCondition(SISVariableSearchTypeNumber numberType);
	

};




#endif