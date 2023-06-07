#ifndef settlement_h
#define settlement_h

#include <stdio.h>

int save_settlement(void);
void print_settlement(char date[]);
int read_settlement_file(char date_input[]);
int read_settlement_line(FILE* fp, char date_input[]);
int read_settlement_product_line(FILE* fp);

#endif /* settlement_h */
