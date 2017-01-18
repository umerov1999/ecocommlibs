/**
 * @file MyScriptAnalyzer-documentToSVG.c
 *
 * This file contains our sample implementation of the serialization of an
 * analyzed document to a SVG string.
 *
 * This implementation is provided as-is, feel free to customize it to suit your
 * own needs.
 */


#include "MyScriptAnalyzer-documentToSVG.h"
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>


// -- configuration ------------------------------------------------------------

#define DOCUMENTTOSVG_INDENT_SPACES       2
#define DOCUMENTTOSVG_BUFFER_SIZE         255

// -- useful mathematical utilities --------------------------------------------

#include <math.h>

#ifndef M_2PI
#define M_2PI         6.283185307179586476925286766559    // 2*pi
#endif
#ifndef M_PI
#define M_PI          3.1415926535897932384626433832795   // pi
#endif

// returns the distance between 2 voAnalyzerPointData points
#define voAnalyzerPointData_getDistance(p1,p2)  ((float)sqrt(((p1).x - (p2).x) * ((p1).x - (p2).x) + ((p1).y - (p2).y) * ((p1).y - (p2).y)))

// returns the point of an ellipse as a voAnalyzerPointData at given angle
static void voAnalyzerPointData_getEllipsePoint(voAnalyzerPointData center, float minRadius, float maxRadius, float orientation, float angle, voAnalyzerPointData* point)
{
  double alpha = atan2(sin(angle)/minRadius, cos(angle)/maxRadius);
  point->x = (float)(center.x + maxRadius*cos(alpha)*cos(orientation) - minRadius*sin(alpha)*sin(orientation));
  point->y = (float)(center.y + maxRadius*cos(alpha)*sin(orientation) + minRadius*sin(alpha)*cos(orientation));
}

// -- floating-point type definitions ------------------------------------------

#include <float.h>

#ifndef DBL_MAX
#define DBL_MAX       1.7976931348623158e+308             // Largest magnitude of a double value
#endif


// -- output state structure ---------------------------------------------------

typedef struct _OutputState
{
  char* destination;  // destination buffer
  int capacity;       // destination buffer capacity
  int offset;         // where to write next in the destination buffer
  bool startOfLine;   // start of line flag
  int indent;         // indent level
} OutputState;


// -- forward declarations -----------------------------------------------------

static bool documentToSVG_(voEngine engine, voAnalyzerDocument document, OutputState* state, DocumentConfig* config);


// -- documentToSVG function ---------------------------------------------------

int documentToSVG(voEngine engine, voAnalyzerDocument document, char* utf8Buffer, int capacity, DocumentConfig* config)
{
  OutputState state = { utf8Buffer, capacity, 0, true, 0 };
  voAnalyzerDocument doc;
  bool successful;

  if (config == NULL)
    return false;

  // keep a user reference to the document while processing
  doc = voAcquireObject(engine, document);

  successful = documentToSVG_(engine, doc, &state, config);

  voReleaseObject(engine, doc);

  if (!successful)
    return -1;

  return state.offset;
}


// -- forward declarations -----------------------------------------------------

static bool computeInkBoundingBox(voEngine engine, voAnalyzerDocument document, OutputState* state, voAnalyzerPointData* topLeft, voAnalyzerPointData* bottomRight);
static bool computeDecorationLength(voEngine engine, voAnalyzerDocument document, OutputState* state, DocumentConfig* config, float* decorationLength);

static bool renderStart(OutputState* state, DocumentConfig* config, voAnalyzerPointData* topLeft, voAnalyzerPointData* bottomRight);
static bool renderInk(voEngine engine, voAnalyzerDocument document, OutputState* state, DocumentConfig* config);
static bool renderText(voEngine engine, voAnalyzerDocument document, OutputState* state, DocumentConfig* config);
static bool renderShapes(voEngine engine, voAnalyzerDocument document, OutputState* state, DocumentConfig* config, float decorationLength);
static bool renderTables(voEngine engine, voAnalyzerDocument document, OutputState* state);
static bool renderEnd(OutputState* state);


// -- documentToSVG_ function --------------------------------------------------

static bool documentToSVG_(voEngine engine, voAnalyzerDocument document, OutputState* state, DocumentConfig* config)
{
  bool successful = true;
  float decorationLength;
  voAnalyzerPointData topLeft, bottomRight;

  successful = successful &&
    computeInkBoundingBox(engine, document, state, &topLeft, &bottomRight);

  successful = successful &&
    computeDecorationLength(engine, document, state, config, &decorationLength);

  successful = successful &&
    renderStart(state, config, &topLeft, &bottomRight);

  successful = successful &&
    renderInk(engine, document, state, config);

  successful = successful &&
    renderText(engine, document, state, config);

  successful = successful &&
    renderShapes(engine, document, state, config, decorationLength);

  successful = successful &&
    renderTables(engine, document, state);

  successful = successful &&
    renderEnd(state);

  return successful;
}


// -- SVG output functions -----------------------------------------------------

static void svgAppend(OutputState* state, const char* input, int length)
{
  length = length == -1 ? (int)strlen(input) : length;

  // append only when destination is non NULL
  if (state->destination != NULL)
  {
    int remaining = state->capacity - state->offset;

    if (remaining > 0)
    {
      const char* inputEnd = input + length;
      char* output = state->destination + state->offset;
      char* outputEnd = output + remaining;

      while (output < outputEnd && input < inputEnd)
      {
        char c = *input++;

        if (c == '\n')
        {
          *output++ = c;
          state->startOfLine = true;
        }
        else
        {
          if (state->startOfLine)
          {
            int spaces = state->indent * DOCUMENTTOSVG_INDENT_SPACES;

            while (output < outputEnd && spaces-- > 0)
              *output++ = ' ';

            if (spaces)
            {
              state->offset += spaces;  // because we always inform client code
                                        // how much we would have written if
                                        // the destination buffer was large
                                        // enough
            }

            state->startOfLine = false;
          }

          *output++ = c;
        }
      }

      state->offset += (int)(output - (state->destination + state->offset));
    }
  }
  else
  {
    int spaces = state->indent * DOCUMENTTOSVG_INDENT_SPACES;

    while (length--)
    {
      char c = *input++;

      if (c == '\n')
      {
        ++state->offset;
        state->startOfLine = true;
      }
      else
      {
        if (state->startOfLine)
        {
          state->offset += spaces;
          state->startOfLine = false;
        }

        ++state->offset;
      }
    }
  }
}

static void svgStartElement(OutputState* state, const char* input, int length)
{
  assert(state);

  svgAppend(state, input, length);
  ++state->indent;
}

static void svgEndElement(OutputState* state, const char* input, int length)
{
  assert(state);

  --state->indent;
  svgAppend(state, input, length);
}

static bool svgStart(OutputState* state, DocumentConfig* config, voAnalyzerPointData* topLeft, voAnalyzerPointData* bottomRight)
{
  char buffer[DOCUMENTTOSVG_BUFFER_SIZE];
  int length = 0;

  svgStartElement(state, "<?xml version=\"1.0\" standalone=\"no\"?>\n", -1);
  svgAppend(state, "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n", -1);
  svgAppend(state, "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\"\n", -1);
  length = sprintf(buffer, "  stroke=\"%s\" fill=\"none\" stroke-width=\"%g\">\n\n",
    config->strokeColor, config->strokeWidth);
  svgAppend(state, buffer, length);

  // We compute a global scale and translation so that the output image fits
  // in the configured document size
  {
    float x, y, width, height;
    float scale, tx, ty;

    // Initialize input rectangle with bounding box
    x = topLeft->x;
    y = topLeft->y;
    width = bottomRight->x - x;
    height = bottomRight->y - y;

    // Apply margin, if any
    if (config->docMargin > 0)
    {
      float referenceSize = width > height ? width : height;
      float margin = referenceSize * config->docMargin;
      x -= margin;
      y -= margin;
      width += 2*margin;
      height += 2*margin;
    }

    // Initialize transform without scale
    scale = 1;
    tx = -x;
    ty = -y;

    // Adjust transform using the configured output box.
    if (config->docWidth > 0 && config->docHeight > 0)
    {
      float scale1 = config->docWidth / width;
      float scale2 = config->docHeight / height;
      if (scale1 < scale2)
      {
        scale = scale1;
        ty += (config->docHeight / scale - height) / 2;
      }
      else if (scale2 < scale1)
      {
        scale = scale2;
        tx += (config->docWidth / scale - width) / 2;
      }
      else
      {
        scale = scale1;
      }
    }
    else if (config->docWidth > 0)
    {
      scale = config->docWidth / width;
    }
    else if (config->docHeight > 0)
    {
      scale = config->docHeight / height;
    }

    // Apply transform
    length = sprintf(buffer, "<g transform=\"scale(%g) translate(%g,%g)\">\n", scale, tx, ty);
    svgStartElement(state, buffer, length);
  }

  return true;
}

static bool svgEnd(OutputState* state)
{
  svgEndElement(state, "</g>\n", -1);
  svgEndElement(state, "</svg>\n", -1);
  return true;
}

static bool svgText(OutputState* state,
                    DocumentConfig* config,
                    float left,
                    float baseline,
                    float width,
                    float textHeight,
                    char* label)
{
  char buffer[DOCUMENTTOSVG_BUFFER_SIZE];
  int length = 0;

  length = sprintf(buffer, "<text x=\"%g\" y=\"%g\" textLength=\"%g\" font-size=\"%g\" fill=\"%s\" stroke-width=\"1\">",
    left,
    baseline,
    width,
    textHeight * config->textHeightFactor,
    config->strokeColor);

  svgStartElement(state, buffer, length);

  svgAppend(state, label, -1);

  svgEndElement(state, "</text>\n", -1);

  return true;
}

static bool svgUnderlinedText(OutputState* state,
                              DocumentConfig* config,
                              float left,
                              float baseline,
                              float width,
                              float textHeight,
                              voAnalyzerUnderlineData* underlines,
                              int32_t underlineCount,
                              char* label)
{
  char buffer[DOCUMENTTOSVG_BUFFER_SIZE];
  int length = 0;
  int32_t i;
  int32_t charCount;
  size_t labelLength = strlen(label);

  length = sprintf(buffer, "<text x=\"%g\" y=\"%g\" textLength=\"%g\" font-size=\"%g\" fill=\"%s\" stroke-width=\"1\">\n",
    left,
    baseline,
    width,
    textHeight * config->textHeightFactor,
    config->strokeColor);
  svgStartElement(state, buffer, length);

  charCount = (underlineCount > 0) ? underlines[0].firstCharacter : (int32_t)labelLength;
  if (charCount > 0)
  {
    svgStartElement(state, "<tspan>", -1);
    svgAppend(state, label, charCount);
    svgEndElement(state, "</tspan>\n", -1);
  }

  // for each underlined sub-string and the following regular sub-string
  for (i = 0; i < underlineCount; ++i)
  {
    // ignore multi underlines, not supported by SVG text decorations
    if (i > 0 &&
      (underlines[i].firstCharacter == underlines[i - 1].firstCharacter) &&
      (underlines[i].lastCharacter == underlines[i - 1].lastCharacter))
      continue;

    // underlined sub-string
    svgStartElement(state, "<tspan text-decoration=\"underline\">", -1);
    svgAppend(state, &label[underlines[i].firstCharacter], underlines[i].lastCharacter - underlines[i].firstCharacter + 1);
    svgEndElement(state, "</tspan>\n", -1);

    // following regular sub-string
    charCount = (i < underlineCount-1) ? underlines[i+1].firstCharacter : (int32_t)labelLength;
    charCount -= underlines[i].lastCharacter + 1;
    if (charCount > 0)
    {
      svgStartElement(state, "<tspan>", -1);
      svgAppend(state, &label[underlines[i].lastCharacter + 1], charCount);
      svgEndElement(state, "</tspan>\n", -1);
    }
  }

  svgEndElement(state, "</text>\n", -1);

  return true;
}

static bool svgMarker(OutputState* state,
                      DocumentConfig* config,
                      int32_t markerIndex,
                      float decorationLength,
                      bool hasStartMarker,
                      float startMarkerAngle,
                      bool hasEndMarker,
                      float endMarkerAngle)
{
  char buffer[DOCUMENTTOSVG_BUFFER_SIZE];
  int length = 0;

  if (!hasStartMarker && !hasEndMarker)
    return false;

  svgStartElement(state, "<defs>\n", -1);

  if (hasStartMarker)
  {
    length = sprintf(buffer, "<marker id=\"arrowStart%d\"\n", markerIndex);
    svgStartElement(state, buffer, length);

    length = sprintf(buffer, "refX=\"%g\" refY=\"%g\"\n",
      decorationLength*config->arrowShapeFactor,
      decorationLength);
    svgAppend(state, buffer, length);

    length = sprintf(buffer, "orient=\"%g\"\n",
      startMarkerAngle);
    svgAppend(state, buffer, length);

    length = sprintf(buffer, "viewBox=\"0 0 %g %g\"\n",
      decorationLength*config->arrowShapeFactor,
      decorationLength*config->arrowShapeFactor);
    svgAppend(state, buffer, length);

    svgAppend(state, "markerUnits=\"userSpaceOnUse\"\n", -1);

    length = sprintf(buffer, "markerWidth=\"%g\" markerHeight=\"%g\"\n",
      decorationLength*config->arrowShapeFactor,
      decorationLength*config->arrowShapeFactor);
    svgAppend(state, buffer, length);

    svgAppend(state, "overflow=\"visible\">\n", -1);

    length = sprintf(buffer, "<path d=\"M 0,%g %g,%g 0,0 %g,%g 0,%g z\"\n",
      decorationLength*config->arrowShapeFactor,  // path point[0].y
      decorationLength*config->arrowShapeFactor,  // path point[1].x
      decorationLength,                           // path point[1].y
      decorationLength,                           // path point[2].x
      decorationLength,                           // path point[2].y
      decorationLength*config->arrowShapeFactor); // path point[3].y
    svgStartElement(state, buffer, length);

    length = sprintf(buffer, "transform=\"matrix(-1,0,0,-1,%g,%g)\"\n",
      decorationLength*config->arrowShapeFactor*2.0f,
      decorationLength*config->arrowShapeFactor);
    svgAppend(state, buffer, length);

    length = sprintf(buffer, "style=\"fill-rule:evenodd;stroke-width:1;fill:%s\"\n",
      config->strokeColor);
    svgAppend(state, buffer, length);

    svgEndElement(state, "/>\n", -1);

    svgEndElement(state, "</marker>\n", -1);
  }

  if (hasEndMarker)
  {
    length = sprintf(buffer, "<marker id=\"arrowEnd%d\"\n", markerIndex);
    svgStartElement(state, buffer, length);

    length = sprintf(buffer, "refX=\"%g\" refY=\"%g\"\n",
      decorationLength*config->arrowShapeFactor,
      decorationLength);
    svgAppend(state, buffer, length);

    length = sprintf(buffer, "orient=\"%g\"\n",
      endMarkerAngle);
    svgAppend(state, buffer, length);

    length = sprintf(buffer, "viewBox=\"0 0 %g %g\"\n",
      decorationLength*config->arrowShapeFactor,
      decorationLength*config->arrowShapeFactor);
    svgAppend(state, buffer, length);

    svgAppend(state, "markerUnits=\"userSpaceOnUse\"\n", -1);

    length = sprintf(buffer, "markerWidth=\"%g\" markerHeight=\"%g\"\n",
      decorationLength*config->arrowShapeFactor,
      decorationLength*config->arrowShapeFactor);
    svgAppend(state, buffer, length);

    svgAppend(state, "overflow=\"visible\">\n", -1);

    length = sprintf(buffer, "<path d=\"M 0,%g %g,%g 0,0 %g,%g 0,%g z\"\n",
      decorationLength*config->arrowShapeFactor,  // path point[0].y
      decorationLength*config->arrowShapeFactor,  // path point[1].x
      decorationLength,                           // path point[1].y
      decorationLength,                           // path point[2].x
      decorationLength,                           // path point[2].y
      decorationLength*config->arrowShapeFactor); // path point[3].y
    svgStartElement(state, buffer, length);

    svgAppend(state, "transform=\"matrix(1,0,0,1,0,0)\"\n", -1);

    length = sprintf(buffer, "style=\"fill-rule:evenodd;stroke-width:1;fill:%s\"\n",
      config->strokeColor);
    svgAppend(state, buffer, length);

    svgEndElement(state, "/>\n", -1);

    svgEndElement(state, "</marker>\n", -1);
  }

  svgEndElement(state, "</defs>\n", -1);

  return true;
}

static bool svgPath(voEngine engine,
                    voAnalyzerDocument document,
                    OutputState* state,
                    DocumentConfig* config,
                    voAnalyzerPointData* stroke,
                    int32_t pointCount,
                    int32_t markerIndex,
                    float decorationLength,
                    bool hasStartMarker,
                    float startMarkerAngle,
                    bool hasEndMarker,
                    float endMarkerAngle,
                    bool closePath)
{
  bool successful = true;
  char buffer[DOCUMENTTOSVG_BUFFER_SIZE];
  int length = 0;

  if (stroke == NULL || pointCount <= 0)
    return false;

  // define markers
  if (hasStartMarker || hasEndMarker)
  {
    successful = successful &&
      svgMarker(state,
                config,
                markerIndex,
                decorationLength,
                hasStartMarker,
                startMarkerAngle,
                hasEndMarker,
                endMarkerAngle);
  }

  if (successful)
  {
    // draw path
    length = sprintf(buffer, "<path d=\"M %g %g",
        stroke[0].x,
        stroke[0].y);
    svgStartElement(state, buffer, length);

    if (pointCount == 1)
    {
      // single point
      length = sprintf(buffer, " L %g %g",
          stroke[0].x + 1,
          stroke[0].y);
      svgAppend(state, buffer, length);
    }
    else
    {
      int32_t i = 0;
      for (i = 1; i < pointCount; ++i)
      {
        length = sprintf(buffer, " L %g %g",
            stroke[i].x,
            stroke[i].y);
        svgAppend(state, buffer, length);
      }
    }

    if (closePath)
    {
      svgAppend(state, " Z", -1);
    }

    svgAppend(state, "\"", -1);

    if (hasStartMarker)
    {
      length = sprintf(buffer, " marker-start=\"url(#arrowStart%d)\"",
          markerIndex);
      svgAppend(state, buffer, length);
    }

    if (hasEndMarker)
    {
      length = sprintf(buffer, " marker-end=\"url(#arrowEnd%d)\"",
          markerIndex);
      svgAppend(state, buffer, length);
    }

    svgEndElement(state, " />\n", -1);
  }

  return successful;
}

static bool svgStroke(voEngine engine,
                      voAnalyzerDocument document,
                      OutputState* state,
                      voShapeInkRange* ranges,
                      int32_t rangeCount)
{
  bool successful = true;
  int32_t strokeCount = 0;
  int32_t pointCount = 0;
  int32_t rangeIndex = 0;

  if (ranges == NULL || rangeCount <= 0)
    return false;

  strokeCount = VO_IFACE(engine, IAnalyzerDocument)->getStrokeCount(engine, document);
  if ((strokeCount == 0) || (strokeCount == -1))
    return false;

  for (rangeIndex = 0; successful && rangeIndex < rangeCount; ++rangeIndex)
  {
    voAnalyzerPointData* stroke = NULL;
  
    successful = successful &&
      (ranges[rangeIndex].stroke < strokeCount);

    successful = successful &&
      VO_IFACE(engine, IAnalyzerDocument)->getStrokeAt(engine, document, ranges[rangeIndex].stroke, NULL, 0, NULL, 0, &pointCount);

    successful = successful &&
      (pointCount > 0);

    if (successful)
      stroke = malloc(sizeof(*stroke) * pointCount);

    successful = successful &&
      (stroke != NULL);

    successful = successful &&
      VO_IFACE(engine, IAnalyzerDocument)->getStrokeAt(engine, document, ranges[rangeIndex].stroke, &stroke[0].x, sizeof(voAnalyzerPointData), &stroke[0].y, sizeof(voAnalyzerPointData), &pointCount);

    successful = successful &&
      svgPath(engine,
              document,
              state,
              NULL,
              stroke,
              pointCount,
              -1,
              0,
              false,
              0,
              false,
              0,
              false);

    free(stroke);
  }

  return successful;
}

static bool svgCircle(OutputState* state,
                      DocumentConfig* config,
                      voAnalyzerPointData centerPoint,
                      float r)
{
  char buffer[DOCUMENTTOSVG_BUFFER_SIZE];
  int length = 0;

  length = sprintf(buffer, "<circle cx=\"%g\" cy=\"%g\" r=\"%g\"",
      centerPoint.x,
      centerPoint.y,
      r);

  svgStartElement(state, buffer, length);

  svgEndElement(state, " />\n", -1);

  return true;
}

static bool svgEllipse(OutputState* state,
                       DocumentConfig* config,
                        voAnalyzerPointData centerPoint,
                        float rx,
                        float ry,
                        float orientation)
{
  char buffer[DOCUMENTTOSVG_BUFFER_SIZE];
  int length = 0;

  length = sprintf(buffer, "<ellipse cx=\"%g\" cy=\"%g\" rx=\"%g\" ry=\"%g\" transform=\"rotate(%g,%g,%g)\"",
      centerPoint.x,
      centerPoint.y,
      rx,
      ry,
      orientation,
      centerPoint.x,
      centerPoint.y);

  svgStartElement(state, buffer, length);

  svgEndElement(state, " />\n", -1);

  return true;
}

static bool svgArc(OutputState* state,
    voAnalyzerPointData startPoint,
    voAnalyzerPointData endPoint,
    float rx,
    float ry,
    float phi,
    int fA,
    int fS)
{
  char buffer[DOCUMENTTOSVG_BUFFER_SIZE];
  int length = 0;

  length = sprintf(buffer, "<path d=\"M %g %g A %g %g %g %d %d %g %g\"",
      startPoint.x,
      startPoint.y,
      rx,
      ry,
      phi,
      fA,
      fS,
      endPoint.x,
      endPoint.y);

  svgStartElement(state, buffer, length);

  svgEndElement(state, " />\n", -1);

  return true;
}

static bool svgDecoratedArc(OutputState* state,
                            DocumentConfig* config,
                            voAnalyzerPointData startPoint,
                            voAnalyzerPointData endPoint,
                            float rx,
                            float ry,
                            float phi,
                            int fA,
                            int fS,
                            int32_t markerIndex,
                            float decorationLength,
                            bool hasStartMarker,
                            float startMarkerAngle,
                            bool hasEndMarker,
                            float endMarkerAngle)
{
  bool successful = true;
  char buffer[DOCUMENTTOSVG_BUFFER_SIZE];
  int length = 0;

  // Define markers
  successful = successful &&
    svgMarker(state,
              config,
              markerIndex,
              decorationLength,
              hasStartMarker,
              startMarkerAngle,
              hasEndMarker,
              endMarkerAngle);

  // Draw the arc refering to the above markers
  length = sprintf(buffer, "<path d=\"M %g %g A %g %g %g %d %d %g %g\"",
      startPoint.x,
      startPoint.y,
      rx,
      ry,
      phi,
      fA,
      fS,
      endPoint.x,
      endPoint.y);

  svgStartElement(state, buffer, length);

  if (hasStartMarker)
  {
    length = sprintf(buffer, " marker-start=\"url(#arrowStart%d)\"",
        markerIndex);

    svgAppend(state, buffer, length);
  }

  if (hasEndMarker)
  {
    length = sprintf(buffer, " marker-end=\"url(#arrowEnd%d)\"",
        markerIndex);

    svgAppend(state, buffer, length);
  }

  svgEndElement(state, " />\n", -1);

  return successful;
}

static bool svgLine(voEngine engine,
    voAnalyzerDocument document,
    OutputState* state,
    voAnalyzerPointData startPoint,
    voAnalyzerPointData endPoint)
{
  voAnalyzerPointData stroke[2];
  stroke[0] = startPoint;
  stroke[1] = endPoint;
  return svgPath( engine,
                  document,
                  state,
                  NULL,
                  stroke,
                  2,
                  -1,
                  0,
                  false,
                  0,
                  false,
                  0,
                  false);
}

static bool svgDecoratedLine( voEngine engine,
                              voAnalyzerDocument document,
                              OutputState* state,
                              DocumentConfig* config,
                              voAnalyzerPointData startPoint,
                              voAnalyzerPointData endPoint,
                              int32_t markerIndex,
                              float decorationLength,
                              bool hasStartMarker,
                              float startMarkerAngle,
                              bool hasEndMarker,
                              float endMarkerAngle)
{
  voAnalyzerPointData stroke[2];
  stroke[0] = startPoint;
  stroke[1] = endPoint;
  return svgPath( engine,
                  document,
                  state,
                  config,
                  stroke,
                  2,
                  markerIndex,
                  decorationLength,
                  hasStartMarker,
                  startMarkerAngle,
                  hasEndMarker,
                  endMarkerAngle,
                  false);
}


// -- document to SVG elements parsing functions -------------------------------

static bool computeInkBoundingBox(voEngine engine, voAnalyzerDocument document, OutputState* state, voAnalyzerPointData* topLeft, voAnalyzerPointData* bottomRight)
{
  bool successful = true;
  int32_t strokeIndex = 0;
  int32_t strokeCount = VO_IFACE(engine, IAnalyzerDocument)->getStrokeCount(engine, document);
  float xmin, ymin, xmax, ymax;

  if ((strokeCount == 0) || (strokeCount == -1))
    return false;

  xmin = ymin = FLT_MAX;
  xmax = ymax = -FLT_MAX;

  for (strokeIndex = 0; successful && strokeIndex < strokeCount; ++strokeIndex)
  {
    voAnalyzerPointData* stroke = NULL;
    int32_t pointIndex = 0;
    int32_t pointCount = 0;
    successful = successful &&
      VO_IFACE(engine, IAnalyzerDocument)->getStrokeAt(engine, document, strokeIndex, NULL, 0, NULL, 0, &pointCount);
    successful = successful &&
      (pointCount > 0);
    if (successful)
      stroke = malloc(sizeof(*stroke) * pointCount);
    successful = successful &&
      (stroke != NULL);
    successful = successful &&
      VO_IFACE(engine, IAnalyzerDocument)->getStrokeAt(engine, document, strokeIndex, &stroke->x, sizeof(voAnalyzerPointData), &stroke->y, sizeof(voAnalyzerPointData), &pointCount);
    for (pointIndex = 0; successful && pointIndex < pointCount; ++pointIndex)
    {
      if (xmin > stroke[pointIndex].x)
        xmin = stroke[pointIndex].x;
      if (ymin > stroke[pointIndex].y)
        ymin = stroke[pointIndex].y;
      if (xmax < stroke[pointIndex].x)
        xmax = stroke[pointIndex].x;
      if (ymax < stroke[pointIndex].y)
        ymax = stroke[pointIndex].y;
    }
    free(stroke);
  }

  if (successful)
  {
    topLeft->x = xmin;
    topLeft->y = ymin;
    bottomRight->x = xmax;
    bottomRight->y = ymax;
  }

  return successful;
}

static bool computeDecorationLength(voEngine engine, voAnalyzerDocument document, OutputState* state, DocumentConfig* config, float* decorationLength)
{
  bool successful = true;
  int32_t shapeIndex = 0;
  float markerTotalLength = 0;
  float markerCount = 0;

  // Accumulate marker's length

  int32_t shapeCount = VO_IFACE(engine, IAnalyzerDocument)->getShapeCount(engine, document);
  if (shapeCount == -1)
    return false;

  for (shapeIndex = 0; successful && shapeIndex < shapeCount; ++shapeIndex)
  {
    voAnalyzerShape shape = NULL;
    voShapeCandidate shapeSegment = NULL, shapeCandidate = NULL;

    shape = VO_IFACE(engine, IAnalyzerDocument)->getShapeAt(engine, document, shapeIndex);

    successful = successful &&
      (shape != NULL);

    shapeSegment = VO_IFACE(engine, IAnalyzerShape)->getSegment(engine, shape);

    successful = successful &&
      (shapeSegment != NULL);

    shapeCandidate = VO_IFACE(engine, IShapeSegment)->getCandidateAt(engine, shapeSegment, 0);

    successful = successful &&
      (shapeCandidate != NULL);

    if (voGetType(engine, shapeCandidate) == VO_ShapeRecognized)
    {
      int32_t primitiveCount = 0;
      int32_t primitiveIndex = 0;

      primitiveCount = VO_IFACE(engine, IShapeRecognized)->getPrimitiveCount(engine, shapeCandidate);

      successful = successful &&
        (primitiveCount != -1);

      for (primitiveIndex = 0; successful && primitiveIndex < primitiveCount; ++primitiveIndex)
      {
        voShapePrimitive primitive = NULL;

        primitive = VO_IFACE(engine, IShapeRecognized)->getPrimitiveAt(engine, shapeCandidate, primitiveIndex);

        successful = successful &&
          (primitive != NULL);

        switch (voGetType(engine, primitive))
        {
          case VO_ShapeDecoratedLine:
            {
              voShapeDecoratedLineData data;

              successful = successful &&
                (VO_IFACE(engine, IShapeDecoratedLine)->getData(engine, primitive, &data));

              if (successful)
              {
                float markerLength = voAnalyzerPointData_getDistance(data.line.p1, data.line.p2);
                markerCount += 1;
                markerTotalLength += markerLength;
              }
            }
            break;
          case VO_ShapeDecoratedEllipticArc:
            {
              voShapeDecoratedEllipticArcData data;
              voAnalyzerPointData center;

              successful = successful &&
                (VO_IFACE(engine, IShapeDecoratedEllipticArc)->getData(engine, primitive, &data));

              if (successful)
              {
                float markerLength;
                voAnalyzerPointData startPoint, endPoint;

                center.x = data.arc.center.x;
                center.y = data.arc.center.y;
                voAnalyzerPointData_getEllipsePoint(center, data.arc.minRadius, data.arc.maxRadius, data.arc.orientation, data.arc.startAngle, &startPoint);
                voAnalyzerPointData_getEllipsePoint(center, data.arc.minRadius, data.arc.maxRadius, data.arc.orientation, data.arc.startAngle + data.arc.sweepAngle, &endPoint);
                markerLength = voAnalyzerPointData_getDistance(startPoint, endPoint);
                markerCount += 1;
                markerTotalLength += markerLength;
              }
            }
            break;
        }

        voReleaseObject(engine, primitive);
      }
    }

    voReleaseObject(engine, shapeCandidate);
    voReleaseObject(engine, shapeSegment);
    voReleaseObject(engine, shape);
  }

  // Compute the desired decoration length

  if (markerCount > 0)
  {
    // Average length of all markers ponderated by configured factor
    *decorationLength = markerTotalLength / markerCount * config->arrowSizeFactor;
  }
  else
  {
    // We could leave this uninitialized because we'll never use it anyway.
    *decorationLength = 0;
  }
  return successful;
}

static bool renderStart(OutputState* state, DocumentConfig* config, voAnalyzerPointData* topLeft, voAnalyzerPointData* bottomRight)
{
  return svgStart(state, config, topLeft, bottomRight);
}

static bool renderEnd(OutputState* state)
{
  return svgEnd(state);
}

static bool renderInk(voEngine engine, voAnalyzerDocument document, OutputState* state, DocumentConfig* config)
{
  bool successful = true;
  char buffer[DOCUMENTTOSVG_BUFFER_SIZE];
  int length = 0;
  int32_t strokeCount = 0;
  int32_t strokeIndex = 0;

  length = sprintf(buffer, "<g stroke=\"%s\" display=\"%s\"> <!-- original ink strokes -->\n",
    config->originalInkColor, config->displayOriginalInk ? "inherit" : "none");

  svgStartElement(state, buffer, length);

  strokeCount = VO_IFACE(engine, IAnalyzerDocument)->getStrokeCount(engine, document);
  if ((strokeCount == 0) || (strokeCount == -1))
    return false;

  for (strokeIndex = 0; successful && strokeIndex < strokeCount; ++strokeIndex)
  {
    int32_t pointCount = 0;
    voAnalyzerPointData* stroke = NULL;

    successful = successful &&
      VO_IFACE(engine, IAnalyzerDocument)->getStrokeAt(engine, document, strokeIndex, NULL, 0, NULL, 0, &pointCount);

    successful = successful &&
      (pointCount > 0);

    if (successful)
      stroke = malloc(sizeof(*stroke) * pointCount);

    successful = successful &&
      (stroke != NULL);

    successful = successful &&
      VO_IFACE(engine, IAnalyzerDocument)->getStrokeAt(engine, document, strokeIndex, &stroke[0].x, sizeof(voAnalyzerPointData), &stroke[0].y, sizeof(voAnalyzerPointData), &pointCount);

    successful = successful &&
      svgPath(engine,
              document,
              state,
              NULL,
              stroke,
              pointCount,
              -1,
              0,
              false,
              0,
              false,
              0,
              false);

    free(stroke);
  }

  svgEndElement(state, "</g>\n", -1);

  return successful;
}

static bool renderText(voEngine engine, voAnalyzerDocument document, OutputState* state, DocumentConfig* config)
{
  int32_t textLineIndex;
  bool successful = true;

  int32_t textLineCount = VO_IFACE(engine, IAnalyzerDocument)->getTextLineCount(engine, document);
  if (textLineCount == -1)
    return false;

  for (textLineIndex = 0; successful && textLineIndex < textLineCount; ++textLineIndex)
  {
    voAnalyzerTextLineData textLineData;
    voAnalyzerTextLine textLine = NULL;
    int32_t underlineCount;
    int32_t underlineIndex;
    voAnalyzerUnderlineData* underlines = NULL;

    textLine = VO_IFACE(engine, IAnalyzerDocument)->getTextLineAt(engine, document, textLineIndex);
    successful = successful &&
      (textLine != NULL);

    underlineCount = VO_IFACE(engine, IAnalyzerTextLine)->getUnderlineCount(engine, textLine);
    successful = successful &&
      (underlineCount != -1);

    if (successful)
    {
      voRecognitionResult result;
      voCandidateIterator iterator;

      successful = successful &&
        VO_IFACE(engine, IAnalyzerTextLine)->getData(engine, textLine, &textLineData);

      result = VO_IFACE(engine, IAnalyzerTextLine)->getResult(engine, textLine);
      successful = successful &&
        (result != NULL);

      iterator = VO_IFACE(engine, IRecognitionResult)->getCandidates(engine, result);
      successful = successful &&
        (iterator != NULL);

      if (!VO_IFACE(engine, IIterator)->isAtEnd(engine, iterator))
      {
        voString label = {NULL, 0};
        successful = successful &&
          VO_IFACE(engine, ICandidateIterator)->getLabel(engine, iterator, NULL, &label);
        label.bytes = malloc(label.byteCount + 1);
        successful = successful &&
          label.bytes != NULL;
        successful = successful &&
          VO_IFACE(engine, ICandidateIterator)->getLabel(engine, iterator, NULL, &label);
        if (successful)
          label.bytes[label.byteCount] = '\0';

        if (underlineCount == 0)
        {
          successful = successful &&
            svgText(state,
                    config,
                    textLineData.topLeftPoint.x,
                    textLineData.baselinePos,
                    textLineData.width,
                    textLineData.textHeight,
                    label.bytes);
        }
        else
        {
          underlines = malloc(sizeof(*underlines) * underlineCount);
          successful = successful && underlines != NULL;

          for (underlineIndex = 0; successful && underlineIndex < underlineCount; ++underlineIndex)
          {
            voAnalyzerUnderlineData data;
            voAnalyzerUnderline underline = VO_IFACE(engine, IAnalyzerTextLine)->getUnderlineAt(engine, textLine, underlineIndex);
            successful = successful &&
              (underline != NULL);

            successful = successful &&
              VO_IFACE(engine, IAnalyzerUnderline)->getData(engine, underline, &data);

            if (successful)
              underlines[underlineIndex] = data;

            voReleaseObject(engine, underline);
          }

          successful = successful &&
            svgUnderlinedText(state,
                              config,
                              textLineData.topLeftPoint.x,
                              textLineData.baselinePos,
                              textLineData.width,
                              textLineData.textHeight,
                              underlines,
                              underlineCount,
                              label.bytes);
          free(underlines);
        }
        free(label.bytes);
      }

      voReleaseObject(engine, iterator);
      voReleaseObject(engine, result);
    }

    voReleaseObject(engine, textLine);
  }

  return successful;
}

static bool renderShapes(voEngine engine, voAnalyzerDocument document, OutputState* state, DocumentConfig* config, float decorationLength)
{
  bool successful = true;
  int32_t shapeIndex = 0;

  int32_t shapeCount = VO_IFACE(engine, IAnalyzerDocument)->getShapeCount(engine, document);
  if (shapeCount == -1)
    return false;

  for (shapeIndex = 0; successful && shapeIndex < shapeCount; ++shapeIndex)
  {
    voAnalyzerShape shape = NULL;
    voShapeCandidate shapeSegment = NULL, shapeCandidate = NULL;

    shape = VO_IFACE(engine, IAnalyzerDocument)->getShapeAt(engine, document, shapeIndex);

    successful = successful &&
      (shape != NULL);

    shapeSegment = VO_IFACE(engine, IAnalyzerShape)->getSegment(engine, shape);

    successful = successful &&
      (shapeSegment != NULL);

    shapeCandidate = VO_IFACE(engine, IShapeSegment)->getCandidateAt(engine, shapeSegment, 0);

    successful = successful &&
      (shapeCandidate != NULL);

    if (voGetType(engine, shapeCandidate) == VO_ShapeRejected)
    {
      int32_t rangeCount = 0;
      int32_t rangeIndex = 0;
      voShapeInkRange* ranges = NULL;

      // It's a rejected shape, which means it looks like an arbitrary drawing.
      // Therefore, we just draw the corresponding digital ink.

      rangeCount = VO_IFACE(engine, IShapeSegment)->getInkRangeCount(engine, shapeSegment);

      successful = successful &&
        (rangeCount != -1);

      if (successful && rangeCount > 0)
      {
        ranges = malloc(sizeof(*ranges) * rangeCount);

        successful = successful &&
          (ranges != NULL);

        for (rangeIndex = 0; successful && rangeIndex < rangeCount; ++rangeIndex)
        {
          voShapeInkRange range;

          successful = successful &&
            (VO_IFACE(engine, IShapeSegment)->getInkRangeAt(engine, shapeSegment, rangeIndex, &range));

          if (successful)
            ranges[rangeIndex] = range;
        }

        successful = successful &&
          svgStroke(engine,
                    document,
                    state,
                    ranges,
                    rangeCount);

        free(ranges);
      }
    }
    else if (voGetType(engine, shapeCandidate) == VO_ShapeRecognized)
    {
      int32_t primitiveCount = 0;
      int32_t primitiveIndex = 0;

      // It's a recognized shape, draw its geometric primitives.

      primitiveCount = VO_IFACE(engine, IShapeRecognized)->getPrimitiveCount(engine, shapeCandidate);

      successful = successful &&
        (primitiveCount != -1);

      for (primitiveIndex = 0; successful && primitiveIndex < primitiveCount; ++primitiveIndex)
      {
        // The primitive
        voShapePrimitive primitive = VO_IFACE(engine, IShapeRecognized)->getPrimitiveAt(engine, shapeCandidate, primitiveIndex);
        // An arbitrary unique id of the primitive for use in markers
        int primitiveId = shapeIndex * shapeCount + primitiveIndex;

        successful = successful &&
          (primitive != NULL);

        switch (voGetType(engine, primitive))
        {
          case VO_ShapeLine:
            {
              voShapeLineData data;
              voAnalyzerPointData p1, p2;

              successful = successful &&
                (VO_IFACE(engine, IShapeLine)->getData(engine, primitive, &data));

              if (successful)
              {
                p1.x = data.p1.x;
                p1.y = data.p1.y;
                p2.x = data.p2.x;
                p2.y = data.p2.y;
              }

              successful = successful &&
                svgLine(engine,
                        document,
                        state,
                        p1,
                        p2);
            }
            break;

          case VO_ShapeDecoratedLine:
            {
              voShapeDecoratedLineData data;
              voAnalyzerPointData p1, p2;
              float tangentAngleP1, tangentAngleP2;

              successful = successful &&
                (VO_IFACE(engine, IShapeDecoratedLine)->getData(engine, primitive, &data));

              if (successful)
              {
                p1.x = data.line.p1.x;
                p1.y = data.line.p1.y;
                p2.x = data.line.p2.x;
                p2.y = data.line.p2.y;
              }

              successful = successful &&
                (VO_IFACE(engine, IShapeDecoratedLine)->getP1TangentAngle(engine, primitive, &tangentAngleP1));

              successful = successful &&
                (VO_IFACE(engine, IShapeDecoratedLine)->getP2TangentAngle(engine, primitive, &tangentAngleP2));

              successful = successful &&
                svgDecoratedLine(engine,
                                 document,
                                 state,
                                 config,
                                 p1,
                                 p2,
                                 primitiveId,
                                 decorationLength,
                                 data.p1Decoration == VO_SHAPE_DECORATION_ARROW_HEAD,
                                 tangentAngleP1*180.0f/(float)M_PI,
                                 data.p2Decoration == VO_SHAPE_DECORATION_ARROW_HEAD,
                                 tangentAngleP2*180.0f/(float)M_PI);
            }
            break;

          case VO_ShapeEllipticArc:
            {
              voShapeEllipticArcData data;
              voAnalyzerPointData center;

               successful = successful &&
                (VO_IFACE(engine, IShapeEllipticArc)->getData(engine, primitive, &data));

              if (successful)
              {
                center.x = data.center.x;
                center.y = data.center.y;

                if (fabs(data.sweepAngle) >= M_2PI)
                {
                  if (data.minRadius == data.maxRadius)
                  {
                    successful = successful &&
                      svgCircle(state,
                                config,
                                center,
                                data.minRadius);
                  }
                  else
                  {
                    successful = successful &&
                      svgEllipse(state,
                                 config,
                                 center,
                                 data.maxRadius,
                                 data.minRadius,
                                 data.orientation*180.0f / (float)M_PI);
                  }
                }
                else
                {
                  voAnalyzerPointData startPoint, endPoint;

                  if (successful)
                  {
                    voAnalyzerPointData_getEllipsePoint(center, data.minRadius, data.maxRadius, data.orientation, data.startAngle, &startPoint);
                    voAnalyzerPointData_getEllipsePoint(center, data.minRadius, data.maxRadius, data.orientation, data.startAngle + data.sweepAngle, &endPoint);
                  }

                  successful = successful &&
                    svgArc(state,
                           startPoint,
                           endPoint,
                           data.maxRadius,
                           data.minRadius,
                           data.orientation*180.0f / (float)M_PI,
                           fabs(data.sweepAngle) > (float)M_PI,
                           data.sweepAngle >= 0);
                }
              }
            }
            break;

          case VO_ShapeDecoratedEllipticArc:
            {
              voShapeDecoratedEllipticArcData data;
              voAnalyzerPointData center;

               successful = successful &&
                (VO_IFACE(engine, IShapeDecoratedEllipticArc)->getData(engine, primitive, &data));

              if (successful)
              {
                center.x = data.arc.center.x;
                center.y = data.arc.center.y;

                if (fabs(data.arc.sweepAngle) >= M_2PI)
                {
                  if (data.arc.minRadius == data.arc.maxRadius)
                  {
                    successful = successful &&
                      svgCircle(state,
                                config,
                                center,
                                data.arc.minRadius);
                  }
                  else
                  {
                    successful = successful &&
                      svgEllipse(state,
                                 config,
                                 center,
                                 data.arc.maxRadius,
                                 data.arc.minRadius,
                                 data.arc.orientation*180.0f / (float)M_PI);
                  }
                }
                else
                {
                  voAnalyzerPointData startPoint, endPoint;
                  float firstTangentAngle, lastTangentAngle;

                  if (successful)
                  {
                    voAnalyzerPointData_getEllipsePoint(center, data.arc.minRadius, data.arc.maxRadius, data.arc.orientation, data.arc.startAngle, &startPoint);
                    voAnalyzerPointData_getEllipsePoint(center, data.arc.minRadius, data.arc.maxRadius, data.arc.orientation, data.arc.startAngle + data.arc.sweepAngle, &endPoint);
                  }

                  successful = successful &&
                    (VO_IFACE(engine, IShapeDecoratedEllipticArc)->getFirstTangentAngle(engine, primitive, &firstTangentAngle));

                  successful = successful &&
                    (VO_IFACE(engine, IShapeDecoratedEllipticArc)->getLastTangentAngle(engine, primitive, &lastTangentAngle));

                  successful = successful &&
                    svgDecoratedArc(state,
                                    config,
                                    startPoint,
                                    endPoint,
                                    data.arc.maxRadius,
                                    data.arc.minRadius,
                                    data.arc.orientation*180.0f / (float)M_PI,
                                    fabs(data.arc.sweepAngle) > (float)M_PI,
                                    data.arc.sweepAngle >= 0,
                                    primitiveId,
                                    decorationLength,
                                    data.firstDecoration == VO_SHAPE_DECORATION_ARROW_HEAD,
                                    firstTangentAngle*180.0f/(float)M_PI,
                                    data.lastDecoration == VO_SHAPE_DECORATION_ARROW_HEAD,
                                    lastTangentAngle*180.0f/(float)M_PI);
                }
              }
            }
            break;

          default:
            break;
        }

        voReleaseObject(engine, primitive);
      }
    }

    voReleaseObject(engine, shapeCandidate);
    voReleaseObject(engine, shapeSegment);
    voReleaseObject(engine, shape);
  }

  return successful;
}

static bool renderTables(voEngine engine,
                       voAnalyzerDocument document,
                       OutputState* state)
{
  bool successful = true;
  int32_t tableIndex = 0;

  int32_t tableCount = VO_IFACE(engine, IAnalyzerDocument)->getTableCount(engine, document);
  if (tableCount == -1)
    return false;

  for (tableIndex = 0; successful && tableIndex < tableCount; ++tableIndex)
  {
    int32_t lineCount;
    int32_t lineIndex;

    voAnalyzerTable table = VO_IFACE(engine, IAnalyzerDocument)->getTableAt(engine, document, tableIndex);

    successful = successful &&
      (table != NULL);

    lineCount = VO_IFACE(engine, IAnalyzerTable)->getLineCount(engine, table);

    successful = successful &&
      (lineCount != -1);

    for (lineIndex = 0; successful && lineIndex < lineCount; ++lineIndex)
    {
      voAnalyzerLineData data;
      voAnalyzerLine line = VO_IFACE(engine, IAnalyzerTable)->getLineAt(engine, table, lineIndex);

      successful = successful &&
        (line != NULL);

      successful = successful &&
        VO_IFACE(engine, IAnalyzerLine)->getData(engine, line, &data);

      successful = successful &&
        svgLine(engine, document, state, data.p1, data.p2);

      voReleaseObject(engine, line);
    }

    voReleaseObject(engine, table);
  }

  return successful;
}
