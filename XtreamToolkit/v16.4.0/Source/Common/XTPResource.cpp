// XTPResource.cpp: implementation of the CXTPResource class.
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

#include "StdAfx.h"
#include <Common/XTPResource.h>

CXTPResource::CXTPResource()
	: m_hResource     (NULL)
	, m_hResourceData (NULL)
	, m_pvResourceData(NULL)
	, m_dwSize        (0)
{
}

CXTPResource::~CXTPResource()
{
}

BOOL CXTPResource::LoadResource(HMODULE hInstance, LPCTSTR pszName, LPCTSTR pszType)
{
   //
   m_hResource = ::FindResource(hInstance, pszName, pszType);

   // Resource not found
   if (NULL == m_hResource)
	  return FALSE;

   // Get the size of the resource
   m_dwSize = ::SizeofResource(hInstance, m_hResource);

   // Resource does not contain any data
   if (0 == m_dwSize)
	  return FALSE;

   // Load the resource
   m_hResourceData = ::LoadResource(hInstance, m_hResource);

   // Can't load resource
   if (NULL == m_hResourceData)
	  return FALSE;

   // Lock the resource
   m_pvResourceData = ::LockResource(m_hResourceData);

   // Can't lock resource
   if (NULL == m_pvResourceData)
	  return FALSE;

   return TRUE;
}
