/*****************************************************************************
 * Copyright (c) Anoto AB 2007-2009.
 * All rights reserved. This code is Anoto proprietary and confidential, and
 * may be protected by patents. Any use of the code for whatever purpose is
 * subject to separate license terms and conditions.
 ************************************************************************V30*/
 
//  original author Tommy Sighagen

#ifndef _LM_LICENSE_SPECIFICATION_H_
#define _LM_LICENSE_SPECIFICATION_H_

// Local includess
#include <common/anotocommon.h>
#include <common/io/outputstream.h>
#include <common/io/inputstream.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** \ingroup LicenseAPI */
/** \defgroup LicenseSpecificationAPI License Specification API
 
	A license specification specifies a number of license properties, such as permissions, number of pages, page size etc. 
	In a sublicensing process a \ref LicenseSpec
	is used in the operation of selecting a (super)license and also to creating the new (sub)license.
 @{*/
	/** \struct LicenseSpec
		\brief A license specification

		A \ref LicenseSpec is as specification over a number of license properties, such as permissions, number of pages, page size etc. 
	@{*/
		struct LicenseSpec;
/*@}*/

		
//Create a license specification
EXTAPI_LICENSE_H struct LicenseSpec *specCreate(char *ipzName, u64 iullUserId, char *ipzOwner, s32 ilExpireDate, 
								 s32 ilWidth, s32 ilHeight, s32 *ipalPermissions, char** ipazPermissionNames,
								 s32 ilNbrOfPermissions, s32 ilNbrOfPages, char* ipzLicenseTypeName, 
								 char* ipzPatternCategoryName, char* ipzInfo, u64 iullAFDGUID);

//Destroy a license specification
EXTAPI_LICENSE_H void specDestroy(struct LicenseSpec **ipptLS);

// Converts a license spec to the current version. Reads spec from file.
EXTAPI_LICENSE_H s32 specConvertToCurrentVersionFromFile(const char *ipFileName, struct LicenseSpec **otppLS);

// Converts a license spec to the current version
EXTAPI_LICENSE_H s32 specConvertToCurrentVersion(struct InputStream* iptStream, struct LicenseSpec **otppLS);

//Load a license spec from file
EXTAPI_LICENSE_H struct LicenseSpec *specLoadFromFile(const char *ipFileName);

//Save a license spec to file
EXTAPI_LICENSE_H boolean specSaveToFile(struct LicenseSpec *tpLS, const char *ipFileName);
 
//Load a license spec using read func (load to a buffer of ones own chooise)
EXTAPI_LICENSE_H struct LicenseSpec *specLoad(struct InputStream *iptStream);

//Save a license spec using write func (saves to a buffer of ones own choise)
EXTAPI_LICENSE_H boolean specSave(struct LicenseSpec *iptLicenseSpec, struct OutputStream *iptStream);

// set 
EXTAPI_LICENSE_H boolean specSetPermissionName(struct LicenseSpec *iptLS,  s32 ilIndex, const char* ipPerName);
EXTAPI_LICENSE_H boolean specSetTypeName(struct LicenseSpec *iptLS, const char* ipTypeName);
EXTAPI_LICENSE_H boolean specSetName(struct LicenseSpec *iptLS, const char* iName);
EXTAPI_LICENSE_H boolean specSetOwner(struct LicenseSpec *iptLS, const char* ipOwner);
EXTAPI_LICENSE_H boolean specSetUserId(struct LicenseSpec *iptLS, u64 iullUserId);
EXTAPI_LICENSE_H boolean specSetPermissions(struct LicenseSpec *iptLS, s32 *ipLicPermissions, s32 ilNbrOfPermissions);
EXTAPI_LICENSE_H boolean specSetPermission(struct LicenseSpec *iptLS,  s32 ilIndex, s32 ilPermissionValue);
EXTAPI_LICENSE_H boolean specSetExpireDate(struct LicenseSpec *iptLS, s32 ilExpireDate);
EXTAPI_LICENSE_H boolean specSetPageHeight(struct LicenseSpec *iptLS, s32 ilHeight);
EXTAPI_LICENSE_H boolean specSetPageWidth(struct LicenseSpec *iptLS, s32 ilWidth);
EXTAPI_LICENSE_H boolean specSetNbrOfPages(struct LicenseSpec *iptLS, s32 ilNbrOfPages);
EXTAPI_LICENSE_H boolean specSetInfo(struct LicenseSpec *iptLS, const char* iInfo);
EXTAPI_LICENSE_H boolean specSetPatternCategoryName(struct LicenseSpec *iptLS, const char * iCategoryName);
EXTAPI_LICENSE_H boolean specSetAFDGUID(struct LicenseSpec *iptLS, u64 iullAFDGUID);

// get
EXTAPI_LICENSE_H boolean specGetPermissionName(struct LicenseSpec *iptLS,  s32 ilIndex, char* opPerName, s32 lSize);
EXTAPI_LICENSE_H boolean specGetTypeName(struct LicenseSpec *iptLS, char* opLicTypeName, s32 ilSize);
EXTAPI_LICENSE_H boolean specGetName(struct LicenseSpec *iptLS, char* opLicName, s32 ilSize);
EXTAPI_LICENSE_H boolean specGetOwner(struct LicenseSpec *iptLS, char* opLicOwner, s32 ilSize);
EXTAPI_LICENSE_H u64 specGetUserId(struct LicenseSpec *iptLS);
EXTAPI_LICENSE_H s32 specGetNbrOfPermissions(struct LicenseSpec *iptLS);
EXTAPI_LICENSE_H s32 specGetPermission(struct LicenseSpec *iptLS, s32 ilIndex);
EXTAPI_LICENSE_H boolean specGetPermissions(struct LicenseSpec *iptLS, s32 olaLicPermissions[], s32 ilSize);
EXTAPI_LICENSE_H s32 specGetExpireDate(struct LicenseSpec *iptLS);
EXTAPI_LICENSE_H s32 specGetPageHeight(struct LicenseSpec *iptLS);
EXTAPI_LICENSE_H s32 specGetPageWidth(struct LicenseSpec *iptLS);
EXTAPI_LICENSE_H boolean specGetInfo(struct LicenseSpec *iptLS, char* opInfo, s32 ilSize);
EXTAPI_LICENSE_H boolean specGetPatternCategoryName(struct LicenseSpec *iptLS, char* opCategoryName, s32 ilSize);
EXTAPI_LICENSE_H s32 specGetNbrOfPages(struct LicenseSpec *iptLS);
EXTAPI_LICENSE_H u64 specGetAFDGUID(struct LicenseSpec *iptLS);
EXTAPI_LICENSE_H boolean specGetVersion(struct LicenseSpec *iptLS, char* opVersion, s32 ilSize);

/*@}*/

#ifdef __cplusplus
}
#endif


#endif






