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

} // namespace app