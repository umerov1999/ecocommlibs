#ifndef C_MYSCRIPTGESTURE_H
#define C_MYSCRIPTGESTURE_H 0x08010000
/**
 * @file c/MyScriptGesture.h
 * Native interface to the MyScript Gesture service.
 */

#include "MyScriptEngine.h"
#include "MyScriptInk.h"
#include "MyScriptConfigurationManager.h"


// -- types and interfaces -----------------------------------------------------

/**
 * Lists the MyScript Gesture types.
 */
enum VO_GESTURE_T
{
  /**
   * `voInkSampler` type identifier.
   *
   * @see voInkSampler
   */
  VO_InkSampler = 3300,

  /**
   * `voGestureRecognizer` type identifier.
   *
   * @see voGestureRecognizer
   */
  VO_GestureRecognizer,

  /**
   * `voGestureResults` type identifier.
   *
   * @see voGestureResults
   */
  VO_GestureResults,

  /**
   * `voGestureResult` type identifier.
   *
   * @see voGestureResult
   */
  VO_GestureResult,

  /**
   * `voGestureRecognizerProperties` type identifier.
   *
   * @see voGestureRecognizerProperties
   */
  VO_GestureRecognizerProperties,
};


/**
 * Lists the MyScript Gesture interfaces.
 */
enum VO_GESTURE_I
{
  /**
   * `voIInkSampler` interface identifier.
   *
   * @see voIInkSampler
   */
  VO_IInkSampler = 3320,

  /**
   * `voIInkSamplerListener` interface identifier.
   *
   * @see voIInkSamplerListener
   */
  VO_IInkSamplerListener,

  /**
   * `voIGestureRecognizer` interface identifier.
   *
   * @see voIGestureRecognizer
   */
  VO_IGestureRecognizer,

  /**
   * `voIGestureResults` interface identifier.
   *
   * @see voIGestureResults
   */
  VO_IGestureResults,

  /**
   * `voIGestureResult` interface identifier.
   *
   * @see voIGestureResult
   */
  VO_IGestureResult,

  /**
   * `voIGestureHandler` interface identifier.
   *
   * @see voIGestureHandler
   */
  VO_IGestureHandler,
};


// -----------------------------------------------------------------------------

/**
 * `voInkSampler` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voIInkSampler, voIInkStrokeSet}
 */
typedef voEngineObject voInkSampler;

/**
 * `voGestureRecognizer` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voIAttachTarget, voIGestureRecognizer}
 */
typedef voEngineObject voGestureRecognizer;

/**
 * `voGestureRecognizerProperties` reference type.
 *
 * {@extends voEngineObject}
 */
typedef voEngineObject voGestureRecognizerProperties;

/**
 * `voGestureResults` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voIGestureResults}
 */
typedef voEngineObject voGestureResults;

/**
 * `voGestureResult` reference type.
 *
 * {@extends voEngineObject}
 * {@implements voIGestureResult}
 */
typedef voEngineObject voGestureResult;


// -- InkSampler ---------------------------------------------------------------

/**
 * Represents the identifier of a pointing device. The value has no special
 * meaning.
 */
typedef int64_t voDeviceId;

/**
 * Represents the identifier of a pointer. The value has no special meaning.
 */
typedef int64_t voPointerId;


/**
 * Lists the types of devices.
 *
 * @see voInkSampler
 */
typedef enum _voDeviceType
{
  /**
   * The type of the pointing device is unknown.
   */
  VO_DEVICE_UNKNOWN,

  /**
   * The pointing device is a mouse.
   */
  VO_DEVICE_MOUSE,

  /**
   * The pointing device is a touch screen.
   */
  VO_DEVICE_TOUCH_SCREEN,

  /**
   * The pointing device is a graphics tablet.
   */
  VO_DEVICE_GRAPHICS_TABLET,

  /**
   * The pointing device is a digital pen.
   */
  VO_DEVICE_DIGITAL_PEN,

  _voDeviceType_FORCE_INT_SIZE = 0x7FFFFFFF
}
voDeviceType;

/**
 * Lists the types of pointers.
 *
 * @see voInkSampler
 */
typedef enum _voPointerType
{
  /**
   * The type of the pointer is unknown.
   */
  VO_POINTER_UNKNOWN,

  /**
   * The pointer is a mouse or puck.
   */
  VO_POINTER_CURSOR,

  /**
   * The pointer is a pen. It may be a digital pen or the pen end of a stylus,
   * either on a graphics tablet or touch screen.
   */
  VO_POINTER_PEN,

  /**
   * The pointer is the eraser end of a stylus, either on a tablet or touch
   * screen.
   */
  VO_POINTER_ERASER,

  /**
   * The pointer is a touch. It may be either a finger or a conductive stylus.
   */
  VO_POINTER_TOUCH,

  _voPointerType_FORCE_INT_SIZE = 0x7FFFFFFF
}
voPointerType;

/**
 * Holds the parameters required to create a new `voInkSampler` instance.
 *
 * @see voInkSampler, voCreateInkSampler
 */
typedef struct _voInkSamplerInitializer
{
  /**
   * The target model. Either a `voInk` object or a `voPage` object.
   *
   * @note When the target object is a `voInk` object the supported gestures are
   *   very limited because context analysis has little relevant information.
   *
   * @see voInk, voPage
   */
  voEngineObject target;
}
voInkSamplerInitializer;


#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voInkSampler` instance.
 *
 * @param engine the engine.
 * @param target the target model. Either a `voInk` object or a `voPage` object.
 *
 * @note When the target object is a `voInk` object the supported gestures are
 *   very limited because context analysis has little relevant information.
 *
 * @return the newly created `voInkSampler` instance on success, otherwise
 *   `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
 * @throws VO_INVALID_OPERATION when `target` is not a `voInk` or `voPage`.
 * @throws VO_LIMIT_EXCEEDED when the number of `voInkSampler` instances would
 *   exceed the allowed limit (please remember that the limit can be shared by
 *   several types).
 */
VO_INLINE voInkSampler voCreateInkSampler(voEngine engine, voEngineObject target)
{
  voInkSamplerInitializer initializer = { target };
  return voCreateObjectEx(engine, VO_InkSampler, &initializer, sizeof(initializer));
}
#endif // end of: #ifndef VO_NO_FUNCTIONS

/**
 * {@propertySet InkSampler}
 */
enum VO_INKSAMPLER_PROP
{
  /**
   * The current model view transform.
   *
   * You should update this property whenever your application changes its view
   * transform, especially for zooming operations.
   *
   * The default value is the identity matrix. The transform must contain valid
   * values and be invertible.
   *
   * {@property voTransform}
   */
  VO_INKSAMPLER_VIEW_TRANSFORM = (VO_InkSampler << 16),

  /**
   * The current pen width.
   *
   * You should update this property whenever your application changes the
   * nominal width of the pen for new input ink.
   *
   * The default value is `1`. The value must be greater than `0`.
   *
   * {@property float}
   */
  VO_INKSAMPLER_PEN_WIDTH
};

/**
 * Functions composing the `voIInkSampler` interface.
 *
 * This interface exposes the public methods of the `voInkSampler` type.
 *
 * {@implementingTypes voInkSampler}
 */
typedef struct _voIInkSampler
{
  /**
   * Adds the specified listener to this ink sampler.
   *
   * @param engine the engine.
   * @param target the target ink sampler.
   * @param listener the ink sampler listener to add.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voInkSampler`.
   * @throws VO_NO_SUCH_OBJECT when the `listener` reference is invalid.
   * @throws VO_INVALID_OBJECT when `listener` does not implement the
   *   `voIInkSamplerListener` interface.
   * @throws VO_INVALID_STATE when `listener` is already registered.
   */
  bool (VO_MSE_CALL *addListener)(voEngine engine, voInkSampler target, voUserObject listener);

  /**
   * Removes the specified listener from this ink sampler.
   *
   * @param engine the engine.
   * @param target the target ink sampler.
   * @param listener the listener to be removed.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voInkSampler`.
   * @throws VO_INVALID_OBJECT when `listener` does not implement the
   *   `voIInkSamplerListener` interface.
   * @throws VO_INVALID_STATE when `listener` is not registered.
   */
  bool (VO_MSE_CALL *removeListener)(voEngine engine, voInkSampler target, voUserObject listener);

  /**
   * Sets the specified gesture recognizer as the handler of this ink sampler.
   *
   * @param engine the engine.
   * @param target the target ink sampler.
   * @param handler the handler to be set. It may be `NULL`, in which case the
   *   previous handler is removed.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voInkSampler`.
   * @throws VO_INVALID_OBJECT when `handler` is not a `voGestureRecognizer`.
   */
  bool (VO_MSE_CALL *setHandler)(voEngine engine, voInkSampler target,
                                 voGestureRecognizer handler);

  /**
   * Tells whether the specified device has already been registered.
   *
   * @param engine the engine.
   * @param target the target ink sampler.
   * @param deviceId the id of the device.
   *
   * @return `VO_YES` or `VO_NO` on success, otherwise `VO_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voInkSampler`.
   */
  voYesNo (VO_MSE_CALL *isDeviceRegistered)(voEngine engine, voInkSampler target,
                                            voDeviceId deviceId);

  /**
   * Registers a device to this ink sampler.
   *
   * @param engine the engine.
   * @param target the target ink sampler.
   * @param deviceId the id of the device to be registered.
   * @param deviceType the type of the device to be registered.
   * @param strokeFormat the associated stroke format of the device to be
   *   registered.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voInkSampler`.
   * @throws VO_INVALID_ARGUMENT when `deviceType` is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `strokeFormat` reference is invalid.
   * @throws VO_INVALID_OBJECT when `strokeFormat` is not a `voInkStrokeFormat`.
   * @throws VO_INVALID_ARGUMENT when `strokeFormat` does not contain X/Y/T
   *   channels.
   * @throws VO_INVALID_STATE when `deviceId` is already registered.
   */
  bool (VO_MSE_CALL *registerDevice)(voEngine engine, voInkSampler target,
                                     voDeviceId deviceId, voDeviceType deviceType,
                                     voInkStrokeFormat strokeFormat);

  /**
   * Tells whether the specified pointer has already been registered.
   *
   * @param engine the engine.
   * @param target the target ink sampler.
   * @param deviceId the id of the device.
   * @param pointerId the id or the pointer.
   *
   * @return `VO_YES` or `VO_NO` on success, otherwise `VO_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voInkSampler`.
   * @throws VO_INVALID_ARGUMENT when `deviceId` is not registered.
   */
  voYesNo (VO_MSE_CALL *isPointerRegistered)(voEngine engine, voInkSampler target,
                                             voDeviceId deviceId, voPointerId pointerId);

  /**
   * Registers a pointer to this ink sampler.
   *
   * @param engine the engine.
   * @param target the target ink sampler.
   * @param deviceId the id of the device associated to the pointer.
   * @param pointerId the id of the pointer to be registered.
   * @param pointerType the type of the pointer to be registered.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voInkSampler`.
   * @throws VO_INVALID_ARGUMENT when `deviceId` is not registered.
   * @throws VO_INVALID_STATE when `pointerId` is already registered.
   */
  bool (VO_MSE_CALL *registerPointer)(voEngine engine, voInkSampler target,
                                      voDeviceId deviceId, voPointerId pointerId,
                                      voPointerType pointerType);

  /**
   * Sends a pointer down event to this ink sampler.
   *
   * @param engine the engine.
   * @param target the target ink sampler.
   * @param deviceId the id of the device.
   * @param pointerId the id of the pointer.
   * @param signature the description of a sample, with one character per
   *   channel. For example "FFI" means following parameters will be float,
   *   float and int. Signature must match the channel settings of the stroke
   *   format.
   * @param ... the sample values corresponding to `signature`.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voInkSampler`.
   * @throws VO_INVALID_ARGUMENT when `deviceId` is not registered.
   * @throws VO_INVALID_ARGUMENT when `pointerId` is not registered.
   * @throws VO_INVALID_USER_BUFFER when `signature` is not readable.
   * @throws VO_INVALID_ARGUMENT when the length of `signature` is less than 2
   *   characters (we need at least X and Y positions).
   * @throws VO_INVALID_STATE when `pointerDown()` has already been called.
   * @throws VO_NOT_PERMITTED_FROM_THIS_THREAD when the call to `pointerDown()`
   *   was made from a different thread than the one `target` was created in.
   */
  bool (VO_MSE_CALL *pointerDown)(voEngine engine, voInkSampler target,
                                  voDeviceId deviceId, voPointerId pointerId,
                                  const char* signature, ...);

  /**
   * Sends a pointer down event to this ink sampler.
   *
   * @param engine the engine.
   * @param target the target ink sampler.
   * @param deviceId the id of the device.
   * @param pointerId the id of the pointer.
   * @param signature the description of a sample, with one character per
   *   channel. For example "FFI" means following parameters will be float,
   *   float and int. Signature must match the channel settings of the stroke
   *   format.
   * @param args the sample values corresponding to `signature`.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voInkSampler`.
   * @throws VO_INVALID_ARGUMENT when `deviceId` is not registered.
   * @throws VO_INVALID_ARGUMENT when `pointerId` is not registered.
   * @throws VO_INVALID_USER_BUFFER when `signature` is not readable.
   * @throws VO_INVALID_ARGUMENT when the length of `signature` is less than 2
   *   characters (we need at least X and Y positions).
   * @throws VO_INVALID_STATE when `pointerDown()` has already been called.
   * @throws VO_NOT_PERMITTED_FROM_THIS_THREAD when the call to `pointerDown()`
   *   was made from a different thread than the one `target` was created in.
   */
  bool (VO_MSE_CALL *pointerDownV)(voEngine engine, voInkSampler target,
                                   voDeviceId deviceId, voPointerId pointerId,
                                   const char* signature, va_list args);

  /**
   * Sends a pointer down event to this ink sampler.
   *
   * @param engine the engine.
   * @param target the target ink sampler.
   * @param deviceId the id of the device.
   * @param pointerId the id of the pointer.
   * @param values a table containing the new sample channel values.
   * @param count the number of channels in `values`.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voInkSampler`.
   * @throws VO_INVALID_ARGUMENT when `deviceId` is not registered.
   * @throws VO_INVALID_ARGUMENT when `pointerId` is not registered.
   * @throws VO_INVALID_USER_BUFFER when `values` is not readable.
   * @throws VO_INVALID_ARGUMENT when `count` is less than 2 (we need at least X
   *    and Y positions).
   * @throws VO_INVALID_STATE when `pointerDown()` has already been called.
   * @throws VO_NOT_PERMITTED_FROM_THIS_THREAD when the call to `pointerDown()`
   *   was made from a different thread than the one `target` was created in.
   */
  bool (VO_MSE_CALL *pointerDownA_f)(voEngine engine, voInkSampler target,
                                     voDeviceId deviceId, voPointerId pointerId,
                                     const float values[], int32_t count);

  /**
   * Sends a pointer move event to this ink sampler.
   *
   * @param engine the engine.
   * @param target the target ink sampler.
   * @param deviceId the id of the device.
   * @param pointerId the id of the pointer.
   * @param signature the description of a sample, with one character per
   *   channel. For example "FFI" means following parameters will be float,
   *   float and int. Signature must match the channel settings of the stroke
   *   format.
   * @param ... the sample values corresponding to `signature`.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voInkSampler`.
   * @throws VO_INVALID_ARGUMENT when `deviceId` is not registered.
   * @throws VO_INVALID_ARGUMENT when `pointerId` is not registered.
   * @throws VO_INVALID_USER_BUFFER when `signature` is not readable.
   * @throws VO_INVALID_ARGUMENT when the length of `signature` is less than 2
   *   characters (we need at least X and Y positions).
   * @throws VO_INVALID_STATE when `pointerDown()` has not been called before.
   * @throws VO_NOT_PERMITTED_FROM_THIS_THREAD when the call to `pointerMove()`
   *   was made from a different thread than the one `target` was created in.
   */
  bool (VO_MSE_CALL *pointerMove)(voEngine engine, voInkSampler target,
                                  voDeviceId deviceId, voPointerId pointerId,
                                  const char* signature, ...);

  /**
   * Sends a pointer move event to this ink sampler.
   *
   * @param engine the engine.
   * @param target the target ink sampler.
   * @param deviceId the id of the device.
   * @param pointerId the id of the pointer.
   * @param signature the description of a sample, with one character per
   *   channel. For example "FFI" means following parameters will be float,
   *   float and int. Signature must match the channel settings of the stroke
   *   format.
   * @param args the sample values corresponding to `signature`.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voInkSampler`.
   * @throws VO_INVALID_ARGUMENT when `deviceId` is not registered.
   * @throws VO_INVALID_ARGUMENT when `pointerId` is not registered.
   * @throws VO_INVALID_USER_BUFFER when `signature` is not readable.
   * @throws VO_INVALID_ARGUMENT when the length of `signature` is less than 2
   *   characters (we need at least X and Y positions).
   * @throws VO_INVALID_STATE when `pointerDown()` has not been called before.
   * @throws VO_NOT_PERMITTED_FROM_THIS_THREAD when the call to `pointerMove()`
   *   was made from a different thread than the one `target` was created in.
   */
  bool (VO_MSE_CALL *pointerMoveV)(voEngine engine, voInkSampler target,
                                   voDeviceId deviceId, voPointerId pointerId,
                                   const char* signature, va_list args);

  /**
   * Sends a pointer move event to this ink sampler.
   *
   * @param engine the engine.
   * @param target the target ink sampler.
   * @param deviceId the id of the device.
   * @param pointerId the id of the pointer.
   * @param values a table containing the new sample channel values.
   * @param count the number of channels in `values`.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voInkSampler`.
   * @throws VO_INVALID_ARGUMENT when `deviceId` is not registered.
   * @throws VO_INVALID_ARGUMENT when `pointerId` is not registered.
   * @throws VO_INVALID_USER_BUFFER when `values` is not readable.
   * @throws VO_INVALID_ARGUMENT when `count` is less than 2 (we need at least X
   *    and Y positions).
   * @throws VO_INVALID_STATE when `pointerDown()` has not been called before.
   * @throws VO_NOT_PERMITTED_FROM_THIS_THREAD when the call to `pointerMove()`
   *   was made from a different thread than the one `target` was created in.
   */
  bool (VO_MSE_CALL *pointerMoveA_f)(voEngine engine, voInkSampler target,
                                     voDeviceId deviceId, voPointerId pointerId,
                                     const float values[], int32_t count);

  /**
   * Sends a pointer up event to this ink sampler.
   *
   * @note The `signature` parameter may be `NULL` or empty to indicate that the
   *   device or platform doesn't give positioning information for pointer up
   *   events.
   *
   * @param engine the engine.
   * @param target the target ink sampler.
   * @param deviceId the id of the device.
   * @param pointerId the id of the pointer.
   * @param signature the description of a sample, with one character per
   *   channel. For example "FFI" means following parameters will be float,
   *   float and int. Signature must match the channel settings of the stroke
   *   format.
   * @param ... the sample values corresponding to `signature`.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voInkSampler`.
   * @throws VO_INVALID_ARGUMENT when `deviceId` is not registered.
   * @throws VO_INVALID_ARGUMENT when `pointerId` is not registered.
   * @throws VO_INVALID_ARGUMENT when the length of `signature` is less than 2
   *   characters (we need at least X and Y positions).
   * @throws VO_INVALID_STATE when `pointerDown()` has not been called before.
   * @throws VO_NOT_PERMITTED_FROM_THIS_THREAD when the call to `pointerUp()`
   *   was made from a different thread than the one `target` was created in.
   */
  bool (VO_MSE_CALL *pointerUp)(voEngine engine, voInkSampler target,
                                voDeviceId deviceId, voPointerId pointerId,
                                const char* signature, ...);

  /**
   * Sends a pointer up event to this ink sampler.
   *
   * @note The `signature` parameter may be `NULL` or empty to indicate that the
   *   device or platform doesn't give positioning information for pointer up
   *   events.
   *
   * @param engine the engine.
   * @param target the target ink sampler.
   * @param deviceId the id of the device.
   * @param pointerId the id of the pointer.
   * @param signature the description of a sample, with one character per
   *   channel. For example "FFI" means following parameters will be float,
   *   float and int. Signature must match the channel settings of the stroke
   *   format.
   * @param args the sample values corresponding to `signature`.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voInkSampler`.
   * @throws VO_INVALID_ARGUMENT when `deviceId` is not registered.
   * @throws VO_INVALID_ARGUMENT when `pointerId` is not registered.
   * @throws VO_INVALID_ARGUMENT when the length of `signature` is less than 2
   *   characters (we need at least X and Y positions).
   * @throws VO_INVALID_STATE when `pointerDown()` has not been called before.
   * @throws VO_NOT_PERMITTED_FROM_THIS_THREAD when the call to `pointerUp()`
   *   was made from a different thread than the one `target` was created in.
   */
  bool (VO_MSE_CALL *pointerUpV)(voEngine engine, voInkSampler target,
                                 voDeviceId deviceId, voPointerId pointerId,
                                 const char* signature, va_list args);

  /**
   * Sends a pointer up event to this ink sampler.
   *
   * @param engine the engine.
   * @param target the target ink sampler.
   * @param deviceId the id of the device.
   * @param pointerId the id of the pointer.
   * @param values a table containing the new sample channel values.
   * @param count the number of channels in `values`.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voInkSampler`.
   * @throws VO_INVALID_ARGUMENT when `deviceId` is not registered.
   * @throws VO_INVALID_ARGUMENT when `pointerId` is not registered.
   * @throws VO_INVALID_USER_BUFFER when `values` is not readable.
   * @throws VO_INVALID_ARGUMENT when `count` is less than 2 (we need at least X
   *    and Y positions).
   * @throws VO_INVALID_STATE when `pointerDown()` has not been called before.
   * @throws VO_NOT_PERMITTED_FROM_THIS_THREAD when the call to `pointerUp()`
   *   was made from a different thread than the one `target` was created in.
   */
  bool (VO_MSE_CALL *pointerUpA_f)(voEngine engine, voInkSampler target,
                                   voDeviceId deviceId, voPointerId pointerId,
                                   const float values[], int32_t count);

  /**
   * Cancels an ongoing pointer trace in this ink sampler.
   *
   * @note What we call an ongoing pointer trace here is a sequence of pointer
   *   events starting with a `pointerDown()` and followed by zero or more
   *   `pointerMove()` events with the same `deviceId` and `pointerId`.
   *
   * @param engine the engine.
   * @param target the target ink sampler.
   * @param deviceId the id of the device.
   * @param pointerId the id of the pointer.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voInkSampler`.
   * @throws VO_INVALID_ARGUMENT when `deviceId` is not registered.
   * @throws VO_INVALID_ARGUMENT when `pointerId` is not registered.
   * @throws VO_NOT_PERMITTED_FROM_THIS_THREAD when the call to
   *   `pointerCancel()` was made from a different thread than the one `target`
   *   was created in.
   */
  bool (VO_MSE_CALL *pointerCancel)(voEngine engine, voInkSampler target,
                                    voDeviceId deviceId, voPointerId pointerId);

  /**
   * Clear all the strokes contained in this ink sampler.
   *
   * @note This operation is only authorized if there is no handler set on the
   *   target.
   *
   * @param engine the engine.
   * @param target the target ink sampler.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voInkSampler`.
   * @throws VO_INVALID_STATE when `target` has a handler attached.
   */
  bool (VO_MSE_CALL *clearStrokes)(voEngine engine, voInkSampler target);

  /**
   * Detaches a stroke from this ink sampler (e.g.: so that it can be added to
   * the target ink or page).
   *
   * @param engine the engine.
   * @param target the target ink sampler.
   * @param index the index of the stroke to be detached.
   *
   * @return the stroke on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voInkSampler`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_INVALID_STATE when the stroke has already been detached.
   * @throws VO_LIMIT_EXCEEDED when the number of `voInkStroke` instances would
   *   exceed the allowed limit (please remember that the limit can be shared by
   *   several types).
   */
  voInkStroke (VO_MSE_CALL *detachStrokeAt)(voEngine engine, voInkSampler target, int32_t index);
}
voIInkSampler;


// -- IInkSamplerListener ------------------------------------------------------

/**
 * Functions composing the `voIInkSamplerListener` interface.
 *
 * @note This interface is implementable by user objects. User code is not
 *   allowed to invoke this interface directly: as a consequence
 *   `voGetInterface()` returns `NULL`.
 */
typedef struct _voIInkSamplerListener
{
  /**
   * Called when the target ink sampler listener is added to an ink sampler.
   *
   * @param engine the engine.
   * @param target the target ink sampler listener.
   * @param sampler the ink sampler to which the ink sampler listener was added.
   */
  void (VO_MSE_CALL *added)(voEngine engine, voEngineObject target, voInkSampler sampler);

  /**
   * Called when the target ink sampler listener is removed from an ink sampler.
   *
   * @param engine the engine.
   * @param target the target ink sampler listener.
   * @param sampler the ink sampler to which the ink sampler listener was
   *   removed.
   */
  void (VO_MSE_CALL *removed)(voEngine engine, voEngineObject target, voInkSampler sampler);

  /**
   * Called when a sample is added to an ink sampler.
   *
   * @param engine the engine.
   * @param target the target ink sampler listener.
   * @param strokeIndex the index of the stroke in which the sample has been
   *   added.
   * @param sampleIndex the index of the sample in the stroke. A value of `0`
   *   means the stroke at index `strokeIndex` was created.
   */
  void (VO_MSE_CALL *onSampleAdded)(voEngine engine, voEngineObject target,
                                    int32_t strokeIndex, int32_t sampleIndex);

  /**
   * Called when a stroke is done in an ink sampler.
   *
   * At this time the stroke at the specified index changes from a
   *   `voInkStrokeBuilder` to the `voInkStroke` created from it.
   *
   * @param engine the engine.
   * @param target the target ink sampler listener.
   * @param strokeIndex the index of the stroke.
   */
  void (VO_MSE_CALL *onStrokeDone)(voEngine engine, voEngineObject target,
                                   int32_t strokeIndex);

  /**
   * Called when a stroke is detached from an ink sampler.
   *
   * @param engine the engine.
   * @param target the target ink sampler listener.
   * @param strokeIndex the index of the stroke that has been detached.
   */
  void (VO_MSE_CALL *onStrokeDetached)(voEngine engine, voEngineObject target,
                                       int32_t strokeIndex);
}
voIInkSamplerListener;


// -- Types of Gestures --------------------------------------------------------

/**
 * Lists the gesture intents.
 */
typedef enum _voGestureIntent
{
  /**
   * Value returned by the `getIntent()` function to indicate an error.
   */
  VO_GESTURE_INTENT_ERR = -1,

  /**
   * The intent is to add content.
   *
   * The application should add the associated strokes to the target model.
   *
   * The associated strokes are given by `getFirstStroke()` and
   * `getLastStroke()`.
   *
   * @note You have to detach the strokes from the ink sampler before adding
   *   them to the target model.
   *
   * @see voIGestureResult::getFirstStroke, voIGestureResult::getLastStroke
   */
  VO_GESTURE_INTENT_ADD,

  /**
   * The intent is to erase content.
   *
   * The application should erase the associated selection.
   *
   * @see voIGestureResult::getSelection
   */
  VO_GESTURE_INTENT_ERASE,

  /**
   * The intent is to select content.
   *
   * The application should update its current selection with the associated
   * selection (e.g. using combine).
   *
   * @see voIGestureResult::getSelection, voISelection::combine
   */
  VO_GESTURE_INTENT_SELECT,

  /**
   * The intent is to insert space.
   *
   * The application should translate the after selection in order to make room
   * between it and the before selection.
   *
   * The amount of space to add as well as its direction depend on the gesture
   * type.
   *
   * If the insert gesture is on a typeset space it will be returned into the
   * associated selection.
   *
   * @see voIGestureResult::getAfterSelection, voIGestureResult::getBeforeSelection, voIGestureResult::getSelection
   */
  VO_GESTURE_INTENT_INSERT,

  /**
   * The intent is to join two items from the content (e.g.: two words).
   *
   * The application should translate the after selection in order to remove the
   * space between it and the before selection.
   *
   * Additionally, the associated selection may be non-empty, in which case it
   * should be erased (e.g.: if you scratch out a typeset space).
   *
   * @see voIGestureResult::getAfterSelection, voIGestureResult::getBeforeSelection, voIGestureResult::getSelection
   */
  VO_GESTURE_INTENT_JOIN,

  /**
   * The intent is to decorate content.
   *
   * The type of decoration is given by the gesture type (e.g. underline).
   */
  VO_GESTURE_INTENT_DECORATE,

  /**
   * The intent is to notify for tap events.
   *
   * The type of tap is given by the gesture type (e.g. double tap).
   */
  VO_GESTURE_INTENT_TAP,

  /**
   * Value used to enable/disable all intents in a row.
   */
  VO_GESTURE_INTENT_ALL = ((int32_t)(0x7FFFFFFF) - 1),

  _voGestureIntent_FORCE_INT_SIZE = 0x7FFFFFFF
}
voGestureIntent;

/**
 * Lists the gesture types.
 */
typedef enum _voGestureType
{
  /**
   * Value returned by the `getType()` function to indicate an error.
   */
  VO_GESTURE_TYPE_ERR = -1,

  /**
   * Not a gesture.
   */
  VO_GESTURE_TYPE_NONE,

  /**
   * A scratch out gesture.
   */
  VO_GESTURE_TYPE_SCRATCH_OUT,

  /**
   * A strike through gesture.
   */
  VO_GESTURE_TYPE_STRIKE_THROUGH,

  /**
   * A double strike through gesture.
   */
  VO_GESTURE_TYPE_DOUBLE_STRIKE_THROUGH,

  /**
   * A cross out gesture.
   */
  VO_GESTURE_TYPE_CROSS_OUT,

  /**
   * A pig-tail out gesture.
   */
  VO_GESTURE_TYPE_PIGTAIL_OUT,

  /**
   * An elliptic surrounding gesture.
   */
  VO_GESTURE_TYPE_ELLIPSE,

  /**
   * A free form surrounding gesture.
   */
  VO_GESTURE_TYPE_FREE_FORM,

  /**
   * An underline.
   */
  VO_GESTURE_TYPE_UNDERLINE,

  /**
   * A double underline.
   */
  VO_GESTURE_TYPE_DOUBLE_UNDERLINE,

  /**
   * An I-beam cursor gesture.
   */
  VO_GESTURE_TYPE_I_BEAM,

  /**
   * A V shaped insertion gesture.
   */
  VO_GESTURE_TYPE_V_INSERT,

  /**
   * A return gesture.
   */
  VO_GESTURE_TYPE_RETURN,

  /**
   * An arc join gesture.
   */
  VO_GESTURE_TYPE_ARC_JOIN,

  /**
   * A tap gesture.
   */
  VO_GESTURE_TYPE_TAP,

  /**
   * A double tap gesture.
   */
  VO_GESTURE_TYPE_DOUBLE_TAP,

  /**
   * A long press gesture.
   */
  VO_GESTURE_TYPE_LONG_PRESS,

  /**
   * An overwrite gesture.
   */
  VO_GESTURE_TYPE_OVERWRITE_STROKES,

  /**
   * An insert gesture.
   */
  VO_GESTURE_TYPE_INSERT_STROKES,

  /**
   * Value used to enable/disable all types in a row.
   */
  VO_GESTURE_TYPE_ALL = ((int32_t)(0x7FFFFFFF) - 1),

  _voGestureType_FORCE_INT_SIZE = 0x7FFFFFFF
}
voGestureType;

/**
 * Lists the gesture geometries.
 */
typedef enum _voGestureGeometry
{
  /**
   * Value returned by the `getGeometry()` function to indicate an error.
   */
  VO_GESTURE_GEOMETRY_ERR = -1,

  /**
   * Undefined geometry.
   */
  VO_GESTURE_GEOMETRY_UNDEFINED,

  /**
   * A leftward straight stroke.
   */
  VO_GESTURE_GEOMETRY_LEFT,

  /**
   * A rightward straight stroke.
   */
  VO_GESTURE_GEOMETRY_RIGHT,

  /**
   * An upward straight stroke.
   */
  VO_GESTURE_GEOMETRY_UP,

  /**
   * A downward straight stroke.
   */
  VO_GESTURE_GEOMETRY_DOWN,

  /**
   * A diagonal straight stroke between bottom-left and top-right corners.
   */
  VO_GESTURE_GEOMETRY_SLASH,

  /**
   * A diagonal straight stroke between top-left and bottom-right corners.
   */
  VO_GESTURE_GEOMETRY_BACKSLASH,

  /**
   * A square angle going downward then leftward.
   */
  VO_GESTURE_GEOMETRY_DOWN_THEN_LEFT,

  /**
   * A square angle going downward then rightward.
   */
  VO_GESTURE_GEOMETRY_DOWN_THEN_RIGHT,

  /**
   * A V shaped stroke from left to right, possibly continued with a horizontal
   * tail (like a square root symbol).
   */
  VO_GESTURE_GEOMETRY_V_TO_RIGHT,

  /**
   * A V shaped stroke from right to left, possibly continued with a horizontal
   * tail (like an horizontally mirrored square root symbol).
   */
  VO_GESTURE_GEOMETRY_V_TO_LEFT,

  /**
   * A cross.
   */
  VO_GESTURE_GEOMETRY_CROSS,

  /**
   * A scratch.
   */
  VO_GESTURE_GEOMETRY_SCRATCH,

  /**
   * An ellipse.
   */
  VO_GESTURE_GEOMETRY_ELLIPSE,

  /**
   * A free form closed stroke.
   */
  VO_GESTURE_GEOMETRY_FREE_FORM,

  /**
   * A pig-tail.
   */
  VO_GESTURE_GEOMETRY_PIGTAIL,

  /**
   * A bottom arc.
   */
  VO_GESTURE_GEOMETRY_BOTTOM_ARC,

  /**
   * A point.
   *
   * (1 stroke looking like a point and entered in a small amount of time)
   */
  VO_GESTURE_GEOMETRY_POINT,

  /**
   * Value used to set detection sensitivity of all geometries in a row.
   */
  VO_GESTURE_GEOMETRY_ALL = ((int32_t)(0x7FFFFFFF) - 1),

  _voGestureGeometry_FORCE_INT_SIZE = 0x7FFFFFFF
}
voGestureGeometry;


// -- GestureRecognizer --------------------------------------------------------

/**
 * {@propertySet GestureRecognizer}
 */
enum VO_GESTURERECOGNIZER_PROP
{
  /**
   * Specifies the time in millisecond to wait before taking a decision.
   *
   * This value must be positive.
   *
   * {@property int32_t}
   */
  VO_GESTURE_DECISION_DELAY = (VO_GestureRecognizer << 16),
};

/**
 * Lists the gesture contexts.
 */
typedef enum _voGestureContext
{
  /**
   * Gesture context for gestures that applies to unrecognized ink.
   *
   * Only add and erase gestures are supported.
   */
  VO_GESTURE_CONTEXT_INK,

  /**
   * Gesture context for gestures that applies to recognized text.
   *
   * All gestures are supported.
   */
  VO_GESTURE_CONTEXT_TEXT,

  /**
   * Gesture context for gestures that applies to recognized shapes.
   *
   * Only add, erase, select and tap gestures are supported.
   */
  VO_GESTURE_CONTEXT_SHAPE,

  /**
   * Gesture context for gestures that applies to recognized math elements.
   *
   * Only add, erase, select and tap gestures are supported.
   */
  VO_GESTURE_CONTEXT_MATH,

  /**
   * Gesture context for gestures that applies to recognized music elements.
   *
   * Only add, erase, select and tap gestures are supported.
   */
  VO_GESTURE_CONTEXT_MUSIC,

  /**
   * Gesture context for gestures that applies on strokes associated to a
   * drawing field.
   *
   * Only add and erase gestures are supported.
   */
  VO_GESTURE_CONTEXT_DRAWING,

  /**
   * Value used to enable/disable intents/types on all contexts in a row.
   */
  VO_GESTURE_CONTEXT_ALL = ((int32_t)(0x7FFFFFFF) - 1),

  _voGestureContext_FORCE_INT_SIZE = 0x7FFFFFFF
}
voGestureContext;

/**
 * Functions composing the `voIGestureRecognizer` interface.
 *
 * This interface exposes the public methods of the `voGestureRecognizer` type.
 *
 * {@implementingTypes voGestureRecognizer}
 */
typedef struct _voIGestureRecognizer
{
  /**
   * Sets the specified handler of this gesture recognizer.
   *
   * @param engine the engine.
   * @param target the target gesture recognizer.
   * @param handler the handler to be set or `NULL`.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voGestureRecognizer`.
   * @throws VO_NO_SUCH_OBJECT when the `handler` reference is invalid.
   * @throws VO_INVALID_OBJECT when `handler` does not implement the
   *   `voIGestureHandler` interface.
   * @throws VO_INVALID_STATE when `handler` is already registered.
   */
  bool (VO_MSE_CALL *setHandler)(voEngine engine, voGestureRecognizer target, voEngineObject handler);

  /**
   * Sets the detection sensitivity of the specified gesture geometry.
   *
   * @note If set to zero the geometry will be disabled.
   *
   * @param engine the engine.
   * @param target the target gesture recognizer.
   * @param geometry the gesture geometry.
   * @param sensitivity the detection sensitivity to be set.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voGestureRecognizer`.
   * @throws VO_INVALID_ARGUMENT when `geometry` is invalid.
   * @throws VO_INVALID_ARGUMENT when `sensitivity` is not between `0` and `1`.
   */
  bool (VO_MSE_CALL *setDetectionSensitivity)(voEngine engine, voGestureRecognizer target,
                                              voGestureGeometry geometry, float sensitivity);

  /**
   * Returns the detection sensitivity of the specified gesture geometry.
   *
   * @param engine the engine.
   * @param target the target gesture recognizer.
   * @param geometry the gesture geometry.
   *
   * @return the detection sensitivity on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voGestureRecognizer`.
   * @throws VO_INVALID_ARGUMENT when `geometry` is invalid.
   */
  float (VO_MSE_CALL *getDetectionSensitivity)(voEngine engine, voGestureRecognizer target,
                                               voGestureGeometry geometry);

  /**
   * Enables a type of intent to occur on a particular type of context.
   *
   * @param engine the engine.
   * @param target the target gesture recognizer.
   * @param intent the type of intent to which the setting applies.
   * @param context the type of context to which the setting applies.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voGestureRecognizer`.
   * @throws VO_INVALID_ARGUMENT when `intent` is invalid.
   * @throws VO_INVALID_ARGUMENT when `context` is invalid.
   * @throws VO_INVALID_ARGUMENT when `intent` can't be enabled for this
   *   `context`.
   */
  bool (VO_MSE_CALL *enableIntent)(voEngine engine, voGestureRecognizer target,
                                   voGestureIntent intent, voGestureContext context);

  /**
   * Disables a type of intent to occur on a particular type of context.
   *
   * @param engine the engine.
   * @param target the target gesture recognizer.
   * @param intent the type of intent to which the setting applies.
   * @param context the type of context to which the setting applies.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voGestureRecognizer`.
   * @throws VO_INVALID_ARGUMENT when `intent` is invalid.
   * @throws VO_INVALID_ARGUMENT when `context` is invalid.
   * @throws VO_INVALID_ARGUMENT when `intent` can't be disabled for this
   *   `context`.
   */
  bool (VO_MSE_CALL *disableIntent)(voEngine engine, voGestureRecognizer target,
                                    voGestureIntent intent, voGestureContext context);

  /**
   * Tells whether a type of intent is enabled on a particular type of context.
   *
   * @param engine the engine.
   * @param target the target gesture recognizer.
   * @param intent the type of intent to which the setting applies.
   * @param context the type of context to which the setting applies.
   *
   * @return `VO_YES` or `VO_NO` on success, otherwise `VO_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voGestureRecognizer`.
   * @throws VO_INVALID_ARGUMENT when `intent` is invalid.
   * @throws VO_INVALID_ARGUMENT when `context` is invalid.
   */
  voYesNo (VO_MSE_CALL *isIntentEnabled)(voEngine engine, voGestureRecognizer target,
                                         voGestureIntent intent, voGestureContext context);

  /**
   * Enables a gesture type for a particular context.
   *
   * @param engine the engine.
   * @param target the target gesture recognizer.
   * @param type the type to which the setting applies.
   * @param context the type of context to which the setting applies.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voGestureRecognizer`.
   * @throws VO_INVALID_ARGUMENT when `type` is invalid.
   * @throws VO_INVALID_ARGUMENT when `context` is invalid.
   * @throws VO_INVALID_ARGUMENT when the gesture can't be enabled for this
   *   context.
   */
  bool (VO_MSE_CALL *enableType)( voEngine engine, voGestureRecognizer target,
                                  voGestureType type, voGestureContext context);

  /**
   * Disables a gesture type for a particular context.
   *
   * @param engine the engine.
   * @param target the target gesture recognizer.
   * @param type the type to which the setting applies.
   * @param context the type of context to which the setting applies.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voGestureRecognizer`.
   * @throws VO_INVALID_ARGUMENT when `type` is invalid.
   * @throws VO_INVALID_ARGUMENT when `context` is invalid.
   * @throws VO_INVALID_ARGUMENT when the gesture can't be disabled for this
   *   context.
   */
  bool (VO_MSE_CALL *disableType)(voEngine engine, voGestureRecognizer target,
                                  voGestureType type, voGestureContext context);

  /**
   * Tells whether a gesture type is enabled for a particular context.
   *
   * @param engine the engine.
   * @param target the target gesture recognizer.
   * @param type the type to which the setting applies.
   * @param context the type of context to which the setting applies.
   *
   * @return `VO_YES` or `VO_NO` on success, otherwise `VO_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voGestureRecognizer`.
   * @throws VO_INVALID_ARGUMENT when `type` is invalid.
   * @throws VO_INVALID_ARGUMENT when `context` is invalid.
   */
  voYesNo (VO_MSE_CALL *isTypeEnabled)( voEngine engine, voGestureRecognizer target,
                                        voGestureType type, voGestureContext context);

  /**
   * Returns the configuration manager associated with the target gesture
   * recognizer.
   *
   * @param engine the engine.
   * @param target the target gesture recognizer.
   *
   * @return the configuration manager on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voGestureRecognizer`.
   */
  voConfigurationManager (VO_MSE_CALL *getConfigurationManager)(voEngine engine, voGestureRecognizer target);

  /**
   * Sets the specified configuration manager to the target gesture recognizer.
   *
   * @param engine the engine.
   * @param target the target gesture recognizer.
   * @param manager the configuration manager to set, or `NULL`.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voGestureRecognizer`.
   * @throws VO_NO_SUCH_OBJECT when the `manager` reference is invalid.
   * @throws VO_INVALID_OBJECT when `manager` is not a `voConfigurationManager`.
   * @throws VO_INVALID_STATE when `manager` is already registered.
   */
  bool (VO_MSE_CALL *setConfigurationManager)(voEngine engine, voGestureRecognizer target, voConfigurationManager manager);
}
voIGestureRecognizer;


#ifndef VO_NO_FUNCTIONS
/**
 * Creates a new `voGestureRecognizer` instance.
 *
 * @param engine the engine.
 *
 * @return the newly created `voGestureRecognizer` instance on success,
 *   otherwise `NULL`.
 *
 * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
 * @throws VO_LIMIT_EXCEEDED when the number of `voGestureRecognizer` instances
 *   would exceed the allowed limit (please remember that the limit can be
 *   shared by several types).
 */
VO_INLINE voGestureRecognizer voCreateGestureRecognizer(voEngine engine)
{
  return voCreateObject(engine, VO_GestureRecognizer);
}
#endif // end of: #ifndef VO_NO_FUNCTIONS


// -- GestureResults -----------------------------------------------------------

/**
 * Functions composing the `voIGestureResults` interface.
 *
 * This interface exposes the public methods of the `voGestureResults` type.
 *
 * {@implementingTypes voGestureResults}
 */
typedef struct _voIGestureResults
{
  /**
   * Returns the number of recognized gestures.
   *
   * @param engine the engine.
   * @param target the target gesture results.
   *
   * @return the number of recognized gestures on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voGestureResults`.
   */
  int32_t (VO_MSE_CALL *getCount)(voEngine engine, voGestureResults target);

  /**
   * Returns the gesture result at the specified index.
   *
   * @param engine the engine.
   * @param target the target gesture results.
   * @param index the index of the requested gesture result.
   *
   * @return the gesture result on success, otherwise `NULL`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voGestureResults`.
   * @throws VO_INDEX_OUT_OF_BOUNDS when `index` is out of bounds.
   * @throws VO_LIMIT_EXCEEDED when the number of `voGestureResult` instances
   *   would exceed the allowed limit (please remember that the limit can be
   *   shared by several types).
   */
  voGestureResult (VO_MSE_CALL *getAt)(voEngine engine, voGestureResults target, int32_t index);
}
voIGestureResults;


/**
 * Functions composing the `voIGestureResult` interface.
 *
 * This interface exposes the public methods of the `voGestureResult` type.
 *
 * {@implementingTypes voGestureResult}
 */
typedef struct _voIGestureResult
{
  /**
   * Returns the intent of this gesture result.
   *
   * @param engine the engine.
   * @param target the target gesture result.
   *
   * @return the intent on success, otherwise `VO_GESTURE_INTENT_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voGestureResult`.
   */
  voGestureIntent (VO_MSE_CALL *getIntent)(voEngine engine, voGestureResult target);

  /**
   * Returns the type of this gesture result.
   *
   * @param engine the engine.
   * @param target the target gesture result.
   *
   * @return the type on success, otherwise `VO_GESTURE_TYPE_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voGestureResult`.
   */
  voGestureType (VO_MSE_CALL *getType)(voEngine engine, voGestureResult target);

  /**
   * Returns the geometry of this gesture result.
   *
   * @param engine the engine.
   * @param target the target gesture result.
   *
   * @return the geometry on success, otherwise `VO_GESTURE_GEOMETRY_ERR`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voGestureResult`.
   */
  voGestureGeometry (VO_MSE_CALL *getGeometry)(voEngine engine, voGestureResult target);

  /**
   * Returns the score of this gesture result.
   *
   * @param engine the engine.
   * @param target the target gesture result.
   *
   * @return the score on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voGestureResult`.
   */
  float (VO_MSE_CALL *getScore)(voEngine engine, voGestureResult target);

  /**
   * Returns the selection associated with this gesture result.
   *
   * @param engine the engine.
   * @param target the target gesture result.
   *
   * @return a reference to the newly created `voInkSelection` or
   *   `voPageSelection` on success, otherwise `NULL`.
   *   When `NULL`, use `voGetError()` to figure out whether an error occurred.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voGestureResult`.
   * @throws VO_LIMIT_EXCEEDED when the number of instances of the specified
   *   type would exceed the allowed limit (please remember that the limit can
   *   be shared by several types).
   */
  voEngineObject (VO_MSE_CALL *getSelection)(voEngine engine, voGestureResult target);

  /**
   * Returns the before selection of this gesture result.
   *
   * @param engine the engine.
   * @param target the target gesture result.
   *
   * @return a reference to the newly created `voInkSelection` or
   *   `voPageSelection` on success, otherwise `NULL`.
   *   When `NULL`, use `voGetError()` to figure out whether an error occurred.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voGestureResult`.
   * @throws VO_LIMIT_EXCEEDED when the number of instances of the specified
   *   type would exceed the allowed limit (please remember that the limit can
   *   be shared by several types).
   */
  voEngineObject (VO_MSE_CALL *getBeforeSelection)(voEngine engine, voGestureResult target);

  /**
   * Returns the after selection of this gesture result.
   *
   * @param engine the engine.
   * @param target the target gesture result.
   *
   * @return a reference to the newly created `voInkSelection` or
   *   `voPageSelection` on success, otherwise `NULL`.
   *   When `NULL`, use `voGetError()` to figure out whether an error occurred.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voGestureResult`.
   * @throws VO_LIMIT_EXCEEDED when the number of instances of the specified
   *   type would exceed the allowed limit (please remember that the limit can
   *   be shared by several types).
   */
  voEngineObject (VO_MSE_CALL *getAfterSelection)(voEngine engine, voGestureResult target);

  /**
   * Returns the index of the first stroke associated with this gesture result.
   *
   * @note This is the index of the stroke in the ink sampler.
   *
   * @param engine the engine.
   * @param target the target gesture result.
   *
   * @return the stroke index on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voGestureResult`.
   */
  int32_t (VO_MSE_CALL *getFirstStroke)(voEngine engine, voGestureResult target);

  /**
   * Returns the index of the last stroke associated with this gesture result.
   *
   * @note This is the index of the stroke in the ink sampler.
   *
   * @param engine the engine.
   * @param target the target gesture result.
   *
   * @return the stroke index on success, otherwise `-1`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voGestureResult`.
   */
  int32_t (VO_MSE_CALL *getLastStroke)(voEngine engine, voGestureResult target);

  /**
   * Returns the reference position of this gesture result.
   *
   * For a Tap gesture the reference point is the first point entered. For the
   * other gestures, this is the gravity center.
   *
   * @param engine the engine.
   * @param target the target gesture result.
   * @param position the recipient for the reference position of a gesture.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voGestureResult`.
   * @throws VO_INVALID_USER_BUFFER when `position` is not writable.
   */
  bool (VO_MSE_CALL *getHotSpot)(voEngine engine, voGestureResult target, voPoint* position);

  /**
   * Returns the device identifier of this gesture result.
   *
   * @param engine the engine.
   * @param target the target gesture result.
   * @param id a recipient for the identifier of the device that has initiated
   *   the gesture.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voGestureResult`.
   * @throws VO_INVALID_USER_BUFFER when `id` is not writable.
   */
  bool (VO_MSE_CALL *getDeviceId)(voEngine engine, voGestureResult target, voDeviceId* id);

  /**
   * Returns the pointer identifier of this gesture result.
   *
   * @param engine the engine.
   * @param target the target gesture result.
   * @param id a recipient for the identifier of the pointer that has initiated
   *   the gesture.
   *
   * @return `true` on success, otherwise `false`.
   *
   * @throws VO_NO_SUCH_ENGINE when the `engine` reference is invalid.
   * @throws VO_NO_SUCH_OBJECT when the `target` reference is invalid.
   * @throws VO_INVALID_OPERATION when `target` is not a `voGestureResult`.
   * @throws VO_INVALID_USER_BUFFER when `id` is not writable.
   */
  bool (VO_MSE_CALL *getPointerId)(voEngine engine, voGestureResult target, voPointerId* id);
}
voIGestureResult;


// -- IGestureHandler ----------------------------------------------------------

/**
 * Functions composing the `voIGestureHandler` interface.
 *
 * @note This interface is implementable by user objects. User code is not
 *   allowed to invoke this interface directly: as a consequence
 *   `voGetInterface()` returns `NULL`.
 */
typedef struct _voIGestureHandler
{
  /**
   * Called when the target gesture handler is added to a gesture recognizer.
   *
   * @param engine the engine.
   * @param target the target gesture handler.
   * @param recognizer the gesture recognizer to which the gesture handler was
   *   added.
   */
  void (VO_MSE_CALL *added)(voEngine engine, voEngineObject target, voGestureRecognizer recognizer);

  /**
   * Called when the target gesture handler is removed from a gesture
   * recognizer.
   *
   * @param engine the engine.
   * @param target the target gesture handler.
   * @param recognizer the gesture recognizer to which the gesture handler was
   *   removed.
   */
  void (VO_MSE_CALL *removed)(voEngine engine, voEngineObject target, voGestureRecognizer recognizer);

  /**
   * Called when gestures are recognized and proposed as a result to the
   * application.
   *
   * @param engine the engine.
   * @param target the target gesture handler.
   * @param results the recognized gestures.
   *
   * @return the index of the result selected by the user, otherwise `-1`.
   */
  int32_t (VO_MSE_CALL *onResults)(voEngine engine, voEngineObject target, voGestureResults results);

  /**
   * Called when a gesture recognizer has been configured through its
   * configuration manager.
   *
   * The application may use the opportunity to apply custom configuration.
   *
   * @param engine the engine.
   * @param target the target gesture handler.
   * @param configurer the configurer being used.
   */
  void (VO_MSE_CALL *onConfigured)(voEngine engine, voEngineObject target, voConfigurer configurer);

  /**
   * Called when an error occurred while configuring the gesture recognizer
   * through its configuration manager.
   *
   * @param engine the engine.
   * @param target the target gesture handler.
   * @param error the `VO_ENGINE_ERR` returned.
   */
  void (VO_MSE_CALL *onError)(voEngine engine, voEngineObject target, int32_t error);
}
voIGestureHandler;

#endif // end of: #ifndef C_MYSCRIPTGESTURE_H
