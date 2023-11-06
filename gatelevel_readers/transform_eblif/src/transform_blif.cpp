/**
 * @file transform_blif.cpp
 * @author manadher Kharroubi (manadher@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-09-25
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "transform_blif.h"

using namespace std;

int main(int argc, char *argv[]) {
  // std::string W, T;
  if (argc < 2 || argc > 3) {
    std::cout << "./tranform_blif <infile> [<outfile>]" << std::endl;
  }
  char *filename = argv[1];
  Eblif_Transformer tr;
  std::ifstream fstr;
  fstr.open(filename);
  std::stringstream ss;
  // Call transform function
  tr.rs_transform_eblif(fstr, ss);
  std::string data = ss.str();
  const char *value = std::getenv("PRINT_TRANSFORMED_EBLIF_FILE");
  if (value) {
    // std::cout << "--- PRINT_TRANSFORMED_EBLIF_FILE: " << value << std::endl;
    std::fstream inner_ofs(value, std::ios::out);
    inner_ofs << ss.str();
    inner_ofs.close();
  } else {
    // std::cout << "--- PRINT_TRANSFORMED_EBLIF_FILE not set." << std::endl;
  }
  // Opening an inmemory file
  FILE *infile = fmemopen((void *)data.c_str(), data.size(), "r");
  FILE *outfile = nullptr;
  if (argc == 3) {
    outfile = fopen(argv[2], "w");
  }
  tr.printFileContents(infile, outfile);
}
