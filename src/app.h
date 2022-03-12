#pragma once

#include <atomic>
#include <condition_variable>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <unordered_map>
#include <vector>

#include "easywsclient.h"

namespace app
{

using milliseconds = std::chrono::milliseconds;
using string = std::string;
using ostream = std::ostream;
using istream = std::istream;
using thread = std::thread;
using WebSocket = easywsclient::WebSocket;

class ThreadSafeQueue;
class App;
enum struct CMDCommandType;
struct CMDCommand;

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

}