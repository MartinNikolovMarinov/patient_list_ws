#include "app.h"

namespace app
{

App::App(string &_url, int timeoutMs, std::ostream &_out, std::istream &_in) : out(_out), in(_in) {
    this->url = _url;
    this->running = false;
    this->sendQueue = app::makeQueue();
    this->recvQueue = app::makeQueue();
    this->timeoutMs = timeoutMs;
    this->networkThread = nullptr;
    this->ws = nullptr;
}
App::~App() {
    this->running = false;
    // std::cout << "DEBUG: Destructor called" << std::endl;
    if (this->networkThread != nullptr && this->networkThread->joinable()) {
        this->networkThread->join();
        delete this->networkThread;
    }
    if (this->ws != nullptr) {
        delete this->ws;
    }
}

bool App::init(std::shared_ptr<CMDCommand> cmd) {
    this->cmd = cmd;

    if (this->cmd->type == CMDCommandType::HELP ||
        this->cmd->type == CMDCommandType::INVALID
    ) {
        return true;
    }

    this->ws = WebSocket::from_url(this->url);
    if (this->ws == nullptr) {
        return false;
    }

    this->running = true;

    this->networkThread = new std::thread([&] {
        static const int pollingInterval = 200;
        while (this->running) {
            // std::cout << "DEBUG: Poll" << std::endl;

            if (this->ws->getReadyState() == WebSocket::CLOSED) {
                // std::cout << "DEBUG: Websocket closed on the other end." << std::endl;
                break;
            }

            std::unique_ptr<string> msgToSend = this->sendQueue->pop(pollingInterval);
            if (msgToSend != nullptr) {
                // std::cout << "DEBUG: Sending" << std::endl;
                this->ws->send(*msgToSend);
            }

            this->ws->poll();
            this->ws->dispatch([&](const std::string & message) {
                // std::cout << "DEBUG: Received" << std::endl;
                this->recvQueue->push(message);
            });
        }

        this->ws->close();
        this->ws->poll();
    });

    return true;
}

void App::run() {
    if (this->cmd != nullptr) {
        this->cmd->execute(*this);
    }
    this->running = false;
}

std::shared_ptr<app::CMDCommand> parseCmdFromArgs(int argc, char const *argv[]) {
    std::shared_ptr<app::CMDCommand> cmd = std::make_shared<app::CMDCommand>();

    if (argc < 2) {
        cmd->type = CMDCommandType::INVALID;
        return cmd;
    }

    std::string commandType = std::string(argv[1]);

    if (commandType.compare("patient_list") == 0 && argc == 2) {
        cmd->type = CMDCommandType::PATIENT_LIST;
    }
    else if (commandType.compare("patient_list_with_details") == 0 && argc == 2) {
        cmd->type = CMDCommandType::PATIENT_LIST_WITH_DETAILS;
    }
    else if (commandType.compare("help") == 0) {
        cmd->type = CMDCommandType::HELP;
    }
    else {
        cmd->type = CMDCommandType::INVALID;
    }

    return cmd;
}

} // namespace app
