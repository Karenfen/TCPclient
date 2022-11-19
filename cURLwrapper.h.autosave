#pragma once

#include <curl/curl.h>
#include <memory>
#include <exception>



class  cURLwrapper
{
public:
    ~cURLwrapper(){
        curl_easy_cleanup(curl_);
    }
    static std::unique_ptr<cURLwrapper> getURL(){
        auto newURL = std::make_unique<cURLwrapper>();
        return newURL;
    }
    CURL* curl(){
        return curl_;
    }
protected:
    cURLwrapper(){
        curl_ = curl_easy_init();
        if(!curl_)
            throw std::logic_error("Init error! First do \'globalInit*()\'");
    }

private:
    CURL* curl_{nullptr};
};
