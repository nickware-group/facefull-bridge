/////////////////////////////////////////////////////////////////////////////
// Name:
// Purpose:
// Author:      Nickolay Babich
// Created:     28.03.2025
// Copyright:   (c) NickWare Group
// Licence:     MIT licence
/////////////////////////////////////////////////////////////////////////////

#ifndef FACEFULL_BRIDGE_WEB_HPP
#define FACEFULL_BRIDGE_WEB_HPP

#ifdef FB_BRIDGE_WEB_SECURE
#define CPPHTTPLIB_OPENSSL_SUPPORT
#endif

#include <facefull/bridge/interface.h>
#include <httplib.h>
#include <utility>

class FacefullBridgeWeb : public FacefullBridgeInterface {
private:
    httplib::Server *Srv = nullptr;
#ifdef FB_BRIDGE_WEB_SECURE
    httplib::SSLServer *SrvSecure = nullptr;
#endif

    std::string Address;
    int Port;

    void WebViewCommandExecutor(const std::string &data) override {}

    void onWindowMaximize() override {}

    void onWindowMinimize() override {}

    void onWindowMove() override {}

    void onWindowReady() override {}

    void onWindowClose() override {}

public:
    typedef httplib::Request WebRequester;
    typedef httplib::Response WebResponser;
    typedef std::function<void(const std::string &data, const std::string &address, const WebRequester &req, WebResponser &res)> WebEventHandler;
    #define doEventResponse(data) {response.set_content(data, "application/json");}

    FacefullBridgeWeb(const std::string &path, int port, std::string address = "0.0.0.0") {
        Srv = new httplib::Server;
        Port = port;
        Address = std::move(address);

        auto ret = Srv -> set_mount_point("/", path);
        if (!ret) {
            std::cerr << "Error opening resource location " << path << std::endl;
            return;
        }
    }

#ifdef FB_BRIDGE_WEB_SECURE
    FacefullBridgeWeb(const std::string &path, int port, const std::string &crt, const std::string &key, std::string address = "0.0.0.0") {
        SrvSecure = new httplib::SSLServer(crt.c_str(), key.c_str());
        Port = port;
        Address = std::move(address);

        auto ret = SrvSecure -> set_mount_point("/", path);
        if (!ret) {
            std::cerr << "Error opening resource location " << path << std::endl;
            return;
        }
    }
#endif

    void doEventAttach(const std::string &eventname, const WebEventHandler &function) const {
        auto callback = [function](const auto &req, auto &res) {
            function(req.body, req.get_header_value("REMOTE_ADDR"), req, res);
        };

        if (Srv) Srv -> Post("/bridge/"+eventname+"/", callback);
#ifdef FB_BRIDGE_WEB_SECURE
        else if (SrvSecure) SrvSecure -> Post("/bridge/"+eventname+"/", callback);
#endif
    }

    void doRunServer() const {
        if (Srv) Srv -> listen(Address, Port);
#ifdef FB_BRIDGE_WEB_SECURE
        else if (SrvSecure) SrvSecure -> listen(Address, Port);
#endif
    }

    void doEventSend(const std::string &eventname, const std::string &data) override {}
    void doEventCatch(const std::string&) override {}

    ~FacefullBridgeWeb() override {
        delete Srv;
#ifdef FB_BRIDGE_WEB_SECURE
        delete SrvSecure;
#endif
    }
};

#endif //FACEFULL_BRIDGE_WEB_HPP
