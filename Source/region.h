#ifndef _REGION_H_
#define _REGION_H_

#include "global.h"
#include "entity.h"
#include "mapping.h"

#define MAX_INPUT   (5)
#define MAX_OUTPUT  (5)
#define MAX_LAYER   (2)
#define MAX_ENTITY  (2)
#define MAX_MAPPING (2)

typedef struct {
    int entityCount;
    entity entities[MAX_ENTITY];
    int mappingCount;
    mapping mappings[MAX_MAPPING];
} layer;

typedef struct {
    int inputCount;
    bool inputArray[MAX_INPUT];
    int layerCount;
    layer layers[MAX_LAYER];
    int outputCount;
    bool outputArray[MAX_OUTPUT];
} region;

void processRegion(region *r);
void printRegionState(region *r);

#endif /*_REGION_H_*/