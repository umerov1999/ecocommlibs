/**
 * The `ConjointRecognitionCompletionAndPrediction` example code shows you how
 * to perform conjoint handwriting recognition, word completion and prediction.
 */


// -- configuration ------------------------------------------------------------

// common configuration parameters
#include "../common/config.h"

// ink parsing
#include "../common/parseInk.h"

// include the MyScript Engine API header(s)
#include <c/MyScriptEngine.h>
#include <c/MyScriptText.h>
#include <c/MyScriptPrediction.h>

// include the certificate
#include "../certificates/MyCertificate.h"


// -- example body -------------------------------------------------------------

#include <stdio.h>

// global engine (for the sake of readability)
voEngine engine = NULL;

// common utility functions
#include "../common/common.h"

// -- parseInk() callback ------------------------------------------------------

struct AppContext
{
  voEngine engine;
  voFloatStructuredInput input;
};

static int parseInkCallback(const float* x, size_t xByteStride,
                            const float* y, size_t yByteStride,
                            int pointCount,
                            void* userParam)
{
  bool successful;
  struct AppContext* context = (struct AppContext*)userParam;

  successful = VO_IFACE(engine, IFloatStructuredInput)->addStroke(context->engine, context->input, x, xByteStride, y, yByteStride, pointCount);

  return successful ? 0 : voGetError(context->engine);
}


// -- main function ------------------------------------------------------------

int main()
{
  voStructuredInputRecognizer recognizer = NULL;
  voPredictor predictor = NULL;
  uint32_t value;
  voResource ak = NULL;
  voResource lk = NULL;
  voResource lkEnglishSecondary = NULL;
  voInput input = NULL;
  voRecognitionResult result = NULL;
  voCandidateIterator textCandidateIterator = NULL;

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
  ak = loadEngineObject(AK_RES);
  printf(" . AlphabetKnowledge resource loaded successfully\n");
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, recognizer, ak));
  printf(" . AlphabetKnowledge resource attached successfully\n");
  lk = loadEngineObject(LK_TEXT_RES);
  printf(" . Main LinguisticKnowledge resource loaded successfully\n");
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, recognizer, lk));
  printf(" . Main LinguisticKnowledge resource attached successfully\n");
  if (strstr(LANG, "en") == NULL)
  {
    const char* path;
    if (strstr(LANG, "zh") != NULL || strstr(LANG, "ja") != NULL || strstr(LANG, "ko") != NULL)
      path = LK_ENGLISH_SECONDARY_2_RES;
    else if (strstr(LANG, "hi") != NULL)
      path = LK_ENGLISH_SECONDARY_3_RES;
    else
      path = LK_ENGLISH_SECONDARY_1_RES;

    lkEnglishSecondary = loadEngineObject(path);
    printf(" . Secondary English LinguisticKnowledge resource loaded successfully\n");
    verify(VO_IFACE(engine, IAttachTarget)->attach(engine, recognizer, lkEnglishSecondary));
    printf(" . Secondary English LinguisticKnowledge resource attached successfully\n");
  }
  printf("\n");

  printf("4) creating a FloatStructuredInput\n");
  input = voCreateFloatStructuredInput(engine);
  verify(input != NULL);
  printf(" . FloatStructuredInput created successfully\n");
  printf("\n");

  printf("5) fueling the input with digital ink\n");
  {
    struct AppContext context = { engine, input };
    FILE* file = fileOpen(DATA_DIR "/good_aftern.ink", "r");
    if (file != NULL)
    {
      verify(parseInk(file, parseInkCallback, &context) != EXIT_FAILURE);
      fclose(file);
    }
  }
  printf(" . input fueled successfully\n");
  printf("\n");

  printf("6) setting the input as the source of the recognizer\n");
  verify(VO_IFACE(engine, IInputConsumer)->setSource(engine, recognizer, input));
  printf(" . input successfully set as the recognizer's source\n");
  printf("\n");

  printf("7) creating a new Predictor\n");
  predictor = voCreatePredictor(engine);
  verify(predictor != NULL);
  printf(" . Predictor created successfully\n");
  printf("\n");

  printf("8) configuring the predictor\n");
  value = 2;
  verifyEx(voSetProperty(engine, predictor, VO_WORD_COMPLETION_CANDIDATE_LIST_SIZE, &value, sizeof(value)),
    "failed to set word completion candidate list size");
  verifyEx(voSetProperty(engine, predictor, VO_WORD_PREDICTION_CANDIDATE_LIST_SIZE, &value, sizeof(value)),
    "failed to set word prediction candidate list size");
  printf(" . Predictor configured successfully\n");
  printf("\n");

  printf("9) attaching the predictor to the recognizer\n");
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, recognizer, predictor));
  printf(" . Predictor object attached successfully\n");
  printf("\n");

  printf("10) running the handwriting recognition process\n");
  verify(VO_IFACE(engine, IRunnable)->run(engine, recognizer, NULL, NULL));
  printf(" . handwriting recognition performed successfully\n");
  printf("\n");

  printf("11) retrieving the RecognitionResult\n");
  result = VO_IFACE(engine, IRecognitionResultProvider)->getResult(engine, recognizer);
  verify(result != NULL);
  printf(" . RecognitionResult retrieved successfully\n");
  printf("\n");

  printf("12) retrieving the first candidate of the RecognitionResult\n");
  printf("    omitted characters (completion / prediction) are underlined\n");
  textCandidateIterator = VO_IFACE(engine, IRecognitionResult)->getCandidates(engine, result);
  verify(textCandidateIterator != NULL);
  if (VO_IFACE(engine, IIterator)->isAtEnd(engine, textCandidateIterator))
    printf(" . sorry, unable to recognize the input\n");
  else
  {
    voSegmentIterator wordIterator;
    int i, wordCount, padding;

    wordIterator = VO_IFACE(engine, ICandidateIterator)->getSegments(engine, textCandidateIterator);
    verify(wordIterator != NULL);

    wordCount = 0;
    while (!VO_IFACE(engine, IIterator)->isAtEnd(engine, wordIterator))
    {
      ++wordCount;
      verify(VO_IFACE(engine, IIterator)->next(engine, wordIterator));
    }
    verify(voReleaseObject(engine, wordIterator));

    printf(" . ");
    padding = 3;

    wordIterator = VO_IFACE(engine, ICandidateIterator)->getSegments(engine, textCandidateIterator);
    verify(wordIterator != NULL);
    // only the last word segment has completions / predictions candidates
    for (i = 0; i < wordCount - 1; ++i) 
    {
      voCandidateIterator wordCandidateIterator;
      int selected;
      voSegmentIterator characterIterator;

      wordCandidateIterator = VO_IFACE(engine, ISegmentIterator)->getCandidates(engine, wordIterator);
      verify(wordCandidateIterator != NULL);

      selected = VO_IFACE(engine, ISegmentIterator)->getSelectedCandidateIndex(engine, wordIterator);
      verify(selected != -1);
      while (selected-- != 0)
        verify(VO_IFACE(engine, IIterator)->next(engine, wordCandidateIterator));

      // print one character at a time
      // and keep track of number of characters printed
      characterIterator = VO_IFACE(engine, ICandidateIterator)->getSegments(engine, wordCandidateIterator);
      verify(characterIterator != NULL);
      while (!VO_IFACE(engine, IIterator)->isAtEnd(engine, characterIterator))
      {
        voCandidateIterator characterCandidateIterator;
        int selected;
        voString label = {NULL, 0};

        characterCandidateIterator = VO_IFACE(engine, ISegmentIterator)->getCandidates(engine, characterIterator);
        verify(characterCandidateIterator != NULL);

        selected = VO_IFACE(engine, ISegmentIterator)->getSelectedCandidateIndex(engine, characterIterator);
        verify(selected != -1);
        while (selected-- != 0)
          verify(VO_IFACE(engine, IIterator)->next(engine, characterCandidateIterator));

        verify(VO_IFACE(engine, ICandidateIterator)->getLabel(engine, characterCandidateIterator, NULL, &label));
        label.bytes = malloc(label.byteCount + 1);
        verify(VO_IFACE(engine, ICandidateIterator)->getLabel(engine, characterCandidateIterator, NULL, &label));
        label.bytes[label.byteCount] = '\0';
        printf("%s", label.bytes);
        free(label.bytes);

        ++padding;
        verify(voReleaseObject(engine, characterCandidateIterator));
        verify(VO_IFACE(engine, IIterator)->next(engine, characterIterator));
      }
      verify(voReleaseObject(engine, characterIterator));

      verify(voReleaseObject(engine, wordCandidateIterator));
      verify(VO_IFACE(engine, IIterator)->next(engine, wordIterator));
    }

    // last word segment 
    {
      voCandidateIterator wordCandidateIterator;
      voSegmentIterator characterIterator;

      wordCandidateIterator = VO_IFACE(engine, ISegmentIterator)->getCandidates(engine, wordIterator);
      verify(wordCandidateIterator != NULL);
      while (!VO_IFACE(engine, IIterator)->isAtEnd(engine, wordCandidateIterator))
      {
        voString label = {NULL, 0};
        int i;

        verify(VO_IFACE(engine, ICandidateIterator)->getLabel(engine, wordCandidateIterator, NULL, &label));
        label.bytes = malloc(label.byteCount + 1);
        verify(VO_IFACE(engine, ICandidateIterator)->getLabel(engine, wordCandidateIterator, NULL, &label));
        label.bytes[label.byteCount] = '\0';
        printf("%s\n", label.bytes);
        free(label.bytes);

        for (i = 0; i < padding; ++i)
          printf("%c", ' ');

        characterIterator = VO_IFACE(engine, ICandidateIterator)->getSegments(engine, wordCandidateIterator);
        verify(characterIterator != NULL);
        while (!VO_IFACE(engine, IIterator)->isAtEnd(engine, characterIterator))
        {
          voCandidateIterator characterCandidateIterator;
          int selected, flags;

          characterCandidateIterator = VO_IFACE(engine, ISegmentIterator)->getCandidates(engine, characterIterator);
          verify(characterCandidateIterator != NULL);

          selected = VO_IFACE(engine, ISegmentIterator)->getSelectedCandidateIndex(engine, characterIterator);
          verify(selected != -1);
          while (selected-- != 0)
            verify(VO_IFACE(engine, IIterator)->next(engine, characterCandidateIterator));

          flags = VO_IFACE(engine, ICandidateIterator)->getFlags(engine, characterCandidateIterator);
          if ((flags & VO_CANDIDATE_FLAG_OMITTED) != 0)
          {
            if ((flags & VO_CANDIDATE_FLAG_COMPLETED) != 0)
              printf("%c", '-');
            else if ((flags & VO_CANDIDATE_FLAG_PREDICTED) != 0)
              printf("%c", '+');
          }
          else
            printf("%c", ' ');

          verify(voReleaseObject(engine, characterCandidateIterator));
          verify(VO_IFACE(engine, IIterator)->next(engine, characterIterator));
        }
        verify(voReleaseObject(engine, characterIterator));

        printf("\n");
        for (i = 0; i < padding; ++i)
          printf("%c", ' ');

        verify(VO_IFACE(engine, IIterator)->next(engine, wordCandidateIterator));
      }
      verify(voReleaseObject(engine, wordCandidateIterator));
    }
    verify(voReleaseObject(engine, wordIterator));
  }
  printf("\n");

  printf("13) cleaning-up: releasing all created objects and destroying the Engine\n");
  verify(voReleaseObject(engine, textCandidateIterator));
  verify(voReleaseObject(engine, result));
  verify(voReleaseObject(engine, predictor));
  verify(voReleaseObject(engine, input));
  verify(voReleaseObject(engine, ak));
  if (lkEnglishSecondary != NULL)
    verify(voReleaseObject(engine, lkEnglishSecondary));
  verify(voReleaseObject(engine, lk));
  verify(voReleaseObject(engine, recognizer));
  verify(voDestroyEngine(engine));
  printf(" . clean-up done successfully\n");
  printf("\n");

  printf("end of example:\n");
  printf(" you just learnt how to perform conjoing handwriting recognition, word completion and prediction\n");

  // -- finalization -----------------------------------------------------------
  unloadMyScriptEngine();

  return EXIT_SUCCESS;
}
