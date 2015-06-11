typedef DWORD (WINAPI* PFNGETLAYOUT)(HDC hdc);
typedef DWORD (WINAPI* PFNSETLAYOUT)(HDC hdc, DWORD dwLayout);


class _XTP_EXT_CLASS CXTPMarkupGdiPlusDeviceContext : public CXTPMarkupDeviceContext
{
public:
	CXTPMarkupGdiPlusDeviceContext(HDC hDC, HDC hAttribDC);
	virtual ~CXTPMarkupGdiPlusDeviceContext();

public:
	virtual void OffsetViewport(const POINT& ptViewortOrg);


	virtual void SetSmoothingMode(XTPMarkupSmoothingMode smoothingMode);

protected:

	Gdiplus::Status GdipCreateBitmapFromHICON_Fixed(HICON hicon, Gdiplus::GpBitmap** bitmap) const;

public:
	virtual void Ellipse(CRect rc, CXTPMarkupStrokeStyle* pStrokeStyle, CXTPMarkupBrush* pFillBrush);
	virtual void DrawRectangle(CRect rc, CXTPMarkupBrush* pBrush, CXTPMarkupThickness* pThickness);
	virtual void FillRectangle(CRect rc, CXTPMarkupBrush* pBrush);
	virtual void DrawLine(int x1, int y1, int x2, int y2, CXTPMarkupStrokeStyle* pStrokeStyle);
	virtual void Polyline(const POINT* points, int nCount, CXTPMarkupStrokeStyle* pStrokeStyle);
	virtual void Polygon(const POINT* points, int nCount, CXTPMarkupStrokeStyle* pStrokeStyle, CXTPMarkupBrush* pFillBrush);
	virtual void DrawPath(CXTPMarkupPathData *pData, CXTPMarkupStrokeStyle* pStrokeStyle, CXTPMarkupBrush* pFillBrush);

	virtual void DrawImage(CXTPImageManagerIcon* pIcon, CRect rc);

	virtual void FillRoundRectangle(CRect rc, CXTPMarkupBrush* pBrush, double* pCornerRadius);
	virtual void DrawRoundRectangle(CRect rc, CXTPMarkupBrush* pBrush, CXTPMarkupThickness* pThickness, double* pCornerRadius);

public:
	static CXTPGdiPlus* GetGdiPlus();
	static void Register(BOOL bInit);


	virtual Gdiplus::GpGraphics* GetGraphics();
	virtual void ReleaseGraphics(Gdiplus::GpGraphics* pGpGraphics);

protected:
	Gdiplus::GpBrush* CreateGpBrush(CXTPMarkupBrush* pBrush, LPCRECT lpRect) const;
	Gdiplus::GpPen* CreateGpPen(CXTPMarkupStrokeStyle* pStrokeStyle, LPCRECT lpRect) const;
	CRect GetPointsBoundRect(const POINT* points, int nCount) const;
	BOOL IsGDIBrush(CXTPMarkupBrush* pBrush) const;

	Gdiplus::GpBitmap *IconToGdiplusBitmap(CXTPImageManagerIcon* pIcon) const;

	Gdiplus::SmoothingMode m_smoothingMode;
};
