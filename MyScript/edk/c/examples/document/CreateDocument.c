/**
 * The `CreateDocument` example code shows you the various ways of creating and
 * loading a MyScript Document.
 */


// -- configuration ------------------------------------------------------------

// common configuration parameters
#include "../common/config.h"

// include the MyScript Engine API header(s)
#include <c/MyScriptEngine.h>
#include <c/MyScriptDocument.h>

// include the certificate
#include "../certificates/MyCertificate.h"


// -- example body -------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>

// global engine (for the sake of readability)
voEngine engine = NULL;

// common utility functions
#include "../common/common.h"

// path to the Document files
// (note that you can rename any notes document as a zip file to watch inside
// it.)
#define TEMPORARY_DIRECTORY OUTPUT_DIR "/tmp"
#define EXISTING_DOCUMENT   DATA_DIR "/example4.notes"
#define NEW_DOCUMENT        OUTPUT_DIR "/new.notes"


// main function
int main()
{
  char temporaryDirectoryBuffer[PATH_MAX];
  char existingDocumentFileNameBuffer[PATH_MAX];
  char newDocumentFileNameBuffer[PATH_MAX];
  voString temporaryDirectory;
  voString existingDocumentFileName;
  voString newDocumentFileName;

  voDocument newDocument = NULL;
  voDocument existingDocument1 = NULL;
  voDocument existingDocument2 = NULL;

  voDocumentInitializer documentInitializer;

  resolvePath(existingDocumentFileNameBuffer, PATH_MAX, EXISTING_DOCUMENT);
  resolvePath(newDocumentFileNameBuffer, PATH_MAX, NEW_DOCUMENT);
  resolvePath(temporaryDirectoryBuffer, PATH_MAX, TEMPORARY_DIRECTORY);

  existingDocumentFileName.bytes = existingDocumentFileNameBuffer;
  existingDocumentFileName.byteCount = VO_ZERO_ENDED;

  newDocumentFileName.bytes = newDocumentFileNameBuffer;
  newDocumentFileName.byteCount = VO_ZERO_ENDED;

  temporaryDirectory.bytes = temporaryDirectoryBuffer;
  temporaryDirectory.byteCount = VO_ZERO_ENDED;

  // -- initialization ---------------------------------------------------------
  if (!loadMyScriptEngine(MYSCRIPT_ENGINE_LIBRARY_DIR, NULL))
    return (fprintf(stderr, "failed to load the MyScriptEngine library\n"), EXIT_FAILURE);

  // -- example code -----------------------------------------------------------
  printf("1) creating a new Engine initialized with your certificate\n");
  engine = voCreateEngine(VO_MSE_VER, &myCertificate, NULL);
  verify(engine != NULL);
  printf(" . Engine created successfully\n");
  printf("\n");

  printf("2) loading an existing Document from a Document initializer\n");
  printf(" . configuring the Document initializer: Document file name\n");
  documentInitializer.fileName.bytes = existingDocumentFileName.bytes;
  documentInitializer.fileName.byteCount = existingDocumentFileName.byteCount;
  printf(" . configuring the Document initializer: temporary directory name\n");
  documentInitializer.temporaryDirectoryName.bytes = temporaryDirectory.bytes;
  documentInitializer.temporaryDirectoryName.byteCount = temporaryDirectory.byteCount;
  printf(" . configuring the Document initializer: charset (use default charset)\n");
  documentInitializer.charset = NULL;
  printf(" . configuring the Document initializer: no need to create file directory as it should already exist\n");
  documentInitializer.createDirectory = false;
  printf(" . configuring the Document initializer: do not overwrite existing document\n");
  documentInitializer.overwrite = false;
  printf(" . loading the Document\n");
  existingDocument1 = voCreateObjectEx(engine, VO_Document, &documentInitializer, sizeof(documentInitializer));
  verify(existingDocument1 != NULL);
  printf(" . Document loaded successfully\n");
  printf("\n");

  printf("3) creating a new Document\n");
  printf(" . creating a Document can be done the same way as when loading a Document\n");
  printf("   with an initializer: if no Document matches the file name specified in the\n");
  printf("   initializer, a new Document is created successfully\n");
  printf(" . configuring the Document initializer: Document file name\n");
  documentInitializer.fileName.bytes = newDocumentFileName.bytes;
  documentInitializer.fileName.byteCount = newDocumentFileName.byteCount;
  printf(" . configuring the Document initializer: temporary directory name\n");
  documentInitializer.temporaryDirectoryName.bytes = temporaryDirectory.bytes;
  documentInitializer.temporaryDirectoryName.byteCount = temporaryDirectory.byteCount;
  printf(" . configuring the Document initializer: charset (use default charset)\n");
  documentInitializer.charset = NULL;
  printf(" . configuring the Document initializer: create file directory if it does not exist\n");
  documentInitializer.createDirectory = true;
  printf(" . configuring the Document initializer: overwrite document if it already exists\n");
  documentInitializer.overwrite = true;
  printf(" . creating the Document\n");
  newDocument = voCreateObjectEx(engine, VO_Document, &documentInitializer, sizeof(documentInitializer));
  verify(newDocument != NULL);
  printf(" . new Document created successfully\n");
  printf("\n");

  printf("4) loading an existing Document with voLoadObjectFromFile()\n");
  printf(" . loading the Document from its file name\n");
  printf("   . if file name is not found, an error is triggered\n");
  existingDocument2 = voLoadObjectFromFile(engine, NULL, &existingDocumentFileName, NULL);
  verify(existingDocument2 != NULL);
  printf(" . setting the Document temporary directory\n");
  verify(VO_IFACE(engine, IDocument)->setTemporaryDirectory(engine, existingDocument2, NULL, &temporaryDirectory));
  printf(" . Document loaded successfully\n");
  printf("\n");

  printf("5) cleaning-up: releasing all created objects and destroying the Engine\n");
  verify(voReleaseObject(engine, existingDocument2));
  verify(voReleaseObject(engine, existingDocument1));
  verify(voReleaseObject(engine, newDocument));
  verify(voDestroyEngine(engine));
  printf(" . clean-up done successfully\n");
  printf("\n");

  printf("end of example:\n");
  printf(" you just learnt how to create and load Documents\n");

  // -- finalization -----------------------------------------------------------
  unloadMyScriptEngine();

  return EXIT_SUCCESS;
}

