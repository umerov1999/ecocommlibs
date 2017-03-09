#include "common.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <../src/MyScriptEngine-strerror.h>

// global engine (for the sake of examples readability)
extern voEngine engine;


// -- error checking -----------------------------------------------------------

void verify_(bool condition, const char* file, int line)
{
  assert(engine);

  if (!condition)
  {
    fprintf(stderr, "%s, %d: %s\n", file, line, strMyScriptError(voGetError(engine)));
    fflush(stderr);
    exit(EXIT_FAILURE);
  }
}

void verifyEx_(bool condition, const char* file, int line, const char* message)
{
  assert(engine);

  if (!condition)
  {
    int error = voGetError(engine);

    fprintf(stderr, "%s, %d: %s\n  %s\n", file, line, strMyScriptError(error), message);
    fflush(stderr);
    exit(EXIT_FAILURE);
  }
}


// -- executable path introspection --------------------------------------------

#define WHEREAMI_H
#define WAI_FUNCSPEC static
#define WAI_PREFIX(function) function
#include "../../src/MyScript-whereami.c"


// -- i/o functions ------------------------------------------------------------

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define PATH_MAX MAX_PATH

#else // else of: #ifdef _WIN32

#include <limits.h>

#endif // end of else of: #ifdef _WIN32

void resolvePath(char* out, size_t capacity, const char* path)
{
  int dirname_length = -1;
  char from, to;

  if (strncmp(path, "${origin}", sizeof("${origin}") - 1) == 0)
  {
    getExecutablePath(out, (int)capacity, &dirname_length);
    path += sizeof("${origin}") - 1;
  }
  else if (strncmp(path, "${executable}", sizeof("${executable}") - 1) == 0)
  {
    getExecutablePath(out, (int)capacity, &dirname_length);
    path += sizeof("${executable}") - 1;
  }
  else if (strncmp(path, "${module}", sizeof("${module}") - 1) == 0)
  {
    getModulePath(out, (int)capacity, &dirname_length);
    path += sizeof("${module}") - 1;
  }

#ifdef _WIN32
  from = '/';
  to = '\\';
#else
  from = '\\';
  to = '/';
#endif

  out += dirname_length + 1;
  capacity -= dirname_length + 1 + 1;

  if (dirname_length > 0 && (*path == from || *path == to))
    ++path;

  while (capacity && *path)
  {
    char c = *path++;

    *out++ = c == from ? to : c;
    --capacity;
  }
  *out = '\0';
}

void printPath(FILE* out, const char* path)
{
  char path_[PATH_MAX];
#if defined(_WIN32)
  wchar_t path__[PATH_MAX];
#else
  char path__[PATH_MAX];
#endif

  assert(out);
  assert(path);

  resolvePath(path_, sizeof(path_), path);

#if defined(_WIN32)
  if (MultiByteToWideChar(CP_UTF8, 0, path_, -1, path__, sizeof(path__) / sizeof(*path__)))
  {
    wchar_t path___[PATH_MAX];

    if (GetFullPathNameW(path__, sizeof(path___) / sizeof(*path___), path___, NULL))
    {
      fwprintf(out, L"%s", path___);
    }
  }
#else
  realpath(path_, path__);
  fprintf(out, "%s", path__);
#endif
}

void storeEngineObject(voEngineObject object, const char* path)
{
  char path_[PATH_MAX];
  voString s = voString_INITIALIZER(path_);

  assert(engine);
  assert(path);

  resolvePath(path_, sizeof(path_), path);
  verify(voStoreObjectToFile(engine, object, NULL, &s, NULL));
}

voEngineObject loadEngineObject(const char* path)
{
  char path_[PATH_MAX];
  voString s = voString_INITIALIZER(path_);
  voEngineObject result;

  assert(engine);
  assert(path);

  resolvePath(path_, sizeof(path_), path);

  result = voLoadObjectFromFile(engine, NULL, &s, NULL);
  if (result == NULL)
  {
    fprintf(stderr, "error: cannot load ");
    printPath(stderr, path);
    fprintf(stderr, "\n");
    fflush(stderr);
    verify(false);
  }

  return result;
}

bool fileExists(const char* path)
{
  FILE* file;
  bool exists;

  file = fileOpen(path, "r");
  exists = file != 0;

  if (file)
    fclose(file);

  return exists;
}

FILE* fileOpen(const char* path, const char* mode)
{
  char path_[PATH_MAX];

  assert(path);

  resolvePath(path_, sizeof(path_), path);

  return fopen(path_, mode);
}


// -- input range output -------------------------------------------------------

void printInputRange(voInputRange range)
{
  unsigned int i;

  for (i = 0; i < range.count; ++i)
  {
    if (i > 0)
      printf("%s", ",");

    printf( "%d:%d:%d-%d:%d:%d",
            range.elements[i].first.unitIndex,
            range.elements[i].first.componentIndex,
            range.elements[i].first.itemIndex,
            range.elements[i].last.unitIndex,
            range.elements[i].last.componentIndex,
            range.elements[i].last.itemIndex);
  }
}


// -- text ui progress bar -----------------------------------------------------

void initializeTextUIProgressBar(TextUIProgressBar* progressBar)
{
  progressBar->redraw = true;
  progressBar->cursorColumn = 0;
  updateTextUIProgressBar(0, 1, progressBar);
}

void finalizeTextUIProgressBar(TextUIProgressBar* progressBar)
{
  updateTextUIProgressBar(1, 1, progressBar);
  putchar('\n');
}

bool updateTextUIProgressBar(uint32_t amountDone, uint32_t amountTodo, void* userParam)
{
  TextUIProgressBar* progressBar = (TextUIProgressBar*)userParam;
  bool doFlush = false;
  int newCursorColumn;
  if (progressBar->redraw)
  {
    printf("  0%% [                                                  ] 100%%\n");
    printf("      ");
    progressBar->cursorColumn = 0;
    progressBar->redraw = false;
    doFlush = true;
  }
  newCursorColumn = (int)(50.0f * (float)amountDone / (float)amountTodo + 0.5f);
  if (newCursorColumn > 50)
    newCursorColumn = 50;
  for (; progressBar->cursorColumn < newCursorColumn; ++progressBar->cursorColumn)
  {
    putchar('^');
    doFlush = true;
  }
  if (doFlush)
    fflush(stdout);
  return true; // continue run
}
