/**
 * The `DocumentAnalysis` example code shows you how to perform document
 * analysis, recognition and beautification.
 */


// -- configuration ------------------------------------------------------------

// common configuration parameters
#include "../common/config.h"

// ink parsing
#include "../common/parseInk.h"

// include the MyScript Engine API header(s)
#include <c/MyScriptEngine.h>
#include <c/MyScriptAnalyzer.h>

// include the certificate
#include "../certificates/MyCertificate.h"


// -- example body -------------------------------------------------------------

#include <stdio.h>

// global engine (for the sake of readability)
voEngine engine = NULL;

// common utility functions
#include "../common/common.h"

// Svg serialization helper
#include <../src/MyScriptAnalyzer-documentToSVG.h>

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

// -- parseInk() callback ------------------------------------------------------

struct AppContext
{
  voEngine engine;
  voAnalyzerDocument document;
};

static int parseInkCallback(const float* x, size_t xByteStride,
                            const float* y, size_t yByteStride,
                            int pointCount,
                            void* userParam)
{
  bool successful;
  struct AppContext* context = (struct AppContext*)userParam;

  successful = VO_IFACE(engine, IAnalyzerDocument)->addStroke(context->engine, context->document, x, xByteStride, y, yByteStride, pointCount);

  return successful ? 0 : voGetError(context->engine);
}


// -- main function ------------------------------------------------------------

int main()
{
  voAnalyzer analyzer = NULL;
  voAnalyzerKnowledge ank = NULL;
  voStructuredInputRecognizer textRecognizer = NULL;
  voShapeRecognizer shapeRecognizer = NULL;
  voShapeBeautifier shapeBeautifier = NULL;
  voResource ak = NULL;
  voResource lk = NULL;
  voResource lkEnglishSecondary = NULL;
  voResource shk = NULL;
  voAnalyzerDocument document = NULL;
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

  // -- example code -----------------------------------------------------------
  printf("1) creating a new Engine initialized with your certificate\n");
  engine = voCreateEngine(VO_MSE_VER, &myCertificate, NULL);
  verify(engine != NULL);
  printf(" . Engine created successfully\n");
  printf("\n");

  printf("2) creating a new Analyzer\n");
  analyzer = voCreateAnalyzer(engine);
  verify(analyzer != NULL);
  printf(" . Analyzer created successfully\n");
  printf("\n");

  printf("3) loading and attaching resource to the Analyzer\n");
  ank = loadEngineObject(ANALYZER_KNOWLEDGE_RES);
  verify(ank != NULL);
  printf(" . AnalyzerKnowledge loaded successfully\n");
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, analyzer, ank));
  printf(" . AnalyzerKnowledge resource successfully attached to the Analyzer\n");
  printf(" . releasing the AnalyzerKnowledge resource (not needed anymore)\n");
  verify(voReleaseObject(engine, ank));
  printf(" . AnalyzerKnowledge resource released successfully\n");
  printf("\n");

  printf("4) creating and configuring a new StructuredInputRecognizer\n");
  textRecognizer = voCreateStructuredInputRecognizer(engine);
  verify(textRecognizer != NULL);
  printf(" . StructuredInputRecognizer created successfully\n");
  printf(" . loading and attaching resources to the StructuredInputRecognizer\n");
  ak = loadEngineObject(AK_RES);
  verify(ak != NULL);
  printf(" . AlphabetKnowledge resource loaded successfully\n");
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, textRecognizer, ak));
  printf(" . AlphabetKnowledge resource attached successfully\n");
  printf(" . releasing the AlphabetKnowledge resource (not needed anymore)\n");
  verify(voReleaseObject(engine, ak));
  printf(" . AlphabetKnowledge resource released successfully\n");
  lk = loadEngineObject(LK_TEXT_RES);
  verify(lk != NULL);
  printf(" . Main LinguisticKnowledge resource loaded successfully\n");
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, textRecognizer, lk));
  printf(" . Main LinguisticKnowledge resource attached successfully\n");
  printf(" . releasing the main LinguisticKnowledge resource (not needed anymore)\n");
  verify(voReleaseObject(engine, lk));
  printf(" . Main LinguisticKnowledge resource released successfully\n");
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
    printf(" . Secondary English LinguisticKnowledge resource attached successfully\n");
    printf(" . releasing the secondary LinguisticKnowledge resource (not needed anymore)\n");
    verify(voReleaseObject(engine, lkEnglishSecondary));
    printf(" . Secondary LinguisticKnowledge resource released successfully\n");
  }
  printf("\n");

  printf("5) creating and configuring a new ShapeRecognizer and a new ShapeBeautifier\n");
  shapeRecognizer = voCreateShapeRecognizer(engine);
  verify(shapeRecognizer != NULL);
  printf(" . ShapeRecognizer created successfully\n");
  shapeBeautifier = voCreateShapeBeautifier(engine);
  verify(shapeBeautifier != NULL);
  printf(" . ShapeBeautifier created successfully\n");
  printf(" . loading and attaching resources to the ShapeRecognizer and ShapeBeautifier\n");
  shk = loadEngineObject(ANALYZER_SHAPE_KNOWLEDGE_RES);
  verify(shk != NULL);
  printf(" . ShapeKnowledge resource loaded successfully\n");
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, shapeRecognizer, shk));
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, shapeBeautifier, shk));
  printf(" . ShapeKnowledge resource attached successfully\n");
  printf(" . releasing the ShapeKnowledge resource (not needed anymore)\n");
  verify(voReleaseObject(engine, shk));
  printf(" . ShapeKnowledge resource released successfully\n");
  printf("\n");

  printf("6) attaching the StructuredInputRecognizer, the ShapeRecognizer and the ShapeBeautifier to the Analyzer\n");
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, analyzer, textRecognizer));
  printf(" . StructuredInputRecognizer successfully attached to the Analyzer\n");
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, analyzer, shapeRecognizer));
  printf(" . ShapeRecognizer successfully attached to the Analyzer\n");
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, analyzer, shapeBeautifier));
  printf(" . ShapeBeautifier successfully attached to the Analyzer\n");
  printf("\n");

  printf("7) creating a new AnalyzerDocument\n");
  document = voCreateAnalyzerDocument(engine);
  verify(document != NULL);
  printf(" . AnalyzerDocument created successfully\n");
  printf("\n");

  printf("8) fueling the AnalyzerDocument with digital ink\n");
  {
    struct AppContext context = { engine, document };
    FILE* file = fileOpen(DATA_DIR "/document.ink", "r");
    if (file != NULL)
    {
      verify(parseInk(file, parseInkCallback, &context) != EXIT_FAILURE);
      fclose(file);
    }
  }
  printf(" . AnalyzerDocument successfully fueled with digital ink\n");
  printf("\n");

  printf("9) running the document analysis process\n");
  verify(VO_IFACE(engine, IAnalyzerDocumentProcessor)->process(engine, analyzer, document, NULL, NULL));
  printf(" . document analysis performed successfully\n");
  printf("\n");

  printf("10) exploring the analyzed document\n");
  length = documentToSVG(engine, document, NULL, 0, &config);
  if (length != -1)
  {
    str = malloc(sizeof(char) * length);

    if (str != NULL)
    {
      FILE* out;

      documentToSVG(engine, document, str, length, &config);

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

  printf("11) cleaning-up: releasing all created objects and destroying the Engine\n");
  verify(voReleaseObject(engine, document));
  verify(voReleaseObject(engine, shapeBeautifier));
  verify(voReleaseObject(engine, shapeRecognizer));
  verify(voReleaseObject(engine, textRecognizer));
  verify(voReleaseObject(engine, analyzer));
  verify(voDestroyEngine(engine));
  printf(" . clean-up done successfully\n");
  printf("\n");

  printf("end of example:\n");
  printf(" you just learnt how to perform a document analysis\n");

  // -- finalization -----------------------------------------------------------
  unloadMyScriptEngine();

  return EXIT_SUCCESS;
}
