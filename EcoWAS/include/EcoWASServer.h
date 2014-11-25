#pragma once

#include "EcoServerBinary.h"
#include "EcoSocketServerManager.h"
#include "FtpClientManager.h"
#include "EcoWASDataBaseManager.h"


class AFX_EXT_API CEcoWASServer
{
public:
	CEcoWASServer(void);
	~CEcoWASServer(void);

private:
	//Client socket관련
	CString	m_strSocketClientIP;
	int	m_nSocketClientPort;
	//DB관련
	int m_nDB;
	CString m_strDBIP;
	int m_nDBPort;
	CString m_strDBUserID;
	CString m_strDBPwd;
	CString m_strDBName;
	//EcoServerBinary관련
	CString m_strEcoTmpRootPath;
	CString m_strEcoNasRootPath;
	//FTP 관련
	CString m_strFtpIP;
	CString m_strFtpUserID;
	CString m_strFtpPwd;
	int m_nFtpPort;

private:
	CEcoServerBinary m_ecoServerBinary;
	CEcoSocketServerManager m_ecoSocketServer;
	CFtpClientManager m_ecoFtpClient;
	CEcoWASDataBaseManager m_ecoDBManager;


private:
	CString GetPath(CString strFilePath);
	DWORD GetFileSize(CString strFilePathName);

private:
	BOOL CommandProcessing(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	//tb_ecomodel_error_list
	BOOL CmdProcEcoModelErrorList(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	//테이블데이터 중복검사
	BOOL CmdProcDuplicateData(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	//DB 생성
	BOOL CmdProcCreateDatabase(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	//Block, 모델, 기타 테이블 생성관련
	BOOL CmdProcCreateBlockModelEtcTB(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	//OTS용 테이블 생성관련
	BOOL CmdProcCreateOtsTB(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	//PM용 테이블 생성관련
	BOOL CmdProcCreatePMTB(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	// tb_user_info
	BOOL CmdProcUserInfo(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	//tb_shipno_info
	BOOL CmdProcShipNoInfo(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	// tb_drawing_template
	BOOL CmdProcDrawingTemplate(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	// tb_system_drawing
	BOOL CmdProcSystemDrawing(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	// tb_project
	BOOL CmdProcProject(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	// tb_measure_point
	BOOL CmdProcMeasurePoint(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	// tb_EcoBlock_relation_info
	BOOL CmdProcEcoBlockRelationInfo(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	// tb_EcoBlock_project
	BOOL CmdProcEcoBlockProject(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	// tb_EcoBlock_design_point_general_info
	BOOL CmdProcEcoBlockDsnPtGeneralInfo(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	// tb_ecoblock_manual_mes_manager
	BOOL CmdProcEcoBlockManualMesManager(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	// tb_EcoBlock_control_point_coord_info
	BOOL CmdProcEcoBlockCtrlPtCoordInfo(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	// tb_EcoBlock_control_point_distance_info
	BOOL CmdProcEcoBlockCtrlPtDistanceInfo(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	// tb_ecoblock_contorl_point_curve_info
	BOOL CmdProcEcoBlockCtrlPtCurveInfo(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	// tb_EcoBlock_control_point_perp_info
	BOOL CmdProcEcoBlockCtrlPtPerpInfo(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	// tb_EcoBlock_control_point_angle_info
	BOOL CmdProcEcoBlockCtrlPtAngleInfo(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	// tb_EcoBlock_control_point_flat_info
	BOOL CmdProcEcoBlockCtrlPtFlatInfo(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	// tb_EcoBlock_camera_info
	BOOL CmdProcEcoBlockCameraInfo(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	// tb_EcoBlock_projection_face_list
	BOOL CmdProcEcoBlockProjectionFaceList(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	// tb_EcoBlock_add_measure_point_info
	BOOL CmdProcEcoBlockAddMesPtInfo(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	// tb_EcoBlock_delete_measure_point_info
	BOOL CmdProcEcoBlockDeleteMesPtInfo(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	// tb_system_block_list
	BOOL CmdProcEcoBlockSystemBlockList(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	// tb_ecoots_project
	BOOL CmdProcEcoOtsProject(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	// tb_ecoots_block_list
	BOOL CmdProcEcoOtsBlockList(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	// tb_ecoots_add_measure_point_info
	BOOL CmdProcEcoOtsAddMesPtInfo(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	// tb_ecoots_delete_measure_point_info
	BOOL CmdProcEcoOtsDeleteMesPtInfo(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	// tb_ecoots_design_point_general_info
	BOOL CmdProcEcoOtsDsnPtGeneralInfo(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	// tb_ecoots_relation_by_des_msr
	BOOL CmdProcEcoOtsRelationByDesMsr(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	// tb_ecoots_relation_by_msr_msr
	BOOL CmdProcEcoOtsRelationByMsrMsr(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	// tb_ecoots_point_cond
	BOOL CmdProcEcoOtsPointCond(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	// tb_ecoots_distance_cond
	BOOL CmdProcEcoOtsDistanceCond(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	// tb_ecoots_drawing_element
	BOOL CmdProcEcoOtsDrawingElement(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	// tb_ecoots_camera_info
	BOOL CmdProcEcoOtsCameraInfo(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	// tb_ecoots_projection_face_list
	BOOL CmdProcEcoOtsProjectionFaceList(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	// tb_ecoots_projection_face_modeling_matrix_list
	BOOL CmdProcEcoOtsProjectionFaceMMList(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	// tb_assembly_level
	BOOL CmdProcCAssemblyLevel(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	// tb_assembly_tree
	BOOL CmdProcAssemblyTree(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	// tb_variable_group
	BOOL CmdProcVariableGroup(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	// tb_variable_of_group
	BOOL CmdProcVariableOfGroup(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	// tb_user_variable
	BOOL CmdProcUserVariable(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	// tb_project_property
	BOOL CmdProcProjectProperty(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	// tb_block_property
	BOOL CmdProcBlockProperty(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	// tb_shipno_property
	BOOL CmdProcShipNoProperty(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	// tb_ecoblock_project_tree
	BOOL CmdProcEcoBlockProjectTree(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	// tb_user_input_block_list
	BOOL CmdProcUserInputBlockList(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	// tb_upload_file
	BOOL CmdProcUploadFile(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	// tb_auto_update_file
	BOOL CmdProcCAutoUpdateFile(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	//tb_concurrent_info
	BOOL CmdProcConcurrentInfo(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	// tribon -> tb_ecomodel_???
	BOOL CmdProcTribonToEcoModel(CEcoPacket& recvPacket, CEcoPacket& sendPacket);
	
	// tb_ecomodel_???
	BOOL CmdProcEcoModel(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	// table복사관련
	BOOL CmdProcTableCopy(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	// tb_user_template_info
	BOOL CmdProcUserTemplateInfo(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	//MEI tb_ecomodel_???
	BOOL CmdProcMEIEcoModel(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	//MEI tb_ecomodel_manager
	BOOL CmdProcMEIEcoModelManager(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	//MEI tb_hierarchy_tree
	BOOL CmdProcMEIHierarchyTree(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	// HHI Jt관련
	BOOL CmdProcJTServerModelInfo_HHI(CEcoPacket& recvPacket, CEcoPacket& sendPacket);

	// sample TEST
	void TESTFUNC();
	



public:
	BOOL Start(void);

	void SetClientSocketInfo(CString strIP, int nPort);
	void SetDataBaseInfo(int nDB, CString strIP, int nPort, CString strID, CString strPwd, CString strDBName);
	void SetFtpInfo(CString strIP, CString strID, CString strPwd, int nPort = 21);
	void SetEcoTmpRootPath(CString strPath);
	void SetEcoNasRootPath(CString strPath);
};

