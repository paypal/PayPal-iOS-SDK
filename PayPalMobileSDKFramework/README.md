# PayPalMobileSDK

This project is intended to help build a dynamic framework from the PayPal iOS
SDK static library.

Currently, this project is being tested and **unofficially supported**.
The name of the project and header files may be subject to change.

## Requirements

* Xcode 7 and iOS SDK 9
* iOS 8.0+ target deployment
* armv7, armv7s, and arm64 devices, and the simulator (not armv6)
* iPhone and iPad of all sizes and resolutions

The iOS 8.0+ target is different than the static library requirements because
iOS 8.0 is the first version of iOS to officially support dynamic frameworks.

If you require iOS < 8.0, then you must use the static library version instead.

## How to Use

You may depend on this framework project like other Xcode project dependencies
(drag and drop this project into your Xcode project, then embed and link the
framework to your app in the target's settings). Alternatively, you can
use Carthage to build and package the framework.

To call the code, add the following import.

```
#import <PayPalMobileSDK/PayPalMobileSDK.h>
```

This is the only import required, and other code snippets in the documentation
should work.

## Carthage

You may want to use [Carthage](https://github.com/Carthage/Carthage) to build
the framework.

You should add the following to a `Cartfile`.

```
github "paypal/PayPal-iOS-SDK"
github "card-io/card.io-iOS-source"
```

Note that you may have to add a specific branch or version number:

```
github "paypal/PayPal-iOS-SDK" "master"
```

Be sure to follow all of Carthage's instructions for integration. Carthage
currently requires a Run shell command Build Phase to package frameworks
correctly for App Store submission.

## Notes

The project is named `PayPalMobileSDK` due to conflicts with using just
`PayPalMobile` (due to it already being used as a class/header file name).
