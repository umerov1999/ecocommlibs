#pragma once

/**************************************************
Define Error Codes
**************************************************/
#define	ECO_OK					0x0000
#define	ECO_FAILE				0x0001
#define	ECO_DUPLICATE_HWND		0x0002

#include "SISPoint.h"
#include "SISMngPoint.h"
#include "SISModelProp.h"
#include "SISAssembly.h"

/**************************************************
Extern Class...
**************************************************/
class AFX_EXT_API CEcoViewerLibs
{
public:
	CEcoViewerLibs();
	~CEcoViewerLibs();

public:
	int SIS_InitLibs(HWND hWnd, UINT* iViewID);
	int SIS_ReleaseLibs(UINT iViewID);
	int SIS_RebindView(UINT iViewID);
	int SIS_InValidateView(UINT iViewID);

// Define Window Events
public:
	int SIS_RbuttonDown(UINT iViewID, UINT nFlags, CPoint point);
	int SIS_RbuttonUp(UINT iViewID, UINT nFlags, CPoint point);
	int SIS_MouseMove(UINT iViewID, UINT nFlags, CPoint point);
	int SIS_LbuttonDown(UINT iViewID, UINT nFlags, CPoint point);
	int SIS_LbuttonUp(UINT iViewID, UINT nFlags, CPoint point);
	int SIS_LbuttonDblClk(UINT iViewID, UINT nFlags, CPoint point);
	int SIS_MbuttonDown(UINT iViewID, UINT nFlags, CPoint point);
	int SIS_MbuttonUp(UINT iViewID, UINT nFlags, CPoint point);
	int SIS_MouseWheel(UINT iViewID, UINT nFlags, short zDelta, CPoint point);
	int SIS_KeyDown(UINT iViewID, UINT nChar, UINT nRepCnt, UINT nFlags);
	int SIS_KeyUp(UINT iViewID, UINT nChar, UINT nRepCnt, UINT nFlags);

public:
	LONG SIS_SetDelPcdCallBack(UINT iViewID, LPVOID lpCallbackFunc);

public:
	LONG SIS_ViewOrbit(UINT iViewID);
	LONG SIS_ViewPanning(UINT iViewID);
	LONG SIS_ViewZoom(UINT iViewID);
	LONG SIS_ViewZoomToRect(UINT iViewID);
	LONG SIS_ViewZoomToExtent(UINT iViewID);

	LONG SIS_SetCameraPosition(UINT iViewID, LPCTSTR strMode);
	LONG SIS_SetRenderMode(UINT iViewID, LPCTSTR strMode);
	void SIS_ViewUpdate(UINT iViewID);
	LONG SIS_ShowCuttingPlaneX(UINT iViewID, BOOL bShow);
	LONG SIS_ShowCuttingPlaneY(UINT iViewID, BOOL bShow);
	LONG SIS_ShowCuttingPlaneZ(UINT iViewID, BOOL bShow);
	LONG SIS_ScreenInit(UINT iViewID);
	LONG SIS_SetWalkThrough(UINT iViewID);
	LONG SIS_ReadDesignModel(UINT iViewID, LPCTSTR strPath, BOOL bInit);
	LONG SIS_ReadPointCloud(UINT iViewID, LPCTSTR strPath, LPCTSTR strUnit, BOOL bInit);
	LONG SIS_CreatePointCoord(UINT iViewID);
	LONG SIS_CreateDistance(UINT iViewID);
	LONG SIS_CreateAngle3Pt(UINT iViewID);
	LONG SIS_SaveProject(UINT iViewID, LPCTSTR strPath);
	LONG SIS_SaveDraft(UINT iViewID, LPCTSTR strPath);
	LONG SIS_LoadProject(UINT iViewID, BOOL bLoad3D, BOOL bLoad2D, LPCTSTR strPath);
	LONG SIS_WriteEBM(UINT iViewID, LPCTSTR strPath);
	LONG SIS_CreateMovePoint(UINT iViewID);
	LONG SIS_Trans1Pt(UINT iViewID, LPCTSTR strTransObject, LPCTSTR strPcdObjectName, SISPoint pPosBase, SISPoint pPosTrans);
	LONG SIS_Trans2Pt(UINT iViewID, LPCTSTR strTransObject, LPCTSTR strPcdObjectName, SISPoint pPosBase1, SISPoint pPosBase2, SISPoint pPosTrans1, SISPoint pPosTrans2, BOOL bLevelPreserve);
	LONG SIS_Trans3Pt(UINT iViewID, LPCTSTR strTransObject, LPCTSTR strPcdObjectName, SISPoint pPosBase1, SISPoint pPosBase2, SISPoint pPosBase3, SISPoint pPosTrans1, SISPoint pPosTrans2, SISPoint pPosTrans3);
	LONG SIS_TransByModelingMatrix(UINT iViewID, LPCTSTR strTransObject, LPCTSTR strPcdObjectName, FLOAT* pModelingMatrix);
	LONG SIS_DefaultOperator(UINT iViewID);
	LONG SIS_UndoTrans(UINT iViewID);
	LONG SIS_RedoTrans(UINT iViewID);
	LONG SIS_UndoHideDimensionObject(UINT iViewID);
	LONG SIS_RedoHideDimensionObject(UINT iViewID);
	LONG SIS_HomeHideDimensionObject(UINT iViewID);
	LONG SIS_UndoHideDesignModel(UINT iViewID);
	LONG SIS_RedoHideDesignModel(UINT iViewID);
	LONG SIS_HomeHideDesignModel(UINT iViewID);
	LONG SIS_ShowPointCoord(UINT iViewID, BOOL bShow);
	LONG SIS_ShowDistance(UINT iViewID, BOOL bShow);
	LONG SIS_ShowAngle(UINT iViewID, BOOL bShow);
	LONG SIS_ShowDesignModel(UINT iViewID, BOOL bShow);
	LONG SIS_SetPcdMarkerSize(UINT iViewID, LONG nSize);
	LONG SIS_SetPcdDecimation(UINT iViewID, float fDecimation);
	LONG SIS_SelectPcdByReSamplingDist(UINT iViewID, LPCTSTR strPcdObjectName, DOUBLE dReSamplingDistance, LPUNKNOWN pSelectedPcd);
	LONG SIS_SelectPcdByReSamplingDistFromDisk(LPCTSTR strPcdFilePath, DOUBLE dReSamplingDistance, LPCTSTR strUnit, LPUNKNOWN pSelectedPcd);
	LONG SIS_SetPcdRenderMode(UINT iViewID, LPCTSTR strMode);
	LONG SIS_SetPcdAntiAliasing(UINT iViewID, BOOL bAntiAliasing);
	LONG SIS_SetColorPcdObject(UINT iViewID, LPCTSTR strPcdObjectName, LONG nR, LONG nG, LONG nB);
	LONG SIS_InitColorPcdObject(UINT iViewID, LPCTSTR strPcdObjectName);
	LONG SIS_SelectPcdByArea(UINT iViewID, LPUNKNOWN pSelectedPcd);
	LONG SIS_SetTransparencyDesignModel(UINT iViewID, LONG nTrans);
	LONG SIS_SetColorDesignModel(UINT iViewID, LONG nR, LONG nG, LONG nB);
	LONG SIS_SelectDesignModel(UINT iViewID, BOOL bSelect);
	LONG SIS_GetPcdNameList(UINT iViewID, LPWSTR* pStrPcdNames);
	LONG SIS_GetPcdPathNameListFromPjt(UINT iViewID, LPCTSTR strPjtPathName, LPWSTR* pStrPcdPathNames);
	LONG SIS_ShowDimensionObject(UINT iViewID, BOOL bShow);
	LONG SIS_DeleteAllDimensionObject(UINT iViewID);
	LONG SIS_CreatePointCoordByValue(UINT iViewID, SISPoint pPos);
	LONG SIS_CreateDistanceByValue(UINT iViewID, SISPoint pPos1, SISPoint pPos2);
	LONG SIS_CreateAngle3PtByValue(UINT iViewID, SISPoint pPosBase, SISPoint pPos1, SISPoint pPos2);
	LONG SIS_CreateMovePointByValue(UINT iViewID, LPCTSTR strTransObject, LPCTSTR strPcdObjectName, SISPoint pPos);
	LONG SIS_GetCoord(UINT iViewID, SISPoint* pPos);
	LONG SIS_DeleteMovePoint(UINT iViewID);
	LONG SIS_SelectPcdByNearestK(UINT iViewID, LONG nPtCnt, LPUNKNOWN pSelectedPcd);
	LONG SIS_SelectPcdByRadius(UINT iViewID, DOUBLE dRadius, LPUNKNOWN pSelectedPcd);
	LONG SIS_GetMngPointCount(UINT iViewID, LONG* pCount);
	LONG SIS_GetMngPointList(UINT iViewID, SISMngPoint* pArrMngPoint);
	LONG SIS_GetSelectedMngPointCount(UINT iViewID, LONG* pCount);
	LONG SIS_GetSelectedMngPointList(UINT iViewID, SISMngPoint* pArrMngPoint);
	LONG SIS_DrawPlane(UINT iViewID, SISPoint centerPos, SISPoint vector, UINT nWidth, UINT nHeight);
	LONG SIS_DrawSphere(UINT iViewID, SISPoint centerPos, double dblRadius);
	LONG SIS_DrawLine(UINT iViewID, SISPoint startPos, SISPoint endPos);
	LONG SIS_SetColorPcdSelBox(UINT iViewID, LONG nR, LONG nG, LONG nB);
	LONG SIS_SetTransparencyPcdSelBox(UINT iViewID, LONG nTrans);

	LONG SIS_GetModelObjCnt(UINT iViewID, UINT* pCnt);
	LONG SIS_GetModelObjProp(UINT iViewID, UINT nCnt, SISModelProp* pModelProp);

	LONG SIS_HideModel(UINT iViewID, UINT nObjNameCnt, LPTSTR* strObjNames);
	LONG SIS_ShowModel(UINT iViewID, UINT nObjNameCnt, LPTSTR* strObjNames);
	LONG SIS_ReverseShowHideModel(UINT iViewID);
	LONG SIS_ZoomModelObject(UINT iViewID, LPCTSTR strObjName);
	LONG SIS_DeleteModel(UINT iViewID, UINT nObjNameCnt, LPTSTR* strObjNames);
	LONG SIS_HighlightModel(UINT iViewID, UINT nObjNameCnt, LPTSTR* strObjNames);
	LONG SIS_UnHighlightModel(UINT iViewID);

	LONG SIS_CaptureFile(UINT iViewID, LPCTSTR strPath, LPCTSTR strFormat);
	LONG SIS_CaptureClipboard(UINT iViewID);
	LONG SIS_CaptureFileByArea(UINT iViewID, LPCTSTR strPath, LPCTSTR strFormat);
	LONG SIS_CaptureClipboardByArea(UINT iViewID);

	LONG SIS_Show3D(UINT iViewID);
	LONG SIS_Show2D(UINT iViewID);
	LONG SIS_LoadDraft(UINT iViewID, LPCTSTR strPath);

	LONG SIS_SetSyncCamera(UINT iStandardViewIdF_For_Sync);
	LONG SIS_SetSyncMouseEvent();
	LONG SIS_ReleaseSyncMouseEvent();

	LONG SIS_Login(UINT iViewID, LPCTSTR strHost, LPCTSTR strUser, LPCTSTR strPwd);
	LONG SIS_Logout(UINT iViewID);
	LONG SIS_LoadProjectFromServer(UINT iViewID);
	LONG SIS_LoadProjectFromServer(UINT iViewID, LPCTSTR strShipNoGroup, LPCTSTR strShipNo, UINT nProjectID);
	LONG SIS_ReadDesignModelFromServer(UINT iViewID, BOOL bInit);
	LONG SIS_ReadDesignModelFromTribon(UINT iViewID, BOOL bInit);
	LONG SIS_LoadDraftFromServer(UINT iViewID);
	LONG SIS_WriteDesignModel(UINT iViewID, LPCTSTR strPath, LPCTSTR strFormat);


	LONG SIS_PolygonSelection(UINT iViewID);
	LONG SIS_SetCameraProjectionMode(UINT iViewID, LPCTSTR strMode);
	BOOL SIS_IsExistInBoundRectangle(UINT iViewID, SISPoint testPt, SISPoint boundInfo[]);
	LONG SIS_UndoDeletePcd(UINT iViewID);
	LONG SIS_RedoDeletePcd(UINT iViewID);
	LONG SIS_UndoHomeDeletePcd(UINT iViewID);
	LONG SIS_RedoLastDeletePcd(UINT iViewID);
	LONG SIS_SetVisiblePcdObject(UINT iViewID, LPCTSTR strPcdObjectName, BOOL bVisible);
	LONG SIS_RemovePcdObject(UINT iViewID, LPCTSTR strPcdObjectName);
	
	LONG SIS_GetAssemblyCnt(UINT iViewID, UINT* pCnt);
	LONG SIS_GetAssembly(UINT iViewID, UINT nCnt, SISAssembly* pAssembly);
	LONG SIS_HideModel(UINT iViewID, UINT nAssemblyCnt, SISAssembly* pAssembly);
	LONG SIS_ShowModel(UINT iViewID, UINT nAssemblyCnt, SISAssembly* pAssembly);
	LONG SIS_ZoomModelObjectByAssemName(UINT iViewID, LPCTSTR strAssemblyName);
	LONG SIS_DeleteModel(UINT iViewID, UINT nAssemblyCnt, SISAssembly* pAssembly);
	LONG SIS_HighlightModel(UINT iViewID, UINT nAssemblyCnt, SISAssembly* pAssembly);
	LONG SIS_HideSelectedObject(UINT iViewID);
	LONG SIS_ShowPerpendicular(UINT iViewID, BOOL bShow);
	LONG SIS_PrintDraft(UINT iViewID);
	LONG SIS_SetFocus(UINT iViewID);
	LONG SIS_OnSize(UINT iViewID);
	LONG SIS_OnPrint(UINT iViewID, CDC* pDC, CPrintInfo* pInfo);
	LONG SIS_CaptureToFileDraft(UINT iViewID);
	LONG SIS_CaptureToClipboardDraft(UINT iViewID);
	LONG SIS_CaptureToFile3D(UINT iViewID);
	LONG SIS_CaptureToClipboard3D(UINT iViewID);
	LONG SIS_Print3D(UINT iViewID);
	LONG SIS_GetCurrentOperator(UINT iViewID, LPTSTR strOp);
};

