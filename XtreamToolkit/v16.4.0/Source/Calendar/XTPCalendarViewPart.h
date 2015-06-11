// XTPCalendarViewPart.h: interface for the CXTPCalendarViewPart class.
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
#if !defined(_XTPCALENDARVIEWPART_H__)
#define _XTPCALENDARVIEWPART_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable: 4097)

class CXTPCalendarView;
class CXTPPropExchange;
class CXTPCalendarTimeLineView;
class CXTPCalendarTimeLineViewEvent;
class CXTPCalendarControl;

//===========================================================================
// Summary:
//     Helper class provides functionality to manage font settings for
//     various graphical elements of control.
//===========================================================================
class _XTP_EXT_CLASS CXTPCalendarViewPartFontValue
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Default constructor.
	//-----------------------------------------------------------------------
	CXTPCalendarViewPartFontValue()
	{

	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to determine if the default font
	//     value is set.
	// Returns:
	//     A BOOL. TRUE if the default value is set. FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL IsDefaultValue()
	{
		return (m_fntCustomValue.GetSafeHandle() == NULL) && (m_fntStandardValue.GetSafeHandle() == NULL);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to set the value for the standard
	//     font.
	// Parameters:
	//     pLogFont - A pointer to a LOGFONT structure that contains the
	//                standard font value.
	// Remarks:
	//     Call this member function to set the standard font. This font
	//     is used as the default font if there is not a custom font value
	//     set.
	//-----------------------------------------------------------------------
	void SetStandardValue(LOGFONT* pLogFont)
	{
		m_fntStandardValue.DeleteObject();
		m_fntStandardValue.CreateFontIndirect(pLogFont);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to delete the custom font and to
	//     set the default font value as the default font.
	// Remarks:
	//     Call this member function to ensure that the default font is
	//     used and not the custom font.
	//-----------------------------------------------------------------------
	void SetDefaultValue()
	{
		m_fntCustomValue.DeleteObject();
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is use to overload the function call operator
	//     for the CXTPCalendarViewPartFontValue class.
	// Remarks:
	//     Use the default font if the custom font does not work. Otherwise,
	//     use a custom font.
	// Returns:
	//     A reference to a CFont object that contains either the standard
	//     font value or the custom font value.
	//-----------------------------------------------------------------------
	operator CFont& ()
	{
		return  (m_fntCustomValue.GetSafeHandle() == 0) ? m_fntStandardValue : m_fntCustomValue;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is use to overload the function call operator
	//     for the CXTPCalendarViewPartFontValue class.
	// Remarks:
	//     Use the default font if the custom font does not work. Otherwise,
	//     use a custom font.
	// Returns:
	//     A pointer to a CFont object that contains either the standard
	//     font value or the custom font value.
	//-----------------------------------------------------------------------
	operator CFont* ()
	{
		return  (m_fntCustomValue.GetSafeHandle() == 0) ? &m_fntStandardValue : &m_fntCustomValue;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function overloads the assignment operator for the
	//     CXTPCalendarViewPartFontValue class.
	// Parameters:
	//     pLogFont - A pointer to a LOGFONT structure.
	// Remarks:
	//     Creates a new custom font.
	// Returns:
	//     A reference to a CXTPCalendarViewPartFontValue.
	//-----------------------------------------------------------------------
	const CXTPCalendarViewPartFontValue& operator=(LOGFONT* pLogFont)
	{
		m_fntCustomValue.DeleteObject();
		if (pLogFont) m_fntCustomValue.CreateFontIndirect(pLogFont);
		return *this;
	}
	//-----------------------------------------------------------------------
	// Summary:
	//     Get a current value object.
	// Returns:
	//     Pointer to the current value object.
	//-----------------------------------------------------------------------
	CFont* operator->()
	{
		return (m_fntCustomValue.GetSafeHandle() == 0) ? &m_fntStandardValue : &m_fntCustomValue;
	};

protected:
	CFont m_fntStandardValue;   // Stores default font.
	CFont m_fntCustomValue;     // Stores custom font.

};

//===========================================================================
// Summary:
//      Helper class template provides functionality to manage customized
//      value for the specified type.
//===========================================================================
template<class _TValue, class _TValueRef = _TValue&>
class CXTPCalendarThemeCustomizableXValueT
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Default object constructor.
	//-----------------------------------------------------------------------
	CXTPCalendarThemeCustomizableXValueT()
	{
		m_bIsStandardSet = FALSE;
		m_bIsCustomSet = FALSE;

		m_bAutoDestroy_Standard = FALSE;
		m_bAutoDestroy_Custom = FALSE;

		m_ValueStandard = _TValue();
		m_ValueCustom = _TValue();
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Default object destructor.
	//-----------------------------------------------------------------------
	virtual ~CXTPCalendarThemeCustomizableXValueT()
	{
		if (m_bAutoDestroy_Standard)
		{
			DoDestroy(m_ValueStandard);
		}
		if (m_bAutoDestroy_Custom)
		{
			DoDestroy(m_ValueCustom);
		}
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to destroy objects of type _TValue if
	//     necessary (Close handles, free memory, ...).
	// Parameters:
	//      refValue - An object reference to destroy.
	//-----------------------------------------------------------------------
	virtual void DoDestroy(_TValue& refValue){UNREFERENCED_PARAMETER(refValue);};

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to determine if the default value is set.
	// Returns:
	//     A BOOL. TRUE if the default value is set. FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL IsDefaultValue() const
	{
		return !m_bIsCustomSet && !m_bIsStandardSet;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function determines if the standard value is set and
	//     used.
	// Returns:
	//     TRUE if standard value is set and custom value is not set,
	//     otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL IsStandardValue() const
	{
		return !m_bIsCustomSet && m_bIsStandardSet;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function determines if the custom value is set and
	//     used.
	// Returns:
	//     TRUE if custom value is set, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL IsCustomValue() const
	{
		return m_bIsCustomSet;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//      This member function is used to set the standard value.
	// Parameters:
	//      refValue    - A standard value.
	//      bAutoDestroy - This parameter indicates should be a value object
	//                    destroyed when destroy or other value is set.
	//                    If TRUE - DoDestroy will be called.
	// Remarks:
	//     Call this member function to set the standard value. This value
	//     is used as the default value if there is not a custom value set.
	// See Also:
	//      SetCustomValue, SetDefaultValue
	//-----------------------------------------------------------------------
	virtual void SetStandardValue(_TValueRef refValue, BOOL bAutoDestroy)
	{
		if (m_bIsStandardSet && m_bAutoDestroy_Standard)
		{
			DoDestroy(m_ValueStandard);
		}
		m_ValueStandard = refValue;

		m_bAutoDestroy_Standard = bAutoDestroy;
		m_bIsStandardSet = TRUE;
	}

	//<COMBINE CXTPCalendarThemeCustomizableXValueT::SetStandardValue@_TValueRef@BOOL>
	virtual void SetStandardValue(_TValueRef refValue)
	{
		SetStandardValue(refValue, m_bAutoDestroy_Standard);
	}


	//-----------------------------------------------------------------------
	// Summary:
	//      This member function is used to set the custom value.
	// Parameters:
	//      refValue    - A custom value.
	//      bAutoDestroy - This parameter indicates should be a value object
	//                    destroyed when destroy or other value is set.
	//                    If TRUE - DoDestroy will be called.
	// Remarks:
	//     Call this member function to set the custom value. If set, this value
	//     is used instead of default value.
	// See Also:
	//      SetStandardValue, SetDefaultValue
	//-----------------------------------------------------------------------
	virtual void SetCustomValue(_TValueRef refValue, BOOL bAutoDestroy)
	{
		if (m_bIsCustomSet && m_bAutoDestroy_Custom)
		{
			DoDestroy(m_ValueCustom);
		}
		m_ValueCustom = refValue;

		m_bAutoDestroy_Custom = bAutoDestroy;
		m_bIsCustomSet = TRUE;
	}

	//<COMBINE CXTPCalendarThemeCustomizableXValueT::SetCustomValue@_TValueRef@BOOL>
	virtual void SetCustomValue(_TValueRef refValue)
	{
		SetCustomValue(refValue, m_bAutoDestroy_Custom);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//      This member function is used to reset the custom value.
	// Remarks:
	//      Call this member function to ensure that the default value is
	//      used and not the custom value.
	//      If the default value is not set - method do nothing.
	// See Also:
	//      SetStandardValue, SetCustomValue
	//-----------------------------------------------------------------------
	virtual void SetDefaultValue()
	{
		if (m_bIsStandardSet)
		{
			if (m_bIsCustomSet && m_bAutoDestroy_Custom)
			{
				DoDestroy(m_ValueCustom);
			}
			m_bIsCustomSet = FALSE;
			m_bAutoDestroy_Custom = FALSE;
		}
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is use to get current value.
	// Remarks:
	//     The default value is used if the custom value is not set. Otherwise,
	//     a custom value is used.
	// Returns:
	//     Current value as _TValueRef.
	// See Also:
	//      SetStandardValue, SetCustomValue, SetDefaultValue
	//-----------------------------------------------------------------------
	virtual _TValueRef GetValue() const
	{
		return  m_bIsCustomSet ? m_ValueCustom : m_ValueStandard;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is use to get current value.
	// Remarks:
	//     The default value is used if the custom value is not set. Otherwise,
	//     a custom value is used.
	// Returns:
	//     Current value as const _TValue&.
	// See Also:
	//      SetStandardValue, SetCustomValue, SetDefaultValue
	//-----------------------------------------------------------------------
	virtual const _TValue& GetValueX() const
	{
		return  m_bIsCustomSet ? m_ValueCustom : m_ValueStandard;
	}


	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is use to get standard value.
	// Returns:
	//     Standard value.
	// See Also:
	//      SetStandardValue, SetCustomValue, SetDefaultValue
	//-----------------------------------------------------------------------
	virtual _TValueRef GetStandardValue() const
	{
		return  m_ValueStandard;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member operator is use to get current value.
	// Remarks:
	//     The default value is used if the custom value is not set. Otherwise,
	//     a custom value is used.
	// Returns:
	//     Current value.
	// See Also:
	//      SetStandardValue, SetCustomValue, SetDefaultValue, GetValue
	//-----------------------------------------------------------------------
	operator _TValueRef () const
	{
		return GetValue();
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is use to set standard value using current value
	//     from the specified object.
	// Parameters:
	//     refSrc - Reference to a source object.
	// See Also:
	//      SetStandardValue, SetCustomValue, SetDefaultValue, GetValue,
	//      IsStandardValue, IsCustomValue, GetStandardValue.
	//-----------------------------------------------------------------------
	void CopySettings(const CXTPCalendarThemeCustomizableXValueT<_TValue, _TValueRef>& refSrc)
	{
		SetStandardValue(refSrc.GetValue());
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member operator is use to set custom value.
	// Parameters:
	//      refValue    - A new custom value.
	// Returns:
	//      Reference to this object.
	//-----------------------------------------------------------------------
	const CXTPCalendarThemeCustomizableXValueT<_TValue, _TValueRef>& operator=(_TValueRef refValue)
	{
		SetCustomValue(refValue);
		return *this;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This method reads or writes data from or to an archive.
	// Parameters:
	//     ar - A CArchive object to serialize to or from.
	// See Also: DoPropExchange()
	//-----------------------------------------------------------------------
	virtual void Serialize(CArchive& ar)
	{
		// for simple types which can be easy serialized.
		// override for more complex cases

		if (ar.IsStoring())
		{
			ar << m_bIsStandardSet;
			ar << m_bIsCustomSet;

			ar << m_ValueStandard;
			ar << m_ValueCustom;
		}
		else
		{
			ASSERT(ar.IsLoading());

			ar >> m_bIsStandardSet;
			ar >> m_bIsCustomSet;

			ar >> m_ValueStandard;
			ar >> m_ValueCustom;

		}
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This method reads or writes data from or to an storage.
	// Parameters:
	//     pPX - A CXTPPropExchange object to serialize to or from.
	//     pcszPropName - The name of the property being exchanged.
	// See Also: Serialize(), DoPX_Value()
	//-----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX, LPCTSTR pcszPropName) = 0;


protected:
	_TValue m_ValueStandard;            // Stores default value.
	BOOL    m_bAutoDestroy_Standard;    // Call DoDestroy for standard vale.
	BOOL    m_bIsStandardSet;           // Is standard value set.

	_TValue m_ValueCustom;              // Stores custom value.
	BOOL    m_bAutoDestroy_Custom;      // Call DoDestroy for custom vale.
	BOOL    m_bIsCustomSet;             // Is custom value set.
};

//===========================================================================
// Summary:
//      Helper class template provides functionality to manage customized
//      value for the specified class objects.
//===========================================================================
template<class _TValue>
class CXTPCalendarViewPartCustomizableValueT
{
public:
	typedef CXTPCalendarViewPartCustomizableValueT<_TValue> TBase;

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Default object constructor.
	//-----------------------------------------------------------------------
	CXTPCalendarViewPartCustomizableValueT()
	{
		m_pValueStandard = NULL;
		m_bAutoDelete_Standard = FALSE;

		m_pValueCustom = NULL;
		m_bAutoDelete_Custom = FALSE;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Default object destructor.
	//-----------------------------------------------------------------------
	virtual ~CXTPCalendarViewPartCustomizableValueT()
	{
		if (m_bAutoDelete_Standard)
		{
			SAFE_DELETE(m_pValueStandard);
		}
		if (m_bAutoDelete_Custom)
		{
			SAFE_DELETE(m_pValueCustom);
		}
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to determine if the default value is set.
	// Returns:
	//     A BOOL. TRUE if the default value is set. FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL IsDefaultValue() const
	{
		return (m_pValueCustom == NULL) && (m_pValueStandard == NULL);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function determines if the standard value is set and
	//     used.
	// Returns:
	//     TRUE if standard value is set and custom value is not set,
	//     otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL IsStandardValue() const
	{
		return (m_pValueCustom == NULL) && (m_pValueStandard != NULL);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function determines if the custom value is set and
	//     used.
	// Returns:
	//     TRUE if custom value is set, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL IsCustomValue() const
	{
		return m_pValueCustom != NULL;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//      This member function is used to set the standard value.
	// Parameters:
	//      pValue      - A pointer to a value object.
	//      bAutoDelete - This parameter indicates should be a value object
	//                    deleted when destroy or other value is set.
	//                    If TRUE - value object will be deleted automatically.
	// Remarks:
	//     Call this member function to set the standard value. This value
	//     is used as the default value if there is not a custom value set.
	// Example:
	//      See example for SetCustomValue.
	// See Also:
	//      SetCustomValue, SetDefaultValue
	//-----------------------------------------------------------------------
	virtual void SetStandardValue(_TValue* pValue, BOOL bAutoDelete)
	{
		if (m_bAutoDelete_Standard)
		{
			SAFE_DELETE(m_pValueStandard);
		}
		m_pValueStandard = pValue;
		m_bAutoDelete_Standard = bAutoDelete;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//      This member function is used to set the custom value.
	// Parameters:
	//      pValue      - A pointer to a value object.
	//      bAutoDelete - This parameter indicates should be a value object
	//                    deleted when destroy or other value is set.
	//                    If TRUE - value object will be deleted automatically.
	// Remarks:
	//     Call this member function to set the custom value. If set, this value
	//     is used instead of default value.
	// Example:
	// <code>
	//      class CMyClass
	//
	//      {
	//      public:
	//          CMyClass();
	//
	//          void Draw(CDC* pDC, CRect& rcRect, BOOL bCustom);
	//          // ...
	//          CBrush m_brushBusy_Custom;
	//
	//          CXTPCalendarViewPartBrushValue m_brushVal_auto;
	//          CXTPCalendarViewPartBrushValue m_brushVal_static;
	//      };
	//
	//      CMyClass::CMyClass()
	//      {
	//          m_brushBusy_Custom.CreateSolidBrush(RGB(0, 0, 0xFF));
	//
	//          m_brushVal_auto.SetStandardValue(new CBrush(RGB(0xFF, 0xFF, 0xFF)), TRUE);
	//          m_brushVal_static.SetStandardValue(&m_brushBusy_Custom, FALSE);
	//      }
	//
	//      void CMyClass::Draw(CDC* pDC, CRect& rcRect, BOOL bCustom)
	//      {
	//          if (bCustom)
	//          {
	//              m_brushVal_auto.SetCustomValue(new CBrush(RGB(0, 0, 0)), TRUE);
	//              m_brushVal_static.SetCustomValue(new CBrush(RGB(255, 255, 255)), TRUE);
	//          }
	//          else
	//          {
	//              // Reset to standard value
	//              m_brushVal_auto.SetDefaultValue();
	//              m_brushVal_static.SetDefaultValue();
	//          }
	//
	//          pDC->FillRect(&rcRect, (CBrush*)m_brushVal_auto);
	//
	//          CBrush* pBrushOld = pDC->SelectObject(m_brushVal_static.GetValue());
	//          // ....
	//          pDC->SelectObject(pBrushOld);
	//      }
	//
	// </code>
	// See Also:
	//      SetStandardValue, SetDefaultValue
	//-----------------------------------------------------------------------
	virtual void SetCustomValue(_TValue* pValue, BOOL bAutoDelete)
	{
		if (m_bAutoDelete_Custom)
		{
			SAFE_DELETE(m_pValueCustom);
		}
		m_pValueCustom = pValue;
		m_bAutoDelete_Custom = bAutoDelete;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//      This member function is used to reset the custom value.
	// Remarks:
	//      Call this member function to ensure that the default value is
	//      used and not the custom value.
	//      If the default value is not set - method do nothing.
	// Example:
	//      See example for SetCustomValue.
	// See Also:
	//      SetStandardValue, SetCustomValue
	//-----------------------------------------------------------------------
	virtual void SetDefaultValue()
	{
		if (m_pValueStandard)
		{
			if (m_bAutoDelete_Custom)
			{
				SAFE_DELETE(m_pValueCustom);
			}
			m_pValueCustom = NULL;
		}
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is use to get current value.
	// Remarks:
	//     The default value is used if the custom value is not set. Otherwise,
	//     a custom value is used.
	// Returns:
	//     A pointer to the current value.
	// Example:
	//      See example for SetCustomValue.
	// See Also:
	//      SetStandardValue, SetCustomValue, SetDefaultValue, operator _TValue*
	//-----------------------------------------------------------------------
	virtual _TValue* GetValue() const
	{
		return  m_pValueCustom ? m_pValueCustom : m_pValueStandard;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is use to get standard value.
	// Returns:
	//     A pointer to the standard value.
	// Example:
	//      See example for SetCustomValue.
	// See Also:
	//      SetStandardValue, SetCustomValue, SetDefaultValue, operator _TValue*
	//-----------------------------------------------------------------------
	virtual _TValue* GetStandardValue() const
	{
		return  m_pValueStandard;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member operator is use to get current value.
	// Remarks:
	//     The default value is used if the custom value is not set. Otherwise,
	//     a custom value is used.
	// Returns:
	//     A pointer to the current value.
	// Example:
	//      See example for SetCustomValue.
	// See Also:
	//      SetStandardValue, SetCustomValue, SetDefaultValue, GetValue
	//-----------------------------------------------------------------------
	operator _TValue* () const
	{
		return GetValue();
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Get a current value object.
	// Returns:
	//     Pointer to the current value object.
	//-----------------------------------------------------------------------
	_TValue* operator->()
	{
		return GetValue();
	};

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is use to set standard value using current value
	//     from the specified object.
	// Parameters:
	//     refSrc - Reference to a source object.
	// See Also:
	//      SetStandardValue, SetCustomValue, SetDefaultValue, GetValue,
	//      IsStandardValue, IsCustomValue, GetStandardValue.
	//-----------------------------------------------------------------------
	void CopySettings(const CXTPCalendarViewPartCustomizableValueT<_TValue>& refSrc)
	{
		SetStandardValue(refSrc.GetValue(), FALSE);
	}

protected:
	_TValue* m_pValueStandard;      // Stores default value.
	BOOL    m_bAutoDelete_Standard; // Call operator delete for the standard value.

	_TValue* m_pValueCustom;        // Stores custom value.
	BOOL    m_bAutoDelete_Custom;   // Call operator delete for the custom value.
};

//===========================================================================
// Summary:
//     Helper class provides functionality to manage brush value objects.
//===========================================================================
class _XTP_EXT_CLASS CXTPCalendarViewPartBrushValue : public
			CXTPCalendarViewPartCustomizableValueT<CBrush>
{
};

//===========================================================================
// Summary:
//     Helper class provides functionality to manage font value objects.
//===========================================================================
class _XTP_EXT_CLASS CXTPCalendarThemeFontValue : public
			CXTPCalendarViewPartCustomizableValueT<CFont>
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Default constructor.
	//-----------------------------------------------------------------------
	CXTPCalendarThemeFontValue()
	{
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to set the value for the standard
	//     font.
	// Parameters:
	//     pLogFont - A pointer to a LOGFONT structure that contains the
	//                standard font value.
	// Remarks:
	//     Call this member function to set the standard font. This font
	//     is used as the default font if there is not a custom font value
	//     set.
	//-----------------------------------------------------------------------
	virtual void SetStandardValue(LOGFONT* pLogFont)
	{
		CFont* pFont = new CFont();
		if (pFont)
		{
			VERIFY(pFont->CreateFontIndirect(pLogFont));
		}
		TBase::SetStandardValue(pFont, TRUE);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to set the value for the custom
	//     font.
	// Parameters:
	//     pLogFont - A pointer to a LOGFONT structure that contains the
	//                custom font value.
	//     pFont    - A pointer to CFont object.
	// Remarks:
	//     Call this member function to set the custom font.
	//     If set, this font is used as the object value.
	//-----------------------------------------------------------------------
	virtual void SetCustomValue(LOGFONT* pLogFont)
	{
		CFont* pFont = new CFont();
		if (pFont)
		{
			VERIFY(pFont->CreateFontIndirect(pLogFont));
		}
		TBase::SetCustomValue(pFont, TRUE);
	}

	// ------------------------------------------------------------------
	// <combine CXTPCalendarViewPartCustomizableValueT::SetStandardValue>
	//
	// Summary:
	//     used to assign standard value to one of view part element
	// Parameters:
	//     pFont :  pointer to CFont object
	// ------------------------------------------------------------------
	virtual void SetStandardValue(CFont* pFont)
	{
		if (!pFont)
		{
			TBase::SetStandardValue(pFont, FALSE);
			return;
		}

		LOGFONT lfFont;
		int nRes = pFont->GetLogFont(&lfFont);
		ASSERT(nRes);
		if(nRes)
			SetStandardValue(&lfFont);
	}

	// ----------------------------------------------------------------
	// <combine CXTPCalendarViewPartCustomizableValueT::SetCustomValue>
	//
	// Summary:
	//     used to assign custom value to one of view part element
	// Parameters:
	//     pFont :  pointer to CFont object
	// ----------------------------------------------------------------
	virtual void SetCustomValue(CFont* pFont)
	{
		if (!pFont)
		{
			TBase::SetStandardValue(pFont, FALSE);
			return;
		}

		LOGFONT lfFont;
		int nRes = pFont->GetLogFont(&lfFont);
		ASSERT(nRes);
		if(nRes)
			SetCustomValue(&lfFont);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function overloads the assignment operator for the
	//     CXTPCalendarViewPartFontValue class.
	// Parameters:
	//     rLogFont - A reference to LOGFONT structure.
	// Remarks:
	//     Creates a new custom font.
	// Returns:
	//     A reference to a CXTPCalendarViewPartFontValue.
	//-----------------------------------------------------------------------
	const CXTPCalendarThemeFontValue& operator=(LOGFONT& rLogFont)
	{
		SetCustomValue(&rLogFont);
		return *this;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is use to set standard value using current value
	//     from the specified object.
	// Parameters:
	//     refSrc - Reference to a source object.
	// See Also:
	//      SetStandardValue, SetCustomValue, SetDefaultValue, GetValue,
	//      IsStandardValue, IsCustomValue, GetStandardValue.
	//-----------------------------------------------------------------------
	void CopySettings(const CXTPCalendarThemeFontValue& refSrc)
	{
		SetStandardValue(refSrc.GetValue());
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This method reads or writes data from or to an archive.
	// Parameters:
	//     ar - A CArchive object to serialize to or from.
	// See Also: DoPropExchange()
	//-----------------------------------------------------------------------
	virtual void Serialize(CArchive& ar);
};

//===========================================================================
// Summary:
//     Helper class provides functionality to manage customized string value
//     objects.
//===========================================================================
class _XTP_EXT_CLASS CXTPCalendarThemeStringValue : public
			CXTPCalendarThemeCustomizableXValueT<CString, LPCTSTR>
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Default constructor.
	//-----------------------------------------------------------------------
	CXTPCalendarThemeStringValue()
	{
		m_bAutoDestroy_Standard = m_bAutoDestroy_Custom = TRUE;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member operator is use to get current value.
	// Remarks:
	//     The default value is used if the custom value is not set. Otherwise,
	//     a custom value is used.
	// Returns:
	//     A pointer to the current value.
	// Example:
	//      See example for SetCustomValue.
	// See Also:
	//      SetStandardValue, SetCustomValue, SetDefaultValue, GetValue
	//-----------------------------------------------------------------------
	operator const CString&() const
	{
		return GetValueX();
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member operator is use to set custom value.
	// Parameters:
	//      pcszValue    - A new custom value.
	// Returns:
	//      Reference to this object.
	//-----------------------------------------------------------------------
	const CXTPCalendarThemeStringValue& operator=(LPCTSTR pcszValue)
	{
		SetCustomValue(pcszValue);
		return *this;
	}

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method reads or writes data from or to an storage.
	// Parameters:
	//     pPX - A CXTPPropExchange object to serialize to or from.
	//     pcszPropName - The name of the property being exchanged.
	// See Also: Serialize(), DoPX_Value()
	//-----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX, LPCTSTR pcszPropName);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This member operator is use to destroy/clear value dada.
	// Parameters:
	//      refValue - A value reference to destroy/clear data.
	//-----------------------------------------------------------------------
	virtual void DoDestroy(CString& refValue)
	{
		refValue.Empty();
	};

	//-----------------------------------------------------------------------
	// Summary:
	//     This method reads or writes value data from or to an storage.
	// Parameters:
	//     pPX          - A CXTPPropExchange object to serialize to or from.
	//     pcszPropName - A value name.
	//     rXValue      - Reference to value.
	//     bStandard    - Standard or Custom value.
	// See Also: DoPropExchange()
	//-----------------------------------------------------------------------
	void DoPX_Value(CXTPPropExchange* pPX, LPCTSTR pcszPropName, CString& rXValue, BOOL bStandard);
};

//===========================================================================
// Summary:
//     Helper class provides functionality to manage customized int value
//     objects.
//===========================================================================
class _XTP_EXT_CLASS CXTPCalendarThemeIntValue : public CXTPCalendarThemeCustomizableXValueT<int, int>
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Default object constructor.
	//-----------------------------------------------------------------------
	CXTPCalendarThemeIntValue()
	{
		m_ValueStandard = m_ValueCustom = 0;
	}

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method reads or writes data from or to an storage.
	// Parameters:
	//     pPX - A CXTPPropExchange object to serialize to or from.
	//     pcszPropName - The name of the property being exchanged.
	// See Also: Serialize(), DoPX_Value()
	//-----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX, LPCTSTR pcszPropName);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member operator is use to set custom value.
	// Parameters:
	//      nValue    - A new custom value.
	// Returns:
	//      Reference to this object.
	//-----------------------------------------------------------------------
	const CXTPCalendarThemeIntValue& operator=(int nValue)
	{
		SetCustomValue(nValue);
		return *this;
	}

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method reads or writes value data from or to an storage.
	// Parameters:
	//     pPX          - A CXTPPropExchange object to serialize to or from.
	//     pcszPropName - A value name.
	//     rXValue      - Reference to value.
	//     bStandard    - Standard or Custom value.
	// See Also: DoPropExchange()
	//-----------------------------------------------------------------------
	void DoPX_Value(CXTPPropExchange* pPX, LPCTSTR pcszPropName, int& rXValue, BOOL bStandard);
};

//===========================================================================
// Summary:
//     Helper class provides functionality to manage customized BOOL value
//     objects.
//===========================================================================
class _XTP_EXT_CLASS CXTPCalendarThemeBOOLValue : public CXTPCalendarThemeCustomizableXValueT<BOOL, BOOL>
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Default object constructor.
	//-----------------------------------------------------------------------
	CXTPCalendarThemeBOOLValue()
	{
		m_ValueStandard = m_ValueCustom = FALSE;
	}

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method reads or writes data from or to an storage.
	// Parameters:
	//     pPX - A CXTPPropExchange object to serialize to or from.
	//     pcszPropName - The name of the property being exchanged.
	// See Also: Serialize(), DoPX_Value()
	//-----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX, LPCTSTR pcszPropName);


	//-----------------------------------------------------------------------
	// Summary:
	//     This member operator is use to set custom value.
	// Parameters:
	//      bValue    - A new custom value.
	// Returns:
	//      Reference to this object.
	//-----------------------------------------------------------------------
	const CXTPCalendarThemeBOOLValue& operator=(BOOL bValue)
	{
		SetCustomValue(bValue);
		return *this;
	}

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method reads or writes value data from or to an storage.
	// Parameters:
	//     pPX          - A CXTPPropExchange object to serialize to or from.
	//     pcszPropName - A value name.
	//     rXValue      - Reference to value.
	//     bStandard    - Standard or Custom value.
	// See Also: DoPropExchange()
	//-----------------------------------------------------------------------
	void DoPX_Value(CXTPPropExchange* pPX, LPCTSTR pcszPropName, BOOL& rXValue, BOOL bStandard);
};

//===========================================================================
// Summary:
//     Helper class provides functionality to manage customized CRect value
//     objects.
//===========================================================================
class _XTP_EXT_CLASS CXTPCalendarThemeRectValue :
		public CXTPCalendarThemeCustomizableXValueT<CRect, CRect>
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Default object constructor.
	//-----------------------------------------------------------------------
	CXTPCalendarThemeRectValue()
	{
		m_ValueStandard = m_ValueCustom = CRect(0, 0, 0, 0);
	}

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method reads or writes data from or to an storage.
	// Parameters:
	//     pPX - A CXTPPropExchange object to serialize to or from.
	//     pcszPropName - The name of the property being exchanged.
	// See Also: Serialize(), DoPX_Value()
	//-----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX, LPCTSTR pcszPropName);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member operator is use to set custom value.
	// Parameters:
	//      rcValue    - A new custom value.
	// Returns:
	//      Reference to this object.
	//-----------------------------------------------------------------------
	const CXTPCalendarThemeRectValue& operator=(const CRect& rcValue)
	{
		SetCustomValue((CRect&)rcValue);
		return *this;
	}

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method reads or writes value data from or to an storage.
	// Parameters:
	//     pPX          - A CXTPPropExchange object to serialize to or from.
	//     pcszPropName - A value name.
	//     rXValue      - Reference to value.
	//     bStandard    - Standard or Custom value.
	// See Also: DoPropExchange()
	//-----------------------------------------------------------------------
	void DoPX_Value(CXTPPropExchange* pPX, LPCTSTR pcszPropName, CRect& rXValue, BOOL bStandard);

};

//===========================================================================
// Summary:
//     Helper base class  to implement parts for calendar paint manager.
//     objects.
//===========================================================================
class _XTP_EXT_CLASS CXTPCalendarViewPart : public CXTPCmdTarget
{
	//{{AFX_CODEJOCK_PRIVATE
	friend class CXTPCalendarPaintManager;
	//}}AFX_CODEJOCK_PRIVATE

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Default constructor.
	// Parameters:
	//     pParentPart - Pointer to parent class, can be NULL.
	//-----------------------------------------------------------------------
	CXTPCalendarViewPart(CXTPCalendarViewPart* pParentPart = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Default destructor.
	//-----------------------------------------------------------------------
	virtual ~CXTPCalendarViewPart();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to set graphical related
	//     parameters equal to the system settings.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the color used to fill
	//     the background of UI elements.
	// Returns:
	//     A COLORREF that contains the value of the background color.
	//-----------------------------------------------------------------------
	virtual COLORREF GetBackgroundColor();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the color used to display text.
	// Returns:
	//     A COLORREF that contains the value of text color.
	//-----------------------------------------------------------------------
	virtual COLORREF GetTextColor();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain a pointer to a CFont
	//     object that contains the font that is used for displaying the
	//     text.
	// Returns:
	//     A reference to a CFont object that contains the font used to
	//     display the text.
	//-----------------------------------------------------------------------
	virtual CFont& GetTextFont();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to display the text using the
	//     custom font and color.
	// Parameters:
	//     pDC     - A pointer to a valid device context.
	//     str     - A CString that contains the text to display.
	//     lpRect  - An LPRECT that contains the rectangle coordinates
	//               used to display the text.
	//     nFormat - A UINT that contains additional format parameters.
	//-----------------------------------------------------------------------
	void DrawText(CDC* pDC, const CString& str, LPRECT lpRect, UINT nFormat)
	{
		CFont* pOldFont = pDC->SelectObject(&GetTextFont());
		pDC->SetTextColor(GetTextColor());
		nFormat |= DT_NOPREFIX;
		pDC->DrawText(str, lpRect, nFormat);
		pDC->SelectObject(pOldFont);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to draw a single line text in the
	//     center of the rect. If rect width is not enough to draw all chars -
	//     text is aligned to left (or right, see nFormat) or the rect.
	// Parameters:
	//     pDC     - A pointer to a valid device context.
	//     str     - A CString that contains the text to display.
	//     lpRect  - An LPRECT that contains the rectangle coordinates
	//               used to display the text.
	//     nFormat - A UINT that contains additional format parameters as
	//               combination of flags: DT_VCENTER, DT_LEFT, DT_RIGHT or 0.
	//-----------------------------------------------------------------------
	void DrawLine_CenterLR(CDC* pDC, const CString& str, LPRECT lpRect, UINT nFormat)
	{
		CFont* pOldFont = pDC->SelectObject(&GetTextFont());
		pDC->SetTextColor(GetTextColor());
		nFormat |= DT_NOPREFIX | DT_SINGLELINE;

		int nLeftRight = nFormat & (DT_LEFT | DT_RIGHT);
		nFormat &= ~(DT_CENTER | DT_LEFT | DT_RIGHT);

		CSize sz = pDC->GetTextExtent(str);
		if (sz.cx < labs(lpRect->right - lpRect->left) )
		{
			nFormat |= DT_CENTER;
		}
		else
		{
			nFormat |= nLeftRight ? nLeftRight : DT_LEFT;
		}

		pDC->DrawText(str, lpRect, nFormat);

		pDC->SelectObject(pOldFont);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to draw a single line text in the rect.
	//     If rect width is not enough to draw all chars - nFormatSmall flags are used,
	//     otherwise nFormatNormal flags are used.
	// Parameters:
	//     pDC           - A pointer to a valid device context.
	//     str           - A CString that contains the text to display.
	//     lpRect        - An LPRECT that contains the rectangle coordinates
	//                     used to display the text.
	//     nFormatNormal - A UINT that contains format parameters when rect width
	//                     is enough to draw all text chars.
	//     nFormatSmall  - A UINT that contains format parameters when rect width
	//                     is not enough to draw all text chars.
	//-----------------------------------------------------------------------
	void DrawLineEx(CDC* pDC, const CString& str, LPRECT lpRect, UINT nFormatNormal, UINT nFormatSmall)
	{
		CFont* pOldFont = pDC->SelectObject(&GetTextFont());
		pDC->SetTextColor(GetTextColor());
		UINT nFormat = 0;

		CSize sz = pDC->GetTextExtent(str);
		if (sz.cx < labs(lpRect->right - lpRect->left) )
		{
			nFormat = nFormatNormal | DT_NOPREFIX | DT_SINGLELINE;
		}
		else
		{
			nFormat = nFormatSmall | DT_NOPREFIX | DT_SINGLELINE;
		}

		pDC->DrawText(str, lpRect, nFormat);

		pDC->SelectObject(pOldFont);
	}


	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to calculate the size of the area
	//     required to display the given text.
	// Parameters:
	//     pDC - A pointer to a valid device context.
	//     str - A CString that contains the string of text to display.
	// Returns:
	//     A CSize object that contains the dimensions required to display
	//     the text.
	//-----------------------------------------------------------------------
	CSize GetTextExtent(CDC* pDC, const CString& str)
	{
		CFont* pOldFont = pDC->SelectObject(&GetTextFont());
		CSize sz = pDC->GetTextExtent(str);
		pDC->SelectObject(pOldFont);
		return sz;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to set the new color used to fill
	//     the background.
	// Parameters:
	//     clr - A COLORREF that contains the new color value.
	//-----------------------------------------------------------------------
	void SetBackgroundColor(COLORREF clr)
	{
		m_clrBackground = clr;
	}
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to set the new color used to display
	//     the text.
	// Parameters:
	//     clr - A COLORREF that contains the new color value.
	//-----------------------------------------------------------------------
	void SetTextColor(COLORREF clr)
	{
		m_clrTextColor = clr;
	}
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to set the new font used to display
	//     the text.
	// Parameters:
	//     pLogFont - A pointer to a LOGFONT struct that contains the new
	//                font used to display the text.
	//-----------------------------------------------------------------------
	void SetTextFont(LOGFONT* pLogFont)
	{
		m_fntText = pLogFont;
	}

	CXTPCalendarControl* GetCalendarControl() const;

// Attributes
protected:
	CXTPPaintManagerColor m_clrTextColor;  // Stores color settings used to display text.
	CXTPPaintManagerColor m_clrBackground; // Stores color settings used to to fill background of UI item.
	CXTPCalendarViewPartFontValue m_fntText;        // Stores font settings used to display text.

	CXTPCalendarViewPart* m_pParentPart;            // Pointer to the parent CXTPCalendarViewPart object.
	CXTPCalendarPaintManager* m_pPaintManager;      // Pointer to containing CXTPCalendarPaintManager object

protected:
	//{{AFX_CODEJOCK_PRIVATE
	virtual void _Init(){};
	//}}AFX_CODEJOCK_PRIVATE
};


//===========================================================================
// Summary:
//     Helper base class  to implement parts for calendar paint manager.
//     objects.
//===========================================================================
class _XTP_EXT_CLASS CXTPCalendarTimeLineViewTimeScalePart : public CXTPCalendarViewPart
{
public:
	// -----------------
	// Summary:
	//     Default object constructor.
	// Parameters:
	//     pParentPart :  pointer to CXTPCalendarViewPart
	// --------------------------------
	CXTPCalendarTimeLineViewTimeScalePart(CXTPCalendarViewPart* pParentPart = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to set graphical related
	//     parameters equal to the system settings.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

	// ------------------------
	// Summary:
	//     This member function is used to calculate height utilizing
	//     the specified device context.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	// Returns:
	//     calculated height as int
	// ------------------------
	virtual int CalcHeigt(CDC* pDC);

	// ---------------------------
	// Summary:
	//     This member function is used to draw the part content utilizing
	//     the specified device context.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//     rcRect :  rectangle to use
	//     pView :   pointer to CXTPCalendarTimeLineView
	// ---------------------------
	virtual void Draw(CDC* pDC, const CRect& rcRect, CXTPCalendarTimeLineView* pView);

	// -----------------
	// Summary:
	//     access function to get header height
	// Returns:
	//     current header height as int
	// -----------------
	virtual int GetHeaderHeight()
	{
		return m_nHeaderHeight;
	}

	// -----------------------------------
	// Summary:
	//     access function to get header date format
	// Parameters:
	//     nLabelInterval :  int param used to select format
	// Returns:
	//     current header date format as CString object
	// -----------------------------------
	virtual CString GetHeaderDateFormat(int nLabelInterval);

	// -----------------------------------
	// Summary:
	//     This member function is used to draw the part content utilizing
	//     the specified device context.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//     rcRect :  rectangle to use
	//     pView :   pointer to CXTPCalendarTimeLineView
	//     nLabelInterval :  param used to select format
	// -----------------------------------
	virtual void DrawHeader(CDC* pDC, const CRect& rcRect, CXTPCalendarTimeLineView* pView, int nLabelInterval);

public:
	CXTPCalendarViewPartFontValue m_fntScaleHeaderText; // Time scale header text font.

protected:
	int m_nHeaderHeight; // internal value of current header height
};

//===========================================================================
// Summary:
//     Helper base class  to implement parts for calendar paint manager.
//     objects.
//===========================================================================
class _XTP_EXT_CLASS CXTPCalendarTimeLineViewPart : public CXTPCalendarViewPart
{
public:
	// -----------------
	// Summary:
	//     Default object constructor.
	// Parameters:
	//     pParentPart :  pointer to CXTPCalendarViewPart
	// --------------------------------
	CXTPCalendarTimeLineViewPart(CXTPCalendarViewPart* pParentPart = NULL) : CXTPCalendarViewPart(pParentPart)
	{
		UNREFERENCED_PARAMETER(pParentPart);
	};

	// ---------------------------
	// Summary:
	//     This member function is used to draw the part content utilizing
	//     the specified device context.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//     rcRect :  rectangle to use
	//     pView :   pointer to CXTPCalendarTimeLineView
	// ---------------------------
	virtual void Draw(CDC* pDC, const CRect& rcRect, CXTPCalendarTimeLineView* pView)
	{
		UNREFERENCED_PARAMETER(pDC);
		UNREFERENCED_PARAMETER(rcRect);
		UNREFERENCED_PARAMETER(pView);
	};

	// -----------------------------------
	// Summary:
	//     This member function is used to calculate rectangle needed to draw event
	// Parameters:
	//     pDC : Pointer to a valid device context.
	//     pEventView :  pointer to CXTPCalendarTimeLineViewEvent
	// Returns:
	//     CSize object needed to draw event
	// -----------------------------------
	virtual CSize CalcEventSize(CDC* pDC, CXTPCalendarTimeLineViewEvent* pEventView)
	{
		UNREFERENCED_PARAMETER(pDC);
		UNREFERENCED_PARAMETER(pEventView);
		return CSize(0, 0);
	};

	// ---------------------------
	// Summary:
	//     This member function is used to draw the event utilizing
	//     the specified device context.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//     rcEvents :  rectangle to use
	//     pEventView : pointer to CXTPCalendarTimeLineViewEvent
	// -----------------------------------
	virtual void DrawEvent(CDC* pDC, const CRect& rcEvents, CXTPCalendarTimeLineViewEvent* pEventView)
	{
		UNREFERENCED_PARAMETER(pDC);
		UNREFERENCED_PARAMETER(rcEvents);
		UNREFERENCED_PARAMETER(pEventView);
	};
};

//---------------------------------------------------------------------------
// Summary:
//     Call this function within your class's DoPropExchange member function
//     to serialize or initialize a COLORREF property.
//     The property's value is read from or written to the variable referenced
//     by refColor, as appropriate.
// Parameters:
//     pPX          - Pointer to the CXTPPropExchange object
//                    (typically passed as a parameter to DoPropExchange).
//     pcszPropName - The name of the property being exchanged.
//     refColor     - Reference to the variable where the property is stored.
// Returns:
//     Nonzero if the exchange was successful; 0 if unsuccessful.
//---------------------------------------------------------------------------
_XTP_EXT_CLASS BOOL AFX_CDECL PX_Color(CXTPPropExchange* pPX, LPCTSTR pcszPropName,
											COLORREF& refColor);

//---------------------------------------------------------------------------
// Summary:
//     Call this function within your class's DoPropExchange member function
//     to serialize or initialize a customized Color property.
//     The property's value is read from or written to the variable referenced
//     by refColor, as appropriate.
// Parameters:
//     pPX          - Pointer to the CXTPPropExchange object
//                    (typically passed as a parameter to DoPropExchange).
//     pcszPropName - The name of the property being exchanged.
//     refColor     - Reference to the variable where the property is stored.
// Returns:
//     Nonzero if the exchange was successful; 0 if unsuccessful.
//---------------------------------------------------------------------------
_XTP_EXT_CLASS BOOL AFX_CDECL PX_Color(CXTPPropExchange* pPX, LPCTSTR pcszPropName,
											CXTPPaintManagerColor& refColor);

//---------------------------------------------------------------------------
// Summary:
//     Call this function within your class's DoPropExchange member function
//     to serialize or initialize a customized Gradient Color property.
//     The property's value is read from or written to the variable referenced
//     by refGrColor, as appropriate.
// Parameters:
//     pPX          - Pointer to the CXTPPropExchange object
//                    (typically passed as a parameter to DoPropExchange).
//     psczPropName - The name of the property being exchanged.
//     refGrColor   - Reference to the variable where the property is stored.
// Returns:
//     Nonzero if the exchange was successful; 0 if unsuccessful.
//---------------------------------------------------------------------------
_XTP_EXT_CLASS BOOL AFX_CDECL PX_GrColor(CXTPPropExchange* pPX, LPCTSTR psczPropName,
											CXTPPaintManagerColorGradient& refGrColor);

//---------------------------------------------------------------------------
// Summary:
//     Call this function within your class's DoPropExchange member function
//     to serialize or initialize a LOGFONT property.
//     The property's value is read from or written to the variable referenced
//     by rLogFont, as appropriate.
// Parameters:
//     pPX          - Pointer to the CXTPPropExchange object
//                    (typically passed as a parameter to DoPropExchange).
//     pcszPropName - The name of the property being exchanged.
//     rLogFont     - Reference to the variable where the property is stored.
// Returns:
//     Nonzero if the exchange was successful; 0 if unsuccessful.
//---------------------------------------------------------------------------
_XTP_EXT_CLASS BOOL AFX_CDECL PX_Font(CXTPPropExchange* pPX, LPCTSTR pcszPropName,
											LOGFONT& rLogFont);

//---------------------------------------------------------------------------
// Summary:
//     Call this function within your class's DoPropExchange member function
//     to serialize or initialize a customized Font property.
//     The property's value is read from or written to the variable referenced
//     by refFont, as appropriate.
// Parameters:
//     pPX          - Pointer to the CXTPPropExchange object
//                    (typically passed as a parameter to DoPropExchange).
//     pcszPropName - The name of the property being exchanged.
//     refFont      - Reference to the variable where the property is stored.
// Returns:
//     Nonzero if the exchange was successful; 0 if unsuccessful.
//---------------------------------------------------------------------------
_XTP_EXT_CLASS BOOL AFX_CDECL PX_Font(CXTPPropExchange* pPX, LPCTSTR pcszPropName,
										CXTPCalendarThemeFontValue& refFont);

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

#endif // !defined(_XTPCALENDARVIEWPART_H__)
