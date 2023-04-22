#ifndef PRINTER_H
#define PRINTER_H

#include "parser-v1.h"

void print_node(ASTNode *node, int depth);
void print_indent(int depth);

#endif /* PRINTER_H */
