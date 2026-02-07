#include <windows.h>
#include "world.h"
#include "entity.h"
#include "mapping.h"

#define MAX_ITERATION   (5)

world w;

#define MAX_INPUT_TO_WORLD (2)
bool inputs[MAX_INPUT];

#define MAX_OUTPUT_FROM_WORLD (1)
bool outputs[MAX_OUTPUT];

static void tick(void);
static void updateRegions(void);

typedef struct {
    bool SET;
    bool RESET;
} testCase;

testCase testCases[MAX_ITERATION] = {   {true, false},
                                        {true, false},
                                        {false, false},
                                        {false, true},
                                        {false, false}};

/*! 
 * 
 * @param 
 * @return None
 * */
void run(void) {
    int iteration;

    /* Re-map to region inputs and outputs */
    /* L0,E0 has SET input and feedback from L1,E0 entity to produce a latch (mapping is OR gate) */
    //w.regions[0].layers[0].mappings[0].inputPorts[0].p = &inputs[0];
    w.regions[0].layers[0].mappings[0].inputPorts[1].p = &w.regions[0].layers[1].entities[0].output;

    /* L1,E0 has RESET input and output taken from L0,E1 (mapping is AND gate with RESET input inverted)*/
    /* This makes SR Latch */

    for(iteration=0; iteration<MAX_ITERATION; iteration++) {
        w.regions[0].layers[0].mappings[0].inputPorts[0].p = &testCases[iteration].SET;
        w.regions[0].layers[1].mappings[0].inputPorts[0].p = &testCases[iteration].RESET;

        //printf("Iteration: %i\n",iteration);
        printf("SET: %d\n",(int)*w.regions[0].layers[0].mappings[0].inputPorts[0].p);
        printf("CLEAR: %d\n",(int)*w.regions[0].layers[1].mappings[0].inputPorts[0].p);
        tick();
        //printMappingState(&w.regions[0].layers[0].mappings[0]);
        //printf("\r\n");
        //printMappingState(&w.regions[0].layers[1].mappings[0]);
        //printEntityState(iteration,0,&w.regions[0].layers[0].entities[0],false,true);
        //printEntityState(iteration,1,&w.regions[0].layers[1].entities[0],false,true);
        printf("Output: %d\n",(int)w.regions[0].layers[1].entities[0].output);
        /*@-unrecog@*/
        Sleep(1);
    }
}

/*! 
 * 
 * @param 
 * @return None
 * */
static void tick(void) {
    updateRegions();
}

/*! 
 * 
 * @param 
 * @return None
 * */
static void updateRegions(void) {
    int regionIterator;
    region *r;
    for (regionIterator=0; regionIterator < w.regionCount; regionIterator++) {
        r = &w.regions[regionIterator];
        if (r != NULL) {
            processRegion(r);
        } else {
            printf("Null region encountered\r\n");
        }
    }
}