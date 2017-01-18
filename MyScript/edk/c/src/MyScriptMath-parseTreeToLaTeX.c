/**
 * @file MyScriptMath-parseTreeToLaTeX.c
 *
 * This file contains our sample implementation of the serialization of a parsed
 * math to a LaTeX string.
 *
 * This implementation is provided as-is, feel free to customize it to suit your
 * own needs.
 */

#include "MyScriptMath-parseTreeToLaTeX.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>

#define PARSETREETOLATEX_BUFFER_SIZE 256
#define PARSETREETOLATEX_ERROR(message)\
  fprintf(stderr, "parseTreeToLaTeX: error: %s\n", message);\
  fflush(stderr)

#define PARSETREETOLATEX_BORDER_BUFFER_SIZE 32

typedef struct _OutputState
{
  char* destination;        // destination buffer
  int capacity;             // destination buffer capacity
  int offset;               // where to write next in the destination buffer
  bool isColumnarOperation; // is parsing columnar operation
  bool isInsideCases;       // is inside cases

} OutputState;

// forward declarations
static bool parseTreeToLaTeX_(voEngine engine, voMathNode node, OutputState* state);

static void parseAppend(OutputState* state, const char* input, int length)
{
  length = length == -1 ? (int)strlen(input) : length;

  // append only when destination is non NULL
  if (state->destination != NULL)
  {
    int remaining = state->capacity - state->offset;

    if (remaining > 0)
    {
      int count = length < remaining ? length : remaining;
      char* output = state->destination + state->offset;

      while (count--)
        *output++ = *input++;
    }
  }

  state->offset += length;
}

int parseTreeToLaTeX(voEngine engine, voMathNode root, char* utf8Buffer, int capacity)
{
  OutputState state = { utf8Buffer, capacity, 0, false, false};
  voMathNode node;
  bool successful;

  // keep a user reference to the root node while processing
  node = voAcquireObject(engine, root);

  successful = parseTreeToLaTeX_(engine, node, &state);
  voReleaseObject(engine, node);

  if (!successful)
    return -1;

  return state.offset;
}

// forward declarations
static bool parseTerminalNode(voEngine engine, voMathTerminalNode node, OutputState* state);
static bool parseNonTerminalNode(voEngine engine, voMathNonTerminalNode node, OutputState* state);
static bool parseRuleNode(voEngine engine, voMathRuleNode node, OutputState* state);
static bool getChildRule(voEngine engine, voMathNode node, const char* name, voMathRuleNode* ruleNode);
static bool getChildRule3(voEngine engine, voMathNode node, const char* name1, const char* name2, const char* name3, voMathRuleNode* ruleNode);

static bool parseTreeToLaTeX_(voEngine engine, voMathNode root, OutputState* state)
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
    PARSETREETOLATEX_ERROR("unknown node type");
    return false;
  }
}

// map input string to LaTeX commands, eventually
static bool toLaTeX(OutputState* state, const char* input, int length)
{
  // selected from http://ctan.org/tex-archive/info/symbols/comprehensive/
  //
  // feel free to report any issue (missing symbol? new needs?)
  const char* mappings[][2] = {
    // LaTeX special characters
    { "#", "\\# " }, // #
    { "$", "\\$ " }, // $
    { "%", "\\% " }, // %
    { "&", "\\& " }, // &
    { "_", "\\_ " }, // _
    { "{", "\\{ " }, // {
    { "}", "\\} " }, // }
    //{ "^", "\\^{}" }, // ^ (not supported)
    //{ "~", "\\~{}" }, // ~ (not supported)
    { "\\", "\\backslash " }, // preferred to \textbackslash, see Table 201

    // binary operators, see Table 50
    { "*", "\\ast " }, // *
    { "\xE2\x88\xA9", "\\cap " }, // ∩
    { "\xC2\xB7", "\\cdot " }, // ·
    { "\xE2\x88\x98", "\\circ " }, // ∘
    { "\xE2\x88\xAA", "\\cup " }, // ∪
    { "\xC3\xB7", "\\div " }, // ÷
    { "\xC2\xB1", "\\pm " }, // ±
    { "\xC3\x97", "\\times " }, // ×
    { "\xE2\x88\xA7", "\\wedge " }, // ∧
    { "\xE2\x88\xA8", "\\vee " }, // ∨
    { "\xE2\x8A\x95", "\\oplus " }, // ⊕
    { "\xE2\x8A\x97", "\\otimes " }, // ⊗
    { "\xE2\x8A\x96", "\\ominus " }, // ⊖
    { "\xE2\x8A\x99", "\\odot " }, // ⊙
    { "\xE2\x8B\x89", "\\ltimes " }, // ⋉
    { "\xE2\x8B\x8A", "\\rtimes " }, // ⋊
    { "\xE2\x88\x93", "\\mp "}, // ∓

    // variable-sized math operators, see Table 71
    { "\xE2\x88\xAB", "\\int " }, // ∫
    { "\xE2\x88\xAE", "\\oint " }, // ∮
    { "\xE2\x88\x8F", "\\prod " }, // ∏
    { "\xE2\x88\x91", "\\sum " }, // ∑
    { "\xE2\x88\xAC", "\\iint " }, // ∬ (requires amsmath)
    { "\xE2\x88\xAD", "\\iiint " }, // ∭ (requires amsmath)
    //{ "\xE2\x88\xAF", "\\oiint " }, // ∯ (requires txfonts/pxfonts)
    //{ "\xE2\x88\xB0", "\\oiiint " }, // ∰ (requires txfonts/pxfonts)
    { "\xE2\x88\x90", "\\coprod " }, // ∐

    // binary relations, see Table 88
    { "\xE2\x89\xA1", "\\equiv " }, // ≡
    { "\xE2\x89\xA2", "\\not\\equiv " }, // ≢
    { "\xE2\x88\xBC", "\\sim " }, // ∼
    { "\xE2\x89\x83", "\\simeq " }, // ≃
    { "\xE2\x88\xA5", "\\parallel " }, // ∥
    { "\xE2\x88\x9D", "\\propto " }, // ∝
    { "\xE2\x89\x92", "\\fallingdotseq " }, // ≒ (requires amsmath)
    { "\xE2\x89\x84", "\\not\\simeq " }, // ≄
    { "\xE2\x89\x85", "\\cong " }, // ≅
    { "\xE2\x89\x87", "\\not\\cong " }, // ≇
    //{ "\xE2\x89\x87", "\\ncong " }, // ≇
    { "\xE2\x89\x88", "\\approx " }, // ≈
    { "\xE2\x89\x89", "\\not\\approx " }, // ≉
    //{ "\xE2\x89\x89", "\\napprox " }, // ≉
    { "\xE2\x89\x8A", "\\approxeq " }, // ≊
    { "\xE2\x88\xA6", "\\nparallel " }, // ∦
    { "\xE2\x88\xB4", "\\therefore " }, // ∴
    { "\xE2\x88\xB5", "\\because " }, // ∵

    // subset and superset relations, see Table 111
    { "\xE2\x8A\x86", "\\subseteq " }, // ⊆
    { "\xE2\x8A\x88", "\\not\\subseteq " }, // ⊈
    //{ "\xE2\x8A\x88", "\\nsubseteq " }, // ⊈
    { "\xE2\x8A\x87", "\\supseteq " }, // ⊇
    { "\xE2\x8A\x89", "\\not\\supseteq " }, // ⊉
    //{ "\xE2\x8A\x89", "\\nsupseteq " }, // ⊉
    { "\xE2\x8A\x82", "\\subset " }, // ⊂
    { "\xE2\x8A\x83", "\\supset " }, // ⊃
    { "\xE2\x8A\x84", "\\not\\subset " }, // ⊄
    //{ "\xE2\x8A\x84", "\\nsubset " }, // ⊄
    { "\xE2\x8A\x85", "\\not\\supset " }, // ⊅
    //{ "\xE2\x8A\x85", "\\nsupset " }, // ⊅

    // inequalities, see Table 121
    { "\xE2\x89\xA5", "\\geq " }, // ≥
    { "\xE2\x89\xAB", "\\gg " }, // ≫
    { "\xE2\x89\xA4", "\\leq " }, // ≤
    { "\xE2\x89\xAA", "\\ll " }, // ≪
    { "\xE2\x89\xA0", "\\neq " }, // ≠
    { "\xE2\x89\xAE", "\\nless " }, // ≮
    { "\xE2\x89\xAF", "\\ngtr " }, // ≯
    { "\xE2\x89\xB0", "\\nleq " }, // ≰
    { "\xE2\x89\xB1", "\\ngeq " }, // ≱
    { "\xE2\x89\xA6", "\\leqq " }, // ≦
    { "\xE2\x89\xA7", "\\geqq " }, // ≧

    // arrows, see Table 137
    { "\xE2\x86\x90", "\\leftarrow " }, // ←
    { "\xE2\x86\x91", "\\uparrow " }, // ↑
    { "\xE2\x86\x92", "\\rightarrow " }, // →
    { "\xE2\x86\x93", "\\downarrow " }, // ↓
    { "\xE2\x86\x94", "\\leftrightarrow " }, // ↔
    { "\xE2\x86\x95", "\\updownarrow " }, // ↕
    { "\xE2\x86\x96", "\\nwarrow " }, // ↖
    { "\xE2\x86\x97", "\\nearrow " }, // ↗
    { "\xE2\x86\x98", "\\searrow " }, // ↘
    { "\xE2\x86\x99", "\\swarrow " }, // ↙
    { "\xE2\x87\x90", "\\Leftarrow " }, // ⇐
    { "\xE2\x87\x91", "\\Uparrow " }, // ⇑
    { "\xE2\x87\x92", "\\Rightarrow " }, // ⇒
    { "\xE2\x87\x93", "\\Downarrow " }, // ⇓
    { "\xE2\x87\x94", "\\Leftrightarrow " }, // ⇔
    { "\xE2\x87\x95", "\\Updownarrow " }, // ⇕
    { "\xE2\x86\xBC", "\\leftharpoonup " }, // ↼
    { "\xE2\x87\x80", "\\rightharpoonup " }, // ⇀
    { "\xE2\x86\xBD", "\\leftharpoondown " }, // ↽
    { "\xE2\x87\x81", "\\rightharpoondown " }, // ⇁
    { "\xE2\x87\x8B", "\\leftrightharpoons " }, // ⇋
    { "\xE2\x87\x8C", "\\rightleftharpoons " }, // ⇌

    // log-like symbols, see Table 177
    // produce the eponymous textual equivalent, with proper surrounding spacing
    { "arcos", "\\arccos " },
    { "arcsin" , "\\arcsin " },
    { "arctan", "\\arctan " },
    { "arg", "\\arg " },
    { "cosh", "\\cosh " },
    { "cos", "\\cos " },
    { "coth", "\\coth " },
    { "cot", "\\cot " },
    { "csc", "\\csc " },
    { "deg", "\\deg " },
    { "det", "\\det " },
    { "dim", "\\dim " },
    { "exp", "\\exp " },
    { "gcd", "\\gcd " },
    { "hom", "\\hom " },
    { "inf", "\\inf " },
    { "ker", "\\ker " },
    { "lg", "\\lg " },
    { "liminf", "\\liminf " },
    { "limsup", "\\limsup " },
    { "lim", "\\lim " },
    { "ln", "\\ln " },
    { "log", "\\log " },
    { "max", "\\max " },
    { "min", "\\min " },
    { "Pr", "\\Pr " },
    { "sec", "\\sec " },
    { "sinh", "\\sinh " },
    { "sin", "\\sin " },
    { "sup", "\\sup " },
    { "tanh", "\\tanh " },
    { "tan", "\\tan " },

    // Greek letters, see Table 180
    { "\xCE\xB1", "\\alpha " }, // α
    { "\xCE\xB2", "\\beta " }, // β
    { "\xCE\xB3", "\\gamma " }, // γ
    { "\xCE\xB4", "\\delta " }, // δ
    { "\xCE\xB5", "\\varepsilon " }, // ε
    { "\xCE\xB7", "\\eta " }, // η
    { "\xCE\xB6", "\\zeta " }, // ζ
    { "\xCE\xB8", "\\theta " }, // θ
    { "\xCE\xB9", "\\iota " }, // ι
    { "\xCE\xBA", "\\kappa " }, // κ
    { "\xCE\xBB", "\\lambda " }, // λ
    { "\xC2\xB5", "\\mu " }, // µ (we map micro sign to \mu)
    //{ "\xCE\xBC", "\\mu " }, // µ (not supported)
    { "\xCE\xBD", "\\nu " }, // ν
    { "\xCE\xBE", "\\xi " }, // ξ
    { "\xCF\x80", "\\pi " }, // π
    { "\xCF\x81", "\\rho " }, // ρ
    { "\xCF\x83", "\\sigma " }, // σ
    { "\xCF\x84", "\\tau " }, // τ
    { "\xCF\x85", "\\upsilon " }, // υ
    { "\xCF\x95", "\\phi " }, // ϕ
    { "\xCF\x86", "\\varphi " }, // φ
    { "\xCF\x87", "\\chi " }, // χ
    { "\xCF\x88", "\\psi " }, // ψ
    { "\xCF\x89", "\\omega " }, // ω

    { "\xCE\x93", "\\Gamma " }, // Γ
    { "\xCE\x94", "\\Delta " }, // Δ
    { "\xCE\x98", "\\Theta " }, // Θ
    { "\xCE\x9B", "\\Lambda " }, // Λ
    { "\xCE\x9E", "\\Xi " }, // Ξ
    { "\xCE\xA0", "\\Pi " }, // Π
    { "\xCE\xA3", "\\Sigma " }, // Σ
    { "\xCE\xA5", "\\Upsilon " }, // Υ
    { "\xCE\xA6", "\\Phi " }, // Φ
    { "\xCE\xA9", "\\Omega " }, // Ω
    { "\xCE\xA8", "\\Psi " }, // Ψ

    // letter-like symbols, see Table 195
    { "\xE2\x88\x83", "\\exists " }, // ∃
    { "\xE2\x88\x84", "\\nexists " }, // ∄
    { "\xE2\x88\x80", "\\forall " }, // ∀
    { "\xE2\x88\x88", "\\in " }, // ∈
    { "\xE2\x88\x89", "\\not\\in " }, // ∉
    //{ "\xE2\x88\x89", "\\notin " }, // ∉
    { "\xE2\x88\x8B", "\\ni " }, // ∋
    { "\xE2\x88\x8C", "\\not\\ni " }, // ∌
    //{ "\xE2\x88\x8C", "\\notni " }, // ∌
    { "\xE2\x88\x82", "\\partial " }, // ∂
    { "\xE2\x84\x8F", "\\hbar " }, // ℏ
    { "\xE2\x8A\xA5", "\\bot " }, // ⊥
    { "\xE2\x84\x91", "\\Im " }, // ℑ
    { "\xE2\x84\x9C", "\\Re " }, // ℜ
    { "\xE2\x84\x93", "\\ell " }, // ℓ

    // delimiters, see Table 214
    { "\xE2\x9F\xA8", "\\langle " }, // mathematical left angle bracket ⟨
    { "\xE2\x9F\xA9", "\\rangle " }, // mathematical right angle bracket ⟩
    { "\xE2\x9F\xA6", "\\llbracket " }, // ⟦
    { "\xE2\x9F\xA7", "\\rrbracket " }, // ⟧
    { "\xE2\x8C\x88", "\\lceil " }, // ⌈
    { "\xE2\x8C\x89", "\\rceil " }, // ⌉
    { "\xE2\x8C\x8A", "\\lfloor " }, // ⌊
    { "\xE2\x8C\x8B", "\\rfloor " }, // ⌋

    // math alphabets, see Table 299
    // (requires amsfonts, amssymb, txfonts, or pxfonts)
    { "\xE2\x84\x82", "\\mathbb{C} " }, // ℂ
    { "\xE2\x84\x95", "\\mathbb{N} " }, // ℕ
    { "\xE2\x84\x9A", "\\mathbb{Q} " }, // ℚ
    { "\xE2\x84\x9D", "\\mathbb{R} " }, // ℝ
    { "\xE2\x84\xA4", "\\mathbb{Z} " }, // ℤ
    { "\xF0\x9D\x94\xBB", "\\mathbb{D} " }, // 𝔻
    { "\xE2\x84\xB1", "\\mathcal{F} " }, // ℱ
    { "\xE2\x84\x92", "\\mathcal{L} " }, // ℒ

    // miscellaneous math LaTeX2e math symbols, see Table 291
    { "\xE2\x88\xA0", "\\angle " }, // ∠
    { "\xE2\x88\x9E", "\\infty " }, // ∞
    { "\xE2\x88\x87", "\\nabla " }, // ∇
    { "\xE2\x88\x85", "\\emptyset " }, // ∅
    //{ "\xE2\x88\x85", "\\varnothing " }, // ∅ (requires amsmath)
    { "\xC2\xAC", "\\neg " }, // ¬
    { "\xE2\x8C\x92", "\\frown " }, // ⌒

    // miscellaneous textcomp symbols, see Table 40
    //{ "\xE2\x80\xB0", "\\textperthousand" }, // ‰

    // currency symbols
    //{ "€", "\\euro " }, // €
    //{ "¥", "\\yen " }, // ¥
    //{ "£", "\\pounds " }, // £
    //{ "¢", "\\cent " }, // ¢
    //{ "₩", "\\textwon " }, // ₩

    // miscellaneous symbols
    { "<", " < " }, // in case LaTeX is embedded inside HTML
    { "/", " / " }, // in case LaTeX is embedded inside HTML
    { "\xE2\x80\xA6", "\\ldots " }, // …
    { "\xE2\x8B\xAE", "\\vdots " }, // ⋮
    { "\xE2\x8B\xB1", "\\ddots " }, // ⋱
    { "\xE2\x8B\xB0", "\\iddots " }, // ⋰
    { "\xE2\x96\xA1", "\\square " }, // □
    { "\xE2\x96\xB3", "\\triangle " } // △
    //{ "©", "\\copyright " } // ©

    // no LaTeX symbol
    // { "\xE2\x89\x86", "\xE2\x89\x86 " }, // ≆
    // { "\xE2\xA6\x9E", "\xE2\xA6\x9E " }, // ⦞
    // { "\xE2\xAB\xBB", "\xE2\xAB\xBB " }, // ⫻
  };

  int i = 0;
  int j = 0;

  length = length == -1 ? (int)strlen(input) : length;
  while (i < length)
  {
    int k;
    int length0;
    int count = (int)(sizeof(mappings) / sizeof(mappings[0]));

    // iterate over the mappings table
    for (k = 0; k < count; ++k)
    {
      length0 = (int)strlen(mappings[k][0]);

      if (length0 <= length - i)
      {
        if (strncmp(mappings[k][0], &input[i], length0) == 0)
          break; // mapping found
      }
    }

    if (k < count) // if mapping found
    {
      if (j > 0) // deferred output?
        parseAppend(state, &input[i - j], j);

      // output mapping
      parseAppend(state, mappings[k][1], -1);

      i += length0; // advance input
      j = 0;        // reset deferred output
    }
    else
    {
      ++i;  // advance input position
      ++j;  // defer output
    }
  }

  if (j > 0) // deferred output?
    parseAppend(state, &input[i - j], j);

  return true;
}

static int selectedTerminalAsUTF8(voEngine engine, voMathTerminalNode node, char* destination, int capacity)
{
  voCharset charset;
  voString label = { destination, (size_t)capacity };
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
  char buffer[PARSETREETOLATEX_BUFFER_SIZE];
  int length;

  assert(voGetType(engine, node) == VO_MathTerminalNode);

  length = selectedTerminalAsUTF8(engine, node, buffer, sizeof(buffer));
  assert(length <= (int)sizeof(buffer));
  voReleaseObject(engine, node);

  if (length == -1)
    return false;

  return toLaTeX(state, buffer, length);
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

  successful = parseTreeToLaTeX_(engine, candidate, state);
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

  successful = parseTreeToLaTeX_(engine, child, state);
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

  successful = parseTreeToLaTeX_(engine, child, state);
  voReleaseObject(engine, child);

  if (!successful)
    return false;

  // right symbol
  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 1);
  if (child == NULL)
    return false;

  successful = parseTreeToLaTeX_(engine, child, state);
  voReleaseObject(engine, child);

  return successful;
}

static bool parseFenceRule(voEngine engine, voMathRuleNode node, OutputState* state)
{
  voMathNode child;
  voMathNode ruleNode;
  char buffer[PARSETREETOLATEX_BUFFER_SIZE];
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

  if (strncmp(buffer, "(", length) == 0)
    parseAppend(state, "\\left( ", -1);
  else if (strncmp(buffer, "[", length) == 0)
    parseAppend(state, "\\left[ ", -1);
  else if (strncmp(buffer, "{", length) == 0)
    parseAppend(state, "\\left\\{ ", -1);
  else if (strncmp(buffer, "|", length) == 0)
    parseAppend(state, "\\left| ", -1);
  else if (strncmp(buffer, "\xE2\x88\xA5", length) == 0) // ∥
    parseAppend(state, "\\left\\| ", -1);
  else if (strncmp(buffer, "\xE2\x9F\xA8", length) == 0) // mathematical left angle ⟨
    parseAppend(state, "\\left\\langle ", -1);
  else if (strncmp(buffer, "\xE2\x9F\xA6", length) == 0) // ⟦
    parseAppend(state, "\\left\\llbracket ", -1);
  else if (strncmp(buffer, "\xE2\x8C\x88", length) == 0) // ⌈
    parseAppend(state, "\\left\\lceil ", -1);
  else if (strncmp(buffer, "\xE2\x8C\x8A", length) == 0) // ⌊
    parseAppend(state, "\\left\\lfloor ", -1);
  else if (strncmp(buffer, "]", length) == 0)
    parseAppend(state, "\\left] ", -1);

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
    parseAppend(state, "\\begin{matrix} ", -1);
    successful = successful && parseTreeToLaTeX_(engine, child, state);
    parseAppend(state, "\\end{matrix} ", -1);

    voReleaseObject(engine, ruleNode);
  }
  else
    successful = successful && parseTreeToLaTeX_(engine, child, state);

  voReleaseObject(engine, child);

  if (!successful)
    return false;

  // right symbol
  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 2);
  if (child == NULL)
    return false;
  length = selectedTerminalAsUTF8(engine, child, buffer, sizeof(buffer));
  assert(length <= (int)sizeof(buffer));
  voReleaseObject(engine, child);

  if (length == -1)
    return false;

  if (strncmp(buffer, ")", length) == 0)
    parseAppend(state, "\\right) ", -1);
  else if (strncmp(buffer, "]", length) == 0)
    parseAppend(state, "\\right] ", -1);
  else if (strncmp(buffer, "}", length) == 0)
    parseAppend(state, "\\right\\} ", -1);
  else if (strncmp(buffer, "|", length) == 0)
    parseAppend(state, "\\right| ", -1);
  else if (strncmp(buffer, "\xE2\x88\xA5", length) == 0) // ∥
    parseAppend(state, "\\right\\| ", -1);
  else if (strncmp(buffer, "\xE2\x9F\xA9", length) == 0) // mathematical right angle ⟩
    parseAppend(state, "\\right\\rangle ", -1);
  else if (strncmp(buffer, "\xE2\x9F\xA7", length) == 0) // ⟧
    parseAppend(state, "\\right\\rrbracket ", -1);
  else if (strncmp(buffer, "\xE2\x8C\x89", length) == 0) // ⌉
    parseAppend(state, "\\right\\rceil ", -1);
  else if (strncmp(buffer, "\xE2\x8C\x8B", length) == 0) // ⌋
    parseAppend(state, "\\right\\rfloor ", -1);
  else if (strncmp(buffer, "[", length) == 0) // ]
    parseAppend(state, "\\right[ ", -1);


  return true;
}

static bool parseFractionRule(voEngine engine, voMathRuleNode node, OutputState* state)
{
  voMathNode child;
  bool successful;

  assert(voGetType(engine, node) == VO_MathRuleNode);
  assert(VO_IFACE(engine, IMathRuleNode)->getChildCount(engine, node) == 3);

  // first child always corresponds to the fraction bar
  parseAppend(state, "\\dfrac {", -1);

  // then comes the numerator
  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 1);
  if (child == NULL)
    return false;

  successful = parseTreeToLaTeX_(engine, child, state);
  voReleaseObject(engine, child);

  if (!successful)
    return false;

  parseAppend(state, "} {", -1);

  // and the denominator
  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 2);
  if (child == NULL)
    return false;

  successful = parseTreeToLaTeX_(engine, child, state);
  voReleaseObject(engine, child);

  if (!successful)
    return false;

  parseAppend(state, "}", -1);

  return true;
}

static bool parseSqrtRule(voEngine engine, voMathRuleNode node, OutputState* state)
{
  voMathNode child;
  bool successful;

  assert(voGetType(engine, node) == VO_MathRuleNode);
  assert(VO_IFACE(engine, IMathRuleNode)->getChildCount(engine, node) == 2);

  // first child always corresponds to the sqrt sign
  parseAppend(state, "\\sqrt {", -1);

  // main expression
  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 1);
  if (child == NULL)
    return false;

  successful = parseTreeToLaTeX_(engine, child, state);
  voReleaseObject(engine, child);

  if (!successful)
    return false;

  parseAppend(state, "}", -1);

  return true;
}

static bool parseSubscriptRule(voEngine engine, voMathRuleNode node, OutputState* state)
{
  voMathNode child;
  bool successful;

  assert(voGetType(engine, node) == VO_MathRuleNode);
  assert(VO_IFACE(engine, IMathRuleNode)->getChildCount(engine, node) == 2);

  // base symbol
  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 0);
  if (child == NULL)
    return false;

  successful = parseTreeToLaTeX_(engine, child, state);
  voReleaseObject(engine, child);

  if (!successful)
    return false;

  parseAppend(state, "_{", -1);
  if (state->isColumnarOperation)
    parseAppend(state, "\\!", -1);

  // subscript symbol
  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 1);
  if (child == NULL)
    return false;

  successful = parseTreeToLaTeX_(engine, child, state);
  voReleaseObject(engine, child);

  if (!successful)
    return false;

  parseAppend(state, "}", -1);

  return true;
}

static bool parseSuperscriptRule(voEngine engine, voMathRuleNode node, OutputState* state)
{
  voMathNode child;
  bool successful;

  assert(voGetType(engine, node) == VO_MathRuleNode);
  assert(VO_IFACE(engine, IMathRuleNode)->getChildCount(engine, node) == 2);

  // base symbol
  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 0);
  if (child == NULL)
    return false;

  successful = parseTreeToLaTeX_(engine, child, state);
  voReleaseObject(engine, child);

  if (!successful)
    return false;

  parseAppend(state, "^{", -1);
  if (state->isColumnarOperation)
    parseAppend(state, "\\!", -1);

  // superscript symbol
  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 1);
  if (child == NULL)
    return false;

  successful = parseTreeToLaTeX_(engine, child, state);
  voReleaseObject(engine, child);

  if (!successful)
    return false;

  parseAppend(state, "}", -1);

  return true;
}

static bool parseSubsuperscriptRule(voEngine engine, voMathRuleNode node, OutputState* state)
{
  voMathNode child;
  bool successful;

  assert(voGetType(engine, node) == VO_MathRuleNode);
  assert(VO_IFACE(engine, IMathRuleNode)->getChildCount(engine, node) == 3);

  // base symbol
  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 0);
  if (child == NULL)
    return false;

  successful = parseTreeToLaTeX_(engine, child, state);
  voReleaseObject(engine, child);

  if (!successful)
    return false;

  // subscript symbol
  parseAppend(state, "_{", -1);

  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 1);
  if (child == NULL)
    return false;

  successful = parseTreeToLaTeX_(engine, child, state);
  voReleaseObject(engine, child);

  if (!successful)
    return false;

  // superscript symbol
  parseAppend(state, "}^{", -1);

  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 2);
  if (child == NULL)
    return false;

  successful = parseTreeToLaTeX_(engine, child, state);
  voReleaseObject(engine, child);

  if (!successful)
    return false;

  parseAppend(state, "}", -1);

  return true;
}

static bool parseUnderscriptRule(voEngine engine, voMathRuleNode node, OutputState* state)
{
  return parseSubscriptRule(engine, node, state);
}

static bool parseOverscriptRule(voEngine engine, voMathRuleNode node, OutputState* state)
{
  voMathNode child;
  char buffer[PARSETREETOLATEX_BUFFER_SIZE];
  OutputState state_ = { buffer, sizeof(buffer) };
  int length;
  bool special = true;
  bool successful;

  assert(voGetType(engine, node) == VO_MathRuleNode);
  assert(VO_IFACE(engine, IMathRuleNode)->getChildCount(engine, node) == 2);

  // top symbol
  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 1);
  if (child == NULL)
    return false;

  successful = parseTreeToLaTeX_(engine, child, &state_);
  length = state_.offset;
  voReleaseObject(engine, child);

  if (!successful)
    return false;

  // special LaTeX commands
  if (strncmp(buffer, "-", length) == 0)
    parseAppend(state, "\\overline {", -1);
  else if (strncmp(buffer, "\\wedge ", length) == 0)
    parseAppend(state, "\\widehat {", -1);
  else if (strncmp(buffer, "\\leftarrow ", length) == 0)
    parseAppend(state, "\\overleftarrow {", -1);
  else if (strncmp(buffer, "\\rightarrow ", length) == 0)
    parseAppend(state, "\\overrightarrow {", -1);
  else if (strncmp(buffer, "\\leftrightarrow ", length) == 0)
    parseAppend(state, "\\overleftrightarrow {", -1);
  else if (strncmp(buffer, "\\frown ", length) == 0)
    parseAppend(state, "\\widearc {", -1);
  else
    special = false;

  // main expression
  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 0);
  if (child == NULL)
    return false;

  successful = parseTreeToLaTeX_(engine, child, state);
  voReleaseObject(engine, child);

  if (!successful)
    return false;

  if (!special)
  {
    parseAppend(state, "^{", -1);
    parseAppend(state, buffer, length);
  }

  parseAppend(state, "}", -1);

  return true;
}

static bool parseUnderoverscriptRule(voEngine engine, voMathRuleNode node, OutputState* state)
{
  return parseSubsuperscriptRule(engine, node, state);
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
    child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 1);
    if (child == NULL)
    {
      voReleaseObject(engine, ruleNode);
      return false;
    }

    parseAppend(state, "\\sqrt [", -1);

    successful = parseTreeToLaTeX_(engine, child, state);
    voReleaseObject(engine, child);

    if (!successful)
    {
      voReleaseObject(engine, ruleNode);
      return false;
    }

    parseAppend(state, "] {", -1);

    child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, ruleNode, 1);
    voReleaseObject(engine, ruleNode);

    if (child == NULL)
      return false;

    successful = parseTreeToLaTeX_(engine, child, state);
    voReleaseObject(engine, child);

    if (!successful)
      return false;

    parseAppend(state, "}", -1);
  }
  else
  {
    if (state->isColumnarOperation)
      parseAppend(state, "\\!", -1);
    parseAppend(state, "\\vphantom {", -1);

    child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 0);
    if (child == NULL)
      return false;

    successful = parseTreeToLaTeX_(engine, child, state);
    voReleaseObject(engine, child);

    if (!successful)
      return false;

    parseAppend(state, "}^{", -1);

    child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 1);
    if (child == NULL)
      return false;

    successful = parseTreeToLaTeX_(engine, child, state);
    voReleaseObject(engine, child);

    if (!successful)
      return false;

    parseAppend(state, "} {", -1);
    if (state->isColumnarOperation)
      parseAppend(state, "\\!", -1);

    child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 0);
    if (child == NULL)
      return false;

    successful = parseTreeToLaTeX_(engine, child, state);
    voReleaseObject(engine, child);

    if (!successful)
      return false;

    parseAppend(state, "}", -1);
  }

  return true;
}

static bool parseVerticalPairRule(voEngine engine, voMathRuleNode node, OutputState* state)
{
  voMathNode child;
  bool successful;

  assert(voGetType(engine, node) == VO_MathRuleNode);
  assert(VO_IFACE(engine, IMathRuleNode)->getChildCount(engine, node) == 2);

  // top symbol
  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 0);
  if (child == NULL)
    return false;

  successful = parseTreeToLaTeX_(engine, child, state);
  voReleaseObject(engine, child);

  if (!successful)
    return false;

  parseAppend(state, "\\\\ ", -1);

  // bottom symbol
  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 1);
  if (child == NULL)
    return false;

  successful = parseTreeToLaTeX_(engine, child, state);
  voReleaseObject(engine, child);

  return successful;
}

static bool parseVerticalListRule(voEngine engine, voMathRuleNode node, OutputState* state)
{
  int32_t i, count;
  bool successful;
  bool isInsideCases = state->isInsideCases;

  assert(voGetType(engine, node) == VO_MathRuleNode);

  count = VO_IFACE(engine, IMathRuleNode)->getChildCount(engine, node);
  assert(count > 1);

  if (!isInsideCases)
    parseAppend(state, "\\begin{align*} & ", -1);

  for (i = 0; i < count; ++i)
  {
    voMathNode child;

    child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, i);
    if (child == NULL)
      return false;

    successful = parseTreeToLaTeX_(engine, child, state);
    voReleaseObject(engine, child);
    if (i < count - 1)
    {
      parseAppend(state, "\\\\ ", -1);

      if (!isInsideCases)
        parseAppend(state, "& ", -1);
    }

    if (!successful)
      return false;
  }

  if (!isInsideCases)
    parseAppend(state, "\\end{align*} ", -1);

  return successful;
}

static bool parseTableRule(voEngine engine, voMathRuleNode node, OutputState* state)
{
  voMathTableData tableData;
  int32_t childIndex, childCount;
  int32_t rowIndex, columnIndex;
  bool isInsideCases = state->isInsideCases;

  assert(voGetType(engine, node) == VO_MathTableNode);

  if (!isInsideCases)
    parseAppend(state, "\\begin{matrix} ", -1);

  if (!VO_IFACE(engine, IMathTableNode)->getData(engine, node, &tableData))
    return false;

  childCount = (int32_t)VO_IFACE(engine, IMathRuleNode)->getChildCount(engine, node);
  if (childCount == -1)
  return false;

  childIndex = 0;
  for (rowIndex = 0; rowIndex < tableData.rowCount; ++rowIndex)
  {
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
          if (!parseTreeToLaTeX_(engine, child, state))
          {
            voReleaseObject(engine, child);
            return false;
          }
          ++childIndex;
        }

        if (columnIndex < tableData.columnCount - 1)
          parseAppend(state, "& ", -1);

        voReleaseObject(engine, child);
      }
      else
      {
        if (columnIndex < tableData.columnCount - 1)
          parseAppend(state, "& ", -1);
      }
    }
    if (rowIndex < tableData.rowCount - 1)
      parseAppend(state, "\\\\ ", -1);
  }

  if (!isInsideCases)
    parseAppend(state, "\\end{matrix} ", -1);

  return true;
}

static bool parseLeftFenceRule(voEngine engine, voMathRuleNode node, OutputState* state)
{
  voMathNode child;
  voMathNode ruleNode;
  char buffer[PARSETREETOLATEX_BUFFER_SIZE];
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

  // main expression
  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 0);
  if (child == NULL)
    return false;

  successful = getChildRule3(engine, child, "table", "vertical pair", "vertical list", &ruleNode);
  if (!successful)
  {
    voReleaseObject(engine, child);
    return false;
  }

  if (strncmp(buffer, "{", length) == 0 && ruleNode != NULL)
  {
    voMathTableData tableData = { 0 };

    if (voGetType(engine, ruleNode) == VO_MathTableNode)
    {
      if (!VO_IFACE(engine, IMathTableNode)->getData(engine, ruleNode, &tableData))
      {
        voReleaseObject(engine, child);
        return false;
      }
    }

    if (tableData.columnCount < 3)
    {
      parseAppend(state, "\\begin{cases} ", -1);
      state->isInsideCases = true;
      successful = parseTreeToLaTeX_(engine, child, state);
      state->isInsideCases = false;
      parseAppend(state, "\\end{cases} ", -1);
    }
    else
    {
      parseAppend(state, "\\left\\{ ", -1);
      successful = parseTreeToLaTeX_(engine, child, state);
      parseAppend(state, "\\right. ", -1);
    }
  }
  else
  {
    if (strncmp(buffer, "(", length) == 0)
      parseAppend(state, "\\left( ", -1);
    else if (strncmp(buffer, "[", length) == 0)
      parseAppend(state, "\\left[ ", -1);
    else if (strncmp(buffer, "{", length) == 0)
      parseAppend(state, "\\left\\{ ", -1);
    else if (strncmp(buffer, "|", length) == 0)
      parseAppend(state, "\\left| ", -1);
    else if (strncmp(buffer, "\xE2\x88\xA5", length) == 0) // ∥
      parseAppend(state, "\\left\\| ", -1);
    else if (strncmp(buffer, "\xE2\x9F\xA8", length) == 0) // mathematical left angle ⟨
      parseAppend(state, "\\left\\langle ", -1);
    else if (strncmp(buffer, "\xE2\x9F\xA6", length) == 0) // ⟦
      parseAppend(state, "\\left\\llbracket ", -1);
    else if (strncmp(buffer, "\xE2\x8C\x88", length) == 0) // ⌈
      parseAppend(state, "\\left\\lceil", -1);
    else if (strncmp(buffer, "\xE2\x8C\x8A", length) == 0) // ⌊
      parseAppend(state, "\\left\\lfloor", -1);

    successful = parseTreeToLaTeX_(engine, child, state);

    parseAppend(state, "\\right. ", -1);
  }

  if (ruleNode != NULL)
    voReleaseObject(engine, ruleNode);

  voReleaseObject(engine, child);

  return successful;
}

static bool parsePresubscriptRule(voEngine engine, voMathRuleNode node, OutputState* state)
{
  voMathNode child;
  bool successful;

  assert(voGetType(engine, node) == VO_MathRuleNode);
  assert(VO_IFACE(engine, IMathRuleNode)->getChildCount(engine, node) == 2);

  if (state->isColumnarOperation)
    parseAppend(state, "\\!", -1);
  parseAppend(state, "\\vphantom {", -1);

  // base symbol
  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 0);
  if (child == NULL)
    return false;

  successful = parseTreeToLaTeX_(engine, child, state);
  voReleaseObject(engine, child);

  if (!successful)
    return false;

  parseAppend(state, "}_{", -1);
  if (state->isColumnarOperation)
    parseAppend(state, "\\!", -1);

  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 1);
  if (child == NULL)
    return false;

  successful = parseTreeToLaTeX_(engine, child, state);
  voReleaseObject(engine, child);

  if (!successful)
    return false;

  parseAppend(state, "} {", -1);

  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 0);
  if (child == NULL)
    return false;

  successful = parseTreeToLaTeX_(engine, child, state);
  voReleaseObject(engine, child);

  if (!successful)
    return false;

  parseAppend(state, "}", -1);

  return true;
}

static bool parseOverlapRule(voEngine engine, voMathRuleNode node, OutputState* state)
{
  voMathNode child;
  char buffer[PARSETREETOLATEX_BUFFER_SIZE];
  OutputState state_ = { buffer, sizeof(buffer) };
  int length;
  bool cancel = false;
  bool text = false;
  bool successful;

  assert(voGetType(engine, node) == VO_MathRuleNode);
  assert(VO_IFACE(engine, IMathRuleNode)->getChildCount(engine, node) == 2);

  // overlap symbol
  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 1);
  if (child == NULL)
    return false;

  successful = parseTreeToLaTeX_(engine, child, &state_);
  length = state_.offset;
  voReleaseObject(engine, child);

  if (!successful)
    return false;

  if (strncmp("\\backslash ", buffer, length) == 0)
  {
    parseAppend(state, "\\bcancel{", -1);
    cancel = true;
  }
  else if (strncmp(" / ", buffer, length) == 0)
  {
    parseAppend(state, "\\cancel{", -1);
    cancel = true;
  }
  else if (strncmp("-", buffer, length) == 0)
  {
    parseAppend(state, "\\text{\\sout{", -1);
    text = true;
    cancel = true;
  }
  else if (strncmp("\\times ", buffer, length) == 0)
  {
    parseAppend(state, "\\xcancel{", -1);
    cancel = true;
  }
  else if (strncmp("X", buffer, length) == 0)
  {
    parseAppend(state, "\\xcancel{", -1);
    cancel = true;
  }

  if (cancel)
  {
    // source symbol
    child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 0);
    if (child == NULL)
      return false;

    successful = parseTreeToLaTeX_(engine, child, state);
    voReleaseObject(engine, child);

    if (!successful)
      return false;

    parseAppend(state, "}", -1);

    if (text)
      parseAppend(state, "}", -1);
  }
  else
  {
    // source symbol
    child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 0);
    if (child == NULL)
      return false;

    successful = parseTreeToLaTeX_(engine, child, state);
    voReleaseObject(engine, child);

    if (!successful)
      return false;

    // overlap symbol
    child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 1);
    if (child == NULL)
      return false;

    successful = parseTreeToLaTeX_(engine, child, state);
    voReleaseObject(engine, child);

    if (!successful)
      return false;
  }

  return true;
}

static bool parseColumnarOperationHorizontalPairRule(voEngine engine, voMathRuleNode node, OutputState* state)
{
  voMathNode child;
  voMathNode ruleNode;
  bool isPreSubSup = false;
  bool isSubSup = false;
  bool successful;

  assert(voGetType(engine, node) == VO_MathRuleNode);
  assert(VO_IFACE(engine, IMathRuleNode)->getChildCount(engine, node) == 2);

  // left symbol
  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 0);
  if (child == NULL)
    return false;

  // check if left symbol is subscript or superscript
  successful = getChildRule(engine, child, "superscript", &ruleNode);
  if (!successful)
  {
    voReleaseObject(engine, child);
    return false;
  }

  if (ruleNode != NULL)
  {
    isSubSup = true;
    voReleaseObject(engine, ruleNode);
  }
  else
  {
    successful = getChildRule(engine, child, "subscript", &ruleNode);
    if (!successful)
    {
      voReleaseObject(engine, child);
      return false;
    }

    if (ruleNode != NULL)
    {
      isSubSup = true;
      voReleaseObject(engine, ruleNode);
    }
  }

  // check if left symbol is presubscript or presuperscript
  successful = getChildRule(engine, child, "presuperscript", &ruleNode);
  if (!successful)
  {
    voReleaseObject(engine, child);
    return false;
  }

  if (ruleNode != NULL)
  {
    isPreSubSup = true;
    voReleaseObject(engine, ruleNode);
  }
  else
  {
    successful = getChildRule(engine, child, "presubscript", &ruleNode);
    if (!successful)
    {
      voReleaseObject(engine, child);
      return false;
    }

    if (ruleNode != NULL)
    {
      isPreSubSup = true;
      voReleaseObject(engine, ruleNode);
    }
  }

  if (isPreSubSup)
    parseAppend(state, "\\!", -1);

  successful = parseTreeToLaTeX_(engine, child, state);
  voReleaseObject(engine, child);

  if (!successful)
    return false;

  if (isSubSup)
    parseAppend(state, "\\!", -1);
  else
    parseAppend(state, "\\,", -1);

  // right symbol
  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 1);
  if (child == NULL)
    return false;

  successful = parseTreeToLaTeX_(engine, child, state);
  voReleaseObject(engine, child);

  return successful;
}

static bool parseColumnarOperationOverscriptRule(voEngine engine, voMathRuleNode node, OutputState* state)
{
  voMathNode child;
  voMathNode ruleNode;
  bool multipleCarries = false;
  bool successful;

  assert(voGetType(engine, node) == VO_MathRuleNode);
  assert(VO_IFACE(engine, IMathRuleNode)->getChildCount(engine, node) == 2);

  parseAppend(state, "\\overset{", -1);

  // top symbol
  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 1);
  if (child == NULL)
    return false;

  // handle multiple carries
  successful = getChildRule(engine, child, "vertical pair", &ruleNode);
  if (!successful)
  {
    voReleaseObject(engine, child);
    return false;
  }

  if (ruleNode != NULL)
  {
    multipleCarries = true;
    parseAppend(state, "\\substack{", -1);
  }

  successful = parseTreeToLaTeX_(engine, child, state);
  voReleaseObject(engine, child);

  if (!successful)
    return false;

  if (multipleCarries)
    parseAppend(state, "}", -1);

  parseAppend(state, "}{", -1);

  // main expression
  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 0);
  if (voGetError(engine) != VO_NO_ERROR)
    return false;

  if (child == NULL) // standalone carry
  {
    parseAppend(state, "}{\\vphantom{x}", -1);
  }
  else
  {
    successful = parseTreeToLaTeX_(engine, child, state);
    voReleaseObject(engine, child);

    if (!successful)
      return false;
  }

  parseAppend(state, "}", -1);

  return true;
}

static bool parseColumnarOperationUnderscriptRule(voEngine engine, voMathRuleNode node, OutputState* state)
{
  voMathNode child;
  bool standaloneCarry = false;
  bool successful;

  assert(voGetType(engine, node) == VO_MathRuleNode);
  assert(VO_IFACE(engine, IMathRuleNode)->getChildCount(engine, node) == 2);

  // main expression
  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 0);
  if (voGetError(engine) != VO_NO_ERROR)
    return false;

  if (child == NULL) // standalone carry
  {
    parseAppend(state, "{\\vphantom{x}}_{", -1);
    standaloneCarry = true;
  }
  else
  {
    parseAppend(state, "\\underset{", -1);
    voReleaseObject(engine, child);
  }

  // bottom symbol
  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 1);
  if (child == NULL)
    return false;

  successful = parseTreeToLaTeX_(engine, child, state);
  voReleaseObject(engine, child);

  if (!successful)
    return false;

  if (!standaloneCarry)
    parseAppend(state, "}{", -1);

  // main expression
  child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, 0);
  if (child != NULL)
  {
    successful = parseTreeToLaTeX_(engine, child, state);
    voReleaseObject(engine, child);

    if (!successful)
      return false;
  }

  parseAppend(state, "}", -1);

  return true;
}

static void addHorizontalLine(const voMathTableData* tableData, const voMathBorderData* borderData, OutputState* state)
{
  if (borderData->start == 0 && borderData->stop == tableData->columnCount - 1)
  {
    parseAppend(state, "\\hline ", -1);
  }
  else
  {
    char buffer[PARSETREETOLATEX_BUFFER_SIZE];
    int length;

    length = sprintf(buffer, "\\cline{%d-%d}", borderData->start + 1, borderData->stop + 1);
    parseAppend(state, buffer, length);
  }
}

static bool parseColumnarOperationRule(voEngine engine, voMathRuleNode node, OutputState* state)
{
  voMathTableData tableData;
  int32_t childIndex, childCount;
  voMathBorderData horizontalBorders[PARSETREETOLATEX_BORDER_BUFFER_SIZE];
  voMathBorderData verticalBorders[PARSETREETOLATEX_BORDER_BUFFER_SIZE];
  int32_t borderIndex, horizontalBorderCount, verticalBorderCount;
  int32_t columnIndex, rowIndex;
  bool successful;

  assert(voGetType(engine, node) == VO_MathTableNode);

  state->isColumnarOperation = true;

  successful = VO_IFACE(engine, IMathTableNode)->getData(engine, node, &tableData);
  if (!successful)
    return false;

  childCount = VO_IFACE(engine, IMathRuleNode)->getChildCount(engine, node);
  if (childCount == -1)
    return false;

  horizontalBorderCount = verticalBorderCount = 0;
  for (childIndex = 0; childIndex < childCount; ++childIndex)
  {
    voMathNode child;

    child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, childIndex);
    if (child == NULL)
      return false;

    if (voGetType(engine, child) == VO_MathBorderNode)
    {
      voMathBorderData borderData;

      if (!VO_IFACE(engine, IMathBorderNode)->getData(engine, child, &borderData))
      {
        voReleaseObject(engine, child);
        return false;
      }

      if (borderData.type == MATH_BORDER_TYPE_HORIZONTAL)
      {
        assert(horizontalBorderCount < PARSETREETOLATEX_BORDER_BUFFER_SIZE);
        horizontalBorders[horizontalBorderCount++] = borderData;
      }
      else
      {
        assert(verticalBorderCount < PARSETREETOLATEX_BORDER_BUFFER_SIZE);
        verticalBorders[verticalBorderCount++] = borderData;
      }
    }

    voReleaseObject(engine, child);
  }

  parseAppend(state, "\\begin{array}{", -1);

  columnIndex = 0;
  for (borderIndex = 0; borderIndex < verticalBorderCount; ++borderIndex)
  {
    if (verticalBorders[borderIndex].position < tableData.columnCount)
    {
      for (; columnIndex < verticalBorders[borderIndex].position; ++columnIndex)
        parseAppend(state, "@{}>{$}p{0.35cm}<{$}@{}", -1);

      parseAppend(state, "|@{}>{$\\:}p{0.35cm}<{$}@{}", -1);
      ++columnIndex;
    }
    else
    {
      if (tableData.columnCount > 0)
        parseAppend(state, "|", -1);
      else
        parseAppend(state, "|c", -1);
    }
  }

  for (; columnIndex < tableData.columnCount; ++columnIndex)
    parseAppend(state, "@{}>{$}p{0.35cm}<{$}@{}", -1);

  parseAppend(state, "}", -1);

  rowIndex = columnIndex = borderIndex = 0;
  for (childIndex = 0; childIndex < childCount; ++childIndex)
  {
    voMathNode child;

    child = VO_IFACE(engine, IMathRuleNode)->getChildAt(engine, node, childIndex);
    if (child == NULL)
      return false;

    if (voGetType(engine, child) == VO_MathCellNode)
    {
      voMathCellData cellData;

      if (!VO_IFACE(engine, IMathCellNode)->getData(engine, child, &cellData))
      {
        voReleaseObject(engine, child);
        return false;
      }

      while (rowIndex <= cellData.rowStart)
      {
        if (borderIndex < horizontalBorderCount && horizontalBorders[borderIndex].position == rowIndex)
        {
          addHorizontalLine(&tableData, &horizontalBorders[borderIndex], state);
          ++borderIndex;
        }

        if (rowIndex < cellData.rowStart)
        {
          while (columnIndex < tableData.columnCount - 1)
          {
            parseAppend(state, "& ", -1);
            ++columnIndex;
          }
          parseAppend(state, "\\\\ ", -1);
          ++rowIndex;
          columnIndex = 0;
        }
        else
        {
          while (columnIndex < cellData.columnStart)
          {
            parseAppend(state, "& ", -1);
            ++columnIndex;
          }

          successful = parseTreeToLaTeX_(engine, child, state);
          if (!successful)
          {
            voReleaseObject(engine, child);
            return false;
          }

          break;
        }
      }
    }
    voReleaseObject(engine, child);
  }

  while (columnIndex < tableData.columnCount - 1)
  {
    parseAppend(state, "& ", -1);
    ++columnIndex;
  }

  while (borderIndex < horizontalBorderCount)
  {
    parseAppend(state, "\\\\ ", -1);
    addHorizontalLine(&tableData, &horizontalBorders[borderIndex], state);

    ++borderIndex;
  }

  parseAppend(state, "\\end{array}", -1);

  state->isColumnarOperation = false;

  return true;
}

static bool parseRuleNode(voEngine engine, voMathRuleNode node, OutputState* state)
{
  voCharset charset;
  char buffer[PARSETREETOLATEX_BUFFER_SIZE];
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
    return state->isColumnarOperation ? parseColumnarOperationHorizontalPairRule(engine, node, state) : parseHorizontalPairRule(engine, node, state);
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
    return state->isColumnarOperation ? parseColumnarOperationUnderscriptRule(engine, node, state) : parseUnderscriptRule(engine, node, state);
  else if (strncmp(name.bytes, "overscript", name.byteCount) == 0)
    return state->isColumnarOperation ? parseColumnarOperationOverscriptRule(engine, node, state) : parseOverscriptRule(engine, node, state);
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
    return parseColumnarOperationRule(engine, node, state);
  else if (strncmp(name.bytes, "vertical list", name.byteCount) == 0)
    return parseVerticalListRule(engine, node, state);
  else if (strncmp(name.bytes, "table", name.byteCount) == 0)
    return parseTableRule(engine, node, state);
  else
    PARSETREETOLATEX_ERROR("unknown rule");

  return false;
}

static bool getChildRule(voEngine engine, voMathNode node, const char* name, voMathRuleNode* ruleNode)
{
  voTypeId type;
  int32_t index;
  voMathNode candidate;
  voCharset charset;
  char buffer[PARSETREETOLATEX_BUFFER_SIZE];
  voString name_ = { buffer, sizeof(buffer) };
  bool successful;

  type = voGetType(engine, node);

  if (type == VO_MathTerminalNode)
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
    PARSETREETOLATEX_ERROR("unknown node type");
    return false;
  }
}

static bool getChildRule3(voEngine engine, voMathNode node, const char* name1, const char* name2, const char* name3, voMathRuleNode* ruleNode)
{
  voTypeId type;
  uint32_t index;
  voMathNode candidate;
  voCharset charset;
  char buffer[PARSETREETOLATEX_BUFFER_SIZE];
  voString name_ = { buffer, sizeof(buffer) };
  bool successful;

  type = voGetType(engine, node);

  if (type == VO_MathTerminalNode)
  {
    *ruleNode = NULL;
    return true;
  }
  else if (voIsCompatible(engine, type, VO_MathNonTerminalNode))
  {
    index = VO_IFACE(engine, IMathNonTerminalNode)->getSelectedCandidateIndex(engine, node);
    if (index == (uint32_t)-1)
      return false;

    candidate = VO_IFACE(engine, IMathNonTerminalNode)->getCandidateAt(engine, node, index);
    if (candidate == NULL)
      return false;

    successful = getChildRule3(engine, candidate, name1, name2, name3, ruleNode);
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

      successful = getChildRule3(engine, child, name1, name2, name3, ruleNode);
      voReleaseObject(engine, child);
    }
    else
    {
      if (  ((name1 != NULL) && (strncmp(name_.bytes, name1, name_.byteCount) == 0))
          ||((name2 != NULL) && (strncmp(name_.bytes, name2, name_.byteCount) == 0))
          ||((name3 != NULL) && (strncmp(name_.bytes, name3, name_.byteCount) == 0)))
      {
        *ruleNode = voAcquireObject(engine, node);
      }
      else
        *ruleNode = NULL;
    }
    return successful;
  }
  else
  {
    PARSETREETOLATEX_ERROR("unknown node type");
    return false;
  }
}
