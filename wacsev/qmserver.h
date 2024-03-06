#ifndef QMSERVER_H
#define QMSERVER_H

#include <QObject>
#include <QLocalServer>
#include <QLocalSocket>
#include "qmobject.h"
#include <QMutex>

class QMServer: public QObject
{
    Q_OBJECT
public:
    QMServer(QString servername,QObject *parent = nullptr);
    void sendToClient(QLocalSocket* localSocket, const QString& string);
private:
    QLocalServer* localServer;
    quint16 nextBlockSize;
    QString m_servername;
    QMObject* m_obj;
    QMutex mutex;
    QLocalSocket* m_socket;
public slots:
    virtual void slotNewConnection();

    // Слот чтения информации от клиента
    void slotReadClient();
    void slotDisconncted(QLocalSocket* pSocket);
    void slotIncoming();
    void slotCallWindow();
    void slotIncomingHide();
    void slotCallWindowHide();
public slots:
    void doWork();
    void setMObj(QMObject* obj){
        m_obj = obj;
    }

signals:
    void finished();
    void recv_param();
};

#endif // QMSERVER_H
