Pod::Spec.new do |spec|
  spec.name             = 'PayPal-iOS-SDK'
  spec.version          = '2.8.1'
  spec.license          = { type: 'BSD', file: 'LICENSE.md' }
  spec.homepage         = 'https://developer.paypal.com/webapps/developer/docs/integration/mobile/mobile-sdk-overview/'
  spec.authors          = { 'PayPal' => 'support@paypal.com' }
  spec.summary          = 'Accept credit cards and PayPal in your iOS app.'
  spec.description      = 'The PayPal iOS SDK provides a software library that makes it easy for an iOS developer to accept both credit cards and PayPal directly within their mobile app.'
  spec.source           = { :git => 'https://github.com/paypal/PayPal-iOS-SDK.git', :tag => "#{spec.version}" }
  spec.platform         = :ios, '6.0'
  spec.ios.deployment_target = '6.0'
  spec.requires_arc     = false
  spec.source_files     = 'PayPalMobile/*.h'
  spec.preserve_path    = 'PayPalMobile/*.a'
  spec.frameworks       = 'AudioToolbox', 'AVFoundation', 'CoreLocation', 'CoreMedia', 'MessageUI', 'MobileCoreServices', 'SystemConfiguration'
  spec.libraries        = 'PayPalMobile'
  spec.compiler_flags   = { 'CLANG_ENABLE_MODULES' => '1', 'CLANG_MODULES_AUTOLINK' => '1' }
  spec.xcconfig         = { 'LIBRARY_SEARCH_PATHS' => '"$(PODS_ROOT)/PayPal-iOS-SDK/PayPalMobile"', 'OTHER_LDFLAGS' => '-lc++ -ObjC'}
end
