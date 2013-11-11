//
//  PayPalPaymentViewController.h
//
//  Version 1.3.6
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
// See https://developer.paypal.com/webapps/developer/docs/integration/mobile/verify-mobile-payment/
// for details.

#pragma mark - Delegates and notifications

/// Exactly one of these delegate methods will get called when the UI completes.
/// You MUST dismiss the modal view controller from these delegate methods.
@protocol PayPalPaymentDelegate <NSObject>
@required

/// User canceled the payment process.
- (void)payPalPaymentDidCancel;

/// User successfully completed the payment.
/// See https://developer.paypal.com/webapps/developer/docs/integration/mobile/verify-mobile-payment/ for
/// information about payment verification.
/// @param completedPayment completedPayment.confirmation contains information needed to verify the payment.
- (void)payPalPaymentDidComplete:(PayPalPayment *)completedPayment;

@end

/// This notification is no longer necessary and has been deprecated.
extern NSString *const PayPalTransactionDidSucceedNotification __attribute__((deprecated("use payPalPaymentDidComplete: method instead")));

/// Production (default): Normal, live environment. Real money gets moved.
/// This environment MUST be used for App Store submissions.
extern NSString *const PayPalEnvironmentProduction;
/// Sandbox: Uses the PayPal sandbox for transactions. Useful for development.
extern NSString *const PayPalEnvironmentSandbox;
/// NoNetwork: Mock mode. Does not submit transactions to PayPal. Fakes successful responses. Useful for unit tests.
extern NSString *const PayPalEnvironmentNoNetwork;


#pragma mark - PayPalPaymentViewController

@interface PayPalPaymentViewController : UINavigationController

/// The designated initalizer. A new view controller MUST be initialized for each use.
/// @param clientId The client id is a public credential identifying the developer. Get yours from developer.paypal.com.
/// @param payPalAccountEmailAddress The email address associated with the PayPal account used to get your client id.
/// @param payerId A unique identifier for the user who is paying, such as a customer number
///    or email address. Do NOT use hardware based identifiers. If you do not have an appropriate payerId,
///    you MAY pass nil.
///    If payerId is nil, then PayPalPaymentViewController will treat the user as a one-time "guest":
///    - if the user logs into their PayPal account, then at the completion of their
///      transaction they will be logged back out; subsequent payments will require fresh logins.
///    - if the user pays with a credit card, that credit card information will not be "remembered"
///      for subsequent payments.
/// @param payment The payment to be processed.
/// @param delegate The delegate you want to receive updates about the payment.
- (instancetype)initWithClientId:(NSString *)clientId
         receiverEmail:(NSString *)payPalAccountEmailAddress
               payerId:(NSString *)payerId
               payment:(PayPalPayment *)payment
              delegate:(id<PayPalPaymentDelegate>)delegate;

/// Delegate access
@property(nonatomic, unsafe_unretained, readwrite) id<PayPalPaymentDelegate> paymentDelegate;

/// The preferred language for all strings appearing in the user interface.
/// If not set, or if set to nil, defaults to the device's current language setting.
///
/// Can be specified as a language code ("en", "fr", "zh-Hans", etc.) or as a locale ("en_AU", "fr_FR", "zh-Hant_HK", etc.).
/// If the library does not contain localized strings for a specified locale, then will fall back to the language. E.g., "es_CO" -> "es".
/// If the library does not contain localized strings for a specified language, then will fall back to American English.
///
/// If you specify only a language code, and that code matches the device's currently preferred language,
/// then the library will attempt to use the device's current region as well.
/// E.g., specifying "en" on a device set to "English" and "United Kingdom" will result in "en_GB".
///
/// These localizations are currently included:
/// da,de,en,en_AU,en_GB,en_SV,es,es_MX,fr,he,it,ja,ko,nb,nl,pl,pt,pt_BR,ru,sv,tr,zh-Hans,zh-Hant_HK,zh-Hant_TW.
@property(nonatomic, copy, readwrite) NSString *languageOrLocale;

/// @return The current active environment.
+ (NSString *)environment;

/// PayPalPaymentViewController can operate in different environments to facilitate development and testing;
/// see the PayPalEnvironment* constants for details about the available environments.
/// The environment MUST NOT be changed while a PayPalPaymentViewController
/// is visible. Setting the environment will cancel all network operations
/// and reset local state.
/// 
/// @param environment The desired environment, e.g. PayPalEnvironmentNoNetwork. The default is PayPalEnvironmentProduction.
+ (void)setEnvironment:(NSString *)environment;

/// You SHOULD preconnect to PayPal to prepare the device for processing payments.
/// This improves the user experience, by making the presentation of the
/// PayPalPaymentViewController faster. The preconnect is valid for a limited time, so
/// the recommended time to preconnect is when you present the view controller in
/// which users may choose to initiate payment.
///
/// @param clientId Your client id. Get it at developer.paypal.com. This MUST be the same client id used
///     later to initialize the PayPalPaymentViewController.
+ (void)prepareForPaymentUsingClientId:(NSString *)clientId;

// If you have prior knowledge of the user's email and/or phone number,
// you MAY supply them here for a better user experience. Note that
// values you provide may be overridden.

/// Default user email address to be shown on the PayPal login view.
@property(nonatomic, copy, readwrite) NSString *defaultUserEmail;
/// Default user phone country code used in the PayPal login view.
@property(nonatomic, copy, readwrite) NSString *defaultUserPhoneCountryCode;
/// Default user phone number to be shown in the PayPal login view.
/// If you set defaultUserPhoneNumber, be sure to also set defaultUserPhoneCountryCode.
@property(nonatomic, copy, readwrite) NSString *defaultUserPhoneNumber;

/// If set to YES, PayPalPaymentViewController will only support paying with PayPal, not with credit cards.
@property(nonatomic, assign, readwrite) BOOL hideCreditCardButton;

/// Normally, the SDK blurs the screen when the app is backgrounded,
/// to obscure credit card or PayPal account details in the iOS-saved screenshot.
/// If your app already does its own blurring upon backgrounding, you might choose to disable this.
/// Defaults to NO.
@property(nonatomic, assign, readwrite) BOOL disableBlurWhenBackgrounding;


/// PayPalPaymentViewControllerState See the state property for context.
typedef NS_ENUM(NSInteger, PayPalPaymentViewControllerState) {
  /// The payment has not been sent. You MAY safely dismiss the PayPalPaymentViewController.
  PayPalPaymentViewControllerStateUnsent = 0,
  /// The payment is in progress. You MUST NOT dismiss the PayPalPaymentViewController.
  PayPalPaymentViewControllerStateInProgress = 1,
  /// The payment is completed. However, you MUST NOT dismiss the PayPalPaymentViewController UNTIL your
  /// payPalPaymentDidComplete: delegate method has been called.
  /// PayPalPaymentViewControllerStateSuccessful has been deprecated. You should remove
  /// code that relies on it, and treat such conditions similarly to
  /// PayPalPaymentViewControllerStateInProgress.
  PayPalPaymentViewControllerStateSuccessful __attribute__((deprecated("Your payPalPaymentDidComplete: method will be called when a transaction succeeds."))) = 2,
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
/// - If the payment goes through successfully, the state changes to
///   PayPalPaymentViewControllerStateSuccessful, and your app's payPalPaymentDidComplete:
///   method is called.
/// - If the payment fails, the state changes back to PayPalPaymentViewControllerStateUnsent.
///   (An appropriate error message is also displayed to the user).
@property(nonatomic, assign, readonly) PayPalPaymentViewControllerState state;

/// @return The SDK version in use. Versions follow http://semver.org/.
/// Please be sure to include this library version in tech support requests.
+ (NSString *)libraryVersion;

@end
