// XTPFlowGraphTools.h
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
#if !defined(__XTPFLOWGRAPHTOOLS_H__)
#define __XTPFLOWGRAPHTOOLS_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//{{AFX_CODEJOCK_PRIVATE
template<typename Traits>
class CXTPFlowGraphGDIHandles : public Traits
{
public:
	typedef typename Traits::FromType FromType;
	typedef typename Traits::ToType ToType;

	// ------------------------------------------------
	// Summary:
	//     Constructs a CXTPFlowGraphGDIHandles object.
	// ------------------------------------------------
	CXTPFlowGraphGDIHandles()
	{
		m_nCount = 0;
	}

	// ------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPFlowGraphGDIHandles object, handles cleanup and
	//     deallocation.
	// ------------------------------------------------------------------
	~CXTPFlowGraphGDIHandles()
	{
		for (int i = 0; i < m_nCount; i++)
		{
			DestroyHandle(m_arr[i].hTo);
		}
	}

	ToType GetHandle(const FromType& hFrom)
	{
		int i;

		for (i = 0; i < m_nCount; i++)
		{
			if (CompareHandle(m_arr[i].hFrom, hFrom))
				return m_arr[i].hTo;
		}

		if (m_nCount == MAX_HANDLE)
		{
			DestroyHandle(m_arr[m_nCount - 1].hTo);
		}
		else if (m_nCount)
		{
			m_arr[m_nCount] = m_arr[m_nCount - 1];
		}

		for (i = m_nCount - 1; i >= 1; i--)
		{
			m_arr[i] = m_arr[i - 1];
		}

		if (m_nCount != MAX_HANDLE)
			m_nCount++;

		m_arr[0].hFrom = hFrom;
		m_arr[0].hTo = CreateHandle(hFrom);

		return m_arr[0].hTo;
	}
	struct GDIREC
	{
		FromType hFrom;
		ToType hTo;
	};

	enum
	{
		MAX_HANDLE = 6
	};

	int m_nCount;
	GDIREC m_arr[MAX_HANDLE];
};

struct CXTPFlowGraphFontTraits
{
	typedef LOGFONT FromType;
	typedef HFONT ToType;

	void DestroyHandle(HFONT hFont) {
		DeleteObject(hFont);
	}
	HFONT CreateHandle(const LOGFONT& lf) {
		return CreateFontIndirect(&lf);
	}
	BOOL CompareHandle(const LOGFONT& lfLeft, const LOGFONT& lfRight) {
		if (lfLeft.lfHeight != lfRight.lfHeight)
			return FALSE;

		if (lfLeft.lfWeight != lfRight.lfWeight)
			return FALSE;

		if (lfLeft.lfItalic != lfRight.lfItalic)
			return FALSE;

		if (lfLeft.lfQuality !=lfRight.lfQuality)
			return FALSE;

		if (_tcscmp(lfLeft.lfFaceName,lfRight.lfFaceName) != 0)
			return FALSE;

		return TRUE;
	}
};

struct CXTPFlowGraphBrushTraits
{
	typedef COLORREF FromType;
	typedef HBRUSH ToType;

	void DestroyHandle(HBRUSH hFont) {
		DeleteObject(hFont);
	}
	HBRUSH CreateHandle(COLORREF clr) {
		return CreateSolidBrush(clr);
	}
	BOOL CompareHandle(COLORREF clrLeft, COLORREF clrRight) {
		return clrLeft == clrRight;
	}
};
//}}AFX_CODEJOCK_PRIVATE


#endif //#if !defined(__XTPFLOWGRAPHTOOLS_H__)
