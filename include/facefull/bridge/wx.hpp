/////////////////////////////////////////////////////////////////////////////
// Name:
// Purpose:
// Author:      Nickolay Babbysh
// Created:     02.01.2025
// Copyright:   (c) NickWare Group
// Licence:     MIT licence
/////////////////////////////////////////////////////////////////////////////

#ifndef FACEFULL_BRIDGE_WX_HPP
#define FACEFULL_BRIDGE_WX_HPP

#include <facefull/bridge/interface.h>
#include <iostream>
#include <wx/wx.h>
#include <wx/webview.h>
#include <wx/display.h>

class FacefullBridgeWx : public FacefullBridgeInterface {
private:
    wxWebView *WebView;
    wxFrame *Frame;
    wxSize WindowSize;
    wxPoint WindowPosition;
    wxPoint PointerPosition;
    wxTimer *WindowTimer;
    std::string WindowLocation;
    bool CaptureFlag;
    bool PreventDefaultHandlerWindowReady;
    bool PreventDefaultHandlerWindowClose;
    bool Maximized = false;

    void WebViewCommandExecutor(const std::string &str) override {
        wxCommandEvent event(wxEVT_COMMAND_ENTER);
        event.SetString(str);
        wxPostEvent(Frame, event);
    }

    void doUISync(wxCommandEvent &evt) {
        if (WebView) WebView -> RunScript(evt.GetString());
    }

    void doMoveWindow(wxMouseEvent &evt) {
        if (!evt.Dragging()) {
            if (WebView) WebView -> ReleaseMouse();
            CaptureFlag = false;
        } else {
            wxPoint p;
            p = wxGetMousePosition();
            if (!CaptureFlag) {
                PointerPosition.x = p.x - Frame->GetScreenPosition().x;
                PointerPosition.y = p.y - Frame->GetScreenPosition().y;
            }
            p.x -= PointerPosition.x;
            p.y -= PointerPosition.y;
            Frame -> SetPosition(p);
            CaptureFlag = true;
        }
    }

    void doWindowPosReset(wxTimerEvent&) {
        Frame -> SetPosition(WindowPosition);
        Frame -> SetClientSize(WindowSize);
    }

    void onWindowMaximize() override {
        if (!Maximized) {
            doWindowMaximize();
        } else {
            doWindowRestore();
        }
    }

    void onEventReceive(wxCommandEvent &evt) {
        doEventCatch(evt.GetString().ToStdString());
    }

    void onWindowMinimize() override {
        Frame -> Iconize();
    }

    void onWindowMove() override {
        if (isMaximized()) return;
        if (WebView) WebView -> CaptureMouse();
    }

    void onWindowReady() override {
        if (!PreventDefaultHandlerWindowReady) {
            Frame -> Show();
        }
    }

    void onWindowClose() override {
        if (!PreventDefaultHandlerWindowClose) {
            Frame -> Hide();
            exit(0);
        }
    }

public:
    FacefullBridgeWx(wxApp *app, wxFrame *frame, wxWebView *webview, const std::string& window) {
        PreventDefaultHandlerWindowReady = false;
        PreventDefaultHandlerWindowClose = false;

        WindowLocation = window;

        WindowTimer = new wxTimer;
        WindowTimer -> Bind(wxEVT_TIMER, &FacefullBridgeWx::doWindowPosReset, this);
        CaptureFlag = false;

        Frame = frame;
        WebView = webview;
        app -> Bind(wxEVT_MOTION, &FacefullBridgeWx::doMoveWindow, this);
        Frame -> Bind(wxEVT_COMMAND_ENTER, &FacefullBridgeWx::doUISync, this);
        WebView -> Bind(wxEVT_WEBVIEW_TITLE_CHANGED, &FacefullBridgeWx::onEventReceive, this);
        WebView -> Bind(wxEVT_WEBVIEW_SCRIPT_MESSAGE_RECEIVED, &FacefullBridgeWx::onEventReceive, this);
        WebView -> AddScriptMessageHandler("facefullio");

        WebView -> LoadURL(window);
    }

    void doWindowMaximizeSizeReset() {
        wxDisplay display(Frame->GetMainWindowOfCompositeControl());
        Frame -> SetClientSize(wxSize(display.GetClientArea().width, display.GetClientArea().height));
    }

    void doWindowMaximize() {
        Maximized = true;
        wxDisplay display(Frame->GetMainWindowOfCompositeControl());
        WindowPosition = Frame -> GetScreenPosition();
        WindowSize = Frame -> GetClientSize();

#ifdef __WIN32__
        APPBARDATA abd;
        abd.cbSize = sizeof(APPBARDATA);
        SHAppBarMessage(ABM_GETTASKBARPOS, &abd);
        if (!abd.rc.top && abd.rc.bottom-abd.rc.top < 256) Frame -> SetPosition(wxPoint(display.GetClientArea().x, abd.rc.bottom+display.GetClientArea().y));
        else if (!abd.rc.left && abd.rc.bottom-abd.rc.top > 256) Frame -> SetPosition(wxPoint(abd.rc.right+display.GetClientArea().x, display.GetClientArea().y));
        else
#endif
        Frame -> SetPosition(wxPoint(display.GetClientArea().x, display.GetClientArea().y));
        Frame -> Maximize();
        doWindowMaximizeSizeReset();
    }

    void doWindowRestore() {
        Frame -> Maximize(false);
        WindowTimer -> StartOnce(1);
        Maximized = false;
    }

    void setWebView(wxWebView *webview) {
        WebView = webview;
        if (webview) {
            WebView -> Bind(wxEVT_WEBVIEW_TITLE_CHANGED, &FacefullBridgeWx::onEventReceive, this);
            WebView -> Bind(wxEVT_WEBVIEW_SCRIPT_MESSAGE_RECEIVED, &FacefullBridgeWx::onEventReceive, this);
            WebView -> AddScriptMessageHandler("facefullio");
            WebView -> LoadURL(WindowLocation);
        }
    }

    void setPreventDefaultHandlerWindowReady(bool prevent) {
        PreventDefaultHandlerWindowReady = prevent;
    }

    void setPreventDefaultHandlerWindowClose(bool prevent) {
        PreventDefaultHandlerWindowClose = prevent;
    }

    bool isMaximized() {
        return Maximized;
    }

    ~FacefullBridgeWx() {
        Frame -> Unbind(wxEVT_COMMAND_ENTER, &FacefullBridgeWx::doUISync, this);
        WebView -> Unbind(wxEVT_WEBVIEW_TITLE_CHANGED, &FacefullBridgeWx::onEventReceive, this);
        WebView -> Unbind(wxEVT_WEBVIEW_SCRIPT_MESSAGE_RECEIVED, &FacefullBridgeWx::onEventReceive, this);
        delete WindowTimer;
    }
};

#endif //FACEFULL_BRIDGE_WX_HPP
