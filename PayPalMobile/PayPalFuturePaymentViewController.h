//
//  PayPalFuturePaymentViewController.h
//
//  Version 2.1.0
//
//  Copyright (c) 2014, PayPal
//  All rights reserved.
//

#import <UIKit/UIKit.h>
#import "PayPalConfiguration.h"

@class PayPalFuturePaymentViewController;

@protocol PayPalFuturePaymentDelegate <NSObject>
@required

/// User canceled the future payment process.
/// @param futurePaymentViewController The PayPalFuturePaymentViewController that the user canceled without agreement.
- (void)payPalFuturePaymentDidCancel:(PayPalFuturePaymentViewController *)futurePaymentViewController;

/// User successfully completed the future payment authorization.
/// @param futurePaymentViewController The PayPalPaymentViewController where the user successfullly made a payment.
/// @param futurePaymentAuthorization A dictionary containing information that  your server will need to process the payment.
- (void)payPalFuturePaymentViewController:(PayPalFuturePaymentViewController *)futurePaymentViewController
                didAuthorizeFuturePayment:(NSDictionary *)futurePaymentAuthorization;

@end

@interface PayPalFuturePaymentViewController : UINavigationController

/// The designated initalizer. A new view controller MUST be initialized for each use.
/// @param configuration The configuration to be used for the lifetime of the controller.
///     The configuration properties merchantName, merchantPrivacyPolicyURL, and merchantUserAgreementURL must be provided.
/// @param delegate The delegate you want to receive updates about the future payment.
- (instancetype)initWithConfiguration:(PayPalConfiguration *)configuration
                             delegate:(id<PayPalFuturePaymentDelegate>)delegate;

/// Delegate access
@property(nonatomic, weak, readonly) id<PayPalFuturePaymentDelegate> futurePaymentDelegate;

@end
