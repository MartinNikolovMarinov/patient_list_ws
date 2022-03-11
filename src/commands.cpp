#include "app.h"

#include <stdlib.h>

namespace app
{

CMDCommand parseCMDFromStream(std::istream &stream) {
    CMDCommand cmd;
    string command;
    stream >> command;

    std::vector<string> split = splitStr(command, ' ');
    if (split.size() < 1) {
        cmd.type = CMDCommandType::HELP;
    }
    else if (split[0] == "patient_list") {
        cmd.type = CMDCommandType::PATIENT_LIST;
        cmd.params.assign(split.begin() + 1, split.end());
    }
    else if (split[0] == "help") {
        cmd.type = CMDCommandType::HELP;
    }
    else if (split[0] == "exit") {
        cmd.type = CMDCommandType::EXIT;
    }
    else {
        cmd.type = CMDCommandType::INVALID;
    }

    return cmd;
}

static void executeHelpCMD(std::ostream &out) {
    out << "LIST OF AVAILABLE COMMANDS" << "\n";
    out << "help \t\t\t\t - displays this message." << "\n";
    out << "exit \t\t\t\t - stops the program." << "\n";
    out << "patient_list \t\t\t - displays brief info for all patients." << "\n";
    out << "patient_list_with_details \t - displays full information for all patients (will flood the screan)." << "\n";
    out << "patient_by_id $id \t\t - gets detailed info about a patient by a specific id." << "\n";
    out << "filter_by_age $id \t\t - gets brief info for all patients with a specific age." << "\n";
    out << "filter_by_gender $id \t\t - gets brief info for all patients with a specific gender (M or F)." << "\n";
}

static void executeInvalidCMD(std::ostream &out) {
    out << "Invalid command. Type help for a list of available commands." << "\n";
}

static void executeExitCMD() {
    exit(0);
}

static void executePatientListCMD(WebSocket::pointer ws, std::ostream &out) {
    ws->send("{\"setSubscriptions\": {\"public:patients\": \"\"}}");
    ws->poll(-1);
    ws->dispatch([](const std::string& message) {
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
    });
}

void CMDCommand::execute(App &app) {
    switch (this->type) {
        case CMDCommandType::HELP:
            executeHelpCMD(app.out);
            break;
        case CMDCommandType::PATIENT_LIST:
            executePatientListCMD(app.ws, app.out);
            break;
        case CMDCommandType::EXIT:
            executeExitCMD();
            break;
        default:
            executeInvalidCMD(app.out);
            break;
    }
}

} // namespace app
