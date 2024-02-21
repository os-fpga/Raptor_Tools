#include "License_manager.hpp"
#include <cstring>

using namespace std;

const map<int, string> stringByStrategyId = {
	{STRATEGY_DEFAULT, "DEFAULT"}, {STRATEGY_ETHERNET, "MAC"}, {STRATEGY_IP_ADDRESS, "IP"}, {STRATEGY_DISK, "Disk"}};

const unordered_map<int, string> stringByEventType = {
	{LICENSE_OK, "OK "},
	{LICENSE_FILE_NOT_FOUND, "license file not found "},
	{LICENSE_SERVER_NOT_FOUND, "license server can't be contacted "},
	{ENVIRONMENT_VARIABLE_NOT_DEFINED, "environment variable not defined "},
	{FILE_FORMAT_NOT_RECOGNIZED, "license file has invalid format (not .ini file) "},
	{LICENSE_MALFORMED, "some mandatory field are missing, or data can't be fully read. "},
	{PRODUCT_NOT_LICENSED, "this product is not licensed "},
	{PRODUCT_EXPIRED, "license expired "},
	{LICENSE_CORRUPTED, "license signature didn't match with current license "},
	{IDENTIFIERS_MISMATCH, "Calculated identifier and the one provided in license didn't match"}};

License_Manager::License_Manager(){}

License_Manager::License_Manager(LicensedProductName licensedProductName)  : License_Manager() {
    auto it = licensedProductNameMap.find(licensedProductName);
    if (it == licensedProductNameMap.end()) {
        throw std::runtime_error("Invalid licensed product name");
    }
    string productName = it->second;

    bool checkoutSuccess = licenseCheckout(productName);
    if (!checkoutSuccess) {
        std::cerr << "License checkout failed for product: " << productName << std::endl;
           throw LicenseFatalException();
    }
}

License_Manager::License_Manager(std::string licensedProductName) {
        bool checkoutSuccess = licenseCheckout(licensedProductName);
        if (!checkoutSuccess) {
            std::cerr << "License checkout failed for product: " << licensedProductName << std::endl;
            throw LicenseFatalException();
        }
    }

bool License_Manager::licenseCheckout(const string &productName) {
    string featureName = productName;
    LicenseLocation licLocation = {LICENSE_PATH};
    //const char* env_var_value = std::getenv("LICENSE_LOCATION");
    LicenseInfo licenseInfo;
    size_t pc_id_sz = LCC_API_PC_IDENTIFIER_SIZE + 1;
    char pc_identifier[LCC_API_PC_IDENTIFIER_SIZE + 1];

    LCC_EVENT_TYPE result = acquire_license(nullptr, &licLocation, &licenseInfo);

    if (result == LICENSE_OK) {
        cout << "License for Raptor software OK" << endl;
        CallerInformations callerInfo;        
        strncpy(callerInfo.feature_name, featureName.c_str(), sizeof(callerInfo.feature_name) - 1);
        callerInfo.feature_name[sizeof(callerInfo.feature_name) - 1] = '\0';
        /*
        // turn on below lines for debug 
        cout << "Copied feature name: " << callerInfo.feature_name << endl;
        cout << "Length of callerInfo.feature_name: " << strlen(callerInfo.feature_name) << endl;
        cout << "Length of productName: " << featureName.length() << endl;
        cout << "productNmae is "  << featureName.c_str() << endl;
        */
        //CallerInformations callerInfo = {"\0", "MPW1"};  // another way to send call to CallerInformations
        result = acquire_license(&callerInfo, &licLocation, &licenseInfo);
        if (result == LICENSE_OK) {
            cout << productName.c_str() << "  is licensed" << endl;
            if (!licenseInfo.linked_to_pc) {
            cout << "No hardware signature in license file. This is a 'demo' license that works on every PC." << endl;
            }
            return true;
        } else {
            cout << productName.c_str() << "  is NOT licensed because " << stringByEventType.at(result) << endl;
            return false;
        }
    } else {
        cout << "License ERROR:" << endl;
        cout << "    " << stringByEventType.at(result) << endl;
        if (identify_pc(STRATEGY_DEFAULT, pc_identifier, &pc_id_sz, nullptr)) {
            cout << "PC signature is:" << endl;
            cout << "    " << pc_identifier << endl;
        } else {
            cout << "Errors in identify_pc" << endl;
        }
        return false;
    }
}


const vector<string> License_Manager::split_string(const string &licensePositions, char splitchar) {
	std::stringstream streamToSplit(licensePositions);
	std::string segment;
	std::vector<string> seglist;

	while (std::getline(streamToSplit, segment, splitchar)) {
		seglist.push_back(segment);
	}
	return seglist;
}
License_Manager::~License_Manager(){}