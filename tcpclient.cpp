#include "tcpclient.h"

#include <exception>
#include <cerrno>
#include <iostream>



TCPclient::TCPclient(): impl_(new clientImpl)
{
    if(!impl_)
        throw std::runtime_error("Initialization error!");
}


bool TCPclient::connect_to_host(const std::string& address, const uint16_t& port)
{
    try
    {
        impl_->connect_to_host(address, port);
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return false;
    }

    return true;
}


void TCPclient::disconnect_host()
{
    impl_->disconnect_host();
}


bool TCPclient::is_connected()
{
    return impl_->is_connected();
}


void TCPclient::run_session()
{
    impl_->run_session();
}

