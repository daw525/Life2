#include "configure.h"
#include "world.h"
#include "region.h"
#include <string.h>

const char REGION_TOK[] = "REGION";
const char INPUT_TOK[] = "INPUT";
const char LAYER_TOK[] = "LAYER";
const char MAPPING_TOK[] = "MAPPING";
const char ENTITY_TOK[] = "ENTITY";
const char OUTPUT_TOK[] = "OUTPUT";


typedef enum {
    REGION,
    INPUT,
    LAYER,
    MAPPING,
    ENTITY,
    OUTPUT,
} configurationFileState;

bool parseLine(char *line) {
    static configurationFileState state = REGION;
    static int numberOfRegions = 0;
    static int numberOfInputs = 0;
    static int numberOfLayers = 0;
    static int numberOfMappings = 0;
    static int numberOfEntities = 0;
    static int integrationTime = 0;
    static int flipTime = 0;
    static int numberOfOutputs = 0;

    char *nextTab;
    char *nextTok;
    char *newLine;
    bool failure = false;
    int tokenFound;
    
    int i;

    newLine = strchr(line,'\n');
    if (newLine != NULL) {
        *newLine = '\0';
    }

    switch (state) {
        case REGION: {
            nextTab = strchr(line, '\t');
            if (nextTab != NULL) {
                *nextTab = '\0';
                tokenFound = strcmp(REGION_TOK,line);
                if (tokenFound == 0) {
                    numberOfRegions++;
                    state = INPUT;
                    failure = false;
                } else {
                    /* Token not found */
                    failure = true;
                }
            } else {
                /* No tab found */
                failure = true;
            }
            break;
        }
        case INPUT: {
            nextTab = strchr(line, '\t');
            if (nextTab != NULL) {
                *nextTab = '\0';
                tokenFound = strcmp(INPUT_TOK,line);
                if (tokenFound == 0) {
                    nextTab++;
                    numberOfInputs = atoi(nextTab);
                    if ((numberOfInputs <= 0) || (numberOfInputs > 1)) {
                        /* Range check set to 1 for now. TBD */
                        failure = true;
                    } else {
                        state = LAYER;
                        failure = false;
                    }
                } else {
                    /* Token not found */
                    failure = true;
                }
            } else {
                /* No tab found */
                failure = true;
            }
            break;
        }
        case LAYER: {
            nextTab = strchr(line, '\t');
            if (nextTab != NULL) {
                *nextTab = '\0';
                tokenFound = strcmp(LAYER_TOK,line);
                if (tokenFound == 0) {
                    numberOfLayers++;
                    state = MAPPING;
                    failure = false;
                } else {
                    /* Token not found */
                    failure = true;
                }
            } else {
                /* No tab found */
                failure = true;
            }
            break;
        }
        case MAPPING: {
            nextTab = strchr(line, '\t');
            if (nextTab != NULL) {
                *nextTab = '\0';
                tokenFound = strcmp(MAPPING_TOK,line);
                if (tokenFound == 0) {
                    numberOfMappings++;
                    state = ENTITY;
                    failure = false;
                } else {
                    /* Token not found */
                    failure = true;
                }
            } else {
                /* No tab found */
                failure = true;
            }
            break;
        }
        case ENTITY: {
            nextTab = strchr(line, '\t');
            if (nextTab != NULL) {
                *nextTab = '\0';
                tokenFound = strcmp(ENTITY_TOK,line);
                if (tokenFound == 0) {
                    numberOfEntities++;
                    nextTok = nextTab;
                    nextTok++;
                    nextTab = strchr(nextTok,'\t');
                    *nextTab = '\0';
                    integrationTime = atoi(nextTok);
                    if ((integrationTime <= 0) || (integrationTime > 10)) {
                            /* Range check set to 10 for now. TBD */
                        failure = true;
                    } else {
                        nextTok = nextTab;
                        nextTok++;
                        flipTime = atoi(nextTok);
                        if ((flipTime <= 0) || (flipTime > 10)) {
                            /* Range check set to 10 for now. TBD */
                            failure = true;
                        } else {
                            /* TODO: stay in here until all entities parsed */
                            state = OUTPUT;
                            failure = false;
                        }

                    }
                } else {
                    /* Token not found */
                    failure = true;
                }
            } else {
                /* No tab found */
                failure = true;
            }
            break;
        }
        case OUTPUT: {
            nextTab = strchr(line, '\t');
            if (nextTab != NULL) {
                *nextTab = '\0';
                tokenFound = strcmp(OUTPUT_TOK,line);
                if (tokenFound == 0) {
                    nextTab++;
                    numberOfOutputs = atoi(nextTab);
                    if ((numberOfOutputs <= 0) || (numberOfOutputs > 1)) {
                        /* Range check set to 1 for now. TBD */
                        failure = true;
                    } else {
                        /* Next transition? This can't be called again as either configured and running or aborted */
                        initaliseRegion(&regions[numberOfRegions-1]);
                        regions[numberOfRegions-1].inputCount = numberOfInputs;
                        for (i=0;i<numberOfInputs;i++) {
                            regions[numberOfRegions-1].inputArray[i] = false;

                        }
                        regions[numberOfRegions-1].outputCount = numberOfOutputs;
                        for (i=0;i<numberOfOutputs;i++) {
                            regions[numberOfRegions-1].outputArray[i] = false;    
                        }
                        failure = addLayerToRegion(&regions[numberOfRegions-1]);
                        
                        if (failure!=true) {
                            failure = addEntityToRegionLayer(&regions[numberOfRegions-1],integrationTime,flipTime);
                            if (failure!=true) {

                                /* Need to add mappings here. Since only one entity is parsed from the config file, mapping is not possible yet.*/
                            
                            }/* failure was true so will propgate up and abort */
                        } /* failure was true so will propgate up and abort */
                    }
                } else {
                    /* Token not found */
                    failure = true;
                }
            } else {
                /* No tab found */
                failure = true;
            }
            break;
        }
        default:
           break;
    }
        // printf("\r\nState: %d\r\n",state);
        // printf("%d\r\n",numberOfRegions);
        // printf("%d\r\n",numberOfInputs);
        // printf("%d\r\n",numberOfLayers);
        // printf("%d\r\n",numberOfMappings);
        // printf("%d\r\n",numberOfEntities);
        // printf("%d\r\n",integrationTime);
        // printf("%d\r\n",flipTime);
        // printf("%d\r\n",numberOfOutputs);
    return failure;
}

bool openFile(char *configurationFile) {
    FILE *file;
    char buffer[256];
    char *newLine;
    bool failure = false;

    file = fopen(configurationFile, "r");

    if (file == NULL) {
        perror("Error opening file for reading");
        failure = true;
    }

    newLine = fgets(buffer, sizeof(buffer), file);
    if (newLine==NULL) {
        failure = true;
    }
    while (newLine != NULL) {
        failure = failure | parseLine(buffer);
        newLine = fgets(buffer, sizeof(buffer), file);
    }

    if (fclose(file) != 0) {
        perror("Error closing file after reading");
        failure = true;
    }

    return failure;
}


bool configure(char *configurationFile) {
    if (openFile(configurationFile)!=true) {
        printRegionState(&regions[0]);
        return false;
    } else {
        printf("Configuration failed\r\n");
        return true;
    }
}