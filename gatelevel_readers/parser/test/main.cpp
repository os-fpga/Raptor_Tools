#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <tuple>
#include <map>

#include "../src/simple_netlist.h"
#include "../src/sexpr.hpp"
std::tuple<
    /*Port name original  */ std::string,
    /*Port name renamed   */ std::string,
    /*Port direction      */ std::string,
    /*Port size           */ std::string>
    ports_tuple;

std::vector<std::tuple<
    /*Port name original  */ std::string,
    /*Port name renamed   */ std::string,
    /*Port direction      */ std::string,
    /*Port size           */ std::string>>
    ports_vector;

// inst_name_orig, inst_name_renamed, inst_cell_ref, prop_lut, prop_width
std::tuple<
    /*instance name original  */ std::string,
    /*Instance name renamed   */ std::string,
    /*Instance cell reference  */ std::string,
    /*Instance property lut    */ std::string,
    /*Instance property width   */ std::string,
    /* check is lut */ bool>
    instance_tuple;

std::vector<std::tuple<
    /*instance name original  */ std::string,
    /*Instance name renamed   */ std::string,
    /*Instance cell reference  */ std::string,
    /*Instance property lut    */ std::string,
    /*Instance property width   */ std::string,
    /* check is lut */ bool>>
    instance_vector;

std::tuple<
    //*net name original  */ std::string,
    /*net port reference  */ std::string,
    /*net pin number   */ std::string,
    /* net instance ref  */ std::string>
    net_tuple;

std::vector<std::tuple<
    //*net name original  */ std::string,
    /*net port reference  */ std::string,
    /*net pin number   */ std::string,
    /* net instance ref  */ std::string>>
    net_vector;

std::map<std::string, std::vector<std::tuple<
                          //*net name original  */ std::string,
                          /*net port reference  */ std::string,
                          /*net pin number   */ std::string,
                          /* net instance ref  */ std::string>>>
    net_map;

struct cells
{
  std::string cell_name_orig;
  std::string cell_name_renamed;
  std::vector<std::tuple<
      /*instance name original  */ std::string,
      /*Instance name renamed   */ std::string,
      /*Instance cell reference  */ std::string,
      /*Instance property lut    */ std::string,
      /*Instance property width   */ std::string,
      /* check is lut */ bool>>
      instance_vector;
  std::vector<std::tuple<
      /*Port name original  */ std::string,
      /*Port name renamed   */ std::string,
      /*Port direction      */ std::string,
      /*Port size           */ std::string>>
      ports_vector;
  std::map<std::string, std::vector<std::tuple<
                            //*net name original  */ std::string,
                            /*net port reference  */ std::string,
                            /*net pin number   */ std::string,
                            /* net instance ref  */ std::string>>>
      net_map;
};
std::vector<struct cells> cells_vector;
std::string top_module;
// print the link list
/*
 * This function is to compare the two strings either they are equal or not.
 * First it will check the size of the two strings and then it will compare it
 * char by char
 */

bool string_compare(const std::string &f_name, const std::string &ext)
{
  if (ext.size() > f_name.size())
    return false;
  auto ext_it = ext.rbegin();
  auto f_it = f_name.rbegin();
  while (ext_it != ext.rend() && f_it != f_name.rend())
  {
    if (*ext_it != *f_it)
      return false;
    ++ext_it;
    ++f_it;
  }
  return true;
}

void print_linklist(struct SNode *head)
{
  struct SNode *current = head;
  while (current != NULL)
  {

    // printf("\nThe string is  %s", (current->value).c_str());
    if (current->type == LIST)
    {
      struct SNode *child = current->list;
      // printf("\ngoing into the child node\n");
      print_linklist(child);
    }
    if (current->type == STRING)
    {
      printf("\nThe string is  %s", current->value);
    }
    if (current->type == SYMBOL)
    {
      printf("\nThe symbol is  %s", current->value);
    }
    if (current->type == INTEGER)
    {
      printf("\nThe integer is  %s", current->value);
    }
    if (current->type == FLOAT)
    {
      printf("\nThe float is  %s", current->value);
    }

    current = current->next;
  }
}
// Get the port data from the
// every time the keyword port is occurs in a cell this function is called and the available data is extracted from this node

void ports(struct SNode *head)
{
  struct SNode *current = head;
  std::string port_name_orig, port_name_renamed, port_direction, port_size;
  // iternate the port node to get the data
  while (current != NULL)
  {
    if (current->type == LIST)
    {
      struct SNode *child = current->list;

      if (string_compare("rename", child->value))
      {
        child = child->next;
        port_name_renamed = child->value;
        child = child->next;
        port_name_orig = child->value;
      }
      else if (string_compare("direction", child->value))
      {
        child = child->next;
        port_direction = child->value;
      }
      else if (string_compare("array", child->value))
      {
        child = child->next;
        port_name_orig = child->value;
        child = child->next;
        port_size = child->value;
      }
    }
    current = current->next;
  }
  ports_tuple = std::make_tuple(port_name_orig, port_name_renamed, port_direction, port_size);
}

// Get the data from the instance. every time the data
void instances(struct SNode *head)
{
  bool is_lut = false;
  struct SNode *current = head;
  // current = current->next;
  std::string inst_name_orig, inst_name_renamed, inst_cell_ref, prop_lut, prop_width;

  while (current != NULL)
  {

    // printf("\nThe string is  %s", (current->value).c_str());
    if (current->type == LIST)
    {
      struct SNode *child = current->list;

      if (string_compare("rename", child->value))
      {
        child = child->next;
        inst_name_renamed = child->value;
        child = child->next;
        inst_name_orig = child->value;
      }
      else if (string_compare("viewRef", child->value))
      {
        child = child->next;
        child = child->next;
        if (child->type == LIST)
        {
          struct SNode *n_child = child->list;
          if (string_compare("cellRef", n_child->value))
            n_child = n_child->next;
          inst_cell_ref = n_child->value;
        }
      }
      // The property is of three types which are
      // property lut
      // propperty width
      // property init
      else if (string_compare("property", child->value))
      {
        child = child->next;
        if (string_compare("LUT", child->value))
        {
          child = child->next;
          if (child->type == LIST)
          {
            struct SNode *n_child = child->list;
            n_child = n_child->next;
            prop_lut = n_child->value;
            is_lut = true;
          }
        }
        if (string_compare("WIDTH", child->value))
        {
          child = child->next;
          if (child->type == LIST)
          {
            struct SNode *n_child = child->list;
            n_child = n_child->next;
            prop_width = n_child->value;
          }
        }
        if (string_compare("INIT", child->value))
        {
          child = child->next;
          if (child->type == LIST)
          {
            struct SNode *n_child = child->list;
            n_child = n_child->next;
            prop_lut = n_child->value;
            is_lut = true;
          }
        }
      }
    }
    current = current->next;
  }
  instance_tuple = std::make_tuple(inst_name_orig, inst_name_renamed, inst_cell_ref, prop_lut, prop_width, is_lut);
}

// One net can connect to multiple pins of lut or sub circuits so a seperate mechanism is developed to get the net connection

void get_nets(struct SNode *head, std::string net_name) //, std::string net_name
{
  struct SNode *current = head; // list for the joined
                                // std::cout << "\nThe portref is " << net_name << std::endl;
  std::string net_port_ref, net_member, net_instance_ref;

  while (current != NULL) // iterate untill the joined does not end
  {
    if (current->type == LIST)
    {
      struct SNode *child = current->list; // reach the net connection list

      if ((string_compare("portRef", child->value)) || (string_compare("portref", child->value)))
      {
        while (child != NULL) // iterate in the net connection list aka (port ref)
        {
          if (child->type == LIST)
          {
            struct SNode *n_child = child->list;
            if (string_compare("member", n_child->value))
            {
              n_child = n_child->next;
              net_port_ref = n_child->value;
              n_child = n_child->next;
              net_member = n_child->value;
            }
            if ((string_compare("instanceRef", n_child->value)) || (string_compare("instanceref", n_child->value)))
            {
              n_child = n_child->next;
              net_instance_ref = n_child->value;
            }
          }
          else
          {
            net_port_ref = child->value;
          }
          child = child->next;
        }
        net_vector.push_back(std::make_tuple(net_port_ref, net_member, net_instance_ref));
        net_port_ref = net_member = net_instance_ref = "";
      }
    }
    current = current->next;
  }
  net_map.insert({net_name, net_vector});
  net_vector.clear();
}

void nets(struct SNode *head, std::string net_name)
{
  std::string n_net_name = net_name;
  struct SNode *current = head;
  while (current != NULL)
  {
    if (current->type == LIST)
    {
      struct SNode *child = current->list;
      //  if (current->type == SYMBOL)
      //{
      std::cout << "The next value is  " << child->value << std::endl;
      if (string_compare("rename", child->value))
      {
        child = child->next;

        child = child->next;

        n_net_name = child->value;
        printf("\nThe net name  is %s\n", n_net_name.c_str());
        // current = current->next;
      }
      if (string_compare("joined", child->value))
      {
        printf("\nThe joined is \n");

        get_nets(child, n_net_name);
      }
    }

    current = current->next;
  }
}

/*
 * The get cell data seperate the nodes of ports, instances and the nets for each cell
 *
 */
void get_cell_data(struct SNode *head, std::string cell_name, struct cells *cell_)
{
  struct SNode *current = head;
  while (current != NULL)
  {
    /*initially the code will recurse for each node in the cell*/
    if (current->type == LIST)
    {
      struct SNode *child = current->list;
      get_cell_data(child, cell_name, cell_);
    }
    /*IF it is a symbol then check for port, instance or net*/
    if (current->type == SYMBOL)
    {
      if (string_compare("rename", current->value))
      /*The port can be either renamed or passed directly we have to get both the values to generate the blif*/
      {
        current = current->next;
        std::cout << "The cell name renamed is  " << current->value << std::endl;
        cell_->cell_name_renamed = current->value;
        ;
        current = current->next;
        std::cout << "The cell name original is   " << current->value << std::endl;
      }
      if (string_compare("port", current->value))
      /*The port can be either renamed or passed directly we have to get both the values to generate the blif*/
      {
        current = current->next;
        if (current->type == LIST)
        {
          // get all the ports
          ports(current);
          // ports_tuple = std::make_tuple(port_name_orig, port_name_renamed, port_direction, port_size);
        }
        else
        {
          // printf("\nThe port name is  %s\n", current->value);
          // get all the ports

          ports(current);
          ports_tuple = std::make_tuple(current->value, current->value, std::get<2>(ports_tuple), std::get<3>(ports_tuple));
        }
        cell_->ports_vector.push_back(ports_tuple);
      }
      for (long unsigned int i = 0; i < ports_vector.size(); i++)
      {
        std::cout << "The port name original is " << std::get<0>(ports_vector[i]) << std::endl;
        std::cout << "The port name renamed is " << std::get<1>(ports_vector[i]) << std::endl;
        std::cout << "The port direction is " << std::get<2>(ports_vector[i]) << std::endl;
        std::cout << "The port size is " << std::get<3>(ports_vector[i]) << std::endl;
      }
      ports_vector.clear();
      if (string_compare("instance", current->value))
      {
        current = current->next;
        if (current->type == LIST)
        {
          instances(current);
        }
        else
        {
          // printf("\nThe instance name is  %s\n", current->value);
          instances(current);
          instance_tuple = std::make_tuple(current->value, current->value, std::get<2>(instance_tuple), std::get<3>(instance_tuple),
                                           std::get<4>(instance_tuple), std::get<5>(instance_tuple));
        }
        cell_->instance_vector.push_back(instance_tuple);
        for (long unsigned int i = 0; i < instance_vector.size(); i++)
        {
          std::cout << "The instance name original is " << std::get<0>(instance_vector[i]) << std::endl;
          std::cout << "The instance name renamed is " << std::get<1>(instance_vector[i]) << std::endl;
          std::cout << "The instance cell is " << std::get<2>(instance_vector[i]) << std::endl;
          std::cout << "The instance property width is " << std::get<3>(instance_vector[i]) << std::endl;
        }
        instance_vector.clear();
      }
      if (string_compare("net", current->value))
      {

        current = current->next;

        if (current->type == LIST)
        {

          nets(current, "net");
        }
        else
        {
          printf("\nThe net name is  %s\n", current->value);
          std::string net_name = current->value;
          nets(current, net_name);
        }
        cell_->net_map = net_map;
        net_map.clear();
      }
    }
    current = current->next;
  }
}

void iterate(struct SNode *head)
{
  struct SNode *current = head;
  while (current != NULL)
  {
    if (current->type == LIST)
    {
      struct SNode *child = current->list;
      iterate(child);
    }
    if (current->type == SYMBOL)
    {
      if (string_compare("cell", current->value))
      {
        struct cells cells_;
        printf("\nA cell struct has been created  \n");
        current = current->next;
        if (current->type == LIST)
          get_cell_data(current, "", &cells_);
        else
        {
          printf("\nThe cell name is  %s\n", current->value);
          cells_.cell_name_orig = current->value;
          get_cell_data(current, current->value, &cells_);
        }
        cells_vector.push_back(cells_);
      }
      if (string_compare("edif", current->value))
      {
        current = current->next;
        top_module = current->value;
        printf("\nA top cell name is %s  \n", current->value);
      }
    }
    current = current->next;
  }
}
/*
 *

std::string find_top_(struct SNode *head )
{

  std::string top;
  struct SNode *current = head;
  while (current != NULL)
  {

    // printf("\nThe string is  %s", (current->value).c_str());
    if (current->type == LIST)
    {
      struct SNode *child = current->list;
      // printf("\ngoing into the child node\n");
      find_top_(child);
    }
    if (current->type == STRING)
    {
      printf("\nThe string is  %s", current->value);
    }
    if (current->type == SYMBOL)

    {

      // std::string input_string = current->value;
      if (string_compare("edif", current->value))
      {
        current = current->next;
        top = current->value;
        break;
      }

      printf("\nThe symbol is  %s", current->value);
    }
    if (current->type == INTEGER)
    {
      printf("\nThe integer is  %s", current->value);
    }
    if (current->type == FLOAT)
    {
      printf("\nThe float is  %s", current->value);
    }

    current = current->next;
  }
  return top;
}*/

// creating a class to seperate the required field
class data_expt
{
public:
  // constructor
  data_expt(struct SNode *node)
  {
    struct SNode *head = node;
    std::cout << "Entering into the class " << std::endl;
    // print_linklist(head);
    iterate(head);
  }
  // Destructor
  ~data_expt() {}
};
/*
void seperate_ports(std::string top_name)
{
  auto it = cell_ports.find(top_name);
  ports = it->second;
  for (unsigned int i = 0; i < ports.size(); i++)
  {
    int size = stoi(std::get<1>(ports[i]));
    if (string_compare("INPUT", get<2>(ports[i])))
    {
      std::string port_name = std::get<3>(ports[i]);
      if (size == 0)
      {
        in_ports.push_back(std::get<3>(ports[i]));
      }
      else
      {
        for (auto s = 0; s < size; s++)
        {
          in_ports.push_back((std::get<3>(ports[i])) + "[" + to_string(s) +
                             "]");
        }
      }
    }
    else if (string_compare("OUTPUT", get<2>(ports[i])))
    {
      if (size == 0)
      {
        out_ports.push_back(std::get<3>(ports[i]));
        map_output.insert({std::get<0>(ports[i]), std::get<3>(ports[i])});
      }
      else
      {
        for (auto s = 0; s < size; s++)
        {
          out_ports.push_back((std::get<3>(ports[i])) + "[" + to_string(s) +
                              "]");
          map_output.insert(
              {((std::get<0>(ports[i])) + "[" + to_string(s) + "]"),
               ((std::get<3>(ports[i])) + "[" + to_string(s) + "]")});
        }
      }
    }
    else if (string_compare("INOUT", get<2>(ports[i])))
    {
      if (size == 0)
      {
        out_ports.push_back(std::get<3>(ports[i]));
      }
      else
      {
        for (auto s = 0; s < size; s++)
        {
          inout_ports.push_back((std::get<3>(ports[i])) + "[" + to_string(s) +
                                "]");
        }
      }
    }
  }
}
*/
int main(int argc, char *argv[])
{

  // Open the file stream
  if (argc == 3)
  {
    FILE *fp = fopen(argv[1], "r");

    // Read the file into a tree
    struct SNode *node = snode_parse(fp);
    data_expt new_data_expt(node);
    // Close the file stream
    fclose(fp);

    for (auto it = net_map.begin(); it != net_map.end(); it++)
    {
      std::cout << std::endl
                << std::endl
                << "The net name is " << it->first << std::endl;
      net_vector = it->second;
      for (long unsigned int i = 0; i < net_vector.size(); i++)
      {
        std::cout << "The net port ref is " << std::get<0>(net_vector[i]) << std::endl;
        std::cout << "The net pin number is" << std::get<1>(net_vector[i]) << std::endl;
        std::cout << "The net instance reference is " << std::get<2>(net_vector[i]) << std::endl;
      }
    }
    // Deallocate the memory used by the tree
    std::cout << "The cell vector size is : " << cells_vector.size();
    for (unsigned int it = 0; it < cells_vector.size(); it++)
    {
      struct cells cell_ = cells_vector[it];
      std::cout << std::endl
                << std::endl
                << "The cell name is  " << cell_.cell_name_orig << std::endl;
    }

    // std::string top_cell= find_top_(node);
    std::cout << "The top cell name is  " << top_module << std::endl;

    simple_netlist sn;
    inst ins_;

    sn.name = top_module;

    for (auto itv = 0; itv < cells_vector.size(); itv++)
    {
      cells cell_;
      cell_ = cells_vector[itv];
     // if (string_compare(cell_.cell_name_orig, top_module))
     // {
     // }
   // }

    snode_free(node);

    return 0;
  }
}
