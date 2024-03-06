#include "qmserver.h"
#include <QMessageBox>
#include <QTime>
#include <QDebug>
QMServer::QMServer(QString servername,QObject *parent)
    : QObject{parent} {
    m_servername = servername;
    m_socket = nullptr;
}
void QMServer::doWork(){
    localServer = new QLocalServer(this);
    if(!localServer->listen(m_servername))
    {
        localServer->close();
        return;
    }
    connect(localServer, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));
}
void QMServer::slotNewConnection()
{
    QLocalSocket* localSocket = localServer->nextPendingConnection();
    qDebug() << "new connection";
    connect(localSocket, SIGNAL(disconnected()), this, SLOT(slotDisconncted()));
    connect(localSocket, SIGNAL(readyRead()), this, SLOT(slotReadClient()));
    sendToClient(localSocket, "Server response: Connected!");
    m_socket = localSocket;
    m_obj->setNativeConnected(true);

}

void QMServer::slotDisconncted(QLocalSocket *pSocket){
    qDebug() << "disconnected";
    m_obj->setNativeConnected(false);
    m_socket = nullptr;
    pSocket->deleteLater();
}


void QMServer::slotReadClient()
{
    QLocalSocket* localSocket = (QLocalSocket*)sender();
    QDataStream in(localSocket);
    in.setVersion(QDataStream::Qt_5_3);
    for(;;)
    {
        if(!nextBlockSize)
        {
            if(localSocket->bytesAvailable() < (int)sizeof(quint16))
                break;
            in >> nextBlockSize;
        }

        QTime time;
        QString string;
        in >> time >> string;
        QString message = string;

        qDebug()<<message;
        nextBlockSize = 0;
        if(QString::compare(message, "pausehide") == 0){
            m_obj->setPause(true);
        } else if(QString::compare(message, "playhide") == 0){
            m_obj->setPause(false);
        }else if(QString::compare(message, "outgoing") == 0){
            m_obj->setOutgonig(true);
        }else if(QString::compare(message, "outgoingplay") == 0){
            m_obj->setOutgonig(false);
        }
    }
}

void QMServer::sendToClient(QLocalSocket* localSocket, const QString& string)
{
    if(localSocket == nullptr) return;
    QByteArray array;
    QDataStream out(&array, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);
    out << quint16(0)<< string;

    out.device()->seek(0);
    out << quint16(array.size() - sizeof(quint16));
    localSocket->write(array);
}

void QMServer::slotCallWindow()
{
    if(m_socket != nullptr){
        sendToClient(m_socket, "callwindow");
    }
    qDebug()<<"callwindow";
}

void QMServer::slotCallWindowHide()
{
    if(m_socket != nullptr){
        sendToClient(m_socket, "callwindowhide");
    }
    qDebug()<<"callwindowhide";
}
void QMServer::slotIncoming()
{
    if(m_socket != nullptr){
        sendToClient(m_socket, "incoming");

    }
    qDebug()<<"incomming";
}
void QMServer::slotIncomingHide()
{
    if(m_socket != nullptr){
        sendToClient(m_socket, "incominghide");

    }
    qDebug()<<"incominghide";
}
