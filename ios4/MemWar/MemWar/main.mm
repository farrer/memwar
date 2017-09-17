//|||||||||||||||||||||||||||||||||||||||||||||||

#include "AppDelegate.h"

int main(int argc, char **argv)
{
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	int retVal = UIApplicationMain(argc, argv, @"UIApplication", @"AppDelegate");
	[pool release];
	return retVal;
#elif (OGRE_PLATFORM == OGRE_PLATFORM_APPLE) && __LP64__
	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    
    mAppDelegate = [[AppDelegate alloc] init];
    [[NSApplication sharedApplication] setDelegate:mAppDelegate];
	int retVal = NSApplicationMain(argc, (const char **) argv);
    
	[pool release];
    
	return retVal;

#endif
}

//|||||||||||||||||||||||||||||||||||||||||||||||
