/**
 * The `InkSearch` example code shows you how to search into a previously
 * indexed document.
 */


// -- configuration ------------------------------------------------------------

// common configuration parameters
#include "../common/config.h"

// include the MyScript Engine API header(s)
#include <c/MyScriptEngine.h>
#include <c/MyScriptDocument.h>

#include <c/MyScriptInkSearch.h>

// include the certificate
#include "../certificates/MyCertificate.h"


// -- example body -------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>

// global engine (for the sake of readability)
voEngine engine = NULL;

// common utility functions
#include "../common/common.h"

// path to the Document files
// Parts of the document must have been previously indexed,
// please look at the project "09.document.11-TextIndexation".
// ("examples3.notes" is "examples2.notes" with indexation data)
#define TEMPORARY_DIRECTORY OUTPUT_DIR "/tmp"
#define DOCUMENT_FILE       DATA_DIR "/example3.notes"


static void printOccurrence(voEngine engine, voDocumentFindOccurrenceIterator occurrenceIterator)
{
  voString pageId = { NULL, 0 };
  voString inkSegment = { NULL, 0 };
  voString label = { NULL, 0 };
  float score = -1.0f;
  int queryIndex = -1;

  verify(true == VO_IFACE(engine, IDocumentFindOccurrenceIterator)->getPageId(engine, occurrenceIterator, NULL, &pageId));
  pageId.bytes = (char*) malloc(pageId.byteCount + 1);
  verify(true == VO_IFACE(engine, IDocumentFindOccurrenceIterator)->getPageId(engine, occurrenceIterator, NULL, &pageId));
  pageId.bytes[pageId.byteCount] = '\0';

  verify(true == VO_IFACE(engine, IDocumentFindOccurrenceIterator)->getInkSegment(engine, occurrenceIterator, NULL, &inkSegment));
  inkSegment.bytes = (char*) malloc(inkSegment.byteCount + 1);
  verify(true == VO_IFACE(engine, IDocumentFindOccurrenceIterator)->getInkSegment(engine, occurrenceIterator, NULL, &inkSegment));
  inkSegment.bytes[inkSegment.byteCount] = '\0';

  verify(true == VO_IFACE(engine, IDocumentFindOccurrenceIterator)->getLabel(engine, occurrenceIterator, NULL, &label));
  label.bytes = (char*) malloc(label.byteCount + 1);
  verify(true == VO_IFACE(engine, IDocumentFindOccurrenceIterator)->getLabel(engine, occurrenceIterator, NULL, &label));
  label.bytes[label.byteCount] = '\0';

  score = VO_IFACE(engine, IDocumentFindOccurrenceIterator)->getScore(engine, occurrenceIterator);
  verify(score >= 0.0f);

  queryIndex = VO_IFACE(engine, IDocumentFindOccurrenceIterator)->getQueryIndex(engine, occurrenceIterator);
  verify(queryIndex >= 0);

  printf("found \"%s\" (sub query %d), at page \"%s\" - \"%s\", with score %g",
         label.bytes, queryIndex,
         pageId.bytes, inkSegment.bytes,
         score);

  free(label.bytes);
  free(inkSegment.bytes);
  free(pageId.bytes);
}

// the document finder callback
static void VO_MSE_CALL finderCallback(voDocumentFinderEvent eventId, voDocumentFindOccurrenceIterator occurrenceIterator, void* userParam)
{
  int index;

  switch (eventId)
  {
    case VO_DOCUMENT_FINDER_EVENT_START:
      printf("  > Find process started...\n");
      break;

    case VO_DOCUMENT_FINDER_EVENT_NEW_OCCURRENCES:
      printf("  > New occurrence(s) found:\n");
      index = 0;
      while(VO_NO == VO_IFACE(engine, IDocumentFindOccurrenceIterator)->isAtEnd(engine, occurrenceIterator))
      {
        printf("    #%d -- ", index);
        printOccurrence(engine, occurrenceIterator);
        printf("\n");

        verify(VO_IFACE(engine, IDocumentFindOccurrenceIterator)->next(engine, occurrenceIterator));
        ++index;
      }
      break;

    case VO_DOCUMENT_FINDER_EVENT_DONE:
      printf("  > Find process done\n");
      break;

    case VO_DOCUMENT_FINDER_EVENT_CANCELLED:
      printf("  > Find process cancelled\n");
      break;
      
    default:
      break;
  }
}

// main function
int main()
{
  char temporaryDirectoryBuffer[PATH_MAX];
  char documentFileNameBuffer[PATH_MAX];
  voString temporaryDirectory;
  voString documentFileName;

  voDocument document = NULL;
  voQuery query = NULL;
  voQuery subQuery1 = NULL;
  voQuery subQuery2 = NULL;
  voString word1 = voString_INITIALIZER("wish*");
  voString word2 = voString_INITIALIZER("birthday");

  voDocumentFinder finder = NULL;
  voDocumentFindResult result = NULL;
  voDocumentFindOccurrenceIterator occurrenceIterator = NULL;
  int index = 0;

  // -- initialization ---------------------------------------------------------
  if (!loadMyScriptEngine(MYSCRIPT_ENGINE_LIBRARY_DIR, NULL))
    return (fprintf(stderr, "failed to load the MyScriptEngine library\n"), EXIT_FAILURE);

  resolvePath(documentFileNameBuffer, PATH_MAX, DOCUMENT_FILE);
  documentFileName.bytes = documentFileNameBuffer;
  documentFileName.byteCount = strlen(documentFileName.bytes);

  resolvePath(temporaryDirectoryBuffer, PATH_MAX, TEMPORARY_DIRECTORY);
  temporaryDirectory.bytes = temporaryDirectoryBuffer;
  temporaryDirectory.byteCount = strlen(temporaryDirectory.bytes);

  // -- example code -----------------------------------------------------------
  printf("1) creating a new Engine initialized with your certificate\n");
  engine = voCreateEngine(VO_MSE_VER, &myCertificate, NULL);
  verify(engine != NULL);
  printf(" . Engine created successfully\n");
  printf("\n");

  printf("2) loading an existing Document containing indexed texts\n");
  document = voLoadObjectFromFile(engine, NULL, &documentFileName, NULL);
  verify(document != NULL);
  verify(VO_IFACE(engine, IDocument)->setTemporaryDirectory(engine, document, NULL, &temporaryDirectory));
  printf(" . ");
  printPath(stdout, documentFileName.bytes);
  printf(" loaded successfully\n");
  printf("\n");

  printf("3) creating a complex search query\n");
  subQuery1 = voCreateStringQuery(engine, NULL, &word1, true, true, true);
  verify(subQuery1 != NULL);
  printf(" . first subquery created successfully\n");

  subQuery2 = voCreateStringQuery(engine, NULL, &word2, true, true, false);
  verify(subQuery2 != NULL);
  printf(" . second subquery created successfully\n");

  query = voCreateBinaryQuery(engine, VO_OrQuery, subQuery1, subQuery2);
  verify(query != NULL);
  printf(" . complex query created successfully\n");
  printf("\n");

  printf("4) create a document finder\n");
  finder = voCreateDocumentFinder(engine, document, finderCallback, NULL);
  verify(finder != NULL);
  printf(" . document finder created successfully\n");
  printf("\n");

  printf("5) launch the find process (look for 'wish*' and 'birthday') & wait for the result\n");
  verify(VO_IFACE(engine, IDocumentFinder)->process(engine, finder, query));
  result = VO_IFACE(engine, IDocumentFinder)->getResult(engine, finder, true);
  printf("\n");

  printf("6) inspect the result\n");

  occurrenceIterator = VO_IFACE(engine, IDocumentFindResult)->getOccurrences(engine, result);
  verify(occurrenceIterator != NULL);
  index = 0;
  while(VO_NO == VO_IFACE(engine, IDocumentFindOccurrenceIterator)->isAtEnd(engine, occurrenceIterator))
  {
    printf(" . occurence #%d -- ", index);
    printOccurrence(engine, occurrenceIterator);
    printf("\n");

    verify(VO_IFACE(engine, IDocumentFindOccurrenceIterator)->next(engine, occurrenceIterator));
    ++index;
  }
  voReleaseObject(engine, occurrenceIterator);
  printf("\n");

  printf("7) cleaning-up: releasing all created objects and destroying the Engine\n");
  verify(voReleaseObject(engine, result));
  verify(voReleaseObject(engine, subQuery2));
  verify(voReleaseObject(engine, subQuery1));
  verify(voReleaseObject(engine, query));
  verify(voReleaseObject(engine, document));
  verify(voDestroyEngine(engine));
  printf(" . clean-up done successfully\n");
  printf("\n");

  printf("end of example:\n");
  printf(" you just learnt how to search into an indexed document\n");

  // -- finalization -----------------------------------------------------------
  unloadMyScriptEngine();

  return EXIT_SUCCESS;
}
