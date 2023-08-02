

#include "sr_gui.h"
#include "sr_gui_internal.h"

#include <libnotify/notify.h>
#include <gtk/gtk.h>

#include <math.h>

void sr_gui_init() {
	notify_init(SR_GUI_APP_NAME);
}

void sr_gui_cleanup() {
	notify_uninit();
}

void sr_gui_show_message(const char* title, const char* message, int level) {
	if(!gtk_init_check(NULL, NULL)) {
		return;
	}

	GtkMessageType levelGtk = GTK_MESSAGE_INFO;
	if(level == SR_GUI_MESSAGE_LEVEL_ERROR) {
		levelGtk = GTK_MESSAGE_ERROR;
	} else if(level == SR_GUI_MESSAGE_LEVEL_WARN) {
		levelGtk = GTK_MESSAGE_WARNING;
	}

	GtkWidget* dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, levelGtk, GTK_BUTTONS_OK, "%s", title);
	gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialog), "%s", message);

	int res = gtk_dialog_run(GTK_DIALOG(dialog));
	(void)res;
	gtk_widget_destroy(dialog);
}

void sr_gui_show_notification(const char* title, const char* message) {
	NotifyNotification* notif = notify_notification_new(title, message, NULL);
	notify_notification_set_timeout(notif, NOTIFY_EXPIRES_DEFAULT);
	notify_notification_set_urgency(notif, NOTIFY_URGENCY_NORMAL);

	gboolean res = notify_notification_show(notif, NULL);
	(void)res;
}

int _sr_gui_add_filter_extensions(const char* exts, GtkFileChooser* dialog) {

	if(!exts || strlen(exts) == 0) {
		GtkFileFilter* globfilter = gtk_file_filter_new();
		gtk_file_filter_add_pattern(globfilter, "*");
		gtk_file_filter_set_name(globfilter, "Any file");
		gtk_file_chooser_add_filter(dialog, globfilter);
		return 1;
	}

	size_t extsLen = strlen(exts);
	// Count extensions.
	int count = 1;
	for(size_t cid = 0; cid < extsLen; ++cid) {
		if(exts[cid] == ',') {
			++count;
		}
	}
	// Add wildcard at the end.
	++count;

	char extBuffer[SR_GUI_MAX_STR_SIZE];
	extBuffer[0] = '*';
	extBuffer[1] = '.';

	size_t cid = 0;
	for(size_t fid = 0; fid < (size_t)((int)(count)-1); ++fid) {
		// After the wildcard.
		size_t bid = 2;
		while(cid < extsLen && exts[cid] != ',') {
			extBuffer[bid] = exts[cid];
			++bid;
			++cid;
		}
		extBuffer[bid] = '\0';

		GtkFileFilter* filter = gtk_file_filter_new();
		gtk_file_filter_add_pattern(filter, extBuffer);
		gtk_file_filter_set_name(filter, &extBuffer[2]);
		gtk_file_chooser_add_filter(dialog, filter);

		// Skip comma.
		++cid;
		if(cid >= extsLen) {
			break;
		}
	}
	// Add global filter at the end.
	GtkFileFilter* globfilter = gtk_file_filter_new();
	gtk_file_filter_add_pattern(globfilter, "*");
	gtk_file_filter_set_name(globfilter, "Any file");
	gtk_file_chooser_add_filter(dialog, globfilter);
	return 1;
}

int sr_gui_ask_directory(const char* title, const char* startDir, char** outPath) {
	*outPath = NULL;
	if(!gtk_init_check(NULL, NULL)) {
		return SR_GUI_CANCELLED;
	}

	GtkFileChooserNative* pickerNative = gtk_file_chooser_native_new(title, NULL, GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER, NULL, NULL);
	GtkFileChooser* picker			   = GTK_FILE_CHOOSER(pickerNative);
	gtk_file_chooser_set_create_folders(picker, TRUE);
	gtk_file_chooser_set_current_folder(picker, startDir);

	int res = gtk_native_dialog_run(GTK_NATIVE_DIALOG(pickerNative));
	if(res != GTK_RESPONSE_ACCEPT) {
		g_object_unref(pickerNative);
		return SR_GUI_CANCELLED;
	}

	char* filename		 = gtk_file_chooser_get_filename(picker);
	const size_t fileLen = strlen(filename);
	*outPath			 = SR_GUI_MALLOC((fileLen + 1) * sizeof(char));
	if(*outPath == NULL) {
		g_free(filename);
		g_object_unref(pickerNative);
		return SR_GUI_CANCELLED;
	}
	SR_GUI_MEMCPY(*outPath, filename, fileLen * sizeof(char));
	(*outPath)[fileLen] = '\0';
	g_free(filename);
	g_object_unref(pickerNative);
	return SR_GUI_VALIDATED;
}

int sr_gui_ask_load_files(const char* title, const char* startDir, const char* exts, char*** outPaths, int* outCount) {
	*outCount = 0;
	*outPaths = NULL;
	if(!gtk_init_check(NULL, NULL)) {
		return SR_GUI_CANCELLED;
	}

	GtkFileChooserNative* pickerNative = gtk_file_chooser_native_new(title, NULL, GTK_FILE_CHOOSER_ACTION_OPEN, NULL, NULL);
	GtkFileChooser* picker			   = GTK_FILE_CHOOSER(pickerNative);
	gtk_file_chooser_set_current_folder(picker, startDir);
	gtk_file_chooser_set_select_multiple(picker, TRUE);

	if(_sr_gui_add_filter_extensions(exts, picker) != 1) {
		g_object_unref(pickerNative);
		return SR_GUI_CANCELLED;
	}

	int res = gtk_native_dialog_run(GTK_NATIVE_DIALOG(pickerNative));
	if(res != GTK_RESPONSE_ACCEPT) {
		g_object_unref(pickerNative);
		return SR_GUI_CANCELLED;
	}

	GSList* filenames	  = gtk_file_chooser_get_filenames(picker);
	const uint itemsCount = g_slist_length(filenames);

	if(itemsCount > 0) {
		*outPaths = (char**)SR_GUI_MALLOC(sizeof(char*) * itemsCount);
		if(*outPaths == NULL) {
			g_object_unref(pickerNative);
			g_slist_free_full(filenames, g_free);
			return SR_GUI_CANCELLED;
		}
		GSList* head = filenames;
		for(uint i = 0; i < itemsCount; ++i) {
			char* path = (char*)(head->data);
			// Move to the next.
			head = g_slist_next(head);
			if(path == NULL) {
				continue;
			}
			const size_t pathLen   = strlen(path);
			(*outPaths)[*outCount] = (char*)SR_GUI_MALLOC((pathLen + 1) * sizeof(char));
			if((*outPaths)[*outCount] == NULL) {
				continue;
			}
			SR_GUI_MEMCPY((*outPaths)[*outCount], path, pathLen * sizeof(char));
			(*outPaths)[*outCount][pathLen] = '\0';
			(*outCount)++;
		}
	}
	g_slist_free_full(filenames, g_free);
	g_object_unref(pickerNative);
	return SR_GUI_VALIDATED;
}

int sr_gui_ask_load_file(const char* title, const char* startDir, const char* exts, char** outPath) {
	*outPath = NULL;
	if(!gtk_init_check(NULL, NULL)) {
		return SR_GUI_CANCELLED;
	}

	GtkFileChooserNative* pickerNative = gtk_file_chooser_native_new(title, NULL, GTK_FILE_CHOOSER_ACTION_OPEN, NULL, NULL);
	GtkFileChooser* picker			   = GTK_FILE_CHOOSER(pickerNative);
	gtk_file_chooser_set_current_folder(picker, startDir);
	gtk_file_chooser_set_select_multiple(picker, FALSE);

	if(_sr_gui_add_filter_extensions(exts, picker) != 1) {
		g_object_unref(pickerNative);
		return SR_GUI_CANCELLED;
	}

	int res = gtk_native_dialog_run(GTK_NATIVE_DIALOG(pickerNative));
	if(res != GTK_RESPONSE_ACCEPT) {
		g_object_unref(pickerNative);
		return SR_GUI_CANCELLED;
	}

	char* filename		 = gtk_file_chooser_get_filename(picker);
	const size_t fileLen = strlen(filename);
	*outPath			 = SR_GUI_MALLOC((fileLen + 1) * sizeof(char));
	if(*outPath == NULL) {
		g_free(filename);
		g_object_unref(pickerNative);
		return SR_GUI_CANCELLED;
	}
	SR_GUI_MEMCPY(*outPath, filename, fileLen * sizeof(char));
	(*outPath)[fileLen] = '\0';

	g_free(filename);
	g_object_unref(pickerNative);
	return SR_GUI_VALIDATED;
}

int sr_gui_ask_save_file(const char* title, const char* startDir, const char* exts, char** outPath) {
	*outPath = NULL;
	if(!gtk_init_check(NULL, NULL)) {
		return SR_GUI_CANCELLED;
	}

	GtkFileChooserNative* pickerNative = gtk_file_chooser_native_new(title, NULL, GTK_FILE_CHOOSER_ACTION_SAVE, NULL, NULL);
	GtkFileChooser* picker			   = GTK_FILE_CHOOSER(pickerNative);
	gtk_file_chooser_set_do_overwrite_confirmation(picker, TRUE);
	gtk_file_chooser_set_create_folders(picker, TRUE);
	gtk_file_chooser_set_current_folder(picker, startDir);

	if(_sr_gui_add_filter_extensions(exts, picker) != 1) {
		g_object_unref(pickerNative);
		return SR_GUI_CANCELLED;
	}

	int res = gtk_native_dialog_run(GTK_NATIVE_DIALOG(pickerNative));
	if(res != GTK_RESPONSE_ACCEPT) {
		g_object_unref(pickerNative);
		return SR_GUI_CANCELLED;
	}

	char* filename		 = gtk_file_chooser_get_filename(picker);
	const size_t fileLen = strlen(filename);
	*outPath			 = SR_GUI_MALLOC((fileLen + 1) * sizeof(char));
	if(*outPath == NULL) {
		g_free(filename);
		g_object_unref(pickerNative);
		return SR_GUI_CANCELLED;
	}
	SR_GUI_MEMCPY(*outPath, filename, fileLen * sizeof(char));
	(*outPath)[fileLen] = '\0';
	g_free(filename);
	g_object_unref(pickerNative);
	return SR_GUI_VALIDATED;
}

int sr_gui_ask_choice(const char* title, const char* message, int level, const char* button0, const char* button1, const char* button2) {
	if(!gtk_init_check(NULL, NULL)) {
		return SR_GUI_CANCELLED;
	}

	const char* iconName = "dialog-information";
	if(level == SR_GUI_MESSAGE_LEVEL_ERROR) {
		iconName = "dialog-error";
	} else if(level == SR_GUI_MESSAGE_LEVEL_WARN) {
		iconName = "dialog-warning";
	}
	const char* buttons[] = { button0, button1, button2 };
	const int bCount = sizeof(buttons)/sizeof(buttons[0]);

	// We allow some labels to be null, and should skip them while preserving the returned index.
	// (pressing button2 should always return 2 even if button1 == NULL)
	const char* selectedButtons[3];
	int selectedIDs[3];
	int localIndex = 0;

	for(int bid = 0; bid < bCount; ++bid){
		if(buttons[bid] == NULL){
			continue;
		}
		selectedButtons[localIndex] = buttons[bid];
		selectedIDs[localIndex] = SR_GUI_BUTTON0 + bid;
		++localIndex;
	}
	for(int bid = localIndex; bid < bCount; ++bid){
		selectedButtons[localIndex] = NULL;
	}

	GtkWidget* dialog = gtk_dialog_new_with_buttons(NULL, NULL, GTK_DIALOG_MODAL, selectedButtons[0], selectedIDs[0], selectedButtons[1], selectedIDs[1], selectedButtons[2], selectedIDs[2], NULL);
	gtk_dialog_set_default_response(GTK_DIALOG(dialog), selectedIDs[0]);

	// Add title, message and icon with the proper layout.

	// Pack title and message in a vertical box.
	// Configure title.
	GtkWidget* head = gtk_label_new(title);
	char* markup	= g_markup_printf_escaped("\t<b>%s</b>", title);
	gtk_label_set_markup(GTK_LABEL(head), markup);
	g_free(markup);
	gtk_widget_set_halign(head, GTK_ALIGN_START);
	// Configure message.
	GtkWidget* label = gtk_label_new(message);
	gtk_label_set_width_chars(GTK_LABEL(label), 50);
	gtk_label_set_max_width_chars(GTK_LABEL(label), 50);
	gtk_label_set_line_wrap(GTK_LABEL(label), TRUE);
	// Pack both labels.
	GtkWidget* textArea = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
	gtk_container_add(GTK_CONTAINER(textArea), head);
	gtk_container_add(GTK_CONTAINER(textArea), label);

	// Pack the icon and texts in an horizontal box.
	GtkWidget* icon		= gtk_image_new_from_icon_name(iconName, GTK_ICON_SIZE_DIALOG);
	GtkWidget* infoArea = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
	gtk_container_add(GTK_CONTAINER(infoArea), icon);
	gtk_container_add(GTK_CONTAINER(infoArea), textArea);
	gtk_widget_set_margin_top(infoArea, 8);
	gtk_widget_set_margin_bottom(infoArea, 8);
	gtk_widget_set_margin_start(infoArea, 16);
	gtk_widget_set_margin_end(infoArea, 16);
	// Insert the top box in the dialog content area.
	GtkWidget* contentArea = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	gtk_container_add(GTK_CONTAINER(contentArea), infoArea);
	gtk_widget_set_margin_bottom(contentArea, 8);
	gtk_widget_set_margin_start(contentArea, 8);
	gtk_widget_set_margin_end(contentArea, 8);
	gtk_widget_show_all(dialog);

	// Run
	int res = gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);

	for(int bid = 0; bid < bCount; ++bid){
		if(buttons[bid] != NULL && ((SR_GUI_BUTTON0+bid) == res)){
			return SR_GUI_BUTTON0 + bid;
		}
	}
	return SR_GUI_CANCELLED;
}

int sr_gui_ask_string(const char* title, const char* message, char** result) {
	if(!gtk_init_check(NULL, NULL)) {
		return SR_GUI_CANCELLED;
	}

	GtkWidget* dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_OTHER, GTK_BUTTONS_OK_CANCEL, "%s", title);
	gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialog), "%s", message);

	GtkWidget* contentArea	= gtk_message_dialog_get_message_area(GTK_MESSAGE_DIALOG(dialog));
	GtkEntryBuffer* textStr = gtk_entry_buffer_new("Default value", 13);
	gtk_entry_buffer_set_max_length(textStr, SR_GUI_MAX_STR_SIZE);
	GtkWidget* textField = gtk_entry_new_with_buffer(textStr);
	gtk_container_add(GTK_CONTAINER(contentArea), textField);
	gtk_widget_show_all(contentArea);

	int res = gtk_dialog_run(GTK_DIALOG(dialog));
	if(res != GTK_RESPONSE_ACCEPT && res != GTK_RESPONSE_OK && res != GTK_RESPONSE_APPLY && res != GTK_RESPONSE_YES) {
		gtk_widget_destroy(dialog);
		return SR_GUI_CANCELLED;
	}

	const char* str		   = gtk_entry_buffer_get_text(textStr);
	const size_t sizeBytes = gtk_entry_buffer_get_bytes(textStr);
	*result				   = (char*)SR_GUI_MALLOC(sizeBytes + sizeof(char));
	if(*result == NULL) {
		gtk_widget_destroy(dialog);
		return SR_GUI_CANCELLED;
	}
	SR_GUI_MEMCPY(*result, str, sizeBytes);
	(*result)[sizeBytes] = '\0';
	gtk_widget_destroy(dialog);
	return SR_GUI_VALIDATED;
}

int sr_gui_ask_color(unsigned char color[3]) {
	if(!gtk_init_check(NULL, NULL)) {
		return SR_GUI_CANCELLED;
	}

	GdkRGBA colorGtk;
	colorGtk.red   = ((float)color[0]) / 255.0f;
	colorGtk.green = ((float)color[1]) / 255.0f;
	colorGtk.blue  = ((float)color[2]) / 255.0f;
	colorGtk.alpha = 1.0f;

	// Set default color.
	GtkWidget* picker = gtk_color_chooser_dialog_new(NULL, NULL);
	gtk_color_chooser_set_rgba(GTK_COLOR_CHOOSER(picker), &colorGtk);
	gtk_color_chooser_set_use_alpha(GTK_COLOR_CHOOSER(picker), 0);

	int res = gtk_dialog_run(GTK_DIALOG(picker));
	// Codes are different for different validation buttons.
	// The button label is "Select", so in doubt check all possible outcomes.
	if(res != GTK_RESPONSE_ACCEPT && res != GTK_RESPONSE_OK && res != GTK_RESPONSE_APPLY && res != GTK_RESPONSE_YES) {
		gtk_widget_destroy(picker);
		return SR_GUI_CANCELLED;
	}

	// Query the color back.
	gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(picker), &colorGtk);
	color[0] = (unsigned char)(fmin(fmax(255.0f * colorGtk.red, 0.0f), 255.0f));
	color[1] = (unsigned char)(fmin(fmax(255.0f * colorGtk.green, 0.0f), 255.0f));
	color[2] = (unsigned char)(fmin(fmax(255.0f * colorGtk.blue, 0.0f), 255.0f));
	gtk_widget_destroy(picker);
	return SR_GUI_VALIDATED;
}

int sr_gui_open_in_explorer(const char* path){
	// TODO: test
	/*
	const unsigned int strSize = strlen(path) + 1;
	char buffer = SR_GUI_MALLOC((strSize + 10) * sizeof(char));
	sprintf(buffer, "xdg-open %s", path);
	system(buffer);
	SR_GUI_FREE(buffer);
	*/
	return SR_GUI_CANCELLED;
}
