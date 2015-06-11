// XTPVisualStudio2012FrameTheme.h : interface for the CXTPVisualStudio2012FrameTheme class.
//
// This file is a part of the XTREME COMMANDBARS MFC class library.
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
#if !defined(__XTPVISUALSTUDIO2012FRAMETHEME_H__)
#define __XTPVISUALSTUDIO2012FRAMETHEME_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPPaintManager;
class CXTPCommandBarsFrameHook;


/////////////////////////////////////////////////////////////////////////////
// CXTPFrameThemeVisualStudio2012

class _XTP_EXT_CLASS CXTPFrameThemeVisualStudio2012 : public CXTPFrameThemeModernUI
{
protected:
	explicit CXTPFrameThemeVisualStudio2012(CXTPPaintManager *pPaintManager);

public:
	virtual ~CXTPFrameThemeVisualStudio2012();

	virtual void RefreshMetrics(CXTPResourceImages *pImages);
	virtual LPCTSTR GetStyleResourceName() const;
	virtual CSize GetCaptionButtonSize(UINT nId) const;
	virtual CRect CalcFrameBorder(const CXTPCommandBarsFrameHook *pFrameHook) const;
};


/////////////////////////////////////////////////////////////////////////////
// CXTPFrameThemeVisualStudio2012Light

class _XTP_EXT_CLASS CXTPFrameThemeVisualStudio2012Light : public CXTPFrameThemeVisualStudio2012
{
public:
	explicit CXTPFrameThemeVisualStudio2012Light(CXTPPaintManager *pPaintManager);
	virtual ~CXTPFrameThemeVisualStudio2012Light();
};


/////////////////////////////////////////////////////////////////////////////
// CXTPFrameThemeVisualStudio2012Dark

class _XTP_EXT_CLASS CXTPFrameThemeVisualStudio2012Dark : public CXTPFrameThemeVisualStudio2012
{
public:
	explicit CXTPFrameThemeVisualStudio2012Dark(CXTPPaintManager *pPaintManager);
	virtual ~CXTPFrameThemeVisualStudio2012Dark();
};


#endif // !defined(__XTPVISUALSTUDIO2012FRAMETHEME_H__)
