/**
 * The `ProcessAnalyzer` example code shows you how to perform an analysis on a
 * document.
 */


// -- configuration ------------------------------------------------------------

// common configuration parameters
#include "../common/config.h"

// include the MyScript Engine API header(s)
#include <c/MyScriptEngine.h>
#include <c/MyScriptDocument.h>

#include <c/MyScriptAnalyzer.h>
#include <../src/MyScriptAnalyzer-documentToSVG.h>

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

// SVG styling
// we use an output box of 630x891, which is close to 600x800 but with A* page
// format series ratio, with a margin of 5%
#define DOCUMENTTOSVG_DOC_WIDTH             630.0f
#define DOCUMENTTOSVG_DOC_HEIGHT            891.0f
#define DOCUMENTTOSVG_DOC_MARGIN            0.05f
#define DOCUMENTTOSVG_STROKE_COLOR          "black"
#define DOCUMENTTOSVG_STROKE_WIDTH          2.0f
#define DOCUMENTTOSVG_TEXT_HEIGHT_FACTOR    1.25f
#define DOCUMENTTOSVG_DISPLAY_ORIGINAL_INK  true
#define DOCUMENTTOSVG_ORIGINAL_INK_COLOR    "grey"
#define DOCUMENTTOSVG_ARROW_SHAPE_FACTOR    2.0f
#define DOCUMENTTOSVG_ARROW_SIZE_FACTOR     0.08f


// Asynchronous content callback (called at completion)
struct DocumentAsyncCommandResult
{
  bool success;
  voEngineObject object;
};

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

  const voString analyzerFieldId = voString_INITIALIZER("myAnalyzerField");
  voContentField analyzerField = NULL;

  voPageSelection pageSelection = NULL;

  voAnalyzer analyzer = NULL;
  voAnalyzerKnowledge analyzerKnowledge = NULL;

  voStructuredInputRecognizer textRecognizer = NULL;
  voShapeRecognizer shapeRecognizer = NULL;
  voShapeBeautifier shapeBeautifier = NULL;
  voAlphabetKnowledge alphabetKnowledge = NULL;
  voLinguisticKnowledge linguisticKnowledge = NULL;
  voLinguisticKnowledge lkEnglishSecondary = NULL;
  voShapeKnowledge shapeKnowledge = NULL;

  voAnalyzerDocument analyzerResult = NULL;
  char* str = NULL;
  int length = 0;

  DocumentConfig config = {
    DOCUMENTTOSVG_DOC_WIDTH,
    DOCUMENTTOSVG_DOC_HEIGHT,
    DOCUMENTTOSVG_DOC_MARGIN,
    DOCUMENTTOSVG_STROKE_COLOR,
    DOCUMENTTOSVG_STROKE_WIDTH,
    DOCUMENTTOSVG_TEXT_HEIGHT_FACTOR,
    DOCUMENTTOSVG_DISPLAY_ORIGINAL_INK,
    DOCUMENTTOSVG_ORIGINAL_INK_COLOR,
    DOCUMENTTOSVG_ARROW_SHAPE_FACTOR,
    DOCUMENTTOSVG_ARROW_SIZE_FACTOR
  };


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

  printf("4) adding a new analyzer field to the content of the first page\n");
  analyzerField = VO_IFACE(engine, IContent)->addNewField(engine, content0, VO_CONTENT_FIELD_ANALYZER, NULL, &analyzerFieldId);
  verify(analyzerField != NULL);
  printf(" . analyzer field added successfully\n");
  printf("\n");

  printf("5) selecting all the first page\n");
  pageSelection = voCreatePageSelection(engine, page0);
  verify(pageSelection != NULL);
  verify(VO_IFACE(engine, ISelection)->selectAll(engine, pageSelection));
  printf(" . selection done successfully\n");
  printf("\n");

  printf("6) adding the selection to the analyzer field\n");
  verify(VO_IFACE(engine, IContentField)->addItems(engine, analyzerField, pageSelection));
  printf(" . selection added to the analyzer field successfully\n");
  printf("\n");

  printf("7) creating and configuring a new Analyzer\n");
  analyzer = voCreateAnalyzer(engine);
  verify(analyzer != NULL);
  printf(" . Analyzer created successfully\n");
  analyzerKnowledge = loadEngineObject(ANALYZER_KNOWLEDGE_RES);
  verify(analyzerKnowledge != NULL);
  printf(" . AnalyzerKnowledge loaded successfully\n");
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, analyzer, analyzerKnowledge));
  printf(" . AnalyzerKnowledge resource attached to the Analyzer successfully\n");
  verify(voReleaseObject(engine, analyzerKnowledge));
  printf(" . AnalyzerKnowledge resource released successfully\n");
  printf("\n");

  printf("8) creating and configuring a new StructuredInputRecognizer\n");
  textRecognizer = voCreateStructuredInputRecognizer(engine);
  verify(textRecognizer != NULL);
  printf(" . StructuredInputRecognizer created successfully\n");
  alphabetKnowledge = loadEngineObject(AK_RES);
  verify(alphabetKnowledge != NULL);
  printf(" . AlphabetKnowledge resource loaded successfully\n");
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

  printf("9) creating and configuring a new ShapeRecognizer and a new ShapeBeautifier\n");
  shapeRecognizer = voCreateShapeRecognizer(engine);
  verify(shapeRecognizer != NULL);
  printf(" . ShapeRecognizer created successfully\n");
  shapeBeautifier = voCreateShapeBeautifier(engine);
  verify(shapeBeautifier != NULL);
  printf(" . ShapeBeautifier created successfully\n");
  shapeKnowledge = loadEngineObject(ANALYZER_SHAPE_KNOWLEDGE_RES);
  verify(shapeKnowledge != NULL);
  printf(" . ShapeKnowledge resource loaded successfully\n");
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, shapeRecognizer, shapeKnowledge));
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, shapeBeautifier, shapeKnowledge));
  printf(" . ShapeKnowledge resource attached to the recognizer and the beautifier successfully\n");
  verify(voReleaseObject(engine, shapeKnowledge));
  printf("\n");

  printf("10) attaching the StructuredInputRecognizer, the ShapeRecognizer and the ShapeBeautifier to the Analyzer\n");
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, analyzer, textRecognizer));
  printf(" . StructuredInputRecognizer attached to the Analyzer successfully\n");
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, analyzer, shapeRecognizer));
  printf(" . ShapeRecognizer attached to the Analyzer successfully\n");
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, analyzer, shapeBeautifier));
  printf(" . ShapeBeautifier attached to the Analyzer successfully\n");
  printf("\n");

  printf("11) running the document analysis process\n");
  initializeTextUIProgressBar(&progressBar);
  verify(VO_IFACE(engine, IContentFieldProcessor)->process(engine, analyzer, analyzerField, updateTextUIProgressBar, &progressBar));
  finalizeTextUIProgressBar(&progressBar);
  printf(" . document analysis performed successfully\n");
  printf("\n");

  printf("12) retrieving the analysis result\n");
  analyzerResult = VO_IFACE(engine, IContentField)->getResult(engine, analyzerField);
  verify(analyzerResult != NULL);
  printf(" . analysis result retrieved successfully\n");
  printf("\n");

  printf("13) exploring the analyzed document\n");
  length = documentToSVG(engine, analyzerResult, NULL, 0, &config);
  if (length != -1)
  {
    str = malloc(sizeof(char) * length);

    if (str != NULL)
    {
      FILE* out;

      documentToSVG(engine, analyzerResult, str, length, &config);
       
      out = fileOpen(OUTPUT_DIR "/DocumentAnalysis-output.svg", "w");
      fwrite(str, 1, length, out);
      fclose(out);
      free(str);

      printf(" . analyzed document stored as SVG: ");
      printPath(stdout, OUTPUT_DIR "/DocumentAnalysis-output.svg");
      printf("\n");
      printf("\n");
    }
  }

  printf("14) cleaning-up: releasing all created objects and destroying the Engine\n");
  verify(voReleaseObject(engine, analyzerResult));

  verify(voReleaseObject(engine, shapeBeautifier));
  verify(voReleaseObject(engine, shapeRecognizer));
  verify(voReleaseObject(engine, textRecognizer));
  verify(voReleaseObject(engine, analyzer));

  verify(voReleaseObject(engine, pageSelection));
  verify(voReleaseObject(engine, analyzerField));

  verify(voReleaseObject(engine, content0));
  verify(voReleaseObject(engine, page0));
  verify(voReleaseObject(engine, document));
  verify(voDestroyEngine(engine));
  printf(" . clean-up done successfully\n");
  printf("\n");

  printf("end of example:\n");
  printf(" you just learnt how to perform an analysis on a document\n");

  // -- finalization -----------------------------------------------------------
  unloadMyScriptEngine();

  return EXIT_SUCCESS;
}
