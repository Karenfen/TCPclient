#pragma once

#include <QObject>
#include <memory>

#include "tcpconnection.h"


class clientImpl : public QObject
{
    Q_OBJECT
public:
    ~clientImpl() = default;

protected:
    clientImpl(QObject *parent = nullptr);
    void connect_to_host(const std::string& address, const uint16_t& port);
    void disconnect_host();
    void run_session();
    bool send_request();
    bool is_connected();

private:
    void data_handling_(const std::string& data);

private slots:
    void exit();

private:
    std::unique_ptr<TCPconnection> connection_{};
    std::string request_{};

    friend class TCPclient;
};

