#ifndef product_h
#define product_h

#include <stdio.h>

int read_product_file(void);
int read_product_line(FILE* fp);
int read_table_amounts_line(FILE* fp);

int productcmp(char* name);
void add_product(char* name, int price);
void write_product_file(void);

#endif
