#include <iostream>
#include <facefull/bridge/web.hpp>

int main(int argc, char *argv[]) {
    FacefullBridgeWeb bridge("../ui", 8080);

    bridge.doEventAttach("doWindowReady", [](const std::string &data, const std::string &address,
        const FacefullBridgeWeb::WebRequester &request, FacefullBridgeWeb::WebResponser &response){
        std::cout << "Page loaded, client IP address: " << address << std::endl;
    });

    bridge.doEventAttach("getTestData", [](const std::string &data, const std::string &address,
        const FacefullBridgeWeb::WebRequester &request, FacefullBridgeWeb::WebResponser &response){
        std::cout << "Got data '" << data << "'" << std::endl;
        doEventResponse("Test message");
    });

    std::cout << "Running facefull web bridge" << std::endl;
    bridge.doRunServer();

    return 0;
}
