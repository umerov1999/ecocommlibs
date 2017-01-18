/**
 * The `ProcessTextRecognition` example code shows you how to perform a text
 * recognition on a document.
 */


// -- configuration ------------------------------------------------------------

// common configuration parameters
#include "../common/config.h"

// include the MyScript Engine API header(s)
#include <c/MyScriptEngine.h>
#include <c/MyScriptDocument.h>

#include <c/MyScriptText.h>

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
  voString temporaryDirectory;
  voString documentFileName;

  struct DocumentAsyncCommandResult aSyncResult = { false, NULL };
  TextUIProgressBar progressBar = { 0 };

  voDocument document = NULL;
  voPage page0 = NULL;
  voContent content0 = NULL;

  const voString textFieldId = voString_INITIALIZER("myTextField");
  voContentField textField = NULL;

  const voRectangle textRectangle = { 100.0f, 300.0f, 1780.0f, 360.0f };
  voPageSelection textSelection = NULL;

  voTextRecognizer textRecognizer = NULL;
  voAlphabetKnowledge alphabetKnowledge = NULL;
  voLinguisticKnowledge linguisticKnowledge = NULL;
  voLinguisticKnowledge lkEnglishSecondary = NULL;

  voRecognitionResult textResult = NULL;
  voCandidateIterator iterator = NULL;

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

  printf("5) selecting a rectangular part of the first page\n");
  textSelection = voCreatePageSelection(engine, page0);
  verify(textSelection != NULL);
  verify(VO_IFACE(engine, ISelection)->selectRectangle(engine, textSelection, &textRectangle, VO_SELECT_SET));
  printf(" . selection done successfully\n");
  printf("\n");

  printf("6) adding selection to the text field\n");
  verify(VO_IFACE(engine, IContentField)->addItems(engine, textField, textSelection));
  printf(" . selection added to the text field successfully\n");
  printf("\n");

  printf("7) creating and configuring a new TextRecognizer\n");
  textRecognizer = voCreateTextRecognizer(engine);
  verify(textRecognizer != NULL);
  printf(" . TextRecognizer created successfully\n");
  alphabetKnowledge = loadEngineObject(AK_CUR_RES);
  verify(alphabetKnowledge != NULL);
  printf(" . AlphabetKnowledge loaded successfully\n");
  linguisticKnowledge = loadEngineObject(LK_TEXT_RES);
  verify(linguisticKnowledge != NULL);
  printf(" . Main LinguisticKnowledge resource loaded successfully\n");
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, textRecognizer, alphabetKnowledge));
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, textRecognizer, linguisticKnowledge));
  verify(voReleaseObject(engine, alphabetKnowledge));
  verify(voReleaseObject(engine, linguisticKnowledge));
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
    verify(VO_IFACE(engine, IAttachTarget)->attach(engine, textRecognizer, lkEnglishSecondary));
    verify(voReleaseObject(engine, lkEnglishSecondary));
  }
  printf(" . AlphabetKnowledge and LinguisticKnowledge resources attached to the recognizer successfully\n");
  printf("\n");


  printf("8) performing a recognition on the selection\n");
  initializeTextUIProgressBar(&progressBar);
  verify(VO_IFACE(engine, IContentFieldProcessor)->process(engine, textRecognizer, textField, updateTextUIProgressBar, &progressBar));
  finalizeTextUIProgressBar(&progressBar);
  printf(" . recognition performed successfully\n");
  printf("\n");

  printf("9) retrieving the recognition result\n");
  textResult = VO_IFACE(engine, IContentField)->getResult(engine, textField);
  verify(textResult != NULL);
  printf(" . result retrieved successfully\n");
  iterator = VO_IFACE(engine, IRecognitionResult)->getCandidates(engine, textResult);
  verify(iterator != NULL);
  if (VO_IFACE(engine, IIterator)->isAtEnd(engine, iterator))
    printf(" . sorry, unable to recognize the input\n");
  else
  {
    voString label = {NULL, 0};
    verify(VO_IFACE(engine, ICandidateIterator)->getLabel(engine, iterator, NULL, &label));
    label.bytes = malloc(label.byteCount + 1);
    verify(VO_IFACE(engine, ICandidateIterator)->getLabel(engine, iterator, NULL, &label));
    label.bytes[label.byteCount] = '\0';
    printf(" . input recognized as: \"%s\"\n", label.bytes);
    free(label.bytes);
  }
  printf("\n");

  printf("10) cleaning-up: releasing all created objects and destroying the Engine\n");
  verify(voReleaseObject(engine, iterator));
  verify(voReleaseObject(engine, textResult));
  verify(voReleaseObject(engine, textRecognizer));
  verify(voReleaseObject(engine, textSelection));
  verify(voReleaseObject(engine, textField));

  verify(voReleaseObject(engine, content0));
  verify(voReleaseObject(engine, page0));
  verify(voReleaseObject(engine, document));
  verify(voDestroyEngine(engine));
  printf(" . clean-up done successfully\n");
  printf("\n");

  printf("end of example:\n");
  printf(" you just learnt how to perform a text recognition on a document\n");

  // -- finalization -----------------------------------------------------------
  unloadMyScriptEngine();

  return EXIT_SUCCESS;
}
