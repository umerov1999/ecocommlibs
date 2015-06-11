// XTPCaptionTheme.h: interface for the CXTPCaptionTheme class.
//
// This file is a part of the XTREME CONTROLS MFC class library.
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
#if !defined(__XTPCAPTIONTHEME_H__)
#define __XTPCAPTIONTHEME_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPCaption;
class CXTPButtonTheme;

//===========================================================================
// Summary:
//     CXTPCaptionTheme is a CXTPControlTheme derived class. It
//     is used to control the caption theme. It is used to control the
//     theme of captions. Derive custom themes from CXTPCaptionTheme.
//===========================================================================
class _XTP_EXT_CLASS CXTPCaptionTheme : public CXTPControlTheme
{
public:

	// -----------------------------------------------
	// Summary:
	//     Constructs a CXTPCaptionTheme object.
	// -----------------------------------------------
	CXTPCaptionTheme();

	// ----------------------------------------------------------------
	// Summary:
	//     This function is called to set the edges of a caption to the
	//     system default colors for edge shadow and edge light.
	// Remarks:
	//     pCaption - Points to the CXTPCaption object.
	// ----------------------------------------------------------------
	virtual void RefreshMetrics(CXTPCaption* pCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to draw the background of the caption at the
	//     location specified in the rcItem rect.
	// Parameters:
	//     pDC      - A CDC pointer that represents the current device
	//                context.
	//     pCaption - A pointer to a CXTPCaption object.
	//     rcItem   - Location where the caption is drawn.
	//-----------------------------------------------------------------------
	virtual void DrawCaptionBack(CDC* pDC, CXTPCaption* pCaption, CRect& rcItem);

	// ----------------------------------------------------------------
	// Summary:
	//     Called by CXTPCaption to draw the caption text.
	// Parameters:
	//     pDC      -  A CDC pointer that represents the current device
	//                 context.
	//     pCaption -  A pointer to a CXTPCaption object.
	// ----------------------------------------------------------------
	virtual void DrawCaptionText(CDC* pDC, CXTPCaption* pCaption);

	// ----------------------------------------------------------------------
	// Summary:
	//     Called by CXTPCaption to draw the caption icon.
	// Parameters:
	//     pDC      -  A CDC pointer that represents the current device
	//                 context.
	//     pCaption -  A pointer to a CXTPCaption object.
	//     rcItem   -  Location where the caption is drawn.
	// Remarks:
	//     Call this function to draw the icon on the caption. Only draws the
	//     icon if there is enough space between the caption borders.
	// ----------------------------------------------------------------------
	virtual void DrawCaptionIcon(CDC* pDC, CXTPCaption* pCaption, CRect& rcItem);

public:
	CXTPPaintManagerColor m_clrEdgeShadow; // The color of the edge shadow for the current theme.
	CXTPPaintManagerColor m_clrEdgeLight;  // The color of the edge light for the current theme.

};

//===========================================================================
// Summary:
//     CXTPCaptionThemeOfficeXP is a CXTPCaptionTheme derived class. It
//     is used to control the caption theme. It is used to control the
//     theme of captions. Derive custom themes from CXTPCaptionTheme.
//===========================================================================
class _XTP_EXT_CLASS CXTPCaptionThemeOfficeXP : public CXTPCaptionTheme
{
public:

	// -----------------------------------------------
	// Summary:
	//     Constructs a CXTPCaptionThemeOfficeXP object.
	// -----------------------------------------------
	CXTPCaptionThemeOfficeXP();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to draw the background of the caption.
	// Parameters:
	//     pDC      - A CDC pointer that represents the current device
	//                context.
	//     pCaption - A pointer to a CXTPCaption object.
	//     rcItem   - CRect object with the size and location where the caption is drawn.
	//-----------------------------------------------------------------------
	virtual void DrawCaptionBack(CDC* pDC, CXTPCaption* pCaption, CRect& rcItem);
};

//===========================================================================
// Summary:
//     CXTPCaptionThemeOffice2003 is derived from CXTPCaptionTheme.
//     It is used to set the caption to the Office 2003 theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPCaptionThemeOffice2003 : public CXTPCaptionTheme
{
public:

	// -----------------------------------------------
	// Summary:
	//     Constructs a CXTPCaptionThemeOffice2003 object.
	// -----------------------------------------------
	CXTPCaptionThemeOffice2003();

//{{AFX_CODEJOCK_PRIVATE
	CXTPCaptionThemeOffice2003(BOOL bPrimaryCaption); // deprecated.
//}}AFX_CODEJOCK_PRIVATE
protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to draw the background of the caption.
	// Parameters:
	//     pDC      - A CDC pointer that represents the current device
	//                context.
	//     pCaption - A pointer to a CXTPCaption object.
	//     rcItem   - CRect object with the size and location where the caption is drawn.
	//-----------------------------------------------------------------------
	virtual void DrawCaptionBack(CDC* pDC, CXTPCaption* pCaption, CRect& rcItem);
};

//===========================================================================
// Summary: CXTPCaptionButtonTheme is derived from CXTPButtonTheme.  This class
//          is used to set the caption for buttons.
//===========================================================================
class _XTP_EXT_CLASS CXTPCaptionButtonTheme : public CXTPButtonTheme
{
public:
	//-----------------------------------------------------------------------
	// Summary: Default class constructor.
	//-----------------------------------------------------------------------
	CXTPCaptionButtonTheme();

protected:
	//-----------------------------------------------------------------------
	// Summary: Call this member function to draw the button caption text.
	// Parameters:
	//       pDC     - A pointer to a valid device context.
	//       nState  - A UINT value that represents the current state of the button.
	//       rcItem  - a CRect object containing the dimensions of the button.
	//       pButton - A pointer to a valid CXTPButton object.
	//-----------------------------------------------------------------------
	virtual void DrawButtonText(CDC* pDC, CXTPButton* pButton);

	//-----------------------------------------------------------------------
	// Summary: Call this member function to get the text color of the button caption.
	// Parameters:
	//      pButton - A pointer to a valid CXTPButton object.
	// Returns:
	//      An RGB value specifying the text color.
	//-----------------------------------------------------------------------
	virtual COLORREF GetTextColor(CXTPButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to draw the background for the
	//     button.
	// Parameters:
	//     lpDIS   - A long pointer to a DRAWITEMSTRUCT structure. The
	//               structure contains information about the item to
	//               be drawn and the type of drawing required.
	//     pButton - Points to a CXTPButton object.
	// Returns:
	//     TRUE if the background was drawn successfully, otherwise returns
	//     FALSE.
	//-----------------------------------------------------------------------
	virtual void DrawButtonBackground(CDC* pDC, CXTPButton* pButton);
};

#endif // !defined(__XTPCAPTIONTHEME_H__)
