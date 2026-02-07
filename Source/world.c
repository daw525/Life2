#include <windows.h>
#include "world.h"
#include "entity.h"
#include "mapping.h"

#define MAX_ITERATION   (15)

world w;

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