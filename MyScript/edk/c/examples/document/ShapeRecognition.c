/**
 * The `ProcessShapeRecognition` example code shows you how to perform a shape
 * recognition on a document.
 */


// -- configuration ------------------------------------------------------------

// common configuration parameters
#include "../common/config.h"

// include the MyScript Engine API header(s)
#include <c/MyScriptEngine.h>
#include <c/MyScriptDocument.h>

#include <c/MyScriptShape.h>

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

  const voString shapeFieldId = voString_INITIALIZER("myShapeField");
  voContentField shapeField = NULL;

  const voRectangle shapeRectangle = { 160.0f, 661.0f, 1700.0f, 900.0f };
  voPageSelection shapeSelection = NULL;

  voShapeRecognizer shapeRecognizer = NULL;
  voShapeKnowledge shapeKnowledge = NULL;

  voShapeDocument shapeResult = NULL;
  uint32_t segmentCount;

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

  printf("4) adding a new shape field to the content of the first page\n");
  shapeField = VO_IFACE(engine, IContent)->addNewField(engine, content0, VO_CONTENT_FIELD_SHAPE, NULL, &shapeFieldId);
  verify(shapeField != NULL);
  printf(" . shape field added successfully\n");
  printf("\n");

  printf("5) selecting a rectangular part of the first page\n");
  shapeSelection = voCreatePageSelection(engine, page0);
  verify(shapeSelection != NULL);
  verify(VO_IFACE(engine, ISelection)->selectRectangle(engine, shapeSelection, &shapeRectangle, VO_SELECT_SET));
  printf(" . selection done successfully\n");
  printf("\n");

  printf("6) adding the selection to the shape field\n");
  verify(VO_IFACE(engine, IContentField)->addItems(engine, shapeField, shapeSelection));
  printf(" . selection added to the shape field successfully\n");
  printf("\n");

  printf("7) creating and configuring a new ShapeRecognizer\n");
  shapeRecognizer = voCreateObject(engine, VO_ShapeRecognizer);
  verify(shapeRecognizer != NULL);
  printf(" . ShapeRecognizer created successfully\n");
  shapeKnowledge = loadEngineObject(SHAPE_KNOWLEDGE_RES);
  verify(shapeKnowledge != NULL);
  printf(" . ShapeKnowledge loaded successfully\n");
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, shapeRecognizer, shapeKnowledge));
  printf(" . ShapeKnowledge attached to the recognizer successfully\n");
  printf("\n");

  printf("8) performing a recognition on the selection\n");
  initializeTextUIProgressBar(&progressBar);
  verify(VO_IFACE(engine, IContentFieldProcessor)->process(engine, shapeRecognizer, shapeField, updateTextUIProgressBar, &progressBar));
  finalizeTextUIProgressBar(&progressBar);
  printf(" . recognition performed successfully\n");
  printf("\n");

  printf("9) retrieving the recognition result\n");
  shapeResult = VO_IFACE(engine, IContentField)->getResult(engine, shapeField);
  verify(shapeResult != NULL);
  printf(" . result retrieved successfully\n");
  segmentCount = VO_IFACE(engine, IShapeDocument)->getSegmentCount(engine, shapeResult);
  verify(segmentCount != (uint32_t)-1);
  printf(" . retrieved %d segments\n", segmentCount);
  {
    uint32_t segmentId;

    for (segmentId = 0; segmentId < segmentCount; ++segmentId)
    {
      voShapeSegment shapeSegment = NULL;
      uint32_t candidateCount;

      shapeSegment = VO_IFACE(engine, IShapeDocument)->getSegmentAt(engine, shapeResult, segmentId);
      verify(shapeSegment != NULL);

      candidateCount = VO_IFACE(engine, IShapeSegment)->getCandidateCount(engine, shapeSegment);
      verify(candidateCount != (uint32_t)-1);
      printf("   . ShapeSegment #%d has %d ShapeCandidate%s\n", segmentId, candidateCount, ((candidateCount > 1) ? "s" : ""));

      verify(voReleaseObject(engine, shapeSegment));
    }
  }
  printf("\n");

  printf("10) cleaning-up: releasing all created objects and destroying the Engine\n");
  verify(voReleaseObject(engine, shapeResult));
  verify(voReleaseObject(engine, shapeRecognizer));
  verify(voReleaseObject(engine, shapeKnowledge));
  verify(voReleaseObject(engine, shapeSelection));
  verify(voReleaseObject(engine, shapeField));

  verify(voReleaseObject(engine, content0));
  verify(voReleaseObject(engine, page0));
  verify(voReleaseObject(engine, document));
  verify(voDestroyEngine(engine));
  printf(" . clean-up done successfully\n");
  printf("\n");

  printf("end of example:\n");
  printf(" you just learnt how to perform a shape recognition on a document\n");

  // -- finalization -----------------------------------------------------------
  unloadMyScriptEngine();

  return EXIT_SUCCESS;
}
