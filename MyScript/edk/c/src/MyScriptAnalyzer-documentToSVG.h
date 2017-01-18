#ifndef EDK_C_SRC_MYSCRIPTANALYZER_DOCUMENTTOSVG_H
/**
 * @file MyScriptAnalyzer-documentToSVG.h
 *
 */


#include <c/MyScriptEngine.h>
#include <c/MyScriptAnalyzer.h>


#ifdef __cplusplus
extern "C" {
#endif

typedef struct _DocumentConfig
{
  /*
   * Document width, used to resize generated SVG image.
   * If set to 0, does not apply.
   */
  float docWidth;

  /*
   * Document height, used to resize generated SVG image.
   * If set to 0, does not apply.
   */
  float docHeight;

  /*
   * Document margin as a ratio of the input bounding box, used when resizing
   * generated SVG image.
   * If set to 0, does not apply.
   */
  float docMargin;

  /*
   * Stroke color for SVG elements, including SVG text and markers.
   */
  const char* strokeColor;

  /*
   * Stroke width for SVG elements.
   */
  float strokeWidth;

  /*
   * Text height factor, can be used to adjust text height to the writer's style
   * of handwriting.
   */
  float textHeightFactor;

  /*
   * Set this in order to display the original ink strokes, can be used for
   * debugging purpose.
   */
  bool displayOriginalInk;

  /*
   * Stroke color for original ink strokes, can be used for debugging purpose.
   */
  const char* originalInkColor;

  /*
   * Arrow shape factor, used to adjust the decoration marker shape.
   */
  float arrowShapeFactor;

  /*
   * Arrow size factor, used to adjust decoration marker sizes.
   */
  float arrowSizeFactor;

} DocumentConfig;

/**
 * Serializes the Analyzer's associated document to a SVG string.
 *
 * Usage:
 *  - first call int length = documentToSVG(engine, document, NULL, 0);
 *    to retrieve the length of the serialized SVG string.
 *  - allocate the destination buffer with str = malloc(length + 1);
 *  - call documentToSVG(engine, document, str, length) to retrieve the
 *    serialized SVG string.
 *  - str[length] = '\0';
 *
 * @param engine Reference to an <code>Engine</code> instance.
 * @param root Reference to the <code>AnalyzerDocument</code> instance.
 * @param utf8Buffer Destination UTF-8 buffer.
 * @param capacity Destination buffer capacity.
 *
 * @return The length of the serialized SVG string on success (without a
 *   terminal NUL character), otherwise <code>-1</code>.
 */
int documentToSVG(voEngine engine, voAnalyzerDocument document, char* utf8Buffer, int capacity, DocumentConfig* config);

#ifdef __cplusplus
}
#endif

#endif // endof: #ifndef EDK_C_SRC_MYSCRIPTANALYZER_DOCUMENTTOSVG_H
