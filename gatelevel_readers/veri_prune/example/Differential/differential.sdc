# Timing constraints
create_clock -name clk -period 5 [get_ports CLK_p]
set_input_delay -clock clk -max 3 [get_ports D_p]
set_input_delay -clock clk -min 1 [get_ports D_p]
set_output_delay -clock clk -max 2.5 [get_ports Q_p]
set_output_delay -clock clk -min 0.5 [get_ports Q_p]

# Location Constraints
# I/O (May not be provided early in design cycle)
# Only necessary to constrain the P-side but can constrain N-side as optional
set_property PIN_LOC AH26 [get_ports D_p]   # D_p pin constrained to pin AH26
set_property PIN_LOC AE18 [get_ports D_n]   # D_n pin constrained to pin AE18 which is the matched diff pin to AH26
set_property PIN_LOC AE6 [get_ports CLK_p] # CLK_p pin constrained to clock-capable pin AE6, CLK_n will be assigned to AE6 automatically which is matched diff pair
set_property PIN_LOC AC19 [get_ports Q_p]   # Q_p pin constrained to pin AC19, Q_n will be assigned to AA17 automatically which is matched diff pair
#Fabric (Optional)
#set_property LOC <FLE_register_coordinate> [get_cells q_reg_dff_inst]   # Inferred regsiter is assigned to a specific register location in the fabric
# Push Register into I/O (If not specified, place register in FLE)
set_property IO_REG TRUE [get_ports Q_p] # In conflict with fabric propertly LOC so use this or the other (or none)

# Specify I/O Standard, for diff standards, P-side is required, N-side is optional (If not specified, use default)
set_property IOSTANDARD LVDS_HR [get_ports D_p]
set_property IOSTANDARD LVDS_HR [get_ports D_n]
set_property IOSTANDARD LVPECL [get_ports CLK_p]
set_property IOSTANDARD LVDS_HR [get_ports Q_p]

# Specify Differential Termination (If not specified, use default) - SLEW, DRIVE_STRENGTH not suported for Differntial I/O
set_property DIFFERENTIAL_TERMINATION TRUE [get_ports Q_p]
set_property DIFFERENTIAL_TERMINATION TRUE [get_ports Q_n]
set_property DIFFERENTIAL_TERMINATION TRUE [get_ports D_p]
set_property DIFFERENTIAL_TERMINATION TRUE [get_ports Q_p]

