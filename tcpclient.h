#pragma once

#include <QObject>
#include <memory>
#include <iostream>
#include "Iconnection.h"


namespace Error
{
    static void ErrorHandling(const std::string& error)
    {
        std::cerr << "ERROR! : " << error << std::endl;
    }

    static void help(const std::string& parameter)
    {
        if(parameter == "-H")
        {
            std::cout << "Additional call options:\n"
                      << "\t 0 \t - \t Qt client.\n"
                      << "\t 1 \t - \t Qt client whith SSL.\n"
                      << "\t 2 \t - \t cURL client.\n"
                      <<std::endl;
        }
    }
}

namespace Connection {
    enum TYPE { QTCPSOCK, QTCPSSL, CURL };
}



class TCPclient : public QObject
{
    Q_OBJECT
public:
    ~TCPclient() = default;
    TCPclient(QObject *parent = nullptr);
    bool init(int type = 0);
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
};
