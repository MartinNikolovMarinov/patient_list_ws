#include "app.h"

#include <thread>
#include <chrono>

namespace app
{
    static const char* GREET_MESSAGE = "Greetings. Connection to websocket was successful!";

    App::App(string &_url, std::ostream &_out, std::istream &_in) : out(_out), in(_in) {
        this->url = _url;
    }
    App::~App() {
        delete this->ws;
    }

    bool App::Init() {
        this->ws = WebSocket::from_url(this->url);
        return this->ws != nullptr;
    }

    void App::Run() {
        this->out << GREET_MESSAGE << "\n";
        CMDCommand helpCmd;
        helpCmd.type = CMDCommandType::HELP;
        helpCmd.execute(*this);

        while (true) {
            CMDCommand cmd = parseCMDFromStream(this->in);
            if (cmd.type == CMDCommandType::EXIT) {
                break;
            }
            cmd.execute(*this);

            if (cmd.type != CMDCommandType::HELP &&
                cmd.type != CMDCommandType::INVALID &&
                cmd.type != CMDCommandType::EXIT
            ) {
                ws->poll();
                ws->send("{\"setSubscriptions\": {\"public:patients\": \"\"}}");
                ws->dispatch([&](const std::string& message) {
                    // TODO: implement proper handler api:

                    // try {
                    //     Response r;
                    //     json rawJson = json::parse(message);
                    //     r.fromJSON(rawJson);

                    //     PatientList list;
                    //     for (auto it : r.uriToDataMap) {
                    //         json& rawJson = it.second;
                    //         list.fromJSON(rawJson);
                    //     }

                    //     out << message << std::endl;
                    // }
                    // catch(const std::exception& e) {
                    //     std::cerr << e.what() << '\n';
                    // }
                    // catch(...) {
                    //     std::cerr << "unexpected error" << '\n';
                    // }
                    this->out << message << "\n";
                });
                // std::this_thread::sleep_for(std::chrono::milliseconds(5000));
                this->out << "AFTER" << "\n";
            }
        }
    }

} // namespace app
