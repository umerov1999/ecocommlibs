#include <StdAfx.h>
#include <Common/XTPImageManager.h>
#include <Common/XTPResourceImage.h>
#include <Common/XTPResourceImageList.h>

//////////////////////////////////////////////////////////////////////////
// CXTPResourceImageList

IMPLEMENT_DYNCREATE(CXTPResourceImageList, CCmdTarget)

CXTPResourceImageList::CXTPResourceImageList()
{
#ifdef _XTP_ACTIVEX
	EnableAutomation();
#endif
}

CXTPResourceImageList::~CXTPResourceImageList()
{
	RemoveAll();
}

CXTPResourceImage* CXTPResourceImageList::SetBitmap(HBITMAP hBitmap, UINT nID, BOOL bAlptha, BOOL bCopyBitmap)
{
	ASSERT(hBitmap);
	if (!hBitmap)
		return NULL;

	HBITMAP hBmp2 = hBitmap;
	if (bCopyBitmap)
	{
		// HBITMAP CXTPImageManagerIcon::CopyAlphaBitmap(HBITMAP hBitmap)
		hBmp2 = (HBITMAP)::CopyImage(hBitmap, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
		ASSERT(hBmp2);
		if (!hBmp2)
			return NULL;
	}

	BITMAP bmpinfo;
	if (!::GetObject(hBmp2, sizeof(bmpinfo), &bmpinfo))
	{
		ASSERT(FALSE);
		::DeleteObject(hBmp2);
		return NULL;
	}

	CXTPResourceImage* pImage = new CXTPResourceImage(this);
	if (!pImage)
		return NULL;

	pImage->SetBitmap(hBmp2, bAlptha);

	SetBitmap(pImage, nID, FALSE);

	return pImage;
}

BOOL CXTPResourceImageList::SetBitmap(CXTPResourceImage* pImage, UINT nID, BOOL bCallAddRef)
{
	ASSERT(pImage);
	if (!pImage)
		return FALSE;

	if (bCallAddRef)
		CMDTARGET_ADDREF(pImage);

	// free prev. image if such exists
	CXTPResourceImage* pImage_old = GetBitmap(nID);
	CMDTARGET_RELEASE(pImage_old);

	m_mapID2Image.SetAt(nID, pImage);

	return TRUE;
}

CXTPResourceImage* CXTPResourceImageList::GetBitmap(UINT nID)
{
	CXTPResourceImage* pImage = NULL;
	if (!m_mapID2Image.Lookup(nID, pImage))
		return NULL;

	return pImage;
}

BOOL CXTPResourceImageList::Remove(UINT nID)
{
	CString strImageFile;
	CXTPResourceImage* pImage = NULL;

	//-------------------------------------------------
	if (!m_mapID2Image.Lookup(nID, pImage))
		return FALSE;

	m_mapID2Image.RemoveKey(nID);
	CMDTARGET_RELEASE(pImage);

	return TRUE;
}

void CXTPResourceImageList::RemoveAll()
{
	UINT nID;
	CXTPResourceImage* pImage;

	POSITION pos = m_mapID2Image.GetStartPosition();
	while (pos != NULL)
	{
		m_mapID2Image.GetNextAssoc( pos, nID, pImage);
		CMDTARGET_RELEASE(pImage);
	}

	m_mapID2Image.RemoveAll();

	m_mapPropeties.RemoveAll();
}

BOOL CXTPResourceImageList::LoadBitmap(LPCTSTR lpcszPath, UINT nID)
{
	BOOL bAlphaBitmap = FALSE;
	HBITMAP hBmp = CXTPImageManagerIcon::LoadBitmapFromFile(lpcszPath, &bAlphaBitmap);

	CXTPResourceImage* pImage = NULL;
	if (hBmp)
	{
		pImage = SetBitmap(hBmp, nID, bAlphaBitmap, FALSE);
	}

	return !!pImage;
}


#ifdef _XTP_ACTIVEX

// {FB788B9F-E371-4690-ACF6-538F88C33741}
static const GUID IID_IResourceImageList =
{ 0xfb788b9f, 0xe371, 0x4690, { 0xac, 0xf6, 0x53, 0x8f, 0x88, 0xc3, 0x37, 0x41 } };

BEGIN_INTERFACE_MAP(CXTPResourceImageList, CCmdTarget)
	INTERFACE_PART(CXTPResourceImageList, IID_IResourceImageList, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPResourceImageList, IID_IResourceImageList)

BEGIN_DISPATCH_MAP(CXTPResourceImageList, CCmdTarget)
	DISP_FUNCTION_ID(CXTPResourceImageList, "AddBitmap",  1, OleAddBitmap,VT_BOOL,  VTS_I4   VTS_I4)
	DISP_FUNCTION_ID(CXTPResourceImageList, "LoadBitmap", 2, LoadBitmap,  VT_BOOL,  VTS_BSTR VTS_I4)
	DISP_FUNCTION_ID(CXTPResourceImageList, "Remove",     3, Remove,      VT_BOOL,  VTS_I4)
	DISP_FUNCTION_ID(CXTPResourceImageList, "RemoveAll",  4, RemoveAll,   VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()

BOOL CXTPResourceImageList::OleAddBitmap(OLE_HANDLE ohBitmap, UINT nID)
{
	CXTPResourceImage* pImage = SetBitmap((HBITMAP)ohBitmap, nID, TRUE);
	return !!pImage;
}

void CXTPResourceImageList::OnFinalRelease()
{
	CCmdTarget::OnFinalRelease();
}

#endif
