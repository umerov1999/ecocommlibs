// XTPChartElement.h
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
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
#if !defined(__XTPCHARTELEMENT_H__)
#define __XTPCHARTELEMENT_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartFont;
class CXTPChartContent;
class CXTPChartElementView;
class CXTPPropExchange;
class CXTPChartDeviceContext;
class CXTPChartDeviceCommand;
class CXTPChartAppearance;

enum XTPChartUpdateOptions
{
	xtpChartUpdateView      = 1,    // Recreate View
	xtpChartUpdateLayout    = 2,    // Don't recreate View but call Calculate
	xtpChartUpdateRedraw    = 4,    // Rebuild Command Tree
};

#ifdef _XTP_ACTIVEX
typedef CXTPCmdTarget CXTPChartObject;
#else
typedef CObject CXTPChartObject;
#endif

//===========================================================================
// Summary:
//     This class abstracts a chart element, this class act as a base class for
//     all chart elements.
// Remarks:
//     This is an MFC class, which has capacity to route Windows command messages.
//     Each element has an owner element.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartElement : public CXTPChartObject
{
	DECLARE_DYNAMIC(CXTPChartElement);
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartElement object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartElement();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartElement object, handle cleanup.
	//-------------------------------------------------------------------------
	virtual ~CXTPChartElement();

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to decrease the usage count of the object.
	//-------------------------------------------------------------------------
	virtual void Release();

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get owner element.
	// Returns
	//     Returns parent CXTPChartElement.
	//-------------------------------------------------------------------------
	CXTPChartElement* GetOwner() const;

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this virtual function to trigger a re-painting, when there is
	//     a change in the intrinsic properties of the chart.
	//-------------------------------------------------------------------------
	virtual void OnChartChanged(XTPChartUpdateOptions updateOptions = xtpChartUpdateView);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this virtual function to get the chart content object.
	// Returns:
	//     A pointer to chart content object.
	// Remarks:
	//     A chart content object is an aggregate of various chart components
	//     like panels, titles, series, legend etc.
	//-------------------------------------------------------------------------
	CXTPChartContent* GetContent() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the the appearance of the chart.
	// Returns:
	//     A pointer to CXTPChartAppearance object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartAppearance* GetAppearance() const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Reads or writes this object from or to an archive.
	// Parameters:
	//     pPX - A CXTPPropExchange object to serialize to or from.
	//----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

public:

#ifdef _XTP_ACTIVEX

public:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPChartElement);
//}}AFX_CODEJOCK_PRIVATE
#else
	DWORD InternalAddRef();
	DWORD InternalRelease();

	long m_dwRef;
#endif

protected:
	CXTPChartElement* m_pOwner;  //Owner of the element.

	friend class CXTPChartElementCollection;
};

//===========================================================================
// Summary:
//     This class represents the chart container, this class act as a container
//     for the chart content.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartContainer
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartContainer object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartContainer();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartContainer object, handle cleanup.
	//-------------------------------------------------------------------------
	virtual ~CXTPChartContainer();

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to trigger a re-painting, when there is a change
	//     in the intrinsic properties of the chart.
	//-------------------------------------------------------------------------
	virtual void OnChartChanged(XTPChartUpdateOptions updateOptions = xtpChartUpdateView);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to set capture to View.
	// Parameters:
	//     pView - View that will receive all mouse messages.
	//-----------------------------------------------------------------------
	virtual void SetCapture(CXTPChartElementView* pView);

protected:
};

//===========================================================================
// Summary:
//     This class abstracts a chart text element, this class is a kind of
//     CXTPChartElement class.
// Remarks:
//     This class act as an abstract base class for various specialized chart text
//     elements like axis label, axis title etc.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartTextElement : public CXTPChartElement
{
	DECLARE_DYNAMIC(CXTPChartTextElement)
public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the font of the text element.
	// Returns:
	//     A pointer to a chart font object, which holds the current font of the
	//     text element.
	//-------------------------------------------------------------------------
	virtual CXTPChartFont* GetFont() const = 0;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get whether the antialiasing is enabled or not.
	// Returns:
	//     TRUE if the anti aliasing is enabled and FALSE if antialiasing is disabled.
	//     Anti aliasing is the process of smoothing the sharp edges in drawings.
	//-------------------------------------------------------------------------
	virtual BOOL GetAntialiasing() const = 0;
};


//===========================================================================
// Summary:
//     This class abstracts a chart collection, this class is a kind of
//     CXTPChartElement class.
// Remarks:
//     This class act as an abstract base class for collection objects such as collection of points, titles, panels, strips.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartElementCollection : public CXTPChartElement
{
	DECLARE_DYNAMIC(CXTPChartElementCollection)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartElementCollection object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartElementCollection();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartElementCollection object, handle cleanup.
	//-------------------------------------------------------------------------
	~CXTPChartElementCollection();

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     This function remove all the individual objects
	//     from this collection object.
	// Remarks:
	// See Also: RemoveAt
	//-------------------------------------------------------------------------
	virtual void RemoveAll();

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the number of objects in
	//     the internal array of this object.
	// Returns:
	//     The number of point elements present in this object.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	virtual int GetCount() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Use this function to remove object from the collection.
	// Parameters:
	//     nIndex - Index of object to remove
	//-------------------------------------------------------------------------
	virtual void RemoveAt(int nIndex);

	//-------------------------------------------------------------------------
	// Summary:
	//     Use this function to remove object from the collection.
	// Parameters:
	//     pElement - Object to remove
	//-------------------------------------------------------------------------
	virtual void Remove(CXTPChartElement* pElement);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get element by its index
	// Parameters:
	//     nIndex - Index of object to get
	// Returns:
	//     Element by its index
	//-------------------------------------------------------------------------
	CXTPChartElement* GetAt(int nIndex) const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get index of specified element
	// Parameters:
	//     pElement - Object to test
	// Returns:
	//     Index of pElement inside internal collection
	//-------------------------------------------------------------------------
	int IndexOf(CXTPChartElement* pElement) const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to add new element to collection
	// Parameters:
	//     pElement - Object to add
	//     nIndex - Index of object to add
	//-------------------------------------------------------------------------
	void InsertAt(int nIndex, CXTPChartElement* pElement);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to decrease the usage count of the object.
	//-------------------------------------------------------------------------
	virtual void Release();

protected:
	CArray<CXTPChartElement*, CXTPChartElement*> m_arrElements;       //Array of CXTPChartElement pointer.
};

AFX_INLINE CXTPChartElement* CXTPChartElement::GetOwner() const {
	return m_pOwner;
}

AFX_INLINE CXTPChartElement* CXTPChartElementCollection::GetAt(int nIndex) const {
	return nIndex >= 0 && nIndex < m_arrElements.GetSize() ? m_arrElements.GetAt(nIndex) : NULL;
}
AFX_INLINE int CXTPChartElementCollection::GetCount() const {
	return (int)m_arrElements.GetSize();
}

#endif //#if !defined(__XTPCHARTELEMENT_H__)
