#include "region.h"

void printRegionState(region *r) {
    int layer, entity, mapping;
    static bool firstPass = true;
    printf("inputCount: %d\r\n",r->inputCount);
    printf("layerCount: %d\r\n",r->layerCount);
    for (layer=0;layer<r->layerCount;layer++) {
        printf("Layer: %d\r\n", layer);
        printf("entityCount: %d\r\n",r->layers[layer].entityCount);
        for (entity=0;entity<r->layers[layer].entityCount;entity++) {
            printf("Entity: %d\r\n",entity);
            //printEntityState(0,entity,&r->layers[layer].entities[entity],firstPass,true);
            firstPass = false;
        }
         for (mapping=0;mapping<r->layers[layer].mappingCount;mapping++) {
            printf("Mapping: %d\r\n",mapping);
            //printMappingState(&r->layers[layer].mappings[mapping]);
        }
     }
}

void initaliseRegion(region *r) {
    r->inputCount=0;
    r->layerCount=0;
    r->outputCount=0;
}
/*!
 *
 *
 * @param Pointer to region
 * @return 0 = success; 1 = failure
 */
bool addLayerToRegion(region *r) {
    if (r->layerCount >= MAX_LAYER) {
        return true;
    } else {
        r->layerCount++;
        return false;
    }
}

/*!
 *
 *
 * @param 
 * @return 0 = success; 1 = failure
 */
bool addEntityToRegionLayer(region *r, int integrationTime, int flipTime) {
    int currentLayer = r->layerCount-1; /* Minus one because it got incremented after new layer added */
    int currentEntitity = r->layers[currentLayer].entityCount;
    if (currentEntitity >= MAX_ENTITY) {
        return true;
    } else {
        initialiseEntity(&r->layers[currentLayer].entities[currentEntitity],integrationTime,flipTime);
        r->layers[currentLayer].entityCount++;
        return false;
    }
}

/*!
 *
 *
 * @param 
 * @return 0 = success; 1 = failure
 */
bool addMappingToRegionLayer(region *r, mapType type, bool *output, bool invertOutput) {
    int currentLayer = r->layerCount-1;  /* Minus one because it got incremented after new layer added */
    int currentMapping = r->layers[currentLayer].mappingCount;
    if (currentMapping >= MAX_MAPPING) {
        return true;
    } else {
        initialiseMapping(&r->layers[currentLayer].mappings[currentMapping],type,output,invertOutput);
        r->layers[currentLayer].mappingCount++;
        return false;
    }
}

void processRegion(region *r) {
    #define VERBOSE (true)
    static bool firstPass=true;
    static int time=0;

    int layer, entity, mapping;
    for (layer=0;layer<r->layerCount;layer++) {
        for (mapping=0;mapping<r->layers[layer].mappingCount;mapping++) {
            evaluateMapping(&r->layers[layer].mappings[mapping]);
            //printMappingState(&r->layers[layer].mappings[mapping]);
        }
        for (entity=0;entity<r->layers[layer].entityCount;entity++) {
            processEntity(&r->layers[layer].entities[entity]);
            printEntityState(time,entity,&r->layers[layer].entities[entity],firstPass,VERBOSE);
            firstPass = false;
        }
    }
    //printRegionState(r);

    time++;
}