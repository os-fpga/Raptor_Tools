#pragma once
using namespace std;
#include <string>
#include <cstdlib>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <ctype.h>
#include <sstream>
#ifdef PC
#include <time.h>
#else
#include <sys/time.h>
#endif
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <vector>

class License_Manager
{
public:
    // List of licensed features. Please update this, the map of
    // feature enum -> name (licensedProductNameMap), and the
    // map of feature enum -> num debted licenses (licenseDebitsPerProduct)
    // in License_manager.cpp every time a new licensed feature is added
    enum class LicensedProductName
    {
        READ_VERILOG,
        OPENFPGA_RS,
        YOSYS_RS,
        YOSYS_RS_PLUGIN,
        DE,
        GEMINI,
        RS1GE75,
        RS1GE100,
        RSGE100_ES1,
        RSGE124_L,
        RS1GE3,
        RS1GVTC,
        RS1GE3C,
        MPW1,
        ANALYZE
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
    License_Manager(string licensedProductName);
    ~License_Manager();

private:
    License_Manager();
    License_Manager(const License_Manager &) = delete;
    License_Manager &operator=(const License_Manager &) = delete;
    // Used to set the default license file location
    // bool licensedefaultpath();
    // // Checks out the license
    // bool licenseCheckout(string &productName, int licenseCount, const string &version);
    // // Checks in the license which was checked out earlier
    // bool licenseCheckin(string &productName);


    //  VENDORCODE code;
    // LM_HANDLE *lm_job;
    LicensedProductName mylicensedProductName;
    static map<LicensedProductName, string> licensedProductNameMap;
    static map<LicensedProductName, int> licenseDebitsPerProduct;
    static map<string, License_Manager::LicensedProductName> licensedProductNameEnumMap;
};
