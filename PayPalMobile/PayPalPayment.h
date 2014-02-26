//
//  PayPalPayment.h
//
//  Version 2.0.1
//
//  Copyright (c) 2013, PayPal
//  All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSInteger, PayPalPaymentIntent) {
  PayPalPaymentIntentSale = 0,
  PayPalPaymentIntentAuthorize = 1,
  };

/// The PayPalPaymentDetails class defines optional amount details.
/// @see https://developer.paypal.com/webapps/developer/docs/api/#details-object for more details.
@interface PayPalPaymentDetails : NSObject <NSCopying>

/// Amount charged for shipping. 10 characters max with support for 2 decimal places.
@property(nonatomic, copy, readwrite) NSDecimalNumber *shipping;

/// Sub-total (amount) of items being paid for. 10 characters max with support for 2 decimal places.
@property(nonatomic, copy, readwrite) NSDecimalNumber *subtotal;

/// Amount charged for tax. 10 characters max with support for 2 decimal places.
@property(nonatomic, copy, readwrite) NSDecimalNumber *tax;

@end


@interface PayPalPayment : NSObject <NSCopying>

/// Convenience constructor. Returns a PayPalPayment with the specified amount, currency code, and short description.
/// See the documentation of the individual properties for more detail.
/// @param amount The amount of the payment.
/// @param currencyCode The ISO 4217 currency for the payment.
/// @param shortDescription A short descripton of the payment.
/// @param intent PayPalPaymentIntentSale for an immediate payment;
///                PayPalPaymentIntentAuthorize for payment authorization only, to be captured separately at a later time.
+ (PayPalPayment *)paymentWithAmount:(NSDecimalNumber *)amount
                        currencyCode:(NSString *)currencyCode
                    shortDescription:(NSString *)shortDescription
                              intent:(PayPalPaymentIntent)intent;


#pragma mark - Required properties

/// ISO standard currency code (http://en.wikipedia.org/wiki/ISO_4217).
@property(nonatomic, copy, readwrite) NSString *currencyCode;

/// Amount in the given currency to process. Must be positive.
@property(nonatomic, copy, readwrite) NSDecimalNumber *amount;

/// A short description of the transaction, for display to the user.
/// The description will be truncated for display, if necessary;
/// limiting it to about 20 characters should prevent truncation in most cases.
@property(nonatomic, copy, readwrite) NSString *shortDescription;

/// The intent of this payment:
/// - PayPalPaymentIntentSale for an immediate payment
/// - PayPalPaymentIntentAuthorize for payment authorization only,
///   to be captured separately at a later time.
/// Defaults to PayPalPaymentIntentSale.
@property(nonatomic, assign, readwrite) PayPalPaymentIntent intent;


#pragma mark - Optional properties

/// Optional payment details @see PaymentDetails properties.
@property (nonatomic, copy, readwrite) PayPalPaymentDetails *paymentDetails;

/// Optional Build Notation code ("BN code"), obtained from partnerprogram@paypal.com,
/// for your tracking purposes.
@property(nonatomic, copy, readwrite) NSString *bnCode;


#pragma mark - Convenience getters

/// Can this payment be processed?
/// A payment might not be processable if, for example:
/// - the amount is non-positive,
/// - the currency is invalid,
/// - the amount includes more decimal fraction digits than the currency allows,
/// - there's no description, or
/// - the payment has already been processed.
@property(nonatomic, assign, readonly) BOOL processable;

/// The payment amount rendered with the appropriate currency symbol.
@property(nonatomic, copy, readonly) NSString *localizedAmountForDisplay;


#pragma mark - Properties available on completed purchases

/// Full payment confirmation, with lots of details including the proof
/// of payment token. You should send the entire confirmation
/// dictionary to your servers and process it there, for maximum flexibility.
/// See https://developer.paypal.com/webapps/developer/docs/integration/mobile/verify-mobile-payment/
/// for more details.
@property(nonatomic, readonly, copy) NSDictionary *confirmation;


@end
