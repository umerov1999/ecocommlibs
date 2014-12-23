#pragma once

#include <windows.h>
#include <io.h>

#include "EcoServerBinary.h"
#include "EcoSocketClientManager.h"
#include "EcoWebConnector.h"
#include "EcoWASDatabaseRecordSet.h"
#include "EcoDatabaseManagerDefine.h"

class AFX_EXT_API CEcoWASClient
{

public:
	CEcoWASClient();
	~CEcoWASClient(void);

private:
	CString m_strAppDataPath;

	CString	m_strUserID;
	CString	m_strPwd;
	CString m_strEcoWASUrl;
	CString m_strEcoTmpRootPath;
	int m_nSocketPort;

	BOOL m_bConnected;
	BOOL m_bExternConnect;

	CString m_strNetFolderUserID;
	CString m_strNetFolderPwd;
	CString m_strNetFolderUnc;	//이 변수가 사용되면 m_strEcoTmpRootPath와 같은 값이 들어감.
	HANDLE m_hNetFolderToken;

private:
	CEcoSocketClientManager m_ecoSocketClient;
	CEcoWebConnector m_ecoWebConnector;
public:
	CEcoServerBinary m_ecoServerBinary;

private:
	BOOL SendCommand(CEcoPacket& sendPacket, CEcoPacket& recvPacket);
	static UINT WebConnectThread(LPVOID pParam);

public:
	BOOL ConnectWAS();
	void DisConnectWAS(BOOL bMust = FALSE);
	void SetUserInfo(CString strUserID, CString strPwd = _T(""));
	void SetSocketPort(int nPort);
	void SetEcoWASUrl(CString strWASUrl);
	void SetEcoTmpRootPath(CString strPath);

public:
	BOOL GetNetFolderInfo();
	BOOL LogonNetFolder();
	void LogOffNetFolder();


public:
	//유틸
	//void Compress(CString lpcsSrcFile, CString lpcsDesFile, bool bCompress, int nBitsPerSample = 18);
	//int MakeRandVal();
	void MakeOleTimeFromMysqlStr(CString stream, COleDateTime* time);
	void MakeMysqlStrFromOleTime(COleDateTime time, CString& strDate);
	CString GetSystemDate();
	HPoint TokenizeStringByHPoint(CString string);
	HPoint TokenizeStringByHPoint(char* string);
	void TokenizeObPropertyString(CString stream, CArray<OBJECT_PROPERTY, OBJECT_PROPERTY&>* ob_property_list);
	void TokenizeMmatrixString(CString stream, float* matrix);
	void TokenizeMmatrixString(CString stream, double* matrix);	//-->하남국-20120826-03-584
	CString MakeStringFromPosition(HPoint val);
	void complete_log(CString strlog);
	CString ConvertUsrVarPropIDToString(UINT nIdentifier);
	UINT ConvertUsrVarPropStringToID(CString strIdentifier);
	CString ConvertOneDBQuery(CString strWhere, CArray<UINT, UINT>* arrFIDs);

public:
	//테이블 스킴 변경관련
// 	int AddQindexColumn(CString table_name);
// 	int SetPrimaryKey(CString table_name, CString column);
// 	int AddFieldToModelTB(CString model_type);
// 	int AddQindexColumnToModelTB(CString model_type);

public:
	//트렌젝션 관련
	int BeginTransaction();
	int RollBackTransaction();
	int CommitTransaction();

public:
	//DB 관련
// 	int ConnectToDatabase(CString host, CString user, CString pwd, CString db_name, unsigned int port);
// 	int DisConnectToDatabase(void);
// 	int FlushUser();
// 	int SendQuery(CString query, _RecordsetPtr& pRs);
// 	int DoUpdateQuery(CString db_name, CString table_name, CString update_syntax, CString where_syntax);


public:
	//tb_ecomodel_error_list
	int GetErrorList(CString project_group, CString ship_no, CString model_type, CString side, CString error_type, CArray<CString, CString&>* object_nm_list);
	int DeleteErrorList(CString project_group, CString ship_no, CString model_type, CString side, CString error_type, CString object_nm);
	int UpdateToErrorList(CString project_group, CString ship_no, CString model_type, CString side, CString error_type, CString object_nm);


public:
	//테이블데이터 중복검사
	int bDuplicateData(CString db, CString table_name, CString where_str, bool* bDuplicate);

public:
//DB생성
	int CreateDatabase(CString strDBName, CString strShipNo, CString strGroup);
	int CreateUpdateInfoDB();
	int CreateShipNoDB();
	int	CreateConCurrentInfoDB();


public:
//Block, 모델, 기타 테이블 생성관련
	int CreateEcoModelTB(CString model_type, bool bCreateErrorListTB = true);
	int CreateEcoBlockProjectTB();
	int CreateGenTabTB();
	int CreateControlPointCurveTB();
	int CreateManualMesManagerTB();
	int CreateSystemBlockNamelistTB();
	int CreateAddMeasurePointinTB();
	int CreateDeleteMeasurePointinTB();
	int CreateSystemDrawingTB();
	int CreateUproadFileTB();
	int CreateUserInfoTB();
	int CreateTemplateTB();
	int CreateUserTemplateInfoTB();
	//MEI관련 추가 Table생성
	int CreateEcoModelTB_MEI(CString project_group, CString ship_no, CString model_type);
	int CreateHierarchyTreeTB();
	int CreateEcoModelManagerTB();
	

//OTS용 테이블 생성관련
	int CreateOtsTables();
	int CreateEcoOtsProjectTB();
	int CreateEcoOtsBlockListTB();
	int CreateEcoOtsAddMeasurePointInfoTB();
	int CreateEcoOtsDeleteMeasurePointInfoTB();
	int CreateEcoOtsDesingPointGeneralInfoTB();
	int CreateEcoOtsRelationByDesMsrTB();
	int CreateEcoOtsRelationByMsrMsrTB();
	int CreateEcoOtsPointCondTB();
	int CreateEcoOtsDistanceCondTB();
	int CreateEcoOtsDrawingElementTB();
	int CreateEcoOtsCameraInfoTB();
	int CreateEcoOtsProjectionFaceListTB();
	int CreateEcoOtsProjectionFaceModelingMatrixListTB();
	
//PM용 테이블 생성관련
	int CreateAssemblyLevelTB();
	int CreateAssemblyTreeTB();
	int CreateVariableGroupTB();
	int CreateUserVariableTB();
	int CreateProjectPropertyTB();
	int CreateBlockPropertyTB();
	int CreateShipnoPropertyTB();
	int CreateEcoblockProjectTreeTB();
	int CreateProjectTB();
	int CreateUserInputBlockListTB();
	int CreateVariableOfGroupTB();

// tb_user_info
public:
	int InsertUserInfoTB(CArray<CUserInfoRecordSet, CUserInfoRecordSet&> *user_info_record);
	int UpdateUserGrant(CArray<CUserInfoRecordSet, CUserInfoRecordSet&> *user_info_record);
	int UpdateUserInfo(CArray<CUserInfoRecordSet, CUserInfoRecordSet&> *user_info_record);
	int DeleteUserInfoTB(CString user_id);
	int SelectUserInfoTB(CString where_condition, CArray<CUserInfoRecordSet, CUserInfoRecordSet&> *user_info_record);
	int CheckUserPriv(CString user_id, CUserInfoRecordSet* record);
	int CheckUserPriv(CString user_id, CString strPwd, CUserInfoRecordSet* record);
	int CheckSuperAdmin(CString id, BOOL* bSuperAdmin);

	//tb_shipno_info
public:
	int DeleteShipNoInfoTB(UINT id);
	int InsertShipNoInfoTB(CArray<CShipNoInfoRecordSet, CShipNoInfoRecordSet&> *record_list);
	int SelectShipNoInfoTB_WithOut_Blob(CString where_condition, CArray<CShipNoInfoRecordSet, CShipNoInfoRecordSet&> *record_list);
	int SelectShipNoInfoTB_With_Blob(CString where_condition, CArray<CShipNoInfoRecordSet, CShipNoInfoRecordSet&> *record_list);
	int GetGenTab(CString file_path, UINT groupID, UINT shipnoID);
	int UpdateToGenTab(CString file_path, CString project_group, CString ship_no);
	int UpdateToActive(CString strWhere, BOOL bActive);
	int GetShipNoID(CString project_group, CString ship_no, UINT* groupID, UINT* shipnoID);
	int GetShipNoList(CArray<CShipNoInfoRecordSet, CShipNoInfoRecordSet&> *record_list);
	CArray<SHIP_NO_INFO, SHIP_NO_INFO> m_ship_no_list;
	int GetTbShipNoData();
	bool ExistShipInfo(CString project_group, CString ship_no);
	//int GetEmProjectInfo(UINT nShipNoID, CString project_group, CString ship_no, CString model_type, CString* active, CString* create_date, CString* update_date);

	// tb_drawing_template
public:
	int UpdateTemplateTB(CArray<CTemplateTbRecordSet, CTemplateTbRecordSet&> *template_record, CString user_id);
	int UpdateTemplateTB_Description(CString strWhere, CString strDescription);
	int SelectTemplateTB_WithOut_Blob(CString where_condition, CArray<CTemplateTbRecordSet, CTemplateTbRecordSet&> *template_record);
	int SelectTemplateTB_With_Blob(CString where_condition, CArray<CTemplateTbRecordSet, CTemplateTbRecordSet&> *template_record);
	int SelectTemplateTB_Category(CArray<CTemplateTbRecordSet, CTemplateTbRecordSet&> *template_record);
	int DeleteTemplate(CString template_name, CString category);

	// tb_system_drawing
public:
	int DeleteSystemDrawing(CString project_group, CString ship_no, CString drawing_name);
	int UpdateSystemDrawingTB(CString project_group, CString ship_no, CArray<CSystemDrawingRecordSet, CSystemDrawingRecordSet&> *record);
	int SelectSystemDrawingTB_WithOut_Blob(CString project_group, CString ship_no, CString where_condition, CArray<CSystemDrawingRecordSet, CSystemDrawingRecordSet&> *record);
	int SelectSystemDrawingTB_With_Blob(CString project_group, CString ship_no, CString where_condition, CArray<CSystemDrawingRecordSet, CSystemDrawingRecordSet&> *record);

	// tb_project
public:
	int DeleteAllProject(CString project_group, CString ship_no);
	int DeleteProjectTB(CString project_group, CString ship_no, UINT id);
	int InsertProjectTB(CString project_group, CString ship_no, CArray<CProjectRecordSet, CProjectRecordSet&> *record);
	int UpdateProjectTB(CString project_group, CString ship_no, CArray<CProjectRecordSet, CProjectRecordSet&> *record_list);
	int SelectProjectTB(CString project_group, CString ship_no, CString where_condition, CArray<CProjectRecordSet, CProjectRecordSet&> *record);
	int SelectProjectTB(CString project_group, CString ship_no, CString where_condition, CArray<PROJECT_WITH_PROPERTY, PROJECT_WITH_PROPERTY&> *record);
	int SelectLastInsertIDProjectTB(CString project_group, CString ship_no, UINT& id);
	int SelectLastInsertIDProjectTBByIdentCurrent(CString project_group, CString ship_no, UINT& id);

	// tb_measure_point 관련
public:
	int DeleteMeasurePointTB(CString project_group, CString ship_no, UINT id);
	int UpdateMeasurePointTB(CArray<CMeasurePointRecordSet, CMeasurePointRecordSet&> *measure_point_record);
	int SelectMeasurePointTB_With_Blob(CString project_group, CString ship_no, 
		CString where_condition, CArray<CMeasurePointRecordSet, CMeasurePointRecordSet&> *measure_point_record);
	int SelectMeasurePointTB_Without_Blob(CString project_group, CString ship_no, 
		CString where_condition, CArray<CMeasurePointRecordSet, CMeasurePointRecordSet&> *measure_point_record);
	int SelectLastInsertIDMeasurePointTB(CString project_group, CString ship_no, UINT& id);
	int SelectLastInsertIDMeasurePointTBByIdentCurrent(CString project_group, CString ship_no, UINT& id);

	// tb_EcoBlock_relation_info 관련
public:
	int DeleteEcoBlockRelationInfoTB(CString project_group, CString ship_no, UINT id);
	int InsertEcoBlockRelationInfoTB(CArray<CEcoBlockRelationInfoRecordSet, CEcoBlockRelationInfoRecordSet&> *eb_relation_record);
	int SelectEcoBlockRelationInfoTB(CString project_group, CString ship_no, 
		CString where_condition, CArray<CEcoBlockRelationInfoRecordSet, CEcoBlockRelationInfoRecordSet&> *eb_relation_record);

	// tb_EcoBlock_project 관련
public:
	int CheckExistEbPjt(CString pjt_group, CString ship_no, UINT nPjtID, bool* bExist, CString* update_user, CString* update_date);
	int DeleteEcoBlockPjtTB(CString project_group, CString ship_no, UINT id);
	//insert
	int UpdateManualEcoBlockProjectTB(CArray<CEcoBlockProjectRecordSet, CEcoBlockProjectRecordSet&> *EcoBlock_project_record, CString user_id);
	//update
	int UpdateManualEcoBlockProjectTB2(CArray<CEcoBlockProjectRecordSet, CEcoBlockProjectRecordSet&> *EcoBlock_project_record, CString user_id);
	int UpdateEcoBlockProjectTB(CArray<CEcoBlockProjectRecordSet, CEcoBlockProjectRecordSet&> *EcoBlock_project_record, CString user_id);
	int SelectEcoBlockProjectTB_With_Blob(CString project_group, CString ship_no, 
		CString where_condition, CArray<CEcoBlockProjectRecordSet, CEcoBlockProjectRecordSet&> *EcoBlock_project_record);
	int SelectEcoBlockProjectTB_Without_Blob(CString project_group, CString ship_no, 
		CString where_condition, CArray<CEcoBlockProjectRecordSet, CEcoBlockProjectRecordSet&> *EcoBlock_project_record);
	int SelectEcoBlockProjectTB_Without_Blob2(CString project_group, CString ship_no, 
		CString where_condition, CArray<CEcoBlockProjectRecordSet, CEcoBlockProjectRecordSet&> *EcoBlock_project_record);

	// tb_EcoBlock_design_point_general_info 관련
public:
	int DeleteEcoBlockDesPtGeneralInfoTB(CString project_group, CString ship_no, UINT id);
	int InsertEcoBlockDesPtGeneralInfoTB(CArray<CEcoBlockDesignPointGeneralInfoRecordSet, CEcoBlockDesignPointGeneralInfoRecordSet&> *eb_des_pt_general_info_record);
	int SelectEcoBlockDesPtGeneralInfoTB(CString project_group, CString ship_no, 
		CString where_condition, CArray<CEcoBlockDesignPointGeneralInfoRecordSet, CEcoBlockDesignPointGeneralInfoRecordSet&> *eb_des_pt_general_info_record);

	// tb_ecoblock_manual_mes_manager 관련
public:
	int DeleteEcoBlockManualMesManagerTB(CString project_group, CString ship_no, UINT id);
	int InsertEcoBlockManualMesManagerTB(CArray<CEcoBlockManualMesManagerRecordSet, CEcoBlockManualMesManagerRecordSet&> *eb_manual_mes_manager_record);
	int SelectEcoBlockManualMesManagerTB(CString project_group, CString ship_no, 
		CString where_condition, CArray<CEcoBlockManualMesManagerRecordSet, CEcoBlockManualMesManagerRecordSet&> *eb_manual_mes_manager_record);

	// tb_EcoBlock_control_point_coord_info 관련
public:
	int DeleteEcoBlockControlPointCoordInfoTB(CString project_group, CString ship_no, UINT id);
	int InsertEcoBlockControlPointCoordInfoTB(CArray<CEcoBlockControlPointCoordInfoRecordSet, CEcoBlockControlPointCoordInfoRecordSet&> *eb_cp_coord_info_record);
	int SelectEcoBlockControlPointCoordInfoTB(CString project_group, CString ship_no, 
		CString where_condition, CArray<CEcoBlockControlPointCoordInfoRecordSet, CEcoBlockControlPointCoordInfoRecordSet&> *eb_cp_coord_info_record);

	// tb_EcoBlock_control_point_distance_info 관련
public:
	int DeleteEcoBlockControlPointDistanceInfoTB(CString project_group, CString ship_no, UINT id);
	int InsertEcoBlockControlPointDistanceInfoTB(CArray<CEcoBlockControlPointDistanceInfoRecordSet, CEcoBlockControlPointDistanceInfoRecordSet&> *eb_cp_distance_info_record);
	int SelectEcoBlockControlPointDistanceInfoTB(CString project_group, CString ship_no, 
		CString where_condition, CArray<CEcoBlockControlPointDistanceInfoRecordSet, CEcoBlockControlPointDistanceInfoRecordSet&> *eb_cp_distance_info_record);

	// tb_ecoblock_contorl_point_curve_info 관련
public:
	int DeleteEcoBlockControlPointCurveTB(CString project_group, CString ship_no, UINT id);
	int InsertEcoBlockControlPointCurveTB(CArray<CEcoBlockControlPointCurveInfoRecordSet, CEcoBlockControlPointCurveInfoRecordSet&> *eb_control_point_curve_info_record);
	int SelectEcoBlockControlPointCurveInfoTB(CString project_group, CString ship_no, 
		CString where_condition, CArray<CEcoBlockControlPointCurveInfoRecordSet, CEcoBlockControlPointCurveInfoRecordSet&> *eb_control_point_curve_info_record);

	// tb_EcoBlock_control_point_perp_info 관련
public:
	int DeleteEcoBlockControlPointPerpInfoTB(CString project_group, CString ship_no, UINT id);
	int InsertEcoBlockControlPointPerpInfoTB(CArray<CEcoBlockControlPointPerpInfoRecordSet, CEcoBlockControlPointPerpInfoRecordSet&> *eb_cp_perp_info_record);
	int SelectEcoBlockControlPointPerpInfoTB(CString project_group, CString ship_no, 
		CString where_condition, CArray<CEcoBlockControlPointPerpInfoRecordSet, CEcoBlockControlPointPerpInfoRecordSet&> *eb_cp_perp_info_record);

	// tb_EcoBlock_control_point_angle_info 관련
public:
	int DeleteEcoBlockControlPointAngleInfoTB(CString project_group, CString ship_no, UINT id);
	int InsertEcoBlockControlPointAngleInfoTB(CArray<CEcoBlockControlPointAngleInfoRecordSet, CEcoBlockControlPointAngleInfoRecordSet&> *eb_cp_angle_info_record);
	int SelectEcoBlockControlPointAngleInfoTB(CString project_group, CString ship_no, 
		CString where_condition, CArray<CEcoBlockControlPointAngleInfoRecordSet, CEcoBlockControlPointAngleInfoRecordSet&> *eb_cp_angle_info_record);

	// tb_EcoBlock_control_point_flat_info 관련
public:
	int DeleteEcoBlockControlPointFlatInfoTB(CString project_group, CString ship_no, UINT id);
	int InsertEcoBlockControlPointFlatInfoTB(CArray<CEcoBlockControlPointFlatInfoRecordSet, CEcoBlockControlPointFlatInfoRecordSet&> *eb_cp_flat_info_record);
	int SelectEcoBlockControlPointFlatInfoTB(CString project_group, CString ship_no, 
		CString where_condition, CArray<CEcoBlockControlPointFlatInfoRecordSet, CEcoBlockControlPointFlatInfoRecordSet&> *eb_cp_flat_info_record);

	// tb_EcoBlock_camera_info 관련
public:
	int DeleteEcoBlockCameraInfoTB(CString project_group, CString ship_no, UINT id);
	int InsertEcoBlockCameraInfoTB(CArray<CEcoBlockCameraInfoRecordSet, CEcoBlockCameraInfoRecordSet&> *eb_camera_info_record, CString user_id);
	int SelectEcoBlockCameraInfoTB(CString project_group, CString ship_no, 
		CString where_condition, CArray<CEcoBlockCameraInfoRecordSet, CEcoBlockCameraInfoRecordSet&> *eb_camera_info_record);

	// tb_EcoBlock_projection_face_list 관련
public:
	int DeleteEcoBlockProjectionFaceListTB(CString project_group, CString ship_no, UINT id);
	int InsertEcoBlockProjectionFaceListTB(CArray<CEcoBlockProjectionFaceListRecordSet, CEcoBlockProjectionFaceListRecordSet&> *eb_projection_face_list_record, CString user_id);
	int SelectEcoBlockProjectionFaceListTB(CString project_group, CString ship_no, 
		CString where_condition, CArray<CEcoBlockProjectionFaceListRecordSet, CEcoBlockProjectionFaceListRecordSet&> *eb_projection_face_list_record);


	// tb_EcoBlock_add_measure_point_info
public:
	int DeleteEcoBlockAddMeasurePointInfoTB(CString project_group, CString ship_no, UINT id);
	int InsertEcoBlockAddMeasurePointInfoTB(CString project_group, CString ship_no,
		CArray<CEcoBlockAddMeasurePointInfoRecordSet, CEcoBlockAddMeasurePointInfoRecordSet&> *record_list);
	int SelectEcoBlockAddMeasurePointInfoTB(CString project_group, CString ship_no, 
		CString where_condition, CArray<CEcoBlockAddMeasurePointInfoRecordSet, CEcoBlockAddMeasurePointInfoRecordSet&> *record_list);

	// tb_EcoBlock_delete_measure_point_info
public:
	int DeleteEcoBlockDeleteMeasurePointInfoTB(CString project_group, CString ship_no, UINT id);
	int InsertEcoBlockDeleteMeasurePointInfoTB(CString project_group, CString ship_no,
		CArray<CEcoBlockDeleteMeasurePointInfoRecordSet, CEcoBlockDeleteMeasurePointInfoRecordSet&> *record_list);
	int SelectEcoBlockDeleteMeasurePointInfoTB(CString project_group, CString ship_no, 
		CString where_condition, CArray<CEcoBlockDeleteMeasurePointInfoRecordSet, CEcoBlockDeleteMeasurePointInfoRecordSet&> *record_list);

public:
	// tb_system_block_list
	//tribon에서 tb_system_block_list로 insert
	int GetSystemBlockName(CString project_group, CString ship_no);
	int GetBlockNameList(CString strGroup, CString ShipNo, CString strQuery, CArray<CString, CString&>* arryList);
	int DeleteSystemBlockListTB(CString project_group, CString ship_no, CString block_name);
	int InsertSystemBlockListTB(CString project_group, CString ship_no, CArray<CSystemBlockListRecordSet, CSystemBlockListRecordSet&> *record_list);
	int UpdateSystemBlockListTB(CString project_group, CString ship_no, CString pre_name, CString new_name);
	int SelectSystemBlockListTB(CString project_group, CString ship_no, CString where_str, 
		CArray<CSystemBlockListRecordSet, CSystemBlockListRecordSet&> *record_list);


// tb_ecoots_project
public:
	int DeleteEcoOtsProjectTB(CString project_group, CString ship_no, UINT id);
	int UpdateEcoOtsProjectTBOnlyTree(CString project_group, CString ship_no, CArray<CEcoOtsProjectRecordSet, CEcoOtsProjectRecordSet&> *record_list);
	int InsertEcoOtsProjectTB(CString project_group, CString ship_no, CString user_id,
		CArray<CEcoOtsProjectRecordSet, CEcoOtsProjectRecordSet&> *record_list);
	int SelectEcoOtsProjectTB_WithOut_Blob(CString project_group, CString ship_no, 
		CString where_condition, CArray<CEcoOtsProjectRecordSet, CEcoOtsProjectRecordSet&> *record_list);
	int SelectEcoOtsProjectTB_With_Blob(CString project_group, CString ship_no, 
		CString where_condition, CArray<CEcoOtsProjectRecordSet, CEcoOtsProjectRecordSet&> *record_list);
	int SelectLastInsertIDEcoOtsProjectTB(CString project_group, CString ship_no, UINT& id);
	int SelectLastInsertIDEcoOtsProjectTBByIdentCurrent(CString project_group, CString ship_no, UINT& id);

// tb_ecoots_block_list
public:
	int DeleteEcoOtsBlockListTB(CString project_group, CString ship_no, UINT id);
	int InsertEcoOtsBlockListTB(CString project_group, CString ship_no, CString user_id,
		CArray<CEcoOtsBlockListRecordSet, CEcoOtsBlockListRecordSet&> *record_list);
	int SelectEcoOtsBlockListTB_WithOut_Blob(CString project_group, CString ship_no, 
		CString where_condition, CArray<CEcoOtsBlockListRecordSet, CEcoOtsBlockListRecordSet&> *record_list);
	int SelectEcoOtsBlockListTB_With_Blob(CString project_group, CString ship_no, 
		CString where_condition, CArray<CEcoOtsBlockListRecordSet, CEcoOtsBlockListRecordSet&> *record_list);

// tb_ecoots_add_measure_point_info
public:
	int DeleteEcoOtsAddMeasurePointInfoTB(CString project_group, CString ship_no, UINT id);
	int InsertEcoOtsAddMeasurePointInfoTB(CString project_group, CString ship_no,
		CArray<CEcoOtsAddMeasurePointInfoRecordSet, CEcoOtsAddMeasurePointInfoRecordSet&> *record_list);
	int SelectEcoOtsAddMeasurePointInfoTB(CString project_group, CString ship_no, 
		CString where_condition, CArray<CEcoOtsAddMeasurePointInfoRecordSet, CEcoOtsAddMeasurePointInfoRecordSet&> *record_list);

// tb_ecoots_delete_measure_point_info
public:
	int DeleteEcoOtsDeleteMeasurePointInfoTB(CString project_group, CString ship_no, UINT id);
	int InsertEcoOtsDeleteMeasurePointInfoTB(CString project_group, CString ship_no,
		CArray<CEcoOtsDeleteMeasurePointInfoRecordSet, CEcoOtsDeleteMeasurePointInfoRecordSet&> *record_list);
	int SelectEcoOtsDeleteMeasurePointInfoTB(CString project_group, CString ship_no, 
		CString where_condition, CArray<CEcoOtsDeleteMeasurePointInfoRecordSet, CEcoOtsDeleteMeasurePointInfoRecordSet&> *record_list);

// tb_ecoots_design_point_general_info
public:
	int DeleteEcoOtsDesPtGeneralInfoTB(CString project_group, CString ship_no, UINT id);
	int InsertEcoOtsDesPtGeneralInfoTB(CString project_group, CString ship_no,
		CArray<CEcoOtsDesignPointGeneralInfoRecordSet, CEcoOtsDesignPointGeneralInfoRecordSet&> *eb_des_pt_general_info_record);
	int SelectEcoOtsDesPtGeneralInfoTB(CString project_group, CString ship_no, 
		CString where_condition, CArray<CEcoOtsDesignPointGeneralInfoRecordSet, CEcoOtsDesignPointGeneralInfoRecordSet&> *eb_des_pt_general_info_record);

// tb_ecoots_relation_by_des_msr
public:
	int DeleteEcoOtsRelationByDesMsrTB(CString project_group, CString ship_no, UINT id);
	int InsertEcoOtsRelationByDesMsrTB(CString project_group, CString ship_no,
		CArray<CEcoOtsRelationByDesMsrRecordSet, CEcoOtsRelationByDesMsrRecordSet&> *record_list);
	int SelectEcoOtsRelationByDesMsrTB(CString project_group, CString ship_no, 
		CString where_condition, CArray<CEcoOtsRelationByDesMsrRecordSet, CEcoOtsRelationByDesMsrRecordSet&> *record_list);

// tb_ecoots_relation_by_msr_msr
public:
	int DeleteEcoOtsRelationByMsrMsrTB(CString project_group, CString ship_no, UINT id);
	int InsertEcoOtsRelationByMsrMsrTB(CString project_group, CString ship_no,
		CArray<CEcoOtsRelationByMsrMsrRecordSet, CEcoOtsRelationByMsrMsrRecordSet&> *record_list);
	int SelectEcoOtsRelationByMsrMsrTB(CString project_group, CString ship_no, 
		CString where_condition, CArray<CEcoOtsRelationByMsrMsrRecordSet, CEcoOtsRelationByMsrMsrRecordSet&> *record_list);

// tb_ecoots_point_cond
public:
	int DeleteEcoOtsPointCondTB(CString project_group, CString ship_no, UINT id);
	int InsertEcoOtsPointCondTB(CString project_group, CString ship_no,
		CArray<CEcoOtsPointCondRecordSet, CEcoOtsPointCondRecordSet&> *record_list);
	int SelectEcoOtsPointCondTB(CString project_group, CString ship_no, 
		CString where_condition, CArray<CEcoOtsPointCondRecordSet, CEcoOtsPointCondRecordSet&> *record_list);

// tb_ecoots_distance_cond
public:
	int DeleteEcoOtsDistanceCondTB(CString project_group, CString ship_no, UINT id);
	int InsertEcoOtsDistanceCondTB(CString project_group, CString ship_no,
		CArray<CEcoOtsDistanceCondRecordSet, CEcoOtsDistanceCondRecordSet&> *record_list);
	int SelectEcoOtsDistanceCondTB(CString project_group, CString ship_no, 
		CString where_condition, CArray<CEcoOtsDistanceCondRecordSet, CEcoOtsDistanceCondRecordSet&> *record_list);

// tb_ecoots_drawing_element
public:
	int DeleteEcoOtsDrawingElementTB(CString project_group, CString ship_no, UINT id);
	int InsertEcoOtsDrawingElementTB(CString project_group, CString ship_no, CString user_id,
		CArray<CEcoOtsDrawingElementRecordSet, CEcoOtsDrawingElementRecordSet&> *record_list);
	int SelectEcoOtsDrawingElementTB_With_Blob(CString project_group, CString ship_no, 
		CString where_condition, CArray<CEcoOtsDrawingElementRecordSet, CEcoOtsDrawingElementRecordSet&> *record_list);

// tb_ecoots_camera_info 관련
public:
	int DeleteEcoOtsCameraInfoTB(CString project_group, CString ship_no, UINT id);
	int InsertEcoOtsCameraInfoTB(CString project_group, CString ship_no, CString user_id,
		CArray<CEcoOtsCameraInfoRecordSet, CEcoOtsCameraInfoRecordSet&> *record_list);
	int SelectEcoOtsCameraInfoTB(CString project_group, CString ship_no, 
		CString where_condition, CArray<CEcoOtsCameraInfoRecordSet, CEcoOtsCameraInfoRecordSet&> *record_list);

// tb_ecoots_projection_face_list
public:
	int DeleteEcoOtsProjectionFaceListTB(CString project_group, CString ship_no, UINT id);
	int InsertEcoOtsProjectionFaceListTB(CString project_group, CString ship_no, CString user_id,
		CArray<CEcoOtsProjectionFaceListRecordSet, CEcoOtsProjectionFaceListRecordSet&> *record_list);
	int SelectEcoOtsProjectionFaceListTB(CString project_group, CString ship_no, 
		CString where_condition, CArray<CEcoOtsProjectionFaceListRecordSet, CEcoOtsProjectionFaceListRecordSet&> *record_list);

// tb_ecoots_projection_face_modeling_matrix_list
public:
	int DeleteEcoOtsProjectionFaceModelingMatrixListTB(CString project_group, CString ship_no, UINT id);
	int InsertEcoOtsProjectionFaceModelingMatrixListTB(CString project_group, CString ship_no, CString user_id,
		CArray<CEcoOtsProjectionFaceModelingMatrixListRecordSet, CEcoOtsProjectionFaceModelingMatrixListRecordSet&> *record_list);
	int SelectEcoOtsProjectionFaceModelingMatrixListListTB(CString project_group, CString ship_no, 
		CString where_condition, CArray<CEcoOtsProjectionFaceModelingMatrixListRecordSet, CEcoOtsProjectionFaceModelingMatrixListRecordSet&> *record_list);

// tb_assembly_level
public:
	int DeleteAllProductionInfo(CString project_group, CString ship_no);

	int DeleteAssemblyLevelTB(CString project_group, CString ship_no, UINT id);
	int InsertAssemblyLevelTB(CString project_group, CString ship_no,
		CArray<CAssemblyLevelRecordSet, CAssemblyLevelRecordSet&> *record_list);
	int UpdateAssemblyLevelTB(CString project_group, CString ship_no,
		CArray<CAssemblyLevelRecordSet, CAssemblyLevelRecordSet&> *record_list);
	int SelectAssemblyLevelTB(CString project_group, CString ship_no, 
		CString where_condition, CArray<CAssemblyLevelRecordSet, CAssemblyLevelRecordSet&> *record_list);
	int SelectLastInsertIDAssemblyLevelTB(CString project_group, CString ship_no, UINT& id);
	int SelectLastInsertIDAssemblyLevelTBByIdentCurrent(CString project_group, CString ship_no, UINT& id);

// tb_assembly_tree
public:
	int DeleteAssemblyTreeTB(CString project_group, CString ship_no, UINT id);
	int InsertAssemblyTreeTB(CString project_group, CString ship_no,
		CArray<CAssemblyTreeRecordSet, CAssemblyTreeRecordSet&> *record_list);
	int UpdateAssemblyTreeTB(CString project_group, CString ship_no,
		CArray<CAssemblyTreeRecordSet, CAssemblyTreeRecordSet&> *record_list);
	int SelectAssemblyTreeTB(CString project_group, CString ship_no, 
		CString where_condition, CArray<CAssemblyTreeRecordSet, CAssemblyTreeRecordSet&> *record_list);
	int SelectLastInsertIDAssemblyTreeTB(CString project_group, CString ship_no, UINT& id);
	int SelectLastInsertIDAssemblyTreeTBByIdentCurrent(CString project_group, CString ship_no, UINT& id);

// tb_variable_group
public:
	int DeleteVariableGroupTB(CString project_group, CString ship_no, UINT id);
	int InsertVariableGroupTB(CString project_group, CString ship_no,
		CArray<CVariableGroupRecordSet, CVariableGroupRecordSet&> *record_list);
	int UpdateVariableGroupTB(CString project_group, CString ship_no,
		CArray<CVariableGroupRecordSet, CVariableGroupRecordSet&> *record_list);
	int SelectVariableGroupTB(CString project_group, CString ship_no, 
		CString where_condition, CArray<CVariableGroupRecordSet, CVariableGroupRecordSet&> *record_list);
	int SelectLastInsertIDVariableGroupTB(CString project_group, CString ship_no, UINT& id);
	int SelectLastInsertIDVariableGroupTBByIdentCurrent(CString project_group, CString ship_no, UINT& id);

//-->하남국-20131113
	//tb_variable_of_group
public:
	int DeleteVariableOfGroupTB(CString project_group, CString ship_no, UINT id);
	int DeleteVariableOfGroupTBByGroupID(CString project_group, CString ship_no, UINT f_id);
	int DeleteVariableOfGroupTBByVarID(CString project_group, CString ship_no, UINT f_id_variable);
	int InsertVariableOfGroupTB(CString project_group, CString ship_no,
		CArray<CVariableOfGroupRecordSet, CVariableOfGroupRecordSet&> *record_list);
	int UpdateVariableOfGroupTB(CString project_group, CString ship_no,
		CArray<CVariableOfGroupRecordSet, CVariableOfGroupRecordSet&> *record_list);
	int SelectVariableOfGroupTB(CString project_group, CString ship_no, 
		CString where_condition, CArray<CVariableOfGroupRecordSet, CVariableOfGroupRecordSet&> *record_list);
	int SelectLastInsertIDVariableOfGroupTB(CString project_group, CString ship_no, UINT& id);
	int SelectLastInsertIDVariableOfGroupTBByIdentCurrent(CString project_group, CString ship_no, UINT& id);
//<--하남국-20131113


// tb_user_variable
public:
	int DeleteUserVariableTB(CString project_group, CString ship_no, UINT id);
	int InsertUserVariableTB(CString project_group, CString ship_no,
											CArray<CUserVariableRecordSet, CUserVariableRecordSet&> *record_list);
	int UpdateUserVariableTB(CString project_group, CString ship_no,
											CArray<CUserVariableRecordSet, CUserVariableRecordSet&> *record_list);
	int SelectUserVariableTB(CString project_group, CString ship_no, 
											CString where_condition, CArray<CUserVariableRecordSet, CUserVariableRecordSet&> *record_list);
	int SelectLastInsertIDUserVariableTB(CString project_group, CString ship_no, UINT& id);
	int SelectLastInsertIDUserVariableTBByIdentCurrent(CString project_group, CString ship_no, UINT& id);

	

// tb_project_property
public:
	int DeleteProjectPropertyTB(CString project_group, CString ship_no, UINT id);
	int DeleteProjectPropertyTB(CString project_group, CString ship_no, CString strWhere);
	int InsertProjectPropertyTB(CString project_group, CString ship_no,
		CArray<CProjectPropertyRecordSet, CProjectPropertyRecordSet&> *record_list);
	int UpdateProjectPropertyTB(CString project_group, CString ship_no,
		CArray<CProjectPropertyRecordSet, CProjectPropertyRecordSet&> *record_list);
	int SelectProjectPropertyTB(CString project_group, CString ship_no, 
		CString where_condition, CArray<CProjectPropertyRecordSet, CProjectPropertyRecordSet&> *record_list);

// tb_block_property
public:
	int DeleteBlockPropertyTB(CString project_group, CString ship_no, UINT id);
	int DeleteAllBlockPropertyTB(CString project_group, CString ship_no, CString strWhere);
	int InsertBlockPropertyTB(CString project_group, CString ship_no,
		CArray<CBlockPropertyRecordSet, CBlockPropertyRecordSet&> *record_list);
	int UpdateBlockPropertyTB(CString project_group, CString ship_no,
		CArray<CBlockPropertyRecordSet, CBlockPropertyRecordSet&> *record_list);
	int SelectBlockPropertyTB(CString project_group, CString ship_no, 
		CString where_condition, CArray<CBlockPropertyRecordSet, CBlockPropertyRecordSet&> *record_list);
	int SelectLastInsertIDBlockPropertyTB(CString project_group, CString ship_no, UINT& id);
	int SelectLastInsertIDBlockPropertyTBByIdentCurrent(CString project_group, CString ship_no, UINT& id);

// tb_shipno_property
public:
	int DeleteShipnoPropertyTB(CString project_group, CString ship_no, UINT id);
	int InsertShipnoPropertyTB(CString project_group, CString ship_no,
		CArray<CShipnoPropertyRecordSet, CShipnoPropertyRecordSet&> *record_list);
	int SelectShipnoPropertyTB(CString project_group, CString ship_no, 
		CString where_condition, CArray<CShipnoPropertyRecordSet, CShipnoPropertyRecordSet&> *record_list);

// tb_ecoblock_project_tree
public:
	int DeleteEcoblockProjectTreeTB(CString project_group, CString ship_no, UINT id);
	int InsertEcoblockProjectTreeTB(CString project_group, CString ship_no,
		CArray<CEcoBlockProjectTreeRecordSet, CEcoBlockProjectTreeRecordSet&> *record_list);
	int UpdateEcoblockProjectTreeTB(CString project_group, CString ship_no,
		CArray<CEcoBlockProjectTreeRecordSet, CEcoBlockProjectTreeRecordSet&> *record_list);
	int SelectEcoblockProjectTreeTB(CString project_group, CString ship_no, 
		CString where_condition, CArray<CEcoBlockProjectTreeRecordSet, CEcoBlockProjectTreeRecordSet&> *record_list);
	int SelectLastInsertIDProjectTreeTB(CString project_group, CString ship_no, UINT& id);
	int SelectLastInsertIDProjectTreeTBByIdentCurrent(CString project_group, CString ship_no, UINT& id);

//-->하남국-20121219
// tb_user_input_block_list
public:
	int DeleteUserInputBlockListTB(CString project_group, CString ship_no, UINT id);
	int DeleteUserInputBlockListTB(CString project_group, CString ship_no, CString name);
	int InsertUserInputBlockListTB(CString project_group, CString ship_no,
		CArray<CUserInputBlockListRecordSet, CUserInputBlockListRecordSet&> *record_list);
	int SelectUserInputBlockListTB(CString project_group, CString ship_no, 
		CString where_condition, CArray<CUserInputBlockListRecordSet, CUserInputBlockListRecordSet&> *record_list);
//<--하남국-20121219

public:
	// tb_upload_file
	int DeleteUploadFile(CString project_group, CString ship_no, UINT nID);
	int DeleteUploadFile(CString project_group, CString ship_no, CString strWhereDelete);
	int InsertUploadFile(CString project_group, CString ship_no, CUploadFileRecordSet* record);
//	int UpdateUploadFile(CString project_group, CString ship_no, UINT nID, CUploadFileRecordSet* record);
	int SelectUploadFile_With_Blob(CString project_group, CString ship_no, CString where_str,
		CArray<CUploadFileRecordSet, CUploadFileRecordSet&>* record_list);
	int SelectUploadFile_WithOut_Blob(CString project_group, CString ship_no, CString where_str,
		CArray<CUploadFileRecordSet, CUploadFileRecordSet&>* record_list);


// tb_auto_update_file
public:
	int DeleteAutoUpdateFileTB(CString file_name);
	int DeleteAutoUpdateFileTB(CString file_name, CString strPlatform);
	int InsertAutoUpdateFileTB(CArray<CAutoUpdateFileRercordSet, CAutoUpdateFileRercordSet&> *record_list);
	int SelectAutoUpdateFileTB_WithOut_Blob(CString where_str, CArray<CAutoUpdateFileRercordSet, CAutoUpdateFileRercordSet&> *record_list);
	int SelectAutoUpdateFileTB_With_Blob(CString where_str, CArray<CAutoUpdateFileRercordSet, CAutoUpdateFileRercordSet&> *record_list);

public:
	//tb_concurrent_info 관련
	int GetFeatureID(CString strProductName, int* i_ID);
	

	//server manager 부분
public:
	// tb_ecomodel_???
	//update
	//tribon -> modelserver
	int UpdateToModelServer(int by_using, int insert_mode, CString file_path, CString project_group, CString ship_no, CString model_type, CString side, CString cpanel_name);
	int UpdateToModelServer(int by_using, int insert_mode, CString file_path,  CString file_path_hsf, CString project_group, CString ship_no, CString model_type, CString side, CString cpanel_name, CProgressCtrl* pCtrl = NULL, CStatic* pStatic = NULL);
public:
	//tb_system_drawing, tb_ecomodel_???
	int GetModelListFromModelServerBy_Q_Index(CString project_group, CString ship_no, CString model_type, CString side, CString query_str, CString query_type, CArray<CString, CString&>* model_list);
	int SelectModeldelete(CString strDB, CString str_Type, CString query_str);
	//모델 copy
	int SelectModelCopy(CString str_OriDB, CString str_ResDB, CString str_Type, CString query_str);

	//delete
	//테이블 자체를 삭제
	int DeleteTableData(CString project_group, CString ship_no, CString model_type, CString where_str);
	//데이터만 모두 삭제
	int DeleteAllModel(CString model_type);

	//select
	int GetModelFromModelServer(CString project_group, CString ship_no, CString model_type, CString side, CString query_str, CArray<CString, CString&>* exception_list, CString query_type, CString download_path);
	int GetModelFromModelServerByHsf(CString project_group, CString ship_no, CString model_type, CString side, CString query_str, CArray<CString, CString&>* exception_list, CString query_type, CString download_path, CArray<CEcoModelRecordSet, CEcoModelRecordSet&>* model_record_list);
	int GetOtherCadModelFromModelServer(CString project_group, CString ship_no, CString query_str, CString download_path);
	int GetModelListFromModelServer(CString project_group, CString ship_no, CString model_type, CString side, CString query_str, CString query_type, CArray<CString, CString&>* model_list);

	int ModelTypetoQindex(CString strDbNm, CString strModelType, CString strModulNm, CString strSuperiorNm, CString str_sidech, CString& strQ_index);
	
	//insert
	int InsertModelAfterDeleteByQIndex(CString db_name, CString project_group, CString ship_no, CString type, CString name, CString product_name, CString superior_name, CString module_name, 
		CString side, CString ass_name, CString contour_data_path, CString hsf_data_path, CString table_name);
	
	//Revision
	int CompareRevisionDate(CString panel_type, CArray<UPDATE_MODEL_INFO, UPDATE_MODEL_INFO&> *res_panel_list, CArray<CString, CString&>* update_model_list);
	int DeleteModelByRevision(CString project_group, CString ship_no, CString panel_type, CMap<CString, LPCTSTR, int, int>* panel_list_map);

	
	

public:
	// table복사관련 함수 추가
	int CreateEcoBlockRelationInfoTB_byCopy();
	int CreateEcoBlockAssemblylevelTB_byCopy(CString strNewDb, CString strtmp, CString strDbNameOri);

public:
	// tb_user_template_info
	int SelectUserTemplateInfo(CString where_str, CArray<CEcoUserTemplateInfo, CEcoUserTemplateInfo&> *record_list);
	int InsertUserTemplateInfo(CString UserID, CString temp_name);
	int UpdateUserTemplateInfo(UINT used_count, CString where_str);


//MEI관련
public:
	bool bmakeModelTB;

	//MEI tb_ecomodel_???
	int InsertEcoModelTB(CString project_group, CString ship_no, CEcoModelDataTableSet* record_list, CString hsf_data_path, int iNickTable, bool bFolder=true);
	int DeleteEcoModelTB(CString project_group, CString ship_no, UINT iNickTable, UINT f_id);

	// tb_ecomodel_manager
	int InsertEcoModelManager(CString project_group, CString ship_no, CArray<CEcoModelNameManagerRecordSet, CEcoModelNameManagerRecordSet&> *record_list);
	int SelectEcoModelManager(CString project_group, CString ship_no, CString where_str, CArray<CEcoModelNameManagerRecordSet, CEcoModelNameManagerRecordSet&> *record_list);

	// tb_hierarchy_tree
	int SelectHierarchytreeTB(CString project_group, CString ship_no, CString where_str, CArray<CEcoModelHierarchytreeRecordSet, CEcoModelHierarchytreeRecordSet&>* record_list);
	int InsertHierarchytreeTB(CString project_group, CString ship_no, CEcoModelHierarchytreeRecordSet* record_list);

	//-->2014-11-20 최조영 JT관련 코드 추가 (현대전용)
	int SelectShipNoInfo_HHI(CArray<CString, CString&>* arrylist);
	int SelectJTHierarchy_HHI(CString ship_no, CMap<CString, LPCTSTR, CString, LPCTSTR>* hierarchy_map);
	int SelectandFTPTransferJTFile_HHI(CString ship_no, CArray<CString, CString&>* arrylist);
	//<--2014-11-20 최조영 JT관련 코드 추가


};

