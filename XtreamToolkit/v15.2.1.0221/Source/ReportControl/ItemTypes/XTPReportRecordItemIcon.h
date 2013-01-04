// XTPReportRecordItemIcon.h: interface for the CXTPReportRecordItemIcon class.
//
// This file is a part of the XTREME REPORTCONTROL MFC class library.
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
#if !defined(__XTPREPORTRECORDITEMICON_H__)
#define __XTPREPORTRECORDITEMICON_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class _XTP_EXT_CLASS CXTPReportRecordItemIcon: public CXTPReportRecordItem
{
	DECLARE_SERIAL(CXTPReportRecordItemIcon)
public:
	CXTPReportRecordItemIcon();

	//-----------------------------------------------------------------------
	// Summary:
	//     Should be overridden by descendants for drawing itself.
	// Parameters:
	//     pDrawArgs - structure which contains drawing arguments:
	// Returns:
	//     An integer specifying the item text width.
	//
	//-----------------------------------------------------------------------
	virtual int Draw(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs);

	//-----------------------------------------------------------------------
	// Summary:
	//     Should be overridden by descendants for drawing itself.
	// Parameters:
	//     pDrawArgs - structure which contain drawing arguments.
	//     pMetrics - structure which contain metrics of the item.
	//-----------------------------------------------------------------------
	virtual void OnDrawCaption(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, XTP_REPORTRECORDITEM_METRICS* pMetrics);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves caption text bounding rectangle
	// Parameters:
	//     pDrawArgs - structure which contain drawing arguments.
	//     rcItem    - Bounding rectangle of the item
	//-----------------------------------------------------------------------
	virtual void GetCaptionRect(XTP_REPORTRECORDITEM_ARGS* pDrawArgs, CRect& rcItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when value of in-place edit control changed
	// Parameters:
	//     pItemArgs - Pointer to structure with items arguments.
	//     szText    - New in-place edit text.
	//-----------------------------------------------------------------------
	void OnEditChanged(XTP_REPORTRECORDITEM_ARGS* pItemArgs, LPCTSTR szText);
};

#endif // !defined(__XTPREPORTRECORDITEMICON_H__)
