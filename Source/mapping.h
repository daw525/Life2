#ifndef _MAPPING_H_
#define _MAPPING_H_

#include "global.h"
#include "entity.h"

#define MAX_INPUT_PORT    (5)

typedef enum {
    AND,
    OR,
    XOR,
    THRESHOLD,
    MAX_MAP_TYPE
} mapType;

typedef struct {
    bool *p;
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

void evaluateMapping(mapping *m);
void printMappingState(mapping *m);

#endif /* _MAPPING_H_ */