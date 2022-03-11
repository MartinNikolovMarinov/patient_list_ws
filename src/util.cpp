#include "app.h"

namespace app
{

json& safeReadKey(json &obj, const char* key) {
    if (!obj.is_object() || !obj.contains(key)) {
        string errMsg = std::string("Failed to read argument: ") + key;
        throw std::invalid_argument(errMsg);
    }
    return obj[key];
}

std::vector<std::string> splitStr(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

} // namespace app