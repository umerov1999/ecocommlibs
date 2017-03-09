#ifndef C_MYSCRIPTCONFIGURATIONMANAGER_H
#define C_MYSCRIPTCONFIGURATIONMANAGER_H 0x08010000
/**
 * @file c/MyScriptConfigurationManager.h
 * Native interface to the MyScript Configuration Manager service. Please note
 * that the implementation of MyScript Configuration Manager is delivered as
 * part of the MyScriptEngine library.
 */

#include "MyScriptEngine.h"


// -- error codes --------------------------------------------------------------

/**
 * Lists the MyScript Configuration Manager errors.
 */
enum VO_CONFIGURATIONMANAGER_ERR
{
  /**
   * The configuration bundle name does not correspond to a `.conf` file in the
   * configuration manager search directories.
   */
  VO_CONFIGURATION_BUNDLE_NOT_FOUND = VO_INVALID_STATE | (8 << 16),

  /**
   * The configuration name does not correspond to an existing configuration in
   * a bundle.
   */
  VO_CONFIGURATION_NOT_FOUND,

  /**
   * The configuration refers to a resource that cannot be found.
   */
  VO_RESOURCE_NOT_FOUND,

  /**
   * The configuration contains incorrect commands.
   */
  VO_INVALID_CONFIGURATION
};


// -- types and interfaces -----------------------------------------------------

/**
 * Lists the MyScript Configuration Manager types.
 */
enum VO_CONFIGURATIONMANAGER_T
{
  /**
   * `voConfigurationManager` type identifier.
   *
   * @see voConfigurationManager
   */
  VO_ConfigurationManager = 256,

  /**
   * `voConfigurer` type identifier.
   *
   * @see voConfigurer
   */
  VO_Configurer
};

/**
 * Lists the MyScript Configuration Manager interfaces.
 */
enum VO_CONFIGURATIONMANAGER_I
{
  /**
   * `voIConfigurationManager` interface identifier.
   *
   * @see voIConfigurationManager
   */
  VO_IConfigurationManager = 260,

  /**
   * `voIConfigurer` interface identifier.
   *
   * @see voIConfigurer
   */
  VO_IConfigurer
};


// -- ConfigurationManager -----------------------------------------------------

/**
 * `voConfigurationManager` reference type.
 *
 * The configuration manager provides easy configuration of all MyScript
 * processing units from Text to Shape, Math, Music, and Analyzer. It manages a
 * set of configuration bundles defined through search directories to leverage
 * named configurations. A set of standard configurations is provided as part of
 * the MyScript SDK in the `conf/text`, `conf/shape`, ... directories. You can
 * modify and/or add configurations to adjust them to your specific needs.
 *
 * The actual configuration is defined through a simple scripting language
 * similar to the scripts in `InkTool`. Below is a list of the supported
 * commands:
 *
 *   Target          | Syntax                                     | Arguments
 *   --------------- | ------------------------------------------ | ----------------------
 *   All             | `AddResDir DIRECTORY`                      | A search directory for resources
 *   .               | `AddResource FILE`                         | A resource file name
 *   TextRecognizer  | `DiscardCaseVariations FLAG`               | An optional boolean value
 *   .               | `DiscardAccentuationVariations FLAG`       | An optional boolean value
 *   .               | `SetCharListSize VALUE`                    | An integer value between 1 and 20
 *   .               | `SetWordListSize VALUE`                    | An integer value between 1 and 20
 *   .               | `SetTextListSize VALUE`                    | An integer value between 1 and 20
 *   .               | `SetSpeedQualityCompromise VALUE`          | An integer value between 0 and 255
 *   .               | `SetOverrideAccentuationScheme SCHEME`     | Either STRICT_ACCENTUATION or LAX_ACCENTUATION
 *   .               | `SetOverrideCasingScheme SCHEME`           | Either LEGACY_CASE, or STRICT_CASE, or LAX_CASE
 *   .               | `SetSpellingDistortion VALUE`              | An integer value between 0 and 255
 *   .               | `SetGlyphDistortion VALUE`                 | An integer value between 0 and 255
 *   .               | `EnableOutOfLexicon FLAG`                  | An optional boolean value
 *   .               | `SetCharacters STRING`                     | A UTF-8 string containing the character subset
 *   .               | `EnablePredictor FLAG`                     | An optional boolean value
 *   .               | `SetWordCompletionListSize VALUE`          | An integer value between 0 and 20
 *   .               | `SetWordPredictionListSize VALUE`          | An integer value between 0 and 20
 *   TextIndexer     | `SetSizeAccuracyCompromize VALUE`          | An integer value between 0 and 255
 *   ShapeRecognizer | `SetRejectDetectionSensitivity VALUE`      | A decimal value between 0 and 1
 *   .               | `SetScratchOutDetectionSensitivity VALUE`  | A decimal value between 0 and 1
 *   .               | `SetModelDetectionSensitivity MODEL VALUE` | The model name and a decimal value between 0 and 1
 *   ShapeBeautifier | `SetAlignmentStrength VALUE`               | A decimal value between 0 and 1
 *   .               | `SetCongruenceStrength VALUE`              | A decimal value between 0 and 1
 *   .               | `SetConnectionStrength VALUE`              | A decimal value between 0 and 1
 *   MathRecognizer  | `SetScratchOutDetectionSensitivity VALUE`  | A decimal value between 0 and 1
 *   MusicRecognizer | `SetScratchOutDetectionSensitivity VALUE`  | A decimal value between 0 and 1
 *   Analyzer        | `SetProcessingLevel LEVEL`                 | Either STROKE_TYPE, or TEXTLINE, or RECOGNITION, or BEAUTIFICATION, or ALL
 *   .               | `SetTextRecognizerConf BUNDLE NAME`        | The bundle name and the configuration name
 *   .               | `SetShapeRecognizerConf BUNDLE NAME`       | The bundle name and the configuration name
 *   .               | `SetShapeBeautifierConf BUNDLE NAME`       | The bundle name and the configuration name
 *
 * When executing a configuration script a configurer will replace
 * `${[_A-Za-z][_0-9A-Za-z]*}` patterns with the value of the environment
 * variable with the name between the curly braces.
 *
 * By default, the environment contains a single variable named `"lite"`,
 * which value is `""` on desktop platforms and `".lite"` on mobile platforms.
 * We set this so that provided configuration bundles load the `*.lite.res`
 * resources on mobiles and the `*.res` resources on desktops, by default.
 *
 * You can override this behavior by setting the `"lite"` variable to the other
 * value. Of course, this function will allow you to set the `"lite"` variable
 * with any other value but applying standard configurations will then stop
 * working.
 *
 * {@extends voEngineObject}
 * {@implements voIConfigurationManager}
 */
typedef voEngineObject voConfigurationManager;

/**
 * `voConfigurer` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voIConfigurer}
 */
typedef voEngineObject voConfigurer;


/**
 * Callback called by the configuration manager.
 *
 * @param engine the engine.
 * @param charset the charset used to encode parameter strings.
 * @param item the container for the `callback` function results
 * @param userParam an optional user defined parameter passed to the callback.
 *
 * @see voIConfigurationManager
 */
typedef bool (VO_MSE_CALL *voStringListCallback)(voEngine engine, voCharset charset, const voString* item, void* userParam);


/**
 * Lists the different configuration types.
 */
typedef enum _voConfigurationType
{
  VO_INVALID_CONF = -1, /**< The configuration is invalid. */
  VO_UNTYPED_CONF,      /**< An untyped configuration. */
  VO_TEXT_CONF,         /**< A text recognizer or indexer configuration. */
  VO_SHAPE_CONF,        /**< A shape recognizer or beautifier configuration. */
  VO_MATH_CONF,         /**< A math recognizer configuration. */
  VO_MUSIC_CONF,        /**< A music recognizer configuration. */
  VO_ANALYZER_CONF,     /**< An analyzer configuration. */
  VO_GESTURE_CONF,      /**< A gesture configuration. */

  _voConfigurationType_FORCE_INT_SIZE = 0x7FFFFFFF
}
voConfigurationType;

/**
 * Functions composing the `voIConfigurationManager` interface.
 *
 * This interface exposes the public methods of the `voConfigurationManager`
 * type.
 *
 * {@implementingTypes voConfigurationManager}
 */
typedef struct _voIConfigurationManager
{
  /**
   * Clears the search path of this configuration manager.
   *
   * @param engine the engine.
   * @param target the target configuration manager.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a
   *   `voConfigurationManager`.
   */
  bool (VO_MSE_CALL *clearSearchPath)(voEngine engine, voConfigurationManager target);

  /**
   * Adds the specified directory to the search path of this configuration
   * manager.
   *
   * @param engine the engine.
   * @param target the target configuration manager.
   * @param charset the charset used to encode parameter strings.
   * @param directoryName the name of the search directory to be added.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a
   *   `voConfigurationManager`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `directoryName` or its `bytes` member
   *   is not readable.
   */
  bool (VO_MSE_CALL *addSearchDir)(voEngine engine, voConfigurationManager target,
                                   voCharset charset, const voString* directoryName);

  /**
   * Configures the specified configurable object by applying the specified
   * configuration.
   *
   * @param engine the engine.
   * @param target the target configuration manager.
   * @param charset the charset used to encode parameter strings.
   * @param bundleName the name of the configurations bundle.
   * @param configurationName the name of the configuration.
   * @param configurable the object to be configured.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a
   *   `voConfigurationManager`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `bundleName` or its `bytes` member is
   *   not readable.
   * @throws VO_INVALID_ARGUMENT when `bundleName` is the empty string.
   * @throws VO_INVALID_USER_BUFFER when `configurationName` or its `bytes`
   *   member is not readable.
   * @throws VO_INVALID_ARGUMENT when `configurationName` is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `configurable` reference is invalid.
   * @throws VO_INVALID_ARGUMENT when `configurable` is not configurable.
   * @throws VO_CONFIGURATION_BUNDLE_NOT_FOUND when the bundle can't be
   *   resolved.
   * @throws VO_CONFIGURATION_NOT_FOUND when `configurationName` doesn't
   *   correspond to a valid configuration in the bundle.
   * @throws VO_RESOURCE_NOT_FOUND when the configuration refers to non existing
   *   resources.
   * @throws VO_INVALID_CONFIGURATION when the configuration contains syntax
   *   errors.
   */
  bool (VO_MSE_CALL *configure)(voEngine engine, voConfigurationManager target,
                                voCharset charset, const voString* bundleName, const voString* configurationName,
                                voEngineObject configurable);

  /**
   * Creates a configurer to configure the specified configurable object.
   *
   * @param engine the engine.
   * @param target the target configuration manager.
   * @param configurable the object to be configured.
   *
   * @return the newly created configurer on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a
   *   `voConfigurationManager`.
   * @throws VO_NO_SUCH_OBJECT when the `configurable` reference is invalid.
   * @throws VO_INVALID_ARGUMENT when `configurable` is not configurable.
   * @throws VO_LIMIT_EXCEEDED when the number of `voConfigurer` instances would
   *   exceed the allowed limit (please remember that the limit can be shared by
   *   several types).
   */
  voConfigurer (VO_MSE_CALL *createConfigurer)(voEngine engine, voConfigurationManager target, voEngineObject configurable);

  /**
   * Lists the bundles in the search directories added with `addSearchDir()`.
   *
   * @param engine the engine.
   * @param target the target configuration manager.
   * @param charset the charset used to encode parameter strings.
   * @param callback the `callback` function to be called during bundle listing.
   * @param userParam an optional pointer to a user defined parameter that will
   *   be passed back to the `callback` function.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a
   *   `voConfigurationManager`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_CALLBACK when `callback` is invalid.
   */
  bool (VO_MSE_CALL *listBundles)(voEngine engine, voConfigurationManager target,
                                  voCharset charset,
                                  voStringListCallback callback, void* userParam);

  /**
   * Lists the available configurations for the specified bundle.
   *
   * @param engine the engine.
   * @param target the target configuration manager.
   * @param charset the charset used to encode parameter strings.
   * @param bundleName the name of the configurations bundle.
   * @param callback the `callback` function to be called during configuration
   *   listing.
   * @param userParam an optional pointer to a user defined parameter that will
   *   be passed back to the `callback` function.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a
   *   `voConfigurationManager`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `bundleName` or its `bytes` member is
   *   not readable.
   * @throws VO_INVALID_ARGUMENT when `bundleName` is the empty string.
   * @throws VO_CONFIGURATION_BUNDLE_NOT_FOUND when the bundle can't be
   *   resolved.
   * @throws VO_INVALID_CALLBACK when `callback` is invalid.
   */
  bool (VO_MSE_CALL *listConfigurations)(voEngine engine, voConfigurationManager target,
                                        voCharset charset, const voString* bundleName,
                                        voStringListCallback callback, void* userParam);

  /**
   * Returns the type of the specified configuration.
   *
   * @param engine the engine.
   * @param target the target configuration manager.
   * @param charset the charset used to encode parameter strings.
   * @param bundleName the name of the configurations bundle.
   * @param configurationName the name of the configuration.
   *
   * @return the configuration type on success, otherwise `VO_INVALID_CONF`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a
   *   `voConfigurationManager`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `bundleName` or its `bytes` member is
   *   not readable.
   * @throws VO_INVALID_ARGUMENT when `bundleName` is the empty string.
   * @throws VO_CONFIGURATION_BUNDLE_NOT_FOUND when the bundle can't be
   *   resolved.
   * @throws VO_INVALID_USER_BUFFER when `configurationName` or its `bytes`
   *   member is not readable.
   * @throws VO_INVALID_ARGUMENT when `configurationName` is invalid.
   * @throws VO_CONFIGURATION_NOT_FOUND when `configurationName` doesn't
   *   correspond to a valid configuration in the bundle.
   */
  voConfigurationType (VO_MSE_CALL *getConfigurationType)(voEngine engine, voConfigurationManager target,
                                                          voCharset charset, const voString* bundleName, const voString* configurationName);

  /**
   * Returns the current configuration script for a given configuration inside a
   * bundle.
   *
   * @param engine the engine.
   * @param target the target configuration manager.
   * @param charset the charset used to encode parameter strings.
   * @param bundleName the name of the bundle to retrieve information from.
   * @param configurationName the name of the configuration in the given bundle.
   * @param configurationScript the recipient for the configuration script.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a
   *   `voConfigurationManager`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `bundleName` or its `bytes` member is
   *   not readable.
   * @throws VO_INVALID_ARGUMENT when `bundleName` is the empty string.
   * @throws VO_CONFIGURATION_BUNDLE_NOT_FOUND when the bundle can't be
   *   resolved.
   * @throws VO_INVALID_USER_BUFFER when `configurationName` or its `bytes`
   *   member is not readable.
   * @throws VO_INVALID_ARGUMENT when `configurationName` is invalid.
   * @throws VO_CONFIGURATION_NOT_FOUND when `configurationName` doesn't
   *   correspond to a valid configuration in the bundle.
   * @throws VO_INVALID_USER_BUFFER when `configurationScript` or its `bytes`
   *   member is not writable.
   */
  bool (VO_MSE_CALL *getConfigurationScript)(voEngine engine, voConfigurationManager target,
                                             voCharset charset, const voString* bundleName, const voString* configurationName,
                                             voString* configurationScript);

  /**
   * Sets the environment variable identified by name with the specified value.
   * Variable names must match `[_A-Za-z][_0-9A-Za-z]*`.
   *
   * @param engine the engine.
   * @param target the target configuration manager.
   * @param charset the charset used to encode parameter strings.
   * @param name the name of the variable to set.
   * @param value the value to set.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a
   *   `voConfigurationManager`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `name` or its `bytes` member is not
   *   readable.
   * @throws VO_INVALID_ARGUMENT when `name` doesn't match
   *   `[_A-Za-z][_0-9A-Za-z]*`.
   * @throws VO_INVALID_USER_BUFFER when `value` or its `bytes` member is not
   *   readable.
   */
  bool (VO_MSE_CALL *setEnv)(voEngine engine, voConfigurationManager target,
                             voCharset charset, const voString* name, const voString* value);

  /**
   * Returns the value of the environment variable identified by name. If no
   * variable has been set with the specified name an empty string is returned.
   *
   * @param engine the engine.
   * @param target the target configuration manager.
   * @param charset the charset used to encode parameter strings.
   * @param name the name of the variable to be retrieved.
   * @param value the recipient for the value.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a
   *   `voConfigurationManager`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `name` or its `bytes` member is not
   *   readable.
   * @throws VO_INVALID_ARGUMENT when `name` doesn't match
   *   `[_A-Za-z][_0-9A-Za-z]*`.
   * @throws VO_INVALID_USER_BUFFER when `value` or its `bytes` member is not
   *   writable.
   */
  bool (VO_MSE_CALL *getEnv)(voEngine engine, voConfigurationManager target,
                             voCharset charset, const voString* name, voString* value);
}
voIConfigurationManager;

#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voConfigurationManager` instance.
 *
 * @param engine the engine.
 *
 * @return the newly created `voConfigurationManager` instance on success,
 *   otherwise `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_LIMIT_EXCEEDED when the number of `voConfigurationManager`
 *   instances would exceed the allowed limit (please remember that the limit
 *   can be shared by several types).
 */
VO_INLINE voConfigurationManager voCreateConfigurationManager(voEngine engine)
{
  return voCreateObject(engine, VO_ConfigurationManager);
}
#endif // end of: #ifndef VO_NO_FUNCTIONS

/**
 * Functions composing the `voIConfigurer` interface.
 *
 * This interface exposes the public methods of the `voConfigurer` type.
 *
 * {@implementingTypes voConfigurer}
 */
typedef struct _voIConfigurer
{
  /**
   * Returns the configuration manager that created this configurer.
   *
   * @param engine the engine.
   * @param target the target configurer.
   *
   * @return the configuration manager on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when the `target` is not a `voConfigurer`.
   * @throws VO_LIMIT_EXCEEDED when the number of `voConfigurationManager`
   *   instances would exceed the allowed limit (please remember that the limit
   *   can be shared by several types).
   */
  voConfigurationManager (VO_MSE_CALL *getConfigurationManager)(voEngine engine, voConfigurer target);

  /**
   * Returns the configurable object to which this configurer applies.
   *
   * @param engine the engine.
   * @param target the target configurer.
   *
   * @return the configurable object on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when the `target` is not a `voConfigurer`.
   * @throws VO_LIMIT_EXCEEDED when the number of instances of the specified
   *   type would exceed the allowed limit (please remember that the limit can
   *   be shared by several types).
   */
  voEngineObject (VO_MSE_CALL *getConfigurable)(voEngine engine, voConfigurer target);

  /**
   * Applies the specified configuration.
   *
   * @param engine the engine.
   * @param target the target configurer.
   * @param charset the charset used to encode parameter strings.
   * @param bundleName the name of the configurations bundle.
   * @param configurationName the name of the configuration.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voConfigurer`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `bundleName` or its `bytes` member is
   *   not readable.
   * @throws VO_INVALID_ARGUMENT when `bundleName` is the empty string.
   * @throws VO_INVALID_USER_BUFFER when `configurationName` or its `bytes`
   *   member is not readable.
   * @throws VO_INVALID_ARGUMENT when `configurationName` is the empty string.
   * @throws VO_CONFIGURATION_BUNDLE_NOT_FOUND when the bundle can't be
   *   resolved.
   * @throws VO_CONFIGURATION_NOT_FOUND when `configurationName` doesn't
   *   correspond to a valid configuration in the bundle.
   * @throws VO_RESOURCE_NOT_FOUND when the configuration refers to non existing
   *   resources.
   * @throws VO_INVALID_CONFIGURATION when the configuration contains syntax
   *   errors.
   */
  bool (VO_MSE_CALL *apply)(voEngine engine, voConfigurer target,
                            voCharset charset, const voString* bundleName, const voString* configurationName);

  /**
   * Executes the specified configuration script.
   *
   * @param engine the engine.
   * @param target the target configurer.
   * @param charset the charset used to encode parameter strings.
   * @param configurationScript the configuration script to be executed.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voConfigurer`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `configurationScript` or its `bytes`
   *   member is not readable.
   * @throws VO_RESOURCE_NOT_FOUND when the configuration refers to non existing
   *   resources.
   * @throws VO_INVALID_CONFIGURATION when the configuration contains syntax
   *   errors.
   */
  bool (VO_MSE_CALL *exec)(voEngine engine, voConfigurer target,
                           voCharset charset, const voString* configurationScript);

  /**
   * Retrieves error messages emitted by previous `apply()` and/or `exec()`
   * calls, when the involved script contain errors.
   *
   * @param engine the engine.
   * @param target the target configuration manager.
   * @param charset the charset used to encode parameter strings.
   * @param messages the recipient for the messages.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voConfigurer`.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `charset` reference is `NULL` and no
   *   default charset was set.
   * @throws VO_INVALID_OBJECT when `charset` is not a `voCharset`.
   * @throws VO_INVALID_USER_BUFFER when `messages` or its `bytes` member is not
   *   writable.
   */
  bool (VO_MSE_CALL *getMessages)(voEngine engine, voConfigurer target,
                                  voCharset charset, voString* messages);
  /**
   * Clears the error messages.
   *
   * @param engine the engine.
   * @param target the target configuration manager.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voConfigurer`.
   */
  bool (VO_MSE_CALL *clearMessages)(voEngine engine, voConfigurer target);
}
voIConfigurer;

#endif // end of: #ifndef C_MYSCRIPTCONFIGURATIONMANAGER_H
