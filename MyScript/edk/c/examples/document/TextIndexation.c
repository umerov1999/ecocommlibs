/**
 * The `ProcessTextIndexation` example code shows you how to perform a text
 * indexation on a document.
 */


// -- configuration ------------------------------------------------------------

// common configuration parameters
#include "../common/config.h"

// include the MyScript Engine API header(s)
#include <c/MyScriptEngine.h>
#include <c/MyScriptDocument.h>

#include <c/MyScriptInkSearch.h>

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
#define DOCUMENT_FILE       DATA_DIR "/example2.notes"
#define DOCUMENT_COPY_FILE  TEMPORARY_DIRECTORY "/example2_copy.notes"


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
    cmdResult->object = NULL;
  }
}

// Asynchronous content callback (called at completion)
static void contentCompletionCallback(voEngine engine, voDocumentAsyncCommand cmd, bool success, voDocumentAsyncCommandResult result, void* userParam)
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
  TextUIProgressBar progressBar = { 0 };

  voDocument document = NULL;
  voPage page0 = NULL;
  voPage page1 = NULL;
  voContent content0 = NULL;
  voContent content1 = NULL;

  const voString textFieldId = voString_INITIALIZER("myTextField");
  voContentField textField = NULL;

  voPageSelection textSelection = NULL;

  voTextIndexer textIndexer = NULL;
  voAlphabetKnowledge alphabetKnowledge = NULL;
  voLinguisticKnowledge linguisticKnowledge = NULL;
  voLinguisticKnowledge lkEnglishSecondary = NULL;

  // -- check required settings have been edited -------------------------------
  if (strlen(LANG) == 0)
  {
    fprintf(stderr, "/!\\ please edit the common/config.h file and set:\n");
    fprintf(stderr, " . the LANG variable\n");
    exit(EXIT_FAILURE);
  }

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

  printf("3) getting the first page of the Document and its content\n");
  page0 = VO_IFACE(engine, IDocument)->getPageAt(engine, document, 0);
  verify(VO_IFACE(engine, IDocument)->sync(engine, document));
  verify(page0 != NULL);
  printf(" . first page retrieved successfully\n");

  aSyncResult.success = false;
  aSyncResult.object = NULL;
  verify(VO_IFACE(engine, IPage)->getContent(engine, page0, contentCompletionCallback, &aSyncResult));
  verify(VO_IFACE(engine, IPage)->sync(engine, page0));
  verify(aSyncResult.success);
  verify(aSyncResult.object != NULL);
  content0 = (voContent)(aSyncResult.object);
  printf(" . first page's content retrieved successfully\n");
  printf("\n");

  printf("4) adding a new text field to the content of the first page\n");
  textField = VO_IFACE(engine, IContent)->addNewField(engine, content0, VO_CONTENT_FIELD_TEXT, NULL, &textFieldId);
  verify(textField != NULL);
  printf(" . text field added successfully\n");
  printf("\n");

  printf("5) selecting all the first page\n");
  textSelection = voCreatePageSelection(engine, page0);
  verify(textSelection != NULL);
  verify(VO_IFACE(engine, ISelection)->selectAll(engine, textSelection));
  printf(" . selection done successfully\n");
  printf("\n");

  printf("6) adding the selection to the text field\n");
  verify(VO_IFACE(engine, IContentField)->addItems(engine, textField, textSelection));
  printf(" . selection added to the text field successfully\n");
  printf("\n");

  printf("7) creating a new TextIndexer\n");
  textIndexer = voCreateObject(engine, VO_TextIndexer);
  verify(textIndexer != NULL);
  printf(" . TextIndexer created successfully\n");
  printf("\n");

  printf("8) configuring the TextIndexer\n");
  alphabetKnowledge = loadEngineObject(AK_CUR_RES);
  verify(alphabetKnowledge != NULL);
  printf(" . AlphabetKnowledge loaded successfully\n");
  linguisticKnowledge = loadEngineObject(LK_TEXT_RES);
  verify(linguisticKnowledge != NULL);
  printf(" . Main LinguisticKnowledge resource loaded successfully\n");
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, textIndexer, alphabetKnowledge));
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, textIndexer, linguisticKnowledge));
  if (strstr(LANG, "en") == NULL)
  {
    const char* path;
    if (strstr(LANG, "zh") != NULL || strstr(LANG, "ja") != NULL || strstr(LANG, "ko") != NULL)
      path = LK_ENGLISH_SECONDARY_2_RES;
    else if (strstr(LANG, "hi") != NULL)
      path = LK_ENGLISH_SECONDARY_3_RES;
    else
      path = LK_ENGLISH_SECONDARY_1_RES;

    lkEnglishSecondary = loadEngineObject(path);
    printf(" . Secondary English LinguisticKnowledge resource loaded successfully\n");
    verify(VO_IFACE(engine, IAttachTarget)->attach(engine, textIndexer, lkEnglishSecondary));
  }
  printf(" . AlphabetKnowledge and LinguisticKnowledge resources attached to the indexer successfully\n");
  printf("\n");

  printf("9) performing an indexation on the selection (first page)\n");
  initializeTextUIProgressBar(&progressBar);
  verify(VO_IFACE(engine, IContentFieldProcessor)->process(engine, textIndexer, textField, updateTextUIProgressBar, &progressBar));
  finalizeTextUIProgressBar(&progressBar);
  printf(" . indexation performed successfully\n");
  printf("\n");

  printf("10) releasing all indexation objects\n");
  verify(voReleaseObject(engine, textIndexer));
  verify(voReleaseObject(engine, textSelection));
  verify(voReleaseObject(engine, textField));
  printf("\n");

  printf("11) getting the second page of the Document and its content\n");
  page1 = VO_IFACE(engine, IDocument)->getPageAt(engine, document, 1);
  verify(VO_IFACE(engine, IDocument)->sync(engine, document));
  verify(page1 != NULL);
  printf(" . second page retrieved successfully\n");

  aSyncResult.success = false;
  aSyncResult.object = NULL;
  verify(VO_IFACE(engine, IPage)->getContent(engine, page1, contentCompletionCallback, &aSyncResult));
  verify(VO_IFACE(engine, IPage)->sync(engine, page1));
  verify(aSyncResult.success);
  verify(aSyncResult.object != NULL);
  content1 = (voContent)(aSyncResult.object);
  printf(" . second page's content retrieved successfully\n");
  printf("\n");

  printf("12) adding a new text field to the content of the second page\n");
  textField = VO_IFACE(engine, IContent)->addNewField(engine, content1, VO_CONTENT_FIELD_TEXT, NULL, &textFieldId);
  verify(textField != NULL);
  printf(" . text field added successfully\n");
  printf("\n");

  printf("13) selecting all the second page\n");
  textSelection = voCreatePageSelection(engine, page1);
  verify(textSelection != NULL);
  verify(VO_IFACE(engine, ISelection)->selectAll(engine, textSelection));
  printf(" . selection done successfully\n");
  printf("\n");

  printf("14) adding the selection to the text field\n");
  verify(VO_IFACE(engine, IContentField)->addItems(engine, textField, textSelection));
  printf(" . selection added to the text field successfully\n");
  printf("\n");

  printf("15) creating and configuring a new TextIndexer\n");
  textIndexer = voCreateObject(engine, VO_TextIndexer);
  verify(textIndexer != NULL);
  printf(" . TextIndexer created successfully\n");
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, textIndexer, alphabetKnowledge));
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, textIndexer, linguisticKnowledge));
  if (lkEnglishSecondary != NULL)
    verify(VO_IFACE(engine, IAttachTarget)->attach(engine, textIndexer, lkEnglishSecondary));
  printf(" . AlphabetKnowledge and LinguisticKnowledge resources attached to the indexer successfully\n");
  printf("\n");

  printf("16) performing an indexation on the selection (second page)\n");
  initializeTextUIProgressBar(&progressBar);
  verify(VO_IFACE(engine, IContentFieldProcessor)->process(engine, textIndexer, textField, updateTextUIProgressBar, &progressBar));
  finalizeTextUIProgressBar(&progressBar);
  printf(" . indexation done successfully\n");
  printf("\n");

  printf("17) releasing all indexation objects\n");
  verify(voReleaseObject(engine, textIndexer));
  verify(voReleaseObject(engine, textSelection));
  verify(voReleaseObject(engine, textField));
  printf("\n");

  printf("18) saving a copy of the Document\n");
  aSyncResult.success = false;
  verify(VO_IFACE(engine, IDocument)->saveAs(engine, document, NULL, &documentCopyFileName, completionCallback, &aSyncResult));
  verify(VO_IFACE(engine, IDocument)->sync(engine, document));
  verify(aSyncResult.success);
  printf(" . ");
  printPath(stdout, documentCopyFileName.bytes);
  printf(" saved successfully\n");
  printf("\n");

  printf("19) cleaning-up: releasing all created objects and destroying the Engine\n");
  verify(voReleaseObject(engine, content1));
  verify(voReleaseObject(engine, content0));
  verify(voReleaseObject(engine, page1));
  verify(voReleaseObject(engine, page0));

  verify(voReleaseObject(engine, document));
  verify(voReleaseObject(engine, alphabetKnowledge));
  verify(voReleaseObject(engine, linguisticKnowledge));
  if (lkEnglishSecondary != NULL)
    verify(voReleaseObject(engine, lkEnglishSecondary));

  verify(voDestroyEngine(engine));
  printf(" . clean-up done successfully\n");
  printf("\n");

  printf("end of example:\n");
  printf(" you just learnt how to perform a text indexation on a document\n");

  // -- finalization -----------------------------------------------------------
  unloadMyScriptEngine();

  return EXIT_SUCCESS;
}
