/*****************************************************************************
 * Copyright (c) Anoto AB 2007-2009.
 * All rights reserved. This code is Anoto proprietary and confidential, and
 * may be protected by patents. Any use of the code for whatever purpose is
 * subject to separate license terms and conditions.
 ************************************************************************V30*/

// original author Tommy Sighagen

#ifndef _LM_LICENSE_H_
#define _LM_LICENSE_H_

// Local includes
#include <common/anotocommon.h>
#include <licensemanager/license/lmpublickey.h>
#include <common/io/outputstream.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** \defgroup LicenseModuleAPI License Module

	The License Module API is separated into two major blocks, the \ref LicenseAPI for license handling and the 
	\ref DatabaseAPI for license database management. 
*/
/** \ingroup LicenseModuleAPI */
/** \defgroup LicenseAPI License API
 
   The License API is in the license handling block, it contains functionality for getting license info, 
   save licenses, create licenses etc (see description for each function)  \n\n

   The central component/object, from where the functions are operating, is the License object, this is an 
   representation of an Anoto pattern license i.e. a license  to be used in an AFP solution.\n
@{*/

struct License;

// save a license to a file
EXTAPI_LICENSE_H boolean licSaveToFile(struct License *tpLic, char *ipFileName);

// save a license using output stream
EXTAPI_LICENSE_H boolean licSave(struct License *iptLic, struct OutputStream *iptStream );

// create a license specification from a license.
EXTAPI_LICENSE_H struct LicenseSpec *licCreateSpecification(struct License *iptLic);

//Creates a trashed root license
EXTAPI_LICENSE_H struct License *licCreateRootPart(char *ipName, u64 iullUserId, char *ipOwner, char* ipParentUniqueID, 
										 s32 ilExpireDate,	s32 ilCreationDate, s32 ilWidth, s32 ilHeight,
										 s32 ilLevel, u64 *ipaullPAStart, 
										 u64 *ipaullPAStop, s32 ilNbrOfPARanges, s32 *ipaulNbrOfPagesLeft, 
										 s32 *iplPermissions, char** ipazPermissionNames,
										 s32 ilNbrOfPermissions, char* ipLicenseTypeName, 
										 char* ipPatternCategoryName, char* ipInfo, char* ipzVersion,  
										 u8* ipzSignature);


// recreates an existing license (from a DB) - checks the signature (must be valid). 
EXTAPI_LICENSE_H struct License *licCreate(char *ipName, u64 iullUserId, char *ipOwner, char* ipParentUniqueID, 
										 s32 ilExpireDate,	s32 ilCreationDate, s32 ilWidth, s32 ilHeight,
										 s32 ilLevel, u64 *ipaullPAStart, 
										 u64 *ipaullPAStop, s32 ilNbrOfPARanges, s32 *ipaulNbrOfPagesLeft, 
										 s32 *iplPermissions, char** ipazPermissionNames,
										 s32 ilNbrOfPermissions, char* ipLicenseTypeName, 
										 char* ipPatternCategoryName, char* ipInfo, char* ipzVersion, u64 ullAFDGUID,
										 u8* ipzSignature ,struct PublicKey *itpPK, 
										 struct License *itpTrashedRootLic);

//set the pattern category name (do not affect license signature)
EXTAPI_LICENSE_H boolean licSetPatternCategoryName(struct License *iptLic, const char * ipCategoryName);

//set the owner (do not affect license signature)
EXTAPI_LICENSE_H boolean licSetOwner(struct License *iptLic, const char* ipOwner);

//set the name (do not affect license signature)
EXTAPI_LICENSE_H boolean licSetName(struct License *iptLic, const char* ipName);

//set the type (do not affect license signature)
EXTAPI_LICENSE_H boolean licSetTypeName(struct License *iptLic, const char* ipTypeName);

//set the info (do not affect license signature)
EXTAPI_LICENSE_H boolean licSetInfo(struct License *iptLic, const char* ipInfo);

//get the license public key
EXTAPI_LICENSE_H struct PublicKey *licGetCreatePublicKey(struct License *iptLic);


// Convenience functions

// Creates a license array
EXTAPI_LICENSE_H struct License **licCreateArray(s32 ilNbrOfLicenses);

// Destroys a license array
EXTAPI_LICENSE_H void licDestroyArray(struct License ***tpppLic, s32 ilNbrOfLicenses);

/*@}*/

#ifdef __cplusplus
}
#endif


#endif






