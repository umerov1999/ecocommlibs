// XTPSize.h
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
#if !defined(__XTPSIZE_H__)
#define __XTPSIZE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <Common/XTPMathUtils.h>

//===========================================================================
// Summary:
//     This class incapsulates size data.
// Parameters:
//     T - type of a single size coordinate component. Must be an integral type.
//===========================================================================
template<class T>
class CXTPSizeT
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs an empty CXTPSizeT object with all components equal to 0.
	//-----------------------------------------------------------------------
	CXTPSizeT()
		: Width(T(0))
		, Height(T(0))
	{
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPSizeT object.
	// Parameters:
	//     width - The Width component.
	//     height - The Height component.
	//-----------------------------------------------------------------------
	CXTPSizeT(T width, T height)
		: Width(width)
		, Height(height)
	{
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPSizeT object from another CXTPSizeT object.
	//     conversion is performed.
	// Parameters:
	//     other - A reference to a source size object.
	//-----------------------------------------------------------------------
	CXTPSizeT(const CXTPSizeT& other)
		: Width(other.Width)
		, Height(other.Height)
	{
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Performs addition of two size objects.
	// Parameters:
	//     other - A reference to a size object to add to the current size object.
	// Returns:
	//     A new size object width added size components.
	//-----------------------------------------------------------------------
	CXTPSizeT operator + (const CXTPSizeT& other) const
	{
		return CXTPSizeT(Width + other.Width, Height + other.Height);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Performs subtraction of two size objects.
	// Parameters:
	//     other - A reference to a size object to subtract from the current size object.
	// Returns:
	//     A new size object width size components after subtraction.
	//-----------------------------------------------------------------------
	CXTPSizeT operator - (const CXTPSizeT& other) const
	{
		return CXTPSizeT(Width - other.Width, Height - other.Height);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Compares two sizes.
	// Parameters:
	//     other - size object reference to compare to.
	// Returns:
	//     true if sizes are equal, otherwise the return value is false.
	//-----------------------------------------------------------------------
	bool operator == (const CXTPSizeT& other) const
	{
		return (&other == this)
			|| (Width == other.Width && Height == other.Height);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Compares two sizes.
	// Parameters:
	//     other - size object reference to compare to.
	// Returns:
	//     true if sizes are not equal, otherwise the return value is false.
	//-----------------------------------------------------------------------
	bool operator != (const CXTPSizeT& other) const
	{
		return !(*this == other);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the size object is empty.
	// Returns:
	//     true if the size object is empty, i.e. each component is equal to 0.
	//     false otherwise.
	//-----------------------------------------------------------------------
	bool IsEmpty() const
	{
		return (T(0) == Width && T(0) == Height);
	}

	template<class U>
	operator CXTPSizeT<U>() const
	{
		return CXTPSizeT<U>(U(Width), U(Height));
	}

public:
	T Width;   // The Width component
	T Height;  // The Height component
};

// CXTPSizeT shortcut for integer size object.
typedef CXTPSizeT<int> CXTPSizeI;

// CXTPSizeT shortcut for float size object.
typedef CXTPSizeT<float> CXTPSizeF;

// CXTPSizeT shortcut for double size object.
typedef CXTPSizeT<double> CXTPSizeD;

//{{AFX_CODEJOCK_PRIVATE
#endif // !defined(__XTPSIZE_H__)
//}}AFX_CODEJOCK_PRIVATE
