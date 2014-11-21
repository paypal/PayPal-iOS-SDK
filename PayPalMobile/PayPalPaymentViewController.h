//
//  PayPalPaymentViewController.h
//
//  Version 2.7.2
//
//  Copyright (c) 2014, PayPal
//  All rights reserved.
//

#import <UIKit/UIKit.h>
#import "PayPalConfiguration.h"
#import "PayPalPayment.h"

// Important note:
//
// This is a proof of payment system. You MUST verify all transactions
// via a call from your servers (not your app) to PayPal's servers, to
// ensure that the transaction was genuine and successful.
// See https://developer.paypal.com/webapps/developer/docs/integration/mobile/verify-mobile-payment/
// for details.

#pragma mark - Delegates and notifications

@class PayPalPaymentViewController;
typedef void (^PayPalPaymentDelegateCompletionBlock)(void);

/// Exactly one of these two required delegate methods will get called when the UI completes.
/// You MUST dismiss the modal view controller from these required delegate methods.
@protocol PayPalPaymentDelegate <NSObject>
@required

/// User canceled the payment process.
/// Your code MUST dismiss the PayPalPaymentViewController.
/// @param paymentViewController The PayPalPaymentViewController that the user canceled without making a payment.
- (void)payPalPaymentDidCancel:(PayPalPaymentViewController *)paymentViewController;

/// User successfully completed the payment.
/// The PayPalPaymentViewController's activity indicator has been dismissed.
/// Your code MAY deal with the completedPayment, if it did not already do so within your optional
///     payPalPaymentViewController:willCompletePayment:completionBlock: method.
/// Your code MUST dismiss the PayPalPaymentViewController.
/// See https://developer.paypal.com/webapps/developer/docs/integration/mobile/verify-mobile-payment/ for
///     information about payment verification.
/// @param paymentViewController The PayPalPaymentViewController where the user successfullly made a payment.
/// @param completedPayment completedPayment.confirmation contains information your server will need to verify the payment.
- (void)payPalPaymentViewController:(PayPalPaymentViewController *)paymentViewController
                 didCompletePayment:(PayPalPayment *)completedPayment;

@optional
/// User successfully completed the payment.
/// The PayPalPaymentViewController's activity indicator is still visible.
/// Your code MAY deal with the completedPayment; e.g., send it to your server and await confirmation.
/// Your code MUST finish by calling the completionBlock.
/// Your code must NOT dismiss the PayPalPaymentViewController.
/// See https://developer.paypal.com/webapps/developer/docs/integration/mobile/verify-mobile-payment/ for
///     information about payment verification.
/// @param paymentViewController The PayPalPaymentViewController where the user successfullly made a payment.
/// @param completedPayment completedPayment.confirmation contains information your server will need to verify the payment.
/// @param completionBlock Block to execute when your processing is done.
- (void)payPalPaymentViewController:(PayPalPaymentViewController *)paymentViewController
                 willCompletePayment:(PayPalPayment *)completedPayment
                    completionBlock:(PayPalPaymentDelegateCompletionBlock)completionBlock;

@end


#pragma mark - PayPalPaymentViewController

@interface PayPalPaymentViewController : UINavigationController

/// The designated initalizer. A new view controller MUST be initialized for each use.
/// @param payment The payment to be processed.
/// @param configuration The configuration to be used for the lifetime of the controller
///     (e.g., default email address or hideCreditCard); can be nil.
/// @param delegate The delegate you want to receive updates about the payment.
- (instancetype)initWithPayment:(PayPalPayment *)payment
                  configuration:(PayPalConfiguration *)configuration
                       delegate:(id<PayPalPaymentDelegate>)delegate;

/// Delegate access
@property(nonatomic, weak, readonly) id<PayPalPaymentDelegate> paymentDelegate;

/// PayPalPaymentViewControllerState See the state property for context.
typedef NS_ENUM(NSInteger, PayPalPaymentViewControllerState) {
  /// The payment has not been sent. You MAY safely dismiss the PayPalPaymentViewController.
  PayPalPaymentViewControllerStateUnsent = 0,
  /// The payment is in progress. You MUST NOT dismiss the PayPalPaymentViewController.
  PayPalPaymentViewControllerStateInProgress = 1,
};

/// Although irrelevant to most apps, if your app needs to know where the user is within
/// the payment flow, you can check this property.
/// (You can use key-value observing to watch for changes.)
///
/// For example, perhaps your app would like to dismiss the PayPalPaymentViewController
/// if the user is taking so long to complete the payment flow that the item they
/// ordered has gone out of stock.
///
/// - The state is initially set to PayPalPaymentViewControllerStateUnsent.
/// - When the user taps the final payment confirmation button, the state changes to
///   PayPalPaymentViewControllerStateInProgress.
/// - If the payment goes through successfully, the state remains at
///   PayPalPaymentViewControllerStateInProgress, and your app's
///   payPalPaymentViewController:didCompletePayment: method is called.
/// - If the payment fails, the state changes back to PayPalPaymentViewControllerStateUnsent.
///   (Also, an appropriate error message is displayed to the user).
@property(nonatomic, assign, readonly) PayPalPaymentViewControllerState state;

@end
