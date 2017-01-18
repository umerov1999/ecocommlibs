/**
 * The `PageUpdateListener` example code shows you how to handle page updates on
 * a document.
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


struct DocumentAsyncCommandResult
{
  bool success;
  voLayout layout;
};

// asynchronous saving callback (called at completion)
static void completionCallback(voEngine engine, voDocumentAsyncCommand cmd, bool success, voDocumentAsyncCommandResult result, void* userParam)
{
  struct DocumentAsyncCommandResult* cmdResult = (struct DocumentAsyncCommandResult*)userParam;

  if (cmdResult != NULL)
  {
    cmdResult->success = success;

    if (cmd == VO_CMD_GET_LAYOUT)
    {
      cmdResult->layout = voAcquireObject(engine, result.getLayout.layout);
    }
  }
}

// listener structures, functions and callbacks
struct PageDescription
{
  voDocument document;
  int pageIndex;
};

static void VO_MSE_CALL onDestroy(voEngine engine, voUserObject target)
{
  printf(" . PageUpdateListener ==> onDestroy\n");
}

static void VO_MSE_CALL onAdd(voEngine engine, voUserObject target, voPage page)
{
  printf(" . PageUpdateListener ==> onAdd\n");
}

static void VO_MSE_CALL onRemove(voEngine engine, voUserObject target, voPage page)
{
  printf(" . PageUpdateListener ==> onRemove\n");
}

static void VO_MSE_CALL onUpdate(voEngine engine, voEngineObject target, voPage page, int layerIndex, const voExtent* extent)
{
  printf( " . PageUpdateListener ==> onUpdate:\n     Layer = %d\n     Extent = { \"xmin\":%0.3f, \"ymin\":%0.3f, \"xmax\":%0.3f, \"ymax\":%0.3f }\n",
          layerIndex, extent->xmin, extent->ymin, extent->xmax, extent->ymax);
}

static voUserObject createPageUpdateListenerUserObject(voEngine engine, void* userParams)
{
  voUserObjectInitializer initializer = { userParams, onDestroy };
  voUserObject userObject = voCreateObjectEx(engine, VO_UserObject, &initializer, sizeof(initializer));

  static const voIPageUpdateListener listener = {
                                                  onAdd,
                                                  onRemove,
                                                  onUpdate,
                                                };

  bool ok = VO_IFACE(engine, IUserObject)->registerInterface(engine, userObject, VO_IPageUpdateListener, &listener);
  verify(ok == true);

  return userObject;
}

// main function
int main()
{
  char temporaryDirectoryBuffer[PATH_MAX];
  char documentFileNameBuffer[PATH_MAX];
  voString temporaryDirectory;
  voString documentFileName;

  struct DocumentAsyncCommandResult aSyncResult = { false, NULL };
  struct PageDescription pageDesc = { NULL, -1 };

  voDocument document = NULL;
  voPage page = NULL;
  voLayout layout = NULL;
  voPageSelection selection = NULL;
  voUserObject listener = NULL;

  int pageCount = 0;
  int layerCount = 0;


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

  printf("3) getting the number of Pages in the Document\n");
  pageCount = VO_IFACE(engine, IDocument)->getPageCount(engine, document);
  verify(pageCount == 1);
  printf(" . the Document contains %d page(s)\n", pageCount);
  printf("\n");

  printf("4) retrieving the first Page of the Document\n");
  page = VO_IFACE(engine, IDocument)->getPageAt(engine, document, 0);
  verify(page != NULL);
  printf(" . Page retrieved successfully\n");
  printf("\n");

  printf("5) retrieving the Layout of the Page\n");
  aSyncResult.success = false;
  aSyncResult.layout = NULL;
  verify(VO_IFACE(engine, IPage)->getLayout(engine, page, completionCallback, &aSyncResult));
  verify(VO_IFACE(engine, IPage)->sync(engine, page));
  verify(aSyncResult.success);
  verify(NULL != aSyncResult.layout);
  layout = aSyncResult.layout;
  printf(" . Layout retrieved successfully\n");
  layerCount =  VO_IFACE(engine, ILayout)->getLayerCount(engine, layout);
  printf(" . the Layout contains %d layer(s)\n", layerCount);
  printf("\n");

  printf("6) creating a user object implementing the IPageListener interface\n");
  pageDesc.document = document;
  pageDesc.pageIndex = 0;
  listener = createPageUpdateListenerUserObject(engine, &pageDesc);
  verify(listener != NULL);
  printf(" . page listener created successfully\n");
  printf("\n");

  printf("7) attaching the listener to the Page\n");
  verify (VO_IFACE(engine, IPage)->addListener(engine, page, listener));
  printf(" . listener attached successfully\n");
  printf("\n");

  printf("8) creating a selection on the Page\n");
  selection = voCreatePageSelection(engine, page);
  verify(selection != NULL);
  printf(" . selection created successfully\n");
  printf("\n");

  printf("9) selecting the whole Page\n");
  verify(VO_IFACE(engine, ISelection)->selectAll(engine, selection));
  printf(" . selection performed successfully\n");
  printf("\n");

  printf("10) erasing the selection from the Page\n");
  verify (VO_IFACE(engine, IPage)->erase(engine, page, selection));
  printf(" . erase operation done successfully\n");
  printf("\n");

  printf("11) detaching the listener from the Page\n");
  verify (VO_IFACE(engine, IPage)->removeListener(engine, page, listener));
  printf(" . listener detached successfully\n");
  printf("\n");

  printf("12) destroying the listener\n");
  verify(voReleaseObject(engine, listener));
  printf(" . listener destroyed successfully\n");
  printf("\n"); 

  printf("13) cleaning-up: releasing all created objects and destroying the Engine\n");
  verify(voReleaseObject(engine, layout));
  verify(voReleaseObject(engine, page));
  verify(voReleaseObject(engine, document));
  verify(voDestroyEngine(engine));
  printf(" . clean-up done successfully\n");
  printf("\n");

  printf("end of example:\n");
  printf(" you just learnt how to listen to the page updates on a document\n");

  // -- finalization -----------------------------------------------------------
  unloadMyScriptEngine();

  return EXIT_SUCCESS;
}
