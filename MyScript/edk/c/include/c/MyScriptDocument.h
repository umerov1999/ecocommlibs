#ifndef C_MYSCRIPTDOCUMENT_H
#define C_MYSCRIPTDOCUMENT_H 0x08010000

#include "MyScriptEngine.h"
#include "MyScriptConfigurationManager.h"
#include "MyScriptJSON.h"
#include "MyScriptInk.h"
#include "MyScriptInkSearch.h"


// -- types and interfaces -----------------------------------------------------

/**
 * Lists the MyScript Document types.
 */
enum VO_DOCUMENT_T
{
  /**
   * `voDocument` type identifier.
   *
   * @see voDocument
   */
  VO_Document = 3200,

  /**
   * `voPage` type identifier.
   *
   * @see voPage
   */
  VO_Page,

  /**
   * `voPageSelection` type identifier.
   *
   * @see voPageSelection
   */
  VO_PageSelection,

  /**
   * `voContent` type identifier.
   *
   * @see voContent
   */
  VO_Content,

  /**
   * `voContentField` type identifier.
   *
   * @see voContentField
   */
  VO_ContentField,

  /**
   * `voLayout` type identifier.
   *
   * @see voLayout
   */
  VO_Layout,

  /**
   * `voLayerIterator` type identifier.
   *
   * @see voLayerIterator
   */
  VO_LayerIterator,

  /**
   * `voPenIterator` type identifier.
   *
   * @see voPenIterator
   */
  VO_PenIterator,

  /**
   * `voLayoutItemIterator` type identifier.
   *
   * @see voLayoutItemIterator
   */
  VO_LayoutItemIterator,

  /**
   * `voDocumentFinder` type identifier.
   *
   * @see voDocumentFinder
   */
  VO_DocumentFinder,

  /**
   * `voDocumentFindResult` type identifier.
   *
   * @see voDocumentFindResult
   */
  VO_DocumentFindResult,

  /**
   * `voDocumentFindOccurrenceIterator` type identifier.
   *
   * @see voDocumentFindOccurrenceIterator
   */
  VO_DocumentFindOccurrenceIterator,

  /**
   * `voObjectIterator` type identifier.
   *
   * @see voObjectIterator
   */
  VO_ObjectIterator,

  /**
   * `voGuideIterator` type identifier.
   *
   * @see voGuideIterator
   */
  VO_GuideIterator,

  /**
   * `voContentProcessor` type identifier.
   *
   * @see voContentProcessor
   * @since 7.0_0
   */
  VO_ContentProcessor,

  /**
   * `voPageChangeSet` type identifier.
   *
   * @see voPageChangeSet
   * @since 7.0_0
   */
  VO_PageChangeSet,

  /**
   * `voTextInspector` type identifier.
   *
   * @see voTextInspector
   */
  VO_TextInspector,

  /**
   * `voTextInterval` type identifier.
   *
   * @see voTextInterval
   */
  VO_TextInterval,

  /**
   * `voTypesetData` type identifier.
   *
   * @see voTypesetData
   * @since 7.0_0
   */
  VO_TypesetData,

  /**
   * `voTypesetItem` type identifier.
   *
   * @see voTypesetItem
   * @since 7.0_0
   */
  VO_TypesetItem,

  /**
   * `voFontIterator` type identifier.
   *
   * @see voFontIterator
   * @since 7.0_0
   */
  VO_FontIterator = 3260,
};


/**
 * Lists the MyScript Document interfaces.
 */
enum VO_DOCUMENT_I
{
  /**
   * `voIDocument` interface identifier.
   *
   * @see voIDocument
   */
  VO_IDocument = 3220,

  /**
   * `voIPage` interface identifier.
   *
   * @see voIPage
   */
  VO_IPage,

  /**
   * `voIPageSelection` interface identifier.
   *
   * @see voIPageSelection
   */
  VO_IPageSelection,

  /**
   * `voIContent` interface identifier.
   *
   * @see voIContent
   */
  VO_IContent,

  /**
   * `voIContentField` interface identifier.
   *
   * @see voIContentField
   */
  VO_IContentField,

  /**
   * `voILayout` interface identifier.
   *
   * @see voILayout
   */
  VO_ILayout,

  /**
   * `voILayerIterator` interface identifier.
   *
   * @see voILayerIterator
   */
  VO_ILayerIterator,

  /**
   * `voIPenIterator` interface identifier.
   *
   * @see voIPenIterator
   */
  VO_IPenIterator,

  /**
   * `voILayoutItemIterator` interface identifier.
   *
   * @see voILayoutItemIterator
   */
  VO_ILayoutItemIterator,

  /**
   * `voIPageUpdateListener` interface identifier.
   *
   * @see voIPageUpdateListener
   */
  VO_IPageUpdateListener,

  /**
   * `voIContentFieldProcessor` interface identifier.
   *
   * @see voIContentFieldProcessor
   */
  VO_IContentFieldProcessor,

  /**
   * `voIDocumentFinder` interface identifier.
   *
   * @see voIDocumentFinder
   */
  VO_IDocumentFinder,

  /**
   * `voIDocumentFindResult` interface identifier.
   *
   * @see voIDocumentFindResult
   */
  VO_IDocumentFindResult,

  /**
   * `voIDocumentFindOccurrenceIterator` interface identifier.
   *
   * @see voIDocumentFindOccurrenceIterator
   */
  VO_IDocumentFindOccurrenceIterator,

  /**
   * `voIObjectIterator` interface identifier.
   *
   * @see voIObjectIterator
   */
  VO_IObjectIterator,

  /**
   * `voIExtentProvider` interface identifier.
   *
   * @see voIExtentProvider
   */
  VO_IExtentProvider,

  /**
   * `voIGuideIterator` interface identifier.
   *
   * @see voIGuideIterator
   */
  VO_IGuideIterator,

  /**
   * `voIContentProcessor` interface identifier.
   *
   * @see voIContentProcessor
   * @since 7.0_0
   */
  VO_IContentProcessor,

  /**
   * `voIContentProcessorListener` interface identifier.
   *
   * @see voIContentProcessorListener
   * @since 7.0_0
   */
  VO_IContentProcessorListener,

  /**
   * `voIPageChangeSet` interface identifier.
   *
   * @see voIPageChangeSet
   * @since 7.0_0
   */
  VO_IPageChangeSet,

  /**
   * `voITextInspector` interface identifier.
   *
   * @see voITextInspector
   * @since 7.0_0
   */
  VO_ITextInspector,

  /**
   * `voITextInterval` interface identifier.
   *
   * @see voITextInterval
   * @since 7.0_0
   */
  VO_ITextInterval,

  /**
   * `voITypesetData` interface identifier.
   *
   * @see voITypesetData
   * @since 7.0_0
   */
  VO_ITypesetData,

  /**
   * `voITypesetItem` interface identifier.
   *
   * @see voITypesetItem
   * @since 7.0_0
   */
  VO_ITypesetItem,

  /**
   * `voIFontIterator` interface identifier.
   *
   * @see voIFontIterator
   * @since 7.0_0
   */
  VO_IFontIterator,
};


// -----------------------------------------------------------------------------

/**
 * `voDocument` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voIDocument, voIVersionable}
 */
typedef voEngineObject voDocument;

/**
 * `voPage` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voIPage, voIVersionable}
 */
typedef voEngineObject voPage;

/**
 * `voContent` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voIContent}
 */
typedef voEngineObject voContent;

/**
 * `voContentField` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voIContentField}
 */
typedef voEngineObject voContentField;

/**
 * `voContentProcessor` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voIContentProcessor}
 * @since 7.0_0
 */
typedef voEngineObject voContentProcessor;

/**
 * `voTypesetData` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voITypesetData}
 * @since 7.0_0
 */
typedef voEngineObject voTypesetData;

/**
 * `voLayout` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voILayout, voIExtentProvider}
 */
typedef voEngineObject voLayout;


// -- Object Iterator ----------------------------------------------------------

/**
 * `voObjectIterator` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voIObjectIterator}
 */
typedef voEngineObject voObjectIterator;

/**
 * Functions composing the `voIObjectIterator` interface.
 *
 * This interface exposes the public methods of the `voObjectIterator` type.
 *
 * @note As the C language does not provide any mechanism that could be used to
 *   simulate inheritance, inherited members are simply copied from the base
 *   structure.
 *
 * {@extends voIIterator}
 * {@implementingTypes voObjectIterator}
 */
typedef struct _voIObjectIterator
{
  /** {@inheritDoc} */
  voYesNo (VO_MSE_CALL *isAtEnd)(voEngine engine, voIterator target);
  /** {@inheritDoc} */
  bool (VO_MSE_CALL *next)(voEngine engine, voIterator target);
  /** {@inheritDoc} */
  voYesNo (VO_MSE_CALL *equals)(voEngine engine, voIterator target, voIterator other);

  /**
   * Returns the URL of the object at the current step of the iteration.
   *
   * @param engine the engine.
   * @param target the target iterator.
   * @param charset the charset used to encode parameter strings.
   * @param url the recipient for the URL value.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voObjectIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `url` or its `bytes` member is not
   *   writable.
   */
  bool (VO_MSE_CALL *getUrl)(voEngine engine, voObjectIterator target, voCharset charset, voString* url);
}
voIObjectIterator;


// -- Asynchronous Command Handler ---------------------------------------------

/**
 * DEPRECATED.
 *
 * Lists the asynchronous commands of `voDocument` and `voPage`.
 *
 * @see voDocumentAsyncCommandCompletionCallback
 *
 * @par deprecated as of 8.0.0
 */
typedef enum _voDocumentAsyncCommand
{
  /**
   * Indicates completion of `voDocument.save()` or `voPage.save()`.
   */
  VO_CMD_SAVE,

  /**
   * Indicates completion of `voDocument.saveAs()`.
   */
  VO_CMD_SAVE_AS,

  /**
   * Indicates completion of `voPage.saveToTemp()`.
   */
  VO_CMD_SAVE_TO_TEMP,

  /**
   * Indicates completion of `voDocument.discardChanges()` or
   * `voPage.discardChanges()`.
   */
  VO_CMD_DISCARD_CHANGES,

  /**
   * Indicates completion of `voDocument.getMetadata()` or
   * `voPage.getMetadata()`.
   */
  VO_CMD_GET_METADATA,

  /**
   * Indicates completion of `voDocument.setMetadata()` or
   * `voPage.setMetadata()`.
   */
  VO_CMD_SET_METADATA,

  /**
   * Indicates completion of `voPage.getInk()`.
   */
  VO_CMD_GET_INK,

  /**
   * Indicates completion of `voPage.getLayout()`.
   */
  VO_CMD_GET_LAYOUT,

  /**
   * Indicates completion of `voPage.getContent()`.
   */
  VO_CMD_GET_CONTENT,

  /**
   * Indicates completion of `voPage.paste()`.
   */
  VO_CMD_PASTE,

  /**
   * Indicates completion of `voDocument.addObject()`.
   */
  VO_CMD_ADD_OBJECT,

  /**
   * Indicates completion of `voDocument.extractObjectTo()`.
   */
  VO_CMD_EXTRACT_OBJECT_TO,

  /**
   * Indicates completion of `voDocument.importPage()`.
   */
  VO_CMD_IMPORT_PAGE,

  _voDocumentAsyncCommand_FORCE_INT_SIZE = 0x7FFFFFFF
}
voDocumentAsyncCommand;


/**
 * DEPRECATED.
 *
 * Contains the result of asynchronous commands of `voDocument` and `voPage`.
 *
 * @see voDocumentAsyncCommandCompletionCallback
 *
 * @par deprecated as of 8.0.0
 */
typedef union _voDocumentAsyncCommandResult
{
  voEngineObject object;
  struct {
    voJson metadata;
  } getMetadata;
  struct {
    voInk ink;
  } getInk;
  struct {
    voLayout layout;
  } getLayout;
  struct {
    voContent content;
  } getContent;
  struct {
    voPage page;
  } importPage;
  struct {
    void* reserved;
  } other;
}
voDocumentAsyncCommandResult;

/**
 * DEPRECATED.
 *
 * Callback called by the document I/O background thread upon completion of
 * asynchronous commands of `voDocument` and `voPage`.
 *
 * The objects provided in `result` are valid only during the callback
 * execution, therefore you must use `voAcquireObject()` if you want to retain
 * the object for later use.
 *
 * Do not call sync() from this callback, or this will result in a deadlock.
 *
 * Do not call other asynchronous functions from this callback (functions
 * that call a callback upon completion), prefer calling asynchronous functions
 * in sequence.
 *
 * @param engine the engine.
 * @param cmd the command that just completed.
 * @param success `true` when the command was successful, otherwise `false`.
 * @param result the result of the command.
 * @param userParam an optional user defined parameter passed to the callback.
 *
 * @see voIDocument, voIPage
 *
 * @par deprecated as of 8.0.0
 */
typedef void (VO_MSE_CALL *voDocumentAsyncCommandCompletionCallback)(voEngine engine, voDocumentAsyncCommand cmd, bool success, voDocumentAsyncCommandResult result, void* userParam);


// -- Document -----------------------------------------------------------------

/**
 * {@propertySet Document}
 */
enum VO_DOCUMENT_PROP
{
  /**
   * Specifies if the document uses the auto save feature.
   *
   * This value must be `false` (disabled) or `true` (enabled).
   *
   * {@property bool}
   */
  VO_DOCUMENT_AUTO_SAVE = (VO_Document << 16),
};

/**
 * Holds the parameters required to create a new `voDocument` instance.
 *
 * @see voDocument, voCreateDocument
 */
typedef struct _voDocumentInitializer
{
  /**
   * The charset used to encode pathnames.
   */
  voCharset charset;

  /**
   * The pathname of the document.
   */
  voString fileName;

  /**
   * The pathname of the temporary directory to use.
   * When empty, temporary directories and files may be created next to the
   * document file.
   *
   * Temporary directories and files are removed upon saving or discarding of
   * the document's modifications (call to `save()` or `saveAs()` or
   * `discardChanges()`).
   *
   */
  voString temporaryDirectoryName;

  /**
   * When `false`, document creation will fail when `fileName` points to a non
   * existing directory. When `true`, `fileName` parent will be created if it
   * does not exist.
   */
  bool createDirectory;

  /**
   * When `true`, a new document will be created, whether `fileName` points to
   * an existing document or not.
   */
  bool overwrite;
}
voDocumentInitializer;

/**
 * Functions composing the `voIDocument` interface.
 *
 * This interface exposes the public methods of the `voDocument` type.
 *
 * {@implementingTypes voDocument}
 */
typedef struct _voIDocument
{
  /**
   * Sets the temporary directory to be used by the document.
   *
   * If a temporary directory has already been created, i.e. if `saveToTemp()`
   * has been called, this operation is not allowed anymore, you must call
   * `save()` or `discardChanges()` first.
   *
   * If the specified temporary directory does not exist, it will be created.
   *
   * @param engine the engine.
   * @param target the target document.
   * @param charset the charset used to encode parameter strings.
   * @param directoryName the name of the temporary directory.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voDocument`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `directoryName` or its `bytes` member
   *   is not readable.
   * @throws VO_INVALID_STATE when the temporary directory has already been
   *   created.
   */
  bool (VO_MSE_CALL *setTemporaryDirectory)(voEngine engine, voDocument target, voCharset charset, const voString* directoryName);

  /**
   * Tells whether the document has been modified since the last save operation.
   *
   * @param engine the engine.
   * @param target the target document.
   *
   * @return `VO_YES` or `VO_NO` on success, otherwise `VO_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voDocument`.
   */
  voYesNo (VO_MSE_CALL *isModified)(voEngine engine, voDocument target);

  /**
   * DEPRECATED call: use `save2()` instead.
   *
   * Requests the saving of all ongoing modifications made to the document.
   *
   * @note Completion and success of this operation is indicated by calling the
   *   provided callback.
   *
   * @param engine the engine.
   * @param target the target document.
   * @param callback the function that will be called upon completion of the
   *   operation.
   * @param userParam an optional pointer to a user defined parameter that will
   *   be passed back to the `callback` function.
   *
   * @return `true` when the request was processed successfully, otherwise
   *   `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voDocument`.
   * @throws VO_INVALID_STATE when the document file name has not been set.
   *
   * @par deprecated as of 8.0.0
   */
  bool (VO_MSE_CALL *save)(voEngine engine, voDocument target, voDocumentAsyncCommandCompletionCallback callback, void* userParam);

  /**
   * DEPRECATED call: use `saveAs2()` instead.
   *
   * Requests the saving of all ongoing modifications made to the document, to
   * a new file.
   *
   * @note Completion and success of this operation is indicated by calling the
   *   provided callback.
   *
   * @param engine the engine.
   * @param target the target document.
   * @param charset the charset used to encode parameter strings.
   * @param fileName the new document name.
   * @param callback the function that will be called upon completion of the
   *   operation.
   * @param userParam an optional pointer to a user defined parameter that will
   *   be passed back to the `callback` function.
   *
   * @return `true` when the request was processed successfully, otherwise
   *   `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voDocument`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `fileName` or its `bytes` member is not
   *   readable.
   *
   * @par deprecated as of 8.0.0
   */
  bool (VO_MSE_CALL *saveAs)(voEngine engine, voDocument target, voCharset charset, const voString* fileName, voDocumentAsyncCommandCompletionCallback callback, void* userParam);

  /**
   * DEPRECATED call: use `saveToTemp2()` instead.
   *
   * Requests the saving of all ongoing modifications made to the document, to
   * a temporary folder.
   *
   * @note Completion and success of this operation is indicated by calling the
   *   provided callback.
   *
   * @param engine the engine.
   * @param target the target document.
   * @param callback the function that will be called upon completion of the
   *   operation.
   * @param userParam an optional pointer to a user defined parameter that will
   *   be passed back to the `callback` function.
   *
   * @return `true` when the request was processed successfully, otherwise
   *   `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voDocument`.
   * @throws VO_INVALID_STATE when no temporary directory has been set.
   *
   * @par deprecated as of 8.0.0
   */
  bool (VO_MSE_CALL *saveToTemp)(voEngine engine, voDocument target, voDocumentAsyncCommandCompletionCallback callback, void* userParam);

  /**
   * DEPRECATED call: use `discardChanges2()` instead.
   *
   * Requests discarding all changes that have been made to the document but not
   * saved yet.
   *
   * This will remove the temporary directory associated with the document and
   * all unsaved content will be lost.
   *
   * @note Completion and success of this operation is indicated by calling the
   *   provided callback.
   *
   * @param engine the engine.
   * @param target the target document.
   * @param callback the function that will be called upon completion of the
   *   operation.
   * @param userParam an optional pointer to a user defined parameter that will
   *   be passed back to the `callback` function.
   *
   * @return `true` when the request was processed successfully, otherwise
   *   `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voDocument`.
   *
   * @par deprecated as of 8.0.0
   */
  bool (VO_MSE_CALL *discardChanges)(voEngine engine, voDocument target, voDocumentAsyncCommandCompletionCallback callback, void* userParam);

  /**
   * DEPRECATED call: use `getMetadata2()` instead.
   *
   * Requests the metadata object associated with this document.
   *
   * @note Completion and success of this operation is indicated by calling the
   *   provided callback.
   *
   * @param engine the engine.
   * @param target the target document.
   * @param callback the function that will be called upon completion of the
   *   operation.
   * @param userParam an optional pointer to a user defined parameter that will
   *   be passed back to the `callback` function.
   *
   * @return `true` when the request was processed successfully, otherwise
   *   `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voDocument`.
   *
   * @par deprecated as of 8.0.0
   */
  bool (VO_MSE_CALL *getMetadata)(voEngine engine, voDocument target, voDocumentAsyncCommandCompletionCallback callback, void* userParam);

  /**
   * DEPRECATED call: use `setMetadata2()` instead.
   *
   * Requests setting the metadata object associated with this document.
   *
   * @note Completion and success of this operation is indicated by calling the
   *   provided callback.
   *
   * @param engine the engine.
   * @param target the target document.
   * @param metadata the metadata to be set.
   * @param callback the function that will be called upon completion of the
   *   operation.
   * @param userParam an optional pointer to a user defined parameter that will
   *   be passed back to the `callback` function.
   *
   * @return `true` when the request was processed successfully, otherwise
   *   `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voDocument`.
   * @throws VO_NO_SUCH_OBJECT when the `metadata` reference is invalid.
   * @throws VO_INVALID_OBJECT when `metadata` is not a `voJson`.
   *
   * @par deprecated as of 8.0.0
   */
  bool (VO_MSE_CALL *setMetadata)(voEngine engine, voDocument target, voJson metadata, voDocumentAsyncCommandCompletionCallback callback, void* userParam);

  /**
   * Returns the number of pages in this document.
   *
   * @param engine the engine.
   * @param target the target document.
   *
   * @return the number of pages on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voDocument`.
   */
  int32_t (VO_MSE_CALL *getPageCount)(voEngine engine, voDocument target);

  /**
   * Returns the page at the specified index.
   *
   * @param engine the engine.
   * @param target the target document.
   * @param index the index of the requested page.
   *
   * @return the page on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voDocument`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_LIMIT_EXCEEDED when the number of `voPage` instances would
   *   exceed the allowed limit (please remember that the limit can be shared by
   *   several types).
   */
  voPage (VO_MSE_CALL *getPageAt)(voEngine engine, voDocument target, int32_t index);

  /**
   * Returns the specified page.
   *
   * @param engine the engine.
   * @param target the target document.
   * @param charset the charset used to encode parameter strings.
   * @param id the identifier of the page to be retrieved.
   *
   * @return the page on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voDocument`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `id` or its `bytes` member is not
   *   readable.
   * @throws VO_INVALID_ARGUMENT when `id` is invalid.
   * @throws VO_LIMIT_EXCEEDED when the number of `voPage` instances would
   *   exceed the allowed limit (please remember that the limit can be shared by
   *   several types).
   */
  voPage (VO_MSE_CALL *getPage)(voEngine engine, voDocument target, voCharset charset, const voString* id);

  /**
   * Returns the index of a page within this document.
   *
   * @param engine the engine.
   * @param target the target document.
   * @param page the page.
   *
   * @return the index of the page on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voDocument`.
   * @throws VO_NO_SUCH_OBJECT when the `page` reference is invalid.
   * @throws VO_INVALID_OBJECT when `page` is not a `voPage`.
   * @throws VO_INVALID_ARGUMENT when `page` does not belong to `target`.
   */
  int32_t (VO_MSE_CALL *indexOfPage)(voEngine engine, voDocument target, voPage page);

  /**
   * Adds a page to the document.
   *
   * @param engine the engine.
   * @param target the target document.
   *
   * @return the new page on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voDocument`.
   * @throws VO_LIMIT_EXCEEDED when the number of `voPage` instances would
   *   exceed the allowed limit (please remember that the limit can be shared by
   *   several types).
   */
  voPage (VO_MSE_CALL *addPage)(voEngine engine, voDocument target);

  /**
   * Adds a page to the document and associates it to the specified ink.
   *
   * @param engine the engine.
   * @param target the target document.
   * @param ink the ink that the new page will contain.
   *
   * @return the new page on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voDocument`.
   * @throws VO_NO_SUCH_OBJECT when the `ink` reference is invalid.
   * @throws VO_INVALID_OBJECT when `ink` is not a `voInk`.
   * @throws VO_INVALID_ARGUMENT when the `ink` is already in use in another
   *   page.
   * @throws VO_LIMIT_EXCEEDED when the number of `voPage` instances would
   *   exceed the allowed limit (please remember that the limit can be shared by
   *   several types).
   */
  voPage (VO_MSE_CALL *addPageWithInk)(voEngine engine, voDocument target, voInk ink);

  /**
   * Removes a page from the document.
   *
   * @param engine the engine.
   * @param target the target document.
   * @param index the index of the page to be removed.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voDocument`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   */
  bool (VO_MSE_CALL *removePage)(voEngine engine, voDocument target, int32_t index);

  /**
   * Moves a page within the document.
   *
   * @param engine the engine.
   * @param target the target document.
   * @param fromIndex the current index of the page to be moved.
   * @param toIndex the new index of the page to be moved.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voDocument`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `fromIndex` is out of bounds.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `toIndex` is out of bounds.
   */
  bool (VO_MSE_CALL *movePage)(voEngine engine, voDocument target, int32_t fromIndex, int32_t toIndex);

  /**
   * Tells whether the document contains a given object.
   *
   * @param engine the engine.
   * @param target the target document.
   * @param charset the charset used to encode parameter strings.
   * @param url the URL of the object within the document.
   *
   * @return `VO_YES` or `VO_NO` on success, otherwise `VO_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voDocument`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `url` or its `bytes` member is not
   *   readable.
   */
  voYesNo (VO_MSE_CALL *containsObject)(voEngine engine, voDocument target, voCharset charset, const voString* url);

  /**
   * Removes an object from the document.
   *
   * The object will not be removed from the document file immediately, to allow
   * undo/redo of this action.
   *
   * However, when the document is released, this object will not be accessible
   * anymore, and the next time the document is loaded, the object will be
   * actually removed.
   *
   * @param engine the engine.
   * @param target the target document.
   * @param charset the charset used to encode parameter strings.
   * @param url the URL of the object within the document.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voDocument`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `url` or its `bytes` member is not
   *   readable.
   */
  bool (VO_MSE_CALL *removeObject)(voEngine engine, voDocument target, voCharset charset, const voString* url);

  /**
   * DEPRECATED call: use `addObjects2()` instead.
   *
   * Requests adding an object file to the document and associates it with a
   * URL.
   *
   * This object can then be referenced by its URL.
   *
   * @note Completion and success of this operation is indicated by calling the
   *   provided callback.
   *
   * @param engine the engine.
   * @param target the target document.
   * @param charset the charset used to encode parameter strings.
   * @param url the URL to be associated with the object.
   * @param fileName the path to the object file to be added to the document.
   * @param callback the function that will be called upon completion of the
   *   operation.
   * @param userParam an optional pointer to a user defined parameter that will
   *   be passed back to the `callback` function.
   *
   * @return `true` when the request was processed successfully, otherwise
   *   `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voDocument`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `url` or its `bytes` member is not
   *   readable.
   * @throws VO_INVALID_USER_BUFFER when `fileName` or its `bytes` member is not
   *   readable.
   * @throws VO_INVALID_STATE when the temporary directory is not set or cannot
   *   be inferred from the document path (the document has never been saved).
   *
   * @par deprecated as of 8.0.0
   */
  bool (VO_MSE_CALL *addObject)(voEngine engine, voDocument target, voCharset charset, const voString* url, const voString* fileName, voDocumentAsyncCommandCompletionCallback callback, void* userParam);

  /**
   * DEPRECATED call: use `extractObjectTo2()` instead.
   *
   * Requests extracting an object from the document to a given path.
   *
   * @note Completion and success of this operation is indicated by calling the
   *   provided callback.
   *
   * The function fails immediately (the callback is not called) if the object
   * is not part of the document.
   *
   * @param engine the engine.
   * @param target the target document.
   * @param charset the charset used to encode parameter strings.
   * @param url the URL of the object within the document.
   * @param fileName the path to extract the object file to.
   * @param callback the function that will be called upon completion of the
   *   operation.
   * @param userParam an optional pointer to a user defined parameter that will
   *   be passed back to the `callback` function.
   *
   * @return `true` when the request was processed successfully, otherwise
   *   `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voDocument`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `url` or its `bytes` member is not
   *   readable.
   * @throws VO_INVALID_USER_BUFFER when `fileName` or its `bytes` member is not
   *   readable.
   * @throws VO_INVALID_ARGUMENT when the `url` does not reference any object in
   *   the target document.
   *
   * @par deprecated as of 8.0.0
   */
  bool (VO_MSE_CALL *extractObjectTo)(voEngine engine, voDocument target, voCharset charset, const voString* url, const voString* fileName, voDocumentAsyncCommandCompletionCallback callback, void* userParam);

  /**
   * DEPRECATED voIDocument no longer implements asynchronous actions.
   *
   * Waits until all pending actions are done.
   *
   * Use this function to wait until `save()` or `saveAs()` requests have
   * completed.
   *
   * @param engine the engine.
   * @param target the target document.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voDocument`.
   *
   * @par deprecated as of 8.0.0
   */
  bool (VO_MSE_CALL *sync)(voEngine engine, voDocument target);

  /**
   * Returns an iterator over the objects of the document.
   *
   * When the document is modified (by adding or removing objects), the iterator
   * is invalidated.
   *
   * @param engine the engine.
   * @param target the target document.
   *
   * @return an object iterator on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voDocument`.
   * @throws VO_LIMIT_EXCEEDED when the number of `voObjectIterator` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   */
   voObjectIterator (VO_MSE_CALL *getObjects)(voEngine engine, voDocument target);

  /**
   * DEPRECATED call: use `importPage2()` instead.
   *
   * Requests adding an existing page to this document.
   *
   * Page modifications that have not been saved (to temporary directory or to
   * an actual document file) will not be imported into `target`.
   *
   * Successful operation completion is indicated by calling the callback
   * provided as parameter, and the imported page instance is provided in
   * parameter of that callback.
   *
   * If an imported page references layout objects, then target document must be
   * associated with a temporary directory, otherwise import will fail and
   * the callback will be called with a `VO_INVALID_STATE` error.
   *
   * @param engine the engine.
   * @param target the target document.
   * @param page the page to import into target.
   * @param callback the function that will be called upon completion of the
   *   operation.
   * @param userParam an optional pointer to a user defined parameter that will
   *   be passed back to the `callback` function.
   *
   * @return `true` when the request was processed successfully, otherwise
   *   `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voDocument`.
   * @throws VO_NO_SUCH_OBJECT when the `page` reference is invalid.
   * @throws VO_INVALID_OBJECT when `page` is not a `voPage`.
   *
   * @par deprecated as of 8.0.0
   */
  bool (VO_MSE_CALL *importPage)(voEngine engine, voDocument target, voPage page, voDocumentAsyncCommandCompletionCallback callback, void* userParam);
}
voIDocument;


#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voDocument` instance.
 *
 * @param engine the engine.
 *
 * @return the newly created `voDocument` instance on success, otherwise `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_LIMIT_EXCEEDED when the number of `voDocument` instances would
 *   exceed the allowed limit (please remember that the limit can be shared by
 *   several types).
 */
VO_INLINE voDocument voCreateDocument(voEngine engine)
{
  return voCreateObject(engine, VO_Document);
}

/**
 * Creates a new `voDocument` instance.
 *
 * @param engine the engine.
 * @param charset the charset used to encode parameter strings.
 * @param fileName the pathname of the document.
 * @param temporaryDirectoryName the pathname of the temporary directory to use.
 *   If empty, a directory name will be set to `fileName + "-files"`.
 * @param createDirectory `true` when parent directory of `fileName` must be
 *   created if it does not exist, otherwise `false`.
 * @param overwrite `true` when a new document must be created even if
 *   `fileName` points to an existing document, otherwise `false`.
 *
 * @return the newly created `voDocument` instance on success, otherwise `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
 * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
 *   default charset was set.
 * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
 * @throws VO_INVALID_USER_BUFFER when `fileName` or its `bytes` member is not
 *   readable.
 * @throws VO_INVALID_USER_BUFFER when `temporaryDirectoryName` or its `bytes`
 *   member is not readable.
 * @throws VO_IO_FAILURE when `fileName` cannot be created or read.
 * @throws VO_LIMIT_EXCEEDED when the number of `voDocument` instances would
 *   exceed the allowed limit (please remember that the limit can be shared by
 *   several types).
 */
VO_INLINE voDocument voCreateDocumentEx2( voEngine engine,
                                          voCharset charset,
                                          const voString* fileName,
                                          const voString* temporaryDirectoryName,
                                          bool createDirectory,
                                          bool overwrite)
{
  voDocumentInitializer initializer = { charset, { 0 } , { 0 } , createDirectory, overwrite };
  if (fileName)
    initializer.fileName = *fileName;
  if (temporaryDirectoryName)
    initializer.temporaryDirectoryName = *temporaryDirectoryName;
  return voCreateObjectEx(engine, VO_Document, &initializer, sizeof(initializer));
}

/**
 * Creates a new `voDocument` instance.
 *
 * @param engine the engine.
 * @param charset the charset used to encode parameter strings.
 * @param fileName the pathname of the document.
 * @param temporaryDirectoryName the pathname of the temporary directory to use.
 *   If empty, a directory name will be set to `fileName + "-files"`.
 *
 * @return the newly created `voDocument` instance on success, otherwise `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
 * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
 *   default charset was set.
 * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
 * @throws VO_INVALID_USER_BUFFER when `fileName` or its `bytes` member is not
 *   readable.
 * @throws VO_INVALID_USER_BUFFER when `temporaryDirectoryName` or its `bytes`
 *   member is not readable.
 * @throws VO_IO_FAILURE when `fileName` cannot be created or read.
 * @throws VO_LIMIT_EXCEEDED when the number of `voDocument` instances would
 *   exceed the allowed limit (please remember that the limit can be shared by
 *   several types).
 */
VO_INLINE voDocument voCreateDocumentEx(voEngine engine,
                                        voCharset charset,
                                        const voString* fileName,
                                        const voString* temporaryDirectoryName)
{
  return voCreateDocumentEx2(engine, charset, fileName, temporaryDirectoryName, false, false);
}
#endif // end of: #ifndef VO_NO_FUNCTIONS


// -- Page ---------------------------------------------------------------------

/**
 * `voPageSelection` reference type.
 *
 * @note While `voPageSelection` implements the `voILayout` interface, you are
 *   only allowed to inspect the content of a page selection. You are not
 *   allowed to modify the layout via a page selection.
 *
 * {@extends voEngineObject}
 * {@implements voIPageSelection, voIInkStrokeSet, voIInkSelection, voISelection, voILayout, voIExtentProvider}
 */
typedef voEngineObject voPageSelection;

/**
 * `voLayerIterator` reference type.
 *
 * {@extends voIterator}
 * {@implements voILayerIterator, voIExtentProvider}
 */
typedef voIterator voLayerIterator;

/**
 * `voPenIterator` reference type.
 *
 * {@extends voIterator}
 * {@implements voIPenIterator}
 */
typedef voIterator voPenIterator;

/**
 * `voFontIterator` reference type.
 *
 * {@extends voIterator}
 * {@implements voIFontIterator}
 */
typedef voIterator voFontIterator;

/**
 * `voLayoutItemIterator` reference type.
 *
 * {@extends voIterator}
 * {@implements voILayoutItemIterator, voIExtentProvider}
 */
typedef voIterator voLayoutItemIterator;

/**
 * Functions composing the `voIPage` interface.
 *
 * This interface exposes the public methods of the `voPage` type.
 *
 * {@implementingTypes voPage}
 */
typedef struct _voIPage
{
  /**
   * Returns the document that contains this page.
   *
   * @param engine the engine.
   * @param target the target page.
   *
   * @return the document on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voPage`.
   * @throws VO_LIMIT_EXCEEDED when the number of `voDocument` instances would
   *   exceed the allowed limit (please remember that the limit can be shared by
   *   several types).
   */
  voDocument (VO_MSE_CALL *getDocument)(voEngine engine, voPage target);

  /**
   * Returns the identifier of this page.
   *
   * @param engine the engine.
   * @param target the target page.
   * @param charset the charset used to encode parameter strings.
   * @param id the recipient for the identifier.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voPage`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `id` or its `bytes` member is not
   *   writable.
   */
  bool (VO_MSE_CALL *getId)(voEngine engine, voPage target, voCharset charset, voString* id);

  /**
   * Adds the specified listener to this page.
   *
   * @param engine the engine.
   * @param target the target page.
   * @param listener the listener to be added.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voPage`.
   * @throws VO_NO_SUCH_OBJECT when the `listener` reference is invalid.
   * @throws VO_INVALID_OBJECT when `listener` does not implement the
   *   `voIPageUpdateListener` interface.
   * @throws VO_INVALID_STATE when `listener` is already registered.
   */
  bool (VO_MSE_CALL *addListener)(voEngine engine, voPage target, voUserObject listener);

  /**
   * Removes the specified listener from this page.
   *
   * @param engine the engine.
   * @param target the target page.
   * @param listener the listener to be removed.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voPage`.
   * @throws VO_NO_SUCH_OBJECT when the `listener` reference is invalid.
   * @throws VO_INVALID_OBJECT when `listener` does not implement the
   *   `voIPageUpdateListener` interface.
   * @throws VO_INVALID_STATE when `listener` is not registered.
   */
  bool (VO_MSE_CALL *removeListener)(voEngine engine, voPage target, voUserObject listener);

  /**
   * Tells whether the page has been modified since the last save operation.
   *
   * @param engine the engine.
   * @param target the target page.
   *
   * @return `VO_YES` or `VO_NO` on success, otherwise `VO_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voPage`.
   */
  voYesNo (VO_MSE_CALL *isModified)(voEngine engine, voPage target);

  /**
   * DEPRECATED call: use `voIDocument.save2()` instead.
   *
   * Requests the saving of all ongoing modifications made to the page.
   *
   * @note Completion and success of this operation is indicated by calling the
   *   provided callback.
   *
   * @param engine the engine.
   * @param target the target page.
   * @param callback the function that will be called upon completion of the
   *   operation.
   * @param userParam an optional pointer to a user defined parameter that will
   *   be passed back to the `callback` function.
   *
   * @return `true` when the request was processed successfully, otherwise
   *   `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voPage`.
   *
   * @par deprecated as of 7.0.0
   */
  bool (VO_MSE_CALL *save)(voEngine engine, voPage target, voDocumentAsyncCommandCompletionCallback callback, void* userParam);

  /**
   * DEPRECATED call: use `voIDocument.saveToTemp2()` instead.
   *
   * Requests the saving of all ongoing modifications made to the page, to a
   * temporary folder.
   *
   * @note Completion and success of this operation is indicated by calling the
   *   provided callback.
   *
   * @param engine the engine.
   * @param target the target page.
   * @param callback the function that will be called upon completion of the
   *   operation.
   * @param userParam an optional pointer to a user defined parameter that will
   *   be passed back to the `callback` function.
   *
   * @return `true` when the request was processed successfully, otherwise
   *   `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voPage`.
   *
   * @par deprecated as of 7.0.0
   */
  bool (VO_MSE_CALL *saveToTemp)(voEngine engine, voPage target, voDocumentAsyncCommandCompletionCallback callback, void* userParam);

  /**
   * DEPRECATED call: use `discardChanges2()` instead.
   *
   * Requests discarding all changes that have been made to the document but not
   * saved yet. Changes saved to the temporary folder will be discarded too.
   *
   * @note Completion and success of this operation is indicated by calling the
   *   provided callback.
   *
   * @param engine the engine.
   * @param target the target page.
   * @param callback the function that will be called upon completion of the
   *   operation.
   * @param userParam an optional pointer to a user defined parameter that will
   *   be passed back to the `callback` function.
   *
   * @return `true` when the request was processed successfully, otherwise
   *   `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voPage`.
   *
   * @par deprecated as of 8.0.0
   */
  bool (VO_MSE_CALL *discardChanges)(voEngine engine, voPage target, voDocumentAsyncCommandCompletionCallback callback, void* userParam);

  /**
   * DEPRECATED call: use `getMetadata2()` instead.
   *
   * Requests the metadata object associated with this page.
   *
   * @note Completion and success of this operation is indicated by calling the
   *   provided callback.
   *
   * @param engine the engine.
   * @param target the target page.
   * @param callback the function that will be called upon completion of the
   *   operation.
   * @param userParam an optional pointer to a user defined parameter that will
   *   be passed back to the `callback` function.
   *
   * @return `true` when the request was processed successfully, otherwise
   *   `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voPage`.
   *
   * @par deprecated as of 8.0.0
   */
  bool (VO_MSE_CALL *getMetadata)(voEngine engine, voPage target, voDocumentAsyncCommandCompletionCallback callback, void* userParam);

  /**
   * DEPRECATED call: use `setMetadata2()` instead.
   *
   * Requests setting the metadata object associated with this page.
   *
   * @note Completion and success of this operation is indicated by calling the
   *   provided callback.
   *
   * @param engine the engine.
   * @param target the target page.
   * @param metadata the metadata to be set.
   * @param callback the function that will be called upon completion of the
   *   operation.
   * @param userParam an optional pointer to a user defined parameter that will
   *   be passed back to the `callback` function.
   *
   * @return `true` when the request was processed successfully, otherwise
   *   `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voPage`.
   * @throws VO_NO_SUCH_OBJECT when the `metadata` reference is invalid.
   * @throws VO_INVALID_OBJECT when `metadata` is not a `voJson`.
   *
   * @par deprecated as of 8.0.0
   */
  bool (VO_MSE_CALL *setMetadata)(voEngine engine, voPage target, voJson metadata, voDocumentAsyncCommandCompletionCallback callback, void* userParam);

  /**
   * DEPRECATED call: use `getInk2()` instead.
   *
   * Requests the ink object associated with this page.
   *
   * @note Completion and success of this operation is indicated by calling the
   *   provided callback.
   *
   * @param engine the engine.
   * @param target the target page.
   * @param callback the function that will be called upon completion of the
   *   operation.
   * @param userParam an optional pointer to a user defined parameter that will
   *   be passed back to the `callback` function.
   *
   * @return `true` when the request was processed successfully, otherwise
   *   `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voPage`.
   *
   * @par deprecated as of 8.0.0
   */
  bool (VO_MSE_CALL *getInk)(voEngine engine, voPage target, voDocumentAsyncCommandCompletionCallback callback, void* userParam);

  /**
   * DEPRECATED call: use `getLayout2()` instead.
   *
   * Requests the layout of this page.
   *
   * @note Completion and success of this operation is indicated by calling the
   *   provided callback.
   *
   * @param engine the engine.
   * @param target the target page.
   * @param callback the function that will be called upon completion of the
   *   operation.
   * @param userParam an optional pointer to a user defined parameter that will
   *   be passed back to the `callback` function.
   *
   * @return `true` when the request was processed successfully, otherwise
   *   `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voPage`.
   *
   * @par deprecated as of 8.0.0
   */
  bool (VO_MSE_CALL *getLayout)(voEngine engine, voPage target, voDocumentAsyncCommandCompletionCallback callback, void* userParam);

  /**
   * DEPRECATED call: use `getContent2()` instead.
   *
   * Requests the content of this page.
   *
   * @note Completion and success of this operation is indicated by calling the
   *   provided callback.
   *
   * @param engine the engine.
   * @param target the target page.
   * @param callback the function that will be called upon completion of the
   *   operation.
   * @param userParam an optional pointer to a user defined parameter that will
   *   be passed back to the `callback` function.
   *
   * @return `true` when the request was processed successfully, otherwise
   *   `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voPage`.
   *
   * @par deprecated as of 8.0.0
   */
  bool (VO_MSE_CALL *getContent)(voEngine engine, voPage target, voDocumentAsyncCommandCompletionCallback callback, void* userParam);

  /**
   * Erases the selected items from this page.
   *
   * @param engine the engine.
   * @param target the target page.
   * @param selection the selection to erase.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voPage`.
   * @throws VO_NO_SUCH_OBJECT when the `selection` reference is invalid.
   * @throws VO_INVALID_OBJECT when `selection` is not a `voPageSelection`.
   * @throws VO_INVALID_ARGUMENT when `selection` was not created from `target`.
   */
  bool (VO_MSE_CALL *erase)(voEngine engine, voPage target, voPageSelection selection);

  /**
   * Transforms the selected items in this page.
   *
   * @param engine the engine.
   * @param target the target page.
   * @param selection the selection to be transformed.
   * @param transform the affine transform to be applied.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voPage`.
   * @throws VO_NO_SUCH_OBJECT when the `selection` reference is invalid.
   * @throws VO_INVALID_OBJECT when `selection` is not a `voPageSelection`.
   * @throws VO_INVALID_ARGUMENT when `selection` was not created from `target`.
   * @throws VO_INVALID_USER_BUFFER when `transform` is not readable.
   * @throws VO_INVALID_ARGUMENT when `transform` contains invalid values or is
   *   not invertible.
   */
  bool (VO_MSE_CALL *transform)(voEngine engine, voPage target, voPageSelection selection, const voTransform* transform);

  /**
   * DEPRECATED call: use `paste2()` instead.
   *
   * Pastes the selected items into this page. The selection may have been
   * created on another page, or another document.
   *
   * @note Completion and success of this operation is indicated by calling the
   *   provided callback.
   *
   * @param engine the engine.
   * @param target the target page.
   * @param selection the selection to be pasted.
   * @param callback the function that will be called upon completion of the
   *   operation.
   * @param userParam an optional pointer to a user defined parameter that will
   *   be passed back to the `callback` function.
   *
   * @return `true` when the request was processed successfully, otherwise
   *   `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voPage`.
   * @throws VO_NO_SUCH_OBJECT when the `selection` reference is invalid.
   * @throws VO_INVALID_OBJECT when `selection` is not a `voPageSelection`.
   *
   * @par deprecated as of 8.0.0
   */
  bool (VO_MSE_CALL *paste)(voEngine engine, voPage target, voPageSelection selection, voDocumentAsyncCommandCompletionCallback callback, void* userParam);

  /**
   * Brings the selected items to the front of their group or layer. Items will
   * stay in the same layer group.
   *
   * The selection must contain items that all belong to the same group (or the
   * same layer if no group is defined).
   *
   * @param engine the engine.
   * @param target the target page.
   * @param selection the selection of items to be moved.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voPage`.
   * @throws VO_NO_SUCH_OBJECT when the `selection` reference is invalid.
   * @throws VO_INVALID_OBJECT when `selection` is not a `voPageSelection`.
   * @throws VO_INVALID_ARGUMENT when `selection` was not created from `target`.
   * @throws VO_INVALID_ARGUMENT when `selection` contains items from different
   *   groups or layers.
   */
  bool (VO_MSE_CALL *bringToFront)(voEngine engine, voPage target, voPageSelection selection);

  /**
   * Pushes the selected items to the back of their layer or group. Items will
   * stay in the same group.
   *
   * The selection must contain items that all belong to the same group (or the
   * same layer if no group is defined).
   *
   * @param engine the engine.
   * @param target the target page.
   * @param selection the selection of items to be moved.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voPage`.
   * @throws VO_NO_SUCH_OBJECT when the `selection` reference is invalid.
   * @throws VO_INVALID_OBJECT when `selection` is not a `voPageSelection`.
   * @throws VO_INVALID_ARGUMENT when `selection` was not created from `target`.
   * @throws VO_INVALID_ARGUMENT when `selection` contains items from different
   *   groups or layers.
   */
  bool (VO_MSE_CALL *pushToBack)(voEngine engine, voPage target, voPageSelection selection);

  /**
   * Moves the selected items to the front of the given layer.
   *
   * Groups are not moved from a layer to another, only content of the groups
   * is moved.
   *
   * @param engine the engine.
   * @param target the target page.
   * @param selection the selection of items to be moved.
   * @param layer the destination layer.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voPage`.
   * @throws VO_NO_SUCH_OBJECT when the `selection` reference is invalid.
   * @throws VO_INVALID_OBJECT when `selection` is not a `voPageSelection`.
   * @throws VO_INVALID_ARGUMENT when `selection` was not created from `target`.
   * @throws VO_NO_SUCH_OBJECT when the `layer` reference is invalid.
   * @throws VO_INVALID_OBJECT when `layer` is not a `voLayerIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `layer` is positioned at the end of the
   *   iteration.
   */
  bool (VO_MSE_CALL *moveToLayer)(voEngine engine, voPage target, voPageSelection selection, voLayerIterator layer);

  /**
   * DEPRECATED voIPage no longer implements asynchronous actions.
   *
   * Waits until all pending actions are done. Use this function to wait until
   * functions that use callbacks are done.
   *
   * @param engine the engine.
   * @param target the target page.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voPage`.
   *
   * @par deprecated as of 8.0.0
   */
  bool (VO_MSE_CALL *sync)(voEngine engine, voPage target);

  /**
   * Tells whether an object is associated with a ink portion. When it's the
   * case, the ink portion can be retrieved by calling
   * `getInkSegmentForObject()`.
   *
   * @param engine the engine.
   * @param target the target page.
   * @param object an object to check for ink reference.
   *
   * @return `VO_YES` or `VO_NO` on success, otherwise `VO_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voPage`.
   * @throws VO_NO_SUCH_OBJECT when the `object` reference is invalid.
   */
  voYesNo (VO_MSE_CALL *isObjectAssociatedWithInkSegment)(voEngine engine, voPage target, voEngineObject object);

  /**
   * Returns the ink segment associated with the specified object.
   *
   * It is advised to call `isObjectAssociatedWithInkSegment()` before calling
   * this function to determine whether object is associated with an ink
   * portion.
   *
   * @param engine the engine.
   * @param target the target page.
   * @param object an object that is associated with an ink portion.
   *
   * @return the ink segment corresponding to `object` on success, otherwise
   *   `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voPage`.
   * @throws VO_NO_SUCH_OBJECT when the `object` reference is invalid.
   * @throws VO_INVALID_OBJECT when `object` is not associated with an ink
   *   portion.
   * @throws VO_INVALID_STATE when the ink portion associated with `object` has
   *   been erased.
   * @throws VO_LIMIT_EXCEEDED when the number of `voInkSegment` instances would
   *   exceed the allowed limit (please remember that the limit can be shared by
   *   several types).
   *
   * @see isObjectAssociatedWithInkSegment
   */
  voInkSegment (VO_MSE_CALL *getInkSegmentForObject)(voEngine engine, voPage target, voEngineObject object);

  /**
   * Tells whether an object is associated with a ink portion that is tagged.
   *
   * When it is, the ink tag id can be retrieved with `getInkTagIdForObject()`.
   *
   * @param engine the engine.
   * @param target the target page.
   * @param object an object to check for tagged ink reference.
   *
   * @return `VO_YES` or `VO_NO` on success, otherwise `VO_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voPage`.
   * @throws VO_NO_SUCH_OBJECT when the `object` reference is invalid.
   */
  voYesNo (VO_MSE_CALL *isObjectAssociatedWithInkTagId)(voEngine engine, voPage target, voEngineObject object);

  /**
   * Returns the ink tag id associated with the specified object.
   *
   * It is advised to call `isObjectAssociatedWithInkTagId()` before calling
   * this  function to determine whether the object is associated with a tagged
   * ink portion.
   *
   * @param engine the engine.
   * @param target the target page.
   * @param object an object that is associated with a tagged ink portion.
   *
   * @return the identifier of the ink tag corresponding to `object` on success,
   *   otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voPage`.
   * @throws VO_NO_SUCH_OBJECT when the `object` reference is invalid.
   * @throws VO_INVALID_OBJECT when `object` is not associated with an ink
   *   portion.
   * @throws VO_INVALID_STATE when the ink portion associated with `object` has
   *   been erased.
   *
   * @see isObjectAssociatedWithInkTagId
   */
  int64_t (VO_MSE_CALL *getInkTagIdForObject)(voEngine engine, voPage target, voEngineObject object);

  /**
   * Returns the typeset data corresponding to the selected area.
   *
   * This typeset data can then be modified and finally applied to the document.
   *
   * @param engine the engine.
   * @param target the target page.
   * @param field the associated content field.
   * @param selection the portion of the page to generate typeset from, either
   *   as a `voPageSelection` or as a `voInkSelection`.
   * @param contentProcessor an optional content processor used to run
   *   recognition on `field`.
   *
   * @return a typeset data describing the outcome of the typeset operation,
   *   otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voPage`.
   * @throws VO_NO_SUCH_OBJECT when the `field` reference is invalid.
   * @throws VO_INVALID_OBJECT when `field` is not a `voContentField`.
   * @throws VO_NO_SUCH_OBJECT when the `selection` reference is invalid.
   * @throws VO_INVALID_OBJECT when `selection` is not a `voPageSelection`
   *   or a `voInkSelection`.
   * @throws VO_NO_SUCH_OBJECT when the `contentProcessor` reference is invalid.
   * @throws VO_INVALID_OBJECT when `contentProcessor` is not a
   *   `voContentProcessor`.
   * @throws VO_INVALID_STATE when the content of `selection` has been modified
   *   since recognition results in `field` were updated.
   * @throws VO_LIMIT_EXCEEDED when the number of `voTypesetData` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   *
   * @since 7.0_0
   */
  voTypesetData (VO_MSE_CALL *getTypesetData)(voEngine engine, voPage target, voContentField field, voEngineObject selection, voContentProcessor contentProcessor);
}
voIPage;


/**
 * Holds the parameters required to create a new `voPageSelection` instance.
 *
 * @see voPageSelection, voCreatePageSelection
 */
typedef struct _voPageSelectionInitializer
{
  /**
   * The target page.
   */
  voPage page;
}
voPageSelectionInitializer;

#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voPageSelection` instance.
 *
 * @param engine the engine.
 * @param page the associated page.
 *
 * @return the newly created `voPageSelection` instance on success, otherwise
 *   `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_NO_SUCH_OBJECT when the `page` reference is invalid.
 * @throws VO_INVALID_OBJECT when `page` is not a `voPage`.
 * @throws VO_LIMIT_EXCEEDED when the number of `voPageSelection` instances
 *   would exceed the allowed limit (please remember that the limit can be
 *   shared by several types).
 *
 * @note A page selection needs the ink and layout of the page to function.
 *   Therefore, this call may block for the time required to load ink and layout
 *   if they are not already loaded.
 */
VO_INLINE voPageSelection voCreatePageSelection(voEngine engine, voPage page)
{
  voPageSelectionInitializer initializer = { page };
  return voCreateObjectEx(engine, VO_PageSelection, &initializer, sizeof(initializer));
}
#endif // end of: #ifndef VO_NO_FUNCTIONS

/**
 * Functions composing the `voIPageSelection` interface.
 *
 * This interface exposes the public methods of the `voPageSelection` type.
 *
 * {@implementingTypes voPageSelection}
 */
typedef struct _voIPageSelection
{
  /**
   * Returns the page on which this page selection is based.
   *
   * @param engine the engine.
   * @param target the target page selection.
   *
   * @return the page on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voPageSelection`.
   * @throws VO_LIMIT_EXCEEDED when the number of `voPage` instances would
   *   exceed the allowed limit (please remember that the limit can be shared by
   *   several types).
   */
  voPage (VO_MSE_CALL *getPage)(voEngine engine, voPageSelection target);

  /**
   * Selects the top most item that intersects the specified circle.
   *
   * @param engine the engine.
   * @param target the target page selection.
   * @param circle the circle to select.
   * @param layer an iterator positioned on the layer on which the selection
   *   should be performed, or `NULL` for all layers.
   * @param modifier the way the new selection should be combined with the
   *   existing selection.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voPageSelection`.
   * @throws VO_NO_SUCH_OBJECT when the `layer` reference is invalid.
   * @throws VO_INVALID_OBJECT when `layer` is not a `voLayerIterator`.
   * @throws VO_INVALID_ARGUMENT when `layer` comes from a different page.
   * @throws VO_NO_SUCH_ELEMENT when `layer` is positioned at the end of the
   *   iteration.
   * @throws VO_INVALID_ARGUMENT when `circle` is invalid.
   * @throws VO_INVALID_ARGUMENT when `modifier` is invalid.
   */
  bool (VO_MSE_CALL *hit)(voEngine engine, voPageSelection target, const voCircle* circle, voLayerIterator layer, voSelectionModifier modifier);

  /**
   * Selects the specified layer.
   *
   * @param engine the engine.
   * @param target the target page selection.
   * @param layer an iterator positioned on the layer to be selected.
   * @param modifier the way the new selection should be combined with the
   *   existing selection.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voPageSelection`.
   * @throws VO_NO_SUCH_OBJECT when the `layer` reference is invalid.
   * @throws VO_INVALID_OBJECT when `layer` is not a `voLayerIterator`.
   * @throws VO_INVALID_ARGUMENT when `layer` comes from a different page.
   * @throws VO_NO_SUCH_ELEMENT when `layer` is positioned at the end of the
   *   iteration.
   * @throws VO_INVALID_ARGUMENT when `modifier` is invalid.
   */
  bool (VO_MSE_CALL *selectLayer)(voEngine engine, voPageSelection target, voLayerIterator layer, voSelectionModifier modifier);

  /**
   * Selects the specified item.
   *
   * If the item belongs to a group, the entire group will be selected.
   *
   * @param engine the engine.
   * @param target the target page selection.
   * @param item an iterator positioned on the layout item to be selected.
   * @param modifier the way the new selection should be combined with the
   *   existing selection.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voPageSelection`.
   * @throws VO_NO_SUCH_OBJECT when the `item` reference is invalid.
   * @throws VO_INVALID_OBJECT when `item` is not a `voLayoutItemIterator`.
   * @throws VO_INVALID_ARGUMENT when `item` comes from a different page.
   * @throws VO_NO_SUCH_ELEMENT when `item` is positioned at the end of the
   *   iteration.
   * @throws VO_INVALID_ARGUMENT when `circle` is invalid.
   * @throws VO_INVALID_ARGUMENT when `modifier` is invalid.
   */
  bool (VO_MSE_CALL *selectItem)(voEngine engine, voPageSelection target, voLayoutItemIterator item, voSelectionModifier modifier);

  /**
   * Returns the ink selection corresponding to this page selection.
   *
   * @param engine the engine.
   * @param target the target page selection.
   *
   * @return the ink selection on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voPageSelection`.
   * @throws VO_LIMIT_EXCEEDED when the number of `voInkSelection` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   */
  voInkSelection (VO_MSE_CALL *getInkSelection)(voEngine engine, voPageSelection target);
}
voIPageSelection;


/**
 * Functions composing the `voIExtentProvider` interface.
 *
 * {@implementingTypes voLayout, voLayerIterator, voLayoutItemIterator, voPageSelection}
 */
typedef struct _voIExtentProvider
{
  /**
   * Returns the extent associated with the target object.
   *
   * @param engine the engine.
   * @param target the target layout-related object.
   * @param extent the recipient for the extent.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_INVALID_USER_BUFFER when `extent` is not writable.
   */
  bool (VO_MSE_CALL *getExtent)(voEngine engine, voEngineObject target, voExtent* extent);
}
voIExtentProvider;


// -- Guides -------------------------------------------------------------------

/**
 * Lists the types of guides.
 *
 * @see voContent
 */
typedef enum _voGuideType
{
  VO_HORIZONTAL_LINE_SET, /**< The type of a horizontal line set guide. */
  VO_VERTICAL_LINE_SET,   /**< The type of a vertical line set guide. */

  _voGuideType_FORCE_INT_SIZE = 0x7FFFFFFF
}
voGuideType;

/**
 * Lists the purposes of guides.
 *
 * @see voContent
 */
typedef enum _voGuidePurpose
{
  /**
   * A guide that has no specific semantics for recognizers.
   */
  VO_GUIDE_NONE,

  /**
   * An horizontal line set that defines baselines for text recognizers.
   */
  VO_GUIDE_TEXT_BASELINE,

  /**
   * An horizontal line set that defines midlines for text recognizers.
   */
  VO_GUIDE_TEXT_MIDLINE,

  /**
   * An horizontal line set that defines ascenders for text recognizers.
   */
  VO_GUIDE_TEXT_ASCENDER,

  /**
   * An horizontal line set that defines descenders for text recognizers.
   */
  VO_GUIDE_TEXT_DESCENDER,

  /**
   * An horizontal line set that defines a staff for music recognizers.
   */
  VO_GUIDE_MUSIC_STAFF,

  _voGuidePurpose_FORCE_INT_SIZE = 0x7FFFFFFF
}
voGuidePurpose;

/**
 * Represents the properties of a content guide.
 *
 * @see voContent
 */
typedef struct _voGuideData
{
  /**
   * The type of the guide.
   */
  voGuideType type;

  /**
   * The purpose of the guide.
   */
  voGuidePurpose purpose;

  union {
    /**
     * The properties of the guide when `type == VO_HORIZONTAL_LINE_SET`.
     */
    voHorizontalLineSet horizontalLineSet;

    /**
     * The properties of the guide when `type == VO_VERTICAL_LINE_SET`.
     */
    voVerticalLineSet verticalLineSet;
  } data;
}
voGuideData;

/**
 * `voGuideIterator` reference type.
 *
 * {@extends voIterator}
 * {@implements voIGuideIterator}
 */
typedef voIterator voGuideIterator;

/**
 * Functions composing the `voIGuideIterator` interface.
 *
 * This interface exposes the public methods of the `voGuideIterator` type.
 *
 * @note As the C language does not provide any mechanism that could be used to
 *   simulate inheritance, inherited members are simply copied from the base
 *   structure.
 *
 * {@extends voIIterator}
 * {@implementingTypes voGuideIterator}
 */
typedef struct _voIGuideIterator
{
  /** {@inheritDoc} */
  voYesNo (VO_MSE_CALL *isAtEnd)(voEngine engine, voIterator target);
  /** {@inheritDoc} */
  bool (VO_MSE_CALL *next)(voEngine engine, voIterator target);
  /** {@inheritDoc} */
  voYesNo (VO_MSE_CALL *equals)(voEngine engine, voIterator target, voIterator other);

  /**
   * Returns the identifier of the guide at the current step of the iteration.
   *
   * @param engine the engine.
   * @param target the target iterator.
   * @param charset the charset used to encode parameter strings.
   * @param id the recipient for the identifier.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voGuideIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `id` or its `bytes` member is not
   *   writable.
   */
  bool (VO_MSE_CALL *getId)(voEngine engine, voGuideIterator target, voCharset charset, voString* id);

  /**
   * Returns the properties of the guide at the current step of the iteration.
   *
   * @param engine the engine.
   * @param target the target iterator.
   * @param data the recipient for the guide properties.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voGuideIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   * @throws VO_INVALID_USER_BUFFER when `data` is not writable.
   */
  bool (VO_MSE_CALL *getData)(voEngine engine, voGuideIterator target, voGuideData* data);

  /**
   * Removes the guide at the current step of the iteration.
   *
   * The iterator advances to the next iteration.
   *
   * When iterating over content guides, the references in fields and layout
   * are also removed.
   *
   * @param engine the engine.
   * @param target the target iterator.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voGuideIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   */
  bool (VO_MSE_CALL *remove)(voEngine engine, voGuideIterator target);
}
voIGuideIterator;


// -- Content ------------------------------------------------------------------

/**
 * Lists the types of content fields.
 *
 * @see voContent
 */
typedef enum _voContentFieldType
{
  VO_CONTENT_FIELD_ERR = -1,  /**< Error marker. */

  VO_CONTENT_FIELD_TEXT,      /**< The type of a text field. */
  VO_CONTENT_FIELD_SHAPE,     /**< The type of a shape field. */
  VO_CONTENT_FIELD_MATH,      /**< The type of a math field. */
  VO_CONTENT_FIELD_ANALYZER,  /**< The type of an analyzer field. */
  VO_CONTENT_FIELD_MUSIC,     /**< The type of a music field. */
  VO_CONTENT_FIELD_DRAWING,   /**< The type of a drawing field. */

  _voContentFieldType_FORCE_INT_SIZE = 0x7FFFFFFF
}
voContentFieldType;

/**
 * Functions composing the `voIContent` interface.
 *
 * This interface exposes the public methods of the `Content` type.
 *
 * {@implementingTypes voContent}
 */
typedef struct _voIContent
{
  /**
   * Creates a field of the specified type.
   *
   * @param engine the engine.
   * @param target the target content.
   * @param type the type of the field.
   * @param charset the charset used to encode parameter strings.
   * @param id the field identifier.
   *
   * @return the newly created `voContentField` instance on success, otherwise
   *   `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voContent`.
   * @throws VO_INVALID_ARGUMENT when `type` is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `id` or its `bytes` member is not
   *   readable.
   * @throws VO_INVALID_ARGUMENT when a field with same `id` already exists.
   * @throws VO_LIMIT_EXCEEDED when the number of `voContentField` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   */
  voContentField (VO_MSE_CALL *addNewField)(voEngine engine, voContent target, voContentFieldType type, voCharset charset, const voString* id);

  /**
   * Returns the specified field.
   *
   * @param engine the engine.
   * @param target the target content.
   * @param charset the charset used to encode parameter strings.
   * @param id the field identifier.
   *
   * @return the field on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voContent`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `id` or its `bytes` member is not
   *   readable.
   * @throws VO_INVALID_ARGUMENT when no field corresponding to `id` was found.
   * @throws VO_LIMIT_EXCEEDED when the number of `voContentField` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   */
  voContentField (VO_MSE_CALL *getField)(voEngine engine, voContent target, voCharset charset, const voString* id);

  /**
   * Removes the specified field.
   *
   * @param engine the engine.
   * @param target the target content.
   * @param charset the charset used to encode parameter strings.
   * @param id the field identifier.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voContent`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `id` or its `bytes` member is not
   *   readable.
   * @throws VO_INVALID_ARGUMENT when no field corresponding to `id` was found.
   */
  bool (VO_MSE_CALL *removeField)(voEngine engine, voContent target, voCharset charset, const voString* id);

  /**
   * Returns the number of fields of the specified type.
   *
   * @param engine the engine.
   * @param target the target content.
   * @param type the type of field.
   *
   * @return the number of fields on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voContent`.
   * @throws VO_INVALID_ARGUMENT when `type` is invalid.
   */
  int32_t (VO_MSE_CALL *getFieldCount)(voEngine engine, voContent target, voContentFieldType type);

  /**
   * Returns the field at the specified index from the list of fields of the
   * specified type.
   *
   * @param engine the engine.
   * @param target the target content.
   * @param type the type of field.
   * @param index the index of the requested field.
   *
   * @return the field on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voContent`.
   * @throws VO_INVALID_ARGUMENT when `type` is invalid.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_LIMIT_EXCEEDED when the number of `voContentField` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   */
  voContentField (VO_MSE_CALL *getFieldAt)(voEngine engine, voContent target, voContentFieldType type, int32_t index);

  /**
   * Adds a new guide to this content.
   *
   * @param engine the engine.
   * @param target the target content.
   * @param charset the charset used to encode parameter strings.
   * @param id the guide identifier.
   * @param data the data associated with the guide.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voContent`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `id` or its `bytes` member is not
   *   readable.
   * @throws VO_INVALID_USER_BUFFER when `data` is not readable.
   * @throws VO_INVALID_ARGUMENT when `data` contains invalid parametrization.
   * @throws VO_INVALID_STATE when a guide with this `id` already exists.
   */
  bool (VO_MSE_CALL *addGuide)(voEngine engine, voContent target, voCharset charset, const voString* id, const voGuideData* data);

  /**
   * Returns an iterator that points only to the guide specified by `id`.
   *
   * When no guide corresponds to `id`, an empty iterator is returned.
   *
   * @param engine the engine.
   * @param target the target content.
   * @param charset the charset used to encode parameter strings.
   * @param id the identifier of the guide to be retrieved.
   *
   * @return a guide iterator on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voContent`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `id` or its `bytes` member is not
   *   readable.
   * @throws VO_LIMIT_EXCEEDED when the number of `voGuideIterator` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   */
  voGuideIterator (VO_MSE_CALL *getGuide)(voEngine engine, voContent target, voCharset charset, const voString* id);

  /**
   * Returns an iterator over the guides of this content.
   *
   * @param engine the engine.
   * @param target the target content.
   *
   * @return a guide iterator on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voContent`.
   * @throws VO_LIMIT_EXCEEDED when the number of `voGuideIterator` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   */
  voGuideIterator (VO_MSE_CALL *getGuides)(voEngine engine, voContent target);
}
voIContent;

/**
 * Functions composing the `voIContentField` interface.
 *
 * {@implementingTypes voContentField}
 */
typedef struct _voIContentField
{
  /**
   * Returns the type of this content field.
   *
   * @param engine the engine.
   * @param target the target content field.
   *
   * @return the type on success, otherwise `VO_CONTENT_FIELD_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voContentField`.
   */
  voContentFieldType (VO_MSE_CALL *getType)(voEngine engine, voContentField target);

  /**
   * Returns the identifier of this content field.
   *
   * @param engine the engine.
   * @param target the target content field.
   * @param charset the charset used to encode parameter strings.
   * @param id the recipient for the identifier.
   *
   * @return `true`on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voContentField`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `id` or its `bytes` member is not
   *   writable.
   */
  bool (VO_MSE_CALL *getId)(voEngine engine, voContentField target, voCharset charset, voString* id);

  /**
   * Clears this content field.
   *
   * @param engine the engine.
   * @param target the target content field.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voContentField`.
   */
  bool (VO_MSE_CALL *clear)(voEngine engine, voContentField target);

  /**
   * Adds the ink described by the given intervals or page selection to this
   * content field.
   *
   * @param engine the engine.
   * @param target the target content field.
   * @param items the ink to be added to this field, as a `voInkIntervals`
   *   (e.g. `voInkSelection` or `voInkSegment`) or as a `voPageSelection`.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voContentField`.
   * @throws VO_NO_SUCH_OBJECT when the `items` reference is invalid.
   * @throws VO_INVALID_OBJECT when `items` is not a `voInkIntervals` or a
   *   `voPageSelection`.
   */
  bool (VO_MSE_CALL *addItems)(voEngine engine, voContentField target, voEngineObject items);

  /**
   * Removes the ink described by the given intervals or page selection from
   * this content field.
   *
   * @param engine the engine.
   * @param target the target content field.
   * @param items the ink to be added to this field, as a `voInkIntervals`
   *   (e.g. `voInkSelection` or `voInkSegment`) or as a `voPageSelection`.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voContentField`.
   * @throws VO_NO_SUCH_OBJECT when the `items` reference is invalid.
   * @throws VO_INVALID_OBJECT when `items` is not a `voInkIntervals` or a
   *   `voPageSelection`.
   */
  bool (VO_MSE_CALL *removeItems)(voEngine engine, voContentField target, voEngineObject items);

  /**
   * Returns the recognition result associated with the content field.
   *
   * @param engine engine the engine.
   * @param target the target content field.
   *
   * @return a recognition result object on success if any, otherwise `NULL`.
   *   When `NULL`, use `voGetError()` to figure out whether an error occurred.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voContentField`.
   * @throws VO_INVALID_STATE when `target` is not in a state where the result
   *   is available.
   * @throws VO_LIMIT_EXCEEDED when the number of instances of the specified
   *   type would exceed the allowed limit (please remember that the limit can
   *   be shared by several types).
   */
  voEngineObject (VO_MSE_CALL *getResult)(voEngine engine, voContentField target);

  /**
   * Adds a reference to a content guide.
   *
   * @param engine the engine.
   * @param target the target content field.
   * @param charset the charset used to encode parameter strings.
   * @param id the guide identifier.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voContentField`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `id` or its `bytes` member is not
   *   readable.
   * @throws VO_INVALID_ARGUMENT when no guide corresponding to `id` was found
   *   in the content.
   * @throws VO_INVALID_STATE when a guide with this `id` is already referenced.
   */
  bool (VO_MSE_CALL *addGuide)(voEngine engine, voContentField target, voCharset charset, const voString* id);

  /**
   * Returns an iterator that points only to the guide specified by `id`.
   *
   * When no guide corresponds to `id`, an empty iterator is returned.
   *
   * @param engine the engine.
   * @param target the target content field.
   * @param charset the charset used to encode parameter strings.
   * @param id the identifier of the guide to be retrieved.
   *
   * @return a guide iterator on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voContentField`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `id` or its `bytes` member is not
   *   readable.
   * @throws VO_LIMIT_EXCEEDED when the number of `voGuideIterator` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   */
  voGuideIterator (VO_MSE_CALL *getGuide)(voEngine engine, voContentField target, voCharset charset, const voString* id);

  /**
   * Returns an iterator over the guides of this content field.
   *
   * @param engine the engine.
   * @param target the target content field.
   *
   * @return a guide iterator on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voContentField`.
   * @throws VO_LIMIT_EXCEEDED when the number of `voGuideIterator` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   */
  voGuideIterator (VO_MSE_CALL *getGuides)(voEngine engine, voContentField target);

  /**
   * Configures the configurations bundle, the configuration name and the
   * configuration script of this content field.
   *
   * These configuration strings may be set to `NULL`, as long as the following
   * rules are followed:
   * <ul>
   * <li>If `bundleName` is `NULL`, then `configurationName` must be `NULL` and
   * `configurationScript` must not be `NULL`.</li>
   * <li>If `configurationName` is `NULL`, then `bundleName` must be `NULL` and
   * `configurationScript` must not be `NULL`.</li>
   * <li>If `configurationScript` is `NULL`, then `bundleName` and
   * `configurationName` must not be `NULL`.</li>
   * </ul>
   *
   * @param engine the engine.
   * @param target the target content field.
   * @param charset the charset used to encode parameter strings.
   * @param bundleName the name of the configurations bundle, or `NULL`.
   * @param configurationName the name of the configuration, or `NULL`.
   * @param configurationScript the configuration script, or `NULL`.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voContentField`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `bundleName` or its `bytes` member is
   *   not readable.
   * @throws VO_INVALID_USER_BUFFER when `configurationName` or its `bytes`
   *   member is not readable.
   * @throws VO_INVALID_USER_BUFFER when `configurationScript` or its `bytes`
   *   member is not readable.
   * @throws VO_INVALID_ARGUMENT when the combination of `bundleName`,
   *   `configurationName` and `configurationScript` is not allowed.
   *
   * @since 7.0_0
   */
  bool (VO_MSE_CALL *setConfiguration)(voEngine engine, voContentField target,
                                       voCharset charset, const voString* bundleName,
                                       const voString* configurationName,
                                       const voString* configurationScript);

  /**
   * Gets the configurations bundle, the configuration name and the
   * configuration script from this content field.
   *
   * @param engine the engine.
   * @param target the target content field.
   * @param charset the charset used to encode parameter strings.
   * @param bundleName the name of the configurations bundle, or `NULL`.
   * @param configurationName the name of the configuration, or `NULL`.
   * @param configurationScript the configuration script, or `NULL`.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voContentField`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `bundleName` or its `bytes` member is
   *   not writable.
   * @throws VO_INVALID_USER_BUFFER when `configurationName` or its `bytes`
   *   member is not writable.
   * @throws VO_INVALID_USER_BUFFER when `configurationScript` or its `bytes`
   *   member is not writable.
   *
   * @since 7.0_0
   */
  bool (VO_MSE_CALL *getConfiguration)(voEngine engine, voContentField target,
                                       voCharset charset, voString* bundleName,
                                       voString* configurationName,
                                       voString* configurationScript);
}
voIContentField;

/**
 * Functions composing the `voIContentFieldProcessor` interface.
 */
typedef struct _voIContentFieldProcessor
{
  /**
   * Processes a content field.
   *
   * @param engine the engine.
   * @param target the target processor.
   * @param contentField the content field to be processed.
   * @param notifyProgressFunc an optional pointer to a `voNotifyProgressFunc`
   *   function.
   * @param userParam an optional pointer to a user defined parameter that will
   *   be passed back to `notifyProgressFunc`.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voContentField`.
   * @throws VO_NO_SUCH_OBJECT when the `contentField` reference is invalid.
   * @throws VO_INVALID_OBJECT when `contentField` is not a `voContentField`.
   * @throws VO_INVALID_CALLBACK when `notifyProgressFunc` is invalid.
   * @throws VO_INVALID_STATE when the `target` object is not in a state where
   *   the `process` function can be invoked.
   * @throws VO_CANCELED when the operation has been canceled.
   */
  bool (VO_MSE_CALL *process)(voEngine engine, voEngineObject target, voContentField contentField, voNotifyProgressFunc notifyProgressFunc, void* userParam);
}
voIContentFieldProcessor;


// -- ContentProcessor ---------------------------------------------------------

#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voContentProcessor` instance.
 *
 * @param engine the engine.
 *
 * @return the newly created `voContentProcessor` instance on success, otherwise
 *   `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_LIMIT_EXCEEDED when the number of `voContentProcessor` instances
 *   would exceed the allowed limit (please remember that the limit can be
 *   shared by several types).
 *
 * @since 7.0_0
 */
VO_INLINE voContentProcessor voCreateContentProcessor(voEngine engine)
{
  return voCreateObject(engine, VO_ContentProcessor);
}
#endif // end of: #ifndef VO_NO_FUNCTIONS

/**
 * Represents the type of action requested to the content processor when
 * processing a content field.
 *
 * @since 7.0_0
 */
typedef enum _voContentProcessorAction
{
  /**
   * Request a recognition of the content field.
   */
  VO_ACTION_RECOGNIZE,

  /**
   * Request a recognition followed by a beautification of the content field.
   * Action ignored if the type of the field is not `VO_CONTENT_FIELD_SHAPE`.
   */
  VO_ACTION_RECOGNIZE_THEN_BEAUTIFY,

  /**
   * Request an indexation of the content field. Action ignored if the type of
   * the field is not `VO_CONTENT_FIELD_TEXT`.
   */
  VO_ACTION_INDEX,

  /**
   * Request a recognition and an indexation of the content field. Action
   * ignored if the type of the field is not `VO_CONTENT_FIELD_TEXT`.
   */
  VO_ACTION_RECOGNIZE_AND_INDEX,

  _voContentProcessorAction_FORCE_INT_SIZE = 0x7FFFFFFF
}
voContentProcessorAction;

/**
 * Functions composing the `voIContentProcessor` interface.
 *
 * This interface exposes the public methods of the `voContentProcessor` type.
 *
 * {@implementingTypes voContentProcessor}
 * @since 7.0_0
 */
typedef struct _voIContentProcessor
{
  /**
   * Adds the specified listener to this content processor.
   *
   * @param engine the engine.
   * @param target the target content processor.
   * @param listener the change listener to be added.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voContentProcessor`.
   * @throws VO_NO_SUCH_OBJECT when the `listener` reference is invalid.
   * @throws VO_INVALID_OBJECT when `listener` does not implement the
   *   `voIContentProcessorListener` interface.
   * @throws VO_INVALID_STATE when `listener` is already registered.
   */
  bool (VO_MSE_CALL *addListener)(voEngine engine, voContentProcessor target, voUserObject listener);

  /**
   * Removes the specified listener from this content processor.
   *
   * @param engine the engine.
   * @param target the target content processor.
   * @param listener the listener to be removed.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voContentProcessor`.
   * @throws VO_NO_SUCH_OBJECT when the `listener` reference is invalid.
   * @throws VO_INVALID_OBJECT when `listener` does not implement the
   *   `voIContentProcessorListener` interface.
   * @throws VO_INVALID_STATE when `listener` is not registered.
   */
  bool (VO_MSE_CALL *removeListener)(voEngine engine, voContentProcessor target, voUserObject listener);

  /**
   * Returns the configuration manager associated with this content processor.
   *
   * @param engine the engine.
   * @param target the target content processor.
   *
   * @return the configuration manager on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voContentProcessor`.
   * @throws VO_LIMIT_EXCEEDED when the number of `voConfigurationManager`
   *   instances would exceed the allowed limit (please remember that the limit
   *   can be shared by several types).
   */
  voConfigurationManager (VO_MSE_CALL *getConfigurationManager)(voEngine engine, voContentProcessor target);

  /**
   * Processes the specified content field.
   *
   * @param engine the engine.
   * @param target the target content processor.
   * @param field the content field to be processed.
   * @param action the action to be performed.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voContentProcessor`.
   * @throws VO_INVALID_ARGUMENT when `action` is invalid.
   */
  bool (VO_MSE_CALL *process)(voEngine engine, voContentProcessor target, voContentField field, voContentProcessorAction action);

  /**
   * Waits until all pending actions are done.
   *
   * @param engine the engine.
   * @param target the target content processor.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voContentProcessor`.
   */
  bool (VO_MSE_CALL *sync)(voEngine engine, voContentProcessor target);

  /**
   * Cancels all pending `process()` requests.
   *
   * @param engine the engine.
   * @param target the target content processor.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voContentProcessor`.
   */
  bool (VO_MSE_CALL *cancel)(voEngine engine, voContentProcessor target);
}
voIContentProcessor;

/**
 * `voPageChangeSet` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voIPageChangeSet}
 *
 * @since 7.0_0
 */
typedef voEngineObject voPageChangeSet;

/**
 * Lists the possible return values of `voPageChangeSet::apply()` calls.
 *
 * @see voIPageChangeSet
 * @since 7.0_0
 */
typedef enum _voPageChangeSetResult
{
  /**
   * Indicates that an error occurred. The patch could not be applied.
   */
  VO_PAGE_CHANGE_SET_ERR = -1,

  /**
   * Indicates that the patch could be applied successfully.
   */
  VO_PAGE_CHANGE_SET_APPLIED,

  /**
   * Indicates that the patch could not be entirely applied, because some
   * changes on the page prevent it. Call `process()` again to ensure all
   * modifications have been taken into account.
   */
  VO_PAGE_CHANGE_SET_PROCESS_REQUIRED,

  /**
   * Indicates that the patch could not be entirely applied and that a new patch
   * will be provided automatically by the content processor.
   */
  VO_PAGE_CHANGE_SET_NEW_PATCH_PENDING,

  _voPageChangeSetResult_FORCE_INT_SIZE = 0x7FFFFFFF
}
voPageChangeSetResult;

/**
 * Functions composing the `voIPageChangeSet` interface.
 *
 * This interface exposes the public methods of the `voPageChangeSet` type.
 *
 * {@implementingTypes voPageChangeSet}
 * @since 7.0_0
 */
typedef struct _voIPageChangeSet
{
  /**
   * Applies the modifications described in this change set.
   *
   * By default, this operation is included in a history manager "ghost
   * transaction", i.e. if `undo()` is called on the document (or page)
   * history manager right after a call to `apply()`, `apply()` will be undone
   * together with the previous action.
   *
   * To make the history manager consider this operation as a regular
   * operation, include it inside a history manager transaction by using
   * `startTransaction()` and `commit()`.
   *
   * @note This function should not be called from the `onResultAvailable()`
   *   content processor callback, in order to avoid concurrent access to the
   *   document.
   *
   * @param engine the engine.
   * @param target the target change set.
   *
   * @return the outcome of patch application on success, otherwise
   *   `VO_PAGE_CHANGE_SET_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voPageChangeSet`.
   * @throws VO_NOT_PERMITTED_FROM_THIS_THREAD when the `apply()` call was made
   *   from within a `voContentProcessor` callback.
   * @throws VO_INVALID_STATE when the change set can't be applied because it
   *   applies to an item that has been removed in the meantime.
   */
  voPageChangeSetResult (VO_MSE_CALL *apply)(voEngine engine, voPageChangeSet target);
}
voIPageChangeSet;

/**
 * Functions composing the `voIContentProcessorListener` interface.
 *
 * @note This interface is implementable by user objects. User code is not
 *   allowed to invoke this interface directly: as a consequence
 *   `voGetInterface()` returns `NULL`.
 *
 * @since 7.0_0
 */
typedef struct _voIContentProcessorListener
{
  /**
   * Called when the target listener is added to a content processor object.
   *
   * @param engine the engine.
   * @param target the target listener.
   * @param processor the content processor to which the listener was added.
   */
  void (VO_MSE_CALL *added)(voEngine engine, voUserObject target, voContentProcessor processor);

  /**
   * Called when the target listener is removed from a content processor object.
   *
   * @param engine the engine.
   * @param target the target listener.
   * @param processor the content processor from which the listener was removed.
   */
  void (VO_MSE_CALL *removed)(voEngine engine, voUserObject target, voContentProcessor processor);

  /**
   * Called when a content field processor has been created.
   *
   * @param engine the engine.
   * @param target the target listener.
   * @param field the content field that will be processed by `processor`.
   * @param fieldProcessor the created content field processor on success,
   *   otherwise `NULL`.
   */
  void (VO_MSE_CALL *onCreated)(voEngine engine, voUserObject target, voContentField field, voEngineObject fieldProcessor);

  /**
   * Called when a content field processor has been configured. The application
   * may configure the content field further.
   *
   * @param engine the engine.
   * @param target the target listener.
   * @param field the content field which processor has been configured.
   * @param configurer the configurer in use.
   *
   * @return `false` if an error occurred during further configuration,
   *   otherwise `true`.
   */
  bool (VO_MSE_CALL *onConfigured)(voEngine engine, voUserObject target, voContentField field, voConfigurer configurer);

  /**
   * Called multiple times during process to indicate progression.
   *
   * @param engine the engine.
   * @param target the target listener.
   * @param field the content field which is being processed.
   * @param progress the progression rate between `0` and `1`.
   */
  void (VO_MSE_CALL *onProgress)(voEngine engine, voUserObject target, voContentField field, float progress);

  /**
   * Called when a page update is available, following a recognition,
   * beautification or ink search process.
   *
   * If the process succeeded the application should send a message to the UI
   * thread with the given change set so that the UI thread applies it.
   *
   * @param engine the engine.
   * @param target the target listener.
   * @param field the content field for which a result update is available.
   * @param changeSet the change set on success, otherwise `NULL`.
   */
  void (VO_MSE_CALL *onResultAvailable)(voEngine engine, voUserObject target, voContentField field, voPageChangeSet changeSet);

  /**
   * Called when an error occurred while processing `field`.
   * DEPRECATED: listeners should implement onErrorEx instead
   *
   * @param engine the engine.
   * @param target the target listener.
   * @param field the content field for which processing triggered an error.
   * @param error the last error encountered.
   */
  void (VO_MSE_CALL *onError)(voEngine engine, voEngineObject target, voContentField field, int32_t error);

  /**
   * Called when the recognition process starts on a field.
   *
   * @param engine the engine.
   * @param target the target listener.
   * @param field the content field.
   */
  void (VO_MSE_CALL *onFieldProcessingStart)(voEngine engine, voUserObject target, voContentField field);

  /**
   * Called when the recognition process starts on an active area.
   *
   * @param engine the engine.
   * @param target the target listener.
   * @param activeArea the active area.
   */
  void (VO_MSE_CALL *onActiveAreaProcessingStart)(voEngine engine, voUserObject target, voEngineObject activeArea);

  /**
   * Called when then recognition process starts on a page.
   *
   * @param engine the engine.
   * @param target the target listener.
   * @param page the page.
   */
  void (VO_MSE_CALL *onPageProcessingStart)(voEngine engine, voUserObject target, voPage page);

  /**
   * Called when the recognition process of a field ended.
   *
   * @param engine the engine.
   * @param target the target listener.
   * @param field the content field.
   */
  void (VO_MSE_CALL *onFieldProcessingEnd)(voEngine engine, voUserObject target, voContentField field);

  /**
   * Called when the recognition process on an active area ended.
   *
   * @param engine the engine.
   * @param target the target listener.
   * @param activeArea the active area.
   */
  void (VO_MSE_CALL *onActiveAreaProcessingEnd)(voEngine engine, voUserObject target, voEngineObject activeArea);

  /**
   * Called when the recognition process on a page ended.
   *
   * @param engine the engine.
   * @param target the target listener.
   * @param page the page.
   */
  void (VO_MSE_CALL *onPageProcessingEnd)(voEngine engine, voUserObject target, voPage page);

  /**
   * Called when an error occurred while processing `field`.
   *
   * @param engine the engine.
   * @param target the target listener.
   * @param field the content field for which processing triggered an error.
   * @param error the last error encountered.
   * @param charset the default charset, used to encode parameter strings.
   * @param messages the error messages.
   */
  void (VO_MSE_CALL *onErrorEx)(voEngine engine, voEngineObject target, voContentField field, int32_t error, voCharset charset, const voString* messages);
}
voIContentProcessorListener;


// -- Typesetting --------------------------------------------------------------

/**
 * `voTypesetItem` reference type.
 *
 * A typeset item is a glyph or primitive generated from recognition results,
 * that will replace a given portion of ink when the associated typeset data is
 * applied.
 *
 * {@extends voEngineObject}
 * {@implements voITypesetItem, voIGlyph, voIPrimitive}
 *
 * @since 7.0_0
 */
typedef voEngineObject voTypesetItem;

/**
 * Functions composing the `voITypesetItem` interface.
 *
 * This interface exposes the public methods of the `voTypesetItem` type.
 *
 * {@implementingTypes voTypesetItem}
 * @since 7.0_0
 */
typedef struct _voITypesetItem
{
  /**
   * Returns the type of the glyph (`VO_Glyph`) or primitive (`VO_LinePrimitive`
   * or `VO_ArcPrimitive`) associated with this item.
   *
   * @note Only glyphs are supported for the time being.
   *
   * @param engine the engine.
   * @param target the target typeset item.
   *
   * @return the type of typeset item on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voTypesetItem`.
   */
  voTypeId (VO_MSE_CALL *getType)(voEngine engine, voTypesetItem target);

  /**
   * Returns the ink portion associated with this item.
   *
   * @param engine the engine.
   * @param target the target typeset item.
   *
   * @return the ink segment on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voTypesetItem`.
   * @throws VO_LIMIT_EXCEEDED when the number of `voInkSegment` instances would
   *   exceed the allowed limit (please remember that the limit can be shared by
   *   several types).
   */
  voInkSegment (VO_MSE_CALL *getInkSegment)(voEngine engine, voTypesetItem target);

  /**
   * Modifies the geometry of the glyph associated with this typeset item.
   *
   * @param engine the engine.
   * @param target the target typeset item.
   * @param parallelogram the new position of the glyph.
   * @param lines the new guideline indications of the glyph, or `NULL` to keep
   *   them unchanged.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voTypesetItem` of type
   *   `VO_Glyph`.
   * @throws VO_INVALID_USER_BUFFER when `parallelogram` is not readable.
   * @throws VO_INVALID_ARGUMENT when `parallelogram` contains invalid values.
   * @throws VO_INVALID_ARGUMENT the `x_height` member of `lines` is negative.
   * @throws VO_INVALID_ARGUMENT `lines` float values are invalid.
   */
  bool (VO_MSE_CALL *setGlyphGeometry)(voEngine engine, voTypesetItem target, const voParallelogram* parallelogram, const voGlyphLines* lines);

  /**
   * Modifies the geometry of the primitive associated with this typeset item.
   *
   * @param engine the engine.
   * @param target the target typeset item.
   * @param data the new characteristics of the primitive, described as a
   *   `voCenterEllipticArc` or a `voEndpointEllipticArc` for a `voArcPrimitive`
   *   or as a `voLineSegment` for a `voLinePrimitive`.
   * @param sizeOfData the size of `data` in bytes.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voTypesetItem` of type
   *   `VO_LinePrimitive` or `VO_ArcPrimitive`.
   * @throws VO_INVALID_USER_BUFFER when `data` is not readable.
   * @throws VO_INVALID_ARGUMENT when `data` contains invalid values.
   * @throws VO_INVALID_ARGUMENT when `sizeOfData` does not match the size of a
   *   `voCenterEllipticArc`, a `voEndpointEllipticArc` or a `voLineSegment`.
   */
  bool (VO_MSE_CALL *setPrimitiveData)(voEngine engine, voTypesetItem target, void* data, size_t sizeOfData);
}
voITypesetItem;

/**
 * Functions composing the `voITypesetData` interface.
 *
 * This interface exposes the public methods of the `voTypesetData` type.
 *
 * {@implementingTypes voTypesetData}
 * @since 7.0_0
 */
typedef struct _voITypesetData
{
  /**
   * Returns the number of items in this typeset data.
   *
   * @param engine the engine.
   * @param target the target typeset data.
   *
   * @return number of items on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voTypesetData`.
   */
  int32_t (VO_MSE_CALL *getItemCount)(voEngine engine, voTypesetData target);

  /**
   * Returns the item at the specified index.
   *
   * @param engine the engine.
   * @param target the target typeset data.
   * @param index the index of the item to be retrieved.
   *
   * @return the typeset item on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voTypesetData`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_LIMIT_EXCEEDED when the number of `voTypesetItem` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   */
  voTypesetItem (VO_MSE_CALL *getItemAt)(voEngine engine, voTypesetData target, int32_t index);

  /**
   * Applies the typesetting modifications described in the given typeset data.
   *
   * If items that already reference a typeset result have been modified, a new
   * glyph or primitive will be created, and the old one will be added to
   * `oldData`.
   *
   * @param engine the engine.
   * @param target the typeset data.
   * @param keepLayout `true` when the layout information associated with the
   *   strokes to be typeset should be transferred to their typeset counterpart.
   * @param oldData a selection will be filled with the strokes that have been
   *   replaced by typeset data, or `NULL`.
   * @param newData a selection will be filled with the primitives or glyphs
   *   generated by the typeset operation, or `NULL`.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voTypesetData`.
   * @throws VO_NOT_PERMITTED_FROM_THIS_THREAD when the `apply()` call was made
   *   from within a `voContentProcessor` callback.
   * @throws VO_INVALID_STATE when the content field associated with `target`
   *   has been modified since `target` was created (e.g. by applying a
   *   `voPageChangeSet` or by processing that content field).
   */
  bool (VO_MSE_CALL *apply)(voEngine engine, voTypesetData target, bool keepLayout, voPageSelection oldData, voPageSelection newData);
}
voITypesetData;


// -- Layout -------------------------------------------------------------------

/**
 * Describes the available item types in the layout of a page.
 *
 * @see voLayoutItemIterator
 */
typedef enum _voLayoutItemType
{
  VO_LAYOUT_ITEM_STROKES = 1, /**< Associates an ink portion with a pen. */
  VO_LAYOUT_ITEM_OBJECT,      /**< Defines the position of a generic object. */
  VO_LAYOUT_ITEM_GUIDE,       /**< Defines the display of a content-guide. */

  _voLayoutItemType_FORCE_INT_SIZE = 0x7FFFFFFF
}
voLayoutItemType;

/**
 * Represents the properties of a strokes layout item.
 *
 * @see voLayout, voLayoutItemData
 */
typedef struct _voLayoutStrokesData
{
  /**
   * The identifier of the tag that defines the set of strokes in the page ink.
   */
  int64_t tagId;

  /**
   * The identifier of the pen used to draw the strokes.
   */
  const char* penId;

  /**
   * The identifier of the font associated with theses strokes.
   */
  const char* fontId;
}
voLayoutStrokesData;

/**
 * Represents the properties of an object layout item.
 *
 * @see voLayout, voLayoutItemData
 */
typedef struct _voLayoutObjectData
{
  /**
   * The URL of the object. You have to give this URL to
   * `voIDocument::extractObjectTo()` in order to retrieve the object data.
   */
  const char* url;

  /**
   * The position of the object in the layout, prior to the transformation
   * if there is one.
   */
  voRectangle rectangle;

  /**
   * The identifier of the transformation to be applied to the object, if any.
   * Otherwise `NULL`.
   *
   * You have to give this identifier to `voILayout::getTransform()` to get the
   * transform matrix.
   */
  const char* transformId;

  /**
   * The mime type of the object, if specified. Otherwise `NULL`.
   */
  const char* mimeType;
}
voLayoutObjectData;

/**
 * Represents the properties of a guide layout item.
 *
 * @see voLayout, voLayoutItemData
 */
typedef struct _voLayoutGuideData
{
  /**
   * The identifier of the guide defined in the content.
   */
  const char* guideId;

  /**
   * The identifier of the pen to be used to draw the guide.
   */
  const char* penId;
}
voLayoutGuideData;

/**
 * Represents the properties of a layout item.
 *
 * @see voLayout
 */
typedef struct _voLayoutItemData
{
  /**
   * The type of the layout item.
   */
  voLayoutItemType type;

  /**
   * The properties of the layout item. Used member depends on `type`.
   */
  union {
    /**
     * The properties of the layout item when `type == VO_LAYOUT_ITEM_STROKES`.
     */
    voLayoutStrokesData strokes;

    /**
     * The properties of the layout item when `type == VO_LAYOUT_ITEM_OBJECT`.
     */
    voLayoutObjectData object;

    /**
     * The properties of the layout item when `type == VO_LAYOUT_ITEM_GUIDE`.
     */
    voLayoutGuideData guide;
  } data;
}
voLayoutItemData;

/**
 * Represents the properties of a pen.
 *
 * @see voLayout
 */
typedef struct _voPenData
{
  /**
   * The name of the brush, if any. Otherwise `NULL`.
   */
  const char* brush;

  /**
   * The name of the smoothing algorithm, if any. Otherwise `NULL`.
   */
  const char* smoothing;

  /**
   * The width of the pen.
   */
  float width;

  /**
   * The color of the pen, in RGBA format.
   */
  uint32_t color;
}
voPenData;

/**
 * Represents a font that can be associated with strokes.
 *
 * @see voLayout
 */
typedef struct _voFontData
{
  /**
   * The font family.
   */
  const char* family;

  /**
   * The weight of the font.
   */
  float weight;

  /**
   * The size of the font.
   */
  float size;

  /**
   * The amount of space above and below inline elements.
   */
  float lineHeight;

  /**
   * The indication of whether normal, italic or oblique faces must be selected
   * within the font family.
   *
   * Typical values are: `normal`, `italic` and `oblique`.
   */
  const char* style;

  /**
   * The indication of whether normal or small caps faces faces must be selected
   * within the font family.
   *
   * Typical values are: `normal`, `none` and `small-caps`.
   */
  const char* variant;
}
voFontData;

/**
 * Functions composing the `voILayout` interface.
 *
 * {@implementingTypes voLayout, voPageSelection}
 */
typedef struct _voILayout
{
  /**
   * Returns the layout viewport.
   *
   * @param engine the engine.
   * @param target the target layout.
   * @param rectangle the recipient for the viewport rectangle.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_INVALID_USER_BUFFER when `rectangle` is not writable.
   */
  bool (VO_MSE_CALL *getViewport)(voEngine engine, voEngineObject target, voRectangle* rectangle);

  /**
   * Sets the layout viewport.
   *
   * @param engine the engine.
   * @param target the target layout.
   * @param rectangle the viewport rectangle.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_INVALID_USER_BUFFER when `rectangle` is not readable.
   */
  bool (VO_MSE_CALL *setViewport)(voEngine engine, voEngineObject target, const voRectangle* rectangle);

  /**
   * Returns the number of layers in the layout.
   *
   * @param engine the engine.
   * @param target the target layout.
   *
   * @return the number of layers on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   */
  int32_t (VO_MSE_CALL *getLayerCount)(voEngine engine, voEngineObject target);

  /**
   * Returns an iterator that points only to the layer specified by `id`.
   *
   * When no layer corresponds to `id`, an empty iterator is returned.
   *
   * When the corresponding page is modified, the iterator is invalidated.
   *
   * @param engine the engine.
   * @param target the target layout.
   * @param charset the charset used to encode parameter strings.
   * @param id the identifier of the layer to be retrieved.
   *
   * @return a layer iterator on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `id` or its `bytes` member is not
   *   readable.
   * @throws VO_LIMIT_EXCEEDED when the number of `voLayerIterator` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   */
   voLayerIterator (VO_MSE_CALL *getLayer)(voEngine engine, voEngineObject target, voCharset charset, const voString* id);

  /**
   * Returns an iterator over the layers of this layout.
   *
   * When the corresponding page is modified, the iterator is invalidated.
   *
   * @param engine the engine.
   * @param target the target layout.
   *
   * @return a layer iterator on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_LIMIT_EXCEEDED when the number of `voLayerIterator` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   */
   voLayerIterator (VO_MSE_CALL *getLayers)(voEngine engine, voEngineObject target);

  /**
   * Appends a new layer on front of this layout.
   *
   * @param engine the engine.
   * @param target the target layout.
   * @param charset the charset used to encode parameter strings.
   * @param id the identifier of the layer to be created.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `id` or its `bytes` member is not
   *   readable.
   * @throws VO_INVALID_STATE when a layout with this `id` already exists.
   */
  bool (VO_MSE_CALL *addLayer)(voEngine engine, voEngineObject target, voCharset charset, const voString* id);

  /**
   * Returns the number of pens in this layout.
   *
   * @param engine the engine.
   * @param target the target layout.
   *
   * @return the number of pens on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   */
  int32_t (VO_MSE_CALL *getPenCount)(voEngine engine, voEngineObject target);

  /**
   * Returns an iterator that points only to the pen specified by `id`.
   *
   * When no pen corresponds to `id`, an empty iterator is returned.
   *
   * When the corresponding page is modified, the iterator is invalidated.
   *
   * @param engine the engine.
   * @param target the target layout.
   * @param charset the charset used to encode parameter strings.
   * @param id the identifier of the pen to be retrieved.
   *
   * @return a pen iterator on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `id` or its `bytes` member is not
   *   readable.
   * @throws VO_LIMIT_EXCEEDED when the number of `voPenIterator` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   */
  voPenIterator (VO_MSE_CALL *getPen)(voEngine engine, voEngineObject target, voCharset charset, const voString* id);

  /**
   * Returns an iterator over the pens of this layout.
   *
   * When the corresponding page is modified, the iterator is invalidated.
   *
   * @param engine the engine.
   * @param target the target layout.
   *
   * @return a pen iterator on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_LIMIT_EXCEEDED when the number of `voPenIterator` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   */
  voPenIterator (VO_MSE_CALL *getPens)(voEngine engine, voEngineObject target);

  /**
   * Adds a new pen element to this layout.
   *
   * @param engine the engine.
   * @param target the target layout.
   * @param charset the charset used to encode parameter strings.
   * @param id the identifier of the pen to be created.
   * @param data the description of the pen.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `id` or its `bytes` member is not
   *   readable.
   * @throws VO_INVALID_USER_BUFFER when `data` is not readable.
   * @throws VO_INVALID_STATE when a pen with this `id` already exists.
   */
  bool (VO_MSE_CALL *addPen)(voEngine engine, voEngineObject target, voCharset charset, const voString* id, const voPenData* data);

  /**
   * Returns the parameters of the specified transform.
   *
   * @param engine the engine.
   * @param target the target layout.
   * @param charset the charset used to encode parameter strings.
   * @param id the identifier of the transform to be retrieved.
   * @param transform the recipient for the transform matrix.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `id` or its `bytes` member is not
   *   readable.
   * @throws VO_INVALID_USER_BUFFER when `transform` is not writable.
   */
  bool (VO_MSE_CALL *getTransform)(voEngine engine, voEngineObject target, voCharset charset, const voString* id, voTransform* transform);

  /**
   * Returns the number of fonts in this layout.
   *
   * @param engine the engine.
   * @param target the target layout.
   *
   * @return the number of fonts on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   */
  int32_t (VO_MSE_CALL *getFontCount)(voEngine engine, voEngineObject target);

  /**
   * Returns an iterator that points only to the font specified by `id`.
   *
   * When no font corresponds to `id`, an empty iterator is returned.
   *
   * When the corresponding page is modified, the iterator is invalidated.
   *
   * @param engine the engine.
   * @param target the target layout.
   * @param charset the charset used to encode parameter strings.
   * @param id the identifier of the font to be retrieved.
   *
   * @return a font iterator on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `id` or its `bytes` member is not
   *   readable.
   * @throws VO_LIMIT_EXCEEDED when the number of `voFontIterator` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   */
  voFontIterator (VO_MSE_CALL *getFont)(voEngine engine, voEngineObject target, voCharset charset, const voString* id);

  /**
   * Returns an iterator over the font of the specified layout.
   *
   * When the corresponding page is modified, the iterator is invalidated.
   *
   * @param engine the engine.
   * @param target the target layout.
   *
   * @return a font iterator on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_LIMIT_EXCEEDED when the number of `voFontIterator` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   */
  voFontIterator (VO_MSE_CALL *getFonts)(voEngine engine, voEngineObject target);

  /**
   * Adds a new font description to the layout.
   *
   * If a font with same id already exists, operation will fail.
   *
   * @param engine the engine.
   * @param target the target layout.
   * @param charset the charset used to encode parameter strings.
   * @param id the identifier of the font to create.
   * @param data the description of the font properties.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `id` or its `bytes` member is not
   *   readable.
   * @throws VO_INVALID_USER_BUFFER when `data`, its `family`, `style` or
   *   `variant` member is not readable.
   * @throws VO_INVALID_STATE when a font with this `id` already exists.
   */
  bool (VO_MSE_CALL *addFont)(voEngine engine, voEngineObject target, voCharset charset, const voString* id, const voFontData* data);
}
voILayout;

/**
 * Functions composing the `voILayerIterator` interface.
 *
 * This interface exposes the public methods of the `voLayerIterator` type.
 *
 * @note Layer iterators created from a `voPageSelection` are not allowed to
 *   modify the layout.
 *
 * @note As the C language does not provide any mechanism that could be used to
 *   simulate inheritance, inherited members are simply copied from the base
 *   structure.
 *
 * {@extends voIIterator}
 * {@implementingTypes voLayerIterator}
 */
typedef struct _voILayerIterator
{
  /** {@inheritDoc} */
  voYesNo (VO_MSE_CALL *isAtEnd)(voEngine engine, voIterator target);
  /** {@inheritDoc} */
  bool (VO_MSE_CALL *next)(voEngine engine, voIterator target);
  /** {@inheritDoc} */
  voYesNo (VO_MSE_CALL *equals)(voEngine engine, voIterator target, voIterator other);

  /**
   * Returns the identifier of the layer at the current step of the iteration.
   *
   * @param engine the engine.
   * @param target the target iterator.
   * @param charset the charset used to encode parameter strings.
   * @param id the recipient for the identifier.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voLayerIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `id` or its `bytes` member is not
   *   writable.
   */
  bool (VO_MSE_CALL *getId)(voEngine engine, voLayerIterator target, voCharset charset, voString* id);

  /**
   * Returns the background color of the layer at the current step of the
   * iteration. The default color is `0xFFFFFF00` (transparent white).
   *
   * @param engine the engine.
   * @param target the target iterator.
   * @param color the color of the background in RGBA format (32 bits).
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voLayerIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   * @throws VO_INVALID_USER_BUFFER when `color` is not writable.
   */
  bool (VO_MSE_CALL *getBackgroundColor)(voEngine engine, voLayerIterator target, uint32_t* color);

  /**
   * Sets the background color of the layer at the current step of the iteration.
   * The default color is `0xFFFFFF00` (transparent white).
   *
   * @param engine the engine.
   * @param target the target iterator.
   * @param color the color of the background in RGBA format (32 bits).
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voLayerIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   */
  bool (VO_MSE_CALL *setBackgroundColor)(voEngine engine, voLayerIterator target, uint32_t color);

  /**
   * Removes the layer at the current step of the iteration from the layout.
   * The iterator advances to the next iteration.
   *
   * @param engine the engine.
   * @param target the target iterator.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voLayerIterator`.
   * @throws VO_INVALID_OPERATION when `target` was created from a
   *   `voPageSelection`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   */
  bool (VO_MSE_CALL *remove)(voEngine engine, voLayerIterator target);

  /**
   * Returns the number of layout items in the layer.
   *
   * @param engine the engine.
   * @param target the target iterator.
   *
   * @return the number of layout items on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voLayerIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   */
  int32_t (VO_MSE_CALL *getItemCount)(voEngine engine, voLayerIterator target);

  /**
   * Returns an iterator over the layout items of the layer. When the
   * corresponding page is modified, the iterator will be invalidated.
   *
   * @param engine the engine.
   * @param target the target iterator.
   *
   * @return the layout item iterator on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voLayerIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   * @throws VO_LIMIT_EXCEEDED when the number of `voLayoutItemIterator`
   *   instances would exceed the allowed limit (please remember that the limit
   *   can be shared by several types).
   */
  voLayoutItemIterator (VO_MSE_CALL *getItems)(voEngine engine, voLayerIterator target);

  /**
   * Adds a new item at the top of the layer.
   *
   * @param engine the engine.
   * @param target the target iterator.
   * @param data the properties of the layout item.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voLayerIterator`.
   * @throws VO_INVALID_ARGUMENT when `target` was created from a
   *   `voPageSelection`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   * @throws VO_INVALID_USER_BUFFER when `data` is not readable.
   * @throws VO_INVALID_ARGUMENT when `data` is of type `VO_LAYOUT_ITEM_STROKES`
   *   and its `fontId` or `penId` member does not refer to an existing font or
   *   pen.
   * @throws VO_INVALID_ARGUMENT when `data` is of type `VO_LAYOUT_ITEM_OBJECT`
   *   and its `transformId` member does not refer to an existing transform.
   * @throws VO_INVALID_ARGUMENT when `data` is of type `VO_LAYOUT_ITEM_GUIDE`
   *   and its `penId` member does not refer to an existing pen.
   */
  bool (VO_MSE_CALL *addItem)(voEngine engine, voLayerIterator target, const voLayoutItemData* data);
}
voILayerIterator;

/**
 * Functions composing the `voIPenIterator` interface.
 *
 * This interface exposes the public methods of the `voPenIterator` type.
 *
 * @note As the C language does not provide any mechanism that could be used to
 *   simulate inheritance, inherited members are simply copied from the base
 *   structure.
 *
 * {@extends voIIterator}
 * {@implementingTypes voIPenIterator}
 */
typedef struct _voIPenIterator
{
  /** {@inheritDoc} */
  voYesNo (VO_MSE_CALL *isAtEnd)(voEngine engine, voIterator target);
  /** {@inheritDoc} */
  bool (VO_MSE_CALL *next)(voEngine engine, voIterator target);
  /** {@inheritDoc} */
  voYesNo (VO_MSE_CALL *equals)(voEngine engine, voIterator target, voIterator other);

  /**
   * Returns the identifier of the pen at the current step of the iteration.
   *
   * @param engine the engine.
   * @param target the target iterator.
   * @param charset the charset used to encode parameter strings.
   * @param id the recipient for the identifier.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voPenIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `id` or its `bytes` member is not
   *   writable.
   */
  bool (VO_MSE_CALL *getId)(voEngine engine, voPenIterator target, voCharset charset, voString* id);

  /**
   * Returns the properties of the pen at the current step of the iteration.
   *
   * @param engine the engine.
   * @param target the target iterator.
   * @param data the recipient for the pen properties.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voPenIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   * @throws VO_INVALID_USER_BUFFER when `data` is not writable.
   */
  bool (VO_MSE_CALL *getData)(voEngine engine, voPenIterator target, voPenData* data);

  /**
   * Sets the properties of the pen at the current step of the iteration.
   *
   * @param engine the engine.
   * @param target the target iterator.
   * @param data the properties of the pen.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voPenIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   * @throws VO_INVALID_USER_BUFFER when `data` is not readable.
   */
  bool (VO_MSE_CALL *setData)(voEngine engine, voPenIterator target, const voPenData* data);
}
voIPenIterator;

/**
 * Functions composing the `voIFontIterator` interface.
 *
 * This interface exposes the public methods of the `voFontIterator` type.
 *
 * @note As the C language does not provide any mechanism that could be used to
 *   simulate inheritance, inherited members are simply copied from the base
 *   structure.
 *
 * {@extends voIIterator}
 * {@implementingTypes voIFontIterator}
 */
typedef struct _voIFontIterator
{
  /** {@inheritDoc} */
  voYesNo (VO_MSE_CALL *isAtEnd)(voEngine engine, voIterator target);
  /** {@inheritDoc} */
  bool (VO_MSE_CALL *next)(voEngine engine, voIterator target);
  /** {@inheritDoc} */
  voYesNo (VO_MSE_CALL *equals)(voEngine engine, voIterator target, voIterator other);

  /**
   * Returns the identifier of the font at the current step of the iteration.
   *
   * @param engine the engine.
   * @param target the target iterator.
   * @param charset the charset used to encode parameter strings.
   * @param id the recipient for the identifier.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voFontIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `id` or its `bytes` member is not
   *   writable.
   */
  bool (VO_MSE_CALL *getId)(voEngine engine, voFontIterator target, voCharset charset, voString* id);

  /**
   * Returns the properties of the font at the current step of the iteration.
   *
   * @param engine the engine.
   * @param target the target iterator.
   * @param data the recipient for the font properties.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voFontIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   * @throws VO_INVALID_USER_BUFFER when `data` is not writable.
   */
  bool (VO_MSE_CALL *getData)(voEngine engine, voFontIterator target, voFontData* data);

  /**
   * Sets the properties of the font at the current step of the iteration.
   *
   * @param engine the engine.
   * @param target the target iterator.
   * @param data the properties of the font.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voFontIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   * @throws VO_INVALID_USER_BUFFER when `data`, its `family`, `style` or
   *   `variant` member is not readable.
   */
  bool (VO_MSE_CALL *setData)(voEngine engine, voFontIterator target, const voFontData* data);
}
voIFontIterator;


/**
 * Functions composing the `voILayoutItemIterator` interface.
 *
 * This interface exposes the public methods of the `voLayoutItemIterator` type.
 *
 * @note As the C language does not provide any mechanism that could be used to
 *   simulate inheritance, inherited members are simply copied from the base
 *   structure.
 *
 * {@extends voIIterator}
 * {@implementingTypes voLayoutItemIterator}
 */
typedef struct _voILayoutItemIterator
{
  /** {@inheritDoc} */
  voYesNo (VO_MSE_CALL *isAtEnd)(voEngine engine, voIterator target);
  /** {@inheritDoc} */
  bool (VO_MSE_CALL *next)(voEngine engine, voIterator target);
  /** {@inheritDoc} */
  voYesNo (VO_MSE_CALL *equals)(voEngine engine, voIterator target, voIterator other);

  /**
   * Returns the properties of the layout item at the current step of the
   * iteration.
   *
   * @param engine the engine.
   * @param target the target iterator.
   * @param data the properties of the layout item.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voLayoutItemIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   * @throws VO_INVALID_USER_BUFFER when `data` is not writable.
   */
  bool (VO_MSE_CALL *getData)(voEngine engine, voLayoutItemIterator target, voLayoutItemData* data);
}
voILayoutItemIterator;


// -- Page monitoring ----------------------------------------------------------

/**
 * Functions composing the `voIPageUpdateListener` interface.
 *
 * @note This interface is implementable by user objects. User code is not
 *   allowed to invoke this interface directly: as a consequence
 *   `voGetInterface()` returns `NULL`.
 */
typedef struct _voIPageUpdateListener
{
  /**
   * Called when the target change listener is added to a page object.
   *
   * @param engine the engine.
   * @param target the target change listener.
   * @param page the page to which the change listener was added.
   */
  void (VO_MSE_CALL *added)(voEngine engine, voUserObject target, voPage page);

  /**
   * Called when the target change listener is removed from a page object.
   *
   * @param engine the engine.
   * @param target the target change listener.
   * @param page the page from which the change listener was removed.
   */
  void (VO_MSE_CALL *removed)(voEngine engine, voUserObject target, voPage page);

  /**
   * Called when a layer of the page layout has changed.
   *
   * @param engine the engine.
   * @param target the target change listener.
   * @param page the page that has been updated.
   * @param layerIndex the layout layer index on to which extent applies.
   * @param extent the extent of updates on the X and Y axes for the layer at
   *   `layerIndex`.
   */
  void (VO_MSE_CALL *onLayerUpdate)(voEngine engine, voUserObject target, voPage page, int32_t layerIndex, const voExtent* extent);

  /**
   * Called when the input of a content field has changed.
   *
   * @param engine the engine.
   * @param target the target change listener.
   * @param page the page that has been updated.
   * @param field the content field which input has changed.
   */
  void (VO_MSE_CALL *onContentFieldInputUpdate)(voEngine engine, voUserObject target, voPage page, voContentField field);

  /**
   * Called when the result of a content field has changed.
   *
   * @param engine the engine.
   * @param target the target change listener.
   * @param page the page that has been updated.
   * @param field the content field which result has changed.
   */
  void (VO_MSE_CALL *onContentFieldResultUpdate)(voEngine engine, voUserObject target, voPage page, voContentField field);

  /**
   * Called when the page layout has changed.
   *
   * @param engine the engine.
   * @param target the target change listener.
   * @param page the page that has been updated.
   * @param extent the extent of updates on the X and Y axes.
   */
  void (VO_MSE_CALL *onLayoutUpdate)(voEngine engine, voUserObject target, voPage page, const voExtent* extent);
}
voIPageUpdateListener;


// -- DocumentFinder -----------------------------------------------------------

/**
 * `voDocumentFinder` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voIDocumentFinder}
 */
typedef voEngineObject voDocumentFinder;

/**
 * `voDocumentFindResult` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voIDocumentFindResult}
 */
typedef voEngineObject voDocumentFindResult;

/**
 * `voDocumentFindOccurrenceIterator` reference type.
 *
 * {@extends voIterator}
 * {@implements voIDocumentFindOccurrenceIterator}
 */
typedef voIterator voDocumentFindOccurrenceIterator;

/**
 * Lists the asynchronous events of `voDocumentFinder`.
 *
 * @see voDocumentFinderCallback
 */
typedef enum _voDocumentFinderEvent
{
  /**
   * Indicates the worker thread started a new find process.
   */
  VO_DOCUMENT_FINDER_EVENT_START,

  /**
   * Indicates the worker thread has found new occurrences.
   */
  VO_DOCUMENT_FINDER_EVENT_NEW_OCCURRENCES,

  /**
   * Indicates the worker thread has completed the find process.
   */
  VO_DOCUMENT_FINDER_EVENT_DONE,

  /**
   * Indicates cancellation of the find process.
   */
  VO_DOCUMENT_FINDER_EVENT_CANCELLED,

  _voDocumentFinderEvent_FORCE_INT_SIZE = 0x7FFFFFFF
}
voDocumentFinderEvent;

/**
 * Callback called by the document finder background thread upon completion of
 * asynchronous events of `voDocumentFinder`.
 *
 * The objects provided in `result` are valid only during the
 * callback execution. You must use `voAcquireObject()` if you want to retain
 * them for later use.
 *
 * Do not call wait() from this callback, or this will result in a deadlock.
 *
 * Do not call other asynchronous functions from this callback (functions
 * that call a callback upon completion), prefer calling asynchronous functions
 * in sequence.
 *
 * @param eventId the identifier of the event that occurred.
 * @param occurrenceIterator an iterator over the new occurrences, when
 *   `eventId` is `VO_EVENT_FINDER_OCCURRENCES`.
 * @param userParam an optional user defined parameter passed to the callback.
 *
 * @see voIDocumentFinder
 */
typedef void (VO_MSE_CALL *voDocumentFinderCallback)(voDocumentFinderEvent eventId, voDocumentFindOccurrenceIterator occurrenceIterator, void* userParam);


/**
 * Holds the parameters required to create a new `voDocumentFinder` instance.
 *
 * @see voDocumentFinder, voCreateDocumentFinder
 */
typedef struct _voDocumentFinderInitializer
{
  /**
   * The associated document.
   */
  voDocument document;

  /**
   * The callback used to handle the asynchronous events of the document finder.
   */
  voDocumentFinderCallback callback;

  /**
   * The user parameter sent to the callback.
   */
  void* userParam;
}
voDocumentFinderInitializer;

#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voDocumentFinder` instance.
 *
 * @param engine the engine.
 * @param document the document to search in.
 * @param callback the callback that will receive worker thread events.
 * @param userParam an optional pointer to a user defined parameter that will
 *   be passed back to the `callback` function.
 *
 * @return the newly created `voDocumentFinder` instance on success, otherwise
 *   `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_NO_SUCH_OBJECT when the `document` reference is invalid.
 * @throws VO_INVALID_OBJECT when `document` is not a `voDocument`.
 * @throws VO_LIMIT_EXCEEDED when the number of `voDocumentFinder` instances
 *   would exceed the allowed limit (please remember that the limit can be
 *   shared by several types).
 */
VO_INLINE voDocumentFinder voCreateDocumentFinder(voEngine engine, voDocument document, voDocumentFinderCallback callback, void* userParam)
{
  voDocumentFinderInitializer initializer = { document, callback, userParam };
  return voCreateObjectEx(engine, VO_DocumentFinder, &initializer, sizeof(initializer));
}
#endif // end of: #ifndef VO_NO_FUNCTIONS

/**
 * Functions composing the `voIDocumentFinder` interface.
 *
 * This interface exposes the public methods of the `voDocumentFinder` type.
 *
 * {@implementingTypes voDocumentFinder}
 */
typedef struct _voIDocumentFinder
{
  /**
   * Launches a find process with the specified query.
   *
   * @param engine the engine.
   * @param target the target document finder.
   * @param query the query to search for.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voDocumentFinder`.
   * @throws VO_INVALID_OBJECT when `query` is not a `voQuery`.
   * @throws VO_INVALID_STATE when `target` finder is being destroyed.
   * @throws VO_INVALID_STATE when a `cancel()` operation is ongoing.
   */
  bool (VO_MSE_CALL *process)(voEngine engine, voDocumentFinder target, voQuery query);

  /**
   * Returns the result of a find process.
   *
   * @param engine the engine.
   * @param target the target document finder.
   * @param wait whether to wait for the result to be available.
   *
   * @return the result of the search if any, otherwise `NULL`.
   *   When `NULL`, use `voGetError()` to figure out whether an error occurred.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voDocumentFinder`.
   * @throws VO_INVALID_STATE when `target` finder is being destroyed.
   */
  voDocumentFindResult (VO_MSE_CALL *getResult)(voEngine engine, voDocumentFinder target, bool wait);

  /**
   * Requests cancellation of an ongoing find process.
   *
   * @param engine the engine.
   * @param target the target document finder.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voDocumentFinder`.
   * @throws VO_INVALID_STATE when `target` finder is being destroyed.
   * @throws VO_INVALID_STATE when a `cancel()` operation is already ongoing.
   */
  bool (VO_MSE_CALL *cancel)(voEngine engine, voDocumentFinder target);
}
voIDocumentFinder;


/**
 * Functions composing the `voIDocumentFindResult` interface.
 *
 * This interface exposes the public methods of the `voDocumentFindResult` type.
 *
 * {@implementingTypes voDocumentFindResult}
 */
typedef struct _voIDocumentFindResult
{
  /**
   * Returns an iterator over the occurrences that compose this find result.
   *
   * @param engine the engine.
   * @param target the target find result.
   *
   * @return an iterator on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voDocumentFindResult`.
   * @throws VO_LIMIT_EXCEEDED when the number of
   * `voDocumentFindOccurenceIterator` instances would exceed the allowed limit
   * (please remember that the limit can be shared by several types).
   */
  voDocumentFindOccurrenceIterator (VO_MSE_CALL *getOccurrences)(voEngine engine, voDocumentFindResult target);
}
voIDocumentFindResult;


/**
 * Functions composing the `voIDocumentFindOccurrenceIterator` interface.
 *
 * This interface exposes the public methods of the
 * `voDocumentFindOccurrenceIterator` type.
 *
 * @note As the C language does not provide any mechanism that could be used to
 *   simulate inheritance, inherited members are simply copied from the base
 *   structure.
 *
 * {@extends voIIterator}
 * {@implementingTypes voDocumentFindOccurrenceIterator}
 */
typedef struct _voIDocumentFindOccurrenceIterator
{
  /** {@inheritDoc} */
  voYesNo (VO_MSE_CALL *isAtEnd)(voEngine engine, voIterator target);
  /** {@inheritDoc} */
  bool (VO_MSE_CALL *next)(voEngine engine, voIterator target);
  /** {@inheritDoc} */
  voYesNo (VO_MSE_CALL *equals)(voEngine engine, voIterator target, voIterator other);

  /**
   * Returns the identifier of the page that contains the occurrence at the
   * current step of the iteration.
   *
   * @param engine the engine.
   * @param target the target iterator.
   * @param charset the charset used to encode parameter strings.
   * @param id the recipient for the identifier.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a
   *   `voDocumentFindOccurrenceIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `id` or its `bytes` member is not
   *   writable.
   */
  bool (VO_MSE_CALL *getPageId)(voEngine engine, voDocumentFindOccurrenceIterator target, voCharset charset, voString* id);

  /**
   * Returns the identifier of the indexed field that contains the occurrence at
   * the current step of the iteration.
   *
   * @param engine the engine.
   * @param target the target iterator.
   * @param charset the charset used to encode parameter strings.
   * @param id the recipient for the identifier.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a
   *   `voDocumentFindOccurrenceIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `id` or its `bytes` member is not
   *   writable.
   */
  bool (VO_MSE_CALL *getFieldId)(voEngine engine, voDocumentFindOccurrenceIterator target, voCharset charset, voString* id);

  /**
   * Returns the string representation of the ink segment of the occurrence at
   * the current step of the iteration.
   *
   * @param engine the engine.
   * @param target the target iterator.
   * @param charset the charset used to encode parameter strings.
   * @param string the recipient for the representation of the ink segment.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a
   *   `voDocumentFindOccurrenceIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `string` or its `bytes` member is not
   *   writable.
   */
  bool (VO_MSE_CALL *getInkSegment)(voEngine engine, voDocumentFindOccurrenceIterator target, voCharset charset, voString* string);

  /**
   * Returns the label of the occurrence at the current step of the iteration.
   *
   * @param engine the engine.
   * @param target the target iterator.
   * @param charset the charset used to encode parameter strings.
   * @param label the recipient for the representation of the label.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a
   *   `voDocumentFindOccurrenceIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `string` or its `bytes` member is not
   *   writable.
   */
  bool (VO_MSE_CALL *getLabel)(voEngine engine, voDocumentFindOccurrenceIterator target, voCharset charset, voString* label);

  /**
   * Returns the score of the occurrence at the current step of the iteration.
   *
   * @param engine the engine.
   * @param target the target iterator.
   *
   * @return the score of this occurrence on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a
   *   `voDocumentFindOccurrenceIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   */
  float (VO_MSE_CALL *getScore)(voEngine engine, voDocumentFindOccurrenceIterator target);

  /**
   * Returns the index of the matched query for the occurrence at the current
   * step of the iteration.
   *
   * @param engine the engine.
   * @param target the target iterator.
   *
   * @return the index on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a
   *   `voDocumentFindOccurrenceIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   */
  int32_t (VO_MSE_CALL *getQueryIndex)(voEngine engine, voDocumentFindOccurrenceIterator target);
}
voIDocumentFindOccurrenceIterator;


// -- TextInspector ------------------------------------------------------------

/**
 * `voTextInspector` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voITextInspector}
 */
typedef voEngineObject voTextInspector;

/**
 * `voTextInterval` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voITextInterval}
 */
typedef voEngineObject voTextInterval;

/**
 * Holds the parameters required to create a new `voTextInspector` instance.
 *
 * @see voTextInspector, voCreateTextInspector
 */
typedef struct _voTextInspectorInitializer
{
  /**
   * Associated page.
   */
  voPage page;
}
voTextInspectorInitializer;

/**
 * Represents the type of boundary used by the expand function.
 *
 * @see voTextInspector::expand
 */
typedef enum _voTextBoundary
{
  VO_TEXT_BOUNDARY_CHAR,  /**< Character boundary. Don't expand. */
  VO_TEXT_BOUNDARY_WORD,  /**< Word boundary. Expand to start/end of word. */
  VO_TEXT_BOUNDARY_LINE,  /**< Line boundary. Expand to start/end of line. */
  VO_TEXT_BOUNDARY_FIELD, /**< Field boundary. Expand to start/end of field. */

  _voTextBoundary_FORCE_INT_SIZE = 0x7FFFFFFF
}
voTextBoundary;


/**
 * Functions composing the `voITextInterval` interface.
 *
 * This interface exposes the public methods of the `voTextInterval` type.
 *
 * {@implementingTypes voTextInterval}
 */
typedef struct _voITextInterval
{
  /**
   * Returns the field associated with this text interval.
   *
   * @param engine the engine.
   * @param target the target text interval.
   *
   * @return the content field on on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voTextInterval`.
   * @throws VO_LIMIT_EXCEEDED when the number of `voContentField` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   */
  voContentField (VO_MSE_CALL *getField)(voEngine engine, voTextInterval target);

  /**
   * Returns the begin position of this text interval.
   *
   * This position corresponds to the index of a recognized character in the
   * content field associated with `target`.
   *
   * @param engine the engine.
   * @param target the target text interval.
   *
   * @return the position on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voTextInterval`.
   */
  int32_t (VO_MSE_CALL *getBeginPosition)(voEngine engine, voTextInterval target);

  /**
   * Returns the end position of this text interval.
   *
   * This position corresponds to the index of a recognized character in the
   * content field associated with `target`.
   *
   * @param engine the engine.
   * @param target the target text interval.
   *
   * @return the position on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voTextInterval`.
   */
  int32_t (VO_MSE_CALL *getEndPosition)(voEngine engine, voTextInterval target);

  /**
   * Sets the begin position of this text interval.
   *
   * @note The text interval does not check that the position you provide is
   *   correct. If you set an invalid position, further operations of the text
   *   inspector using this text interval will fail.
   *
   * @param engine the engine.
   * @param target the target text interval.
   * @param position the target position.
   *
   * @return `true`on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voTextInterval`.
   * @throws VO_INVALID_ARGUMENT when `position` is negative.
   */
  bool (VO_MSE_CALL *setBeginPosition)(voEngine engine, voTextInterval target, int32_t position);

  /**
   * Sets the end position of this text interval.
   *
   * @note The text interval does not check that the position you provide is
   *   correct. If you set an invalid position, further operations of the text
   *   inspector using this text interval will fail.
   *
   * @param engine the engine.
   * @param target the target text interval.
   * @param position the target position.
   *
   * @return `true`on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voTextInterval`.
   * @throws VO_INVALID_ARGUMENT when `position` is negative.
   */
  bool (VO_MSE_CALL *setEndPosition)(voEngine engine, voTextInterval target, int32_t position);
}
voITextInterval;


#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voTextInspector` instance.
 *
 * @param engine the engine.
 * @param page the associated page.
 *
 * @return the newly created `voTextInspector` instance on success, otherwise
 *   `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_NO_SUCH_OBJECT when the `page` reference is invalid.
 * @throws VO_INVALID_OBJECT when `page` is not a `voPage`.
 * @throws VO_LIMIT_EXCEEDED when the number of `voTextInspector` instances
 *   would exceed the allowed limit (please remember that the limit can be
 *   shared by several types).
 */
VO_INLINE voTextInspector voCreateTextInspector(voEngine engine, voPage page)
{
  voTextInspectorInitializer initializer = { page };
  return voCreateObjectEx(engine, VO_TextInspector, &initializer, sizeof(initializer));
}
#endif // end of: #ifndef VO_NO_FUNCTIONS

/**
 * Represents information associated with a character in the text inspector.
 *
 * @see voTextInspector
 */
typedef struct _voCharInfo
{
  int32_t wordIndex;      /**< The containing word. */
  int32_t lineIndex;      /**< The containing line. */
  voPoint gridPoints[4];  /**< The four points that define the grid cell. */
}
voCharInfo;

/**
 * Represents information associated with a word in the text inspector.
 *
 * @see voTextInspector
 */
typedef struct _voWordInfo
{
  int32_t charBeginIndex; /**< The begin index of the contained characters. */
  int32_t charEndIndex;   /**< The end index of the contained characters. */
  int32_t lineBeginIndex; /**< The begin index of the containing lines. */
  int32_t lineEndIndex;   /**< The end index of the containing lines. */
}
voWordInfo;

/**
 * Represents information associated with a line in the text inspector.
 *
 * @see voTextInspector
 */
typedef struct _voLineInfo
{
  int32_t charBeginIndex; /**< The begin index of the contained characters. */
  int32_t charEndIndex;   /**< The end index of the contained characters. */
  voLineSegment baseline; /**< The baseline. */
  float xHeight;          /**< The x-height. */
  float shearAngle;       /**< The character x-axis shear angle. */
}
voLineInfo;

/**
 * Functions composing the `voITextInspector` interface.
 *
 * This interface exposes the public methods of the `voTextInspector` type.
 *
 * {@implementingTypes voTextInspector}
 */
typedef struct _voITextInspector
{
  /**
   * Returns the page associated with this text inspector.
   *
   * @param engine the engine.
   * @param target the target text inspector.
   *
   * @return the page on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voTextInspector`.
   * @throws VO_LIMIT_EXCEEDED when the number of `voPage` instances would
   *   exceed the allowed limit (please remember that the limit can be shared by
   *   several types).
   */
  voPage (VO_MSE_CALL *getPage)(voEngine engine, voTextInspector target);

  /**
   * Returns a text interval that corresponds to the closest character boundary
   * from the specified position.
   *
   * The interval is a cursor position so the end position will be the same as
   * the begin position.
   *
   * @note A text content field can be specified to constrain the search. Won't
   *   hit when position is too far from the field boundaries.
   *
   * @param engine the engine.
   * @param target the target text inspector.
   * @param position the geometric position.
   * @param field a text content field, or `NULL`.
   *
   * @return the newly created `voTextInterval` instance, if found.
   *   When `NULL`, use `voGetError()` to figure out whether an error occurred.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voTextInspector`.
   * @throws VO_INVALID_USER_BUFFER when `position` is not readable.
   * @throws VO_NO_SUCH_OBJECT when the `field` reference is invalid.
   * @throws VO_INVALID_OBJECT when `field` is not a `voContentField` of type
   *   `VO_CONTENT_FIELD_TEXT`.
   * @throws VO_INVALID_ARGUMENT when `field` comes from a different page.
   * @throws VO_INVALID_ARGUMENT when `field` refers to a content field that has
   *   been removed.
   * @throws VO_LIMIT_EXCEEDED when the number of `voTextInterval` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   */
  voTextInterval (VO_MSE_CALL *hitCursor)(voEngine engine, voTextInspector target,
                                          const voPoint* position, voContentField field);

  /**
   * Returns a text interval that corresponds to the character under the
   * specified position.
   *
   * The interval surrounds the character so the begin
   * position points to it and the end position points just after it.
   *
   * @note A text content field can be specified to constrain the search.
   *
   * @param engine the engine.
   * @param target the target text inspector.
   * @param position the geometric position.
   * @param field a text content field, or `NULL`.
   *
   * @return the newly created `voTextInterval` instance, if found.
   *   When `NULL`, use `voGetError()` to figure out whether an error occurred.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voTextInspector`.
   * @throws VO_INVALID_USER_BUFFER when `position` is not readable.
   * @throws VO_NO_SUCH_OBJECT when the `field` reference is invalid.
   * @throws VO_INVALID_OBJECT when `field` is not a `voContentField` of type
   *   `VO_CONTENT_FIELD_TEXT`.
   * @throws VO_INVALID_ARGUMENT when `field` comes from a different page.
   * @throws VO_INVALID_ARGUMENT when `field` refers to a content field that has
   *   been removed.
   * @throws VO_LIMIT_EXCEEDED when the number of `voTextInterval` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   */
  voTextInterval (VO_MSE_CALL *hitChar)(voEngine engine, voTextInspector target,
                                        const voPoint* position, voContentField field);

  /**
   * Returns a text interval that corresponds to the specified text content
   * field.
   *
   * The interval surrounds the whole field: the begin position is zero and the
   * end position is the number of characters in the field.
   *
   * @param engine the engine.
   * @param target the target text inspector.
   * @param field a text content field.
   *
   * @return the newly created `voTextInterval` instance on success, otherwise
   *   `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voTextInspector`.
   * @throws VO_NO_SUCH_OBJECT when the `field` reference is invalid.
   * @throws VO_INVALID_OBJECT when `field` is not a `voContentField` of type
   *   `VO_CONTENT_FIELD_TEXT`.
   * @throws VO_INVALID_ARGUMENT when `field` comes from a different page.
   * @throws VO_INVALID_ARGUMENT when `field` refers to a content field that has
   *   been removed.
   * @throws VO_LIMIT_EXCEEDED when the number of `voTextInterval` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   */
  voTextInterval (VO_MSE_CALL *fromField)(voEngine engine, voTextInspector target,
                                          voContentField field);

  /**
   * Returns a text interval that corresponds to the specified selection.
   *
   * @note When the selection does not contain any recognized ink, `NULL` is
   *   returned without throwing error.
   *
   * @param engine the engine.
   * @param target the target text inspector.
   * @param selection the selection to evaluate.
   * @param field a text content field, or `NULL`.
   *
   * @return the text interval if any, otherwise `NULL`.
   *   When `NULL`, use `voGetError()` to figure out whether an error occurred.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voTextInspector`.
   * @throws VO_NO_SUCH_OBJECT when the `selection` reference is invalid.
   * @throws VO_INVALID_OBJECT when `selection` is not a `voPageSelection`
   *   or a `voInkSelection`.
   * @throws VO_INVALID_ARGUMENT when `selection` comes from a different page.
   * @throws VO_NO_SUCH_OBJECT when the `field` reference is invalid.
   * @throws VO_INVALID_OBJECT when `field` is not a `voContentField` of type
   *   `VO_CONTENT_FIELD_TEXT`.
   * @throws VO_INVALID_ARGUMENT when `field` comes from a different page.
   * @throws VO_INVALID_ARGUMENT when `field` refers to a content field that has
   *   been removed.
   * @throws VO_LIMIT_EXCEEDED when the number of `voTextInterval` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   */
  voTextInterval (VO_MSE_CALL *fromSelection)(voEngine engine, voTextInspector target,
                                              voEngineObject selection, voContentField field);

  /**
   * Expands the specified text interval using the specified limits.
   *
   * @param engine the engine.
   * @param target the target text inspector.
   * @param interval the text interval to be expanded.
   * @param beginBoundary the boundary to which the begin position should be
   *   expanded.
   * @param endBoundary the boundary to which the end position should be
   *   expanded.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voTextInspector`.
   * @throws VO_NO_SUCH_OBJECT when the `interval` reference is invalid.
   * @throws VO_INVALID_OPERATION when `interval` is not a `voTextInterval`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `beginBoundary` or `endBoundary` is
   *   out of bounds.
   * @throws VO_INVALID_ARGUMENT when `interval` refers to a content field that
   *   has been removed.
   */
  bool (VO_MSE_CALL *expand)(voEngine engine, voTextInspector target, voTextInterval interval,
                             voTextBoundary beginBoundary, voTextBoundary endBoundary);

  /**
   * Updates the specified page selection based on the specified text interval.
   *
   * @param engine the engine.
   * @param target the target text inspector.
   * @param interval the text interval defining the begin and end of selection.
   * @param selection the selection to be modified.
   * @param modifier the way the new selection should be combined with the
   *   existing selection.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voTextInspector`.
   * @throws VO_NO_SUCH_OBJECT when the `interval` reference is invalid.
   * @throws VO_INVALID_OPERATION when `interval` is not a `voTextInterval`.
   * @throws VO_INVALID_ARGUMENT when `interval` refers to a content field that
   *   has been removed.
   * @throws VO_NO_SUCH_OBJECT when the `selection` reference is invalid.
   * @throws VO_INVALID_OBJECT when `selection` is not a `voPageSelection`
   *   or a `voInkSelection`.
   * @throws VO_INVALID_ARGUMENT when `selection` comes from a different page.
   * @throws VO_INDEX_OUT_OF_BOUNDS when the `interval` begin or end position is
   *   out of bounds.
   * @throws VO_INVALID_ARGUMENT when `modifier` is invalid.
   */
  bool (VO_MSE_CALL *select)(voEngine engine, voTextInspector target,
                             voTextInterval interval, voEngineObject selection,
                             voSelectionModifier modifier);

  /**
   * Returns a segment iterator containing the character candidates associated
   * with the begin position of the specified text interval.
   *
   * @param engine the engine.
   * @param target the target text inspector.
   * @param cursor the cursor for which candidates are requested.
   *
   * @return the newly created `voSegmentIterator` instance on success,
   *   otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voTextInspector`.
   * @throws VO_NO_SUCH_OBJECT when the `cursor` reference is invalid.
   * @throws VO_INVALID_OBJECT when `cursor` is not a `voTextInterval`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when the `cursor` begin or end position is
   *   out of bounds.
   * @throws VO_INVALID_ARGUMENT when `cursor` refers to a content field that
   *   has been removed.
   * @throws VO_LIMIT_EXCEEDED when the number of `voSegmentIterator` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   */
  voIterator (VO_MSE_CALL *getCharSegment)(voEngine engine, voTextInspector target,
                                           voTextInterval cursor);

  /**
   * Returns a segment iterator containing the word candidates associated with
   * the begin position of the specified text interval.
   *
   * @param engine the engine.
   * @param target the target text inspector.
   * @param cursor the cursor for which candidates are requested.
   *
   * @return the newly created `voSegmentIterator` instance on success,
   *   otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voTextInspector`.
   * @throws VO_NO_SUCH_OBJECT when the `cursor` reference is invalid.
   * @throws VO_INVALID_OBJECT when `cursor` is not a `voTextInterval`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when the `cursor` begin or end position is
   *   out of bounds.
   * @throws VO_INVALID_ARGUMENT when `cursor` refers to a content field that
   *   has been removed.
   * @throws VO_LIMIT_EXCEEDED when the number of `voSegmentIterator` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   */
  voIterator (VO_MSE_CALL *getWordSegment)(voEngine engine, voTextInspector target,
                                           voTextInterval cursor);

  /**
   * Returns the label associated with the specified text interval.
   *
   * @param engine the engine.
   * @param target the target text inspector.
   * @param interval the interval for which label is requested.
   * @param charset the charset used to encode parameter strings.
   * @param string the recipient for the label.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voTextInspector`.
   * @throws VO_NO_SUCH_OBJECT when the `interval` reference is invalid.
   * @throws VO_INVALID_OBJECT when `interval` is not a `voTextInterval`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `string` or its `bytes` member is not
   *   writable.
   * @throws VO_INDEX_OUT_OF_BOUNDS when the `cursor` begin or end position is
   *   out of bounds.
   * @throws VO_INVALID_ARGUMENT when `interval` refers to a content field that
   *   has been removed.
   */
  bool (VO_MSE_CALL *getLabel)(voEngine engine, voTextInspector target,
                               voTextInterval interval,
                               voCharset charset, voString* string);

  /**
   * Returns the number of characters recognized in the specified text content
   * field.
   *
   * @param engine the engine.
   * @param target the target text inspector.
   * @param field the text field for which the information is requested.
   *
   * @return the number of characters on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voTextInspector`.
   * @throws VO_NO_SUCH_OBJECT when the `field` reference is invalid.
   * @throws VO_INVALID_OBJECT when `field` is not a `voContentField` of type
   *   `VO_CONTENT_FIELD_TEXT`.
   * @throws VO_INVALID_ARGUMENT when `field` comes from a different page.
   * @throws VO_INVALID_ARGUMENT when `field` refers to a content field that has
   *   been removed.
   */
  int32_t (VO_MSE_CALL *getCharCount)(voEngine engine, voTextInspector target, voContentField field);

  /**
   * Returns information associated with a character in the specified text
   * content field.
   *
   * @param engine the engine.
   * @param target the target text inspector.
   * @param field the text field for which the information is requested.
   * @param index the index of the character.
   * @param charInfo the recipient for the character information.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voTextInspector`.
   * @throws VO_NO_SUCH_OBJECT when the `field` reference is invalid.
   * @throws VO_INVALID_OBJECT when `field` is not a `voContentField` of type
   *   `VO_CONTENT_FIELD_TEXT`.
   * @throws VO_INVALID_ARGUMENT when `field` comes from a different page.
   * @throws VO_INVALID_ARGUMENT when `field` refers to a content field that has
   *   been removed.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_INVALID_USER_BUFFER when `charInfo` is not writable.
   */
  bool (VO_MSE_CALL *getCharAt)(voEngine engine, voTextInspector target, voContentField field,
                                int32_t index, voCharInfo* charInfo);

  /**
   * Returns the number of words recognized in the specified text content field.
   *
   * @param engine the engine.
   * @param target the target text inspector.
   * @param field the text field for which the information is requested.
   *
   * @return the number of words on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voTextInspector`.
   * @throws VO_NO_SUCH_OBJECT when the `field` reference is invalid.
   * @throws VO_INVALID_OBJECT when `field` is not a `voContentField` of type
   *   `VO_CONTENT_FIELD_TEXT`.
   * @throws VO_INVALID_ARGUMENT when `field` comes from a different page.
   * @throws VO_INVALID_ARGUMENT when `field` refers to a content field that has
   *   been removed.
   */
  int32_t (VO_MSE_CALL *getWordCount)(voEngine engine, voTextInspector target, voContentField field);

  /**
   * Returns information associated with a word in the specified text content
   * field.
   *
   * @param engine the engine.
   * @param target the target text inspector.
   * @param field the text field for which the information is requested.
   * @param index the index of the word.
   * @param wordInfo the recipient for the word information.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voTextInspector`.
   * @throws VO_NO_SUCH_OBJECT when the `field` reference is invalid.
   * @throws VO_INVALID_OBJECT when `field` is not a `voContentField` of type
   *   `VO_CONTENT_FIELD_TEXT`.
   * @throws VO_INVALID_ARGUMENT when `field` comes from a different page.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_INVALID_USER_BUFFER when `wordInfo` is not writable.
   * @throws VO_INVALID_ARGUMENT when `field` refers to a content field that has
   *   been removed.
   */
  bool (VO_MSE_CALL *getWordAt)(voEngine engine, voTextInspector target, voContentField field,
                                int32_t index, voWordInfo* wordInfo);

  /**
   * Returns the number of recognized lines in the specified text content field.
   *
   * @param engine the engine.
   * @param target the target text inspector.
   * @param field the text field for which the information is requested.
   *
   * @return the number of lines on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voTextInspector`.
   * @throws VO_NO_SUCH_OBJECT when the `field` reference is invalid.
   * @throws VO_INVALID_OBJECT when `field` is not a `voContentField` of type
   *   `VO_CONTENT_FIELD_TEXT`.
   * @throws VO_INVALID_ARGUMENT when `field` comes from a different page.
   * @throws VO_INVALID_ARGUMENT when `field` refers to a content field that has
   *   been removed.
   */
  int32_t (VO_MSE_CALL *getLineCount)(voEngine engine, voTextInspector target, voContentField field);

  /**
   * Returns information associated with a line in the specified text content
   * field.
   *
   * @param engine the engine.
   * @param target the target text inspector.
   * @param field the text field for which the information is requested.
   * @param index the index of the line.
   * @param lineInfo the recipient for the line information.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voTextInspector`.
   * @throws VO_NO_SUCH_OBJECT when the `field` reference is invalid.
   * @throws VO_INVALID_OBJECT when `field` is not a `voContentField` of type
   *   `VO_CONTENT_FIELD_TEXT`.
   * @throws VO_INVALID_ARGUMENT when `field` comes from a different page.
   * @throws VO_INVALID_ARGUMENT when `field` refers to a content field that has
   *   been removed.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_INVALID_USER_BUFFER when `lineInfo` is not writable.
   */
  bool (VO_MSE_CALL *getLineAt)(voEngine engine, voTextInspector target, voContentField field,
                                int32_t index, voLineInfo* lineInfo);
}
voITextInspector;

#endif // end of: #ifndef C_MYSCRIPTDOCUMENT_H
