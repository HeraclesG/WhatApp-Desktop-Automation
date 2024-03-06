#ifndef QMOBJECT_H
#define QMOBJECT_H

#include <QObject>
#include <Windows.h>
#include <QUrl>
#include <QDesktopServices>
#include <QSettings>
#include <QDir>
#include <QMessageBox>
#include <QDebug>
#include <QMutex>

#define MOVERECTX 20000
#define MOVERECTY 10000
#define MOVERECTW 700
#define MOVERECTH 500
#define ALPHAVALUE 1

// #define MOVERECTX 0
// #define MOVERECTY 0
// #define MOVERECTW 700
// #define MOVERECTH 500
// #define ALPHAVALUE 100

class QMObject : public QObject
{
    Q_OBJECT
public:
    QMObject(QObject *parent = nullptr);
private:
    bool isHide;
    bool native_connected;
    bool hide_pause;
    bool out_going;
    bool bcall_screen;
    bool bprev_call;
    bool bprev_income;
    bool bnow_call;
    // HWND g_hExistingWindow;

public:
    void emitCallWindow();
    void emitCallHide();
public slots:
    void recvParam(){
        qDebug()<<"recv param";
    }
    void setParam(){
        qDebug()<<"recv param";
        isHide = true;
    }
    void setPause(bool flag){
        qDebug()<<"hide"<<flag;
        hide_pause = flag;
    }
    void setOutgonig(bool flag){
        qDebug()<<"hide"<<flag;
        out_going = flag;
    }
    void setNativeConnected(bool flag){
        qDebug()<<"native connected"<<flag;
        native_connected = flag;
    }
public slots:
    void doWork();
    void hideWindow(HWND targetWindow);
signals:
    void finished();
    void incoming();
    void callwindow();
    void incominghide();
    void callwindowhide();
};

#endif // QMOBJECT_H
