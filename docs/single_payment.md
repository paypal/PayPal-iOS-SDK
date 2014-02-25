Single Payment
==============

Receive a single, immediate payment from your customer, via either PayPal or payment card with [card.io](https://www.card.io/).

_If you haven't already, see the [README](../README.md) for an initial overview and instructions for adding the SDK to your project._


Overview
--------

* The PayPal iOS SDK...
    1. Presents UI to gather payment information from the user.
    2. Coordinates payment with PayPal.
    3. Returns the payment response to your app.
* Your code...
    1. Receives payment response from the PayPal iOS SDK.
    2. [Sends the payment response to your servers for verification](https://developer.paypal.com/webapps/developer/docs/integration/mobile/verify-mobile-payment/).
    3. Provides the user their goods or services.


Sample Code
-----------

This document shows sample code for using the PayPal iOS SDK's Payment API in your application.

1. Initialize the SDK and provide your Client IDs. A typical place to do this is in your app delegate's `didFinishLaunchingWithOptions:` method.

    ```obj-c
    - (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
    {
      // ...
      [PayPalMobile initializeWithClientIdsForEnvironments:@{PayPalEnvironmentProduction : @"YOUR_CLIENT_ID_FOR_PRODUCTION",
                                                             PayPalEnvironmentSandbox : @"YOUR_CLIENT_ID_FOR_SANDBOX"}];
      // ...
      return YES;
    }
    ```

    *Note: if you have not yet obtained a Client ID for the Live environment, you may omit the `PayPalEnvironmentProduction` entry.*

2. Create a class (such as a subclass of UIViewController) that conforms to PayPalPaymentDelegate.

    ```obj-c
    // SomeViewController.h
    #import "PayPalMobile.h"

    @interface SomeViewController : UIViewController<PayPalPaymentDelegate>
    // ...
    @end
    ```

3.  Create a `PayPalConfiguration` object. This object allows you to configure various aspects of the SDK.

    ```obj-c
    // SomeViewController.m

    @interface SomeViewController ()
    // ...
    @property (nonatomic, strong, readwrite) PayPalConfiguration *payPalConfiguration;
    // ...
    @end

    @implementation SomeViewController

    - (instancetype)initWithCoder:(NSCoder *)aDecoder {
      self = [super initWithCoder:aDecoder];
      if (self) {
        _payPalConfiguration = [[PayPalConfiguration alloc] init];

        // See PayPalConfiguration.h for details and default values.
        // Should you wish to change any of the values, you can do so here.
        // For example, if you wish to accept PayPal but not payment card payments, then add:
        _payPalConfiguration.acceptCreditCards = NO;
      }
      return self;
    }

    ```

4. Establish environment, and preconnect to PayPal's servers.

   We recommend doing this when you first display the view controller from which your users *might* initiate payment.
   (Do not preconnect significantly earlier than that, as the connection has a limited lifetime.)

    ```obj-c
    // SomeViewController.m

    - (void)viewWillAppear:(BOOL)animated {
      [super viewWillAppear:animated];

      // Start out working with the test environment! When you are ready, switch to PayPalEnvironmentProduction.
      [PayPalMobile preconnectWithEnvironment:PayPalEnvironmentNoNetwork];
    }
    ```

5. Create a `PayPalPayment` with an amount, a currency code, short description, and intent (Authorization vs Sale):

    ```obj-c
    // SomeViewController.m

    - (IBAction)pay {

      // Create a PayPalPayment
      PayPalPayment *payment = [[PayPalPayment alloc] init];

      // Amount, currency, and description
      payment.amount = [[NSDecimalNumber alloc] initWithString:@"39.95"];
      payment.currencyCode = @"USD";
      payment.shortDescription = @"Awesome saws";

      // Use the intent property to indicate that this is a "sale" payment,
      // meaning combined Authorization + Capture. To perform Authorization only,
      // and defer Capture to your server, use PayPalPaymentIntentAuthorize.
      payment.intent = PayPalPaymentIntentSale;

      // Check whether payment is processable.
      if (!payment.processable) {
        // If, for example, the amount was negative or the shortDescription was empty, then
        // this payment would not be processable. You would want to handle that here.
      }

      // continued below...
    ```

6. Create and display a `PayPalPaymentViewController` with your `PayPalPayment` and `PayPalConfiguration` objects (see above).

    ```obj-c
      // ...continued from above

      // Create a PayPalPaymentViewController.
      PayPalPaymentViewController *paymentViewController;
      paymentViewController = [[PayPalPaymentViewController alloc] initWithPayment:payment
                                                                     configuration:self.payPalConfiguration
                                                                          delegate:self];

      // Present the PayPalPaymentViewController.
      [self presentViewController:paymentViewController animated:YES completion:nil];
    }
    ```

7. Write delegate methods to receive either the completed payment or a cancellation. These delegate methods
   are responsible for dismissing the modal view controller.

    ```obj-c
    // SomeViewController.m

    #pragma mark - PayPalPaymentDelegate methods

    - (void)payPalPaymentViewController:(PayPalPaymentViewController *)paymentViewController
                     didCompletePayment:(PayPalPayment *)completedPayment {
      // Payment was processed successfully; send to server for verification and fulfillment.
      [self verifyCompletedPayment:completedPayment];

      // Dismiss the PayPalPaymentViewController.
      [self dismissViewControllerAnimated:YES completion:nil];
    }

    - (void)payPalPaymentDidCancel:(PayPalPaymentViewController *)paymentViewController {
      // The payment was canceled; dismiss the PayPalPaymentViewController.
      [self dismissViewControllerAnimated:YES completion:nil];
    }
    ```

8. [Send the payment response to your servers for verification](https://developer.paypal.com/webapps/developer/docs/integration/mobile/verify-mobile-payment/), as well as any other processing required for your business, such as fulfillment.

   **Tip:** At this point, the payment has been completed, and the user has been charged. **If you can't reach your server, it is important that you save the proof of payment and try again later.**

    ```obj-c
    // SomeViewController.m

    - (void)verifyCompletedPayment:(PayPalPayment *)completedPayment {
      // Send the entire confirmation dictionary
      NSData *confirmation = [NSJSONSerialization dataWithJSONObject:completedPayment.confirmation
                                                             options:0
                                                               error:nil];

      // Send confirmation to your server; your server should verify the proof of payment
      // and give the user their goods or services. If the server is not reachable, save
      // the confirmation and try again later.
    }
    ```

Next Steps
----------

**Avoid fraud!** Be sure to [verify the proof of payment](https://developer.paypal.com/webapps/developer/docs/integration/mobile/verify-mobile-payment/).
