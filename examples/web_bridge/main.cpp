#include <iostream>

// for secure web bridge
// #define FB_BRIDGE_WEB_SECURE
#include <facefull/bridge/web.hpp>

int main(int argc, char *argv[]) {
    FacefullBridgeWeb bridge("../ui", 8080);

    // for secure web bridge
    // FacefullBridgeWeb bridge("../ui", 8080, "cert.pem", "key.pem");

    bridge.doEventAttach("doWindowReady", [](const std::string &data, const std::string &address,
        const auto &request, auto &response){
        std::cout << "Page loaded, client IP address: " << address << std::endl;
    });

    bridge.doEventAttach("getTestData", [](const std::string &data, const std::string &address,
        const auto &request, auto &response){
        std::cout << "Got data '" << data << "'" << std::endl;
        doEventResponse("Test message");
    });

    std::cout << "Running facefull web bridge (http://localhost:8080)" << std::endl;
    bridge.doRunServer();

    return 0;
}
