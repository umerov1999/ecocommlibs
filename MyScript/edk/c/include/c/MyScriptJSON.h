#ifndef C_MYSCRIPTJSON_H
#define C_MYSCRIPTJSON_H 0x08000000
/**
 * @file c/MyScriptJSON.h
 * Native interface to the MyScript JSON service. Please note that the
 * implementation of MyScript JSON is delivered as part of the MyScriptEngine
 * library.
 */

#include "MyScriptEngine.h"


// -- types and interfaces -----------------------------------------------------

/**
 * Lists the MyScript JSON types.
 */
enum VO_JSON_T
{
  /**
   * `voJson` type identifier.
   *
   * @see voJson
   */
  VO_Json = 3000,

  /**
   * `voJsonObjectIterator` type identifier.
   *
   * @see voJsonObjectIterator
   */
  VO_JsonObjectIterator
};


/**
 * Lists the MyScript JSON interfaces.
 */
enum VO_JSON_I
{
  /**
   * `voIJson` interface identifier.
   *
   * @see voIJson
   */
  VO_IJson = 3002,

  /**
   * `voIJsonObjectIterator` interface identifier.
   *
   * @see voIJsonObjectIterator
   */
  VO_IJsonObjectIterator
};


// -----------------------------------------------------------------------------

/**
 * `voJson` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voIJson}
 */
typedef voEngineObject voJson;

/**
 * `voJsonObjectIterator` reference type.
 *
 * {@extends voIterator}
 * {@implements voIJsonObjectIterator}
 */
typedef voIterator voJsonObjectIterator;

/**
 * Represents the type of a JSON value.
 */
typedef enum _voJsonType
{
  VO_JSON_ERR = -1, /**< Error marker. */

  VO_JSON_NULL,     /**< The JSON value `null`. */
  VO_JSON_STRING,   /**< A JSON string value. */
  VO_JSON_NUMBER,   /**< A JSON number value. */
  VO_JSON_BOOLEAN,  /**< A JSON boolean value. */
  VO_JSON_OBJECT,   /**< A JSON object value. */
  VO_JSON_ARRAY,    /**< A JSON array value. */

  _voJsonType_FORCE_INT_SIZE = 0x7FFFFFFF
}
voJsonType;

/**
 * Functions composing the `voIJson` interface.
 *
 * {@implementingTypes voJson}
 */
typedef struct _voIJson
{
  /**
   * Returns type of a JSON value.
   *
   * @param engine the engine.
   * @param target the target JSON value.
   *
   * @return the type of the JSON value on success, otherwise `VO_JSON_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voJson`.
   */
  voJsonType (VO_MSE_CALL *getType)(voEngine engine, voJson target);

  /**
   * Creates a new JSON null value.
   *
   * @param engine the engine.
   *
   * @return the newly created JSON null value on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_LIMIT_EXCEEDED when the number of `voJson` instances would
   *   exceed the allowed limit (please remember that the limit can be shared by
   *   several types).
   */
  voJson (VO_MSE_CALL *createNull)(voEngine engine);

  /**
   * Creates a new JSON string.
   *
   * @param engine the engine.
   * @param charset the charset used to encode parameter strings.
   * @param value the string value.
   *
   * @return the newly created JSON string on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `value` or its `bytes` member is not
   *   readable.
   * @throws VO_LIMIT_EXCEEDED when the number of `voJson` instances would
   *   exceed the allowed limit (please remember that the limit can be shared by
   *   several types).
   */
  voJson (VO_MSE_CALL *createString)(voEngine engine, voCharset charset, const voString* value);

  /**
   * Creates a new JSON number.
   *
   * @param engine the engine.
   * @param value the number value.
   *
   * @return the newly created JSON number on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_LIMIT_EXCEEDED when the number of `voJson` instances would
   *   exceed the allowed limit (please remember that the limit can be shared by
   *   several types).
   * @throws VO_INVALID_ARGUMENT when `value` is infinity or not a number.
   */
  voJson (VO_MSE_CALL *createNumber)(voEngine engine, double value);

  /**
   * Creates a new JSON boolean.
   *
   * @param engine the engine.
   * @param value the boolean value.
   *
   * @return the newly created JSON boolean on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_LIMIT_EXCEEDED when the number of `voJson` instances would
   *   exceed the allowed limit (please remember that the limit can be shared by
   *   several types).
   */
  voJson (VO_MSE_CALL *createBoolean)(voEngine engine, bool value);

  /**
   * Creates a new JSON object.
   *
   * @param engine the engine.
   *
   * @return the newly created JSON object on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_LIMIT_EXCEEDED when the number of `voJson` instances would
   *   exceed the allowed limit (please remember that the limit can be shared by
   *   several types).
   */
  voJson (VO_MSE_CALL *createObject)(voEngine engine);

  /**
   * Creates a new JSON array.
   *
   * @param engine the engine.
   *
   * @return the newly created JSON array on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_LIMIT_EXCEEDED when the number of `voJson` instances would
   *   exceed the allowed limit (please remember that the limit can be shared by
   *   several types).
   */
  voJson (VO_MSE_CALL *createArray)(voEngine engine);

  /**
   * Parses the given input string as JSON.
   *
   * @param engine the engine.
   * @param charset the charset used to encode parameter strings.
   * @param input the input JSON string.
   *
   * @return the JSON value on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `input` or its `bytes` member is not
   *   readable.
   * @throws VO_INVALID_ARGUMENT when `input` is not valid JSON.
   * @throws VO_LIMIT_EXCEEDED when the number of `voJson` instances would
   *   exceed the allowed limit (please remember that the limit can be shared by
   *   several types).
   */
  voJson (VO_MSE_CALL *parse)(voEngine engine, voCharset charset, const voString* input);

  /**
   * Returns the value of a JSON string.
   *
   * @param engine the engine.
   * @param target the target JSON string.
   * @param charset the charset used to encode parameter strings.
   * @param value the recipient for the value.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voJson` of type
   *   `VO_JSON_STRING`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `value` or its `bytes` member is not
   *   writable.
   */
  bool (VO_MSE_CALL *getStringValue)(voEngine engine, voJson target, voCharset charset, voString* value);

  /**
   * Returns the value of a JSON number.
   *
   * @param engine the engine.
   * @param target the target JSON number.
   * @param value the recipient for the value.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voJson` of type
   *   `VO_JSON_NUMBER`.
   * @throws VO_INVALID_USER_BUFFER when `value` is not writable.
   */
  bool (VO_MSE_CALL *getNumberValue)(voEngine engine, voJson target, double* value);

  /**
   * Returns the value of a JSON boolean.
   *
   * @param engine the engine.
   * @param target the target JSON boolean.
   * @param value the recipient for the value.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voJson` of type
   *   `VO_JSON_BOOLEAN`.
   * @throws VO_INVALID_USER_BUFFER when `value` is not writable.
   */
  bool (VO_MSE_CALL *getBooleanValue)(voEngine engine, voJson target, bool* value);

  /**
   * Returns an iterator over the entries of a JSON object.
   *
   * @note MyScript JSON keeps the insertion order for entries iteration.
   *
   * @param engine the engine.
   * @param target the target JSON object.
   *
   * @return the iterator on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voJson` of type
   *   `VO_JSON_OBJECT`.
   * @throws VO_LIMIT_EXCEEDED when the number of `voJsonObjectIterator`
   *   instances would exceed the allowed limit (please remember that the limit
   *   can be shared by several types).
   */
  voJsonObjectIterator (VO_MSE_CALL *getObjectEntries)(voEngine engine, voJson target);

  /**
   * Returns the value associated with the specified key within a JSON object.
   *
   * @param engine the engine.
   * @param target the target JSON object.
   * @param charset the charset used to encode parameter strings.
   * @param key the key to lookup.
   *
   * @return the value associated with the key if any, otherwise `NULL`.
   *   When `NULL`, use `voGetError()` to figure out whether an error occurred.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voJson` of type
   *   `VO_JSON_OBJECT`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `key` or its `bytes` member is not
   *   readable.
   * @throws VO_INVALID_ARGUMENT when `key` is the empty string.
   */
  voJson (VO_MSE_CALL *getObjectEntryValue)(voEngine engine, voJson target, voCharset charset, const voString* key);

  /**
   * Puts a new association in a JSON object. Any former value is erased.
   *
   * @param engine the engine.
   * @param target the target JSON object.
   * @param charset the charset used to encode parameter strings.
   * @param key the key to which the value should be associated.
   * @param value the JSON value.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voJson` of type
   *   `VO_JSON_OBJECT`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `key` or its `bytes` member is not
   *   readable.
   * @throws VO_INVALID_ARGUMENT when `key` is the empty string.
   * @throws VO_NO_SUCH_OBJECT when the `value` reference is invalid.
   * @throws VO_INVALID_OBJECT when `value` is not a `voJson`.
   */
  bool (VO_MSE_CALL *putObjectEntry)(voEngine engine, voJson target, voCharset charset, const voString* key, voJson value);

  /**
   * Removes an association from a JSON object.
   *
   * @param engine the engine.
   * @param target the target JSON object.
   * @param charset the charset used to encode parameter strings.
   * @param key the key of the association to remove.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voJson` of type
   *   `VO_JSON_OBJECT`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `key` or its `bytes` member is not
   *   readable.
   * @throws VO_INVALID_ARGUMENT when `key` is the empty string.
   */
  bool (VO_MSE_CALL *removeObjectEntry)(voEngine engine, voJson target, voCharset charset, const voString* key);

  /**
   * Returns the number of elements within a JSON array.
   *
   * @param engine the engine.
   * @param target the target JSON array.
   *
   * @return the number of elements in the array on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voJson` of type
   *   `VO_JSON_ARRAY`.
   */
  int32_t (VO_MSE_CALL *getArrayLength)(voEngine engine, voJson target);

  /**
   * Returns the element at the specified index in a JSON array.
   *
   * @param engine the engine.
   * @param target the target JSON array.
   * @param index the index of the element to be retrieved.
   *
   * @return the element at the specified index on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voJson` of type
   *   `VO_JSON_ARRAY`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   */
  voJson (VO_MSE_CALL *getArrayValueAt)(voEngine engine, voJson target, int32_t index);

  /**
   * Inserts an element to a JSON array.
   *
   * If the given index is valid then the value is placed at the given position
   * and the existing values starting from the index are shifted.
   *
   * If the given index is `-1` the value is inserted at the end of the array.
   *
   * @param engine the engine.
   * @param array the target JSON array.
   * @param index the index at which to place the new value. Use `-1` to append
   *   at the end of the array.
   * @param value the JSON value to insert.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voJson` of type
   *   `VO_JSON_ARRAY`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds or not `-1`.
   * @throws VO_NO_SUCH_OBJECT when the `value` reference is invalid.
   * @throws VO_INVALID_OBJECT when `value` is not a `voJson`.
   */
  bool (VO_MSE_CALL *insertArrayValueAt)(voEngine engine, voJson array, int32_t index, voJson value);

  /**
   * Removes the element at the specified position from a JSON array.
   *
   * @param engine the engine.
   * @param array the target JSON array.
   * @param index the index of the element to be removed.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voJson` of type
   *   `VO_JSON_ARRAY`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   */
  bool (VO_MSE_CALL *removeArrayValueAt)(voEngine engine, voJson array, int32_t index);

  /**
   * Creates a new JSON string and fills it with the string representation of a
   * timestamp.
   *
   * Timestamp string representation used is `dd-mm-yyyy hh:mm:ss.ms`.
   *
   * @param engine the engine.
   * @param timestamp the timestamp, expressed in microseconds elapsed since
   *   the EPOCH.
   *
   * @return the newly created JSON string on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_ARGUMENT when `timestamp` is negative.
   * @throws VO_LIMIT_EXCEEDED when the number of `voJson` instances would
   *   exceed the allowed limit (please remember that the limit can be shared by
   *   several types).
   */
  voJson (VO_MSE_CALL *createStringFromTimestamp)(voEngine engine, int64_t timestamp);

  /**
   * Returns the value of a JSON string, parsed as a timestamp string.
   *
   * Timestamp string representation supported is `dd-mm-yyyy hh:mm:ss.ms`.
   *
   * @param engine the engine.
   * @param target the target JSON string.
   * @param timestamp the recipient for the timestamp, expressed in microseconds
   *   elapsed since the EPOCH.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voJson` of type
   *   `VO_JSON_STRING`.
   * @throws VO_INVALID_USER_BUFFER when `timestamp` is not writable.
   * @throws VO_INVALID_STATE when `target` does not match the timestamp format.
   */
  bool (VO_MSE_CALL *getStringValueAsTimestamp)(voEngine engine, voJson target, int64_t* timestamp);

  /**
   * Creates a new JSON integer.
   *
   * @param engine the engine.
   * @param value the integer value.
   *
   * @return the newly created JSON integer on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_LIMIT_EXCEEDED when the number of `voJson` instances would
   *   exceed the allowed limit (please remember that the limit can be shared by
   *   several types).
   */
  voJson (VO_MSE_CALL *createInteger)(voEngine engine, int64_t value);

  /**
   * Returns the value of a JSON integer.
   *
   * @param engine the engine.
   * @param target the target JSON integer.
   * @param value the recipient for the value.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voJson` of type
   *   `VO_JSON_NUMBER`.
   * @throws VO_INVALID_USER_BUFFER when `value` is not writable.
   */
  bool (VO_MSE_CALL *getIntegerValue)(voEngine engine, voJson target, int64_t* value);
}
voIJson;

/**
 * Functions composing the `voIJsonObjectIterator` interface.
 *
 * This interface exposes the public methods of the `voJsonObjectIterator` type.
 *
 * @note As the C language does not provide any mechanism that could be used to
 *   simulate inheritance, inherited members are simply copied from the base
 *   structure.
 *
 * {@extends voIIterator}
 * {@implementingTypes voJsonObjectIterator}
 */
typedef struct _voIJsonObjectIterator
{
  /** {@inheritDoc} */
  voYesNo (VO_MSE_CALL *isAtEnd)(voEngine engine, voIterator target);
  /** {@inheritDoc} */
  bool (VO_MSE_CALL *next)(voEngine engine, voIterator target);
  /** {@inheritDoc} */
  voYesNo (VO_MSE_CALL *equals)(voEngine engine, voIterator target, voIterator other);

  /**
   * Returns the key of the JSON object entry at the current step of the
   * iteration.
   *
   * @param engine the engine.
   * @param target the target JSON object iterator.
   * @param charset the charset used to encode parameter strings.
   * @param key the recipient for the key.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voJsonObjectIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `key` or its `bytes` member is not
   *   writable.
   */
  bool (VO_MSE_CALL *getKey)(voEngine engine, voJsonObjectIterator target, voCharset charset, voString* key);

  /**
   * Returns the value of the JSON object entry at the current step of the
   * iteration.
   *
   * @param engine the engine.
   * @param target the target JSON object iterator.
   *
   * @return the JSON value on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voJsonObjectIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   * @throws VO_LIMIT_EXCEEDED when the number of `voJson` instances would
   *   exceed the allowed limit (please remember that the limit can be shared by
   *   several types).
   */
  voJson (VO_MSE_CALL *getValue)(voEngine engine, voJsonObjectIterator target);
}
voIJsonObjectIterator;

#endif // end of: #ifndef C_MYSCRIPTJSON_H
