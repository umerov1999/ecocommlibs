/**
 * The `EditInk` example code shows you how to inspect and edit an `Ink` object.
 */


// -- configuration ------------------------------------------------------------

// common configuration parameters
#include "../common/config.h"

// include the MyScript Engine API header(s)
#include <c/MyScriptEngine.h>
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

// path to the .jink file
#define JINK_FILE DATA_DIR "/flower.jink"


// main function
int main(void)
{
  voInk ink;
  int strokeCount;
  voInkStroke stroke;
  const voPoint* pointArray;
  int pointCount;
  voRectangle rectangle;
  voInkSelection selection;
  int selectionStrokeCount;
  voTransform transform;
  voInkStroke selectionStroke;
  voPoint point;
  float tx;
  float ty;
  voInkStroke transformedStroke;
  voPoint transformedPoint;
  const float floatTolerance = 0.001f;
  int strokeCountAfterTransform;
  int strokeCountAfterDeletion;
  voInkStroke newStroke;
  voPoint* allocatedPoints;

  // -- initialization ---------------------------------------------------------
  if (!loadMyScriptEngine(MYSCRIPT_ENGINE_LIBRARY_DIR, NULL))
    return (fprintf(stderr, "failed to load the MyScriptEngine library\n"), EXIT_FAILURE);

  // -- example code -----------------------------------------------------------
  printf("1) creating a new Engine initialized with your certificate\n");
  engine = voCreateEngine(VO_MSE_VER, &myCertificate, NULL);
  verify(engine != NULL);
  printf(" . Engine created successfully\n");
  printf("\n");

  printf("2) loading an existing .jink file\n");
  ink = loadEngineObject(JINK_FILE);
  verify(ink != NULL);
  printf(" . ");
  printPath(stdout, JINK_FILE);
  printf(" loaded successfully\n");
  printf("\n");

  printf("3) inspecting the content of the loaded Ink\n");
  strokeCount = VO_IFACE(engine, IInkStrokeSet)->getStrokeCount(engine, ink);
  printf(" . the ink contains %d strokes\n", strokeCount);
  printf("\n");

  printf("4) inspecting the first stroke\n");
  stroke = VO_IFACE(engine, IInkStrokeSet)->getStrokeAt(engine, ink, 0);
  verify(stroke != NULL);
  pointCount = VO_IFACE(engine, IInkStroke)->getSampleCount(engine, stroke);
  printf(" . the first stroke contains %d points\n", pointCount);
  verify(VO_YES == VO_IFACE(engine, IInkStroke)->isPointArrayCompatible(engine, stroke));
  printf(" . the first stroke can provide a pointer to its data\n");
  pointArray = VO_IFACE(engine, IInkStroke)->getPointArray(engine, stroke);
  verify(pointArray != NULL);
  printf(" . coordinates of first point in the stroke: { %g ; %g }\n", pointArray[0].x, pointArray[0].y);
  printf("\n");

  printf("5) selecting a portion of Ink that partially contains the first stroke\n");
  selection = voCreateInkSelection(engine, ink);
  verify(selection != NULL);
  // select a portion of ink
  rectangle.x = 230;
  rectangle.y = 300;
  rectangle.width = 50;
  rectangle.height = 50;
  verify(VO_IFACE(engine, ISelection)->selectRectangle(engine, selection, &rectangle, VO_SELECT_SET));
  // check that we selected something
  selectionStrokeCount = VO_IFACE(engine, IInkStrokeSet)->getStrokeCount(engine, selection);
  printf(" . selection successfully created, containing %d strokes or substrokes\n", selectionStrokeCount);
  selectionStroke = VO_IFACE(engine, IInkStrokeSet)->getStrokeAt(engine, selection, 0);
  verify(selectionStroke != NULL);
  verify(VO_IFACE(engine, IInkStroke)->getPoints(engine, selectionStroke, 0, 1, &point));
  printf(" . coordinates of first point in the selection: { %g ; %g }\n", point.x, point.y);
  printf("\n");

  printf("6) transforming the selection\n");
  verify(VO_IFACE(engine, IGeometry)->setIdentity(engine, &transform));
  tx = 10.0f;
  ty = 100.0f;
  verify(VO_IFACE(engine, IGeometry)->translate(engine, &transform, tx, ty));
  verify(VO_IFACE(engine, IInk)->transform(engine, ink, selection, &transform));
  strokeCountAfterTransform = VO_IFACE(engine, IInkStrokeSet)->getStrokeCount(engine, ink);
  transformedStroke = VO_IFACE(engine, IInkStrokeSet)->getStrokeAt(engine, selection, 0);
  verify(transformedStroke != NULL);
  printf(" . selection successfully transformed; now the Ink has %d strokes\n", strokeCountAfterTransform);
  verify(VO_IFACE(engine, IInkStroke)->getPoints(engine, transformedStroke, 0, 1, &transformedPoint));
  verify((1.0f - floatTolerance) * transformedPoint.x <= tx + point.x && (1.0f + floatTolerance) * transformedPoint.x >= tx + point.x);
  printf(" . coordinates of first point in the selection after transform: { %g ; %g }\n", transformedPoint.x, transformedPoint.y);
  printf("\n");

  printf("7) erasing selection\n");
  verify(VO_IFACE(engine, IInk)->erase(engine, ink, selection));
  strokeCountAfterDeletion = VO_IFACE(engine, IInkStrokeSet)->getStrokeCount(engine, ink);
  verify(strokeCountAfterTransform - selectionStrokeCount == strokeCountAfterDeletion);
  printf(" . selection successfully erased; now the Ink has %d strokes\n", strokeCountAfterDeletion);
  printf("\n");

  printf("8) inspecting the new first stroke\n");
  newStroke = VO_IFACE(engine, IInkStrokeSet)->getStrokeAt(engine, ink, 0);
  verify(newStroke != NULL);
  verify(VO_NO == VO_IFACE(engine, IInkStroke)->isPointArrayCompatible(engine, newStroke));
  printf(" . the first stroke cannot directly provide a pointer to its data\n");
  // when isPointArrayCompatible returns false, we have to allocate a
  // recipient buffer for the points
  pointCount = VO_IFACE(engine, IInkStroke)->getSampleCount(engine, newStroke);
  allocatedPoints = malloc(sizeof(voPoint) * pointCount);
  verify(allocatedPoints != NULL);
  verify(VO_IFACE(engine, IInkStroke)->getPoints(engine, newStroke, 0, pointCount, allocatedPoints));
  printf(" . coordinates of first point in the stroke: { %g ; %g }\n", allocatedPoints[0].x, allocatedPoints[0].y);
  printf("\n");

  printf("9) cleaning-up: releasing all created objects and destroying the Engine\n");
  free(allocatedPoints);
  verify(voReleaseObject(engine, transformedStroke));
  verify(voReleaseObject(engine, selectionStroke));
  verify(voReleaseObject(engine, selection));
  verify(voReleaseObject(engine, stroke));
  verify(voReleaseObject(engine, ink));
  verify(voDestroyEngine(engine));
  printf(" . clean-up done successfully\n");
  printf("\n");

  printf("end of example:\n");
  printf(" you just learnt how to inspect and edit ink\n");

  // -- finalization -----------------------------------------------------------
  unloadMyScriptEngine();

  return EXIT_SUCCESS;
}
