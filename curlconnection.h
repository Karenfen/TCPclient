#pragma once

#include <QObject>
#include "cURLwrapper.h"
#include "Iconnection.h"



class cURLconnection : public QObject, public Iconnection
{
    Q_OBJECT
public:
    explicit cURLconnection(QObject *parent = nullptr): QObject(parent) {};
    ~cURLconnection() override;
    bool init() override;
    bool connectTo(const QHostAddress& Address, uint16_t port) override;
    bool _disconnect() override;
    const std::string& getLastError() override { return lastError; }
    bool isConnected() override;
    std::string read() override;
    bool write(const std::string& data) override;
    void abort() override;

signals:
    void disconnected();

private slots:
    void close();

private:
    std::unique_ptr<cURLwrapper> curlWrap_{nullptr};
    std::string lastErrore{};
};

