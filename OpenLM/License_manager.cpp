#include "License_manager.hpp"
#include <cstring>

using namespace std;

const map<int, string> stringByStrategyId = {
	{STRATEGY_DEFAULT, "DEFAULT"}, {STRATEGY_ETHERNET, "MAC"}, {STRATEGY_IP_ADDRESS, "IP"}, {STRATEGY_DISK, "Disk"}};

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
License_Manager::~License_Manager(){}

