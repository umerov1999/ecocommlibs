#pragma once
#include <afxwin.h>  
#include <afxext.h>

#include "SISVariableSearchObject.h"
#include "SISProjectSearchTypeName.h"
#include "SISProjectSearchTypePjtType.h"
#include "SISProjectSearchTypeUserID.h"
#include "SISProjectSearchTypeUpdateDate.h"


class AFX_EXT_CLASS SISProjectSearchOption
{
public:
	CArray<SISVariableSearchObject, SISVariableSearchObject&> m_arrPjtSearchOpt;

public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISProjectSearchOption();

	/**
	@brief Creator
	@param c [in]Object of SISNProjectSearchOption
	@return
	@remark Initialize member variable using value of input object
	*/
	SISProjectSearchOption(const SISProjectSearchOption& c);

	/**
	@brief Destructors
	@param
	@return
	@remark
	*/
	~SISProjectSearchOption();

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISProjectSearchOption& operator = (const SISProjectSearchOption &other);

public:
	/**
	@brief Get search condition
	@param
	@return Search condition string
	@remark
	*/
	CString GetSearchCondition();

	/**
	@brief Add project name search condition
	@param nameType [in]project name search condition
	@return Search condition string
	@remark
	*/
	CString AddPjtNameCondition(SISProjectSearchTypeName nameType);

	/**
	@brief Add project type search condition
	@param pjtTypeType [in]project type search condition
	@return Search condition string
	@remark
	*/
	CString AddPjtTypeCondition(SISProjectSearchTypePjtType pjtTypeType);

	/**
	@brief Add user ID search condition
	@param userIDType [in]user ID search condition
	@return Search condition string
	@remark
	*/
	CString AddUserIDCondition(SISProjectSearchTypeUserID userIDType);

	/**
	@brief Add update date search condition
	@param updateDateType [in]update date search condition
	@return Search condition string
	@remark
	*/
	CString AddUpdateDateCondition(SISProjectSearchTypeUpdateDate updateDateType);
};
