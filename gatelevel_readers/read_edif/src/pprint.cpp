#include "pprint.hpp"
using namespace std;

void pprint::print_linklist(struct SNode *head)
{
    struct SNode *current = head;
    while (current != NULL)
    {

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
void pprint::ports_print(std::vector<std::tuple<std::string, std::string, std::string, std::string>> &ports_vector)
{
  for (long unsigned int i = 0; i < ports_vector.size(); i++)
  {
    std::cout << "The port name original is " << std::get<0>(ports_vector[i]) << std::endl;
    std::cout << "The port name renamed is " << std::get<1>(ports_vector[i]) << std::endl;
    std::cout << "The port direction is " << std::get<2>(ports_vector[i]) << std::endl;
    std::cout << "The port size is " << std::get<3>(ports_vector[i]) << std::endl;
  }
}

void pprint::nets_print(std::map<std::string, std::vector<std::tuple<
                                                  //*net name original  */ std::string,
                                                  /*net port reference  */ std::string,
                                                  /*net pin number   */ std::string,
                                                  /* net instance ref  */ std::string>>>
                            &net_map)
{
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
}

void pprint::instances_print(std::vector<std::tuple<
                                 /*instance name original  */ std::string,
                                 /*Instance name renamed   */ std::string,
                                 /*Instance cell reference  */ std::string,
                                 /*Instance property lut    */ std::string,
                                 /*Instance property width   */ std::string,
                                 /* check is lut */ bool>>
                                 &instance_vector)
{
  for (long unsigned int i = 0; i < instance_vector.size(); i++)
  {
    std::cout << "The instance name original is " << std::get<0>(instance_vector[i]) << std::endl;
    std::cout << "The instance name renamed is " << std::get<1>(instance_vector[i]) << std::endl;
    std::cout << "The instance cell is " << std::get<2>(instance_vector[i]) << std::endl;
    std::cout << "The instance property width is " << std::get<3>(instance_vector[i]) << std::endl;
  }
}
