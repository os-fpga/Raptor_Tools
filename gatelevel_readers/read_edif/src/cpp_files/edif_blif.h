#include <assert.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>

//#include "simple_netlist.h"
//#include "read_edif.cpp"
//#include "data.cpp"
using std::ofstream;
void edif_bilf(char *argv_1, char *argv_2) ;
/*{
  FILE *fp = fopen(argv_1, "r");
  if (fp == NULL) {
    perror("Failed to open the edif file : ");
  }
  std::vector<std::tuple<std::string, std::string, std::string, std::string> >
  temprory_vector;
  struct SNode *node = snode_parse(fp);

  std::string top_name = find_top_(node);

  map_cell_ports(node);
  seperate_ports(top_name);
  resolve_instance(node);
  find_cell_net(node, top_name);

  // passing data to simplenetlist.h
  simple_netlist sn;
  ofstream outfile(argv_2, ios::out);
  if (!(outfile.is_open())) {
    perror("Failed to open the blif file : ");
  }
  sn.name = top_name;

  sn.in_ports = in_ports;
  sn.out_ports = out_ports;
  sn.inout_ports = inout_ports;

  sn.b_print(outfile);
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
    ins.blif_print(outfile);
  }
  end_print(outfile);
  printf("Close the file stream\n");
  fclose(fp);
}*/