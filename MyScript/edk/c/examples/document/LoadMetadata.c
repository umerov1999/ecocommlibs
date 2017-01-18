/**
 * The `MetadataLoad` example code shows you how to load metadata from a
 * document.
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
#define DOCUMENT_FILE       DATA_DIR "/example1.notes"


struct DocumentAsyncCommandResult
{
  bool success;
  voEngineObject object;
};

// Asynchronous metadata callback (called at completion)
static void metadataCompletionCallback(voEngine engine, voDocumentAsyncCommand cmd, bool success, voDocumentAsyncCommandResult result, void* userParam)
{
  struct DocumentAsyncCommandResult* cmdResult = (struct DocumentAsyncCommandResult*)userParam;

  if (cmdResult != NULL)
  {
    cmdResult->success = success;
    cmdResult->object = (success ? voAcquireObject(engine, result.object) : NULL);
  }
}

// main function
int main()
{
  char temporaryDirectoryBuffer[PATH_MAX];
  char documentFileNameBuffer[PATH_MAX];
  voString temporaryDirectory;
  voString documentFileName;

  struct DocumentAsyncCommandResult aSyncResult = { false, NULL };

  voDocument document = NULL;
  voJson documentMetadata = NULL;

  const voString documentKeyVersion = voString_INITIALIZER("format-version");
  voString documentStringVersion;
  voJson documentValueVersion = NULL;

  int pageCount = 0;
  int pageId;

  // -- initialization ---------------------------------------------------------
  if (!loadMyScriptEngine(MYSCRIPT_ENGINE_LIBRARY_DIR, NULL))
    return (fprintf(stderr, "failed to load the MyScriptEngine library\n"), EXIT_FAILURE);

  resolvePath(documentFileNameBuffer, PATH_MAX, DOCUMENT_FILE);
  documentFileName.bytes = documentFileNameBuffer;
  documentFileName.byteCount = strlen(documentFileName.bytes);

  resolvePath(temporaryDirectoryBuffer, PATH_MAX, TEMPORARY_DIRECTORY);
  temporaryDirectory.bytes = temporaryDirectoryBuffer;
  temporaryDirectory.byteCount = strlen(temporaryDirectory.bytes);

  // -- example code -----------------------------------------------------------
  printf("1) creating a new Engine initialized with your certificate\n");
  engine = voCreateEngine(VO_MSE_VER, &myCertificate, NULL);
  verify(engine != NULL);
  printf(" . Engine created successfully\n");
  printf("\n");

  printf("2) loading an existing Document (use default charset)\n");
  document = voLoadObjectFromFile(engine, NULL, &documentFileName, NULL);
  verify(document != NULL);
  verify(VO_IFACE(engine, IDocument)->setTemporaryDirectory(engine, document, NULL, &temporaryDirectory));
  printf(" . ");
  printPath(stdout, documentFileName.bytes);
  printf(" loaded successfully\n");
  printf("\n");

  printf("3) getting the metadata of the Document\n");
  aSyncResult.success = false;
  aSyncResult.object = NULL;
  verify(VO_IFACE(engine, IDocument)->getMetadata(engine, document, metadataCompletionCallback, &aSyncResult));
  verify(VO_IFACE(engine, IDocument)->sync(engine, document));
  verify(aSyncResult.success);
  verify(aSyncResult.object != NULL);
  documentMetadata = (voJson)(aSyncResult.object);
  printf(" . Document's metadata loaded successfully\n");
  printf("\n");

  printf("4) getting the version of the Document from its metadata\n");
  documentValueVersion = VO_IFACE(engine, IJson)->getObjectEntryValue(engine, documentMetadata, NULL, &documentKeyVersion);
  verify(documentValueVersion != NULL);

  documentStringVersion.bytes = NULL;
  documentStringVersion.byteCount = 0;
  verify(true == VO_IFACE(engine, IJson)->getStringValue(engine, documentValueVersion, NULL, &documentStringVersion));
  documentStringVersion.bytes = (char*) malloc(documentStringVersion.byteCount + 1);
  verify(documentStringVersion.bytes != NULL);

  verify(true == VO_IFACE(engine, IJson)->getStringValue(engine, documentValueVersion, NULL, &documentStringVersion));
  documentStringVersion.bytes[documentStringVersion.byteCount] = '\0';
  printf(" . Document's version = %s\n", documentStringVersion.bytes);
  free(documentStringVersion.bytes);
  printf("\n");

  printf("5) getting the number of pages of the Document\n");
  pageCount = VO_IFACE(engine, IDocument)->getPageCount(engine, document);
  verify(pageCount == 2);
  printf(" . the Document contains %d page%s\n", pageCount, ((pageCount > 1) ? "s" : ""));
  printf("\n");

  printf("6) getting all the pages' metadata\n");
  for (pageId = 0; pageId < pageCount; ++pageId)
  {
    voPage page = NULL;
    voJson pageMetadata = NULL;
    voJsonObjectIterator iterator = NULL;

    page = VO_IFACE(engine, IDocument)->getPageAt(engine, document, pageId);
    verify(VO_IFACE(engine, IDocument)->sync(engine, document));
    verify(page != NULL);
    printf(" . page %d\n", pageId);

    aSyncResult.success = false;
    aSyncResult.object = NULL;
    verify(VO_IFACE(engine, IPage)->getMetadata(engine, page, metadataCompletionCallback, &aSyncResult));
    verify(VO_IFACE(engine, IPage)->sync(engine, page));
    verify(aSyncResult.success);
    verify(aSyncResult.object != NULL);
    pageMetadata = (voJson)(aSyncResult.object);
    printf("   . metadata loaded successfully\n");

    iterator = VO_IFACE(engine, IJson)->getObjectEntries(engine, pageMetadata);
    verify(NULL != iterator);

    while (VO_NO == VO_IFACE(engine, IJsonObjectIterator)->isAtEnd(engine, iterator))
    {
      voString keyString = { NULL, 0 };
      voString valueString = { NULL, 0 };
      voJson value;

      VO_IFACE(engine, IJsonObjectIterator)->getKey(engine, iterator, NULL, &keyString);
      keyString.bytes = malloc(keyString.byteCount + 1);
      verify(keyString.bytes != NULL);

      VO_IFACE(engine, IJsonObjectIterator)->getKey(engine, iterator, NULL, &keyString);
      keyString.bytes[keyString.byteCount] = '\0';

      value = VO_IFACE(engine, IJsonObjectIterator)->getValue(engine, iterator);

      if (VO_JSON_STRING == VO_IFACE(engine, IJson)->getType(engine, value))
      {
        VO_IFACE(engine, IJson)->getStringValue(engine, value, NULL, &valueString);
        valueString.bytes = (char*) malloc(valueString.byteCount + 1);
        verify(valueString.bytes != NULL);

        VO_IFACE(engine, IJson)->getStringValue(engine, value, NULL, &valueString);
        valueString.bytes[valueString.byteCount] = '\0';

        printf("    . %s = %s\n", keyString.bytes, valueString.bytes);
        free(valueString.bytes);
      }

      voReleaseObject(engine, value);
      free(keyString.bytes);

      verify(true == VO_IFACE(engine, IJsonObjectIterator)->next(engine, iterator));
    }

    verify(voReleaseObject(engine, iterator));
    verify(voReleaseObject(engine, pageMetadata));
    verify(voReleaseObject(engine, page));
  }
  printf("\n");

  printf("7) cleaning-up: releasing all created objects and destroying the Engine\n");
  verify(voReleaseObject(engine, documentValueVersion));
  verify(voReleaseObject(engine, documentMetadata));
  verify(voReleaseObject(engine, document));
  verify(voDestroyEngine(engine));
  printf(" . clean-up done successfully\n");
  printf("\n");

  printf("end of example:\n");
  printf(" you just learnt how to load metadata from a document\n");

  // -- finalization -----------------------------------------------------------
  unloadMyScriptEngine();

  return EXIT_SUCCESS;
}
