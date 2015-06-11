//===========================================================================
// Summary:
//     Standalone class used to manage Image resources.
//===========================================================================
class CXTPImageManagerResource
{
protected:


public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPImageManagerResource object used to create common bitmap for all icons of image manager
	// Parameters:
	//      pImageManager : Pointer to Parent ImageManager class
	//      szIcon : Size of images that will be managed by this resource
	//---------------------------------------------------------------
	CXTPImageManagerResource(CXTPImageManager* pImageManager, CSize szIcon);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPImageManagerResource object, handles cleanup and de-
	//     allocation.
	//-----------------------------------------------------------------------
	~CXTPImageManagerResource();

	//---------------------------------------------------------------
	// Summary:
	//      This method is called to add new icon to common bitmap fo resource manger
	// Parameters:
	//      hHandle : Handle of image to add
	// Returns:
	//      Returns pointer to XTP_IMAGERESOURCE_INFO contained position and style of new icon.
	//---------------------------------------------------------------
	XTP_IMAGERESOURCE_INFO* Add(const CXTPImageManagerIconHandle& hHandle);

	//---------------------------------------------------------------
	// Summary:
	//     Clears image in specified position
	// Parameters:
	//      pRInfo : Posiotion of image to clear
	//---------------------------------------------------------------
	void Clear(XTP_IMAGERESOURCE_INFO* pRInfo);

	//---------------------------------------------------------------
	// Summary:
	//     This method is called to draw image.
	// Parameters:
	//      pDC - Pointer to a valid device context.
	//      pRInfo : pointer to XTP_IMAGERESOURCE_INFO
	//      pt : point to start draw
	//      szIcon : size of icon
	//      bDrawComposited : BOOL flag of composite or not way of draw
	//---------------------------------------------------------------
	void Draw(CDC* pDC, XTP_IMAGERESOURCE_INFO* pRInfo, CPoint pt, CSize szIcon, BOOL bDrawComposited);

	//---------------------------------------------------------------
	// Summary:
	//      Creates new icon from bitmap specified by pRInfo parameter
	// Parameters:
	//      hHandle : Handle of icon to be created
	//      pRInfo : Posiotion of icon inside m_hbmpImage to create
	//---------------------------------------------------------------
	void CreateIcon(CXTPImageManagerIconHandle& hHandle, XTP_IMAGERESOURCE_INFO* pRInfo);
	//---------------------------------------------------------------
	// Summary:
	//     Returns size of iconsmanaged by this Resource
	// Returns:
	//      Size of images contained inside m_hbmpImage
	//---------------------------------------------------------------
	CSize GetExtent() const;

private:
	XTP_IMAGERESOURCE_INFO* AllocBitmap();
	void RemoveAll();
	void DrawIconComposited(CDC* pDC, XTP_IMAGERESOURCE_INFO* pRInfo, CPoint pt, CSize szIcon);

protected:
	HBITMAP m_hbmpImage; // Handle of bitmap contained all images of the resources
	HBITMAP m_hbmpMask;  // Handle of bitmap contained mask for images of the resources
	CXTPBitmapDC m_dc;      // Helper device context class for quick image select

	BYTE* m_pBits;       // Bits of m_hbmpImage
	CXTPImageManager* m_pImageManager; // Parent CXTPImageManager object
	CSize m_szIcon;                    // Size of images of the resources
	int m_nAllocWidth;                 // Total allocated with of the resources
	CArray<XTP_IMAGERESOURCE_INFO*, XTP_IMAGERESOURCE_INFO*> m_arrIcons; // List of used icons
	CArray<XTP_IMAGERESOURCE_INFO*, XTP_IMAGERESOURCE_INFO*> m_arrAlloc; // List of free icons
	CRITICAL_SECTION m_cs; // Critical section for multithreading support.

private:
	friend class CXTPImageManager;
	friend class CXTPImageManagerIconHandle;
	friend class CXTPImageManagerIcon;
};
