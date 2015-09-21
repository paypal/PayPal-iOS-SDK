//
//  PayPalFuturePaymentViewController.h
//
//  Version 2.11.5
//
//  Copyright (c) 2014, PayPal
//  All rights reserved.
//

#import <UIKit/UIKit.h>
#import "PayPalConfiguration.h"

@class PayPalFuturePaymentViewController;
typedef void (^PayPalFuturePaymentDelegateCompletionBlock)(void);

#pragma mark - PayPalFuturePaymentDelegate

/// Exactly one of these two required delegate methods will get called when the UI completes.
/// You MUST dismiss the modal view controller from these required delegate methods.
@protocol PayPalFuturePaymentDelegate <NSObject>
@required

/// User canceled the future payment process.
/// Your code MUST dismiss the PayPalFuturePaymentViewController.
/// @param futurePaymentViewController The PayPalFuturePaymentViewController that the user canceled without agreement.
- (void)payPalFuturePaymentDidCancel:(PayPalFuturePaymentViewController *)futurePaymentViewController;

/// User successfully completed the future payment authorization.
/// The PayPalFuturePaymentViewController's activity indicator has been dismissed.
/// Your code MAY deal with the futurePaymentAuthorization, if it did not already do so within your optional
///     payPalFuturePaymentViewController:willAuthorizeFuturePayment:completionBlock: method.
/// Your code MUST dismiss the PayPalFuturePaymentViewController.
/// @param futurePaymentViewController The PayPalFuturePaymentViewController where the user successfullly authorized.
/// @param futurePaymentAuthorization A dictionary containing information that your server will need to process the payment.
- (void)payPalFuturePaymentViewController:(PayPalFuturePaymentViewController *)futurePaymentViewController
                didAuthorizeFuturePayment:(NSDictionary *)futurePaymentAuthorization;

@optional
/// User successfully completed the future payment authorization.
/// The PayPalFuturePaymentViewController's activity indicator is still visible.
/// Your code MAY deal with the futurePaymentAuthorization; e.g., send it to your server and await confirmation.
/// Your code MUST finish by calling the completionBlock.
/// Your code must NOT dismiss the PayPalFuturePaymentViewController.
/// @param futurePaymentViewController The PayPalFuturePaymentViewController where the user successfullly authorized.
/// @param futurePaymentAuthorization A dictionary containing information that your server will need to process the payment.
/// @param completionBlock Block to execute when your processing is done.
- (void)payPalFuturePaymentViewController:(PayPalFuturePaymentViewController *)futurePaymentViewController
                willAuthorizeFuturePayment:(NSDictionary *)futurePaymentAuthorization
                          completionBlock:(PayPalFuturePaymentDelegateCompletionBlock)completionBlock;
@end


#pragma mark - PayPalFuturePaymentViewController

@interface PayPalFuturePaymentViewController : UINavigationController

/// The designated initalizer. A new view controller MUST be initialized for each use.
/// @param configuration The configuration to be used for the lifetime of the controller.
///     The configuration properties merchantName, merchantPrivacyPolicyURL, and merchantUserAgreementURL must be provided.
/// @param delegate The delegate you want to receive updates about the future payment authorization.
- (instancetype)initWithConfiguration:(PayPalConfiguration *)configuration
                             delegate:(id<PayPalFuturePaymentDelegate>)delegate;

/// Delegate access
@property (nonatomic, weak, readonly) id<PayPalFuturePaymentDelegate> futurePaymentDelegate;

@end
