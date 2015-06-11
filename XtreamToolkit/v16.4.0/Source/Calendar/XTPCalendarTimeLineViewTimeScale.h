// XTPCalendarTimeLineViewTimeScale.h: interface for the
// CXTPCalendarTimeLineViewTimeScale class.
//
// This file is a part of the XTREME CALENDAR MFC class library.
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
#ifndef _XTPCALENDARTIMELINEVIEWTIMESCALE_H__
#define _XTPCALENDARTIMELINEVIEWTIMESCALE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


struct XTPCALENDARHITTESTINFO_TIMELINE_VIEW;
class CXTPCalendarTimeLineView;
class CXTPCalendarTLV_TimeScaleParams;
class CXTPCalendarTimeLineViewTimeScalePart;

//============================================================================
// Summary:
//     predefine set of different scales based on time intervals
//============================================================================
enum XTPEnumCalendarDateItem
{
	xtpDTScale_Unknown  = 0, // Typed unknown value

	xtpDTScale_Sec      = 0x0001, // Sec scale
	xtpDTScale_Min      = 0x0002, // Min scale
	xtpDTScale_Hour     = 0x0004, // Hour scale
	xtpDTScale_Day      = 0x0010, // Day scale
	xtpDTScale_WorkWeek = 0x0015, // WorkWeek scale
	xtpDTScale_Week     = 0x0020, // Week scale
	xtpDTScale_Month    = 0x0040, // Month scale
	xtpDTScale_Year     = 0x0080, // Year scale
};

//============================================================================
// Summary:
//     Timeline view scale is
//============================================================================
struct XTP_CALENDAR_TIMELINE_VIEW_SCALE
{
	int m_eType;    // one of the values from enum XTPEnumCalendarDateItem
	int m_nValue;   // number of days to show (e.g. 1, 5, 7, 31)
};

//============================================================================
// Summary:
//     This class implements a specific view portion of the Calendar
//     control - called Timeline view scale.
//============================================================================
class _XTP_EXT_CLASS CXTPCalendarTimeLineViewTimeScale : public CXTPCmdTarget
{
public:
	// --------------------------
	// Summary:
	//     Default object constructor.
	// Parameters:
	//     pView :  pointer to associated CXTPCalendarTimeLineView object.
	// --------------------------
	CXTPCalendarTimeLineViewTimeScale(CXTPCalendarTimeLineView* pView);

	//-----------------------------------------------------------------------
	// Summary:
	//     Default class destructor.
	// Remarks:
	//     Handles member items deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPCalendarTimeLineViewTimeScale();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the owner view object.
	// Returns:
	//     CXTPCalendarTimeLineView pointer to the owner view object.
	//-----------------------------------------------------------------------
	CXTPCalendarTimeLineView* GetView() const;

	// -----------------
	// Summary:
	//  access function to get time scale params
	// Returns:
	//  pointer to CXTPCalendarTLV_TimeScaleParams object
	// -----------------
	virtual CXTPCalendarTLV_TimeScaleParams* GetTimeScaleParams() const;

	// ---------------------------------------
	// Summary:
	//  access function to set time scale params
	//  from predefined set XTPEnumCalendarTimeScaleParamsID
	// Parameters:
	//  eTimeScaleParamsID :  int as one of XTPEnumCalendarTimeScaleParamsID set
	// ---------------------------------------
	virtual void SetTimeScaleParams(int eTimeScaleParamsID);

	// ---------------------------------------
	// Summary:
	//  access function to set time scale params
	//  from another CXTPCalendarTLV_TimeScaleParams object
	// Parameters:
	// pSParams :  pointer to CXTPCalendarTLV_TimeScaleParams object
	// -----------------------------
	virtual void SetTimeScaleParams2(CXTPCalendarTLV_TimeScaleParams* pSParams);

	// -----------------
	// Summary:
	//  access function to get start view date value
	// Returns:
	//  start view date value as COleDateTime object
	// -----------------
	virtual COleDateTime GetStartViewDate() const;

	// -----------------------
	// Summary:
	//  access function to set start view date value
	// Parameters:
	// dt :  new start view date value
	// -----------------------
	virtual void SetStartViewDate(COleDateTime dt);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to adjust time scale layout
	//     depending on the provided parent rectangle
	// Parameters:
	//     rcParentRect :  provided parent rectangle
	// ---------------------------------
	virtual void AdjustLayout(CRect rcParentRect);

	// ----------------------------
	// Summary:
	//  access function to get client rectangle according
	//  to passed vertical start position
	// Parameters:
	//     nYstart :  vertical start position
	// Returns:
	//     client rectangle as CRect object
	// ----------------------------
	virtual CRect GetClientRect(int nYstart = 0);

	// -----------------
	// Summary:
	//  access function to get time scale rectangle
	// Returns:
	// time scale rectangle as CRect object
	// -----------------
	virtual CRect GetRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to draw the time scale
	//     the specified device context.
	// Parameters:
	//  pDC - A pointer to a valid device context.
	//-----------------------------------------------------------------------
	virtual void Draw(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to determine which view item,
	//     if any, is at a specified position index, and returns additional
	//     info in a XTP_CALENDAR_HITTESTINFO struct.
	// Parameters:
	//     pt       - A CPoint that contains the coordinates of the point to
	//                test.
	//     pHitTest - A pointer to a XTP_CALENDAR_HITTESTINFO structure that
	//                contains information about the point to test.
	// Returns:
	//     A BOOL. TRUE if the item is found. FALSE otherwise.
	// See Also: XTP_CALENDAR_HITTESTINFO
	//-----------------------------------------------------------------------
	virtual BOOL HitTestEx(CPoint pt, XTP_CALENDAR_HITTESTINFO* pHitTest) const;

	CRect m_rcScale; // internal member to keep time scale rectangle

protected:
	CXTPCalendarTimeLineView* m_pParentView; //internal pointer to parent view
	COleDateTime m_dtStartViewDate; // internal datetime object to keep start view date
	CRect m_rcRect;  // internal member used for layout adjustment
	CXTPCalendarTLV_TimeScaleParams* m_pTimeScaleParams; // internal pointer to CXTPCalendarTLV_TimeScaleParams
};

//============================================================================
// Summary:
//     This class implements a Timeline scale params.
//============================================================================
class _XTP_EXT_CLASS CXTPCalendarTLV_TimeScaleParams : public CXTPCmdTarget
{
	friend class CXTPCalendarTimeLineViewTimeScale;
public:
	// -----------------
	// Summary:
	//     Default object constructor.
	// Parameters:
	//     nTimeScaleParamsID - passed new params ID
	//     pTimeScale - pointer to CXTPCalendarTimeLineViewTimeScale
	// -----------------
	CXTPCalendarTLV_TimeScaleParams(int nTimeScaleParamsID,
									CXTPCalendarTimeLineViewTimeScale* pTimeScale);

	//-----------------------------------------------------------------------
	// Summary:
	//     Default class destructor.
	// Remarks:
	//     Handles member items deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPCalendarTLV_TimeScaleParams();

	// -----------------
	// Summary:
	//     access function to get current parent time scale object
	// Returns:
	//     pointer to CXTPCalendarTimeLineViewTimeScale
	// -----------------
	CXTPCalendarTimeLineViewTimeScale* GetParentTimeScale() const;

	// -----------------
	// Summary:
	//     access function to get current time scale params ID
	// Returns:
	//     int as current time scale params ID
	// -----------------
	virtual int GetTimeScaleParamsID() const;

	// -----------------
	// Summary:
	//     access function to get current time scale inteval
	// Returns:
	//     interval as XTP_CALENDAR_TIMELINE_VIEW_SCALE object
	// -----------------
	virtual XTP_CALENDAR_TIMELINE_VIEW_SCALE GetTimeLineViewScale() const;

	// -----------------
	// Summary:
	//     access function to get current time scale inteval
	// Returns:
	//     interval as COleDateTimeSpan object
	// -----------------
	virtual COleDateTimeSpan GetScaleInterval() const;

	// -----------------
	// Summary:
	//     access function to get current time scale inteval width
	// Returns:
	//     int as current time scale inteval width
	// -----------------
	virtual int GetScaleIntervalWidth_px() const;

	// -----------------
	// Summary:
	//     access function to get current time per pixel value
	// Returns:
	//     current time per pixel value  as COleDateTimeSpan object
	// -----------------
	virtual COleDateTimeSpan GetTimePerPixel() const;

	// -----------------
	//  Summary:
	// use to calculate scale inteval initial position
	//  Parameters:
	// dtStart - start time as COleDateTime object
	//  Returns:
	// scale inteval initial position as COleDateTime object
	// -----------------
	virtual COleDateTime CalcScaleIntervalBegin(COleDateTime dtStart);

	// -----------------
	// Summary:
	//     abstract function - real ones are:
	//     CXTPCalendarTimeLineViewTimeScalePart* CXTPCalendarTLV_TimeScaleParams_Day::GetPart()
	//     CXTPCalendarTimeLineViewTimeScalePart* CXTPCalendarTLV_TimeScaleParams_Week::GetPart()
	//     CXTPCalendarTimeLineViewTimeScalePart* CXTPCalendarTLV_TimeScaleParams_Month::GetPart()
	// Returns:
	//     pointer to CXTPCalendarTimeLineViewTimeScalePart object
	// -----------------
	virtual CXTPCalendarTimeLineViewTimeScalePart* GetPart() const = 0;

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to set the time scale interval m_scaleInterval.
	// Parameters:
	//     eType - passed to m_eType
	//     nValue - passed to m_nValue
	// -----------------
	void SetScaleInterval(int eType, int nValue);

	XTP_CALENDAR_TIMELINE_VIEW_SCALE m_scaleInterval; // internal instance with m_eType and m_nValue

	int m_nScaleIntervalWidth_px; // internal value for scale inteval (by x)

private:
	CXTPCalendarTimeLineViewTimeScale* m_pTimeScale; // pointer to time scale object
	int m_nTimeScaleParamsID; // internal value of current time scale param id
};

//============================================================================
// Summary:
//     This class implements a Timeline scale params for Day case.
//============================================================================
class _XTP_EXT_CLASS CXTPCalendarTLV_TimeScaleParams_Day :
	public CXTPCalendarTLV_TimeScaleParams
{
public:
	// -----------------
	// Summary:
	//     Default object constructor.
	// Parameters:
	//     pTimeScale - pointer to CXTPCalendarTimeLineViewTimeScale
	// -----------------
	CXTPCalendarTLV_TimeScaleParams_Day(CXTPCalendarTimeLineViewTimeScale* pTimeScale);

	//-----------------------------------------------------------------------
	// Summary:
	//     Default class destructor.
	// Remarks:
	//     Handles member items deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPCalendarTLV_TimeScaleParams_Day();

	// -----------------
	// Summary:
	//     abstract function - real ones are:
	//     CXTPCalendarTimeLineViewTimeScalePart* CXTPCalendarTLV_TimeScaleParams_Day::GetPart()
	//     CXTPCalendarTimeLineViewTimeScalePart* CXTPCalendarTLV_TimeScaleParams_Week::GetPart()
	//     CXTPCalendarTimeLineViewTimeScalePart* CXTPCalendarTLV_TimeScaleParams_Month::GetPart()
	// Returns:
	//     pointer to CXTPCalendarTimeLineViewTimeScalePart object
	// -----------------
	virtual CXTPCalendarTimeLineViewTimeScalePart* GetPart() const;
};

//============================================================================
// Summary:
//     This class implements a Timeline scale params for Week case.
//============================================================================
class _XTP_EXT_CLASS CXTPCalendarTLV_TimeScaleParams_Week :
	public CXTPCalendarTLV_TimeScaleParams
{
public:
	// -----------------
	// Summary:
	//     Default object constructor.
	// Parameters:
	//     pTimeScale - pointer to CXTPCalendarTimeLineViewTimeScale
	// -----------------
	CXTPCalendarTLV_TimeScaleParams_Week(CXTPCalendarTimeLineViewTimeScale* pTimeScale);

	//-----------------------------------------------------------------------
	// Summary:
	//     Default class destructor.
	// Remarks:
	//     Handles member items deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPCalendarTLV_TimeScaleParams_Week();

	// -----------------
	// Summary:
	//     abstract function - real ones are:
	//     CXTPCalendarTimeLineViewTimeScalePart* CXTPCalendarTLV_TimeScaleParams_Day::GetPart()
	//     CXTPCalendarTimeLineViewTimeScalePart* CXTPCalendarTLV_TimeScaleParams_Week::GetPart()
	//     CXTPCalendarTimeLineViewTimeScalePart* CXTPCalendarTLV_TimeScaleParams_Month::GetPart()
	// Returns:
	//     pointer to CXTPCalendarTimeLineViewTimeScalePart object
	// -----------------
	virtual CXTPCalendarTimeLineViewTimeScalePart* GetPart() const;
};

//============================================================================
// Summary:
//     This class implements a Timeline scale params for Month case.
//============================================================================
class _XTP_EXT_CLASS CXTPCalendarTLV_TimeScaleParams_Month :
	public CXTPCalendarTLV_TimeScaleParams
{
public:
	// -----------------
	// Summary:
	//     Default object constructor.
	// Parameters:
	//     pTimeScale - pointer to CXTPCalendarTimeLineViewTimeScale
	// -----------------
	CXTPCalendarTLV_TimeScaleParams_Month(CXTPCalendarTimeLineViewTimeScale* pTimeScale);

	//-----------------------------------------------------------------------
	// Summary:
	//     Default class destructor.
	// Remarks:
	//     Handles member items deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPCalendarTLV_TimeScaleParams_Month();

	// -----------------
	// Summary:
	//     abstract function - real ones are:
	//     CXTPCalendarTimeLineViewTimeScalePart* CXTPCalendarTLV_TimeScaleParams_Day::GetPart()
	//     CXTPCalendarTimeLineViewTimeScalePart* CXTPCalendarTLV_TimeScaleParams_Week::GetPart()
	//     CXTPCalendarTimeLineViewTimeScalePart* CXTPCalendarTLV_TimeScaleParams_Month::GetPart()
	// Returns:
	//     pointer to CXTPCalendarTimeLineViewTimeScalePart object
	// -----------------
	virtual CXTPCalendarTimeLineViewTimeScalePart* GetPart() const;

	//virtual CString FormatLowerScaleLabel(COleDateTime dtDateTime);
	//virtual void DrawLowerScale(CDC* pDC, DATE dtStart, const CRect& rcRect);
};

AFX_INLINE CXTPCalendarTimeLineView* CXTPCalendarTimeLineViewTimeScale::GetView() const {
	return m_pParentView;
}
AFX_INLINE CXTPCalendarTLV_TimeScaleParams* CXTPCalendarTimeLineViewTimeScale::GetTimeScaleParams() const {
	return m_pTimeScaleParams;
}
AFX_INLINE COleDateTime CXTPCalendarTimeLineViewTimeScale::GetStartViewDate() const {
	return m_dtStartViewDate;
}
AFX_INLINE void CXTPCalendarTimeLineViewTimeScale::SetStartViewDate(COleDateTime dt) {
	m_dtStartViewDate = dt;
}
AFX_INLINE CRect CXTPCalendarTimeLineViewTimeScale::GetRect() const {
	return m_rcRect;
}
AFX_INLINE CXTPCalendarTimeLineViewTimeScale* CXTPCalendarTLV_TimeScaleParams::GetParentTimeScale() const {
	return m_pTimeScale;
}
AFX_INLINE XTP_CALENDAR_TIMELINE_VIEW_SCALE CXTPCalendarTLV_TimeScaleParams::GetTimeLineViewScale() const {
	return m_scaleInterval;
}
AFX_INLINE int CXTPCalendarTLV_TimeScaleParams::GetScaleIntervalWidth_px() const {
	return m_nScaleIntervalWidth_px;
}
AFX_INLINE COleDateTimeSpan CXTPCalendarTLV_TimeScaleParams::GetTimePerPixel() const {
	ASSERT(m_nScaleIntervalWidth_px > 0);
	COleDateTimeSpan spSInterval = GetScaleInterval();
	COleDateTimeSpan spTimePerPixel = (DATE)((double)GetScaleInterval() / max(m_nScaleIntervalWidth_px, 1));
	return spTimePerPixel;
}
AFX_INLINE int CXTPCalendarTLV_TimeScaleParams::GetTimeScaleParamsID() const {
	return m_nTimeScaleParamsID;
}

//============================================================================
#endif // (_XTPCALENDARTIMELINEVIEWTIMESCALE_H__)
