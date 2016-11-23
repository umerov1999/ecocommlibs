/*****************************************************************************
 * Copyright (c) Anoto AB 2007-2009.
 * All rights reserved. This code is Anoto proprietary and confidential, and
 * may be protected by patents. Any use of the code for whatever purpose is
 * subject to separate license terms and conditions.
 ************************************************************************V30*/

// original author Tommy Sighagen

#ifndef _lm_key_pair_h_
#define _lm_key_pair_h_

// Local includess
#include <common/anotocommon.h>

#ifdef __cplusplus
extern "C"
{
#endif

	
/** \ingroup LicenseAPI */
/** \defgroup KeyPairAPI Key Pair API
 
	The KeyPair is an object holding a private-public key pair (asymmetric encryption) for the license.
 @{*/
	/** \struct KeyPair
		\brief A private-public key pair

		A \ref KeyPair is an object holding a private-public key pair for the license.
		A key pair is needed for license security and is used in the
		signing and verification of the license (normally during creation in a sublicening process)
	@{*/
		struct KeyPair;
	/*@}*/


// Create a key pair
EXTAPI_LICENSE_H struct KeyPair *kpCreate(void);

// Destroy a key pair
EXTAPI_LICENSE_H void kpDestroy(struct KeyPair **itppKeyPair);

/*@}*/  
#ifdef __cplusplus
}
#endif

#endif  // _lm_key_pair_h_






