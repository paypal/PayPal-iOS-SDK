PayPal iOS SDK release notes
============================

2.12.7
------
* Update networking code for iOS 7 to fix potential ClassicURLConnection crash

2.12.6
------
* When the wrong PayPal login credentials are used, use shake animation
  and allow user to login again without having to click on Pay button again

2.12.5
------
* In CocoaPods, add subspecs to allow PayPal SDK to be used without card.io. By
  default, all libraries are included. If you do not want to use card.io, use
  the `Core` subspec like `PayPal-iOS-SDK/Core` in your Podfile. See the
  SampleApp without card.io to see how you can setup your application without
  credit card scanning. See [issue #358](https://github.com/paypal/PayPal-iOS-SDK/issues/358).
* Update to use NSURLSession whenever possible. Falls back to NSURLConnection for iOS 6.

2.12.4
------
* Split card.io libraries into 3 different libraries to help reduce the size of the individual files. [issue #339](https://github.com/paypal/PayPal-iOS-SDK/issues/339)
  You should link to `libPayPalMobile.a`, `libCardIO.a`, `libopencv_core.a', and `libopencv_imgproc.a` for your app now. See notes in 2.12.2 for more information.
* Fix issue with iOS 6 API availability with call to initWithBase64EncodedData:options: [issue #351](https://github.com/paypal/PayPal-iOS-SDK/issues/351)
* Switch to use vendored_libraries in `PayPal-iOS-SDK.podspec` to help some CocoaPod issues

2.12.3
------
* Fixed long string description text wrapping for iOS9
* Updated Swift Sample App to Swift 2.0 syntax
* Fixed cocoapods spec file to unzip libCardIO.a.zip when installing for dependency resolution
* Parsing enhancement for empty response received from server APIs

2.12.2
------
* There are now 2 static libraries which you should link to your application:
  - `libPayPalMobile.a` in the `PayPalMobile` directory
  - `libCardIO.a` in the `CardIO` directory. Please unzip the `libCardIO.a.zip` archive.
* `libPayPalMobile.a` is a required static library which has all the previous PayPal functionality using the REST APIs
* `libCardIO.a` is an optional static library. If you want to allow payments by scanning credit cards, you must also
  link this library. It is currently zipped in a `libCardIO.a.zip` file. Use a standard unzip tool to unzip
  `CardIO\libCardIO.a.zip` to get `libCardIO.a`.
* If you want the same functionality which you had before in version 2.12.1, link to both `libPayPalMobile.a` and
  `libCardIO.a`. You must use the `libPayPalMobile.a` and `libCardIO.a` from the same SDK.
* Bitcode enabled the libraries. While the static libraries are larger, the application delivered to users should not
  be significantly different in size compared to before. [Issue #284](https://github.com/paypal/PayPal-iOS-SDK/issues/284)
* You must use Xcode 7.0 with these versions of the SDK. For Xcode 6.4, you can use the 2.11.x libraries but it is
  advised that you upgrade as soon as possible. Xcode 6.4 is not compatible with Bitcode enabled libraries.

2.12.1
------
* Fix [issue #320](https://github.com/paypal/PayPal-iOS-SDK/issues/320) with 2-Factor Authentication flow
* Add README note for URL schemes which the SDK queries

2.12.0
------
* Built using Xcode 7.0
* Use `SFSafariViewController` via `SafariServices.framework` (if linked) for opening web pages
* Fixed behavior where `retrieve_shipping_address ` is disabled and no `shipping address` is provided from the app,
  so it will not default to the PayPal account `shipping address`.

2.11.5
------
* Note this is the same code as 2.12.1 except built with Xcode 6.4 [issue #327](https://github.com/paypal/PayPal-iOS-SDK/issues/327)
* Fix [issue #320](https://github.com/paypal/PayPal-iOS-SDK/issues/320) with 2-Factor Authentication flow
* Add README note for URL schemes which the SDK queries
* Use `SFSafariViewController` via `SafariServices.framework` (if linked) for opening web pages
* Fixed behavior where `retrieve_shipping_address ` is disabled and no `shipping address` is provided from the app,
  so it will not default to the PayPal account `shipping address`.

2.11.4
------
* Fix [issue #290](https://github.com/paypal/PayPal-iOS-SDK/issues/290).

2.11.3
------
* Fix [issue #306 - App in a bad state if interrupted during transition](https://github.com/paypal/PayPal-iOS-SDK/issues/306).

2.11.2
------
* Update card.io to 5.1.0.

2.11.1
------
* Update card.io library to 5.0.6.
* Minor bug fixes.

2.11.0
------
* NOTE: version 2.11.0 requires Xcode 6.3+. If you are still using Xcode 6.2, please use version 2.10.2 of this SDK.
* Update card.io library to 5.0.5. (fixes https://github.com/paypal/PayPal-iOS-SDK/issues/278)
* Minor bug fixes.

2.10.2
------
* Fix an iPad display issue. (see https://github.com/paypal/PayPal-iOS-SDK/issues/272)
* Improve mSDK screen blurring when app is backgrounded.
  NOTE: this change requires that you add one more framework to your project:
        `Accelerate.framework`
* Minor bug fixes.

2.10.1
------
* Improve the country code displayed for Phone/PIN login.
* Build the SDK using Xcode 6.2 rather than 6.3, to provide better compatibility
  for those developers still using Xcode 6.2.

2.10.0
------
* Improve currency display for some currencies/locales.
  (see http://stackoverflow.com/questions/29409480/paypal-for-ios-in-arabic-language-giving-issue)
* Update card.io library to 5.0.4.
* Minor bug fixes.

2.9.0
-----
* Add `alwaysDisplayCurrencyCodes` property to `PayPalConfiguration`.
* Update card.io library to 5.0.3.

2.8.5
-----
* Change "Send Payment" button to "Pay". (see https://github.com/paypal/PayPal-iOS-SDK/issues/174)
* Minor bug fixes.
* Update card.io library to 5.0.2.

2.8.4
-----
* Add button allowing user to create a new PayPal account.
* Minor bug fixes.
* Update card.io library to 5.0.1.

2.8.3
-----
* Fix an iOS 6 crash.
* Improve UI for country codes during Phone/PIN login.
* Display currency codes for mixed-currency situations;
  e.g., when choosing between balances in different currencies.
* Minor bug fixes.
* Update card.io library to 5.0.0.

2.8.2
-----
* Improve handling of some server errors.
* Minor bug fixes.
* Update card.io library to 4.0.2.

2.8.1
-----
* Prevent a rare crash. (https://github.com/paypal/PayPal-iOS-SDK/issues/220)
* Update card.io library to 4.0.1.

2.8.0
-----
* Update `Paypal-Application-Correlation-Id` header to `PayPal-Client-Metadata-Id`,
  and deprecate `[PayPalMobile applicationCorrelationIDForEnvironment:]` in favor
  of `[PayPalMobile clientMetadataID]`.
* Update card.io library to 4.0.0.

2.7.2
-----
* Minor bug fixes.

2.7.1
-----
* Prevent two MORE potential "duplicate symbol" problems for apps that are also integrating with 1Password. Grr.

2.7.0
-----
* Add support for Russian Rubles. [issue #223](https://github.com/paypal/PayPal-iOS-SDK/issues/223)
* Include public header files for [card.io](https://www.card.io), which is contained inside the PayPal iOS SDK,
  for apps that use card.io directly. [issue #219](https://github.com/paypal/PayPal-iOS-SDK/issues/219)
* Minor improvements to Sample App.
* Prevent a few more potential "duplicate symbol" problems for apps that are also integrating with 1Password.
* Update card.io library to 3.10.1.

2.6.1
-----
* Minor improvements relevant only to select partners.

2.6.0
-----
* Allow login by users who have enabled two-factor authentication on their PayPal accounts.
* Streamline Profile Sharing for cases where a user has previously authorized sharing.
* Prevent a "duplicate symbol" problem for apps that are also integrating with 1Password.
* Minor bug fixes.

2.5.0
-----
* NOTE: version 2.5.0 requires Xcode 6 and iOS 8 SDK. If you are still using Xcode 5, please use version 2.3.2 of this SDK.
* Add `presentingInPopover` property to `PayPalConfiguration`.
* Add a new optional delegate method for each of our view controllers, to allow your app
  to talk with your server while our activity indicator is still visible.
* Fix an iOS 6 crash. [issue #202](https://github.com/paypal/PayPal-iOS-SDK/issues/202)
* Update card.io library to 3.10.0.

2.4.2
-----
* NOTE: version 2.4.2 requires Xcode 6 and iOS 8 SDK. If you are still using Xcode 5, please use version 2.3.2 of this SDK.
* Correct the number of expected decimal places for the HUF and TWD currencies. [issue #191](https://github.com/paypal/PayPal-iOS-SDK/issues/191)
* Update card.io library to 3.9.0.

2.4.1
-----
* NOTE: version 2.4.1 requires Xcode 6 and iOS 8 SDK. If you are still using Xcode 5, please use version 2.3.2 of this SDK.
* Fix a bug where user selection of Shipping Address (for a Single Payment) sometimes did not work correctly.

2.4.0
-----
* NOTE 1: version 2.4.0 requires Xcode 6 and iOS 8 SDK. If you are still using Xcode 5, please use version 2.3.2 of this SDK.
* NOTE 2: in the `Link Binary With Libraries` section of your project target's Build Phases, add:
    * `AudioToolbox.framework`
    * `CoreMedia.framework`
    * `MobileCoreServices.framework`
* Official release for iOS 8, built with Xcode 6 GM seed.
* Support use of the iOS 8 1Password Extension for PayPal logins.
* Update Sample App icons.
* Update card.io library to 3.8.6.
* Improve mSDK behavior when presented within a popover.
* Update some localized strings.
* Minor bug fixes.

2.3.2
-----
* Fix version reporting

2.3.1
-----
* Update some localized strings.

2.3.0
-----
* Add support for `order` payments.
* Add `invoiceNumber`, `custom`, and `softDescriptor` as optional properties on `PayPalPayment`.
* For Single Payments, an individual `PayPalItem` `price` may now be negative (for coupons, etc). [issue #162](https://github.com/paypal/PayPal-iOS-SDK/issues/162)

2.2.1
-----
* Minor bug fixes.

2.2.0
-----
* Add Profile Sharing feature. (See `PayPalProfileSharingViewController.h`.)
* For PayPal single payments, allow user to choose from app-provided and/or PayPal-account-provided Shipping Address.
* For PayPal single payments, user will now automatically be offered a choice of funding options (when appropriate).
* Fix a bug in the `clearAllUserData` method of `PayPalMobile`. (Credit card not being cleared while app remains in memory.)

2.1.6
-----
* Minor bug fixes.

2.1.5
-----
* Fix a very rare problem involving the app caches directory getting cleared. [issue #163](https://github.com/paypal/PayPal-iOS-SDK/issues/163)

2.1.4
-----
* Fix some localization issues. [issue #164](https://github.com/paypal/PayPal-iOS-SDK/issues/164)
* Update card.io library to 3.8.2.

2.1.3
-----
* Fix a crash involving PayPal accounts that have enabled two-factor authentication.
* Fix an iOS 8 crash.
* Make the HK dialect of Traditional Chinese the default dialect. (zh-Hant_TW remains available as well.)
* Update card.io library to 3.8.0.

2.1.2
-----
* Fix a bug involving the format of `PayPalItem` `price` in server requests.
  (The bug affected items whose fractional price was a multiple of 0.10)

2.1.1
-----
* Add translations for Thai.
* Update card.io library to 3.7.1.
* Allow hyphens or underscores within the optional Build Notation code ("BN code").

2.1.0
-----
* Add support for Multiple Items per payment.
* Add `clearAllUserData` method to `PayPalMobile`.
* If app provides `defaultUserEmail`, `defaultUserPhoneCountryCode`, and/or `defaultUserPhoneNumber`,
  these will now be overridden by the user's most recent PayPal login.
* Update PayPal logo.
* Update card.io library to 3.7.0.

2.0.5
-----
* Update a Spanish translation. [issue #139](https://github.com/paypal/PayPal-iOS-SDK/issues/139)

2.0.4
-----
* Eliminate remaining references to IDFA (Advertising ID) to prevent potential issues for app approvals.

2.0.3
-----
* For payments using `PayPalPaymentIntentAuthorize`, the `PayPalPayment.confirmation` now includes `authorization_id`.
  [issue #125](https://github.com/paypal/PayPal-iOS-SDK/issues/125#issuecomment-39613855)
* Correct the handling of currencies HKD, HUF, and TWD.
  [issue #130](https://github.com/paypal/PayPal-iOS-SDK/issues/130)

2.0.2
-----
* Add full support for device rotation.
* Add translations for Arabic and Malay.
* Update card.io library to 3.6.4.

2.0.1
-----
* A few very minor fixes:
  * Correct the version string returned by the SDK.
  * Tidy up some messages in the console log.
  * Improve handling of iOS 7 dialects such as "British English."

2.0.0
-----

* Users can now authorize future payments with PayPal.
* Reorganize API, for both single-payment and future-payment use cases:
    * Use the `PayPalMobile` class to initialize the SDK with Client IDs for Production and Sandbox.
    * Use a `PayPalConfiguration` object for common configuration across both single-payment and future-payment use cases.
    * Allow an optional `PayPalPaymentDetails` object to specify Shipping, Subtotal, and Tax amounts.
    * Allow an optional Build Notation code ("BN code") for each payment.
    * Add a `PayPalPaymentViewController` parameter to the methods of `PayPalPaymentDelegate`.
    * Introduce `PayPalFuturePaymentViewController` and associated `PayPalFuturePaymentDelegate`.
* Single payments now include an `intent` property, to distinguish between:
    1. immediate payment processing
    2. payment authorization only, with subsequent capture by your server.
* For API errors, the console log will now provide additional information, including a PayPal Debug-ID for MTS investigations.
* The SDK now uses PayPal's new REST APIs exclusively.

1.4.6
-----
* Update card.io library to 3.4.4,
  fixes [Issue #83: Crashes when card.io doesn't detect number](https://github.com/paypal/PayPal-iOS-SDK/issues/83)

1.4.5
-----
* Fix a bug that caused "duplicated symbol" errors when an app used the -all_load linker flag.

1.4.4
-----
* Fix a bug that arose when `hideCreditCardButton` is set.
* Update card.io library to 3.4.3

1.4.3
-----
* Improve the handling of right-to-left languages (e.g., Hebrew).
* Update card.io library to 3.4.2
* Fix [issue #66](https://github.com/paypal/PayPal-iOS-SDK/issues/66)

1.4.2
-----
* Fix [issue #60](https://github.com/paypal/PayPal-iOS-SDK/issues/60)

1.4.1
-----
* Fix a problem with the 64-bit library

1.4.0
-----
* Make 64-bit compatible
* Require iOS 6.0 or greater
* Update card.io library to 3.4.0

1.3.6
-----
* Fix a problem with repeat purchase resulting in 52003 error on Sandbox

1.3.5
-----

* Fix a problem with accepting American Express cards.

1.3.4
-----

* Fix [Issue #46: Back text overlaps Cancel text](https://github.com/paypal/PayPal-iOS-SDK/issues/46).

1.3.3
-----

* Official release for iOS 7, built with Xcode 5 GM seed.
* Modify header comments to support Xcode 5 Quick Help.
* Update card.io library to 3.2.4.

1.3.2-beta
----------

* Better reposting of 1.3.1-beta

1.3.1-beta
----------

* Fix [issue #39](https://github.com/paypal/PayPal-iOS-SDK/issues/39).

1.3.0-beta
----------

* Add support for iOS 7.
    * Library built with Xcode 5 DP 5.
    * The SDK is still compatible with iOS 5 and 6, as well as Xcode 4.
* Update UI across all supported versions of iOS.
    * The confirm view has been merged into the payment selection view.

1.2.0
-----

* Support all currencies that are currently accepted by the REST APIs. See [README](README.md) for details.
* Eliminate the final "Complete" screen.
   * Your app's payPalPaymentDidComplete: method will now be called as soon as the payment has completed.
   * The PayPalTransactionDidSucceedNotification is no longer necessary and has been deprecated.
* Additional localization: ko (Korean)
* Update card.io library to 3.2.3

1.1.1
-----

* Additional localizations: es_MX, pt_BR
* Update card.io library to 3.2.1
* Fix small bugs.

1.1.0
-----

* Update the required C++ Standard Library from `libstdc++` to `libc++`
   * NOTE: this will probably require a corresponding change to your app's `Other Linker Flags`, from `-lstdc++` to `-lc++`.
   * (If you still need `libstdc++` for a component other than card.io, you should be able to specify BOTH `-lstdc++` AND `-lc++`.)
* Add a new dependency: include SystemConfiguration.framework in your app's required libraries.
* Add translations of all strings into ~20 languages, in addition to American English.
   * Translation choice is controlled by a new `languageOrLocale` property of `PayPalPaymentViewController`.
   * NOTE: Default language, if not set by your app, will now be based upon the device's current language setting.
* Clarify library behavior if you pass `nil` for `payerId`. (See `PayPalPaymentViewController.h` for details.)
* Update card.io library to 3.2.0
* Fix small bugs.

1.0.5
-----

* Fix [issue #18](https://github.com/paypal/PayPal-iOS-SDK/issues/18).
* New properties of `PayPalPaymentViewController`:
   * `disableBlurWhenBackgrounding` - blur screen when app is backgrounded, for security purposes. Enabled by default.
   * `state` - for optional key-value-observing.
* Update card.io library to 3.1.1.
* Fix small bugs.

1.0.4
-----

* `PayPalPaymentViewController` now only accepts `PayPalPayment` instances that are `processable`. Fixes [bug reported in comment in issue #7](https://github.com/paypal/PayPal-iOS-SDK/issues/7#issuecomment-15387415).
* The `prepareForPaymentUsingClientId:` call is now actually optional (and still recommended). Fixes [issue reported on StackOverflow](http://stackoverflow.com/q/15556622/306657).

1.0.3
-----

* Update card.io library to 3.0.9.
    * Fixes [issue #10](https://github.com/paypal/PayPal-iOS-SDK/issues/10) in which the scanned card image obscured manual entry fields on iPad in landscape.
* Add `PayPalPayment` convenience contructor.
* Fix small bugs.
* Add BSD license.

1.0.2
-----

* Bugfixes.
* Minor Sample App UI improvements.

1.0.1
-----

* Bugfixes and minor enhancements.
* Update card.io to 3.0.8.

1.0.0
-----

* Hello world! Add support for paying with PayPal or a credit card in a native iOS app.
