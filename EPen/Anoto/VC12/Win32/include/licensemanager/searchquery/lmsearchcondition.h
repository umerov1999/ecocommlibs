/*****************************************************************************
 * Copyright (c) Anoto AB 2007-2009.
 * All rights reserved. This code is Anoto proprietary and confidential, and
 * may be protected by patents. Any use of the code for whatever purpose is
 * subject to separate license terms and conditions.
 ************************************************************************V30*/

// original author Tommy Sighagen

#ifndef _lm_searchcondition_h_
#define _lm_searchcondition_h_


// Common includes
#include <os/typedefs.h>
#include <common/anotocommon.h>


#ifdef __cplusplus
extern "C"
{
#endif
	
/** \ingroup SearchQueryAPI */
/** \defgroup LMSearchConditionTypes Search Condition Types

 	Search Condition Types
@{*/

/** Page address type ex 21.0.2.1, used with operators: SC_INSIDE, SC_OUTSIDE */
#define SC_PAGE_ADDRESS_STRING		0   
/** Name type, used with operators: SC_EQUAL, SC_SMALLER_THAN, SC_BIGGER_THAN*/
#define SC_NAME_STRING				1
/** Parent Id type, used with operators: SC_EQUAL, SC_SMALLER_THAN, SC_BIGGER_THAN */
#define SC_PARENT_ID_STRING			2
/** License type name type ex 21.0.2.2_2, used with operators: SC_EQUAL, SC_SMALLER_THAN, SC_BIGGER_THAN*/
#define SC_TYPE_NAME_STRING			3
/** Owner type, used with operators: SC_EQUAL, SC_SMALLER_THAN, SC_BIGGER_THAN*/
#define SC_OWNER_STRING				4
/** Expire date type ex 20070608, used with operators: SC_EQUAL, SC_SMALLER_THAN, SC_BIGGER_THAN */
#define SC_EXPIRE_S32				5
/** Cretation date type ex 20050405, used with operators: SC_EQUAL, SC_SMALLER_THAN, SC_BIGGER_THAN*/
#define SC_CREATION_S32				6
/** Width type (au), used with operators: SC_EQUAL, SC_SMALLER_THAN, SC_BIGGER_THAN*/
#define SC_WIDTH_S32				7
/** Heigth type (au), used with operators: SC_EQUAL, SC_SMALLER_THAN, SC_BIGGER_THAN*/
#define SC_HEIGHT_S32				8
/** Permission type (requires index), used with operators: SC_EQUAL, SC_SMALLER_THAN, SC_BIGGER_THAN*/
#define SC_PERMISSION_S32			9
/** Pages left type, used with operators: SC_EQUAL, SC_SMALLER_THAN, SC_BIGGER_THAN */
#define SC_PAGES_LEFT_S32			10
/** UUnique id type ex 21.0.2.2_2, used with operators: SC_EQUAL, SC_SMALLER_THAN, SC_BIGGER_THAN*/
#define SC_UNIQUE_ID_STRING			11
/** Total pages type, used with operators: SC_EQUAL, SC_SMALLER_THAN, SC_BIGGER_THAN*/
#define SC_TOTAL_PAGES_S32			12
/** Info type, used with operators: SC_EQUAL, SC_SMALLER_THAN, SC_BIGGER_THAN*/
#define SC_INFO_STRING				13
/** License category type, used with operators: SC_EQUAL, SC_SMALLER_THAN, SC_BIGGER_THAN */
#define SC_CATEGORY_STRING			14

/*@}*/


/** \ingroup SearchQueryAPI */
/** \defgroup LMSearchConditionOperators Search Condition Operators

 	Query Operators
@{*/

/** Equal operator*/
#define SC_EQUAL			0
/** Smaller than operator*/
#define SC_SMALLER_THAN		1
/** Bigger than*/
#define SC_BIGGER_THAN		2
/** Inside operator*/
#define SC_INSIDE			3
/** Outside operator*/
#define SC_OUTSIDE			4

/*@}*/


/** \ingroup SearchQueryAPI */
/** \defgroup SearchConditionAPI Search Condition API
 
 The Search Condition API is an API for the management of search conditions.\n\n

 Each search condition is a condition (or sub query) to be used in a search query. The search query 
 itself builds up the entire query to be used in a search for a pattern license. 
 
@{*/
	/** \struct SearchCondition
		\brief A single search condition in a query. 

		A a search condition (or sub query) to be used in a search query.
	@{*/
		struct SearchCondition;
	/*@}*/
	
/** String type*/
#define SC_STRING	0
/** s32 type*/
#define SC_S32		1

// Create a search condition using a string as a selection value
EXTAPI_LICENSE_H struct SearchCondition *scCreateWithString( s32 ilQueryType,  const char *ipQueryValue, s32 ilCompareSetting);

// Create a search condition using a s32 as a selection value
EXTAPI_LICENSE_H struct SearchCondition *scCreateWithS32( s32 ilQueryType,  s32 ilQueryValue, 
													  s32 ilCompareSetting, s32 ilIndex);

// Destroy a search condition
EXTAPI_LICENSE_H void scDestroy( struct SearchCondition ** ipptSC );

// get
EXTAPI_LICENSE_H s32 scGetValueType( struct SearchCondition * iptSC);
EXTAPI_LICENSE_H s32 scGetCompareOperator( struct SearchCondition * iptSC);
EXTAPI_LICENSE_H s32 scGetType( struct SearchCondition * iptQE);
EXTAPI_LICENSE_H boolean scGetValueString(struct SearchCondition * iptSC, char *opQueryValue, s32 ilSizeBuf);
EXTAPI_LICENSE_H s32 scGetValueS32(struct SearchCondition * iptSC);
EXTAPI_LICENSE_H s32 scGetIndex(struct SearchCondition * iptSC);

/*@}*/

#ifdef __cplusplus
}
#endif


#endif  // _lm_searchcondition_h_






