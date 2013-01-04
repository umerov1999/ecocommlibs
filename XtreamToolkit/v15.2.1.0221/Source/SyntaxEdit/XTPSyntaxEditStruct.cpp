// XTPSyntaxEditStruct.cpp
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// (c)1998-2012 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME SYNTAX EDIT LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"


// common includes
#include "Common/XTPVC80Helpers.h"
#include "Common/XTPColorManager.h"

// syntax editor includes
#include "XTPSyntaxEditDefines.h"
#include "XTPSyntaxEditStruct.h"
#include "XTPSyntaxEditCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//===========================================================================
// XTP_EDIT_LINECOL
//===========================================================================
const XTP_EDIT_LINECOL XTP_EDIT_LINECOL::MAXPOS = {INT_MAX, INT_MAX};
const XTP_EDIT_LINECOL XTP_EDIT_LINECOL::MINPOS = {0, 0};
const XTP_EDIT_LINECOL XTP_EDIT_LINECOL::Pos1 = {1, 0};

const XTP_EDIT_LINECOL XTP_EDIT_LINECOL::MakeLineCol(int nParamLine, int nParamCol)
{
	XTP_EDIT_LINECOL ret;
	ret.nLine = nParamLine;
	ret.nCol = nParamCol;
	return ret;
}

const XTP_EDIT_LINECOL XTP_EDIT_LINECOL::min2(const XTP_EDIT_LINECOL& pos1, const XTP_EDIT_LINECOL& pos2)
{
	XTP_EDIT_LINECOL lcPos;
	lcPos.nLine = min(pos1.nLine, pos2.nLine);
	lcPos.nCol = min(pos1.nCol, pos2.nCol);
	return lcPos;
}

const XTP_EDIT_LINECOL XTP_EDIT_LINECOL::max2(const XTP_EDIT_LINECOL& pos1, const XTP_EDIT_LINECOL& pos2)
{
	XTP_EDIT_LINECOL lcPos;
	lcPos.nLine = max(pos1.nLine, pos2.nLine);
	lcPos.nCol = max(pos1.nCol, pos2.nCol);
	return lcPos;
}

DWORD XTP_EDIT_LINECOL::GetXLC() const
{
	return XTP_EDIT_XLC(nLine, nCol);
}

BOOL XTP_EDIT_LINECOL::IsValidData() const
{
	return nLine > 0;
}

void XTP_EDIT_LINECOL::Clear()
{
	nLine = 0;
	nCol = 0;
}

BOOL XTP_EDIT_LINECOL::operator < (const XTP_EDIT_LINECOL& pos2) const
{
	return  nLine < pos2.nLine ||
		nLine == pos2.nLine && nCol < pos2.nCol;
}

BOOL XTP_EDIT_LINECOL::operator <= (const XTP_EDIT_LINECOL& pos2) const
{
	return  nLine < pos2.nLine ||
		nLine == pos2.nLine && nCol <= pos2.nCol;
}

BOOL XTP_EDIT_LINECOL::operator > (const XTP_EDIT_LINECOL& pos2) const
{
	return  nLine > pos2.nLine ||
		nLine == pos2.nLine && nCol > pos2.nCol;
}

BOOL XTP_EDIT_LINECOL::operator >= (const XTP_EDIT_LINECOL& pos2) const
{
	return  nLine > pos2.nLine ||
		nLine == pos2.nLine && nCol >= pos2.nCol;
}

BOOL XTP_EDIT_LINECOL::operator == (const XTP_EDIT_LINECOL& pos2) const
{
	return  nLine == pos2.nLine && nCol == pos2.nCol;
}

//-----------------------------------------------------------------------
// Summary:
//     This member function is used to Get Back Color Ex
// Parameters:
//     pEditCtrl - pointer to CXTPSyntaxEditCtrl object
//-----------------------------------------------------------------------

COLORREF XTP_EDIT_COLORVALUES::GetBackColorEx(CXTPSyntaxEditCtrl* pEditCtrl)
{
	if (pEditCtrl && pEditCtrl->IsReadOnly() && !pEditCtrl->IsViewOnly())
		return (COLORREF)crReadOnlyBack;

	return (COLORREF)crBack;
}


//===========================================================================
// XTP_EDIT_FONTOPTIONS
//===========================================================================
XTP_EDIT_FONTOPTIONS::XTP_EDIT_FONTOPTIONS()
{
	lfHeight            = XTP_EDIT_FONTOPTIONS_UNSPEC_OPTION;
	lfWidth             = XTP_EDIT_FONTOPTIONS_UNSPEC_OPTION;
	lfEscapement        = XTP_EDIT_FONTOPTIONS_UNSPEC_OPTION;
	lfOrientation       = XTP_EDIT_FONTOPTIONS_UNSPEC_OPTION;
	lfWeight            = XTP_EDIT_FONTOPTIONS_UNSPEC_OPTION;
	lfItalic            = XTP_EDIT_FONTOPTIONS_UNSPEC_OPTION;
	lfUnderline         = XTP_EDIT_FONTOPTIONS_UNSPEC_OPTION;
	lfStrikeOut         = XTP_EDIT_FONTOPTIONS_UNSPEC_OPTION;
	lfCharSet           = XTP_EDIT_FONTOPTIONS_UNSPEC_OPTION;
	lfOutPrecision      = XTP_EDIT_FONTOPTIONS_UNSPEC_OPTION;
	lfClipPrecision     = XTP_EDIT_FONTOPTIONS_UNSPEC_OPTION;
	lfQuality           = XTP_EDIT_FONTOPTIONS_UNSPEC_OPTION;
	lfPitchAndFamily    = XTP_EDIT_FONTOPTIONS_UNSPEC_OPTION;
	lfFaceName[0]       = '\0';
}

//===========================================================================
// XTP_EDIT_TEXTBLOCK
//===========================================================================

// Default constructor
XTP_EDIT_TEXTBLOCK::XTP_EDIT_TEXTBLOCK()
{
	nPos = 0;
	nNextBlockPos = 0;
}
