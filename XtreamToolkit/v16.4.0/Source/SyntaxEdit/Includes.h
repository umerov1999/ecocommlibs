// Includes.h : header file
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

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPSYNTAXEDIT_INCLUDES_H__)
#define __XTPSYNTAXEDIT_INCLUDES_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Resource.h"

#include "XTPSyntaxEditDefines.h"
#include "XTPSyntaxEditStruct.h"

#include "XTPSyntaxEditTextIterator.h"
#include "XTPSyntaxEditFindReplaceDlg.h"
#include "XTPSyntaxEditCtrl.h"
#include "XTPSyntaxEditDoc.h"
#include "XTPSyntaxEditView.h"
#include "XTPSyntaxEditPaintManager.h"
#include "XTPSyntaxEditColorSampleText.h"
#include "XTPSyntaxEditColorComboBox.h"
#include "XTPSyntaxEditPropertiesPage.h"
#include "XTPSyntaxEditPropertiesDlg.h"
#include "XTPSyntaxEditGoToLineDlg.h"
#include "XTPSyntaxEditToolTipCtrl.h"
#include "XTPSyntaxEditAutoCompleteWnd.h"
#include "XTPSyntaxEditSelection.h"

#ifdef _XTP_INCLUDE_SYNTAXEDIT_LEXER

#include "XTPSyntaxEditUndoManager.h"
#include "XTPSyntaxEditLineMarksManager.h"
#include "XTPSyntaxEditLexPtrs.h"
#include "XTPSyntaxEditLexParser.h"
#include "XTPSyntaxEditBufferManager.h"

// Commented to save compiler heap
#include "XTPSyntaxEditSectionManager.h"
#include "XTPSyntaxEditLexClassSubObjT.h"
#include "XTPSyntaxEditTextIterator.h"
#include "XTPSyntaxEditLexCfgFileReader.h"
#include "XTPSyntaxEditLexClassSubObjDef.h"
#include "XTPSyntaxEditLexClass.h"
#include "XTPSyntaxEditLexColorFileReader.h"

using namespace XTPSyntaxEditLexAnalyser;
#endif

#endif // !defined(__XTPSYNTAXEDIT_INCLUDES_H__)
//}}AFX_CODEJOCK_PRIVATE
