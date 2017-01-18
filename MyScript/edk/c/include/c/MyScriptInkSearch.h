#ifndef C_MYSCRIPTINKSEARCH_H
#define C_MYSCRIPTINKSEARCH_H 0x08000000
/**
 * @file c/MyScriptInkSearch.h
 * Native interface to the MyScript InkSearch service.
 */

#include "MyScriptEngine.h"
#include "MyScriptText.h"


// -- error codes --------------------------------------------------------------

/**
 * Lists the MyScript InkSearch errors.
 */
enum VO_INKSEARCH_ERR
{
  /**
   * Unable to complete the requested operation because a `voIndex` is missing.
   */
  VO_MISSING_INDEX = VO_INVALID_STATE | (3 << 16),

  /**
   * Unable to complete the requested operation because a `voQuery` is missing.
   */
  VO_MISSING_QUERY
};


// -- types and interfaces -----------------------------------------------------

/**
 * Lists the MyScript InkSearch types.
 */
enum VO_INKSEARCH_T
{
  /**
   * `voIndexer` type identifier.
   *
   * @see voIndexer
   */
  VO_Indexer = 800,

  /**
   * `voStructuredInputIndexer` type identifier.
   *
   * @see voStructuredInputIndexer
   */
  VO_StructuredInputIndexer = 802,

  /**
   * `voStructuredInputSlimIndexer` type identifier.
   *
   * @see voStructuredInputSlimIndexer
   */
  VO_StructuredInputSlimIndexer = 804,

  /**
   * `voIndex` type identifier.
   *
   * @see voIndex
   */
  VO_Index,

  /**
   * `voQuery` type identifier.
   *
   * @see voQuery
   */
  VO_Query,

  /**
   * `voVocabularyQuery` type identifier.
   *
   * @see voVocabularyQuery
   */
  VO_VocabularyQuery,

  /**
   * `voStringQuery` type identifier.
   *
   * @see voStringQuery
   */
  VO_StringQuery,

  /**
   * `voOrQuery` type identifier.
   *
   * @see voOrQuery
   */
  VO_OrQuery,

  /**
   * `voAndQuery` type identifier.
   *
   * @see voAndQuery
   */
  VO_AndQuery,

  /**
   * `voAndNotQuery` type identifier.
   *
   * @see voAndNotQuery
   */
  VO_AndNotQuery,

  /**
   * `voNearQuery` type identifier.
   *
   * @see voNearQuery
   */
  VO_NearQuery,

  /**
   * `voFinder` type identifier.
   *
   * @see voFinder
   */
  VO_Finder,

  /**
   * `voFindResult` type identifier.
   *
   * @see voFindResult
   */
  VO_FindResult,

  /**
   * `voOccurrenceIterator` type identifier.
   *
   * @see voOccurrenceIterator
   */
  VO_OccurrenceIterator,

  /**
   * `voOccurrenceCharacterIterator` type identifier.
   *
   * @see voOccurrenceCharacterIterator
   * @since 4.1_0
   */
  VO_OccurrenceCharacterIterator,

  /**
   * `voTextIndexer` type identifier.
   *
   * @see voTextIndexer
   * @since 6.0_0
   */
  VO_TextIndexer
};


/**
 * Lists the MyScript InkSearch interfaces.
 */
enum VO_INKSEARCH_I
{
  /**
   * `voIIndexingResultProvider` interface identifier.
   *
   * @see voIIndexingResultProvider
   */
  VO_IIndexingResultProvider = 900,

  /**
   * `voIFindResultProvider` interface identifier.
   *
   * @see voIFindResultProvider
   */
  VO_IFindResultProvider,

  /**
   * `voIFindResult` interface identifier.
   *
   * @see voIFindResult
   */
  VO_IFindResult,

  /**
   * `voIOccurrenceIterator` interface identifier.
   *
   * @see voIOccurrenceIterator
   */
  VO_IOccurrenceIterator,

  /**
   * `voIOccurrenceCharacterIterator` interface identifier.
   *
   * @see voIOccurrenceCharacterIterator
   * @since 4.1_0
   */
  VO_IOccurrenceCharacterIterator
};


// --Index ---------------------------------------------------------------------

/**
 * `voIndex` reference type.
 *
 * {@extends voEngineObject}
 * @see voStructuredInputIndexer, voStructuredInputSlimIndexer
 */
typedef voEngineObject voIndex;


// -- Indexer ------------------------------------------------------------------

/**
 * `voIndexer` reference type.
 *
 * {@extends voHandwritingContext}
 * {@subTypes voStructuredInputIndexer, voStructuredInputSlimIndexer}
 * {@implements voIRunnable, voIIndexingResultProvider}
 * {@properties VO_INDEXER_PROP}
 * @see voStructuredInputIndexer, voStructuredInputSlimIndexer
 */
typedef voHandwritingContext voIndexer;

/**
 * Lists the index size versus accuracy compromise special values.
 */
enum VO_INDEX_SIZE_ACCURACY_COMPROMISE_VALUE
{
  VO_MINIMIZE_INDEX_SIZE  = 0,  /**< Minimize index size. */
  VO_MAXIMIZE_ACCURACY    = 255 /**< Maximize search accuracy. */
};

/**
 * {@propertySet Indexer}
 */
enum VO_INDEXER_PROP
{
  /**
   * Adjusts the compromise between index size and search accuracy.
   *
   * {@property uint8_t}
   *
   * @note You can use `VO_MINIMIZE_INDEX_SIZE`, `VO_MAXIMIZE_ACCURACY`, or any
   *   value between `0` and `255`. However, there are less than 256 levels and
   *   the value will be adjusted to the closest acceptable value. The default
   *   value of this property is `128`.
   */
  VO_INDEX_SIZE_ACCURACY_COMPROMISE = (VO_Indexer << 16)
};


// -- IIndexingResultProvider --------------------------------------------------

/**
 * Functions composing the `voIIndexingResultProvider` interface.
 *
 * {@implementingTypes voStructuredInputIndexer, voStructuredInputSlimIndexer}
 */
typedef struct _voIIndexingResultProvider
{
  /**
   * Returns the indexing result associated with the target provider.
   *
   * @param engine the engine.
   * @param target the target provider.
   *
   * @return an index on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_INVALID_STATE when `target` is not in a state where an indexing
   *   result is available.
   * @throws VO_LIMIT_EXCEEDED when the number of `voIndex` instances would
   *   exceed the allowed limit (please remember that the limit can be shared by
   *   several types).
   */
  voIndex (VO_MSE_CALL *getResult)(voEngine engine, voEngineObject target);
}
voIIndexingResultProvider;


// -- StructuredInputIndexer ---------------------------------------------------

/**
 * `voStructuredInputIndexer` reference type.
 *
 * {@extends voIndexer}
 */
typedef voIndexer voStructuredInputIndexer;

#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voStructuredInputIndexer` instance.
 *
 * @param engine the engine.
 *
 * @return the newly created `voStructuredInputIndexer` instance on success,
 *   otherwise `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_LIMIT_EXCEEDED when the number of `voStructuredInputIndexer`
 *   instances would exceed the allowed limit (please remember that the limit
 *   can be shared by several types).
 */
VO_INLINE voStructuredInputIndexer voCreateStructuredInputIndexer(voEngine engine)
{
  return voCreateObject(engine, VO_StructuredInputIndexer);
}
#endif // end of: #ifndef VO_NO_FUNCTIONS


// -- StructuredInputSlimIndexer -----------------------------------------------

/**
 * `voStructuredInputSlimIndexer` reference type.
 *
 * {@extends voIndexer}
 */
typedef voIndexer voStructuredInputSlimIndexer;

#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voStructuredInputSlimIndexer` instance.
 *
 * @param engine the engine.
 *
 * @return the newly created `voStructuredInputSlimIndexer` instance on success,
 *   otherwise `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_LIMIT_EXCEEDED when the number of `voStructuredInputSlimIndexer`
 *   instances would exceed the allowed limit (please remember that the limit
 *   can be shared by several types).
 */
VO_INLINE voStructuredInputSlimIndexer voCreateStructuredInputSlimIndexer(voEngine engine)
{
  return voCreateObject(engine, VO_StructuredInputSlimIndexer);
}
#endif // end of: #ifndef VO_NO_FUNCTIONS


// -- TextIndexer --------------------------------------------------------------

/**
 * `voTextIndexer` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voIAttachTarget, voIContentFieldProcessor}
 * {@properties VO_INDEXER_PROP}
 */
typedef voEngineObject voTextIndexer;

#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voTextIndexer` instance.
 *
 * @param engine the engine.
 *
 * @return the newly created `voTextIndexer` instance on success, otherwise
 *   `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_LIMIT_EXCEEDED when the number of `voTextIndexer` instances would
 *   exceed the allowed limit (please remember that the limit can be shared by
 *   several types).
 */
VO_INLINE voTextIndexer voCreateTextIndexer(voEngine engine)
{
  return voCreateObject(engine, VO_TextIndexer);
}
#endif // end of: #ifndef VO_NO_FUNCTIONS


// -- Query --------------------------------------------------------------------

/**
 * `voQuery` reference type.
 *
 * {@extends voEngineObject}
 * {@subTypes voVocabularyQuery, voStringQuery, voOrQuery, voAndQuery, voAndNotQuery, voNearQuery}
 */
typedef voEngineObject voQuery;

/**
 * `voVocabularyQuery` reference type.
 *
 * {@extends voQuery}
 */
typedef voQuery voVocabularyQuery;

/**
 * `voStringQuery` reference type.
 *
 * {@extends voQuery}
 */
typedef voQuery voStringQuery;

/**
 * `voOrQuery` reference type.
 *
 * {@extends voQuery}
 */
typedef voQuery voOrQuery;

/**
 * `voAndQuery` reference type.
 *
 * {@extends voQuery}
 */
typedef voQuery voAndQuery;

/**
 * `voAndNotQuery` reference type.
 *
 * {@extends voQuery}
 */
typedef voQuery voAndNotQuery;

/**
 * `voNearQuery` reference type.
 *
 * {@extends voQuery}
 */
typedef voQuery voNearQuery;

/**
 * Holds the parameters required to create a new `voVocabularyQuery` instance.
 *
 * @see voVocabularyQuery, voCreateVocabularyQuery
 */
typedef struct _voVocabularyQueryInitializer
{
  /**
   * Vocabulary.
   */
  voVocabulary vocabulary;
}
voVocabularyQueryInitializer;

/**
 * Holds the parameters required to create an instance of the `voStringQuery`
 * type.
 *
 * @see voStringQuery, voCreateStringQuery
 */
typedef struct _voStringQueryInitializer
{
  /**
   * Charset.
   */
  voCharset charset;

  /**
   * String.
   */
  const voString* string;

  /**
   * Case sensitivity.
   */
  bool matchCase;

  /**
   * Accent sensitivity.
   */
  bool matchAccent;

  /**
   * Interpret, or not, wildcard characters: '?' (any character) and '*' (any
   * character, any number of times).
   */
  bool interpretWildcards;
}
voStringQueryInitializer;

/**
 * Holds the parameters required to create an instance of the `voBinaryQuery`
 * type (`voOrQuery`, `voAndQuery`, `voAndNotQuery`, `voNearQuery`).
 *
 * @see voBinaryQuery, voCreateBinaryQuery
 */
typedef struct _voBinaryQueryInitializer
{
  /**
   * First query.
   */
  voQuery query1;

  /**
   * Second query.
   */
  voQuery query2;
}
voBinaryQueryInitializer;

#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voVocabularyQuery` instance.
 *
 * @param engine the engine.
 * @param vocabulary the associated vocabulary.
 *
 * @return the newly created `voVocabularyQuery` instance on success,
 *   otherwise `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_NO_SUCH_OBJECT when the `vocabulary` reference is invalid.
 * @throws VO_INVALID_OBJECT when `vocabulary` is not a `voVocabulary`.
 * @throws VO_NOT_COMPILED when `vocabulary` is not compiled.
 * @throws VO_LIMIT_EXCEEDED when the number of `voVocabularyQuery` instances
 *   would exceed the allowed limit (please remember that the limit can be
 *   shared by several types).
 */
VO_INLINE voVocabularyQuery voCreateVocabularyQuery(voEngine engine, voVocabulary vocabulary)
{
  voVocabularyQueryInitializer initializer = { vocabulary };
  return voCreateObjectEx(engine, VO_VocabularyQuery, &initializer, sizeof(initializer));
}

/**
 * Creates a new `voStringQuery` instance.
 *
 * @param engine the engine.
 * @param charset the charset used to encode parameter strings.
 * @param string the string.
 * @param matchCase case sensitivity.
 * @param matchAccent accent sensitivity.
 * @param interpretWildcards interpret, or not, wildcard characters: '?'
 *   (any character) and '*' (any character, any number of times).
 *
 * @return the newly created `voStringQuery` instance on success, otherwise
 *   `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
 * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
 *   default charset was set.
 * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
 * @throws VO_INVALID_USER_BUFFER when `string` or its `bytes` member is not
 *   readable.
 * @throws VO_LIMIT_EXCEEDED when the number of `voStringQuery` instances would
 *   exceed the allowed limit (please remember that the limit can be
 *   shared by several types).
 */
VO_INLINE voStringQuery voCreateStringQuery(voEngine engine, voCharset charset, const voString* string, bool matchCase, bool matchAccent, bool interpretWildcards)
{
  voStringQueryInitializer initializer = { charset, string, matchCase, matchAccent, interpretWildcards };
  return voCreateObjectEx(engine, VO_StringQuery, &initializer, sizeof(initializer));
}

/**
 * Creates a new binary query of the specified type.
 *
 * @param engine the engine.
 * @param type the type of query to be created: `VO_OrQuery`, `VO_AndQuery`,
 *   `VO_AndNotQuery` or `VO_NearQuery`.
 * @param query1 the first query.
 * @param query2 the second query.
 *
 * @return the newly created `voQuery` instance on success, otherwise `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_NO_SUCH_TYPE when the specified type is invalid.
 * @throws VO_TYPE_NOT_CONCRETE when the specified type is not concrete.
 * @throws VO_NO_SUCH_CONSTRUCTOR when the specified type does not have a
 *   constructor with the given initializer.
 * @throws VO_NO_SUCH_OBJECT when the `query1` reference is invalid.
 * @throws VO_INVALID_OBJECT when `query1` is not a `voQuery`.
 * @throws VO_NO_SUCH_OBJECT when the `query2` reference is invalid.
 * @throws VO_INVALID_OBJECT when `query2` is not a `voQuery`.
 * @throws VO_LIMIT_EXCEEDED when the number of `voQuery` instances would exceed
 *   the allowed limit (please remember that the limit can be shared by several
 *   types).
 */
VO_INLINE voQuery voCreateBinaryQuery(voEngine engine, voTypeId type, voQuery query1, voQuery query2)
{
  voBinaryQueryInitializer initializer = { query1, query2 };
  return voCreateObjectEx(engine, type, &initializer, sizeof(initializer));
}
#endif // end of: #ifndef VO_NO_FUNCTIONS


// -- Finder -------------------------------------------------------------------

/**
 * `voFinder` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voIAttachTarget, voIRunnable, voIFindResultProvider}
 * {@properties VO_FINDER_PROP}
 */
typedef voEngineObject voFinder;

#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voFinder` instance.
 *
 * @param engine the engine.
 *
 * @return the newly created `voFinder` instance on success, otherwise `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_LIMIT_EXCEEDED when the number of `voFinder` instances would
 *   exceed the allowed limit (please remember that the limit can be shared by
 *   several types).
 */
VO_INLINE voFinder voCreateFinder(voEngine engine)
{
  return voCreateObject(engine, VO_Finder);
}
#endif // end of: #ifndef VO_NO_FUNCTIONS

/**
 * {@propertySet Finder}
 */
enum VO_FINDER_PROP
{
  /**
   * Value between `0` and `1`, below which occurrences are rejected.
   *
   * The default value is `0.3f`.
   *
   * {@property float}
   */
  VO_FIND_SCORE_THRESHOLD = (VO_Finder << 16)
};


// -- FindResult ---------------------------------------------------------------

/**
 * `voFindResult` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voIFindResult}
 */
typedef voEngineObject voFindResult;

/**
 * `voOccurrenceIterator` reference type.
 *
 * {@extends voIterator}
 * {@implements voIOccurrenceIterator}
 */
typedef voIterator voOccurrenceIterator;

/**
 * `voOccurrenceCharacterIterator` reference type.
 *
 * {@extends voIterator}
 * {@implements voIOccurrenceCharacterIterator}
 * @since 4.1_0
 */
typedef voIterator voOccurrenceCharacterIterator;

/**
 * Functions composing the `voIFindResultProvider` interface.
 *
 * {@implementingTypes voFinder}
 */
typedef struct _voIFindResultProvider
{
  /**
   * Returns the find result associated with the target provider.
   *
   * @param engine the engine.
   * @param target the target provider.
   *
   * @return a find result on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_INVALID_STATE when `target` is not in a state where a find
   *   result is available.
   * @throws VO_LIMIT_EXCEEDED when the number of `voFindResult` instances would
   *   exceed the allowed limit (please remember that the limit can be shared by
   *   several types).
   */
  voFindResult (VO_MSE_CALL *getResult)(voEngine engine, voEngineObject target);
}
voIFindResultProvider;

/**
 * Functions composing the `voIFindResult` interface.
 *
 * This interface exposes the public methods of the `voFindResult` type.
 *
 * {@implementingTypes voFindResult}
 */
typedef struct _voIFindResult
{
  /**
   * Returns the global search result score, which is a combination of all the
   * occurrence scores. The score varies from `0.0f` (low probability) to `1.0f`
   * (high probability).
   *
   * @param engine the engine.
   * @param target the target find result.
   *
   * @return the global search score on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voFindResult`.
   */
  float (VO_MSE_CALL *getGlobalScore)(voEngine engine, voFindResult target);

  /**
   * Returns an iterator over the different occurrences.
   *
   * @param engine the engine.
   * @param target the target find result.
   *
   * @return an iterator over the different occurrences on success, otherwise
   *   `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voFindResult`.
   * @throws VO_LIMIT_EXCEEDED when the number of `voOccurrenceIterator`
   *   instances would exceed the allowed limit (please remember that the limit
   *   can be shared by several types).
   */
  voOccurrenceIterator (VO_MSE_CALL *getOccurrences)(voEngine engine, voFindResult target);
}
voIFindResult;

/**
 * Functions composing the `voIOccurrenceIterator` interface.
 *
 * This interface exposes the public methods of the `voOccurrenceIterator` type.
 *
 * @note As the C language does not provide any mechanism that could be used to
 *   simulate inheritance, inherited members are simply copied from the base
 *   structure.
 *
 * {@extends voIIterator}
 * {@implementingTypes voOccurrenceIterator}
 */
typedef struct _voIOccurrenceIterator
{
  /** {@inheritDoc} */
  voYesNo (VO_MSE_CALL *isAtEnd)(voEngine engine, voIterator target);
  /** {@inheritDoc} */
  bool (VO_MSE_CALL *next)(voEngine engine, voIterator target);
  /** {@inheritDoc} */
  voYesNo (VO_MSE_CALL *equals)(voEngine engine, voIterator target, voIterator other);

  /**
   * Returns the label of this occurrence.
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
   * @throws VO_INVALID_OPERATION when `target` is not a `voOccurrenceIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `label` or its `bytes` member is not
   *   writable.
   */
  bool (VO_MSE_CALL *getLabel)(voEngine engine, voOccurrenceIterator target, voCharset charset, voString* label);

  /**
   * Returns the score of this occurrence.
   *
   * @param engine the engine.
   * @param target the target iterator.
   *
   * @return the score of this occurrence on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voOccurrenceIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   */
  float (VO_MSE_CALL *getScore)(voEngine engine, voOccurrenceIterator target);

  /**
   * Returns the range in the input that corresponds to this occurrence.
   *
   * @param engine the engine.
   * @param target the target iterator.
   * @param range the recipient for the input range.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voOccurrenceIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   * @throws VO_INVALID_USER_BUFFER when `range`, or its `elements` member is
   *   not writable.
   * @throws VO_INVALID_STATE when the portion of a MyScript Document associated
   *   with this segment has been modified, invalidating input ranges.
   */
  bool (VO_MSE_CALL *getInputRange)(voEngine engine, voOccurrenceIterator target, voInputRange* range);

  /**
   * Returns the index of the search unit that contains this occurrence.
   *
   * @param engine the engine.
   * @param target the target iterator.
   *
   * @return the search unit of this occurrence on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voOccurrenceIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   */
  int16_t (VO_MSE_CALL *getSearchUnit)(voEngine engine, voOccurrenceIterator target);

  /**
   * Returns an iterator over the different characters of this occurrence.
   *
   * @param engine the engine.
   * @param target the target iterator.
   *
   * @return an iterator over the characters of this occurrence on success,
   *   otherwise `NULL`
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voOccurrenceIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   * @throws VO_LIMIT_EXCEEDED when the number of
   *   `voOccurrenceCharacterIterator` instances would exceed the allowed limit
   *   (please remember that the limit can be shared by several types).
   *
   * @since 4.1_0
   */
  voOccurrenceCharacterIterator (VO_MSE_CALL *getCharacters)(voEngine engine, voOccurrenceIterator target);

  /**
   * Returns the index of the matched query.
   *
   * @param engine the engine.
   * @param target the target iterator.
   *
   * @return the index of the matched query on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voOccurrenceIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   *
   * @since 4.4_0
   */
  int32_t (VO_MSE_CALL *getQueryIndex)(voEngine engine, voOccurrenceIterator target);
}
voIOccurrenceIterator;

/**
 * Functions composing the `voIOccurrenceCharacterIterator` interface.
 *
 * This interface exposes the public methods of the
 * `voOccurrenceCharacterIterator` type.
 *
 * @note As the C language does not provide any mechanism that could be used to
 *   simulate inheritance, inherited members are simply copied from the base
 *   structure.
 *
 * {@extends voIIterator}
 * {@implementingTypes voOccurrenceCharacterIterator}
 * @since 4.1_0
 */
typedef struct _voIOccurrenceCharacterIterator
{
  /** {@inheritDoc} */
  voYesNo (VO_MSE_CALL *isAtEnd)(voEngine engine, voIterator target);
  /** {@inheritDoc} */
  bool (VO_MSE_CALL *next)(voEngine engine, voIterator target);
  /** {@inheritDoc} */
  voYesNo (VO_MSE_CALL *equals)(voEngine engine, voIterator target, voIterator other);

  /**
   * Returns the label of this character occurrence.
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
   * @throws VO_INVALID_OPERATION when `target` is not a
   *   `voOccurrenceCharacterIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `label` or its `bytes` member is not
   *   writable.
   */
  bool (VO_MSE_CALL *getLabel)(voEngine engine, voOccurrenceCharacterIterator target, voCharset charset, voString* label);

  /**
   * Returns the range in the input that corresponds to this character
   * occurrence.
   *
   * @param engine the engine.
   * @param target the target iterator.
   * @param range the recipient for the input range.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a
   *   `voOccurrenceCharacterIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   * @throws VO_INVALID_USER_BUFFER when `range`, or its `elements` member is
   *   not writable.
   * @throws VO_INVALID_STATE when the portion of a MyScript Document associated
   *   with this segment has been modified, invalidating input ranges.
   *
  */
  bool (VO_MSE_CALL *getInputRange)(voEngine engine, voOccurrenceCharacterIterator target, voInputRange* range);
}
voIOccurrenceCharacterIterator;

#endif // end of: #ifndef C_MYSCRIPTINKSEARCH_H
