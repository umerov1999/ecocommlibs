#ifndef C_MYSCRIPTINK_H
#define C_MYSCRIPTINK_H 0x08010000
/**
 * @file c/MyScriptInk.h
 * Native interface to the MyScript Ink service.
 */

#include "MyScriptEngine.h"
#include <stdarg.h>


// -- error codes --------------------------------------------------------------

/**
 * Lists the MyScript Ink errors.
 */
enum VO_INK_ERR
{
  /**
   * A style class provided does not correspond to any class in the style
   * sheets.
   */
  VO_INVALID_STYLE_CLASS = VO_INVALID_STATE | (9 << 16)
};


// -- types and interfaces -----------------------------------------------------

/**
 * Lists the MyScript Ink types.
 */
enum VO_INK_T
{
  /**
   * `voInkStrokeFormat` type identifier.
   *
   * @see voInkStrokeFormat
   */
  VO_InkStrokeFormat = 3100,

  /**
   * `voInkStrokeFormatBuilder` type identifier.
   *
   * @see voInkStrokeFormatBuilder
   */
  VO_InkStrokeFormatBuilder,

  /**
   * `voInkStroke` type identifier.
   *
   * @see voInkStroke
   */
  VO_InkStroke,

  /**
   * `voInkStrokeBuilder` type identifier.
   *
   * @see voInkStrokeBuilder
   */
  VO_InkStrokeBuilder,

  /**
   * `voInk` type identifier.
   *
   * @see voInk
   */
  VO_Ink,

  /**
   * `voInkIntervals` type identifier.
   *
   * @see voInkIntervals
   */
  VO_InkIntervals,

  /**
   * `voInkSegment` type identifier.
   *
   * @see voInkSegment
   */
  VO_InkSegment,

  /**
   * `voInkSelection` type identifier.
   *
   * @see voInkSelection
   */
  VO_InkSelection,

  /**
   * `voInkTagIterator` type identifier.
   *
   * @see voInkTagIterator
   */
  VO_InkTagIterator,

  /**
   * `voGlyph` type identifier.
   *
   * @see voGlyph
   */
  VO_Glyph,

  /**
   * `voArcPrimitive` type identifier.
   *
   * @see voArcPrimitive
   */
  VO_ArcPrimitive,

  /**
   * `voLinePrimitive` type identifier.
   *
   * @see voLinePrimitive
   */
  VO_LinePrimitive,
};


/**
 * Lists the MyScript Ink interfaces.
 */
enum VO_INK_I
{
  /**
   * `voISelectionChangeListener` interface identifier.
   *
   * @see voISelectionChangeListener
   */
  VO_ISelectionChangeListener = 3120,

  /**
   * `voISelection` interface identifier.
   *
   * @see voISelection
   */
  VO_ISelection,

  /**
   * `voIInkStrokeFormat` interface identifier.
   *
   * @see voIInkStrokeFormat
   */
  VO_IInkStrokeFormat,

  /**
   * `voIInkStrokeFormatBuilder` interface identifier.
   *
   * @see voIInkStrokeFormatBuilder
   */
  VO_IInkStrokeFormatBuilder,

  /**
   * `voIInkStroke` interface identifier.
   *
   * @see voIInkStroke
   */
  VO_IInkStroke,

  /**
   * `voIInkStrokeBuilder` interface identifier.
   *
   * @see voIInkStrokeBuilder
   */
  VO_IInkStrokeBuilder,

  /**
   * `voIInk` interface identifier.
   *
   * @see voIInk
   */
  VO_IInk,

  /**
   * `voIInkIntervals` interface identifier.
   *
   * @see voIInkIntervals
   */
  VO_IInkIntervals,

  /**
   * `voIInkStrokeSet` interface identifier.
   *
   * @see voIInkStrokeSet
   */
  VO_IInkStrokeSet,

  /**
   * `voIInkSelection` interface identifier.
   *
   * @see voIInkSelection
   */
  VO_IInkSelection,

  /**
   * `voIGeometry` interface identifier.
   *
   * @see voIGeometry
   */
  VO_IGeometry,

  /**
   * `voIInkTagIterator` interface identifier.
   *
   * @see voIInkTagIterator
   */
  VO_IInkTagIterator,

  /**
   * `voIInkUpdateListener` interface identifier.
   *
   * @see voIInkUpdateListener
   */
  VO_IInkUpdateListener,

  /**
   * `voIGlyph` interface identifier.
   *
   * @see voIGlyph
   */
  VO_IGlyph,

  /**
   * `voIPrimitive` interface identifier.
   *
   * @see voIPrimitive
   */
  VO_IPrimitive
};


// -- Geometry -----------------------------------------------------------------

/**
 * Represents a point on the 2D plane.
 */
typedef struct _voPoint
{
  float x;  /**< The x position of the point. */
  float y;  /**< The y position of the point. */
}
voPoint;

/**
 * Represents the extent of some geometric object on the 2D plane.
 */
typedef struct _voExtent
{
  float xmin; /**< The minimal x value. */
  float ymin; /**< The minimal y value. */
  float xmax; /**< The maximal x value. */
  float ymax; /**< The maximal y value. */
}
voExtent;

/**
 * Represents an axis aligned rectangle on the 2D plane.
 */
typedef struct _voRectangle
{
  float x;      /**< The x position of the left edge of the rectangle. */
  float y;      /**< The y position of the top edge of the rectangle. */
  float width;  /**< The width of the rectangle. */
  float height; /**< The height of the rectangle. */
}
voRectangle;

/**
 * Represents a parallelogram with an origin and two vectors.
 */
typedef struct _voParallelogram
{
  float x;  /**< The x position of the origin point. */
  float y;  /**< The y position of the origin point. */
  float ux; /**< The x projection of u: the first side vector. */
  float uy; /**< The y projection of u: the first side vector. */
  float vx; /**< The x projection of v: the adjacent vector. */
  float vy; /**< The y projection of v: the adjacent vector. */
}
voParallelogram;

/**
 * Represents an axis-aligned rounded rectangle on the 2D plane.
 */
typedef struct _voRoundedRectangle
{
  float x;      /**< The x position of the left edge of the rectangle. */
  float y;      /**< The y position of the top edge of the rectangle. */
  float width;  /**< The width of the rectangle. */
  float height; /**< The height of the rectangle. */
  float rx;     /**< The x-axis radius of the corner ellipse. */
  float ry;     /**< The y-axis radius of the corner ellipse. */
}
voRoundedRectangle;

/**
 * Represents a circle on the 2D plane.
 */
typedef struct _voCircle
{
  float cx; /**< The x position of the center of the circle. */
  float cy; /**< The y position of the center of the circle. */
  float r;  /**< The radius of the circle. */
}
voCircle;

/**
 * Represents an axis-aligned ellipse on the 2D plane.
 */
typedef struct _voEllipse
{
  float cx; /**< The x position of the center of the ellipse. */
  float cy; /**< The y position of the center of the ellipse. */
  float rx; /**< The x-axis radius of the ellipse. */
  float ry; /**< The y-axis radius of the ellipse. */
}
voEllipse;

/**
 * Represents a line segment on the 2D plane.
 */
typedef struct _voLineSegment
{
  voPoint p1; /**< first point. */
  voPoint p2; /**< last point. */
}
voLineSegment;

/**
 * Represents a cubic bezier curve on the 2D plane.
 */
typedef struct _voCubicBezierCurve
{
  voPoint p0; /**< first point. */
  voPoint p3; /**< last point. */
  voPoint p1; /**< first control point. */
  voPoint p2; /**< last control point. */
}
voCubicBezierCurve;

/**
 * Represents a quadratic bezier curve on the 2D plane.
 */
typedef struct _voQuadraticBezierCurve
{
  voPoint p0; /**< first point. */
  voPoint p2; /**< last point. */
  voPoint p1; /**< control point. */
}
voQuadraticBezierCurve;

/**
 * Represents an elliptic arc on the 2D plane, by its endpoint parameterization.
 *
 * @note In order to ensure bijective conversion between endpoint and center
 *   parameterizations, we interpret the degenerate case where the two endpoints
 *   are equal as follow: When `p2` equals `p1`, they define the `(1,0)` point
 *   on the origin circle (i.e.: before stretch and rotate), and a full ellipse.
 *
 * @see voCenterEllipticArc
 */
typedef struct _voEndpointEllipticArc
{
  voPoint p1; /**< first point. */
  voPoint p2; /**< last point. */
  float rx;   /**< x-axis radius (must be positive). */
  float ry;   /**< y-axis radius (must be positive). */
  float phi;  /**< x-axis rotation angle. */
  int32_t fA; /**< large-arc flag. */
  int32_t fS; /**< sweep-arc flag. */
}
voEndpointEllipticArc;

/**
 * Represents an elliptic arc on the 2D plane, by its center parameterization.
 *
 * @see voEndpointEllipticArc
 */
typedef struct _voCenterEllipticArc
{
  float cx;         /**< center x position. */
  float cy;         /**< center y position. */
  float rx;         /**< x-axis or semi-major radius (must be positive). */
  float ry;         /**< y-axis or semi-minor radius (must be positive). */
  float phi;        /**< x-axis rotation angle. */
  float startAngle; /**< start angle (prior to stretch and rotate). */
  float sweepAngle; /**< sweep angle (prior to stretch and rotate). */
}
voCenterEllipticArc;

/**
 * Represents a set of equally spaced horizontal lines.
 */
typedef struct _voHorizontalLineSet
{
  /**
   * The y position of the zero-indexed line.
   */
  float y0;

  /**
   * The y offset between two consecutive lines.
   */
  float dy;

  /**
   * The number of lines in this set.
   *
   * Set `count` to `0` for an infinite number of lines.
   */
  int32_t count;

  /**
   * The index of the first line (can be negative).
   */
  int32_t first;

  /**
   * The x position of the beginning of the lines.
   *
   * Set `x1` to `-FLT_MAX` for an infinite horizontal guideline.
   */
  float x1;

  /**
   * The x position of the end of the lines.
   *
   * Set `x2` to `FLT_MAX` for an infinite horizontal guideline.
   */
  float x2;
}
voHorizontalLineSet;

/**
 * Represents a set of equally spaced vertical lines.
 */
typedef struct _voVerticalLineSet
{
  /**
   * The x position of the zero-indexed line.
   */
  float x0;

  /**
   * The x offset between two consecutive lines.
   */
  float dx;

  /**
   * The number of lines in this set.
   *
   * Set `count` to `0` for an infinite number of lines.
   */
  int32_t count;

  /**
   * The index of the first line (can be negative).
   */
  int32_t first;

  /**
   * The y position of the beginning of the lines.
   *
   * Set `y1` to `-FLT_MAX` for an infinite vertical guideline.
   */
  float y1;

  /**
   * The y position of the end of the lines.
   *
   * Set `y2` to `-FLT_MAX` for an infinite vertical guideline.
   */
  float y2;
}
voVerticalLineSet;

/**
 * Represents a 2D affine transform, defined as a 3x3 matrix with an implicit
 * third raw of `[ 0 0 1 ]`:
 *
 * <pre>
 * [  x' ]   [ a  b  c ]   [ x ]   [ a * x + b * y + c ]
 * [  y' ] = [ d  e  f ] * [ y ] = [ d * x + e * y + f ]
 * [  1  ]   [ 0  0  1 ]   [ 1 ]   [         1         ]
 * </pre>
 *
 * A `voTransform` is invalid if a value is infinite or not a number, or if the
 * matrix is not invertible (when its determinant `(a * e - b * d)` is zero).
 */
typedef struct _voTransform
{
  /**
   * The X coordinate scaling element <i>M</i><sub>1,1</sub> of the transform
   * matrix.
   */
  float a;

  /**
   * The X coordinate shearing element <i>M</i><sub>1,2</sub> of the transform
   * matrix.
   */
  float b;

  /**
   * The X coordinate translation element <i>M</i><sub>1,3</sub> of the
   * transform matrix.
   */
  float c;

  /**
   * The Y coordinate shearing element <i>M</i><sub>2,1</sub> of the transform
   * matrix.
   */
  float d;

  /**
   * The Y coordinate scaling element <i>M</i><sub>2,2</sub> of the transform
   * matrix.
   */
  float e;

  /**
   * The Y coordinate translation element <i>M</i><sub>2,3</sub> of the
   * transform matrix.
   */
  float f;
}
voTransform;


// -- Digital Ink --------------------------------------------------------------

/**
 * `voInkStrokeFormat` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voIInkStrokeFormat}
 */
typedef voEngineObject voInkStrokeFormat;

/**
 * `voInkStrokeFormatBuilder` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voIInkStrokeFormat, voIInkStrokeFormatBuilder}
 */
typedef voEngineObject voInkStrokeFormatBuilder;

/**
 * `voInkStroke` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voIInkStroke}
 */
typedef voEngineObject voInkStroke;

/**
 * `voInkStrokeBuilder` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voIInkStroke, voIInkStrokeBuilder}
 */
typedef voEngineObject voInkStrokeBuilder;

/**
 * `voInk` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voIInk, voIInkStrokeSet, voIVersionable}
 */
typedef voEngineObject voInk;

/**
 * `voInkIntervals` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voIInkIntervals, voIInkStrokeSet}
 */
typedef voEngineObject voInkIntervals;

/**
 * `voInkSegment` reference type.
 *
 * {@extends voInkIntervals}
 */
typedef voInkIntervals voInkSegment;

/**
 * `voInkSelection` reference type.
 *
 * {@extends voInkIntervals}
 * {@implements voISelection, voIInkSelection}
 */
typedef voInkIntervals voInkSelection;

/**
 * `voInkTagIterator` reference type.
 *
 * {@extends voIterator}
 * {@implements voIInkTagIterator}
 */
typedef voIterator voInkTagIterator;

/**
 * `voGlyph` reference type.
 *
 * {@extends voInkStroke}
 * {@implements voIInkStroke, voIGlyph}
 */
typedef voInkStroke voGlyph;

/**
 * `voArcPrimitive` reference type.
 *
 * {@extends voInkStroke}
 * {@implements voIInkStroke, voIPrimitive}
 */
typedef voInkStroke voArcPrimitive;

/**
 * `voLinePrimitive` reference type.
 *
 * {@extends voInkStroke}
 * {@implements voIInkStroke, voIPrimitive}
 */
typedef voInkStroke voLinePrimitive;

#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voInkStrokeFormatBuilder` instance.
 *
 * @param engine the engine.
 *
 * @return the newly created `voInkStrokeFormatBuilder` instance on success,
 *   otherwise `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_LIMIT_EXCEEDED when the number of `voInkStrokeFormatBuilder`
 *   instances would exceed the allowed limit (please remember that the limit
 *   can be shared by several types).
 */
VO_INLINE voInkStrokeFormatBuilder voCreateInkStrokeFormatBuilder(voEngine engine)
{
  return voCreateObject(engine, VO_InkStrokeFormatBuilder);
}
#endif // end of: #ifndef VO_NO_FUNCTIONS

/**
 * Holds the parameters required to create a new `voInkStrokeBuilder` instance.
 *
 * @see voInkStrokeBuilder, voCreateInkStrokeBuilder1
 */
typedef struct _voInkStrokeBuilderInitializer1
{
  /**
   * The format of the strokes built by the stroke builder.
   */
  voInkStrokeFormat strokeFormat;
}
voInkStrokeBuilderInitializer1;

#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voInkStrokeBuilder` instance.
 *
 * @param engine the engine.
 * @param strokeFormat the format of the strokes built by the stroke builder.
 *
 * @return the newly created `voInkStrokeBuilder` instance on success, otherwise
 *   `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_NO_SUCH_OBJECT when the `strokeFormat` reference is invalid.
 * @throws VO_INVALID_OBJECT when `strokeFormat` is not a `voInkStrokeFormat`.
 * @throws VO_LIMIT_EXCEEDED when the number of `voInkStrokeBuilder` instances
 *   would exceed the allowed limit (please remember that the limit can be
 *   shared by several types).
 */

VO_INLINE voInkStrokeBuilder voCreateInkStrokeBuilder1(voEngine engine, voInkStrokeFormat strokeFormat)
{
  voInkStrokeBuilderInitializer1 initializer = { strokeFormat };
  return voCreateObjectEx(engine, VO_InkStrokeBuilder, &initializer, sizeof(initializer));
}
#endif // end of: #ifndef VO_NO_FUNCTIONS

/**
 * Holds the parameters required to create a new `voInkStrokeBuilder` instance.
 *
 * @see voInkStrokeBuilder, voCreateInkStrokeBuilder2
 */
typedef struct _voInkStrokeBuilderInitializer2
{
  /**
   * The charset to be used to decode the stroke format string, or `NULL` to use
   * the default charset.
   */
  voCharset charset;

  /**
   * The format of the strokes built by the stroke builder.
   */
  voString strokeFormatString;
}
voInkStrokeBuilderInitializer2;

#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voInkStrokeBuilder` instance.
 *
 * @param engine the engine.
 * @param charset the charset used to encode parameter strings.
 * @param strokeFormatString the format of the strokes built by the stroke
 *   builder.
 *
 * @return the newly created `voInkStrokeBuilder` instance on success, otherwise
 *   `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
 * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
 *   default charset was set.
 * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
 * @throws VO_INVALID_USER_BUFFER when `strokeFormatString` or its `bytes`
 *   member is not readable.
 * @throws VO_LIMIT_EXCEEDED when the number of `voInkStrokeBuilder` instances
 *   would exceed the allowed limit (please remember that the limit can be
 *   shared by several types).
 */
VO_INLINE voInkStrokeBuilder voCreateInkStrokeBuilder2(voEngine engine, voCharset charset, const voString* strokeFormatString)
{
  voInkStrokeBuilderInitializer2 initializer = { charset, { strokeFormatString->bytes, strokeFormatString->byteCount } };
  return voCreateObjectEx(engine, VO_InkStrokeBuilder, &initializer, sizeof(initializer));
}
#endif // end of: #ifndef VO_NO_FUNCTIONS


/**
 * Holds the parameters required to create a new `voInk` instance, with a
 * default stroke format.
 *
 * @see voInk, voCreateInk
 */
typedef struct _voInkInitializer
{
  /**
   * The default format to be used to store the stroke.
   */
  voInkStrokeFormat strokeFormat;
}
voInkInitializer;


#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voInk` instance.
 *
 * @param engine the engine.
 *
 * @return the newly created `voInk` instance on success, otherwise `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_LIMIT_EXCEEDED when the number of `voInk` instances would exceed
 *   the allowed limit (please remember that the limit can be shared by several
 *   types).
 */
VO_INLINE voInk voCreateInk(voEngine engine)
{
  return voCreateObject(engine, VO_Ink);
}

/**
 * Creates a new `voInk` instance.
 *
 * @param engine the engine.
 * @param strokeFormat the format to be used to store the stroke.
 *
 * @return the newly created `voInk` instance on success, otherwise `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_NO_SUCH_OBJECT when the `strokeFormat` reference is invalid.
 * @throws VO_INVALID_OBJECT when `strokeFormat` is not a `voInkStrokeFormat`.
 * @throws VO_LIMIT_EXCEEDED when the number of `voInk` instances would exceed
 *   the allowed limit (please remember that the limit can be shared by several
 *   types).
 */
VO_INLINE voInk voCreateInkWithStrokeFormat(voEngine engine, voInkStrokeFormat strokeFormat)
{
  voInkInitializer inkInitializer = { strokeFormat };
  return voCreateObjectEx(engine, VO_Ink, &inkInitializer, sizeof(voInkInitializer));
}
#endif // end of: #ifndef VO_NO_FUNCTIONS

/**
 * Holds the parameters required to create a new `voInkSelection` instance.
 *
 * @see voInkSelection, voCreateInkSelection
 */
typedef struct _voInkSelectionInitializer
{
  /**
   * The ink on which the selection will be based.
   */
  voInk ink;
}
voInkSelectionInitializer;

#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voInkSelection` instance.
 *
 * @param engine the engine.
 * @param ink the ink on which the selection will be based.
 *
 * @return the newly created `voInkSelection` instance on success, otherwise
 *   `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_NO_SUCH_OBJECT when the `ink` reference is invalid.
 * @throws VO_INVALID_OBJECT when `ink` is not a `voInk`.
 * @throws VO_LIMIT_EXCEEDED when the number of `voInkSelection` instances would
 *   exceed the allowed limit (please remember that the limit can be shared by
 *   several types).
 */
VO_INLINE voInkSelection voCreateInkSelection(voEngine engine, voInk ink)
{
  voInkSelectionInitializer initializer = { ink };
  return voCreateObjectEx(engine, VO_InkSelection, &initializer, sizeof(initializer));
}
#endif // end of: #ifndef VO_NO_FUNCTIONS

/**
 * Represents a position on the discrete trace formed by the sequence of strokes
 * in a stroke set.
 */
typedef struct _voInkCursor
{
  /**
   * The index of the stroke.
   */
  int32_t stroke;

  /**
   * The point position.
   *
   * The integer part represents the index of the sample in the stroke and the
   * decimal part represents position along the segment that links to the next
   * sample.
   */
  float point;

  /**
   * When used as the endpoint of an interval, tells whether this endpoint is
   * included or not.
   */
  bool included;
}
voInkCursor;

/**
 * Represents an interval on the ink cursor line.
 */
typedef struct _voInkInterval
{
  /**
   * The lower endpoint.
   */
  voInkCursor from;

  /**
   * The upper endpoint.
   */
  voInkCursor to;
}
voInkInterval;

/**
 * Functions composing the `voIInkStrokeFormat` interface.
 *
 * {@implementingTypes voInkStrokeFormat, voInkStrokeFormatBuilder}
 */
typedef struct _voIInkStrokeFormat
{
  /**
   * Returns the number of channels in this stroke format.
   *
   * @param engine the engine.
   * @param target the target stroke format.
   *
   * @return the number of channels on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   */
  int32_t (VO_MSE_CALL *getChannelCount)(voEngine engine, voEngineObject target);

  /**
   * Returns the name of the channel at the specified index.
   *
   * @param engine the engine.
   * @param target the target stroke format.
   * @param index the channel index.
   * @param charset the charset used to encode parameter strings.
   * @param name the recipient for the channel name.
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
   * @throws VO_INVALID_USER_BUFFER when `name` or its `bytes` member is not
   *   writable.
   */
  bool (VO_MSE_CALL *getChannelNameAt)(voEngine engine, voEngineObject target, int32_t index, voCharset charset, voString* name);

  /**
   * Returns the unit of the channel at the specified index.
   *
   * @note Unit is expressed as a string and formatted following the SI
   *   (International System of Units), for example: `mm`, `s`, etc.
   *
   * @param engine the engine.
   * @param target the target stroke format.
   * @param index the channel index.
   * @param charset the charset used to encode parameter strings.
   * @param unit the recipient for the channel unit.
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
   * @throws VO_INVALID_USER_BUFFER when `unit` or its `bytes` member is not
   *   writable.
   * @throws VO_INVALID_STATE when no unit is specified for the channel.
   */
  bool (VO_MSE_CALL *getChannelUnitAt)(voEngine engine, voEngineObject target, int32_t index, voCharset charset, voString* unit);

  /**
   * Returns the quantization value of the channel at the specified index.
   *
   * @param engine the engine.
   * @param target the target stroke format.
   * @param index the channel index.
   * @param quantization the recipient for the channel quantization.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_INVALID_USER_BUFFER when `quantization` is not writable.
   * @throws VO_INVALID_STATE when no quantization is specified for the channel.
   */
  bool (VO_MSE_CALL *getChannelQuantizationAt)(voEngine engine, voEngineObject target, int32_t index, float* quantization);

  /**
   * Returns the minimum value of the channel at the specified index.
   *
   * @param engine the engine.
   * @param target the target stroke format.
   * @param index the channel index.
   * @param min the recipient for the channel minimum value.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_INVALID_USER_BUFFER when `min` is not writable.
   * @throws VO_INVALID_STATE when no minimum is specified for the channel.
   */
  bool (VO_MSE_CALL *getChannelMinAt)(voEngine engine, voEngineObject target, int32_t index, float* min);

  /**
   * Returns the maximum value of the channel at the specified index.
   *
   * @param engine the engine.
   * @param target the target stroke format.
   * @param index the channel index.
   * @param max the recipient for the channel maximum value.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_INVALID_USER_BUFFER when `max` is not writable.
   * @throws VO_INVALID_STATE when no maximum is specified for the channel.
   */
  bool (VO_MSE_CALL *getChannelMaxAt)(voEngine engine, voEngineObject target, int32_t index, float* max);

  /**
   * Returns the nominal sample rate of this stroke format.
   *
   * @note The result may slightly differ from the sample rate that was given to
   *   the ink stroke format builder, due to rounding operations.
   *
   * @param engine the engine.
   * @param target the target stroke format.
   *
   * @return the nominal sample rate on success, otherwise a negative value.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   */
  float (VO_MSE_CALL *getSampleRate)(voEngine engine, voEngineObject target);

  /**
   * Tells whether the sample rate is uniform or not.
   *
   * @param engine the engine.
   * @param target the target stroke format.
   *
   * @return `VO_YES` or `VO_NO` on success, otherwise `VO_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   */
  voYesNo (VO_MSE_CALL *isSampleRateUniform)(voEngine engine, voEngineObject target);

  /**
   * Lookups this stroke format for a channel with the specified name.
   *
   * @param engine the engine.
   * @param target the target stroke format.
   * @param charset the charset used to encode parameter strings.
   * @param name the name of the channel to look for.
   *
   * @return the index of the channel when found, otherwise `-1`.
   *   When `-1`, use `voGetError()` to figure out whether an error occurred.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `name` or its `bytes` member is not
   *   readable.
   */
  int32_t (VO_MSE_CALL *lookupChannel)(voEngine engine, voEngineObject target, voCharset charset, const voString* name);
}
voIInkStrokeFormat;

/**
 * Functions composing the `voIInkStrokeFormatBuilder` interface.
 *
 * This interface exposes the public methods of the `voInkStrokeFormatBuilder`
 * type.
 *
 * {@implementingTypes voInkStrokeFormatBuilder}
 */
typedef struct _voIInkStrokeFormatBuilder
{
  /**
   * Clears this stroke format builder before building a new stroke format.
   *
   * @param engine the engine.
   * @param target the target stroke format builder.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a
   *   `voInkStrokeFormatBuilder`.
   */
  bool (VO_MSE_CALL *clear)(voEngine engine, voInkStrokeFormatBuilder target);

  /**
   * Adds a new channel to the stroke format to build.
   *
   * Here below are the predefined names and their recognized meaning:
   *
   * <pre>
   *   Name | Description
   *   ---- | -----------
   *   X    | Horizontal position, increasing to the right.
   *   Y    | Vertical position, increasing downward.
   *   T    | Sample timestamp.
   *   F    | Tip force.
   *   W    | Stroke width.
   *   OTx  | Tilt along the X axis.
   *   OTy  | Tilt along the Y axis.
   *   OA   | Azimuth angle (yaw).
   *   OE   | Elevation angle (pitch).
   *   OR   | Rotation (counter-clockwise rotation around the pen axis).
   * </pre>
   *
   * You can also store application specific values by using a name not found in
   * the table above.
   *
   * @note A stroke format must at least contain `X` and `Y` channels.
   *
   * @param engine the engine.
   * @param target the target stroke format builder.
   * @param charset the charset used to encode parameter strings.
   * @param name the name of channel to be added.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a
   *   `voInkStrokeFormatBuilder`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `name` or its `bytes` member is not
   *   readable.
   * @throws VO_INVALID_STATE when a channel with this `name` already exists.
   */
  bool (VO_MSE_CALL *addChannel)(voEngine engine, voInkStrokeFormatBuilder target, voCharset charset, const voString* name);

  /**
   * Sets the unit of a channel.
   *
   * Unit should be expressed as a string and formatted following the SI
   * (International System of Units), for example: `mm`, `s`, etc.</p>
   *
   * A dpi resolution can be expressed as the invert of a unit, i.e. a 1000
   * dpi resolution corresponds to a unit described as `0.001 in`.
   *
   * @param engine the engine.
   * @param target the target stroke format builder.
   * @param charset the charset used to encode parameter strings.
   * @param name the name of the channel.
   * @param unit the unit of the channel.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a
   *   `voInkStrokeFormatBuilder`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `name` or its `bytes` member is not
   *   readable.
   * @throws VO_INVALID_USER_BUFFER when `unit` or its `bytes` member is not
   *   readable.
   * @throws VO_INVALID_ARGUMENT when `unit` cannot be parsed as a unit string
   *   description.
   */
  bool (VO_MSE_CALL *setChannelUnit)(voEngine engine, voInkStrokeFormatBuilder target, voCharset charset, const voString* name, const voString* unit);

  /**
   * Sets the quantization of a channel.
   *
   * This quantization value is typically used together with minimum and maximum
   * value to enable optimum integer quantization. A value of `0.0f` means the
   * quantization is unknown.
   *
   * @param engine the engine.
   * @param target the target stroke format builder.
   * @param charset the charset used to encode parameter strings.
   * @param name the name of channel.
   * @param quantization the channel quantization.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a
   *   `voInkStrokeFormatBuilder`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `name` or its `bytes` member is not
   *   readable.
   * @throws VO_INVALID_ARGUMENT when `quantization < 0.0f`.
   */
  bool (VO_MSE_CALL *setChannelQuantization)(voEngine engine, voInkStrokeFormatBuilder target, voCharset charset, const voString* name, float quantization);

  /**
   * Sets the minimum value of a channel.
   *
   * This minimum value is typically used together with quantization and maximum
   * value to enable optimum integer quantization.
   *
   * @note The values stored in the channel will not be checked against this
   *   minimum: values below the minimum may be stored in the channel.
   *
   * @param engine the engine.
   * @param target the target stroke format builder.
   * @param charset the charset used to encode parameter strings.
   * @param name the name of channel.
   * @param min the minimum value in the channel.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a
   *   `voInkStrokeFormatBuilder`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `name` or its `bytes` member is not
   *   readable.
   * @throws VO_INVALID_ARGUMENT when `name` does not correspond to a channel.
   */
  bool (VO_MSE_CALL *setChannelMin)(voEngine engine, voInkStrokeFormatBuilder target, voCharset charset, const voString* name, float min);

  /**
   * Sets the maximum value of a channel.
   *
   * This maximum value is typically used together with quantization and maximum
   * value to enable optimum integer quantization.
   *
   * @note The values stored in the channel will not be checked against this
   *   maximum: values below the maximum may be stored in the channel.
   *
   * @param engine the engine.
   * @param target the target stroke format builder.
   * @param charset the charset used to encode parameter strings.
   * @param name the name of channel.
   * @param max the maximum value in the channel.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a
   *   `voInkStrokeFormatBuilder`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `name` or its `bytes` member is not
   *   readable.
   * @throws VO_INVALID_ARGUMENT when `name` does not correspond to a channel.
   */
  bool (VO_MSE_CALL *setChannelMax)(voEngine engine, voInkStrokeFormatBuilder target, voCharset charset, const voString* name, float max);

  /**
   * Sets the sample rate and indicates whether it is uniform or not.
   *
   * @param engine the engine.
   * @param target the target stroke format builder.
   * @param rate the nominal sample rate.
   * @param uniform indicates whether the sample rate is uniform or not.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a
   *   `voInkStrokeFormatBuilder`.
   */
  bool (VO_MSE_CALL *setSampleRate)(voEngine engine, voInkStrokeFormatBuilder target, float rate, bool uniform);

  /**
   * Creates the stroke format.
   *
   * @param engine the engine.
   * @param target the target stroke format builder.
   *
   * @return the newly created `voInkStrokeFormat` instance on success,
   *   otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a
   *   `voInkStrokeFormatBuilder`.
   * @throws VO_LIMIT_EXCEEDED when the number of `voInkStrokeFormat` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   */
  voInkStrokeFormat (VO_MSE_CALL *createStrokeFormat)(voEngine engine, voInkStrokeFormatBuilder target);

  /**
   * Creates a stroke format from its string representation.
   *
   * @param engine the engine.
   * @param charset the charset used to encode parameter strings.
   * @param string the string representation of the stroke format to be created.
   *
   * @return the newly created `voInkStrokeFormat` instance on success,
   *   otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `string` or its `bytes` member is not
   *   readable.
   * @throws VO_LIMIT_EXCEEDED when the number of `voInkStrokeFormat` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   */
  voInkStrokeFormat (VO_MSE_CALL *parseStrokeFormat)(voEngine engine, voCharset charset, const voString* string);
}
voIInkStrokeFormatBuilder;

/**
 * Functions composing the `voIInkStroke` interface.
 *
 * {@implementingTypes voInkStroke, voInkStrokeBuilder}
 */
typedef struct _voIInkStroke
{
  /**
   * Returns the format of this stroke.
   *
   * @param engine the engine.
   * @param target the target stroke.
   *
   * @return the stroke format on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_LIMIT_EXCEEDED when the number of `voInkStrokeFormat` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   */
  voInkStrokeFormat (VO_MSE_CALL *getFormat)(voEngine engine, voEngineObject target);

  /**
   * Returns the timestamp of the stroke as the number of microseconds elapsed
   * since the Unix EPOCH (January 1, 1970, 00:00:00 GMT).
   *
   * @param engine the engine.
   * @param target the target stroke.
   *
   * @return the timestamp of the stroke on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   */
  int64_t (VO_MSE_CALL *getTimestamp)(voEngine engine, voEngineObject target);

  /**
   * Tells whether the first point of the stroke corresponds to the left
   * boundary of a left open interval.
   *
   * @param engine the engine.
   * @param target the target stroke.
   *
   * @return `VO_YES` or `VO_NO` on success, otherwise `VO_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   */
  voYesNo (VO_MSE_CALL *isFirstPointIncluded)(voEngine engine, voEngineObject target);

  /**
   * Tells whether the last point of the stroke corresponds to the right
   * boundary of a right open interval.
   *
   * @param engine the engine.
   * @param target the target stroke.
   *
   * @return `VO_YES` or `VO_NO` on success, otherwise `VO_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   */
  voYesNo (VO_MSE_CALL *isLastPointIncluded)(voEngine engine, voEngineObject target);

  /**
   * Tells whether the first point of the stroke corresponds to the beginning
   * of a raw stroke.
   *
   * @param engine the engine.
   * @param target the target stroke.
   *
   * @return `VO_YES` or `VO_NO` on success, otherwise `VO_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   */
  voYesNo (VO_MSE_CALL *isFirstPointPenDown)(voEngine engine, voEngineObject target);

  /**
   * Tells whether the last point of the stroke corresponds to the end of a
   * raw stroke.
   *
   * @param engine the engine.
   * @param target the target stroke.
   *
   * @return `VO_YES` or `VO_NO` on success, otherwise `VO_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   */
  voYesNo (VO_MSE_CALL *isLastPointPenUp)(voEngine engine, voEngineObject target);

  /**
   * Returns the number of samples in this stroke.
   *
   * @param engine the engine.
   * @param target the target stroke.
   *
   * @return the number of samples on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   */
  int32_t (VO_MSE_CALL *getSampleCount)(voEngine engine, voEngineObject target);

  /**
   * Tells whether this stroke has interleaved floating-point `X` and `Y`
   * channels that can be retrieved directly as an array of `voPoint`.
   *
   * @param engine the engine.
   * @param target the target stroke.
   *
   * @return `VO_YES` or `VO_NO` on success, otherwise `VO_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   */
  voYesNo (VO_MSE_CALL *isPointArrayCompatible)(voEngine engine, voEngineObject target);

  /**
   * Returns the points array for this stroke.
   *
   * @note This function can be called only if `isPointArrayCompatible()`
   *   returns `VO_YES`.
   *
   * @param engine the engine.
   * @param target the target stroke.
   *
   * @return the array of points for this stroke on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_INVALID_STATE when this function cannot be called (use
   *   `getPoints()` instead).
   */
  const voPoint* (VO_MSE_CALL *getPointArray)(voEngine engine, voEngineObject target);

  /**
   * Returns the points of the stroke.
   *
   * @param engine the engine.
   * @param target the target stroke.
   * @param sampleOffset the offset of the points to be retrieved.
   * @param sampleCount the number of points to be retrieved.
   * @param points the recipient for the points.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `sampleOffset` and/or `sampleCount` are
   *   out of bounds.
   * @throws VO_INVALID_USER_BUFFER when `points` is not writable.
   */
  bool (VO_MSE_CALL *getPoints)(voEngine engine, voEngineObject target,
                                int32_t sampleOffset, int32_t sampleCount,
                                voPoint* points);

  /**
   * Tells whether the sample data for a given channel can be retrieved as
   * a direct `float` array.
   *
   * @param engine the engine.
   * @param target the target stroke.
   * @param channelIndex the channel index.
   *
   * @return `VO_YES` or `VO_NO` on success, otherwise `VO_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `channelIndex` is out of bounds.
   */
  voYesNo (VO_MSE_CALL *isFloatArrayCompatible)(voEngine engine, voEngineObject target, int32_t channelIndex);

  /**
   * Returns the `float` array corresponding to the stroke samples of a given
   * channel.
   *
   * @note This function can be called only if `isFloatArrayCompatible()`
   *   returns `VO_YES`.
   *
   * @param engine the engine.
   * @param target the target stroke.
   * @param channelIndex the channel index.
   *
   * @return a pointer to the `float` array that contains the channel samples on
   *   success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `channelIndex` is out of bounds.
   * @throws VO_INVALID_STATE when this function cannot be called (use
   *   `getValuesAsFloat()` instead).
   */
  const float* (VO_MSE_CALL *getFloatArray)(voEngine engine, voEngineObject target, int32_t channelIndex);

  /**
   * Returns the stroke samples of a given channel as `float` values.
   *
   * @param engine the engine.
   * @param target the target stroke.
   * @param channelIndex the channel index.
   * @param sampleOffset the offset of the samples to be retrieved.
   * @param sampleCount the number of samples to be retrieved.
   * @param values the recipient for the values.
   * @param byteStride the offset, in bytes, between 2 values.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `channelIndex` is out bounds.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `sampleOffset` and/or `sampleCount` are
   *   out of bounds.
   * @throws VO_INVALID_USER_BUFFER when `values` is not writable.
   */
  bool (VO_MSE_CALL *getValuesAsFloat)(voEngine engine, voEngineObject target,
                                       int32_t channelIndex,
                                       int32_t sampleOffset, int32_t sampleCount,
                                       float* values, size_t byteStride);

  /**
   * Tells whether the sample data for a given channel can be retrieved as a
   * direct `int32_t` array.
   *
   * @param engine the engine.
   * @param target the target stroke.
   * @param channelIndex the channel index.
   *
   * @return `VO_YES` or `VO_NO` on success, otherwise `VO_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `channelIndex` is out of bounds.
   */
  voYesNo (VO_MSE_CALL *isInt32ArrayCompatible)(voEngine engine, voEngineObject target, int32_t channelIndex);

  /**
   * Returns the `int32_t` array corresponding to the stroke samples of a given
   * channel.
   *
   * @param engine the engine.
   * @param target the target stroke.
   * @param channelIndex the channel index.
   *
   * @return a pointer to the `int32_t` array that contains the channel samples
   *   on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `channelIndex` is out of bounds.
   * @throws VO_INVALID_STATE when this function cannot be called (use
   *   `getValuesAsInt32()` instead).
   */
  const int32_t* (VO_MSE_CALL *getInt32Array)(voEngine engine, voEngineObject target, int32_t channelIndex);

  /**
   * Returns the stroke samples of a given channel as `int32_t` values.
   *
   * @param engine the engine.
   * @param target the target stroke.
   * @param channelIndex the channel index.
   * @param sampleOffset the offset of the samples to be retrieved.
   * @param sampleCount the number of samples to be retrieved.
   * @param values the recipient for the values.
   * @param byteStride the offset, in bytes, between 2 values.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `channelIndex` is out bounds.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `sampleOffset` and/or `sampleCount` are
   *   out of bounds.
   * @throws VO_INVALID_USER_BUFFER when `values` is not writable.
   */
  bool (VO_MSE_CALL *getValuesAsInt32)(voEngine engine, voEngineObject target,
                                       int32_t channelIndex,
                                       int32_t sampleOffset, int32_t sampleCount,
                                       int32_t* values, size_t byteStride);

  /**
   * Tells whether the sample data for a given channel can be retrieved as
   * a direct `int64_t` array.
   *
   * @param engine the engine.
   * @param target the target stroke.
   * @param channelIndex the channel index.
   *
   * @return `VO_YES` or `VO_NO` on success, otherwise `VO_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `channelIndex` is out of bounds.
   */
  voYesNo (VO_MSE_CALL *isInt64ArrayCompatible)(voEngine engine, voEngineObject target, int32_t channelIndex);

  /**
   * Returns the `int64_t` array corresponding to the stroke samples of a given
   * channel.
   *
   * @param engine the engine.
   * @param target the target stroke.
   * @param channelIndex the channel index.
   *
   * @return a pointer to the `int64_t` array that contains the channel samples
   *   on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `channelIndex` is out of bounds.
   * @throws VO_INVALID_STATE when this function cannot be called (use
   *   `getValuesAsInt64()` instead).
   */
  const int64_t* (VO_MSE_CALL *getInt64Array)(voEngine engine, voEngineObject target, int32_t channelIndex);

  /**
   * Returns the stroke samples of a given channel as `int64_t` values.
   *
   * @param engine the engine.
   * @param target the target stroke.
   * @param channelIndex the channel index.
   * @param sampleOffset the offset of the samples to be retrieved.
   * @param sampleCount the number of samples to be retrieved.
   * @param values the recipient for the values.
   * @param byteStride the offset, in bytes, between 2 values.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `channelIndex` is out bounds.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `sampleOffset` and/or `sampleCount` are
   *   out of bounds.
   * @throws VO_INVALID_USER_BUFFER when `values` is not writable.
   */
  bool (VO_MSE_CALL *getValuesAsInt64)(voEngine engine, voEngineObject target,
                                       int32_t channelIndex,
                                       int32_t sampleOffset, int32_t sampleCount,
                                       int64_t* values, size_t byteStride);
}
voIInkStroke;

/**
 * Holds the parameters required to create a new `voGlyph` instance.
 *
 * @see voGlyph, voCreateGlyph1
 */
typedef struct _voGlyphInitializer1
{
  /**
   * The input character describing the glyph.
   */
  voInputCharacter character;

  /**
   * The extent (or bounding box) of the glyph.
   */
  voExtent extent;

  /**
   * The charset used to encode `unit`, or `NULL` to use the default charset.
   */
  voCharset charset;

  /**
   * The unit in which `extent` is expressed, formatted following the SI
   * (International System of Units), for example: `mm`, `s`, etc
   *
   * @note `unit` may be left empty, which indicates that no unit is specified.
   */
  voString unit;
}
voGlyphInitializer1;

/**
 * Holds the parameters required to create a new `voGlyph` instance.
 *
 * @see voGlyph, voCreateGlyph2
 */
typedef struct _voGlyphInitializer2
{
  /**
   * The input character describing the glyph.
   */
  voInputCharacter character;

  /**
   * The glyph bounding parallelogram, the origin point being the top left point
   * for a straight glyph, and u, the vector on top of the glyph.
   */
  voParallelogram parallelogram;

  /**
   * The charset used to encode `unit`, or `NULL` to use the default charset.
   */
  voCharset charset;

  /**
   * The unit in which `parallelogram` is expressed, formatted following the SI
   * (International System of Units), for example: `mm`, `s`, etc.
   *
   * @note `unit` may be left empty, which indicates that no unit is specified.
   */
  voString unit;
}
voGlyphInitializer2;

/**
 * Represents the typography lines of a glyph.
 */
typedef struct _voGlyphLines
{
  /**
   * The ratio between the distance from the top side of the glyph to the
   * baseline, and the height of the glyph:
   *   `baseline = (baseline_position - top_of_glyph) / glyph_height`.
   *
   * This ratio can be greater than `1` or negative if this glyph is not
   * properly placed on a long text line.
   */
  float baseline;

  /**
   * The positive ratio between the height of the letter 'x', and the height of
   * the glyph :
   *   `x_height = height_of_the_letter_x / glyph_height`.
   *
   * @note `0` means unspecified.
   */
  float x_height;
}
voGlyphLines;

/**
 * Holds the parameters required to create a new `voGlyph` instance.
 *
 * @see voGlyph, voCreateGlyph3
 */
typedef struct _voGlyphInitializer3
{
  /**
   * The input character describing the glyph.
   */
  voInputCharacter character;

  /**
   * The glyph bounding parallelogram, the origin point being the top left point
   * for a straight glyph, and u, the vector on top of the glyph.
   */
  voParallelogram parallelogram;

  /**
   * The glyph typography lines.
   */
  voGlyphLines lines;

  /**
   * The charset used to encode `unit`, or `NULL` to use the default charset.
   */
  voCharset charset;

  /**
   * The unit in which `parallelogram` is expressed, formatted following the SI
   * (International System of Units), for example: `mm`, `s`, etc.
   *
   * @note `unit` may be left empty, which indicates that no unit is specified.
   */
  voString unit;
}
voGlyphInitializer3;

#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voGlyph` instance.
 *
 * @param engine the engine.
 * @param character the input character describing the glyph.
 * @param extent the extent (or bounding box) of the glyph.
 * @param charset the charset used to encode parameter strings.
 * @param unit the unit in which `extent` is expressed.
 *
 * @return the newly created `voGlyph` instance on success, otherwise `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_NO_SUCH_OBJECT when the `character` reference is invalid.
 * @throws VO_INVALID_OBJECT when `character` is not a `voInputCharacter`.
 * @throws VO_INVALID_USER_BUFFER when `extent` is not readable.
 * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
 * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
 *   default charset was set.
 * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
 * @throws VO_INVALID_USER_BUFFER when `unit` or its `bytes` member is not
 *   readable.
 * @throws VO_LIMIT_EXCEEDED when the number of `voGlyph` instances would exceed
 *   the allowed limit (please remember that the limit can be shared by several
 *   types).
 */
VO_INLINE voGlyph voCreateGlyph1(voEngine engine, voInputCharacter character, const voExtent* extent, voCharset charset, const voString* unit)
{
  voGlyphInitializer1 initializer = { character, *extent, charset, *unit };
  return voCreateObjectEx(engine, VO_Glyph, &initializer, sizeof(initializer));
}

/**
 * Creates a new `voGlyph` instance.
 *
 * @param engine the engine.
 * @param character the input character describing the glyph.
 * @param parallelogram the bounding parallelogram of the glyph, the origin
 *   point being the top left point for a straight glyph, and u, the vector on
 *   top of the glyph.
 * @param charset the charset used to encode parameter strings.
 * @param unit the unit in which `parallelogram` is expressed.
 *
 * @return the newly created `voGlyph` instance on success, otherwise `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_NO_SUCH_OBJECT when the `character` reference is invalid.
 * @throws VO_INVALID_OBJECT when `character` is not a `voInputCharacter`.
 * @throws VO_INVALID_USER_BUFFER when `parallelogram` is not readable.
 * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
 * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
 *   default charset was set.
 * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
 * @throws VO_INVALID_USER_BUFFER when `unit` or its `bytes` member is not
 *   readable.
 * @throws VO_LIMIT_EXCEEDED when the number of `voGlyph` instances would exceed
 *   the allowed limit (please remember that the limit can be shared by several
 *   types).
 */
VO_INLINE voGlyph voCreateGlyph2(voEngine engine, voInputCharacter character, const voParallelogram* parallelogram, voCharset charset, const voString* unit)
{
  voGlyphInitializer2 initializer = { character, *parallelogram, charset, *unit };
  return voCreateObjectEx(engine, VO_Glyph, &initializer, sizeof(initializer));
}

/**
 * Creates a new `voGlyph` instance.
 *
 * @param engine the engine.
 * @param character the input character describing the glyph.
 * @param parallelogram the bounding parallelogram of the glyph, the origin
 *   point being the top left point for a straight glyph, and u, the vector on
 *   top of the glyph.
 * @param lines the glyph typography lines.
 * @param charset the charset used to encode parameter strings.
 * @param unit the unit in which `parallelogram` is expressed.
 *
 * @return the newly created `voGlyph` instance on success, otherwise `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_NO_SUCH_OBJECT when the `character` reference is invalid.
 * @throws VO_INVALID_OBJECT when `character` is not a `voInputCharacter`.
 * @throws VO_INVALID_USER_BUFFER when `parallelogram` is not readable.
 * @throws VO_INVALID_USER_BUFFER when `lines` is not readable.
 * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
 * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
 *   default charset was set.
 * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
 * @throws VO_INVALID_USER_BUFFER when `unit` or its `bytes` member is not
 *   readable.
 * @throws VO_INVALID_ARGUMENT the `x_height` member of `lines` is negative.
 * @throws VO_INVALID_ARGUMENT `lines` float values are invalid.
 * @throws VO_LIMIT_EXCEEDED when the number of `voGlyph` instances would exceed
 *   the allowed limit (please remember that the limit can be shared by several
 *   types).
 */
VO_INLINE voGlyph voCreateGlyph3(voEngine engine, voInputCharacter character, const voParallelogram* parallelogram, const voGlyphLines* lines, voCharset charset, const voString* unit)
{
  voGlyphInitializer3 initializer = { character, *parallelogram, *lines, charset, *unit };
  return voCreateObjectEx(engine, VO_Glyph, &initializer, sizeof(initializer));
}
#endif // end of: #ifndef VO_NO_FUNCTIONS

/**
 * Functions composing the `voIGlyph` interface.
 *
 * This interface exposes the public methods of the `voGlyph` type.
 *
 * {@implementingTypes voGlyph}
 */
typedef struct _voIGlyph
{
  /**
   * Returns the most probable label of this glyph.
   *
   * @param engine the engine.
   * @param target the target glyph.
   * @param charset the charset used to encode parameter strings.
   * @param label the recipient for the label.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voGlyph`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `label` or its `bytes` member is not
   *   writable.
   */
  bool (VO_MSE_CALL *getLabel)(voEngine engine, voGlyph target, voCharset charset, voString* label);

  /**
   * Returns the parallelogram that defines this glyph.
   *
   * @param engine the engine.
   * @param target the target glyph.
   * @param parallelogram the recipient for the bounding parallelogram.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voGlyph`.
   * @throws VO_INVALID_USER_BUFFER when `parallelogram` is not writable.
   */
  bool (VO_MSE_CALL *getParallelogram)(voEngine engine, voGlyph target, voParallelogram* parallelogram);

  /**
   * Tells whether this glyph has typography lines defined.
   *
   * @param engine the engine.
   * @param target the target glyph.
   *
   * @return `VO_YES` or `VO_NO` on success, otherwise `VO_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voGlyph`.
   */
  voYesNo (VO_MSE_CALL *hasLines)(voEngine engine, voGlyph target);

  /**
   * Returns the typography lines of this glyph.
   *
   * @param engine the engine.
   * @param target the target glyph.
   * @param lines the recipient for the typography lines.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voGlyph`.
   * @throws VO_INVALID_USER_BUFFER when `lines` is not writable.
   */
  bool (VO_MSE_CALL *getLines)(voEngine engine, voGlyph target, voGlyphLines* lines);

  /**
   * Returns the alternate count of this glyph.
   *
   * @param engine the engine.
   * @param target the target glyph.
   *
   * @return the alternate count on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voGlyph`.
   */
  int32_t (VO_MSE_CALL *getAlternateCount)(voEngine engine, voGlyph target);

  /**
   * Returns the alternate at the specified index.
   *
   * @param engine the engine.
   * @param target the target glyph.
   * @param index the index of the alternate to be retrieved.
   * @param p the recipient for the probability associated with the alternate.
   * @param charset the charset used to encode parameter strings.
   * @param label the recipient for the label.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voGlyph`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_INVALID_USER_BUFFER when `p` is not writable.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `label` or its `bytes` member is not
   *   writable.
   */
  bool (VO_MSE_CALL *getAlternateAt)(voEngine engine, voGlyph target, int32_t index, float* p, voCharset charset, voString* label);
}
voIGlyph;


/**
 * Lists the decoration types.
 *
 * @note This enumeration matches the `voShapeDecorationType` enum defined in
 *   `MyScriptShape.h`.
 */
typedef enum _voInkDecorationType
{
  VO_INK_DECORATION_ERR = -1,   /**< Error marker. */

  VO_INK_DECORATION_NONE,       /**< No decoration. */
  VO_INK_DECORATION_ARROW_HEAD, /**< Arrow head. */

  _voInkDecorationType_FORCE_INT_SIZE = 0x7FFFFFFF
}
voInkDecorationType;

/**
 * Holds the parameters required to create a new `voArcPrimitive` instance,
 * using center parameterization.
 *
 * @see voArcPrimitive, voCreateArcPrimitiveFromCenterParametrization
 */
typedef struct _voArcPrimitiveInitializer
{
  /**
   * The geometric parametrization of the primitive.
   *
   * The angle parameters can be modified internally so that the `phi` angle
   * belongs to the interval `]-pi/2 ; pi/2]`.
   */
  voCenterEllipticArc data;

  /**
   * The type of decoration at the first point of the primitive,
   * `VO_INK_DECORATION_NONE` by default.
   */
  voInkDecorationType firstDecoration;

  /**
   * The type of decoration at the last point of the primitive,
   * `VO_INK_DECORATION_NONE` by default.
   */
  voInkDecorationType lastDecoration;

  /**
   * The charset used to encode `unit`, or `NULL` to use the default charset.
   */
  voCharset charset;

  /**
   * The unit in which `data` is expressed, formatted following the SI
   * (International System of Units), for example: `mm`, `s`, etc.
   *
   * @note `unit` may be left empty, which indicates that no unit is specified.
   */
  voString unit;
}
voArcPrimitiveInitializer;

/**
 * Holds the parameters required to create a new `voArcPrimitive` instance,
 * using endpoint parameterization.
 *
 * @see voArcPrimitive, voCreateArcPrimitiveFromEndPointParametrization
 */
typedef struct _voArcPrimitiveInitializer2
{
  /**
   * The geometric parametrization of the primitive.
   *
   * The angle parameter `phi` might be modified internally so that it
   * belongs to the interval `]-pi/2 ; pi/2]`.
   *
   * The radii `rx` and `ry` also might be scaled to fit the provided end
   * points.
   */
  voEndpointEllipticArc data;

  /**
   * The type of decoration at the first point of the primitive,
   * `VO_INK_DECORATION_NONE` by default.
   */
  voInkDecorationType firstDecoration;

  /**
   * The type of decoration at the last point of the primitive,
   * `VO_INK_DECORATION_NONE` by default.
   */
  voInkDecorationType lastDecoration;

  /**
   * The charset used to encode `unit`, or `NULL` to use the default charset.
   */
  voCharset charset;

  /**
   * The unit in which `data` is expressed, formatted following the SI
   * (International System of Units), for example: `mm`, `s`, etc.
   *
   * @note `unit` may be left empty, which indicates that no unit is specified.
   */
  voString unit;
}
voArcPrimitiveInitializer2;

#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voArcPrimitive` instance.
 *
 * @param engine the engine.
 * @param data the center parametrization describing the arc primitive.
 * @param firstDecoration the decoration of the first point of the primitive.
 * @param lastDecoration the decoration of the last point of the primitive.
 * @param charset the charset used to encode parameter strings.
 * @param unit the unit in which `data` is expressed.
 *
 * @return the newly created `voArcPrimitive` instance on success, otherwise
 *   `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_INVALID_USER_BUFFER when `data` is not readable.
 * @throws VO_INVALID_ARGUMENT when `data` contains invalid values.
 * @throws VO_INVALID_ARGUMENT when `firstDecoration` is invalid.
 * @throws VO_INVALID_ARGUMENT when `lastDecoration` is invalid.
 * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
 * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
 *   default charset was set.
 * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
 * @throws VO_INVALID_USER_BUFFER when `unit` or its `bytes` member is not
 *   readable.
 * @throws VO_LIMIT_EXCEEDED when the number of `voArcPrimitive` instances would
 *   exceed the allowed limit (please remember that the limit can be shared by
 *   several types).
 */
VO_INLINE voArcPrimitive voCreateArcPrimitiveFromCenterParametrization(voEngine engine, const voCenterEllipticArc* data, voInkDecorationType firstDecoration, voInkDecorationType lastDecoration, voCharset charset, const voString* unit)
{
  voArcPrimitiveInitializer initializer = { *data, firstDecoration, lastDecoration, charset, *unit };
  return voCreateObjectEx(engine, VO_ArcPrimitive, &initializer, sizeof(initializer));
}

/**
 * Creates a new `voArcPrimitive` instance.
 *
 * @param engine the engine.
 * @param data the endpoint parametrization describing the arc primitive.
 * @param firstDecoration the decoration of the first point of the primitive.
 * @param lastDecoration the decoration of the last point of the primitive.
 * @param charset the charset used to encode parameter strings.
 * @param unit the unit in which `data` is expressed.
 *
 * @return the newly created `voArcPrimitive` instance on success, otherwise
 *   `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_INVALID_USER_BUFFER when `data` is not readable.
 * @throws VO_INVALID_ARGUMENT when `data` contains invalid values.
 * @throws VO_INVALID_ARGUMENT when `firstDecoration` is invalid.
 * @throws VO_INVALID_ARGUMENT when `lastDecoration` is invalid.
 * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
 * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
 *   default charset was set.
 * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
 * @throws VO_INVALID_USER_BUFFER when `unit` or its `bytes` member is not
 *   readable.
 * @throws VO_LIMIT_EXCEEDED when the number of `voArcPrimitive` instances would
 *   exceed the allowed limit (please remember that the limit can be shared by
 *   several types).
 */
VO_INLINE voArcPrimitive voCreateArcPrimitiveFromEndPointParametrization(voEngine engine, const voEndpointEllipticArc* data, voInkDecorationType firstDecoration, voInkDecorationType lastDecoration, voCharset charset, const voString* unit)
{
  voArcPrimitiveInitializer2 initializer = { *data, firstDecoration, lastDecoration, charset, *unit };
  return voCreateObjectEx(engine, VO_ArcPrimitive, &initializer, sizeof(initializer));
}
#endif // end of: #ifndef VO_NO_FUNCTIONS

/**
 * Holds the parameters required to create a new `voLinePrimitive` instance.
 *
 * @see voLinePrimitive, voCreateLinePrimitive
 */
typedef struct _voLinePrimitiveInitializer
{
  /**
   * The line segment describing the line primitive.
   */
  voLineSegment data;

  /**
   * The type of decoration at the first point of the primitive,
   * `VO_INK_DECORATION_NONE` by default.
   */
  voInkDecorationType firstDecoration;

  /**
   * The type of decoration at the last point of the primitive,
   * `VO_INK_DECORATION_NONE` by default.
   */
  voInkDecorationType lastDecoration;

  /**
   * The charset used to encode `unit`, or `NULL` to use the default charset.
   */
  voCharset charset;

  /**
   * The unit in which `data` is expressed, formatted following the SI
   * (International System of Units), for example: `mm`, `s`, etc.
   *
   * @note `unit` may be left empty, which indicates that no unit is specified.
   */
  voString unit;
}
voLinePrimitiveInitializer;

#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voLinePrimitive` instance.
 *
 * @param engine the engine.
 * @param data the line segment describing the primitive.
 * @param firstDecoration the decoration of the first point of the primitive.
 * @param lastDecoration the decoration of the last point of the primitive.
 * @param charset the charset used to encode parameter strings.
 * @param unit the unit in which `data` is expressed.
 *
 * @return the newly created `voLinePrimitive` instance on success, otherwise
 *   `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_INVALID_USER_BUFFER when `data` is not readable.
 * @throws VO_INVALID_ARGUMENT when `data` contains invalid values.
 * @throws VO_INVALID_ARGUMENT when `firstDecoration` is invalid.
 * @throws VO_INVALID_ARGUMENT when `lastDecoration` is invalid.
 * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
 * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
 *   default charset was set.
 * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
 * @throws VO_INVALID_USER_BUFFER when `unit` or its `bytes` member is not
 *   readable.
 * @throws VO_LIMIT_EXCEEDED when the number of `voLinePrimitive` instances
 *   would exceed the allowed limit (please remember that the limit can be
 *   shared by several types).
 */
VO_INLINE voLinePrimitive voCreateLinePrimitive(voEngine engine, const voLineSegment* data, voInkDecorationType firstDecoration, voInkDecorationType lastDecoration, voCharset charset, const voString* unit)
{
  voLinePrimitiveInitializer initializer = { *data, firstDecoration, lastDecoration, charset, *unit };
  return voCreateObjectEx(engine, VO_LinePrimitive, &initializer, sizeof(initializer));
}
#endif // end of: #ifndef VO_NO_FUNCTIONS

/**
 * Functions composing the `voIPrimitive` interface.
 *
 * {@implementingTypes voArcPrimitive, voLinePrimitive}
 */
typedef struct _voIPrimitive
{
  /**
   * Returns the intrinsic geometric parameters of this primitive.
   *
   * @note The actual type of the returned data depends on the type of the
   *   target primitive, resolved by the provided size of data recipient:
   *
   * @param engine the engine.
   * @param target the target primitive.
   * @param data the recipient for the geometric parameters. For a
   *   `voLinePrimitive` you must give a pointer to a `voLineSegment`.
   *   For a `voArcPrimitive` you must give either a pointer to a
   *   `voEndpointEllipticArc` or a pointer to a `voCenterEllipticArc`.
   * @param sizeOfData the size of the data recipient.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_INVALID_USER_BUFFER when `data` is not writable.
   * @throws VO_INVALID_ARGUMENT when `sizeOfData` does not correspond to one of
   *   the possible data for the target primitive.
   */
  bool (VO_MSE_CALL *getData)(voEngine engine, voEngineObject target, void* data, size_t sizeOfData);

  /**
   * Returns the type of decoration at the first endpoint of this primitive.
   *
   * @param engine the engine.
   * @param target the target primitive.
   *
   * @return the type of the decoration on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   */
  voInkDecorationType (VO_MSE_CALL *getFirstDecoration)(voEngine engine, voEngineObject target);

  /**
   * Returns the tangent angle at the first endpoint of this primitive.
   *
   * @note This is a derived property that is computed from the intrinsic
   *   geometric parameters.
   *
   * @param engine the engine.
   * @param target the target primitive.
   * @param angle the recipient for the computed tangent angle.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_INVALID_USER_BUFFER when `angle` is not writable.
   */
  bool (VO_MSE_CALL *getFirstTangentAngle)(voEngine engine, voEngineObject target, float* angle);

  /**
   * Returns the type of the decoration at the last endpoint of this primitive.
   *
   * @param engine the engine.
   * @param target the target primitive.
   *
   * @return the type of decoration on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   */
  voInkDecorationType (VO_MSE_CALL *getLastDecoration)(voEngine engine, voEngineObject target);

  /**
   * Returns the tangent angle at the last endpoint of this primitive.
   *
   * @note This is a derived property that is computed from the intrinsic
   *   geometric parameters.
   *
   * @param engine the engine.
   * @param target the target primitive.
   * @param angle the recipient for the computed tangent angle.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_INVALID_USER_BUFFER when `angle` is not writable.
   */
  bool (VO_MSE_CALL *getLastTangentAngle)(voEngine engine, voEngineObject target, float* angle);
}
voIPrimitive;

/**
 * Functions composing the `voIInkStrokeBuilder` interface.
 *
 * This interface exposes the public methods of the `voInkStrokeBuilder` type
 *
 * {@implementingTypes voInkStrokeBuilder}
 */
typedef struct _voIInkStrokeBuilder
{
  /**
   * Clears this stroke builder before building a new stroke.
   *
   * @param engine the engine.
   * @param target the target stroke builder.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voInkStrokeBuilder`.
   */
  bool (VO_MSE_CALL *clear)(voEngine engine, voInkStrokeBuilder target);

  /**
   * Adds a new sample to the stroke being built.
   *
   * @param engine the engine.
   * @param target the target stroke builder.
   * @param signature the description of a sample, with one character per
   *   channel. For example "FFI" means following parameters will be float,
   *   float and int. Signature must match the channel settings of the stroke
   *   format.
   * @param ... the sample values corresponding to `signature`.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voInkStrokeBuilder`.
   * @throws VO_INVALID_USER_BUFFER when `signature` is not readable.
   * @throws VO_LIMIT_EXCEEDED when the sample count exceeds `32767`.
   * @throws VO_INVALID_ARGUMENT when one of the sample values is invalid.
   */
  bool (VO_MSE_CALL *addSample)(voEngine engine, voInkStrokeBuilder target, const char* signature, ...);

  /**
   * Adds a new sample to the stroke being built.
   *
   * @param engine the engine.
   * @param target the target stroke builder.
   * @param signature the description of a sample, with one character per
   *   channel. For example "FFI" means following parameters will be float,
   *   float and int. Signature must match the channel settings of the stroke
   *   format.
   * @param args the sample values corresponding to `signature` description.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voInkStrokeBuilder`.
   * @throws VO_INVALID_USER_BUFFER when `signature` is not readable.
   * @throws VO_LIMIT_EXCEEDED when the sample count exceeds `32767`.
   * @throws VO_INVALID_ARGUMENT when one of the sample values is invalid.
   */
  bool (VO_MSE_CALL *addSampleV)(voEngine engine, voInkStrokeBuilder target, const char* signature, va_list args);

  /**
   * Adds a new sample to the stroke being built.
   *
   * @param engine the engine.
   * @param target the target stroke builder.
   * @param values a table containing the new sample channel values.
   * @param count the number of channels in `values`.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voInkStrokeBuilder`.
   * @throws VO_INVALID_USER_BUFFER when `values` is not readable.
   * @throws VO_INVALID_ARGUMENT when `count <= 0` or `count` is greater than
   *   the number of channels.
   * @throws VO_LIMIT_EXCEEDED when the sample count exceeds `32767`.
   * @throws VO_INVALID_ARGUMENT when one of the sample values is invalid.
   */
  bool (VO_MSE_CALL *addSampleA_f)(voEngine engine, voInkStrokeBuilder target, const float values[], int32_t count);

  /**
   * Creates the built stroke.
   *
   * @param engine the engine.
   * @param target the target stroke builder.
   *
   * @return the newly created `voInkStroke` instance on success, otherwise
   *   `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voInkStrokeBuilder`.
   * @throws VO_INVALID_STATE when `target` does not contains any sample.
   * @throws VO_LIMIT_EXCEEDED when the number of `voInkStroke` instances would
   *   exceed the allowed limit (please remember that the limit can be shared by
   *   several types).
   */
  voInkStroke (VO_MSE_CALL *createStroke)(voEngine engine, voInkStrokeBuilder target);
}
voIInkStrokeBuilder;

/**
 * Functions composing the `voIInkStrokeSet` interface.
 *
 * {@implementingTypes voInk, voInkIntervals}
 */
typedef struct _voIInkStrokeSet
{
  /**
   * Returns the number of strokes in this stroke set.
   *
   * @param engine the engine.
   * @param target the target stroke set.
   *
   * @return the number of strokes on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   */
  int32_t (VO_MSE_CALL *getStrokeCount)(voEngine engine, voEngineObject target);

  /**
   * Returns the stroke at the specified index.
   *
   * @param engine the engine.
   * @param target the target stroke set.
   * @param index the index of the stroke to be retrieved.
   *
   * @return the requested stroke on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_LIMIT_EXCEEDED when the number of `voInkStroke` instances would
   *   exceed the allowed limit (please remember that the limit can be shared by
   *   several types).
   */
  voInkStroke (VO_MSE_CALL *getStrokeAt)(voEngine engine, voEngineObject target, int32_t index);
}
voIInkStrokeSet;

/**
 * Functions composing the `voIInkIntervals` interface.
 *
 * This interface exposes the public methods of the `voInkIntervals` type.
 *
 * {@implementingTypes voInkIntervals}
 */
typedef struct _voIInkIntervals
{
  /**
   * Converts this intervals into a segment.
   *
   * @param engine the engine.
   * @param target the target intervals.
   *
   * @return the ink segment corresponding to the selection on success,
   *   otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voInkIntervals`.
   * @throws VO_LIMIT_EXCEEDED when the number of `voInkSegment` instances would
   *   exceed the allowed limit (please remember that the limit can be shared by
   *   several types).
   */
  voInkSegment (VO_MSE_CALL *toSegment)(voEngine engine, voInkIntervals target);

  /**
   * Returns the number of intervals that compose this ink intervals.
   *
   * @param engine the engine.
   * @param target the target intervals.
   *
   * @return the number of intervals on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voInkIntervals`.
   */
  int32_t (VO_MSE_CALL *getIntervalCount)(voEngine engine, voInkIntervals target);

  /**
   * Returns the interval at the specified index.
   *
   * @param engine the engine.
   * @param target the target intervals.
   * @param index the index of the interval to be retrieved.
   * @param result the recipient for the interval value.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voInkIntervals`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_INVALID_ARGUMENT when `result` is not writable.
   */
  bool (VO_MSE_CALL *getIntervalAt)(voEngine engine, voInkIntervals target,
                                    int32_t index, voInkInterval* result);

  /**
   * Lookups this intervals for the specified cursor.
   *
   * @param engine the engine.
   * @param target the target intervals.
   * @param cursor the cursor to search for.
   * @param index the recipient for the interval index.
   *
   * @return `VO_YES` when the cursor is inside the interval at the returned
   *   index, `VO_NO` when the cursor is before the interval at the returned
   *   index, otherwise `VO_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voInkIntervals`.
   * @throws VO_INVALID_USER_BUFFER when `cursor` is not readable.
   * @throws VO_INVALID_USER_BUFFER when `index` is not writable.
   */
  voYesNo (VO_MSE_CALL *lookup)(voEngine engine, voInkIntervals target,
                                const voInkCursor* cursor, int32_t* index);

  /**
   * Tells whether the two intervals intersects or not.
   *
   * @param engine the engine.
   * @param target the target intervals.
   * @param other the other intervals to test the intersection with.
   * @param equal a pointer to a boolean value that will be set to `true` when
   *   the intervals are equal.
   *
   * @return `VO_YES` or `VO_NO` on success, otherwise `VO_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voInkIntervals`.
   * @throws VO_NO_SUCH_OBJECT when the `other` reference is invalid.
   * @throws VO_INVALID_OBJECT when `other` is not a `voInkIntervals`.
   * @throws VO_INVALID_USER_BUFFER when `equals` is not writable.
   */
  voYesNo (VO_MSE_CALL *intersects)(voEngine engine, voInkIntervals target,
                                    voInkIntervals other, bool* equal);
}
voIInkIntervals;

/**
 * Represents the way a lookup handles the query intervals parameter.
 */
typedef enum _voInkIntervalsMatchMode
{
  /**
   * Search for objects that strictly include the query.
   */
  VO_INKINTERVALS_MATCH_INCLUDE,

  /**
   * Search for objects that possess exactly the query.
   */
  VO_INKINTERVALS_MATCH_EXACT,

  /**
   * Search for objects that intersect the query.
   */
  VO_INKINTERVALS_MATCH_INTERSECT,

  /**
   * Search for objects that are strictly included by the query.
   */
  VO_INKINTERVALS_MATCH_INCLUDED_BY,

  _voInkIntervalsMatchMode_FORCE_INT_SIZE = 0x7FFFFFFF
}
voInkIntervalsMatchMode;

/**
 * Holds the parameters to run a lookup on tag set.
 */
typedef struct _voInkTagQuery
{
  /**
   * The identifier of the tag to look for, or `-1` for any id.
   */
  int64_t id;

  /**
   * The charset used to encode name, or `NULL` to use the default charset.
   */
  voCharset charset;

  /**
   * The name of the tag(s) to look for, or empty string for any name.
   */
  voString name;

  /**
   * The intervals to look for, or `NULL` for any intervals.
   */
  voInkIntervals intervals;

  /**
   * The intervals match mode : do the provided intervals match exactly
   * (or be included by, or intersect with) the tags's segments?
   */
  voInkIntervalsMatchMode mode;
}
voInkTagQuery;

#define voInkTagQuery_EMPTY_INITIALIZER                       { -1, NULL, { NULL, 0 }, NULL, VO_INKINTERVALS_MATCH_INCLUDE }
#define voInkTagQueryById_INITIALIZER(id)                     { id, NULL, { NULL, 0 }, NULL, VO_INKINTERVALS_MATCH_INCLUDE }
#define voInkTagQueryByVoString_INITIALIZER(string)           { -1, NULL, string, NULL, VO_INKINTERVALS_MATCH_INCLUDE }
#define voInkTagQueryByName_INITIALIZER(name)                 { -1, NULL, voString_INITIALIZER(name), NULL, VO_INKINTERVALS_MATCH_INCLUDE }
#define voInkTagQueryByIntervals_INITIALIZER(intervals, mode) { -1, NULL, { NULL, 0 }, intervals, mode }

/**
 * Functions composing the `voIInk` interface.
 *
 * This interface exposes the public methods of the `voInk` type.
 *
 * {@implementingTypes voInk}
 */
typedef struct _voIInk
{
  /**
   * Adds the specified listener to this ink.
   *
   * @param engine the engine.
   * @param target the target ink.
   * @param listener the listener to be added.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voInk`.
   * @throws VO_NO_SUCH_OBJECT when the `listener` reference is invalid.
   * @throws VO_INVALID_OBJECT when `listener` does not implement the
   *   `voIInkUpdateListener` interface.
   * @throws VO_INVALID_STATE when `listener` is already registered.
   */
  bool (VO_MSE_CALL *addListener)(voEngine engine, voInk target, voUserObject listener);

  /**
   * Removes the specified listener from this ink.
   *
   * @param engine the engine.
   * @param target the target ink.
   * @param listener the listener to be removed.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voInk`.
   * @throws VO_NO_SUCH_OBJECT when the `listener` reference is invalid.
   * @throws VO_INVALID_OBJECT when `listener` does not implement the
   *   `voIInkUpdateListener` interface.
   * @throws VO_INVALID_STATE when `listener` is not registered.
   */
  bool (VO_MSE_CALL *removeListener)(voEngine engine, voInk target, voUserObject listener);

  /**
   * Adds a stroke to this ink.
   *
   * @note A call to this function may change the stroke format and values of
   *   the stroke being added: when the stroke format already in use in the ink
   *   differs from the stroke format of the stroke to be added, samples are
   *   converted to ensure that the units associated with each stroke format
   *   channel are the same for all strokes.
   *
   * @param engine the engine.
   * @param target the target ink.
   * @param stroke the stroke to be added.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voInk`.
   * @throws VO_NO_SUCH_OBJECT when the `stroke` reference is invalid.
   * @throws VO_INVALID_OBJECT when `stroke` is not a `voInkStroke`.
   * @throws VO_INVALID_ARGUMENT when `stroke` is already part of `target` or
   *   already belongs to another ink.
   * @throws VO_LIMIT_EXCEEDED when the stroke count exceeds `32767`.
   */
  bool (VO_MSE_CALL *addStroke)(voEngine engine, voInk target, voInkStroke stroke);

  /**
   * Adds a stroke to this ink and also add it to existing tags.
   *
   * @param engine the engine.
   * @param target the target ink.
   * @param stroke the stroke to be added.
   * @param tagIds the tags to be modified.
   * @param tagCount the number tags.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voInk`.
   * @throws VO_NO_SUCH_OBJECT when the `stroke` reference is invalid.
   * @throws VO_INVALID_OBJECT when `stroke` is not a `voInkStroke`.
   * @throws VO_INVALID_ARGUMENT when `stroke` is already part of `target` or
   *   already belongs to another ink.
   * @throws VO_INVALID_USER_BUFFER when `tagIds` is not readable.
   * @throws VO_INVALID_ARGUMENT when `count` is invalid.
   * @throws VO_INVALID_ARGUMENT when one of the ids in `tagIds` is invalid.
   * @throws VO_LIMIT_EXCEEDED when the stroke count exceeds `32767`.
   */
  bool (VO_MSE_CALL *addStrokeEx)(voEngine engine, voInk target, voInkStroke stroke, const int64_t* tagIds, int32_t tagCount);

  /**
   * Adds all the strokes from the specified stroke set to this ink.
   *
   * @param engine the engine.
   * @param target the target ink.
   * @param strokeSet the set of strokes to paste. Any object implementing the
   *   `voIInkStrokeSet` interface can be used here.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voInk`.
   * @throws VO_NO_SUCH_OBJECT when the `strokeSet` reference is invalid.
   * @throws VO_INVALID_OBJECT when `strokeSet` does not implement the
   *   `voIInkStrokeSet` interface.
   */
  bool (VO_MSE_CALL *paste)(voEngine engine, voInk target, voEngineObject strokeSet);

  /**
   * Erases the specified portion of ink.
   *
   * @param engine the engine.
   * @param target the target ink.
   * @param intervals the ink intervals that represent the portion to be erased.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voInk`.
   * @throws VO_NO_SUCH_OBJECT when the `intervals` reference is invalid.
   * @throws VO_INVALID_OBJECT when `intervals` is not a `voInkIntervals`.
   * @throws VO_INVALID_ARGUMENT when `intervals` does not refer to this ink.
   */
  bool (VO_MSE_CALL *erase)(voEngine engine, voInk target, voInkIntervals intervals);

  /**
   * Chops the strokes at the lower and upper endpoints of each interval from
   * the given intervals.
   *
   * @param engine the engine.
   * @param target the target ink.
   * @param intervals the intervals that represent the strokes to be chopped.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voInk`.
   * @throws VO_NO_SUCH_OBJECT when the `intervals` reference is invalid.
   * @throws VO_INVALID_OBJECT when `intervals` is not a `voInkIntervals`.
   * @throws VO_INVALID_ARGUMENT when `intervals` does not refer to this ink.
   */
  bool (VO_MSE_CALL *chop)(voEngine engine, voInk target, voInkIntervals intervals);

  /**
   * Transforms the strokes referenced by the given intervals.
   *
   * @param engine the engine.
   * @param target the target ink.
   * @param intervals the intervals that represent the strokes to be
   *   transformed.
   * @param transform the transform matrix to be applied.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voInk`.
   * @throws VO_NO_SUCH_OBJECT when the `intervals` reference is invalid.
   * @throws VO_INVALID_OBJECT when `intervals` is not a `voInkIntervals`.
   * @throws VO_INVALID_ARGUMENT when `intervals` does not refer to this ink.
   * @throws VO_INVALID_USER_BUFFER when `transform` is not readable.
   * @throws VO_INVALID_ARGUMENT when `transform` contains invalid values or is
   *   not invertible.
   */
  bool (VO_MSE_CALL *transform)(voEngine engine, voInk target, voInkIntervals intervals, const voTransform* transform);

  /**
   * Returns the raw strokes of this ink as a stroke set.
   *
   * Raw strokes are left unchanged after any of the `erase()`, `chop()`, and
   * `transform()` operations.
   *
   * @param engine the engine.
   * @param target the target ink.
   *
   * @return an object that implements the `voIInkStrokeSet` interface on
   *   success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voInk`.
   * @throws VO_LIMIT_EXCEEDED when the number of instances of the specified
   *   type would exceed the allowed limit (please remember that the limit can
   *   be shared by several types).
   */
  voEngineObject (VO_MSE_CALL *getRawStrokes)(voEngine engine, voInk target);

  /**
   * Returns the interval associated with a given stroke.
   *
   * @param engine the engine.
   * @param target the target ink.
   * @param stroke the stroke to get the interval from.
   * @param result the recipient for the interval.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voInk`.
   * @throws VO_NO_SUCH_OBJECT when the `stroke` reference is invalid.
   * @throws VO_INVALID_OBJECT when `stroke` is not a `voInkStroke`.
   * @throws VO_INVALID_USER_BUFFER when `result` is not writable.
   */
  bool (VO_MSE_CALL *getStrokeInterval)(voEngine engine, voInk target, voInkStroke stroke, voInkInterval* result);

  /**
   * Adds a new tag into this ink.
   *
   * @param engine the engine.
   * @param target the target ink.
   * @param charset the charset used to encode parameter strings.
   * @param name the name to be associated with the tag.
   * @param intervals the intervals that define the segment to be associated
   *   with the tag, or `NULL` if the tag should be empty.
   * @param data the data to be associated with the tag (typically a `voJson`
   *   object), or `NULL` if no data should be associated with the tag.
   *
   * @return the identifier of the added tag on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voInk`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `name` or its `bytes` member is not
   *   readable.
   * @throws VO_INVALID_ARGUMENT when `name` is the empty string.
   * @throws VO_NO_SUCH_OBJECT when the `intervals` reference is invalid.
   * @throws VO_INVALID_OBJECT when `intervals` is not a `voInkIntervals`.
   * @throws VO_INVALID_ARGUMENT when `intervals` does not refer to this ink.
   * @throws VO_NO_SUCH_OBJECT when the `data` reference is invalid.
   */
  int64_t (VO_MSE_CALL *addTag)(voEngine engine, voInk target, voCharset charset, const voString* name, voInkIntervals intervals, voEngineObject data);

  /**
   * Updates a tag from this ink.
   *
   * To keep the data associated with this tag unchanged, set `newData` to
   * `NULL`.
   *
   * To clear the data associated with this tag, set it to a JSON `null` value
   * (@see voJson::createNull()).
   *
   * @param engine the engine.
   * @param target the target ink.
   * @param tagId the identifier of the tag, as returned by `addTag()`.
   * @param newIntervals the intervals that define the segment to be associated
   *   with the tag, or `NULL` to keep the segment unchanged.
   * @param newData the data to be associated with the tag (typically a `voJson`
   *   object), or `NULL` to keep the data unchanged.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voInk`.
   * @throws VO_INVALID_ARGUMENT when `tagId` is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `newIntervals` reference is invalid.
   * @throws VO_INVALID_OBJECT when `newIntervals` is not a `voInkIntervals`.
   * @throws VO_INVALID_ARGUMENT when `newIntervals` does not refer to this ink.
   * @throws VO_INVALID_ARGUMENT when both `newIntervals` and `newData` are
   *   `NULL`.
   * @throws VO_NO_SUCH_OBJECT when the `newData` reference is invalid.
   */
  bool (VO_MSE_CALL *updateTag)(voEngine engine, voInk target, int64_t tagId, voInkIntervals newIntervals, voEngineObject newData);

  /**
   * Returns the number of tags selected by the specified query.
   *
   * @param engine the engine.
   * @param target the target ink.
   * @param query the query that defines the selection criteria.
   *
   * @return the number of selected tags on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voInk`.
   * @throws VO_INVALID_USER_BUFFER when `query` is not readable.
   * @throws VO_INVALID_ARGUMENT when the `query.id` is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `query.charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `query.charset` reference is `NULL` and
   *   no default charset was set.
   * @throws VO_INVALID_OBJECT when `query.charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `query.name` or its `bytes` member is
   *   not readable.
   * @throws VO_NO_SUCH_OBJECT when the `query.intervals` reference is invalid.
   * @throws VO_INVALID_OBJECT when `query.intervals` is not a `voInkIntervals`.
   * @throws VO_INVALID_ARGUMENT when `query.intervals` does not refer to this
   *   ink.
   * @throws VO_INVALID_ARGUMENT when `query.mode` is invalid.
   */
  int32_t (VO_MSE_CALL *tagLookupCount)(voEngine engine, voInk target, const voInkTagQuery* query);

  /**
   * Returns an iterator over the tags selected by the specified query.
   *
   * @param engine the engine.
   * @param target the target ink.
   * @param query the query that defines the selection criteria.
   *
   * @return an iterator over the selected tags on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voInk`.
   * @throws VO_INVALID_USER_BUFFER when `query` is not readable.
   * @throws VO_INVALID_ARGUMENT when the `query.id` is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `query.charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `query.charset` reference is `NULL` and
   *   no default charset was set.
   * @throws VO_INVALID_OBJECT when `query.charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `query.name` or its `bytes` member is
   *   not readable.
   * @throws VO_NO_SUCH_OBJECT when the `query.intervals` reference is invalid.
   * @throws VO_INVALID_OBJECT when `query.intervals` is not a `voInkIntervals`.
   * @throws VO_INVALID_ARGUMENT when `query.intervals` does not refer to this
   *   ink.
   * @throws VO_INVALID_ARGUMENT when `query.mode` is invalid.
   * @throws VO_LIMIT_EXCEEDED when the number of `voInkTagIterator` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   */
   voInkTagIterator (VO_MSE_CALL *tagLookup)(voEngine engine, voInk target, const voInkTagQuery* query);

  /**
   * Returns the identifier associated with the specified stroke.
   *
   * When a stroke is chopped or erased (even partially), its identifier is
   * invalidated.
   *
   * When a stroke is entirely transformed, its identifier remains valid.
   *
   * @param engine the engine.
   * @param target the target ink.
   * @param stroke the stroke to get the identifier from.
   * @param id the recipient for the stroke identifier.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voInk`.
   * @throws VO_NO_SUCH_OBJECT when the `stroke` reference is invalid.
   * @throws VO_INVALID_OBJECT when `stroke` is not a `voInkStroke`.
   * @throws VO_INVALID_ARGUMENT when `stroke` does not belong to `target`.
   * @throws VO_INVALID_USER_BUFFER when `id` is not writable.
   */
  bool (VO_MSE_CALL *getStrokeId)(voEngine engine, voInk target, voInkStroke stroke, int64_t* id);

  /**
   * Returns the stroke associated with the specified identifier.
   *
   * A stroke's identifier can be retrieved by calling `getStrokeId()`.
   *
   * When a stroke is chopped or erased (even partially), its identifier is
   * invalidated.
   *
   * Calling this function with an outdated identifier returns `NULL`.
   *
   * @param engine the engine.
   * @param target the target ink.
   * @param id the identifier of the stroke to be retrieved.
   *
   * @return the stroke associated with `id` if any, otherwise `NULL`.
   *   When `NULL`, use `voGetError()` to figure out whether an error occurred.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voInk`.
   * @throws VO_INVALID_ARGUMENT when `id` does not correspond to a possible
   *   stroke (e.g. it references a deleted stroke).
   */
  voInkStroke (VO_MSE_CALL *getStrokeFromId)(voEngine engine, voInk target, int64_t id);
}
voIInk;

/**
 * Lists the possible values of the `action` member of `voInkUpdateTagInfo`.
 *
 * @see voInkUpdateTagInfo
 */
typedef enum _voTagAction
{
  VO_TAG_ADDED,   /**< The tag was added. */
  VO_TAG_UPDATED, /**< The tag was updated. */
  VO_TAG_DELETED, /**< The tag was deleted. */

  _voTagAction_FORCE_INT_SIZE = 0x7FFFFFFF
}
voTagAction;

/**
 * Represents information about what happened to a tag.
 */
typedef struct _voInkUpdateTagInfo
{
  int64_t tagId;                  /**< The identifier of the tag. */
  voTagAction action;             /**< The action that happened. */
  VO_FLAGS_T(voChangeFlag) flags; /**< The change flags, if any. */
}
voInkUpdateTagInfo;


/**
 * Functions composing the `voIInkUpdateListener` interface.
 *
 * @note This interface is implementable by user objects. User code is not
 *   allowed to invoke this interface directly: as a consequence
 *   `voGetInterface()` returns `NULL`.
 */
typedef struct _voIInkUpdateListener
{
  /**
   * Called when the target change listener is added to an ink object.
   *
   * @param engine the engine.
   * @param target the target change listener.
   * @param ink the ink to which the change listener was added.
   */
  void (VO_MSE_CALL *added)(voEngine engine, voUserObject target, voInk ink);

  /**
   * Called when the target change listener is removed from an ink object.
   *
   * @param engine the engine.
   * @param target the target change listener.
   * @param ink the ink from which the change listener was removed.
   */
  void (VO_MSE_CALL *removed)(voEngine engine, voUserObject target, voInk ink);

  /**
   * Called when the ink has been updated.
   *
   * @param engine the engine.
   * @param target the target change listener.
   * @param ink the ink that has been updated.
   * @param extent the extent of updates on the X and Y axes. Note that you
   *   should add half the pen width to compute screen update rectangle.
   * @param tagInfos an array of information about tag related actions.
   * @param tagInfoCount the number of items in the above array.
   */
  void (VO_MSE_CALL *onUpdate)(voEngine engine, voUserObject target, voInk ink, const voExtent* extent, const voInkUpdateTagInfo* tagInfos, int32_t tagInfoCount);
}
voIInkUpdateListener;


/**
 * Represents the way an input will be combined with an existing selection.
 */
typedef enum _voSelectionModifier
{
  /**
   * Only use the input in the selection (discard existing selection).
   */
  VO_SELECT_SET = 0,

  /**
   * Select the union between the existing selection and the input.
   */
  VO_SELECT_UNION = 1,

  /**
   * Select the intersection of the existing selection and the input.
   */
  VO_SELECT_INTERSECTION = 2,

  /**
   * Select the difference between the existing selection and the input.
   */
  VO_SELECT_DIFFERENCE = 3,

  _voSelectionModifier_FORCE_INT_SIZE = 0x7FFFFFFF
}
voSelectionModifier;


/**
 * Functions composing the `voISelection` interface.
 *
 * {@implementingTypes voInkSelection}
 */
typedef struct _voISelection
{
  /**
   * Adds the specified change listener to this selection.
   *
   * @param engine the engine.
   * @param target the target selection.
   * @param listener the change listener to be added.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_NO_SUCH_OBJECT when the `listener` reference is invalid.
   * @throws VO_INVALID_OBJECT when `listener` does not implement the
   *   `voISelectionChangeListener` interface.
   * @throws VO_INVALID_STATE when `listener` is already registered.
   */
  bool (VO_MSE_CALL *addChangeListener)(voEngine engine, voEngineObject target, voUserObject listener);

  /**
   * Removes the specified change listener from this selection.
   *
   * @param engine the engine.
   * @param target the target selection.
   * @param listener the change listener to be removed.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_NO_SUCH_OBJECT when the `listener` reference is invalid.
   * @throws VO_INVALID_OBJECT when `listener` does not implement the
   *   `voISelectionChangeListener` interface.
   * @throws VO_INVALID_STATE when `listener` is not registered.
   */
  bool (VO_MSE_CALL *removeChangeListener)(voEngine engine, voEngineObject target, voUserObject listener);

  /**
   * Creates a new selection of the items contained in the specified rectangle
   * and combines it with this selection.
   *
   * @param engine the engine.
   * @param target the target selection.
   * @param rectangle the rectangle from which to build the selection.
   * @param modifier the way the new selection should be combined with the
   *   existing selection.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_INVALID_USER_BUFFER when `rectangle` is not readable.
   * @throws VO_INVALID_ARGUMENT when `modifier` is invalid.
   */
  bool (VO_MSE_CALL *selectRectangle)(voEngine engine, voEngineObject target, const voRectangle* rectangle, voSelectionModifier modifier);

  /**
   * Creates a new selection of the items that are contained in the specified
   * polygon and combines it with the existing selection.
   *
   * @param engine the engine.
   * @param target the target selection.
   * @param vertices the array of points that define the polygon from which to
   *   build the selection.
   * @param vertexCount the number of points of the polygon.
   * @param modifier the way the new selection should be combined with the
   *   existing selection.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_INVALID_USER_BUFFER when `vertices` is not readable.
   * @throws VO_INVALID_ARGUMENT when `vertexCount <= 0`.
   * @throws VO_INVALID_ARGUMENT when `modifier` is invalid.
   */
  bool (VO_MSE_CALL *selectPolygon)(voEngine engine, voEngineObject target, const voPoint vertices[], int32_t vertexCount, voSelectionModifier modifier);

  /**
   * Creates a new selection of the items that are contained in the specified
   * circle and combines it with the existing selection.
   *
   * @param engine the engine.
   * @param target the target selection.
   * @param circle the circle from which to build the selection.
   * @param modifier the way the new selection should be combined with the
   *   existing selection.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_INVALID_USER_BUFFER when `circle` is not readable.
   * @throws VO_INVALID_ARGUMENT when `circle` is invalid.
   * @throws VO_INVALID_ARGUMENT when `modifier` is invalid.
   */
  bool (VO_MSE_CALL *selectCircle)(voEngine engine, voEngineObject target, const voCircle* circle, voSelectionModifier modifier);

  /**
   * Selects all the items from the selection's source.
   *
   * @param engine the engine.
   * @param target the target selection.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   */
  bool (VO_MSE_CALL *selectAll)(voEngine engine, voEngineObject target);

  /**
   * Clears the selection.
   *
   * @param engine the engine.
   * @param target the target selection.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   */
  bool (VO_MSE_CALL *selectNone)(voEngine engine, voEngineObject target);

  /**
   * Combines the existing selection with another selection.
   *
   * @param engine the engine.
   * @param target the target selection.
   * @param other the other selection, must be of the same type as `target`.
   * @param modifier the way the new selection should be combined with the
   *   existing selection.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_NO_SUCH_OBJECT when the `other` reference is invalid.
   * @throws VO_INVALID_OBJECT when `other` does not have the same type as
   *   `target`.
   * @throws VO_INVALID_OBJECT when `target` and `other` are not associated
   *   with the same object (e.g. the same `voInk` or same `voPage`).
   * @throws VO_INVALID_ARGUMENT when `modifier` is invalid.
   */
  bool (VO_MSE_CALL *combine)(voEngine engine, voEngineObject target, voEngineObject other, voSelectionModifier modifier);

  /**
   * Tells whether the selection is empty.
   *
   * @param engine the engine.
   * @param target the target selection.
   *
   * @return `VO_YES` or `VO_NO` on success, otherwise `VO_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   */
  voYesNo (VO_MSE_CALL *isEmpty)(voEngine engine, voEngineObject target);
}
voISelection;

/**
 * Functions composing the `voIInkSelection` interface.
 *
 * {@implementingTypes voInkSelection, voPageSelection}
 */
typedef struct _voIInkSelection
{
  /**
   * Returns the ink on which this selection is based.
   *
   * @param engine the engine.
   * @param target the target selection.
   *
   * @return the ink on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_LIMIT_EXCEEDED when the number of `voInk` instances would exceed
   *   the allowed limit (please remember that the limit can be shared by
   *   several types).
   */
  voInk (VO_MSE_CALL *getInk)(voEngine engine, voEngineObject target);

  /**
   * Parses the given string as a selection and combines it into this selection.
   *
   * @param engine the engine.
   * @param target the target selection.
   * @param charset the charset used to encode parameter strings.
   * @param string the string to be parsed.
   *   `"[0,1]U]5:1.2,7:6.25[U[7:7,7:10$]"` is an example of valid string.
   * @param modifier the way the new selection should be combined with the
   *   existing selection.
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
   * @throws VO_INVALID_USER_BUFFER when `string` or its `bytes` member is not
   *   readable.
   * @throws VO_INVALID_ARGUMENT when `string` does not represent a valid union
   *   of intervals, e.g. if a `from` cursor is after the `to` cursor, or if
   *   some indices do not match the ink in `target`.
   * @throws VO_INVALID_ARGUMENT when `modifier` is invalid.
   */
  bool (VO_MSE_CALL *parse)(voEngine engine, voEngineObject target, voCharset charset, const voString* string, voSelectionModifier modifier);

  /**
   * Creates a new selection of the strokes and portion of strokes that are
   * contained in the specified interval and combines it with this selection.
   *
   * @param engine the engine.
   * @param target the target selection.
   * @param interval the interval from which to build the selection.
   * @param modifier the way the new selection should be combined with the
   *   existing selection.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_INVALID_USER_BUFFER when `interval` is not readable.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `interval` stroke or point indices do
   *   not match existing ink.
   * @throws VO_INVALID_ARGUMENT when `interval` does not match existing ink
   *   or if its `from` cursor is after its `to` cursor.
   * @throws VO_INVALID_ARGUMENT when `modifier` is invalid.
   */
  bool (VO_MSE_CALL *selectInterval)(voEngine engine, voEngineObject target, const voInkInterval* interval, voSelectionModifier modifier);

  /**
   * Creates a new selection of the strokes that are contained in a given tag
   * and combines it with this selection.
   *
   * @param engine the engine.
   * @param target the target selection.
   * @param tagId the identifier of the tag to be selected.
   * @param modifier the way the new selection should be combined with the
   *   existing selection.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_INVALID_ARGUMENT when `tagId` is invalid.
   * @throws VO_INVALID_ARGUMENT when `modifier` is invalid.
   */
  bool (VO_MSE_CALL *selectTag)(voEngine engine, voEngineObject target, int64_t tagId, voSelectionModifier modifier);

  /**
   * Creates a new selection of the input stroke and combines it with this
   * selection.
   *
   * @param engine the engine.
   * @param target the target selection.
   * @param stroke the input stroke.
   * @param modifier the way the new selection should be combined with the
   *   existing selection.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_NO_SUCH_OBJECT when the `stroke` reference is invalid.
   * @throws VO_INVALID_OBJECT when `stroke` is not a `voInkStroke`.
   * @throws VO_INVALID_ARGUMENT when `stroke` does not belong to the ink
   *   referenced by `target`.
   * @throws VO_INVALID_ARGUMENT when `modifier` is invalid.
   */
  bool (VO_MSE_CALL *selectStroke)(voEngine engine, voEngineObject target, voInkStroke stroke, voSelectionModifier modifier);

  /**
   * Combine this selection with the specified segment.
   *
   * @param engine the engine.
   * @param target the target selection.
   * @param intervals the ink intervals to be selected.
   * @param modifier the way the new selection should be combined with the
   *   existing selection.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_NO_SUCH_OBJECT when the `intervals` reference is invalid.
   * @throws VO_INVALID_OBJECT when `intervals` is not a `voInkIntervals`.
   * @throws VO_INVALID_ARGUMENT when `intervals` does not refer to this ink.
   * @throws VO_INVALID_ARGUMENT when `modifier` is invalid.
   */
  bool (VO_MSE_CALL *selectIntervals)(voEngine engine, voEngineObject target, voInkIntervals intervals, voSelectionModifier modifier);

  /**
   * Adjusts this selection to full strokes, only if all the selected portions
   * of each stroke exceed the specified length ratio (compared to the full
   * stroke length).
   *
   * @param engine the engine.
   * @param target the target selection.
   * @param lengthRatio the length ratio (between `0` and `1`) above which a
   *   stroke will be kept.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_INVALID_ARGUMENT when `lengthRatio` is not between `0` and `1`.
   */
  bool (VO_MSE_CALL *adjustToStrokeBoundaries)(voEngine engine, voEngineObject target, float lengthRatio);

  /**
   * Adjusts the selection to a given tag name.
   *
   * @param engine the engine.
   * @param target the target selection.
   * @param charset the charset used to encode parameter strings.
   * @param name the name of the tag to be selected.
   * @param overlapRatio the overlap ratio (between `0` and `1`) above which a
   *   tag will be kept.
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
   * @throws VO_INVALID_USER_BUFFER when `name` or its `bytes` member is not
   *   readable.
   * @throws VO_INVALID_ARGUMENT when `overlapRatio` is not between `0` and `1`.
   */
  bool (VO_MSE_CALL *adjustToTagBoundaries)(voEngine engine, voEngineObject target, voCharset charset, const voString* name, float overlapRatio);

  /**
   * Select the closest stroke within a circle.
   *
   * @param engine the engine.
   * @param target the target selection.
   * @param circle the circle to select from.
   * @param modifier the way the new selection should be combined with the
   *   existing selection.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` does not implement this
   *   interface.
   * @throws VO_INVALID_USER_BUFFER when `circle` is not readable.
   * @throws VO_INVALID_ARGUMENT when `circle` is invalid.
   * @throws VO_INVALID_ARGUMENT when `modifier` is invalid.
   */
  bool (VO_MSE_CALL *hitStroke)(voEngine engine, voEngineObject target, const voCircle* circle, voSelectionModifier modifier);

  /**
   * Selects the closest tag within a circle. The closest tag is the tag that
   * has the bigger overlap and is closest to the center if more than one tag
   * completely overlaps.
   *
   * @param engine the engine.
   * @param target the target selection.
   * @param charset the charset used to encode parameter strings.
   * @param name the name of the tag to be selected.
   * @param circle the circle to select from.
   * @param tagId a recipient that will receive the identifier of the tag if
   *   found, otherwise -1.
   * @param modifier the way the new selection should be combined with the
   *   existing selection.
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
   * @throws VO_INVALID_USER_BUFFER when `name` or its `bytes` member is not
   *   readable.
   * @throws VO_INVALID_USER_BUFFER when `circle` is not readable.
   * @throws VO_INVALID_ARGUMENT when `circle` is invalid.
   * @throws VO_INVALID_ARGUMENT when `modifier` is invalid.
   */
  bool (VO_MSE_CALL *hitTag)(voEngine engine, voEngineObject target, voCharset charset, const voString* name, const voCircle* circle, int64_t* tagId, voSelectionModifier modifier);
}
voIInkSelection;


/**
 * Lists the flag bits for the `flags` parameter of the
 * `voISelectionChangeListener::changed()` function.
 *
 * Those flags may be combined using the bitwise OR operator.
 */
typedef enum _voChangeFlag
{
  /**
   * Indicates that data changed, which means the intrinsic parameters of an
   * object changed.
   */
  VO_CF_DATA_CHANGED = 1,

  /**
   * Indicates that a transform has been applied on an object or part of that
   * object.
   */
  VO_CF_TRANSFORM_APPLIED = 2,
}
voChangeFlag;

/**
 * Functions composing the `voISelectionChangeListener` interface.
 *
 * @note This interface is implementable by user objects. User code is not
 *   allowed to invoke this interface directly: as a consequence
 *   `voGetInterface()` returns `NULL`.
 */
typedef struct _voISelectionChangeListener
{
  /**
   * Called when the target change listener is added to an ink object.
   *
   * @param engine the engine.
   * @param target the target change listener.
   * @param selection the selection to which the change listener was added.
   */
  void (VO_MSE_CALL *added)(voEngine engine, voUserObject target, voEngineObject selection);

  /**
   * Called when the target change listener is removed from an ink object.
   *
   * @param engine the engine.
   * @param target the target change listener.
   * @param selection the selection from which the change listener was removed.
   */
  void (VO_MSE_CALL *removed)(voEngine engine, voUserObject target, voEngineObject selection);

  /**
   * Called when the selection has changed.
   *
   * @param engine the engine.
   * @param target the target change listener.
   * @param selection the selection that changed.
   * @param flags an indication of what changed in the selection.
   */
  void (VO_MSE_CALL *changed)(voEngine engine, voUserObject target, voEngineObject selection, VO_FLAGS_T(voChangeFlag) flags);
}
voISelectionChangeListener;


/**
 * Functions composing the `voIGeometry` interface.
 */
typedef struct _voIGeometry
{
  /**
   * Sets the transform to the identity.
   *
   * @param engine the engine.
   * @param transform the target transform.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_INVALID_USER_BUFFER when `transform` is not writable.
   */
  bool (VO_MSE_CALL *setIdentity)(voEngine engine, voTransform* transform);

  /**
   * Multiplies the transform with a translation transformation. This is
   * equivalent to calling `multiply(T)`, where `T` is a `voTransform`
   * represented by the following matrix:
   *
   * <pre>
   *   [   1    0    tx  ]
   *   [   0    1    ty  ]
   *   [   0    0    1   ]
   * </pre>
   *
   * @param engine the engine.
   * @param transform the target transform.
   * @param tx the translation offset along the x axis.
   * @param ty the translation offset along the y axis.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_INVALID_USER_BUFFER when `transform` is not writable.
   * @throws VO_INVALID_ARGUMENT when `transform` contains invalid values or is
   *   not invertible.
   */
  bool (VO_MSE_CALL *translate)(voEngine engine, voTransform* transform, float tx, float ty);

  /**
   * Multiplies this transform with a scaling transformation. This is equivalent
   * to calling `multiply(S)`, where `S` is a `voTransform` represented by the
   * following matrix:
   *
   * <pre>
   *   [   sx   0    0   ]
   *   [   0    sy   0   ]
   *   [   0    0    1   ]
   * </pre>
   *
   * @param engine the engine.
   * @param transform the target transform.
   * @param sx the scaling factor along the x axis.
   * @param sy the scaling factor along the y axis.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_INVALID_USER_BUFFER when `transform` is not writable.
   * @throws VO_INVALID_ARGUMENT when `transform` contains invalid values or is
   *   not invertible.
   */
  bool (VO_MSE_CALL *scale)(voEngine engine, voTransform* transform, float sx, float sy);

  /**
   * Multiplies the transform with a rotation transformation. This is equivalent
   * to calling `multiply(R)`, where `R` is a `voTransform` represented by the
   * following matrix:
   *
   * <pre>
   *   [   cos(a)   -sin(a)    -cos(a) * x0 + sin(a) * y0 + x0   ]
   *   [   sin(a)    cos(a)    -sin(a) * x0 - cos(a) * y0 + y0   ]
   *   [   0           0                      1                  ]
   * </pre>
   *
   * @param engine the engine.
   * @param transform the target transform.
   * @param a the rotation angle in radians.
   * @param x0 the x position of the origin point.
   * @param y0 the y position of the origin point.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_INVALID_USER_BUFFER when `transform` is not writable.
   * @throws VO_INVALID_ARGUMENT when `transform` contains invalid values or is
   *   not invertible.
   */
  bool (VO_MSE_CALL *rotate)(voEngine engine, voTransform* transform, float a, float x0, float y0);

  /**
   * Multiplies the transform with an horizontal shearing transformation. This
   * is equivalent to calling `multiply(SHX), where `SHX` is a `voTransform`
   * represented by the following matrix:
   *
   * <pre>
   *   [    1     tan(a)   -y0 * tan(a) ]
   *   [    0       1           0       ]
   *   [    0       0           1       ]
   * </pre>
   *
   * @param engine the engine.
   * @param transform the target transform.
   * @param a the shearing angle in radians.
   * @param y0 the y position of the origin point.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_INVALID_USER_BUFFER when `transform` is not writable.
   * @throws VO_INVALID_ARGUMENT when `transform` contains invalid values or is
   *   not invertible.
   */
  bool (VO_MSE_CALL *shearX)(voEngine engine, voTransform* transform, float a, float y0);

  /**
   * Multiplies the transform with a vertical shearing transformation. This is
   * equivalent to calling `multiply(SHY)`, where `SHY` is a `voTransform`
   * represented by the following matrix:
   *
   * <pre>
   *   [    1       0           0       ]
   *   [   tan(a)   1      -x0 * tan(a) ]
   *   [    0       0           1       ]
   * </pre>
   *
   *
   * @param engine the engine.
   * @param transform the target transform.
   * @param a the shearing angle in radians.
   * @param x0 the x position of the origin point.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_INVALID_USER_BUFFER when `transform` is not writable.
   * @throws VO_INVALID_ARGUMENT when `transform` contains invalid values or is
   *   not invertible.
   */
  bool (VO_MSE_CALL *shearY)(voEngine engine, voTransform* transform, float a, float x0);

  /**
   * Multiplies this transform by the second transform.
   *
   * @param engine the engine.
   * @param transform1 the target transform.
   * @param transform2 the second transform.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_INVALID_USER_BUFFER when `transform1` is not writable.
   * @throws VO_INVALID_USER_BUFFER when `transform2` is not writable.
   * @throws VO_INVALID_ARGUMENT when `transform1` or `transform2` contain
   *   invalid values or are not invertible.
   */
  bool (VO_MSE_CALL *multiply)(voEngine engine, voTransform* transform1, const voTransform* transform2);

  /**
   * Inverts this transform.
   *
   * @param engine the engine.
   * @param transform the target transform.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_INVALID_USER_BUFFER when `transform` is not writable.
   * @throws VO_INVALID_ARGUMENT when `transform` contains invalid values or is
   *   not invertible.
   */
  bool (VO_MSE_CALL *invert)(voEngine engine, voTransform* transform);
}
voIGeometry;


/**
 * Functions composing the `voIInkTagIterator` interface.
 *
 * This interface exposes the public methods of the `voInkTagIterator` type.
 *
 * @note As the C language does not provide any mechanism that could be used to
 *   simulate inheritance, inherited members are simply copied from the base
 *   structure.
 *
 * {@extends voIIterator}
 * {@implementingTypes voInkTagIterator}
 */
typedef struct _voIInkTagIterator
{
  /** {@inheritDoc} */
  voYesNo (VO_MSE_CALL *isAtEnd)(voEngine engine, voIterator target);
  /** {@inheritDoc} */
  bool (VO_MSE_CALL *next)(voEngine engine, voIterator target);
  /** {@inheritDoc} */
  voYesNo (VO_MSE_CALL *equals)(voEngine engine, voIterator target, voIterator other);

  /**
   * Removes the tag at the current step of the iteration.
   *
   * @note This does not invalidate the iterator: several tags can be removed in
   *   the same iteration.
   *
   * @param engine the engine.
   * @param target the target iterator.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voInkTagIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   */
  bool (VO_MSE_CALL *remove)(voEngine engine, voInkTagIterator target);

  /**
   * Returns the identifier of the tag at the current step of the iteration.
   *
   * @param engine the engine.
   * @param target the target iterator.
   *
   * @return the tag id on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voInkTagIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   */
  int64_t (VO_MSE_CALL *getId)(voEngine engine, voInkTagIterator target);

  /**
   * Returns the name of the tag at the current step of the iteration.
   *
   * @param engine the engine.
   * @param target the target iterator.
   * @param charset the charset used to encode parameter strings.
   * @param name the recipient for the name.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voInkTagIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `name` or its `bytes` member is not
   *   writable.
   */
  bool (VO_MSE_CALL *getName)(voEngine engine, voInkTagIterator target, voCharset charset, voString* name);

  /**
   * Returns the ink segment associated with the tag at the current step of the
   * iteration.
   *
   * @param engine the engine.
   * @param target the target iterator.
   *
   * @return the ink segment on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voInkTagIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   * @throws VO_LIMIT_EXCEEDED when the number of `voInkSegment` instances would
   *   exceed the allowed limit (please remember that the limit can be shared by
   *   several types).
   */
  voInkSegment (VO_MSE_CALL *getSegment)(voEngine engine, voInkTagIterator target);

  /**
   * Returns the data associated with the tag at the current step of the
   * iteration.
   *
   * @param engine the engine.
   * @param target the target iterator.
   *
   * @return the data if any, otherwise `NULL`.
   *   When `NULL`, use `voGetError()` to figure out whether an error occurred.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voInkTagIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   * @throws VO_LIMIT_EXCEEDED when the number of instances of the specified
   *   type would exceed the allowed limit (please remember that the limit can
   *   be shared by several types).
   */
  voEngineObject (VO_MSE_CALL *getData)(voEngine engine, voInkTagIterator target);

  /**
   * Associates a `voEngineObject` with the tag at the current step of the
   * iteration.
   *
   * @note The former data is released.
   *
   * @param engine the engine.
   * @param target the target iterator.
   * @param data the data to be associated with the tag.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voInkTagIterator`.
   * @throws VO_NO_SUCH_ELEMENT when `target` is positioned at the end of the
   *   iteration.
   * @throws VO_NO_SUCH_OBJECT when the `data` reference is invalid.
   */
  bool (VO_MSE_CALL *setData)(voEngine engine, voInkTagIterator target, voEngineObject data);
}
voIInkTagIterator;

#endif // end of: #ifndef C_MYSCRIPTINK_H
