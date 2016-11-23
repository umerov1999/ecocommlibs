/*****************************************************************************
 * Copyright (c) Anoto AB 2007-2009.
 * All rights reserved. This code is Anoto proprietary and confidential, and
 * may be protected by patents. Any use of the code for whatever purpose is
 * subject to separate license terms and conditions.
 ************************************************************************V30*/

#ifndef _OS_ASTDIO_H_
#define _OS_ASTDIO_H_

#include <stdarg.h>

#include <os/typedefs.h>
#include <common/anotocommon.h>
#include <common/extapicfg.h>

#ifdef __cplusplus
extern "C" {
#endif

/** \defgroup AFPPortabilityAPI Portability Layer API

The AFP Portability Layer is a platform abstraction library that gives the other AFP modules a platform independent interface to platform specific features they require. It provides an ANSI C like API to file- and directory operations, dynamic memory allocation, mutexes and debug assertions.

The AFP Portability Layer is not intended for general use. It is separated from the other modules primarily to make it easier to replace or update it separately on platforms where special adaptation to the host environment is needed.
*/
/*@{*/

/** Maximum length of a full path name (excluding NUL termination). */
#define MAX_PATH_LENGTH  255

/** Maximum length of a line of input. */
#define MAX_LINE        2048


// Minimal equivalents to some ANSI/POSIX interfaces:

/** Seeking (values are identical to trad. implementations): */

#define ASEEK_SET   0
#define ASEEK_CUR   1
#define ASEEK_END   2

/** Minimal directory entry implementation: */

#define ADIR_UNKNOWN    0
#define ADIR_FILE       1
#define ADIR_DIRECTORY  2
#define ADIR_DEVICE     3

struct aDirEntry
{
	char d_name[ MAX_PATH_LENGTH ];
	u32  d_type;
};

/** Minimal struct stat look-alike. */

#define ASTAT_FILE        0x00008000
#define ASTAT_DIRECTORY   0x00004000
#define ASTAT_DEVICE      0x00002000

#define ASTAT_READ        0x00000100
#define ASTAT_WRITE       0x00000080
#define ASTAT_EXECUTE     0x00000040

/**
 The only field guaranteed to be meaningful in struct aStat is st_Size.
 In particular, st_cTime is NOT EVER guaranteed to be "creation time"
 and so may be of limited use. The st_Mode values for struct aStat are
 defined as their POSIX counter-parts, but note that e.g. WIN32 does not
 define the permission bits at all -- only the type bits.
*/
struct aStat
{
	u32 st_Dev;
	u32 st_Inode;
	u32 st_Mode;
	u32 st_aTime;
	u32 st_mTime;
	u32 st_cTime;
	u32 st_Size;
};

struct _aDir;

/** Opaque, internal data type representing a directory. Actual implementation is platform specific. */
typedef struct _aDir ADIR;

struct _aFILE;

/** Opaque, internal data type representing a file. Actual implementation is platform specific. */
typedef struct _aFILE AFILE;

#if defined(_WIN32) || defined(__APPLE__)
#define aStdin ((AFILE *) stdin)
#define aStdout ((AFILE *) stdout)
#define aStderr ((AFILE *) stderr)
#else
extern AFILE *aStdin;
extern AFILE *aStdout;
extern AFILE *aStderr;
#endif

#if !defined(EOF)
#define	EOF	(-1)
#endif

/**
 * The public astdio interface:
 * NOTE: aFread/aFwrite use and return a single BYTE count,
 * contrary to their ANSI C stdio equivalents!
 */

/**
 * Open/create a file for reading/writing, with a subset of ANSI C "fopen" semantics.
 * Only mode flags "r", "w", "a", "b" and "+" are supported. Creation/Truncation
 * semantics as per ANSI C "fopen".
 *
 * @param[in] izFileName Name of file to open/create/truncate.
 * @param[in] izMode File mode string.
 *
 * @retval "Pointer to an AFILE file descriptor object" Success
 * @retval NULL Failure
 */
EXTAPI_PORTABILITY_H AFILE *aFopen(const char *izFileName, const char *izMode);

/**
 * Read from an AFILE. Like ANSI C "fread", may return a short read count and
 * does not distinguish between end-of-file and read error. Use aFeof() and
 * aFerror() to tell the difference.
 *
 * @param[out] opbBuffer Pointer to location to read into.
 * @param[in]  ilSize The number of bytes to read.
 * @param[in]  iptFile The AFILE descriptor to read from.
 *
 * @retval "Number of bytes successfully read" Success
 * @retval 0 Failure
 * @retval 0 End of file
 */
EXTAPI_PORTABILITY_H s32    aFread(void *opbBuffer, s32 ilSize, AFILE *iptFile);


/**
 * Reads characters from an AFILE. Like ANSI C "fgets".
 * Returns pointer to read characters or NULL, does not distinguish between end-of-file
 * and read error. Use aFeof() and aFerror() to tell the difference.
 *
 * @param[out] opBuffer Pointer to location to read into.
 * @param[in]  ilBufSize Max number of characters to read (usually the length of the input buffer is used).
 * @param[in]  iptFile The AFILE descriptor to read from.
 *
 * @retval "Pointer to the characters read (same as opBuffer)" Success
 * @retval NULL Failure
 * @retval NULL End of file and no characters read.
 */
EXTAPI_PORTABILITY_H char  *aFgets(char *opBuffer, s32 ilBufSize, AFILE *iptFile);

/**
 * Get character from an AFILE. Like ANSI C "fgetc".
 * Returns the read character as an int value or 0 if End-Of-File or failure
 * (use aFeof() and aFerror() to tell the difference).
 *
 * @param[in]  iptFile The AFILE descriptor to read from.
 *
 * @retval "The read character as an int value" Success
 * @retval 0 Failure
 * @retval 0 End of file
 */
EXTAPI_PORTABILITY_H s32    aFgetc(AFILE *iptFile);

/**
 * Write to an AFILE.
 *
 * @param[in] ipbBuffer Pointer to data to write.
 * @param[in] ilSize The number of bytes to write.
 * @param[in] iptFile The AFILE descriptor to write to.
 *
 * @retval "Number of bytes successfully written" Success
 * @retval 0 Failure
 */
EXTAPI_PORTABILITY_H s32    aFwrite(void *ipbBuffer, s32 ilSize, AFILE *iptFile);

/**
 * Print formatted output to aStdout.
 *
 * @param[in] izFormat An ANSI C "printf" format string.
 * @param[in] ... Variable number of arguments matching the supplied format.
 *
 * @retval "The number of characters printed" Success
 * @retval "A negative value" Failure
 */
EXTAPI_PORTABILITY_H s32    aPrintf(const char *izFormat, ... );

/**
 * Print formatted output to an AFILE.
 *
 * @param[in] iptFile The AFILE descriptor to write to.
 * @param[in] izFormat An ANSI C "printf" format string.
 * @param[in] ... Variable number of arguments matching the supplied format.
 *
 * @retval "The number of characters printed" Success
 * @retval "A negative value" Failure
 */
EXTAPI_PORTABILITY_H s32    aFprintf(AFILE *iptFile, const char *izFormat, ... );

/**
 * Print formatted output to an AFILE, stdarg version.
 *
 * @param[in] iptFile The AFILE descriptor to write to.
 * @param[in] izFormat An ANSI C "printf" format string.
 * @param[in] pArgs An initialized variable argument descriptor handle.
 *
 * @retval "The number of characters printed" Success
 * @retval "A negative value" Failure
 */
EXTAPI_PORTABILITY_H s32    aVfprintf(AFILE *iptFile, const char *izFormat, va_list pArgs );

/**
 * Write a character to an AFILE descriptor.
 *
 * @param[in] ilChar The character to write.
 * @param[in] iptFile The AFILE to write to.
 *
 * @retval "The character written" Success
 * @retval ANOTO_ERROR Failure
 */
EXTAPI_PORTABILITY_H s32    aFputc(s32 ilChar, AFILE *iptFile);

/**
 * Write a NUL-terminated string to an AFILE.
 *
 * @param[in] izString The string to write.
 * @param[in] iptFile The AFILE to write to.
 *
 * @retval ANOTO_OK Success
 * @retval ANOTO_ERROR Failure
 */
EXTAPI_PORTABILITY_H s32    aFputs(const char *izString, AFILE *iptFile);

/**
 * Reposition an AFILE descriptor, with ANSI C semantics.
 *
 * @param[in] iptFile The AFILE to resposition.
 * @param[in] ilOffset The new offset, relative to ilOrigin.
 * @param[in] ilOrigin One of ASEEK_SET, ASEEK_CUR and ASEEK_END.
 *
 * @retval ANOTO_OK Success
 * @retval ANOTO_ERROR Failure
 */

EXTAPI_PORTABILITY_H s32    aFseek(AFILE *iptFile, s32 ilOffset, s32 ilOrigin);

/**
 * Obtain the current position of an AFILE descriptor.
 *
 * @param[in] iptFile The AFILE to examine.
 *
 * @retval The AFILE's current file position
 */
EXTAPI_PORTABILITY_H s32    aFtell(AFILE *iptFile);

/**
 * Close an AFILE descriptor, flushing any associated buffers.
 *
 * @param[in] iptFile The AFILE descriptor to close.
 *
 * @retval ANOTO_OK Success
 * @retval ANOTO_ERROR Failure
 */
EXTAPI_PORTABILITY_H s32    aFclose(AFILE *iptFile);

/**
 * Obtain information about the file underlying an AFILE descriptor.
 * See struct aStat for usefulness issues.
 *
 * @param[in] iptFile The AFILE descriptor representing the file.
 * @param[out] optBuf The location to store file information to.
 *
 * @retval ANOTO_OK Success
 * @retval ANOTO_ERROR Failure
 */
EXTAPI_PORTABILITY_H s32    aFstat(AFILE *iptFile, struct aStat *optBuf);

/**
 * Obtain information about a file. See struct aStat for usefulness issues.
 *
 * @param[in] izFileName The name of the file to examine.
 * @param[out] optBuf The location to store file information to.
 *
 * @retval ANOTO_OK Success
 * @retval ANOTO_ERROR Failure
 */
EXTAPI_PORTABILITY_H s32    aStat(const char *izFileName, struct aStat *optBuf);

/**
 * Flush any buffers associated with an AFILE.
 * Like ANSI C "fflush", supplying NULL instead of a valid
 * AFILE descriptor causes all file descriptors to be flushed.
 *
 * @param[in] iptFile The file to flush.
 *
 * @retval ANOTO_OK Success
 * @retval ANOTO_ERROR Failure
 */
EXTAPI_PORTABILITY_H s32    aFflush(AFILE *iptFile);

/**
 * Determine if an AFILE descriptor is positioned at end-of-file.
 *
 * @param[in] iptFile The AFILE to examine.
 *
 * @retval TRUE
 * @retval FALSE
 */
EXTAPI_PORTABILITY_H s32    aFeof(AFILE *iptFile);

/**
 * Query the error state of an AFILE descriptor.
 *
 * @param[in] iptFile The AFILE to examine.
 *
 * @retval TRUE
 * @retval FALSE
 */
EXTAPI_PORTABILITY_H s32    aFerror(AFILE *iptFile);

/**
 * Clear the error and end-of-file flags of an AFILE.
 *
 * @param[in] iptFile The AFILE to clear.
 */
EXTAPI_PORTABILITY_H void   aClearErr(AFILE *iptFile);

/**
 * Remove a file or empty directory.
 * The removal may fail if the user does not have the privileges
 * required to remove the object, or if the name supplied refers
 * to a directory that is not empty.
 *
 * @param[in] izFileName The name of the file or directory to remove.
 *
 * @retval ANOTO_OK Success
 * @retval ANOTO_ERROR Failure
 */
EXTAPI_PORTABILITY_H s32    aRemove(const char *izFileName);

/**
 * Rename a file or directory.
 *
 * @param[in] izFrom The original name.
 * @param[in] izTo The new name.
 *
 * @retval ANOTO_OK Success
 * @retval ANOTO_ERROR Failure
 */
EXTAPI_PORTABILITY_H s32    aRename(const char *izFrom, const char *izTo);

/**
 * Finds the first entry in the named directory, Windows style.
 * Not guaranteed to be thread safe with respect to other directory operations.
 *
 * @param[in]   izDir       Directory to examine.
 * @param[out]  ozFileName  Result buffer. Must be at least MAX_PATH_LENGTH.
 *
 * @retval "A directory iterator handle" Success
 * @retval NULL Failure
 */
EXTAPI_PORTABILITY_H ADIR  *aFindFirstFile(const char *izDir, char *ozFileName);

/**
 * Finds the next file in the specified directory, Windows style.
 * Not guaranteed to be thread safe with respect to other directory operations.
 *
 * @param[in]   iptDirHandle Directory iterator handle.
 * @param[out]  ozFileName   Result buffer. Must be at least MAX_PATH_LENGTH.
 *
 * @retval      ANOTO_OK Success
 * @retval      ANOTO_ERROR Failure or end-of-file
 */
EXTAPI_PORTABILITY_H s32    aFindNextFile(ADIR *iptDirHandle, char *ozFileName);

/**
 * Open a directory path for reading, POSIX style.
 *
 * @param[in] izDir Name of directory to open
 *
 * @retval "A directory iterator handle for use with aReadDir()" Success
 * @retval  NULL Failure
 */
EXTAPI_PORTABILITY_H ADIR  *aOpenDir(const char *izDir);

/**
 * Read the next entry on a directory iterator handle.
 * Not guaranteed to be thread safe with respect to other directory operations.
 *
 * @param[in]   iptDirHandle   Directory iterator handle.
 * @param[out]  optDirEntry    Location to store the directory entry.
 *
 * @retval      ANOTO_OK Success
 * @retval      ANOTO_ERROR Failure or end-of-file
 */
EXTAPI_PORTABILITY_H s32    aReadDir(ADIR *iptDirHandle, struct aDirEntry *optDirEntry);

/**
 * Close and deallocate directory iterator handle.
 *
 * @param[in]   iptDirHandle   Directory iterator handle.
 */
EXTAPI_PORTABILITY_H void   aCloseDir(ADIR *iptDirHandle);

/**
 * Create a directory.
 *
 * @param[in]   izPath       The directory name to create.
 *
 * @retval      ANOTO_OK Success
 * @retval      ANOTO_ERROR Failure
 */
EXTAPI_PORTABILITY_H s32    aMkDir(const char *izPath);

/**
 * Remove a directory.
 *
 * @param[in]   izPath       The directory name to remove.
 *
 * @retval      ANOTO_OK Success
 * @retval      ANOTO_ERROR Failure
 */
EXTAPI_PORTABILITY_H s32    aRmDir(const char *izPath);

/**
 * Change current working directory.
 *
 * @param[in]   izPath       The new current working directory.
 *
 * @retval      ANOTO_OK Success
 * @retval      ANOTO_ERROR Failure
 */
EXTAPI_PORTABILITY_H s32    aChDir(const char *izPath);


/**
 * Get the current working directory.
 *
 * @param[in]   ozBuffer     Buffer to copy working directory into.
 * @param[in]   ilSize       Size of buffer.
 * @retval      ANOTO_OK Success
 * @retval      ANOTO_ERROR Buffer was too small to hold name.
 */
EXTAPI_PORTABILITY_H s32 aGetCwd(char *ozBuffer, s32 ilSize);


/**
 * Creates a directory, including missing parent directories.
 *
 * @param[in] ipPath    The path and dir to create
 *
 * @retval TRUE                         Success
 * @retval FALSE                        Failure
 */
EXTAPI_PORTABILITY_H boolean createDirInc(char* ipPath);

/*@}*/

#ifdef __cplusplus
}
#endif

#endif /* _OS_ASTDIO_H_ */
