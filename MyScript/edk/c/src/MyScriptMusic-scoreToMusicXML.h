#ifndef EDK_C_SRC_MYSCRIPTMUSIC_SCORETOMUSICXML_H
/**
 * @file MyScriptMusic-scoreToMusicXML.h
 *
 */


#include <c/MyScriptEngine.h>
#include <c/MyScriptMusic.h>


#ifdef __cplusplus
extern "C" {
#endif

/**
 * Serializes the MusicDocument's associated score to a MusicXML string.
 *
 * Usage:
 *  - first call int length = scoreToMusicXML(engine, score, NULL, 0); to
 *    retrieve the length of the serialized MusicXML string.
 *  - allocate the destination buffer with str = malloc(length + 1);
 *  - call scoreToMusicXML(engine, score, str, length) to retrieve the
 *    serialized MusicXML string.
 *  - str[length] = '\0';
 *
 * @param engine Reference to an <code>Engine</code> instance.
 * @param score Reference to a <code>MusicScore</code> instance.
 * @param utf8Buffer Destination UTF-8 buffer.
 * @param capacity Destination buffer capacity.
 *
 * @return The length of the serialized MusicXML string on success (without a
 *   terminal NUL character), otherwise <code>-1</code>.
 */
int scoreToMusicXML(voEngine engine, voMusicScore score, char* utf8Buffer, int capacity);

#ifdef __cplusplus
}
#endif

#endif // endof: #ifndef EDK_C_SRC_MYSCRIPTMUSIC_SCORETOMUSICXML_H
