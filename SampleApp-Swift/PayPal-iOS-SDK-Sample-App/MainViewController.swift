//
//  MainViewController.swift
//  PayPal-iOS-SDK-Sample-App
//
//  Copyright (c) 2015-2016 PayPal, Inc. All rights reserved.
//

import UIKit


class MainViewController: UIViewController, PayPalPaymentDelegate, PayPalFuturePaymentDelegate, PayPalProfileSharingDelegate, FlipsideViewControllerDelegate {
  var environment:String = PayPalEnvironmentNoNetwork {
    willSet(newEnvironment) {
      if (newEnvironment != environment) {
        PayPalMobile.preconnectWithEnvironment(newEnvironment)
      }
    }
  }

#if HAS_CARDIO
  // You should use the PayPal-iOS-SDK+card-Sample-App target to enable this setting.
  // For your apps, you will need to link to the libCardIO and dependent libraries. Please read the README.md
  // for more details.
  var acceptCreditCards: Bool = true {
    didSet {
      payPalConfig.acceptCreditCards = acceptCreditCards
    }
  }
#else
  var acceptCreditCards: Bool = false {
    didSet {
      payPalConfig.acceptCreditCards = acceptCreditCards
    }
  }
#endif

  var resultText = "" // empty
  var payPalConfig = PayPalConfiguration() // default
  
  @IBOutlet weak var successView: UIView!
  
  override func viewDidLoad() {
    super.viewDidLoad()
    
    // Do any additional setup after loading the view.
    title = "PayPal SDK Demo"
    successView.hidden = true
    
    // Set up payPalConfig
    payPalConfig.acceptCreditCards = acceptCreditCards;
    payPalConfig.merchantName = "Awesome Shirts, Inc."
    payPalConfig.merchantPrivacyPolicyURL = NSURL(string: "https://www.paypal.com/webapps/mpp/ua/privacy-full")
    payPalConfig.merchantUserAgreementURL = NSURL(string: "https://www.paypal.com/webapps/mpp/ua/useragreement-full")
    
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
    
    payPalConfig.languageOrLocale = NSLocale.preferredLanguages()[0] 
    
    // Setting the payPalShippingAddressOption property is optional.
    //
    // See PayPalConfiguration.h for details.
    
    payPalConfig.payPalShippingAddressOption = .PayPal;
    
    print("PayPal iOS SDK Version: \(PayPalMobile.libraryVersion())")
    
  }
  
  override func viewWillAppear(animated: Bool) {
    super.viewWillAppear(animated)
    PayPalMobile.preconnectWithEnvironment(environment)
  }
  
  
  // MARK: Single Payment
  @IBAction func buyClothingAction(sender: AnyObject) {
    // Remove our last completed payment, just for demo purposes.
    resultText = ""
    
    // Note: For purposes of illustration, this example shows a payment that includes
    //       both payment details (subtotal, shipping, tax) and multiple items.
    //       You would only specify these if appropriate to your situation.
    //       Otherwise, you can leave payment.items and/or payment.paymentDetails nil,
    //       and simply set payment.amount to your total charge.
    
    // Optional: include multiple items
    let item1 = PayPalItem(name: "Old jeans with holes", withQuantity: 2, withPrice: NSDecimalNumber(string: "84.99"), withCurrency: "USD", withSku: "Hip-0037")
    let item2 = PayPalItem(name: "Free rainbow patch", withQuantity: 1, withPrice: NSDecimalNumber(string: "0.00"), withCurrency: "USD", withSku: "Hip-00066")
    let item3 = PayPalItem(name: "Long-sleeve plaid shirt (mustache not included)", withQuantity: 1, withPrice: NSDecimalNumber(string: "37.99"), withCurrency: "USD", withSku: "Hip-00291")
    
    let items = [item1, item2, item3]
    let subtotal = PayPalItem.totalPriceForItems(items)
    
    // Optional: include payment details
    let shipping = NSDecimalNumber(string: "5.99")
    let tax = NSDecimalNumber(string: "2.50")
    let paymentDetails = PayPalPaymentDetails(subtotal: subtotal, withShipping: shipping, withTax: tax)
    
    let total = subtotal.decimalNumberByAdding(shipping).decimalNumberByAdding(tax)
    
    let payment = PayPalPayment(amount: total, currencyCode: "USD", shortDescription: "Hipster Clothing", intent: .Sale)
    
    payment.items = items
    payment.paymentDetails = paymentDetails
    
    if (payment.processable) {
      let paymentViewController = PayPalPaymentViewController(payment: payment, configuration: payPalConfig, delegate: self)
      presentViewController(paymentViewController, animated: true, completion: nil)
    }
    else {
      // This particular payment will always be processable. If, for
      // example, the amount was negative or the shortDescription was
      // empty, this payment wouldn't be processable, and you'd want
      // to handle that here.
      print("Payment not processalbe: \(payment)")
    }
    
  }
  
  // PayPalPaymentDelegate
  
  func payPalPaymentDidCancel(paymentViewController: PayPalPaymentViewController!) {
    print("PayPal Payment Cancelled")
    resultText = ""
    successView.hidden = true
    paymentViewController?.dismissViewControllerAnimated(true, completion: nil)
  }
  
  func payPalPaymentViewController(paymentViewController: PayPalPaymentViewController!, didCompletePayment completedPayment: PayPalPayment!) {
    print("PayPal Payment Success !")
    paymentViewController?.dismissViewControllerAnimated(true, completion: { () -> Void in
      // send completed confirmaion to your server
      print("Here is your proof of payment:\n\n\(completedPayment.confirmation)\n\nSend this to your server for confirmation and fulfillment.")
      
      self.resultText = completedPayment!.description
      self.showSuccess()
    })
  }
  
  
  // MARK: Future Payments
  
  @IBAction func authorizeFuturePaymentsAction(sender: AnyObject) {
    let futurePaymentViewController = PayPalFuturePaymentViewController(configuration: payPalConfig, delegate: self)
    presentViewController(futurePaymentViewController, animated: true, completion: nil)
  }
  
  
  func payPalFuturePaymentDidCancel(futurePaymentViewController: PayPalFuturePaymentViewController!) {
    print("PayPal Future Payment Authorization Canceled")
    successView.hidden = true
    futurePaymentViewController?.dismissViewControllerAnimated(true, completion: nil)
  }
  
  func payPalFuturePaymentViewController(futurePaymentViewController: PayPalFuturePaymentViewController!, didAuthorizeFuturePayment futurePaymentAuthorization: [NSObject : AnyObject]!) {
    print("PayPal Future Payment Authorization Success!")
    // send authorization to your server to get refresh token.
    futurePaymentViewController?.dismissViewControllerAnimated(true, completion: { () -> Void in
      self.resultText = futurePaymentAuthorization!.description
      self.showSuccess()
    })
  }
  
  // MARK: Profile Sharing
  
  @IBAction func authorizeProfileSharingAction(sender: AnyObject) {
    let scopes = [kPayPalOAuth2ScopeOpenId, kPayPalOAuth2ScopeEmail, kPayPalOAuth2ScopeAddress, kPayPalOAuth2ScopePhone]
    let profileSharingViewController = PayPalProfileSharingViewController(scopeValues: NSSet(array: scopes) as Set<NSObject>, configuration: payPalConfig, delegate: self)
    presentViewController(profileSharingViewController, animated: true, completion: nil)
  }
  
  // PayPalProfileSharingDelegate
  
  func userDidCancelPayPalProfileSharingViewController(profileSharingViewController: PayPalProfileSharingViewController!) {
    print("PayPal Profile Sharing Authorization Canceled")
    successView.hidden = true
    profileSharingViewController?.dismissViewControllerAnimated(true, completion: nil)
  }
  
  func payPalProfileSharingViewController(profileSharingViewController: PayPalProfileSharingViewController!, userDidLogInWithAuthorization profileSharingAuthorization: [NSObject : AnyObject]!) {
    print("PayPal Profile Sharing Authorization Success!")
    
    // send authorization to your server
    
    profileSharingViewController?.dismissViewControllerAnimated(true, completion: { () -> Void in
      self.resultText = profileSharingAuthorization!.description
      self.showSuccess()
    })

  }

  
  // MARK: - Navigation
  // In a storyboard-based application, you will often want to do a little preparation before navigation
  override func prepareForSegue(segue: UIStoryboardSegue, sender: AnyObject?) {
    // Get the new view controller using segue.destinationViewController.
    // Pass the selected object to the new view controller.
    
    if segue.identifier == "pushSettings" {
      // [segue destinationViewController] setDelegate:(id)self];
      if let flipSideViewController = segue.destinationViewController as? FlipsideViewController {
        flipSideViewController.flipsideDelegate = self
      }
    }
  }
  
  
  // MARK: Helpers
  
  func showSuccess() {
    successView.hidden = false
    successView.alpha = 1.0
    UIView.beginAnimations(nil, context: nil)
    UIView.setAnimationDuration(0.5)
    UIView.setAnimationDelay(2.0)
    successView.alpha = 0.0
    UIView.commitAnimations()
  }
  
  
  
  
  // MARK: Memory
  
  override func didReceiveMemoryWarning() {
    super.didReceiveMemoryWarning()
    // Dispose of any resources that can be recreated.
  }
  
  
  
  
  
}
