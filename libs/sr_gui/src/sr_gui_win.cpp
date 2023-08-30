#if defined(__cplusplus)

#	include "sr_gui.h"
#	include "sr_gui_internal.h"
#	include <stdio.h>
#	include <cstdint>
#	include <wchar.h>
#	include <windows.h>
#	include <shobjidl.h>
#	include <psapi.h>
#	include <propvarutil.h>
#	include <Propkey.h>

#	include <wrl.h>
#	include <windows.ui.notifications.h>
#	include <NotificationActivationCallback.h>
#	include <appmodel.h>
#	include <wrl/wrappers/corewrappers.h>
#	include <string>
#   include <Shlobj.h>

#	pragma comment(linker, "\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

using namespace ABI::Windows::UI::Notifications;
using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;
using namespace ABI::Windows::Data::Xml::Dom;

void sr_gui_init() {
	HRESULT res = CoInitializeEx(NULL, ::COINIT_APARTMENTTHREADED | ::COINIT_DISABLE_OLE1DDE);
	(void)res;
}

void sr_gui_cleanup() {
	CoUninitialize();
}

// Transfer output ownership to the caller.
wchar_t* _sr_gui_widen_string(const char* str) {
	if(str == NULL){
		return NULL;
	}
	const int sizeWide = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	wchar_t* output	   = (wchar_t*)SR_GUI_MALLOC(sizeWide * sizeof(wchar_t));
	MultiByteToWideChar(CP_UTF8, 0, str, -1, output, sizeWide);
	return output;
}

// Transfer output ownership to the caller.
char* _sr_gui_narrow_string(const wchar_t* wstr) {
	if(wstr == NULL) {
		return NULL;
	}
	const int sizeNarrow = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* output		 = (char*)SR_GUI_MALLOC(sizeNarrow * sizeof(char));
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, output, sizeNarrow, NULL, NULL);
	return output;
}

void sr_gui_show_message(const char* title, const char* message, int level) {

	wchar_t* titleW	  = _sr_gui_widen_string(title);
	wchar_t* messageW = _sr_gui_widen_string(message);

	PCWSTR icon = TD_INFORMATION_ICON;
	if(level == SR_GUI_MESSAGE_LEVEL_ERROR) {
		icon = TD_ERROR_ICON;
	} else if(level == SR_GUI_MESSAGE_LEVEL_WARN) {
		icon = TD_WARNING_ICON;
	}

	HRESULT res = TaskDialog(NULL, NULL, NULL, titleW, messageW, TDCBF_OK_BUTTON, icon, NULL);
	(void)res;

	SR_GUI_FREE(titleW);
	SR_GUI_FREE(messageW);
}

void sr_gui_show_notification(const char* title, const char* message) {
	// Generate wide char constant string from a char one.
#	define LTEXTC(M) L##M
#	define LTEXT(M) LTEXTC(M)

	// AppID size is limited.
	if(wcslen(LTEXT(SR_GUI_APP_ID)) > SCHAR_MAX) {
		return;
	}

	// Create shortcut
	wchar_t shortcutPath[MAX_PATH];
	DWORD charWritten = GetEnvironmentVariable(L"APPDATA", shortcutPath, MAX_PATH);
	if(charWritten <= 0) {
		return;
	}
	errno_t concatError = wcscat_s(shortcutPath, ARRAYSIZE(shortcutPath), L"\\Microsoft\\Windows\\Start Menu\\Programs\\" LTEXT(SR_GUI_APP_NAME) ".lnk");
	if(concatError != 0) {
		return;
	}
	// Check that the shortcut doesn't already exist.
	DWORD attributes = GetFileAttributes(shortcutPath);
	if(!(attributes < 0xFFFFFFF)) {
		// Get app path.
		wchar_t exePath[MAX_PATH];
		charWritten = GetModuleFileNameEx(GetCurrentProcess(), nullptr, exePath, ARRAYSIZE(exePath));
		if(charWritten <= 0) {
			return;
		}
		// Create the shortcut link.
		ComPtr<IShellLink> shellLink;
		HRESULT res = CoCreateInstance(CLSID_ShellLink, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&shellLink));
		if(FAILED(res)) {
			return;
		}
		res = shellLink->SetPath(exePath);
		if(FAILED(res)) {
			shellLink->Release();
			return;
		}
		res = shellLink->SetArguments(L"");
		if(FAILED(res)) {
			shellLink->Release();
			return;
		}
		ComPtr<IPropertyStore> propertyStore;
		res = shellLink.As(&propertyStore);
		if(FAILED(res)) {
			shellLink->Release();
			return;
		}
		PROPVARIANT appIdPropVar;
		res = InitPropVariantFromString(LTEXT(SR_GUI_APP_ID), &appIdPropVar);
		if(FAILED(res)) {
			shellLink->Release();
			return;
		}
		res = propertyStore->SetValue(PKEY_AppUserModel_ID, appIdPropVar);
		if(FAILED(res)) {
			PropVariantClear(&appIdPropVar);
			shellLink->Release();
			return;
		}
		res = propertyStore->Commit();
		PropVariantClear(&appIdPropVar);
		if(FAILED(res)) {
			shellLink->Release();
			return;
		}
		ComPtr<IPersistFile> persistFile;
		res = shellLink.As(&persistFile);
		if(FAILED(res)) {
			shellLink->Release();
			return;
		}
		res = persistFile->Save(shortcutPath, TRUE);
		shellLink->Release();
		if(FAILED(res)) {
			return;
		}
	}

	// Register App ID.
	SetCurrentProcessExplicitAppUserModelID(LTEXT(SR_GUI_APP_ID));

	// Create the notification manager.
	ComPtr<IToastNotificationManagerStatics> toastStatics;
	HRESULT res = Windows::Foundation::GetActivationFactory(HStringReference(RuntimeClass_Windows_UI_Notifications_ToastNotificationManager).Get(), &toastStatics);
	if(FAILED(res)) {
		return;
	}

	ComPtr<IToastNotifier> notifier;
	toastStatics->CreateToastNotifierWithId(HStringReference(LTEXT(SR_GUI_APP_ID)).Get(), &notifier);

	// Create XML document.
	ComPtr<IXmlDocument> templateXml;
	res = Windows::Foundation::ActivateInstance(HStringReference(RuntimeClass_Windows_Data_Xml_Dom_XmlDocument).Get(), &templateXml);
	if(FAILED(res)) {
		toastStatics->Release();
		notifier->Release();
		return;
	}
	ComPtr<IXmlDocumentIO> templateIO;
	res = templateXml.As(&templateIO);
	if(FAILED(res)) {
		templateXml->Release();
		toastStatics->Release();
		return;
	}

	// Create the notification template
	WCHAR* templateBase			   = L"<toast><visual><binding template='ToastGeneric'><text>%s</text><text>%s</text></binding></visual></toast>";
	WCHAR* titleW				   = _sr_gui_widen_string(title);
	WCHAR* messageW				   = _sr_gui_widen_string(message);
	const size_t templateTotalSize = wcslen(templateBase) + wcslen(titleW) + wcslen(messageW) + 1;
	WCHAR* templateStr			   = (WCHAR*)SR_GUI_MALLOC(templateTotalSize * sizeof(WCHAR));
	if(templateStr == NULL) {
		toastStatics->Release();
		notifier->Release();
		SR_GUI_FREE(titleW);
		SR_GUI_FREE(messageW);
		return;
	}
	// Generate template string.
	wsprintf(templateStr, templateBase, titleW, messageW);
	SR_GUI_FREE(titleW);
	SR_GUI_FREE(messageW);

	res = templateIO->LoadXml(HStringReference(templateStr).Get());
	SR_GUI_FREE(templateStr);
	if(FAILED(res)) {
		templateXml->Release();
		toastStatics->Release();
		return;
	}
	// Create the notification
	ComPtr<IToastNotificationFactory> factory;
	res = Windows::Foundation::GetActivationFactory(HStringReference(RuntimeClass_Windows_UI_Notifications_ToastNotification).Get(), &factory);
	if(FAILED(res)) {
		templateXml->Release();
		toastStatics->Release();
		return;
	}
	IToastNotification* notification;
	res = factory->CreateToastNotification(templateXml.Get(), &notification);
	if(FAILED(res)) {
		factory->Release();
		templateXml->Release();
		toastStatics->Release();
		return;
	}
	res = notifier->Show(notification);

	factory->Release();
}

bool _sr_gui_add_filter_extensions(const char* exts, IFileDialog* dialog) {
	if(!exts) {
		COMDLG_FILTERSPEC filter;
		filter.pszSpec = L"*.*";
		filter.pszName = L"*.*";
		dialog->SetFileTypes(1, &filter);
		return true;
	}
	size_t extsLen = strlen(exts);
	if(extsLen == 0) {
		COMDLG_FILTERSPEC filter;
		filter.pszSpec = L"*.*";
		filter.pszName = L"*.*";
		dialog->SetFileTypes(1, &filter);
		return true;
	}
	// Count extensions.
	int count = 1;
	for(size_t cid = 0; cid < extsLen; ++cid) {
		if(exts[cid] == ',') {
			++count;
		}
	}
	// Add wildcard at the end.
	++count;

	COMDLG_FILTERSPEC* filterList = (COMDLG_FILTERSPEC*)SR_GUI_MALLOC(count * sizeof(COMDLG_FILTERSPEC));
	if(filterList == nullptr) {
		return false;
	}
	char extBuffer[SR_GUI_MAX_STR_SIZE];
	extBuffer[0] = '*';
	extBuffer[1] = '.';

	size_t cid = 0;

	for(size_t fid = 0; fid < size_t(int(count) - 1); ++fid) {
		// After the wildcard.
		size_t bid = 2;
		while(cid < extsLen && exts[cid] != ',') {
			extBuffer[bid] = exts[cid];
			++bid;
			++cid;
		}
		extBuffer[bid] = '\0';
		// ...
		filterList[fid].pszName = _sr_gui_widen_string(&extBuffer[2]);
		filterList[fid].pszSpec = _sr_gui_widen_string(extBuffer);
		// Skip comma.
		++cid;
		if(cid >= extsLen) {
			break;
		}
	}
	filterList[count - 1].pszSpec = L"*.*";
	filterList[count - 1].pszName = L"*.*";
	dialog->SetFileTypes(count, filterList);

	for(size_t fid = 0; fid < size_t(int(count) - 1); ++fid) {
		SR_GUI_FREE((void*)(filterList[fid].pszSpec));
		SR_GUI_FREE((void*)(filterList[fid].pszName));
	}
	SR_GUI_FREE(filterList);
	return true;
}

bool _sr_gui_add_default_path(const char* path, IFileDialog* dialog) {
	if(!path) {
		return true;
	}
	if(strlen(path) == 0) {
		return true;
	}
	// Skip local paths, unsupported by shell item.
	if(strcmp(path, "./") == 0 || path[0] == '.') {
		return true;
	}
	WCHAR* startDirW = _sr_gui_widen_string(path);
	IShellItem* pathShell;
	HRESULT res = SHCreateItemFromParsingName(startDirW, NULL, IID_PPV_ARGS(&pathShell));
	SR_GUI_FREE(startDirW);

	if(!SUCCEEDED(res) && (res != HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND)) && (res != HRESULT_FROM_WIN32(ERROR_INVALID_DRIVE))) {
		return false;
	}
	dialog->SetFolder(pathShell);
	return true;
}

int sr_gui_ask_directory(const char* title, const char* startDir, char** outPath) {
	*outPath = nullptr;

	IFileOpenDialog* dialog = NULL;
	HRESULT res				= CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_PPV_ARGS(&dialog));
	if(!SUCCEEDED(res)) {
		return SR_GUI_CANCELLED;
	}
	// Set default path.
	bool success = _sr_gui_add_default_path(startDir, dialog);
	if(!success) {
		dialog->Release();
		return SR_GUI_CANCELLED;
	}

	// Set title.
	WCHAR* titleW = _sr_gui_widen_string(title);
	res			  = dialog->SetTitle(titleW);
	SR_GUI_FREE(titleW);
	if(!SUCCEEDED(res)) {
		dialog->Release();
		return SR_GUI_CANCELLED;
	}

	// Options: pick only directories.
	DWORD options = 0;
	res			  = dialog->GetOptions(&options);
	if(!SUCCEEDED(res)) {
		dialog->Release();
		return SR_GUI_CANCELLED;
	}
	res = dialog->SetOptions(options | FOS_PICKFOLDERS);
	if(!SUCCEEDED(res)) {
		dialog->Release();
		return SR_GUI_CANCELLED;
	}

	// Present
	res = dialog->Show(nullptr);
	if(!SUCCEEDED(res)) {
		dialog->Release();
		return SR_GUI_CANCELLED;
	}

	IShellItem* selectedItem = nullptr;
	res						 = dialog->GetResult(&selectedItem);
	dialog->Release();
	if(!SUCCEEDED(res)) {
		return SR_GUI_CANCELLED;
	}

	WCHAR* selectedPath = nullptr;
	res					= selectedItem->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &selectedPath); // Shouldn't this be SIGDN_FILESYSPATH
	selectedItem->Release();
	if(!SUCCEEDED(res)) {
		return SR_GUI_CANCELLED;
	}
	*outPath = _sr_gui_narrow_string(selectedPath);
	CoTaskMemFree(selectedPath);
	return SR_GUI_VALIDATED;
}

int sr_gui_ask_load_files(const char* title, const char* startDir, const char* exts, char*** outPaths, int* outCount) {
	*outCount = 0;
	*outPaths = NULL;

	IFileOpenDialog* dialog = NULL;
	HRESULT res				= CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_PPV_ARGS(&dialog));
	if(!SUCCEEDED(res)) {
		return SR_GUI_CANCELLED;
	}
	// Set default path.
	bool success = _sr_gui_add_default_path(startDir, dialog);
	if(!success) {
		dialog->Release();
		return SR_GUI_CANCELLED;
	}

	// Set extension filter.
	success = _sr_gui_add_filter_extensions(exts, dialog);
	if(!success) {
		dialog->Release();
		return SR_GUI_CANCELLED;
	}

	// Set title.
	WCHAR* titleW = _sr_gui_widen_string(title);
	res			  = dialog->SetTitle(titleW);
	SR_GUI_FREE(titleW);
	if(!SUCCEEDED(res)) {
		dialog->Release();
		return SR_GUI_CANCELLED;
	}
	// Options: multiple selection allowed.
	DWORD options = 0;
	res			  = dialog->GetOptions(&options);
	if(!SUCCEEDED(res)) {
		dialog->Release();
		return SR_GUI_CANCELLED;
	}
	res = dialog->SetOptions(options | FOS_ALLOWMULTISELECT);
	if(!SUCCEEDED(res)) {
		dialog->Release();
		return SR_GUI_CANCELLED;
	}

	// Present
	res = dialog->Show(nullptr);
	if(!SUCCEEDED(res)) {
		dialog->Release();
		return SR_GUI_CANCELLED;
	}
	IShellItemArray* selectedItems = nullptr;
	res							   = dialog->GetResults(&selectedItems);
	dialog->Release();
	if(!SUCCEEDED(res)) {
		return SR_GUI_CANCELLED;
	}

	DWORD itemsCount;
	res		  = selectedItems->GetCount(&itemsCount);
	*outCount = 0;
	if(itemsCount > 0) {
		*outPaths = (char**)SR_GUI_MALLOC(sizeof(char*) * itemsCount);
		if(*outPaths == nullptr) {
			selectedItems->Release();
			return SR_GUI_CANCELLED;
		}

		for(int i = 0; i < int(itemsCount); ++i) {
			IShellItem* selectedItem = nullptr;
			selectedItems->GetItemAt(i, &selectedItem);
			// Skip non-file elements.
			SFGAOF attributes;
			res = selectedItem->GetAttributes(SFGAO_FILESYSTEM, &attributes);
			if(!(attributes & SFGAO_FILESYSTEM)) {
				continue;
			}

			WCHAR* selectedPath = nullptr;
			res					= selectedItem->GetDisplayName(SIGDN_FILESYSPATH, &selectedPath);
			char* path			= _sr_gui_narrow_string(selectedPath);
			CoTaskMemFree(selectedPath);
			if(path == nullptr) {
				continue;
			}
			(*outPaths)[*outCount] = path;
			(*outCount)++;
		}
	}
	selectedItems->Release();

	return SR_GUI_VALIDATED;
}

int sr_gui_ask_load_file(const char* title, const char* startDir, const char* exts, char** outPath) {
	*outPath = NULL;

	IFileOpenDialog* dialog = NULL;
	HRESULT res				= CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_PPV_ARGS(&dialog));
	if(!SUCCEEDED(res)) {
		return SR_GUI_CANCELLED;
	}
	// Set default path.
	bool success = _sr_gui_add_default_path(startDir, dialog);
	if(!success) {
		dialog->Release();
		return SR_GUI_CANCELLED;
	}

	// Set extension filter.
	success = _sr_gui_add_filter_extensions(exts, dialog);
	if(!success) {
		dialog->Release();
		return SR_GUI_CANCELLED;
	}

	// Set title.
	WCHAR* titleW = _sr_gui_widen_string(title);
	res			  = dialog->SetTitle(titleW);
	SR_GUI_FREE(titleW);
	if(!SUCCEEDED(res)) {
		dialog->Release();
		return SR_GUI_CANCELLED;
	}

	// Present
	res = dialog->Show(nullptr);
	if(!SUCCEEDED(res)) {
		dialog->Release();
		return SR_GUI_CANCELLED;
	}
	
	IShellItem* selectedItem = nullptr;
	res						 = dialog->GetResult(&selectedItem);
	dialog->Release();
	if(!SUCCEEDED(res)) {
		return SR_GUI_CANCELLED;
	}

	WCHAR* selectedPath = nullptr;
	res					= selectedItem->GetDisplayName(SIGDN_FILESYSPATH, &selectedPath);
	selectedItem->Release();
	if(!SUCCEEDED(res)) {
		return SR_GUI_CANCELLED;
	}
	*outPath = _sr_gui_narrow_string(selectedPath);
	CoTaskMemFree(selectedPath);
	return SR_GUI_VALIDATED;
}

int sr_gui_ask_save_file(const char* title, const char* startDir, const char* exts, char** outPath) {
	*outPath = NULL;

	IFileSaveDialog* dialog = NULL;
	HRESULT res				= CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL, IID_PPV_ARGS(&dialog));
	if(!SUCCEEDED(res)) {
		return SR_GUI_CANCELLED;
	}
	// Set default path.
	bool success = _sr_gui_add_default_path(startDir, dialog);
	if(!success) {
		dialog->Release();
		return SR_GUI_CANCELLED;
	}

	// Set extension filter.
	success = _sr_gui_add_filter_extensions(exts, dialog);
	if(!success) {
		dialog->Release();
		return SR_GUI_CANCELLED;
	}

	// Set title.
	WCHAR* titleW = _sr_gui_widen_string(title);
	res			  = dialog->SetTitle(titleW);
	SR_GUI_FREE(titleW);
	if(!SUCCEEDED(res)) {
		dialog->Release();
		return SR_GUI_CANCELLED;
	}

	// Present
	res = dialog->Show(nullptr);
	if(!SUCCEEDED(res)) {
		dialog->Release();
		return SR_GUI_CANCELLED;
	}

	IShellItem* selectedItem = nullptr;
	res						 = dialog->GetResult(&selectedItem);
	dialog->Release();
	if(!SUCCEEDED(res)) {
		return SR_GUI_CANCELLED;
	}

	WCHAR* selectedPath = nullptr;
	res					= selectedItem->GetDisplayName(SIGDN_FILESYSPATH, &selectedPath);
	selectedItem->Release();
	if(!SUCCEEDED(res)) {
		return SR_GUI_CANCELLED;
	}
	*outPath = _sr_gui_narrow_string(selectedPath);
	CoTaskMemFree(selectedPath);
	return SR_GUI_VALIDATED;
}

int sr_gui_ask_choice(const char* title, const char* message, int level, const char* button0, const char* button1, const char* button2) {
	WCHAR* titleW	= _sr_gui_widen_string(title);
	WCHAR* messageW = _sr_gui_widen_string(message);
	WCHAR* button0W = _sr_gui_widen_string(button0);
	WCHAR* button1W = _sr_gui_widen_string(button1);
	WCHAR* button2W = _sr_gui_widen_string(button2);

	WCHAR* buttons[] = {button0W, button1W, button2W};
	const int bCount = sizeof(buttons)/sizeof(buttons[0]);

	TASKDIALOG_BUTTON winButtons[3];
	int localIndex = 0;

	// We allow some labels to be null, and should skip them while preserving the returned index.
	// (pressing button2 should always return 2 even if button1 == NULL)
	for(int bid = 0; bid < bCount; ++bid){
		if(buttons[bid] == NULL){
			continue;
		}
		winButtons[localIndex] = {SR_GUI_BUTTON0 + bid, buttons[bid]};
		++localIndex;
	}

	PCWSTR icon = TD_INFORMATION_ICON;
	if(level == SR_GUI_MESSAGE_LEVEL_ERROR) {
		icon = TD_ERROR_ICON;
	} else if(level == SR_GUI_MESSAGE_LEVEL_WARN) {
		icon = TD_WARNING_ICON;
	}

	TASKDIALOGCONFIG dialog	  = {0};
	dialog.cbSize			  = sizeof(TASKDIALOGCONFIG);
	dialog.hInstance		  = NULL;
	dialog.pszMainIcon		  = icon;
	dialog.pszMainInstruction = titleW;
	dialog.pszContent		  = messageW;
	dialog.pButtons			  = winButtons;
	dialog.cButtons			  = localIndex;
	dialog.nDefaultButton	  = winButtons[0].nButtonID;

	int button	= -1;
	HRESULT res = TaskDialogIndirect(&dialog, &button, NULL, NULL);

	// Cleanup.
	SR_GUI_FREE(titleW);
	SR_GUI_FREE(messageW);
	SR_GUI_FREE(button0W);
	SR_GUI_FREE(button1W);
	SR_GUI_FREE(button2W);

	// Result
	if(FAILED(res)) {
		return SR_GUI_CANCELLED;
	}

	for(int bid = 0; bid < bCount; ++bid){
		if(buttons[bid] != NULL && ((SR_GUI_BUTTON0 + bid) == button)) {
			return SR_GUI_BUTTON0 + bid;
		}
	}

	return SR_GUI_CANCELLED;
}

struct _sr_gui_message_callback_data {
	HWND textField;
	HFONT font;
	char** content;
};

HRESULT _sr_gui_message_callback(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, LONG_PTR lpRefData) {
	if(msg == TDN_CREATED) {
		// Retrieve window size.
		int w = 366;
		int h = 186;
		RECT winSize;
		if(GetWindowRect(hwnd, &winSize)) {
			w = abs(winSize.right - winSize.left);
			h = abs(winSize.bottom - winSize.top);
		}
		_sr_gui_message_callback_data* data = (_sr_gui_message_callback_data*)(lpRefData);
		// Add text field.
		data->textField = CreateWindow(L"EDIT", NULL, WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_LEFT | WS_TABSTOP, 10, h - 110, w - 40, 18, hwnd, (HMENU)5, NULL, NULL);
		// Field default value and font.
		const WCHAR* dfltName = L"Default string";
		SendMessage(data->textField, WM_SETTEXT, NULL, (LPARAM)dfltName);
		SendMessage(data->textField, EM_SETSEL, 0, -1);

		HDC context	   = GetDC(hwnd);
		INT fontHeight = MulDiv(10, GetDeviceCaps(context, LOGPIXELSY), 72);
		data->font	   = CreateFont(fontHeight, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("MS Shell Dlg"));
		SendMessage(data->textField, WM_SETFONT, (WPARAM)(data->font), TRUE);
		// Make sure this is focused.
		SetFocus(data->textField);
	} else if(msg == TDN_DESTROYED) {
		// Copy back the string result in our data.
		_sr_gui_message_callback_data* data = (_sr_gui_message_callback_data*)(lpRefData);
		WCHAR res[SR_GUI_MAX_STR_SIZE];
		res[0] = '\0';
		SendMessage(data->textField, WM_GETTEXT, SR_GUI_MAX_STR_SIZE, (LPARAM)res);
		*(data->content) = _sr_gui_narrow_string(res);
	}
	return S_OK;
}

int sr_gui_ask_string(const char* title, const char* message, char** result) {

	// Message string.
	WCHAR* messageWTemp		 = _sr_gui_widen_string(message);
	const size_t messageSize = strlen(message);
	WCHAR* messageW			 = (WCHAR*)SR_GUI_MALLOC((messageSize + 2 + 1) * sizeof(WCHAR));
	if(messageW == NULL) {
		SR_GUI_FREE(messageWTemp);
		return SR_GUI_CANCELLED;
	}
	// Append a line return to make room for the text field that we will add afterwards.
	memcpy(messageW, messageWTemp, messageSize * sizeof(WCHAR));
	messageW[messageSize]	  = TEXT('\n');
	messageW[messageSize + 1] = TEXT('\n');
	messageW[messageSize + 2] = '\0';
	SR_GUI_FREE(messageWTemp);

	WCHAR* titleW = _sr_gui_widen_string(title);

	_sr_gui_message_callback_data callData;
	callData.content = result;

	TASKDIALOGCONFIG dialog	  = {0};
	dialog.cbSize			  = sizeof(TASKDIALOGCONFIG);
	dialog.hInstance		  = NULL;
	dialog.dwCommonButtons	  = TDCBF_OK_BUTTON | TDCBF_CANCEL_BUTTON;
	dialog.pszMainInstruction = titleW;
	dialog.pszContent		  = messageW;
	dialog.nDefaultButton	  = IDOK;
	dialog.lpCallbackData	  = (LONG_PTR)(&callData);
	dialog.pfCallback		  = _sr_gui_message_callback;

	int button	= -1;
	HRESULT res = TaskDialogIndirect(&dialog, &button, NULL, NULL);
	SR_GUI_FREE(messageW);
	SR_GUI_FREE(titleW);
	DeleteObject(callData.font);
	DeleteObject(callData.textField);

	// Result status.
	if(FAILED(res) || button != IDOK) {
		if(*(callData.content) != NULL) {
			SR_GUI_FREE(*(callData.content));
		}
		*(callData.content) = NULL;
		return SR_GUI_CANCELLED;
	}

	// The string pointer has been stored in the callback data and is already up to date.
	return SR_GUI_VALIDATED;
}

int sr_gui_ask_color(unsigned char color[3]) {

	DWORD colorD = RGB(color[0], color[1], color[2]);
	// Preserve palette of favorite colors between calls.
	static COLORREF acrCustClr[16];
	CHOOSECOLOR colorSettings;
	memset(&colorSettings, 0, sizeof(CHOOSECOLOR));
	colorSettings.lStructSize  = sizeof(CHOOSECOLOR);
	colorSettings.hwndOwner	   = NULL;
	colorSettings.rgbResult	   = colorD;
	colorSettings.lpCustColors = (LPDWORD)acrCustClr;
	colorSettings.Flags		   = CC_FULLOPEN | CC_RGBINIT | CC_ANYCOLOR;

	BOOL res = ChooseColor(&colorSettings);

	if(res != TRUE) {
		return SR_GUI_CANCELLED;
	}

	color[0] = GetRValue(colorSettings.rgbResult);
	color[1] = GetGValue(colorSettings.rgbResult);
	color[2] = GetBValue(colorSettings.rgbResult);

	return SR_GUI_VALIDATED;
}

int sr_gui_open_in_explorer(const char* path){

	WCHAR* pathW = _sr_gui_widen_string(path);

	bool done = false;
	IShellFolder* desktop = NULL;
	HRESULT hr = SHGetDesktopFolder(&desktop);
	if (!FAILED(hr)) {
		LPITEMIDLIST file_item = NULL;
		hr = desktop->ParseDisplayName(NULL, NULL, pathW, NULL, &file_item, NULL);
		if (!FAILED(hr)) {
			hr = SHOpenFolderAndSelectItems(file_item, 0, NULL, NULL);
			done = !FAILED(hr);
		}
		if (file_item) {
			CoTaskMemFree(file_item);
		}
	}
	if (desktop) {
		desktop->Release();
	}
	if (!done) {
		// Open parent directory using shell.
		// Extract prefix path by finding the last separator.
		wchar_t* tail = wcsrchr(pathW, '\\');
		if (tail == NULL) {
			tail = wcsrchr(pathW, '/');
		}
		if (tail) {
			*tail = '\0';
		}
		ShellExecute(NULL, L"open", pathW, NULL, NULL, SW_SHOW);
	}
	
	SR_GUI_FREE(pathW);
	return SR_GUI_VALIDATED;
}

#endif
