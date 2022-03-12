#include "src/app.h"

int main(int argc, char const *argv[]) {
    std::string url = "ws://apply.viewray.ai:4645";
    app::App a(url, 10000, std::cout, std::cin);

    auto cmd =  app::parseCmdFromArgs(argc, argv);
    if (!a.init(cmd)) {
        std::cerr << "Failed to establish web socket connection with: " << url << std::endl;
        return -1;
    }

    a.run();
    return 0;
}