/**
 * The `MathRecognition` example code shows you how to perform math
 * recognition.
 */


// -- configuration ------------------------------------------------------------

// common configuration parameters
#include "../common/config.h"

// include the MyScript Engine API header(s)
#include <c/MyScriptEngine.h>
#include <c/MyScriptMath.h>
#include <c/MyScriptJSON.h>
#include <c/MyScriptInk.h>

// include the certificate
#include "../certificates/MyCertificate.h"


// -- example body -------------------------------------------------------------

#include <stdio.h>

// global engine (for the sake of readability)
voEngine engine = NULL;

// common utility functions
#include "../common/common.h"

// LaTeX serialization helper
#include <../src/MyScriptMath-parseTreeToLaTeX.h>
// MathML serialization helper
#include <../src/MyScriptMath-parseTreeToMathML.h>

// ink parsing helper
static bool parseInput(const char* path, voMathDocument math);

// main function
int main(int argc, char** argv)
{
  const char* default_argv[] = { argv[0], DATA_DIR "/math.ink" };
  voMathRecognizer recognizer = NULL;
  voMathAlphabetKnowledge ak = NULL;
  voMathGrammar grm = NULL;
  voMathDocument math = NULL;
  char* str = NULL;
  int length = 0;
  int exitCode = EXIT_SUCCESS;
  int i;

  // -- check arguments --------------------------------------------------------
  if (argc == 1)
  {
    argc = (int)(sizeof(default_argv) / sizeof(default_argv[0]));
    argv = (char**)default_argv;
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

  printf("2) creating a new MathRecognizer\n");
  recognizer = voCreateMathRecognizer(engine);
  verify(recognizer != NULL);
  printf(" . MathRecognizer created successfully\n");
  printf("\n");

  printf("3) loading and attaching resources to the recognizer\n");
  ak = loadEngineObject(MATH_AK_RES);
  printf(" . MathAlphabetKnowledge loaded successfully\n");
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, recognizer, ak));
  printf(" . MathAlphabetKnowledge resource successfully attached to the MathRecognizer\n");
  printf(" . releasing the MathAlphabetKnowledge resource (not needed anymore)\n");
  verify(voReleaseObject(engine, ak));
  printf(" . MathAlphabetKnowledge resource released successfully\n");
  grm = loadEngineObject(MATH_GRM_RES);
  printf(" . MathGrammar loaded successfully\n");
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, recognizer, grm));
  printf(" . MathGrammar resource successfully attached to the MathRecognizer\n");
  printf(" . releasing the MathGrammar resource (not needed anymore)\n");
  verify(voReleaseObject(engine, grm));
  printf(" . MathGrammar resource released successfully\n");
  printf("\n");

  printf("4) creating a new MathDocument object\n");
  math = voCreateMathDocument(engine);
  verify(math != NULL);
  printf(" . MathDocument created successfully\n");
  printf("\n");

  printf("5) processing input files\n");
  
  for (i = 1; i < argc; ++i)
  {
    voMathNode root = NULL;

    if (!fileExists(argv[i]))
    {
        fprintf(stderr, "missing ink file: %s\n", argv[i]);
        exitCode = EXIT_FAILURE;
        break;
    }
   
    printf(" . parsing input ink ");
    printPath(stdout, argv[i]);
    printf("\n");
    verify(parseInput(argv[i], math));

    printf("   . input fueled successfully\n");
    printf("\n");

    printf(" . running the math recognition process\n");
    verify(VO_IFACE(engine, IMathRecognizer)->process(engine, recognizer, math, NULL, NULL));
    printf("   . math recognition performed successfully\n");
    printf("\n");

    printf(" . retrieving the recognition parse tree\n");
    if (VO_IFACE(engine, IMathDocument)->hasParseTree(engine, math))
    {
      root = VO_IFACE(engine, IMathDocument)->getParseTree(engine, math);
      verify(root != NULL);
      printf("   . parse tree retrieved successfully\n");
      printf("\n");

      printf(" . exploring the recognition parse tree\n");
      length = parseTreeToLaTeX(engine, root, NULL, 0);
      if (length != -1)
      {
        str = malloc(sizeof(char) * (length + 1));

        if (str != NULL)
        {
          parseTreeToLaTeX(engine, root, str, length);
          str[length] = '\0';
          printf("   . math recognized as LaTeX string: \n\n%s\n", str);
          free(str);
          printf("\n");
        }
      }
      length = parseTreeToMathML(engine, root, NULL, 0);
      if (length != -1)
      {
        str = malloc(sizeof(char) * (length + 1));

        if (str != NULL)
        {
          parseTreeToMathML(engine, root, str, length);
          str[length] = '\0';
          printf("   . math recognized as MathML string: \n\n%s\n", str);
          free(str);
          printf("\n");
        }
      }
    }
    else
    {
      printf("   . no recognition result\n");
      printf("\n");
    }

    verify(voReleaseObject(engine, root));

    verify(VO_IFACE(engine, IMathDocument)->clear(engine, math));

  } // end of: for (i = 1; i < argc; ++i)

  printf("6) cleaning-up: releasing all created objects and destroying the Engine\n");
  verify(voReleaseObject(engine, math));
  verify(voReleaseObject(engine, recognizer));
  verify(voDestroyEngine(engine));
  printf("\n");

  printf("end of example:\n");
  printf(" you just learnt how to perform math recognition\n");

  // -- finalization -----------------------------------------------------------
  unloadMyScriptEngine();

  return exitCode;
}


// -- parseInput helper --------------------------------------------------------

static bool parseInput(const char* path, voMathDocument math)
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

        if (!VO_IFACE(engine, IMathDocument)->addStroke(engine, math, &points->x, sizeof(voPoint), &points->y, sizeof(voPoint), pointCount))
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

        if (!VO_IFACE(engine, IMathDocument)->addStroke(engine, math, &points->x, sizeof(voPoint), &points->y, sizeof(voPoint), pointCount))
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
