#ifndef C_MYSCRIPTENGINE_H
#define C_MYSCRIPTENGINE_H 0x08010000
/**
 * @file c/MyScriptEngine.h
 * Native interface to the MyScript Engine. This is the interface through which
 * MyScript services expose their own interfaces.
 */

#include <stddef.h>
#include <c/std/stdbool.h>
#include <c/std/stdint.h>


// -----------------------------------------------------------------------------

#define VO_MAKE_VERSION(major, minor, revision) (((major) << 24) | ((minor) << 16) | (revision))
#define VO_VERSION(major, minor, revision)      VO_MAKE_VERSION(major##U, minor##U, revision##U)

/**
 * The version you must pass to the `voCreateEngine()` function.
 */
#define VO_MSE_VER C_MYSCRIPTENGINE_H

#ifndef VO_MSE_IMPORT
  #ifndef _WIN32
    #ifndef __cplusplus
      /** Declares an external symbol to be imported into the code. */
      #define VO_MSE_IMPORT extern
    #else
      #define VO_MSE_IMPORT extern "C"
    #endif
  #else
    #ifndef __cplusplus
      /** Declares an external symbol to be imported into the code. */
      #define VO_MSE_IMPORT __declspec(dllimport)
    #else
      #define VO_MSE_IMPORT extern "C" __declspec(dllimport)
    #endif
  #endif
#endif

#ifndef VO_MSE_EXPORT
  #ifndef _WIN32
    #ifndef __cplusplus
      /** Declares an external symbol to be exported from the code. */
      #define VO_MSE_EXPORT
    #else
      #define VO_MSE_EXPORT extern "C"
    #endif
  #else
    #ifndef __cplusplus
      /** Declares an external symbol to be exported from the code. */
      #define VO_MSE_EXPORT __declspec(dllexport)
    #else
      #define VO_MSE_EXPORT extern "C" __declspec(dllexport)
    #endif
  #endif
#endif

#ifndef VO_MSE_CALL
  #ifdef _WIN32
    #define VO_MSE_CALL __cdecl
  #else
    /** Defines the calling convention of a function or function type. */
    #define VO_MSE_CALL
  #endif
#endif

#ifndef VO_MSE_FUNC
  #ifndef VO_MSE_IMPL
    #ifndef VO_MSE_FUNC_THROUGH_POINTERS
      /* Declares a function as part of the MyScript Engine native interface. */
      #define VO_MSE_FUNC(type,name,params) VO_MSE_IMPORT type VO_MSE_CALL name params
    #else
      #ifndef __cplusplus
        #define VO_MSE_FUNC(type,name,params) extern type (VO_MSE_CALL *name) params
      #else
        #define VO_MSE_FUNC(type,name,params) extern "C" type (VO_MSE_CALL *name) params
      #endif
    #endif
  #else
    #define VO_MSE_FUNC(type,name,params) VO_MSE_EXPORT type VO_MSE_CALL name params
  #endif
#endif

/** Declares an inline function. */
#ifndef VO_INLINE
  #if defined(_MSC_VER)
    #define VO_INLINE static __inline
  #elif defined(__GNUC__)
    #define VO_INLINE static __inline__
  #else
    #define VO_INLINE static
  #endif
#endif

#ifndef VO_FUNCTIONS_ONLY


// -- basic types and constants ------------------------------------------------

/**
 * Invalid size value, returned by some functions to indicate an error.
 */
#define VO_INVALID_SIZE_T ((size_t)-1)

#ifndef VO_ID_TYPES
#define VO_ID_TYPES

/**
 * Abstract identifier.
 */
typedef int32_t voId;

/**
 * Type identifier.
 */
typedef voId voTypeId;

/**
 * Property identifier.
 */
typedef voId voPropId;

#endif // end of: #ifndef VO_ID_TYPES

/**
 * Defines the underlying type for flags.
 */
#define VO_FLAGS_T(enum) uint32_t

/**
 * Invalid flags value, returned by some functions to indicate an error.
 */
#define VO_INVALID_FLAGS ((uint32_t)-1)

#ifndef VO_YES_NO_TYPE
#define VO_YES_NO_TYPE

/**
 * Return type of functions that provide a binary response but need a third
 * value to notify a possible failure.
 */
typedef enum _voYesNo
{
  /**
   * The function failed.
   */
  VO_ERR = -1,

  /**
   * The response is "no".
   */
  VO_NO = 0,

  /**
   * The response is "yes".
   */
  VO_YES = 1,

  _voYesNo_FORCE_INT_SIZE = 0x7FFFFFFF
}
voYesNo;

#endif // end of: #ifndef VO_YES_NO_TYPE

#ifndef VO_CERTIFICATE_TYPE
#define VO_CERTIFICATE_TYPE

/**
 * Certificate.
 */
typedef struct _voCertificate
{
  /**
   * Pointer to the bytes composing the certificate.
   */
  const char* bytes;

  /**
   * Length of the certificate.
   */
  size_t length;
}
voCertificate;

#endif // end of: #ifndef VO_CERTIFICATE_TYPE

#ifndef VO_REFERENCE_TYPE
#define VO_REFERENCE_TYPE

struct _voReference;
/**
 * Reference to a MyScript internal object.
 */
typedef struct _voReference* voReference;

#endif // end of: #ifndef VO_REFERENCE_TYPE


#ifndef VO_READ_FUNC_TYPE
#define VO_READ_FUNC_TYPE

/**
 * Defines an application supplied function that performs read operations
 * triggered by the `voLoadObject()` function.
 *
 * @param buffer a pointer to the destination/source buffer.
 * @param bufferLength the length of the buffer in bytes.
 * @param userParam an optional user defined parameter passed to the callback.
 *
 * @return the number of bytes actually read, or `VO_INVALID_SIZE_T` on failure.
 *
 * @note When your implementation of the read function returns `0` on failure
 *   instead of the magic `VO_INVALID_SIZE_T` value, the `voGetError()` function
 *   will report the `VO_PREMATURE_END_OF_DATA` error instead of the
 *   `VO_IO_FAILURE` error.
 *
 * @see voLoadObject
 */
typedef size_t (VO_MSE_CALL *voReadFunc)(void* buffer, size_t bufferLength, void* userParam);

#endif // end of: #ifndef VO_READ_FUNC_TYPE


#ifndef VO_WRITE_FUNC_TYPE
#define VO_WRITE_FUNC_TYPE

/**
 * Defines an application supplied function that performs write operations
 * triggered by the `voStoreObject()` function.
 *
 * @param buffer a pointer to the destination/source buffer.
 * @param bufferLength the length of the buffer in bytes.
 * @param userParam an optional user defined parameter passed to the callback.
 *
 * @return the number of bytes actually written.
 *
 * @see voStoreObject
 */
typedef size_t (VO_MSE_CALL *voWriteFunc)(const void* buffer, size_t bufferLength, void* userParam);

#endif // end of: #ifndef VO_WRITE_FUNC_TYPE


#ifndef VO_NOTIFY_PROGRESS_FUNC_TYPE
#define VO_NOTIFY_PROGRESS_FUNC_TYPE

/**
 * Defines an application supplied function that will receive progress
 * notifications from a time consuming task.
 *
 * @param amountDone the amount of work done since the beginning of the task.
 * @param amountTodo the total amount of work to do to complete the task.
 * @param userParam an optional user defined parameter passed to the callback.
 *
 * @return `true` to continue the task, `false` to request its cancelation.
 *
 * @note a particular operation may not be cancelable or cancelable only up till
 *   some point in the progress, in which case it will continue as if you
 *   returned `true`.
 */
typedef bool (VO_MSE_CALL * voNotifyProgressFunc)(uint32_t amountDone, uint32_t amountTodo, void* userParam);

#endif // end of: #ifndef VO_NOTIFY_PROGRESS_FUNC_TYPE


// -- error codes --------------------------------------------------------------

#ifndef VO_ERR_TYPE
#define VO_ERR_TYPE

/**
 * Lists the MyScript errors.
 */
enum VO_ERR
{
  /**
   * No error occurred. This is the value returned by `voGetError()` when there
   * is no error to report.
   */
  VO_NO_ERROR,

  /**
   * The argument is invalid.
   */
  VO_INVALID_ARGUMENT = 0x1000000,

  /**
   * The requested operation is not implemented for the target object.
   */
  VO_INVALID_OPERATION = 0x2000000,

  /**
   * The requested operation cannot be performed within the current state.
   */
  VO_INVALID_STATE = 0x3000000,

  /**
   * Runtime error.
   */
  VO_RUNTIME_ERROR = 0x4000000,

  /**
   * The reference is invalid.
   */
  VO_NO_SUCH_REFERENCE = 0x1010000,

  /**
   * The engine reference is invalid.
   */
  VO_NO_SUCH_ENGINE,

  /**
   * The object reference is invalid.
   */
  VO_NO_SUCH_OBJECT,

  /**
   * Invalid certificate.
   */
  VO_INVALID_CERTIFICATE = 0x1020001,

  /**
   * The index is out of bounds.
   */
  VO_INDEX_OUT_OF_BOUNDS,

  /**
   * The object is invalid for the requested operation because it doesn't have
   * the proper type.
   */
  VO_INVALID_OBJECT,

  /**
   * The given value does not have the right size.
   */
  VO_INVALID_SIZE = 0x1020005,

  /**
   * A user supplied buffer is either unreadable or unwritable.
   *
   * Typically `NULL`.
   */
  VO_INVALID_USER_BUFFER = 0x1030000,

  /**
   * The specified type does not exist.
   */
  VO_NO_SUCH_TYPE = 0x1040000,

  /**
   * The specified type is not concrete. It is either an abstract type or an
   * interface type. This means instantiation is not allowed.
   */
  VO_TYPE_NOT_CONCRETE,

  /**
   * The specified type does not have a constructor with the specified
   * initializer.
   */
  VO_NO_SUCH_CONSTRUCTOR,

  /**
   * The specified interface does not exist.
   */
  VO_NO_SUCH_INTERFACE,

  /**
   * The specified property does not exist.
   */
  VO_NO_SUCH_PROPERTY,

  /**
   * The specified property is read-only.
   */
  VO_READ_ONLY_PROPERTY,

  /**
   * The specified object does not support serialization.
   */
  VO_UNSUPPORTED_SERIALIZATION,

  /**
   * The requested operation cannot be performed because it would exceed the
   * limit fixed as a programming guard.
   */
  VO_LIMIT_EXCEEDED = 0x3010000,

  /**
   * The object cannot be modified because it is in use by another object.
   */
  VO_MODIFICATION_ACCESS_DENIED = 0x3010002,

  /**
   * The engine ran out of memory.
   */
  VO_OUT_OF_MEMORY = 0x4000001,

  /**
   * An Input/Output operation failed.
   */
  VO_IO_FAILURE,

  /**
   * End of input has been reached while more data was needed.
   */
  VO_PREMATURE_END_OF_DATA,

  /**
   * The engine crashed.
   */
  VO_INTERNAL_ERROR,

  /**
   * The operation was canceled by the progress notification function.
   */
  VO_CANCELED,
};

#endif // end of: #ifndef VO_ERR_TYPE

/*
 * Lists the MyScript Engine errors.
 */
enum VO_ENGINE_ERR
{
  /**
   * Invalid version.
   */
  VO_INVALID_VERSION = 0x1020000,

  /**
   * The option is invalid for the requested operation.
   */
  VO_INVALID_OPTION = 0x1020004,

  /**
   * The iteration has no such element.
   */
  VO_NO_SUCH_ELEMENT = 0x1020006,

  /**
   * The callback is invalid.
   */
  VO_INVALID_CALLBACK = 0x1030001,

  /**
   * The object implements `voICompilable` and needs to be compiled prior to
   * performing the requested operation.
   */
  VO_NOT_COMPILED = 0x3010001,

  /**
   * The `endInputUnit()` call does not match any prior `startInputUnit()` call.
   */
  VO_UNMATCHED_INPUT_UNIT = 0x3010003,

  /**
   * The operation is not permitted from the current thread.
   *
   * @since 7.0_0
   */
  VO_NOT_PERMITTED_FROM_THIS_THREAD,

  /**
   * The file is missing.
   */
  VO_NO_SUCH_FILE = 0x4000006
};


// -- types and interfaces -----------------------------------------------------

/**
 * Lists the MyScript Engine types.
 */
enum VO_ENGINE_T
{
  /**
   * `voEngine` type identifier.
   *
   * @see voEngine
   */
  VO_Engine,

  /**
   * `voEngineObject` type identifier.
   *
   * @see voEngineObject
   */
  VO_EngineObject = 4,

  /**
   * `voIterator` type identifier.
   *
   * @see voIterator
   */
  VO_Iterator = 8,

  /**
   * `voCharset` type identifier.
   *
   * @see voCharset
   */
  VO_Charset = 101,

  /**
   * `voInput` type identifier.
   *
   * @see voInput
   */
  VO_Input,

  /**
   * `voStructuredInput` type identifier.
   *
   * @see voStructuredInput
   */
  VO_StructuredInput = 107,

  /**
   * `voFloatStructuredInput` type identifier.
   *
   * @see voFloatStructuredInput
   */
  VO_FloatStructuredInput,

  /**
   * `voInputCharacter` type identifier.
   *
   * @see voInputCharacter
   */
  VO_InputCharacter = 111,

  /**
   * `voHistoryManager` type identifier.
   *
   * @see voHistoryManager
   */
  VO_HistoryManager,

  /**
   * `voUserObject` type identifier.
   *
   * @see voUserObject
   */
  VO_UserObject,

  /**
  * `voInputString` type identifier.
  *
  * @see voInputString
  *
  * @since 8.1_0
  */
  VO_InputString
};


/**
 * Lists the MyScript Engine interfaces.
 */
enum VO_ENGINE_I
{
  /**
   * `voIIterator` interface identifier.
   *
   * @see voIIterator
   */
  VO_IIterator = 150,

  /**
   * `voICharset` interface identifier.
   *
   * @see voICharset
   */
  VO_ICharset,

  /**
   * `voIAttachTarget` interface identifier.
   *
   * @see voIAttachTarget
   */
  VO_IAttachTarget,

  /**
   * `voICompilable` interface identifier.
   *
   * @see voICompilable
   */
  VO_ICompilable,

  /**
   * `voIInput` interface identifier.
   *
   * @see voIInput
   */
  VO_IInput,

  /**
   * `voIStructuredInput` interface identifier.
   *
   * @see voIStructuredInput
   */

  VO_IStructuredInput = 159,
  /**
   * `voIFloatStructuredInput` interface identifier.
   *
   * @see voIFloatStructuredInput
   */
  VO_IFloatStructuredInput,

  /**
   * `voIInputConsumer` interface identifier.
   *
   * @see voIInputConsumer
   */
  VO_IInputConsumer = 163,

  /**
   * `voIRunnable` interface identifier.
   *
   * @see voIRunnable
   */
  VO_IRunnable,

  /**
   * `voIInputCharacter` interface identifier.
   *
   * @see voIInputCharacter
   */
  VO_IInputCharacter = 166,

  /**
   * `voIHistoryManager` interface identifier.
   *
   * @see voIHistoryManager
   */
  VO_IHistoryManager,

  /**
   * `voIVersionable` interface identifier.
   *
   * @see voIVersionable
   */
  VO_IVersionable,

  /**
   * `voIUserObject` interface identifier.
   *
   * @see voIUserObject
   */
  VO_IUserObject,

  /**
   * `voIInputString` interface identifier.
   *
   * @see voIInputString
   *
   * @since 8.1_0
   */
  VO_IInputString = 171
};


// -- Engine -------------------------------------------------------------------

/**
 * `voEngine` reference type.
 *
 * Represents the MyScript runtime execution environment.
 *
 * {@typeProperties VO_ENGINE_TPROP}
 *
 * @note The `voEngine` type differs from the `voEngineObject` type and as a
 *   consequence will cause a `VO_NO_SUCH_OBJECT` error if you try to use a
 *   `voEngine` instead of a `voEngineObject`.
 *
 * @see voCreateEngine, voDestroyEngine
 */
typedef voReference voEngine;

/**
 * {@typePropertySet Engine}
 */
enum VO_ENGINE_TPROP
{
  /**
   * Represents the maximum number of `voEngine` instances that can exist at the
   * same time in a MyScript enabled application. The special value zero means
   * no limit. Other values are adjusted to the next multiple of 16. A value
   * under the current count is permitted and will constrain future creations.
   *
   * {@property uint32_t}
   *
   * @note The engine parameter of `vo[Get|Set]TypeProperty()` must be set to
   *   `NULL`.
   */
  VO_ENGINE_LIMIT,

  /**
   * Gives the number of `voEngine` instances that currently exist in a MyScript
   * enabled application.
   *
   * {@property read-only uint32_t}
   *
   * @note The engine parameter of `voGetTypeProperty()` must be set to `NULL`.
   */
  VO_ENGINE_COUNT,

  /**
   * Gives statistics about memory allocated by a `voEngine` instance.
   *
   * {@property read-only voMemoryStatistics}
   *
   * @note The engine parameter of `voGetTypeProperty()` can be set to `NULL` in
   *   order to retrieve memory statistics from the engine factory, which is
   *   unique for the whole application.
   */
  VO_MEM_STATS = 4,

  /**
   * Holds a default `voCharset` object. This property enables you to specify a
   * default charset once and for all: when done, you are allowed to pass `NULL`
   * to every function that takes a `voCharset` parameter.
   *
   * @note Since version 5.0 `voCreateEngine()` initializes this with a UTF-8
   *   charset, while previous versions did not set any default charset.
   *
   * {@property voCharset}
   */
  VO_DEFAULT_CHARSET
};

#ifndef VO_MEMORY_FUNCTIONS_TYPE
#define VO_MEMORY_FUNCTIONS_TYPE

/**
 * A set of memory management functions that may be given to `voCreateEngine()`.
 *
 * @see voCreateEngine
 */
typedef struct _voMemoryFunctions
{
  /**
   * The version of this struct: must be `1`.
   */
  int32_t structVersion;

  /**
   * Pointer to a function that allocates space for an array of `nmemb` objects
   * of size `size`, initialized to all bits zero;
   * in conformance with C89-7.10.3.1 / C99-7.20.3.1.
   */
  void* (VO_MSE_CALL *ptr_calloc)(size_t nmemb, size_t size);

  /**
   * Pointer to a function that deallocates the space pointed to by `ptr`;
   * in conformance with C89-7.10.3.2 / C99-7.20.3.2.
   */
  void (VO_MSE_CALL *ptr_free)(void* ptr);

  /**
   * Pointer to a function that allocates space for `size` bytes;
   * in conformance with C89-7.10.3.3 / C99-7.20.3.3.
   */
  void* (VO_MSE_CALL *ptr_malloc)(size_t size);

  /**
   * Pointer to a function that resizes the space pointed to by `ptr` to `size`;
   * in conformance with C89-7.10.3.4 / C99 7.20.3.4.
   */
  void* (VO_MSE_CALL *ptr_realloc)(void* ptr, size_t size);

  /**
   * Pointer to an optional function that returns the size available in the
   * space pointed to by `ptr`. If `NULL`, memory statistics will count only
   * requested not actual memory usage.
   */
  size_t (VO_MSE_CALL *ptr_msize)(void* ptr);
}
voMemoryFunctions;

#endif // end of: #ifndef VO_MEMORY_FUNCTIONS_TYPE

#ifndef VO_MEMORY_STATISTICS_TYPE
#define VO_MEMORY_STATISTICS_TYPE

/**
 * Represents statistics about memory usage.
 */
typedef struct _voMemoryStatistics
{
  /**
   * Current memory usage.
   */
  size_t curMemoryUsage;

  /**
   * Maximum memory usage.
   */
  size_t maxMemoryUsage;

  /**
   * Current number of allocated memory blocks.
   */
  size_t curAllocCount;

  /**
   * Total number of allocated memory blocks.
   */
  size_t totalAllocCount;
}
voMemoryStatistics;

#endif // end of: #ifndef VO_MEMORY_STATISTICS_TYPE


// -- EngineObject -------------------------------------------------------------

/**
 * `voEngineObject` reference type.
 *
 * The `voEngineObject` type is the abstract base type of all the types that
 * expose the different aspects of the MyScript technologies.
 *
 * Each `voEngineObject` is associated with a specific `voEngine`, which can
 * be viewed as its parent.
 *
 * Remember that when you destroy a parent `voEngine`, all its children are
 * released.
 *
 * {@typeProperties VO_ENGINEOBJECT_TPROP}
 * {@properties VO_ENGINEOBJECT_PROP}
 */
typedef voReference voEngineObject;

/**
 * {@typePropertySet EngineObject}
 */
enum VO_ENGINEOBJECT_TPROP
{
  /**
   * Represents the maximum number of `voEngineObject` instances that can exist
   * at the same time in the user side of a MyScript enabled application.
   *
   * The special value zero means no limit. Other values are adjusted to the
   * next multiple of 16. A value under the current count is permitted and will
   * constrain future creations.
   *
   * {@property uint32_t}
   *
   * @note This applies only to types that do not define their own user limit
   *   property.
   */
  VO_USER_OBJ_LIMIT,

  /**
   * Gives the number of `voEngineObject` instances that currently exist in the
   * user side of a MyScript enabled application.
   *
   * {@property read-only uint32_t}
   *
   * @note This applies only to types that do not define their own user limit
   *   property.
   */
  VO_USER_OBJ_COUNT
};

/**
 * {@propertySet EngineObject}
 */
enum VO_ENGINEOBJECT_PROP
{
  /**
   * Gives the number of user references to this object.
   *
   * {@property read-only uint32_t}
   */
  VO_USER_REFERENCE_COUNT = 0x40000,

  /**
   * Gives the number of attach references to this object.
   *
   * {@property read-only uint32_t}
   */
  VO_ATTACH_REFERENCE_COUNT,

  /**
   * Gives the memory usage of this object, in bytes.
   *
   * {@property read-only size_t}
   *
   * @note This property is available only for some types of objects, typically
   *   serializable objects. If the object does not provide the information,
   *   `voGetProperty()` returns `false` and `voGetError()` returns
   *   `VO_INVALID_OPERATION`.
   */
  VO_MEMORY_USAGE
};

#endif // end of: #ifndef VO_FUNCTIONS_ONLY

#ifndef VO_NO_FUNCTIONS
// -- library entry points -----------------------------------------------------

/**
 * Creates a new engine instance.
 *
 * @param version must be `VO_MSE_VER`.
 * @param certificate the certificate that identifies you as a legitimate
 *   MyScript customer.
 * @param memoryFunctions optional memory functions.
 *
 * @return the reference to the newly created engine on success, otherwise
 *   `NULL`.
 *
 * @throws VO_INVALID_VERSION when the requested version is invalid.
 * @throws VO_INVALID_CERTIFICATE when the certificate is invalid.
 * @throws VO_INVALID_USER_BUFFER when either the `certificate` or its `bytes`
 *   member is `NULL`.
 * @throws VO_INVALID_ARGUMENT when the memory functions are invalid.
 * @throws VO_LIMIT_EXCEEDED when creating the reference to the `voEngine` would
 *   cause the `VO_ENGINE_COUNT` value to exceed the `VO_ENGINE_LIMIT` value.
 *
 * @see voDestroyEngine
 */
VO_MSE_FUNC(voEngine, voCreateEngine, (uint32_t version, const voCertificate* certificate, const voMemoryFunctions* memoryFunctions));

/**
 * Destroys an engine and releases all the objects created with it.
 *
 * @param engine the engine to be destroyed.
 *
 * @return `true` on success, otherwise `false`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 *
 * see @voCreateEngine.
 */
VO_MSE_FUNC(bool, voDestroyEngine, (voEngine engine));

/**
 * Returns the error set by the last function call in the current running
 * thread.
 *
 * @param engine the engine.
 *
 * @return the error set by the last function call in the current running
 *   thread, or `VO_NO_ERROR` if no error occurred.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 *
 * @note In order to get error information about errors that have occurred in
 *   the `voCreateEngine()` or `voDestroyEngine()` functions, you have to pass
 *   `NULL` for the `engine` parameter of this function.
 *
 * @see voCreateEngine, voDestroyEngine
 */
VO_MSE_FUNC(int32_t, voGetError, (voEngine engine));

/**
 * Explicitly sets an error for the specified engine. The error code can be
 * further retrieved via `voGetError()`.
 *
 * @param engine the engine.
 * @param code the error code.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 */
VO_MSE_FUNC(void, voSetError, (voEngine engine, int32_t code));

/**
 * Returns the functions for the specified interface.
 *
 * @param engine the engine.
 * @param iface the identifier of the interface.
 *
 * @return a pointer to the corresponding interface functions structure on
 *   success, otherwise `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_NO_SUCH_INTERFACE when the specified interface does not exist.
 * @throws VO_INVALID_OPERATION when the specified interface has no functions.
 *
 * @note This function is not supposed to be used directly, please use the
 *   `VO_IFACE()` helper macro instead.
 *
 * @note Invoking an interface function with an instance of a type that does not
 *   implement this interface sets the error code to `VO_INVALID_OPERATION`.
 *
 * @see VO_IFACE, voImplements
 */
VO_MSE_FUNC(const void*, voGetInterface, (voEngine engine, voTypeId iface));

/**
 * Helper macro that facilitates the use of interface functions by performing
 * the correct cast on the result of `voGetInterface()`.
 *
 * @param engine the engine.
 * @param IName the name of the interface without prefix, so that prepending
 *   `VO_` gives the identifier and prepending `vo` gives the function structure
 *   type.
 *
 * @example
 * <pre>
 * VO_IFACE(engine, ICharset)->setDefaultCharacter(engine, charset, '?');
 * </pre>
 *
 * @note Invoking an interface function with an instance of a type that does not
 *   implement this interface sets the error code to `VO_INVALID_OPERATION`.
 *
 * @see voGetInterface, voImplements
 */
#define VO_IFACE(engine, IName) ((const vo##IName*)voGetInterface(engine, VO_##IName))

/**
 * Answers the question: does the specified type implement the specified
 * interface?
 *
 * @param engine the engine.
 * @param type the identifier of the type.
 * @param iface the identifier of the interface.
 *
 * @return `VO_YES` or `VO_NO` on success, otherwise `VO_ERR`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_NO_SUCH_TYPE when the specified type does not exist.
 * @throws VO_NO_SUCH_INTERFACE when the specified interface does not exist.
 */
VO_MSE_FUNC(voYesNo, voImplements, (voEngine engine, voTypeId type, voTypeId iface));

/**
 * Answers the question: is `type1` compatible with `type2`?
 *
 * A type is said to be compatible with another type if it is either the same
 * type or it inherits from the second type.
 *
 * @param engine the engine.
 * @param type1 the identifier of the derived type.
 * @param type2 the identifier of the base type.
 *
 * @return `VO_YES` or `VO_NO` on success, otherwise `VO_ERR`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_NO_SUCH_TYPE when `type1` or `type2` does not exist.
 */
VO_MSE_FUNC(voYesNo, voIsCompatible, (voEngine engine, voTypeId type1, voTypeId type2));

/**
 * Returns the value of the specified type property.
 *
 * @param engine the engine.
 * @param type the identifier of the type (e.g. `VO_Engine`).
 * @param property the identifier of the property (e.g. `VO_MEM_STATS`).
 * @param value the recipient for the value.
 * @param sizeOfValue the size of the recipient for the value, in bytes. This
 *   parameter is used by the engine to ensure that the recipient you are using
 *   has the proper type.
 *
 * @return `true` on success, otherwise `false`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_NO_SUCH_TYPE when the specified type does not exist.
 * @throws VO_NO_SUCH_PROPERTY when the specified property does not exist for
 *   the specified type.
 * @throws VO_INVALID_SIZE when the recipient does not have the proper size.
 * @throws VO_INVALID_USER_BUFFER when the given recipient is not writable.
 */
VO_MSE_FUNC(bool, voGetTypeProperty, (voEngine engine, voTypeId type, voPropId property, void* value, size_t sizeOfValue));

/**
 * Sets or resets the value of the specified type property.
 *
 * @param engine the engine.
 * @param type the identifier of the type (e.g. `VO_Engine`).
 * @param property the identifier of the property (e.g. `VO_MEM_STATS`).
 * @param value the recipient for the value. Use `NULL` when you want to reset
 *   the property to its default value.
 * @param sizeOfValue the size of the recipient for the value, in bytes. This
 *   parameter is used by the engine to ensure that the recipient you are using
 *   has the proper type.
 *
 * @return `true` on success, otherwise `false`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_NO_SUCH_TYPE when the specified type does not exist.
 * @throws VO_NO_SUCH_PROPERTY when the specified property does not exist for
 *   the specified type.
 * @throws VO_READ_ONLY_PROPERTY when the property is read-only.
 * @throws VO_INVALID_SIZE when the recipient does not have the proper size.
 * @throws VO_INVALID_USER_BUFFER when the given recipient is not readable.
 * @throws VO_INVALID_ARGUMENT when the given value is invalid.
 */
VO_MSE_FUNC(bool, voSetTypeProperty, (voEngine engine, voTypeId type, voPropId property, const void* value, size_t sizeOfValue));

/**
 * Creates a new object of the specified type.
 *
 * @param engine the engine.
 * @param type the type of the object to be created.
 *
 * @return the reference to the newly created object on success, otherwise
 *   `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_NO_SUCH_TYPE when the specified type does not exist.
 * @throws VO_TYPE_NOT_CONCRETE when the specified type is not concrete. It may
 *   be an abstract type or an interface.
 * @throws VO_NO_SUCH_CONSTRUCTOR when the specified type does not have a
 *   default constructor.
 * @throws VO_LIMIT_EXCEEDED when the number of instances of the specified type
 *   would exceed the allowed limit (please remember that the limit can be
 *   shared by several types).
 *
 * @see voCreateObjectEx
 */
VO_MSE_FUNC(voEngineObject, voCreateObject, (voEngine engine, voTypeId type));

/**
 * Creates a new object of the specified type, initialized with the specified
 * initializer.
 *
 * @param engine the engine.
 * @param type the type of the object to be created.
 * @param initializer a pointer to the initializer.
 * @param sizeOfInitializer the size of the initializer in bytes.
 *
 * @return the reference to the newly created object on success, otherwise
 *   `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_NO_SUCH_TYPE when the specified type does not exist.
 * @throws VO_TYPE_NOT_CONCRETE when the specified type is not concrete. It may
 *   be an abstract type or an interface.
 * @throws VO_NO_SUCH_CONSTRUCTOR when the specified type does not have a
 *   constructor with the given initializer.
 * @throws VO_INVALID_USER_BUFFER when the specified initializer points to an
 *   unreadable memory location.
 * @throws VO_INVALID_SIZE when the specified initializer does not have the
 *   proper size.
 * @throws VO_LIMIT_EXCEEDED when the number of instances of the specified type
 *   would exceed the allowed limit (please remember that the limit can be
 *   shared by several types).
 *
 * @see voCreateObject
 */
VO_MSE_FUNC(voEngineObject, voCreateObjectEx, (voEngine engine, voTypeId type, const void* initializer, size_t sizeOfInitializer));

/**
 * Loads an object from an application supplied binary data stream.
 *
 * @param engine the engine.
 * @param readFunc a pointer to a `voReadFunc` function.
 * @param userParam an optional pointer to a user defined parameter that will
 *   be passed back to `readFunc`.
 *
 * @return the reference to the newly created object on success, otherwise
 *   `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_INVALID_CALLBACK when `readFunc` is invalid.
 * @throws VO_PREMATURE_END_OF_DATA when the binary data stream does not contain
 *   enough data.
 * @throws VO_IO_FAILURE when the binary data stream is invalid or corrupted.
 * @throws VO_LIMIT_EXCEEDED when the number of instances of the specified type
 *   would exceed the allowed limit (please remember that the limit can be
 *   shared by several types).
 */
VO_MSE_FUNC(voEngineObject, voLoadObject, (voEngine engine, voReadFunc readFunc, void* userParam));

/**
 * Stores an object to an application supplied binary data stream.
 *
 * @param engine the engine.
 * @param object the object to be stored.
 * @param writeFunc a pointer to a `voWriteFunc` function.
 * @param userParam an optional pointer to a user defined parameter that will
 *   be passed back to `writeFunc`.
 *
 * @return `true` on success, otherwise `false`.

 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_NO_SUCH_OBJECT when the `object` reference is invalid.
 * @throws VO_NOT_COMPILED when `object` implements the `voICompilable`
 *   interface and has to be compiled prior to being stored.
 * @throws VO_INVALID_CALLBACK when `writeFunc` is invalid.
 * @throws VO_IO_FAILURE when `writeFunc` did not write the requested number of
 *   bytes.
 * @throws VO_UNSUPPORTED_SERIALIZATION when `object` is not serializable.
 */
VO_MSE_FUNC(bool, voStoreObject, (voEngine engine, voEngineObject object, voWriteFunc writeFunc, void* userParam));

/**
 * Returns a new reference to the specified object. This function is useful in
 * some languages like C++ where you need to be able to design copyable objects.
 *
 * @example
 * <pre>
 * class EngineObject
 * {
 * public:
 *   EngineObject(voEngine engine, voTypeId type) : e(engine), o(voCreateObject(engine, type)) {}
 *   EngineObject(const EngineObject& rhs) : e(rhs.e), o(voAcquireObject(rhs.e, rhs.o)) {}
 *
 *   ...
 *
 *   ~EngineObject() { voReleaseObject(e, o); }
 * private:
 *   voEngine e;
 *   voEngineObject o;
 * };
 * </pre>
 *
 * @param engine the engine.
 * @param object the object to be acquired.
 *
 * @return a new reference to the specified object on success, otherwise `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_NO_SUCH_OBJECT when the `object` reference is invalid.
 * @throws VO_LIMIT_EXCEEDED when the number of user references to `object`
 *   would exceed `65535`.
 *
 * @see voReleaseObject
 */
VO_MSE_FUNC(voEngineObject, voAcquireObject, (voEngine engine, voEngineObject object));

/**
 * Clones the specified object.
 *
 * @param engine the engine.
 * @param object the object to be cloned.
 *
 * @return a reference to a clone of the specified object on success, otherwise
 *   `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_NO_SUCH_OBJECT when the `object` reference is invalid.
 * @throws VO_LIMIT_EXCEEDED when the number of instances of the specified type
 *   would exceed the allowed limit (please remember that the limit can be
 *   shared by several types).
 * @throws VO_INVALID_OPERATION when `object` does not support cloning.
 */
VO_MSE_FUNC(voEngineObject,voCloneObject,(voEngine engine, voEngineObject object));

/**
 * Releases a reference to the specified object.
 *
 * @param engine the engine.
 * @param object the object to be released.
 *
 * @return `true` on success, otherwise `false`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_NO_SUCH_OBJECT when the `object` reference is invalid.
 *
 * @see voAcquireObject
 */
VO_MSE_FUNC(bool, voReleaseObject, (voEngine engine, voEngineObject object));

/**
 * Answers the question: do the two references have the same referee?
 *
 * @param engine the engine.
 * @param object1 the first object.
 * @param object2 the second object.

 * @return `VO_YES` or `VO_NO` on success, otherwise `VO_ERR`.

 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_NO_SUCH_OBJECT when the `object1` reference is invalid.
 * @throws VO_NO_SUCH_OBJECT when the `object2` reference is invalid.
 */
VO_MSE_FUNC(voYesNo, voIsSame, (voEngine engine, voEngineObject object1, voEngineObject object2));

/**
 * Returns the type of the specified object.
 *
 * @param engine the engine.
 * @param object the object to be queried.
 *
 * @return the type of `object` on success, otherwise `-1`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_NO_SUCH_OBJECT when the `object` reference is invalid.
 */
VO_MSE_FUNC(voTypeId, voGetType, (voEngine engine, voEngineObject object));

/**
 * Returns the value of the specified object property.
 *
 * @param engine the engine.
 * @param object the object to be queried.
 * @param property the identifier of the object property.
 * @param value the recipient for the value.
 * @param sizeOfValue the size of the recipient for the value, in bytes. This
 *   parameter is used by the engine to ensure that the recipient you are using
 *   has the proper type.
 *
 * @return `true` on success, otherwise `false`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_NO_SUCH_OBJECT when the `object` reference is invalid.
 * @throws VO_NO_SUCH_PROPERTY when the specified property does not exist for
 *   the specified object.
 * @throws VO_INVALID_SIZE when the recipient does not have the proper size.
 * @throws VO_INVALID_USER_BUFFER when the given recipient is not writable.
 */
VO_MSE_FUNC(bool, voGetProperty, (voEngine engine, voEngineObject object, voPropId property, void* value, size_t sizeOfValue));

/**
 * Sets or resets the value of the specified object property.
 *
 * @param engine the engine.
 * @param object the object to be modified.
 * @param property the identifier of the object property.
 * @param value the recipient for the value. Use `NULL` when you want to reset
 *   the property to its default value.
 * @param sizeOfValue the size of the recipient for the value, in bytes. This
 *   parameter is used by the engine to ensure that the recipient you are using
 *   has the proper type.
 *
 * @return `true` on success, otherwise `false`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_NO_SUCH_OBJECT when the `object` reference is invalid.
 * @throws VO_NO_SUCH_PROPERTY when the specified property does not exist for
 *   the specified object.
 * @throws VO_READ_ONLY_PROPERTY when the property is read-only.
 * @throws VO_INVALID_SIZE when the recipient does not have the proper size.
 * @throws VO_INVALID_USER_BUFFER when the given recipient is not readable.
 * @throws VO_INVALID_ARGUMENT when the given value is invalid.
 */
VO_MSE_FUNC(bool,voSetProperty,(voEngine engine, voEngineObject object, voPropId property, const void* value, size_t sizeOfValue));
#endif // end of: #ifndef VO_NO_FUNCTIONS

#ifndef VO_FUNCTIONS_ONLY


// -- Iterator -----------------------------------------------------------------

/**
 * `voIterator` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voIIterator}
 */
typedef voEngineObject voIterator;

/**
 * Functions composing the `voIIterator` interface.
 *
 * This interface exposes the public methods of the `voIterator` type.
 *
 * {@implementingTypes voIterator}
 */
typedef struct _voIIterator
{
  /**
   * Tells if this iterator is positioned at the end of the iteration.
   *
   * @param engine the engine.
   * @param target the target iterator.
   *
   * @return `VO_YES` or `VO_NO` on success, otherwise `VO_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voIterator`.
   */
  voYesNo (VO_MSE_CALL *isAtEnd)(voEngine engine, voIterator target);

  /**
   * Positions this iterator on the next element of the iteration.
   *
   * @param engine the engine.
   * @param target the target iterator.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   */
  bool (VO_MSE_CALL *next)(voEngine engine, voIterator target);

  /**
   * Tells whether the two iterators are equal. Two iterators are considered
   * equal if they are positioned at the same position and iterate on the same
   * collection. This function will also return `VO_YES` if both iterators are
   * `NULL` or if one of the iterators is `NULL` and the other is positioned at
   * the end of the iteration.
   *
   * @param engine the engine.
   * @param target the target iterator.
   * @param other the other iterator.
   *
   * @return `VO_YES` or `VO_NO` on success, otherwise `VO_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voIterator`.
   * @throws VO_NO_SUCH_OBJECT when the `other` reference is invalid.
   * @throws VO_INVALID_OBJECT when `other` is not a `voIterator`.
   *
   * #since 7.3_0
   */
  voYesNo (VO_MSE_CALL *equals)(voEngine engine, voIterator target, voIterator other);
}
voIIterator;


// -- Charset ------------------------------------------------------------------

/**
 * `voCharset` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voICharset}
 */
typedef voEngineObject voCharset;

#ifndef VO_STRING_TYPE
#define VO_STRING_TYPE

/**
 * To be used in place of `voString.byteCount` when the string is zero ended.
 */
#define VO_ZERO_ENDED ((size_t)-1)

/**
 * Represents a character string encoded in a specific charset.
 *
 * When a `voString` is used as an output by a function, the typical usage is to
 * call the function twice:
 *
 *   - once with `bytes` set to `NULL` in which case `byteCount` will be set to
 *     the number of bytes you have to allocate/provide.
 *   - a second time with `bytes` pointing a writable buffer and `byteCount` set
 *     to the maximum number of bytes the function is allowed to write.
 *   - in any case, on output `byteCount` will always be set to the number of
 *     bytes needed to hold the string -- even if you allocated less.
 */
typedef struct _voString
{
  /**
   * Pointer to the bytes of the string.
   */
  char* bytes;
  /**
   * Number of bytes.
   */
  size_t byteCount;
}
voString;

#ifndef voString_INITIALIZER
/**
 * Helper macro that initializes a `voString` variable from a zero-ended string.
 * Usage:
 * <pre>
 *   voString s = voString_INITIALIZER("Hello world");
 * </pre>
 */
#define voString_INITIALIZER(string) { (char*)(string), VO_ZERO_ENDED }
#endif

#endif // end of: #ifndef VO_STRING_TYPE

/**
 * Functions composing the `voICharset` interface.
 *
 * This interface exposes the public methods of the `voCharset` type.
 *
 * {@implementingTypes voCharset}
 */
typedef struct _voICharset
{
  /**
   * Returns the default character associated with this charset.
   *
   * @param engine the engine.
   * @param target the target charset.
   *
   * @return the default character on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voCharset`.
   */
  int32_t (VO_MSE_CALL *getDefaultCharacter)(voEngine engine, voCharset target);

  /**
   * Sets the default character associated with this charset.
   *
   * @param engine the engine.
   * @param target the target charset.
   * @param character the new default character.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voCharset`.
   * @throws VO_INVALID_ARGUMENT when the specified default character is not
   *   encodable using `target`.
   * @throws VO_MODIFICATION_ACCESS_DENIED when `target` is in use by another
   *   object.
   */
  bool (VO_MSE_CALL *setDefaultCharacter)(voEngine engine, voCharset target, int32_t character);

  /**
   * Converts a character string from one representation to another.
   *
   * @param engine the engine.
   * @param target the target charset with which `inString` is encoded.
   * @param inString the input string.
   * @param other the destination charset with which `outString` will be
   *   encoded.
   * @param outString the output string.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `inString` or its `bytes` member is not
   *   readable.
   * @throws VO_NO_SUCH_OBJECT when the `other` reference is invalid.
   * @throws VO_INVALID_OBJECT when `other` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `outString` or its `bytes` member is
   *   not writable.
   */
  bool (VO_MSE_CALL *convert)(voEngine engine, voCharset target, const voString* inString, voCharset other, voString* outString);
}
voICharset;

#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voCharset` instance.
 *
 * @param engine the engine.
 * @param name the name of the charset to be created. Accepted values are:
 *   "US-ASCII", "ISO-8859-1", "ISO-8859-15", "windows-1252", "UTF-8", "UTF-8Y",
 *   "UTF-16", "UTF-16BE", "UTF-16LE", "UTF-32", "UTF-32BE", "UTF-32LE".
 *   If NULL, "UTF-8" is selected.
 *
 * @return the newly created `voCharset` instance on success, otherwise `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_INVALID_ARGUMENT when `name` is invalid.
 * @throws VO_LIMIT_EXCEEDED when the number of `voCharset` instances would
 *   exceed the allowed limit (please remember that the limit can be shared by
 *   several types).
 */
VO_INLINE voCharset voCreateCharset(voEngine engine, const char* name)
{
  return voCreateObjectEx(engine, VO_Charset, name, sizeof(const char*));
}

#endif // end of: #ifndef #ifndef VO_NO_FUNCTIONS

#endif // end of: #ifndef VO_FUNCTIONS_ONLY

#ifndef VO_NO_FUNCTIONS
/**
 * Loads an object from the specified file.
 *
 * @param engine the engine.
 * @param charset the charset used to encode parameter strings.
 * @param fileName the name of the file to be loaded.
 * @param mediaType an optional media type to use. This is useful if the actual
 *   mime type can't be derived from the file itself.
 *
 * @return the reference to the newly created object on success, otherwise
 *   `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
 * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
 *   default charset was set.
 * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
 * @throws VO_INVALID_USER_BUFFER when `fileName` or its `bytes` member is not
 *   readable.
 * @throws VO_INVALID_USER_BUFFER when `mediaType` or its `bytes` member is not
 *   readable.
 * @throws VO_PREMATURE_END_OF_DATA when the file does not contain enough data.
 * @throws VO_IO_FAILURE when the file content is invalid or corrupted.
 * @throws VO_NO_SUCH_FILE when the file does not exist.
 * @throws VO_NO_SUCH_TYPE when the media type is not supported, this may be
 *   caused by a missing or unauthorized library.
 * @throws VO_LIMIT_EXCEEDED when the number of instances of the specified type
 *   would exceed the allowed limit (please remember that the limit can be
 *   shared by several types).
 */
VO_MSE_FUNC(voEngineObject, voLoadObjectFromFile, (voEngine engine, voCharset charset, const voString* fileName, const voString* mediaType));

/**
 * Stores an object to the specified file.
 *
 * @param engine the engine.
 * @param object the object to be stored.
 * @param charset the charset used to encode parameter strings.
 * @param fileName the name of the file to be loaded.
 * @param mediaType an optional media type to use. This is useful if the actual
 *   mime type can't be derived from the file itself.
 *
 * @return `true` on success, otherwise `false`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_NO_SUCH_ENGINE when the `object` reference is invalid.
 * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
 * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
 *   default charset was set.
 * @throws VO_INVALID_USER_BUFFER when `fileName` or its `bytes` member is not
 *   readable.
 * @throws VO_INVALID_USER_BUFFER when `mediaType` or its `bytes` member is not
 *   readable.
 * @throws VO_NOT_COMPILED when `object` implements the `voICompilable`
 *   interface and has to be compiled prior to being stored.
 * @throws VO_IO_FAILURE when there was a problem writing the file.
 * @throws VO_NO_SUCH_FILE when a component of the pahtname does not exist.
 * @throws VO_UNSUPPORTED_SERIALIZATION when `object` is not serializable.
 */
VO_MSE_FUNC(bool, voStoreObjectToFile, (voEngine engine, voEngineObject object, voCharset charset, const voString* fileName, const voString* mediaType));
#endif // end of: #ifndef VO_NO_FUNCTIONS

#ifndef VO_FUNCTIONS_ONLY


// -- IAttachTarget ------------------------------------------------------------

/**
 * Functions composing the `voIAttachTarget` interface.
 */
typedef struct _voIAttachTarget
{
  /**
   * Attaches an object to this attach target.
   *
   * @param engine the engine.
   * @param target the attach target.
   * @param other the object to be attached.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `other` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_INVALID_OBJECT when `other` cannot be attached to `target`.
   * @throws VO_NOT_COMPILED when `other` implements the `voICompilable`
   *   interface and has to be compiled prior to being attached.
   * @throws VO_LIMIT_EXCEEDED when the number of attach references to `other`
   *   would exceed `65535`.
   */
  bool (VO_MSE_CALL *attach)(voEngine engine, voEngineObject target, voEngineObject other);

  /**
   * Detaches an object from this attach target.
   *
   * @param engine the engine.
   * @param target the attach target.
   * @param other the object to be detached.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `other` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_INVALID_OBJECT when `other` is not attached to `target`.
   */
  bool (VO_MSE_CALL *detach)(voEngine engine, voEngineObject target, voEngineObject other);

  /**
   * Returns the number of objects attached to this attach target.
   *
   * @param engine the engine.
   * @param target the attach target.
   *
   * @return the number of objects on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   */
  int32_t (VO_MSE_CALL *getAttachedCount)(voEngine engine, voEngineObject target);

  /**
   * Returns the object attached at the specified index.
   *
   * @note The order in which the objects are returned does not necessarily
   *   correspond to the order in which they were attached.
   *
   * @param engine the engine.
   * @param target the attach target.
   * @param index the index of the requested attached object.
   *
   * @return the attached object on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_LIMIT_EXCEEDED when the number of instances of the specified
   *   type would exceed the allowed limit (please remember that the limit can
   *   be shared by several types).
   */
  voEngineObject (VO_MSE_CALL *getAttachedAt)(voEngine engine, voEngineObject target, int32_t index);
}
voIAttachTarget;


// -- ICompilable --------------------------------------------------------------

/**
 * Functions composing the `voICompilable` interface.
 */
typedef struct _voICompilable
{
  /**
   * Tells whether or not the target object is compiled.
   *
   * @param engine the engine.
   * @param target the target object.
   *
   * @return `VO_YES` or `VO_NO` on success, otherwise `VO_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   */
  voYesNo (VO_MSE_CALL *isCompiled)(voEngine engine, voEngineObject target);

  /**
   * Compiles the target object.
   *
   * @param engine the engine.
   * @param target the target object.
   * @param notifyProgressFunc an optional pointer to a `voNotifyProgressFunc`
   *   function.
   * @param userParam an optional pointer to a user defined parameter that will
   *   be passed back to `notifyProgressFunc`.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_INVALID_STATE when `target` can't be compiled.
   * @throws VO_CANCELED when the operation has been canceled.
   *
   * @see voNotifyProgressFunc
   */
  bool (VO_MSE_CALL *compile)(voEngine engine, voEngineObject target, voNotifyProgressFunc notifyProgressFunc, void* userParam);

  /**
   * Decompiles the target object.
   *
   * @param engine the engine.
   * @param target the target object.
   * @param notifyProgressFunc an optional pointer to a `voNotifyProgressFunc`
   *   function.
   * @param userParam an optional pointer to a user defined parameter that will
   *   be passed back to `notifyProgressFunc`.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_INVALID_STATE when `target` can't be decompiled.
   * @throws VO_MODIFICATION_ACCESS_DENIED when `target` is in use by another
   *   object (see \ref voIAttachTarget).
   * @throws VO_CANCELED when the operation has been canceled.
   *
   * @see voNotifyProgressFunc
   */
  bool (VO_MSE_CALL *decompile)(voEngine engine, voEngineObject target, voNotifyProgressFunc notifyProgressFunc, void* userParam);
}
voICompilable;


// -- Input --------------------------------------------------------------------

/**
 * `voInput` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voIInput}
 * {@implementingTypes voFloatStructuredInput}
 */
typedef voEngineObject voInput;

/**
 * Functions composing the `voIInput` interface.
 *
 * {@implementingTypes voInput}
 */
typedef struct _voIInput
{
  /**
   * Clears the input.
   *
   * @param engine the engine.
   * @param target the target input.
   * @param keepMemory when `true`, tells the `target` input that it is allowed
   *   to keep its internal memory for later use.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   */
  bool (VO_MSE_CALL *clear)(voEngine engine, voEngineObject target, bool keepMemory);
}
voIInput;


// -- InputCharacter -----------------------------------------------------------

/**
 * `voInputCharacter` reference type. This is a character with alternates and
 * associated probabilities taking part in the input to a recognizer.
 *
 * {@extends voEngineObject}
 * {@implements voIInputCharacter}
 */
typedef voEngineObject voInputCharacter;

/**
 * Functions composing the `voIInputCharacter` interface.
 *
 * This interface exposes the public methods of the `voInputCharacter` type.
 *
 * {@implementingTypes voInputCharacter}
 */
typedef struct _voIInputCharacter
{
  /**
   * Clears this input character.
   *
   * @param engine the engine.
   * @param target the target input character.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voInputCharacter`.
   */
  bool (VO_MSE_CALL *clear)(voEngine engine, voInputCharacter target);

  /**
   * Adds an alternate to this input character.
   *
   * @param engine the engine.
   * @param target the target input character.
   * @param charset the charset used to encode the alternate string.
   * @param string the alternate string.
   * @param probability the probability associated with the alternate.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voInputCharacter`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `string` or its `bytes` member is not
   *   readable.
   * @throws VO_INVALID_ARGUMENT when `string` is the empty string.
   * @throws VO_INVALID_ARGUMENT when `string` contains more than one Unicode
   *   Grapheme Cluster.
   * @throws VO_INVALID_ARGUMENT when `probability` is not in the valid
   *   `[0.0, 1.0]` range.
   * @throws VO_INVALID_STATE when `string` contains a spacing character and the
   *   target character is not empty.
   */
  bool (VO_MSE_CALL *addAlternate)(voEngine engine, voInputCharacter target, voCharset charset, const voString* string, float probability);
}
voIInputCharacter;

#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voInputCharacter` instance.

 *
 * @param engine the engine.
 *
 * @return the newly created `voInputCharacter` instance on success, otherwise
 *   `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_LIMIT_EXCEEDED when the number of `voInputCharacter` instances
 *   would exceed the allowed limit (please remember that the limit can be
 *   shared by several types).
 */
VO_INLINE voInputCharacter voCreateInputCharacter(voEngine engine)
{
  return voCreateObject(engine, VO_InputCharacter);
}
#endif // end of: #ifndef VO_NO_FUNCTIONS


// -- InputString --------------------------------------------------------------

/**
 * `voInputString` reference type. This is a string with alternates and
 * associated probabilities taking part in the input to a recognizer.
 *
 * {@extends voEngineObject}
 * {@implements voIInputString}
 * @since 8.1_0
 */
typedef voEngineObject voInputString;

/**
 * Functions composing the `voIInputString` interface.
 *
 * This interface exposes the public methods of the `voInputString` type.
 *
 * {@implementingTypes voInputString}
 * @see VO_IInputString, VO_InputString.
 * @since 8.1_0
 */
typedef struct _voIInputString
{
  /**
   * Clears this input string.
   *
   * @param engine the engine.
   * @param target the target input string.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voInputString`.
   */
  bool (VO_MSE_CALL *clear)(voEngine engine, voInputString target);

  /**
   * Adds an alternate to this input string.
   *
   * @param engine the engine.
   * @param target the target input string.
   * @param charset the charset used to encode the alternate string.
   * @param string the alternate string.
   * @param probability the probability associated with the alternate.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voInputString`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `string` or its `bytes` member is not
   *   readable.
   * @throws VO_INVALID_ARGUMENT when `string` is the empty string.
   * @throws VO_INVALID_ARGUMENT when `probability` is not in the valid
   *   `[0.0, 1.0]` range.
   */
  bool (VO_MSE_CALL *addAlternate)(voEngine engine, voInputString target, voCharset charset, const voString* alternateString, float alternateProbability);
}
voIInputString;

#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voInputString` instance.
 *
 * @param engine the engine.
 *
 * @return the newly created `voInputString` instance on success, otherwise
 *   `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_LIMIT_EXCEEDED when the number of `voInputString` instances
 *   would exceed the allowed limit (please remember that the limit can be
 *   shared by several types).
 */
VO_INLINE voInputString voCreateInputString(voEngine engine)
{
  return voCreateObject(engine, VO_InputString);
}
#endif // end of: #ifndef VO_NO_FUNCTIONS


// -- StructuredInput ----------------------------------------------------------

/**
 * Lists the input unit types.
 */
typedef enum _voInputUnitType
{
  VO_CHAR = 2 << 24,    /**< Character input unit. */
  VO_WORD,              /**< Word input unit. */
  VO_SINGLE_LINE_TEXT,  /**< Single-line text input unit. */
  VO_MULTI_LINE_TEXT,   /**< Multi-line text input unit. */

  _voInputType_FORCE_INT_SIZE = 0x7FFFFFFF
}
voInputUnitType;


/**
 * `voStructuredInput` reference type.
 *
 * {@extends voInput}
 * {@subTypes voFloatStructuredInput}
 * {@implements voIStructuredInput}
 */
typedef voInput voStructuredInput;

/**
 * Functions composing the `voIStructuredInput` interface.
 *
 * This interface exposes the public methods of the `voStructuredInput` type.
 *
 * @note As the C language does not provide any mechanism that could be used to
 *   simulate inheritance, inherited members are simply copied from the base
 *   structure.
 *
 * {@extends voIInput}
 * {@implementingTypes voStructuredInput}
 */
typedef struct _voIStructuredInput
{
  /** {@inheritDoc} */
  bool (VO_MSE_CALL *clear)(voEngine engine, voInput target, bool keepMemory);

  /**
   * Starts an input unit.
   *
   * @param engine the engine.
   * @param target the target structured input.
   * @param type the type of input unit.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voStructuredInput`.
   * @throws VO_INVALID_ARGUMENT when `type` is invalid.
   */
  bool (VO_MSE_CALL *startInputUnit)(voEngine engine, voStructuredInput target, voInputUnitType type);

  /**
   * Ends an input unit.
   *
   * @param engine the engine.
   * @param target the target structured input.
   * @param type the type of input unit.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voStructuredInput`.
   * @throws VO_INVALID_ARGUMENT when `type` is invalid.
   * @throws VO_UNMATCHED_INPUT_UNIT when there is no matching start input unit.
   */
  bool (VO_MSE_CALL *endInputUnit)(voEngine engine, voStructuredInput target, voInputUnitType type);

  /**
   * Adds a text string to the current input unit.
   *
   * @param engine the engine.
   * @param target the target structured input.
   * @param charset the charset used to encode the string.
   * @param string the string data.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voStructuredInput`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `string` or its `bytes` member is not
   *   readable.
   * @throws VO_INVALID_ARGUMENT when `string` is empty.
   * @throws VO_INVALID_ARGUMENT when `string` has a bidi control character.
   * @throws VO_INVALID_STATE when the current input unit is a character input
   *   unit and is not empty or the string to be added contains more than one
   *   Unicode Grapheme Cluster.
   */
  bool (VO_MSE_CALL *addString)(voEngine engine, voStructuredInput target, voCharset charset, const voString* string);
}
voIStructuredInput;


/**
 * `voFloatStructuredInput` reference type.
 *
 * {@extends voStructuredInput}
 * {@implements voIFloatStructuredInput}
 */
typedef voStructuredInput voFloatStructuredInput;

/**
 * Functions composing the `voIFloatStructuredInput` interface.
 *
 * This interface exposes the public methods of the `voFloatStructuredInput`
 * type.
 *
 * @note As the C language does not provide any mechanism that could be used to
 *   simulate inheritance, inherited members are simply copied from the base
 *   structure.
 *
 * {@extends voIStructuredInput}
 * {@implementingTypes voFloatStructuredInput}
 */
typedef struct _voIFloatStructuredInput
{
  /** {@inheritDoc} */
  bool (VO_MSE_CALL *clear)(voEngine engine, voInput target, bool keepMemory);
  /** {@inheritDoc} */
  bool (VO_MSE_CALL *startInputUnit)(voEngine engine, voStructuredInput target, voInputUnitType type);
  /** {@inheritDoc} */
  bool (VO_MSE_CALL *endInputUnit)(voEngine engine, voStructuredInput target, voInputUnitType type);
  /** {@inheritDoc} */
  bool (VO_MSE_CALL *addString)(voEngine engine, voStructuredInput target, voCharset charset, const voString* string);

  /**
   * Sets the resolution of the coordinates in dpi. This is the resolution of
   * the grid of integer values in the chosen coordinate system. For example,
   * if you use floating-point pixel coordinates you must give the screen
   * resolution, no matter the resolution of the capturing device.
   *
   * @param engine the engine.
   * @param target the target structured input.
   * @param resolution the coordinate resolution.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a
   *   `voFloatStructuredInput`.
   * @throws VO_INVALID_ARGUMENT when `resolution <= 0`.
   */
  bool (VO_MSE_CALL *setCoordinateResolution)(voEngine engine, voFloatStructuredInput target, float resolution);

  /**
   * Defines a guide box for the current input unit.
   *
   * @param engine the engine.
   * @param target the target structured input.
   * @param x the coordinate of the box origin (corner) along the x axis.
   * @param y the coordinate of the box origin (corner) along the y axis.
   * @param width the width of the guide box.
   * @param height the height of the guide box.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a
   *   `voFloatStructuredInput`.
   * @throws VO_INVALID_ARGUMENT when `width <= 0`.
   * @throws VO_INVALID_ARGUMENT when `height <= 0`.
   */
  bool (VO_MSE_CALL *setGuideBox)(voEngine engine, voFloatStructuredInput target, float x, float y, float width, float height);

  /**
   * Defines guide lines with a single baseline for the current input unit.
   *
   * @param engine the engine.
   * @param target the target structured input.
   * @param baselinePos the position of the baseline along the y axis.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a
   *   `voFloatStructuredInput`.
   */
  bool (VO_MSE_CALL *setGuideLines_1)(voEngine engine, voFloatStructuredInput target, float baselinePos);

  /**
   * Defines guide lines with a baseline and a midline for the current input
   * unit.
   *
   * @param engine the engine.
   * @param target the target structured input.
   * @param baselinePos the position of the baseline along the y axis.
   * @param toMidline the distance from the baseline to the midline.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a
   *   `voFloatStructuredInput`.
   * @throws VO_INVALID_ARGUMENT when `toMidline <= 0`.
   */
  bool (VO_MSE_CALL *setGuideLines_2)(voEngine engine, voFloatStructuredInput target, float baselinePos, float toMidline);

  /**
   * Defines guide lines with baseline, midline, ascender and descender lines
   * for the current input unit.
   *
   * @param engine the engine.
   * @param target the target structured input.
   * @param baselinePos the position of the baseline along the y axis.
   * @param toMidline the distance from the baseline to the midline.
   * @param toAscender the distance from the baseline to the ascender line.
   * @param toDescender the distance from the baseline to the descender line.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a
   *   `voFloatStructuredInput`.
   * @throws VO_INVALID_ARGUMENT when `toMidline <= 0`.
   * @throws VO_INVALID_ARGUMENT when `toAscender <= toMidline`.
   * @throws VO_INVALID_ARGUMENT when `toDescender <= 0`.
   */
  bool (VO_MSE_CALL *setGuideLines_3)(voEngine engine, voFloatStructuredInput target, float baselinePos, float toMidline, float toAscender, float toDescender);

  /**
   * Defines multiple guides with only baselines for the current input unit.
   *
   * @param engine the engine.
   * @param target the target structured input.
   * @param guideCount the number of guides.
   * @param firstBaselinePos the position of the first baseline along the y
   *   axis.
   * @param gap the distance between two consecutive baselines.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a
   *   `voFloatStructuredInput`.
   * @throws VO_INVALID_ARGUMENT when `count <= 1`.
   * @throws VO_INVALID_ARGUMENT when `gap <= 0`.
   */
  bool (VO_MSE_CALL *setMultipleGuideLines_1)(voEngine engine, voFloatStructuredInput target, int32_t guideCount, float firstBaselinePos, float gap);

  /**
   * Defines multiple guides with baselines and midlines for the current input
   * unit.
   *
   * @param engine the engine.
   * @param target the target structured input.
   * @param guideCount the number of guides.
   * @param firstBaselinePos the position of the first baseline along the y
   *   axis.
   * @param gap the distance between two consecutive baselines.
   * @param toMidline the distance from the baseline to the midline.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a
   *   `voFloatStructuredInput`.
   * @throws VO_INVALID_ARGUMENT when `count <= 1`.
   * @throws VO_INVALID_ARGUMENT when `gap <= 0`.
   * @throws VO_INVALID_ARGUMENT when `toMidline <= 0`.
   * @throws VO_INVALID_ARGUMENT when `toMidline >= gap`.
   */
  bool (VO_MSE_CALL *setMultipleGuideLines_2)(voEngine engine, voFloatStructuredInput target, int32_t guideCount, float firstBaselinePos, float gap, float toMidline);

  /**
   * Defines multiple guides with baselines, midlines, ascender and descender
   * lines for the current input unit.
   *
   * @param engine the engine.
   * @param target the target structured input.
   * @param guideCount the number of guides.
   * @param firstBaselinePos the position of the first baseline along the y
   *   axis.
   * @param gap the distance between two consecutive baselines.
   * @param toMidline the distance from the baseline to the midline.
   * @param toAscender the distance from the baseline to the ascender line.
   * @param toDescender the distance from the baseline to the descender line.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a
   *   `voFloatStructuredInput`.
   * @throws VO_INVALID_ARGUMENT when `count <= 1`.
   * @throws VO_INVALID_ARGUMENT when `gap <= 0`.
   * @throws VO_INVALID_ARGUMENT when `toMidline <= 0`.
   * @throws VO_INVALID_ARGUMENT when `toMidline >= gap`.
   * @throws VO_INVALID_ARGUMENT when `toAscender <= toMidline`.
   * @throws VO_INVALID_ARGUMENT when `toDescender <= 0`.
   * @throws VO_INVALID_ARGUMENT when `gap - toAscender <= toDescender`.
   */
  bool (VO_MSE_CALL *setMultipleGuideLines_3)(voEngine engine, voFloatStructuredInput target, int32_t guideCount, float firstBaselinePos, float gap, float toMidline, float toAscender, float toDescender);

  /**
   * Adds a pen stroke to the current input unit.
   *
   * @param engine the engine.
   * @param target the target structured input.
   * @param x a pointer to the first x value.
   * @param xByteStride the stride in bytes between each x value.
   * @param y a pointer to the first y value.
   * @param yByteStride the stride in bytes between each y value.
   * @param pointCount the number of points.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a
   *   `voFloatStructuredInput`.
   * @throws VO_INVALID_STATE when the current input unit is not a character,
   *   word, or text input unit.
   * @throws VO_INVALID_ARGUMENT when `xByteStride == 0`.
   * @throws VO_INVALID_ARGUMENT when `yByteStride == 0`.
   * @throws VO_INVALID_ARGUMENT when `pointCount <= 0`.
   * @throws VO_INVALID_USER_BUFFER when the combination of `x`, `xByteStride`,
   *   `y`, `yByteStride` and `pointCount` leads to invalid memory access.
   * @throws VO_LIMIT_EXCEEDED when `pointCount > 32767`.
   */
  bool (VO_MSE_CALL *addStroke)(voEngine engine, voFloatStructuredInput target, const float* x, size_t xByteStride, const float* y, size_t yByteStride, int32_t pointCount);

  /**
   * Adds a pen stroke to the current input unit.
   *
   * @param engine the engine.
   * @param target the target structured input.
   * @param x a pointer to the first x value.
   * @param xByteStride the stride in bytes between each x value.
   * @param y a pointer to the first y value.
   * @param yByteStride the stride in bytes between each y value.
   * @param pointCount the number of points.
   * @param strokeOrder an integer that indicates the real temporal order of the
   *   stroke in a sequence. The stroke order may be used only if you set it for
   *   all the strokes in the input.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a
   *   `voFloatStructuredInput`.
   * @throws VO_INVALID_STATE when the current input unit is not a character,
   *   word, or text input unit.
   * @throws VO_INVALID_ARGUMENT when `xByteStride == 0`.
   * @throws VO_INVALID_ARGUMENT when `yByteStride == 0`.
   * @throws VO_INVALID_ARGUMENT when `pointCount == 0`.
   * @throws VO_INVALID_USER_BUFFER when the combination of `x`, `xByteStride`,
   *   `y`, `yByteStride` and `pointCount` leads to invalid memory access.
   * @throws VO_LIMIT_EXCEEDED when `pointCount > 32767`.
   * @throws VO_INVALID_ARGUMENT when `strokeOrder < 0`.
   */
  bool (VO_MSE_CALL *addStroke2)(voEngine engine, voFloatStructuredInput target, const float* x, size_t xByteStride, const float* y, size_t yByteStride, int32_t pointCount, int32_t strokeOrder);

  /**
   * Adds a character with respect to its position within the current input
   * unit. If the position of the character is unknown, the `x`, `y`, `width`,
   * and `height` values must all be set to `0`.
   *
   * @param engine the engine.
   * @param target the target structured input.
   * @param character the character to be added.
   * @param x the origin of the character bounding box along the x axis.
   * @param y the origin of the character bounding box along the y axis.
   * @param width the width of the character bounding box.
   * @param height the height of the character bounding box.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a
   *   `voFloatStructuredInput`.
   * @throws VO_NO_SUCH_OBJECT when the `character` reference is invalid.
   * @throws VO_INVALID_OBJECT when `character` is not a `voInputCharacter`.
   * @throws VO_INVALID_ARGUMENT when `character` contains no alternate.
   * @throws VO_INVALID_ARGUMENT when `character` contains a space and the
   *   current input unit is a word input unit.
   * @throws VO_INVALID_ARGUMENT when `character` contains a line feed alternate
   *   and the current input unit is a word or a single line text input unit.
   * @throws VO_INVALID_ARGUMENT when `character` is a bidi control character.
   * @throws VO_INVALID_ARGUMENT when `width < 0`.
   * @throws VO_INVALID_ARGUMENT when `height < 0`.
   * @throws VO_INVALID_ARGUMENT when `width` or `height` is equal to `0` and
   *   when `x`, `y`, `width`, and `height` are not all equal to `0`.
   * @throws VO_INVALID_STATE when the current input unit is a non empty
   *   character input.
   *
   * @since 4.5_4
   */
  bool (VO_MSE_CALL *addCharacter)(voEngine engine, voFloatStructuredInput target, voInputCharacter character, float x, float y, float width, float height);

  /**
   * Adds a text string to the current input unit.
   *
   * @param engine the engine.
   * @param target the target structured input.
   * @param string the string to be added.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voStructuredInput`.
   * @throws VO_NO_SUCH_OBJECT when the `string` reference is invalid.
   * @throws VO_INVALID_OBJECT when `string` is not a `voInputString`.
   * @throws VO_INVALID_ARGUMENT when `string` contains no alternate.
   *
   * @since 8.1_0
   */
  bool (VO_MSE_CALL *addString2)(voEngine engine, voFloatStructuredInput target, voInputString string);
}
voIFloatStructuredInput;

#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voFloatStructuredInput` instance.
 *
 * @param engine the engine.
 *
 * @return the newly created `voFloatStructuredInput` instance on success,
 *   otherwise `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_LIMIT_EXCEEDED when the number of `voFloatStructuredInput`
 *   instances would exceed the allowed limit (please remember that the limit
 *   can be shared by several types).
 */
VO_INLINE voFloatStructuredInput voCreateFloatStructuredInput(voEngine engine)
{
  return voCreateObject(engine, VO_FloatStructuredInput);
}
#endif // end of: #ifndef VO_NO_FUNCTIONS


// --- IInputConsumer ----------------------------------------------------------

/**
 * Functions composing the `voIInputConsumer` interface. An input consumer is
 * a special input that consumes another input to produce any desired result.
 * As it is itself an input, it can become the source of another consumer.
 *
 * {@extends voIInput}
 */
typedef struct _voIInputConsumer
{
  /**
   * Clears this input consumer, in other words clears any produced result.
   *
   * @param engine the engine.
   * @param target the target input consumer.
   * @param keepMemory when `true`, tells the `target` consumer that it is
   *   allowed to keep its internal memory for later use.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   */
  bool (VO_MSE_CALL *clear)(voEngine engine, voEngineObject target, bool keepMemory);

  /**
   * Sets the source of this input consumer.
   *
   * @note You can remove the current source from the consumer by giving a
   *   `NULL` source to this function.
   *
   * @param engine the engine.
   * @param target the target input consumer.
   * @param source the input source.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_NO_SUCH_OBJECT when the `source` reference is invalid.
   * @throws VO_INVALID_OBJECT when `source` does not implement this interface.
   */
  bool (VO_MSE_CALL *setSource)(voEngine engine, voEngineObject target, voEngineObject source);

  /**
   * Returns the source associated with this input consumer.
   *
   * @param engine the engine.
   * @param target the target input consumer.
   *
   * @return the source if any, otherwise `NULL`.
   *   When `NULL`, use `voGetError()` to figure out whether an error occurred.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_LIMIT_EXCEEDED when the number of instances of the specified type
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   */
  voEngineObject (VO_MSE_CALL *getSource)(voEngine engine, voEngineObject target);
}
voIInputConsumer;


// -- IRunnable ----------------------------------------------------------------

/**
 * Functions composing the `voIRunnable` interface.
 */
typedef struct _voIRunnable
{
  /**
   * Runs this object.
   *
   * @param engine the engine.
   * @param target the runnable target.
   * @param notifyProgressFunc an optional pointer to a `voNotifyProgressFunc`
   *   function.
   * @param userParam an optional pointer to a user defined parameter that will
   *   be passed back to `notifyProgressFunc`.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_INVALID_CALLBACK when `notifyProgressFunc` is invalid.
   * @throws VO_INVALID_STATE when `target` is not in a state where the `run`
   *   function can be invoked.
   * @throws VO_CANCELED when the operation has been canceled.
   */
  bool (VO_MSE_CALL *run)(voEngine engine, voEngineObject target, voNotifyProgressFunc notifyProgressFunc, void* userParam);
}
voIRunnable;


// -- HistoryManager -----------------------------------------------------------


/**
 * `voHistoryManager` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voIHistoryManager}
 */
typedef voEngineObject voHistoryManager;

/**
 * Functions composing the `voIHistoryManager` interface.
 *
 * This interface exposes the public methods of the `voHistoryManager` type.
 *
 * {@implementingTypes voHistoryManager}
 */
typedef struct _voIHistoryManager
{
  /**
   * Tells whether one can call `undo()`.
   *
   * @param engine the engine.
   * @param target the target history manager.
   *
   * @return `VO_YES` or `VO_NO` on success, otherwise `VO_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voHistoryManager`.
   */
  voYesNo (VO_MSE_CALL *canUndo)(voEngine engine, voHistoryManager target);

  /**
   * Undo the last action.
   *
   * When undo is not possible, performs no action and returns `true`.
   *
   * @param engine the engine.
   * @param target the target history manager.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voHistoryManager`.
   * @throws VO_INVALID_STATE when there is a pending transation.
   */
  bool (VO_MSE_CALL *undo)(voEngine engine, voHistoryManager target);

  /**
   * Tells whether one can call `redo()`.
   *
   * @param engine the engine.
   * @param target the target history manager.
   *
   * @return `VO_YES` or `VO_NO` on success, otherwise `VO_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voHistoryManager`.
   */
  voYesNo (VO_MSE_CALL *canRedo)(voEngine engine, voHistoryManager target);

  /**
   * Redo the last action reverted by `undo()`.
   *
   * When `redo()` is not possible, perform no action and return `true`.
   *
   * @param engine the engine.
   * @param target the target history manager.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voHistoryManager`.
   * @throws VO_INVALID_STATE when there is a pending transation.
   */
  bool (VO_MSE_CALL *redo)(voEngine engine, voHistoryManager target);

  /**
   * Indicates the start of a transaction.
   *
   * When `undo()` or `redo()` are called, a transaction is considered as one
   * action only.
   *
   * Call `commit()` to end the transaction.

   * If no transaction is ongoing, every action is considered as a transaction.
   *
   * @param engine the engine.
   * @param target the target history manager.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voHistoryManager`.
   */
  bool (VO_MSE_CALL *startTransaction)(voEngine engine, voHistoryManager target);

  /**
   * Indicates the end of a transaction.
   *
   * The actions performed since `startTransaction()` was called are considered
   * as a single one, as far as `undo()` and `redo()` are concerned.
   *
   * @param engine the engine.
   * @param target the target history manager.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voHistoryManager`.
   * @throws VO_INVALID_STATE when no transaction has been started.
   *
   * @see startTransaction
   */
  bool (VO_MSE_CALL *commit)(voEngine engine, voHistoryManager target);

  /**
   * Cancels an ongoing transaction.
   *
   * Undoes all actions performed since `startTransaction()` was called.
   *
   * @param engine the engine.
   * @param target the target history manager.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voHistoryManager`.
   */
  bool (VO_MSE_CALL *rollback)(voEngine engine, voHistoryManager target);

  /**
   * Returns the current undo stack depth.
   *
   * @param engine the engine.
   * @param target the target history manager.
   *
   * @return the undo stack depth on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voHistoryManager`.
   */
  int32_t (VO_MSE_CALL *getUndoStackDepth)(voEngine engine, voHistoryManager target);

  /**
   * Returns the maximum undo stack depth.
   *
   * @param engine the engine.
   * @param target the target history manager.
   *
   * @return the maximum undo stack depth on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voHistoryManager`.
   */
  int32_t (VO_MSE_CALL *getMaximumUndoStackDepth)(voEngine engine, voHistoryManager target);

  /**
   * Sets the maximum undo stack depth.
   *
   * @param engine the engine.
   * @param target the target history manager.
   * @param value the maximum depth of the undo stack. `0` means unlimited, use
   *   with care.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voHistoryManager`.
   * @throws VO_INVALID_ARGUMENT when `value < 0`.
   */
  bool (VO_MSE_CALL *setMaximumUndoStackDepth)(voEngine engine, voHistoryManager target, int32_t value);

  /**
   * Returns the factor applied to maximum history depth that gives the
   * threshold above which automatic purge will be triggered.
   *
   * @param engine the engine.
   * @param target the target history manager.
   *
   * @return the auto purge factor on success, otherwise `-1`;
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voHistoryManager`.
   */
  float (VO_MSE_CALL *getAutoPurgeFactor)(voEngine engine, voHistoryManager target);

  /**
   * Sets the factor applied to the maximum history depth that gives the
   * threshold above which automatic purge will be triggered.
   *
   * @param engine the engine.
   * @param target the target history manager.
   * @param value the purge factor.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voHistoryManager`.
   * @throws VO_INVALID_ARGUMENT when `value < 1.0`.
   */
  bool (VO_MSE_CALL *setAutoPurgeFactor)(voEngine engine, voHistoryManager target, float value);

  /**
   * Prevents undo of past action(s).
   *
   * @param engine the engine.
   * @param target the target history manager.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voHistoryManager`.
   */
  bool (VO_MSE_CALL *preventUndo)(voEngine engine, voHistoryManager target);

  /**
   * Indicates the end of a transaction.
   *
   * The actions performed since `startTransaction()` was called are grouped
   * under a single "ghost" transaction, i.e. it will be grouped with previous
   * regular commit when an `undo()` or a `redo()` command is called.
   *
   * @note "ghost" commits are visible only from the history manager they have
   *   been declared to: a "ghost" commit created at Page history manager level
   *   will be considered as a regular commit from the corresponding Document
   *   history manager.
   *
   * @param engine the engine.
   * @param target the target history manager.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voHistoryManager`.
   * @throws VO_INVALID_STATE when no transaction has been started.
   */
  bool (VO_MSE_CALL *commitAsGhost)(voEngine engine, voHistoryManager target);
}
voIHistoryManager;

/**
 * Functions composing the `voIVersionable` interface.
 *
 * A versionable object (an object that implement this interface) has an history
 * manager that records the history of changes and allows undo / redo.
 */
typedef struct _voIVersionable
{
  /**
   * Returns the history manager that controls this object's history of changes.
   *
   * @param engine the engine.
   * @param target the target object.
   *
   * @return the history manager if any, otherwise `NULL`.
   *   When `NULL`, use `voGetError()` to figure out whether an error occurred.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_LIMIT_EXCEEDED when the number of `voHistoryManager` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   */
  voHistoryManager (VO_MSE_CALL *getHistoryManager)(voEngine engine, voEngineObject target);

  /**
   * Acquires exclusive access to this object by the current thread.
   *
   * @note This should be used whenever a versionable object is accessed from
   *   several threads, to ensure modifications do not collide.
   *
   * @param engine the engine.
   * @param target the target object.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   */
  bool (VO_MSE_CALL *lock)(voEngine engine, voEngineObject target);

  /**
   * Releases exclusive access to this object by the current thread.
   *
   * @note This should be used whenever a versionable object is accessed from
   *   several threads, to ensure modifications do not collide.
   *
   * @param engine the engine.
   * @param target the target object.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   */
  bool (VO_MSE_CALL *unlock)(voEngine engine, voEngineObject target);
}
voIVersionable;


// -- UserObject ---------------------------------------------------------------

/**
 * `voUserObject` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voIUserObject}
 */
typedef voEngineObject voUserObject;

/**
 * Callback called when a user object is about to be destroyed.
 *
 * @param engine the engine.
 * @param target the user object that is about to be destroyed.
 */
typedef void (VO_MSE_CALL *voUserObjectOnDestroyCallback)(voEngine engine, voUserObject target);

/**
 * Holds the parameters required to create a new `voUserObject` instance.
 *
 * @see voUserObject, voCreateUserObject
 */
typedef struct _voUserObjectInitializer
{
  /**
   * An application side pointer that will be made available via the
   * `voIUserObject::getUserParam()` method.
   */
  void* userParam;

  /**
   * An application side function that will be called when the user object is
   * about to be destroyed.
   *
   * @param engine the engine.
   * @param target the user object that is about to be destroyed.
   */
  void (VO_MSE_CALL *onDestroy)(voEngine engine, voUserObject target);
}
voUserObjectInitializer;

#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voUserObject` instance.
 *
 * @param engine the engine.
 * @param userParam an application side pointer that will be bound to the user
 *   object.
 * @param callback the application side function that will be called when the
 *   user object is about to be destroyed.
 *
 * @return the newly created `voUserObject` instance on success, otherwise
 *   `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_LIMIT_EXCEEDED when the number of `voUserObject` instances would
 *   exceed the allowed limit (please remember that the limit can be shared by
 *   several types).
 */
VO_INLINE voUserObject voCreateUserObject(voEngine engine,
                                          void* userParam,
                                          voUserObjectOnDestroyCallback callback)
{
  voUserObjectInitializer initializer = { userParam, callback };
  return voCreateObjectEx(engine, VO_UserObject, &initializer, sizeof(initializer));
}
#endif // end of: #ifndef VO_NO_FUNCTIONS

/**
 * Functions composing the `voIUserObject` interface.
 *
 * This interface exposes the public methods of the `voUserObject` type.
 *
 * {@implementingTypes voUserObject}
 */
typedef struct _voIUserObject
{
  /**
   * Returns the application side pointer associated with the user object.
   *
   * @param engine the engine.
   * @param target the target user object.
   *
   * @return the application side pointer.
   *   When `NULL`, use `voGetError()` to figure out whether an error occurred.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voUserObject`.
   */
  void* (VO_MSE_CALL *getUserParam)(voEngine engine, voUserObject target);

  /**
   * Sets the application side pointer associated with the user object.
   * @param engine the engine.
   * @param target the target user object.
   * @param newUserParam the new application side pointer.
   *
   * @return the previous application side pointer.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voUserObject`.
   */
  void* (VO_MSE_CALL *setUserParam)(voEngine engine, voUserObject target, void* newUserParam);

  /**
   * Registers implementation of the specified interface.
   *
   * @param engine the engine.
   * @param target the target user object.
   * @param interfaceId the interface identifier.
   * @param interfaceFunctions the pointer to the interface function table.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voUserObject`.
   * @throws VO_NO_SUCH_INTERFACE when `interfaceId` does not correspond to an
   *   interface.
   * @throws VO_INVALID_ARGUMENT when `interfaceFunctions` is `NULL`.
   * @throws VO_INVALID_ARGUMENT when the interface is not implementable by user
   *   objects. The documentation of the interface should state it.
   */
  bool (VO_MSE_CALL *registerInterface)(voEngine engine, voUserObject target,
                                        voTypeId interfaceId,
                                        const void* interfaceFunctions);
}
voIUserObject;


#endif // end of: #ifndef VO_FUNCTIONS_ONLY

#endif // end of: #ifndef C_MYSCRIPTENGINE_H
