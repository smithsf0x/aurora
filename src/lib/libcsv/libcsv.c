/*
* @Author: smithsf0x
* @Date:   2015-12-27 01:16:10
* @Last Modified by:   smithsf0x
* @Last Modified time: 2015-12-27 01:23:35
*/

#include "libcsv.h"


size_t getline(char **lineptr, size_t *n, FILE *stream) {
    char *bufptr = NULL;
    char *p = bufptr;
    size_t size;
    int c;

    if (lineptr == NULL) {
        return -1;
    }
    if (stream == NULL) {
        return -1;
    }
    if (n == NULL) {
        return -1;
    }
    bufptr = *lineptr;
    size = *n;

    c = fgetc(stream);
    if (c == EOF) {
        return -1;
    }
    if (bufptr == NULL) {
        bufptr = malloc(128);
        if (bufptr == NULL) {
            return -1;
        }
        size = 128;
    }
    p = bufptr;
    while(c != EOF) {
        if ((p - bufptr) > (size - 1)) {
            size = size + 128;
            bufptr = realloc(bufptr, size);
            if (bufptr == NULL) {
                return -1;
            }
        }
        *p++ = c;
        if (c == '\n') {
            break;
        }
        c = fgetc(stream);
    }

    *p++ = '\0';
    *lineptr = bufptr;
    *n = size;

    return p - bufptr - 1;
}

int* readCSV(const char * path){
    #ifdef DEBUG
        printf("--> readCSV(%s)\n", path);
    #endif

    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    int * mapArray = malloc(sizeof(int[1599]));
    int cntr = 0;
    char delimiter[] = ",";
    char *ptr;
    int ret;

    fp = fopen(path, "r");
    if (fp == NULL) return 0;

    while ((read = getline(&line, &len, fp)) != -1) {
        #ifdef DEBUG
            printf("--> %s\n", line);
        #endif
        // initialisieren und ersten Abschnitt erstellen
        ptr = strtok(line, delimiter);

        while(ptr != NULL) {
            ret = strtol(ptr, (char ** )NULL, 10);
            mapArray[cntr] = ret;
            cntr++;
            // naechsten Abschnitt erstellen
            ptr = strtok(NULL, delimiter);
        }
    }
    //fclose(fp);
    sceIoClose(fp);
    if (line) free(line);
    #ifdef DEBUG
        printf("--> readCSV() END // OK\n");
    #endif
    return mapArray;
}

