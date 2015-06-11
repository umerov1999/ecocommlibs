// XTPAnimationDC.h : header file
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
#if !defined(__XTPANIMATIONDC_H__)
#define __XTPANIMATIONDC_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ---------------------------------------------------------------
// Summary:
//     Enumeration used to determine animation style.
// Remarks:
//     XTPAnimationDCType type defines constants used by the
//     CXTPAnimationDC class to determine the animation
//     effect for the current device context.
// See Also:
//     CXTPAnimationDC, CXTPAnimationDC::Animate, CXTPAnimationDC::SetCustomAnimation
//
// <KEYWORDS xtpAniDefault, xtpAniRandom, xtpAniUnfold, xtpAniSlide, xtpAniFade, xtpAniNone>
// -------------------------------------------------------------------------------
enum XTPAnimationDCType
{
	xtpAniDefault,  // Uses Windows default animation settings.
	xtpAniRandom,   // Animates a window with a random animation type.
	xtpAniUnfold,   // Animates a window to unfold top to bottom.
	xtpAniSlide,    // Animates a window to slide in from left.
	xtpAniFade,     // Animates a window with a fade-in effect.
	xtpAniNone      // No animation.
};

// -------------------------------------------------------------------------------------
// Summary:
//     CXTPAnimationDC is a CXTPBufferDC derived class. This class
//     can be used to render animation effects for the current device
//     context.
// Remarks:
//     By calling Animate the device context is drawn on the
//     screen with a defined animation type. See XTPAnimationDCType for
//     a list of available animation styles.<p/>
//     Animations are similar to the menu animation effect as seen in
//     Microsoft(r) Windows(r) or Microsoft(r) Office products.
// Example:
//     See Animate for an example that demonstrates the use of CXTPAnimationDC.
// See Also:
//     LPFNANIMATION, XTPAnimationDCType, Animate, SetCustomAnimation
// -------------------------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPAnimationDC : public CDC
{
public:
	// -------------------------------------------------------------------
	// Summary:
	//     Function pointer used to define a custom animation effect.
	// Parameters:
	//     rc -              Area to display.
	//     pDestDC -         Destination device context.
	//     pSrcDC -          Source device context.
	//     nType -           Enumerated XTPAnimationDCType to determine
	//                       animation effect.
	//     nSteps -          Number of steps to complete animation.
	//     nAnimationTime -  Amount of time in milliseconds between each step.
	// Example:
	//     See CXTPAnimationDC::SetCustomAnimation for an example that
	//     demonstrates the use of LPFNANIMATION.
	// See Also:
	//     CXTPAnimationDC::SetCustomAnimation, XTPAnimationDCType
	// -------------------------------------------------------------------
	typedef void (AFX_CDECL* LPFNANIMATION)(CRect rc, CDC* pDestDC, CDC* pSrcDC, int nType, int nSteps, int nAnimationTime);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPBufferDC object used to create a memory
	//     device context used to draw to an off-screen bitmap.
	// Parameters:
	//     hDestDC - Handle to the destination device context the memory
	//               device is BitBlt to.
	//     rcPaint - Size of the area to paint.
	//     clrBack - Represents background color for fill, can be COLORREF or gradient values using CXTPPaintManagerColorGradient.
	//     bHorz   - Used when drawing gradient background, TRUE to draw gradient from left to right, otherwise drawn top to bottom.
	//-----------------------------------------------------------------------
	CXTPAnimationDC(HDC hDestDC, const CRect& rcPaint);
	CXTPAnimationDC(HDC hDestDC, const CRect& rcPaint, const CXTPPaintManagerColorGradient& clrBack, const BOOL bHorz = FALSE);

	// -------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPAnimationDC object, handles cleanup and
	//     deallocation.
	// -------------------------------------------------------------------
	virtual ~CXTPAnimationDC();

	// ----------------------------------------------------------------------------
	// Summary:
	//     This member function performs the animation
	// Parameters:
	//     nType -           [in] Type of animation to perform.
	//     nSteps -          [in] Number of steps to take during animation.
	//     nAnimationTime -  [in] Amount of time to rest, in milliseconds, between
	//                       each step.
	// Remarks:
	//     Call this member function from your WM_PAINT handler to render the
	//     animation background.
	// Example:
	//     The following example demonstrates the use of Animate.
	// <code>
	// void CMainFrame::DoAnimationTest(CDC* pDC, XTPAnimationDCType anim)
	// {
	//     CRect r(100, 100, 300, 200);
	//     CXTPBufferDC background(pDC, r, (COLORREF)-1);
	//     background.FromDC();
	//     {
	//         CXTPAnimationDC memDC(pDC, r, NULL);
	//
	//         r -= r.TopLeft();
	//
	//         memDC.Rectangle(r);
	//         memDC.MoveTo(0, 0);
	//         memDC.LineTo(r.BottomRight());
	//         memDC.SelectStockObject(GRAY_BRUSH);
	//         memDC.FloodFill(r.left + 10, r.top + 10, RGB(0, 0, 0));
	//         memDC.Animate(anim, 10, 1000);
	//     }
	//
	//     \::Sleep(2000);
	// }
	// </code>
	// See Also:
	//     LPFNANIMATION, XTPAnimationDCType, SetCustomAnimation
	// ----------------------------------------------------------------------------
	void Animate(int nType = xtpAniDefault, int nSteps = 10, int nAnimationTime = 1000);

	// ------------------------------------------------------------------------------
	// Summary:
	//     This member implements default animation effects <i>Fade</i>, <i>Slide</i>
	//     and <i>Unfold</i>.
	// Parameters:
	//     rc -              [in] Bounding rectangle.
	//     pDestDC -         [in] Pointer to device context you must draw to.
	//     pSrcDC -          [in] Device context that contains the bitmap you must
	//                       take.
	//     nType -           [in] Type of animation to perform. For custom animation
	//                       you must use numbers greater than <i>xtpAniNone</i>.
	//     nSteps -          [in] Number of steps to take during animation.
	//     nAnimationTime -  [in] Amount of time to rest, in milliseconds, between
	//                       each step.
	// Remarks:
	//     You can add new animation effects to call SetCustomAnimation
	//     member.
	// Example:
	//     See SetCustomAnimation for an example that demonstrates the use of
	//     DefaultAnimation.
	// See Also:
	//     LPFNANIMATION, XTPAnimationDCType, Animate, SetCustomAnimation
	// ------------------------------------------------------------------------------
	static void AFX_CDECL DefaultAnimation(CRect rc, CDC* pDestDC, CDC* pSrcDC, int nType, int nSteps, int nAnimationTime);

	// -----------------------------------------------------------------------
	// Summary:
	//     Call this member function to setup new Animation effects.
	// Parameters:
	//     pCustom -  [in] Function pointer that holds the address of a custom
	//                animation function.
	// Remarks:
	//     You can call DefaultAnimation in your custom animation method to
	//     handle default animation effects.
	// Example:
	//     The following example demonstrates the use of SetCustomAnimation.
	// <code>
	// int CMainFrame::OnCreate()
	// {
	//     // custom animation procedure.
	//     CXTPAnimationDC::SetCustomAnimation(CustomAnimation);
	//     ...
	// }
	// ...
	// void CMainFrame::CustomAnimation(CRect rc, CDC* pDestDC,
	//     CDC* pSrcDC, int nType, int nSteps, int nAnimationTime)
	// {
	//     if (nType == xtpAniUnfold)
	//     {
	//         // do custom animation
	//     }
	//     else
	//     {
	//         CXTPAnimationDC::DefaultAnimation(rc, pDestDC,
	//             pSrcDC, nType, nSteps, nAnimationTime);
	//     }
	// }
	//     </code>
	// See Also:
	//     LPFNANIMATION, XTPAnimationDCType, Animate, DefaultAnimation
	// -----------------------------------------------------------------------
	static void SetCustomAnimation(LPFNANIMATION pCustom);

private:

	// -------------------------------------------------------------------
	// Summary:
	//     Private LPFNANIMATION pointer that points to the address of a
	//     user defined animation method.
	// -------------------------------------------------------------------
	static LPFNANIMATION m_pCustomAnimation;

	// ---------------------------------------------------------
	// Summary:
	//     Private method is used internally for alpha-blending.
	// ---------------------------------------------------------
	static void AlphaBlendU(PBYTE, PBYTE, int, int, PBYTE, BYTE);

protected:
	HDC     m_hDestDC;    // Handle to the destination device context.
	CBitmap m_bitmap;     // Bitmap in memory device context
	CRect   m_rect;       // Size of the area to paint.
	HGDIOBJ m_hOldBitmap; // Handle to the previously selected bitmap.
};

#endif // !defined(__XTPANIMATIONDC_H__)
