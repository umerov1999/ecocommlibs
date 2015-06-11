// XTPReportRecordItemConstraint.h: interface for the CXTPReportRecordItemConstraint class.
//
// This file is a part of the XTREME REPORTCONTROL MFC class library.
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
#if !defined(__XTPREPORTRECORDITEMCONSTRAINT_H__)
#define __XTPREPORTRECORDITEMCONSTRAINT_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Summary:
//     CXTPReportRecordItemConstraint is a CXTPCmdTarget derived class.
//     It represents a single item constraints.
//===========================================================================
class _XTP_EXT_CLASS CXTPReportRecordItemConstraint : public CXTPCmdTarget
{
public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridItemConstraint object.
	//-------------------------------------------------------------------------
	CXTPReportRecordItemConstraint();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the index of this constraint
	//     within the collection of constraints.
	// Returns:
	//     Index of this constraint.
	//-----------------------------------------------------------------------
	int GetIndex() const;

public:
	CString m_strConstraint;    // Caption text of constraint.  This is the
	                            // text displayed for this constraint.
	DWORD_PTR   m_dwData;       // The 32-bit value associated with the item.

protected:
	int m_nIndex;               // Index of constraint.

private:
#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPReportRecordItemConstraint);
//}}AFX_CODEJOCK_PRIVATE
#endif
	friend class CXTPReportRecordItemConstraints;
	friend class CXTPReportRecordItemEditOptions;
};

//===========================================================================
// Summary:
//     CXTPReportRecordItemConstraints is a CCmdTarget derived class. It represents the item
//     constraints collection.
//===========================================================================
class _XTP_EXT_CLASS CXTPReportRecordItemConstraints : public CXTPCmdTarget
{
public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPReportRecordItemConstraints object.
	//-------------------------------------------------------------------------
	CXTPReportRecordItemConstraints();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPropertyGridItemConstraints object, handles cleanup and deallocation
	//-------------------------------------------------------------------------
	~CXTPReportRecordItemConstraints();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine the total number of constraints in the list.
	// Returns:
	//     Returns the total number of constraints added to the ReportRecordItem and\or ReportColumn.
	//-----------------------------------------------------------------------
	int GetCount() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this member to remove all constraints from the list of constraints.
	//-------------------------------------------------------------------------
	void RemoveAll();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve a constraint at nIndex.
	// Parameters:
	//     nIndex - Position in constraint collection.
	// Returns:
	//     Pointer to the constraint at nIndex in the collection of constraints.
	//-----------------------------------------------------------------------
	CXTPReportRecordItemConstraint* GetAt(int nIndex) const;

protected:
	CArray<CXTPReportRecordItemConstraint*, CXTPReportRecordItemConstraint*> m_arrConstraints;  // Collection of constraints

private:
#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPReportRecordItemConstraints);

	int OleGetItemCount();
	afx_msg LPDISPATCH OleGetItem(long nIndex);
	afx_msg void OleAdd(LPCTSTR lpszCaption, long dwData);
	DECLARE_ENUM_VARIANT(CXTPReportRecordItemConstraints)

	enum
	{
		dispidCount = 1L,
		dispidAdd = 2L,
	};

//}}AFX_CODEJOCK_PRIVATE
#endif

	friend class CXTPReportRecordItemEditOptions;

};

#endif //#if !defined(__XTPREPORTRECORDITEMCONSTRAINT_H__)
