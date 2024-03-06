#include "qmobject.h"

#define INCOMECLICK 5

#define OFFMAINX 150
#define OFFMAINY 0
#define OFFCALLX 0
#define OFFCALLY 50
#define OFFINVX 0
#define OFFINVY 50

#define CNTSHOW 6
#define CNTSLEEP 50

#include <QProcess>
#include <tlhelp32.h>

QMObject* mobj;
int is_income_clicked;
bool bincomming;


bool bcallwindow = false;
bool ncallwindow = false;
HWND callwnd = nullptr;

void setWindowTransparency(HWND hwnd, BYTE alpha) {
    LONG_PTR style = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
    SetWindowLongPtr(hwnd, GWL_EXSTYLE, style | WS_EX_LAYERED);
    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), alpha, LWA_ALPHA);
}


QString getExecutableNameFromHWND(HWND hwnd)
{
    DWORD processId;
    GetWindowThreadProcessId(hwnd, &processId);

    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap == INVALID_HANDLE_VALUE)
        return QString();

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(hSnap, &pe))
    {
        do
        {
            if (pe.th32ProcessID == processId)
            {
                CloseHandle(hSnap);
                return QString::fromWCharArray(pe.szExeFile);
            }
        } while (Process32Next(hSnap, &pe));
    }

    CloseHandle(hSnap);
    return QString();
}

BOOL CALLBACK HideCallWindow(HWND hwnd, LPARAM lParam) {
    char className[256];
    char captionName[256];
    GetClassNameA(hwnd, className, sizeof(className) / sizeof(wchar_t));

    QString str;
    GetWindowTextA(hwnd,captionName,256);
    // if(strcmp(captionName, "PopupHost") == 0)
    if(strcmp(captionName, "Voice call ?- WhatsApp") == 0)
    {

            // qDebug()<<exefile;
        ncallwindow = true;
        callwnd = hwnd;

        return TRUE;
        // ShowWindow(hwnd, SW_HIDE);
        // setWindowTransparency(hwnd, ALPHAVALUE);
        // RECT rt;
        // GetWindowRect(hwnd, &rt);
        // MoveWindow(hwnd, MOVERECTX,MOVERECTY,rt.right-rt.left, rt.bottom-rt.top,FALSE);
    }
    return TRUE;
}

void QMObject::emitCallWindow(){
    bnow_call = true;
}

BOOL CALLBACK CheckCallWindow(HWND hwnd, LPARAM lParam) {
    char className[256];
    char captionName[256];
    GetClassNameA(hwnd, className, sizeof(className) / sizeof(wchar_t));
    QString str;
    GetWindowTextA(hwnd,captionName,256);
    // if(strcmp(captionName, "Untitled - Notepad") == 0)
    if(strcmp(captionName, "Voice call ?- WhatsApp") == 0)
    {
        // setWindowTransparency(hwnd, 100);
        mobj->emitCallWindow();
    }
    return TRUE;
}
void SimulateMouseClick(int x, int y) {
    INPUT input[2] = {0};
    input[0].type = INPUT_MOUSE;
    input[0].mi.dx = x;
    input[0].mi.dy = y;
    input[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

    input[1].type = INPUT_MOUSE;
    input[1].mi.dx = x;
    input[1].mi.dy = y;
    input[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;

    SendInput(2, input, sizeof(INPUT));
}
BOOL HideIncomingWindow(HWND hwnd) {
    ShowWindow(hwnd, SW_HIDE);
    setWindowTransparency(hwnd, ALPHAVALUE);
}
HWND checkIncoming(){
    POINT pt;
    pt.x = 1400 + 100;
    pt.y = 120 + 30;
    HWND hWnd = WindowFromPoint(pt);
    const char* vccaption = "Voice call ?- WhatsApp";
    // const char* vccaption = "Untitled - Notepad";
    // Check if the window handle is valid
    char captionName[256];

    if (hWnd != nullptr) {
        GetWindowTextA(hWnd, captionName, 256);
        while(hWnd != nullptr && strcmp(captionName, vccaption) != 0)
        {
            void* hwndVoidPtr = reinterpret_cast<void*>(hWnd);
            int hwndIntt = reinterpret_cast<intptr_t>(hwndVoidPtr);
            hWnd = GetParent(hWnd);
            GetWindowTextA(hWnd, captionName, 256);
        }
        if(hWnd != nullptr){
            // if(!bincomming){
            //     if(is_income_clicked >= INCOMECLICK){
            //         is_income_clicked = 0;
            //         return hWnd;
            //     } else {
            //         is_income_clicked++;
            //         POINT ptn;
            //         GetCursorPos(&ptn);
            //         SetCursorPos(pt.x, pt.y);
            //         SimulateMouseClick(pt.x, pt.y);
            //         Sleep(10);
            //         SetCursorPos(ptn.x, ptn.y);
            //         return hWnd;
            //     }
            // } else {
            //     return hWnd;
            // }
        }
    }
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    pt.x = screenWidth - 150;
    pt.y = 100;
    hWnd = WindowFromPoint(pt);

    // Check if the window handle is valid
    if (hWnd != nullptr) {
        GetWindowTextA(hWnd, captionName, 256);
        while(hWnd != nullptr && strcmp(captionName, vccaption) != 0)
        {
            void* hwndVoidPtr = reinterpret_cast<void*>(hWnd);
            int hwndIntt = reinterpret_cast<intptr_t>(hwndVoidPtr);
            hWnd = GetParent(hWnd);
            GetWindowTextA(hWnd, captionName, 256);
        }
        if(hWnd != nullptr){
            // if(!bincomming){
            //     if(is_income_clicked >= INCOMECLICK){
            //         is_income_clicked = 0;
            //         return hWnd;
            //     } else {
            //         is_income_clicked++;
            //         POINT ptn;
            //         GetCursorPos(&ptn);
            //         SetCursorPos(pt.x, pt.y);
            //         SimulateMouseClick(pt.x, pt.y);
            //         Sleep(10);
            //         SetCursorPos(ptn.x, ptn.y);
            //         return hWnd;
            //     }
            // } else {
            //     return hWnd;
            // }
        }
    }
    // is_income_clicked = 0;
    return nullptr;
}



void setWhatsAppSubkeys() {


    QString parentKeyPath = "HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Notifications\\Settings";
    QString valueName = "Enabled";
    int valueData = 0;

    QSettings settings(parentKeyPath, QSettings::NativeFormat);
    QStringList subKeys = settings.childGroups();

    foreach (const QString& subKey, subKeys)
    {
        // qDebug()<< subKey;
        if (subKey.contains("WhatsAppDesktop"))
        {
            QString keyPath = parentKeyPath + "\\" + subKey;
            QSettings skySettings(keyPath, QSettings::NativeFormat);
            skySettings.setValue(valueName, valueData);
            break;
        }
    }


}

QMObject::QMObject(QObject *parent) : QObject{parent}
{
    hide_pause = false;
    native_connected = false;
    out_going = false;
    bcall_screen = false;
    bnow_call = false;
    bincomming = false;
    bprev_call = false;
    bprev_income = false;
}

LRESULT CALLBACK ChildWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}

void RegisterChildWindowClass() {
    HINSTANCE hInstance = GetModuleHandle(NULL);
    WNDCLASS wc = {};
    wc.lpfnWndProc = ChildWndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"CustomChildWindow";
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); // Black background

    RegisterClass(&wc);
}

HWND CreateChildWindow(HWND hParent, int x, int y, int w, int h) {
    qDebug()<<"werwer";

    // Create the child window
    HWND hChild = CreateWindowEx(
        0, // Extended window style
        L"CustomChildWindow", // Class name
        L"", // Window text
        WS_CHILD | WS_VISIBLE, // Styles
        x, y, w, h, // x, y, width, height
        hParent, // Parent window's HWND
        NULL, // Menu
        GetModuleHandle(NULL), // Instance handle
        NULL); // Additional application data

    return hChild;
}


// Callback function to enumerate child windows
BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam) {
    HWND hNewParent = (HWND)lParam;
    SetParent(hwnd, hNewParent); // Reparent the child window
    return TRUE;
}

// Function to reparent existing window's children to a new parent window
void ReparentWindowControls(HWND hwnd, HWND hNewParent) {
    if (hwnd) {
        // EnumChildWindows(hwnd, EnumChildProc, (LPARAM)hNewParent);
        SetParent(hwnd, hNewParent);
    }
    // SetParent(hwnd, hNewParent);
}

WNDPROC originalWndProc = nullptr;

// Custom window procedure to handle messages
LRESULT CALLBACK CustomWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    // Handle the messages you're interested in
    qDebug()<<"====="<<msg;
    switch (msg) {
    case WM_CLOSE:
        qDebug() << "Window is being closed.";
        // Do additional processing if needed
        break;
    default:
        // Call the original window procedure for other messages
        return CallWindowProc(originalWndProc, hwnd, msg, wParam, lParam);
    }
    // Return 0 for messages that are handled
    return 0;
}

void subclassWindow(HWND hwnd) {
    // Subclass the window by replacing its window procedure
    originalWndProc = (WNDPROC)SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)CustomWndProc);
}

// Function to restore the original window procedure
void restoreWindowProc(HWND hwnd) {
    // Restore the original window procedure
    SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)originalWndProc);
}


void drawRectangle(HWND hwnd, int left, int top, int right, int bottom) {
    // Get the device context of the window
    HDC hdc = GetDC(hwnd);

    // Define the rectangle coordinates
    RECT rect = {left, top, right, bottom};

    // Create a black brush
    HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));

    // Fill the rectangle with the black brush
    FillRect(hdc, &rect, hBrush);

    // Release the brush
    DeleteObject(hBrush);

    // Release the device context
    ReleaseDC(hwnd, hdc);
}

void modifyWindowStyles(HWND hwnd) {
    // Retrieve the current window style
    LONG_PTR style = GetWindowLongPtr(hwnd, GWL_STYLE);

    // Remove the close button and sizing border
    style &= ~WS_MINIMIZEBOX;
    style &= ~WS_THICKFRAME;
    style &= ~WS_SYSMENU;

    // Apply the modified style
    SetWindowLongPtr(hwnd, GWL_STYLE, style);
}

int getProcessStatus(const QString& processName){
    int a = 0;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32 entry;
        entry.dwSize = sizeof(PROCESSENTRY32);

        if (Process32First(snapshot, &entry)) {
            do {
                QString currentProcessName = QString::fromWCharArray(entry.szExeFile);
                if (currentProcessName.compare(processName, Qt::CaseInsensitive) == 0) {
                    // qDebug()<<entry.th32ProcessID;
                    a++;
                    break;
                }
            } while (Process32Next(snapshot, &entry));

        }

        CloseHandle(snapshot);
    }
    if(a == 0){
        // qDebug()<<"exit";
        return 0;
    } else {
        return 1;
    }
}

bool getp = false;
QProcess* getProcessObjectFromPID(HWND hwnd) {
    QProcess* process = nullptr;
    DWORD processId;
    DWORD threadId = GetWindowThreadProcessId(hwnd, &processId);
    if (threadId == 0) {
        return nullptr;
    }
    if (processId != 0) {
        HANDLE handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, static_cast<DWORD>(processId));
        if (handle) {
            DWORD pathSize = MAX_PATH;
            char path[MAX_PATH];

            if (QueryFullProcessImageNameA(handle, 0, path, &pathSize)) {
                qDebug()<<path;
                process = new QProcess();
                process->setProgram(QString::fromUtf16(reinterpret_cast<const ushort*>(path)));
            }

            CloseHandle(handle);
        }

        return process;
    }
    return nullptr;
}

HWND g_hExistingWindow = nullptr;
HWND callchild = nullptr;
void QMObject::doWork() {

    const char* targetCaption = "WhatsApp";
    // const char* targetCaption = "Untitled - Notepad";
    const char* VOICECALL =   "Voice call ?- WhatsApp";
    HWND targetWindow = nullptr;
    mobj = this;
    is_income_clicked = 0;
    bnow_call = false;
    bcall_screen = false;
    g_hExistingWindow = nullptr;
    RegisterChildWindowClass(); // Ensure the class is registered

    do{
        Sleep(10);
        targetWindow = FindWindowA(nullptr, (LPCSTR)targetCaption);
        setWhatsAppSubkeys();
        if(targetWindow == nullptr){
            getp = false;
            bnow_call = false;
            is_income_clicked = 0;
            if(g_hExistingWindow != nullptr){
                DestroyWindow(g_hExistingWindow);
                g_hExistingWindow = nullptr;

            }
            qDebug()<<"111111";
            QString url = "whatsapp://send/";
            QDesktopServices::openUrl(QUrl(url));
            Sleep(1000);
        } else {

            subclassWindow(targetWindow);
            RECT rect;
            GetWindowRect(targetWindow, &rect);
            if(rect.left == 0){
                if(g_hExistingWindow != nullptr){
                    // qDebug()<<"hide";
                    // bnow_call = false;
                    // if(g_hExistingWindow != nullptr){
                    //     DestroyWindow(g_hExistingWindow);
                    //     g_hExistingWindow = nullptr;

                    // }
                    is_income_clicked = 0;
                }
            } else {
                if(!getProcessStatus("WhatsApp.exe")){
                    DestroyWindow(g_hExistingWindow);
                    DestroyWindow(targetWindow);
                }
                if(!bnow_call){
                    if(g_hExistingWindow == nullptr)
                    {
                        g_hExistingWindow = CreateChildWindow(targetWindow, 0, 0, rect.right-rect.left-OFFMAINX, rect.bottom-rect.top);

                        // ReparentWindowControls(g_hExistingWindow, targetWindow);
                        // SetWindowPos( g_hExistingWindow, HWND_TOP,rect.left,rect.top, rect.right - rect.left-200, rect.bottom - rect.top, 0);
                    }
                    // ReparentWindowControls(g_hExistingWindow, targetWindow);
                    SetFocus(targetWindow);
                    SetForegroundWindow(g_hExistingWindow);
                    char str[1000];
                    GetWindowTextA(g_hExistingWindow, str, 1000);
                    qDebug()<<str;
                    is_income_clicked = 0;
                    bnow_call = true;
                } else {
                    if(is_income_clicked < CNTSHOW){
                        if(g_hExistingWindow != nullptr){
                            Sleep(CNTSLEEP);
                            ShowWindow(g_hExistingWindow, SW_SHOW);
                            // ReparentWindowControls(g_hExistingWindow, targetWindow);
                            // MoveWindow(g_hExistingWindow, 0, 0, rect.right-rect.left-OFFMAINX, rect.bottom-rect.top, false);
                            // SetWindowPos(g_hExistingWindow, HWND_TOPMOST,0,0, rect.right - rect.left-200, rect.bottom - rect.top-200, SWP_NOACTIVATE);
                            // UpdateWindow(targetWindow);
                            SetForegroundWindow(g_hExistingWindow);
                            SetFocus(targetWindow);
                            is_income_clicked++;
                        }
                    } else {
                        // is_income_clicked = 0;
                    }
                }
                //getProcessStatus(targetWindow);
                // qDebug()<<"call-"<<targetWindow;
                if(g_hExistingWindow != nullptr){
                    MoveWindow(g_hExistingWindow, 0, 0, rect.right-rect.left-OFFMAINX, rect.bottom-rect.top, false);
                    SetFocus(targetWindow);
                    SetForegroundWindow(g_hExistingWindow);
                }
            }

            ncallwindow = false;
            EnumWindows(HideCallWindow, 0);
            if(ncallwindow){
                // qDebug()<<"ncallwindow-";
                if(bcallwindow == false){
                    // qDebug()<<"ncallwindow-";
                    void* hwndVoidPtr = reinterpret_cast<void*>(callwnd);
                    int hwndInt = reinterpret_cast<intptr_t>(hwndVoidPtr);
                    QFile file("output.txt");
                    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                        QTextStream stream(&file);
                        stream << hwndInt;
                        file.close();
                        qDebug() << "String saved to file successfully!";
                    } else {
                        qDebug() << "Failed to open file for writing.";
                    }
                    // QMessageBox::information(nullptr, "", QString::number(hwndInt));
                    if(callchild != nullptr){
                        DestroyWindow(callchild);
                    }
                    RECT rt;
                    GetWindowRect(callwnd, &rt);
                    callchild = CreateChildWindow(callwnd, 0, 0, rect.right-rect.left, rect.bottom-rect.top);
                    qDebug()<<"call-"<<callchild;
                    ReparentWindowControls(callchild, callwnd);
                    bcallwindow = true;
                }

            } else {
                // qDebug()<<"nowncallwindow-";
                bcallwindow = false;
                if(callchild != nullptr){
                    DestroyWindow(callchild);
                }
            }
            // checkIncoming();
            // GetWindowRect(targetWindow, &rect);
            // MoveWindow(g_hExistingWindow,rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,false);

            // SetWindowPos(g_hExistingWindow, HWND_TOPMOST, 0 , 0, 300, 300, SWP_NOACTIVATE);
            // UpdateWindow(targetWindow);
            // UpdateOverlayPosition();
            // if(!native_connected || !out_going){
            //     EnumWindows(HideCallWindow, 0);
            // }
            // if(!native_connected || !hide_pause){
            //     RECT rt;
            //     GetWindowRect(targetWindow, &rt);
            //     if(rt.left == 0){
            //         QString url = "whatsapp://send/";
            //         QDesktopServices::openUrl(QUrl(url));
            //     }
            //     WINDOWPLACEMENT wp;
            //     wp.length = sizeof(WINDOWPLACEMENT);
            //     // hideWindow(targetWindow);

            //     // if (GetWindowPlacement(targetWindow, &wp)) {
            //     //     switch (wp.showCmd) {
            //     //     case SW_SHOWNORMAL:
            //     //         hideWindow(targetWindow);
            //     //         break;
            //     //     case SW_SHOWMINIMIZED:
            //     //         hideWindow(targetWindow);
            //     //         break;
            //     //     case SW_SHOWMAXIMIZED:
            //     //         hideWindow(targetWindow);
            //     //         break;
            //     //     // Add more cases as needed
            //     //     default:
            //     //         hideWindow(targetWindow);
            //     //         break;
            //     //     }
            //     // } else {
            //     //     hideWindow(targetWindow);
            //     // }
            // }
            // bnow_call = false;
            // EnumWindows(CheckCallWindow, 0);
            // if(bnow_call){
            //     if(!bcall_screen){
            //         bcall_screen = true;
            //         qDebug()<<bnow_call<<" "<<bcall_screen;
            //         emit callwindow();
            //     } else {

            //     }
            //     // emit callwindow();
            // } else {
            //     if(bcall_screen){
            //         bcall_screen = false;
            //         emit callwindowhide();
            //     } else {

            //     }
            //     // emit callwindowhide();
            // }
            // HWND incomingWindow = checkIncoming();
            // if(incomingWindow != nullptr){
            //     if(!bincomming){
            //         bprev_income = bincomming;
            //         bincomming = true;
            //         qDebug()<<"set";
            //         emit incoming();
            //         qDebug()<<"set incomming";
            //         HideIncomingWindow(incomingWindow);
            //         qDebug()<<"seted";
            //     }
            //     // emit incoming();
            // } else {
            //     is_income_clicked = 0;
            //     if(bincomming){
            //         bprev_income = bincomming;
            //         bincomming = false;
            //         qDebug()<<"set";
            //         emit incominghide();

            //         qDebug()<<"set incomming";
            //         // HideIncomingWindow(incomingWindow);
            //         qDebug()<<"seted";
            //     }
            //     // emit incominghide();
            // }
        }
    }while(1);
    emit finished();
}
void QMObject::hideWindow(HWND targetWindow){
    ShowWindow(targetWindow, SW_HIDE);
    setWindowTransparency(targetWindow, ALPHAVALUE);
    MoveWindow(targetWindow, MOVERECTX,MOVERECTY,MOVERECTW,MOVERECTH,FALSE);
}
