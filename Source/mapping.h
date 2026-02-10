#ifndef _MAPPING_H_
#define _MAPPING_H_

#include "global.h"
#include "entity.h"

#define MAX_INPUT_PORT    (5)

typedef enum {
    THRESHOLD,
    MAX_MAP_TYPE
} mapType;

typedef struct {
    bool *p;
    bool invert;
} port;

typedef struct {
    int inputPortCount;
    port inputPorts[MAX_INPUT_PORT];
    port outputPort;
    mapType type;
    bool evaluation;
    int count;
    int onThreshold;
    int offThreshold;
} mapping;

bool initialisePort(port *p, bool *b, const bool invert);
bool initialiseMapping(mapping *m, const mapType t);
bool addPortToMapping(mapping *m, const port *p, const bool inputPort);
bool setMappingThresholds(mapping *m, const int onThreshold, const int offThreshold);
bool evaluateMapping(mapping *m);
void printMappingState(mapping *m);

#endif /* _MAPPING_H_ */