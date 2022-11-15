#include "tcpclient.h"
#include "tcpconnection.h"

#include <QString>
#include <QHostAddress>
#include <QHostInfo>
#include <iostream>
#include <QApplication>




TCPclient::TCPclient(QObject *parent) : QObject(parent),
    connection_(new TCPconnection())
{
    if(!connection_)
        throw std::runtime_error("Initialization error!");

    connect(dynamic_cast<TCPconnection*>(connection_.get()), &TCPconnection::disconnected, this, &TCPclient::exit);
    connect(this, &TCPclient::close_app, this, [](int exit_code){qApp->exit();});
}


void TCPclient::connect_to_host(const std::string &address, const uint16_t& port)
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
                Error::ErrorHandling("Connection timed out!");
        }
    }

    if(!connection_->isConnected())
        throw std::logic_error("Connection error!");
}


void TCPclient::disconnect_host()
{
    if(!connection_->_disconnect())
        throw std::logic_error("Disconnection error!");
}


void TCPclient::run_session()
{
    std::cout << "session started!" << std::endl;

    while (connection_->isConnected())
    {
        if(!send_request())
            break;

        std::cout << "Request sent." << std::endl;

        std::string answer;
        std::cout << "Waiting an answer..." << std::endl;

        if(!recive_data(answer))
            break;

        std::cout << "Received (" << answer.size() << ") bytes." << std::endl;

        data_handling(answer);
    }

    std::cout << "Sission ended!" << std::endl;
}


void TCPclient::data_handling(std::string data)
{
    std::cout << data << std::endl;
}


bool TCPclient::send_request()
{
    std::string request{};

    std::getline(std::cin, request);

    if(!connection_->write(request))
    {
        Error::ErrorHandling(connection_->getLastError());
        return false;
    }

    return true;
}


bool TCPclient::recive_data(std::string &buffer)
{
    buffer = connection_->read();
    if(buffer.empty())
    {
        Error::ErrorHandling(connection_->getLastError());
        return false;
    }

    return true;
}


bool TCPclient::is_connected()
{
    return connection_->isConnected();
}



void TCPclient::exit()
{
    connection_->abort();

    std::cout << "Connection to the server was interrupted!" << std::endl;

    qApp->quit();
}
