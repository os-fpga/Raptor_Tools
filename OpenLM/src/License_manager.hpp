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

using namespace std;


class License_Manager
{
public:

    enum class LicensedProductName
    {
        MPW1
    };

    struct LicenseFatalException : public exception
    {
        const char *what() const throw()
        {
            return "License was not acquired due to a fatal error ";
        }
    };

    struct LicenseCorrectableException : public exception
    {
        const char *what() const throw()
        {
            return "License was not acquired due to a correctable error";
        }
    };

    License_Manager(LicensedProductName licensedProductName);
    bool licenseCheckout(const string &productName);
    const std::vector<std::string> split_string(const std::string& stringToBeSplit, const char splitchar);
private:
    static map<LicensedProductName, string> licensedProductNameMap;
    static map<string, LicensedProductName> licensedProductNameEnumMap;
};