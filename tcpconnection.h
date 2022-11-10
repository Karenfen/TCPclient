#pragma once


#include <QObject>
#include <QTcpSocket>
#include <memory>


#define __MAXIMUM_WAITING_TIME 3000


class TCPconnection : public QObject
{
    Q_OBJECT
public:
    explicit TCPconnection(QObject *parent = nullptr);
    ~TCPconnection(){ abort(); }
    bool connectTo(const QHostAddress& Address, uint16_t port, QIODevice::OpenMode openMode = QIODevice::OpenModeFlag::ReadWrite);
    bool _disconnect();
    const std::string& getLastError(){ return lastError; }
    bool isConnected();
    bool read(std::string& buffer);
    bool write(const std::string& data);
    void abort();

signals:
    void disconnected();

private slots:
    void close();

private:
    std::unique_ptr<QTcpSocket> socketPtr_{};
    std::string lastError{};
};
