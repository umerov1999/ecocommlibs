/**
 * The `CreateDestroyEngine` example code shows you the basic steps to create
 * and destroy of a MyScript `Engine`, properly initialized with a certificate
 * that identifies you as a legitimate MyScript user.
 */


// -- configuration ------------------------------------------------------------

// common configuration parameters
#include "../common/config.h"

// include the MyScript Engine API header(s)
#include <c/MyScriptEngine.h>

// include the certificate
#include "../certificates/MyCertificate.h"


// -- example body -------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>

// global engine (for the sake of readability)
voEngine engine = NULL;

// common utility functions
#include "../common/common.h"

// main function
int main()
{
  voMemoryStatistics memoryStatistics = {0};

  // -- initialization ---------------------------------------------------------
  if (!loadMyScriptEngine(MYSCRIPT_ENGINE_LIBRARY_DIR, NULL))
    return (fprintf(stderr, "failed to load the MyScriptEngine library\n"), EXIT_FAILURE);

  // -- example code -----------------------------------------------------------
  printf("1) creating a new Engine initialized with your certificate\n");
  engine = voCreateEngine(VO_MSE_VER, &myCertificate, NULL);
  verify(engine != NULL);
  printf(" . Engine created successfully\n");
  printf("\n");

  printf("2) on an Engine, you can perform various operations\n");
  printf(" . gathering global memory statistics:\n");
  verify(voGetTypeProperty(engine, VO_Engine, VO_MEM_STATS, &memoryStatistics, sizeof(memoryStatistics)));
  printf("   . current allocation count = " SIZET_FMT "\n", memoryStatistics.curAllocCount);
  printf("   . total allocation count = " SIZET_FMT "\n", memoryStatistics.totalAllocCount);
  printf("   . current memory usage in bytes = " SIZET_FMT "\n", memoryStatistics.curMemoryUsage);
  printf("   . maximum memory usage in bytes = " SIZET_FMT "\n", memoryStatistics.maxMemoryUsage);
  printf("\n");

  printf("3) destroying it!\n");
  verify(voDestroyEngine(engine));
  printf(" . Engine destroyed successfully\n");
  printf("\n");

  printf("end of example:\n");
  printf(" you just learnt how to create and destroy an Engine\n");

  // -- finalization -----------------------------------------------------------
  unloadMyScriptEngine();

  return EXIT_SUCCESS;
}
