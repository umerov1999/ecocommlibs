#pragma once

#ifndef _SISAssemBlockSearchOption_H
#define _SISAssemBlockSearchOption_H

#include <afxwin.h>  
#include <afxext.h>

#include "SISShipNoInfo.h"

class AFX_EXT_CLASS SISAssemBlockSearchOption
{
private:
	CArray<SISShipNoInfo, SISShipNoInfo&> m_arrShipNoSearchOpt;
	CArray<CString, CString&> m_arrAssemLevelSearchOpt;
	CArray<CString, CString&> m_arrAssemBlockSearchOpt;
	
public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISAssemBlockSearchOption();

	/**
	@brief Creator
	@param c [in]Object of SISNAssemBlockSearchOption
	@return
	@remark Initialize member variable using value of input object
	*/
	SISAssemBlockSearchOption(const SISAssemBlockSearchOption &c);

	/**
	@brief Creator
	@param shipNoCond [in]Ship number
	@param strAssemLevelCond [in]Assembly Level
	@param strAssemBlockCond [in]Assembly Block
	@return
	@remark Initialize member variable using parameter
	*/
	SISAssemBlockSearchOption(SISShipNoInfo shipNoCond, CString strAssemLevelCond, CString strAssemBlockCond);

	/**
	@brief Destructors
	@param
	@return
	@remark
	*/
	~SISAssemBlockSearchOption();

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISAssemBlockSearchOption& operator = (const SISAssemBlockSearchOption &other);

public:
	/**
	@brief Clear ship number search condition
	@param
	@return
	@remark
	*/
	void ClearShipNoCondition();

	/**
	@brief Clear assembly level search condition
	@param
	@return
	@remark
	*/
	void ClearAssemLevelCondition();

	/**
	@brief Clear assembly block search condition
	@param
	@return
	@remark
	*/
	void ClearAssemBlockCondition();

	/**
	@brief Add ship number search condition
	@param shipNoCondition [in]Ship number search condition
	@return
	@remark
	*/
	void AddShipNoCondition(SISShipNoInfo shipNoCondition);

	/**
	@brief Add assembly level search condition
	@param strCondition [in]Assembly level search condition
	@return
	@remark
	*/
	void AddAssemLevelCondition(CString strCondition);

	/**
	@brief Add assembly block search condition
	@param strCondition [in]Assembly block search condition
	@return
	@remark
	*/
	void AddAssemBlockCondition(CString strCondition);

	/**
	@brief Get ship number search condition list
	@param arrShipNoCond [out]Ship number search condition list
	@return
	@remark
	*/
	void GetShipNoCondition(CArray<SISShipNoInfo, SISShipNoInfo&>& arrShipNoCond);

	/**
	@brief Get assembly level search condition list
	@param arrAssemLevelCond [out]Assembly level search condition list
	@return
	@remark
	*/
	void GetAssemLevelCondition(CArray<CString, CString&>& arrAssemLevelCond);

	/**
	@brief Get assembly block search condition list
	@param arrAssemBlockCond [out]Assembly block search condition list
	@return
	@remark
	*/
	void GetAssemBlockCondition(CArray<CString, CString&>& arrAssemBlockCond);

};



#endif