/**
 * The `AdjustingModelDetectionSensitivity` example code shows you how to adjust
 * models' detection sensitivity.
 */


// -- configuration ------------------------------------------------------------

// common configuration parameters
#include "../common/config.h"

// include the MyScript Engine API header(s)
#include <c/MyScriptEngine.h>
#include <c/MyScriptShape.h>

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
#include "almost_rectangle.strokes.h"

// forward declaration of example functions
static void printResult(voShapeDocument document);

// main function
int main()
{
  voShapeRecognizer recognizer = NULL;
  voShapeKnowledge shapeKnowledge = NULL;
  voShapeDocument document = NULL;
  voShapeDocument snapshot = NULL;
  uint32_t count = 0;
  uint32_t i = 0;

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

  printf("3) loading and attaching the ShapeKnowledge\n");
  shapeKnowledge = loadEngineObject(SHAPE_KNOWLEDGE_RES);
  printf(" . ShapeKnowledge resource loaded successfully\n");
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, recognizer, shapeKnowledge));
  printf(" . ShapeKnowledge resource successfully attached to the ShapeRecognizer\n");
  printf(" . releasing the ShapeKnowledge resource (not needed anymore)\n");
  verify(voReleaseObject(engine, shapeKnowledge));
  printf(" . ShapeKnowledge resource released successfully\n");
  printf("\n");

  printf("4) creating a ShapeDocument object\n");
  document = voCreateShapeDocument(engine);
  verify(document != NULL);
  printf(" . ShapeDocument created successfully\n");
  printf("\n");

  printf("5) fueling the ShapeDocument with digital ink\n");
  printf(" . adding almost a rectangle\n");
  printf("     ,----------.\n");
  printf("    /            \\\n");
  printf("   |              |\n");
  printf("   |              |\n");
  printf("    \\            /\n");
  printf("     '----------'\n");
  // as we are doing single shape recognition,
  // we force the segmentation by issuing startSegment()/endSegment() calls
  verify(VO_IFACE(engine, IShapeDocument)->startSegment(engine, document));
  verify(VO_IFACE(engine, IShapeDocument)->addStroke(engine, document, &almost_rectangle_s0[0], 2 * sizeof(*almost_rectangle_s0), &almost_rectangle_s0[1], 2 * sizeof(*almost_rectangle_s0), (uint32_t)(sizeof(almost_rectangle_s0) / sizeof(*almost_rectangle_s0)) / 2));
  verify(VO_IFACE(engine, IShapeDocument)->endSegment(engine, document));
  printf(" . ShapeDocument successfully fueled with digital ink\n");
  printf("\n");

  printf("6) taking a snapshot of the ShapeDocument before recognition\n");
  snapshot = voCloneObject(engine, document);
  verify(snapshot != NULL);
  printf(" . snapshot taken successfully\n");
  printf("\n");

  printf("7) running the shape recognition process\n");
  verify(VO_IFACE(engine, IShapeDocumentProcessor)->process(engine, recognizer, document, NULL, NULL));
  printf(" . shape recognition performed successfully\n");
  printf("\n");

  printf("8) retrieving the recognition result\n");
  printResult(document);
  printf("\n");

  printf("9) disabling ellipse and circle ShapeModels\n");
  count = VO_IFACE(engine, IShapeRecognizer)->getModelCount(engine, recognizer);
  verify(count != (uint32_t)-1);

  for (i = 0; i < count; ++i)
  {
    voShapeModel model = NULL;
    voString label = {NULL, 0};

    model = VO_IFACE(engine, IShapeRecognizer)->getModelAt(engine, recognizer, i);
    verify(model != NULL);

    verify(VO_IFACE(engine, IShapeModel)->getLabel(engine, model, NULL, &label));
    label.bytes = malloc(label.byteCount + 1);
    verify(VO_IFACE(engine, IShapeModel)->getLabel(engine, model, NULL, &label));
    label.bytes[label.byteCount] = '\0';

    if (strcmp("ellipse", label.bytes) == 0 || strcmp("circle", label.bytes) == 0)
      verify(VO_IFACE(engine, IShapeRecognizer)->setModelDetectionSensitivity(engine, recognizer, model, 0));

    free(label.bytes);
    verify(voReleaseObject(engine, model));
  }
  printf(" . ellipse and circle ShapeModels disabled successfully\n");
  printf("\n");

  printf("10) swapping the ShapeDocument with its snapshot before recognition\n");
  verify(voReleaseObject(engine, document));
  document = snapshot;
  printf(" . snapshot swapped successfully\n");
  printf("\n");

  printf("11) running the shape recognition process\n");
  verify(VO_IFACE(engine, IShapeDocumentProcessor)->process(engine, recognizer, document, NULL, NULL));
  printf(" . shape recognition performed successfully\n");
  printf("\n");

  printf("12) retrieving the recognition result\n");
  printResult(document);
  printf("\n");

  printf("13) cleaning-up: releasing all created objects and destroying the Engine\n");
  verify(voReleaseObject(engine, document));
  verify(voReleaseObject(engine, recognizer));
  verify(voDestroyEngine(engine));
  printf(" . clean-up done successfully\n");
  printf("\n");

  printf("end of example:\n");
  printf(" you just learnt how to adjust models detection sensitivity\n");

  // -- finalization -----------------------------------------------------------
  unloadMyScriptEngine();

  return EXIT_SUCCESS;
}

static void printResult(voShapeDocument document)
{
  voShapeSegment segment = NULL;
  uint32_t count = 0;
  uint32_t i = 0;

  segment = VO_IFACE(engine, IShapeDocument)->getSegmentAt(engine, document, 0);
  verify(segment != NULL);
  printf(" . first ShapeSegment retrieved successfully\n");

  printf(" . retrieving the number of ShapeCandidates in the ShapeSegment: ");
  count = VO_IFACE(engine, IShapeSegment)->getCandidateCount(engine, segment);
  verify(count != (uint32_t)-1);
  printf("%d\n", count);

  printf(" . iterating over the ShapeRecognized candidates:\n");
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

          printf("   . %s (rs:%.4f - nrs:%.4f)\n", label.bytes, rs, nrs);

          free(label.bytes);
          // don't forget to release objects returned by the API once you're done
          verify(voReleaseObject(engine, model));
        }
        break;

      case -1:
        printf("unable to get candidate type\n");
        break;

      default:
        // ignore other types of candidates
        break;
    }

    verify(voReleaseObject(engine, candidate));
  }

  verify(voReleaseObject(engine, segment));
}
