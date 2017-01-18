/**
 * The `ProcessMathRecognition` example code shows you how to perform an
 * math recognition on a document.
 */


// -- configuration ------------------------------------------------------------

// common configuration parameters
#include "../common/config.h"

// include the MyScript Engine API header(s)
#include <c/MyScriptEngine.h>
#include <c/MyScriptDocument.h>

#include <c/MyScriptMath.h>
#include <../src/MyScriptMath-parseTreeToLaTeX.h>

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
  voPage page1 = NULL;
  voContent content1 = NULL;

  const voString mathFieldId = voString_INITIALIZER("myMathField");
  voContentField mathField = NULL;

  const voCircle mathCircle = { 750.0f, 450.0f, 350.0f };
  voPageSelection mathSelection = NULL;

  voMathRecognizer mathRecognizer = NULL;
  voMathAlphabetKnowledge mathAlphabetKnowledge = NULL;
  voMathGrammar mathGrammarKnowledge = NULL;

  voMathNode mathRoot = NULL;

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

  printf("3) getting the second page of the Document and its content\n");
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

  printf("4) adding a new math field to the content of the second page\n");
  mathField = VO_IFACE(engine, IContent)->addNewField(engine, content1, VO_CONTENT_FIELD_MATH, NULL, &mathFieldId);
  verify(mathField != NULL);
  printf(" . math field added successfully\n");
  printf("\n");

  printf("5) selecting a circular part of the second page\n");
  mathSelection = voCreatePageSelection(engine, page1);
  verify(mathSelection != NULL);
  verify(VO_IFACE(engine, ISelection)->selectCircle(engine, mathSelection, &mathCircle, VO_SELECT_SET));
  printf(" . selection done successfully\n");
  printf("\n");

  printf("6) adding the selection to the math field\n");
  verify(VO_IFACE(engine, IContentField)->addItems(engine, mathField, mathSelection));
  printf(" . selection added to the math field successfully\n");
  printf("\n");

  printf("7) creating and configuring a new MathRecognizer\n");
  mathRecognizer = voCreateObject(engine, VO_MathRecognizer);
  verify(mathRecognizer != NULL);
  printf(" . MathRecognizer created successfully\n");
  mathAlphabetKnowledge = loadEngineObject(MATH_AK_RES);
  verify(mathAlphabetKnowledge != NULL);
  printf(" . MathAlphabetKnowledge loaded successfully\n");
  mathGrammarKnowledge = loadEngineObject(MATH_GRM_RES);
  verify(mathGrammarKnowledge != NULL);
  printf(" . MathGrammar loaded successfully\n");
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, mathRecognizer, mathAlphabetKnowledge));
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, mathRecognizer, mathGrammarKnowledge));
  printf(" . MathAlphabetKnowledge and MathGrammar resources attached to the recognizer successfully\n");
  printf("\n");

  printf("8) performing a recognition on the selection\n");
  initializeTextUIProgressBar(&progressBar);
  verify(VO_IFACE(engine, IContentFieldProcessor)->process(engine, mathRecognizer, mathField, updateTextUIProgressBar, &progressBar));
  finalizeTextUIProgressBar(&progressBar);
  printf(" . recognition performed successfully\n");
  printf("\n");

  printf("9) retrieving the recognition result\n");
  mathRoot = VO_IFACE(engine, IContentField)->getResult(engine, mathField);
  verify(mathRoot != NULL);
  printf(" . result retrieved successfully\n");
  {
    int length = parseTreeToLaTeX(engine, mathRoot, NULL, 0);

    if (length != -1)
    {
      char* str = malloc(sizeof(char) * (length + 1));

      if (str != NULL)
      {
        parseTreeToLaTeX(engine, mathRoot, str, length);
        str[length] = '\0';
        printf(" . math recognized as LaTeX string: \n\n%s\n", str);
        free(str);
      }
    }
  }
  printf("\n");

  printf("10) cleaning-up: releasing all created objects and destroying the Engine\n");
  verify(voReleaseObject(engine, mathRoot));
  verify(voReleaseObject(engine, mathRecognizer));
  verify(voReleaseObject(engine, mathGrammarKnowledge));
  verify(voReleaseObject(engine, mathAlphabetKnowledge));
  verify(voReleaseObject(engine, mathSelection));
  verify(voReleaseObject(engine, mathField));

  verify(voReleaseObject(engine, content1));
  verify(voReleaseObject(engine, page1));
  verify(voReleaseObject(engine, document));
  verify(voDestroyEngine(engine));
  printf(" . clean-up done successfully\n");
  printf("\n");

  printf("end of example:\n");
  printf(" you just learnt how to perform an math recognition on a document\n");

  // -- finalization -----------------------------------------------------------
  unloadMyScriptEngine();

  return EXIT_SUCCESS;
}
