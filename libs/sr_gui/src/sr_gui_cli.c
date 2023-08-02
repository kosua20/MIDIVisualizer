

#include "sr_gui.h"
#include "sr_gui_internal.h"

#include <stdio.h>
#include <math.h>
#include <string.h>

void sr_gui_init() {
	// Nothing.
}

void sr_gui_cleanup() {
	// Nothing.
}

void _sr_gui_absorb_line_from_cin() {
	int c = 0;
	while(c != '\n' && c != EOF) {
		c = getchar();
	}
}

int _sr_gui_query_line_from_cin(char* buffer, int size) {
	// Wait for some input.
	int lenRead = 0;
	buffer[0]	= '\0';
	// Read everything there is. The first line is our result, the rest should be absorbed.
	if(fgets(buffer, size - 1, stdin) == NULL) {
		return 0;
	}
	// Remove endlines.
	char* endLine = strchr(buffer, '\n');
	if(endLine) {
		*endLine = '\0';
	}
	endLine = strchr(buffer, '\r');
	if(endLine) {
		*endLine = '\0';
	}
	lenRead = (int)strlen(buffer);
	// Read what's left in a temporary buffer.
	/*char temp[SR_GUI_MAX_STR_SIZE];
	while(fgets(temp, SR_GUI_MAX_STR_SIZE-1, stdin) != NULL){
		endLine = strchr(temp, '\n');
		if(endLine){
			*endLine = '\0';
		}
		endLine = strchr(temp, '\r');
		if(endLine){
			*endLine = '\0';
		}
		if(strlen(temp) == 0){
			break;
		}
	}*/

	return lenRead;
}

void sr_gui_show_message(const char* title, const char* message, int level) {
	char* header = "Info";
	if(level == SR_GUI_MESSAGE_LEVEL_ERROR) {
		header = "Error";
	} else if(level == SR_GUI_MESSAGE_LEVEL_WARN) {
		header = "Warning";
	}
	fprintf(stdout, "--- %s --- %s\n", header, title);
	fprintf(stdout, "%s\n", message);
	fprintf(stdout, "Press enter to continue...");
	_sr_gui_absorb_line_from_cin();
}

void sr_gui_show_notification(const char* title, const char* message) {
	fprintf(stdout, "--- %s\n", title);
	fprintf(stdout, "%s", message);
	// And play sound.
	fprintf(stdout, "\a\n");
}

int sr_gui_ask_directory(const char* title, const char* startDir, char** outPath) {
	*outPath = NULL;
	(void)startDir;

	fprintf(stdout, "--- Directory selection --- %s\n", title);
	fprintf(stdout, "Type the absolute path to a directory below and press enter to validate, or submit an empty line to cancel.\n");
	char buffer[SR_GUI_MAX_STR_SIZE];
	const int size = _sr_gui_query_line_from_cin(buffer, SR_GUI_MAX_STR_SIZE);
	if(size == 0) {
		return SR_GUI_CANCELLED;
	}
	// Copy to result, allocating enough space.
	*outPath = (char*)SR_GUI_MALLOC(sizeof(char) * (size + 1));
	if(*outPath == NULL) {
		return SR_GUI_CANCELLED;
	}
	SR_GUI_MEMCPY(*outPath, buffer, sizeof(char) * (size + 1));
	return SR_GUI_VALIDATED;
}

int sr_gui_ask_load_files(const char* title, const char* startDir, const char* exts, char*** outPaths, int* outCount) {
	*outCount = 0;
	*outPaths = NULL;
	(void)startDir;

	fprintf(stdout, "--- File(s) selection --- %s\n", title);
	if(exts && strlen(exts) > 0) {
		fprintf(stdout, "Allowed extensions: %s\n", exts);
	}
	fprintf(stdout, "Type the absolute path to a file on disk below and press enter. You can then input another path, or press enter again to finish. Submit an empty line to cancel\n");

	char* tempPaths[SR_GUI_MAX_STR_SIZE];
	int size   = 0;
	int pathId = 0;
	char buffer[SR_GUI_MAX_STR_SIZE];
	while((size = _sr_gui_query_line_from_cin(buffer, SR_GUI_MAX_STR_SIZE)) != 0) {
		// Copy the buffer.
		tempPaths[pathId] = (char*)SR_GUI_MALLOC(sizeof(char) * (size + 1));
		if(tempPaths[pathId] == NULL) {
			continue;
		}
		SR_GUI_MEMCPY(tempPaths[pathId], buffer, sizeof(char) * (size + 1));

		++pathId;
		if(pathId >= SR_GUI_MAX_STR_SIZE) {
			break;
		}
	}

	*outCount = pathId;
	if(pathId == 0) {
		// No tempPaths allocation happened.
		return SR_GUI_CANCELLED;
	}

	*outPaths = (char**)SR_GUI_MALLOC(sizeof(char*) * pathId);
	if(*outPaths == NULL) {
		// Free tempPaths string.
		for(int i = 0; i < pathId; ++i) {
			SR_GUI_FREE(tempPaths[i]);
		}
		return SR_GUI_CANCELLED;
	}
	SR_GUI_MEMCPY(*outPaths, tempPaths, sizeof(char*) * pathId);
	return SR_GUI_VALIDATED;
}


int sr_gui_ask_load_file(const char* title, const char* startDir, const char* exts, char** outPath) {
	*outPath = NULL;
	(void)startDir;

	fprintf(stdout, "--- File selection --- %s\n", title);
	if(exts && strlen(exts) > 0) {
		fprintf(stdout, "Allowed extensions: %s\n", exts);
	}
	fprintf(stdout, "Type the absolute path to a file on disk below and press enter to finish. Submit an empty line to cancel\n");

	char buffer[SR_GUI_MAX_STR_SIZE];
	const int size = _sr_gui_query_line_from_cin(buffer, SR_GUI_MAX_STR_SIZE);
	if(size == 0) {
		return SR_GUI_CANCELLED;
	}
	// Copy to result, allocating enough space.
	*outPath = (char*)SR_GUI_MALLOC(sizeof(char) * (size + 1));
	if(*outPath == NULL) {
		return SR_GUI_CANCELLED;
	}
	SR_GUI_MEMCPY(*outPath, buffer, sizeof(char) * (size + 1));
	
	return SR_GUI_VALIDATED;
}

int sr_gui_ask_save_file(const char* title, const char* startDir, const char* exts, char** outPath) {
	*outPath = NULL;
	(void)startDir;

	fprintf(stdout, "--- Output selection --- %s\n", title);
	if(exts && strlen(exts) > 0) {
		fprintf(stdout, "Allowed extensions: %s\n", exts);
	}
	fprintf(stdout, "Type the absolute path to an output file below and press enter to validate, or submit an empty to cancel.\n");

	char buffer[SR_GUI_MAX_STR_SIZE];
	const int size = _sr_gui_query_line_from_cin(buffer, SR_GUI_MAX_STR_SIZE);
	if(size == 0) {
		return SR_GUI_CANCELLED;
	}
	// Copy to result, allocating enough space.
	*outPath = (char*)SR_GUI_MALLOC(sizeof(char) * (size + 1));
	if(*outPath == NULL) {
		return SR_GUI_CANCELLED;
	}
	SR_GUI_MEMCPY(*outPath, buffer, sizeof(char) * (size + 1));

	return SR_GUI_VALIDATED;
}

int sr_gui_ask_choice(const char* title, const char* message, int level, const char* button0, const char* button1, const char* button2) {
	char* header = "Info";
	if(level == SR_GUI_MESSAGE_LEVEL_ERROR) {
		header = "Error";
	} else if(level == SR_GUI_MESSAGE_LEVEL_WARN) {
		header = "Warning";
	}
	const char* buttons[] = { button0, button1, button2 };

	fprintf(stdout, "--- %s --- %s\n", header, title);
	fprintf(stdout, "%s\n", message);
	
	int following = 0;
	const int bCount = sizeof(buttons)/sizeof(buttons[0]);
	for(int bid = 0; bid < bCount; ++bid){
		if(buttons[bid] == NULL){
			continue;
		}
		if(following){
			fprintf(stdout, "--- ");
		}

		fprintf(stdout, "(%d) %s ", (bid + 1), buttons[bid]);
		following = 1;
	}
	fprintf(stdout, "\n");
	fprintf(stdout, "Type the number corresponding to your selected option, and press enter to validate.\n");

	char buffer[64];
	const int size = _sr_gui_query_line_from_cin(buffer, 64);
	if(size == 0) {
		return SR_GUI_CANCELLED;
	}
	int button = 0;
#ifdef _WIN32
	int res = sscanf_s(buffer, "%d", &button);
#else
	int res = sscanf(buffer, "%d", &button);
#endif
	if(res < 1 || res == EOF) {
		return SR_GUI_CANCELLED;
	}

	for(int bid = 0; bid < bCount; ++bid){
		if(buttons[bid] != NULL && ((bid+1) == button)){
			return SR_GUI_BUTTON0 + bid;
		}
	}
	return SR_GUI_CANCELLED;
}

int sr_gui_ask_string(const char* title, const char* message, char** result) {
	fprintf(stdout, "--- %s\n", title);
	fprintf(stdout, "%s\n", message);
	fprintf(stdout, "Type your text, and press enter to validate, or submit an empty line to cancel.\n");
	char buffer[SR_GUI_MAX_STR_SIZE];
	const int size = _sr_gui_query_line_from_cin(buffer, SR_GUI_MAX_STR_SIZE);
	if(size == 0) {
		return SR_GUI_CANCELLED;
	}
	*result = (char*)SR_GUI_MALLOC(sizeof(char) * (size + 1));
	if(*result == NULL) {
		return SR_GUI_CANCELLED;
	}
	SR_GUI_MEMCPY(*result, buffer, sizeof(char) * (size + 1));
	return SR_GUI_VALIDATED;
}

int sr_gui_ask_color(unsigned char color[3]) {
	fprintf(stdout, "--- Color selection\n");
	fprintf(stdout, "Current RGB color is: %d %d %d\n", (int)color[0], (int)color[1], (int)color[2]);
	fprintf(stdout, "Type the three values separated by space, and press enter to validate, or submit an empty line to cancel.\n");
	char buffer[64];
	const int size = _sr_gui_query_line_from_cin(buffer, 64);
	if(size == 0) {
		return SR_GUI_CANCELLED;
	}
	int rgb[3];
#ifdef _WIN32
	int res = sscanf_s(buffer, "%d %d %d", &rgb[0], &rgb[1], &rgb[2]);
#else
	int res = sscanf(buffer, "%d %d %d", &rgb[0], &rgb[1], &rgb[2]);
#endif
	if(res < 3 || res == EOF) {
		return SR_GUI_CANCELLED;
	}
	for(int i = 0; i < 3; ++i) {
		color[i] = (unsigned char)(fmin(fmax(rgb[i], 0), 255));
	}
	return SR_GUI_VALIDATED;
}

int sr_gui_open_in_explorer(const char* path){
	fprintf(stdout, "* Path: %s\n", path);
	fprintf(stdout, "Press enter to continue...");
	_sr_gui_absorb_line_from_cin();
	return SR_GUI_VALIDATED;
}
