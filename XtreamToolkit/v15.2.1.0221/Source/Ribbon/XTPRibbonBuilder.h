// XTPRibbonBuilder.h: interface for the CXTPRibbonBuilder class.
//
// This file is a part of the XTREME RIBBON MFC class library.
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
#if !defined(__XTPRIBBONBUILDER_H__)
#define __XTPRIBBONBUILDER_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef RT_RIBBON
#define RT_RIBBON MAKEINTRESOURCE(28)
#endif

class CXTPRibbonBar;
class CXTPPropExchange;
class CXTPCommandBars;
class CXTPPropExchangeXMLNode;
class CXTPRibbonGroup;
class CXTPRibbonGroup;
class CXTPRibbonTab;
class CXTPControl;

class _XTP_EXT_CLASS CXTPRibbonBuilder
{
public:
	CXTPRibbonBuilder();
	virtual ~CXTPRibbonBuilder();

public:
	BOOL LoadFromResource(UINT uiXMLResID, LPCTSTR lpszResType = RT_RIBBON, HINSTANCE hInstance = NULL);

	BOOL Build(CXTPRibbonBar* pRibbonBar);

protected:
	virtual CCmdTarget* CreateElement(const CString& strElementName);

protected:
	BOOL BuildCategories(CXTPPropExchange* pPX, CXTPRibbonBar* pRibbonBar);
	BOOL BuildGroups(CXTPPropExchange* pPX, CXTPRibbonTab* pRibbonTab);
	BOOL BuildGroupControls(CXTPPropExchange* pPX, CXTPRibbonGroup* pRibbonGroup);
	void BuildControl(CXTPPropExchange* pPX, CXTPControl* pControl);
	BOOL BuildMainButton(CXTPPropExchange* pPX, CXTPRibbonBar* pRibbonBar);
	BOOL BuildMainButtonPopupBar(CXTPPropExchange* pPX, CXTPRibbonBar* pRibbonBar);
	void BuildControlPopupBar(CXTPPropExchange* pPX, CXTPControl*& pControl, CRuntimeClass* pPopupBarClass);
	void BuildQATElements(CXTPPropExchange* pPX, CXTPRibbonBar* pRibbonBar);
	BOOL BuildTabElements(CXTPPropExchange* pPX, CXTPRibbonBar* pRibbonBar);

	void AddImageIndex(int nIndex, int nId, int nImageType);
	void LoadIcons(CXTPPropExchange* pPX);
	void LoadIcons(int nId, CUIntArray& arrIcons);

protected:
	CUIntArray m_arrImageSmall;
	CUIntArray m_arrImageLarge;
	CXTPCommandBars* m_pCommandBars;
	CXTPPropExchangeXMLNode* m_pPX;
};

#endif // !defined(__XTPRIBBONBUILDER_H__)
