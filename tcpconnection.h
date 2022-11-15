#pragma once

#include <QObject>
#include <QTcpSocket>
#include <memory>
#include "Iconnection.h"




class TCPconnection : public QObject, public Iconnection
{
    Q_OBJECT
public:
    TCPconnection(QObject *parent = nullptr);
    ~TCPconnection() override { abort(); }
    bool connectTo(const QHostAddress& Address, uint16_t port) override;
    bool _disconnect() override;
    const std::string& getLastError() override { return lastError; }
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
    std::string lastError{};
};
