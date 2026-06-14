#include <iostream>
#include "frame.h"
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <string>

Frame::Frame(wxApp *app, const wxString& title) : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(1000, 700), wxRESIZE_BORDER) {
    Centre();
    auto apath = wxFileName(wxStandardPaths::Get().GetExecutablePath()).GetPath().ToStdString();

    std::cout  << "WebView backend: " << wxWebView::GetBackendVersionInfo().ToString() << std::endl;
    if (wxWebView::IsBackendAvailable(wxWebViewBackendEdge)) {
        WebView = wxWebView::New(this, wxID_ANY, wxEmptyString, wxDefaultPosition, GetClientSize(), wxWebViewBackendEdge, wxNO_BORDER);
    } else {
        WebView = wxWebView::New(this, wxID_ANY, wxEmptyString, wxDefaultPosition, GetClientSize(), wxWebViewBackendDefault, wxNO_BORDER);
    }

    WebView -> EnableAccessToDevTools(true);

    Bridge = new FacefullBridgeWx(app, this, WebView, "file://"+apath+"/../ui/index.html");

    std::cout << "Window loaded" << std::endl;

    Bridge -> doEventAttach("onTestMessage", [this](const std::string& data) {
        std::cout << data << std::endl;
    });

    Bridge -> doEventAttach("doWindowReady", [this](const std::string& data) {
        Bridge -> doEventSend("doTestMessage", "Bridge event system test message");
    });
}

#ifdef __WIN32__
WXLRESULT Frame::MSWWindowProc(WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam) {
    switch (nMsg) {
        case WM_NCACTIVATE:
            lParam = -1;
            break;
        case WM_NCCALCSIZE:
            if (wParam) {
                HWND hWnd = (HWND)this->GetHandle();
                WINDOWPLACEMENT wPos;
                wPos.length = sizeof(wPos);
                GetWindowPlacement(hWnd, &wPos);
                if (wPos.showCmd != SW_SHOWMAXIMIZED) {
                    RECT borderThickness;
                    SetRectEmpty(&borderThickness);
                    AdjustWindowRectEx(&borderThickness,
                                       GetWindowLongPtr(hWnd, GWL_STYLE) & ~WS_CAPTION, FALSE, NULL);
                    borderThickness.left *= -1;
                    borderThickness.top *= -1;
                    auto sz = reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam);
                    sz -> rgrc[0].top += 1;
                    sz -> rgrc[0].left += borderThickness.left;
                    sz -> rgrc[0].right -= borderThickness.right;
                    sz -> rgrc[0].bottom -= borderThickness.bottom;
                    return 0;
                }
            }
            break;
    }
    return wxFrame::MSWWindowProc(nMsg, wParam, lParam);
}
#endif
