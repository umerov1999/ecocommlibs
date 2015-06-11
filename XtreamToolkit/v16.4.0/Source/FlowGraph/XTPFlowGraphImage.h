// XTPFlowGraphImage.h: interface for the CXTPFlowGraphmage class.
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
#if !defined(__XTPFLOWGRAPHIMAGE_H__)
#define __XTPFLOWGRAPHIMAGE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace Gdiplus
{
	class Image;
	class Bitmap;
};

class CXTPImageManager;
class CXTPImageManagerIcon;

class CXTPFlowGraphControl;

// ------------------------------------------------------------
//
// Summary:
//     Object used to represent an Image displayed in a node or
//     connection point.
// ------------------------------------------------------------
class _XTP_EXT_CLASS CXTPFlowGraphImage : public CXTPCmdTarget
{
public:
	// -------------------------------------------
	// Summary:
	//     Constructs a CXTPFlowGraphImage object.
	// -------------------------------------------
	CXTPFlowGraphImage();
	// -------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPFlowGraphImage object, handles cleanup and
	//     deallocation.
	// -------------------------------------------------------------
	virtual ~CXTPFlowGraphImage();

public:
	// ----------------------------------------------------------------------
	// Summary:
	//     Loads an image from a resource.
	// Parameters:
	//     hInstance :      Handle to an instance of the module that contains
	//                      the image to be loaded.
	//     nResourceName :  The ID of the buffer containing the name of the
	//                      resource to load.
	//
	// ----------------------------------------------------------------------
	void LoadFromResource(HINSTANCE hInstance, UINT nResourceName);
	// --------------------------------------------------------------------
	// Summary:
	//     Loads an image from a resource.
	// Parameters:
	//     hInstance :         Handle to an instance of the module that
	//                         contains the image to be loaded.
	//     lpszResourceName :  Pointer to the string containing the name of
	//                         the resource containing the image to load.
	//     lpszResourceType :  The type of resource.
	//
	// --------------------------------------------------------------------
	void LoadFromResource(HINSTANCE hInstance, LPCTSTR lpszResourceName, LPCTSTR lpszResourceType);

	// ------------------------------------------------------------
	//
	// Summary:
	//     Loads an image so that it can be used in the flow graph.
	// Parameters:
	//     lpszFileName :  Filename and path to the file to add.
	// ------------------------------------------------------------
	void LoadFromFile(LPCTSTR lpszFileName);
	// --------------------------------------------------------------------
	// Summary:
	//     Call this method to assign a bitmap handle to CXTPFlowGraphImage
	//     class
	// Parameters:
	//     hBitmap :  Bitmap handle.
	//
	// --------------------------------------------------------------------
	void SetBitmap(HBITMAP hBitmap);
	// -------------------------------------------------------------------
	// Summary:
	//     Call this method to assign an icon handle to CXTPFlowGraphImage
	//     class.
	// Parameters:
	//     hIcon :  Icon Handle to assign.
	// -------------------------------------------------------------------
	void SetIcon(HICON hIcon);

	// -----------------------------------------------------
	// Summary:
	//     Gets the size of this image.
	// Returns:
	//     A CSize object containing the size of this image.
	// -----------------------------------------------------
	CSize GetSize() const;

	// -------------------------------------------------------------------
	// Summary:
	//     Gets a reference to a Gdiplus::Image for this flow graph image.
	// Returns:
	//     Reference to a Gdiplus::Image for this flow graph image.
	// -------------------------------------------------------------------
	Gdiplus::Image* GetImage() const;
	// -------------------------------------------------------------------------
	// Summary:
	//     Gets a reference to a CXTPImageManagerIcon for this flow graph image.
	// Returns:
	//     Reference to a CXTPImageManagerIcon for this flow graph image.
	// -------------------------------------------------------------------------
	CXTPImageManagerIcon* GetIcon() const;

protected:
	// -----------------------------------------------------------------
	// Summary:
	//     This method is called when the image is removed from the flow
	//     graph's collection of images.
	// -----------------------------------------------------------------
	void OnRemoved();
	// -----------------------------------------------------------------
	// Summary:
	//     This method is called whenever the image has been changed.
	// Remarks:
	//     This is called when a bitmap\\icon is set or when an image is
	//     loaded from a resource or file.
	// -----------------------------------------------------------------
	void ImageChanged();

	friend class CXTPFlowGraphImages;

protected:
	Gdiplus::Bitmap* m_pImage; // Reference to a Gdiplus::Image for this flow graph image.
	CXTPImageManagerIcon* m_pIcon; // Reference to a CXTPImageManagerIcon for this flow graph image.

	CSize m_szImage; // Size of the image.
	CXTPFlowGraphImages* m_pParent; // Reference to the collection of images that this image belongs to.

#ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPFlowGraphImage)

	//}}AFX_CODEJOCK_PRIVATE
#endif
	friend class CXTPFlowGraphImages;

	UINT m_nCommandId;
};


// --------------------------------------------------------------------
//
// Summary:
//     Collection of images that can be used in the flow graph.
// Remarks:
//     Images can be loaded and displayed next to the node's caption or
//     connection point captions.
// --------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPFlowGraphImages : public CXTPCmdTarget
{
protected:
	// --------------------------------------------
	// Summary:
	//     Constructs a CXTPFlowGraphImages object.
	// --------------------------------------------
	CXTPFlowGraphImages(CXTPFlowGraphControl* pControl);
	// -------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPFlowGraphImage object, handles cleanup and
	//     deallocation.
	// -------------------------------------------------------------
	virtual ~CXTPFlowGraphImages();

public:
	// ---------------------------------------------------------------------
	//
	// Summary:
	//     Adds an Image object that can then be used to load an image file.
	// Remarks:
	//     AddImage must first be used to create a FlowGraphImage. You can
	//     use this object to load an image file.
	// Parameters:
	//     pPage :  Reference to the page to add this image to.
	//
	// Returns:
	//     Reference to the FlowGraphImage that was just added.
	// ---------------------------------------------------------------------
	CXTPFlowGraphImage* AddImage(CXTPFlowGraphImage* pPage);

	// -------------------------------------------
	//
	// Summary:
	//     Removes all images from the flow graph.
	// -------------------------------------------
	void RemoveAll();

	// ----------------------------------------------------------------
	//
	// Summary:
	//     \Returns the total number of images added to the flow graph.
	// Returns:
	//     \Returns the total number of images added to the flow graph.
	// ----------------------------------------------------------------
	int GetCount() const;
	// --------------------------------------------------------------------
	//
	// Summary:
	//     Gets the specified image from the collection of all images added
	//     to the flow graph.
	// Parameters:
	//     nIndex :  Index of the desired image to return.
	//
	// Returns:
	//     Reference to the specified image.
	// --------------------------------------------------------------------
	CXTPFlowGraphImage* GetAt(int nIndex) const;

	// ---------------------------------------------------------
	// Summary:
	//     Removes all images from the specified page.
	// Parameters:
	//     pPage :  Reference to the page to remove images form.
	// ---------------------------------------------------------
	void Remove(CXTPFlowGraphImage* pPage);
	// ---------------------------------------------------------------
	// Summary:
	//     Removes the specified images from the collection of images.
	// Parameters:
	//     nIndex :  Index of the image to remove.
	// ---------------------------------------------------------------
	void RemoveAt(int nIndex);

	// --------------------------------------------------------------------
	// Summary:
	//     Retrieves Image manager associated with the Glow Graph.
	// Returns:
	//     Pointer to the image manager object that stores the images in
	//     the flow graph.
	// --------------------------------------------------------------------
	CXTPImageManager* GetImageManager() const;

protected:
	CArray<CXTPFlowGraphImage*, CXTPFlowGraphImage*> m_arrImages; // List of images for the Flow Graph
	CXTPImageManager* m_pImageManager; // Pointer to ImageManager associated with the Flow Graph.

	CXTPFlowGraphControl* m_pControl;

	friend class CXTPFlowGraphControl;

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPFlowGraphImages)
	DECLARE_ENUM_VARIANT(CXTPFlowGraphImages)

	afx_msg int OleGetItemCount();
	afx_msg LPDISPATCH OleGetItem(int nIndex);
	afx_msg LPDISPATCH OleAddImage();
//}}AFX_CODEJOCK_PRIVATE
#endif
};

AFX_INLINE int CXTPFlowGraphImages::GetCount() const {
	return (int)m_arrImages.GetSize();
}
AFX_INLINE CXTPFlowGraphImage* CXTPFlowGraphImages::GetAt(int nIndex) const {
	return nIndex >= 0 && nIndex < m_arrImages.GetSize() ? m_arrImages[nIndex] : NULL;
}
AFX_INLINE CXTPImageManager* CXTPFlowGraphImages::GetImageManager() const {
	return m_pImageManager;
}
AFX_INLINE CSize CXTPFlowGraphImage::GetSize() const {
	return m_szImage;
}

#endif //#if !defined(__XTPFLOWGRAPHIMAGE_H__)
