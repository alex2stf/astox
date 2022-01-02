#ifndef ASTOX_ICC_H
#define ASTOX_ICC_H
#include "types.h"

//    #include <stdlib.h>
//    #include "FileSystem.h"
//
//enum DialogFlags {
//    DG_DEFAULT,
//    DG_OK_WARNING,
//    DG_OK_ERROR,
//    DG_OK_INFO,
//    DG_OKCANCEL,
//    DG_OKCANCEL_WARNING,
//    DG_OKCANCEL_ERROR,
//    DG_OKCANCEL_INFO,
//    DG_YESNO,
//    DG_YESNO_WARNING,
//    DG_YESNO_ERROR,
//    DG_YESNO_INFO,
//};
//
//struct OpenFileResult {
//    const char * filePath;
//    int result;
//};
//
//OpenFileResult MINGUI_OpenFile(const char * selectTitle = "Open File",
//        const char * message = "Choose a file",
//    #ifdef ASTOX_OS_WINDOWS
//        const char * startupDirectory = "C:",
//    #else
//        const char * startupDirectory = "/",
//    #endif
//        bool canChooseFiles = true, bool canChooseDirectories = true);
//
//    #ifdef ASTOX_OS_WINDOWS
//        #include <windows.h>
//        #include <stdlib.h>
//        #include <string.h>
//        #include <tchar.h>
//        #include <ctime>
//        #include <iostream>
//
//        #pragma comment(lib, "user32")
//
//enum DialogAnswers {
//    DGANSW_OK = IDOK,
//    DGANSW_CANCEL = IDCANCEL,
//    DGANSW_YES = IDYES,
//    DGANSW_NO = IDNO,
//    DGANSW_CLOSED = IDABORT
//};
//
//
//
//
//LRESULT CALLBACK __WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
//int WINAPI __WinMain(const char * title, const char * text, const char * footer);
//int MINGUI_Dialog(const char * title, const char * message, void (*callback)(int stat, int pid), int flags, int pid = 0);
//void MINGUI_Notification(const char * title, const char * subtitle, const char * footer, int lifetime);
//
//
//    #elif ASTOX_ENABLE_GTK
//        #include <gtk/gtk.h>
//        #include <libnotify/notify.h>
//        #include <iostream>
//
//enum DialogAnswers {
//    DGANSW_OK = GTK_RESPONSE_OK,
//    DGANSW_CANCEL = GTK_RESPONSE_CANCEL,
//    DGANSW_YES = GTK_RESPONSE_YES,
//    DGANSW_NO = GTK_RESPONSE_NO,
//    DGANSW_CLOSED = GTK_RESPONSE_CLOSE
//};
//void MINGUI_init(int argc, char *argv[]);
//int MINGUI_Dialog(const char * title, const char * message, void (*callback)(int stat, int pid) = NULL, int flags = 0, int pid = 1);
//void MINGUI_Notification(const char * title = "NotificationTitle", const char * subtitle = "notificationSubtitle", const char * footer = "notificationFooter", int lifetime = 5);
//
//#elif ASTX_ENABLE_OBJECTIVECPP
//#pragma once
//
//enum DialogAnswers {
//    DGANSW_OK = 0,
//    DGANSW_CANCEL = 1,
//    DGANSW_YES = 2,
//    DGANSW_NO = 3,
//    DGANSW_CLOSED = 4
//};
//
//        #ifdef __cplusplus
//extern "C" {
//        #endif
//    int MINGUI_Dialog(const char * title, const char * message, void (*callback)(int stat, int pid) = NULL, int flags = 0, int pid = 1);
//    void MINGUI_Notification(const char * title = "NotificationTitle", const char * subtitle = "notificationSubtitle", const char * footer = "notificationFooter", int lifetime = 5);
//
//        #ifdef __cplusplus
//}
//        #endif
//    #elif ASTX_ENABLE_QT_MINGUI
//
//enum DialogAnswers {
//    DGANSW_OK = 0,
//    DGANSW_CANCEL = 1,
//    DGANSW_YES = 2,
//    DGANSW_NO = 3,
//    DGANSW_CLOSED = 4
//};
//
//void MINGUI_init();
//int MINGUI_Dialog(const char * title, const char * message, void (*callback)(int stat, int pid) = NULL, int flags = 0, int pid = 1);
//void MINGUI_Notification(const char * title = "NotificationTitle", const char * subtitle = "notificationSubtitle", const char * footer = "notificationFooter", int lifetime = 5);
//#else
//
//    enum DialogAnswers {
//    DGANSW_OK = 0,
//    DGANSW_CANCEL = 1,
//    DGANSW_YES = 2,
//    DGANSW_NO = 3,
//    DGANSW_CLOSED = 4
//};
//int MINGUI_Dialog(const char * title, const char * message, void (*callback)(int stat, int pid) = NULL, int flags = 0, int pid = 1);
//void MINGUI_Notification(const char * title = "NotificationTitle", const char * subtitle = "notificationSubtitle", const char * footer = "notificationFooter", int lifetime = 5);


namespace astox {
	namespace icc {

		#ifdef ASTOX_OS_WINDOWS
			#define ASTX_WIN32_STYLE WS_DLGFRAME | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX
		
			#define ADDPOPUPMENU(hmenu, string) \
					HMENU hSubMenu = CreatePopupMenu(); \
					AppendMenu(hmenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, string);

			#define ADDMENUITEM(hmenu, ID, string) \
				AppendMenu(hSubMenu, MF_STRING, ID, string);
		#endif

		struct Dimension {
			int width;
			int height;
		};

		Dimension getScreenSize();
		int countMonitors();
		
		class GenericModel : public Value {
		protected:
			int _width;
			int _height;
			int _x;
			int _y;
			std::map<char *, astox_function_type> events;
			
		public:
			void addEventListener(char * name, astox_function_type type) {
				events[name] = type;
			}

			void callEvent(char * eventName, Value * value) {
				if (events.find(eventName) != events.end()) {
					//TODO
					//events[eventName](value);
				}
			};

			int getInt(std::string key);
			GenericModel * set(std::string key, int value);
			GenericModel * set(std::string key, std::string value);
			GenericModel * set(std::string property, double a, double b, double c, double d);
		};

		class Menu : public Value {
		private:
			std::string _text;
			astox_function_type _clickevent;
		public:
			Menu(std::string text, astox_function_type clickevent);
			std::string getText();
			void callClick();
		};

		class SubMenu : public Value {
			private:
				std::string _title;
				std::vector<Menu *> menus;
				std::vector<SubMenu *> submenus;
			public:
				SubMenu * addSubMenu(std::string text);
				Menu * addMenu(std::string, astox_function_type handler);
				std::string getTitle();
				void setTitle(std::string title);
				std::vector<Menu *> &getMenus();
				std::vector<SubMenu *> &getSubMenus();
		};

		class GUIRoot : public GenericModel, public SubMenu {
		public:
			static GUIRoot * getInstance();
			int display();
			
			GUIRoot * setUndecorated(bool decorated);
			bool isUndecorated();
			std::map<int, Menu*> menumap;

			#ifdef ASTOX_OS_WINDOWS
				void win32setHwnd(HWND hwnd);
			#endif // ASTOX_OS_WINDOWS
			Menu * getMenuById(int id);
			void registerMenu(int id, Menu * menu);
		private:
			static GUIRoot * instance;
			GUIRoot();			
			bool _undecor;

		
			#ifdef ASTOX_OS_WINDOWS
				HWND hwnd;
			#endif // ASTOX_OS_WINDOWS
		};

		#ifdef ASTOX_OS_WINDOWS
			int WINAPI win32_window_construct(GUIRoot * window);
		#endif

	}
}

#endif // !ASTOX_ICC_H
