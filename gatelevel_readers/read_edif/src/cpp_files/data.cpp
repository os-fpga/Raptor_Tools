/*
 * data.cpp
 *
 *  Created on: Aug 3, 2022
 *      Author: rjafari
 */

#include <assert.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <tuple>
#include <iostream>
#include <map>
#include <bits/stdc++.h>
#include "data.h"
#include "edif_blif.h"
#include "read_edif.h"
#include "simple_netlist_beta.h"

/*
 * Keywords
 * All the keywords will be first declared
 *
 */
const std::string EDIF = "edif";
const std::string CELL = "cell";
// ports section
const std::string INTERFACE = "interface";
const std::string PORT = "port";
const std::string ARRAY = "array";
const std::string DIRECTION = "direction";
const std::string CONTENTS = "contents";
const std::string INSTANCE = "instance";
const std::string INSTANCEREF = "instanceRef";
const std::string RENAME = "rename";
const std::string DESIGN = "design";
const std::string NET = "net";
const std::string LUT = "$lut";
const std::string PORTREF = "portRef";
const std::string MEMBER = "member";
const std::string INTEGER_I = "integer";
const std::string GND = "GND";
const std::string VCC = "VCC";

/*
 * This function is to compare the two strings either they are equal or not.
 * First it will check the size of the two strings and then it will compare it
 * char by char
 */
bool string_compare(const std::string &f_name, const std::string &ext) {
  if (ext.size() > f_name.size())
    return false;
  auto ext_it = ext.rbegin();
  auto f_it = f_name.rbegin();
  while (ext_it != ext.rend() && f_it != f_name.rend()) {
    if (*ext_it != *f_it)
      return false;
    ++ext_it;
    ++f_it;
  }
  return true;
}

/*
 * Handle remaining
 */

struct SNode *handle_renaiming(struct SNode *head, std::string &original, std::string &renamed) {
  
  struct SNode *current = head;
  current = current->next;
            if (current->type == 0) {
              current = current->next;
              if (string_compare(RENAME, current->value)) {
                current = current->next;
                renamed = current->value;
                current = current->next;
                original = current->value;
                current = current->next;
              }
            //
            } else if (current->type == 2) {
          renamed = current->value;
          original = current->value;
        }
          //  current = current->next;
  return current;
}

/*
 *
 */
std::string find_top_(struct SNode *head) {
  std::string top;
  struct SNode *current = head;
  while (current != NULL) {
    if (current->type == 2) {
      std::string input_string = current->value;
      if (string_compare(EDIF, input_string)) {
        current = current->next;
        top = current->value;
        break;
      }
    }
    current = current->next;
  }
  return top;
}


/*
 * This function will extract the ports of each cell
 */
void find_ports(struct SNode *head, int cell_start,
                int cell_end) {

  struct SNode *current = head;
  std::string port_name_renamed;
  std::string port_size;
  std::string port_direction;
  std::string port_name_original;
  int c_e=0;
 // int c_start = cell_start;
  while (c_e >= 0) {
    if ( (current->type == 5)) {
      c_e = ((current->list_counter)-cell_start);
    }
    int port_start=0;
    if (current->type == 0) {
      port_start = current->list_counter;
    }
    if (current->type == 2) {
      if (string_compare(PORT, current->value)) {

        port_size = "0";
        current = current->next;
        if (current->type == 0) {
          current = current->next;
          if (string_compare(RENAME, current->value)) {
            current = current->next;
            port_name_renamed = current->value;
            current = current->next;
            port_name_original = current->value;
          }
          if (string_compare(ARRAY, current->value)) {
            current = handle_renaiming(current,port_name_original,port_name_renamed);
            current = current->next;
            port_size = current->value;
          }
          current = current->next;
        } else if (current->type == 2) {
          port_name_renamed = current->value;
          port_name_original = current->value;
        }
        current = current->next;
        current = current->next;
        current = current->next;
        port_direction = current->value;

        ports.push_back(std::make_tuple(port_name_renamed, port_size,
                                        port_direction, port_name_original));
      }
    }
    current = current->next;
  }
}


void map_cell_ports(struct SNode *head) {


  int cell_start = 0;
  int cell_end = 0;
  std::string cell_name_orig;
  std::string cell_name_renamed;


  struct SNode *current = head;
  while (current != NULL) {
    if (current->type == 0) {
      cell_start = current->list_counter;
    }
    if (current->type == 5) {
      cell_end = current->list_counter;
    }
    if (current->type == 2) {
      std::string input_string = current->value;
      if (string_compare(CELL, input_string)) {
        current = handle_renaiming(current,cell_name_orig,cell_name_renamed);
        find_ports(current, cell_start, cell_end);
        cell_ports.insert({ cell_name_renamed, ports });
        ports.clear();
        cell_names.insert({ cell_name_renamed, cell_name_orig });
      }
    }
    current = current->next;
  }
}

void resolve_instance(struct SNode *head) {
  instance ins;
  std::string instance_name_orig;
  std::string instance_name_renamed;
  std::string CELL_REFERENCE;
  std::string Property_lut;
  std::string Property_width;
  std::string property_lut_type;
  int ins_start=0;
  int ins_end = 0;
  struct SNode *current = head;
  std::vector<std::tuple<std::string, std::string, std::string, std::string> >
  temprory_vector;
  std::vector<std::pair<std::string, std::string> > conn;
  std::vector<std::pair<std::string, int> > p_size;
  // print the map to check
  while (current != NULL) {
     if (current->type == 0) {
      ins_start = current->list_counter;
    }
    if (current->type == 5) {
      ins_end = current->list_counter;
    }
    if (current->type == 2) {
      std::string input_string = current->value;
      if (string_compare(INSTANCE, input_string)) {
        current = handle_renaiming(current,instance_name_orig,instance_name_renamed);
        ins.instance_name_real = instance_name_orig;
         int c_e=0;

      while (c_e >= 0) {
        if ( (current->type == 5)) {
        c_e = ((current->list_counter)-ins_start);
      }
      if ( (current->type == 2)) {
         if (string_compare("cellRef", current->value)){
          current = current->next;
          CELL_REFERENCE = current->value;
         }
         if (string_compare("property", current->value)){
          current = current->next;
            if (string_compare("LUT", current->value)){
          current = current->next;
          current = current->next;
         property_lut_type = current->value;
         current = current->next;
          Property_lut = current->value;
            }
          if (string_compare("WIDTH", current->value)){
          current = current->next;
          current = current->next;
         //property_lut_type = current->value;
         current = current->next;
          Property_width = current->value;
            }
         }

      }
       current = current->next;
    } 
       
      
        auto it = cell_ports.find(CELL_REFERENCE);
        if (it != cell_ports.end())
          temprory_vector = it->second;
        auto cell_orig_it = cell_names.find(CELL_REFERENCE);
        std::string cell_orig_name;
        if (cell_orig_it != cell_names.end())
          cell_orig_name = cell_orig_it->second;
        ins.instance_name_real = cell_orig_name;
        // dedicated block for lut data
        if (string_compare(LUT, cell_orig_name)) {
         
          bool is_hex;
         // property_lut_type = current->value;
          if (string_compare(INTEGER_I, property_lut_type)) {
            is_hex = false;
          } else {
            is_hex = true;
          }
          int Property_width_i = stoi(Property_width);
          std::string p_name = get<0>(temprory_vector[0]);
          p_size.push_back(std::make_pair(p_name, Property_width_i));
          if (Property_width_i > 1) {
            p_name = p_name + "[";
            for (int ip = 0; ip <= (Property_width_i - 1); ip++) {
              std::string p_name_new = p_name + std::to_string(ip) + "]";
              conn.push_back(std::make_pair(p_name_new, "$undef"));
            }
          } else {
            conn.push_back(std::make_pair(p_name, "$undef"));
          }
          p_name = get<0>(temprory_vector[1]);
          p_size.push_back(
              std::make_pair(p_name, stoi(get<1>(temprory_vector[1]))));
          conn.push_back(std::make_pair(p_name, p_name));
          get_truth_table(Property_lut, Property_width_i, is_hex);
        } else {
          for (unsigned int ip = 0; ip < temprory_vector.size(); ip++) {
            std::string p_name = get<0>(temprory_vector[ip]);
            int size = stoi(std::get<1>(temprory_vector[ip]));
            p_size.push_back(std::make_pair(p_name, size));
            if (size == 0) {
              conn.push_back(std::make_pair(p_name, "$undef"));
            } else {
              for (auto s = 0; s < size; s++) {
                p_name = (std::get<3>(temprory_vector[ip])) + "[" +
                         to_string(s) + "]";
                conn.push_back(std::make_pair(p_name, "$undef"));
              }
            }
          }
        }
        ins.conn = conn;
        ins.tt = tt_vector;
        ins.sizes = p_size;
        // map the cell ports
        instance_.insert({ instance_name_renamed, ins });
        conn.clear();
        p_size.clear();
        tt_vector.clear();
      }
    }
    current = current->next;
  }
}

void seperate_ports(std::string top_name) {
  auto it = cell_ports.find(top_name);
  ports = it->second;
  for (unsigned int i = 0; i < ports.size(); i++) {
    int size = stoi(std::get<1>(ports[i]));
    if (string_compare("INPUT", get<2>(ports[i]))) {
      std::string port_name = std::get<3>(ports[i]);
      if (size == 0) {
        in_ports.push_back(std::get<3>(ports[i]));

      } else {
        for (auto s = 0; s < size; s++) {
          in_ports.push_back((std::get<3>(ports[i])) + "[" + to_string(s) +
                             "]");
        }
      }
    } else if (string_compare("OUTPUT", get<2>(ports[i]))) {
      if (size == 0) {
        out_ports.push_back(std::get<3>(ports[i]));
        map_output.insert({ std::get<0>(ports[i]), std::get<3>(ports[i]) });
      } else {
        for (auto s = 0; s < size; s++) {
          out_ports.push_back((std::get<3>(ports[i])) + "[" + to_string(s) +
                              "]");
          map_output.insert(
              {((std::get<0>(ports[i])) + "[" + to_string(s) + "]"),
               ((std::get<3>(ports[i])) + "[" + to_string(s) + "]") });
        }
      }
    } else if (string_compare("INOUT", get<2>(ports[i]))) {
      if (size == 0) {
        out_ports.push_back(std::get<3>(ports[i]));
      } else {
        for (auto s = 0; s < size; s++) {
          inout_ports.push_back((std::get<3>(ports[i])) + "[" + to_string(s) +
                                "]");
        }
      }
    }
  }
}

std::vector<std::string> special_vector;
void find_cell_net(struct SNode *head, std::string top_name) {
  std::pair<std::string, std::vector<std::pair<std::string, std::string> > >
  net_pair;
  std::vector<std::tuple<std::string, std::string, std::string, std::string> >
  temprory_vector;
  std::vector<std::pair<std::string, std::string> > temp;
  std::vector<unsigned> row_;
  row_.push_back(1);
  row_.push_back(1);
  int counter = 1;
  int list_depth = 0;
  struct SNode *current = head;
  std::string cell_name;
  std::string net_name;
  std::string net_name_real;
  std::string port_ref;
  std::string net_member;
  std::string member_num;
  std::string instance_ref;
  bool member_exit = false;
  while (current != NULL) {
    if (current->type == 0 || current->type == 5) {
      list_depth = current->list_counter;
    } else if (current->type == 2) {
      if (string_compare(NET, current->value)) {
        current = current->next;
        if (current->type == 0 || current->type == 5) {
          current = current->next;
          if (string_compare(RENAME, current->value)) {
            current = current->next;
            current = current->next;
            net_name = current->value;
          }
        } else {
          net_name = current->value;
        }
        int net_joining_iteration = list_depth - 1;
        while (list_depth > net_joining_iteration) {
          current = current->next;
          if (current->type == 0 || current->type == 5) {
            list_depth = current->list_counter;
          }
          if (current->type == 2) {
            if (string_compare(PORTREF, current->value)) {
              current = current->next;
              if (current->type == 0 || current->type == 5) {
                current = current->next;
                current = current->next;
                port_ref = current->value;
                current = current->next;
                member_num = current->value;
                member_exit = true;
                current = current->next;
              } else {
                port_ref = current->value;
                member_exit = false;
                member_num = "";
              }
              instance_ref = top_name;
              current = current->next;
              if (current->type == 0 || current->type == 5) {
                list_depth = current->list_counter;
              }
              current = current->next;
              if (current->type == 2) {
                if (string_compare("instanceRef", current->value)) {
                  current = current->next;
                  instance_ref = current->value;
                  if (string_compare("VCC", instance_ref)) {
                    net_name = "$true";
                  } else if (string_compare("GND", instance_ref)) {
                    net_name_real = net_name;
                    net_name = "$false";
                  }
                  //  printf("\n member_number is  %s \n", member_num.c_str());
                }
              }
              if (member_exit) {
                // std::cout<<"The instance ref is  " <<instance_ref<<std::endl;
                if (string_compare(instance_ref, top_name)) {

                  auto it = cell_ports.find(top_name);
                  temprory_vector = it->second;
                  int port_size;
                  for (int i = 0; i < temprory_vector.size(); i++) {
                    if (string_compare(port_ref,
                                       std::get<0>(temprory_vector[i]))) {
                      port_size = stoi(std::get<1>(temprory_vector[i]));
                      port_size = port_size - 1;
                      member_num = to_string(port_size - stoi(member_num));
                      member_num = "[" + member_num + "]";
                      break;
                    }
                  }
                } else {
                  std::vector<std::pair<std::string, int> > sizes;
                  auto it = instance_.find(instance_ref);
                  if (it != instance_.end()) {
                    instance ins;
                    ins = it->second;
                    sizes = ins.sizes;
                    int port_size;
                    for (int i = 0; i < sizes.size(); i++) {
                      if (string_compare(port_ref, std::get<0>(sizes[i]))) {
                        port_size = std::get<1>(sizes[i]);
                        port_size = port_size - 1;
                        member_num = to_string(port_size - stoi(member_num));
                        member_num = "[" + member_num + "]";
                        break;
                      }
                    }
                    sizes.clear();
                  }
                }
              }
              port_ref = port_ref + member_num;
              //    std::cout<<"The final member  is  " <<port_ref<<std::endl;
              temp.push_back(std::make_pair(instance_ref, port_ref));
            }
          }
        }
        instance ins;
        for (unsigned int z = 0; z < temp.size(); z++) {
          auto it = instance_.find(temp[z].first);
          if (it != instance_.end()) {
            ins = it->second;
            for (unsigned int i = 0; i < ins.conn.size(); i++) {
              if (ins.conn[i].first == temp[z].second) {
                ins.conn[i].second = net_name;
              }
            }
            it->second = ins;
          }

          // For the special case when the circuit is optimized
          // if it is not in the instance ports means it is in the top module
          // port
          else {
            // std::cout<<"The port not found is  " <<temp[z].second<<std::endl;
            auto ports_it = map_output.find(temp[z].second);
            if (ports_it != map_output.end()) {
              std::string port_real_name = ports_it->second;
              for (auto ou_p = 0; ou_p < out_ports.size(); ou_p++) {
                if (out_ports[ou_p] == port_real_name) {
                  special_vector.push_back(port_real_name);
                  break;
                }
              }
            }
          }
          // adding the .names for $false and the port name thing
          // .names $false name of port
          // 11
        }
        temp.clear();
        if ((special_vector.size() >= 1) &&
            (!(string_compare(net_name, "$false")) ||
             (string_compare(net_name, "$true")))) {
          instance ins_false;
          ins_false.instance_name_real = "$lut";
          ins_false.tt.push_back(row_);
          for (auto sv = 0; sv < special_vector.size(); sv++) {
            if (!(string_compare(net_name, special_vector[sv]))) {
              ins_false.conn.push_back(std::make_pair(net_name, net_name));
              ins_false.conn.push_back(
                  std::make_pair(special_vector[sv], special_vector[sv]));
              instance_.insert({ to_string(counter), ins_false });
              counter++;
              ins_false.conn.clear();
            }
          }
        }
        if ((special_vector.size() >= 1) &&
            ((string_compare(net_name, "$false")) ||
             (string_compare(net_name, "$true")))) {
          // else {
          instance ins_false;
          ins_false.instance_name_real = "$lut";
          ins_false.tt.push_back(row_);
          for (auto sv = 0; sv < special_vector.size(); sv++) {
            ins_false.conn.push_back(std::make_pair(net_name, net_name));
            ins_false.conn.push_back(
                std::make_pair(special_vector[sv], special_vector[sv]));
            instance_.insert({ to_string(counter), ins_false });
            counter++;
            ins_false.conn.clear();
          }
        }
        special_vector.clear();
      }
    }
    current = current->next;
  }
}

void get_truth_table(std::string tt_output_str, int width, bool is_hex) {
 
  std::vector<std::vector<int> > rows;
  std::string property_lut = tt_output_str;
  unsigned long long int index;

  if (is_hex) {
    int pos = property_lut.find("h");

    property_lut = property_lut.substr(pos + 1);
    index = std::stoul(property_lut, nullptr, 16);
  } else {
    index = std::stoll(property_lut);
  }
  int property_wid = width;

  int pp_wid = property_wid;
  int max_tt = 1;
  while (pp_wid != 0) {
    max_tt = (max_tt * 2);
    pp_wid--;
  }

  std::vector<int> property_lut_binary(max_tt, 0);
  int pp_required = max_tt - 1;
  while (index != 0) {
    property_lut_binary.at(pp_required) = (index % 2);
    index /= 2;
    pp_required--;
  }

  int tt_output = max_tt - 1;
  for (int tt = 0; tt < max_tt; tt++) {
    std::vector<unsigned> row_((property_wid + 1), 0);
    int tt_size = property_wid;
    row_.at(tt_size) = property_lut_binary.at(tt_output);
    tt_output--;
    int tt_i = tt;
    while (tt_i > 0) {
      int tt_input = tt_i % 2;
      tt_i = tt_i / 2;
      tt_size--;
      row_.at(tt_size) = tt_input;
    }
    if ((row_[row_.size() - 1]) == 1) {
      tt_vector.push_back(row_);
    }
  }
}


void edif_bilf(const char *argv_1, FILE *argv_2) {
  FILE *fp = fopen(argv_1, "r");
  if (fp == NULL) {
    perror("Failed to open the edif file : ");
  }
  std::stringstream ss;
  std::vector<std::tuple<std::string, std::string, std::string, std::string> >
  temprory_vector;
  printf("parsing the file\n");
  struct SNode *node = snode_parse(fp);
  printf("Getting the top name\n");
  std::string top_name = find_top_(node);
  printf("Mapping cell ports\n");
  map_cell_ports(node);
  //printf("Making the ports on the basis of size\n");
  seperate_ports(top_name);
  printf("Initializing the instance\n");
  resolve_instance(node);
  printf("Getting the nets\n");
  find_cell_net(node, top_name);

  // passing data to simplenetlist.h
  simple_netlist_beta sn;
  sn.name = top_name;
  sn.in_ports = in_ports;
  sn.out_ports = out_ports;
  sn.inout_ports = inout_ports;

  sn.b_print(ss);
  instance INS;
  inst ins;
  for (auto it = instance_.begin(); it != instance_.end(); it++) {
    INS = it->second;
    ins.mod_name_ = INS.instance_name_real;
    if ((string_compare("GND", INS.instance_name_real)) ||
        (string_compare("VCC", INS.instance_name_real))) {
      continue;
    }
    ins.name_ = it->first;
    ins.conns_ = INS.conn;
    ins.truthTable_ = INS.tt;
    ins.blif_print(ss);
  }
  end_print(ss);
  printf("Close the file stream\n");
  fclose(fp);
   fputs(ss.str().c_str(), argv_2);
 // outfile.close();
}