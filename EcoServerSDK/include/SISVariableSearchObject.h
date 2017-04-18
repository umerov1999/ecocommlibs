#pragma once
#include <afxwin.h>  
#include <afxext.h>

class AFX_EXT_CLASS SISVariableSearchObject
{
public:
	CString m_strVarName;
	CArray<CString, CString&> m_arrCondition;

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISVariableSearchObject();

	/**
	@brief Destructors
	@param
	@return
	@remark
	*/
	~SISVariableSearchObject();

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISVariableSearchObject& operator = (const SISVariableSearchObject &other);

public:
	/**
	@brief Make search condition
	@param
	@return Condition string
	@remark
	*/
	virtual CString MakeCondition();

	/**
	@brief Clear search condition
	@param
	@return
	@remark
	*/
	void ClearCondition();

	/**
	@brief Set variable name
	@param strVarName [in]Variable name
	@return
	@remark
	*/
	virtual void SetVariableName(CString strVarName);

	/**
	@brief Get variable name
	@param
	@return variable name
	@remark
	*/
	CString GetVariableName();
};


