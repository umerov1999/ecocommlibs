// XTPMarkupImage.h: interface for the CXTPMarkupImage class.
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
#if !defined(__XTPMARKUPIMAGE_H__)
#define __XTPMARKUPIMAGE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CXTPImageManagerIcon;
class CXTPImageManager;


//===========================================================================
// Summary: CXTPMarkupImage is CXTPMarkupFrameworkElement derived class. It implements Image XAML Tag
//===========================================================================
class _XTP_EXT_CLASS CXTPMarkupImage : public CXTPMarkupFrameworkElement
{
	DECLARE_MARKUPCLASS(CXTPMarkupImage)

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPMarkupImage object
	//-----------------------------------------------------------------------
	CXTPMarkupImage();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPMarkupImage object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPMarkupImage();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set source of the image to load.
	// Parameters:
	//     lpszSource - String contained URI of image to load
	// Example:
	// <code>
	//     pImage->SetSource("file://c:\\image.bmp); // Load from file
	//     pImage->SetSource("res://#125"); // Load from resources
	//     pImage->SetSource("125");  // Use Image manager index
	//  </code>
	//-----------------------------------------------------------------------
	void SetSource(LPCTSTR lpszSource);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns source of the image to load
	// Returns:
	//     String contained URI of image to load
	//-----------------------------------------------------------------------
	CString GetSource();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set how image will be stretched inside bondings of the image
	// Parameters:
	//     stretch - Stretch type
	//-----------------------------------------------------------------------
	void SetStretch(XTPMarkupStretch stretch);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines current stretch type of the image
	// Returns:
	//     Stretch type
	//-----------------------------------------------------------------------
	XTPMarkupStretch GetStretch() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns original size of the image, not scalled
	//-----------------------------------------------------------------------
	CSize GetOriginalSize() const;

//{{AFX_CODEJOCK_PRIVATE
	// Implementation
public:
	virtual CSize MeasureOverride(CXTPMarkupDrawingContext* pDC, CSize constraint);
	virtual CSize ArrangeOverride(CSize arrangeSize);
	virtual void OnRender(CXTPMarkupDrawingContext* pDC);

protected:
	void LoadImage(int cx);
	CSize MeasureArrangeHelper(CSize inputSize);


protected:
	CXTPImageManager* m_pImageManager;
	CXTPImageManagerIcon* m_pImage;

public:
	static CXTPMarkupDependencyProperty* m_pSourceProperty;
	static CXTPMarkupDependencyProperty* m_pStretchProperty;
//}}AFX_CODEJOCK_PRIVATE

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
public:
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPMarkupImage);

	afx_msg void OleSetSource(LPCTSTR lpszSource);
	afx_msg BSTR OleGetSource();
	afx_msg long OleGetStretch();
	afx_msg void OleSetStretch(long stretch);
//}}AFX_CODEJOCK_PRIVATE
#endif
};

AFX_INLINE XTPMarkupStretch CXTPMarkupImage::GetStretch() const {
	CXTPMarkupEnum* pValue =  MARKUP_STATICCAST(CXTPMarkupEnum, GetValue(m_pStretchProperty));
	return pValue != NULL ? (XTPMarkupStretch)(int)*pValue : xtpMarkupStretchNone;
}
AFX_INLINE void CXTPMarkupImage::SetStretch(XTPMarkupStretch stretch) {
	SetValue(m_pStretchProperty, CXTPMarkupEnum::CreateValue(stretch));
}


#endif // !defined(__XTPMARKUPIMAGE_H__)
