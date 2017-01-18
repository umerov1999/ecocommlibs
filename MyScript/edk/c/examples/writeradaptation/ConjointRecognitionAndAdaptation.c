/**
 * The `ConjointRecognitionAndAdaptation` example shows you how how to perform
 * conjoint handwriting recognition and adaptation.
 */


// -- configuration ------------------------------------------------------------

// common configuration parameters
#include "../common/config.h"

// ink parsing
#include "../common/parseInk.h"

// include the MyScript Engine API header(s)
#include <c/MyScriptEngine.h>
#include <c/MyScriptText.h>
#include <c/MyScriptWriterAdaptation.h>

// include the certificate
#include "../certificates/MyCertificate.h"


// -- example body -------------------------------------------------------------

#include <stdio.h>

// global engine (for the sake of readability)
voEngine engine = NULL;

// common utility functions
#include "../common/common.h"

// path to the file that represents the writer adaptation profile
#define PROFILE_FILE OUTPUT_DIR "/ConjointRecognitionAndAdaptation-profile.res"


// -- parseInk() callback ------------------------------------------------------

struct AppContext
{
  voEngine engine;
  voFloatStructuredInput input;
};

static int parseInkCallback(const float* x, size_t xByteStride,
                            const float* y, size_t yByteStride,
                            int pointCount,
                            void* userParam)
{
  bool successful;
  struct AppContext* context = (struct AppContext*)userParam;

  successful = VO_IFACE(engine, IFloatStructuredInput)->addStroke(context->engine, context->input, x, xByteStride, y, yByteStride, pointCount);

  return successful ? 0 : voGetError(context->engine);
}


// -- main function ------------------------------------------------------------

int main()
{
  voWriterAdaptationProfile profile = NULL;
  voWriterAdaptationCollector collector = NULL;
  voWriterAdaptationTrainer trainer = NULL;
  voStructuredInputRecognizer recognizer = NULL;
  voInput input = NULL;
  voResource ak = NULL;
  voResource lk = NULL;
  voResource lkEnglishSecondary = NULL;
  TextUIProgressBar progressBar = { 0 };

  // -- check required settings have been edited -------------------------------
  if (strlen(LANG) == 0)
  {
    fprintf(stderr, "/!\\ please edit the common/config.h file and set:\n");
    fprintf(stderr, " . the LANG variable\n");
    exit(EXIT_FAILURE);
  }

  if (strlen(PROFILE_FILE) == 0)
  {
    printf("/!\\ please edit this example program setting the PROFILE_FILE variable to a valid file name\n");
    exit(EXIT_FAILURE);
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

  printf("2) setting up a WriterAdaptationProfile\n");
  if (fileExists(PROFILE_FILE))
  {
    printf(" . found existing profile, loading it\n");
    profile = loadEngineObject(PROFILE_FILE);
  }
  else
  {
    printf(" . no existing profile found, creating it\n");
    profile = voCreateObject(engine, VO_WriterAdaptationProfile);
  }
  verify(profile != NULL);
  printf(" . profile set up successfully\n");
  printf("\n");

  printf("3) creating and configuring a new StructuredInputRecognizer\n");
  recognizer = voCreateStructuredInputRecognizer(engine);
  verify(recognizer != NULL);
  printf(" . recognizer created successfully\n");
  ak = loadEngineObject(AK_RES);
  printf(" . AlphabetKnowledge resource loaded successfully\n");
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, recognizer, ak));
  printf(" . AlphabetKnowledge resource attached successfully to the recognizer\n");
  lk = loadEngineObject(LK_TEXT_RES);
  printf(" . Main LinguisticKnowledge resource loaded successfully\n");
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, recognizer, lk));
  printf(" . Main LinguisticKnowledge resource attached successfully\n");
  if (strstr(LANG, "en") == NULL)
  {
    const char* path;
    if (strstr(LANG, "zh") != NULL || strstr(LANG, "ja") != NULL || strstr(LANG, "ko") != NULL)
      path = LK_ENGLISH_SECONDARY_2_RES;
    else if (strstr(LANG, "hi") != NULL)
      path = LK_ENGLISH_SECONDARY_3_RES;
    else
      path = LK_ENGLISH_SECONDARY_1_RES;

    lkEnglishSecondary = loadEngineObject(path);
    printf(" . Secondary English LinguisticKnowledge resource loaded successfully\n");
    verify(VO_IFACE(engine, IAttachTarget)->attach(engine, recognizer, lkEnglishSecondary));
    printf(" . Secondary English LinguisticKnowledge resource attached successfully\n");
  }
  printf("\n");

  printf("4) creating a new WriterAdaptationCollector\n");
  collector = voCreateObject(engine, VO_WriterAdaptationCollector);
  verify(collector != NULL);
  printf(" . collector created successfully\n");
  printf("\n");

  printf("5) attaching the profile to the collector\n");
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, collector, profile));
  printf(" . profile attached successfully to the collector\n");
  printf("   . the collector will store writer data into the profile for further training\n");
  printf("\n");

  printf("6) attaching the collector to the recognizer\n");
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, recognizer, collector));
  printf(" . collector attached successfully to the recognizer\n");
  printf("   . the recognizer will transfer incoming writer data to the collector\n");
  printf("\n");

  printf("7) attaching the profile to the recognizer\n");
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, recognizer, profile));
  printf(" . profile attached successfully to the recognizer\n");
  printf("   . the recognizer will use existing writer data to adapt the recognition process\n");
  printf("\n");

  printf("8) sending digital ink to the recognizer\n");
  input = voCreateFloatStructuredInput(engine);
  verify(input != NULL);
  {
    struct AppContext context = { engine, input };
    FILE* file = fileOpen(DATA_DIR "/sentence.ink", "r");
    if (file != NULL)
    {
      verify(parseInk(file, parseInkCallback, &context) != EXIT_FAILURE);
      fclose(file);
    }
  }
  verify(VO_IFACE(engine, IInputConsumer)->setSource(engine, recognizer, input));
  printf(" . digital ink successfully sent to the recognizer\n");
  printf("\n");

  printf("9) running the conjoint handwriting recognition/adaptation process\n");
  initializeTextUIProgressBar(&progressBar);
  verify(VO_IFACE(engine, IRunnable)->run(engine, recognizer, updateTextUIProgressBar, &progressBar));
  finalizeTextUIProgressBar(&progressBar);
  printf(" . handwriting recognition/adaptation performed successfully\n");
  printf("\n");

  printf("10) committing the collected data\n");
  verify(VO_IFACE(engine, IWriterAdaptationCollector)->commit(engine, collector));
  printf(" . collected data committed successfully\n");
  printf("\n");

  printf("11) creating and configuring a new WriterAdaptationTrainer\n");
  trainer = voCreateObject(engine, VO_WriterAdaptationTrainer);
  verify(trainer != NULL);
  printf(" . trainer created successfully\n");
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, trainer, ak));
  printf(" . AlphabetKnowledge resource attached successfully to the trainer\n");
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, trainer, profile));
  printf(" . profile attached successfully to the trainer\n");
  printf(" . trainer created and configured successfully\n");
  printf("\n");

  printf("12) running the training process on the collected data\n");
  initializeTextUIProgressBar(&progressBar);
  verify(VO_IFACE(engine, IRunnable)->run(engine, trainer, updateTextUIProgressBar, &progressBar));
  finalizeTextUIProgressBar(&progressBar);
  printf("   . the trainer updated the collected writer data; next recognition will adjust itself to the writer writing style\n");
  printf("\n");

  printf("13) storing the profile to disk\n");
  storeEngineObject(profile, PROFILE_FILE);
  printf(" . profile stored successfully to: ");
  printPath(stdout, PROFILE_FILE);
  printf("\n");
  printf("\n");

  printf("14) cleaning-up: releasing all created objects and destroying the Engine\n");
  verify(voReleaseObject(engine, input));
  verify(voReleaseObject(engine, trainer));
  verify(voReleaseObject(engine, collector));
  if (lkEnglishSecondary != NULL)
    verify(voReleaseObject(engine, lkEnglishSecondary));
  verify(voReleaseObject(engine, lk));
  verify(voReleaseObject(engine, ak));
  verify(voReleaseObject(engine, recognizer));
  verify(voReleaseObject(engine, profile));
  verify(voDestroyEngine(engine));
  printf(" . clean-up done successfully\n");
  printf("\n");

  printf("end of example:\n");
  printf(" you just learnt how to perform writer adaptation\n");

  // -- finalization -----------------------------------------------------------
  unloadMyScriptEngine();

  return EXIT_SUCCESS;
}
