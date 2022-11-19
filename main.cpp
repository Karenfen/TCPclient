#include <QCoreApplication>
#include <iostream>

#include "tcpclient.h"


int main(int argc, char *argv[])
{;

    if(argc != 3)
    {
        std::cout << "Usage: " << argv[0] << "  <address>  <port> " <<std::endl;
        return EXIT_FAILURE;
    }

    QCoreApplication a(argc, argv);

    TCPclient client;

    client.connect_to_host(argv[1], std::stoi(argv[2]));

    if(client.is_connected())
        client.run_session();
    else
    {
        std::cerr << "Failed to connect to the server " << argv[1] << "/" << argv[2] << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

