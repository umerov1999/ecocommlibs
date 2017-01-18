/**
 * The `MetadataEdit` example code shows you how to edit  metadata in a
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
  voString tempString = { NULL, 0 };

  voDocument document = NULL;
  voJson documentMetadata = NULL;

  const voString documentKeyAuthor = voString_INITIALIZER("Author");
  const voString documentStringAuthor = voString_INITIALIZER("MyScript");
  voJson documentValueAuthor = NULL;

  const voString documentKeyDesc = voString_INITIALIZER("Description");
  const voString documentStringDesc = voString_INITIALIZER("Edition of metadata in a document");
  voJson documentValueDesc = NULL;

  voPage page = NULL;
  voJson pageMetadata = NULL;
  voJson pageObject = NULL;
  voJson pageArray = NULL;
  voJson pageValue = NULL;

  const voString pageKeyDesc = voString_INITIALIZER("Description");
  const voString pageStringDesc = voString_INITIALIZER("First page of the document");

  const voString pageKeyObject = voString_INITIALIZER("Misc");
  const voString pageKeyNumber = voString_INITIALIZER("Number");
  const voString pageKeyBoolean = voString_INITIALIZER("Boolean");
  const voString pageKeyArray = voString_INITIALIZER("Array");

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

  printf("4) setting the author of the Document in its metadata\n");
  documentValueAuthor = VO_IFACE(engine, IJson)->createString(engine, NULL, &documentStringAuthor);
  verify(documentValueAuthor != NULL);
  verify(true == VO_IFACE(engine, IJson)->putObjectEntry(engine, documentMetadata, NULL, &documentKeyAuthor, documentValueAuthor));
  printf(" . author set successfully\n");
  printf("\n");

  printf("5) setting the description of the Document in its metadata\n");
  documentValueDesc = VO_IFACE(engine, IJson)->createString(engine, NULL, &documentStringDesc);
  verify(documentValueDesc != NULL);
  verify(true == VO_IFACE(engine, IJson)->putObjectEntry(engine, documentMetadata, NULL, &documentKeyDesc, documentValueDesc));
  printf(" . description set successfully\n");
  printf("\n");

  printf("6) removing the description of the Document in its metadata\n");
  verify(voReleaseObject(engine, documentValueDesc));
  documentValueDesc = VO_IFACE(engine, IJson)->getObjectEntryValue(engine, documentMetadata, NULL, &documentKeyDesc);
  verify(documentValueDesc != NULL);
  verify(true == VO_IFACE(engine, IJson)->removeObjectEntry(engine, documentMetadata, NULL, &documentKeyDesc));
  verify(voReleaseObject(engine, documentValueDesc));
  documentValueDesc = VO_IFACE(engine, IJson)->getObjectEntryValue(engine, documentMetadata, NULL, &documentKeyDesc);
  verify(documentValueDesc == NULL);
  printf(" . description removed successfully\n");
  printf("\n");

  printf("7) re-setting the first description of the Document in its metadata\n");
  documentValueDesc = VO_IFACE(engine, IJson)->createString(engine, NULL, &documentStringDesc);
  verify(documentValueDesc != NULL);
  verify(true == VO_IFACE(engine, IJson)->putObjectEntry(engine, documentMetadata, NULL, &documentKeyDesc, documentValueDesc));
  printf(" . description set successfully\n");
  printf("\n");

  printf("8) getting the description of the document from its metadata\n");
  verify(voReleaseObject(engine, documentValueDesc));
  documentValueDesc = VO_IFACE(engine, IJson)->getObjectEntryValue(engine, documentMetadata, NULL, &documentKeyDesc);
  verify(documentValueDesc != NULL);
  verify(VO_JSON_STRING == VO_IFACE(engine, IJson)->getType(engine, documentValueDesc));

  tempString.bytes = NULL;
  tempString.byteCount = 0;
  verify(true == VO_IFACE(engine, IJson)->getStringValue(engine, documentValueDesc, NULL, &tempString));
  tempString.bytes = (char*) malloc(tempString.byteCount + 1);
  verify(tempString.bytes != NULL);

  verify(true == VO_IFACE(engine, IJson)->getStringValue(engine, documentValueDesc, NULL, &tempString));
  tempString.bytes[tempString.byteCount] = '\0';
  printf(" . Document's description = %s\n", tempString.bytes);
  free(tempString.bytes);
  printf("\n");

  printf("9) getting the metadata of the first page of the Document\n");
  page = VO_IFACE(engine, IDocument)->getPageAt(engine, document, 0);
  verify(VO_IFACE(engine, IDocument)->sync(engine, document));
  verify(page != NULL);

  aSyncResult.success = false;
  aSyncResult.object = NULL;
  verify(VO_IFACE(engine, IPage)->getMetadata(engine, page, metadataCompletionCallback, &aSyncResult));
  verify(VO_IFACE(engine, IPage)->sync(engine, page));
  verify(aSyncResult.success);
  verify(aSyncResult.object != NULL);
  pageMetadata = (voJson)(aSyncResult.object);
  printf(" . metadata retrieved successfully\n");
  printf("\n");

  printf("10) setting misc values in the metadata of the first page of the document\n");
  pageValue = VO_IFACE(engine, IJson)->createString(engine, NULL, &pageStringDesc);
  verify(pageValue != NULL);
  verify(true == VO_IFACE(engine, IJson)->putObjectEntry(engine, pageMetadata, NULL, &pageKeyDesc, pageValue));
  verify(voReleaseObject(engine, pageValue));
  printf(" . page description set successfully\n");

  pageObject = VO_IFACE(engine, IJson)->createObject(engine);
  verify(pageObject != NULL);
  verify(true == VO_IFACE(engine, IJson)->putObjectEntry(engine, pageMetadata, NULL, &pageKeyObject, pageObject));
  printf(" . new JSON object added successfully\n");

  pageValue = VO_IFACE(engine, IJson)->createBoolean(engine, true);
  verify(pageValue != NULL);
  verify(true == VO_IFACE(engine, IJson)->putObjectEntry(engine, pageObject, NULL, &pageKeyBoolean, pageValue));
  verify(voReleaseObject(engine, pageValue));
  printf(" . new boolean added successfully\n");

  printf(" . adding an array\n");
  pageArray = VO_IFACE(engine, IJson)->createArray(engine);
  verify(pageArray != NULL);
  {
    pageValue = VO_IFACE(engine, IJson)->createBoolean(engine, false);
    verify(pageValue != NULL);
    verify(true == VO_IFACE(engine, IJson)->insertArrayValueAt(engine, pageArray, -1, pageValue));
    verify(voReleaseObject(engine, pageValue));
    printf("   . boolean added in the array\n");

    pageValue = VO_IFACE(engine, IJson)->createNumber(engine, 987);
    verify(pageValue != NULL);
    verify(true == VO_IFACE(engine, IJson)->insertArrayValueAt(engine, pageArray, -1, pageValue));
    verify(voReleaseObject(engine, pageValue));
    printf("   . number added in the array\n");
  }
  verify(true == VO_IFACE(engine, IJson)->putObjectEntry(engine, pageObject, NULL, &pageKeyArray, pageArray));
  printf(" . array added successfully\n");

  pageValue = VO_IFACE(engine, IJson)->createNumber(engine, 1234.456);
  verify(pageValue != NULL);
  verify(true == VO_IFACE(engine, IJson)->putObjectEntry(engine, pageObject, NULL, &pageKeyNumber, pageValue));
  printf(" . number added successfully\n");

  printf(" . metadata set successfully\n");
  printf("\n");

  printf("11) cleaning-up: releasing all created objects and destroying the Engine\n");
  verify(voReleaseObject(engine, pageValue));
  verify(voReleaseObject(engine, pageArray));
  verify(voReleaseObject(engine, pageObject));
  verify(voReleaseObject(engine, pageMetadata));
  verify(voReleaseObject(engine, page));
  verify(voReleaseObject(engine, documentValueDesc));
  verify(voReleaseObject(engine, documentValueAuthor));
  verify(voReleaseObject(engine, documentMetadata));
  verify(voReleaseObject(engine, document));
  verify(voDestroyEngine(engine));
  printf(" . clean-up done successfully\n");
  printf("\n");

  printf("end of example:\n");
  printf(" you just learnt how to edit metadata of a document\n");

  // -- finalization -----------------------------------------------------------
  unloadMyScriptEngine();

  return EXIT_SUCCESS;
}
