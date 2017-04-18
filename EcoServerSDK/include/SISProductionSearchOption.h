#pragma once
#include <afxwin.h>  
#include <afxext.h>

#include "SISShipNoInfo.h"

class AFX_EXT_CLASS SISProductionSearchOption
{
private:
	CArray<SISShipNoInfo, SISShipNoInfo&> m_arrShipNoSearchOpt;
	CArray<CString, CString&> m_arrAssemLevelSearchOpt;
	CArray<CString, CString&> m_arrAssemBlockSearchOpt;
	CArray<CString, CString&> m_arrVarGroupSearchOpt;
	BOOL m_bIncNoneVarGropPjt;
	
public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISProductionSearchOption();

	/**
	@brief Creator
	@param c [in]Object of SISNProductionSearchOption
	@return
	@remark Initialize member variable using value of input object
	*/
	SISProductionSearchOption(const SISProductionSearchOption& c);

	/**
	@brief Creator
	@param shipNoCond [in]Group of ship number
	@param strAssemLevelCond [in]Assembly level
	@param strAssemBlockCond [in]Assembly block
	@param strVarGroupCond [in]Variable group
	@param bIncNoneVarGropPjt [in]Include none variable group project or not
	@return
	@remark Initialize member variable using parameter
	*/
	SISProductionSearchOption(SISShipNoInfo shipNoCond, CString strAssemLevelCond, CString strAssemBlockCond, CString strVarGroupCond, BOOL bIncNoneVarGropPjt = FALSE);

	/**
	@brief Destructors
	@param
	@return
	@remark
	*/
	~SISProductionSearchOption();

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISProductionSearchOption& operator = (const SISProductionSearchOption &other);

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
	@brief Clear variable group search condition
	@param
	@return
	@remark
	*/
	void ClearVarGroupCondition();

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
	@brief Add variable group search condition
	@param strCondition [in]Variable group search condition
	@return
	@remark
	*/
	void AddVariableGroupCondition(CString strCondition);

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

	/**
	@brief Get variable group search condition list
	@param arrVarGroupCond [out]Variable group search condition list
	@return
	@remark
	*/
	void GetVarGroupCondition(CArray<CString, CString&>& arrVarGroupCond);

	/**
	@brief Set include none variable group project
	@param bIncNoneVarGropPjt [in]Include none variable group project or not
	@return
	@remark
	*/
	void SetIncludeNoneVariableGroupProject(BOOL bIncNoneVarGropPjt);

	/**
	@brief Get include none variable group project
	@param 
	@return Include none variable group project or not
	@remark
	*/
	BOOL GetIncludeNoneVariableGroupProject();

};



