// XTPChartAxisCustomLabels.h
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
#if !defined(__XTPCHARTAXISCUSTOMLABELS_H__)
#define __XTPCHARTAXISCUSTOMLABELS_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartAxis;
class CXTPChartLineStyle;
class CXTPChartLineStyle;

//===========================================================================
// Summary:
//     CXTPChartAxisCustomLabel is CXTPChartElement derived class
//     this class represents a custom label text on Axis (CXTPChartAxis).
//
// Remarks:
//
//===========================================================================
class _XTP_EXT_CLASS CXTPChartAxisCustomLabel : public CXTPChartElement
{
	DECLARE_DYNCREATE(CXTPChartAxisCustomLabel)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartAxisCustomLabel object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartAxisCustomLabel();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartAxisCustomLabel object, handles cleanup
	//-------------------------------------------------------------------------
	virtual ~CXTPChartAxisCustomLabel();

public:


	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to set the text of the custom label.
	// Parameters:
	//     lpszTitle - The string value for the new text, its type is CXTPChartString.
	// Remarks:
	//-----------------------------------------------------------------------
	void SetText(const CXTPChartString& lpszTitle);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the text of the custom label
	// Returns:
	//     The string value for the item text, its type is CXTPChartString.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartString GetText() const;



	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to get the value of the constant line in the axis.
	// Returns:
	//     A string denoting the axis value.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartString GetAxisValue() const;
	double GetAxisValueInternal() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the value of the constant line in the axis.
	// Parameters:
	//     lpszValue - A string denoting the axis value.
	// Remarks:
	//-----------------------------------------------------------------------
	void SetAxisValue(LPCTSTR lpszValue);
	void SetAxisValue(double dValue);


public:
	void DoPropExchange(CXTPPropExchange* pPX);

protected:


#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartAxisCustomLabel);

	BSTR OleGetText();
	void OleSetText(LPCTSTR lpszText);
	VARIANT OleGetAxisValue();
	void OleSetAxisValue(const VARIANT& var);


//}}AFX_CODEJOCK_PRIVATE
#endif


protected:
	CXTPChartString m_strText;               //The text of the item.
	CXTPChartString m_strAxisValue;          //The axis value.
	double m_dAxisValue;

	friend class CXTPChartAxisCustomLabels;
};

//===========================================================================
// Summary:
//     This class encapsulates a collection of CXTPChartAxisCustomLabel objects.
// Remarks:
//
class _XTP_EXT_CLASS CXTPChartAxisCustomLabels : public CXTPChartElementCollection
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartAxisCustomLabels object.
	// Parameters:
	//     pAxis - The pointer to a CXTPChartAxis object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartAxisCustomLabels(CXTPChartAxis* pAxis);

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartAxisCustomLabels object, handles cleanup
	//-------------------------------------------------------------------------
	virtual ~CXTPChartAxisCustomLabels();


public:

	//-----------------------------------------------------------------------
	// Summary:
	//     This function a new custom label to the collection of labels.
	// Parameters:
	//     pCustomLabel - The pointer to a CXTPChartAxisCustomLabel object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartAxisCustomLabel* Add(CXTPChartAxisCustomLabel* pCustomLabel);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the label object at a particular
	//     index in the collection.
	// Parameters:
	//     nIndex - The zero based index of the of item in the collection.
	// Returns:
	//     The pointer to a CXTPChartAxisCustomLabel object, at the index selected.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartAxisCustomLabel* GetAt(int nIndex) const;

public:
	void DoPropExchange(CXTPPropExchange* pPX);

public:
#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPChartAxisCustomLabels);

	afx_msg long OleGetItemCount();
	afx_msg LPDISPATCH OleGetItem(long nIndex);
	DECLARE_ENUM_VARIANT(CXTPChartAxisCustomLabels)

	afx_msg LPDISPATCH OleAdd(LPCTSTR lpszName, const VARIANT& Value);
//}}AFX_CODEJOCK_PRIVATE
#endif

protected:
};


AFX_INLINE CXTPChartAxisCustomLabel* CXTPChartAxisCustomLabels::GetAt(int nIndex) const {
	return nIndex >= 0 && nIndex < m_arrElements.GetSize() ? (CXTPChartAxisCustomLabel*)m_arrElements.GetAt(nIndex) : NULL;
}


AFX_INLINE CXTPChartString CXTPChartAxisCustomLabel::GetAxisValue() const {
	return m_strAxisValue;
}
AFX_INLINE double CXTPChartAxisCustomLabel::GetAxisValueInternal() const {
	return m_dAxisValue;
}
AFX_INLINE void CXTPChartAxisCustomLabel::SetAxisValue(LPCTSTR lpszValue) {
	m_strAxisValue = lpszValue;
	OnChartChanged();
}
AFX_INLINE void CXTPChartAxisCustomLabel::SetAxisValue(double dValue) {
	m_dAxisValue = dValue;
	m_strAxisValue.Empty();
	OnChartChanged();
}
AFX_INLINE CXTPChartString CXTPChartAxisCustomLabel::GetText() const {
	return m_strText;
}

#endif //#if !defined(__XTPCHARTAXISCUSTOMLABELS_H__)
