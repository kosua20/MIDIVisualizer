# sr_gui

This is a simple wrapper around graphical user interface (GUI) frameworks on Windows, macOS and Linux, along with a platform-independent command-line version. It respectively interacts with:

* the common dialog and controls library on Windows (Windows 10 required for notifications)
* the AppKit framework on macOS
* the GTK+3 library on Linux
* only `stdout`/`stdin` for the CLI version

A basic example in C can be found in the `example` directory.

Common files: `include/sr_gui.h`, `src/sr_gui_internal.h`.

## Dependencies

Each platform implementation can be found in a separate file.

* On Windows, this should link against `User2`, `Comdlg32`, `Comctl32`, `Shell32` and `runtimeobject`. Implementation is in `src/sr_gui_win.cpp` (C++). **Due to a system limitation**, notifications will only work after the first run with a given app identifier (see [Customization](#customization)).
* On macOS, this should link against `AppKit.framework`. Implementation is in `src/sr_gui_mac.m` (Objective-C). 
* On Linux, this should link against `gtk+-3.0` and `libnotify`. Implementation is in `src/sr_gui_lin.c` (C).
* The command line version has no dependencies. Implementation is in `src/sr_gui_cli.c` (C).

## Usage

You need to initialize and cleanup the library by respectively calling

    void sr_gui_init();

before doing any other call, and

    void sr_gui_cleanup();

at the end of your program.

All function below will display some GUI and block until the user validates or cancels (except for notifications). All strings arguments below should be null terminated. Conversion to wide strings on Windows is handled internally.

### Displaying a message dialog

    void sr_gui_show_message(const char* title, const char* message, int level);

Display a modal dialog with the given title and message, along with a unique OK button. The importance level of the message can be selected among `SR_GUI_MESSAGE_LEVEL_INFO`, `SR_GUI_MESSAGE_LEVEL_WARN` and `SR_GUI_MESSAGE_LEVEL_ERROR`. The displayed dialog will pick its icon accordingly.

### Displaying a desktop notification

    void sr_gui_show_notification(const char* title, const char* message);

This will display a passive notification with the given title and message via the desktop notification system. On most platforms, an app identifier has to be provided for notifications to work (see [Customization](#customization)).

### Multiple choice query

    int sr_gui_ask_choice(const char* title, const char* message, int level, const char* button0, const char* button1, const char* button2);

Display a modal dialog with the given title and message, along with three buttons with custom labels. Returns either `SR_GUI_BUTTON0`, `SR_GUI_BUTTON1` or `SR_GUI_BUTTON2` based on the button pressed. If the system interface allows for cancellation, `SR_GUI_CANCELLED` might also be returned when the user does so. Passing `NULL` as a label will hide the corresponding button.

### String query

    int sr_gui_ask_string(const char* title, const char* message, char** result);

Display a modal dialog with the given title and message, along with a text field the user can type in. Returns `SR_GUI_VALIDATED` if the user validated, or `SR_GUI_CANCELLED` if the dialog was cancelled. If validated, result will point to a null-terminated string allocated on the heap internally (see [Customization](#customization)).

### Color query

    int sr_gui_ask_color(unsigned char color[3]);

Display a color picker initialized with the values passed as input. When returning the color parameter will contain the selected RGB color with values in [0,255]. Returns `SR_GUI_VALIDATED` if the user validated, or `SR_GUI_CANCELLED` if the dialog was cancelled.

### File and directory queries

For now, only local files and folders can be selected though these functions.
The three functions return `SR_GUI_VALIDATED` if the user validated, or `SR_GUI_CANCELLED` if the query was cancelled. All results are stored as null-terminated strings allocated on the heap internally (see [Customization](#customization)).

#### Selecting existing files

    int sr_gui_ask_load_files(const char* title, const char* startDir, const char* exts, char*** outPaths, int* outCount);

Display a picker dialog with the given title to select one or multiple files. The directory to be initially presented to the user can be specified. Specific file extensions can be requested by passing a comma-separated list (for instance `"png,jpg,bmp"`). The number of selected items and their paths will be populated once the user validates.

    int sr_gui_ask_load_file(const char* title, const char* startDir, const char* exts, char** outPath);

Similar function, but only one item can be selected and will be returned once the user validates.

#### Selecting an existing directory

    int sr_gui_ask_directory(const char* title, const char* startDir, char** outPath);

Display a picker dialog with the given title to select a directory. The directory to be initially presented to the user can be specified. The selected directory path will be populated once the user validates.

#### Selecting a destination file

    int sr_gui_ask_save_file(const char* title, const char* startDir, const char* exts, char** outpath);

Display a picker dialog with the given title to select an output file (existing or to be created). The directory to be initially presented to the user can be specified. Specific file extensions can be requested by passing a comma-separated list (for instance `"png,jpg,bmp"`). The selected file path will be populated once the user validates.

### System interactions

    int sr_gui_open_in_explorer(const char* path);

Open the directory containing the given path in the system file browser (Explorer, Finder, Nautilus,...), and if possible select the file.

    int sr_gui_open_in_default_app(const char* path);

Open the file in the application assigned to its filetype by default, if it exists.

    int sr_gui_open_in_browser(const char* url);

Open the URL in a tab of the default web browser.

    int sr_gui_get_app_data_path(char** outPath);

Query the path to the application data directory of the user, where it is customary for applications to store their configuration in a distinct sub-directory. There is no guarantee that the returned path exists on disk.

## Customization

A few options can be adjusted when compiling `sr_gui`, by editing the `src/sr_gui_internal.h` header file.

`sr_gui` internally allocate a few strings (for labels and paths). By default, it uses the standard `malloc` and `free` but these can be overriden by defining `SR_GUI_MALLOC(S)` and `SR_GUI_FREE(S)`.

On Windows and Linux, a name has to be defined for any application that wants to display notifications. Windows also needs a URI identifier. They default to `"SR GUI App"` and `"com.sr.sr_gui"`, but can be overriden by defining `SR_GUI_APP_NAME` and `SR_GUI_APP_ID` to the desired strings.


## Limitations

The appearance of the string query dialog is lacking compared to dialogs in native apps.  
The color picker doesn't support an alpha channel because the Windows color picker only handles RGB.  
The command-line version is lacking proper buffered processing if the user inputs more than 1023 characters.

