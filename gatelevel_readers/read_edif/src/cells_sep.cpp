#include "cells_sep.hpp"

// Get the port data from the
// every time the keyword port is occurs in a cell this function is called and the available data is extracted from this node

void cells_sep::ports(struct SNode *head)
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
                if (child->type == LIST)
                {
                    struct SNode *n_child = child->list;
                    if (string_compare("rename", n_child->value))
                    {
                        n_child = n_child->next;
                        port_name_renamed = n_child->value;
                        n_child = n_child->next;
                        port_name_orig = n_child->value;
                    }
                }
                else
                {
                    port_name_orig = child->value;
                    port_name_renamed = child->value;
                }
                child = child->next;
                port_size = child->value;
            }
        }
        current = current->next;
    }
    ports_tuple = std::make_tuple(port_name_orig, port_name_renamed, port_direction, port_size);
}

// Get the data from the instance. every time the data
void cells_sep::cells_sep::instances(struct SNode *head)
{
    bool is_lut = false;
    struct SNode *current = head;
    // current = current->next;
    // std::string inst_name_orig, inst_name_renamed, inst_cell_ref, prop_lut, prop_width;
    std::string inst_name_orig = "";
    std::string inst_name_renamed = "";
    std::string inst_cell_ref = "";
    std::string prop_lut = "";
    std::string prop_width = "";
    while (current != NULL)
    {
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
            else if ((string_compare("viewRef", child->value)) || (string_compare("viewref", child->value)))
            {
                child = child->next;
                child = child->next;
                if (child->type == LIST)
                {
                    struct SNode *n_child = child->list;
                    if ((string_compare("cellRef", n_child->value)) || (string_compare("cellref", n_child->value)))
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
                else if (string_compare("WIDTH", child->value))
                {
                    child = child->next;
                    if (child->type == LIST)
                    {
                        struct SNode *n_child = child->list;
                        n_child = n_child->next;
                        prop_width = n_child->value;
                    }
                }
                else if (string_compare("INIT", child->value))
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

void cells_sep::cells_sep::get_nets(struct SNode *head, std::string net_name, std::string net_name_remaned) //, std::string net_name
{
    struct SNode *current = head; // list for the joined
    std::string net_port_ref = "";
    std::string net_member = "";
    std::string net_instance_ref = "";

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
                net_vector.push_back(std::make_tuple(net_port_ref, net_member, net_instance_ref,net_name_remaned ));
                net_port_ref = net_member = net_instance_ref = "";
            }
        }
        current = current->next;
    }
    net_map.insert({net_name, net_vector});
    net_vector.clear();
}

void cells_sep::cells_sep::nets(struct SNode *head, std::string net_name)
{
    std::string n_net_name = net_name;
    std::string n_net_name_renamed = net_name;
    struct SNode *current = head;
    while (current != NULL)
    {
        if (current->type == LIST)
        {
            struct SNode *child = current->list;

            if (string_compare("rename", child->value))
            {
                child = child->next;
                    n_net_name_renamed = child->value;
                child = child->next;

                n_net_name = child->value;
            }
            if (string_compare("joined", child->value))
            {

                get_nets(child, n_net_name, n_net_name_renamed);
            }
        }

        current = current->next;
    }
}

/*
 * The get cell data seperate the nodes of ports, instances and the nets for each cell
 *
 */
void cells_sep::cells_sep::get_cell_data(struct SNode *head, std::string cell_name, struct cells *cell_)
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
                cell_->cell_name_renamed = current->value;
                current = current->next;
                cell_->cell_name_orig = current->value;
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
                    // get all the ports
                    ports(current);
                    ports_tuple = std::make_tuple(current->value, current->value, std::get<2>(ports_tuple), std::get<3>(ports_tuple));
                }
                cell_->ports_vector.push_back(ports_tuple);
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
                    instances(current);
                    instance_tuple = std::make_tuple(current->value, current->value, std::get<2>(instance_tuple), std::get<3>(instance_tuple),
                                                     std::get<4>(instance_tuple), std::get<5>(instance_tuple));
                }
                cell_->instance_vector.push_back(instance_tuple);
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
                    std::string net_name = current->value;
                    nets(current, net_name);
                }
                cell_->net_map = net_map;
                // net_map.clear();
            }
        }
        current = current->next;
    }
}

void cells_sep::cells_sep::iterate(struct SNode *head)
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
                current = current->next;
                if (current->type == LIST)
                    get_cell_data(current, "", &cells_);
                else
                {
                    cells_.cell_name_orig = current->value;
                    cells_.cell_name_renamed = current->value;
                    get_cell_data(current, current->value, &cells_);
                }
                cells_vector.push_back(cells_);
            }
            if (string_compare("edif", current->value))
            {
                current = current->next;
                top_module = current->value;
            }
        }
        current = current->next;
    }
}
