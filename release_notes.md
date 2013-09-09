PayPal iOS SDK release notes
============================

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
