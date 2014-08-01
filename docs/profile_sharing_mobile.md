Profile Sharing Mobile Integration
==================================

This section will show how to obtain a user's authorized consent for profile sharing from their PayPal account.

_If you haven't already, see the [README](../README.md) for an initial overview and instructions for adding the SDK to your project._


Overview
--------

You must [obtain customer consent](#obtain-customer-consent) to share information from their PayPal account. How this works:

* On the PayPal Developer site...
    1. Specify the pieces of information that you wish your customers to share with you.
* The PayPal iOS SDK...
    1. Presents UI for your user to authenticate using their PayPal account.
    2. Asks your user to consent to [OAuth access token scope](http://tools.ietf.org/html/rfc6749#page-23) to use PayPal for profile sharing.
    3. Returns an OAuth2 authorization code to your app.
* Your app...
    1. Receives an OAuth2 authorization code from the SDK.
    2. Sends the authorization code to your server, which then [exchanges the code for OAuth2 access and refresh tokens](profile_sharing_server.md#obtain-oauth2-tokens).
    3. Your server then [uses its OAuth2 tokens to request the relevant customer information from PayPal](profile_sharing_server.md).

**Notes:**  
1. See `PayPalOAuthScopes.h` for a complete list of scope-values available to the PayPal iOS SDK.  
2. See [Log In with PayPal user attributes](https://developer.paypal.com/docs/integration/direct/identity/attributes/) for a complete list of available scope attributes.


Specify Desired Information for Sharing
---------------------------------------

1. Log in to the [PayPal Developer site](https://developer.paypal.com).
2. Select [your app](https://developer.paypal.com/webapps/developer/applications/myapps).
3. Under `APP CAPABILITIES` select `Log In with PayPal`, and click `Advanced options`.
4. Under `Information requested from customers` select the items ("scope attributes") you wish to have shared.
5. If you provide Privacy Policy and User Agreement URLs under `Links shown on customer consent page`, these will override the corresponding  URLs that you provide below in the [`PayPalConfiguration` object](#obtain-customer-consent).


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

2. Create a class (such as a subclass of UIViewController) that conforms to `PayPalProfileSharingDelegate`.

    ```obj-c
    // SomeViewController.h
    #import "PayPalMobile.h"

    @interface SomeViewController : UIViewController<PayPalProfileSharingDelegate>
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

5. Create and display a `PayPalProfileSharingViewController` with your `PayPalConfiguration` object and with the appropriate scope-values.

    ```obj-c
    // SomeViewController.m

    - (IBAction)obtainConsent {

      // Choose whichever scope-values apply in your case. See `PayPalOAuthScopes.h` for a complete list of available scope-values.
      NSSet *scopeValues = [NSSet setWithArray:@[kPayPalOAuth2ScopeOpenId, kPayPalOAuth2ScopeEmail, kPayPalOAuth2ScopeAddress, kPayPalOAuth2ScopePhone]];

      PayPalProfileSharingViewController *psViewController;
      psViewController = [[PayPalProfileSharingViewController alloc] initWithScopeValues:scopeValues
                                                                           configuration:self.payPalConfiguration
                                                                                delegate:self];

      // Present the PayPalProfileSharingViewController
      [self presentViewController:psViewController animated:YES completion:nil];
    }
    ```

6. Implement the `PayPalProfileSharingDelegate` delegate methods to receive either the authorization response on success,
   or notification that the user cancelled. Your implementation is responsible for dismissing the modal view controller.

    ```obj-c
    // SomeViewController.m

    #pragma mark - PayPalProfileSharingDelegate methods

    - (void)userDidCancelPayPalProfileSharingViewController:(PayPalProfileSharingViewController *)profileSharingViewController {
      // User cancelled login. Dismiss the PayPalProfileSharingViewController, breathe deeply.
      [self dismissViewControllerAnimated:YES completion:nil];
    }

    - (void)payPalProfileSharingViewController:(PayPalProfileSharingViewController *)profileSharingViewController
                 userDidLogInWithAuthorization:(NSDictionary *)profileSharingAuthorization {
      // The user has successfully logged into PayPal, and has consented to profile sharing.

      // Your code must now send the authorization response to your server.
      [self sendAuthorizationToServer:profileSharingAuthorization];

      // Be sure to dismiss the PayPalProfileSharingViewController.
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
      // Your server must then use these tokens to retrieve customer information from PayPal.
    }
    ```




Next Steps
----------

Read [Profile Sharing Server-Side Integration](profile_sharing_server.md) to exchange the authorization code for OAuth2 tokens and retrieve the customer information from PayPal.
