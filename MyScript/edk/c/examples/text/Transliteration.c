/**
* The `Transliteration` example code shows you how to perform
* handwriting recognition and transliteration on digital ink.
*/

// -- configuration ------------------------------------------------------------

// common configuration parameters
#include "../common/config.h"

// include the MyScript Engine API header(s)
#include <c/MyScriptEngine.h>
#include <c/MyScriptText.h>
#include <c/MyScriptJSON.h>
#include <c/MyScriptInk.h>

// include the certificate
#include "../certificates/MyCertificate.h"


// -- example body -------------------------------------------------------------

#include <stdio.h>

// global engine (for the sake of readability)
voEngine engine = NULL;

// common utility functions
#include "../common/common.h"

// helpers
static bool parseInput(const char* path, voFloatStructuredInput input);
static uint32_t moveIterator(voIterator iterator, uint32_t count);
static voCandidateIterator getSelectedCandidate(voSegmentIterator segment);
static char* getCandidateLabel(voCandidateIterator candidate);
static bool isSpace(char* label);

#define MAX_TRANSLITERATION_CANDIDATE_COUNT 5
//#define MAX_TRANSLITERATION_CANDIDATE_COUNT INT32_MAX

// main function
int main(int argc, char** argv)
{
  const char* default_argv_zh_CN[] = { argv[0], DATA_DIR "/transliteration_zh_CN.ink" };
  const char* default_argv_ja_JP[] = { argv[0], DATA_DIR "/transliteration_ja_JP.ink" };
  voStructuredInputRecognizer recognizer = NULL;
  voResource akCursive = NULL;
  voResource lkText = NULL;
  voResource lkEnglishSecondary = NULL;
  voInput input = NULL;
  voString str_zh_CN = voString_INITIALIZER("nali ma?");
  voString str_ja_JP = voString_INITIALIZER("ですね。");
  voString* string = NULL;

  int exitCode = EXIT_SUCCESS;
  int i;

#ifdef _WIN32
  SetConsoleOutputCP(CP_UTF8);
#endif

  // -- check required settings have been edited -------------------------------
  // e.g. set LANG to "zh_CN" and HWSTYLE to "cur"
if (strlen(LANG) == 0 || strlen(HWSTYLE) == 0)
  {
    fprintf(stderr, "/!\\ please edit the common/config.h file and set:\n");
    fprintf(stderr, " . the LANG variable\n");
    fprintf(stderr, " . the HWSTYLE variable\n");
    fflush(stderr);
    exit(EXIT_FAILURE);
  }

  // -- check arguments --------------------------------------------------------
  if (strcmp(LANG, "zh_CN") == 0)
  {
    if (argc == 1)
    {
      argc = (int)(sizeof(default_argv_zh_CN) / sizeof(default_argv_zh_CN[0]));
      argv = (char**)default_argv_zh_CN;
    }

    string = &str_zh_CN;
  }
  else if (argc == 1 && strcmp(LANG, "ja_JP") == 0 )
  {
    if (argc == 1)
    {
      argc = (int)(sizeof(default_argv_ja_JP) / sizeof(default_argv_ja_JP[0]));
      argv = (char**)default_argv_ja_JP;
    }

    string = &str_ja_JP;
  }
  else
  {
    fprintf(stderr, "/!\\ transliteration is only supported in zh_CN or ja_JP\n");
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

  printf("3) enabling transliteration\n");
  {
    bool enableTransliteration = true;
    verify(voSetProperty(engine, recognizer, VO_ENABLE_TRANSLITERATION, &enableTransliteration, sizeof(bool)));
  }
  printf(" . property set successfully\n");
  printf("\n");

  printf("4) loading and attaching resources to the recognizer\n");
  akCursive = loadEngineObject(AK_CUR_RES);
  printf(" . CursiveAlphabetKnowledge resource loaded successfully\n");
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, recognizer, akCursive));
  printf(" . CursiveAlphabetKnowledge resource attached successfully\n");
  lkText = loadEngineObject(LK_TEXT_RES);
  printf(" . Main LinguisticKnowledge resource loaded successfully\n");
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, recognizer, lkText));
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

  printf("5) creating a FloatStructuredInput\n");
  input = voCreateFloatStructuredInput(engine);
  verify(input != NULL);
  printf(" . FloatStructuredInput created successfully\n");
  printf("\n");

  printf("6) setting the input as the source of the recognizer\n");
  verify(VO_IFACE(engine, IInputConsumer)->setSource(engine, recognizer, input));
  printf(" . input successfully set as the recognizer's source\n");
  printf("\n");

  printf("7) processing input files\n");

  for (i = 1; i < argc; ++i)
  {
    voRecognitionResult result = NULL;
    voCandidateIterator textCandidateIterator = NULL;
    voSegmentIterator wordSegmentIterator  = NULL;
    voCandidateIterator selectedWordCandidate = NULL;

    if (!fileExists(argv[i]))
    {
      fprintf(stderr, "missing ink file: %s\n", argv[i]);
      exitCode = EXIT_FAILURE;
      break;
    }

    printf(" . parsing input ink ");
    printPath(stdout, argv[i]);
    printf("\n");
    verify(parseInput(argv[i], input));

    printf("   . input fueled successfully\n");
    printf("\n");

    if (string)
    {
      printf(" . adding digital text\n");
      verify(VO_IFACE(engine, IStructuredInput)->addString(engine, input, NULL, string));
      printf("   . digital text added successfully\n");
      printf("\n");
    }

    printf(" .  running the handwriting recognition process\n");
    verify(VO_IFACE(engine, IRunnable)->run(engine, recognizer, NULL, NULL));
    printf("   . handwriting recognition performed successfully\n");
    printf("\n");

    printf(" . retrieving the RecognitionResult\n");
    result = VO_IFACE(engine, IRecognitionResultProvider)->getResult(engine, recognizer);
    verify(result != NULL);
    printf("   . RecognitionResult retrieved successfully\n");
    printf("\n");

    printf(" . retrieving the first candidate of the RecognitionResult");
    printf("\n");

    textCandidateIterator = VO_IFACE(engine, IRecognitionResult)->getCandidates(engine, result);
    verify(textCandidateIterator != NULL);
    if (VO_IFACE(engine, IIterator)->isAtEnd(engine, textCandidateIterator))
      printf("   . sorry, unable to recognize the input\n");
    else
    {
      char* label = NULL;
      label = getCandidateLabel(textCandidateIterator);
      printf("   . input recognized as: \"%s\"\n", label);
      free(label);
    }
    printf("\n");

    printf(" . iterating over word segments and displaying transliteration candidates");
    printf("\n");
    wordSegmentIterator = VO_IFACE(engine, ICandidateIterator)->getSegments(engine, textCandidateIterator);
    verify(wordSegmentIterator != NULL);

    printf("   . \n");

    while (!(VO_IFACE(engine, ISegmentIterator)->isAtEnd(engine, wordSegmentIterator)))
    {
      char* label = NULL;
      bool isSpaceSegment, lastWord;

      selectedWordCandidate = getSelectedCandidate(wordSegmentIterator);
      label = getCandidateLabel(selectedWordCandidate);

      verify(VO_IFACE(engine, ISegmentIterator)->next(engine, wordSegmentIterator));
      lastWord = VO_IFACE(engine, ISegmentIterator)->isAtEnd(engine, wordSegmentIterator) == VO_YES;

      isSpaceSegment = isSpace(label);
      if(!isSpaceSegment)
      {
        voCandidateIterator transliterationCandidateIterator = NULL;
        int selected, j;

        if (lastWord)
          printf("   └── \"%s\"\n", label);
        else
          printf("   ├── \"%s\"\n", label);

        free(label);

        transliterationCandidateIterator = VO_IFACE(engine, ICandidateIterator)->getTransliterationCandidates(engine, selectedWordCandidate);
        if (!(VO_IFACE(engine, ICandidateIterator)->isAtEnd(engine, transliterationCandidateIterator)))
        {
          selected = VO_IFACE(engine, ICandidateIterator)->getSelectedTransliterationCandidateIndex(engine, selectedWordCandidate);

          j = 0;
          while (!(VO_IFACE(engine, ICandidateIterator)->isAtEnd(engine, transliterationCandidateIterator)))
          {
            char* label = NULL;
            bool lastTransliterationCandidate;

            if (lastWord)
              printf("   └──");
            else
              printf("   │  ");

            label = getCandidateLabel(transliterationCandidateIterator);

            ++j;
            VO_IFACE(engine, ICandidateIterator)->next(engine, transliterationCandidateIterator);
            lastTransliterationCandidate = (j == MAX_TRANSLITERATION_CANDIDATE_COUNT) || VO_IFACE(engine, ICandidateIterator)->isAtEnd(engine, transliterationCandidateIterator);

            if (lastTransliterationCandidate)
              printf(" └── \"%s\"", label);
            else
              printf(" ├── \"%s\"", label);

            if (selected == j - 1)
              printf(" (selected)");
            printf("\n");

            free(label);

            if (j == MAX_TRANSLITERATION_CANDIDATE_COUNT && j >= selected)
              break;
          }

          verify(voReleaseObject(engine, transliterationCandidateIterator));
        }
      }

      if (!lastWord && !isSpaceSegment)
        printf("   │\n");

      verify(voReleaseObject(engine, selectedWordCandidate));
    }
    printf("\n");

    verify(voReleaseObject(engine, wordSegmentIterator));
    verify(voReleaseObject(engine, textCandidateIterator));
    verify(voReleaseObject(engine, result));

    verify(VO_IFACE(engine, IFloatStructuredInput)->clear(engine, input, false));

  } // end of: for (i = 1; i < argc; ++i)

  printf("8) cleaning-up: releasing all created objects and destroying the Engine\n");
  verify(voReleaseObject(engine, input));
  if (lkEnglishSecondary != NULL)
    verify(voReleaseObject(engine, lkEnglishSecondary));
  verify(voReleaseObject(engine, lkText));
  verify(voReleaseObject(engine, akCursive));
  verify(voReleaseObject(engine, recognizer));
  verify(voDestroyEngine(engine));
  printf(" . clean-up done successfully\n");
  printf("\n");

  printf("end of example:\n");
  printf(" you just learnt how to perform handwriting recognition and transliteration on digital ink\n");

  // -- finalization -----------------------------------------------------------
  unloadMyScriptEngine();

  return exitCode;
}


// -- Helpers ------------------------------------------------------------------

/**
 * Parses ink file and feed the input
 *
 * @param path the .ink file path.
 * @param input the FloatStructuredInput to feed the ink into.
 *
 * @return `true` on success, otherwise
 * `false`.
 */
static bool parseInput(const char* path, voFloatStructuredInput input)
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

        if (!VO_IFACE(engine, IFloatStructuredInput)->addStroke(engine, input, &points->x, sizeof(voPoint), &points->y, sizeof(voPoint), pointCount))
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

        if (!VO_IFACE(engine, IFloatStructuredInput)->addStroke(engine, input, &points->x, sizeof(voPoint), &points->y, sizeof(voPoint), pointCount))
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

/**
 * Tells if the specified label is a space.
 *
 * @return `true` if the specified label is a space, otherwise
 * `false`.
 */
static bool isSpace(char* label)
{
  verifyEx(label != NULL, "invalid label");

  return strcmp(label, "") == 0 || strcmp(label, " ") == 0 || strcmp(label, "\xA0") == 0 || strcmp(label, "\n") == 0;
}

/**
 * Returns the label of the current candidate.
 *
 * @return the label of the current candidate.
 */
static char* getCandidateLabel(voCandidateIterator candidate)
{
  voString label = {NULL, 0};

  verifyEx(candidate != NULL, "invalid candidate");
  verifyEx(!VO_IFACE(engine, IIterator)->isAtEnd(engine, candidate), "candidate iterator is at end");

  verify(VO_IFACE(engine, ICandidateIterator)->getLabel(engine, candidate, NULL, &label));
  label.bytes = malloc(label.byteCount + 1);
  verify(VO_IFACE(engine, ICandidateIterator)->getLabel(engine, candidate, NULL, &label));
  label.bytes[label.byteCount] = '\0';

  return label.bytes;
}

/**
 * Returns a candidate iterator positioned on the selected candidate of the
 * specified segment.
 *
 * @param segment a segment iterator.
 *
 * @return a candidate iterator positioned on the selected candidate of the
 * specified segment.
 */
static voCandidateIterator getSelectedCandidate(voSegmentIterator segment)
{
  voCandidateIterator candidate;
  uint32_t index;

  verifyEx(segment != NULL, "invalid segment");

  candidate = VO_IFACE(engine, ISegmentIterator)->getCandidates(engine, segment);
  index = VO_IFACE(engine, ISegmentIterator)->getSelectedCandidateIndex(engine, segment);
  verify(index == moveIterator(candidate, index));

  return candidate;
}

/**
 * Moves the given iterator by `count` steps.
 *
 * @param count the number of steps to perform.
 *
 * @return The number of steps actually performed.
 */
static uint32_t moveIterator(voIterator iterator, uint32_t count)
{
  uint32_t i;

  verifyEx(iterator != NULL, "invalid iterator");

  for (i = 0; (i < count) && !VO_IFACE(engine, IIterator)->isAtEnd(engine, iterator); ++i)
    verify(VO_IFACE(engine, IIterator)->next(engine, iterator));

  return i;
}
