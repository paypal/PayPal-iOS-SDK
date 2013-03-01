//
//  ZZFlipsideViewController.m
//  PayPal-iOS-SDK-Sample-App
//
//  Copyright (c) 2013 PayPal. All rights reserved.
//

#import "ZZFlipsideViewController.h"

@interface ZZFlipsideViewController ()

@property(nonatomic, strong, readwrite) IBOutlet UISegmentedControl *environmentSegmentedControl;
@property(nonatomic, strong, readwrite) IBOutlet UISwitch *acceptCreditCardsSwitch;

@end

@implementation ZZFlipsideViewController

- (void)awakeFromNib {
  self.contentSizeForViewInPopover = CGSizeMake(320.0, 480.0);
  [super awakeFromNib];
}

- (void)viewDidLoad {
  [super viewDidLoad];
  // Do any additional setup after loading the view, typically from a nib.
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
