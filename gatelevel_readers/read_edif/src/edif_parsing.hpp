#ifndef EDIF_PARSING
#define EDIF_PARSING
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

enum SNodeType {
  LIST,
  STRING,
  SYMBOL,
  INTEGER,
  FLOAT
};

struct SNode {
  struct SNode *next;
  enum SNodeType type;
  union {
    struct SNode *list;
    char *value;
  };
};

struct SNode *snode_parse(FILE *fp);
void snode_free(struct SNode *node);
bool string_compare(const std::string &f_name, const std::string &ext);

#endif