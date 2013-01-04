// XTPChartDeviceCommand.h
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// (c)1998-2012 Codejock Software, All Rights Reserved.
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
#if !defined(__XTPCHARTDEVICECOMMAND_H__)
#define __XTPCHARTDEVICECOMMAND_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CXTPChartDeviceCommand;
class CXTPChartDeviceContext;
class CXTPChartFont;
class CXTPChartElement;

//===========================================================================
// Summary:
//     This class handles the rendering elements in a chart.
//     This class act as a base class for all the specialized device
//     command objects which do specific rendering jobs related to each
//     element in a chart.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartDeviceCommand
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartDeviceCommand object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartDeviceCommand();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartDeviceCommand object, handles cleanup
	//-------------------------------------------------------------------------
	virtual ~CXTPChartDeviceCommand();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to add a child device command.
	// Parameters:
	//     pCommand - A pointer to the child device command.
	// Remarks:
	//     A device command object keeps an array of child device commands, an
	//     instruction to execute the drawing of the parent object trigger the
	//     drawing of children as well.
	//-----------------------------------------------------------------------
	CXTPChartDeviceCommand* AddChildCommand(CXTPChartDeviceCommand* pCommand);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to execute the drawing.
	// Parameters:
	//     pDC - A pointer to the chart device context.
	// Remarks:
	//     An instruction to execute the drawing of the parent object trigger the
	//     drawing of children as well.
	//-----------------------------------------------------------------------
	virtual void Execute(CXTPChartDeviceContext* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     Override this function to do some ground works if any, before the drawing.
	// Parameters:
	//     pDC - A pointer to the chart device context.
	// Remarks:
	//-----------------------------------------------------------------------
	virtual void BeforeExecute(CXTPChartDeviceContext* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     Override this function to do some final cut if any, after the drawing.
	// Parameters:
	//     pDC - A pointer to the chart device context.
	// Remarks:
	//-----------------------------------------------------------------------
	virtual void AfterExecute(CXTPChartDeviceContext* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     Override this function to write the drawing code for specific objects.
	// Parameters:
	//     pDC - A pointer to the chart device context.
	// Remarks:
	//-----------------------------------------------------------------------
	virtual void ExecuteOverride(CXTPChartDeviceContext* pDC);


	virtual CXTPChartElement* HitTest(CPoint point, CXTPChartElement* pParent) const;

protected:
	CArray<CXTPChartDeviceCommand*, CXTPChartDeviceCommand*> m_arrChildren;   //The array of child device commands.
};


class CXTPChartHitTestElementCommand : public CXTPChartDeviceCommand
{
public:
	CXTPChartHitTestElementCommand(CXTPChartElement* pElement);
	CXTPChartHitTestElementCommand(CXTPChartElement* pElement, const CRect& rcBounds);
	CXTPChartHitTestElementCommand(CXTPChartElement* pElement, const CXTPChartRectF& rcBounds);

public:
	virtual CXTPChartElement* HitTest(CPoint point, CXTPChartElement* pParent) const;

protected:
	CXTPChartElement* m_pElement;
	CRect m_rcBounds;
};

//===========================================================================
// Summary:
//     This class is a kind of CXTPChartDeviceCommand and it draws the element and
//     does some additional task to smooth the drawings using antialiasing.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartPolygonAntialiasingDeviceCommand : public CXTPChartDeviceCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartPolygonAntialiasingDeviceCommand object.
	// Parameters:
	//      bAntiAlias - TRUE if the antialiasing is enabled and FALSE if the
	//      antialiasing is disabled.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartPolygonAntialiasingDeviceCommand(BOOL bAntiAlias = TRUE);
	virtual ~CXTPChartPolygonAntialiasingDeviceCommand();

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//      This override do some ground works if any, before the native mode
	//      antialiased drawing of polygons.
	// Parameters:
	//     pDC - A pointer to the chart native device context.
	// Remarks:
	//-----------------------------------------------------------------------
	void BeforeExecute(CXTPChartDeviceContext* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     This override do some final cut if any, after the native antialiased
	//     drawing of polygons.
	// Parameters:
	//     pDC - A pointer to the chart native device context.
	// Remarks:
	//-----------------------------------------------------------------------
	void AfterExecute(CXTPChartDeviceContext* pDC);

protected:
	BOOL m_bAntiAlias;              //TRUE if antialiasing enabled, FALSE if not.
	long m_bOldAntiAlias;           //The previous value of anti aliasing.
};


#endif //#if !defined(__XTPCHARTDEVICECOMMAND_H__)
