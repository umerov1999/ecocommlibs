/**
 * The `LoadSaveDocument` example code shows you how to perform IO operations on
 * a document (loading and saving).
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
#define TEMPORARY_DIRECTORY  OUTPUT_DIR "/tmp"
#define DOCUMENT_FILE        DATA_DIR "/example1.notes"
#define DOCUMENT_COPY_FILE   TEMPORARY_DIRECTORY "/example1_copy.notes"


struct DocumentAsyncCommandResult
{
  bool success;
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

// main function
int main()
{
  char temporaryDirectoryBuffer[PATH_MAX];
  char documentFileNameBuffer[PATH_MAX];
  char documentCopyFileNameBuffer[PATH_MAX];
  voString temporaryDirectory;
  voString documentFileName;
  voString documentCopyFileName;

  struct DocumentAsyncCommandResult aSyncResult = { false };

  voDocument document = NULL;
  voPage page = NULL;
  int pageCount = 0;

  voDocumentInitializer documentInitializer;
  bool enableAutoSave;

  // -- initialization ---------------------------------------------------------
  if (!loadMyScriptEngine(MYSCRIPT_ENGINE_LIBRARY_DIR, NULL))
    return (fprintf(stderr, "failed to load the MyScriptEngine library\n"), EXIT_FAILURE);

  resolvePath(documentFileNameBuffer, PATH_MAX, DOCUMENT_FILE);
  documentFileName.bytes = documentFileNameBuffer;
  documentFileName.byteCount = VO_ZERO_ENDED;

  resolvePath(documentCopyFileNameBuffer, PATH_MAX, DOCUMENT_COPY_FILE);
  documentCopyFileName.bytes = documentCopyFileNameBuffer;
  documentCopyFileName.byteCount = VO_ZERO_ENDED;

  resolvePath(temporaryDirectoryBuffer, PATH_MAX, TEMPORARY_DIRECTORY);
  temporaryDirectory.bytes = temporaryDirectoryBuffer;
  temporaryDirectory.byteCount = VO_ZERO_ENDED;

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

  printf("3) getting the number of pages of the document\n");
  pageCount = VO_IFACE(engine, IDocument)->getPageCount(engine, document);
  verify(pageCount == 2);
  printf(" . the Document contains %d page(s)\n", pageCount);
  printf("\n");

  printf("4) adding a new page to the Document\n");
  page = VO_IFACE(engine, IDocument)->addPage(engine, document);
  verify(page != NULL);
  pageCount = VO_IFACE(engine, IDocument)->getPageCount(engine, document);
  verify(voReleaseObject(engine, page));
  verify(pageCount == 3);
  printf(" . the Document contains %d page(s)\n", pageCount);
  printf("\n");

  printf("5) saving a copy of the Document\n");
  aSyncResult.success = false;
  verify(VO_IFACE(engine, IDocument)->saveAs(engine, document, NULL, &documentCopyFileName, completionCallback, &aSyncResult));
  verify(VO_IFACE(engine, IDocument)->sync(engine, document));
  verify(aSyncResult.success);
  printf(" . ");
  printPath(stdout, documentCopyFileName.bytes);
  printf(" saved successfully\n");
  printf("\n");

  printf("6) closing the Document\n");
  verify(voReleaseObject(engine, document));
  printf(" . Document closed successfully\n");
  printf("\n");

  printf("7) loading the copy of the Document\n");
  document = voLoadObjectFromFile(engine, NULL, &documentCopyFileName, NULL);
  verify(document != NULL);
  verify(VO_IFACE(engine, IDocument)->setTemporaryDirectory(engine, document, NULL, &temporaryDirectory));
  printf(" . ");
  printPath(stdout, documentCopyFileName.bytes);
  printf(" loaded successfully\n");
  pageCount = VO_IFACE(engine, IDocument)->getPageCount(engine, document);
  verify(pageCount == 3);

  printf(" . the Document contains %d page(s)\n", pageCount);
  printf("\n");

  printf("8) adding a new page to the Document\n");
  page = VO_IFACE(engine, IDocument)->addPage(engine, document);
  verify(page != NULL);
  verify(voReleaseObject(engine, page));
  pageCount = VO_IFACE(engine, IDocument)->getPageCount(engine, document);
  verify(pageCount == 4);
  printf(" . the Document contains %d page(s)\n", pageCount);
  printf("\n");

  printf("9) saving a temporary copy of the Document\n");
  aSyncResult.success = false;
  verify(VO_IFACE(engine, IDocument)->saveToTemp(engine, document, completionCallback, &aSyncResult));
  verify(VO_IFACE(engine, IDocument)->sync(engine, document));
  verify(aSyncResult.success);
  printf(" . temporary document saved successfully\n");
  printf("\n");

  printf("10) discarding changes and closing the Document\n");
  aSyncResult.success = false;
  verify(VO_IFACE(engine, IDocument)->discardChanges(engine, document, completionCallback, &aSyncResult));
  verify(VO_IFACE(engine, IDocument)->sync(engine, document));
  verify(aSyncResult.success);
  verify(voReleaseObject(engine, document));
  printf(" . Document closed successfully\n");
  printf("\n");

  printf("11) re-loading the copy of the Document\n");
  document = voLoadObjectFromFile(engine, NULL, &documentCopyFileName, NULL);
  verify(document != NULL);
  verify(VO_IFACE(engine, IDocument)->setTemporaryDirectory(engine, document, NULL, &temporaryDirectory));
  printf(" . ");
  printPath(stdout, documentCopyFileName.bytes);
  printf(" loaded successfully\n");
  pageCount = VO_IFACE(engine, IDocument)->getPageCount(engine, document);
  verify(pageCount == 3);
  printf(" . the Document contains %d page(s)\n", pageCount);
  printf("\n");

  printf("12) adding a new page in the Document\n");
  page = VO_IFACE(engine, IDocument)->addPage(engine, document);
  verify(page != NULL);
  verify(voReleaseObject(engine, page));
  pageCount = VO_IFACE(engine, IDocument)->getPageCount(engine, document);
  verify(pageCount == 4);
  printf(" . the Document contains %d page(s)\n", pageCount);
  printf("\n");

  printf("13) re-saving the copy of the Document\n");
  aSyncResult.success = false;
  verify(VO_IFACE(engine, IDocument)->save(engine, document, completionCallback, &aSyncResult));
  verify(VO_IFACE(engine, IDocument)->sync(engine, document));
  verify(aSyncResult.success);
  printf(" . Document re-saved successfully\n");
  printf("\n");

  printf("14) adding a new page in the Document\n");
  page = VO_IFACE(engine, IDocument)->addPage(engine, document);
  verify(page != NULL);
  verify(voReleaseObject(engine, page));
  pageCount = VO_IFACE(engine, IDocument)->getPageCount(engine, document);
  verify(pageCount == 5);
  printf(" . the Document contains %d page(s)\n", pageCount);
  printf("\n");

  printf("15) closing the Document\n");
  verify(voReleaseObject(engine, document));
  printf(" . Document closed successfully\n");
  printf("\n");

  printf("16) re-loading the copy of the Document\n");
  document = voLoadObjectFromFile(engine, NULL, &documentCopyFileName, NULL);
  verify(document != NULL);
  verify(VO_IFACE(engine, IDocument)->setTemporaryDirectory(engine, document, NULL, &temporaryDirectory));
  printf(" . ");
  printPath(stdout, documentCopyFileName.bytes);
  printf(" loaded successfully\n");
  pageCount = VO_IFACE(engine, IDocument)->getPageCount(engine, document);
  verify(pageCount == 4);
  printf(" . the Document contains %d page(s)\n", pageCount);
  printf("\n");

  printf("17) testing and enabling the auto save on the Document\n");
  verify(voGetProperty(engine, document, VO_DOCUMENT_AUTO_SAVE, &enableAutoSave, sizeof(enableAutoSave)));
  verify(enableAutoSave == false);
  printf(" . Auto save feature is disabled\n");
  enableAutoSave = true;
  verify(voSetProperty(engine, document, VO_DOCUMENT_AUTO_SAVE, &enableAutoSave, sizeof(enableAutoSave)));
  printf(" . Auto save feature updated successfully\n");
  verify(voGetProperty(engine, document, VO_DOCUMENT_AUTO_SAVE, &enableAutoSave, sizeof(enableAutoSave)));
  verify(enableAutoSave == true);
  printf(" . Auto save feature is enabled\n");
  printf("\n");

  printf("18) adding a new page in the Document\n");
  page = VO_IFACE(engine, IDocument)->addPage(engine, document);
  verify(page != NULL);
  verify(voReleaseObject(engine, page));
  pageCount = VO_IFACE(engine, IDocument)->getPageCount(engine, document);
  verify(pageCount == 5);
  printf(" . the Document contains %d page(s)\n", pageCount);
  printf("\n");

  printf("19) closing the Document\n");
  verify(voReleaseObject(engine, document));
  printf(" . Document closed successfully\n");
  printf(" . Modifications were automatically saved in the temporary directory\n");
  printf("\n");

  printf("20) re-loading the copy of the Document using an initializer to specify the temporary directory\n");
  documentInitializer.fileName.bytes = documentCopyFileName.bytes;
  documentInitializer.fileName.byteCount = documentCopyFileName.byteCount;
  documentInitializer.temporaryDirectoryName.bytes = temporaryDirectory.bytes;
  documentInitializer.temporaryDirectoryName.byteCount = temporaryDirectory.byteCount;
  documentInitializer.charset = NULL;
  documentInitializer.createDirectory = false;
  documentInitializer.overwrite = false;
  printf(" . loading the Document\n");
  document = voCreateObjectEx(engine, VO_Document, &documentInitializer, sizeof(documentInitializer));
  verify(document != NULL);
  printf(" . ");
  printPath(stdout, documentCopyFileName.bytes);
  printf(" loaded successfully\n");
  pageCount = VO_IFACE(engine, IDocument)->getPageCount(engine, document);
  verify(pageCount == 5);
  printf(" . the Document contains %d page(s)\n", pageCount);
  printf("\n");

  printf("21) discarding changes and closing the Document\n");
  aSyncResult.success = false;
  verify(VO_IFACE(engine, IDocument)->discardChanges(engine, document, completionCallback, &aSyncResult));
  verify(VO_IFACE(engine, IDocument)->sync(engine, document));
  verify(aSyncResult.success);
  verify(voReleaseObject(engine, document));
  printf(" . Document closed successfully\n");
  printf("\n");

  printf("22) re-loading the copy of the Document using an initializer to specify the temporary directory\n");
  printf(" . loading the Document\n");
  document = voCreateObjectEx(engine, VO_Document, &documentInitializer, sizeof(documentInitializer));
  verify(document != NULL);
  printf(" . ");
  printPath(stdout, documentCopyFileName.bytes);
  printf(" loaded successfully\n");
  pageCount = VO_IFACE(engine, IDocument)->getPageCount(engine, document);
  verify(pageCount == 4);
  printf(" . the Document contains %d page(s)\n", pageCount);
  printf("\n");

  printf("23) cleaning-up: releasing all created objects and destroying the Engine\n");
  verify(voReleaseObject(engine, document));
  verify(voDestroyEngine(engine));
  printf(" . clean-up done successfully\n");
  printf("\n");

  printf("end of example:\n");
  printf(" you just learnt how to perform IO operations on a document (loading and saving)\n");

  // -- finalization -----------------------------------------------------------
  unloadMyScriptEngine();

  return EXIT_SUCCESS;
}
