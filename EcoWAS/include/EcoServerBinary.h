#pragma once
class AFX_EXT_API CEcoServerBinary
{
public:
	CEcoServerBinary(void);
	~CEcoServerBinary(void);

private:
	BOOL m_bSetUser;
	BOOL m_bSetShipNo;
	BOOL m_bSetTmpRootPath;
	BOOL m_bSetNasRootPath;

private:
	CString m_strSystemDrawingDotExt;
	CString m_strGentabDotExt;
	CString m_strProjectDotExt;
	CString m_strMeasureDotExt;
	CString m_strTemplateDotExt;
	CString m_strThumbnailDotExt;
	CString m_strModelDotExt;
	CString m_strUserID;
	CString m_strShipNoGroup;
	CString m_strShipNo;
	CString m_strTmpRootPath;
	CString m_strNasRootPath;

private:
	CString m_strTempPath;
	CString m_strTemplatePath;
	CString m_strThumbnailPath;
	CString m_strAutoUpdatePath;

	CString m_strSystemDrawingPath;
	CString m_strShipNoGenTabPath;
	CString m_strPjtUploadFilePath;
	CString m_strBlockG2ModelPath;
	CString m_strBlockG2DraftPath;
	CString m_strOtsG2DraftPath;
	CString m_strMeasurePath;
	CString m_strDesignModelPath;
	CString m_strCablewayContourPath;
	CString m_strCablewayDataPath;
	CString m_strCPanelContourPath;
	CString m_strCPanelDataPath;
	CString m_strEquipmentContourPath;
	CString m_strEquipmentDataPath;
	CString m_strOtherCadContourPath;
	CString m_strOtherCadDataPath;
	CString m_strPipeContourPath;
	CString m_strPipeDataPath;
	CString m_strPlacedVolumnContourPath;
	CString m_strPlacedVolumnDataPath;
	CString m_strPPanelContourPath;
	CString m_strPPanelDataPath;
	CString m_strShellProfileContourPath;
	CString m_strShellProfileDataPath;
	CString m_strStructureContourPath;
	CString m_strStructureDataPath;
	CString m_strVentilationContourPath;
	CString m_strVentilationDataPath;
	CString m_strMEIModelDataPath;
	CString m_strMES_MeasurePath;

private:
	void DisplayLog(CString strLog);

	BOOL TmpBasicSettingCheck();
	BOOL TmpRootPathSettingCheck();
	BOOL NasBasicSettingCheck();
	BOOL NasRootPathSettingCheck();
	CString MakeTmpSystemDrawingFileName(CString strFileName);
	CString MakeTmpShipNoGenTabFileName(CString strFileName);
	CString MakeTmpProjectName(CString strAssemLevel, CString strPjtGroup, CString strPjtName);
	CString MakeProjectUploadFileName(CString strAssemLevel, CString strPjtGroup, CString strPjtName, CString strFileName);
	CString MakeTmpMeasureName(CString strAssemLevel, CString strPjtGroup, CString strMsrName);
	CString MakeTmpTemplateName(CString strTplGroupName, CString strTplName);
	CString MakeTmpThumbnailName(CString strTplGroupName, CString strTplName);
	CString MakeTmpModelName(CString strModelName);
	CString MakeTmpMESMeasureName(CString strId, CString strf_id, CString strMsrName);


	CString MakeNasSystemDrawingFileName(CString strFileName);
	CString MakeNasShipNoGenTabFileName(CString strFileName);
	CString MakeNasProjectName(CString strAssemLevel, CString strPjtGroup, CString strPjtName);
	CString MakeNasMeasureName(CString strAssemLevel, CString strPjtGroup, CString strMsrName);
	CString MakeNasTemplateName(CString strTplGroupName, CString strTplName);
	CString MakeNasThumbnailName(CString strTplGroupName, CString strTplName);
	CString MakeNasModelName(CString strModelName);
	CString MakeNasMESMeasureName(CString strId, CString strf_id, CString strMsrName);
	
	
	CString ConvertGeneralFileNameRule(CString strFileName);
	CString ConvertModelFileNameRule(CString strFileName);
	CString GetFileDotExt(CString strFileName);
	CString GetTmpGeneralHeadPath();
	CString GetNasCommonHeadPath();
public:
	CString GetTmpCommonHeadPath();
	CString GetNasGeneralHeadPath();


private:
	CString GetTempPath(void);
	CString GetTemplatePath(void);
	CString GetThumbnailPath(void);
	CString GetAutoUpdatePath(void);

	CString GetSystemDrawingPath(void);
	CString GetShipNoGenTabPath(void);
	CString GetPjtUploadFilePath(void);
	CString GetBlockG2ModelPath(void);
	CString GetBlockG2DraftPath(void);
	CString GetOtsG2DraftPath(void);
	CString GetMeasurePath(void);
	CString GetDesignModelPath(void);
	CString GetCablewayContourPath(void);
	CString GetCablewayDataPath(void);
	CString GetCPanelContourPath(void);
	CString GetCPanelDataPath(void);
	CString GetEquipmentContourPath(void);
	CString GetEquipmentDataPath(void);
	CString GetOtherCadContourPath(void);
	CString GetOtherCadDataPath(void);
	CString GetPipeContourPath(void);
	CString GetPipeDataPath(void);
	CString GetPlacedVolumnContourPath(void);
	CString GetPlacedVolumnDataPath(void);
	CString GetPPanelContourPath(void);
	CString GetPPanelDataPath(void);
	CString GetShellprofileContourPath(void);
	CString GetShellprofileDataPath(void);
	CString GetStructureContourPath(void);
	CString GetStructureDataPath(void);
	CString GetVentilationContourPath(void);
	CString GetVentilationDataPath(void);
	CString GetMEIModelDataPath(int iNickTable);
	CString GetMESMeasurePath(void);


public:
	void SetSystemDrawingDotExt(CString strDotExt);
	void SetGenTabDotExt(CString strDotExt);
	void SetProjectDotExt(CString strDotExt);
	void SetMeasureDotExt(CString strDotExt);
	void SetTemplateDotExt(CString strDotExt);
	void SetThumbnailDotExt(CString strDotExt);
	void SetModelDotExt(CString strDotExt);
	void SetUser(CString strUser);
	void SelectProject(CString strGroup, CString strShipNo);
	void SetRootPath(CString strTmpPath, CString strNasPath);
	void SetTmpRootPath(CString strPath);
	void SetNasRootPath(CString strPath);
	CString GetTmpRootPath(void);
	CString GetNasRootPath(void);
	CString GetTmpJtFileFolderPath(void);

public:
	CString GetTmpTempPathName(CString strFileName);
	CString GetTmpTemplatePathName(CString strTplGroupName, CString strTplName);
	CString GetTmpTemplatePathName(CString strTplName);
	CString GetTmpThumbnailPathName(CString strTplGroupName, CString strTplName);
	CString GetTmpThumbnailPathName(CString strTplName);
	CString GetTmpAutoUpdatePathName(CString strFileName);

	CString GetTmpSystemDrawingPathName(CString strFileName);
	CString GetTmpShipNoGenTabPathName(CString strFileName);
	CString GetTmpPjtUploadFilePathName(CString strAssemLevel, CString strPjtGroup, CString strPjtName, CString strFileName);
	CString GetTmpPjtUploadFilePathName(CString strFileName);
	CString GetTmpBlockG2ModelPathName(CString strAssemLevel, CString strPjtGroup, CString strPjtName);
	CString GetTmpBlockG2ModelPathName(CString strPjtName);
	CString GetTmpBlockG2DraftPathName(CString strAssemLevel, CString strPjtGroup, CString strPjtName);
	CString GetTmpBlockG2DraftPathName(CString strPjtName);
	CString GetTmpOtsG2DraftPathName(CString strAssemLevel, CString strPjtGroup, CString strPjtName);
	CString GetTmpOtsG2DraftPathName(CString strPjtName);
	CString GetTmpMeasurePathName(CString strAssemLevel, CString strPjtGroup, CString strMsrName);
	CString GetTmpMeasurePathName(CString strMsrName);
	CString GetTmpDesignModelContourPathName(CString strModelType, CString strModelName);
	CString GetTmpDesignModelDataPathName(CString strModelType, CString strModelName);
	CString GetTmpCablewayContourPathName(CString strModelName);
	CString GetTmpCablewayDataPathName(CString strModelName);
	CString GetTmpCPanelContourPathName(CString strModelName);
	CString GetTmpCPanelDataPathName(CString strModelName);
	CString GetTmpEquipmentContourPathName(CString strModelName);
	CString GetTmpEquipmentDataPathName(CString strModelName);
	CString GetTmpOtherCadContourPathName(CString strModelName);
	CString GetTmpOtherCadDataPathName(CString strModelName);
	CString GetTmpPipeContourPathName(CString strModelName);
	CString GetTmpPipeDataPathName(CString strModelName);
	CString GetTmpPlacedVolumnContourPathName(CString strModelName);
	CString GetTmpPlacedVolumnDataPathName(CString strModelName);
	CString GetTmpPPanelContourPathName(CString strModelName);
	CString GetTmpPPanelDataPathName(CString strModelName);
	CString GetTmpShellprofileContourPathName(CString strModelName);
	CString GetTmpShellprofileDataPathName(CString strModelName);
	CString GetTmpStructureContourPathName(CString strModelName);
	CString GetTmpStructureDataPathName(CString strModelName);
	CString GetTmpVentilationContourPathName(CString strModelName);
	CString GetTmpVentilationDataPathName(CString strModelName);
	CString GetTmpMEIModelPathName(int iNicktable, CString strModelName);
	CString GetTmpMESMeasurePathName(CString strId, CString strf_id, CString strMsrName);
	CString GetTmpMESMeasurePathName(CString strMsrName);

	CString GetNasTemplatePathName(CString strTplGroupName, CString strTplName);
	CString GetNasTemplatePathName(CString strTplName);
	CString GetNasThumbnailPathName(CString strTplGroupName, CString strTplName);
	CString GetNasThumbnailPathName(CString strTplName);
	CString GetNasAutoUpdatePathName(CString strFileName);

	CString GetNasSystemDrawingPathName(CString strFileName);
	CString GetNasShipNoGenTabPathName(CString strFileName);
	CString GetNasPjtUploadFilePathName(CString strAssemLevel, CString strPjtGroup, CString strPjtName, CString strFileName);
	CString GetNasPjtUploadFilePathName(CString strFileName);
	CString GetNasBlockG2ModelPathName(CString strAssemLevel, CString strPjtGroup, CString strPjtName);
	CString GetNasBlockG2ModelPathName(CString strPjtName);
	CString GetNasBlockG2DraftPathName(CString strAssemLevel, CString strPjtGroup, CString strPjtName);
	CString GetNasBlockG2DraftPathName(CString strPjtName);
	CString GetNasOtsG2DraftPathName(CString strAssemLevel, CString strPjtGroup, CString strPjtName);
	CString GetNasOtsG2DraftPathName(CString strPjtName);
	CString GetNasMeasurePathName(CString strAssemLevel, CString strPjtGroup, CString strMsrName);
	CString GetNasMeasurePathName(CString strMsrName);
	CString GetNasDesignModelContourPathName(CString strModelType, CString strModelName);
	CString GetNasDesignModelDataPathName(CString strModelType, CString strModelName);
	CString GetNasCablewayContourPathName(CString strModelName);
	CString GetNasCablewayDataPathName(CString strModelName);
	CString GetNasCPanelContourPathName(CString strModelName);
	CString GetNasCPanelDataPathName(CString strModelName);
	CString GetNasEquipmentContourPathName(CString strModelName);
	CString GetNasEquipmentDataPathName(CString strModelName);
	CString GetNasOtherCadContourPathName(CString strModelName);
	CString GetNasOtherCadDataPathName(CString strModelName);
	CString GetNasPipeContourPathName(CString strModelName);
	CString GetNasPipeDataPathName(CString strModelName);
	CString GetNasPlacedVolumnContourPathName(CString strModelName);
	CString GetNasPlacedVolumnDataPathName(CString strModelName);
	CString GetNasPPanelContourPathName(CString strModelName);
	CString GetNasPPanelDataPathName(CString strModelName);
	CString GetNasShellprofileContourPathName(CString strModelName);
	CString GetNasShellprofileDataPathName(CString strModelName);
	CString GetNasStructureContourPathName(CString strModelName);
	CString GetNasStructureDataPathName(CString strModelName);
	CString GetNasVentilationContourPathName(CString strModelName);
	CString GetNasVentilationDataPathName(CString strModelName);
	CString GetNasMEIModelPathName(int iNicktable, CString strModelName);
	CString GetNasMESMeasurePathName(CString strId, CString strf_id, CString strMsrName);
	CString GetNasMESMeasurePathName(CString strMsrName);


public:
	BOOL CreateDirectory(CString strLocalDir);
	
};

