#include "global.h"
#include "world.h"
#include "configure.h"

/*! Main function description
 * 
 * @param none
 * @return int: EXIT_FAILURE or EXIT_SUCCESS
 * */
int main(void) {
    bool initalisationFailed;

    //return configure("configuration.csv");
    
    initalisationFailed = initialiseRegions();
    if (initalisationFailed != false) {
        return EXIT_FAILURE;
    } else {
        run();
        return EXIT_SUCCESS;
    }
}