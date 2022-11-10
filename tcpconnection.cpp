#include "tcpconnection.h"



TCPconnection::TCPconnection(QObject *parent) : QObject(parent),
    socketPtr_(std::make_unique<QTcpSocket>())
{
    if(!socketPtr_)
        throw std::runtime_error("Initialization error!");

    socketPtr_->setSocketOption(QAbstractSocket::LowDelayOption, 0);

    connect(socketPtr_.get(), &QTcpSocket::disconnected, this, &TCPconnection::close);
}


bool TCPconnection::connectTo(const QHostAddress& Address, uint16_t port, QIODevice::OpenMode openMode)
{
    socketPtr_->connectToHost(Address, port, openMode);

    if(!socketPtr_->waitForConnected(__MAXIMUM_WAITING_TIME))
    {
        lastError = "Connection timed out!";
        return false;
    }

    return true;
}


bool TCPconnection::_disconnect()
{
    socketPtr_->disconnectFromHost();

    if(socketPtr_->waitForDisconnected())
        return true;

    lastError = "Disconnection timed out!";
    return false;
}


bool TCPconnection::isConnected()
{
    return (socketPtr_->state() == QAbstractSocket::ConnectedState);
}


bool TCPconnection::read(std::string &buffer)
{
    if(!socketPtr_->waitForReadyRead())
    {
        lastError = "Reading timed out";
        return false;
    }

    buffer = socketPtr_->readAll().toStdString();

    return true;
}


bool TCPconnection::write(const std::string &data)
{
    if(socketPtr_->write(QByteArray(data.c_str())) == -1)
    {
        lastError = "Write error!";
        return false;
    }

    if(!socketPtr_->waitForBytesWritten())
    {
        lastError = "Writing timed out!";
        return false;
    }

    return true;
}


void TCPconnection::close()
{
    emit disconnected();
}


void TCPconnection::abort()
{
    this->disconnect();
    socketPtr_->abort();
}
