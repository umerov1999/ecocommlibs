#ifndef C_MYSCRIPTANALYZER_H
#define C_MYSCRIPTANALYZER_H 0x08010000
/**
 * @file c/MyScriptAnalyzer.h
 * Native interface to the MyScript Analyzer service.
 */

#include "MyScriptEngine.h"
#include "MyScriptText.h"
#include "MyScriptShape.h"


// -- error codes --------------------------------------------------------------

/**
 * Lists the MyScript Analyzer errors.
 */
enum VO_ANALYZER_ERR
{
  /**
   * Unable to complete the requested operation because a `voAnalyzerKnowledge`
   * is missing.
   *
   * @see voAnalyzerKnowledge
   */
  VO_MISSING_ANALYZER_KNOWLEDGE = VO_INVALID_STATE | (6 << 16),
};


// -- types and interfaces -----------------------------------------------------

/**
 * Lists the MyScript Analyzer types.
 */
enum VO_ANALYZER_T
{
  /**
   * `voAnalyzer` type identifier.
   *
   * @see voAnalyzer
   */
  VO_Analyzer = 2000,

  /**
   * `voAnalyzerDocument` type identifier.
   *
   * @see voAnalyzerDocument
   */
  VO_AnalyzerDocument,

  /**
   * `voAnalyzerKnowledge` type identifier.
   *
   * @see voAnalyzerKnowledge
   */
  VO_AnalyzerKnowledge,

  /**
   * `voAnalyzerElement` type identifier.
   *
   * @see voAnalyzerElement
   */
  VO_AnalyzerElement,

  /**
   * `voAnalyzerUnderline` type identifier.
   *
   * @see voAnalyzerUnderline
   */
  VO_AnalyzerUnderline,

  /**
   * `voAnalyzerTextLine` type identifier.
   *
   * @see voAnalyzerTextLine
   */
  VO_AnalyzerTextLine,

  /**
   * `voAnalyzerLine` type identifier.
   *
   * @see voAnalyzerLine
   */
  VO_AnalyzerLine,

  /**
   * `voAnalyzerCell` type identifier.
   *
   * @see voAnalyzerCell
   */
  VO_AnalyzerCell,

  /**
   * `voAnalyzerTable` type identifier.
   *
   * @see voAnalyzerTable
   */
  VO_AnalyzerTable,

  /**
   * `voAnalyzerShape` type identifier.
   *
   * @see voAnalyzerShape
   */
  VO_AnalyzerShape,

  /**
   * `voAnalyzerGroup` type identifier.
   *
   * @see voAnalyzerGroup
   */
  VO_AnalyzerGroup
};

/**
 * Lists the MyScript Analyzer interfaces.
 */
enum VO_ANALYZER_I
{
  /**
   * `voIAnalyzerDocument` interface identifier.
   *
   * @see voIAnalyzerDocument
   */
  VO_IAnalyzerDocument = 2100,

  /**
   * `voIAnalyzerDocumentProcessor` interface identifier.
   *
   * @see voIAnalyzerDocumentProcessor
   */
  VO_IAnalyzerDocumentProcessor,

  /**
   * `voIAnalyzerUnderline` interface identifier.
   *
   * @see voIAnalyzerUnderline
   */
  VO_IAnalyzerUnderline,

  /**
   * `voIAnalyzerTextLine` interface identifier.
   *
   * @see voIAnalyzerTextLine
   */
  VO_IAnalyzerTextLine,

  /**
   * `voIAnalyzerLine` interface identifier.
   *
   * @see voIAnalyzerLine
   */
  VO_IAnalyzerLine,

  /**
   * `voIAnalyzerCell` interface identifier.
   *
   * @see voIAnalyzerCell
   */
  VO_IAnalyzerCell,

  /**
   * `voIAnalyzerTable` interface identifier.
   *
   * @see voIAnalyzerTable
   */
  VO_IAnalyzerTable,

  /**
   * `voIAnalyzerShape` interface identifier.
   *
   * @see voIAnalyzerShape
   */
  VO_IAnalyzerShape,

  /**
   * `voIAnalyzerGroup` interface identifier.
   *
   * @see voIAnalyzerGroup
   */
  VO_IAnalyzerGroup
};


// -----------------------------------------------------------------------------

/**
 * Lists the stroke types.
 */
typedef enum _voAnalyzerStrokeType
{
  VO_ANALYZER_STROKE_TYPE_ERR = -1,     /**< Error marker. */

  VO_ANALYZER_STROKE_TYPE_UNKNOWN,      /**< Unknown. */
  VO_ANALYZER_STROKE_TYPE_TEXT,         /**< Text. */
  VO_ANALYZER_STROKE_TYPE_NON_TEXT,     /**< Non text. */
  VO_ANALYZER_STROKE_TYPE_UNDERLINE,    /**< Underline. */
  VO_ANALYZER_STROKE_TYPE_TABLE,        /**< Table. */
  VO_ANALYZER_STROKE_TYPE_SHAPE,        /**< Shape. */
  VO_ANALYZER_STROKE_TYPE_CORRECTION,   /**< Correction. */
  VO_ANALYZER_STROKE_TYPE_ERASED_TEXT,  /**< Erased text. */

  VO_ANALYZER_STROKE_TYPE_COUNT,

  _voAnalyzerStrokeType_FORCE_INT_SIZE = 0x7FFFFFFF
}
voAnalyzerStrokeType;

/**
 * Lists the group types.
 */
typedef enum _voAnalyzerGroupType
{
  VO_ANALYZER_GROUP_TYPE_ERR = -1,              /**< Error marker. */

  VO_ANALYZER_GROUP_TYPE_PARAGRAPH,             /**< Paragraph. */
  VO_ANALYZER_GROUP_TYPE_LIST,                  /**< List. */
  VO_ANALYZER_GROUP_TYPE_TEXT_INSIDE_GRAPHICS,  /**< Text inside graphics. */

  VO_ANALYZER_GROUP_TYPE_COUNT,

  _voAnalyzerGroupType_FORCE_INT_SIZE = 0x7FFFFFFF
}
voAnalyzerGroupType;

/**
 * Lists the justification types.
 */
typedef enum _voAnalyzerJustificationType
{
  VO_ANALYZER_JUSTIFICATION_TYPE_LEFT_ALIGN,  /**< Left align. */
  VO_ANALYZER_JUSTIFICATION_TYPE_CENTER,      /**< Center. */

  VO_ANALYZER_JUSTIFICATION_TYPE_COUNT,

  _voAnalyzerJustificationType_TYPE_FORCE_INT_SIZE = 0x7FFFFFFF
}
voAnalyzerJustificationType;

/**
 * Represents a range in the input.
 */
typedef struct _voAnalyzerInkRange
{
  /**
   * Index of the stroke.
   */
  int32_t stroke;

  /**
   * Position of the first point within the stroke.
   */
  float firstPoint;

  /**
   * Position of the last point within the stroke.
   */
  float lastPoint;
}
voAnalyzerInkRange;

/**
 * Represents the location of a point in the 2D space.
 */
typedef struct _voAnalyzerPointData
{
  /**
   * Coordinate along the x axis.
   */
  float x;

  /**
   * Coordinate along the y axis.
   */
  float y;
}
voAnalyzerPointData;


// -- Analyzer -----------------------------------------------------------------

/**
 * `voAnalyzer` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voIAttachTarget, voIAnalyzerDocumentProcessor}
 */
typedef voEngineObject voAnalyzer;

/**
 * Lists the processing levels, each level including the previous levels.
 */
typedef enum _voAnalyzerProcessingLevel
{
  /**
   * Stroke type processing level: text/non-text processing.
   */
  VO_ANALYZER_PROCESSING_LEVEL_STROKE_TYPE = 1,

  /**
   * Layout processing level: text-lines extraction, implies
   * `VO_ANALYZER_PROCESSING_LEVEL_STROKE_TYPE`.
   */
  VO_ANALYZER_PROCESSING_LEVEL_TEXTLINE = 2,

  /**
   * Recognition processing level: shape/text recognition, implies
   * `VO_ANALYZER_PROCESSING_LEVEL_STROKE_TYPE` and
   * `VO_ANALYZER_PROCESSING_LEVEL_TEXTLINE`.
   */
  VO_ANALYZER_PROCESSING_LEVEL_RECOGNITION = 3,

  /**
   * Beautification processing level: layout beautification, implies
   * `VO_ANALYZER_PROCESSING_LEVEL_STROKE_TYPE` and
   * `VO_ANALYZER_PROCESSING_LEVEL_TEXTLINE` and
   * `VO_ANALYZER_PROCESSING_LEVEL_RECOGNITION`.
   */
  VO_ANALYZER_PROCESSING_LEVEL_ALL = 4,

  _voAnalyzerProcessingLevel_FORCE_INT_SIZE = 0x7FFFFFFF
}
voAnalyzerProcessingLevel;

/**
 * {@propertySet voAnalyzer}
 */
enum VO_ANALYZER_PROP
{
  /**
   * Adjusts the level of processing applied to the analyzer document.
   *
   * By default, the highest level of processing is applied.
   *
   * {@property voAnalyzerProcessingLevel}
   */
  VO_ANALYZER_PROCESSING_LEVEL = (VO_Analyzer << 16)
};

#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voAnalyzer` instance.
 *
 * @param engine the engine.
 *
 * @return the newly created `voAnalyzer` instance on success, otherwise `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_LIMIT_EXCEEDED when the number of `voAnalyzer` instances would
 *   exceed the allowed limit (please remember that the limit can be shared by
 *   several types).
 */
VO_INLINE voAnalyzer voCreateAnalyzer(voEngine engine)
{
  return voCreateObject(engine, VO_Analyzer);
}
#endif // end of: #ifndef VO_NO_FUNCTIONS


// -- AnalyzerElement ----------------------------------------------------------

/**
 * `voAnalyzerElement` reference type.
 *
 * {@extends voEngineObject}
 */
typedef voEngineObject voAnalyzerElement;

/**
 * {@typePropertySet AnalyzerElement}
 */
enum VO_ANALYZER_ELEMENT_TPROP
{
  /**
   * Represents the maximum number of `voAnalyzerElement` instances that can
   * exist at the same time in the user side of a MyScript enabled application.
   *
   * The special value zero means no limit. Other values are adjusted to the
   * next multiple of 16. A value under the current count is permitted and will
   * constrain future resource creations.
   *
   * {@property uint32_t}
   */
  VO_USER_ANALYZER_ELEMENT_LIMIT = VO_USER_OBJ_LIMIT,

  /**
   * Gives the number of `voAnalyzerElement` instances that currently exist in
   * the user side of a MyScript enabled application.
   *
   * {@property read-only uint32_t}
   */
  VO_USER_ANALYZER_ELEMENT_COUNT = VO_USER_OBJ_COUNT
};


// -- AnalyzerUnderline --------------------------------------------------------

/**
 * `voAnalyzerUnderline` reference type.
 *
 * {@extends voAnalyzerElement}
 * {@implements voIAnalyzerUnderline}
 */
typedef voAnalyzerElement voAnalyzerUnderline;

/**
 * Represents the data of an underline portion.
 *
 * @see voAnalyzerUnderline
 */
typedef struct _voAnalyzerUnderlineData
{
  /**
   * Position of the first underlined character.
   */
  int32_t firstCharacter;

  /**
   * Position of the last underlined character.
   */
  int32_t lastCharacter;
}
voAnalyzerUnderlineData;

/**
 * Functions composing the `voIAnalyzerUnderline` interface.
 *
 * This interface exposes the public methods of the `voAnalyzerUnderline` type.
 *
 * {@implementingTypes voAnalyzerUnderline}
 */
typedef struct _voIAnalyzerUnderline
{
  /**
   * Returns the data corresponding to this underline.
   *
   * @param engine the engine.
   * @param target the target analyzer underline.
   * @param data the recipient for the data.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voAnalyzerUnderline`.
   * @throws VO_INVALID_USER_BUFFER when `data` is not writable.
   */
  bool (VO_MSE_CALL *getData)(voEngine engine, voAnalyzerUnderline target, voAnalyzerUnderlineData* data);

  /**
   * Returns the number of ink ranges this underline covers.
   *
   * @param engine the engine.
   * @param target the target analyzer underline.
   *
   * @return the number of ink ranges on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voAnalyzerUnderline`.
   * @throws VO_INVALID_STATE when the portion of a MyScript Document associated
   *   with this underline has been modified, invalidating ink ranges.
   */
  int32_t (VO_MSE_CALL *getInkRangeCount)(voEngine engine, voAnalyzerUnderline target);

  /**
   * Returns the ink range at the specified index.
   *
   * @param engine the engine.
   * @param target the target analyzer underline.
   * @param index the index of the requested ink range.
   * @param range the recipient for the ink range.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voAnalyzerUnderline`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_INVALID_USER_BUFFER when `range` is not writable.
   * @throws VO_INVALID_STATE when the portion of a MyScript Document associated
   *   with this underline has been modified, invalidating ink ranges.
   */
  bool (VO_MSE_CALL *getInkRangeAt)(voEngine engine, voAnalyzerUnderline target, int32_t index, voAnalyzerInkRange* range);
}
voIAnalyzerUnderline;


// -- AnalyzerTextLine ---------------------------------------------------------

/**
 * `voAnalyzerTextLine` reference type.
 *
 * {@extends voAnalyzerElement}
 * {@implements voIAnalyzerTextLine}
 */
typedef voAnalyzerElement voAnalyzerTextLine;

/**
 * Represents the data of a text line portion.
 *
 * @see voAnalyzerTextLine
 */
typedef struct _voAnalyzerTextLineData
{
  /**
   * Top-left point composing the text line box.
   */
  voAnalyzerPointData topLeftPoint;

  /**
   * Width of the box.
   */
  float width;

  /**
   * Height of the box.
   */
  float height;

  /**
   * Orientation of the box (angle in radians).
   */
  float orientation;

  /**
   * Position of the baseline.
   */
  float baselinePos;

  /**
   * Height of the text.
   */
  float textHeight;

  /**
   * Type of justification.
   */
  voAnalyzerJustificationType justificationType;
}
voAnalyzerTextLineData;

/**
 * Functions composing the `voIAnalyzerTextLine` interface.
 *
 * This interface exposes the public methods of the `voAnalyzerTextLine` type.
 *
 * {@implementingTypes voAnalyzerTextLine}
 */
typedef struct _voIAnalyzerTextLine
{
  /**
   * Returns the data corresponding to this text line.
   *
   * @param engine the engine.
   * @param target the target analyzer text line.
   * @param data the recipient for the data.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voAnalyzerTextLine`.
   * @throws VO_INVALID_USER_BUFFER when `data` is not writable.
   */
  bool (VO_MSE_CALL *getData)(voEngine engine, voAnalyzerTextLine target, voAnalyzerTextLineData* data);

  /**
   * Returns the recognition result of the text line.
   *
   * @param engine the engine.
   * @param target the target analyzer text line.
   *
   * @return the recognition result on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voAnalyzerTextLine`.
   * @throws VO_LIMIT_EXCEEDED when the number of `voRecognitionResult`
   *   instances would exceed the allowed limit (please remember that the limit
   *   can be shared by several types).
   */
  voRecognitionResult (VO_MSE_CALL *getResult)(voEngine engine, voAnalyzerTextLine target);

  /**
   * Returns the number of underlines.
   *
   * @param engine the engine.
   * @param target the target analyzer text line.
   *
   * @return the number of underlines on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voAnalyzerTextLine`.
   */
  int32_t (VO_MSE_CALL *getUnderlineCount)(voEngine engine, voAnalyzerTextLine target);

  /**
   * Returns the underline at the specified index.
   *
   * @param engine the engine.
   * @param target the target analyzer text line.
   * @param index the index of the requested underline.
   *
   * @return the underline on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voAnalyzerTextLine`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_LIMIT_EXCEEDED when the number of `voAnalyzerUnderline`
   *   instances would exceed the allowed limit (please remember that the limit
   *   can be shared by several types).
   */
  voAnalyzerUnderline (VO_MSE_CALL *getUnderlineAt)(voEngine engine, voAnalyzerTextLine target, int32_t index);

  /**
   * Returns the number of ink ranges this text line covers.
   *
   * @param engine the engine.
   * @param target the target analyzer text line.
   *
   * @return the number of ink ranges on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voAnalyzerTextLine`.
   * @throws VO_INVALID_STATE when the portion of a MyScript Document associated
   *   with this text line has been modified, invalidating ink ranges.
   */
  int32_t (VO_MSE_CALL *getInkRangeCount)(voEngine engine, voAnalyzerTextLine target);

  /**
   * Returns the ink range at the specified index.
   *
   * @param engine the engine.
   * @param target the target analyzer text line.
   * @param index the index of the requested ink range.
   * @param range the recipient for the ink range.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voAnalyzerTextLine`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_INVALID_USER_BUFFER when `range` is not writable.
   * @throws VO_INVALID_STATE when the portion of a MyScript Document associated
   *   with this text line has been modified, invalidating ink ranges.
   */
  bool (VO_MSE_CALL *getInkRangeAt)(voEngine engine, voAnalyzerTextLine target, int32_t index, voAnalyzerInkRange* range);
}
voIAnalyzerTextLine;


// -- AnalyzerLine -------------------------------------------------------------

/**
 * `voAnalyzerLine` reference type.
 *
 * {@extends voAnalyzerElement}
 * {@implements voIAnalyzerLine}
 */
typedef voAnalyzerElement voAnalyzerLine;

/**
 * Represents the data of a 2D line segment.
 *
 * @see voAnalyzerLine
 */
typedef struct _voAnalyzerLineData
{
  /**
   * First point composing the line segment.
   */
  voAnalyzerPointData p1;

  /**
   * Second point composing the line segment.
   */
  voAnalyzerPointData p2;
}
voAnalyzerLineData;

/**
 * Functions composing the `voIAnalyzerLine` interface.
 *
 * This interface exposes the public methods of the `voAnalyzerLine` type.
 *
 * {@implementingTypes voAnalyzerLine}
 */
typedef struct _voIAnalyzerLine
{
  /**
   * Returns the data corresponding to this line.
   *
   * @param engine the engine.
   * @param target the target analyzer line.
   * @param data the recipient for the data.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voAnalyzerLine`.
   * @throws VO_INVALID_USER_BUFFER when `data` is not writable.
   */
  bool (VO_MSE_CALL *getData)(voEngine engine, voAnalyzerLine target, voAnalyzerLineData* data);
}
voIAnalyzerLine;


// -- AnalyzerCell -------------------------------------------------------------

/**
 * `voAnalyzerCell` reference type.
 *
 * {@extends voAnalyzerElement}
 * {@implements voIAnalyzerCell}
 */
typedef voAnalyzerElement voAnalyzerCell;

/**
 * Represents the range of a cell space.
 *
 * @see voAnalyzerCell
 */
typedef struct _voAnalyzerCellData
{
  /**
   * Position of the first row of the cell.
   */
  int32_t firstRow;

  /**
   * Position of the last row of the cell.
   */
  int32_t lastRow;

  /**
   * Position of the first column of the cell.
   */
  int32_t firstColumn;

  /**
   * Position of the last column of the cell.
   */
  int32_t lastColumn;

  /**
   * Top-Left point composing the cell box.
   */
  voAnalyzerPointData topLeftPoint;

  /**
   * Width of the cell box.
   */
  float width;

  /**
   * Height of the cell box.
   */
  float height;

  /**
   * Orientation of the cell box (angle in radians).
   */
  float orientation;

  /**
   * Tells if the cell has a left border.
   */
  bool leftBorder;

  /**
   * Tells if the cell has a top border.
   */
  bool topBorder;

  /**
   * Tells if the cell has a right border.
   */
  bool rightBorder;

  /**
   * Tells if the cell has a bottom border.
   */
  bool bottomBorder;
}
voAnalyzerCellData;

/**
 * Functions composing the `voIAnalyzerCell` interface.
 *
 * This interface exposes the public methods of the `voAnalyzerCell` type.
 *
 * {@implementingTypes voAnalyzerCell}
 */
typedef struct _voIAnalyzerCell
{
  /**
   * Returns the data corresponding to this cell.
   *
   * @param engine the engine.
   * @param target the target analyzer cell.
   * @param data the recipient for the data.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voAnalyzerCell`.
   * @throws VO_INVALID_USER_BUFFER when `data` is not writable.
   */
  bool (VO_MSE_CALL *getData)(voEngine engine, voAnalyzerCell target, voAnalyzerCellData* data);
}
voIAnalyzerCell;


// -- AnalyzerTable ------------------------------------------------------------

/**
 * `voAnalyzerTable` reference type.
 *
 * {@extends voAnalyzerElement}
 * {@implements voIAnalyzerTable}
 */
typedef voAnalyzerElement voAnalyzerTable;

/**
 * Represents the data of a table space.
 *
 * @see voAnalyzerTable
 */
typedef struct _voAnalyzerTableData
{
  /**
   * Number of rows composing the table.
   */
  int32_t rowCount;

  /**
   * Number of columns composing the table.
   */
  int32_t columnCount;
}
voAnalyzerTableData;

/**
 * Functions composing the `voIAnalyzerTable` interface.
 *
 * This interface exposes the public methods of the `voAnalyzerTable` type.
 *
 * {@implementingTypes voAnalyzerTable}
 */
typedef struct _voIAnalyzerTable
{
  /**
   * Returns the data corresponding to this table.
   *
   * @param engine the engine.
   * @param target the target analyzer table.
   * @param data the recipient for the data.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voAnalyzerTable`.
   * @throws VO_INVALID_USER_BUFFER when `data` is not writable.
   */
  bool (VO_MSE_CALL *getData)(voEngine engine, voAnalyzerTable target, voAnalyzerTableData* data);

  /**
   * Returns the number of ink ranges this table covers.
   *
   * @param engine the engine.
   * @param target the target analyzer table.
   *
   * @return the number of ink ranges on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voAnalyzerTable`.
   * @throws VO_INVALID_STATE when the portion of a MyScript Document associated
   *   with this table has been modified, invalidating ink ranges.
   */
  int32_t (VO_MSE_CALL *getInkRangeCount)(voEngine engine, voAnalyzerTable target);

  /**
   * Returns the ink range at the specified index.
   *
   * @param engine the engine.
   * @param target the target analyzer table.
   * @param index the index of the requested ink range.
   * @param range the recipient for the ink range.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voAnalyzerTable`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_INVALID_USER_BUFFER when `range` is not writable.
   * @throws VO_INVALID_STATE when the portion of a MyScript Document associated
   *   with this table has been modified, invalidating ink ranges.
   */
  bool (VO_MSE_CALL *getInkRangeAt)(voEngine engine, voAnalyzerTable target, int32_t index, voAnalyzerInkRange* range);

  /**
   * Returns the number of lines.
   *
   * @param engine the engine.
   * @param target the target analyzer table.
   *
   * @return the number of lines on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voAnalyzerTable`.
   */
  int32_t (VO_MSE_CALL *getLineCount)(voEngine engine, voAnalyzerTable target);

  /**
   * Returns the line at the specified index.
   *
   * @param engine the engine.
   * @param target the target analyzer table.
   * @param index the index of the requested line.
   *
   * @return the line on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voAnalyzerTable`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_LIMIT_EXCEEDED when the number of `voAnalyzerLine` instances
   *   would exceed the allowed limit (please remember that the limit
   *   can be shared by several types).
   */
  voAnalyzerLine (VO_MSE_CALL *getLineAt)(voEngine engine, voAnalyzerTable target, int32_t index);

  /**
   * Returns the number of cells.
   *
   * @param engine the engine.
   * @param target the target analyzer table.
   *
   * @return the number of cells on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voAnalyzerTable`.
   */
  int32_t (VO_MSE_CALL *getCellCount)(voEngine engine, voAnalyzerTable target);

  /**
   * Returns the cell at the specified index.
   *
   * @param engine the engine.
   * @param target the target analyzer table.
   * @param index the index of the requested cell.
   *
   * @return the cell on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voAnalyzerTable`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_LIMIT_EXCEEDED when the number of `voAnalyzerCell` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   */
  voAnalyzerCell (VO_MSE_CALL *getCellAt)(voEngine engine, voAnalyzerTable target, int32_t index);
}
voIAnalyzerTable;


// -- AnalyzerShape ------------------------------------------------------------

/**
 * `voAnalyzerShape` reference type.
 *
 * {@extends voAnalyzerElement}
 * {@implements voIAnalyzerShape}
 */
typedef voAnalyzerElement voAnalyzerShape;

/**
 * Functions composing the `voIAnalyzerShape` interface.
 *
 * This interface exposes the public methods of the `voAnalyzerShape` type.
 *
 * {@implementingTypes voAnalyzerShape}
 */
typedef struct _voIAnalyzerShape
{
  /**
   * Returns the shape segment.
   *
   * @param engine the engine.
   * @param target the target analyzer shape.
   *
   * @return the shape segment on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voAnalyzerShape`.
   * @throws VO_LIMIT_EXCEEDED when the number of `voShapeSegment` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   */
  voShapeSegment (VO_MSE_CALL *getSegment)(voEngine engine, voAnalyzerShape target);
}
voIAnalyzerShape;


// -- AnalyzerGroup ------------------------------------------------------------

/**
 * `voAnalyzerGroup` reference type.
 *
 * {@extends voAnalyzerElement}
 * {@implements voIAnalyzerGroup}
 */
typedef voAnalyzerElement voAnalyzerGroup;

/**
 * Functions composing the `voIAnalyzerGroup` interface.
 *
 * This interface exposes the public methods of the `voAnalyzerGroup` type.
 *
 * {@implementingTypes voAnalyzerGroup}
 */
typedef struct _voIAnalyzerGroup
{
  /**
   * Returns the type of that group.
   *
   * @param engine the engine.
   * @param target the target analyzer group.
   *
   * @return the type of group on success, otherwise
   *   `VO_ANALYZER_GROUP_TYPE_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voAnalyzerGroup`.
   */
  voAnalyzerGroupType (VO_MSE_CALL *getType)(voEngine engine, voAnalyzerGroup target);

  /**
   * Returns the number of elements in the group.
   *
   * @param engine the engine.
   * @param target the target analyzer group.
   *
   * @return the number of elements on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voAnalyzerGroup`.
   */
  int32_t (VO_MSE_CALL *getElementCount)(voEngine engine, voAnalyzerGroup target);

  /**
   * Returns the element at the specified index.
   *
   * @param engine the engine.
   * @param target the target analyzer underline.
   * @param index the index of the requested element.
   *
   * @return the element on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voAnalyzerGroup`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_LIMIT_EXCEEDED when the number of `voAnalyzerElement` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   */
  voAnalyzerElement (VO_MSE_CALL *getElementAt)(voEngine engine, voAnalyzerGroup target, int32_t index);
}
voIAnalyzerGroup;


// -- AnalyzerDocument ---------------------------------------------------------

/**
 * `voAnalyzerDocument` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voIAnalyzerDocument}
 */
typedef voEngineObject voAnalyzerDocument;

/**
 * Functions composing the `voIAnalyzerDocument` interface.
 *
 * This interface exposes the public methods of the `voAnalyzerDocument` type.
 *
 * {@implementingTypes voAnalyzerDocument}
 */
typedef struct _voIAnalyzerDocument
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
   * @throws VO_INVALID_OPERATION when `target` is not a `voAnalyzerDocument`.
   */
  bool (VO_MSE_CALL *clear)(voEngine engine, voAnalyzerDocument target);

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
   * @throws VO_INVALID_OPERATION when `target` is not a `voAnalyzerDocument`.
   * @throws VO_INVALID_ARGUMENT when `xByteStride == 0`.
   * @throws VO_INVALID_ARGUMENT when `yByteStride == 0`.
   * @throws VO_INVALID_ARGUMENT when `pointCount <= 0`.
   * @throws VO_INVALID_USER_BUFFER when the combination of `x`, `xByteStride`,
   *   `y`, `yByteStride` and `pointCount` leads to invalid memory access.
   * @throws VO_LIMIT_EXCEEDED when `pointCount > 32767`.
   */
  bool (VO_MSE_CALL *addStroke)(voEngine engine, voAnalyzerDocument target, const float* x, size_t xByteStride, const float* y, size_t yByteStride, int32_t pointCount);

  /**
   * Returns the number of strokes in this document.
   *
   * @param engine the engine.
   * @param target the target document.
   *
   * @return the number of strokes on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voAnalyzerDocument`.
   */
  int32_t (VO_MSE_CALL *getStrokeCount)(voEngine engine, voAnalyzerDocument target);

  /**
   * Returns the points of the stroke at the specified index.
   *
   * @param engine the engine.
   * @param target the target document.
   * @param index the index of the requested stroke.
   * @param x the recipient for the x values.
   * @param xByteStride the stride in bytes between each x value.
   * @param y the recipient for the y values.
   * @param yByteStride the stride in bytes between each y value.
   * @param pointCount on input, the capacity of the `x` and `y` recipients, on
   *   output, the recipient for the number of points in the stroke.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voAnalyzerDocument`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_INVALID_USER_BUFFER when `pointCount` is not readable or
   *   writable.
   * @throws VO_INVALID_ARGUMENT when `x` or `y` is not `NULL` and
   *   `pointCount <= 0`
   * @throws VO_INVALID_USER_BUFFER when the combination of `x`, `xByteStride`,
   *   `y`, `yByteStride` and `pointCount` leads to invalid memory access.
   *
   * @note You can pass `NULL` in place of `x` and `y` to retrieve the number of
   *   points in the stroke.
   */
  bool (VO_MSE_CALL *getStrokeAt)(voEngine engine, voAnalyzerDocument target, int32_t index, float* x, size_t xByteStride, float* y, size_t yByteStride, int32_t* pointCount);

  /**
   * Returns the type of the stroke at the specified index.
   *
   * @param engine the engine.
   * @param target the target document.
   * @param index the index of the stroke.
   *
   * @return the type of the stroke on success, otherwise
   *   `VO_ANALYZER_STROKE_TYPE_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voAnalyzerDocument`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   */
  voAnalyzerStrokeType (VO_MSE_CALL *getStrokeTypeAt)(voEngine engine, voAnalyzerDocument target, int32_t index);

  /**
   * Returns the confidence score of the stroke at the specified index.
   *
   * @param engine the engine.
   * @param target the target document.
   * @param index the index of the stroke.
   *
   * @return the confidence score of the stroke on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voAnalyzerDocument`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   */
  float (VO_MSE_CALL *getStrokeConfidenceAt)(voEngine engine, voAnalyzerDocument target, int32_t index);

  /**
   * Returns the number of text lines.
   *
   * @param engine the engine.
   * @param target the target document.
   *
   * @return the number of text lines on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voAnalyzerDocument`.
   */
  int32_t (VO_MSE_CALL *getTextLineCount)(voEngine engine, voAnalyzerDocument target);

  /**
   * Returns the text line at the specified index.
   *
   * @param engine the engine.
   * @param target the target document.
   * @param index the index of the requested text line.
   *
   * @return the text line on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voAnalyzerDocument`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_LIMIT_EXCEEDED when the number of `voAnalyzerTextLine` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   */
  voAnalyzerTextLine (VO_MSE_CALL *getTextLineAt)(voEngine engine, voAnalyzerDocument target, int32_t index);

  /**
   * Returns the number of tables.
   *
   * @param engine the engine.
   * @param target the target document.
   *
   * @return the number of tables on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voAnalyzerDocument`.
   */
  int32_t (VO_MSE_CALL *getTableCount)(voEngine engine, voAnalyzerDocument target);

  /**
   * Returns the table at the specified index.
   *
   * @param engine the engine.
   * @param target the target document.
   * @param index the index of the requested table.
   *
   * @return the table on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voAnalyzerDocument`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_LIMIT_EXCEEDED when the number of `voAnalyzerTable` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   */
  voAnalyzerTable (VO_MSE_CALL *getTableAt)(voEngine engine, voAnalyzerDocument target, int32_t index);

  /**
   * Returns the number of shapes.
   *
   * @param engine the engine.
   * @param target the target document.
   *
   * @return the number of shapes on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voAnalyzerDocument`.
   */
  int32_t (VO_MSE_CALL *getShapeCount)(voEngine engine, voAnalyzerDocument target);

  /**
   * Returns the shape at the specified index.
   *
   * @param engine the engine.
   * @param target the target document.
   * @param index the index of the requested shape.
   *
   * @return the shape on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voAnalyzerDocument`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_LIMIT_EXCEEDED when the number of `voAnalyzerShape` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   */
  voAnalyzerShape (VO_MSE_CALL *getShapeAt)(voEngine engine, voAnalyzerDocument target, int32_t index);

  /**
   * Returns the number of groups.
   *
   * @param engine the engine.
   * @param target the target document.
   *
   * @return the number of groups on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voAnalyzerDocument`.
   */
  int32_t (VO_MSE_CALL *getGroupCount)(voEngine engine, voAnalyzerDocument target);

  /**
   * Returns the group at the specified index.
   *
   * @param engine the engine.
   * @param target the target document.
   * @param index the index of the requested group.
   *
   * @return the group on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voAnalyzerDocument`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_LIMIT_EXCEEDED when the number of `voAnalyzerGroup` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   */
  voAnalyzerGroup (VO_MSE_CALL *getGroupAt)(voEngine engine, voAnalyzerDocument target, int32_t index);

  /**
   * Sets the resolution of the coordinates in dpi. This is the resolution of
   * the grid of integer values in the chosen coordinate system. For example,
   * if you use floating-point pixel coordinates you must give the screen
   * resolution, no matter the resolution of the capturing device.
   *
   * @param engine the engine.
   * @param target the target document.
   * @param resolution the coordinate resolution.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voAnalyzerDocument`.
   * @throws VO_INVALID_ARGUMENT when `resolution` is not strictly positive.
   */
  bool (VO_MSE_CALL *setCoordinateResolution)(voEngine engine, voAnalyzerDocument target, float resolution);
}
voIAnalyzerDocument;

#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voAnalyzerDocument` instance.
 *
 * @param engine the engine.
 *
 * @return the newly created `voAnalyzerDocument` instance on success, otherwise
 *   `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_LIMIT_EXCEEDED when the number of `voAnalyzerDocument` instances
 *   would exceed the allowed limit (please remember that the limit can be
 *   shared by several types).
 */
VO_INLINE voAnalyzerDocument voCreateAnalyzerDocument(voEngine engine)
{
  return voCreateObject(engine, VO_AnalyzerDocument);
}
#endif // end of: #ifndef VO_NO_FUNCTIONS


// -- IAnalyzerDocumentProcessor -----------------------------------------------

/**
 * Functions composing the `voIAnalyzerDocumentProcessor` interface.
 *
 * {@implementingTypes voAnalyzer}
 */
typedef struct _voIAnalyzerDocumentProcessor
{
  /**
   * Processes an analyzer document.
   *
   * @param engine the engine.
   * @param target the target processor.
   * @param document the analyzer document to be processed.
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
   * @throws VO_NO_SUCH_OBJECT when the `document` reference is invalid.
   * @throws VO_INVALID_OBJECT when `document` is not a `voAnalyzerDocument`.
   * @throws VO_INVALID_CALLBACK when `notifyProgressFunc` is invalid.
   * @throws VO_INVALID_STATE when the `target` object is not in a state where
   *   the `process` function can be invoked.
   * @throws VO_MISSING_ANALYZER_KNOWLEDGE when `recognizer` has no attached
   *   `voAnalyzerKnowledge`.
   * @throws VO_CANCELED when the operation has been canceled.
   *
   * @see voAnalyzerDocument
   */
  bool (VO_MSE_CALL *process)(voEngine engine, voEngineObject target, voAnalyzerDocument document, voNotifyProgressFunc notifyProgressFunc, void* userParam);
}
voIAnalyzerDocumentProcessor;


// -- AnalyzerKnowledge --------------------------------------------------------

/**
 * `voAnalyzerKnowledge` reference type.
 *
 * {@extends voEngineObject}
 */
typedef voEngineObject voAnalyzerKnowledge;

#endif // end of: #ifndef C_MYSCRIPTANALYZER_H
