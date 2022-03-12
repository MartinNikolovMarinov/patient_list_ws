#include "app.h"

namespace app
{

static void executeHelpCMD(std::ostream &out) {
    out << "LIST OF AVAILABLE COMMANDS" << "\n";
    out << "help \t\t\t\t - displays this message." << "\n";
    out << "patient_list \t\t\t - displays brief info for all patients." << "\n";
    out << "patient_list_with_details \t - displays full information for all patients (will flood the screan)." << "\n";
    // out << "patient_by_id $id \t\t - gets detailed info about a patient by a specific id." << "\n";
    // out << "filter_by_age $id \t\t - gets brief info for all patients with a specific age." << "\n";
    // out << "filter_by_gender $id \t\t - gets brief info for all patients with a specific gender (M or F)." << "\n";
    out << "\n";
}

static void executeInvalidCMD(std::ostream &out) {
    out << "Invalid command." << "\n";
    executeHelpCMD(out);
}

static void executePatientListCMD(App &app) {
    string msg = "{\"setSubscriptions\": {\"public:patients\": \"\"}}";
    app.sendQueue->push(msg);

    std::unique_ptr<string> receivedMsg = app.recvQueue->pop(app.timeoutMs);
    if (receivedMsg != nullptr) {
        // app.out << "DEBUG: message in main thread" << std::endl << std::endl;
        app.out << *receivedMsg << std::endl;
    } else {
        app.out << "DEBUG: Connection timedout" << std::endl;
    }
}

void CMDCommand::execute(App &app) {
    switch (this->type) {
        case CMDCommandType::HELP:
            executeHelpCMD(app.out);
            break;
        case CMDCommandType::PATIENT_LIST:
            executePatientListCMD(app);
            break;
        default:
            executeInvalidCMD(app.out);
            break;
    }
}

} // namespace app
