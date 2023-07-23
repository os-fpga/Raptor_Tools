#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

// Latch flattening
std::unordered_map<std::string, std::string> latch_lut_LUT_strs = {
    {"LATCH", "10101100"},
    {"LATCHN", "10101100"},
    {"LATCHR", "0000101000001100"},
    {"LATCHS", "1111101011111100"},
    {"LATCHNR", "0000101000001100"},
    {"LATCHNS", "1111101011111100"},
    {"LATCHSRE", "1011101111110011111100111111001100000000000000000000000000000000"},
    {"LATCHNSRE", "1111001110111011111100111111001100000000000000000000000000000000"}};

std::unordered_map<std::string, std::string> latch_lut_WIDTH_strs = {
    {"LATCH", "00000000000000000000000000000011"},
    {"LATCHN", "00000000000000000000000000000011"},
    {"LATCHR", "00000000000000000000000000000100"},
    {"LATCHS", "00000000000000000000000000000100"},
    {"LATCHNR", "00000000000000000000000000000100"},
    {"LATCHNS", "00000000000000000000000000000100"},
    {"LATCHSRE", "00000000000000000000000000000110"},
    {"LATCHNSRE", "00000000000000000000000000000110"}};
std::unordered_map<std::string, std::string> latch_ports = {
    {"LATCH", "DGQ"},
    {"LATCHN", "DGQ"},
    {"LATCHR", "DGRQ"},
    {"LATCHS", "DGRQ"},
    {"LATCHNR", "DGRQ"},
    {"LATCHNS", "DGRQ"},
    {"LATCHSRE", "QSRDGE"},
    {"LATCHNSRE", "QSRDGE"}};

std::unordered_map<std::string, std::string> lut_A_port_connections = {
    {"LATCH", "GQD"},
    {"LATCHN", "GDQ"},
    {"LATCHR", "GRQD"},
    {"LATCHS", "GRQD"},
    {"LATCHNR", "GRDQ"},
    {"LATCHNS", "GRDQ"},
    {"LATCHSRE", "RGEQSD"},
    {"LATCHNSRE", "REGQSD"}};

std::unordered_map<std::string, std::string> lut_port_map_LATCH{{"G", "A[2]"}, {"Q", "A[1]"}, {"D", "A[0]"}};
std::unordered_map<std::string, std::string> lut_port_map_LATCHN{{"G", "A[2]"}, {"D", "A[1]"}, {"Q", "A[0]"}};
std::unordered_map<std::string, std::string> lut_port_map_LATCHR{{"G", "A[3]"}, {"R", "A[2]"}, {"Q", "A[1]"}, {"D", "A[0]"}};
std::unordered_map<std::string, std::string> lut_port_map_LATCHS{{"G", "A[3]"}, {"R", "A[2]"}, {"Q", "A[1]"}, {"D", "A[0]"}};
std::unordered_map<std::string, std::string> lut_port_map_LATCHNR{{"G", "A[3]"}, {"R", "A[2]"}, {"D", "A[1]"}, {"Q", "A[0]"}};
std::unordered_map<std::string, std::string> lut_port_map_LATCHNS{{"G", "A[3]"}, {"R", "A[2]"}, {"D", "A[1]"}, {"Q", "A[0]"}};
std::unordered_map<std::string, std::string> lut_port_map_LATCHSRE{{"R", "A[5]"}, {"G", "A[4]"}, {"E", "A[3]"}, {"Q", "A[2]"}, {"S", "A[1]"}, {"D", "A[0]"}};
std::unordered_map<std::string, std::string> lut_port_map_LATCHNSRE{{"R", "A[5]"}, {"E", "A[4]"}, {"G", "A[3]"}, {"Q", "A[2]"}, {"S", "A[1]"}, {"D", "A[0]"}};

std::unordered_map<std::string, std::unordered_map<std::string, std::string>> latch_lut_port_conversion{
    {"LATCH", lut_port_map_LATCH},
    {"LATCHN", lut_port_map_LATCHN},
    {"LATCHR", lut_port_map_LATCHR},
    {"LATCHS", lut_port_map_LATCHS},
    {"LATCHNR", lut_port_map_LATCHNR},
    {"LATCHNS", lut_port_map_LATCHNS},
    {"LATCHSRE", lut_port_map_LATCHSRE},
    {"LATCHNSRE", lut_port_map_LATCHNSRE}};





