// $Header: //depot/release/ironman2012/develop/global/inc/dbxsdk/opmexp.h#1 $ $Change: 237375 $ $DateTime: 2011/01/30 18:32:54 $ $Author: integrat $
#ifndef __OPMEXP_H_
// $NoKeywords: $
#define __OPMEXP_H_
//
// (C) Copyright 1998-2008 by Autodesk, Inc.
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted,
// provided that the above copyright notice appears in all copies and
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS.
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC.
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

#include "geassign.h"  //for asDblArray
#include "dbxutil.h"   //for acdbWcs2Ucs

#define BEGIN_ELEMENT_SETGET(theComClass, theArxClass)              \
    int theComClass::SetGetElementValue(int GetOrSet, DISPID dispid, DWORD cookie, VARIANT *val, AcDbObject *pObj) \
    {                                                               \
        theArxClass *TheArxClass;                                   \
                                                                    \
        if ((TheArxClass = theArxClass::cast(pObj)) == NULL)        \
            return 0;


#define ELEMENT_GET_ENTRY(DID, ACGEType, getelement, vtype)         \
        if (dispid == DID)                                          \
        {                                                           \
            ACGEType AcGeType = TheArxClass->##getelement();        \
            if (GetOrSet == ELEMENTGET)                             \
            {                                                       \
                ACGEType AcGeTypeUCS;                               \
                acdbWcs2Ucs(asDblArray(AcGeType), asDblArray(AcGeTypeUCS), 0); \
                V_R8(val) = AcGeTypeUCS[cookie];                    \
                V_VT(val) = VT_R8;                                  \
                return 1;                                           \
            }                                                       \
        }

#define COM_ELEMENT_GET_ENTRY(DID, comclass, getfunc, objID)        \
        if (dispid == DID)                                          \
        {                                                           \
            VARIANT dumVar;                                         \
            VariantInit(&dumVar);                                   \
            pObj->close();                                          \
            I##comclass *p = static_cast<I##comclass *>(this);      \
            if(FAILED(p->##getfunc(&dumVar)))                       \
                return 0;                                           \
            if (GetOrSet == ELEMENTGET)                             \
            {                                                       \
                double dPt[3]; double dPtUCS[3];                    \
                if(FAILED(SafeArrayPointToDoubleArrayPoint(V_ARRAY(&dumVar), dPt))) \
                    return 0;                                       \
                acdbWcs2Ucs(dPt, dPtUCS, 0);                        \
                V_R8(val) = dPtUCS[cookie];                         \
                V_VT(val) = VT_R8;                                  \
            }                                                       \
            acdbOpenAcDbObject(pObj, objID, AcDb::kForRead);        \
            return 1;                                               \
        }

#define ELEMENT_SETGET_ENTRY(DID, ACGEType, setelement, getelement, vtype) \
        if (dispid == DID)                                          \
        {                                                           \
            ACGEType AcGeTypeUCS;                                   \
            ACGEType AcGeType = TheArxClass->##getelement();        \
            acdbWcs2Ucs(asDblArray(AcGeType), asDblArray(AcGeTypeUCS), 0); \
            if (GetOrSet == ELEMENTGET)                             \
            {                                                       \
                V_R8(val) = AcGeTypeUCS[cookie];                    \
                V_VT(val) = VT_R8;                                  \
            }                                                       \
            else                                                    \
            {                                                       \
                AcGeTypeUCS[cookie] = V_R8(val);                    \
                acdbUcs2Wcs(asDblArray(AcGeTypeUCS), asDblArray(AcGeType), 0); \
                TheArxClass->##setelement(AcGeType);                \
            }                                                       \
            return 1;                                               \
        }

#define ELEMENT_SETGET_ENTRY_GEPNT2D(DID, setelement, getelement, vtype) \
        if (dispid == DID)                                          \
        {                                                           \
            AcGePoint3d AcGeTypeUCS;                                   \
            AcGePoint2d AcGeType_tmp = TheArxClass->##getelement();        \
            AcGePoint3d AcGeType(AcGeType_tmp[0], AcGeType_tmp[1], 0.0); \
            acdbWcs2Ucs(asDblArray(AcGeType), asDblArray(AcGeTypeUCS), 0); \
            if (GetOrSet == ELEMENTGET)                             \
            {                                                       \
                V_R8(val) = AcGeTypeUCS[cookie];                    \
                V_VT(val) = VT_R8;                                  \
            }                                                       \
            else                                                    \
            {                                                       \
                AcGeTypeUCS[cookie] = V_R8(val);                    \
                acdbUcs2Wcs(asDblArray(AcGeTypeUCS), asDblArray(AcGeType), 0); \
                TheArxClass->##setelement(AcGePoint2d(AcGeType[0], AcGeType[1])); \
            }                                                       \
            return 1;                                               \
        }

#define COM_ELEMENT_SETGET_ENTRY(DID, comclass, setfunc, getfunc, objID) \
        if (dispid == DID)                                          \
        {                                                           \
            VARIANT dumVar;                                         \
            VariantInit(&dumVar);                                   \
            pObj->close();                                          \
                                                                    \
            I##comclass *p = static_cast<I##comclass *>(this);      \
            if(FAILED(p->##getfunc(&dumVar)))                       \
                return 0;                                           \
                                                                    \
            double dPt[3]; double dPtUCS[3];                        \
            if(FAILED(SafeArrayPointToDoubleArrayPoint(V_ARRAY(&dumVar), dPt))) \
                return 0;                                           \
            acdbWcs2Ucs(dPt, dPtUCS, 0);                            \
                                                                    \
            if (GetOrSet == ELEMENTGET)                             \
            {                                                       \
                V_R8(val) = dPtUCS[cookie];                         \
                V_VT(val) = VT_R8;                                  \
            }                                                       \
            else                                                    \
            {                                                       \
                dPtUCS[cookie] = V_R8(val);                         \
                acdbUcs2Wcs(dPtUCS, dPt, 0);                        \
                                                                    \
                SAFEARRAY* pSafeArr;                                \
                if(FAILED(CreateSafeArray(VT_R8, 3, &pSafeArr)))    \
                    return 0;                                       \
                if(FAILED(DoubleArrayPointToSafeArrayPoint(dPt,pSafeArr))) \
                {                                                   \
                    SafeArrayDestroy(pSafeArr);                     \
                    return 0;                                       \
                }                                                   \
                VARIANT var;                                        \
                V_ARRAY(&var) = pSafeArr;                           \
                V_VT(&var) = VT_R8 | VT_ARRAY;                      \
                if(FAILED(p->##setfunc(var)))                       \
                {                                                   \
                    SafeArrayDestroy(pSafeArr);                     \
                    return 0;                                       \
                }                                                   \
                SafeArrayDestroy(pSafeArr);                         \
            }                                                       \
            acdbOpenAcDbObject(pObj, objID, AcDb::kForRead);        \
            return 1;                                               \
        }

#define ELEMENT_GROUP_SETGET_ENTRY(DID, CountFunc, ACGEType, setAt, getAt, vtype) \
        if (dispid == DID)                                          \
        {                                                           \
            ACAD_OPMPROPMAP_ENTRY *pMap = this->GetOPMPropertyMap();\
            if (GetOrSet == ELEMENTCOUNT)                           \
            {                                                       \
                ULONG cnt = TheArxClass->##CountFunc();             \
                V_I4(val) = cnt;                                    \
                V_VT(val) = VT_I4;                                  \
                return 1;                                           \
            }                                                       \
                                                                    \
            ACGEType AcGeType;                                      \
            long grouping = 1;                                      \
            for(int i = 0; pMap[i].pclsidPropPage != NULL; i++)     \
            {                                                       \
                if (pMap[i].dispid == dispid)                       \
                {                                                   \
                    grouping = pMap[i].grouping;                    \
                    break;                                          \
                }                                                   \
            }                                                       \
            ACGEType AcGeTypeUCS;                                   \
            TheArxClass->##getAt(cookie/grouping, AcGeType);        \
            acdbWcs2Ucs(asDblArray(AcGeType), asDblArray(AcGeTypeUCS), 0); \
            if (GetOrSet == ELEMENTGET)                             \
            {                                                       \
                V_R8(val) = AcGeTypeUCS[cookie % grouping ];        \
                V_VT(val) = VT_R8;                                  \
            }                                                       \
            else                                                    \
            {                                                       \
                AcGeTypeUCS[cookie % grouping] = V_R8(val);         \
                acdbUcs2Wcs(asDblArray(AcGeTypeUCS), asDblArray(AcGeType), 0); \
                TheArxClass->##setAt(cookie/grouping, AcGeType);    \
            }                                                       \
            return 1;                                               \
        }

#define ELEMENT_GROUP2_SETGET_ENTRY(DID, CountFunc, ACGEType, setAt, getAt, vtype) \
        if (dispid == DID)                                          \
        {                                                           \
            ACAD_OPMPROPMAP_ENTRY *pMap = this->GetOPMPropertyMap();\
            if (GetOrSet == ELEMENTCOUNT)                           \
            {                                                       \
                ULONG cnt = TheArxClass->##CountFunc();             \
                V_I4(val) = cnt;                                    \
                V_VT(val) = VT_I4;                                  \
                return 1;                                           \
            }                                                       \
                                                                    \
            ACGEType AcGeType;                                      \
            long grouping = 1;                                      \
            for(int i = 0; pMap[i].pclsidPropPage != NULL; i++)     \
            {                                                       \
                if (pMap[i].dispid == dispid)                       \
                {                                                   \
                    grouping = pMap[i].grouping;                    \
                    break;                                          \
                }                                                   \
            }                                                       \
                                                                    \
            ACGEType AcGeTypeUCS;                                   \
            AcGeType = TheArxClass->##getAt(cookie/grouping);       \
            acdbWcs2Ucs(asDblArray(AcGeType), asDblArray(AcGeTypeUCS), 0); \
            if (GetOrSet == ELEMENTGET)                             \
            {                                                       \
                V_R8(val) = AcGeTypeUCS[cookie % grouping ];        \
                V_VT(val) = VT_R8;                                  \
            }                                                       \
            else                                                    \
            {                                                       \
                AcGeTypeUCS[cookie % grouping] = V_R8(val);         \
                acdbUcs2Wcs(asDblArray(AcGeTypeUCS), asDblArray(AcGeType), 0); \
                TheArxClass->##setAt(cookie/grouping, AcGeType);    \
            }                                                       \
            return 1;                                               \
        }

#define ELEMENT_GROUP3_SETGET_ENTRY(DID, Count, ACGEType, setAt, getAt, vtype) \
        if (dispid == DID)                                          \
        {                                                           \
            ACAD_OPMPROPMAP_ENTRY *pMap = this->GetOPMPropertyMap();\
            if (GetOrSet == ELEMENTCOUNT)                           \
            {                                                       \
                ULONG cnt = Count;                                  \
                V_I4(val) = cnt;                                    \
                V_VT(val) = VT_I4;                                  \
                return 1;                                           \
            }                                                       \
                                                                    \
            ACGEType AcGeType;                                      \
            long grouping = 1;                                      \
            for(int i = 0; pMap[i].pclsidPropPage != NULL; i++)     \
            {                                                       \
                if (pMap[i].dispid == dispid)                       \
                {                                                   \
                    grouping = pMap[i].grouping;                    \
                    break;                                          \
                }                                                   \
            }                                                       \
                                                                    \
            ACGEType AcGeTypeUCS;                                   \
            TheArxClass->##getAt((Adesk::UInt16)(cookie/grouping), AcGeType); \
            acdbWcs2Ucs(asDblArray(AcGeType), asDblArray(AcGeTypeUCS), 0); \
            if (GetOrSet == ELEMENTGET)                             \
            {                                                       \
                V_R8(val) = AcGeTypeUCS[cookie % grouping ];        \
                V_VT(val) = VT_R8;                                  \
            }                                                       \
            else                                                    \
            {                                                       \
                AcGeTypeUCS[cookie % grouping] = V_R8(val);         \
                acdbUcs2Wcs(asDblArray(AcGeTypeUCS), asDblArray(AcGeType), 0); \
                TheArxClass->##setAt((Adesk::UInt16)(cookie/grouping), AcGeType); \
            }                                                       \
            return 1;                                               \
        }

#define ELEMENT_ACDB_ITERATE_SETGET_ENTRY(DID, Iter, recordt, ACGEType, setelem, getelem, vtype) \
        if (dispid == DID)                                          \
        {                                                           \
            AcDbObjectIterator* pIter = NULL;                       \
            ACAD_OPMPROPMAP_ENTRY *pMap = this->GetOPMPropertyMap();\
            if (GetOrSet == ELEMENTCOUNT)                           \
            {                                                       \
                ULONG cnt = 0;                                      \
                pIter = TheArxClass-> Iter##();                     \
                for (pIter->start(); !pIter->done(); pIter->step())cnt++; \
                delete pIter;                                       \
                V_I4(val) = cnt;                                    \
                V_VT(val) = VT_I4;                                  \
                return 1;                                           \
            }                                                       \
                                                                    \
            ACGEType AcGeType;                                      \
            long grouping = 1;                                      \
            for(int i = 0; pMap[i].pclsidPropPage != NULL; i++)     \
            {                                                       \
                if (pMap[i].dispid == dispid)                       \
                {                                                   \
                    grouping = pMap[i].grouping;                    \
                    break;                                          \
                }                                                   \
            }                                                       \
                                                                    \
            int j =  0;                                             \
            int vert = cookie/grouping;                             \
            pIter = TheArxClass->Iter();                            \
                                                                    \
            for (pIter->start(); !pIter->done(); pIter->step())     \
            {                                                       \
                if (j == vert)                                      \
                    break;                                          \
                else                                                \
                    j++;                                            \
            }                                                       \
            Acad::ErrorStatus es;                                   \
            AcDbObjectId objID = pIter->objectId();                 \
            recordt *record;                                        \
            AcDbObject *pRecObj;                                    \
                                                                    \
            delete pIter;                                           \
                                                                    \
            if (j != vert)                                          \
                return 0;                                           \
                                                                    \
            if ((es = acdbOpenAcDbObject(pRecObj, objID,            \
                         GetOrSet == ELEMENTGET ? AcDb::kForRead :  \
                                     AcDb::kForWrite)) != Acad::eOk) \
                return 0;                                           \
                                                                    \
            if ((record = recordt##::cast(pRecObj)) == NULL)        \
            {                                                       \
                pRecObj->close();                                   \
                return 0;                                           \
            }                                                       \
                                                                    \
            ACGEType AcGeTypeUCS;                                   \
            AcGeType = record-> getelem##();                        \
            acdbWcs2Ucs(asDblArray(AcGeType), asDblArray(AcGeTypeUCS), 0); \
            if (GetOrSet == ELEMENTGET)                             \
            {                                                       \
                                                                    \
                V_R8(val) = AcGeTypeUCS[cookie % grouping ];        \
                V_VT(val) = VT_R8;                                  \
            }                                                       \
            else                                                    \
            {                                                       \
                AcGeTypeUCS[cookie % grouping] = V_R8(val);         \
                acdbUcs2Wcs(asDblArray(AcGeTypeUCS), asDblArray(AcGeType), 0); \
                record-> setelem##(AcGeType);                       \
            }                                                       \
                                                                    \
           record->close();                                         \
           return 1;                                                \
        }

#define END_ELEMENT_SETGET                                          \
    return 0;                                                       \
    }

#endif