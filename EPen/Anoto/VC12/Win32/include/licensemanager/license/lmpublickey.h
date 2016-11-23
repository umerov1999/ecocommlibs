/*****************************************************************************
 * Copyright (c) Anoto AB 2007-2009.
 * All rights reserved. This code is Anoto proprietary and confidential, and
 * may be protected by patents. Any use of the code for whatever purpose is
 * subject to separate license terms and conditions.
 ************************************************************************V30*/

// original author Tommy Sighagen

#ifndef _lm_public_key_h_
#define _lm_public_key_h_

// Local includess
#include <common/anotocommon.h>

#ifdef __cplusplus
extern "C"
{
#endif

	
/** \ingroup LicenseAPI */
/** \defgroup PublicKeyAPI Public Key API
 
	The PublicKey is an object holding the public key for the license.
 @{*/
	/** \struct PublicKey
		\brief A public key

		A \ref PublicKey is an object holding the public key for the license. 
	@{*/
		struct PublicKey;
	/*@}*/


// Create a public key
EXTAPI_LICENSE_H struct PublicKey *pukCreate(u8 *ibModulus, u8 *ibExponent);

// Destroy a public key
EXTAPI_LICENSE_H void pukDestroy(struct PublicKey **itppPubKey);

// Get the modulus value from the pub key
EXTAPI_LICENSE_H boolean pukGetModulus(struct PublicKey *itpPubKey, u8 *obModulus, s32 ilSize);

// Get the exponent value from the pub key 
EXTAPI_LICENSE_H boolean pukGetExponent(struct PublicKey *itpPubKey, u8 *obExponent, s32 ilSize);

// Set the modulus value in the pub key
EXTAPI_LICENSE_H boolean pukSetModulus(struct PublicKey *itpPubKey, u8 *ibModulus);

// Set the exponent value in the pub key
EXTAPI_LICENSE_H boolean pukSetExponent(struct PublicKey *itpPubKey, u8 *ibExponent);
/*@}*/  
#ifdef __cplusplus
}
#endif

#endif  // _lm_public_key_h_






