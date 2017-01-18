/**
 * @file MyScript-dlHelper.c
 *
 */

#if !(defined(WINAPI_FAMILY) && ((WINAPI_FAMILY == WINAPI_FAMILY_APP) || (WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP)))
#define WHEREAMI_H
#define WAI_FUNCSPEC static
#define WAI_PREFIX(function) function
#include "MyScript-whereami.c"
#endif

#ifdef VO_MSE_FUNCPTR_STATIC
  #define VO_MSE_FUNCPTR_STORAGECLASS static
#else
  #define VO_MSE_FUNCPTR_STORAGECLASS
#endif // end of: #ifdef VO_MSE_FUNCPTR_STATIC

#ifndef CONF_SUFFIX
  #ifndef NDEBUG
    #define CONF_SUFFIX "-Debug"
  #else
    #define CONF_SUFFIX
  #endif
#endif // end of: #ifndef CONF_SUFFIX

#ifdef SDK_DIR
#ifndef MYSCRIPT_ENGINE_LIBRARY_DIR
  #if defined(_MSC_VER) && defined(_M_X64) && defined(WINAPI_FAMILY) && ((WINAPI_FAMILY == WINAPI_FAMILY_APP) || (WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP))
    #define MYSCRIPT_ENGINE_LIBRARY_DIR NULL
  #elif defined(_MSC_VER) && defined(_M_X64)
    #define MYSCRIPT_ENGINE_LIBRARY_DIR SDK_DIR "/engine/bin/win-x64" CONF_SUFFIX
  #elif defined(_MSC_VER) && defined(_M_IX86)
    #define MYSCRIPT_ENGINE_LIBRARY_DIR SDK_DIR "/engine/bin/win-x86" CONF_SUFFIX
  #elif defined(__ANDROID__) && defined(__ARMEL__) && defined(__ARM_PCS_VFP)
    #define MYSCRIPT_ENGINE_LIBRARY_DIR SDK_DIR "/engine/bin/android-armeabi-v7a-hard" CONF_SUFFIX
  #elif defined(__ANDROID__) && defined(__ARMEL__)
    #define MYSCRIPT_ENGINE_LIBRARY_DIR SDK_DIR "/engine/bin/android-armeabi-v7a" CONF_SUFFIX
  #elif defined(__ANDROID__) && defined(__AARCH64EL__)
    #define MYSCRIPT_ENGINE_LIBRARY_DIR SDK_DIR "/engine/bin/android-arm64-v8a" CONF_SUFFIX
  #elif defined(__ANDROID__) && defined(__x86_64__)
    #define MYSCRIPT_ENGINE_LIBRARY_DIR SDK_DIR "/engine/bin/android-x64" CONF_SUFFIX
  #elif defined(__ANDROID__) && defined(__i386__)
    #define MYSCRIPT_ENGINE_LIBRARY_DIR SDK_DIR "/engine/bin/android-x86" CONF_SUFFIX
  #elif defined(__linux__) && defined(__x86_64__)
    #define MYSCRIPT_ENGINE_LIBRARY_DIR SDK_DIR "/engine/bin/lin-x64" CONF_SUFFIX
  #elif defined(__linux__) && defined(__i386__)
    #define MYSCRIPT_ENGINE_LIBRARY_DIR SDK_DIR "/engine/bin/lin-x86" CONF_SUFFIX
  #elif defined(__linux__) && defined(__ARMEL__)
    #define MYSCRIPT_ENGINE_LIBRARY_DIR SDK_DIR "/engine/bin/lin-armv7" CONF_SUFFIX
  #elif defined(__linux__) && (defined(__AARCH64EL__) || defined(__aarch64__))
    #define MYSCRIPT_ENGINE_LIBRARY_DIR SDK_DIR "/engine/bin/lin-arm64" CONF_SUFFIX
  #elif defined(__APPLE__)
    #define MYSCRIPT_ENGINE_LIBRARY_DIR SDK_DIR "/engine/bin/mac-x86+x64" CONF_SUFFIX
  #elif defined(SITARA)
    #define MYSCRIPT_ENGINE_LIBRARY_DIR SDK_DIR "/engine/bin/sitara-armv7" CONF_SUFFIX
  #elif defined(TIZEN) && defined(__i386__)
    #define MYSCRIPT_ENGINE_LIBRARY_DIR SDK_DIR "/engine/bin/tizen-x86" CONF_SUFFIX
  #elif defined(TIZEN) && defined(__arm__)
    #define MYSCRIPT_ENGINE_LIBRARY_DIR SDK_DIR "/engine/bin/tizen-armv7" CONF_SUFFIX
  #elif defined(QNX650) && defined(__arm__)
    #define MYSCRIPT_ENGINE_LIBRARY_DIR SDK_DIR "/engine/bin/qnx650-armv7" CONF_SUFFIX
  #elif defined(QNX660) && defined(__arm__)
    #define MYSCRIPT_ENGINE_LIBRARY_DIR SDK_DIR "/engine/bin/qnx660-armv7" CONF_SUFFIX
  #else
    #error UNSUPPORTED PLATFORM
  #endif
#endif // end of: #ifndef MYSCRIPT_ENGINE_LIBRARY_DIR
#endif // end of: #ifdef SDK_DIR

#if defined(_WIN32)
#define DEFAULT_MYSCRIPT_ENGINE_LIBRARY_NAME "MyScriptEngine.dll"
#elif defined(__linux__) || defined(__ANDROID__) || defined(__QNXNTO__)
#define DEFAULT_MYSCRIPT_ENGINE_LIBRARY_NAME "libMyScriptEngine.so"
#elif defined(__APPLE__)
#define DEFAULT_MYSCRIPT_ENGINE_LIBRARY_NAME "libMyScriptEngine.dylib"
#else
#error UNSUPPORTED PLATFORM
#endif

#include <stdlib.h>
#include <string.h>


// -- POSIX dynamic linking support --------------------------------------------

#ifdef _WIN32

  #define WIN32_LEAN_AND_MEAN
  #pragma warning(push)
  #pragma warning(disable: 4668)
  #include <windows.h>
  #pragma warning(pop)
  #define PATH_MAX MAX_PATH

  #define RTLD_LAZY 1
  #define RTLD_NOW 2
  #define RTLD_GLOBAL 4
  #define RTLD_LOCAL 8

  static void* dlopen(const char* fileName, int mode)
  {
    wchar_t buffer[MAX_PATH];

    if (MultiByteToWideChar(CP_UTF8, 0, fileName, -1, buffer, sizeof(buffer) / sizeof(*buffer)))
    {
#if defined(WINAPI_FAMILY) && ((WINAPI_FAMILY == WINAPI_FAMILY_APP) || (WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP))
      return (void*)LoadPackagedLibrary(buffer, 0);
#else
      wchar_t buffer_[MAX_PATH];

      GetFullPathNameW(buffer, sizeof(buffer_) / sizeof(*buffer_), buffer_, NULL);

      return (void*)LoadLibraryW(buffer_);
#endif
    }

    return NULL;
  }

  static int dlclose(void* handle)
  {
    return FreeLibrary((HMODULE)handle) ? 0 : -1;
  }

  static void* dlsym(void* handle, const char* symbol)
  {
    return GetProcAddress((HMODULE)handle, symbol);
  }

  static const char* dlerror(void)
  {
    DWORD error;

    error = GetLastError();

    if (error)
    {
      static char message[1024];
      DWORD length;

      length = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, error, MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT), message, sizeof(message), NULL);

      return message;
    }

    return "no error";
  }

#else // else of: #ifdef _WIN32

  #include <dlfcn.h>
  #include <limits.h>

  static void* dlopen_(const char* fileName, int mode)
  {
    char in[PATH_MAX];
    char out[PATH_MAX];
    const char* separator;

    if ((separator = strstr(fileName, "!/")))
    {
      strncpy(in, fileName, separator - fileName);
      in[separator - fileName] = '\0';
      if (!realpath(in, out))
        return NULL;
      strcat(out, separator);
    }
    else
    {
      if (!realpath(fileName, out))
        return NULL;
    }

    return dlopen(out, mode);
  }
  #pragma push_macro("dlopen")
  #define dlopen dlopen_

#endif // end of else of: #ifdef _WIN32


// -- define function pointers -------------------------------------------------

#ifdef VO_MSE_FUNC_THROUGH_POINTERS

#define VO_NO_FUNCTIONS
#include <c/MyScriptEngine.h>
#undef VO_NO_FUNCTIONS

#ifndef VO_MSE_FUNC_THROUGH_POINTERS_NO_PTR_DEF
#undef VO_MSE_FUNC
#define VO_MSE_FUNC(type, name, params) VO_MSE_FUNCPTR_STORAGECLASS type (VO_MSE_CALL *name) params = NULL
#endif // end of: #ifndef VO_MSE_FUNC_THROUGH_POINTERS_NO_PTR_DEF
#undef C_MYSCRIPTENGINE_H
#define VO_FUNCTIONS_ONLY
#include <c/MyScriptEngine.h>


// -- library loading / unloading ----------------------------------------------

static void* libraryHandle = NULL;

static void unloadMyScriptEngine(void);

/**
 * Loads the MyScriptEngine library.
 *
 * @param dirName Optional location directory name. When `NULL`, system
 *   dependent rules are used to locate the library.
 *   When this string starts with `"${origin}"` or `"${executable}"` it will be
 *   expanded to the executable directory name.
 *   When this string starts with `"${module}"` it will be expanded to the
 *   calling module directory name.
 * @param fileName Optional name of the library. When `NULL`, the
 *   default library name is used, which is what we recommend you.
 *
 * @return A non zero value if successful.
 */
static int loadMyScriptEngine(const char* dirName, const char* fileName)
{
  int ok;
  void** sym_ptr;

  for (ok = 0; !ok; ok = 1)
  {
    if (!fileName)
      fileName = DEFAULT_MYSCRIPT_ENGINE_LIBRARY_NAME;

  #if !(defined(WINAPI_FAMILY) && ((WINAPI_FAMILY == WINAPI_FAMILY_APP) || (WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP)))
    if (dirName)
    {
      char path[PATH_MAX];
      int dirname_length = -1;
      int i = 0;
      int length = 0;
      int remaining;
      char from, to;

      if (strncmp(dirName, "${origin}", sizeof("${origin}") - 1) == 0)
      {
        getExecutablePath(path, sizeof(path) - 1, &dirname_length);
        dirName += sizeof("${origin}") - 1;
      }
      else if (strncmp(dirName, "${executable}", sizeof("${executable}") - 1) == 0)
      {
        getExecutablePath(path, sizeof(path) - 1, &dirname_length);
        dirName += sizeof("${executable}") - 1;
      }
      else if (strncmp(dirName, "${module}", sizeof("${module}") - 1) == 0)
      {
        getModulePath(path, sizeof(path) - 1, &dirname_length);
        dirName += sizeof("${module}") - 1;
      }

#ifdef _WIN32
      from = '/';
      to = '\\';
#else
      from = '\\';
      to = '/';
#endif

      i = dirname_length + 1;
      remaining = (int)sizeof(path) - 1 - i;

      if (dirname_length > 0 && (*dirName == from || *dirName == to))
        ++dirName;

      while (remaining && *dirName)
      {
        char c = *dirName++;

        path[i++] = c == from ? to : c;
        --remaining;
      }

      if (i > 0 && path[i - 1] != to)
      {
        if (remaining < 1)
          break;

        path[i++] = to;
        --remaining;
      }

      if (*fileName == from || *fileName == to)
        ++fileName;

      length = (int)strlen(fileName);
      if (length > remaining)
        break;

      strcpy(&path[i], fileName);

      libraryHandle = dlopen(path, RTLD_NOW);
      if (!libraryHandle)
        break;
    }
    else
  #endif
    {
      libraryHandle = dlopen(fileName, RTLD_NOW);
      if (!libraryHandle)
        break;
    }

    // get functions pointers
  #undef VO_MSE_FUNC
  #define VO_MSE_FUNC(type, name, params)\
    sym_ptr = (void**)&name, *sym_ptr = dlsym(libraryHandle, #name);\
    if (!sym_ptr)\
      break
  #undef C_MYSCRIPTENGINE_H
  #include <c/MyScriptEngine.h>
  #undef VO_MSE_FUNC
  }

  if (libraryHandle != NULL && !ok)
    unloadMyScriptEngine();

  return ok;
}

/**
 * Unloads the MyScriptEngine library.
 */
static void unloadMyScriptEngine()
{
  if (libraryHandle != NULL)
    dlclose(libraryHandle);
  libraryHandle = NULL;

  // reset function pointers
#undef VO_MSE_FUNC
#define VO_MSE_FUNC(type, name, params)\
  name = NULL
#undef C_MYSCRIPTENGINE_H
#include <c/MyScriptEngine.h>
#undef VO_MSE_FUNC
}

#undef VO_FUNCTIONS_ONLY

#ifndef _WIN32
#pragma pop_macro("dlopen")
#endif // end of: #ifndef _WIN32

#else // else of: #ifdef VO_MSE_FUNC_THROUGH_POINTERS

  #define loadMyScriptEngine(dirName, fileName) true
  #define unloadMyScriptEngine()

#endif // end of else of: #ifdef VO_MSE_FUNC_THROUGH_POINTERS
