/* 
 *  dump_tcl.i
 */
%module dump
%include "std_string.i"
%include "std_vector.i"
%include "std_map.i"
%include "std_pair.i"

%{
  #include "dump_tcl.h"
%}

%include "dump_tcl.h"

//namespace std {
//    %template(portInfo) std::vector<std::string>;
//%template(portsInfo) std::vector<std::vector<std::string>>;
//}