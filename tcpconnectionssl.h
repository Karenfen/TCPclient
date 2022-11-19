#pragma once

#include <QObject>
#include <QTcpSocket>
#include <memory>
#include <openssl/ssl3.h>
#include <openssl/err.h>
#include "iconnection.h"





class TCPconnectionSSL : public QObject, public Iconnection
{
    Q_OBJECT
public:
    TCPconnectionSSL(QObject *parent = nullptr);
    ~TCPconnectionSSL() override;
    bool connectTo(const QHostAddress& Address, uint16_t port) override;
    bool _disconnect() override;
    const std::string& getLastError() override { return lastError; }
    std::string getSSLerror();
    bool isConnected() override;
    std::string read() override;
    bool write(const std::string& data) override;
    void abort() override;

signals:
    void disconnected();

private slots:
    void close();

private:
    std::unique_ptr<QTcpSocket> socketPtr_{};
    SSL* SSLptr_{};
    std::string lastError{};
};

