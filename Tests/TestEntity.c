#include "./Unity/unity.h"
#include "../Source/entity.h"
#include <string.h>

#define MAX_MESSAGE     (128)
#define MAX_TEST_CASE    (20)

typedef struct {
    bool testEnabled;
    char message[MAX_MESSAGE];
    bool expectedOutput;
    entity e;
} testCase;




void setUp(void) {

}

void tearDown(void) {

}

void initialiseTestCase(testCase *t, char *m, bool input, bool expectedOutput, int integrationTime, int flipTime) {
    int sample;
    t->testEnabled = true;
    sprintf(t->message, m);
    t->expectedOutput = expectedOutput;
    t->e.input = input;
    t->e.output = !(expectedOutput);    /* Initialise to inverse of expected so change can be observed */
    t->e.previousOutput = false;
    for(sample=0;sample<MAX_SAMPLE;sample++) {    
        t->e.inputs[sample] = NO_SAMPLE;
        t->e.outputs[sample] = NO_SAMPLE;
    }
    t->e.INTEGRATION_TIME = integrationTime;
    t->e.integrationTime = 0;
    t->e.inputTimeTrue = 0;
    t->e.outputTimeTrue = 0;
    t->e.outputTimeFalse = 0;
    t->e.error = 0;
    t->e.trueWeight = 0;
    t->e.falseWeight = 0;
    t->e.FLIP_TIME = flipTime;
    t->e.flipTime = 0;
    t->e.firstPass = true; 
}

void test_processEntityFirstPassBehaviour(void) {
    testCase tests[MAX_TEST_CASE];
    int testCase;

    memset(tests,0,sizeof(tests));
    
    testCase=0;
    initialiseTestCase(&tests[testCase],"Sample time 1, input true\0",true,true,1,10);
    initialiseTestCase(&tests[testCase++],"Sample time 1, input false\0",false,false,1,10);
    initialiseTestCase(&tests[testCase++],"Sample time 2, input true\0",true,true,2,10);
    initialiseTestCase(&tests[testCase++],"Sample time 2, input false\0",false,false,2,10);
    initialiseTestCase(&tests[testCase++],"Sample time 1, input true, flip time 1\0",true,true,1,1);
    initialiseTestCase(&tests[testCase++],"Sample time 1, input false, flip time 1\0",false,false,1,1);
    initialiseTestCase(&tests[testCase++],"Sample time 2, input true, flip time 1\0",true,true,2,1);
    initialiseTestCase(&tests[testCase++],"Sample time 2, input false, flip time 1\0",false,false,2,1);
    initialiseTestCase(&tests[testCase++],"Sample time 1, input true, flip time 0\0",true,false,1,0);
    initialiseTestCase(&tests[testCase++],"Sample time 1, input false, flip time 0\0",false,true,1,0);
    initialiseTestCase(&tests[testCase++],"Sample time 2, input true, flip time 0\0",true,false,2,0);
    initialiseTestCase(&tests[testCase++],"Sample time 2, input false, flip time 0\0",false,true,2,0);

    for(testCase=0;testCase<MAX_TEST_CASE;testCase++) {
        if (tests[testCase].testEnabled == true) {

            processEntity(&tests[testCase].e);

            TEST_ASSERT_EQUAL_MESSAGE(tests[testCase].expectedOutput,tests[testCase].e.output,tests[testCase].message);
        }
    }
}

void test_processEntityFiveCycles(void) {
    #define MAX_CYCLE (10)
    testCase test;
    int cycle;
    bool inputs[MAX_CYCLE] =            {true,   true,  true,   false,  false,  false,  true,   false,  true,   true};
    bool expectedOutput[MAX_CYCLE] =    {true,   true,  true,   true,   false,  false,  false,  false,  true,  true};
    
    initialiseTestCase(&test,"Sample time 2, input true\0",true,true,2,20);

    for(cycle=0;cycle<MAX_CYCLE;cycle++) {
            test.e.input = inputs[cycle];
            processEntity(&test.e);
            printEntityState(cycle,0,&test.e,cycle==0,true);
            //TEST_ASSERT_EQUAL_MESSAGE(expectedOutput[cycle],test.e.output,test.message);
            //TEST_ASSERT_FALSE(test.e.firstPass);
    }
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_processEntityFirstPassBehaviour);
    RUN_TEST(test_processEntityFiveCycles);
    return UNITY_END();
}