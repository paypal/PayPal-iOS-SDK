//
//  AppDelegate.swift
//  PayPal-iOS-SDK-Sample-App
//
//  Created by Gujgiczer Máté on 10/03/16.
//  Copyright © 2016 PayPal. All rights reserved.
//

import UIKit

@UIApplicationMain
class AppDelegate: UIResponder, UIApplicationDelegate {
    var window : UIWindow?

     func application(application: UIApplication, didFinishLaunchingWithOptions launchOptions: [NSObject: AnyObject]?) -> Bool {
     
        //TODO: - Enter your credentials
        PayPalMobile .initializeWithClientIdsForEnvironments([PayPalEnvironmentProduction: "YOUR_CLIENT_ID_FOR_PRODUCTION",
                                                                 PayPalEnvironmentSandbox: "YOUR_CLIENT_ID_FOR_SANDBOX"])
        return true
    }
}
