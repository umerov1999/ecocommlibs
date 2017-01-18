/**
 * The `DigitalInkTagging` example code shows you how to perform digital ink
 * tagging.
 */


// -- configuration ------------------------------------------------------------

// common configuration parameters
#include "../common/config.h"

// include the MyScript Engine API header(s)
#include <c/MyScriptEngine.h>
#include <c/MyScriptText.h>

// include the certificate
#include "../certificates/MyCertificate.h"


// -- example body -------------------------------------------------------------

#include <stdio.h>

// global engine (for the sake of readability)
voEngine engine = NULL;

// common utility functions
#include "../common/common.h"

// digital ink
#include "DigitalInkTagging.strokes.h"

// include the MyScriptText tag type utility
#include <../src/MyScriptText-strTagType.h>

// main function
int main()
{
  voDigitalInkTagger tagger = NULL;
  voResource akIso = NULL;
  voInput input = NULL;
  voTagList tags = NULL;
  voTagIterator iterator = NULL;
  int i = 0;

  // -- check required settings have been edited -------------------------------
  if (strlen(LANG) == 0)
  {
    fprintf(stderr, "/!\\ please edit the common/config.h file and set:\n");
    fprintf(stderr, " . the LANG variable\n");
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

  printf("2) creating a new DigitalInkTagger\n");
  tagger = voCreateDigitalInkTagger(engine);
  verify(tagger != NULL);
  printf(" . DigitalInkTagger created successfully\n");
  printf("\n");

  printf("3) loading and attaching resources to the DigitalInkTagger\n");
  printf(" . loading the IsolatedAlphabetKnowledge resource\n");
  akIso = loadEngineObject(AK_ISO_RES);
  printf(" . IsolatedAlphabetKnowledge resource loaded successfully\n");

  printf(" . attaching the IsolatedAlphabetKnowledge resource to the DigitalInkTagger\n");
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, tagger, akIso));
  printf(" . IsolatedAlphabetKnowledge resource attached successfully\n");
  printf("\n");

  printf("4) creating a FloatStructuredInput\n");
  input = voCreateFloatStructuredInput(engine);
  verify(input != NULL);
  printf(" . FloatStructuredInput created successfully\n");
  printf("\n");

  printf("5) fueling the input with strokes written in a boxed field\n");
  verify(VO_IFACE(engine, IFloatStructuredInput)->startInputUnit(engine, input, VO_CHAR));
  verify(VO_IFACE(engine, IFloatStructuredInput)->setGuideBox(engine, input, b0[0], b0[1], b0[2], b0[3]));
  verify(VO_IFACE(engine, IFloatStructuredInput)->addStroke2(engine, input, &b0s0[1], 2 * sizeof(*b0s0), &b0s0[2], 2 * sizeof(*b0s0), (int)(sizeof(b0s0)/sizeof(*b0s0) - 1) / 2, (int)b0s0[0]));
  verify(VO_IFACE(engine, IFloatStructuredInput)->startInputUnit(engine, input, VO_CHAR));
  verify(VO_IFACE(engine, IFloatStructuredInput)->setGuideBox(engine, input, b1[0], b1[1], b1[2], b1[3]));
  verify(VO_IFACE(engine, IFloatStructuredInput)->startInputUnit(engine, input, VO_CHAR));
  verify(VO_IFACE(engine, IFloatStructuredInput)->setGuideBox(engine, input, b2[0], b2[1], b2[2], b2[3]));
  verify(VO_IFACE(engine, IFloatStructuredInput)->addStroke2(engine, input, &b2s0[1], 2 * sizeof(*b2s0), &b2s0[2], 2 * sizeof(*b2s0), (int)(sizeof(b2s0)/sizeof(*b2s0) - 1) / 2, (int)b2s0[0]));
  verify(VO_IFACE(engine, IFloatStructuredInput)->startInputUnit(engine, input, VO_CHAR));
  verify(VO_IFACE(engine, IFloatStructuredInput)->setGuideBox(engine, input, b3[0], b3[1], b3[2], b3[3]));
  verify(VO_IFACE(engine, IFloatStructuredInput)->addStroke2(engine, input, &b3s0[1], 2 * sizeof(*b3s0), &b3s0[2], 2 * sizeof(*b3s0), (int)(sizeof(b3s0)/sizeof(*b3s0) - 1) / 2, (int)b3s0[0]));
  verify(VO_IFACE(engine, IFloatStructuredInput)->addStroke2(engine, input, &b3s1[1], 2 * sizeof(*b3s1), &b3s1[2], 2 * sizeof(*b3s1), (int)(sizeof(b3s1)/sizeof(*b3s1) - 1) / 2, (int)b3s1[0]));
  verify(VO_IFACE(engine, IFloatStructuredInput)->addStroke2(engine, input, &b3s2[1], 2 * sizeof(*b3s2), &b3s2[2], 2 * sizeof(*b3s2), (int)(sizeof(b3s2)/sizeof(*b3s2) - 1) / 2, (int)b3s2[0]));
  verify(VO_IFACE(engine, IFloatStructuredInput)->startInputUnit(engine, input, VO_CHAR));
  verify(VO_IFACE(engine, IFloatStructuredInput)->setGuideBox(engine, input, b4[0], b4[1], b4[2], b4[3]));
  verify(VO_IFACE(engine, IFloatStructuredInput)->addStroke2(engine, input, &b4s0[1], 2 * sizeof(*b4s0), &b4s0[2], 2 * sizeof(*b4s0), (int)(sizeof(b4s0)/sizeof(*b4s0) - 1) / 2, (int)b4s0[0]));
  verify(VO_IFACE(engine, IFloatStructuredInput)->startInputUnit(engine, input, VO_CHAR));
  verify(VO_IFACE(engine, IFloatStructuredInput)->setGuideBox(engine, input, b5[0], b5[1], b5[2], b5[3]));
  verify(VO_IFACE(engine, IFloatStructuredInput)->addStroke2(engine, input, &b5s0[1], 2 * sizeof(*b5s0), &b5s0[2], 2 * sizeof(*b5s0), (int)(sizeof(b5s0)/sizeof(*b5s0) - 1) / 2, (int)b5s0[0]));
  verify(VO_IFACE(engine, IFloatStructuredInput)->addStroke2(engine, input, &b5s1[1], 2 * sizeof(*b5s1), &b5s1[2], 2 * sizeof(*b5s1), (int)(sizeof(b5s1)/sizeof(*b5s1) - 1) / 2, (int)b5s1[0]));
  printf(" . input fueled successfully\n");
  printf("\n");

  printf("6) setting the input as the source of the tagger\n");
  verify(VO_IFACE(engine, IInputConsumer)->setSource(engine, tagger, input));
  printf(" . input successfully set as the tagger's source\n");
  printf("\n");

  printf("7) running the digital ink tagging process\n");
  verify(VO_IFACE(engine, IRunnable)->run(engine, tagger, NULL, NULL));
  printf(" . digital ink tagging performed successfully\n");
  printf("\n");

  printf("8) retrieving the TagList\n");
  tags = VO_IFACE(engine, IDigitalInkTagger)->getTags(engine, tagger);
  verify(tags != NULL);
  printf(" . TagList retrieved successfully\n");
  printf("\n");

  printf("9) iterating over the tags in the TagList\n");
  iterator = VO_IFACE(engine, ITagList)->getIterator(engine, tags);
  verify(iterator != NULL);

  for (i = 1; !VO_IFACE(engine, ITagIterator)->isAtEnd(engine, iterator); ++i)
  {
    voInputRange inputRange = {NULL, 0};
    verify(VO_IFACE(engine, ITagIterator)->getInputRange(engine, iterator, &inputRange));
    inputRange.elements = malloc(sizeof(*inputRange.elements)*inputRange.count);
    verify(VO_IFACE(engine, ITagIterator)->getInputRange(engine, iterator, &inputRange));
    printf(" . %s, ",
      strMyScriptTagType(VO_IFACE(engine, ITagIterator)->getType(engine, iterator)));
    printInputRange(inputRange);
    printf("\n");
    free(inputRange.elements);
    verify(VO_IFACE(engine, ITagIterator)->next(engine, iterator));
  }
  printf("\n");

  printf("10) cleaning-up: releasing all created objects and destroying the Engine\n");
  verify(voReleaseObject(engine, iterator));
  verify(voReleaseObject(engine, tags));
  verify(voReleaseObject(engine, akIso));
  verify(voReleaseObject(engine, tagger));
  verify(voDestroyEngine(engine));
  printf(" . clean-up done successfully\n");
  printf("\n");

  printf("end of example:\n");
  printf(" you just learnt how to perform digital ink tagging\n");

  // -- finalization -----------------------------------------------------------
  unloadMyScriptEngine();

  return EXIT_SUCCESS;
}
