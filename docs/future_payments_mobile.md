Future Payments Mobile Integration
==================================

This section will show how to obtain a user's authorized consent for future payments using their PayPal account.

_If you haven't already, see the [README](../README.md) for an initial overview and instructions for adding the SDK to your project._


Overview
--------

Your app will use the mobile SDK to obtain two pieces of information at different times.

First, you must [obtain customer consent](#obtain-customer-consent) to take payments from their PayPal account. How this works:

* The PayPal iOS SDK...
    1. Presents UI for your user to authenticate using their PayPal account.
    2. Asks your user to consent to [OAuth access token scope](http://tools.ietf.org/html/rfc6749#page-23) to use PayPal for future payments.
    3. Returns an OAuth2 authorization code to your app.
* Your app...
    1. Receives an OAuth2 authorization code from the SDK.
    2. Sends the authorization code to your server, which then [exchanges the code for OAuth2 access and refresh tokens](future_payments_server.md#obtain-oauth2-tokens).

Later, when initiating a pre-consented payment, you must [obtain a Client Metadata ID](#obtain-a-client-metadata-id). How this works:

* The PayPal iOS SDK...
    * Provides a Client Metadata ID.
* Your app...
    * Sends the Client Metadata ID and transaction information to your server.
    * Your server then [uses its OAuth2 tokens, Client Metadata ID, and transaction info to create a payment](future_payments_server.md).


Obtain Customer Consent
-----------------------

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

2. Create a class (such as a subclass of UIViewController) that conforms to `PayPalFuturePaymentDelegate`.

    ```obj-c
    // SomeViewController.h
    #import "PayPalMobile.h"

    @interface SomeViewController : UIViewController<PayPalFuturePaymentDelegate>
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

        // Minimally, you will need to set three merchant information properties.
        // These should be the same values that you provided to PayPal when you registered your app.
        _payPalConfiguration.merchantName = @"Ultramagnetic Omega Supreme";
        _payPalConfiguration.merchantPrivacyPolicyURL = [NSURL URLWithString:@"https://www.omega.supreme.example/privacy"];
        _payPalConfiguration.merchantUserAgreementURL = [NSURL URLWithString:@"https://www.omega.supreme.example/user_agreement"];

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

      // Start out working with the mock environment. When you are ready, switch to PayPalEnvironmentProduction.
      [PayPalMobile preconnectWithEnvironment:PayPalEnvironmentNoNetwork];
    }
    ```

5. Create and display a `PayPalFuturePaymentViewController` with configuration.

    ```obj-c
    // SomeViewController.m

    - (IBAction)obtainConsent {

      PayPalFuturePaymentViewController *fpViewController;
      fpViewController = [[PayPalFuturePaymentViewController alloc] initWithConfiguration:self.payPalConfiguration
                                                                                 delegate:self];

      // Present the PayPalFuturePaymentViewController
      [self presentViewController:fpViewController animated:YES completion:nil];
    }
    ```

6. Implement the `PayPalFuturePaymentDelegate` delegate methods to receive either the authorization response on success,
   or notification that the user cancelled. Your implementation is responsible for dismissing the modal view controller.

    ```obj-c
    // SomeViewController.m

    #pragma mark - PayPalFuturePaymentDelegate methods

    - (void)payPalFuturePaymentDidCancel:(PayPalFuturePaymentViewController *)futurePaymentViewController {
      // User cancelled login. Dismiss the PayPalLoginViewController, breathe deeply.
      [self dismissViewControllerAnimated:YES completion:nil];
    }

    - (void)payPalFuturePaymentViewController:(PayPalFuturePaymentViewController *)futurePaymentViewController
                    didAuthorizeFuturePayment:(NSDictionary *)futurePaymentAuthorization {
      // The user has successfully logged into PayPal, and has consented to future payments.

      // Your code must now send the authorization response to your server.
      [self sendAuthorizationToServer:futurePaymentAuthorization];

      // Be sure to dismiss the PayPalLoginViewController.
      [self dismissViewControllerAnimated:YES completion:nil];
    }
    ```

7. Send the authorization response to your server in order to complete the process.

    ```obj-c
    // SomeViewController.m

    - (void)sendAuthorizationToServer:(NSDictionary *)authorization {
      // Send the entire authorization reponse
      NSData *consentJSONData = [NSJSONSerialization dataWithJSONObject:authorization
                                                                options:0
                                                                  error:nil];

      // (Your network code here!)
      //
      // Send the authorization response to your server, where it can exchange the authorization code
      // for OAuth access and refresh tokens.
      //
      // Your server must then store these tokens, so that your server code can execute payments
      // for this user in the future.
    }
    ```


Obtain a Client Metadata ID
---------------------------

When initiating a pre-consented payment (a "future payment") from a mobile device, your mobile application must obtain a Client Metadata ID from the Mobile SDK to pass to your server. Your server must include this Client Metadata ID in its payment request sent to PayPal.

PayPal uses this Client Metadata ID to verify that the payment is originating from a valid, user-consented device+application. This helps reduce fraud and decrease declines. **PayPal does not provide any loss protection for transactions that do not correctly supply a Client Metadata ID.**

**Do not cache or store this value.**

Example:

```obj-c
- (IBAction)paymentButtonTapped:(UIButton *)sender {

    // Display activity indicator...

    NSString *metadataID = [PayPalMobile clientMetadataID];

    // Send metadataID and transaction details to your server for processing with PayPal...
}
```

When your server makes its payment request to PayPal, it must include a `PayPal-Client-Metadata-Id` HTTP header with this Client Metadata ID value obtained from the SDK.




Next Steps
----------

Read [Future Payments Server-Side Integration](future_payments_server.md) to exchange the authorization code for OAuth2 tokens and create payments with an access token and a Client Metadata ID.
