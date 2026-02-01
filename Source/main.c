#include "global.h"
#include "world.h"
#include "windows.h"

#define MAX_ITERATION   (15)

/*! Main function description
 * 
 * @param none
 * @return none
 * */
void main(void) {
    uint8_t i;
    initialiseRegions();
    for(i=0;i<MAX_ITERATION;i++) {
        tick();
        Sleep(1);
    }
}