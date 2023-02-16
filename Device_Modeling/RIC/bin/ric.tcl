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

set ::enum_scope [ dict create ]

set ::param_scope [ dict create ]

set ::block_scope [ dict create ]

set ::attribute_scope [ dict create ]

# Lookup this scope then ::enum_scope
set ::param_type_scope [ dict create integer 32 double 64 string 32 ]

set ::optionpat {
    ^[\-]([a-z]+)?$
}

set ::version_pat {
    ^\d+(\.\d+){2}$
}

set ::simple_id_pat {
    ^[a-zA-Z_]([a-zA-Z0-9\[\]_-]+)?$
}

set ::hier_id_pat {
    ^[a-zA-Z_]([a-zA-Z0-9\[\]_-]+)?(\.[a-zA-Z_]([a-zA-Z0-9\[\]_-]+)?)?$
}

set ::par_name_ports [ dict create  \
    -name { 1 simple_id hier_id } \
    -ports { -1 list } \
    -force { 0 Nill } \
]

set ::pat_block_ports [ dict create  \
    -block { 1 simple_id } \
    -ports { -1 list } \
]

set ::par_name_value_List [ dict create  \
    -name { 1 simple_id hier_id } \
    -values { -1 list } \
    -force { 0 Nill } \
]

set ::attribute_pat [ dict create  \
    -block  {  1 simple_id hier_id } \
    -name   {  1 simple_id } \
    -type   {  1 simple_id } \
    -addr   {  1 integer } \
    -width  {  1 integer } \
    -enum { -1 list } \
    -enumname   {  1 simple_id  hier_id } \
    -force  {  0 Nill } \
]

set ::ric_schema_version 0.0
set ::device_name None
set ::device_version 0.0

proc dm_type value {
    if { [regexp -expanded $::simple_id_pat $value ] } {
        return simple_id
    } elseif { [regexp -expanded $::hier_id_pat $value ] } {
        return hier_id
    } elseif { [regexp -expanded $::version_pat $value ] } {
        return version
    }  elseif { [regexp -expanded $::optionpat $value ] } {
        return option
    }   elseif {[string is integer -strict $value]} {
        return  integer
    } elseif {[string is double -strict $value]} {
        return  double
    } elseif {[string is boolean -strict $value]} {
        return  boolean
    } elseif {[string is xdigit -strict $value]} {
        return  xdigit
    } elseif {[string is alpha -strict $value]} {
        return  alpha
    } elseif {[string is list -strict $value]} {
        return  list
    } else {
        return __unknown_type__
    }
}

proc verify_make_params { params actuals } {
    set idx 0
    set dictRes [dict create valid 1 ]
    while { $idx < [llength $actuals ] } {
        set op [ lindex $actuals $idx ]
        set tp [ dm_type $op ]
        if { $tp != "option" } {
            puts "Too much values for last option at this value \"$op\""
            puts "Got $tp while expecting option"
            dict set dictRes valid 0
            return $dictRes
        }
        if { ! [dict exists $params $op] } {
            puts "Unknown option : $op"
            dict set dictRes valid 0
            return $dictRes
        }
        set valDesc [ dict get $params $op ]
        if { "list" != [ dm_type $valDesc ] } {
            puts "Not well formed option dictionary"
            dict set dictRes valid 0
            return $dictRes
        }
        set numValues [ lindex $valDesc 0 ]
        incr idx
        if { ! $numValues } {
            dict set dictRes $op 1
            continue
        }
        set values {  }
        if { $numValues > 0 } {
            while { $idx < [llength $actuals ] && $numValues } {
                set typeCheck 0
                set i 1
                while { $i < [llength $valDesc ] } {
                    set elem [ lindex $actuals $idx ]
                    set elType [ dm_type $elem ]
                    set expectedType [ lindex $valDesc $i ]
                    set typeCheck [ expr { $typeCheck || [ expr { $elType ==  $expectedType } ] }  ]
                    if { $typeCheck } {
                        lappend values $elem
                        break
                    }
                    incr i
                }
                if { ! $typeCheck } {
                    puts "The option $op takes values of type $expectedType , $elem is of type $elType"
                    dict set dictRes valid 0
                    return $dictRes
                }
                incr idx
                incr numValues -1
            }
            if { $numValues } {
                puts "The option $op takes $numValues values"
                dict set dictRes valid 0
                return $dictRes
            }
            if { [dict exists $dictRes $op] } {
                puts "The option $op is specified twice"
                dict set dictRes valid 0
                return $dictRes
            }
            dict set dictRes $op $values
        }
        if { $numValues == -1 } {
            set cnt 0
            set elem [ lindex $actuals $idx ]
            set elType [ dm_type [ lindex $actuals $idx ] ]
            set expectedType [ lindex $valDesc 1 ]

            set values { }
            while { $idx < [llength $actuals ] && "option" != $elType  } {
                if { $expectedType != $elType } {
                    puts "The option $op takes values of type [ lindex $valDesc 1 ]"
                    dict set dictRes valid 0
                    return $dictRes
                }
                incr idx
                incr cnt
                lappend values $elem
                set elem [ lindex $actuals $idx ]
                set elType [ dm_type [ lindex $actuals $idx ] ]
            }
            if { ! $cnt } {
                puts "The option $op needs at least one argument"
                dict set dictRes valid 0
                return $dictRes
            }
            if { [dict exists $dictRes $op] } {
                puts "The option $op is specified twice"
                dict set dictRes valid 0
                return $dictRes
            }
            dict set dictRes $op $values
        }

    }
    return $dictRes
}

#===================================================================
#                           RIC Commands
#===================================================================

proc ric_schema_version { args } {
    set tp [ dm_type $args ]
    set dictRes [dict create valid 1 ]
    if { $tp == "list"  || $tp == "double" || $tp == "version" } {
        set ::ric_schema_version $args
        return $dictRes
    }
    puts "Invalid ric_schema_version : $args"
    dict set dictRes valid 0
    return $dictRes
}

proc device_name  { args } {
    set tp [ dm_type $args ]
    set dictRes [dict create valid 1 ]
    if { $tp == "simple_id" } {
        set ::device_name $args
        return $dictRes
    }
    puts "Invalid device_name : $args"
    dict set dictRes valid 0
    return $dictRes
}

proc device_version  { args } {
    set tp [ dm_type $args ]
    set dictRes [dict create valid 1 ]
    if { $tp == "list"  || $tp == "double" || $tp == "version" } {
        set ::device_version $args
        return $dictRes
    }
    puts "Invalid device_version : $args"
    dict set dictRes valid 0
    return $dictRes
}

proc define_enum_type { args } {
    # -name <enumerate>
    # -values { {<name>, value }, …, {<name>, value }, {"default", value } }
    # -force Nill   // Overrides older definition
    # example : puts [ define_enum_type -name myAttrType -values { { one 0x7 } { two 0x2 } { three 0x3} { default 0x9 } } -force ]

    set paramDict $::par_name_value_List
    set actual [ verify_make_params $paramDict $args ]
    set valuesDict [dict create ]
    set max -1
    set min 1000000
    dict set actual valid 0
    foreach elem [ lindex [ dict get $actual "-values" ] 0 ] {
        set tp  [ dm_type $elem ]
        if { $tp != "list" } {
            puts "An enum element should be a name vlue pair"
            return $actual
        }
        set l [ llength $elem]
        if { 2 > $l || 3 < $l } {
            if {$l } {
                puts "An enum element should be a name vlue pair at enum \"[lindex $elem 0]\""
            } else {
                puts "An enum element should be a name vlue pair , empty enum spec$elem in [ dict get $actual "-name" ]"
            }
            return $actual
        }
        set name [lindex $elem 0]
        set value [lindex $elem 1]
        set ntp [ dm_type $name ]
        set vtp [ dm_type $value ]
        if { "simple_id" != $ntp || "integer" != $vtp || $value < 0 } {
            puts "Mal formed attribute element { $elem }"
            return $elem
        }
        if {  [dict exists $valuesDict $name ] } {
            puts "$epeated attribute element { $elem }, ignoring"
            continue
        }
        dict set valuesDict $name $value
        # allows for default value to be un-named to allow forcing an distinct setting when needed
        if { $value < $min } {
            set min $value
        }
        if { $value > $max } {
            set max $value
        }
    }
    set width 1
    set m 1
    while { ($m << $width) <= $max } {
        incr width
    }
    dict set actual "-width" $width
    dict set actual "-values" $valuesDict

    if { ! [dict exists $::enum_scope [ dict get $actual "-name" ] ] || [dict exists $actual "-force" ]} {
        dict set actual valid 1
        dict set ::enum_scope [ dict get $actual "-name" ] $actual
        return 1
    }
    return 0
}

proc define_block  { args } {
    # -name <type>                      string // The name of the currently defined block
    # -ports  <list_of_port_objects>..  [ {DIR port , port} ] // Set of ports
    # Example : define_block -ports { in a b c } { out aa bb cc } { in ss } -name kkk

    set paramDict $::par_name_ports
    set actual  [ verify_make_params $paramDict $args ]
    # Fast access to keys, values not used for now, may get width or usage type (clk)
    set inputs  [ dict create ]
    set outputs [ dict create ]
    dict set actual valid 0
    if { [ dict exists $actual "-ports"] } {
        foreach port_set [ dict get $actual "-ports" ]  {
            if { "in" == [ lindex $port_set 0 ] } {
                set idx 1
                while {$idx < [llength $port_set ] } {
                    set in [ lindex $port_set $idx ]
                    if { "simple_id" != [ dm_type $in ]} {
                        puts "Not a suitable port name $in"
                        return 0
                    }
                    if { [dict exists $outputs $in ] || [dict exists $inputs $in ]} {
                        puts "Duplicated port name $out"
                        return 0
                    }
                    dict set inputs $in 1
                    incr idx
                }
            } elseif {"out" == [ lindex $port_set 0 ]  } {
                set idx 1
                while {$idx < [llength $port_set ] } {
                    set out [ lindex $port_set $idx ]
                    if { "simple_id" != [ dm_type $out ]} {
                        puts "Not a suitable port name $out"
                        return 0
                    }
                    if { [dict exists $outputs $out ] || [dict exists $inputs $out ]} {
                        puts "Duplicated port name $out"
                        return 0
                    }
                    dict set outputs $out 1
                    incr idx
                }
            } else {
                puts "Not a Valid specification of ports $port_set, should start with in or out"
                return 0
            }
        }
        dict set actual "-inputs" $inputs
        dict set actual "-outputs" $outputs
    }
    if { ![dict exists $::block_scope [ dict get $actual "-name" ]  ]  || [dict exists $actual "-force" ]} {
        dict set actual valid 1
        dict set ::block_scope [ dict get $actual "-name" ] $actual
        return 1
    }
    return 0
}

# We may encapsilate all Global definitions in a __ROOT__ or __DEVICE_NAME__ module to be able to cleanly handle several Desighs 
define_block -name __ROOT__

proc define_param { args } {
    # A parameter is a generalized attribute, it does not, necessarily, have to get an address neither a predefined size
    # -block <block_type_name> O   string // the name of a defined block to insert the attribute in if None then Global.
    # -name <attr_name>        N   string // The name of the currently defined attribute
    # -addr <offset_address>   O   string // Relative address of the attribute in the block config space.
    # -width <nb_bits>         O   int    // Number of bits intended to represent this parameter (to match with type)
    # -type <type_name>        N   string // Number of bits enough to code all possible values of this attribute
    # -force                   O   Nill    // Rewrite existing attribute if any
    # define_param -block GEARBOX -name P1 -addr 0x0 -width 0x4 -type integer
    set paramDict $::attribute_pat
    set actual  [ verify_make_params $paramDict $args ]
    puts $actual
    if { ! [ dict exists $actual "-name"] || ! [ dict exists $actual "-type"]  } {
        dict set actual valid 0
        puts "The options -name and -type"
        puts "are necessary in the command define_attr : $actual"
        dict set actual valid 0
        return 0
    }
    set name [ dict get $actual "-name" ]
    set type [ dict get $actual "-type" ]
    if { ! [ dict exists $::param_type_scope $type ] && ! [ dict exists $::enum_scope $type ]} {
        puts "Could not find type $type for parameter $name"
        dict set actual valid 0
        return 0
    }
    if { ( [ dict exists $::param_scope $name ] ||
        [ dict exists $::attribute_scope $name ] ) &&
        ! [ dict exists  $actual "-force" ]
        } { puts "Already defined parameter $name, no -force specified"
        dict set actual valid 0
        return 0
    }
    if { [ dict exists  $actual "-width" ] && [ dict get  $actual "-width" ] < 1 } {
        puts "The width of a parameter can not be less than 1"
        dict set actual valid 0
        return 0
    }
    if { [ dict exists  $actual "-addr" ] && [ dict get  $actual "-addr" ] < 0 } {
        puts "The address of a parameter can not be less than 0"
        dict set actual valid 0
        return 0
    }
    if { [ dict exists  $actual "-block" ] } {
        set block_name  [ dict get  $actual "-block" ]
        if { ! [dict exists  $::block_scope $block_name]} {
            puts "I could not find the block $block_name for the definition of the parameter $name"
            dict set actual valid 0
            return 0
        } else {
            if { [ dict exists ::block_scope $block_name "-params"  ] } {
                dict set ::block_scope $block_name -params [ dict create ]
            }
            dict set ::block_scope $block_name -params $name $actual
        }
    } else {
        dict set ::param_scope $name $actual
    }

    return [ dict get $actual valid ]
}

proc define_attr { args } {
    # Create an attribute named -name associated with -block (if specified ) and being global (about the device if no -block)
    # the values come from the enum type -enumname if the enumtype -enumname does not exist and -enum is specified we generate
    # the enumtype and call it <-block>.<-enumname> or global__.<-enumname>
    # -block <block_type_name>    string // the name of a defined block to insert the attribute in if None then Global.
    # -name <attr_name>           string // The name of the currently defined attribute
    # -addr <offset_address>      string // Relative address of the attribute in the block config space.
    # -width <nb_bits>            int // Number of bits enough to code all possible values of this attribute
    # -enum { {<name>, value }, …, {<name>, value }, {"default", value } } list // Inline Attribute enumerated values
    # -enumname <enumerate_name>  string // Offline Attribute enumerated values
    # -force                      Nill    // Rewrite existing attribute if any
    # set options {-block -name -addr -width -enum -force }
    # define_attr -block "GEARBOX" -enumname sdsd -name "MODE0" -addr "0x017" -width 8 -enum {Mode_BP_SDR_A_TX 0x012} {Mode_BP_DDR_A_TX 0x013 default} {Mode_RATE_3_A_TX  0x014}

    set paramDict $::attribute_pat
    set actual  [ verify_make_params $paramDict $args ]
    dict set actual valid 0
    if { ! [ dict exists $actual "-name"] || ! [ dict exists $actual "-addr"] || \
        ! [ dict exists $actual "-width"] || ! [ dict exists $actual "-enumname"] } {
        puts "The options -name, -addr, -width and -enumname"
        puts "are necessary in the command define_attr : $actual"
        return 0
    }
    set registeringName ""
    if { [ dict exists $actual "-enumname"] } {
        set baseName global__
        set enName [ dict get $actual "-enumname" ]
        if { [ dict exists $actual "-block"] } {
            set baseName [ dict get $actual "-block" ]
        }
        set registeringName [ string cat $baseName "." $enName ]
        if { ! [ dict exists $::enum_scope $enName ] } {
            if { [ dict exists $actual "-enum"] } {
                set res [ define_enum_type -name $registeringName -values [ dict get $actual "-enum" ] ]
                set defWidth [ dict get [ dict get $::enum_scope $registeringName ] "-width" ]
                set actualWidth [ dict get $actual "-width"]
                if { $defWidth > $actualWidth } {
                    puts "The specified width of $actualWidth is not enough to represent all the values of the attribute [ dict get $actual "-name"]"
                    puts "At least $defWidth bits are needed"
                    dict set actual valid 0
                    return 0
                }
            } else {
                puts "Could not find the definition of $enName and no inline definition is provided to define it"
                dict set actual valid 0
                return 0
            }
            dict set actual -type $registeringName
        } else {
            dict set actual -type $enName
        }
    } else {
        puts "Should not reach here DBG"
    }
    # ::attribute_scope
    if { [ dict exists $actual "-block"] } {
        set bName [ dict get $actual "-block"]
        set attributeName [ dict get $actual "-name"]
        if { ! [ dict exists $::block_scope $bName] } {
            puts "Could not find the block $bName"
            dict set actual valid 0
            return 0
        } else {
            if { [ dict exists [ dict get $::block_scope $bName] "-attributes" ] } {
                dict set ::block_scope $bName -attributes $attributeName $actual
            } else {
                dict set ::block_scope $bName -attributes [ dict create $attributeName $actual]
            }
        }
    } else {
        dict set ::attribute_scope [ dict get $actual "-name"] $actual
    }
    dict set actual valid 1
    return 1
}

proc define_ports { args } {
    # -block <block_type_name>    string // the name of a defined block to insert the port in
    # -ports <port_list>          list // a list of port definitions atarting by in/out each time
    set paramDict $::pat_block_ports
    set actual  [ verify_make_params $paramDict $args ]
    set inputs  [ dict create ]
    set outputs [ dict create ]
    dict set actual valid 0
    if { [ dict exists $actual "-block"] && [ dict exists $actual "-ports"] } {
        set block_name  [ dict get  $actual "-block" ]
        if { ! [dict exists  $::block_scope $block_name]} {
            puts "I could not find the block $block_name for the definition of ports $actual"
            dict set actual valid 0
            return 0
        }
        foreach port_set [ dict get $actual "-ports" ]  {
            if { "in" == [ lindex $port_set 0 ] } {
                set idx 1
                while {$idx < [llength $port_set ] } {
                    set in [ lindex $port_set $idx ]
                    if { "simple_id" != [ dm_type $in ]} {
                        puts "Not a suitable port name $in"
                        return 0
                    }
                    if { [dict exists $outputs $in ] || [dict exists $inputs $in ]} {
                        puts "Duplicated port name $out"
                        return 0
                    }
                    dict set inputs $in 1
                    incr idx
                }
            } elseif {"out" == [ lindex $port_set 0 ]  } {
                set idx 1
                while {$idx < [llength $port_set ] } {
                    set out [ lindex $port_set $idx ]
                    if { "simple_id" != [ dm_type $out ]} {
                        puts "Not a suitable port name $out"
                        return 0
                    }
                    if { [dict exists $outputs $out ] || [dict exists $inputs $out ]} {
                        puts "Duplicated port name $out"
                        return 0
                    }
                    dict set outputs $out 1
                    incr idx
                }
            } else {
                puts "Not a Valid specification of ports $port_set, should start with in or out"
                return 0
            }
        }
        if { [llength [dict keys $inputs] ]} {
            if { ! [ dict exists $::block_scope $block_name -inputs ] } {
                dict set ::block_scope $block_name -inputs [ dict create ]
            }
            foreach port_name [dict keys $inputs] {
                dict set ::block_scope $block_name -inputs $port_name 1
            }
        }
         if { [llength [dict keys $outputs] ]} {
            if { ! [ dict exists $::block_scope $block_name -outputs ] } {
                dict set ::block_scope $block_name -outputs [ dict create ]
            }
            foreach port_name [dict keys $outputs] {
                dict set ::block_scope $block_name -outputs $port_name 1
            }
        }
    } else {
        puts "A port definition must have a -block and a -ports parameter."
        return 0
    }
}

proc define_constraint { args } {
    # -block <block_type_name>          // Inner block constraint in the form of SV implication
    # -contraint <sv-like-implication>  // Inner block constraint in the form of SV implication
    set options {-block }
    set pat_constraint [ dict create  -block { 1 simple_id } -constraint { -1 list } ]
    set paramDict $pat_constraint
    set actual [ verify_make_params $paramDict $args ]
    if { ! [ dict exists $actual "-block"] || ! [ dict exists $actual "-constraint"]  } {
        dict set actual valid 0
        puts "The options -block and -constraint"
        puts "are necessary in the command define_constraint : $actual"
        dict set actual valid 0
        return 0
    }
    set block_name [ dict get $actual "-block" ]
    set const [ dict get $actual "-constraint" ]
    if { ! [dict exists  $::block_scope $block_name]} {
        puts "I could not find the block $block_name for the definition of the constraint $actual"
        dict set actual valid 0
        return 0
    }
    set key "Constraint0"
    if { ! [dict exists  $::block_scope $block_name -constraint ] } {
        dict set ::block_scope $block_name -constraint [dict create ]
    } else {
        set idx 1
        while { [ dict exists $::block_scope $block_name -constraint $key ] } {
            set key "Constraint$idx"
            incr idx
        }
    }
    dict set ::block_scope $block_name -constraint $key $const
    # puts [dict get $::block_scope $block_name]
    return 1
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

proc drive_port { args } {
    # -name <port_name>               string  // The name of the currently driven net.
    # -source <net_name/port_name>    string  // The name of the driver of the currently driven net.
    # // port_name should be in the format instance_name.port_name

    set options {-name -source }
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

proc get_block_names { args } {
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

