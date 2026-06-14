/////////////////////////////////////////////////////////////////////////////
// Name:
// Purpose:
// Author:      Nickolay Babich
// Created:     03.01.2025
// Copyright:   (c) NickWare Group
// Licence:     MIT licence
/////////////////////////////////////////////////////////////////////////////

#ifndef FACEFULL_BRIDGE_QT5WEBKIT_HPP
#define FACEFULL_BRIDGE_QT5WEBKIT_HPP

#ifndef __WIN32__
#include <facefull/bridge/interface.h>
#include <iostream>
#include <QMainWindow>
#include <QMouseEvent>
#include <QtWebKitWidgets/QWebView>
#include <QtWebKitWidgets/QWebPage>
#include <QtWebKitWidgets/QWebFrame>

class FacefullBridgeQt5WebKit : public FacefullBridgeInterface {
private:
    QMainWindow *Frame;
    QWebView *WebView;
    QPoint MousePosition;
    bool CaptureFlag;
    bool PreventDefaultHandlerWindowReady;
    bool PreventDefaultHandlerWindowClose;

    void WebViewCommandExecutor(const std::string &data) override {
        auto frame = WebView->page()->mainFrame();
        frame -> evaluateJavaScript(QString::fromStdString(data));
    }

    void onWindowMaximize() override {
        if (isMaximized()) Frame -> setWindowState(Qt::WindowNoState);
        else Frame -> setWindowState(Qt::WindowMaximized);
    }

    void onWindowMinimize() override {
        Frame -> setWindowState(Frame->windowState() | Qt::WindowMinimized);
    }

    void onWindowMove() override {
        if (isMaximized()) return;
        CaptureFlag = true;
        MousePosition = QPoint(0, 0);
    }

    void onWindowReady() override {
        FacefullBridgeInterface::doEventSend("doConnectQtBridgeEventHandler", "");
        if (!PreventDefaultHandlerWindowReady) {
            Frame -> show();
        }
    }

    void onWindowClose() override {
        if (!PreventDefaultHandlerWindowClose) {
            Frame -> hide();
            exit(0);
        }
    }

public:
    FacefullBridgeQt5WebKit(QMainWindow *frame, QWebView *webview, const QUrl& window) {
        PreventDefaultHandlerWindowReady = false;
        PreventDefaultHandlerWindowClose = false;

        CaptureFlag = false;
        Frame = frame;
        WebView = webview;

        QObject::connect(WebView, SIGNAL(titleChanged(const QString&)), Frame, SLOT(doBridgeEventReceive(const QString&)));

        WebView -> installEventFilter(Frame);
        WebView -> setUrl(window);
        WebView -> page() -> mainFrame() -> addToJavaScriptWindowObject("bridge", (QMainWindow*)Frame);
    }

    void doMoveWindow(QMouseEvent *evt) {
        if (!CaptureFlag) return;

        if (evt->type() == QEvent::MouseMove) {
            if (MousePosition.x() && MousePosition.y()) {
                auto d = evt->globalPos() - MousePosition;
                auto wx = Frame->x() + d.x();
                auto wy = Frame->y() + d.y();
                Frame -> move(wx, wy);
            }

            MousePosition = evt -> globalPos();
        } else if (evt->type() == QEvent::MouseButtonRelease) {
            MousePosition = QPoint(0, 0);
            CaptureFlag = false;
        }
    }

    void doEventSend(const std::string& comm, const std::string &data) override {
        QMetaObject::invokeMethod((QMainWindow*)Frame
                , "BridgeEventHandler"
                , Qt::AutoConnection
                , Q_ARG(QString, QString::fromStdString(comm)), Q_ARG(QString, QString::fromStdString(data)));
    }

    void setWebView(QWebView *webview) {
        WebView = webview;
    }

    void setPreventDefaultHandlerWindowReady(bool prevent) {
        PreventDefaultHandlerWindowReady = prevent;
    }

    void setPreventDefaultHandlerWindowClose(bool prevent) {
        PreventDefaultHandlerWindowClose = prevent;
    }

    bool isMaximized() {
        return Frame -> windowState() == Qt::WindowMaximized;
    }
};
#endif

#endif //FACEFULL_BRIDGE_QT5WEBKIT_HPP
