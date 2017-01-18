/**
 * The `CreateArchive` example code shows you the basic steps to create an
 * `Archive` resource, attach other resources to it and then store it on the
 * hard drive as a resource file.
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
  voArchive archive = NULL;
  voLexicon lexicon = NULL;
  voString word = {"word", -1};
  voSubsetKnowledge sk = NULL;
  voString string = {"abcdefghijklmnopqrstuvwxyz", -1};

  // -- initialization ---------------------------------------------------------
  if (!loadMyScriptEngine(MYSCRIPT_ENGINE_LIBRARY_DIR, NULL))
    return (fprintf(stderr, "failed to load the MyScriptEngine library\n"), EXIT_FAILURE);
  
  // -- example code -----------------------------------------------------------
  printf("1) creating a new Engine initialized with your certificate\n");
  engine = voCreateEngine(VO_MSE_VER, &myCertificate, NULL);
  verify(engine != NULL);
  printf(" . Engine created successfully\n");
  printf("\n");
    
  printf("2) creating a new Archive\n");
  archive = voCreateArchive(engine);
  verify(archive != NULL);
  printf(" . Archive created successfully\n");
  printf(" . right after its creation, the status of the Archive is:\n");
  printf("   . number of resources in the Archive = %d\n", VO_IFACE(engine, IAttachTarget)->getAttachedCount(engine, archive));
  printf("\n");

  printf("3) creating a Lexicon and a SubsetKnowledge we will attach to the Archive \n");
  lexicon = voCreateLexicon(engine);
  verify(lexicon != NULL);
  verify(VO_IFACE(engine, ILexicon)->addWord(engine, lexicon, NULL, &word));
  verify(VO_IFACE(engine, ICompilable)->compile(engine, lexicon, NULL, NULL));
  printf(" . Lexicon created successfully\n");
  sk = voCreateSubsetKnowledge(engine);
  verify(sk != NULL);
  verify(VO_IFACE(engine, ISubsetKnowledge)->disableAllCharacters(engine, sk));
  verify(VO_IFACE(engine, ISubsetKnowledge)->enableCharacters(engine, sk, NULL, &string));
  printf(" . SubsetKnowledge created successfully\n");
  printf("\n");

  printf("4) attaching the Lexicon and SubsetKnowledge to the Archive\n");
  printf(" . attaching the Lexicon to the Archive\n");
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, archive, lexicon));
  printf(" . Lexicon attached successfully\n");

  printf(" . attaching the SubsetKnowledge to the Archive\n");
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, archive, sk));
  printf(" . SubsetKnowledge attached successfully\n");

  printf(" . releasing the Lexicon (not needed anymore because we attached it)\n");
  verify(voReleaseObject(engine, lexicon));
  printf(" . Lexicon released successfully\n");

  printf(" . releasing the SubsetKnowledge (not needed anymore because we attached it)\n");
  verify(voReleaseObject(engine, sk));
  printf(" . SubsetKnowledge released successfully\n");

  printf(" . right after having attached the resources, the status of the Archive is:\n");
  printf("   . number of resources in the Archive = %d\n", VO_IFACE(engine, IAttachTarget)->getAttachedCount(engine, archive));
  printf("\n");

  printf("5) storing the Archive as a resource file:\n");
  storeEngineObject(archive, OUTPUT_DIR "/CreateArchive-output.res");
  printf(" . Archive stored successfully to: ");
  printPath(stdout, OUTPUT_DIR "/CreateArchive-output.res");
  printf("\n");
  printf("\n");
  
  printf("6) cleaning-up: releasing all created objects and destroying the Engine\n");
  verify(voReleaseObject(engine, archive));
  verify(voDestroyEngine(engine));
  printf(" . clean-up done successfully\n");
  printf("\n");

  printf("end of example:\n");
  printf(" you just learnt how to create, edit, store and release an Archive\n");

  // -- finalization -----------------------------------------------------------
  unloadMyScriptEngine();

  return EXIT_SUCCESS;
}
