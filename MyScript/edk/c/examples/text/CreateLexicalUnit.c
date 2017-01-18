/**
 * The `CreateLexicalUnit` example code shows you the basic steps to create a
 * `LexicalUnit` resource, add regular expressions to it and store it on the
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
  voLexicalUnit lexicalUnit = NULL;
  voString name = {NULL, -1};
  voString regex = {NULL, -1};

  // -- library initialization -------------------------------------------------
  if (!loadMyScriptEngine(MYSCRIPT_ENGINE_LIBRARY_DIR, NULL))
    return (fprintf(stderr, "failed to load the MyScriptEngine library\n"), EXIT_FAILURE);

  // -- example code -----------------------------------------------------------
  printf("1) creating a new Engine initialized with your certificate\n");
  engine = voCreateEngine(VO_MSE_VER, &myCertificate, NULL);
  verify(engine != NULL);
  printf(" . Engine created successfully\n");
  printf("\n");
    
  printf("2) creating a new LexicalUnit\n");
  lexicalUnit = voCreateLexicalUnit(engine);
  verify(lexicalUnit != NULL);
  printf(" . LexicalUnit created successfully\n");
  printf(" . right after its creation, the status of the LexicalUnit is:\n");
  printf("   . is the LexicalUnit compiled = %s\n", VO_IFACE(engine, ICompilable)->isCompiled(engine, lexicalUnit) ? "yes" : "no");
  printf("\n");

  printf("3) editing our LexicalUnit in order to model:\n");
  printf("\n");
  printf(" num_month = 0[1-9]|1[0-2]\n");
  printf(" num_day = 0[1-9]|[1-2][0-9]|3[0-1]\n");
  printf(" day_of_birth = {num_day}\n");
  printf(" month_of_birth = {num_month}\n");
  printf(" year_of_birth = [0-9][0-9]\n");
  printf(" extra_number = [0-9]\n");
  printf(" sep = [- /]?\n");
  printf(" id_number = {day_of_birth}{month_of_birth}{year_of_birth}{sep}{extra_number}?\n");
  printf(" {id_number}\n");
  printf("\n");

  name.bytes = "num_month";
  regex.bytes = "0[1-9]|1[0-2]";
  verify(VO_IFACE(engine, ILexicalUnit)->defineRegex(engine, lexicalUnit, NULL, &name, &regex, 0));
  name.bytes = "num_day";
  regex.bytes = "0[1-9]|[1-2][0-9]|3[0-1]";
  verify(VO_IFACE(engine, ILexicalUnit)->defineRegex(engine, lexicalUnit, NULL, &name, &regex, 0));
  name.bytes = "day_of_birth";
  regex.bytes = "{num_day}";
  verify(VO_IFACE(engine, ILexicalUnit)->defineRegex(engine, lexicalUnit, NULL, &name, &regex, 0));
  name.bytes = "month_of_birth";
  regex.bytes = "{num_month}";
  verify(VO_IFACE(engine, ILexicalUnit)->defineRegex(engine, lexicalUnit, NULL, &name, &regex, 0));
  name.bytes = "year_of_birth";
  regex.bytes = "[0-9][0-9]";
  verify(VO_IFACE(engine, ILexicalUnit)->defineRegex(engine, lexicalUnit, NULL, &name, &regex, 0));
  name.bytes = "extra_number";
  regex.bytes = "[0-9]";
  verify(VO_IFACE(engine, ILexicalUnit)->defineRegex(engine, lexicalUnit, NULL, &name, &regex, 0));
  name.bytes = "sep";
  regex.bytes = "[- /]?";
  verify(VO_IFACE(engine, ILexicalUnit)->defineRegex(engine, lexicalUnit, NULL, &name, &regex, 0));
  name.bytes = "id_number";
  regex.bytes = "{day_of_birth}{month_of_birth}{year_of_birth}{sep}{extra_number}?";
  verify(VO_IFACE(engine, ILexicalUnit)->defineRegex(engine, lexicalUnit, NULL, &name, &regex, 0));
  regex.bytes = "{id_number}";
  verify(VO_IFACE(engine, ILexicalUnit)->defineMainRegex(engine, lexicalUnit, NULL, &regex, 0));
  printf(" . edited the LexicalUnit successfully\n");
  printf("\n");

  printf("4) compiling the LexicalUnit (mandatory if you want to store and/or use it):\n");
  verify(VO_IFACE(engine, ICompilable)->compile(engine, lexicalUnit, NULL, NULL));
  printf(" . LexicalUnit compiled successfully\n");
  printf("   . is the LexicalUnit compiled = %s\n", VO_IFACE(engine, ICompilable)->isCompiled(engine, lexicalUnit) ? "yes" : "no");
  printf("\n");

  printf("5) storing the LexicalUnit as a resource file:\n");
  storeEngineObject(lexicalUnit, OUTPUT_DIR "/CreateLexicalUnit-output.res");
  printf(" . LexicalUnit stored successfully to: ");
  printPath(stdout, OUTPUT_DIR "/CreateLexicalUnit-output.res");
  printf("\n");
  printf("\n");

  printf("6) cleaning-up: releasing all created objects and destroying the Engine\n");
  verify(voReleaseObject(engine, lexicalUnit));
  voDestroyEngine(engine);
  printf(" . clean-up done successfully\n");
  printf("\n");

  // -- finalization -----------------------------------------------------------
  unloadMyScriptEngine();

  printf("end of example:\n");
  printf(" you just learnt how to create, edit, store and release a LexicalUnit\n");

  return EXIT_SUCCESS;
}
