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

static void tick(void);
static void updateRegions(void);

/*! 
 * 
 * @param 
 * @return None
 * */
void run(void) {
    int iteration;

    for(iteration=0; iteration<MAX_ITERATION; iteration++) {
        tick();
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
    int r;
    /* Only one region for now...*/
    for (r=0;r<1;r++) {
        processRegion(&regions[r]);
    }
}