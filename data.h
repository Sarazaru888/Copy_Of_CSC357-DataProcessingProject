#ifndef DATA_H
#define DATA_H

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <ctype.h>
#include <inttypes.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <arpa/inet.h>
#include <fcntl.h>

struct demo_node {
    char* County;
    char State[3];
    float Education_Bachelors;
    float Education_HighSchool;
    /*  0 == Ethnicities.American Indian and Alaska Native Alone
        1 == Ethnicities.Asian Alone
        2 == Ethnicities.Black Alone
        3 == Ethnicities.Hispanic or Latino
        4 == Ethnicities.Native Hawaiian and Other Pacific Islander Alone
        5 == Ethnicities.Two or More Races
        6 == Ethnicities.White Alone
        7 == Ethnicities.White Alone not Hispanic or Latino
    */
    float Ethnicities[8];
    /*  0 == Income.Median Household Income
        1 == Income.Per Capita Income
        2 == Income.Persons Below Poverty Level
    */
    int Income[3];
    int Population2014; 
};

void validate_args(int argc, char* argv[], FILE** datafile, FILE** commandsfile);
void *checked_malloc(size_t size);
struct demo_node* createDemoNode();
void destroyDemoNode(struct demo_node* node);
void processData(FILE* file, struct demo_node*** theArray, int* sizeOfArray, int* arrayIndex);
int processDemoNode(struct demo_node *node, char *line, const int fieldArray[]);
void printData(struct demo_node *node);
void processCommands(FILE* file, struct demo_node ***theArray,int *sizeOfArray,int *arrayIndex);
void display(struct demo_node **theArray, int length);
int population_total(struct demo_node **theArray, int length);
void filter_state(struct demo_node ***theArray, int* arrayIndex, int* size, const char* stateAbrev);
float population_field(struct demo_node **theArray, int length, int whichField);
void filter(struct demo_node ***theArray, int* arrayIndex, int* size, int lessThan,int whichField,float num);

#endif