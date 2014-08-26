Single Payment
==============

Receive a one-time payment from your customer, via either PayPal or payment card with [card.io](https://www.card.io/). This can be either (1) an **immediate** payment which your servers should subsequently **verify**, or (2) an **authorization** for a payment which your servers must subsequently **capture**, or (3) a payment for an **order** which your servers must subsequently **authorize** and **capture**.

_If you haven't already, see the [README](../README.md) for an initial overview and instructions for adding the SDK to your project._


Overview
--------

* The PayPal iOS SDK...
    1. Presents UI to gather payment information from the user.
    2. Coordinates payment with PayPal.
    3. Returns the payment response to your app.
* Your code...
    1. Receives payment response from the PayPal iOS SDK.
    2. Sends the payment response to your servers for [verification](https://developer.paypal.com/webapps/developer/docs/integration/mobile/verify-mobile-payment/) or [capture](https://developer.paypal.com/webapps/developer/docs/integration/direct/capture-payment/#capture-the-payment) or [order processing](https://developer.paypal.com/webapps/developer/docs/integration/direct/create-process-order/).
    3. Provides the user their goods or services (usually via your servers).

* Optionally, your app can also instruct the PayPal iOS SDK to ask the user to pick a **Shipping Address**:
    * Your code...
        1. Instructs the PayPal iOS SDK to display an app-provided Shipping Address and/or the Shipping Addresses already associated with the user's PayPal account.
    * The PayPal iOS SDK...
        1. Allows the user to examine and choose from the displayed Shipping Address(es).
        2. Adds the chosen Shipping Address to the payment information sent to PayPal's servers.
    * Your server...
        1. When [verifying](https://developer.paypal.com/webapps/developer/docs/integration/mobile/verify-mobile-payment/), [capturing](https://developer.paypal.com/webapps/developer/docs/integration/direct/capture-payment/#capture-the-payment), or [processing](https://developer.paypal.com/webapps/developer/docs/integration/direct/create-process-order/) the payment, retrieves the Shipping Address from the payment information.


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
        // Or if you wish to have the user choose a Shipping Address from those already
        // associated with the user's PayPal account, then add:
        _payPalConfiguration.payPalShippingAddressOption = PayPalShippingAddressOptionPayPal;
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

5. Create a `PayPalPayment` with an amount, a currency code, short description, and intent (immediate sale vs. authorization/capture vs. order/authorization/capture). If you like, include optional fields such as `invoiceNumber` and `PayPalShippingAddress` (as defined in `PayPalPayment.h`):

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
      // meaning combined Authorization + Capture.
      // To perform Authorization only, and defer Capture to your server,
      // use PayPalPaymentIntentAuthorize.
      // To place an Order, and defer both Authorization and Capture to
      // your server, use PayPalPaymentIntentOrder.
      // (PayPalPaymentIntentOrder is valid only for PayPal payments, not credit card payments.)
      payment.intent = PayPalPaymentIntentSale;

      // If your app collects Shipping Address information from the customer,
      // or already stores that information on your server, you may provide it here.
      payment.shippingAddress = address; // a previously-created PayPalShippingAddress object
      
      // Several other optional fields that you can set here are documented in PayPalPayment.h,
      // including paymentDetails, items, invoiceNumber, custom, softDescriptor, etc.

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

8. Send the payment response to your servers for [verification](https://developer.paypal.com/webapps/developer/docs/integration/mobile/verify-mobile-payment/), [capture](https://developer.paypal.com/webapps/developer/docs/integration/direct/capture-payment/#capture-the-payment), or [processing](https://developer.paypal.com/webapps/developer/docs/integration/direct/create-process-order/), as well as any other processing required for your business, such as fulfillment.

   **Tip:** At this point, for an **immediate** payment, the payment has been completed and the user has been charged. For an **authorization/capture** payment or an **order**, your server still must capture the payment to actually charge the user. **If you can't reach your server, it is important that you save the proof of payment and try again later.**

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

**Avoid fraud!** For an immediate payment, be sure to [verify the proof of payment](https://developer.paypal.com/webapps/developer/docs/integration/mobile/verify-mobile-payment/).

For an authorization/capture payment, your server must [capture the payment](https://developer.paypal.com/webapps/developer/docs/integration/direct/capture-payment/#capture-the-payment).

For an order, your server must [process the order](https://developer.paypal.com/webapps/developer/docs/integration/direct/create-process-order/).
