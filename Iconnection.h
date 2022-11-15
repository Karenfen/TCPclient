#pragma once


#include <string>

#define __MAXIMUM_WAITING_TIME 10000
#define __MAX_BUFFER_SIZE 256

class QHostAddress;


typedef const std::string& str;
using namespace std;

class Iconnection
{
public:
    virtual ~Iconnection() = default ;
    bool virtual connectTo(const QHostAddress& Address, uint16_t port) = 0;
    bool virtual _disconnect() = 0;
    str virtual getLastError() = 0;
    bool virtual isConnected() = 0;
    string virtual read() = 0;
    bool virtual write(const std::string& data) = 0;
    void virtual abort() = 0;
};
