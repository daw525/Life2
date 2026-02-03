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
    /* Region 0 */
    initaliseRegion(&regions[0]);

    regions[0].inputCount = 2;
    regions[0].inputArray[0] = false;
    regions[0].inputArray[1] = false;

    regions[0].outputCount = 1;
    regions[0].outputArray[0] = false;

    initalisationFailure = addLayerToRegion(&regions[0]);
    if (initalisationFailure!=false) {return true;}

    initalisationFailure = addMappingToRegionLayer(&regions[0],OR,&regions[0].layers[0].entities[0].input,false);
    if (initalisationFailure!=false) {return true;}

    initalisationFailure = addInputPortToMapping(&regions[0].layers[0].mappings[0],&regions[0].inputArray[0],false);
    if (initalisationFailure!=false) {return true;}

    initalisationFailure = addInputPortToMapping(&regions[0].layers[0].mappings[0],&regions[0].inputArray[1],false);
    if (initalisationFailure!=false) {return true;}

    initalisationFailure = addEntityToRegionLayer(&regions[0],5,5);
    if (initalisationFailure!=false) {return true;}

    return false;
}

void updateRegions(void) {
    int r;
    for (r=0;r<MAX_REGION;r++) {
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
    regions[0].inputArray[1] = true;
    for(i=0;i<MAX_ITERATION;i++) {
        tick();
        Sleep(1);
        /* Basic test case. Toggle inputs on on and even cycles */
        if (i%1==0) {regions[0].inputArray[0]^=true;}
        if (i%2==0) {regions[0].inputArray[1]^=true;}
    }
}