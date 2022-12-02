#include "edif_blif.hpp"

int main(int argc, char *argv[])
{

if (argc == 3)
  {
    FILE *outfile = fopen(argv[2], "w");
    const char* infile=argv[1];
    edif_blif(infile,outfile);
    return 0;
  }
  else{
    std::cout<<"Please provide input edif  and name of blif output file e.g. and.blif"<<std::endl;
  }
}
