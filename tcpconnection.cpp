#include "tcpconnection.h"



TCPconnection::TCPconnection(QObject *parent) : QObject(parent),
    socketPtr_(std::make_unique<QTcpSocket>())
{
    if(!socketPtr_)
        throw std::runtime_error("Initialization error!");

    socketPtr_->setSocketOption(QAbstractSocket::LowDelayOption, 0);

    connect(socketPtr_.get(), &QTcpSocket::disconnected, this, &TCPconnection::close);
    connect(socketPtr_.get(), QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
            [=](QAbstractSocket::SocketError socketError){this->lastError = socketPtr_->errorString().toStdString();});
}


bool TCPconnection::connectTo(const QHostAddress& Address, uint16_t port)
{
    socketPtr_->connectToHost(Address, port);

    if(socketPtr_->waitForConnected(__MAXIMUM_WAITING_TIME))
        return true;

    lastError = socketPtr_->errorString().toStdString();
    return false;
}


bool TCPconnection::_disconnect()
{
    socketPtr_->disconnectFromHost();

    if(socketPtr_->waitForDisconnected(__MAXIMUM_WAITING_TIME))
        return true;

    lastError = socketPtr_->errorString().toStdString();
    return false;
}


bool TCPconnection::isConnected()
{
    return (socketPtr_->state() == QAbstractSocket::ConnectedState);
}


std::string TCPconnection::read()
{
    std::string result{};
    if(!socketPtr_->waitForReadyRead(__MAXIMUM_WAITING_TIME))
    {
        lastError = socketPtr_->errorString().toStdString();
        return result;
    }

    result = socketPtr_->readAll().toStdString();

    return result;
}


bool TCPconnection::write(const std::string &data)
{
    if(socketPtr_->write(QByteArray(data.c_str())) == -1)
    {
        lastError = socketPtr_->errorString().toStdString();
        return false;
    }

    if(!socketPtr_->waitForBytesWritten(__MAXIMUM_WAITING_TIME))
    {
        lastError = socketPtr_->errorString().toStdString();
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
