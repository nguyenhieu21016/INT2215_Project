//
//  INT2215_ProjectUITestsLaunchTests.m
//  INT2215_ProjectUITests
//
//  Created by Nguyen Hieu on 28/2/25.
//

#import <XCTest/XCTest.h>

@interface INT2215_ProjectUITestsLaunchTests : XCTestCase

@end

@implementation INT2215_ProjectUITestsLaunchTests

+ (BOOL)runsForEachTargetApplicationUIConfiguration {
    return YES;
}

- (void)setUp {
    self.continueAfterFailure = NO;
}

- (void)testLaunch {
    XCUIApplication *app = [[XCUIApplication alloc] init];
    [app launch];

    // Insert steps here to perform after app launch but before taking a screenshot,
    // such as logging into a test account or navigating somewhere in the app

    XCTAttachment *attachment = [XCTAttachment attachmentWithScreenshot:XCUIScreen.mainScreen.screenshot];
    attachment.name = @"Launch Screen";
    attachment.lifetime = XCTAttachmentLifetimeKeepAlways;
    [self addAttachment:attachment];
}

@end
