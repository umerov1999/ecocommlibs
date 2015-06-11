// XTPTaskPanelVisualStudio2012Theme.h : implementation of the CXTPTaskPanelVisualStudio2012Theme class.
//
// This file is a part of the XTREME TASKPANEL MFC class library.
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
#ifndef XTPTaskPanelVisualStudio2012Theme_h__
#define XTPTaskPanelVisualStudio2012Theme_h__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Summary:
//     The CXTPTaskPanelVisualStudio2012Theme class is used to enable a VS.NET 2012 style theme.
// See Also:
//     CXTPTaskPanel::SetTheme
//===========================================================================
class _XTP_EXT_CLASS CXTPTaskPanelVisualStudio2012Theme : public CXTPTaskPanelToolboxWhidbeyTheme
{
protected:

	void Triangle(CDC* pDC, POINT *pPoints, COLORREF clr, BOOL bFilled);

public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTaskPanelVisualStudio2012Theme object.
	//-------------------------------------------------------------------------
	CXTPTaskPanelVisualStudio2012Theme(BOOL bLight);
	~CXTPTaskPanelVisualStudio2012Theme();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to draw the background of the group caption.
	// Parameters:
	//     pDC    - Pointer to a valid device context.
	//     pGroup - Fill group caption of this group.
	//     rc     - Bounding rectangle of the group caption.
	// Returns:
	//     Bounding rectangle of text within group caption.
	// Remarks:
	//     This member will draw the Whidbey color gradient in the group
	//     caption rectangle.
	//-----------------------------------------------------------------------
	CRect DrawGroupCaptionBackground(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the face of the group.
	// Parameters:
	//     pDC    - Pointer to a valid device context
	//     pGroup - Points to a CXTPTaskPanelGroup object.
	//     rc     - Rectangle of group to be draw.
	//-----------------------------------------------------------------------
	void DrawGroupClientFace(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member draws a rectangle around a group item when selected,
	//     pressed or highlighted.
	// Parameters:
	//     pDC - Pointer to a valid device context
	//     pItem - Points to a CXTPTaskPanelGroupItem object.
	//     rc    - Bounding rectangle group to draw.
	//-----------------------------------------------------------------------
	virtual void DrawGroupItemFrame(CDC* pDC, CXTPTaskPanelGroupItem* pItem, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Fills group rectangle
	// Parameters:
	//     pDC - Pointer to a valid device context
	//     pGroup - Points to a CXTPTaskPanelGroup object
	//     rc - Rectangle of group to be draw.
	// See Also: DrawGroupClientFace
	//-----------------------------------------------------------------------
	void FillGroupClientFace(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to fill background of task panel control.
	// Parameters:
	//     pDC - Pointer to a valid device context
	//     pTaskPanel - Points to a CXTPTaskPanel object
	//-----------------------------------------------------------------------
	void FillTaskPanel(CDC* pDC, CXTPTaskPanel* pTaskPanel);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw text caption of the task panel item.
	// Parameters:
	//     pDC     - Pointer to a valid device context
	//     pItem   - Points to a CXTPTaskPanelItem object.
	//     rcText  - Rectangle of item to draw.
	//     nFormat - [in] Specifies the method of formatting the text.
	//     DrawGroupCaption uses this method to draw the group caption with
	//     nFormat = DT_SINGLELINE | DT_LEFT | DT_END_ELLIPSIS | DT_VCENTER | DT_NOPREFIX
	//
	//     DrawGroupItemImageWithText and DrawGroupItemImageWithTextBelow use
	//     DrawItemCaption to draw the group item caption.
	//-----------------------------------------------------------------------
	virtual void DrawItemCaption(CDC* pDC, CXTPTaskPanelItem* pItem, CRect rcText, UINT nFormat);

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of task panel.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();

protected:

	virtual CXTPPaintManagerColor GetTriangleColorAndState(CXTPTaskPanelGroup* pGroup, BOOL &bFilled);

	virtual CRect DrawExpandButton(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc);

protected:
	CXTPPaintManagerColor m_clrSelectedWithoutFocus; //color of selected item when TaskPanel lost focus

	CXTPPaintManagerColor m_clrText;//normal color of text
	CXTPPaintManagerColor m_clrSelectedText;//color of text when item is selected
	CXTPPaintManagerColor m_clrSelectedTextWithoutFocus; //color of text when item is selected and TaskPanel lost focus

	CXTPPaintManagerColor m_clrTriangle;
	CXTPPaintManagerColor m_clrTriangleFocused;
	CXTPPaintManagerColor m_clrTriangleHot;

	BOOL m_bLight;
};

#endif // XTPTaskPanelVisualStudio2012Theme_h__