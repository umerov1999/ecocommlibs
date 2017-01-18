#ifndef C_MYSCRIPTWRITERADAPTATION_H
#define C_MYSCRIPTWRITERADAPTATION_H 0x08000000
/**
 * @file c/MyScriptWriterAdaptation.h
 * Native interface to the MyScript WriterAdaptation service.
 */

#include "MyScriptEngine.h"
#include "MyScriptText.h"


// -- types and interfaces -----------------------------------------------------

/**
 * Lists the MyScript Writer Adaptation types.
 */
enum VO_WRITERADAPTATION_T
{
  /**
   * `voWriterAdaptationCollector` type identifier.
   *
   * @see voWriterAdaptationCollector
   */
  VO_WriterAdaptationCollector = 7000,

  /**
   * `voWriterAdaptationProfile` type identifier.
   *
   * @see voWriterAdaptationProfile
   */
  VO_WriterAdaptationProfile,

  /**
   * `voWriterAdaptationTrainer` type identifier.
   *
   * @see voWriterAdaptationTrainer
   */
  VO_WriterAdaptationTrainer,
};

/**
 * Lists the MyScript Writer Adaptation interfaces.
 */
enum VO_WRITERADAPTATION_I
{
  /**
   * `voIWriterAdaptationCollector` interface identifier.
   *
   * @see voIWriterAdaptationCollector
   */
  VO_IWriterAdaptationCollector = 7005,
};


// -- WriterAdaptationCollector ------------------------------------------------

/**
 * `voWriterAdaptationCollector` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voIAttachTarget}
 *
 * @see voStoreObject, voLoadObject
 */
typedef voEngineObject voWriterAdaptationCollector;

/**
 * Functions composing the `voIWriterAdaptationCollector` interface.
 *
 * This interface exposes the public methods of the
 * `voWriterAdaptationCollector` type.
 *
 * {@implementingTypes voWriterAdaptationCollector}
 */
typedef struct _voIWriterAdaptationCollector
{
  /**
   * Commits the samples collected during the last recognition to the attached
   * profile.
   *
   * @param engine the engine.
   * @param target the target collector.
   *
   * @return `true`on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a
   *   `voWriterAdaptationCollector`.
   * @throws VO_INVALID_STATE when `target` has no attached profile.
   */
  bool (VO_MSE_CALL *commit)(voEngine engine, voWriterAdaptationCollector target);
}
voIWriterAdaptationCollector;


// -- WriterAdaptationProfile --------------------------------------------------

/**
 * `voWriterAdaptationProfile` reference type.
 *
 * {@extends voEngineObject}
 * @see voStoreObject, voLoadObject
 */
typedef voEngineObject voWriterAdaptationProfile;


// -- WriterAdaptationTrainer --------------------------------------------------

/**
 * `voWriterAdaptationTrainer` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voIAttachTarget}
 * {@implements voIRunnable}
 *
 * @see voStoreObject, voLoadObject
 */
typedef voEngineObject voWriterAdaptationTrainer;


#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voWriterAdaptationProfile` instance.
 *
 * @param engine the engine.
 *
 * @return the newly created `voWriterAdaptationProfile` instance on success,
 *   otherwise `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_LIMIT_EXCEEDED when the number of `voWriterAdaptationProfile`
 *   instances would exceed the allowed limit (please remember that the limit
 *   can be shared by several types).
 */
VO_INLINE voWriterAdaptationProfile voCreateWriterAdaptationProfile(voEngine engine)
{
  return voCreateObject(engine, VO_WriterAdaptationProfile);
}

/**
 * Creates a new `voWriterAdaptationCollector` instance.
 *
 * @param engine the engine.
 *
 * @return the newly created `voWriterAdaptationCollector` instance on success,
 *   otherwise `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_LIMIT_EXCEEDED when the number of `voWriterAdaptationCollector`
 *   instances would exceed the allowed limit (please remember that the limit
 *   can be shared by several types).
 */
VO_INLINE voWriterAdaptationCollector voCreateWriterAdaptationCollector(voEngine engine)
{
  return voCreateObject(engine, VO_WriterAdaptationCollector);
}

/**
 * Creates a new `voWriterAdaptationTrainer` instance.
 *
 * @param engine the engine.
 *
 * @return the newly created `voWriterAdaptationTrainer` instance on success,
 *   otherwise `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_LIMIT_EXCEEDED when the number of `voWriterAdaptationTrainer`
 *   instances would exceed the allowed limit (please remember that the limit
 *   can be shared by several types).
 */
VO_INLINE voWriterAdaptationTrainer voCreateWriterAdaptationTrainer(voEngine engine)
{
  return voCreateObject(engine, VO_WriterAdaptationTrainer);
}
#endif // end of: #ifndef VO_NO_FUNCTIONS

#endif // end of: #ifndef C_MYSCRIPTWRITERADAPTATION_H
