// XTPMarkupButton.cpp: implementation of the CXTPMarkupButton class.
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
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

#include "stdafx.h"

#include "Common/XTPDrawHelpers.h"
#include "Common/XTPWinThemeWrapper.h"

#include "XTPMarkupObject.h"
#include "XTPMarkupInputElement.h"
#include "XTPMarkupUIElement.h"
#include "XTPMarkupFrameworkElement.h"
#include "XTPMarkupControl.h"
#include "XTPMarkupButton.h"
#include "XTPMarkupDrawingContext.h"
#include "XTPMarkupThickness.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPMarkupButtonBase

CXTPMarkupRoutedEvent* CXTPMarkupButtonBase::m_pClickEvent = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupButtonBase::m_pIsPressedProperty = NULL;

IMPLEMENT_MARKUPCLASS(NULL, CXTPMarkupButtonBase, CXTPMarkupContentControl)

void CXTPMarkupButtonBase::RegisterMarkupClass()
{
	m_pClickEvent = CXTPMarkupRoutedEvent::RegisterRoutedEvent(L"Click", CXTPMarkupRoutedEvent::routingBubble, MARKUP_TYPE(CXTPMarkupButtonBase));
	m_pIsPressedProperty = CXTPMarkupDependencyProperty::Register(L"IsPressed", MARKUP_TYPE(CXTPMarkupBool), MARKUP_TYPE(CXTPMarkupButtonBase),
		new CXTPMarkupPropertyMetadata(CXTPMarkupBool::CreateFalseValue(), CXTPMarkupPropertyMetadata::flagAffectsArrange));
}

CXTPMarkupButtonBase::CXTPMarkupButtonBase()
{
	m_bPushed = FALSE;
	m_bMouseOver = FALSE;

	m_themeButton = new CXTPWinThemeWrapper();

	SetValue(m_pFocusableProperty, CXTPMarkupBool::CreateTrueValue());
}

CXTPMarkupButtonBase::~CXTPMarkupButtonBase()
{
	SAFE_DELETE(m_themeButton);
}


void CXTPMarkupButtonBase::OnMouseLeave(CXTPMarkupMouseEventArgs* e)
{
	m_bMouseOver = FALSE;

	if (m_bPushed)
	{
		SetValue(m_pIsPressedProperty, NULL);
	}
	else
	{
		CXTPMarkupContentControl::OnMouseLeave(e);
		InvalidateVisual();
	}
}

void CXTPMarkupButtonBase::OnMouseEnter(CXTPMarkupMouseEventArgs* e)
{
	m_bMouseOver = TRUE;

	if (m_bPushed)
	{
		SetValue(m_pIsPressedProperty, CXTPMarkupBool::CreateTrueValue());
	}
	else
	{
		CXTPMarkupContentControl::OnMouseEnter(e);
		InvalidateVisual();
	}
}

void CXTPMarkupButtonBase::OnLostMouseCapture(CXTPMarkupMouseEventArgs* /*e*/)
{
	SetPressed(FALSE);
}

void CXTPMarkupButtonBase::SetPressed(BOOL bPressed)
{
	if (!bPressed && m_bPushed)
	{
		m_bPushed = FALSE;
		SetValue(m_pIsPressedProperty, NULL);

		if (!m_bMouseOver && IsMouseOver())
		{
			SetValue(m_pIsMouseOverProperty, NULL);
			InvalidateVisual();
		}
	}
	else if (bPressed && !m_bPushed)
	{
		m_bPushed = TRUE;
		SetValue(m_pIsPressedProperty, CXTPMarkupBool::CreateTrueValue());
	}
}

void CXTPMarkupButtonBase::OnMouseLeftButtonUp(CXTPMarkupMouseButtonEventArgs* e)
{
	if (m_bPushed)
	{
		ReleaseMouseCapture();

		SetPressed(FALSE);

		if (m_bMouseOver)
		{
			OnClick();
		}

		e->SetHandled();
	}
}

void CXTPMarkupButtonBase::OnMouseLeftButtonDown(CXTPMarkupMouseButtonEventArgs* e)
{
	e->SetHandled();

	if (!IsEnabled())
		return;

	Focus();

	SetPressed(TRUE);

	CaptureMouse();
}

void CXTPMarkupButtonBase::OnClick()
{
	CXTPMarkupRoutedEventArgs* e = new CXTPMarkupRoutedEventArgs(m_pClickEvent, this);
	RaiseEvent(e);
	e->Release();
}


//////////////////////////////////////////////////////////////////////////
// CXTPMarkupButton

CXTPMarkupButton::CXTPMarkupButton()
{
}

CXTPMarkupButton::~CXTPMarkupButton()
{
}

IMPLEMENT_MARKUPCLASS(L"Button", CXTPMarkupButton, CXTPMarkupButtonBase)

void CXTPMarkupButton::RegisterMarkupClass()
{
}

CSize CXTPMarkupButton::ArrangeOverride(CSize szFinalSize)
{
	CXTPMarkupUIElement* pContent = GetContent();
	if (pContent != NULL)
	{
		CRect rt(0, 0, szFinalSize.cx, szFinalSize.cy);

		if (IsPressed() && (!m_themeButton->IsAppThemed() || GetValue(m_pBackgroundProperty) != NULL))
		{
			rt.OffsetRect(1, 1);
		}

		CRect finalRect = CXTPMarkupThickness::HelperDeflateRect(rt, GetPadding());
		finalRect.DeflateRect(4, 4);
		pContent->Arrange(finalRect);
	}
	return szFinalSize;
}

CSize CXTPMarkupButton::MeasureOverride(CXTPMarkupDrawingContext* pDC, CSize szAvailableSize)
{
	m_themeButton->OpenTheme(0, L"BUTTON");

	CSize size3 = GetPadding()->GetSize();
	size3.cx += 8;
	size3.cy += 8;

	CXTPMarkupUIElement* pContent = GetContent();
	if (pContent != NULL)
	{
		CSize availableSize(max(0, szAvailableSize.cx - size3.cx), max(0, szAvailableSize.cy - size3.cy));
		pContent->Measure(pDC, availableSize);

		CSize desiredSize = pContent->GetDesiredSize();

		return CSize(desiredSize.cx + size3.cx, desiredSize.cy + size3.cy);
	}

	return size3;
}


void CXTPMarkupButton::OnRender(CXTPMarkupDrawingContext* pDC)
{
	BOOL bPressed = IsPressed();
	BOOL bHot = IsMouseOver();
	BOOL bDefault = FALSE;
	BOOL bEnabled = IsEnabled();
	BOOL bChecked = FALSE;
	CRect rc(0, 0, GetRenderSize().cx, GetRenderSize().cy);

	CXTPMarkupBrush* pBackground = MARKUP_STATICCAST(CXTPMarkupBrush, GetValue(m_pBackgroundProperty));

	if (pBackground == NULL && m_themeButton->IsAppThemed())
	{
		HDC hDC = pDC->GetDC();

		int nState = !bEnabled? PBS_DISABLED: bPressed? PBS_PRESSED: bHot? PBS_HOT: bChecked ? PBS_PRESSED : bDefault? PBS_DEFAULTED: PBS_NORMAL;
		m_themeButton->DrawThemeBackground(hDC, BP_PUSHBUTTON, nState, rc, NULL);

		pDC->ReleaseDC(hDC);
	}
	else
	{
		COLORREF clrLight, clrDark, clrDarkDark, clrBackground;

		if (pBackground)
		{
			clrBackground = pBackground->GetHintColor();
			DWORD dwHSLBackground = CXTPDrawHelpers::RGBtoHSL(clrBackground);

			DWORD dwL = GetBValue(dwHSLBackground);

			DWORD dwLight = (dwL + 240) / 2;

			clrLight = CXTPDrawHelpers::HSLtoRGB(RGB(GetRValue(dwHSLBackground), GetGValue(dwHSLBackground), dwLight));

			DWORD dwDark = MulDiv(dwL, 2, 3);

			clrDark = CXTPDrawHelpers::HSLtoRGB(RGB(GetRValue(dwHSLBackground), GetGValue(dwHSLBackground), dwDark));
			clrDarkDark = RGB(GetRValue(clrDark) / 2, GetGValue(clrDark) / 2, GetBValue(clrDark) / 2);

			pDC->FillRectangle(rc, pBackground);
		}
		else
		{
			clrBackground = GetSysColor(COLOR_3DFACE);
			clrLight = GetSysColor(COLOR_WINDOW);
			clrDark = GetSysColor(COLOR_BTNSHADOW);
			clrDarkDark = GetSysColor(COLOR_3DDKSHADOW);

			CXTPMarkupSolidColorBrush brushBackground(clrBackground);
			pDC->FillRectangle(rc, &brushBackground);
		}

		CXTPMarkupSolidColorBrush brushDark(clrDark);

		if (bPressed)
		{
			CXTPMarkupSolidColorBrush brushFrame(GetSysColor(COLOR_WINDOWFRAME));

			pDC->DrawFrame(rc, &brushFrame);
			rc.DeflateRect(1, 1);
			pDC->DrawFrame(rc, &brushDark);
		}
		else
		{
			CXTPMarkupSolidColorBrush brushLight(clrLight);
			CXTPMarkupSolidColorBrush brushDarkDark(clrDarkDark);

			pDC->FillRectangle(CRect(rc.left, rc.top, rc.right, 1), &brushLight);
			pDC->FillRectangle(CRect(rc.left, rc.top, rc.left + 1, rc.bottom), &brushLight);

			pDC->FillRectangle(CRect(rc.left + 1, rc.bottom - 2, rc.right - 1, rc.bottom - 1), &brushDark);
			pDC->FillRectangle(CRect(rc.right - 2, rc.top + 1, rc.right - 1, rc.bottom - 1), &brushDark);

			pDC->FillRectangle(CRect(rc.left, rc.bottom - 1, rc.right - 1, rc.bottom), &brushDarkDark);
			pDC->FillRectangle(CRect(rc.right - 1, rc.top, rc.right, rc.bottom), &brushDarkDark);
		}
	}
}

void CXTPMarkupButton::OnRenderFocusVisual(CXTPMarkupDrawingContext* drawingContext)
{
	CRect rc(0, 0, GetRenderSize().cx, GetRenderSize().cy);

	rc.DeflateRect(3, 3);

	HDC hDC = drawingContext->GetDC();

	SetTextColor(hDC, 0xFFFFFF);
	SetBkColor(hDC, 0);
	DrawFocusRect(hDC, rc);

	drawingContext->ReleaseDC(hDC);
}

//////////////////////////////////////////////////////////////////////////
// CXTPMarkupToggleButton

CXTPMarkupDependencyProperty* CXTPMarkupToggleButton::m_pIsThreeStateProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupToggleButton::m_pIsCheckedProperty = NULL;
CXTPMarkupRoutedEvent* CXTPMarkupToggleButton::m_pCheckedEvent = NULL;
CXTPMarkupRoutedEvent* CXTPMarkupToggleButton::m_pUncheckedEvent = NULL;
CXTPMarkupRoutedEvent* CXTPMarkupToggleButton::m_pIndeterminateEvent = NULL;


IMPLEMENT_MARKUPCLASS(NULL, CXTPMarkupToggleButton, CXTPMarkupButtonBase)

void CXTPMarkupToggleButton::RegisterMarkupClass()
{
	m_pCheckedEvent = CXTPMarkupRoutedEvent::RegisterRoutedEvent(L"Checked", CXTPMarkupRoutedEvent::routingBubble, MARKUP_TYPE(CXTPMarkupToggleButton));
	m_pUncheckedEvent = CXTPMarkupRoutedEvent::RegisterRoutedEvent(L"Unchecked", CXTPMarkupRoutedEvent::routingBubble, MARKUP_TYPE(CXTPMarkupToggleButton));
	m_pIndeterminateEvent = CXTPMarkupRoutedEvent::RegisterRoutedEvent(L"Indeterminate", CXTPMarkupRoutedEvent::routingBubble, MARKUP_TYPE(CXTPMarkupToggleButton));

	m_pIsCheckedProperty = CXTPMarkupDependencyProperty::Register(L"IsChecked", MARKUP_TYPE(CXTPMarkupEnum), MARKUP_TYPE(CXTPMarkupToggleButton),
		new CXTPMarkupPropertyMetadata(CXTPMarkupEnum::CreateValue(0), &CXTPMarkupToggleButton::ConvertIsChecked, &CXTPMarkupToggleButton::OnIsCheckedChanged, CXTPMarkupPropertyMetadata::flagAffectsRender));

	m_pIsThreeStateProperty = CXTPMarkupDependencyProperty::Register(L"IsThreeState", MARKUP_TYPE(CXTPMarkupBool), MARKUP_TYPE(CXTPMarkupToggleButton),
		new CXTPMarkupPropertyMetadata(CXTPMarkupBool::CreateFalseValue(), CXTPMarkupPropertyMetadata::flagAffectsRender));
}

CXTPMarkupObject* CXTPMarkupToggleButton::ConvertIsChecked(CXTPMarkupBuilder* /*pBuilder*/, CXTPMarkupObject* pObject)
{
	if (IsStringObject(pObject))
	{
		if (_wcsicmp(*((CXTPMarkupString*)pObject), L"True") == 0)
		{
			return CXTPMarkupEnum::CreateValue(BST_CHECKED);
		}

		if (_wcsicmp(*((CXTPMarkupString*)pObject), L"False") == 0)
		{
			return CXTPMarkupEnum::CreateValue(BST_UNCHECKED);
		}

		if (_wcsicmp(*((CXTPMarkupString*)pObject), L"") == 0)
		{
			return CXTPMarkupEnum::CreateValue(BST_INDETERMINATE);
		}
	}
	return NULL;
}

void CXTPMarkupToggleButton::OnClick()
{
	OnToggle();
	CXTPMarkupButtonBase::OnClick();
}

void CXTPMarkupToggleButton::OnToggle()
{
	if (GetChecked() == BST_CHECKED)
	{
		SetChecked(IsThreeState() ? BST_INDETERMINATE : BST_UNCHECKED);
	}
	else
	{
		SetChecked(GetChecked() == BST_INDETERMINATE ? BST_UNCHECKED : BST_CHECKED);
	}
}

void CXTPMarkupToggleButton::OnChecked(CXTPMarkupRoutedEventArgs* e)
{
	RaiseEvent(e);
}

void CXTPMarkupToggleButton::OnUnchecked(CXTPMarkupRoutedEventArgs* e)
{
	RaiseEvent(e);
}

void CXTPMarkupToggleButton::OnIndeterminate(CXTPMarkupRoutedEventArgs* e)
{
	RaiseEvent(e);
}

void CXTPMarkupToggleButton::OnIsCheckedChanged(CXTPMarkupObject* d, CXTPMarkupPropertyChangedEventArgs* e)
{
	CXTPMarkupToggleButton* pButton = MARKUP_DYNAMICCAST(CXTPMarkupToggleButton, d);
	if (!pButton)
		return;

	CXTPMarkupEnum* pValue = MARKUP_STATICCAST(CXTPMarkupEnum, e->m_pNewValue);

	int bValue = pValue != NULL ? (int)*pValue : FALSE;

	if (bValue == BST_CHECKED)
	{
		CXTPMarkupRoutedEventArgs args(CXTPMarkupToggleButton::m_pCheckedEvent, pButton);
		pButton->OnChecked(&args);
	}
	else if (bValue == BST_UNCHECKED)
	{
		CXTPMarkupRoutedEventArgs args(CXTPMarkupToggleButton::m_pUncheckedEvent, pButton);
		pButton->OnUnchecked(&args);
	}
	else
	{
		CXTPMarkupRoutedEventArgs args(CXTPMarkupToggleButton::m_pIndeterminateEvent, pButton);
		pButton->OnIndeterminate(&args);
	}
}


//////////////////////////////////////////////////////////////////////////
// CXTPMarkupCheckBox

IMPLEMENT_MARKUPCLASS(L"CheckBox", CXTPMarkupCheckBox, CXTPMarkupToggleButton)

void CXTPMarkupCheckBox::RegisterMarkupClass()
{

}


CXTPMarkupCheckBox::CXTPMarkupCheckBox()
{
}

CXTPMarkupCheckBox::~CXTPMarkupCheckBox()
{
}

CSize CXTPMarkupCheckBox::ArrangeOverride(CSize szFinalSize)
{
	CXTPMarkupUIElement* pContent = GetContent();
	if (pContent != NULL)
	{
		CRect rt(0, 0, szFinalSize.cx, szFinalSize.cy);

		CRect finalRect = CXTPMarkupThickness::HelperDeflateRect(rt, GetPadding());
		finalRect.DeflateRect(13 + 3, 0, 0, 0);
		pContent->Arrange(finalRect);
	}
	return szFinalSize;
}

CSize CXTPMarkupCheckBox::MeasureOverride(CXTPMarkupDrawingContext* pDC, CSize szAvailableSize)
{
	m_themeButton->OpenTheme(0, L"BUTTON");

	CSize size3 = GetPadding()->GetSize();
	size3.cx += 13 + 3;

	CXTPMarkupUIElement* pContent = GetContent();
	if (pContent != NULL)
	{
		CSize availableSize(max(0, szAvailableSize.cx - size3.cx), max(0, szAvailableSize.cy - size3.cy));
		pContent->Measure(pDC, availableSize);

		CSize desiredSize = pContent->GetDesiredSize();

		return CSize(desiredSize.cx + size3.cx, max(13, desiredSize.cy + size3.cy));
	}

	return size3;
}

void CXTPMarkupCheckBox::OnRender(CXTPMarkupDrawingContext* pDC)
{
	BOOL bPressed = IsPressed();
	BOOL bHot = IsMouseOver();
	BOOL bEnabled = IsEnabled();
	BOOL bChecked = GetChecked();

	CRect rc(0, GetRenderSize().cy / 2 - 6, 13, GetRenderSize().cy / 2 + 7);

	HDC hDC = pDC->GetDC();

	if (m_themeButton->IsAppThemed())
	{
		int nState = !bEnabled? CBS_UNCHECKEDDISABLED: bPressed? CBS_UNCHECKEDPRESSED: bHot?
			CBS_UNCHECKEDHOT: CBS_UNCHECKEDNORMAL;
		if (bChecked == BST_CHECKED)
			nState += 4;
		if (bChecked == BST_INDETERMINATE)
			nState += 8;

		m_themeButton->DrawThemeBackground(hDC, BP_CHECKBOX, nState, rc, NULL);
	}
	else
	{
		DrawFrameControl(hDC, rc, DFC_BUTTON, DFCS_BUTTONCHECK | (bPressed ? DFCS_PUSHED : 0)
			| (bChecked == BST_CHECKED ? DFCS_CHECKED : 0) | (bChecked == BST_INDETERMINATE ? DFCS_BUTTON3STATE | DFCS_CHECKED : 0));
	}

	pDC->ReleaseDC(hDC);
}

void CXTPMarkupCheckBox::OnRenderFocusVisual(CXTPMarkupDrawingContext* drawingContext)
{
	CRect rc(0, 0, GetRenderSize().cx, GetRenderSize().cy);

	CXTPMarkupVisual* pContent = GetContent();
	if (!pContent)
		return;

	rc.left += 13 + 2;

	HDC hDC = drawingContext->GetDC();

	SetTextColor(hDC, 0xFFFFFF);
	SetBkColor(hDC, 0);
	DrawFocusRect(hDC, rc);

	drawingContext->ReleaseDC(hDC);
}


//////////////////////////////////////////////////////////////////////////
// CXTPMarkupRadioButton

IMPLEMENT_MARKUPCLASS(L"RadioButton", CXTPMarkupRadioButton, CXTPMarkupToggleButton)

void CXTPMarkupRadioButton::RegisterMarkupClass()
{

}

CXTPMarkupRadioButton::CXTPMarkupRadioButton()
{
}

CXTPMarkupRadioButton::~CXTPMarkupRadioButton()
{
}

CSize CXTPMarkupRadioButton::ArrangeOverride(CSize szFinalSize)
{
	CXTPMarkupUIElement* pContent = GetContent();
	if (pContent != NULL)
	{
		CRect rt(0, 0, szFinalSize.cx, szFinalSize.cy);

		CRect finalRect = CXTPMarkupThickness::HelperDeflateRect(rt, GetPadding());
		finalRect.DeflateRect(13 + 3, 0, 0, 0);
		pContent->Arrange(finalRect);
	}
	return szFinalSize;
}

CSize CXTPMarkupRadioButton::MeasureOverride(CXTPMarkupDrawingContext* pDC, CSize szAvailableSize)
{
	m_themeButton->OpenTheme(0, L"BUTTON");

	CSize size3 = GetPadding()->GetSize();
	size3.cx += 13 + 3;

	CXTPMarkupUIElement* pContent = GetContent();
	if (pContent != NULL)
	{
		CSize availableSize(max(0, szAvailableSize.cx - size3.cx), max(0, szAvailableSize.cy - size3.cy));
		pContent->Measure(pDC, availableSize);

		CSize desiredSize = pContent->GetDesiredSize();

		return CSize(desiredSize.cx + size3.cx, max(13, desiredSize.cy + size3.cy));
	}

	return CSize(size3.cx, max(13, size3.cy));
}

void CXTPMarkupRadioButton::OnRender(CXTPMarkupDrawingContext* pDC)
{
	BOOL bPressed = IsPressed();
	BOOL bHot = IsMouseOver();
	BOOL bEnabled = IsEnabled();
	BOOL bChecked = GetChecked();

	CRect rc(0, GetRenderSize().cy / 2 - 6, 13, GetRenderSize().cy / 2 + 7);

	HDC hDC = pDC->GetDC();

	if (m_themeButton->IsAppThemed())
	{
		int nState = !bEnabled? RBS_UNCHECKEDDISABLED: bPressed? RBS_UNCHECKEDPRESSED: bHot?
			RBS_UNCHECKEDHOT: RBS_UNCHECKEDNORMAL;

		if (bChecked)
			nState += 4;

		m_themeButton->DrawThemeBackground(hDC, BP_RADIOBUTTON, nState, rc, NULL);
	}
	else
	{
		DrawFrameControl(hDC, rc, DFC_BUTTON, DFCS_BUTTONRADIO | (bPressed ? DFCS_PUSHED : 0)
			| (bChecked ? DFCS_CHECKED : 0));
	}

	pDC->ReleaseDC(hDC);
}

void CXTPMarkupRadioButton::OnRenderFocusVisual(CXTPMarkupDrawingContext* drawingContext)
{
	CRect rc(0, 0, GetRenderSize().cx, GetRenderSize().cy);

	CXTPMarkupVisual* pContent = GetContent();
	if (!pContent)
		return;

	rc.left += 13 + 2;

	HDC hDC = drawingContext->GetDC();

	SetTextColor(hDC, 0xFFFFFF);
	SetBkColor(hDC, 0);
	DrawFocusRect(hDC, rc);

	drawingContext->ReleaseDC(hDC);
}

void CXTPMarkupRadioButton::OnToggle()
{
	SetChecked(TRUE);
}

void CXTPMarkupRadioButton::OnChecked(CXTPMarkupRoutedEventArgs* e)
{
	UpdateRadioButtonGroup();
	CXTPMarkupToggleButton::OnChecked(e);
}


void CXTPMarkupRadioButton::UpdateRadioButtonGroup()
{
	CXTPMarkupVisual* pParent = GetVisualParent();
	if (!pParent)
		return;

	int nCount = pParent->GetVisualChildrenCount();

	for (int i = 0; i < nCount; i++)
	{
		CXTPMarkupRadioButton* pButton = MARKUP_DYNAMICCAST(CXTPMarkupRadioButton, pParent->GetVisualChild(i));

		if (pButton && pButton != this && pButton->GetChecked())
		{
			pButton->SetChecked(FALSE);
		}
	}
}

