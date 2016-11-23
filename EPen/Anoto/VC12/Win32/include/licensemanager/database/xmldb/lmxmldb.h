/*****************************************************************************
 * Copyright (c) Anoto AB 2007-2009.
 * All rights reserved. This code is Anoto proprietary and confidential, and
 * may be protected by patents. Any use of the code for whatever purpose is
 * subject to separate license terms and conditions.
 ************************************************************************V30*/

// original author Tommy Sighagen

#ifndef lmxmlabstractdb_H
#define lmxmlabstractdb_H

// Standard includes

// Local includes
#include <common/anotocommon.h>
#include <os/typedefs.h>
#include <licensemanager/searchquery/lmsearchquery.h>
#include <licensemanager/license/lmlicense.h>

#ifdef __cplusplus
extern "C"
{
#endif 

struct License;	//Forward declaration

/** \ingroup DatabaseAPI */
/** \defgroup XMLDatabaseAPI XML Database API

  The XML Database API is an implementation of LM:s abstract database interface.\n\n

  The implementation is towards a folder structured database. The licenses are saved in xml format.
@{*/


//Sets up the database and creates the db object
EXTAPI_LICENSE_H struct Database *dbXMLCreateDatabase(char* izDBFolderPath); 

/*@}*/

#ifdef __cplusplus
}
#endif


#endif















