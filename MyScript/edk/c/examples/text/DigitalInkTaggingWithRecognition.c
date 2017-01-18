/**
 * The `DigitalInkTaggingWithRecognition` example code shows you how to perform
 * digital ink tagging with conjoint recognition.
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
#include "DigitalInkTaggingWithRecognition.strokes.h"

// include the MyScriptText tag type utility
#include <../src/MyScriptText-strTagType.h>

// main function
int main()
{
  voStructuredInputRecognizer recognizer = NULL;
  voResource ak = NULL;
  voResource lu = NULL;
  voString name = {NULL, -1};
  voString regex = {NULL, -1};
  voDigitalInkTagger tagger = NULL;
  voInput input = NULL;
  voTagList tags = NULL;
  voTagIterator tagIterator = NULL;
  voRecognitionResult result = NULL;
  voCandidateIterator candidateIterator = NULL;
  int i = 0;

  // -- check required settings have been edited -------------------------------
  if (strlen(LANG) == 0 || strlen(HWSTYLE) == 0)
  {
    fprintf(stderr, "/!\\ please edit the common/config.h file and set:\n");
    fprintf(stderr, " . the LANG variable\n");
    fprintf(stderr, " . the HWSTYLE variable\n");
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
  printf(" . loading the AlphabetKnowledge resource\n");
  ak = loadEngineObject(AK_RES);
  printf(" . AlphabetKnowledge resource loaded successfully\n");
  printf(" . attaching the AlphabetKnowledge resource to the recognizer\n");
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, recognizer, ak));
  printf(" . AlphabetKnowledge resource attached successfully\n");

  printf(" . creating a LexicalUnit resource used to constrain the recognition\n");
  lu = voCreateLexicalUnit(engine);
  verify(lu != NULL);
  name.bytes = "lexicon";
  regex.bytes = "hello|joe|john|how|are|you";
  verify(VO_IFACE(engine, ILexicalUnit)->defineRegex(engine, lu, NULL, &name, &regex, 0));
  name.bytes = "punct";
  regex.bytes = "?";
  verify(VO_IFACE(engine, ILexicalUnit)->defineRegex(engine, lu, NULL, &name, &regex, 0));
  regex.bytes = "{lexicon}( {lexicon}|{punct})*";
  verify(VO_IFACE(engine, ILexicalUnit)->defineMainRegex(engine, lu, NULL, &regex, 0));

  printf(" . compiling the LexicalUnit\n");
  verify(VO_IFACE(engine, ICompilable)->compile(engine, lu, NULL, NULL));
  printf(" . LexicalUnit compiled successfully\n");
  printf(" . attaching the LexicalUnit to the recognizer\n");
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, recognizer, lu));
  printf(" . LexicalUnit attached successfully\n");
  printf("\n");

  printf("4) creating a new DigitalInkTagger\n");
  tagger = voCreateDigitalInkTagger(engine);
  verify(tagger != NULL);
  printf(" . DigitalInkTagger created successfully\n");
  printf("\n");

  printf("5) attaching the tagger to the recognizer\n");
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, recognizer, tagger));
  printf(" . DigitalInkTagger attached successfully\n");
  printf("\n");

  printf("6) creating a FloatStructuredInput\n");
  input = voCreateFloatStructuredInput(engine);
  verify(input != NULL);
  printf(" . FloatStructuredInput created successfully\n");
  printf("\n");

  printf("7) fueling the input with strokes from the digital ink\n");
  verify(VO_IFACE(engine, IFloatStructuredInput)->addStroke(engine, input, &s0[0], 2 * sizeof(*s0), &s0[1], 2 * sizeof(*s0), (int)(sizeof(s0)/sizeof(*s0)) / 2));
  verify(VO_IFACE(engine, IFloatStructuredInput)->addStroke(engine, input, &s1[0], 2 * sizeof(*s1), &s1[1], 2 * sizeof(*s1), (int)(sizeof(s1)/sizeof(*s1)) / 2));
  verify(VO_IFACE(engine, IFloatStructuredInput)->addStroke(engine, input, &s2[0], 2 * sizeof(*s2), &s2[1], 2 * sizeof(*s2), (int)(sizeof(s2)/sizeof(*s2)) / 2));
  verify(VO_IFACE(engine, IFloatStructuredInput)->addStroke(engine, input, &s3[0], 2 * sizeof(*s3), &s3[1], 2 * sizeof(*s3), (int)(sizeof(s3)/sizeof(*s3)) / 2));
  verify(VO_IFACE(engine, IFloatStructuredInput)->addStroke(engine, input, &s4[0], 2 * sizeof(*s4), &s4[1], 2 * sizeof(*s4), (int)(sizeof(s4)/sizeof(*s4)) / 2));
  verify(VO_IFACE(engine, IFloatStructuredInput)->addStroke(engine, input, &s5[0], 2 * sizeof(*s5), &s5[1], 2 * sizeof(*s5), (int)(sizeof(s5)/sizeof(*s5)) / 2));
  verify(VO_IFACE(engine, IFloatStructuredInput)->addStroke(engine, input, &s6[0], 2 * sizeof(*s6), &s6[1], 2 * sizeof(*s6), (int)(sizeof(s6)/sizeof(*s6)) / 2));
  verify(VO_IFACE(engine, IFloatStructuredInput)->addStroke(engine, input, &s7[0], 2 * sizeof(*s7), &s7[1], 2 * sizeof(*s7), (int)(sizeof(s7)/sizeof(*s7)) / 2));
  verify(VO_IFACE(engine, IFloatStructuredInput)->addStroke(engine, input, &s8[0], 2 * sizeof(*s8), &s8[1], 2 * sizeof(*s8), (int)(sizeof(s8)/sizeof(*s8)) / 2));
  verify(VO_IFACE(engine, IFloatStructuredInput)->addStroke(engine, input, &s9[0], 2 * sizeof(*s9), &s9[1], 2 * sizeof(*s9), (int)(sizeof(s9)/sizeof(*s9)) / 2));
  verify(VO_IFACE(engine, IFloatStructuredInput)->addStroke(engine, input, &s10[0], 2 * sizeof(*s10), &s10[1], 2 * sizeof(*s10), (int)(sizeof(s10)/sizeof(*s10)) / 2));
  verify(VO_IFACE(engine, IFloatStructuredInput)->addStroke(engine, input, &s11[0], 2 * sizeof(*s11), &s11[1], 2 * sizeof(*s11), (int)(sizeof(s11)/sizeof(*s11)) / 2));
  verify(VO_IFACE(engine, IFloatStructuredInput)->addStroke(engine, input, &s12[0], 2 * sizeof(*s12), &s12[1], 2 * sizeof(*s12), (int)(sizeof(s12)/sizeof(*s12)) / 2));
  verify(VO_IFACE(engine, IFloatStructuredInput)->addStroke(engine, input, &s13[0], 2 * sizeof(*s13), &s13[1], 2 * sizeof(*s13), (int)(sizeof(s13)/sizeof(*s13)) / 2));
  verify(VO_IFACE(engine, IFloatStructuredInput)->addStroke(engine, input, &s14[0], 2 * sizeof(*s14), &s14[1], 2 * sizeof(*s14), (int)(sizeof(s14)/sizeof(*s14)) / 2));
  verify(VO_IFACE(engine, IFloatStructuredInput)->addStroke(engine, input, &s15[0], 2 * sizeof(*s15), &s15[1], 2 * sizeof(*s15), (int)(sizeof(s15)/sizeof(*s15)) / 2));
  verify(VO_IFACE(engine, IFloatStructuredInput)->addStroke(engine, input, &s16[0], 2 * sizeof(*s16), &s16[1], 2 * sizeof(*s16), (int)(sizeof(s16)/sizeof(*s16)) / 2));
  verify(VO_IFACE(engine, IFloatStructuredInput)->addStroke(engine, input, &s17[0], 2 * sizeof(*s17), &s17[1], 2 * sizeof(*s17), (int)(sizeof(s17)/sizeof(*s17)) / 2));
  verify(VO_IFACE(engine, IFloatStructuredInput)->addStroke(engine, input, &s18[0], 2 * sizeof(*s18), &s18[1], 2 * sizeof(*s18), (int)(sizeof(s18)/sizeof(*s18)) / 2));
  verify(VO_IFACE(engine, IFloatStructuredInput)->addStroke(engine, input, &s19[0], 2 * sizeof(*s19), &s19[1], 2 * sizeof(*s19), (int)(sizeof(s19)/sizeof(*s19)) / 2));
  verify(VO_IFACE(engine, IFloatStructuredInput)->addStroke(engine, input, &s20[0], 2 * sizeof(*s20), &s20[1], 2 * sizeof(*s20), (int)(sizeof(s20)/sizeof(*s20)) / 2));
  verify(VO_IFACE(engine, IFloatStructuredInput)->addStroke(engine, input, &s21[0], 2 * sizeof(*s21), &s21[1], 2 * sizeof(*s21), (int)(sizeof(s21)/sizeof(*s21)) / 2));
  verify(VO_IFACE(engine, IFloatStructuredInput)->addStroke(engine, input, &s22[0], 2 * sizeof(*s22), &s22[1], 2 * sizeof(*s22), (int)(sizeof(s22)/sizeof(*s22)) / 2));
  verify(VO_IFACE(engine, IFloatStructuredInput)->addStroke(engine, input, &s23[0], 2 * sizeof(*s23), &s23[1], 2 * sizeof(*s23), (int)(sizeof(s23)/sizeof(*s23)) / 2));
  verify(VO_IFACE(engine, IFloatStructuredInput)->addStroke(engine, input, &s24[0], 2 * sizeof(*s24), &s24[1], 2 * sizeof(*s24), (int)(sizeof(s24)/sizeof(*s24)) / 2));
  verify(VO_IFACE(engine, IFloatStructuredInput)->addStroke(engine, input, &s25[0], 2 * sizeof(*s25), &s25[1], 2 * sizeof(*s25), (int)(sizeof(s25)/sizeof(*s25)) / 2));
  verify(VO_IFACE(engine, IFloatStructuredInput)->addStroke(engine, input, &s26[0], 2 * sizeof(*s26), &s26[1], 2 * sizeof(*s26), (int)(sizeof(s26)/sizeof(*s26)) / 2));
  verify(VO_IFACE(engine, IFloatStructuredInput)->addStroke(engine, input, &s27[0], 2 * sizeof(*s27), &s27[1], 2 * sizeof(*s27), (int)(sizeof(s27)/sizeof(*s27)) / 2));
  printf(" . input fueled successfully\n");
  printf("\n");

  printf("8) setting the input as the source of the recognizer\n");
  verify(VO_IFACE(engine, IInputConsumer)->setSource(engine, recognizer, input));
  printf(" . input successfully set as the recognizer's source\n");
  printf("\n");

  printf("9) running the conjoint digital ink tagging with recognition process\n");
  verify(VO_IFACE(engine, IRunnable)->run(engine, recognizer, NULL, NULL));
  printf(" . conjoint digital ink tagging with recognition performed successfully\n");
  printf("\n");

  printf("10) retrieving the TagList\n");
  tags = VO_IFACE(engine, IDigitalInkTagger)->getTags(engine, tagger);
  verify(tags != NULL);
  printf(" . TagList retrieved successfully\n");
  printf("\n");

  printf("11) iterating over the tags in the TagList\n");
  tagIterator = VO_IFACE(engine, ITagList)->getIterator(engine, tags);
  verify(tagIterator != NULL);

  for (i = 1; !VO_IFACE(engine, ITagIterator)->isAtEnd(engine, tagIterator); ++i)
  {
    voInputRange inputRange = {NULL, 0};
    verify(VO_IFACE(engine, ITagIterator)->getInputRange(engine, tagIterator, &inputRange));
    inputRange.elements = malloc(sizeof(*inputRange.elements)*inputRange.count);
    verify(VO_IFACE(engine, ITagIterator)->getInputRange(engine, tagIterator, &inputRange));
    printf(" . %s, ",
      strMyScriptTagType(VO_IFACE(engine, ITagIterator)->getType(engine, tagIterator)));
    printInputRange(inputRange);
    printf("\n");
    free(inputRange.elements);
    verify(VO_IFACE(engine, ITagIterator)->next(engine, tagIterator));
  }
  printf("\n");

  printf("12) retrieving the RecognitionResult\n");
  result = VO_IFACE(engine, IRecognitionResultProvider)->getResult(engine, recognizer);
  verify(result != NULL);
  printf(" . RecognitionResult retrieved successfully\n");
  printf("\n");

  printf("13) retrieving the first candidate of the RecognitionResult\n");
  candidateIterator = VO_IFACE(engine, IRecognitionResult)->getCandidates(engine, result);
  verify(candidateIterator != NULL);
  if (VO_IFACE(engine, IIterator)->isAtEnd(engine, candidateIterator))
    printf(" . sorry, unable to recognize the input\n");
  else
  {
    voString label = {NULL, 0};
    verify(VO_IFACE(engine, ICandidateIterator)->getLabel(engine, candidateIterator, NULL, &label));
    label.bytes = malloc(label.byteCount + 1);
    verify(VO_IFACE(engine, ICandidateIterator)->getLabel(engine, candidateIterator, NULL, &label));
    label.bytes[label.byteCount] = '\0';
    printf(" . input recognized as: \"%s\"\n", label.bytes);
    free(label.bytes);
  }
  printf("\n");

  printf("14) cleaning-up: releasing all created objects and destroying the Engine\n");
  verify(voReleaseObject(engine, candidateIterator));
  verify(voReleaseObject(engine, result));
  verify(voReleaseObject(engine, tagIterator));
  verify(voReleaseObject(engine, tags));
  verify(voReleaseObject(engine, tagger));
  verify(voReleaseObject(engine, lu));
  verify(voReleaseObject(engine, ak));
  verify(voReleaseObject(engine, recognizer));
  verify(voDestroyEngine(engine));
  printf(" . clean-up done successfully\n");
  printf("\n");

  printf("end of example:\n");
  printf(" you just learnt how to perform digital ink tagging with recognition\n");

  // -- finalization -----------------------------------------------------------
  unloadMyScriptEngine();

  return EXIT_SUCCESS;
}
