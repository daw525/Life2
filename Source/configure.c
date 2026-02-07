#include "configure.h"
#include "world.h"
#include "region.h"
#include <string.h>

typedef enum {
    NOT_STARTED,
    REGION,
    LAYER,
    MAPPING,
    INPUT,
    INPUT_FROM_LAYER,
    INPUT_FROM_ENTITY,
    INPUT_INVERT,
    TYPE,
    OUTPUT_TO_ENTITY,
    OUTPUT_INVERT,
    ENTITY,
    INTEGRATION_TIME,
    FLIP_TIME,
    FINISHED,
    MAX_CONFIGURATION_STATE
} configurationStateEnum;

#define MAX_TOKEN_LENGTH (20)   /* Calculated based on longest token (don't forget to include null terminator)*/

static char configurationStateTokens[MAX_CONFIGURATION_STATE][MAX_TOKEN_LENGTH] = {
    "NOT_STARTED\0",
    "REGION\0",
    "LAYER\0",
    "MAPPING\0",
    "INPUT\0",
    "INPUT_FROM_LAYER\0",
    "INPUT_FROM_ENTITY\0",
    "INPUT_INVERT\0",
    "TYPE\0",
    "OUTPUT_TO_ENTITY\0",
    "OUTPUT_INVERT\0",
    "ENTITY\0",
    "INTEGRATION_TIME\0",
    "FLIP_TIME\0",
    "FINISHED\0"
};

typedef struct {
    int dataLowerLimit;
    int dataUpperLimit;
    configurationStateEnum nextStates[MAX_CONFIGURATION_STATE];
} configurationState;

static configurationState configurationStates[MAX_CONFIGURATION_STATE] = {
/* dataLowerLimit   dataUpperLimit  nextStates  */
    {0,             0,              {REGION,MAX_CONFIGURATION_STATE}},
    {0,             MAX_REGION,     {LAYER,MAX_CONFIGURATION_STATE}},
    {0,             MAX_LAYER,      {MAPPING,MAX_CONFIGURATION_STATE}},
    {0,             MAX_MAPPING,    {INPUT,MAX_CONFIGURATION_STATE}},
    {0,             MAX_INPUT_PORT, {INPUT_FROM_LAYER,MAX_CONFIGURATION_STATE}},
    {0,             MAX_LAYER,      {INPUT_FROM_ENTITY,MAX_CONFIGURATION_STATE}},
    {0,             MAX_ENTITY,     {INPUT_INVERT,MAX_CONFIGURATION_STATE}},
    {0,             1,              {INPUT,TYPE,MAX_CONFIGURATION_STATE}},
    {AND,           THRESHOLD,      {OUTPUT_TO_ENTITY,MAX_CONFIGURATION_STATE}},
    {0,             MAX_ENTITY,     {OUTPUT_INVERT,MAX_CONFIGURATION_STATE}},
    {0,             1,              {ENTITY,MAX_CONFIGURATION_STATE}},
    {0,             MAX_ENTITY,     {INTEGRATION_TIME,MAX_CONFIGURATION_STATE}},
    {0,             MAX_SAMPLE,     {FLIP_TIME,MAX_CONFIGURATION_STATE}},
    {0,             MAX_FLIP_TIME,  {ENTITY,LAYER,REGION,FINISHED,MAX_CONFIGURATION_STATE}}
};


static bool openFile(char *configurationFile);
static bool convertStringToInt(char *stringInt, int *returnValue);
static bool parseLine(char *line);

/*! 
 *  Requirement 1: If initialisation of configuration data fails then the software shall abort.

 * @param 
 * @return boolean failure indication (true = failure, false = success) 
 * */
bool configure(char *configurationFile) {
    if (openFile(configurationFile)!=true) {
        return false;
    } else {
        printf("Configuration failed\r\n");
        return true;
    }
}

/*! 
 * 
 * @param 
 * @return boolean failure indication (true = failure, false = success) 
 * */
static bool openFile(char *configurationFile) {
    FILE *file;
    char buffer[256];
    char *newLine;
    bool failure = false;

    file = fopen(configurationFile, "r");

    if (file == NULL) {
        perror("Error opening file for reading");
        failure = true;
    } else {

        newLine = fgets(buffer, (int)sizeof(buffer), file);
        if (newLine==NULL) {
            failure = true;
        }
        while (( newLine != NULL) && (failure == false)) {
            failure = failure | parseLine(buffer);
            newLine = fgets(buffer, (int)sizeof(buffer), file);
        }

        if (fclose(file) != 0) {
            perror("Error closing file after reading");
            failure = true;
        }
    }
    return failure;
}

/*! 
 * 
 * @param 
 * @return boolean failure indication (true = failure, false = success) 
 * */
static bool parseLine(char *line) {

    static int regionIndex;
    static region *r = NULL;
    int layer,mapping,inputPort,entity;
    static int portFromLayer;

    bool failure = true;
    const char SPLIT = '\t';
    int dataInt=0;
    char *dataStr;
    char *endOfLine; 
    static configurationStateEnum previousState = NOT_STARTED;
    configurationStateEnum nextState = NOT_STARTED;
    configurationStateEnum nextStateArrayIterator = NOT_STARTED;
    int nextStateFound = 1;

    endOfLine = strchr(line,'\n');
    if (endOfLine != NULL) {
        *endOfLine = '\0';
    }
    
    /* Find tab */
    dataStr = strchr(line,SPLIT);
    if (dataStr != NULL) {
        /* Replace tab with null terminator */
        *dataStr = '\0';
        dataStr++;
        /* data now points at the data item, line points at the token. Both data and token are null terminated */
        while( (nextStateArrayIterator < MAX_CONFIGURATION_STATE) && (nextStateFound != 0)) {
            nextStateFound = strcmp(configurationStateTokens[configurationStates[previousState].nextStates[nextStateArrayIterator]], line);
            nextStateArrayIterator++;
        }
        if (nextStateFound == 0) {
            /* Convert count of state (1 based) back to index to state (0 based) */
            nextStateArrayIterator = nextStateArrayIterator - 1;
            nextState = configurationStates[previousState].nextStates[nextStateArrayIterator];
            failure = convertStringToInt(dataStr, &dataInt);
            if (failure == false) {
                if ( (dataInt >= configurationStates[nextState].dataLowerLimit) && (dataInt <= configurationStates[nextState].dataUpperLimit ) ){
                    /* Token found and data in range */
                    //printf("%s\t%i\r\n",line,dataInt);
                    /* initialise structure and move on to next state */
                    switch (nextState) {
                        case (NOT_STARTED):
                            /*  No states currently have NOT_STARTED as an exit state so will never get in here.
                                Could be used in future, for example if wanting to reconfigure without closing the program.
                                In here we would memset the whole of the regions array on word to 0 so it's ready to reconfigure. */
                        break;

                        case (REGION):
                            regionIndex = dataInt;
                            r = &regions[regionIndex];

                        break;

                        case (LAYER):
                            if (r != NULL) {
                                /* Count of layers is 1 based */
                                r->layerCount = dataInt + 1;
                            } else {
                                failure = true;
                            }
                        break;

                        case (MAPPING):
                            if (r != NULL) {
                                /* Index to layer is zero based */
                                layer = r->layerCount - 1;
                                r->layers[layer].mappingCount = dataInt + 1;
                            } else {
                                failure = true;
                            }
                        break;

                        case (INPUT):
                            if (r != NULL) {
                                layer = r->layerCount - 1;
                                mapping = r->layers[layer].mappingCount - 1;
                                r->layers[layer].mappings[mapping].inputPortCount = dataInt + 1;
                            } else {
                                failure = true;
                            }
                        break;

                        case (INPUT_FROM_LAYER):
                            portFromLayer = dataInt;
                        break;

                        case (INPUT_FROM_ENTITY):
                            if (r != NULL) {
                                bool *inputPortBool;
                                layer = r->layerCount - 1;
                                mapping = r->layers[layer].mappingCount - 1;
                                inputPort = r->layers[layer].mappings[mapping].inputPortCount - 1;
                                if (portFromLayer == layer) {
                                    /* If in same layer, need to use previous output as current cycle not caluclated yet */
                                    inputPortBool = &r->layers[portFromLayer].entities[dataInt].previousOutput;
                                } else {
                                    /* Entity from any other layer can use output.
                                    for Entities in layers above, we get their latest output
                                    for entities in layers below, we get their previous output */
                                    inputPortBool = &r->layers[portFromLayer].entities[dataInt].output;
                                }
                                r->layers[layer].mappings[mapping].inputPorts[inputPort].p = inputPortBool;
                        } else {
                            failure = true;
                        }
                        break;

                        case (INPUT_INVERT):
                            if (r != NULL) {
                                layer = r->layerCount - 1;
                                mapping = r->layers[layer].mappingCount - 1;
                                inputPort = r->layers[layer].mappings[mapping].inputPortCount - 1;
                                r->layers[layer].mappings[mapping].inputPorts[inputPort].invert = (bool)dataInt;
                                r->layers[layer].mappings[mapping].inputPorts[inputPort].enabled = true;
                            } else {
                                failure = true;
                            }
                        break;

                        case (TYPE):
                            if (r != NULL) {
                                layer = r->layerCount - 1;
                                mapping = r->layers[layer].mappingCount - 1;
                                r->layers[layer].mappings[mapping].type = (mapType)dataInt;
                            } else {
                                failure = true;
                            }
                        break;

                        case(OUTPUT_TO_ENTITY):
                            if (r != NULL) {
                                bool *outputPortBool;
                                layer = r->layerCount - 1;
                                mapping = r->layers[layer].mappingCount - 1;
                                outputPortBool = &r->layers[layer].entities[dataInt].input;
                                r->layers[layer].mappings[mapping].outputPort.p = outputPortBool;
                            } else {
                                failure = true;
                            }
                        break;

                        case(OUTPUT_INVERT):
                            if (r != NULL) {
                                layer = r->layerCount - 1;
                                mapping = r->layers[layer].mappingCount - 1;
                                r->layers[layer].mappings[mapping].outputPort.invert = (bool)dataInt;
                                r->layers[layer].mappings[mapping].outputPort.enabled = true;
                            } else {
                                failure = true;
                            }
                        break;

                        case(ENTITY):
                            if (r != NULL) {
                                layer = r->layerCount - 1;
                                r->layers[layer].entityCount = dataInt + 1;
                                /* Not ideal doing this.. Need to rethink the initialise routines for entity (and region, and layer..) */
                                r->layers[layer].entities[dataInt].firstPass = true;
                            } else {
                                failure = true;
                            }
                        break;

                        case(INTEGRATION_TIME):
                            if (r != NULL) {
                                layer = r->layerCount - 1;
                                entity = r->layers[layer].entityCount - 1;
                                r->layers[layer].entities[entity].INTEGRATION_TIME = dataInt;
                            } else {
                                failure = true;
                            } 
                        break;

                        case(FLIP_TIME):
                            if (r != NULL) {
                                layer = r->layerCount - 1;
                                entity = r->layers[layer].entityCount - 1;
                                r->layers[layer].entities[entity].FLIP_TIME = dataInt;
                                printRegionState(r);
                            } else {
                                failure = true;
                            }
                        break;

                        case(FINISHED):
                            /*  The config file currently does not have a FINISHED state.                            
                                Could be used in future, for example if wanting to reconfigure without closing the program.
                                However as there is no state after finished in the configStates array,
                                the range check of data against dataLowLimit and dataUpperLimit will try to access limits tat are not initialised. */
                        break;

                        case(MAX_CONFIGURATION_STATE):
                        default:
                            failure = true;
                        break;
                    }

                    /* End of initialisation */
                    previousState = nextState;
                } else {
                    printf("Range check failed for %s. Data %i, lower %i, upper %i\r\n", line,dataInt, configurationStates[nextState].dataLowerLimit, configurationStates[nextState].dataUpperLimit);
                }
            }
        }
    }
    return failure;
}

/*! 
 * 
 * @param 
 * @return boolean failure indication (true = failure, false = success) 
 * */
static bool convertStringToInt(char *stringInt, int *returnValue) {
    #define MAX_CHARACTER (3) /* Currently no need to support data values bigger than 999 */
    const int powersOfTen[MAX_CHARACTER] = {1,10,100};
    bool failure = false;
    int characterCount = 0;

    while ( (*stringInt != '\0') && (failure == false) && (characterCount < MAX_CHARACTER) ) {
        if ( (*stringInt < '0') || (*stringInt >  '9') ) {
            failure = true;
        } else {
            *stringInt = *stringInt - (char)0x30; /* Convert ascii to decimal */
            characterCount++;
            stringInt++;
        }
    }
    if (failure == false) {
        stringInt = stringInt-1;
        *returnValue = 0;
        while (characterCount > 0) {
            *returnValue = *returnValue + ((int)*stringInt * powersOfTen[characterCount-1]);
            stringInt--;
            characterCount--;
        }
    }
    return failure;
}