/**
 * The `UseTags` example code shows you how to use tags within ink.
 */


// -- configuration ------------------------------------------------------------

// common configuration parameters
#include "../common/config.h"

// include the MyScript Engine API header(s)
#include <c/MyScriptEngine.h>
#include <c/MyScriptInk.h>
#include <c/MyScriptJSON.h>

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

#define BUFFER_SIZE 200

// main function
int main(void)
{
  voInk ink;
  int tagCount;
  voRectangle rectangle;
  voInkSelection selection;
  int strokeCount;
  float overlapRatio;
  voInkTagQuery tagQuery = voInkTagQuery_EMPTY_INITIALIZER;
  voInkTagIterator tagIterator;
  voString string_chinese = voString_INITIALIZER("chinese_word");
  int64_t tagId;
  char stringBuffer[BUFFER_SIZE];
  voString string = { stringBuffer, BUFFER_SIZE };
  voEngineObject tagData;
  voJsonObjectIterator jsonIterator;

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
  tagCount = VO_IFACE(engine, IInk)->tagLookupCount(engine, ink, &tagQuery);
  verify(tagCount > -1);
  printf(" . the Ink contains %d tags\n", tagCount);
  printf("\n");

  printf("4) selecting a portion of Ink\n");
  selection = voCreateInkSelection(engine, ink);
  verify(selection != NULL);
  // select a portion of ink
  rectangle.x = 230;
  rectangle.y = 300;
  rectangle.width = 50;
  rectangle.height = 50;
  verify(VO_IFACE(engine, ISelection)->selectRectangle(engine, selection, &rectangle, VO_SELECT_SET));
  strokeCount = VO_IFACE(engine, IInkStrokeSet)->getStrokeCount(engine, selection);
  printf(" . selection created with %d strokes\n", strokeCount);
  printf("\n");

  printf("5) adjusting the selection to tag boundaries\n");
  overlapRatio = 0.3f;
  verify(VO_IFACE(engine, IInkSelection)->adjustToTagBoundaries(engine, selection, NULL, &string_chinese, overlapRatio));
  strokeCount = VO_IFACE(engine, IInkStrokeSet)->getStrokeCount(engine, selection);
  printf(" . the adjusted selection now contains %d strokes\n", strokeCount);
  printf("\n");

  printf("6) inspecting the tags intersecting with this selection\n");
  tagQuery.intervals = selection;
  tagQuery.mode = VO_INKINTERVALS_MATCH_INTERSECT;
  tagCount = VO_IFACE(engine, IInk)->tagLookupCount(engine, ink, &tagQuery);
  printf(" . the adjusted selection intersects with %d tags:\n", tagCount);
  tagIterator = VO_IFACE(engine, IInk)->tagLookup(engine, ink, &tagQuery);
  verify(tagIterator != NULL);
  while(VO_NO == VO_IFACE(engine, IInkTagIterator)->isAtEnd(engine, tagIterator))
  {
    tagId = VO_IFACE(engine, IInkTagIterator)->getId(engine, tagIterator);
    string.byteCount = BUFFER_SIZE;
    verify(VO_IFACE(engine, IInkTagIterator)->getName(engine, tagIterator, NULL, &string));
    verify(string.byteCount < BUFFER_SIZE);
    string.bytes[string.byteCount] = '\0';
    printf("     tag id = %lld, tag name = \"%s\"\n", tagId, string.bytes);
    verify(VO_IFACE(engine, IInkTagIterator)->next(engine, tagIterator));
  }
  verify(voReleaseObject(engine, tagIterator));
  printf("\n");

  tagQuery.intervals = NULL;
  tagQuery.id = 1;
  printf("7) selecting the tag with id %lld\n", tagQuery.id);
  tagIterator = VO_IFACE(engine, IInk)->tagLookup(engine, ink, &tagQuery);
  verify(tagIterator != NULL);
  verify(VO_NO == VO_IFACE(engine, IInkTagIterator)->isAtEnd(engine, tagIterator));
  string.byteCount = BUFFER_SIZE;
  verify(VO_IFACE(engine, IInkTagIterator)->getName(engine, tagIterator, NULL, &string));
  verify(string.byteCount < BUFFER_SIZE);
  string.bytes[string.byteCount] = '\0';
  printf(" . the name of this tag is \"%s\"\n", string.bytes);
  tagData = VO_IFACE(engine, IInkTagIterator)->getData(engine, tagIterator);
  verify(tagData != NULL);
  verify(VO_Json == voGetType(engine, tagData));
  printf(" . the tag has associated data of type JSON\n");
  verify(VO_IFACE(engine, IInkTagIterator)->next(engine, tagIterator));
  verify(VO_YES == VO_IFACE(engine, IInkTagIterator)->isAtEnd(engine, tagIterator));
  verify(voReleaseObject(engine, tagIterator));
  printf("\n");

  printf("8) inspecting JSON data associated with tag %lld\n", tagQuery.id);
  jsonIterator = VO_IFACE(engine, IJson)->getObjectEntries(engine, tagData);
  verify(jsonIterator != NULL);
  printf(" . the JSON data is a JSON object which contains the following keys:\n");
  while(VO_NO == VO_IFACE(engine, IJsonObjectIterator)->isAtEnd(engine, jsonIterator))
  {
    string.byteCount = BUFFER_SIZE;
    verify(VO_IFACE(engine, IJsonObjectIterator)->getKey(engine, jsonIterator, NULL, &string));
    verify(string.byteCount < BUFFER_SIZE);
    string.bytes[string.byteCount] = '\0';
    printf("     key = \"%s\"\n", string.bytes);
    verify(VO_IFACE(engine, IJsonObjectIterator)->next(engine, jsonIterator));
  }
  verify(voReleaseObject(engine, jsonIterator));
  verify(voReleaseObject(engine, tagData));
  printf("\n");

  printf("9) erasing the selection\n");
  verify(VO_IFACE(engine, IInk)->erase(engine, ink, selection));
  tagQuery.id = -1;
  tagCount = VO_IFACE(engine, IInk)->tagLookupCount(engine, ink, &tagQuery);
  verify(tagCount > -1);
  printf(" . the Ink contains %d tags after a selection removal\n", tagCount);
  printf("\n");

  tagQuery.id = 1;
  printf("10) erasing tag with id %lld\n", tagQuery.id);
  tagIterator = VO_IFACE(engine, IInk)->tagLookup(engine, ink, &tagQuery);
  verify(tagIterator != NULL);
  verify(VO_NO == VO_IFACE(engine, IInkTagIterator)->isAtEnd(engine, tagIterator));
  verify(VO_IFACE(engine, IInkTagIterator)->remove(engine, tagIterator));
  verify(VO_IFACE(engine, IInkTagIterator)->next(engine, tagIterator));
  verify(VO_YES == VO_IFACE(engine, IInkTagIterator)->isAtEnd(engine, tagIterator));
  verify(voReleaseObject(engine, tagIterator));
  tagQuery.id = -1;
  tagCount = VO_IFACE(engine, IInk)->tagLookupCount(engine, ink, &tagQuery);
  verify(tagCount > -1);
  printf(" . the Ink contains %d tags after tag removal\n", tagCount);
  printf("\n");

  printf("11) cleaning-up: releasing all created objects and destroying the Engine\n");
  verify(voReleaseObject(engine, selection));
  verify(voReleaseObject(engine, ink));
  verify(voDestroyEngine(engine));
  printf(" . clean-up done successfully\n");
  printf("\n");

  printf("end of example:\n");
  printf(" you just learnt how to use tags within ink\n");

  // -- finalization -----------------------------------------------------------
  unloadMyScriptEngine();

  return EXIT_SUCCESS;
}
