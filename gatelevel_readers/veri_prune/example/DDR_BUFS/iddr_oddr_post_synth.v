/* Generated by Yosys 0.18+10 (git sha1 daf9624a5, gcc 11.2.0 -fPIC -Os) */

module ddr_mem_intf(clk, reset, data_in, write_enable, empty, full, data_out);
  input clk;
  input data_in;
  output data_out;
  output empty;
  output full;
  input reset;
  input write_enable;
  wire _000_;
  wire _001_;
  wire _002_;
  wire _003_;
  (* keep = 32'h00000001 *)
  wire _004_;
  (* keep = 32'h00000001 *)
  wire _005_;
  (* keep = 32'h00000001 *)
  wire _006_;
  (* keep = 32'h00000001 *)
  wire _007_;
  (* keep = 32'h00000001 *)
  wire _008_;
  (* keep = 32'h00000001 *)
  wire _009_;
  (* keep = 32'h00000001 *)
  wire _010_;
  (* keep = 32'h00000001 *)
  wire _011_;
  wire _012_;
  wire _013_;
  (* keep = 32'h00000001 *)
  wire _014_;
  (* keep = 32'h00000001 *)
  wire _015_;
  (* keep = 32'h00000001 *)
  wire _016_;
  wire _017_;
  wire _018_;
  wire _019_;
  wire _020_;
  wire _021_;
  wire _022_;
  wire _023_;
  wire _024_;
  wire _025_;
  wire _026_;
  wire _027_;
  wire _028_;
  wire _029_;
  wire _030_;
  wire _031_;
  wire _032_;
  (* src = "/nfs_eda_sw/softwares/Raptor/instl_dir/06_23_2023_09_15_01/bin/../share/yosys/rapidsilicon/genesis3/ffs_map.v:80.61-80.63" *)
  wire _033_;
  (* keep = 32'h00000001 *)
  wire _034_;
  (* keep = 32'h00000001 *)
  wire _035_;
  (* keep = 32'h00000001 *)
  wire _036_;
  (* keep = 32'h00000001 *)
  wire _037_;
  (* keep = 32'h00000001 *)
  wire _038_;
  (* keep = 32'h00000001 *)
  wire _039_;
  (* keep = 32'h00000001 *)
  wire _040_;
  (* keep = 32'h00000001 *)
  wire _041_;
  (* keep = 32'h00000001 *)
  wire _042_;
  (* keep = 32'h00000001 *)
  wire _043_;
  (* keep = 32'h00000001 *)
  wire _044_;
  (* keep = 32'h00000001 *)
  wire _045_;
  (* keep = 32'h00000001 *)
  wire _046_;
  (* keep = 32'h00000001 *)
  wire _047_;
  (* keep = 32'h00000001 *)
  wire _048_;
  (* keep = 32'h00000001 *)
  wire _049_;
  (* keep = 32'h00000001 *)
  wire _050_;
  (* keep = 32'h00000001 *)
  wire _051_;
  (* keep = 32'h00000001 *)
  wire _052_;
  (* keep = 32'h00000001 *)
  wire _053_;
  (* keep = 32'h00000001 *)
  wire _054_;
  (* keep = 32'h00000001 *)
  wire _055_;
  (* keep = 32'h00000001 *)
  wire _056_;
  (* keep = 32'h00000001 *)
  wire _057_;
  (* keep = 32'h00000001 *)
  wire _058_;
  (* keep = 32'h00000001 *)
  wire _059_;
  (* keep = 32'h00000001 *)
  wire _060_;
  (* keep = 32'h00000001 *)
  wire _061_;
  (* keep = 32'h00000001 *)
  wire _062_;
  (* keep = 32'h00000001 *)
  wire _063_;
  (* keep = 32'h00000001 *)
  wire _064_;
  (* keep = 32'h00000001 *)
  wire _065_;
  (* keep = 32'h00000001 *)
  wire _066_;
  (* keep = 32'h00000001 *)
  wire _067_;
  (* keep = 32'h00000001 *)
  wire _068_;
  (* keep = 32'h00000001 *)
  wire _069_;
  (* keep = 32'h00000001 *)
  wire _070_;
  (* keep = 32'h00000001 *)
  wire _071_;
  (* keep = 32'h00000001 *)
  wire _072_;
  (* keep = 32'h00000001 *)
  wire _073_;
  (* keep = 32'h00000001 *)
  wire _074_;
  (* keep = 32'h00000001 *)
  wire _075_;
  (* keep = 32'h00000001 *)
  wire _076_;
  (* keep = 32'h00000001 *)
  wire _077_;
  (* keep = 32'h00000001 *)
  wire _078_;
  (* keep = 32'h00000001 *)
  wire _079_;
  (* keep = 32'h00000001 *)
  wire [1:0] _080_;
  (* keep = 32'h00000001 *)
  wire _081_;
  (* src = "/nfs_eda_sw/softwares/Raptor/instl_dir/06_23_2023_09_15_01/bin/../share/yosys/rapidsilicon/genesis3/brams_map_new.v:379.4-390.3|/nfs_eda_sw/softwares/Raptor/instl_dir/06_23_2023_09_15_01/bin/../share/yosys/rapidsilicon/genesis3/brams_final_map_new.v:397.28-397.34" *)
  (* unused_bits = "0" *)
  wire _082_;
  (* src = "/nfs_eda_sw/softwares/Raptor/instl_dir/06_23_2023_09_15_01/bin/../share/yosys/rapidsilicon/genesis3/brams_map_new.v:379.4-390.3|/nfs_eda_sw/softwares/Raptor/instl_dir/06_23_2023_09_15_01/bin/../share/yosys/rapidsilicon/genesis3/brams_final_map_new.v:6522.14-6522.27" *)
  (* unused_bits = "2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17" *)
  wire [17:0] _083_;
  (* src = "/nfs_eda_sw/softwares/Raptor/instl_dir/06_23_2023_09_15_01/bin/../share/yosys/rapidsilicon/genesis3/brams_map_new.v:379.4-390.3|/nfs_eda_sw/softwares/Raptor/instl_dir/06_23_2023_09_15_01/bin/../share/yosys/rapidsilicon/genesis3/brams_final_map_new.v:6524.14-6524.27" *)
  (* unused_bits = "0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17" *)
  wire [17:0] _084_;
  (* src = "/nfs_eda_sw/softwares/Raptor/instl_dir/06_23_2023_09_15_01/bin/../share/yosys/rapidsilicon/genesis3/brams_map_new.v:379.4-390.3|/nfs_eda_sw/softwares/Raptor/instl_dir/06_23_2023_09_15_01/bin/../share/yosys/rapidsilicon/genesis3/brams_final_map_new.v:6523.14-6523.27" *)
  (* unused_bits = "0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17" *)
  wire [17:0] _085_;
  (* src = "/nfs_eda_sw/softwares/Raptor/instl_dir/06_23_2023_09_15_01/bin/../share/yosys/rapidsilicon/genesis3/brams_map_new.v:379.4-390.3|/nfs_eda_sw/softwares/Raptor/instl_dir/06_23_2023_09_15_01/bin/../share/yosys/rapidsilicon/genesis3/brams_final_map_new.v:6525.14-6525.27" *)
  (* unused_bits = "0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17" *)
  wire [17:0] _086_;
  (* src = "/nfs_scratch/zafar/FPGA_PRIMITIVE_TEST_CASE/ddr_mem_intf.sv:3" *)
  (* src = "/nfs_scratch/zafar/FPGA_PRIMITIVE_TEST_CASE/ddr_mem_intf.sv:3" *)
  wire clk;
  (* src = "/nfs_scratch/zafar/FPGA_PRIMITIVE_TEST_CASE/ddr_mem_intf.sv:5" *)
  (* src = "/nfs_scratch/zafar/FPGA_PRIMITIVE_TEST_CASE/ddr_mem_intf.sv:5" *)
  wire data_in;
  (* src = "/nfs_scratch/zafar/FPGA_PRIMITIVE_TEST_CASE/ddr_mem_intf.sv:9" *)
  (* src = "/nfs_scratch/zafar/FPGA_PRIMITIVE_TEST_CASE/ddr_mem_intf.sv:9" *)
  wire data_out;
  (* src = "/nfs_scratch/zafar/FPGA_PRIMITIVE_TEST_CASE/ddr_mem_intf.sv:7" *)
  (* src = "/nfs_scratch/zafar/FPGA_PRIMITIVE_TEST_CASE/ddr_mem_intf.sv:7" *)
  wire empty;
  (* hdlname = "fifo data_in" *)
  (* src = "/nfs_scratch/zafar/FPGA_PRIMITIVE_TEST_CASE/ddr_mem_intf.sv:56" *)
  wire [1:0] \fifo.data_in ;
  (* hdlname = "fifo data_out" *)
  (* src = "/nfs_scratch/zafar/FPGA_PRIMITIVE_TEST_CASE/ddr_mem_intf.sv:57" *)
  wire [1:0] \fifo.data_out ;
  (* hdlname = "fifo empty" *)
  (* src = "/nfs_scratch/zafar/FPGA_PRIMITIVE_TEST_CASE/ddr_mem_intf.sv:59" *)
  wire \fifo.empty ;
  (* hdlname = "fifo full" *)
  (* src = "/nfs_scratch/zafar/FPGA_PRIMITIVE_TEST_CASE/ddr_mem_intf.sv:58" *)
  wire \fifo.full ;
  (* hdlname = "fifo next_write_pointer" *)
  (* src = "/nfs_scratch/zafar/FPGA_PRIMITIVE_TEST_CASE/ddr_mem_intf.sv:68" *)
  (* unused_bits = "8" *)
  wire [8:0] \fifo.next_write_pointer ;
  (* hdlname = "fifo read_pointer" *)
  (* keep = 32'h00000001 *)
  (* src = "/nfs_scratch/zafar/FPGA_PRIMITIVE_TEST_CASE/ddr_mem_intf.sv:67" *)
  wire [8:0] \fifo.read_pointer ;
  (* hdlname = "fifo write_enable" *)
  (* src = "/nfs_scratch/zafar/FPGA_PRIMITIVE_TEST_CASE/ddr_mem_intf.sv:54" *)
  wire \fifo.write_enable ;
  (* hdlname = "fifo write_pointer" *)
  (* keep = 32'h00000001 *)
  (* src = "/nfs_scratch/zafar/FPGA_PRIMITIVE_TEST_CASE/ddr_mem_intf.sv:66" *)
  wire [8:0] \fifo.write_pointer ;
  (* src = "/nfs_scratch/zafar/FPGA_PRIMITIVE_TEST_CASE/ddr_mem_intf.sv:8" *)
  (* src = "/nfs_scratch/zafar/FPGA_PRIMITIVE_TEST_CASE/ddr_mem_intf.sv:8" *)
  wire full;
  (* src = "/nfs_scratch/zafar/FPGA_PRIMITIVE_TEST_CASE/ddr_mem_intf.sv:4" *)
  (* src = "/nfs_scratch/zafar/FPGA_PRIMITIVE_TEST_CASE/ddr_mem_intf.sv:4" *)
  wire reset;
  (* src = "/nfs_scratch/zafar/FPGA_PRIMITIVE_TEST_CASE/ddr_mem_intf.sv:6" *)
  (* src = "/nfs_scratch/zafar/FPGA_PRIMITIVE_TEST_CASE/ddr_mem_intf.sv:6" *)
  wire write_enable;
  (* module_not_derived = 32'h00000001 *)
  (* src = "/nfs_eda_sw/softwares/Raptor/instl_dir/06_23_2023_09_15_01/bin/../share/yosys/rapidsilicon/genesis3/ffs_map.v:80.11-80.65" *)
  dffre _087_ (
    .C(clk),
    .D(\fifo.next_write_pointer [2]),
    .E(_001_),
    .Q(\fifo.write_pointer [2]),
    .R(_033_)
  );
  (* module_not_derived = 32'h00000001 *)
  (* src = "/nfs_eda_sw/softwares/Raptor/instl_dir/06_23_2023_09_15_01/bin/../share/yosys/rapidsilicon/genesis3/ffs_map.v:80.11-80.65" *)
  dffre _088_ (
    .C(clk),
    .D(\fifo.next_write_pointer [3]),
    .E(_001_),
    .Q(\fifo.write_pointer [3]),
    .R(_033_)
  );
  (* module_not_derived = 32'h00000001 *)
  (* src = "/nfs_eda_sw/softwares/Raptor/instl_dir/06_23_2023_09_15_01/bin/../share/yosys/rapidsilicon/genesis3/ffs_map.v:80.11-80.65" *)
  dffre _089_ (
    .C(clk),
    .D(\fifo.next_write_pointer [4]),
    .E(_001_),
    .Q(\fifo.write_pointer [4]),
    .R(_033_)
  );
  (* module_not_derived = 32'h00000001 *)
  (* src = "/nfs_eda_sw/softwares/Raptor/instl_dir/06_23_2023_09_15_01/bin/../share/yosys/rapidsilicon/genesis3/ffs_map.v:80.11-80.65" *)
  dffre _090_ (
    .C(clk),
    .D(\fifo.next_write_pointer [0]),
    .E(_001_),
    .Q(\fifo.write_pointer [0]),
    .R(_033_)
  );
  (* module_not_derived = 32'h00000001 *)
  (* src = "/nfs_eda_sw/softwares/Raptor/instl_dir/06_23_2023_09_15_01/bin/../share/yosys/rapidsilicon/genesis3/ffs_map.v:80.11-80.65" *)
  dffre _091_ (
    .C(clk),
    .D(\fifo.next_write_pointer [7]),
    .E(_001_),
    .Q(\fifo.write_pointer [7]),
    .R(_033_)
  );
  (* module_not_derived = 32'h00000001 *)
  (* src = "/nfs_eda_sw/softwares/Raptor/instl_dir/06_23_2023_09_15_01/bin/../share/yosys/rapidsilicon/genesis3/ffs_map.v:80.11-80.65" *)
  dffre _092_ (
    .C(clk),
    .D(\fifo.next_write_pointer [1]),
    .E(_001_),
    .Q(\fifo.write_pointer [1]),
    .R(_033_)
  );
  (* module_not_derived = 32'h00000001 *)
  (* src = "/nfs_eda_sw/softwares/Raptor/instl_dir/06_23_2023_09_15_01/bin/../share/yosys/rapidsilicon/genesis3/ffs_map.v:80.11-80.65" *)
  dffre _093_ (
    .C(clk),
    .D(\fifo.next_write_pointer [6]),
    .E(_001_),
    .Q(\fifo.write_pointer [6]),
    .R(_033_)
  );
  (* module_not_derived = 32'h00000001 *)
  (* src = "/nfs_eda_sw/softwares/Raptor/instl_dir/06_23_2023_09_15_01/bin/../share/yosys/rapidsilicon/genesis3/ffs_map.v:80.11-80.65" *)
  dffre _094_ (
    .C(clk),
    .D(\fifo.next_write_pointer [5]),
    .E(_001_),
    .Q(\fifo.write_pointer [5]),
    .R(_033_)
  );
  (* module_not_derived = 32'h00000001 *)
  (* src = "/nfs_eda_sw/softwares/Raptor/instl_dir/06_23_2023_09_15_01/bin/../share/yosys/rapidsilicon/genesis3/ffs_map.v:80.11-80.65" *)
  dffre _095_ (
    .C(clk),
    .D(_017_),
    .E(_003_),
    .Q(\fifo.read_pointer [6]),
    .R(_033_)
  );
  (* module_not_derived = 32'h00000001 *)
  (* src = "/nfs_eda_sw/softwares/Raptor/instl_dir/06_23_2023_09_15_01/bin/../share/yosys/rapidsilicon/genesis3/ffs_map.v:80.11-80.65" *)
  dffre _096_ (
    .C(clk),
    .D(_018_),
    .E(_003_),
    .Q(\fifo.read_pointer [7]),
    .R(_033_)
  );
  (* module_not_derived = 32'h00000001 *)
  (* src = "/nfs_eda_sw/softwares/Raptor/instl_dir/06_23_2023_09_15_01/bin/../share/yosys/rapidsilicon/genesis3/ffs_map.v:80.11-80.65" *)
  dffre _097_ (
    .C(clk),
    .D(_019_),
    .E(_003_),
    .Q(\fifo.read_pointer [5]),
    .R(_033_)
  );
  (* module_not_derived = 32'h00000001 *)
  (* src = "/nfs_eda_sw/softwares/Raptor/instl_dir/06_23_2023_09_15_01/bin/../share/yosys/rapidsilicon/genesis3/ffs_map.v:80.11-80.65" *)
  dffre _098_ (
    .C(clk),
    .D(_020_),
    .E(_003_),
    .Q(\fifo.read_pointer [3]),
    .R(_033_)
  );
  (* module_not_derived = 32'h00000001 *)
  (* src = "/nfs_eda_sw/softwares/Raptor/instl_dir/06_23_2023_09_15_01/bin/../share/yosys/rapidsilicon/genesis3/ffs_map.v:80.11-80.65" *)
  dffre _099_ (
    .C(clk),
    .D(_021_),
    .E(_003_),
    .Q(\fifo.read_pointer [2]),
    .R(_033_)
  );
  (* module_not_derived = 32'h00000001 *)
  (* src = "/nfs_eda_sw/softwares/Raptor/instl_dir/06_23_2023_09_15_01/bin/../share/yosys/rapidsilicon/genesis3/ffs_map.v:80.11-80.65" *)
  dffre _100_ (
    .C(clk),
    .D(_022_),
    .E(_003_),
    .Q(\fifo.read_pointer [1]),
    .R(_033_)
  );
  (* module_not_derived = 32'h00000001 *)
  (* src = "/nfs_eda_sw/softwares/Raptor/instl_dir/06_23_2023_09_15_01/bin/../share/yosys/rapidsilicon/genesis3/ffs_map.v:80.11-80.65" *)
  dffre _101_ (
    .C(clk),
    .D(_023_),
    .E(_003_),
    .Q(\fifo.read_pointer [0]),
    .R(_033_)
  );
  (* module_not_derived = 32'h00000001 *)
  (* src = "/nfs_eda_sw/softwares/Raptor/instl_dir/06_23_2023_09_15_01/bin/../share/yosys/rapidsilicon/genesis3/ffs_map.v:80.11-80.65" *)
  dffre _102_ (
    .C(clk),
    .D(_024_),
    .E(_003_),
    .Q(\fifo.read_pointer [4]),
    .R(_033_)
  );
  (* module_not_derived = 32'h00000001 *)
  (* src = "/nfs_eda_sw/softwares/Raptor/instl_dir/06_23_2023_09_15_01/bin/../share/yosys/rapidsilicon/genesis3/ffs_map.v:10.11-10.70" *)
  dffre _103_ (
    .C(clk),
    .D(\fifo.data_out [0]),
    .E(1'h1),
    .Q(_014_),
    .R(1'h1)
  );
  (* module_not_derived = 32'h00000001 *)
  (* src = "/nfs_eda_sw/softwares/Raptor/instl_dir/06_23_2023_09_15_01/bin/../share/yosys/rapidsilicon/genesis3/ffs_map.v:10.11-10.70" *)
  dffre _104_ (
    .C(clk),
    .D(\fifo.data_out [1]),
    .E(1'h1),
    .Q(_015_),
    .R(1'h1)
  );
  (* module_not_derived = 32'h00000001 *)
  (* src = "/nfs_eda_sw/softwares/Raptor/instl_dir/06_23_2023_09_15_01/bin/../share/yosys/rapidsilicon/genesis3/ffs_map.v:10.11-10.70" *)
  dffre _105_ (
    .C(clk),
    .D(_002_),
    .E(1'h1),
    .Q(_016_),
    .R(1'h1)
  );
  \$lut  #(
    .LUT(64'h7fffffff80000000),
    .WIDTH(32'h00000006)
  ) _106_ (
    .A(\fifo.write_pointer [5:0]),
    .Y(\fifo.next_write_pointer [5])
  );
  \$lut  #(
    .LUT(64'h6ff6ffffffff6ff6),
    .WIDTH(32'h00000006)
  ) _107_ (
    .A({ \fifo.read_pointer [4], \fifo.write_pointer [4], \fifo.read_pointer [3], \fifo.write_pointer [3], \fifo.read_pointer [0], \fifo.write_pointer [0] }),
    .Y(_025_)
  );
  \$lut  #(
    .LUT(4'h9),
    .WIDTH(32'h00000002)
  ) _108_ (
    .A({ \fifo.read_pointer [5], \fifo.write_pointer [5] }),
    .Y(_026_)
  );
  \$lut  #(
    .LUT(64'h6ff6ffffffff6ff6),
    .WIDTH(32'h00000006)
  ) _109_ (
    .A({ \fifo.write_pointer [7], \fifo.read_pointer [7:6], \fifo.write_pointer [6], \fifo.read_pointer [5], \fifo.write_pointer [5] }),
    .Y(_027_)
  );
  \$lut  #(
    .LUT(64'h1001000000001001),
    .WIDTH(32'h00000006)
  ) _110_ (
    .A({ \fifo.write_pointer [2], \fifo.read_pointer [2], \fifo.write_pointer [1], \fifo.read_pointer [1], _027_, _025_ }),
    .Y(\fifo.empty )
  );
  \$lut  #(
    .LUT(8'h78),
    .WIDTH(32'h00000003)
  ) _111_ (
    .A(\fifo.write_pointer [2:0]),
    .Y(\fifo.next_write_pointer [2])
  );
  \$lut  #(
    .LUT(32'h80000000),
    .WIDTH(32'h00000005)
  ) _112_ (
    .A({ \fifo.read_pointer [2:1], \fifo.read_pointer [4:3], \fifo.read_pointer [0] }),
    .Y(_028_)
  );
  \$lut  #(
    .LUT(64'h7fffffff80000000),
    .WIDTH(32'h00000006)
  ) _113_ (
    .A({ \fifo.read_pointer [5], \fifo.read_pointer [2:1], \fifo.read_pointer [4:3], \fifo.read_pointer [0] }),
    .Y(_019_)
  );
  \$lut  #(
    .LUT(16'h7f80),
    .WIDTH(32'h00000004)
  ) _114_ (
    .A(\fifo.write_pointer [3:0]),
    .Y(\fifo.next_write_pointer [3])
  );
  \$lut  #(
    .LUT(64'h3efdff3efffffdff),
    .WIDTH(32'h00000006)
  ) _115_ (
    .A({ _026_, \fifo.read_pointer [4], \fifo.write_pointer [4], \fifo.next_write_pointer [3], \fifo.read_pointer [3], \fifo.write_pointer [3] }),
    .Y(_029_)
  );
  \$lut  #(
    .LUT(64'hbeffffbedff7f7df),
    .WIDTH(32'h00000006)
  ) _116_ (
    .A({ \fifo.read_pointer [0], \fifo.read_pointer [2], \fifo.write_pointer [2], \fifo.read_pointer [1], \fifo.write_pointer [1:0] }),
    .Y(_030_)
  );
  \$lut  #(
    .LUT(64'h8000000000000000),
    .WIDTH(32'h00000006)
  ) _117_ (
    .A({ \fifo.write_pointer [4:1], \fifo.write_pointer [5], \fifo.write_pointer [0] }),
    .Y(_031_)
  );
  \$lut  #(
    .LUT(64'hefbefbfffbffefbe),
    .WIDTH(32'h00000006)
  ) _118_ (
    .A({ \fifo.write_pointer [7], \fifo.read_pointer [7], _031_, \fifo.read_pointer [6], \fifo.write_pointer [6], _030_ }),
    .Y(_032_)
  );
  \$lut  #(
    .LUT(4'h1),
    .WIDTH(32'h00000002)
  ) _119_ (
    .A({ _032_, _029_ }),
    .Y(\fifo.full )
  );
  \$lut  #(
    .LUT(8'he0),
    .WIDTH(32'h00000003)
  ) _120_ (
    .A({ \fifo.write_enable , _029_, _032_ }),
    .Y(_001_)
  );
  \$lut  #(
    .LUT(4'h1),
    .WIDTH(32'h00000002)
  ) _121_ (
    .A({ \fifo.empty , \fifo.write_enable  }),
    .Y(_003_)
  );
  \$lut  #(
    .LUT(4'h6),
    .WIDTH(32'h00000002)
  ) _122_ (
    .A(\fifo.write_pointer [1:0]),
    .Y(\fifo.next_write_pointer [1])
  );
  \$lut  #(
    .LUT(16'h7f80),
    .WIDTH(32'h00000004)
  ) _123_ (
    .A(\fifo.read_pointer [3:0]),
    .Y(_020_)
  );
  \$lut  #(
    .LUT(4'h6),
    .WIDTH(32'h00000002)
  ) _124_ (
    .A({ _031_, \fifo.write_pointer [6] }),
    .Y(\fifo.next_write_pointer [6])
  );
  \$lut  #(
    .LUT(16'h0e00),
    .WIDTH(32'h00000004)
  ) _125_ (
    .A({ \fifo.write_enable , reset, _029_, _032_ }),
    .Y(_000_)
  );
  \$lut  #(
    .LUT(8'h78),
    .WIDTH(32'h00000003)
  ) _126_ (
    .A(\fifo.read_pointer [2:0]),
    .Y(_021_)
  );
  \$lut  #(
    .LUT(8'h78),
    .WIDTH(32'h00000003)
  ) _127_ (
    .A({ \fifo.read_pointer [6], _028_, \fifo.read_pointer [5] }),
    .Y(_017_)
  );
  \$lut  #(
    .LUT(4'h6),
    .WIDTH(32'h00000002)
  ) _128_ (
    .A(\fifo.read_pointer [1:0]),
    .Y(_022_)
  );
  \$lut  #(
    .LUT(16'h7f80),
    .WIDTH(32'h00000004)
  ) _129_ (
    .A({ \fifo.read_pointer [7], _028_, \fifo.read_pointer [6:5] }),
    .Y(_018_)
  );
  \$lut  #(
    .LUT(32'h7fff8000),
    .WIDTH(32'h00000005)
  ) _130_ (
    .A({ \fifo.read_pointer [4], \fifo.read_pointer [2:1], \fifo.read_pointer [3], \fifo.read_pointer [0] }),
    .Y(_024_)
  );
  \$lut  #(
    .LUT(8'h78),
    .WIDTH(32'h00000003)
  ) _131_ (
    .A({ \fifo.write_pointer [7], _031_, \fifo.write_pointer [6] }),
    .Y(\fifo.next_write_pointer [7])
  );
  \$lut  #(
    .LUT(8'hac),
    .WIDTH(32'h00000003)
  ) _132_ (
    .A({ _016_, _015_, _013_ }),
    .Y(\fifo.data_out [1])
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'h00000003)
  ) _133_ (
    .A({ _016_, _012_, _014_ }),
    .Y(\fifo.data_out [0])
  );
  \$lut  #(
    .LUT(8'h01),
    .WIDTH(32'h00000003)
  ) _134_ (
    .A({ \fifo.empty , \fifo.write_enable , reset }),
    .Y(_002_)
  );
  \$lut  #(
    .LUT(32'h7fff8000),
    .WIDTH(32'h00000005)
  ) _135_ (
    .A(\fifo.write_pointer [4:0]),
    .Y(\fifo.next_write_pointer [4])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'h00000001)
  ) _136_ (
    .A(reset),
    .Y(_033_)
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'h00000001)
  ) _137_ (
    .A(\fifo.write_pointer [0]),
    .Y(\fifo.next_write_pointer [0])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'h00000001)
  ) _138_ (
    .A(\fifo.read_pointer [0]),
    .Y(_023_)
  );
  (* module_not_derived = 32'h00000001 *)
  (* src = "/nfs_eda_sw/softwares/Raptor/instl_dir/06_23_2023_09_15_01/bin/../share/yosys/rapidsilicon/genesis3/brams_map_new.v:379.4-390.3|/nfs_eda_sw/softwares/Raptor/instl_dir/06_23_2023_09_15_01/bin/../share/yosys/rapidsilicon/genesis3/brams_final_map_new.v:6619.3-6654.3" *)
  RS_TDP36K #(
    .INIT_i(36864'hxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx),
    .MODE_BITS(81'h06db05005005b68280501)
  ) \fifo.memory.0.0  (
    .ADDR_A1({ 6'h00, \fifo.read_pointer [7:0], 1'h0 }),
    .ADDR_A2(14'hxxxx),
    .ADDR_B1({ 6'h00, \fifo.write_pointer [7:0], 1'h0 }),
    .ADDR_B2(14'hxxxx),
    .BE_A1(2'h0),
    .BE_A2(2'h0),
    .BE_B1({ 1'h0, _000_ }),
    .BE_B2(2'hx),
    .CLK_A1(clk),
    .CLK_A2(1'hx),
    .CLK_B1(clk),
    .CLK_B2(1'hx),
    .FLUSH1(1'h0),
    .FLUSH2(1'h0),
    .RDATA_A1({ _083_[17:2], _013_, _012_ }),
    .RDATA_A2({ _084_[17:1], _082_ }),
    .RDATA_B1(_085_),
    .RDATA_B2(_086_),
    .REN_A1(_002_),
    .REN_A2(1'hx),
    .REN_B1(1'h0),
    .REN_B2(1'h0),
    .WDATA_A1(18'h3ffff),
    .WDATA_A2(18'h3ffff),
    .WDATA_B1({ 16'hxxxx, \fifo.data_in  }),
    .WDATA_B2(18'hxxxxx),
    .WEN_A1(1'h0),
    .WEN_A2(1'h0),
    .WEN_B1(_000_),
    .WEN_B2(1'hx)
  );
  I_BUF #(
    .DELAY(1'h0),
    .PULL_UP_DOWN("NONE"),
    .REG_EN("FALSE"),
    .SLEW_RATE("FAST")
  ) ibuf_inst (
    .C(clk),
    .I(write_enable),
    .O(\fifo.write_enable )
  );
  I_DDR #(
    .DELAY(1'h0),
    .SLEW_RATE("FAST")
  ) iddr_inst (
    .C(clk),
    .D(data_in),
    .DLY_ADJ(1'h0),
    .DLY_INC(1'h1),
    .DLY_LD(1'h1),
    .Q(\fifo.data_in ),
    .R(reset)
  );
  O_BUF #(
    .DELAY(1'h0),
    .PULL_UP_DOWN("NONE"),
    .REG_EN("FALSE"),
    .SLEW_RATE("FAST")
  ) obuf_inst (
    .C(clk),
    .I(\fifo.empty ),
    .O(empty)
  );
  O_BUF #(
    .DELAY(1'h0),
    .PULL_UP_DOWN("NONE"),
    .REG_EN("FALSE"),
    .SLEW_RATE("FAST")
  ) obuf_inst1 (
    .C(clk),
    .I(\fifo.full ),
    .O(full)
  );
  O_DDR #(
    .DELAY(1'h0),
    .SLEW_RATE("FAST")
  ) oddr_inst (
    .C(clk),
    .D(\fifo.data_out ),
    .DLY_ADJ(1'h0),
    .DLY_INC(1'h0),
    .DLY_LD(1'h1),
    .Q(data_out),
    .R(reset)
  );
  assign _004_ = \fifo.write_pointer [2];
  assign _005_ = \fifo.write_pointer [3];
  assign _006_ = \fifo.write_pointer [4];
  assign _007_ = \fifo.write_pointer [1];
  assign _008_ = \fifo.write_pointer [5];
  assign _009_ = \fifo.write_pointer [6];
  assign _010_ = \fifo.write_pointer [7];
  assign _011_ = \fifo.write_pointer [0];
  assign _034_ = \fifo.write_pointer [0];
  assign _035_ = \fifo.write_pointer [1];
  assign _036_ = \fifo.write_pointer [2];
  assign _037_ = \fifo.write_pointer [3];
  assign _038_ = \fifo.write_pointer [4];
  assign _039_ = \fifo.write_pointer [5];
  assign _040_ = \fifo.write_pointer [6];
  assign _041_ = \fifo.write_pointer [7];
  assign _042_ = \fifo.read_pointer [0];
  assign _043_ = \fifo.read_pointer [1];
  assign _044_ = \fifo.read_pointer [2];
  assign _045_ = \fifo.read_pointer [3];
  assign _046_ = \fifo.read_pointer [4];
  assign _047_ = \fifo.read_pointer [5];
  assign _048_ = \fifo.read_pointer [6];
  assign _049_ = \fifo.read_pointer [7];
  assign _050_ = _014_;
  assign _051_ = _015_;
  assign _052_ = _016_;
  assign _053_ = \fifo.write_pointer [0];
  assign _054_ = \fifo.write_pointer [1];
  assign _055_ = \fifo.write_pointer [2];
  assign _056_ = \fifo.write_pointer [3];
  assign _057_ = \fifo.write_pointer [4];
  assign _058_ = \fifo.write_pointer [5];
  assign _059_ = \fifo.write_pointer [6];
  assign _060_ = \fifo.write_pointer [7];
  assign _061_ = \fifo.read_pointer [7];
  assign _062_ = \fifo.read_pointer [5];
  assign _063_ = \fifo.read_pointer [3];
  assign _064_ = \fifo.read_pointer [6];
  assign _065_ = \fifo.read_pointer [4];
  assign _066_ = \fifo.read_pointer [0];
  assign _067_ = \fifo.read_pointer [1];
  assign _068_ = \fifo.read_pointer [2];
  assign _069_ = _014_;
  assign _070_ = _015_;
  assign _071_ = _016_;
  assign _072_ = \fifo.read_pointer [6];
  assign _073_ = \fifo.read_pointer [7];
  assign _074_ = \fifo.read_pointer [5];
  assign _075_ = \fifo.read_pointer [3];
  assign _076_ = \fifo.read_pointer [2];
  assign _077_ = \fifo.read_pointer [1];
  assign _078_ = \fifo.read_pointer [0];
  assign _079_ = \fifo.read_pointer [4];
  assign _080_ = { _015_, _014_ };
  assign _081_ = _016_;
  assign _083_[1:0] = { _013_, _012_ };
  assign _084_[0] = _082_;
  assign \fifo.read_pointer [8] = 1'h0;
  assign \fifo.write_pointer [8] = 1'h0;
endmodule
