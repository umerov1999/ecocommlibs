// XTPMarkupResourceDictionary.h: interface for the CXTPMarkupResourceDictionary class.
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

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPMARKUPRESOURCEDICTIONARY_H__)
#define __XTPMARKUPRESOURCEDICTIONARY_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class _XTP_EXT_CLASS CXTPMarkupResourceDictionary : public CXTPMarkupObject
{
	DECLARE_MARKUPCLASS(CXTPMarkupResourceDictionary);
public:
	CXTPMarkupResourceDictionary();
	~CXTPMarkupResourceDictionary();

public:
	void Add(CXTPMarkupObject* pKey, CXTPMarkupObject* pValue);
	CXTPMarkupObject* Lookup(const CXTPMarkupObject* pKey) const;

	static CXTPMarkupObject* AFX_CDECL FindResource(const CXTPMarkupObject* pElement, const CXTPMarkupObject* pKey);

protected:
	void SetContentObject(CXTPMarkupBuilder* pBuilder, CXTPMarkupObject* pContent);
	CXTPMarkupObject* GetObjectKey(CXTPMarkupObject* pContent) const;

protected:
	CMap<CXTPMarkupObjectPtr, CXTPMarkupObjectPtr, CXTPMarkupObjectPtr, CXTPMarkupObjectPtr> m_mapDictionary;
};

class _XTP_EXT_CLASS CXTPMarkupSetter : public CXTPMarkupObject
{
	DECLARE_MARKUPCLASS(CXTPMarkupSetter);
public:
	CXTPMarkupSetter();
	CXTPMarkupSetter(CXTPMarkupDependencyProperty* pProperty, CXTPMarkupObject* pValue);
public:
	CXTPMarkupDependencyProperty* GetSetterProperty() const;
	CXTPMarkupObject* GetSetterValue() const;

public:
	static CXTPMarkupDependencyProperty* m_pPropertyProperty;
	static CXTPMarkupDependencyProperty* m_pValueProperty;
};


class _XTP_EXT_CLASS CXTPMarkupSetterColection : public CXTPMarkupCollection
{
	DECLARE_MARKUPCLASS(CXTPMarkupSetterColection);

public:
	CXTPMarkupSetterColection();

public:
	CXTPMarkupSetter* GetItem(int nIndex) const;
};

class _XTP_EXT_CLASS CXTPMarkupTrigger : public CXTPMarkupObject
{
	DECLARE_MARKUPCLASS(CXTPMarkupTrigger);
public:
	CXTPMarkupTrigger();
	CXTPMarkupTrigger(CXTPMarkupDependencyProperty* pProperty, CXTPMarkupObject* pValue);

public:
	CXTPMarkupSetterColection* GetSetters() const;
	void SetSetters(CXTPMarkupSetterColection* pSetters);

public:
	CXTPMarkupDependencyProperty* GetTriggerProperty() const;
	CXTPMarkupObject* GetTriggerValue() const;


public:
	static CXTPMarkupDependencyProperty* m_pSettersProperty;
	static CXTPMarkupDependencyProperty* m_pPropertyProperty;
	static CXTPMarkupDependencyProperty* m_pValueProperty;
};

class _XTP_EXT_CLASS CXTPMarkupTriggerCollection : public CXTPMarkupCollection
{
	DECLARE_MARKUPCLASS(CXTPMarkupTriggerCollection);
public:
	CXTPMarkupTriggerCollection();

public:
	CXTPMarkupTrigger* GetItem(int nIndex) const;

public:
};


class _XTP_EXT_CLASS CXTPMarkupStyle : public CXTPMarkupObject
{
	DECLARE_MARKUPCLASS(CXTPMarkupStyle);

public:
	CXTPMarkupStyle();
	~CXTPMarkupStyle();

public:
	void Seal();

	CXTPMarkupSetterColection* GetSetters() const;
	CXTPMarkupTriggerCollection* GetTriggers() const;
	void SetTriggers(CXTPMarkupTriggerCollection* pTriggers);

	CXTPMarkupStyle* GetBasedStyle() const;
	void SetBasedStyle(CXTPMarkupStyle* pStyle);

	CXTPMarkupObject* GetStyleValue(CXTPMarkupDependencyProperty* pProperty) const;

	CXTPMarkupObject* FindResource(const CXTPMarkupObject* pKey) const;

protected:
	void SetContentObject(CXTPMarkupBuilder* pBuilder, CXTPMarkupObject* pContent);
	void ResolveSetterProperty(CXTPMarkupBuilder* pBuilder, CXTPMarkupObject* pSetter);
	void ResolveTriggerProperty(CXTPMarkupBuilder* pBuilder, CXTPMarkupTrigger* pTrigger);
	void SetPropertyObject(CXTPMarkupBuilder* pBuilder, CXTPMarkupDependencyProperty* pProperty, CXTPMarkupObject* pValue);

protected:
	CXTPMarkupSetterColection* m_pSetters;
	BOOL m_bSealed;
	CXTPMarkupProperties* m_pProperties;

public:
	static CXTPMarkupDependencyProperty* m_pTargetTypeProperty;
	static CXTPMarkupDependencyProperty* m_pBasedOnProperty;
	static CXTPMarkupDependencyProperty* m_pResourcesProperty;
	static CXTPMarkupDependencyProperty* m_pTriggersProperty;
};

AFX_INLINE CXTPMarkupStyle* CXTPMarkupStyle::GetBasedStyle() const {
	return MARKUP_STATICCAST(CXTPMarkupStyle, GetValue(m_pBasedOnProperty));
}
AFX_INLINE void CXTPMarkupStyle::SetBasedStyle(CXTPMarkupStyle* pStyle) {
	SetValue(m_pBasedOnProperty, pStyle);
}
AFX_INLINE CXTPMarkupSetterColection* CXTPMarkupStyle::GetSetters() const {
	return m_pSetters;
}
AFX_INLINE CXTPMarkupSetter* CXTPMarkupSetterColection::GetItem(int nIndex) const {
	return nIndex >= 0 && nIndex < m_arrItems.GetSize() ? (CXTPMarkupSetter*)m_arrItems[nIndex] : NULL;
}
AFX_INLINE CXTPMarkupDependencyProperty* CXTPMarkupSetter::GetSetterProperty() const {
	return MARKUP_STATICCAST(CXTPMarkupDependencyProperty, GetValue(m_pPropertyProperty));
}
AFX_INLINE CXTPMarkupObject* CXTPMarkupSetter::GetSetterValue() const {
	return CXTPMarkupObject::GetValue(m_pValueProperty);
}
AFX_INLINE CXTPMarkupTriggerCollection* CXTPMarkupStyle::GetTriggers() const {
	return MARKUP_STATICCAST(CXTPMarkupTriggerCollection, GetValue(m_pTriggersProperty));
}
AFX_INLINE void CXTPMarkupStyle::SetTriggers(CXTPMarkupTriggerCollection* pTriggers) {
	SetValue(m_pTriggersProperty, pTriggers);
}

AFX_INLINE CXTPMarkupTrigger* CXTPMarkupTriggerCollection::GetItem(int nIndex) const {
	return nIndex >= 0 && nIndex < GetCount() ? (CXTPMarkupTrigger*)m_arrItems[nIndex] : NULL;
}
AFX_INLINE CXTPMarkupSetterColection* CXTPMarkupTrigger::GetSetters() const {
	return MARKUP_STATICCAST(CXTPMarkupSetterColection, GetValue(m_pSettersProperty));
}
AFX_INLINE  void CXTPMarkupTrigger::SetSetters(CXTPMarkupSetterColection* pSetters) {
	SetValue(m_pSettersProperty, pSetters);
}

AFX_INLINE CXTPMarkupDependencyProperty* CXTPMarkupTrigger::GetTriggerProperty() const {
	return MARKUP_STATICCAST(CXTPMarkupDependencyProperty, GetValue(m_pPropertyProperty));
}
AFX_INLINE CXTPMarkupObject* CXTPMarkupTrigger::GetTriggerValue() const {
	return CXTPMarkupObject::GetValue(m_pValueProperty);
}

#endif // !defined(__XTPMARKUPRESOURCEDICTIONARY_H__)
