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
}

proc device_name { args } {
}

proc device_version { args } {
}

proc define_enum_type { args } {
}
                 
proc define_block { args } {
}
                   

proc define_attr { args } {
}

proc define_port { args } {
}
                    

proc create_instance { args } {
}

                 
proc define_constraint { args } {
}
                   
proc define_chain { args } {
}
                   
proc add_block_to_chain_type { args } {
}
                    

proc create_chain_instance { args } {
}
                  

proc link_chain { args } {
}
               

proc append_instance_to_chain { args } {
}
                   
source $model

puts "RIC: Done." 

