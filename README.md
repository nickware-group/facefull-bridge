<p align="center">
    <img width="600" height="200" src="https://nickware.group/repository/products/ff/logonew_t.png"><br><br>
    <a href="https://github.com/nickware44/facefull">Facefull</a> bridge library
    <br>1.0.4
</p>

Facefull bridge is a communication system which connects application native backend (C++/Java/etc...) and Facefull user interface (runs through webview component). 
This library allows you to use Facefull to create a user interface for native desktop applications using the webview component. In native mode you can use built-in event system to communicate with your native desktop application C++ backend.

This library contains Facefull bridge interface and bridge implementation for wxWidgets (webview/webkit) and Qt5 (webkit) frameworks.
You can use predefined implementations or create your own.

A web bridge mode is also available. In this mode, the backend application runs an HTTP server and provides communication between the native C++ code and Facefull frontend via the bridge system.

<p align="center">
    <img width="820" height="640" src="https://nickware.group/repository/products/ff/scheme_full_en.png"><br><br>
</p>


[facefull official page](https://nickware.group/products/facefull)

----------------------------------------------------------------
### LICENCE
Facefull bridge library is distributed under the MIT Licence.

----------------------------------------------------------------
### USAGE EXAMPLES
#### 1. [Bridge for wxWidgets](examples/wxwidgets)

Supported OS:
- Windows (uses [Microsoft Edge WebView2 Runtime](https://developer.microsoft.com/en-us/microsoft-edge/webview2?form=MA13LH&cs=578062562))
- Linux (uses WebKit)

#### 2. [Bridge for Qt5](examples/qt5webkit)

Supported OS:
- Linux (uses WebKit)

#### 3. [Bridge for web](examples/web_bridge)

Supported OS:
- Windows
- Linux


----------------------------------------------------------------
### HOW TO BUILD
All files will be in dist directory.
#### Windows
Just run build.cmd script.

#### Linux
Just run build.sh script.
