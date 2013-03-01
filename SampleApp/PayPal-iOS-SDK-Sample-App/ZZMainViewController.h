//
//  ZZMainViewController.h
//  PayPal-iOS-SDK-Sample-App
//
//  Copyright (c) 2013 PayPal. All rights reserved.
//

#import "ZZFlipsideViewController.h"
#import "PayPalMobile.h"

@interface ZZMainViewController : UIViewController <PayPalPaymentDelegate, ZZFlipsideViewControllerDelegate, UIPopoverControllerDelegate>

@property(nonatomic, strong, readwrite) UIPopoverController *flipsidePopoverController;

@property(nonatomic, strong, readwrite) NSString *environment;
@property(nonatomic, assign, readwrite) BOOL acceptCreditCards;

@end
