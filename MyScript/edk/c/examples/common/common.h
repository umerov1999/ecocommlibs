#ifndef EDK_C_EXAMPLES_COMMON_COMMON_H
/**
 * @file edk/c/examples/common/common.h
 *
 * This file contains common functions used by all examples. Most of the
 * functions assume the existence of a global voEngine variable that is already
 * defined for the sake of readability.
 */


#include <c/std/stdbool.h>
#include <stdio.h>

#include <c/MyScriptEngine.h>
#include <c/MyScriptText.h>


// -- error checking -----------------------------------------------------------

#define verify(condition) verify_(condition, __FILE__, __LINE__)
void verify_(bool condition, const char* file, int line);

#define verifyEx(condition, message) verifyEx_(condition, __FILE__, __LINE__, message)
void verifyEx_(bool condition, const char* file, int line, const char* message);


// -- i/o functions ------------------------------------------------------------

void resolvePath(char* out, size_t capacity, const char* path);
void printPath(FILE* out, const char* path);
void storeEngineObject(voEngineObject object, const char* path);
voEngineObject loadEngineObject(const char* path);
bool fileExists(const char* path);
FILE* fileOpen(const char* path, const char* mode);


// -- input range output -------------------------------------------------------

void printInputRange(voInputRange range);


// -- text ui progress bar -----------------------------------------------------

struct _TextUIProgressBar
{
  bool redraw;
  int cursorColumn;
};
typedef struct _TextUIProgressBar TextUIProgressBar;

void initializeTextUIProgressBar(TextUIProgressBar* progressBar);
void finalizeTextUIProgressBar(TextUIProgressBar* progressBar);
bool updateTextUIProgressBar(uint32_t amountDone, uint32_t amountTodo, void* userParam);


// -- printf format to be used when outputing size_t ---------------------------

#if defined(_WIN32)
  #define SIZET_FMT "%Iu"
#elif defined(__linux__)
  #define SIZET_FMT "%zu"
#elif defined(__APPLE__)
  #define SIZET_FMT "%zu"
#else
  #define SIZET_FMT "%u"
#endif

#endif // #ifndef EDK_C_EXAMPLES_COMMON_COMMON_H
