// XTPResourceImage.h: interface for the CXTPResourceImage class.
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
#if !defined(__XTPRESOURCEIMAGE_H__)
#define __XTPRESOURCEIMAGE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//{{AFX_CODEJOCK_PRIVATE

// Private image wrappers:

#define XTP_DI_COMPOSITE 1

class CXTPResourceImages;
class CXTPResourceImageList;
class CXTPNotifyConnection;

class _XTP_EXT_CLASS CXTPResourceImage : public CXTPCmdTarget
{
public:
	CXTPResourceImage(CXTPResourceImages* pImages);
	virtual ~CXTPResourceImage();

public:
	BOOL LoadFile(HMODULE hModule, LPCTSTR lpszBitmapFileName);
	void SetBitmap(HBITMAP hBitmap, BOOL bAlpha = FALSE);

	void DrawImage(CDC* pDC, const CRect& rcDest, CRect rcSrc, const CRect& rcSizingMargins, COLORREF clrTransparent, DWORD dwFlags);
	void DrawImage(CDC* pDC, const CRect& rcDest, CRect rcSrc, const CRect& rcSizingMargins, COLORREF clrTransparent);
	void DrawImage(CDC* pDC, const CRect& rcDest, CRect rcSrc, CRect rcSizingMargins);
	void DrawImage(CDC* pDC, const CRect& rcDest, CRect rcSrc);

	int GetHeight() const;
	int GetWidth() const;
	CSize GetExtent() const;

	BOOL IsAlphaImage() const;

	void PreserveOrientation() {
		m_bImageOrientaionPreserved = TRUE;
	}

	CRect GetSource(int nState = 0, int nCount = 1) const;

	BOOL GetBitmapPixel(CPoint pt, COLORREF& clrPixel);

	void Premultiply();

	void DrawImageFlipped(BOOL bFlip);


protected:
	BOOL DrawImagePart(CDC* pDCDest, CRect rcDest,
		CDC* pDCSrc, CRect rcSrc) const;

	BOOL DrawImageTile(CDC* pDCDest, const CRect& rcDest,
		CDC* pDCSrc, const CRect& rcSrc, BOOL bTile) const;

	void InvertBitmap();
	BOOL ConvertToBits(HBITMAP hBitmap);

protected:
	LPVOID m_pBits;
	BOOL m_bAlpha;
	BOOL m_bMultiply;
	BOOL m_bImageOrientaionPreserved;
	BOOL m_bInvert;
	CSize m_szExtent;
	BOOL m_bFlipped;

	CXTPResourceImages* m_pImages;

	friend class CXTPResourceImages;
	friend class CXTPResourceImageList;
};

class _XTP_EXT_CLASS CXTPResourceImages : public CXTPCmdTarget
{
private:
	class CBitmapDC : public CDC
	{
	public:
		CBitmapDC()
		{
			m_hOldBitmap = 0;
		}
		~CBitmapDC()
		{
			if (m_hOldBitmap)
			{
				::SelectObject(GetSafeHdc(), m_hOldBitmap);
			}
		}
		void SetBitmap(HBITMAP hBitmap)
		{
			if (hBitmap)
			{
				m_hOldBitmap = (HBITMAP)::SelectObject(GetSafeHdc(), hBitmap);
			}
			else if (m_hOldBitmap)
			{
				::SelectObject(GetSafeHdc(), m_hOldBitmap);
				m_hOldBitmap = NULL;
			}

		}
		HBITMAP m_hOldBitmap;
	};
public:
	CXTPResourceImages();
	~CXTPResourceImages();
public:
	void RemoveAll();

	CXTPResourceImage* LoadFile(LPCTSTR lpszImageFile);

	BOOL SetHandle(HMODULE hResource, LPCTSTR lpszIniFileName = NULL, BOOL bFreeOnRelease = FALSE);
	BOOL SetHandle(LPCTSTR lpszDllFileName, LPCTSTR lpszIniFileName = NULL);
	BOOL InitResourceHandle(LPCTSTR lpszTestImageFile, LPCTSTR lpResType = RT_BITMAP);

	COLORREF GetImageColor(LPCTSTR lpszSectionName, LPCTSTR lpszKeyName, COLORREF clrDefault = (COLORREF)-1) const;
	int GetImageInt(LPCTSTR lpszSectionName, LPCTSTR lpszKeyName, int nDefault = -1) const;
	CRect GetImageRect(LPCTSTR lpszSectionName, LPCTSTR lpszKeyName, CRect rcDefault = CRect(0, 0, 0, 0)) const;

	CString GetImageString(LPCTSTR lpszSectionName, LPCTSTR lpszKeyName) const;
	CString GetImageValue(LPCTSTR lpszSectionName, LPCTSTR lpszKeyName) const;
	BOOL IsValid() const;
	void AssertValid();

	CXTPNotifyConnection* GetConnection();

	CString m_strDllFileName;
	CString m_strIniFileName;

protected:
	CXTPResourceImage* LoadImage(LPCTSTR strImageFile);
	CString _ImageNameToResourceName(LPCTSTR strImageFile);

	void LoadPropeties();

private:
	BOOL ReadString(CString& str, LPSTR& lpTextFile);
	BOOL LoadResources();

	static BOOL CALLBACK EnumResNameProc(HMODULE hModule,
		LPCTSTR lpszType, LPTSTR lpszName, LONG_PTR lParam);
	HRSRC FindResourceIniFile(HMODULE hResource);

	UINT GetKeyHash(LPCTSTR lpszSection, LPCTSTR lpszKeyName) const;


protected:
	HMODULE m_hResource;
	LPSTR m_lpTextFile;
	LPSTR m_lpTextFileEnd;
	BOOL m_bFreeOnRelease;
	CRITICAL_SECTION m_cs;
	BOOL m_bPremultiplyImages;

	CBitmapDC m_dc;
	HBITMAP m_hbmLayer;
	LPDWORD m_pLayerBits;

	CXTPNotifyConnection* m_pConnection;

	CMap<UINT, UINT, CString, CString> m_mapPropeties;

protected:
	CMapStringToPtr m_mapImages;



	friend class CXTPResourceImage;

};

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
};

_XTP_EXT_CLASS CXTPResourceImages* AFX_CDECL XTPResourceImages();

//}}AFX_CODEJOCK_PRIVATE

#endif // !defined(__XTPRESOURCEIMAGE_H__)
