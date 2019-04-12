//
//  MPSCNNLoss.h
//  MPS
//
//  Created by Anna Tikhonova on 10/6/17.
//  Copyright © 2017 Apple. All rights reserved.
//

#ifndef MPSCNNLoss_h
#define MPSCNNLoss_h

#import <MPSNeuralNetwork/MPSCNNKernel.h>
#import <MPSNeuralNetwork/MPSCNNTypes.h>
#import <MPSCore/MPSState.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma mark -
#pragma mark MPSCNNLossDataDescriptor
    
/*!
 *  @class      MPSCNNLossDataDescriptor
 *  @dependency This depends on Metal.framework.
 *  @discussion The MPSCNNLossDataDescriptor specifies a loss data descriptor.
 *              The same descriptor can be used to initialize both the
 *              labels and the optional weights data.
 */
MPS_CLASS_AVAILABLE_STARTING(macos(10.13.4), ios(11.3), tvos(11.3))
@interface MPSCNNLossDataDescriptor : NSObject <NSCopying>

/*! @property   layout
 *  @abstract   Data layout of loss data. See MPSImage.h for more information.
 *  @discussion This parameter specifies the layout of loss data.
 */
@property (readonly, nonatomic) MPSDataLayout layout;

/*! @property   size
 *  @abstract   Size of loss data: (width, height, feature channels}.
 *  @discussion This parameter specifies the size of loss data.
 */
@property (readonly, nonatomic) MTLSize size;

/*! @property   bytesPerRow
 *  @abstract   Row bytes of loss data.
 *  @discussion This parameter specifies the row bytes of loss data.
 */
@property (readwrite, nonatomic) NSUInteger bytesPerRow;

/*! @property   bytesPerImage
 *  @abstract   Slice bytes of loss data.
 *  @discussion This parameter specifies the slice bytes of loss data.
 */
@property (readwrite, nonatomic) NSUInteger bytesPerImage;

/*
 * You must use the cnnLossDataDescriptorWithDataLayout interface instead.
 */
-(nonnull instancetype) init NS_UNAVAILABLE;

/*!
 *  @abstract   Make a descriptor loss data. The bytesPerRow and bytesPerImage
 *              are automatically calculated assuming a dense array. If it is
 *              not a dense array, adjust bytesPerRow and bytesPerImage to the
 *              right value by changing properties.
 *  @param      data                        The per-element loss data. The data must be in floating point format.
 *  @param      layout                      The data layout of loss data.
 *  @param      size                        The size of loss data.
 *  @return     A valid MPSCNNLossDataDescriptor object or nil, if failure.
 */
+(nullable MPSCNNLossDataDescriptor*) cnnLossDataDescriptorWithData: (NSData*__nonnull) data
                                                             layout: (MPSDataLayout) layout
                                                               size: (MTLSize) size;

@end /* MPSCNNLossDataDescriptor */

    
#pragma mark -
#pragma mark MPSCNNLossLabels

/*!
 *  @class      MPSCNNLossLabels
 *  @dependency This depends on Metal.framework.
 *  @discussion The MPSCNNLossLabels is used to hold the per-element weights buffer
 *              used by both MPSCNNLoss forward filter and MPSCNNLossGradient backward filter.
 *              The MPSCNNLoss forward filter populates the MPSCNNLossLabels object
 *              and the MPSCNNLossGradient backward filter consumes the state object.
 */
MPS_CLASS_AVAILABLE_STARTING(macos(10.13.4), ios(11.3), tvos(11.3))
@interface MPSCNNLossLabels : MPSState

/*!
 *  Use one of the interfaces below instead.
 */
-(nonnull instancetype) init NS_UNAVAILABLE;

/*!
 *  @abstract   Set labels (aka targets, ground truth) for the MPSCNNLossLabels object. 
 *  @discussion The labels and weights data are copied into internal storage. The computed loss can either be a
 *                                      scalar value (in batch mode, a single value per image in a batch) or it
 *                                      can be one value per feature channel. Thus, the size of the loss image
 *                                      must either match the size of the input source image or be {1, 1, 1},
 *                                      which results in a scalar value. In this convinience initializer, the
 *                                      assumed size of the loss image is {1, 1, 1}.
 *  @param      device                  Device the state resources will be created on.
 *  @param      labelsDescriptor        Describes the labels data. This includes:
 *                                          - The per-element labels data. The data must be in floating point format.
 *                                          - Data layout of labels data. See MPSImage.h for more information.
 *                                          - Size of labels data: (width, height, feature channels}.
 *                                          - Optionally, row bytes of labels data.
 *                                          - Optionally, slice bytes of labels data.
 */
-(nonnull instancetype) initWithDevice: (nonnull id<MTLDevice>)device
                      labelsDescriptor: (MPSCNNLossDataDescriptor* _Nonnull) labelsDescriptor;

/*!
 *  @abstract   Set labels (aka targets, ground truth) and weights for the MPSCNNLossLabels object.
 *              Weights are optional.
 *  @discussion The labels and weights data are copied into internal storage.
 *  @param      device                  Device the state resources will be created on.
 *  @param      lossImageSize           The size of the resulting loss image: { width, height, featureChannels }.
 *                                      The computed loss can either be a scalar value (in batch mode, a single
 *                                      value per image in a batch) or it can be one value per feature channel.
 *                                      Thus, the size of the loss image must either match the size of the input
 *                                      source image or be {1, 1, 1}, which results in a scalar value.
 *  @param      labelsDescriptor        Describes the labels data. This includes:
 *                                          - The per-element labels data. The data must be in floating point format.
 *                                          - Data layout of labels data. See MPSImage.h for more information.
 *                                          - Size of labels data: (width, height, feature channels}.
 *                                          - Optionally, row bytes of labels data.
 *                                          - Optionally, slice bytes of labels data.
 *  @param      weightsDescriptor       Describes the weights data. This includes:
 *                                          - The per-element weights data. The data must be in floating point format.
 *                                          - Data layout of weights data. See MPSImage.h for more information.
 *                                          - Size of weights data: (width, height, feature channels}.
 *                                          - Optionally, row bytes of weights data.
 *                                          - Optionally, slice bytes of weights data.
 *                                      This parameter is optional. If you are using a single weight, please use the
 *                                      weight property of the MPSCNNLossDescriptor object.
 */
-(nonnull instancetype) initWithDevice: (nonnull id<MTLDevice>)device
                         lossImageSize: (MTLSize) lossImageSize
                      labelsDescriptor: (MPSCNNLossDataDescriptor* _Nonnull) labelsDescriptor
                     weightsDescriptor: (MPSCNNLossDataDescriptor* _Nullable) weightsDescriptor NS_DESIGNATED_INITIALIZER;

/*!
 *  @abstract   Loss image accessor method.
 *  @return     An autoreleased MPSImage object, containing the loss data.
 *              The loss data is populated in the -encode call, thus the contents
 *              are undefined until you -encode the filter.
 *
 *              In order to gaurantee that the image is correctly synchronized for CPU side access,
 *              it is the application's responsibility to call the [gradientState synchronizeOnCommandBuffer:]
 *              method before accessing the data in the image.
 */
-(nonnull MPSImage*) lossImage;

@end /* MPSCNNLossLabels */
    

typedef NSArray <MPSCNNLossLabels*>  MPSCNNLossLabelsBatch MPS_AVAILABLE_STARTING(macos(10.13.4), ios(11.3), tvos(11.3));

#pragma mark -
#pragma mark MPSCNNLossDescriptor
    
/*!
 *  @class      MPSCNNLossDescriptor
 *  @dependency This depends on Metal.framework.
 *  @discussion The MPSCNNLossDescriptor specifies a loss filter descriptor.
 *              The same descriptor can be used to initialize both the
 *              MPSCNNLoss and the MPSCNNLossGradient filters.
 */
MPS_CLASS_AVAILABLE_STARTING(macos(10.13.4), ios(11.3), tvos(11.3))
@interface MPSCNNLossDescriptor : NSObject <NSCopying>

/*! @property   lossType
 *  @abstract   The type of a loss filter.
 *  @discussion This parameter specifies the type of a loss filter.
 */
@property (readwrite, nonatomic) MPSCNNLossType lossType;

/*! @property   reductionType
 *  @abstract   The type of a reduction operation performed in the loss filter.
 *  @discussion This parameter specifies the type of a reduction operation
 *              performed in the loss filter.
 */
@property (readwrite, nonatomic) MPSCNNReductionType reductionType;

/*! @property   weight
 *  @abstract   The scale factor to apply to each element of a result.
 *  @discussion Each element of a result is multiplied by the weight value.
 *              The default value is 1.0f.
 */
@property (readwrite, nonatomic) float weight;

/*!
 * @property    labelSmoothing
 * @abstract    The label smoothing parameter. The default value is 0.0f.
 * @discussion  This parameter is valid only for the loss functions of the following type(s):
 *              MPSCNNLossFunctionTypeSoftmaxCrossEntropy, MPSCNNLossFunctionTypeSigmoidCrossEntropy.
 *
 *              MPSCNNLossFunctionTypeSoftmaxCrossEntropy: given labels (ground truth), it is applied in the following way:
 *              labels = labelSmoothing > 0 ? labels * (1 - labelSmoothing) + labelSmoothing / numberOfClasses : labels
 *
 *              MPSCNNLossFunctionTypeSigmoidCrossEntropy: given labels (ground truth), it is applied in the following way:
 *              labels = labelSmoothing > 0 ? labels * (1 - labelSmoothing) + 0.5 * labelSmoothing : labels
 */
@property (readwrite, nonatomic) float labelSmoothing;

/*!
 * @property    numberOfClasses
 * @abstract    The number of classes parameter. The default value is 1.
 * @discussion  This parameter is valid only for the loss functions of the following type(s):
 *              MPSCNNLossFunctionTypeSoftmaxCrossEntropy.
 *
 *              Given labels (ground truth), it is applied in the following way:
 *              labels = labelSmoothing > 0 ? labels * (1 - labelSmoothing) + labelSmoothing / numberOfClasses : labels
 */
@property (readwrite, nonatomic) NSUInteger numberOfClasses;

/*!
 * @property    epsilon
 * @abstract    The epsilon parameter. The default value is 1e-7.
 * @discussion  This parameter is valid only for the loss functions of the following type(s):
 *              MPSCNNLossTypeLog.
 *
 *              Given predictions and labels (ground truth), it is applied in the following way:
 *              -(labels * log(predictions + epsilon)) - ((1 - labels) * log(1 - predictions + epsilon))
 */
@property (readwrite, nonatomic) float epsilon;

/*!
 * @property    delta
 * @abstract    The delta parameter. The default value is 1.0f.
 * @discussion  This parameter is valid only for the loss functions of the following type(s):
 *              MPSCNNLossTypeHuber.
 *
 *              Given predictions and labels (ground truth), it is applied in the following way:
 *              if (|predictions - labels| <= delta, loss = 0.5f * predictions^2
 *              if (|predictions - labels| >  delta, loss = 0.5 * delta^2 + delta * (|predictions - labels| - delta)
 */
@property (readwrite, nonatomic) float delta;

/*
 * You must use one of the interfaces below instead.
 */
-(nonnull instancetype) init NS_UNAVAILABLE;

/*!
 *  @abstract   Make a descriptor for a MPSCNNLoss or MPSCNNLossGradient object.
 *  @param      lossType                    The type of a loss filter.
 *  @param      reductionType               The type of a reduction operation to apply.
 *                                          This argument is ignored in the MPSCNNLossGradient filter.
 *  @return     A valid MPSCNNLossDescriptor object or nil, if failure.
 */
+(nullable MPSCNNLossDescriptor*) cnnLossDescriptorWithType:(MPSCNNLossType) lossType
                                              reductionType:(MPSCNNReductionType) reductionType;

@end /* MPSCNNLossDescriptor */


#pragma mark -
#pragma mark MPSCNNLoss

/*!
 *  @class      MPSCNNLoss
 *  @dependency This depends on Metal.framework.
 *  @discussion The MPSCNNLoss filter is only used for training. This filter performs both the forward and
 *              backward pass computations. Specifically, it computes the loss between the input (predictions)
 *              and target data (labels) and the loss gradient. The loss value can be a 1 x 1 x 1 image containing
 *              a scalar loss value or an image (of the same size as the input source image) with per feature
 *              channel losses. The loss value is used to determine whether to continue the training operation or
 *              to terminate it, once satisfactory results are achieved. The loss gradient is the first gradient
 *              computed for the backward pass and serves as input to the next gradient filter (in the backward
 *              direction).
 *
 *              The MPSCNNLoss filter is created with a MPSCNNLossDescriptor describing the type of a loss filter
 *              and the type of a reduction to use for computing the overall loss.
 *
 *              The MPSCNNLoss filter takes the output of the inference pass (predictions) as input. It also
 *              requires the target data (labels) and optionally, weights for the labels. If per-label weights
 *              are not supplied, there is an option to use a single weight value by setting the 'weight' properly
 *              on the MPSCNNLossDescriptor object. The labels and optional weights need to be supplied by the user
 *              using the MPSCNNLossLabels object. The labels and weights are described via the MPSCNNLossDataDescriptor
 *              objects, which are in turn used to initialize the MPSCNNLossLabels object.
 *
 *              If the specified reduction operation is MPSCNNReductionTypeNone, the destinationImage should be
 *              at least as large as the specified clipRect. The detinationImage will then contain per-element
 *              losses. Otherse, a reduction operation will be performed, according to the specified reduction
 *              type, and the filter will return a scalar value containing the overall loss. For more information
 *              on the available reduction types, see MPSCNNTypes.h. Also see MPSCNNLossDescriptor for the
 *              description of optional parameters.
 *
 *              Here is a code example:
 *
 *              // Setup
 *              MPSCNNLossDataDescriptor* labelsDescriptor =
 *                  [MPSCNNLossDataDescriptor cnnLossDataDescriptorWithData: labelsData
 *                                                                   layout: MPSDataLayoutHeightxWidthxFeatureChannels
 *                                                                     size: labelsDataSize];
 *              MPSCNNLossLabels* labels = [[MPSCNNLossLabels alloc] initWithDevice: device
 *                                                                 labelsDescriptor: labelsDescriptor];
 *              MPSCNNLossDescriptor *lossDescriptor =
 *                  [MPSCNNLossDescriptor cnnLossDescriptorWithType: (MPSCNNLossType)MPSCNNLossTypeMeanAbsoluteError
 *                                                    reductionType: (MPSCNNReductionType)MPSCNNReductionTypeSum];
 *              MPSCNNLoss* lossFilter = [[MPSCNNLoss alloc] initWithDevice: device lossDescriptor: lossDescriptor];
 *
 *              // Encode loss filter.
 *              // The sourceImage is the output of a previous layer, for example, the SoftMax layer. The lossGradientsImage
 *              // is the sourceGradient input image to the first gradient layer (in the backward direction), for example,
 *              // the SoftMax gradient filter.
 *              [lossFilter encodeToCommandBuffer: commandBuffer sourceImage: sourceImage
 *                                                                    labels: labels
 *                                                          destinationImage: lossGradientsImage];
 *
 *              // In order to gaurantee that the loss image data is correctly synchronized for CPU side access,
 *              // it is the application's responsibility to call the [labels synchronizeOnCommandBuffer:]
 *              // method before accessing the loss image data.
 *              [labels synchronizeOnCommandBuffer:commandBuffer];
 *              MPSImage* lossImage = [labels lossImage];
 *
 *
 *
 *              For predictions (y) and labels (t), the available loss filter types are the following:
 *
 *              Mean Absolute Error loss filter. This filter measures the absolute error of the element-wise
 *              difference between the predictions and labels.
 *              This loss function is computed according to the following formulas:
 *                  Compute losses:          losses = |y - t|
 *                  Compute weighted losses: weighted_losses = weight(s) * losses
 *                  Compute overall loss:    loss = reduce(weighted_losses, reductionType)
 *
 *              Mean Squared Error loss filter. This filter measures the squared error of the element-wise
 *              difference between the predictions and labels.
 *              This loss function is computed according to the following formulas:
 *                  Compute losses:          losses = (y - t)^2
 *                  Compute weighted losses: weighted_losses = weight(s) * losses
 *                  Compute overall loss:    loss = reduce(weighted_losses, reductionType)
 * 
 *              SoftMax Cross Entropy loss filter. This loss filter is applied element-wise.
 *              This loss filter combines the LogSoftMax and Negative Log Likelihood operations in a
 *              single filter. It is useful for training a classification problem with multiple classes.
 *              This loss function is computed according to the following formulas:
 *                  Compute losses:          losses = -t * LogSoftMax(y)
 *                  Compute weighted losses: weighted_losses = weight(s) * losses
 *                  Compute overall loss:    loss = reduce(weighted_losses, reductionType)
 *                                           If reductionType is MPSCNNReductionTypeMean, the accumulated
 *                                           loss value is divided by width * height instead of
 *                                           width * height * featureChannels.
 *
 *              Sigmoid Cross Entropy loss filter. This loss filter is applied element-wise.
 *              This loss function is computed according to the following formulas:
 *                  Compute losses:          losses = max(y, 0) - y * t + log(1 + exp(-|y|))
 *                  Compute weighted losses: weighted_losses = weight(s) * losses
 *                  Compute overall loss:    loss = reduce(weighted_losses, reductionType)
 *
 *              Categorical Cross Entropy loss filter. This loss filter is applied element-wise.
 *              This loss function is computed according to the following formulas:
 *                  Compute losses:          losses = -t * log(y)
 *                  Compute weighted losses: weighted_losses = weight(s) * losses
 *                  Compute overall loss:    loss = reduce(weighted_losses, reductionType)
 *
 *              Hinge loss filter. This loss filter is applied element-wise.
 *              The labels are expected to be 0.0 or 1.0.
 *              This loss function is computed according to the following formulas:
 *                  Compute losses:          losses = max(1 - (t * y), 0.0f)
 *                  Compute weighted losses: weighted_losses = weight(s) * losses
 *                  Compute overall loss:    loss = reduce(weighted_losses, reductionType)
 *
 *              Huber loss filter. This loss filter is applied element-wise.
 *              This loss function is computed according to the following formulas:
 *                  Compute losses:          if (|y - t| <= delta, losses = 0.5 * y^2
 *                                           if (|y - t| >  delta, losses = 0.5 * delta^2 + delta * (|y - t| - delta)
 *                  Compute weighted losses: weighted_losses = weight(s) * losses
 *                  Compute overall loss:    loss = reduce(weighted_losses, reductionType)
 *
 *              Cosine Distance loss filter. This loss filter is applied element-wise.
 *              The only valid reduction type for this loss filter is MPSCNNReductionTypeSum.
 *              This loss function is computed according to the following formulas:
 *                  Compute losses:          loss = 1 - reduce_sum(y * t)
 *                  Compute overall loss:    weighted_loss = weight * loss
 *
 *              Log loss filter. This loss filter is applied element-wise.
 *              This loss function is computed according to the following formulas:
 *                  Compute losses:          losses = -(t * log(y + epsilon)) - ((1 - t) * log(1 - y + epsilon))
 *                  Compute weighted losses: weighted_losses = weight(s) * losses
 *                  Compute overall loss:    loss = reduce(weighted_losses, reductionType)
 *
 *              Kullback-Leibler Divergence loss filter. This loss filter is applied element-wise.
 *              The input (predictions) is expected to contain log-probabilities.
 *                  This loss function is computed according to the following formulas:
 *                  Compute losses:          losses = t * (log(t) - y)
 *                  Compute weighted losses: weighted_losses = weight(s) * losses
 *                  Compute overall loss:    loss = reduce(weighted_losses, reductionType)
 *
 *
 *
 *              For predictions (y) and labels (t), the loss gradient for each available loss filter type
 *              is computed as follows:
 *
 *              Mean Absolute Error loss.
 *              The loss gradient is computed according to the following formulas:
 *                  Compute gradient:          d/dy = (y - t) / |y - t|
 *                  Compute weighted gradient: weighted_gradient = weight(s) * gradient
 *
 *              Mean Squared Error loss.
 *              The loss gradient is computed according to the following formulas:
 *                  Compute gradient:          d/dy = 2 * (y - t)
 *                  Compute weighted gradient: weighted_gradient = weight(s) * gradient
 *
 *              SoftMax Cross Entropy loss.
 *              The loss gradient is computed according to the following formulas:
 *                  First, apply the same label smoothing as in the MPSCNNLoss filter.
 *                  Compute gradient:          d/dy = y - t
 *                  Compute weighted gradient: weighted_gradient = weight(s) * gradient
 *
 *              Sigmoid Cross Entropy loss.
 *              The loss gradient is computed according to the following formulas:
 *              First, apply the same label smoothing as in the MPSCNNLoss filter.
 *                  Compute gradient:          d/dy = (1 / (1 + exp(-y)) - t
 *                  Compute weighted gradient: weighted_gradient = weight(s) * gradient
 *
 *              Categorical Cross Entropy loss.
 *              The loss gradient is computed according to the following formulas:
 *                  Compute gradient:          d/dy = -t / y
 *                  Compute weighted gradient: weighted_gradient = weight(s) * gradient
 *
 *              Hinge loss.
 *              The loss gradient is computed according to the following formulas:
 *                  Compute gradient:          d/dy = ((1 + ((1 - (2 * t)) * y)) > 0) ? 1 - (2 * t) : 0
 *                  Compute weighted gradient: weighted_gradient = weight(s) * gradient
 *
 *              Huber loss.
 *              The loss gradient is computed according to the following formulas:
 *                  Compute gradient:          d/dy = |y - t| > delta ? delta : y - t
 *                  Compute weighted gradient: weighted_gradient = weight(s) * gradient
 *
 *              Cosine Distance loss.
 *              The loss gradient is computed according to the following formulas:
 *                  Compute gradient:          d/dy = -t
 *                  Compute weighted gradient: weighted_gradient = weight(s) * gradient
 *
 *              Log loss.
 *              The loss gradient is computed according to the following formulas:
 *                  Compute gradient:          d/dy = (-2 * epsilon * t - t + y + epsilon) / (y * (1 - y) + epsilon * (epsilon + 1))
 *                  Compute weighted gradient: weighted_gradient = weight(s) * gradient
 *
 *              Kullback-Leibler Divergence loss.
 *              The loss gradient is computed according to the following formulas:
 *                  Compute gradient:          d/dy = -t / y
 *                  Compute weighted gradient: weighted_gradient = weight(s) * gradient
 *
 *              The number of output feature channels remains the same as the number of input feature
 *              channels.
 */
MPS_CLASS_AVAILABLE_STARTING(macos(10.13.4), ios(11.3), tvos(11.3))
@interface MPSCNNLoss : MPSCNNKernel

/*!
 * See MPSCNNLossDescriptor for information about the following properties.
 */
@property (readonly, nonatomic) MPSCNNLossType lossType;
@property (readonly, nonatomic) MPSCNNReductionType reductionType;
@property (readonly, nonatomic) float weight;
@property (readonly, nonatomic) float labelSmoothing;
@property (readonly, nonatomic) NSUInteger numberOfClasses;
@property (readonly, nonatomic) float epsilon;
@property (readonly, nonatomic) float delta;

/*
 * You must use initWithDevice:lossDescriptor instead.
 */
-(nonnull instancetype) initWithDevice: (nonnull id <MTLDevice>) device NS_UNAVAILABLE;

/*!
 *  @abstract   Initialize the loss filter with a loss descriptor.
 *  @param      device                   The device the filter will run on.
 *  @param      lossDescriptor           The loss descriptor.
 *  @return     A valid MPSCNNLoss object or nil, if failure.
 */
-(nonnull instancetype) initWithDevice: (nonnull id <MTLDevice>) device
                        lossDescriptor: (MPSCNNLossDescriptor*_Nonnull) lossDescriptor NS_DESIGNATED_INITIALIZER;

/*! @abstract <NSSecureCoding> support */
-(nullable instancetype) initWithCoder:(NSCoder * __nonnull)aDecoder device:(nonnull id <MTLDevice>) device NS_DESIGNATED_INITIALIZER;

/*! @abstract   Encode a MPSCNNLoss filter and return a gradient in the destinationImage.
 *  @discussion This filter consumes the output of a previous layer, for example, the SoftMax layer containing
 *              predictions, and the MPSCNNLossLabels object containing the target data (labels) and optionally,
 *              weights for the labels. The destinationImage contains the computed gradient for the loss layer.
 *              It serves as a source gradient input image to the first gradient layer (in the backward direction),
 *              in our example, the SoftMax gradient layer.
 *
 *  @param      commandBuffer       The MTLCommandBuffer on which to encode.
 *  @param      sourceImage         The source image from the previous filter in the graph (in the inference direction).
 *  @param      labels              The object containing the target data (labels) and optionally, weights for the labels.
 *  @param      destinationImage    The MPSImage into which to write the gradient result.
 */
-(void) encodeToCommandBuffer: (nonnull id <MTLCommandBuffer>) commandBuffer
                  sourceImage: (MPSImage * __nonnull) sourceImage
                       labels: (MPSCNNLossLabels * __nonnull) labels
             destinationImage: (MPSImage * __nonnull) destinationImage
            MPS_SWIFT_NAME( encode(commandBuffer:sourceImage:labels:destinationImage:));

/*! @abstract   Encode a MPSCNNLoss filter and return a gradient.
 *  @discussion This -encode call is similar to the encodeToCommandBuffer:sourceImage:labels:destinationImage: above,
 *              except that it creates and returns the MPSImage with the loss gradient result.
 *
 *  @param      commandBuffer       The MTLCommandBuffer on which to encode.
 *  @param      sourceImage         The source image from the previous filter in the graph (in the inference direction).
 *  @param      labels              The object containing the target data (labels) and optionally, weights for the labels.
 *  @return     The MPSImage containing the gradient result.
 */
-(MPSImage*__nonnull) encodeToCommandBuffer: (nonnull id <MTLCommandBuffer>) commandBuffer
                                sourceImage: (MPSImage * __nonnull) sourceImage
                                     labels: (MPSCNNLossLabels * __nonnull) labels
            MPS_SWIFT_NAME( encode(commandBuffer:sourceImage:labels:));


-(void) encodeBatchToCommandBuffer: (nonnull id <MTLCommandBuffer>) commandBuffer
                      sourceImages: (MPSImageBatch * __nonnull) sourceImage
                            labels: (MPSCNNLossLabelsBatch * __nonnull) labels
                 destinationImages: (MPSImageBatch * __nonnull) destinationImage
            MPS_SWIFT_NAME( encode(commandBuffer:sourceImages:labels:destinationImages:));

-(MPSImageBatch*__nonnull) encodeBatchToCommandBuffer: (nonnull id <MTLCommandBuffer>) commandBuffer
                                         sourceImages: (MPSImageBatch * __nonnull) sourceImage
                                               labels: (MPSCNNLossLabelsBatch * __nonnull) labels
            MPS_SWIFT_NAME( encode(commandBuffer:sourceImages:labels:));

@end /* MPSCNNLoss */


#ifdef __cplusplus
}
#endif

#endif /* MPSCNNLoss_h */
