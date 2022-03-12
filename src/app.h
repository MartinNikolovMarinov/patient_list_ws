#pragma once

#include <atomic>
#include <condition_variable>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <unordered_map>
#include <vector>

#include "easywsclient.h"
#include "json.hpp"

namespace app
{

using milliseconds = std::chrono::milliseconds;
using string = std::string;
using ostream = std::ostream;
using istream = std::istream;
using thread = std::thread;
using WebSocket = easywsclient::WebSocket;
using json = nlohmann::json;

class ThreadSafeQueue;
class App;
enum struct CMDCommandType;
struct CMDCommand;
struct PatientInfo;

json& safeReadKey(json &obj, const char* key);

class ThreadSafeQueue {
public:
    ThreadSafeQueue() = default;
    virtual ~ThreadSafeQueue() = default;

    virtual void push(const string &input) = 0;
    virtual std::unique_ptr<string> pop(int timeoutMillis) = 0;
private:
    ThreadSafeQueue(const ThreadSafeQueue&) = delete;
    ThreadSafeQueue& operator=(const ThreadSafeQueue&) = delete;
};

std::unique_ptr<ThreadSafeQueue> makeQueue();

class App {
public:
    WebSocket::pointer ws;
    ostream &out;
    istream &in;
    std::atomic<bool> running;
    std::unique_ptr<ThreadSafeQueue> sendQueue;
    std::unique_ptr<ThreadSafeQueue> recvQueue;
    int timeoutMs;
    std::shared_ptr<CMDCommand> cmd;

    App(string &url, int timeoutMs, ostream &out = std::cout, istream &in = std::cin);
    ~App();

    bool init(std::shared_ptr<CMDCommand> cmd);
    void run();

private:
    string url;
    thread *networkThread;
};

enum struct CMDCommandType {
    INVALID                     = 0,
    HELP                        = 1,
    PATIENT_LIST                = 2,
    PATIENT_LIST_WITH_DETAILS   = 3,
};

struct CMDCommand {
    CMDCommandType type;
    std::vector<string> params;

    void execute(App &app);
};

std::shared_ptr<app::CMDCommand> parseCmdFromArgs(int argc, char const *argv[]);

struct Response {
    std::unordered_map<string, json> uriToDataMap;
    void fromJSON(json &rawJSON);
};

struct Request {
    std::vector<std::pair<string, string>> uris;
    string toJSONStr();
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
    void toStream(ostream &out);
};

struct PatientList {
    std::vector<PatientInfo> patients;

    void fromJSON(json &rawJSON);
    void toStream(ostream &out);
};

}