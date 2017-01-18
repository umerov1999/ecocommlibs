#ifndef C_MYSCRIPTMATH_H
#define C_MYSCRIPTMATH_H 0x08000000
/**
 * @file c/MyScriptMath.h
 * Native interface to the MyScript Math service.
 */

#include "MyScriptEngine.h"


// -- error codes --------------------------------------------------------------

/**
 * Lists the MyScript Math errors.
 */
enum VO_MATH_ERR
{
  /**
   * Unable to complete the requested operation because a
   * `voMathAlphabetKnowledge` is missing.
   *
   * @see voMathAlphabetKnowledge
   */
  VO_MISSING_MATH_ALPHABET_KNOWLEDGE = VO_INVALID_STATE | (5 << 16),

  /**
   * Unable to complete the requested operation because a `voMathGrammar` is
   * missing.
   *
   * @see voMathGrammar
   */
  VO_MISSING_MATH_GRAMMAR
};


// -- types and interfaces -----------------------------------------------------

/**
 * Lists the MyScript Math types.
 */
enum VO_MATH_T
{
  /**
   * `voMathDocument` type identifier.
   *
   * @see voMathDocument
   */
  VO_MathDocument = 5000,

  /**
   * `voMathAlphabetKnowledge` type identifier.
   *
   * @see voMathAlphabetKnowledge
   */
  VO_MathAlphabetKnowledge,

  /**
   * `voMathGrammar` type identifier.
   *
   * @see voMathGrammar
   */
  VO_MathGrammar,

  /**
   * `voMathRecognizer` type identifier.
   *
   * @see voMathRecognizer
   */
  VO_MathRecognizer,

  /**
   * `voMathNode` type identifier.
   *
   * @see voMathNode
   */
  VO_MathNode = 8001,

  /**
   * `voMathTerminalNode` type identifier.
   *
   * @see voMathTerminalNode
   */
  VO_MathTerminalNode = 5005,

  /**
   * `voMathNonTerminalNode` type identifier.
   *
   * @see voMathNonTerminalNode
   */
  VO_MathNonTerminalNode,

  /**
   * `voMathRuleNode` type identifier.
   *
   * @see voMathRuleNode
   */
  VO_MathRuleNode,

  /**
   * `voMathScratchOut` type identifier.
   *
   * @see voMathScratchOut
   *
   * @since 6.1_0
   */
  VO_MathScratchOut,

  /**
   * `voMathCellNode` type identifier.
   *
   * @see voMathCellNode
   *
   * @since 6.4_0
   */
  VO_MathCellNode,

  /**
   * `voMathBorderNode` type identifier.
   *
   * @see voMathBorderNode
   *
   * @since 6.4_0
   */
  VO_MathBorderNode,

  /**
   * `voMathTableNode` type identifier.
   *
   * @see voMathTableNode
   *
   * @since 6.4_0
   */
  VO_MathTableNode
};

/**
 * Lists the MyScript Math interfaces.
 */
enum VO_MATH_I
{
  /**
   * `voIMathDocument` interface identifier.
   *
   * @see voIMathDocument
   */
  VO_IMathDocument = 5100,

  /**
   * `voIMathAlphabet` interface identifier.
   *
   * @see voIMathAlphabet
   */
  VO_IMathAlphabet,

  /**
   * `voIMathGrammar` interface identifier.
   *
   * @see voIMathGrammar
   */
  VO_IMathGrammar,

  /**
   * `voIMathRecognizer` interface identifier.
   *
   * @see voIMathRecognizer
   */
  VO_IMathRecognizer,

  /**
   * `voIMathTerminalNode` interface identifier.
   *
   * @see voIMathTerminalNode
   */
  VO_IMathTerminalNode,

  /**
   * `voIMathNonTerminalNode` interface identifier.
   *
   * @see voIMathNonTerminalNode
   */
  VO_IMathNonTerminalNode,

  /**
   * `voIMathRuleNode` interface identifier.
   *
   * @see voIMathRuleNode
   */
  VO_IMathRuleNode,

  /**
   * `voIMathScratchOut` interface identifier.
   *
   * @see voIMathScratchOut
   *
   * @since 6.1_0
   */
  VO_IMathScratchOut,

  /**
   * `voIMathCellNode` interface identifier.
   *
   * @see voIMathCellNode
   *
   * @since 6.4_0
   */
  VO_IMathCellNode,

  /**
   * `voIMathBorderNode` interface identifier.
   *
   * @see voIMathBorderNode
   *
   * @since 6.4_0
   */
  VO_IMathBorderNode,

  /**
   * `voIMathTableNode` interface identifier.
   *
   * @see voIMathTableNode
   *
   * @since 6.4_0
   */
  VO_IMathTableNode,
};


// -- MathNode -----------------------------------------------------------------

/**
 * `voMathNode` reference type.
 *
 * {@extends voEngineObject}
 */
typedef voEngineObject voMathNode;

/**
 * {@typePropertySet MathNode}
 */
enum VO_MATH_NODE_TPROP
{
  /**
   * Represents the maximum number of `voMathNode` instances that can exist at
   * the same time in the user side of a MyScript enable application.
   *
   * The special value zero means no limit. Other values are adjusted to the
   * next multiple of 16. A value under the current count is permitted and will
   * constrain future resource creations.
   *
   * {@property uint32_t}
   */
  VO_USER_MATH_NODE_LIMIT = VO_USER_OBJ_LIMIT,

  /**
   * Gives the number of `voMathNode` instances that currently exist in the user
   * side of a MyScript enabled application.
   *
   * {@property read-only uint32_t}
   */
  VO_USER_MATH_NODE_COUNT = VO_USER_OBJ_COUNT
};


// -- MathScratchOut -----------------------------------------------------------

/**
 * `voMathScratchOut` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voIMathScratchOut}
 * @since 6.1_0
 */
typedef voEngineObject voMathScratchOut;


// -- MathDocument -------------------------------------------------------------

/**
 * `voMathDocument` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voIMathDocument}
 */
typedef voEngineObject voMathDocument;

/**
 * Functions composing the `voIMathDocument` interface.
 *
 * This interface exposes the public methods of the `voMathDocument` type.
 *
 * {@implementingTypes voMathDocument}
 */
typedef struct _voIMathDocument
{
  /**
   * Clears this document.
   *
   * @param engine the engine.
   * @param target the target document.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMathDocument`.
   */
  bool (VO_MSE_CALL *clear)(voEngine engine, voMathDocument target);

  /**
   * Adds a pen stroke to this document.
   *
   * @param engine the engine.
   * @param target the target document.
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
   * @throws VO_INVALID_OPERATION when `target` is not a `voMathDocument`.
   * @throws VO_INVALID_ARGUMENT when `xByteStride == 0`.
   * @throws VO_INVALID_ARGUMENT when `yByteStride == 0`.
   * @throws VO_INVALID_ARGUMENT when `pointCount <= 0`.
   * @throws VO_INVALID_USER_BUFFER when the combination of `x`, `xByteStride`,
   *   `y`, `yByteStride` and `pointCount` leads to invalid memory access.
   * @throws VO_LIMIT_EXCEEDED when `pointCount > 32767`.
   */
  bool (VO_MSE_CALL *addStroke)(voEngine engine, voMathDocument target, const float* x, size_t xByteStride, const float* y, size_t yByteStride, int32_t pointCount);

  /**
   * Adds character with respect to its position in this document.
   *
   * @param engine the engine.
   * @param target the target document.
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
   * @throws VO_INVALID_OPERATION when `target` is not a `voMathDocument`.
   * @throws VO_NO_SUCH_OBJECT when the `character` reference is invalid.
   * @throws VO_INVALID_OBJECT when `character` is not a `voInputCharacter`.
   * @throws VO_INVALID_ARGUMENT when `character` contains no alternate.
   * @throws VO_INVALID_ARGUMENT when `width < 0`.
   * @throws VO_INVALID_ARGUMENT when `height < 0`.
   */
  bool (VO_MSE_CALL *addCharacter)(voEngine engine, voMathDocument target, voInputCharacter character, float x, float y, float width, float height);

  /**
   * Deletes a component from this document.
   *
   * @param engine the engine.
   * @param target the target document.
   * @param index the index of the component to be deleted.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMathDocument`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   *
   * @since 5.0_0
   */
  bool (VO_MSE_CALL *deleteComponentAt)(voEngine engine, voMathDocument target, int32_t index);

  /**
   * Undeletes a component from this document.
   *
   * @param engine the engine.
   * @param target the target document.
   * @param index the index of the component to be undeleted.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMathDocument`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   *
   * @since 5.0_0
   */
  bool (VO_MSE_CALL *undeleteComponentAt)(voEngine engine, voMathDocument target, int32_t index);

  /**
   * Tells whether this document has an associated parse tree.
   *
   * @param engine the engine.
   * @param target the target document.
   *
   * @return `VO_YES` or `VO_NO` on success, otherwise `VO_ERR` on failure.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMathDocument`.
   */
  voYesNo (VO_MSE_CALL *hasParseTree)(voEngine engine, voMathDocument target);

  /**
   * Returns the root node of the parse tree after this document has been
   * processed.
   *
   * @param engine the engine.
   * @param target the target document.
   *
   * @return the root node on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMathDocument`.
   * @throws VO_INVALID_STATE when `target` has not been processed.
   * @throws VO_LIMIT_EXCEEDED when the number of `voMathNode` instances would
   *   exceed the allowed limit (please remember that the limit can be shared by
   *   several types).
   */
  voMathNode (VO_MSE_CALL *getParseTree)(voEngine engine, voMathDocument target);

  /**
   * Returns the number of active scratch outs after this document has been
   * processed.
   *
   * @param engine the engine.
   * @param target the target document.
   *
   * @return the number of active scratch outs on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMathDocument`.
   *
   * @since 6.1_0
   */
  int32_t (VO_MSE_CALL *getScratchOutCount)(voEngine engine, voMathDocument target);

  /**
   * Returns the scratch out at the specified index after this document has been
   * processed.
   *
   * @param engine the engine.
   * @param target the target document.
   * @param index the index of the scratch out to be retrieved.
   *
   * @return the scratch out on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMathDocument`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   *
   * @since 6.1_0
   */
  voMathScratchOut (VO_MSE_CALL *getScratchOutAt)(voEngine engine, voMathDocument target, int32_t index);
}
voIMathDocument;

#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voMathDocument` instance.
 *
 * @param engine the engine.
 *
 * @return the newly created `voMathDocument` instance on success, otherwise
 *   `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_LIMIT_EXCEEDED when the number of `voMathDocument` instances would
 *   exceed the allowed limit (please remember that the limit can be shared by
 *   several types).
 */
VO_INLINE voMathDocument voCreateMathDocument(voEngine engine)
{
  return voCreateObject(engine, VO_MathDocument);
}
#endif // end of: #ifndef VO_NO_FUNCTIONS


// -- MathAlphabetKnowledge ----------------------------------------------------

/**
 * `voMathAlphabetKnowledge` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voIMathAlphabet}
 */
typedef voEngineObject voMathAlphabetKnowledge;


// -- IMathAlphabet ------------------------------------------------------------

/**
 * Functions composing the `voIMathAlphabet` interface. The term "alphabet" here
 * denotes a finite set of characters.

 * {@implementingTypes voMathAlphabetKnowledge}
 */
typedef struct _voIMathAlphabet
{
  /**
   * Returns the number of characters in this alphabet.
   *
   * @param engine the engine.
   * @param target the target alphabet.
   *
   * @return the number of characters on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   */
  int32_t (VO_MSE_CALL *getCharacterCount)(voEngine engine, voEngineObject target);

  /**
   * Returns the character at the specified index.
   *
   * @param engine the engine.
   * @param target the target alphabet.
   * @param index the index of the character to be retrieved.
   * @param charset the charset used to encode parameter strings.
   * @param character the recipient for the character.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `character` or its `bytes` member is
   *   not writable.
   */
  bool (VO_MSE_CALL *getCharacterAt)(voEngine engine, voEngineObject target, int32_t index, voCharset charset, voString* character);
}
voIMathAlphabet;


// -- MathGrammar --------------------------------------------------------------

/**
 * Represents the different decimal separators.
 */
typedef enum _voMathDecimalSeparatorFlag
{
  MATH_DECIMAL_SEPARATOR_NONE,              /**< None. */
  MATH_DECIMAL_SEPARATOR_DOT    = 1 << 0,   /**< Dot. */
  MATH_DECIMAL_SEPARATOR_COMMA  = 1 << 1    /**< Comma. */
}
voMathDecimalSeparatorFlag;

/**
 * Represents different carry cancels.
 */
typedef enum _voMathCancelFlag
{
  MATH_CANCEL_NONE,                         /**< None. */
  MATH_CANCEL             = 1 << 0,         /**< Cancel. */
  MATH_BCANCEL            = 1 << 1,         /**< B(ack) cancel. */
  MATH_XCANCEL            = 1 << 2          /**< X cancel. */
}
voMathCancelFlag;

/**
 * Represents the different columnar operations.
 */
typedef enum _voMathOperationFlag
{
  MATH_OPERATION_ADDITION          = 1 << 0,  /**< Addition. */
  MATH_OPERATION_SUBTRACTION       = 1 << 1,  /**< Subtraction. */
  MATH_OPERATION_MULTIPLICATION    = 1 << 2,  /**< Multiplication. */
  MATH_OPERATION_DIVISION_ENGLISH  = 1 << 3,  /**< English division. */
  MATH_OPERATION_DIVISION_FRENCH   = 1 << 4,  /**< French division. */
  MATH_OPERATION_DIVISION_DUTCH    = 1 << 5,  /**< Dutch division. */
  MATH_OPERATION_DIVISION_INDIAN   = 1 << 6,  /**< Indian division. */
  MATH_OPERATION_DIVISION_GERMAN   = 1 << 7   /**< German division. */
}
voMathOperationFlag;

/**
 * `voMathGrammar` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voIMathGrammar}
 */
typedef voEngineObject voMathGrammar;

/**
 * Type of a symbol in a `voMathGrammar`.
 */
typedef voId voMathSymbol;

#define MATH_SYMBOL_NONE (voMathSymbol)-1

/**
 * Functions composing the `voIMathGrammar` interface.
 *
 * This interface exposes the public methods of the `voMathGrammar` type.
 *
 * {@implementingTypes voMathGrammar}
 */
typedef struct _voIMathGrammar
{
  /**
   * Creates a terminal symbol in this grammar.
   *
   * @param engine the engine.
   * @param target the target grammar.
   * @param charset the charset used to encode parameter strings.
   * @param name the name of the symbol.
   * @param value the terminal string.
   *
   * @return the symbol id on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMathGrammar`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `name` or its `bytes` member is not
   *   readable.
   * @throws VO_INVALID_ARGUMENT when `name` is the empty string.
   * @throws VO_INVALID_USER_BUFFER when `value` or its `bytes` member is not
   *   readable.
   * @throws VO_INVALID_ARGUMENT when `value` is the empty string.
   * @throws VO_LIMIT_EXCEEDED when `value` would exceed the allowed number of
   *   characters.
   */
  voMathSymbol (VO_MSE_CALL *createTerminalSymbol)(voEngine engine, voMathGrammar target, voCharset charset, voString* name, voString* value);

  /**
   * Creates a non terminal symbol in this grammar.
   *
   * @param engine the engine.
   * @param target the target grammar.
   * @param charset the charset used to encode parameter strings.
   * @param name the name of the symbol.
   *
   * @return the symbol id on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMathGrammar`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `name` or its `bytes` member is not
   *   readable.
   * @throws VO_INVALID_ARGUMENT when `name` is the empty string.
   */
  voMathSymbol (VO_MSE_CALL *createNonTerminalSymbol)(voEngine engine, voMathGrammar target, voCharset charset, voString* name);

  /**
   * Adds an identity rule to this grammar.
   *
   * @param engine the engine.
   * @param target the target grammar.
   * @param targetSymbol the id of target non terminal symbol.
   * @param sourceSymbol the id of source symbol.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMathGrammar`.
   * @throws VO_INVALID_ARGUMENT when `targetSymbol` is invalid.
   * @throws VO_INVALID_ARGUMENT when `sourceSymbol` is invalid.
   */
  bool (VO_MSE_CALL *addIdentityRule)(voEngine engine, voMathGrammar target, voMathSymbol targetSymbol, voMathSymbol sourceSymbol);

  /**
   * Adds an horizontal pair rule to this grammar.
   *
   * @param engine the engine.
   * @param target the target grammar.
   * @param targetSymbol the id of target non terminal symbol.
   * @param leftSymbol the id of left symbol.
   * @param rightSymbol the id of right symbol.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMathGrammar`.
   * @throws VO_INVALID_ARGUMENT when `targetSymbol` is invalid.
   * @throws VO_INVALID_ARGUMENT when `leftSymbol` is invalid.
   * @throws VO_INVALID_ARGUMENT when `rightSymbol` is invalid.
   */
  bool (VO_MSE_CALL *addHorizontalPairRule)(voEngine engine, voMathGrammar target,
                                            voMathSymbol targetSymbol, voMathSymbol leftSymbol, voMathSymbol rightSymbol);

  /**
   * Adds a fence rule to this grammar.
   *
   * @param engine the engine.
   * @param target the target grammar.
   * @param targetSymbol the id of target non terminal symbol.
   * @param sourceSymbol the id of source symbol.
   * @param leftSymbol the id of left terminal symbol.
   * @param rightSymbol the id of right terminal symbol.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMathGrammar`.
   * @throws VO_INVALID_ARGUMENT when `targetSymbol` is invalid.
   * @throws VO_INVALID_ARGUMENT when `sourceSymbol` is invalid.
   * @throws VO_INVALID_ARGUMENT when `leftSymbol` is invalid.
   * @throws VO_INVALID_ARGUMENT when `rightSymbol` is invalid.
   */
  bool (VO_MSE_CALL *addFenceRule)( voEngine engine, voMathGrammar target,
                                    voMathSymbol targetSymbol, voMathSymbol sourceSymbol, voMathSymbol leftSymbol, voMathSymbol rightSymbol);

  /**
   * Adds a fraction rule to this grammar.
   *
   * @param engine the engine.
   * @param target the target grammar.
   * @param targetSymbol the id of target non terminal symbol.
   * @param numeratorSymbol the id of numerator symbol.
   * @param denominatorSymbol the id of denominator symbol.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMathGrammar`.
   * @throws VO_INVALID_ARGUMENT when `targetSymbol` is invalid.
   * @throws VO_INVALID_ARGUMENT when `numeratorSymbol` is invalid.
   * @throws VO_INVALID_ARGUMENT when `denominatorSymbol` is invalid.
   */
  bool (VO_MSE_CALL *addFractionRule)(voEngine engine, voMathGrammar target,
                                      voMathSymbol targetSymbol, voMathSymbol numeratorSymbol, voMathSymbol denominatorSymbol);

  /**
   * Adds a square root rule to this grammar.
   *
   * @param engine the engine.
   * @param target the target grammar.
   * @param targetSymbol the id of target non terminal symbol.
   * @param sourceSymbol the id of source symbol.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMathGrammar`.
   * @throws VO_INVALID_ARGUMENT when `targetSymbol` is invalid.
   * @throws VO_INVALID_ARGUMENT when `sourceSymbol` is invalid.
   */
  bool (VO_MSE_CALL *addSqrtRule)(voEngine engine, voMathGrammar target,
                                  voMathSymbol targetSymbol, voMathSymbol sourceSymbol);

  /**
   * Adds a subscript rule to this grammar.
   *
   * @param engine the engine.
   * @param target the target grammar.
   * @param targetSymbol the id of target non terminal symbol.
   * @param sourceSymbol the id of source symbol.
   * @param indexSymbol the id of index symbol.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMathGrammar`.
   * @throws VO_INVALID_ARGUMENT when `targetSymbol` is invalid.
   * @throws VO_INVALID_ARGUMENT when `sourceSymbol` is invalid.
   * @throws VO_INVALID_ARGUMENT when `indexSymbol` is invalid.
   */
  bool (VO_MSE_CALL *addSubscriptRule)( voEngine engine, voMathGrammar target,
                                        voMathSymbol targetSymbol, voMathSymbol sourceSymbol, voMathSymbol indexSymbol);

  /**
   * Adds a superscript rule to this grammar.
   *
   * @param engine the engine.
   * @param target the target grammar.
   * @param targetSymbol the id of target non terminal symbol.
   * @param sourceSymbol the id of source symbol.
   * @param exponentSymbol the id of exponent symbol.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMathGrammar`.
   * @throws VO_INVALID_ARGUMENT when `targetSymbol` is invalid.
   * @throws VO_INVALID_ARGUMENT when `sourceSymbol` is invalid.
   * @throws VO_INVALID_ARGUMENT when `exponentSymbol` is invalid.
   */
  bool (VO_MSE_CALL *addSuperscriptRule)( voEngine engine, voMathGrammar target,
                                          voMathSymbol targetSymbol, voMathSymbol sourceSymbol, voMathSymbol exponentSymbol);

  /**
   * Adds a subsuperscript rule to this grammar.
   *
   * @param engine the engine.
   * @param target the target grammar.
   * @param targetSymbol the id of target non terminal symbol.
   * @param sourceSymbol the id of source symbol.
   * @param indexSymbol the id of index symbol.
   * @param exponentSymbol the id of exponent symbol.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMathGrammar`.
   * @throws VO_INVALID_ARGUMENT when `targetSymbol` is invalid.
   * @throws VO_INVALID_ARGUMENT when `sourceSymbol` is invalid.
   * @throws VO_INVALID_ARGUMENT when `indexSymbol` is invalid.
   * @throws VO_INVALID_ARGUMENT when `exponentSymbol` is invalid.
   */
  bool (VO_MSE_CALL *addSubsuperscriptRule)(voEngine engine, voMathGrammar target,
                                            voMathSymbol targetSymbol, voMathSymbol sourceSymbol, voMathSymbol indexSymbol, voMathSymbol exponentSymbol);

  /**
   * Adds an underscript rule to this grammar.
   *
   * @param engine the engine.
   * @param target the target grammar.
   * @param targetSymbol the id of target non terminal symbol.
   * @param sourceSymbol the id of source symbol.
   * @param bottomSymbol the id of bottom symbol.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMathGrammar`.
   * @throws VO_INVALID_ARGUMENT when `targetSymbol` is invalid.
   * @throws VO_INVALID_ARGUMENT when `sourceSymbol` is invalid.
   * @throws VO_INVALID_ARGUMENT when `bottomSymbol` is invalid.
   */
  bool (VO_MSE_CALL *addUnderscriptRule)( voEngine engine, voMathGrammar target,
                                          voMathSymbol targetSymbol, voMathSymbol sourceSymbol, voMathSymbol bottomSymbol);

  /**
   * Adds an overscript rule to this grammar.
   *
   * @param engine the engine.
   * @param target the target grammar.
   * @param targetSymbol the id of target non terminal symbol.
   * @param sourceSymbol the id of source symbol.
   * @param topSymbol the id of top symbol.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMathGrammar`.
   * @throws VO_INVALID_ARGUMENT when `targetSymbol` is invalid.
   * @throws VO_INVALID_ARGUMENT when `sourceSymbol` is invalid.
   * @throws VO_INVALID_ARGUMENT when `topSymbol` is invalid.
   */
  bool (VO_MSE_CALL *addOverscriptRule)(voEngine engine, voMathGrammar target,
                                        voMathSymbol targetSymbol, voMathSymbol sourceSymbol, voMathSymbol topSymbol);

  /**
   * Adds an underoverscript rule to this grammar.
   *
   * @param engine the engine.
   * @param target the target grammar.
   * @param targetSymbol the id of target non terminal symbol.
   * @param sourceSymbol the id of source symbol.
   * @param bottomSymbol the id of bottom symbol.
   * @param topSymbol the id of top symbol.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMathGrammar`.
   * @throws VO_INVALID_ARGUMENT when `targetSymbol` is invalid.
   * @throws VO_INVALID_ARGUMENT when `sourceSymbol` is invalid.
   * @throws VO_INVALID_ARGUMENT when `bottomSymbol` is invalid.
   * @throws VO_INVALID_ARGUMENT when `topSymbol` is invalid.
   */
  bool (VO_MSE_CALL *addUnderoverscriptRule)( voEngine engine, voMathGrammar target,
                                              voMathSymbol targetSymbol, voMathSymbol sourceSymbol, voMathSymbol bottomSymbol, voMathSymbol topSymbol);

  /**
   * Sets the start symbol of this grammar.
   *
   * @param engine the engine.
   * @param target the target grammar.
   * @param startSymbol the id of start symbol.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMathGrammar`.
   * @throws VO_INVALID_ARGUMENT when `startSymbol` is invalid.
   */
  bool (VO_MSE_CALL *setStartSymbol)(voEngine engine, voMathGrammar target, voMathSymbol startSymbol);

  /**
   * Adds a presuperscript rule to this grammar.
   *
   * @param engine the engine.
   * @param target the target grammar.
   * @param targetSymbol the id of target non terminal symbol.
   * @param sourceSymbol the id of source symbol.
   * @param exponentSymbol the id of exponent symbol.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMathGrammar`.
   * @throws VO_INVALID_ARGUMENT when `targetSymbol` is invalid.
   * @throws VO_INVALID_ARGUMENT when `sourceSymbol` is invalid.
   * @throws VO_INVALID_ARGUMENT when `exponentSymbol` is invalid.
   */
  bool (VO_MSE_CALL *addPresuperscriptRule)(voEngine engine, voMathGrammar target,
                                            voMathSymbol targetSymbol, voMathSymbol sourceSymbol, voMathSymbol exponentSymbol);

  /**
   * Adds a vertical pair rule to this grammar.
   *
   * @param engine the engine.
   * @param target the target grammar.
   * @param targetSymbol the id of target non terminal symbol.
   * @param topSymbol the id of top symbol.
   * @param bottomSymbol the id of bottom symbol.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMathGrammar`.
   * @throws VO_INVALID_ARGUMENT when `targetSymbol` is invalid.
   * @throws VO_INVALID_ARGUMENT when `topSymbol` is invalid.
   * @throws VO_INVALID_ARGUMENT when `bottomSymbol` is invalid.
   *
   * @since 6.0_0
   */
  bool (VO_MSE_CALL *addVerticalPairRule)(voEngine engine, voMathGrammar target,
                                          voMathSymbol targetSymbol, voMathSymbol topSymbol, voMathSymbol bottomSymbol);

   /**
   * Adds a left fence rule to this grammar.
   *
   * @param engine the engine.
   * @param target the target grammar.
   * @param targetSymbol the id of target non terminal symbol.
   * @param sourceSymbol the id of source symbol.
   * @param leftSymbol the id of left terminal symbol.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMathGrammar`.
   * @throws VO_INVALID_ARGUMENT when `targetSymbol` is invalid.
   * @throws VO_INVALID_ARGUMENT when `sourceSymbol` is invalid.
   * @throws VO_INVALID_ARGUMENT when `leftSymbol` is invalid.
   *
   * @since 6.0_0
   */
  bool (VO_MSE_CALL *addLeftFenceRule)( voEngine engine, voMathGrammar target,
                                        voMathSymbol targetSymbol, voMathSymbol sourceSymbol, voMathSymbol leftSymbol);

  /**
   * Adds a presubscript rule to this grammar.
   *
   * @param engine the engine.
   * @param target the target grammar.
   * @param targetSymbol the id of target non terminal symbol.
   * @param sourceSymbol the id of source symbol.
   * @param indexSymbol the id of index symbol.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMathGrammar`.
   * @throws VO_INVALID_ARGUMENT when `targetSymbol` is invalid.
   * @throws VO_INVALID_ARGUMENT when `sourceSymbol` is invalid.
   * @throws VO_INVALID_ARGUMENT when `indexSymbol` is invalid.
   *
   * @since 6.4_0
   */
  bool (VO_MSE_CALL *addPresubscriptRule)(voEngine engine, voMathGrammar target,
                                          voMathSymbol targetSymbol, voMathSymbol sourceSymbol, voMathSymbol indexSymbol);

  /**
   * Adds a columnar operation rule to this grammar.
   *
   * @param engine the engine.
   * @param target the target grammar.
   * @param targetSymbol the id of target non terminal symbol.
   * @param digitSymbol the id of digit terminal symbol.
   * @param carrySymbol the id of digit terminal symbol (or `MATH_SYMBOL_NONE`
   *   to disable carries).
   * @param decimalSeparatorFlags flags describing the decimal separator.
   * @param cancelFlags flags describing the carry cancels.
   * @param operationFlags flags describing the allowed operations.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMathGrammar`.
   * @throws VO_INVALID_ARGUMENT when `targetSymbol` is invalid.
   * @throws VO_INVALID_ARGUMENT when `digitSymbol` is invalid.
   * @throws VO_INVALID_ARGUMENT when `decimalSeparatorFlags` is not a valid
   *   combination of flags.
   * @throws VO_INVALID_ARGUMENT when `cancelFlags` is not a valid combination
   *   of flags.
   * @throws VO_INVALID_ARGUMENT when `operationFlags` is not a valid
   *   combination of flags.
   *
   * @since 6.4_0
   */
  bool (VO_MSE_CALL *addColumnarOperationRule)( voEngine engine, voMathGrammar target,
                                                voMathSymbol targetSymbol, voMathSymbol digitSymbol, voMathSymbol carrySymbol,
                                                VO_FLAGS_T(voMathDecimalSeparatorFlag) decimalSeparatorFlags,
                                                VO_FLAGS_T(voMathCancelFlag) cancelFlags,
                                                VO_FLAGS_T(voMathOperationFlag) operationFlags);

  /**
   * Adds a vertical list rule to this grammar.
   *
   * @param engine the engine.
   * @param target the target grammar.
   * @param targetSymbol the id of target non terminal symbol.
   * @param sourceSymbol the id of source symbol.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMathGrammar`.
   * @throws VO_INVALID_ARGUMENT when `targetSymbol` is invalid.
   * @throws VO_INVALID_ARGUMENT when `sourceSymbol` is invalid.
   *
   * @since 7.2_0
   */
  bool (VO_MSE_CALL *addVerticalListRule)(voEngine engine, voMathGrammar target, voMathSymbol targetSymbol, voMathSymbol sourceSymbol);

  /**
   * Adds a table rule to this grammar.
   *
   * @param engine the engine.
   * @param target the target grammar.
   * @param targetSymbol the id of target non terminal symbol.
   * @param sourceSymbol the id of source symbol.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMathGrammar`.
   * @throws VO_INVALID_ARGUMENT when `targetSymbol` is invalid.
   * @throws VO_INVALID_ARGUMENT when `sourceSymbol` is invalid.
   *
   * @since 7.3_4
   */
  bool (VO_MSE_CALL *addTableRule)(voEngine engine, voMathGrammar target, voMathSymbol targetSymbol, voMathSymbol sourceSymbol);
}
voIMathGrammar;

#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voMathGrammar` instance.
 *
 * @param engine the engine.
 *
 * @return the newly created `voMathGrammar` instance on success, otherwise
 *   `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_LIMIT_EXCEEDED when the number of `voMathGrammar` instances would
 *   exceed the allowed limit (please remember that the limit can be shared by
 *   several types).
 */
VO_INLINE voMathGrammar voCreateMathGrammar(voEngine engine)
{
  return voCreateObject(engine, VO_MathGrammar);
}
#endif // end of: #ifndef VO_NO_FUNCTIONS


// -- MathRecognizer -----------------------------------------------------------

/**
 * `voMathRecognizer` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voIAttachTarget, voIMathRecognizer}
 */
typedef voEngineObject voMathRecognizer;

/**
 * {@propertySet MathRecognizer}
 */
enum VO_MATH_RECOGNIZER_PROP
{
  /**
   * A value between `0.0f` and `1.0f` that adjusts the scratch out detection
   * sensitivity.
   *
   * {@property float}
   *
   * @note A value equal to `0.0f` disables scratch out detection.
   */
  VO_MATH_RECOGNIZER_SCRATCH_OUT_DETECTION_SENSITIVITY = (VO_MathRecognizer << 16)
};

/**
 * Functions composing the `voIMathRecognizer` interface.
 *
 * This interface exposes the public methods of the `voMathRecognizer` type.
 *
 * {@implementingTypes voMathRecognizer}
 */
typedef struct _voIMathRecognizer
{
  /**
   * Performs the recognition of a math document.
   *
   * @param engine the engine.
   * @param recognizer the target recognizer.
   * @param document the math document to be recognized.
   * @param notifyProgressFunc an optional pointer to a `voNotifyProgressFunc`
   *   function.
   * @param userParam an optional pointer to a user defined parameter that will
   *   be passed back to `notifyProgressFunc`.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMathRecognizer`.
   * @throws VO_NO_SUCH_OBJECT when the `document` reference is invalid.
   * @throws VO_INVALID_OBJECT when `document` is not a `voDocument`.
   * @throws VO_INVALID_CALLBACK when `notifyProgressFunc` is invalid.
   * @throws VO_INVALID_STATE when the `target` object is not in a state where
   *   the `process` function can be invoked.
   * @throws VO_MISSING_MATH_ALPHABET_KNOWLEDGE when `recognizer` has no
   *   attached `voMathAlphabetKnowledge`.
   * @throws VO_MISSING_MATH_GRAMMAR when `recognizer` has no attached
   *   `voMathGrammar`.
   * @throws VO_CANCELED when the operation has been canceled.
   */
  bool (VO_MSE_CALL *process)(voEngine engine, voMathRecognizer recognizer, voMathDocument document, voNotifyProgressFunc notifyProgressFunc, void* userParam);
}
voIMathRecognizer;

#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voMathRecognizer` instance.
 *
 * @param engine the engine.
 *
 * @return the newly created `voMathRecognizer` instance on success, otherwise
 *   `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_LIMIT_EXCEEDED when the number of `voMathRecognizer` instances
 *   would exceed the allowed limit (please remember that the limit can be
 *   shared by several types).
 */
VO_INLINE voMathRecognizer voCreateMathRecognizer(voEngine engine)
{
  return voCreateObject(engine, VO_MathRecognizer);
}
#endif // end of: #ifndef VO_NO_FUNCTIONS


// -- MathTerminalNode ---------------------------------------------------------

/**
 * `voMathTerminalNode` reference type.
 *
 * {@extends voMathNode}
 * {@implements voIMathTerminalNode}
 */
typedef voMathNode voMathTerminalNode;

/**
 * Represents a range in the input.
 */
typedef struct _voMathInputRange
{
  /**
   * Index of the component (stroke or character).
   */
  int32_t component;

  /**
   * Position of the first item within the component (eg, first point within a
   * stroke).
   */
  float firstItem;

  /**
   * Position of the last item within the component (eg, last point within a
   * stroke).
   */
  float lastItem;
}
voMathInputRange;

/**
 * Functions composing the `voIMathTerminalNode` interface.
 *
 * This interface exposes the public methods of the `voMathTerminalNode` type.
 *
 * {@implementingTypes voMathTerminalNode}
 */
typedef struct _voIMathTerminalNode
{
  /**
   * Returns the name of this terminal node.
   *
   * @param engine the engine.
   * @param target the target terminal node.
   * @param charset the charset used to encode parameter strings.
   * @param name the recipient for the name.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMathTerminalNode`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `name` or its `bytes` member is not
   *   writable.
   */
  bool (VO_MSE_CALL *getName)(voEngine engine, voMathTerminalNode target, voCharset charset, voString* name);

  /**
   * Returns the number of input ranges this terminal node covers.
   *
   * @param engine the engine.
   * @param target the target terminal node.
   *
   * @return the number of input ranges on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMathTerminalNode`.
   * @throws VO_INVALID_STATE when the portion of a MyScript Document associated
   *   with this terminal node has been modified, invalidating input ranges.
   */
  int32_t (VO_MSE_CALL *getInputRangeCount)(voEngine engine, voMathTerminalNode target);

  /**
   * Returns the input range at the specified index.
   *
   * @param engine the engine.
   * @param target the target terminal node.
   * @param index the index of the requested input range.
   * @param range the recipient for the input range.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMathTerminalNode`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_INVALID_USER_BUFFER when `range` is not writable.
   * @throws VO_INVALID_STATE when the portion of a MyScript Document associated
   *   with this terminal node has been modified, invalidating input ranges.
   */
  bool (VO_MSE_CALL *getInputRangeAt)(voEngine engine, voMathTerminalNode target, int32_t index, voMathInputRange* range);

  /**
   * Returns the number of candidates this terminal node contains.
   *
   * @param engine the engine.
   * @param target the target terminal node.
   *
   * @return the number of candidates on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMathTerminalNode`.
   */
  int32_t (VO_MSE_CALL *getCandidateCount)(voEngine engine, voMathTerminalNode target);

  /**
   * Returns the label of the candidate at the specified index.
   *
   * @param engine the engine.
   * @param target the target terminal node.
   * @param index the index of the candidate.
   * @param charset the charset used to encode parameter strings.
   * @param label the recipient for the label.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMathTerminalNode`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `label` or its `bytes` member is not
   *   writable.
   */
  bool (VO_MSE_CALL *getCandidateLabelAt)(voEngine engine, voMathTerminalNode target, int32_t index, voCharset charset, voString* label);

  /**
   * Returns the normalized recognition score of the candidate at the specified
   * index.
   *
   * @param engine the engine.
   * @param target the target terminal node.
   * @param index the index of the candidate.
   *
   * @return the normalized recognition score on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMathTerminalNode`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   */
  float (VO_MSE_CALL *getCandidateNormalizedRecognitionScoreAt)(voEngine engine, voMathTerminalNode target, int32_t index);

  /**
   * Returns the index of the best candidate from the current context in this
   * terminal node's top list (list of candidates).
   *
   * @param engine the engine.
   * @param target the target terminal node.
   *
   * @return the index of the best candidate on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMathTerminalNode`.
   * @throws VO_INVALID_STATE when `target` is not in a state where the selected
   *   candidate is available.
   */
  int32_t (VO_MSE_CALL *getSelectedCandidateIndex)(voEngine engine, voMathTerminalNode target);
}
voIMathTerminalNode;


// -- MathNonTerminalNode ------------------------------------------------------

/**
 * `voMathNonTerminalNode` reference type.
 *
 * {@extends voMathNode}
 * {@implements voIMathNonTerminalNode}
 */
typedef voMathNode voMathNonTerminalNode;

/**
 * Functions composing the `voIMathNonTerminalNode` interface.
 *
 * This interface exposes the public methods of the `voMathNonTerminalNode`
 * type.
 *
 * {@implementingTypes voMathNonTerminalNode}
 */
typedef struct _voIMathNonTerminalNode
{
  /**
   * Returns the name of this non terminal node.
   *
   * @param engine the engine.
   * @param target the target terminal node.
   * @param charset the charset used to encode parameter strings.
   * @param name the recipient for the name.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a
   *   `voMathNonTerminalNode`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `name` or its `bytes` member is not
   *   writable.
   */
  bool (VO_MSE_CALL *getName)(voEngine engine, voMathNonTerminalNode target, voCharset charset, voString* name);

  /**
   * Returns the number of input ranges this non terminal node covers.
   *
   * @param engine the engine.
   * @param target the target non terminal node.
   *
   * @return the number of input ranges on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a
   *   `voMathNonTerminalNode`.
   * @throws VO_INVALID_STATE when the portion of a MyScript Document associated
   *   with this non terminal node has been modified, invalidating input ranges.
   */
  int32_t (VO_MSE_CALL *getInputRangeCount)(voEngine engine, voMathNonTerminalNode target);

  /**
   * Returns the input range at the specified index.
   *
   * @param engine the engine.
   * @param target the target non terminal node.
   * @param index the index of the requested input range.
   * @param range the recipient for the input range.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a
   *   `voMathNonTerminalNode`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_INVALID_USER_BUFFER when `range` is not writable.
   * @throws VO_INVALID_STATE when the portion of a MyScript Document associated
   *   with this non terminal node has been modified, invalidating input ranges.
   */
  bool (VO_MSE_CALL *getInputRangeAt)(voEngine engine, voMathNonTerminalNode target, int32_t index, voMathInputRange* range);

  /**
   * Returns the number of candidates this non terminal node contains.
   *
   * @param engine the engine.
   * @param target the target non terminal node.
   *
   * @return the number of candidates on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a
   *   `voMathNonTerminalNode`.
   */
  int32_t (VO_MSE_CALL *getCandidateCount)(voEngine engine, voMathNonTerminalNode target);

  /**
   * Returns the candidate at the specified index.
   *
   * @param engine the engine.
   * @param target the target non terminal node.
   * @param index the index of the candidate to be retrieved.
   *
   * @return the candidate on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a
   *   `voMathNonTerminalNode`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_LIMIT_EXCEEDED when the number of `voMathNode` instances would
   *   exceed the allowed limit (please remember that the limit can be shared by
   *   several types).
   */
  voMathNode (VO_MSE_CALL *getCandidateAt)(voEngine engine, voMathNonTerminalNode target, int32_t index);

  /**
   * Returns the index of the best candidate from the current context in this
   * non terminal node's top list (list of candidates).
   *
   * @param engine the engine.
   * @param target the target non terminal node.
   *
   * @return the index of the best candidate on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a
   *   `voMathNonTerminalNode`.
   * @throws VO_INVALID_STATE when `target` is not in a state where the selected
   *   candidate is available.
   */
  int32_t (VO_MSE_CALL *getSelectedCandidateIndex)(voEngine engine, voMathNonTerminalNode target);
}
voIMathNonTerminalNode;


// -- MathRuleNode -------------------------------------------------------------

/**
 * `voMathRuleNode` reference type.
 *
 * {@extends voMathNode}
 * {@implements voIMathRuleNode}
 */
typedef voMathNode voMathRuleNode;

/**
 * Functions composing the `voIMathRuleNode` interface.
 *
 * This interface exposes the public methods of the `voMathRuleNode` type.
 *
 * {@implementingTypes voMathRuleNode}
 */
typedef struct _voIMathRuleNode
{
  /**
   * Returns the name of this rule node.
   *
   * @param engine the engine.
   * @param target the target rule node.
   * @param charset the charset used to encode parameter strings.
   * @param name the recipient for the name.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMathRuleNode`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `name` or its `bytes` member is not
   *   writable.
   */
  bool (VO_MSE_CALL *getName)(voEngine engine, voMathRuleNode target, voCharset charset, voString* name);

  /**
   * Returns the number of children this rule node contains.
   *
   * @param engine the engine.
   * @param target the target rule node.
   *
   * @return the number of children on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMathRuleNode`.
   */
  int32_t (VO_MSE_CALL *getChildCount)(voEngine engine, voMathRuleNode target);

  /**
   * Returns the child at the specified index.
   *
   * @param engine the engine.
   * @param target the target rule node.
   * @param index the index of the child to be retrieved.
   *
   * @return the child on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMathRuleNode`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_LIMIT_EXCEEDED when the number of `voMathNode` instances would
   *   exceed the allowed limit (please remember that the limit can be shared by
   *   several types).
   */
  voMathNode (VO_MSE_CALL *getChildAt)(voEngine engine, voMathRuleNode target, int32_t index);
}
voIMathRuleNode;


// -- MathCellNode -------------------------------------------------------------

/**
 * `voMathCellNode` reference type.
 *
 * {@extends voMathNonTerminalNode}
 * {@implements voIMathNonTerminalNode, voIMathCellNode}
 */
typedef voMathNonTerminalNode voMathCellNode;

/**
 * Represents the position and size of a cell in a table.
 *
 * @see voMathCellNode
 */
typedef struct _voMathCellData
{
  int32_t rowStart;     /**< Start row. */
  int32_t rowStop;      /**< Stop row. */
  int32_t columnStart;  /**< Start column. */
  int32_t columnStop;   /**< Stop column. */
}
voMathCellData;

/**
 * Functions composing the `voIMathCellNode` interface.
 *
 * This interface exposes the public methods of the `voMathCellNode` type.
 *
 * {@implementingTypes voMathCellNode}
 */
typedef struct _voIMathCellNode
{
  /**
   * Returns the data corresponding to this cell node.
   *
   * @param engine the engine.
   * @param target the target cell node.
   * @param data the recipient for the data.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMathCellNode`.
   * @throws VO_INVALID_USER_BUFFER when `data` is not writable.
   */
  bool (VO_MSE_CALL *getData)(voEngine engine, voMathCellNode target, voMathCellData* data);
}
voIMathCellNode;


// -- MathBorderNode -----------------------------------------------------------

/**
 * `voMathBorderNode` reference type.
 *
 * {@extends voMathNonTerminalNode}
 * {@implements voIMathNonTerminalNode, voIMathBorderNode}
 */
typedef voMathNonTerminalNode voMathBorderNode;

/**
 * Represents a border type.
 */
typedef enum _voMathBorderType
{
  MATH_BORDER_TYPE_HORIZONTAL,  /**< Horizontal border. */
  MATH_BORDER_TYPE_VERTICAL,    /**< Vertical border. */

  MATH_BORDER_TYPE_COUNT,

  _voMathBorderType_FORCE_INT_SIZE = 0x7FFFFFFF
}
voMathBorderType;

/**
 * Represents the position and size of a border in a table.
 *
 * @see voMathBorderNode
 */
typedef struct _voMathBorderData
{
  /**
   * ype of border.
   */
  voMathBorderType type;

  /**
   * Position, depending on type (`0` is leftmost or topmost).
   */
  int32_t position;

  /**
   * Start line or column, depending on type.
   */
  int32_t start;

  /**
   * Stop line or column, depending on type.
   */
  int32_t stop;
}
voMathBorderData;

/**
 * Functions composing the `voIMathBorderNode` interface.
 *
 * This interface exposes the public methods of the `voMathBorderNode` type.
 *
 * {@implementingTypes voMathBorderNode}
 */
typedef struct _voIMathBorderNode
{
  /**
   * Returns the data corresponding to this border node.
   *
   * @param engine the engine.
   * @param target the target border node.
   * @param data the recipient for the data.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMathBorderNode`.
   * @throws VO_INVALID_USER_BUFFER when `data` is not writable.
   */
  bool (VO_MSE_CALL *getData)(voEngine engine, voMathBorderNode target, voMathBorderData* data);
}
voIMathBorderNode;


// -- MathTableNode ------------------------------------------------------------

/**
 * `voMathTableNode` reference type.
 *
 * {@extends voMathRuleNode}
 * {@implements voIMathRuleNode, voIMathTableNode}
 */
typedef voMathRuleNode voMathTableNode;

/**
 * Represents the size of a table.
 *
 * @see voMathTableNode
 */
typedef struct _voMathTableData
{
  int32_t rowCount;
  int32_t columnCount;
}
voMathTableData;

/**
 * Functions composing the `voIMathTableNode` interface.
 *
 * This interface exposes the public methods of the `voMathTableNode` type.
 *
 * {@implementingTypes voMathTableNode}
 */
typedef struct _voIMathTableNode
{
  /**
   * Returns the data corresponding to this table node.
   *
   * @param engine the engine.
   * @param target the target table node.
   * @param data the recipient for the data.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMathTableNode`.
   * @throws VO_INVALID_USER_BUFFER when `data` is not writable.
   */
  bool (VO_MSE_CALL *getData)(voEngine engine, voEngineObject target, voMathTableData* data);
}
voIMathTableNode;


// -- MathScratchOut -----------------------------------------------------------

/**
 * Functions composing the `voIMathScratchOut` interface.
 *
 * This interface exposes the public methods of the `voMathScratchOut` type.
 *
 * {@implementingTypes voMathScratchOut}
 * @since 6.1_0
 */
typedef struct _voIMathScratchOut
{
  /**
   * Returns the number of input ranges this scratch out covers.
   *
   * @param engine the engine.
   * @param target the target scratch out.
   *
   * @return the number of input ranges on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMathScratchOut`.
   * @throws VO_INVALID_STATE when the portion of a MyScript Document associated
   *   with this scratch out has been modified, invalidating input ranges.
   */
  int32_t (VO_MSE_CALL *getInputRangeCount)(voEngine engine, voMathScratchOut target);

  /**
   * Returns the input range at the specified index this scratch out covers.
   *
   * @param engine the engine.
   * @param target the target scratch out.
   * @param index the index of the requested input range.
   * @param range the recipient for the input range.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMathScratchOut`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_INVALID_USER_BUFFER when `range` is not writable.
   * @throws VO_INVALID_STATE when the portion of a MyScript Document associated
   *   with this scratch out has been modified, invalidating input ranges.
   */
  bool (VO_MSE_CALL *getInputRangeAt)(voEngine engine, voMathScratchOut target, int32_t index, voMathInputRange* range);

  /**
   * Returns the number of input ranges this scratch out erases.
   *
   * @param engine the engine.
   * @param target the target scratch out.
   *
   * @return the number of input ranges on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMathScratchOut`.
   * @throws VO_INVALID_STATE when the portion of a MyScript Document associated
   *   with this scratch out has been modified, invalidating input ranges.
   */
  int32_t (VO_MSE_CALL *getErasedInputRangeCount)(voEngine engine, voMathScratchOut target);

  /**
   * Returns the input range at the specified index this scratch out erases.
   *
   * @param engine the engine.
   * @param target the target scratch out.
   * @param index the index of the requested input range.
   * @param range the recipient for the input range.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMathScratchOut`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_INVALID_USER_BUFFER when `range` is not writable.
   * @throws VO_INVALID_STATE when the portion of a MyScript Document associated
   *   with this scratch out has been modified, invalidating input ranges.
   */
  bool (VO_MSE_CALL *getErasedInputRangeAt)(voEngine engine, voMathScratchOut target, int32_t index, voMathInputRange* range);
}
voIMathScratchOut;

#endif // end of: #ifndef C_MYSCRIPTMATH_H
