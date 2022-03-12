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
    Request req;
    auto u = std::pair<string, string>("public:patients", "");
    req.uris.push_back(u);
    string msg = req.toJSONStr();
    app.sendQueue->push(msg);

    try {
        std::unique_ptr<string> receivedMsg = app.recvQueue->pop(app.timeoutMs);
        if (receivedMsg == nullptr) {
            throw std::invalid_argument("message receive timedout");
        }

        Response r;
        json rawJson = json::parse(*receivedMsg);
        r.fromJSON(rawJson);

        PatientList list;
        for (auto it : r.uriToDataMap) {
            json& rawJson = it.second;
            list.fromJSON(rawJson);
        }

        list.toStream(app.out);
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
    catch(...) {
        std::cerr << "unexpected error" << '\n';
    }
}

static void executePatientListWithDetailsCMD(App &app) {
    Request req;
    auto u = std::pair<string, string>("public:patients", "");
    req.uris.push_back(u);
    string msg = req.toJSONStr();
    app.sendQueue->push(msg);

    try {
        std::unique_ptr<string> receivedMsg = app.recvQueue->pop(app.timeoutMs);
        if (receivedMsg == nullptr) {
            throw std::invalid_argument("message receive timedout");
        }

        Response r;
        json rawJson = json::parse(*receivedMsg);
        r.fromJSON(rawJson);

        PatientList list;
        for (auto it : r.uriToDataMap) {
            json& rawJson = it.second;
            list.fromJSON(rawJson);
        }

        Request detailsReq;
        for (size_t i = 0; i < list.patients.size(); i++) {
            auto p = list.patients[i];
            auto u = std::pair<string, string>(p.uri, "patient");
            detailsReq.uris.push_back(u);
        }

        string detailsMsg = detailsReq.toJSONStr();
        app.sendQueue->push(detailsMsg);

        int count = detailsReq.uris.size();
        while (count > 0) {
            std::unique_ptr<string> receivedDetailsMsg = app.recvQueue->pop(app.timeoutMs);
            if (receivedDetailsMsg == nullptr) {
                break;
            }

            // TODO: create model to parse this response:
            app.out << *receivedDetailsMsg << '\n';
            count--;
        }
    }
    catch(const std::exception& e) {
        app.out << e.what() << '\n';
    }
    catch(...) {
        app.out << "unexpected error" << '\n';
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
        case CMDCommandType::PATIENT_LIST_WITH_DETAILS:
            executePatientListWithDetailsCMD(app);
            break;
        default:
            executeInvalidCMD(app.out);
            break;
    }
}

} // namespace app
