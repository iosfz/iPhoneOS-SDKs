//
//  INSetAudioSourceInCarIntent.h
//  Intents
//
//  Copyright © 2016 Apple. All rights reserved.
//

#import <Intents/INIntent.h>
#import <Intents/INIntentResolutionResult.h>

#import <Intents/INCarAudioSource.h>
#import <Intents/INRelativeReference.h>

@class INCarAudioSourceResolutionResult;
@class INRelativeReferenceResolutionResult;

NS_ASSUME_NONNULL_BEGIN

API_AVAILABLE(ios(10.0))
API_UNAVAILABLE(macosx)
@interface INSetAudioSourceInCarIntent : INIntent

- (instancetype)initWithAudioSource:(INCarAudioSource)audioSource
       relativeAudioSourceReference:(INRelativeReference)relativeAudioSourceReference NS_DESIGNATED_INITIALIZER;

@property (readonly, assign, NS_NONATOMIC_IOSONLY) INCarAudioSource audioSource;

@property (readonly, assign, NS_NONATOMIC_IOSONLY) INRelativeReference relativeAudioSourceReference;

@end

@class INSetAudioSourceInCarIntentResponse;

/*!
 @brief Protocol to declare support for handling an INSetAudioSourceInCarIntent 
 @abstract By implementing this protocol, a class can provide logic for resolving, confirming and handling the intent.
 @discussion The minimum requirement for an implementing class is that it should be able to handle the intent. The resolution and confirmation methods are optional. The handling method is always called last, after resolving and confirming the intent.
 */

API_AVAILABLE(ios(10.0))
API_UNAVAILABLE(macosx)
@protocol INSetAudioSourceInCarIntentHandling <NSObject>

@required

/*!
 @brief handling method

 @abstract Execute the task represented by the INSetAudioSourceInCarIntent that's passed in
 @discussion This method is called to actually execute the intent. The app must return a response for this intent.

 @param  setAudioSourceInCarIntent The input intent
 @param  completion The response handling block takes a INSetAudioSourceInCarIntentResponse containing the details of the result of having executed the intent

 @see  INSetAudioSourceInCarIntentResponse
 */

- (void)handleSetAudioSourceInCar:(INSetAudioSourceInCarIntent *)intent
                       completion:(void (^)(INSetAudioSourceInCarIntentResponse *response))completion NS_SWIFT_NAME(handle(setAudioSourceInCar:completion:));

@optional

/*!
 @brief Confirmation method
 @abstract Validate that this intent is ready for the next step (i.e. handling)
 @discussion These methods are called prior to asking the app to handle the intent. The app should return a response object that contains additional information about the intent, which may be relevant for the system to show the user prior to handling. If unimplemented, the system will assume the intent is valid following resolution, and will assume there is no additional information relevant to this intent.

 @param  setAudioSourceInCarIntent The input intent
 @param  completion The response block contains an INSetAudioSourceInCarIntentResponse containing additional details about the intent that may be relevant for the system to show the user prior to handling.

 @see INSetAudioSourceInCarIntentResponse

 */

- (void)confirmSetAudioSourceInCar:(INSetAudioSourceInCarIntent *)intent
                        completion:(void (^)(INSetAudioSourceInCarIntentResponse *response))completion NS_SWIFT_NAME(confirm(setAudioSourceInCar:completion:));

/*!
 @brief Resolution methods
 @abstract Determine if this intent is ready for the next step (confirmation)
 @discussion These methods are called to make sure the app extension is capable of handling this intent in its current form. This method is for validating if the intent needs any further fleshing out.

 @param  setAudioSourceInCarIntent The input intent
 @param  completion The response block contains an INIntentResolutionResult for the parameter being resolved

 @see INIntentResolutionResult

 */

- (void)resolveAudioSourceForSetAudioSourceInCar:(INSetAudioSourceInCarIntent *)intent
                                  withCompletion:(void (^)(INCarAudioSourceResolutionResult *resolutionResult))completion NS_SWIFT_NAME(resolveAudioSource(forSetAudioSourceInCar:with:));

- (void)resolveRelativeAudioSourceReferenceForSetAudioSourceInCar:(INSetAudioSourceInCarIntent *)intent
                                                   withCompletion:(void (^)(INRelativeReferenceResolutionResult *resolutionResult))completion NS_SWIFT_NAME(resolveRelativeAudioSourceReference(forSetAudioSourceInCar:with:));

@end

NS_ASSUME_NONNULL_END
