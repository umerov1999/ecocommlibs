#define DECLARE_GDIPLUS_METHOD(val, calltype, proc, proc_param, call_param)\
	inline GpStatus calltype proc##proc_param\
	{\
		typedef Status (calltype* PFN##proc)##proc_param;\
		if (!m_ptrMethods[val])\
		{\
			m_ptrMethods[val] = GetProcAddress(m_hModule, #proc);\
		}\
		return ((PFN##proc)m_ptrMethods[val])##call_param;\
	}

#define DECLARE_GDIPLUS_METHOD_VOID(val, calltype, proc, proc_param, call_param)\
	inline void calltype proc##proc_param\
	{\
		typedef void (calltype* PFN##proc)##proc_param;\
		if (!m_ptrMethods[val])\
		{\
			m_ptrMethods[val] = GetProcAddress(m_hModule, #proc);\
		}\
		((PFN##proc)m_ptrMethods[val])##call_param;\
	}


class CXTPGdiPlus
{
public:
	CXTPGdiPlus()
	{
		m_hModule = NULL;
		m_nGdiplusToken = NULL;
		m_nCount = 0;
		ZeroMemory(&m_ptrMethods, sizeof(m_ptrMethods));
	}

public:
	void Register(BOOL bInit);

	DECLARE_GDIPLUS_METHOD(0, WINAPI, GdiplusStartup, (ULONG_PTR *token, const GdiplusStartupInput *input, GdiplusStartupOutput *output), (token, input, output))
	DECLARE_GDIPLUS_METHOD_VOID(1, WINAPI, GdiplusShutdown, (ULONG_PTR token), (token))

	DECLARE_GDIPLUS_METHOD(2, WINGDIPAPI, GdipSetPixelOffsetMode, (GpGraphics* graphics, PixelOffsetMode pixelOffsetMode), (graphics, pixelOffsetMode))
	DECLARE_GDIPLUS_METHOD(3, WINGDIPAPI, GdipSetPageUnit, (GpGraphics *graphics, GpUnit unit), (graphics, unit))
	DECLARE_GDIPLUS_METHOD(4, WINGDIPAPI, GdipSetSmoothingMode, (GpGraphics *graphics, SmoothingMode smoothingMode), (graphics, smoothingMode))
	DECLARE_GDIPLUS_METHOD(5, WINGDIPAPI, GdipSetLinePresetBlend, (GpLineGradient *brush, GDIPCONST ARGB *blend, GDIPCONST REAL* positions, INT count), (brush, blend, positions, count))

	DECLARE_GDIPLUS_METHOD(6, WINGDIPAPI, GdipCreateFromHDC2, (HDC hdc, HANDLE hDevice, GpGraphics **graphics), (hdc, hDevice, graphics))
	DECLARE_GDIPLUS_METHOD(7, WINGDIPAPI, GdipCreateFromHDC, (HDC hdc, GpGraphics **graphics), (hdc, graphics))
	DECLARE_GDIPLUS_METHOD(8, WINGDIPAPI, GdipCreatePath2, (GDIPCONST GpPointF* points, GDIPCONST BYTE* data, INT dataCount, GpFillMode fillMode, GpPath **path), (points, data, dataCount, fillMode, path))
	DECLARE_GDIPLUS_METHOD(9, WINGDIPAPI, GdipCreateSolidFill, (ARGB color, GpSolidFill **brush), (color, brush))

	DECLARE_GDIPLUS_METHOD(10, WINGDIPAPI, GdipCreateLineBrush, (GDIPCONST GpPointF* point1, GDIPCONST GpPointF* point2, ARGB color1, ARGB color2, GpWrapMode wrapMode, GpLineGradient **lineGradient), (point1, point2, color1, color2, wrapMode, lineGradient))
	DECLARE_GDIPLUS_METHOD(11, WINGDIPAPI, GdipCreatePen1, (ARGB color, REAL width, GpUnit unit, GpPen **pen), (color, width, unit, pen))
	DECLARE_GDIPLUS_METHOD(12, WINGDIPAPI, GdipCreatePen2, (GpBrush *brush, REAL width, GpUnit unit, GpPen **pen), (brush, width, unit, pen))

	DECLARE_GDIPLUS_METHOD(13, WINGDIPAPI, GdipDeleteGraphics, (GpGraphics *graphics), (graphics))
	DECLARE_GDIPLUS_METHOD(14, WINGDIPAPI, GdipDeleteBrush, (GpBrush *brush), (brush))
	DECLARE_GDIPLUS_METHOD(15, WINGDIPAPI, GdipDeletePen, (GpPen *pen), (pen))
	DECLARE_GDIPLUS_METHOD(16, WINGDIPAPI, GdipDeletePath, (GpPath* path), (path))

	DECLARE_GDIPLUS_METHOD(17, WINGDIPAPI, GdipDrawEllipse, (GpGraphics *graphics, GpPen *pen, REAL x, REAL y, REAL width, REAL height), (graphics, pen, x, y, width, height))
	DECLARE_GDIPLUS_METHOD(18, WINGDIPAPI, GdipDrawLinesI, (GpGraphics *graphics, GpPen *pen, GDIPCONST GpPoint *points, INT count), (graphics, pen, points, count))
	DECLARE_GDIPLUS_METHOD(19, WINGDIPAPI, GdipDrawPolygonI, (GpGraphics *graphics, GpPen *pen, GDIPCONST GpPoint *points, INT count), (graphics, pen, points, count))
	DECLARE_GDIPLUS_METHOD(20, WINGDIPAPI, GdipDrawPath, (GpGraphics *graphics, GpPen *pen, GpPath *path), (graphics, pen, path))
	DECLARE_GDIPLUS_METHOD(21, WINGDIPAPI, GdipDrawLineI, (GpGraphics *graphics, GpPen *pen, INT x1, INT y1, INT x2, INT y2), (graphics, pen, x1, y1, x2, y2))

	DECLARE_GDIPLUS_METHOD(22, WINGDIPAPI, GdipFillEllipse, (GpGraphics *graphics, GpBrush *brush, REAL x, REAL y, REAL width, REAL height), (graphics, brush, x, y, width, height))
	DECLARE_GDIPLUS_METHOD(23, WINGDIPAPI, GdipFillPolygonI, (GpGraphics *graphics, GpBrush *brush, GDIPCONST GpPoint *points, INT count, GpFillMode fillMode), (graphics, brush, points, count, fillMode))
	DECLARE_GDIPLUS_METHOD(24, WINGDIPAPI, GdipFillRectangleI, (GpGraphics *graphics, GpBrush *brush, INT x, INT y, INT width, INT height), (graphics, brush, x, y, width, height))
	DECLARE_GDIPLUS_METHOD(25, WINGDIPAPI, GdipFillPath, (GpGraphics *graphics, GpBrush *brush, GpPath *path), (graphics, brush, path))
	DECLARE_GDIPLUS_METHOD(26, WINGDIPAPI, GdipGetClipBoundsI, (GpGraphics *graphics, GpRect *rect), (graphics, rect))

	DECLARE_GDIPLUS_METHOD(27, WINGDIPAPI, GdipCreateFontFromLogfontA, (HDC hdc, LOGFONTA *logfont, GpFont **font), (hdc, logfont, font))
	DECLARE_GDIPLUS_METHOD(28, WINGDIPAPI, GdipCreateFontFromLogfontW, (HDC hdc, LOGFONTW *logfont, GpFont **font), (hdc, logfont, font))
	DECLARE_GDIPLUS_METHOD(29, WINGDIPAPI, GdipDeleteFont, (GpFont* font), (font))

	DECLARE_GDIPLUS_METHOD(30, WINGDIPAPI, GdipMeasureString, ( GpGraphics *graphics, GDIPCONST WCHAR *string, INT length, GDIPCONST GpFont *font, GDIPCONST RectF *layoutRect, GDIPCONST GpStringFormat *stringFormat, RectF *boundingBox, INT *codepointsFitted, INT *linesFilled ),
		(graphics, string, length, font, layoutRect, stringFormat, boundingBox, codepointsFitted, linesFilled))

	DECLARE_GDIPLUS_METHOD(31, WINGDIPAPI, GdipTranslateWorldTransform, (GpGraphics *graphics, REAL dx, REAL dy, GpMatrixOrder order), (graphics, dx, dy, order))

	DECLARE_GDIPLUS_METHOD(32, WINGDIPAPI, GdipDeleteStringFormat, (GpStringFormat *format), (format))
	DECLARE_GDIPLUS_METHOD(33, WINGDIPAPI, GdipCreateStringFormat, ( INT formatAttributes, LANGID language, GpStringFormat **format ), (formatAttributes, language, format))

	DECLARE_GDIPLUS_METHOD(34, WINGDIPAPI, GdipGetDC, (GpGraphics* graphics, HDC * hdc), (graphics, hdc))
	DECLARE_GDIPLUS_METHOD(35, WINGDIPAPI, GdipReleaseDC, (GpGraphics* graphics, HDC hdc), (graphics, hdc))

	DECLARE_GDIPLUS_METHOD(36, WINGDIPAPI, GdipDrawString, ( GpGraphics *graphics, GDIPCONST WCHAR *string, INT length, GDIPCONST GpFont *font, GDIPCONST RectF *layoutRect, GDIPCONST GpStringFormat *stringFormat, GDIPCONST GpBrush *brush ),
		( graphics, string, length, font, layoutRect, stringFormat, brush ))

	DECLARE_GDIPLUS_METHOD(37, WINGDIPAPI, GdipStringFormatGetGenericTypographic, (GpStringFormat **format), ( format ))
	DECLARE_GDIPLUS_METHOD(38, WINGDIPAPI, GdipCloneStringFormat, (GDIPCONST GpStringFormat *format, GpStringFormat **newFormat), (format, newFormat))
	DECLARE_GDIPLUS_METHOD(39, WINGDIPAPI, GdipSetStringFormatFlags, (GpStringFormat *format, INT flags), (format, flags))

	DECLARE_GDIPLUS_METHOD(40, WINGDIPAPI, GdipSetPenDashArray, (GpPen *pen, GDIPCONST REAL *dash, INT count), (pen, dash, count))
	DECLARE_GDIPLUS_METHOD(41, WINGDIPAPI, GdipSetPenLineJoin, (GpPen *pen, GpLineJoin lineJoin), (pen, lineJoin))
	DECLARE_GDIPLUS_METHOD(42, WINGDIPAPI, GdipSetPenStartCap, (GpPen *pen, GpLineCap startCap), (pen, startCap))
	DECLARE_GDIPLUS_METHOD(43, WINGDIPAPI, GdipSetPenEndCap, (GpPen *pen, GpLineCap endCap), (pen, endCap))

	DECLARE_GDIPLUS_METHOD(44, WINGDIPAPI, GdipCreateRegion, (GpRegion **region), (region))
	DECLARE_GDIPLUS_METHOD(45, WINGDIPAPI, GdipGetClip, (GpGraphics *graphics, GpRegion *region), (graphics, region))
	DECLARE_GDIPLUS_METHOD(46, WINGDIPAPI, GdipSetClipRegion, (GpGraphics *graphics, GpRegion *region, CombineMode combineMode), (graphics, region, combineMode))
	DECLARE_GDIPLUS_METHOD(47, WINGDIPAPI, GdipDeleteRegion, (GpRegion *region), (region))
	DECLARE_GDIPLUS_METHOD(48, WINGDIPAPI, GdipSetClipRectI, (GpGraphics *graphics, INT x, INT y, INT width, INT height, CombineMode combineMode),  (graphics, x, y, width, height, combineMode))

	DECLARE_GDIPLUS_METHOD(49, WINGDIPAPI, GdipDrawImageRect, (GpGraphics *graphics, GpImage *image, REAL x, REAL y, REAL width, REAL height), (graphics, image, x, y, width, height))
	DECLARE_GDIPLUS_METHOD(50, WINGDIPAPI, GdipCreateBitmapFromHBITMAP, (HBITMAP hbm, HPALETTE hpal, GpBitmap** bitmap), (hbm, hpal, bitmap))
	DECLARE_GDIPLUS_METHOD(51, WINGDIPAPI, GdipCreateBitmapFromScan0, (INT width, INT height, INT stride, PixelFormat format, BYTE* scan0, GpBitmap** bitmap), (width, height, stride, format, scan0, bitmap))
	DECLARE_GDIPLUS_METHOD(52, WINGDIPAPI, GdipImageRotateFlip, (GpImage *image, RotateFlipType rfType), (image, rfType))
	DECLARE_GDIPLUS_METHOD(53, WINGDIPAPI, GdipDisposeImage, (GpImage *image), (image))

	DECLARE_GDIPLUS_METHOD(54, WINGDIPAPI, GdipBitmapLockBits, (GpBitmap* bitmap, GDIPCONST GpRect* rect, UINT flags,PixelFormat format, BitmapData* lockedBitmapData), (bitmap, rect, flags, format, lockedBitmapData))
	DECLARE_GDIPLUS_METHOD(55, WINGDIPAPI, GdipBitmapUnlockBits, (GpBitmap* bitmap, BitmapData* lockedBitmapData), (bitmap, lockedBitmapData))

public:
	HMODULE m_hModule;
	ULONG_PTR m_nGdiplusToken;
	int m_nCount;
	LPVOID m_ptrMethods[57];
};
