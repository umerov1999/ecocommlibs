/**
 * The `UndoRedo` example code shows you how to undo or redo actions performed
 * on a document.
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
#define DOCUMENT_FILE       DATA_DIR "/example4.notes"


// callback called upon layout load completion
static void GetLayoutCompletionCallback(voEngine engine, voDocumentAsyncCommand cmd, bool success, voDocumentAsyncCommandResult result, void* userParam)
{
  voLayout* layout = (voLayout*)userParam;
  verify(success);
  *layout = voAcquireObject(engine, result.getLayout.layout);
}

// main function
int main()
{
  char temporaryDirectoryBuffer[PATH_MAX];
  char documentFileNameBuffer[PATH_MAX];
  voString temporaryDirectory;
  voString documentFileName;
  voString newLayerId = voString_INITIALIZER("newLayer");

  voDocument document = NULL;
  voPage page = NULL;
  voPageSelection selection = NULL;
  voHistoryManager historyManager = NULL;
  voLayout layout = NULL;

  voRectangle rectangle = { 32.0, 122.0, 300.0, 500.0 };
  voTransform transform = { 0 };
  
  int pageCount = 0;
  int layerCount = 0;

  voYesNo canUndo = VO_NO;
  voYesNo canRedo = VO_NO;

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

  printf("3) retrieving the first Page of the Document\n");
  pageCount = VO_IFACE(engine, IDocument)->getPageCount(engine, document);
  verify(pageCount > 0);
  page = VO_IFACE(engine, IDocument)->getPageAt(engine, document, 0);
  verify(page != NULL);
  printf(" . Page retrieved successfully\n");
  printf("\n");

  printf("4) retrieving the Layout of the Page\n");
  printf(" . sending the Layout request to the Page\n");
  verify(VO_IFACE(engine, IPage)->getLayout(engine, page, GetLayoutCompletionCallback, &layout));
  printf(" . waiting for the Layout request to complete\n");
  printf("   . note that sync() should be called only if you need the Layout\n");
  printf("     immediately and if you have nothing else to do in the meantime\n");
  verify(VO_IFACE(engine, IPage)->sync(engine, page));
  verify(layout != NULL);
  printf(" . Layout retrieved successfully\n");
  printf("\n");

  printf("5) retrieving the Document HistoryManager\n");
  historyManager = VO_IFACE(engine, IVersionable)->getHistoryManager(engine, document);
  verify(historyManager != NULL);
  printf(" . HistoryManager retrieved successfully\n");
  printf("\n");

  printf("6) creating a PageSelection\n");
  selection = voCreatePageSelection(engine, page);
  verify(selection != NULL);
  printf(" . PageSelection created successfully\n");
  printf("\n");

  printf("7) selecting a portion of the Page\n");
  verify(VO_IFACE(engine, ISelection)->selectRectangle(engine, selection, &rectangle, VO_SELECT_SET));
  printf(" . selection performed successfully\n");
  printf("\n");

  printf("8) erasing selected portion of the Page\n");
  verify(VO_IFACE(engine, IPage)->erase(engine, page, selection));
  printf(" . selection erased successfully\n");
  printf("\n");

  printf("9) selecting the remaining data in the Page\n");
  verify(VO_IFACE(engine, ISelection)->selectAll(engine, selection));
  printf(" . selection performed successfully\n");
  printf("\n");

  printf("10) creating an horizontal translation transform\n");
  verify(VO_IFACE(engine, IGeometry)->setIdentity(engine, &transform));
  verify(VO_IFACE(engine, IGeometry)->translate(engine, &transform, 10.0, 0.0));
  printf(" . transform created successfully\n");
  printf("\n");

  printf("11) translating the selected portion of the Page\n");
  verify(VO_IFACE(engine, IPage)->transform(engine, page, selection, &transform));
  printf(" . translation performed successfully\n");
  printf("\n");

  printf("12) undoing the translation operation\n");
  canUndo = VO_IFACE(engine, IHistoryManager)->canUndo(engine, historyManager);
  verify(canUndo == VO_YES);
  verify(VO_IFACE(engine, IHistoryManager)->undo(engine, historyManager));
  printf(" . transform undone successfully\n");
  printf("\n");

  printf("13) undoing the erasing operation\n");
  canUndo = VO_IFACE(engine, IHistoryManager)->canUndo(engine, historyManager);
  verify(canUndo == VO_YES);
  verify(VO_IFACE(engine, IHistoryManager)->undo(engine, historyManager));
  printf(" . erase undone successfully\n");
  printf("\n");

  printf("14) redoing the erasing operation\n");
  canRedo = VO_IFACE(engine, IHistoryManager)->canRedo(engine, historyManager);
  verify(canRedo == VO_YES);
  verify(VO_IFACE(engine, IHistoryManager)->redo(engine, historyManager));
  printf(" . erase redone successfully\n");
  printf("\n");

  printf("15) starting an HistoryManager transaction, which groups several \n");
  printf("    actions so that they can be undone (or redone) with a single call to\n");
  printf("    undo (or redo)\n");
  verify(VO_IFACE(engine, IHistoryManager)->startTransaction(engine, historyManager));
  printf(" . transaction started successfully\n");
  printf("\n");
  
  printf("16) adding a new Layer to the Layout\n");
  layerCount = VO_IFACE(engine, ILayout)->getLayerCount(engine, layout);
  verify(layerCount >= 0);
  verify(VO_IFACE(engine, ILayout)->addLayer(engine, layout, NULL, &newLayerId));
  verify(VO_IFACE(engine, ILayout)->getLayerCount(engine, layout) == layerCount + 1);
  printf(" . new Layer added successfully\n");
  printf("\n");

  printf("17) selecting the remaining data in the Page\n");
  verify(VO_IFACE(engine, ISelection)->selectAll(engine, selection));
  printf(" . selection performed successfully\n");
  printf("\n");

  printf("18) generating a vertical translation Transform\n");
  verify(VO_IFACE(engine, IGeometry)->setIdentity(engine, &transform));
  verify(VO_IFACE(engine, IGeometry)->translate(engine, &transform, 0.0, 10.0));
  printf(" . transform created successfully\n");
  printf("\n");

  printf("19) translating the selected portion of the Page\n");
  verify(VO_IFACE(engine, IPage)->transform(engine, page, selection, &transform));
  printf(" . translation performed successfully\n");
  printf("\n");

  printf("20) committing the HistoryManager transaction\n");
  verify(VO_IFACE(engine, IHistoryManager)->commit(engine, historyManager));
  printf(" . transaction commited successfully\n");
  printf("\n");

  printf("21) undoing the operations performed within the HistoryManager transaction\n");
  canUndo = VO_IFACE(engine, IHistoryManager)->canUndo(engine, historyManager);
  verify(canUndo == VO_YES);
  verify(VO_IFACE(engine, IHistoryManager)->undo(engine, historyManager));
  verify(VO_IFACE(engine, ILayout)->getLayerCount(engine, layout) == layerCount);
  printf(" . operations undone successfully\n");
  printf("\n");

  printf("22) cleaning-up: releasing all created objects and destroying the Engine\n");
  verify(voReleaseObject(engine, selection));
  verify(voReleaseObject(engine, layout));
  verify(voReleaseObject(engine, page));
  verify(voReleaseObject(engine, historyManager));
  verify(voReleaseObject(engine, document));
  verify(voDestroyEngine(engine));
  printf(" . clean-up done successfully\n");
  printf("\n");

  printf("end of example:\n");
  printf(" you just learnt how to undo and redo document modifications\n");

  // -- finalization -----------------------------------------------------------
  unloadMyScriptEngine();

  return EXIT_SUCCESS;
}

