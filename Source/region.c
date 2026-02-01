#include "region.h"

void printRegionState(region *r) {
    int layer, entity, mapping;
    static bool firstPass = true;
    //printf("inputCount: %d\r\n",r->inputCount);
    //bool inputArray[MAX_INPUT];
    //printf("layerCount: %d\r\n",r->layerCount);
    for (layer=0;layer<r->layerCount;layer++) {
        //printf("Layer: %d\r\n", layer);
        //printf("entityCount: %d\r\n",r->layers[layer].entityCount);
        for (entity=0;entity<r->layers[layer].entityCount;entity++) {
            //printf("Entity: %d\r\n",entity);
            printEntityState(0,entity,&r->layers[layer].entities[entity],firstPass,true);
            firstPass = false;
        }
        //firstPass = true;
        for (mapping=0;mapping<r->layers[layer].mappingCount;mapping++) {
           //Mapping: %d\r\n",mapping);
            //printMappingState(&r->layers[layer].mappings[mapping]);
        }
     }
    //layer layers[MAX_LAYER];
    //printf("outputCount: %d\r\n",r->outputCount);
    //bool outputArray[MAX_OUTPUT];
}

void initaliseRegion(region *r) {
    r->inputCount=0;
    r->layerCount=0;
    r->outputCount=0;
}

bool addLayerToRegion(region *r) {
    if (r->layerCount >= MAX_LAYER) {
        return false;
    } else {
        r->layerCount++;
        return true;
    }
}

bool addEntityToRegionLayer(region *r, int integrationTime, int flipTime) {
    int currentLayer = r->layerCount-1; /* Minus one because it got incremented after new layer added */
    int currentEntitity = r->layers[currentLayer].entityCount;
    if (currentEntitity >= MAX_ENTITY) {
        return false;
    } else {
        initialiseEntity(&r->layers[currentLayer].entities[currentEntitity],integrationTime,flipTime);
        r->layers[currentLayer].entityCount++;
        return true;
    }
}

bool addMappingToRegionLayer(region *r, mapType type, entity *output, bool invertOutput) {
    int currentLayer = r->layerCount-1;  /* Minus one because it got incremented after new layer added */
    int currentMapping = r->layers[currentLayer].mappingCount;
    if (currentMapping >= MAX_MAPPING) {
        return false;
    } else {
        initialiseMapping(&r->layers[currentLayer].mappings[currentMapping],type,output,invertOutput);
        r->layers[currentLayer].mappingCount++;
        return true;
    }
}

void processRegion(region *r) {
    #define VERBOSE (true)
    static bool firstPass=true;
    static int time=0;

    int layer, entity, mapping;
    for (layer=0;layer<r->layerCount;layer++) {
        for (entity=0;entity<r->layers[layer].entityCount;entity++) {
            processEntity(&r->layers[layer].entities[entity]);
            //printEntityState(time,entity,&r->layers[layer].entities[entity],firstPass,VERBOSE);
            firstPass = false;
        }
        for (mapping=0;mapping<r->layers[layer].mappingCount;mapping++) {
            evaluateMapping(&r->layers[layer].mappings[mapping]);
            //printMappingState(&r->layers[layer].mappings[mapping]);
        }
    }
    printRegionState(r);

    time++;
}