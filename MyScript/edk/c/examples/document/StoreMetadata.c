/**
 * The `MetadataStore` example code shows you how to store metadata in a
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
#define DOCUMENT_COPY_FILE  TEMPORARY_DIRECTORY "/example1_copy.notes"


struct DocumentAsyncCommandResult
{
  bool success;
  voEngineObject object;
};

// Asynchronous saving callback (called at completion)
static void completionCallback(voEngine engine, voDocumentAsyncCommand cmd, bool success, voDocumentAsyncCommandResult result, void* userParam)
{
  struct DocumentAsyncCommandResult* cmdResult = (struct DocumentAsyncCommandResult*)userParam;

  if (cmdResult != NULL)
  {
    cmdResult->success = success;
  }
}

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
  char documentCopyFileNameBuffer[PATH_MAX];
  voString temporaryDirectory;
  voString documentFileName;
  voString documentCopyFileName;

  struct DocumentAsyncCommandResult aSyncResult = { false, NULL };

  voDocument document = NULL;
  voJson documentMetadata = NULL;

  const voString documentKeyAuthor = voString_INITIALIZER("author");
  const voString documentStringAuthor = voString_INITIALIZER("MyScript");
  const voString pageKeyDesc = voString_INITIALIZER("description");
  voJson documentValueAuthor = NULL;

  int pageCount = 0;
  int pageId;

  // -- initialization ---------------------------------------------------------
  if (!loadMyScriptEngine(MYSCRIPT_ENGINE_LIBRARY_DIR, NULL))
    return (fprintf(stderr, "failed to load the MyScriptEngine library\n"), EXIT_FAILURE);

  resolvePath(documentFileNameBuffer, PATH_MAX, DOCUMENT_FILE);
  documentFileName.bytes = documentFileNameBuffer;
  documentFileName.byteCount = strlen(documentFileName.bytes);

  resolvePath(documentCopyFileNameBuffer, PATH_MAX, DOCUMENT_COPY_FILE);
  documentCopyFileName.bytes = documentCopyFileNameBuffer;
  documentCopyFileName.byteCount = strlen(documentCopyFileName.bytes);

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

  printf("3) Saving a copy of the Document\n");
  aSyncResult.success = false;
  aSyncResult.object = NULL;
  verify(VO_IFACE(engine, IDocument)->saveAs(engine, document, NULL, &documentCopyFileName, completionCallback, &aSyncResult));
  verify(VO_IFACE(engine, IDocument)->sync(engine, document));
  verify(aSyncResult.success);
  printf(" . ");
  printPath(stdout, documentCopyFileName.bytes);
  printf(" saved successfully\n");
  printf("\n");

  printf("4) closing the Document\n");
  verify(voReleaseObject(engine, document));
  printf(" . Document closed successfully\n");
  printf("\n");

  printf("5) loading the copy of the Document\n");
  document = voLoadObjectFromFile(engine, NULL, &documentCopyFileName, NULL);
  verify(document != NULL);
  verify(VO_IFACE(engine, IDocument)->setTemporaryDirectory(engine, document, NULL, &temporaryDirectory));
  printf(" . ");
  printPath(stdout, documentCopyFileName.bytes);
  printf(" loaded successfully\n");
  printf("\n");

  printf("6) getting the metadata of the Document\n");
  aSyncResult.success = false;
  aSyncResult.object = NULL;
  verify(VO_IFACE(engine, IDocument)->getMetadata(engine, document, metadataCompletionCallback, &aSyncResult));
  verify(VO_IFACE(engine, IDocument)->sync(engine, document));
  verify(aSyncResult.success);
  verify(aSyncResult.object != NULL);
  documentMetadata = (voJson)(aSyncResult.object);
  printf(" . Document's metadata loaded successfully\n");
  printf("\n");

  printf("7) trying to get the author of the Document from its metadata\n");
  documentValueAuthor = VO_IFACE(engine, IJson)->getObjectEntryValue(engine, documentMetadata, NULL, &documentKeyAuthor);
  verify(documentValueAuthor == NULL);
  if (documentValueAuthor == NULL)
    printf(" . Document has no 'author' metadata\n");
  printf("\n");

  printf("8) adding the author of the Document in its metadata\n");
  documentValueAuthor = VO_IFACE(engine, IJson)->createString(engine, NULL, &documentStringAuthor);
  verify(documentValueAuthor != NULL);
  verify(true == VO_IFACE(engine, IJson)->putObjectEntry(engine, documentMetadata, NULL, &documentKeyAuthor, documentValueAuthor));

  aSyncResult.success = false;
  aSyncResult.object = NULL;
  verify(VO_IFACE(engine, IDocument)->setMetadata(engine, document, documentMetadata, metadataCompletionCallback, &aSyncResult));
  verify(VO_IFACE(engine, IDocument)->sync(engine, document));
  verify(aSyncResult.success);
  printf(" . Document's metadata updated successfully\n");
  printf("\n");

  printf("9) updating all the pages' metadata\n");
  pageCount = VO_IFACE(engine, IDocument)->getPageCount(engine, document);
  printf(" . the Document contains %d page%s\n", pageCount, ((pageCount > 1) ? "s" : ""));

  for (pageId = 0; pageId < pageCount; ++pageId)
  {
    voPage page = NULL;
    voJson pageMetadata = NULL;
    char pageBufferDesc[32];
    voString pageStringDesc;
    voJson pageValueDesc = NULL;

    page = VO_IFACE(engine, IDocument)->getPageAt(engine, document, pageId);
    verify(VO_IFACE(engine, IDocument)->sync(engine, document));
    verify(page != NULL);
    printf("   . page %d\n", pageId);

    aSyncResult.success = false;
    aSyncResult.object = NULL;
    verify(VO_IFACE(engine, IPage)->getMetadata(engine, page, metadataCompletionCallback, &aSyncResult));
    verify(VO_IFACE(engine, IPage)->sync(engine, page));
    verify(aSyncResult.success);
    verify(aSyncResult.object != NULL);
    pageMetadata = (voJson)(aSyncResult.object);
    printf("     . metadata loaded successfully\n");

    sprintf(pageBufferDesc, "This is page #%d", pageId);
    pageStringDesc.bytes = pageBufferDesc;
    pageStringDesc.byteCount = VO_ZERO_ENDED;
    pageValueDesc = VO_IFACE(engine, IJson)->createString(engine, NULL, &pageStringDesc);
    verify(pageValueDesc != NULL);
    verify(true == VO_IFACE(engine, IJson)->putObjectEntry(engine, pageMetadata, NULL, &pageKeyDesc, pageValueDesc));

    aSyncResult.success = false;
    aSyncResult.object = NULL;
    verify(VO_IFACE(engine, IPage)->setMetadata(engine, page, pageMetadata, metadataCompletionCallback, &aSyncResult));
    verify(VO_IFACE(engine, IPage)->sync(engine, page));
    verify(aSyncResult.success);
    printf("     . page metadata updated successfully\n");
    printf("\n");

    verify(voReleaseObject(engine, pageMetadata));
    verify(voReleaseObject(engine, page));
  }

  printf("10) Saving the Document\n");
  aSyncResult.success = false;
  aSyncResult.object = NULL;
  verify(VO_IFACE(engine, IDocument)->save(engine, document, completionCallback, &aSyncResult));
  verify(VO_IFACE(engine, IDocument)->sync(engine, document));
  verify(aSyncResult.success);
  printf(" . Document saved successfully\n");
  printf("\n");

  printf("11) cleaning-up: releasing all created objects and destroying the Engine\n");
  verify(voReleaseObject(engine, documentValueAuthor));
  verify(voReleaseObject(engine, documentMetadata));
  verify(voReleaseObject(engine, document));
  verify(voDestroyEngine(engine));
  printf(" . clean-up done successfully\n");
  printf("\n");

  printf("end of example:\n");
  printf(" you just learnt how to store metadata to a document\n");

  // -- finalization -----------------------------------------------------------
  unloadMyScriptEngine();

  return EXIT_SUCCESS;
}
