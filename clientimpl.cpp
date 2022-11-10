#include "clientimpl.h"

#include <QString>
#include <QHostAddress>
#include <QHostInfo>
#include <iostream>
#include <QApplication>




clientImpl::clientImpl(QObject *parent) : QObject(parent),
    connection_(std::make_unique<TCPconnection>())
{
    if(!connection_)
        throw std::runtime_error("Initialization error!");

    connect(connection_.get(), &TCPconnection::disconnected, this, &clientImpl::exit);
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

        if(!connection_->connectTo(host_address, port))
            throw std::logic_error(connection_->getLastError());
    }
    else
    {
        for(const QHostAddress& address : addresses)
        {
            std::cout << "Trying to connect to " << address.toString().toStdString() << "/" << port << std::endl;

            if(connection_->connectTo(address, port))
                break;
            else
                std::cerr << "Connection timed out!" << std::endl;
        }
    }

    if(!connection_->isConnected())
        throw std::logic_error("Connection error!");
}


void clientImpl::disconnect_host()
{
    if(!connection_->_disconnect())
        throw std::logic_error("Disconnection error!");
}


void clientImpl::run_session()
{
    std::cout << "session started!" << std::endl;

    while (connection_->isConnected())
    {      
        if(!send_request())
            continue;


        std::cout << "Request sent. Size (" << request_.length() << " bytes)" << std::endl;
        std::cout << "Waiting an answer..." << std::endl;

        std::string answer;

        if(!connection_->read(answer))
        {
            std::cerr << connection_->getLastError() << std::endl;
            continue;
        }

        std::cout << "answer received" << std::endl;

        data_handling_(answer);
    }

}


void clientImpl::data_handling_(const std::string &data)
{
    std::cout << "Response received!\n" << data << std::endl;
}


bool clientImpl::send_request()
{
    std::getline(std::cin, request_);

    std::cout << "sending a request..." << std::endl;

    if(!connection_->write(request_))
    {
        std::cerr << connection_->getLastError() << std::endl;
        return false;
    }

    return true;
}


bool clientImpl::is_connected()
{
    return connection_->isConnected();
}



void clientImpl::exit()
{
    connection_->abort();

    std::cout << "Connection to the server was interrupted!" << std::endl;

    qApp->quit();
}
