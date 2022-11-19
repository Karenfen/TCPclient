#include "curlconnection.h"
#include <QHostAddress>




cURLconnection::~cURLconnection()
{
    curl_global_cleanup();
}


bool cURLconnection::init()
{
    curl_global_init(CURL_GLOBAL_ALL);
    try
    {
        curlWrap_ = cURLwrapper::getURL();

    }  catch (std::exception& e)
    {
        curl_global_cleanup();
        lastErrore = e.what();
        return false;
    }

    return true;
}


bool cURLconnection::connectTo(const QHostAddress &Address, uint16_t port)
{
    curl_slist *connectTo = nullptr;
    connectTo = curl_slist_append(nullptr, std::string("::") + Address.toString().toStdString() + ":" + );
    curl_easy_setopt(curlWrap_->curl(), CURLOPT_PROTOCOLS_STR, "http,https");
    curl_easy_setopt(curlWrap_->curl(), CURLOPT_CONNECT_TO, );
    return true;
}
