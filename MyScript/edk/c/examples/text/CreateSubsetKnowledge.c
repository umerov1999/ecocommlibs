/**
 * The `CreateSubsetKnowledge` example code shows you the basic steps to create
 * a `SubsetKnowledge` resource that constrains enables you to specify the
 * characters a handwriting recognizer is allowed to recognize.
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
  voSubsetKnowledge sk = NULL;
  voString string = voString_INITIALIZER("abcdefghijklmnopqrstuvwxyz");

  // -- library initialization -------------------------------------------------
  if (!loadMyScriptEngine(MYSCRIPT_ENGINE_LIBRARY_DIR, NULL))
    return (fprintf(stderr, "failed to load the MyScriptEngine library\n"), EXIT_FAILURE);

  // -- example code -----------------------------------------------------------
  printf("1) creating a new Engine initialized with your certificate\n");
  engine = voCreateEngine(VO_MSE_VER, &myCertificate, NULL);
  verify(engine != NULL);
  printf(" . Engine created successfully\n");
  printf("\n");
    
  printf("2) creating a new SubsetKnowledge\n");
  sk = voCreateSubsetKnowledge(engine);
  verify(sk != NULL);
  printf(" . SubsetKnowledge created successfully\n");
  printf("\n");

  printf("3) disabling all characters in the SubsetKnowledge\n");
  verify(VO_IFACE(engine, ISubsetKnowledge)->disableAllCharacters(engine, sk));
  printf(" . all characters disabled successfully\n");
  printf("\n");

  printf("4) enabling \"abcdefghijklmnopqrstuvwxyz\" characters\n");
  verify(VO_IFACE(engine, ISubsetKnowledge)->enableCharacters(engine, sk, NULL, &string));
  printf(" . characters enabled successfully\n");
  printf("\n");

  printf("5) storing the SubsetKnowledge as a resource file:\n");
  storeEngineObject(sk, OUTPUT_DIR "/CreateSubsetKnowledge-output.res");
  printf(" . SubsetKnowldedge stored successfully to: ");
  printPath(stdout, OUTPUT_DIR "/CreateSubsetKnowledge-output.res");
  printf("\n");
  printf("\n");

  printf("6) cleaning-up: releasing all created objects and destroying the Engine\n");
  verify(voReleaseObject(engine, sk));
  voDestroyEngine(engine);
  printf(" . clean-up done successfully\n");
  printf("\n");

  printf("end of example:\n");
  printf(" you just learnt how to create, edit, store release a SubsetKnowledge\n");

  // -- finalization -----------------------------------------------------------
  unloadMyScriptEngine();

  return EXIT_SUCCESS;
}
