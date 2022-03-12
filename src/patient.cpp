#include "app.h"

namespace app
{

static const char* UPDATE_SUBSCRIPTIONS_KEY = "updateSubscriptions";
static const char* SET_SUBSCRIPTIONS_KEY = "setSubscriptions";

void Response::fromJSON(json &rawJSON) {
    auto setSubscriptions = safeReadKey(rawJSON, UPDATE_SUBSCRIPTIONS_KEY);
    for (auto &s : setSubscriptions.items()) {
        string uri = s.key();
        this->uriToDataMap[uri] = s.value();
    }
}

string Request::toJSONStr() {
    json j = "{ \"setSubscriptions\": {} }"_json;
    for (size_t i = 0; i < this->uris.size(); i++) {
        auto uri = this->uris[i];
        j[SET_SUBSCRIPTIONS_KEY][uri.first] = uri.second;
    }

    string ret = j.dump();
    return ret;
}

void PatientInfo::fromJSON(json &rawJSON) {
    this->id = safeReadKey(rawJSON, "id").get<string>();
    this->mrn = safeReadKey(rawJSON, "mrn").get<string>();
    this->sex = safeReadKey(rawJSON, "sex").get<string>();
    this->dateOfBirth = safeReadKey(rawJSON, "date_of_birth").get<string>();
    this->firstName = safeReadKey(rawJSON, "first_name").get<string>();
    this->middleName = safeReadKey(rawJSON, "middle_name").get<string>();
    this->lastName = safeReadKey(rawJSON, "last_name").get<string>();
    this->fractionsTotal = safeReadKey(rawJSON, "fractions_total").get<int>();
    this->fractionsCompleted = safeReadKey(rawJSON, "fractions_completed").get<int>();
    this->weightInKg = safeReadKey(rawJSON, "weight_kg").get<int>();
    this->readyForTreatment = safeReadKey(rawJSON, "ready_for_treatment").get<bool>();
    this->registrationTime = safeReadKey(rawJSON, "registration_time").get<int>();
    this->uri = safeReadKey(rawJSON, "uri").get<string>();
}

void PatientInfo::toStream(ostream &out) {
    out << "Patient Info (id=" << this->id << ")" << "\n";
    out << "\t" << "mrn = " << this->mrn << "\n";
    out << "\t" << "sex = " << this->sex << "\n";
    out << "\t" << "date_of_birth = " << this->dateOfBirth << "\n";
    out << "\t" << "first_name = " << this->firstName << "\n";
    out << "\t" << "middle_name = " << this->middleName << "\n";
    out << "\t" << "last_name = " << this->lastName << "\n";
    out << "\t" << "fractions_total = " << this->fractionsTotal << "\n";
    out << "\t" << "fractions_completed = " << this->fractionsCompleted << "\n";
    out << "\t" << "weight_in_kg = " << this->weightInKg << "\n";
    out << "\t" << "ready_for_treatment = " << this->readyForTreatment << "\n";
    out << "\t" << "registration_time = " << this->registrationTime << "\n";
    out << "\t" << "uri = " << this->uri << "\n";
}

void PatientList::fromJSON(json &rawJSON) {
    string type = safeReadKey(rawJSON, "type").get<string>();
    if (type != "PatientList") {
        return;
    }

    json value = safeReadKey(rawJSON, "value");
    size_t pcount = value.size();
    this->patients.reserve(pcount);
    for (json &rawPatientInfo : value) {
        PatientInfo pinfo;
        pinfo.fromJSON(rawPatientInfo);
        this->patients.push_back(pinfo);
    }
}

void PatientList::toStream(ostream &out) {
    for (size_t i = 0; i < this->patients.size(); i++) {
        PatientInfo *p = &this->patients[i];
        p->toStream(out);
        out << "\n";
    }
}

} // namespace app
