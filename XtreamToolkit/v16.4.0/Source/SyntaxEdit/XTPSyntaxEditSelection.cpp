// XTPSyntaxEditSelection.cpp: implementation of the CXTPSyntaxEditAutoCompleteWnd class.
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// (c)1998-2013 Codejock Software, All Rights Reserved.
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
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPColorManager.h"

// syntax editor includes
#include "XTPSyntaxEditDefines.h"
#include "XTPSyntaxEditStruct.h"
#include "XTPSyntaxEditSelection.h"

#include "XTPSyntaxEditBufferManager.h"
#include "XTPSyntaxEditCtrl.h"
#include "XTPSyntaxEditDrawTextProcessor.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CXTPSyntaxEditSelection, CXTPCmdTarget)



// class CXTPSyntaxEditSelection =======================================================
CXTPSyntaxEditSelection::CXTPSyntaxEditSelection()
{
	m_pOwnerCtrl = NULL;

	selStart_disp = selEnd_disp = XTP_EDIT_LINECOL::MakeLineCol(1, 1);
	selStart_str = selEnd_str = XTP_EDIT_LINECOL::MakeLineCol(1, 0);

	bSelectingRunning = FALSE;
	bWordSelectionMode = FALSE;
	bBlockSelectionMode = FALSE;
	nSelStartTextRowFromLeftBar = 0;

#ifdef _XTP_ACTIVEX
	m_pAxStart = NULL;
	m_pAxEnd = NULL;
	m_pAxNormalStart = NULL;
	m_pAxNormalEnd = NULL;

	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPSyntaxEditSelection::CXTPSyntaxEditSelection(const CXTPSyntaxEditSelection& rSrc)
{
#ifdef _XTP_ACTIVEX
	m_pAxStart = NULL;
	m_pAxEnd = NULL;
	m_pAxNormalStart = NULL;
	m_pAxNormalEnd = NULL;

	EnableAutomation();
	EnableTypeLib();
#endif

	m_pOwnerCtrl = NULL;

	*this = rSrc;
}

#define XTP_SE_AX_DETACH(pObject)   if (pObject) { \
										pObject->Detach(); \
										pObject->InternalRelease(); }

CXTPSyntaxEditSelection::~CXTPSyntaxEditSelection()
{
#ifdef _XTP_ACTIVEX

	XTP_SE_AX_DETACH(m_pAxStart);
	XTP_SE_AX_DETACH(m_pAxEnd);
	XTP_SE_AX_DETACH(m_pAxNormalStart);
	XTP_SE_AX_DETACH(m_pAxNormalEnd);
#endif
}

void CXTPSyntaxEditSelection::SetStart_str(int nTextRow, int nStrPos)
{
	selStart_str.nLine = nTextRow;
	selStart_str.nCol = nStrPos;

	selStart_disp.nLine = nTextRow;

	if (m_pOwnerCtrl && m_pOwnerCtrl->GetEditBuffer())
	{
		selStart_disp.nCol = m_pOwnerCtrl->GetEditBuffer()->StrPosToCol(nTextRow, nStrPos);
	}
	else
	{
		ASSERT(FALSE);
		selStart_disp.nCol = nStrPos;
	}
}

void CXTPSyntaxEditSelection::SetEnd_str(int nTextRow, int nStrPos)
{
	selEnd_str.nLine = nTextRow;
	selEnd_str.nCol = nStrPos;

	selEnd_disp.nLine = nTextRow;

	if (m_pOwnerCtrl && m_pOwnerCtrl->GetEditBuffer())
	{
		selEnd_disp.nCol = m_pOwnerCtrl->GetEditBuffer()->StrPosToCol(nTextRow, nStrPos);
	}
	else
	{
		ASSERT(FALSE);
		selEnd_disp.nCol = nStrPos;
	}
}

void CXTPSyntaxEditSelection::SetStart_disp(int nTextRow, int nDispCol)
{
	ASSERT(nDispCol > 0);
	nDispCol = max(1, nDispCol);

	selStart_disp.nLine = nTextRow;
	selStart_disp.nCol = nDispCol;

	selStart_str.nLine = nTextRow;

	if (m_pOwnerCtrl && m_pOwnerCtrl->GetEditBuffer())
	{
		selStart_str.nCol = m_pOwnerCtrl->GetEditBuffer()->ColToStrPos(nTextRow, nDispCol);
	}
	else
	{
		ASSERT(FALSE);
		selStart_str.nCol = nDispCol;
	}
}

void CXTPSyntaxEditSelection::SetEnd_disp(int nTextRow, int nDispCol)
{
	ASSERT(nDispCol > 0);
	nDispCol = max(1, nDispCol);

	selEnd_disp.nLine = nTextRow;
	selEnd_disp.nCol = nDispCol;

	selEnd_str.nLine = nTextRow;

	if (m_pOwnerCtrl && m_pOwnerCtrl->GetEditBuffer())
	{
		selEnd_str.nCol = m_pOwnerCtrl->GetEditBuffer()->ColToStrPos(nTextRow, nDispCol);
	}
	else
	{
		ASSERT(FALSE);
		selEnd_str.nCol = nDispCol;
	}
}

BOOL CXTPSyntaxEditSelection::IsSelExist() const
{
	XTP_EDIT_LINECOL normStart = GetNormalStart_disp();
	XTP_EDIT_LINECOL normEnd = GetNormalEnd_disp();

	return normStart < normEnd;
}

BOOL CXTPSyntaxEditSelection::_IsInSel(BOOL bStr, int nTextRow, int nColX) const
{
	XTP_EDIT_LINECOL normStart = bStr ? GetNormalStart_str() : GetNormalStart_disp();
	XTP_EDIT_LINECOL normEnd = bStr ? GetNormalEnd_str() : GetNormalEnd_disp();

	if (!(normStart < normEnd))
		return FALSE;

	XTP_EDIT_LINECOL pos = XTP_EDIT_LINECOL::MakeLineCol(nTextRow, nColX);

	if (bBlockSelectionMode)
	{
		return pos.nLine >= normStart.nLine && pos.nLine < normEnd.nLine &&
			pos.nCol >= normStart.nCol && pos.nCol < normEnd.nCol;
	}
	return pos >= normStart && pos < normEnd;
}

BOOL CXTPSyntaxEditSelection::_IsIntersectSel(BOOL bStr, int nTextRow, int nCol1, int nCol2) const
{
	XTP_EDIT_LINECOL normStart = bStr ? GetNormalStart_str() : GetNormalStart_disp();
	XTP_EDIT_LINECOL normEnd = bStr ? GetNormalEnd_str() : GetNormalEnd_disp();

	if (!(normStart < normEnd))
		return FALSE;

	if (nTextRow != normEnd.nLine)
	{
		// infinit sel by end
		if (m_pOwnerCtrl->_IsVirtualSpaceActive())
		{
			normEnd.nCol = INT_MAX;
		}
		else
		{
			int nTextLenC = m_pOwnerCtrl->GetEditBuffer()->GetLineTextLengthC(nTextRow);
			if (bStr)
				normEnd.nCol = nTextLenC;
			else
				normEnd.nCol = m_pOwnerCtrl->GetEditBuffer()->StrPosToCol(nTextRow, nTextLenC);
		}
	}

	XTP_EDIT_LINECOL pos1 = XTP_EDIT_LINECOL::MakeLineCol(nTextRow, nCol1);
	XTP_EDIT_LINECOL pos2 = XTP_EDIT_LINECOL::MakeLineCol(nTextRow, nCol2);

	BOOL bIntersect = !(pos2 <= normStart || pos1 >= normEnd);

	if (bIntersect && bBlockSelectionMode)
	{
		// just condition by cols only
		bIntersect = !(pos2.nCol <= normStart.nCol || pos1.nCol >= normEnd.nCol);
	}

	return bIntersect;
}

int CXTPSyntaxEditSelection::GetSelStartForRow_str(int nTextRow, int nDispLine)
{
	XTP_EDIT_LINECOL normStart = GetNormalStart_str();
	XTP_EDIT_LINECOL normEnd = GetNormalEnd_str();

	if (nTextRow >= normStart.nLine && nTextRow <= normEnd.nLine)
	{
		if (bBlockSelectionMode)
		{
			int nDispPos = GetNormalStart_disp().nCol;
			ASSERT(m_pOwnerCtrl);
			if (m_pOwnerCtrl)
				return m_pOwnerCtrl->GetDrawTextProcessor().DispPosToStrPos(nDispLine, nDispPos - 1, TRUE);
				//m_pOwnerCtrl->_IsVirtualSpaceActive());

			return normStart.nCol;
		}
		else
		{
			if (nTextRow == normStart.nLine)
				return normStart.nCol;
			else
				return 0;
		}
	}

	return 0;
}

int CXTPSyntaxEditSelection::GetSelEndForRow_str(int nTextRow, int nDispLine, BOOL* pbInfinitSelEnd)
{
	if (pbInfinitSelEnd)
		*pbInfinitSelEnd = FALSE;


	ASSERT(m_pOwnerCtrl);
	if (!m_pOwnerCtrl)
		return 0;

	XTP_EDIT_LINECOL normStart = GetNormalStart_str();
	XTP_EDIT_LINECOL normEnd = GetNormalEnd_str();

	if (nTextRow >= normStart.nLine && nTextRow <= normEnd.nLine)
	{
		if (bBlockSelectionMode)
		{
			int nDispPos = GetNormalEnd_disp().nCol;
			return m_pOwnerCtrl->GetDrawTextProcessor().DispPosToStrPos(nDispLine, nDispPos - 1, TRUE);
				//m_pOwnerCtrl->_IsVirtualSpaceActive());
		}
		else
		{
			if (nTextRow == normEnd.nLine)
			{
				if (pbInfinitSelEnd && nSelStartTextRowFromLeftBar)
					*pbInfinitSelEnd = m_pOwnerCtrl->_IsVirtualSpaceActive();

				return normEnd.nCol;
			}
			else
			{
				int nDispPos = GetNormalEnd_disp().nCol;

				int nPos = m_pOwnerCtrl->GetDrawTextProcessor().DispPosToStrPos(nDispLine, nDispPos - 1,
						m_pOwnerCtrl->_IsVirtualSpaceActive());
				if (pbInfinitSelEnd)
					*pbInfinitSelEnd = m_pOwnerCtrl->_IsVirtualSpaceActive();

				int nRowTextLen = m_pOwnerCtrl->GetEditBuffer()->GetLineTextLengthC(nTextRow);
				nPos = max(nPos, nRowTextLen + 1);

				return nPos;
			}
		}
	}

	return 0;
}

void CXTPSyntaxEditSelection::Reset_str(int nTextRow, int nStrPos)
{
	bSelectingRunning = FALSE;
	bWordSelectionMode = FALSE;
	bBlockSelectionMode = FALSE;
	nSelStartTextRowFromLeftBar = 0;

	SetStart_str(nTextRow, nStrPos);
	SetEnd_str(nTextRow, nStrPos);
}

void CXTPSyntaxEditSelection::Reset_disp(int nTextRow, int nDispCol)
{
	bSelectingRunning = FALSE;
	bWordSelectionMode = FALSE;
	bBlockSelectionMode = FALSE;
	nSelStartTextRowFromLeftBar = 0;

	SetStart_disp(nTextRow, nDispCol);
	SetEnd_disp(nTextRow, nDispCol);
}

const CXTPSyntaxEditSelection& CXTPSyntaxEditSelection::operator=(const CXTPSyntaxEditSelection& rSrc)
{
	if (!m_pOwnerCtrl)
		m_pOwnerCtrl = rSrc.m_pOwnerCtrl;

	bSelectingRunning = rSrc.bSelectingRunning;
	bWordSelectionMode = rSrc.bWordSelectionMode;
	bBlockSelectionMode = rSrc.bBlockSelectionMode;
	nSelStartTextRowFromLeftBar = rSrc.nSelStartTextRowFromLeftBar;

	selStart_disp = rSrc.selStart_disp;
	selEnd_disp = rSrc.selEnd_disp;

	selStart_str = rSrc.selStart_str;
	selEnd_str = rSrc.selEnd_str;

	return *this;
}

BOOL CXTPSyntaxEditSelection::operator==(const CXTPSyntaxEditSelection& rSrc) const
{
	return  bSelectingRunning == rSrc.bSelectingRunning &&
		bWordSelectionMode == rSrc.bWordSelectionMode &&
		bBlockSelectionMode == rSrc.bBlockSelectionMode &&
		nSelStartTextRowFromLeftBar == rSrc.nSelStartTextRowFromLeftBar &&

		selStart_disp == rSrc.selStart_disp &&
		selEnd_disp == rSrc.selEnd_disp &&

		selStart_str == rSrc.selStart_str &&
		selEnd_str == rSrc.selEnd_str;
}

BOOL CXTPSyntaxEditSelection::operator!=(const CXTPSyntaxEditSelection& rSrc) const
{
	return !operator==(rSrc);
}

#ifdef _XTP_ACTIVEX

/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CXTPSyntaxEditTextPosition_imp, CCmdTarget)

// {257C9FDC-9B37-4919-9853-F8C71AC13170}
static const GUID IID_ISyntaxEditTextPosition =
{ 0x257c9fdc, 0x9b37, 0x4919, { 0x98, 0x53, 0xf8, 0xc7, 0x1a, 0xc1, 0x31, 0x70 } };

BEGIN_INTERFACE_MAP(CXTPSyntaxEditTextPosition_imp, CCmdTarget)
	INTERFACE_PART(CXTPSyntaxEditTextPosition_imp, IID_ISyntaxEditTextPosition, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPSyntaxEditTextPosition_imp, IID_ISyntaxEditTextPosition)

BEGIN_DISPATCH_MAP(CXTPSyntaxEditTextPosition_imp, CCmdTarget)
	DISP_PROPERTY_EX_ID(CXTPSyntaxEditTextPosition_imp, "Row", 1, OleGetRow, OleSetRow, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPSyntaxEditTextPosition_imp, "Col", 2, OleGetCol, OleSetCol, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPSyntaxEditTextPosition_imp, "StrPos", 3, OleGetStrPos, OleSetStrPos, VT_I4)

	DISP_FUNCTION_ID(CXTPSyntaxEditTextPosition_imp, "SetPos", 10, Ole_SetPos, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CXTPSyntaxEditTextPosition_imp, "SetPosStr", 11, Ole_SetPosStr, VT_EMPTY, VTS_I4 VTS_I4)
END_DISPATCH_MAP()

CXTPSyntaxEditTextPosition_imp::CXTPSyntaxEditTextPosition_imp()
{
	EnableAutomation();
	EnableTypeLib();
}

/////////////////////////////////////////////////////////////////////////////
CXTPSyntaxEditSelPosEx_imp::CXTPSyntaxEditSelPosEx_imp(CXTPSyntaxEditSelection* pOwner, BOOL bUseStart)
{
	ASSERT(pOwner);
	m_pOwner = pOwner;
	m_bUseStart = bUseStart;
}

long CXTPSyntaxEditSelPosEx_imp::OleGetRow()
{
	ASSERT(m_pOwner);

	if (m_pOwner)
	{
		if (m_bUseStart)
		{
			return m_pOwner->GetStart_str().nLine;
		}
		else
		{
			return m_pOwner->GetEnd_str().nLine;
		}
	}
	return 0;
}

long CXTPSyntaxEditSelPosEx_imp::OleGetCol()
{
	ASSERT(m_pOwner);

	if (m_pOwner)
	{
		if (m_bUseStart)
		{
			return m_pOwner->GetStart_disp().nCol;
		}
		else
		{
			return m_pOwner->GetEnd_disp().nCol;
		}
	}
	return 0;
}

long CXTPSyntaxEditSelPosEx_imp::OleGetStrPos()
{
	ASSERT(m_pOwner);

	if (m_pOwner)
	{
		if (m_bUseStart)
		{
			return m_pOwner->GetStart_str().nCol;
		}
		else
		{
			return m_pOwner->GetEnd_str().nCol;
		}
	}
	return 0;
}

void CXTPSyntaxEditSelPosEx_imp::Ole_SetPos(long nRow, long nCol)
{
	ASSERT(m_pOwner);

	if (m_pOwner)
	{
		if (m_bUseStart)
		{
			m_pOwner->SetStart_disp(nRow, nCol);
		}
		else
		{
			m_pOwner->SetEnd_disp(nRow, nCol);
		}
	}
}

void CXTPSyntaxEditSelPosEx_imp::Ole_SetPosStr(long nRow, long nStrPos)
{
	ASSERT(m_pOwner);

	if (m_pOwner)
	{
		if (m_bUseStart)
		{
			m_pOwner->SetStart_str(nRow, nStrPos);
		}
		else
		{
			m_pOwner->SetEnd_str(nRow, nStrPos);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
//CXTPSyntaxEditSelPosExNormal_imp
CXTPSyntaxEditSelPosExNormal_imp::CXTPSyntaxEditSelPosExNormal_imp(CXTPSyntaxEditSelection* pOwner, BOOL bUseStart) :
	CXTPSyntaxEditSelPosEx_imp(pOwner, bUseStart)
{
}

long CXTPSyntaxEditSelPosExNormal_imp::OleGetRow()
{
	ASSERT(m_pOwner);

	if (m_pOwner)
	{
		if (m_bUseStart)
		{
			return m_pOwner->GetNormalStart_str().nLine;
		}
		else
		{
			return m_pOwner->GetNormalEnd_str().nLine;
		}
	}
	return 0;
}

long CXTPSyntaxEditSelPosExNormal_imp::OleGetCol()
{
	ASSERT(m_pOwner);

	if (m_pOwner)
	{
		if (m_bUseStart)
		{
			return m_pOwner->GetNormalStart_disp().nCol;
		}
		else
		{
			return m_pOwner->GetNormalEnd_disp().nCol;
		}
	}
	return 0;
}

long CXTPSyntaxEditSelPosExNormal_imp::OleGetStrPos()
{
	ASSERT(m_pOwner);

	if (m_pOwner)
	{
		if (m_bUseStart)
		{
			return m_pOwner->GetNormalStart_str().nCol;
		}
		else
		{
			return m_pOwner->GetNormalEnd_str().nCol;
		}
	}
	return 0;
}

void CXTPSyntaxEditSelPosExNormal_imp::Ole_SetPos(long /*nRow*/, long /*nCol*/)
{
	SetNotSupported();
}

void CXTPSyntaxEditSelPosExNormal_imp::Ole_SetPosStr(long /*nRow*/, long /*nStrPos*/)
{
	SetNotSupported();
}

//////////////////////////////////////////////////////////////////////////

// {5B480A66-1E28-4880-9D4E-97069B345899}
static const GUID IID_ISyntaxEditTextSelection =
{ 0x5b480a66, 0x1e28, 0x4880, { 0x9d, 0x4e, 0x97, 0x6, 0x9b, 0x34, 0x58, 0x99 } };

BEGIN_INTERFACE_MAP(CXTPSyntaxEditSelection, CCmdTarget)
	INTERFACE_PART(CXTPSyntaxEditSelection, IID_ISyntaxEditTextSelection, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPSyntaxEditSelection, IID_ISyntaxEditTextSelection)

BEGIN_DISPATCH_MAP(CXTPSyntaxEditSelection, CCmdTarget)
	DISP_PROPERTY_EX_ID(CXTPSyntaxEditSelection, "Text", 1, OleGetText, OleSetText, VT_BSTR)
	DISP_PROPERTY_ID(CXTPSyntaxEditSelection, "BlockSelectionMode", 2, bBlockSelectionMode, VT_BOOL)

	DISP_FUNCTION_ID(CXTPSyntaxEditSelection, "Start",  10, OleGetStart, VT_DISPATCH, VTS_NONE)
	DISP_FUNCTION_ID(CXTPSyntaxEditSelection, "End",    11, OleGetEnd,   VT_DISPATCH, VTS_NONE)
	DISP_FUNCTION_ID(CXTPSyntaxEditSelection, "NormalStart", 12, OleGetNormalStart, VT_DISPATCH, VTS_NONE)
	DISP_FUNCTION_ID(CXTPSyntaxEditSelection, "NormalEnd",   13, OleGetNormalEnd,   VT_DISPATCH, VTS_NONE)

	DISP_FUNCTION_ID(CXTPSyntaxEditSelection, "Reset",  14, OleReset, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()

BSTR CXTPSyntaxEditSelection::OleGetText()
{
	CString strText;

	ASSERT(m_pOwnerCtrl);
	if (m_pOwnerCtrl)
		m_pOwnerCtrl->GetSelectionText(strText);

	return strText.AllocSysString();
}

void CXTPSyntaxEditSelection::OleSetText(LPCTSTR pcszSelText)
{
	ASSERT(m_pOwnerCtrl);
	if (m_pOwnerCtrl)
		m_pOwnerCtrl->ReplaceSel(pcszSelText);
}

void CXTPSyntaxEditSelection::OleReset()
{
	ASSERT(m_pOwnerCtrl);
	if (m_pOwnerCtrl)
		m_pOwnerCtrl->Unselect();
}

LPDISPATCH CXTPSyntaxEditSelection::OleGetStart()
{
	if (!m_pAxStart)
		m_pAxStart = new CXTPSyntaxEditSelPosEx_imp(this, TRUE);

	if (m_pAxStart)
		return m_pAxStart->GetIDispatch(TRUE);

	return NULL;
}

LPDISPATCH CXTPSyntaxEditSelection::OleGetEnd()
{
	if (!m_pAxEnd)
		m_pAxEnd = new CXTPSyntaxEditSelPosEx_imp(this, FALSE);

	if (m_pAxEnd)
		return m_pAxEnd->GetIDispatch(TRUE);

	return NULL;
}

LPDISPATCH CXTPSyntaxEditSelection::OleGetNormalStart()
{
	if (!m_pAxNormalStart)
		m_pAxNormalStart = new CXTPSyntaxEditSelPosExNormal_imp(this, TRUE);

	if (m_pAxNormalStart)
		return m_pAxNormalStart->GetIDispatch(TRUE);

	return NULL;
}

LPDISPATCH CXTPSyntaxEditSelection::OleGetNormalEnd()
{
	if (!m_pAxNormalEnd)
		m_pAxNormalEnd = new CXTPSyntaxEditSelPosExNormal_imp(this, FALSE);

	if (m_pAxNormalEnd)
		return m_pAxNormalEnd->GetIDispatch(TRUE);

	return NULL;
}

#endif
