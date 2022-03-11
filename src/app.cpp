#include "app.h"

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
        }
    }

} // namespace app
