// XTPControlProgress.h : interface for the CXTPControlProgress class.
//
// This file is a part of the XTREME COMMANDBARS MFC class library.
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
#if !defined(__XTPCONTOLPROGRESS_H__)
#define __XTPCONTOLPROGRESS_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CXTPProgressPaintManager;
class CXTPWinThemeWrapper;

//===========================================================================
// Summary:
//     CXTPControlProgressBase is base class for all progress objects
//===========================================================================
class _XTP_EXT_CLASS CXTPProgressBase
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPProgressBase object
	//-----------------------------------------------------------------------
	CXTPProgressBase();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPProgressBase object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPProgressBase();

public:

	//----------------------------------------------------------------------
	// Summary:
	//     Sets the upper and lower limits of the progress bar control's range.
	// Parameters:
	//     nLower - Lower limit of the progress bar control's range.
	//     nUpper - Upper limit of the progress bar control's range.
	//----------------------------------------------------------------------
	void SetRange(int nLower, int nUpper);

	//----------------------------------------------------------------------
	// Summary:
	//     Gets the upper and lower limits of the progress bar control's range.
	// Parameters:
	//     nLower - Lower limit of the progress bar control's range.
	//     nUpper - Upper limit of the progress bar control's range.
	//----------------------------------------------------------------------
	void GetRange(int& nLower, int& nUpper) const;

	//----------------------------------------------------------------------
	// Summary:
	//     Gets the current position of the progress bar, whose return value
	//     is always between the values for the Max and Min properties, inclusive.
	// Returns:
	//     Current position of the progress bar.
	//----------------------------------------------------------------------
	int GetPos() const;

	//----------------------------------------------------------------------
	// Summary:
	//     Sets the current position of the progress bar, whose return value
	//     is always between the values for the Max and Min properties, inclusive.
	// Parameters:
	//     nPos - Position to move progress bar.
	//----------------------------------------------------------------------
	void SetPos(int nPos);

public:
	//----------------------------------------------------------------------
	// Summary:
	//     This method is called to get bounding rectangle of progressbar
	//----------------------------------------------------------------------
	virtual CRect GetProgressRect() = 0;

protected:
	//----------------------------------------------------------------------
	// Summary:
	//     This method is called to redraw progress bar
	//----------------------------------------------------------------------
	virtual void RedrawProgress() = 0;

	//----------------------------------------------------------------------
	// Summary:
	//     This method is called to retrieve pointer to CXTPProgressPaintManager
	//----------------------------------------------------------------------
	virtual CXTPProgressPaintManager* GetProgressPaintManager() const = 0;

protected:
	int m_nMin;     // Minimum
	int m_nMax;     // Maximum
	int m_nPos;     // Current position

};


//===========================================================================
// Summary:
//     CXTPControlProgress is a CXTPControl derived class.
//     It represents an progress control.
//===========================================================================
class _XTP_EXT_CLASS CXTPControlProgress : public CXTPControl, public CXTPProgressBase
{
	DECLARE_XTP_CONTROL(CXTPControlProgress)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPControlProgress object
	//-----------------------------------------------------------------------
	CXTPControlProgress();

protected:
	//----------------------------------------------------------------------
	// Summary:
	//     This method is called to determine the size of the control.
	// Parameters:
	//     pDC - Pointer to a valid device context
	// Returns:
	//     Size of the control.
	//----------------------------------------------------------------------
	virtual CSize GetSize(CDC* pDC);

	//----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the control.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//----------------------------------------------------------------------
	virtual void Draw(CDC* pDC);

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to redraw the progress bar.
	//-----------------------------------------------------------------------
	virtual void RedrawProgress();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the paint manager of the progress
	//     bar.
	// Returns:
	//     A pointer to a CXTPProgressPaintManager object
	//-----------------------------------------------------------------------
	virtual CXTPProgressPaintManager* GetProgressPaintManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the bounding rectangle of the control.
	// Returns:
	//     The bounding rectangle of the control.
	//-----------------------------------------------------------------------
	virtual CRect GetProgressRect();

	//----------------------------------------------------------------------
	// Summary:
	//     This method is called when the user clicks the control.
	// Parameters:
	//     bKeyboard - TRUE if the control is selected using the keyboard.
	//     pt        - Mouse cursor position.
	//----------------------------------------------------------------------
	virtual void OnClick(BOOL bKeyboard = FALSE, CPoint pt = CPoint(0, 0));


protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method makes a copy of the progress bar.
	// Parameters:
	//     pControl - Control needed to be copied.
	//     bRecursive - TRUE to copy recursively.
	//-----------------------------------------------------------------------
	void Copy(CXTPControl* pControl, BOOL bRecursive = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads or writes this object from or to an archive.
	// Parameters:
	//     pPX - A CXTPPropExchange object to serialize to or from.
	//----------------------------------------------------------------------
	void DoPropExchange(CXTPPropExchange* pPX);

};


//===========================================================================
// Summary: CXTPProgressPaintManager object
//===========================================================================
class _XTP_EXT_CLASS CXTPProgressPaintManager
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPProgressPaintManager object
	// Parameters:
	//     pPaintManager - PaintManager of commandbars
	//-----------------------------------------------------------------------
	CXTPProgressPaintManager(CXTPPaintManager* pPaintManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPProgressPaintManager object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPProgressPaintManager();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw progress
	// Parameters:
	//     pDC - Pointer to device context
	//     pProgressBar - Progress to draw
	//-----------------------------------------------------------------------
	virtual void DrawProgress(CDC* pDC, CXTPProgressBase* pProgressBar);

	//-------------------------------------------------------------------------
	// Summary:
	//     Recalculates metrics
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns parent paintmanager object
	//-----------------------------------------------------------------------
	CXTPPaintManager* GetPaintManager() const;

protected:
	CXTPWinThemeWrapper* m_themeProgress;        // Theme wrapper
	CXTPPaintManager* m_pPaintManager;          // Pointer to parent PaintManager

public:
	int m_cyProgress;           // Progress height
};

AFX_INLINE  int CXTPProgressBase::GetPos() const {
	return m_nPos;
}
AFX_INLINE CXTPPaintManager* CXTPProgressPaintManager::GetPaintManager() const {
	return m_pPaintManager;
}


#endif //#if !defined(__XTPCONTOLPROGRESS_H__)
