#ifndef __AppDelegate_H__
#define __AppDelegate_H__

#include "memwar.h"
#include "touchview.h"

#ifdef __OBJC__

#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>

// Defaulting to 2 means that we run at 30 frames per second. For 60 frames, use a value of 1.
// 30 FPS is usually sufficient and results in lower power consumption.
#define DISPLAYLINK_FRAME_INTERVAL      2


@interface AppDelegate : NSObject <UIApplicationDelegate, UIAccelerometerDelegate>
{
   NSTimer *mTimer;
   MemWar::MemWarMain* gameApp;
    
   // Use of the CADisplayLink class is the preferred method for controlling your animation timing.
   // CADisplayLink will link to the main display and fire every vsync when added to a given run-loop.
   // The NSTimer class is used only as fallback when running on a pre 3.1 device where CADisplayLink
   // isn't available.
   id mDisplayLink;
   NSDate *mDate;
   NSTimeInterval mLastFrameTime;
}

- (void)go;
- (void)renderOneFrame:(id)sender;

@property (nonatomic) NSTimeInterval mLastFrameTime;

@end

@implementation AppDelegate

@dynamic mLastFrameTime;

- (NSTimeInterval)mLastFrameTime
{
   return mLastFrameTime;
}

/**
 Returns the URL to the application's Documents directory.
 */
- (NSURL *)applicationDocumentsDirectory
{
   return [[[NSFileManager defaultManager] URLsForDirectory:NSDocumentDirectory
                                                  inDomains:NSUserDomainMask] lastObject];
}

- (void)setLastFrameTime:(double)frameInterval
{
    // Frame interval defines how many display frames must pass between each time the
    // display link fires. The display link will only fire 30 times a second when the
    // frame internal is two on a display that refreshes 60 times a second. The default
    // frame interval setting of one will fire 60 times a second when the display refreshes
    // at 60 times a second. A frame interval setting of less than one results in undefined
    // behavior.
    if (frameInterval >= 1)
    {
        mLastFrameTime = frameInterval;
    }
}

- (void)go 
{
   NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
   NSURL* docUrl = [self applicationDocumentsDirectory];
   NSString* docString = [docUrl absoluteString];
   
   CGRect screenBounds = [[UIScreen mainScreen] bounds];
   CGFloat screenScale = [[UIScreen mainScreen] scale];
   CGSize screenSize = CGSizeMake(screenBounds.size.width ,
                                  screenBounds.size.height );
   
   /* Create the game */
   gameApp = new MemWar::MemWarMain();
   gameApp->create([docString UTF8String], screenSize.width, screenSize.height,
                   screenScale);

   [pool release];
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
   mLastFrameTime = DISPLAYLINK_FRAME_INTERVAL;
   mDisplayLink = nil;
   
   [self go];
   
   /* Retrieve Ogre UiViewController and UiView */
   unsigned long hView = 0;
   gameApp->getWindow()->getCustomAttribute("VIEWCONTROLLER", &hView);
   //UIViewController* ogreViewController = (UIViewController*)hView;
   gameApp->getWindow()->getCustomAttribute("VIEW", &hView);
   UIView* ogreView = (UIView*)hView;
   
   /* Create a subview only for touch input */
   Ogre::RenderWindow *mWindow = gameApp->getWindow();
   UIWindow *mUIWindow = nil;
   mWindow->getCustomAttribute("WINDOW", &mUIWindow);
   TouchView *myView = [[TouchView alloc] initWithFrame:[mUIWindow bounds]];
   [myView setBackgroundColor:[UIColor clearColor]];
   myView.multipleTouchEnabled = YES;
   [myView setExclusiveTouch:false];
   [ogreView addSubview:myView];
   
   [mUIWindow makeKeyAndVisible];
   
   return YES;
   
}

- (void)applicationWillTerminate:(UIApplication *)application
{
   delete gameApp;
   gameApp = nil;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
   //gameApp->sendToBackground();
   
   [mDate release];
   mDate = nil;
   
   [mDisplayLink invalidate];
   mDisplayLink = nil;
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
   //gameApp->sendToForeground();
   mDate = [[NSDate alloc] init];
   mLastFrameTime = DISPLAYLINK_FRAME_INTERVAL; // Reset the timer
   
   mDisplayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(renderOneFrame:)];
   [mDisplayLink setFrameInterval:mLastFrameTime];
   [mDisplayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
}

- (void)renderOneFrame:(id)sender
{
   NSTimeInterval currentFrameTime = -[mDate timeIntervalSinceNow];
   //NSTimeInterval differenceInSeconds = currentFrameTime - mLastFrameTime;
   mLastFrameTime = currentFrameTime;
   
   dispatch_async(dispatch_get_main_queue(), ^(void)
                  {
                     if(gameApp != nil)
                     {
                        gameApp->run();
                     }
                  });
}

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application
{
   /* Some clean up, if possible */
   //gameApp->lowMemoryClean();
}

@end

#endif

#endif
