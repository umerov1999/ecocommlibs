/*****************************************************************************
 * Copyright (c) Anoto AB 2007-2009.
 * All rights reserved. This code is Anoto proprietary and confidential, and
 * may be protected by patents. Any use of the code for whatever purpose is
 * subject to separate license terms and conditions.
 ************************************************************************V30*/

// original author Tommy Sighagen

#ifndef lmabstractdb_H
#define lmabstractdb_H

// Standard includes

// Local includes
#include <common/anotocommon.h>
#include <os/typedefs.h>
#include <licensemanager/searchquery/lmsearchquery.h>

#ifdef __cplusplus
extern "C"
{
#endif

// Database types.
#define SUPER_LICENSE_DB 0
#define SUB_LICENSE_DB 1

struct Database {

	// Adds (saves) a license to a database (SQL, folder based structure etc depending on implementation) 
	// Parameters:
	// (in) iptDB - the database object
	// (in) ilDBType - the database type (i.e. SUB_LICENSE_DB or SUPER_LICENSE_DB)
	// (in) iptLicense - the license 
	s32 (*dbAddLicense)(struct Database *iptDB , s32 ilDBType,struct License *iptLicense);


	// Removes a license from a database 
	// Parameters:
	// (in) iptDB - the database object
	// (in) ilDBType - the database type (i.e. SUB_LICENSE_DB or SUPER_LICENSE_DB)
	// (in) ipzLicenseID - the license id (example 21.0.0.10)
	s32 (*dbRemoveLicense)(struct Database *iptDB , s32 ilDBType,char* ipzLicenseID);
	
	// Get total nbr of licenses
	// Parameters:
	// (in) iptDB - the database object
	// (in) ilDBType - the database type (i.e. SUB_LICENSE_DB or SUPER_LICENSE_DB)
	s32 (*dbGetNbrOfLicenses)(struct Database *iptDB , s32 ilDBType);

	// Load license from a database 
	// Parameters:
	// (in) iptDB - the database object
	// (in) ilDBType - the database type (i.e. SUB_LICENSE_DB or SUPER_LICENSE_DB)
	// (in) ipzLicenseID - the license id (example 21.0.0.10)
	struct License *(*dbLoadLicense)(struct Database *iptDB, s32 ilDBType,char* ipzLicenseID);
	
	// Load all licenses from a database
	// Parameters:
	// (in) iptDB - the database object
	// (in) ilDBType - the database type (i.e. SUB_LICENSE_DB or SUPER_LICENSE_DB)
	// (out) olNbrIfLicense - number of licenses loaded
	struct License **(*dbLoadAllLicenses)(struct Database *iptDB, s32 ilDBType, s32 *olNbrIfLicense );

	// Search and load a specific license or a group of licenses
	// Parameters:
	// iptDB - the database object
	// ipQC - The query collection containing sub queries for the search
	// ilDBType - the database type (i.e. SUB_LICENSE_DB or SUPER_LICENSE_DB)
	// oNbrOfLicenses - number of licenses found
	struct License **(*dbSearchAndLoadLicenses)(struct Database *iptDB, struct SearchQuery *ipSQ, s32 
												ilDBType, s32 *oNbrOfLicenses);
	
	// Tear down db and destroy the object
	// Parameters:
	// ipptDB - the database object
	void (*dbTearDown)(struct Database **ipptDB);

	// Optional data. Void pointer which can be pointing at any data needed, for example 
	// some kind of database context
	void *pUserData;
	
	// should be set in a db create function using dbSetKeyPair.
	struct KeyPair *tpKP;   
}; 

#ifdef __cplusplus
}
#endif

#endif

