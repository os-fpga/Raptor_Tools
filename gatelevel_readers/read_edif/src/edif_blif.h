#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <tuple>
#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream> // std::stringstream, std::stringbuf
#include <unordered_set>
#include <unordered_map>
#include <ctype.h>
#include <cstring> // strchr ...
#include <cctype>  // isalpha, etc ...
using namespace std;
#include "pprint.hpp"
void get_truth_table(std::string tt_output_str, int width, bool is_hex, std::vector<std::vector<unsigned>> &tt_vector);
void ports_addition(std::vector<std::string>
                        &ports,
                    std::string port_name, int port_size);

void seperate_ports(std::vector<std::tuple<
                        /*Port name original  */ std::string,
                        /*Port name renamed   */ std::string,
                        /*Port direction      */ std::string,
                        /*Port size           */ std::string>>
                        &ports_vector,
                    std::vector<std::string>
                        &in_ports,
                    std::vector<std::string>
                        &out_ports,
                    std::vector<std::string>
                        &inout_ports,
                    bool is_lut, std::string width);

std::string find_corresp_net(std::map<std::string, std::vector<std::pair<

                                                       /*net_name           */ std::string,
                                                       /* net instance ref  */ std::string>>> &
                                 net_reduced_map,
                             std::string ins_ref, std::string port_name);

void edif_blif(FILE *InputFile, const char *edif_bl);