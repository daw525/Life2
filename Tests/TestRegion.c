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
    t->r.inputCount = 2;

    /* Add layer */
    currentLayer = t->r.layerCount;
    initialiseLayer(&t->r.layers[currentLayer]);

    /* Add mappings */
    currentMapping = t->r.layers[currentLayer].mappingCount;

    initialiseMapping(&t->r.layers[currentLayer].mappings[currentMapping],THRESHOLD);
    /* Set input ports */

    initialisePort(&tempPort,&t->r.inputArray[0],false);    // set
    addPortToMapping(&t->r.layers[currentLayer].mappings[currentMapping],&tempPort,true);

    initialisePort(&tempPort,&t->r.layers[0].entities[0].output,false);
    addPortToMapping(&t->r.layers[currentLayer].mappings[currentMapping],&tempPort,true);

    /* Set output port */
    initialisePort(&tempPort,&t->r.layers[currentLayer].entities[0].input,true);    // invert output
    addPortToMapping(&t->r.layers[currentLayer].mappings[currentMapping],&tempPort,false);

    
    /* Set mapping threshold */
    setMappingThresholds(&t->r.layers[currentLayer].mappings[currentMapping],1,1);  // NOR gate

    /* Next mapping */
    t->r.layers[currentLayer].mappingCount++;
    currentMapping = t->r.layers[currentLayer].mappingCount;

    /* Add entities */
    currentEntity = t->r.layers[currentLayer].entityCount;
    initialiseEntity(&t->r.layers[currentLayer].entities[currentEntity],1);

    /* Next entity */
    t->r.layers[currentLayer].entityCount++;
    currentEntity = t->r.layers[currentLayer].entityCount;

    /* Next layer  ------------------------------------------------ */
    t->r.layerCount++;

    /* Add layer */
    currentLayer = t->r.layerCount;
    initialiseLayer(&t->r.layers[currentLayer]);

    /* Add mappings */
    currentMapping = t->r.layers[currentLayer].mappingCount;

    initialiseMapping(&t->r.layers[currentLayer].mappings[currentMapping],THRESHOLD);
    /* Set input ports */

    initialisePort(&tempPort,&t->r.inputArray[1],false);    // reset
    addPortToMapping(&t->r.layers[currentLayer].mappings[currentMapping],&tempPort,true);

    initialisePort(&tempPort,&t->r.layers[0].entities[0].output,false);
    addPortToMapping(&t->r.layers[currentLayer].mappings[currentMapping],&tempPort,true);

    /* Set output port */
    initialisePort(&tempPort,&t->r.outputArray[0],true);   // bounrdary
    addPortToMapping(&t->r.layers[currentLayer].mappings[currentMapping],&tempPort,false);

    
    /* Set mapping threshold */
    setMappingThresholds(&t->r.layers[currentLayer].mappings[currentMapping],1,1);  // NOR gate

    /* Next mapping */
    t->r.layers[currentLayer].mappingCount++;
    currentMapping = t->r.layers[currentLayer].mappingCount;

    /* Add entities */
    currentEntity = t->r.layers[currentLayer].entityCount;
    initialiseEntity(&t->r.layers[currentLayer].entities[currentEntity],1);

    /* Next entity */
    t->r.layers[currentLayer].entityCount++;
    currentEntity = t->r.layers[currentLayer].entityCount;

    /* Next layer */
    t->r.layerCount++;
    currentLayer = t->r.layerCount;

    /* Set up region outputs */
    t->r.outputCount = 2;

}

void setUp(void) {
     
}

void tearDown(void) {

}

void test_processRegion(void) {
    #define MAX_TEST    (20)
    testCase test;
    int cycle;
    memset(&test,0,sizeof(test));

    typedef struct {
        bool set;
        bool reset;
    } srLatch;

    srLatch srl[20] = {  {false, false},
                        {false,  false},
                        {false, false},
                        {false, false},
                        {false,  false},
                        {false, false}};

    initialiseTestCase(&test,"Setting up test framework for region\0");

    for(cycle=0;cycle<MAX_TEST_CASE;cycle++) {
            switch (cycle) {
                case 3:
                    test.r.inputArray[0] = true;
                break;
                case 7:
                    test.r.inputArray[1] = true;
                break;
                default:
                    test.r.inputArray[0] = false;
                    test.r.inputArray[1] = false;
                break;
            }
            printf("Cycle: %i\n",cycle);
            printf("Set: %d Clear: %d\n", test.r.inputArray[0],test.r.inputArray[1]);            
            processRegion(&test.r);
            //printRegionState(&tests[testCase].r);
            printf("Q: %d NotQ: %d\n\n", test.r.outputArray[0], test.r.layers[0].entities[0].output);
            //TEST_ASSERT_EQUAL_MESSAGE(tests[testCase].expectedOutput,tests[testCase].e.output,tests[testCase].message);
    }
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_processRegion);
    return UNITY_END();
}