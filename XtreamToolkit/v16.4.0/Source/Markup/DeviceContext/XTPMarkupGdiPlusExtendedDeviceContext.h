class _XTP_EXT_CLASS CXTPMarkupGdiPlusExtendedDeviceContext : public CXTPMarkupGdiPlusDeviceContext
{
public:
	CXTPMarkupGdiPlusExtendedDeviceContext(Gdiplus::GpGraphics* pGpGraphics);
	virtual ~CXTPMarkupGdiPlusExtendedDeviceContext();

public:
	virtual Gdiplus::GpGraphics* GetGraphics();
	virtual void ReleaseGraphics(Gdiplus::GpGraphics* pGpGraphics);

	virtual void GetClipBox(LPRECT lpRect);
	virtual void IntersectClipRect(const RECT& rcLayoutClip);
	virtual HRGN SaveClipRegion();
	virtual void RestoreClipRegion(HRGN hRgn);
	virtual void OffsetViewport(const POINT& ptViewortOrg);
	virtual SIZE MeasureString(LPCWSTR lpszText, int nCount) const;
	virtual void DrawString(LPCWSTR lpszString, UINT nCount, LPCRECT lpRect);
	virtual void SetTextColor(CXTPMarkupBrush* pBrush);
	virtual void SetTextFont(CXTPMarkupFont* pFont);

	HDC GetDC();
	void ReleaseDC(HDC hDC);

protected:
	Gdiplus::GpGraphics* m_pGraphics;
	Gdiplus::GpFont* m_pGpFont;
	Gdiplus::GpBrush* m_pGpTextBrush;
	Gdiplus::GpStringFormat* m_pGpStringFormat;
};
