#include "region.h"

/*! 
 * 
 * @param 
 * @return None
 * */
void processRegion(region *r) {
    #define VERBOSE (false)
    static bool firstPass=false;
    static int time=0;

    int layer, entity, mapping;
    
    for (layer=0;layer<r->layerCount;layer++) {
        for (mapping=0;mapping<r->layers[layer].mappingCount;mapping++) {
            evaluateMapping(&r->layers[layer].mappings[mapping]);
            //printMappingState(&r->layers[layer].mappings[mapping]);
        }
        for (entity=0;entity<r->layers[layer].entityCount;entity++) {
            processEntity(&r->layers[layer].entities[entity]);
            //printEntityState(time,entity,&r->layers[layer].entities[entity],firstPass,VERBOSE);
            firstPass = false;
        }
    }
    time++;
}

/*! 
 * 
 * @param 
 * @return None
 * */
void printRegionState(region *r) {
    int layer, entity, mapping, port;
    static bool firstPass = true;
    printf("inputCount: %d\r\n",r->inputCount);
    for (port=0;port<r->inputCount;port++) {
        printf("in[%i]:%d\t", port, r->inputArray[port]);
    }
    printf("\n");
    printf("layerCount: %d\r\n",r->layerCount);
    for (layer=0;layer<r->layerCount;layer++) {
        printf("Layer: %d\r\n", layer);
        printf("entityCount: %d\r\n",r->layers[layer].entityCount);
         for (mapping=0;mapping<r->layers[layer].mappingCount;mapping++) {
            printf("Mapping: %d\r\n",mapping);
            //printMappingState(&r->layers[layer].mappings[mapping]);
        }
        for (entity=0;entity<r->layers[layer].entityCount;entity++) {
            printf("Entity: %d\r\n",entity);
            //printEntityState(0,entity,&r->layers[layer].entities[entity],firstPass,true);
            firstPass = false;
        }

     }
    printf("outputCount: %d\r\n",r->outputCount);
    for (port=0;port<r->outputCount;port++) {
        printf("out[%i]:%d\t", port, r->outputArray[port]);
    }
    printf("\n");
}