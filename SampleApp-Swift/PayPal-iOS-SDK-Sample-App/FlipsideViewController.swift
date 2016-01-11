//
//  FlipsideViewController.swift
//  PayPal-iOS-SDK-Sample-App
//
//  Copyright (c) 2015-2016 PayPal, Inc. All rights reserved.
//

import UIKit

protocol FlipsideViewControllerDelegate {
  
  var acceptCreditCards: Bool { get set }
  var environment: String { get set }
  var resultText: String {get set}
}

class FlipsideViewController: UIViewController {
  
  @IBOutlet weak var environmentSegmentedControl: UISegmentedControl!
  @IBOutlet weak var acceptCreditCardsSwitch: UISwitch!
  @IBOutlet weak var payResultLabel: UILabel!
  @IBOutlet weak var payResultTextView: UITextView!
  
  var flipsideDelegate: FlipsideViewControllerDelegate?
  
  override func viewDidLoad() {
    super.viewDidLoad()
    
    // Do any additional setup after loading the view.
    preferredContentSize = CGSizeMake(320.0, 480.0)
  }
  
  override func didReceiveMemoryWarning() {
    super.didReceiveMemoryWarning()
    // Dispose of any resources that can be recreated.
  }
  
  func logEnvironment() {
    print("Environment: \(flipsideDelegate?.environment). Accept credit cards? \(flipsideDelegate?.acceptCreditCards)")
  }
  
  override func viewWillAppear(animated: Bool) {
    super.viewWillAppear(animated)
    logEnvironment()
    
    // select correct segment
    var numberOfSegments = environmentSegmentedControl.numberOfSegments
    while (numberOfSegments-- > 0) {
      let title = environmentSegmentedControl.titleForSegmentAtIndex(numberOfSegments)
      if title == flipsideDelegate!.environment {
        environmentSegmentedControl.selectedSegmentIndex = numberOfSegments
        break
      }
    }
    
    acceptCreditCardsSwitch.on = flipsideDelegate!.acceptCreditCards
    
#if !HAS_CARDIO
    acceptCreditCardsSwitch.enabled = false
#endif
    
    let resultText = flipsideDelegate!.resultText
    if !resultText.isEmpty {
      print("\(resultText)")
      payResultTextView.text = resultText.stringByTrimmingCharactersInSet(NSCharacterSet.whitespaceCharacterSet())
    } else {
      payResultTextView.hidden = true
      payResultLabel.hidden = true
    }
    payResultTextView.layer.cornerRadius = 8.0
  }
  
  
  
  @IBAction func environmentChanged(sender: AnyObject) {
    flipsideDelegate?.environment = environmentSegmentedControl.titleForSegmentAtIndex(environmentSegmentedControl.selectedSegmentIndex)!.lowercaseString
    logEnvironment()
  }
  
  
  @IBAction func switchChanged(sender: AnyObject) {
    flipsideDelegate?.acceptCreditCards = acceptCreditCardsSwitch.on
    logEnvironment()
  }
  
}
