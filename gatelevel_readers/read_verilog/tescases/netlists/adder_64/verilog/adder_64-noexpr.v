/* Generated by Yosys 0.17+76 (git sha1 035496b50, gcc 9.1.0 -fPIC -Os) */

(* dynports =  1  *)
(* top =  1  *)
(* src = "./rtl/adder_64/adder_64.v:3.1-12.10" *)
module adder_64(cout, sum, a, b, cin);
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:43.7-43.9" *)
  wire _000_;
  wire _001_;
  wire _002_;
  wire _003_;
  wire _004_;
  wire _005_;
  wire _006_;
  wire _007_;
  wire _008_;
  wire _009_;
  wire _010_;
  wire _011_;
  wire _012_;
  wire _013_;
  wire _014_;
  wire _015_;
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
  wire _033_;
  wire _034_;
  wire _035_;
  wire _036_;
  wire _037_;
  wire _038_;
  wire _039_;
  wire _040_;
  wire _041_;
  wire _042_;
  wire _043_;
  wire _044_;
  wire _045_;
  wire _046_;
  wire _047_;
  wire _048_;
  wire _049_;
  wire _050_;
  wire _051_;
  wire _052_;
  wire _053_;
  wire _054_;
  wire _055_;
  wire _056_;
  wire _057_;
  wire _058_;
  wire _059_;
  wire _060_;
  wire _061_;
  wire _062_;
  wire _063_;
  wire _064_;
  wire _065_;
  wire _066_;
  wire _067_;
  wire _068_;
  wire _069_;
  (* force_downto = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:47.19-47.20" *)
  wire [65:0] _070_;
  (* force_downto = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:49.21-49.22" *)
  (* unused_bits = "63" *)
  wire [64:0] _071_;
  (* src = "./rtl/adder_64/adder_64.v:8.18-8.19" *)
  input [63:0] a;
  wire [63:0] a;
  (* src = "./rtl/adder_64/adder_64.v:8.21-8.22" *)
  input [63:0] b;
  wire [63:0] b;
  (* src = "./rtl/adder_64/adder_64.v:7.7-7.10" *)
  input cin;
  wire cin;
  (* src = "./rtl/adder_64/adder_64.v:5.8-5.12" *)
  output cout;
  wire cout;
  (* src = "./rtl/adder_64/adder_64.v:6.19-6.22" *)
  output [63:0] sum;
  wire [63:0] sum;
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _072_ (
    .A(a[21]),
    .Y(_069_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _073_ (
    .A({ b[21], _069_, a[21] }),
    .Y(_071_[21])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _074_ (
    .A(a[22]),
    .Y(_004_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _075_ (
    .A({ b[22], _004_, a[22] }),
    .Y(_071_[22])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _076_ (
    .A(a[23]),
    .Y(_005_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _077_ (
    .A({ b[23], _005_, a[23] }),
    .Y(_071_[23])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _078_ (
    .A(a[24]),
    .Y(_006_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _079_ (
    .A({ b[24], _006_, a[24] }),
    .Y(_071_[24])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _080_ (
    .A(a[25]),
    .Y(_007_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _081_ (
    .A({ b[25], _007_, a[25] }),
    .Y(_071_[25])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _082_ (
    .A(a[26]),
    .Y(_008_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _083_ (
    .A({ b[26], _008_, a[26] }),
    .Y(_071_[26])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _084_ (
    .A(a[27]),
    .Y(_009_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _085_ (
    .A({ b[27], _009_, a[27] }),
    .Y(_071_[27])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _086_ (
    .A(a[28]),
    .Y(_010_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _087_ (
    .A({ b[28], _010_, a[28] }),
    .Y(_071_[28])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _088_ (
    .A(a[29]),
    .Y(_011_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _089_ (
    .A({ b[29], _011_, a[29] }),
    .Y(_071_[29])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _090_ (
    .A(a[30]),
    .Y(_012_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _091_ (
    .A({ b[30], _012_, a[30] }),
    .Y(_071_[30])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _092_ (
    .A(a[31]),
    .Y(_013_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _093_ (
    .A({ b[31], _013_, a[31] }),
    .Y(_071_[31])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _094_ (
    .A(a[32]),
    .Y(_014_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _095_ (
    .A({ b[32], _014_, a[32] }),
    .Y(_071_[32])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _096_ (
    .A(a[33]),
    .Y(_015_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _097_ (
    .A({ b[33], _015_, a[33] }),
    .Y(_071_[33])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _098_ (
    .A(a[34]),
    .Y(_016_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _099_ (
    .A({ b[34], _016_, a[34] }),
    .Y(_071_[34])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _100_ (
    .A(a[35]),
    .Y(_017_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _101_ (
    .A({ b[35], _017_, a[35] }),
    .Y(_071_[35])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _102_ (
    .A(a[36]),
    .Y(_018_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _103_ (
    .A({ b[36], _018_, a[36] }),
    .Y(_071_[36])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _104_ (
    .A(a[37]),
    .Y(_019_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _105_ (
    .A({ b[37], _019_, a[37] }),
    .Y(_071_[37])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _106_ (
    .A(a[38]),
    .Y(_020_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _107_ (
    .A({ b[38], _020_, a[38] }),
    .Y(_071_[38])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _108_ (
    .A(a[39]),
    .Y(_021_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _109_ (
    .A({ b[39], _021_, a[39] }),
    .Y(_071_[39])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _110_ (
    .A(a[40]),
    .Y(_022_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _111_ (
    .A({ b[40], _022_, a[40] }),
    .Y(_071_[40])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _112_ (
    .A(a[41]),
    .Y(_023_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _113_ (
    .A({ b[41], _023_, a[41] }),
    .Y(_071_[41])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _114_ (
    .A(a[42]),
    .Y(_024_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _115_ (
    .A({ b[42], _024_, a[42] }),
    .Y(_071_[42])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _116_ (
    .A(a[43]),
    .Y(_025_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _117_ (
    .A({ b[43], _025_, a[43] }),
    .Y(_071_[43])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _118_ (
    .A(a[44]),
    .Y(_026_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _119_ (
    .A({ b[44], _026_, a[44] }),
    .Y(_071_[44])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _120_ (
    .A(a[45]),
    .Y(_027_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _121_ (
    .A({ b[45], _027_, a[45] }),
    .Y(_071_[45])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _122_ (
    .A(a[46]),
    .Y(_028_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _123_ (
    .A({ b[46], _028_, a[46] }),
    .Y(_071_[46])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _124_ (
    .A(a[47]),
    .Y(_029_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _125_ (
    .A({ b[47], _029_, a[47] }),
    .Y(_071_[47])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _126_ (
    .A(a[48]),
    .Y(_030_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _127_ (
    .A({ b[48], _030_, a[48] }),
    .Y(_071_[48])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _128_ (
    .A(a[49]),
    .Y(_031_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _129_ (
    .A({ b[49], _031_, a[49] }),
    .Y(_071_[49])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _130_ (
    .A(a[50]),
    .Y(_032_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _131_ (
    .A({ b[50], _032_, a[50] }),
    .Y(_071_[50])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _132_ (
    .A(a[51]),
    .Y(_033_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _133_ (
    .A({ b[51], _033_, a[51] }),
    .Y(_071_[51])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _134_ (
    .A(a[52]),
    .Y(_034_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _135_ (
    .A({ b[52], _034_, a[52] }),
    .Y(_071_[52])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _136_ (
    .A(a[53]),
    .Y(_035_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _137_ (
    .A({ b[53], _035_, a[53] }),
    .Y(_071_[53])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _138_ (
    .A(a[54]),
    .Y(_036_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _139_ (
    .A({ b[54], _036_, a[54] }),
    .Y(_071_[54])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _140_ (
    .A(a[55]),
    .Y(_037_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _141_ (
    .A({ b[55], _037_, a[55] }),
    .Y(_071_[55])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _142_ (
    .A(a[56]),
    .Y(_038_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _143_ (
    .A({ b[56], _038_, a[56] }),
    .Y(_071_[56])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _144_ (
    .A(a[57]),
    .Y(_039_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _145_ (
    .A({ b[57], _039_, a[57] }),
    .Y(_071_[57])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _146_ (
    .A(a[58]),
    .Y(_040_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _147_ (
    .A({ b[58], _040_, a[58] }),
    .Y(_071_[58])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _148_ (
    .A(a[59]),
    .Y(_041_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _149_ (
    .A({ b[59], _041_, a[59] }),
    .Y(_071_[59])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _150_ (
    .A(a[60]),
    .Y(_042_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _151_ (
    .A({ b[60], _042_, a[60] }),
    .Y(_071_[60])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _152_ (
    .A(a[61]),
    .Y(_043_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _153_ (
    .A({ b[61], _043_, a[61] }),
    .Y(_071_[61])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _154_ (
    .A(a[62]),
    .Y(_044_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _155_ (
    .A({ b[62], _044_, a[62] }),
    .Y(_071_[62])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _156_ (
    .A(a[63]),
    .Y(_045_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _157_ (
    .A({ b[63], _045_, 1'h1 }),
    .Y(_001_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _158_ (
    .A({ b[63], _045_, a[63] }),
    .Y(_002_)
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _159_ (
    .A(_000_),
    .Y(_046_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _160_ (
    .A({ _002_, _046_, 1'h1 }),
    .Y(_003_)
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _161_ (
    .A(_001_),
    .Y(_047_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _162_ (
    .A({ _003_, _047_, 1'h1 }),
    .Y(cout)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _163_ (
    .A({ _002_, _046_, _000_ }),
    .Y(sum[63])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _164_ (
    .A(a[0]),
    .Y(_048_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _165_ (
    .A({ b[0], _048_, a[0] }),
    .Y(_071_[0])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _166_ (
    .A(a[1]),
    .Y(_049_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _167_ (
    .A({ b[1], _049_, a[1] }),
    .Y(_071_[1])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _168_ (
    .A(a[2]),
    .Y(_050_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _169_ (
    .A({ b[2], _050_, a[2] }),
    .Y(_071_[2])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _170_ (
    .A(a[3]),
    .Y(_051_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _171_ (
    .A({ b[3], _051_, a[3] }),
    .Y(_071_[3])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _172_ (
    .A(a[4]),
    .Y(_052_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _173_ (
    .A({ b[4], _052_, a[4] }),
    .Y(_071_[4])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _174_ (
    .A(a[5]),
    .Y(_053_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _175_ (
    .A({ b[5], _053_, a[5] }),
    .Y(_071_[5])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _176_ (
    .A(a[6]),
    .Y(_054_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _177_ (
    .A({ b[6], _054_, a[6] }),
    .Y(_071_[6])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _178_ (
    .A(a[7]),
    .Y(_055_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _179_ (
    .A({ b[7], _055_, a[7] }),
    .Y(_071_[7])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _180_ (
    .A(a[8]),
    .Y(_056_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _181_ (
    .A({ b[8], _056_, a[8] }),
    .Y(_071_[8])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _182_ (
    .A(a[9]),
    .Y(_057_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _183_ (
    .A({ b[9], _057_, a[9] }),
    .Y(_071_[9])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _184_ (
    .A(a[10]),
    .Y(_058_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _185_ (
    .A({ b[10], _058_, a[10] }),
    .Y(_071_[10])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _186_ (
    .A(a[11]),
    .Y(_059_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _187_ (
    .A({ b[11], _059_, a[11] }),
    .Y(_071_[11])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _188_ (
    .A(a[12]),
    .Y(_060_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _189_ (
    .A({ b[12], _060_, a[12] }),
    .Y(_071_[12])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _190_ (
    .A(a[13]),
    .Y(_061_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _191_ (
    .A({ b[13], _061_, a[13] }),
    .Y(_071_[13])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _192_ (
    .A(a[14]),
    .Y(_062_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _193_ (
    .A({ b[14], _062_, a[14] }),
    .Y(_071_[14])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _194_ (
    .A(a[15]),
    .Y(_063_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _195_ (
    .A({ b[15], _063_, a[15] }),
    .Y(_071_[15])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _196_ (
    .A(a[16]),
    .Y(_064_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _197_ (
    .A({ b[16], _064_, a[16] }),
    .Y(_071_[16])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _198_ (
    .A(a[17]),
    .Y(_065_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _199_ (
    .A({ b[17], _065_, a[17] }),
    .Y(_071_[17])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _200_ (
    .A(a[18]),
    .Y(_066_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _201_ (
    .A({ b[18], _066_, a[18] }),
    .Y(_071_[18])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _202_ (
    .A(a[19]),
    .Y(_067_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _203_ (
    .A({ b[19], _067_, a[19] }),
    .Y(_071_[19])
  );
  \$lut  #(
    .LUT(2'h1),
    .WIDTH(32'd1)
  ) _204_ (
    .A(a[20]),
    .Y(_068_)
  );
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'd3)
  ) _205_ (
    .A({ b[20], _068_, a[20] }),
    .Y(_071_[20])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:79.19-85.8" *)
  adder_carry _206_ (
    .cin(_070_[63]),
    .g(1'h0),
    .p(1'h0),
    .sumout(_000_)
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _207_ (
    .cin(_070_[0]),
    .cout(_070_[1]),
    .g(a[0]),
    .p(_071_[0]),
    .sumout(sum[0])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _208_ (
    .cin(_070_[10]),
    .cout(_070_[11]),
    .g(a[10]),
    .p(_071_[10]),
    .sumout(sum[10])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _209_ (
    .cin(_070_[11]),
    .cout(_070_[12]),
    .g(a[11]),
    .p(_071_[11]),
    .sumout(sum[11])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _210_ (
    .cin(_070_[12]),
    .cout(_070_[13]),
    .g(a[12]),
    .p(_071_[12]),
    .sumout(sum[12])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _211_ (
    .cin(_070_[13]),
    .cout(_070_[14]),
    .g(a[13]),
    .p(_071_[13]),
    .sumout(sum[13])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _212_ (
    .cin(_070_[14]),
    .cout(_070_[15]),
    .g(a[14]),
    .p(_071_[14]),
    .sumout(sum[14])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _213_ (
    .cin(_070_[15]),
    .cout(_070_[16]),
    .g(a[15]),
    .p(_071_[15]),
    .sumout(sum[15])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _214_ (
    .cin(_070_[16]),
    .cout(_070_[17]),
    .g(a[16]),
    .p(_071_[16]),
    .sumout(sum[16])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _215_ (
    .cin(_070_[17]),
    .cout(_070_[18]),
    .g(a[17]),
    .p(_071_[17]),
    .sumout(sum[17])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _216_ (
    .cin(_070_[18]),
    .cout(_070_[19]),
    .g(a[18]),
    .p(_071_[18]),
    .sumout(sum[18])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _217_ (
    .cin(_070_[19]),
    .cout(_070_[20]),
    .g(a[19]),
    .p(_071_[19]),
    .sumout(sum[19])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _218_ (
    .cin(_070_[1]),
    .cout(_070_[2]),
    .g(a[1]),
    .p(_071_[1]),
    .sumout(sum[1])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _219_ (
    .cin(_070_[20]),
    .cout(_070_[21]),
    .g(a[20]),
    .p(_071_[20]),
    .sumout(sum[20])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _220_ (
    .cin(_070_[21]),
    .cout(_070_[22]),
    .g(a[21]),
    .p(_071_[21]),
    .sumout(sum[21])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _221_ (
    .cin(_070_[22]),
    .cout(_070_[23]),
    .g(a[22]),
    .p(_071_[22]),
    .sumout(sum[22])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _222_ (
    .cin(_070_[23]),
    .cout(_070_[24]),
    .g(a[23]),
    .p(_071_[23]),
    .sumout(sum[23])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _223_ (
    .cin(_070_[24]),
    .cout(_070_[25]),
    .g(a[24]),
    .p(_071_[24]),
    .sumout(sum[24])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _224_ (
    .cin(_070_[25]),
    .cout(_070_[26]),
    .g(a[25]),
    .p(_071_[25]),
    .sumout(sum[25])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _225_ (
    .cin(_070_[26]),
    .cout(_070_[27]),
    .g(a[26]),
    .p(_071_[26]),
    .sumout(sum[26])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _226_ (
    .cin(_070_[27]),
    .cout(_070_[28]),
    .g(a[27]),
    .p(_071_[27]),
    .sumout(sum[27])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _227_ (
    .cin(_070_[28]),
    .cout(_070_[29]),
    .g(a[28]),
    .p(_071_[28]),
    .sumout(sum[28])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _228_ (
    .cin(_070_[29]),
    .cout(_070_[30]),
    .g(a[29]),
    .p(_071_[29]),
    .sumout(sum[29])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _229_ (
    .cin(_070_[2]),
    .cout(_070_[3]),
    .g(a[2]),
    .p(_071_[2]),
    .sumout(sum[2])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _230_ (
    .cin(_070_[30]),
    .cout(_070_[31]),
    .g(a[30]),
    .p(_071_[30]),
    .sumout(sum[30])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _231_ (
    .cin(_070_[31]),
    .cout(_070_[32]),
    .g(a[31]),
    .p(_071_[31]),
    .sumout(sum[31])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _232_ (
    .cin(_070_[32]),
    .cout(_070_[33]),
    .g(a[32]),
    .p(_071_[32]),
    .sumout(sum[32])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _233_ (
    .cin(_070_[33]),
    .cout(_070_[34]),
    .g(a[33]),
    .p(_071_[33]),
    .sumout(sum[33])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _234_ (
    .cin(_070_[34]),
    .cout(_070_[35]),
    .g(a[34]),
    .p(_071_[34]),
    .sumout(sum[34])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _235_ (
    .cin(_070_[35]),
    .cout(_070_[36]),
    .g(a[35]),
    .p(_071_[35]),
    .sumout(sum[35])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _236_ (
    .cin(_070_[36]),
    .cout(_070_[37]),
    .g(a[36]),
    .p(_071_[36]),
    .sumout(sum[36])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _237_ (
    .cin(_070_[37]),
    .cout(_070_[38]),
    .g(a[37]),
    .p(_071_[37]),
    .sumout(sum[37])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _238_ (
    .cin(_070_[38]),
    .cout(_070_[39]),
    .g(a[38]),
    .p(_071_[38]),
    .sumout(sum[38])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _239_ (
    .cin(_070_[39]),
    .cout(_070_[40]),
    .g(a[39]),
    .p(_071_[39]),
    .sumout(sum[39])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _240_ (
    .cin(_070_[3]),
    .cout(_070_[4]),
    .g(a[3]),
    .p(_071_[3]),
    .sumout(sum[3])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _241_ (
    .cin(_070_[40]),
    .cout(_070_[41]),
    .g(a[40]),
    .p(_071_[40]),
    .sumout(sum[40])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _242_ (
    .cin(_070_[41]),
    .cout(_070_[42]),
    .g(a[41]),
    .p(_071_[41]),
    .sumout(sum[41])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _243_ (
    .cin(_070_[42]),
    .cout(_070_[43]),
    .g(a[42]),
    .p(_071_[42]),
    .sumout(sum[42])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _244_ (
    .cin(_070_[43]),
    .cout(_070_[44]),
    .g(a[43]),
    .p(_071_[43]),
    .sumout(sum[43])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _245_ (
    .cin(_070_[44]),
    .cout(_070_[45]),
    .g(a[44]),
    .p(_071_[44]),
    .sumout(sum[44])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _246_ (
    .cin(_070_[45]),
    .cout(_070_[46]),
    .g(a[45]),
    .p(_071_[45]),
    .sumout(sum[45])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _247_ (
    .cin(_070_[46]),
    .cout(_070_[47]),
    .g(a[46]),
    .p(_071_[46]),
    .sumout(sum[46])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _248_ (
    .cin(_070_[47]),
    .cout(_070_[48]),
    .g(a[47]),
    .p(_071_[47]),
    .sumout(sum[47])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _249_ (
    .cin(_070_[48]),
    .cout(_070_[49]),
    .g(a[48]),
    .p(_071_[48]),
    .sumout(sum[48])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _250_ (
    .cin(_070_[49]),
    .cout(_070_[50]),
    .g(a[49]),
    .p(_071_[49]),
    .sumout(sum[49])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _251_ (
    .cin(_070_[4]),
    .cout(_070_[5]),
    .g(a[4]),
    .p(_071_[4]),
    .sumout(sum[4])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _252_ (
    .cin(_070_[50]),
    .cout(_070_[51]),
    .g(a[50]),
    .p(_071_[50]),
    .sumout(sum[50])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _253_ (
    .cin(_070_[51]),
    .cout(_070_[52]),
    .g(a[51]),
    .p(_071_[51]),
    .sumout(sum[51])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _254_ (
    .cin(_070_[52]),
    .cout(_070_[53]),
    .g(a[52]),
    .p(_071_[52]),
    .sumout(sum[52])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _255_ (
    .cin(_070_[53]),
    .cout(_070_[54]),
    .g(a[53]),
    .p(_071_[53]),
    .sumout(sum[53])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _256_ (
    .cin(_070_[54]),
    .cout(_070_[55]),
    .g(a[54]),
    .p(_071_[54]),
    .sumout(sum[54])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _257_ (
    .cin(_070_[55]),
    .cout(_070_[56]),
    .g(a[55]),
    .p(_071_[55]),
    .sumout(sum[55])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _258_ (
    .cin(_070_[56]),
    .cout(_070_[57]),
    .g(a[56]),
    .p(_071_[56]),
    .sumout(sum[56])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _259_ (
    .cin(_070_[57]),
    .cout(_070_[58]),
    .g(a[57]),
    .p(_071_[57]),
    .sumout(sum[57])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _260_ (
    .cin(_070_[58]),
    .cout(_070_[59]),
    .g(a[58]),
    .p(_071_[58]),
    .sumout(sum[58])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _261_ (
    .cin(_070_[59]),
    .cout(_070_[60]),
    .g(a[59]),
    .p(_071_[59]),
    .sumout(sum[59])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _262_ (
    .cin(_070_[5]),
    .cout(_070_[6]),
    .g(a[5]),
    .p(_071_[5]),
    .sumout(sum[5])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _263_ (
    .cin(_070_[60]),
    .cout(_070_[61]),
    .g(a[60]),
    .p(_071_[60]),
    .sumout(sum[60])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _264_ (
    .cin(_070_[61]),
    .cout(_070_[62]),
    .g(a[61]),
    .p(_071_[61]),
    .sumout(sum[61])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _265_ (
    .cin(_070_[62]),
    .cout(_070_[63]),
    .g(a[62]),
    .p(_071_[62]),
    .sumout(sum[62])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _266_ (
    .cin(_070_[6]),
    .cout(_070_[7]),
    .g(a[6]),
    .p(_071_[6]),
    .sumout(sum[6])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _267_ (
    .cin(_070_[7]),
    .cout(_070_[8]),
    .g(a[7]),
    .p(_071_[7]),
    .sumout(sum[7])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _268_ (
    .cin(_070_[8]),
    .cout(_070_[9]),
    .g(a[8]),
    .p(_071_[8]),
    .sumout(sum[8])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:66.16-72.4" *)
  adder_carry _269_ (
    .cin(_070_[9]),
    .cout(_070_[10]),
    .g(a[9]),
    .p(_071_[9]),
    .sumout(sum[9])
  );
  (* module_not_derived = 32'd1 *)
  (* src = "./rtl/adder_64/adder_64.v:10.22-10.33|/home/users/eda_tools/opensource/yosys_verifc/yosys_verific_rs/yosys/install/bin/../share/yosys/rapidsilicon/genesis/arith_map.v:54.19-60.8" *)
  adder_carry _270_ (
    .cout(_070_[0]),
    .g(cin),
    .p(1'h0)
  );
  assign _070_[65:64] = { 1'h0, cout };
  assign _071_[64] = 1'h0;
endmodule