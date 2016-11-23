/*****************************************************************************
 * Copyright (c) Anoto AB 2007-2009.
 * All rights reserved. This code is Anoto proprietary and confidential, and
 * may be protected by patents. Any use of the code for whatever purpose is
 * subject to separate license terms and conditions.
 ************************************************************************V30*/

// original author Tommy Sighagen

#ifndef _IM_DATABASE_HANDLER_
#define _IM_DATABASE_HANDLER_

// Local includess
//#include <licensemanager/src/common/lmcommon.h>
#include <licensemanager/license/lmlicense.h>
#include <licensemanager/database/lmabstractdb.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** \ingroup LicenseModuleAPI */
/** \defgroup DatabaseAPI Database API

 The  \ref Database is the object representation of a database to be used for storage of pattern licenses. 
 It will be managed by the License (Manager) Module (LM). The Database is abstract, which means that the
 user can select a database of their own choice (SQL, folder structure based etc).\n\n


 To use the LM the abstract db interface must be implemented (See file below), optionally one can use the 
 folder structured (xml based version) included\n
 
 The license example 3 and 4 also demonstrates how this implementation could be done (common code in 'SQLimpl') 
 
  \include components/licensemanager/database/lmabstractdb.h
  
@{*/
	/** \struct Database
		\brief An abstract database.

	  The database (abstract) to be used by LM for storing and managing pattern licenses.
	  The Database is an interface with a number of function pointers, pointing to functions which 
	  can be implemented for a specific database of ones choice (SQL, file structure etc.). 
	@{*/
	
struct Database;

	/*@}*/
/*@}*/

/** \ingroup DatabaseAPI */
/** \defgroup DatabaseHandlerAPI Database Handler API

 The Database Handler API contains functions connected to database handling of pattern licenses.
 \n
@{*/

// Create a sublicense (LM selects by a bestfit algoritm)
EXTAPI_LICENSE_H struct License* dbCreateSubLicense(struct Database *iptDB, struct LicenseSpec *iptLicSpec);

// Creates a preview license (no changes made to the db) - trashed signature
EXTAPI_LICENSE_H struct License *dbCreateSubLicensePreview(struct Database *iptDB, struct LicenseSpec *iptLicSpec);

//Create a sublicense from a specific license
EXTAPI_LICENSE_H struct License* dbCreateSubLicenseFromSpecificLicense(struct Database *iptDB, struct LicenseSpec *iptLicSpec, 
															 const char* pUniqueIdSuperLic);

// Creates a preview license (no changes made to the db) - trashed signature
EXTAPI_LICENSE_H struct License *dbCreateSubLicenseFromSpecificLicensePreview(struct Database *iptDB, struct LicenseSpec *iptLicSpec, 
															 const char* ipUniqueIdSuperLic);

// Deallocate a sublicense i.e. return the pattern space
EXTAPI_LICENSE_H s32 dbDeallocateSubLicense(struct Database *iptDB, struct License *iptLic);

// Changes the sublicense according to the specification - original pattern unchanged
EXTAPI_LICENSE_H struct License *dbChangeSubLicense(struct Database *iptDB, const char *ipUniqueId, 
										  struct LicenseSpec *iptLicSpec);

// Creates a new key pair, if there is need (security issue)
EXTAPI_LICENSE_H boolean dbSetNewKeyPair(struct Database *iptDB);



////// Functions directly mapped towards the abstract db.
////// For these to work the abstract db must be implemented towards a specific database

// Adds (saves) a license to a database (SQL, folder based structure etc depending on implementation) 
EXTAPI_LICENSE_H s32 dbAddLicense(struct Database *iptDB , s32 ilDBType,struct License *iptLicense);

// Removes a license from a database 
EXTAPI_LICENSE_H s32 dbRemoveLicense(struct Database *iptDB , s32 ilDBType,char* ipzLicenseID);

// Get total nbr of licenses
EXTAPI_LICENSE_H s32 dbGetNbrOfLicenses(struct Database *iptDB , s32 ilDBType);

// Load license from a database 
EXTAPI_LICENSE_H struct License * dbLoadLicense(struct Database *iptDB, s32 ilDBType,char* ipzLicenseID);

// Load all licenses from a database
EXTAPI_LICENSE_H struct License ** dbLoadAllLicenses(struct Database *iptDB, s32 ilDBType, s32 *olNbrIfLicense);

// Search and load a specific license or a group of licenses
EXTAPI_LICENSE_H struct License ** dbSearchAndLoadLicenses(struct Database *iptDB, struct SearchQuery *ipSQ, 
														   s32 ilDBType, s32 *oNbrOfLicenses);

// Tear down db and destroy the object
EXTAPI_LICENSE_H void dbTearDown(struct Database **ipptDB);


/*@}*/

#ifdef __cplusplus
}
#endif

#endif
