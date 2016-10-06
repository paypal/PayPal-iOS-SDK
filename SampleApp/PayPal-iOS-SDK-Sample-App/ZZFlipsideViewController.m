//
//  ZZFlipsideViewController.m
//  PayPal-iOS-SDK-Sample-App
//
//  Copyright (c) 2014-2016 PayPal, Inc. All rights reserved.
//  All rights reserved.
//

#import "ZZFlipsideViewController.h"
#import "ZZMainViewController.h"
#import <QuartzCore/QuartzCore.h>

@interface ZZFlipsideViewController ()

@property(nonatomic, strong, readwrite) IBOutlet UISegmentedControl *environmentSegmentedControl;
@property(nonatomic, strong, readwrite) IBOutlet UITextView *payPalResultTextView;
@property(nonatomic, strong, readwrite) IBOutlet UILabel *payPalResultLabel;
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
  NSLog(@"Environment: %@.", self.delegate.environment);
}

- (void)viewWillAppear:(BOOL)animated {
  [self logEnvironment];

  int numberOfSegments = (int)self.environmentSegmentedControl.numberOfSegments;
  while (numberOfSegments--) {
    NSString *title = [self.environmentSegmentedControl titleForSegmentAtIndex:numberOfSegments];
    if ([[title lowercaseString] isEqualToString:self.delegate.environment]) {
      self.environmentSegmentedControl.selectedSegmentIndex = numberOfSegments;
      break;
    }
  }
  
  if ([self.delegate resultText]) {
    NSLog(@"%@", [self.delegate resultText]);
    self.payPalResultTextView.text = [[self.delegate resultText] stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
  } else {
    self.payPalResultTextView.hidden = YES;
    self.payPalResultLabel.hidden = YES;
  }
  self.payPalResultTextView.layer.cornerRadius = 8.0f;
}

- (void)didReceiveMemoryWarning {
  [super didReceiveMemoryWarning];
  // Dispose of any resources that can be recreated.
}

#pragma mark - Actions

- (IBAction)environmentControlDidUpdate:(id)sender {
  if (self.environmentSegmentedControl == sender) {
    [self.delegate setPayPalEnvironment:[[self.environmentSegmentedControl titleForSegmentAtIndex:self.environmentSegmentedControl.selectedSegmentIndex] lowercaseString]];
  }
  [self logEnvironment];
}

@end
