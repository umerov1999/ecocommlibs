/**
 * The `StructuredInkIndexing` example code shows you how to perform ink
 * indexing on a structured input.
 */


// -- configuration ------------------------------------------------------------

// common configuration parameters
#include "../common/config.h"

// include the MyScript Engine API header(s)
#include <c/MyScriptEngine.h>
#include <c/MyScriptInkSearch.h>

// include the certificate
#include "../certificates/MyCertificate.h"


// -- example body -------------------------------------------------------------

#include <stdio.h>

// global engine (for the sake of readability)
voEngine engine = NULL;

// common utility functions
#include "../common/common.h"

// digital ink
static float x0[] = {374, 374, 374, 374, 376, 378, 379, 381, 384, 387, 389, 390};
static float y0[] = {493, 491, 493, 499, 511, 520, 522, 518, 509, 499, 493, 494};
static float x1[] = {386, 388, 389, 391, 391, 391, 391, 391, 391, 394};
static float y1[] = {518, 517, 516, 514, 511, 511, 513, 516, 517, 517};
static float x2[] = {397, 396};
static float y2[] = {502, 502};
static float x3[] = {397, 398, 400, 400, 403, 404, 405, 404, 402, 400, 400, 402, 404, 406, 406, 407, 407};
static float y3[] = {519, 518, 516, 514, 514, 516, 518, 520, 522, 520, 518, 516, 514, 514, 516, 518, 520};
static float x4[] = {409, 409, 408};
static float y4[] = {505, 503, 503};
static float x5[] = {413, 414, 412, 412, 412, 414, 416, 417, 417, 415, 413};
static float y5[] = {517, 516, 516, 518, 521, 522, 520, 517, 514, 513, 515};
static float x6[] = {420, 420, 420, 420, 421, 422, 423, 424, 424, 425, 426, 428, 429, 430, 432, 433, 435, 435};
static float y6[] = {518, 517, 515, 513, 511, 511, 511, 514, 517, 518, 518, 516, 514, 514, 515, 518, 519, 519};

// path of the file where the index will be stored.
#define INDEX_FILE OUTPUT_DIR "/InkIndexing-output.index"

// main function
int main()
{
  voStructuredInput input = NULL;
  voString string = {" Objects", -1};
  voIndex index = NULL;
  voStructuredInputIndexer indexer = NULL;
  voResource ak = NULL;
  voResource lk = NULL;
  voResource lkEnglishSecondary = NULL;

  if (strlen(LANG) == 0 || strlen(HWSTYLE) == 0)
  {
    fprintf(stderr, "/!\\ please edit the common/config.h file and set:\n");
    fprintf(stderr, " . the LANG variable\n");
    fprintf(stderr, " . the HWSTYLE variable\n");
    fflush(stderr);
    exit(EXIT_FAILURE);
  }

  if (strlen(INDEX_FILE) == 0)
  {
    printf("/!\\ please edit this example program setting the INDEX_FILE variable to a valid file name\n");
    exit(EXIT_FAILURE);
  }
  
  // -- library initialization -------------------------------------------------
  if (!loadMyScriptEngine(MYSCRIPT_ENGINE_LIBRARY_DIR, NULL))
    return (fprintf(stderr, "failed to load the MyScriptEngine library\n"), EXIT_FAILURE);

  // -- example code -----------------------------------------------------------
  printf("1) creating a new Engine initialized with your certificate\n");
  engine = voCreateEngine(VO_MSE_VER, &myCertificate, NULL);
  verify(engine != NULL);
  printf(" . Engine created successfully\n");
  printf("\n");

  printf("2) creating a new StructuredInputIndexer\n");
  indexer = voCreateStructuredInputIndexer(engine);
  verify(indexer != NULL);
  printf(" . StructuredInputIndexer created successfully\n");
  printf("\n");

  printf("3) loading and attaching resources to the indexer\n");
  ak = loadEngineObject(AK_RES);
  printf(" . AlphabetKnowledge resource loaded successfully\n");
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, indexer, ak));
  printf(" . AlphabetKnowledge resource attached successfully\n");
  lk = loadEngineObject(LK_TEXT_RES);
  printf(" . Main LinguisticKnowledge resource loaded successfully\n");
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, indexer, lk));
  printf(" . Main LinguisticKnowledge resource attached successfully\n");
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
    verify(VO_IFACE(engine, IAttachTarget)->attach(engine, indexer, lkEnglishSecondary));
    printf(" . Secondary English LinguisticKnowledge resource attached successfully\n");
  }
  printf("\n");

  printf("4) creating an FloatStructuredInput\n");
  input = voCreateFloatStructuredInput(engine);
  verify(input != NULL);
  printf(" . FloatStructuredInput created successfully\n");
  printf("\n");

  printf("5) fueling the input with strokes\n");
  verify(VO_IFACE(engine, IFloatStructuredInput)->startInputUnit(engine, input, VO_SINGLE_LINE_TEXT));
  verify(VO_IFACE(engine, IFloatStructuredInput)->addStroke(engine, input, x0, sizeof(*x0), y0, sizeof(*y0), (int)(sizeof(x0)/sizeof(*x0))));
  verify(VO_IFACE(engine, IFloatStructuredInput)->addStroke(engine, input, x1, sizeof(*x1), y1, sizeof(*y1), (int)(sizeof(x1)/sizeof(*x1))));
  verify(VO_IFACE(engine, IFloatStructuredInput)->addStroke(engine, input, x2, sizeof(*x2), y2, sizeof(*y2), (int)(sizeof(x2)/sizeof(*x2))));
  verify(VO_IFACE(engine, IFloatStructuredInput)->addStroke(engine, input, x3, sizeof(*x3), y3, sizeof(*y3), (int)(sizeof(x3)/sizeof(*x3))));
  verify(VO_IFACE(engine, IFloatStructuredInput)->addStroke(engine, input, x4, sizeof(*x4), y4, sizeof(*y4), (int)(sizeof(x4)/sizeof(*x4))));
  verify(VO_IFACE(engine, IFloatStructuredInput)->addStroke(engine, input, x5, sizeof(*x5), y5, sizeof(*y5), (int)(sizeof(x5)/sizeof(*x5))));
  verify(VO_IFACE(engine, IFloatStructuredInput)->addStroke(engine, input, x6, sizeof(*x6), y6, sizeof(*y6), (int)(sizeof(x6)/sizeof(*x6))));
  verify(VO_IFACE(engine, IFloatStructuredInput)->addString(engine, input, NULL, &string));
  verify(VO_IFACE(engine, IFloatStructuredInput)->endInputUnit(engine, input, VO_SINGLE_LINE_TEXT));
  printf(" . input fueled successfully\n");
  printf("\n");

  printf("6) setting the input as the source of the indexer\n");
  verify(VO_IFACE(engine, IInputConsumer)->setSource(engine, indexer, input));
  printf(" . input successfully set as the indexer's source\n");
  printf("\n");

  printf("7) running the indexing process\n");
  verify(VO_IFACE(engine, IRunnable)->run(engine, indexer, NULL, NULL));
  printf(" . indexing performed successfully\n");
  printf("\n");

  printf("8) retrieving the Index\n");
  index = VO_IFACE(engine, IIndexingResultProvider)->getResult(engine, indexer);
  verify(index != NULL);
  printf(" . Index retrieved successfully\n");
  printf("\n");

  printf("9) storing the Index in a file for later search\n");
  storeEngineObject(index, INDEX_FILE);
  printf(" . Index stored successfully to: ");
  printPath(stdout, INDEX_FILE);
  printf("\n");
  printf("\n");

  printf("10) cleaning-up: releasing all created objects and destroying the Engine\n");
  verify(voReleaseObject(engine, index));
  verify(voReleaseObject(engine, input));
  verify(voReleaseObject(engine, ak));
  if (lkEnglishSecondary != NULL)
    verify(voReleaseObject(engine, lkEnglishSecondary));
  verify(voReleaseObject(engine, lk));
  verify(voReleaseObject(engine, indexer));
  verify(voDestroyEngine(engine));
  printf(" . clean-up done successfully\n");
  printf("\n");

  printf("end of example:\n");
  printf(" you just learnt how to perform ink indexing on a structured input\n");

  // -- finalization -----------------------------------------------------------
  unloadMyScriptEngine();

  return EXIT_SUCCESS;
}
