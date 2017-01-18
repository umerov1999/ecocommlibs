#ifndef C_MYSCRIPTSHAPE_H
#define C_MYSCRIPTSHAPE_H 0x08000000
/**
 * @file c/MyScriptShape.h
 * Native interface to the MyScript Shape service.
 */

#include "MyScriptEngine.h"


// -- error codes --------------------------------------------------------------

/**
 * Lists the MyScript Shape errors.
 */
enum VO_SHAPE_ERR
{
  /**
   * Unable to complete the requested operation because a `voShapeKnowledge` is
   * missing.
   *
   * @see voShapeKnowledge
   */
  VO_MISSING_SHAPE_KNOWLEDGE = VO_INVALID_STATE | (4 << 16),

  /**
   * The `endSegment()` call does not match any prior `startSegment()` call.
   */
  VO_UNMATCHED_SHAPE_SEGMENT
};


// -- types and interfaces -----------------------------------------------------

/**
 * Lists the MyScript Shape types.
 */
enum VO_SHAPE_T
{
  /**
   * `voShapeModel` type identifier.
   *
   * @see voShapeModel
   */
  VO_ShapeModel = 4000,

  /**
   * `voShapeKnowledge` type identifier.
   *
   * @see voShapeKnowledge
   */
  VO_ShapeKnowledge,

  /**
   * `voShapeRecognizer` type identifier.
   *
   * @see voShapeRecognizer
   */
  VO_ShapeRecognizer,

  /**
   * `voShapeBeautifier` type identifier.
   *
   * @see voShapeBeautifier
   */
  VO_ShapeBeautifier,

  /**
   * `voShapeDocument` type identifier.
   *
   * @see voShapeDocument
   */
  VO_ShapeDocument,

  /**
   * `voShapePrimitive` type identifier.
   *
   * @see voShapePrimitive
   */
  VO_ShapePrimitive,

  /**
   * `voShapeLine` type identifier.
   *
   * @see voShapeLine
   */
  VO_ShapeLine,

  /**
   * `voShapeDecoratedLine` type identifier.
   *
   * @see voShapeDecoratedLine
   */
  VO_ShapeDecoratedLine,

  /**
   * `voShapeEllipticArc` type identifier.
   *
   * @see voShapeEllipticArc
   */
  VO_ShapeEllipticArc,

  /**
   * `voShapeDecoratedEllipticArc` type identifier.
   *
   * @see voShapeDecoratedEllipticArc
   */
  VO_ShapeDecoratedEllipticArc,

  /**
   * `voShapeSegment` type identifier.
   *
   * @see voShapeSegment
   */
  VO_ShapeSegment,

  /**
   * `voShapeCandidate` type identifier.
   *
   * @see voShapeCandidate
   */
  VO_ShapeCandidate,

  /**
   * `voShapeRecognized` type identifier.
   *
   * @see voShapeRecognized
   */
  VO_ShapeRecognized,

  /**
   * `voShapeScratchOut` type identifier.
   *
   * @see voShapeScratchOut
   */
  VO_ShapeScratchOut,

  /**
   * `voShapeErased` type identifier.
   *
   * @see voShapeErased
   */
  VO_ShapeErased,

  /**
   * `voShapeRejected` type identifier.
   *
   * @see voShapeRejected
   */
  VO_ShapeRejected,

  /**
   * `voShapeHistoryManager` type identifier.
   *
   * @see voShapeHistoryManager
   */
  VO_ShapeHistoryManager = 4029
};

/**
 * Lists the MyScript Shape interfaces.
 */
enum VO_SHAPE_I
{
  /**
   * `voIShapeModel` interface identifier.
   *
   * @see voIShapeModel
   */
  VO_IShapeModel = 4100,

  /**
   * `voIShapeKnowledge` interface identifier.
   *
   * @see voIShapeKnowledge
   */
  VO_IShapeKnowledge,

  /**
   * `voIShapeRecognizer` interface identifier.
   *
   * @see voIShapeRecognizer
   */
  VO_IShapeRecognizer,

  /**
   * `voIShapeDocument` interface identifier.
   *
   * @see voIShapeDocument
   */
  VO_IShapeDocument,

  /**
   * `voIShapeLine` interface identifier.
   *
   * @see voIShapeLine
   */
  VO_IShapeLine,

  /**
   * `voIShapeDecoratedLine` interface identifier.
   *
   * @see voIShapeDecoratedLine
   */
  VO_IShapeDecoratedLine,

  /**
   * `voIShapeEllipticArc` interface identifier.
   *
   * @see voIShapeEllipticArc
   */
  VO_IShapeEllipticArc,

  /**
   * `voIShapeDecoratedEllipticArc` interface identifier.
   *
   * @see voIShapeDecoratedEllipticArc
   */
  VO_IShapeDecoratedEllipticArc,

  /**
   * `voIShapeSegment` interface identifier.
   *
   * @see voIShapeSegment
   */
  VO_IShapeSegment,

  /**
   * `voIShapeRecognized` interface identifier.
   *
   * @see voIShapeRecognized
   */
  VO_IShapeRecognized,

  /**
   * `voIShapeScratchOut` interface identifier.
   *
   * @see voIShapeScratchOut
   */
  VO_IShapeScratchOut,

  /**
   * `voIShapeDocumentProcessor` interface identifier.
   *
   * @see voIShapeDocumentProcessor
   */
  VO_IShapeDocumentProcessor,

  /**
   * `voIShapeHistoryManager` interface identifier.
   *
   * @see voIShapeHistoryManager
   */
  VO_IShapeHistoryManager,

  /**
   * `voIShapePrimitive` interface identifier.
   *
   * @see voIShapePrimitive
   */
  VO_IShapePrimitive
};


// -- ShapeModel ---------------------------------------------------------------

/**
 * `voShapeModel` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voIShapeModel}
 */
typedef voEngineObject voShapeModel;

/**
 * Functions composing the `voIShapeModel` interface.
 *
 * This interface exposes the public methods of the `voShapeModel` type.
 *
 * {@implementingTypes voShapeModel}
 */
typedef struct _voIShapeModel
{
  /**
   * Returns the label of this shape model.
   *
   * @param engine the engine.
   * @param target the target shape model.
   * @param charset the charset used to encode parameter strings.
   * @param label the recipient for the label.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voShapeModel`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `label` or its `bytes` member is not
   *   writable.
   */
  bool (VO_MSE_CALL *getLabel)(voEngine engine, voShapeModel target, voCharset charset, voString* label);
}
voIShapeModel;


// -- ShapeKnowledge -----------------------------------------------------------

/**
 * `voShapeKnowledge` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voIShapeKnowledge}
 */
typedef voEngineObject voShapeKnowledge;

/**
 * Functions composing the `voIShapeKnowledge` interface.
 *
 * This interface exposes the public methods of the `voShapeKnowledge` type.
 *
 * {@implementingTypes voShapeKnowledge}
 */
typedef struct _voIShapeKnowledge
{
  /**
   * Returns the number of models in this shape knowledge.
   *
   * @param engine the engine.
   * @param target the target shape knowledge.
   *
   * @return the number of models on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voShapeKnowledge`.
   */
  int32_t (VO_MSE_CALL *getModelCount)(voEngine engine, voShapeKnowledge target);

  /**
   * Returns the model at the specified index.
   *
   * @param engine the engine.
   * @param target the target shape knowledge.
   * @param index the index of the model to be retrieved.
   *
   * @return the model on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voShapeKnowledge`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_LIMIT_EXCEEDED when the number of `voShapeModel` instances would
   *   exceed the allowed limit (please remember that the limit can be shared by
   *   several types).
   */
  voShapeModel (VO_MSE_CALL *getModelAt)(voEngine engine, voShapeKnowledge target, int32_t index);
}
voIShapeKnowledge;


// -- ShapeRecognizer ----------------------------------------------------------

/**
 * `voShapeRecognizer` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voIAttachTarget, voIShapeDocumentProcessor, voIShapeRecognizer}
 */
typedef voEngineObject voShapeRecognizer;

/**
 * {@propertySet ShapeRecognizer}
 */
enum VO_SHAPE_RECOGNIZER_PROP
{
  /**
   * A value between `0.0f` and `1.0f` that adjusts the reject detection
   * sensitivity.
   *
   * {@property float}
   *
   * @note Even with a value equals to `0.0f`, rejected segments may be provided
   *   when no other shape candidate is recognized.
   */
  VO_SHAPE_RECOGNIZER_REJECT_DETECTION_SENSITIVITY = (VO_ShapeRecognizer << 16),

  /**
   * A value between `0.0f` and `1.0f` that adjusts the scratch out detection
   * sensitivity.
   *
   * {@property float}
   *
   * @note A value equal to `0.0f` disables scratch out detection.
   */
  VO_SHAPE_RECOGNIZER_SCRATCH_OUT_DETECTION_SENSITIVITY
};

/**
 * Functions composing the `voIShapeRecognizer` interface.
 *
 * This interface exposes the public methods of the `voShapeRecognizer` type.
 *
 * {@implementingTypes voShapeRecognizer}
 */
typedef struct _voIShapeRecognizer
{
  /**
   * Returns the number of models in the shape knowledge attached to this
   * recognizer.
   *
   * @param engine the engine.
   * @param target the target recognizer.
   *
   * @return the number of models on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voShapeRecognizer`.
   * @throws VO_MISSING_SHAPE_KNOWLEDGE when `target` has no shape knowledge
   *   attached.
   */
  int32_t (VO_MSE_CALL *getModelCount)(voEngine engine, voShapeRecognizer target);

  /**
   * Returns the model at the specified index in the shape knowledge attached
   * to this recognizer.
   *
   * @param engine the engine.
   * @param target the target recognizer.
   * @param index the index of the model to be retrieved.
   *
   * @return the model on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voShapeRecognizer`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_LIMIT_EXCEEDED when the number of `voShapeModel` instances would
   *   exceed the allowed limit (please remember that the limit can be shared by
   *   several types).
   * @throws VO_MISSING_SHAPE_KNOWLEDGE when `target` has no shape knowledge
   *   attached.
   */
  voShapeModel (VO_MSE_CALL *getModelAt)(voEngine engine, voShapeRecognizer target, int32_t index);

  /**
   * Returns the detection sensitivity associated with the specified model.
   *
   * @param engine the engine.
   * @param target the target recognizer.
   * @param model the target shape model.
   *
   * @return the detection sensitivity on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voShapeRecognizer`.
   * @throws VO_NO_SUCH_OBJECT when the `model` reference is invalid.
   * @throws VO_INVALID_OBJECT when `model` is not a `voShapeModel`.
   * @throws VO_INVALID_ARGUMENT when `model` does not belong to the shape
   *   knowledge that is attached to `target`.
   * @throws VO_MISSING_SHAPE_KNOWLEDGE when `target` has no shape knowledge
   *   attached.
   */
  float (VO_MSE_CALL *getModelDetectionSensitivity)(voEngine engine, voShapeRecognizer target, voShapeModel model);

  /**
   * Sets the detection sensitivity associated with the specified model.
   *
   * @param engine the engine.
   * @param target the target recognizer.
   * @param model the target shape model.
   * @param sensitivity the detection sensitivity for the specified model.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voShapeRecognizer`.
   * @throws VO_NO_SUCH_OBJECT when the `model` reference is invalid.
   * @throws VO_INVALID_OBJECT when `model` is not a `voShapeModel`.
   * @throws VO_INVALID_ARGUMENT when `model` does not belong to the shape
   *   knowledge that is attached to `target`.
   * @throws VO_INVALID_ARGUMENT when `sensitivity` is not between `0.0f` and
   *   `1.0f`.
   * @throws VO_MISSING_SHAPE_KNOWLEDGE when `target` has no shape knowledge
   *   attached.
   */
  bool (VO_MSE_CALL *setModelDetectionSensitivity)(voEngine engine, voShapeRecognizer target, voShapeModel model, float sensitivity);
}
voIShapeRecognizer;

#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voShapeRecognizer` instance.
 *
 * @param engine the engine.
 *
 * @return the newly created `voShapeRecognizer` instance on success, otherwise
 *   `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_LIMIT_EXCEEDED when the number of `voShapeRecognizer` instances
 *   would exceed the allowed limit (please remember that the limit can be
 *   shared by several types).
 */
VO_INLINE voShapeRecognizer voCreateShapeRecognizer(voEngine engine)
{
  return voCreateObject(engine, VO_ShapeRecognizer);
}
#endif // end of: #ifndef VO_NO_FUNCTIONS


// -- ShapeBeautifier ----------------------------------------------------------

/**
 * `voShapeBeautifier` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voIAttachTarget, voIShapeDocumentProcessor}
 */
typedef voEngineObject voShapeBeautifier;

/**
 * {@propertySet ShapeBeautifier}
 */
enum VO_SHAPE_BEAUTIFIER_PROP
{
  /**
   * A value between `0.0f` and `1.0f` that adjusts the beautification alignment
   * strength of the beautifier.
   *
   * {@property float}
   *
   * @note A value equal to `0.0f` disables the alignment constraints of the
   * beautification process. A value close to `1.0f` increases the likelihood of
   * shapes being aligned with the vertical and horizontal document axis.
   */
  VO_SHAPE_BEAUTIFIER_ALIGNMENT_STRENGTH = (VO_ShapeBeautifier << 16),

  /**
   * A value between `0.0f` and `1.0f` that adjusts the beautification
   * congruence strength of the beautifier.
   *
   * {@property float}
   *
   * @note A value equal to `0.0f` disables the congruence constraints of the
   *   beautification process. A value close to `1.0f` increases the likelihood
   *   of shapes being of the same size and aligned with each other.
   */
  VO_SHAPE_BEAUTIFIER_CONGRUENCE_STRENGTH,

  /**
   * A value between `0.0f` and `1.0f` that adjusts the beautification
   * connection strength of the beautifier.
   *
   * {@property float}
   *
   * @note A value equal to `0.0f` disables the connection constraints of the
   *   beautification process. A value close to `1.0f` increases the likelihood
   *   of shapes being connected.
   */
  VO_SHAPE_BEAUTIFIER_CONNECTION_STRENGTH
};

#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voShapeBeautifier` instance.
 *
 * @param engine the engine.
 *
 * @return the newly created `voShapeBeautifier` instance on success, otherwise
 *   `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_LIMIT_EXCEEDED when the number of `voShapeBeautifier` instances
 *   would exceed the allowed limit (please remember that the limit can be
 *   shared by several types).
 */
VO_INLINE voShapeBeautifier voCreateShapeBeautifier(voEngine engine)
{
  return voCreateObject(engine, VO_ShapeBeautifier);
}
#endif // end of: #ifndef VO_NO_FUNCTIONS


// -- ShapeCandidate -----------------------------------------------------------

/**
 * `voShapeCandidate` reference type.
 *
 * {@extends voEngineObject}
 */
typedef voEngineObject voShapeCandidate;

#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voShapeCandidate` instance.
 *
 * @param engine the engine.
 *
 * @return the newly created `voShapeCandidate` instance on success, otherwise
 *   `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_LIMIT_EXCEEDED when the number of `voShapeCandidate` instances
 *   would exceed the allowed limit (please remember that the limit can be
 *   shared by several types).
 */
VO_INLINE voShapeCandidate voCreateShapeCandidate(voEngine engine)
{
  return voCreateObject(engine, VO_ShapeCandidate);
}
#endif // end of: #ifndef VO_NO_FUNCTIONS


// -- ShapeInkRange ------------------------------------------------------------

/**
 * Represents a range in the input.
 */
typedef struct _voShapeInkRange
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
voShapeInkRange;


// -- ShapeSegment -------------------------------------------------------------

/**
 * `voShapeSegment` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voIShapeSegment}
 */
typedef voEngineObject voShapeSegment;

/**
 * {@propertySet ShapeSegment}
 */
enum VO_SHAPE_SEGMENT_PROP
{
  /**
   * Freezes the recognition process for this segment.
   *
   * {@property bool}
   */
  VO_SHAPE_SEGMENT_FREEZE_RECOGNITION = (VO_ShapeSegment << 16),

  /**
   * Freezes the beautification process for this segment.
   *
   * {@property bool}
   */
  VO_SHAPE_SEGMENT_FREEZE_BEAUTIFICATION
};

/**
 * Functions composing the `voIShapeSegment` interface.
 *
 * This interface exposes the public methods of the `voShapeSegment` type.
 *
 * {@implementingTypes voShapeSegment}
 */
typedef struct _voIShapeSegment
{
  /**
   * Returns the number of ink ranges this segment covers.
   *
   * @param engine the engine.
   * @param target the target segment.
   *
   * @return the number of ink ranges on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voShapeSegment`.
   * @throws VO_INVALID_STATE when the portion of a MyScript Document associated
   *   with this segment has been modified, invalidating ink ranges.
   */
  int32_t (VO_MSE_CALL *getInkRangeCount)(voEngine engine, voShapeSegment target);

  /**
   * Returns the ink range at the specified index.
   *
   * @param engine the engine.
   * @param target the target segment.
   * @param index the index of the requested ink range.
   * @param range the recipient for the ink range.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voShapeSegment`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_INVALID_USER_BUFFER when `range` is not writable.
   * @throws VO_INVALID_STATE when the portion of a MyScript Document associated
   *   with this segment has been modified, invalidating ink ranges.
   */
  bool (VO_MSE_CALL *getInkRangeAt)(voEngine engine, voShapeSegment target, int32_t index, voShapeInkRange* range);

  /**
   * Returns the number of candidates this segment contains.
   *
   * @param engine the engine.
   * @param target the target segment.
   *
   * @return the number of candidates on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voShapeSegment`.
   */
  int32_t (VO_MSE_CALL *getCandidateCount)(voEngine engine, voShapeSegment target);

  /**
   * Returns the candidate at the specified index.
   *
   * @param engine the engine.
   * @param target the target segment.
   * @param index the index of the candidate to be retrieved.
   *
   * @return the candidate on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voShapeSegment`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_LIMIT_EXCEEDED when the number of `voShapeCandidate` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   */
  voShapeCandidate (VO_MSE_CALL *getCandidateAt)(voEngine engine, voShapeSegment target, int32_t index);

  /**
   * Returns the index of the best candidate from the current context in this
   * segment's top list (list of candidates).
   *
   * @param engine the engine.
   * @param target the target segment.
   *
   * @return the index of the best candidate on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voShapeSegment`.
   * @throws VO_INVALID_STATE when `target` is not in a state where the selected
   *   candidate is available.
   */
  int32_t (VO_MSE_CALL *getSelectedCandidateIndex)(voEngine engine, voShapeSegment target);

  /**
   * Selects the candidate at the specified index in this segment's candidate
   * list.
   *
   * @param engine the engine.
   * @param target the target segment.
   * @param index the index of the candidate to be selected.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voShapeSegment`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_INVALID_STATE when `target` is not in a state where the selected
   *   candidate can be set.
   */
  bool (VO_MSE_CALL *selectCandidateAt)(voEngine engine, voShapeSegment target, int32_t index);
}
voIShapeSegment;


// -- ShapePrimitive -----------------------------------------------------------

/**
 * `voShapePrimitive` reference type.
 *
 * {@extends voEngineObject}
 */
typedef voEngineObject voShapePrimitive;

/**
 * Functions composing the `voIShapePrimitive` interface.
 *
 * This interface exposes the public methods of the `voShapePrimitive` type.
 *
 * {@implementingTypes voShapePrimitive}
 */

typedef struct _voIShapePrimitive
{
  /**
   * Returns the points generated from the primitive.
   *
   * @param engine the engine.
   * @param target the target primitive.
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
   * @throws VO_INVALID_OPERATION when `target` is not a `voShapePrimitive`.
   * @throws VO_INVALID_USER_BUFFER when `pointCount` is not readable or
   *   writable.
   * @throws VO_INVALID_ARGUMENT when `x` or `y` is not `NULL` and
   *   `pointCount <= 0`
   * @throws VO_INVALID_USER_BUFFER when the combination of `x`, `xByteStride`,
   *   `y`, `yByteStride` and `pointCount` leads to invalid memory access.
   *
   * @note You can pass `NULL` in place of `x` and `y` to retrieve the number of
   *   points in the stroke.
   *
   * @since 7.0_0
   */
  bool (VO_MSE_CALL *getPoints)(voEngine engine, voShapePrimitive target, float* x, size_t xByteStride, float* y, size_t yByteStride, int32_t* pointCount);

  /**
   * Returns the number of ink ranges this primitive covers.
   *
   * @param engine the engine.
   * @param target the target primitive.
   *
   * @return the number of ink ranges on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voShapePrimitive`.
   * @throws VO_INVALID_STATE when the portion of a MyScript Document associated
   *   with this primitive has been modified, invalidating ink ranges.
   *
   * @since 7.0_0
   */
  int32_t (VO_MSE_CALL *getInkRangeCount)(voEngine engine, voShapePrimitive target);

  /**
   * Returns the ink range at the specified index.
   *
   * @param engine the engine.
   * @param target the target primitive.
   * @param index the index of the requested ink range.
   * @param range the recipient for the ink range.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voShapePrimitive`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_INVALID_USER_BUFFER when `range` is not writable.
   * @throws VO_INVALID_STATE when the portion of a MyScript Document associated
   *   with this primitive has been modified, invalidating ink ranges.
   *
   * @since 7.0_0
   */
  bool (VO_MSE_CALL *getInkRangeAt)(voEngine engine, voShapePrimitive target, int32_t index, voShapeInkRange* range);
}
voIShapePrimitive;


// -- ShapeDocument ------------------------------------------------------------

/**
 * `voShapeDocument` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voIShapeDocument}
 */
typedef voEngineObject voShapeDocument;

/**
 * {@propertySet ShapeDocument}
 */
enum VO_SHAPE_DOCUMENT_PROP
{
  /**
   * Tells the shape document to not automatically commit its result, i.e. once
   * a document is processed by a shape recognizer, the recognition result is
   * not used as input for the next recognition.
   *
   * {@property bool}
   *
   * @since 7.0_0
   */
  VO_SHAPE_DOCUMENT_DISABLE_AUTO_COMMIT_RESULT = (VO_ShapeDocument << 16),

  /**
   * Tells the shape document to not create a new segment for each new stroke or
   * primitive that is added to it, unless `startSegment()` has been called.
   *
   * {@property bool}
   *
   * @since 7.0_0
   */
  VO_SHAPE_DOCUMENT_DISABLE_AUTO_SEGMENT_CREATION,
};

/**
 * Functions composing the `voIShapeDocument` interface.
 *
 * This interface exposes the public methods of the `voShapeDocument` type.
 *
 * {@implementingTypes voShapeDocument}
 */
typedef struct _voIShapeDocument
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
   * @throws VO_INVALID_OPERATION when `target` is not a `voShapeDocument`.
   */
  bool (VO_MSE_CALL *clear)(voEngine engine, voShapeDocument target);

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
   * @throws VO_INVALID_OPERATION when `target` is not a `voShapeDocument`.
   * @throws VO_INVALID_ARGUMENT when `xByteStride == 0`.
   * @throws VO_INVALID_ARGUMENT when `yByteStride == 0`.
   * @throws VO_INVALID_ARGUMENT when `pointCount == 0`.
   * @throws VO_INVALID_USER_BUFFER when the combination of `x`, `xByteStride`,
   *   `y`, `yByteStride` and `pointCount` leads to invalid memory access.
   * @throws VO_LIMIT_EXCEEDED when `pointCount > 32767`.
   */
  bool (VO_MSE_CALL *addStroke)(voEngine engine, voShapeDocument target, const float* x, size_t xByteStride, const float* y, size_t yByteStride, int32_t pointCount);

  /**
   * Adds a primitive to this document.
   *
   * @param engine the engine.
   * @param target the target document.
   * @param primitive the primitive to be added.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voShapeDocument`.
   * @throws VO_NO_SUCH_OBJECT when the `primitive` reference is invalid.
   * @throws VO_INVALID_OBJECT when `primitive` is not a `voShapePrimitive`.
   */
  bool (VO_MSE_CALL *addPrimitive)(voEngine engine, voShapeDocument target, voShapePrimitive primitive);

  /**
   * Starts a new segment.
   *
   * @param engine the engine.
   * @param target the target document.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voShapeDocument`.
   */
  bool (VO_MSE_CALL *startSegment)(voEngine engine, voShapeDocument target);

  /**
   * Ends the current segment.
   *
   * @param engine the engine.
   * @param target the target document.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voShapeDocument`.
   * @throws VO_UNMATCHED_SHAPE_SEGMENT when the `endSegment()` call does not
   *   match any prior `startSegment()` call.
   */
  bool (VO_MSE_CALL *endSegment)(voEngine engine, voShapeDocument target);

  /**
   * Returns the number of segments in this document.
   *
   * @param engine the engine.
   * @param target the target document.
   *
   * @return the number of segments on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voShapeDocument`.
   */
  int32_t (VO_MSE_CALL *getSegmentCount)(voEngine engine, voShapeDocument target);

  /**
   * Returns the segment at the specified index.
   *
   * @param engine the engine.
   * @param target the target document.
   * @param index the index of the segment to be retrieved.
   *
   * @return the segment on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voShapeDocument`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_LIMIT_EXCEEDED when the number of `voShapeSegment` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   */
  voShapeSegment (VO_MSE_CALL *getSegmentAt)(voEngine engine, voShapeDocument target, int32_t index);

  /**
   * Removes the segment at the specified index.
   *
   * @param engine the engine.
   * @param target the target document.
   * @param index the index of the segment to be removed.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voShapeDocument`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   */
  bool (VO_MSE_CALL *removeSegmentAt)(voEngine engine, voShapeDocument target, int32_t index);

  /**
   * Inserts a segment at the specified index.
   *
   * @param engine the engine.
   * @param target the target document.
   * @param segment the shape segment to be inserted.
   * @param index the index designing the position at with to insert
   *   the segment.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voShapeDocument`.
   * @throws VO_NO_SUCH_OBJECT when the `segment` reference is invalid.
   * @throws VO_INVALID_OBJECT when `segment` is not a `voShapeSegment`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_INVALID_STATE when `segment` already exists in `target`.
   */
  bool (VO_MSE_CALL *insertSegmentAt)(voEngine engine, voShapeDocument target, voShapeSegment segment, int32_t index);

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
   * @throws VO_INVALID_OPERATION when `target` is not a `voShapeDocument`.
   */
  int32_t (VO_MSE_CALL *getStrokeCount)(voEngine engine, voShapeDocument target);

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
   * @throws VO_INVALID_OPERATION when `target` is not a `voShapeDocument`.
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
  bool (VO_MSE_CALL *getStrokeAt)(voEngine engine, voShapeDocument target, int32_t index, float* x, size_t xByteStride, float* y, size_t yByteStride, int32_t* pointCount);

  /**
   * Sets the resolution of the coordinates in dpi. This is the resolution of
   * the grid of integer values in the chosen coordinate system. For example, if
   * you use floating-point pixel coordinates you must give the screen
   * resolution, no matter the resolution of the capturing device.
   *
   * @note The value `0` resets to the default behavior of automatic estimation.
   *
   * @param engine the engine.
   * @param target the target document.
   * @param resolution the coordinate resolution.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voShapeDocument`.
   * @throws VO_INVALID_ARGUMENT when `resolution < 0`.
   *
   * @since 5.0_0
   */
  bool (VO_MSE_CALL *setCoordinateResolution)(voEngine engine, voShapeDocument target, float resolution);

  /**
   * Deletes a stroke or a primitive from this document.
   *
   * @param engine the engine.
   * @param target the target document.
   * @param index the index of the component to delete.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voShapeDocument`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   *
   * @since 7.0_0
   */
  bool (VO_MSE_CALL *deleteComponentAt)(voEngine engine, voShapeDocument target, int32_t index);

  /**
   * Undeletes a stroke or a primitive from this document.
   *
   * @param engine the engine.
   * @param target the target document.
   * @param index the index of the component to undelete.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voShapeDocument`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   *
   * @since 7.0_0
   */
  bool (VO_MSE_CALL *undeleteComponentAt)(voEngine engine, voShapeDocument target, int32_t index);

  /**
   * Marks a portion of the shape document segments as committed.
   *
   * @note The shape recognition results associated with committed portions of a
   *   document are used as input of the next shape recognition process, rather
   *   than the corresponding input strokes.
   *
   * @param engine the engine.
   * @param target the target document.
   * @param ranges an array of ink ranges, or `NULL` to commit the entire
   *   result.
   * @param rangeCount the number of ink ranges values in `ranges`.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voShapeDocument`.
   * @throws VO_INVALID_USER_BUFFER when `ranges` is not readable.
   * @throws VO_INVALID_ARGUMENT when `ranges` is not `NULL` and
   *   `rangeCount <= 0`
   * @throws VO_INVALID_ARGUMENT when `ranges` does not match the content of the
   *   document.
   *
   * @since 7.0_0
   */
  bool (VO_MSE_CALL *commitResult)(voEngine engine, voShapeDocument target, const voShapeInkRange* ranges, int32_t rangeCount);
}
voIShapeDocument;

#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voShapeDocument` instance.
 *
 * @param engine the engine.
 *
 * @return the newly created `voShapeDocument` instance on success, otherwise
 *   `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_LIMIT_EXCEEDED when the number of `voShapeDocument` instances
 *   would exceed the allowed limit (please remember that the limit can be
 *   shared by several types).
 */
VO_INLINE voShapeDocument voCreateShapeDocument(voEngine engine)
{
  return voCreateObject(engine, VO_ShapeDocument);
}
#endif // end of: #ifndef VO_NO_FUNCTIONS


// -- ShapeLine ----------------------------------------------------------------

/**
 * `voShapeLine` reference type.
 *
 * {@extends voShapePrimitive}
 * {@implements voIShapeLine, voIShapePrimitive}
 */
typedef voShapePrimitive voShapeLine;

/**
 * Represents the location of a point in the 2D space.
 */
typedef struct _voShapePointData
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
voShapePointData;

/**
 * Represents the data of a line segment in the 2D space.
 *
 * @see voShapeLine
 */
typedef struct _voShapeLineData
{
  /**
   * First point composing the line segment.
   */
  voShapePointData p1;

  /**
   * Second point composing the line segment.
   */
  voShapePointData p2;
}
voShapeLineData;

/**
 * Functions composing the `voIShapeLine` interface.
 *
 * This interface exposes the public methods of the `voShapeLine` type.
 *
 * {@implementingTypes voShapeLine}
 */
typedef struct _voIShapeLine
{
  /**
   * Returns the data corresponding to this line.
   *
   * @param engine the engine.
   * @param target the target line.
   * @param data the recipient for the data.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voShapeLine`.
   * @throws VO_INVALID_USER_BUFFER when `data` is not writable.
   */
  bool (VO_MSE_CALL *getData)(voEngine engine, voShapeLine target, voShapeLineData* data);

  /**
   * Sets the data corresponding to this line.
   *
   * @param engine the engine.
   * @param target the target line.
   * @param data the data to be associated with this line.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voShapeLine`.
   * @throws VO_INVALID_USER_BUFFER when `data` is not readable.
   */
  bool (VO_MSE_CALL *setData)(voEngine engine, voShapeLine target, const voShapeLineData* data);
}
voIShapeLine;

#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voShapeLine` instance.
 *
 * @param engine the engine.
 * @param data the data of the line to be created.
 *
 * @return the newly created `voShapeLine` instance on success, otherwise
 *   `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_INVALID_USER_BUFFER when `data` is not readable.
 * @throws VO_LIMIT_EXCEEDED when the number of `voShapeLine` instances would
 *   exceed the allowed limit (please remember that the limit can be shared by
 *   several types).
 */
VO_INLINE voShapeLine voCreateShapeLine(voEngine engine, const voShapeLineData* data)
{
  return voCreateObjectEx(engine, VO_ShapeLine, data, sizeof(*data));
}
#endif // end of: #ifndef VO_NO_FUNCTIONS


// -- ShapeDecoratedLine -------------------------------------------------------

/**
 * Lists the decoration types.
 */
typedef enum _voShapeDecorationType
{
  /**
   * No decoration.
   */
  VO_SHAPE_DECORATION_NONE,

  /**
   * Arrow head.
   */
  VO_SHAPE_DECORATION_ARROW_HEAD,

  _voShapeDecorationType_FORCE_INT_SIZE = 0x7FFFFFFF
}
voShapeDecorationType;

/**
 * `voShapeDecoratedLine` reference type.
 *
 * {@extends voShapePrimitive}
 * {@implements voIShapeDecoratedLine, voIShapePrimitive}
 */
typedef voShapePrimitive voShapeDecoratedLine;

/**
 * Represents the data of a decorated line in the 2D space.
 *
 * @see voShapeDecoratedLine
 */
typedef struct _voShapeDecoratedLineData
{
  /**
   * Line data.
   */
  voShapeLineData line;

  /**
   * Decoration associated with the first point of the line.
   */
  voShapeDecorationType p1Decoration;

  /**
   * Decoration associated with the second point of the line.
   */
  voShapeDecorationType p2Decoration;
}
voShapeDecoratedLineData;

/**
 * Functions composing the `voIShapeDecoratedLine` interface.
 *
 * This interface exposes the public methods of the `voShapeDecoratedLine` type.
 *
 * {@implementingTypes voShapeDecoratedLine}
 */
typedef struct _voIShapeDecoratedLine
{
  /**
   * Returns the data corresponding to this decorated line.
   *
   * @param engine the engine.
   * @param target the target line.
   * @param data the recipient for the data.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voShapeDecoratedLine`.
   * @throws VO_INVALID_USER_BUFFER when `data` is not writable.
   */
  bool (VO_MSE_CALL *getData)(voEngine engine, voShapeDecoratedLine target, voShapeDecoratedLineData* data);

  /**
   * Sets the data corresponding to this decorated line.
   *
   * @param engine the engine.
   * @param target the target line.
   * @param data the data to be associated with this decorated line.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voShapeDecoratedLine`.
   * @throws VO_INVALID_USER_BUFFER when `data` is not readable.
   */
  bool (VO_MSE_CALL *setData)(voEngine engine, voShapeDecoratedLine target, const voShapeDecoratedLineData* data);

  /**
   * Computes the tangent angle at point p1 of the line.
   *
   * @param engine the engine.
   * @param target the target line.
   * @param angle the recipient for the tangent angle.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voShapeDecoratedLine`.
   * @throws VO_INVALID_USER_BUFFER when `angle` is not writable.
   */
  bool (VO_MSE_CALL *getP1TangentAngle)(voEngine engine, voShapeDecoratedLine target, float* angle);

  /**
   * Computes the tangent angle at point p2 of the line.
   *
   * @param engine the engine.
   * @param target the target line.
   * @param angle the recipient for the tangent angle.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voShapeDecoratedLine`.
   * @throws VO_INVALID_USER_BUFFER when `angle` is not writable.
   */
  bool (VO_MSE_CALL *getP2TangentAngle)(voEngine engine, voShapeDecoratedLine target, float* angle);
}
voIShapeDecoratedLine;

#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voShapeDecoratedLine` instance.
 *
 * @param engine the engine.
 * @param data the data of the line to be created.
 *
 * @return the newly created `voShapeDecoratedLine` instance on success,
 *   otherwise `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_INVALID_USER_BUFFER when `data` is not readable.
 * @throws VO_LIMIT_EXCEEDED when the number of `voShapeDecoratedLine` instances
 *   would exceed the allowed limit (please remember that the limit can be
 *   shared by several types).
 */
VO_INLINE voShapeDecoratedLine voCreateShapeDecoratedLine(voEngine engine, const voShapeDecoratedLineData* data)
{
  return voCreateObjectEx(engine, VO_ShapeDecoratedLine, data, sizeof(*data));
}
#endif // end of: #ifndef VO_NO_FUNCTIONS


// -- ShapeEllipticArc ---------------------------------------------------------

/**
 * `voShapeEllipticArc` reference type.
 *
 * {@extends voShapePrimitive}
 * {@implements voIShapeEllipticArc, voIShapePrimitive}
 */
typedef voShapePrimitive voShapeEllipticArc;

/**
 * Represents the data of an elliptic arc in the 2D space.
 *
 * @see voShapeEllipticArc
 */
typedef struct _voShapeEllipticArcData
{
  /**
   * Center of ellipse.
   */
  voShapePointData center;

  /**
   * Ellipse minor radius.
   */
  float minRadius;

  /**
  * Ellipse major radius.
   */
  float maxRadius;

  /**
   * Orientation: angle of major axis of ellipse, in radians.
   */
  float orientation;

  /**
   * Starting angle of open arc, in radians.
   */
  float startAngle;

  /**
   * Sweep angle of open arc, measured clockwise, in radians.
   */
  float sweepAngle;
}
voShapeEllipticArcData;

/**
 * Functions composing the `voIShapeEllipticArc` interface.
 *
 * This interface exposes the public methods of the `voShapeEllipticArc` type.
 *
 * {@implementingTypes voShapeEllipticArc}
 */
typedef struct _voIShapeEllipticArc
{
  /**
   * Returns the data corresponding to this elliptic arc.
   *
   * @param engine the engine.
   * @param target the target elliptic arc.
   * @param data the recipient for the data.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voShapeEllipticArc`.
   * @throws VO_INVALID_USER_BUFFER when `data` is not writable.
   */
  bool (VO_MSE_CALL *getData)(voEngine engine, voShapeEllipticArc target, voShapeEllipticArcData* data);

  /**
   * Sets the data corresponding to this elliptic arc.
   *
   * @param engine the engine.
   * @param target the target elliptic arc.
   * @param data the data to be associated with this elliptic arc.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voShapeEllipticArc`.
   * @throws VO_INVALID_USER_BUFFER when `data` is not readable.
   */
  bool (VO_MSE_CALL *setData)(voEngine engine, voShapeEllipticArc target, const voShapeEllipticArcData* data);
}
voIShapeEllipticArc;

#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voShapeEllipticArc` instance.
 *
 * @param engine the engine.
 * @param data the data of the elliptic arc to be created.
 *
 * @return the newly created `voShapeEllipticArc` instance on success, otherwise
 *   `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_INVALID_USER_BUFFER when `data` is not readable.
 * @throws VO_LIMIT_EXCEEDED when the number of `voShapeEllipticArc` instances
 *   would exceed the allowed limit (please remember that the limit can be
 *   shared by several types).
 */
VO_INLINE voShapeEllipticArc voCreateShapeEllipticArc(voEngine engine, const voShapeEllipticArcData* data)
{
  return voCreateObjectEx(engine, VO_ShapeEllipticArc, data, sizeof(*data));
}
#endif // end of: #ifndef VO_NO_FUNCTIONS


// -- ShapeDecoratedEllipticArc ------------------------------------------------

/**
 * `voShapeDecoratedEllipticArc` reference type.
 *
 * {@extends voShapePrimitive}
 * {@implements voIShapeDecoratedEllipticArc, voIShapePrimitive}
 */
typedef voShapePrimitive voShapeDecoratedEllipticArc;

/**
 * Represents the data of a decorated elliptic arc in the 2D space.
 *
 * @see voShapeDecoratedEllipticArc
 */
typedef struct _voShapeDecoratedEllipticArcData
{
  /**
   * Arc data.
   */
  voShapeEllipticArcData arc;

  /**
   * Decoration associated with the first end of the arc.
   */
  voShapeDecorationType firstDecoration;

  /**
   * Decoration associated with the last end of the arc.
   */
  voShapeDecorationType lastDecoration;
}
voShapeDecoratedEllipticArcData;

/**
 * Functions composing the `voIShapeDecoratedEllipticArc` interface.
 *
 * This interface exposes the public methods of the
 * `voShapeDecoratedEllipticArc` type.
 *
 * {@implementingTypes voShapeDecoratedEllipticArc}
 */
typedef struct _voIShapeDecoratedEllipticArc
{
  /**
   * Returns the data corresponding to this decorated elliptic arc.
   *
   * @param engine the engine.
   * @param target the target arc.
   * @param data the recipient for the data.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a
   *   `voShapeDecoratedEllipticArc`.
   * @throws VO_INVALID_USER_BUFFER when `data` is not writable.
   */
  bool (VO_MSE_CALL *getData)(voEngine engine, voShapeDecoratedEllipticArc target, voShapeDecoratedEllipticArcData* data);

  /**
   * Sets the data corresponding to this decorated elliptic arc.
   *
   * @param engine the engine.
   * @param target the target arc.
   * @param data the data to be associated with this decorated elliptic arc.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a
   *   `voShapeDecoratedEllipticArc`.
   * @throws VO_INVALID_USER_BUFFER when `data` is not readable.
   */
  bool (VO_MSE_CALL *setData)(voEngine engine, voShapeDecoratedEllipticArc target, const voShapeDecoratedEllipticArcData* data);

  /**
   * Computes the tangent angle at the first end of this decorated elliptic arc.
   *
   * @param engine the engine.
   * @param target the target arc.
   * @param angle the recipient for the tangent angle.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a
   *   `voShapeDecoratedEllipticArc`.
   * @throws VO_INVALID_USER_BUFFER when `angle` is not writable.
   */
  bool (VO_MSE_CALL *getFirstTangentAngle)(voEngine engine, voShapeDecoratedEllipticArc target, float* angle);

  /**
   * Computes the tangent angle at the last end of this decorated elliptic arc.
   *
   * @param engine the engine.
   * @param target the target arc.
   * @param angle the recipient for the tangent angle.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a
   *   `voShapeDecoratedEllipticArc`.
   * @throws VO_INVALID_USER_BUFFER when `angle` is not writable.
   */
  bool (VO_MSE_CALL *getLastTangentAngle)(voEngine engine, voShapeDecoratedEllipticArc target, float* angle);
}
voIShapeDecoratedEllipticArc;

#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voShapeDecoratedEllipticArc` instance.
 *
 * @param engine the engine.
 * @param data the data of the decorated elliptic arc to be created.
 *
 * @return the newly created `voShapeDecoratedEllipticArc` instance on success,
 *   otherwise `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_INVALID_USER_BUFFER when `data` is not readable.
 * @throws VO_LIMIT_EXCEEDED when the number of `voShapeDecoratedEllipticArc`
 *   instances would exceed the allowed limit (please remember that the limit
 *   can be shared by several types).
 */
VO_INLINE voShapeDecoratedEllipticArc voCreateShapeDecoratedEllipticArc(voEngine engine, const voShapeDecoratedEllipticArcData* data)
{
  return voCreateObjectEx(engine, VO_ShapeDecoratedEllipticArc, data, sizeof(*data));
}
#endif // end of: #ifndef VO_NO_FUNCTIONS


// -- ShapeRecognized ----------------------------------------------------------

/**
 * `voShapeRecognized` reference type.
 *
 * {@extends voShapeCandidate}
 * {@implements voIShapeRecognized}
 */
typedef voShapeCandidate voShapeRecognized;

/**
 * Functions composing the `voIShapeRecognized` interface.
 *
 * This interface exposes the public methods of the `voShapeRecognized` type.
 *
 * {@implementingTypes voShapeRecognized}
 */
typedef struct _voIShapeRecognized
{
  /**
   * Returns the shape model associated with this recognized shape.
   *
   * @param engine the engine.
   * @param target the target shape.
   *
   * @return the shape model on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voShapeRecognized`.
   * @throws VO_LIMIT_EXCEEDED when the number of `voShapeModel` instances would
   *   exceed the allowed limit (please remember that the limit can be shared by
   *   several types).
   */
  voShapeModel (VO_MSE_CALL *getModel)(voEngine engine, voShapeRecognized target);

  /**
   * Returns the resemblance score of this recognized shape.
   *
   * @param engine the engine.
   * @param target the target shape.
   *
   * @return the resemblance score on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voShapeRecognized`.
   */
  float (VO_MSE_CALL *getResemblanceScore)(voEngine engine, voShapeRecognized target);

  /**
   * Returns the normalized recognition score of this recognized shape.
   *
   * @param engine the engine.
   * @param target the target shape.
   *
   * @return the normalized recognition score on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voShapeRecognized`.
   */
  float (VO_MSE_CALL *getNormalizedRecognitionScore)(voEngine engine, voShapeRecognized target);

  /**
   * Returns the number of primitives this recognized shape is made of.
   *
   * @param engine the engine.
   * @param target the target shape.
   *
   * @return the number of primitives on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voShapeRecognized`.
   */
  int32_t (VO_MSE_CALL *getPrimitiveCount)(voEngine engine, voShapeRecognized target);

  /**
   * Returns the primitive at the specified index.
   *
   * @param engine the engine.
   * @param target the target shape.
   * @param index the index of the primitive to be retrieved.
   *
   * @return the primitive on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voShapeRecognized`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_LIMIT_EXCEEDED when the number of `voShapePrimitive` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   */
  voShapePrimitive (VO_MSE_CALL *getPrimitiveAt)(voEngine engine, voShapeRecognized target, int32_t index);
}
voIShapeRecognized;


// -- ShapeScratchOut ----------------------------------------------------------

/**
 * `voShapeScratchOut` reference type.
 *
 * {@extends voShapeCandidate}
 * {@implements voIShapeScratchOut}
 */
typedef voShapeCandidate voShapeScratchOut;

/**
 * Functions composing the `voIShapeScratchOut` interface.
 *
 * This interface exposes the public methods of the `voShapeScratchOut` type.
 *
 * {@implementingTypes voShapeScratchOut}
 */
typedef struct _voIShapeScratchOut
{
  /**
   * Returns the number of ink ranges this scratch out erases.
   *
   * @param engine the engine.
   * @param target the target scratch out.
   *
   * @return the number of ink ranges on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voShapeScratchOut`.
   * @throws VO_INVALID_STATE when the portion of a MyScript Document associated
   *   with this scratch out has been modified, invalidating ink ranges.
   */
  int32_t (VO_MSE_CALL *getErasedInkRangeCount)(voEngine engine, voShapeScratchOut target);

  /**
   * Returns the ink range at the specified index this scratch out erases.
   *
   * @param engine the engine.
   * @param target the target scratch out.
   * @param index the index of the requested ink range.
   * @param range the recipient for the ink range.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voShapeScratchOut`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_INVALID_USER_BUFFER when `range` is not writable.
   * @throws VO_INVALID_STATE when the portion of a MyScript Document associated
   *   with this scratch out has been modified, invalidating ink ranges.
   */
  bool (VO_MSE_CALL *getErasedInkRangeAt)(voEngine engine, voShapeScratchOut target, int32_t index, voShapeInkRange* range);
}
voIShapeScratchOut;


// -- ShapeErased --------------------------------------------------------------

/**
 * `voShapeErased` reference type.
 *
 * {@extends voShapeCandidate}
 */
typedef voShapeCandidate voShapeErased;


// -- ShapeRejected ------------------------------------------------------------

/**
 * `voShapeRejected` reference type.
 *
 * {@extends voShapeCandidate}
 */
typedef voShapeCandidate voShapeRejected;


// -- IShapeDocumentProcessor --------------------------------------------------

/**
 * Functions composing the `voIShapeDocumentProcessor` interface.
 *
 * {@implementingTypes voShapeRecognizer, voShapeBeautifier}
 */
typedef struct _voIShapeDocumentProcessor
{
  /**
   * Processes a shape document.
   *
   * @param engine the engine.
   * @param target the target processor.
   * @param document the shape document to be processed.
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
   * @throws VO_INVALID_OBJECT when `document` is not a `voShapeDocument`.
   * @throws VO_INVALID_CALLBACK when `notifyProgressFunc` is invalid.
   * @throws VO_INVALID_STATE when the `target` object is not in a state where
   *   the `process` function can be invoked.
   * @throws VO_MISSING_SHAPE_KNOWLEDGE when `recognizer` has no attached
   *   `voShapeKnowledge`.
   * @throws VO_CANCELED when the operation has been canceled.
   *
   * @see voShapeDocument
   */
  bool (VO_MSE_CALL *process)(voEngine engine, voEngineObject target, voShapeDocument document, voNotifyProgressFunc notifyProgressFunc, void* userParam);
}
voIShapeDocumentProcessor;


// -- ShapeHistoryManager ------------------------------------------------------

/**
 * `voShapeHistoryManager` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voIShapeHistoryManager}
 */
typedef voEngineObject voShapeHistoryManager;

/**
 * {@propertySet ShapeHistoryManager}
 */
enum VO_SHAPE_HISTORY_MANAGER_PROP
{
  /**
   * A value that adjusts the depth of the undo stack.
   *
   * {@property uint32_t}
   *
   * @note A value equal to `0` removes any limit.
   */
  VO_SHAPE_HISTORY_MANAGER_UNDO_STACK_DEPTH = (VO_ShapeHistoryManager << 16),
};

/**
 * Functions composing the `voIShapeHistoryManager` interface.
 *
 * This interface exposes the public methods of the `voShapeHistoryManager`
 * type.
 *
 * {@implementingTypes voShapeHistoryManager}
 */
typedef struct _voIShapeHistoryManager
{
  /**
   * Tells whether you can undo the last stroke on the associated document.
   *
   * @param engine the engine.
   * @param target the target history manager.
   *
   * @return `VO_YES` or `VO_NO` on success, otherwise `VO_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a
   *   `voShapeHistoryManager`.
   * @throws VO_INVALID_STATE when `target` is not attached to a document.
   */
  voYesNo (VO_MSE_CALL *canUndo)(voEngine engine, voShapeHistoryManager target);

  /**
   * Tells whether you can redo the last stroke on the associated document.
   *
   * @param engine the engine.
   * @param target the target history manager.
   *
   * @return `VO_YES` or `VO_NO` on success, otherwise `VO_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a
   *   `voShapeHistoryManager`.
   * @throws VO_INVALID_STATE when `target` is not attached to a document.
   */
  voYesNo (VO_MSE_CALL *canRedo)(voEngine engine, voShapeHistoryManager target);

  /**
   * Undo the last stroke and re-process the associated document.
   *
   * @param engine the engine.
   * @param target the target history manager.
   * @param recognizer the recognizer to be used when processing the document.
   * @param beautifier the beautifier to be used when processing the document,
   *   or `NULL` when no beautification is required.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a
   *   `voShapeHistoryManager`.
   * @throws VO_NO_SUCH_OBJECT when the `recognizer` reference is invalid.
   * @throws VO_INVALID_OBJECT when `recognizer` is not a `voShapeRecognizer`.
   * @throws VO_NO_SUCH_OBJECT when the `beautifier` reference is invalid.
   * @throws VO_INVALID_OBJECT when `beautifier` is not a `voShapeBeautifier`.
   * @throws VO_INVALID_STATE when `target` is not attached to a document.
   * @throws VO_INVALID_STATE when undo is not possible.
   */
  bool (VO_MSE_CALL *undoLastStroke)(voEngine engine, voShapeHistoryManager target, voShapeRecognizer recognizer, voShapeBeautifier beautifier);

  /**
   * Redo the last stroke and re-process the associated document.
   *
   * @param engine the engine.
   * @param target the target history manager.
   * @param recognizer the recognizer to be used when processing the document.
   * @param beautifier the beautifier to be used when processing the document,
   *   or `NULL` when no beautification is required.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a
   *   `voShapeHistoryManager`.
   * @throws VO_NO_SUCH_OBJECT when the `recognizer` reference is invalid.
   * @throws VO_INVALID_OBJECT when `recognizer` is not a `voShapeRecognizer`.
   * @throws VO_NO_SUCH_OBJECT when the `beautifier` reference is invalid.
   * @throws VO_INVALID_OBJECT when `beautifier` is not a `voShapeBeautifier`.
   * @throws VO_INVALID_STATE when `target` is not attached to a document.
   * @throws VO_INVALID_STATE when redo is not possible.
   */
  bool (VO_MSE_CALL *redoLastStroke)(voEngine engine, voShapeHistoryManager target, voShapeRecognizer recognizer, voShapeBeautifier beautifier);
}
voIShapeHistoryManager;

#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voShapeHistoryManager` instance.
 *
 * @param engine the engine.
 *
 * @return the newly created `voShapeHistoryManager` instance on success,
 *   otherwise `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_LIMIT_EXCEEDED when the number of `voShapeHistoryManager`
 *   instances would exceed the allowed limit (please remember that the limit
 *   can be shared by several types).
 */
VO_INLINE voShapeHistoryManager voCreateShapeHistoryManager(voEngine engine)
{
  return voCreateObject(engine, VO_ShapeHistoryManager);
}
#endif // end of: #ifndef VO_NO_FUNCTIONS

#endif // end of: #ifndef C_MYSCRIPTSHAPE_H
