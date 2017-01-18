/**
 * The `CreateLexicon` example code shows you the basic steps to create a
 * `Lexicon` resource, add words to it and store it on the hard drive as a
 * resource file.
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

// main function
int main()
{
  voLexicon lexicon = NULL;
  voString word1 = {"Ishtar", -1};
  voString word2 = {"Marduk", -1};
  voString word3 = {"Shamash", -1};
  uint32_t wordCount = 0;
  uint32_t maxWordLength = 0;

  // -- library initialization -------------------------------------------------
  if (!loadMyScriptEngine(MYSCRIPT_ENGINE_LIBRARY_DIR, NULL))
    return (fprintf(stderr, "failed to load the MyScriptEngine library\n"), EXIT_FAILURE);

  // -- example code -----------------------------------------------------------
  printf("1) creating a new Engine initialized with your certificate\n");
  engine = voCreateEngine(VO_MSE_VER, &myCertificate, NULL);
  verify(engine != NULL);
  printf(" . Engine created successfully\n");
  printf("\n");
    
  printf("2) creating a new Lexicon\n");
  lexicon = voCreateLexicon(engine);
  verify(lexicon != NULL);
  printf(" . Lexicon created successfully\n");
  printf(" . right after its creation, the status of the Lexicon is:\n");
  printf("   . is the Lexicon compiled = %s\n", VO_IFACE(engine, ICompilable)->isCompiled(engine, lexicon) ? "yes" : "no");
  verify(voGetProperty(engine, lexicon, VO_WORD_COUNT, &wordCount, sizeof(wordCount)));
  printf("   . number of words in the Lexicon = %u\n", wordCount);
  verify(voGetProperty(engine, lexicon, VO_MAX_WORD_LENGTH, &maxWordLength, sizeof(maxWordLength)));
  printf("   . length of the longest word in the Lexicon = %u\n", maxWordLength);
  printf("\n");

  printf("3) adding 3 words to our Lexicon:\n");
  printf(" . adding \"Ishtar\", \"Marduk\", \"Shamash\", names of Babylonian gods\n");
  verify(VO_IFACE(engine, ILexicon)->addWord(engine, lexicon, NULL, &word1));
  verify(VO_IFACE(engine, ILexicon)->addWord(engine, lexicon, NULL, &word2));
  verify(VO_IFACE(engine, ILexicon)->addWord(engine, lexicon, NULL, &word3));
  printf(" . right after having added words, the status of the Lexicon is:\n");
  verify(voGetProperty(engine, lexicon, VO_WORD_COUNT, &wordCount, sizeof(wordCount)));
  printf("   . number of words in the Lexicon = %u\n", wordCount);
  verify(voGetProperty(engine, lexicon, VO_MAX_WORD_LENGTH, &maxWordLength, sizeof(maxWordLength)));
  printf("   . length of the longest word in the Lexicon = %u\n", maxWordLength);
  printf("\n");

  printf("4) compiling the Lexicon (mandatory if you want to store and/or use it):\n");
  verify(VO_IFACE(engine, ICompilable)->compile(engine, lexicon, NULL, NULL));
  printf(" . Lexicon compiled successfully\n");
  printf("   . is the Lexicon compiled = %s\n", VO_IFACE(engine, ICompilable)->isCompiled(engine, lexicon) ? "yes" : "no");
  printf("\n");

  printf("5) storing the Lexicon as a resource file:\n");
  storeEngineObject(lexicon, OUTPUT_DIR "/CreateLexicon-output.res");
  printf(" . Lexicon stored successfully to: ");
  printPath(stdout, OUTPUT_DIR "/CreateLexicon-output.res");
  printf("\n");
  printf("\n");

  printf("6) cleaning-up: releasing all created objects and destroying the Engine\n");
  verify(voReleaseObject(engine, lexicon));
  voDestroyEngine(engine);
  printf(" . clean-up done successfully\n");
  printf("\n");

  printf("end of example:\n");
  printf(" you just learnt how to create, edit, store and release a Lexicon\n");

  // -- finalization -----------------------------------------------------------
  unloadMyScriptEngine();

  return EXIT_SUCCESS;
}
