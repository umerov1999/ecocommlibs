#pragma once
#include <afxwin.h>  
#include <afxext.h>

#include "SISVariableSearchObject.h"

class AFX_EXT_CLASS SISVariableSearchTypeDate : public SISVariableSearchObject
{
public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISVariableSearchTypeDate();

	/**
	@brief Creator
	@param c [in]Object of SISNVariableSearchTypeDate
	@return
	@remark Initialize member variable using value of input object
	*/
	SISVariableSearchTypeDate(SISVariableSearchTypeDate& c);

	/**
	@brief Destructors
	@param
	@return
	@remark
	*/
	~SISVariableSearchTypeDate();

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISVariableSearchTypeDate& operator = (const SISVariableSearchTypeDate &other);


public:
	/**
	@brief Make search condition
	@param
	@return Condition string
	@remark
	*/
	CString MakeCondition();

	/**
	@brief Add date type variable search condition
	@param dtStart [in]Start date
	@param dtEnd [in]End date
	@return Condition string
	@remark
	*/
	CString AddCondition(COleDateTime dtStart, COleDateTime dtEnd);

	/**
	@brief Add date type variable search condition
	@param nMinusSpanDay [in]-day
	@param dtBase [in]Standard date
	@param nPlusSpanDay [in]+day
	@return Condition string
	@remark
	*/
	CString AddCondition(int nMinusSpanDay, COleDateTime dtBase, int nPlusSpanDay);

	/**
	@brief Add date type variable search condition
	@param dtDate [in]Standard date
	@param strInequality [in]Inequality
	@return Condition string
	@remark
	*/
	CString AddCondition(COleDateTime dtDate, CString strInequality);
};

