#include "License_manager.hpp"

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
	{PRODUCT_NOT_LICENSED, "this product was not licensed "},
	{PRODUCT_EXPIRED, "license expired "},
	{LICENSE_CORRUPTED, "license signature didn't match with current license "},
	{IDENTIFIERS_MISMATCH, "Calculated identifier and the one provided in license didn't match"}};

// map<License_Manager::LicensedProductName, string> License_Manager::licensedProductNameMap = {
//     {License_Manager::LicensedProductName::MPW1, "MPW1"}};

//  map<string, License_Manager::LicensedProductName> License_Manager::licensedProductNameEnumMap = {
//      {"RSMPW1", License_Manager::LicensedProductName::MPW1},
//   };

License_Manager::License_Manager(LicensedProductName licensedProductName) {
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

bool License_Manager::licenseCheckout(const string &productName) {
    LicenseInfo licenseInfo;
	char hw_identifier[LCC_API_PC_IDENTIFIER_SIZE + 1];
	size_t bufSize = LCC_API_PC_IDENTIFIER_SIZE + 1;
	ExecutionEnvironmentInfo exec_env_info;
	for (const auto& x : stringByStrategyId) {
		if (identify_pc(static_cast<LCC_API_HW_IDENTIFICATION_STRATEGY>(x.first), hw_identifier, &bufSize,
						&exec_env_info)) {
		//	std::cout << x.second << ':' << hw_identifier << std::endl;
		} else {
			std::cout << x.second << ": NA" << endl;
		}
	}
    	CallerInformations callInfo;
	strcpy(callInfo.feature_name,productName.c_str());
	LicenseLocation licLocation = {LICENSE_PATH};
     const char* env_var_value = std::getenv("LICENSE_LOCATION");
		if (env_var_value != nullptr && env_var_value[0] != '\0') {
			const vector<string> declared_licenses = split_string(string(env_var_value), ';');
			for (string fname : declared_licenses) {
				ifstream license_file(fname);
               std::copy(fname.begin(), fname.end(), licLocation.licenseData); }}
    LCC_EVENT_TYPE result = acquire_license(&callInfo, &licLocation, &licenseInfo);

    if (result == LICENSE_OK) {
        cout << "License OK" << endl;
        if (!licenseInfo.linked_to_pc) {
            cout << "No hardware signature in license file. This is a 'demo' license that works on every PC." << endl;
        }
        return true;  // Successful checkout
    } else {
        cout << "License ERROR :" << endl;
        cout << "    " << stringByEventType.at(result) << endl;
        return false;  // Unsuccessful checkout
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