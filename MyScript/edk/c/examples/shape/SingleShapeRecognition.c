/**
 * The `SingleShapeRecognition` example code shows you how to perform
 * recognition of a single handwritten shape.
 */


// -- configuration ------------------------------------------------------------

// common configuration parameters
#include "../common/config.h"

// include the MyScript Engine API header(s)
#include <c/MyScriptEngine.h>
#include <c/MyScriptShape.h>
#include <c/MyScriptJSON.h>
#include <c/MyScriptInk.h>

// include the certificate
#include "../certificates/MyCertificate.h"


// -- example body -------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>

// global engine (for the sake of readability)
voEngine engine = NULL;

// common utility functions
#include "../common/common.h"

// ink parsing helper
static bool parseInput(const char* path, voShapeDocument document);

// main function
int main(int argc, char** argv)
{
  const char* default_argv[] = { argv[0], DATA_DIR "/shape.ink" };
  voShapeRecognizer recognizer = NULL;
  voShapeKnowledge shapeKnowledge = NULL;
  voShapeDocument document = NULL;
  voShapeCandidate segment = NULL;
  uint32_t count = 0;
  uint32_t i = 0;
  int exitCode = EXIT_SUCCESS;
  int j;

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

  printf("2) creating a new ShapeRecognizer\n");
  recognizer = voCreateShapeRecognizer(engine);
  verify(recognizer != NULL);
  printf(" . ShapeRecognizer created successfully\n");
  printf("\n");

  printf("3) loading and attaching the ShapeKnowledge to the ShapeRecognizer\n");
  shapeKnowledge = loadEngineObject(SHAPE_KNOWLEDGE_RES);
  printf(" . ShapeKnowledge resource loaded successfully\n");
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, recognizer, shapeKnowledge));
  printf(" . ShapeKnowledge resource successfully attached to the ShapeRecognizer\n");
  printf(" . releasing the ShapeKnowledge resource (not needed anymore)\n");
  verify(voReleaseObject(engine, shapeKnowledge));
  printf(" . ShapeKnowledge resource released successfully\n");
  printf("\n");

  printf("4) creating a new ShapeDocument\n");
  document = voCreateShapeDocument(engine);
  verify(document != NULL);
  printf(" . ShapeDocument created successfully\n");
  printf("\n");

  printf("5) processing input files\n");
  
  for (j = 1; j < argc; ++j)
  {
    if (!fileExists(argv[j]))
    {
      fprintf(stderr, "missing ink file: %s\n", argv[j]);
      exitCode = EXIT_FAILURE;
      break;
    }
  
    printf(" . parsing input ink ");
    printPath(stdout, argv[j]);
    printf("\n");

    // as we are doing single shape recognition,
    // we force the segmentation by issuing startSegment()/endSegment() calls
    verify(VO_IFACE(engine, IShapeDocument)->startSegment(engine, document));
    verify(parseInput(argv[j], document));
    verify(VO_IFACE(engine, IShapeDocument)->endSegment(engine, document));
    printf("   . ShapeDocument successfully fueled with digital ink\n");
    printf("\n");

    printf(" . running the shape recognition process\n");
    verify(VO_IFACE(engine, IShapeDocumentProcessor)->process(engine, recognizer, document, NULL, NULL));
    printf("   . shape recognition performed successfully\n");
    printf("\n");

    printf(" . retrieving the recognition result\n");
    segment = VO_IFACE(engine, IShapeDocument)->getSegmentAt(engine, document, 0);
    printf("   . first ShapeSegment retrieved successfully\n");
    verify(segment != NULL);
    printf("   . retrieving the number of ShapeCandidates in the ShapeSegment: ");
    count = VO_IFACE(engine, IShapeSegment)->getCandidateCount(engine, segment);
    verify(count != (uint32_t)-1);
    printf("%d\n", count);
    printf("   . iterating over the ShapeRecognized candidates:\n");
    for (i = 0; i < count; ++i)
    {
      voShapeCandidate candidate = NULL;

      candidate = VO_IFACE(engine, IShapeSegment)->getCandidateAt(engine, segment, i);
      verify(candidate != NULL);

      switch(voGetType(engine, candidate))
      {
        case VO_ShapeRecognized:
          {
            voShapeModel model = NULL;
            voString label = {NULL, 0};

            float rs = 0;
            float nrs = 0;

            model = VO_IFACE(engine, IShapeRecognized)->getModel(engine, candidate);
            verify(model != NULL);

            verify(VO_IFACE(engine, IShapeModel)->getLabel(engine, model, NULL, &label));
            label.bytes = malloc(label.byteCount + 1);
            verify(VO_IFACE(engine, IShapeModel)->getLabel(engine, model, NULL, &label));
            label.bytes[label.byteCount] = '\0';

            rs = VO_IFACE(engine, IShapeRecognized)->getResemblanceScore(engine, candidate);
            nrs = VO_IFACE(engine, IShapeRecognized)->getNormalizedRecognitionScore(engine, candidate);

            printf("     . %s (rs:%.4f - nrs:%.4f)\n", label.bytes, rs, nrs);

            free(label.bytes);
            // don't forget to release objects returned by the API once you're done
            verify(voReleaseObject(engine, model));
          }
          break;

        case -1:
          printf("     . unable to get candidate type\n");
          break;

        default:
          // ignore other types of candidates
          break;
      }

      verify(voReleaseObject(engine, candidate));
    }
    printf("\n");

    verify(voReleaseObject(engine, segment));
    verify(VO_IFACE(engine, IShapeDocument)->clear(engine, document));

  } // end of: for (j = 1; j < argc; ++j)
  

  printf("6) cleaning-up: releasing all created objects and destroying the Engine\n");
  verify(voReleaseObject(engine, document));
  verify(voReleaseObject(engine, recognizer));
  verify(voDestroyEngine(engine));
  printf(" . clean-up done successfully\n");
  printf("\n");

  printf("end of example:\n");
  printf(" you just learnt how to perform single shape recognition\n");

  // -- finalization -----------------------------------------------------------
  unloadMyScriptEngine();

  return exitCode;
}


// -- parseInput helper --------------------------------------------------------

static bool parseInput(const char* path, voShapeDocument document)
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

        if (!VO_IFACE(engine, IShapeDocument)->addStroke(engine, document, &points->x, sizeof(voPoint), &points->y, sizeof(voPoint), pointCount))
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

        if (!VO_IFACE(engine, IShapeDocument)->addStroke(engine, document, &points->x, sizeof(voPoint), &points->y, sizeof(voPoint), pointCount))
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
