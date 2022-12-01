#include "edif_blif.hpp"

int main(int argc, char *argv[])
{

if (argc == 3)
  {
    FILE *infile = fopen(argv[1], "r");
    const char* edif_bl=argv[2];
    edif_blif(infile,edif_bl);
    return 0;
  }
  else{
    std::cout<<"Please provide input edif  and name of blif output file e.g. and.blif"<<std::endl;
  }
}
