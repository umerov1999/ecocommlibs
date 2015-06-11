// XTPSkinManagerSchema.h: interface for the CXTPSkinManagerSchema class.
//
// This file is a part of the XTREME SKINFRAMEWORK MFC class library.
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
#if !defined(_XTPSKINMANAGERSCHEMA_H__)
#define _XTPSKINMANAGERSCHEMA_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPSkinManagerResourceFile;
class CXTPSkinManagerClass;
class CXTPSkinManager;
class CXTPSkinObjectFrame;
class CXTPSkinImage;
class CXTPSkinManagerMetrics;
class CXTPSkinManagerSchema;
struct XTP_SKINSCROLLBAR_POSINFO;

//#define _OLD_METHOD_GET_TMS_CODES_VERIFY

//===========================================================================
// Summary:
//     CXTPSkinManagerSchemaProperty is standalone class represented single property of visual styles ini file
//===========================================================================
class _XTP_EXT_CLASS CXTPSkinManagerSchemaProperty : public CXTPSynchronized
{
public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPSkinManagerSchemaProperty object.
	//-------------------------------------------------------------------------
	CXTPSkinManagerSchemaProperty();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPSkinManagerSchemaProperty object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	~CXTPSkinManagerSchemaProperty();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to convert string to Color property
	// Parameters:
	//     lpszValue - String to convert property from
	//-----------------------------------------------------------------------
	void SetPropertyColor(LPCTSTR lpszValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to set string property
	// Parameters:
	//     lpszValue - String to convert property from
	//-----------------------------------------------------------------------
	void SetPropertyString(LPCTSTR lpszValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to convert string to BOOL property
	// Parameters:
	//     lpszValue - String to convert property from
	//-----------------------------------------------------------------------
	void SetPropertyBool(LPCTSTR lpszValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to convert string to int property
	// Parameters:
	//     lpszValue - String to convert property from
	//-----------------------------------------------------------------------
	void SetPropertyInt(LPCTSTR lpszValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to set enum property
	// Parameters:
	//     nEnumValue - Enumerator to set
	//-----------------------------------------------------------------------
	void SetPropertyEnum(int nEnumValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to convert string to size property
	// Parameters:
	//     lpszValue - String to convert property from
	//-----------------------------------------------------------------------
	void SetPropertySize(LPCTSTR lpszValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to convert string to CRect property
	// Parameters:
	//     lpszValue - String to convert property from
	//-----------------------------------------------------------------------
	void SetPropertyRect(LPCTSTR lpszValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to convert string to LOGFONT property
	// Parameters:
	//     lpszValue - String to convert property from
	//-----------------------------------------------------------------------
	void SetPropertyFont(LPCTSTR lpszValue);
	void ClearProperty();

	BOOL operator==(const CXTPSkinManagerSchemaProperty& obj) const;

public:
	XTPSkinManagerProperty propType;    // Property type
	int nPropertyCode;                  // Property code

	union
	{
		COLORREF clrVal;    // Color value of the property
		LPTSTR lpszVal;     // LPCTSTR value of the property
		BOOL bVal;          // BOOL value of the property
		int iVal;           // Integer value of the property
		RECT rcVal;         // Rect value of the property
		SIZE szVal;         // SIZE value of the property
		PLOGFONT lfVal;     // LOGFONT value
	};
};


//===========================================================================
// Summary:
//     CXTPSkinManagerSchema is standalone class used to read visual styles schema file
//===========================================================================
class _XTP_EXT_CLASS CXTPSkinManagerSchema 
	: public CXTPCmdTarget
	, public CXTPSynchronized
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPSkinManagerSchema object.
	// Parameters:
	//     pResourceFile - Resource file to read
	//-----------------------------------------------------------------------
	CXTPSkinManagerSchema(CXTPSkinManagerResourceFile* pResourceFile);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPSkinManagerSchema object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPSkinManagerSchema();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to read all properties from resource file
	// Parameters:
	//     pResourceFile - Resource file to read
	// Returns:
	//     S_OK if successful
	//-----------------------------------------------------------------------
	HRESULT ReadProperties();

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns class hash code
	// Parameters:
	//     strClass - Class name
	// Returns:
	//     UINT class hash code
	//-----------------------------------------------------------------------
	static UINT AFX_CDECL GetClassCode(LPCTSTR strClass);

#ifdef _OLD_METHOD_GET_TMS_CODES_VERIFY
	static UINT AFX_CDECL GetClassCodeOld(LPCTSTR strClass);
#endif

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Get the value for the specified string property
	// Parameters:
	//     iClassId    - Class hash code
	//     iPartId     - Part number
	//     iStateId    - State number of part
	//     iPropId     - The property number to get the value for
	//     strVal      - receives the string property value
	// Returns:
	//     S_OK if successful
	//-----------------------------------------------------------------------
	HRESULT GetStringProperty(UINT iClassId, int iPartId, int iStateId, int iPropId, CString& strVal);

	//-----------------------------------------------------------------------
	// Summary:
	//     Get the value for the specified rect property
	// Parameters:
	//     iClassId    - Class hash code
	//     iPartId     - Part number
	//     iStateId    - State number of part
	//     iPropId     - The property number to get the value for
	//     rcVal       - receives the rect property value
	// Returns:
	//     S_OK if successful
	//-----------------------------------------------------------------------
	HRESULT GetRectProperty(UINT iClassId, int iPartId, int iStateId, int iPropId, CRect& rcVal);

	//-----------------------------------------------------------------------
	// Summary:
	//     Get the value for the specified int property
	// Parameters:
	//     iClassId    - Class hash code
	//     iPartId     - Part number
	//     iStateId    - State number of part
	//     iPropId     - The property number to get the value for
	//     iVal        - receives the int property value
	// Returns:
	//     S_OK if successful
	//-----------------------------------------------------------------------
	HRESULT GetIntProperty(UINT iClassId, int iPartId, int iStateId, int iPropId, int& iVal);

	//-----------------------------------------------------------------------
	// Summary:
	//     Get the value for the specified BOOL property
	// Parameters:
	//     iClassId    - Class hash code
	//     iPartId     - Part number
	//     iStateId    - State number of part
	//     iPropId     - The property number to get the value for
	//     bVal        - receives the BOOL property value
	// Returns:
	//     S_OK if successful
	//-----------------------------------------------------------------------
	HRESULT GetBoolProperty(UINT iClassId, int iPartId, int iStateId, int iPropId, BOOL& bVal);

	//-----------------------------------------------------------------------
	// Summary:
	//     Get the value for the specified COLORREF property
	// Parameters:
	//     iClassId    - Class hash code
	//     iPartId     - Part number
	//     iStateId    - State number of part
	//     iPropId     - The property number to get the value for
	//     clrVal      - receives the COLORREF property value
	// Returns:
	//     S_OK if successful
	//-----------------------------------------------------------------------
	HRESULT GetColorProperty(UINT iClassId, int iPartId, int iStateId, int iPropId, COLORREF& clrVal);

	//-----------------------------------------------------------------------
	// Summary:
	//     Get the value for the specified enum property
	// Parameters:
	//     iClassId    - Class hash code
	//     iPartId     - Part number
	//     iStateId    - State number of part
	//     iPropId     - The property number to get the value for
	//     nVal        - receives the enum property value
	// Returns:
	//     S_OK if successful
	//-----------------------------------------------------------------------
	HRESULT GetEnumProperty(UINT iClassId, int iPartId, int iStateId, int iPropId, int& nVal);

	//-----------------------------------------------------------------------
	// Summary:
	//     Get the value for the specified LOGFONT property
	// Parameters:
	//     iClassId    - Class hash code
	//     iPartId     - Part number
	//     iStateId    - State number of part
	//     iPropId     - The property number to get the value for
	//     lfVal       - receives the LOGFONT property value
	// Returns:
	//     S_OK if successful
	//-----------------------------------------------------------------------
	HRESULT GetFontProperty(UINT iClassId, int iPartId, int iStateId, int iPropId, LOGFONT& lfVal);

	//-----------------------------------------------------------------------
	// Summary:
	//     Get the value for the specified CSize property
	// Parameters:
	//     iClassId    - Class hash code
	//     iPartId     - Part number
	//     iStateId    - State number of part
	//     iPropId     - The property number to get the value for
	//     szVal       - Receives the CSize property value
	// Returns:
	//     S_OK if successful
	//-----------------------------------------------------------------------
	HRESULT GetSizeProperty(UINT iClassId, int iPartId, int iStateId, int iPropId, CSize& szVal);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns schema name
	// Returns:
	//     Selected schema name
	//-----------------------------------------------------------------------
	CString GetSchemaName() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves parent skin manager object
	// Returns:
	//     Pointer to CXTPSkinManager object
	//-----------------------------------------------------------------------
	CXTPSkinManager* GetSkinManager() const;

	CXTPSkinManagerResourceFile* GetResourceFile() const;

	CXTPSkinManagerMetrics* GetMetrics() const;


//{{AFX_CODEJOCK_PRIVATE

protected:
	enum FRAMEPART
	{
		frameLeft, frameTop, frameRight, frameBottom
	};

	struct FRAMEREGIONPART
	{
		FRAMEREGIONPART()
		{
			nCenter = 0;
			rcSizingMargins.SetRectEmpty();
		}

		CRgn rgnLeft;
		CRgn rgnRight;
		CRect rcSizingMargins;
		int nCenter;
	};

	struct FRAMEREGION
	{
		int nCaptionHeight;
		int nBorderHeight;
		BOOL bToolWindow;
		BOOL bAutoDelete;

		FRAMEREGIONPART Part[4];
	};

public:
	HICON GetFrameSmIcon(HWND hWnd, BOOL bCheckStyle = TRUE) const;

public:
	static UINT AFX_CDECL GetSchemaPartCode(CString strSchemaPart);
	static UINT AFX_CDECL GetClassPartCode(LPCTSTR lpszClass, LPCTSTR lpszClassPart);

#ifdef _OLD_METHOD_GET_TMS_CODES_VERIFY

	static UINT AFX_CDECL GetClassPartCodeOld(LPCTSTR lpszClass, LPCTSTR lpszClassPart);

#endif // _OLD_METHOD_GET_TMS_CODES_VERIFY

	static UINT AFX_CDECL GetStateCode(LPCTSTR lpszClassPart, LPCTSTR lpszState);
	static int AFX_CDECL GetEnumCode(LPCTSTR lpszProperty, LPCTSTR lpszValue);

#ifdef _OLD_METHOD_GET_TMS_CODES_VERIFY
	static UINT AFX_CDECL GetStateCodeOld(LPCTSTR lpszClassPart, LPCTSTR lpszState);
	static int AFX_CDECL GetEnumCodeOld(LPCTSTR lpszProperty, LPCTSTR lpszValue);
	static UINT AFX_CDECL GetPropertyCodeOld(LPCTSTR lpszProperty, XTPSkinManagerProperty& nPropertyType);
#endif // _OLD_METHOD_GET_TMS_CODES_VERIFY

	static UINT AFX_CDECL GetPropertyCode(LPCTSTR lpszProperty, XTPSkinManagerProperty& nPropertyType);
	static CXTPSkinManagerSchemaProperty* AFX_CDECL CreateProperty(LPCTSTR lpszProperty, XTPSkinManagerProperty nPropertyType, LPCTSTR lpszValue);
	static UINT AFX_CDECL CalculatePropertyCode(UINT iClassId, int iPartId, int iStateId, int iPropId);
protected:
	void RemoveAllProperties();
	CXTPSkinManagerSchemaProperty* GetProperty(UINT iClassId, int iPartId, int iStateId, int iPropId);
	COLORREF GetColor(int nIndex) const;
//}}AFX_CODEJOCK_PRIVATE

public:

//{{AFX_CODEJOCK_PRIVATE
	virtual BOOL DrawThemeBackground(CDC* pDC, CXTPSkinManagerClass* pClass, int iPartId, int iStateId, const RECT *pRect);
	virtual void DrawThemeFrame(CDC* pDC, CXTPSkinObjectFrame* pFrame);
	virtual CRect CalcFrameBorders(CXTPSkinObjectFrame* pFrame);
	virtual CRect CalcFrameBorders(DWORD dwStyle, DWORD dwExStyle);
	virtual HRGN CalcFrameRegion(CXTPSkinObjectFrame* pFrame, CSize sz);
	virtual void RefreshMetrcis();
	void DrawNonClientRect(CDC* pDC, CRect rcFrame, CXTPSkinObjectFrame* pFrame);
	void DrawClientEdge(CDC* pDC, const CRect& rcFrame, CXTPSkinObjectFrame* pFrame);
	virtual void DrawThemeScrollBar(CDC* pDC, CXTPSkinObjectFrame* pFrame, XTP_SKINSCROLLBAR_POSINFO *pSBInfo);
	virtual COLORREF GetScrollBarSizeBoxColor(CXTPSkinObjectFrame* pFrame);
//}}AFX_CODEJOCK_PRIVATE

protected:

//{{AFX_CODEJOCK_PRIVATE
	BOOL DrawThemeBackgroundGlyph(CDC* pDC, CXTPSkinManagerClass* pClass, int iPartId, int iStateId, const RECT *pRect);
	BOOL DrawThemeBackgroundBorder(CDC* pDC, CXTPSkinManagerClass* pClass, int iPartId, int iStateId, const RECT *pRect);
	int FindBestImageGlyphSize(CXTPSkinManagerClass* pClass, int iPartId, int iStateId, const CRect& rcDest, int nImageCount, BOOL bHorizontalImageLayout);

#ifdef _XTP_ACTIVEX
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPSkinManagerSchema);
#endif
//}}AFX_CODEJOCK_PRIVATE

private:
	int GetWindowBorders(LONG lStyle, DWORD dwExStyle, BOOL fWindow, BOOL fClient);
	void FreeFrameRegions();
	void RegionFromBitmap(CBitmap* pBitmap, FRAMEPART nAlign, FRAMEREGIONPART* pPart, const CRect& rc, COLORREF clrTransparent);

	FRAMEREGION* CreateFrameRegion(CXTPSkinObjectFrame* pFrame, CSize sz);
	CSize GetCaptionButtonSize(CXTPSkinManagerClass* pClass, int yButton);
	BOOL DrawWindowPart(CXTPSkinObjectFrame* pFrame, CBitmap& bmp, int iPartId, BOOL bVertical, int nBorderHeight, CRect& rcDest, CRect& rcSizingMargins, COLORREF& clrTransparent);
	int DrawThemeFrameButtons(CDC* pDC, CXTPSkinObjectFrame* pFrame);

public:
	BOOL m_bPreMultiplyImages;

protected:
	CMap<UINT, UINT, CXTPSkinManagerSchemaProperty*, CXTPSkinManagerSchemaProperty*> m_mapProperties;   // Properties collection

	CXTPSkinManager* m_pManager;        // Parent CXTPSkinManager class
	CXTPSkinManagerResourceFile* m_pResourceFile; // Resources.
	CXTPSkinManagerMetrics* m_pMetrics; // Skin metrics.

	CString m_strSchemaName;            // Schema name
	CRITICAL_SECTION m_csDraw;

	CArray<FRAMEREGION*, FRAMEREGION*> m_arrFrameRegions;   // Frame region

protected:
	int m_nGlobalClassId;
	int m_nWindowClassId;

	CMapStringToPtr m_mapClasses;       // Classes collection

	friend class CXTPSkinManagerClass;
	friend class CXTPSkinManager;
};



AFX_INLINE CString CXTPSkinManagerSchema::GetSchemaName() const {
	return m_strSchemaName;
}
AFX_INLINE CXTPSkinManager* CXTPSkinManagerSchema::GetSkinManager() const {
	return m_pManager;
}
AFX_INLINE CXTPSkinManagerResourceFile* CXTPSkinManagerSchema::GetResourceFile() const {
	return m_pResourceFile;
}
AFX_INLINE CXTPSkinManagerMetrics* CXTPSkinManagerSchema::GetMetrics() const {
	return m_pMetrics;
}


#endif // !defined(AFX_SKINMANAGERSCHEMA_H__44A8B96F_B15B_43F7_A734_39C649784B29__INCLUDED_)
