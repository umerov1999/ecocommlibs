/*****************************************************************************
 * Copyright (c) Anoto AB 2007-2009.
 * All rights reserved. This code is Anoto proprietary and confidential, and
 * may be protected by patents. Any use of the code for whatever purpose is
 * subject to separate license terms and conditions.
 ************************************************************************V30*/
 
#ifndef anotocommon_h
#define anotocommon_h

/*
 * file anotocommon.h
 * Common API Definitions.
 * This file contains the definition of common return values and read/write functions
 * that are common to several ANSI C APIs.
 * date    2005-05-27
 * author  Bj?n Fransson
*/

#ifdef __cplusplus
extern "C"
{
#endif

#include <os/typedefs.h>

#include "extapicfg.h"

 
/** \ingroup Anotocommon
	\defgroup AnotoDocumentTypes Anoto Document Types

	These types are used when handling Anoto documents. 
@{*/
/**
 * Document type.
 * Used to distinguish between the different formats an Anoto document can be saved in.
 */
typedef enum
{
	DOC_MOUNT_DIR = 0, /**< The document is a directory */
	DOC_MOUNT_ZIP = 1 /**< The document is a zip archive */
} DOC_MOUNT_TYPE;

/**
 * Open mode for document.
 */
typedef enum
{
	DOC_OPEN_READ		= 0, /**< The document is read only */
	DOC_OPEN_READ_WRITE	= 1  /**< The document is read/write. */
} DOC_OPEN_MODE;
/*@}*/

/** \ingroup ErrorCodeRanges
    \defgroup ReturnValues Common return values

	These return values are the default return values from functions that can have
	multiple different outcomes. Especially working with streams these are heavily used
@{*/
/** Return value indicating success */
#define ANOTO_OK                    0
/** Return value indicating error       */
#define ANOTO_ERROR					-1
/** Return value indicating an out of memory situation*/
#define ANOTO_OUT_OF_MEMORY			-2
/** Return value indicating a parse error */
#define ANOTO_PARSE_ERROR			-3
/** Return value indicating that an non open handle was used*/
#define ANOTO_NOT_OPEN				-4
/** Return value indicating that we tried to open an already open handle*/
#define ANOTO_ALREADY_OPEN			-5
/** Return value indicating that we tried to open a read handle for writing or a write handle for reading   */
#define ANOTO_ILLEGAL_MODE			-6
/** Return value indicating that we tried to use NULL as a handle */
#define ANOTO_ILLEGAL_ARGUMENT		-7
/** Return value indicating that data ended (not necessarily an error)*/
#define ANOTO_EOF					-8
/** Trying to write items in incorrect order */
#define ANOTO_OUT_OF_SEQUENCE       -9
/** The current item does not have the right type */
#define ANOTO_WRONG_TYPE			-10
/** An unexpected error has occurred */
#define ANOTO_UNEXPECTED_ERROR		-11
/** The current item already exists */
#define ANOTO_ALREADY_EXIST			-12
/** The current item can not be found */
#define ANOTO_NOT_FOUND			-13
/** Return value indicating failed precondition (e.g. trying to delete non-empty folder) */
#define ANOTO_PRECONDITION_FAILED   -14
/** The version of the current item is not supported */
#define ANOTO_INVALID_VERSION		-15
/** Return value indicating a generic IO error */
#define ANOTO_IO_ERROR              -100
/** Return value indicating a premature end of data */
#define ANOTO_EOF_ERROR             -101
  

// seek defines
#define ANOTO_SEEK_BEG  0
#define ANOTO_SEEK_CUR  1
#define ANOTO_SEEK_END  2

/** \ingroup File
 * Prototype for seek callback function.
 *
 * @param[in]	offset		the offset, can be negative.
 * @param[in]	lOrigin		the starting point. Can be either of: ANOTO_SEEK_BEG (Beginning of stream),
 *							ANOTO_SEEK_CUR (Current position of stream), ANOTO_SEEK_END (end of stream) 
 * @param[in]	readHandle	a pointer to custom data that is interpreted by the read
 *							function e.g. a file handle of a file to read from. 
 *							The read handle is supplied by caller when registering 
 *							the read function.
 *
 * @retval ANOTO_OK			Success
 * @retval ANOTO_ERROR		Failure
 */
typedef s32 AnotoSeekFunc(s32 offset, s32 lOrigin, void* readHandle);

/** \ingroup File
 * Prototype for read callback function used by various functions.
 * 
 * @param[out]	buffer		will contain the read data when the function returns.
 * @param[in]	bufferSize	maximum number of bytes that can be written to buffer.
 * @param[in]	readHandle	a pointer to custom data that is interpreted by the read
 *							function e.g. a file handle of a file to read from. 
 *							The read handle is supplied by caller when registering 
 *							the read function.
 *
 * @retval >0				postive return values are used to indicate the number of
 *							bytes written to the buffer. 
 * @retval 0				0 is used to indicate that no more data exists to read.
 * @retval <0				negative return values are used to indicate errors. Errors
 *							are propagated unchanged through the API to the caller. 
 *							You can therefore define your own custom error codes and handle
 *							them in the calling code.
 * @retval ANOTO_IO_ERROR	pre-defined error code indicating a generic IO error
 * @retval ANOTO_EOF_ERROR	pre-defined error code indicating a premature end of data
 */
typedef s32 AnotoReadFunc(char *buffer, s32 bufferSize, void* readHandle);

/** \ingroup File
 * Prototype for write callback function used by various functions.
 *
 * @param[in]	buffer		contains the data to write
 * @param[in]	dataSize	the number of bytes to write from buffer
 * @param[in]	writeHandle	a pointer to custom data that is interpreted by the write
 *							function e.g. a handle of a socket to write to.
 *							The write handle is supplied by caller when registering 
 *							the write function.
 *
 * @retval >0				postive return values are used to indicate the number of
 *							bytes sucessfully written.
 * @retval <0				negative return values are used to indicate errors. Errors
 *							are propagated unchanged through the API to the caller. 
 *							You can therefore define your own custom error codes and handle
 *							them in the calling code.
 * @retval ANOTO_IO_ERROR	pre-defined error code indicating a generic IO error
 */
typedef s32 AnotoWriteFunc(char *buffer, s32 dataSize, void* writeHandle);



/*@}*/

#ifdef __cplusplus
}
#endif

#endif // anotocommon_h
