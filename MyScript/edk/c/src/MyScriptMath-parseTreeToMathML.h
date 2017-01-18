#ifndef EDK_C_SRC_MYSCRIPTMATH_PARSETREETOMATHML_H
/**
 * @file MyScriptMath-parseTreeToMathML.h
 *
 */


#include <c/MyScriptEngine.h>
#include <c/MyScriptMath.h>


#ifdef __cplusplus
extern "C" {
#endif

/**
 * Serializes the MathDocument's associated parse tree to a MathML string.
 *
 * Usage:
 *  - first call int length = parseTreeToMathML(engine, root, NULL, 0); to
 *    retrieve the length of the serialized MathML string.
 *  - allocate the destination buffer with str = malloc(length + 1);
 *  - call parseTreeToMathML(engine, root, str, length) to retrieve the
 *    serialized MathML string.
 *  - str[length] = '\0';
 *
 * @param engine Reference to an <code>Engine</code> instance.
 * @param root Reference to the root <code>MathNode</code> instance.
 * @param utf8Buffer Destination UTF-8 buffer.
 * @param capacity Destination buffer capacity.
 *
 * @return The length of the serialized MathML string on success (without a
 *   terminal NUL character), otherwise <code>-1</code>.
 */
int parseTreeToMathML(voEngine engine, voMathNode root, char* utf8Buffer, int capacity);

#ifdef __cplusplus
}
#endif

#endif // endof: #ifndef EDK_C_SRC_MYSCRIPTMATH_PARSETREETOMATHML_H
