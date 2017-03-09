#ifndef C_MYSCRIPTTEXT_H
#define C_MYSCRIPTTEXT_H 0x08010000
/**
 * @file c/MyScriptText.h
 * Native interface to the MyScript HandWriting Recognition (Text) service.
 */

#include "MyScriptEngine.h"


// -- error codes --------------------------------------------------------------

/**
 * Lists the MyScript Text errors.
 */
enum VO_TEXT_ERR
{
  /**
   * Unable to complete the requested operation because a `voAlphabetKnowledge`
   * resource is missing.
   *
   * This does not necessarily mean that no `voAlphabetKnowledge` resource is
   * attached. For example, if you fill a `voStructuredInput` with
   * `VO_MULTI_LINE_TEXT` input units and you attach only a
   * `voIsolatedAlphabetKnowledge` you will get this error.
   *
   * @see voAlphabetKnowledge
   */
  VO_MISSING_ALPHABET_KNOWLEDGE = VO_INVALID_STATE | (2 << 16),

  /**
   * Unable to complete the requested operation because the input consumer
   * source is missing.
   *
   * @see voIInputConsumer
   */
  VO_MISSING_SOURCE,

  /**
   * One of the character input units within an input exceeds the stroke limit
   * fixed by the recognizer
   *
   * @note This limit may change from one recognizer configuration to another.
   */
  VO_CHAR_STROKE_LIMIT_EXCEEDED,

  /**
   * Parse error in a regular expression: sub-expression imbalance.
   *
   * This means that the number of open and close parentheses does not match.
   *
   * @see voLexicalUnit
   */
  VO_REGEX_SUBEXP_IMBALANCE = VO_INVALID_ARGUMENT | (5 << 16),

  /**
   * Parse error in a regular expression: unmatched open set.
   *
   * This means that an open square bracket exists without the matching close
   * square bracket.
   *
   * @see voLexicalUnit
   */
  VO_REGEX_UNMATCHED_OPEN_SET,

  /**
   * Parse error in a regular expression: unmatched open reference.
   *
   * This means that an open brace exists without the matching close brace.
   *
   * @see voLexicalUnit
   */
  VO_REGEX_UNMATCHED_OPEN_REF,

  /**
   * Parse error in a regular expression: trailing escape.
   *
   * This means that a back slash was found at the end of the expression
   * (without the escaped character).
   *
   * @see voLexicalUnit
   */
  VO_REGEX_TRAILING_ESCAPE,

  /**
   * Parse error in a regular expression: invalid character.
   *
   * This means that an invalid character was found.
   *
   * @see voLexicalUnit
   */
  VO_REGEX_INVALID_CHARACTER,

  /**
   * Parse error in a regular expression: invalid reference.
   *
   * This means that a reference was found but the corresponding definition does
   * not exist.
   *
   * @see voLexicalUnit
   */
  VO_REGEX_INVALID_REFERENCE,

  /**
   * Parse error in a regular expression: unmatched close set.
   *
   * This means that a close square bracket exists without the matching open
   * square bracket.
   *
   * @see voLexicalUnit
   * @since 4.2_2
   */
  VO_REGEX_UNMATCHED_CLOSE_SET,

  /**
   * Parse error in a regular expression: unmatched close reference.
   *
   * This means that a close brace exists without the matching open brace.
   *
   * @see voLexicalUnit
   * @since 4.2_2
   */
  VO_REGEX_UNMATCHED_CLOSE_REF,

  /**
   * Parse error in a regular expression: unescaped operator.
   *
   * This means that a regular expression operator was found when a normal
   * character was expected.
   *
   * Escape it using the backslash character if you meant it literally.
   *
   * @see voLexicalUnit
   * @since 4.2_2
   */
  VO_REGEX_UNESCAPED_OPERATOR,

  /**
   * An input with too many strokes or too many input units was sent to the
   * target `voHandwritingContext`.
   *
   * @see voHandwritingContext
   * @since 4.2_2
   */
  VO_INPUT_LIMIT_EXCEEDED
};


// -- types and interfaces -----------------------------------------------------

/**
 * Lists the MyScript Text types.
 */
enum VO_TEXT_T
{
  /**
   * `voHandwritingContext` type identifier.
   *
   * @see voHandwritingContext
   */
  VO_HandwritingContext = 300,

  /**
   * `voRecognizer` type identifier.
   *
   * @see voRecognizer
   */
  VO_Recognizer,

  /**
   * `voStructuredInputRecognizer` type identifier.
   *
   * @see voStructuredInputRecognizer
   */
  VO_StructuredInputRecognizer = 303,

  /**
   * `voTextRecognizer` type identifier.
   *
   * @see voTextRecognizer
   */
  VO_TextRecognizer,

  /**
   * `voDigitalTextMatcher` type identifier.
   *
   * @see voDigitalTextMatcher
   */
  VO_DigitalTextMatcher = VO_TextRecognizer + 2,

  /**
   * `voResource` type identifier.
   *
   * @see voResource
   */
  VO_Resource,

  /**
   * `voAlphabetKnowledge` type identifier.
   *
   * @see voAlphabetKnowledge
   */
  VO_AlphabetKnowledge,

  /**
   * `voCursiveAlphabetKnowledge` type identifier.
   *
   * @see voCursiveAlphabetKnowledge
   */
  VO_CursiveAlphabetKnowledge,

  /**
   * `voHandPrintedAlphabetKnowledge` type identifier.
   *
   * @see voHandPrintedAlphabetKnowledge
   */
  VO_HandPrintedAlphabetKnowledge,

  /**
   * `voIsolatedAlphabetKnowledge` type identifier.
   *
   * @see voIsolatedAlphabetKnowledge
   */
  VO_IsolatedAlphabetKnowledge,

  /**
   * `voLinguisticKnowledge` type identifier.
   *
   * @see voLinguisticKnowledge
   */
  VO_LinguisticKnowledge,

  /**
   * `voVocabulary` type identifier.
   *
   * @see voVocabulary
   */
  VO_Vocabulary,

  /**
   * `voLexicalUnit` type identifier.
   *
   * @see voLexicalUnit
   */
  VO_LexicalUnit,

  /**
   * `voLexicon` type identifier.
   *
   * @see voLexicon
   */
  VO_Lexicon,

  /**
   * `voGrammar` type identifier.
   *
   * @see voGrammar
   */
  VO_Grammar = 317,

  /**
   * `voSubsetKnowledge` type identifier.
   *
   * @see voSubsetKnowledge
   */
  VO_SubsetKnowledge,

  /**
   * `voArchive` type identifier.
   *
   * @see voArchive
   */
  VO_Archive,

  /**
   * `voWordIterator` type identifier.
   *
   * @see voWordIterator
   */
  VO_WordIterator,

  /**
   * `voRecognitionResult` type identifier.
   *
   * @see voRecognitionResult
   */
  VO_RecognitionResult,

  /**
   * `voCandidateIterator` type identifier.
   *
   * @see voCandidateIterator
   */
  VO_CandidateIterator,

  /**
   * `voSegmentIterator` type identifier.
   *
   * @see voSegmentIterator
   */
  VO_SegmentIterator,

  /**
   * `voDigitalInkTagger` type identifier.
   *
   * @see voDigitalInkTagger
   * @since 4.4_4
   */
  VO_DigitalInkTagger,

  /**
   * `voTagList` type identifier.
   *
   * @see voTagList
   * @since 4.4_4
   */
  VO_TagList,

  /**
   * `voTagIterator` type identifier.
   *
   * @see voTagIterator
   * @since 4.4_4
   */
  VO_TagIterator,

  /**
   * `voTextSegmenter` type identifier.
   *
   * @see voTextSegmenter
   * @since 7.4_0
   */
  VO_TextSegmenter,

  /**
   * `voLinguisticKnowledgeBuilder` type identifier.
   *
   * @see voLinguisticKnowledgeBuilder
   * @since 7.4_0
   */
  VO_LinguisticKnowledgeBuilder,
  
  /**
   * `voTextResultSegmentIterator` type identifier.
   *
   * @see voTextResultSegmentIterator
   * @since 8.0_0
   */
  VO_TextResultSegmentIterator = 734,
  
  /**
   * `voTextResultCandidateIterator` type identifier.
   *
   * @see voTextResultCandidateIterator
   * @since 8.0_0
   */
  VO_TextResultCandidateIterator = 735,
};


/**
 * Lists the MyScript Text interfaces.
 */
enum VO_TEXT_I
{
  /**
   * `voILexicalUnit` interface identifier.
   *
   * @see voILexicalUnit
   */
  VO_ILexicalUnit = 350,

  /**
   * `voILexicon` interface identifier.
   *
   * @see voILexicon
   */
  VO_ILexicon,

  /**
   * `voISubsetKnowledge` interface identifier.
   *
   * @see voISubsetKnowledge
   */
  VO_ISubsetKnowledge,

  /**
   * `voIWordIterator` interface identifier.
   *
   * @see voIWordIterator
   */
  VO_IWordIterator,

  /**
   * `voIRecognitionResult` interface identifier.
   *
   * @see voIRecognitionResult
   */
  VO_IRecognitionResult,

  /**
   * `voICandidateIterator` interface identifier.
   *
   * @see voICandidateIterator
   */
  VO_ICandidateIterator,

  /**
   * `voISegmentIterator` interface identifier.
   *
   * @see voISegmentIterator
   */
  VO_ISegmentIterator,

  /**
   * `voIDigitalTextMatcher` interface identifier.
   *
   * @see voIDigitalTextMatcher
   */
  VO_IDigitalTextMatcher = VO_ISegmentIterator + 2,

  /**
   * `voIAlphabet` interface identifier.
   *
   * @see voIAlphabet
   */
  VO_IAlphabet = 400,

  /**
   * `voIRecognitionResultProvider` interface identifier.
   *
   * @see voIRecognitionResultProvider
   */
  VO_IRecognitionResultProvider,

  /**
   * `voIDigitalInkTagger` interface identifier.
   *
   * @see voIDigitalInkTagger
   * @since 4.4_4
   */
  VO_IDigitalInkTagger = VO_IRecognitionResultProvider + 2,

  /**
   * `voITagList` interface identifier.
   *
   * @see voITagList
   * @since 4.4_4
   */
  VO_ITagList,

  /**
   * `voITagIterator` interface identifier.
   *
   * @see voITagIterator
   * @since 4.4_4
   */
  VO_ITagIterator,

  /**
   * `voITextSegmenter` interface type.
   *
   * @see voITextSegmenter
   * @since 7.4_0
   */
  VO_ITextSegmenter,

  /**
   * `voILinguisticKnowledgeBuilder` interface type.
   *
   * @see voILinguisticKnowledgeBuilder
   * @since 7.4_0
   */
  VO_ILinguisticKnowledgeBuilder,
  
  /**
   * `voITextResultSegmentIterator` type identifier.
   *
   * @see voITextResultSegmentIterator
   * @since 8.0_0
   */
  VO_ITextResultSegmentIterator,
  
  /**
   * `voITextResultCandidateIterator` type identifier.
   *
   * @see voITextResultCandidateIterator
   * @since 8.0_0
   */
  VO_ITextResultCandidateIterator,
};


// -- HanwritingContext --------------------------------------------------------

/**
 * `voHandwritingContext` reference type.
 *
 * {@extends voEngineObject}
 * {@subTypes voRecognizer}
 * {@implements voIAttachTarget, voIInputConsumer}
 * {@typeProperties VO_HANDWRITINGCONTEXT_TPROP}
 * {@properties VO_HANDWRITINGCONTEXT_PROP}
 * @see VO_HANDWRITINGCONTEXT_TPROP
 * @see voRecognizer
 */
typedef voEngineObject voHandwritingContext;

/**
 * {@typePropertySet HandwritingContext}
 */
enum VO_HANDWRITINGCONTEXT_TPROP
{
  /**
   * Represents the maximum number of `voHandwritingContext` instances that can
   * exist at the same time in the user side of a MyScript enabled application.
   *
   * The special value zero means no limit. Other values are adjusted to the
   * next multiple of 16. A value under the current count is permitted and will
   * constrain future creations.
   *
   * {@property uint32_t}
   */
  VO_USER_HWCTX_LIMIT = VO_USER_OBJ_LIMIT,

  /**
   * Gives the number of `voHandwritingContext` instances that currently exist
   * in the user side of a MyScript enabled application.
   *
   * {@property read-only uint32_t}
   */
  VO_USER_HWCTX_COUNT = VO_USER_OBJ_COUNT
};

/**
 * Lists the speed versus quality compromise special values.
 */
enum VO_SPEED_QUALITY_COMPROMISE_VALUE
{
  VO_MAXIMIZE_SPEED   = 0,  /**< Maximize speed against quality. */
  VO_MAXIMIZE_QUALITY = 255 /**< Maximize quality against speed. */
};

/**
 * {@propertySet HandwritingContext}
 */
enum VO_HANDWRITINGCONTEXT_PROP
{
  /**
   * Adjusts the compromise between speed and quality.
   *
   * {@property uint8_t}
   *
   * @note You can use `VO_MAXIMIZE_SPEED`, `VO_MAXIMIZE_QUALITY`, or any value
   *   between `0` and `255`. However, there are less than `256` levels and the
   *   value will be adjusted to the closest available value.
   */
  VO_SPEED_QUALITY_COMPROMISE = (VO_HandwritingContext << 16)
};


// -- Recognizer ---------------------------------------------------------------

/**
 * `voRecognizer` reference type.
 *
 * {@extends voHandwritingContext}
 * {@subTypes voStructuredInputRecognizer}
 * {@properties VO_RECOGNIZER_PROP}
 * @see voStructuredInputRecognizer,
 *   voDigitalTextMatcher.
 */
typedef voHandwritingContext voRecognizer;

/**
 * Lists the casing scheme values.
 *
 * @since 4.5_5
 */
typedef enum _voCasingScheme
{
  /**
   * The recognition process applies conservative casing rules: only the strict
   * content of a `voVocabulary` can be recognized, without allowing case
   * variations.
   */
  VO_STRICT_CASE,

  /**
   * The recognition process applies Vision Object's legacy casing rules:
   * <pre>
   * [Vocabulary entry] -> [What can be recognized]
   * hello              -> hello, Hello, HELLO
   * Hello              -> Hello, HELLO
   * HELLO              -> HELLO
   * </pre>
   */
  VO_LEGACY_CASE,

  /**
   * The recognition process applies relaxed casing rules: any combination of
   * lowercase or uppercase letters is allowed in a `voVocabulary` entry,
   * <em>"lIkE THiS"</em>.
   */
  VO_LAX_CASE,

  _voCasingScheme_FORCE_INT_SIZE = 0x7FFFFFFF
}
voCasingScheme;

/**
 * Lists the accentuation scheme values.
 *
 * @since 4.5_5
 */
typedef enum _voAccentuationScheme
{
  /**
   * The recognition process applies conservative accentuation rules: only the
   * strict content of a `voVocabulary` can be recognized, without allowing
   * accentuation variations.
   */
  VO_STRICT_ACCENTUATION,

  /**
   * The recognition process applies relaxed accentuation rules: any letter in a
   * `voVocabulary` entry can be recognized with or without an accent.
   *
   * However, a letter can be recognized in an accentuated form only if the
   * `voAlphabetKnowledge` being used contains it.
   */
  VO_LAX_ACCENTUATION,

  _voAccentuationScheme_FORCE_INT_SIZE = 0x7FFFFFFF
}
voAccentuationScheme;

/**
 * Lists the half width / full width character conversion scheme values.
 *
 * @since 7.3_5
 */
typedef enum _voHalfWidthFullWidthScheme
{
  /**
   * No specific rule is applied to convert characters into their half width
   * or full width forms. Forms defined within the LK resources are kept.
   */
  VO_DEFAULT_HALF_WIDTH_FULL_WIDTH,

  /**
   * Smart rules are applied to convert symbols into their half width or full
   * width forms depending on the context.
   */
  VO_AUTO_HALF_WIDTH_FULL_WIDTH,

  /**
   * Force conversion of characters into their half width forms if any.
   */
  VO_FORCE_HALF_WIDTH,

  /**
   * Force conversion of characters into their full width forms if any.
   */
  VO_FORCE_FULL_WIDTH,

  _voHalfWidthFullWidthScheme_FORCE_INT_SIZE = 0x7FFFFFFF
}
voHalfWidthFullWidthScheme;

/**
 * Lists the fuzzy pinyin flags that configure the different fuzzy pinyin
 * behaviors.
 *
 * @since 7.4_0
 */
typedef enum _voFuzzyPinyinFlag
{
  VO_FUZZY_PINYIN_NONE      = 0,        /**< None. */
  VO_FUZZY_PINYIN_C_CH      = 1 << 0,   /**< C is equivalent to CH. */
  VO_FUZZY_PINYIN_Z_ZH      = 1 << 1,   /**< Z is equivalent to ZH. */
  VO_FUZZY_PINYIN_S_SH      = 1 << 2,   /**< S is equivalent to SH. */
  VO_FUZZY_PINYIN_F_H       = 1 << 3,   /**< F is equivalent to H. */
  VO_FUZZY_PINYIN_K_G       = 1 << 4,   /**< K is equivalent to G. */
  VO_FUZZY_PINYIN_L_N       = 1 << 5,   /**< L is equivalent to N. */
  VO_FUZZY_PINYIN_L_R       = 1 << 6,   /**< L is equivalent to R. */
  VO_FUZZY_PINYIN_AN_ANG    = 1 << 7,   /**< AN is equivalent to ANG. */
  VO_FUZZY_PINYIN_EN_ENG    = 1 << 8,   /**< EN is equivalent to ENG. */
  VO_FUZZY_PINYIN_IN_ING    = 1 << 9,   /**< IN is equivalent to ING. */
  VO_FUZZY_PINYIN_IAN_IANG  = 1 << 10,  /**< IAN is equivalent to IANG. */
  VO_FUZZY_PINYIN_UAN_UANG  = 1 << 11   /**< UAN is equivalent to UANG. */
}
voFuzzyPinyinFlag;

/**
 * Lists the half width / full width character conversion scheme values.
 *
 * @since 8.0_0
 */
typedef enum _voSuperimposedScheme
{
  /**
   * Cursive superimposed.
   */
  VO_CURSIVE_SUPERIMPOSED,

  /**
   * Handprint superimposed.
   */
  VO_HANDPRINT_SUPERIMPOSED,

  _voSuperimposedScheme_FORCE_INT_SIZE = 0x7FFFFFFF
}
voSuperimposedScheme;

/**
 * {@propertySet Recognizer}
 */
enum VO_RECOGNIZER_PROP
{
  /**
   * Specifies the size of the candidate lists that will be provided at the
   * character level in the recognition result.
   *
   * This value must be between `1` and `20`.
   *
   * {@property uint32_t}
   */
  VO_CHAR_CANDIDATE_LIST_SIZE = (VO_Recognizer << 16) + 1,

  /**
   * Specifies the size of the candidate lists that will be provided at the word
   * level in the recognition result.
   *
   * This value must be between `1` and `20`.
   *
   * {@property uint32_t}
   */
  VO_WORD_CANDIDATE_LIST_SIZE,

  /**
   * Specifies the size of the candidate lists that will be provided at the text
   * level in the recognition result.
   *
   * This value must be between `1` and `20`.
   *
   * {@property uint32_t}
   */
  VO_TEXT_CANDIDATE_LIST_SIZE,

  /**
   * Tells the recognizer to detect the candidates that differ only in case and
   * return the best candidate of that lot.
   *
   * {@property bool}
   */
  VO_DISCARD_CASE_VARIATIONS,

  /**
   * Overrides the casing scheme specified by every `voVocabulary` resource
   * attached to a recognizer.
   *
   * {@property voCasingScheme}
   *
   * @since 4.5_5
   */
  VO_OVERRIDE_CASING_SCHEME = 19726342,

  /**
   * Overrides the accentuation scheme specified by every `voVocabulary`
   * resource attached to a recognizer.
   *
   * {@property voAccentuationScheme}
   *
   * @since 4.5_5
   */
  VO_OVERRIDE_ACCENTUATION_SCHEME,

  /**
   * Tells the recognizer to detect the candidates that differ only in
   * accentuation and return the best candidate of that lot.
   *
   * {@property bool}
   *
   * @since 4.5_5
   */
  VO_DISCARD_ACCENTUATION_VARIATIONS,

  /**
   * Specifies the amount of glyph distortion.
   *
   * `0` means no distortion. `255` means maximum supported distortion.
   *
   * {@property uint8_t}
   *
   * @since 4.5_5
   */
  VO_GLYPH_DISTORTION,

  /**
   * Specifies the amount of spelling distortion.
   *
   * `0` means no distortion. `255` means maximum supported distortion.
   *
   * {@property uint8_t}
   *
   * @since 4.5_5
   */
  VO_SPELLING_DISTORTION,

  /**
   * Enables out of lexicon matches.
   *
   * {@property bool}
   *
   * @since 4.5_5
   */
  VO_ENABLE_OUT_OF_LEXICON,

  /**
   * Enables recognition of rotated text.
   *
   * {@property bool}
   *
   * @since 6.4
   */
  VO_ENABLE_ROTATED_TEXT,

  /**
   * Enables transliteration.
   *
   * {@property bool}
   *
   * @since 7.0
   */
  VO_ENABLE_TRANSLITERATION,

  /**
   * Specifies the half width / full width character conversion scheme.
   *
   * {@property voHalfWidthFullWidthScheme}
   *
   * @since 7.3_5
   */
  VO_HALF_WIDTH_FULL_WIDTH_SCHEME,

  /**
   * Enables the support of alien characters.
   *
   * {@property bool}
   *
   * @since 7.3_5
   */
  VO_ENABLE_ALIEN_CHARACTERS,

  /**
  * Configures fuzzy pinyin.
  *
  * {@property VO_FLAGS_T(voFuzzyPinyinFlag)}
  *
  * @since 7.4_0
  */
  VO_FUZZY_PINYIN_FLAGS,

  /**
  * Disables superimposed handwriting recognition mode when using a
  * cursive-superimposed AlphabetKnowledge resource.
  *
  * {@property voSuperimposedScheme}
  *
  * @since 8.0_0
  */
  VO_SUPERIMPOSED_SCHEME
};


// -- StructuredInputRecognizer ------------------------------------------------

/**
 * `voStructuredInputRecognizer` reference type.
 *
 * {@extends voRecognizer}
 * {@implements voIRunnable, voIRecognitionResultProvider}
 * {@properties VO_RECOGNIZER_PROP}
 */
typedef voRecognizer voStructuredInputRecognizer;

#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voStructuredInputRecognizer` instance.
 *
 * @param engine the engine.
 *
 * @return the newly created `voStructuredInputRecognizer` instance on success,
 *   otherwise `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_LIMIT_EXCEEDED when the number of `voStructuredInputRecognizer`
 *   instances would exceed the allowed limit (please remember that the limit
 *   can be shared by several types).
 */
VO_INLINE voStructuredInputRecognizer voCreateStructuredInputRecognizer(voEngine engine)
{
  return voCreateObject(engine, VO_StructuredInputRecognizer);
}
#endif // end of: #ifndef VO_NO_FUNCTIONS


// -- TextRecognizer -----------------------------------------------------------

/**
 * `voTextRecognizer` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voIAttachTarget, voIContentFieldProcessor}
 */
typedef voEngineObject voTextRecognizer;

#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voTextRecognizer` instance.
 *
 * @param engine the engine.
 *
 * @return the newly created `voTextRecognizer` instance on success, otherwise
 *   `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_LIMIT_EXCEEDED when the number of `voTextRecognizer` instances
 *   would exceed the allowed limit (please remember that the limit can be
 *   shared by several types).
 */
VO_INLINE voTextRecognizer voCreateTextRecognizer(voEngine engine)
{
  return voCreateObject(engine, VO_TextRecognizer);
}
#endif // end of: #ifndef VO_NO_FUNCTIONS


// -- DigitalTextMatcher -------------------------------------------------------

/**
 * `voDigitalTextMatcher` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voIAttachTarget, voIDigitalTextMatcher}
 */
typedef voEngineObject voDigitalTextMatcher;

/**
 * Functions composing the `voIDigitalTextMatcher` interface.
 *
 * This interface exposes the public methods of the `voDigitalTextMatcher` type.
 *
 * {@implementingTypes voDigitalTextMatcher}
 */
typedef struct _voIDigitalTextMatcher
{
  /**
   * Answers the question: does the given string match the current resource
   * configuration?
   *
   * @param engine the engine.
   * @param target the target digital text matcher.
   * @param charset the charset used to encode parameter strings.
   * @param string the string to be tested.
   *
   * @return `VO_YES` or `VO_NO` on success, otherwise `VO_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voDigitalTextMatcher`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `string` or its `bytes` member is not
   *   readable.
   * @throws VO_INVALID_ARGUMENT when `string` is empty.
   */
  voYesNo (VO_MSE_CALL *matches)(voEngine engine, voDigitalTextMatcher target, voCharset charset, const voString* string);
}
voIDigitalTextMatcher;

#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voDigitalTextMatcher` instance.
 *
 * @param engine the engine.
 *
 * @return the newly created `voDigitalTextMatcher` instance on success,
 *   otherwise `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_LIMIT_EXCEEDED when the number of `voDigitalTextMatcher` instances
 *   would exceed the allowed limit (please remember that the limit can be
 *   shared by several types).
 */
VO_INLINE voDigitalTextMatcher voCreateDigitalTextMatcher(voEngine engine)
{
  return voCreateObject(engine, VO_DigitalTextMatcher);
}
#endif // end of: #ifndef VO_NO_FUNCTIONS


// -- Resource -----------------------------------------------------------------

/**
 * `voResource` reference type.
 *
 * The `voResource` type is an abstract base type that models knowledge about
 * the various language elements you are recognizing.
 *
 * {@extends voEngineObject}
 * {@subTypes voAlphabetKnowledge, voLinguisticKnowledge, voSubsetKnowledge, voArchive}
 * {@typeProperties VO_RESOURCE_TPROP}
 * {@properties VO_RESOURCE_PROP}
 * @see VO_RES_TPROP
 * @see voAlphabetKnowledge, voLinguisticKnowledge, voSubsetKnowledge,
 *   voArchive.
 */
typedef voEngineObject voResource;

/**
 * {@typePropertySet Resource}
 */
enum VO_RESOURCE_TPROP
{
  /**
   * Represents the maximum number of `voResource` instances that can exist at
   * the same time in the user side of a MyScript enabled application.
   *
   * The special value zero means no limit. Other values are adjusted to the next
   * multiple of 16. A value under the current count is permitted and will
   * constrain future resource creations.
   *
   * {@property uint32_t}
   */
  VO_USER_RES_LIMIT = VO_USER_OBJ_LIMIT,

  /**
   * Gives the number of `voResource` instances that currently exist in the user
   * side of a MyScript enabled application.
   *
   * {@property read-only uint32_t}
   */
  VO_USER_RES_COUNT = VO_USER_OBJ_COUNT
};

/**
 * {@propertySet Resource}
 */
enum VO_RESOURCE_PROP
{
  /**
   * Represents the name of the resource (encoded in US-ASCII).
   *
   * This property is writable by the resource creator and readable by any user.
   *
   * The value must be a sequence of at most 56 characters taken among the
   * following list:
   * `['a'...'z', 'A'...'Z', '0'...'9', '_', '+', '/', '-', '.']`.
   *
   * {@property voString}
   */
  VO_RESOURCE_NAME = (VO_Resource << 16),

  /**
   * Represents the content version of the resource.
   *
   * This property is writable by the resource creator and readable by any user.
   *
   * The format of this number is specified by the `VO_VERSION()` macro.
   *
   * {@property uint32_t}
   */
  VO_RESOURCE_VERSION
};


// -- AlphabetKnowledge --------------------------------------------------------

/**
 * `voAlphabetKnowledge` reference type.
 *
 * The `voAlphabetKnowledge` type models knowledge of the alphabet and the
 * segmentation characteristics of a specific language.
 *
 * {@extends voResource}
 * {@implements voIAlphabet}
 */
typedef voResource voAlphabetKnowledge;


// -- CursiveAlphabetKnowledge -------------------------------------------------

/**
 * `voCursiveAlphabetKnowledge` reference type.
 *
 * {@extends voAlphabetKnowledge}
 */
typedef voAlphabetKnowledge voCursiveAlphabetKnowledge;


// -- HandPrintedAlphabetKnowledge ---------------------------------------------

/**
 * `voHandPrintedAlphabetKnowledge` reference type.
 *
 * {@extends voAlphabetKnowledge}
 */
typedef voAlphabetKnowledge voHandPrintedAlphabetKnowledge;


// -- IsolatedAlphabetKnowledge ------------------------------------------------

/**
 * `voIsolatedAlphabetKnowledge` reference type.
 *
 * {@extends voAlphabetKnowledge}
 */
typedef voAlphabetKnowledge voIsolatedAlphabetKnowledge;


// -- LinguisticKnowledge ------------------------------------------------------

/**
 * `voLinguisticKnowledge` reference type.
 *
 * The `voLinguisticKnowledge` type is an abstract type that models knowledge
 * about the words, phrases or particular semantic units that may exist in a
 * specific language.
 *
 * {@extends voResource}
 * {@subTypes voVocabulary, voGrammar}
 */
typedef voResource voLinguisticKnowledge;


// -- Vocabulary ---------------------------------------------------------------

/**
 * `voVocabulary` reference type.
 *
 * The `voVocabulary` type is an abstract type that models vocabulary level
 * linguistic knowledge given to the recognizer.
 *
 * {@extends voLinguisticKnowledge}
 * {@subTypes voLexicalUnit, voLexicon}
 * {@properties VO_VOCABULARY_PROP}
 * @see voLinguisticKnowledge
 * @see voLexicalUnit, voLexicon
 * @see VO_VOCABULARY_PROP
 */
typedef voLinguisticKnowledge voVocabulary;

/**
 * {@propertySet Vocabulary}
 */
enum VO_VOCABULARY_PROP
{
  /**
   * Scales the frequency values of a `voVocabulary` resource so that their
   * associated probabilities are given more or less importance against other
   * probabilities in the recognition process.
   *
   * The default value is `1`.
   *
   * {@property float}
   */
  VO_FREQUENCY_SCALE = (VO_Vocabulary << 16),

  /**
   * Sets aside a certain percentage (expressed by a value between `0` and `1`)
   * of the paths that the recognizer explores for one particular resource.
   *
   * By doing this, the vocabulary resource can supply a recognition candidate
   * even if the scores from its paths are noticeably lower than the other
   * scores.
   *
   * For example you could use `VO_FREQUENCY_SCALE` in conjunction with
   * `VO_RESERVED_RECOGNITION_PATHS` to give precedence to a resource but
   * reserve some paths for another resource in order to provide a safety net in
   * case of a nil match with the first one.
   *
   * {@property float}
   */
  VO_RESERVED_RECOGNITION_PATHS,

  /**
   * Sets aside a maximum percentage (expressed as a value between `0` and `1`)
   * of the paths that the recognizer explores for one particular resource.
   *
   * By doing this, the vocabulary resource will not populate the whole
   * recognition path bandwidth. Using this property in conjunction with the
   * `VO_RESERVED_RECOGNITION_PATHS` property gives complete control over the
   * bandwidth for the vocabulary resource.
   *
   * {@property float}
   * @since 4.0_1
   */
  VO_RECOGNITION_PATH_LIMIT = VO_RESERVED_RECOGNITION_PATHS + 2,

  /**
   * Controls the way character case variations are being handled by the
   * recognition process.
   *
   * By default, `voLexicon` and `voLexicalUnit` resources use legacy casing
   * rules (@see VO_LEGACY_CASE):
   *
   * <pre>
   * [Vocabulary entry] -> [What can be recognized]
   * hello              -> hello, Hello, HELLO
   * Hello              -> Hello, HELLO
   * HELLO              -> HELLO
   * </pre>
   *
   * {@property voCasingScheme}
   * @since 4.5_5
   */
  VO_CASING_SCHEME,

  /**
   * Controls the way character accentuation variations are being handled by the
   * recognition process.
   *
   * By default, all `voVocabulary` resources use strict accentuation rules
   * (@see VO_STRICT_ACCENTUATION).
   *
   * {@property voAccentuationScheme}
   * @since 4.5_5
   */
  VO_ACCENTUATION_SCHEME
};


// -- LexicalUnit --------------------------------------------------------------

/**
 * `voLexicalUnit` reference type.
 *
 * {@extends voVocabulary}
 * {@implements voILexicalUnit, voICompilable}
 */
typedef voVocabulary voLexicalUnit;

/**
 * Functions composing the `voILexicalUnit` interface.
 *
 * This interface exposes the public methods of the `voLexicalUnit` type.
 *
 * {@implementingTypes voLexicalUnit}
 */
typedef struct _voILexicalUnit
{
  /**
   * Defines a named regular expression.
   *
   * @param engine the engine.
   * @param target the target lexical unit.
   * @param charset the charset used to encode parameter strings.
   * @param name the name of the regular expression (to be used when you want to
   *   reference it from another regular expression).
   * @param regex the regular expression.
   * @param frequency the frequency assigned to this regular expression.
   *   This value must be between `-1` (low frequency) and `1` (high frequency).
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voLexicalUnit`.
   * @throws VO_MODIFICATION_ACCESS_DENIED when `target` is in use by another
   *   object.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `name` or its `bytes` member is not
   *   readable.
   * @throws VO_INVALID_ARGUMENT when `name` is empty.
   * @throws VO_INVALID_ARGUMENT when `name` contains invalid characters.
   * @throws VO_INVALID_USER_BUFFER when `regex` or its `bytes` member is not
   *   readable.
   * @throws VO_INVALID_ARGUMENT when `regex` is empty.
   * @throws VO_REGEX_SUBEXP_IMBALANCE when `regex` contains a sub-expression
   *   imbalance.
   * @throws VO_REGEX_UNMATCHED_OPEN_SET when `regex` contains an unmatched open
   *   set.
   * @throws VO_REGEX_UNMATCHED_OPEN_REF when `regex` contains an unmatched open
   *   reference.
   * @throws VO_REGEX_TRAILING_ESCAPE when `regex` contains a trailing escape
   *   character.
   * @throws VO_REGEX_INVALID_CHARACTER when `regex` contains an invalid
   *   character.
   * @throws VO_REGEX_INVALID_REFERENCE when `regex` refers to an undefined
   *   entity.
   * @throws VO_REGEX_UNMATCHED_CLOSE_SET when `regex` contains an unmatched
   *   close set
   * @throws VO_REGEX_UNMATCHED_CLOSE_REF when `regex` contains an unmatched
   *   close reference.
   * @throws VO_REGEX_UNESCAPED_OPERATOR when `regex` contains an unescaped
   *   operator.
   * @throws VO_INVALID_ARGUMENT when `frequency` is not between `-1` and `1`.
   * @throws VO_INVALID_STATE when `target` is compiled.
   */
  bool (VO_MSE_CALL *defineRegex)(voEngine engine, voLexicalUnit target, voCharset charset, const voString* name, const voString* regex, float frequency);

  /**
   * Defines the main regular expression.
   *
   * @param engine the engine.
   * @param target the target lexical unit.
   * @param charset the charset used to encode parameter strings.
   * @param regex the regular expression.
   * @param frequency the frequency assigned to this regular expression.
   *   This value must be between `-1` (low frequency) and `1` (high frequency).
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voLexicalUnit`.
   * @throws VO_MODIFICATION_ACCESS_DENIED when `target` is in use by another
   *   object.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `regex` or its `bytes` member is not
   *   readable.
   * @throws VO_INVALID_ARGUMENT when `regex` is empty.
   * @throws VO_REGEX_INVALID_CHARACTER when `name` contains invalid characters.
   * @throws VO_REGEX_SUBEXP_IMBALANCE when `regex` contains a sub-expression
   *   imbalance.
   * @throws VO_REGEX_UNMATCHED_OPEN_SET when `regex` contains an unmatched open
   *   set.
   * @throws VO_REGEX_UNMATCHED_OPEN_REF when `regex` contains an unmatched open
   *   reference.
   * @throws VO_REGEX_TRAILING_ESCAPE when `regex` contains a trailing escape
   *   character.
   * @throws VO_REGEX_INVALID_CHARACTER when `regex` contains an invalid
   *   character.
   * @throws VO_REGEX_INVALID_REFERENCE when `regex` refers to an undefined
   *   entity.
   * @throws VO_REGEX_UNMATCHED_CLOSE_SET when `regex` contains an unmatched
   *   close set
   * @throws VO_REGEX_UNMATCHED_CLOSE_REF when `regex` contains an unmatched
   *   close reference.
   * @throws VO_REGEX_UNESCAPED_OPERATOR when `regex` contains an unescaped
   *   operator.
   * @throws VO_INVALID_ARGUMENT when `frequency` is not between `-1` and `1`.
   * @throws VO_INVALID_STATE when `target` is compiled.
   *
   * @note This is the regular expression that defines the lexical unit itself.
   *   This means that you will get a `VO_INVALID_STATE` error if you try to
   *   compile a `voLexicalUnit` with only named regular expressions defined.
   */
  bool (VO_MSE_CALL *defineMainRegex)(voEngine engine, voLexicalUnit target, voCharset charset, const voString* regex, float frequency);

  /**
   * Imports a vocabulary resource into this lexical unit.
   *
   * @param engine the engine.
   * @param target the target lexical unit.
   * @param charset the charset used to encode parameter strings.
   * @param name the name of the regular expression (to be used when you want to
   *   reference it from another regular expression).
   * @param vocabulary the vocabulary to be imported.
   * @param frequency the frequency assigned to this regular expression.
   *   This value must be between `-1` (low frequency) and `1` (high frequency).
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voLexicalUnit`.
   * @throws VO_MODIFICATION_ACCESS_DENIED when `target` is in use by another
   *   object.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `name` or its `bytes` member is not
   *   readable.
   * @throws VO_INVALID_ARGUMENT when `name` is empty.
   * @throws VO_INVALID_ARGUMENT when `name` contains invalid characters.
   * @throws VO_NO_SUCH_OBJECT when the `vocabulary` reference is invalid.
   * @throws VO_INVALID_OBJECT when `vocabulary` is not a `voVocabulary`.
   * @throws VO_INVALID_ARGUMENT when `frequency` is not between `-1` and `1`.
   * @throws VO_INVALID_STATE when `target` is compiled.
   */
  bool (VO_MSE_CALL *importVocabulary)(voEngine engine, voLexicalUnit target, voCharset charset, const voString* name, voVocabulary vocabulary, float frequency);
}
voILexicalUnit;

#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voLexicalUnit` instance.
 *
 * @param engine the engine.
 *
 * @return the newly created `voLexicalUnit` instance on success, otherwise
 *   `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_LIMIT_EXCEEDED when the number of `voLexicalUnit` instances would
 *   exceed the allowed limit (please remember that the limit can be shared by
 *   several types).
 */
VO_INLINE voLexicalUnit voCreateLexicalUnit(voEngine engine)
{
  return voCreateObject(engine, VO_LexicalUnit);
}
#endif // end of: #ifndef VO_NO_FUNCTIONS


// -- Lexicon ------------------------------------------------------------------

/**
 * `voLexicon` reference type.
 *
 * {@extends voVocabulary}
 * {@implements voILexicon, voICompilable}
 * {@properties VO_LEXICON_PROP}
 */
typedef voVocabulary voLexicon;

/**
 * {@propertySet Lexicon}
 */
enum VO_LEXICON_PROP
{
  /**
   * Returns the number of words in the lexicon.
   *
   * {@property read-only uint32_t}
   */
  VO_WORD_COUNT = (VO_Lexicon << 16),

  /**
   * Returns the maximum length of a word in the lexicon.
   *
   * {@property read-only uint32_t}
   */
  VO_MAX_WORD_LENGTH
};


/**
 * `voWordIterator` reference type.
 *
 * {@extends voIterator}
 * {@implements voIWordIterator}
 */
typedef voIterator voWordIterator;

/**
 * Functions composing the `voIWordIterator` interface.
 *
 * This interface exposes the public methods of the `voWordIterator` type.
 *
 * @note As the C language does not provide any mechanism that could be used to
 *   simulate inheritance, inherited members are simply copied from the base
 *   structure.
 *
 * {@extends voIIterator}
 * {@implementingTypes voWordIterator}
 */
typedef struct _voIWordIterator
{
  /** {@inheritDoc} */
  voYesNo (VO_MSE_CALL *isAtEnd)(voEngine engine, voIterator target);
  /** {@inheritDoc} */
  bool (VO_MSE_CALL *next)(voEngine engine, voIterator target);
  /** {@inheritDoc} */
  voYesNo (VO_MSE_CALL *equals)(voEngine engine, voIterator target, voIterator other);

  /**
   * Returns the current word.
   *
   * @param engine the engine.
   * @param target the target iterator.
   * @param charset the charset used to encode parameter strings.
   * @param word the recipient for the word.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voWordIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `word` or its `bytes` member is not
   *   writable.
   */
  bool (VO_MSE_CALL *get)(voEngine engine, voWordIterator target, voCharset charset, voString* word);
}
voIWordIterator;


/**
 * Functions composing the `voILexicon` interface.
 *
 * This interface exposes the public methods of the `voLexicon` type.
 *
 * {@implementingTypes voLexicon}
 */
typedef struct _voILexicon
{
  /**
   * Adds a word to this lexicon.
   *
   * @param engine the engine.
   * @param target the target lexicon.
   * @param charset the charset used to encode parameter strings.
   * @param word the word to be added.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voLexicon`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `word` or its `bytes` member is not
   *   readable.
   * @throws VO_INVALID_ARGUMENT when `word` is the empty string or only contain
   *   space characters.
   * @throws VO_INVALID_STATE when `target` is compiled.
   */
  bool (VO_MSE_CALL *addWord)(voEngine engine, voLexicon target, voCharset charset, const voString* word);

  /**
   * Returns an iterator over the words of this lexicon.
   *
   * @param engine the engine.
   * @param target the target lexicon.
   *
   * @return a word iterator on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voLexicon`.
   * @throws VO_LIMIT_EXCEEDED when the number of `voWordIterator` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   * @throws VO_INVALID_OPERATION when word retrieval has not been enabled when
   *   `target` was compiled.
   */
  voWordIterator (VO_MSE_CALL *getWords)(voEngine engine, voLexicon target);

  /**
   * Removes a word from this lexicon.
   *
   * @note This function succeeds when the word is not present in the lexicon.
   *
   * @param engine the engine.
   * @param target the target lexicon.
   * @param charset the charset used to encode parameter strings.
   * @param word the word to be removed.
   * @param removed a optional pointer to a boolean set to `true` when the word
   *   was part of this lexicon.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voLexicon`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `word` or its `bytes` member is not
   *   readable.
   * @throws VO_INVALID_STATE when `target` is compiled.
   *
   * @since 7.4_0
   */
  bool (VO_MSE_CALL *removeWord)(voEngine engine, voLexicon target, voCharset charset, const voString* word, bool* removed);
}
voILexicon;

#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voLexicon` instance.
 *
 * @param engine the engine.
 *
 * @return the newly created `voLexicon` instance on success, otherwise `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_LIMIT_EXCEEDED when the number of `voLexicon` instances would
 *   exceed the allowed limit (please remember that the limit can be shared by
 *   several types).
 */
VO_INLINE voLexicon voCreateLexicon(voEngine engine)
{
  return voCreateObject(engine, VO_Lexicon);
}
#endif // end of: #ifndef VO_NO_FUNCTIONS

/**
 * Holds the parameters required to create an instance of the `voLexicon` type.
 *
 * @see voLexicon, voCreateLexiconEx
 */
typedef struct _voLexiconInitializer
{
  /**
   * Enables later retrieval of the words contained in the lexicon.
   */
  bool enableWordRetrieval;
}
voLexiconInitializer;

#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voLexicon` instance.
 *
 * @param engine the engine.
 * @param enableWordRetrieval when `true`, enables later retrieval of the words
 *   contained in the lexicon.
 *
 * @return the newly created `voLexicon` instance on success, otherwise
 *   `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_NO_SUCH_OBJECT when the `page` reference is invalid.
 * @throws VO_LIMIT_EXCEEDED when the number of `voLexicon` instances would
 *   exceed the allowed limit (please remember that the limit can be shared by
 *   several types).
 */
VO_INLINE voLexicon voCreateLexiconEx(voEngine engine, bool enableWordRetrieval)
{
  voLexiconInitializer initializer = { enableWordRetrieval };
  return voCreateObjectEx(engine, VO_Lexicon, &initializer, sizeof(initializer));
}
#endif // end of: #ifndef VO_NO_FUNCTIONS


// -- Grammar ------------------------------------------------------------------

/**
 * `voGrammar` reference type.
 *
 * Grammars enable recognition of multiple words in the same field; a grammar
 * must be used to recognize several strings in a row.
 *
 * {@extends voLinguisticKnowledge}
 * @see voLinguisticKnowledge
 */
typedef voLinguisticKnowledge voGrammar;


// -- SubsetKnowledge ----------------------------------------------------------

/**
 * `voSubsetKnowledge` reference type.
 *
 * {@extends voResource}
 * {@implements voISubsetKnowledge}
 */
typedef voResource voSubsetKnowledge;

/**
 * Functions composing the `voISubsetKnowledge` interface.
 *
 * This interface exposes the public methods of the `voSubsetKnowledge` type.
 *
 * {@implementingTypes voSubsetKnowledge}
 */
typedef struct _voISubsetKnowledge
{
  /**
   * Disables the recognition of all characters.
   *
   * @param engine the engine.
   * @param target the target subset knowledge.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voSubsetKnowledge`.
   */
  bool (VO_MSE_CALL *disableAllCharacters)(voEngine engine, voSubsetKnowledge target);

  /**
   * Enables the recognition of all characters.
   *
   * @param engine the engine.
   * @param target the target subset knowledge.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voSubsetKnowledge`.
   */
  bool (VO_MSE_CALL *enableAllCharacters)(voEngine engine, voSubsetKnowledge target);

  /**
   * Disables the specified characters (plural).
   *
   * @param engine the engine.
   * @param target the target subset knowledge.
   * @param charset the charset used to encode parameter strings.
   * @param characters the characters to be disabled.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voSubsetKnowledge`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `characters` or its `bytes` member is
   *   not readable.
   * @throws VO_INVALID_ARGUMENT when `characters` is empty.
   */
  bool (VO_MSE_CALL *disableCharacters)(voEngine engine, voSubsetKnowledge target, voCharset charset, const voString* characters);

  /**
   * Enables the specified characters (plural).
   *
   * @param engine the engine.
   * @param target the target subset knowledge.
   * @param charset the charset used to encode parameter strings.
   * @param characters the characters to be enabled.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voSubsetKnowledge`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `characters` or its `bytes` member is
   *   not readable.
   * @throws VO_INVALID_ARGUMENT when `characters` is empty.
   */
  bool (VO_MSE_CALL *enableCharacters)(voEngine engine, voSubsetKnowledge target, voCharset charset, const voString* characters);

  /**
   * Disables the specified character (singular).
   *
   * @param engine the engine.
   * @param target the target subset knowledge.
   * @param charset the charset used to encode parameter strings.
   * @param character the character to be disabled.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voSubsetKnowledge`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `character` or its `bytes` member is
   *   not readable.
   * @throws VO_INVALID_ARGUMENT when `character` is empty.
   *
   * @since 4.5_6
   */
  bool (VO_MSE_CALL *disableCharacter)(voEngine engine, voSubsetKnowledge target, voCharset charset, const voString* character);

  /**
   * Enables the specified character (singular).
   *
   * @param engine the engine.
   * @param target the target subset knowledge.
   * @param charset the charset used to encode parameter strings.
   * @param character the character to be enabled.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voSubsetKnowledge`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `character` or its `bytes` member is
   *   not readable.
   * @throws VO_INVALID_ARGUMENT when `character` is empty.
   *
   * @since 4.5_6
   */
  bool (VO_MSE_CALL *enableCharacter)(voEngine engine, voSubsetKnowledge target, voCharset charset, const voString* character);
}
voISubsetKnowledge;

#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voSubsetKnowledge` instance.
 *
 * @param engine the engine.
 *
 * @return the newly created `voSubsetKnowledge` instance on success, otherwise
 *   `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_LIMIT_EXCEEDED when the number of `voSubsetKnowledge` instances
 *   would exceed the allowed limit (please remember that the limit can be
 *   shared by several types).
 */
VO_INLINE voSubsetKnowledge voCreateSubsetKnowledge(voEngine engine)
{
  return voCreateObject(engine, VO_SubsetKnowledge);
}
#endif // end of: #ifndef VO_NO_FUNCTIONS


// -- Archive ------------------------------------------------------------------

/**
 * `voArchive` reference type.
 *
 * {@extends voResource}
 * {@implements voIAttachTarget}
 */
typedef voResource voArchive;

#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voArchive` instance.
 *
 * @param engine the engine.
 *
 * @return the newly created `voArchive` instance on success, otherwise `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_LIMIT_EXCEEDED when the number of `voArchive` instances would
 *   exceed the allowed limit (please remember that the limit can be shared by
 *   several types).
 */
VO_INLINE voArchive voCreateArchive(voEngine engine)
{
  return voCreateObject(engine, VO_Archive);
}
#endif // end of: #ifndef VO_NO_FUNCTIONS


// -- RecognitionResult --------------------------------------------------------

/**
 * `voRecognitionResult` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voIRecognitionResult}
 */
typedef voEngineObject voRecognitionResult;

/**
 * `voCandidateIterator` reference type.
 *
 * {@extends voIterator}
 * {@implements voICandidateIterator}
 */
typedef voIterator voCandidateIterator;

/**
 * `voSegmentIterator` reference type.
 *
 * {@extends voIterator}
 * {@implements voISegmentIterator}
 */
typedef voIterator voSegmentIterator;


/**
 * Functions composing the `voIRecognitionResult` interface.
 *
 * This interface exposes the public methods of the `voRecognitionResult` type.
 *
 * {@implementingTypes voRecognitionResult}
 */
typedef struct _voIRecognitionResult
{
  /**
   * Returns the observation score of the best candidate.
   *
   * @param engine the engine.
   * @param target the target recognition result.
   *
   * @return the observation score of the best candidate on success,
   *   otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voRecognitionResult`.
   * @throws VO_INVALID_STATE when there are no recognition candidates
   *   available.
   *
   * @since 8.1_0
   */
  float (VO_MSE_CALL *getObservationScore)(voEngine engine, voRecognitionResult target);

  /**
   * Returns an iterator over the different candidates.
   *
   * @param engine the engine.
   * @param target the target recognition result.
   *
   * @return an iterator over the different candidates on success, otherwise
   *   `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voRecognitionResult`.
   * @throws VO_LIMIT_EXCEEDED when the number of `voCandidateIterator`
   *   instances would exceed the allowed limit (please remember that the limit
   *   can be shared by several types).
   */
  voCandidateIterator (VO_MSE_CALL *getCandidates)(voEngine engine, voRecognitionResult target);
}
voIRecognitionResult;


/**
 * Lists the recognition result candidate flags.
 *
 * @since 4.5_5
 */
typedef enum _voCandidateFlag
{
  /**
   * The candidate contains at least one omitted character.
   *
   * An omitted character is a character that was omitted by the user and has
   * been added by the spelling distortion match. Consequently, the input range
   * of the corresponding character level segment will be empty.
   */
  VO_CANDIDATE_FLAG_OMITTED = 1 << 0,

  /**
   * The candidate contains at least one superfluous character.
   *
   * A superfluous character is a character that was superfluously added by the
   * user and has been discarded by the spelling distortion match. Consequently,
   * the label of the corresponding character level candidate will be empty.
   */
  VO_CANDIDATE_FLAG_SUPERFLUOUS = 1 << 1,

  /**
   * The candidate contains at least one substituted character.
   *
   * A substituted character is a character which label has been substituted by
   * the spelling distortion match.
   */
  VO_CANDIDATE_FLAG_SUBSTITUTED = 1 << 2,

  /**
   * The candidate contains at least one unknown character.
   *
   * An unknown character is a character that was considered unreadable by the
   * recognizer. Note that this character will not match normal vocabulary
   * entries which will be forced to transform it to a substituted character.
   * Consequently, only out of lexicon matches or unconstrained recognition may
   * output this.
   */
  VO_CANDIDATE_FLAG_UNKNOWN = 1 << 3,

  /**
   * The candidate contains at least one erased character.
   *
   * An erased character is a character that has been detected has erased by an
   * attached digital ink tagger (using either a strike-through or a double
   * strike-through or a cross out or a scratch out gesture).
   */
  VO_CANDIDATE_FLAG_ERASED = 1 << 4,

  /**
   * The candidate contains at least one emphasized character.
   *
   * An emphasized character is a character that has been detected has
   * emphasized by an attached digital ink tagger. Consequently, the resemblance
   * score of this character may not be accurate.
   */
  VO_CANDIDATE_FLAG_EMPHASIZED = 1 << 5,

  /**
   * The candidate was produced by an out of lexicon match.
   */
  VO_CANDIDATE_FLAG_OUT_OF_LEXICON = 1 << 6,

  /**
   * The candidate corresponds to a partial match.
   *
   * @since 4.9_8
   */
  VO_CANDIDATE_FLAG_PARTIAL = 1 << 7
}
voCandidateFlag;

/**
 * Functions composing the `voICandidateIterator` interface.
 *
 * This interface exposes the public methods of the `voCandidateIterator` type.
 *
 * @note As the C language does not provide any mechanism that could be used to
 *   simulate inheritance, inherited members are simply copied from the base
 *   structure.
 *
 * {@extends voIIterator}
 * {@implementingTypes voCandidateIterator}
 */
typedef struct _voICandidateIterator
{
  /** {@inheritDoc} */
  voYesNo (VO_MSE_CALL *isAtEnd)(voEngine engine, voIterator target);
  /** {@inheritDoc} */
  bool (VO_MSE_CALL *next)(voEngine engine, voIterator target);
  /** {@inheritDoc} */
  voYesNo (VO_MSE_CALL *equals)(voEngine engine, voIterator target, voIterator other);

  /**
   * Returns the label of this candidate.
   *
   * @param engine the engine.
   * @param target the target iterator.
   * @param charset the charset used to encode parameter strings.
   * @param label the recipient for the label.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voCandidateIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `label` or its `bytes` member is not
   *   writable.
   */
  bool (VO_MSE_CALL *getLabel)(voEngine engine, voCandidateIterator target, voCharset charset, voString* label);

  /**
   * Returns the normalized recognition score of the current candidate.
   *
   * @param engine the engine.
   * @param target the target iterator.
   *
   * @return the normalized recognition score of this candidate on success,
   *   otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voCandidateIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   */
  float (VO_MSE_CALL *getNormalizedRecognitionScore)(voEngine engine, voCandidateIterator target);

  /**
   * Returns an iterator over the segments of the current candidate.
   *
   * @note A segment iterator iterates over the pieces of input that define the
   *   candidate's segmentation.
   *
   * @param engine the engine.
   * @param target the target iterator.
   *
   * @return a segment iterator on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voCandidateIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   * @throws VO_LIMIT_EXCEEDED when the number of `voSegmentIterator` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   */
  voSegmentIterator (VO_MSE_CALL *getSegments)(voEngine engine, voCandidateIterator target);

  /**
   * Returns the observation score of the current candidate.
   *
   * @param engine the engine.
   * @param target the target iterator.
   *
   * @return the observation score of the current candidate on success,
   *   otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voCandidateIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   *
   * @since 8.1_0
   */
  float (VO_MSE_CALL *getObservationScore)(voEngine engine, voCandidateIterator target);

  /**
   * Returns the flags of the current candidate.
   *
   * @param engine the engine.
   * @param target the target iterator.
   *
   * @return the combination of flags on success, otherwise `VO_INVALID_FLAGS`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voCandidateIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   *
   * @since 4.5_5
   */
  VO_FLAGS_T(voCandidateFlag) (VO_MSE_CALL *getFlags)(voEngine engine, voCandidateIterator target);

  /**
   * Returns the spelling distortion ratio of the current candidate.
   *
   * This is the number of characters that are either omitted or superfluous
   * or substituted divided by the total number of characters.
   *
   * @param engine the engine.
   * @param target the target iterator.
   *
   * @return the spelling distortion ration on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voCandidateIterator`.
   * @throws VO_INVALID_STATE when there is no transliteration available.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   *
   * @since 4.5_5
   */
  float (VO_MSE_CALL *getSpellingDistortionRatio)(voEngine engine, voCandidateIterator target);

  /**
   * Returns the linguistic knowledge used to generate the current word-level
   * candidate (can be `voVocabulary` or `voGrammar`).
   *
   * @param engine the engine.
   * @param target the target iterator.
   *
   * @return the linguistic knowledge, if any.
   *   When `NULL`, use `voGetError()` to figure out whether an error occurred.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voCandidateIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   * @throws VO_INVALID_OPERATION when `target` is not a word-level iterator.
   * @throws VO_INVALID_STATE when `target` comes from a detached
   *   `voRecognitionResult` in which case the information is not available
   *   anymore.
   * @throws VO_INVALID_STATE when there was no attached linguistic knowledge at
   *   recognition time.
   *
   * @since 4.9_8
   */
  voLinguisticKnowledge (VO_MSE_CALL *getSourceLinguisticKnowledge)(voEngine engine, voCandidateIterator target);

  /**
   * Returns the name of the linguistic knowledge used to generate the current
   * word-level candidate.
   *
   * @note In case the linguistic knowledge name is not available, an empty
   *   string is returned.
   *
   * @param engine the engine.
   * @param target the target iterator.
   * @param charset the charset used to encode parameter strings.
   * @param name the recipient for the linguistic knowledge name.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voCandidateIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   * @throws VO_INVALID_OPERATION when `target` is not a word-level iterator.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `name` or its `bytes` member is not
   *   writable.
   * @throws VO_INVALID_STATE when there was no attached linguistic knowledge at
   *   recognition time.
   *
   * @since 6.0
   */
  bool (VO_MSE_CALL *getSourceLinguisticKnowledgeName)(voEngine engine, voCandidateIterator target, voCharset charset, voString* name);

  /**
   * Returns the index of the best transliteration candidate from the current
   * context in this candidate's top list (list of transliteration candidates).
   *
   * @param engine the engine.
   * @param target the target iterator.
   *
   * @return the index of the best transliteration candidate if any, otherwise
   *   `-1`.
   *   When `-1`, use `voGetError()` to figure out whether an error occurred.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voCandidateIterator`.
   * @throws VO_INVALID_STATE when there is no transliteration available.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   *
   * @since 7.0
   */
  int32_t (VO_MSE_CALL *getSelectedTransliterationCandidateIndex)(voEngine engine, voCandidateIterator target);

  /**
   * Returns an iterator over the transliteration candidates of the current
   * candidate.
   *
   * @param engine the engine.
   * @param target the target iterator.
   *
   * @return an iterator over the transliteration candidates of the current
   *   candidate on success, otherwise `NULL`.
   *   When `NULL`, use `voGetError()` to figure out whether an error occurred.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voCandidateIterator`.
   * @throws VO_INVALID_STATE when there is no transliteration available.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   * @throws VO_LIMIT_EXCEEDED when the number of `voCandidateIterator`
   *   instances would exceed the allowed limit (please remember that the limit
   *   can be shared by several types).
   *
   * @since 7.0
   */
  voCandidateIterator (VO_MSE_CALL *getTransliterationCandidates)(voEngine engine, voCandidateIterator target);
}
voICandidateIterator;


/**
 * Represents the location of an item in the input.
 *
 * An input item is either a point within a stroke component, or a character
 * within a string component.
 *
 * Then each component is located within an input unit.
 */
typedef struct _voInputItemLocator
{
  /**
   * Index of the input unit that contains the item.
   */
  short unitIndex;

  /**
   * Index of the stroke component or the string component that contains the
   * item, within its unit.
   */
  short componentIndex;

  /**
   * Index of the point or character item, within its component.
   */
  short itemIndex;
}
voInputItemLocator;

/**
 * Represents a contiguous range in the input delimited by its first and last
 * enclosed items.
 */
typedef struct _voInputRangeElement
{
  /**
   * Location of the first item.
   */
  voInputItemLocator first;

  /**
   * Location of the last item.
   */
  voInputItemLocator last;
}
voInputRangeElement;

/**
 * Represents a discrete range in the input, denoted by the list of the
 * contiguous range elements that compose it.
 */
typedef struct _voInputRange
{
  /**
   * Pointer to the contiguous range elements.
   */
  voInputRangeElement* elements;

  /**
   * Number of contiguous range elements.
   */
  size_t count;
}
voInputRange;

/**
 * Functions composing the `voISegmentIterator` interface.
 *
 * This interface exposes the public methods of the `voSegmentIterator` type.
 *
 * @note As the C language does not provide any mechanism that could be used to
 *   simulate inheritance, inherited members are simply copied from the base
 *   structure.
 *
 * {@extends voIIterator}
 * {@implementingTypes voSegmentIterator}
 */
typedef struct _voISegmentIterator
{
  /** {@inheritDoc} */
  voYesNo (VO_MSE_CALL *isAtEnd)(voEngine engine, voIterator target);
  /** {@inheritDoc} */
  bool (VO_MSE_CALL *next)(voEngine engine, voIterator target);
  /** {@inheritDoc} */
  voYesNo (VO_MSE_CALL *equals)(voEngine engine, voIterator target, voIterator other);

  /**
   * Returns the range in the input that corresponds to this segment.
   *
   * @param engine the engine.
   * @param target the target iterator.
   * @param range the recipient for the input range.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voSegmentIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   * @throws VO_INVALID_USER_BUFFER when `range`, or its `elements` member is
   *   not writable.
   * @throws VO_INVALID_STATE when the portion of a MyScript Document associated
   *   with this segment has been modified, invalidating input ranges.
   */
  bool (VO_MSE_CALL *getInputRange)(voEngine engine, voSegmentIterator target, voInputRange* range);

  /**
   * Returns the index of the best candidate from the current context in this
   * segment's top list (list of candidates).
   *
   * @param engine the engine.
   * @param target the target iterator.
   *
   * @return the index of the best candidate on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voSegmentIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   */
  int32_t (VO_MSE_CALL *getSelectedCandidateIndex)(voEngine engine, voSegmentIterator target);

  /**
   * Returns the observation score of the selected candidate for this segment.
   *
   * @param engine the engine.
   * @param target the target iterator.
   *
   * @return the observation score of the selected candidate on success,
   *   otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voSegmentIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   *
   * @since 8.1_0
   */
  float (VO_MSE_CALL *getObservationScore)(voEngine engine, voSegmentIterator target);

  /**
   * Returns an iterator over the candidates of this segment.
   *
   * @param engine the engine.
   * @param target the target iterator.
   *
   * @return an iterator over the candidates of this segment.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voSegmentIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   * @throws VO_LIMIT_EXCEEDED when the number of `voCandidateIterator`
   *   instances would exceed the allowed limit (please remember that the limit
   *   can be shared by several types).
   */
  voCandidateIterator (VO_MSE_CALL *getCandidates)(voEngine engine, voSegmentIterator target);
}
voISegmentIterator;


// -- IAlphabet ----------------------------------------------------------------

/**
 * Functions composing the `voIAlphabet` interface.
 *
 * This interface exposes the public methods of the `voAlphabetKnowledge` type.
 *
 * @note The term "alphabet" here denotes a finite set of symbols.

 * {@implementingTypes voAlphabetKnowledge}
 */
typedef struct _voIAlphabet
{
  /**
   * Returns the number of symbols in this alphabet.
   *
   * @param engine the engine.
   * @param target the target alphabet.
   *
   * @return the number of symbols in this alphabet on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   */
  int32_t (VO_MSE_CALL *getSymbolCount)(voEngine engine, voEngineObject target);

  /**
   * Returns the symbol at the specified index.
   *
   * @param engine the engine.
   * @param target the target alphabet.
   * @param index the index of the symbol to be retrieved.
   * @param charset the charset used to encode parameter strings.
   * @param symbol the recipient for the symbol.
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
   * @throws VO_INVALID_USER_BUFFER when `symbol` or its `bytes` member is not
   *   writable.
   */
  bool (VO_MSE_CALL *getSymbolAt)(voEngine engine, voEngineObject target, int32_t index, voCharset charset, voString* symbol);
}
voIAlphabet;


// -- IRecognitionResultProvider -----------------------------------------------

/**
 * Functions composing the `voIRecognitionResultProvider` interface.
 *
 * {@implementingTypes voStructuredInputRecognizer}
 */
typedef struct _voIRecognitionResultProvider
{
  /**
   * Returns the recognition result associated with the target provider.
   *
   * @param engine the engine.
   * @param target the target provider.
   *
   * @return the recognition result on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_INVALID_STATE when `target` is not in a state where a
   *   recognition result is available.
   * @throws VO_LIMIT_EXCEEDED when the number of `voRecognitionResult`
   *   instances would exceed the allowed limit (please remember that the limit
   *   can be shared by several types).
   */
  voRecognitionResult (VO_MSE_CALL *getResult)(voEngine engine, voEngineObject target);

  /**
   * Returns the recognition result associated with the target object.
   *
   * @param engine the engine.
   * @param target the target provider.
   * @param enablePartialMatches when `true`,  partial matches of the last word
   *   segment will be allowed.
   *
   * @return the recognition result on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_INVALID_STATE when `target` is not in a state where a
   *   recognition result is available.
   * @throws VO_LIMIT_EXCEEDED when the number of `voRecognitionResult`
   *   instances would exceed the allowed limit (please remember that the limit
   *   can be shared by several types).
   *
   * @since 4.9_8
   */
  voRecognitionResult (VO_MSE_CALL *getResultEx)(voEngine engine, voEngineObject target, bool enablePartialMatches);
}
voIRecognitionResultProvider;


// -- Digital Ink Tagger -------------------------------------------------------

/**
 * Lists the digital ink tag types.
 *
 * @par deprecated as of 7.4.0
 */
typedef enum _voTagType
{
  /**
   * Error marker.
   */
  VO_TAG_ERR = -1,

  /**
   * Type of a text line tag.
   */
  VO_TAG_TEXT_LINE,

  /**
   * Type of a text block tag.
   */
  VO_TAG_TEXT_BLOCK,

  /**
   * Type of a correction block tag.
   */
  VO_TAG_CORRECTION_BLOCK,

  /**
   * Type of an erasure tag.
   */
  VO_TAG_ERASURE,

  /**
   * Type of an erased text tag.
   */
  VO_TAG_ERASED_TEXT,

  /**
   * Type of a strike-through tag.
   */
  VO_TAG_STRIKETHROUGH,

  /**
   * Type of a double strike-through tag.
   */
  VO_TAG_DOUBLE_STRIKETHROUGH,

  /**
   * Type of a cross out tag.
   */
  VO_TAG_CROSS_OUT,

  /**
   * Type of a scratch out tag.
   */
  VO_TAG_SCRATCH_OUT,

  /**
   * Type of an inserted text tag.
   */
  VO_TAG_INSERTED_TEXT,

  /**
   * Type of a missing pen lift tag.
   */
  VO_TAG_MISSING_PEN_LIFT,

  /**
   * Type of an emphasized character tag.
   */
  VO_TAG_EMPHASIZED_CHARACTER,

  /**
   * Type of a text column tag.
   *
   * @since 4.7_7
   */
  VO_TAG_TEXT_COLUMN,

  /**
   * The number of tags.
   */
  VO_TAG_COUNT = 32,

  _voTagType_FORCE_INT_SIZE = 0x7FFFFFFF
}
voTagType;


/**
 * `voTagIterator` reference type.
 *
 * {@extends voIterator}
 * {@implements voITagIterator}
 */
typedef voIterator voTagIterator;

/**
 * Functions composing the `voITagIterator` interface.
 *
 * This interface exposes the public methods of the `voTagIterator` type.
 *
 * @note As the C language does not provide any mechanism that could be used to
 *   simulate inheritance, inherited members are simply copied from the base
 *   structure.
 *
 * {@extends voIIterator}
 * {@implementingTypes voTagIterator}
 *
 * @par deprecated as of 7.4.0
 */
typedef struct _voITagIterator
{
  /** {@inheritDoc} */
  voYesNo (VO_MSE_CALL *isAtEnd)(voEngine engine, voIterator target);
  /** {@inheritDoc} */
  bool (VO_MSE_CALL *next)(voEngine engine, voIterator target);
  /** {@inheritDoc} */
  voYesNo (VO_MSE_CALL *equals)(voEngine engine, voIterator target, voIterator other);

  /**
   * Returns the type of the tag at the current step of the iteration.
   *
   * @param engine the engine.
   * @param target the target iterator.
   *
   * @return the type of the current tag on success, otherwise `VO_TAG_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voTagIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   */
  voTagType (VO_MSE_CALL *getType)(voEngine engine, voTagIterator target);

  /**
   * Returns the range in the input that corresponds to this tag.
   *
   * @param engine the engine.
   * @param target the target iterator.
   * @param range the recipient for the input range.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voSegmentIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   * @throws VO_INVALID_USER_BUFFER when `range`, or its `elements` member is
   *   not writable.
   * @throws VO_INVALID_STATE when the portion of a MyScript Document associated
   *   with this segment has been modified, invalidating input ranges.
   */
  bool (VO_MSE_CALL *getInputRange)(voEngine engine, voTagIterator target, voInputRange* range);
}
voITagIterator;


/**
 * `voTagList` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voITagList}
 *
 * @par deprecated as of 7.4.0
 */
typedef voEngineObject voTagList;

/**
 * Functions composing the `voITagList` interface.
 *
 * This interface exposes the public methods of the `voTagList` type.
 *
 * {@implementingTypes voTagList}
 *
 * @par deprecated as of 7.4.0
 */
typedef struct _voITagList
{
  /**
    * Returns an iterator over the items of this tag list.
    *
    * @param engine the engine.
    * @param target the target tag list.
    *
    * @return a tag iterator on success, otherwise `NULL`.
    *
    * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
    * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
    * @throws VO_INVALID_OPERATION when `target` is not a `voTagList`.
    * @throws VO_LIMIT_EXCEEDED when the number of `voTagIterator` instances
    *   would exceed the allowed limit (please remember that the limit can be
    *   shared by several types).
    */
  voTagIterator (VO_MSE_CALL *getIterator)(voEngine engine, voTagList target);
}
voITagList;


/**
 * `voDigitalInkTagger` reference type.
 *
 * {@extends voHandwritingContext}
 * {@implements voIDigitalInkTagger, voIRunnable}
 *
 * @par deprecated as of 7.4.0
 */
typedef voHandwritingContext voDigitalInkTagger;

/**
 * Functions composing the `voIDigitalInkTagger` interface.
 *
 * This interface exposes the public methods of the `voDigitalInkTagger` type.
 *
 * {@implementingTypes voDigitalInkTagger}
 *
 * @par deprecated as of 7.4.0
 */
typedef struct _voIDigitalInkTagger
{
  /**
   * Returns the detection sensitivity of the specific tag type.
   *
   * @param engine the engine.
   * @param target the target digital ink tagger.
   * @param tagType the type of tag.
   *
   * @return the detection sensitivity for the specified tag type on success,
   *   otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voDigitalInkTagger`.
   * @throws VO_INVALID_ARGUMENT when `tagType` is invalid.
   */
  float (VO_MSE_CALL *getDetectionSensitivity)(voEngine engine, voDigitalInkTagger target, int32_t tagType);

  /**
   * Sets the detection sensitivity of the specific tag type.
   *
   * @param engine the engine.
   * @param target the target digital ink tagger.
   * @param tagType the type of tag.
   * @param sensitivity the detection sensitivity to be set.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voDigitalInkTagger`.
   * @throws VO_INVALID_ARGUMENT when `tagType` is invalid.
   * @throws VO_INVALID_ARGUMENT when `sensitivity` is not between `0` and `1`.
   */
  bool (VO_MSE_CALL *setDetectionSensitivity)(voEngine engine, voDigitalInkTagger target, int32_t tagType, float sensitivity);

  /**
   * Returns the list of recognized tags.
   *
   * @param engine the engine.
   * @param target the target digital ink tagger.
   *
   * @return a tag list on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voDigitalInkTagger`.
   * @throws VO_INVALID_STATE when `target` was not run.
   * @throws VO_LIMIT_EXCEEDED when the number of `voTagList` instances would
   *   exceed the allowed limit (please remember that the limit can be shared by
   *   several types).
   *
   * @see voIRunnable
   */
  voTagList (VO_MSE_CALL *getTags)(voEngine engine, voDigitalInkTagger target);
}
voIDigitalInkTagger;

#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voDigitalInkTagger` instance.
 *
 * @param engine the engine.
 *
 * @return the newly created `voDigitalInkTagger` instance on success,
 *   otherwise `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_LIMIT_EXCEEDED when the number of `voDigitalInkTagger` instances
 *   would exceed the allowed limit (please remember that the limit can be
 *   shared by several types).
 *
 * @par deprecated as of 7.4.0
 */
VO_INLINE voDigitalInkTagger voCreateDigitalInkTagger(voEngine engine)
{
  return voCreateObject(engine, VO_DigitalInkTagger);
}
#endif // end of: #ifndef VO_NO_FUNCTIONS


// -- Text Segmenter -----------------------------------------------------------

/**
 * `voTextSegmenter` reference type.
 *
 * {@implements voITextSegmenter}
 * {@extends voEngineObject}
 * @since 7.4_0
 */
typedef voEngineObject voTextSegmenter;

/**
 * Functions composing the `voITextSegmenter` interface.
 *
 * This interface exposes the public methods of the `voTextSegmenter` type.
 *
 * {@implementingTypes voTextSegmenter}
 * @see VO_ITextSegmenter, VO_TextSegmenter
 * @since 7.4_0
 */
typedef struct _voITextSegmenter
{
  /**
   * Splits the input string into end-of-word boundaries.
   *
   * @param engine the engine.
   * @param target the target text segmenter.
   * @param charset the charset used to encode parameter strings.
   * @param string the string to be split.
   * @param ends the recipient for the end-of-word boundaries.
   * @param capacity the capacity of the `ends` buffer.
   *
   * @return the number of boundaries in this string on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voTextSegmenter`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `string` or its `bytes` member is not
   *   readable.
   * @throws VO_INVALID_ARGUMENT when `string` is empty.
   * @throws VO_INVALID_USER_BUFFER when `ends` is not writable.
   * @throws VO_INVALID_ARGUMENT when `capacity < 0`.
   */
  int32_t (VO_MSE_CALL *split)(voEngine engine, voTextSegmenter target, voCharset charset, const voString* string, uint32_t* ends, int32_t capacity);
}
voITextSegmenter;

#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voTextSegmenter` instance.
 *
 * @param engine the engine.
 *
 * @return the newly created `voTextSegmenter` instance on success, otherwise
 *   `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_LIMIT_EXCEEDED when the number of `voTextSegmenter`
 *   instances would exceed the allowed limit (please remember that the limit
 *   can be shared by several types).
 */
VO_INLINE voTextSegmenter voCreateTextSegmenter(voEngine engine)
{
  return voCreateObject(engine, VO_TextSegmenter);
}
#endif // end of: #ifndef VO_NO_FUNCTIONS


// -- LinguisticKnowledge Builder ----------------------------------------------

/**
 * `voLinguisticKnowledgeBuilder` reference type.
 *
 * {@implements voILinguisticKnowledgeBuilder}
 * {@extends voEngineObject}
 * @since 7.4_0
 */
typedef voEngineObject voLinguisticKnowledgeBuilder;

/**
 * Functions composing the `voILinguisticKnowledgeBuilder` interface.
 *
 * This interface exposes the public methods of the
 * `voLinguisticKnowledgeBuilder` type.
 *
 * {@implementingTypes voLinguisticKnowledgeBuilder}
 * @see VO_ILinguisticKnowledgeBuilder, VO_LinguisticKnowledgeBuilder
 * @since 7.4_0
 */
typedef struct _voILinguisticKnowledgeBuilder
{
  /**
   * Adds a sequence to the user-adapted linguistic resource.
   *
   * @param engine the engine.
   * @param target the target builder.
   * @param charset the charset used to encode parameter strings.
   * @param sequence the sequence to be added.
   * @param ends the end-of-word boundaries in `sequence`.
   * @param count the size of the `ends` buffer.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a
   *   `voLinguisticKnowledgeBuilder`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `sequence` or its `bytes` member is not
   *   readable.
   * @throws VO_INVALID_USER_BUFFER when `ends` is not readable.
   * @throws VO_INVALID_ARGUMENT when `count <= 0`.
   * @throws VO_LIMIT_EXCEEDED when the user-adapted linguistic resource reached
   *   its maximum size.
   */
  bool (VO_MSE_CALL *addSequence)(voEngine engine, voLinguisticKnowledgeBuilder target, voCharset charset, const voString* sequence, const uint32_t* ends, int32_t count);

  /**
   * Adds a sequence with its corresponding transliteration to the user-adapted
   * linguistic resource.
   *
   * @param engine the engine.
   * @param target the target builder.
   * @param charset the charset used to encode parameter strings.
   * @param sequence the sequence to be processed (e.g. in pinyin-hanzi use
   *   case, `sequence` is the pinyin string).
   * @param ends the end-of-word boundaries in `sequence`.
   * @param transliteration the corresponding transliteration (e.g. in
   *   pinyin-hanzi use case, `transliteration` is the hanzi string).
   * @param transliterationEnds the end-of-word boundaries in `transliteration`.
   * @param count the size of the `ends` and the `transliterationEnds` buffers.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a
   *   `voLinguisticKnowledgeBuilder`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `sequence` or its `bytes` member is not
   *   readable.
   * @throws VO_INVALID_USER_BUFFER when `transliteration` or its `bytes` member
   *   is not readable.
   * @throws VO_INVALID_ARGUMENT when `count <= 0`.
   * @throws VO_LIMIT_EXCEEDED when the user-adapted linguistic resource reached
   *   its maximum size.
   */
  bool (VO_MSE_CALL *addSequenceWithTransliteration)(voEngine engine, voLinguisticKnowledgeBuilder target, voCharset charset, const voString* sequence, const uint32_t* ends, const voString* transliteration, const uint32_t* transliterationEnds, int32_t count);

  /**
   * Prunes a word from the user-adapted linguistic resource.
   *
   * @param engine the engine.
   * @param target the target builder.
   * @param charset the charset used to encode parameter strings.
   * @param word the word to be pruned.
   * @param pruned a optional pointer to a boolean set to `true` when the word
   *   was part of the model.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a
   *   `voLinguisticKnowledgeBuilder`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `word` or its `bytes` member is not
   *   readable.
   */
  bool (VO_MSE_CALL *pruneWord)(voEngine engine, voLinguisticKnowledgeBuilder target, voCharset charset, const voString* word, bool* pruned);

  /**
   * Builds the user-adapted linguistic resource.
   *
   * @param engine the engine.
   * @param target the target builder.
   *
   * @return the user-adapted linguistic resource on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a
   *   `voLinguisticKnowledgeBuilder`.
   * @throws VO_INVALID_STATE when no sequence was added in the user-adapted
   *   linguistic resource.
   */
  voResource (VO_MSE_CALL *build)(voEngine engine, voLinguisticKnowledgeBuilder target);
}
voILinguisticKnowledgeBuilder;

/**
 * Holds the parameters required to create an instance of the
 * `voLinguisticKnowledgeBuilder` type.
 *
 * @see voLinguisticKnowledgeBuilder, voCreateLinguisticKnowledgeBuilder
 */
typedef struct _voLinguisticKnowledgeBuilderInitializer
{
  /**
   * Enables forgetability mechanism.
   */
  bool forget;

  /**
   * A resource built from a previous session.
   */
  voResource resource;
}
voLinguisticKnowledgeBuilderInitializer;

#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voLinguisticKnowledgeBuilder` instance.
 *
 * @param engine the engine.
 * @param forget when `true`, enables forgetability mechanism.
 * @param resource an optional resource built from a previous session by a
 *   `voLinguisticKnowledgeBuilder`.
 *
 * @return the newly created `voLinguisticKnowledgeBuilder` instance on success,
 *   otherwise `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_LIMIT_EXCEEDED when the number of `voLinguisticKnowledgeBuilder`
 *   instances would exceed the allowed limit (please remember that the limit
 *   can be shared by several types).
 * @throws VO_INVALID_ARGUMENT when the resource has not been created by a
 *   `voLinguisticKnowledgeBuilder`.
 */
VO_INLINE voLinguisticKnowledgeBuilder voCreateLinguisticKnowledgeBuilder(voEngine engine, bool forget, voResource resource)
{
  voLinguisticKnowledgeBuilderInitializer initializer = { forget, resource };
  return voCreateObjectEx(engine, VO_LinguisticKnowledgeBuilder, &initializer, sizeof(initializer));
}
#endif // end of: #ifndef VO_NO_FUNCTIONS

// -- Text result iterators ----------------------------------------------------

/**
 * `voTextResultSegmentIterator` reference type.
 *
 * {@extends voIterator}
 * {@implements voITextResultSegmentIterator}
 */
typedef voIterator voTextResultSegmentIterator;

/**
 * `voTextResultCandidateIterator` reference type.
 *
 * {@extends voIterator}
 * {@implements voITextResultCandidateIterator}
 */
typedef voIterator voTextResultCandidateIterator;

/**
 * Functions composing the `voITextResultSegmentIterator` interface.
 *
 * This interface exposes the public methods of the `voTextResultSegmentIterator` type.
 *
 * @note As the C language does not provide any mechanism that could be used to
 *   simulate inheritance, inherited members are simply copied from the base
 *   structure.
 *
 * {@extends voIIterator}
 * {@implementingTypes voTextResultSegmentIterator}
 */
typedef struct _voITextResultSegmentIterator
{
  /** {@inheritDoc} */
  voYesNo (VO_MSE_CALL *isAtEnd)(voEngine engine, voIterator target);
  /** {@inheritDoc} */
  bool (VO_MSE_CALL *next)(voEngine engine, voIterator target);
  /** {@inheritDoc} */
  voYesNo (VO_MSE_CALL *equals)(voEngine engine, voIterator target, voIterator other);
  
  /**
   * Returns an iterator over the candidates of the current segment.
   *
   * @param engine the engine.
   * @param target the target iterator.
   *
   * @return a candidate iterator on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voTextResultSegmentIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   * @throws VO_LIMIT_EXCEEDED when the number of `voTextResultCandidateIterator`
   *   instances would exceed the allowed limit (please remember that the limit
   *   can be shared by several types).
   */
  voTextResultCandidateIterator (VO_MSE_CALL *getCandidates)(voEngine engine, voTextResultSegmentIterator target);
  
  /**
   * Returns an iterator containing a single item, that is the selected candidate
   * for the current segment.
   *
   * @param engine the engine.
   * @param target the target iterator.
   * @param isFrozen boolean output variable set to `true` if the selected
   *   candidate is frozen, `false` otherwise. Can be `NULL`.
   *
   * @return a candidate iterator on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voTextResultSegmentIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   * @throws VO_LIMIT_EXCEEDED when the number of `voTextResultCandidateIterator`
   *   instances would exceed the allowed limit (please remember that the limit
   *   can be shared by several types).
   */
  voTextResultCandidateIterator (VO_MSE_CALL *getSelectedCandidate)(voEngine engine, voTextResultSegmentIterator target, bool* isFrozen);
  
  /**
   * Returns the index of the currently selected candidate
   * for the current segment.
   *
   * @param engine the engine.
   * @param target the target iterator.
   * @param isFrozen boolean output variable set to `true` if the selected
   *   candidate is frozen, `false` otherwise. Can be `NULL`.
   *
   * @return the candidate index on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voTextResultSegmentIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   */
  int (VO_MSE_CALL *getSelectedCandidateIndex)(voEngine engine, voTextResultSegmentIterator target, bool* isFrozen);
  
  /**
   * Sets the index of the currently selected candidate
   * for the current segment.
   *
   * @param engine the engine.
   * @param target the target iterator.
   * @param index the index of the new selected candidate.
   * @param inkSelectionToTypeset an optional selection that will receive the
   *   ink that needs to be typeset again.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voTextResultSegmentIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   * @throws VO_INVALID_ARGUMENT when `id` is invalid.
   * @throws VO_INVALID_OBJECT when `inkSelectionToTypeset` is not a
   *   `voInkSelection`.
   */
  bool (VO_MSE_CALL *setSelectedCandidateIndex)(voEngine engine, voTextResultSegmentIterator target, int newIndex, voEngineObject inkSelectionToTypeset);
}
voITextResultSegmentIterator;

/**
 * Functions composing the `voITextResultCandidateIterator` interface.
 *
 * This interface exposes the public methods of the `voTextResultCandidateIterator` type.
 *
 * @note As the C language does not provide any mechanism that could be used to
 *   simulate inheritance, inherited members are simply copied from the base
 *   structure.
 *
 * {@extends voIIterator}
 * {@implementingTypes voTextResultCandidateIterator}
 */
typedef struct _voITextResultCandidateIterator
{
  /** {@inheritDoc} */
  voYesNo (VO_MSE_CALL *isAtEnd)(voEngine engine, voIterator target);
  /** {@inheritDoc} */
  bool (VO_MSE_CALL *next)(voEngine engine, voIterator target);
  /** {@inheritDoc} */
  voYesNo (VO_MSE_CALL *equals)(voEngine engine, voIterator target, voIterator other);
  
  /**
   * Returns an iterator over the segments of the current candidate.
   *
   * @note A segment iterator iterates over the pieces of input that define the
   *   candidate's segmentation.
   *
   * @param engine the engine.
   * @param target the target iterator.
   *
   * @return a segment iterator on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voTextResultCandidateIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   * @throws VO_LIMIT_EXCEEDED when the number of `voTextResultSegmentIterator` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   */
  voTextResultSegmentIterator (VO_MSE_CALL *getSegments)(voEngine engine, voTextResultCandidateIterator target);
  
  /**
   * Returns the index of the current candidate within the list of candidates
   * of the parent segment.
   *
   * @param engine the engine.
   * @param target the target iterator.
   *
   * @return the candidate index on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voTextResultCandidateIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   */
  int (VO_MSE_CALL *getIndex)(voEngine engine, voTextResultCandidateIterator target);
  
  /**
   * Returns the label of the current candidate.
   *
   * @param engine the engine.
   * @param target the target iterator.
   * @param charset the charset used to encode parameter strings.
   * @param label the recipient for the label.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voTextResultCandidateIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `label` or its `bytes` member is not
   *   writable.
   */
  bool (VO_MSE_CALL *getLabel)(voEngine engine, voTextResultCandidateIterator target, voCharset charset, voString* label);
  
  /**
   * Returns the label of the current candidate along with begin positions
   * of the completion and prediction parts of the label.
   *
   * @param engine the engine.
   * @param target the target iterator.
   * @param charset the charset used to encode parameter strings.
   * @param label the recipient for the label.
   * @param completionOffset the offset of the beginning of the completion
   *   part of the label.
   * @param predictionOffset the offset of the beginning of the prediction
   *   part of the label.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voTextResultCandidateIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `label` or its `bytes` member,
   *   or `completionOffset` or `predictionOffset` is not writable.
   */
  bool (VO_MSE_CALL *getLabelEx)(voEngine engine, voTextResultCandidateIterator target, voCharset charset, voString* label, uint32_t* completionOffset, uint32_t* predictionOffset);
  
  /**
   * Returns the normalized recognition score of the current candidate.
   *
   * @param engine the engine.
   * @param target the target iterator.
   *
   * @return the normalized recognition score of this candidate on success,
   *   otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voTextResultCandidateIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   */
  float (VO_MSE_CALL *getNormalizedRecognitionScore)(voEngine engine, voTextResultCandidateIterator target);
  
  /**
   * Returns the resemblance score of the current candidate.
   *
   * @param engine the engine.
   * @param target the target iterator.
   *
   * @return the resemblance score of the best candidate on success, otherwise
   *   `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voTextResultCandidateIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   *
   * @since 8.1_0
   */
  float (VO_MSE_CALL *getObservationScore)(voEngine engine, voTextResultCandidateIterator target);
  
  /**
   * Returns the flags of the current candidate.
   *
   * @param engine the engine.
   * @param target the target iterator.
   *
   * @return the combination of flags on success, otherwise `VO_INVALID_FLAGS`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voTextResultCandidateIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   */
  VO_FLAGS_T(voCandidateFlag) (VO_MSE_CALL *getFlags)(voEngine engine, voTextResultCandidateIterator target);
  
  /**
   * Returns the name of the linguistic knowledge used to generate the current
   * word-level candidate.
   *
   * @note In case the linguistic knowledge name is unknown, an empty string
   *   is returned.
   *
   * @param engine the engine.
   * @param target the target iterator.
   * @param charset the charset used to encode parameter strings.
   * @param name the recipient for the linguistic knowledge name.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voTextResultCandidateIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   * @throws VO_INVALID_OPERATION when `target` is not a word-level iterator.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `name` or its `bytes` member is not
   *   writable.
   * @throws VO_INVALID_STATE when there was no attached linguistic knowledge at
   *   recognition time.
   */
  bool (VO_MSE_CALL *getSourceLinguisticKnowledgeName)(voEngine engine, voTextResultCandidateIterator target, voCharset charset, voString* name);
  
  /**
   * Returns an iterator over the transliteration candidates of the current
   * candidate.
   *
   * @note Transliteration candidates are only available at word level.
   *
   * @note This iterator runs through a filtered subset of all the candidates
   * of the parent segment. If you want to select one of these transliteration
   * candidates, you need to call the getIndex() API of this iterator to
   * retrieve the actual absolute index of the candidate within the list of
   * candidates. Then you can use this absolute index to set the selected
   * candidate of the parent segment.
   *
   * @param engine the engine.
   * @param target the target iterator.
   *
   * @return an iterator over the transliteration candidates of the current
   *   candidate on success, otherwise `NULL`.
   *   When `NULL`, use `voGetError()` to figure out whether an error occurred.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voTextResultCandidateIterator`.
   * @throws VO_INVALID_STATE when there is no transliteration available.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   * @throws VO_LIMIT_EXCEEDED when the number of `voTextResultCandidateIterator`
   *   instances would exceed the allowed limit (please remember that the limit
   *   can be shared by several types).
   */
  voTextResultCandidateIterator (VO_MSE_CALL *getTransliterationCandidates)(voEngine engine, voTextResultCandidateIterator target);
  
  /**
   * Returns an iterator containing a single item, that is the selected transliteration
   * for the current candidate.
   *
   * @note Transliteration candidates are only available at word level.
   *
   * @param engine the engine.
   * @param target the target iterator.
   *
   * @return an iterator pointing to the selected transliteration candidate of the
   *   current candidate on success, otherwise `NULL`.
   *   When `NULL`, use `voGetError()` to figure out whether an error occurred.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voTextResultCandidateIterator`.
   * @throws VO_INVALID_STATE when there is no transliteration available.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   * @throws VO_LIMIT_EXCEEDED when the number of `voTextResultCandidateIterator`
   *   instances would exceed the allowed limit (please remember that the limit
   *   can be shared by several types).
   */
  voTextResultCandidateIterator (VO_MSE_CALL *getSelectedTransliterationCandidate)(voEngine engine, voTextResultCandidateIterator target);

  /**
   * Returns an iterator containing a single item, that corresponds to the user input
   * of the current transliteration candidate.
   *
   * @note In case the current candidate is not a transliteration candidate, an
   *   error is returned.
   *
   * @param engine the engine.
   * @param target the target iterator.
   *
   * @return an iterator pointing to the user input of the current transliteration
   *   candidate on success, otherwise `NULL`.
   *   When `NULL`, use `voGetError()` to figure out whether an error occurred.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voTextResultCandidateIterator`.
   * @throws VO_INVALID_STATE when the current candidate is not a transliteration candidate.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   * @throws VO_LIMIT_EXCEEDED when the number of `voTextResultCandidateIterator`
   *   instances would exceed the allowed limit (please remember that the limit
   *   can be shared by several types).
   */
  voTextResultCandidateIterator (VO_MSE_CALL *getInputCandidate)(voEngine, voTextResultCandidateIterator target);

  /**
   * Returns the label of the user input of the current transliteration candidate.
   *
   * @note This method can be used at word and character levels.
   *
   * @note In case the current candidate is not a transliteration candidate, an
   *   error is returned.
   *
   * @param engine the engine.
   * @param target the target iterator.
   * @param charset the charset used to encode parameter strings.
   * @param label the recipient for the label.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voTextResultCandidateIterator`.
   * @throws VO_INVALID_STATE when the current candidate is not a transliteration candidate.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `label` or its `bytes` member is not
   *   writable.
   */
  bool (VO_MSE_CALL *getInputLabel)(voEngine engine, voTextResultCandidateIterator target, voCharset charset, voString* label);

  /**
   * Returns the label of the user input of the current transliteration candidate
   * along with begin positions of the completion and prediction parts of the label.
   *
   * @param engine the engine.
   * @param target the target iterator.
   * @param charset the charset used to encode parameter strings.
   * @param label the recipient for the label.
   * @param completionOffset the offset of the beginning of the completion
   *   part of the label.
   * @param predictionOffset the offset of the beginning of the prediction
   *   part of the label.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voTextResultCandidateIterator`.
   * @throws VO_INVALID_STATE when the current candidate is not a transliteration candidate.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `label` or its `bytes` member,
   *   or `completionOffset` or `predictionOffset` is not writable.
   */
  bool (VO_MSE_CALL *getInputLabelEx)(voEngine engine, voTextResultCandidateIterator target, voCharset charset, voString* label, uint32_t* completionOffset, uint32_t* predictionOffset);
}
voITextResultCandidateIterator;

#endif // end of: #ifndef C_MYSCRIPTTEXT_H
