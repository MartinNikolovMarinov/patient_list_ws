#include "app.h"

namespace app
{

static const char* UPDATE_SUBSCRIPTIONS_KEY = "updateSubscriptions";

void Response::fromJSON(json &rawJSON) {
    auto setSubscriptions = safeReadKey(rawJSON, UPDATE_SUBSCRIPTIONS_KEY);
    for (auto &s : setSubscriptions.items()) {
        string uri = s.key();
        this->uriToDataMap[uri] = s.value();
    }
}

} // namespace app