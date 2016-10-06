//
//  ZZFlipsideViewController.h
//  PayPal-iOS-SDK-Sample-App
//
//  Copyright (c) 2014-2016 PayPal, Inc. All rights reserved.
//  All rights reserved.
//

#import <UIKit/UIKit.h>
#import "PayPalMobile.h"

@class ZZFlipsideViewController;

@protocol ZZFlipsideViewControllerDelegate

- (void)setPayPalEnvironment:(NSString *)environment;

@property(nonatomic, strong, readwrite) NSString *environment;
@property(nonatomic, strong, readwrite) NSString *resultText;

@end

#pragma mark -

@interface ZZFlipsideViewController : UIViewController

@property(weak, nonatomic) id <ZZFlipsideViewControllerDelegate> delegate;

@end
