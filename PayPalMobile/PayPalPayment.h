//
//  PayPalPayment.h
//
//  Version 2.18.1
//
//  Copyright (c) 2014-2016 PayPal, Inc. All rights reserved.
//  All rights reserved.
//

#import <Foundation/Foundation.h>

#pragma mark - PayPalPaymentIntent

typedef NS_ENUM(NSInteger, PayPalPaymentIntent) {
  PayPalPaymentIntentSale = 0,
  PayPalPaymentIntentAuthorize = 1,
  PayPalPaymentIntentOrder = 2,
  };


#pragma mark - PayPalPaymentDetails

/// The PayPalPaymentDetails class defines optional amount details.
/// @see https://developer.paypal.com/webapps/developer/docs/api/#details-object for more details.
@interface PayPalPaymentDetails : NSObject <NSCopying>

/// Convenience constructor. NSDecimalNumber parameters must be initialized using a string value.
/// See the documentation of the individual properties for more detail.
/// @param subtotal Sum of amounts for all items in this transaction.
/// @param shipping Shipping and handling amount for the overall transaction.
/// @param tax Tax amount for the overall transaction.
+ (nonnull PayPalPaymentDetails *)paymentDetailsWithSubtotal:(nullable NSDecimalNumber *)subtotal
                                                withShipping:(nullable NSDecimalNumber *)shipping
                                                     withTax:(nullable NSDecimalNumber *)tax;

/// Sub-total (amount) of items being paid for. 10 characters max with support for 2 decimal places.
@property(nullable, nonatomic, copy, readwrite) NSDecimalNumber *subtotal;

/// Amount charged for shipping. 10 characters max with support for 2 decimal places.
@property(nullable, nonatomic, copy, readwrite) NSDecimalNumber *shipping;

/// Amount charged for tax. 10 characters max with support for 2 decimal places.
@property(nullable, nonatomic, copy, readwrite) NSDecimalNumber *tax;

@end


#pragma mark - PayPalItem

/// The PayPalItem class defines an optional itemization for a payment.
/// @see https://developer.paypal.com/docs/api/#item-object for more details.
@interface PayPalItem : NSObject <NSCopying>

/// Convenience constructor. NSDecimalNumber parameters must be initialized using a string value.
/// See the documentation of the individual properties for more detail.
/// @param name Name of the item.
/// @param quantity Number of units.
/// @param price Unit price for this item.
/// @param currency ISO standard currency code.
/// @param sku The stock keeping unit for this item.
+ (nonnull PayPalItem *)itemWithName:(nonnull NSString *)name
                        withQuantity:(NSUInteger)quantity
                           withPrice:(nonnull NSDecimalNumber *)price
                        withCurrency:(nonnull NSString *)currency
                             withSku:(nullable NSString *)sku;

/// Convenience utility.
/// Returns the total of (quantity * price) for all of the items.
/// @param array of PayPalItem objects.
+ (nonnull NSDecimalNumber *)totalPriceForItems:(nonnull NSArray *)items;

/// Item name. 127 characters max. Required.
@property(nonnull, nonatomic, copy, readwrite) NSString *name;

/// Number of a particular item. 10 characters max. Required.
@property(nonatomic, assign, readwrite) NSUInteger quantity;

/// Item cost. 10 characters max. May be negative for "coupon" etc. Required.
@property(nonnull, nonatomic, copy, readwrite) NSDecimalNumber *price;

/// ISO standard currency code (http://en.wikipedia.org/wiki/ISO_4217). Required.
@property(nonnull, nonatomic, copy, readwrite) NSString *currency;

/// Stock keeping unit corresponding (SKU) to item. 50 characters max.
@property(nullable, nonatomic, copy, readwrite) NSString *sku;

@end


#pragma mark - PayPalShippingAddress

/// The PayPalShippingAddress class defines an optional customer shipping address.
/// @see https://developer.paypal.com/webapps/developer/docs/api/#shippingaddress-object for more details
@interface PayPalShippingAddress : NSObject <NSCopying>

/// Convenience constructor.
/// See the documentation of the individual properties for more detail.
/// @param receipientName Name of recipient.
/// @param line1 First line of address.
/// @param line2 Second line of address, if any.
/// @param city City.
/// @param state State, county, etc., as appropriate.
/// @param postalCode Appropriate ZIP or postal code.
/// @param countryCode 2-letter country code.
+ (nonnull PayPalShippingAddress *)shippingAddressWithRecipientName:(nonnull NSString *)recipientName
                                                          withLine1:(nonnull NSString *)line1
                                                          withLine2:(nullable NSString *)line2
                                                           withCity:(nonnull NSString *)city
                                                          withState:(nullable NSString *)state
                                                     withPostalCode:(nullable NSString *)postalCode
                                                    withCountryCode:(nonnull NSString *)countryCode;

/// Name of the recipient at this address. 50 characters max. Required.
@property(nonnull, nonatomic, copy, readwrite) NSString *recipientName;

/// Line 1 of the address (e.g., Number, street, etc). 100 characters max. Required.
@property(nonnull, nonatomic, copy, readwrite) NSString *line1;

/// Line 2 of the address (e.g., Suite, apt #, etc). 100 characters max.
@property(nullable, nonatomic, copy, readwrite) NSString *line2;

/// City name. 50 characters max. Required.
@property(nonnull, nonatomic, copy, readwrite) NSString *city;

/// 2-letter code for US states, and the equivalent for other countries. 100 characters max. Required in certain countries.
@property(nullable, nonatomic, copy, readwrite) NSString *state;

/// ZIP code or equivalent is usually required for countries that have them. 20 characters max. Required in certain countries.
@property(nullable, nonatomic, copy, readwrite) NSString *postalCode;

/// 2-letter country code. 2 characters max. Required.
@property(nonnull, nonatomic, copy, readwrite) NSString *countryCode;

@end


#pragma mark - PayPalPayment

@interface PayPalPayment : NSObject <NSCopying>

/// Convenience constructor. NSDecimalNumber parameters must be initialized using a string value.
/// See the documentation of the individual properties for more detail.
/// @param amount The amount of the payment.
/// @param currencyCode The ISO 4217 currency for the payment.
/// @param shortDescription A short descripton of the payment.
/// @param intent PayPalPaymentIntentSale for an immediate payment;
///                PayPalPaymentIntentAuthorize for payment authorization only, to be captured separately at a later time;
///                PayPalPaymentIntentOrder for taking an order, with authorization and capture to be done separately at a later time.
+ (nonnull PayPalPayment *)paymentWithAmount:(nonnull NSDecimalNumber *)amount
                                currencyCode:(nonnull NSString *)currencyCode
                            shortDescription:(nonnull NSString *)shortDescription
                                      intent:(PayPalPaymentIntent)intent;


#pragma mark Required properties

/// ISO standard currency code (http://en.wikipedia.org/wiki/ISO_4217).
@property(nonnull, nonatomic, copy, readwrite) NSString *currencyCode;

/// Amount in the given currency to process. Must be positive.
@property(nonnull, nonatomic, copy, readwrite) NSDecimalNumber *amount;

/// A short description of the transaction, for display to the user.
/// The description will be truncated for display, if necessary;
/// limiting it to about 20 characters should prevent truncation in most cases.
@property(nonnull, nonatomic, copy, readwrite) NSString *shortDescription;

/// The intent of this payment:
///
/// - PayPalPaymentIntentSale for an immediate payment
///
/// - PayPalPaymentIntentAuthorize for payment authorization only,
///   to be captured separately at a later time.
///
/// - PayPalPaymentIntentOrder for taking an order, with authorization
///   and capture to be done separately at a later time.
///
///   (PayPalPaymentIntentOrder is valid only for PayPal payments,
///   not credit card payments.)
///
/// Defaults to PayPalPaymentIntentSale.
@property(nonatomic, assign, readwrite) PayPalPaymentIntent intent;


#pragma mark Optional properties

/// Optional payment details @see PayPalPaymentDetails properties.
/// @note If you provide payment details, be sure that `subtotal`, `shipping`, and `tax`
/// sum exactly to the payment `amount`.
@property (nullable, nonatomic, copy, readwrite) PayPalPaymentDetails *paymentDetails;

/// Optional array of PayPalItem objects. @see PayPalItem
/// @note If you provide one or more items, be sure that the various prices correctly
/// sum to the payment `amount` or to `paymentDetails.subtotal`.
@property (nullable, nonatomic, copy, readwrite) NSArray *items;

/// Optional customer shipping address, if your app wishes to provide this to the SDK.
@property (nullable, nonatomic, copy, readwrite) PayPalShippingAddress *shippingAddress;

/// Optional invoice number, for your tracking purposes. (up to 256 characters)
@property (nullable, nonatomic, copy, readwrite) NSString *invoiceNumber;

/// Optional text, for your tracking purposes. (up to 256 characters)
@property (nullable, nonatomic, copy, readwrite) NSString *custom;

/// Optional text which will appear on the customer's credit card statement. (up to 22 characters)
@property (nullable, nonatomic, copy, readwrite) NSString *softDescriptor;

/// Optional third-party receiver for single payments. When specified, payments will be sent to this receiver, instead of the account of the provided client_id.
@property (nullable, nonatomic, copy, readwrite) NSString *payeeEmail;

/// Optional Build Notation code ("BN code"), obtained from partnerprogram@paypal.com,
/// for your tracking purposes.
@property(nullable, nonatomic, copy, readwrite) NSString *bnCode;


#pragma mark Convenience getters

/// Can this payment be processed?
/// A payment might not be processable if, for example:
/// - the amount is non-positive,
/// - the currency is invalid,
/// - the amount includes more decimal fraction digits than the currency allows,
/// - there's no description, or
/// - the payment has already been processed.
@property(nonatomic, assign, readonly) BOOL processable;

/// The payment amount rendered with the appropriate currency symbol.
@property(nonnull, nonatomic, copy, readonly) NSString *localizedAmountForDisplay;


#pragma mark Properties available on completed purchases

/// Full payment confirmation, with lots of details including the proof
/// of payment token. You should send the entire confirmation
/// dictionary to your servers and process it there, for maximum flexibility.
/// See https://developer.paypal.com/webapps/developer/docs/integration/mobile/verify-mobile-payment/
/// and https://developer.paypal.com/webapps/developer/docs/integration/direct/capture-payment/
/// for more details.
@property(nonnull, nonatomic, readonly, copy) NSDictionary *confirmation;


@end
