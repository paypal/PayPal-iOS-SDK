//
//  PayPalPaymentViewController.h
//
//  Version 1.0.4
//
//  Copyright (c) 2013, PayPal
//  All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//
//  1. Redistributions of source code must retain the above copyright notice, this
//  list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright notice,
//  this list of conditions and the following disclaimer in the documentation
//  and/or other materials provided with the distribution.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
//  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
//  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
//  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//  The views and conclusions contained in the software and documentation are those
//  of the authors and should not be interpreted as representing official policies,
//  either expressed or implied, of the FreeBSD Project.
//

#import <UIKit/UIKit.h>
#import "PayPalPayment.h"

// Important note:
//
// This is a proof of payment system. You MUST verify all transactions
// via a call from your servers (not your app) to PayPal's servers, to
// ensure that the transaction was genuine and successful.

#pragma mark - Delegates and notifications

// Exactly one of these delegate methods will get called when the UI completes.
// You MUST dismiss the modal view controller from these delegate methods.
@protocol PayPalPaymentDelegate <NSObject>
@required

- (void)payPalPaymentDidCancel;
- (void)payPalPaymentDidComplete:(PayPalPayment *)completedPayment; // details will be present in completedPayment.confirmation

@end

// This notification corresponds to the -payPalPaymentDidComplete: method above, with one key
// difference. The delegate method is called when the UI completes; this notification
// is posted as soon as the payment has completed, which can occur before the UI completes.
// For example, the UI may contain a summary or receipt view. You MAY use this notification
// to do background network processing while the UI completes.
// You MUST NOT dismiss the modal view controller from this notification; that MUST be
// done in the delegate method.
//
// The notification's object is the completed PayPalPayment, same as the delegate method.
extern NSString *const PayPalTransactionDidSucceedNotification;

// PayPalPaymentViewController can operate in different environments to facilitate development:
//
// Production (default): Normal, live environment. Real money gets moved.
//                       This environment MUST be used for App Store submissions.
// Sandbox: Uses the PayPal sandbox for transactions.
// NoNetwork: Mock mode. Does not submit transactions to PayPal. Fakes successful responses.
extern NSString *const PayPalEnvironmentProduction;
extern NSString *const PayPalEnvironmentSandbox;
extern NSString *const PayPalEnvironmentNoNetwork;


#pragma mark - PayPalPaymentViewController

@interface PayPalPaymentViewController : UINavigationController

// The designated initalizer. A new view controller MUST be initialized for each use.
//
// Get a client id from developer.paypal.com. The receiverEmail is the email address
// associated with the PayPal account that you used to log in to developer.paypal.com.
//
// payerId is a unique identifier for the user who is paying, such as a customer number
// or email address. Do not use hardware based identifiers. If you do not have a payerId,
// you MAY pass nil. However, without a payerId, PayPalPaymentViewController will not
// "remember" credit cards for easier subsequent payments.
- (id)initWithClientId:(NSString *)clientId
         receiverEmail:(NSString *)payPalAccountEmailAddress
               payerId:(NSString *)payerId
               payment:(PayPalPayment *)payment
              delegate:(id<PayPalPaymentDelegate>)delegate;

// Delegate access
@property(nonatomic, unsafe_unretained, readwrite) id<PayPalPaymentDelegate> paymentDelegate;

// Use to change the environment -- see the PayPalEnvironment constants above.
//
// The environment must not be changed while a PayPalPaymentViewController
// is visible. Setting the environment will cancel all network operations
// and reset local state.
//
// Defaults to PayPalEnvironmentProduction.
+ (NSString *)environment;
+ (void)setEnvironment:(NSString *)environment;

// You SHOULD preconnect to PayPal to prepare the device for processing payments.
// This improves the user experience, by making the presentation of the
// PayPalPaymentViewController faster. The preconnect is valid for a limited time, so
// the recommended time to preconnect is when you present the view controller in
// which users may choose to initiate payment.
+ (void)prepareForPaymentUsingClientId:(NSString *)clientId;

// If you have prior knowledge of the user's email and/or phone number,
// you MAY supply them here for a better user experience. Note that these
// defaults may be overridden. If you provide a default phone number,
// be sure to provide the country code as well.
@property(nonatomic, copy, readwrite) NSString *defaultUserEmail;
@property(nonatomic, copy, readwrite) NSString *defaultUserPhoneCountryCode;
@property(nonatomic, copy, readwrite) NSString *defaultUserPhoneNumber;

// If set to YES, PayPalPaymentViewController will only support paying with PayPal, not with credit cards.
@property(nonatomic, assign, readwrite) BOOL hideCreditCardButton;

// The SDK version that you are using; follows http://semver.org/.
// Please be sure to include the library version in any tech support requests.
+ (NSString *)libraryVersion;

@end
