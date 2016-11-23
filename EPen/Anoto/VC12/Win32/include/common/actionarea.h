/*****************************************************************************
 * Copyright (c) Anoto AB 2007-2009.
 * All rights reserved. This code is Anoto proprietary and confidential, and
 * may be protected by patents. Any use of the code for whatever purpose is
 * subject to separate license terms and conditions.
 ************************************************************************V30*/

// original author Mattias Mårtensson

#ifndef _FILE_ACTIONAREA_H_
#define _FILE_ACTIONAREA_H_


/** \ingroup Anotocommon */
/** \defgroup AAAPI ActionArea API

This section contains defines that determines the meaning and graphical representation of
an Action Area.

The meaning is determined by the parameters type (0-255) and value (0-65535). see
\ref ActionAreaTypes "Action Area types" and \ref ActionAreaValues "Action Area values" for details. 

The graphical representation of an Action Area is a patch of special pattern that has been
pasted into a page. The active part of an Action Area is a square in the center of the pattern, leaving
a 7mm border where no events are triggered. It is recommended that this "active"
part of the Action Area is represented by human readable graphics in the paper product.

ActionAreas come in a set of predefined sizes. These can be found in \ref ActionAreaSizes "Action Area sizes".

@{*/

/** @} AAAPI */


/**
 \ingroup AAAPI
 \defgroup ActionAreaTypes Action Area types

The action events are divided into two groups: AFP common events and custom events.
The AFP common events are defined by this API and listed below. The AFP events have a specific
meaning and may be interpreted by different part of the AFP system. 
Custom events are only transported by the AFP system and must be interpreted by the customer application. 

AFP common action event range from 0 to ACTION_AREA_CUSTOM_BASE - 1.
Custom action events are negative values starting with ACTION_AREA_CUSTOM_BASE and below.

@{*/

/** This type is reserved for pen/paper interaction and will always be consumed by the pen internally. 
 *  Event values are defined by \ref ActionAreaValuePen*/
#define	ACTION_AREA_TYPE_PEN					0
/** Used to represent pen stroke color. Event values use the X11 color set of CSS3 as defined by \ref X11Color.  */
#define	ACTION_AREA_TYPE_COLOR				1
/** Used for setting specific PIN code for using a LAN access point. Values 0-9999 sets the PIN, 10000 means pen should use its default PIN.*/
#define	ACTION_AREA_TYPE_LAN_ACCESS_PIN				2

/** The first custom action event. 
Types x, 0 <= x < ACTION_AREA_CUSTOM_BASE, are reserved by Anoto.
Types x, ACTION_AREA_CUSTOM_BASE <= x <= 255, may be used by customers.
*/
#define ACTION_AREA_CUSTOM_BASE				128
/*@} ActionAreaTypes */

/**
 * \ingroup AAAPI
 * \defgroup ActionAreaValues Action Area values
 *
 * This section defines the valid ranges for different \ref ActionAreaTypes "Action Area types". In general
 * the value may be in the range [\ref ACTION_AREA_MIN_VALUE, \ref ACTION_AREA_MAX_VALUE], but for types used by AFP often have a more limited value set.
 *
 * - \ref ACTION_AREA_TYPE_PEN, value set = \ref ActionAreaValuePen .
 * - \ref ACTION_AREA_TYPE_COLOR, value set = \ref X11Color.
 * - \ref ACTION_AREA_TYPE_LAN_ACCESS_PIN, value set = \ref ActionAreaValueLanAccess
 * @{
 */
 
/** Smallest allowed value for an Action Area. */
#define ACTION_AREA_MIN_VALUE	0
/** Largest allowed value for an Action Area. */
#define ACTION_AREA_MAX_VALUE	65534	
/** Use when the value is not known. Use for example to initialize a variable that stores
    the last known value of an action area. */
#define ACTION_AREA_UNDEFINED_VALUE 65535	


/** @} ActionAreaValues. */

/** \ingroup ActionAreaValues
 *  \defgroup ActionAreaValuePen Action Area Type Pen values
 * @{ */
/** Sets the pen in Bluetooth pairing mode.*/
#define ACTION_AREA_VALUE_PEN_PAIR 0

/** Instructs pen to send all stored data to the device it is paired with. */
#define ACTION_AREA_VALUE_SEND_STORED_DATA 1

/////////////////////////////////////////////////////////////////
// DEVICE_SELECTION
//
// Determines what algorithm to use when selecting which paired
// Bluetooth device to connect to. Pen behaviour is described
// in the FW spec.
/////////////////////////////////////////////////////////////////

/** Use automatic device selection */
#define ACTION_AREA_VALUE_DEVICE_SELECTION_ANY 100

/** Select computer */
#define ACTION_AREA_VALUE_DEVICE_SELECTION_COMPUTER 101

/** Select phone */
#define ACTION_AREA_VALUE_DEVICE_SELECTION_PHONE 102

/** Select network access point */
#define ACTION_AREA_VALUE_DEVICE_SELECTION_NETWORK 103

/////////////////////////////////////////////////////////////////
// MODE_SELECTION
//
// Determines what the pen will do with collected coordinates.
/////////////////////////////////////////////////////////////////

/** Pen should stream coordinates to an external device */
#define ACTION_AREA_VALUE_MODE_SELECTION_STREAM 200

/** Pen should store coordinates for later transfer.*/
#define ACTION_AREA_VALUE_MODE_SELECTION_STORE 201

/** Pen should select to store or stream data based on paper attributes*/
#define ACTION_AREA_VALUE_MODE_SELECTION_AUTO 202

/** @} ActionAreaValuePen */

/** \ingroup ActionAreaValues
 *  \defgroup ActionAreaValueLanAccess Action Area Type Lan Access values
 *
 *  Use values 0-9999 for PIN codes.
 *
 *  Values 10001 - ACTION_AREA_MAX_VALUE are currently not in use.
 * @{ */

/** Pen should use the default PIN when connecting to LAN access points. Pen 
  * will forget previously set PIN codes. */
#define ACTION_AREA_VALUE_USE_DEFAULT_PIN 10000

/** @} ActionAreaValueLanAccess */


/** \ingroup AAAPI
 *  \defgroup ActionAreaSizes Action Area sizes
 * 
 * These are the predefined values for Action Area size.
 * 
@{*/
/** Action Area size 1 4x4 mm */
#define ACTION_AREA_SIZE_4X4MM			0
/** Action Area size 2 6x6 mm */
#define ACTION_AREA_SIZE_6x6MM			1
/** Action Area size 3 8x8 mm */	
#define ACTION_AREA_SIZE_8x8MM			2
/** Action Area size 4 10x10 mm */	
#define ACTION_AREA_SIZE_10x10MM		3
/** Action Area size 5 12x12 mm */
#define ACTION_AREA_SIZE_12x12MM		4
/** Action Area size 6 14x14 mm */
#define ACTION_AREA_SIZE_14x14MM		5
/** Action Area size 7 16x16 mm */
#define ACTION_AREA_SIZE_16x16MM		6
/** Action Area size 8 18x18 mm */
#define ACTION_AREA_SIZE_18x18MM		7
/** Action Area size 9 20x20 mm */
#define ACTION_AREA_SIZE_20x20MM		8
/** Action Area size 10 25x25 mm */
#define ACTION_AREA_SIZE_25x25MM		9
/** Action Area size 11 30x30 mm */
#define ACTION_AREA_SIZE_30x30MM		10
/** Action Area size 12 35x35 mm */
#define ACTION_AREA_SIZE_35x35MM		11
/** Action Area size 13 40x40 mm */
#define ACTION_AREA_SIZE_40x40MM		12
/** Action Area size 14 45x45 mm */
#define ACTION_AREA_SIZE_45x45MM		13
/** Action Area size 15 50x50 mm */
#define ACTION_AREA_SIZE_50x50MM		14
/** Action Area size 16 100x100 mm */
#define ACTION_AREA_SIZE_100x100MM		15
/** Number or ACTION_AREA_SIZE_<> defines */
#define ACTION_AREA_NBR_OF_SIZES		16
/*@}*/



#endif



