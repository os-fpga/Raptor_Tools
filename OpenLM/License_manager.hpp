#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <unordered_map>
#include <licensecc/licensecc.h>
#include <fstream>
#include <string.h>
#include <iomanip>
#include <sstream>
#include <string>
using namespace std;

class License_Manager {
public:

    enum class LicensedProductName {
        DE,
    };

    map<License_Manager::LicensedProductName, string> licensedProductNameMap = {
        {License_Manager::LicensedProductName::DE, "DE"}
    };

    License_Manager(LicensedProductName licensedProductName);
    License_Manager(string licensedProductName);
    

    bool licenseCheckout(const string &productName);
    struct LicenseFatalException : public exception {
        const char *what() const throw() {
            return "License was not acquired due to a fatal error";
        }
    };

    struct LicenseCorrectableException : public exception {
        const char *what() const throw() {
            return "License was not acquired due to a correctable error";
        }
    };

    map<string, LicensedProductName> licensedProductNameEnumMap = {
        {"DE", License_Manager::LicensedProductName::DE}    
};

    ~License_Manager();
private:
    License_Manager();
};
