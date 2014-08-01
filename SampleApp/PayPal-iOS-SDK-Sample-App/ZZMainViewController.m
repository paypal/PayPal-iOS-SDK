//
//  ZZMainViewController.m
//  PayPal-iOS-SDK-Sample-App
//
//  Copyright (c) 2014, PayPal
//  All rights reserved.
//

#import "ZZMainViewController.h"
#import <QuartzCore/QuartzCore.h>

// Set the environment:
// - For live charges, use PayPalEnvironmentProduction (default).
// - To use the PayPal sandbox, use PayPalEnvironmentSandbox.
// - For testing, use PayPalEnvironmentNoNetwork.
#define kPayPalEnvironment PayPalEnvironmentNoNetwork

@interface ZZMainViewController ()

@property(nonatomic, strong, readwrite) IBOutlet UIButton *payNowButton;
@property(nonatomic, strong, readwrite) IBOutlet UIButton *payFutureButton;
@property(nonatomic, strong, readwrite) IBOutlet UIView *successView;

@property(nonatomic, strong, readwrite) PayPalConfiguration *payPalConfig;

@end

@implementation ZZMainViewController

- (void)viewDidLoad {
  [super viewDidLoad];
  self.title = @"PayPal SDK Demo";

  // Set up payPalConfig
  _payPalConfig = [[PayPalConfiguration alloc] init];
  _payPalConfig.acceptCreditCards = YES;
  _payPalConfig.languageOrLocale = @"en";
  _payPalConfig.merchantName = @"Awesome Shirts, Inc.";
  _payPalConfig.merchantPrivacyPolicyURL = [NSURL URLWithString:@"https://www.paypal.com/webapps/mpp/ua/privacy-full"];
  _payPalConfig.merchantUserAgreementURL = [NSURL URLWithString:@"https://www.paypal.com/webapps/mpp/ua/useragreement-full"];
  
  // Setting the languageOrLocale property is optional.
  //
  // If you do not set languageOrLocale, then the PayPalPaymentViewController will present
  // its user interface according to the device's current language setting.
  //
  // Setting languageOrLocale to a particular language (e.g., @"es" for Spanish) or
  // locale (e.g., @"es_MX" for Mexican Spanish) forces the PayPalPaymentViewController
  // to use that language/locale.
  //
  // For full details, including a list of available languages and locales, see PayPalPaymentViewController.h.
  
  _payPalConfig.languageOrLocale = [NSLocale preferredLanguages][0];
  
  
  // Setting the payPalShippingAddressOption property is optional.
  //
  // See PayPalConfiguration.h for details.
  
  _payPalConfig.payPalShippingAddressOption = PayPalShippingAddressOptionPayPal;

  // Do any additional setup after loading the view, typically from a nib.

  self.successView.hidden = YES;
  
  // use default environment, should be Production in real life
  self.environment = kPayPalEnvironment;

  NSLog(@"PayPal iOS SDK version: %@", [PayPalMobile libraryVersion]);

}

- (void)viewWillAppear:(BOOL)animated {
  [super viewWillAppear:YES];

  // Preconnect to PayPal early
  [PayPalMobile preconnectWithEnvironment:self.environment];
}

#pragma mark - Receive Single Payment

- (IBAction)pay {
  // Remove our last completed payment, just for demo purposes.
  self.resultText = nil;
  
  // Note: For purposes of illustration, this example shows a payment that includes
  //       both payment details (subtotal, shipping, tax) and multiple items.
  //       You would only specify these if appropriate to your situation.
  //       Otherwise, you can leave payment.items and/or payment.paymentDetails nil,
  //       and simply set payment.amount to your total charge.
  
  // Optional: include multiple items
  PayPalItem *item1 = [PayPalItem itemWithName:@"Old jeans with holes"
                                  withQuantity:2
                                     withPrice:[NSDecimalNumber decimalNumberWithString:@"84.99"]
                                  withCurrency:@"USD"
                                       withSku:@"Hip-00037"];
  PayPalItem *item2 = [PayPalItem itemWithName:@"Free rainbow patch"
                                  withQuantity:1
                                     withPrice:[NSDecimalNumber decimalNumberWithString:@"0.00"]
                                  withCurrency:@"USD"
                                       withSku:@"Hip-00066"];
  PayPalItem *item3 = [PayPalItem itemWithName:@"Long-sleeve plaid shirt (mustache not included)"
                                  withQuantity:1
                                     withPrice:[NSDecimalNumber decimalNumberWithString:@"37.99"]
                                  withCurrency:@"USD"
                                       withSku:@"Hip-00291"];
  NSArray *items = @[item1, item2, item3];
  NSDecimalNumber *subtotal = [PayPalItem totalPriceForItems:items];
  
  // Optional: include payment details
  NSDecimalNumber *shipping = [[NSDecimalNumber alloc] initWithString:@"5.99"];
  NSDecimalNumber *tax = [[NSDecimalNumber alloc] initWithString:@"2.50"];
  PayPalPaymentDetails *paymentDetails = [PayPalPaymentDetails paymentDetailsWithSubtotal:subtotal
                                                                             withShipping:shipping
                                                                                  withTax:tax];

  NSDecimalNumber *total = [[subtotal decimalNumberByAdding:shipping] decimalNumberByAdding:tax];
  
  PayPalPayment *payment = [[PayPalPayment alloc] init];
  payment.amount = total;
  payment.currencyCode = @"USD";
  payment.shortDescription = @"Hipster clothing";
  payment.items = items;  // if not including multiple items, then leave payment.items as nil
  payment.paymentDetails = paymentDetails; // if not including payment details, then leave payment.paymentDetails as nil

  if (!payment.processable) {
    // This particular payment will always be processable. If, for
    // example, the amount was negative or the shortDescription was
    // empty, this payment wouldn't be processable, and you'd want
    // to handle that here.
  }

  // Update payPalConfig re accepting credit cards.
  self.payPalConfig.acceptCreditCards = self.acceptCreditCards;
  
  PayPalPaymentViewController *paymentViewController = [[PayPalPaymentViewController alloc] initWithPayment:payment
                                                                                              configuration:self.payPalConfig
                                                                                                   delegate:self];
  [self presentViewController:paymentViewController animated:YES completion:nil];
}

#pragma mark PayPalPaymentDelegate methods

- (void)payPalPaymentViewController:(PayPalPaymentViewController *)paymentViewController didCompletePayment:(PayPalPayment *)completedPayment {
  NSLog(@"PayPal Payment Success!");
  self.resultText = [completedPayment description];
  [self showSuccess];

  [self sendCompletedPaymentToServer:completedPayment]; // Payment was processed successfully; send to server for verification and fulfillment
  [self dismissViewControllerAnimated:YES completion:nil];
}

- (void)payPalPaymentDidCancel:(PayPalPaymentViewController *)paymentViewController {
  NSLog(@"PayPal Payment Canceled");
  self.resultText = nil;
  self.successView.hidden = YES;
  [self dismissViewControllerAnimated:YES completion:nil];
}

#pragma mark Proof of payment validation

- (void)sendCompletedPaymentToServer:(PayPalPayment *)completedPayment {
  // TODO: Send completedPayment.confirmation to server
  NSLog(@"Here is your proof of payment:\n\n%@\n\nSend this to your server for confirmation and fulfillment.", completedPayment.confirmation);
}


#pragma mark - Authorize Future Payments

- (IBAction)getUserAuthorizationForFuturePayments:(id)sender {
  
  PayPalFuturePaymentViewController *futurePaymentViewController = [[PayPalFuturePaymentViewController alloc] initWithConfiguration:self.payPalConfig delegate:self];
  [self presentViewController:futurePaymentViewController animated:YES completion:nil];
}


#pragma mark PayPalFuturePaymentDelegate methods

- (void)payPalFuturePaymentViewController:(PayPalFuturePaymentViewController *)futurePaymentViewController
                didAuthorizeFuturePayment:(NSDictionary *)futurePaymentAuthorization {
  NSLog(@"PayPal Future Payment Authorization Success!");
  self.resultText = [futurePaymentAuthorization description];
  [self showSuccess];

  [self sendFuturePaymentAuthorizationToServer:futurePaymentAuthorization];
  [self dismissViewControllerAnimated:YES completion:nil];
}

- (void)payPalFuturePaymentDidCancel:(PayPalFuturePaymentViewController *)futurePaymentViewController {
  NSLog(@"PayPal Future Payment Authorization Canceled");
  self.successView.hidden = YES;
  [self dismissViewControllerAnimated:YES completion:nil];
}

- (void)sendFuturePaymentAuthorizationToServer:(NSDictionary *)authorization {
  // TODO: Send authorization to server
  NSLog(@"Here is your authorization:\n\n%@\n\nSend this to your server to complete future payment setup.", authorization);
}


#pragma mark - Authorize Profile Sharing

- (IBAction)getUserAuthorizationForProfileSharing:(id)sender {
  
  NSSet *scopeValues = [NSSet setWithArray:@[kPayPalOAuth2ScopeOpenId, kPayPalOAuth2ScopeEmail, kPayPalOAuth2ScopeAddress, kPayPalOAuth2ScopePhone]];
  
  PayPalProfileSharingViewController *profileSharingPaymentViewController = [[PayPalProfileSharingViewController alloc] initWithScopeValues:scopeValues configuration:self.payPalConfig delegate:self];
  [self presentViewController:profileSharingPaymentViewController animated:YES completion:nil];
}


#pragma mark PayPalProfileSharingDelegate methods

- (void)payPalProfileSharingViewController:(PayPalProfileSharingViewController *)profileSharingViewController
             userDidLogInWithAuthorization:(NSDictionary *)profileSharingAuthorization {
  NSLog(@"PayPal Profile Sharing Authorization Success!");
  self.resultText = [profileSharingAuthorization description];
  [self showSuccess];
  
  [self sendProfileSharingAuthorizationToServer:profileSharingAuthorization];
  [self dismissViewControllerAnimated:YES completion:nil];
}

- (void)userDidCancelPayPalProfileSharingViewController:(PayPalProfileSharingViewController *)profileSharingViewController {
  NSLog(@"PayPal Profile Sharing Authorization Canceled");
  self.successView.hidden = YES;
  [self dismissViewControllerAnimated:YES completion:nil];
}

- (void)sendProfileSharingAuthorizationToServer:(NSDictionary *)authorization {
  // TODO: Send authorization to server
  NSLog(@"Here is your authorization:\n\n%@\n\nSend this to your server to complete profile sharing setup.", authorization);
}


#pragma mark - Helpers

- (void)showSuccess {
  self.successView.hidden = NO;
  self.successView.alpha = 1.0f;
  [UIView beginAnimations:nil context:NULL];
  [UIView setAnimationDuration:0.5];
  [UIView setAnimationDelay:2.0];
  self.successView.alpha = 0.0f;
  [UIView commitAnimations];
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
  if ([[segue identifier] isEqualToString:@"pushSettings"]) {
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
