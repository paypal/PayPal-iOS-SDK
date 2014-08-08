//
//  PayPalConfiguration.h
//
//  Version 2.2.1
//
//  Copyright (c) 2014, PayPal
//  All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSInteger, PayPalShippingAddressOption) {
  PayPalShippingAddressOptionNone = 0,
  PayPalShippingAddressOptionProvided = 1,
  PayPalShippingAddressOptionPayPal = 2,
  PayPalShippingAddressOptionBoth = 3,
};

/// You use a PayPalConfiguration object to configure many aspects of how the SDK behaves.
@interface PayPalConfiguration : NSObject <NSCopying>

/// Optional default user email address to be shown on the PayPal login view.
/// Will be overridden by email used in most recent PayPal login.
/// @see forceDefaultsInSandbox
@property(nonatomic, copy, readwrite) NSString *defaultUserEmail;
/// Optional default user phone country code used in the PayPal login view.
/// Will be overridden by phone country code used in most recent PayPal login.
/// @see forceDefaultsInSandbox
@property(nonatomic, copy, readwrite) NSString *defaultUserPhoneCountryCode;
/// Optional default user phone number to be shown in the PayPal login view.
/// Will be overridden by phone number used in most recent PayPal login.
/// @note If you set defaultUserPhoneNumber, be sure to also set defaultUserPhoneCountryCode.
/// @see forceDefaultsInSandbox
@property(nonatomic, copy, readwrite) NSString *defaultUserPhoneNumber;

/// Your company name, as it should be displayed to the user
/// when requesting consent via a PayPalFuturePaymentViewController or a PayPalProfileSharingViewController.
@property(nonatomic, copy, readwrite) NSString *merchantName;
/// URL of your company's privacy policy, which will be offered to the user
/// when requesting consent via a PayPalFuturePaymentViewController or a PayPalProfileSharingViewController.
@property(nonatomic, copy, readwrite) NSURL *merchantPrivacyPolicyURL;
/// URL of your company's user agreement, which will be offered to the user
/// when requesting consent via a PayPalFuturePaymentViewController or a PayPalProfileSharingViewController.
@property(nonatomic, copy, readwrite) NSURL *merchantUserAgreementURL;

/// If set to NO, the SDK will only support paying with PayPal, not with credit cards.
/// This applies only to single payments (via PayPalPaymentViewController).
/// Future payments (via PayPalFuturePaymentViewController) always use PayPal.
/// Defaults to YES.
@property(nonatomic, assign, readwrite) BOOL acceptCreditCards;

/// For single payments, options for the shipping address.
/// - PayPalShippingAddressOptionNone: no shipping address applies.
/// - PayPalShippingAddressOptionProvided: shipping address will be provided by your app,
///   in the shippingAddress property of PayPalPayment.
/// - PayPalShippingAddressOptionPayPal: user will choose from shipping addresses on file
///   for their PayPal account.
/// - PayPalShippingAddressOptionBoth: user will choose from the shipping address provided by your app,
///   in the shippingAddress property of PayPalPayment, plus the shipping addresses on file for the user's PayPal account.
/// Defaults to PayPalShippingAddressOptionNone.
@property(nonatomic, assign, readwrite) PayPalShippingAddressOption payPalShippingAddressOption;

/// If set to YES, then if the user pays via their PayPal account,
/// the SDK will remember the user's PayPal username or phone number;
/// if the user pays via their credit card, then the SDK will remember
/// the PayPal Vault token representing the user's credit card.
///
/// If set to NO, then any previously-remembered username, phone number, or
/// credit card token will be erased, and subsequent payment information will
/// not be remembered.
///
/// Defaults to YES.
///
/// @see PayPalMobile +(void)clearAllUserData
@property(nonatomic, assign, readwrite) BOOL rememberUser;

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
/// ar,da,de,en,en_AU,en_GB,en_SE,es,es_MX,fr,he,it,ja,ko,ms,nb,nl,pl,pt,pt_BR,ru,sv,th,tr,zh-Hans,zh-Hant,zh-Hant_TW.
@property(nonatomic, copy, readwrite) NSString *languageOrLocale;

/// Normally, the SDK blurs the screen when the app is backgrounded,
/// to obscure credit card or PayPal account details in the iOS-saved screenshot.
/// If your app already does its own blurring upon backgrounding, you might choose to disable this.
/// Defaults to NO.
@property(nonatomic, assign, readwrite) BOOL disableBlurWhenBackgrounding;

/// Sandbox credentials can be difficult to type on a mobile device. Setting this flag to YES will
/// cause the sandboxUserPassword and sandboxUserPin to always be pre-populated into login fields.
///
/// Defaults to NO.
///
/// This setting will have no effect if the environment is PayPalEnvironmentProduction.
///
/// @note: Use defaultUserEmail, defaultUserPhoneCountryCode, and defaultUserPhoneNumber to
/// pre-populate the email and phone fields as well.
@property (nonatomic, assign, readwrite) BOOL forceDefaultsInSandbox;

/// Password to use for sandbox if 'forceDefaultsInSandbox' is set.
@property (nonatomic, copy, readwrite) NSString* sandboxUserPassword;
/// PIN to use for sandbox if 'forceDefaultsInSandbox' is set.
@property (nonatomic, copy, readwrite) NSString* sandboxUserPin;

@end
