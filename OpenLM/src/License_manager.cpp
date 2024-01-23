 #include "License_manager.hpp"

using namespace std;


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

map<License_Manager::LicensedProductName, string> License_Manager::licensedProductNameMap = {
    {License_Manager::LicensedProductName::MPW1, "MPW1"},
};

map<string, License_Manager::LicensedProductName> License_Manager::licensedProductNameEnumMap = {
    {"MPW1", License_Manager::LicensedProductName::MPW1},
    };

License_Manager::License_Manager(LicensedProductName licensedProductName) {
    auto it = licensedProductNameMap.find(licensedProductName);
    if (it == licensedProductNameMap.end()) {
        throw std::runtime_error("Invalid licensed product name");
    }
    string productName = it->second;

    bool checkoutSuccess = licenseCheckout(productName);
    if (!checkoutSuccess) {
        std::cerr << "License checkout failed for product: " << productName << std::endl;
        // Handle the failure as necessary
    }
}

bool License_Manager::licenseCheckout(const string &productName) {
    LicenseInfo licenseInfo;
    size_t pc_id_sz = LCC_API_PC_IDENTIFIER_SIZE + 1;
    char pc_identifier[LCC_API_PC_IDENTIFIER_SIZE + 1];
    	CallerInformations callInfo;
	strcpy(callInfo.feature_name,productName.c_str());
  //  CallerInformations callerInfo = {"\0", productName.c_str()};
    LCC_EVENT_TYPE result = acquire_license(&callInfo, nullptr, &licenseInfo);

    if (result == LICENSE_OK) {
        cout << "License OK" << endl;
        if (!licenseInfo.linked_to_pc) {
            cout << "No hardware signature in license file. This is a 'demo' license that works on every PC." << endl;
        }
        return true;  // Successful checkout
    } else {
        cout << "License ERROR :" << endl;
        cout << "    " << stringByEventType.at(result) << endl;
        if (identify_pc(STRATEGY_DEFAULT, pc_identifier, &pc_id_sz, nullptr)) {
            cout << "Hardware id is :" << endl;
            cout << "    " << pc_identifier << endl;
        } else {
            cerr << "Errors in identify_pc" << endl;
        }
        return false;  // Unsuccessful checkout
    }
}
