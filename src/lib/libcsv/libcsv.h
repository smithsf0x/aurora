#ifndef LIBCSV_H
#define LIBCSV_H

#include "../include.h"

size_t getline(char **lineptr, size_t *n, FILE *stream);
int* readCSV(const char * path);


#endif