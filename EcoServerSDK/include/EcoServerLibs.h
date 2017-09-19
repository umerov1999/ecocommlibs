#pragma once

#include "SISPointStr.h"
#include "SISUserInfo.h"
#include "SISShipNoInfo.h"
#include "SISAssemLevel.h"
#include "SISAssemBlockInfo.h"
#include "SISVariableGroup.h"
#include "SISUserVariable.h"
#include "SISUserVariableOfGroup.h"
#include "SISProjectInfo.h"
#include "SISAssemBlockProp.h"
#include "SISProjectProp.h"
#include "SISObjNameSearchOption.h"
#include "SISVariableSearchOption.h"
#include "SISProductionSearchOption.h"
#include "SISAssemBlockSearchOption.h"
#include "SISProjectSearchOption.h"
#include "SISAnalysisInfo.h"
#include "SISDefines.h"
#include "SISAlarmInfo.h"
#include "SISPermission.h"
#include "SISFieldInfo.h"
#include "SISWorkFlow.h"
#include "SISAssign.h"
#include "SISFieldType.h"
#include "SISWork.h"
#include "SISNotification.h"
#include "SISFieldValue.h"
#include "SISTime.h"
#include "SISMesMsrData.h"


/**************************************************
Define Error Codes
**************************************************/
#define	ECO_OK					0x0000
#define	ECO_FAILE				0x0001
#define	ECO_DUPLICATE_HWND		0x0002
#define	ECO_DB_NOT_CONNECT		0x0003
#define	ECO_NOT_FOUND_SHIPNO_INFO 0x0004
#define	ECO_PARAMETER_ERROR 0x0005
#define	ECO_INVALID_COORD 0x0006
#define	ECO_VAR_SEARCH_OPT_ERROR 0x0007
#define	ECO_PJT_SEARCH_OPT_ERROR 0x0008
#define	ECO_NOT_FOUND_PROJECT 0x0009
#define	ECO_NOT_EXIST_DRAFT 0x0010
#define	ECO_NOT_EXIST_ATTACH_FILE 0x0011
#define	ECO_NOT_FOUND_DATA 0x0012
#define	ECO_EXIST_DATA 0x0013
#define	ECO_CAN_NOT_DELETE 0x0014
#define	ECO_EXIST_FILE 0x0015
#define	ECO_EXIST_USER 0x0016
#define ECO_UNLOCK_FAIL 0x0017
#define	ECO_CANCEL 0x0018

/**************************************************
Extern Class...
**************************************************/

class CEcoServerCtrl;
class CDataBaseManager;
class CConCurrent;
class AFX_EXT_API CEcoServerLibs
{
public:
	CEcoServerLibs();
	~CEcoServerLibs();

private:
	CEcoServerCtrl* m_pServerCtrl;
	CDataBaseManager* m_pDBManager;
	CConCurrent* m_pConCurrent;

public:
	BOOL m_bDBConnect;

public:
	/**
	@brief Unlock product
	@param strUnlock [in]NULL or license string
	@return Error code
	@remark
	*/
	LONG SIS_UnlockProduct(const char* strUnlock = NULL);

	/**
	@brief Login to EcoDB
	@param strHost [in]IP Address
	@param strUser [in]User ID
	@param strPwd [in]Password
	@return Error code
	@remark
	*/
	LONG SIS_Login(CString strHost, CString strUser, CString strPwd);

	/**
	@brief Logout from EcoDB
	@param
	@return Error code
	@remark
	*/
	LONG SIS_Logout();

//production 정보 검색
public:
	/**
	@brief Verify ship number information
	@param arrShipNo [out]Ship number
	@return Error code
	@remark
	*/
	LONG SIS_GetShipNo(CArray<SISShipNoInfo, SISShipNoInfo&>& arrShipNo);

	/**
	@brief Verify user information
	@param arrUser [out]User information
	@return Error code
	@remark
	*/
	LONG SIS_GetUser(CArray<SISUserInfo, SISUserInfo&>& arrUser);

	/**
	@brief Verify Assembly Level
	@param shipNoInfo [in]Ship number
	@param arrAssemLevel [out]Assembly level list
	@return Error code
	@remark
	*/
	LONG SIS_GetAssemblyLevel(SISShipNoInfo shipNoInfo, CArray<SISAssemLevel, SISAssemLevel&>& arrAssemLevel);

	/**
	@brief Verify assembly level
	@param arrShipNo [in]Ship number list
	@param arrAssemLevel [out]Assembly level list
	@return Error code
	@remark
	*/
	LONG SIS_GetAssemblyLevel(CArray<SISShipNoInfo, SISShipNoInfo&>& arrShipNo, CArray<SISAssemLevel, SISAssemLevel&>& arrAssemLevel);

	/**
	@brief Verify user variable
	@param shipNoInfo [in]Ship number
	@param arrUserVariable [out]User variable list
	@return Error code
	@remark
	*/
	LONG SIS_GetUserVariable(SISShipNoInfo shipNoInfo, CArray<SISUserVariable, SISUserVariable&>& arrUserVariable);

	/**
	@brief Verify user variable
	@param arrShipNo [in]Ship number list
	@param arrUserVariable [out]User variable list
	@return Error code
	@remark
	*/
	LONG SIS_GetUserVariable(CArray<SISShipNoInfo, SISShipNoInfo&>& arrShipNo, CArray<SISUserVariable, SISUserVariable&>& arrUserVariable);

	/**
	@brief Verify variable group
	@param shipNoInfo [in]Ship number
	@param strAssemLevel [in]Assembly level
	@param arrVariableGroup [out]Variable group list
	@return Error code
	@remark
	*/
	LONG SIS_GetUserVariableGroup(SISShipNoInfo shipNoInfo, CString strAssemLevel, CArray<SISVariableGroup, SISVariableGroup&>& arrVariableGroup);

	/**
	@brief Verify variable group
	@param arrShipNo [in]Ship number list
	@param arrAssemLevel [in]Assembly level list
	@param arrVariableGroup [out]Variable group list
	@return Error code
	@remark
	*/
	LONG SIS_GetUserVariableGroup(CArray<SISShipNoInfo, SISShipNoInfo&>& arrShipNo, CArray<CString, CString&>& arrAssemLevel, CArray<SISVariableGroup, SISVariableGroup&>& arrVariableGroup);

	/**
	@brief Verify variable of variable group
	@param shipNoInfo [in]Ship number
	@param strAssemLevel [in]Assembly level
	@param strVarGroup [in]Variable group
	@param arrUserVariable [out]Variable list of variable group
	@return Error code
	@remark
	*/
	LONG SIS_GetUserVariableOfGroup(SISShipNoInfo shipNoInfo, CString strAssemLevel, CString strVarGroup, CArray<SISUserVariableOfGroup, SISUserVariableOfGroup&>& arrUserVariable);

	/**
	@brief Verify variable of variable group
	@param arrShipNo [in]Ship number list
	@param arrAssemLevel [in]Assembly level list
	@param arrVarGroup [in]Variable group list
	@param arrUserVariable [out]Variable list of variable group
	@return Error code
	@remark
	*/
	LONG SIS_GetUserVariableOfGroup(CArray<SISShipNoInfo, SISShipNoInfo&>& arrShipNo, CArray<CString, CString&>& arrAssemLevel, 
									CArray<CString, CString&>& arrVarGroup, CArray<SISUserVariableOfGroup, SISUserVariableOfGroup&>& arrUserVariable);
	
//블록 및 프로젝트 검색
public:
	/**
	@brief Verify assembly block
	@param shipNoInfo [in]Ship number
	@param strAssemLevel [in]Assembly level
	@param strAssemBlock [in]Assembly block
	@param arrAssemBlockInfo [out]Assembly block list
	@return Error code
	@remark
	*/
	LONG SIS_GetAssemblyBlockInfo(SISShipNoInfo shipNoInfo, CString strAssemLevel, CString strAssemBlock, CArray<SISAssemBlockInfo, SISAssemBlockInfo&>& arrAssemBlockInfo);

	/**
	@brief Verify assembly block
	@param assemBlockSearchOption [in]Assembly block search condition
	@param arrAssemBlockInfo [out]Assembly block list
	@return Error code
	@remark
	*/
	LONG SIS_GetAssemblyBlockInfo(SISAssemBlockSearchOption assemBlockSearchOption, CArray<SISAssemBlockInfo, SISAssemBlockInfo&>& arrAssemBlockInfo);
	
	/**
	@brief Verify project information
	@param shipNoInfo [in]Ship number
	@param strAssemLevel [in]Assembly level
	@param strAssemBlock [in]Assembly block
	@param strVarGroup [in]Variable group
	@param bIncNoneVarGropPjt [in]Include none variable group project or not
	@param strPjtName [in]Project name
	@param arrPjtInfo [out]Project information
	@return Error code
	@remark
	*/
	LONG SIS_GetProjectInfo(SISShipNoInfo shipNoInfo, CString strAssemLevel, CString strAssemBlock, CString strVarGroup, BOOL bIncNoneVarGropPjt, CString strPjtName, CArray<SISProjectInfo, SISProjectInfo&>& arrPjtInfo);

	/**
	@brief Verify project information
	@param prodSearchOption [in]Production information search condition
	@param strPjtName [in]Project name
	@param arrPjtInfo [out]Project information
	@return Error code
	@remark
	*/
	LONG SIS_GetProjectInfo(SISProductionSearchOption prodSearchOption, CString strPjtName, CArray<SISProjectInfo, SISProjectInfo&>& arrPjtInfo);

	/**
	@brief Verify project information
	@param prodSearchOption [in]Production information search condition
	@param pjtSearchOption [in]Project search condition
	@param arrPjtInfo [out]Project information
	@return Error code
	@remark
	*/
	LONG SIS_GetProjectInfo(SISProductionSearchOption prodSearchOption, SISProjectSearchOption pjtSearchOption, CArray<SISProjectInfo, SISProjectInfo&>& arrPjtInfo);

	/**
	@brief Verify project information
	@param shipNoInfo [in]Ship number
	@param nProjectID [in]Project ID
	@param pjtInfo [out]Project information
	@return Error code
	@remark
	*/
	LONG SIS_GetProjectInfo(SISShipNoInfo shipNoInfo, UINT nProjectID, SISProjectInfo& pjtInfo);

	///**
	//@brief Verify assembly block property
	//@param shipNoInfo [in]Ship number
	//@param strAssemLevel [in]Assembly level
	//@param strAssemBlock [in]Assembly block
	//@param arrAssemBlockProp [out]Assembly block property list
	//@return Error code
	//@remark
	//*/
	//LONG SIS_GetAssemblyBlockProp(SISShipNoInfo shipNoInfo, CString strAssemLevel, CString strAssemBlock, CArray<SISAssemBlockProp, SISAssemBlockProp&>& arrAssemBlockProp);

	///**
	//@brief Verify assembly block property
	//@param assemBlockSearchOption [in]Assembly block search condition
	//@param arrAssemBlockProp [out]Assembly block property list
	//@return Error code
	//@remark
	//*/
	//LONG SIS_GetAssemblyBlockProp(SISAssemBlockSearchOption assemBlockSearchOption, CArray<SISAssemBlockProp, SISAssemBlockProp&>& arrAssemBlockProp);

	/**
	@brief Verify project property
	@param shipNoInfo [in]Ship number
	@param strAssemLevel [in]Assembly level
	@param strAssemBlock [in]Assembly block
	@param strVarGroup [in]Variable group
	@param bIncNoneVarGropPjt [in]Include none variable group project or not
	@param strPjtName [in]Project name
	@param arrPjtProp [out]Project property list
	@return Error code
	@remark
	*/
	LONG SIS_GetProjectProp(SISShipNoInfo shipNoInfo, CString strAssemLevel, CString strAssemBlock, CString strVarGroup, BOOL bIncNoneVarGropPjt, CString strPjtName, CArray<SISProjectProp, SISProjectProp&>& arrPjtProp);

	/**
	@brief Verify project property
	@param prodSearchOption [in]Production information search condition
	@param strPjtName [in]Project name
	@param arrPjtProp [out]Project property list
	@return Error code
	@remark
	*/
	LONG SIS_GetProjectProp(SISProductionSearchOption prodSearchOption, CString strPjtName, CArray<SISProjectProp, SISProjectProp&>& arrPjtProp);

	/**
	@brief Verify project property
	@param prodSearchOption [in]Production information search condition
	@param pjtSearchOption [in]Project search condition
	@param arrPjtProp [out]Project property list
	@return Error code
	@remark
	*/
	LONG SIS_GetProjectProp(SISProductionSearchOption prodSearchOption, SISProjectSearchOption pjtSearchOption, CArray<SISProjectProp, SISProjectProp&>& arrPjtProp);

	///**
	//@brief Verify assembly block information
	//@param shipNoInfo [in]Ship number
	//@param strAssemLevel [in]Assembly level
	//@param strAssemBlock [in]Assembly block
	//@param varSearchOption [in]Variable search condition
	//@param arrAssemBlock [out]Assembly block information list
	//@return Error code
	//@remark
	//*/
	LONG SIS_GetAssemblyBlock(SISShipNoInfo shipNoInfo, CString strAssemLevel, CString strAssemBlock, SISVariableSearchOption varSearchOption, CArray<SISAssemBlock, SISAssemBlock&>& arrAssemBlock);

	///**
	//@brief Verify assembly block information
	//@param assemBlockSearchOption [in]Assembly block search condition
	//@param varSearchOption [in]Variable search condition
	//@param arrAssemBlock Assembly block information list
	//@return Error code
	//@remark
	//*/
	LONG SIS_GetAssemblyBlock(SISAssemBlockSearchOption assemBlockSearchOption, SISVariableSearchOption varSearchOption, CArray<SISAssemBlock, SISAssemBlock&>& arrAssemBlock);

	/**
	@brief Verify project information
	@param shipNoInfo [in]Ship number
	@param strAssemLevel [in]Assembly level
	@param strAssemBlock [in]Assembly block
	@param strVarGroup [in]Variable group
	@param bIncNoneVarGropPjt [in]Include none variable group project or not
	@param strPjtName [in]Project name
	@param varSearchOption [in]Variable search condition
	@param arrPjt [out]Project information
	@return Error code
	@remark
	*/
	LONG SIS_GetProject(SISShipNoInfo shipNoInfo, CString strAssemLevel, CString strAssemBlock, CString strVarGroup, BOOL bIncNoneVarGropPjt, CString strPjtName, SISVariableSearchOption varSearchOption, CArray<SISProject, SISProject&>& arrPjt);

	/**
	@brief Verify project information
	@param prodSearchOption [in]Production information search condition
	@param strPjtName [in]Project name
	@param varSearchOption [in]Variable search condition
	@param arrPjt [out]Project information
	@return Error code
	@remark
	*/
	LONG SIS_GetProject(SISProductionSearchOption prodSearchOption, CString strPjtName, SISVariableSearchOption varSearchOption, CArray<SISProject, SISProject&>& arrPjt);

	/**
	@brief Verify project information
	@param prodSearchOption [in]Production information search condition
	@param pjtSearchOption [in]Project search condition
	@param varSearchOption [in]Variable search condition
	@param arrPjt [out]Project information
	@return Error code
	@remark
	*/
	LONG SIS_GetProject(SISProductionSearchOption prodSearchOption, SISProjectSearchOption pjtSearchOption, SISVariableSearchOption varSearchOption, CArray<SISProject, SISProject&>& arrPjt);

	/**
	@brief Get Linked project presence or absence
	@param strGroup [in]Group of ship number
	@param strShipNo [in]Ship number
	@param nProjectID [in]Project ID
	@param bLinked [out]Linked or UnLinked
	@return Error code
	@remark
	*/
	LONG SIS_IsLinkedProject(CString strGroup, CString strShipNo, UINT nProjectID, BOOL& bLinked);

	/**
	@brief Get Linked project ID
	@param strGroup [in]Group of ship number
	@param strShipNo [in]Ship number
	@param nProjectID [in]Project ID
	@param arrLinkedPjtID [out]Linked project ID
	@return Error code
	@remark
	*/
	LONG SIS_GetLinkedProjectID(CString strGroup, CString strShipNo, CArray<UINT, UINT>& arrLinkedPjtID);

	/**
	@brief Get Linked project presence or absence
	@param strGroup [in]Group of ship number
	@param strShipNo [in]Ship number
	@param strAssemLevel [in]Assembly level
	@param strAssemBlock [in]Assembly block
	@param strMsrName [in]Measurement data name
	@param arrMesMsrData [out]Measurement data information
	@return Error code
	@remark
	*/
	LONG SIS_GetMesMsrDataInfo(CString strGroup, CString strShipNo, CString strAssemLevel, CString strAssemBlock, CString strMsrName, CArray<SISMesMsrData, SISMesMsrData&>& arrMesMsrData);

//치수조회
public:
	/**
	@brief Verify Dimension information using model object name
	@param shipNoInfo [in]Ship number
	@param strAssemLevel [in]Assembly level
	@param strAssemBlock [in]Assembly block
	@param strVarGroup [in]Variable group
	@param bIncNoneVarGropPjt [in]Include none variable group project or not
	@param strPjtName [in]Project name
	@param strMngType [in]Dimension type—(see below)
	@param objNameOption [in]Assembly block
	@param arrAnalysisInfo [out]Dimension information
	@return Error code
	@remark
	The strMngType parameters is the following\r\n
	"MNG_ALL" - The value is all dimension\r\n
	"MNG_POINT" - The value is dimension for management point\r\n
	"MNG_LENGTH" - The value is dimension for management length\r\n
	"MNG_PERP" - The value is dimension for management perpendicular\r\n
	"MNG_ANGLE" - The value is dimension for management angle
	*/
	LONG SIS_GetAnalysisInfoByObjectName(SISShipNoInfo shipNoInfo, CString strAssemLevel, CString strAssemBlock, CString strVarGroup, BOOL bIncNoneVarGropPjt, CString strPjtName, CString strMngType,
		SISObjNameSearchOption objNameOption, CArray<SISAnalysisInfo, SISAnalysisInfo&>& arrAlaysisInfo);

	/**
	@brief Verify Dimension information using model object name
	@param prodSearchOption [in]Production information search condition
	@param strPjtName [in]Project name
	@param strMngType [in]Dimension type—(see below)
	@param objNameOption [in]Assembly block
	@param arrAnalysisInfo [out]Dimension information
	@return Error code
	@remark
	The strMngType parameters is the following\r\n
	"MNG_ALL" - The value is all dimension\r\n
	"MNG_POINT" - The value is dimension for management point\r\n
	"MNG_LENGTH" - The value is dimension for management length\r\n
	"MNG_PERP" - The value is dimension for management perpendicular\r\n
	"MNG_ANGLE" - The value is dimension for management angle
	*/
	LONG SIS_GetAnalysisInfoByObjectName(SISProductionSearchOption prodSearchOption, CString strPjtName, CString strMngType,
		SISObjNameSearchOption objNameOption, CArray<SISAnalysisInfo, SISAnalysisInfo&>& arrAlaysisInfo);

	/**
	@brief Verify Dimension information using model object name
	@param prodSearchOption [in]Production information search condition
	@param pjtSearchOption [in]Project search condition
	@param strMngType [in]Dimension type—(see below)
	@param objNameOption [in]Assembly block
	@param arrAnalysisInfo [out]Dimension information
	@return Error code
	@remark
	The strMngType parameters is the following\r\n
	"MNG_ALL" - The value is all dimension\r\n
	"MNG_POINT" - The value is dimension for management point\r\n
	"MNG_LENGTH" - The value is dimension for management length\r\n
	"MNG_PERP" - The value is dimension for management perpendicular\r\n
	"MNG_ANGLE" - The value is dimension for management angle
	*/
	LONG SIS_GetAnalysisInfoByObjectName(SISProductionSearchOption prodSearchOption, SISProjectSearchOption pjtSearchOption, CString strMngType, 
		SISObjNameSearchOption objNameOption, CArray<SISAnalysisInfo, SISAnalysisInfo&>& arrAlaysisInfo);

	/**
	@brief Verify Dimension information using bounding
	@param shipNoInfo [in]Ship number
	@param strAssemLevel [in]Assembly level
	@param strAssemBlock [in]Assembly block
	@param strVarGroup [in]Variable group
	@param bIncNoneVarGropPjt [in]Include none variable group project or not
	@param strPjtName [in]Project name
	@param strMngType [in]Dimension type—(see below)
	@param strBoundDir [in]Standard direction of bounding—(see below)
	@param ptMin [in]Minimum coordinate
	@param ptMax [in]Maximum coordinate
	@param arrAnalysisInfo [out]Dimension information
	@return Error code
	@remark
	The strMngType parameters is the following\r\n
	"MNG_ALL" - The value is all dimension\r\n
	"MNG_POINT" - The value is dimension for management point\r\n
	"MNG_LENGTH" - The value is dimension for management length\r\n
	"MNG_PERP" - The value is dimension for management perpendicular\r\n
	"MNG_ANGLE" - The value is dimension for management angle\r\n\r\n
	The strBoundDir parameters is the following\r\n
	"BOUND_X" - The value is valid only in the X direction of the bounding\r\n
	"BOUND_Y" - The value is valid only in the Y direction of the bounding\r\n
	"BOUND_Z" - The value is valid only in the Z direction of the bounding\r\n
	"BOUND_XY" - The value is valid only in the X, Y direction of the bounding\r\n
	"BOUND_XZ" - The value is valid only in the X, Z direction of the bounding\r\n
	"BOUND_YZ" - The value is valid only in the Y, Z direction of the bounding\r\n
	"BOUND_XYZ" - The value is valid only in the X, Y, Z direction of the bounding\r\n
	*/
	LONG SIS_GetAnalysisInfoByBounding(SISShipNoInfo shipNoInfo, CString strAssemLevel, CString strAssemBlock, CString strVarGroup, BOOL bIncNoneVarGropPjt, CString strPjtName, CString strMngType,
		CString strBoundDir, SISPoint ptMin, SISPoint ptMax, CArray<SISAnalysisInfo, SISAnalysisInfo&>& arrAlaysisInfo);

	/**
	@brief Verify Dimension information using bounding
	@param prodSearchOption [in]Production information search condition
	@param strPjtName [in]Project name
	@param strMngType [in]Dimension type—(see below)
	@param strBoundDir [in]Standard direction of bounding—(see below)
	@param ptMin [in]Minimum coordinate
	@param ptMax [in]Maximum coordinate
	@param arrAnalysisInfo [out]Dimension information
	@return Error code
	@remark
	The strMngType parameters is the following\r\n
	"MNG_ALL" - The value is all dimension\r\n
	"MNG_POINT" - The value is dimension for management point\r\n
	"MNG_LENGTH" - The value is dimension for management length\r\n
	"MNG_PERP" - The value is dimension for management perpendicular\r\n
	"MNG_ANGLE" - The value is dimension for management angle\r\n\r\n
	The strBoundDir parameters is the following\r\n
	"BOUND_X" - The value is valid only in the X direction of the bounding\r\n
	"BOUND_Y" - The value is valid only in the Y direction of the bounding\r\n
	"BOUND_Z" - The value is valid only in the Z direction of the bounding\r\n
	"BOUND_XY" - The value is valid only in the X, Y direction of the bounding\r\n
	"BOUND_XZ" - The value is valid only in the X, Z direction of the bounding\r\n
	"BOUND_YZ" - The value is valid only in the Y, Z direction of the bounding\r\n
	"BOUND_XYZ" - The value is valid only in the X, Y, Z direction of the bounding\r\n
	*/
	LONG SIS_GetAnalysisInfoByBounding(SISProductionSearchOption prodSearchOption, CString strPjtName, CString strMngType,
		CString strBoundDir, SISPoint ptMin, SISPoint ptMax, CArray<SISAnalysisInfo, SISAnalysisInfo&>& arrAlaysisInfo);

	/**
	@brief Verify Dimension information using bounding
	@param prodSearchOption [in]Production information search condition
	@param pjtSearchOption [in]Project search condition
	@param strMngType [in]Dimension type—(see below)
	@param strBoundDir [in]Standard direction of bounding—(see below)
	@param ptMin [in]Minimum coordinate
	@param ptMax [in]Maximum coordinate
	@param arrAnalysisInfo [out]Dimension information
	@return Error code
	@remark
	The strMngType parameters is the following\r\n
	"MNG_ALL" - The value is all dimension\r\n
	"MNG_POINT" - The value is dimension for management point\r\n
	"MNG_LENGTH" - The value is dimension for management length\r\n
	"MNG_PERP" - The value is dimension for management perpendicular\r\n
	"MNG_ANGLE" - The value is dimension for management angle\r\n\r\n
	The strBoundDir parameters is the following\r\n
	"BOUND_X" - The value is valid only in the X direction of the bounding\r\n
	"BOUND_Y" - The value is valid only in the Y direction of the bounding\r\n
	"BOUND_Z" - The value is valid only in the Z direction of the bounding\r\n
	"BOUND_XY" - The value is valid only in the X, Y direction of the bounding\r\n
	"BOUND_XZ" - The value is valid only in the X, Z direction of the bounding\r\n
	"BOUND_YZ" - The value is valid only in the Y, Z direction of the bounding\r\n
	"BOUND_XYZ" - The value is valid only in the X, Y, Z direction of the bounding\r\n
	*/
	LONG SIS_GetAnalysisInfoByBounding(SISProductionSearchOption prodSearchOption, SISProjectSearchOption pjtSearchOption, CString strMngType,
		CString strBoundDir, SISPoint ptMin, SISPoint ptMax, CArray<SISAnalysisInfo, SISAnalysisInfo&>& arrAlaysisInfo);

	/**
	@brief Verify Dimension information using bounding
	@param shipNoInfo [in]Ship number
	@param strAssemLevel [in]Assembly level
	@param strAssemBlock [in]Assembly block
	@param strVarGroup [in]Variable group
	@param bIncNoneVarGropPjt [in]Include none variable group project or not
	@param strPjtName [in]Project name
	@param strMngType [in]Dimension type—(see below)
	@param strBoundDir [in]Standard direction of bounding—(see below)
	@param ptMinStr [in]Minimum coordinate of the string form(Able FR, LP form)
	@param ptMaxStr [in]Maximum coordinate of the string form(Able FR, LP form)
	@param arrAnalysisInfo [out]Dimension information
	@return Error code
	@remark
	The strMngType parameters is the following\r\n
	"MNG_ALL" - The value is all dimension\r\n
	"MNG_POINT" - The value is dimension for management point\r\n
	"MNG_LENGTH" - The value is dimension for management length\r\n
	"MNG_PERP" - The value is dimension for management perpendicular\r\n\r\n
	"MNG_ANGLE" - The value is dimension for management angle\r\n
	The strBoundDir parameters is the following\r\n
	"BOUND_X" - The value is valid only in the X direction of the bounding\r\n
	"BOUND_Y" - The value is valid only in the Y direction of the bounding\r\n
	"BOUND_Z" - The value is valid only in the Z direction of the bounding\r\n
	"BOUND_XY" - The value is valid only in the X, Y direction of the bounding\r\n
	"BOUND_XZ" - The value is valid only in the X, Z direction of the bounding\r\n
	"BOUND_YZ" - The value is valid only in the Y, Z direction of the bounding\r\n
	"BOUND_XYZ" - The value is valid only in the X, Y, Z direction of the bounding\r\n
	*/
	LONG SIS_GetAnalysisInfoByBounding(SISShipNoInfo shipNoInfo, CString strAssemLevel, CString strAssemBlock, CString strVarGroup, BOOL bIncNoneVarGropPjt, CString strPjtName, CString strMngType,
		CString strBoundDir, SISPointStr ptMinStr, SISPointStr ptMaxStr, CArray<SISAnalysisInfo, SISAnalysisInfo&>& arrAlaysisInfo);

	/**
	@brief Verify Dimension information using bounding
	@param prodSearchOption [in]Production information search condition
	@param strPjtName [in]Project name
	@param strMngType [in]Dimension type—(see below)
	@param strBoundDir [in]Standard direction of bounding—(see below)
	@param ptMinStr [in]Minimum coordinate of the string form(Able FR, LP form)
	@param ptMaxStr [in]Maximum coordinate of the string form(Able FR, LP form)
	@param arrAnalysisInfo [out]Dimension information
	@return Error code
	@remark
	The strMngType parameters is the following\r\n
	"MNG_ALL" - The value is all dimension\r\n
	"MNG_POINT" - The value is dimension for management point\r\n
	"MNG_LENGTH" - The value is dimension for management length\r\n
	"MNG_PERP" - The value is dimension for management perpendicular\r\n
	"MNG_ANGLE" - The value is dimension for management angle\r\n\r\n
	The strBoundDir parameters is the following\r\n
	"BOUND_X" - The value is valid only in the X direction of the bounding\r\n
	"BOUND_Y" - The value is valid only in the Y direction of the bounding\r\n
	"BOUND_Z" - The value is valid only in the Z direction of the bounding\r\n
	"BOUND_XY" - The value is valid only in the X, Y direction of the bounding\r\n
	"BOUND_XZ" - The value is valid only in the X, Z direction of the bounding\r\n
	"BOUND_YZ" - The value is valid only in the Y, Z direction of the bounding\r\n
	"BOUND_XYZ" - The value is valid only in the X, Y, Z direction of the bounding\r\n
	*/
	LONG SIS_GetAnalysisInfoByBounding(SISProductionSearchOption prodSearchOption, CString strPjtName, CString strMngType,
		CString strBoundDir, SISPointStr ptMinStr, SISPointStr ptMaxStr, CArray<SISAnalysisInfo, SISAnalysisInfo&>& arrAlaysisInfo);

	/**
	@brief Verify Dimension information using bounding
	@param prodSearchOption [in]Production information search condition
	@param pjtSearchOption [in]Project search condition
	@param strMngType [in]Dimension type—(see below)
	@param strBoundDir [in]Standard direction of bounding—(see below)
	@param ptMinStr [in]Minimum coordinate of the string form(Able FR, LP form)
	@param ptMaxStr [in]Maximum coordinate of the string form(Able FR, LP form)
	@param arrAnalysisInfo [out]Dimension information
	@return Error code
	@remark
	The strMngType parameters is the following\r\n
	"MNG_ALL" - The value is all dimension\r\n
	"MNG_POINT" - The value is dimension for management point\r\n
	"MNG_LENGTH" - The value is dimension for management length\r\n
	"MNG_PERP" - The value is dimension for management perpendicular\r\n
	"MNG_ANGLE" - The value is dimension for management angle\r\n\r\n
	The strBoundDir parameters is the following\r\n
	"BOUND_X" - The value is valid only in the X direction of the bounding\r\n
	"BOUND_Y" - The value is valid only in the Y direction of the bounding\r\n
	"BOUND_Z" - The value is valid only in the Z direction of the bounding\r\n
	"BOUND_XY" - The value is valid only in the X, Y direction of the bounding\r\n
	"BOUND_XZ" - The value is valid only in the X, Z direction of the bounding\r\n
	"BOUND_YZ" - The value is valid only in the Y, Z direction of the bounding\r\n
	"BOUND_XYZ" - The value is valid only in the X, Y, Z direction of the bounding\r\n
	*/
	LONG SIS_GetAnalysisInfoByBounding(SISProductionSearchOption prodSearchOption, SISProjectSearchOption pjtSearchOption, CString strMngType,
		CString strBoundDir, SISPointStr ptMinStr, SISPointStr ptMaxStr, CArray<SISAnalysisInfo, SISAnalysisInfo&>& arrAlaysisInfo);

	/**
	@brief Verify Dimension information using X , Y, Z plane
	@param shipNoInfo [in]Ship number
	@param strAssemLevel [in]Assembly level
	@param strAssemBlock [in]Assembly block
	@param strVarGroup [in]Variable group
	@param bIncNoneVarGropPjt [in]Include none variable group project or not
	@param strPjtName [in]Project name
	@param strMngType [in]Dimension type—(see below)
	@param strStandardPlane [in]Standard plane—(see below)
	@param strDirection [in]Plane direction—(see below)
	@param dblStandardPos [in]Plane position
	@param arrAnalysisInfo [out]Dimension information
	@return Error code
	@remark
	The strMngType parameters is the following\r\n
	"MNG_ALL" - The value is all dimension\r\n
	"MNG_POINT" - The value is dimension for management point\r\n
	"MNG_LENGTH" - The value is dimension for management length\r\n
	"MNG_PERP" - The value is dimension for management perpendicular\r\n
	"MNG_ANGLE" - The value is dimension for management angle\r\n\r\n
	The strStandardPlane parameters is the following\r\n
	"PLANE_X" - The value is get analysis information based on the X-plane\r\n
	"PLANE_Y" - The value is get analysis information based on the Y-plane\r\n
	"PLANE_Z" - The value is get analysis information based on the Z-plane\r\n\r\n
	The strDirection parameters is the following\r\n
	"+" - The value is get analysis information for plus direction of the strStandardPlane parameters\r\n
	"-" - The value is get analysis information for minus direction of the strStandardPlane parameters\r\n
	*/
	LONG SIS_GetAnalysisInfoByPlane(SISShipNoInfo shipNoInfo, CString strAssemLevel, CString strAssemBlock, CString strVarGroup, BOOL bIncNoneVarGropPjt, CString strPjtName, CString strMngType,
		CString strStandardPlane, CString strDirection, double dblStandardPos, CArray<SISAnalysisInfo, SISAnalysisInfo&>& arrAlaysisInfo);

	/**
	@brief Verify Dimension information using X , Y, Z plane
	@param prodSearchOption [in]Production information search condition
	@param strPjtName [in]Project name
	@param strMngType [in]Dimension type—(see below)
	@param strStandardPlane [in]Standard plane—(see below)
	@param strDirection [in]Plane direction—(see below)
	@param dblStandardPos [in]Plane position
	@param arrAnalysisInfo [out]Dimension information
	@return Error code
	@remark
	The strMngType parameters is the following\r\n
	"MNG_ALL" - The value is all dimension\r\n
	"MNG_POINT" - The value is dimension for management point\r\n
	"MNG_LENGTH" - The value is dimension for management length\r\n
	"MNG_PERP" - The value is dimension for management perpendicular\r\n
	"MNG_ANGLE" - The value is dimension for management angle\r\n\r\n
	The strStandardPlane parameters is the following\r\n
	"PLANE_X" - The value is get analysis information based on the X-plane\r\n
	"PLANE_Y" - The value is get analysis information based on the Y-plane\r\n
	"PLANE_Z" - The value is get analysis information based on the Z-plane\r\n\r\n
	The strDirection parameters is the following\r\n
	"+" - The value is get analysis information for plus direction of the strStandardPlane parameters\r\n
	"-" - The value is get analysis information for minus direction of the strStandardPlane parameters\r\n
	*/
	LONG SIS_GetAnalysisInfoByPlane(SISProductionSearchOption prodSearchOption, CString strPjtName, CString strMngType,
		CString strStandardPlane, CString strDirection, double dblStandardPos, CArray<SISAnalysisInfo, SISAnalysisInfo&>& arrAlaysisInfo);

	/**
	@brief Verify Dimension information using X , Y, Z plane
	@param prodSearchOption [in]Production information search condition
	@param pjtSearchOption [in]Project search condition
	@param strMngType [in]Dimension type—(see below)
	@param strStandardPlane [in]Standard plane—(see below)
	@param strDirection [in]Plane direction—(see below)
	@param dblStandardPos [in]Plane position
	@param arrAnalysisInfo [out]Dimension information
	@return Error code
	@remark
	The strMngType parameters is the following\r\n
	"MNG_ALL" - The value is all dimension\r\n
	"MNG_POINT" - The value is dimension for management point\r\n
	"MNG_LENGTH" - The value is dimension for management length\r\n
	"MNG_PERP" - The value is dimension for management perpendicular\r\n
	"MNG_ANGLE" - The value is dimension for management angle\r\n\r\n
	The strStandardPlane parameters is the following\r\n
	"PLANE_X" - The value is get analysis information based on the X-plane\r\n
	"PLANE_Y" - The value is get analysis information based on the Y-plane\r\n
	"PLANE_Z" - The value is get analysis information based on the Z-plane\r\n\r\n
	The strDirection parameters is the following\r\n
	"+" - The value is get analysis information for plus direction of the strStandardPlane parameters\r\n
	"-" - The value is get analysis information for minus direction of the strStandardPlane parameters\r\n
	*/
	LONG SIS_GetAnalysisInfoByPlane(SISProductionSearchOption prodSearchOption, SISProjectSearchOption pjtSearchOption, CString strMngType,
		CString strStandardPlane, CString strDirection, double dblStandardPos, CArray<SISAnalysisInfo, SISAnalysisInfo&>& arrAlaysisInfo);

	/**
	@brief Verify Dimension information using X , Y, Z plane
	@param shipNoInfo [in]Ship number
	@param strAssemLevel [in]Assembly level
	@param strAssemBlock [in]Assembly block
	@param strVarGroup [in]Variable group
	@param bIncNoneVarGropPjt [in]Include none variable group project or not
	@param strPjtName [in]Project name
	@param strMngType [in]Dimension type—(see below)
	@param strStandardPlane [in]Standard plane—(see below)
	@param strDirection [in]Plane direction—(see below)
	@param strStandardPos [in]Plane position of the string form(Able FR, LP form)
	@param arrAnalysisInfo [out]Dimension information
	@return Error code
	@remark
	The strMngType parameters is the following\r\n
	"MNG_ALL" - The value is all dimension\r\n
	"MNG_POINT" - The value is dimension for management point\r\n
	"MNG_LENGTH" - The value is dimension for management length\r\n
	"MNG_PERP" - The value is dimension for management perpendicular\r\n
	"MNG_ANGLE" - The value is dimension for management angle\r\n\r\n
	The strStandardPlane parameters is the following\r\n
	"PLANE_X" - The value is get analysis information based on the X-plane\r\n
	"PLANE_Y" - The value is get analysis information based on the Y-plane\r\n
	"PLANE_Z" - The value is get analysis information based on the Z-plane\r\n\r\n
	The strDirection parameters is the following\r\n
	"+" - The value is get analysis information for plus direction of the strStandardPlane parameters\r\n
	"-" - The value is get analysis information for minus direction of the strStandardPlane parameters\r\n
	*/
	LONG SIS_GetAnalysisInfoByPlane(SISShipNoInfo shipNoInfo, CString strAssemLevel, CString strAssemBlock, CString strVarGroup, BOOL bIncNoneVarGropPjt, CString strPjtName, CString strMngType,
		CString strStandardPlane, CString strDirection, CString strStandardPos, CArray<SISAnalysisInfo, SISAnalysisInfo&>& arrAlaysisInfo);

	/**
	@brief Verify Dimension information using X , Y, Z plane
	@param prodSearchOption [in]Production information search condition
	@param strPjtName [in]Project name
	@param strMngType [in]Dimension type—(see below)
	@param strStandardPlane [in]Standard plane—(see below)
	@param strDirection [in]Plane direction—(see below)
	@param strStandardPos [in]Plane position of the string form(Able FR, LP form)
	@param arrAnalysisInfo [out]Dimension information
	@return Error code
	@remark
	The strMngType parameters is the following\r\n
	"MNG_ALL" - The value is all dimension\r\n
	"MNG_POINT" - The value is dimension for management point\r\n
	"MNG_LENGTH" - The value is dimension for management length\r\n
	"MNG_PERP" - The value is dimension for management perpendicular\r\n
	"MNG_ANGLE" - The value is dimension for management angle\r\n\r\n
	The strStandardPlane parameters is the following\r\n
	"PLANE_X" - The value is get analysis information based on the X-plane\r\n
	"PLANE_Y" - The value is get analysis information based on the Y-plane\r\n
	"PLANE_Z" - The value is get analysis information based on the Z-plane\r\n\r\n
	The strDirection parameters is the following\r\n
	"+" - The value is get analysis information for plus direction of the strStandardPlane parameters\r\n
	"-" - The value is get analysis information for minus direction of the strStandardPlane parameters\r\n
	*/
	LONG SIS_GetAnalysisInfoByPlane(SISProductionSearchOption prodSearchOption, CString strPjtName, CString strMngType,
		CString strStandardPlane, CString strDirection, CString strStandardPos, CArray<SISAnalysisInfo, SISAnalysisInfo&>& arrAlaysisInfo);

	/**
	@brief Verify Dimension information using X , Y, Z plane
	@param prodSearchOption [in]Production information search condition
	@param pjtSearchOption [in]Project search condition
	@param strMngType [in]Dimension type—(see below)
	@param strStandardPlane [in]Standard plane—(see below)
	@param strDirection [in]Plane direction—(see below)
	@param strStandardPos [in]Plane position of the string form(Able FR, LP form)
	@param arrAnalysisInfo [out]Dimension information
	@return Error code
	@remark
	The strMngType parameters is the following\r\n
	"MNG_ALL" - The value is all dimension\r\n
	"MNG_POINT" - The value is dimension for management point\r\n
	"MNG_LENGTH" - The value is dimension for management length\r\n
	"MNG_PERP" - The value is dimension for management perpendicular\r\n
	"MNG_ANGLE" - The value is dimension for management angle\r\n\r\n
	The strStandardPlane parameters is the following\r\n
	"PLANE_X" - The value is get analysis information based on the X-plane\r\n
	"PLANE_Y" - The value is get analysis information based on the Y-plane\r\n
	"PLANE_Z" - The value is get analysis information based on the Z-plane\r\n\r\n
	The strDirection parameters is the following\r\n
	"+" - The value is get analysis information for plus direction of the strStandardPlane parameters\r\n
	"-" - The value is get analysis information for minus direction of the strStandardPlane parameters\r\n
	*/
	LONG SIS_GetAnalysisInfoByPlane(SISProductionSearchOption prodSearchOption, SISProjectSearchOption pjtSearchOption, CString strMngType,
		CString strStandardPlane, CString strDirection, CString strStandardPos, CArray<SISAnalysisInfo, SISAnalysisInfo&>& arrAlaysisInfo);

//파일저장
public:
	/**
	@brief Verify exist presence or absence of 2D draft
	@param strGroup [in]Group of ship number
	@param strShipNo [in]Ship number
	@param nProjectID [in]Project ID
	@param bExist [out]The presence or absence
	@return Error code
	@remark
	*/
	LONG SIS_ExistDraftFile(CString strGroup, CString strShipNo, UINT nProjectID, BOOL& bExist);

	/**
	@brief Save 2D draft file
	@param strGroup [in]Group of ship number
	@param strShipNo [in]Ship number
	@param nProjectID [in]Project ID
	@param strSavePath [in]Save path
	@return Error code
	@remark 2D draft file name is automatically given
	*/
	LONG SIS_SaveDraftFile(CString strGroup, CString strShipNo, UINT nProjectID, CString strSavePath);

	/**
	@brief Save file of work property
	@param strGroup [in]Group of ship number
	@param strShipNo [in]Ship number
	@param nFileID [in]File ID
	@param strSavePath [in]Save path
	@return Error code
	@remark
	*/
	LONG SIS_SaveFileOfWorkProp(CString strGroup, CString strShipNo, UINT nFileID, CString strSavePath);


public:
	/**
	@brief Register user
	@param userInfo [in]User information
	@return Error code
	@remark
	*/
	LONG SIS_RegisterUser(SISUserInfo userInfo);

	/**
	@brief Delete user
	@param strUserID [in]User ID
	@return Error code
	@remark
	*/
	LONG SIS_DeleteUser(CString strUserID);

	/**
	@brief Update user information
	@param userInfo [in]User information
	@return Error code
	@remark
	*/
	LONG SIS_UpdateUserInfo(SISUserInfo userInfo);

	/**
	@brief Approval user login
	@param strUserID [in]User ID
	@param bApproval [in]Approval presence or absence
	@return Error code
	@remark
	*/
	LONG SIS_ApprovalLoginUser(CString strUserID, BOOL bApproval);

	/**
	@brief Approval administrator
	@param strUserID [in]User ID
	@param bApproval [in]Administrator presence or absence
	@return Error code
	@remark
	*/
	LONG SIS_ApprovalAdminUser(CString strUserID, BOOL bApproval);

	/**
	@brief Set user alarm
	@param strUserID [in]User ID
	@param strWorkflow [in]Workflow name
	@param strStatus [in]Status name
	@param alamInfo [in]Alarm information
	@return Error code
	@remark
	*/
	LONG SIS_SetUserAlarm(CString strUserID, CString strWorkflow, CString strStatus, SISAlarmInfo alamInfo);

	/**
	@brief Create permission group
	@param strPermissionGroup [in]Permission group name
	@return Error code
	@remark
	*/
	//LONG SIS_CreatePermissionGroup(CString strPermissionGroup);

	/**
	@brief Set permission
	@param strPermissionGroup [in]Permission group name
	@param strStatus [in]Status name
	@param permission [in]Permission information
	@return Error code
	@remark
	*/
	//status(메뉴)별로 권한부여한 그룹 설정
	//LONG SIS_SetPermission(CString strPermissionGroup, CString strStatus, SISPermissionInfo permission);

	/**
	@brief Remove permission group
	@param strPermissionGroup [in]Permission group name
	@return Error code
	@remark
	*/
	//LONG SIS_RemovePermissionGroup(CString strPermissionGroup);

	/**
	@brief Set user to permission group
	@param strPermissionGroup [in]Permission group name
	@param strUserID [in]User ID
	@return Error code
	@remark
	*/
	//status(메뉴)별로 부여한 권한그룹에 사용자 설정
	//LONG SIS_SetUserToPermissionGroup(CString strPermissionGroup, CString strUserID); //한명씩

	/**
	@brief Set user to permission group
	@param strPermissionGroup [in]Permission group name
	@param arrUserID [in]User ID list
	@return Error code
	@remark
	*/
	//LONG SIS_SetUserToPermissionGroup(CString strPermissionGroup, CArray<CString, CString&>& arrUserID); //여러명씩

	/**
	@brief Remove user from permission group
	@param strPermissionGroup [in]Permission group name
	@param strUserID [in]User ID
	@return Error code
	@remark
	*/
	//LONG SIS_RemoveUserFromPermissionGroup(CString strPermissionGroup, CString strUserID); //한명씩

	/**
	@brief Remove user from permission group
	@param strPermissionGroup [in]Permission group name
	@param arrUserID [in]User ID list
	@return Error code
	@remark
	*/
	//LONG SIS_RemoveUserFromPermissionGroup(CString strPermissionGroup, CArray<CString, CString&>& arrUserID); //여러명씩

	/**
	@brief Set user define permission
	@param strUserID [in]User ID
	@param strWorkflow [in]Workflow name
	@param strStatus [in]Status name
	@param permission [in]Permission information
	@return Error code
	@remark
	*/
	//한 User에 사용자가 정의한 권한을 status별로 설정 함.(status(메뉴)별 User 설정 후 사용)
	LONG SIS_SetUserDefinePermission(CString strUserID, CString strWorkflow, CString strStatus, SISPermissionInfo permission);

	/**
	@brief Set user define permission
	@param strUserID [in]User ID
	@param strWorkflow [in]Workflow name
	@param arrStatusPermi [in]Status Permission list
	@return Error code
	@remark
	*/
	LONG SIS_SetUserDefinePermission(CString strUserID, CString strWorkflow, CArray<SISStatusPermission, SISStatusPermission&>& arrStatusPermi);

	/**
	@brief Set use user defined permission(TRUE) or use permission group(FALSE)
	@param strUserID [in]User ID
	@param bUse [in]Use user defined permission(TRUE) or use permission group(FALSE)
	@return Error code
	@remark
	*/
	LONG SIS_SetUseUserDefinePermission(CString strUserID, BOOL bUse);

	/**
	@brief Set notification count
	@param strUserID [in]User ID
	@param nCnt [in]Notification count
	@return Error code
	@remark
	*/
	//공지 개수
	LONG SIS_SetNotificationCnt(CString strUserID, UINT nCnt);

	/**
	@brief Change notification state
	@param nNotificationID [in]Notification ID
	@param state [in]Notification state
	@return Error code
	@remark
	*/
	LONG SIS_SetNotificationState(UINT nNotificationID, NotificationState state);

	/**
	@brief Send notification
	@param strData [in]Notification content
	@param arrTargerUserID [in]Receive user id
	@return Error code
	@remark
	*/
	LONG SIS_SetNotification(CString strData, CArray<CString, CString&>& arrTargerUserID);

	/**
	@brief Set user affiliation information[COMPANY, DEPARTMENT, POSITION, ROLE]
	@param userAffiliationType [in]User affiliation type
	@param strName [in]Name
	@return Error code
	@remark
	*/
	//회사, 부서, 직책, Role
	LONG SIS_SetUserAffiliationInfo(UserAffiliation userAffiliationType, CString strName);

	/**
	@brief Remove user affiliation information[COMPANY, DEPARTMENT, POSITION, ROLE]
	@param userAffiliationType [in]User affiliation type
	@param strName [in]Name
	@return Error code
	@remark
	*/
	LONG SIS_RemoveUserAffiliationInfo(UserAffiliation userAffiliationType, CString strName);

	/**
	@brief Set status
	@param strStatus [in]Status name
	@return Error code
	@remark
	*/
	//status(메뉴) 설정
	LONG SIS_SetStatus(CString strStatus);

	/**
	@brief Remove status
	@param strStatus [in]Status name
	@return Error code
	@remark
	*/
	LONG SIS_RemoveStatus(CString strStatus);

	/**
	@brief Set work time at each status
	@param strStatus [in]Status name
	@param startTime [in]Work start time
	@param endTime [in]Work end time
	@return Error code
	@remark
	*/
	//status(메뉴) 별 업무시간 설정
	LONG SIS_SetWorkTime(CString strStatus, SISTime startTime, SISTime endTime);
	
	/**
	@brief Set assign change time at each status
	@param strStatus [in]Status name
	@param timeSpan [in]Time span
	@return Error code
	@remark
	*/
	//status(메뉴) 별 Assign 변경시간 설정
	LONG SIS_SetAssignChangeTimeSpan(CString strStatus, SISTime timeSpan);

	/**
	@brief Set field at each status
	@param strStatus [in]Status name
	@param arrField [in]Field list
	@return Error code
	@remark
	*/
	//status(메뉴)별 필드(변수) 설정
	LONG SIS_SetFieldOfStatus(CString strStatus, CArray<CString, CString&>& arrField); //전체를 한번에 추가

	/**
	@brief Set field at each status
	@param strStatus [in]Status name
	@param strField [in]Field name
	@return Error code
	@remark
	*/
	LONG SIS_SetFieldOfStatus(CString strStatus, CString strField); //하나하나를 순서대로 추가

	/**
	@brief Set property to field of status
	@param strStatus [in]Status name
	@param strField [in]Field name
	@param bEssential [in]Essential presence or absence
	@return Error code
	@remark
	*/
	LONG SIS_SetFieldOfStatusProp(CString strStatus, CString strField, BOOL bEssential);

	/**
	@brief Remove field at each status
	@param strStatus [in]Status name
	@param strField [in]Field name
	@return Error code
	@remark
	*/
	LONG SIS_RemoveFieldOfStatus(CString strStatus, CString strField);

	///**
	//@brief Set user at each status
	//@param strWorkflow [in]Workflow name
	//@param strStatus [in]Status name
	//@param strUserID [in]User ID
	//@return Error code
	//@remark
	//*/
	////status(메뉴)별 User 설정(+알람정보 set)
	//LONG SIS_SetUserToStatus(CString strWorkflow, CString strStatus, CString strUserID);

	///**
	//@brief Remove user at each status
	//@param strWorkflow [in]Workflow name
	//@param strStatus [in]Status name
	//@param strUserID [in]User ID
	//@return Error code
	//@remark
	//*/
	//LONG SIS_RemoveUserFromStatus(CString strWorkflow, CString strStatus, CString strUserID);

	///**
	//@brief Get user from status
	//@param strWorkflow [in]Workflow name
	//@param strStatus [in]Status name
	//@param strUserID [out]User ID list
	//@return Error code
	//@remark
	//*/
	//LONG SIS_GetUserFromStatus(CString strWorkflow, CString strStatus, CArray<CString, CString&>& strUserID);

	/**
	@brief Set field
	@param fieldInfo [in]field information
	@return Error code
	@remark
	*/
	//필드(변수) 설정
	LONG SIS_SetField(SISFieldInfo fieldInfo);

	/**
	@brief Remove field
	@param strField [in]field name
	@return Error code
	@remark
	*/
	LONG SIS_RemoveField(CString strField);

	/**
	@brief Set field type
	@param fieldType [in]Field type
	@return Error code
	@remark
	*/
	//사용자가 정의하는..필드(변수) 타입 설정
	LONG SIS_SetFieldType(SISFieldType fieldType);
	
	/**
	@brief Remove field type
	@param strFieldType [in]Field type name
	@return Error code
	@remark
	*/
	//사용자가 정의한 필드(변수) 타입 중에서만 삭제가능
	LONG SIS_RemoveFieldType(CString strFieldType);

	/**
	@brief Set work flow
	@param workFlow [in]Work flow
	@return Error code
	@remark
	*/
	//Work flow 설정
	LONG SIS_SetWorkFlow(SISWorkFlow workFlow); //전체를 한번에 설정

	/**
	@brief Remove work flow
	@param strWorkflow [in]Workflow name
	@return Error code
	@remark
	*/
	LONG SIS_RemoveWorkFlow(CString strWorkflow);

	/**
	@brief Set next work
	@param strGroup [in]Group of ship number
	@param strShipNo [in]Ship number
	@param strAssemLevel [in]Assembly level name
	@param strAssemBlock [in]Block name
	@param nWorkflowID [in]Workflow ID
	@param nWorkID [in]Work ID - If first set input zero
	@param strNextStatus [in]Next status name
	@param assign [in]Assign information
	@param arrFieldValue [in]Field value list
	@return Error code
	@remark
	*/
	LONG SIS_SetNextWork(CString strGroup, CString strShipNo, CString strAssemLevel, CString strAssemBlock, UINT nWorkflowID,
						UINT nWorkID, CString strNextStatus, SISAssignUser assign, CArray<SISFieldValue, SISFieldValue&>& arrFieldValue);

	/**
	@brief Set work property
	@param strGroup [in]Group of ship number
	@param strShipNo [in]Ship number
	@param nWorkID [in]Work ID
	@param arrFieldValue [in]Field value list(Exclude file and multifile type)
	@return Error code
	@remark
	*/
	LONG SIS_SetWorkPropertyExcludeFile(CString strGroup, CString strShipNo, UINT nWorkID, CArray<SISFieldValue, SISFieldValue&>& arrFieldValue);

	/**
	@brief Set work file property
	@param strGroup [in]Group of ship number
	@param strShipNo [in]Ship number
	@param nWorkID [in]Work ID
	@param fieldValue [in]Field value(Only file type)
	@return Error code
	@remark
	*/
	LONG SIS_SetWorkFileProperty(CString strGroup, CString strShipNo, UINT nWorkID, SISFieldValue fieldValue);

	/**
	@brief Delete work file property
	@param strGroup [in]Group of ship number
	@param strShipNo [in]Ship number
	@param nWorkID [in]Work ID
	@param fileIndex [in]File index
	@return Error code
	@remark
	*/
	LONG SIS_DeleteWorkFileProperty(CString strGroup, CString strShipNo, UINT nWorkID, UINT fileIndex);

	/**
	@brief Add work multifile property
	@param strGroup [in]Group of ship number
	@param strShipNo [in]Ship number
	@param nWorkID [in]Work ID
	@param arrFieldValue [in]Field value list(Only multifile type)
	@return Error code
	@remark
	*/
	LONG SIS_AddWorkMultiFileProperty(CString strGroup, CString strShipNo, UINT nWorkID, CArray<SISFieldValue, SISFieldValue&>& arrFieldValue);

	/**
	@brief Delete work multifile property
	@param strGroup [in]Group of ship number
	@param strShipNo [in]Ship number
	@param nWorkID [in]Work ID
	@param arrFileIndex [in]File index list
	@return Error code
	@remark
	*/
	LONG SIS_DeleteWorkMultiFileProperty(CString strGroup, CString strShipNo, UINT nWorkID, CArray<UINT, UINT&>& arrFileIndex);

	/**
	@brief Remove work
	@param strGroup [in]Group of ship number
	@param strShipNo [in]Ship number
	@param nWorkID [in]Work ID
	@return Error code
	@remark
	*/
	LONG SIS_RemoveWork(CString strGroup, CString strShipNo, UINT nWorkID);

	/**
	@brief Modify assign of work
	@param strGroup [in]Group of ship number
	@param strShipNo [in]Ship number
	@param nWorkID [in]Work ID
	@param assign [in]Assign information
	@return Error code
	@remark
	*/
	//Assign변경
	LONG SIS_ModifyAssign(CString strGroup, CString strShipNo, UINT nWorkID, SISAssignUser assign);

	/**
	@brief Set next assign
	@param strGroup [in]Group of ship number
	@param strShipNo [in]Ship number
	@param nWorkID [in]Work ID
	@param strToUserID [in]User ID
	@return Error code
	@remark
	*/
	//Assign history기록(각 work flow 단계별 assing histroy 관리)
	LONG SIS_SetNextAssign(CString strGroup, CString strShipNo, UINT nWorkID, CString strToUserID); //한 work에 대해 다음 Assign으로 변경

	/**
	@brief Set next assign
	@param strGroup [in]Group of ship number
	@param strShipNo [in]Ship number
	@return Error code
	@remark Set next assign for assign auto change of work
	*/
	//Assign자동변경이 진행중인 work에 대해 다음 Assign으로 변경
	LONG SIS_SetNextAssign(CString strGroup, CString strShipNo);

	/**
	@brief Set assign confirm
	@param strGroup [in]Group of ship number
	@param strShipNo [in]Ship number
	@param nWorkID [in]Work ID
	@param bConfirm [in]Confirm presence or absence
	@return Error code
	@remark 
	*/
	LONG SIS_SetAssignConfirm(CString strGroup, CString strShipNo, UINT nWorkID, BOOL bConfirm); //Assign confirm(+Assign자동변경 중지)

public:
	/**
	@brief Get user information
	@param strUserID [in]User ID
	@param userInfo [out]User information
	@return Error code
	@remark
	*/
	LONG SIS_GetUser(CString strUserID, SISUserInfo& userInfo);

	/**
	@brief Get user defined permission
	@param strUserID [in]User name
	@param strWorkflow [in]Workflow name
	@param strStatus [in]Status name
	@param statusPermission [out]User permission
	@return Error code
	@remark
	*/
	//한 User에 사용자가 정의한 권한을 status별로 설정 함.(status(메뉴)별 User 설정 후 사용)
	LONG SIS_GetUserDefinePermission(CString strUserID, CString strWorkflow, CString strStatus, SISPermissionInfo& permiGroup);

	/**
	@brief Get user defined permission
	@param strUserID [in]User name
	@param strWorkflow [in]Workflow name
	@param arrStatusPermi [out]Status permission list
	@return Error code
	@remark
	*/
	LONG SIS_GetUserDefinePermission(CString strUserID, CString strWorkflow, CArray<SISStatusPermission, SISStatusPermission&>& arrStatusPermi);

	/**
	@brief Get user defined permission
	@param strUserID [in]User name
	@param arrUserDefinePermi [out]User defined permission list
	@return Error code
	@remark
	*/
	LONG SIS_GetUserDefinePermission(CString strUserID, CArray<SISUserDefinePermission, SISUserDefinePermission&>& arrUserDefinePermi);

	/**
	@brief Get Using user defined permission(TRUE) or Using permission group(FALSE)
	@param strUserID [in]User ID
	@param bUse [out]Using user defined permission(TRUE) or Using permission group(FALSE)
	@return Error code
	@remark
	*/
	LONG SIS_GetUseUserDefinePermission(CString strUserID, BOOL& bUse);

	/**
	@brief Get user alarm
	@param strUserID [in]User ID
	@param strWorkflow [in]Workflow ID
	@param strStatus [in]Status name
	@param userAlarm [out]Alam information
	@return Error code
	@remark
	*/
	//user 알람설정 정보
	LONG SIS_GetUserAlarm(CString strUserID, CString strWorkflow, CString strStatus, SISUserAlarm& userAlarm);

	/**
	@brief Get user alarm by status
	@param strWorkflow [in]Workflow ID
	@param strStatus [in]Status name
	@param arrUserAlarm [out]Alam information
	@return Error code
	@remark
	*/
	LONG SIS_GetUserAlarmByStatus(CString strWorkflow, CString strStatus, CArray<SISUserAlarm, SISUserAlarm&>& arrUserAlarm);

	/**
	@brief Get user alarm by user ID
	@param strUserID [in]User ID
	@param userAlarm [out]Alam information
	@return Error code
	@remark
	*/
	LONG SIS_GetUserAlarmByUserID(CString strUserID, SISUserAlarm& userAlarm);

	/**
	@brief Get notification count
	@param strUserID [in] User ID
	@param nCnt [out]Notification count
	@return Error code
	@remark
	*/
	//공지 개수
	LONG SIS_GetNotificationCnt(CString strUserID, UINT& nCnt);

	/**
	@brief Get notification
	@param strUserID [in]User ID
	@param arrNotification [out]Notification information list
	@return Error code
	@remark
	*/
	//사용자별 공지정보 Notification Cnt 만큼만 가져옴.
	LONG SIS_GetNotification(CString strUserID, CArray<SISNotification, SISNotification&>& arrNotification);

	/**
	@brief Get user affiliation information[COMPANY, DEPARTMENT, POSITION, ROLE]
	@param userAffiliationType [in]Affiliation type
	@param arrName [out]Value list
	@return Error code
	@remark
	*/
	LONG SIS_GetUserAffiliationInfo(UserAffiliation userAffiliationType, CArray<CString, CString&>& arrName);

	/**
	@brief Get status
	@param arrStatus [out]Status name list
	@return Error code
	@remark
	*/
	//status(메뉴) 설정
	LONG SIS_GetStatus(CArray<CString, CString&>& arrStatus);

	/**
	@brief Get status in workflow
	@param strWorkflow [in]Workflow name
	@param arrStatus [out]Status name list
	@return Error code
	@remark
	*/
	LONG SIS_GetStatus(CString strWorkflow, CArray<CString, CString&>& arrStatus);

	/**
	@brief Get work time
	@param strStatus [in]Status name
	@param startTime [out]Work start time
	@param endTime [out]Work end time
	@return Error code
	@remark
	*/
	//status(메뉴) 별 업무시간 설정
	LONG SIS_GetWorkTime(CString strStatus, SISTime& startTime, SISTime& endTime);

	/**
	@brief Get assign change time
	@param strStatus [in]Status name
	@param timeSpan [out]Time span
	@return Error code
	@remark
	*/
	//status(메뉴) 별 Assign 변경시간 설정
	LONG SIS_GetAssignChangeTimeSpan(CString strStatus, SISTime& timeSpan);

	/**
	@brief Get field information of status
	@param strStatus [in]Status name
	@param arrFieldInfo [out]Field information list
	@return Error code
	@remark
	*/
	//status(메뉴)별 필드(변수) 설정
	LONG SIS_GetFieldOfStatus(CString strStatus, CArray<SISFieldInfo, SISFieldInfo&>& arrFieldInfo);

	/**
	@brief Get field information
	@param arrFieldInfo [out]Field information list
	@return Error code
	@remark
	*/
	//필드(변수) 설정
	LONG SIS_GetField(CArray<SISFieldInfo, SISFieldInfo&>& arrFieldInfo);

	/**
	@brief Get field type
	@param arrFieldType [out]Field type list
	@return Error code
	@remark
	*/
	//필드(변수) 타입 설정
	LONG SIS_GetFieldType(CArray<SISFieldType, SISFieldType&>& arrFieldType);

	/**
	@brief Get work flow names
	@param arrWorkFlowNames [out]Work flow name list
	@return Error code
	@remark
	*/
	LONG SIS_GetWorkFlowName(CArray<CString, CString&>& arrWorkFlowNames);

	/**
	@brief Get work flow
	@param arrWorkFlow [out]Work flow list
	@return Error code
	@remark
	*/
	LONG SIS_GetWorkFlow(CArray<SISWorkFlow, SISWorkFlow&>& arrWorkFlow);

	/**
	@brief Get work flow
	@param workFlow [out]Work flow
	@return Error code
	@remark
	*/
	LONG SIS_GetWorkFlow(CString strWorkflowName, SISWorkFlow& workFlow);
	
	/**
	@brief Get work history
	@param strGroup [in]Group of ship number
	@param strShipNo [in]Ship number
	@param nIssueID [in]Issue ID
	@param workHistory [out]Work history
	@return Error code
	@remark
	*/
	//한 이슈에 대한 Work history
	LONG SIS_GetWorkHistory(CString strGroup, CString strShipNo, UINT nIssueID, SISWorkHistory& workHistory);

	/**
	@brief Get works
	@param strGroup [in]Group of ship number
	@param strShipNo [in]Ship number
	@param strStatus [in]Status name
	@param arrWork [out]Work list
	@return Error code
	@remark
	*/
	//입력한 status와 history의 마지막 work가 동일한거 반환
	LONG SIS_GetCurrentWork(CString strGroup, CString strShipNo, CString strStatus, CArray<SISWork, SISWork&>& arrWork);

	/**
	@brief Get works
	@param strGroup [in]Group of ship number
	@param strShipNo [in]Ship number
	@param strAssemLevel [in]Assembly level
	@param strStatus [in]Status name
	@param arrWork [out]Work list
	@return Error code
	@remark
	*/
	LONG SIS_GetCurrentWork(CString strGroup, CString strShipNo, CString strAssemLevel, CString strStatus, CArray<SISWork, SISWork&>& arrWork);

	/**
	@brief Get works
	@param strGroup [in]Group of ship number
	@param strShipNo [in]Ship number
	@param strAssemLevel [in]Assembly level
	@param strAssemBlock [in]Assembly block
	@param strStatus [in]Status name
	@param arrWork [out]Work list
	@return Error code
	@remark
	*/
	LONG SIS_GetCurrentWork(CString strGroup, CString strShipNo, CString strAssemLevel, CString strAssemBlock, CString strStatus, CArray<SISWork, SISWork&>& arrWork);

	/**
	@brief Get works
	@param strGroup [in]Group of ship number
	@param strShipNo [in]Ship number
	@param strAssemLevel [in]Assembly level or NULL string
	@param strAssemBlock [in]Assembly block or NULL string
	@param strWorkflow [in]Workflow name or NULL string
	@param strStatus [in]Status name or NULL string
	@param arrWork [out]Work list
	@return Error code
	@remark
	*/
	LONG SIS_GetCurrentWork(CString strGroup, CString strShipNo, CString strAssemLevel, CString strAssemBlock, CString strWorkflow, CString strStatus, CArray<SISWork, SISWork&>& arrWork);

	/**
	@brief Get property of work
	@param strGroup [in]Group of ship number
	@param strShipNo [in]Ship number
	@param nWorkID [in]Work ID
	@param arrFieldValue [out]Property list
	@return Error code
	@remark
	*/
	LONG SIS_GetWorkProperty(CString strGroup, CString strShipNo, UINT nWorkID, CArray<SISFieldValue, SISFieldValue&>& arrFieldValue);

	/**
	@brief Get assign history
	@param strGroup [in]Group of ship number
	@param strShipNo [in]Ship number
	@param nWorkID [in]Work ID
	@param assignHistory [out]Assign history
	@return Error code
	@remark
	*/
	//Assign history(각 work flow 단계별 assing histroy 관리)
	LONG SIS_GetAssignHistroy(CString strGroup, CString strShipNo, UINT nWorkID, SISAssignHistory& assignHistory);

	/**
	@brief Get assign information
	@param strGroup [in]Group of ship number
	@param strShipNo [in]Ship number
	@param nWorkID [in]Work ID
	@param assignInfo [out]Assign information
	@return Error code
	@remark
	*/
	//각 work flow의 현재 assing
	LONG SIS_GetAssign(CString strGroup, CString strShipNo, UINT nWorkID, SISAssign& assignInfo);

	/**
	@brief Get assign confirm 
	@param strGroup [in]Group of ship number
	@param strShipNo [in]Ship number
	@param nWorkID [in]Work ID
	@param bConfirm [out]Confirm presence or absence
	@return Error code
	@remark
	*/
	//Assign confirm(+Assign자동변경 중지)
	LONG SIS_GetAssignConfirm(CString strGroup, CString strShipNo, UINT nWorkID, BOOL& bConfirm);

	/**
	@brief Set block permission
	@param shipNoInfo [in]Ship number
	@param strBlockName [in]Block name
	@param userAffiliationType [in]Affiliation type
	@param strAffiliationName [in]Affiliation name
	@param bSubBlockPermi [in]Have permission of sub block(TRUE) or Have not permission of sub block(FALSE)
	@return Error code
	@remark
	*/
	LONG SIS_SetBlockPermission(SISShipNoInfo shipNoInfo, CString strBlockName, UserAffiliation userAffiliationType, CString strAffiliationName, BOOL bSubBlockPermi);

	/**
	@brief Get block permission
	@param shipNoInfo [in]Ship number
	@param strBlockName [in]Block name
	@param arrBlockPermi [out]Block permission list
	@return Error code
	@remark
	*/
	LONG SIS_GetBlockPermission(SISShipNoInfo shipNoInfo, CString strBlockName, CArray<SISBlockPermission, SISBlockPermission&>& arrBlockPermi);

	/**
	@brief Get permissioned block
	@param shipNoInfo [in]Ship number
	@param userAffiliationType [in]Affiliation type
	@param strAffiliationName [in]Affiliation name
	@param arrAssemBlockInfo [out]Block information list
	@return Error code
	@remark
	*/
	LONG SIS_GetPermissionedBlock(SISShipNoInfo shipNoInfo, UserAffiliation userAffiliationType, CString strAffiliationName, CArray<SISAssemBlockInfo, SISAssemBlockInfo&>& arrAssemBlockInfo);

	/**
	@brief Verify exist presence or absence of block permission
	@param shipNoInfo [in]Ship number
	@param userAffiliationType [in]Affiliation type
	@param strAffiliationName [in]Affiliation name
	@param bExist [out]The presence or absence
	@return Error code
	@remark
	*/
	LONG SIS_ExistBlockPermission(SISShipNoInfo shipNoInfo, UserAffiliation userAffiliationType, CString strAffiliationName, BOOL& bExist);

	/**
	@brief Remove block permission
	@param shipNoInfo [in]Ship number
	@param strBlockName [in]Block name
	@param userAffiliationType [in]Affiliation type
	@param strAffiliationName [in]Affiliation name
	@return Error code
	@remark
	*/
	LONG SIS_RemoveBlockPermission(SISShipNoInfo shipNoInfo, CString strBlockName, UserAffiliation userAffiliationType, CString strAffiliationName);

	/**
	@brief Set ship number permission(Have permission for all blocks)
	@param shipNoType [in]Composition of ship number(Using 'MASTER', 'GROUP', 'SHIPNO')
	@param strGroup [in]Group of ship number
	@param strShipNo [in]Ship number or group
	@param userAffiliationType [in]Affiliation type
	@param strAffiliationName [in]Affiliation name
	@return Error code
	@remark
	The shipNoType parameters is the following\r\n
	AssemTreeComposition::ATC_MASTER - Set NULL to strGroup, strShipNo parameter
	AssemTreeComposition::ATC_GROUP - Set NULL to strShipNo parameter
	AssemTreeComposition::ATC_SHIPNO - Set NULL to strGroup parameter
	*/
	LONG SIS_SetShipNoPermission(AssemTreeComposition shipNoType, CString strGroup, CString strShipNo, UserAffiliation userAffiliationType, CString strAffiliationName);

	/**
	@brief Get ship number permission
	@param shipNoType [in]Composition of ship number(Using 'MASTER', 'GROUP', 'SHIPNO')
	@param strGroup [in]Group of ship number
	@param strShipNo [in]Ship number
	@param arrBlockPermi [out]Ship number or group permission list
	@return Error code
	@remark
	The shipNoType parameters is the following\r\n
	AssemTreeComposition::ATC_MASTER - Set NULL to strGroup, strShipNo parameter
	AssemTreeComposition::ATC_GROUP - Set NULL to strShipNo parameter
	AssemTreeComposition::ATC_SHIPNO - Set NULL to strGroup parameter
	*/
	LONG SIS_GetShipNoPermission(AssemTreeComposition shipNoType, CString strGroup, CString strShipNo, CArray<SISShipNoPermission, SISShipNoPermission&>& arrShipNoPermi);

	/**
	@brief Get permissioned ship nuber
	@param userAffiliationType [in]Affiliation type
	@param strAffiliationName [in]Affiliation name
	@param arrShipNoInfo [out]Ship number information list
	@return Error code
	@remark
	*/
	LONG SIS_GetPermissionedShipNo(UserAffiliation userAffiliationType, CString strAffiliationName, CArray<SISShipNoInfo, SISShipNoInfo&>& arrShipNoInfo);

	/**
	@brief Remove ship number permission
	@param shipNoType [in]Composition of ship number(Using 'MASTER', 'GROUP', 'SHIPNO')
	@param strGroup [in]Group of ship number
	@param strShipNo [in]Ship number
	@param userAffiliationType [in]Affiliation type
	@param strAffiliationName [in]Affiliation name
	@return Error code
	@remark
	The shipNoType parameters is the following\r\n
	AssemTreeComposition::ATC_MASTER - Set NULL to strGroup, strShipNo parameter
	AssemTreeComposition::ATC_GROUP - Set NULL to strShipNo parameter
	AssemTreeComposition::ATC_SHIPNO - Set NULL to strGroup parameter
	*/
	LONG SIS_RemoveShipNoPermission(AssemTreeComposition shipNoType, CString strGroup, CString strShipNo, UserAffiliation userAffiliationType, CString strAffiliationName);

	/**
	@brief Create permission group
	@param strPermissionGroup [in]Permission group name
	@return Error code
	@remark
	*/
	LONG SIS_CreatePermissionGroup(CString strPermissionGroup);

	/**
	@brief Set permission
	@param strPermissionGroup [in]Permission group name
	@param strWorkflow [in]Workflow name
	@param statusPermi [in]Permission information of status
	@return Error code
	@remark
	*/
	LONG SIS_SetPermission(CString strPermissionGroup, CString strWorkflow, CArray<SISStatusPermission, SISStatusPermission&>& arrStatusPermi);

	/**
	@brief Remove permission group
	@param strPermissionGroup [in]Permission group name
	@return Error code
	@remark
	*/
	LONG SIS_RemovePermissionGroup(CString strPermissionGroup);

	/**
	@brief Get permission group
	@param arrGroupName [out]Group name list
	@return Error code
	@remark
	*/
	//권한 그룹의 정보
	LONG SIS_GetPermissionGroup(CArray<CString, CString&>& arrGroupName);

	/**
	@brief Get permission information
	@param arrPermissionGroup [out]Permission group information list
	@return Error code
	@remark
	*/
	LONG SIS_GetPermission(CArray<SISPermissionGroup, SISPermissionGroup&>& arrPermissionGroup);

	/**
	@brief Get permission information
	@param strGroupName [in]Group name
	@param arrPermissionGroup [out]Permission group information list
	@return Error code
	@remark
	*/
	LONG SIS_GetPermission(CString strGroupName, SISPermissionGroup& permissionGroup);

	/**
	@brief Set permission group to user role
	@param strRoleName [in]User role
	@param strPermiGroup [in]Permission group name
	@return Error code
	@remark
	*/
	LONG SIS_SetRolePermission(CString strRoleName, CString strPermiGroup);

	/**
	@brief Get permission group for user role
	@param strRoleName [in]User role
	@param arrPermiGroup [out]Permission group name list
	@return Error code
	@remark
	*/
	LONG SIS_GetRolePermission(CString strRoleName, CArray<CString, CString&>& arrPermiGroup);

	/**
	@brief Remove permission group from user role
	@param strRoleName [in]User role
	@param strPermiGroup [in]Permission group name
	@return Error code
	@remark
	*/
	LONG SIS_RemoveRolePermission(CString strRoleName, CString strPermiGroup);

	/**
	@brief Upload image project
	@param strGroup [in]Group of ship number
	@param strShipNo [in]Ship number
	@param strAssemLevel [in]Assembly level name
	@param strAssemBlock [in]Block name
	@param nWorkID [in]Work ID
	@param strFilePathName [in]File path
	@return Error code
	@remark
	*/
	LONG SIS_SetImageProject(CString strGroup, CString strShipNo, CString strAssemLevel, CString strAssemBlock, UINT nWorkID, CString strFilePathName);
	
	/**
	@brief Download image project
	@param strGroup [in]Group of ship number
	@param strShipNo [in]Ship number
	@param strAssemLevel [in]Assembly level name
	@param strAssemBlock [in]Block name
	@param nWorkID [in]Work ID
	@param strSavePathName [in]File path
	@return Error code
	@remark
	*/
	LONG SIS_GetImageProject(CString strGroup, CString strShipNo, CString strAssemLevel, CString strAssemBlock, UINT nWorkID, CString strSavePathName);

	/**
	@brief Remove image project
	@param strGroup [in]Group of ship number
	@param strShipNo [in]Ship number
	@param strAssemLevel [in]Assembly level name
	@param strAssemBlock [in]Block name
	@param nWorkID [in]Work ID
	@return Error code
	@remark
	*/
	LONG SIS_RemoveImageProject(CString strGroup, CString strShipNo, CString strAssemLevel, CString strAssemBlock, UINT nWorkID);

	/**
	@brief Get program permission for one user
	@param strUserID [in]User ID
	@param programPermi [out]Program permission
	@return Error code
	@remark
	*/
	LONG SIS_GetProgramPermission(CString strUserID, SISProgramPermission& programPermi);

	/**
	@brief Get program permission for all user
	@param strUserID [in]User ID
	@param arrProgramPermi [out]Program permission list
	@return Error code
	@remark
	*/
	LONG SIS_GetProgramPermission(CArray<SISProgramPermission, SISProgramPermission&>& arrProgramPermi);

	/**
	@brief Set program permission for one user
	@param strUserID [in]User ID
	@param programPermi [in]Program permission
	@return Error code
	@remark
	*/
	LONG SIS_SetProgramPermission(CString strUserID, SISProgramPermission programPermi);
};

