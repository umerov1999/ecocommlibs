/**
 * The `CursiveRecognition` example code shows you how to perform handwriting
 * recognition on cursive digital ink.
 */


// -- configuration ------------------------------------------------------------

// common configuration parameters
#include "../common/config.h"

// include the MyScript Engine API header(s)
#include <c/MyScriptEngine.h>
#include <c/MyScriptText.h>
#include <c/MyScriptJSON.h>
#include <c/MyScriptInk.h>

// include the certificate
#include "../certificates/MyCertificate.h"


// -- example body -------------------------------------------------------------

#include <stdio.h>
#include <string.h>

// global engine (for the sake of readability)
voEngine engine = NULL;

// common utility functions
#include "../common/common.h"

// ink parsing helper
static bool parseInput(const char* path, voFloatStructuredInput input);

// main function
int main(int argc, char** argv)
{
  const char* default_argv[] = { argv[0], DATA_DIR "/hello_world_cursive.ink" };
  voStructuredInputRecognizer recognizer = NULL;
  voResource akCursive = NULL;
  voResource lkText = NULL;
  voResource lkEnglishSecondary = NULL;
  voInput input = NULL;

  int exitCode = EXIT_SUCCESS;
  int i;

  // -- check arguments --------------------------------------------------------
  if (argc == 1)
  {
    argc = (int)(sizeof(default_argv) / sizeof(default_argv[0]));
    argv = (char**)default_argv;
  }

  // -- check required settings have been edited -------------------------------
  // e.g. set the LANG variable to "en_US"
  if (strlen(LANG) == 0)
  {
    fprintf(stderr, "/!\\ please edit the common/config.h file and set:\n");
    fprintf(stderr, " . the LANG variable\n");
    fflush(stderr);
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

  printf("2) creating a new StructuredInputRecognizer\n");
  recognizer = voCreateStructuredInputRecognizer(engine);
  verify(recognizer != NULL);
  printf(" . StructuredInputRecognizer created successfully\n");
  printf("\n");

  printf("3) loading and attaching resources to the recognizer\n");
  akCursive = loadEngineObject(AK_CUR_RES);
  printf(" . CursiveAlphabetKnowledge resource loaded successfully\n");
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, recognizer, akCursive));
  printf(" . CursiveAlphabetKnowledge resource attached successfully\n");
  lkText = loadEngineObject(LK_TEXT_RES);
  printf(" . Main LinguisticKnowledge resource loaded successfully\n");
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, recognizer, lkText));
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
    verify(VO_IFACE(engine, IAttachTarget)->attach(engine, recognizer, lkEnglishSecondary));
    printf(" . Secondary English LinguisticKnowledge resource attached successfully\n");
  }
  printf("\n");

  printf("4) creating a FloatStructuredInput\n");
  input = voCreateFloatStructuredInput(engine);
  verify(input != NULL);
  printf(" . FloatStructuredInput created successfully\n");
  printf("\n");

  printf("5) setting the input as the source of the recognizer\n");
  verify(VO_IFACE(engine, IInputConsumer)->setSource(engine, recognizer, input));
  printf(" . input successfully set as the recognizer's source\n");
  printf("\n");

  printf("6) processing input files\n");

  for (i = 1; i < argc; ++i)
  {
    voRecognitionResult result = NULL;
    voCandidateIterator iterator = NULL;

    if (!fileExists(argv[i]))
    {
      fprintf(stderr, "missing ink file: %s\n", argv[i]);
      exitCode = EXIT_FAILURE;
      break;
    }
   
    printf(" . parsing input ink ");
    printPath(stdout, argv[i]);
    printf("\n");
    verify(parseInput(argv[i], input));

    printf("   . input fueled successfully\n");
    printf("\n");

    printf(" . running the handwriting recognition process\n");
    verify(VO_IFACE(engine, IRunnable)->run(engine, recognizer, NULL, NULL));
    printf("   . handwriting recognition performed successfully\n");
    printf("\n");

    printf(" . retrieving the RecognitionResult\n");
    result = VO_IFACE(engine, IRecognitionResultProvider)->getResult(engine, recognizer);
    verify(result != NULL);
    printf("   . RecognitionResult retrieved successfully\n");
    printf("\n");

    printf(" . retrieving the first candidate of the RecognitionResult\n");
    iterator = VO_IFACE(engine, IRecognitionResult)->getCandidates(engine, result);
    verify(iterator != NULL);
    if (VO_IFACE(engine, IIterator)->isAtEnd(engine, iterator))
      printf("   . sorry, unable to recognize the input\n");
    else
    {
      voString label = {NULL, 0};
      verify(VO_IFACE(engine, ICandidateIterator)->getLabel(engine, iterator, NULL, &label));
      label.bytes = malloc(label.byteCount + 1);
      verify(VO_IFACE(engine, ICandidateIterator)->getLabel(engine, iterator, NULL, &label));
      label.bytes[label.byteCount] = '\0';
      printf("   . input recognized as: \"%s\"\n", label.bytes);
      free(label.bytes);
    }
    printf("\n");

    verify(voReleaseObject(engine, iterator));
    verify(voReleaseObject(engine, result));

    verify(VO_IFACE(engine, IFloatStructuredInput)->clear(engine, input, false));

  } // end of: for (i = 1; i < argc; ++i)

  printf("7) cleaning-up: releasing all created objects and destroying the Engine\n");
  verify(voReleaseObject(engine, input));
  if (lkEnglishSecondary != NULL)
    verify(voReleaseObject(engine, lkEnglishSecondary));
  verify(voReleaseObject(engine, lkText));
  verify(voReleaseObject(engine, akCursive));
  verify(voReleaseObject(engine, recognizer));
  verify(voDestroyEngine(engine));
  printf(" . clean-up done successfully\n");
  printf("\n");

  printf("end of example:\n");
  printf(" you just learnt how to perform handwriting recognition on cursive digital ink\n");

  // -- finalization -----------------------------------------------------------
  unloadMyScriptEngine();

  return exitCode;
}


// -- parseInput helper --------------------------------------------------------

static bool parseInput(const char* path, voFloatStructuredInput input)
{
  voInk ink = NULL;
  voInkStroke stroke = NULL;
  bool ok = false;

  for (;;)
  {
    int i, count;
    voPoint* points = NULL;

    ink = loadEngineObject(path);
    if (ink == NULL)
     break;

    count = VO_IFACE(engine, IInkStrokeSet)->getStrokeCount(engine, ink);
    if (count == -1)
      break;

    for (i = 0; i < count; ++i)
    {
      int pointCount;

      stroke = VO_IFACE(engine, IInkStrokeSet)->getStrokeAt(engine, ink, i);
      if (stroke == NULL)
        break;

      pointCount = VO_IFACE(engine, IInkStroke)->getSampleCount(engine, stroke);
      if (pointCount == -1)
        break;

      if (VO_IFACE(engine, IInkStroke)->isPointArrayCompatible(engine, stroke) == VO_YES)
      {
        const voPoint* points = VO_IFACE(engine, IInkStroke)->getPointArray(engine, stroke);
        if (points == NULL)
          break;

        if (!VO_IFACE(engine, IFloatStructuredInput)->addStroke(engine, input, &points->x, sizeof(voPoint), &points->y, sizeof(voPoint), pointCount))
          break;
      }
      else
      {
        voPoint* tmp = realloc(points, sizeof(voPoint) * pointCount);
        if (tmp == NULL)
          break;
        points = tmp;
 
        if (!VO_IFACE(engine, IInkStroke)->getPoints(engine, stroke, 0, pointCount, points))
          break;

        if (!VO_IFACE(engine, IFloatStructuredInput)->addStroke(engine, input, &points->x, sizeof(voPoint), &points->y, sizeof(voPoint), pointCount))
          break;
      }

      voReleaseObject(engine, stroke);
      stroke = NULL;
    }

    ok = i == count;

    free(points);
    break; 
  }

  if (stroke != NULL)
    voReleaseObject(engine, stroke);
  if (ink != NULL)
    voReleaseObject(engine, ink);

  return ok;
}
