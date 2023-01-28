#!/usr/bin/env tclsh

# RIC modeling system initial Tcl implementation
# Copyright (C) 2023 Rapid Silicon

puts "Raptor IP Configuration (RIC) Modeling tool"
puts "Help: ric <model_file> : Compile the model, checks the model, generate Raptor DB and SV constraints"

if {[llength $argv] == 0} {
    exit 0
}

set model  [lindex $argv 0]

puts "RIC: Processing model: $model..."

proc ric_schema_version { args } {
    # -version <version> string // the version of the Device Modeling Schema (e.g 1.0.2)
    set options {-version}
}

proc device_name  { args } {
    # -name <name>   string // Defines the Device name (e.g GEMINI)
    set options {-name}
}

proc device_version  { args } {
    # -version <version>  string // Defines the version of the Device (e.g 1.0.2)
    set options {-version}
}

proc define_enum_type { args } {
    # -name <enumerate>
    # -values { {<name>, value }, …, {<name>, value }, {"default", value } }
    set options {-name -values}
}

proc define_block  { args } {
    # -name <type>           string // The name of the currently defined attribute
    # -ports  <list_of_port_objects> [ {port} ] // Set of ports
    # -force                         Nill    // Rewrite existing block if  previously any named same
    set options {-name -ports -force }
}

proc define_attr { args } {
    # -block <block_type_name>    string // the name of a defined block to insert the attribute in.
    # -name <attr_name>           string // The name of the currently defined attribute
    # -addr <offset_address>      string // Relative address of the attribute in the block config space.
    # -width <nb_bits>            int // Number of bits enough to code all possible values of this attribute
    # -enum { {<name>, value }, …, {<name>, value }, {"default", value } }  // Inline Attribute enumerated values
    # -enum <enumerate_name>           {enum} // Offline Attribute enumerated values
    # -force                  Nill    // Rewrite existing attribute if any
    set options {-block -name -addr -width -enum -force }
}

proc define_port { args } {
    # -block <block_type_name>    string // the name of a defined block to insert the port in
    # -name <port_name>           string // The name of the currently defined port
    # -direction <"in"/"out">     string // the direction of the currently defined port "in"/"out"
    # -source <port_name/"open">  string // the port name driving this port or the string "open" if none.
    # -force                       Nill  // Rewrite existing port if any

    set options {-block -name -direction -source -force }
}

proc create_instance { args } {
    # -block <block_type_name>        string      // the name of a defined block we are instanciating
    # -name <instance_name>           string      // the currently defined instance name
    # -id <instance_id>               int         // A per design unique instance id (Should be automatic)
    # -logic_location                 {int, int}  // Logic Location (VPR)
    # -logic_address <logical_address> int        // Logic address in chain
    # -io_bank <io_bank_name> (optional)  string  // IO bank name
    # -parent <block_type>    (optional)  string  // Parent block (Creates hierarchy – like Verilog folded model)
    set options {-block -name -logic_location -logic_address -io_bank -parent }
}

proc define_constraint { args } {
    # -block <block_type_name> <sv-like-implication>  // Inner block constraint in the form of SV implication
    set options {-block }
}

proc define_constraint { args } {
    # -inst <instance_name> <sv-like-implication>     // Inner instance constraint in the form of SV implication
    set options {-inst }
}

proc define_constraint  { args } {
    # -glob <sv-like-implication with hierarchical path> // Global constraint with hierarchical path
    set options {-glob }
}

proc define_chain        { args } {
    # -type <type>                    string      // The name of the currently created chain
    set options {-type }
}

proc add_block_to_chain_type { args } {
    # -type <chain_type_name>         string      // The name of the currently created chain
    # -block <block_type_name>        string      // The name of the currently created chain
    set options {-type -block }
}

proc create_chain_instance { args } {
    # -type <chain_type_name>                     string // The name of the instanciated chain
    # -name <chain_instance_name>                 string // The name of the currently defined chain instance
    # -start_address <START_LOGICAL_ADDRESS>      int    // Logic address
    # -end_address <END_LOGICAL_ADDRESS>          int    // Logic address

    set options {-type -name -start_address -end_address }
}

proc link_chain { args } {
    # -inst <block_instance_name>          string      // The name of the currently created chain
    # -chain <chain_instance_name>          string      // The name of the currently created chain
    set options {-inst -chain }
}

proc append_instance_to_chain  { args } {
    # -chain <chain_instance_name>
    # -inst <block_instance_name>
    set options {-inst -chain }
}

proc define_net { args } {
    # -name <net_name>                string // The name of the currently defined net.
    # -source <net_name/port_name>    string  // The name of the driver of the currently defined net.
    set options {-name -source }
}

proc drive_net { args } {
    # -name <net_name>                string  // The name of the currently driven net.
    # -source <net_name/port_name>    string  // The name of the driver of the currently driven net.
    // port_name should be in the format instance_name.port_name
    set options {-name -source }
}

proc drive_port{ args } {
    # -name <port_name>               string  // The name of the currently driven net.
    # -source <net_name/port_name>    string  // The name of the driver of the currently driven net.
    # // port_name should be in the format instance_name.port_name

    set options {-name -source }
}

proc get_block_names     { args } {
    # -reg <regular_expression>       string  // The regular expression that the names match, all if empty
    set options {-reg }
}

proc get_ports { args } {
    # -reg <regular_expression>       string  // The regular expression that the names match, all if empty
    set options {-reg }
}

proc get_instance_names { args } {
    # -reg <regular_expression>       string  // The regular expression that the names match, all if empty
    set options {-reg }
}

proc get_instance_id { args } {
    # -instance <instance_name>       string  //
    set options {-instance }
}

proc get_instance_block_name { args } {
    # -instance <instance_name>       string  //
    set options {-instance }
}

proc get_instance_by_id { args } {
    # -id <instance_id>               int     //
    set options {-id }
}

proc get_instance_id_set { args } {
    # -block <block_name>             string  //
    set options {-block }
}

proc get_instance_name_set { args } {
    # -block <block_name>             string  //
    set options {-block }
}

proc get_attributes { args } {
    # -block <block_name>             string  //
    set options {-block }
}

proc get_constraints { args } {
    # -block <block_name>             string  //
    set options {}
}

proc get_attributes { args } {
    # -instance <instance_name>               string  // The name of
    set options {-instance }
}

proc get_constraints { args } {
    # -instance <instance_name>               string  // The name of
    set options {-instance }
}

proc get_port_connections { args } {
    # -instance <instance_name>               string  // The name of
    set options {-instance }
}

proc get_chain_names { args } {
    # -device <device_name>               string  // The name of
    set options {-device }
}

proc get_parent { args } {
    # -instance <instance_name>               string  // The name of
    set options {-instance }
}

proc get_port_connection_source { args } {
    # -port_conn <port_conn_name>             string  // The name of the port instance_name::port_name
    set options {-port_conn }
}

proc get_port_connection_sink_set { args } {
    # -port_conn <port_conn_name>             string  // The name of the port instance_name::port_name
    set options {-port_conn }
}

proc get_net_source { args } {
    # -net <net_name>                         string  // The name of the net
    set options {-net }
}

proc get_net_sink_set { args } {
    # -net <net_name>                         string  // The name of the net
    set options {-net }
}

proc get_logic_location { args } {
    # -instance <instance_name>  |            string  // The name of the instance from which we are reading.
    # -id <id>                                int     //

    set options {-instance -id }
}

proc set_logic_location  { args } {
    # -instance <instance_name>   |           string  // The name of the instance from which we are reading.
    # -id <id>                                 int     //
    # -location <liocation>                   {int, int} //
    set options {-instance -id -location }
}

proc get_logic_address { args } {
    # -instance <instance_name>               string  // The name of the instance from which we are reading.
    set options {-instance }
}

proc get_io_bank { args } {
    # -instance <instance_name>   |           string  // The name of the instance from which we are reading.
    # -id <id>
    set options {-instance -id }
}

proc set_io_bank  { args } {
    # -instance <instance_name>   |           string  // The name of the instance from which we are reading.
    # -id <id>
    # -io_bank {$IO_BANK}

    set options {-instance -id -io_bank }
}

proc get_phy_address  { args } {
    # -instance <instance_name>  |            string  // The name of the instance from which we are reading.
    # -id <id>                                int     //
    set options {-instance -id }
}

proc set_phy_address  { args } {
    # -instance <instance_name>   |           string  // The name of the instance from which we are reading.
    # -id <id>                                int     //
    # -address <ph_address>            int     //
    set options {-instance -id -address }
}

source $model

puts "RIC: Done."

