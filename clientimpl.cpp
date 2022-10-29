#include "clientimpl.h"

#include <QString>
#include <QHostAddress>
#include <QHostInfo>
#include <iostream>
#include <QApplication>

#define __MAXIMUM_WAITING_TIME 3000



clientImpl::clientImpl(QObject *parent) : QObject(parent),
    socket_ptr_(std::make_unique<QTcpSocket>())
{
    if(!socket_ptr_)
        throw std::runtime_error("Socket initialization error!");

    socket_ptr_->setSocketOption(QAbstractSocket::LowDelayOption, 0);

    connect(socket_ptr_.get(), &QTcpSocket::disconnected, this, &clientImpl::exit);
//    connect(socket_ptr_.get(), &QTcpSocket::bytesWritten, this, &clientImpl::request_sent);
//    connect(socket_ptr_.get(), &QTcpSocket::readyRead, this, &clientImpl::receive_response);
//    connect(this, &clientImpl::request_ready, this, &clientImpl::send_request);

}


void clientImpl::connect_to_host(const std::string &address, const uint16_t& port)
{
    std::cout << "server connection..." << std::endl;

    auto addresses = QHostInfo::fromName(address.c_str()).addresses();

    if(addresses.empty())
    {
        QHostAddress host_address(QString(address.c_str()));

        if(host_address.isNull())
            throw std::logic_error("Incorrect address!");

        socket_ptr_->connectToHost(host_address, port);

        if(!socket_ptr_->waitForConnected(__MAXIMUM_WAITING_TIME))
            throw std::logic_error("Connection timed out!");
    }
    else
    {
        for(const QHostAddress& address : addresses)
        {
            socket_ptr_->connectToHost(address, port);

            std::cout << "Trying to connect to " << address.toString().toStdString() << "/" << port << std::endl;

            if(socket_ptr_->waitForConnected(__MAXIMUM_WAITING_TIME))
                break;
            else
                std::cerr << "Connection timed out!" << std::endl;
        }
    }

    if(socket_ptr_->state() != QAbstractSocket::ConnectedState)
        throw std::logic_error("Connection error!");
}


bool clientImpl::is_connected()
{
    return (socket_ptr_->state() == QAbstractSocket::ConnectedState);
}


void clientImpl::disconnect_host()
{
    socket_ptr_->disconnectFromHost();
}


void clientImpl::run_session()
{
    std::cout << "session started!" << std::endl;

    while (is_connected())
    {
        send_request();

        if(!socket_ptr_->waitForBytesWritten())
        {
            std::cerr << "An error occurred while sending the request!" << std::endl;
            continue;
        }

        std::cout << "Request sent. Size (" << request_.length() << " bytes)" << std::endl;
        std::cout << "Waiting an answer..." << std::endl;

        if(!socket_ptr_->waitForReadyRead())
        {
            std::cerr << "Response timed out" << std::endl;
            continue;
        }

        std::cout << "receiving an answer..." << std::endl;

        QByteArray response = socket_ptr_->readAll();

        data_handling_(response);
    }

    //send_request();
}


void clientImpl::data_handling_(const QByteArray &data)
{
    std::cout << "Response received!\n" << data.toStdString() << std::endl;
}


void clientImpl::send_request()
{
    if(socket_ptr_->state() != QAbstractSocket::ConnectedState)
        throw std::logic_error("Connection aborted!");

    std::getline(std::cin, request_);

    std::cout << "sending a request..." << std::endl;

    if(socket_ptr_->write(QByteArray(request_.c_str())) == -1)
        std::cerr << "An error occurred while sending the request!" << std::endl;
}


//void clientImpl::request_sent(qint64 bytes)
//{
//    std::cout << "Request sent. Size (" << bytes << " bytes)" << std::endl;

//    receive_response();
//}


//void clientImpl::receive_response()
//{
//    std::cout << "receiving an answer..." << std::endl;

//    QByteArray response = socket_ptr_->readAll();

//    data_handling_(response);

//    send_request();
//}

void clientImpl::exit()
{
    socket_ptr_->abort();

    std::cout << "Connection to the server was interrupted!" << std::endl;

    qApp->quit();
}
