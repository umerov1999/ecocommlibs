#pragma once

#include "EcoPacketEntity.h"

class CEcoPacket
{
public:
	CEcoPacket(void);
	~CEcoPacket(void);

	CEcoPacket& operator = (const CEcoPacket &other);
	CEcoPacket(const CEcoPacket &s);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & m_nCommand;

		std::string stdStr;
		if(m_bSendRecvPacket == TRUE)
		{
			stdStr = CStringConverter::CStringWToCStringA(m_strUserID);
			ar & stdStr;
		}
		else
		{
			ar & stdStr;
			m_strUserID = CStringConverter::CStringAToCStringW(stdStr.c_str());
		}

		ar & m_bNetFolderInfo;

		ar & m_bCreateTable;
		ar & m_bDuplicateData;
		ar & m_bTableCopy;

		ar & m_bShipNoInfo;
		ar & m_bDrawingTemplate;
		ar & m_bUserTemplateInfo;
		ar & m_bUserInfo;
		ar & m_bAutoUpdateFile;
		ar & m_bConcurrentInfo;

		ar & m_bProject;
		ar & m_bEcoBlockProjectTree;
		ar & m_bMeasurePoint;
		ar & m_bEcoBlockProject;
		ar & m_bEcoBlockRelaInfo;
		ar & m_bEcoBlockManualMesManager;
		ar & m_bEcoBlockDsnPtGenInfo;
		ar & m_bEcoBlockCtrlPtCoordInfo;
		ar & m_bEcoBlockCtrlPtDistanceInfo;
		ar & m_bEcoBlockCtrlPtCurveInfo;
		ar & m_bEcoBlockCtrlPtPerpInfo;
		ar & m_bEcoBlockCtrlPtAngleInfo;
		ar & m_bEcoBlockCtrlPtFlatInfo;
		ar & m_bEcoBlockCameraInfo;
		ar & m_bEcoBlockProjFaceList;
		ar & m_bEcoBlockAddMsrPtInfo;
		ar & m_bEcoBlockDeleteMstrPtInfo;
		ar & m_bSystemDrawing;
		ar & m_bSystemBlockList;

		ar & m_bEcoOtsBlockList;
		ar & m_bEcoOtsProject;
		ar & m_bEcoOtsAddMsrPtInfo;
		ar & m_bEcoOtsDeleteMsrPtInfo;
		ar & m_bEcoOtsDesignPointGenInfo;
		ar & m_bEcoOtsRelaByDesMsr;
		ar & m_bEcoOtsRelaByMsrMsr;
		ar & m_bEcoOtsDistCond;
		ar & m_bEcoOtsPointCond;
		ar & m_bEcoOtsDrawingElement;
		ar & m_bEcoOtsProjFaceList;
		ar & m_bEcoOtsCameraInfo;
		ar & m_bEcoOtsProjFaceMMList;

		ar & m_bAssemblyLevel;
		ar & m_bAssemblyTree;
		ar & m_bVariableOfGroup;
		ar & m_bProjectProperty;
		ar & m_bShipNoProperty;
		ar & m_bVariableGroup;
		ar & m_bUserVariable;
		ar & m_bBlockProperty;
		ar & m_bUploadFile;
		ar & m_bUserInputBlockList;

		ar & m_bEcoModel;
		ar & m_bTribonToEcoModel;
		ar & m_bEcoModelErrorList;

		ar & m_bCreateMEIModelTable;
		ar & m_bMEIEcoModel;
		ar & m_bMEIEcoModelManager;
		ar & m_bMEIHierarchyTree;

		ar & m_bJTServerProperty;

		if(m_bNetFolderInfo == TRUE)
			m_pkNetFolderInfo.serialize(ar, version, m_bSendRecvPacket);

		if(m_bCreateTable == TRUE)
			m_pkCreateTable.serialize(ar, version, m_bSendRecvPacket);
		if(m_bDuplicateData == TRUE)
			m_pkDuplicateData.serialize(ar, version, m_bSendRecvPacket);
		if(m_bTableCopy == TRUE)
			m_pkTableCopy.serialize(ar, version, m_bSendRecvPacket);

		if(m_bShipNoInfo == TRUE)
			m_pkShipNoInfo.serialize(ar, version, m_bSendRecvPacket);
		if(m_bDrawingTemplate == TRUE)
			m_pkDrawingTemplate.serialize(ar, version, m_bSendRecvPacket);
		if(m_bUserTemplateInfo == TRUE)
			m_pkUserTemplateInfo.serialize(ar, version, m_bSendRecvPacket);
		if(m_bUserInfo == TRUE)
			m_pkUserInfo.serialize(ar, version, m_bSendRecvPacket);
		if(m_bAutoUpdateFile == TRUE)
			m_pkAutoUpdateFile.serialize(ar, version, m_bSendRecvPacket);
		if(m_bConcurrentInfo == TRUE)
			m_pkConcurrentInfo.serialize(ar, version, m_bSendRecvPacket);

		if(m_bProject == TRUE)
			m_pkProject.serialize(ar, version, m_bSendRecvPacket);
		if(m_bEcoBlockProjectTree == TRUE)
			m_pkEcoBlockProjectTree.serialize(ar, version, m_bSendRecvPacket);
		if(m_bMeasurePoint == TRUE)
			m_pkMeasurePoint.serialize(ar, version, m_bSendRecvPacket);
		if(m_bEcoBlockProject == TRUE)
			m_pkEcoBlockProject.serialize(ar, version, m_bSendRecvPacket);
		if(m_bEcoBlockRelaInfo == TRUE)
			m_pkEcoBlockRelaInfo.serialize(ar, version, m_bSendRecvPacket);
		if(m_bEcoBlockManualMesManager == TRUE)
			m_pkEcoBlockManualMesManager.serialize(ar, version, m_bSendRecvPacket);
		if(m_bEcoBlockDsnPtGenInfo == TRUE)
			m_pkEcoBlockDsnPtGenInfo.serialize(ar, version, m_bSendRecvPacket);
		if(m_bEcoBlockCtrlPtCoordInfo == TRUE)
			m_pkEcoBlockCtrlPtCoordInfo.serialize(ar, version, m_bSendRecvPacket);
		if(m_bEcoBlockCtrlPtDistanceInfo == TRUE)
			m_pkEcoBlockCtrlPtDistanceInfo.serialize(ar, version, m_bSendRecvPacket);
		if(m_bEcoBlockCtrlPtCurveInfo == TRUE)
			m_pkEcoBlockCtrlPtCurveInfo.serialize(ar, version, m_bSendRecvPacket);
		if(m_bEcoBlockCtrlPtPerpInfo == TRUE)
			m_pkEcoBlockCtrlPtPerpInfo.serialize(ar, version, m_bSendRecvPacket);
		if(m_bEcoBlockCtrlPtAngleInfo == TRUE)
			m_pkEcoBlockCtrlPtAngleInfo.serialize(ar, version, m_bSendRecvPacket);
		if(m_bEcoBlockCtrlPtFlatInfo == TRUE)
			m_pkEcoBlockCtrlPtFlatInfo.serialize(ar, version, m_bSendRecvPacket);
		if(m_bEcoBlockCameraInfo == TRUE)
			m_pkEcoBlockCameraInfo.serialize(ar, version, m_bSendRecvPacket);
		if(m_bEcoBlockProjFaceList == TRUE)
			m_pkEcoBlockProjFaceList.serialize(ar, version, m_bSendRecvPacket);
		if(m_bEcoBlockAddMsrPtInfo == TRUE)
			m_pkEcoBlockAddMsrPtInfo.serialize(ar, version, m_bSendRecvPacket);
		if(m_bEcoBlockDeleteMstrPtInfo == TRUE)
			m_pkEcoBlockDeleteMstrPtInfo.serialize(ar, version, m_bSendRecvPacket);
		if(m_bSystemDrawing == TRUE)
			m_pkSystemDrawing.serialize(ar, version, m_bSendRecvPacket);
		if(m_bSystemBlockList == TRUE)
			m_pkSystemBlockList.serialize(ar, version, m_bSendRecvPacket);

		if(m_bEcoOtsBlockList == TRUE)
			m_pkEcoOtsBlockList.serialize(ar, version, m_bSendRecvPacket);
		if(m_bEcoOtsProject == TRUE)
			m_pkEcoOtsProject.serialize(ar, version, m_bSendRecvPacket);
		if(m_bEcoOtsAddMsrPtInfo == TRUE)
			m_pkEcoOtsAddMsrPtInfo.serialize(ar, version, m_bSendRecvPacket);
		if(m_bEcoOtsDeleteMsrPtInfo == TRUE)
			m_pkEcoOtsDeleteMsrPtInfo.serialize(ar, version, m_bSendRecvPacket);
		if(m_bEcoOtsDesignPointGenInfo == TRUE)
			m_pkEcoOtsDesignPointGenInfo.serialize(ar, version, m_bSendRecvPacket);
		if(m_bEcoOtsRelaByDesMsr == TRUE)
			m_pkEcoOtsRelaByDesMsr.serialize(ar, version, m_bSendRecvPacket);
		if(m_bEcoOtsRelaByMsrMsr == TRUE)
			m_pkEcoOtsRelaByMsrMsr.serialize(ar, version, m_bSendRecvPacket);
		if(m_bEcoOtsDistCond == TRUE)
			m_pkEcoOtsDistCond.serialize(ar, version, m_bSendRecvPacket);
		if(m_bEcoOtsPointCond == TRUE)
			m_pkEcoOtsPointCond.serialize(ar, version, m_bSendRecvPacket);
		if(m_bEcoOtsDrawingElement == TRUE)
			m_pkEcoOtsDrawingElement.serialize(ar, version, m_bSendRecvPacket);
		if(m_bEcoOtsProjFaceList == TRUE)
			m_pkEcoOtsProjFaceList.serialize(ar, version, m_bSendRecvPacket);
		if(m_bEcoOtsCameraInfo == TRUE)
			m_pkEcoOtsCameraInfo.serialize(ar, version, m_bSendRecvPacket);
		if(m_bEcoOtsProjFaceMMList == TRUE)
			m_pkEcoOtsProjFaceMMList.serialize(ar, version, m_bSendRecvPacket);

		if(m_bAssemblyLevel == TRUE)
			m_pkAssemblyLevel.serialize(ar, version, m_bSendRecvPacket);
		if(m_bAssemblyTree == TRUE)
			m_pkAssemblyTree.serialize(ar, version, m_bSendRecvPacket);
		if(m_bVariableOfGroup == TRUE)
			m_pkVariableOfGroup.serialize(ar, version, m_bSendRecvPacket);
		if(m_bProjectProperty == TRUE)
			m_pkProjectProperty.serialize(ar, version, m_bSendRecvPacket);
		if(m_bShipNoProperty == TRUE)
			m_pkShipNoProperty.serialize(ar, version, m_bSendRecvPacket);
		if(m_bVariableGroup == TRUE)
			m_pkVariableGroup.serialize(ar, version, m_bSendRecvPacket);
		if(m_bUserVariable == TRUE)
			m_pkUserVariable.serialize(ar, version, m_bSendRecvPacket);
		if(m_bBlockProperty == TRUE)
			m_pkBlockProperty.serialize(ar, version, m_bSendRecvPacket);
		if(m_bUploadFile == TRUE)
			m_pkUploadFile.serialize(ar, version, m_bSendRecvPacket);
		if(m_bUserInputBlockList == TRUE)
			m_pkUserInputBlockList.serialize(ar, version, m_bSendRecvPacket);

		if(m_bEcoModel == TRUE)
			m_pkEcoModel.serialize(ar, version, m_bSendRecvPacket);
		if(m_bTribonToEcoModel == TRUE)
			m_pkTribonToEcoModel.serialize(ar, version, m_bSendRecvPacket);
		if(m_bEcoModelErrorList == TRUE)
			m_pkEcoModelErrorList.serialize(ar, version, m_bSendRecvPacket);

		if(m_bCreateMEIModelTable == TRUE)
			m_pkCreateMEIModelTable.serialize(ar, version, m_bSendRecvPacket);
		if(m_bMEIEcoModel == TRUE)
			m_pkMEIEcoModel.serialize(ar, version, m_bSendRecvPacket);
		if(m_bMEIEcoModelManager == TRUE)
			m_pkMEIEcoModelManager.serialize(ar, version, m_bSendRecvPacket);
		if(m_bMEIHierarchyTree == TRUE)
			m_pkMEIHierarchyTree.serialize(ar, version, m_bSendRecvPacket);

		if(m_bJTServerProperty == TRUE)
			m_pkJTServerProperty.serialize(ar, version, m_bSendRecvPacket);
	}

private:
	int m_nCommand;
	BOOL m_bSendRecvPacket;
	CString m_strUserID;

private:
//설정정보
	CPKNetFolderInfo	m_pkNetFolderInfo;

//기타 Packet들
	CPKCreateTable		m_pkCreateTable;
	CPKDuplicateData	m_pkDuplicateData;
	CPKTableCopy		m_pkTableCopy;

//Common Table 관련 Packet들
	CPKShipNoInfo		m_pkShipNoInfo;
	CPKDrawingTemplate	m_pkDrawingTemplate;
	CPKUserTemplateInfo	m_pkUserTemplateInfo;
	CPKUserInfo			m_pkUserInfo;
	CPKAutoUpdateFile	m_pkAutoUpdateFile;
	CPKConcurrentInfo	m_pkConcurrentInfo;

//EcoBlock 프로젝트 관련 Packet들
	CPKProject							m_pkProject;
	CPKEcoBlockProjectTree				m_pkEcoBlockProjectTree;
	CPKMeasurePoint						m_pkMeasurePoint;
	CPKEcoBlockProject					m_pkEcoBlockProject;
	CPKEcoBlockRelationInfo				m_pkEcoBlockRelaInfo;
	CPKEcoBlockManualMesManager			m_pkEcoBlockManualMesManager;
	CPKEcoBlockDesignPointGeneralInfo	m_pkEcoBlockDsnPtGenInfo;
	CPKEcoBlockControlPointCoordInfo	m_pkEcoBlockCtrlPtCoordInfo;
	CPKEcoBlockControlPointDistanceInfo	m_pkEcoBlockCtrlPtDistanceInfo;
	CPKEcoBlockContorlPointCurveInfo	m_pkEcoBlockCtrlPtCurveInfo;
	CPKEcoBlockControlPointPerpInfo		m_pkEcoBlockCtrlPtPerpInfo;
	CPKEcoBlockControlPointAngleInfo	m_pkEcoBlockCtrlPtAngleInfo;
	CPKEcoBlockControlPointFlatInfo		m_pkEcoBlockCtrlPtFlatInfo;
	CPKEcoBlockCameraInfo				m_pkEcoBlockCameraInfo;
	CPKEcoBlockProjectionFaceList		m_pkEcoBlockProjFaceList;
	CPKEcoBlockAddMeasurePointInfo		m_pkEcoBlockAddMsrPtInfo;
	CPKEcoBlockDeleteMeasurePointInfo	m_pkEcoBlockDeleteMstrPtInfo;
	CPKSystemDrawing					m_pkSystemDrawing;
	CPKSystemBlockList					m_pkSystemBlockList;

//EcoOTS 프로젝트 관련 Packet들
	CPKEcoOtsBlockList							m_pkEcoOtsBlockList;
	CPKEcoOtsProject							m_pkEcoOtsProject;
	CPKEcoOtsAddMeasurePointInfo				m_pkEcoOtsAddMsrPtInfo;
	CPKEcoOtsDeleteMeasurePointInfo				m_pkEcoOtsDeleteMsrPtInfo;
	CPKEcoOtsDesignPointGeneralInfo				m_pkEcoOtsDesignPointGenInfo;
	CPKEcoOtsRelationByDesMsr					m_pkEcoOtsRelaByDesMsr;
	CPKEcoOtsRelationByMsrMsr					m_pkEcoOtsRelaByMsrMsr;
	CPKEcoOtsDistanceCond						m_pkEcoOtsDistCond;
	CPKEcoOtsPointCond							m_pkEcoOtsPointCond;
	CPKEcoOtsDrawingElement						m_pkEcoOtsDrawingElement;
	CPKEcoOtsProjectionFaceList					m_pkEcoOtsProjFaceList;
	CPKEcoOtsCameraInfo							m_pkEcoOtsCameraInfo;
	CPKEcoOtsProjectionFaceModelingMatrixList	m_pkEcoOtsProjFaceMMList;

//Production관련 Packet들
	CPKAssemblyLevel		m_pkAssemblyLevel;
	CPKAssemblyTree			m_pkAssemblyTree;
	CPKVariableOfGroup		m_pkVariableOfGroup;
	CPKProjectProperty		m_pkProjectProperty;
	CPKShipNoProperty		m_pkShipNoProperty;
	CPKVariableGroup		m_pkVariableGroup;
	CPKUserVariable			m_pkUserVariable;
	CPKBlockProperty		m_pkBlockProperty;
	CPKUploadFile			m_pkUploadFile;
	CPKUserInputBlockList	m_pkUserInputBlockList;

//모델관련 Packet들
	CPKEcoModel				m_pkEcoModel;
	CPKTribonToEcoModel		m_pkTribonToEcoModel;
	CPKEcoModelErrorList	m_pkEcoModelErrorList;

//MEI모델관련 Packet들
	CPKCreateMEIModelTable	m_pkCreateMEIModelTable;
	CPKMEIEcoModel			m_pkMEIEcoModel;
	CPKMEIEcoModelManager	m_pkMEIEcoModelManager;
	CPKMEIHierarchyTree		m_pkMEIHierarchyTree;

	//-->2014-11-20 최조영 JT관련 코드 추가 (현대전용)
	CPKJTServerProperty     m_pkJTServerProperty;
	//<--2014-11-20 최조영 JT관련 코드 추가

	//-->2015-02-12 손지훈 Assembly Tree 동기화 관련 코드 추가(현대전용)


private:
	BOOL m_bNetFolderInfo;

	BOOL m_bCreateTable;
	BOOL m_bDuplicateData;
	BOOL m_bTableCopy;

	BOOL m_bShipNoInfo;
	BOOL m_bDrawingTemplate;
	BOOL m_bUserTemplateInfo;
	BOOL m_bUserInfo;
	BOOL m_bAutoUpdateFile;
	BOOL m_bConcurrentInfo;

	BOOL m_bProject;
	BOOL m_bEcoBlockProjectTree;
	BOOL m_bMeasurePoint;
	BOOL m_bEcoBlockProject;
	BOOL m_bEcoBlockRelaInfo;
	BOOL m_bEcoBlockManualMesManager;
	BOOL m_bEcoBlockDsnPtGenInfo;
	BOOL m_bEcoBlockCtrlPtCoordInfo;
	BOOL m_bEcoBlockCtrlPtDistanceInfo;
	BOOL m_bEcoBlockCtrlPtCurveInfo;
	BOOL m_bEcoBlockCtrlPtPerpInfo;
	BOOL m_bEcoBlockCtrlPtAngleInfo;
	BOOL m_bEcoBlockCtrlPtFlatInfo;
	BOOL m_bEcoBlockCameraInfo;
	BOOL m_bEcoBlockProjFaceList;
	BOOL m_bEcoBlockAddMsrPtInfo;
	BOOL m_bEcoBlockDeleteMstrPtInfo;
	BOOL m_bSystemDrawing;
	BOOL m_bSystemBlockList;

	BOOL m_bEcoOtsBlockList;
	BOOL m_bEcoOtsProject;
	BOOL m_bEcoOtsAddMsrPtInfo;
	BOOL m_bEcoOtsDeleteMsrPtInfo;
	BOOL m_bEcoOtsDesignPointGenInfo;
	BOOL m_bEcoOtsRelaByDesMsr;
	BOOL m_bEcoOtsRelaByMsrMsr;
	BOOL m_bEcoOtsDistCond;
	BOOL m_bEcoOtsPointCond;
	BOOL m_bEcoOtsDrawingElement;
	BOOL m_bEcoOtsProjFaceList;
	BOOL m_bEcoOtsCameraInfo;
	BOOL m_bEcoOtsProjFaceMMList;

	BOOL m_bAssemblyLevel;
	BOOL m_bAssemblyTree;
	BOOL m_bVariableOfGroup;
	BOOL m_bProjectProperty;
	BOOL m_bShipNoProperty;
	BOOL m_bVariableGroup;
	BOOL m_bUserVariable;
	BOOL m_bBlockProperty;
	BOOL m_bUploadFile;
	BOOL m_bUserInputBlockList;

	BOOL m_bEcoModel;
	BOOL m_bTribonToEcoModel;
	BOOL m_bEcoModelErrorList;

	BOOL m_bCreateMEIModelTable;
	BOOL m_bMEIEcoModel;
	BOOL m_bMEIEcoModelManager;
	BOOL m_bMEIHierarchyTree;

	//-->2014-11-20 최조영 JT관련 코드 추가 (현대전용)
	BOOL m_bJTServerProperty;
	//<--2014-11-20 최조영 JT관련 코드 추가

private:
	void InitMemberVariable(void);
	void InitMemberObject(void);

public:
	void Init();
	void SetCommand(int nCommand);
	int GetCommand(void);
	void SetUser(CString strUserID);
	CString GetUser(void);
	void SetBSendNRecvPacket(BOOL bSendRecvPacket);

public:
//설정정보
	void SetPKNetFolderInfo(CPKNetFolderInfo& pk);
	void GetPKNetFolderInfo(CPKNetFolderInfo& pk);

public:
//기타
	void SetPKCreateTable(CPKCreateTable& pk);
	void SetPKDuplicateData(CPKDuplicateData& pk);
	void SetPKTableCopy(CPKTableCopy& pk);
	void GetPKCreateTable(CPKCreateTable& pk);
	void GetPKDuplicateData(CPKDuplicateData& pk);
	void GetPKTableCopy(CPKTableCopy& pk);

//Common Table 관련
	void SetPKShipNoInfo(CPKShipNoInfo& pk);
	void SetPKDrawingTemplate(CPKDrawingTemplate& pk);
	void SetPKUserTemplateInfo(CPKUserTemplateInfo& pk);
	void SetPKUserInfo(CPKUserInfo& pk);
	void SetPKAutoUpdateFile(CPKAutoUpdateFile& pk);
	void SetPKConcurrentInfo(CPKConcurrentInfo& pk);
	void GetPKShipNoInfo(CPKShipNoInfo& pk);
	void GetPKDrawingTemplate(CPKDrawingTemplate& pk);
	void GetPKUserTemplateInfo(CPKUserTemplateInfo& pk);
	void GetPKUserInfo(CPKUserInfo& pk);
	void GetPKAutoUpdateFile(CPKAutoUpdateFile& pk);
	void GetPKConcurrentInfo(CPKConcurrentInfo& pk);

//EcoBlock 프로젝트 관련
	void SetPKProject(CPKProject& pk);
	void SetPKEcoBlockProjectTree(CPKEcoBlockProjectTree& pk);
	void SetPKMeasurePoint(CPKMeasurePoint& pk);
	void SetPKEcoBlockProject(CPKEcoBlockProject& pk);
	void SetPKEcoBlockRelationInfo(CPKEcoBlockRelationInfo& pk);
	void SetPKEcoBlockManualMesManager(CPKEcoBlockManualMesManager& pk);
	void SetPKEcoBlockDesignPointGeneralInfo(CPKEcoBlockDesignPointGeneralInfo& pk);
	void SetPKEcoBlockControlPointCoordInfo(CPKEcoBlockControlPointCoordInfo& pk);
	void SetPKEcoBlockControlPointDistanceInfo(CPKEcoBlockControlPointDistanceInfo& pk);
	void SetPKEcoBlockContorlPointCurveInfo(CPKEcoBlockContorlPointCurveInfo& pk);
	void SetPKEcoBlockControlPointPerpInfo(CPKEcoBlockControlPointPerpInfo& pk);
	void SetPKEcoBlockControlPointAngleInfo(CPKEcoBlockControlPointAngleInfo& pk);
	void SetPKEcoBlockControlPointFlatInfo(CPKEcoBlockControlPointFlatInfo& pk);
	void SetPKEcoBlockCameraInfo(CPKEcoBlockCameraInfo& pk);
	void SetPKEcoBlockProjectionFaceList(CPKEcoBlockProjectionFaceList& pk);
	void SetPKEcoBlockAddMeasurePointInfo(CPKEcoBlockAddMeasurePointInfo& pk);
	void SetPKEcoBlockDeleteMeasurePointInfo(CPKEcoBlockDeleteMeasurePointInfo& pk);
	void SetPKSystemDrawing(CPKSystemDrawing& pk);
	void SetPKSystemBlockList(CPKSystemBlockList& pk);
	void GetPKProject(CPKProject& pk);
	void GetPKEcoBlockProjectTree(CPKEcoBlockProjectTree& pk);
	void GetPKMeasurePoint(CPKMeasurePoint& pk);
	void GetPKEcoBlockProject(CPKEcoBlockProject& pk);
	void GetPKEcoBlockRelationInfo(CPKEcoBlockRelationInfo& pk);
	void GetPKEcoBlockManualMesManager(CPKEcoBlockManualMesManager& pk);
	void GetPKEcoBlockDesignPointGeneralInfo(CPKEcoBlockDesignPointGeneralInfo& pk);
	void GetPKEcoBlockControlPointCoordInfo(CPKEcoBlockControlPointCoordInfo& pk);
	void GetPKEcoBlockControlPointDistanceInfo(CPKEcoBlockControlPointDistanceInfo& pk);
	void GetPKEcoBlockContorlPointCurveInfo(CPKEcoBlockContorlPointCurveInfo& pk);
	void GetPKEcoBlockControlPointPerpInfo(CPKEcoBlockControlPointPerpInfo& pk);
	void GetPKEcoBlockControlPointAngleInfo(CPKEcoBlockControlPointAngleInfo& pk);
	void GetPKEcoBlockControlPointFlatInfo(CPKEcoBlockControlPointFlatInfo& pk);
	void GetPKEcoBlockCameraInfo(CPKEcoBlockCameraInfo& pk);
	void GetPKEcoBlockProjectionFaceList(CPKEcoBlockProjectionFaceList& pk);
	void GetPKEcoBlockAddMeasurePointInfo(CPKEcoBlockAddMeasurePointInfo& pk);
	void GetPKEcoBlockDeleteMeasurePointInfo(CPKEcoBlockDeleteMeasurePointInfo& pk);
	void GetPKSystemDrawing(CPKSystemDrawing& pk);
	void GetPKSystemBlockList(CPKSystemBlockList& pk);

//EcoOTS 프로젝트 관련
	void SetPKEcoOtsBlockList(CPKEcoOtsBlockList& pk);
	void SetPKEcoOtsProject(CPKEcoOtsProject& pk);
	void SetPKEcoOtsAddMeasurePointInfo(CPKEcoOtsAddMeasurePointInfo& pk);
	void SetPKEcoOtsDeleteMeasurePointInfo(CPKEcoOtsDeleteMeasurePointInfo& pk);
	void SetPKEcoOtsDesignPointGeneralInfo(CPKEcoOtsDesignPointGeneralInfo& pk);
	void SetPKEcoOtsRelationByDesMsr(CPKEcoOtsRelationByDesMsr& pk);
	void SetPKEcoOtsRelationByMsrMsr(CPKEcoOtsRelationByMsrMsr& pk);
	void SetPKEcoOtsDistanceCond(CPKEcoOtsDistanceCond& pk);
	void SetPKEcoOtsPointCond(CPKEcoOtsPointCond& pk);
	void SetPKEcoOtsDrawingElement(CPKEcoOtsDrawingElement& pk);
	void SetPKEcoOtsProjectionFaceList(CPKEcoOtsProjectionFaceList& pk);
	void SetPKEcoOtsCameraInfo(CPKEcoOtsCameraInfo& pk);
	void SetPKEcoOtsProjectionFaceModelingMatrixList(CPKEcoOtsProjectionFaceModelingMatrixList& pk);
	void GetPKEcoOtsBlockList(CPKEcoOtsBlockList& pk);
	void GetPKEcoOtsProject(CPKEcoOtsProject& pk);
	void GetPKEcoOtsAddMeasurePointInfo(CPKEcoOtsAddMeasurePointInfo& pk);
	void GetPKEcoOtsDeleteMeasurePointInfo(CPKEcoOtsDeleteMeasurePointInfo& pk);
	void GetPKEcoOtsDesignPointGeneralInfo(CPKEcoOtsDesignPointGeneralInfo& pk);
	void GetPKEcoOtsRelationByDesMsr(CPKEcoOtsRelationByDesMsr& pk);
	void GetPKEcoOtsRelationByMsrMsr(CPKEcoOtsRelationByMsrMsr& pk);
	void GetPKEcoOtsDistanceCond(CPKEcoOtsDistanceCond& pk);
	void GetPKEcoOtsPointCond(CPKEcoOtsPointCond& pk);
	void GetPKEcoOtsDrawingElement(CPKEcoOtsDrawingElement& pk);
	void GetPKEcoOtsProjectionFaceList(CPKEcoOtsProjectionFaceList& pk);
	void GetPKEcoOtsCameraInfo(CPKEcoOtsCameraInfo& pk);
	void GetPKEcoOtsProjectionFaceModelingMatrixList(CPKEcoOtsProjectionFaceModelingMatrixList& pk);

//Production관련
	void SetPKAssemblyLevel(CPKAssemblyLevel& pk);
	void SetPKAssemblyTree(CPKAssemblyTree& pk);
	void SetPKVariableOfGroup(CPKVariableOfGroup& pk);
	void SetPKProjectProperty(CPKProjectProperty& pk);
	void SetPKShipNoProperty(CPKShipNoProperty& pk);
	void SetPKVariableGroup(CPKVariableGroup& pk);
	void SetPKUserVariable(CPKUserVariable& pk);
	void SetPKBlockProperty(CPKBlockProperty& pk);
	void SetPKUploadFile(CPKUploadFile& pk);
	void SetPKUserInputBlockList(CPKUserInputBlockList& pk);
	void GetPKAssemblyLevel(CPKAssemblyLevel& pk);
	void GetPKAssemblyTree(CPKAssemblyTree& pk);
	void GetPKVariableOfGroup(CPKVariableOfGroup& pk);
	void GetPKProjectProperty(CPKProjectProperty& pk);
	void GetPKShipNoProperty(CPKShipNoProperty& pk);
	void GetPKVariableGroup(CPKVariableGroup& pk);
	void GetPKUserVariable(CPKUserVariable& pk);
	void GetPKBlockProperty(CPKBlockProperty& pk);
	void GetPKUploadFile(CPKUploadFile& pk);
	void GetPKUserInputBlockList(CPKUserInputBlockList& pk);

//모델관련
	void SetPKEcoModel(CPKEcoModel& pk);
	void SetPKTribonToEcoModel(CPKTribonToEcoModel& pk);
	void SetPKEcoModelErrorList(CPKEcoModelErrorList& pk);
	void GetPKEcoModel(CPKEcoModel& pk);
	void GetPKTribonToEcoModel(CPKTribonToEcoModel& pk);
	void GetPKEcoModelErrorList(CPKEcoModelErrorList& pk);

//MEI모델관련
	void SetPKCreateMEIModelTable(CPKCreateMEIModelTable& pk);
	void SetPKMEIEcoModel(CPKMEIEcoModel& pk);
	void SetPKMEIEcoModelManager(CPKMEIEcoModelManager& pk);
	void SetPKMEIHierarchyTree(CPKMEIHierarchyTree& pk);
	void GetPKCreateMEIModelTable(CPKCreateMEIModelTable& pk);
	void GetPKMEIEcoModel(CPKMEIEcoModel& pk);
	void GetPKMEIEcoModelManager(CPKMEIEcoModelManager& pk);
	void GetPKMEIHierarchyTree(CPKMEIHierarchyTree& pk);

//-->2014-11-20 최조영 JT관련 코드 추가 (현대전용)
//HHI JT서버 관련
//	void SetPKShipNoInfo_HHI(CPKJTServerProperty& pk);
	void SetPKJTHierarchyInfo_HHI(CPKJTServerProperty& pk);
//	void GetPKShipNoInfo_HHI(CPKJTServerProperty& pk);
	void GetPKJTHierarchyInfo_HHI(CPKJTServerProperty& pk);
	
//<--2014-11-20 최조영 JT관련 코드 추가

};

