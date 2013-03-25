PayPal iOS SDK release notes
============================

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