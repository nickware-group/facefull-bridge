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

#include <facefull/bridge/interface.h>
#include <httplib.h>
#include <utility>

class FacefullBridgeWeb : public FacefullBridgeInterface {
private:
    httplib::Server *Srv;
    std::string Address;
    int Port;

    void WebViewCommandExecutor(const std::string &data) override {}

    void onWindowMaximize() override {}

    void onWindowMinimize() override {}

    void onWindowMove() override {}

    void onWindowReady() override {}

    void onWindowClose() override {}

public:
    typedef std::function<void(const std::string&, httplib::Response &res)> WebEventHandler;
    typedef httplib::Response WebResponser;
    #define doEventResponse(data) {response.set_content(data, "application/json");}

    FacefullBridgeWeb(const std::string& path, int port, std::string address = "0.0.0.0") {
        Srv = new httplib::Server;
        Port = port;
        Address = std::move(address);

        auto ret = Srv -> set_mount_point("/", path);
        if (!ret) {
            std::cerr << "Error opening resource location " << path << std::endl;
            return;
        }
    }

    void doEventAttach(const std::string &eventname, WebEventHandler function) {
        Srv -> Post("/bridge/"+eventname+"/", [this, function](const httplib::Request &req, httplib::Response &res) {
            function(req.body, res);
        });
    }

    void doRunServer() {
        Srv -> listen(Address, Port);
    }

    void doEventSend(const std::string &eventname, const std::string &data) override {}
    void doEventCatch(const std::string&) {}

    ~FacefullBridgeWeb() {
        delete Srv;
    }
};

#endif //FACEFULL_BRIDGE_WEB_HPP
