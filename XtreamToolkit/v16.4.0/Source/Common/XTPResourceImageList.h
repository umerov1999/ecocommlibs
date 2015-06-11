class _XTP_EXT_CLASS CXTPResourceImageList : public CXTPResourceImages
{
	DECLARE_DYNCREATE(CXTPResourceImageList)
	friend class CXTPResourceImage;
public:
	CXTPResourceImageList();
	virtual ~CXTPResourceImageList();

	CXTPResourceImage* GetBitmap(UINT nID);

	CXTPResourceImage* SetBitmap(HBITMAP hBitmap, UINT nID, BOOL bAlptha, BOOL bCopyBitmap = TRUE);

	BOOL SetBitmap(CXTPResourceImage* pImage, UINT nID, BOOL bCallAddRef);

	BOOL LoadBitmap(LPCTSTR lpcszPath, UINT nID);

	BOOL Remove(UINT nID);
	void RemoveAll();

protected:
	CMap<UINT, UINT, CXTPResourceImage*, CXTPResourceImage*> m_mapID2Image;

protected:
#ifdef _XTP_ACTIVEX
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPResourceImageList);

	void OnFinalRelease();


	// Only for Calendar package
#ifdef XTP_CALENDAR_SITENOTIFY_KEY
	DECLARE_OLECREATE_EX(CXTPResourceImageList)
#endif

	BOOL OleAddBitmap(OLE_HANDLE ohBitmap, UINT nID);
#endif
};
