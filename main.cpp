#include "src/app.h"

int main(int argc, char const *argv[]) {
    std::string url = "ws://apply.viewray.ai:4645";
    app::App a(url, std::cout, std::cin);
    if (!a.Init()) {
        std::cerr << "Failed to establish web socket connection with: " << url << std::endl;
        return -1;
    }

    a.Run();
    return 0;
}