/**
 * The `PageEdition` example code shows you how to manipulate the  content of a
 * page.
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

  voString inkLayerName = voString_INITIALIZER("Ink");
  voString newLayerName = voString_INITIALIZER("newLayer");

  voDocument document = NULL;
  voPage page = NULL;
  voPageSelection selection = NULL;
  voLayout layout = NULL;
  voLayerIterator layerIterator = NULL;

  voRectangle rectangle = { 32.0, 0.0, 300.0, 500.0 };
  voCircle circle = { 386.0, 264.0, 10.0 };
  voTransform transform = { 0 };
  
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
  printf(" . Layout loaded successfully\n");
  printf("\n");

  printf("5) creating a PageSelection\n");
  selection = voCreatePageSelection(engine, page);
  verify(selection != NULL);
  printf(" . PageSelection created successfully\n");
  printf("\n");

  printf("6) selecting a Layer of the Page\n");
  printf(" . retrieving an iterator over the Layers\n");
  layerIterator = VO_IFACE(engine, ILayout)->getLayer(engine, layout, NULL, &inkLayerName);
  verify(layerIterator != NULL);
  verify(VO_IFACE(engine, ILayerIterator)->isAtEnd(engine, layerIterator) == VO_NO);
  printf(" . selecting Layer\n");
  verify(VO_IFACE(engine, IPageSelection)->selectLayer(engine, selection, layerIterator, VO_SELECT_SET));
  printf(" . releasing the LayerIterator\n");
  verify(voReleaseObject(engine, layerIterator));
  printf(" . selection performed successfully\n");
  printf("\n");

  printf("7) selecting a rectangle within the selection\n");
  verify(VO_IFACE(engine, ISelection)->selectRectangle(engine, selection, &rectangle, VO_SELECT_INTERSECTION));
  printf(" . rectangle selection performed successfully\n");
  printf("\n");

  printf("8) erasing the selected portion of the Page\n");
  verify(VO_IFACE(engine, IPage)->erase(engine, page, selection));
  printf(" . selection erased successfully\n");
  printf("\n");

  printf("9) checking that the Page is marked as modified\n");
  verify(VO_IFACE(engine, IPage)->isModified(engine, page) == VO_YES);
  printf(" . Page marked as modified successfully\n");
  printf("\n");

  printf("10) selecting the closest InkStroke from a given point\n");
  verify(VO_IFACE(engine, IPageSelection)->hit(engine, selection, &circle, NULL, VO_SELECT_SET));
  verify(VO_IFACE(engine, IInkStrokeSet)->getStrokeCount(engine, selection) == 1);
  printf(" . selection performed successfully\n");
  printf("\n");

  printf("11) creating a rotation transform\n");
  verify(VO_IFACE(engine, IGeometry)->setIdentity(engine, &transform));
  verify(VO_IFACE(engine, IGeometry)->rotate(engine, &transform, 3.14f, 470.0f, 293.0f));
  printf(" . transform created successfully\n");
  printf("\n");

  printf("12) applying rotation on the selected InkStroke\n");
  verify(VO_IFACE(engine, IPage)->transform(engine, page, selection, &transform));
  printf(" . rotation performed successfully\n");
  printf("\n");

  printf("13) adding a new Layer to the Layout\n");
  layerCount = VO_IFACE(engine, ILayout)->getLayerCount(engine, layout);
  verify(layerCount >= 0);
  verify(VO_IFACE(engine, ILayout)->addLayer(engine, layout, NULL, &newLayerName));
  verify(VO_IFACE(engine, ILayout)->getLayerCount(engine, layout) == layerCount + 1);
  printf(" . new Layer added successfully\n");
  printf("\n");

  printf("14) moving selected content to the new Layer\n");
  printf(" . retrieving an iterator over the Layers\n");
  layerIterator = VO_IFACE(engine, ILayout)->getLayer(engine, layout, NULL, &newLayerName);
  verify(layerIterator != NULL);
  verify(VO_IFACE(engine, ILayerIterator)->isAtEnd(engine, layerIterator) == VO_NO);
  printf(" . moving the selection to the Layer\n");
  verify(VO_IFACE(engine, IPage)->moveToLayer(engine, page, selection, layerIterator));
  printf(" . releasing the LayerIterator\n");
  verify(voReleaseObject(engine, layerIterator));
  printf(" . selection moved successfully\n");
  printf("\n");

  printf("15) cleaning-up: releasing all created objects and destroying the Engine\n");
  verify(voReleaseObject(engine, selection));
  verify(voReleaseObject(engine, layout));
  verify(voReleaseObject(engine, page));
  verify(voReleaseObject(engine, document));
  verify(voDestroyEngine(engine));
  printf(" . clean-up done successfully\n");
  printf("\n");

  printf("end of example:\n");
  printf(" you just learnt how to manipulate the content of a page \n");

  // -- finalization -----------------------------------------------------------
  unloadMyScriptEngine();

  return EXIT_SUCCESS;
}
