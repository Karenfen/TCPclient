#pragma once



#include <memory>
#include "clientimpl.h"



class TCPclient
{

public:
    TCPclient();
    ~TCPclient() = default;
    bool connect_to_host(const std::string& address, const uint16_t& port);
    void disconnect_host();
    bool is_connected();
    void run_session();

private:
    std::unique_ptr<clientImpl> impl_;

};
