#include "world.h"
#include "entity.h"
#include "mapping.h"

region regions[MAX_REGION];

static void updateRegions(void);

/*!
 * If initialisation of configuration data fails then the software shall abort.
 *
 * @param none
 * @return bool 0 = success; 1 = failure
 */

bool initialiseRegions(void) {
    bool initalisationFailure;
    /* Region 0 */
    initaliseRegion(&regions[0]);

    regions[0].inputCount = 2;
    regions[0].inputArray[0] = false;
    regions[0].inputArray[1] = false;

    regions[0].outputCount = 2;
    regions[0].outputArray[0] = false;
    regions[0].outputArray[1] = false;

    initalisationFailure = addLayerToRegion(&regions[0]);
    if (initalisationFailure!=false) {return true;}

    initalisationFailure = addEntityToRegionLayer(&regions[0],5,5);
    if (initalisationFailure!=false) {return true;}

    initalisationFailure = addEntityToRegionLayer(&regions[0],5,5);
     if (initalisationFailure!=false) {return true;}

    initalisationFailure = addMappingToRegionLayer(&regions[0],OR,&regions[0].layers[0].entities[0],false);
    if (initalisationFailure!=false) {return true;}

    initalisationFailure = addInputPortToMapping(&regions[0].layers[0].mappings[0],&regions[0].layers[0].entities[1],false);
    if (initalisationFailure!=false) {return true;}

    initalisationFailure = addMappingToRegionLayer(&regions[0],OR,&regions[0].layers[0].entities[1],false);
    if (initalisationFailure!=false) {return true;}

    initalisationFailure = addInputPortToMapping(&regions[0].layers[0].mappings[1],&regions[0].layers[0].entities[0],false);
    if (initalisationFailure!=false) {return true;}

    /* Region 1 */
    // initaliseRegion(&regions[1]);
    
    // regions[1].inputCount = 1;
    // regions[1].inputArray[0] = false;

    // regions[1].outputCount = 1;
    // regions[1].outputArray[0] = false;

    // addLayerToRegion(&regions[1]);
    // addEntityToRegionLayer(&regions[1],5,5);
    // addMappingToRegionLayer(&regions[1],OR,&regions[0].inputArray[0],false);
    // addInputPortToMapping(&regions[1].layers[0].mappings[0],&regions[1].inputArray[0],false);

    return false;
}

void updateRegions(void) {
    int r;
    for (r=0;r<MAX_REGION;r++) {
        processRegion(&regions[r]);
        //printRegionState(&regions[r]);
    }
}

// Maintains array of entities
// #define MAX_ENTITY (2)
// entity entities[MAX_ENTITY];

// void initialiseEntities(void) {
//     uint8_t e;
//     for (e=0;e<MAX_ENTITY;e++) {
//         initialiseEntity(&entities[e], 5, 5);
//     }
//     initialiseMappings();
// }

// #define MAX_MAPPING (2)
// mapping mappings[MAX_MAPPING];

// void initialiseMappings(void) {
//     initialiseMapping(&mappings[0],AND,&entities[1].previousOutput,false);
//     addInputPortToMapping(&mappings[0],&entities[0],false);

//     initialiseMapping(&mappings[1],OR,&entities[0],false);
//     addInputPortToMapping(&mappings[1],&entities[1],false);

// }

// Generates tick rate
// With each tick, iterate over array of entities and call their process operation
// Ouput is the "current" output from all entities. Print this to screen.

// void updateEntities(void) {
//     uint8_t m;
//     for(m=0;m<MAX_MAPPING;m++) {
//         evaluateMapping(&mappings[m]);
//         //printMappingState(&mappings[m]);
//     }
// }

void tick(void) {
    updateRegions();
}
