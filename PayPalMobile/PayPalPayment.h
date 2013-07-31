//
//  PayPalPayment.h
//
//  Version 1.1.1
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

#import <Foundation/Foundation.h>

@interface PayPalPayment : NSObject <NSCopying>

// Convenience constructor. See below for details on the parameters.
+ (PayPalPayment *)paymentWithAmount:(NSDecimalNumber *)amount
                        currencyCode:(NSString *)currencyCode
                    shortDescription:(NSString *)shortDescription;


#pragma mark - Required properties

// ISO standard currency code (http://en.wikipedia.org/wiki/ISO_4217)
@property(nonatomic, copy, readwrite) NSString *currencyCode;

// Amount in the given currency to process. Must be positive.
@property(nonatomic, copy, readwrite) NSDecimalNumber *amount;

// A short description of the transaction, for display to the user.
// The description will be truncated for display, if necessary;
// limiting it to about 20 characters should prevent truncation in most cases.
@property(nonatomic, copy, readwrite) NSString *shortDescription;


#pragma mark - Convenience getters

// Can this payment be processed?
// A payment might not be processable if, for example:
// - the amount is non-positive,
// - the currency is invalid,
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
