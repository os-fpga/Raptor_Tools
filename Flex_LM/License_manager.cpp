#include "License_manager.hpp"

using namespace std;

// change this string version for every release
// current version 2022.10 is for october release
// std::vector<std::string> ALLOWED_VERSIONS = {"2022.12", "2022.10", "2023.12"};
// std::string version;
// int currentIndex = 0;
// std::string getNextVersion(const std::vector<std::string> &versions, int &index)
// {
//     if (index < versions.size())
//     {
//         std::string currentVersion = versions[index];
//         index++;
//         return currentVersion;
//     }
//     return "";
// }
// std::string homedir = getenv("HOME");
// std::string license_path = homedir + "/.local/Rapidsilicon/raptor.lic";
// char *LICPATH = (char *)license_path.c_str();

map<License_Manager::LicensedProductName, string> License_Manager::licensedProductNameMap = {
    {License_Manager::LicensedProductName::READ_VERILOG, "read_verilog"},
    {License_Manager::LicensedProductName::OPENFPGA_RS, "openfpga_rs"},
    {License_Manager::LicensedProductName::YOSYS_RS, "yosys_rs"},
    {License_Manager::LicensedProductName::YOSYS_RS_PLUGIN, "yosys_rs_plugin"},
    {License_Manager::LicensedProductName::DE, "de"},
    {License_Manager::LicensedProductName::GEMINI, "GEMINI"},
    {License_Manager::LicensedProductName::RS1GE75, "1GE75"},
    {License_Manager::LicensedProductName::RSGE100_ES1, "1GE100-ES1"},
    {License_Manager::LicensedProductName::RS1GVTC, "1GVTC"},
    {License_Manager::LicensedProductName::RSGE124_L, "1GE124-L"},
    {License_Manager::LicensedProductName::RS1GE100, "1GE100"},
    {License_Manager::LicensedProductName::RS1GE3, "1GE3"},
    {License_Manager::LicensedProductName::RS1GE3C, "1GE3C"},
    {License_Manager::LicensedProductName::MPW1, "MPW1"},
    {License_Manager::LicensedProductName::ANALYZE, "analyze"}};

map<string, License_Manager::LicensedProductName> License_Manager::licensedProductNameEnumMap = {
    {"read_verilog", License_Manager::LicensedProductName::READ_VERILOG},
    {"openfpga_rs", License_Manager::LicensedProductName::OPENFPGA_RS},
    {"yosys_rs", License_Manager::LicensedProductName::YOSYS_RS},
    {"yosys_rs_plugin", License_Manager::LicensedProductName::YOSYS_RS_PLUGIN},
    {"de", License_Manager::LicensedProductName::DE},
    {"GEMINI", License_Manager::LicensedProductName::GEMINI},
    {"1GE75", License_Manager::LicensedProductName::RS1GE75},
    {"1GE100-ES1", License_Manager::LicensedProductName::RSGE100_ES1},
    {"1GVTC", License_Manager::LicensedProductName::RS1GVTC},
    {"1GE124-L", License_Manager::LicensedProductName::RSGE124_L},
    {"1GE3", License_Manager::LicensedProductName::RS1GE3},
    {"1GE3C", License_Manager::LicensedProductName::RS1GE3C},
    {"1GE100", License_Manager::LicensedProductName::RS1GE100},
    {"MPW1", License_Manager::LicensedProductName::MPW1},
    {"analyze", License_Manager::LicensedProductName::ANALYZE}};

map<License_Manager::LicensedProductName, int> License_Manager::licenseDebitsPerProduct = {
    {License_Manager::LicensedProductName::READ_VERILOG, 1},
    {License_Manager::LicensedProductName::OPENFPGA_RS, 1},
    {License_Manager::LicensedProductName::YOSYS_RS, 1},
    {License_Manager::LicensedProductName::YOSYS_RS_PLUGIN, 1},
    {License_Manager::LicensedProductName::DE, 1},
    {License_Manager::LicensedProductName::GEMINI, 1},
    {License_Manager::LicensedProductName::RS1GE75, 1},
    {License_Manager::LicensedProductName::RSGE100_ES1, 1},
    {License_Manager::LicensedProductName::RS1GVTC, 1},
    {License_Manager::LicensedProductName::RSGE124_L, 1},
    {License_Manager::LicensedProductName::RS1GE3, 1},
    {License_Manager::LicensedProductName::RS1GE3C, 1},
    {License_Manager::LicensedProductName::RS1GE100, 1},
    {License_Manager::LicensedProductName::MPW1, 1},
    {License_Manager::LicensedProductName::ANALYZE, 1}};

License_Manager::License_Manager()
{
    // if (lc_new_job(0, lc_new_job_arg2, &code, &lm_job))
    // {
    //     lc_perror(lm_job, "lc_new_job failed");
    //     exit(lc_get_errno(lm_job));
    // }
}

License_Manager::License_Manager(LicensedProductName licensedProductName) : License_Manager()
{
    // mylicensedProductName = licensedProductName;
    // this->licenseCheckout(licensedProductNameMap.find(mylicensedProductName)->second,
    //                       licenseDebitsPerProduct.find(mylicensedProductName)->second,
    //                       version);
}

License_Manager::License_Manager(string licensedProductName)
{
    // auto it = licensedProductNameEnumMap.find(licensedProductName);
    // if (it == end(licensedProductNameEnumMap))
    //     throw(LicenseFatalException());
    // if (lc_new_job(0, lc_new_job_arg2, &code, &lm_job))
    // {
    //     lc_perror(lm_job, "lc_new_job failed");
    //     exit(lc_get_errno(lm_job));
    // }
    // mylicensedProductName = it->second;

    // std::string version = getNextVersion(ALLOWED_VERSIONS, currentIndex);
    // while (version != "")
    // {
    //     version = getNextVersion(ALLOWED_VERSIONS, currentIndex);
    // }

    // this->licenseCheckout(licensedProductName,
    //                       licenseDebitsPerProduct.find(mylicensedProductName)->second,
    //                     version);
}

License_Manager::~License_Manager()
{
    // this->licenseCheckin(licensedProductNameMap.find(mylicensedProductName)->second);
    // lc_free_job(lm_job);
}
// bool License_Manager::licensedefaultpath()
// {

//     /* Call lc_set_attr() with the expected location
//     of the application�s license file directory. */
//     (void)lc_set_attr(lm_job, LM_A_LICENSE_DEFAULT, (LM_A_VAL_TYPE)LICPATH);
//     return true;
// }

// bool License_Manager::licenseCheckout(string &productName, int licenseCount,
//                                       const string &version)
// {
//     this->licensedefaultpath();
//     int errorno = lc_checkout(lm_job, productName.c_str(), version.c_str(), licenseCount, LM_CO_WAIT, &code, LM_DUP_NONE);
//     if (errorno)
//     {
//         lc_perror(lm_job, "License Checkout Failed");
//         lc_free_job(lm_job);
//         switch (errorno)
//         {
//         case LM_BUSYNEWSERV:
//         case LM_CANTCONNECT:
//         case LM_SERVBUSY:
//         case LM_NOT_THIS_HOST:
//         case LM_HOSTDOWN:
//             throw(LicenseCorrectableException());
//         default:
//             throw(LicenseFatalException());
//         }
//     }
//     return true;
// }

// bool License_Manager::licenseCheckin(string &productName)
// {
//     lc_checkin(lm_job, productName.c_str(), 0);
//     return true;
// }

// Figure out why this is needed
// void License_Manager::licenattribute()
//{
//    (void)lc_set_attr(lm_job, LM_A_LICENSE_DEFAULT, (LM_A_VAL_TYPE)LICPATH);
//}
