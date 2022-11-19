#include "tcpconnectionssl.h"



TCPconnectionSSL::TCPconnectionSSL(QObject *parent) : QObject(parent),
    socketPtr_(std::make_unique<QTcpSocket>()), SSLptr_(nullptr)
{
    if(!socketPtr_)
        throw std::runtime_error("Initialization error!");

    socketPtr_->setSocketOption(QAbstractSocket::LowDelayOption, 0);

    connect(socketPtr_.get(), &QTcpSocket::disconnected, this, &TCPconnectionSSL::close);
    connect(socketPtr_.get(), QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
            [=](QAbstractSocket::SocketError socketError){this->lastError = socketPtr_->errorString().toStdString();});

    SSL_library_init();
    SSLeay_add_ssl_algorithms();
    SSL_load_error_strings();

    const SSL_METHOD *SSLmeth = TLS_client_method();
    SSL_CTX* ssl_ctx = SSL_CTX_new(SSLmeth);
    SSLptr_ = SSL_new(ssl_ctx);

    if(!SSLptr_)
        throw std::runtime_error(getSSLerror());
}


TCPconnectionSSL::~TCPconnectionSSL()
{
    this->disconnect();
    SSL_shutdown(SSLptr_);
    socketPtr_->abort();
}


bool TCPconnectionSSL::connectTo(const QHostAddress& Address, uint16_t port)
{    
    socketPtr_->connectToHost(Address, port);

    if(socketPtr_->waitForConnected(__MAXIMUM_WAITING_TIME))
    {
        SSL_set_fd(SSLptr_, socketPtr_->socketDescriptor());

        if(SSL_connect(SSLptr_) <= 0)
        {
            lastError = getSSLerror();
            return false;
        }
        return true;
    }

    lastError = "Connection timed out!";
    return false;
}


bool TCPconnectionSSL::_disconnect()
{
    if(SSL_shutdown(SSLptr_))
        return true;

    lastError = getSSLerror();
    return false;
}


std::string TCPconnectionSSL::getSSLerror()
{
    auto errorCode = ERR_get_error();
    std::string errorStr = ERR_error_string(errorCode, nullptr);
    return errorStr;
}


bool TCPconnectionSSL::isConnected()
{
    return (socketPtr_->state() == QAbstractSocket::ConnectedState);
}


std::string TCPconnectionSSL::read()
{
    std::string result{};
    char buffer[__MAX_BUFFER_SIZE];
    size_t reciveBites = 0;

    while(SSL_read_ex(SSLptr_, buffer, __MAX_BUFFER_SIZE, &reciveBites))
    {
        result.append(buffer, reciveBites);
    }

    if(result.size() == 0)
        lastError = getSSLerror();

    return result;
}


bool TCPconnectionSSL::write(const std::string &data)
{
    if(SSL_write(SSLptr_, data.c_str(), data.size()) > 0)
        return true;

    lastError = getSSLerror();
    return false;
}


void TCPconnectionSSL::close()
{
    emit disconnected();
}


void TCPconnectionSSL::abort()
{
    this->disconnect();
    SSL_shutdown(SSLptr_);
    socketPtr_->abort();
}

