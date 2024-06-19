#pragma once
/*
 * @file reconstruct_ram36k.h
 * @author Manadher Kharroubi (manadher@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-01-
 *
 * @copyright Copyright (c) 2024
 */

#include <set>

#include "reconstruct_utils.h"

struct TDP_RAM36K_instance {
  std::string str_1 = "00000000000000000000000000000001";
  std::string str_2 = "00000000000000000000000000000010";
  std::string str_4 = "00000000000000000000000000000100";
  std::string str_9 = "00000000000000000000000000001001";
  std::string str_18 = "00000000000000000000000000010010";
  std::string str_36 = "00000000000000000000000000100100";
  std::string str_26_zeroes = "00000000000000000000000000";
  std::string read_mode_A;
  std::string read_mode_B;
  std::string write_mode_A;
  std::string write_mode_B;
  std::set<std::string> TDP_RAM36K_internal_signals = {
      "DATA_OUT_A1", "DATA_OUT_A2", "DATA_OUT_B1", "DATA_OUT_B2",
      "WDATA_A1",    "WDATA_A2",    "WDATA_B1",    "WDATA_B2"};
  std::string per_instance_sig_name(std::string sn, int number) {
    std::stringstream ss;
    std::string num;
    ss << number;
    ss >> num;
    for (auto &sg : TDP_RAM36K_internal_signals) {
      if (sn.find(sg) == 0) {
        return sn + std::string("_") + num;
      }
    }
    return sn;
  }
  std::unordered_map<std::string, int> parameterSize = {
      {"INIT", 32768},       {"INIT_PARITY", 4096}, {"READ_WIDTH_A", 32},
      {"WRITE_WIDTH_A", 32}, {"READ_WIDTH_B", 32},  {"WRITE_WIDTH_B", 32}};
  std::unordered_map<std::string, std::string> parameters = {
      {"INIT", ""},
      {"INIT_PARITY", ""},
      {"READ_WIDTH_A", "00000000000000000000000000100100"},
      {"WRITE_WIDTH_A", "00000000000000000000000000100100"},
      {"READ_WIDTH_B", "00000000000000000000000000100100"},
      {"WRITE_WIDTH_B", "00000000000000000000000000100100"}};
  bool set_param(const std::string &par_name, std::string value) {
    if (parameters.find(par_name) != end(parameters)) {
      // Implementing expansion and truncation of parameters to their specified
      // lengths
      if (value.size() < parameterSize[par_name]) {
        value =
            std::string(parameterSize[par_name] - value.size(), '0') + value;
      } else if (value.size() > parameterSize[par_name]) {
        value = value.substr(parameterSize[par_name] - value.size());
      }
      parameters[par_name] = value;
      return true;
    }
    return false;
  }
  // Collapsed assigns
  unordered_map<string, string> TDP_RAM36K_to_RS_TDP36K_collapsed_assigns = {
      {"FLUSH2", "$false"},
      {"RDATA_B2[10]", "RDATA_B[26]"},
      {"RDATA_B2[7]", "RDATA_B[23]"},
      {"RDATA_B2[6]", "RDATA_B[22]"},
      {"RDATA_B2[4]", "RDATA_B[20]"},
      {"RDATA_B2[0]", "RDATA_B[16]"},
      {"RDATA_A2[17]", "RPARITY_A[3]"},
      {"RDATA_A2[16]", "RPARITY_A[2]"},
      {"RDATA_B2[15]", "RDATA_B[31]"},
      {"RDATA_A2[11]", "RDATA_A[27]"},
      {"RDATA_A2[9]", "RDATA_A[25]"},
      {"RDATA_A2[8]", "RDATA_A[24]"},
      {"RDATA_A2[7]", "RDATA_A[23]"},
      {"RDATA_A2[4]", "RDATA_A[20]"},
      {"RDATA_A2[0]", "RDATA_A[16]"},
      {"WDATA_B2[17]", "WPARITY_B[3]"},
      {"RDATA_B2[8]", "RDATA_B[24]"},
      {"RDATA_A2[1]", "RDATA_A[17]"},
      {"WDATA_B2[16]", "WPARITY_B[2]"},
      {"WDATA_B2[15]", "WDATA_B[31]"},
      {"WDATA_B2[14]", "WDATA_B[30]"},
      {"WDATA_B2[12]", "WDATA_B[28]"},
      {"WDATA_B2[10]", "WDATA_B[26]"},
      {"WDATA_B2[6]", "WDATA_B[22]"},
      {"RDATA_B2[14]", "RDATA_B[30]"},
      {"WDATA_B2[5]", "WDATA_B[21]"},
      {"WDATA_B2[3]", "WDATA_B[19]"},
      {"WDATA_B2[1]", "WDATA_B[17]"},
      {"WDATA_A2[17]", "WPARITY_A[3]"},
      {"WDATA_A2[16]", "WPARITY_A[2]"},
      {"WDATA_A2[14]", "WDATA_A[30]"},
      {"WDATA_A2[10]", "WDATA_A[26]"},
      {"RDATA_A2[14]", "RDATA_A[30]"},
      {"WDATA_A2[9]", "WDATA_A[25]"},
      {"WDATA_A2[8]", "WDATA_A[24]"},
      {"WDATA_A2[6]", "WDATA_A[22]"},
      {"WDATA_A2[3]", "WDATA_A[19]"},
      {"RDATA_B2[12]", "RDATA_B[28]"},
      {"RDATA_A2[5]", "RDATA_A[21]"},
      {"WDATA_A2[2]", "WDATA_A[18]"},
      {"WDATA_A2[1]", "WDATA_A[17]"},
      {"WDATA_B2[4]", "WDATA_B[20]"},
      {"WDATA_A2[0]", "WDATA_A[16]"},
      {"RDATA_B2[1]", "RDATA_B[17]"},
      {"WDATA_A2[13]", "WDATA_A[29]"},
      {"ADDR_B2[13]", "ADDR_B[13]"},
      {"ADDR_B2[11]", "ADDR_B[11]"},
      {"ADDR_B2[5]", "ADDR_B[5]"},
      {"ADDR_B2[4]", "ADDR_B[4]"},
      {"ADDR_B2[3]", "ADDR_B[3]"},
      {"ADDR_A2[13]", "ADDR_A[13]"},
      {"ADDR_A2[12]", "ADDR_A[12]"},
      {"ADDR_A2[11]", "ADDR_A[11]"},
      {"WDATA_A2[12]", "WDATA_A[28]"},
      {"ADDR_A2[10]", "ADDR_A[10]"},
      {"ADDR_A2[9]", "ADDR_A[9]"},
      {"ADDR_A2[7]", "ADDR_A[7]"},
      {"WDATA_B2[7]", "WDATA_B[23]"},
      {"ADDR_B2[8]", "ADDR_B[8]"},
      {"ADDR_A2[6]", "ADDR_A[6]"},
      {"ADDR_A2[5]", "ADDR_A[5]"},
      {"ADDR_A2[4]", "ADDR_A[4]"},
      {"WDATA_A1[15]", "WDATA_A[15]"},
      {"WDATA_A1[14]", "WDATA_A[14]"},
      {"RDATA_A2[3]", "RDATA_A[19]"},
      {"WDATA_A1[11]", "WDATA_A[11]"},
      {"ADDR_B2[7]", "ADDR_B[7]"},
      {"WDATA_A1[7]", "WDATA_A[7]"},
      {"WDATA_B1[3]", "WDATA_B[3]"},
      {"RDATA_A2[13]", "RDATA_A[29]"},
      {"ADDR_B2[0]", "ADDR_B[0]"},
      {"WDATA_A1[6]", "WDATA_A[6]"},
      {"WDATA_A1[3]", "WDATA_A[3]"},
      {"ADDR_B2[2]", "ADDR_B[2]"},
      {"WDATA_A1[1]", "WDATA_A[1]"},
      {"ADDR_B1[9]", "ADDR_B[9]"},
      {"REN_A1", "REN_A"},
      {"RDATA_B1[3]", "RDATA_B[3]"},
      {"WDATA_A1[0]", "WDATA_A[0]"},
      {"RDATA_B2[5]", "RDATA_B[21]"},
      {"ADDR_B1[5]", "ADDR_B[5]"},
      {"ADDR_B1[1]", "ADDR_B[1]"},
      {"RDATA_B2[17]", "RPARITY_B[3]"},
      {"WDATA_B2[11]", "WDATA_B[27]"},
      {"WEN_A2", "WEN_A"},
      {"WDATA_A1[10]", "WDATA_A[10]"},
      {"WDATA_A1[2]", "WDATA_A[2]"},
      {"BE_B1[1]", "BE_B[1]"},
      {"WDATA_B2[13]", "WDATA_B[29]"},
      {"WDATA_A1[17]", "WPARITY_A[1]"},
      {"ADDR_A1[2]", "ADDR_A[2]"},
      {"ADDR_B1[13]", "ADDR_B[13]"},
      {"RDATA_B2[2]", "RDATA_B[18]"},
      {"RDATA_B1[1]", "RDATA_B[1]"},
      {"BE_A1[1]", "BE_A[1]"},
      {"WDATA_B2[2]", "WDATA_B[18]"},
      {"ADDR_A1[7]", "ADDR_A[7]"},
      {"ADDR_B1[12]", "ADDR_B[12]"},
      {"ADDR_A1[0]", "ADDR_A[0]"},
      {"RDATA_B2[9]", "RDATA_B[25]"},
      {"ADDR_B1[0]", "ADDR_B[0]"},
      {"BE_A1[0]", "BE_A[0]"},
      {"WDATA_A1[4]", "WDATA_A[4]"},
      {"ADDR_B1[4]", "ADDR_B[4]"},
      {"WDATA_B1[13]", "WDATA_B[13]"},
      {"CLK_B1", "CLK_B"},
      {"RDATA_A1[7]", "RDATA_A[7]"},
      {"ADDR_A1[1]", "ADDR_A[1]"},
      {"REN_B1", "REN_B"},
      {"RDATA_A2[10]", "RDATA_A[26]"},
      {"RDATA_A1[0]", "RDATA_A[0]"},
      {"RDATA_A1[14]", "RDATA_A[14]"},
      {"RDATA_B2[11]", "RDATA_B[27]"},
      {"WEN_B1", "WEN_B"},
      {"CLK_B2", "CLK_B"},
      {"BE_B1[0]", "BE_B[0]"},
      {"WDATA_A2[5]", "WDATA_A[21]"},
      {"RDATA_A1[6]", "RDATA_A[6]"},
      {"WDATA_A1[12]", "WDATA_A[12]"},
      {"ADDR_B2[12]", "ADDR_B[12]"},
      {"WDATA_B1[0]", "WDATA_B[0]"},
      {"ADDR_A1[8]", "ADDR_A[8]"},
      {"RDATA_B1[2]", "RDATA_B[2]"},
      {"WDATA_A1[13]", "WDATA_A[13]"},
      {"WDATA_B1[9]", "WDATA_B[9]"},
      {"ADDR_B1[14]", "ADDR_B[14]"},
      {"RDATA_B1[11]", "RDATA_B[11]"},
      {"BE_B2[1]", "BE_B[3]"},
      {"ADDR_A2[3]", "ADDR_A[3]"},
      {"ADDR_B1[7]", "ADDR_B[7]"},
      {"RDATA_A2[12]", "RDATA_A[28]"},
      {"WDATA_A2[4]", "WDATA_A[20]"},
      {"ADDR_A1[14]", "ADDR_A[14]"},
      {"ADDR_A1[9]", "ADDR_A[9]"},
      {"RDATA_A1[8]", "RDATA_A[8]"},
      {"ADDR_A1[11]", "ADDR_A[11]"},
      {"RDATA_B2[13]", "RDATA_B[29]"},
      {"WEN_A1", "WEN_A"},
      {"ADDR_A1[3]", "ADDR_A[3]"},
      {"REN_A2", "REN_A"},
      {"ADDR_B1[6]", "ADDR_B[6]"},
      {"WDATA_B2[9]", "WDATA_B[25]"},
      {"ADDR_B1[3]", "ADDR_B[3]"},
      {"WDATA_A1[9]", "WDATA_A[9]"},
      {"WDATA_A2[15]", "WDATA_A[31]"},
      {"WDATA_A1[5]", "WDATA_A[5]"},
      {"ADDR_A1[12]", "ADDR_A[12]"},
      {"ADDR_B1[8]", "ADDR_B[8]"},
      {"ADDR_A1[4]", "ADDR_A[4]"},
      {"ADDR_B1[10]", "ADDR_B[10]"},
      {"RDATA_A2[15]", "RDATA_A[31]"},
      {"CLK_A1", "CLK_A"},
      {"RDATA_B2[3]", "RDATA_B[19]"},
      {"ADDR_A1[5]", "ADDR_A[5]"},
      {"ADDR_A1[10]", "ADDR_A[10]"},
      {"RDATA_A2[6]", "RDATA_A[22]"},
      {"WDATA_B1[1]", "WDATA_B[1]"},
      {"WDATA_B1[2]", "WDATA_B[2]"},
      {"RDATA_B1[0]", "RDATA_B[0]"},
      {"WDATA_B1[4]", "WDATA_B[4]"},
      {"WDATA_B1[6]", "WDATA_B[6]"},
      {"WDATA_B1[5]", "WDATA_B[5]"},
      {"WDATA_B1[7]", "WDATA_B[7]"},
      {"ADDR_B2[1]", "ADDR_B[1]"},
      {"WDATA_B1[11]", "WDATA_B[11]"},
      {"WDATA_B1[17]", "WPARITY_B[1]"},
      {"WDATA_B2[0]", "WDATA_B[16]"},
      {"WDATA_B1[12]", "WDATA_B[12]"},
      {"RDATA_B1[12]", "RDATA_B[12]"},
      {"WDATA_B1[15]", "WDATA_B[15]"},
      {"ADDR_B2[10]", "ADDR_B[10]"},
      {"WDATA_A1[8]", "WDATA_A[8]"},
      {"WDATA_B1[16]", "WPARITY_B[0]"},
      {"RDATA_A1[1]", "RDATA_A[1]"},
      {"RDATA_A1[3]", "RDATA_A[3]"},
      {"RDATA_A1[2]", "RDATA_A[2]"},
      {"RDATA_A1[4]", "RDATA_A[4]"},
      {"RDATA_A1[5]", "RDATA_A[5]"},
      {"BE_A2[1]", "BE_A[3]"},
      {"RDATA_A1[9]", "RDATA_A[9]"},
      {"ADDR_A1[6]", "ADDR_A[6]"},
      {"RDATA_A1[10]", "RDATA_A[10]"},
      {"RDATA_A1[11]", "RDATA_A[11]"},
      {"WDATA_A2[11]", "WDATA_A[27]"},
      {"WDATA_A2[7]", "WDATA_A[23]"},
      {"RDATA_A1[12]", "RDATA_A[12]"},
      {"RDATA_A1[13]", "RDATA_A[13]"},
      {"WDATA_B1[10]", "WDATA_B[10]"},
      {"WDATA_B1[8]", "WDATA_B[8]"},
      {"RDATA_A1[15]", "RDATA_A[15]"},
      {"ADDR_A2[8]", "ADDR_A[8]"},
      {"RDATA_A1[16]", "RPARITY_A[0]"},
      {"RDATA_B1[4]", "RDATA_B[4]"},
      {"ADDR_B2[9]", "ADDR_B[9]"},
      {"RDATA_B1[5]", "RDATA_B[5]"},
      {"ADDR_B2[6]", "ADDR_B[6]"},
      {"RDATA_B1[17]", "RPARITY_B[1]"},
      {"RDATA_B1[6]", "RDATA_B[6]"},
      {"RDATA_B1[7]", "RDATA_B[7]"},
      {"ADDR_B1[11]", "ADDR_B[11]"},
      {"RDATA_A1[17]", "RPARITY_A[1]"},
      {"RDATA_B1[8]", "RDATA_B[8]"},
      {"RDATA_B1[9]", "RDATA_B[9]"},
      {"RDATA_B1[10]", "RDATA_B[10]"},
      {"WDATA_A1[16]", "WPARITY_A[0]"},
      {"ADDR_B1[2]", "ADDR_B[2]"},
      {"RDATA_B1[13]", "RDATA_B[13]"},
      {"RDATA_B2[16]", "RPARITY_B[2]"},
      {"RDATA_B1[14]", "RDATA_B[14]"},
      {"RDATA_B1[15]", "RDATA_B[15]"},
      {"RDATA_A2[2]", "RDATA_A[18]"},
      {"RDATA_B1[16]", "RPARITY_B[0]"},
      {"FLUSH1", "$false"},
      {"WEN_B2", "WEN_B"},
      {"WDATA_B2[8]", "WDATA_B[24]"},
      {"ADDR_A1[13]", "ADDR_A[13]"},
      {"ADDR_A2[1]", "ADDR_A[1]"},
      {"REN_B2", "REN_B"},
      {"CLK_A2", "CLK_A"},
      {"BE_A2[0]", "BE_A[2]"},
      {"WDATA_B1[14]", "WDATA_B[14]"},
      {"BE_B2[0]", "BE_B[2]"},
      {"ADDR_A2[0]", "ADDR_A[0]"},
      {"ADDR_A2[2]", "ADDR_A[2]"}};

  std::string conf_code(std::string W) {
    return W == str_36   ? "110"
           : W == str_18 ? "010"
           : W == str_9  ? "100"
           : W == str_4  ? "001"
           : W == str_2  ? "011"
                         : "101";
  }

  std::string get_MODE_BITS() {
    read_mode_A = conf_code(parameters["READ_WIDTH_A"]);
    read_mode_B = conf_code(parameters["WRITE_WIDTH_B"]);
    write_mode_A = conf_code(parameters["WRITE_WIDTH_A"]);
    write_mode_B = conf_code(parameters["WRITE_WIDTH_B"]);
    std::string MODE_BITS = std::string("0") + read_mode_A + read_mode_B +
                            write_mode_A + write_mode_B + std::string(29, '0') +
                            read_mode_A + read_mode_B + write_mode_A +
                            write_mode_B + std::string(27, '0');
    return MODE_BITS;
  };
  void print(std::ostream &ofs) {
    std::string rs_prim = "RS_TDP36K";
    port_connections["$false"] = "$false";
    port_connections["$true"] = "$true";
    port_connections["$undef"] = "$undef";
    ofs << ".subckt " << rs_prim << " ";
    std::string dont_care_clock;
    // Find a clock already connected to the BRAM to be used as don't care clock
    // for other unconnected clocks
    for (auto &cn : TDP_RAM36K_to_RS_TDP36K_collapsed_assigns) {
      if (port_connections.find(cn.second) != port_connections.end()) {
        std::string high_conn = port_connections[cn.second];
        if (cn.first.find("CLK") != std::string::npos) {
          if (high_conn != "$undef") {
            dont_care_clock = high_conn;
            break;
          }
        }
      }
    }
    for (auto &cn : TDP_RAM36K_to_RS_TDP36K_collapsed_assigns) {
      if (port_connections.find(cn.second) != port_connections.end()) {
        std::string high_conn = port_connections[cn.second];
        if (cn.first.find("CLK") == std::string::npos) {
          // Unconnected Data signals can be ommited from connection list,
          // not tie-ing them off to $undef (Constant 0) benefits FMax
          if (high_conn != "$undef") {
            ofs << " " << cn.first;
            ofs << "=" << high_conn;
          }
        } else {
          ofs << " " << cn.first;
          if (high_conn == "$undef") {
            // We cannot have undriven BRAM clock pins nor clock pins driven by
            // constants ($undef is constant 0), clock pins have to be driven by
            // legal clocks. If the clock is assigned to $undef (Don't care) in
            // the original netlist, any clock connected to the block will do.
            ofs << "=" << dont_care_clock;
          } else {
            ofs << "=" << high_conn;
          }
        }
      } else {
        // std::cout << "WARN: Un-connected " << cn.second
        //           << " from TDP_RAM36K then no connection for " << cn.first
        //           << " from RS_TDP36K" << std::endl;
      }
      // for (auto &force : port_connections) {
      //   if (TDP_RAM36K_to_RS_TDP36K_collapsed_assigns.find(force.first) ==
      //       end(TDP_RAM36K_to_RS_TDP36K_collapsed_assigns)) {
      //     std::cout << "WARN: Critical forcing the connection of missing port
      //     "
      //               << force.first << " in cell RS_TDP36K to " <<
      //               force.second
      //               << std::endl;
      //   }
      // }
    }
    ofs << std::endl;
    ofs << ".param MODE_BITS " << get_MODE_BITS() << std::endl;
    ofs << ".param INIT_i "
        << get_init_i1(parameters["INIT"], parameters["INIT_PARITY"])
        << std::endl;
  }
  static std::string extract_sram1(const std::string &init,
                                   const std::string &init_parity) {
    std::string sram1(18432, '0');
    for (int i = 0; i < 2048; i += 2) {
      sram1.replace((i * 9), 16, init.substr(i * 16, 16));
      sram1.replace(((i + 2) * 9) - 2, 2, init_parity.substr(i * 2, 2));
    }
    return sram1;
  }
  static std::string extract_sram2(const std::string &init,
                                   const std::string &init_parity) {
    std::string sram2(18432, '0');
    for (int i = 1; i < 2048; i += 2) {
      sram2.replace((i - 1) * 9, 16, init.substr(i * 16, 16));
      sram2.replace(((i + 1) * 9) - 2, 2, init_parity.substr(i * 2, 2));
    }
    return sram2;
  }

  static std::string get_init_i1(std::string &init, std::string &init_parity) {
    std::reverse(begin(init), end(init));
    std::reverse(begin(init_parity), end(init_parity));
    std::string res =
        extract_sram1(init, init_parity) + extract_sram2(init, init_parity);
    std::reverse(begin(init), end(init));
    std::reverse(begin(init_parity), end(init_parity));
    std::reverse(begin(res), end(res));
    return res;
  }
  std::unordered_map<std::string, std::string> port_connections;
};
