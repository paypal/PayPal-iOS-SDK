//
//  ZZMainViewController.m
//  PayPal-iOS-SDK-Sample-App
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

#import "ZZMainViewController.h"
#import <QuartzCore/QuartzCore.h>

#warning "Enter your credentials"
#define kPayPalClientId @"YOUR CLIENT ID HERE"
#define kPayPalReceiverEmail @"YOUR_PAYPAL_EMAIL@yourdomain.com"

@interface ZZMainViewController ()

@property(nonatomic, strong, readwrite) IBOutlet UIButton *payButton;
@property(nonatomic, strong, readwrite) IBOutlet UIView *successView;

@end

@implementation ZZMainViewController

- (void)viewDidLoad {
  [super viewDidLoad];
  self.title = @"PayPal iOS Library Demo";
  self.acceptCreditCards = YES;
  self.environment = PayPalEnvironmentNoNetwork;
  // Do any additional setup after loading the view, typically from a nib.

  self.successView.hidden = YES;

  NSLog(@"PayPal iOS SDK version: %@", [PayPalPaymentViewController libraryVersion]);
}

- (void)viewWillAppear:(BOOL)animated {
  [super viewWillAppear:YES];

  UIEdgeInsets insets = UIEdgeInsetsMake(0, 15.0f, 0, 14.0f);
  UIImage *payBackgroundImage = [[UIImage imageNamed:@"button_secondary.png"] resizableImageWithCapInsets:insets];
  UIImage *payBackgroundImageHighlighted = [[UIImage imageNamed:@"button_secondary_selected.png"] resizableImageWithCapInsets:insets];
  [self.payButton setBackgroundImage:payBackgroundImage forState:UIControlStateNormal];
  [self.payButton setBackgroundImage:payBackgroundImageHighlighted forState:UIControlStateHighlighted];
  [self.payButton setTitleColor:[UIColor darkGrayColor] forState:UIControlStateNormal];
  [self.payButton setTitleColor:[UIColor darkGrayColor] forState:UIControlStateHighlighted];
  
  // Optimization: Prepare for display of the payment UI by getting network work done early
  [PayPalPaymentViewController setEnvironment:self.environment];
  [PayPalPaymentViewController prepareForPaymentUsingClientId:kPayPalClientId];
}

- (void)didReceiveMemoryWarning {
  [super didReceiveMemoryWarning];
  // Dispose of any resources that can be recreated.
}

#pragma mark - Pay action

- (IBAction)pay {

  // Remove our last completed payment, just for demo purposes.
  self.completedPayment = nil;
  
  PayPalPayment *payment = [[PayPalPayment alloc] init];
  payment.amount = [[NSDecimalNumber alloc] initWithString:@"9.95"];
  payment.currencyCode = @"USD";
  payment.shortDescription = @"Hipster t-shirt";

  if (!payment.processable) {
    // This particular payment will always be processable. If, for
    // example, the amount was negative or the shortDescription was
    // empty, this payment wouldn't be processable, and you'd want
    // to handle that here.
  }

  // Any customer identifier that you have will work here. Do NOT use a device- or
  // hardware-based identifier.
  NSString *customerId = @"user-11723";

  // Set the environment:
  // - For live charges, use PayPalEnvironmentProduction (default).
  // - To use the PayPal sandbox, use PayPalEnvironmentSandbox.
  // - For testing, use PayPalEnvironmentNoNetwork.
  [PayPalPaymentViewController setEnvironment:self.environment];

  PayPalPaymentViewController *paymentViewController = [[PayPalPaymentViewController alloc] initWithClientId:kPayPalClientId
                                                                                               receiverEmail:kPayPalReceiverEmail
                                                                                                     payerId:customerId
                                                                                                     payment:payment
                                                                                                    delegate:self];
  paymentViewController.hideCreditCardButton = !self.acceptCreditCards;

  [self presentViewController:paymentViewController animated:YES completion:nil];
}

#pragma mark - Proof of payment validation

- (void)sendCompletedPaymentToServer:(PayPalPayment *)completedPayment {
  // TODO: Send completedPayment.confirmation to server
  NSLog(@"Here is your proof of payment:\n\n%@\n\nSend this to your server for confirmation and fulfillment.", completedPayment.confirmation);
}

#pragma mark - PayPalPaymentDelegate methods

- (void)payPalPaymentDidComplete:(PayPalPayment *)completedPayment {
  NSLog(@"PayPal Payment Success!");
  self.completedPayment = completedPayment;
  self.successView.hidden = NO;
  
  [self sendCompletedPaymentToServer:completedPayment]; // Payment was processed successfully; send to server for verification and fulfillment
  [self dismissViewControllerAnimated:YES completion:nil];
}

- (void)payPalPaymentDidCancel {
  NSLog(@"PayPal Payment Canceled");
  self.completedPayment = nil;
  self.successView.hidden = YES;
  [self dismissViewControllerAnimated:YES completion:nil];
}

#pragma mark - Flipside View Controller

- (void)flipsideViewControllerDidFinish:(ZZFlipsideViewController *)controller {
  if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone) {
    [self dismissViewControllerAnimated:YES completion:nil];
  } else {
    [self.flipsidePopoverController dismissPopoverAnimated:YES];
    self.flipsidePopoverController = nil;
  }
}

- (void)popoverControllerDidDismissPopover:(UIPopoverController *)popoverController {
  self.flipsidePopoverController = nil;
}

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
  if ([[segue identifier] isEqualToString:@"showAlternate"]) {
    [[segue destinationViewController] setDelegate:self];

    if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPad) {
      UIPopoverController *popoverController = [(UIStoryboardPopoverSegue *)segue popoverController];
      self.flipsidePopoverController = popoverController;
      popoverController.delegate = self;
    }
  }
}

- (IBAction)togglePopover:(id)sender {
  if (self.flipsidePopoverController) {
    [self.flipsidePopoverController dismissPopoverAnimated:YES];
    self.flipsidePopoverController = nil;
  } else {
    [self performSegueWithIdentifier:@"showAlternate" sender:sender];
  }
}

@end
