/**
 * The `CreateInk` example code shows you how to create, fill and serialize ink
 * with an `Ink` object.
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
#define JINK_FILE OUTPUT_DIR "/CreateInk-output.jink"


// main function
int main(void)
{
  voInk ink;
  int strokeCount;
  voInkStrokeBuilder strokeBuilder;
  voString strokeFormatString = voString_INITIALIZER("{\"channels\":[{\"name\":\"X\"},{\"name\":\"Y\"}]}");
  voInkStroke stroke;

  // -- initialization ---------------------------------------------------------
  if (!loadMyScriptEngine(MYSCRIPT_ENGINE_LIBRARY_DIR, NULL))
    return (fprintf(stderr, "failed to load the MyScriptEngine library\n"), EXIT_FAILURE);

  // -- example code -----------------------------------------------------------
  printf("1) creating a new Engine initialized with your certificate\n");
  engine = voCreateEngine(VO_MSE_VER, &myCertificate, NULL);
  verify(engine != NULL);
  printf(" . Engine created successfully\n");
  printf("\n");

  printf("2) creating an empty Ink object\n");
  ink = voCreateInk(engine);
  verify(ink != NULL);
  printf(" . Ink created successfully\n");
  printf("\n");

  printf("3) creating a new stroke builder\n");
  strokeBuilder = voCreateInkStrokeBuilder2(engine, NULL, &strokeFormatString);
  verify(strokeBuilder != NULL);
  printf(" . stroke builder created successfully\n");
  printf("\n");

  printf("4) filling the stroke builder with samples\n");
  verify(VO_IFACE(engine, IInkStrokeBuilder)->clear(engine, strokeBuilder));
  verify(0 == VO_IFACE(engine, IInkStroke)->getSampleCount(engine, strokeBuilder));
  verify(VO_IFACE(engine, IInkStrokeBuilder)->addSample(engine, strokeBuilder, "FF", 41.0f,  60.0f));
  verify(VO_IFACE(engine, IInkStrokeBuilder)->addSample(engine, strokeBuilder, "FF", 40.0f, 100.0f));
  verify(VO_IFACE(engine, IInkStrokeBuilder)->addSample(engine, strokeBuilder, "FF", 38.0f,  64.0f));
  verify(VO_IFACE(engine, IInkStrokeBuilder)->addSample(engine, strokeBuilder, "FF", 47.0f,  60.0f));
  verify(VO_IFACE(engine, IInkStrokeBuilder)->addSample(engine, strokeBuilder, "FF", 55.0f,  70.0f));
  verify(VO_IFACE(engine, IInkStrokeBuilder)->addSample(engine, strokeBuilder, "FF", 52.0f,  80.0f));
  verify(VO_IFACE(engine, IInkStrokeBuilder)->addSample(engine, strokeBuilder, "FF", 42.0f,  79.0f));
  printf(" . stroke builder succesfully filled with %d points\n", VO_IFACE(engine, IInkStroke)->getSampleCount(engine, strokeBuilder));
  printf("\n");

  printf("5) creating a new stroke and adding it to the Ink\n");
  stroke = VO_IFACE(engine, IInkStrokeBuilder)->createStroke(engine, strokeBuilder);
  verify(stroke != NULL);
  printf(" . stroke created successfully\n");
  strokeCount = VO_IFACE(engine, IInkStrokeSet)->getStrokeCount(engine, ink);
  printf(" . the Ink still contains %d stroke\n", strokeCount);
  verify(VO_IFACE(engine, IInk)->addStroke(engine, ink, stroke));
  strokeCount = VO_IFACE(engine, IInkStrokeSet)->getStrokeCount(engine, ink);
  printf(" . stroke successfully added\n");
  printf(" . the Ink now contains %d stroke\n", strokeCount);
  printf("\n");

  printf("6) storing the Ink to disk\n");
  storeEngineObject(ink, JINK_FILE);
  printf(" . Ink stored successfully to: ");
  printPath(stdout, JINK_FILE);
  printf("\n");
  printf("\n");

  printf("7) cleaning-up: releasing all created objects and destroying the Engine\n");
  verify(voReleaseObject(engine, stroke));
  verify(voReleaseObject(engine, strokeBuilder));
  verify(voReleaseObject(engine, ink));
  verify(voDestroyEngine(engine));
  printf(" . clean-up done successfully\n");
  printf("\n");

  printf("end of example:\n");
  printf(" you just learnt how to create, fill and serialize ink\n");

  // -- finalization -----------------------------------------------------------
  unloadMyScriptEngine();

  return EXIT_SUCCESS;
}
