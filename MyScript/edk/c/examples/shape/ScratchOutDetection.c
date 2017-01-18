/**
 * The `ScratchOutDetection` example code shows you how to handle scratch out.
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
#include "scratch_out.strokes.h"

// forward declaration of example functions
static void printResult(voShapeDocument document);

// main function
int main()
{
  voShapeRecognizer recognizer = NULL;
  voShapeKnowledge shapeKnowledge = NULL;
  voShapeDocument document = NULL;

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
  printf(" . adding ink that ALMOST forms a square\n");
  printf("   ,-------------\n");
  printf("   |      |\n");
  printf("   |      |\n");
  printf("   '------'\n");
  verify(VO_IFACE(engine, IShapeDocument)->addStroke(engine, document, &scratch_out_s0[0], 2 * sizeof(*scratch_out_s0), &scratch_out_s0[1], 2 * sizeof(*scratch_out_s0), (uint32_t)(sizeof(scratch_out_s0) / sizeof(*scratch_out_s0)) / 2));
  printf(" . ShapeDocument successfully fueled with digital ink\n");
  printf("\n");

  printf("6) running the shape recognition process\n");
  verify(VO_IFACE(engine, IShapeDocumentProcessor)->process(engine, recognizer, document, NULL, NULL));
  printf(" . shape recognition performed successfully\n");
  printf("\n");

  printf("7) retrieving the recognition result\n");
  printResult(document);
  printf("\n");

  printf("8) fueling the ShapeDocument with more digital ink\n");
  printf(" . scratching out the previous ink\n");
  printf("   ,------/-/-/-/\n");
  printf("   |      |\n");
  printf("   |      |\n");
  printf("   '------'\n");
  verify(VO_IFACE(engine, IShapeDocument)->addStroke(engine, document, &scratch_out_s1[0], 2 * sizeof(*scratch_out_s1), &scratch_out_s1[1], 2 * sizeof(*scratch_out_s1), (uint32_t)(sizeof(scratch_out_s1) / sizeof(*scratch_out_s1)) / 2));
  printf(" . ShapeDocument successfully fueled with digital ink\n");
  printf("\n");

  printf("9) running the shape recognition process\n");
  verify(VO_IFACE(engine, IShapeDocumentProcessor)->process(engine, recognizer, document, NULL, NULL));
  printf(" . shape recognition performed successfully\n");
  printf("\n");

  printf("10) retrieving the recognition result\n");
  printResult(document);
  printf("\n");

  printf("11) cleaning-up: releasing all created objects and destroying the Engine\n");
  verify(voReleaseObject(engine, document));
  verify(voReleaseObject(engine, recognizer));
  verify(voDestroyEngine(engine));
  printf(" . clean-up done successfully\n");
  printf("\n");

  printf("end of example:\n");
  printf(" you just learnt how to handle scratch out\n");

  // -- finalization -----------------------------------------------------------
  unloadMyScriptEngine();

  return EXIT_SUCCESS;
}

static void printResult(voShapeDocument document)
{
  uint32_t segmentCount = 0;
  uint32_t i = 0;

  printf(" . retrieving the number of ShapeSegments in the ShapeDocument: ");
  segmentCount = VO_IFACE(engine, IShapeDocument)->getSegmentCount(engine, document);
  verify(segmentCount != (uint32_t)-1);
  printf("%d\n", segmentCount);

  printf(" . iterating over the ShapeSegments:\n");
  for (i = 0; i < segmentCount; ++i)
  {
    voShapeSegment segment = NULL;
    uint32_t candidateCount = 0;

    segment = VO_IFACE(engine, IShapeDocument)->getSegmentAt(engine, document, i);
    verify(segment != NULL);

    candidateCount = VO_IFACE(engine, IShapeSegment)->getCandidateCount(engine, segment);
    verify(candidateCount != (uint32_t)-1);

    if (candidateCount != 0)
    {
      voShapeCandidate candidate = NULL;

      // get the best candidate
      candidate = VO_IFACE(engine, IShapeSegment)->getCandidateAt(engine, segment, 0);
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

            printf("   . segment %d, best candidate: %s (rs:%.4f - nrs:%.4f)\n", i, label.bytes, rs, nrs);

            free(label.bytes);
            // don't forget to release objects returned by the API once you're done
            verify(voReleaseObject(engine, model));
          }
          break;

        case VO_ShapeScratchOut:
          {
            printf("   . segment %d, scratch out\n", i);
          }
          break;

        case VO_ShapeErased:
          {
            printf("   . segment %d, erased ink\n", i);
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
}
