/*****************************************************************************
 * Copyright (c) Anoto AB 2007-2009.
 * All rights reserved. This code is Anoto proprietary and confidential, and
 * may be protected by patents. Any use of the code for whatever purpose is
 * subject to separate license terms and conditions.
 ************************************************************************V30*/
 
#ifndef _PAGEADDRESS_H_
#define _PAGEADDRESS_H_

#include <common/anotocommon.h>
#include <os/typedefs.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define SERIES_BITPOS 52
#define SEGMENT_BITPOS 40
#define SHELF_BITPOS 24
#define BOOK_BITPOS 12
#define PAGE_BITPOS 0

#ifdef _WIN32

#define PA_INVALID ( 0xFFEFFFFFFFFFFFFF )
#define PA_WILDCARD ( 0xFFFFFFFFFFFFFFFF )
#define PA_MISSING ( 0xFFFFFFFFFFFFEFFF )

#define PA_MASK_GETSERIES	0xFFF0000000000000
#define PA_MASK_GETSEGMENT	0x000FFF0000000000  
#define PA_MASK_GETSHELF	0x000000FFFF000000  
#define PA_MASK_GETBOOK		0x0000000000FFF000  
#define PA_MASK_GETPAGE		0x0000000000000FFF  

#define PA_MASK_SETSERIES	0x000FFFFFFFFFFFFF
#define PA_MASK_SETSEGMENT	0xFFF000FFFFFFFFFF 
#define PA_MASK_SETHELF		0xFFFFFF0000FFFFFF 
#define PA_MASK_SETBOOK		0xFFFFFFFFFF000FFF 
#define PA_MASK_SETPAGE		0xFFFFFFFFFFFFF000

#else

#define PA_INVALID ( 0xFFEFFFFFFFFFFFFFULL )
#define PA_WILDCARD ( 0xFFFFFFFFFFFFFFFFULL )
#define PA_MISSING ( 0xFFFFFFFFFFFFEFFFULL )

#define PA_MASK_GETSERIES	0xFFF0000000000000ULL
#define PA_MASK_GETSEGMENT	0x000FFF0000000000ULL  
#define PA_MASK_GETSHELF	0x000000FFFF000000ULL
#define PA_MASK_GETBOOK		0x0000000000FFF000ULL  
#define PA_MASK_GETPAGE		0x0000000000000FFFULL  

#define PA_MASK_SETSERIES	0x000FFFFFFFFFFFFFULL
#define PA_MASK_SETSEGMENT	0xFFF000FFFFFFFFFFULL
#define PA_MASK_SETHELF		0xFFFFFF0000FFFFFFULL
#define PA_MASK_SETBOOK		0xFFFFFFFFFF000FFFULL
#define PA_MASK_SETPAGE		0xFFFFFFFFFFFFF000ULL

#endif

#define PA_GET_SERIES( pa ) ( (s32)(  ( pa & PA_MASK_GETSERIES ) >> SERIES_BITPOS ) )
#define PA_GET_SEGMENT( pa ) ( (s32)(  ( pa & PA_MASK_GETSEGMENT ) >> SEGMENT_BITPOS ) )
#define PA_GET_SHELF( pa )   ( (s32)( ( pa & PA_MASK_GETSHELF ) >> SHELF_BITPOS ) )
#define PA_GET_BOOK( pa )    ( (s32)( ( pa & PA_MASK_GETBOOK) >> BOOK_BITPOS ) )
#define PA_GET_PAGE( pa )    ( (s32)(   pa & PA_MASK_GETPAGE) )

#define PA_SET_SERIES( pa, i ) ( pa = ( pa & PA_MASK_SETSERIES ) | ( ((u64)i) << SERIES_BITPOS )   )
#define PA_SET_SEGMENT( pa, i ) ( pa = ( pa & PA_MASK_SETSEGMENT ) | ( ((u64)i) << SEGMENT_BITPOS )   )
#define PA_SET_SHELF( pa, i )   ( pa = ( pa & PA_MASK_SETHELF ) | ( ((u64)i) << SHELF_BITPOS )   )
#define PA_SET_BOOK( pa, i )    ( pa = ( pa & PA_MASK_SETBOOK ) | ( ((u64)i) << BOOK_BITPOS )   )
#define PA_SET_PAGE( pa, i )    ( pa = ( pa & PA_MASK_SETPAGE ) | ( ((u64)i) << PAGE_BITPOS  )   )


#define PA_SET( seg, shelf, book, page ) ( ( ((u64)(seg))<<SEGMENT_BITPOS ) | ( ((u64)(shelf))<<SHELF_BITPOS ) | ( ((u64)(book))<<BOOK_BITPOS ) | ((u64)(page)) )
#define PA_SET2( section, seg, shelf, book, page ) ( (( ((u64)(section))<<SERIES_BITPOS ) | ((u64)(seg))<<SEGMENT_BITPOS ) | ( ((u64)(shelf))<<SHELF_BITPOS ) | ( ((u64)(book))<<BOOK_BITPOS ) | ((u64)(page)) )
#define PA_DBG(pa) PA_GET_SEGMENT(pa),PA_GET_SHELF(pa),PA_GET_BOOK(pa),PA_GET_PAGE(pa)
#define PA_DBG2(pa) PA_GET_SERIES(pa),PA_GET_SEGMENT(pa),PA_GET_SHELF(pa),PA_GET_BOOK(pa),PA_GET_PAGE(pa)

boolean paExpandWildcard( u64 iullPA, u64 * oullLowPA, u64 * oullHighPA );
boolean paContains( u64 iullMask, u64 iullAddress );

boolean paGetSegmentProperites( u64 iullPA,u32 *oulNumberofShelves,u32 *oulNumberofBooks, u32 *oulNumberofPages );

/** \ingroup PatternAdress
    \defgroup PatternAddressDefs Pattern Address defines
    Different defines used by the \ref PatternAdress.
@{*/
/**  \def PERMISSION_STREAM */
/** Streaming permission */
#define PERMISSION_STREAM 3
/*@}*/

// Checks the rights to use a specific permission given a page address
EXTAPI_COMMON_H boolean paCheckPermission( u64 iullPA, s32 ilPermission);

// Calculates number of pages between two page addresses i.e. 0.1.0.0.2 - 0.1.0.0.0 = 3. 
EXTAPI_COMMON_H s32 paSubtract( u64 iullStopPA, u64 iullStartPA );

// Calculates a page address by adding a number of pages to a start PA 
EXTAPI_COMMON_H u64 paAdd( u64 iullPA, s32 ilOffset );

// Serialize a page address i.e. turn it into a string format
EXTAPI_COMMON_H void paSerializePA( char *ozBuffer, u64 iullPA );

// Deserialize a page address i.e. convert the string format to a 64 bit value
EXTAPI_COMMON_H boolean paDeserializePA( const char *izBuffer, u64 *oullPA );


#ifdef __cplusplus
}
#endif

#endif
