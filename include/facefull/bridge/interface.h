/////////////////////////////////////////////////////////////////////////////
// Name:
// Purpose:
// Author:      Nickolay Babich
// Created:     02.10.2021
// Copyright:   (c) NickWare Group
// Licence:     MIT licence
/////////////////////////////////////////////////////////////////////////////

#ifndef FACEFULL_BRIDGE_INTERFACE_H
#define FACEFULL_BRIDGE_INTERFACE_H

#include <string>
#include <map>
#include <functional>

class FacefullBridgeInterface {
public:
    typedef std::function<void(const std::string&)> EventHandler;
    FacefullBridgeInterface();
    void doEventAttach(const std::string &eventname, EventHandler function);
    virtual void doEventSend(const std::string &eventname, const std::string &data);
    bool isWindowReady() const;
    void doEventCatch(const std::string&);
    ~FacefullBridgeInterface() = default;
private:
    std::multimap<std::string, EventHandler> Events;
    bool WindowReady;
protected:
    virtual void onWindowReady() = 0;
    virtual void onWindowMove() = 0;
    virtual void onWindowMaximize() = 0;
    virtual void onWindowMinimize() = 0;
    virtual void onWindowClose() = 0;
    virtual void WebViewCommandExecutor(const std::string&) = 0;
};

#endif //FACEFULL_BRIDGE_INTERFACE_H
