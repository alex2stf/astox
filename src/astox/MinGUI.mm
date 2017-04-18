


#ifdef ASTX_ENABLE_OBJECTIVECPP

    #include <Foundation/Foundation.h>
    #include <CoreFoundation/CoreFoundation.h>
    #include <objc/runtime.h>
    #include <Cocoa/Cocoa.h>
    #include "MinGUI.h"


NSString *fakeBundleIdentifier = nil;

@implementation NSBundle(swizle)
- (NSString *) __bundleIdentifier {
    if (self == [NSBundle mainBundle]) {
        return fakeBundleIdentifier ? fakeBundleIdentifier : @"com.apple.finder";
    } else {
        return [self __bundleIdentifier];
    }
}

@end

BOOL installNSBundleHook() {
    Class classp = objc_getClass("NSBundle");
    if (classp) {
        method_exchangeImplementations(class_getInstanceMethod(classp, @selector(bundleIdentifier)),
                class_getInstanceMethod(classp, @selector(__bundleIdentifier)));
        return YES;
    }
    return NO;
}

BOOL keepRunning = YES;

@interface NotificationCenterDelegate : NSObject<NSUserNotificationCenterDelegate>
//@property (nonatomic, assign)
@end

@implementation NotificationCenterDelegate
- (void) userNotificationCenter : (NSUserNotificationCenter *) center didDeliverNotification : (NSUserNotification *) notification {
    keepRunning = NO;
}

@end

void MINGUI_Notification(const char * title, const char * subtitle, const char * footer, int lifetime) {
    if (installNSBundleHook()) {
        NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];

        fakeBundleIdentifier = [defaults stringForKey : @"identifier"];

        NSUserNotificationCenter *nc = [NSUserNotificationCenter defaultUserNotificationCenter];
        NotificationCenterDelegate *ncDelegate = [[NotificationCenterDelegate alloc]init];
        //        NSUserNotificationCenterDelegate *ncDelegate = [[NSUserNotificationCenterDelegate alloc] init];
        //        ncDelegate.keepRunning = YES;
        nc.delegate = ncDelegate;



        //        [nc.delegate keepRunning: YES];

        NSUserNotification *note = [[NSUserNotification alloc] init];
        note.title = [defaults stringForKey : @"title"];
        note.subtitle = [defaults stringForKey : @"subtitle"];
        note.informativeText = [defaults stringForKey : @"informativeText"];

        if (!(note.title || note.subtitle || note.informativeText)) {
            note.title = [[NSString alloc] initWithUTF8String : title]; //@"Usage: usernotification";
            note.subtitle = [[NSString alloc] initWithUTF8String : subtitle];
            note.informativeText = [[NSString alloc] initWithUTF8String : footer]; //@"Options: [-identifier <IDENTIFIER>] [-title <TEXT>] [-subtitle TEXT] [-informativeText TEXT]";
        }

        [nc deliverNotification : note];

        while (keepRunning) {
            [[NSRunLoop currentRunLoop] runUntilDate : [NSDate dateWithTimeIntervalSinceNow : 0.1]];
        }
    }
}

int MINGUI_Dialog(const char * title, const char * message, void (*callback)(int stat, int pid), int flags, int pid) {
    CFStringRef header_ref = CFStringCreateWithCString(NULL, title, strlen(title));
    CFStringRef message_ref = CFStringCreateWithCString(NULL, message, strlen(message));
    //    printf("%s & %s flags: %i\n", title, message, flags);
    //    CFUserNotificationRef pDlg = NULL;
    //    const void* keys[] = { kCFUserNotificationAlertHeaderKey,  kCFUserNotificationAlertMessageKey };
    //    const void* vals[] = { header_ref,  message_ref };
    //
    //    CFDictionaryRef dict = CFDictionaryCreate(0, keys, vals,
    //                                              sizeof(keys)/sizeof(*keys),
    //                                              &kCFTypeDictionaryKeyCallBacks,
    //                                              &kCFTypeDictionaryValueCallBacks);
    //
    //
    //
    //    pDlg = CFUserNotificationCreate(kCFAllocatorDefault, 0, kCFUserNotificationPlainAlertLevel, &nRes, dict);

    CFOptionFlags result;
    //    kCFUserNotificationAlertHeaderKey
    //kCFUserNotificationPlainAlertLevel

    switch (flags) {
        case DG_DEFAULT:
            CFUserNotificationDisplayAlert(0, kCFUserNotificationPlainAlertLevel, NULL, NULL, NULL, header_ref, message_ref, NULL, NULL, NULL, &result);
            break;
        case DG_OK_WARNING:
            CFUserNotificationDisplayAlert(0, kCFUserNotificationCautionAlertLevel, NULL, NULL, NULL, header_ref, message_ref, NULL, NULL, NULL, &result);
            break;
        case DG_OK_ERROR:
            CFUserNotificationDisplayAlert(0, kCFUserNotificationStopAlertLevel, NULL, NULL, NULL, header_ref, message_ref, NULL, NULL, NULL, &result);
            break;
        case DG_OK_INFO:
            CFUserNotificationDisplayAlert(0, kCFUserNotificationNoteAlertLevel, NULL, NULL, NULL, header_ref, message_ref, NULL, NULL, NULL, &result);
            break;

        case DG_OKCANCEL:
            CFUserNotificationDisplayAlert(0, kCFUserNotificationPlainAlertLevel, NULL, NULL, NULL, header_ref, message_ref, NULL, CFSTR("Cancel"), NULL, &result);
            break;
        case DG_OKCANCEL_WARNING:
            CFUserNotificationDisplayAlert(0, kCFUserNotificationCautionAlertLevel, NULL, NULL, NULL, header_ref, message_ref, NULL, CFSTR("Cancel"), NULL, &result);
            break;
        case DG_OKCANCEL_ERROR:
            CFUserNotificationDisplayAlert(0, kCFUserNotificationStopAlertLevel, NULL, NULL, NULL, header_ref, message_ref, NULL, CFSTR("Cancel"), NULL, &result);
            break;
        case DG_OKCANCEL_INFO:
            CFUserNotificationDisplayAlert(0, kCFUserNotificationNoteAlertLevel, NULL, NULL, NULL, header_ref, message_ref, NULL, CFSTR("Cancel"), NULL, &result);
            break;

        case DG_YESNO:
            CFUserNotificationDisplayAlert(0, kCFUserNotificationPlainAlertLevel, NULL, NULL, NULL, header_ref, message_ref, CFSTR("Yes"), CFSTR("No"), NULL, &result);
            break;
        case DG_YESNO_WARNING:
            CFUserNotificationDisplayAlert(0, kCFUserNotificationCautionAlertLevel, NULL, NULL, NULL, header_ref, message_ref, CFSTR("Yes"), CFSTR("No"), NULL, &result);
            break;
        case DG_YESNO_ERROR:
            CFUserNotificationDisplayAlert(0, kCFUserNotificationStopAlertLevel, NULL, NULL, NULL, header_ref, message_ref, CFSTR("Yes"), CFSTR("No"), NULL, &result);
            break;
        case DG_YESNO_INFO:
            CFUserNotificationDisplayAlert(0, kCFUserNotificationNoteAlertLevel, NULL, NULL, NULL, header_ref, message_ref, CFSTR("Yes"), CFSTR("No"), NULL, &result);
            break;

        default:
            CFUserNotificationDisplayAlert(
                    0, // no timeout
                    kCFUserNotificationPlainAlertLevel, //change it depending message_type flags ( MB_ICONASTERISK.... etc.)
                    NULL, //icon url, use default, you can change it depending message_type flags
                    NULL, //not used
                    NULL, //localization of strings
                    header_ref, //header text
                    message_ref, //message text
                    NULL, //default "ok" text in button
                    NULL, //CFSTR("Cancel"), //alternate button title
                    NULL, //other button title, null--> no other button
                    &result //response flags
                    );
            break;
    }


    if (callback != NULL) {
        callback(result, pid);
    }

    if (flags == DG_YESNO || flags == DG_YESNO_ERROR || flags == DG_YESNO_INFO || flags == DG_YESNO_WARNING) {
        if (result == 0) {
            return DGANSW_YES;
        }
        return DGANSW_NO;
    }

    return result;


    //    CFRelease( header_ref );
    //    CFRelease( message_ref );

}

OpenFileResult MINGUI_OpenFile(const char * selectTitle, const char * message, const char * startupDirectory, bool canChooseFiles, bool canChooseDirectories) {
    OpenFileResult response;
    //open pane code
    NSString *nstitle = [[NSString alloc] initWithUTF8String : selectTitle];
    NSString *nsmessage = [[NSString alloc] initWithUTF8String : message];
    NSString *nsdirectory = [[NSString alloc] initWithUTF8String : startupDirectory];


    NSOpenPanel * panel = [NSOpenPanel openPanel];
    [panel setAllowsMultipleSelection : NO];
    [panel setResolvesAliases : YES];

    if (canChooseFiles) {
        [panel setCanChooseFiles : YES];
    } else {
        [panel setCanChooseFiles : NO];
    }

    if (canChooseDirectories) {
        [panel setCanChooseDirectories : YES];
    } else {
        [panel setCanChooseDirectories : NO];
    }

    [panel setDirectoryURL : [NSURL fileURLWithPath : nsdirectory]];
    [panel setTreatsFilePackagesAsDirectories : YES];
    [panel setMessage : nsmessage];
    [panel setPrompt : nstitle];
    response.result = [panel runModal];

    NSString *sp = [[panel URL] path];
    response.filePath = [sp cStringUsingEncoding : NSUTF8StringEncoding];
    return response;

};

#endif
