// XTPChartTransformationDeviceCommand.h
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// (c)1998-2013 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
/////////////////////////////////////////////////////////////////////////////

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPCHARTTRANSFORMATIONDEVICECOMMAND_H__)
#define __XTPCHARTTRANSFORMATIONDEVICECOMMAND_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

namespace Gdiplus
{
	class GpRegion;
};


//===========================================================================
// Summary:
//     This class helps to save the state of drawing.This object is a kind of
//     CXTPChartDeviceCommand.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartSaveStateDeviceCommand : public CXTPChartDeviceCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartSaveStateDeviceCommand object.
	//-----------------------------------------------------------------------
	CXTPChartSaveStateDeviceCommand();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Override this function to do some  works if any, before the drawing.
	// Parameters:
	//     pDC - A pointer to the chart device context.
	// Remarks:
	//-----------------------------------------------------------------------
	virtual void BeforeExecute(CXTPChartDeviceContext* pDC);
	virtual void AfterExecute(CXTPChartDeviceContext* pDC);

protected:
	UINT m_nState;
};

//===========================================================================
// Summary:
//     This class abstracts the clipping operation of the sceen.This object is
//     a kind of CXTPChartDeviceCommand.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartClipDeviceCommand : public CXTPChartDeviceCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartClipDeviceCommand object.
	//-----------------------------------------------------------------------
	CXTPChartClipDeviceCommand(CRect rcClip);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to do the steps necessary to do the clipping
	//     operation.
	// Parameters:
	//     pDC - A pointer to the chart device context.
	// Remarks:
	//-----------------------------------------------------------------------
	virtual void BeforeExecute(CXTPChartDeviceContext* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to do the steps after the clipping operation.
	// Parameters:
	//     pDC - A pointer to the chart device context.
	// Remarks:
	//-----------------------------------------------------------------------
	virtual void AfterExecute(CXTPChartDeviceContext* pDC);


protected:
	CRect m_rcClip;     //The clipping rectangle.
	CRect m_rcState;    //The default clipping rectangle.
};

//===========================================================================
// Summary:
//     This class abstracts the clipping operation of the sceen.This object is
//     a kind of CXTPChartDeviceCommand.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartClipRegionDeviceCommand : public CXTPChartDeviceCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartClipDeviceCommand object.
	//-----------------------------------------------------------------------
	CXTPChartClipRegionDeviceCommand(const CXTPChartPoints& points);
	~CXTPChartClipRegionDeviceCommand();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to do the steps necessary to do the clipping
	//     operation.
	// Parameters:
	//     pDC - A pointer to the chart device context.
	// Remarks:
	//-----------------------------------------------------------------------
	virtual void BeforeExecute(CXTPChartDeviceContext* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to do the steps after the clipping operation.
	// Parameters:
	//     pDC - A pointer to the chart device context.
	// Remarks:
	//-----------------------------------------------------------------------
	virtual void AfterExecute(CXTPChartDeviceContext* pDC);


protected:
	Gdiplus::GpRegion* m_pGpState;     //The clipping rectangle.
	Gdiplus::GpRegion* m_pGpClip;    //The default clipping rectangle.
};


//===========================================================================
// Summary:
//     This class abstracts a rotation device command, which does rotation
//     transformtions.This is a kind of CXTPChartDeviceCommand.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartRotateDeviceCommand  : public CXTPChartDeviceCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartRotateDeviceCommand object.
	// Parameters:
	//     fAngle - The angle of rotation.
	//-----------------------------------------------------------------------
	CXTPChartRotateDeviceCommand(float fAngle);
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartRotateDeviceCommand object.
	// Parameters:
	//     fAngle        - The angle of rotation.
	//     rotateVector  - The vector to be rotated.
	//-----------------------------------------------------------------------
	CXTPChartRotateDeviceCommand(float fAngle, const CXTPPoint3d& rotateVector);

public:

	CXTPChartElement* HitTest(CPoint point, CXTPChartElement* pParent) const;
protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to do the actual drawing.
	// Parameters:
	//     pDC - A pointer to the chart device context.
	// Remarks:
	//-----------------------------------------------------------------------
	virtual void ExecuteOverride(CXTPChartDeviceContext* pDC);

protected:

	float m_fAngle;                         //The angle of rotation.
	CXTPPoint3d m_ptRotateVector;  //The rotation vector.
};

//===========================================================================
// Summary:
//     This class abstracts a rotation device command, which does translation
//     on vertices.This is a kind of CXTPChartDeviceCommand.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartTranslateDeviceCommand : public CXTPChartDeviceCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartTranslateDeviceCommand object.
	// Parameters:
	//     dx - The change in X direction
	//     dy - The change in Y direction
	//     dz - The change in Z direction
	//-----------------------------------------------------------------------
	CXTPChartTranslateDeviceCommand(double dx, double dy, double dz);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to do the actual drawing.
	// Parameters:
	//     pDC - A pointer to the chart device context.
	// Remarks:
	//-----------------------------------------------------------------------
	virtual void ExecuteOverride(CXTPChartDeviceContext* pDC);

public:

	CXTPChartElement* HitTest(CPoint point, CXTPChartElement* pParent) const;


protected:

	double m_dx;    //The change in X direction.
	double m_dy;    //The change in Y direction.
	double m_dz;    //The change in Z direction.
};



#endif //#if !defined(__XTPCHARTTRANSFORMATIONDEVICECOMMAND_H__)
