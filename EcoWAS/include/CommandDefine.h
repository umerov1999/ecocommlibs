#pragma once

//Message
#define				PKCMD_SUCCESS								0
#define				PKCMD_ERROR									1
#define				PKCMD_CLOSE									2

//네트워크 폴더 접근 정보
#define				PKCMD_GetNetFolderInfo						50

//tb_ecomodel_error_list
#define				PKCMD_GetErrorList							100
#define				PKCMD_DeleteErrorList						101
#define				PKCMD_UpdateToErrorList						102

//테이블데이터 중복검사
#define				PKCMD_bDuplicateData						200

//DB 생성
#define				PKCMD_CreateDatabase						300
#define				PKCMD_CreateUpdateInfoDB					301
#define				PKCMD_CreateShipNoDB						302
#define				PKCMD_CreateConCurrentInfoDB				303

//Block, 모델, 기타 테이블 생성관련
#define				PKCMD_CreateEcoModelTB						400
#define				PKCMD_CreateEcoBlockProjectTB				401
#define				PKCMD_CreateGenTabTB						402
#define				PKCMD_CreateControlPointCurveTB				403
#define				PKCMD_CreateManualMesManagerTB				404
#define				PKCMD_CreateSystemBlockNamelistTB			405
#define				PKCMD_CreateAddMeasurePointinTB				406
#define				PKCMD_CreateDeleteMeasurePointinTB			407
#define				PKCMD_CreateSystemDrawingTB					408
#define				PKCMD_CreateUproadFileTB					409
#define				PKCMD_CreateUserInfoTB						410
#define				PKCMD_CreateTemplateTB						411
#define				PKCMD_CreateUserTemplateInfoTB				412
//MEI관련 추가 Table생성
#define				PKCMD_CreateEcoModelTB_MEI					413
#define				PKCMD_CreateHierarchyTreeTB					414
#define				PKCMD_CreateEcoModelManagerTB				415


//OTS용 테이블 생성관련
#define				PKCMD_CreateOtsTables									500
#define				PKCMD_CreateEcoOtsProjectTB								501
#define				PKCMD_CreateEcoOtsBlockListTB							502
#define				PKCMD_CreateEcoOtsAddMeasurePointInfoTB					503
#define				PKCMD_CreateEcoOtsDeleteMeasurePointInfoTB				504
#define				PKCMD_CreateEcoOtsDesingPointGeneralInfoTB				505
#define				PKCMD_CreateEcoOtsRelationByDesMsrTB					506
#define				PKCMD_CreateEcoOtsRelationByMsrMsrTB					507
#define				PKCMD_CreateEcoOtsPointCondTB							508
#define				PKCMD_CreateEcoOtsDistanceCondTB						509
#define				PKCMD_CreateEcoOtsDrawingElementTB						510
#define				PKCMD_CreateEcoOtsCameraInfoTB							511
#define				PKCMD_CreateEcoOtsProjectionFaceListTB					512
#define				PKCMD_CreateEcoOtsProjectionFaceModelingMatrixListTB	513

//PM용 테이블 생성관련
#define				PKCMD_CreateAssemblyLevelTB					600
#define				PKCMD_CreateAssemblyTreeTB					601
#define				PKCMD_CreateVariableGroupTB					602
#define				PKCMD_CreateUserVariableTB					603
#define				PKCMD_CreateProjectPropertyTB				604
#define				PKCMD_CreateBlockPropertyTB					605
#define				PKCMD_CreateShipnoPropertyTB				606
#define				PKCMD_CreateEcoblockProjectTreeTB			607
#define				PKCMD_CreateProjectTB						608
#define				PKCMD_CreateUserInputBlockListTB			609
#define				PKCMD_CreateVariableOfGroupTB				610

// tb_user_info
#define				PKCMD_InsertUserInfoTB						700
#define				PKCMD_UpdateUserGrant						701
#define				PKCMD_UpdateUserInfo						702
#define				PKCMD_DeleteUserInfoTB						703
#define				PKCMD_SelectUserInfoTB						704
#define				PKCMD_CheckUserPriv							705
#define				PKCMD_CheckUserPriv_PWD						706
#define				PKCMD_CheckSuperAdmin						707

//tb_shipno_info
#define				PKCMD_DeleteShipNoInfoTB					800
#define				PKCMD_InsertShipNoInfoTB					801
#define				PKCMD_SelectShipNoInfoTB_WithOut_Blob		802
#define				PKCMD_SelectShipNoInfoTB_With_Blob			803
#define				PKCMD_GetGenTab								804
#define				PKCMD_UpdateToGenTab						805
#define				PKCMD_UpdateToActive						806
#define				PKCMD_GetShipNoID							807
#define				PKCMD_GetShipNoList							808
#define				PKCMD_GetTbShipNoData						809
#define				PKCMD_ExistShipInfo							810

// tb_drawing_template
#define				PKCMD_UpdateTemplateTB						900
#define				PKCMD_UpdateTemplateTB_Description			901
#define				PKCMD_SelectTemplateTB_WithOut_Blob			902
#define				PKCMD_SelectTemplateTB_With_Blob			903
#define				PKCMD_SelectTemplateTB_Category				904
#define				PKCMD_DeleteTemplate						905

// tb_system_drawing
#define				PKCMD_DeleteSystemDrawing					1000
#define				PKCMD_UpdateSystemDrawingTB					1001
#define				PKCMD_SelectSystemDrawingTB_WithOut_Blob	1002
#define				PKCMD_SelectSystemDrawingTB_With_Blob		1003

// tb_project
#define				PKCMD_DeleteProjectTB						1100
#define				PKCMD_InsertProjectTB						1101
#define				PKCMD_UpdateProjectTB						1102
#define				PKCMD_SelectProjectTB_CProjectRecordSet		1103
#define				PKCMD_SelectProjectTB_PROJECT_WITH_PROPERTY	1104
#define				PKCMD_SelectLastInsertIDProjectTB			1105

// tb_measure_point 관련
#define				PKCMD_DeleteMeasurePointTB					1200
#define				PKCMD_UpdateMeasurePointTB					1201
#define				PKCMD_SelectMeasurePointTB_With_Blob		1202
#define				PKCMD_SelectMeasurePointTB_Without_Blob		1203
#define				PKCMD_SelectLastInsertIDMeasurePointTB		1204

// tb_EcoBlock_relation_info 관련
#define				PKCMD_DeleteEcoBlockRelationInfoTB			1300
#define				PKCMD_InsertEcoBlockRelationInfoTB			1301
#define				PKCMD_SelectEcoBlockRelationInfoTB			1302

// tb_EcoBlock_project 관련
#define				PKCMD_CheckExistEbPjt							1400
#define				PKCMD_DeleteEcoBlockPjtTB						1401
#define				PKCMD_IsSuccessInfoChange						1402
#define				PKCMD_UpdateManualEcoBlockProjectTB				1403
#define				PKCMD_UpdateManualEcoBlockProjectTB2			1404
#define				PKCMD_UpdateEcoBlockProjectTB					1405
#define				PKCMD_SelectEcoBlockProjectTB_With_Blob			1406
#define				PKCMD_SelectEcoBlockProjectTB_Without_Blob		1407
#define				PKCMD_SelectEcoBlockProjectTB_Without_Blob2		1408

// tb_EcoBlock_design_point_general_info 관련
#define				PKCMD_DeleteEcoBlockDesPtGeneralInfoTB			1500
#define				PKCMD_InsertEcoBlockDesPtGeneralInfoTB			1501
#define				PKCMD_SelectEcoBlockDesPtGeneralInfoTB			1502

// tb_ecoblock_manual_mes_manager 관련
#define				PKCMD_DeleteEcoBlockManualMesManagerTB			1600
#define				PKCMD_InsertEcoBlockManualMesManagerTB			1601
#define				PKCMD_SelectEcoBlockManualMesManagerTB			1602

// tb_EcoBlock_control_point_coord_info 관련
#define				PKCMD_DeleteEcoBlockControlPointCoordInfoTB		1700
#define				PKCMD_InsertEcoBlockControlPointCoordInfoTB		1701
#define				PKCMD_SelectEcoBlockControlPointCoordInfoTB		1702

// tb_EcoBlock_control_point_distance_info 관련
#define				PKCMD_DeleteEcoBlockControlPointDistanceInfoTB	1800
#define				PKCMD_InsertEcoBlockControlPointDistanceInfoTB	1801
#define				PKCMD_SelectEcoBlockControlPointDistanceInfoTB	1802

// tb_ecoblock_contorl_point_curve_info 관련
#define				PKCMD_DeleteEcoBlockControlPointCurveTB			1900
#define				PKCMD_InsertEcoBlockControlPointCurveTB			1901
#define				PKCMD_SelectEcoBlockControlPointCurveInfoTB		1902

// tb_EcoBlock_control_point_perp_info 관련
#define				PKCMD_DeleteEcoBlockControlPointPerpInfoTB		2000
#define				PKCMD_InsertEcoBlockControlPointPerpInfoTB		2001
#define				PKCMD_SelectEcoBlockControlPointPerpInfoTB		2002

// tb_EcoBlock_control_point_angle_info 관련
#define				PKCMD_DeleteEcoBlockControlPointAngleInfoTB		2100
#define				PKCMD_InsertEcoBlockControlPointAngleInfoTB		2101
#define				PKCMD_SelectEcoBlockControlPointAngleInfoTB		2102

// tb_EcoBlock_control_point_flat_info 관련
#define				PKCMD_DeleteEcoBlockControlPointFlatInfoTB		2200
#define				PKCMD_InsertEcoBlockControlPointFlatInfoTB		2201
#define				PKCMD_SelectEcoBlockControlPointFlatInfoTB		2202

// tb_EcoBlock_camera_info 관련
#define				PKCMD_DeleteEcoBlockCameraInfoTB				2300
#define				PKCMD_InsertEcoBlockCameraInfoTB				2301
#define				PKCMD_SelectEcoBlockCameraInfoTB				2302

// tb_EcoBlock_projection_face_list 관련
#define				PKCMD_DeleteEcoBlockProjectionFaceListTB		2400
#define				PKCMD_InsertEcoBlockProjectionFaceListTB		2401
#define				PKCMD_SelectEcoBlockProjectionFaceListTB		2402

// tb_EcoBlock_add_measure_point_info
#define				PKCMD_DeleteEcoBlockAddMeasurePointInfoTB		2500
#define				PKCMD_InsertEcoBlockAddMeasurePointInfoTB		2501
#define				PKCMD_SelectEcoBlockAddMeasurePointInfoTB		2502

// tb_EcoBlock_delete_measure_point_info
#define				PKCMD_DeleteEcoBlockDeleteMeasurePointInfoTB	2600
#define				PKCMD_InsertEcoBlockDeleteMeasurePointInfoTB	2601
#define				PKCMD_SelectEcoBlockDeleteMeasurePointInfoTB	2602

// tb_system_block_list
#define				PKCMD_GetSystemBlockName						2700
#define				PKCMD_GetBlockNameList							2701
#define				PKCMD_DeleteSystemBlockListTB					2702
#define				PKCMD_InsertSystemBlockListTB					2703
#define				PKCMD_UpdateSystemBlockListTB					2704
#define				PKCMD_SelectSystemBlockListTB					2705

// tb_ecoots_project
#define				PKCMD_DeleteEcoOtsProjectTB						2800
#define				PKCMD_UpdateEcoOtsProjectTBOnlyTree				2801
#define				PKCMD_InsertEcoOtsProjectTB						2802
#define				PKCMD_SelectEcoOtsProjectTB_WithOut_Blob		2803
#define				PKCMD_SelectEcoOtsProjectTB_With_Blob			2804
#define				PKCMD_SelectLastInsertIDEcoOtsProjectTB			2805
#define				PKCMD_SelectLastInsertIDEcoOtsProjectTBByIdentCurrent 2806

// tb_ecoots_block_list
#define				PKCMD_DeleteEcoOtsBlockListTB					2900
#define				PKCMD_InsertEcoOtsBlockListTB					2901
#define				PKCMD_SelectEcoOtsBlockListTB_WithOut_Blob		2902
#define				PKCMD_SelectEcoOtsBlockListTB_With_Blob			2903

// tb_ecoots_add_measure_point_info
#define				PKCMD_DeleteEcoOtsAddMeasurePointInfoTB			3000
#define				PKCMD_InsertEcoOtsAddMeasurePointInfoTB			3001
#define				PKCMD_SelectEcoOtsAddMeasurePointInfoTB			3002

// tb_ecoots_delete_measure_point_info
#define				PKCMD_DeleteEcoOtsDeleteMeasurePointInfoTB		3100
#define				PKCMD_InsertEcoOtsDeleteMeasurePointInfoTB		3101
#define				PKCMD_SelectEcoOtsDeleteMeasurePointInfoTB		3102

// tb_ecoots_design_point_general_info
#define				PKCMD_DeleteEcoOtsDesPtGeneralInfoTB			3200
#define				PKCMD_InsertEcoOtsDesPtGeneralInfoTB			3201
#define				PKCMD_SelectEcoOtsDesPtGeneralInfoTB			3202

// tb_ecoots_relation_by_des_msr
#define				PKCMD_DeleteEcoOtsRelationByDesMsrTB			3300
#define				PKCMD_InsertEcoOtsRelationByDesMsrTB			3301
#define				PKCMD_SelectEcoOtsRelationByDesMsrTB			3302

// tb_ecoots_relation_by_msr_msr
#define				PKCMD_DeleteEcoOtsRelationByMsrMsrTB			3400
#define				PKCMD_InsertEcoOtsRelationByMsrMsrTB			3401
#define				PKCMD_SelectEcoOtsRelationByMsrMsrTB			3402

// tb_ecoots_point_cond
#define				PKCMD_DeleteEcoOtsPointCondTB					3500
#define				PKCMD_InsertEcoOtsPointCondTB					3501
#define				PKCMD_SelectEcoOtsPointCondTB					3502

// tb_ecoots_distance_cond
#define				PKCMD_DeleteEcoOtsDistanceCondTB				3600
#define				PKCMD_InsertEcoOtsDistanceCondTB				3601
#define				PKCMD_SelectEcoOtsDistanceCondTB				3602

// tb_ecoots_drawing_element
#define				PKCMD_DeleteEcoOtsDrawingElementTB				3700
#define				PKCMD_InsertEcoOtsDrawingElementTB				3701
#define				PKCMD_SelectEcoOtsDrawingElementTB_With_Blob	3702

// tb_ecoots_camera_info 관련
#define				PKCMD_DeleteEcoOtsCameraInfoTB					3800
#define				PKCMD_InsertEcoOtsCameraInfoTB					3801
#define				PKCMD_SelectEcoOtsCameraInfoTB					3802

// tb_ecoots_projection_face_list
#define				PKCMD_DeleteEcoOtsProjectionFaceListTB			3900
#define				PKCMD_InsertEcoOtsProjectionFaceListTB			3901
#define				PKCMD_SelectEcoOtsProjectionFaceListTB			3902

// tb_ecoots_projection_face_modeling_matrix_list
#define				PKCMD_DeleteEcoOtsProjectionFaceModelingMatrixListTB		4000
#define				PKCMD_InsertEcoOtsProjectionFaceModelingMatrixListTB		4001
#define				PKCMD_SelectEcoOtsProjectionFaceModelingMatrixListListTB	4002

// tb_assembly_level
#define				PKCMD_DeleteAssemblyLevelTB						4100
#define				PKCMD_InsertAssemblyLevelTB						4101
#define				PKCMD_UpdateAssemblyLevelTB						4102
#define				PKCMD_SelectAssemblyLevelTB						4103

// tb_assembly_tree
#define				PKCMD_DeleteAssemblyTreeTB						4200
#define				PKCMD_InsertAssemblyTreeTB						4201
#define				PKCMD_UpdateAssemblyTreeTB						4202
#define				PKCMD_SelectAssemblyTreeTB						4203
#define				PKCMD_SelectLastInsertIDAssemblyTreeTB			4204

// tb_variable_group
#define				PKCMD_DeleteVariableGroupTB						4300
#define				PKCMD_InsertVariableGroupTB						4301
#define				PKCMD_UpdateVariableGroupTB						4302
#define				PKCMD_SelectVariableGroupTB						4303
#define				PKCMD_SelectLastInsertIDVariableGroupTB			4304

// tb_variable_of_group
#define				PKCMD_DeleteVariableOfGroupTB					4400
#define				PKCMD_DeleteVariableOfGroupTBByGroupID			4401
#define				PKCMD_DeleteVariableOfGroupTBByVarID			4402
#define				PKCMD_InsertVariableOfGroupTB					4403
#define				PKCMD_UpdateVariableOfGroupTB					4404
#define				PKCMD_SelectVariableOfGroupTB					4405
#define				PKCMD_SelectLastInsertIDVariableOfGroupTB		4406

// tb_user_variable
#define				PKCMD_DeleteUserVariableTB						4500
#define				PKCMD_InsertUserVariableTB						4501
#define				PKCMD_UpdateUserVariableTB						4502
#define				PKCMD_SelectUserVariableTB						4503
#define				PKCMD_SelectLastInsertIDUserVariableTB			4504


// tb_project_property
#define				PKCMD_DeleteProjectPropertyTB_ID				4600
#define				PKCMD_DeleteProjectPropertyTB_Where				4601
#define				PKCMD_InsertProjectPropertyTB					4602
#define				PKCMD_UpdateProjectPropertyTB					4603
#define				PKCMD_SelectProjectPropertyTB					4604

// tb_block_property
#define				PKCMD_DeleteBlockPropertyTB						4700
#define				PKCMD_InsertBlockPropertyTB						4701
#define				PKCMD_UpdateBlockPropertyTB						4702
#define				PKCMD_SelectBlockPropertyTB						4703
#define				PKCMD_DeleteAllBlockPropertyTB					4704

// tb_shipno_property
#define				PKCMD_DeleteShipnoPropertyTB					4800
#define				PKCMD_InsertShipnoPropertyTB					4801
#define				PKCMD_SelectShipnoPropertyTB					4802

// tb_ecoblock_project_tree
#define				PKCMD_DeleteEcoblockProjectTreeTB				4900
#define				PKCMD_InsertEcoblockProjectTreeTB				4901
#define				PKCMD_UpdateEcoblockProjectTreeTB				4902
#define				PKCMD_SelectEcoblockProjectTreeTB				4903
#define				PKCMD_SelectLastInsertIDProjectTreeTB			4904

// tb_user_input_block_list
#define				PKCMD_DeleteUserInputBlockListTB_ID				5000
#define				PKCMD_DeleteUserInputBlockListTB_NAME			5001
#define				PKCMD_InsertUserInputBlockListTB				5002
#define				PKCMD_SelectUserInputBlockListTB				5003

// tb_upload_file
#define				PKCMD_DeleteUploadFile_ID						5100
#define				PKCMD_DeleteUploadFile_Where					5101
#define				PKCMD_InsertUploadFile							5102
#define				PKCMD_SelectUploadFile_With_Blob				5103
#define				PKCMD_SelectUploadFile_WithOut_Blob				5104

// tb_auto_update_file
#define				PKCMD_DeleteAutoUpdateFileTB					5200
#define				PKCMD_DeleteAutoUpdateFileTB_Platform			5201
#define				PKCMD_InsertAutoUpdateFileTB					5202
#define				PKCMD_SelectAutoUpdateFileTB_WithOut_Blob		5203
#define				PKCMD_SelectAutoUpdateFileTB_With_Blob			5204

//tb_concurrent_info 관련
#define				PKCMD_GetFeatureID								5300

//server manager 부분
// tb_ecomodel_???
//update
//tribon -> modelserver
#define				PKCMD_UpdateToModelServer						5400
#define				PKCMD_UpdateToModelServer_Hsf					5401

//tb_system_drawing, tb_ecomodel_???
#define				PKCMD_GetModelListFromModelServerBy_Q_Index		5500
#define				PKCMD_SelectModeldelete							5501
//모델 copy
#define				PKCMD_SelectModelCopy							5502
//delete
//테이블 자체를 삭제
#define				PKCMD_DeleteTableData							5503
//데이터만 모두 삭제
#define				PKCMD_DeleteAllModel							5504
//select
#define				PKCMD_GetModelFromModelServer					5505
#define				PKCMD_GetModelFromModelServerByHsf				5506
#define				PKCMD_GetOtherCadModelFromModelServer			5507
#define				PKCMD_GetModelListFromModelServer				5508
#define				PKCMD_ModelTypetoQindex							5509

//insert
#define				PKCMD_InsertModelAfterDeleteByQIndex			5510
//Revision
#define				PKCMD_CompareRevisionDate						5511
#define				PKCMD_DeleteModelByRevision						5512


// table복사관련 함수 추가
#define				PKCMD_CreateEcoBlockRelationInfoTB_byCopy		5600
#define				PKCMD_CreateEcoBlockAssemblylevelTB_byCopy		5601

// tb_user_template_info
#define				PKCMD_SelectUserTemplateInfo					5700
#define				PKCMD_InsertUserTemplateInfo					5701
#define				PKCMD_UpdateUserTemplateInfo					5702


//MEI관련
//MEI tb_ecomodel_???
#define				PKCMD_InsertEcoModelTB							5800
#define				PKCMD_DeleteEcoModelTB							5801

// tb_ecomodel_manager
#define				PKCMD_InsertEcoModelManager						5900
#define				PKCMD_SelectEcoModelManager						5901

// tb_hierarchy_tree
#define				PKCMD_SelectHierarchytreeTB						6000
#define				PKCMD_InsertHierarchytreeTB						6001

// HHI JT관련
#define				PKCMD_SelectJTShipNoListinfo					6100
#define				PKCMD_SelectJTHierarchyinfo						6101
#define				PKCMD_SelectandFTPTransferJTFile				6102


