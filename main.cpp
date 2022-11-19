#include <QCoreApplication>
#include <iostream>

#include "tcpclient.h"


int main(int argc, char *argv[])
{;

    if(argc < 2 || argc > 4)
    {
        std::cout << "Usage: " << argv[0] << "  <address>  <port> \n"
                  "or: " << argv[0] << " <address>  <port> <client type>\n"
                  "For more information enter: " << argv[0] << " -H" << std::endl;
        return EXIT_FAILURE;
    }

    if(argc == 2)
    {
        Error::help(argv[1]);
        return EXIT_SUCCESS;
    }

    QCoreApplication a(argc, argv);

    TCPclient client;
    bool res;

    if(argc == 4)
        res = client.init(std::stoi(argv[3]));
    else
        res = client.init();

    if(!res)
         return EXIT_FAILURE;

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

