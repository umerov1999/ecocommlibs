/**
 * The `InkSearching` example code shows you how to perform ink searching on
 * digital ink that has already been indexed.
 */


// -- configuration ------------------------------------------------------------

// common configuration parameters
#include "../common/config.h"

// include the MyScript Engine API header(s)
#include <c/MyScriptEngine.h>
#include <c/MyScriptInkSearch.h>

// include the certificate
#include "../certificates/MyCertificate.h"


// -- example body -------------------------------------------------------------

#include <stdio.h>

// global engine (for the sake of readability)
voEngine engine = NULL;

// common utility functions
#include "../common/common.h"

// path of the file where the index is stored.
#define INDEX_FILE OUTPUT_DIR "/InkIndexing-output.index"

// main function
int main()
{
  voIndex index = NULL;
  voQuery query1 = NULL;
  voQuery query2 = NULL;
  voQuery query = NULL;
  voString word1 = {"Vision", -1};
  voString word2 = {"Objects", -1};
  voFinder finder = NULL;
  voFindResult result = NULL;
  voOccurrenceIterator occurrences = NULL;
  int i = 0;

  if (strlen(INDEX_FILE) == 0)
  {
    printf("/!\\ please edit this example program setting the INDEX_FILE variable to a valid file name\n");
    exit(EXIT_FAILURE);
  }
  
  if (!fileExists(INDEX_FILE))
  {
    printf("/!\\ please run StructuredInputInkIndexing first in order to generate the index file which is needed to run this example\n");
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

  printf("2) creating a BinaryQuery (\"Vision OR Objects\")\n");
  query1 = voCreateStringQuery(engine, NULL, &word1, true, true, false);
  verify(query1 != NULL);
  printf(" . \"Vision\" StringQuery created successfully\n");
  query2 = voCreateStringQuery(engine, NULL, &word2, true, true, false);
  verify(query2 != NULL);
  printf(" . \"Objects\" StringQuery created successfully\n");
  query = voCreateBinaryQuery(engine, VO_OrQuery, query1, query2);
  verify(query != NULL);
  printf(" . \"Vision OR Objects\" OrQuery created successfully\n");
  printf("\n");

  printf("3) creating a Finder\n");
  finder = voCreateFinder(engine);
  verify(finder != NULL);
  printf(" . Finder created successfully\n");
  printf("\n");

  printf("4) attaching the OrQuery to the Finder\n");
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, finder, query));
  printf(" . OrQuery attached successfully\n");
  printf("\n");

  printf("5) loading the Index generated from the indexing example\n");
  index = loadEngineObject(INDEX_FILE);
  printf(" . Index loaded successfully\n");
  printf("\n");

  printf("6) attaching the Index to the Finder\n");
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, finder, index));
  printf(" . Index attached successfully\n");
  printf("\n");

  printf("7) running the search process\n");
  verify(VO_IFACE(engine, IRunnable)->run(engine, finder, NULL, NULL));
  printf(" . search process performed successfully\n");
  printf("\n");

  printf("8) retrieving the FindResult\n");
  result = VO_IFACE(engine, IFindResultProvider)->getResult(engine, finder);
  verify(result != NULL);
  printf(" . FindResult retrieved successfully\n");
  printf("\n");

  printf("9) displaying the result\n");
  printf(" . global score: %g\n", VO_IFACE(engine, IFindResult)->getGlobalScore(engine, result));
  occurrences = VO_IFACE(engine, IFindResult)->getOccurrences(engine, result);
  verify(occurrences != NULL);
  for (i = 1; !VO_IFACE(engine, IOccurrenceIterator)->isAtEnd(engine, occurrences); ++i)
  {
    voInputRange inputRange = {NULL, 0};
    verify(VO_IFACE(engine, IOccurrenceIterator)->getInputRange(engine, occurrences, &inputRange));
    inputRange.elements = malloc(sizeof(*inputRange.elements)*inputRange.count);
    verify(VO_IFACE(engine, IOccurrenceIterator)->getInputRange(engine, occurrences, &inputRange));
    printf(" . occurrence %d: score=%g, inputRange=",
      i,
      VO_IFACE(engine, IOccurrenceIterator)->getScore(engine, occurrences));
    printInputRange(inputRange);
    printf(", searchUnit=%d\n",
      (int)VO_IFACE(engine, IOccurrenceIterator)->getSearchUnit(engine, occurrences));
    free(inputRange.elements);
    verify(VO_IFACE(engine, IOccurrenceIterator)->next(engine, occurrences));
  }
  printf("\n");

  printf("10) cleaning-up: releasing all created objects and destroying the Engine\n");
  verify(voReleaseObject(engine, occurrences));
  verify(voReleaseObject(engine, result));
  verify(voReleaseObject(engine, finder));
  verify(voReleaseObject(engine, query));
  verify(voReleaseObject(engine, query2));
  verify(voReleaseObject(engine, query1));
  verify(voReleaseObject(engine, index));
  verify(voDestroyEngine(engine));
  printf(" . clean-up done successfully\n");
  printf("\n");

  printf("end of example:\n");
  printf(" you just learnt how to perform ink searching\n");

  // -- finalization -----------------------------------------------------------
  unloadMyScriptEngine();

  return EXIT_SUCCESS;
}
