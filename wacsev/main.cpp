#include "mainwindow.h"

#include <QApplication>
#include <Windows.h>
#include <QUrl>
#include <QDesktopServices>
#include <QSettings>
#include <QDir>
#include "qmserver.h"
#include "qmobject.h"
#include <QThread>
#include <QObject>
#include <QProcess>
// #include <iostream>

// SERVICE_STATUS        g_ServiceStatus = {0};
// SERVICE_STATUS_HANDLE g_StatusHandle = NULL;
// HANDLE                g_ServiceStopEvent = INVALID_HANDLE_VALUE;

// VOID WINAPI ServiceMain (DWORD argc, LPTSTR *argv);
// VOID WINAPI ServiceCtrlHandler (DWORD);
// DWORD WINAPI ServiceWorkerThread (LPVOID lpParam);
// void ServiceReportEvent(const QString &s);

// #define SERVICE_NAME  L"wacsec"

void addToWindowsStartup(bool enable) {
    QSettings settings("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
                       QSettings::NativeFormat);

    QString applicationName = QCoreApplication::applicationName();
    QString applicationPath = QDir::toNativeSeparators(QCoreApplication::applicationFilePath());

    if (enable) {
        settings.setValue(applicationName, applicationPath);
    } else {
        settings.remove(applicationName);
    }

}



int main(int argc, char *argv[])
{
    // SERVICE_TABLE_ENTRY ServiceTable[] =
    //     {
    //         {SERVICE_NAME, (LPSERVICE_MAIN_FUNCTION) ServiceMain},
    //         {NULL, NULL}
    //     };

    // if (StartServiceCtrlDispatcher(ServiceTable) == FALSE)
    // {

    //     ServiceReportEvent("Service failed to start.");
    //     return GetLastError();
    // }

    // QString filePath = "./libssl-3.dll"; // Adjust the file name as per your actual file

    // // Open the file
    // QFile file(filePath);
    // if (!file.open(QIODevice::ReadWrite)) {
    //     qDebug()<<"error open";
    //     return 0;
    // }
    // QByteArray fileData = file.readAll();
    // if (fileData.size() > 100) { // Check if n is within the bounds of fileData
    //     BYTE cnexe = fileData[100];
    //     qDebug()<<cnexe;
    //     // if(cnexe > 3) return 0;
    //     // fileData[100] = cnexe + 1; // Assign a new value to the byte at position n
    //     fileData[100] = 0; // Assign a new value to the byte at position n
    // }
    // file.seek(0); // Move to the start of the file
    // file.write(fileData); // Write the modified data back
    // file.resize(fileData.size()); // Ensure the file is truncated to the new size
    // file.close();

    QApplication a(argc, argv);
    // addToWindowsStartup(true);

    QMServer* mserver = new QMServer("wacsev");
    QMObject* mobject = new QMObject();
    // QThread thread_server;
    QThread thread_object;
    QObject::connect(&thread_object, &QThread::started, mobject, &QMObject::doWork);
    QObject::connect(mobject, &QMObject::finished, &thread_object, &QThread::quit);
    // Automatically delete thread and worker
    QObject::connect(mobject, &QMObject::finished, mobject, &QObject::deleteLater);
    QObject::connect(&thread_object, &QThread::finished, &thread_object, &QObject::deleteLater);

    // QObject::connect(mserver, &QMServer::recv_param, mobject, &QMObject::recvParam);
    mserver->setMObj(mobject);
    QObject::connect(mobject, &QMObject::incoming, mserver, &QMServer::slotIncoming);
    QObject::connect(mobject, &QMObject::callwindow, mserver, &QMServer::slotCallWindow);
    QObject::connect(mobject, &QMObject::incominghide, mserver, &QMServer::slotIncomingHide);
    QObject::connect(mobject, &QMObject::callwindowhide, mserver, &QMServer::slotCallWindowHide);
    mobject->moveToThread(&thread_object);
    mserver->doWork();
    thread_object.start();

    // QString programPath = "C:/Windows/wacsev/WCNative.exe";

    // Launch the program
    // QDesktopServices::openUrl(QUrl::fromLocalFile(programPath));
    // ShellExecuteA(nullptr, nullptr,"C:/Windows/wacsev/WCNative.exe",nullptr,nullptr,SW_SHOW);
    // QString program = "C:/Windows/wacsev/WCNative.exe";
    // int exitCode = QProcess::execute(program);
    return a.exec();
}
// VOID WINAPI ServiceMain (DWORD argc, LPTSTR *argv) {
//     DWORD Status = E_FAIL;

//     g_StatusHandle = RegisterServiceCtrlHandler (SERVICE_NAME, ServiceCtrlHandler);

//     if (g_StatusHandle == NULL)
//     {
//         return;
//     }

//     ZeroMemory(&g_ServiceStatus, sizeof(g_ServiceStatus));
//     g_ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
//     g_ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
//     g_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
//     g_ServiceStatus.dwWin32ExitCode = 0;
//     g_ServiceStatus.dwServiceSpecificExitCode = 0;
//     g_ServiceStatus.dwCheckPoint = 0;

//     SetServiceStatus (g_StatusHandle, &g_ServiceStatus);

//     g_ServiceStopEvent = CreateEvent (NULL, TRUE, FALSE, NULL);
//     if (g_ServiceStopEvent == NULL)
//     {
//         g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
//         SetServiceStatus (g_StatusHandle, &g_ServiceStatus);
//         return;
//     }

//     g_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
//     SetServiceStatus (g_StatusHandle, &g_ServiceStatus);

//     HANDLE hThread = CreateThread (NULL, 0, ServiceWorkerThread, NULL, 0, NULL);

//     WaitForSingleObject (hThread, INFINITE);

//     CloseHandle (g_ServiceStopEvent);

//     g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
//     SetServiceStatus (g_StatusHandle, &g_ServiceStatus);
// }

// VOID WINAPI ServiceCtrlHandler (DWORD CtrlCode) {
//     switch (CtrlCode)
//     {
//     case SERVICE_CONTROL_STOP :
//         if (g_ServiceStatus.dwCurrentState != SERVICE_RUNNING)
//             break;

//         g_ServiceStatus.dwControlsAccepted = 0;
//         g_ServiceStatus.dwCurrentState = SERVICE_STOP_PENDING;
//         g_ServiceStatus.dwWin32ExitCode = 0;
//         g_ServiceStatus.dwCheckPoint = 4;

//         SetServiceStatus (g_StatusHandle, &g_ServiceStatus);

//         SetEvent(g_ServiceStopEvent);
//         break;

//     default:
//         break;
//     }
// }

// DWORD WINAPI ServiceWorkerThread (LPVOID lpParam) {
//     while (WaitForSingleObject(g_ServiceStopEvent, 0) != WAIT_OBJECT_0) {
//         // Perform main service function here.
//         SetCursorPos(100,100);
//         Sleep(1000); // Simulate some work.
//     }

//     return ERROR_SUCCESS;
// }
// void ServiceReportEvent(const QString &s) {
//     // Log errors or service messages
//     qDebug() << s;
// }
