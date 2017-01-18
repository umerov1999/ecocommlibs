/**
 * The `IsolatedRecognition` example code shows you how to perform handwriting
 * recognition on isolated characters digital ink.
 */


// -- configuration ------------------------------------------------------------

// common configuration parameters
#include "../common/config.h"

// include the MyScript Engine API header(s)
#include <c/MyScriptEngine.h>
#include <c/MyScriptText.h>
#include <c/MyScriptJSON.h>
#include <c/MyScriptInk.h>

// include the certificate
#include "../certificates/MyCertificate.h"


// -- example body -------------------------------------------------------------

#include <stdio.h>

// global engine (for the sake of readability)
voEngine engine = NULL;

// common utility functions
#include "../common/common.h"

/**
 * Represents a form field.
 */
typedef struct _FormField
{
  float x0;         // x position of the first cell
  float y0;         // y position of the first cell
  float cellWidth;  // cell width
  float cellHeight; // cell height
  float padding;    // padding
  int rowCount;     // number of rows in the form
  int columnCount;  // number of columns in the form
}
FormField;

// ink parsing helper
static bool parseInput(const char* path, voFloatStructuredInput input, const FormField* formField);

// main function
int main(int argc, char** argv)
{
  const char* default_argv[] = { argv[0], DATA_DIR "/hello_world_isolated.ink" };
  FormField formField;
  voStructuredInputRecognizer recognizer = NULL;
  voResource akIsolated = NULL;
  voResource lkText = NULL;
  voResource lkEnglishSecondary = NULL;
  voInput input = NULL;

  int exitCode = EXIT_SUCCESS;
  int i;

  // -- check arguments --------------------------------------------------------
  if (argc == 1)
  {
    argc = (int)(sizeof(default_argv) / sizeof(default_argv[0]));
    argv = (char**)default_argv;
  }

  // -- check required settings have been edited -------------------------------
  // e.g. set the LANG variable to "en_US"
  if (strlen(LANG) == 0)
  {
    fprintf(stderr, "/!\\ please edit the common/config.h file and set:\n");
    fprintf(stderr, " . the LANG variable\n");
    fflush(stderr);
    exit(EXIT_FAILURE);
  }

  // -- library initialization -------------------------------------------------
  if (!loadMyScriptEngine(MYSCRIPT_ENGINE_LIBRARY_DIR, NULL))
    return (fprintf(stderr, "failed to load the MyScriptEngine library\n"), EXIT_FAILURE);

  // -- example code -----------------------------------------------------------

  // -- initialize the form field
  formField.x0 = 210.0f;
  formField.y0 = 425.0f;
  formField.cellWidth = 50.0f;
  formField.cellHeight = 60.0f;
  formField.padding = 5.0f;
  formField.columnCount = 12;
  formField.rowCount = 1;

  printf("1) creating a new Engine initialized with your certificate\n");
  engine = voCreateEngine(VO_MSE_VER, &myCertificate, NULL);
  verify(engine != NULL);
  printf(" . Engine created successfully\n");
  printf("\n");

  printf("2) creating a new StructuredInputRecognizer\n");
  recognizer = voCreateStructuredInputRecognizer(engine);
  verify(recognizer != NULL);
  printf(" . StructuredInputRecognizer created successfully\n");
  printf("\n");

  printf("3) loading and attaching resources to the recognizer\n");
  akIsolated = loadEngineObject(AK_ISO_RES);
  printf(" . IsolatedAlphabetKnowledge resource loaded successfully\n");
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, recognizer, akIsolated));
  printf(" . IsolatedAlphabetKnowledge resource attached successfully\n");
  lkText = loadEngineObject(LK_TEXT_RES);
  printf(" . Main LinguisticKnowledge resource loaded successfully\n");
  verify(VO_IFACE(engine, IAttachTarget)->attach(engine, recognizer, lkText));
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

  printf("4) creating a FloatStructuredInput\n");
  input = voCreateFloatStructuredInput(engine);
  verify(input != NULL);
  printf(" . FloatStructuredInput created successfully\n");
  printf("\n");

  printf("5) setting the input as the source of the recognizer\n");
  verify(VO_IFACE(engine, IInputConsumer)->setSource(engine, recognizer, input));
  printf(" . input successfully set as the recognizer's source\n");
  printf("\n");

  printf("6) processing input files\n");

  for (i = 1; i < argc; ++i)
  {
    voRecognitionResult result = NULL;
    voCandidateIterator iterator = NULL;

    if (!fileExists(argv[i]))
    {
      fprintf(stderr, "missing ink file: %s\n", argv[i]);
      exitCode = EXIT_FAILURE;
      break;
    }
   
    printf(" . parsing input ink ");
    printPath(stdout, argv[i]);
    printf("\n");
    verify(parseInput(argv[i], input, &formField));

    printf("   . input fueled successfully\n");
    printf("\n");

    printf(" . running the handwriting recognition process\n");
    verify(VO_IFACE(engine, IRunnable)->run(engine, recognizer, NULL, NULL));
    printf("   . handwriting recognition performed successfully\n");
    printf("\n");

    printf(" . retrieving the RecognitionResult\n");
    result = VO_IFACE(engine, IRecognitionResultProvider)->getResult(engine, recognizer);
    verify(result != NULL);
    printf("   . RecognitionResult retrieved successfully\n");
    printf("\n");

    printf(" . retrieving the first candidate of the RecognitionResult\n");
    iterator = VO_IFACE(engine, IRecognitionResult)->getCandidates(engine, result);
    verify(iterator != NULL);
    if (VO_IFACE(engine, IIterator)->isAtEnd(engine, iterator))
      printf("   . sorry, unable to recognize the input\n");
    else
    {
      voString label = {NULL, 0};
      verify(VO_IFACE(engine, ICandidateIterator)->getLabel(engine, iterator, NULL, &label));
      label.bytes = malloc(label.byteCount + 1);
      verify(VO_IFACE(engine, ICandidateIterator)->getLabel(engine, iterator, NULL, &label));
      label.bytes[label.byteCount] = '\0';
      printf("   . input recognized as: \"%s\"\n", label.bytes);
      free(label.bytes);
    }
    printf("\n");

    verify(voReleaseObject(engine, iterator));
    verify(voReleaseObject(engine, result));

    verify(VO_IFACE(engine, IFloatStructuredInput)->clear(engine, input, false));

  } // end of: for (i = 1; i < argc; ++i)

  printf("7) cleaning-up: releasing all created objects and destroying the Engine\n");
  verify(voReleaseObject(engine, input));
  if (lkEnglishSecondary != NULL)
    verify(voReleaseObject(engine, lkEnglishSecondary));
  verify(voReleaseObject(engine, lkText));
  verify(voReleaseObject(engine, akIsolated));
  verify(voReleaseObject(engine, recognizer));
  verify(voDestroyEngine(engine));
  printf(" . clean-up done successfully\n");
  printf("\n");

  printf("end of example:\n");
  printf(" you just learnt how to perform handwriting recognition on isolated digital ink\n");

  // -- finalization -----------------------------------------------------------
  unloadMyScriptEngine();

  return exitCode;
}


// -- parseInput helper --------------------------------------------------------

static bool parseInput(const char* path, voFloatStructuredInput input, const FormField* formField)
{
  voInk ink = NULL;
  voInkSelection selection;
  voInkStroke stroke = NULL;
  voRectangle activeBox, guideBox;
  float x0, y0, cellWidth, cellHeight, padding;
  int i, j, k, count, rowCount, columnCount;
  voPoint* points = NULL;
  bool ok = false;

  for (;;)
  {
    x0 = formField->x0;
    y0 = formField->y0;
    cellWidth = formField->cellWidth;
    cellHeight = formField->cellHeight;
    padding = formField->padding;
    rowCount = formField->rowCount;
    columnCount = formField->columnCount;

    ink = loadEngineObject(path);
    if (ink == NULL)
      break;

    for (j = 0; j < rowCount; ++j)
    {
      for (k = 0; k < columnCount; ++k)
      {
        // create a rectangular selection
        selection = voCreateInkSelection(engine, ink);
        if (selection == NULL)
          break;

        // set the active box coordinates
        activeBox.x = x0 + cellWidth * k;
        activeBox.y = y0 + cellHeight * j;
        activeBox.width = cellWidth;
        activeBox.height = cellHeight;

        // select ink in a character field
        if (!VO_IFACE(engine, ISelection)->selectRectangle(engine, selection, &activeBox, VO_SELECT_SET))
          break;
        
        // make sure entire strokes are selected
        if (!VO_IFACE(engine, IInkSelection)->adjustToStrokeBoundaries(engine, selection, 0.5f))
          break;

        // check that we selected something
        count = VO_IFACE(engine, IInkStrokeSet)->getStrokeCount(engine, selection);
        if (count == -1)
          break;

        // start input unit and send ink 
        if (!VO_IFACE(engine, IFloatStructuredInput)->startInputUnit(engine, input, VO_CHAR))
          break;
        
        // set the guide box from the active box, applying the padding
        guideBox = activeBox;
        guideBox.x += padding;
        guideBox.y += padding;
        guideBox.width -= 2.0f * padding;
        guideBox.height -= 2.0f * padding;
        
        if (!VO_IFACE(engine, IFloatStructuredInput)->setGuideBox(engine, input, guideBox.x, guideBox.y, guideBox.width, guideBox.height))
          break;

        for (i = 0; i < count; ++i)
        {
          int pointCount;

          stroke = VO_IFACE(engine, IInkStrokeSet)->getStrokeAt(engine, selection, i);
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

            if (!VO_IFACE(engine, IFloatStructuredInput)->addStroke(engine, input, &points->x, sizeof(voPoint), &points->y, sizeof(voPoint), pointCount))
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

            if (!VO_IFACE(engine, IFloatStructuredInput)->addStroke(engine, input, &points->x, sizeof(voPoint), &points->y, sizeof(voPoint), pointCount))
              break;
          }

          voReleaseObject(engine, stroke);
          stroke = NULL;
          
        } // end of: for (i = 0; i < count; ++i)
        
        if (i < count)
          break;

        // end input unit
        verify(VO_IFACE(engine, IFloatStructuredInput)->endInputUnit(engine, input, VO_CHAR));
        
      } // end of: for (k = 0; k < columnCount; ++k)
      
      if (k < columnCount)
        break;
      
    } // end of: for (j = 0; j < rowCount; ++j)
    
    if (j < rowCount)
      break;

    ok = true;
    break;
  }

  if (stroke != NULL)
    voReleaseObject(engine, stroke);
  if (ink != NULL)
    voReleaseObject(engine, ink);
  free(points);

  return ok;
}
