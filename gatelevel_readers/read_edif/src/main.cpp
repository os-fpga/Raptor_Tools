/*
 * main.cpp
 *
 *  Created on: Jul 29, 2022
 *      Author: rjafari
 */
#include "edif_blif.h"

int main(int argc, char *argv[]) {

  printf("Open the file stream\n");

    FILE* fp = fopen(argv[2], "w");

  edif_bilf(argv[1], fp);

  return 0;
}