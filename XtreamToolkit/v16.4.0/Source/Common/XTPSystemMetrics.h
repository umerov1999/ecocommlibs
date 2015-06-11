//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPSYSTEMMETRICS_H__)
#define __XTPSYSTEMMETRICS_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class _XTP_EXT_CLASS CXTPSystemMetrics
{
public:
	CXTPSystemMetrics();

	void RefreshMetrics();

public:

	int m_nBorderY;    // The height of a window border, in pixels.
	int m_nCaptionY;   // The height of a caption area, in pixels.
	int m_nSizeFrameY; // The thickness of the sizing border around the perimeter of a window that can be resized, in pixels.

	// Frame icon
	int m_nSmallIconX; // SM_CYSMICON
	int m_nSmallIconY; // SM_CXSMICON

};


CXTPSystemMetrics *XTPSystemMetrics();


class _XTP_EXT_CLASS CXTPDeviceCaps
{
public:
	CXTPDeviceCaps();

	void RefreshMetrics();

public:
	// Log pixels
	int m_nLogPixelsX; // LOGPIXELSX
	int m_nLogPixelsY; // LOGPIXELSY
};

CXTPDeviceCaps *XTPDeviceCaps();


#endif // !defined(__XTPSYSTEMMETRICS_H__)
