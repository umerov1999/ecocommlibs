/**
 * @file MyScriptMath-parseTreeToMathML.c
 *
 * This file contains our sample implementation of the serialization of a parsed
 * math to a MathML string.
 *
 * This implementation is provided as-is, feel free to customize it to suit your
 * own needs.
 */

#include "MyScriptMath-parseTreeToMathML.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define PARSETREETOMATHML_BUFFER_SIZE 1024
#define PARSETREETOMATHML_ERROR(message)\
  fprintf(stderr, "parseTreeToMathML: error: %s\n", message);\
  fflush(stderr)

#define PARSETREETOMATHML_INDENT_SPACES 2
#define PARSETREETOMATHML_DISPLAY_STYLE 1

#define PARSETREETOMATHML_ROW_BUFFER_SIZE 64
#define PARSETREETOMATHML_COLUMN_BUFFER_SIZE 64
#define PARSETREETOMATHML_CARRY_BUFFER_SIZE 64

enum PARSETREETOMATHML_CARRY_LOCATION
{
  CARRY_LOCATION_NONE,
  CARRY_LOCATION_W,
  CARRY_LOCATION_NW,
  CARRY_LOCATION_N,
  CARRY_LOCATION_NE,
  CARRY_LOCATION_E,
  CARRY_LOCATION_SE,
  CARRY_LOCATION_S,
  CARRY_LOCATION_SW
};

typedef struct _OutputState
{
  char* destination;  // destination buffer
  int capacity;       // destination buffer capacity
  int offset;         // where to write next in the destination buffer

  bool startOfLine;   // start of line flag
  int indent;         // indent level

  bool number;        // <mn> element has started
  bool dot;           // and the character stream contains a decimal separator

} OutputState;

// forward declarations
static void startElement(OutputState* state, const char* input, int length);
static void endElement(OutputState* state, const char* input, int length);

static bool parseTreeToMathML_(voEngine engine, voMathNode node, OutputState* state);

int parseTreeToMathML(voEngine engine, voMathNode root, char* utf8Buffer, int capacity)
{
  OutputState state = { utf8Buffer, capacity };
  voMathNode node;
  bool successful;

  startElement(&state, "<math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n", -1);
#if PARSETREETOMATHML_DISPLAY_STYLE
  startElement(&state, "<mstyle displaystyle=\"true\">\n", -1);
#endif

  // keep a user reference to the root node while processing the tree
  node = voAcquireObject(engine, root);

  successful = parseTreeToMathML_(engine, node, &state);
  voReleaseObject(engine, node);

  if (!successful)
    return -1;

#if PARSETREETOMATHML_DISPLAY_STYLE
  endElement(&state, "</mstyle>\n", -1);
#endif
  endElement(&state, "</math>", -1);

  return state.offset;
}

static void parseAppend(OutputState* state, const char* input, int length)
{
  length = length == -1 ? (int)strlen(input) : length;

  // append only when destination is non NULL
  if (state->destination)
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
            int spaces = state->indent * PARSETREETOMATHML_INDENT_SPACES;

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
    int spaces = state->indent * PARSETREETOMATHML_INDENT_SPACES;

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

  if (state->number)
  {
    state->number = false;
    state->dot = false;
    parseAppend(state, " </mn>\n", -1);
    --state->indent;
  }

  parseAppend(state, input, length);
  ++state->indent;
}

static void addElement(OutputState* state, const char* input, int length)
{
  assert(state);

  if (state->number)
  {
    state->number = false;
    state->dot = false;
    parseAppend(state, " </mn>\n", -1);
    --state->indent;
  }

  parseAppend(state, input, length);
}

static void endElement(OutputState* state, const char* input, int length)
{
  assert(state);

  if (state->number)
  {
    state->number = false;
    state->dot = false;
    parseAppend(state, " </mn>\n", -1);
    --state->indent;
  }

  --state->indent;
  parseAppend(state, input, length);
}

// forward declarations
static bool parseTerminalNode(voEngine engine, voMathTerminalNode root, OutputState* state);
static bool parseNonTerminalNode(voEngine engine, voMathNonTerminalNode root, OutputState* state);
static bool parseRuleNode(voEngine engine, voMathRuleNode root, OutputState* state);
static bool getChildRule(voEngine engine, voMathNode node, const char* name, voMathRuleNode* ruleNode);
static bool getChildTerminal(voEngine engine, voMathNode node, const char* name, voMathTerminalNode* terminalNode);

static bool parseTreeToMathML_(voEngine engine, voMathNode root, OutputState* state)
{
  voTypeId type;
  type = voGetType(engine, root);

  if (type == VO_MathTerminalNode)
  {
    return parseTerminalNode(engine, (voMathTerminalNode)root, state);
  }
  else if (voIsCompatible(engine, type, VO_MathNonTerminalNode))
  {
    return parseNonTerminalNode(engine, (voMathNonTerminalNode)root, state);
  }
  else if (voIsCompatible(engine, type, VO_MathRuleNode))
  {
    return parseRuleNode(engine, (voMathRuleNode)root, state);
  }
  else
  {
    PARSETREETOMATHML_ERROR("unknown node type");
    return false;
  }
}

// format input to MathML
static bool toMathML(OutputState* state, const int32_t* codePoints, int codeCount)
{
  // maximum code point value is U+10FFFF
  char buffer[PARSETREETOMATHML_BUFFER_SIZE * sizeof("&#x10FFFF;")];
  int length = 0;
  int i;
  int j;

  assert(codeCount > 1);
  assert(codeCount <= PARSETREETOMATHML_BUFFER_SIZE);
  assert(codePoints[0] == 0xFEFF); // U+FEFF is Unicode BOM

  for (i = 1; i < codeCount; ++i)
  {
    int codePoint = codePoints[i];

    if (codePoint == '&') // escaped HTML entities
    {
      length+= sprintf(&buffer[length], "&amp;");
    }
    else if (codePoint == '<')
    {
      length+= sprintf(&buffer[length], "&lt;");
    }
    else if (codePoint == '>')
    {
      length+= sprintf(&buffer[length], "&gt;");
    }
    else if (codePoint >= ' ' && codePoint <= '~') // Unicode Basic Latin
    {
      buffer[length++] = (char)codePoint;
    }
    else // escaped Unicode entities
    {
      length += sprintf(&buffer[length], "&#x%04X;", codePoint);
    }
  }

  i = 0;
  j = 0;

  while (i < length)
  {
    char c = buffer[i];

    if (c >= '0' && c <= '9')
    {
      if (!state->number)
      {
        startElement(state, "<mn> ", -1);
        state->number = true;
      }
      parseAppend(state, &c, 1);
      ++i;
    }
    else if ((c == '.' || c == ',') && state->number && !state->dot)
    {
      parseAppend(state, &c, 1);
      state->dot = true;
      ++i;
    }
    else
    {
      // selected from
      // http://www.w3.org/TR/2010/REC-MathML3-20101021/appendixc.html
      //
      // feel free to report any issue (missing symbol?, new needs?)
      const char* operators[][2] = {
        { "(", NULL}, // (
        { ")", NULL}, // )
        { "[", NULL}, // [
        { "]", NULL}, // ]
        { "{", NULL}, // {
        { "|", NULL}, // |
        { " }", NULL}, // }
        { ";", NULL}, // ;
        { ",", NULL}, // ,
        { ":", NULL}, // :
        { "&#x2026;", "horizontal ellipsis" }, // …
        { "&#x220B;", "contains as member" }, // ∋
        { "&#x2227;", "logical and" }, // ∧
        { "&#x2228;", "logical or" }, // ∨
        { "&#x2200;", "for all" }, // ∀
        { "&#x2203;", "there exists" }, // ∃
        { "&#x2204;", "there does not exist" }, // ∄
        { "&#x2208;", "element of" }, // ∈
        { "&#x2209;", "not an element of" }, // ∉
        { "&#x220C;", "does not contain as member" }, // ∌
        { "&#x2282;", "subset of" }, // ⊂
        { "&#x2283;", "superset of" }, // ⊃
        { "&#x2284;", "not a subset of" }, // ⊄
        { "&#x2285;", "not a superset of" }, // ⊅
        { "&#x2286;", "subset of or equal to" }, // ⊆
        { "&#x2287;", "superset of or equal to" }, // ⊇
        { "&#x2288;", "neither a subset of nor equal to" }, // ⊈
        { "&#x2289;", "neither a superset of nor equal to" }, // ⊉
        { "&#x2264;", "less-than or equal to" }, // ≤
        { "&#x2265;", "greater-than or equal to" }, // ≥
        { "&gt;", NULL}, // >
        { "&lt;", NULL}, // <
        { "&#x223C;", "tilde operator" }, // ∼
        { "&#x2262;", "not identical to" }, // ≢
        { "&#x2260;", "not equal to" }, // ≠
        { "=", NULL}, // =
        { "&#x221D;", "proportional to" }, // ∝
        { "&#x2225;", "parallel to" }, // ∥
        { "&#x2243;", "asymptotically equal to" }, // ≃
        { "&#x2261;", "identical to" }, // ≡
        { "&#x226A;", "much less-than" }, // ≪
        { "&#x226B;", "much greater-than" }, // ≫
        { "&#x2218;", "ring operator" }, // ∘
        { "&#x2190;", "leftwards arrow" }, // ←
        { "&#x2191;", "upwards arrow" }, // ↑
        { "&#x2192;", "rightwards arrow" }, // →
        { "&#x2193;", "downwards arrow" }, // ↓
        { "&#x2194;", "left right arrow" }, // ↔
        { "&#x2195;", "up down arrow" }, // ↕
        { "&#x2196;", "north west arrow" }, // ↖
        { "&#x2197;", "north east arrow" }, // ↗
        { "&#x2198;", "south east arrow" }, // ↘
        { "&#x2199;", "south west arrow" }, // ↙
        { "&#x21D0;", "leftwards double arrow" }, // ⇐
        { "&#x21D1;", "upwards double arrow" }, // ⇑
        { "&#x21D2;", "rightwards double arrow" }, // ⇒
        { "&#x21D3;", "downwards double arrow" }, // ⇓
        { "&#x21D4;", "left right double arrow" }, // ⇔
        { "&#x21D5;", "up down double arrow" }, // ⇕
        { "+", NULL}, // +
        { "-", NULL}, // -
        { "&#x00B1;", "plus-minus sign" }, // ±
        { "&#x2210;", "n-ary coproduct" }, // ∐
        { "&#x2211;", "n-ary summation" }, // ∑
        { "&#x222B;", "integral" }, // ∫
        { "&#x222C;", "double integral" }, // ∬
        { "&#x222D;", "triple integral" }, // ∭
        { "&#x222E;", "contour integral" }, // ∮
        { "&#x222F;", "surface integral" }, // ∯
        { "&#x2230;", "volume integral" }, // ∰
        { "&#x220F;", "n-ary product" }, // ∏
        { "&#x2229;", "intersection" }, // ∩
        { "&#x222A;", "union" }, // ∪
        { "*", NULL}, // *
        { ".", NULL}, // .
        { "&#x00D7;", "multiplication sign" }, // ×
        { "&#x00B7;", "middle dot" }, // ·
        { "%", NULL}, // %
        { "\\", NULL}, // \ (/!\ beware line continuation)
        { "/", NULL}, // /
        { "&#x00F7;", "division sign" }, // ÷
        { "&#x2220;", "angle" }, // ∠
        { "&#x2202;", "partial differential" }, // ∂
        { "&#x2207;", "nabla" }, // ∇
        { "!", NULL}, // !
        { "@", NULL}, // @b
        { "?", NULL}, // ?
        { "&#x221A;", "square root" }, // √
        { "&amp;", NULL}, // &
        { "'", NULL}, // '
        { "_", NULL}, // _
        { "&#x2295;", "circled plus" }, // ⊕
        { "&#x2296;", "circled minus" }, // ⊖
        { "&#x2297;", "circled times" }, // ⊗
        { "&#x2299;", "circled dot operator" }, // ⊙
        { "&#x27E8;", "mathematical left angle bracket" }, // mathematical left angle bracket ⟨
        { "&#x27E9;", "mathematical right angle bracket" }, // mathematical right angle bracket ⟩
        { "&#x2252;", "approximately equal to or the image of" }, // ≒
        { "&#x2030;", "per mille sign" }, // ‰
        { "&#x22A5;", "up tack" }, // ⊥
        { "&#x2205;", "empty set" }, // ∅

        { "&#x00AC;", "not sign" }, // ¬
        { "&#x21BC;", "leftwards harpoon with barb upwards" }, // ↼
        { "&#x21C0;", "rightwards harpoon with barb upwards" }, // ⇀
        { "&#x21BD;", "leftwards harpoon with barb downwards" }, // ↽
        { "&#x21C1;", "rightwards harpoon with barb downwards" }, // ⇁
        { "&#x21CB;", "leftwards harpoon over rightwards harpoon" }, // ⇋
        { "&#x21CC;", "rightwards harpoon over leftwards harpoon" }, // ⇌
        { "&#x2110;", "n-ary coproduct" }, // ∐
        { "&#x2226;", "not parallel to" }, // ∦
        { "&#x2244;", "not asymptotically equal to" }, // ≄
        { "&#x2245;", "approximately equal to" }, // ≅
        { "&#x2246;", "approximately but not actually equal to" }, // ≆
        { "&#x2247;", "neither approximately nor actually equal to" }, // ≇
        { "&#x2248;", "almost equal to" }, // ≈
        { "&#x2249;", "not almost equal to" }, // ≉
        { "&#x224A;", "almost equal or equal to" }, // ≊
        { "&#x226E;", "not less-than" }, // ≮
        { "&#x226F;", "not greater-than" }, // ≯
        { "&#x2270;", "not less-than nor equal to" }, // ≰
        { "&#x2271;", "not greater-than nor equal to" }, // ≱
        { "&#x22C9;", "left normal factor semidirect product" }, // ⋉
        { "&#x22CA;", "right normal factor semidirect product" }, // ⋊
        { "&#x22EE;", "vertical ellipsis" }, // ⋮
        { "&#x2308;", "left ceiling" }, // ⌈
        { "&#x2309;", "right ceiling" }, // ⌉
        { "&#x230A;", "left floor" }, // ⌊
        { "&#x230B;", "right floor" }, // ⌋
        { "&#x25A1;", "white square" }, // □
        { "&#x25B3;", "white up-pointing triangle" }, // △
        { "&#x27E6;", "mathematical left white square bracket" }, // ⟦
        { "&#x27E7;", "mathematical right white square bracket" }, // ⟧
        { "&#x299E;", "angle with s inside" }, // ⦞
        { "&#x2AFB;", "triple solidus binary relation" }, // ⫻
        { "&#x22F0;", "up right diagonal ellipsis" }, // ⋰
        { "&#x22F1;", "down right diagonal ellipsis" }, // ⋱
        { "&#x2213;", "minor-or-plus sign" }, // ∓
        { "&#x2234;", "therefore" }, // ∴
        { "&#x2235;", "because" }, // ∵
        { "&#x2266;", "less-than over equal to" }, // ≦
        { "&#x2267;", "greater-than over equal to" }, // ≧
        { "&#x2312;", "arc" } // ⌒
      };

      const char* text[][2] = {
        // currency symbols
        { "&#x20AC;", "euro sign" }, // €
        { "&#x00A5;", "yen sign" }, // ¥
        { "&#x00A2;", "cent sign" }, // ¢
        { "&#x00A3;", "pound sign" }, // £
        { "&#x20A9;", "won sign"}, // ₩
        // miscellaneous symbols
        { "#", NULL}, // #
        { "$", NULL}, // $
        { "&#x00A9;", "copyright sign"}, // ©
        // math alphabets, http://www.w3.org/TR/xml-entity-names/isomopf.html
        { "&#x2102;", "double-struck capital C" }, // ℂ
        { "&#x2115;", "double-struck capital N" }, // ℕ
        { "&#x211A;", "double-struck capital Q" }, // ℚ
        { "&#x211D;", "double-struck capital R" }, // ℝ
        { "&#x2124;", "double-struck capital Z" }, // ℤ
        { "&#x1D53B;", "mathematical double-struck capital d" }, // 𝔻
        { "&#x2111;", "black-letter capital i" }, // ℑ
        { "&#x211C;", "black-letter capital r" }, // ℜ
        { "&#x2113;", "script small l" }, // ℓ
        { "&#x2131;", "script capital F" }, // ℱ
        { "&#x2112;", "script capital L" }, // ℒ
      };

      const char* identifiers[][2] = {
        { "&#x221E;", "infinity" }, // ∞
        { "&#x210F;", "planck constant over two pi" }, // ℏ
        // greek letters, http://www.w3.org/TR/xml-entity-names/isogrk3.html
        { "&#x03B1;", "greek small letter alpha" }, // α
        { "&#x03B2;", "greek small letter beta" }, // β
        { "&#x03B3;", "greek small letter gamma" }, // γ
        { "&#x03B4;", "greek small letter delta" }, // δ
        { "&#x03B5;", "greek small letter epsilon" }, // ε
        { "&#x03B7;", "greek small letter eta" }, // η
        { "&#x03B6;", "greek small letter zeta" }, // ζ
        { "&#x03B8;", "greek small letter theta" }, // θ
        { "&#x03B9;", "greek small letter iota" }, // ι
        { "&#x03BA;", "greek small letter kappa" }, // κ
        { "&#x03BB;", "greek small letter lambda" }, // λ
        { "&#x00B5;", "micro sign" }, // µ (micro sign)
        //{ "&#x03BC;", "greek small letter mu" }, // µ (not supported)
        { "&#x03BD;", "greek small letter nu" }, // ν
        { "&#x03BE;", "greek small letter xi" }, // ξ
        { "&#x03C0;", "greek small letter pi" }, // π
        { "&#x03C1;", "greek small letter rho" }, // ρ
        { "&#x03C3;", "greek small letter sigma" }, // σ
        { "&#x03C4;", "greek small letter tau" }, // τ
        { "&#x03C5;", "greek small letter upsilon" }, // υ
        { "&#x03D5;", "greek phi symbol" }, // ϕ
        { "&#x03C6;", "greek small letter phi" }, // φ
        { "&#x03C7;", "greek small letter chi" }, // χ
        { "&#x03C8;", "greek small letter psi " }, // ψ
        { "&#x03C9;", "greek small letter omega" }, // ω

        { "&#x0393;", "greek capital letter gamma" }, // Γ
        { "&#x0394;", "greek capital letter delta" }, // Δ
        { "&#x0398;", "greek capital letter theta" }, // Θ
        { "&#x039B;", "greek capital letter lambda" }, // Λ
        { "&#x039E;", "greek capital letter xi" }, // Ξ
        { "&#x03A0;", "greek capital letter pi" }, // Π
        { "&#x03A3;", "greek capital letter sigma" }, // Σ
        { "&#x03A5;", "greek capital letter upsilon" }, // Υ
        { "&#x03A6;", "greek capital letter phi" }, // Φ
        { "&#x03A9;", "greek capital letter omega" }, // Ω

        { "&#x03A8;", "greek capital leter psi" }, // Ψ
      };

      int k;
      int length0;
      int count = (int)(sizeof(operators) / sizeof(operators[0]));

      // iterate over the mappings table
      for (k = 0; k < count; ++k)
      {
        length0 = (int)strlen(operators[k][0]);

        if (length0 <= length - i)
        {
          if (strncmp(operators[k][0], &buffer[i], length0) == 0)
            break; // mapping found
        }
      }

      if (k < count) // if operator found
      {
        if (j > 0) // deferred output?
        {
          startElement(state, "<mi> ", -1);
          parseAppend(state, &buffer[i - j], j);
          endElement(state, " </mi>\n", -1);
        }

        startElement(state, "<mo> ", -1);
        parseAppend(state, operators[k][0], -1);

        if (operators[k][1] != NULL)
        {
          // output XML comment
          parseAppend(state, "<!--", -1);
          parseAppend(state, operators[k][1], -1);
          parseAppend(state, "-->", -1);
        }

        endElement(state, " </mo>\n", -1);

        i += length0; // advance input
        j = 0;        // reset deferred output

        continue;
      }

      count = (int)(sizeof(text) / sizeof(text[0]));

      // iterate over the mappings table
      for (k = 0; k < count; ++k)
      {
        length0 = (int)strlen(text[k][0]);

        if (length0 <= length - i)
        {
          if (strncmp(text[k][0], &buffer[i], length0) == 0)
            break; // mapping found
        }
      }

      if (k < count) // if text found
      {
        if (j > 0) // deferred output?
        {
          startElement(state, "<mi> ", -1);
          parseAppend(state, &buffer[i - j], j);
          endElement(state, " </mi>\n", -1);
        }

        startElement(state, "<mtext> ", -1);
        parseAppend(state, text[k][0], -1);

        if (text[k][1] != NULL)
        {
          // output XML comment
          parseAppend(state, "<!--", -1);
          parseAppend(state, text[k][1], -1);
          parseAppend(state, "-->", -1);
        }

        endElement(state, " </mtext>\n", -1);

        i += length0; // advance input
        j = 0;        // reset deferred output

        continue;
      }

      count = (int)(sizeof(identifiers) / sizeof(identifiers[0]));

      // iterate over the mappings table
      for (k = 0; k < count; ++k)
      {
        length0 = (int)strlen(identifiers[k][0]);

        if (length0 <= length - i)
        {
          if (strncmp(identifiers[k][0], &buffer[i], length0) == 0)
            break; // mapping found
        }
      }

      if (k < count) // if identifier found
      {
        if (j > 0) // deferred output?
        {
          startElement(state, "<mi> ", -1);
          parseAppend(state, &buffer[i - j], j);
          endElement(state, " </mi>\n", -1);
        }

        startElement(state, "<mi> ", -1);
        parseAppend(state, identifiers[k][0], -1);

        if (identifiers[k][1] != NULL)
        {
          // output XML comment
          parseAppend(state, "<!--", -1);
          parseAppend(state, identifiers[k][1], -1);
          parseAppend(state, "-->", -1);
        }

        endElement(state, " </mi>\n", -1);

        i += length0; // advance input
        j = 0;        // reset deferred output

        continue;
      }

      // identifier or number
      ++i;  // advance input position
      ++j;  // defer output
    }
  }

  if (j > 0) // deferred output?
  {
    startElement(state, "<mi> ", -1);
    parseAppend(state, &buffer[i - j], j);
    endElement(state, " </mi>\n", -1);
  }

  return true;
}

static int selectedTerminalAsUTF8(voEngine engine, voMathTerminalNode node, char* destination, int capacity)
{
  voCharset charset;
  voString label = { destination, capacity };
  int32_t index;
  bool successful;

  assert(destination != NULL);
  assert(capacity > 0);
  assert(voGetType(engine, node) == VO_MathTerminalNode);

  index = VO_IFACE(engine, IMathTerminalNode)->getSelectedCandidateIndex(engine, node);
  if (index == -1)
    return -1;

  charset = voCreateCharset(engine, "UTF-8");
  assert(charset);

  successful = VO_IFACE(engine, IMathTerminalNode)->getCandidateLabelAt(engine, node, index, charset, &label);
  assert((int)label.byteCount <= capacity);
  voReleaseObject(engine, charset);

  if (!successful)
    return -1;

  return (int)label.byteCount;
}

static bool parseTerminalNode(voEngine engine, voMathTerminalNode node, OutputState* state)
{
  voCharset charset;
  int32_t buffer[PARSETREETOMATHML_BUFFER_SIZE];
  voString label = { (char*)buffer, sizeof(buffer) };
  int32_t index;
  bool successful;

  assert(voGetType(engine, node) == VO_MathTerminalNode);

  index = VO_IFACE(engine, IMathTerminalNode)->getSelectedCandidateIndex(engine, node);
  if (index == -1)
    return false;

  charset = voCreateCharset(engine, "UTF-32");
  assert(charset);

  successful = VO_IFACE(engine, IMathTerminalNode)->getCandidateLabelAt(engine, node, index, charset, &label);
  assert(label.byteCount <= sizeof(buffer));
  voReleaseObject(engine, charset);

  if (!successful)
    return false;

  return toMathML(state, (int32_t*)label.bytes, (int)(label.byteCount / sizeof(int32_t)));
}

static bool parseNonTerminalNode(voEngine engine, voMathNonTerminalNode node, OutputState* state)
{
  int32_t index;
  voMathNode candidate;
  bool successful;

  assert(voIsCompatible(engine, voGetType(engine, node), VO_MathNonTerminalNode));

  index = VO_IFACE(engine, IMathNonTerminalNode)->getSelectedCandidateIndex(engine, node);
  if (index == -1)
    return false;

  candidate = VO_IFACE(engine, IMathNonTerminalNode)->getCandidateAt(engine, node, index);
  if (candidate == NULL)
    return false;

  successful = parseTreeToMathML_(engine, candidate, state);
  voReleaseObject(engine, candidate);

  return successful;
}

static bool parseIdentityRule(voEngine engine, voMathRuleNode node, OutputState* state)
{
  voMathNode child;
  bool successful;

  assert(voGetType(engine, node) == VO_MathRuleNode);
  assert(VO_IFACE(engine, IMathRuleNode)->getChildCount(engine, node) == 1);

  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 0);
  if (child == NULL)
    return false;

  successful = parseTreeToMathML_(engine, child, state);
  voReleaseObject(engine, child);

  return successful;
}

static bool parseHorizontalPairRule(voEngine engine, voMathRuleNode node, OutputState* state)
{
  voMathNode child;
  bool successful;

  assert(voGetType(engine, node) == VO_MathRuleNode);
  assert(VO_IFACE(engine, IMathRuleNode)->getChildCount(engine, node) == 2);

  // left symbol
  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 0);
  if (child == NULL)
    return false;

  successful = parseTreeToMathML_(engine, child, state);
  voReleaseObject(engine, child);

  if (!successful)
    return false;

  // right symbol
  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 1);
  if (child == NULL)
    return false;

  successful = parseTreeToMathML_(engine, child, state);
  voReleaseObject(engine, child);

  return successful;
}

static bool parseFenceRule(voEngine engine, voMathRuleNode node, OutputState* state)
{
  voMathNode child;
  voMathNode ruleNode;
  char buffer[PARSETREETOMATHML_BUFFER_SIZE];
  int length;
  bool successful;

  assert(voGetType(engine, node) == VO_MathRuleNode);
  assert(VO_IFACE(engine, IMathRuleNode)->getChildCount(engine, node) == 3);

  // left symbol
  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 1);
  if (child == NULL)
    return false;

  length = selectedTerminalAsUTF8(engine, child, buffer, sizeof(buffer));
  assert(length <= (int)sizeof(buffer));
  voReleaseObject(engine, child);

  if (length == -1)
    return false;

  if (strncmp(buffer, "(", length) != 0)
  {
    startElement(state, "<mfenced open=\"", -1);
    parseAppend(state, buffer, length);
    parseAppend(state, "\"", -1);
  }
  else
    startElement(state, "<mfenced", -1);

  // right symbol
  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 2);
  if (child == NULL)
    return false;
  length = selectedTerminalAsUTF8(engine, child, buffer, sizeof(buffer));
  assert(length <= (int)sizeof(buffer));
  voReleaseObject(engine, child);

  if (length == -1)
    return false;

  if (strncmp(buffer, ")", length) != 0)
  {
    parseAppend(state, " close=\"", -1);
    parseAppend(state, buffer, length);
    parseAppend(state, "\">\n", -1);
  }
  else
    parseAppend(state, ">\n", -1);

  startElement(state, "<mrow>\n", -1);

  // main expression
  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 0);
  if (child == NULL)
    return false;

  successful = getChildRule(engine, child, "vertical pair", &ruleNode);
  if (!successful)
  {
    voReleaseObject(engine, child);
    return false;
  }

  if (ruleNode != NULL)
  {
    startElement(state, "<mtable>\n", -1);
    successful = successful && parseTreeToMathML_(engine, child, state);
    endElement(state, "</mtable>\n", -1);

    voReleaseObject(engine, ruleNode);
  }
  else
    successful = successful && parseTreeToMathML_(engine, child, state);

  voReleaseObject(engine, child);

  if (!successful)
    return false;

  endElement(state, "</mrow>\n", -1);
  endElement(state, "</mfenced>\n", -1);

  return true;
}

static bool parseFractionRule(voEngine engine, voMathRuleNode node, OutputState* state)
{
  voMathNode child;
  bool successful;

  assert(voGetType(engine, node) == VO_MathRuleNode);
  assert(VO_IFACE(engine, IMathRuleNode)->getChildCount(engine, node) == 3);

  // first child always corresponds to the fraction bar
  startElement(state, "<mfrac>\n", -1);
  startElement(state, "<mrow>\n", -1);

  // then comes the numerator
  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 1);
  if (child == NULL)
    return false;

  successful = parseTreeToMathML_(engine, child, state);
  voReleaseObject(engine, child);

  if (!successful)
    return false;

  endElement(state, "</mrow>\n", -1);
  startElement(state, "<mrow>\n", -1);

  // and the denominator
  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 2);
  if (child == NULL)
    return false;

  successful = parseTreeToMathML_(engine, child, state);
  voReleaseObject(engine, child);

  if (!successful)
    return false;

  endElement(state, "</mrow>\n", -1);
  endElement(state, "</mfrac>\n", -1);

  return true;
}

static bool parseSqrtRule(voEngine engine, voMathRuleNode node, OutputState* state)
{
  voMathNode child;
  bool successful;

  assert(voGetType(engine, node) == VO_MathRuleNode);
  assert(VO_IFACE(engine, IMathRuleNode)->getChildCount(engine, node) == 2);

  // first child always corresponds to the sqrt sign
  startElement(state, "<msqrt>\n", -1);

  // main expression
  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 1);
  if (child == NULL)
    return false;

  successful = parseTreeToMathML_(engine, child, state);
  voReleaseObject(engine, child);

  if (!successful)
    return false;

  endElement(state, "</msqrt>\n", -1);

  return true;
}

static bool parseSubscriptRule(voEngine engine, voMathRuleNode node, OutputState* state)
{
  voMathNode child;
  bool successful;

  assert(voGetType(engine, node) == VO_MathRuleNode);
  assert(VO_IFACE(engine, IMathRuleNode)->getChildCount(engine, node) == 2);

  startElement(state, "<msub>\n", -1);
  startElement(state, "<mrow>\n", -1);

  // base symbol
  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 0);
  if (child == NULL)
    return false;

  successful = parseTreeToMathML_(engine, child, state);
  voReleaseObject(engine, child);

  if (!successful)
    return false;

  endElement(state, "</mrow>\n", -1);
  startElement(state, "<mrow>\n", -1);

  // subscript symbol
  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 1);
  if (child == NULL)
    return false;

  successful = parseTreeToMathML_(engine, child, state);
  voReleaseObject(engine, child);

  if (!successful)
    return false;

  endElement(state, "</mrow>\n", -1);
  endElement(state, "</msub>\n", -1);

  return true;
}

static bool parseSuperscriptRule(voEngine engine, voMathRuleNode node, OutputState* state)
{
  voMathNode child;
  bool successful;

  assert(voGetType(engine, node) == VO_MathRuleNode);
  assert(VO_IFACE(engine, IMathRuleNode)->getChildCount(engine, node) == 2);

  startElement(state, "<msup>\n", -1);
  startElement(state, "<mrow>\n", -1);

  // base symbol
  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 0);
  if (child == NULL)
    return false;

  successful = parseTreeToMathML_(engine, child, state);
  voReleaseObject(engine, child);

  if (!successful)
    return false;

  endElement(state, "</mrow>\n", -1);
  startElement(state, "<mrow>\n", -1);

  // superscript symbol
  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 1);
  if (child == NULL)
    return false;

  successful = parseTreeToMathML_(engine, child, state);
  voReleaseObject(engine, child);

  if (!successful)
    return false;

  endElement(state, "</mrow>\n", -1);
  endElement(state, "</msup>\n", -1);

  return true;
}

static bool parseSubsuperscriptRule(voEngine engine, voMathRuleNode node, OutputState* state)
{
  voMathNode child;
  bool successful;

  assert(voGetType(engine, node) == VO_MathRuleNode);
  assert(VO_IFACE(engine, IMathRuleNode)->getChildCount(engine, node) == 3);

  startElement(state, "<msubsup>\n", -1);
  startElement(state, "<mrow>\n", -1);

  // base symbol
  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 0);
  if (child == NULL)
    return false;

  successful = parseTreeToMathML_(engine, child, state);
  voReleaseObject(engine, child);

  if (!successful)
    return false;

  endElement(state, "</mrow>\n", -1);
  startElement(state, "<mrow>\n", -1);

  // subscript symbol
  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 1);
  if (child == NULL)
    return false;

  successful = parseTreeToMathML_(engine, child, state);
  voReleaseObject(engine, child);

  if (!successful)
    return false;

  endElement(state, "</mrow>\n", -1);
  startElement(state, "<mrow>\n", -1);

  // superscript symbol
  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 2);
  if (child == NULL)
    return false;

  successful = parseTreeToMathML_(engine, child, state);
  voReleaseObject(engine, child);

  if (!successful)
    return false;

  endElement(state, "</mrow>\n", -1);
  endElement(state, "</msubsup>\n", -1);

  return true;
}

static bool parseUnderscriptRule(voEngine engine, voMathRuleNode node, OutputState* state)
{
  voMathNode child;
  bool successful;

  assert(voGetType(engine, node) == VO_MathRuleNode);
  assert(VO_IFACE(engine, IMathRuleNode)->getChildCount(engine, node) == 2);

  startElement(state, "<munder>\n", -1);
  startElement(state, "<mrow>\n", -1);

  // base symbol
  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 0);
  if (voGetError(engine) != VO_NO_ERROR)
    return false;

  if (child != NULL)
  {
    successful = parseTreeToMathML_(engine, child, state);
    voReleaseObject(engine, child);

    if (!successful)
      return false;
  }

  endElement(state, "</mrow>\n", -1);
  startElement(state, "<mrow>\n", -1);

  // bottom symbol
  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 1);
  if (child == NULL)
    return false;

  successful = parseTreeToMathML_(engine, child, state);
  voReleaseObject(engine, child);

  if (!successful)
    return false;

  endElement(state, "</mrow>\n", -1);
  endElement(state, "</munder>\n", -1);

  return true;
}

static bool parseOverscriptRule(voEngine engine, voMathRuleNode node, OutputState* state)
{
  voMathNode child;
  char buffer[PARSETREETOMATHML_BUFFER_SIZE];
  OutputState state_ = { buffer, sizeof(buffer) };
  int length;
  bool successful;

  assert(voGetType(engine, node) == VO_MathRuleNode);
  assert(VO_IFACE(engine, IMathRuleNode)->getChildCount(engine, node) == 2);

  startElement(state, "<mover>\n", -1);
  startElement(state, "<mrow>\n", -1);

  // base symbol
  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 0);
  if (voGetError(engine) != VO_NO_ERROR)
    return false;

  if (child != NULL)
  {
    successful = parseTreeToMathML_(engine, child, state);
    voReleaseObject(engine, child);

    if (!successful)
      return false;

  }

  endElement(state, "</mrow>\n", -1);
  startElement(state, "<mrow>\n", -1);

  // top symbol
  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 1);
  if (child == NULL)
    return false;

  successful = parseTreeToMathML_(engine, child, &state_);
  length = state_.offset;

  if (!successful)
  {
    voReleaseObject(engine, child);
    return false;
  }

  if (strncmp(buffer, "<mo> - </mo>\n", length) == 0)
    parseAppend(state, "<mo> &#x203E;<!--overline--> </mo>\n", -1);
  else if (strncmp(buffer, "<mo> &#x2227;<!--logical and--> </mo>\n", length) == 0)
    parseAppend(state, "<mo> &#x005E;<!--circumflex accent--> </mo>\n", -1);
  else
    successful = parseTreeToMathML_(engine, child, state); // we parse again to update state

  voReleaseObject(engine, child);

  if (!successful)
    return false;

  endElement(state, "</mrow>\n", -1);
  endElement(state, "</mover>\n", -1);

  return true;
}

static bool parseUnderoverscriptRule(voEngine engine, voMathRuleNode node, OutputState* state)
{
  voMathNode child;
  bool successful;

  assert(voGetType(engine, node) == VO_MathRuleNode);
  assert(VO_IFACE(engine, IMathRuleNode)->getChildCount(engine, node) == 3);

  startElement(state, "<munderover>\n", -1);
  startElement(state, "<mrow>\n", -1);

  // base symbol
  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 0);
  if (child == NULL)
    return false;

  successful = parseTreeToMathML_(engine, child, state);
  voReleaseObject(engine, child);

  if (!successful)
    return false;

  endElement(state, "</mrow>\n", -1);
  startElement(state, "<mrow>\n", -1);

  // bottom symbol
  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 1);
  if (child == NULL)
    return false;

  successful = parseTreeToMathML_(engine, child, state);
  voReleaseObject(engine, child);

  if (!successful)
    return false;

  endElement(state, "</mrow>\n", -1);
  startElement(state, "<mrow>\n", -1);

  // top symbol
  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 2);
  if (child == NULL)
    return false;

  successful = parseTreeToMathML_(engine, child, state);
  voReleaseObject(engine, child);

  if (!successful)
    return false;

  endElement(state, "</mrow>\n", -1);
  endElement(state, "</munderover>\n", -1);

  return true;
}

static bool parsePresuperscriptRule(voEngine engine, voMathRuleNode node, OutputState* state)
{
  voMathNode child;
  voMathNode ruleNode;
  bool successful;

  assert(voGetType(engine, node) == VO_MathRuleNode);
  assert(VO_IFACE(engine, IMathRuleNode)->getChildCount(engine, node) == 2);

  // base symbol
  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 0);
  if (child == NULL)
    return false;

  successful = getChildRule(engine, child, "sqrt", &ruleNode);
  voReleaseObject(engine, child);

  if (!successful)
    return false;

  if (ruleNode != NULL)
  {
    child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, ruleNode, 1);
    voReleaseObject(engine, ruleNode);

    if (child == NULL)
      return false;

    startElement(state, "<mroot>\n", -1);
    startElement(state, "<mrow>\n", -1);

    successful = parseTreeToMathML_(engine, child, state);
    voReleaseObject(engine, child);

    if (!successful)
      return false;

    endElement(state, "</mrow>\n", -1);
    startElement(state, "<mrow>\n", -1);

    child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 1);
    if (child == NULL)
      return false;

    successful = parseTreeToMathML_(engine, child, state);
    voReleaseObject(engine, child);

    if (!successful)
      return false;

    endElement(state, "</mrow>\n", -1);
    endElement(state, "</mroot>\n", -1);
  }
  else
  {
    startElement(state, "<mmultiscripts>\n", -1);
    startElement(state, "<mrow>\n", -1);

    child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 0);
    if (child == NULL)
      return false;

    successful = parseTreeToMathML_(engine, child, state);
    voReleaseObject(engine, child);

    if (!successful)
      return false;

    endElement(state, "</mrow>\n", -1);
    addElement(state, "<none/>\n", -1);
    addElement(state, "<none/>\n", -1);
    addElement(state, "<mprescripts/>\n", -1);
    addElement(state, "<none/>\n", -1);
    startElement(state, "<mrow>\n", -1);

    child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 1);
    if (child == NULL)
      return false;

    successful = parseTreeToMathML_(engine, child, state);
    voReleaseObject(engine, child);

    if (!successful)
      return false;

    endElement(state, "</mrow>\n", -1);
    endElement(state, "</mmultiscripts>\n", -1);
  }

  return true;
}

static bool parseVerticalPairRule(voEngine engine, voMathRuleNode node, OutputState* state)
{
  voMathNode child;
  voMathNode ruleNode;
  bool successful;

  assert(voGetType(engine, node) == VO_MathRuleNode);
  assert(VO_IFACE(engine, IMathRuleNode)->getChildCount(engine, node) == 2);

  // top symbol
  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 0);
  if (child == NULL)
    return false;

  successful = getChildRule(engine, child, "vertical pair", &ruleNode);
  if (!successful)
  {
    voReleaseObject(engine, child);
    return false;
  }

  if (ruleNode != NULL)
  {
    successful = parseTreeToMathML_(engine, child, state);
    voReleaseObject(engine, ruleNode);
  }
  else
  {
    startElement(state, "<mtr>\n", -1);
    startElement(state, "<mtd>\n", -1);
    successful = parseTreeToMathML_(engine, child, state);
    endElement(state, "</mtd>\n", -1);
    endElement(state, "</mtr>\n", -1);
  }

  voReleaseObject(engine, child);

  if (!successful)
    return false;

  // bottom symbol
  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 1);
  if (child == NULL)
    return false;

  successful = getChildRule(engine, child, "vertical pair", &ruleNode);
  if (!successful)
  {
    voReleaseObject(engine, child);
    return false;
  }

  if (ruleNode != NULL)
  {
    successful = parseTreeToMathML_(engine, child, state);
    voReleaseObject(engine, ruleNode);
  }
  else
  {
    startElement(state, "<mtr>\n", -1);
    startElement(state, "<mtd>\n", -1);
    successful = parseTreeToMathML_(engine, child, state);
    endElement(state, "</mtd>\n", -1);
    endElement(state, "</mtr>\n", -1);
  }

  voReleaseObject(engine, child);

  return successful;
}

static bool parseVerticalListRule(voEngine engine, voMathRuleNode node, OutputState* state)
{
  int32_t i, count;
  bool successful;

  assert(voGetType(engine, node) == VO_MathRuleNode);

  count = VO_IFACE(engine, IMathRuleNode)->getChildCount(engine, node);
  assert(count > 1);

  startElement(state, "<mtable columnalign=\"left\">\n", -1);

  for (i = 0; i < count; ++i)
  {
    voMathNode child;

    child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, i);
    if (child == NULL)
      return false;

    startElement(state, "<mtr>\n", -1);
    startElement(state, "<mtd>\n", -1);
    successful = parseTreeToMathML_(engine, child, state);
    endElement(state, "</mtd>\n", -1);
    endElement(state, "</mtr>\n", -1);

    voReleaseObject(engine, child);

    if (!successful)
      return false;
  }

  endElement(state, "</mtable>\n", -1);

  return successful;
}

static bool parseTableRule(voEngine engine, voMathRuleNode node, OutputState* state)
{
  voMathTableData tableData;
  int32_t childIndex, childCount;
  int32_t rowIndex, columnIndex;
  
  assert(voGetType(engine, node) == VO_MathTableNode);

  startElement(state, "<mtable>\n", -1);

  if (!VO_IFACE(engine, IMathTableNode)->getData(engine, node, &tableData))
    return false;

  childCount = (int32_t)VO_IFACE(engine, IMathRuleNode)->getChildCount(engine, node);
  if (childCount == -1)
    return false;

  childIndex = 0;
  for (rowIndex = 0; rowIndex < tableData.rowCount; ++rowIndex)
  {
    startElement(state, "<mtr>\n", -1);

    for (columnIndex = 0; columnIndex < tableData.columnCount; ++columnIndex)
    {
      if (childIndex < childCount)
      {
        voMathNode child;
        voMathCellData cellData;

        child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, childIndex);
        assert(voGetType(engine, child) == VO_MathCellNode);

        if (!VO_IFACE(engine, IMathCellNode)->getData(engine, child, &cellData))
        {
          voReleaseObject(engine, child);
          return false;
        }

        assert(cellData.columnStart == cellData.columnStop);
        assert(cellData.rowStart == cellData.rowStop);

        if (rowIndex == cellData.rowStart && columnIndex == cellData.columnStart)
        {
          startElement(state, "<mtd>\n", -1);
          if (!parseTreeToMathML_(engine, child, state))
          {
            voReleaseObject(engine, child);
            return false;
          }
          endElement(state, "</mtd>\n", -1);
          ++childIndex;
        }
        else
        {
          startElement(state, "<mtd>\n", -1);
          addElement(state, "<none/>\n", -1);
          endElement(state, "</mtd>\n", -1);
        }

        voReleaseObject(engine, child);
      }
      else
      {
        startElement(state, "<mtd>\n", -1);
        addElement(state, "<none/>\n", -1);
        endElement(state, "</mtd>\n", -1);
      }
    }

    endElement(state, "</mtr>\n", -1);
  }

  endElement(state, "</mtable>\n", -1);

  return true;
}

static bool parseLeftFenceRule(voEngine engine, voMathRuleNode node, OutputState* state)
{
  voMathNode child;
  voMathNode ruleNode;
  char buffer[PARSETREETOMATHML_BUFFER_SIZE];
  int length;
  bool successful;

  assert(voGetType(engine, node) == VO_MathRuleNode);
  assert(VO_IFACE(engine, IMathRuleNode)->getChildCount(engine, node) == 2);

  // left symbol
  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 1);
  if (child == NULL)
    return false;

  length = selectedTerminalAsUTF8(engine, child, buffer, sizeof(buffer));
  assert(length <= (int)sizeof(buffer));
  voReleaseObject(engine, child);

  if (length == -1)
    return false;

  if (strncmp(buffer, "(", length) != 0)
  {
    startElement(state, "<mfenced open=\"", -1);
    parseAppend(state, buffer, length);
    parseAppend(state, "\"", -1);
  }
  else
    startElement(state, "<mfenced", -1);

  parseAppend(state, " close=\"\">\n", -1);

  startElement(state, "<mrow>\n", -1);

  // main expression
  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 0);
  if (child == NULL)
    return false;

  successful = getChildRule(engine, child, "vertical pair", &ruleNode);
  if (!successful)
  {
    voReleaseObject(engine, child);
    return false;
  }

  if (ruleNode != NULL)
  {
    startElement(state, "<mtable columnalign=\"left\">\n", -1);
    successful = successful && parseTreeToMathML_(engine, child, state);
    endElement(state, "</mtable>\n", -1);

    voReleaseObject(engine, ruleNode);
  }
  else
    successful = successful && parseTreeToMathML_(engine, child, state);

  voReleaseObject(engine, child);

  endElement(state, "</mrow>\n", -1);
  endElement(state, "</mfenced>\n", -1);

  return successful;
}

static bool parsePresubscriptRule(voEngine engine, voMathRuleNode node, OutputState* state)
{
  voMathNode child;
  bool successful;

  assert(voGetType(engine, node) == VO_MathRuleNode);
  assert(VO_IFACE(engine, IMathRuleNode)->getChildCount(engine, node) == 2);

  // base symbol
  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 0);
  if (child == NULL)
    return false;

  startElement(state, "<mmultiscripts>\n", -1);
  startElement(state, "<mrow>\n", -1);

  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 0);
  if (child == NULL)
    return false;

  successful = parseTreeToMathML_(engine, child, state);
  voReleaseObject(engine, child);

  if (!successful)
    return false;

  endElement(state, "</mrow>\n", -1);
  addElement(state, "<none/>\n", -1);
  addElement(state, "<none/>\n", -1);
  addElement(state, "<mprescripts/>\n", -1);
  startElement(state, "<mrow>\n", -1);

  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 1);
  if (child == NULL)
    return false;

  successful = parseTreeToMathML_(engine, child, state);
  voReleaseObject(engine, child);

  endElement(state, "</mrow>\n", -1);
  addElement(state, "<none/>\n", -1);
  endElement(state, "</mmultiscripts>\n", -1);

  return successful;
}

static bool parseOverlapRule(voEngine engine, voMathRuleNode node, OutputState* state)
{
  voMathNode child;
  char buffer[PARSETREETOMATHML_BUFFER_SIZE];
  OutputState state_ = { buffer, sizeof(buffer) };
  int length;
  bool successful;

  assert(voGetType(engine, node) == VO_MathRuleNode);
  assert(VO_IFACE(engine, IMathRuleNode)->getChildCount(engine, node) == 2);

  // overlap symbol
  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 1);
  if (child == NULL)
    return false;

  successful = parseTreeToMathML_(engine, child, &state_);
  length = state_.offset;
  voReleaseObject(engine, child);

  if (!successful)
    return false;

  if (strncmp("<mo> \\ </mo>\n", buffer, length) == 0)
    startElement(state, "<menclose notation=\"updiagonalstrike\">\n", -1);
  else if (strncmp("<mo> / </mo>\n", buffer, length) == 0)
    startElement(state, "<menclose notation=\"downdiagonalstrike\">\n", -1);
  else if (strncmp("<mo> - </mo>\n", buffer, length) == 0)
    startElement(state, "<menclose notation=\"horizontalstrike\">\n", -1);
  else if (strncmp("<mo> &#x00D7;<!--multiplication sign--> </mo>\n", buffer, length) == 0)
    startElement(state, "<menclose notation=\"updiagonalstrike downdiagonalstrike\">\n", -1);
  else if (strncmp("<mi> X </mi>\n", buffer, length) == 0)
    startElement(state, "<menclose notation=\"updiagonalstrike downdiagonalstrike\">\n", -1);
  else
  {
    PARSETREETOMATHML_ERROR("unknown symbol");
    return false;
  }

  // base symbol
  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 0);
  if (child == NULL)
    return false;

  successful = parseTreeToMathML_(engine, child, state);
  voReleaseObject(engine, child);

  if (!successful)
    return false;

  endElement(state, "</menclose>\n", -1);

  return true;
}

static void addHorizontalLine(const voMathTableData* tableData, const voMathBorderData* borderData, int32_t columnStart, int32_t columnStop, bool* columnHasDecimalPoint, OutputState* state)
{
  if (borderData->start == columnStart && borderData->stop == columnStop)
  {
    addElement(state, "<msline/>\n", -1);
  }
  else
  {
    char buffer[PARSETREETOMATHML_BUFFER_SIZE];
    int length;
    int32_t columnIndex;
    int p, l;

    l = 0;
    for (columnIndex = borderData->start; columnIndex <= borderData->stop; ++columnIndex)
    {
      if (columnHasDecimalPoint != NULL && columnHasDecimalPoint[columnIndex])
        ++l;

      ++l;
    }

    p = 0;
    for (columnIndex = borderData->stop + 1; columnIndex <= columnStop; ++columnIndex)
    {
      if (columnHasDecimalPoint != NULL && columnHasDecimalPoint[columnIndex])
        ++p;

      ++p;
    }

    length = sprintf(buffer, "<msline position=\"%d\" length=\"%d\"/>\n", p, l);
    addElement(state, buffer, length);
  }
}

static int32_t getCarryNodes(voEngine engine, voMathNode node, voMathNode* carryNodes, int32_t* carryCount)
{
  int32_t count = 0;

  node = voAcquireObject(engine, node);
  for (;;)
  {
    voTypeId type;

    type = voGetType(engine, node);

    if (type == VO_MathTerminalNode)
    {
      carryNodes[*carryCount] = node;
      ++*carryCount;
      ++count;
      break;
    }
    else if (voIsCompatible(engine, type, VO_MathNonTerminalNode))
    {
      int32_t index;
      voMathNode candidate;

      index = VO_IFACE(engine, IMathNonTerminalNode)->getSelectedCandidateIndex(engine, node);
      if (index == -1)
      {
        count = -1;
        break;
      }

      candidate = VO_IFACE(engine, IMathNonTerminalNode)->getCandidateAt(engine, node, index);
      if (candidate == NULL)
      {
        count = -1;
        break;
      }

      voReleaseObject(engine, node);
      node = candidate;
    }
    else if (voIsCompatible(engine, type, VO_MathRuleNode))
    {
      voMathNode child;
      char buffer[PARSETREETOMATHML_BUFFER_SIZE];
      voString name = { buffer, sizeof(buffer) };
      voCharset charset;
      bool successful;

      charset = voCreateCharset(engine, "UTF-8");
      assert(charset);

      successful = VO_IFACE(engine, IMathRuleNode)->getName(engine, node, charset, &name);
      voReleaseObject(engine, charset);

      if (!successful)
      {
        count = -1;
        break;
      }

      if (strncmp(name.bytes, "identity", name.byteCount) == 0)
      {
        child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 0);
        if (child == NULL)
        {
          count = -1;
          break;
        }

        voReleaseObject(engine, node);
        node = child;
      }
      else if (strncmp(name.bytes, "vertical pair", name.byteCount) == 0)
      {
        child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 0);
        if (child == NULL)
        {
          count = -1;
          break;
        }

        count += getCarryNodes(engine, child, carryNodes, carryCount);
        voReleaseObject(engine, child);

        child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 1);
        if (child == NULL)
        {
          count = -1;
          break;
        }

        voReleaseObject(engine, node);
        node = child;
      }
      else
      {
        carryNodes[*carryCount] = node;
        ++*carryCount;
        ++count;
        break;
      }
    }
    else
    {
      PARSETREETOMATHML_ERROR("unknown node type");
      return false;
    }
  }

  if (count == -1)
  {
    int32_t i;

    voReleaseObject(engine, node);

    for (i = 0, count = *carryCount; i < count; ++i)
    {
      assert(carryNodes[i] != NULL);
      voReleaseObject(engine, carryNodes[i]);
    }

    return -1;
  }

  return count;
}

static bool parseColumnarRow(voEngine engine, voMathTableData* tableData, voMathCellNode* cells, voMathCellData* cellsData, int32_t cellCount, int32_t columnStart, int32_t columnStop, bool* columnHasDecimalPoint, OutputState* state)
{
  int32_t cellIndex;
  int carryLocations[PARSETREETOMATHML_COLUMN_BUFFER_SIZE];
  voMathNode baseNodes[PARSETREETOMATHML_COLUMN_BUFFER_SIZE] = { 0 };
  voMathTerminalNode decimalSeparatorNodes[PARSETREETOMATHML_COLUMN_BUFFER_SIZE] = { 0 };
  voMathNode carryNodes[PARSETREETOMATHML_CARRY_BUFFER_SIZE] = { 0 };
  int32_t carryCounts[PARSETREETOMATHML_COLUMN_BUFFER_SIZE];
  int32_t carryIndex, carryCount, carryRowCount;
  int32_t columnIndex;
  bool successful = true;

  carryCount = carryRowCount = 0;
  for (cellIndex = 0; cellIndex < cellCount; ++cellIndex)
  {
    voMathNode node;

    carryLocations[cellIndex] = CARRY_LOCATION_NONE;
    carryCounts[cellIndex] = 0;
    baseNodes[cellIndex] = NULL;
    decimalSeparatorNodes[cellIndex] = NULL;

    node = voAcquireObject(engine, cells[cellIndex]);
    for (;;)
    {
      voTypeId type;

      type = voGetType(engine, node);

      if (type == VO_MathTerminalNode)
      {
        baseNodes[cellIndex] = node;
        break;
      }
      else if (voIsCompatible(engine, type, VO_MathNonTerminalNode))
      {
        int32_t index;
        voMathNode candidate;

        index = VO_IFACE(engine, IMathNonTerminalNode)->getSelectedCandidateIndex(engine, node);
        if (index == -1)
        {
          successful = false;
          goto cleanup;
        }

        candidate = VO_IFACE(engine, IMathNonTerminalNode)->getCandidateAt(engine, node, index);
        if (candidate == NULL)
        {
          successful = false;
          goto cleanup;
        }

        voReleaseObject(engine, node);
        node = candidate;
      }
      else if (voIsCompatible(engine, type, VO_MathRuleNode))
      {
        voMathNode child;
        char buffer[PARSETREETOMATHML_BUFFER_SIZE];
        voString name = { buffer, sizeof(buffer) };
        voCharset charset;
        int32_t count;

        charset = voCreateCharset(engine, "UTF-8");
        assert(charset);

        successful = VO_IFACE(engine, IMathRuleNode)->getName(engine, node, charset, &name);
        voReleaseObject(engine, charset);

        if (!successful)
        {
          successful = false;
          goto cleanup;
        }

        if (strncmp(name.bytes, "identity", name.byteCount) == 0)
        {
          child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 0);
          if (child == NULL)
          {
            successful = false;
            goto cleanup;
          }

          voReleaseObject(engine, node);
          node = child;
        }
        else if (strncmp(name.bytes, "horizontal pair", name.byteCount) == 0)
        {
          child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 1);
          if (child == NULL)
          {
            successful = false;
            goto cleanup;
          }

          assert(voGetType(engine, child) == VO_MathTerminalNode);
          decimalSeparatorNodes[cellIndex] = child;

          child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 0);
          if (child == NULL)
          {
            successful = false;
            goto cleanup;
          }

          voReleaseObject(engine, node);
          node = child;
        }
        else
        {
          if (strncmp(name.bytes, "presuperscript", name.byteCount) == 0)
            carryLocations[cellIndex] = CARRY_LOCATION_NW;
          else if (strncmp(name.bytes, "overscript", name.byteCount) == 0)
            carryLocations[cellIndex] = CARRY_LOCATION_N;
          else if (strncmp(name.bytes, "superscript", name.byteCount) == 0)
            carryLocations[cellIndex] = CARRY_LOCATION_NE;
          else if (strncmp(name.bytes, "subscript", name.byteCount) == 0)
            carryLocations[cellIndex] = CARRY_LOCATION_SE;
          else if (strncmp(name.bytes, "underscript", name.byteCount) == 0)
            carryLocations[cellIndex] = CARRY_LOCATION_S;
          else if (strncmp(name.bytes, "presubscript", name.byteCount) == 0)
            carryLocations[cellIndex] = CARRY_LOCATION_SW;
          else
            baseNodes[cellIndex] = voAcquireObject(engine, node);

          if (carryLocations[cellIndex] != CARRY_LOCATION_NONE)
          {
            child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 0);
            if (voGetError(engine) != VO_NO_ERROR)
            {
              successful = false;
              goto cleanup;
            }

            baseNodes[cellIndex] = child;

            child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 1);
            if (child == NULL)
            {
              successful = false;
              goto cleanup;
            }

            count = getCarryNodes(engine, child, carryNodes, &carryCount);
            if (count == -1)
            {
              successful = false;
              goto cleanup;
            }

            carryCounts[cellIndex] = count;
            carryRowCount = carryRowCount > count ? carryRowCount : count;
          }

          voReleaseObject(engine, node);
          break;
        }
      }
      else
      {
        PARSETREETOMATHML_ERROR("unknown node type");
        successful = false;
        goto cleanup;
      }
    }
  }

  if (carryCount != 0)
  {
    int32_t carryBaseIndex, carryRowIndex;

    for (carryRowIndex = 0; carryRowIndex < carryRowCount; ++carryRowIndex)
    {
      int carryLocation;

      for (cellIndex = 0; cellIndex < cellCount; ++cellIndex)
      {
        carryLocation = carryLocations[cellIndex];

        if (carryLocation != CARRY_LOCATION_NONE)
          break;
      }
      assert(carryLocation != CARRY_LOCATION_NONE);

      switch (carryLocation)
      {
        case CARRY_LOCATION_NW:
          startElement(state, "<mscarries location=\"nw\">\n", -1);
          break;
        case CARRY_LOCATION_N:
          startElement(state, "<mscarries>\n", -1);
          break;
        case CARRY_LOCATION_NE:
          startElement(state, "<mscarries location=\"ne\">\n", -1);
          break;
        case CARRY_LOCATION_SE:
          startElement(state, "<mscarries location=\"se\">\n", -1);
          break;
        case CARRY_LOCATION_S:
          startElement(state, "<mscarries location=\"s\">\n", -1);
          break;
        case CARRY_LOCATION_SW:
          startElement(state, "<mscarries location=\"sw\">\n", -1);
          break;
        default:
          PARSETREETOMATHML_ERROR("unknown carry location");
          successful = false;
          goto cleanup;
      }

      carryBaseIndex = 0;
      for (cellIndex = 0; cellIndex < cellCount; ++cellIndex)
      {
        if (cellIndex > 0)
        {
          for (columnIndex = cellsData[cellIndex - 1].columnStop + 1; columnIndex < cellsData[cellIndex].columnStart; ++columnIndex)
          {
            addElement(state, "<none/>\n", -1);

            if (columnHasDecimalPoint != NULL && columnHasDecimalPoint[columnIndex])
              addElement(state, "<none/>\n", -1);
          }
        }

        if (carryCounts[cellIndex] >= (carryRowCount - carryRowIndex))
        {
          carryIndex = carryBaseIndex + carryRowIndex - (carryRowCount - carryCounts[cellIndex]);

          if (carryLocation != carryLocations[cellIndex])
          {
            switch (carryLocations[cellIndex])
            {
              case CARRY_LOCATION_NW:
                startElement(state, "<mscarry location=\"nw\">\n", -1);
                break;
              case CARRY_LOCATION_N:
                startElement(state, "<mscarry location=\"n\">\n", -1);
                break;
              case CARRY_LOCATION_NE:
                startElement(state, "<mscarry location=\"ne\">\n", -1);
                break;
              case CARRY_LOCATION_SE:
                startElement(state, "<mscarry location=\"se\">\n", -1);
                break;
              case CARRY_LOCATION_S:
                startElement(state, "<mscarry location=\"s\">\n", -1);
                break;
              case CARRY_LOCATION_SW:
                startElement(state, "<mscarry location=\"sw\">\n", -1);
                break;
              default:
                PARSETREETOMATHML_ERROR("unknown carry location");
                successful = false;
                goto cleanup;
            }
          }

          successful = parseTreeToMathML_(engine, carryNodes[carryIndex], state);
          if (!successful)
            goto cleanup;

          addElement(state, "", -1);

          if (carryLocation != carryLocations[cellIndex])
            endElement(state, "</mscarry>\n", -1);

          if (columnHasDecimalPoint != NULL && columnHasDecimalPoint[cellsData[cellIndex].columnStop])
            addElement(state, "<none/>\n", -1);
        }
        else
        {
          addElement(state, "<none/>\n", -1);

          if (columnHasDecimalPoint != NULL && columnHasDecimalPoint[cellsData[cellIndex].columnStop])
            addElement(state, "<none/>\n", -1);
        }

        carryBaseIndex += carryCounts[cellIndex];
      }

      for (columnIndex = cellsData[cellIndex - 1].columnStop + 1; columnIndex <= columnStop; ++columnIndex)
      {
        addElement(state, "<none/>\n", -1);

        if (columnHasDecimalPoint != NULL && columnHasDecimalPoint[columnIndex])
          addElement(state, "<none/>\n", -1);
      }

      endElement(state, "</mscarries>\n", -1);
    }
  }

  startElement(state, "<msrow>\n", -1);

  for (cellIndex = 0; cellIndex < cellCount; ++cellIndex)
  {
    if (cellIndex > 0)
    {
      for (columnIndex = cellsData[cellIndex - 1].columnStop + 1; columnIndex < cellsData[cellIndex].columnStart; ++columnIndex)
      {
        addElement(state, "<none/>\n", -1);

        if (columnHasDecimalPoint != NULL && columnHasDecimalPoint[columnIndex])
          addElement(state, "<none/>\n", -1);
      }
    }

    if (baseNodes[cellIndex] != NULL)
    {
      successful = parseTreeToMathML_(engine, baseNodes[cellIndex], state);
      if (!successful)
        goto cleanup;
    }
    else
      addElement(state, "<none/>\n", -1);

    if (decimalSeparatorNodes[cellIndex] != NULL)
    {
      successful = parseTreeToMathML_(engine, decimalSeparatorNodes[cellIndex], state);
      if (!successful)
        goto cleanup;
    }
    else if (columnHasDecimalPoint != NULL && columnHasDecimalPoint[cellsData[cellIndex].columnStop])
      addElement(state, "<none/>\n", -1);
  }

  for (columnIndex = cellsData[cellIndex - 1].columnStop + 1; columnIndex <= columnStop; ++columnIndex)
  {
    addElement(state, "<none/>\n", -1);

    if (columnHasDecimalPoint != NULL && columnHasDecimalPoint[columnIndex])
      addElement(state, "<none/>\n", -1);
  }

  endElement(state, "</msrow>\n", -1);

cleanup:
  for (cellIndex = 0; cellIndex < cellCount; ++cellIndex)
  {
    if (baseNodes[cellIndex] != NULL)
      voReleaseObject(engine, baseNodes[cellIndex]);

    if (decimalSeparatorNodes[cellIndex] != NULL)
      voReleaseObject(engine, decimalSeparatorNodes[cellIndex]);
  }

  for (carryIndex = 0; carryIndex < carryCount; ++carryIndex)
    voReleaseObject(engine, carryNodes[carryIndex]);

  return successful;
}

static bool parseColumnarRegion(voEngine engine, voMathTableNode node, voMathTableData* tableData, int32_t childCount, voMathBorderData* horizontalBorders, int32_t borderIndex, int32_t horizontalBorderCount, int32_t rowStart, int32_t rowStop, int32_t columnStart, int32_t columnStop, bool* columnHasDecimalPoint, bool noneIfEmpty, OutputState* state)
{
  voMathCellNode cells[PARSETREETOMATHML_ROW_BUFFER_SIZE];
  voMathCellData cellsData[PARSETREETOMATHML_ROW_BUFFER_SIZE];
  int32_t childIndex;
  int32_t rowIndex, cellIndex, cellCount;
  bool successful = true;

  rowIndex = rowStart;
  childIndex = cellCount = 0;
  while (rowIndex <= rowStop)
  {
    bool flush = false;

    while (childIndex < childCount && !flush)
    {
      voMathNode child;

      child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, childIndex);
      if (child == NULL)
      {
        successful = false;
        goto cleanup;
      }

      if (voGetType(engine, child) == VO_MathCellNode)
      {
        voMathCellData cellData;

        successful = VO_IFACE(engine, IMathCellNode)->getData(engine, child, &cellData);

        if (!successful)
        {
          voReleaseObject(engine, child);
          goto cleanup;
        }

        if (cellData.columnStart >= columnStart && cellData.columnStop <= columnStop)
        {
          if (cellData.rowStart == rowIndex)
          {
            assert(cellCount < PARSETREETOMATHML_ROW_BUFFER_SIZE);
            cells[cellCount] = voAcquireObject(engine, child);
            cellsData[cellCount] = cellData;
            ++cellCount;
          }
          else
            flush = cellCount != 0;
        }
      }

      if (!flush)
        ++childIndex;

      voReleaseObject(engine, child);
    }

    if (cellCount == 0)
    {
      if (noneIfEmpty)
      {
        startElement(state, "<msrow>\n", -1);
        addElement(state, "<none/>\n", -1);
        endElement(state, "</msrow>\n", -1);
      }
      break;
    }

    while (rowIndex <= cellsData[0].rowStart)
    {
      if (borderIndex < horizontalBorderCount && horizontalBorders[borderIndex].position == rowIndex)
      {
        addHorizontalLine(tableData, &horizontalBorders[borderIndex], columnStart, columnStop, columnHasDecimalPoint, state);
        ++borderIndex;
      }

      if (rowIndex < cellsData[0].rowStart)
      {
        ++rowIndex;
      }
      else
      {
        successful = parseColumnarRow(engine, tableData, cells, cellsData, cellCount, columnStart, columnStop, columnHasDecimalPoint, state);
        if (!successful)
          goto cleanup;

        for (cellIndex = 0; cellIndex < cellCount; ++cellIndex)
          voReleaseObject(engine, cells[cellIndex]);

        cellCount = 0;
        ++rowIndex;
        break;
      }
    }
  }

  while (borderIndex < horizontalBorderCount)
  {
    addHorizontalLine(tableData, &horizontalBorders[borderIndex], columnStart, columnStop, columnHasDecimalPoint, state);
    ++borderIndex;
  }

cleanup:
  for (cellIndex = 0; cellIndex < cellCount; ++cellIndex)
    voReleaseObject(engine, cells[cellIndex]);

  return successful;
}

static bool parseColumnarOperationRule(voEngine engine, voMathTableNode node, const char* name, OutputState* state)
{
  int divisionType;
  voMathTableData tableData;
  int32_t childIndex, childCount;
  bool columnHasDecimalPoint[PARSETREETOMATHML_COLUMN_BUFFER_SIZE] = { 0 };
  voMathBorderData horizontalBorders[PARSETREETOMATHML_COLUMN_BUFFER_SIZE];
  voMathBorderData verticalBorders[PARSETREETOMATHML_COLUMN_BUFFER_SIZE];
  int32_t borderIndex, horizontalBorderCount, verticalBorderCount;
  int32_t divisionSeparator1, divisionSeparator2, divisionColumnStop;
  bool searchDecimalSeparator;
  int32_t rowStart, rowStop, columnStart, columnStop;
  bool successful;

  assert(voGetType(engine, node) == VO_MathTableNode);

  if (strcmp(name, "columnar division english") == 0)
    divisionType = MATH_OPERATION_DIVISION_ENGLISH;
  else if (strcmp(name, "columnar division french") == 0)
    divisionType = MATH_OPERATION_DIVISION_FRENCH;
  else if (strcmp(name, "columnar division dutch") == 0)
    divisionType = MATH_OPERATION_DIVISION_DUTCH;
  else if (strcmp(name, "columnar division indian") == 0)
    divisionType = MATH_OPERATION_DIVISION_INDIAN;
  else if (strcmp(name, "columnar division german") == 0)
    divisionType = MATH_OPERATION_DIVISION_GERMAN;
  else
    divisionType = 0;

  successful = VO_IFACE(engine, IMathTableNode)->getData(engine, node, &tableData);
  if (!successful)
    return false;

  childCount = VO_IFACE(engine, IMathRuleNode)->getChildCount(engine, node);
  if (childCount == -1)
    return false;

  divisionSeparator1 = divisionSeparator2 = tableData.columnCount;
  divisionColumnStop = 0;
  horizontalBorderCount = verticalBorderCount = 0;
  for (childIndex = 0; childIndex < childCount; ++childIndex)
  {
    voMathNode child;

    child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, childIndex);
    if (child == NULL)
      return false;

    if (voGetType(engine, child) == VO_MathCellNode)
    {
      voMathCellData cellData;
      voMathRuleNode ruleNode;

      successful = VO_IFACE(engine, IMathCellNode)->getData(engine, child, &cellData);

      if (!successful)
      {
        voReleaseObject(engine, child);
        return false;
      }

      searchDecimalSeparator = true;
      if (divisionType != 0)
      {
        voMathTerminalNode terminalNode;

        if (divisionType == MATH_OPERATION_DIVISION_ENGLISH)
        {
          if (divisionSeparator1 == tableData.columnCount && cellData.rowStart == cellData.rowStop)
          {
            successful = getChildTerminal(engine, child, "right parenthesis", &terminalNode);

            if (terminalNode != NULL)
            {
              divisionSeparator1 = cellData.columnStop;
              voReleaseObject(engine, terminalNode);
            }
          }
        }
        else if (divisionType == MATH_OPERATION_DIVISION_FRENCH)
        {
          assert(verticalBorderCount == 1);

          searchDecimalSeparator = cellData.columnStop < verticalBorders[0].position;
        }
        else
        {
          const char* separator1;
          const char* separator2;

          if (divisionType == MATH_OPERATION_DIVISION_DUTCH)
          {
            separator1 = "solidus";
            separator2 = "reverse solidus";
          }
          else if (divisionType == MATH_OPERATION_DIVISION_INDIAN)
          {
            separator1 = "right parenthesis";
            separator2 = "left parenthesis";
          }
          else if (divisionType == MATH_OPERATION_DIVISION_GERMAN)
          {
            separator1 = "colon";
            separator2 = "equals sign";
          }
          else
          {
            PARSETREETOMATHML_ERROR("unknown division type");
            return false;
          }

          if (cellData.rowStart == cellData.rowStop && cellData.rowStart == 0)
          {
            if (divisionSeparator1 == tableData.columnCount)
            {
              successful = getChildTerminal(engine, child, separator1, &terminalNode);

              if (terminalNode != NULL)
              {
                divisionSeparator1 = cellData.columnStop;
                voReleaseObject(engine, terminalNode);
              }
            }

            if (divisionSeparator2 == tableData.columnCount)
            {
              successful = successful && getChildTerminal(engine, child, separator2, &terminalNode);

              if (terminalNode != NULL)
              {
                divisionSeparator2 = cellData.columnStop;
                voReleaseObject(engine, terminalNode);
              }
            }

            if (divisionType == MATH_OPERATION_DIVISION_DUTCH)
              searchDecimalSeparator = cellData.columnStart > divisionSeparator1 && cellData.columnStop < divisionSeparator2;
            else if (divisionType == MATH_OPERATION_DIVISION_INDIAN)
              searchDecimalSeparator = cellData.columnStart > divisionSeparator1 && cellData.columnStop < divisionSeparator2;
            else if (divisionType == MATH_OPERATION_DIVISION_GERMAN)
              searchDecimalSeparator = cellData.columnStop < divisionSeparator1;
            else
            {
              PARSETREETOMATHML_ERROR("unknown division type");
              return false;
            }
          }
        }

        if (!successful)
        {
          voReleaseObject(engine, child);
          return false;
        }
      }

      if (searchDecimalSeparator)
      {
        divisionColumnStop = cellData.columnStop > divisionColumnStop ? cellData.columnStop : divisionColumnStop;

        successful = getChildRule(engine, child, "horizontal pair", &ruleNode);
        if (!successful)
        {
          voReleaseObject(engine, child);
          return false;
        }

        if (ruleNode != NULL)
        {
          assert(cellData.columnStop < PARSETREETOMATHML_COLUMN_BUFFER_SIZE);
          columnHasDecimalPoint[cellData.columnStop] = true;

          voReleaseObject(engine, ruleNode);
        }
      }
    }
    else if (voGetType(engine, child) == VO_MathBorderNode)
    {
      voMathBorderData borderData;

      if (!VO_IFACE(engine, IMathBorderNode)->getData(engine, child, &borderData))
      {
        voReleaseObject(engine, child);
        return false;
      }

      if (borderData.type == MATH_BORDER_TYPE_HORIZONTAL)
      {
        assert(horizontalBorderCount < PARSETREETOMATHML_COLUMN_BUFFER_SIZE);
        horizontalBorders[horizontalBorderCount++] = borderData;
      }
      else
      {
        assert(verticalBorderCount < PARSETREETOMATHML_COLUMN_BUFFER_SIZE);
        verticalBorders[verticalBorderCount++] = borderData;
      }
    }

    voReleaseObject(engine, child);
  }

  if (divisionType != 0)
    startElement(state, "<mstyle decimalpoint=\"\">\n", -1);
  else
    startElement(state, "<mstack stackalign=\"right\">\n", -1);

  if (divisionType == MATH_OPERATION_DIVISION_ENGLISH)
  {
    bool hasDivisor = divisionSeparator1 > 0;
    bool hasResult = !(horizontalBorderCount > 0 && horizontalBorders[0].position == 0);

    startElement(state, "<mlongdiv longdivstyle=\"lefttop\" stackalign=\"right\">\n", -1);

    if (hasDivisor)
    {
      if (hasResult)
        successful = parseColumnarRegion(engine, node, &tableData, childCount, NULL, 0, 0, 1, 1, 0, divisionSeparator1 - 1, NULL, false, state);
      else
        successful = parseColumnarRegion(engine, node, &tableData, childCount, NULL, 0, 0, 0, 0, 0, divisionSeparator1 - 1, NULL, false, state);

      if (!successful)
        return false;
    }
    else
    {
      startElement(state, "<msrow>\n", -1);
      addElement(state, "<none/>\n", -1);
      endElement(state, "</msrow>\n", -1);
    }

    if (hasResult)
    {
      successful = parseColumnarRegion(engine, node, &tableData, childCount, NULL, 0, 0, 0, 0, 0, tableData.columnCount - 1, NULL, false, state);
      if (!successful)
        return false;
    }
    else
    {
      startElement(state, "<msrow>\n", -1);
      addElement(state, "<none/>\n", -1);
      endElement(state, "</msrow>\n", -1);
    }

    rowStart = hasResult ? 1 : 0;
    rowStop = tableData.rowCount - 1;
    columnStart = divisionSeparator1 + 1;
    columnStop = divisionColumnStop;
    borderIndex = 1;
  }
  else if (divisionType == MATH_OPERATION_DIVISION_FRENCH)
  {
    bool hasLhs = !(verticalBorderCount > 0 && verticalBorders[0].position == 0);

    startElement(state, "<mlongdiv longdivstyle=\"stackedrightright\" stackalign=\"right\">\n", -1);
    successful = parseColumnarRegion(engine, node, &tableData, childCount, NULL, 0, 0, 0, 0, verticalBorders[0].position, tableData.columnCount - 1, NULL, true, state);
    if (!successful)
      return false;

    successful = parseColumnarRegion(engine, node, &tableData, childCount, NULL, 0, 0, 1, 1, verticalBorders[0].position, tableData.columnCount - 1, NULL, true, state);
    if (!successful)
      return false;

    rowStart = 0;
    rowStop = tableData.rowCount - 1;
    columnStart = 0;
    columnStop = hasLhs ? divisionColumnStop : -1;
    borderIndex = 1;
  }
  else if (divisionType == MATH_OPERATION_DIVISION_DUTCH || divisionType == MATH_OPERATION_DIVISION_INDIAN)
  {
    int32_t columnIndex, groupPosition;
    char buffer[PARSETREETOMATHML_BUFFER_SIZE];
    int length;

    if (divisionType == MATH_OPERATION_DIVISION_DUTCH)
      startElement(state, "<mlongdiv longdivstyle=\"left/\\right\" stackalign=\"right\">\n", -1);
    else
      startElement(state, "<mlongdiv longdivstyle=\"left)(right\" stackalign=\"right\">\n", -1);

    successful = parseColumnarRegion(engine, node, &tableData, childCount, NULL, 0, 0, 0, 0, 0, divisionSeparator1 - 1, NULL, true,state);
    if (!successful)
      return false;

    successful = parseColumnarRegion(engine, node, &tableData, childCount, NULL, 0, 0, 0, 0, divisionSeparator2 + 1, tableData.columnCount - 1, NULL, true, state);
    if (!successful)
      return false;

    successful = parseColumnarRegion(engine, node, &tableData, childCount, NULL, 0, 0, 0, 0, divisionSeparator1 + 1, divisionSeparator2 - 1, columnHasDecimalPoint, true, state);
    if (!successful)
      return false;

    rowStart = 1;
    rowStop = tableData.rowCount - 1;
    columnStart = 0;
    columnStop = divisionColumnStop;
    borderIndex = 0;

    for (groupPosition = 0, columnIndex = divisionSeparator2; columnIndex <= divisionColumnStop; ++columnIndex)
    {
      if (columnHasDecimalPoint[columnIndex])
        --groupPosition;
      --groupPosition;
    }

    length = sprintf(buffer, "<msgroup position=\"%d\">\n", groupPosition);
    startElement(state, buffer, length);
  }
  else if (divisionType == MATH_OPERATION_DIVISION_GERMAN)
  {
    int32_t columnIndex, groupPosition;
    char buffer[PARSETREETOMATHML_BUFFER_SIZE];
    int length;

    startElement(state, "<mlongdiv longdivstyle=\":right=right\" stackalign=\"right\">\n", -1);

    successful = parseColumnarRegion(engine, node, &tableData, childCount, NULL, 0, 0, 0, 0, divisionSeparator1 + 1, divisionSeparator2 - 1, NULL, true, state);
    if (!successful)
      return false;

    successful = parseColumnarRegion(engine, node, &tableData, childCount, NULL, 0, 0, 0, 0, divisionSeparator2 + 1, tableData.columnCount - 1, NULL, true, state);
    if (!successful)
      return false;

    successful = parseColumnarRegion(engine, node, &tableData, childCount, NULL, 0, 0, 0, 0, 0, divisionSeparator1 - 1, columnHasDecimalPoint, true, state);
    if (!successful)
      return false;

    rowStart = 1;
    rowStop = tableData.rowCount - 1;
    columnStart = 0;
    columnStop = divisionColumnStop;
    borderIndex = 0;

    for (groupPosition = 0, columnIndex = divisionSeparator1; columnIndex <= divisionColumnStop; ++columnIndex)
    {
      if (columnHasDecimalPoint[columnIndex])
        --groupPosition;
      --groupPosition;
    }

    length = sprintf(buffer, "<msgroup position=\"%d\">\n", groupPosition);
    startElement(state, buffer, length);
  }
  else
  {
    rowStart = 0;
    rowStop = tableData.rowCount - 1;
    columnStart = 0;
    columnStop = tableData.columnCount - 1;
    borderIndex = 0;
  }

  successful = parseColumnarRegion(engine, node, &tableData, childCount, horizontalBorders, borderIndex, horizontalBorderCount, rowStart, rowStop, columnStart, columnStop, columnHasDecimalPoint, false, state);
  if (!successful)
    return false;

  if (divisionType == MATH_OPERATION_DIVISION_DUTCH || divisionType == MATH_OPERATION_DIVISION_INDIAN || divisionType == MATH_OPERATION_DIVISION_GERMAN)
    endElement(state, "</msgroup>\n", -1);

  if (divisionType != 0)
  {
    endElement(state, "</mlongdiv>\n", -1);
    endElement(state, "</mstyle>\n", -1);
  }
  else
    endElement(state, "</mstack>\n", -1);

  return successful;
}

static bool parseRuleNode(voEngine engine, voMathRuleNode node, OutputState* state)
{
  voCharset charset;
  char buffer[PARSETREETOMATHML_BUFFER_SIZE];
  voString name = { buffer, sizeof(buffer) };
  bool successful;

  assert(voIsCompatible(engine, voGetType(engine, node), VO_MathRuleNode));

  charset = voCreateCharset(engine, "UTF-8");
  assert(charset);

  successful = VO_IFACE(engine, IMathRuleNode)->getName(engine, node, charset, &name);
  voReleaseObject(engine, charset);
  buffer[name.byteCount] = 0;

  if (!successful)
    return false;

  if (strncmp(name.bytes, "identity", name.byteCount) == 0)
    return parseIdentityRule(engine, node, state);
  else if (strncmp(name.bytes, "horizontal pair", name.byteCount) == 0)
    return parseHorizontalPairRule(engine, node, state);
  else if (strncmp(name.bytes, "fence", name.byteCount) == 0)
    return parseFenceRule(engine, node, state);
  else if (strncmp(name.bytes, "fraction", name.byteCount) == 0)
    return parseFractionRule(engine, node, state);
  else if (strncmp(name.bytes, "sqrt", name.byteCount) == 0)
    return parseSqrtRule(engine, node, state);
  else if (strncmp(name.bytes, "subscript", name.byteCount) == 0)
    return parseSubscriptRule(engine, node, state);
  else if (strncmp(name.bytes, "superscript", name.byteCount) == 0)
    return parseSuperscriptRule(engine, node, state);
  else if (strncmp(name.bytes, "subsuperscript", name.byteCount) == 0)
    return parseSubsuperscriptRule(engine, node, state);
  else if (strncmp(name.bytes, "underscript", name.byteCount) == 0)
    return parseUnderscriptRule(engine, node, state);
  else if (strncmp(name.bytes, "overscript", name.byteCount) == 0)
    return parseOverscriptRule(engine, node, state);
  else if (strncmp(name.bytes, "underoverscript", name.byteCount) == 0)
    return parseUnderoverscriptRule(engine, node, state);
  else if (strncmp(name.bytes, "presuperscript", name.byteCount) == 0)
    return parsePresuperscriptRule(engine, node, state);
  else if (strncmp(name.bytes, "vertical pair", name.byteCount) == 0)
    return parseVerticalPairRule(engine, node, state);
  else if (strncmp(name.bytes, "left fence", name.byteCount) == 0)
    return parseLeftFenceRule(engine, node, state);
  else if (strncmp(name.bytes, "presubscript", name.byteCount) == 0)
    return parsePresubscriptRule(engine, node, state);
  else if (strncmp(name.bytes, "overlap", name.byteCount) == 0)
    return parseOverlapRule(engine, node, state);
  else if (strstr(name.bytes, "columnar") != NULL)
    return parseColumnarOperationRule(engine, (voMathTableNode)node, buffer, state);
  else if (strncmp(name.bytes, "vertical list", name.byteCount) == 0)
    return parseVerticalListRule(engine, node, state);
  else if (strncmp(name.bytes, "table", name.byteCount) == 0)
    return parseTableRule(engine, node, state);
  else
    PARSETREETOMATHML_ERROR("unknown rule");
  return false;
}

static bool getChildRule(voEngine engine, voMathNode node, const char* name, voMathRuleNode* ruleNode)
{
  voTypeId type;
  int32_t index;
  voMathNode candidate;
  voCharset charset;
  char buffer[PARSETREETOMATHML_BUFFER_SIZE];
  voString name_ = { buffer, sizeof(buffer) };
  bool successful;

  type = voGetType(engine, node);

  if(type == VO_MathTerminalNode)
  {
    *ruleNode = NULL;
    return true;
  }
  else if (voIsCompatible(engine, type, VO_MathNonTerminalNode))
  {
    index = VO_IFACE(engine, IMathNonTerminalNode)->getSelectedCandidateIndex(engine, node);
    if (index == -1)
      return false;

    candidate = VO_IFACE(engine, IMathNonTerminalNode)->getCandidateAt(engine, node, index);
    if (candidate == NULL)
      return false;

    successful = getChildRule(engine, candidate, name, ruleNode);
    voReleaseObject(engine, candidate);
    return successful;
  }
  else if (voIsCompatible(engine, type, VO_MathRuleNode))
  {
    charset = voCreateCharset(engine, "UTF-8");
    assert(charset);

    successful = VO_IFACE(engine, IMathRuleNode)->getName(engine, node, charset, &name_);
    voReleaseObject(engine, charset);

    if (!successful)
      return false;

    if (strncmp(name_.bytes, "identity", name_.byteCount) == 0)
    {
      voMathNode child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 0);
      if (child == NULL)
        return false;

      successful = getChildRule(engine, child, name, ruleNode);
      voReleaseObject(engine, child);
    }
    else
    {
      if (name != NULL)
      {
        if (strncmp(name_.bytes, name, name_.byteCount) == 0)
          *ruleNode = voAcquireObject(engine, node);
        else
          *ruleNode = NULL;
      }
      else
        *ruleNode = voAcquireObject(engine, node);
    }

    return successful;
  }
  else
  {
    PARSETREETOMATHML_ERROR("unknown node type");
    return false;
  };
}

static bool getChildTerminal(voEngine engine, voMathNode node, const char* name, voMathTerminalNode *terminalNode)
{
  voTypeId type;
  int32_t index;
  voMathNode candidate;
  voCharset charset;
  char buffer[PARSETREETOMATHML_BUFFER_SIZE];
  voString name_ = { buffer, sizeof(buffer) };
  bool successful;

  type = voGetType(engine, node);

  if(type == VO_MathTerminalNode)
  {
    if (name != NULL)
    {
      charset = voCreateCharset(engine, "UTF-8");
      assert(charset);

      successful = VO_IFACE(engine, IMathTerminalNode)->getName(engine, node, charset, &name_);
      voReleaseObject(engine, charset);

      if (!successful)
        return false;

      if (strncmp(name_.bytes, name, name_.byteCount) == 0)
        *terminalNode = voAcquireObject(engine, node);
      else
        *terminalNode = NULL;
    }
    else
      *terminalNode = voAcquireObject(engine, node);

    return true;
  }
  else if (voIsCompatible(engine, type, VO_MathNonTerminalNode))
  {
    index = VO_IFACE(engine, IMathNonTerminalNode)->getSelectedCandidateIndex(engine, node);
    if (index == -1)
      return false;

    candidate = VO_IFACE(engine, IMathNonTerminalNode)->getCandidateAt(engine, node, index);
    if (candidate == NULL)
      return false;

    successful = getChildTerminal(engine, candidate, name, terminalNode);
    voReleaseObject(engine, candidate);
    return successful;
  }
  else if (voIsCompatible(engine, type, VO_MathRuleNode))
  {
    charset = voCreateCharset(engine, "UTF-8");
    assert(charset);

    successful = VO_IFACE(engine, IMathRuleNode)->getName(engine, node, charset, &name_);
    voReleaseObject(engine, charset);

    if (!successful)
      return false;

    if (strncmp(name_.bytes, "identity", name_.byteCount) == 0)
    {
      voMathNode child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 0);
      if (child == NULL)
        return false;

      successful = getChildTerminal(engine, child, name, terminalNode);
      voReleaseObject(engine, child);
    }
    else
      *terminalNode = NULL;

    return successful;
  }
  else
  {
    PARSETREETOMATHML_ERROR("unknown node type");
    return false;
  };
}
