#pragma once

#include <unordered_map>
#include <vector>
#include <iostream>

#include "easywsclient.h"
#include "json.hpp"

namespace app
{

using WebSocket = easywsclient::WebSocket;
using json = nlohmann::json;
using string = std::string;

class App;
struct Response;
struct PatientInfo;
struct PatientList;
enum struct CMDCommandType;
struct CMDCommand;
struct HelpCMD;

static const char* PATIENT_LIST_URI = "public:patients";

json& safeReadKey(json &obj, const char* key);
std::vector<std::string> splitStr(const std::string& s, char delimiter);

class App {
public:
    WebSocket::pointer ws;
    std::ostream &out;
    std::istream &in;

    App(string &url, std::ostream &out = std::cout, std::istream &in = std::cin);
    ~App();

    bool Init();
    void Run();

private:
    string url;
};

struct Response {
    std::unordered_map<string, json> uriToDataMap;
    void fromJSON(json &rawJSON);
};

struct PatientInfo {
    string id;
    string mrn;
    string sex;
    string dateOfBirth; // TODO: convert to std::chrono
    string firstName;
    string middleName;
    string lastName;
    int fractionsTotal;
    int fractionsCompleted;
    int weightInKg;
    bool readyForTreatment;
    int registrationTime; // TODO: unix timestamp ?
    string uri;

    void fromJSON(json &rawJSON);
};

struct PatientList {
    std::vector<PatientInfo> patients;

    void fromJSON(json &rawJSON);
    size_t getPatientsCount();
};

enum struct CMDCommandType {
    INVALID                     = 0,
    PATIENT_LIST                = 1,
    PATIENT_LIST_WITH_DETAILS   = 2,
    PATIENT_BY_ID               = 3,
    FILTER_BY_AGE               = 4,
    FILTER_BY_GENDER            = 5,
    HELP                        = 6,
    EXIT                        = 7
};

CMDCommand parseCMDFromStream(std::istream &stream);

struct CMDCommand {
    CMDCommandType type;
    std::vector<string> params;

    void execute(App &app);
};

} // namespace app

