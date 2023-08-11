#if defined(__OBJC__)

#	include "sr_gui.h"
#	include "sr_gui_internal.h"

#	import <Foundation/Foundation.h>
#	import <AppKit/AppKit.h>

void sr_gui_init() {
	//Nothing.
}

void sr_gui_cleanup() {
	//Nothing.
}

void sr_gui_show_message(const char* title, const char* message, int level) {
	NSAlert* alert		  = [[NSAlert alloc] init];
	alert.messageText	  = [[NSString alloc] initWithUTF8String:title];
	alert.informativeText = [[NSString alloc] initWithUTF8String:message];
	// Pick the proper alert style.
	if(level == SR_GUI_MESSAGE_LEVEL_ERROR) {
		alert.alertStyle = NSAlertStyleCritical;
		// Icon will be app + warning sign.
	} else if(level == SR_GUI_MESSAGE_LEVEL_WARN) {
		alert.alertStyle = NSAlertStyleWarning;
		alert.icon		 = [NSImage imageNamed:NSImageNameCaution];
	} else {
		alert.alertStyle = NSAlertStyleInformational;
		alert.icon		 = [NSImage imageNamed:NSImageNameInfo];
	}
	// Run and release.
	[alert runModal];
	[alert release];
}

void sr_gui_show_notification(const char* title, const char* message) {
	NSUserNotification* notif = [[NSUserNotification alloc] init];
	notif.identifier		  = [[NSUUID UUID] UUIDString];
	notif.title				  = [[NSString alloc] initWithUTF8String:title];
	notif.informativeText	  = [[NSString alloc] initWithUTF8String:message];
	notif.soundName			  = NSUserNotificationDefaultSoundName;

	[[NSUserNotificationCenter defaultUserNotificationCenter] scheduleNotification:notif];
	[notif release];
}

int sr_gui_ask_directory(const char* title, const char* startDir, char** outPath) {
	*outPath = NULL;

	NSOpenPanel* panel = [NSOpenPanel openPanel];
	[panel setTitle:[NSString stringWithUTF8String:title]];
	[panel setAllowsMultipleSelection:NO];
	[panel setCanChooseFiles:NO];
	[panel setCanChooseDirectories:YES];
	[panel setCanCreateDirectories:YES];
	[panel setDirectoryURL:[NSURL fileURLWithPath:[NSString stringWithUTF8String:startDir]]];

	NSModalResponse res = [panel runModal];
	if(res != NSModalResponseOK) {
		[panel release];
		return SR_GUI_CANCELLED;
	}

	const char* resStr = [[[panel URL] path] UTF8String];
	const int strSize  = strlen(resStr);

	*outPath = (char*)SR_GUI_MALLOC(sizeof(char) * (strSize + 1));
	SR_GUI_MEMCPY(*outPath, resStr, sizeof(char) * strSize);
	(*outPath)[strSize] = '\0';

	[panel release];
	return SR_GUI_VALIDATED;
}

int sr_gui_ask_load_files(const char* title, const char* startDir, const char* exts, char*** outPaths, int* outCount) {
	*outCount = 0;
	*outPaths = NULL;

	NSOpenPanel* panel = [NSOpenPanel openPanel];
	[panel setTitle:[NSString stringWithUTF8String:title]];
	[panel setAllowsMultipleSelection:YES];
	[panel setCanChooseFiles:YES];
	[panel setCanChooseDirectories:NO];
	[panel setDirectoryURL:[NSURL fileURLWithPath:[NSString stringWithUTF8String:startDir]]];

	if(exts != NULL && strlen(exts) > 0) {
		NSString* extsStr	 = [NSString stringWithUTF8String:exts];
		NSArray* allowedExts = [extsStr componentsSeparatedByString:@","];
		[panel setAllowedFileTypes:allowedExts];
		[panel setAllowsOtherFileTypes:NO];
	}

	NSModalResponse res = [panel runModal];
	if(res != NSModalResponseOK) {
		[panel release];
		return SR_GUI_CANCELLED;
	}

	NSArray* urls = [panel URLs];
	*outCount	  = [urls count];
	if(*outCount > 0) {
		*outPaths = (char**)SR_GUI_MALLOC(sizeof(char*) * (*outCount));
		for(int i = 0; i < *outCount; ++i) {
			const char* resStr = [[[urls objectAtIndex:i] path] UTF8String];
			const int strSize  = strlen(resStr);
			(*outPaths)[i]	   = (char*)SR_GUI_MALLOC(sizeof(char) * (strSize + 1));
			SR_GUI_MEMCPY((*outPaths)[i], resStr, sizeof(char) * strSize);
			(*outPaths)[i][strSize] = '\0';
		}
	}

	[panel release];
	return SR_GUI_VALIDATED;
}


int sr_gui_ask_load_file(const char* title, const char* startDir, const char* exts, char** outPath) {
	*outPath = NULL;

	NSOpenPanel* panel = [NSOpenPanel openPanel];
	[panel setTitle:[NSString stringWithUTF8String:title]];
	[panel setAllowsMultipleSelection:NO];
	[panel setCanChooseFiles:YES];
	[panel setCanChooseDirectories:NO];
	[panel setDirectoryURL:[NSURL fileURLWithPath:[NSString stringWithUTF8String:startDir]]];

	if(exts != NULL && strlen(exts) > 0) {
		NSString* extsStr	 = [NSString stringWithUTF8String:exts];
		NSArray* allowedExts = [extsStr componentsSeparatedByString:@","];
		[panel setAllowedFileTypes:allowedExts];
		[panel setAllowsOtherFileTypes:NO];
	}

	NSModalResponse res = [panel runModal];
	if(res != NSModalResponseOK) {
		[panel release];
		return SR_GUI_CANCELLED;
	}

	NSArray* urls = [panel URLs];
	const int outCount	  = [urls count];
	if(outCount > 0) {
		const char* resStr = [[[urls objectAtIndex:0] path] UTF8String];
		const int strSize  = strlen(resStr);

		*outPath = (char*)SR_GUI_MALLOC(sizeof(char) * (strSize + 1));
		SR_GUI_MEMCPY(*outPath, resStr, sizeof(char) * strSize);
		(*outPath)[strSize] = '\0';
	}

	[panel release];
	return SR_GUI_VALIDATED;
}

int sr_gui_ask_save_file(const char* title, const char* startDir, const char* exts, char** outPath) {
	*outPath = NULL;

	NSSavePanel* panel = [NSSavePanel savePanel];
	[panel setTitle:[NSString stringWithUTF8String:title]];
	[panel setExtensionHidden:NO];
	[panel setCanCreateDirectories:YES];
	[panel setDirectoryURL:[NSURL fileURLWithPath:[NSString stringWithUTF8String:startDir]]];

	if(exts != NULL && strlen(exts) > 0) {
		NSString* extsStr	 = [NSString stringWithUTF8String:exts];
		NSArray* allowedExts = [extsStr componentsSeparatedByString:@","];
		[panel setAllowedFileTypes:allowedExts];
		[panel setAllowsOtherFileTypes:NO];
	}

	NSModalResponse res = [panel runModal];

	if(res != NSModalResponseOK) {
		[panel release];
		return SR_GUI_CANCELLED;
	}

	const char* resStr = [[[panel URL] path] UTF8String];
	const int strSize  = strlen(resStr);

	*outPath = (char*)SR_GUI_MALLOC(sizeof(char) * (strSize + 1));
	SR_GUI_MEMCPY(*outPath, resStr, sizeof(char) * strSize);
	(*outPath)[strSize] = '\0';

	[panel release];
	return SR_GUI_VALIDATED;
}

int sr_gui_ask_choice(const char* title, const char* message, int level, const char* button0, const char* button1, const char* button2) {

	const char* buttons[] = {button0, button1, button2};
	const int bCount = sizeof(buttons)/sizeof(buttons[0]);

	NSAlert* alert		  = [[NSAlert alloc] init];
	alert.messageText	  = [[NSString alloc] initWithUTF8String:title];
	alert.informativeText = [[NSString alloc] initWithUTF8String:message];
	// Pick the proper alert style.
	if(level == SR_GUI_MESSAGE_LEVEL_ERROR) {
		alert.alertStyle = NSAlertStyleCritical;
		// Icon will be app + warning sign.
	} else if(level == SR_GUI_MESSAGE_LEVEL_WARN) {
		alert.alertStyle = NSAlertStyleWarning;
		alert.icon		 = [NSImage imageNamed:NSImageNameCaution];
	} else {
		alert.alertStyle = NSAlertStyleInformational;
		alert.icon		 = [NSImage imageNamed:NSImageNameInfo];
	}

	// We allow some labels to be null, and should skip them while preserving the returned index.
	// (pressing button2 should always return 2 even if button1 == NULL)
	int localIndex = 0;
	for(int bid = 0; bid < bCount; ++bid){
		if(buttons[bid] == NULL){
			continue;
		}
		[alert addButtonWithTitle: [[NSString alloc] initWithUTF8String: buttons[bid]]];
		[[[alert buttons] objectAtIndex: localIndex] setTag:(SR_GUI_BUTTON0 + bid)];
		++localIndex;
	}

	// Run and release.
	NSModalResponse rep = [alert runModal];
	[alert release];

	if(rep == NSModalResponseStop || rep == NSModalResponseAbort || rep == NSModalResponseCancel) {
		return SR_GUI_CANCELLED;
	}

	return (int)rep;
}

int sr_gui_ask_string(const char* title, const char* message, char** result) {

	NSAlert* alert		  = [[NSAlert alloc] init];
	alert.messageText	  = [[NSString alloc] initWithUTF8String:title];
	alert.informativeText = [[NSString alloc] initWithUTF8String:message];
	alert.alertStyle	  = NSAlertStyleInformational;
	// Buttons.
	[alert addButtonWithTitle:@"OK"];
	[[[alert buttons] objectAtIndex:0] setTag:SR_GUI_BUTTON0];
	[alert addButtonWithTitle:@"Cancel"];
	[[[alert buttons] objectAtIndex:1] setTag:SR_GUI_BUTTON1];
	// Add text field.
	NSTextField* field = [[NSTextField alloc] initWithFrame:NSMakeRect(0, 0, 200, 20)];
	[field setStringValue:[[NSString alloc] initWithUTF8String:*result]];
	[alert setAccessoryView:field];
	// Run and release.
	NSModalResponse rep = [alert runModal];

	if(rep != SR_GUI_BUTTON0) {
		[alert release];
		[field release];
		return SR_GUI_CANCELLED;
	}
	// We don't own the UTF8 string, copy it.
	const char* strRes	= [[field stringValue] UTF8String];
	const int resLength = strlen(strRes);

	*result = (char*)SR_GUI_MALLOC(sizeof(char) * (resLength + 1));
	SR_GUI_MEMCPY(*result, strRes, sizeof(char) * resLength);
	(*result)[resLength] = '\0';

	[alert release];
	[field release];
	return SR_GUI_VALIDATED;
}

@interface SRColorPanel : NSObject
@property(retain) NSColor* color;
@end

@implementation SRColorPanel

- (instancetype)initWithColor:(NSColor*)color shouldAskAlpha:(BOOL)alpha {
	self	   = [super init];
	self.color = color;
	// Register closing notification (we don't care about continuous updates).
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(colorClose:) name:NSWindowWillCloseNotification object:[NSColorPanel sharedColorPanel]];
	[[NSColorPanel sharedColorPanel] setColor:self.color];
	[[NSColorPanel sharedColorPanel] setShowsAlpha:alpha];

	return self;
}

- (void)colorClose:(NSNotification*)notif {
	self.color = [[NSColorPanel sharedColorPanel] color];
	[NSApp stopModalWithCode:NSModalResponseOK];
}

- (int)run {
	NSModalResponse res = [NSApp runModalForWindow:[NSColorPanel sharedColorPanel]];
	return res == NSModalResponseOK ? SR_GUI_VALIDATED : SR_GUI_CANCELLED;
}

@end

int sr_gui_ask_color(unsigned char color[3]) {

	CGFloat red		  = ((CGFloat)color[0]) / 255.0f;
	CGFloat gre		  = ((CGFloat)color[1]) / 255.0f;
	CGFloat blu		  = ((CGFloat)color[2]) / 255.0f;
	CGFloat alp		  = 1.0f;
	NSColor* inputCol = [NSColor colorWithRed:red green:gre blue:blu alpha:alp];

	SRColorPanel* panel = [[SRColorPanel alloc] initWithColor:inputCol shouldAskAlpha:NO];
	int res				= [panel run];
	if(res != SR_GUI_VALIDATED) {
		[panel release];
		[inputCol release];
		return res;
	}

	[[panel color] getRed:&red green:&gre blue:&blu alpha:&alp];

	color[0] = (unsigned char)(fmax(fmin((float)red * 255.0f, 255.0f), 0.0f));
	color[1] = (unsigned char)(fmax(fmin((float)gre * 255.0f, 255.0f), 0.0f));
	color[2] = (unsigned char)(fmax(fmin((float)blu * 255.0f, 255.0f), 0.0f));

	[panel release];
	[inputCol release];
	return SR_GUI_VALIDATED;
}

int sr_gui_open_in_explorer(const char* path){
	if(!path){
		return SR_GUI_CANCELLED;
	}
	NSString* pathStr = [[NSString alloc] initWithUTF8String:path];
	if(!pathStr){
		return SR_GUI_CANCELLED;
	}
	if([[NSWorkspace sharedWorkspace] selectFile:[pathStr stringByResolvingSymlinksInPath] inFileViewerRootedAtPath:@""]){
		return SR_GUI_VALIDATED;
	}
	return SR_GUI_CANCELLED;
}

#endif
