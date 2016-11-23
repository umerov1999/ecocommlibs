/*****************************************************************************
 * Copyright (c) Anoto AB 2007-2009.
 * All rights reserved. This code is Anoto proprietary and confidential, and
 * may be protected by patents. Any use of the code for whatever purpose is
 * subject to separate license terms and conditions.
 ************************************************************************V30*/

// original author Stefan Lynggaard

#ifndef AFDPRINT_H
#define AFDPRINT_H

// Common includes
#include <os/typedefs.h>
#include <common/anotocommon.h>

// Include to set dependency in wrapper generation.
#include <licensemanager/license/lmlicense.h>

#ifdef __cplusplus
extern "C"
{
#endif

// Print API -  win 32

/** \addtogroup PrintAPI
@{*/

   	 // Creates and destroys a printer object
	EXTAPI_PRINT_H struct PrinterList *afdPrinterListCreate( void );
	EXTAPI_PRINT_H void afdPrinterListDestroy( struct PrinterList **iptPrinters  );

	// gets number of installed printers of system
	EXTAPI_PRINT_H s32 afdPrintGetNumberofPrinters( struct PrinterList *iptPrinters  );

	// get index of default printer in system
	EXTAPI_PRINT_H s32 afdPrintGetDefaultPrinter( struct PrinterList *iptPrinters  );

	// get a reference to a printer name (read only ofcourse)
	EXTAPI_PRINT_H const char *afdPrintGetNameReference( struct PrinterList *iptPrinters, s32 ilPrinterIndex);

	// get a printer name
	EXTAPI_PRINT_H boolean afdGetPrinterName(struct PrinterList *iptPrinters, s32 ilPrinterIndex, char *opPrintName );

	// TRUE = printer is qualified, FALSE = printer is not qualified
	EXTAPI_PRINT_H boolean afdPrintIsQualified(struct PrinterList *iptPrinters, s32 ilPrinterIndex, char *ipPathToSettingsFile);

	// TRUE = Printer has a profile and it is returned in opPrintProfileName
	EXTAPI_PRINT_H boolean afdGetPrinterProfileName(struct PrinterList *iptPrinters, s32 ilPrinterIndex, char *opPrintProfileName, char *ipPathToSettingsFile );

	// TRUE = printer has the specified pdl support, FALSE = has not support for the pdl
	EXTAPI_PRINT_H boolean afdPrintHasPDLSupport(s32 ilPDL, s32 ilPrinterIndex, s32 *oplErrorCode);

	// prints file -1 in ilNumberofCopies means all copies. Remember quotes around optional params with space!
	EXTAPI_PRINT_H s32 afdPrint( const char *iAFDfilename,s32 ilNumberofCopies, s32 ilPrinterIndex, 
		const char *iOptionalparams, struct License **ipaLicenses, s32 ilNbrOfLicenses  );

	// prints file -1,-1 in start/endcopy means all copies. 
	EXTAPI_PRINT_H s32 afdPrintSpecified( const char *iAFDfilename,s32 ilStartPage, s32 ilEndPage, s32 ilStartCopy, s32 ilEndCopy,
		s32 ilPrinterIndex, const char *iOptionalparams, struct License **ipaLicenses, s32 ilNbrOfLicenses  );


	// --------  PS API -------- 

	EXTAPI_PRINT_H s32 afdPrintGeneratePostscript( const char *iAFDfilename,s32 ilStartPage, s32 ilEndPage, s32 ilStartCopy, 
		s32 ilEndCopy, const char *iPostscriptfilename, const char *iOptionalparams, struct License **ipaLicenses, s32 ilNbrOfLicenses );
	
	EXTAPI_PRINT_H s32 afdPrintGeneratePDF( const char *iAFDfilename ,s32 ilStartPage, s32 ilEndPage, s32 ilStartCopy, 
		s32 ilEndCopy, const char *iPDFfilename, const char *iOptionalparams, struct License **ipaLicenses, s32 ilNbrOfLicenses );

	EXTAPI_PRINT_H s32 afdPrintGenerateEPS( const char *iAFDfilename, s32 ilPage, s32 ilCopy, 
											  const char *iEPSOutputFilename, const char *iOptionalparams,
											   struct License **ipaLicenses, s32 ilNbrOfLicenses);


	// -------- Background API -------- 

	EXTAPI_PRINT_H s32 afdPrintGenerateBackgroundImages( const char *iAFDfilename, const char *iXmldestination, 
		const char *iTargetpath, const char *iTargeturl, const char *iOptionalparams, float ifRes1, float ifRes2, 
		float ifRes3, struct License **ipaLicenses, s32 ilNbrOfLicenses );

	EXTAPI_PRINT_H s32 afdPrintGenerateBackgroundImagesVerbose( const char *iAFDfilename, s32 ilStartPage, s32 ilEndPage, 
		const char *iXmldestination, const char *iTargetpath, const char *iTargeturl, 
		const char *iOptionalparams, float ifRes1, float ifRes2, 
		float ifRes3, struct License **ipaLicenses, s32 ilNbrOfLicenses );
	
	EXTAPI_PRINT_H s32 afdPrintGenerateAndEmbedBackgroundImages( const char *iAFDfilename, const char *iOptionalparams, 
						float ifRes1, float ifRes2, float ifRes3, 
						struct License **ipaLicenses, s32 ilNbrOfLicenses );

	EXTAPI_PRINT_H s32 afdPrintGenerateAndEmbedBackgroundImagesVerbose( const char *iAFDfilename,s32 ilStartPage, s32 ilEndPage,
						const char *iOptionalparams, float ifRes1, float ifRes2, float ifRes3, 
						struct License **ipaLicenses, s32 ilNbrOfLicenses , 
						boolean ieIncludePageInstancesBgs, boolean ieDeleteOldFiles);

	EXTAPI_PRINT_H boolean afdPrintGetModuleVersion(char *opVersionBuf,  s32 ilBufSize);



/*@}*/

#ifdef __cplusplus
}
#endif



#endif
