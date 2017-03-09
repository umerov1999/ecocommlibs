#ifndef C_MYSCRIPTMUSIC_H
#define C_MYSCRIPTMUSIC_H 0x08010000
/**
 * @file c/MyScriptMusic.h
 * Native interface to the MyScript Music service.
 */

#include "MyScriptEngine.h"


// -- error codes --------------------------------------------------------------

/**
 * Lists the MyScript Music errors.
 */
enum VO_MUSIC_ERR
{
  /**
   * Unable to complete the requested operation because a
   * `voMusicAlphabetKnowledge` is missing.
   *
   * @see voMusicAlphabetKnowledge
   */
  VO_MISSING_MUSIC_ALPHABET_KNOWLEDGE = VO_INVALID_STATE | (7 << 16),

  /**
   * Unable to complete the requested operation because a `voMusicGrammar` is
   * missing.
   *
   * @see voMusicGrammar
   */
  VO_MISSING_MUSIC_GRAMMAR,

  /**
   * Unable to complete the requested operation because a staff is missing.
   *
   * @see voMusicDocument
   * @see voMusicRecognizer
   */
  VO_MISSING_MUSIC_STAFF
};


// -- types and interfaces -----------------------------------------------------

/**
 * Lists the MyScript Music types.
 */
enum VO_MUSIC_T
{
  /**
   * `voMusicDocument` type identifier.
   *
   * @see voMusicDocument
   */
  VO_MusicDocument = 9000,

  /**
   * `voMusicAlphabetKnowledge` type identifier.
   *
   * @see voMusicAlphabetKnowledge
   */
  VO_MusicAlphabetKnowledge,

  /**
   * `voMusicGrammar` type identifier.
   *
   * @see voMusicGrammar
   */
  VO_MusicGrammar,

  /**
   * `voMusicRecognizer` type identifier.
   *
   * @see voMusicRecognizer
   */
  VO_MusicRecognizer,

  /**
   * `voMusicScore` type identifier.
   *
   * @see voMusicScore
   */
  VO_MusicScore,

  /**
   * `voMusicPart` type identifier.
   *
   * @see voMusicPart
   */
  VO_MusicPart,

  /**
   * `voMusicElement` type identifier.
   *
   * @see voMusicElement
   */
  VO_MusicElement,

  /**
   * `voMusicDecoration` type identifier.
   *
   * @see voMusicDecoration
   */
  VO_MusicDecoration,

  /**
   * `voMusicBar` type identifier.
   *
   * @see voMusicBar
   */
  VO_MusicBar,

  /**
   * `voMusicClef` type identifier.
   *
   * @see voMusicClef
   */
  VO_MusicClef,

  /**
   * `voMusicAccidental` type identifier.
   *
   * @see voMusicAccidental
   */
  VO_MusicAccidental,

  /**
   * `voMusicKeySignature` type identifier.
   *
   * @see voMusicKeySignature
   */
  VO_MusicKeySignature,

  /**
   * `voMusicAnnotation` type identifier.
   *
   * @see voMusicAnnotation
   */
  VO_MusicAnnotation,

  /**
   * `voMusicTimeSignature` type identifier.
   *
   * @see voMusicTimeSignature
   */
  VO_MusicTimeSignature,

  /**
   * `voMusicNote` type identifier.
   *
   * @see voMusicNote
   */
  VO_MusicNote,

  /**
   * `voMusicHead` type identifier.
   *
   * @see voMusicHead
   */
  VO_MusicHead,

  /**
   * `voMusicStem` type identifier.
   *
   * @see voMusicStem
   */
  VO_MusicStem,

  /**
   * `voMusicBeam` type identifier.
   *
   * @see voMusicBeam
   */
  VO_MusicBeam,

  /**
   * `voMusicDots` type identifier.
   *
   * @see voMusicDots
   */
  VO_MusicDots,

  /**
   * `voMusicTie` type identifier.
   *
   * @see voMusicTie
   */
  VO_MusicTie,

  /**
   * `voMusicSlur` type identifier.
   *
   * @see voMusicSlur
   */
  VO_MusicSlur,

  /**
   * `voMusicLedgerLine` type identifier.
   *
   * @see voMusicLedgerLine
   */
  VO_MusicLedgerLine,

  /**
   * `voMusicRest` type identifier.
   *
   * @see voMusicRest
   */
  VO_MusicRest,

  /**
   * `voMusicChord` type identifier.
   *
   * @see voMusicChord
   */
  VO_MusicChord,

  /**
   * `voMusicScratchOut` type identifier.
   *
   * @see voMusicScratchOut
   * @since 6.1_0
   */
  VO_MusicScratchOut,

  /**
   * `voMusicArpeggiate` type identifier.
   *
   * @see voMusicArpeggiate
   * @since 6.3_0
   */
  VO_MusicArpeggiate,

  /**
   * `voMusicTuplet` type identifier.
   *
   * @see voMusicTuplet
   * @since 7.0_0
   */
  VO_MusicTuplet,

  /**
   * `voMusicTupletBracket` type identifier.
   *
   * @see voMusicTupletBracket
   * @since 7.0_0
   */
  VO_MusicTupletBracket
};

/**
 * Lists the MyScript Music interfaces.
 */
enum VO_MUSIC_I
{
  /**
   * `voIMusicDocument` interface identifier.
   *
   * @see voIMusicDocument
   */
  VO_IMusicDocument = 9050,

  /**
   * `voIMusicRecognizer` interface identifier.
   *
   * @see voIMusicRecognizer
   */
  VO_IMusicRecognizer = 9052,

  /**
   * `voIMusicScore` interface identifier.
   *
   * @see voIMusicScore
   */
  VO_IMusicScore,

  /**
   * `voIMusicPart` interface identifier.
   *
   * @see voIMusicPart
   */
  VO_IMusicPart,

  /**
   * `voIMusicElement` interface identifier.
   *
   * @see voIMusicElement
   */
  VO_IMusicElement,

  /**
   * `voIMusicDecoration` interface identifier.
   *
   * @see voIMusicDecoration
   */
  VO_IMusicDecoration,

  /**
   * `voIMusicBar` interface identifier.
   *
   * @see voIMusicBar
   */
  VO_IMusicBar,

  /**
   * `voIMusicClef` interface identifier.
   *
   * @see voIMusicClef
   */
  VO_IMusicClef,

  /**
   * `voIMusicAccidental` interface identifier.
   *
   * @see voIMusicAccidental
   */
  VO_IMusicAccidental,

  /**
   * `voIMusicKeySignature` interface identifier.
   *
   * @see voIMusicKeySignature
   */
  VO_IMusicKeySignature,

  /**
   * `voIMusicAnnotation` interface identifier.
   *
   * @see voIMusicAnnotation
   */
  VO_IMusicAnnotation,

  /**
   * `voIMusicTimeSignature` interface identifier.
   *
   * @see voIMusicTimeSignature
   */
  VO_IMusicTimeSignature,

  /**
   * `voIMusicNote` interface identifier.
   *
   * @see voIMusicNote
   */
  VO_IMusicNote,

  /**
   * `voIMusicHead` interface identifier.
   *
   * @see voIMusicHead
   */
  VO_IMusicHead,

  /**
   * `voIMusicStem` interface identifier.
   *
   * @see voIMusicStem
   */
  VO_IMusicStem,

  /**
   * `voIMusicBeam` interface identifier.
   *
   * @see voIMusicBeam
   */
  VO_IMusicBeam,

  /**
   * `voIMusicDots` interface identifier.
   *
   * @see voIMusicDots
   */
  VO_IMusicDots,

  /**
   * `voIMusicTie` interface identifier.
   *
   * @see voIMusicTie
   */
  VO_IMusicTie,

  /**
   * `voIMusicSlur` interface identifier.
   *
   * @see voIMusicSlur
   */
  VO_IMusicSlur,

  /**
   * `voIMusicRest` interface identifier.
   *
   * @see voIMusicRest
   */
  VO_IMusicRest,

  /**
   * `voIMusicChord` interface identifier.
   *
   * @see voIMusicChord
   */
  VO_IMusicChord,

  /**
   * `voIMusicScratchOut` type identifier.
   *
   * @see voIMusicScratchOut
   * @since 6.1_0
   */
  VO_IMusicScratchOut,

  /**
   * `voIMusicArpeggiate` interface identifier.
   *
   * @see voIMusicArpeggiate
   * @since 6.3_0
   */
  VO_IMusicArpeggiate,

   /**
   * `voIMusicTuplet` interface identifier.
   *
   * @see voIMusicTuplet
   * @since 7.0_0
   */
  VO_IMusicTuplet,

  /**
   * `voIMusicTupletBracket` interface identifier.
   *
   * @see voIMusicTupletBracket
   * @since 7.0_0
   */
  VO_IMusicTupletBracket
};


// -- MusicElement -------------------------------------------------------------

/**
 * Represents an element placement.
 */
typedef enum _voMusicPlacement
{
  VO_MUSIC_PLACEMENT_ERR = -1, /**< Error marker. */

  VO_MUSIC_PLACEMENT_ABOVE,    /**< Above. */
  VO_MUSIC_PLACEMENT_BELOW,    /**< Below. */

  VO_MUSIC_PLACEMENT_COUNT,

  _voMusicPlacement_FORCE_INT_SIZE = 0x7FFFFFFF
}
voMusicPlacement;


/**
 * `voMusicElement` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voIMusicElement}
 */
typedef voEngineObject voMusicElement;

/**
 * {@typePropertySet MusicElement}
 */
enum VO_MUSIC_ELEMENT_TPROP
{
  /**
   * Represents the maximum number of `voMusicElement` instances that can exist
   * at the same time in the user side of a MyScript enabled application.
   *
   * The special value zero means no limit. Other values are adjusted to the
   * next multiple of 16. A value under the current count is permitted and will
   * constrain future resource creations.
   *
   * {@property uint32_t}
   */
  VO_USER_MUSIC_ELEMENT_LIMIT = VO_USER_OBJ_LIMIT,
  /**
   * Gives the number of `voMusicElement` instances that currently exist in the
   * user side of a MyScript enabled application.
   *
   * {@property read-only uint32_t}
   */
  VO_USER_MUSIC_ELEMENT_COUNT = VO_USER_OBJ_COUNT
};


/**
 * Represents a range in the input.
 */
typedef struct _voMusicInputRange
{
  /**
   * Index of the component (stroke or element).
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
voMusicInputRange;

/**
 * Functions composing the `voIMusicElement` interface.
 *
 * This interface exposes the public methods of the `voMusicElement` type.
 *
 * {@implementingTypes voMusicElement}
 */
typedef struct _voIMusicElement
{
  /**
   * Returns the number of input ranges this element covers.
   *
   * @param engine the engine.
   * @param target the target element.
   *
   * @return the number of input ranges on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicElement`.
   * @throws VO_INVALID_STATE when the portion of a MyScript Document associated
   *   with this element has been modified, invalidating input ranges.
   */
  int32_t (VO_MSE_CALL *getInputRangeCount)(voEngine engine, voMusicElement target);

  /**
   * Returns the input range at the specified index.
   *
   * @param engine the engine.
   * @param target the target element.
   * @param index the index of the requested input range.
   * @param range the recipient for the input range.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicElement`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_INVALID_USER_BUFFER when `range` is not writable.
   * @throws VO_INVALID_STATE when the portion of a MyScript Document associated
   *   with this element has been modified, invalidating input ranges.
   */
  bool (VO_MSE_CALL *getInputRangeAt)(voEngine engine, voMusicElement target, int32_t index, voMusicInputRange* range);
}
voIMusicElement;


// -- MusicDecoration ----------------------------------------------------------

/**
 * `voMusicDecoration` reference type.
 *
 * {@extends voMusicElement}
 * {@implements voIMusicDecoration, voIMusicElement}
 */
typedef voMusicElement voMusicDecoration;

/**
 * Represents a music decoration symbol.
 */
typedef enum _voMusicDecorationSymbol
{
  VO_MUSIC_DECORATION_SYMBOL_ERR = -1,          /**< Error marker. */

  // Fermatas
  VO_MUSIC_DECORATION_SYMBOL_NORMAL_FERMATA,    /**< Normal fermata. */
  VO_MUSIC_DECORATION_SYMBOL_ANGLED_FERMATA,    /**< Angled fermata. */
  VO_MUSIC_DECORATION_SYMBOL_SQUARE_FERMATA,    /**< Square fermata. */

  // Articulations
  VO_MUSIC_DECORATION_SYMBOL_ACCENT,            /**< Accent. */
  VO_MUSIC_DECORATION_SYMBOL_STACCATO,          /**< Staccato. */
  VO_MUSIC_DECORATION_SYMBOL_TENUTO,            /**< Tenuto. */
  VO_MUSIC_DECORATION_SYMBOL_STACCATISSIMO,     /**< Staccatissimo. */
  VO_MUSIC_DECORATION_SYMBOL_STRONG_ACCENT_UP,  /**< Strong accent up. */

  // Ornaments
  VO_MUSIC_DECORATION_SYMBOL_MORDENT,           /**< Mordent. */
  VO_MUSIC_DECORATION_SYMBOL_TRILL_MARK,        /**< Trill mark. */
  VO_MUSIC_DECORATION_SYMBOL_INVERTED_MORDENT,  /**< Inverted mordent. */
  VO_MUSIC_DECORATION_SYMBOL_INVERTED_TURN,     /**< Inverted turn. */
  VO_MUSIC_DECORATION_SYMBOL_TURN,              /**< Turn. */

  // Technical signs
  VO_MUSIC_DECORATION_SYMBOL_UP_BOW,            /**< Up bow. */
  VO_MUSIC_DECORATION_SYMBOL_STOPPED,           /**< Stopped. */
  VO_MUSIC_DECORATION_SYMBOL_DOWN_BOW,          /**< Down bow. */

  VO_MUSIC_DECORATION_SYMBOL_COUNT,

  _voMusicDecorationSymbol_FORCE_INT_SIZE = 0x7FFFFFFF
}
voMusicDecorationSymbol;

/**
 * Functions composing the `voIMusicDecoration` interface.
 *
 * This interface exposes the public methods of the `voMusicDecoration` type.
 *
 * {@implementingTypes voMusicDecoration}
 */
typedef struct _voIMusicDecoration
{
  /**
   * Returns the symbol of this decoration.
   *
   * @param engine the engine.
   * @param target the target decoration.
   *
   * @return the decoration symbol on success, otherwise
   *   `VO_MUSIC_DECORATION_SYMBOL_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicDecoration`.
   */
  voMusicDecorationSymbol (VO_MSE_CALL *getSymbol)(voEngine engine, voMusicDecoration target);

  /**
   * Returns the placement of this decoration.
   *
   * @param engine the engine.
   * @param target the target decoration.
   *
   * @return the placement on success, otherwise `VO_MUSIC_PLACEMENT_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicDecoration`.
   */
  voMusicPlacement (VO_MSE_CALL *getPlacement)(voEngine engine, voMusicDecoration target);
}
voIMusicDecoration;


// -- MusicAnnotation ----------------------------------------------------------

/**
 * `voMusicAnnotation` reference type.
 *
 * {@extends voMusicElement}
 * {@implements voIMusicAnnotation, voIMusicElement}
 */
typedef voMusicElement voMusicAnnotation;

/**
 * Functions composing the `voIMusicAnnotation` interface.
 *
 * This interface exposes the public methods of the `voMusicAnnotation` type.
 *
 * {@implementingTypes voMusicAnnotation}
 */
typedef struct _voIMusicAnnotation
{
  /**
   * Returns the label of this annotation.
   *
   * @param engine the engine.
   * @param target the target annotation.
   * @param charset the charset used to encode parameter strings.
   * @param label the recipient for the label.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicAnnotation`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `label` or its `bytes` member is not
   *   writable.
   */
  bool (VO_MSE_CALL *getLabel)(voEngine engine, voMusicAnnotation target, voCharset charset, voString* label);
}
voIMusicAnnotation;


// -- MusicTupletBracket -------------------------------------------------------

/**
 * `voMusicTupletBracket` reference type.
 *
 * {@extends voMusicElement}
 * {@implements voIMusicTupletBracket, voIMusicElement}
 * @since 7.0_0
 */
typedef voMusicElement voMusicTupletBracket;

/**
 * Represents a tuplet bracket type.
 *
 * @since 7.0_0
*/
typedef enum _voMusicTupletBracketType
{
  VO_MUSIC_TUPLET_BRACKET_TYPE_ERR = -1,      /**< Error marker. */

  VO_MUSIC_TUPLET_BRACKET_TYPE_START_ABOVE,   /**< Start above. */
  VO_MUSIC_TUPLET_BRACKET_TYPE_STOP_ABOVE,    /**< Stop above. */
  VO_MUSIC_TUPLET_BRACKET_TYPE_START_BELOW,   /**< Start below. */
  VO_MUSIC_TUPLET_BRACKET_TYPE_STOP_BELOW,    /**< Stop below. */

  VO_MUSIC_TUPLET_BRACKET_TYPE_COUNT,

  _voMusicTupletBracketType_FORCE_INT_SIZE = 0x7FFFFFFF
}
voMusicTupletBracketType;

/**
 * Functions composing the `voIMusicTupletBracket` interface.
 *
 * This interface exposes the public methods of the `voMusicTupletBracket` type.
 *
 * {@implementingTypes voMusicTupletBracket}
 * @since 7.0_0
 */
typedef struct _voIMusicTupletBracket
{
  /**
   * Returns the type of this tuplet bracket.
   *
   * @param engine the engine.
   * @param target the target tuplet bracket.
   *
   * @return the type of tuplet bracket on success, otherwise
   *   `VO_MUSIC_TUPLET_BRACKET_TYPE_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicTupletBracket`.
   */
   voMusicTupletBracketType (VO_MSE_CALL *getType)(voEngine engine, voMusicTupletBracket target);
}
voIMusicTupletBracket;


// -- MusicTuplet --------------------------------------------------------------

/**
 * `voMusicTuplet` reference type.
 *
 * {@extends voMusicElement}
 * {@implements voIMusicTuplet, voIMusicElement}
 * @since 7.0_0
 */
typedef voMusicElement voMusicTuplet;

/**
 * Functions composing the `voIMusicTuplet` interface.
 *
 * This interface exposes the public methods of the `voMusicTuplet` type.
 *
 * {@implementingTypes voMusicTuplet}
 * @since 7.0_0
 */
typedef struct _voIMusicTuplet
{
  /**
   * Returns the placement of this tuplet.
   *
   * @param engine the engine.
   * @param target the target tuplet.
   *
   * @return the placement on success, otherwise `VO_MUSIC_PLACEMENT_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicTuplet`.
   */
   voMusicPlacement (VO_MSE_CALL *getPlacement)(voEngine engine, voMusicTuplet target);

   /**
   * Returns the number of brackets on this tuplet.
   *
   * @param engine the engine.
   * @param target the target tuplet.
   *
   * @return the number of brackets on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicTuplet`.
   */
   int32_t (VO_MSE_CALL *getBracketCount)(voEngine engine, voMusicTuplet target);

   /**
   * Returns the bracket at the specified index.
   *
   * @param engine the engine.
   * @param target the target tuplet.
   * @param index the index of the bracket to be retrieved.
   *
   * @return the bracket on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicTuplet`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_LIMIT_EXCEEDED when the number of `voMusicTupletBracket`
   *   instances would exceed the allowed limit (please remember that the limit
   *   can be shared by several types).
   */
   voMusicTupletBracket (VO_MSE_CALL *getBracketAt)(voEngine engine, voMusicTuplet target, int32_t index);

   /**
   * Returns the tuplet number annotation.
   *
   * @param engine the engine.
   * @param target the target tuplet.
   *
   * @return the number annotation if any, otherwise `NULL`.
   *   When `NULL`, use `voGetError()` to figure out whether an error occurred.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicTuplet`.
   * @throws VO_LIMIT_EXCEEDED when the number of `voMusicAnnotation` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   */
   voMusicAnnotation (VO_MSE_CALL *getNumber)(voEngine engine, voMusicTuplet target);
}
voIMusicTuplet;


// -- MusicBar -----------------------------------------------------------------

/**
 * `voMusicBar` reference type.
 *
 * {@extends voMusicElement}
 * {@implements voIMusicBar, voIMusicElement}
 */
typedef voMusicElement voMusicBar;

/**
 * Represents a music bar style.
 */
typedef enum _voMusicBarStyle
{
  VO_MUSIC_BAR_STYLE_ERR = -1,     /**< Error marker. */

  VO_MUSIC_BAR_STYLE_REGULAR,      /**< Normal. */
  VO_MUSIC_BAR_STYLE_DOTTED,       /**< Dotted. */
  VO_MUSIC_BAR_STYLE_LIGHT_LIGHT,  /**< Double. */
  VO_MUSIC_BAR_STYLE_LIGHT_HEAVY,  /**< End. */
  VO_MUSIC_BAR_STYLE_HEAVY_LIGHT,  /**< Start repeat. */

  VO_MUSIC_BAR_STYLE_COUNT,

  _voMusicBarStyle_FORCE_INT_SIZE = 0x7FFFFFFF
}
voMusicBarStyle;

/**
 * Represents a music bar repeat direction (if any).
 */
typedef enum _voMusicBarRepeatDirection
{
  VO_MUSIC_BAR_REPEAT_DIRECTION_ERR = -1, /**< Error marker. */

  VO_MUSIC_BAR_REPEAT_DIRECTION_NONE,     /**< None. */
  VO_MUSIC_BAR_REPEAT_DIRECTION_FORWARD,  /**< Repeat forward. */
  VO_MUSIC_BAR_REPEAT_DIRECTION_BACKWARD, /**< Repeat backward. */

  VO_MUSIC_BAR_REPEAT_DIRECTION_COUNT,

  _voMusicBarRepeatDirection_FORCE_INT_SIZE = 0x7FFFFFFF
}
voMusicBarRepeatDirection;

/**
 * Functions composing the `voIMusicBar` interface.
 *
 * This interface exposes the public methods of the `voMusicBar` type.
 *
 * {@implementingTypes voMusicBar}
 */
typedef struct _voIMusicBar
{
  /**
   * Returns the bar style.
   *
   * @param engine the engine.
   * @param target the target bar.
   *
   * @return the bar style on success, otherwise `VO_MUSIC_BAR_STYLE_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicBar`.
   */
  voMusicBarStyle (VO_MSE_CALL *getStyle)(voEngine engine, voMusicBar target);

   /**
   * Returns the bar repeat direction.
   *
   * @param engine the engine.
   * @param target the target bar.
   *
   * @return the bar repeat direction on success, otherwise
   *   `VO_MUSIC_BAR_REPEAT_DIRECTION_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicBar`.
   */
  voMusicBarRepeatDirection (VO_MSE_CALL *getRepeatDirection)(voEngine engine, voMusicBar target);

  /**
   * Returns the number of decorations on this bar.
   *
   * @param engine the engine.
   * @param target the target bar.
   *
   * @return the number of decorations on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicBar`.
   */
  int32_t (VO_MSE_CALL *getDecorationCount)(voEngine engine, voMusicBar target);

  /**
   * Returns the decoration at the specified index.
   *
   * @param engine the engine.
   * @param target the target bar.
   * @param index the index of the requested decoration.
   *
   * @return the decoration on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicBar`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_LIMIT_EXCEEDED when the number of `voMusicDecoration` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   */
  voMusicDecoration (VO_MSE_CALL *getDecorationAt)(voEngine engine, voMusicBar target, int32_t index);
}
voIMusicBar;


// -- MusicClef ----------------------------------------------------------------

/**
 * `voMusicClef` reference type.
 *
 * {@extends voMusicElement}
 * {@implements voIMusicClef, voIMusicElement}
 */
typedef voMusicElement voMusicClef;

/**
 * Represents a music clef symbol.
 */
typedef enum _voMusicClefSymbol
{
  VO_MUSIC_CLEF_SYMBOL_ERR = -1,  /**< Error marker. */

  VO_MUSIC_CLEF_SYMBOL_G,         /**< G. */
  VO_MUSIC_CLEF_SYMBOL_F,         /**< F. */
  VO_MUSIC_CLEF_SYMBOL_C,         /**< C. */

  VO_MUSIC_CLEF_SYMBOL_COUNT,

  _voMusicClefSymbol_FORCE_INT_SIZE = 0x7FFFFFFF
}
voMusicClefSymbol;

/**
 * Functions composing the `voIMusicClef` interface.
 *
 * This interface exposes the public methods of the `voMusicClef` type.
 *
 * {@implementingTypes voMusicClef}
 */
typedef struct _voIMusicClef
{
  /**
   * Returns the clef symbol.
   *
   * @param engine the engine.
   * @param target the target clef.
   *
   * @return the clef symbol on success, otherwise `VO_MUSIC_CLEF_SYMBOL_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicClef`.
   */
  voMusicClefSymbol (VO_MSE_CALL *getSymbol)(voEngine engine, voMusicClef target);

  /**
   * Returns the line on which the clef is positioned.
   *
   * @param engine the engine.
   * @param target the target clef.
   *
   * @return the line on success, otherwise `INT32_MAX`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicClef`.
   */
  int32_t (VO_MSE_CALL *getLine)(voEngine engine, voMusicClef target);

  /**
   * Returns the clef octave.
   *
   * @param engine the engine.
   * @param target the target clef.
   *
   * @return the clef octave on success otherwise `INT32_MAX`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicClef`.
   */
  int32_t (VO_MSE_CALL *getOctave)(voEngine engine, voMusicClef target);
}
voIMusicClef;


// -- MusicAccidental ----------------------------------------------------------

/**
 * `voMusicAccidental` reference type.
 *
 * {@extends voMusicElement}
 * {@implements voIMusicAccidental, voIMusicElement}
 */
typedef voMusicElement voMusicAccidental;

/**
 * Represents a music accidental type.
 */
typedef enum _voMusicAccidentalType
{
  VO_MUSIC_ACCIDENTAL_TYPE_ERR = -1,      /**< Error marker. */

  VO_MUSIC_ACCIDENTAL_TYPE_NATURAL,       /**< Natural accidental. */
  VO_MUSIC_ACCIDENTAL_TYPE_SHARP,         /**< Sharp accidental. */
  VO_MUSIC_ACCIDENTAL_TYPE_DOUBLE_SHARP,  /**< Double sharp accidental. */
  VO_MUSIC_ACCIDENTAL_TYPE_FLAT,          /**< Flat accidental. */
  VO_MUSIC_ACCIDENTAL_TYPE_FLAT_FLAT,     /**< Double flat accidental. */

  VO_MUSIC_ACCIDENTAL_TYPE_COUNT,

  _voMusicAccidentalType_FORCE_INT_SIZE = 0x7FFFFFFF
}
voMusicAccidentalType;

/**
 * Functions composing the `voIMusicAccidental` interface.
 *
 * This interface exposes the public methods of the `voMusicAccidental` type.
 *
 * {@implementingTypes voMusicAccidental}
 */
typedef struct _voIMusicAccidental
{
  /**
   * Returns the type of accidental.
   *
   * @param engine the engine.
   * @param target the target accidental.
   *
   * @return the type of accidental on success, otherwise
   *   `VO_MUSIC_ACCIDENTAL_TYPE_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicAccidental`.
   */
  voMusicAccidentalType (VO_MSE_CALL *getType)(voEngine engine, voMusicAccidental target);
}
voIMusicAccidental;


// -- MusicKeySignature --------------------------------------------------------

/**
 * `voMusicKeySignature` reference type.
 *
 * {@extends voMusicElement}
 * {@implements voIMusicKeySignature, voIMusicElement}
 */
typedef voMusicElement voMusicKeySignature;

/**
 * Represents a key signature.
 *
 * @see voMusicKeySignature
 */
typedef struct _voMusicKeySignatureData
{
  int32_t fifths; /**< Fifths. */
  int32_t cancel; /**< Cancel. */
}
voMusicKeySignatureData;

/**
 * Functions composing the `voIMusicKeySignature` interface.
 *
 * This interface exposes the public methods of the `voMusicKeySignature` type.
 *
 * {@implementingTypes voMusicKeySignature}
 */
typedef struct _voIMusicKeySignature
{
  /**
   * Return the key signature data.
   *
   * @param engine the engine.
   * @param target the target key signature.
   * @param data the recipient for the data.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicKeySignature`.
   * @throws VO_INVALID_USER_BUFFER when `data` is not writable.
   */
  bool (VO_MSE_CALL *getSignature)(voEngine engine, voMusicKeySignature target, voMusicKeySignatureData* data);

  /**
   * Returns the number of accidentals in this key signature.
   *
   * @param engine the engine.
   * @param target the target key signature.
   *
   * @return the number of accidentals on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicKeySignature`.
   */
  int32_t (VO_MSE_CALL *getAccidentalCount)(voEngine engine, voMusicKeySignature target);

  /**
   * Returns the accidental at the specified index.
   *
   * @param engine the engine.
   * @param target the target key signature.
   * @param index the index of the requested accidental.
   *
   * @return the accidental on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicKeySignature`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_LIMIT_EXCEEDED when the number of `voMusicAccidental` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   */
  voMusicAccidental (VO_MSE_CALL *getAccidentalAt)(voEngine engine, voMusicKeySignature target, int32_t index);
}
voIMusicKeySignature;


// -- MusicTimeSignature -------------------------------------------------------

/**
 * `voMusicTimeSignature` reference type.
 *
 * {@extends voMusicElement}
 * {@implements voIMusicTimeSignature, voIMusicElement}
 */
typedef voMusicElement voMusicTimeSignature;

/**
 * Represents a time signature type.
 */
typedef enum _voMusicTimeSignatureType
{
  VO_MUSIC_TIME_SIGNATURE_TYPE_ERR = -1,    /**< Error marker. */

  VO_MUSIC_TIME_SIGNATURE_TYPE_NORMAL,      /**< Normal. */
  VO_MUSIC_TIME_SIGNATURE_TYPE_COMMON,      /**< Common. */
  VO_MUSIC_TIME_SIGNATURE_TYPE_ALLA_BREVE,  /**< Alla breve. */

  VO_MUSIC_TIME_SIGNATURE_TYPE_COUNT,

  _voMusicTimeSignatureType_FORCE_INT_SIZE = 0x7FFFFFFF
}
voMusicTimeSignatureType;

/**
 * Functions composing the `voIMusicTimeSignature` interface.
 *
 * This interface exposes the public methods of the `voMusicTimeSignature` type.
 *
 * {@implementingTypes voMusicTimeSignature}
 */
typedef struct _voIMusicTimeSignature
{
  /**
   * Returns the time signature type.
   *
   * @param engine the engine.
   * @param target the target time signature.
   *
   * @return the time signature type on success, otherwise
   *   `VO_MUSIC_TIME_SIGNATURE_TYPE_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicTimeSignature`.
   */
  voMusicTimeSignatureType (VO_MSE_CALL *getType)(voEngine engine, voMusicTimeSignature target);

  /**
   * Returns the time signature top annotation (optional).
   *
   * @param engine the engine.
   * @param target the target time signature.
   *
   * @return the top annotation if any, otherwise `NULL`
   *   When `NULL`, use `voGetError()` to figure out whether an error occurred.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicTimeSignature`.
   * @throws VO_LIMIT_EXCEEDED when the number of `voMusicAnnotation` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   */
  voMusicAnnotation (VO_MSE_CALL *getTop)(voEngine engine, voMusicTimeSignature target);

  /**
   * Returns the time signature bottom annotation (optional).
   *
   * @param engine the engine.
   * @param target the target time signature.
   *
   * @return the bottom annotation if any, otherwise `NULL`
   *   When `NULL`, use `voGetError()` to figure out whether an error occurred.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicTimeSignature`.
   * @throws VO_LIMIT_EXCEEDED when the number of `voMusicAnnotation` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   */
  voMusicAnnotation (VO_MSE_CALL *getBottom)(voEngine engine, voMusicTimeSignature target);
}
voIMusicTimeSignature;


// -- MusicHead ----------------------------------------------------------------

/**
 * `voMusicHead` reference type.
 *
 * {@extends voMusicElement}
 * {@implements voIMusicHead, voIMusicElement}
 */
typedef voMusicElement voMusicHead;

/**
 * Represents a head type.
 */
typedef enum _voMusicHeadType
{
  VO_MUSIC_HEAD_TYPE_ERR = -1,  /**< Error marker. */

  VO_MUSIC_HEAD_TYPE_BLACK,     /**< Black / Full. */
  VO_MUSIC_HEAD_TYPE_WHITE,     /**< White / Empty. */

  VO_MUSIC_HEAD_TYPE_COUNT,

  _voMusicHeadType_FORCE_INT_SIZE = 0x7FFFFFFF
}
voMusicHeadType;

/**
 * Functions composing the `voIMusicHead` interface.
 *
 * This interface exposes the public methods of the `voMusicHead` type.
 *
 * {@implementingTypes voMusicHead}
 */
typedef struct _voIMusicHead
{
  /**
   * Returns the type of head.
   *
   * @param engine the engine.
   * @param target the target head.
   *
   * @return the type of head on success, otherwise `VO_MUSIC_HEAD_TYPE_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicHead`.
   */
  voMusicHeadType (VO_MSE_CALL *getType)(voEngine engine, voMusicHead target);
}
voIMusicHead;


// -- MusicStem ----------------------------------------------------------------

/**
 * `voMusicStem` reference type.
 *
 * {@extends voMusicElement}
 * {@implements voIMusicStem, voIMusicElement}
 */
typedef voMusicElement voMusicStem;

/**
 * Represents a stem type.
 */
typedef enum _voMusicStemType
{
  VO_MUSIC_STEM_TYPE_ERR = -1,    /**< Error marker. */

  VO_MUSIC_STEM_TYPE_UP,          /**< Up. */
  VO_MUSIC_STEM_TYPE_UP_FLAG1,    /**< Up eighth. */
  VO_MUSIC_STEM_TYPE_UP_FLAG2,    /**< Up 16th. */
  VO_MUSIC_STEM_TYPE_UP_FLAG3,    /**< Up 32nd. */
  VO_MUSIC_STEM_TYPE_UP_FLAG4,    /**< Up 64th. */

  VO_MUSIC_STEM_TYPE_DOWN,        /**< Down. */
  VO_MUSIC_STEM_TYPE_DOWN_FLAG1,  /**< Down eighth. */
  VO_MUSIC_STEM_TYPE_DOWN_FLAG2,  /**< Down 16th. */
  VO_MUSIC_STEM_TYPE_DOWN_FLAG3,  /**< Down 32nd. */
  VO_MUSIC_STEM_TYPE_DOWN_FLAG4,  /**< Down 64th. */

  VO_MUSIC_STEM_TYPE_COUNT,

  _voMusicStemType_FORCE_INT_SIZE = 0x7FFFFFFF
}
voMusicStemType;

/**
 * Functions composing the `voIMusicStem` interface.
 *
 * This interface exposes the public methods of the `voMusicStem` type.
 *
 * {@implementingTypes voMusicStem}
 */
typedef struct _voIMusicStem
{
  /**
   * Returns the type of stem.
   *
   * @param engine the engine.
   * @param target the target stem.
   *
   * @return the type of stem on success, otherwise `VO_MUSIC_STEM_TYPE_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicStem`.
   */
  voMusicStemType (VO_MSE_CALL *getType)(voEngine engine, voMusicStem target);
}
voIMusicStem;


// -- MusicBeam ----------------------------------------------------------------

/**
 * `voMusicBeam` reference type.
 *
 * {@extends voMusicElement}
 * {@implements voIMusicElement}
 */
typedef voMusicElement voMusicBeam;

/**
 * Represents a beam type.
 */
typedef enum _voMusicBeamType
{
  VO_MUSIC_BEAM_TYPE_ERR = -1,      /**< Error marker. */

  VO_MUSIC_BEAM_TYPE_BEGIN,         /**< Begin. */
  VO_MUSIC_BEAM_TYPE_CONTINUE,      /**< Continue. */
  VO_MUSIC_BEAM_TYPE_END,           /**< End. */
  VO_MUSIC_BEAM_TYPE_FORWARD_HOOK,  /**< Forward hook. */
  VO_MUSIC_BEAM_TYPE_BACKWARD_HOOK, /**< Backward hook. */

  VO_MUSIC_BEAM_TYPE_COUNT,

  _voMusicBeamType_FORCE_INT_SIZE = 0x7FFFFFFF
}
voMusicBeamType;

/**
 * Represents a beam slope.
 */
typedef enum _voMusicBeamSlope
{
  VO_MUSIC_BEAM_SLOPE_ERR = -1,   /**< Error marker. */

  VO_MUSIC_BEAM_SLOPE_HORIZONTAL, /**< Horizontal beam. */
  VO_MUSIC_BEAM_SLOPE_UP,         /**< Upslope beam. */
  VO_MUSIC_BEAM_SLOPE_DOWN,       /**< Downslope beam. */

  VO_MUSIC_BEAM_SLOPE_COUNT,

  _voMusicBeamSlope_FORCE_INT_SIZE = 0x7FFFFFFF
}
voMusicBeamSlope;

/**
 * Functions composing the `voIMusicBeam` interface.
 *
 * This interface exposes the public methods of the `voMusicBeam` type.
 *
 * {@implementingTypes voMusicBeam}
 */
typedef struct _voIMusicBeam
{
  /**
   * Returns the placement of the beam.
   *
   * @param engine the engine.
   * @param target the target beam.
   *
   * @return the placement on success, otherwise `VO_MUSIC_PLACEMENT_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicBeam`.
   */
  voMusicPlacement (VO_MSE_CALL *getPlacement)(voEngine engine, voMusicBeam target);

  /**
   * Return the number of lines on the left half of the beam.
   *
   * @param engine the engine.
   * @param target the target beam.
   *
   * @return the number of lines on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicBeam`.
   */
  int32_t (VO_MSE_CALL *getLeftCount)(voEngine engine, voMusicBeam target);

  /**
   * Return the number of lines on the right half of the beam.
   *
   * @param engine the engine.
   * @param target the target beam.
   *
   * @return the number of lines on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicBeam`.
   */
  int32_t (VO_MSE_CALL *getRightCount)(voEngine engine, voMusicBeam target);
}
voIMusicBeam;


// -- MusicDots ----------------------------------------------------------------

/**
 * `voMusicDots` reference type.
 *
 * {@extends voMusicElement}
 * {@implements voIMusicDots, voIMusicElement}
 */
typedef voMusicElement voMusicDots;

/**
 * Functions composing the `voIMusicDots` interface.
 *
 * This interface exposes the public methods of the `voMusicDots` type.
 *
 * {@implementingTypes voMusicDots}
 */
typedef struct _voIMusicDots
{
  /**
   * Return the number of dots.
   *
   * @param engine the engine.
   * @param target the target dots.
   *
   * @return the number of dots on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicDots`.
   */
  int32_t (VO_MSE_CALL *getCount)(voEngine engine, voMusicDots target);
}
voIMusicDots;


// -- MusicTie -----------------------------------------------------------------

/**
 * `voMusicTie` reference type.
 *
 * {@extends voMusicElement}
 * {@implements voIMusicTie, voIMusicElement}
 */
typedef voMusicElement voMusicTie;

/**
 * Functions composing the `voIMusicTie` interface.
 *
 * This interface exposes the public methods of the `voMusicTie` type.
 *
 * {@implementingTypes voMusicTie}
 */
typedef struct _voIMusicTie
{
  /**
   * Returns the placement of the tie.
   *
   * @param engine the engine.
   * @param target the target tie.
   *
   * @return the placement on success, otherwise `VO_MUSIC_PLACEMENT_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicTie`.
   */
  voMusicPlacement (VO_MSE_CALL *getPlacement)(voEngine engine, voMusicTie target);
}
voIMusicTie;


// -- MusicSlur ----------------------------------------------------------------

/**
 * `voMusicSlur` reference type.
 *
 * {@extends voMusicElement}
 * {@implements voIMusicSlur, voIMusicElement}
 */
typedef voMusicElement voMusicSlur;

/**
 * Functions composing the `voIMusicSlur` interface.
 *
 * This interface exposes the public methods of the `voMusicSlur` type.
 *
 * {@implementingTypes voMusicSlur}
 */
typedef struct _voIMusicSlur
{
  /**
   * Returns the placement of the slur.
   *
   * @param engine the engine.
   * @param target the target slur.
   *
   * @return the placement on success, otherwise `VO_MUSIC_PLACEMENT_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicSlur`.
   */
  voMusicPlacement (VO_MSE_CALL *getPlacement)(voEngine engine, voMusicSlur target);
}
voIMusicSlur;


// -- MusicLedgerLine ----------------------------------------------------------

/**
 * `voMusicLedgerLine` reference type.
 *
 * {@extends voMusicElement}
 * {@implements voIMusicElement}
 */
typedef voMusicElement voMusicLedgerLine;


// -- MusicNote ----------------------------------------------------------------

/**
 * `voMusicNote` reference type.
 *
 * {@extends voMusicElement}
 * {@implements voIMusicNote, voIMusicElement}
 */
typedef voMusicElement voMusicNote;

/**
 * Represents a pitch step.
 */
typedef enum _voMusicPitchStep
{
  VO_MUSIC_PITCH_STEP_UNKNOWN,  /**< Unknown pitch step. */

  VO_MUSIC_PITCH_STEP_A,        /**< A. */
  VO_MUSIC_PITCH_STEP_B,        /**< B. */
  VO_MUSIC_PITCH_STEP_C,        /**< C. */
  VO_MUSIC_PITCH_STEP_D,        /**< D. */
  VO_MUSIC_PITCH_STEP_E,        /**< E. */
  VO_MUSIC_PITCH_STEP_F,        /**< F. */
  VO_MUSIC_PITCH_STEP_G,        /**< G. */

  VO_MUSIC_PITCH_STEP_COUNT,

  _voMusicPitchStep_FORCE_INT_SIZE = 0x7FFFFFFF
}
voMusicPitchStep;

/**
 * Represents pitch data.
 *
 * @see voMusicNote
 */
typedef struct _voMusicPitchData
{
  voMusicPitchStep step;  /**< Pitch data. */
  int32_t octave;         /**< Octave. */
  int32_t alteration;     /**< Chromatic alteration. */
}
voMusicPitchData;

/**
 * Represents a note type.
 *
 * @see voMusicNote
 */
typedef enum _voMusicNoteType
{
  VO_MUSIC_NOTE_TYPE_ERR = -1,      /**< Error marker. */

  VO_MUSIC_NOTE_TYPE_WHOLE,         /**< Whole. */
  VO_MUSIC_NOTE_TYPE_HALF,          /**< Half. */
  VO_MUSIC_NOTE_TYPE_QUARTER,       /**< Quarter. */
  VO_MUSIC_NOTE_TYPE_EIGHTH,        /**< Eighth. */
  VO_MUSIC_NOTE_TYPE_SIXTEENTH,     /**< 16th. */
  VO_MUSIC_NOTE_TYPE_THIRTY_SECOND, /**< 32nd. */
  VO_MUSIC_NOTE_TYPE_SIXTY_FOURTH,  /**< 64th. */

  VO_MUSIC_NOTE_TYPE_COUNT,

  _voMusicNoteType_FORCE_INT_SIZE = 0x7FFFFFFF
}
voMusicNoteType;

/**
 * Represents a time modification type.
 *
 * @see voMusicNote, voMusicTimeModificationData
 */
typedef enum _voMusicTimeModificationType
{
  VO_MUSIC_TIME_MODIFICATION_TYPE_NONE,           /**< None. */
  VO_MUSIC_TIME_MODIFICATION_TYPE_UNKNOWN,        /**< Unknown. */

  VO_MUSIC_TIME_MODIFICATION_TYPE_WHOLE,          /**< Whole. */
  VO_MUSIC_TIME_MODIFICATION_TYPE_HALF,           /**< Half. */
  VO_MUSIC_TIME_MODIFICATION_TYPE_QUARTER,        /**< Quarter. */
  VO_MUSIC_TIME_MODIFICATION_TYPE_EIGHTH,         /**< Eighth. */
  VO_MUSIC_TIME_MODIFICATION_TYPE_SIXTEENTH,      /**< 16th. */
  VO_MUSIC_TIME_MODIFICATION_TYPE_THIRTY_SECOND,  /**< 32nd. */
  VO_MUSIC_TIME_MODIFICATION_TYPE_SIXTY_FOURTH,   /**< 64th. */

  VO_MUSIC_TIME_MODIFICATION_TYPE_COUNT,

  _voMusicTimeModificationType_FORCE_INT_SIZE = 0x7FFFFFFF
}
voMusicTimeModificationType;

/**
 * Represents a time modification.
 *
 * @see voMusicNote
 */
typedef struct _voMusicTimeModificationData
{
  voMusicTimeModificationType type; /**< Type. */
  int32_t actual;                   /**< Actual. */
  int32_t normal;                   /**< Normal. */
  int32_t dots;                     /**< Normal Dots. */
}
voMusicTimeModificationData;

/**
 * Functions composing the `voIMusicNote` interface.
 *
 * This interface exposes the public methods of the `voMusicNote` type.
 *
 * {@implementingTypes voMusicNote}
 */
typedef struct _voIMusicNote
{
  /**
   * Returns the note type.
   *
   * @param engine the engine.
   * @param target the target note.
   *
   * @return the note type on success, otherwise `VO_MUSIC_NOTE_TYPE_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicNote`.
   */
  voMusicNoteType (VO_MSE_CALL *getType)(voEngine engine, voMusicNote target);

  /**
   * Returns the note pitch.
   *
   * @param engine the engine.
   * @param target the target note.
   * @param data the recipient for the data.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicNote`.
   * @throws VO_INVALID_USER_BUFFER when `data` is not writable.
   */
  bool (VO_MSE_CALL *getPitch)(voEngine engine, voMusicNote target, voMusicPitchData* data);

  /**
   * Returns the note duration.
   *
   * @param engine the engine.
   * @param target the target note.
   * @param divisions the number of divisions.
   *
   * @return the note duration on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicNote`.
   * @throws VO_INVALID_ARGUMENT when `division <= 0`.
   */
  int32_t (VO_MSE_CALL *getDuration)(voEngine engine, voMusicNote target, int32_t divisions);

  /**
   * Returns the line on which the note is positioned.
   *
   * @param engine the engine.
   * @param target the target note.
   *
   * @return the line on success, otherwise `INT32_MAX`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicNote`.
   */
  int32_t (VO_MSE_CALL *getLine)(voEngine engine, voMusicNote target);

  /**
   * Returns the note head.
   *
   * @param engine the engine.
   * @param target the target note.
   *
   * @return the note head on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicNote`.
   * @throws VO_LIMIT_EXCEEDED when the number of `voMusicHead` instances would
   *   exceed the allowed limit (please remember that the limit can be shared by
   *   several types).
   */
  voMusicHead (VO_MSE_CALL *getHead)(voEngine engine, voMusicNote target);

  /**
   * Returns the note stem (optional).
   *
   * @param engine the engine.
   * @param target the target note.
   *
   * @return the note stem if any, otherwise `NULL`.
   *   When `NULL`, use `voGetError()` to figure out whether an error occurred.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicNote`.
   * @throws VO_LIMIT_EXCEEDED when the number of `voMusicStem` instances would
   *   exceed the allowed limit (please remember that the limit can be shared by
   *   several types).
   */
  voMusicStem (VO_MSE_CALL *getStem)(voEngine engine, voMusicNote target);

  /**
   * Returns the note accidental (optional).
   *
   * @param engine the engine.
   * @param target the target note.

   * @return the note accidental if any, otherwise `NULL`.
   *   When `NULL`, use `voGetError()` to figure out whether an error occurred.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicNote`.
   * @throws VO_LIMIT_EXCEEDED when the number of `voMusicAccidental` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   */
  voMusicAccidental (VO_MSE_CALL *getAccidental)(voEngine engine, voMusicNote target);

  /**
   * Returns the note dots (optional).
   *
   * @param engine the engine.
   * @param target the target note.
   *
   * @return the note dots if any, otherwise `NULL`.
   *   When `NULL`, use `voGetError()` to figure out whether an error occurred.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicNote`.
   * @throws VO_LIMIT_EXCEEDED when the number of `voMusicDots` instances would
   *   exceed the allowed limit (please remember that the limit can be shared by
   *   several types).
   */
  voMusicDots (VO_MSE_CALL *getDots)(voEngine engine, voMusicNote target);

  /**
   * Returns the number of beam types on this note.
   *
   * @param engine the engine.
   * @param target the target note.
   *
   * @return the number of beam types on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicNote`.
   */
  int32_t (VO_MSE_CALL *getBeamTypeCount)(voEngine engine, voMusicNote target);

  /**
   * Returns the beam type at the specified index.
   *
   * @param engine the engine.
   * @param target the target note.
   * @param index the index of the requested beam.
   *
   * @return the beam type at the specified index on success, otherwise
   *   `VO_MUSIC_BEAM_TYPE_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicNote`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   */
  voMusicBeamType (VO_MSE_CALL *getBeamTypeAt)(voEngine engine, voMusicNote target, int32_t index);

  /**
   * Returns the beam starting from this note (optional).
   *
   * @param engine the engine.
   * @param target the target note.
   *
   * @return the starting beam if any, otherwise `NULL`.
   *   When `NULL`, use `voGetError()` to figure out whether an error occurred.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicNote`.
   * @throws VO_LIMIT_EXCEEDED when the number of `voMusicBeam` instances would
   *   exceed the allowed limit (please remember that the limit can be shared by
   *   several types).
   */
  voMusicBeam (VO_MSE_CALL *getStartBeam)(voEngine engine, voMusicNote target);

  /**
   * Returns the beam stopping on this note (optional).
   *
   * @param engine the engine.
   * @param target the target note.
   *
   * @return the stopping beam if any, otherwise `NULL`.
   *   When `NULL`, use `voGetError()` to figure out whether an error occurred.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicNote`.
   * @throws VO_LIMIT_EXCEEDED when the number of `voMusicBeam` instances would
   *   exceed the allowed limit (please remember that the limit can be shared by
   *   several types).
   */
  voMusicBeam (VO_MSE_CALL *getStopBeam)(voEngine engine, voMusicNote target);

  /**
   * Returns the tie starting from this note (optional).
   *
   * @param engine the engine.
   * @param target the target note.
   *
   * @return the starting tie if any, otherwise `NULL`.
   *   When `NULL`, use `voGetError()` to figure out whether an error occurred.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicNote`.
   * @throws VO_LIMIT_EXCEEDED when the number of `voMusicTie` instances would
   *   exceed the allowed limit (please remember that the limit can be shared by
   *   several types).
   */
  voMusicTie (VO_MSE_CALL *getStartTie)(voEngine engine, voMusicNote target);

  /**
   * Returns the tie stopping on this note (optional).
   *
   * @param engine the engine.
   * @param target the target note.
   *
   * @return the stopping tie if any, otherwise `NULL`.
   *   When `NULL`, use `voGetError()` to figure out whether an error occurred.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicNote`.
   * @throws VO_LIMIT_EXCEEDED when the number of `voMusicTie` instances would
   *   exceed the allowed limit (please remember that the limit can be shared by
   *   several types).
   */
  voMusicTie (VO_MSE_CALL *getStopTie)(voEngine engine, voMusicNote target);

  /**
   * Returns the number of slurs starting from this note.
   *
   * @param engine the engine.
   * @param target the target note.
   *
   * @return the number of slurs on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicNote`.
   */
  int32_t (VO_MSE_CALL *getStartSlurCount)(voEngine engine, voMusicNote target);

  /**
   * Returns the starting slur at the specified index.
   *
   * @param engine the engine.
   * @param target the target note.
   * @param index the index of the requested slur.
   *
   * @return the slur on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicNote`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_LIMIT_EXCEEDED when the number of `voMusicSlur` instances would
   *   exceed the allowed limit (please remember that the limit can be shared by
   *   several types).
   */
  voMusicSlur (VO_MSE_CALL *getStartSlurAt)(voEngine engine, voMusicNote target, int32_t index);

  /**
   * Returns the number of slurs stopping on this note.
   *
   * @param engine the engine.
   * @param target the target note.
   *
   * @return the number of slurs on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicNote`.
   */
  int32_t (VO_MSE_CALL *getStopSlurCount)(voEngine engine, voMusicNote target);

  /**
   * Returns the stopping slur at the specified index.
   *
   * @param engine the engine.
   * @param target the target note.
   * @param index the index of the requested slur.
   *
   * @return the slur on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicNote`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_LIMIT_EXCEEDED when the number of `voMusicSlur` instances would
   *   exceed the allowed limit (please remember that the limit can be shared by
   *   several types).
   */
  voMusicSlur (VO_MSE_CALL *getStopSlurAt)(voEngine engine, voMusicNote target, int32_t index);

  /**
   * Returns the number of ledger lines for this note.
   *
   * @param engine the engine.
   * @param target the target note.
   *
   * @return the number of ledger lines on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicNote`.
   */
  int32_t (VO_MSE_CALL *getLedgerLineCount)(voEngine engine, voMusicNote target);

  /**
   * Returns the ledger line at the specified index.
   *
   * @param engine the engine.
   * @param target the target note.
   * @param index the index of the requested ledger line.
   *
   * @return the ledger line on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicNote`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_LIMIT_EXCEEDED when the number of `voMusicLedgerLine` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   */
  voMusicLedgerLine (VO_MSE_CALL *getLedgerLineAt)(voEngine engine, voMusicNote target, int32_t index);

  /**
   * Returns the number of decorations on this note.
   *
   * @param engine the engine.
   * @param target the target note.
   *
   * @return the number of decorations on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicNote`.
   */
  int32_t (VO_MSE_CALL *getDecorationCount)(voEngine engine, voMusicNote target);

  /**
   * Returns the decoration at the specified index.
   *
   * @param engine the engine.
   * @param target the target note.
   * @param index the index of the requested decoration.
   *
   * @return the decoration on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicNote`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_LIMIT_EXCEEDED when the number of `voMusicDecoration` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   */
  voMusicDecoration (VO_MSE_CALL *getDecorationAt)(voEngine engine, voMusicNote target, int32_t index);

  /**
   * Returns the tuplet starting from this note (optional).
   *
   * @param engine the engine.
   * @param target the target note.
   *
   * @return the starting tuplet if any, otherwise `NULL`.
   *   When `NULL`, use `voGetError()` to figure out whether an error occurred.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicNote`.
   * @throws VO_LIMIT_EXCEEDED when the number of `voMusicTuplet` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   *
   * @since 7.0_0
   **/
  voMusicTuplet (VO_MSE_CALL *getStartTuplet)(voEngine engine, voMusicNote target);

  /**
   * Returns the tuplet stopping on this note (optional).
   *
   * @param engine the engine.
   * @param target the target note.
   *
   * @return the stopping tuplet if any, otherwise `NULL`.
   *   When `NULL`, use `voGetError()` to figure out whether an error occurred.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicNote`.
   * @throws VO_LIMIT_EXCEEDED when the number of `voMusicTuplet` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   *
   * @since 7.0_0
   **/
  voMusicTuplet (VO_MSE_CALL *getStopTuplet)(voEngine engine, voMusicNote target);

   /**
   * Returns the time modification for this note (optional).
   *
   * @param engine the engine.
   * @param target the target note.
   * @param data the recipient for the data.
   *
   * @return true on success else false.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicNote`.
   * @throws VO_INVALID_USER_BUFFER when `data` is not writable.
   *
   * @since 7.0_0
   **/
  bool (VO_MSE_CALL *getTimeModification)(voEngine engine, voMusicNote target, voMusicTimeModificationData* data);
}
voIMusicNote;


// -- MusicRest ----------------------------------------------------------------

/**
 * `voMusicRest` reference type.
 *
 * {@extends voMusicElement}
 * {@implements voIMusicRest, voIMusicElement}
 */
typedef voMusicElement voMusicRest;

/**
 * Represents a rest type.
 */
typedef enum _voMusicRestType
{
  VO_MUSIC_REST_TYPE_ERR = -1,      /**< Error marker. */

  VO_MUSIC_REST_TYPE_WHOLE,         /**< Whole. */
  VO_MUSIC_REST_TYPE_HALF,          /**< Half. */
  VO_MUSIC_REST_TYPE_QUARTER,       /**< Quarter. */
  VO_MUSIC_REST_TYPE_EIGHTH,        /**< Eighth. */
  VO_MUSIC_REST_TYPE_SIXTEENTH,     /**< 16th. */
  VO_MUSIC_REST_TYPE_THIRTY_SECOND, /**< 32nd. */
  VO_MUSIC_REST_TYPE_SIXTY_FOURTH,  /**< 64th. */

  VO_MUSIC_REST_TYPE_COUNT,

  _voMusicRestType_FORCE_INT_SIZE = 0x7FFFFFFF
}
voMusicRestType;

/**
 * Functions composing the `voIMusicRest` interface.
 *
 * This interface exposes the public methods of the `voMusicRest` type.
 *
 * {@implementingTypes voMusicRest}
 */
typedef struct _voIMusicRest
{
  /**
   * Returns the rest type.
   *
   * @param engine the engine.
   * @param target the target rest.
   *
   * @return the rest type on success, otherwise `VO_MUSIC_REST_TYPE_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicRest`.
   */
  voMusicRestType (VO_MSE_CALL *getType)(voEngine engine, voMusicRest target);

  /**
   * Returns the rest duration.
   *
   * @param engine the engine.
   * @param target the target rest.
   * @param divisions the number of divisions.
   *
   * @return the rest duration on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicRest`.
   * @throws VO_INVALID_ARGUMENT when `division <= 0`.
   */
  int32_t (VO_MSE_CALL *getDuration)(voEngine engine, voMusicRest target, int32_t divisions);

  /**
   * Returns the rest dots (optional).
   *
   * @param engine the engine.
   * @param target the target rest.
   *
   * @return the rest dots if any, otherwise `NULL`.
   *   When `NULL`, use `voGetError()` to figure out whether an error occurred.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicRest`.
   * @throws VO_LIMIT_EXCEEDED when the number of `voMusicDots` instances would
   *   exceed the allowed limit (please remember that the limit can be shared by
   *   several types).
   */
  voMusicDots (VO_MSE_CALL *getDots)(voEngine engine, voMusicRest target);

  /**
   * Returns the number of slurs starting from this rest.
   *
   * @param engine the engine.
   * @param target the target rest.
   *
   * @return the number of slurs on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicRest`.
   */
  int32_t (VO_MSE_CALL *getStartSlurCount)(voEngine engine, voMusicRest target);

  /**
   * Returns the starting slur at the specified index.
   *
   * @param engine the engine.
   * @param target the target rest.
   * @param index the index of the requested slur.
   *
   * @return the slur on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicRest`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_LIMIT_EXCEEDED when the number of `voMusicSlur` instances would
   *   exceed the allowed limit (please remember that the limit can be shared by
   *   several types).
   */
  voMusicSlur (VO_MSE_CALL *getStartSlurAt)(voEngine engine, voMusicRest target, int32_t index);

  /**
   * Returns the number of slurs stopping on this rest.
   *
   * @param engine the engine.
   * @param target the target rest.
   *
   * @return the number of slurs on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicRest`.
   */
  int32_t (VO_MSE_CALL *getStopSlurCount)(voEngine engine, voMusicRest target);

  /**
   * Returns the stopping slur at the specified index.
   *
   * @param engine the engine.
   * @param target the target rest.
   * @param index the index of the requested slur.
   *
   * @return the slur on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicRest`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_LIMIT_EXCEEDED when the number of `voMusicSlur` instances would
   *   exceed the allowed limit (please remember that the limit can be shared by
   *   several types).
   */
  voMusicSlur (VO_MSE_CALL *getStopSlurAt)(voEngine engine, voMusicRest target, int32_t index);

  /**
   * Returns the number of decorations on this rest.
   *
   * @param engine the engine.
   * @param target the target rest.
   *
   * @return the number of decorations on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicRest`.
   */
  int32_t (VO_MSE_CALL *getDecorationCount)(voEngine engine, voMusicRest target);

  /**
   * Returns the decoration at the specified index.
   *
   * @param engine the engine.
   * @param target the target rest.
   * @param index the index of the requested decoration.
   *
   * @return the decoration on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicRest`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_LIMIT_EXCEEDED when the number of `voMusicDecoration` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   */
  voMusicDecoration (VO_MSE_CALL *getDecorationAt)(voEngine engine, voMusicRest target, int32_t index);

  /**
   * Returns the tuplet starting from this rest (optional).
   *
   * @param engine the engine.
   * @param target the target rest.
   *
   * @return the starting tuplet if any, otherwise `NULL`.
   *   When `NULL`, use `voGetError()` to figure out whether an error occurred.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicRest`.
   * @throws VO_LIMIT_EXCEEDED when the number of `voMusicTuplet` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   *
   * @since 7.0_0
   **/
  voMusicTuplet (VO_MSE_CALL *getStartTuplet)(voEngine engine, voMusicRest target);

  /**
   * Returns the tuplet stopping on this rest (optional).
   *
   * @param engine the engine.
   * @param target the target rest.
   *
   * @return the stopping tuplet if any, otherwise `NULL`.
   *   When `NULL`, use `voGetError()` to figure out whether an error occurred.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicRest`.
   * @throws VO_LIMIT_EXCEEDED when the number of `voMusicTuplet` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   *
   * @since 7.0_0
   **/
  voMusicTuplet (VO_MSE_CALL *getStopTuplet)(voEngine engine, voMusicRest target);

   /**
    * Returns the time modification for this rest (optional).
    *
    * @param engine the engine.
    * @param target the target rest.
    * @param data the recipient for the data.
    *
    * @return true on success else false.
    *
    * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
    * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
    * @throws VO_INVALID_OPERATION when `target` is not a `voMusicRest`.
    * @throws VO_INVALID_USER_BUFFER when `data` is not writable.
    *
    * @since 7.0_0
   **/
  bool (VO_MSE_CALL *getTimeModification)(voEngine engine, voMusicRest target, voMusicTimeModificationData* data);
}
voIMusicRest;


// -- MusicArpeggiate ----------------------------------------------------------

/**
 * `voMusicArpeggiate` reference type.
 *
 * {@extends voMusicElement}
 * {@implements voIMusicArpeggiate, voIMusicElement}
 */
typedef voMusicElement voMusicArpeggiate;

/**
 * Represents a music arpeggiate type.
 *
 * @since 6.3_0
 */
typedef enum _voMusicArpeggiateType
{
  VO_MUSIC_ARPEGGIATE_TYPE_ERR = -1,        /**< Error marker. */

  VO_MUSIC_ARPEGGIATE_TYPE_NORMAL,          /**< Arpeggiate. */
  VO_MUSIC_ARPEGGIATE_TYPE_UP,              /**< Arpeggiate up. */
  VO_MUSIC_ARPEGGIATE_TYPE_DOWN,            /**< Arpeggiate down. */
  VO_MUSIC_ARPEGGIATE_TYPE_NON_ARPEGGIATE,  /**< Non-arpeggiate. */

  VO_MUSIC_ARPEGGIATE_TYPE_COUNT,

  _voMusicArpeggiateType_FORCE_INT_SIZE = 0x7FFFFFFF
}
voMusicArpeggiateType;

/**
 * Functions composing the `voIMusicArpeggiate` interface.
 *
 * This interface exposes the public methods of the `voMusicArpeggiate` type.
 *
 * {@implementingTypes voMusicArpeggiate}
 *
 * @since 6.3_0
 */
typedef struct _voIMusicArpeggiate
{
  /**
   * Returns the arpeggiate symbol.
   *
   * @param engine the engine.
   * @param target the target arpeggiate.
   *
   * @return the arpeggiate symbol on success, otherwise
   *   `VO_MUSIC_ARPEGGIATE_TYPE_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicArpeggiate`.
   */
  voMusicArpeggiateType (VO_MSE_CALL *getType)(voEngine engine, voMusicArpeggiate target);
}
voIMusicArpeggiate;


// -- MusicChord ---------------------------------------------------------------

/**
 * `voMusicChord` reference type.
 *
 * {@extends voMusicElement}
 * {@implements voIMusicChord, voIMusicElement}
 */
typedef voMusicElement voMusicChord;

/**
 * Functions composing the `voIMusicChord` interface.
 *
 * This interface exposes the public methods of the `voMusicChord` type.
 *
 * {@implementingTypes voMusicChord}
 */
typedef struct _voIMusicChord
{
  /**
   * Returns the chord duration.
   *
   * @param engine the engine.
   * @param target the target chord.
   * @param divisions the number of divisions.
   *
   * @return the chord duration on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicChord`.
   * @throws VO_INVALID_ARGUMENT when `division <= 0`.
   */
  int32_t (VO_MSE_CALL *getDuration)(voEngine engine, voMusicChord target, int32_t divisions);

  /**
   * Returns the chord stem (optional).
   *
   * @param engine the engine.
   * @param target the target chord.
   *
   * @return the chord stem if any, otherwise `NULL`.
   *   When `NULL`, use `voGetError()` to figure out whether an error occurred.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicChord`.
   * @throws VO_LIMIT_EXCEEDED when the number of `voMusicStem` instances would
   *   exceed the allowed limit (please remember that the limit can be shared by
   *   several types).
   */
  voMusicStem (VO_MSE_CALL *getStem)(voEngine engine, voMusicChord target);

  /**
   * Returns the number of beam types on this chord.
   *
   * @param engine the engine.
   * @param target the target chord.
   *
   * @return the number of beam types on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicChord`.
   */
  int32_t (VO_MSE_CALL *getBeamTypeCount)(voEngine engine, voMusicChord target);

  /**
   * Returns the beam type at the specified index.
   *
   * @param engine the engine.
   * @param target the target chord.
   * @param index the index of the requested beam.
   *
   * @return the beam type at the specified index on success, otherwise
   *   `VO_MUSIC_BEAM_TYPE_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicChord`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   */
  voMusicBeamType (VO_MSE_CALL *getBeamTypeAt)(voEngine engine, voMusicChord target, int32_t index);

  /**
   * Returns the beam starting from this chord (optional).
   *
   * @param engine the engine.
   * @param target the target chord.
   *
   * @return the starting beam if any, otherwise `NULL`.
   *   When `NULL`, use `voGetError()` to figure out whether an error occurred.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicChord`.
   * @throws VO_LIMIT_EXCEEDED when the number of `voMusicBeam` instances would
   *   exceed the allowed limit (please remember that the limit can be shared by
   *   several types).
   */
  voMusicBeam (VO_MSE_CALL *getStartBeam)(voEngine engine, voMusicChord target);

  /**
   * Returns the beam stopping on this chord (optional).
   *
   * @param engine the engine.
   * @param target the target chord.
   *
   * @return the stopping beam if any, otherwise `NULL`.
   *   When `NULL`, use `voGetError()` to figure out whether an error occurred.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicChord`.
   * @throws VO_LIMIT_EXCEEDED when the number of `voMusicBeam` instances would
   *   exceed the allowed limit (please remember that the limit can be shared by
   *   several types).
   */
  voMusicBeam (VO_MSE_CALL *getStopBeam)(voEngine engine, voMusicChord target);

  /**
   * Returns the number of slurs starting from this chord.
   *
   * @param engine the engine.
   * @param target the target chord.
   *
   * @return the number of slurs on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicChord`.
   */
  int32_t (VO_MSE_CALL *getStartSlurCount)(voEngine engine, voMusicChord target);

  /**
   * Returns the starting slur at the specified index.
   *
   * @param engine the engine.
   * @param target the target chord.
   * @param index the index of the requested slur.
   *
   * @return the slur on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicChord`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_LIMIT_EXCEEDED when the number of `voMusicSlur` instances would
   *   exceed the allowed limit (please remember that the limit can be shared by
   *   several types).
   */
  voMusicSlur (VO_MSE_CALL *getStartSlurAt)(voEngine engine, voMusicChord target, int32_t index);

  /**
   * Returns the number of slurs stopping on this chord.
   *
   * @param engine the engine.
   * @param target the target chord.
   *
   * @return the number of slurs on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicChord`.
   */
  int32_t (VO_MSE_CALL *getStopSlurCount)(voEngine engine, voMusicChord target);

  /**
   * Returns the stopping slur at the specified index.
   *
   * @param engine the engine.
   * @param target the target chord.
   * @param index the index of the requested slur.
   *
   * @return the slur on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicChord`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_LIMIT_EXCEEDED when the number of `voMusicSlur` instances would
   *   exceed the allowed limit (please remember that the limit can be shared by
   *   several types).
   */
  voMusicSlur (VO_MSE_CALL *getStopSlurAt)(voEngine engine, voMusicChord target, int32_t index);

  /**
   * Returns the number of ledger lines for this chord.
   *
   * @param engine the engine.
   * @param target the target chord.
   *
   * @return the number of ledger lines on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicChord`.
   */
  int32_t (VO_MSE_CALL *getLedgerLineCount)(voEngine engine, voMusicChord target);

  /**
   * Returns the ledger line at the specified index.
   *
   * @param engine the engine.
   * @param target the target chord.
   * @param index the index of the requested ledger line.
   *
   * @return the ledger line on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicChord`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_LIMIT_EXCEEDED when the number of `voMusicLedgerLine` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   */
  voMusicLedgerLine (VO_MSE_CALL *getLedgerLineAt)(voEngine engine, voMusicChord target, int32_t index);

  /**
   * Returns the number of decorations on this chord.
   *
   * @param engine the engine.
   * @param target the target chord.
   *
   * @return the number of decorations on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicChord`.
   */
  int32_t (VO_MSE_CALL *getDecorationCount)(voEngine engine, voMusicChord target);

  /**
   * Returns the decoration at the specified index.
   *
   * @param engine the engine.
   * @param target the target chord.
   * @param index the index of the requested decoration.
   *
   * @return the decoration on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicChord`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_LIMIT_EXCEEDED when the number of `voMusicDecoration` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   */
  voMusicDecoration (VO_MSE_CALL *getDecorationAt)(voEngine engine, voMusicChord target, int32_t index);

  /**
   * Returns the number of notes in this chord.
   *
   * @param engine the engine.
   * @param target the target chord.
   *
   * @return the number of notes on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicChord`.
   */
  int32_t (VO_MSE_CALL *getNoteCount)(voEngine engine, voMusicChord target);

  /**
   * Returns the note at the specified index.
   *
   * @param engine the engine.
   * @param target the target chord.
   * @param index the index of the requested note.
   *
   * @return the note on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicChord`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_LIMIT_EXCEEDED when the number of `voMusicNote` instances would
   *   exceed the allowed limit (please remember that the limit can be shared by
   *   several types).
   */
  voMusicNote (VO_MSE_CALL *getNoteAt)(voEngine engine, voMusicChord target, int32_t index);

  /**
   * Returns the chord arpeggiate (optional).
   *
   * @param engine the engine.
   * @param target the target chord.
   *
   * @return the arpeggiate if any, otherwise `NULL`.
   *   When `NULL`, use `voGetError()` to figure out whether an error occurred.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicChord`.
   * @throws VO_LIMIT_EXCEEDED when the number of `voMusicArpeggiate` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   *
   * @since 6.3_0
   */
  voMusicArpeggiate (VO_MSE_CALL *getArpeggiate)(voEngine engine, voMusicChord target);

  /**
   * Returns the tuplet starting from this chord (optional).
   *
   * @param engine the engine.
   * @param target the target chord.
   *
   * @return the starting tuplet if any, otherwise `NULL`.
   *   When `NULL`, use `voGetError()` to figure out whether an error occurred.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicChord`.
   * @throws VO_LIMIT_EXCEEDED when the number of `voMusicTuplet` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   *
   * @since 7.0_0
   **/
  voMusicTuplet (VO_MSE_CALL *getStartTuplet)(voEngine engine, voMusicChord target);

  /**
   * Returns the tuplet stopping on this chord (optional).
   *
   * @param engine the engine.
   * @param target the target chord.
   *
   * @return the stopping tuplet if any, otherwise `NULL`.
   *   When `NULL`, use `voGetError()` to figure out whether an error occurred.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicChord`.
   * @throws VO_LIMIT_EXCEEDED when the number of `voMusicTuplet` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   *
   * @since 7.0_0
   **/
  voMusicTuplet (VO_MSE_CALL *getStopTuplet)(voEngine engine, voMusicChord target);
}
voIMusicChord;


// -- MusicPart ----------------------------------------------------------------

/**
 * `voMusicPart` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voIMusicPart}
 */
typedef voEngineObject voMusicPart;

/**
 * Functions composing the `voIMusicPart` interface.
 *
 * This interface exposes the public methods of the `voMusicPart` type.
 *
 * {@implementingTypes voMusicPart}
 */
typedef struct _voIMusicPart
{
  /**
   * Returns the number of elements in this part.
   *
   * @param engine the engine.
   * @param target the target part.
   *
   * @return the number of elements on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicPart`.
   */
  int32_t (VO_MSE_CALL *getElementCount)(voEngine engine, voMusicPart target);

  /**
   * Returns the element at the specified index.
   *
   * @param engine the engine.
   * @param target the target part.
   * @param index the index of the requested element.
   *
   * @return the element on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicPart`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_LIMIT_EXCEEDED when the number of `voMusicElement` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   */
  voMusicElement (VO_MSE_CALL *getElementAt)(voEngine engine, voMusicPart target, int32_t index);
}
voIMusicPart;


// -- MusicScore ---------------------------------------------------------------

/**
 * `voMusicScore` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voIMusicScore}
 */
typedef voEngineObject voMusicScore;

/**
 * Functions composing the `voIMusicScore` interface.
 *
 * This interface exposes the public methods of the `voMusicScore` type.
 *
 * {@implementingTypes voMusicScore}
 */
typedef struct _voIMusicScore
{
  /**
   * Returns the number of parts in this document.
   *
   * @param engine the engine.
   * @param target the target score.
   *
   * @return the number of parts on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicScore`.
   */
  int32_t (VO_MSE_CALL *getPartCount)(voEngine engine, voMusicScore target);

  /**
   * Returns the part at the specified index.
   *
   * @param engine the engine.
   * @param target the target score.
   * @param index the index of the requested part.
   *
   * @return the part on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicScore`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_LIMIT_EXCEEDED when the number of `voMusicPart` instances would
   *   exceed the allowed limit (please remember that the limit can be shared by
   *   several types).
   */
  voMusicPart (VO_MSE_CALL *getPartAt)(voEngine engine, voMusicScore target, int32_t index);
}
voIMusicScore;


// -- MusicScratchOut ----------------------------------------------------------

/**
 * `voMusicScratchOut` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voIMusicScratchOut}
 *
 * @since 6.1_0
 */
typedef voEngineObject voMusicScratchOut;

/**
 * Functions composing the `voIMusicScratchOut` interface.
 *
 * This interface exposes the public methods of the `voMusicScratchOut` type.
 *
 * {@implementingTypes voMusicScratchOut}
 *
 * @since 6.1_0
 */
typedef struct _voIMusicScratchOut
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
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicScratchOut`.
   * @throws VO_INVALID_STATE when the portion of a MyScript Document associated
   *   with this scratch out has been modified, invalidating input ranges.
   */
  int32_t (VO_MSE_CALL *getInputRangeCount)(voEngine engine, voMusicScratchOut target);

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
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicScratchOut`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_INVALID_USER_BUFFER when `range` is not writable.
   * @throws VO_INVALID_STATE when the portion of a MyScript Document associated
   *   with this scratch out has been modified, invalidating input ranges.
   */
  bool (VO_MSE_CALL *getInputRangeAt)(voEngine engine, voMusicScratchOut target, int32_t index, voMusicInputRange* range);

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
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicScratchOut`.
   * @throws VO_INVALID_STATE when the portion of a MyScript Document associated
   *   with this scratch out has been modified, invalidating input ranges.
   */
  int32_t (VO_MSE_CALL *getErasedInputRangeCount)(voEngine engine, voMusicScratchOut target);

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
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicScratchOut`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_INVALID_USER_BUFFER when `range` is not writable.
   * @throws VO_INVALID_STATE when the portion of a MyScript Document associated
   *   with this scratch out has been modified, invalidating input ranges.
   */
  bool (VO_MSE_CALL *getErasedInputRangeAt)(voEngine engine, voMusicScratchOut target, int32_t index, voMusicInputRange* range);
}
voIMusicScratchOut;


// -- MusicDocument ------------------------------------------------------------

/**
 * `voMusicDocument` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voIMusicDocument}
 */
typedef voEngineObject voMusicDocument;

/**
 * Functions composing the `voIMusicDocument` interface.
 *
 * This interface exposes the public methods of the `voMusicDocument` type.
 *
 * {@implementingTypes voMusicDocument}
 */
typedef struct _voIMusicDocument
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
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicDocument`.
   */
  bool (VO_MSE_CALL *clear)(voEngine engine, voMusicDocument target);

  /**
   * Adds a staff by defining line positions.
   *
   * @param engine the engine.
   * @param target the target document.
   * @param count the number of lines in the staff.
   * @param top the position of the top line along the y axis.
   * @param gap the distance between two consecutive lines.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicDocument`.
   * @throws VO_LIMIT_EXCEEDED when the number of staves would exceed the
   *   current limit of `1`.
   * @throws VO_INVALID_ARGUMENT when `count != 5`.
   * @throws VO_INVALID_ARGUMENT when `gap <= 0`.
   */
  bool (VO_MSE_CALL *addStaff)(voEngine engine, voMusicDocument target, int32_t count, float top, float gap);

  /**
   * Adds a pen stroke to this document.
   *
   * @param engine the engine.
   * @param target the target music document.
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
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicDocument`.
   * @throws VO_INVALID_ARGUMENT when `xByteStride == 0`.
   * @throws VO_INVALID_ARGUMENT when `yByteStride == 0`.
   * @throws VO_INVALID_ARGUMENT when `pointCount <= 0`.
   * @throws VO_INVALID_USER_BUFFER when the combination of `x`, `xByteStride`,
   *   `y`, `yByteStride` and `pointCount` leads to invalid memory access.
   * @throws VO_LIMIT_EXCEEDED when `pointCount > 32767`.
   */
  bool (VO_MSE_CALL *addStroke)(voEngine engine, voMusicDocument target, const float* x, size_t xByteStride, const float* y, size_t yByteStride, int32_t pointCount);

  /**
   * Adds a decoration with respect to its position in this document.
   *
   * @param engine the engine.
   * @param target the target document.
   * @param symbol the symbol of the decoration to be added.
   * @param placement the placement of the decoration to be added.
   * @param x the origin of the element bounding box along the x axis.
   * @param y the origin of the element bounding box along the y axis.
   * @param width the width of the element bounding box.
   * @param height the height of the element bounding box.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicDocument`.
   * @throws VO_INVALID_ARGUMENT when `symbol` is invalid.
   * @throws VO_INVALID_ARGUMENT when `placement` is invalid.
   * @throws VO_INVALID_ARGUMENT when `width <= 0`.
   * @throws VO_INVALID_ARGUMENT when `height <= 0`.
   */
  bool (VO_MSE_CALL *addDecoration)(voEngine engine, voMusicDocument target,
                                    voMusicDecorationSymbol symbol, voMusicPlacement placement,
                                    float x, float y, float width, float height);

  /**
   * Adds a bar with respect to its position in this document.
   *
   * @param engine the engine.
   * @param target the target document.
   * @param style the style of the bar to be added.
   * @param repeatDirection the repeat direction of the bar to be added.
   * @param x the origin of the element bounding box along the x axis.
   * @param y the origin of the element bounding box along the y axis.
   * @param width the width of the element bounding box.
   * @param height the height of the element bounding box.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicDocument`.
   * @throws VO_INVALID_ARGUMENT when `style` is invalid.
   * @throws VO_INVALID_ARGUMENT when `repeatDirection` is invalid.
   * @throws VO_INVALID_ARGUMENT when `width <= 0`.
   * @throws VO_INVALID_ARGUMENT when `height <= 0`.
   */
  bool (VO_MSE_CALL *addBar)( voEngine engine, voMusicDocument target,
                              voMusicBarStyle style, voMusicBarRepeatDirection repeatDirection,
                              float x, float y, float width, float height);

  /**
   * Adds a clef with respect to its position in this document.
   *
   * @param engine the engine.
   * @param target the target document.
   * @param symbol the symbol of the clef to be added.
   * @param octave the octave of the clef to be added.
   * @param yAnchor the y position of the anchor point of the clef to be added.
   * @param x the origin of the element bounding box along the x axis.
   * @param y the origin of the element bounding box along the y axis.
   * @param width the width of the element bounding box.
   * @param height the height of the element bounding box.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicDocument`.
   * @throws VO_INVALID_ARGUMENT when `symbol` is invalid.
   * @throws VO_INVALID_ARGUMENT when `octave` is invalid.
   * @throws VO_INVALID_ARGUMENT when `width <= 0`.
   * @throws VO_INVALID_ARGUMENT when `height <= 0`.
   */
  bool (VO_MSE_CALL *addClef)(voEngine engine, voMusicDocument target,
                              voMusicClefSymbol symbol, int32_t octave, float yAnchor,
                              float x, float y, float width, float height);

  /**
   * Adds an accidental with respect to its position in this document.
   *
   * @param engine the engine.
   * @param target the target document.
   * @param type the type of accidental to be added.
   * @param x the origin of the element bounding box along the x axis.
   * @param y the origin of the element bounding box along the y axis.
   * @param width the width of the element bounding box.
   * @param height the height of the element bounding box.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicDocument`.
   * @throws VO_INVALID_ARGUMENT when `type` is invalid.
   * @throws VO_INVALID_ARGUMENT when `width <= 0`.
   * @throws VO_INVALID_ARGUMENT when `height <= 0`.
   */
  bool (VO_MSE_CALL *addAccidental)(voEngine engine, voMusicDocument target,
                                    voMusicAccidentalType type,
                                    float x, float y, float width, float height);

  /**
   * Adds a time signature with respect to its position in this document.
   *
   * @param engine the engine.
   * @param target the target document.
   * @param type the type of time signature to be added (common or alla breve).
   * @param x the origin of the element bounding box along the x axis.
   * @param y the origin of the element bounding box along the y axis.
   * @param width the width of the element bounding box.
   * @param height the height of the element bounding box.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicDocument`.
   * @throws VO_INVALID_ARGUMENT when `type` is invalid.
   * @throws VO_INVALID_ARGUMENT when `width <= 0`.
   * @throws VO_INVALID_ARGUMENT when `height <= 0`.
   */
  bool (VO_MSE_CALL *addTimeSignature)( voEngine engine, voMusicDocument target,
                                        voMusicTimeSignatureType type,
                                        float x, float y, float width, float height);

  /**
   * Adds a note head with respect to its position in this document.
   *
   * @param engine the engine.
   * @param target the target document.
   * @param type the type of head to be added.
   * @param x the origin of the element bounding box along the x axis.
   * @param y the origin of the element bounding box along the y axis.
   * @param width the width of the element bounding box.
   * @param height the height of the element bounding box.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicDocument`.
   * @throws VO_INVALID_ARGUMENT when `type` is invalid.
   * @throws VO_INVALID_ARGUMENT when `width <= 0`.
   * @throws VO_INVALID_ARGUMENT when `height <= 0`.
   */
  bool (VO_MSE_CALL *addHead)(voEngine engine, voMusicDocument target,
                              voMusicHeadType type,
                              float x, float y, float width, float height);

  /**
   * Adds a note stem with respect to its position in this document.
   *
   * @param engine the engine.
   * @param target the target document.
   * @param type the type of stem to be added.
   * @param x the origin of the element bounding box along the x axis.
   * @param y the origin of the element bounding box along the y axis.
   * @param width the width of the element bounding box.
   * @param height the height of the element bounding box.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicDocument`.
   * @throws VO_INVALID_ARGUMENT when `type` is invalid.
   * @throws VO_INVALID_ARGUMENT when `width <= 0`.
   * @throws VO_INVALID_ARGUMENT when `height <= 0`.
   */
  bool (VO_MSE_CALL *addStem)(voEngine engine, voMusicDocument target,
                              voMusicStemType type,
                              float x, float y, float width, float height);

  /**
   * Adds a note beam with respect to its position in this document.
   *
   * @param engine the engine.
   * @param target the target document.
   * @param placement the placement of the beam to be added.
   * @param leftCount the number of lines on the left half of the beam to be
   *   added.
   * @param rightCount the number of lines on the right half of the beam to be
   *   added.
   * @param slope the slope of the beam to be added.
   * @param gap the distance between two consecutive lines.
   * @param x the origin of the element bounding box along the x axis.
   * @param y the origin of the element bounding box along the y axis.
   * @param width the width of the element bounding box.
   * @param height the height of the element bounding box.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicDocument`.
   * @throws VO_INVALID_ARGUMENT when `placement` is invalid.
   * @throws VO_INVALID_ARGUMENT When `leftCount <= 0`
   * @throws VO_INVALID_ARGUMENT When `leftCount > 4`
   * @throws VO_INVALID_ARGUMENT When `rightCount <= 0`
   * @throws VO_INVALID_ARGUMENT When `rightCount > 4`
   * @throws VO_INVALID_ARGUMENT when `slope` is invalid.
   * @throws VO_INVALID_ARGUMENT when `gap <= 0`.
   * @throws VO_INVALID_ARGUMENT when `width <= 0`.
   * @throws VO_INVALID_ARGUMENT when `height <= 0`.
   */
  bool (VO_MSE_CALL *addBeam)(voEngine engine, voMusicDocument target,
                              voMusicPlacement placement, int32_t leftCount, int32_t rightCount, voMusicBeamSlope slope, float gap,
                              float x, float y, float width, float height);

  /**
   * Adds dots with respect to their position in this document.
   *
   * @param engine the engine.
   * @param target the target document.
   * @param count the number of dots to be added.
   * @param x the origin of the element bounding box along the x axis.
   * @param y the origin of the element bounding box along the y axis.
   * @param width the width of the element bounding box.
   * @param height the height of the element bounding box.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicDocument`.
   * @throws VO_INVALID_ARGUMENT When `count <= 0`
   * @throws VO_INVALID_ARGUMENT When `count > 2`
   * @throws VO_INVALID_ARGUMENT when `width <= 0`.
   * @throws VO_INVALID_ARGUMENT when `height <= 0`.
   */
  bool (VO_MSE_CALL *addDots)(voEngine engine, voMusicDocument target,
                              int32_t count,
                              float x, float y, float width, float height);

  /**
   * Adds a note tie or a slur with respect to its position in this document.
   *
   * @param engine the engine.
   * @param target the target document.
   * @param placement the placement of the tie or slur to be added.
   * @param x the origin of the element bounding box along the x axis.
   * @param y the origin of the element bounding box along the y axis.
   * @param width the width of the element bounding box.
   * @param height the height of the element bounding box.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicDocument`.
   * @throws VO_INVALID_ARGUMENT when `placement` is invalid.
   * @throws VO_INVALID_ARGUMENT when `width <= 0`.
   * @throws VO_INVALID_ARGUMENT when `height <= 0`.
   */
  bool (VO_MSE_CALL *addTieOrSlur)( voEngine engine, voMusicDocument target,
                                    voMusicPlacement placement,
                                    float x, float y, float width, float height);

  /**
   * Adds a ledger line with respect to its position in this document.
   *
   * @param engine the engine.
   * @param target the target document.
   * @param x the origin of the element bounding box along the x axis.
   * @param y the origin of the element bounding box along the y axis.
   * @param width the width of the element bounding box.
   * @param height the height of the element bounding box.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicDocument`.
   * @throws VO_INVALID_ARGUMENT when `width <= 0`.
   * @throws VO_INVALID_ARGUMENT when `height <= 0`.
   */
  bool (VO_MSE_CALL *addLedgerLine)(voEngine engine, voMusicDocument target,
                                    float x, float y, float width, float height);

  /**
   * Adds a rest with respect to its position in this document.
   *
   * @param engine the engine.
   * @param target the target document.
   * @param type the type of rest to be added.
   * @param x the origin of the element bounding box along the x axis.
   * @param y the origin of the element bounding box along the y axis.
   * @param width the width of the element bounding box.
   * @param height the height of the element bounding box.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicDocument`.
   * @throws VO_INVALID_ARGUMENT when `type` is invalid.
   * @throws VO_INVALID_ARGUMENT when `width <= 0`.
   * @throws VO_INVALID_ARGUMENT when `height <= 0`.
   */
  bool (VO_MSE_CALL *addRest)(voEngine engine, voMusicDocument target,
                              voMusicRestType type,
                              float x, float y, float width, float height);

  /**
   * Adds a character with respect to its position in this document.
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
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicDocument`.
   * @throws VO_NO_SUCH_OBJECT when the `character` reference is invalid.
   * @throws VO_INVALID_OBJECT when `character` is not a `voInputCharacter`.
   * @throws VO_INVALID_ARGUMENT when `character` contains no alternate.
   * @throws VO_INVALID_ARGUMENT when `width < 0`.
   * @throws VO_INVALID_ARGUMENT when `height < 0`.
   */
  bool (VO_MSE_CALL *addCharacter)( voEngine engine, voMusicDocument target,
                                    voInputCharacter character,
                                    float x, float y, float width, float height);

  /**
   * Deletes a component from this document.
   *
   * @param engine the engine.
   * @param target the target document.
   * @param index the index of the component to delete.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicDocument`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   */
  bool (VO_MSE_CALL *deleteComponentAt)(voEngine engine, voMusicDocument target, int32_t index);

  /**
   * Undeletes a component from this document.
   *
   * @param engine the engine.
   * @param target the target document.
   * @param index the index of the component to undelete.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicDocument`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   */
  bool (VO_MSE_CALL *undeleteComponentAt)(voEngine engine, voMusicDocument target, int32_t index);

  /**
   * Returns the interpretation of this document after it has been processed.
   *
   * @param engine the engine.
   * @param target the target document.
   *
   * @return the score on success, otherwise `NULL`.
   *   When `NULL`, use `voGetError()` to figure out whether an error occurred.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicDocument`.
   * @throws VO_INVALID_STATE when `target` has not been processed.
   * @throws VO_LIMIT_EXCEEDED when the number of `voMusicScore` instances would
   *   exceed the allowed limit (please remember that the limit can be shared by
   *   several types).
   */
  voMusicScore (VO_MSE_CALL *getScore)(voEngine engine, voMusicDocument target);

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
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicDocument`.
   *
   * @since 6.1_0
   */
  int32_t (VO_MSE_CALL *getScratchOutCount)(voEngine engine, voMusicDocument target);

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
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicDocument`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_LIMIT_EXCEEDED when the number of `voMusicScratchOut` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   *
   * @since 6.1_0
   */
  voMusicScratchOut (VO_MSE_CALL *getScratchOutAt)(voEngine engine, voMusicDocument target, int32_t index);

  /**
   * Adds an arpeggiate with respect to its position in this document.
   *
   * @param engine the engine.
   * @param target the target document.
   * @param type the type of arpeggiate to be added.
   * @param x the origin of the element bounding box along the x axis.
   * @param y the origin of the element bounding box along the y axis.
   * @param width the width of the element bounding box.
   * @param height the height of the element bounding box.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicDocument`.
   * @throws VO_INVALID_ARGUMENT when `type` is invalid.
   * @throws VO_INVALID_ARGUMENT when `width <= 0`.
   * @throws VO_INVALID_ARGUMENT when `height <= 0`.
   *
   * @since 6.3_0
   */
  bool (VO_MSE_CALL *addArpeggiate)(voEngine engine, voMusicDocument target,
                                    voMusicArpeggiateType type,
                                    float x, float y, float width, float height);

  /**
   * Adds a tuplet bracket with respect to its position in this document.
   *
   * @param engine the engine.
   * @param target the target document.
   * @param type the type of tuplet bracket to be added.
   * @param x the origin of the element bounding box along the x axis.
   * @param y the origin of the element bounding box along the y axis.
   * @param width the width of the element bounding box.
   * @param height the height of the element bounding box.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicDocument`.
   * @throws VO_INVALID_ARGUMENT when `type` is invalid.
   * @throws VO_INVALID_ARGUMENT when `width <= 0`.
   * @throws VO_INVALID_ARGUMENT when `height <= 0`.
   *
   * @since 7.0_0
   */
  bool (VO_MSE_CALL *addTupletBracket)( voEngine engine, voMusicDocument target,
                                        voMusicTupletBracketType type,
                                        float x, float y, float width, float height);
}
voIMusicDocument;

#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voMusicDocument` instance.
 *
 * @param engine the engine.
 *
 * @return the newly created `voMusicDocument` instance on success, otherwise
 *   `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_LIMIT_EXCEEDED when the number of `voMusicDocument` instances would
 *   exceed the allowed limit (please remember that the limit can be shared by
 *   several types).
 */
VO_INLINE voMusicDocument voCreateMusicDocument(voEngine engine)
{
  return voCreateObject(engine, VO_MusicDocument);
}
#endif // end of: #ifndef VO_NO_FUNCTIONS


// -- MusicAlphabetKnowledge ---------------------------------------------------

/**
 * `voMusicAlphabetKnowledge` reference type.
 *
 * {@extends voEngineObject}
 */
typedef voEngineObject voMusicAlphabetKnowledge;


// -- MusicGrammar -------------------------------------------------------------

/**
 * `voMusicGrammar` reference type.
 *
 * {@extends voEngineObject}
 */
typedef voEngineObject voMusicGrammar;


// -- MusicRecognizer ----------------------------------------------------------

/**
 * `voMusicRecognizer` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voIAttachTarget, voIMusicRecognizer}
 */
typedef voEngineObject voMusicRecognizer;

/**
 * {@propertySet MusicRecognizer}
 */
enum VO_MUSIC_RECOGNIZER_PROP
{
  /**
   * A value between `0.0f` and `1.0f` that adjusts the scratch out detection
   * sensitivity.
   *
   * {@property float}
   *
   * @note A value equal to `0.0f` disables scratch out detection.
   */
  VO_MUSIC_RECOGNIZER_SCRATCH_OUT_DETECTION_SENSITIVITY = (VO_MusicRecognizer << 16)
};

/**
 * Functions composing the `voIMusicRecognizer` interface.
 *
 * This interface exposes the public methods of the `voMusicRecognizer` type.
 *
 * {@implementingTypes voMusicRecognizer}
 */
typedef struct _voIMusicRecognizer
{
  /**
   * Performs the recognition of a music document.
   *
   * @param engine the engine.
   * @param recognizer the target recognizer.
   * @param document the music document to be recognized.
   * @param notifyProgressFunc an optional pointer to a `voNotifyProgressFunc`
   *   function.
   * @param userParam an optional pointer to a user defined parameter that will
   *   be passed back to `notifyProgressFunc`.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voMusicRecognizer`.
   * @throws VO_NO_SUCH_OBJECT when the `document` reference is invalid.
   * @throws VO_INVALID_OBJECT when `document` is not a `voMusicDocument`.
   * @throws VO_INVALID_CALLBACK when `notifyProgressFunc` is invalid.
   * @throws VO_INVALID_STATE when the `target` object is not in a state where
   *   the `process` function can be invoked.
   * @throws VO_MISSING_MUSIC_ALPHABET_KNOWLEDGE when `recognizer` has no
   *   attached `voMusicAlphabetKnowledge`.
   * @throws VO_MISSING_MUSIC_GRAMMAR when `recognizer` has no attached
   *   `voMusicGrammar`.
   * @throws VO_MISSING_MUSIC_STAFF when `document` has no staff defined.
   * @throws VO_CANCELED when the operation has been canceled.
   */
  bool (VO_MSE_CALL *process)(voEngine engine, voMusicRecognizer recognizer, voMusicDocument document, voNotifyProgressFunc notifyProgressFunc, void* userParam);
}
voIMusicRecognizer;

#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voMusicRecognizer` instance.
 *
 * @param engine the engine.
 *
 * @return the newly created `voMusicRecognizer` instance on success, otherwise
 *   `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_LIMIT_EXCEEDED when the number of `voMusicRecognizer` instances
 *   would exceed the allowed limit (please remember that the limit can be
 *   shared by several types).
 */
VO_INLINE voMusicRecognizer voCreateMusicRecognizer(voEngine engine)
{
  return voCreateObject(engine, VO_MusicRecognizer);
}
#endif // end of: #ifndef VO_NO_FUNCTIONS

#endif // end of: #ifndef C_MYSCRIPTMUSIC_H
