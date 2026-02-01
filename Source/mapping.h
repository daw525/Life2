#ifndef _MAPPING_H_
#define _MAPPING_H_

#include "global.h"
#include "entity.h"

#define MAX_INPUT_PORT    (5)

typedef enum {
    AND,
    OR,
    XOR,
    THRESHOLD
} mapType;

typedef struct {
    entity *e;
    bool invert;
    bool enabled;
} port;

typedef struct {
    int inputPortCount;
    port inputPorts[MAX_INPUT_PORT];
    port outputPort;
    mapType type;
    bool evaluation;
    int onThreshold;
    int offThreshold;
} mapping;

void initialiseMapping(mapping *m, mapType type, entity *output, bool invertOutput);
bool addInputPortToMapping(mapping *m, entity *input, bool invert);
void setMappingThresholds(mapping *m, int onThresdhold, int offThreshold);
void evaluateMapping(mapping *m);
void printMappingState(mapping *m);

#endif /* _MAPPING_H_ */