// $Header: //depot/release/ironman2012/develop/global/inc/dbhaext.h#1 $ $Change: 237375 $ $DateTime: 2011/01/30 18:32:54 $ $Author: integrat $
// $NoKeywords: $
//
// (C) Copyright 1993-1999 by Autodesk, Inc.
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
// DBHAEXT.H
//
// DESCRIPTION: This class provides the abstract base class for
//              protocol extension on AcDbHostApplicationServices
//              whereby a host application can instruct to an Object
//              Enabler what services it doesn't want to expose to
//              its end users. The Object Enabler, upon querying this,
//              selectively denys certain service for that particular
//              host app.
//
//              Absence of this protocol extension on the host app
//              is interpreted as all service is allowed.
//

#ifndef AcDbHostApplicationServiceRestrictions_INC
#define AcDbHostApplicationServiceRestrictions_INC

#include "acdb.h"
#include "dbmain.h"

#pragma pack (push, 8)

class AcDbHostApplicationServiceRestrictions : public AcRxObject
{
public:

    enum RestrictionType {
        kAllAllowed              = 0x00,
        kTransformNotAllowed     = 0x01,
        kEditingNotAllowed       = 0x02,
        kCloneNotAllowed         = 0x04
        };

	ACRX_DECLARE_MEMBERS(AcDbHostApplicationServiceRestrictions);

	virtual AcDbHostApplicationServiceRestrictions::RestrictionType
                whatsNotAllowed() const  = 0;
};

#pragma pack (pop)

#endif

