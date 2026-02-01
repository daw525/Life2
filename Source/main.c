#include "global.h"
#include "world.h"
#include <windows.h>

#define MAX_ITERATION   (15)

/*! Main function description
 * 
 * @param none
 * @return int: EXIT_FAILURE or EXIT_SUCCESS
 * */
int main(void) {
    uint8_t i;
    bool initalisationFailed;

    initalisationFailed = initialiseRegions();
    if (initalisationFailed != false) {
        return EXIT_FAILURE;
    } else {
        for(i=0;i<MAX_ITERATION;i++) {
            tick();
            Sleep(1);
        }
        return EXIT_SUCCESS;
    }
}