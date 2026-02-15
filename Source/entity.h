#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "global.h"

#define MAX_SAMPLE (10)
#define MAX_FLIP_TIME   (20)

typedef enum {
    FALSE_SAMPLE = 0,
    TRUE_SAMPLE = 1,
    NO_SAMPLE = 3
} sample;

typedef struct {
    bool input;
    bool output;
    sample inputs[MAX_SAMPLE];
    sample outputs[MAX_SAMPLE];
    int INTEGRATION_TIME;
    int integrationTime;
    float inputTimeTrue;
    float outputTimeTrue;
    float outputTimeFalse;
    bool firstPass; 
} entity;

void initialiseEntity(entity *e, int integrationTime);
void processEntity(entity *e);
void printEntityState(int time, int identifier, entity *e, bool withHeader, bool verbose);

#endif /*_ENTITY_H_*/