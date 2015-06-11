//////////////////////////////////////////////////////////////////////
// XTPNumberFormat.h: interface for the CXTPNumberFormat class.
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
#if !defined(__XTPNUMBERFORMAT_H__)
#define __XTPNUMBERFORMAT_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class _XTP_EXT_CLASS CXTPNumberFormat : public CXTPCmdTarget
{
public:
	CXTPNumberFormat();
	virtual ~CXTPNumberFormat();

protected:
private:

	enum
	{
		BufferSize = 16
	};

   /**
	  Buffer for the decimal separator string where
	  NUMBERFMT::lpDecimalSep will point to.
   */

	CString m_sDecimalSep;
   /**
	  Buffer for the thousand separator string where
	  NUMBERFMT::lpThousandSep will point to.
   */
   CString m_sThousandSep;

	/**
		Number format.
	*/
	NUMBERFMT m_numberFormat;

private:

	BSTR OleGetDecimalSeperator();
};

#endif // !defined(__XTPNUMBERFORMAT_H__)
