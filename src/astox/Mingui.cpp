#include "Mingui.h"



//LRESULT CALLBACK __WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
//    HDC hdcStatic;
//    switch (message) {
//        case WM_CREATE:
//            return 0;
//        case WM_DESTROY:
//            PostQuitMessage(0);
//            break;
//        case WM_CTLCOLORSTATIC:
//            hdcStatic = (HDC) wParam;
//            SetTextColor(hdcStatic, RGB(0, 0, 0));
//            SetBkMode(hdcStatic, TRANSPARENT);
//            return (LRESULT) GetStockObject(NULL_BRUSH);
//        default:
//            return DefWindowProc(hwnd, message, wParam, lParam);
//    }
//    return 0;
//}
//
//static inline int WINAPI __WinMain(const char * title, const char * text, const char * footer) {
//    HINSTANCE hThisInstance = GetModuleHandle(NULL);
//    LPSTR lpszArgument = NULL;
//    int nFunsterStil = 1, _winWidth = 200, _winHeight = 60, _winX = GetSystemMetrics(SM_CXSCREEN) - _winWidth - 4, _winY = 0;
//
//    HWND hwnd;
//    WNDCLASSEX wincl;
//    wincl.hInstance = hThisInstance;
//    wincl.lpszClassName = astox::util::CHAR2LPCWSTR(title);
//    wincl.lpfnWndProc = __WindowProcedure;
//    wincl.style = CS_DBLCLKS;
//    wincl.cbSize = sizeof (WNDCLASSEX);
//    wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
//    wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
//    wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
//    wincl.lpszMenuName = NULL;
//    wincl.cbClsExtra = 0;
//    wincl.cbWndExtra = 0;
//    wincl.hbrBackground = (HBRUSH) CTLCOLOR_DLG;
//
//    /* Register the window class, and if it fails quit the program */
//    if (!RegisterClassEx(&wincl))
//        return 0;
//
//    hwnd = CreateWindowEx(
//            0, /* Extended possibilites for variation */
//		astox::util::CHAR2LPCWSTR(title), /* Classname */
//		astox::util::CHAR2LPCWSTR(title), /* Title Text */
//            WS_DLGFRAME, /* default window */ //WS_POPUP  pentru no title and title bar
//            _winX, /* Windows decides the position */
//            _winY, /* where the window ends up on the screen */
//            _winWidth, /* The programs width */
//            _winHeight, /* and height in pixels */
//            HWND_DESKTOP, /* The window is a child-window to desktop */
//            NULL, /* No menu */
//            hThisInstance, /* Program Instance handler */
//            NULL /* No Window Creation data */
//            );
//
//    //HWND_TOPMOST o mentine in fata
//    SetWindowPos(hwnd, HWND_TOPMOST, _winX, _winY, _winWidth, _winHeight, SWP_SHOWWINDOW);
//    HFONT hfFont1 = CreateFont(14, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, CHAR2LPCWSTR("Arial"));
//    HFONT hfFont2 = CreateFont(13, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, CHAR2LPCWSTR("Arial"));
//    /*HWND hwnd_st_u = CreateWindowEx(0,  "static", NULL, WS_CHILD | WS_VISIBLE | WS_TABSTOP, 0, 0, 200, 10,  hwnd,   (HMENU)(2),  (HINSTANCE) GetWindowLong (hwnd, GWL_HINSTANCE),   NULL);*/
//
//    HWND titleComp = CreateWindowEx(0, CHAR2LPCWSTR("STATIC"),
//		astox::util::CHAR2LPCWSTR(text), WS_CHILD | WS_VISIBLE | SS_LEFT,
//            0, 2, 200, 14,
//            hwnd, HMENU(NULL), GetModuleHandle(NULL), NULL);
//
//    HWND subtitle = CreateWindowEx(0, CHAR2LPCWSTR("STATIC"), CHAR2LPCWSTR(footer), WS_CHILD | WS_VISIBLE | SS_LEFT, 0, 19, 200, 12, hwnd, HMENU(NULL), GetModuleHandle(NULL), NULL);
//
//    /* Make the window visible on the screen */
//    ShowWindow(hwnd, nFunsterStil);
//    SendMessage(titleComp, WM_SETFONT, WPARAM(hfFont1), TRUE);
//    SendMessage(subtitle, WM_SETFONT, WPARAM(hfFont2), TRUE);
//    return 0; //messages.wParam;
//}
//
//int MINGUI_Dialog(const char * title, const char * message, void (*callback)(int stat, int pid), int flags, int pid) {
//    UINT __msgListFlags[] = {
//        NULL,
//        MB_OK | MB_ICONWARNING | MB_ICONEXCLAMATION,
//        MB_OK | MB_ICONSTOP | MB_ICONERROR | MB_ICONHAND,
//        MB_OK | MB_ICONINFORMATION | MB_ICONASTERISK,
//        MB_OKCANCEL,
//        MB_OKCANCEL | MB_ICONWARNING | MB_ICONEXCLAMATION,
//        MB_OKCANCEL | MB_ICONSTOP | MB_ICONERROR | MB_ICONHAND,
//        MB_OKCANCEL | MB_ICONINFORMATION | MB_ICONASTERISK,
//        MB_YESNO,
//        MB_YESNO | MB_ICONWARNING | MB_ICONEXCLAMATION,
//        MB_YESNO | MB_ICONSTOP | MB_ICONERROR | MB_ICONHAND,
//        MB_YESNO | MB_ICONINFORMATION | MB_ICONASTERISK,
//    };
//    int result = MessageBox(NULL, astox::util::CHAR2LPCWSTR(title), CHAR2LPCWSTR(message), __msgListFlags[flags]);
//    if (callback != NULL) {
//        callback(result, pid);
//    }
//    return result;
//}
//
//void MINGUI_Notification(const char * title, const char * subtitle, const char * footer, int lifetime) {
//    printf("startiwherse");
//    clock_t startedAt = clock(); // da 0, 2000
//    clock_t updateAt = 0; // da 1, 2001
//    int diff = updateAt - startedAt; // MSG messages;
//    __WinMain(title, subtitle, footer);
//    while (diff <= lifetime) { //	while (GetMessage (&messages, NULL, 0, 0)) {
//        updateAt = clock();
//        diff = updateAt - startedAt;
//        //std::cout << diff << std::endl; //TranslateMessage(&messages); //DispatchMessage(&messages);
//    }
//}
//
//OpenFileResult MINGUI_OpenFile(const char * selectTitle, const char * message, const char * startupDirectory,
//        bool canChooseFiles, bool canChooseDirectories) {
//    OpenFileResult r;
//    r.filePath = "";
//    r.result = 0;
//    printf("windows method not implemented\n");
//
//    return r;
//}
//
//#elif ASTOX_ENABLE_GTK
//
//void MINGUI_init(int argc, char *argv[]) {
//    gtk_init(&argc, &argv);
//}
//
//int MINGUI_Dialog(const char * title, const char * message, void (*callback)(int stat, int pid), int flags, int pid) {
//    GtkWidget *dialog;
//
//    switch (flags) {
//        case DG_DEFAULT:
//            dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_OTHER, GTK_BUTTONS_OK, message);
//            break;
//
//        case DG_OK_WARNING:
//            dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, message);
//            break;
//
//        case DG_OK_ERROR:
//            dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, message);
//            break;
//
//        case DG_OK_INFO:
//            dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, message);
//            break;
//
//        case DG_OKCANCEL:
//            dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_OTHER, GTK_BUTTONS_OK_CANCEL, message);
//            break;
//
//        case DG_OKCANCEL_WARNING:
//            dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK_CANCEL, message);
//            break;
//
//        case DG_OKCANCEL_ERROR:
//            dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK_CANCEL, message);
//            break;
//
//        case DG_OKCANCEL_INFO:
//            dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK_CANCEL, message);
//            break;
//
//        case DG_YESNO:
//            dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_OTHER, GTK_BUTTONS_YES_NO, message);
//            break;
//
//        case DG_YESNO_WARNING:
//            dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_YES_NO, message);
//            break;
//
//        case DG_YESNO_ERROR:
//            dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_YES_NO, message);
//            break;
//
//        case DG_YESNO_INFO:
//            dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_YES_NO, message);
//            break;
//
//        default:
//            dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK_CANCEL, message);
//            break;
//    }
//
//
//    gtk_window_set_title(GTK_WINDOW(dialog), title);
//    gint result = gtk_dialog_run(GTK_DIALOG(dialog));
//    gtk_widget_destroy(dialog);
//    if (callback != NULL) {
//        callback(result, pid);
//    }
//
//    return result;
//
//};
//
//void MINGUI_Notification(const char * title, const char * text, const char * footer, int lifetime) {
//    notify_init(title);
//    NotifyNotification * notification;
//    std::string cmptxt = text;
//    cmptxt.append("\n");
//    cmptxt.append(footer);
//    notification = notify_notification_new(title, cmptxt.c_str(), NULL);
//    notify_notification_set_timeout(notification, lifetime);
//    notify_notification_close(notification, NULL);
//    notify_notification_show(notification, NULL);
//};
//
//OpenFileResult MINGUI_OpenFile(const char * selectTitle, const char * message, const char * startupDirectory,
//        bool canChooseFiles, bool canChooseDirectories) {
//
//
//
//    OpenFileResult response;
//    GtkWidget *dialog;
//    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
//    gint res;
//
//    dialog = gtk_file_chooser_dialog_new(selectTitle,
//            NULL,
//            action,
//            "Cancel",
//            GTK_RESPONSE_CANCEL,
//            "Open",
//            GTK_RESPONSE_ACCEPT,
//            NULL);
//    gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog), message);
//
//    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), startupDirectory);
//
//    response.result = gtk_dialog_run(GTK_DIALOG(dialog));
//    GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
//    response.filePath = gtk_file_chooser_get_filename(chooser);
//    gtk_widget_destroy(dialog);
//    return response;
//};
//#elif ASTX_ENABLE_QT_MINGUI
//
//static bool started = false;
//static QApplication * rootApp;
//
//void MINGUI_init() {
//    if (!started) {
//        char arg0[] = "programName";
//        char arg1[] = "arg";
//        char arg2[] = "another arg";
//        char* argv[] = {&arg0[0], &arg1[0], &arg2[0], NULL};
//        int argc = (int) (sizeof (argv) / sizeof (argv[0])) - 1;
//        rootApp = new QApplication(argc, &argv[0]);
//        rootApp->exec();
//        QWindow window;
//        window.show();
//    }
//};
//
//int MINGUI_Dialog(const char * title, const char * message, void (*callback)(int stat, int pid),
//        int flags, int pid) {
//    if (!started) {
//        MINGUI_init();
//    }
//
//
//    QMessageBox msgBox;
//    msgBox.setText("The document has been modified.");
//    msgBox.setInformativeText("Do you want to save your changes?");
//    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
//    msgBox.setDefaultButton(QMessageBox::Save);
//    msgBox.setFocus();
//    msgBox.setWindowFlags(Qt::WindowStaysOnTopHint);
//    int ret = msgBox.exec();
//    //the_application.exit();
//
//}
//
//void MINGUI_Notification(const char * title, const char * subtitle,
//        const char * footer, int lifetime) {
//
//}
//
//OpenFileResult MINGUI_OpenFile(const char * selectTitle,
//        const char * message, const char * startupDirectory,
//        bool canChooseFiles, bool canChooseDirectories) {
//
//}
//#else
//
//int MINGUI_Dialog(const char * title, const char * message, void (*callback)(int stat, int pid),
//        int flags, int pid) {
//
//    astox_console_write(BG_GREEN_1, "DIALOG:\n\t%s\n\t%s\n", title, message);
//    if (callback) {
//        callback(0, 0);
//    }
//    return 0;
//};
//
//void MINGUI_Notification(const char * title, const char * subtitle,
//        const char * footer, int lifetime) {
//    astox_console_write(BG_GREEN_2, "NOTIFY:\n\t%s\n\t%s\n", title, subtitle);
//};
//
//OpenFileResult MINGUI_OpenFile(const char * selectTitle,
//        const char * message, const char * startupDirectory,
//        bool canChooseFiles, bool canChooseDirectories) {
//    astox_console_write(BG_GREEN_2, "MINGUI_OpenFile:\n\t%s\n\t%s\n", selectTitle, message);
//    OpenFileResult r;
//    return r;
//}

namespace astox {
	namespace icc {
		using namespace astox::util;

		GUIRoot * GUIRoot::instance = new GUIRoot();

		Dimension getScreenSize() {
			Dimension n;
			#ifdef  ASTOX_OS_WINDOWS
				n.width = GetSystemMetrics(SM_CXSCREEN);
				n.height = GetSystemMetrics(SM_CYSCREEN);
			#endif //  ASTOX_OS_WINDOWS
			return n;
		};

		int countMonitors() {
			#ifdef  ASTOX_OS_WINDOWS
				return GetSystemMetrics(SM_CMONITORS);
			#endif //  ASTOX_OS_WINDOWS
			return 0;
		}

		GenericModel * GenericModel::set(std::string key, int value) {
			if (key == "width") { _width = value; }
			if (key == "height") { _height = value; }
			if (key == "x") { _x = value; }
			if (key == "y") { _y = value; }
			return this;
		}

		int  GenericModel::getInt(std::string key) {
			if (key == "width") { return _width; }
			if (key == "height") { return _height; }
			if (key == "x") { return _x; }
			if (key == "y") { return _y; }

			return 0;
		}

		GenericModel * GenericModel::set(std::string key, std::string value) {
			return this;
		}

		GenericModel * GenericModel::set(std::string property, double a, double b, double c, double d) {
			return this;
		}

		

		Menu::Menu(std::string text, astox_function_type clickevent) {
			static int num_menus = 1;
			num_menus++;
			_text = text;
			_clickevent = clickevent;
		}

		void Menu::callClick() {
			_clickevent(this);
		};

		std::string Menu::getText() {
			return _text;
		}

		Menu * SubMenu::addMenu(std::string text, astox_function_type handler) {
			Menu * menu = new Menu(text, handler);
			menus.push_back(menu);
			GUIRoot::getInstance()->registerMenu(menu->getUid(), menu);
			return menu;
		};

		SubMenu * SubMenu::addSubMenu(std::string text) {
			SubMenu * subMenu = new SubMenu();
			subMenu->setTitle(text);
			submenus.push_back(subMenu);
			return subMenu;
		};


		std::vector<Menu *> &SubMenu::getMenus() {
			return menus;
		};

		std::vector<SubMenu *> &SubMenu::getSubMenus() {
			return submenus;
		};


		GUIRoot::GUIRoot() {
			_x = 0;
			_y = 0;
			_width = getScreenSize().width;
			_height = getScreenSize().height;
			_undecor = false;
		}

		GUIRoot * GUIRoot::getInstance() {
			return instance;
		}

		std::string SubMenu::getTitle() {
			return _title;
		}

		void SubMenu::setTitle(std::string title) {
			_title = title;
		}

		GUIRoot *  GUIRoot::setUndecorated(bool prm) {
			_undecor = prm;
			#ifdef ASTOX_OS_WINDOWS
			if (hwnd != NULL) {
				if (!_undecor) {
					SetWindowLong(hwnd, GWL_STYLE, ASTX_WIN32_STYLE);
				}
				else {
					SetWindowLong(hwnd, GWL_STYLE, WS_POPUP);
				}
			}
			#endif // ASTOX_OS_WINDOWS

			return this;
		};

		Menu *  GUIRoot::getMenuById(int id) {
			return menumap[id];
		};
		void  GUIRoot::registerMenu(int id, Menu * menu) {
			menumap[id] = menu;
		};

		bool  GUIRoot::isUndecorated() {
			return _undecor;
		};

		#ifdef ASTOX_OS_WINDOWS
			void GUIRoot::win32setHwnd(HWND h) {
				hwnd = h;
			};
		#endif // ASTOX_OS_WINDOWS




#ifdef ASTOX_OS_WINDOWS
		static inline LRESULT CALLBACK win32_window_procedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
			HDC hdcStatic;
			switch (message) {
			case WM_COMMAND:
				GUIRoot::getInstance()->getMenuById(LOWORD(wParam))->callClick();
				return 0;
			case WM_CLOSE:
				DestroyWindow(hwnd);
			case WM_CREATE:
				return 0;
			case WM_DESTROY:
				PostQuitMessage(0);
				break;
			case WM_CTLCOLORSTATIC:
				hdcStatic = (HDC)wParam;
				SetTextColor(hdcStatic, RGB(0, 0, 0));
				SetBkMode(hdcStatic, TRANSPARENT);
				return (LRESULT)GetStockObject(NULL_BRUSH);
			default:
				return DefWindowProc(hwnd, message, wParam, lParam);
			}
			return 0;
		}

		static inline void win32_fill_menus(std::vector<Menu*> menus, std::vector<SubMenu *> submenus, HWND window, HMENU menu) {
			for (int i = 0; i < menus.size(); i++) {
				std::string text = menus.at(i)->getText();
				int id = menus.at(i)->getUid();
				std::wstring tmp(text.begin(), text.end());
				AppendMenu(menu, MF_STRING, id, tmp.c_str());
			}

			if (!submenus.empty()) {
				for (int i = 0; i < submenus.size(); i++) {
					HMENU hSubMenu = CreatePopupMenu();
					std::string title = submenus.at(i)->getTitle();
					std::wstring tmp2(title.begin(), title.end());
					AppendMenu(menu, MF_STRING | MF_POPUP,(UINT_PTR)hSubMenu, tmp2.c_str());
					win32_fill_menus(
						submenus.at(i)->getMenus(),
						submenus.at(i)->getSubMenus(),
						window,
						hSubMenu
					);
				}
			}

		}

		int WINAPI win32_window_construct(GUIRoot * win) {
			HINSTANCE hThisInstance = GetModuleHandle(NULL);
			LPSTR lpszArgument = NULL;
			int nFunsterStil = 1, 
				_winWidth = win->getInt("width"), 
				_winHeight = win->getInt("height"), 
				_winX = win->getInt("x"), 
				_winY = win->getInt("y");
			const char * title =  win->getTitle().c_str();

			HWND hwnd;
			
			WNDCLASSEX wincl;
			wincl.hInstance = hThisInstance;
			wincl.lpszClassName = TEXT("AstoxMainWindow");
			wincl.lpfnWndProc = win32_window_procedure;
			wincl.style = CS_DBLCLKS;
			wincl.cbSize = sizeof(WNDCLASSEX);
			wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
			wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
			wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
			wincl.lpszMenuName = NULL;
			wincl.cbClsExtra = 0;
			wincl.cbWndExtra = 0;
			wincl.hbrBackground = (HBRUSH)CTLCOLOR_DLG;
		


			/* Register the window class, and if it fails quit the program */
			if (!RegisterClassEx(&wincl)) {
				return 0;
			}
			DWORD dwStyle;
			if (win->isUndecorated()) {
				dwStyle = WS_POPUP;
			}
			else {
				dwStyle = ASTX_WIN32_STYLE;
			}

			hwnd = CreateWindowEx(
				0, /* Extended possibilites for variation */
				TEXT("AstoxMainWindow"), /* Classname */
				CHAR2LPCWSTR(title), /* Title Text */
				dwStyle, /* default window */ //WS_POPUP  pentru no title and title bar
				_winX, /* Windows decides the position */
				_winY, /* where the window ends up on the screen */
				_winWidth, /* The programs width */
				_winHeight, /* and height in pixels */
				HWND_DESKTOP, /* The window is a child-window to desktop */
				NULL, /* No menu */
				hThisInstance, /* Program Instance handler */
				NULL /* No Window Creation data */
			);

			if (hwnd == 0) {
				MessageBox(0, L"Window Creation FAILED", 0, 0);
				return -1;
			}

			win->win32setHwnd(hwnd);
			win->callEvent("opened", NULL);
			HMENU menu = CreateMenu();
			win32_fill_menus(win->getMenus(), win->getSubMenus(), hwnd, menu);
			SetMenu(hwnd, menu);
			

			
			SetWindowPos(hwnd, HWND_TOPMOST, _winX, _winY, _winWidth, _winHeight, SWP_SHOWWINDOW);
			HFONT hfFont1 = CreateFont(14, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, CHAR2LPCWSTR("Arial"));
			HFONT hfFont2 = CreateFont(13, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, CHAR2LPCWSTR("Arial"));
			/*HWND hwnd_st_u = CreateWindowEx(0,  "static", NULL, WS_CHILD | WS_VISIBLE | WS_TABSTOP, 0, 0, 200, 10,  hwnd,   (HMENU)(2),  (HINSTANCE) GetWindowLong (hwnd, GWL_HINSTANCE),   NULL);*/

			HWND titleComp = CreateWindowEx(0, CHAR2LPCWSTR("STATIC"),
				CHAR2LPCWSTR(title), WS_CHILD | WS_VISIBLE | SS_LEFT,
				0, 2, 200, 14,
				hwnd, HMENU(NULL), GetModuleHandle(NULL), NULL);

			HWND subtitle = CreateWindowEx(0, CHAR2LPCWSTR("STATIC"), CHAR2LPCWSTR(title), WS_CHILD | WS_VISIBLE | SS_LEFT, 0, 19, 200, 12, hwnd, HMENU(NULL), GetModuleHandle(NULL), NULL);
			

			/* Make the window visible on the screen */
			ShowWindow(hwnd, nFunsterStil);
			SendMessage(titleComp, WM_SETFONT, WPARAM(hfFont1), TRUE);
			SendMessage(subtitle, WM_SETFONT, WPARAM(hfFont2), TRUE);
			UpdateWindow(hwnd);
			printf("final");

			//this prevent window from having loading cursor
			MSG msg;
			while (GetMessage(&msg, NULL, 0, 0) != 0) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			return 0; //messages.wParam;
		}
#endif

		int GUIRoot::display() {
			#ifdef ASTOX_OS_WINDOWS
				return win32_window_construct(this);
            #endif
			return 0;
		}


	}
}
