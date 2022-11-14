#pragma once

#include <QObject>
#include <memory>
#include "iconnection.h"


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
    bool recive_data(std::string& buffer);
    bool is_connected();

signals:
    void close_app(int exit_code);

private slots:
    void exit();
    void data_handling(std::string data);

private:
    std::unique_ptr<Iconnection> connection_{};

    friend class TCPclient;
};

