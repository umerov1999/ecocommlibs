/**
 * The `ExploreRecognitionResult` example code shows how to explore the
 * `RecognitionResult` and how to manipulate it.
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

// forward declaration of example functions
static voCandidateIterator getTextCandidates(voRecognitionResult result);
static uint32_t printCandidateLabels(voCandidateIterator candidates, uint32_t selected);
static uint32_t displayMenu(char* text, uint32_t count);
static uint32_t moveIterator(voIterator iterator, uint32_t count);
static voCandidateIterator getTextCandidate(voRecognitionResult result, uint32_t index);
static char* getCandidateLabel(voCandidateIterator candidate);
static voCandidateIterator getSelectedCandidate(voSegmentIterator segment);
static bool isSpace(char* label);
static uint32_t printSelectedCandidateLabels(voSegmentIterator segments, bool ignoreSpaceSegments);
static char* getSegmentSelectedCandidateLabel(voSegmentIterator segments, uint32_t index, bool ignoreSpaceSegments);
static voSegmentIterator getSegment(voCandidateIterator textCandidate, uint32_t index, bool ignoreSpaceSegments);
static voCandidateIterator getSegmentCandidate(voSegmentIterator segments, uint32_t segmentIndex, uint32_t candidateIndex, bool ignoreSpaceSegments);
static void printSelectedCandidateScores(voSegmentIterator segments);
static bool parseInput(const char* path, voFloatStructuredInput input);

// example main functions.

/**
 * Configures the given recognizer so that it produces the desired number of
 * candidates at each level and the desired case variation constraints.
 * This has to be done before the recognition process.
 */
static void configureCandidateListProduction(voRecognizer recognizer)
{
  uint32_t charCandidateListSize;
  uint32_t wordCandidateListSize;
  uint32_t textCandidateListSize;
  bool discardCaseVariations;

  charCandidateListSize = 5;
  verifyEx(voSetProperty(engine, recognizer, VO_CHAR_CANDIDATE_LIST_SIZE, &charCandidateListSize, sizeof(charCandidateListSize)),
    "failed to set character candidate list size");
  wordCandidateListSize = 5;
  verifyEx(voSetProperty(engine, recognizer, VO_WORD_CANDIDATE_LIST_SIZE, &wordCandidateListSize, sizeof(wordCandidateListSize)),
    "failed to set word candidate list size");
  textCandidateListSize = 10;
  verifyEx(voSetProperty(engine, recognizer, VO_TEXT_CANDIDATE_LIST_SIZE, &textCandidateListSize, sizeof(textCandidateListSize)),
    "failed to set text candidate list size");

  discardCaseVariations = true;
  verifyEx(voSetProperty(engine, recognizer, VO_DISCARD_CASE_VARIATIONS, &discardCaseVariations, sizeof(discardCaseVariations)),
    "failed to set discard case variations");
}

/**
 * Configures the given recognizer so that it overrides the casing scheme and
 * accentuation scheme of the `Vocabulary` resources attached to it.
 * This has to be done before the recognition process.
 */
static void configureCasingAndAccentuationSchemeOverrides(voRecognizer recognizer)
{
  voId value = -1;
  
  printf("1) casing scheme override configuration\n");
  printf(" [1] NONE\n");
  printf(" [2] STRICT_CASE\n");
  printf(" [3] LEGACY_CASE\n");
  printf(" [4] LAX_CASE\n");
  printf("\n");
  
  value = (voId)displayMenu("choose a casing scheme override for the rest of the example", 4) - 1;
  printf("\n");
  
  if (value != -1)
    verifyEx(voSetProperty(engine, recognizer, VO_OVERRIDE_CASING_SCHEME, &value, sizeof(value)),
      "failed to override casing scheme");

  printf("2) accentuation scheme override configuration\n");
  printf(" [1] NONE\n");
  printf(" [2] STRICT_ACCENTUATION\n");
  printf(" [3] LAX_ACCENTUATION\n");
  printf("\n");
  
  value = (voId)displayMenu("choose a casing scheme override for the rest of the example", 3) - 1;
  printf("\n");
  
  if (value != -1)
    verifyEx(voSetProperty(engine, recognizer, VO_OVERRIDE_ACCENTUATION_SCHEME, &value, sizeof(value)),
             "failed to override casing scheme");  
}

/**
 * Shows how to explore the recognition result.
 */
static void exploreRecognitionResult(voRecognitionResult result)
{
  voCandidateIterator textCandidates = NULL;
  uint32_t textCandidateIndex = 0;
  voCandidateIterator textCandidate = NULL;
  uint32_t textCandidateCount = 0;
  char* label = NULL;
  voSegmentIterator wordSegments = NULL;
  uint32_t wordSegmentCount = 0;
  uint32_t wordSegmentIndex = 0;
  voSegmentIterator wordSegment = NULL;
  uint32_t selectedWordCandidateIndex = 0;
  voCandidateIterator wordCandidates = NULL;
  uint32_t wordCandidateCount = 0;
  uint32_t wordCandidateIndex = 0;
  voCandidateIterator wordCandidate = NULL;
  voSegmentIterator characterSegments = NULL;
  uint32_t characterSegmentCount = 0;
  uint32_t characterSegmentIndex = 0;
  voSegmentIterator characterSegment = NULL;
  uint32_t selectedCharacterCandidateIndex = 0;
  voCandidateIterator characterCandidates = NULL;
  float rs = 0;
  float nrs = 0;

  printf("-- recognition result structure analysis ---------------------------------------");
  printf("\n");

  printf("1) candidate list at the text level\n");
  textCandidates = getTextCandidates(result);
  textCandidateCount = printCandidateLabels(textCandidates, -1);
  verify(voReleaseObject(engine, textCandidates));
  printf("\n");

  textCandidateIndex = displayMenu("2) choose a text candidate for the rest of the example", textCandidateCount);
  textCandidate = getTextCandidate(result, textCandidateIndex);
  label = getCandidateLabel(textCandidate);
  printf(" [%2u] \"%s\" chosen\n", textCandidateIndex + 1, label);
  free (label);
  printf("\n");

  printf("3) selected candidates for each word segment of the chosen text candidate\n");
  printf("   see the documentation for the concept of \"selected candidate\"\n");
  wordSegments = VO_IFACE(engine, ICandidateIterator)->getSegments(engine, textCandidate);
  wordSegmentCount = printSelectedCandidateLabels(wordSegments, true);
  verify(voReleaseObject(engine, wordSegments));
  printf("\n");

  wordSegmentIndex = displayMenu("4) choose a word segment for the rest of the example", wordSegmentCount);
  wordSegments = VO_IFACE(engine, ICandidateIterator)->getSegments(engine, textCandidate);
  label = getSegmentSelectedCandidateLabel(wordSegments, wordSegmentIndex, true);
  printf(" [%2u] \"%s\" chosen\n", wordSegmentIndex + 1, label);
  free (label);
  verify(voReleaseObject(engine, wordSegments));
  printf("\n");

  printf("5) candidate list for the chosen word segment\n");
  wordSegment = getSegment(textCandidate, wordSegmentIndex, true);
  verify(wordSegment != NULL);
  selectedWordCandidateIndex = VO_IFACE(engine, ISegmentIterator)->getSelectedCandidateIndex(engine, wordSegment);
  wordCandidates = VO_IFACE(engine, ISegmentIterator)->getCandidates(engine, wordSegment);
  wordCandidateCount = printCandidateLabels(wordCandidates, selectedWordCandidateIndex);
  verify(voReleaseObject(engine, wordCandidates));
  verify(voReleaseObject(engine, wordSegment));
  printf("\n");

  wordCandidateIndex = displayMenu("6) choose a word candidate for the rest of the example", wordCandidateCount);
  wordSegments = VO_IFACE(engine, ICandidateIterator)->getSegments(engine, textCandidate);
  wordCandidate = getSegmentCandidate(wordSegments, wordSegmentIndex, wordCandidateIndex, true);
  label = getCandidateLabel(wordCandidate);
  printf(" [%2u] \"%s\" chosen\n", wordCandidateIndex + 1, label);
  free (label);
  verify(voReleaseObject(engine, wordSegments));
  printf("\n");

  printf("7) selected candidates for each character segment of the chosen word candidate\n");
  printf("   see the documentation for the concept of \"selected candidate\"\n");
  characterSegments = VO_IFACE(engine, ICandidateIterator)->getSegments(engine, wordCandidate);
  characterSegmentCount = printSelectedCandidateLabels(characterSegments, true);
  verify(voReleaseObject(engine, characterSegments));
  printf("\n");

  characterSegmentIndex = displayMenu("8) choose a character segment for the rest of the example", characterSegmentCount);
  characterSegments = VO_IFACE(engine, ICandidateIterator)->getSegments(engine, wordCandidate);
  label = getSegmentSelectedCandidateLabel(characterSegments, characterSegmentIndex, false);
  printf(" [%2u] \"%s\" chosen\n", characterSegmentIndex + 1, label);
  free (label);
  verify(voReleaseObject(engine, characterSegments));
  printf("\n");

  printf("9) candidate list for the chosen character segment\n");
  characterSegment = getSegment(wordCandidate, characterSegmentIndex, true);
  verify(characterSegment != NULL);
  selectedCharacterCandidateIndex = VO_IFACE(engine, ISegmentIterator)->getSelectedCandidateIndex(engine, characterSegment);
  characterCandidates = VO_IFACE(engine, ISegmentIterator)->getCandidates(engine, characterSegment);
  printCandidateLabels(characterCandidates, selectedCharacterCandidateIndex);
  verify(voReleaseObject(engine, characterCandidates));
  verify(voReleaseObject(engine, characterSegment));
  printf("\n");

  verify(voReleaseObject(engine, textCandidate));
  verify(voReleaseObject(engine, wordCandidate));

  printf("-- recognition result scores analysis ------------------------------------------");
  printf("\n");

  printf("format: NRS: (normalized recognition score) - (label)\n");
  printf("\n");

  printf("1) candidate list at the text level\n");
  textCandidates = getTextCandidates(result);
  textCandidateCount = printCandidateLabels(textCandidates, -1);
  verify(voReleaseObject(engine, textCandidates));
  printf("\n");

  textCandidateIndex = displayMenu("2) choose a text candidate for the rest of the example", textCandidateCount);
  textCandidate = getTextCandidate(result, textCandidateIndex);
  label = getCandidateLabel(textCandidate);
  printf(" [%2u] \"%s\" chosen\n", textCandidateIndex + 1, label);
  free (label);
  printf("\n");

  printf("3) chosen text candidate\n");
  nrs = VO_IFACE(engine, ICandidateIterator)->getNormalizedRecognitionScore(engine, textCandidate);
  label = getCandidateLabel(textCandidate);

  printf(" NRS: %.4f - \"%s\"\n", nrs, label);

  free (label);
  printf("\n");

  printf("4) selected candidates for each word segment of the chosen text candidate\n");
  wordSegments = VO_IFACE(engine, ICandidateIterator)->getSegments(engine, textCandidate);
  printSelectedCandidateScores(wordSegments);
  verify(voReleaseObject(engine, wordSegments));
  printf("\n");

  printf("5) selected candidates for each character segment of the chosen text candidate\n");
  wordSegments = VO_IFACE(engine, ICandidateIterator)->getSegments(engine, textCandidate);
  while (!VO_IFACE(engine, ISegmentIterator)->isAtEnd(engine, wordSegments))
  {
    wordCandidate = getSelectedCandidate(wordSegments);
    verify(wordCandidate != NULL);
    
    characterSegments = VO_IFACE(engine, ICandidateIterator)->getSegments(engine, wordCandidate);
    printSelectedCandidateScores(characterSegments);
    verify(voReleaseObject(engine, characterSegments));
    
    verify(VO_IFACE(engine, ISegmentIterator)->next(engine, wordSegments));
  }
  verify(voReleaseObject(engine, wordSegments));

  verify(voReleaseObject(engine, textCandidate));
  verify(voReleaseObject(engine, wordCandidate));
}

// example functions

/**
 * Returns a candidate iterator at the text level.
 */
static voCandidateIterator getTextCandidates(voRecognitionResult result)
{
  voCandidateIterator candidateIterator;
  
  candidateIterator = VO_IFACE(engine, IRecognitionResult)->getCandidates(engine, result);
  verify(candidateIterator != NULL);

  return candidateIterator;
}

/**
 * Prints the label of each candidate of the candidate iterator.
 *
 * @param selected the index of the selected candidate.
 *
 * @return the number of candidates printed.
 */
static uint32_t printCandidateLabels(voCandidateIterator candidateIterator, uint32_t selected)
{
  uint32_t i = 0;

  verifyEx(candidateIterator != NULL, "invalid candidate iterator");
  verifyEx(!VO_IFACE(engine, IIterator)->isAtEnd(engine, candidateIterator), "candidate iterator is already at end");

  while (!VO_IFACE(engine, ICandidateIterator)->isAtEnd(engine, candidateIterator))
  {
    char* label = getCandidateLabel(candidateIterator);

    printf(" [%2u] \"%s\"", i+1, label);
    free(label);

    if (selected != (uint32_t)-1 && i == selected)
      printf(" (selected)");

    printf("\n");

    ++i;
    verify(VO_IFACE(engine, ICandidateIterator)->next(engine, candidateIterator));
  }

  return i;
}

/**
 * Displays a menu for user selection.
 *
 * @param message a message to be displayed.
 * @param count the number of values that can be chosen.
 *
 * @return the user choice.
 */
static uint32_t displayMenu(char* message, uint32_t count)
{
  uint32_t menuItem = 0;

  do
  {
    char line[256];
    printf("%s [1-%u] : ", message, (uint32_t)count);
    if (!fgets(line, (int)sizeof(line), stdin))
    {
      fprintf(stderr, "unable to read from stdin\n");
      exit(EXIT_FAILURE);
    }
    sscanf(line, "%u", &menuItem);
  }
  while ((menuItem == 0) || (menuItem > count));

  return menuItem - 1;
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

/**
 * Returns a candidate iterator positioned on the text candidate at the
 * specified `index`.
 *
 * @param result a recognition result.
 * @param index the index of the text candidate to be returned.
 *
 * @return a candidate iterator positioned on the text candidate at the
 * specified `index`.
 */
static voCandidateIterator getTextCandidate(voRecognitionResult result, uint32_t index)
{
  voCandidateIterator candidateIterator;

  verifyEx(result != NULL, "invalid recognition result");

  candidateIterator = VO_IFACE(engine, IRecognitionResult)->getCandidates(engine, result);
  verify(candidateIterator != NULL);
  moveIterator(candidateIterator, index);

  return candidateIterator;
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
 * Tells if the specified label is a space.
 *
 * @return `true` if the specified label is a space, otherwise
 * `false`.
 */
static bool isSpace(char* label)
{
  verifyEx(label != NULL, "invalid label");

  return strcmp(label, "") == 0 || strcmp(label, " ") == 0 || strcmp(label, "\xA0") == 0;
}

/**
 * Prints the label of the selected candidate for each segment.
 *
 * @param segments a segment iterator.
 * @param ignoreSpaceSegments ignore space segments.
 *
 * @return the number of candidate printed.
 */
static uint32_t printSelectedCandidateLabels(voSegmentIterator segments, bool ignoreSpaceSegments)
{
  uint32_t current = 1;

  verifyEx(segments != NULL, "invalid segment iterator");
  verifyEx(!VO_IFACE(engine, IIterator)->isAtEnd(engine, segments), "iterator is at end");

  while (!VO_IFACE(engine, ISegmentIterator)->isAtEnd(engine, segments))
  {
    voCandidateIterator candidate = NULL;
    char* label = NULL;

    candidate = getSelectedCandidate(segments);
    verify(candidate != NULL);

    label = getCandidateLabel(candidate);

    if (ignoreSpaceSegments && isSpace(label))
      printf(" [--] ");
    else
      printf(" [%2u] ", current++);

    printf("\"%s\"\n", label);

    free(label);

    verify(voReleaseObject(engine, candidate));

    verify(VO_IFACE(engine, ISegmentIterator)->next(engine, segments));
  }

  return current - 1;
}

/**
 * Returns the label of the selected candidate for the specified segment.
 *
 * @param segments a segment iterator positioned at the beginning of the
 * iteration.
 * @param index the index of the segment.
 * @param ignoreSpaceSegments ignore space segments.
 *
 * @return the label of the selected candidate for the specified segment.
 */
static char* getSegmentSelectedCandidateLabel(voSegmentIterator segments, uint32_t index, bool ignoreSpaceSegments)
{
  uint32_t current = 0;

  verifyEx(segments != NULL, "invalid segment iterator");
  verifyEx(!VO_IFACE(engine, IIterator)->isAtEnd(engine, segments), "iterator is at end");

  while (!VO_IFACE(engine, ISegmentIterator)->isAtEnd(engine, segments))
  {
    voCandidateIterator candidate = NULL;
    char* label = NULL;

    candidate = getSelectedCandidate(segments);
    verify(candidate != NULL);

    label = getCandidateLabel(candidate);

    verify(voReleaseObject(engine, candidate));

    if (!(ignoreSpaceSegments && isSpace(label)) && (current++ == index))
      return label;

    free(label);

    verify(VO_IFACE(engine, ISegmentIterator)->next(engine, segments));
  }

  return NULL;
}

/**
 * Returns the specified candidate for the specified segment.
 *
 * @param segments a segment iterator positioned at the beginning of the
 * iteration.
 * @param index the index of the segment.
 * @param index the index of the candidate.
 * @param ignoreSpaceSegments ignore space segments.
 *
 * @return the specified candidate for the specified segment.
 */
static voCandidateIterator getSegmentCandidate(voSegmentIterator segments, uint32_t segmentIndex, uint32_t candidateIndex, bool ignoreSpaceSegments)
{
  uint32_t current = 0;

  verifyEx(segments != NULL, "invalid segment iterator");
  verifyEx(!VO_IFACE(engine, IIterator)->isAtEnd(engine, segments), "iterator is at end");

  while (!VO_IFACE(engine, ISegmentIterator)->isAtEnd(engine, segments))
  {
    voCandidateIterator candidate = NULL;
    char* label = NULL;

    candidate = getSelectedCandidate(segments);
    verify(candidate != NULL);

    label = getCandidateLabel(candidate);

    verify(voReleaseObject(engine, candidate));

    if (!(ignoreSpaceSegments && isSpace(label)) && (current++ == segmentIndex))
    {
      free(label);

      candidate = VO_IFACE(engine, ISegmentIterator)->getCandidates(engine, segments);
      verify(candidate != NULL);

      moveIterator(candidate, candidateIndex);

      return candidate;
    }

    free(label);

    verify(VO_IFACE(engine, ISegmentIterator)->next(engine, segments));
  }

  return NULL;
}

/**
 * Returns a segment iterator positioned on the word segment at the specified
 * index of the given text candidate.
 *
 * @return a segment iterator positioned on the word segment at the specified
 * index of the given text candidate.
 */
static voSegmentIterator getSegment(voCandidateIterator textCandidate, uint32_t index, bool ignoreSpaceSegments)
{
  voSegmentIterator segment = NULL;
  uint32_t current = 0;
  
  verifyEx(textCandidate != NULL, "invalid text candidate");
  verifyEx(!VO_IFACE(engine, IIterator)->isAtEnd(engine, textCandidate),"iterator is at end");

  segment = VO_IFACE(engine, ICandidateIterator)->getSegments(engine, textCandidate);
  verify(segment != NULL);

  while (!VO_IFACE(engine, ISegmentIterator)->isAtEnd(engine, segment))
  {
    voCandidateIterator candidate = NULL;
    char* label = NULL;

    candidate = getSelectedCandidate(segment);
    verify(candidate != NULL);

    label = getCandidateLabel(candidate);

    verify(voReleaseObject(engine, candidate));

    if (!(ignoreSpaceSegments && isSpace(label)) && (current++ == index))
    {
      free(label);
      return segment;
    }

    free(label);

    verify(VO_IFACE(engine, ISegmentIterator)->next(engine, segment));
  }

  return segment;
}

/**
 * Prints the normalized recognition scores of the selected candidate for each
 * segment.
 *
 * @param segments a segment iterator.
 */
static void printSelectedCandidateScores(voSegmentIterator segments)
{
  verifyEx(segments != NULL, "invalid segment iterator");
  verifyEx(!VO_IFACE(engine, IIterator)->isAtEnd(engine, segments), "iterator is at end");

  while (!VO_IFACE(engine, ISegmentIterator)->isAtEnd(engine, segments))
  {
    voCandidateIterator candidate = NULL;
    char* label = NULL;
    float nrs;

    candidate = getSelectedCandidate(segments);
    verify(candidate != NULL);

    label = getCandidateLabel(candidate);
    nrs = VO_IFACE(engine, ICandidateIterator)->getNormalizedRecognitionScore(engine, candidate);

    printf(" NRS: %.4f - \"%s\"\n", nrs, label);

    free(label);

    verify(voReleaseObject(engine, candidate));

    verify(VO_IFACE(engine, ISegmentIterator)->next(engine, segments));
  }
}

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


// main function
int main(int argc, char** argv)
{
  const char* default_argv[] = { argv[0], DATA_DIR "/horsemanship.ink" };
  voStructuredInputRecognizer recognizer = NULL;
  voAlphabetKnowledge alphabetKnowledge = NULL;
  voLexicalUnit lexicalUnit = NULL;
  voString name = {NULL, -1};
  voString regex = {NULL, -1};
  voInput input = NULL;
  voRecognitionResult result = NULL;
  voCandidateIterator candidateIterator = NULL;
  int i;
  int exitCode = EXIT_SUCCESS;

  // -- check arguments -------------------------------
  if (argc == 1)
  {
    argc = (int)(sizeof(default_argv) / sizeof(default_argv[0]));
    argv = (char**)default_argv;
  }

  // -- check required settings have been edited -------------------------------
  // e.g. set the LANG variable to "en_US" and set the HWSTYLE variable to "cur"
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
  engine = voCreateEngine(VO_MSE_VER, &myCertificate, NULL);
  verifyEx(engine != NULL, "failed to create the engine");

  recognizer = voCreateStructuredInputRecognizer(engine);
  verifyEx(recognizer != NULL, "failed to create the recognizer");

  alphabetKnowledge = loadEngineObject(AK_RES);
  verifyEx(VO_IFACE(engine, IAttachTarget)->attach(engine, recognizer, alphabetKnowledge), "failed to attach the alphabet knowledge");
  verify(voReleaseObject(engine, alphabetKnowledge));

  lexicalUnit = voCreateLexicalUnit(engine);
  name.bytes = "lexicon";
  regex.bytes = "horse|house|man|more|ship|skip|horseman|warship|horsemanship";
  verify(VO_IFACE(engine, ILexicalUnit)->defineRegex(engine, lexicalUnit, NULL, &name, &regex, 0));
  regex.bytes = "{lexicon}( {lexicon})*";
  verify(VO_IFACE(engine, ILexicalUnit)->defineMainRegex(engine, lexicalUnit, NULL, &regex, 0));
  verify(VO_IFACE(engine, ICompilable)->compile(engine, lexicalUnit, NULL, NULL));
  verifyEx(VO_IFACE(engine, IAttachTarget)->attach(engine, recognizer, lexicalUnit),
    "failed to attach the lexicalUnit");
  verify(voReleaseObject(engine, lexicalUnit));

  input = voCreateFloatStructuredInput(engine);
  verifyEx(input != NULL, 
    "failed to create the input");

  for (i = 1; i < argc; ++i)
  {
    if (!fileExists(argv[i]))
    {
      fprintf(stderr, "missing ink file: %s\n", argv[i]);
      exitCode = EXIT_FAILURE;
      break;
    }
    
    printf("parsing input ink: ");
    printPath(stdout, argv[i]);
    printf("\n\n");
    verify(parseInput(argv[i], input));

    verifyEx(VO_IFACE(engine, IInputConsumer)->setSource(engine, recognizer, input), "input not set as the recognizer's source");

    printf("-- recognizer configuration ----------------------------------------------------");
    printf("\n");
    configureCandidateListProduction(recognizer);
    configureCasingAndAccentuationSchemeOverrides(recognizer);
  
    verifyEx(VO_IFACE(engine, IRunnable)->run(engine, recognizer, NULL, NULL), "handwriting recognition failed");

    result = VO_IFACE(engine, IRecognitionResultProvider)->getResult(engine, recognizer);
    verifyEx(result != NULL, "RecognitionResult not retrieved");

    candidateIterator = VO_IFACE(engine, IRecognitionResult)->getCandidates(engine, result);

    if (!VO_IFACE(engine, ICandidateIterator)->isAtEnd(engine, candidateIterator))
      exploreRecognitionResult(result);
    else
      printf("no recognition result found :(\n");

    verify(VO_IFACE(engine, IFloatStructuredInput)->clear(engine, input, false));

  } // end of: for (i = 1; i < argc; ++i)

  verify(voReleaseObject(engine, candidateIterator));
  verify(voReleaseObject(engine, result));
  verify(voReleaseObject(engine, input));
  verify(voReleaseObject(engine, recognizer));
  verify(voDestroyEngine(engine));

  printf("\n");
  printf("end of example:\n");
  printf(" you just learnt how to explore a RecognitionResult\n");

  // -- finalization -----------------------------------------------------------
  unloadMyScriptEngine();

  return exitCode;
}
