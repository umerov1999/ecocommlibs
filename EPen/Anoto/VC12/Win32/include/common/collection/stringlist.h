/*****************************************************************************
 * Copyright (c) Anoto AB 2007-2009.
 * All rights reserved. This code is Anoto proprietary and confidential, and
 * may be protected by patents. Any use of the code for whatever purpose is
 * subject to separate license terms and conditions.
 ************************************************************************V30*/

#ifndef _stringlist_h_
#define _stringlist_h_

#include <common/anotocommon.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** \ingroup CollectionAPI */
/** \defgroup StringListClass StringList
 *  StringList is a double linked list that contains strings.
 @{*/
 /** \struct StringList
     \brief A list of strings
@{*/
struct StringList;
/*@}*/

 /** \struct StringListIterator
     \brief A StringListIterator enables iteration over the strings in a StringList.
@{*/
struct StringListIterator;
/*@}*/
/*@}*/

////////////////////////////////////////////////////////
// List Public API
////////////////////////////////////////////////////////


// Constructor
EXTAPI_COMMON_H struct StringList* strListCreate(void);

// Destructor
EXTAPI_COMMON_H s32 strListDestroy(struct StringList** ipptList);
// Add a string to the beginning of the list
EXTAPI_COMMON_H s32 strListAdd(struct StringList* iptList, char* izString);
// Add a string to the end of the list
EXTAPI_COMMON_H s32 strListAppend(struct StringList* iptList, char *izString);
// Remove a string from the list
EXTAPI_COMMON_H s32 strListRemove(struct StringList* iptList, char *izString);
// Create an iterator
EXTAPI_COMMON_H s32 strListCreateIterator(struct StringList* iptList, struct StringListIterator** opptIter);


/////////////////////////////////////////////////////////
// StringListIterator
/////////////////////////////////////////////////////////

// Gets the string and point to the next string in the list
EXTAPI_COMMON_H char *strListIterNext(struct StringListIterator* iptIter);
// Check if the end of the list is reached
EXTAPI_COMMON_H boolean strListIterEnd(struct StringListIterator* iptIter);
// Destroy the iterator
EXTAPI_COMMON_H s32 strListIterDestroy(struct StringListIterator** ipptIter);


#ifdef __cplusplus
}
#endif

#endif  // _stringlist_h_

