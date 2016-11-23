/*****************************************************************************
 * Copyright (c) Anoto AB 2007-2009.
 * All rights reserved. This code is Anoto proprietary and confidential, and
 * may be protected by patents. Any use of the code for whatever purpose is
 * subject to separate license terms and conditions.
 ************************************************************************V30*/

// original author Staffan Solén

#ifndef DIFF_PROPERTY_H
#define DIFF_PROPERTY_H

// Common includes
#include <os/typedefs.h>
#include <afd/document/anotodocument.h>

boolean dpSetTimeDiff(struct AnotoDocument *iptAD, u64 iullPenid, u64 iullTimeDiff );

u64 dpGetTimeDiff(struct AnotoDocument *iptAD, u64 iullPenid );


#endif // DIFF_PROPERTY_H
