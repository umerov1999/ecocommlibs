/*****************************************************************************
 * Copyright (c) Anoto AB 2007-2009.
 * All rights reserved. This code is Anoto proprietary and confidential, and
 * may be protected by patents. Any use of the code for whatever purpose is
 * subject to separate license terms and conditions.
 ************************************************************************V30*/

// original author Tommy Sighagen

#ifndef _LM_LICENSEREADER_H_
#define _LM_LICENSEREADER_H_

// Common includes
#include <os/typedefs.h>
#include <common/anotocommon.h>
#include <common/io/inputstream.h>

#ifdef __cplusplus
extern "C"
{
#endif
 

#if defined(_WIN32)
  #define LICENSE_MODULE_VERSION "2.7.1.0_2"
#elif defined(__APPLE__)
  #define LICENSE_MODULE_VERSION "2.5.0.0_2"
#else
  #define LICENSE_MODULE_VERSION "2.5.0.0_1"  // Other plattform...ARM, Linux	
#endif
	

#define USER_ID_UN_DEF ULL(0xFFFFFFFFFFFFFFFF)

/** \ingroup LicenseAPI */

/** \ingroup ErrorCodeRanges
    \defgroup LicenseManagerErrorCodes Anoto License Module Error codes
@{*/
/** Return value indicating that two hash values did not match */
#define LM_HASH_ERROR		-1200
/** Return value indicating that RSA encryption or decryption failed*/
#define LM_RSA_CRYPTO_ERROR	-1201
/** Return value indicating that RSA key generation error failed*/
#define LM_RSA_KEY_GEN_ERROR	-1202
/** Return value indicating that the private key does not match the embedded public key*/
#define LM_RSA_KEY_ERROR		-1203
/** The license's major version is not supported. License cannot be opened or converted. */
#define LM_UNSUPPORTED_MAJOR_VERSION  -1204
/** The license's minor version is not fully supported. License cannot be opened as is, but may be converted to a readable version.   */
#define LM_UNSUPPORTED_MINOR_VERSION  -1205

/**---------------------------------------*/
/*@}*/

/** \ingroup LicenseAPI */
/** \defgroup LMHardPermissions Hardcoded permissions

 	Hardcoded permissions
@{*/

/** Permission to sublicense*/
#define SUBLICENSING_PERMISSION_ID		0   
/** Permission to deallocate pattern*/
#define DEALLOCATION_PERMISSION_ID		1  
/** Permission to print*/
#define PRINT_PERMISSION_ID				2   

/**--------------------------------------*/
	
/*@}*/

/** \ingroup LicenseAPI */
/** \defgroup LMVersions LM Versions

LM Versions
@{*/

/** Current license version */
#define LICENSE_VERSION		"2.0" 

/** Current license specification version */
#define LICENSE_SPEC_VERSION		"1.0" 
/**--------------------------------------*/

/*@}*/
	
/** \ingroup LicenseAPI */
/** \defgroup LicenseReaderAPI License Reader API
 
 The license reader is an API for reading, get info, verifying and do permission checks on pattern licenses.
@{*/

struct License;

// Converts a license to the current version. Reads license from file.
EXTAPI_LICENSE_H s32 licConvertToCurrentVersionFromFile(const char *ipFileName, struct License **otppLicense);

// Converts a license to the current version
EXTAPI_LICENSE_H s32 licConvertToCurrentVersion(struct InputStream* iptStream, struct License **otppLicense);

//Loads a license from a file (not zip)
EXTAPI_LICENSE_H struct License *licLoadFromFile(const char *ipFileName);

//Loads a license from some kind of stream (buffer)
EXTAPI_LICENSE_H struct License *licLoad(struct InputStream* iptStream );

//Destroys a license 
EXTAPI_LICENSE_H void licDestroy( struct License **ipptLicense );

//Check permission 
EXTAPI_LICENSE_H boolean licAskPermission(struct License *iptLicense, s32 ilPermissionId, s32 ilPermissionValue);

//Check if a page address is inside the license pattern space
EXTAPI_LICENSE_H boolean licPAInLicense(struct License *iptLicense, u64 iullPageAddress);

//Check authentication and for manipulations
EXTAPI_LICENSE_H s32 licVerify(struct License *iptLicense);

//get
EXTAPI_LICENSE_H s32 licGetTotalNbrOfPagesLeft(struct License *iptLicense);
EXTAPI_LICENSE_H boolean licGetUniqueId(struct License *iptLicense, char* opUId, s32 lSize);
EXTAPI_LICENSE_H s32 licGetPermission(struct License *iptLic, s32 ilIndex);
EXTAPI_LICENSE_H boolean licGetPermissionName(struct License *iptLic,  s32 ilIndex, char* opPerName, s32 lSize);
EXTAPI_LICENSE_H boolean licGetParentUniqueId(struct License *iptLic,  char* opUId, s32 lSize);
EXTAPI_LICENSE_H boolean licGetTypeName(struct License *iptLic, char* opLicTypeName, s32 ilSize);
EXTAPI_LICENSE_H boolean licGetName(struct License *iptLic, char* opLicName, s32 ilSize);
EXTAPI_LICENSE_H boolean licGetOwner(struct License *iptLic, char* opLicOwner, s32 ilSize);
EXTAPI_LICENSE_H u64 licGetUserId(struct License *iptLic);
EXTAPI_LICENSE_H s32 licGetNbrOfPermissions(struct License *iptLic);
EXTAPI_LICENSE_H boolean licGetPermissions(struct License *iptLic, s32 olaLicPermissions[], s32 ilSize);
EXTAPI_LICENSE_H s32 licGetExpireDate(struct License *iptLic);
EXTAPI_LICENSE_H s32 licGetCreationDate(struct License *iptLic);
EXTAPI_LICENSE_H s32 licGetPageHeight(struct License *iptLic);
EXTAPI_LICENSE_H s32 licGetPageWidth(struct License *iptLic);
EXTAPI_LICENSE_H s32 licGetSubLicenseLevel(struct License *iptLic);
EXTAPI_LICENSE_H s32 licGetNbrOfPARanges(struct License *iptLic);
EXTAPI_LICENSE_H boolean licGetPageAddressRange(struct License *iptLic, s32 ilIndex ,u64 *opullPAStart, u64 *opullPAStop, s32 *opullPagesLeft);
EXTAPI_LICENSE_H s32 licGetTotalNbrOfPages(struct License *iptLic);
EXTAPI_LICENSE_H s32 licGetPagesPerBook(struct License *iptLic);
EXTAPI_LICENSE_H boolean licGetInfo(struct License *iptLic, char* opInfo, s32 ilSize);
EXTAPI_LICENSE_H boolean licGetPatternCategoryName(struct License *iptLic, char* opCategoryName, s32 ilSize);
EXTAPI_LICENSE_H boolean licGetVersion(struct License *iptLic, char* opVersion, s32 ilSize);
EXTAPI_LICENSE_H boolean licGetSignature(struct License *iptLic, u8 *opSignature, s32 ilSize);
EXTAPI_LICENSE_H u64 licGetAFDGUID(struct License *iptLic);
EXTAPI_LICENSE_H struct License *licGetRootLicPart(struct License *iptLicense);

EXTAPI_LICENSE_H boolean licGetModuleVersion(struct License *itpLic, char *opVersionBuf,  s32 ilBufSize);

// Convenience functions

// Serializes a byte array
EXTAPI_LICENSE_H void licSerializeByteArray(u8 *izTempBufIn, char *ozTempBufOut, s32 ilNbrOfValues);

// Deserializes a byte array
EXTAPI_LICENSE_H void licDeserializeByteArray(const char *ipData, u32 iulLength, u8* obputData);

/*@}*/

#ifdef __cplusplus
}
#endif


#endif



