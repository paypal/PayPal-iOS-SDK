//
//  ZZFlipsideViewController.m
//  PayPal-iOS-SDK-Sample-App
//
//  Copyright (c) 2013, PayPal
//  All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//
//  1. Redistributions of source code must retain the above copyright notice, this
//  list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright notice,
//  this list of conditions and the following disclaimer in the documentation
//  and/or other materials provided with the distribution.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
//  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
//  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
//  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//  The views and conclusions contained in the software and documentation are those
//  of the authors and should not be interpreted as representing official policies,
//  either expressed or implied, of the FreeBSD Project.
//

#import "ZZFlipsideViewController.h"
#import <QuartzCore/QuartzCore.h>

@interface ZZFlipsideViewController ()

@property(nonatomic, strong, readwrite) IBOutlet UISegmentedControl *environmentSegmentedControl;
@property(nonatomic, strong, readwrite) IBOutlet UISwitch *acceptCreditCardsSwitch;
@property(nonatomic, strong, readwrite) IBOutlet UITextView *proofOfPaymentTextView;
@property(nonatomic, strong, readwrite) IBOutlet UILabel *proofOfPaymentLabel;
@end

@implementation ZZFlipsideViewController

- (void)awakeFromNib {
  self.contentSizeForViewInPopover = CGSizeMake(320.0, 480.0);
  [super awakeFromNib];
}

- (void)viewDidLoad {
  [super viewDidLoad];
  
}

- (void)logEnvironment {
  NSLog(@"Environment: %@. Accept credit cards? %d", self.delegate.environment, self.delegate.acceptCreditCards);
}

- (void)viewWillAppear:(BOOL)animated {
  [self logEnvironment];
  if ([PayPalEnvironmentProduction isEqualToString:self.delegate.environment]) {
    self.environmentSegmentedControl.selectedSegmentIndex = 2;
  } else if ([PayPalEnvironmentSandbox isEqualToString:self.delegate.environment]) {
    self.environmentSegmentedControl.selectedSegmentIndex = 1;
  } else if ([PayPalEnvironmentNoNetwork isEqualToString:self.delegate.environment]) {
    self.environmentSegmentedControl.selectedSegmentIndex = 0;
  }
  self.acceptCreditCardsSwitch.on = self.delegate.acceptCreditCards;

  if ([self.delegate completedPayment]) {
    NSLog(@"%@", [self.delegate completedPayment]);
    self.proofOfPaymentTextView.text = [[NSString stringWithFormat:@"%@", [self.delegate completedPayment]] stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
    
    
  } else {
    self.proofOfPaymentTextView.hidden = YES;
    self.proofOfPaymentLabel.hidden = YES;
  }
  self.proofOfPaymentTextView.layer.cornerRadius = 8.0f;
}

- (void)didReceiveMemoryWarning {
  [super didReceiveMemoryWarning];
  // Dispose of any resources that can be recreated.
}

#pragma mark - Actions

- (IBAction)environmentControlDidUpdate:(id)sender {
  switch (self.environmentSegmentedControl.selectedSegmentIndex) {
    case 0:
      self.delegate.environment = PayPalEnvironmentNoNetwork;
      break;
    case 1:
      self.delegate.environment = PayPalEnvironmentSandbox;
      break;
    default:
      self.delegate.environment = PayPalEnvironmentProduction;
      break;
  }
  [self logEnvironment];
}

- (IBAction)processCreditCardsChanged:(id)sender {
  [self.delegate setAcceptCreditCards:self.acceptCreditCardsSwitch.on];
  [self logEnvironment];
}

- (IBAction)done:(id)sender {
  [self logEnvironment];
  [self.delegate flipsideViewControllerDidFinish:self];
}

@end
