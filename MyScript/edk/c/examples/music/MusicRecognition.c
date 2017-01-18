/**
 * @file edk/c/examples/music/MusicRecognition.c
 * 
 *
 * The <code>MusicRecognition</code> example code shows you how to perform
 * recognition on a music score.
 */


// -- configuration ------------------------------------------------------------

// common configuration parameters
#include "../common/config.h"

// include the MyScript Engine API header(s)
#include <c/MyScriptEngine.h>
#include <c/MyScriptJSON.h>
#include <c/MyScriptInk.h>
#include <c/MyScriptMusic.h>

// include the certificate
#include "../certificates/MyCertificate.h"


// -- example body -------------------------------------------------------------

#include <stdio.h>

// global engine (for the sake of readability)
voEngine engine = NULL;

// common utility functions
#include "../common/common.h"

// serialization helper
#include <../src/MyScriptMusic-scoreToMusicXML.h>


// .jink parsing helper
static bool parseInput(const char* path, voMusicDocument document);

// main function
int main(int argc, char** argv)
{
  const char* default_argv[] = { argv[0], DATA_DIR "/music.jink" };
  voMusicRecognizer recognizer = NULL;
  voMusicAlphabetKnowledge ak = NULL;
  voMusicGrammar grm = NULL;
  voMusicDocument document = NULL;

  int i;
  int exitCode = EXIT_SUCCESS;

  if (argc == 1)
  {
    argc = (int)(sizeof(default_argv) / sizeof(default_argv[0]));
    argv = (char**)default_argv;
  }

  // -- initialization ---------------------------------------------------------
  if (!loadMyScriptEngine(MYSCRIPT_ENGINE_LIBRARY_DIR, NULL))
    return (fprintf(stderr, "failed to load the MyScriptEngine library\n"), EXIT_FAILURE);

  // -- example code -----------------------------------------------------------
  printf("1) creating a new Engine initialized with your certificate\n");
  engine = voCreateEngine(VO_MSE_VER, &myCertificate, NULL);
  verify(engine != NULL);
  printf(" . Engine created successfully\n");
  printf("\n");

  printf("2) creating a new MusicRecognizer\n");
  recognizer = voCreateMusicRecognizer(engine);
  verify(recognizer != NULL);
  printf(" . MusicRecognizer created successfully\n");
  printf("\n");

  printf("3) loading and attaching resources to the recognizer\n");
  ak = loadEngineObject(MUSIC_AK_RES);
  printf(" . MusicAlphabetKnowledge loaded successfully\n");
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, recognizer, ak));
  printf(" . MusicAlphabetKnowledge resource successfully attached to the MusicRecognizer\n");
  printf(" . releasing the MusicAlphabetKnowledge resource (not needed anymore)\n");
  verify(voReleaseObject(engine, ak));
  printf(" . MusicAlphabetKnowledge resource released successfully\n");
  grm = loadEngineObject(MUSIC_GRM_RES);
  printf(" . MusicGrammar loaded successfully\n");
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, recognizer, grm));
  printf(" . MusicGrammar resource successfully attached to the MusicRecognizer\n");
  printf(" . releasing the MusicGrammar resource (not needed anymore)\n");
  verify(voReleaseObject(engine, grm));
  printf(" . MusicGrammar resource released successfully\n");
  printf("\n");

  printf("4) creating a new MusicDocument object\n");
  document = voCreateMusicDocument(engine);
  verify(document != NULL);
  printf(" . MusicDocument created successfully\n");
  printf("\n");

  printf("5) processing input files\n");

  for (i = 1; i < argc; ++i)
  {
    voMusicScore score = NULL;
    char* str = NULL;
    int length = 0;

    if (!fileExists(argv[i]))
    {
      fprintf(stderr, "missing ink file: %s\n", argv[i]);
      exitCode = EXIT_FAILURE;
      break;
    }

    printf(" . parsing: ");
    printPath(stdout, argv[i]);
    printf("\n");
    verify(parseInput(argv[i], document));
    printf("   . input parsed successfully\n");
    printf(" . running the music recognition process\n");
    verify(VO_IFACE(engine, IMusicRecognizer)->process(engine, recognizer, document, NULL, NULL));
    printf("   . music recognition performed successfully\n");
    printf(" . retrieving the recognition result\n");
    score = VO_IFACE(engine, IMusicDocument)->getScore(engine, document);
    verify(score != NULL);

    if (score != NULL)
    {
      length = scoreToMusicXML(engine, score, NULL, 0);
      if (length != -1)
      {
        str = malloc(sizeof(char) * (length + 1));

        if (str != NULL)
        {
          FILE* out;

          scoreToMusicXML(engine, score, str, length);

          out = fileOpen(OUTPUT_DIR "/MusicRecognition-output.xml", "w");
          fwrite(str, 1, length, out);
          fclose(out);
          free(str);

          printf("   . score stored as MusicXML: ");
          printPath(stdout, OUTPUT_DIR "/MusicRecognition-output.xml");
          printf("\n");
          printf("\n");
        }
      }
    }
    else
    {
      printf("   . no recognition result\n");
      printf("\n");
    }

    verify(VO_IFACE(engine, IMusicDocument)->clear(engine, document));
    verify(voReleaseObject(engine, score));
  }

  printf("6) cleaning-up: releasing all created objects and destroying the Engine\n");
  verify(voReleaseObject(engine, document));
  verify(voReleaseObject(engine, recognizer));
  verify(voDestroyEngine(engine));
  printf("\n");

  printf("end of example:\n");
  printf(" you just learnt how to perform music recognition\n");

  // -- finalization -----------------------------------------------------------
  unloadMyScriptEngine();

  return exitCode;
}

// -- parseInput helper --------------------------------------------------------

static bool parseInput(const char* path, voMusicDocument document)
{
  voInk ink = NULL;
  voInkTagIterator tagIterator = NULL;
  voJson tagData = NULL;
  voJson value = NULL;
  voInkStroke stroke = NULL;
  bool ok = false;

  for (;;)
  {
    voString s = voString_INITIALIZER("");
    voInkTagQuery query = voInkTagQueryByName_INITIALIZER("STAFF");
    double d;
    int i, count;
    float top, gap;
    voPoint* points = NULL;

    ink = loadEngineObject(path);
    if (ink == NULL)
     break;

    tagIterator = VO_IFACE(engine, IInk)->tagLookup(engine, ink, &query);
    if (tagIterator == NULL)
      break;

    if (VO_IFACE(engine, IInkTagIterator)->isAtEnd(engine, tagIterator) != VO_NO)
      break;

    tagData = VO_IFACE(engine, IInkTagIterator)->getData(engine, tagIterator);
    if (tagData == NULL)
      break;

    s.bytes = "count";
    value = VO_IFACE(engine, IJson)->getObjectEntryValue(engine, tagData, NULL, &s);
    if (value == NULL)
      break;

    if (!VO_IFACE(engine, IJson)->getNumberValue(engine, value, &d))
      break;

    voReleaseObject(engine, value);
    value = NULL;
    count = (int)d;

    s.bytes = "top";
    value = VO_IFACE(engine, IJson)->getObjectEntryValue(engine, tagData, NULL, &s);
    if (value == NULL)
      break;

    if (!VO_IFACE(engine, IJson)->getNumberValue(engine, value, &d))
      break;

    voReleaseObject(engine, value);
    value = NULL;
    top = (float)d;

    s.bytes = "gap";
    value = VO_IFACE(engine, IJson)->getObjectEntryValue(engine, tagData, NULL, &s);
    if (value == NULL)
      break;

    if (!VO_IFACE(engine, IJson)->getNumberValue(engine, value, &d))
      break;

    voReleaseObject(engine, value);
    value = NULL;
    gap = (float)d;

    if (!VO_IFACE(engine, IMusicDocument)->addStaff(engine, document, count, top, gap))
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

        if (!VO_IFACE(engine, IMusicDocument)->addStroke(engine, document, &points->x, sizeof(voPoint), &points->y, sizeof(voPoint), pointCount))
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

        if (!VO_IFACE(engine, IMusicDocument)->addStroke(engine, document, &points->x, sizeof(voPoint), &points->y, sizeof(voPoint), pointCount))
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
  if (value != NULL)
    voReleaseObject(engine, value);
  if (value != tagData)
    voReleaseObject(engine, tagData);
  if (tagIterator != NULL)
    voReleaseObject(engine, tagIterator);
  if (ink != NULL)
    voReleaseObject(engine, ink);

  return ok;
}
