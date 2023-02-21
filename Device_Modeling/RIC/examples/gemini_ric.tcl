
# Gemini Raptor IP Configuration (RIC) description
# This is an example only, not a representation of the actual architecture

device_name GEMINI 

ric_schema_version 1.0 

device_version 1.0 

####################################
# Define the block definition (Example GEARBOX. Note:  IP block definitions can be in separate .tcl files sourced together to form the complete device configuration space) 

define_block -name HV_IOBANK

define_block -name GEARBOX

# define its attributes (parameters) 

 
define_attr -block "GEARBOX" -name "RATIO" -addr "0x004" -width 4 -enum {One 0x1} {Two 0x2} {Tree 0x3} {Ten 0xA} -enumname T1
define_attr -block "GEARBOX" -name "DIFF_OR_SINGLE" -addr 0x009 -width 1 -enum {Differential 0x0} {SingleEnded 0x1 default}  -enumname T2
define_attr -block "GEARBOX" -name "MASTER_SLAVE" -addr 0x012 -width 2 -enum {Single 0x0 default} {Master 0x1} {Slave 0x2}   -enumname T3
define_attr -block "GEARBOX" -name "MODE" -addr 0x017 -width 8 -enum {Mode_BP_SDR_A_TX 0x012} {Mode_BP_DDR_A_TX 0x013 default} {Mode_RATE_3_A_TX  0x014}    -enumname T4

# Constraints within block attributes 

define_constraint -block GEARBOX -constraint {(DIFF_OR_SINGLE == Differential) -> {RATIO inside {One Two}}}
define_constraint -block GEARBOX -constraint {(DIFF_OR_SINGLE == SingleEnded) -> {Ratio inside {[One:Ten]}} 
define_constraint -block GEARBOX -constraint {(MODE == Mode_BP_SDR_A_TX) -> {DIFF_OR_SINGLE == Differential}} 


######################################
# Create GBX instances (Layout) 
# create_instance -block <type> -name <name> -id <id> -logic_location {x y} -logic_address <logical_address> -io_bank <io_bank_name_optional> 


# Method 1: create block instances one by one (With hierarchical folded relation) 

 
create_instance -block “GEARBOX" -name “GBX0_0” -id 0 -logic_location {0 0} -logic_address 0xFF00 -io_bank BANK0 -parent HV_IOBANK   
create_instance -block “GEARBOX" -name “GBX1_0” -id 1 -logic_location {1 0} -logic_address 0xFF01 -io_bank BANK0 -parent HV_IOBANK 

 

create_instance -block HV_IOBANK -name HV_IOBANK_TOP_LEFT  
create_instance -block HV_IOBANK -name HV_IOBANK_TOP_RIGHT 

 
# Inter block instance constraints using block hierarchical path attributes and values 

# If gearbox is master, next (below) has to be slave 
define_constraint -block __ROOT__ -constraint {(GBX0_0.MASTER_SLAVE == GBX0_0.Master) -> {GBX1_0.MASTER_SLAVE == GBX1_0.Slave}} 

# If gearbox is single, previous one (above) cannot be master 
define_constraint  -block __ROOT__ -constraint {(GBX1_0.MASTER_SLAVE == GBX0_0.Single) -> {GBX0_0.MASTER_SLAVE == GBX0_0.Single || GBX0_0.MASTER_SLAVE == GBX0_0.Slave}} 



# Method 2: create blocks with for loop 
set GBX_START_ID 0
set GBX_END_ID 40
# ids are absolute and unique for the entire model, creating objects with the same is is a fatal error
for {set gbx_instance_id $GBX_START_ID} {$gbx_instance_id <= $GBX_END_ID} {incr gbx_instance_id} { 

    create_instance -block “GEARBOX" -name GBX_$gbx_instance_id _Y} -id $gbx_instance_id -logic_location  “$gbx_instance_id  0” -logic_address 0x[expr $gbx_instance_id * 10] -io_bank BANK0 -parent HV_IOBANK

   # Create constraints in between blocks 

} 

 
####################################
# Device level functions 

# Create IO configuration chain (instance of a chain)
set START_LOGICAL_ADDRESS 0x00000
set END_LOGICAL_ADDRESS   0x0F000
create_chain_instance -type ICB_CHAIN -name GBX_IO_CHAIN -start_address $START_LOGICAL_ADDRESS -end_address $END_LOGICAL_ADDRESS

#for {set gbx_instance_id $GBX_START_ID} {$gbx_instance_id <= $GBX_END_ID} {incr gbx_instance_id} { 
#   append_instance_to_chain -chain “GBX_IO_CHAIN” -inst [get_instance_by_id {$gbx_instance_id}] 
#} 

# Create IO configuration controller definition and instance 

define_block -name ICB_CONTROLLER 

create_instance -block ICB_CONTROLLER -name GBX_CONTROLLER_BLOCK

 

# Link IO configuration chain to controller instance 

link_chain -inst GBX_CONTROLLER_BLOCK -chain GBX_IO_CHAIN

# Create PLL blocks 

# Similarly, like GBX blocks, we can use for loop to create PLL blocks, which is not shown here 

 


