#include "./app.h"

namespace app
{

// TODO: This queue potentionally copies a lot of data.
//       It can be re-implemented with move sematics to avoid this.
//       But that would be over engineering for this simple use case.

class ThreadSafeQueueImpl : public ThreadSafeQueue {
public:

    ThreadSafeQueueImpl() {}
    ~ThreadSafeQueueImpl() {}

    void push(const string &input) override {
        {
            std::lock_guard<std::mutex> lock(this->mux);
            queue.push(input); // this line copies...
        }

        this->condVar.notify_one();
    }

    /**
     * Pops a message from the queue in a thread safe way.
     * Conditionally blocks for some time period if the queue is empty.
     * If the time period is negative it will block until some other
     * thread pushes data to the queue.
    */
    std::unique_ptr<string> pop(int timeoutMillis) override {
        std::unique_lock<std::mutex> lock(this->mux);

        if (timeoutMillis <= 0) {
            this->condVar.wait(lock, [&]{ return !this->queue.empty(); });
        }
        else {
            auto timeoutOccurred = !this->condVar.wait_for(
                lock,
                milliseconds(timeoutMillis),
                [&]{ return !this->queue.empty(); }
            );

            if (timeoutOccurred) {
                return nullptr;
            }
        }

        auto msg = this->queue.front();
        this->queue.pop();
        return std::make_unique<string>(msg);
    }

private:
    std::mutex mux;
    std::queue<string> queue;
    std::condition_variable condVar;
};

std::unique_ptr<ThreadSafeQueue> makeQueue() {
    return std::make_unique<ThreadSafeQueueImpl>();
}

} // namespace app
