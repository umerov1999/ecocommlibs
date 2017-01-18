/**
 * The `LayoutEdition` example code shows you how to add images and ink strokes
 * to the layout of a page.
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

// digital ink
#include "strokes.h"

// path to the Document files
// (note that you can rename any notes document as a zip file to watch inside
// it.)
#define TEMPORARY_DIRECTORY OUTPUT_DIR "/tmp"
#define DOCUMENT_FILE       DATA_DIR "/example4.notes"
#define IMAGE_FILE          DATA_DIR "/image.png"


// callback called upon layout load completion
static void GetLayoutCompletionCallback(voEngine engine, voDocumentAsyncCommand cmd, bool success, voDocumentAsyncCommandResult result, void* userParam)
{
  voLayout* layout = (voLayout*)userParam;
  verify(success);
  *layout = voAcquireObject(engine, result.getLayout.layout);
}

// callback called upon ink load completion
static void GetInkCompletionCallback(voEngine engine, voDocumentAsyncCommand cmd, bool success, voDocumentAsyncCommandResult result, void* userParam)
{
  voInk* ink = (voInk*)userParam;
  verify(success);
  *ink = voAcquireObject(engine, result.getInk.ink);
}

// callback called upon object addition completion
static void AddObjectCompletionCallback(voEngine engine, voDocumentAsyncCommand cmd, bool success, voDocumentAsyncCommandResult result, void* userParam)
{
  verify(success);
  printf("   . Object added successfully\n");
}

// callback called upon change discard completion
static void DiscardChangesCompletionCallback(voEngine engine, voDocumentAsyncCommand cmd, bool success, voDocumentAsyncCommandResult result, void* userParam)
{
  verify(success);
  printf(" . changes discarded successfully (asynchronous operation requested at step 14)\n");
}

// main function
int main()
{
  char temporaryDirectoryBuffer[PATH_MAX];
  char documentFileNameBuffer[PATH_MAX];
  char imagePath[PATH_MAX];
  voString temporaryDirectory;
  voString documentFileName;
  voString imageFileName;

  voString layerName = voString_INITIALIZER("newLayer");
  voString tagName = voString_INITIALIZER("strokesLayoutItem");
  voString xChannelName = voString_INITIALIZER("X");
  voString yChannelName = voString_INITIALIZER("Y");

  const char* penId = "newPen";
  voString penIdString = voString_INITIALIZER(penId);

  const char* fontId = "newFont";
  voString fontIdString = voString_INITIALIZER(fontId);

  const char* imageUrl = "exampleImage.png";
  voString imageUrlString = voString_INITIALIZER(imageUrl);

  const char* imageMimeTye = "image/png";

  voDocument document = NULL;
  voPage page = NULL;
  voLayout layout = NULL;
  voInk ink = NULL;
  voLayerIterator layerIterator = NULL;

  voInkStrokeFormatBuilder strokeFormatBuilder = NULL;
  voInkStrokeFormat strokeFormat = NULL;
  voInkStrokeBuilder strokeBuilder = NULL;
  voInkStroke stroke_1 = NULL;
  voInkStroke stroke_2 = NULL;

  voPenData pen = { "CalligraphicBrush", "None", 1.0, 0x15A607FF };
  voFontData font = { "courier", 400.0f, 12.0f, 1.2f, "normal", "normal" };
  voLayoutItemData item;

  int sampleIndex = 0;
  int pageCount = 0;
  int layerCount = 0;
  int64_t tagId = -1;


  // -- initialization ---------------------------------------------------------
  if (!loadMyScriptEngine(MYSCRIPT_ENGINE_LIBRARY_DIR, NULL))
    return (fprintf(stderr, "failed to load the MyScriptEngine library\n"), EXIT_FAILURE);

  resolvePath(documentFileNameBuffer, PATH_MAX, DOCUMENT_FILE);
  documentFileName.bytes = documentFileNameBuffer;
  documentFileName.byteCount = strlen(documentFileName.bytes);

  resolvePath(temporaryDirectoryBuffer, PATH_MAX, TEMPORARY_DIRECTORY);
  temporaryDirectory.bytes = temporaryDirectoryBuffer;
  temporaryDirectory.byteCount = strlen(temporaryDirectory.bytes);

  resolvePath(imagePath, PATH_MAX, IMAGE_FILE);
  imageFileName.bytes = imagePath;
  imageFileName.byteCount = strlen(imageFileName.bytes);

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

  printf("4) retrieving the Layout and Ink of the Page\n");
  printf(" . sending the Layout request to the Page\n");
  verify(VO_IFACE(engine, IPage)->getLayout(engine, page, GetLayoutCompletionCallback, &layout));
  printf(" . sending the Ink request to the Page\n");
  verify(VO_IFACE(engine, IPage)->getInk(engine, page, GetInkCompletionCallback, &ink));
  printf(" . waiting for the requests to complete\n");
  printf("   . note that sync() should be called only if you need the pending \n");
  printf("     requests to be completed before any other operation is performed\n");
  verify(VO_IFACE(engine, IPage)->sync(engine, page));
  verify(layout != NULL);
  verify(ink != NULL);
  printf(" . Layout and Ink retrieved successfully\n");
  printf("\n");

  printf("5) adding a Layer to the Layout\n");
  layerCount = VO_IFACE(engine, ILayout)->getLayerCount(engine, layout);
  verify(VO_IFACE(engine, ILayout)->addLayer(engine, layout, NULL, &layerName));
  verify(VO_IFACE(engine, ILayout)->getLayerCount(engine, layout) == layerCount + 1);
  printf(" . new Layer added successfully\n");
  printf("\n");

  printf("6) adding a new Pen and Font to the Layout\n");
  verify(VO_IFACE(engine, ILayout)->addPen(engine, layout, NULL, &penIdString, &pen));
  printf(" . new Pen added successfully\n");
  verify(VO_IFACE(engine, ILayout)->addFont(engine, layout, NULL, &fontIdString, &font));
  printf(" . new Font added successfully\n");
  printf("\n");

  printf("7) adding a LayoutItem of type strokes\n");
  printf(" . creating a Tag to tag the ink that will be associated\n");
  printf("   with the new LayoutItem\n");
  tagId = VO_IFACE(engine, IInk)->addTag(engine, ink, NULL, &tagName, NULL, NULL);
  verify(tagId != -1);
  printf(" . retrieving an iterator over the Layers\n");
  layerIterator = VO_IFACE(engine, ILayout)->getLayer(engine, layout, NULL, &layerName);
  verify(layerIterator != NULL);
  verify(VO_IFACE(engine, ILayerIterator)->isAtEnd(engine, layerIterator) == VO_NO);
  printf(" . adding the LayoutItem to the Layer\n");
  item.type = VO_LAYOUT_ITEM_STROKES;
  item.data.strokes.penId = penId;
  item.data.strokes.tagId = tagId;
  item.data.strokes.fontId = fontId;
  verify(VO_IFACE(engine, ILayerIterator)->addItem(engine, layerIterator, &item));
  printf(" . LayoutItem added successfully\n");
  printf("\n");

  printf("8) creating an InkStrokeBuilder\n");
  printf(" . creating an InkStrokeFormatBuilder\n");
  strokeFormatBuilder = voCreateInkStrokeFormatBuilder(engine);
  verify(strokeFormatBuilder != NULL);
  printf(" . specifying the format content: 2 channels X and Y\n");
  verify(VO_IFACE(engine, IInkStrokeFormatBuilder)->addChannel(engine, strokeFormatBuilder, NULL, &xChannelName));
  verify(VO_IFACE(engine, IInkStrokeFormatBuilder)->addChannel(engine, strokeFormatBuilder, NULL, &yChannelName));
  printf(" . creating the InkStrokeFormat\n");
  strokeFormat = VO_IFACE(engine, IInkStrokeFormatBuilder)->createStrokeFormat(engine, strokeFormatBuilder);
  verify(strokeFormat != NULL);
  printf(" . creating an InkStrokeBuilder with the InkStrokeFormat\n");
  strokeBuilder = voCreateInkStrokeBuilder1(engine, strokeFormat);
  verify(strokeBuilder != NULL);
  printf(" . InkStrokeBuilder created successfully\n");
  printf("\n");

  printf("9) creating an InkStroke\n");
  printf(" . adding the InkStroke samples to the InkStrokeBuilder\n");
  for (sampleIndex = 0; sampleIndex < (int)(sizeof(rectangle) / sizeof(*rectangle)) / 2; ++sampleIndex)
  {
    verify(VO_IFACE(engine, IInkStrokeBuilder)->addSampleA_f(engine, strokeBuilder, rectangle + 2*sampleIndex, 2));
  }
  printf(" . retrieving the InkStroke from the InkStrokeBuilder\n");
  stroke_1 = VO_IFACE(engine, IInkStrokeBuilder)->createStroke(engine, strokeBuilder);
  verify(stroke_1 != NULL);
  printf(" . InkStroke created successfully\n");
  printf("\n");

  printf("10) adding the InkStroke to the previously created LayoutItem (through its Tag id)\n");
  verify(VO_IFACE(engine, IInk)->addStrokeEx(engine, ink, stroke_1, &tagId, 1));
  printf(" . InkStroke added successfully\n");
  printf("\n");

  printf("11) creating another InkStroke and adding it to the same LayoutItem\n");
  printf(" . clearing the InkStrokeBuilder\n");
  verify(VO_IFACE(engine, IInkStrokeBuilder)->clear(engine, strokeBuilder));
  printf(" . adding the stroke samples to the InkStrokeBuilder\n");
  for (sampleIndex = 0; sampleIndex < (int)(sizeof(circle) / sizeof(*circle)) / 2; ++sampleIndex)
  {
    verify(VO_IFACE(engine, IInkStrokeBuilder)->addSampleA_f(engine, strokeBuilder, circle + 2*sampleIndex, 2));
  }
  printf(" . retrieving the InkStroke from the InkStrokeBuilder\n");
  stroke_2 = VO_IFACE(engine, IInkStrokeBuilder)->createStroke(engine, strokeBuilder);
  verify(stroke_2 != NULL);
  printf(" . adding the InkStroke to the previously created LayoutItem (through its Tag id)\n");
  verify(VO_IFACE(engine, IInk)->addStrokeEx(engine, ink, stroke_2, &tagId, 1));
  printf(" . InkStroke added successfully\n");
  printf("\n");

  printf("12) creating a LayoutItem of type image\n");
  printf(" . adding an image to the document\n");
  verify(VO_IFACE(engine, IDocument)->addObject(engine, document, NULL, &imageUrlString, &imageFileName, AddObjectCompletionCallback, NULL));
  printf(" . waiting for the image addition to complete successfully\n");
  printf("   . note that sync() should be called only if you need the pending \n");
  printf("     requests to be completed before any other operation is performed\n");
  verify(VO_IFACE(engine, IDocument)->sync(engine, document));
  printf(" . adding a new LayoutItem referencing the image\n");
  item.type = VO_LAYOUT_ITEM_OBJECT;
  item.data.object.url = imageUrl;
  item.data.object.rectangle.x = 1000.0;
  item.data.object.rectangle.y = 0.0;
  item.data.object.rectangle.width = 1000.0;
  item.data.object.rectangle.height = 1500.0;
  item.data.object.transformId = NULL;
  item.data.object.mimeType = imageMimeTye;
  verify(VO_IFACE(engine, ILayerIterator)->addItem(engine, layerIterator, &item));
  printf(" . LayoutItem added successfully\n");
  printf("\n");
  
  printf("13) displaying Layout description\n");
  {
    voLayerIterator layerIterator2;
    printf(" . retrieving number of Layers");
    layerCount = VO_IFACE(engine, ILayout)->getLayerCount(engine, layout);
    printf(" = %d\n", layerCount);
    printf(" . iterating on all Layers\n");
    layerIterator2 = VO_IFACE(engine, ILayout)->getLayers(engine, layout);
    verify(layerIterator2 != NULL);
    while(VO_IFACE(engine, ILayerIterator)->isAtEnd(engine, layerIterator2) == VO_NO)
    {
      voString currentLayerName = { NULL, 0 };
      int layoutItemCount = -1;
      voLayoutItemIterator layoutItemIterator = NULL;

      printf("   . retrieving Layer id");
      verify(VO_IFACE(engine, ILayerIterator)->getId(engine, layerIterator2, NULL, &currentLayerName));
      currentLayerName.bytes = (char*)malloc(currentLayerName.byteCount + 1);
      verify(currentLayerName.bytes != NULL);
      verify(VO_IFACE(engine, ILayerIterator)->getId(engine, layerIterator2, NULL, &currentLayerName));
      currentLayerName.bytes[currentLayerName.byteCount] = '\0';
      printf(" = \"%s\"\n", currentLayerName.bytes);
      free(currentLayerName.bytes);

      printf("     . retrieving number of Layout Items in the Layer");
      layoutItemCount = VO_IFACE(engine, ILayerIterator)->getItemCount(engine, layerIterator2);
      verify(layoutItemCount >= 0);
      printf(" = %d\n", layoutItemCount);

      if (layoutItemCount > 0)
      {
        printf("     . iterating on Layout Items\n");
        layoutItemIterator = VO_IFACE(engine, ILayerIterator)->getItems(engine, layerIterator2);
        verify(layoutItemIterator != NULL);
        while(VO_IFACE(engine, ILayoutItemIterator)->isAtEnd(engine, layoutItemIterator) == VO_NO)
        {
          voLayoutItemData itemData;
          verify(VO_IFACE(engine, ILayoutItemIterator)->getData(engine, layoutItemIterator, &itemData));

          if (itemData.type == VO_LAYOUT_ITEM_STROKES)
          {
            printf("       item : VO_LAYOUT_ITEM_STROKES\n");
            printf("       . pen Id = \"%s\"\n", itemData.data.strokes.penId);
            printf("       . font Id = \"%s\"\n", itemData.data.strokes.fontId);
            printf("       . tag Id = %lld\n", itemData.data.strokes.tagId);
          }
          else if (itemData.type == VO_LAYOUT_ITEM_OBJECT)
          {
            printf("       item : VO_LAYOUT_ITEM_OBJECT\n");
            printf("       . url = \"%s\"\n", itemData.data.object.url);
            printf("       . rectangle = { %f, %f, %f, %f }\n", 
                    itemData.data.object.rectangle.x, 
                    itemData.data.object.rectangle.y, 
                    itemData.data.object.rectangle.width, 
                    itemData.data.object.rectangle.height);
            if (itemData.data.object.transformId != NULL)
              printf("       . transform = \"%s\"\n", itemData.data.object.transformId);
            if (itemData.data.object.mimeType != NULL)
              printf("       . mime type = \"%s\"\n", itemData.data.object.mimeType);
          }
      
          verify(VO_IFACE(engine, ILayoutItemIterator)->next(engine, layoutItemIterator));
        }
        printf("     . releasing Layout Items iterator\n");
        verify(voReleaseObject(engine, layoutItemIterator));
      }
      verify(VO_IFACE(engine, ILayerIterator)->next(engine, layerIterator2));
    }
    printf("   . releasing Layer iterator\n");
    voReleaseObject(engine, layerIterator2);
    printf(" . Layout description displayed successfully\n");
  }
  printf("\n");

  printf("14) requesting to discard the modifications performed on the Document\n");
  verify(VO_IFACE(engine, IDocument)->discardChanges(engine, document, DiscardChangesCompletionCallback, NULL));
  printf("\n");

  printf("15) cleaning-up: releasing all created objects and destroying the Engine\n");
  verify(voReleaseObject(engine, strokeFormatBuilder));
  verify(voReleaseObject(engine, strokeFormat));
  verify(voReleaseObject(engine, strokeBuilder));
  verify(voReleaseObject(engine, stroke_1));
  verify(voReleaseObject(engine, stroke_2));

  voReleaseObject(engine, layerIterator);
  verify(voReleaseObject(engine, layout));
  verify(voReleaseObject(engine, ink));
  verify(voReleaseObject(engine, page));
  verify(voReleaseObject(engine, document));
  verify(voDestroyEngine(engine));
  printf(" . clean-up done successfully\n");
  printf("\n");

  printf("end of example:\n");
  printf(" you just learnt how to manipulate the Layout of a Page \n");

  // -- finalization -----------------------------------------------------------
  unloadMyScriptEngine();

  return EXIT_SUCCESS;
}
