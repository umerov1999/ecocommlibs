/**
 * The `AdjustingModelDetectionSensitivity` example code shows you how to adjust
 * beautifier properties.
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
#include <math.h>

// global engine (for the sake of readability)
voEngine engine = NULL;

// common utility functions
#include "../common/common.h"

// digital ink
#include "rectangle.strokes.h"
#include "arrow.strokes.h"
#include "circle.strokes.h"
#include "circles.strokes.h"

// forward declaration of example functions
static void unfreezeBeautification(voShapeDocument document);
static void printResult(voShapeDocument document, voId prop);

// main function
int main()
{
  voShapeRecognizer recognizer = NULL;
  voShapeBeautifier beautifier = NULL;
  voShapeKnowledge shapeKnowledge = NULL;
  voShapeDocument document = NULL;
  float strength = 0.0f;

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
  printf(" . we are going to adjust the alignment with the x and y axis\n");
  printf(" . adding a rectangle (which is in fact not aligned with the x and y axis\n");
  printf("   ,--------.\n");
  printf("   |        |\n");
  printf("   |        |\n");
  printf("   '--------'\n");
  verify(VO_IFACE(engine, IShapeDocument)->addStroke(engine, document, &rectangle_s0[0], 2 * sizeof(*rectangle_s0), &rectangle_s0[1], 2 * sizeof(*rectangle_s0), (uint32_t)(sizeof(rectangle_s0) / sizeof(*rectangle_s0)) / 2));
  verify(VO_IFACE(engine, IShapeDocument)->addStroke(engine, document, &rectangle_s1[0], 2 * sizeof(*rectangle_s1), &rectangle_s1[1], 2 * sizeof(*rectangle_s1), (uint32_t)(sizeof(rectangle_s1) / sizeof(*rectangle_s1)) / 2));
  verify(VO_IFACE(engine, IShapeDocument)->addStroke(engine, document, &rectangle_s2[0], 2 * sizeof(*rectangle_s2), &rectangle_s2[1], 2 * sizeof(*rectangle_s2), (uint32_t)(sizeof(rectangle_s2) / sizeof(*rectangle_s2)) / 2));
  verify(VO_IFACE(engine, IShapeDocument)->addStroke(engine, document, &rectangle_s3[0], 2 * sizeof(*rectangle_s3), &rectangle_s3[1], 2 * sizeof(*rectangle_s3), (uint32_t)(sizeof(rectangle_s3) / sizeof(*rectangle_s3)) / 2));
  printf(" . ShapeDocument successfully fueled with digital ink\n");
  printf("\n");

  printf("6) running the shape recognition and beautification processes\n");
  verify(VO_IFACE(engine, IShapeDocumentProcessor)->process(engine, recognizer, document, NULL, NULL));
  verify(VO_IFACE(engine, IShapeDocumentProcessor)->process(engine, beautifier, document, NULL, NULL));
  printf(" . shape recognition and beautification performed successfully\n");
  printf("\n");

  printf("7) retrieving the recognition and beautification result\n");
  printResult(document, VO_SHAPE_BEAUTIFIER_ALIGNMENT_STRENGTH);
  printf("\n");

  printf("8) un-freezing the beautification\n");
  unfreezeBeautification(document);
  printf("\n");

  printf("9) adjusting ShapeBeautifier's alignment strength to 0 (disabled)\n");
  strength = 0.0f;
  verify(voSetProperty(engine, beautifier, VO_SHAPE_BEAUTIFIER_ALIGNMENT_STRENGTH, &strength, sizeof(strength)));
  printf(" . alignment strength adjusted successfully\n");
  printf("\n");

  printf("10) running the shape beautification process again\n");
  verify(VO_IFACE(engine, IShapeDocumentProcessor)->process(engine, beautifier, document, NULL, NULL));
  printf(" . shape beautification performed successfully\n");
  printf("\n");

  printf("11) retrieving the recognition and beautification result\n");
  printResult(document, VO_SHAPE_BEAUTIFIER_ALIGNMENT_STRENGTH);
  printf("\n");

  printf("12) clearing the ShapeDocument\n");
  verify(VO_IFACE(engine, IShapeDocument)->clear(engine, document));
  printf(" . ShapeDocument cleared successfully\n");
  printf("\n");

  printf("13) fueling the ShapeDocument with digital ink\n");
  printf(" . we are going to adjust the connection between shapes\n");
  printf(" . adding an arrow\n");
  printf("   ------>\n");
  verify(VO_IFACE(engine, IShapeDocument)->addStroke(engine, document, &arrow_s0[0], 2 * sizeof(*arrow_s0), &arrow_s0[1], 2 * sizeof(*arrow_s0), (uint32_t)(sizeof(arrow_s0) / sizeof(*arrow_s0)) / 2));
  verify(VO_IFACE(engine, IShapeDocument)->addStroke(engine, document, &arrow_s1[0], 2 * sizeof(*arrow_s1), &arrow_s1[1], 2 * sizeof(*arrow_s1), (uint32_t)(sizeof(arrow_s1) / sizeof(*arrow_s1)) / 2));
  
  printf(" . adding a circle\n");
  printf("             ,---.\n");
  printf("            /     \\\n");
  printf("           |       |\n");
  printf("   ------>  \\     /\n");
  printf("             '---'\n");
  verify(VO_IFACE(engine, IShapeDocument)->addStroke(engine, document, &circle_s0[0], 2 * sizeof(*circle_s0), &circle_s0[1], 2 * sizeof(*circle_s0), (uint32_t)(sizeof(circle_s0) / sizeof(*circle_s0)) / 2));
  printf(" . ShapeDocument successfully fueled with digital ink\n");
  printf("\n");

  printf("14) resetting ShapeBeautifier's alignment strength to its default value\n");
  verify(voSetProperty(engine, beautifier, VO_SHAPE_BEAUTIFIER_ALIGNMENT_STRENGTH, NULL, sizeof(strength)));
  printf(" . alignment strength resetted successfully\n");
  printf("\n");

  printf("15) running the shape recognition and beautification processes\n");
  verify(VO_IFACE(engine, IShapeDocumentProcessor)->process(engine, recognizer, document, NULL, NULL));
  verify(VO_IFACE(engine, IShapeDocumentProcessor)->process(engine, beautifier, document, NULL, NULL));
  printf(" . shape recognition and beautification performed successfully\n");
  printf("\n");

  printf("16) retrieving the recognition and beautification result\n");
  printResult(document, VO_SHAPE_BEAUTIFIER_CONNECTION_STRENGTH);
  printf("\n");

  printf("17) un-freezing the beautification\n");
  unfreezeBeautification(document);
  printf("\n");

  printf("18) adjusting ShapeBeautifier's connection strength to 0 (disabled)\n");
  strength = 0.0f;
  verify(voSetProperty(engine, beautifier, VO_SHAPE_BEAUTIFIER_CONNECTION_STRENGTH, &strength, sizeof(strength)));
  printf(" . connection strength adjusted successfully\n");
  printf("\n");

  printf("19) running the shape beautification process again\n");
  verify(VO_IFACE(engine, IShapeDocumentProcessor)->process(engine, beautifier, document, NULL, NULL));
  printf(" . shape beautification performed successfully\n");
  printf("\n");

  printf("20) retrieving the recognition and beautification result\n");
  printResult(document, VO_SHAPE_BEAUTIFIER_CONNECTION_STRENGTH);
  printf("\n");

  printf("21) clearing the ShapeDocument\n");
  verify(VO_IFACE(engine, IShapeDocument)->clear(engine, document));
  printf(" . ShapeDocument cleared successfully\n");
  printf("\n");

  printf("22) fueling the ShapeDocument with digital ink\n");
  printf(" . we are going to adjust the congruence between shapes\n");
  printf(" . adding a circle\n");
  printf("     ,---.\n");
  printf("    /     \\\n");
  printf("   |       |\n");
  printf("    \\     /\n");
  printf("     '---'\n");
  verify(VO_IFACE(engine, IShapeDocument)->addStroke(engine, document, &circles_s0[0], 2 * sizeof(*circles_s0), &circles_s0[1], 2 * sizeof(*circles_s0), (uint32_t)(sizeof(circles_s0) / sizeof(*circles_s0)) / 2));

  printf(" . adding another circle\n");
  printf("               ,---.\n");
  printf("              /     \\\n");
  printf("    ,---.    |       |\n");
  printf("   /     \\    \\     /\n");
  printf("  |       |    '---'\n");
  printf("   \\     /\n");
  printf("    '---'\n");
  verify(VO_IFACE(engine, IShapeDocument)->addStroke(engine, document, &circles_s1[0], 2 * sizeof(*circles_s1), &circles_s1[1], 2 * sizeof(*circles_s1), (uint32_t)(sizeof(circles_s1) / sizeof(*circles_s1)) / 2));
  printf(" . ShapeDocument successfully fueled with digital ink\n");
  printf("\n");

  printf("23) resetting ShapeBeautifier's connection strength to its default value\n");
  verify(voSetProperty(engine, beautifier, VO_SHAPE_BEAUTIFIER_CONNECTION_STRENGTH, NULL, sizeof(strength)));
  printf(" . connection strength resetted successfully\n");
  printf("\n");

  printf("24) running the shape recognition and beautification processes\n");
  verify(VO_IFACE(engine, IShapeDocumentProcessor)->process(engine, recognizer, document, NULL, NULL));
  verify(VO_IFACE(engine, IShapeDocumentProcessor)->process(engine, beautifier, document, NULL, NULL));
  printf(" . shape recognition and beautification performed successfully\n");
  printf("\n");

  printf("25) retrieving the recognition and beautification result\n");
  printResult(document, VO_SHAPE_BEAUTIFIER_CONGRUENCE_STRENGTH);
  printf("\n");

  printf("26) un-freezing the beautification\n");
  unfreezeBeautification(document);
  printf("\n");

  printf("27) adjusting ShapeBeautifier's congruence strength to 1 (strongest)\n");
  strength = 1.0f;
  verify(voSetProperty(engine, beautifier, VO_SHAPE_BEAUTIFIER_CONGRUENCE_STRENGTH, &strength, sizeof(strength)));
  printf(" . congruence strength adjusted successfully\n");
  printf("\n");

  printf("28) running the shape beautification process again\n");
  verify(VO_IFACE(engine, IShapeDocumentProcessor)->process(engine, beautifier, document, NULL, NULL));
  printf(" . shape beautification performed successfully\n");
  printf("\n");

  printf("29) retrieving the recognition and beautification result\n");
  printResult(document, VO_SHAPE_BEAUTIFIER_CONGRUENCE_STRENGTH);
  printf("\n");

  printf("30) cleaning-up: releasing all created objects and destroying the Engine\n");
  verify(voReleaseObject(engine, document));
  verify(voReleaseObject(engine, beautifier));
  verify(voReleaseObject(engine, recognizer));
  verify(voDestroyEngine(engine));
  printf(" . clean-up done successfully\n");
  printf("\n");

  printf("end of example:\n");
  printf(" you just learnt how to adjust ShapeBeautifier properties\n");

  // -- finalization -----------------------------------------------------------
  unloadMyScriptEngine();

  return EXIT_SUCCESS;
}

static void unfreezeBeautification(voShapeDocument document)
{
  uint32_t count = 0;
  uint32_t i = 0;

  count = VO_IFACE(engine, IShapeDocument)->getSegmentCount(engine, document);
  verify(count != (uint32_t)-1);

  for (i = 0; i < count; ++i)
  {
    voShapeModel segment = NULL;
    bool value = false;

    segment = VO_IFACE(engine, IShapeDocument)->getSegmentAt(engine, document, i);

    verify(voSetProperty(engine, segment, VO_SHAPE_SEGMENT_FREEZE_BEAUTIFICATION, &value, sizeof(value)));

    verify(voReleaseObject(engine, segment));
  }
  printf(" . beautification un-frozen successfully\n");
}

static float distance(voShapePointData p1, voShapePointData p2)
{
  return sqrtf((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
}

static void printResult(voShapeDocument document, voId prop)
{
  uint32_t segmentCount = 0;
  uint32_t i = 0;
  voShapePointData p1 = {0};
  voShapePointData p2 = {0};
  float radius = 0.0f;

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
            uint32_t primitiveCount = 0;
            uint32_t j = 0;

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

            primitiveCount = VO_IFACE(engine, IShapeRecognized)->getPrimitiveCount(engine, candidate);
            verify(primitiveCount != (uint32_t)-1);

            for (j = 0; j < primitiveCount; ++j)
            {
              voShapePrimitive primitive = NULL;

              primitive = VO_IFACE(engine, IShapeRecognized)->getPrimitiveAt(engine, candidate, j);
              verify(primitive != NULL);

              switch(voGetType(engine, primitive))
              {
                case VO_ShapeLine:
                  {
                    voShapeLineData data;

                    verify(VO_IFACE(engine, IShapeLine)->getData(engine, primitive, &data));
                    printf( "     . line: (%.2f, %.2f), (%.2f, %.2f)\n",
                            data.p1.x, data.p1.y,
                            data.p2.x, data.p2.y);

                    if (prop == VO_SHAPE_BEAUTIFIER_ALIGNMENT_STRENGTH && j == 0)
                    {
                      p1 = data.p1;
                      p2 = data.p2;
                    }
                  }
                  break;

                case VO_ShapeEllipticArc:
                  {
                    voShapeEllipticArcData data;

                    verify(VO_IFACE(engine, IShapeEllipticArc)->getData(engine, primitive, &data));
                    printf( "     . arc: (%.2f, %.2f), %.2f, %.2f, %.2f, %.2f, %.2f\n",
                            data.center.x, data.center.y,
                            data.minRadius,
                            data.maxRadius,
                            data.orientation,
                            data.startAngle,
                            data.sweepAngle);

                    if (prop == VO_SHAPE_BEAUTIFIER_CONNECTION_STRENGTH && strcmp("circle", label.bytes) == 0)
                    {
                      p2 = data.center;
                      radius = data.minRadius;
                    }
                    else if (prop == VO_SHAPE_BEAUTIFIER_CONGRUENCE_STRENGTH && strcmp("circle", label.bytes) == 0)
                    {
                      if (i == 0)
                        p1 = data.center;
                      else
                        p2 = data.center;
                    }

                  }
                  break;

                case VO_ShapeDecoratedLine:
                  {
                    voShapeDecoratedLineData data;

                    verify(VO_IFACE(engine, IShapeDecoratedLine)->getData(engine, primitive, &data));
                    printf( "     . decorated line: (%.2f, %.2f)%s, (%.2f, %.2f)%s, \n",
                            data.line.p1.x, data.line.p1.y,
                            data.p1Decoration == VO_SHAPE_DECORATION_ARROW_HEAD ? " head" : "",
                            data.line.p2.x, data.line.p2.y,
                            data.p2Decoration == VO_SHAPE_DECORATION_ARROW_HEAD ? " head" : "");

                    if (prop == VO_SHAPE_BEAUTIFIER_CONNECTION_STRENGTH && strcmp("arrow", label.bytes) == 0)
                    {
                      p1 = data.line.p2;
                    }
                  }
                  break;

                default:
                  break;
              }

              verify(voReleaseObject(engine, primitive));
            }

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

  if (prop == VO_SHAPE_BEAUTIFIER_ALIGNMENT_STRENGTH)
  {
    printf(" . rectangle aligned with the axis: %s\n", (fabsf(p1.x - p2.x) <= 0.01f || fabsf(p1.y - p2.y) <= 0.01f) ? "true" : "false");
  }
  else if (prop == VO_SHAPE_BEAUTIFIER_CONNECTION_STRENGTH)
  {
    printf(" . distance between arrow head and circle: %.2f\n", fabsf(distance(p1, p2) - radius));
  }
  else if (prop == VO_SHAPE_BEAUTIFIER_CONGRUENCE_STRENGTH)
  {
    printf(" . are the two circles aligned vertically: %s\n", fabsf(p1.y - p2.y) <= 0.01f ? "true" : "false");
  }
}
