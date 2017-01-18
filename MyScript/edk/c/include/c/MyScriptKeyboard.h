#ifndef C_MYSCRIPTKEYBOARD_H
#define C_MYSCRIPTKEYBOARD_H 0x08000000
/**
 * @file c/MyScriptKeyboard.h
 * Native interface to the MyScript Keyboard service.
 */

#include "MyScriptEngine.h"


// -- types and interfaces -----------------------------------------------------

/**
 * Lists the MyScript Keyboard types.
 */
enum VO_KEYBOARD_T
{
  /**
   * `voKeyboardInput` type identifier.
   *
   * @see voKeyboard
   */
  VO_KeyboardInput = 9500,

  /**
   * `voKeyboardLayout` type identifier.
   *
   * @see voKeyboardLayout
   */
  VO_KeyboardLayout
};

/**
 * Lists the MyScript Keyboard interfaces.
 */
enum VO_KEYBOARD_I
{
  /**
   * `voIKeyboardInput` interface identifier.
   *
   * @see voIKeyboardInput
   */
  VO_IKeyboardInput = 9550,

  /**
   * `voIKeyboardLayout` interface identifier.
   *
   * @see voIKeyboardLayout
   */
  VO_IKeyboardLayout
};


// -- KeyboardInput ------------------------------------------------------------

/**
 * `voKeyboardInput` reference type.
 *
 * {@implements voIKeyboardInput}
 * {@extends voInput}
 */
typedef voEngineObject voKeyboardInput;

#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voKeyboardInput` instance.
 *
 * @param engine the engine.
 *
 * @return the newly created `voKeyboardInput` instance on success, otherwise
 *   `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_LIMIT_EXCEEDED when the number of `voKeyboardInput` instances
 *   would exceed the allowed limit (please remember that the limit can be
 *   shared by several types).
 */
VO_INLINE voKeyboardInput voCreateKeyboardInput(voEngine engine)
{
  return voCreateObject(engine, VO_KeyboardInput);
}
#endif // end of: #ifndef VO_NO_FUNCTIONS

/**
 * Lists the different capitalization modes.
 */
typedef enum _voKeyboardInputCapitalization
{
  VO_KEYBOARD_INPUT_CAPITALIZATION_AUTO,      /**< Auto. */
  VO_KEYBOARD_INPUT_CAPITALIZATION_TITLECASE, /**< Titlecase. */
  VO_KEYBOARD_INPUT_CAPITALIZATION_UPPERCASE  /**< Uppercase. */
}
voKeyboardInputCapitalization;

/**
 * Functions composing the `voIKeyboardInput` interface.
 *
 * This interface exposes the public methods of the `voKeyboardInput` type.
 *
 * {@implementingTypes voKeyboardInput}
 * @see VO_IKeyboardInput, VO_KeyboardInput
 */
typedef struct _voIKeyboardInput
{
  /**
   * Clears this input.
   *
   * @param engine the engine.
   * @param target the target input.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voKeyboardInput`.
   */
  bool (VO_MSE_CALL *clear)(voEngine engine, voKeyboardInput target);

  /**
   * Sends a pointer down event to this input.
   *
   * @param engine the engine.
   * @param target the target input.
   * @param x the x coordinate of the event.
   * @param y the y coordinate of the event.
   * @param t the timestamp of the event.
   * @param capitalization the capitalization mode.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voKeyboardInput`.
   * @throws VO_INVALID_ARGUMENT when `capitalization` is invalid.
   */
  bool (VO_MSE_CALL *pointerDown)(voEngine engine, voKeyboardInput target, float x, float y, int64_t t, voKeyboardInputCapitalization capitalization);

  /**
   * Sends a pointer move event to this input.
   *
   * @param engine the engine.
   * @param target the target input.
   * @param x the x coordinate of the event.
   * @param y the y coordinate of the event.
   * @param t the timestamp of the event.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voKeyboardInput`.
   * @throws VO_INVALID_STATE when `target` received no pointer down event.
   */
  bool (VO_MSE_CALL *pointerMove)(voEngine engine, voKeyboardInput target, float x, float y, int64_t t);

  /**
   * Sends a pointer up event to this input.
   *
   * @param engine the engine.
   * @param target the target input.
   * @param x the x coordinate of the event.
   * @param y the y coordinate of the event.
   * @param t the timestamp of the event.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voKeyboardInput`.
   * @throws VO_INVALID_STATE when `target` received no pointer down event.
   */
  bool (VO_MSE_CALL *pointerUp)(voEngine engine, voKeyboardInput target, float x, float y, int64_t t);

  /**
   * Adds a text string to this input.
   *
   * @param engine the engine.
   * @param target the target input.
   * @param charset the charset used to encode the string.
   * @param string the string data.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voKeyboardInput`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `string` or its `bytes` member is not
   *   readable.
   * @throws VO_INVALID_ARGUMENT when `string` is empty.
   */
  bool (VO_MSE_CALL *addString)(voEngine engine, voStructuredInput target, voCharset charset, const voString* string);

  /**
   * Adds a character to this input.
   *
   * @param engine the engine.
   * @param target the target input.
   * @param character the character to be added.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voKeyboardInput`.
   * @throws VO_NO_SUCH_OBJECT when the `character` reference is invalid.
   * @throws VO_INVALID_OBJECT when `character` is not a `voInputCharacter`.
   * @throws VO_INVALID_ARGUMENT when `character` contains no alternate.
   */
  bool (VO_MSE_CALL *addCharacter)(voEngine engine, voKeyboardInput target, voInputCharacter character);

  /**
   * Ends the transliteration sequence for the current symbol.
   *
   * @note The next input event will start the transliteration of another
   *   symbol.
   * @note This is API is specific to `zh_CN` stroke transliteration.
   *
   * @param engine the engine.
   * @param target the target input.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voKeyboardInput`.
   */
  bool (VO_MSE_CALL *addTransliterationBreak)(voEngine engine, voKeyboardInput target);

  /**
   * Adds a placeholder input event to this input that corresponds to any key
   * defined in the layout.
   *
   * @note This is API is specific to `zh_CN` stroke transliteration.
   *
   * @param engine the engine.
   * @param target the target input.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voKeyboardInput`.
   */
  bool (VO_MSE_CALL *addAnyKey)(voEngine engine, voKeyboardInput target);
}
voIKeyboardInput;


// -- KeyboardLayout -----------------------------------------------------------

/**
 * `voKeyboardLayout` reference type.
 *
 * {@implements voIKeyboardLayout}
 * {@extends voEngineObject}
 */
typedef voEngineObject voKeyboardLayout;

/**
 * Represents the different key flags.
 */
typedef enum _voKeyFlag
{
  VO_KF_NONE,
  VO_KF_DISABLE_CASE_ALTERNATES         = 1 << 0,
  VO_KF_DISABLE_ACCENTUATION_ALTERNATES = 1 << 1,
  VO_KF_DISABLE_RELATED_ALTERNATES      = 1 << 2
}
voKeyFlag;

/**
 * Functions composing the `voIKeyboardLayout` interface.
 *
 * This interface exposes the public methods of the `voKeyboardLayout` type.
 *
 * {@implementingTypes voKeyboardLayout}
 * @see VO_IKeyboardLayout, VO_KeyboardLayout
 */
typedef struct _voIKeyboardLayout
{
  /**
   * Adds a key to this layout.
   *
   * @param engine the engine.
   * @param target the target layout.
   * @param character the character (along with its alternates) on the key to be
   *   added.
   * @param x the origin of the key bounding box along the x axis.
   * @param y the origin of the key bounding box along the y axis.
   * @param width the width of the key bounding box.
   * @param height the height of the key bounding box.
   * @param flags flags controlling the behavior of the key.
   */
  bool (VO_MSE_CALL *addKey)(voEngine engine, voKeyboardLayout target, voInputCharacter character, float x, float y, float width, float height, VO_FLAGS_T(voKeyFlag) flags);
}
voIKeyboardLayout;

#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voKeyboardLayout` instance.
 *
 * @param engine the engine.
 *
 * @return the newly created `voKeyboardLayout` instance on success, otherwise
 *   `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_LIMIT_EXCEEDED when the number of `voKeyboardLayout` instances
 *   would exceed the allowed limit (please remember that the limit can be
 *   shared by several types).
 */
VO_INLINE voKeyboardLayout voCreateKeyboardLayout(voEngine engine)
{
  return voCreateObject(engine, VO_KeyboardLayout);
}
#endif // end of: #ifndef VO_NO_FUNCTIONS

/**
 * Lists recognition result candidate flags for candidates recognized from a
 * `voKeyboardInput`.
 *
 * @see VO_CANDIDATE_FLAG.
 */
enum VO_KEYBOARD_CANDIDATE_FLAG
{
  /**
   * The candidate corresponds to the raw input.
   */
  VO_CANDIDATE_FLAG_KEYBOARD_RAW = 1 << 10,

  /**
   * The candidate corresponds to the input interpreted as fuzzy typing.
   */
  VO_CANDIDATE_FLAG_KEYBOARD_FUZZY = 1 << 11,

  /**
   * The candidate corresponds to the input interpreted as smooth typing.
   */
  VO_CANDIDATE_FLAG_KEYBOARD_SMOOTH = 1 << 12
};

#endif // end of: #ifndef C_MYSCRIPTKEYBOARD_H
