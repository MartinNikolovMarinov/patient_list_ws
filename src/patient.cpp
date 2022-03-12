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

string Request::toJSONStr() const {
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

void Plan::fromJSON(json &rawJSON) {
    this->label = safeReadKey(rawJSON, "label").get<string>();
}

void Plan::toStream(ostream &out) {
    out << "\t\tPlan" << "\n";
    out << "\t\t\t" << "label = " << this->label << "\n";
}

void Prescription::fromJSON(json &rawJSON) {
    this->description = safeReadKey(rawJSON, "description");
    this->label = safeReadKey(rawJSON, "label");
    this->numFractions = safeReadKey(rawJSON, "num_fractions");

    json value = safeReadKey(rawJSON, "plans");
    size_t plansCount = value.size();
    this->plans.reserve(plansCount);
    for (json &rawPlan : value) {
        Plan plan;
        plan.fromJSON(rawPlan);
        this->plans.push_back(plan);
    }
}

void Prescription::toStream(ostream &out) {
    out << "\tPrescription" << "\n";
    out << "\t\t" << "description = " << description << "\n";
    out << "\t\t" << "label = " << label << "\n";
    out << "\t\t" << "num_fractions = " << numFractions << "\n";

    for (auto &&p : this->plans) {
        p.toStream(out);
    }
}

void Diagnose::fromJSON(json &rawJSON) {
    this->description = safeReadKey(rawJSON, "description");
    this->label = safeReadKey(rawJSON, "label");

    json value = safeReadKey(rawJSON, "prescriptions");
    size_t prescriptionCount = value.size();
    this->prescriptions.reserve(prescriptionCount);
    for (json &rawPerscription : value) {
        Prescription perscription;
        perscription.fromJSON(rawPerscription);
        this->prescriptions.push_back(perscription);
    }
}

void Diagnose::toStream(ostream &out) {
    out << "Diagnose" << "\n";
    out << "\t" << "description = " << description << "\n";
    out << "\t" << "label = " << label << "\n";

    for (auto &&p : this->prescriptions) {
        p.toStream(out);
    }
}

void Patient::fromJSON(const string &id, json &rawJSON) {
    this->uri = id;
    json value = safeReadKey(rawJSON, "diagnoses");
    size_t diagnosisCount = value.size();
    this->diagnoses.reserve(diagnosisCount);
    for (json &rawDiagnose : value) {
        Diagnose diagnose;
        diagnose.fromJSON(rawDiagnose);
        this->diagnoses.push_back(diagnose);
    }
}

void Patient::toStream(ostream &out) {
    out << "Patient (uri=" << this->uri << ")" << "\n";
    for (auto &&p : this->diagnoses) {
        p.toStream(out);
    }
}

} // namespace app
