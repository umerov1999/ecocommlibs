#ifndef C_MYSCRIPTPREDICTION_H
#define C_MYSCRIPTPREDICTION_H 0x08000000
/**
 * @file c/MyScriptPrediction.h
 * Native interface to the MyScript Prediction service.
 */

#include "MyScriptEngine.h"


// -- types and interfaces -----------------------------------------------------

/**
 * Lists the MyScript Prediction types.
 */
enum VO_PREDICTION_T
{
  /**
   * `voPredictor` type identifier.
   *
   * @see voPredictor
   */
  VO_Predictor = 6000
};


// -- Predictor ----------------------------------------------------------------

/**
 * `voPredictor` reference type.
 *
 * {@extends voEngineObject}
 */
typedef voEngineObject voPredictor;

/**
 * {@propertySet Predictor}
 */
enum VO_PREDICTOR_PROP
{
  /**
   * Specifies the size of the word completion candidate lists that will be
   * provided in the recognition result.
   *
   * This value must be between `0` and `20`.
   *
   * {@property uint32_t}
   */
  VO_WORD_COMPLETION_CANDIDATE_LIST_SIZE = (VO_Predictor << 16),

  /**
   * Specifies the size of the word prediction candidate lists that will be
   * provided in the recognition result.
   *
   * This value must be between `0` and `20`.
   *
   * {@property uint32_t}
   */
  VO_WORD_PREDICTION_CANDIDATE_LIST_SIZE,

  /**
   * Enables predictions on completion results.
   *
   * {@property bool}
   *
   * @since 7.4_0
   */
  VO_ENABLE_PREDICTION_ON_COMPLETION
};

#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voPredictor` instance.
 *
 * @param engine the engine.
 *
 * @return the newly created `voPredictor` instance on success, otherwise
 *   `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_LIMIT_EXCEEDED when the number of `voPredictor` instances would
 *   exceed the allowed limit (please remember that the limit can be shared by
 *   several types).
 */
VO_INLINE voPredictor voCreatePredictor(voEngine engine)
{
  return voCreateObject(engine, VO_Predictor);
}
#endif // end of: #ifndef VO_NO_FUNCTIONS

/**
 * Lists the recognition result candidate flags for candidates added by the
 * predictor.
 *
 * @note All the candidates added by the predictor are also flagged with
 *   `VO_CANDIDATE_FLAG_OMITTED`.
 *
 * @see VO_CANDIDATE_FLAG
 */
enum VO_PREDICTION_CANDIDATE_FLAG
{
  /**
   * The candidate comes from the completion phase.
   */
  VO_CANDIDATE_FLAG_COMPLETED = 1 << 8,
  /**
   * The candidate comes from the prediction phase.
   */
  VO_CANDIDATE_FLAG_PREDICTED = 1 << 9
};

#endif // end of: #ifndef C_MYSCRIPTPREDICTION_H
