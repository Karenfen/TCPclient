#pragma once

#include <QObject>
#include <memory>
#include <QTcpSocket>


class clientImpl : public QObject
{
    Q_OBJECT
public:
    ~clientImpl() = default;

protected:
    clientImpl(QObject *parent = nullptr);
    void connect_to_host(const std::string& address, const uint16_t& port);
    bool is_connected();
    void disconnect_host();
    void run_session();

private:
    void data_handling_(const QByteArray& data);

private slots:
    void send_request();
//    void request_sent(qint64 bytes);
//    void receive_response();
    void exit();

signals:
//    void request_ready();

private:
    std::unique_ptr<QTcpSocket> socket_ptr_{};
    std::string request_{};

    friend class TCPclient;
};

