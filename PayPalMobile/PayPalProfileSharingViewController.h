//
//  PayPalProfileSharingViewController.h
//
//  Version 2.13.0
//
//  Copyright (c) 2014-2016 PayPal, Inc. All rights reserved.
//  All rights reserved.
//

#import <UIKit/UIKit.h>
#import "PayPalConfiguration.h"
#import "PayPalOAuthScopes.h"

@class PayPalProfileSharingViewController;
typedef void (^PayPalProfileSharingDelegateCompletionBlock)(void);

#pragma mark - PayPalProfileSharingDelegate

/// Exactly one of these two required delegate methods will get called when the UI completes.
/// You MUST dismiss the modal view controller from these required delegate methods.
@protocol PayPalProfileSharingDelegate <NSObject>
@required

/// User canceled without consenting.
/// Your code MUST dismiss the PayPalProfileSharingViewController.
/// @param profileSharingViewController The PayPalProfileSharingViewController that the user canceled without consenting.
- (void)userDidCancelPayPalProfileSharingViewController:(nonnull PayPalProfileSharingViewController *)profileSharingViewController;

/// User successfully logged in and consented.
/// The PayPalProfileSharingViewController's activity indicator has been dismissed.
/// Your code MAY deal with the profileSharingAuthorization, if it did not already do so within your optional
///     PayPalProfileSharingViewController:userWillLogInWithAuthorization:completionBlock: method.
/// Your code MUST dismiss the PayPalProfileSharingViewController.
/// @param profileSharingViewController The PayPalProfileSharingViewController where the user successfully consented.
/// @param authorization The authorization response, which you will return to your server.
- (void)payPalProfileSharingViewController:(nonnull PayPalProfileSharingViewController *)profileSharingViewController
             userDidLogInWithAuthorization:(nonnull NSDictionary *)profileSharingAuthorization;

@optional
/// User successfully logged in and consented.
/// The PayPalProfileSharingViewController's activity indicator is still visible.
/// Your code MAY deal with the profileSharingAuthorization; e.g., send it to your server and await confirmation.
/// Your code MUST finish by calling the completionBlock.
/// Your code must NOT dismiss the PayPalProfileSharingViewController.
/// @param profileSharingViewController The PayPalProfileSharingViewController where the user successfully consented.
/// @param authorization The authorization response, which you will return to your server.
/// @param completionBlock Block to execute when your processing is done.
- (void)payPalProfileSharingViewController:(nonnull PayPalProfileSharingViewController *)profileSharingViewController
            userWillLogInWithAuthorization:(nonnull NSDictionary *)profileSharingAuthorization
                           completionBlock:(nonnull PayPalProfileSharingDelegateCompletionBlock)completionBlock;
@end


#pragma mark - PayPalProfileSharingViewController

@interface PayPalProfileSharingViewController : UINavigationController

/// The designated initalizer. A new view controller MUST be initialized for each use.
/// @param scopeValues Set of requested scope-values. Each scope-value is defined in PayPalOAuthScopes.h.
/// @param configuration The configuration to be used for the lifetime of the controller
///     The configuration properties merchantName, merchantPrivacyPolicyURL, and merchantUserAgreementURL must be provided.
/// @param delegate The delegate you want to receive updates about the profile-sharing authorization.
- (nullable instancetype)initWithScopeValues:(nonnull NSSet *)scopeValues
                               configuration:(nonnull PayPalConfiguration *)configuration
                                    delegate:(nullable id<PayPalProfileSharingDelegate>)delegate;

/// Delegate access
@property (nonatomic, weak, readonly) id<PayPalProfileSharingDelegate> profileSharingDelegate;

@end
