//
//  PayPalPayment.h
//
//  Version 1.0.1
//
//  Copyright (c) 2013- PayPal. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface PayPalPayment : NSObject <NSCopying>


#pragma mark - Required properties

// ISO standard currency code (http://en.wikipedia.org/wiki/ISO_4217)
@property(nonatomic, copy, readwrite) NSString *currencyCode;

// Amount in the given currency to process. Must be positive.
@property(nonatomic, copy, readwrite) NSDecimalNumber *amount;

// Description of the transaction, for display to user. Should fit on one line.
@property(nonatomic, copy, readwrite) NSString *shortDescription;


#pragma mark - Convenience getters

// Can this payment be processed?
// A payment might not be processable if, for example:
// - the amount is non-positive,
// - the currency is invalid [NOTE: at this time, only USD is supported],
// - the amount includes more decimal fraction digits than the currency allows,
// - there's no description, or
// - the payment has already been processed.
@property(nonatomic, assign, readonly) BOOL processable;

// The payment amount rendered with the appropriate currency symbol.
@property(nonatomic, copy, readonly) NSString *localizedAmountForDisplay;


#pragma mark - Properties available on completed purchases

// Full payment confirmation, with lots of details including the proof
// of payment token. You should send the entire confirmation
// dictionary to your servers and process it there, for maximum flexibility.
@property(nonatomic, readonly, copy) NSDictionary *confirmation;


@end
