#include "edif_blif.hpp"

struct inst_ed {
  std::string name_;
  std::string mod_name_;
  std::vector<std::pair<std::string, std::string>> conns_;
  std::unordered_map<std::string, std::string> params_;
  std::vector<std::vector<unsigned>> truthTable_;
  inst_ed() {}
  inst_ed(std::string name, std::string mod_name,
          std::vector<std::pair<std::string, std::string>> conn)
      : name_(name), mod_name_(mod_name), conns_(conn) {}
  void p_print_ed(std::ostream &f) {

    f << "Instance : " << name_ << " of module " << mod_name_ << std::endl;
    for (auto &p : conns_) {
      f << "." << p.first << "(" << p.second << "), ";
    }
    f << std::endl;
    for (auto &p : params_) {
      f << p.first << " : " << p.second << endl;
    }

    if ("$lut" == mod_name_.substr(0, 4)) {
      blif_names_print(f);
    }
  }
  void blif_names_print(std::ostream &f) {
    f << ".names";
    for (int idx = conns_.size() - 2; idx >= 0; --idx) {
      string s =
          (conns_[idx].second == "GND_NET") ? "$false" : conns_[idx].second;
      s = (s == "VCC_NET") ? "$true" : s;
      s = (s == "n3") ? "$undef" : s;
      if ('\\' == s[0])
        s[0] = ' ';
      f << " " << s;
    }
    f << " " << conns_.back().second << std::endl;
    for (auto &v : truthTable_) {
      for (long unsigned int i = 0; i < v.size() - 1; ++i) {
        f << v[i];
      }
      f << " " << v[v.size() - 1] << endl;
    }
  }
  void blif_subckt_print(std::ostream &f) {
    string no_param_name;
    // reducing a correctly named parametrized module MyModule(par1=99) to
    // MyModule discuss with thierry !
    for (auto k : mod_name_)
      if ('(' == k)
        break;
      else
        no_param_name.push_back(k);
    f << ".subckt " << no_param_name;
    for (auto &p : conns_) {
      string s = (p.second == "GND_NET") ? "$false" : p.second;
      s = (s == "VCC_NET") ? "$true" : s;
      s = (s == "n3") ? "$undef" : s;
      f << " " << p.first << "=" << s;
    }
    f << std::endl;
    for (auto &p : params_) {
      // TO_DO verify that verific gives always binary otherwise hande other
      // formats
      std::string s(p.second);
      std::size_t found = s.find('b');
      if (found == std::string::npos)
        found = s.find('B');
      if (found != std::string::npos) {
        for (int i = found; i >= 0; i--)
          s[i] = ' ';
      }
      while (s.size() && (s.back() == '"' || isspace(s.back())))
        s.pop_back();
      f << ".param " << p.first << " " << s << std::endl;
    }
  }
  void blif_print(std::ostream &f) {
    if ("$lut" == mod_name_.substr(0, 4)) {
      blif_names_print(f);
    } else {
      blif_subckt_print(f);
    }
  }
};

struct simple_netlist_ed {
  void p_print_ed(std::ostream &f) {
    f << "Netlist Name ----: " << name << std::endl;
    f << "Netlist in ports ----: " << std::endl;
    p_print_ed(in_ports, f);
    f << "Netlist out ports ----: " << std::endl;
    p_print_ed(out_ports, f);
    f << "Netlist nets ----: " << std::endl;
    p_print_ed(nets, f);
    f << "Netlist blocks ----: " << std::endl;
    p_print_ed(blocks, f);
  }
  void b_port_print_json(std::ostream &f)
    {
        bool first = true;
        f << "[\n  {\n\t\"ports\": [";
        for (auto &in : ports)
        {
            if ('\\' == in[0])
                in[0] = ' ';
            if (!first)
                f << ",";
            f << "\n\t  {";
            string s = "output";
            f << "\n\t\t\"direction\": \"" << ((in_set.find(in)==end(in_set)) ? "output" : "input") << "\",";
            f << "\n\t\t\"name\": \"" << in << "\",";
            f << "\n\t\t\"type\": \"WIRE\"";
            f << "\n\t  }" ;
            first = false;
        }
        f << "\n\t]," << endl;
        f << "\t\"topModule\": \"" << name << "\"" << endl;
        f << "  }\n]" << endl;
    }

    void b_port_print_separate_json(std::ostream &f)
    {
        bool first = true;
        f << "{\n\t\"inputs\": [";
        for (auto &in : in_ports)
        {
            if ('\\' == in[0])
                in[0] = ' ';
            if (!first)
                f << ",";
            f << "\n\t\t\"" << in << "\"";
            first = false;
        }
        f << "\n\t]," << endl;
        first = true;
        f << "\n\t\"outputs\": [";
        for (auto &in : out_ports)
        {
            if ('\\' == in[0])
                in[0] = ' ';
            if (!first)
                f << ",";
            f << "\n\t\t\"" << in << "\"";
            first = false;
        }
        f << "\n\t]\n}" << endl;
    }

  void b_print_ed(std::ostream &f) {
    f << ".model " << name << std::endl;
    f << ".inputs";
    for (auto &in : in_ports) {
      if ('\\' == in[0])
        in[0] = ' ';
      f << " " << in;
    }
    f << endl;
    f << ".outputs";
    for (auto &in : out_ports) {
      if ('\\' == in[0])
        in[0] = ' ';
      f << " " << in;
    }
    f << endl;
    f << ".names $false" << endl;
    f << ".names $true\n1" << endl;
    f << ".names $undef" << endl;
    b_print_ed(blocks, f);
    f << ".end" << endl;
  }
  void b_print_ed(std::vector<inst_ed> v, std::ostream &f) {
    for (auto &el : v) {
      el.blif_print(f);
    }
  }
  void p_print_ed(std::vector<std::string> v, std::ostream &f) {
    for (auto &el : v) {
      f << "\t" << el;
      f << std::endl;
    }
    f << std::endl;
  }
  void p_print_ed(std::vector<inst_ed> v, std::ostream &f) {
    for (auto &el : v) {
      f << "\t";
      el.p_print_ed(f);
      f << std::endl;
    }
    f << std::endl;
  }
  std::string name;
   std::vector<std::string> ports;
  std::vector<std::string> in_ports;
  std::vector<std::string> out_ports;
  std::vector<std::string> inout_ports;
  std::vector<std::string> nets;
  std::vector<inst_ed> blocks;
  std::unordered_set<std::string> in_set;
};

void get_truth_table(std::string tt_output_str, int width, bool is_hex,
                     std::vector<std::vector<unsigned>> &tt_vector) {

  std::vector<std::vector<int>> rows;
  // std::vector<std::vector<unsigned> > tt_vector;
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

void ports_addition(std::vector<std::string> &ports, std::string port_name,
                    int port_size) {

  if ((port_size == 0) || (port_size == 1)) {
    ports.push_back(port_name);
  } else {
    for (auto s = 0; s < port_size; s++) {
      ports.push_back(port_name + "[" + to_string(s) + "]");
    }
  }
}

void seperate_ports(std::vector<std::tuple<
                        /*Port name original  */ std::string,
                        /*Port name renamed   */ std::string,
                        /*Port direction      */ std::string,
                        /*Port size           */ std::string>> &ports_vector,
                    std::vector<std::string> &in_ports,
                    std::vector<std::string> &out_ports,
                    std::vector<std::string> &inout_ports, bool is_lut,
                    std::string width) {

  for (unsigned int i = 0; i < ports_vector.size(); i++) {
    int size;
    if (string_compare(
            std::get<3>(ports_vector[i]),
            "")) // if the input port size is not defined set it to zero
      size = 0;
    else
      size = stoi(
          std::get<3>(ports_vector[i])); // else get the size from the value

    if (string_compare("INPUT", get<2>(ports_vector[i]))) {
      if (is_lut) {
        std::string empty;
        if (string_compare(width, empty)) {
          size = 0;
        } else {
          size = stoi(width);
        }
      }

      ports_addition(in_ports, std::get<0>(ports_vector[i]), size);
    } else if (string_compare("OUTPUT", get<2>(ports_vector[i]))) {
      ports_addition(out_ports, std::get<0>(ports_vector[i]), size);
    } else if (string_compare("INOUT", get<2>(ports_vector[i]))) {
      ports_addition(inout_ports, std::get<0>(ports_vector[i]), size);
    }
  }
}

std::string find_corresp_net(
    std::unordered_map<std::string, std::vector<std::tuple<

                                        /*net_name           */ std::string,
                                        /* net instance ref  */ std::string,
                                        /* net name  renamed  */ std::string>>>
        &net_reduced_map,
    std::string ins_ref, std::string port_name) {
  std::pair<std::string, std::string> port_ref_pair =
      std::make_pair(port_name, ins_ref);
  std::pair<std::string, std::string> port_init_pair;
  std::vector<std::tuple<

      /*net_name           */ std::string,
      /* net instance ref  */ std::string,
      /* net name  renamed  */ std::string>>::iterator t;
  for (auto it = net_reduced_map.begin(); it != net_reduced_map.end(); it++) {
    for (long unsigned int i = 0; i < it->second.size(); i++) {
      port_init_pair = std::make_pair(std::get<0>(it->second[i]),
                                      std::get<1>(it->second[i]));
      if (port_init_pair == port_ref_pair) {
        t = (it->second.begin() + i);
        // port_init_pair= *t;
        // std::cout << "The deleted pair is "<< port_init_pair.first<< " "
        // <<port_init_pair.second<<std::endl;
        it->second.erase(t);

        return it->first;
      }
    }
  }
  return "";
}

void edif_blif(const char *InputFile, FILE *edif_bl) {

  std::vector<std::tuple<
      /*net_name           */ std::string,
      /* net instance ref  */ std::string,
      /* net name renamed  */ std::string>>
      net_reduced_vector;
  std::unordered_map<std::string, std::vector<std::tuple<

                                      /*net_name           */ std::string,
                                      /* net instance ref  */ std::string,
                                      /* net name renamed  */ std::string>>>
      net_reduced_map;
  // pprint p1;
  cells_sep cell1_;

  FILE *fp;
  fp = fopen(InputFile, "r");
  if (fp == NULL) {
    std::cout << "Unable to read the input file: " << InputFile << std::endl;
    return;
  }
  // Read the file into a tree
  struct SNode *node = snode_parse(fp);
  cell1_.iterate(node);
  // Close the file stream
  fclose(fp);
  simple_netlist_ed sn;

  std::stringstream ss;
  sn.name = cell1_.top_module;

  for (auto itv = cell1_.cells_map.begin(); itv != cell1_.cells_map.end();
       itv++) {
    std::string cell_name = itv->first;
    cells_sep::cells cell_curr = itv->second;
    // Initially create a net vector in which only the port name and its
    // corresponding net is present.
    for (auto it = cell_curr.net_map.begin(); it != cell_curr.net_map.end();
         it++) {

      for (long unsigned int i = 0; i < it->second.size(); i++) {

        std::string port_name = std::get<0>(it->second[i]);

        if (!string_compare(std::get<1>(it->second[i]),
                            "")) // condition check if member exist
        {
          if (string_compare(
                  std::get<2>(it->second[i]),
                  "")) // condition check if the port is from top module
          {
            for (long unsigned int itp = 0; itp < cell_curr.ports_vector.size();
                 itp++) {
              if (string_compare(
                      std::get<1>(cell_curr.ports_vector[itp]),
                      std::get<0>(it->second[i]))) // comparing the port names
              {
                int port_size =
                    stoi(std::get<3>(cell_curr.ports_vector[itp])) - 1;
                int pin_number = stoi(std::get<1>(it->second[i]));
                pin_number = port_size - pin_number;
                port_name = port_name + "[" + std::to_string(pin_number) + "]";
              }
            }
          } else {
            port_name = port_name + "[" + std::get<1>(it->second[i]) + "]";
          }
        }
        net_reduced_vector.push_back(std::make_tuple(
            port_name, std::get<2>(it->second[i]), std::get<3>(it->second[i])));
      }
      net_reduced_map.insert({it->first, net_reduced_vector});
      net_reduced_vector.clear();
    }

    // find the top cell which will be build for the blif
    if (string_compare(cell_curr.cell_name_orig, cell1_.top_module)) {
      // get the cell ios and place it in the simple netlist vector
      seperate_ports(cell_curr.ports_vector, sn.in_ports, sn.out_ports,
                     sn.inout_ports, false, "");

      for (unsigned int iti = 0; iti < cell_curr.instance_vector.size();
           iti++) {
        inst_ed ins_;
        int input_port_size;
        ins_.name_ = std::get<1>(
            cell_curr.instance_vector[iti]); // write the instance name
        ins_.mod_name_ = std::get<2>(
            cell_curr.instance_vector[iti]); // write the instance model name
        if ((string_compare(std::get<2>(cell_curr.instance_vector[iti]),
                            "GND")) ||
            (string_compare(std::get<2>(cell_curr.instance_vector[iti]),
                            "VCC")))
          continue;

        auto itvv =
            cell1_.cells_map.find(std::get<2>(cell_curr.instance_vector[iti]));
        if (itvv != cell1_.cells_map.end()) {
          std::string inst_name = itvv->first;
          cells_sep::cells cell_inst = itvv->second;
          std::vector<std::string> in_ports;
          std::vector<std::string> out_ports;
          std::vector<std::string> inout_ports;

          if (string_compare(cell_inst.cell_name_renamed,
                             std::get<2>(cell_curr.instance_vector[iti]))) {
            // Get all the ports of that instance cell
            seperate_ports(cell_inst.ports_vector, in_ports, out_ports,
                           inout_ports,
                           std::get<5>(cell_curr.instance_vector[iti]),
                           std::get<4>(cell_curr.instance_vector[iti]));
            int inpt = 0;

            // Getting the net connections from the ports
            // First entering the inputs
            input_port_size = in_ports.size();
            for (inpt = in_ports.size() - 1; inpt >= 0; inpt--) {
              std::string result_net =
                  find_corresp_net(net_reduced_map, ins_.name_, in_ports[inpt]);
              if (!string_compare(result_net, ""))
                ins_.conns_.push_back(
                    std::make_pair(in_ports[inpt], result_net));
            }
            long unsigned int inpt1 = 0;
            // First putting the output
            for (inpt1 = 0; inpt1 < out_ports.size(); inpt1++) {
              std::string result_net = find_corresp_net(
                  net_reduced_map, ins_.name_, out_ports[inpt1]);
              if (!string_compare(result_net, ""))
                ins_.conns_.push_back(
                    std::make_pair(out_ports[inpt1], result_net));
            }
            for (inpt1 = 0; inpt1 < inout_ports.size(); inpt1++) {
              std::string result_net = find_corresp_net(
                  net_reduced_map, ins_.name_, inout_ports[inpt1]);
              if (!string_compare(result_net, ""))
                ins_.conns_.push_back(
                    std::make_pair(inout_ports[inpt1], result_net));
            }
          }

          if (std::get<5>(cell_curr.instance_vector[iti])) {
            ins_.mod_name_ = "$lut";
            bool is_hex = false;
            const long unsigned int pos =
                std::get<3>(cell_curr.instance_vector[iti]).find("h");
            if (pos != string::npos) {
              is_hex = true;
            }
            int width;

            if (string_compare(std::get<4>(cell_curr.instance_vector[iti]),
                               "")) {
              width = input_port_size;
            } else {

              width = stoi(std::get<4>(cell_curr.instance_vector[iti]));
            }

            get_truth_table(std::get<3>(cell_curr.instance_vector[iti]), width,
                            is_hex, ins_.truthTable_);

            sn.blocks.push_back(ins_);
          } else {
            sn.blocks.push_back(ins_);
          }
        }
      }

      // The remaining nets are connected with the ports or ground or vcc so
      // adding
      for (auto it = net_reduced_map.begin(); it != net_reduced_map.end();
           it++) {

        for (long unsigned int i = 0; i < it->second.size(); i++) {

          // No need to connect the string port name with the string name and
          // name might be same but it should be of different instances
          if (((it->first != std::get<0>(it->second[i])) &&
               (std::get<0>(it->second[i]) != std::get<2>(it->second[i]))) &&
              (string_compare(std::get<1>(it->second[i]), ""))) {
            inst_ed ins_;
            ins_.mod_name_ = "$lut";
            //
            ins_.conns_.push_back(std::make_pair(it->first, it->first));
            // check if the port name is from the io's. if it is renamed then
            // use the original name
            if (string_compare(std::get<1>(it->second[i]), "")) {
              // This confirms that the port is of top module
              // if the port is from top module it can be renamed so get the
              // original name
              for (long unsigned int itp = 0;
                   itp < cell_curr.ports_vector.size(); itp++) {
                // posibilities are that in the port list is array which can be
                // renamed
                //std::cout << "comparing " << std::get<0>(it->second[i])
                //          << " with "
                //          << std::get<1>(cell_curr.ports_vector[itp])
                //          << std::endl;
                
                // first we will check if the member exist then it will compare
                // it will create the ports bit by bit if member exists
                if (!string_compare("",
                                    std::get<3>(cell_curr.ports_vector[itp]))) {
                // member exists
                  int number = stoi(std::get<3>(cell_curr.ports_vector[itp]));
                 // std::cout << " port size is " << number << std::endl;
                  for (int k = 0; k <= number; k++) {
                    // now ports with their number will be created
                    // renamed port name is used as the array can be renamed or the port name may also be renamed.
                    int port_orig_num = number-k;
                    std::string port_name =
                        std::get<1>(cell_curr.ports_vector[itp]) + "[" +
                        to_string(port_orig_num) + "]";
                    //std::cout<< "comparing "<< std::get<0>(it->second[i]) << " with " << port_name << std::endl;
                    if (string_compare(std::get<0>(it->second[i]), port_name)) {
                        std::string port_name_orig =
                        std::get<0>(cell_curr.ports_vector[itp]) + "[" +
                        to_string(port_orig_num) + "]";

                      ins_.conns_.push_back(
                          std::make_pair(port_name_orig, port_name_orig));
                        break;
                    }
                  }
                }// if the member does not exist then it will be added directly 
                else if (string_compare(
                               std::get<0>(it->second[i]),
                               std::get<1>(cell_curr.ports_vector[itp]))) {

                  ins_.conns_.push_back(
                      std::make_pair(std::get<0>(cell_curr.ports_vector[itp]),
                                     std::get<0>(cell_curr.ports_vector[itp])));
                }
              }
            }// if it is not from the io of the top module then it will be added directly 
            else {
              ins_.conns_.push_back(std::make_pair(std::get<0>(it->second[i]),
                                                   std::get<0>(it->second[i])));
            }
    
            get_truth_table("2", 1, false, ins_.truthTable_);
            sn.blocks.push_back(ins_);
          }
        }
      }
    }
  }



  sn.b_print_ed(ss);
  fputs(ss.str().c_str(), edif_bl);
  // section for dumping the ports in the 
  long unsigned int i;
  for ( i=0; i<sn.in_ports.size();i++)
  {
    sn.ports.push_back(sn.in_ports.at(i));
     sn.in_set.insert(sn.in_ports.at(i));

  }
  for ( i=0; i<sn.out_ports.size();i++)
  {
    sn.ports.push_back(sn.out_ports.at(i));
     //sn.in_set.insert(sn.in_ports.at(i));

  }

  string js_port_file(InputFile);
    while ('.' != js_port_file.back())
    {
        js_port_file.pop_back();
    }
    js_port_file.pop_back();
    js_port_file += "_ports.json";
    ofstream myfile;
    myfile.open(js_port_file.c_str());
    sn.b_port_print_json(myfile);
    myfile.close();

  snode_free(node);
}
