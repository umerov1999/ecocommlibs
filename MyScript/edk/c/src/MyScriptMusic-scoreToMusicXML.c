/**
 * @file MyScriptMusic-scoreToMusicXML.c
 *
 * This file contains our sample implementation of the serialization of a
 * recognized score to a MusciXML string.
 *
 * This implementation is provided as-is, feel free to customize it to suit your
 * own needs.
 */

#include "MyScriptMusic-scoreToMusicXML.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define SCORETOMUSICXML_BUFFER_SIZE 1024
#define SCORETOMUSICXML_ERROR(message)\
  fprintf(stderr, "scoreToMusicXML: error: %s\n", message);\
  fflush(stderr)

#define SCORETOMUSICXML_INDENT_SPACES           2
#define SCORETOMUSICXML_DIVISIONS               480
#define SCORETOMUSICXML_SLUR_BUFFER_SIZE        16
#define SCORETOMUSICXML_DECORATION_BUFFER_SIZE  16

typedef struct _OutputState
{
  char* destination;      // destination buffer
  int capacity;           // destination buffer capacity
  int offset;             // where to write next in the destination buffer

  bool startOfLine;       // start of line flag
  int indent;             // indent level

  bool measure;           // <measure> element has started
  int measureNumber;      // current measure number
  bool attributes;        // <attributes> element has started

  voMusicKeySignature keySignature;
  voMusicTimeSignature timeSignature;
  voMusicClef clef;

  voMusicChord chord;
  voMusicSlur slurs[SCORETOMUSICXML_SLUR_BUFFER_SIZE];
  uint32_t slurCount;

} OutputState;

// forward declarations
static bool scoreToMusicXML_(voEngine engine, voMusicScore score, OutputState* state);

int scoreToMusicXML(voEngine engine, voMusicScore score, char* utf8Buffer, int capacity)
{
  OutputState state = { utf8Buffer, capacity };
  bool successful;

  assert(voGetType(engine, score) == VO_MusicScore);

  // keep a user reference to the score while processing
  score = voAcquireObject(engine, score);

  successful = scoreToMusicXML_(engine, score, &state);
  voReleaseObject(engine, score);

  if (!successful)
    return -1;

  return state.offset;
}

static void append(OutputState* state, const char* input, int length)
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
            int spaces = state->indent * SCORETOMUSICXML_INDENT_SPACES;

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
    int spaces = state->indent * SCORETOMUSICXML_INDENT_SPACES;

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

static void startElement(OutputState* state, const char* input, int length)
{
  assert(state);

  append(state, input, length);
  ++state->indent;
}

static void endElement(OutputState* state, const char* input, int length)
{
  assert(state);

  --state->indent;
  append(state, input, length);
}

// forward declarations
static bool partToMusicXML(voEngine engine, voMusicPart part, OutputState* state);
static bool barToMusicXML(voEngine engine, voMusicBar bar, OutputState* state);
static bool clefToMusicXML(voEngine engine, voMusicClef clef, OutputState* state);
static bool keySignatureToMusicXML(voEngine engine, voMusicKeySignature signature, OutputState* state);
static bool timeSignatureToMusicXML(voEngine engine, voMusicTimeSignature signature, OutputState* state);
static bool noteToMusicXML(voEngine engine, voMusicNote note, bool chordTag, OutputState* state);
static bool restToMusicXML(voEngine engine, voMusicRest rest, OutputState* state);
static bool chordToMusicXML(voEngine engine, voMusicChord chord, OutputState* state);

static bool scoreToMusicXML_(voEngine engine, voMusicScore score, OutputState* state)
{
  char buffer[SCORETOMUSICXML_BUFFER_SIZE];
  int length;
  int i, count;
  bool successful;

  assert(voGetType(engine, score) == VO_MusicScore);

  append(state, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n", -1);
  append(state, "<!DOCTYPE score-partwise PUBLIC \"-//Recordare//DTD MusicXML 1.0 Partwise//EN\"\n", -1);
  append(state, "                                \"http://www.musicxml.org/dtds/partwise.dtd\">\n", -1);

  startElement(state, "<score-partwise version=\"3.0\">\n", -1);

  count = VO_IFACE(engine, IMusicScore)->getPartCount(engine, score);
  if (count == -1)
    return false;

  startElement(state, "<part-list>\n", -1);
  for (i = 0; i < count; ++i)
  {
    length = sprintf(buffer, "<score-part id=\"P%d\">\n", i + 1);
    startElement(state, buffer, length);

    append(state, "<part-name></part-name>\n", -1);

    endElement(state, "</score-part>\n", -1);
  }
  endElement(state, "</part-list>\n", -1);

  for (i = 0; i < count; ++i)
  {
    voMusicPart part;

    length = sprintf(buffer, "<part id=\"P%d\">\n", i + 1);
    startElement(state, buffer, length);

    part = VO_IFACE(engine, IMusicScore)->getPartAt(engine, score, i);
    if (part == NULL)
      return false;

    successful = partToMusicXML(engine, part, state);
    voReleaseObject(engine, part);

    if (!successful)
      return false;

    endElement(state, "</part>\n", -1);
  }

  endElement(state, "</score-partwise>\n", -1);

  return true;
}

static bool attributesToMusicXML(voEngine engine, OutputState* state)
{
  char buffer[SCORETOMUSICXML_BUFFER_SIZE];
  int length;
  bool successful = false;

  for (;;)
  {
    startElement(state, "<attributes>\n", -1);

    length = sprintf(buffer, "<divisions>%d</divisions>\n", SCORETOMUSICXML_DIVISIONS);
    append(state, buffer, length);

    if (state->keySignature != NULL)
    {
      if (!keySignatureToMusicXML(engine, state->keySignature, state))
        break;
    }

    if (state->timeSignature != NULL)
    {
      if (!timeSignatureToMusicXML(engine, state->timeSignature, state))
        break;
    }

    if (state->clef != NULL)
    {
      if (!clefToMusicXML(engine, state->clef, state))
        break;
    }

    endElement(state, "</attributes>\n", -1);

    successful = true;
    break;
  }

  if (state->keySignature != NULL)
    voReleaseObject(engine, state->keySignature);

  if (state->timeSignature != NULL)
    voReleaseObject(engine, state->timeSignature);

  if (state->clef != NULL)
    voReleaseObject(engine, state->clef);

  state->keySignature = NULL;
  state->timeSignature = NULL;
  state->clef = NULL;

  state->attributes = false;

  return successful;
}

static bool partToMusicXML(voEngine engine, voMusicPart part, OutputState* state)
{
  char buffer[SCORETOMUSICXML_BUFFER_SIZE];
  int length;
  int i, count;
  bool successful = true;

  assert(voGetType(engine, part) == VO_MusicPart);

  count = VO_IFACE(engine, IMusicPart)->getElementCount(engine, part);
  if (count == -1)
    return false;

  for (i = 0; i < count; ++i)
  {
    voMusicElement element;
    voTypeId type;

    if (!state->measure)
    {
      length = sprintf(buffer, "<measure number=\"%d\">\n", ++state->measureNumber);
      startElement(state, buffer, length);

      state->measure = true;
    }

    element = VO_IFACE(engine, IMusicPart)->getElementAt(engine, part, i);
    if (element == NULL)
      return false;

    type = voGetType(engine, element);
    switch (type)
    {
      case VO_MusicClef:
      case VO_MusicKeySignature:
      case VO_MusicTimeSignature:
        state->attributes = true;
        break;

      case VO_MusicBar:
        break;

      default:
        if (state->attributes)
          successful = attributesToMusicXML(engine, state);
        break;
    };

    if (!successful)
      return false;

    switch (type)
    {
      case VO_MusicBar:
        successful = barToMusicXML(engine, (voMusicBar)element, state);
        break;

      case VO_MusicClef:
        state->clef = voAcquireObject(engine, element);
        break;

      case VO_MusicKeySignature:
        state->keySignature = voAcquireObject(engine, element);
        break;

      case VO_MusicTimeSignature:
        state->timeSignature = voAcquireObject(engine, element);
        break;

      case VO_MusicNote:
        successful = noteToMusicXML(engine, (voMusicNote)element, false, state);
        break;

      case VO_MusicRest:
        successful = restToMusicXML(engine, (voMusicRest)element, state);
        break;

      case VO_MusicChord:
        successful = chordToMusicXML(engine, (voMusicChord)element, state);
        break;

      default:
        SCORETOMUSICXML_ERROR("invalid top level element");
        successful = false;
    };

    voReleaseObject(engine, element);

    if (!successful)
      return false;
  }

  if (state->attributes)
    successful = attributesToMusicXML(engine, state);

  if (!successful)
    return false;

  if (state->measure)
  {
    endElement(state, "</measure>\n", -1);
    state->measure = false;
  }

  return true;
}

typedef struct Decoration_
{
  voMusicDecorationSymbol symbol;
  voMusicPlacement placement;
}
Decoration;

static bool decorationsToMusicXML(voEngine engine, voMusicElement element, OutputState* state)
{
  char buffer[SCORETOMUSICXML_BUFFER_SIZE];
  int length;
  voTypeId type;
  int i, count;
  voMusicDecoration decoration;
  Decoration fermata[SCORETOMUSICXML_DECORATION_BUFFER_SIZE];
  Decoration ornaments[SCORETOMUSICXML_DECORATION_BUFFER_SIZE];
  Decoration technical[SCORETOMUSICXML_DECORATION_BUFFER_SIZE];
  Decoration articulations[SCORETOMUSICXML_DECORATION_BUFFER_SIZE];
  int fermataCount = 0;
  int ornamentsCount = 0;
  int technicalCount = 0;
  int articulationsCount = 0;

  static const char* placementToStr[] = {
    "above",
    "below"
  };
  static const char* ornamentsToStr[] = {
    "mordent",
    "trill-mark",
    "inverted-mordent",
    "inverted-turn",
    "turn",
  };
  static const char* technicalToStr[] = {
    "up-bow",
    "stopped",
    "down-bow"
  };
  static const char* articulationsToStr[] = {
    "accent",
    "staccato",
    "tenuto",
    "staccatissimo",
    "strong-accent"
  };
  static const char* fermataPlacementToStr[] = {
    "upright",
    "inverted"
  };
  static const char* fermataToStr[] = {
    "normal",
    "angled",
    "square"
  };

  type = voGetType(engine, element);
  if (type == -1)
    return false;

  switch (type)
  {
    case VO_MusicBar:
      count = VO_IFACE(engine, IMusicBar)->getDecorationCount(engine, (voMusicBar)element);
      break;

    case VO_MusicNote:
      count = VO_IFACE(engine, IMusicNote)->getDecorationCount(engine, (voMusicNote)element);
      break;

    case VO_MusicRest:
      count = VO_IFACE(engine, IMusicRest)->getDecorationCount(engine, (voMusicRest)element);
      break;

    case VO_MusicChord:
      count = VO_IFACE(engine, IMusicChord)->getDecorationCount(engine, (voMusicChord)element);
      break;

    default:
      SCORETOMUSICXML_ERROR("unsupported element");
      return false;
  }

  if (count == -1)
    return false;

  for (i = 0; i < count; ++i)
  {
    Decoration current;

    switch (type)
    {
      case VO_MusicBar:
        decoration = VO_IFACE(engine, IMusicBar)->getDecorationAt(engine, (voMusicBar)element, i);
        break;

      case VO_MusicNote:
        decoration = VO_IFACE(engine, IMusicNote)->getDecorationAt(engine, (voMusicNote)element, i);
        break;

      case VO_MusicRest:
        decoration = VO_IFACE(engine, IMusicRest)->getDecorationAt(engine, (voMusicRest)element, i);
        break;

      case VO_MusicChord:
        decoration = VO_IFACE(engine, IMusicChord)->getDecorationAt(engine, (voMusicChord)element, i);
        break;

      default:
        SCORETOMUSICXML_ERROR("unsupported element");
        return false;
    }

    if (decoration == NULL)
      break;

    current.symbol = VO_IFACE(engine, IMusicDecoration)->getSymbol(engine, decoration);
    if (current.symbol == VO_MUSIC_DECORATION_SYMBOL_ERR)
    {
      voReleaseObject(engine, decoration);
      break;
    }

    current.placement = VO_IFACE(engine, IMusicDecoration)->getPlacement(engine, decoration);
    if (current.placement == VO_MUSIC_PLACEMENT_ERR)
    {
      voReleaseObject(engine, decoration);
      break;
    }

    switch (current.symbol)
    {
      case VO_MUSIC_DECORATION_SYMBOL_NORMAL_FERMATA:
      case VO_MUSIC_DECORATION_SYMBOL_ANGLED_FERMATA:
      case VO_MUSIC_DECORATION_SYMBOL_SQUARE_FERMATA:
        fermata[fermataCount] = current;
        ++fermataCount;
        break;

      case VO_MUSIC_DECORATION_SYMBOL_ACCENT:
      case VO_MUSIC_DECORATION_SYMBOL_STACCATO:
      case VO_MUSIC_DECORATION_SYMBOL_TENUTO:
      case VO_MUSIC_DECORATION_SYMBOL_STACCATISSIMO:
      case VO_MUSIC_DECORATION_SYMBOL_STRONG_ACCENT_UP:
        articulations[articulationsCount] = current;
        ++articulationsCount;
        break;

      case VO_MUSIC_DECORATION_SYMBOL_MORDENT:
      case VO_MUSIC_DECORATION_SYMBOL_TRILL_MARK:
      case VO_MUSIC_DECORATION_SYMBOL_INVERTED_MORDENT:
      case VO_MUSIC_DECORATION_SYMBOL_INVERTED_TURN:
      case VO_MUSIC_DECORATION_SYMBOL_TURN:
        ornaments[ornamentsCount] = current;
        ++ornamentsCount;
        break;

      case VO_MUSIC_DECORATION_SYMBOL_UP_BOW:
      case VO_MUSIC_DECORATION_SYMBOL_STOPPED:
      case VO_MUSIC_DECORATION_SYMBOL_DOWN_BOW:
        technical[technicalCount] = current;
        ++technicalCount;
        break;

      default:
        SCORETOMUSICXML_ERROR("unknown decoration symbol");
        break;
    }

    voReleaseObject(engine, decoration);
  }

  if (i == count)
  {
    // ornaments
    if (ornamentsCount != 0)
    {
      startElement(state, "<ornaments>\n", -1);

      for (i = 0; i < ornamentsCount; ++i)
      {
        length = sprintf(buffer, "<%s placement=\"%s\"/>\n", ornamentsToStr[ornaments[i].symbol - VO_MUSIC_DECORATION_SYMBOL_MORDENT], placementToStr[ornaments[i].placement]);
        append(state, buffer, length);
      }

      endElement(state, "</ornaments>\n", -1);
    }

    // technical
    if (technicalCount != 0)
    {
      startElement(state, "<technical>\n", -1);

      for (i = 0; i < technicalCount; ++i)
      {
        length = sprintf(buffer, "<%s placement=\"%s\"/>\n", technicalToStr[technical[i].symbol - VO_MUSIC_DECORATION_SYMBOL_UP_BOW], placementToStr[technical[i].placement]);
        append(state, buffer, length);
      }

      endElement(state, "</technical>\n", -1);
    }

    // articulations
    if (articulationsCount != 0)
    {
      startElement(state, "<articulations>\n", -1);

      for (i = 0; i < articulationsCount; ++i)
      {
        length = sprintf(buffer, "<%s placement=\"%s\"/>\n", articulationsToStr[articulations[i].symbol - VO_MUSIC_DECORATION_SYMBOL_ACCENT], placementToStr[articulations[i].placement]);
        append(state, buffer, length);
      }

      endElement(state, "</articulations>\n", -1);
    }

    // fermata
    for (i = 0; i < fermataCount; ++i)
    {
       length = sprintf(buffer, "<fermata type=\"%s\">%s</fermata>\n", fermataPlacementToStr[fermata[i].placement], fermataToStr[fermata[i].symbol - VO_MUSIC_DECORATION_SYMBOL_NORMAL_FERMATA]);
       append(state, buffer, length);
    }

    return true;
  }

  return false;
}

static bool barToMusicXML(voEngine engine, voMusicBar bar, OutputState* state)
{
  static const char* styleToStr[] = {
    "regular",
    "dotted",
    "light-light",
    "light-heavy",
    "heavy-light"
  };

  static const char* repeatDirectionToStr[] = {
    "none",
    "forward",
    "backward"
  };

  char buffer[SCORETOMUSICXML_BUFFER_SIZE];
  int length;
  voMusicBarStyle style;
  voMusicBarRepeatDirection repeatDirection;
  bool repeatForwardLeft;

  assert(voGetType(engine, bar) == VO_MusicBar);

  style = VO_IFACE(engine, IMusicBar)->getStyle(engine, bar);
  if (style == VO_MUSIC_BAR_STYLE_ERR)
    return false;

  repeatDirection = VO_IFACE(engine, IMusicBar)->getRepeatDirection(engine, bar);
  if (repeatDirection == VO_MUSIC_BAR_REPEAT_DIRECTION_ERR)
    return false;

  repeatForwardLeft = (state->attributes && (style == VO_MUSIC_BAR_STYLE_HEAVY_LIGHT) && (repeatDirection == VO_MUSIC_BAR_REPEAT_DIRECTION_FORWARD));
  if (state->attributes && !attributesToMusicXML(engine, state))
    return false;

  if (repeatForwardLeft)
  {
    startElement(state, "<barline location=\"left\">\n", -1);
  }
  else
  {
    startElement(state, "<barline location=\"right\">\n", -1);
  }

  length = sprintf(buffer, "<bar-style>%s</bar-style>\n", styleToStr[style]);
  append(state, buffer, length);

  if (repeatDirection != VO_MUSIC_BAR_REPEAT_DIRECTION_NONE)
  {
    length = sprintf(buffer, "<repeat direction=\"%s\"/>\n", repeatDirectionToStr[repeatDirection]);
    append(state, buffer, length);
  }

  if (!decorationsToMusicXML(engine, bar, state))
    return false;

  endElement(state, "</barline>\n", -1);

  if (!repeatForwardLeft)
  {
    endElement(state, "</measure>\n", -1);
    state->measure = false;
  }

  return true;
}

static bool clefToMusicXML(voEngine engine, voMusicClef clef, OutputState* state)
{
  static const char* symbolToStr[] = {
    "G",
    "F",
    "C"
  };

  char buffer[SCORETOMUSICXML_BUFFER_SIZE];
  int length;
  voMusicClefSymbol symbol;
  int line, octave;

  assert(voGetType(engine, clef) == VO_MusicClef);

  startElement(state, "<clef>\n", -1);

  symbol = VO_IFACE(engine, IMusicClef)->getSymbol(engine, clef);
  if (symbol == VO_MUSIC_CLEF_SYMBOL_ERR)
    return false;

  startElement(state, "<sign>", -1);
  length = sprintf(buffer, "%s", symbolToStr[symbol]);
  append(state, buffer, length);
  endElement(state, "</sign>\n", -1);

  line = VO_IFACE(engine, IMusicClef)->getLine(engine, clef);
  if (line == INT32_MAX)
    return false;

  startElement(state, "<line>", -1);
  length = sprintf(buffer, "%d", line);
  append(state, buffer, length);
  endElement(state, "</line>\n", -1);

  octave = VO_IFACE(engine, IMusicClef)->getOctave(engine, clef);
  if (octave == INT32_MAX)
    return false;

  startElement(state, "<clef-octave-change>", -1);
  length = sprintf(buffer, "%d", octave);
  append(state, buffer, length);
  endElement(state, "</clef-octave-change>\n", -1);

  endElement(state, "</clef>\n", -1);

  return true;
}

static bool keySignatureToMusicXML(voEngine engine, voMusicKeySignature signature, OutputState* state)
{
  char buffer[SCORETOMUSICXML_BUFFER_SIZE];
  int length;
  voMusicKeySignatureData data;

  assert(voGetType(engine, signature) == VO_MusicKeySignature);

  if (VO_IFACE(engine, IMusicKeySignature)->getSignature(engine, signature, &data) == false)
    return false;

  startElement(state, "<key>\n", -1);

  if (data.cancel)
  {
    length = sprintf(buffer, "<cancel>%d</cancel>\n", data.cancel);
    append(state, buffer, length);
  }

  length = sprintf(buffer, "<fifths>%d</fifths>\n", data.fifths);
  append(state, buffer, length);

  append(state, "<mode>major</mode>\n", -1);

  endElement(state, "</key>\n", -1);

  return true;
}

static bool timeSignatureToMusicXML(voEngine engine, voMusicTimeSignature signature, OutputState* state)
{
  static const char* typeToStr[] = {
    "normal",
    "common",
    "cut"
  };

  char buffer[SCORETOMUSICXML_BUFFER_SIZE];
  int length;
  voMusicTimeSignatureType type;
  bool successful = true;

  assert(voGetType(engine, signature) == VO_MusicTimeSignature);

  type = VO_IFACE(engine, IMusicTimeSignature)->getType(engine, signature);
  if (type == VO_MUSIC_TIME_SIGNATURE_TYPE_ERR)
    return false;

  switch (type)
  {
    case VO_MUSIC_TIME_SIGNATURE_TYPE_COMMON:
      length = sprintf(buffer, "<time symbol=\"%s\">\n", typeToStr[type]);
      startElement(state, buffer, length);
      append(state, "<beats>4</beats>\n", -1);
      append(state, "<beat-type>4</beat-type>\n", -1);
      break;

    case VO_MUSIC_TIME_SIGNATURE_TYPE_ALLA_BREVE:
      length = sprintf(buffer, "<time symbol=\"%s\">\n", typeToStr[type]);
      startElement(state, buffer, length);
      append(state, "<beats>2</beats>\n", -1);
      append(state, "<beat-type>2</beat-type>\n", -1);
      break;

    case VO_MUSIC_TIME_SIGNATURE_TYPE_NORMAL:
    {
      voMusicAnnotation top = NULL;
      voMusicAnnotation bottom = NULL;
      voCharset charset;
      voString label = { buffer, sizeof(buffer) };

      startElement(state, "<time>\n", -1);

      for (;;)
      {
        charset = voCreateCharset(engine, "UTF-8");
        assert(charset);

        top = VO_IFACE(engine, IMusicTimeSignature)->getTop(engine, signature);
        if (top == NULL)
          break;

        bottom = VO_IFACE(engine, IMusicTimeSignature)->getBottom(engine, signature);
        if (bottom == NULL)
          break;

        successful = VO_IFACE(engine, IMusicAnnotation)->getLabel(engine, top, charset, &label);
        assert(label.byteCount <= sizeof(buffer));
        if (!successful)
          break;

        append(state, "<beats>", -1);
        append(state, label.bytes, (int)label.byteCount);
        append(state, "</beats>\n", -1);

        successful = VO_IFACE(engine, IMusicAnnotation)->getLabel(engine, bottom, charset, &label);
        assert(label.byteCount <= sizeof(buffer));
        if (!successful)
          break;

        append(state, "<beat-type>", -1);
        append(state, label.bytes, (int)label.byteCount);
        append(state, "</beat-type>\n", -1);

        break;
      }

      voReleaseObject(engine, charset);
      if (top != NULL)
        voReleaseObject(engine, top);
      if (bottom != NULL)
        voReleaseObject(engine, bottom);

      break;
    }

    default:
      SCORETOMUSICXML_ERROR("unknown time signature type");
      return false;
  }

  if (!successful)
    return false;

  endElement(state, "</time>\n", -1);

  return true;
}

static int lookupSlur(voEngine engine, voMusicSlur slur, bool start, OutputState* state)
{
  int i;

  assert(voGetType(engine, slur) == VO_MusicSlur);

  if (start) // start slur -> find next free slot
  {
    for(i = 0; i < SCORETOMUSICXML_SLUR_BUFFER_SIZE; ++i)
    {
      if (state->slurs[i] == NULL)
      {
        state->slurs[i] = voAcquireObject(engine, slur);
        return i;
      }
    }
  }
  else // end slur -> release slot
  {
    for(i = 0; i < SCORETOMUSICXML_SLUR_BUFFER_SIZE; ++i)
    {
      if (state->slurs[i] != NULL)
      {
        int count;

        count = VO_IFACE(engine, IMusicElement)->getInputRangeCount(engine, state->slurs[i]);
        if (count == -1)
          return -1;

        if (count == VO_IFACE(engine, IMusicElement)->getInputRangeCount(engine, slur))
        {
          int j;

          for(j = 0; j < count; ++j)
          {
            voMusicInputRange lhs, rhs;

            if (!VO_IFACE(engine,IMusicElement)->getInputRangeAt(engine, state->slurs[i], j, &lhs))
              return -1;
            if (!VO_IFACE(engine,IMusicElement)->getInputRangeAt(engine, slur, j, &rhs))
              return -1;

            if (  lhs.component != rhs.component
                ||lhs.firstItem != rhs.firstItem
                ||lhs.lastItem != rhs.lastItem)
              break;
          }

          if (j == count) // found
          {
            voReleaseObject(engine,  state->slurs[i]);
            state->slurs[i] = NULL;
            return i;
          }
        }
      }
    }
  }

  return -1;
}

static bool dotsToMusicXML(voEngine engine, voMusicElement element, OutputState* state)
{
  voTypeId type;
  voMusicDots dots;

  type = voGetType(engine, element);
  if (type == -1)
    return false;

  switch (type)
  {
    case VO_MusicNote:
      dots = VO_IFACE(engine, IMusicNote)->getDots(engine, (voMusicNote)element);
      break;

    case VO_MusicRest:
      dots = VO_IFACE(engine, IMusicRest)->getDots(engine, (voMusicRest)element);
      break;

    default:
      SCORETOMUSICXML_ERROR("unsupported element");
      return false;
  }
  if (voGetError(engine) != VO_NO_ERROR)
    return false;

  if (dots != NULL)
  {
    int i, count;

    count = VO_IFACE(engine, IMusicDots)->getCount(engine, dots);
    if (count == -1)
    {
      voReleaseObject(engine, dots);
      return false;
    }

    for (i = 0; i < count; ++i)
      append(state, "<dot/>\n", -1);

    voReleaseObject(engine, dots);
  }

  return true;
}

static bool stemToMusicXML(voEngine engine, voMusicElement element, OutputState* state)
{
  voTypeId type;
  voMusicStem stem;

  type = voGetType(engine, element);
  if (type == -1)
    return false;

  switch (type)
  {
    case VO_MusicNote:
      stem = VO_IFACE(engine, IMusicNote)->getStem(engine, (voMusicNote)element);
      break;

    case VO_MusicChord:
      stem = VO_IFACE(engine, IMusicChord)->getStem(engine, (voMusicRest)element);
      break;

    default:
      SCORETOMUSICXML_ERROR("unsupported element");
      return false;
  }
  if (voGetError(engine) != VO_NO_ERROR)
    return false;

  if (stem != NULL)
  {
    static const char* typeToStr[] = {
      "up", "up", "up", "up","up",
      "down", "down", "down", "down","down"
    };

    char buffer[SCORETOMUSICXML_BUFFER_SIZE];
    int length;
    voMusicStemType stemType;

    stemType = VO_IFACE(engine, IMusicStem)->getType(engine, stem);
    if (stemType == VO_MUSIC_STEM_TYPE_ERR)
    {
      voReleaseObject(engine, stem);
      return false;
    }

    length = sprintf(buffer, "<stem>%s</stem>\n", typeToStr[stemType]);
    append(state, buffer, length);

    voReleaseObject(engine, stem);
  }

  return true;
}

static bool slursToMusicXML(voEngine engine, voMusicElement element, OutputState* state)
{
  static const char* placementToStr[] = {
    "above",
    "below"
  };

  char buffer[SCORETOMUSICXML_BUFFER_SIZE];
  int length;
  voTypeId type;
  int i, stopSlurCount, startSlurCount;

  type = voGetType(engine, element);
  if (type == -1)
    return false;

  switch (type)
  {
    case VO_MusicNote:
      stopSlurCount = VO_IFACE(engine, IMusicNote)->getStopSlurCount(engine, (voMusicNote)element);
      startSlurCount = VO_IFACE(engine, IMusicNote)->getStartSlurCount(engine, (voMusicNote)element);
      break;

    case VO_MusicRest:
      stopSlurCount = VO_IFACE(engine, IMusicRest)->getStopSlurCount(engine, (voMusicRest)element);
      startSlurCount = VO_IFACE(engine, IMusicRest)->getStartSlurCount(engine, (voMusicRest)element);
      break;

    case VO_MusicChord:
      stopSlurCount = VO_IFACE(engine, IMusicChord)->getStopSlurCount(engine, (voMusicChord)element);
      startSlurCount = VO_IFACE(engine, IMusicChord)->getStartSlurCount(engine, (voMusicChord)element);
      break;

    default:
      SCORETOMUSICXML_ERROR("unsupported element");
      return false;
  }

  if (stopSlurCount == -1 || startSlurCount == -1)
    return false;

  for (i = 0; i < stopSlurCount; ++i)
  {
    voMusicSlur slur;
    voMusicPlacement placement;
    int j;

    switch (type)
    {
      case VO_MusicNote:
        slur = VO_IFACE(engine, IMusicNote)->getStopSlurAt(engine, (voMusicNote)element, i);
        break;

      case VO_MusicRest:
        slur = VO_IFACE(engine, IMusicRest)->getStopSlurAt(engine, (voMusicRest)element, i);
        break;

      case VO_MusicChord:
        slur = VO_IFACE(engine, IMusicChord)->getStopSlurAt(engine, (voMusicChord)element, i);
        break;

      default:
        SCORETOMUSICXML_ERROR("unsupported element");
        return false;
    }

    if (slur == NULL)
      break;

    j = lookupSlur(engine, slur, false, state);
    if (j == -1)
    {
      voReleaseObject(engine, slur);
      break;
    }

    placement = VO_IFACE(engine, IMusicSlur)->getPlacement(engine, slur);
    if (placement == VO_MUSIC_PLACEMENT_ERR)
    {
      voReleaseObject(engine, slur);
      break;
    }

    length = sprintf(buffer, "<slur type=\"stop\" number=\"%d\" placement=\"%s\"/>\n", j + 1, placementToStr[placement]);
    append(state, buffer, length);

    voReleaseObject(engine, slur);
  }

  if (i != stopSlurCount)
    return false;

  for (i = 0; i < startSlurCount; ++i)
  {
    voMusicSlur slur;
    voMusicPlacement placement;
    int j;

    switch (type)
    {
      case VO_MusicNote:
        slur = VO_IFACE(engine, IMusicNote)->getStartSlurAt(engine, (voMusicNote)element, i);
        break;

      case VO_MusicRest:
        slur = VO_IFACE(engine, IMusicRest)->getStartSlurAt(engine, (voMusicRest)element, i);
        break;

      case VO_MusicChord:
        slur = VO_IFACE(engine, IMusicChord)->getStartSlurAt(engine, (voMusicChord)element, i);
        break;

      default:
        SCORETOMUSICXML_ERROR("unsupported element");
        return false;
    }

    if (slur == NULL)
      break;

    j = lookupSlur(engine, slur, true, state);
    if (j == -1)
    {
      voReleaseObject(engine, slur);
      break;
    }

    placement = VO_IFACE(engine, IMusicSlur)->getPlacement(engine, slur);
    if (placement == VO_MUSIC_PLACEMENT_ERR)
    {
      voReleaseObject(engine, slur);
      break;
    }

    length = sprintf(buffer, "<slur type=\"start\" number=\"%d\" placement=\"%s\"/>\n", j + 1, placementToStr[placement]);
    append(state, buffer, length);

    voReleaseObject(engine, slur);
  }

  if (i != startSlurCount)
    return false;

  return true;
}

static bool timeModificationToMusicXML(voEngine engine, voMusicElement element, OutputState* state)
{
  static const char* timeModificationTypeToStr[] = {
    "none",
    "unknown",
    "whole",
    "half",
    "quarter",
    "eighth",
    "16th",
    "32nd",
    "64th"
  };

  voTypeId type;
  voMusicTimeModificationData timeModification;

  type = voGetType(engine, element);
  if (type == -1)
    return false;

  switch (type)
  {
    case VO_MusicNote:
      if (!VO_IFACE(engine, IMusicNote)->getTimeModification(engine, (voMusicNote)element, &timeModification))
        return false;
      break;

    case VO_MusicRest:
      if (!VO_IFACE(engine, IMusicRest)->getTimeModification(engine, (voMusicRest)element, &timeModification))
        return false;
      break;

    default:
      SCORETOMUSICXML_ERROR("unsupported element");
      return false;
  }

  if (timeModification.type != VO_MUSIC_TIME_MODIFICATION_TYPE_NONE)
  {
    char buffer[SCORETOMUSICXML_BUFFER_SIZE];
    int length;

    startElement(state, "<time-modification>\n", -1);

    length = sprintf(buffer, "<actual-notes>%d</actual-notes>\n", timeModification.actual);
    append(state, buffer, length);

    length = sprintf(buffer, "<normal-notes>%d</normal-notes>\n", timeModification.normal);
    append(state, buffer, length);

    if (timeModification.type != VO_MUSIC_TIME_MODIFICATION_TYPE_UNKNOWN)
    {
      int i;

      length = sprintf(buffer, "<normal-type>%s</normal-type>\n", timeModificationTypeToStr[timeModification.type]);
      append(state, buffer, length);

      for (i = 0; i < timeModification.dots; ++i)
        append(state, "<normal-dot/>\n", -1);
    }

    endElement(state, "</time-modification>\n", -1);
  }
  return true;
}

static bool tupletToMusicXML(voEngine engine, voMusicElement element, const char* type, OutputState* state)
{
  static const char* placementToStr[] = {
    "above",
    "below"
  };

  char buffer[SCORETOMUSICXML_BUFFER_SIZE];
  int length;
  voMusicPlacement placement;
  int bracketCount;
  voMusicAnnotation tupletNumber = NULL;

  assert(voGetType(engine, element) == VO_MusicTuplet);

  length = sprintf(buffer, "<tuplet type=\"%s\" ", type);
  append(state, buffer, length);

  placement = VO_IFACE(engine, IMusicTuplet)->getPlacement(engine, element);
  if (placement == VO_MUSIC_PLACEMENT_ERR)
     return false;

  length = sprintf(buffer, "placement=\"%s\" ", placementToStr[placement]);
  append(state, buffer, length);

  bracketCount = VO_IFACE(engine, IMusicTuplet)->getBracketCount(engine, element);
  if (bracketCount == -1)
    return false;

  if (bracketCount > 0)
    append(state, "bracket=\"yes\" ", -1);
  else
    append(state, "bracket=\"no\" ", -1);

  tupletNumber = VO_IFACE(engine, IMusicTuplet)->getNumber(engine, element);
  if (voGetError(engine) != VO_NO_ERROR)
    return false;

  if (tupletNumber != NULL)
  {
    voCharset charset;
    voString label = { buffer, sizeof(buffer) };
    bool successful;

    charset = voCreateCharset(engine, "UTF-8");
    assert(charset);

    for (;;)
    {
      size_t i;
      bool showBothNumbers = false;

      successful = VO_IFACE(engine, IMusicAnnotation)->getLabel(engine, tupletNumber, charset, &label);
      assert(label.byteCount <= sizeof(buffer));
      if (!successful)
        break;

      for (i = 0; i < label.byteCount; ++i)
      {
        if (label.bytes[i] == ':')
        {
          showBothNumbers = true;
          break;
        }
      }

      if (showBothNumbers)
        append(state, "show-number=\"both\"", -1);
      else
        append(state, "show-number=\"actual\"", -1);

      break;
    }

    voReleaseObject(engine, charset);

    if (tupletNumber != NULL)
      voReleaseObject(engine, tupletNumber);

    if (!successful)
      return false;
  }
  else
    append(state, "show-number=\"none\"", -1);

  append(state, "/>\n", -1);

  return true;
}

static bool noteToMusicXML(voEngine engine, voMusicNote note, bool chordTag, OutputState* state)
{
  static const char* placementToStr[] = {
    "above",
    "below"
  };

  static const char* typeToStr[] = {
    "whole",
    "half",
    "quarter",
    "eighth",
    "16th",
    "32nd",
    "64th"
  };

  char buffer[SCORETOMUSICXML_BUFFER_SIZE];
  int length;
  voMusicTie startTie = NULL;
  voMusicTie stopTie = NULL;
  voMusicAccidental accidental = NULL;
  voMusicArpeggiate arpeggiate = NULL;
  voMusicTuplet startTuplet = NULL;
  voMusicTuplet stopTuplet = NULL;
  bool successful = false;

  assert(voGetType(engine, note) == VO_MusicNote);

  for (;;)
  {
    voMusicPitchData pitch;
    int duration;
    voMusicNoteType type;
    int startSlurCount, stopSlurCount, decorationCount;

    startElement(state, "<note>\n", -1);

    if (chordTag)
      append(state, "<chord/>\n", -1);

    if (!VO_IFACE(engine, IMusicNote)->getPitch(engine, note, &pitch))
      break;

    if (pitch.step != VO_MUSIC_PITCH_STEP_UNKNOWN)
    {
      startElement(state, "<pitch>\n", -1);

      length = sprintf(buffer, "<step>%c</step>\n", 'A' + pitch.step - 1);
      append(state, buffer, length);

      if (pitch.alteration)
      {
        length = sprintf(buffer, "<alter>%d</alter>\n", pitch.alteration);
        append(state, buffer, length);
      }

      length = sprintf(buffer, "<octave>%d</octave>\n", pitch.octave);
      append(state, buffer, length);

      endElement(state, "</pitch>\n", -1);
    }

    duration = VO_IFACE(engine, IMusicNote)->getDuration(engine, note, SCORETOMUSICXML_DIVISIONS);
    if (duration == -1)
      break;

    length = sprintf(buffer, "<duration>%d</duration>\n", duration);
    append(state, buffer, length);

    stopTie = VO_IFACE(engine, IMusicNote)->getStopTie(engine, note);
    if (voGetError(engine) != VO_NO_ERROR)
      break;

    startTie = VO_IFACE(engine, IMusicNote)->getStartTie(engine, note);
    if (voGetError(engine) != VO_NO_ERROR)
      break;

    if (stopTie != NULL)
      append(state, "<tie type=\"stop\"/>\n", -1);

    if (startTie != NULL)
      append(state, "<tie type=\"start\"/>\n", -1);

    append(state, "<voice>1</voice>\n", -1);

    type = VO_IFACE(engine, IMusicNote)->getType(engine, note);
    if (type == VO_MUSIC_NOTE_TYPE_ERR)
      break;

    length = sprintf(buffer, "<type>%s</type>\n", typeToStr[type]);
    append(state, buffer, length);

    if (!dotsToMusicXML(engine, note, state))
      break;

    accidental = VO_IFACE(engine, IMusicNote)->getAccidental(engine, note);
    if (voGetError(engine) != VO_NO_ERROR)
      break;

    if (accidental != NULL)
    {
      static const char* accidentalTypeToStr[] = {
        "natural",
        "sharp",
        "double-sharp",
        "flat",
        "flat-flat"
      };

      voMusicAccidentalType accidentalType;

      accidentalType = VO_IFACE(engine, IMusicAccidental)->getType(engine, accidental);
      if (accidentalType == VO_MUSIC_ACCIDENTAL_TYPE_ERR)
      {
        voReleaseObject(engine, accidental);
        return false;
      }

      length = sprintf(buffer, "<accidental>%s</accidental>\n", accidentalTypeToStr[accidentalType]);
      append(state, buffer, length);
    }

    if (!timeModificationToMusicXML(engine, note, state))
       break;

    if (state->chord != NULL)
    {
      if (!stemToMusicXML(engine, state->chord, state))
        break;
    }
    else
    {
      if (!stemToMusicXML(engine, note, state))
        break;
    }

    if (state->chord != NULL)
    {
      arpeggiate = VO_IFACE(engine, IMusicChord)->getArpeggiate(engine, state->chord);
      if (voGetError(engine) != VO_NO_ERROR)
        break;
     }

    startSlurCount = stopSlurCount = decorationCount = 0;
    if (!chordTag)
    {
      static const char* beamTypeToStr[] = {
        "begin",
        "continue",
        "end",
        "forward hook",
        "backward hook"
      };

      int i, count;

      if (state->chord != NULL)
      {
        count = VO_IFACE(engine, IMusicChord)->getBeamTypeCount(engine, state->chord);
        if (count == -1)
          return false;

        for (i = 0; i < count; ++i)
        {
          voMusicBeamType beamType;

          beamType = VO_IFACE(engine, IMusicChord)->getBeamTypeAt(engine, state->chord, i);
          if (beamType == VO_MUSIC_BEAM_TYPE_ERR)
            return false;

          length = sprintf(buffer, "<beam number=\"%d\">%s</beam>\n", i + 1, beamTypeToStr[beamType]);
          append(state, buffer, length);
        }
      }
      else
      {
        count = VO_IFACE(engine, IMusicNote)->getBeamTypeCount(engine, note);
        if (count == -1)
          return false;

        for (i = 0; i < count; ++i)
        {
          voMusicBeamType beamType;

          beamType = VO_IFACE(engine, IMusicNote)->getBeamTypeAt(engine, note, i);
          if (beamType == VO_MUSIC_BEAM_TYPE_ERR)
            return false;

          length = sprintf(buffer, "<beam number=\"%d\">%s</beam>\n", i + 1, beamTypeToStr[beamType]);
          append(state, buffer, length);
        }
      }

      if (state->chord != NULL)
      {
        startSlurCount = VO_IFACE(engine, IMusicChord)->getStartSlurCount(engine, state->chord);
        if (startSlurCount == -1)
          break;

        stopSlurCount = VO_IFACE(engine, IMusicChord)->getStopSlurCount(engine, state->chord);
        if (stopSlurCount == -1)
          break;

        decorationCount = VO_IFACE(engine, IMusicChord)->getDecorationCount(engine, state->chord);
        if (decorationCount == -1)
          break;

        startTuplet =  VO_IFACE(engine, IMusicChord)->getStartTuplet(engine, state->chord);
        if (voGetError(engine) != VO_NO_ERROR)
          break;

        stopTuplet =  VO_IFACE(engine, IMusicChord)->getStopTuplet(engine, state->chord);
        if (voGetError(engine) != VO_NO_ERROR)
          break;
      }
      else
      {
        startSlurCount = VO_IFACE(engine, IMusicNote)->getStartSlurCount(engine, note);
        if (startSlurCount == -1)
          break;

        stopSlurCount = VO_IFACE(engine, IMusicNote)->getStopSlurCount(engine, note);
        if (stopSlurCount == -1)
          break;

        startTuplet =  VO_IFACE(engine, IMusicNote)->getStartTuplet(engine, note);
        if (voGetError(engine) != VO_NO_ERROR)
          break;

        stopTuplet =  VO_IFACE(engine, IMusicNote)->getStopTuplet(engine, note);
        if (voGetError(engine) != VO_NO_ERROR)
          break;

        decorationCount = VO_IFACE(engine, IMusicNote)->getDecorationCount(engine, note);
        if (decorationCount == -1)
          break;
      }
    }

    if (stopTie != NULL || startTie != NULL || startSlurCount != 0 || stopSlurCount != 0 || decorationCount != 0 || arpeggiate != NULL || startTuplet != NULL || stopTuplet != NULL)
      startElement(state, "<notations>\n", -1);

    if (stopTie != NULL)
    {
      voMusicPlacement placement = VO_IFACE(engine, IMusicTie)->getPlacement(engine, stopTie);
      if (placement == VO_MUSIC_PLACEMENT_ERR)
        break;

      length = sprintf(buffer, "<tied type=\"stop\" placement=\"%s\"/>\n", placementToStr[placement]);
      append(state, buffer, length);
    }

    if (startTie != NULL)
    {
      voMusicPlacement placement = VO_IFACE(engine, IMusicTie)->getPlacement(engine, startTie);
      if (placement == VO_MUSIC_PLACEMENT_ERR)
        break;

      length = sprintf(buffer, "<tied type=\"start\" placement=\"%s\"/>\n", placementToStr[placement]);
      append(state, buffer, length);
    }

    if (!chordTag)
    {
      if (state->chord != NULL)
      {
        if (!slursToMusicXML(engine, state->chord, state))
          break;
      }
      else
      {
        if (!slursToMusicXML(engine, note, state))
          break;
      }
    }

    if (startTuplet != NULL)
      if (!tupletToMusicXML(engine, startTuplet, "start", state))
        break;

    if (stopTuplet != NULL)
      if (!tupletToMusicXML(engine, stopTuplet, "stop", state))
         break;

    if (!chordTag)
    {
      if (state->chord != NULL)
      {
        if (!decorationsToMusicXML(engine, state->chord, state))
          break;
      }
      else
      {
        if (!decorationsToMusicXML(engine, note, state))
          break;
      }
    }

    if (arpeggiate != NULL)
    {
      voMusicArpeggiateType arpeggiateType =  VO_IFACE(engine, IMusicArpeggiate)->getType(engine, arpeggiate);
      if (arpeggiateType == VO_MUSIC_ARPEGGIATE_TYPE_ERR)
        break;

      switch (arpeggiateType)
      {
        case VO_MUSIC_ARPEGGIATE_TYPE_NORMAL:
          append(state, "<arpeggiate/>\n", -1);
          break;

        case VO_MUSIC_ARPEGGIATE_TYPE_UP:
          append(state, "<arpeggiate direction=\"up\"/>\n", -1);
          break;

        case VO_MUSIC_ARPEGGIATE_TYPE_DOWN:
          append(state, "<arpeggiate direction=\"down\"/>\n", -1);
          break;

        case VO_MUSIC_ARPEGGIATE_TYPE_NON_ARPEGGIATE:
          append(state, "<non-arpeggiate/>\n", -1);
          break;

         default:
          SCORETOMUSICXML_ERROR("unknown arpeggiate type");
      }
    }

    if (stopTie != NULL || startTie != NULL || startSlurCount != 0 || stopSlurCount != 0 || decorationCount != 0 || arpeggiate != NULL || startTuplet != NULL || stopTuplet != NULL)
      endElement(state, "</notations>\n", -1);

    endElement(state, "</note>\n", -1);

    successful = true;
    break;
  }

  if (accidental != NULL)
    voReleaseObject(engine, accidental);
  if (startTie != NULL)
    voReleaseObject(engine, startTie);
  if (stopTie != NULL)
    voReleaseObject(engine, stopTie);
  if (arpeggiate != NULL)
    voReleaseObject(engine, arpeggiate);
  if (startTuplet != NULL)
    voReleaseObject(engine, startTuplet);
  if (stopTuplet != NULL)
    voReleaseObject(engine, stopTuplet);

  return successful;
}

static bool restToMusicXML(voEngine engine, voMusicRest rest, OutputState* state)
{
  static const char* typeToStr[] = {
    "whole",
    "half",
    "quarter",
    "eighth",
    "16th",
    "32nd",
    "64th"
  };

  char buffer[SCORETOMUSICXML_BUFFER_SIZE];
  int length;
  bool successful = false;
  voMusicTuplet startTuplet = NULL;
  voMusicTuplet stopTuplet = NULL;

  assert(voGetType(engine, rest) == VO_MusicRest);

  for (;;)
  {
    int duration;
    voMusicRestType type;
    int startSlurCount, stopSlurCount, decorationCount;

    startElement(state, "<note>\n", -1);

    append(state, "<rest/>\n", -1);

    duration = VO_IFACE(engine, IMusicRest)->getDuration(engine, rest, SCORETOMUSICXML_DIVISIONS);
    if (duration == -1)
      break;

    length = sprintf(buffer, "<duration>%d</duration>\n", duration);
    append(state, buffer, length);

    append(state, "<voice>1</voice>\n", -1);

    type = VO_IFACE(engine, IMusicRest)->getType(engine, rest);
    if (type == VO_MUSIC_REST_TYPE_ERR)
      break;

    length = sprintf(buffer, "<type>%s</type>\n", typeToStr[type]);
    append(state, buffer, length);

    if (!dotsToMusicXML(engine, rest, state))
      break;

    if (!timeModificationToMusicXML(engine, rest, state))
       break;

    startTuplet =  VO_IFACE(engine, IMusicRest)->getStartTuplet(engine, rest);
    if (voGetError(engine) != VO_NO_ERROR)
      break;

    stopTuplet =  VO_IFACE(engine, IMusicRest)->getStopTuplet(engine, rest);
    if (voGetError(engine) != VO_NO_ERROR)
      break;

    startSlurCount = VO_IFACE(engine, IMusicRest)->getStartSlurCount(engine, rest);
    if (startSlurCount == -1)
      break;

    stopSlurCount = VO_IFACE(engine, IMusicRest)->getStopSlurCount(engine, rest);
    if (stopSlurCount == -1)
      break;

    decorationCount = VO_IFACE(engine, IMusicRest)->getDecorationCount(engine, rest);
    if (decorationCount == -1)
      break;

    if (startSlurCount != 0 || stopSlurCount != 0 || decorationCount != 0 || startTuplet != NULL || stopTuplet != NULL)
      startElement(state, "<notations>\n", -1);

    if (!slursToMusicXML(engine, rest, state))
      break;

    if (startTuplet != NULL)
      if (!tupletToMusicXML(engine, startTuplet, "start", state))
        break;

    if (stopTuplet != NULL)
      if (!tupletToMusicXML(engine, stopTuplet, "stop", state))
        break;

    if (!decorationsToMusicXML(engine, rest, state))
      break;

    if (startSlurCount != 0 || stopSlurCount != 0 || decorationCount != 0 || startTuplet != NULL || stopTuplet != NULL)
      endElement(state, "</notations>\n", -1);

    endElement(state, "</note>\n", -1);

    successful = true;
    break;
  }

  return successful;
}

static bool chordToMusicXML(voEngine engine, voMusicChord chord, OutputState* state)
{
  int i, count;
  assert(voGetType(engine, chord) == VO_MusicChord);

  state->chord = chord;

  count = VO_IFACE(engine, IMusicChord)->getNoteCount(engine, chord);
  if (count == -1)
    return false;

  for (i = 0; i < count; ++i)
  {
    voMusicNote note = VO_IFACE(engine, IMusicChord)->getNoteAt(engine, chord, i);
    if (note == NULL)
      return false;

    if (!noteToMusicXML(engine, note, i != 0, state))
      return false;
  }

  state->chord = NULL;

  return true;
}
