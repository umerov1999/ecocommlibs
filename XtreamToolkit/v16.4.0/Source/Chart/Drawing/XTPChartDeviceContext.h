// XTPChartDeviceContext.h
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
#if !defined(__XTPCHARTDEVICECONTEXT_H__)
#define __XTPCHARTDEVICECONTEXT_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


namespace Gdiplus
{
	class GpGraphics;
	class GpFont;
	class Graphics;
};

class CXTPChartContainer;
class CXTPChartDeviceCommand;

//===========================================================================
// Summary:
//     This class abstracts a device context, A device context stores,
//     retrieves, and modifies the attributes of graphic objects and
//     specifies graphic modes.This class is a kind of CCmdTarget to enhance
//     the MFC command routing.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartDeviceContext : public CXTPCmdTarget
{
	class CGdiPlus;
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartDeviceContext object.
	// Parameters:
	//     hDC - Handle to the windows device context.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartDeviceContext(CXTPChartContainer* pContainer, HDC hDC);
	CXTPChartDeviceContext(CXTPChartContainer* pContainer, Gdiplus::Graphics*, HDC hDC);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartDeviceContext object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartDeviceContext(CXTPChartContainer* pContainer);

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartDeviceContext object, handles cleanup
	//-------------------------------------------------------------------------
	virtual ~CXTPChartDeviceContext();

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to Intialize/Uninitialize the GDI+ library, and
	//     keep track of the usage count.
	// Parameters:
	//     bInit - TRUE to initialize the GDI+ library, FALSE to uninitialized
	//             Also a TRUE value will increase the usage count and FALSE will
	//             decrease the usage count.
	// Remarks
	//     This is a static function.
	//-------------------------------------------------------------------------
	static void AFX_CDECL Register(BOOL bInit);

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to trigger the drawing.
	// Parameters:
	//     pCommand - A pointer to chart device command object.
	//-------------------------------------------------------------------------
	virtual void Execute(CXTPChartDeviceCommand* pCommand);

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the GDI+ graphics class.
	// Returns:
	//     A pointer to GpGraphics object.
	//-------------------------------------------------------------------------
	Gdiplus::GpGraphics* GetGraphics() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the windows device context.
	// Returns:
	//     A handle to the windows device context.
	//-------------------------------------------------------------------------
	HDC GetHDC() const;

public:
	CXTPChartContainer* GetContainer() const;

public:
	virtual BOOL GetNativeDrawing() const;
	virtual void SetNativeDrawing(BOOL bNativeDrawing);


public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the measurements of texts rendered in a
	//     device context using a specific font.
	// Parameters:
	//     pText - The string to be measured.
	//     pFont - Pointer to chart font object.
	//-------------------------------------------------------------------------
	virtual CXTPChartSizeF MeasureString(const CXTPChartString* pText, CXTPChartFont* pFont);

	static void AFX_CDECL SaveToFile(HBITMAP hBitmap, LPCTSTR lpszFileName);

protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the CGDIPlus object.
	// Returns:
	//     A pointer to CGdiPlus object.
	// Remarks
	//     This is a static function.
	//-------------------------------------------------------------------------
	static CGdiPlus* AFX_CDECL GetGdiPlus();

protected:
	ULONG_PTR m_nGdiplusToken;  //The GDI+ token.

	Gdiplus::GpGraphics* m_pGpGraphics;  //Pointer to GDI+ graphics object.
	HDC m_hDC;                  //The windows device context.

	CXTPChartContainer* m_pContainer;
};

AFX_INLINE Gdiplus::GpGraphics* CXTPChartDeviceContext::GetGraphics() const {
	return m_pGpGraphics;
}

AFX_INLINE HDC CXTPChartDeviceContext::GetHDC() const {
	return m_hDC;
}
AFX_INLINE CXTPChartContainer* CXTPChartDeviceContext::GetContainer() const {
	return m_pContainer;
}
AFX_INLINE BOOL CXTPChartDeviceContext::GetNativeDrawing() const {
	return TRUE;
}
AFX_INLINE void CXTPChartDeviceContext::SetNativeDrawing(BOOL /*bNativeDrawing*/) {

}

#endif //#if !defined(__XTPCHARTDEVICECONTEXT_H__)
