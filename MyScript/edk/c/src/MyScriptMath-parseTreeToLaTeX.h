#ifndef EDK_C_SRC_MYSCRIPTMATH_PARSETREETOLATEX_H
/**
 * @file MyScriptMath-parseTreeToLaTeX.h
 *
 */


#include <c/MyScriptEngine.h>
#include <c/MyScriptMath.h>


#ifdef __cplusplus
extern "C" {
#endif

/**
 * Serializes the MathDocument's associated parse tree to a LaTeX string.
 *
 * Usage:
 *  - first call int length = parseTreeToLaTeX(engine, root, NULL, 0); to
 *    retrieve the length of the serialized LaTeX string.
 *  - allocate the destination buffer with str = malloc(length + 1);
 *  - call parseTreeToLaTeX(engine, root, str, length) to retrieve the
 *    serialized LaTeX string.
 *  - str[length] = '\0';
 *
 * @param engine Reference to an <code>Engine</code> instance.
 * @param root Reference to the root <code>MathNode</code> instance.
 * @param utf8Buffer Destination UTF-8 buffer.
 * @param capacity Destination buffer capacity.
 *
 * @return The length of the serialized LaTeX string on success (without a
 *   terminal NUL character), otherwise <code>-1</code>.
 */
int parseTreeToLaTeX(voEngine engine, voMathNode root, char* utf8Buffer, int capacity);

#ifdef __cplusplus
}
#endif

#endif // endof: #ifndef EDK_C_SRC_MYSCRIPTMATH_PARSETREETOLATEX_H
