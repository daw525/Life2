#include <windows.h>
#include "world.h"
#include "entity.h"
#include "mapping.h"

#define MAX_ITERATION   (15)

region regions[MAX_REGION];

#define MAX_INPUT_TO_WORLD (2)
bool inputs[MAX_INPUT];

#define MAX_OUTPUT_FROM_WORLD (1)
bool outputs[MAX_OUTPUT];

static void updateRegions(void);

/*!
 * If initialisation of configuration data fails then the software shall abort.
 *
 * @param none
 * @return bool 0 = success; 1 = failure
 */

bool initialiseRegions(void) {
    bool initalisationFailure;

    /*
    MAP 1 is an OR gate. If IN 1 is set then removed, could we get it to oscilate
    [IN 1]
        |
        -->[MAP 1]<----
               |      |
           [ENTITY 1] |
                |     |
             [MAP 2]  |
                |     |
            [ENTITY 2]-    
                |  
    */


    /* Region 0 */
    initaliseRegion(&regions[0]);

    regions[0].inputCount = 1;
    regions[0].inputArray[0] = false;

    regions[0].outputCount = 1;
    regions[0].outputArray[0] = false;

    /* Layer 1 */
    initalisationFailure = addLayerToRegion(&regions[0]);
    if (initalisationFailure!=false) {return true;}

    /* Mapping 1 */
    initalisationFailure = addMappingToRegionLayer(&regions[0],OR,&regions[0].layers[0].entities[0].input,false);
    if (initalisationFailure!=false) {return true;}

    initalisationFailure = addInputPortToMapping(&regions[0].layers[0].mappings[0],&regions[0].inputArray[0],false);
    if (initalisationFailure!=false) {return true;}

    initalisationFailure = addInputPortToMapping(&regions[0].layers[0].mappings[0],&regions[0].layers[1].entities[0].output,false);
    if (initalisationFailure!=false) {return true;}

    /* Entity 1 */

    initalisationFailure = addEntityToRegionLayer(&regions[0],3,20);
    if (initalisationFailure!=false) {return true;}

    /* Layer 2 */
    initalisationFailure = addLayerToRegion(&regions[0]);
    if (initalisationFailure!=false) {return true;}

    /* Mapping */

    initalisationFailure = addMappingToRegionLayer(&regions[0],OR,&regions[0].layers[1].entities[0].input,false);
    if (initalisationFailure!=false) {return true;}

    initalisationFailure = addInputPortToMapping(&regions[0].layers[1].mappings[0],&regions[0].layers[0].entities[0].output,false);
    if (initalisationFailure!=false) {return true;}

    /* Entity */
    
    initalisationFailure = addEntityToRegionLayer(&regions[0],3,20);
    if (initalisationFailure!=false) {return true;}

    return false;
}

void updateRegions(void) {
    int r;
    for (r=0;r<1;r++) {
        processRegion(&regions[r]);
        //printRegionState(&regions[r]);
    }
}


void tick(void) {
    updateRegions();
}


void run(void) {
    int i;
    regions[0].inputArray[0] = true;
    for(i=0;i<MAX_ITERATION;i++) {
        tick();
        Sleep(1);
        /* Put input 1 true first pass only */
        regions[0].inputArray[0] = false;
        /* Basic test case. Toggle inputs on on and even cycles */
        //if (i%1==0) {regions[0].inputArray[0]^=true;}
        //if (i%2==0) {regions[0].inputArray[1]^=true;}
    }
}