#include "data.h"

#define MAX_FIELDS 52
#define NUM_FIELDS 16

const char* fieldArray[] = {
    "County",
    "State",
    "Education.Bachelor's Degree or Higher",
    "Education.High School or Higher",
    "Ethnicities.American Indian and Alaska Native Alone",
    "Ethnicities.Asian Alone",
    "Ethnicities.Black Alone",
    "Ethnicities.Hispanic or Latino",
    "Ethnicities.Native Hawaiian and Other Pacific Islander Alone",
    "Ethnicities.Two or More Races",
    "Ethnicities.White Alone",
    "Ethnicities.White Alone not Hispanic or Latino",
    "Income.Median Household Income\"",
    "Income.Per Capita Income",
    "Income.Persons Below Poverty Level",
    "Population.2014 Population"
};

const char* operationsArray[] = {
    "display",
    "filter-state",
    "filter",
    "population-total",
    "population",
    "percent"
};

const int fieldIndex[] = {
    1,
    2,
    6,
    7,
    12,
    13,
    14,
    15,
    16,
    17,
    18,
    19,
    26,
    27,
    28,
    39
};

void validate_args(int argc, char *argv[], FILE **datafile, FILE **commandsfile)
{
    if (argc != 3)
    {
        fprintf(stderr, "wrong number of arguments\n");
        exit(1);
    }

    if ((*datafile = fopen(argv[1], "r")) == NULL)
    {
        fprintf(stderr, "%s doesn't exist\n", argv[1]);
        exit(1);
    }
    if ((*commandsfile = fopen(argv[2], "r")) == NULL)
    {
        fclose(*datafile);
        fprintf(stderr, "%s doesn't exist\n", argv[2]);
        exit(1);
    }
}

void *checked_malloc(size_t size)
{
    void *p;
    p = malloc(size);
    if (p == NULL)
    {
        perror("malloc");
        exit(1);
    }
    return p;
}

struct demo_node *createDemoNode()
{
    struct demo_node *node = checked_malloc(sizeof(struct demo_node));
    node->County = NULL;
    return node;
}

void destroyDemoNode(struct demo_node *node)
{
    free(node->County);
    free(node);
}

/*  1 == County
    2 == State
    6 == Education_Bachelors
    7 == Education_HighSchool
    12 == Ethnicities[0]
    19 == Ethnicities[7]
    26 == Income[0]
    27 == Income[1]
    28 == Income[2]
    39 == Population2014
*/
int processDemoNode(struct demo_node *node, char *line, const int fieldArray[])
{
    int curfields = 0;
    int numfields = 1;
    int curChar = 0;
    int first = 0;

    int firstQuote = 0;
    char *field = NULL;
    while (line[curChar] != '\0')
    {
        if (line[curChar] == '"')
        {
            line[curChar] = '\0';
            if (firstQuote == 0)
            {
                first = curChar + 1;
                firstQuote = 1;
            }
            else
            {
                if (numfields == fieldArray[curfields]){
                    field = strdup(&line[first]);
                    // do stuff for that field
                    if(curfields == 0){
                        node->County = field;
                    }
                    else{
                        if (curfields ==1 ){
                            node->State[0] = field[0];
                            node->State[1] = field[1];
                            if (field[2]!='\0'){
                                return -1;
                            }
                            node->State[2] = '\0';
                        }
                        else if (curfields == 2){
                            node->Education_Bachelors = atof(field);
                        }
                        else if (curfields == 3){
                            node->Education_HighSchool = atof(field);
                        }
                        else if (curfields >=4&&curfields<=11){
                            node->Ethnicities[curfields -4] = atof(field);
                        }
                        else if (curfields >= 12 && curfields <= 14){
                            node->Income[curfields - 12] = atoi(field);
                        }
                        else if (curfields == 15){
                            node->Population2014 = atoi(field);
                        }
                        free(field);
                    }
                    curfields++;
                }
                firstQuote = 0;
            }
        }
        else if (line[curChar] == ',')
        {
            numfields++;
            line[curChar] = '\0';
        }
        curChar++;
    }
    if(numfields != MAX_FIELDS){
        return -1;
    }
    return 0;
}

void printData(struct demo_node *node){
    printf("Demographics for: %s,  %s\n\tEducation.Bachelor's Degree or Higher: %.2f\n\t"  
        "Education.High School or Higher: %.2f\n\tEthnicities.American Indian and Alaska Native Alone: %.2f\n\t"
        "Ethnicities.Asian Alone: %.2f\n\tEthnicities.Black Alone: %.2f\n\tEthnicities.Hispanic or Latino: %.2f\n\t"
        "Ethnicities.Native Hawaiian and Other Pacific Islander Alone: %.2f\n\tEthnicities.Two or More Races: %.2f\n\t"
        "Ethnicities.White Alone: %.2f\n\tEthnicities.White Alone not Hispanic or Latino: %.2f\n\t"
        "Income.Median Household Income: %d\n\tIncome.Per Capita Income: %d\n\t"
        "Income.Persons Below Poverty Level: %d\n\tPopulation.2014 Population: %d\n", 
        node->County, node->State, node->Education_Bachelors, node->Education_HighSchool, node->Ethnicities[0],
        node->Ethnicities[1],node->Ethnicities[2],node->Ethnicities[3],node->Ethnicities[4],node->Ethnicities[5],
        node->Ethnicities[6],node->Ethnicities[7],node->Income[0],node->Income[1],node->Income[2],
        node->Population2014);
            
}

void processData(FILE *file, struct demo_node ***theArray, int *sizeOfArray, int *arrayIndex)
{
    *theArray = checked_malloc(sizeof(struct demo_node *) * 10);
    *sizeOfArray = 10;
    char *line = NULL;
    size_t size;
    int lineNum = 0;

    getline(&line, &size, file);

    while (getline(&line, &size, file) > 0)
    {
        if (*arrayIndex >= *sizeOfArray)
        {
            *theArray = realloc(*theArray, sizeof(struct demo_node *) * *sizeOfArray * 2);
            *sizeOfArray = *sizeOfArray * 2;
        }
        (*theArray)[*arrayIndex] = createDemoNode();
        if (processDemoNode((*theArray)[*arrayIndex], line, fieldIndex) < 0)
        {
            destroyDemoNode((*theArray)[*arrayIndex]);
            fprintf(stderr, "Line number: %d is invalid\n", lineNum + 1);
        }
        else
        {
            //printData((*theArray)[*arrayIndex]);
            *arrayIndex = *arrayIndex + 1;
        }
        lineNum++;
    }
    free(line);
    printf("Number of file entries loaded: %d\n", *arrayIndex);
}

void display(struct demo_node **theArray, int length){
    int i = 0;
    while(i<length){
        printData((theArray)[i]);
        i++;
    }
}

int population_total(struct demo_node **theArray, int length){
    int pop = 0;
    int i = 0;
    while(i<length){
        pop += theArray[i]->Population2014;
        i++;
    }
    return pop;
}

void filter_state(struct demo_node ***theArray, int* arrayIndex, int* size, const char* stateAbrev){
    int i = 0;

    while(i<*arrayIndex){
        if(strcmp((*theArray)[i]->State,stateAbrev)==0){
            i++;//do nothing cuz it fits the filter
        }
        else{
            destroyDemoNode((*theArray)[i]);
            //then update pointer values so that the values "move down" the array
            int j = i+1;
            while(j<*arrayIndex){
                (*theArray)[j-1] = (*theArray)[j];
                j++;
            }
            *arrayIndex -= 1;
        }
    }
}

float population_field(struct demo_node **theArray, int length, int whichField){
    float sub = 0;
    int i = 0;


    while(i<length){
        float value = 0;
        if (whichField==2){
            value = theArray[i]->Education_Bachelors;
        }
        else if (whichField==3){
            value = theArray[i]->Education_HighSchool;
        }
        else if (whichField == 14){
            value = theArray[i]->Income[2];
        }
        else{
            value = theArray[i]->Ethnicities[whichField-4];
        }
        sub+= value * theArray[i]->Population2014 / 100;
        i++;
    }
    return sub;
}

void filter(struct demo_node ***theArray, int* arrayIndex, int* size, int lessThan,int whichField,float num){
    //lessThan == 0 means <= while 1 means >=
    int i = 0;

    while(i<*arrayIndex){
        //get field value
        float value;
        if (whichField==2){
            value = (*theArray)[i]->Education_Bachelors;
        }
        else if (whichField==3){
            value = (*theArray)[i]->Education_HighSchool;
        }
        else if (whichField >=4 && whichField <=11){
            value = (*theArray)[i]->Ethnicities[whichField-4];
        }
        else if(whichField>=12 && whichField <=14){
            value = (*theArray)[i]->Income[whichField-4];
        }
        else if(whichField==15){
            value = (*theArray)[i]->Population2014;
        }
        else{
            fprintf(stderr, "Incorrect field index");
            return;
        }
        //compare field value
        int passed = 0;
        if(lessThan==0){
            if (value <= num){
                passed = 1;
            }
        }
        else if (lessThan==1){
            if(value >= num){
                passed = 1;
            }
        }
        //if it doesn't pass the filter, remove the node
        if(passed==0){
            destroyDemoNode((*theArray)[i]);
            //then update pointer values so that the values "move down" the array
            int j = i+1;
            while(j<*arrayIndex){
                (*theArray)[j-1] = (*theArray)[j];
                j++;
            }
            *arrayIndex -= 1;
        }
        else{
            i++;
        }
    }
}

void processCommands(FILE* file, struct demo_node ***theArray,int *sizeOfArray,int *arrayIndex){
    char *line = NULL;
    size_t size;
    int lineNum = 1;
    int numChar = 0;
    //keeps track of index of each argument in a line
    int argIndexes[4];

    while ((numChar = getline(&line, &size, file)) > 0){
        //printf("Line %d has length: %d\n",lineNum,numChar);
        if(!(numChar==1&&line[0]=='\n')){
            int numArgs = 1;
            int i = 0;
            argIndexes[0] = 0;

            //separate arguments by adding null characters in place of : and \n
            // --> also count how many arguments and set pointers to each argument
            while(line[i]!='\0'){
                if (line[i] == '\n'){
                    line[i] = '\0';
                }
                else if(line[i] == ':'){
                    argIndexes[numArgs] = i+1;
                    numArgs++;
                    line[i] = '\0';
                }
                i++;
            }
            
            //do functions
            
            if (numArgs==1){
                if(strcmp(&line[argIndexes[0]],operationsArray[0])==0){
                    display(*theArray,*arrayIndex);
                }
                else if (strcmp(&line[argIndexes[0]],operationsArray[3])==0){
                    printf("2014 population: %d\n", population_total(*theArray,*arrayIndex));
                }
                else{
                    fprintf(stderr, "Operation in Line: %d is invalid\n",lineNum);
                }
            }
            else if (numArgs==2){
                if(strcmp(&line[argIndexes[0]],operationsArray[1])==0){
                    filter_state(theArray,arrayIndex,sizeOfArray,&line[argIndexes[1]]);
                    printf("Filter: state == %s (%d entries)\n", &line[argIndexes[1]], *arrayIndex);
                }
                else if (strcmp(&line[argIndexes[0]],operationsArray[4])==0){
                    int tempo = 2;
                    while(tempo < NUM_FIELDS-3){
                        if (strcmp(&line[argIndexes[1]],fieldArray[tempo])==0){
                            printf("2014 %s population: %f\n",fieldArray[tempo],population_field(*theArray,*arrayIndex,tempo));
                            tempo = NUM_FIELDS+1;
                        }
                        tempo++;
                    }
                    if(strcmp(&line[argIndexes[1]],fieldArray[14])==0){
                        printf("2014 %s population: %f\n",fieldArray[14],population_field(*theArray,*arrayIndex,14));
                        tempo = NUM_FIELDS+2;
                    }
                    if(tempo != NUM_FIELDS+2){
                        fprintf(stderr,"Invalid field at Line: %d\n", lineNum);
                    }
                }
                else if (strcmp(&line[argIndexes[0]],operationsArray[5])==0){
                    int tempo = 2;
                    while(tempo < NUM_FIELDS-3){
                        if (strcmp(&line[argIndexes[1]],fieldArray[tempo])==0){
                            float sub = population_field(*theArray,*arrayIndex,tempo);
                            float pop = population_total(*theArray,*arrayIndex);
                            printf("2014 %s percentage: %f\n", &line[argIndexes[1]], sub / pop * 100);
                            tempo = NUM_FIELDS+1;
                        }
                        tempo++;
                    }
                    if(strcmp(&line[argIndexes[1]],fieldArray[14])==0){
                        float sub = population_field(*theArray,*arrayIndex,14);
                        float pop = population_total(*theArray,*arrayIndex);
                        printf("2014 %s percentage: %f\n", &line[argIndexes[1]], sub / pop * 100);
                        tempo = NUM_FIELDS+2;
                    }
                    if(tempo != NUM_FIELDS+2){
                        fprintf(stderr,"Invalid field at Line: %d\n", lineNum);
                    }
                }
                else{
                    fprintf(stderr, "Operation in Line: %d is invalid\n",lineNum);
                }
            }
            else if (numArgs==4){
                if(strcmp(&line[argIndexes[0]],operationsArray[2])==0){
                    int tempo = 2;
                    int fieldIndex = 0;
                    int lessThan = -1;
                    while(tempo < NUM_FIELDS){
                        if (strcmp(&line[argIndexes[1]],fieldArray[tempo])==0){
                            fieldIndex = tempo;
                            tempo = NUM_FIELDS+1;
                        }
                        tempo++;
                    }
                    if(strcmp(&line[argIndexes[2]],"ge")==0){
                        lessThan = 1;
                    }
                    else if(strcmp(&line[argIndexes[2]],"le")==0){
                        lessThan = 0;
                    }
                    if(tempo != NUM_FIELDS+2||lessThan==-1){
                        fprintf(stderr,"Invalid field at Line: %d\n", lineNum);
                    }
                    else{
                        float num = atof(&line[argIndexes[3]]);
                        filter(theArray,arrayIndex,sizeOfArray,lessThan,fieldIndex,num);
                        printf("Filter: %s %s %f (%d entries)\n",&line[argIndexes[1]],&line[argIndexes[2]],num,*arrayIndex);
                    }
                }
            }
            else{
                fprintf(stderr, "Operation in Line: %d is invalid\n",lineNum);
            }
        }
        lineNum++;
    }
    free(line);
}

int main(int argc, char *argv[])
{
    FILE *datafile;
    FILE *commandsfile;
    struct demo_node **thearray;
    int size = 0;
    int index = 0;
    
    validate_args(argc, argv, &datafile, &commandsfile);
    processData(datafile, &thearray, &size, &index);
    
    processCommands(commandsfile, &thearray,&size, &index);
    

    //clean up, clean up. everybody do their share!
    int i = 0;
    for (i = 0; i < index; i++)
    {
        destroyDemoNode(thearray[i]);
    }
    free(thearray);
    fclose(datafile);
    fclose(commandsfile);
    return 0;
}