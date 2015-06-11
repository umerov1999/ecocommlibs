// XTPChartPalette.h
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
#if !defined(__XTPCHARTPALETTE_H__)
#define __XTPCHARTPALETTE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//===========================================================================
// Summary:
//     This class represents a palette entry, which abstracts two 32 bit color
//     values of type ARGB.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartPaletteEntry
{
public:
	CXTPChartColor Color;
	CXTPChartColor Color2;
};

//===========================================================================
// Summary:
//     This type represents a collection of CXTPChartPaletteEntry objects, the
//     container here used is CArray.
// Remarks:
//===========================================================================
typedef CArray<CXTPChartPaletteEntry, CXTPChartPaletteEntry&> CXTPChartPaletteEntries;

//===========================================================================
// Summary:
//     This class abstracts a color palette.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartPalette : public CXTPChartElement
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartPalette object.
	// Parameters:
	//     pOwner - The owner element of the palette object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartPalette(CXTPChartElement* pOwner);

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartPalette object, handles cleanup.
	//-------------------------------------------------------------------------
	virtual ~CXTPChartPalette();

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the first color at a particular index in
	//     the palette.
	// Parameters:
	//     nIndex - The zero based index of the required palette entry.
	//-------------------------------------------------------------------------
	CXTPChartColor GetColor(int nIndex) const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the second color at a particular index in
	//     the palette.
	// Parameters:
	//     nIndex - The zero based index of the required palette entry.
	//-------------------------------------------------------------------------
	CXTPChartColor GetColor2(int nIndex) const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get number of entris in the palette.
	// Returns:
	//     An integer value specifying the number of entries in the palette.
	//-------------------------------------------------------------------------
	int GetCount() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the palette entries.
	// Returns:
	//     A reference to the palette entry collection member.
	//-------------------------------------------------------------------------
	CXTPChartPaletteEntries& GetEntries();


public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to Store/Load the properties of palette object
	//      using the specified data object.
	// Parameters:
	//     pPX - Source or destination CXTPPropExchange data object reference.
	// Remarks:
	//     This member function is used to store or load property data to or
	//     from a storage.
	//-----------------------------------------------------------------------
	void DoPropExchange(CXTPPropExchange* pPX);


#ifdef _XTP_ACTIVEX
public:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartPalette);

	afx_msg OLE_COLOR OleGetColor(int nIndex);
	afx_msg OLE_COLOR OleGetColor2(int nIndex);
	afx_msg void OleSetColor(int nIndex, OLE_COLOR clr);
	afx_msg void OleSetColor2(int nIndex, OLE_COLOR clr);
	afx_msg void OleDeleteAll();
	afx_msg void OleAddEntry(OLE_COLOR clr, OLE_COLOR clr2);
	afx_msg int OleGetCount();
//}}AFX_CODEJOCK_PRIVATE
#endif

protected:
	CXTPChartPaletteEntries m_arrEntries;    //The palette entry collection object.
};

BOOL AFX_CDECL PX_Color(CXTPPropExchange* pPX, LPCTSTR pszPropName, CXTPChartColor& clrValue);

AFX_INLINE int CXTPChartPalette::GetCount() const {
	return (int)m_arrEntries.GetSize();
}
AFX_INLINE CXTPChartPaletteEntries& CXTPChartPalette::GetEntries() {
	return m_arrEntries;
}


#endif //#if !defined(__XTPCHARTPALETTE_H__)
