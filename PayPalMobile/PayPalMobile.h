//
//  PayPalMobile.h
//
//  Version 2.2.1
//
//  Copyright (c) 2014, PayPal
//  All rights reserved.
//

//  All-in-one import for the PayPal Mobile SDK

#import "PayPalConfiguration.h"
#import "PayPalFuturePaymentViewController.h"
#import "PayPalOAuthScopes.h"
#import "PayPalPayment.h"
#import "PayPalPaymentViewController.h"
#import "PayPalProfileSharingViewController.h"

/// Production (default): Normal, live environment. Real money gets moved.
/// This environment MUST be used for App Store submissions.
extern NSString *const PayPalEnvironmentProduction;
/// Sandbox: Uses the PayPal sandbox for transactions. Useful for development.
extern NSString *const PayPalEnvironmentSandbox;
/// NoNetwork: Mock mode. Does not submit transactions to PayPal. Fakes successful responses. Useful for unit tests.
extern NSString *const PayPalEnvironmentNoNetwork;

@interface PayPalMobile : NSObject

/// You MUST call this method to initialize the PayPal Mobile SDK.
///
/// The PayPal Mobile SDK can operate in different environments to facilitate development and testing.
/// See PayPalEnvironmentProduction, PayPalEnvironmentSandbox, PayPalEnvironmentNoNetwork for more details.
/// @param clientIdsForEnvironments Your client id for each relevant environment, as obtained from developer.paypal.com.
/// You do not need to provide a client id for PayPalEnvironmentNoNetwork.
/// For example,
///  @{PayPalEnvironmentProduction : @"my-client-id-for-Production",
///    PayPalEnvironmentSandbox : @"my-client-id-for-Sandbox"}
+ (void)initializeWithClientIdsForEnvironments:(NSDictionary *)clientIdsForEnvironments;

/// You MUST preconnect to PayPal to prepare the device for processing payments.
/// This improves the user experience because it allows the PayPal Mobile SDK to make its
/// setup request early and in the background.
///
/// The preconnect is valid for a limited time, so the recommended time to preconnect
/// is when you present the UI in which users *might* choose to initiate payment.
///
/// Calling this method a second time, with a different value, will change the environment used
/// by subsequently allocated PayPal Mobile SDK viewcontrollers. Existing viewcontrollers
/// will be unaffected.
///
/// @param environment
/// The PayPal Mobile SDK can operate in different environments to facilitate development and testing.
/// See PayPalEnvironmentProduction, PayPalEnvironmentSandbox, PayPalEnvironmentNoNetwork for more details.
+ (void)preconnectWithEnvironment:(NSString *)environment;

/// Once a user has consented to future payments, when the user subsequently initiates a PayPal payment
/// from their device to be completed by your server, PayPal uses a Correlation ID to verify that the
/// payment is originating from a valid, user-consented device+application.
/// This helps reduce fraud and decrease declines.
/// This method MUST be called prior to initiating a pre-consented payment (a "future payment") from a mobile device.
/// Pass the result to your server, to include in the payment request sent to PayPal.
/// Do not otherwise cache or store this value.
/// @param environment
/// The PayPal Mobile SDK can operate in different environments to facilitate development and testing.
/// See PayPalEnvironmentProduction, PayPalEnvironmentSandbox, PayPalEnvironmentNoNetwork for more details.
/// @return applicationCorrelationID Your server will send this to PayPal in a 'Paypal-Application-Correlation-Id' header.
+ (NSString *)applicationCorrelationIDForEnvironment:(NSString *)environment;

/// Delete all previously remembered user data (credit card, email, phone, PayPal account),
/// for all environments. (See PayPalEnvironmentProduction, PayPalEnvironmentSandbox, PayPalEnvironmentNoNetwork.)
///
/// You might choose to call this method, for example, when a user logs out of your app.
///
/// @note Do NOT call this method while any PayPal Mobile SDK viewcontroller is active.
/// @see PayPalConfiguration rememberUser.
+ (void)clearAllUserData;

/// @return The version of the PayPal Mobile SDK in use. Version numbering follows http://semver.org/.
/// @note Please be sure to include this library version in tech support requests.
+ (NSString *)libraryVersion;


@end
