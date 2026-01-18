#include "WickedEngine.h"
#include "config.h"

#import <AppKit/AppKit.h>
#include <Carbon/Carbon.h>

wi::Application application;
bool running = true;

@interface WindowDelegate : NSObject <NSWindowDelegate>
@end

int main( int argc, char* argv[] )
{
    @autoreleasepool{
        [NSApplication sharedApplication];
        [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
        
        CGRect frame = (CGRect){ {100.0, 100.0}, {1280.0, 720.0} };
        NSWindow* window = [[NSWindow alloc] initWithContentRect:frame
                                                       styleMask:(NSWindowStyleMaskTitled |
                                                                  NSWindowStyleMaskClosable |
                                                                  NSWindowStyleMaskMiniaturizable |
                                                                  NSWindowStyleMaskResizable)
                                                         backing:NSBackingStoreBuffered
                                                           defer:NO];
        [window setTitle:@"Wicked Shooter"];
        [window center];
        [window makeKeyAndOrderFront:nil];
        
        WindowDelegate *delegate = [[WindowDelegate alloc] init];
        [window setDelegate:delegate];
        
        [NSApp activateIgnoringOtherApps:YES];
        
        application.SetWindow((__bridge wi::platform::window_type)window);
        
        wi::backlog::setFontColor(config::backlog_color);
        
        while (running)
        {
            @autoreleasepool{
                NSEvent* event;
                while ((event = [NSApp nextEventMatchingMask:NSEventMaskAny
                                                   untilDate:[NSDate distantPast]
                                                      inMode:NSDefaultRunLoopMode
                                                     dequeue:YES]))
                {
                    switch (event.type) {
                        case NSEventTypeKeyDown:
                            switch (event.keyCode) {
                                case kVK_Delete:
                                    wi::gui::TextInputField::DeleteFromInput();
                                    break;
                                case kVK_Return:
                                    break;
                                default:
                                {
                                    NSString* characters = event.characters;
                                    if (characters && characters.length > 0)
                                    {
                                        unichar c = [characters characterAtIndex:0];
                                        wchar_t wchar = (wchar_t)c;
                                        wi::gui::TextInputField::AddInput(wchar);
                                    }
                                }
                                break;
                            }
                            break;
                        case NSEventTypeKeyUp:
                            break;
                        case NSEventTypeScrollWheel:
                        {
                            float amount = -event.scrollingDeltaY;
                            if (event.hasPreciseScrollingDeltas)
                            {
                                amount *= -0.05f; // reversed touchpad scrolling
                            }
                            wi::input::AddMouseScrollEvent(amount);
                        }
                        break;
                        case NSEventTypeMouseMoved:
                        case NSEventTypeLeftMouseDragged:
                        case NSEventTypeRightMouseDragged:
                        case NSEventTypeOtherMouseDragged:
                            wi::input::AddMouseMoveDeltaEvent(XMFLOAT2(event.deltaX, event.deltaY));
                            break;
                        default:
                            [NSApp sendEvent:event];
                            break;
                    }
                }
                
                application.Run();

                static bool is_startup = false;
                if (!is_startup && wi::initializer::IsInitializeFinished())
                {
                    std::string path = wi::helper::GetExecutablePath();
                    size_t found;
                    found = path.find_last_of("/\\");
                    path = path.substr(0, found + 1);
                    wi::lua::RunFile(path + std::string(config::main_script_file));
                    is_startup = true;
                }
                
            }
        }
        
        wi::jobsystem::ShutDown();
    }
    
    return 0;
}

@implementation WindowDelegate
- (void)windowWillClose:(NSNotification *)notification {
    running = false;
}
- (void)windowDidResize:(NSNotification *)notification {
    NSWindow* nsWindow = (NSWindow*)notification.object;
    application.SetWindow((__bridge wi::platform::window_type)nsWindow);
}
@end
