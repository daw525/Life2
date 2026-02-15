#include "./Unity/unity.h"
#include "../Source/region.h"
#include <string.h>

#define MAX_MESSAGE     (128)
#define MAX_TEST_CASE    (20)

typedef struct {
    bool testEnabled;
    char message[MAX_MESSAGE];
    region r;
} testCase;


void initialiseLayer(layer *l) {

}

void initialiseRegion(region *r) {
    memset(r,0,sizeof(r));
}

void initialiseTestCase(testCase *t, char *message) {
    port tempPort;
    int currentLayer, currentMapping, currentEntity, currentPort;
    t->testEnabled=true;
    sprintf(t->message,message);
    initialiseRegion(&t->r);

    /* Set up region inputs */
    t->r.inputCount++;

    /* Add layer */
    currentLayer = t->r.layerCount;
    initialiseLayer(&t->r.layers[currentLayer]);

    /* Add mappings */
    currentMapping = t->r.layers[currentLayer].mappingCount;

    initialiseMapping(&t->r.layers[currentLayer].mappings[currentMapping],THRESHOLD);
    /* Set input ports */
    initialisePort(&tempPort,&t->r.inputArray[0],false);
    addPortToMapping(&t->r.layers[currentLayer].mappings[currentMapping],&tempPort,true);
   /* Set output port */
    initialisePort(&tempPort,&t->r.layers[currentLayer].entities[0].input,false);
    addPortToMapping(&t->r.layers[currentLayer].mappings[currentMapping],&tempPort,false);

    t->r.layers[currentLayer].mappingCount++;
    currentMapping = t->r.layers[currentLayer].mappingCount;
    /* Next mapping */

    /* Add entities */
    currentEntity = t->r.layers[currentLayer].entityCount;
    initialiseEntity(&t->r.layers[currentLayer].entities[currentEntity],1);

    /* Next entity */
    t->r.layers[currentLayer].entityCount++;
    currentEntity = t->r.layers[currentLayer].entityCount;

    /* Next layer */
    t->r.layerCount++;
    currentLayer = t->r.layerCount;

}

void setUp(void) {
     
}

void tearDown(void) {

}

void test_processRegion(void) {
    testCase tests[MAX_TEST_CASE];
    int testCase;

    testCase = 0;
    initialiseTestCase(&tests[testCase],"Setting up test framework for region\0");

    for(testCase=0;testCase<MAX_TEST_CASE;testCase++) {
        if (tests[testCase].testEnabled == true) {

            processRegion(&tests[testCase].r);
            printRegionState(&tests[testCase].r);
            //TEST_ASSERT_EQUAL_MESSAGE(tests[testCase].expectedOutput,tests[testCase].e.output,tests[testCase].message);
        }
    }
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_processRegion);
    return UNITY_END();
}