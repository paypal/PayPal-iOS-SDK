//
//  PayPalOAuthScopes.h
//
//  Version 2.17.0
//
//  Copyright (c) 2014-2016 PayPal, Inc. All rights reserved.
//  All rights reserved.
//

// Currently available scope-values to which the user can be asked to consent.
// @see https://developer.paypal.com/docs/integration/direct/identity/attributes/ for more details

/// Authorize charges for future purchases paid for with PayPal.
extern NSString * _Nonnull const kPayPalOAuth2ScopeFuturePayments;
/// Share basic account information.
extern NSString * _Nonnull const kPayPalOAuth2ScopeProfile;
/// Basic Authentication.
extern NSString * _Nonnull const kPayPalOAuth2ScopeOpenId;
/// Share your personal and account information.
extern NSString * _Nonnull const kPayPalOAuth2ScopePayPalAttributes;
/// Share your email address.
extern NSString * _Nonnull const kPayPalOAuth2ScopeEmail;
/// Share your account address.
extern NSString * _Nonnull const kPayPalOAuth2ScopeAddress;
/// Share your phone number.
extern NSString * _Nonnull const kPayPalOAuth2ScopePhone;

