#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "global.h"

#define MAX_SAMPLE (10)

typedef struct {
    bool input;
    bool output;
    bool previousOutput;
    bool inputs[MAX_SAMPLE];
    bool outputs[MAX_SAMPLE];
    int INTEGRATION_TIME;
    int integrationTime;
    int inputTimeTrue;
    int outputTimeTrue;
    int outputTimeFalse;
    int error;
    float trueWeight;
    float falseWeight;
    int FLIP_TIME;
    int flipTime;    
} entity;

void initialiseEntity(entity *e, int integrationTime, int flipTime);
void processEntity(entity *e);
void printEntityState(int time, int identifier, entity *e, bool withHeader, bool verbose);



#endif /*_ENTITY_H_*/