#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <licensecc/licensecc.h>
#include <fstream>
#include <string.h>
#include <iomanip>
// #include "../library/base/string_utils.h"
// #include "../library/ini/SimpleIni.h"
// #include "../library/os/dmi_info.hpp"
// #include "../library/os/cpu_info.hpp"
// #include "../library/os/dmi_info.hpp"
// #include "../library/os/network.hpp"

using namespace std;
//using namespace license::os;

class License_Manager
{
public:
    // List of licensed features. Please update this, the map of
    // feature enum -> name (licensedProductNameMap), and the
    // map of feature enum -> num debted licenses (licenseDebitsPerProduct)
    // in License_manager.cpp every time a new licensed feature is added
    enum class LicensedProductName
    {
        MPW1
    };

    // struct LicenseFatalException : public exception
    // {
    //     const char *what() const throw()
    //     {
    //         return "License was not acquired due to a fatal error ";
    //     }
    // };

    // struct LicenseCorrectableException : public exception
    // {
    //     const char *what() const throw()
    //     {
    //         return "License was not acquired due to a correctable error";
    //     }
    // };

    License_Manager(LicensedProductName licensedProductName);
    bool licenseCheckout(const string &productName);

private:
    static map<LicensedProductName, string> licensedProductNameMap;
    static map<string, LicensedProductName> licensedProductNameEnumMap;
};