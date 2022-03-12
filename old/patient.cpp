#include "app.h"

namespace app
{

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

size_t PatientList::getPatientsCount() {
    return this->patients.size();
}

} // namespace app