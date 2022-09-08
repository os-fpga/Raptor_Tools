/*
 * data.h
 *
 *  Created on: Aug 3, 2022
 *      Author: rjafari
 */

#ifndef DATA_H_
#define DATA_H_

#include <assert.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <tuple>
#include <iostream>
#include <map>
#include <bits/stdc++.h>


/*
 * ports vector will have the three strings which holds
 * 1) port name renamed (needed for finding the cell nets from the instance as
 * the renamed name is used all in the netlist)
 * 2) port size
 * 3) port direction
 * 4) port name original (We have to use the original names in the blif to
 * compare)
 */

//  std::vector<std::tuple <port name renamed ,port size , port direction , port
// name original>>ports;
std::vector<std::tuple<std::string, std::string, std::string, std::string> >
ports;

/*
 * this map will have all a cell name and its ports
 * the reason of storing them seperately is because we need them at resolving
 * the instances
 * The ports instances will
 */
std::map<std::string, std::vector<std::tuple<std::string, std::string,std::string, std::string> > >cell_ports;

/*
 * The edif rename the cell names but in blif the original names are needed so
 * the cell names are
 * saved with the corresponding renamed name
 */

std::map<std::string, std::string> cell_names;




/*
 * The data will be forward in the format of instance
 * instance real name will be the $lut, GND etc
 * The conn vector is for forwarding the nets with their corresponding port name
 * The tt vector contains the truth table if it is the lut
 * The sizes keep the port name and size of each instance.
 */
struct instance {
  std::string instance_name_real;
  std::vector<std::pair<std::string, std::string> > conn;
  std::vector<std::vector<unsigned> > tt;
  std::vector<std::pair<std::string, int> > sizes;
};



/*
 * The instance_ports has the name of in
 */
std::map<std::string, instance> instance_;
std::vector<std::vector<unsigned> > tt_vector;

std::map<std::string, std::string> map_output;
std::vector<std::string> in_ports;
std::vector<std::string> out_ports;
std::vector<std::string> inout_ports;

/*
 * This function compares two string in more optimized way.
 */
bool string_compare(const std::string &f_name, const std::string &ext);

/*
 * This function gets the top module name from the netlist
 */
std::string find_top_(struct SNode *head);

/*
 * This function will collect all the ios
 */

void find_ports(struct SNode *head, int cell_start, int cell_end);
/*
 * This function will collect all the ios of the particular cell and map them with the name of cell
 * These cells ios will be used for instantiation
 */

void map_cell_ports(struct SNode *head);

void seperate_ports(std::string top_name);

/*
 * The resolve instance read the edif instance part and create the instance
 * This instances will be mapped in a map with all the information
 * In the instance creation all the parent cell ports were copied in to the instance except for the
 * lut as in case of lut the property_width defines the lut input size so the size is changed for
 * the particular instance of lut
 * At the same point the truth table for the lut is also read from the edif and the data is passed to
 * the get_truth_table which rewrites the truth table in the required manner
 */
void resolve_instance(struct SNode *head);
void get_truth_table(std::string tt_output_str, int width, bool is_hex);
/*
 * The net connecting the ports along with the instance name were read from the edif
 * and the instance_ map is called which finds the instance and in this instance it finds the port
 * present in the conn vector and place the net name next to port
 */
void find_cell_net(struct SNode *head, std::string top_name);





//#include "edif_blif.cpp"
//using std::ofstream;
//void edif_bilf(char *argv_1, char *argv_2);

#endif /* DATA_H_ */
