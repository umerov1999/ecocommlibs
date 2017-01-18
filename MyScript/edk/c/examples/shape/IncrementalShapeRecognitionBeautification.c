/**
 * The `IncrementalShapeRecognitionBeautification` example code shows you how to
 * perform incremental shape recognition and beautification.
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
#include "rectangle.strokes.h"
#include "circle.strokes.h"
#include "line.strokes.h"
#include "arrow_head.strokes.h"

// forward declaration of example functions
static void printResult(voShapeDocument document);

// main function
int main()
{
  voShapeRecognizer recognizer = NULL;
  voShapeBeautifier beautifier = NULL;
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

  printf("2) creating a new ShapeRecognizer and a new ShapeBeautifier\n");
  recognizer = voCreateShapeRecognizer(engine);
  verify(recognizer != NULL);
  printf(" . ShapeRecognizer created successfully\n");
  beautifier = voCreateShapeBeautifier(engine);
  verify(beautifier != NULL);
  printf(" . ShapeBeautifier created successfully\n");
  printf("\n");

  printf("3) loading and attaching the ShapeKnowledge\n");
  shapeKnowledge = loadEngineObject(SHAPE_KNOWLEDGE_RES);
  printf(" . ShapeKnowledge resource loaded successfully\n");
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, recognizer, shapeKnowledge));
  printf(" . ShapeKnowledge resource successfully attached to the ShapeRecognizer\n");
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, beautifier, shapeKnowledge));
  printf(" . ShapeKnowledge resource successfully attached to the ShapeBeautifier\n");
  printf(" . releasing the ShapeKnowledge resource (not needed anymore)\n");
  verify(voReleaseObject(engine, shapeKnowledge));
  printf(" . ShapeKnowledge resource released successfully\n");
  printf("\n");

  printf("4) creating a ShapeDocument\n");
  document = voCreateShapeDocument(engine);
  verify(document != NULL);
  printf(" . ShapeDocument created successfully\n");
  printf("\n");

  printf("5) fueling the ShapeDocument with digital ink\n");
  printf(" . adding a rectangle\n");
  printf("   ,--------.\n");
  printf("   |        |\n");
  printf("   |        |\n");
  printf("   '--------'\n");
  verify(VO_IFACE(engine, IShapeDocument)->addStroke(engine, document, &rectangle_s0[0], 2 * sizeof(*rectangle_s0), &rectangle_s0[1], 2 * sizeof(*rectangle_s0), (uint32_t)(sizeof(rectangle_s0) / sizeof(*rectangle_s0)) / 2));
  verify(VO_IFACE(engine, IShapeDocument)->addStroke(engine, document, &rectangle_s1[0], 2 * sizeof(*rectangle_s1), &rectangle_s1[1], 2 * sizeof(*rectangle_s1), (uint32_t)(sizeof(rectangle_s1) / sizeof(*rectangle_s1)) / 2));
  verify(VO_IFACE(engine, IShapeDocument)->addStroke(engine, document, &rectangle_s2[0], 2 * sizeof(*rectangle_s2), &rectangle_s2[1], 2 * sizeof(*rectangle_s2), (uint32_t)(sizeof(rectangle_s2) / sizeof(*rectangle_s2)) / 2));
  verify(VO_IFACE(engine, IShapeDocument)->addStroke(engine, document, &rectangle_s3[0], 2 * sizeof(*rectangle_s3), &rectangle_s3[1], 2 * sizeof(*rectangle_s3), (uint32_t)(sizeof(rectangle_s3) / sizeof(*rectangle_s3)) / 2));

  printf(" . adding an ALMOST horizontal line\n");
  printf("   ,--------.    ,----\n");
  printf("   |        | ---\n");
  printf("   |        |\n");
  printf("   '--------'\n");
  verify(VO_IFACE(engine, IShapeDocument)->addStroke(engine, document, &line_s0[0], 2 * sizeof(*line_s0), &line_s0[1], 2 * sizeof(*line_s0), (uint32_t)(sizeof(line_s0) / sizeof(*line_s0)) / 2));

  printf(" . adding a circle\n");
  printf("                         ,---.\n");
  printf("   ,--------.    ,----  /     \\\n");
  printf("   |        | ---      |       |\n");
  printf("   |        |           \\     /\n");
  printf("   '--------'            '---'\n");
  verify(VO_IFACE(engine, IShapeDocument)->addStroke(engine, document, &circle_s0[0], 2 * sizeof(*circle_s0), &circle_s0[1], 2 * sizeof(*circle_s0), (uint32_t)(sizeof(circle_s0) / sizeof(*circle_s0)) / 2));
  printf(" . ShapeDocument successfully fueled with digital ink\n");
  printf("\n");

  printf("6) running the shape recognition and beautification processes\n");
  verify(VO_IFACE(engine, IShapeDocumentProcessor)->process(engine, recognizer, document, NULL, NULL));
  printf(" . shape recognition performed successfully\n");
  verify(VO_IFACE(engine, IShapeDocumentProcessor)->process(engine, beautifier, document, NULL, NULL));
  printf(" . shape beautification performed successfully\n");
  printf("\n");

  printf("7) retrieving the recognition and beautification result\n");
  printResult(document);
  printf("\n");

  printf("8) fueling the ShapeDocument with more digital ink\n");
  printf(" . adding an arrow head\n");
  printf(" . please note that in the typical incremental scenario, the user\n");
  printf("   draws the arrow head on top of a render of the previously recognized\n");
  printf("   and beautified diagram, the ink is not displayed anymore\n");
  printf(" . as you'll see next, the ink for the arrow head is in fact drawn\n");
  printf("   below the right extremity of that almost horizontal line\n");
  printf("                         ,---.\n");
  printf("   ,--------.           /     \\\n");
  printf("   |        | -------->|       |\n");
  printf("   |        |           \\     /\n");
  printf("   '--------'            '---'\n");
  verify(VO_IFACE(engine, IShapeDocument)->addStroke(engine, document, &arrow_head_s0[0], 2 * sizeof(*arrow_head_s0), &arrow_head_s0[1], 2 * sizeof(*arrow_head_s0), (uint32_t)(sizeof(arrow_head_s0) / sizeof(*arrow_head_s0)) / 2));
  printf(" . ShapeDocument successfully fueled with digital ink\n");
  printf("\n");

  printf("9) running the shape recognition and beautification processes\n");
  verify(VO_IFACE(engine, IShapeDocumentProcessor)->process(engine, recognizer, document, NULL, NULL));
  printf(" . shape recognition performed successfully\n");
  verify(VO_IFACE(engine, IShapeDocumentProcessor)->process(engine, beautifier, document, NULL, NULL));
  printf(" . shape beautification performed successfully\n");
  printf("\n");

  printf("10) retrieving the recognition and beautification result\n");
  printResult(document);
  printf("\n");

  printf("11) clearing the ShapeDocument\n");
  verify(VO_IFACE(engine, IShapeDocument)->clear(engine, document));
  printf(" . ShapeDocument cleared successfully\n");
  printf("\n");

  printf("12) fueling the ShapeDocument with digital ink AS A WHOLE\n");
  printf(" . adding a rectangle\n");
  printf("   ,--------.\n");
  printf("   |        |\n");
  printf("   |        |\n");
  printf("   '--------'\n");
  verify(VO_IFACE(engine, IShapeDocument)->addStroke(engine, document, &rectangle_s0[0], 2 * sizeof(*rectangle_s0), &rectangle_s0[1], 2 * sizeof(*rectangle_s0), (uint32_t)(sizeof(rectangle_s0) / sizeof(*rectangle_s0)) / 2));
  verify(VO_IFACE(engine, IShapeDocument)->addStroke(engine, document, &rectangle_s1[0], 2 * sizeof(*rectangle_s1), &rectangle_s1[1], 2 * sizeof(*rectangle_s1), (uint32_t)(sizeof(rectangle_s1) / sizeof(*rectangle_s1)) / 2));
  verify(VO_IFACE(engine, IShapeDocument)->addStroke(engine, document, &rectangle_s2[0], 2 * sizeof(*rectangle_s2), &rectangle_s2[1], 2 * sizeof(*rectangle_s2), (uint32_t)(sizeof(rectangle_s2) / sizeof(*rectangle_s2)) / 2));
  verify(VO_IFACE(engine, IShapeDocument)->addStroke(engine, document, &rectangle_s3[0], 2 * sizeof(*rectangle_s3), &rectangle_s3[1], 2 * sizeof(*rectangle_s3), (uint32_t)(sizeof(rectangle_s3) / sizeof(*rectangle_s3)) / 2));

  printf(" . adding an ALMOST horizontal line\n");
  printf("   ,--------.    ,---- \n");
  printf("   |        | ---      \n");
  printf("   |        |\n");
  printf("   '--------'\n");
  verify(VO_IFACE(engine, IShapeDocument)->addStroke(engine, document, &line_s0[0], 2 * sizeof(*line_s0), &line_s0[1], 2 * sizeof(*line_s0), (uint32_t)(sizeof(line_s0) / sizeof(*line_s0)) / 2));

  printf(" . adding a circle\n");
  printf("                         ,---.\n");
  printf("   ,--------.    ,----  /     \\\n");
  printf("   |        | ---      |       |\n");
  printf("   |        |           \\     /\n");
  printf("   '--------'            '---'\n");
  verify(VO_IFACE(engine, IShapeDocument)->addStroke(engine, document, &circle_s0[0], 2 * sizeof(*circle_s0), &circle_s0[1], 2 * sizeof(*circle_s0), (uint32_t)(sizeof(circle_s0) / sizeof(*circle_s0)) / 2));

  printf(" . adding an arrow head (as if the line was horizontal)\n");
  printf("                         ,---.\n");
  printf("   ,--------.    ,----  /     \\\n");
  printf("   |        | ---     >|       |\n");
  printf("   |        |           \\     /\n");
  printf("   '--------'            '---'\n");
  verify(VO_IFACE(engine, IShapeDocument)->addStroke(engine, document, &arrow_head_s0[0], 2 * sizeof(*arrow_head_s0), &arrow_head_s0[1], 2 * sizeof(*arrow_head_s0), (uint32_t)(sizeof(arrow_head_s0) / sizeof(*arrow_head_s0)) / 2));
  printf(" . ShapeDocument successfully fueled with digital ink\n");
  printf("\n");

  printf("13) running the shape recognition and beautification processes\n");
  verify(VO_IFACE(engine, IShapeDocumentProcessor)->process(engine, recognizer, document, NULL, NULL));
  printf(" . shape recognition performed successfully\n");
  verify(VO_IFACE(engine, IShapeDocumentProcessor)->process(engine, beautifier, document, NULL, NULL));
  printf(" . shape beautification performed successfully\n");
  printf("\n");

  printf("14) retrieving the recognition result\n");
  printResult(document);
  printf("\n");

  printf("15) cleaning-up: releasing all created objects and destroying the Engine\n");
  verify(voReleaseObject(engine, document));
  verify(voReleaseObject(engine, recognizer));
  verify(voDestroyEngine(engine));
  printf(" . clean-up done successfully\n");
  printf("\n");

  printf("end of example:\n");
  printf(" you just learnt how to perform incremental recognition and beautification\n");

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
